#pragma once
#include <cstddef>
#include <vector>
#include <queue>
// #include <deque>
#include <unordered_set>

#include <raylib.h>
#include <raymath.h>

#include "common/math.hpp"
#include "Grid.hpp"
#include "PathfindCell.hpp"
#include "interfaces.hpp"

using namespace std;

using FlowCell = PathfindCell;

struct CompareFlowCellPriorityPtr {
    bool operator()(const FlowCell* a, const FlowCell* b) const {
        return a->pfDirWeight < b->pfDirWeight;
    }
};

class FlowGrid : public Grid<FlowCell>, public IPathfinderGrid {
public:
    static ivec2 SIZE;
    static vec2 CELL_SIZE;

    int pfRun = 0;
    FlowCell* destCell;
    FlowCell* startCell;

    FlowGrid () : Grid(FlowGrid::SIZE, FlowGrid::CELL_SIZE) {};
    FlowGrid (ivec2& size, vec2& cellSize, FlowCell* destCell)
        : Grid(size, cellSize) {};

    // void setCompassWeights (FlowCell* fromCell, FlowCell* toCell);
    vector<FlowCell*> setFlowField(FlowCell* startCell, FlowCell* destCell);
    vector<FlowCell*> setPath(FlowCell* startCell, FlowCell* destCell, FlowCell* usedCell = nullptr);

    void draw (const Rectangle& rect);

    void clearQueue ();

    vec2 getDir (vec2& pos, bool repeat = false);

    vector<FlowCell*> getPath (vec2& start, vec2& dest) override;

    // TODO: move vec2 logic to PathfinderGrid extender
    FlowCell* cellAt (vec2& v) override { return at(v); };

    // void setObstacles (vector<Obstacle*>& obstacles);

    void reset ();

protected:
    std::unordered_set<FlowCell*> passedCells;
    // std::queue<FlowCell*> queueCells;
    std::priority_queue<FlowCell*, std::vector<FlowCell*>, CompareFlowCellPriorityPtr> queueCells;
};
