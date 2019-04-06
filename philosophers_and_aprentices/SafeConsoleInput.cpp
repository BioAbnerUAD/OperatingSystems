#include "SafeConsoleInput.h"

#include <iostream>

#ifdef __unix__

#include <unistd.h>
#include <termios.h>

// no standard getch in linux. must define one using linux libraries
char _getch() {
  char buf = 0;
  struct termios old = { 0 };
  if (tcgetattr(0, &old) < 0)
    perror("tcsetattr()");
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0)
    perror("tcsetattr ICANON");
  if (read(0, &buf, 1) < 0)
    perror("read()");
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0)
    perror("tcsetattr ~ICANON");
  return (buf);
}

#elif defined(_WIN32) || defined(WIN32)

#include <conio.h> // for windows getch

#endif

void Console::Clear() {
  #if (defined(_WIN32) || defined(WIN32))
  std::system("cls");
  #elif defined(__unix__)
  std::system("clear");
  #endif
}

float Console::ReadFloat(float max) {
  bool decimal = false; //true when dealing with decimals false when whole numbers
  std::string resStr; //char buffer
  char c; //current char being read

  do {
    c = _getch();
    if (c == '\b') { //deal with backspaces
      if (resStr.size() > 0) {
        if (resStr[resStr.size() - 1] == '.') {
          decimal = false;
        }
        resStr.pop_back();
        std::cout << ('\b');
        std::cout << (' ');
        std::cout << ('\b');
      }
    }
    else if (c == '.') { //change to decimal mode
      if (!decimal) {
        decimal = true;
        resStr += c;
        std::cout << (c);
      }
    }
    else if (c >= '0' && c <= '9') { //accept numbers
      if (c != '0' || resStr.size() != 0) {
        resStr += c;
        std::cout << (c);
      }
    }
    // end when we have a valid float and enter is pressed
  } while (c != 13 || resStr.size() == 0 || resStr == ".");

  std::cout << ('\n');

  return std::fmin(std::stof(resStr), max); //return all chars as a float
}

int Console::ReadInt(int max) {
  std::string resStr = ""; //char buffer
  char c; //current char being read
  do {
    c = _getch();
    if (c == '\b') { //deal with backspaces
      if (resStr.size() > 0) {
        resStr.pop_back();
        std::cout << ('\b');
        std::cout << (' ');
        std::cout << ('\b');
      }
    }
    else if (c >= '0' && c <= '9') { //accept numbers
      if (c != '0' || resStr.size() > 0) {
        resStr.push_back(c);
        std::cout << c;
      }
    }
    // end when we have a valid int and enter is pressed
  } while (c != '\r' && c != '\n' && c != '\0' || resStr.size() == 0);

  int ret = std::stoi(resStr); //return all chars as an int

  return (ret < max) ? ret : max;
}

std::string Console::ReadString() {
  std::string resStr = ""; //char buffer
  char c; //current char being read
  do {
    c = _getch();
    if (c == '\b') { //deal with backspaces
      if (resStr.size() > 0) {
        resStr.pop_back();
        std::cout << ('\b');
        std::cout << (' ');
        std::cout << ('\b');
      }
    }
    // accept any except white space chars (does accept actual spaces)
    else if (c != '\r' && c != '\n' && c != '\t' && c != '\0') {
      resStr.push_back(c);
      std::cout << c;
    }
    // end when we have a valid string and enter is pressed
  } while (c != '\r' && c != '\n' && c != '\0' || resStr.size() == 0);

  return resStr;
}

bool Console::ReadBool(char trueChar, char falseChar) {

  bool res; //resulting flag
  char c; //current char being read
  do {
    c = _getch();
    // only accept trueChar or falseChar
    if (c == trueChar || c == falseChar) {
      res = (c == trueChar); //result is equivalent to trueChar
      std::cout << c;
      break; //exits when given valid char
    }

  } while (true);

  return res;
}
