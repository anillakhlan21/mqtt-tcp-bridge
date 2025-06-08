#ifndef __MQTT_H__
#define __MQTT_H__

#include <mosquitto.h>
#include <string>
#include "controllercommand.h"
#include <functional>

using std::string;
class Mqtt
{
  public:
    Mqtt(const char* clientId, const char* host, int port, int keepAlive, const char* userName, const char* password, bool cleanSession, string publish_topic);
    ~Mqtt();

    void loop();
    void publishMessage(string payload, int qos = 0, int retain = 0);
    void subscribeTopic(const char* topic, int qos = 0);
    void setOnMessageCallback(std::function<void(const std::string&)> cb);

  private:
    void initializeMosquittoLib();
    void disconnect_callback(struct mosquitto* mosq, void* userdata, int rc);
    void connect_callback(struct mosquitto* mosq, void* userdata, int result);
    mosquitto* createMosquittoClientInstance(const char* clientId, bool cleanSession);
    void setUserNameAndPassword(mosquitto* mosq, const char* username, const char* password);
    bool connect(mosquitto* mosq, const char* host, int port, int keepAlive);
    bool reconnect();
    void on_message(const struct mosquitto_message* message);

    mosquitto* mosq;
    const char* clientId;
    int port;
    int keepAlive;
    const char* host;
    const char* topic;
    std::function<void(const std::string&)> onMessageCallback;
};

#endif