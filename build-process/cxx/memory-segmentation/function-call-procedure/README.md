# Function Call Procedure (x86-64)

The function call procedure in an x86-64 Linux system follows the System V AMD64 
ABI calling convention. 

ABI stands for **Application Binary Interface**. It defines the low-level interface 
between two program modules, typically between a compiled application and the 
operating system, or between different parts of a program.

Here's a breakdown of the procedure:

1. **Function Call Setup**:
* Registers for Arguments: The first six integer or pointer arguments are passed in registers:
`rdi`, `rsi`, `rdx`, `rcx`, `r8`, and `r9`.
* Additional arguments are passed on the stack.
* Floating-point arguments are passed in `xmm0` to `xmm7`. 
   `xmm0` to `xmm7` are 128-bit SIMD registers available in x86-64 architecture.

2. **Stack Alignment**:
* The **stack pointer (rsp)** must be aligned to a 16-byte boundary before a 
call instruction. 
* If necessary, the caller adjusts the stack by subtracting space from rsp.

3. **Caller Responsibilities**:
* Save any caller-saved registers (rdi, rsi, rdx, rcx, r8, r9, rax, r10, r11) that it wants 
to preserve.
* Push arguments onto the stack if there are more than six arguments.
* Issue the `call` instruction, which pushes the return address onto the stack and 
jumps to the callee.

4. **Callee Responsibilities**:
* Save any callee-saved registers (`rbx`, `rbp`, `r12`, `r13`, `r14`, `r15`) that it modifies.
* Set up a new stack frame:
   - Push the old base pointer `rbp` onto the stack.
   - Copy the stack pointer `rsp` to rbp.
   - Allocate space on the stack for local variables by subtracting from `rsp`.

5. **Function Execution**:
* The callee uses the registers or stack to access arguments.
* Local variables are stored in the allocated stack space.

6. **Return Value**:
* The return value is placed in the `rax` register.
* For floating-point return values, `xmm0` is used.


7. **Function Return**:
* The callee restores the stack pointer `rsp` and base pointer `rbp`.
* The `ret` instruction pops the return address from the stack and jumps back 
to the caller.

8. **Caller Cleanup**:
* The caller cleans up the stack if necessary (e.g., for variadic functions or 
non-standard calling conventions).
* Restore any saved caller-saved registers.


## Assembly Code of C Functions

In the assembly code, the **function prolog** and **function epilog** sections 
are clearly visible. These sections are crucial for setting up and tearing down 
the stack frame during a function call.


### Function Prolog
The prolog is responsible for preparing the stack and saving the necessary 
registers. It typically includes:
1. **Saving the base pointer `rbp`**:
   - The current base pointer is pushed onto the stack to preserve the 
      caller's stack frame.
2. **Setting up the new stack frame**:
   - The stack pointer `rsp` is copied to the base pointer `rbp`.
3. **Allocating space for local variables**:
   - Space is reserved on the stack by subtracting from the stack pointer.

_Example from `add1` function:_
```assembly
0x00000000004004f1 <+0>:     push   rbp
0x00000000004004f2 <+1>:     mov    rbp,rsp
0x00000000004004f5 <+4>:     sub    rsp,0x18
```

### Function Epilog
The epilog is responsible for cleaning up the stack and restoring the 
caller's state. It typically includes:
1. **Restoring the base pointer `rbp`**:
   - The saved base pointer is popped from the stack to restore the 
      caller's stack frame.
2. **Returning to the caller**:
   - The `ret` instruction is used to return to the address stored 
   in the return address register.

_Example from `add1` function:_
```assembly
0x0000000000400514 <+35>:    leave
0x0000000000400515 <+36>:    ret
```

These sections ensure that the function operates in isolation without 
corrupting the caller's execution context.


```bash
$ gdb ./function_call
```

