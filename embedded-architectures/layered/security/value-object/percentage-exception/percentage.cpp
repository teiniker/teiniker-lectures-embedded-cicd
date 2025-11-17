#include <cstdint>
#include <stdexcept>

#include "percentage.h"

using namespace std;

Percentage Percentage::make(int value) 
{
    // Input validation
    if (value < 0 || value > 100) 
    {
        throw invalid_argument("Invalid percentage value");
    }
    return Percentage(value);
}


