#pragma once

#include <string>
#include <optional>

class Filename 
{
public:
    // Static factory method
    static std::optional<Filename> make(const std::string& filename);

    // Accessor
    const std::string& value() const { return _filename;  }   

    // Copy constructor and assignment operator
    Filename(const Filename&) = default;

    // Assignment operator
    Filename& operator=(const Filename&) = default;

    // Comparison: gives us ==, !=, <, <=, >, >= for free, all by comparing the object's members.
    auto operator<=>(const Filename&) const = default;

private:
    // Private constructor to enforce use of static make() method
    Filename(const std::string& filename);
    
    std::string _filename;
};
