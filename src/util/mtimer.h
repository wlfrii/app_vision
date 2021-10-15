/**
 * @file mtimer.h
 *
 * @author Longfei Wang (longfei.wang@sjtu.end.cn)
 *
 * @brief This file includes some useful timer interfaces for time counting.
 *
 * The interfaces including in this file are mainly based on Class steady_clock,
 * which included in std::chrono namespace and represents a monotonic clock.
 * The time points of this clock cannot decrease as physical time moves forward
 * and the time between ticks of this clock is constant. This clock is not
 * related to wall clock time (for example, it can be time since last reboot),
 * and is most suitable for measuring intervals.
 *
 * @version 2.1
 *
 * @date 2020-01-01
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef _MTIMER_H_LF_
#define _MTIMER_H_LF_
#include <typeinfo>
#include <cstdio>
#include <string>
#include <ctime>
#include <chrono>
/** Ignorethe waring in Windows. */
#ifdef _WIN64
#pragma warning( disable : 4996 ) // disable the error of localtime
#endif

namespace util { namespace mtimer
{

using std::chrono::steady_clock;
using std::chrono::system_clock;

/**
 * @brief The flags used for time count.
 */
enum TimeUnit
{
    MICROSECOND,
    MILLISECOND,
    SECOND
};


/**
 * @brief Return current time point
 */
inline steady_clock::time_point getCurrentTimePoint()
{
    return steady_clock::now();
}


/**
 * @brief Get the duration time from start_time_point, the default time unit
 * is milliseconds.
 *
 * @tparam Tp The type of time unit, default type is milliseconds.
 * @param start_time_point The time point retured by steady_clock::now().
 * @return long long The return type
 */
template<typename Tp = std::chrono::milliseconds>
long long getDurationSince(const steady_clock::time_point &start_time_point)
{
    steady_clock::time_point now = steady_clock::now();
    return std::chrono::duration_cast<Tp>(now - start_time_point).count();
}


/**
 * @brief Return time duration in milliseconds since epoch.
 */
template<typename Tp = std::chrono::microseconds>
Tp getDurationSinceEpoch()
{
    return std::chrono::duration_cast<Tp>(system_clock::now().time_since_epoch());
}


/**
 * @brief Get the duration since epoch time.
 *
 * @tparam Tp
 * @param start_time_point A time point evaluated by epoch time point.
 * @param unit The unit to evaluated the output time.
 * @return float
 */
template<typename Tp = std::chrono::microseconds>
float getDurationSince(const Tp &start_time_point, TimeUnit unit = MILLISECOND)
{
    auto current_time_point = std::chrono::duration_cast<Tp>(system_clock::now().time_since_epoch());

    float k[3] = { 0 };
    if (typeid(Tp) == typeid(std::chrono::microseconds))
    {
        k[0] = 1.f;
        k[1] = 1.f / 1000.f;
        k[2] = 1.f / 1000000.f;
    }
    else if (typeid(Tp) == typeid(std::chrono::milliseconds))
    {
        k[0] = 1000.f;
        k[1] = 1.f;
        k[2] = 1.f / 1000.f;
    }
    else if (typeid(Tp) == typeid(std::chrono::seconds))
    {
        k[0] = 1000000.f;
        k[1] = 1000.f;
        k[2] = 1.f;
    }
    switch (unit)
    {
    case TimeUnit::MICROSECOND:
        return (current_time_point - start_time_point).count() * k[0];
    case TimeUnit::MILLISECOND:
        return (current_time_point - start_time_point).count() * k[1];
    case TimeUnit::SECOND:
        return (current_time_point - start_time_point).count() * k[2];
    }
    return (float)(current_time_point - start_time_point).count();
}


/**
 * @brief Return current time string.
 */
inline ::std::string getCurrentTimeStr()
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y%m%d_%H%M%S", localtime(&timep));

    return std::string(tmp);
}


}} // namespace::util::timer


/**
 * @brief Return current time point.
 * Equal to mtimer::getCurrentTimePoint().
 */
#define MTIMER__GET_CURRENT_TIME_POINT(time_point)                          \
    auto time_point = util::mtimer::getCurrentTimePoint()

/**
 * @brief Calcualte the duration, and print the time count and the related info.
 */
#define MTIMER__PRINT_DURATION_SINCE(time_point, fmt, ...)                  \
    do {                                                                    \
        long long time = util::mtimer::getDurationSince(time_point);        \
        printf("[%ld ms] is consumed to do: " fmt, time, ##__VA_ARGS__);    \
    } while(0)

/**
 * @brief Time counting for a specified void() function.
 */
#define MTIMER__COUNT_TIME_IN_VOID(func, fmt, ...)                          \
    do {                                                                    \
            MTIMER__GET_CURRENT_TIME_POINT(start);                          \
            func;                                                           \
            MTIMER__PRINT_DURATION_SINCE(start, fmt, ...)                   \
     } while (0)

/**
 * @brief Time counting for a specified non-void() function.
 */
#define MTIMER__COUNT_TIME_IN(func, ret, fmt, ...)                          \
    do {                                                                    \
            MTIMER__GET_CURRENT_TIME_POINT(start);                          \
            auto ret = func;                                                \
            GET_CURRENT_TIME_POINT(end);                                    \
            MTIMER__PRINT_DURATION_SINCE(start, fmt, ...)                   \
     } while (0)




#endif // _MTIMER_H_LF_
