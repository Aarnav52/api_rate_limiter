#pragma once

#include <unordered_map>
#include <string>
#include <mutex>

#include "Client.hpp"
#include "Logger.hpp"
#include "ApiResponse.hpp"

class SlidingWindowRateLimiter
{
private:

    std::unordered_map<std::string, Client> clients;

    std::mutex mtx;

    Logger logger;

public:

    ApiResponse  registerClient(const std::string &clientid,long long limit,long long window);

    bool validateClient(const std::string &clientid);

    void cleanupExpiredRequests(Client &client, TimePoint now);

    bool isAllowed(Client &client);

    void recordRequest(Client &client, TimePoint now);

    ApiResponse allowRequest(const std::string &clientid);
};