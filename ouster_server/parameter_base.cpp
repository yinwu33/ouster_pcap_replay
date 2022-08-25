#include "parameter_base.hpp"

#include <memory>

namespace util_json {

ParameterBase::ParameterBase(const std::string& config_file)
    : config_file_(config_file) {
  loadJson(config_file, root_);
}

ParameterBase::ParameterBase(const std::string& config_file,
                             const std::string& extra_config_file) : config_file_(config_file), extra_config_file_(extra_config_file){
  loadJson(config_file, root_);
  loadJson(extra_config_file, extra_root_);
}

void ParameterBase::loadJson(const std::string& config_file,
                             Json::Value& root) {
  std::ifstream ifs(config_file);
  if (!ifs.is_open()) {
    std::cerr << "Open Json File Failed: " << config_file << std::endl;
    throw;
  }

  Json::CharReaderBuilder builder;
  std::stringstream ss;
  ss << ifs.rdbuf();

  std::string errors;

  if (!Json::parseFromStream(builder, ss, &root, &errors)) {
    std::cerr << "Parse Json Node Failed: " << errors << std::endl;
    throw;
  }
  ifs.close();
}

void ParameterBase::read(const Json::Value& root, const std::string& key,
                         int& value) {
  value = isValid(root, key) ? root[key].asInt() : 0;
}

void ParameterBase::read(const Json::Value& root, const std::string& key,
                         float& value) {
  value = isValid(root, key) ? root[key].asFloat() : 0.0;
}

void ParameterBase::read(const Json::Value& root, const std::string& key,
                         double& value) {
  value = isValid(root, key) ? root[key].asDouble() : 0.0;
}

void ParameterBase::read(const Json::Value& root, const std::string& key,
                         std::string& value) {
  value = isValid(root, key) ? root[key].asString() : "";
}

void ParameterBase::read(const Json::Value& root, const std::string& key,
                         bool& value) {
  value = isValid(root, key) ? root[key].asBool() : false;
}

void ParameterBase::readArray(const Json::Value& root, const std::string& key,
                              std::vector<int>& value, size_t size) {
  if (isValid(root, key)) {
    for (size_t i = 0; i < size; ++i)
      value.emplace_back(std::stoi(root[key].asString()));
  }
}

void ParameterBase::readArray(const Json::Value& root, const std::string& key,
                              std::vector<float>& value, size_t size) {
  if (isValid(root, key)) {
    for (size_t i = 0; i < size; ++i)
      value.emplace_back(std::stof(root[key].asString()));
  }
}

void ParameterBase::readArray(const Json::Value& root, const std::string& key,
                              std::vector<double>& value, size_t size) {
  if (isValid(root, key)) {
    for (size_t i = 0; i < size; ++i)
      value.emplace_back(std::stod(root[key].asString()));
  }
}

void ParameterBase::readArray(const Json::Value& root, const std::string& key,
                              std::vector<std::string>& value, size_t size) {
  if (isValid(root, key)) {
    for (size_t i = 0; i < size; ++i) value.emplace_back(root[key].asString());
  }
}

void ParameterBase::readArray(const Json::Value& root, const std::string& key,
                              std::vector<bool>& value, size_t size) {
  if (isValid(root, key)) {
    for (size_t i = 0; i < size; ++i)
      value.emplace_back(std::stoi(root[key].asString()));
  }
}

void ParameterBase::serialization(const Json::Value& root, std::string& value) {
  value = root.toStyledString();
}

bool ParameterBase::isValid(const Json::Value& root, const std::string& key) {
  if (!root.isMember(key)) {
    std::cerr << "Key is not in Json: " << key << std::endl;
    return false;
  }
  return true;
}

void ParameterBase::saveFile(const Json::Value& root, const std::string& file) {
  std::ofstream ofs(file);

  if (!ofs.is_open()) {
    std::cerr << "Save Config Error, Can not Open File: " << file;
    ofs.close();
    return;
  }

  Json::StreamWriterBuilder builder;
  builder["commentStyle"] = "None";
  builder["indentation"] = "  ";
  std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
  writer->write(root, &ofs);

  ofs.close();

  std::cout << "Save Config Successfully to: " << file << std::endl;
}

}  // namespace util_json
