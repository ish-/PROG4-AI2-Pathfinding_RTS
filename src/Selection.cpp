#include "Selection.hpp"

void Selection::start (vec2 _from) {
  reset();
  active = true;
  from = _from;
}

void Selection::update (vec2 _to) {
  to = _to;

  float fromX = std::min(from.x, to.x);
  float fromY = std::min(from.y, to.y);
  float toX = std::max(from.x, to.x);
  float toY = std::max(from.y, to.y);
  rect = { fromX, fromY, toX - fromX, toY - fromY };
}

void Selection::stop (vec2 to) {
  update(to);
  active = false;
}

void Selection::reset () {
  active = false;
  from = {0,0};
  to = {0,0};
  rect = {0,0,0,0};
}

void Selection::draw () {
  if (active) {
    DrawRectangleLinesEx(rect, 2, WHITE);
    DrawRectangleRec(rect, {255,255,255,150});
  }
}
