#include <mutex>
#include <list>
#include <memory>
#include <thread>
#include <unordered_map>
#include "async.h"
#include "parser.h"
#include "processor.h"
#include "consoleLogger.h"
#include "fileLogger.h"

namespace async {

static BulkQueueShared_t consoleQueue = std::make_shared<BulkQueue_t>();
static BulkQueueShared_t fileQueue = std::make_shared<BulkQueue_t>();

static ConsoleLogger consoleLogger(consoleQueue);
static FileLogger fileLogger1(fileQueue);
static FileLogger fileLogger2(fileQueue);
static std::unordered_map<handle_t, std::string> contextBuffers;
static std::mutex buffersMutex;

struct Context {
    std::unique_ptr<Parser> parser;
    std::shared_ptr<Processor> processor;
};

static std::list<std::unique_ptr<Context>> contexts;
static std::mutex contextsMutex;

handle_t connect(std::size_t bulkSize) {
    static std::once_flag flag;
    std::call_once(flag, [](){
        consoleLogger.start();
        fileLogger1.start();
        fileLogger2.start();
    });
    
    std::lock_guard<std::mutex> lock(contextsMutex);
    
    auto context = std::make_unique<Context>();
    
    context->processor = std::make_shared<Processor>(consoleQueue, fileQueue);
    context->parser = std::make_unique<Parser>(bulkSize, context->processor);
    
    contexts.push_back(std::move(context));
    return contexts.back().get();
}

void receive(handle_t handle, const char *data, std::size_t size) {
    std::lock_guard<std::mutex> lock(contextsMutex);
    
    for (auto& ctx : contexts) {
        if (ctx.get() == handle) {

            std::string& buffer = contextBuffers[handle];
            buffer.append(data, size);
            
            size_t pos = 0;
            while (true) {
                size_t newline = buffer.find('\n', pos);
                if (newline == std::string::npos) break;
                
                std::string cmd = buffer.substr(pos, newline - pos);
                if (!cmd.empty()) {
                    ctx->parser->Process(cmd);
                }
                pos = newline + 1; 
            }
            
            if (pos < buffer.size()) {
                buffer = buffer.substr(pos);
            } else {
                buffer.clear();
            }
            break;
        }
    }
}


void disconnect(handle_t handle) {
    std::lock_guard<std::mutex> lock(contextsMutex);
    
    auto bufIt = contextBuffers.find(handle);
    if (bufIt != contextBuffers.end() && !bufIt->second.empty()) {
 
        for (auto& ctx : contexts) {
            if (ctx.get() == handle) {
                ctx->parser->Process(bufIt->second);
                break;
            }
        }
        contextBuffers.erase(bufIt);
    }
    
    auto it = contexts.begin();
    while (it != contexts.end()) {
        if ((*it).get() == handle) {
            (*it)->parser->Eof();
            contexts.erase(it);
            break;
        }
        ++it;
    }
}

void stop() {
    consoleQueue->stop();
    fileQueue->stop();
    
    consoleLogger.stop();
    fileLogger1.stop();
    fileLogger2.stop();
}

}