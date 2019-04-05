#pragma once
#include <vector>
#include "Semaphore.h"

class Philosopher;

class PhilosopherQueue
{
public:
  PhilosopherQueue();
  ~PhilosopherQueue();

  /**
   * @brief: adds philosopher to queue system
   */
  void AddPhilosopher(Philosopher*);

  /**
   * @brief: run thread (not to be invoked directly but as a thread)
   */
  void run();

private:
  /**
   * @brief: try to give forks to given philosopher
   * doesn't really give forks but rather suspends threads with low priority
   * so high priority threads can take forks
   */
  void giveForks(Philosopher*);

  std::vector<Philosopher*> highPriorityQueue; // for masters
  std::vector<Philosopher*> lowPriorityQueue; // for apprentices
  std::vector<Philosopher*> doneQueue; // for philosophers done eating

  int numPhilosophers; // total number of philosophers in queue system
};

