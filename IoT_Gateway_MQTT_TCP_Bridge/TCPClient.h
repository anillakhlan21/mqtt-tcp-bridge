#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <string>

class TCPClient {
public:
    virtual void connect(const std::string& ip, int port) = 0;
    virtual void send(const std::string& message) = 0;
    virtual std::string receive() = 0;
    virtual ~TCPClient() = default;
};

#endif // TCPCLIENT_H