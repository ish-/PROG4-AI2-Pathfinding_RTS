#pragma once
#include <vector>
#include "Selection.hpp"
#include "Boid.hpp"

class BoidSelection: public Selection {
public:
  std::vector<Boid*> items;

  void stop (vec2& _to, std::vector<Boid*>& boids);

  void reset () override;
};
