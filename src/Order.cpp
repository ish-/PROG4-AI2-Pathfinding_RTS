#include "Order.hpp"
#include "FlowGrid.hpp"
#include "Obstacle.hpp"

float MoveOrder::DONE_DIST_SQR = 20 * 20;
int MoveOrder::MoveOrder_i = 0;

MoveOrder::MoveOrder (vec2& destination, vector<Boid*>& items, vector<Obstacle*>& obstacles)
  : destination(destination), items(items) {

  id = MoveOrder_i++;
  destCell = grid.at(destination);
  LOG("MoveOrder()", id);

  // projection Obtacles to Grid and setting them as .obstacle
  for (Obstacle* obstacle : obstacles) {
    Rectangle gridObstacleRect = obstacle->rect / FlowGrid::CELL_SIZE;
    for (auto* cell : grid.atRect(gridObstacleRect))
      cell->obstacle = true;
  }

  if (destCell->obstacle)
    return;

  pathfinders.reserve(items.size());
  for (auto* boid : items) {
    Pathfinder* pf = new Pathfinder(grid, obstacles);
    pathfinders[boid] = pf;

    pf->calcPath(boid->pos, destination);
    // vector<FlowCell*> path = grid.setFlowField(cell, destCell);

  }

  // if (grid.path.size())
  //   shortPath.calc(grid.path, obstacles, FlowGrid::CELL_SIZE);
  inited = true;
}

vec2 MoveOrder::getDir (Boid* boid) {
  return pathfinders[boid]->getDir(boid->pos);
  // if (dir.x == 0 && dir.y == 0)
  //   return Vector2Normalize(boid->pos - destination);
  // return dir;
}

vec2 MoveOrder::getDestination () {
  return destination;
}

////////////////////////////////////////

shared_ptr<MoveOrder> MoveOrderManager::create (vector<Boid*>& boids, vec2& destination, vector<Obstacle*>& obstacles) {
  if (boids.empty())
    return nullptr;

  auto order = std::make_shared<MoveOrder>(destination, boids, obstacles);
  if (!order->inited)
    return nullptr;
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
