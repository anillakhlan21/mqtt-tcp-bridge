#include <iostream>
#include <thread>
#include <atomic>
#include "mqtt_bridge.h"
#include "tcp_client.h"
#include "thread_manager.h"
#include "bridge_config.h"

int main() {
    try {
        AppConfig config = load_config("config.json");
        std::cout << "IoT Gateway MQTT-TCP Bridge Running..." << std::endl;

        Mqtt mqttClient(
            config.mqtt.clientId.c_str(),
            config.mqtt.host.c_str(),
            config.mqtt.port,
            config.mqtt.keepAlive,
            config.mqtt.username.c_str(),
            config.mqtt.password.c_str(),
            true,
            config.mqtt.publish_topic // use publish_topic for publishing
        );
        TCPClientImpl tcpClient;

        mqttClient.subscribeTopic(config.mqtt.subscribe_topic.c_str(), 1);
        tcpClient.connect(config.tcp.host.c_str(), config.tcp.port);

        // Set up MQTT-to-TCP forwarding with command mapping
        mqttClient.setOnMessageCallback([&tcpClient, &config](const std::string& msg) {
            try {
                auto it = config.commands.find(msg);
                if (it != config.commands.end()) {
                    tcpClient.send(it->second);
                    std::cout << "Mapped MQTT command '" << msg << "' to TCP value: " << it->second << std::endl;
                } else {
                    tcpClient.send(msg);
                    std::cout << "Forwarded raw MQTT message to TCP: " << msg << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Failed to forward MQTT message to TCP: " << e.what() << std::endl;
            }
        });

        std::atomic<bool> running(true);

        // Thread for processing MQTT events (including receiving messages)
        std::thread mqttLoopThread([&]() {
            while (running) {
                mqttClient.loop();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
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

        // Wait for threads to finish (placeholder for actual termination logic)
        std::cout << "Press Enter to stop..." << std::endl;
        std::cin.get();
        running = false;
        mqttLoopThread.join();
        tcpThread.join();

    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    return 0;
}