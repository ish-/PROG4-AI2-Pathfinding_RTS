#pragma once

#include "common/math.hpp"

class Selection {
public:
  bool active;
  vec2 from;
  vec2 to;
  Rectangle rect;

  void start (vec2 _from);

  void update (vec2 _to);

  void stop (vec2 to);

  virtual void reset ();

  void draw ();
};
