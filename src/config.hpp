#pragma once
#include <fstream>
#include <iostream>
#include <raylib.h>

#include "common/math.hpp"
#include "nlohmann/detail/macro_scope.hpp"
#include "nlohmann/json.hpp"

#include "common/log.hpp"

struct BoidConfig {
    float orderWeight;
    float speed;
    float maxSpeed;
    float hitHp;
    float separateDist;
    float alignDist;
    float followDist;
    float separateWeight;
    float obstacleAvoidWeight;
    float alignWeight;
    float followWeight;
    float seekWeight;
    float allInflWeight;
    float obstacleAvoidDist;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(BoidConfig, orderWeight,speed,maxSpeed,hitHp,separateDist,alignDist,followDist,separateWeight,obstacleAvoidWeight,alignWeight,followWeight,seekWeight,allInflWeight,obstacleAvoidDist)
};

inline void from_json(const nlohmann::json& j, Color& c) {
    float color[4];
    j.at(0).get_to(color[0]); j.at(1).get_to(color[1]); j.at(2).get_to(color[2]); j.at(3).get_to(color[3]);
    c = toRlColor(color);
}

inline void to_json(nlohmann::json& j, const Color& c) {
    j = nlohmann::json::array({c.r, c.g, c.b, c.a});
}

struct Config {
    BoidConfig Boid;
    Color DEBUG_COLOR;

    int BOIDS_N;
    int GROUPS_N;
    int OBSTACLES_N;
    int GRID_SIZE_MULT;

    std::string version = "0";

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, Boid, DEBUG_COLOR, BOIDS_N, GROUPS_N, OBSTACLES_N, GRID_SIZE_MULT, version)
};

inline Config CONF;

inline Config& LoadConfig () {
    std::ifstream configJsonFile("resources/config.json");

    if (!configJsonFile.is_open()) {
        std::cerr << "Could not open config.json file!" << std::endl;
    }

    try {
        nlohmann::json configJson;
        configJsonFile >> configJson;
        CONF = configJson.get<Config>();
        LOG("config.json loaded!");
        return CONF;
    } catch (const std::exception& e) {
        std::cerr << "Parsing error: " << e.what() << std::endl;
    }

    configJsonFile.close();
    return CONF;
}