#pragma once
#include <string>

#include "PhilosopherQueue.h"

namespace Policies
{
  enum E
  {
    FCFS = 0,
    ROUNDROBIN = 1,
    SRT = 2
  };
}

namespace PhilState
{
  enum E
  {
    EATING = 0,
    HUNGRY,
    THINKING,
    FULL,
    SUSPENDED
  };
}

namespace PhilType
{
  enum E
  {
    MASTER = 0,
    APRENTICE,
  };
}

class Philosopher
{
public:
  /**
   * @brief: construct philosopher
   */
  Philosopher(int m_id,
              const std::string& name, 
              PhilType::E type,
              int timeToEat,
              Semaphore& leftFork,
              Semaphore& rightFork)
    : m_rightID(m_id),
      m_name(name),
      m_type(type),
      m_totalTimeToEat(timeToEat),
      m_leftFork(leftFork), 
      m_rightFork(rightFork),
      m_timeToEat(0),
      m_timeEaten(0){}

  ~Philosopher() = default;

  /**
   * @brief: must be initialized before running
   */
  void init(Philosopher*  leftPhil,
            Philosopher* rightPhil,
            PhilosopherQueue* queue);

  /**
   * @brief: run thread (not to be invoked directly but as a thread)
   */
  void run();

private:
  /**
   * @brief: do the actual eating
   */
  void eat();

  /**
   * @brief: goes hungry and waits to start eating
   */
  void try_eat();

  /**
   * @brief: takes forks... duh
   */
  void take_forks();

  /**
   * @brief: stops to think for a random amount of time
   */
  void think();

  /**
   * @brief: while eating stops to give someone else it's forks
   */
  void suspend();

  /**
   * @brief: waits until forks are given back to continue eating
   */
  void restore();

private:
  std::string m_name;

  int m_leftID;
  int m_rightID;

  Semaphore& m_leftFork;
  Semaphore& m_rightFork;

  Philosopher* m_leftPhil;
  Philosopher* m_rightPhil;

  PhilosopherQueue* m_queue;

  PhilState::E m_state;
  PhilType::E m_type;
  int m_timeToEat;
  int m_totalTimeToEat;
  int m_timeEaten;

public:
  static int s_policies;
  static int s_roundRobinTime;
  static bool s_useRandomEatTime;

  friend class PhilosopherQueue;
};
