#pragma once

#include <memory>
#include <vector>
#include <raylib.h>

#include "common/math.hpp"
#include "Obstacle.hpp"

using namespace std;

class Boid;

class IBoidMoveOrder {
public:
    virtual vec2 getDir(Boid* boid) = 0;
    virtual vec2 getDestination () = 0;
};


struct BoidClosest {
    float dist;
    Boid* boid;
};

class Boid {
public:
    static Color selectedColor;
    static int i;

    int id = -1;
    Vector2 vel{ 0,0 };
    Vector2 pos{ 0,0 };
    Vector2 acc{ 0,0 };
    float size = 4.;
    float hp = 1.;
    int group = 0;
    static vector<int> scores;
    bool isAlive = true;
    Color color = WHITE;
    BoidClosest closestBoid;

    vec2 orderVel;
    shared_ptr<IBoidMoveOrder> order;
    bool selected = false;

    Boid(Vector2 _pos, Vector2 _vel, int _group/*, Texture2D _tex*/);

    void updateClosest(Boid* boid, float& dist, Vector2& dir);

    Vector2 followClosest() const;

    void kill(Boid* victim);

    Vector2 separate(Boid* boid, float& dist, Vector2& dir) const;

    Vector2 align(Boid* boid, float& dist, Vector2& dir) const;

    Vector2 seek(Vector2& p, float& dir) const;

    void checkBoudariesAndReflect(vec2 wSize);

    void __update();

    Vector2 avoid(Obstacle* obstacle) const;

    void update(
        double delta,
        vector<Boid*>& boids,
        vector<Obstacle*>& obstacles,
        vec2 wSize
    );

    void draw() const;
};