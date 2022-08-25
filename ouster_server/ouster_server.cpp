#include "ouster_server.hpp"

namespace ouster_pcap_replay
{
OusterServer::OusterServer(const SensorConfigManager::Ptr manager) : manager_(manager) {
  init();
}

void OusterServer::close() { // ! not working
  std::cout << "Good Bye" << std::endl;
  ::close(fd_);
}

void OusterServer::init() {
  // 1. create socket
  fd_ = socket(AF_INET, SOCK_STREAM, 0);

  if (fd_ == -1) {
    std::cerr << "Create TCP Socket Error";
    throw;
  }
  std::cout << "Create Socket wit id: " << fd_ << std::endl;

  // 2. bind port
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(static_cast<uint16_t>(port));

  if (bind(fd_, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
    std::cerr << "Bind to Port Error: " << port;
    throw;
  }

  std::cout << "Server Bind to IP: " << inet_ntoa(server_addr.sin_addr) << " Port: " << ntohs(server_addr.sin_port) << std::endl;

  // 3. set listen mode
  if (listen(fd_, 5)) {
    std::cerr << "Set Listen to 5 Error";
    throw;
  }
}

void OusterServer::run() {
  std::cout << "Waiting for Connection..." << std::endl;
  waitForConnection();

  char buff[1024 * 16];

  while (true) {
    memset(buff, 0, sizeof(buff));

    int ret = recv(client_fd_, buff, sizeof(buff), 0);

    if (ret < 0) {
      std::cerr << "Recv Error";
      throw;
    }
    else if (ret == 0) {
      std::cout << "\nConnection Break, Waiting for new Conenction..." << std::endl;
      waitForConnection();
      continue;
    }

    std::string req(buff);
    std::cout << "<-- Receive:" << strlen(req.c_str()) << "<-" << req << "<-" << std::endl;

    std::string resp;
    manager_->getResponse(req, resp);
    // getResponse(req, resp);
    memset(buff, 0, sizeof(buff));
    strcpy(buff, resp.c_str());
    std::cout << "--> Send: " << strlen(resp.c_str()) << "->" << resp << "->" << std::endl;

    if (send(client_fd_, buff, strlen(buff), 0) <= 0) {
      std::cout << "Send Error" << std::endl;
      continue;
    }
  }
}

void OusterServer::waitForConnection() {
  if (doSave && manager_->configChanged)
    manager_->saveConfig();

  ::close(client_fd_);

  int sock_len = sizeof(struct sockaddr_in);

  struct sockaddr_in client_addr_;

  client_fd_ = accept(fd_, (struct sockaddr*)&client_addr_, (socklen_t*)&sock_len); // todo why use &

  std::cout << "Client Connected with IP: " << inet_ntoa(client_addr_.sin_addr) << " Port: " << ntohs(client_addr_.sin_port) << std::endl;
}

} // namespace fake_ouster
