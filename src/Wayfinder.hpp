#include "interfaces.hpp"
#include "Obstacle.hpp"

template<typename A>
class Pathfinder {
public:
    // A* agent;
    Obstacles& obstacles;
    IPathfinderGrid<typename T>& grid;

    Pathfinder(Grid& grid, Obstacles* obstacles) : grid(grid), obstacles(obstacles) {

    };

    findPath(vec2& start, vec2& dest) {
        // agent->path = pathfinder.setFlowField(cell, destCell, agent->id);
    }

    virtual vec2 getDir(vec2& pos) = 0;
};