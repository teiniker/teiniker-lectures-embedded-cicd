#include <cstdint>
#include <stdexcept>
#include <regex>

#include "filename.h"

using namespace std;

Filename::Filename(const std::string& filename)
{
    // Input validation
    regex pattern(R"([a-zA-Z0-9_]{1,255}\.[a-zA-Z0-9]{0,3})");
    bool match = regex_match(filename, pattern);
    if (!match)
    {
        throw invalid_argument("Invalid filename format");
    }

    _filename = filename;
}

