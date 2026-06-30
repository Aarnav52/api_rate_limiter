#include<iostream>
#include<queue>
#include<unordered_map>
#include<string>
#include<mutex>
#include<chrono>

using TimePoint = std::chrono::steady_clock::time_point;

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

// Api response 
struct ApiResponse{
    bool allowed;
    int statuscode;
    std::string message;
};

class SlidingWindowRateLimiter{

private:
    std::unordered_map<std::string,Client> clients;
    std::mutex mtx;

public:

    ApiResponse registerClient(const std::string &clientid,long long limit,long long window){
        std::lock_guard<std::mutex> lock(mtx);

        if(!clients.count(clientid)){
            clients[clientid].config.limit = limit;
            clients[clientid].config.window = window;
            return {true, 200, "Client Registered Successfully"};
        }

        return {false, 409, "Client already exists"};
    }

    bool validateClient(const std::string &clientid){
        return clients.count(clientid);
    }

    void cleanupExpiredRequests(Client &client, TimePoint now){
        //window is in integer format to clean up the queue we need to convert window into duration 
        auto windowDuration = std::chrono::seconds(client.config.window);

        while(!client.state.requests.empty() &&
              client.state.requests.front() <= now - windowDuration){
            client.state.requests.pop();
        }
    }

    bool isAllowed(Client &client){
        return client.state.requests.size() < client.config.limit;
    }

    void recordRequest(Client &client, TimePoint now){
        client.state.requests.push(now);
    }

    ApiResponse allowRequest(const std::string &clientid){

        std::lock_guard<std::mutex> lock(mtx);

        if(!validateClient(clientid)){
            return {false, 404, "Client not Registered"};
        }

        TimePoint now = std::chrono::steady_clock::now();

        Client &client = clients[clientid];

        cleanupExpiredRequests(client, now);

        if(isAllowed(client)){
            recordRequest(client, now);
            return {true, 200, "Request Accepted"};
        }

        return {false, 429, "Too many requests"};
    }
};
int main(){

}