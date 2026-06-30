#include<iostream>
#include<queue>
#include<unordered_map>
#include<string>

// config of a particular client with their limit and window 
struct ClientConfig{
    long long limit;
    long long window;
};
//queue keeping the tract of the requests made by the client 
struct ClientState{
    std::queue<long long> requests;
};
// client structure keeping data of both clientconfig and clientstate
struct Client{
    ClientConfig config;
    ClientState state;
};
//Api response 
struct ApiResponse{
    bool allowed;
    int statuscode;
    std::string message;
};
class SlidingWindowRateLimiter{
    // a universal hash map where key is clientid or name and value is both structure clientconfig and clientstate
    private:
    std::unordered_map<std::string,Client> clients;

    public:
    
    ApiResponse registerClient(const std::string &clientid,long long limit,long long window){
        if(!clients.count(clientid)){
            clients[clientid].config.limit=limit;
            clients[clientid].config.window=window;
            return{
                true,
                200,
                "Client Registered Successfully"
            };
        }
        else{
            return{
                false,
                409,
                "Client already exist"
            };
        }
    }
    ////////////////////////////////////
   
   bool validateClient(const std::string &clientid) {
    return clients.count(clientid);
}
   void cleanupExpiredRequests(Client &client, long long currenttime) {
    while (!client.state.requests.empty() &&
           client.state.requests.front() <= currenttime - client.config.window) {
        client.state.requests.pop();
    }
}
     bool isallowed(Client &client){
        return client.state.requests.size()<client.config.limit;
    }
    void recordRequest(Client &client,long long currenttime){
        client.state.requests.push(currenttime);
    }
    ////////////////////////////////////
   ApiResponse allowRequest(const std::string &clientid, long long currenttime) {

    if (!validateClient(clientid)) {
        return {false, 404, "Client not Registered"};
    }

    Client &client = clients[clientid];

    cleanupExpiredRequests(client, currenttime);

    if (isallowed(client)) {
        recordRequest(client, currenttime);
        return {true, 200, "Request Accepted"};
    }

    return {false, 429, "Too many requests"};
}
};
int main(){
    SlidingWindowRateLimiter rl;
    rl.registerClient("Aarnav",3,10);
    for(int i=1;i<15;i++){
        ApiResponse res=rl.allowRequest("Aarnav",i);
        ApiResponse res1=rl.allowRequest("Aarnav",i);

        std::cout<<"Time " << i<<res.message;
        std::cout<<"Time " << i<<res1.message;
        std::cout<<"\n";
    }
}