```assembly
(gdb) disass main
Dump of assembler code for function main:
   0x0000000000400516 <+0>:     push   rbp
   0x0000000000400517 <+1>:     mov    rbp,rsp
   0x000000000040051a <+4>:     sub    rsp,0x20
   0x000000000040051e <+8>:     mov    DWORD PTR [rbp-0x14],edi
   0x0000000000400521 <+11>:    mov    QWORD PTR [rbp-0x20],rsi

   0x0000000000400525 <+15>:    mov    esi,0x44444444
   0x000000000040052a <+20>:    mov    edi,0x22222222
   0x000000000040052f <+25>:    call   0x4004f1 <add1>
   0x0000000000400534 <+30>:    mov    DWORD PTR [rbp-0x4],eax
   0x0000000000400537 <+33>:    mov    eax,DWORD PTR [rbp-0x4]
   0x000000000040053a <+36>:    mov    esi,eax
   0x000000000040053c <+38>:    mov    edi,0x4005f0
   0x0000000000400541 <+43>:    mov    eax,0x0
   0x0000000000400546 <+48>:    call   0x4003f0 <printf@plt>
   0x000000000040054b <+53>:    mov    eax,0x0

   0x0000000000400550 <+58>:    leave
   0x0000000000400551 <+59>:    ret
```

```assembly
(gdb) disass add1
Dump of assembler code for function add1:
   0x00000000004004f1 <+0>:     push   rbp
   0x00000000004004f2 <+1>:     mov    rbp,rsp
   0x00000000004004f5 <+4>:     sub    rsp,0x18
   0x00000000004004f9 <+8>:     mov    DWORD PTR [rbp-0x14],edi
   0x00000000004004fc <+11>:    mov    DWORD PTR [rbp-0x18],esi

   0x00000000004004ff <+14>:    mov    edx,DWORD PTR [rbp-0x18]
   0x0000000000400502 <+17>:    mov    eax,DWORD PTR [rbp-0x14]
   0x0000000000400505 <+20>:    mov    esi,edx
   0x0000000000400507 <+22>:    mov    edi,eax
   0x0000000000400509 <+24>:    call   0x4004d7 <add2>
   0x000000000040050e <+29>:    mov    DWORD PTR [rbp-0x4],eax
   0x0000000000400511 <+32>:    mov    eax,DWORD PTR [rbp-0x4]

   0x0000000000400514 <+35>:    leave
   0x0000000000400515 <+36>:    ret
```

```assembly
(gdb) disass add2
Dump of assembler code for function add2:
   0x00000000004004d7 <+0>:     push   rbp
   0x00000000004004d8 <+1>:     mov    rbp,rsp
   0x00000000004004db <+4>:     mov    DWORD PTR [rbp-0x14],edi
   0x00000000004004de <+7>:     mov    DWORD PTR [rbp-0x18],esi

   0x00000000004004e1 <+10>:    mov    edx,DWORD PTR [rbp-0x14]
   0x00000000004004e4 <+13>:    mov    eax,DWORD PTR [rbp-0x18]
   0x00000000004004e7 <+16>:    add    eax,edx
   0x00000000004004e9 <+18>:    mov    DWORD PTR [rbp-0x4],eax
   0x00000000004004ec <+21>:    mov    eax,DWORD PTR [rbp-0x4]

   0x00000000004004ef <+24>:    pop    rbp
   0x00000000004004f0 <+25>:    ret
```



## Function Call Sequence in Assembly (Step-by-Step)

This example demonstrates the step-by-step execution of a function call using 
GDB to monitor the register and memory states. Additionally, the register and 
memory states can be used to understand how arguments are passed and return 
values are received.

