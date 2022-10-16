#include "thread/thread_pool.h"

using chisel::thread::ThreadPool;

ThreadPool::ThreadPool( const int threadCount ) {
    _threads.resize(threadCount);
}

void ThreadPool::start() {
    for(uint32_t i = 0; i < _threads.size(); i++) {
        _threads.at(i) = std::thread(&ThreadPool::search_job, this);
    }
}

void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(_qMutex);
        _active = true;
    }

    _mutCondition.notify_all();

    for (auto& active_thread : _threads) {
        active_thread.join();
    }

    _threads.clear();
}

void ThreadPool::queue( const std::function<void()>& job ) {
    {
        std::unique_lock<std::mutex> lock(_qMutex);
        _jobs.push(job);
    }

    _mutCondition.notify_one();
}

void ThreadPool::search_job() {
    while(true) {
        std::function<void()> job;

        {
            std::unique_lock<std::mutex> lock(_qMutex);

            _mutCondition.wait(lock, [this] {
                return !_jobs.empty() || !_active;
            });

            if(!_active) return;

            job = _jobs.front();
            _jobs.pop();
        }

        job();
    }
}