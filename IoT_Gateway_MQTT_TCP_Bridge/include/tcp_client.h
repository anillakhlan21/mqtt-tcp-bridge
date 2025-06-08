#ifndef TCPCLIENTIMPL_H
#define TCPCLIENTIMPL_H

#include "TCPClient.h"
#include <string>

class TCPClientImpl : public TCPClient {
public:
    TCPClientImpl();
    void connect(const std::string& ip, int port) override;
    void send(const std::string& message) override;
    std::string receive() override;
    ~TCPClientImpl();

private:
    int sockfd;
};

#endif // TCPCLIENTIMPL_H