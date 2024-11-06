#include "Order.hpp"

int Order::i = 0;

std::unordered_map<int, OrderPtr> Order::orders;