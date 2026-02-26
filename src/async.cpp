#include <mutex>
#include <list>
#include <memory>
#include <thread>

#include "async.h"
#include "parser.h"
#include "processor.h"
#include "consoleLogger.h"
#include "fileLogger.h"

namespace async {

// Общие очереди для всех контекстов
static BulkQueueShared_t consoleQueue = std::make_shared<BulkQueue_t>();
static BulkQueueShared_t fileQueue = std::make_shared<BulkQueue_t>();

// Логгеры в отдельных потоках
static ConsoleLogger consoleLogger(consoleQueue);
static FileLogger fileLogger1(fileQueue);
static FileLogger fileLogger2(fileQueue);

// Контексты подключений
struct Context {
    std::unique_ptr<Parser> parser;
    std::shared_ptr<Processor> processor;
};

static std::list<std::unique_ptr<Context>> contexts;
static std::mutex contextsMutex;

handle_t connect(std::size_t bulkSize) {
    // Запускаем логгеры при первом подключении
    static std::once_flag flag;
    std::call_once(flag, [](){
        consoleLogger.start();
        fileLogger1.start();
        fileLogger2.start();
    });
    
    std::lock_guard<std::mutex> lock(contextsMutex);
    
    auto context = std::make_unique<Context>();
    
    // Теперь процессор получает обе очереди
    context->processor = std::make_shared<Processor>(consoleQueue, fileQueue);
    context->parser = std::make_unique<Parser>(bulkSize, context->processor);
    
    contexts.push_back(std::move(context));
    return contexts.back().get();
}

void receive(handle_t handle, const char *data, std::size_t size) {
    std::lock_guard<std::mutex> lock(contextsMutex);
    
    for (auto& ctx : contexts) {
        if (ctx.get() == handle) {
            // Разбираем входные данные по символам
            std::string current;
            for (size_t i = 0; i < size; ++i) {
                if (data[i] == '\n' || data[i] == '\0') {
                    if (!current.empty()) {
                        ctx->parser->Process(current);
                        current.clear();
                    }
                } else {
                    current.push_back(data[i]);
                }
            }
            if (!current.empty()) {
                ctx->parser->Process(current);
            }
            break;
        }
    }
}

void disconnect(handle_t handle) {
    std::lock_guard<std::mutex> lock(contextsMutex);
    
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