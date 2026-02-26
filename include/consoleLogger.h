#pragma once

#include "iLogger.h"
#include <thread>
#include "concurrentQueue.h"

using BulkQueue_t = ConcurrentQueue<Bulk>;   
using BulkQueueShared_t = std::shared_ptr<BulkQueue_t>;
using BulkQueueWeak_t = std::weak_ptr<BulkQueue_t>;

class ConsoleLogger : public ILogger {
public:
    explicit ConsoleLogger(const BulkQueueShared_t& queue);
    ~ConsoleLogger() override;
    
    void start() override;
    void stop() override;

private:
    void process();
    
    std::mutex mutex_;
    BulkQueueWeak_t m_queue;
    Bulk m_bulk;
    std::thread m_thread;
    bool m_running{false};
};