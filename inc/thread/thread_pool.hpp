#pragma once

#include <thread>
#include <vector>

namespace chisel::thread {
class ThreadPool {
public:
    ThreadPool ( int threadCount );
    ~ThreadPool();

    ThreadPool             (const ThreadPool&) = delete;
    ThreadPool& operator = (const ThreadPool&) = delete;

    template<typename F, typename A>
    void push(F&&, A&&);
private:
    std::vector<std::thread> _threads;
};
}