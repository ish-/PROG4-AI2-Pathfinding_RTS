#include <fstream>
#include <iostream>
#include <stdexcept>

#include "simdjson.h"

using namespace simdjson;

struct Config {
    std::string version;
};

#if !SIMDJSON_SUPPORTS_DESERIALIZATION
// This code is not necessary if you have a C++20 compliant system:
template <>
simdjson_inline simdjson_result<std::vector<double>>
simdjson::ondemand::value::get() noexcept {
  ondemand::array array;
  auto error = get_array().get(array);
  if (error) { return error; }
  std::vector<double> vec;
  for (auto v : array) {
    double val;
    error = v.get_double().get(val);
    if (error) { return error; }
    vec.push_back(val);
  }
  return vec;
}
#endif

template <>
simdjson_inline simdjson_result<Config> simdjson::ondemand::value::get() & noexcept {
  ondemand::object obj;
  auto error = get_object().get(obj);
  if (error) { return error; }
  Config config;
  if((error = obj["version"].get_string(config.version))) { return error; }
  return config;
}

// template <>
// simdjson_inline simdjson_result<Config> simdjson::ondemand::value::get() noexcept {
//   ondemand::object obj;
//   auto error = get_object().get(obj);
//   if (error) { return error; }
//   Config config;
//   if((error = obj["version"].get_string(config.version))) { return error; }
//   return config;
// }


inline Config LoadConfig () {

    try {
        ondemand::parser parser;
        padded_string json = padded_string::load("../resources/config.json");
        ondemand::document doc = parser.iterate(json);
        // ondemand::object obj = doc.get_object();
        Config config(doc);
        // std::ifstream configJsonFile("../resources/config.json");

        // if (!configJsonFile.is_open()) {
        //     std::cerr << "Could not open the file!" << std::endl;
        // }
        // nlohmann::json configJson;
        // configJsonFile >> configJson;
        // return configJson.get<Config>();
        // configJsonFile.close();
        return config;
    } catch (const std::exception& e) {
        std::cerr << "Parsing error: " << e.what() << std::endl;
        throw std::runtime_error("Could not read config.json!");
        Config config;
        return config;
    }

}