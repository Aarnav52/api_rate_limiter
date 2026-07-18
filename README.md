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

This is the basic structure of our flow 
here we use both hash map and then data base the reason is that data base are slow in performance than ram so we first authenticate the user if requests are valid then only we go for database 

HashMap → 0.0001 ms
Database → 5 ms

Request
    │
    ▼
Check HashMap
    │
    ├── API Key Found
    │      │
    │      ▼
    │ Check Request Count
    │      │
    │      ▼
    │ Allow / Reject
    │
    └── API Key Not Found
           │
           ▼
    Query PostgreSQL
           │
           ▼
    Valid API Key?
           │
     Yes          No
      │            │
      ▼            ▼
Add to HashMap   Return 401
      │
      ▼
Continue Request