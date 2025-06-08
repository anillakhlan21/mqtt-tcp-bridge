#include "MQTT.h"
// #include <stdio.h>
#include <iostream>
#include <cstring>   // for strerror
#include <unistd.h>  // Include for sleep function on Unix/Linux
#include <thread>
#include <chrono>

using std::cout;
using std::endl;
using std::string;

Mqtt::Mqtt( const char* clientId, const char* host, int port, int keepAlive, const char* userName, const char* password, bool cleanSession, string publish_topic )
{
    mosquitto* mosqClient = createMosquittoClientInstance( clientId, cleanSession );
    setUserNameAndPassword( mosqClient, userName, password );
    bool isConnected = false;
    while ( !isConnected )
    {
        isConnected = connect( mosqClient, host, port, keepAlive );
        std::this_thread::sleep_for( std::chrono::seconds( 5 ) );
    }
    mosq = mosqClient;
    topic = strdup( publish_topic.c_str() );
}

void Mqtt::disconnect_callback( struct mosquitto* mosq, void* userdata, int rc )
{
    if ( rc == MOSQ_ERR_SUCCESS )
    {
        cout << "Disconnected from broker" << endl;
        int retry = 5;
        while ( retry > 0 )
        {
            cout << "Reconnection to broker failed, retrying..." << endl;
            retry--;
            sleep( 5 );  // Wait before retrying
            if ( reconnect() )
            {
                break;  // Exit the loop if reconnection is successful
            }
        }
    }
}

void Mqtt::connect_callback( struct mosquitto* mosq, void* userdata, int result )
{
    if ( result == 0 )
    {
        cout << "Connected to broker" << endl;
    }
    else
    {
        cout << "Connection failed: " << mosquitto_connack_string( result ) << endl;
        throw std::runtime_error( "An error occurred in connect_callback function" );
    }
}

mosquitto* Mqtt::createMosquittoClientInstance( const char* clientId, bool cleanSession )
{
    mosquitto* mosqClient = mosquitto_new( clientId, cleanSession, nullptr );
    if ( !mosqClient )
    {
        if ( errno == ENOMEM )
        {
            std::cerr << "Error: Out of memory while creating mosquitto instance" << std::endl;
            throw std::runtime_error( "An error occurred in createMosquittoClientInstance function" );
        }
        else if ( errno == EINVAL )
        {
            std::cerr << "Error: Invalid input parameters while creating mosquitto instance." << std::endl;
            throw std::runtime_error( "An error occurred in createMosquittoClientInstance function" );
        }
        else
        {
            std::cerr << "Error: " << strerror( errno ) << std::endl;
            throw std::runtime_error( "An error occurred in createMosquittoClientInstance function" );
        }
    }
    // mosquitto_disconnect_callback_set( mosqClient, disconnect_callback );
    // mosquitto_connect_callback_set( mosqClient, connect_callback );
    return mosqClient;
}

void Mqtt::initializeMosquittoLib()
{
    if ( mosquitto_lib_init() != MOSQ_ERR_SUCCESS )
    {
        cout << "Error: Unable to initialize Mosquitto library." << endl;
        throw std::runtime_error( "An error occurred in initializeMosquittoLib function" );
    }
}

bool Mqtt::connect( mosquitto* mosq, const char* host, int port, int keepAlive )
{
    int rc = mosquitto_connect( mosq, host, port, keepAlive );

    switch ( rc )
    {
        case MOSQ_ERR_SUCCESS:
            cout << "Successfully connected to mqtt broker" << endl << endl;
            return true;
            break;
        case MOSQ_ERR_INVAL:
            cout << "MQTT Error:: Invalid mqtt parameters (check mosq/host/port/keepAlive)" << endl << endl;
            // throw std::runtime_error( "An error occurred in connect function" );
            return false;
            break;
        case MOSQ_ERR_ERRNO:
            cout << "MQTT Error: " << strerror( errno ) << endl << endl;
            // throw std::runtime_error( "An error occurred in connect function" );
            return false;
            break;
        default:
            cout << "MQTT Error: " << strerror( errno ) << endl << endl;
            // throw std::runtime_error( "An error occurred in connect function" );
            return false;
            break;
    }
}

