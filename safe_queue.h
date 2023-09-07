//
// Created by 小贾 on 2023/6/17,0017.
//

#ifndef APM_SAFE_QUEUE_H
#define APM_SAFE_QUEUE_H

#include <queue>
#include <mutex>

namespace apm
{

    template <typename V>
    class SafeQueue
    {
    public:
        SafeQueue() {}

        ~SafeQueue() {}

        SafeQueue(const SafeQueue &rhs)
        {
            queue_ = rhs.queue_;
        }

        SafeQueue &operator=(const SafeQueue &rhs)
        {
            if (&rhs != this)
            {
                queue_ = rhs.queue_;
            }

            return *this;
        }

        int Size()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.size();
        }

        bool IsEmpty()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.empty();
        }

        void Push(const V &value)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(value);
        }

        V Front()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.front();
        }

        void pop()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.pop();
        }

        void Clear()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.clear();
            return;
        }

    private:
        std::mutex mutex_;
        std::queue<V> queue_;
    };

} // namespace apm

#endif // APM_SAFE_QUEUE_H
