# GDB Debugging Lab: Intel Syntax

## Objective
Learn how to inspect a compiled C program using GDB and read its assembly instructions in Intel syntax.

## Tools
- C compiler (GCC)
- GNU Debugger (GDB)
- Linux environment

## Key Concept
GDB can display machine-level instructions in different assembly syntaxes. Intel syntax is an alternative to AT&T syntax and uses a different operand notation.

## Command
```gdb
set disassembly-flavor intel
```

This command configures GDB to display disassembled instructions using Intel syntax.

## Planned Practice
- Compile a small C program with debugging symbols.
- Start GDB and inspect a function.
- Set a breakpoint and step through execution.
- Inspect registers and function arguments.

## Results
To be completed after running the commands and recording the actual output.

## Learning Notes
This report documents my learning process. Results will be added after practical verification.

## Practical Experiment: Debugging a C Function

### Environment
- Operating system: Kali Linux
- Debugger: GDB
- Language: C
- Function under test: `add_numbers(a, b)`

### Program Behavior
The function receives two integer arguments, adds them, stores the result in a local variable, and returns the result.

Test inputs:
- `a = 10`
- `b = 20`

Expected result: `30`

### Commands Used
```gdb
set disassembly-flavor intel
disassemble add_numbers
break add_numbers
run
info registers
print a
print b
print result
next
display/i $pc
stepi
```

### Observations
1. GDB stopped at `add_numbers` with `a = 10` and `b = 20`.
2. The `rdi` register contained `10`, and `rsi` contained `20` when the function breakpoint was reached, consistent with the x86-64 System V calling convention for the first two integer arguments.
3. The assembly instructions loaded the arguments into registers, performed the addition with `add eax, edx`, and stored the result in memory.
4. Before executing the addition statement, `result` was `0`; after `next`, it was `30`.
5. `display/i $pc` showed the current machine instruction. `stepi` advanced execution one machine instruction at a time.

### Assembly Analysis
```asm
mov edx, DWORD PTR [rbp-0x14]
mov eax, DWORD PTR [rbp-0x18]
add eax, edx
mov DWORD PTR [rbp-0x4], eax
```

These instructions load the function arguments from stack-frame memory into registers, add them, and store the result in the local variable's stack slot.

### Conclusion
This experiment demonstrated how to use GDB to inspect a C function, examine registers and variables, read Intel-syntax assembly, and follow execution at both source-line and machine-instruction level.

### Note
Instruction addresses may differ between disassembly output and runtime because executable memory can be relocated. The exact addresses depend on the build and runtime environment.

### Follow-up Experiment: Function Return Value and Register Tracking

**Objective:** Trace the return value from `add_numbers()` back to `main()` using GDB and Intel-syntax assembly.

**Test inputs**
- `a = 10`
- `b = 20`
- Expected return value: `30`

**Observed results**
1. `print result` returned `30` inside `add_numbers()`.
2. `info registers rax` showed `RAX = 0x1e` (decimal `30`).
3. The instruction `mov eax, DWORD PTR [rbp-0x4]` loaded the local result into `EAX`.
4. The `ret` instruction returned execution to `main()`.
5. The instruction `mov DWORD PTR [rbp-0xc], eax` stored the returned value in the local stack slot used by `main()`.
6. The instruction `mov esi, eax` prepared the value for use as an argument to `printf()`.

**Conclusion**

This experiment demonstrated how a C function returns an integer through `EAX/RAX`, how execution returns to the caller, and how GDB can be used to trace register values and assembly instructions across a function call.

**Technical note:** `0x1e` is the hexadecimal representation of decimal `30`. On x86-64, writing to `EAX` zero-extends the value into `RAX`.
