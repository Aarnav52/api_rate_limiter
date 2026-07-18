#include "Logger.hpp"
#include<iostream>
#include <ctime>
#include <chrono>
#include <iomanip>
void Logger::log(
        LogLevel level,
        const std::string& clientId,
        ApiStatus status,
        const std::string& message
    ){
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
            << clientId
            << " | "
            << static_cast<int>(status)
            << " | "
            << message
            << '\n';
    }
