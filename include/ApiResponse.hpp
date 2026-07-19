#pragma once

#include <string>
#include "Common.hpp"

struct ApiResponse
{
    ApiStatus status;
    std::string message;
};