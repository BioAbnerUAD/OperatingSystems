#pragma once
#include <mutex>
#include <condition_variable>

class Semaphore
{
public:
  Semaphore() {
    mutex_ = new std::mutex();
    condition_ = new std::condition_variable();
  }

  ~Semaphore() {
    delete mutex_;
    delete condition_;
  }

private:
  std::mutex* mutex_;
  std::condition_variable* condition_;
  int count = 1; // Initialized as unlocked.

public:
  void post() {
    std::lock_guard<std::mutex> lock(*mutex_);
    ++count;
    condition_->notify_one();
  }

  void wait() {
    std::unique_lock<std::mutex> lock(*mutex_);
    while (count <= 0) // Handle spurious wake-ups.
    {
      condition_->wait(lock);
    }
    --count;
  }

  bool try_wait() {
    std::lock_guard<std::mutex> lock(*mutex_);
    if (count <= 0) {
      --count;
      return true;
    }
    return false;
  }
};
