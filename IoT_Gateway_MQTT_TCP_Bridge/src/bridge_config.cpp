#include "bridge_config.h"
#include <fstream>

using nlohmann::json;

AppConfig load_config(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("Could not open config file");
    json j;
    file >> j;
    AppConfig config;
    config.mqtt.clientId = j["mqtt"].value("clientId", "");
    config.mqtt.host = j["mqtt"].value("host", "localhost");
    config.mqtt.port = j["mqtt"].value("port", 1883);
    config.mqtt.keepAlive = j["mqtt"].value("keepAlive", 60);
    config.mqtt.username = j["mqtt"].value("username", "");
    config.mqtt.password = j["mqtt"].value("password", "");
    config.mqtt.subscribe_topic = j["mqtt"].value("subscribe_topic", "iot/in");
    config.mqtt.publish_topic = j["mqtt"].value("publish_topic", "iot/out");
    config.tcp.host = j["tcp"].value("host", "127.0.0.1");
    config.tcp.port = j["tcp"].value("port", 8081);
    return config;
}
