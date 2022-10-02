#include "thread/thread_pool.h"

using chisel::thread::ThreadPool;

ThreadPool::ThreadPool( int threadCount ) {

}

ThreadPool::~ThreadPool() {

}

template<typename F, typename A>
void ThreadPool::push(F&& pr, A&& arg) {

}