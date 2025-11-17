#include <optional>

class Percentage 
{
public:
    // Creation method
    static Percentage make(int value);

    int value() const { return value_;  }   
    
    // Copy constructor and assignment operator
    Percentage(const Percentage&) = default;

    // Assignment operator
    Percentage& operator=(const Percentage&) = default;

    // Comparison: gives us ==, !=, <, <=, >, >= for free, all by comparing the object’s members.
    auto operator<=>(const Percentage&) const = default;

private:
    // private constructor to enforce use of creation method
    explicit Percentage(int v) : value_(v) {}
    int value_;
};
