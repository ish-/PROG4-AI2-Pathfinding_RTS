#pragma once
#include <vector>
using namespace std;
#include <raylib.h>
#include "FlowGrid.hpp"
#include "Obstacle.hpp"

class Line {
  vec2 start;
  vec2 end;

  Line (vec2& start, vec2& end, Color color = WHITE) : start(start), end(end) {

  }

  void draw () {
    DrawLineEx({start.x, start.y}, {end.x, end.y}, 2, WHITE);
  }
};

class ShortPath {
  vector<FlowCell*> lines;

  ShortPath (vector<FlowCell> path, vector<Obstacle*> obstacles) {
    for (int i = 0; i < path.size() - 1; i++) {
      lines.push_back(new Line(path[i].pos, path[i+1].pos));
    }
  }
};
