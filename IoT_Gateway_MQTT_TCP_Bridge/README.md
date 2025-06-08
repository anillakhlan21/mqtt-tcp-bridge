# IoT Gateway MQTT-TCP Bridge

## Overview
This C++ application is designed for OpenWRT IoT gateways. It connects to an MQTT broker and TCP port, enabling bi-directional data flow using multithreading.

## Features
- Modular code with Single Responsibility Principle
- Threading with synchronization
- Robust exception handling
- No memory leaks

## Build & Run Instructions

### Prerequisites
- OpenWRT SDK or GCC for OpenWRT
- VS Code with C++ extension
- MQTT and TCP libraries as needed

### Build
```sh
g++ -std=c++17 main.cpp ThreadManager.cpp -o iot_gateway_app -lpthread
```

### Run
```sh
./iot_gateway_app
```
# sudo make -j1 V=s package/IoT_Gateway_MQTT_TCP_Bridge/compile
# rm -rf /home/rituraj/SDK/RUT956/RUT9M_R_GPL_00.07.14.2/rutos-ramips-rut9m-sdk/build_dir/target-mipsel_24kc_musl/IoT_Gateway_MQTT_TCP_Bridge-1.0/
# cd /SDK/RUT956/RUT9M_R_GPL_00.07.14.2/rutos-ramips-rut9m-sdk


<!-- mosquitto_pub -h localhost -t iot/data -m "Hello from MQTT" -->
<!-- mosquitto_sub -h localhost -t iot/data -->