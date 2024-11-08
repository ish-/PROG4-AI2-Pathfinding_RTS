#pragma once
#include <raylib.h>

class Obstacle {
public:
    Rectangle rect;
    float margin = 10.;
    float touched = 0.;

    Obstacle(const Rectangle& rect);

    void draw();
};
