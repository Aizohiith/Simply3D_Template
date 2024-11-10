#ifndef TIMER_HPP_INCLUDED
#define TIMER_HPP_INCLUDED

#include <functional>
#include <chrono>

class Timer
{
    public:
        Timer();
        typedef std::function<void(int)> TimeOutPointer;
        TimeOutPointer OnTimeout;
        

        void Start();
        void Stop();

        void SetStartTime(uint64_t pi_StartTime);
        uint64_t GetStartTime() const;

        void SetInterval(int pi_Interval);
        int GetInterval() const;

        void CallTimeout(uint64_t Now);

    private:
        uint64_t StartTime;
        int Interval;//ms
        bool Active;
};

#endif // TIMER_HPP_INCLUDED
