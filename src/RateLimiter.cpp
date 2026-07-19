#include "RateLimiter.hpp"
#include<chrono>
#include<queue>

ApiResponse SlidingWindowRateLimiter::registerClient(const std::string &clientid,long long limit,long long window){
      std::lock_guard<std::mutex> lock(mtx);
       

        if(!clients.count(clientid)){
            clients[clientid].config.limit = limit;
            clients[clientid].config.window = window;
            logger.log(LogLevel::INFO,clientid,ApiStatus::SUCCESS, "Client Registered Successfully");

            // this return dose not print anything that part is done by logger instead it return bool to the function register client
               return {
        
        ApiStatus::SUCCESS,
        "Client Registered Successfully"
    };
        }

        logger.log(
    LogLevel::WARNING,
    clientid,
    ApiStatus::CLIENT_EXIST,
    "Client already exists"
);

return {
   
    ApiStatus::CLIENT_EXIST,
    "Client already exists"
};

} 

bool SlidingWindowRateLimiter::validateClient(const std::string &clientid){
    return clients.count(clientid);
}

void SlidingWindowRateLimiter::cleanupExpiredRequests(Client &client, TimePoint now){
     //window is in integer format to clean up the queue we need to convert window into duration 
        auto windowDuration = std::chrono::seconds(client.config.window);

        while(!client.state.requests.empty() &&
              client.state.requests.front() <= now - windowDuration){
            client.state.requests.pop();
        }
}

bool SlidingWindowRateLimiter::isAllowed(Client &client){
        return client.state.requests.size() < client.config.limit;
    }

void SlidingWindowRateLimiter::recordRequest(Client &client, TimePoint now){
        client.state.requests.push(now);
    }

ApiResponse SlidingWindowRateLimiter::allowRequest(const std::string &clientid){

        std::lock_guard<std::mutex> lock(mtx);

        if(!validateClient(clientid)){
            logger.log(LogLevel::ERROR,clientid,ApiStatus::CLIENT_NOT_FOUND,"Client not registered");
            return{
                ApiStatus::CLIENT_NOT_FOUND,
                "Client not registered"
            };
        }

        TimePoint now = std::chrono::steady_clock::now();

        Client &client = clients[clientid];

        cleanupExpiredRequests(client, now);

        if(isAllowed(client)){
            recordRequest(client, now);
            logger.log(LogLevel::INFO,clientid,ApiStatus::SUCCESS,"Request Accepted");
            return{
                ApiStatus::SUCCESS,
                "Request Accepted"
            };
        }
        logger.log(LogLevel::WARNING,clientid,ApiStatus::RATE_LIMIT_EXCEEDED,"Rate Limit Exceeded");
        return { ApiStatus::RATE_LIMIT_EXCEEDED, "Rate limit Exceeded"};
    }

