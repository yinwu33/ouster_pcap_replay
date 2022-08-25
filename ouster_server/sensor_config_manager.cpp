#include "sensor_config_manager.hpp"

// #include <istream>

namespace ouster_pcap_replay {
static std::vector<std::string> splitStrBySpace(const std::string& str) {
  std::vector<std::string> results;
  std::istringstream iss(str);

  std::string temp;
  while (getline(iss, temp, ' ')) results.emplace_back(temp);

  return results;
}

SensorConfigManager::SensorConfigManager(const std::string& config_file,
                                         const std::string& extra_config_file)
    : ParameterBase(config_file, extra_config_file) {}

void SensorConfigManager::getResponse(const std::string& request,
                                      std::string& response) {
  if (request.find("set_config_param") != std::string::npos) {
    std::vector<std::string> substrs = splitStrBySpace(request);

    if (!setConfig(substrs[1], substrs[2])) {
      std::cerr << "Error with: " << request;
      // response = "Unknown Key: " + substrs[1] + "\n";
      response = "set_config_param\n";
      return;
    }

    response = "set_config_param\n";
  } else if (request.find("reinitialize") != std::string::npos)
    response = reinitialize();
  else if (request.find("get_sensor_info") != std::string::npos)
    response = get_sensor_info();
  else if (request.find("get_config_param active") != std::string::npos)
    response = get_config_param_active();
  else if (request.find("get_beam_intrinsics") != std::string::npos)
    response = get_beam_intrinsics();
  else if (request.find("get_imu_intrinsics") != std::string::npos)
    response = get_imu_intrinsics();
  else if (request.find("get_lidar_intrinsics") != std::string::npos)
    response = get_lidar_intrinsics();
  else if (request.find("get_lidar_data_format") != std::string::npos)
    response = get_lidar_data_format();
  else if (request.find("get_calibration_status") != std::string::npos)
    response = get_calibration_status();
  else {
    std::cerr << "\nWARNING: Request Unknown!\n\n";
    response = "Unknown\n";
  }
}

bool SensorConfigManager::setConfig(const std::string& key,
                                    const std::string& value) {
  auto& param_node = extra_root_["get_config_param_active"];

  if (!isValid(param_node, key)) return false;

  std::string clean_value = value.substr(0, value.size() - 1);

  std::cout << "-> set config param: " << key << " value: " << clean_value
            << std::endl;
  std::cout << value << std::endl;

  if (key == "auto_start_flag" or key.find("port") != std::string::npos) {
    std::cout << "clean: " << clean_value << std::endl;
    param_node[key] = std::stoi(value);
  } else {
    param_node[key] = clean_value;
  }

  configChanged = true;

  return true;
}

void SensorConfigManager::saveConfig() {
  saveFile(extra_root_, extra_config_file_);
  configChanged = false;
}

std::string SensorConfigManager::reinitialize() const {
  return "reinitialize\n";
}

std::string SensorConfigManager::get_sensor_info() const {
  Json::Value temp;
  temp["base_pn"] = root_["base_pn"];
  temp["base_sn"] = root_["base_sn"];
  temp["build_date"] = root_["build_date"];
  temp["build_rev"] = root_["build_rev"];
  temp["image_rev"] = root_["base_image_revpn"];
  temp["prod_line"] = root_["prod_line"];
  temp["prod_pn"] = root_["prod_pn"];
  temp["prod_sn"] = root_["prod_sn"];
  temp["proto_rev"] = root_["proto_rev"];
  temp["status"] = root_["status"];

  return temp.toStyledString();
}

std::string SensorConfigManager::get_config_param_active() {
  extra_root_["get_config_param_active"]["lidar_mode"] = root_["lidar_mode"];
  return extra_root_["get_config_param_active"].toStyledString();
}

std::string SensorConfigManager::get_beam_intrinsics() const {
  Json::Value temp;
  temp["beam_altitude_angles"] = root_["beam_altitude_angles"];
  temp["beam_azimuth_angles"] = root_["beam_azimuth_angles"];
  temp["lidar_origin_to_beam_origin_mm"] =
      root_["lidar_origin_to_beam_origin_mm"];

  return temp.toStyledString();
}

std::string SensorConfigManager::get_imu_intrinsics() const {
  Json::Value temp;
  temp["imu_to_sensor_transform"] = root_["imu_to_sensor_transform"];

  return temp.toStyledString();
}

std::string SensorConfigManager::get_lidar_intrinsics() const {
  Json::Value temp;
  temp["lidar_to_sensor_transform"] = root_["lidar_to_sensor_transform"];

  return temp.toStyledString();
}

std::string SensorConfigManager::get_lidar_data_format() const {
  return root_["data_format"].toStyledString();
}

std::string SensorConfigManager::get_calibration_status() const {
  return extra_root_["get_calibration_status"].toStyledString();
}

}  // namespace ouster_pcap_replay
