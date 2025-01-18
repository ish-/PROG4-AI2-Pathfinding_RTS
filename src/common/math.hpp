#pragma once

#include <iostream>
#include <string>

#include <raylib.h>
#include <raymath.h>

using vec2 = Vector2;

struct ivec2 {
    int x, y;
    ivec2(int _x, int _y) : x(_x), y(_y) {}
    ivec2(int _x) : x(_x), y(_x) {}
    ivec2() : x(0), y(0) {}

    ivec2 operator+(const ivec2& v) const { return ivec2(x + v.x, y + v.y); }
    vec2 operator+(const vec2& v) const { return vec2(x + v.x, y + v.y); }

    // ivec2 operator*(const int& scalar) const { return ivec2(x * scalar, y * scalar); }
    template <typename T = float>
    vec2 operator*(const T& scalar) const { return vec2(x * scalar, y * scalar); }
    vec2 operator*(const vec2& v) const { return vec2(x * v.x, y * v.y); }

    operator vec2() const { return vec2(x, y); }

    operator std::string() const {
        return "ivec2(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};

inline vec2 operator+ (const vec2& v1, const vec2& v2) { return vec2(v1.x + v2.x, v1.y + v2.y); }
template <typename T = vec2>
inline vec2 operator+=(vec2& a, const T& b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}
template <typename T = vec2>
inline vec2 operator-=(vec2& a, const T& b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}
inline vec2 operator*=(vec2& a, const float i) {
    a.x *= i;
    a.y *= i;
    return a;
}
inline vec2 operator-(const vec2& a, const vec2& b) {
    return vec2{a.x - b.x, a.y - b.y};
}
inline vec2 operator*(const vec2& a, const float i) {
    return Vector2Multiply(a, vec2{ i,i });
}
inline vec2 operator/(const vec2& a, const float i) {
    return Vector2Divide(a, vec2{ i,i });
}
inline vec2 operator/(const vec2& a, const vec2& b) {
    return Vector2Divide(a, b);
}
template <typename T = vec2>
inline vec2 operator*(const vec2& a, const T& b) {
    return vec2({ a.x * b.x, a.y * b.y});
}
template <typename T = vec2>
inline bool operator==(const T& a, const T& b) requires requires (T t) { t.x; t.y; } {
    return a.x == b.x && a.y == b.y;
}
template <typename T = vec2>
inline vec2 abs(const T& a) {
    return { abs(a.x), abs(a.y) };
}

template <typename T = vec2>
inline Rectangle operator/(const Rectangle& r, const T& v) {
    return { r.x / v.x, r.y / v.y, r.width / v.x, r.height / v.y };
}
// template <typename T = vec2>
// inline Rectangle operator-(const Rectangle& r, const T& v) {
//     return { r.x - v.x, r.y - v.y, r.width - v.x, r.height - v.y };
// }
// inline Vector2 operator ivec2(const Vector2& a) {
//     return ivec2(int(a.x), int(a.y));
// }

inline float mapRange(float x, float in_min, float in_max, float out_min, float out_max) {
    return out_min + (x - in_min) * (out_max - out_min) / (in_max - in_min);
}

inline float lag(float current, float target, float smoothingFactor, float dt) {
    return current + (target - current) * smoothingFactor * dt;
}

template <typename T = vec2>
inline bool isZero (const T& v) { return v.x == 0 && v.y == 0; }

#include <random>
inline std::random_device _rd;
inline std::mt19937 _rdGen(_rd());
inline std::uniform_real_distribution<float> _rdGenDistf(0,1);
inline float randf () { return _rdGenDistf(_rdGen); }
template <typename T = float>
inline float randf (const T& min, const T& max) { return std::lerp(min, max, _rdGenDistf(_rdGen)); }

inline std::ostream& operator<<(std::ostream& os, const vec2& v) {
  os << "vec2(" << v.x << ", " << v.y << ")";
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const ivec2& v) {
  os << "ivec2(" << v.x << ", " << v.y << ")";
  return os;
}

template <typename T>
Color toRlColor (T& c) {
    return Color{static_cast<unsigned char>(c[0]*255.), static_cast<unsigned char>(c[1]*255.), static_cast<unsigned char>(c[2]*255.), static_cast<unsigned char>(c[3]*255.)};
}