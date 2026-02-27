#pragma once

#include <vector>
#include <memory>
#include <mutex>
#include "concurrentQueue.h"
#include "iLogger.h"

using BulkQueue_t = ConcurrentQueue<Bulk>;   
using BulkQueueShared_t = std::shared_ptr<BulkQueue_t>;
using BulkQueueWeak_t = std::weak_ptr<BulkQueue_t>;

class Processor {
public:
    explicit Processor(const BulkQueueShared_t& consoleQueue, const BulkQueueShared_t& fileQueue);
    void Add(const std::string& cmd);
    void Finish();

private:
    std::mutex mutex_;
    Bulk current_;
    BulkQueueShared_t consoleQueue_;
    BulkQueueShared_t fileQueue_;
};