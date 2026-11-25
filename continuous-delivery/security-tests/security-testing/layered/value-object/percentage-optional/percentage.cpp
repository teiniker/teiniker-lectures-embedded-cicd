#include <optional>
#include <cstdint>

#include "percentage.h"

std::optional<Percentage> Percentage::make(int value) 
{
    // Input validation
    if (value < 0 || value > 100) 
    {
        return std::nullopt;    // invalid input
    }
    
    return Percentage(value);   // valid
}


