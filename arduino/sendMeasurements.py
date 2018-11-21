#!/usr/bin/python
import socket
import argparse
import time
import json
from datetime import datetime

parser = argparse.ArgumentParser(description="Send measurement data via python script since the Arduino UDP module does not work on Yun")
parser.add_argument("--temp", required=True, help="Temperature")
parser.add_argument("--hum", required=True, help="Humidity")
args = parser.parse_args()

# Create a separate config file so we can put this in GIT
with open("config.json") as f:
    config_data = json.load(f)

dt = datetime.now()
DATE = dt.strftime("%Y-%m-%d %H:%M")
UDP_IP = config_data["ip"]
UDP_PORT = config_data["port"]
MESSAGE = DATE + ", " + config_data["sensor_id"] + ", " + str(args.temp) + ", " + str(args.hum)

# Debug prints
print("UDP target Ip: ", UDP_IP)
print("UDP target Port: ", UDP_PORT)
print("Message: ", MESSAGE)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(MESSAGE, (UDP_IP, int(UDP_PORT)))

