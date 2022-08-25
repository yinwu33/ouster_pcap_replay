#include <signal.h>
#include <functional>

#include "sensor_config_manager.hpp"
#include "ouster_server.hpp"

using namespace ouster_pcap_replay;


int main(int argc, char** argv) {
  std::string config_file = "./config/ouster.json";
  std::string extra_config_file = "./config/extra_ouster.json";

  if (argc == 3) {
    config_file = std::string(argv[1]);
    extra_config_file = std::string(argv[2]);
  }
  std::cout << "Using Server Config File: " << config_file << std::endl;


  SensorConfigManager::Ptr manager = createSensorConfigManager(config_file, extra_config_file);

  OusterServer server(manager);

  server.run();
}