#include <ctime>
#include "processor.h"

Processor::Processor(const BulkQueueShared_t& consoleQueue, const BulkQueueShared_t& fileQueue) 
    : m_consoleQueue(consoleQueue), m_fileQueue(fileQueue) {}

void Processor::Add(const std::string& cmd) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (current_.commands.empty()) {
        current_.timestamp = std::time(nullptr);
    }
    current_.commands.push_back(cmd);
}

void Processor::Finish() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (current_.commands.empty()) {
        return;
    }

    if (m_consoleQueue) {
        m_consoleQueue->push(current_);
    }
    
    if (m_fileQueue) {
        m_fileQueue->push(current_);
    }

    current_.commands.clear();
}