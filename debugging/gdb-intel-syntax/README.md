# GDB Debugging Lab: Intel Syntax and x86-64 Analysis

## 1. Abstract

This laboratory documents the practical use of the GNU Debugger (GDB) to analyze a compiled C program at both source-code and machine-instruction levels. The experiment focuses on Intel assembly syntax, CPU register inspection, function execution flow, stack-frame analysis, memory examination, and hardware watchpoints.

The laboratory demonstrates how high-level C operations are translated into x86-64 assembly instructions and how GDB can be used to trace program execution and examine runtime data.

## 2. Objectives

The main objectives of this laboratory are to:

- Configure GDB to display Intel assembly syntax.
- Inspect compiled machine instructions using disassembly.
- Set breakpoints and control program execution.
- Examine CPU registers and function arguments.
- Trace function return values between function calls.
- Inspect stack frames and process memory.
- Understand little-endian byte representation.
- Use watchpoints to detect changes in memory.
- Examine saved return addresses and execution flow.

## 3. Laboratory Environment
_____________________________________________
| Component            | Description        |
|----------------------|--------------------|
| Operating System     | Kali Linux         |
| Programming Language | C                  |
| Compiler             | GCC                |
| Debugger             | GNU Debugger (GDB) |
| Architecture         | x86-64             |
| Assembly Syntax      | Intel              |
| Target Program       | `main`             |
| Function Under Test  | `add_numbers()`    |
|______________________|____________________|

## 4. Program Compilation and Execution

The experiment uses a C function named `add_numbers()` that receives two integer arguments, calculates their sum, stores the result in a local variable, and returns the value.

### Compilation

```bash
gcc -g -O0 -o main main.c
```

Compilation options:

- `-g`: Generates debugging information.
- `-O0`: Disables compiler optimization to make debugging and instruction analysis easier.
- `-o main`: Specifies the output executable name.

### Test Inputs
________________________
| Variable        |Value|
|-----------------------|
| `a`             | 10  |
| `b`             | 20  |
| Expected Result | 30  |
|_______________________|

## 5. Methodology and Implementation

### 5.1 Intel Assembly Configuration

```gdb
set disassembly-flavor intel
show disassembly-flavor
```

This configuration instructs GDB to display assembly instructions using Intel syntax instead of the default AT&T syntax.

### 5.2 Function Disassembly

```gdb
disassemble add_numbers
disassemble /r add_numbers
```

These commands display the assembly instructions generated for the target function. The `/r` option also displays the raw instruction bytes.

### 5.3 Breakpoint Configuration

```gdb
break add_numbers
run
```

A breakpoint was placed at the beginning of the target function to inspect its execution before proceeding.

### 5.4 Register and Argument Inspection

```gdb
info registers
info args
info locals
```

These commands were used to inspect CPU register values, function arguments, and local variables.

### 5.5 Instruction-Level Execution

```gdb
display/i $pc
stepi
```

`display/i $pc` continuously displays the instruction at the current program counter, while `stepi` executes one machine instruction at a time.

### 5.6 Memory Inspection

The following commands were used to inspect memory:

```gdb
print &result
x/wd ADDRESS
x/4wx ADDRESS
x/4wd ADDRESS
x/4xb ADDRESS
```

The memory examination formats provide different representations:

- `x/wd`: Displays one word as a signed decimal integer.
- `x/4wx`: Displays four words in hexadecimal.
- `x/4wd`: Displays four words in decimal.
- `x/4xb`: Displays four individual bytes in hexadecimal.

### 5.7 Watchpoint Monitoring

```gdb
watch result
continue
```

A watchpoint was used to detect changes to the `result` variable during program execution.

## 6. Experimental Results

### 6.1 Function Arguments and Register Analysis

When execution reached `add_numbers()`, the following values were observed:

- `a = 10`
- `b = 20`
- `RDI = 10`
- `RSI = 20`

This behavior is consistent with the x86-64 System V calling convention, where the first two integer or pointer arguments are passed in RDI and RSI.

### 6.2 Assembly Instruction Analysis

The following instructions were observed:

```asm
mov edx, DWORD PTR [rbp-0x14]
mov eax, DWORD PTR [rbp-0x18]
add eax, edx
mov DWORD PTR [rbp-0x4], eax
```

Technical interpretation:

1. The first instruction loads one function argument from stack-frame memory into EDX.
2. The second instruction loads the other argument into EAX.
3. The `add` instruction adds the values stored in EAX and EDX.
4. The final instruction stores the calculated result in the stack location assigned to the local variable.

The observed result was:

```text
10 + 20 = 30
```

