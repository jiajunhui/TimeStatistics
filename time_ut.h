//
// Created by 小贾 on 2023/6/17,0017.
//

#include <chrono>

#ifndef APM_TIME_UT_H
#define APM_TIME_UT_H

using statistics_time_point = std::chrono::high_resolution_clock::time_point;

namespace apm
{
    namespace time_ut
    {

        unsigned long currentTimeMs();

        std::chrono::high_resolution_clock::time_point clockNow();

    }
}

#endif // APM_TIME_UT_H
