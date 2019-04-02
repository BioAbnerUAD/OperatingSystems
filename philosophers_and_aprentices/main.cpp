#include "PhilosopherProgram.h"

#include <iostream>

int main() {
  
  do {
    Console::Clear();

    PhilosopherProgram program;
    program.run();

    std::cout << "Continue? [y,n]: " << std::endl;
  } while (Console::ReadBool('y', 'n'));

  return 0;
}
