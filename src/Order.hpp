#pragma once
#include <memory>

#include "common/math.hpp"
#include "Boid.hpp"
#include "FlowGrid.hpp"


class MoveOrder: public IBoidMoveOrder {
public:
  static float DONE_DIST_SQR;
  static int MoveOrder_i;

  int id;
  vec2 destination;
  vector<Boid*> items;
  FlowGrid pathfinder;

  MoveOrder (vec2& destination, vector<Boid*>& items, vector<Obstacle*>& obstacles);

  vec2 getDir (vec2& pos) override;

  vec2 getDestination () override;
};

class MoveOrderManager {
public:
  vector<std::weak_ptr<MoveOrder>> orders;

  void create (vector<Boid*>& boids, vec2& destination, vector<Obstacle*>& obstacles);

  void clear();
};