#include "tcp_client.h"
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <arpa/inet.h>

TCPClientImpl::TCPClientImpl() : sockfd(-1) {}

void TCPClientImpl::connect(const std::string& ip, int port) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) throw std::runtime_error("Socket creation failed");

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    if (::connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        throw std::runtime_error("TCP Connect failed");
    }
}

void TCPClientImpl::send(const std::string& message) {
    if (write(sockfd, message.c_str(), message.size()) < 0) {
        throw std::runtime_error("TCP Send failed");
    }
}

std::string TCPClientImpl::receive() {
    char buffer[1024] = {0};
    int n = read(sockfd, buffer, sizeof(buffer));
    if (n < 0) throw std::runtime_error("TCP Receive failed");
    return std::string(buffer, n);
}

TCPClientImpl::~TCPClientImpl() {
    if (sockfd != -1) close(sockfd);
}