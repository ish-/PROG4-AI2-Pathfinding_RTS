#pragma once
#include <unordered_map>
#include <memory>

#include "FlowGrid.h"
#include "Selection.hpp"
#include "common/log.hpp"

class Order;
using OrderPtr = std::shared_ptr<Order>;

class Order {
public:
  static int i;
  static std::unordered_map<int, OrderPtr> orders;

  static OrderPtr create (BoidSelection& selection) {
    OrderPtr order = std::make_shared<Order>(selection);
    orders[order->id] = order;
    return order;
  }

  FlowGrid grid;
  BoidSelection& selection;
  int id;

  Order(BoidSelection& selection): selection(selection), id(i++) {
    LOG("Order()");
  }

  ~Order() {
    LOG("~Order()");
    orders.erase(id);
  }
};
