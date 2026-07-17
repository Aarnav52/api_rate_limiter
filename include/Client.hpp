#pragma once
#include<queue>
#include "Common.hpp"
// config of a particular client with their limit and window 
struct ClientConfig{
    long long limit;
    long long window; // interpreted as seconds
};

// queue keeping track of requests
struct ClientState{
    std::queue<TimePoint> requests;
};

// client structure
struct Client{
    ClientConfig config;
    ClientState state;
};