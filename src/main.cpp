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
int mouseZ = 0;
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
void setDestination (Cell* hoverCell);
void setObstacles ();

std::array<Rectangle, 40> obstacleRects;

int main()
{
    CONF = LoadConfig();
    printf("Config version: %s\n", CONF.version.c_str());

    InitWindow(wSize.x, wSize.y, "AI2-pathfinding-rts");
    // ToggleBorderlessWindowed();
    // HideCursor();
    // DisableCursor();
    // wSize = {(float)GetMonitorWidth(0), (float)GetMonitorHeight(0)};
    printf("WINDOW SIZE: %f - %f\n", wSize.x, wSize.y);
    printf("GetWorkingDirectory(): %s\n", GetWorkingDirectory());
    // SetWindowSize(wSize.x, wSize.y);

    setObstacles();

    SetTargetFPS(FPS);
    ClearBackground(BLACK);

    double frameTime = 0.;
    while (!WindowShouldClose()) {
        now = GetTime(); frame++;
        delta = GetFrameTime();

        mousePos = GetMousePosition();
        mouseZ = IsMouseButtonPressed(0) ? 1 : (IsMouseButtonPressed(1) ? -1 : 0);

        if (showDebugBtn.hasChangedOn(IsKeyPressed(KEY_D)))
            showDebug = !showDebug;

        hoverCell = getHoverCell();

        if (pointerActBtn.hasChangedOn(IsMouseButtonPressed(0)))
            setDestination(hoverCell);

        LOG_TIMER timer("FlowGrid.setFlowField", true);
        if (hoverCell && !hoverCell->obstacle && destinationCell && lastHoverCell != hoverCell) {
            grid.reset();
            grid.setFlowField(hoverCell, destinationCell);
        }
        timer.stop();

        BeginDrawing();
            ClearBackground(BLACK);

            grid.draw({0,0,wSize.x,wSize.y}, hoverCell);

            if (showDebug) {
                DrawText(TextFormat("%i fps", GetFPS()), 20, 20, 30, DEBUG_COLOR);
                if (hoverCell)
                    DrawText(TextFormat("hoverCell: %i : %i", hoverCell->pos.x, hoverCell->pos.y), 20, 50, 30, DEBUG_COLOR);
                if (destinationCell)
                    DrawText(TextFormat("setFlow: %f", timer.elapsed), 20, 80, 30, DEBUG_COLOR);
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

void setObstacles () {
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

void setDestination (Cell* hoverCell) {
    if (hoverCell)
        destinationCell = hoverCell;
}