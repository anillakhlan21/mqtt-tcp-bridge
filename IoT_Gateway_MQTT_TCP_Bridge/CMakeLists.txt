cmake_minimum_required(VERSION 3.16.0)
project(MQTT_TCP_Gateway)
# set(tools 
# /home/rituraj/SDK/RUT956/RUT9M_R_GPL_00.07.14.2/rutos-ramips-rut9m-sdk/staging_dir/toolchain-mipsel_24kc_gcc-8.4.0_musl)
# set(CMAKE_C_COMPILER ${tools}/bin/mispel-openwrt-linux-gcc) 
# set(CMAKE_CXX_COMPILER ${tools}/bin/mipsel-openwrt-linux-g++)
set(Libs libssl.so libcrypto.so libcurl.so libmodbus.so libnghttp2.so libstdc++.so.6 libjsoncpp.so.24)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread") 
# link_directories(/home/rituraj/SDK/RUT956/RUT9M_R_GPL_00.07.14.2/rutos-ramips-rut9m-sdk/staging_dir/target-mipsel_24kc_musl/usr/lib) 
# link_directories(/home/rituraj/SDK/RUT956/RUT9M_R_GPL_00.07.14.2/rutos-ramips-rut9m-sdk/staging_dir/toolchain-mipsel_24kc_gcc-8.4.0_musl/lib) 
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

include_directories(${PROJECT_SOURCE_DIR})
include_directories(lib/curl) 
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(/opt/homebrew/include) # Add Homebrew include path for nlohmann/json
link_directories(/opt/homebrew/lib) # Add Homebrew lib path for Mosquitto

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
#target_link_libraries(${PROJECT_NAME} ${Libs})
