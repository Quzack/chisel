#pragma once

#include <thread>
#include <vector>

namespace chisel::thread {
class ThreadPool {
public:
    ThreadPool ( int );
    ~ThreadPool();

    template<typename F, typename A>
    void push(F&&, A&&);
private:
    std::vector<std::thread> _threads;
};
}