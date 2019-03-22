
#include <pthread.h> 
#include <semaphore.h>
#include <stdio.h>
#include <string>
#include <thread>

#define N 5

enum philState
{
  EATING = 0, 
  HUNGRY,
  THINKING, 
};

#define LEFT (phnum + 4) % N 
#define RIGHT (phnum + 1) % N 

int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };
std::string philName[N] = { "Kierkegaard",
                            "Nietzsche",
                            "Socrates",
                            "Descartes",
                            "Don Ramon" };

sem_t mutex;
sem_t semaphore[N];

void test(int phnum) {
  if (state[phnum] == HUNGRY
      && state[LEFT] != EATING
      && state[RIGHT] != EATING) {
    // state that eating 
    state[phnum] = EATING;

    printf((philName[phnum] + " takes fork %d and %d\n").c_str(), LEFT + 1, phnum + 1);

    printf((philName[phnum] + " is Eating\n").c_str());

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // sem_post(&S[phnum]) has no effect 
    // during takefork 
    // used to wake up hungry philosophers 
    // during putfork 
    sem_post(&semaphore[phnum]);
  }
}

// take up chopsticks 
void take_fork(int phnum) {

  sem_wait(&mutex);

  // state that hungry 
  state[phnum] = HUNGRY;

  printf((philName[phnum] + " is Hungry\n").c_str());

  // eat if neighbours are not eating 
  test(phnum);

  sem_post(&mutex);

  // if unable to eat wait to be signalled 
  sem_wait(&semaphore[phnum]);

  std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

// put down chopsticks 
void put_fork(int phnum) {

  sem_wait(&mutex);

  // state that thinking 
  state[phnum] = THINKING;

  printf((philName[phnum] + " putting fork %d and %d down\n").c_str(), LEFT + 1, phnum + 1);
  printf((philName[phnum] + " is thinking\n").c_str());

  test(LEFT);
  test(RIGHT);

  sem_post(&mutex);
}

void* philospher(void* num) {

  while (1) {

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

  // initialize the semaphores 
  sem_init(&mutex, 0, 1);

  for (i = 0; i < N; i++) {
    sem_init(&semaphore[i], 0, 0);
  }

  for (i = 0; i < N; i++) {

    // create philosopher processes 
    thread_id[i] = new std::thread(philospher, &phil[i]);

    printf((philName[i] + " is thinking\n").c_str());
  }

  for (i = 0; i < N; i++) {
    thread_id[i]->join();
  }
}
