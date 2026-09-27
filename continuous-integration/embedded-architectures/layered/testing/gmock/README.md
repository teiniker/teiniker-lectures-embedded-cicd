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


## Creating Mock Classes

Mock classes are defined as normal classes, using the `MOCK_METHOD` macro 
to generate mocked methods.

The first 3 parameters are simply the method declaration, split into 3 parts. 
The 4th parameter accepts a closed list of qualifiers, which affect the generated method:

* `const`: Makes the mocked method a const method. Required if overriding a const method.
* `override`: Marks the method with override. Recommended if overriding a virtual method.
* `noexcept`: Marks the method with noexcept. Required if overriding a noexcept method.

_Example:_ Mock class for a given interface

```c++
// Interface
class UartService 
{
	public:                
		virtual ~UartService(void) {}

		virtual void init(const BaudRate baudRate, DataBits dataBits, Parity parity, StopBits stopBits) = 0;
		virtual void send(const std::string &data) = 0;
		virtual std::string receive(void) = 0;

		virtual uint32_t baudRate(void) const = 0;
		virtual uint8_t dataBits(void) const = 0;
		virtual uint8_t parity(void) const = 0;
		virtual uint8_t stopBits(void) const = 0;
 };

// Mock Class
class MockUartService : public UartService
{
public:
    MOCK_METHOD(void, init, (const BaudRate baudRate, DataBits dataBits, Parity parity, StopBits stopBits), (override));

    MOCK_METHOD(void, send, (const std::string &data), (override));
    MOCK_METHOD(std::string, receive, (), (override));

    MOCK_METHOD(uint32_t, baudRate, (), (const, override));
    MOCK_METHOD(uint8_t, dataBits, (), (const, override));
    MOCK_METHOD(uint8_t, parity, (), (const, override));
    MOCK_METHOD(uint8_t, stopBits, (), (const, override));
};
```

## Using Mocks in Tests

The typical work flow of using a mock class is:

* Import the gMock names from the **`testing` namespace** such that 
    we can use them unqualified
* **Create mock objects**.
* **Specify expectations** on them (How many times will 
    a method be called? With what arguments? What should it 
    do? etc.).
* **Exercise some code** that uses the mocks; optionally, check 
    the result using googletest assertions. 
    If a mock method is called more than expected or with 
    wrong arguments, you’ll get an error immediately.
* When a **mock object is destructed**, gMock will automatically 
    check whether all expectations on it have been satisfied.

Note that gMock requires **expectations to be set before the mock 
functions are called**, otherwise the behavior is undefined.

Do not alternate between calls to `EXPECT_CALL()` and calls to 
the mock functions, and do not set any expectations on a mock 
after passing the mock to an API.

_Example:_ 
```C++
// Import namespace
using namespace testing;

TEST(UartServiceTest, UartSendReceive_Mock)
{
    // Setup

    // Create mock object 
    MockUartService mock;
    string testData = "Hello World!";
    // Specify our expectations 
    EXPECT_CALL(mock, send(testData)).Times(1);
    EXPECT_CALL(mock, receive())
        .Times(1)
        .WillOnce(Return(testData));

    // Exercise
    mock.send(testData);
    string receivedData = mock.receive();

    // Verify
    EXPECT_EQ(testData, receivedData);
}
```

## Setting Expectations

The key to using a mock object successfully is to set the right expectations on it. 

In gMock we use the **EXPECT_CALL() macro** to set an expectation on a mock method. 

The general syntax is:

```C++
EXPECT_CALL(mock_object, method(matchers))
    .Times(cardinality)
    .WillOnce(action)
    .WillRepeatedly(action);
```

The macro has two arguments: first the **mock object**, and then 
the **method and its arguments**. 


### Matchers

When a mock function takes arguments, we may specify what arguments we are expecting:

```C++
EXPECT_CALL(mock, init(BaudRate::BR_115200, DataBits::EIGHT, Parity::NONE, StopBits::ONE));
```

If we are not interested in the value of an argument, we write `_` 
as the argument, which means *anything goes*.

`_` is an instance of what we call matchers. 
A **matcher** is like a predicate and can test whether an argument 
is what we would expect. 
We can use a matcher inside `EXPECT_CALL()` wherever a function argument 
is expected. `_` is a convenient way of saying *any value*.

If you don’t care about any arguments, rather than specify `_` for 
each of them you may instead **omit the parameter list**:

```C++
EXPECT_CALL(mock, init);
```

There are many **built-in matchers** for common types:

**Generic Comparison**:

* `Eq(value)` or `value`: argument == value
* `Ge(value)`: argument >= value
* `Gt(value)`: argument > value
* `Le(value)`: argument <= value
* `Lt(value)`: argument < value
* `Ne(value)`: argument != value
* `IsFalse()`: argument evaluates to false in a Boolean context.
* `IsTrue()`: argument evaluates to true in a Boolean context.
* `IsNull()`: argument is a NULL pointer (raw or smart).
* `NotNull()`: argument is a non-null pointer (raw or smart).

