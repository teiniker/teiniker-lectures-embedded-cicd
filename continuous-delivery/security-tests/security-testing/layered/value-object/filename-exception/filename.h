#pragma once

#include <string>

class Filename 
{
public:
    // Constructor with validation
    Filename(const std::string& filename);

    // Accessor
    const std::string& value() const { return _filename;  }   

    // Copy constructor and assignment operator
    Filename(const Filename&) = default;

    // Assignment operator
    Filename& operator=(const Filename&) = default;

    // Comparison: gives us ==, !=, <, <=, >, >= for free, all by comparing the object’s members.
    auto operator<=>(const Filename&) const = default;

private:
    // private constructor to enforce use of creation method
    std::string _filename;
};
