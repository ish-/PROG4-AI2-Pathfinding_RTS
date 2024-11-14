#include "Collisions.hpp"
#include "interfaces.hpp"
#include "PathfindCell.hpp"
#include "Obstacle.hpp"

using Path = std::vector<PathfindCell*>;

template<typename A>
class Pathfinder {
public:
    // A* agent;
    Path waypoints;
    Obstacles& obstacles;
    IPathfinderGrid& grid;

    Pathfinder(Grid& grid, Obstacles& obstacles) : grid(grid), obstacles(obstacles) {

    };

    void calcPath(vec2& start, vec2& dest) {
        Path path;
        // vec2 toDest = dest - start;

        if (ifDirectLine(start, dest))
            return;

        path = grid.getPath(start, dest);
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
        addWaypoint(grid.cellAt(start), dest);
        addWaypoint(grid.cellAt(dest));
        return true;
    }

    void addWaypoint(PathfindCell* cell, PathfindCell* toCell = nullptr) {
        cell->pfWaypoint = toCell;
        waypoints.push_back(cell);
    }

    virtual vec2 getDir(vec2& pos) = 0;
};