#pragma once

#include <iostream>
#include <string>

#include <raylib.h>

using vec2 = Vector2;

struct ivec2 {
    int x, y;
    ivec2(int _x, int _y) : x(_x), y(_y) {}
    ivec2(int _x) : x(_x), y(_x) {}
    ivec2() : x(0), y(0) {}

    ivec2 operator+(const ivec2& v) const { return ivec2(x + v.x, y + v.y); }
    vec2 operator+(const vec2& v) const { return vec2(x + v.x, y + v.y); }

    ivec2 operator*(const int& scalar) const { return ivec2(x * scalar, y * scalar); }
    template <typename T = float>
    vec2 operator*(const T& scalar) const { return vec2(x * scalar, y * scalar); }

    operator std::string() const {
        return "ivec2(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};

inline vec2 operator+ (const vec2& v1, const vec2& v2) { return vec2(v1.x + v2.x, v1.y + v2.y); }

inline float mapRange(float x, float in_min, float in_max, float out_min, float out_max) {
    return out_min + (x - in_min) * (out_max - out_min) / (in_max - in_min);
}

inline float lag(float current, float target, float smoothingFactor, float dt) {
    return current + (target - current) * smoothingFactor * dt;
}

template <typename T, typename U>
inline bool AABBCollision (T& one, U& two) {
    bool collisionX = one.x + one.width >= two.x && two.x + two.width >= one.x;
    bool collisionY = one.y + one.height >= two.y && two.y + two.height >= one.y;
    return collisionX && collisionY;
}

#include <random>
inline std::random_device _rd;
inline std::mt19937 _rdGen(_rd());
inline std::uniform_real_distribution<float> _rdGenDistf(0,1);
inline float randf () {
    return _rdGenDistf(_rdGen);
}

inline std::ostream& operator<<(std::ostream& os, const vec2& v) {
  os << "vec2(" << v.x << ", " << v.y << ")";
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const ivec2& v) {
  os << "ivec2(" << v.x << ", " << v.y << ")";
  return os;
}
