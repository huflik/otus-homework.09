#include <ctime>
#include "processor.h"

Processor::Processor(const BulkQueueShared_t& consoleQueue, const BulkQueueShared_t& fileQueue) 
    : consoleQueue_(consoleQueue), fileQueue_(fileQueue) {}

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

    if (consoleQueue_) {
        consoleQueue_->push(current_);
    }
    
    if (fileQueue_) {
        fileQueue_->push(current_);
    }

    current_.commands.clear();
}