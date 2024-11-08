#pragma once
#include <memory>
#include <raylib.h>
#include <raymath.h>

#include "common/log.hpp"
#include "Boid.h"
#include "FlowGrid.h"


class MoveOrder: public IBoidMoveOrder {
public:
  static float DONE_DIST_SQR;
  static int MoveOrder_i;

  int id;
  vec2 destination;
  vector<Boid*> items;
  FlowGrid pathfinder;

  MoveOrder (vec2& destination, vector<Boid*>& items, vector<Obstacle*>& obstacles)
    : destination(destination), items(items) {

    id = MoveOrder_i++;
    LOG("MoveOrder()", id);
    vec2 meanPos {0,0};
    for (Boid* boid : items)
      meanPos += boid->pos;
    meanPos = meanPos / items.size();

    auto* fromCell = pathfinder.at(pathfinder.toCoord(FlowGrid::CELL_SIZE, meanPos));
    auto* destCell = pathfinder.at(pathfinder.toCoord(FlowGrid::CELL_SIZE, destination));

    for (Obstacle* obstacle : obstacles) {
      Rectangle gridObstacleRect = obstacle->rect / FlowGrid::CELL_SIZE;
      for (auto* cell : pathfinder.atRect(gridObstacleRect))
        cell->obstacle = true;
    }

    pathfinder.setFlowField(destCell, fromCell);
  }

  ~MoveOrder () {
    LOG("~MoveOrder()", id);
  }

  vec2 getDir (vec2& pos) override {
    vec2 dir = pathfinder.getDir(pos);
    if (dir.x == 0 && dir.y == 0)
      return Vector2Normalize(pos - destination);
    return dir;
  }

  vec2 getDestination () override {
    return destination;
  }
};

class MoveOrderManager {
public:
  vector<std::weak_ptr<MoveOrder>> orders;

  void create (vector<Boid*>& boids, vec2& destination, vector<Obstacle*>& obstacles) {
    auto order = std::make_shared<MoveOrder>(destination, boids, obstacles);
    orders.push_back(order);
    for (Boid* boid : boids)
      boid->order = order;
  }

  void clear () {
    for (auto it = orders.begin(); it != orders.end(); ) {
      if (it->expired())
        it = orders.erase(it);
      else
        ++it;
    }
  }
};