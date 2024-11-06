#include <climits>
#include <cstdio>
#include <cstring>
#include <exception>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>

#include <raylib.h>
#include <raymath.h>

#include "common/math.hpp"
#include "common/log.hpp"
#include "FlowGrid.h"
#include "StateChangeDetector.hpp"
#include "config.hpp"
#include "Selection.hpp"
// #include "Order.hpp"
#include "Boid.h"

Config CONF;
float FPS = 120.f;
float FRAME_TIME = 1. / FPS;
Vector2 wSize {1280, 720};
Vector2 wRatio { 16, 9 };
ivec2 gridSize { (int)wRatio.x*6, (int)wRatio.y*6 };
Vector2 cellSize = { wSize.x / gridSize.x, wSize.y / gridSize.y };
// Color CELL_HOVER_COLOR = ORANGE;
Color DEBUG_COLOR = WHITE;

bool showDebug;
Vector2 pointerPos {wSize.x/2,wSize.y/2};
float mouseZ = 0;
int frame = 0;
double now = 0;
double delta = 0;

using FGrid = FlowGrid;
using FCell = FlowCell;
FGrid grid (gridSize);

Vector2& pointer = pointerPos;
StateChangeDetector fullscreenBtn;
StateChangeDetector reloadConfigBtn;
StateChangeDetector showDebugBtn;
StateChangeDetector pointerActBtn;
StateChangeDetector pointerCtxBtn;

FCell* lastHoverCell;
FCell* hoverCell;
FCell* destinationCell;
FCell* GetHoverCell (vec2 pos);
void SetDestination (FCell* hoverCell);
void CreateObstacles ();

BoidSelection selection;

vector<Obstacle*> obstacles;
vector<Boid*> boids;
void CreateBoids() {
    for (int i = 0; i < BOIDS_COUNT; i++) {
        Vector2 pos{ randf() * float(wSize.x), randf() * float(wSize.y) };
        Vector2 vel{ 0, 0 };
        int group = i % GROUPS_COUNT;
        Boid* boid = new Boid(pos, vel, group);
        float bias = float(i) / float(BOIDS_COUNT);
        boid->hp = lerp(.8, 1.2, bias);
        boids.push_back(boid);
    }
}

auto drawAliveAndClearDead = !DEBUG_PERF ? [](Boid* boid) {
    if (boid->isAlive)
        boid->draw();
    return !boid->isAlive;
} : [](Boid* boid) {
    boid->draw();
    return false;
};

OrderPtr CreateOrder (BoidSelection selection, vec2 dest) {
    OrderPtr order = Order::create();
    FGrid& grid = order->grid;
    grid.init(gridSize);

    for (auto& cell : grid.cells) {
        Rectangle cellRect = {
            cell.pos.x * cellSize.x,
            cell.pos.y * cellSize.y,
            cellSize.x,
            cellSize.y,
        };
        for (int i = 0; i < OBSTACLES_COUNT; i++) {
            Obstacle* obstacle = obstacles[i];
            cell.obstacle = AABBCollision(obstacle->rect, cellRect);
            // LOG("cell.obstacle", cell.obstacle);
            if (cell.obstacle)
                break;
        }
    }

    ivec2 fromCellCoord = grid.toCoord(cellSize, { selection.rect.x + selection.rect.width/2, selection.rect.y + selection.rect.height/2 });
    FCell* fromCell = grid.at(fromCellCoord);
    FCell* destCell = grid.at(grid.toCoord(cellSize, dest));
    grid.setFlowField(destCell, fromCell);

    for (Boid* boid : selection.boids)
        boid->order = order;
    return order;
}

