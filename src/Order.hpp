#pragma once

#include "FlowGrid.h"
#include "Selection.hpp"

class Order {
public:
  FlowGrid grid;
  int id;

  Order(BoidSelection selection) {
      id = i++;
  }
  static int i;
};

int Order::i = 0;
