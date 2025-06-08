#include <iostream>
#include <thread>
#include <atomic>
#include "MQTT.h"
#include "TCPClient.h"
#include "TCPClientImpl.h"
#include "ThreadManager.h"

int main() {
    try {
        std::cout << "IoT Gateway MQTT-TCP Bridge Running..." << std::endl;

        Mqtt mqttClient("clientId", "localhost", 1883, 60, "username", "password", true, "iot/data");
        TCPClientImpl tcpClient;

        mqttClient.subscribeTopic("iot/data", 1);
        tcpClient.connect("127.0.0.1", 8080);

        std::atomic<bool> running(true);

        // Thread for listening to MQTT messages
        std::thread mqttThread([&]() {
            while (running) {
                std::string mqttMessage = mqttClient.receiveMessage();
                if (!mqttMessage.empty()) {
                    std::cout << "MQTT Message Received: " << mqttMessage << std::endl;
                }
            }
        });

        // Thread for listening to TCP socket
        std::thread tcpThread([&]() {
            while (running) {
                std::string tcpMessage = tcpClient.receive();
                mqttClient.publishMessage(tcpMessage, 1, 0);
                if (!tcpMessage.empty()) {
                    std::cout << "TCP Message Received: " << tcpMessage << std::endl;
                }
            }
        });

        mqttThread.join();
        tcpThread.join();
        
        // Wait for threads to finish (placeholder for actual termination logic)
        std::cout << "Press Enter to stop..." << std::endl;
        std::cin.get();
        running = false;

       

    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    return 0;
}