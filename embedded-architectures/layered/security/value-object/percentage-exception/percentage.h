#include <optional>

class Percentage 
{
public:
    // Constructor
    Percentage(const int value);

    // Accessor
    int value() const { return _value;  }   
    
    // Copy constructor and assignment operator
    Percentage(const Percentage&) = default;

    // Assignment operator
    Percentage& operator=(const Percentage&) = default;

    // Comparison: gives us ==, !=, <, <=, >, >= for free, all by comparing the object’s members.
    auto operator<=>(const Percentage&) const = default;

private:
    int _value;
};
