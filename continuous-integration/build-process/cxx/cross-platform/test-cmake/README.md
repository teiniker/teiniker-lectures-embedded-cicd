# Example: Cross-Compiler Using CMake and GTest 

Given a simple GTest

```c++
#include<gtest/gtest.h>
#include <vector>

using namespace std;

TEST(VectorTest, Length) 
{
    vector<int> numbers = {7, 5, 16, 8};
    size_t len = numbers.size();

    ASSERT_EQ(4, len);
}

TEST(VectorTest, IndexOperator) 
{
    vector<int> numbers = {7, 5, 16, 8};

    EXPECT_EQ(7, numbers[0]);
    EXPECT_EQ(5, numbers[1]);
    EXPECT_EQ(16, numbers[2]);
    EXPECT_EQ(8, numbers[3]);
}
//...
```

To build the test executable for a Raspberry Pi (ARM64), configure CMake to 
use the Raspberry Pi cross‑compiler and build in an isolated build directory. 
Ensure the cross‑compiler (aarch64-rpi5-linux-gnu-c++) and target libraries 
(GTest) are available on your host or provided via a toolchain/sysroot.

```bash
$ cmake -S . -B build -DCMAKE_CXX_COMPILER=aarch64-rpi5-linux-gnu-c++ 
$ cd build/
$ make
```

Finally, we copy the test executable to the target device.
```bash
$ scp build/test student@xxx.xxx.xxx.xxx:/home/student/Downloads
```


*Egon Teiniker, 2025, GPL v3.0*


