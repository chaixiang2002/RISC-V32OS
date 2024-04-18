.include "reg_macro.S"

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
	csrr	a2, mscratch	# !!!!!!!!!! 多了上下文一个参数
	call 	trap_handler	# 相对于 trap_handler(mempc,mcause)

	# ------------------------------------------------------------
	# Trap_handler将通过a0返回返回地址。
	csrw	mepc,a0

	# trap已经处理完了，恢复trap前的任务上下文
	csrr	t6,mscratch
	reg_restore t6

	# 回到我们在trap之前做的事情。
	mret 	# mret 指令，会将 mepc 中的值恢复到 pc 中（实现返回的效果）

.end