#pragma once

#include <thread>
#include <vector>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>




class ThreadPool
{
private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasksQueue_;

    std::mutex mutex_; // to protect the queue and stop flag
    std::condition_variable cv_;
    bool stopFlag_;


public:
    ThreadPool(size_t threadCount);


    // not copyable or movable
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    // user must wrap his task in callable that "returns void and no arguments"
    // TODO: ask if there is a better way
    bool tryEnqueueTask(std::function<void()> task);

    ~ThreadPool();
};