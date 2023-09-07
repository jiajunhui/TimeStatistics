//
// Created by 小贾 on 2023/6/17,0017.
//

#ifndef APM_TIME_STATISTICS_H
#define APM_TIME_STATISTICS_H

#include <iostream>
#include "safe_map.h"
#include "time_ut.h"
#include <string>
#include <vector>
#include <functional>

typedef std::function<void(std::string)> _StatisticsOutputCallback;

namespace apm
{
    namespace time_statistics
    {

        class TimeStatistics;
        using TimeStatisticsPtr = std::shared_ptr<TimeStatistics>;

        struct VEntry
        {
            statistics_time_point t_start;
            statistics_time_point t_end;
        };

        struct TailIndicator
        {
            bool start;
            statistics_time_point t_start;
        };

        template <typename V>
        class TimeVector : public std::vector<V>
        {
        public:
            bool start;
            statistics_time_point t_start;
        };

        class TimeStatistics
        {
        private:
            SafeMap<std::string, TimeVector<VEntry>> _time_map;
            SafeMap<std::string, TailIndicator> _tail_indicator_map;
            _StatisticsOutputCallback _output_callback;

        public:
            static TimeStatisticsPtr get();

            void _setStatisticsOutputCallback(_StatisticsOutputCallback callback)
            {
                _output_callback = std::move(callback);
            }

            int p_start(const std::string &key, statistics_time_point start);

            int p_start(const std::string &key, const std::string &tail, statistics_time_point start);

            int p_end(const std::string &key, statistics_time_point end);

            int p_end(const std::string &key, const std::string &tail, statistics_time_point end);

            int printStatistics();
        };

    } // namespace time_statistics
} // namespace apm

#endif // APM_TIME_STATISTICS_H
