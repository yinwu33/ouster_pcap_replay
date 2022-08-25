#pragma once

#include <jsoncpp/json/json.h>

#include <memory>
#include <string>

#include "parameter_base.hpp"

namespace ouster_pcap_replay {

class SensorConfigManager : public util_json::ParameterBase {
 public:
  using Ptr = std::shared_ptr<SensorConfigManager>;

  SensorConfigManager(const std::string& config_file, const std::string& extra_config_file);

  void getResponse(const std::string& request, std::string& response);

  bool setConfig(const std::string& key, const std::string& value);

  void saveConfig();

  // void loadConfig();

  void initConfigFromOuster();  // todo

  bool configChanged{false};

 private:
  std::string reinitialize() const;

  std::string get_sensor_info() const;

  std::string get_config_param_active();

  std::string get_beam_intrinsics() const;

  std::string get_imu_intrinsics() const;

  std::string get_lidar_intrinsics() const;

  std::string get_lidar_data_format() const;

  std::string get_calibration_status() const;
};

inline SensorConfigManager::Ptr createSensorConfigManager(
    const std::string& config_file, const std::string& extra_config_file) {
  return std::make_shared<SensorConfigManager>(config_file, extra_config_file);
}

}  // namespace ouster_pcap_replay