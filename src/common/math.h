#pragma once

#include <iostream>
#include <string>

#include <raylib.h>

using vec2 = Vector2;

struct ivec2 {
    int x, y;
    ivec2(int _x, int _y) : x(_x), y(_y) {}
    ivec2() : x(0), y(0) {}
    operator std::string() const {
        return "ivec2(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};

inline float mapRange(float x, float in_min, float in_max, float out_min, float out_max);
inline float lag(float current, float target, float smoothingFactor, float dt);

inline std::ostream& operator<<(std::ostream& os, const vec2& v) {
  os << "vec2(" << v.x << ", " << v.y << ")";
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const ivec2& v) {
  os << "ivec2(" << v.x << ", " << v.y << ")";
  return os;
}
