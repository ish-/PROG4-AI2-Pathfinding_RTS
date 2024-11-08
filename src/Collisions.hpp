#pragma once
#include "common/math.hpp"

namespace Collisions {

template <typename T, typename U>
inline bool AABB (T& one, U& two) {
    bool collisionX = one.x + one.width >= two.x && two.x + two.width >= one.x;
    bool collisionY = one.y + one.height >= two.y && two.y + two.height >= one.y;
    return collisionX && collisionY;
}

inline bool lineLine (float& x1, float& y1, float& x2, float& y2, float& x3, float& y3, float& x4, float& y4) {
  float d0 = (y4-y3)*(x2-x1) - (x4-x3)*(y2-y1);
  float d1 = (y1-y3);
  float d2 = (x1-x3);
  float uA = ((x4-x3)*d1 - (y4-y3)*d2) / d0;
  float uB = ((x2-x1)*d1 - (y2-y1)*d2) / d0;
  return uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1;
}

inline bool lineLine (vec2& v1, vec2& v2, vec2 v3, vec2& v4) {
  return lineLine(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, v4.x, v4.y);
}

inline bool lineRect (vec2& v1, vec2& v2, Rectangle& r) {
  vec2 p1 = {r.x, r.y};
  vec2 p2 = {r.x + r.width, r.y};
  vec2 p3 = {r.x + r.width, r.y + r.height};
  vec2 p4 = {r.x, r.y + r.height};
  return lineLine(v1, v2, p1, p2)
      || lineLine(v1, v2, p2, p3)
      || lineLine(v1, v2, p3, p4)
      || lineLine(v1, v2, p4, p1);
}

} // namespace Collisions