int main()
{
    CONF = LoadConfig();
    printf("Config version: %s\n", CONF.version.c_str());

    InitWindow(wSize.x, wSize.y, "AI2-pathfinding-rts");
    SetTargetFPS(FPS);
    ClearBackground(BLACK);

    CreateObstacles();
    CreateBoids();

    double frameTime = 0.;
    while (!WindowShouldClose()) {
        now = GetTime(); frame++;
        delta = GetFrameTime();

        pointerPos = GetMousePosition();
        mouseZ = IsMouseButtonDown(0) ? 1 : (IsMouseButtonDown(1) ? -1 : 0);

        if (showDebugBtn.hasChangedOn(IsKeyPressed(KEY_D)))
            showDebug = !showDebug;

        hoverCell = GetHoverCell(pointerPos);


        bool pointerActBtnChanged = pointerActBtn.hasChanged(IsMouseButtonDown(0));
        if (pointerActBtnChanged) {
            if (pointerActBtn.state) {
                SetDestination(hoverCell);
                selection.start(pointerPos);
            } else {
                selection.stop(pointerPos, boids);
            }
        } else if (pointerActBtn.state) {
            selection.update(pointerPos);
        }

        if (pointerCtxBtn.hasChanged(IsMouseButtonDown(1))) {
            if (pointerCtxBtn.state)
                CreateOrder(selection, pointerPos);
        }

        // LOG_TIMER timer("FlowGrid.setFlowField", true);
        if (hoverCell && !hoverCell->obstacle && destinationCell && lastHoverCell != hoverCell) {
            grid.reset();
            grid.setFlowField(hoverCell, destinationCell);
        }
        // timer.stop();

        BeginDrawing();
            ClearBackground(BLACK);

            try {
                Boid::selectedColor.a = (unsigned char)mapRange(sinf(GetTime() * 5.), -1, 1, 100, 255);
                for (auto boid: boids) {
                    boid->update(boids, obstacles);
                }
            } catch (const std::exception& e) {
                // std::cerr << "\033[32mException caught: " << e.what() << "\033[0m" << std::endl;
                LOG("\033[1;4;31mException caught: ", e.what(), "\033[0m");
                break;
            }

            boids.erase(
                remove_if(boids.begin(), boids.end(),
                    drawAliveAndClearDead),
                boids.end()
            );

            if (selection.active)
                selection.draw();

            for (auto* obstacle : obstacles)
                obstacle->draw();

            if (showDebug) {
                Order::orders[Order::i-1]->grid.draw({0,0,wSize.x,wSize.y}, hoverCell);
                DrawText(TextFormat("%i fps", GetFPS()), 20, 20, 30, DEBUG_COLOR);
                if (hoverCell)
                    DrawText(TextFormat("hoverCell: %i : %i", hoverCell->pos.x, hoverCell->pos.y), 20, 50, 30, DEBUG_COLOR);
                // if (destinationCell)
                    // DrawText(TextFormat("setFlow: %f", timer.elapsed), 20, 80, 30, DEBUG_COLOR);
            }
        EndDrawing();

        lastHoverCell = hoverCell;
    }

    CloseWindow();

    return 0;
}

ivec2 ScreenToGrid (vec2 pos) {
    return ivec2 {
        static_cast<int>(Clamp(floor(pos.x / cellSize.x), 0.f, gridSize.x)),
        static_cast<int>(Clamp(floor(pos.y / cellSize.y), 0.f, gridSize.y))
    };
}

FCell* GetHoverCell (vec2 pos) {
    if (0 < pos.x && pos.x < wSize.x && 0 < pos.y && pos.y < wSize.y) {
        return grid.at(grid.toCoord(cellSize, pos));
    }
    return nullptr;
}

void CreateObstacles () {
    for (int i = 0; i < OBSTACLES_COUNT; i++) {
        obstacles.push_back(new Obstacle({
            std::lerp(-100.f, wSize.x, randf()),
            std::lerp(-100.f, wSize.y, randf()),
            std::lerp(50.f, 150.f, randf()),
            std::lerp(50.f, 150.f, randf())
        }));
    }
}

void SetDestination (FCell* hoverCell) {
    if (hoverCell)
        destinationCell = hoverCell;
}