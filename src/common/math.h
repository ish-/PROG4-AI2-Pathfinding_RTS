#ifndef MATH_H
#define MATH_H

#include <iostream>
#include "raymath.h"

struct ivec2 {
    int x, y;
    ivec2(int _x, int _y) : x(_x), y(_y) {}
    ivec2() : x(0), y(0) {}
};

// std::ostream& operator<<(std::ostream& os, const Vector2& v);
inline float mapRange(float x, float in_min, float in_max, float out_min, float out_max);
inline float lag(float current, float target, float smoothingFactor, float dt);

#endif 