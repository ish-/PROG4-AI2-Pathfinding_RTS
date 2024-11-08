#pragma once
#include <vector>
#include <queue>
#include <unordered_set>

#include <raylib.h>
#include <raymath.h>

#include "common/math.hpp"
#include "Grid.hpp"

using namespace std;

class IPathfind {
public:
  virtual vec2 getDir (vec2& pos) = 0;
};

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

class FlowGrid : public Grid<FlowCell>, public IPathfind {
public:
    static ivec2 SIZE;
    static vec2 CELL_SIZE;

    std::vector<FlowCell*> path;

    FlowGrid () : Grid(FlowGrid::SIZE, FlowGrid::CELL_SIZE) {};
    FlowGrid (ivec2& size, vec2& cellSize)
        : Grid(size, cellSize) {};

    void setFlowField(FlowCell* fromCell, FlowCell* toCell);
    void setPath(FlowCell* fromCell, FlowCell* toCell);

    void draw (const Rectangle& rect/*,  FlowCell* hoverCell*/);

    vec2 getDir (vec2& pos);

    // void setObstacles (vector<Obstacle*>& obstacles);

    void reset ();

protected:
    std::unordered_set<FlowCell*> passedCells;
    std::queue<FlowCell*> queueCells;
};
