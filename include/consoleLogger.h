#pragma once

#include <thread>
#include "concurrentQueue.h"
#include "iLogger.h"

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
    BulkQueueWeak_t queue_;
    Bulk bulk_;
    std::thread thread_;
    bool running_{false};
};