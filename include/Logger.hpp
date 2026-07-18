// Logger.hpp

#pragma once

#include <string>
#include "Common.hpp"

class Logger
{
public:
    void log(
        LogLevel level,
        const std::string& clientId,
        ApiStatus status,
        const std::string& message
    );
};