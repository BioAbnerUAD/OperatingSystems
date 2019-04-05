#include "PhilosopherQueue.h"

#include "Philosopher.h"
#include "PrintMultiThread.h"

PhilosopherQueue::PhilosopherQueue() {}

PhilosopherQueue::~PhilosopherQueue() {}

void PhilosopherQueue::AddPhilosopher(Philosopher * philosopher) {
  if (philosopher->m_type == PhilType::MASTER) {
    highPriorityQueue.push_back(philosopher);
  }
  else {
    lowPriorityQueue.push_back(philosopher);
  }
  numPhilosophers++;
}

void PhilosopherQueue::run() {
  do {
    // give priority to all in hight priority first
    auto it = highPriorityQueue.begin();
    while (it != highPriorityQueue.end()) {
      Philosopher * phil = *it;
      if (phil->m_state == PhilState::HUNGRY) {
        giveForks(phil);
      }
      if (phil->m_state == PhilState::FULL) {
        it = highPriorityQueue.erase(it);
        doneQueue.push_back(phil);
      }
      else {
        ++it;
      }
    }

    // then give priority to all in low priority
    it = lowPriorityQueue.begin();
    while (it != lowPriorityQueue.end()) {
      Philosopher * phil = *it;
      if (phil->m_state == PhilState::HUNGRY) {
        giveForks(phil);
      }
      if (phil->m_state == PhilState::FULL) {
        it = lowPriorityQueue.erase(it);
        doneQueue.push_back(phil);
      }
      else {
        ++it;
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  } while (doneQueue.size() < numPhilosophers);
}

void PhilosopherQueue::giveForks(Philosopher *phil) {
  Philosopher *rightPhil = phil->m_rightPhil;
  Philosopher *leftPhil = phil->m_leftPhil;

  if (phil->m_type == PhilType::MASTER) {
    if (rightPhil->m_state == PhilState::EATING &&
        rightPhil->m_type == PhilType::APRENTICE) {

      rightPhil->suspend();
      PrintMultiThread::print(phil->m_name + " takes fork away from " 
                              + rightPhil->m_name + ".\n");
    }

    if (leftPhil->m_state == PhilState::EATING &&
        leftPhil->m_type == PhilType::APRENTICE) {

      leftPhil->suspend();
      PrintMultiThread::print(phil->m_name + " takes fork away from "
                              + leftPhil->m_name + ".\n");
    }
  }
  if ((Philosopher::s_policies & Policies::SRT) == Policies::SRT) {
    if (rightPhil->m_state == PhilState::EATING &&
        rightPhil->m_type == phil->m_type &&
        rightPhil->m_timeToEat > phil->m_totalTimeToEat) {

      rightPhil->suspend();
      PrintMultiThread::print(phil->m_name + " takes fork away from "
                              + rightPhil->m_name + ".\n");
    }

    if (leftPhil->m_state == PhilState::EATING &&
        leftPhil->m_type == phil->m_type &&
        leftPhil->m_timeToEat > phil->m_totalTimeToEat) {

      leftPhil->suspend();
      PrintMultiThread::print(phil->m_name + " takes fork away from "
                              + leftPhil->m_name + ".\n");
    }
  }
}
