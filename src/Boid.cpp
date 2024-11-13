#define _USE_MATH_DEFINES
#include <math.h>

#include <raylib.h>
#include <raymath.h>

#include "common/math.hpp"
// #include "common/log.hpp"
#include "config.hpp"

#include "Boid.hpp"

static const Color COLORS[] = { WHITE, PINK, GREEN, BLUE };

vector<int> Boid::scores {0,0,0,0,0,0,0,0,0};

Vector2 nullVec2 = Vector2{0,0};

Color Boid::selectedColor = WHITE;

int Boid::i = 0;
Boid::Boid(Vector2 _pos, Vector2 _vel, int _group/*, Texture2D _tex*/) {
    pos = _pos;
    vel = _vel;
    group = _group;
    id = Boid::i++;
    // tex = _tex;
    // texRect = Rectangle{ 0,0, (float)tex.width, (float)tex.height };
}

void Boid::updateClosest(Boid* boid, float& dist, Vector2& dir) {
    if (boid->group == (group + 1) % CONF.GROUPS_N) {
        if (dist < CONF.Boid.followDist && dist < closestBoid.dist)
            closestBoid = { dist, boid };

            if (dist < size * .9)
                kill(boid);
    }
}

void Boid::kill(Boid* victim) {
    //boidsToDeleteIdxs.push_back(victimIdx);
    victim->hp -= CONF.Boid.hitHp;
    hp += CONF.Boid.hitHp / 2;
    if (victim->hp < 0.) {
        scores[group]++;
        victim->isAlive = false;
    }
    int size = 50;
    DrawRectangleLines(pos.x - size/2, pos.y - size/2, size, size, RED);
}

void Boid::checkBoudariesAndReflect(vec2 wSize) {
    if (pos.x <= 0 || pos.x >= wSize.x) {
        vel.x = -vel.x;
        pos.x = (pos.x <= 0) ? 0 : wSize.x;
    }

    if (pos.y <= 0 || pos.y >= wSize.y) {
        vel.y = -vel.y;
        pos.y = (pos.y <= 0) ? 0 : wSize.y;
    }
}

Vector2 Boid::followClosest() const {
    if (closestBoid.dist < 9000.)
      return  Vector2Normalize(closestBoid.boid->pos - pos);
    return nullVec2;
}

Vector2 Boid::separate(Boid* boid, float& dist, Vector2& dir) const {
    // if (boid->group == this->group || boid->group != (CONF.GROUPS_N + group - 1) % CONF.GROUPS_N)
    {
        if (dist < CONF.Boid.separateDist) {
            return dir * -1.f;
        }
    }
    return nullVec2;
}

Vector2 Boid::seek(Vector2& p, float& dir) const {
    Vector2 to = (p - pos) * dir;
    return to;
}

Vector2 Boid::avoid(Obstacle* obstacle) const {
    Vector2 infl{0,0};
    if (CheckCollisionCircleRec(pos, size, obstacle->rect)) {
        vec2 obstacleCenter = vec2{obstacle->rect.x + obstacle->rect.width/2, obstacle->rect.y + obstacle->rect.height/2};
        Vector2 to = pos - obstacleCenter;
        obstacle->touched = min(255., obstacle->touched + 10.);
        return Vector2Normalize(to);
    }
    return nullVec2;
}

Vector2 Boid::align(Boid* boid, float& dist, Vector2& dir) const {
    if (group == boid->group) {
        if (dist < CONF.Boid.alignDist) {
            return boid->vel;
        }
    }
    return nullVec2;
}

// void Boid::update(vector<Boid*> boids, vector<Obstacle*> obstacles, Vector2& mouse, float& mouseZ) {
void Boid::update(double delta, vector<Boid*>& boids, vector<Obstacle*>& obstacles, vec2 wSize) {
    closestBoid = {9999., nullptr };
    Vector2 avoidInfl{0,0};
    Vector2 alignInfl{0,0};
    Vector2 separateInfl{0,0};
    // Vector2 seekInfl = seek(mouse, mouseZ);

    for (Boid* boid : boids) {
        if (boid == this)
            continue;

        Vector2 to = boid->pos - pos;
        float dist = Vector2Length(to);
        Vector2 dir = Vector2Normalize(to);

        // updateClosest(boid, dist, dir);
        // alignInfl += align(boid, dist, dir);
        separateInfl += separate(boid, dist, dir);
    }

    // Vector2 followInfl = followClosest();

    for (Obstacle* obstacle : obstacles)
        avoidInfl += avoid(obstacle);

    vec2 infl = vec2{0,0}
        + Vector2Normalize(avoidInfl) * CONF.Boid.obstacleAvoidWeight
        // + Vector2Normalize(alignInfl) * CONF.Boid.alignWeight
        + Vector2Normalize(separateInfl) * CONF.Boid.separateWeight
        // + Vector2Normalize(seekInfl)
        // + Vector2Normalize(followInfl) * CONF.Boid.followWeight
    ;

    if (order) {
        float dist = Vector2Length(order->getDestination() - pos);
        if (dist < 30.)
            order.reset();
        else {
            vec2 orderDir = order->getDir(pos);
            // LOG("orderDir", dist, orderDir);
            infl -= (orderDir * CONF.Boid.orderWeight);
        }
    }

    //acc = infl - vel;
    //vel += acc;
    vel += infl * CONF.Boid.allInflWeight;
    vel = Vector2ClampValue(vel, -CONF.Boid.maxSpeed, CONF.Boid.maxSpeed);
    // if (!order)
      vel *= .95;
    checkBoudariesAndReflect(wSize);
    pos += vel * float(delta) * CONF.Boid.speed;

    // if ((hp -= .001) < 0.)
    //     isAlive = false;
    // draw();
}

void Boid::draw() const {
    float s = (.5 + hp) * size * 4;
    float angle = atan2(vel.y, vel.x) / M_PI * 180/* + 180*/;
    // DrawTexturePro(tex, texRect, { pos.x, pos.y, s, s }, { s/2, s/2 }, angle, COLORS[group]);
    if (selected)
      DrawPoly(pos, 3, 12, angle, Boid::selectedColor);
    DrawPoly(pos, 3, 8, angle, COLORS[group]);
    if (selected)
      DrawText(TextFormat("%i", id), pos.x-16, pos.y-16, 8, WHITE);
}
