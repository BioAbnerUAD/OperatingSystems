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

  /**
   * @brief: run program
   */
  void run();

private:
  /**
   * @brief: initializes all threads asking user for the details
   */
  void PrepareThreads();

  /**
   * @brief: ask user for details regarding scheduling
   */
  void Scheduling();

  /**
   * @brief: run all threads
   */
  void RunThreads();
private:
  int numPhilosophers;

  PhilosopherQueue queueSystem;

  std::vector<Semaphore> forks;
  std::vector<Philosopher> philosophers;

  std::vector <std::thread> philThread;
};