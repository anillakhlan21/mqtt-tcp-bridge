#ifndef UTILITY_H
#define UTILITY_H
#include <string>
#include <iostream>

class Utility {
public:
    static int writeToTcpSocket(const std::string& message, int socketFd);
    static int writeToMqtt(const std::string& message);
};