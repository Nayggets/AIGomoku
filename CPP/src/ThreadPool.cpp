#include "ThreadPool.hpp"

ThreadPool::ThreadPool(unsigned int nbThreads)
{
    this->stop = false;
    for (unsigned int i = 0; i < nbThreads; ++i)
    {
        workers.emplace_back([this]()
        {
            while (true)
            {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(queueMutex);

                    condition.wait(lock, [this]()
                    {
                        return stop || !tasks.empty();
                    });

                    if (stop && tasks.empty())
                        return;

                    task = std::move(tasks.front());
                    tasks.pop();
                }

                task();
            }
        });
    }
}


ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;
    }

    condition.notify_all();

    for (std::thread& worker : workers)
    {
        worker.join();
    }
}

