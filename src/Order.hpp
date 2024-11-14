#pragma once
#include <memory>
#include <unordered_map>

#include "common/math.hpp"
#include "Boid.hpp"
#include "FlowGrid.hpp"
#include "Pathfinder.hpp"
// #include "ShortPath.hpp"

class MoveOrder;
using MoveOrderPtr = std::shared_ptr<MoveOrder>;

class MoveOrder: public IBoidMoveOrder {
public:
  static float DONE_DIST_SQR;
  static int MoveOrder_i;

  int id;
  vec2 destination;
  FlowCell* destCell;
  vector<Boid*> items;
  unordered_map<Boid*, Pathfinder*> pathfinders;
  FlowGrid grid;
  // ShortPath shortPath;

  MoveOrder (vec2& destination, vector<Boid*>& items, vector<Obstacle*>& obstacles);

  vec2 getDir (Boid* boid) override;

  vec2 getDestination () override;
};

class MoveOrderManager {
public:
  vector<std::weak_ptr<MoveOrder>> orders;

  MoveOrderPtr create (vector<Boid*>& boids, vec2& destination, vector<Obstacle*>& obstacles);

  void clear();
};