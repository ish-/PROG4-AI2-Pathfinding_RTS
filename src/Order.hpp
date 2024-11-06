#pragma once

#include "FlowGrid.h"

class Order {
public:
  static int i;

  FlowGrid grid;
  int id;

  Order() {
    this->id = i++;
  }
};
