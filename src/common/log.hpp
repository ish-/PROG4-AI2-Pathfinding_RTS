#pragma once
#include <iostream>
#include <raymath.h>

#include "common/math.hpp"

template <typename... T>
inline void LOG (const char* title, T&&... args) {
  std::cout << "[LOG] " << title << " -> ";
  ((std::cout << args << " "), ...);
  std::cout << std::endl;
}

class LOG_TIMER {
public:
  double start;
  std::string title;

  LOG_TIMER (const char* title) : title(title) {
    start = GetTime();
  }

  void stop () {
    delete this;
  }

  ~LOG_TIMER () {
    LOG("[timer]", title, GetTime() - start);
  }
};
