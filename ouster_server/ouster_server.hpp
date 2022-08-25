#pragma once

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "sensor_config_manager.hpp"

// #include "metadata.hpp"

namespace ouster_pcap_replay
{
constexpr int port = 7501;
class OusterServer {

  using SOCKET = int;

public:
  explicit OusterServer(const SensorConfigManager::Ptr manager);

  ~OusterServer() = default;

  void run();

  void close();

private:
  void init();

  void waitForConnection();

private:
  SOCKET fd_{ -1 };

  SOCKET client_fd_{ -1 };

  struct sockaddr_in client_addr_;

  SensorConfigManager::Ptr manager_;

  bool doSave{ true };
};
} // namespace fake_ouster
