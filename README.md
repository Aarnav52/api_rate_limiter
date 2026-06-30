# API Rate Limiter (Sliding Window)

A production-oriented Sliding Window Rate Limiter implemented in C++.

## Features

- Sliding Window algorithm
- Thread-safe using std::mutex
- Real-time timestamps using std::chrono
- Per-client configurable rate limits
- Structured API responses
- Modular and production-style code

## Tech Stack

- C++17
- STL
- std::mutex
- std::chrono

## Current Progress

-  Client registration
-  Sliding Window Rate Limiter
-  Thread safety
-  Chrono-based timestamps
- Structured logging (next)
-  HTTP server integration