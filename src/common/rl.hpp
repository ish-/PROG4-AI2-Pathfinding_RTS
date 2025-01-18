#pragma once
#include <raylib.h>
#include "common/math.hpp"

struct Line {
  vec2 start;
  vec2 end;

  Line operator* (const float& f) {
    return { start * f, end * f };
  }
  Line operator* (const vec2& v) {
    return { start * v, end * v };
  }
};

inline void DrawLine (const vec2& start, const vec2& end, const Color color) {
  DrawLine(start.x, start.y, end.x, end.y, color);
}

inline void DrawLine (const Line& line, const Color color) {
  DrawLine(line.start.x, line.start.y, line.end.x, line.end.y, color);
}

inline void DrawRectangle (const Rectangle& rect, const Color color) {
  DrawRectangle(rect.x, rect.y, rect.width, rect.height, color);
}