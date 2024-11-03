#pragma once
#include <iostream>
#include <raymath.h>

#include "common/math.h"

template <typename... T>
inline void LOG (const char* title, T&&... args) {
  std::cout << "[LOG] " << title << " -> ";
  ((std::cout << args << " "), ...);
  std::cout << std::endl;
}