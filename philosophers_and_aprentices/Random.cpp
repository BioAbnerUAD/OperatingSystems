#include "Random.h"

#include <random>
#include <ctime>

int Random::randomInt(int min, int max, int salt) {
  // initialize random number generator with given seed
  std::default_random_engine generator;
  generator.seed(static_cast<int>(std::time(0) + salt));

  // gets a random number within a uniform distribution (all nums are equally probable)
  std::uniform_int_distribution<int> distribution(min, max);
  return distribution(generator);
}
