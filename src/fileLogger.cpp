#include <fstream>
#include <sstream>
#include <thread>
#include "fileLogger.h"

FileLogger::FileLogger(const BulkQueueShared_t& queue) 
    : queue_(queue) {}

FileLogger::~FileLogger() {
    stop();
}

void FileLogger::start() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!running_) {
        running_ = true;
        thread_ = std::thread(&FileLogger::process, this);
    }
}

void FileLogger::stop() {
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

std::string FileLogger::generateFilename(const Bulk& bulk) {
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return "bulk" + std::to_string(bulk.timestamp) + "-" + 
           ss.str() + "-" + std::to_string(counter_++) + ".log";
}

void FileLogger::process() {
    auto queue = queue_.lock();
    if (!queue) return;
    
    while (queue->pop(bulk_)) {
        std::ofstream out(generateFilename(bulk_));
        for (size_t i = 0; i < bulk_.commands.size(); ++i) {
            if (i != 0) out << ", ";
            out << bulk_.commands[i];
        }
    }
}