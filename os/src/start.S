#include "platform.h"

    # ？？？ .equ??????????????????????????????????????????????
    .equ    STACK_SIZE,1024     # 栈的大小设置1024

    .global _start  # 定义一个叫_start全局的符号，使得链接器能够全局识别它，般用于定义程序的入口标签

    .text   # ???

# 1. 判断当前hartid

_start:
    csrr    t0,mhartid
    mv      tp,t0
    bnez    t0,park
    # 2.hartid 不为0的hart跳走，为0的初始化栈指针


    la a0, _bss_start       # a0，a1 放入 _bss的开始和结束
    la a1, _bss_end
    bgeu    a0, a1, 2f      # a1 >= a0（无符号数比较），才会跳转 2f

# 新添1：

1:  
    sw      zero,(a0)       # ？？？？？？？？？？？？？？？？？？？
    addi    a0,a0,4
    bltu    a0,a1,1b
2:
    slli    t0,t0,10                # 左移 10位
    la sp,stacks + STACK_SIZE       # 请问这是申请了 1024*8 +1024 栈的内存吗？？？？？？？？？？？？？？？？
    add sp,sp,t0    
                    # 1024*8 +1024 再加上0吗？？？？？？？？？？？？？？？？？？？？？？？？？？？？
    
    # 在start_kernel结束时，schedule()将调用MRET进行切换
    # 到第一个任务，所以我们在这里准备状态。
    # 注意:默认mstatus为0
    # 设置mstatus。MPP到3，所以在MRET之后我们仍然在Machine模式下运行。
    # 设置mstatus。MPIE为1，因此MRET将启用中断。
	li	t0, 3 << 11 | 1 << 7
	csrr	a1, mstatus
	or	t0, t0, a1
	csrw	mstatus, t0
    
    j   start_kernel



# 本系统暂时只运行在一个hart上，其他的hart 不断休眠
park:
    wfi
    j park

.align 16   # 将当前PC地址推进到2的16次方字节对齐的位置， 2字节对齐
# ?????????????????????????????????????????????
stacks:
    .skip   STACK_SIZE * MAXNUM_CPU

    .end