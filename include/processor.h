#pragma once

#include "iLogger.h"
#include <vector>
#include <memory>
#include "concurrentQueue.h"

using BulkQueue_t = ConcurrentQueue<Bulk>;   
using BulkQueueShared_t = std::shared_ptr<BulkQueue_t>;
using BulkQueueWeak_t = std::weak_ptr<BulkQueue_t>;

class Processor {
public:
    explicit Processor(const BulkQueueShared_t& consoleQueue, const BulkQueueShared_t& fileQueue);
    
    //void Subscribe(std::shared_ptr<ILogger> logger); // оставляем для обратной совместимости
    void Add(const std::string& cmd);
    void Finish();

private:
    Bulk current_;
    BulkQueueShared_t m_consoleQueue;
    BulkQueueShared_t m_fileQueue;
};