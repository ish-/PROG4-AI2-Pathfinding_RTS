#pragma once
#include <vector>
using namespace std;

#include "common/math.hpp"
#include "PathfindCell.hpp"

class IPathfinderGrid {
public:
  virtual PathfindCell* cellAt (vec2& v) = 0;
  virtual vector<PathfindCell*> getPath (vec2& start, vec2& dest) = 0;
};

class IPathfinder {
  virtual vec2 getDir(vec2& pos) = 0;
};
