#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t threadCount) {

  if (threadCount == 0) {
    throw std::invalid_argument("Thread count must be greater than 0");
  }

  for (size_t i = 0; i < threadCount; i++) {

    // inside each worker thread an infinte loop that waits for a task and
    // executes it
    workers_.emplace_back([this] {
      while (true) {

        std::function<void()> task;
        // scope for the unique lock
        {
          std::unique_lock<std::mutex> lock(mutex_);
          cv_.wait(lock, [this] { return !tasksQueue_.empty() || stopFlag_; });

          // only exit the loop if stopFlag_ is true && the queue is empty
          if (stopFlag_ && tasksQueue_.empty())
            return;

          task = std::move(tasksQueue_.front());
          tasksQueue_.pop();
        }

        task();
      }
    });
  }
}

ThreadPool::~ThreadPool() {

  {
    std::lock_guard<std::mutex> lock(mutex_);
    stopFlag_ = true;
  }

  cv_.notify_all();

  for (size_t i = 0; i < workers_.size(); i++) {
    workers_[i].join();
  }
}

bool ThreadPool::tryEnqueueTask(std::function<void()> task) {

  if (!task) {
    // empty task
    return false;
  }

  std::unique_lock<std::mutex> lock(mutex_);

  // only enqueue a task if the thread pool is not stopped
  // can happen when the destructor is called in a sperate thread to the one
  // trying to enqueue
  if (stopFlag_) {
    return false;
  }

  tasksQueue_.emplace(std::move(task));
  lock.unlock();
  cv_.notify_one();

  return true;
}