### 6.3 Function Return Value Analysis

The return value was traced from `add_numbers()` back to `main()`.

Observed results:

| Observation | Value |
|---|---|
| Local result | 30 |
| RAX | `0x1e` |
| Decimal representation | 30 |
| Return instruction | `ret` |

The following instruction was observed:

```asm
mov eax, DWORD PTR [rbp-0x4]
```

This instruction loads the local result into EAX before the function returns.

On x86-64, integer return values are conventionally delivered through EAX/RAX. Writing to EAX also zero-extends the value into the upper half of RAX.

After the `ret` instruction, execution returned to `main()`, where the returned value was stored in a local stack location and prepared for use as an argument to `printf()`.

### 6.4 Stack Frame Analysis

The `info frame` command was used to inspect the current stack frame.

The following information was examined:

- Current frame information.
- Saved instruction pointer.
- Local variable addresses.
- Function execution context.

The observed address of the local variable `result` was:

```text
0x7fffffffdcdc
```

This address belongs to the recorded debugging session and is not guaranteed to remain the same in another execution.

### 6.5 Memory Representation and Little-Endian Analysis

The integer value 30 was observed in hexadecimal as:

```text
0x0000001e
```

Its byte representation on the tested little-endian system was:

```text
1e 00 00 00
```

In little-endian representation, the least significant byte is stored at the lowest memory address.

### 6.6 Watchpoint Results

A watchpoint was configured to monitor the local variable:

```gdb
watch result
```

The observed change was:

| State | Value |
|---|---:|
| Previous value | 0 |
| New value | 30 |

The watchpoint demonstrated how GDB can detect a change in a monitored expression during program execution.

### 6.7 Return Address Verification

The saved return address was examined using:

```gdb
x/gx $rbp+8
```

The resulting address was observed as:

```text
0x555555555178
```

It matched the `saved rip` reported by `info frame` in the same debugging session.

This confirmed the relationship between the saved return address and the return location of the function.

## 7. Technical Analysis

The experiment demonstrated the relationship between C source code, x86-64 registers, assembly instructions, and stack memory.

The main technical observations include:

- Function arguments were available through RDI and RSI according to the System V calling convention.
- Arithmetic operations were performed using general-purpose CPU registers.
- The local variable was stored in stack-frame memory.
- The function return value was delivered through EAX/RAX.
- The `ret` instruction transferred execution back to the caller.
- Memory inspection exposed the little-endian representation of integer values.
- Watchpoints provided a mechanism for monitoring changes to program data.

The experiment also demonstrated that GDB can provide visibility into runtime behavior that is not directly apparent from the C source code.

## 8. Security Relevance

Understanding registers, memory, stack frames, and function execution is important in several cybersecurity fields:

- **Reverse Engineering:** Understanding how compiled programs implement high-level operations.
- **Binary Analysis:** Examining executable instructions and runtime behavior.
- **Vulnerability Research:** Understanding stack memory, function calls, and memory-related program behavior.
- **Exploit Development Education:** Building the foundational knowledge required to understand stack-based vulnerabilities in controlled environments.
- **Malware Analysis:** Using debugging techniques to investigate program execution and internal behavior.

This laboratory is an educational foundation and does not demonstrate exploitation of a vulnerability.

## 9. Limitations

- The experiment used a small local C program.
- The analysis focused on x86-64 architecture and Intel assembly syntax.
- Compiler optimization was disabled to make instruction-level analysis easier.
- Runtime memory addresses may change between executions.
- The observed assembly instructions are specific to the tested build and compilation environment.
- No vulnerability exploitation was performed.

## 10. Conclusion

This laboratory provided practical experience with GNU GDB and x86-64 Intel assembly analysis.

The experiments covered function disassembly, breakpoint management, register inspection, instruction-level execution, function return-value tracing, stack-frame examination, memory inspection, little-endian representation, and watchpoint monitoring.

The results strengthened the understanding of how compiled C programs execute at the machine level and established a practical foundation for further study in reverse engineering, binary analysis, and vulnerability research.

## 11. References

1. GNU Project. [Debugging with GDB](https://sourceware.org/gdb/current/onlinedocs/gdb/).
2. Intel. [Intel 64 and IA-32 Architectures Software Developer Manuals](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html).
3. System V Application Binary Interface. [AMD64 Architecture Processor Supplement](https://gitlab.com/x86-psABIs/x86-64-ABI).

---

**Lab Classification:** Practical Debugging and Binary Analysis  
**Architecture:** x86-64  
**Debugger:** GNU GDB  
**Assembly Syntax:** Intel  
**Environment:** Kali Linux
