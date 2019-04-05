#include "Philosopher.h"

#include "PrintMultiThread.h"
#include "Random.h"

void Philosopher::init(Philosopher* leftPhil,
                       Philosopher* rightPhil,
                       PhilosopherQueue* queue) {
  m_leftPhil = leftPhil;
  m_rightPhil = rightPhil;
  m_leftID = leftPhil->m_rightID;
  m_queue = queue;
}

void Philosopher::run() {

  while (m_totalTimeToEat > 0) {
    think();
    try_eat();
  }

  m_state = PhilState::FULL;
  PrintMultiThread::print(m_name + " is done eating.\n");
}

void Philosopher::eat() {
  m_state = PhilState::EATING; // state that eating

  if (s_useRandomEatTime) { //use random eating time
    m_timeToEat = Random::randomInt(500, 5000, m_rightID);
    if ((s_policies & Policies::ROUNDROBIN) == Policies::ROUNDROBIN &&
        m_timeToEat > s_roundRobinTime) {
      m_timeToEat = s_roundRobinTime; //random eating time clamped
    }
    else if (m_timeToEat > m_totalTimeToEat) {
      m_timeToEat = m_totalTimeToEat; //plain random eating time
    }
  }
  else { //plain eating time
    if ((s_policies & Policies::ROUNDROBIN) == Policies::ROUNDROBIN) {
      m_timeToEat = s_roundRobinTime; //eat in time intervals
    }
    else {
      m_timeToEat = m_totalTimeToEat; //eat all your time at once
    }
  }

  float seconds = m_timeToEat / 1000.f;

  PrintMultiThread::print(m_name + " is Eating for " + std::to_string(seconds) + " seconds.\n");

  while (m_timeToEat > 0) { // this is the actual eating
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    m_timeToEat--;
    m_totalTimeToEat--;
    m_timeEaten++;

    // if suspended that wait until restored
    if (PhilState::SUSPENDED == m_state) {
      restore();
    }
  }

  // put down forks
  m_leftFork.post();
  m_rightFork.post();

  // print when finished eating
  PrintMultiThread::print(m_name + " putting fork " + std::to_string(m_rightID) + " and " +
                          std::to_string(m_leftID) + " down.\n");

  PrintMultiThread::print(m_name + " has eaten for a total of " + 
                          std::to_string(m_timeEaten / 1000.f) + " seconds.\n");
}

void Philosopher::try_eat() {

  // m_state that hungry 
  m_state = PhilState::HUNGRY;

  PrintMultiThread::print(m_name + " is Hungry.\n");

  take_forks(); // get forks before eating

  eat(); // eat if neighbors are not eating 

  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void Philosopher::take_forks() {
  //take forks
  m_leftFork.wait();
  m_rightFork.wait();

  //print that took forks
  PrintMultiThread::print(m_name + " takes fork " + std::to_string(m_rightID) +
                          " and " + std::to_string(m_leftID) + ".\n");
}

void Philosopher::think() {

  m_state = PhilState::THINKING; // state that thinking 

  // think for a random amount of time
  int thinkTime = Random::randomInt(500, 5000, m_rightID);

  float seconds = thinkTime / 1000.f;

  // print that thinking
  PrintMultiThread::print(m_name + " is thinking for " + 
                          std::to_string(seconds) + " seconds.\n");

  // think
  std::this_thread::sleep_for(std::chrono::milliseconds(thinkTime));
}

void Philosopher::suspend() {
  m_state = PhilState::SUSPENDED; // state that suspended
  m_leftFork.post(); //drop forks
  m_rightFork.post();
}

void Philosopher::restore() {
  m_leftFork.wait();  //get forks
  m_rightFork.wait();

  m_state = PhilState::EATING; // state that eating

  float seconds = m_timeToEat / 1000.f;

  // print that once again eating
  PrintMultiThread::print(m_name + " takes fork " + std::to_string(m_leftID) +
                          " and " + std::to_string(m_rightID) + ".\n");

  PrintMultiThread::print(m_name + " continues eating for " + 
                          std::to_string(seconds) + " seconds.\n");
}

int Philosopher::s_policies = 0;

int Philosopher::s_roundRobinTime = 0;

bool Philosopher::s_useRandomEatTime = false;
