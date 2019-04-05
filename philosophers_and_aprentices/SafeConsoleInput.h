#pragma once
#include <string>

namespace Console
{
  /**
   * @brief: clear console from previous output
   */
  void Clear();

  /**
   * @brief: read float value from console
   */
  float ReadFloat();

  /**
   * @brief: read integer value from console
   */
  int ReadInt();

  /**
   * @brief: read string message from console
   */
  std::string ReadString();

  /**
   * @brief: read a character as a boolean from console
   * @returns:
   * true when given the trueChar, 
   * false when given the falseChar 
   * and ignores all other input
   */
  bool ReadBool(char trueChar, char falseChar);
}