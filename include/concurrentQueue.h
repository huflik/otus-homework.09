#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <iostream>
#include <atomic>

template<typename T>
struct ConcurrentQueue
{
    ConcurrentQueue() : headPtr_{new Node}, tailPtr_{headPtr_.get()}, stopped_{false} {}

    ConcurrentQueue(const ConcurrentQueue&) = delete;
    ConcurrentQueue(ConcurrentQueue &&) = delete;
    ConcurrentQueue& operator=(const ConcurrentQueue&) = delete;
    ConcurrentQueue& operator=(const ConcurrentQueue&&) = delete;

    [[nodiscard]] bool pop(T &value)
    {
        std::unique_lock<std::mutex> lock{mutex_consumer_};
        conditional_.wait(lock, [&](){
            if(stopped_) return true;
            return (headPtr_.get() != tail());
        });
        
        if(stopped_ && headPtr_.get() == tail())
        {
            return false;
        }

        value = std::move(headPtr_->value);
        std::unique_ptr<Node> prevHead = std::move(headPtr_);
        headPtr_ = std::move(prevHead->next);
        return true;
    }

    void push(const T& value)  
    {
        std::unique_ptr<Node> node{new Node};
        {
            std::lock_guard<std::mutex> lock{mutex_producer_};
            if(stopped_) return;
            tailPtr_->value = value;  
            tailPtr_->next = std::move(node);
            tailPtr_ = tailPtr_->next.get();
        }
        conditional_.notify_one();
    }

    void stop()
    {
        std::lock_guard<std::mutex> lock1{mutex_consumer_};
        std::lock_guard<std::mutex> lock2{mutex_producer_};
        stopped_ = true;
        conditional_.notify_all();
    }

private:
    struct Node
    {
        T value;
        std::unique_ptr<Node> next;
    };

    Node* tail()
    {
        std::lock_guard<std::mutex> lock{mutex_producer_};
        return tailPtr_;
    }

    std::mutex mutex_consumer_;
    std::mutex mutex_producer_;
    std::condition_variable conditional_;
    std::unique_ptr<Node> headPtr_;
    Node *tailPtr_;
    std::atomic<bool> stopped_;
};