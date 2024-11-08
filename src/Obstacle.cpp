#include <algorithm>
#include <raylib.h>
#include "Obstacle.hpp"

Obstacle::Obstacle(const Rectangle& rect): rect{rect} {
}

void Obstacle::draw() {
    DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, touched ? YELLOW : GRAY);
    Color fill = YELLOW;
    fill.a = touched;
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, fill);

    touched = std::max(0., touched - 255. / 60.);
}
