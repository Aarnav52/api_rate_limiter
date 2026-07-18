#include<iostream>
#include<queue>
#include<unordered_map>
#include<string>
#include<mutex>
#include<chrono>
#include <ctime>
#include <iomanip>

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
  
    ApiStatus status;
    std::string message;
};
class Logger{
public:

    void log(LogLevel level,
             const std::string &clientid,
             ApiStatus status,
             const std::string &message)
    {
        // Get current time
        auto now = std::chrono::system_clock::now();

        std::time_t currentTime =
            std::chrono::system_clock::to_time_t(now);

        std::tm* localTime =
            std::localtime(&currentTime);

        
        std::cout << std::put_time(localTime,"%Y-%m-%d %H:%M:%S");

        std::cout << " | ";

    
        if(level == LogLevel::INFO)
            std::cout << "INFO";
        else if(level == LogLevel::WARNING)
            std::cout << "WARNING";
        else
            std::cout << "ERROR";

        // Step 4: Print everything else
        std::cout
            << " | "
            << clientid
            << " | "
            << static_cast<int>(status)
            << " | "
            << message
            << '\n';
    }
};

class SlidingWindowRateLimiter{

private:
    std::unordered_map<std::string,Client> clients;
    std::mutex mtx;
    Logger logger;

public:

    ApiResponse registerClient(const std::string &clientid,long long limit,long long window){

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
};
int main()
{
    SlidingWindowRateLimiter limiter;

    limiter.registerClient("client1", 2 ,10);

    limiter.registerClient("client1", 2, 10);

    limiter.allowRequest("client1");

    limiter.allowRequest("client1");

    limiter.allowRequest("client1");

    limiter.allowRequest("client2");

    return 0;
}