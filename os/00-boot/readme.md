Make file:

riscv64-unknown-elf-gcc -nostdlib -fno-builtin -march=rv32ima -mabi=ilp32 -g -Wall -c  kernel.c -o  kernel.o

riscv64-unknown-elf-gcc -nostdlib -fno-builtin -march=rv32ima -mabi=ilp32 -g -Wall -c  start.S -o  start.o

riscv64-unknown-elf-gcc -nostdlib -fno-builtin -march=rv32ima -mabi=ilp32 -g -Wall -Ttext=0x80000000   kernel.o start.o -o  os.elf

sudo qemu-system-riscv32 -nographic -smp 1 -machine virt -bios none -kernel os.elf 



- `riscv64-unknown-elf-gcc`：这是 GCC 编译器的 RISC-V 版本。RISC-V 是一种开源的指令集架构。
- `-nostdlib`：这个选项告诉编译器不要链接标准库。这通常在编写操作系统或嵌入式系统时使用。
- `-fno-builtin`：这个选项禁止编译器自动使用内建函数，除非程序员明确地调用它们。
- `-march=rv32ima`：这个选项设置目标架构为 RV32IMA，这是 RISC-V 的一个变种，包括整数（I）、乘法和除法（M）以及原子（A）指令。
- `-mabi=ilp32`：这个选项设置二进制接口（ABI）为 ILP32，意味着 int、long 和指针都是 32 位的。
- `-g`：这个选项让编译器生成调试信息，以便使用像 GDB 这样的调试器进行调试。
- `-Wall`：这个选项让编译器生成所有警告信息，帮助开发者发现潜在的问题。
- `-Ttext=0x80000000`：这个选项设置程序的加载地址为 0x80000000。
- `kernel.o start.o`：这些是输入文件，包含了你要编译的代码。
- `-o os.elf`：这个选项指定输出文件的名称为 os.elf。

总的来说，这条命令将 `kernel.o` 和 `start.o` 这两个对象文件链接成一个名为 `os.elf` 的 ELF 可执行文件，不使用标准库，并且生成所有警告和调试信息。程序将被加载到内存地址 `0x80000000`。目标架构设置为 RV32IMA，二进制接口设置为 ILP32。
禁止编译器自动使用内建函数的原因通常与系统级编程有关。在许多情况下，例如在编写操作系统或嵌入式系统的代码时，程序员需要对系统有完全的控制，包括所有执行的代码。这意味着他们可能需要自己实现一些通常由编译器自动处理的功能。

内建函数是编译器提供的一组函数，它们通常用于执行一些常见任务，如内存管理和数学运算。然而，在某些情况下，这些函数可能不适用，或者可能无法提供足够的控制。例如，一个嵌入式系统可能有特殊的内存管理需求，不能使用标准的 malloc 和 free 函数。


-Ttext=0x80000000参数指定的起始地址具有特殊含义。这个地址是一个虚拟地址，常用于嵌入式系统和操作系统开发中。以下是一些关于该地址的常见含义：

内核空间：在某些操作系统中，虚拟地址0x80000000到0xFFFFFFFF被保留用作内核空间。这意味着这个地址范围内的内存是专门为操作系统内核使用的，用户程序无法直接访问。

虚拟内存映射：在一些嵌入式系统和操作系统中，0x80000000地址也用于虚拟内存映射。通过在操作系统中设置适当的页表和映射机制，可以将物理内存映射到这个地址范围内，从而实现虚拟内存的管理。

这条命令是用来启动 QEMU 虚拟机并加载你的 RISC-V 程序的。下面是每个参数的解释：

- `sudo`：这个命令让你以超级用户（root）的身份运行接下来的命令。这通常是因为启动虚拟机需要特殊的系统权限。
- `qemu-system-riscv32`：这是 QEMU 虚拟机的 RISC-V 32 位版本。QEMU 是一个开源的处理器模拟器和虚拟机。
- `-nographic`：这个选项告诉 QEMU 不要启动图形界面，只使用命令行界面。
- `-smp 1`：这个选项设置虚拟机有一个模拟的 CPU 核心。
- `-machine virt`：这个选项设置虚拟机使用 "virt" 机器模型，这是一个为 RISC-V 设计的简单和通用的模型。
- `-bios none`：这个选项告诉 QEMU 不要加载任何 BIOS，因为你的程序将直接运行在裸机上。
- `-kernel os.elf`：这个选项指定要加载到虚拟机中运行的程序（在这种情况下是 `os.elf`）。

总的来说，这条命令将启动一个没有图形界面、只有一个 CPU 核心、使用 "virt" 机器模型、不加载 BIOS 的 QEMU 虚拟机，并加载并运行 `os.elf` 程序。希望这个解释对你有所帮助！

内容介绍

1. 设置引导过程，地址
2. 上电、判断hartid，初始化栈，转跳c函数入口


启动顺序
1. start.S   引入   platform.h 
2. start.S   跳转到 kernel.c
3. kernel.c  不停循环



riscv64-unknown-elf-objdump -d  kernel.o > kernel.s