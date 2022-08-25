import argparse
import yaml
import os

from PyQt5.QtCore import QProcess


class TcpReplayer:
    def __init__(self, config_path: str) -> None:
        # load configurations
        with open(config_path, "r") as f:
            config = yaml.load(f)
        self.config = config

        self.ouster_config_path = os.path.abspath(config["ouster_config"])

        self.interface = config["network"]["interface"]
        self.dest_mac = config["network"]["dest_mac"]
        self.new_dest_ip = config["network"]["new_dest_ip"]

        self.pcap_file_path = config["pcap_file"]["path"]
        self.old_dest_ip = config["pcap_file"]["old_dest_ip"]
        self.old_dest_port = config["pcap_file"]["old_dest_port"]

        # create a process
        self.play_process = QProcess()
        self.play_process.setProcessChannelMode(QProcess.ForwardedChannels)

    def replay(self) -> None:
        # get the newest lidar port from os server
        with open(self.ouster_config_path, "r") as f:
            os_server_config = yaml.load(f)

        new_dest_port = os_server_config["get_config_param_active"]["udp_port_lidar"]

        cmd = f"sudo tcpreplay-edit -i {self.interface} -l 0 --dstipmap {self.old_dest_ip}:{self.new_dest_ip} --portmap {self.old_dest_port}:{new_dest_port} --enet-dmac={self.dest_mac} {self.pcap_file_path}"
        print(cmd)

        # play and wait for finishing
        self.play_process.start(cmd)
        self.play_process.waitForFinished(-1)


if __name__ == "__main__":
    print(os.path.abspath("./config/tcpreplay.yaml"))
    player = TcpReplayer(os.path.abspath("./config/tcpreplay.yaml"))
    player.replay()
