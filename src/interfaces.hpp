#pragma once
#include "vector"
using namespace std;

#include "common/math.hpp"

class PathfinderCell {
public:
  bool obstacle = false;
  vec2 pfToStart = vec2{0,0};
  PathfinderCell* pfToStartCell = nullptr;
  PathfinderCell* pfToDestCell = nullptr;
  PathfinderCell* pfWaypoint = nullptr;
};

template<typename TCell = PathfinderCell>
class IPathfinderGrid {
public:
  virtual vector<TCell> getPath (vec2& start, vec2& dest) = 0;
};
