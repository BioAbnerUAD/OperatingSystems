#pragma once

namespace Random
{
  /**
   * @brief: generate random integer between min and max (inclusive)
   * uses time as seed yet can be salted so seed becomes more random
   */
  int randomInt(int min, int max, int salt = 0);
}