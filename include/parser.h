#pragma once

#include "processor.h"
#include <string>
#include <memory>

class Parser {
public:
    Parser(size_t n, std::shared_ptr<Processor> processor);
    void Process(const std::string& line);
    void Eof();

private:
    size_t n_;
    size_t count_{0};
    int depth_{0};
    std::shared_ptr<Processor> processor_;
};