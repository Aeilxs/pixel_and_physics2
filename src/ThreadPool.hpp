#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

class ThreadPool {
  public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    template <class F> std::future<void> enqueue(F &&f);

  private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;
};

template <class F> std::future<void> ThreadPool::enqueue(F &&f) {
    auto task = std::make_shared<std::packaged_task<void()>>(std::forward<F>(f));
    std::future<void> result = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        if (stop) throw std::runtime_error("Enqueue on stopped ThreadPool");
        tasks.emplace([task]() { (*task)(); });
    }
    condition.notify_one();
    return result;
}

#endif // THREADPOOL_H
