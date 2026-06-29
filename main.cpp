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
class SlidingWindowRateLimiter{
    // a universal hash map where key is clientid or name and value is both structure clientconfig and clientstate
    private:
    std::unordered_map<std::string,Client> clients;

    public:
    
    bool registerClient(const std::string &clientid,long long limit,long long window){
        if(!clients.count(clientid)){
            clients[clientid].config.limit=limit;
            clients[clientid].config.window=window;
            return true;
        }
        else{
            return false;
        }
    }
    bool allowRequest(const std::string &clientid,long long currenttime){
        if(!clients.count(clientid)) return false;

        Client& client=clients[clientid];
        while(!client.state.requests.empty() && client.state.requests.front()<=currenttime-client.config.window){
            client.state.requests.pop();
        }
        if(client.state.requests.size()<client.config.limit){
            client.state.requests.push(currenttime);
            return true;
        }
        else{
            return false;
        }
    }
};
int main(){
    
}