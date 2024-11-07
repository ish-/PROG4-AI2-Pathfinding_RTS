#pragma once
#include <unordered_map>
#include <memory>

#include "FlowGrid.h"
#include "common/log.hpp"

class Order;
using OrderPtr = std::shared_ptr<Order>;

class Order {
public:
  static int i;
  static std::unordered_map<int, OrderPtr> orders;

  static OrderPtr create () {
    OrderPtr order = std::make_shared<Order>();
    orders[order->id] = order;
    return order;
  }

  FlowGrid grid;
  int id;

  Order(): id(i++) {
    LOG("Order()");
  }

  ~Order() {
    LOG("~Order()");
    orders.erase(id);
  }
};
