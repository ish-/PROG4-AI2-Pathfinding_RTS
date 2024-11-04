#include <cstdio>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

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
Color DEBUG_COLOR = BLACK;

void DrawRenderTexture (RenderTexture2D rt) {
    DrawTextureRec(rt.texture, {0,0,wSize.x,-wSize.y}, {0,0}, WHITE);
}

vec2 AABB_NULL {0,0};
vec2 checkAABBCollision (Rectangle obstacle, Rectangle obj) {
    vec2& result = AABB_NULL;
    if (obj.x < obstacle.x + obstacle.width && obj.x + obj.width > obstacle.x &&
        obj.y < obstacle.y + obstacle.height && obj.y + obj.height > obstacle.y) {
        if (obj.x + obj.width > obstacle.x && obj.x < obstacle.x)
            result.x = obstacle.x - (obj.x + obj.width);
        else if (obj.x < obstacle.x + obstacle.width && obj.x + obj.width > obstacle.x + obstacle.width)
            result.x = (obstacle.x + obstacle.width) - obj.x;
        if (obj.y + obj.height > obstacle.y && obj.y < obstacle.y)
            result.y = obstacle.y - (obj.y + obj.height);
        else if (obj.y < obstacle.y + obstacle.height && obj.y + obj.height > obstacle.y + obstacle.height)
            result.y = (obstacle.y + obstacle.height) - obj.y;
    }
    return result;
}

bool showDebug;
Vector2 mousePos {wSize.x/2,wSize.y/2};
int mouseZ = 0;
int frame = 0;
double now = 0;
double delta = 0;

using Cell = FlowCell;
FlowGrid grid (gridSize);
Cell* lastHoverCell;
Cell* hoverCell;

Vector2& pointer = mousePos;
StateChangeDetector fullscreenBtn;
StateChangeDetector reloadConfigBtn;
StateChangeDetector showDebugBtn;
StateChangeDetector pointerActBtn;

Cell* getHoverCell () {
    if (0 < mousePos.x && mousePos.x < wSize.x && 0 < mousePos.y && mousePos.y < wSize.y) {
        // unique_lock<mutex> lock(gridMutex);
        int x = Clamp(floor(mousePos.x / cellSize.x), 0.f, gridSize.x);
        int y = Clamp(floor(mousePos.y / cellSize.y), 0.f, gridSize.y);
        return grid.at(x, y);
    }
    return nullptr;
}

void handleMouseClick () {
    // hoverCell->color = RED;

    // LOG("handleMouseClick()", hoverCell->coord);
    // grid.init();
    grid.reset();
    grid.setFlowField(hoverCell, grid.at(0,0));
}

int main()
{
    CONF = LoadConfig();
    printf("Config version: %s\n", CONF.version.c_str());
    
    InitWindow(wSize.x, wSize.y, "Osc Cpp");
    // ToggleBorderlessWindowed();
    // HideCursor();
    // DisableCursor();
    // wSize = {(float)GetMonitorWidth(0), (float)GetMonitorHeight(0)};
    printf("WINDOW SIZE: %f - %f\n", wSize.x, wSize.y);
    printf("GetWorkingDirectory(): %s\n", GetWorkingDirectory());
    // SetWindowSize(wSize.x, wSize.y);

    for (auto& cell : grid.cells)
        cell.obstacle = bool((rand() / (float)RAND_MAX) > .8);

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
            handleMouseClick();

        BeginDrawing();
            ClearBackground(BLACK);

            // if (lastHoverCell != nullptr)
            //     lastHoverCell->color = grid.cellColor;
            // if (hoverCell != nullptr) {
            //     hoverCell->color = CELL_HOVER_COLOR;
            //     lastHoverCell = hoverCell;
            // }

            grid.draw({0,0,wSize.x,wSize.y}, hoverCell);

            if (showDebug) {
                DrawText(TextFormat("%i fps", GetFPS()), 20, 20, 30, DEBUG_COLOR);
                if (hoverCell)
                    DrawText(TextFormat("hoverCell: %i : %i", hoverCell->pos.x, hoverCell->pos.y), 20, 50, 30, DEBUG_COLOR);
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}