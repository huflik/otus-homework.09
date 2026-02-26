#include "fileLogger.h"
#include <fstream>
#include <sstream>
#include <thread>

FileLogger::FileLogger(const BulkQueueShared_t& queue) 
    : m_queue(queue) {}

FileLogger::~FileLogger() {
    stop();
}

void FileLogger::start() {
    if (!m_running) {
        m_running = true;
        m_thread = std::thread(&FileLogger::process, this);
    }
}

void FileLogger::stop() {
    if (m_running) {
        m_running = false;
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }
}

std::string FileLogger::generateFilename(const Bulk& bulk) {
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return "bulk" + std::to_string(bulk.timestamp) + "-" + 
           ss.str() + "-" + std::to_string(m_counter++) + ".log";
}

void FileLogger::process() {
    auto queue = m_queue.lock();
    if (!queue) return;
    
    while (queue->pop(m_bulk)) {
        std::ofstream out(generateFilename(m_bulk));
        for (size_t i = 0; i < m_bulk.commands.size(); ++i) {
            if (i != 0) out << ", ";
            out << m_bulk.commands[i];
        }
    }
}