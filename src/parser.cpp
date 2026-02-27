#include "parser.h"

Parser::Parser(size_t n, std::shared_ptr<Processor> p) : n_(n), processor_(std::move(p)) {}

void Parser::Process(const std::string& line) {
    
    std::lock_guard<std::mutex> lock(mutex_);

    if (line == "{") {
        if (depth_ == 0) {
            processor_->Finish();
        }

        ++depth_;
        return;
    }

    if (line == "}") {
        if(depth_ == 0) {
            return;
        }
        if (--depth_ == 0) {
            processor_->Finish();
        }
        return;
    }
    processor_->Add(line);

    if (depth_ == 0) {
        ++count_;

        if (count_ == n_) {
            processor_->Finish();
            count_ = 0;
        }
    }
}

void Parser::Eof() {

    std::lock_guard<std::mutex> lock(mutex_);

    if (depth_ == 0) {
        processor_->Finish();
    }
}