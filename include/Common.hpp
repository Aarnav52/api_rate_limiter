#pragma once
#include<chrono>
using TimePoint = std::chrono::steady_clock::time_point;
enum class LogLevel{
    INFO,
    WARNING,
    ERROR
};
enum class ApiStatus{
    SUCCESS=200,
    CLIENT_NOT_FOUND=404,
    CLIENT_EXIST=409,
    RATE_LIMIT_EXCEEDED=429
};