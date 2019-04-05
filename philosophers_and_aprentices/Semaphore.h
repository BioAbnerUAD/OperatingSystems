#pragma once
#include <mutex>
#include <condition_variable>

class Semaphore
{
public:
  /**
   * @brief: default constructor
   */
  Semaphore() {
    mutex_ = new std::mutex();
    condition_ = new std::condition_variable();
  }

  /**
   * @brief: default destructor
   */
  ~Semaphore() {
    delete mutex_;
    delete condition_;
  }

private:
  std::mutex* mutex_; // for locking threads
  std::condition_variable* condition_; // for signaling
  bool locked = false; // Initialized as unlocked.

public:
  /**
   * @brief: release resource and tell one of the other threads to stop waiting 
   */
  void post() {
    std::lock_guard<std::mutex> lock(*mutex_);
    locked = false;
    condition_->notify_one();
  }

  /*
   * @brief: wait until resource is given
   */
  void wait() {
    std::unique_lock<std::mutex> lock(*mutex_);
    while (locked)
    {
      condition_->wait(lock);
    }
    locked = true;
  }

  /**
   * @brief: try to get resource but don't wait until it's given
   * @returns: flag to signal if resource was given
   */
  bool try_wait() {
    std::lock_guard<std::mutex> lock(*mutex_);
    if (!locked) {
      locked = true;
      return true;
    }
    return false;
  }
};
