#pragma once

#include <thread>
#include <vector>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <queue>

namespace chisel::thread {
class ThreadPool {
public:
    ThreadPool ( const int );

    void start();
    void stop ();
    void queue( const std::function<void()>& );
private:
    std::vector<std::thread>          _threads;
    std::queue<std::function<void()>> _jobs;
    std::mutex                        _qMutex;
    std::atomic<bool>                 _active;
    std::condition_variable           _mutCondition;

    void search_job();
};
}