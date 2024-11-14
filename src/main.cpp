#include <algorithm>
#include <cstring>
#include <exception>
#include <memory>
#include <vector>

#include <raylib.h>
#include <raymath.h>

#include "Grid.hpp"
#include "common/math.hpp"
#include "common/log.hpp"
#include "StateChangeDetector.hpp"
#include "config.hpp"
#include "FlowGrid.hpp"
#include "Obstacle.hpp"
#include "Boid.hpp"
#include "BoidSelection.hpp"
#include "Order.hpp"

float FPS = 60.f;
float FRAME_TIME = 1. / FPS;
Vector2 wSize {1280, 720};
Vector2 wRatio { 16, 9 };

bool showDebug;
Vector2 pointerPos {wSize.x/2,wSize.y/2};
float mouseZ = 0;
int frame = 0;
double now = 0;
double delta = 0;
bool paused = false;

Vector2& pointer = pointerPos;
StateChangeDetector fullscreenBtn;
StateChangeDetector reloadConfigBtn;
StateChangeDetector showDebugBtn;
StateChangeDetector pauseBtn;
StateChangeDetector pointerActBtn;
StateChangeDetector pointerCtxBtn;

std::vector<Obstacle*> obstacles;
std::vector<Boid*> boids;
BoidSelection selection;
MoveOrderManager moveOrderManager;
void CreateObstacles ();
void CreateBoids();
bool DrawAliveAndClearDead (Boid* boid);
MoveOrderPtr GetLastOrder ();

int main()
{
    std::filesystem::current_path(BIN_TO_BUILD_PATH);

    LoadConfig();
    LOG("Config version", CONF.version);

    FlowGrid::SIZE = { (int)wRatio.x * CONF.GRID_SIZE_MULT, (int)wRatio.y * CONF.GRID_SIZE_MULT };
    FlowGrid::CELL_SIZE = { wSize.x / FlowGrid::SIZE.x, wSize.y / FlowGrid::SIZE.y };

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

        if (reloadConfigBtn.hasChangedOn(IsKeyPressed(KEY_R)))
            LoadConfig();

        if (showDebugBtn.hasChangedOn(IsKeyPressed(KEY_D)))
            showDebug = !showDebug;

        if (pointerActBtn.hasChanged(IsMouseButtonDown(0))) {
            if (pointerActBtn.state) {
                selection.start(pointerPos);
            } else {
                selection.stop(pointerPos, boids);
            }
        } else if (pointerActBtn.state) {
            selection.update(pointerPos);
        }

        if (pointerCtxBtn.hasChanged(IsMouseButtonDown(1))) {
            if (pointerCtxBtn.state)
                moveOrderManager.create(selection.items, pointerPos, obstacles);
        }

        if (pauseBtn.hasChangedOn(IsKeyPressed(KEY_SPACE)))
            paused = !paused;

        moveOrderManager.clear();

        BeginDrawing();
            ClearBackground(BLACK);

            if (!paused) {
                try {
                    Boid::selectedColor.a = (unsigned char)mapRange(sinf(GetTime() * 5.), -1, 1, 100, 255);
                    for (auto& boid: boids) {
                        boid->update(delta, boids, obstacles, wSize);
                    }
                } catch (const std::exception& e) {
                    LOG("\033[1;4;31mException caught: ", e.what(), "\033[0m");
                    break;
                }
            }

            boids.erase(
                remove_if(boids.begin(), boids.end(),
                    DrawAliveAndClearDead),
                boids.end()
            );

            if (selection.active)
                selection.draw();

            for (auto& obstacle : obstacles)
                obstacle->draw();

            if (showDebug) {
                DrawText(TextFormat("%i fps, %i orders", GetFPS(), moveOrderManager.orders.size()), 20, 20, 30, CONF.DEBUG_COLOR);

                if (MoveOrderPtr order = GetLastOrder()) {
                    order->pathfinder.draw({0,0,wSize.x,wSize.y});
                    // order->shortPath.draw();
                    DrawText(TextFormat("order: %i", order->id), 620, 20, 30, CONF.DEBUG_COLOR);
                    if (selection.items.size()) {
                        Boid* boid = selection.items[0];
                        if (auto moveOrder = dynamic_pointer_cast<MoveOrder>(boid->order))
                            DrawText(TextFormat("dest: (%i, %i)", moveOrder->destCell->pos.x, moveOrder->destCell->pos.y), 20, 80, 30, CONF.DEBUG_COLOR);
                    }

                    if (FlowCell* hoverCell = order->pathfinder.at(pointerPos))
                        DrawText(TextFormat(
                            "(%i, %i) (%.1f, %.1f)-%.2f <- (%i, %i) -> (%i, %i)",
                            hoverCell->pos.x,
                            hoverCell->pos.y,
                            hoverCell->pfToStart.x,
                            hoverCell->pfToStart.y,
                            hoverCell->pfDirWeight,
                            hoverCell->pfToStartCell ? hoverCell->pfToStartCell->pos.x : -1,
                            hoverCell->pfToStartCell ? hoverCell->pfToStartCell->pos.y : -1,
                            hoverCell->pfToDestCell ? hoverCell->pfToDestCell->pos.x : -1,
                            hoverCell->pfToDestCell ? hoverCell->pfToDestCell->pos.y : -1
                        ), 20, 120, 30, CONF.DEBUG_COLOR);
                }
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void CreateObstacles () {
    float thickness = 30;
    bool flipFlop = false;
    for (int i = 0; i < CONF.OBSTACLES_N; i++) {
        float length = randf(50.f, 200.f);
        obstacles.push_back(new Obstacle({
            randf(-100.f, wSize.x),
            randf(-100.f, wSize.y),
            flipFlop ? thickness : length,
            flipFlop ? length : thickness,
        }));
        flipFlop = !flipFlop;
    }
}

void CreateBoids() {
    for (int i = 0; i < CONF.BOIDS_N; i++) {
        Vector2 pos{ randf() * float(wSize.x), randf() * float(wSize.y) };
        Vector2 vel{ 0, 0 };
        int group = i % CONF.GROUPS_N;
        Boid* boid = new Boid(pos, vel, group);
        float bias = float(i) / float(CONF.BOIDS_N);
        boid->hp = lerp(.8, 1.2, bias);
        boids.push_back(boid);
    }
}

bool DrawAliveAndClearDead (Boid* boid)
{
    if (boid->isAlive)
        boid->draw();
    return !boid->isAlive;
};

MoveOrderPtr GetLastOrder () {
    if (moveOrderManager.orders.size()) {
        auto wOrder = moveOrderManager.orders.back();
        if (!wOrder.expired())
            return wOrder.lock();
    }
    return nullptr;
}