#include <iostream>
#include <memory>
#ifdef __ANDROID__
#include <unistd.h>
#else
#include <windows.h>
#endif
#include "statistics_time_tracker.h"
#include <string>

using namespace apm::statistics_time_tracker;

#ifdef __ANDROID__
#define SLEEP_MS(ms) usleep(ms * 1000)
#else
#define SLEEP_MS(ms) Sleep(ms)
#endif

void callbackLog(std::string &log)
{
    std::cout << "callbackLog:" << log << std::endl;
}

int main()
{

    std::cout << "<=====start=====>" << std::endl;

    setStatisticsOutputCallback(callbackLog);

    time_track_start("p_key_1");

    SLEEP_MS(200);

    time_track_end("p_key_1");

    time_track_start("p_key_1");

    SLEEP_MS(318);

    time_track_end("p_key_1");

    time_track_start("p_key_1");

    SLEEP_MS(180);

    time_track_end("p_key_1");
    time_track_end("p_key_1");

    time_track_start("p_key_1", std::to_string(100001));

    SLEEP_MS(300);

    time_track_end("p_key_1", std::to_string(100001));

    time_track_start("p_key_1", std::to_string(100002));

    SLEEP_MS(411);

    time_track_end("p_key_1", std::to_string(100002));

    time_track_start("p_key_1", std::to_string(100003));

    SLEEP_MS(321);

    time_track_end("p_key_1", std::to_string(100003));

    //----------------------------------

    time_track_start("p_key_2");

    SLEEP_MS(490);

    time_track_end("p_key_2");

    time_track_start("p_key_2");

    SLEEP_MS(321);

    time_track_end("p_key_2");

    time_track_start("p_key_2");

    SLEEP_MS(180);

    time_track_end("p_key_2");

    time_track_start("p_key_2", std::to_string(200001));

    SLEEP_MS(243);

    time_track_end("p_key_2", std::to_string(200001));

    time_track_start("p_key_2", std::to_string(200002));

    SLEEP_MS(310);

    time_track_end("p_key_2", std::to_string(200002));

    time_track_start("p_key_2", std::to_string(200003));

    SLEEP_MS(290);

    time_track_end("p_key_2", std::to_string(200003));

    printStatistics();

    return 0;
}
