#include "platform.h"

    # ？？？ .equ??????????????????????????????????????????????
    .equ    STACK_SIZE,1024     # 栈的大小设置1024
    # .equ：这是一个汇编指令，用于定义常量。语法是 CONSTANT_NAME EQU expression。例如，TOTAL_STUDENTS equ 50，你可以在代码中使用这个常量值1。
    .global _start  # ???

    .text   # ??? 这是一个汇编指令，用于声明代码段的开始。

# 1. 判断当前hartid

_start:
    csrr    t0,mhartid
    mv  tp,t0
    bnez    t0,park
    # 2.hartid 不为0的hart跳走，为0的初始化栈指针

    slli    t0,t0,10                # 左移 10位
    la sp,stacks + STACK_SIZE       # 请问这是申请了 1024*8 +1024 栈的内存吗？？？？？？？？？？？？？？？？
    add sp,sp,t0                    # 1024*8 +1024 再加上0吗？？？？？？？？？？？？？？？？？？？？？？？？？？？？

    j   start_kernel

# 本系统暂时只运行在一个hart上，其他的hart 不断休眠
park:
    wfi
    j park

.align 16   # ???
# ?????????????????????????????????????????????
stacks:
    .skip   STACK_SIZE * MAXNUM_CPU
    # .skip：这是一个汇编指令，用于在数据段中跳过一定数量的字节，从而为变量分配空间。例如，.skip 1024 将在数据段中跳过 1024 个字节2。
    .end

# #这部分代码是在汇编语言中为所有的 hart（硬件线程）分配栈空间。下面是每行代码的解释：
# stacks:：这是一个标签，表示所有 hart 的栈空间的开始位置。
# .skip STACK_SIZE * MAXNUM_CPU：.skip 是一个汇编指令，用于在数据段中跳过一定数量的字节，从而为变量分配空间。在这里，它跳过了 STACK_SIZE * MAXNUM_CPU 个字节。STACK_SIZE 是每个 hart 的栈大小，MAXNUM_CPU 是 hart 的总数。所以，这行代码为所有的 hart 分配了足够的栈空间。
# .end：这是一个汇编指令，表示文件的结束。它告诉汇编器这是源文件的结尾，之后的所有内容都会被忽略。
# 总的来说，这段代码为每个 hart 分配了一个大小为 STACK_SIZE 字节的栈空间，并标记了文件的结束。