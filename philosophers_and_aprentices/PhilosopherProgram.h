#pragma once
#include <thread>
#include <vector>

#include "Philosopher.h"
#include "SafeConsoleInput.h"

class PhilosopherProgram
{
public:
  PhilosopherProgram() = default;
  ~PhilosopherProgram() = default;
  void run();

private:
  void PrepareThreads();
  void Scheduling();
  void RunThreads();
private:
  int numPhilosophers;

  Semaphore print_mutex;
  std::vector<Semaphore> fork;
  std::vector<Philosopher> philosophers;

  std::vector <std::thread> philThread;
};