```bash
(gdb) break main
Breakpoint 1 at 0x400525: file function_call.c, line 23.

(gdb) break add1
Breakpoint 2 at 0x4004ff: file function_call.c, line 14.

(gdb) break add2
Breakpoint 3 at 0x4004e1: file function_call.c, line 6.


(gdb) r
Breakpoint 1, main (argc=1, argv=0x7fffffffdb18) at function_call.c:23
23          result = add1(0x22222222, 0x44444444);

(gdb) stepi
(gdb) stepi

(gdb) disass
Dump of assembler code for function main:
   0x0000000000400516 <+0>:     push   rbp
   0x0000000000400517 <+1>:     mov    rbp,rsp
   0x000000000040051a <+4>:     sub    rsp,0x20

   0x000000000040051e <+8>:     mov    DWORD PTR [rbp-0x14],edi
   0x0000000000400521 <+11>:    mov    QWORD PTR [rbp-0x20],rsi
   0x0000000000400525 <+15>:    mov    esi,0x44444444
   0x000000000040052a <+20>:    mov    edi,0x22222222
=> 0x000000000040052f <+25>:    call   0x4004f1 <add1>
>  0x0000000000400534 <+30>:    mov    DWORD PTR [rbp-0x4],eax
   0x0000000000400537 <+33>:    mov    eax,DWORD PTR [rbp-0x4]
   0x000000000040053a <+36>:    mov    esi,eax
   0x000000000040053c <+38>:    mov    edi,0x4005f0
   0x0000000000400541 <+43>:    mov    eax,0x0
   0x0000000000400546 <+48>:    call   0x4003f0 <printf@plt>
   0x000000000040054b <+53>:    mov    eax,0x0

   0x0000000000400550 <+58>:    leave
   0x0000000000400551 <+59>:    ret

(gdb) info register
rax            0x400516 4195606
rbx            0x0      0
rcx            0x0      0
rdx            0x7fffffffdb28   140737488345896
rsi            0x44444444       1145324612          <=
rdi            0x22222222       572662306           <=
rbp            0x7fffffffda30   0x7fffffffda30
rsp            0x7fffffffda10   0x7fffffffda10

(gdb) s

Breakpoint 2, add1 (a=572662306, b=1145324612) at function_call.c:14
14          s = add2(a,b);

(gdb) stepi
(gdb) stepi
(gdb) stepi
(gdb) stepi

(gdb) disass
Dump of assembler code for function add1:
   0x00000000004004f1 <+0>:     push   rbp
   0x00000000004004f2 <+1>:     mov    rbp,rsp
   0x00000000004004f5 <+4>:     sub    rsp,0x18

   0x00000000004004f9 <+8>:     mov    DWORD PTR [rbp-0x14],edi
   0x00000000004004fc <+11>:    mov    DWORD PTR [rbp-0x18],esi
   0x00000000004004ff <+14>:    mov    edx,DWORD PTR [rbp-0x18]
   0x0000000000400502 <+17>:    mov    eax,DWORD PTR [rbp-0x14]
   0x0000000000400505 <+20>:    mov    esi,edx
   0x0000000000400507 <+22>:    mov    edi,eax
=> 0x0000000000400509 <+24>:    call   0x4004d7 <add2>
>  0x000000000040050e <+29>:    mov    DWORD PTR [rbp-0x4],eax
   0x0000000000400511 <+32>:    mov    eax,DWORD PTR [rbp-0x4]

   0x0000000000400514 <+35>:    leave
   0x0000000000400515 <+36>:    ret

(gdb) info register
rax            0x22222222       572662306
rdx            0x44444444       1145324612
rsi            0x44444444       1145324612
rdi            0x22222222       572662306
rbp            0x7fffffffda00   0x7fffffffda00
rsp            0x7fffffffd9e8   0x7fffffffd9e8
rip            0x400509 0x400509 <add1+24>

(gdb) x/16xg $rsp
0x7fffffffd9e8: 0x2222222244444444      0x0000000000000001
0x7fffffffd9f8: 0x00000000004005ad      0x00007fffffffda30
0x7fffffffda08: 0x0000000000400534      0x00007fffffffdb18
0x7fffffffda18: 0x0000000100400400      0x00007fffffffdb10
0x7fffffffda28: 0x0000000000000000      0x0000000000400560
0x7fffffffda38: 0x00007f7c450be50a      0x0000000000000001
0x7fffffffda48: 0x00007fffffffdb18      0x0000000100040000

(gdb) s

Breakpoint 3, add2 (a=572662306, b=1145324612) at function_call.c:6
6           s = a+b;

(gdb) disass
Dump of assembler code for function add2:
   0x00000000004004d7 <+0>:     push   rbp
   0x00000000004004d8 <+1>:     mov    rbp,rsp

   0x00000000004004db <+4>:     mov    DWORD PTR [rbp-0x14],edi
   0x00000000004004de <+7>:     mov    DWORD PTR [rbp-0x18],esi
   0x00000000004004e1 <+10>:    mov    edx,DWORD PTR [rbp-0x14]
   0x00000000004004e4 <+13>:    mov    eax,DWORD PTR [rbp-0x18]
=> 0x00000000004004e7 <+16>:    add    eax,edx
   0x00000000004004e9 <+18>:    mov    DWORD PTR [rbp-0x4],eax
   0x00000000004004ec <+21>:    mov    eax,DWORD PTR [rbp-0x4]

   0x00000000004004ef <+24>:    pop    rbp
   0x00000000004004f0 <+25>:    ret

(gdb) stepi
(gdb) stepi

(gdb) info register
rax            0x44444444       1145324612
rdx            0x22222222       572662306
rsi            0x44444444       1145324612
rdi            0x22222222       572662306
rbp            0x7fffffffd9d8   0x7fffffffd9d8
rsp            0x7fffffffd9d8   0x7fffffffd9d8
rip            0x4004e7 0x4004e7 <add2+16>

(gdb) x/16xg $rbp-0x18
0x7fffffffd9c0: 0x2222222244444444      0x00000000756e6547
0x7fffffffd9d0: 0x0000000000000009      0x00007fffffffda00
0x7fffffffd9e0: 0x000000000040050e      0x2222222244444444
0x7fffffffd9f0: 0x0000000000000001      0x00000000004005ad
0x7fffffffda00: 0x00007fffffffda30      0x0000000000400534
0x7fffffffda10: 0x00007fffffffdb18      0x0000000100400400
0x7fffffffda20: 0x00007fffffffdb10      0x0000000000000000

(gdb) stepi
(gdb) stepi
(gdb) stepi

gdb) disass
Dump of assembler code for function add2:
   0x00000000004004d7 <+0>:     push   rbp
   0x00000000004004d8 <+1>:     mov    rbp,rsp

   0x00000000004004db <+4>:     mov    DWORD PTR [rbp-0x14],edi
   0x00000000004004de <+7>:     mov    DWORD PTR [rbp-0x18],esi
   0x00000000004004e1 <+10>:    mov    edx,DWORD PTR [rbp-0x14]
   0x00000000004004e4 <+13>:    mov    eax,DWORD PTR [rbp-0x18]
   0x00000000004004e7 <+16>:    add    eax,edx
   0x00000000004004e9 <+18>:    mov    DWORD PTR [rbp-0x4],eax
   0x00000000004004ec <+21>:    mov    eax,DWORD PTR [rbp-0x4]

=> 0x00000000004004ef <+24>:    pop    rbp
   0x00000000004004f0 <+25>:    ret

(gdb)  x/16xg $rbp-0x18
0x7fffffffd9c0: 0x2222222244444444      0x00000000756e6547
0x7fffffffd9d0: 0x6666666600000009      0x00007fffffffda00
0x7fffffffd9e0: 0x000000000040050e      0x2222222244444444
0x7fffffffd9f0: 0x0000000000000001      0x00000000004005ad
0x7fffffffda00: 0x00007fffffffda30      0x0000000000400534
0x7fffffffda10: 0x00007fffffffdb18      0x0000000100400400
0x7fffffffda20: 0x00007fffffffdb10      0x0000000000000000
0x7fffffffda30: 0x0000000000400560      0x00007fd20ce4a50a

(gdb) s
add1 (a=572662306, b=1145324612) at function_call.c:15

(gdb) stepi

(gdb) disass
Dump of assembler code for function add1:
   0x00000000004004f1 <+0>:     push   rbp
   0x00000000004004f2 <+1>:     mov    rbp,rsp
   0x00000000004004f5 <+4>:     sub    rsp,0x18

   0x00000000004004f9 <+8>:     mov    DWORD PTR [rbp-0x14],edi
   0x00000000004004fc <+11>:    mov    DWORD PTR [rbp-0x18],esi
   0x00000000004004ff <+14>:    mov    edx,DWORD PTR [rbp-0x18]
   0x0000000000400502 <+17>:    mov    eax,DWORD PTR [rbp-0x14]
   0x0000000000400505 <+20>:    mov    esi,edx
   0x0000000000400507 <+22>:    mov    edi,eax
   0x0000000000400509 <+24>:    call   0x4004d7 <add2>
   0x000000000040050e <+29>:    mov    DWORD PTR [rbp-0x4],eax
   0x0000000000400511 <+32>:    mov    eax,DWORD PTR [rbp-0x4]

=> 0x0000000000400514 <+35>:    leave
   0x0000000000400515 <+36>:    ret

(gdb) info register
rax            0x66666666       1717986918
rdx            0x22222222       572662306
rsi            0x44444444       1145324612
rdi            0x22222222       572662306
rbp            0x7fffffffda00   0x7fffffffda00
rsp            0x7fffffffd9e8   0x7fffffffd9e8
rip            0x400514 0x400514 <add1+35>

(gdb) x/16xg $rsp
0x7fffffffd9e8: 0x2222222244444444      0x0000000000000001
0x7fffffffd9f8: 0x66666666004005ad      0x00007fffffffda30
0x7fffffffda08: 0x0000000000400534      0x00007fffffffdb18
0x7fffffffda18: 0x0000000100400400      0x00007fffffffdb10
0x7fffffffda28: 0x0000000000000000      0x0000000000400560
0x7fffffffda38: 0x00007fd20ce4a50a      0x0000000000000001
0x7fffffffda48: 0x00007fffffffdb18      0x0000000100040000
0x7fffffffda58: 0x0000000000400516      0x0000000000000000

(gdb) s
main (argc=1, argv=0x7fffffffdb18) at function_call.c:25

(gdb) stepi

(gdb) disass
Dump of assembler code for function main:
   0x0000000000400516 <+0>:     push   rbp
   0x0000000000400517 <+1>:     mov    rbp,rsp
   0x000000000040051a <+4>:     sub    rsp,0x20

   0x000000000040051e <+8>:     mov    DWORD PTR [rbp-0x14],edi
   0x0000000000400521 <+11>:    mov    QWORD PTR [rbp-0x20],rsi
   0x0000000000400525 <+15>:    mov    esi,0x44444444
   0x000000000040052a <+20>:    mov    edi,0x22222222
   0x000000000040052f <+25>:    call   0x4004f1 <add1>
   0x0000000000400534 <+30>:    mov    DWORD PTR [rbp-0x4],eax
   0x0000000000400537 <+33>:    mov    eax,DWORD PTR [rbp-0x4]
=> 0x000000000040053a <+36>:    mov    esi,eax
   0x000000000040053c <+38>:    mov    edi,0x4005f0
   0x0000000000400541 <+43>:    mov    eax,0x0
   0x0000000000400546 <+48>:    call   0x4003f0 <printf@plt>
   0x000000000040054b <+53>:    mov    eax,0x0

   0x0000000000400550 <+58>:    leave
   0x0000000000400551 <+59>:    ret

(gdb) info register
rax            0x66666666       1717986918
rdx            0x22222222       572662306
rsi            0x44444444       1145324612
rdi            0x22222222       572662306
rbp            0x7fffffffda30   0x7fffffffda30
rsp            0x7fffffffda10   0x7fffffffda10
rip            0x40053a 0x40053a <main+36>

(gdb) x/16xg $rsp
0x7fffffffda10: 0x00007fffffffdb18      0x0000000100400400
0x7fffffffda20: 0x00007fffffffdb10      0x6666666600000000
0x7fffffffda30: 0x0000000000400560      0x00007fd20ce4a50a
0x7fffffffda40: 0x0000000000000001      0x00007fffffffdb18
0x7fffffffda50: 0x0000000100040000      0x0000000000400516
0x7fffffffda60: 0x0000000000000000      0x3608c4628c556a7f
0x7fffffffda70: 0x0000000000400400      0x00007fffffffdb10
0x7fffffffda80: 0x0000000000000000      0x0000000000000000

(gdb) c
Continuing.
result = 0x66666666
```

*Egon Teiniker, 2025, GPL v3.0*