#pragma once
#include "common/math.hpp"
#include "GridCell.hpp"

struct PathfindCell : public GridCell {
    float pfDist = 9999;
    vec2 pfToStart = vec2{0,0};
    PathfindCell* pfToStartCell = nullptr;
    PathfindCell* pfToDestCell = nullptr;
    PathfindCell* pfWaypoint = nullptr;
    float pfDirWeight = 0;
    bool pfPath = false;
    int pfRun = -1;
    bool obstacle = false;
    bool corner = false;
    bool debug = false;
};
