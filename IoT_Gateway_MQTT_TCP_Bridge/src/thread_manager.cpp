#include "thread_manager.h"

ThreadManager::ThreadManager() : running(false) {}

void ThreadManager::start(std::function<void()> task) {
    running = true;
    worker = std::thread([this, task]() {
        while (running) {
            task();
        }
    });
}

void ThreadManager::stop() {
    running = false;
    if (worker.joinable()) {
        worker.join();
    }
}

ThreadManager::~ThreadManager() {
    stop();
}