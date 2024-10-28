#pragma once

inline float mapRange(float x, float in_min, float in_max, float out_min, float out_max);
inline float lag(float current, float target, float smoothingFactor, float dt);