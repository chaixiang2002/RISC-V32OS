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

# 中断和异常在machine模式下来到这里。
.globl	trap_vector
#  trap vector基址必须始终在4字节边界上对齐
.align 4
trap_vector:
# 补充：临时寄存器，清零没有用。对于临时寄存器坚持“先赋值再使用”的原则 ！！！！
# mepc		装的是某个任务具体到哪指令的地址
# mscratch	装的是某个任务上下文的基地址
	# 保存trap前任务的上下文
	csrrw	t6,mscratch,t6	# 交换	t6,mscratch
	reg_save t6				# 把trap前任务的寄存器上下文保存到 mscratch中的地址

	# 再单独保存的t6寄存器
	mv t5,t6				# t5 指向 当前上下文
	csrr	t6,mscratch		# 读 mscratch的值到 t6
	sw	t6,120(t5)			# 以t5为基+120的地址上保存t6的值，

	# 再单独把mepc寄存器保存		，将mepc保存到当前任务的上下文
	csrr	a0, mepc
	sw	a0, 124(t5)


	# -------------------------------------------------------------
	# 恢复 trap任务的上下文
	csrw 	mscratch,t5 # 把mscratch的值再从t5中拿回来

	# 调用trap. C中的trap处理程序
	csrr	a0,mepc
	csrr	a1,mcause
	call 	trap_handler	# 相对于 trap_handler(mempc,mcause)

	# ------------------------------------------------------------
	# Trap_handler将通过a0返回返回地址。
	csrw	mepc,a0

	# trap已经处理完了，恢复trap前的任务上下文
	csrr	t6,mscratch
	reg_restore t6

	# 回到我们在trap之前做的事情。
	mret 	# mret 指令，会将 mepc 中的值恢复到 pc 中（实现返回的效果）



# void switch_to(struct context *next);
# a0:指向下一个任务上下文的指针
.globl switch_to        # 定义程序的入口标签 switch_to
.align 4                # 将当前PC地址推进到2的4次方字节对齐的位置

switch_to:	# 补充：临时寄存器，清零没有用。对于临时寄存器坚持“先赋值再使用”的原则 ！！！！
	# 切换mscratch以指向下一个任务的上下文的基地址
	csrw	mscratch,	a0
	# 设置mepc为下一个任务所在的PC
	lw	a1,124(a0)		# 根据基地址把该任务的pc取出到a1
	csrw	mepc,a1

	# 恢复所有通用寄存器
	# 用t6指向下一个任务的上下文
	mv	t6,a0
	reg_restore t6

	# 真正做上下文切换的地方
	# 注意 mret会开启全局中断????
	# ret			    # ret时，pc寄存器 会设置为 ra寄存器存的地址
	mret 				# mret 将 PC 设置为 mepc的值



.end