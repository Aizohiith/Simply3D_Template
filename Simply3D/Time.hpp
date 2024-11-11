#pragma once
#include <iostream>
#include <chrono>

#define COMPILER 11

namespace Time
{
    inline uint64_t GetCurrentTime()
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        return (std::chrono::time_point_cast<std::chrono::milliseconds>(now).time_since_epoch().count());
    }

    inline int GetCurrentSec()
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm localTime;
        #if COMPILER == 11
            localtime_s(&localTime, &currentTime);
        #endif // COMPILER == 11
        #if COMPILER == 23
                    localtime_r(&currentTime, &localTime);
        #endif // COMPILER == 23

        
        return localTime.tm_sec;
    }

    inline int GetCurrentMin()
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm localTime;
        #if COMPILER == 11
                localtime_s(&localTime, &currentTime);
        #endif // COMPILER == 11
        #if COMPILER == 23
                localtime_r(&currentTime, &localTime);
        #endif // COMPILER == 23
        return localTime.tm_min;
    }

    inline int GetCurrentHour()
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm localTime;
        #if COMPILER == 11
                localtime_s(&localTime, &currentTime);
        #endif // COMPILER == 11
        #if COMPILER == 23
                localtime_r(&currentTime, &localTime);
        #endif // COMPILER == 23
        return localTime.tm_hour;
    }

    inline int GetCurrentMill()
    {
        return static_cast<int>(GetCurrentTime() % 1000);
    }

    inline int GetTimeDiffrenceSince(uint64_t LastTime)
    {
        return static_cast<int>(GetCurrentTime() - LastTime);
    }
};
