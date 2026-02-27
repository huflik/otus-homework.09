#pragma once

#include <thread>
#include <atomic>
#include <mutex>
#include "concurrentQueue.h"
#include "iLogger.h"

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
    
    std::mutex mutex_;
    BulkQueueWeak_t queue_;
    Bulk bulk_;
    std::thread thread_;
    std::atomic<size_t> counter_{0};
    bool running_{false};
};