#include "SafeConsoleInput.h"

#include <iostream>

#ifdef __unix__

#include <unistd.h>
#include <termios.h>

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

#include <conio.h>

#endif

void Console::Clear() {
  #if (defined(_WIN32) || defined(WIN32))
  std::system("cls");
  #elif defined(__unix__)
  std::system("clear");
  #endif
}

float Console::ReadFloat() {
  bool decimal = false;
  std::string resStr;
  char c;

  do {
    c = _getch();
    if (c == '\b') {
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
    else if (c == '.') {
      if (!decimal) {
        decimal = true;
        resStr += c;
        std::cout << (c);
      }
    }
    else if (c >= '0' && c <= '9') {
      if (c != '0' || resStr.size() != 0) {
        resStr += c;
        std::cout << (c);
      }
    }
  } while (c != 13 || resStr.size() == 0 || resStr == ".");

  std::cout << ('\n');

  return std::stof(resStr);
}

int Console::ReadInt() {
  std::string resStr = "";
  char c;
  do {
    c = _getch();
    if (c == '\b') {
      if (resStr.size() > 0) {
        resStr.pop_back();
        std::cout << ('\b');
        std::cout << (' ');
        std::cout << ('\b');
      }
    }
    else if (c >= '0' && c <= '9') {
      if (c != '0' || resStr.size() > 0) {
        resStr.push_back(c);
        std::cout << c;
      }
    }

  } while (c != '\r' && c != '\n' && c != '\0' || resStr.size() == 0);

  return std::stoi(resStr);
}

std::string Console::ReadString() {
  std::string resStr = "";
  char c;
  do {
    c = _getch();
    if (c == '\b') {
      if (resStr.size() > 0) {
        resStr.pop_back();
        std::cout << ('\b');
        std::cout << (' ');
        std::cout << ('\b');
      }
    }
    else if (c != '\r' && c != '\n' && c != '\t' && c != '\0') {
      resStr.push_back(c);
      std::cout << c;
    }

  } while (c != '\r' && c != '\n' && c != '\0' || resStr.size() == 0);

  return resStr;
}

bool Console::ReadBool(char trueChar, char falseChar) {

  bool res;
  char c;
  do {
    c = _getch();
    if (c == trueChar || c == falseChar) {
      res = c == trueChar;
      std::cout << c;
      break;
    }

  } while (true);

  return res;
}
