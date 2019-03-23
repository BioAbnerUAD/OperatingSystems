
#include <stdio.h>
#include <string>
#include <thread>
#include <ctime>
#include <mutex>
#include <condition_variable>

#define N 5

#define LEFT (phnum + 4) % N 
#define RIGHT (phnum + 1) % N 

class semaphore
{
private:
  std::mutex mutex_;
  std::condition_variable condition_;
  unsigned long count_ = 1; // Initialized as unlocked.

public:
  void notify() {
    std::lock_guard<decltype(mutex_)> lock(mutex_);
    ++count_;
    condition_.notify_one();
  }

  void wait() {
    std::unique_lock<decltype(mutex_)> lock(mutex_);
    while (!count_) // Handle spurious wake-ups.
      condition_.wait(lock);
    --count_;
  }

  bool try_wait() {
    std::lock_guard<decltype(mutex_)> lock(mutex_);
    if (count_) {
      --count_;
      return true;
    }
    return false;
  }
};

enum philState
{
  EATING = 0,
  HUNGRY,
  THINKING,
};

int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };
std::string philName[N] = { "Kierkegaard",
                            "Nietzsche",
                            "Socrates",
                            "Descartes",
                            "Don Ramon" };

semaphore print_mutex;
semaphore fork[N];

void test(int phnum) {
  if (state[phnum] == HUNGRY &&
      state[LEFT] != EATING &&
      state[RIGHT] != EATING) {

    // state that eating 
    state[phnum] = EATING;

    int micseconds = std::rand() % 4501 + 500;
    float seconds = micseconds / 1000.f;

    printf((philName[phnum] + " takes fork %d and %d\n").c_str(), LEFT + 1, phnum + 1);

    printf((philName[phnum] + " is Eating for %f seconds\n").c_str(), seconds);

    std::this_thread::sleep_for(std::chrono::milliseconds(micseconds));

    // sem_post(&fork[phnum]) has no effect 
    // during takefork 
    // used to wake up hungry philosophers 
    // during putfork 
    fork[phnum].notify();
  }
}

// take up chopsticks 
void take_fork(int phnum) {

  print_mutex.wait();

  // state that hungry 
  state[phnum] = HUNGRY;

  printf((philName[phnum] + " is Hungry\n").c_str());

  // eat if neighbours are not eating 
  test(phnum);

  print_mutex.notify();

  // if unable to eat wait to be signalled 
  fork[phnum].wait();

  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

// put down chopsticks 
void put_fork(int phnum) {

  print_mutex.wait();

  // state that thinking 
  state[phnum] = THINKING;

  printf((philName[phnum] + " putting fork %d and %d down\n").c_str(), LEFT + 1, phnum + 1);
  printf((philName[phnum] + " is thinking\n").c_str());

  test(LEFT);
  test(RIGHT);

  print_mutex.notify();
}

void* philospher(void* num) {

  while (true) {

    int* i = (int*)num;

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    take_fork(*i);

    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    put_fork(*i);
  }
}

int main() {

  int i;
  std::thread* thread_id[N];

  std::srand(std::time(0));

  for (i = 0; i < N; i++) {

    // create philosopher processes 
    thread_id[i] = new std::thread(philospher, &phil[i]);

    printf((philName[i] + " is thinking\n").c_str());
  }

  for (i = 0; i < N; i++) {
    thread_id[i]->join();
  }


  for (i = 0; i < N; i++) {
    delete thread_id[i];
  }

  return 0;
}
