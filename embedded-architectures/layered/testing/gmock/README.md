# gMock Framework

Google Mock (gmock) is a C++ mocking framework that integrates tightly with 
Google Test (gtest) to support **test doubles**, **behavior verification**,
and expectations in unit tests. 

While gtest handles assertions and test organization, 
**gmock focuses on mocking objects and verifying interactions**.

When using gMock:

* first, we use some simple macros to describe the interface we want to mock, 
    and they will expand to the implementation of your mock class;
* next, we create some mock objects and specify its expectations and behavior 
    using an intuitive syntax;
* then we exercise code that uses the mock objects. gMock will catch any 
    violation to the expectations as soon as it arises.


## References

* [gMock for Dummies](https://google.github.io/googletest/gmock_for_dummies.html)

_Egon Teiniker, 2025, GPL v3.0_