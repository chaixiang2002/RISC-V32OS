.include "reg_macro.S"
.text

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