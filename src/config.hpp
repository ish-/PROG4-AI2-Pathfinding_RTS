#pragma once
#include <fstream>
#include <iostream>
#include <raylib.h>

#include "common/math.hpp"
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
};

struct Config {
    BoidConfig Boid;
    Color DEBUG_COLOR;

    std::string version = "0";
};

inline void from_json (const nlohmann::json& j, Color& c) {
    float color[4];
    j.at(0).get_to(color[0]); j.at(1).get_to(color[1]); j.at(2).get_to(color[2]); j.at(3).get_to(color[3]);
    c = toRlColor(color);
}

inline void from_json(const nlohmann::json& j, BoidConfig& b) {
    j.at("speed").get_to(b.speed);
    j.at("orderWeight").get_to(b.orderWeight);
    j.at("maxSpeed").get_to(b.maxSpeed);
    j.at("hitHp").get_to(b.hitHp);
    j.at("separateDist").get_to(b.separateDist);
    j.at("alignDist").get_to(b.alignDist);
    j.at("followDist").get_to(b.followDist);
    j.at("separateWeight").get_to(b.separateWeight);
    j.at("obstacleAvoidWeight").get_to(b.obstacleAvoidWeight);
    j.at("alignWeight").get_to(b.alignWeight);
    j.at("followWeight").get_to(b.followWeight);
    j.at("seekWeight").get_to(b.seekWeight);
    j.at("allInflWeight").get_to(b.allInflWeight);
    j.at("obstacleAvoidDist").get_to(b.obstacleAvoidDist);
}

inline void from_json(const nlohmann::json& j, Config& c) {
    j.at("Boid").get_to(c.Boid);
    // j.at("pointerX_range").at(0).get_to(c.pointerX_range[0]);
    j.at("DEBUG_COLOR").get_to(c.DEBUG_COLOR);

    j.at("version").get_to(c.version);
}

inline Config CONF;

inline Config& LoadConfig () {
    std::ifstream configJsonFile("../resources/config.json");

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