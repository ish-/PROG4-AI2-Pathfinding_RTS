#include "common/math.hpp"

#include "Boid.hpp"
#include "BoidSelection.hpp"
#include "Collisions.hpp"

void BoidSelection::stop (vec2& _to, std::vector<Boid*>& boids) {
  Selection::stop(to);

  for (Boid* boid : boids) {
    Rectangle boidRect = { boid->pos.x - boid->size/2, boid->pos.y - boid->size/2, boid->size, boid->size };
    if (Collisions::AABB(this->rect, boidRect)) {
      boid->selected = true;
      this->items.push_back(boid);
    }
  }
}

void BoidSelection::reset () {
  Selection::reset();
  for (Boid* boid: this->items) {
    boid->selected = false;
  }
  items.erase(items.begin(), items.end());
}