# Regular Expressions 

> A regular expression (RegEx) is a sequence of characters that define a 
> search pattern. 
> It is used for matching, searching, and manipulating text strings based 
> on defined rules or patterns. 

Regular expressions are a powerful tool in programming, text processing, and data validation.

## Basic Structure of a Regular Expression

A regular expression consists of:

* **Literals**: Match exact characters.

    _Example:_ "cat" matches the word "cat".

* **Metacharacters**: Special characters used to define patterns.

    _Example:_ `.` matches any single character.

* **Quantifiers**: Specify how many times a pattern should repeat.

    _Example:_ `a*` matches zero or more occurrences of `a`.

* **Character Classes**: Define sets of characters to match.

    _Example:_ `[a-z]` matches any lowercase letter.

* **Anchors**: Specify position in the string.

    _Example:_ `^` matches the start of a string, `$` matches the end.


**Common Regular Expression Syntax**:

| **Syntax**       | **Description**                                                                 | **Example**                                 |
|-------------------|---------------------------------------------------------------------------------|---------------------------------------------|
| `.`               | Matches any single character except a newline.                                 | `c.t` matches "cat", "cut", "c-t".          |
| `^`               | Matches the beginning of a string.                                             | `^The` matches "The cat".                   |
| `$`               | Matches the end of a string.                                                   | `end$` matches "The end".                   |
| `*`               | Matches zero or more of the preceding character.                               | `a*` matches "", "a", "aaa".                |
| `+`               | Matches one or more of the preceding character.                                | `a+` matches "a", "aaa", but not "".        |
| `?`               | Matches zero or one of the preceding character.                                | `a?` matches "", "a".                       |
| `{n}`             | Matches exactly `n` occurrences.                                              | `a{3}` matches "aaa".                       |
| `{n,}`            | Matches `n` or more occurrences.                                               | `a{2,}` matches "aa", "aaa".                |
| `{n,m}`           | Matches between `n` and `m` occurrences.                                       | `a{1,3}` matches "a", "aa", "aaa".          |
| `[abc]`           | Matches any one of the characters inside the brackets.                         | `[abc]` matches "a", "b", "c".              |
| `[a-z]`           | Matches any character in the specified range.                                  | `[a-z]` matches any lowercase letter.        |
| `[^abc]`          | Matches any character not in the brackets.                                     | `[^abc]` matches "d", "e", etc.             |
| `\d`              | Matches any digit (equivalent to `[0-9]`).                                      | `\d` matches "3", "7".                      |
| `\D`              | Matches any non-digit character.                                               | `\D` matches "a", "z".                      |
| `\w`              | Matches any word character (letters, digits, underscores).                     | `\w` matches "A", "9", "_".                 |
| `\W`              | Matches any non-word character.                                                | `\W` matches "!", "@".                      |
| `\s`              | Matches any whitespace character.                                              | `\s` matches spaces, tabs, newlines.        |
| `\S`              | Matches any non-whitespace character.                                          | `\S` matches "a", "1".                      |
| `(pattern)`       | Captures the matching text for the pattern inside parentheses.                 | `(cat)` captures "cat".                     |
| `|`               | Acts as an OR operator between patterns.                                       | `cat|dog` matches "cat" or "dog".           |


_Example:_ RegEx: `^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$`

* `[a-zA-Z0-9._%+-]+`: Matches the username part.
* `@`: Matches the literal `@` symbol.
* `[a-zA-Z0-9.-]+`: Matches the domain part.
* `\.[a-zA-Z]{2,}$`: Matches the top-level domain (e.g., `.com`).



## Regular Expressions in C++

In C++, regular expressions are provided by the `<regex>` standard library 
header (since C++11).

We can use `std::regex`, `std::regex_match`, `std::regex_search`, and 
`std::regex_replace` to work with them.


## Matching


## Searching


## Replacing


## Techniques Used Together With Regular Expressions

### Normalization 

Normalization refers to **transforming input data into a standard, 
consistent format** before applying validations or processing. 

This step ensures that variations in input (e.g., different encodings, 
extra whitespace, or character representations) do not affect the 
matching or validation process.

In the context of RegEx, normalization is particularly important 
when dealing with **Unicode text**, where visually identical 
characters can have different binary representations.

_Example:_ Convert Unicode characters to a consistent representation


### Whitelisting

Whitelisting is the practice of specifying a set of allowed patterns or 
characters and rejecting anything that does not conform. This is a 
restrictive approach, where only explicitly permitted inputs are 
considered valid.

Whitelisting is used to enforce strict rules, allowing only well-defined 
inputs and excluding all others.

_Example:_ Whitelisting in Python
```Python
filename_regex = r"^[a-zA-Z0-9_]+\.(txt|jpg|png)$"
```

* Allows filenames like document.txt or image.png.

* Rejects filenames with special characters or unsupported extensions.


### Blacklisting

Blacklisting is the practice of specifying a set of patterns or 
characters that are explicitly disallowed. Inputs matching the 
blacklisted patterns are rejected, while everything else is permitted.

Blacklisting is useful when there is a specific set of known patterns 
that must be avoided, such as special characters or malicious strings.

_Example:_ Blacklisting Using RegEx
```Python
sql_injection_regex = r"(;|--|\b(OR|AND)\b)"

if re.search(sql_injection_regex, user_input, re.IGNORECASE):
    raise ValueError("Potential SQL injection detected")
```

* Protects against specific threats or unwanted patterns.

* Useful for adding additional safeguards on top of whitelisting.


## References

* [Regular expressions library](https://en.cppreference.com/w/cpp/regex.html)

* Jeffrey E.F. Friedl. **Mastering Regular Expressions**. O'Reilly, 3rd edition 2006

_Egon Teiniker, 2025, GPL v3.0_
