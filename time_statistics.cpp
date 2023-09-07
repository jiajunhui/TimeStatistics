//
// Created by 小贾 on 2023/6/17,0017.
//

#include <iomanip>
#include "time_statistics.h"
#include <cmath>

namespace apm
{
    namespace time_statistics
    {

        TimeStatisticsPtr apm::time_statistics::TimeStatistics::get()
        {
            static TimeStatisticsPtr timeStatisticsPtr = TimeStatisticsPtr(new TimeStatistics());
            return timeStatisticsPtr;
        }

        static unsigned long getMicroSeconds(statistics_time_point param_start, statistics_time_point param_end)
        {
            std::chrono::high_resolution_clock::time_point start = (std::chrono::high_resolution_clock::time_point)param_start;
            std::chrono::high_resolution_clock::time_point end = (std::chrono::high_resolution_clock::time_point)param_end;
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            return duration.count();
        }

        int apm::time_statistics::TimeStatistics::p_start(const std::string &key, statistics_time_point start)
        {
            TimeVector<VEntry> timeVector;
            bool find = _time_map.Find(key, timeVector);
            if (find && timeVector.start)
            {
                // illegal call
                return -1;
            }
            else
            {
                timeVector.t_start = start;
                timeVector.start = true;
                if (find)
                {
                    const TimeVector<VEntry> newTimeVector = timeVector;
                    _time_map.FindOldAndSetNew(key, timeVector, newTimeVector);
                }
                else
                {
                    _time_map.Insert(key, timeVector);
                }
                return 0;
            }
        }

        int apm::time_statistics::TimeStatistics::p_start(const std::string &key, const std::string &tail, statistics_time_point start)
        {
            TailIndicator tailIndicator;
            bool find = _tail_indicator_map.Find(key + tail, tailIndicator);
            if (find && tailIndicator.start)
            {
                return -1;
            }
            else
            {
                tailIndicator.t_start = start;
                tailIndicator.start = true;
                if (find)
                {
                    const TailIndicator newTailIndicator = tailIndicator;
                    _tail_indicator_map.FindOldAndSetNew(key + tail, tailIndicator, newTailIndicator);
                }
                else
                {
                    _tail_indicator_map.Insert(key + tail, tailIndicator);
                }
            }
            return 0;
        }

        int apm::time_statistics::TimeStatistics::p_end(const std::string &key, statistics_time_point end)
        {
            TimeVector<VEntry> timeVector;
            if (_time_map.Find(key, timeVector) && timeVector.start)
            {
                VEntry vEntry;
                vEntry.t_start = timeVector.t_start;
                vEntry.t_end = end;
                timeVector.insert(timeVector.end(), vEntry);
                timeVector.start = false;
                const TimeVector<VEntry> newTimeVector = timeVector;
                _time_map.FindOldAndSetNew(key, timeVector, newTimeVector);
                char buf[100];
                unsigned long dTimeMicroSeconds = getMicroSeconds(vEntry.t_start, vEntry.t_end);
                snprintf(buf, sizeof(buf), "time_tracker:[%s]:t_us=%ld", key.c_str(), dTimeMicroSeconds);
                if (_output_callback)
                {
                    _output_callback(std::string(buf));
                }
                return 0;
            }
            else
            {
                // illegal call
                return -1;
            }
        }

        int apm::time_statistics::TimeStatistics::p_end(const std::string &key, const std::string &tail, statistics_time_point end)
        {
            TailIndicator tailIndicator;
            if (_tail_indicator_map.Find(key + tail, tailIndicator) && tailIndicator.start)
            {
                tailIndicator.start = false;
                TimeVector<VEntry> timeVector;
                VEntry vEntry;
                vEntry.t_start = tailIndicator.t_start;
                vEntry.t_end = end;
                if (_time_map.Find(key, timeVector))
                {
                    timeVector.insert(timeVector.end(), vEntry);
                    const TimeVector<VEntry> newTimeVector = timeVector;
                    _time_map.FindOldAndSetNew(key, timeVector, newTimeVector);
                }
                else
                {
                    timeVector.insert(timeVector.end(), vEntry);
                    _time_map.Insert(key, timeVector);
                }
                _tail_indicator_map.Erase(key + tail);
                char buf[100];
                unsigned long dTimeMicroSeconds = getMicroSeconds(vEntry.t_start, vEntry.t_end);
                snprintf(buf, sizeof(buf), "time_tracker:[%s:%s]:t_us=%ld", key.c_str(), tail.c_str(), dTimeMicroSeconds);
                if (_output_callback)
                {
                    _output_callback(std::string(buf));
                }
            }
            else
            {
                return -1;
            }
            return 0;
        }

        int apm::time_statistics::TimeStatistics::printStatistics()
        {
            auto it = _time_map.begin();
            while (it != _time_map.end())
            {
                std::string key = it->first;
                TimeVector<VEntry> timeVector = it->second;
                size_t size = timeVector.size();
                if (size > 0)
                {
                    unsigned long total = 0;
                    unsigned long min = getMicroSeconds(timeVector.at(0).t_start, timeVector.at(0).t_end);
                    unsigned long max = 0;
                    unsigned long dV;
                    for (int i = 0; i < size; i++)
                    {
                        dV = getMicroSeconds(timeVector.at(i).t_start, timeVector.at(i).t_end);
                        min = std::min(min, dV);
                        max = std::max(max, dV);
                        total += dV;
                    }
                    double avg = (double)total / (int)size;
                    char buf[100];
                    snprintf(buf, sizeof(buf), "time_tracker:summary:[%s]:avg_t_us=%.2f, count=%d, max=%ld, min=%ld", key.c_str(), avg, (int)size, max, min);
                    if (_output_callback)
                    {
                        _output_callback(std::string(buf));
                    }
                }
                it++;
            }
            return 0;
        }

    } // namespace time_statistics
} // namespace apm
