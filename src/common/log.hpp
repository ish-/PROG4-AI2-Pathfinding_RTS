#pragma once
#include <iostream>
// #include <raymath.h>

// #include "common/math.hpp"

template <typename... T>
inline void LOG (const char* title, T&&... args) {
  std::cout << "[LOG] " << title << " -> ";
  ((std::cout << args << " "), ...);
  std::cout << std::endl;
}

// class LOG_TIMER {
// public:
//   double start;
//   double elapsed;
//   bool silent;
//   std::string title;

//   LOG_TIMER (const char* title, bool silent = false) : title(title), silent(silent) {
//     start = GetTime();
//   }

//   void stop () {
//     elapsed = GetTime() - start;
//   }

//   void destroy () {
//     delete this;
//   }

//   ~LOG_TIMER () {
//     if (silent)
//       return;
//     LOG("[timer]", title, elapsed);
//   }
// };
