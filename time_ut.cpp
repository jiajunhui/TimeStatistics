//
// Created by 小贾 on 2023/6/17,0017.
//

#include "time_ut.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

namespace apm
{
    namespace time_ut
    {
        unsigned long currentTimeMs()
        {

#ifdef _WIN32
            return GetTickCount64();
#else
            struct timeval tv;
            gettimeofday(&tv, NULL);
            return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
        }

        std::chrono::high_resolution_clock::time_point clockNow()
        {
            return std::chrono::high_resolution_clock::now();
        }

    }
}