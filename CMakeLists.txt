cmake_minimum_required(VERSION 3.20)

project(ApiRateLimiter)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    ApiRateLimiter
    src/main.cpp
    src/Logger.cpp
    src/RateLimiter.cpp
)

target_include_directories(
    ApiRateLimiter
    PRIVATE
    include
)