#pragma once
#include <vector>
#include <queue>
// #include <deque>
#include <unordered_set>

#include <raylib.h>
#include <raymath.h>

#include "common/math.hpp"
#include "Grid.hpp"

using namespace std;

class IPathfind {
public:
  virtual vec2 getDir (vec2& pos, bool repeat = false) = 0;
};

struct FlowCell : GridCell {
    float pfDist = 9999;
    vec2 pfToStart = vec2{0,0};
    FlowCell* pfFromCell = nullptr;
    FlowCell* pfToCell = nullptr;
    float pfDirWeight = 0;
    bool pfPath = false;
    int pfRun = -1;
    bool obstacle = false;
    bool corner = false;
    bool debug = false;

    bool operator > (const FlowCell& other) const {
        return pfDirWeight > other.pfDirWeight;
    }

    // ~FlowCell () {
    //     if (pfFromCell) {
    //         delete pfFromCell;
    //         pfFromCell = nullptr;
    //     }
    // }
};

// class CompassCell: public GridCell {
// public:
//     float dirWeight = 0;
// };

struct CompareFlowCellPriorityPtr {
    bool operator()(const FlowCell* a, const FlowCell* b) const {
        return a->pfDirWeight < b->pfDirWeight;  // Max-heap: Higher priority comes first
    }
};

class FlowGrid : public Grid<FlowCell>, public IPathfind {
public:
    static ivec2 SIZE;
    static vec2 CELL_SIZE;

    // std::vector<CompassCell*> compass = {
    //     new CompassCell{{ 1.,  0}}, // E
    //     new CompassCell{{ 1., -1}}, // NE
    //     new CompassCell{{ 0., -1}}, // N
    //     new CompassCell{{-1., -1}}, // NW
    //     new CompassCell{{-1.,  0}}, // W
    //     new CompassCell{{-1.,  1}}, // SW
    //     new CompassCell{{ 0.,  1}}, // S
    //     new CompassCell{{ 1.,  1}}, // SE
    // };

    int pfRun = 0;
    std::vector<FlowCell*> path;
    FlowCell* destCell;
    FlowCell* startCell;

    FlowGrid () : Grid(FlowGrid::SIZE, FlowGrid::CELL_SIZE) {};
    FlowGrid (ivec2& size, vec2& cellSize, FlowCell* destCell)
        : Grid(size, cellSize) {};

    // void setCompassWeights (FlowCell* fromCell, FlowCell* toCell);
    void setFlowField(FlowCell* startCell, FlowCell* destCell, int boidId);
    void setPath(FlowCell* startCell, FlowCell* destCell);

    void draw (const Rectangle& rect);

    void clearQueue ();

    vec2 getDir (vec2& pos, bool repeat = false);

    // void setObstacles (vector<Obstacle*>& obstacles);

    void reset ();

protected:
    std::unordered_set<FlowCell*> passedCells;
    // std::queue<FlowCell*> queueCells;
    std::priority_queue<FlowCell*, std::vector<FlowCell*>, CompareFlowCellPriorityPtr> queueCells;
};
