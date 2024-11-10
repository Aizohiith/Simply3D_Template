#pragma once
#include <iostream>
#include <chrono>

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
        localtime_s(&localTime, &currentTime);
        return localTime.tm_sec;
    }

    inline int GetCurrentMin()
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm localTime;
        localtime_s(&localTime, &currentTime);
        return localTime.tm_min;
    }

    inline int GetCurrentHour()
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm localTime;
        localtime_s(&localTime, &currentTime);
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
