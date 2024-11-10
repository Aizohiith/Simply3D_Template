#include "Timer.h"
Timer::Timer()
{
    StartTime = 0;
    Interval = 0;
    Active = false;
    OnTimeout = nullptr;
}

void Timer::Start()
{
    if (OnTimeout != nullptr)
    {
        Active = true;
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        SetStartTime(std::chrono::time_point_cast<std::chrono::milliseconds>(now).time_since_epoch().count());
    }
}

void Timer::Stop()
{
    Active = false;
}

void Timer::SetStartTime(uint64_t pi_StartTime)
{
    StartTime = pi_StartTime;
}
uint64_t Timer::GetStartTime() const
{
    return StartTime;
}

void Timer::SetInterval(int pi_Interval)
{
    Interval = pi_Interval;
}
int Timer::GetInterval() const
{
    return Interval;
}

void Timer::CallTimeout(uint64_t pi_Now)
{
    if (!Active)
        return;
    if (OnTimeout == nullptr)
        return;
    if ((pi_Now - StartTime) < Interval)
        return;
  
    uint64_t li_StartTime = StartTime;
    SetStartTime(pi_Now);
    OnTimeout(pi_Now - li_StartTime);
}