#pragma once
#include <iostream>
#include <raymath.h>

inline std::ostream& operator<<(std::ostream& os, const Vector2& v) {
  os << "Vector2(" << v.x << ", " << v.y << ")";
  return os;
}

template <typename... T>
inline void LOG (const char* title, T&&... args) {
  std::cout << "[LOG] " << title << " -> ";
  ((std::cout << args << " "), ...);
  std::cout << std::endl;
}