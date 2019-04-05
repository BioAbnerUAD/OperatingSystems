#pragma once
#include "Semaphore.h"
#include <string>

class PrintMultiThread
{
public:
  /**
   * @brief: prints a message without having two threads printing at the same time
   */
  static void print(std::string message);
private:
  static Semaphore mutex; // to signal threads when to print
};

