#include <cstdint>
#include <regex>
#include <optional>

#include "filename.h"

using namespace std;

std::optional<Filename> Filename::make(const std::string& filename)
{
    // Input validation
    regex pattern(R"([a-zA-Z0-9_]{1,255}\.[a-zA-Z0-9]{0,3})");
    bool match = regex_match(filename, pattern);
    if (!match)
    {
        return std::nullopt;
    }

    return Filename(filename);
}

Filename::Filename(const std::string& filename) : _filename(filename)
{
}

