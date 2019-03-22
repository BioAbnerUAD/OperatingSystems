
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h>
#include <stdio.h>
#include <string>

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
sem_t S[N];

void test(int phnum) {
  if (state[phnum] == HUNGRY
      && state[LEFT] != EATING
      && state[RIGHT] != EATING) {
    // state that eating 
    state[phnum] = EATING;

    printf((philName[phnum] + " takes fork %d and %d\n").c_str(), LEFT + 1, phnum + 1);

    printf((philName[phnum] + " is Eating\n").c_str());

    usleep(1000);

    // sem_post(&S[phnum]) has no effect 
    // during takefork 
    // used to wake up hungry philosophers 
    // during putfork 
    sem_post(&S[phnum]);
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
  sem_wait(&S[phnum]);

  usleep(5000);
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

  while (true) {

    int* i = (int*)num;

    usleep(1);

    take_fork(*i);

    usleep(0);

    put_fork(*i);
  }
}

int main() {

  int i;
  pthread_t thread_id[N];

  // initialize the semaphores 
  sem_init(&mutex, 0, 1);

  for (i = 0; i < N; i++)

    sem_init(&S[i], 0, 0);

  for (i = 0; i < N; i++) {

    // create philosopher processes 
    pthread_create(&thread_id[i], NULL,
                   philospher, &phil[i]);

    printf((philName[i] + " is thinking\n").c_str());
  }

  for (i = 0; i < N; i++)

    pthread_join(thread_id[i], NULL);
}
