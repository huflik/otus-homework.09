#include "consoleLogger.h"
#include <iostream>

ConsoleLogger::ConsoleLogger(const BulkQueueShared_t& queue) 
    : m_queue(queue) {}

ConsoleLogger::~ConsoleLogger() {
    stop();
}

void ConsoleLogger::start() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!m_running) {
        m_running = true;
        m_thread = std::thread(&ConsoleLogger::process, this);
    }
}

void ConsoleLogger::stop() {
    std::thread threadToJoin;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!m_running) return;
        m_running = false;
        threadToJoin = std::move(m_thread);
    }
    
    if (threadToJoin.joinable()) {
        threadToJoin.join();
    }
}

void ConsoleLogger::process() {
    auto queue = m_queue.lock();
    if (!queue) return;
    
    while (queue->pop(m_bulk)) {
        std::cout << "bulk: ";
        for (size_t i = 0; i < m_bulk.commands.size(); ++i) {
            if (i != 0) std::cout << ", ";
            std::cout << m_bulk.commands[i];
        }
        std::cout << '\n' << std::flush;
    }
}