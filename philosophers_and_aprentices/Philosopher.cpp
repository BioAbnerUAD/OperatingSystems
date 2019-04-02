#include "Philosopher.h"

#include <iostream>

#include "Random.h"

void Philosopher::init(Philosopher * leftPhil, Philosopher * rightPhil) {
  m_leftPhil = leftPhil;
  m_rightPhil = rightPhil;
}

void Philosopher::run() {

  while (m_totalTimeToEat > 0) {
    think();
    try_eat();
  }

  m_printMutex.wait();

  std::cout << m_name << " is done eating." << std::endl;

  m_printMutex.post();
}

void Philosopher::eat() {
  m_printMutex.wait();

  if (m_state == PhilState::HUNGRY) {
    take_forks();

    m_state = PhilState::EATING;
    if (s_useRandomEatTime) {
      m_timeToEat = Random::randomInt(500, 10000, m_id);
      if ((s_policies & Policies::ROUNDROBIN) == Policies::ROUNDROBIN &&
          m_timeToEat > s_roundRobinTime) {
        m_timeToEat = s_roundRobinTime;
      }
      else if(m_timeToEat > m_totalTimeToEat) {
        m_timeToEat = m_totalTimeToEat;
      }
    }
    else{
      if ((s_policies & Policies::ROUNDROBIN) == Policies::ROUNDROBIN) {
        m_timeToEat = s_roundRobinTime;
      }
      else {
        m_timeToEat = m_totalTimeToEat;
      }
    }

    float seconds = m_timeToEat / 1000.f;

    std::cout << m_name << " is Eating for " << seconds << " seconds" << std::endl;

    m_printMutex.post();

    while (m_timeToEat > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      m_timeToEat--;
      m_totalTimeToEat--;
      if (PhilState::SUSPENDED == m_state) {
        restore();
      }
    }

    m_printMutex.wait();
    std::cout << m_name << " putting fork " << m_id << " and "
              << m_leftPhil->m_id << " down" << std::endl;

    m_leftFork.post();
    m_rightFork.post();

    m_printMutex.post();

  }
  else {
    m_printMutex.post();
  }
}

void Philosopher::try_eat() {

  m_printMutex.wait();

  // m_state that hungry 
  m_state = PhilState::HUNGRY;

  m_printMutex.post();

  // eat if neighbors are not eating 
  eat();

  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void Philosopher::take_forks() {
  if (m_type == PhilType::MASTER) {
    if (m_rightPhil->m_state == PhilState::EATING &&
        m_rightPhil->m_type == PhilType::APRENTICE) {

      m_rightPhil->suspend();
      std::cout << m_name << " takes fork away from " << m_rightPhil->m_name << std::endl;
    }

    if (m_leftPhil->m_state == PhilState::EATING &&
        m_leftPhil->m_type == PhilType::APRENTICE) {

      m_leftPhil->suspend();
      std::cout << m_name << " takes fork away from " << m_leftPhil->m_name << std::endl;
    }
  }
  if ((s_policies & Policies::SRT) == Policies::SRT) {
    if (m_rightPhil->m_state == PhilState::EATING &&
        m_rightPhil->m_type == m_type &&
        m_rightPhil->m_timeToEat > m_totalTimeToEat) {

      m_rightPhil->suspend();
      std::cout << m_name << " takes fork away from " << m_rightPhil->m_name << std::endl;
    }

    if (m_leftPhil->m_state == PhilState::EATING &&
        m_leftPhil->m_type == m_type &&
        m_leftPhil->m_timeToEat > m_totalTimeToEat) {

      m_leftPhil->suspend();
      std::cout << m_name << " takes fork away from " << m_leftPhil->m_name << std::endl;
    }
  }

  m_printMutex.post();

  m_leftFork.wait();
  m_rightFork.wait();

  m_printMutex.wait();

  std::cout << m_name << " takes fork " 
            << m_id << " and " << m_leftPhil->m_id << std::endl;
}

void Philosopher::think() {

  m_printMutex.wait();

  // m_state that thinking 
  m_state = PhilState::THINKING;

  int thinkTime = Random::randomInt(500, 10000, m_id);

  float seconds = thinkTime / 1000.f;

  std::cout << m_name << " is thinking for " << seconds << " seconds" << std::endl;

  m_printMutex.post();

  std::this_thread::sleep_for(std::chrono::milliseconds(thinkTime));
}

void Philosopher::suspend() {
  m_state = PhilState::SUSPENDED;
  m_leftFork.post();
  m_rightFork.post();
}

void Philosopher::restore() {
  m_leftFork.wait();
  m_rightFork.wait();

  m_state = PhilState::EATING;

  m_printMutex.wait();

  std::cout << m_name << " takes fork "
    << m_leftPhil->m_id << " and " << m_id << std::endl;

  float seconds = m_timeToEat / 1000.f;
  std::cout << m_name << " continues eating for " << seconds << " seconds" << std::endl;

  m_printMutex.post();
}

int Philosopher::s_policies = 0;

int Philosopher::s_roundRobinTime = 0;

bool Philosopher::s_useRandomEatTime = false;
