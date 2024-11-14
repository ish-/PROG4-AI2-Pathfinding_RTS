#include "common/log.hpp"

#include "Collisions.hpp"
#include "interfaces.hpp"
#include "PathfindCell.hpp"
#include "Obstacle.hpp"
#include "raymath.h"

using Path = std::vector<PathfindCell*>;

// template<typename A>
class Pathfinder: public IPathfinder {
public:
    // A* agent;
    Path waypoints;
    Obstacles& obstacles;
    IPathfinderGrid& grid;

    Pathfinder(IPathfinderGrid& grid, Obstacles& obstacles) : grid(grid), obstacles(obstacles) {

    }

    vec2 getDir(vec2& pos) override {
        if (waypoints.empty()) {
            LOG(CRED("No waypoints!"), pos);
            return { 0, 0 };
        }

        while (!waypoints.empty()) {
            PathfindCell* cell = waypoints.back();
            // TODO: DANGEROUS is nullptr sometime
            if (cell) {
                vec2 cellPos = cell->pos * FlowGrid::CELL_SIZE;
                vec2 toWaypoint = pos - cellPos;
                float dist = Vector2Length(toWaypoint);
                if (dist > 30)
                    return toWaypoint / dist;
            }
            waypoints.pop_back();
        }

        return { 0, 0 };
    }

    void calcPath(vec2& start, vec2& dest) {
        Path path;
        // vec2 toDest = dest - start;

        if (ifDirectLine(start, dest))
            return;

        waypoints = grid.getPath(start, dest);
    }

    void findWaypoints (Path path) {
        for (PathfindCell* cell : path) {
            if (cell->pfWaypoint)
                addWaypoint(cell, cell->pfWaypoint);
        }
    }

    bool ifDirectLine (vec2& start, vec2& dest) {
        for (Obstacle* obstacle : obstacles) {
            if (Collisions::lineRect(start, dest, obstacle->rect))
                return false;
        }
        auto* destCell = grid.cellAt(dest);
        addWaypoint(destCell);
        addWaypoint(grid.cellAt(start), destCell);
        return true;
    }

    void addWaypoint(PathfindCell* cell, PathfindCell* toCell = nullptr) {
        cell->pfWaypoint = toCell;
        waypoints.push_back(cell);
    }


};