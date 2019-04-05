#include "PrintMultiThread.h"

#include <iostream>

void PrintMultiThread::print(std::string message) {
  mutex.wait(); // wait for permission to print
  std::cout << message; // print
  mutex.post(); // give another thread permission to print
}

Semaphore PrintMultiThread::mutex = Semaphore();
