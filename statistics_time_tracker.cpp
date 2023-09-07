#include "statistics_time_tracker.h"
#include "time_statistics.h"
#include "safe_queue.h"
#include "time_ut.h"
#include <thread>

constexpr auto POINT_TYPE_PRINT = -1;
constexpr auto POINT_TYPE_START = 0;
constexpr auto POINT_TYPE_START_TAIL = 1;
constexpr auto POINT_TYPE_END = 2;
constexpr auto POINT_TYPE_END_TAIL = 3;

using namespace ::apm::time_statistics;
using namespace ::apm::time_ut;

namespace apm
{
    namespace statistics_time_tracker
    {

        struct Task
        {
            int point_type;
            statistics_time_point point_time;
            std::string key;
            std::string tail;

            Task(int point_type, statistics_time_point point_time)
                : point_type(point_type), point_time(point_time), key(""), tail("") {}
            Task(int point_type, statistics_time_point point_time, std::string key)
                : point_type(point_type), point_time(point_time), key(key), tail("") {}
            Task(int point_type, statistics_time_point point_time, std::string key, std::string tail)
                : point_type(point_type), point_time(point_time), key(key), tail(tail) {}
        };

        TimeStatisticsPtr timeStatisticsPtr = TimeStatistics::get();
        SafeQueue<Task> task_queue;
        std::mutex mtx;
        std::condition_variable ctx;
        StatisticsOutputCallback _OutputCallback;

        void workThread(void *args);
        void internalOutputCallback(std::string log);

        static std::thread worker_thread = std::thread(&workThread, nullptr);

        void workThread(void *args)
        {
            while (true)
            {
                std::unique_lock<std::mutex> lock(mtx);
                ctx.wait(lock, []
                         { return !task_queue.IsEmpty(); });

                Task task = task_queue.Front();
                task_queue.pop();
                lock.unlock();

                int ret = 0;
                switch (task.point_type)
                {
                case POINT_TYPE_START:
                    ret = timeStatisticsPtr->p_start(task.key, task.point_time);
                    break;
                case POINT_TYPE_START_TAIL:
                    ret = timeStatisticsPtr->p_start(task.key, task.tail, task.point_time);
                    break;
                case POINT_TYPE_END:
                    ret = timeStatisticsPtr->p_end(task.key, task.point_time);
                    break;
                case POINT_TYPE_END_TAIL:
                    ret = timeStatisticsPtr->p_end(task.key, task.tail, task.point_time);
                    break;
                case POINT_TYPE_PRINT:
                    timeStatisticsPtr->printStatistics();
                    break;
                }
                if (ret == -1)
                {
                    internalOutputCallback("time_tracker:WARNING: maybe you repeat call same tracker method, you must first call start() then end().");
                }
            }
        }

        void internalOutputCallback(std::string log)
        {
            if (_OutputCallback)
            {
                _OutputCallback(log);
            }
        }

        void setStatisticsOutputCallback(StatisticsOutputCallback callback)
        {
            _OutputCallback = std::move(callback);
            timeStatisticsPtr->_setStatisticsOutputCallback(internalOutputCallback);
        }

        void time_track_start(const std::string &key)
        {
            statistics_time_point time_point = clockNow();
            std::unique_lock<std::mutex> lock(mtx);
            task_queue.Push(Task(POINT_TYPE_START, time_point, key));
            lock.unlock();
            ctx.notify_one();
        }

        void time_track_start(const std::string &key, const std::string &tail)
        {
            statistics_time_point time_point = clockNow();
            std::unique_lock<std::mutex> lock(mtx);
            task_queue.Push(Task(POINT_TYPE_START_TAIL, time_point, key, tail));
            lock.unlock();
            ctx.notify_one();
        }

        void time_track_end(const std::string &key)
        {
            statistics_time_point time_point = clockNow();
            std::unique_lock<std::mutex> lock(mtx);
            task_queue.Push(Task(POINT_TYPE_END, time_point, key));
            lock.unlock();
            ctx.notify_one();
        }

        void time_track_end(const std::string &key, const std::string &tail)
        {
            statistics_time_point time_point = clockNow();
            std::unique_lock<std::mutex> lock(mtx);
            task_queue.Push(Task(POINT_TYPE_END_TAIL, time_point, key, tail));
            lock.unlock();
            ctx.notify_one();
        }

        void printStatistics()
        {
            std::unique_lock<std::mutex> lock(mtx);
            task_queue.Push(Task(POINT_TYPE_PRINT, clockNow()));
            lock.unlock();
            ctx.notify_one();
        }

    } // namespace statistics_time_tracker
} // namespace apm
