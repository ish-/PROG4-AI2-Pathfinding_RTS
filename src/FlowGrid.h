#pragma once
#include <vector>
#include <queue>
#include <unordered_set>

#include <raylib.h>
#include <raymath.h>

#include "Grid.h"
#include "common/math.hpp"

// using vec2 = Vector2;

struct FlowCell : GridCell {
    float pfDist = 9999;
    Vector2 pfToStart = Vector2{0,0};
    FlowCell* pfFromCell = nullptr;
    bool pfPath = false;
    bool obstacle = false;
    bool corner = false;

    // ~FlowCell () {
    //     if (pfFromCell) {
    //         delete pfFromCell;
    //         pfFromCell = nullptr;
    //     }
    // }
};

class FlowGrid : public Grid<FlowCell> {
public:
    std::vector<FlowCell> path;

    FlowGrid (ivec2& size) : Grid(size) {};

    void setFlowField(FlowCell* fromCell, FlowCell* toCell);
    void setPath(FlowCell* fromCell, FlowCell* toCell);

    void draw (const Rectangle& rect,  FlowCell* hoverCell);

    void reset ();

protected:
    std::unordered_set<FlowCell*> passedCells;
    std::queue<FlowCell*> queueCells;
};
