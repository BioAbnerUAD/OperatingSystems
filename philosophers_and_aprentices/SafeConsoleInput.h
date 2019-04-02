#pragma once
#include <string>

namespace Console
{
  void Clear();

  float ReadFloat();

  int ReadInt();

  std::string ReadString();

  bool ReadBool(char trueChar, char falseChar);
}