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
    sw      zero,(a0)       # 初始化 .bss 段的内容
    addi    a0,a0,4
    bltu    a0,a1,1b        # 如果 a0 小于 a1，则跳转到标签 1 处，继续循环执行初始化操作
2:
    # slli    t0,t0,10                # 左移 10位 
    la sp,stacks + STACK_SIZE       # 请问这是申请了 1024*8 +1024 栈的内存吗？？？？？？？？？？？？？？？？
    # add sp,sp,t0    
                    # 1024*8 +1024 再加上0吗？？？？？？？？？？？？？？？？？？？？？？？？？？？？
    
# ---
# https://lore.kernel.org/qemu-devel/20201223192553.332508-1-atish.patra@wdc.com/
# 对于qemu version >= 6.0，如果没有配置PMP enty，将引发异常。所以只要配置一个条目，它就允许所有的整体
# 32位物理地址范围为R/W/X。
# FIXME:我说这是一个临时的解决方案，因为我认为上面的补丁包含错误，我已经向qemu提出了新的问题，但直到现在还没有根因。详情请参阅
# https://gitlab.com/qemu-project/qemu/-/issues/585或
# https://gitee.com/unicornx/riscv-operating-system-mooc/issues/I441IC(中文)
# 所以这只是一个临时的解决方案，直到现在，不阻止人们想要尝试新的qemu(>= 6.0)。
	li      t0, 0xffffffff  # 写入立即数0xffffffff
	csrw    pmpaddr0, t0    # pmpaddr0？？？？？？？
	li      t0, 0xf
	csrw    pmpcfg0, t0     # pmpcfg0？？？？？？？

# 在start_kernel结束时，schedule()将调用MRET进行切换
# 到第一个任务，所以我们在这里准备状态。
# 注意:默认mstatus为0

# 设置mstatus。MPP为0，因此我们将在MRET后以用户模式运行。
# 无需设置mstatus。MPIE显式地转换为1，因为根据ISA
# specification: m模式(高于u模式)的中断总是全局启用，而不管全局MIE位的设置。
# 所以最后我们把t0重置为0。
    # li  t0,0    #


    # # 在start_kernel结束时，schedule()将调用MRET进行切换
    # # 到第一个任务，所以我们在这里准备状态。
    # # 注意:默认mstatus为0
    # # 设置mstatus。MPP到3，所以在MRET之后我们仍然在Machine模式下运行。
    # # 设置mstatus。MPIE为1，因此MRET将启用中断。

	# li	t0, 3 << 11 | 1 << 7    # 11是Machine级别
	# csrr	a1, mstatus

    # Set mstatus.MPP as 0, so we will run in User mode after MRET.
	# No need to set mstatus.MPIE to 1 explicitly, because according to ISA
	# specification: interrupts for M-mode, which is higher than U-mode, are
	# always globally enabled regardless of the setting of the global MIE bit.
    li	t0, 3 << 11
	csrc	mstatus, t0
    
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