void Mqtt::setUserNameAndPassword( mosquitto* mosq, const char* username, const char* password )
{
    int rc = mosquitto_username_pw_set( mosq, username, password );
    switch ( rc )
    {
        case MOSQ_ERR_SUCCESS:
            cout << "Info: Setting mqtt username and password successful" << endl;
            break;
        case MOSQ_ERR_INVAL:
            cout << "Error: Input parameter invalid while setting mqtt username and password" << endl;
            throw std::runtime_error( "An error occurred in setUserNameAndPassword function" );

            break;
        case MOSQ_ERR_NOMEM:
            cout << "Error: Out of memory while setting username/password for mqtt" << endl;
            throw std::runtime_error( "An error occurred in setUserNameAndPassword function" );

            break;
        default:
            break;
    }
}

void Mqtt::publishMessage( string payload, int qos, int retain )
{
    int rc = mosquitto_publish( mosq, NULL, topic, payload.length(), payload.c_str(), 0, 0 );
    switch ( rc )
    {
        case MOSQ_ERR_SUCCESS:
            std::cout << "Message published successfully" << std::endl;
            break;
        case MOSQ_ERR_INVAL:
            std::cerr << "Error: Invalid parameters for mqtt publish" << std::endl;
            break;
        case MOSQ_ERR_NOMEM:
            std::cerr << "Error: Out of memory while publishing message" << std::endl;
            break;
        case MOSQ_ERR_NO_CONN:
            std::cerr << "Error:  not connected to a broker" << std::endl;
            reconnect();
            break;
        case MOSQ_ERR_PROTOCOL:
            std::cerr << "Error: Protocol error during publish" << std::endl;
            break;
        case MOSQ_ERR_PAYLOAD_SIZE:
            std::cerr << "Error: Payload is too large" << std::endl;
            break;
        default:
            std::cerr << "Error: Unknown error during publish" << std::endl;
            std::cerr << "Error ( rc :" << rc << " ) :" << strerror( errno ) << std::endl;
            break;
    }
}

void Mqtt::subscribeTopic(const char *topic, int qos)
{
    int rc = mosquitto_subscribe(mosq, NULL, topic, qos);
    switch (rc)
    {
        case MOSQ_ERR_SUCCESS:
            std::cout << "Subscribed to topic: " << topic << std::endl;
            break;
        case MOSQ_ERR_INVAL:
            std::cerr << "Error: Invalid parameters for mqtt subscribe" << std::endl;
            break;
        case MOSQ_ERR_NOMEM:
            std::cerr << "Error: Out of memory while subscribing to topic" << std::endl;
            break;
        case MOSQ_ERR_NO_CONN:
            std::cerr << "Error: Not connected to a broker" << std::endl;
            reconnect();
            break;
        default:
            std::cerr << "Error: Unknown error during subscribe" << std::endl;
            break;
    }
}

bool Mqtt::reconnect()
{
    cout << "Reconnecting..." << endl;
    int rc = mosquitto_reconnect( mosq );
    switch ( rc )
    {
        case MOSQ_ERR_SUCCESS:
            std::cout << "Reconnected to the MQTT broker" << std::endl;
            return true;
            break;
        case MOSQ_ERR_NO_CONN:
            std::cerr << "Error: Not connected to a broker" << std::endl;
            // throw std::runtime_error( "An error occurred in reconnect function" );
            return false;
            break;
        case MOSQ_ERR_ERRNO:
            std::cerr << "Error: " << strerror( errno ) << std::endl;
            // throw std::runtime_error( "An error occurred in reconnect function" );
            return false;

            break;
        default:
            std::cerr << "Error: Unknown error during reconnect" << std::endl;
            // throw std::runtime_error( "An error occurred in reconnect function" );
            return false;

            break;
    }
}

void Mqtt::on_message(const mosquitto_message *message)
{
    if (message && message->payload) {
        std::string topic = message->topic;
        std::string payload(static_cast<char *>(message->payload), message->payloadlen);
    }
    else {
        std::cerr << "Error: Received message is null or empty" << std::endl;
    }
}

void Mqtt::loop()
{
    mosquitto_loop( mosq, -1, 1 );
}

Mqtt::~Mqtt()
{
    mosquitto_disconnect( mosq );
    mosquitto_destroy( mosq );
    mosquitto_lib_cleanup();

    if ( topic != nullptr )
    {
        free( const_cast< char* >( topic ) );
        topic = nullptr;  // Set to nullptr to avoid potential double deletion
    }
}