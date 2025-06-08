#pragma once
#include <string>
#include <nlohmann/json.hpp>

struct MqttConfig {
    std::string clientId;
    std::string host;
    int port;
    int keepAlive;
    std::string username;
    std::string password;
    std::string subscribe_topic;
    std::string publish_topic;
};

struct TcpConfig {
    std::string host;
    int port;
};

struct AppConfig {
    MqttConfig mqtt;
    TcpConfig tcp;
};

AppConfig load_config(const std::string& filename);
