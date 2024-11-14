#include "Order.hpp"
#include "FlowGrid.hpp"
#include "Obstacle.hpp"
#include "ShortPath.hpp"

float MoveOrder::DONE_DIST_SQR = 20 * 20;
int MoveOrder::MoveOrder_i = 0;

MoveOrder::MoveOrder (vec2& destination, vector<Boid*>& items, vector<Obstacle*>& obstacles)
  : destination(destination), items(items) {

  id = MoveOrder_i++;
  LOG("MoveOrder()", id);
  vec2 meanPos {0,0};
  for (Boid* boid : items)
    meanPos += boid->pos;
  meanPos = meanPos / items.size();

  // projection Obtacles to Grid and setting them as .obstacles
  for (Obstacle* obstacle : obstacles) {
    Rectangle gridObstacleRect = obstacle->rect / FlowGrid::CELL_SIZE;
    for (auto* cell : pathfinder.atRect(gridObstacleRect))
      cell->obstacle = true;
  }

  destCell = pathfinder.at(destination);
  // auto* destCell = pathfinder.at(pathfinder.toCoord(FlowGrid::CELL_SIZE, meanPos));
  // pathfinder.setFlowField(destCell, destCell);

  for (auto* boid : items) {
    auto* cell = pathfinder.at(boid->pos);
    vector<FlowCell*> path = pathfinder.setFlowField(cell, destCell, boid->id);

  }

  // if (pathfinder.path.size())
  //   shortPath.calc(pathfinder.path, obstacles, FlowGrid::CELL_SIZE);
}

vec2 MoveOrder::getDir (vec2& pos) {
  vec2 dir = pathfinder.getDir(pos);
  if (dir.x == 0 && dir.y == 0)
    return Vector2Normalize(pos - destination);
  return dir;
}

vec2 MoveOrder::getDestination () {
  return destination;
}

////////////////////////////////////////

shared_ptr<MoveOrder> MoveOrderManager::create (vector<Boid*>& boids, vec2& destination, vector<Obstacle*>& obstacles) {
  auto order = std::make_shared<MoveOrder>(destination, boids, obstacles);
  orders.push_back(order);
  for (Boid* boid : boids)
    boid->order = order;
  return order;
}

void MoveOrderManager::clear() {
  for (auto it = orders.begin(); it != orders.end();) {
    if (it->expired())
      it = orders.erase(it);
    else
      ++it;
  }
}
