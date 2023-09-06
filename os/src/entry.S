# 保存所有通用寄存器到上下文。
# struct context *base = &ctx_task;
# base->ra = ra;
# ……这些要保存的通用寄存器不包含gp
# 和tp，因为它们不是调用者保存的或
# callee-saved。这两个寄存器经常被使用
# 用于特殊用途。例如，在RVOS中，'tp'
# (又名“线程指针”)用于存储hartid，
# 是全局值，不能更改
# 在上下文切换期间。

# .macro和.endm用来定义一个宏。
.macro reg_save base
	sw ra, 0(\base)
	sw sp, 4(\base)
	sw t0, 16(\base)
	sw t1, 20(\base)
	sw t2, 24(\base)
	sw s0, 28(\base)
	sw s1, 32(\base)
	sw a0, 36(\base)
	sw a1, 40(\base)
	sw a2, 44(\base)
	sw a3, 48(\base)
	sw a4, 52(\base)
	sw a5, 56(\base)
	sw a6, 60(\base)
	sw a7, 64(\base)
	sw s2, 68(\base)
	sw s3, 72(\base)
	sw s4, 76(\base)
	sw s5, 80(\base)
	sw s6, 84(\base)
	sw s7, 88(\base)
	sw s8, 92(\base)
	sw s9, 96(\base)
	sw s10, 100(\base)
	sw s11, 104(\base)
	sw t3, 108(\base)
	sw t4, 112(\base)
	sw t5, 116(\base)
    # 这里我们不保存t6，因为我们使用了它作为base，
    # 我们必须在额外的步骤中保存t6在reg_save之外
.endm

# 从上下文中恢复所有通用寄存器
# 除了gp和tp。
# struct context *base = &ctx_task;
# ra = base->ra;
# ……

.macro reg_restore base
	lw ra, 0(\base)
	lw sp, 4(\base)
	lw t0, 16(\base)
	lw t1, 20(\base)
	lw t2, 24(\base)
	lw s0, 28(\base)
	lw s1, 32(\base)
	lw a0, 36(\base)
	lw a1, 40(\base)
	lw a2, 44(\base)
	lw a3, 48(\base)
	lw a4, 52(\base)
	lw a5, 56(\base)
	lw a6, 60(\base)
	lw a7, 64(\base)
	lw s2, 68(\base)
	lw s3, 72(\base)
	lw s4, 76(\base)
	lw s5, 80(\base)
	lw s6, 84(\base)
	lw s7, 88(\base)
	lw s8, 92(\base)
	lw s9, 96(\base)
	lw s10, 100(\base)
	lw s11, 104(\base)
	lw t3, 108(\base)
	lw t4, 112(\base)
	lw t5, 116(\base)
	lw t6, 120(\base)
.endm

# 关于保存/恢复的注意事项:
# -我们使用mscratch保存一个指向当前任务上下文的指针
# -我们使用t6作为reg_save/reg_restore的“基础”，因为它是最底层寄存器(x31)，在加载期间不会被覆盖。
# 注意:由于加载/恢复指令只接受通用寄存器，CSRs(mscratch)不能用作'基'。

.text
# void switch_to(struct context *next);
# a0:指向下一个任务上下文的指针
.globl switch_to        # 定义程序的入口标签 switch_to
.align 4                # 将当前PC地址推进到2的4次方字节对齐的位置

switch_to:
    csrrw   t6,mscratch,t6  # 交换 t6 和 mscratch里的值
    beqz    t6,1f           # t6等于0时，转跳1标签(系统刚启动时，t6=0)

    reg_save t6             # 保存上一个任务的上下文

    # 保存实际的t6寄存器，我们将其交换到mscratch中
    mv t5,t6                # t5 指向当前任务的上下文
    csrr    t6,mscratch     # csrr，读取一个 CSR 的值到通用寄存器。如：csrr t0, mstatus，读取 mstatus 的值到 t0 中
    sw t6,120(t5)           # 保存t6，以t5为base


1:
    # 切换 mscratch 去 指向下一个任务的上下文
    csrw    mscratch,a0 # 把a0给mscratch，a0 装的是 想要跳转任务的上下文的地址
 
    # 恢复所有通用寄存器
    # 使用t6指向新任务的上下文
    mv t6,a0        # a0 装的是 想要跳转任务的上下文的地址
    reg_restore t6  # 恢复 想要跳转任务的上下文

    # Do actual context switching.
	ret			    # ret时，pc寄存器 会设置为 ra寄存器存的地址


.end