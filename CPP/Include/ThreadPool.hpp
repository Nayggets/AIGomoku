#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>

class ThreadPool
{
public:
    explicit ThreadPool(unsigned int nbThreads);
    ~ThreadPool();
        template<class F>
        auto submit(F&& function) -> std::future<std::invoke_result_t<F>>
        {
            using ReturnType = std::invoke_result_t<F>;

            auto task = std::make_shared<std::packaged_task<ReturnType()>>(
                std::forward<F>(function)
            );

            std::future<ReturnType> result = task->get_future();

            {
                std::lock_guard<std::mutex> lock(queueMutex);

                if (stop)
                    throw std::runtime_error("ThreadPool has been stopped");

                tasks.emplace([task]()
                {
                    (*task)();
                });
            }

            condition.notify_one();

            return result;
        }
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable condition;

    bool stop;


};