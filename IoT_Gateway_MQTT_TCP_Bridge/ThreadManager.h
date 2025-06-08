#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <thread>
#include <atomic>
#include <functional>

class ThreadManager {
public:
    ThreadManager();
    void start(std::function<void()> task);
    void stop();
    ~ThreadManager();

private:
    std::thread worker;
    std::atomic<bool> running;
};

#endif // THREADMANAGER_H