**Floating-Point Matchers**:

* `DoubleEq(a_double)`: argument is a double value approximately 
    equal to a_double, treating two NaNs as unequal.
* `FloatEq(a_float)`: argument is a float value approximately equal 
    to a_float, treating two NaNs as unequal.
* `NanSensitiveDoubleEq(a_double)`: argument is a double value 
    approximately equal to a_double, treating two NaNs as equal.
* `NanSensitiveFloatEq(a_float)`: argument is a float value 
    approximately equal to a_float, treating two NaNs as equal.
* `IsNan()`: argument is any floating-point type with a NaN value.

**String Matchers**:
* `ContainsRegex(string)`: argument matches the given regular expression.
* `EndsWith(suffix)`: argument ends with string suffix.
* `HasSubstr(string)`: argument contains string as a sub-string.
* `IsEmpty()`: argument is an empty string.
* `MatchesRegex(string)`: argument matches the given regular 
    expression with the match starting at the first character 
    and ending at the last character.
* `StartsWith(prefix)`: argument starts with string prefix.
* `StrCaseEq(string)`: argument is equal to string, ignoring case.
* `StrCaseNe(string)`: argument is not equal to string, ignoring case.
* `StrEq(string)`: argument is equal to string.
* `StrNe(string)`: argument is not equal to string.

And many more...


### Cardinalities

The first clause we can specify following an `EXPECT_CALL()` is `Times()`. 
We call its argument a cardinality as it tells **how many times the call 
should occur**. It allows us to repeat an expectation many times without 
actually writing it as many times.

An interesting special case is when we say `Times(0)`. Which means that 
the **function shouldn’t be called with the given arguments at all**, 
and gMock will report a googletest failure whenever the function is 
called.

If we omit `Times()`, gMock will infer the cardinality for us. 
The rules are easy to remember:
* If neither `WillOnce()` nor `WillRepeatedly()` is in the `EXPECT_CALL()`, 
    the inferred cardinality is `Times(1)`.
* If there are `n` `WillOnce()`’s but no `WillRepeatedly()`, where `n >= 1`, 
    the cardinality is `Times(n)`.
* If there are `n` `WillOnce()`’s and one `WillRepeatedly()`, where `n >= 0`, 
    the cardinality is `Times(AtLeast(n))`.


### Actions

We have to tell a mock object what to do when a method is invoked.

* If the **return type** of a mock function is a **built-in type or a pointer**, 
    the function has a **default action** (a void function will just return, 
    a bool function will return false, and other functions will return 0). 
    
    In addition, in C++ 11 and above, a mock function whose return type has 
    a default constructor, the default action is to return a default-constructed 
    value. 

* Second, we can specify the action to be taken each time the expectation 
    matches using a series of `WillOnce()` clauses followed by an optional 
    `WillRepeatedly()`.

    _Example:_ When `readPin(BUTTON_UP_PIN)` is called once, it will return `true`.

    ```C++
    EXPECT_CALL(*mock, readPin(BUTTON_UP_PIN))
        .WillOnce(Return(true));
    ```

    _Example:_ When `readPin(BUTTON_UP_PIN)` is called many times, first it 
        will return `true`, next it will return `false` and from then it will 
        always return `true`.

    ```C++
    EXPECT_CALL(*mock, readPin(BUTTON_UP_PIN))
        .WillOnce(Return(true))
        .WillOnce(Return(false))
        .WillRepeatedly(Return(true));
    ```


### Ordered Calls

By **default**, an expectation can match a call even though an earlier 
expectation hasn’t been satisfied. In other words, the **calls don’t 
have to occur in the order the expectations are specified**.

By creating an object of type `InSequence`, **all expectations in its 
scope are put into a sequence** and have to occur sequentially.

_Example:_ Verify expectations in a sequence

```C++
TEST(ControllerTest, ControllerControl_UpButton)
{
    // Setup
    auto mock = std::make_shared<MockGpio>();
    Controller controller(mock);

    {
        InSequence seq;

        EXPECT_CALL(*mock, readPin(BUTTON_UP_PIN)).WillOnce(Return(true));  
        EXPECT_CALL(*mock, writePin(MOTOR_IN1_PIN, true));                  
        EXPECT_CALL(*mock, writePin(MOTOR_IN2_PIN, false));
    }

    // Exercise
    controller.control();
    // Verify: Expectations are verified on destruction of mock
}
```

## References

* [gMock for Dummies](https://google.github.io/googletest/gmock_for_dummies.html)

* [gMock Cookbook](https://google.github.io/googletest/gmock_cook_book.html)

* [Matchers Reference](https://google.github.io/googletest/reference/matchers.html) 


_Egon Teiniker, 2025, GPL v3.0_