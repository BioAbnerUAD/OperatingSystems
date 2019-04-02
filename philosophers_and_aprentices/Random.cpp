#include "Random.h"

#include <random>
#include <ctime>

int Random::randomInt(int min, int max, int salt) {
  std::default_random_engine generator;
  generator.seed(static_cast<int>(std::time(0) + salt));
  std::uniform_int_distribution<int> distribution(min, max);
  return distribution(generator);
}
