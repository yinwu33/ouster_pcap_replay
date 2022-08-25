#pragma once

#include <jsoncpp/json/json.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace util_json {

struct ParameterBase {
  ParameterBase() = default;

  ParameterBase(const std::string& config_file);

  ParameterBase(const std::string& config_file, const std::string& extra_config_file);

  void loadJson(const std::string& config_file, Json::Value& root);

  void read(const Json::Value& root, const std::string& key, int& value);

  void read(const Json::Value& root, const std::string& key, float& value);

  void read(const Json::Value& root, const std::string& key, double& value);

  void read(const Json::Value& root, const std::string& key,
            std::string& value);

  void read(const Json::Value& root, const std::string& key, bool& value);

  void readArray(const Json::Value& root, const std::string& key,
                 std::vector<int>& value, size_t size);

  void readArray(const Json::Value& root, const std::string& key,
                 std::vector<float>& value, size_t size);

  void readArray(const Json::Value& root, const std::string& key,
                 std::vector<double>& value, size_t size);

  void readArray(const Json::Value& root, const std::string& key,
                 std::vector<std::string>& value, size_t size);

  void readArray(const Json::Value& root, const std::string& key,
                 std::vector<bool>& value, size_t size);

  void serialization(const Json::Value& root, std::string& value);

  bool isValid(const Json::Value& root, const std::string& key);

  void saveFile(const Json::Value& root, const std::string& file);

  Json::Value root_;

  Json::Value extra_root_;

  std::string config_file_;

  std::string extra_config_file_;
};

}  // namespace util_json
