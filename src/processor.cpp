#include "processor.h"
#include <ctime>

Processor::Processor(const BulkQueueShared_t& consoleQueue, const BulkQueueShared_t& fileQueue) 
    : m_consoleQueue(consoleQueue), m_fileQueue(fileQueue) {}

//void Processor::Subscribe(std::shared_ptr<ILogger> logger) {
    // Оставляем для совместимости, но не используем
//   loggers_.push_back(logger);
//}

void Processor::Add(const std::string& cmd) {
    if (current_.commands.empty()) {
        current_.timestamp = std::time(nullptr);
    }
    current_.commands.push_back(cmd);
}

void Processor::Finish() {
    if (current_.commands.empty()) {
        return;
    }

    // Отправляем в обе очереди
    if (m_consoleQueue) {
        m_consoleQueue->push(current_);
    }
    
    if (m_fileQueue) {
        m_fileQueue->push(current_);
    }

    current_.commands.clear();
}