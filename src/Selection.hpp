#pragma once
#include <vector>
#include "raylib.h"

#include "common/math.hpp"
#include "common/log.hpp"
#include "Boid.h"

class Selection {
public:
    bool active;
    vec2 from;
    vec2 to;
    Rectangle rect;

    void start (vec2& _from) {
        reset();
        active = true;
        from = _from;
    }
    void update (vec2& _to) {
        to = _to;

        float fromX = std::min(from.x, to.x);
        float fromY = std::min(from.y, to.y);
        float toX = std::max(from.x, to.x);
        float toY = std::max(from.y, to.y);
        rect = { fromX, fromY, toX - fromX, toY - fromY };
    }
    void stop (vec2& to) {
        update(to);
        active = false;
    }
    virtual void reset () {
        LOG("Selection::reset");
        active = false;
        from = {0,0};
        to = {0,0};
        rect = {0,0,0,0};
    };

    void draw () {
        if (active) {
            DrawRectangleLinesEx(rect, 2, WHITE);
            DrawRectangleRec(rect, {255,255,255,150});
        }
    }
};

class BoidSelection: public Selection {
public:
    vector<Boid*> boids;

    void stop (vec2& _to, std::vector<Boid*>& boids) {
        Selection::stop(to);

        for (Boid* boid : boids) {
            Rectangle boidRect = { boid->pos.x - boid->size/2, boid->pos.y - boid->size/2, boid->size, boid->size };
            if (AABBCollision(this->rect, boidRect)) {
                boid->selected = true;
                this->boids.push_back(boid);
            }
        }
    }

    void reset () override {
        Selection::reset();
        for (Boid* boid: this->boids) {
            boid->selected = false;
        }
        boids.erase(boids.begin(), boids.end());
        LOG("BoidSelection::reset", boids.size());
    }
};
