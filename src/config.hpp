#include <fstream>
#include <iostream>
#include <stdexcept>

struct Config {
    std::string version;
};

inline Config LoadConfig () {

    try {
        // std::ifstream configJsonFile("../resources/config.json");

        // if (!configJsonFile.is_open()) {
        //     std::cerr << "Could not open the file!" << std::endl;
        // }
        // nlohmann::json configJson;
        // configJsonFile >> configJson;
        // return configJson.get<Config>();
        // configJsonFile.close();
        return { "0.1" };
    } catch (const std::exception& e) {
        std::cerr << "Parsing error: " << e.what() << std::endl;
        throw std::runtime_error("Could not read config.json!");
    }

}