#pragma once

#include "iLogger.h"
#include <thread>
#include <atomic>
#include "concurrentQueue.h"

using BulkQueue_t = ConcurrentQueue<Bulk>;   
using BulkQueueShared_t = std::shared_ptr<BulkQueue_t>;
using BulkQueueWeak_t = std::weak_ptr<BulkQueue_t>;

class FileLogger : public ILogger {
public:
    explicit FileLogger(const BulkQueueShared_t& queue);
    ~FileLogger() override;
    
    void start() override;
    void stop() override;

private:
    void process();
    std::string generateFilename(const Bulk& bulk);
    
    BulkQueueWeak_t m_queue;
    Bulk m_bulk;
    std::thread m_thread;
    std::atomic<size_t> m_counter{0};
    bool m_running{false};
};