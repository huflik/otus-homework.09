#pragma once

#include <string>
#include <memory>
#include <mutex>
#include "processor.h"

class Parser {
public:
    Parser(size_t n, std::shared_ptr<Processor> processor);
    void Process(const std::string& line);
    void Eof();

private:
    std::mutex mutex_;
    size_t n_;
    size_t count_{0};
    int depth_{0};
    std::shared_ptr<Processor> processor_;
};