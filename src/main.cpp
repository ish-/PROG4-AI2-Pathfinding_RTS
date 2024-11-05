#include <climits>
#include <cstdio>
#include <cstring>
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
Vector2 mousePos {wSize.x/2,wSize.y/2};
float mouseZ = 0;
int frame = 0;
double now = 0;
double delta = 0;

using Cell = FlowCell;
FlowGrid grid (gridSize);

Vector2& pointer = mousePos;
StateChangeDetector fullscreenBtn;
StateChangeDetector reloadConfigBtn;
StateChangeDetector showDebugBtn;
StateChangeDetector pointerActBtn;

Cell* lastHoverCell;
Cell* hoverCell;
Cell* destinationCell;
Cell* getHoverCell ();
void SetDestination (Cell* hoverCell);
void SetObstacles ();

std::array<Rectangle, 40> obstacleRects;

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


int main()
{
    CONF = LoadConfig();
    printf("Config version: %s\n", CONF.version.c_str());

    InitWindow(wSize.x, wSize.y, "AI2-pathfinding-rts");
    SetTargetFPS(FPS);
    ClearBackground(BLACK);

    SetObstacles();
    CreateBoids();

    double frameTime = 0.;
    while (!WindowShouldClose()) {
        now = GetTime(); frame++;
        delta = GetFrameTime();

        mousePos = GetMousePosition();
        mouseZ = IsMouseButtonDown(0) ? 1 : (IsMouseButtonDown(1) ? -1 : 0);

        if (showDebugBtn.hasChangedOn(IsKeyPressed(KEY_D)))
            showDebug = !showDebug;

        hoverCell = getHoverCell();


        bool pointerActBtnChanged = pointerActBtn.hasChanged(IsMouseButtonDown(0));
        if (pointerActBtnChanged) {
            if (pointerActBtn.state) {
                SetDestination(hoverCell);
                selection.start(mousePos);
            } else {
                selection.stop(mousePos, boids);
            }
        } else if (pointerActBtn.state) {
            selection.update(mousePos);
        }

        // LOG_TIMER timer("FlowGrid.setFlowField", true);
        if (hoverCell && !hoverCell->obstacle && destinationCell && lastHoverCell != hoverCell) {
            grid.reset();
            grid.setFlowField(hoverCell, destinationCell);
        }
        // timer.stop();

        BeginDrawing();
            ClearBackground(BLACK);

            // grid.draw({0,0,wSize.x,wSize.y}, hoverCell);

            Boid::selectedColor.a = (unsigned char)mapRange(sinf(GetTime() * 5.), -1, 1, 100, 255);
            for (auto boid: boids) {
                boid->update(boids, obstacles);
            }
            boids.erase(
                remove_if(boids.begin(), boids.end(),
                    drawAliveAndClearDead),
                boids.end()
            );

            if (selection.active)
                selection.draw();

            if (showDebug) {
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

Cell* getHoverCell () {
    if (0 < mousePos.x && mousePos.x < wSize.x && 0 < mousePos.y && mousePos.y < wSize.y) {
        // unique_lock<mutex> lock(gridMutex);
        int x = Clamp(floor(mousePos.x / cellSize.x), 0.f, gridSize.x);
        int y = Clamp(floor(mousePos.y / cellSize.y), 0.f, gridSize.y);
        return grid.at(x, y);
    }
    return nullptr;
}

void SetObstacles () {
    for (auto& rect : obstacleRects) {
        rect.x = std::lerp(-100, wSize.x, randf());
        rect.y = std::lerp(-100, wSize.y, randf());
        rect.width = std::lerp(50, 150, randf());
        rect.height = std::lerp(50, 150, randf());
    }

    for (auto& cell : grid.cells) {
        Rectangle cellRect = {
            cell.pos.x * cellSize.x,
            cell.pos.y * cellSize.y,
            cellSize.x,
            cellSize.y,
        };
        for (int i = 0; i < obstacleRects.size(); i++) {
            Rectangle obstr = obstacleRects[i];
            cell.obstacle = AABBCollision(obstr, cellRect);
            if (cell.obstacle)
                break;
        }
    }
}

void SetDestination (Cell* hoverCell) {
    if (hoverCell)
        destinationCell = hoverCell;
}