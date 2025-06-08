#include "bridge_utils.h"
#include "tcp_client.h"
#include "mqtt_bridge.h"
#include <sys/types.h>
#include <sys/socket.h>

int Utility::writeToTcpSocket(const std::string& message, int socketFd) {
    ssize_t bytesSent = send(socketFd, message.c_str(), message.size(), 0);
    if (bytesSent < 0) {
        perror("Error sending message to TCP socket");
        return -1;
    }
    return static_cast<int>(bytesSent);
}
int Utility::writeToMqtt(const std::string& message) {
    // Placeholder for MQTT write logic
    // This function should interact with the MQTT client to publish the message
    // For now, we just print the message to simulate writing to MQTT
    std::cout << "Writing to MQTT: " << message << std::endl;
    return static_cast<int>(message.size());
}