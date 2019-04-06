#include "PhilosopherProgram.h"

#include <iostream>

// Circular List Helpers
int next(int current, int size) {
  return (current + 1 + size) % size;
}

int prev(int current, int size) {
  return (current - 1 + 2 * size) % size;
}

void PhilosopherProgram::run() {

  PrepareThreads();

  Scheduling();

  RunThreads();
}

void PhilosopherProgram::PrepareThreads() {
  std::cout << "How many philosophers will there be? (max 10) : ";

  numPhilosophers = Console::ReadInt(10);
  std::cout << std::endl;
  std::cout << std::endl;

  forks.resize(numPhilosophers);
  philosophers.reserve(numPhilosophers);
  philThread.reserve(numPhilosophers + 1);

  for (int i = 0; i < numPhilosophers; i++) {
    std::cout << "Name philosopher number " << i + 1 << ": ";
    std::string philName = Console::ReadString();
    std::cout << std::endl;

    std::cout << "Is " << philName << " a master or an apprentice? [m/a]: ";
    bool isMaster = Console::ReadBool('m', 'a');
    std::cout << std::endl;

    std::cout << "How much time does " << philName << " need to eat? (seconds max 20.0): ";
    int timeToEat = static_cast<int>(Console::ReadFloat(20.0f) * 1000.f);
    std::cout << std::endl;

    philosophers.emplace_back(i + 1, philName,
                              (isMaster ? PhilType::MASTER : PhilType::APRENTICE),
                              timeToEat,
                              forks[i], 
                              forks[next(i, numPhilosophers)]);

    queueSystem.AddPhilosopher(&philosophers.back());
  }
}

void PhilosopherProgram::RunThreads() {
  for (int i = 0; i < numPhilosophers; i++) {
    philosophers[i].init(&philosophers[prev(i, numPhilosophers)],
                         &philosophers[next(i, numPhilosophers)], 
                         &queueSystem);

    // create Philosopher processes 
    philThread.push_back(std::thread(&Philosopher::run, &philosophers[i]));
  }

  philThread.push_back(std::thread(&PhilosopherQueue::run, &queueSystem));

  for (int i = 0; i <= numPhilosophers; i++) {
    philThread[i].join();
  }

  std::cout << std::endl;
  std::cout << "All philosophers are done eating" << std::endl;
}

void PhilosopherProgram::Scheduling() {
  Philosopher::s_policies = Policies::FCFS;

  std::cout << "Use Round Robin? [y/n]: ";
  bool RoundRobin = Console::ReadBool('y', 'n');
  std::cout << std::endl;

  if (RoundRobin) {

    std::cout << "Round Robin Time (max 5.0): ";
    Philosopher::s_roundRobinTime = static_cast<int>(Console::ReadFloat(5.0f) * 1000.f);
    std::cout << std::endl;

    Philosopher::s_policies += Policies::ROUNDROBIN;
  }

  std::cout << "Use Shortest Remaining Time? [y/n]: ";
  bool Srt = Console::ReadBool('y', 'n');
  std::cout << std::endl;

  if (Srt) {
    Philosopher::s_policies += Policies::SRT;
  }

  std::cout << "Use Random for Eating Time? [y/n]: ";
  Philosopher::s_useRandomEatTime = Console::ReadBool('y', 'n');
  std::cout << std::endl;
  std::cout << std::endl;
}
