cmake_minimum_required(VERSION 3.16.0)
project(MQTT_TCP_Gateway)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)
add_executable(IoT_Gateway_MQTT_TCP_Bridge
    src/gateway_main.cpp
    src/thread_manager.cpp
    src/tcp_client.cpp
    src/mqtt_bridge.cpp
    src/bridge_utils.cpp
    src/bridge_config.cpp
)

target_link_libraries(IoT_Gateway_MQTT_TCP_Bridge
    pthread
    mosquitto
)
