#ifndef STATISTICS_TIME_TRACKER_H
#define STATISTICS_TIME_TRACKER_H

#include <functional>
#include <string>

typedef std::function<void(std::string &)> StatisticsOutputCallback;

namespace apm
{
    namespace statistics_time_tracker
    {

        void setStatisticsOutputCallback(StatisticsOutputCallback callback);

        void time_track_start(const std::string &key);

        void time_track_start(const std::string &key, const std::string &tail);

        void time_track_end(const std::string &key);

        void time_track_end(const std::string &key, const std::string &tail);

        void printStatistics();

    } // namespace statistics_time_tracker
} // namespace apm

#endif