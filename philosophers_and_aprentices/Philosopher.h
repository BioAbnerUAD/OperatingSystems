#pragma once
#include <string>

#include "Semaphore.h"

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
  Philosopher(int id, 
              const std::string& name, 
              PhilType::E type,
              int timeToEat,
              Semaphore& printMutex,
              Semaphore& leftFork, 
              Semaphore& rightFork)
    : m_id(id), 
      m_name(name),
      m_type(type),
      m_totalTimeToEat(timeToEat),
      m_printMutex(printMutex),
      m_leftFork(leftFork), 
      m_rightFork(rightFork),
      m_timeToEat(0) {}

  ~Philosopher() = default;

  void init(Philosopher* leftPhil, Philosopher* rightPhil);

  void run();

private:
  void eat();

  void try_eat();

  void take_forks();

  // put down chopsticks 
  void think();

  void suspend();

  void restore();

private:
  int m_id;
  std::string m_name;

  Semaphore& m_printMutex;
  Semaphore& m_leftFork;
  Semaphore& m_rightFork;

  Philosopher* m_rightPhil;
  Philosopher* m_leftPhil;

  PhilState::E m_state;
  PhilType::E m_type;
  int m_timeToEat;
  int m_totalTimeToEat;

public:
  static int s_policies;
  static int s_roundRobinTime;
  static bool s_useRandomEatTime;
};
