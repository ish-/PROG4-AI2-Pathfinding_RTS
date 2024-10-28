#include <cstdio>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "common/math.h"
#include "Grid.h"
#include "StateChangeDetector.hpp"
#include "config.hpp"

Config CONF;
float FPS = 120.f;
float FRAME_TIME = 1. / FPS;
Vector2 wSize {1280, 720};
Vector2 wRatio { 16, 9 };
Vector2 gridSize { wRatio.x*2, wRatio.y*2 };
Vector2 cellSize = { wSize.x / gridSize.x, wSize.y / gridSize.y };
Color CELL_HOVER_COLOR = ORANGE;
Color DEBUG_COLOR = BLACK;

void DrawRenderTexture (RenderTexture2D rt) {
    DrawTextureRec(rt.texture, {0,0,wSize.x,-wSize.y}, {0,0}, WHITE);
}

bool showDebug;
int main()
{
    CONF = LoadConfig();

    InitWindow(wSize.x, wSize.y, "Osc Cpp");
    // ToggleBorderlessWindowed();
    // HideCursor();
    // DisableCursor();
    // wSize = {(float)GetMonitorWidth(0), (float)GetMonitorHeight(0)};
    printf("WINDOW SIZE: %f - %f\n", wSize.x, wSize.y);
    printf("GetWorkingDirectory(): %s\n", GetWorkingDirectory());
    // SetWindowSize(wSize.x, wSize.y);

    SetTargetFPS(FPS);
    ClearBackground(BLACK);

    Vector2 mouse {wSize.x/2,wSize.y/2};
    int mouseZ = 0;
    int frame = 0;

    Grid grid ({0, 0, wSize.x, wSize.y }, gridSize);
    Cell* lastHoverCell = nullptr;

    Vector2& pointer = mouse;
    StateChangeDetector fullscreenBtn;
    StateChangeDetector reloadConfigBtn;
    StateChangeDetector showDebugBtn;

    double frameTime = 0.;
    while (!WindowShouldClose()) {
        double now = GetTime(); frame++;
        double delta = GetFrameTime();
        mouse = GetMousePosition();
        mouseZ = IsMouseButtonPressed(0) ? 1 : (IsMouseButtonPressed(1) ? -1 : 0);

        if (showDebugBtn.hasChangedOn(IsKeyPressed(KEY_D)))
            showDebug = !showDebug;

        Cell* hoverCell = nullptr;
        if (0 < mouse.x && mouse.x < wSize.x && 0 < mouse.y && mouse.y < wSize.y) {
            // unique_lock<mutex> lock(gridMutex);
            int x = clamp(floor(mouse.x / cellSize.x), 0.f, gridSize.x);
            int y = clamp(floor(mouse.y / cellSize.y), 0.f, gridSize.y);
            hoverCell = grid.at(x, y);
        }

        BeginDrawing();
            ClearBackground(BLACK);

            if (lastHoverCell != nullptr)
                lastHoverCell->color = grid.cellColor;
            if (hoverCell != nullptr) {
                hoverCell->color = CELL_HOVER_COLOR;
                lastHoverCell = hoverCell;
            }

            grid.draw();

            if (showDebug) {
                DrawText(TextFormat("%i fps", GetFPS()), 20, 20, 30, DEBUG_COLOR);
                if (lastHoverCell != nullptr)
                    DrawText(TextFormat("pointer: %.0f : %.0f", hoverCell->coord.x, hoverCell->coord.y), 20, 50, 30, DEBUG_COLOR);
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}