#include <iostream>
#include "consoleLogger.h"

ConsoleLogger::ConsoleLogger(const BulkQueueShared_t& queue) 
    : queue_(queue) {}

ConsoleLogger::~ConsoleLogger() {
    stop();
}

void ConsoleLogger::start() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!running_) {
        running_ = true;
        thread_ = std::thread(&ConsoleLogger::process, this);
    }
}

void ConsoleLogger::stop() {
    std::thread threadToJoin;
    
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!running_) return;
        running_ = false;
        threadToJoin = std::move(thread_);
    }
    
    if (threadToJoin.joinable()) {
        threadToJoin.join();
    }
}

void ConsoleLogger::process() {
    auto queue = queue_.lock();
    if (!queue) return;
    
    while (queue->pop(bulk_)) {
        std::cout << "bulk: ";
        for (size_t i = 0; i < bulk_.commands.size(); ++i) {
            if (i != 0) std::cout << ", ";
            std::cout << bulk_.commands[i];
        }
        std::cout << '\n' << std::flush;
    }
}