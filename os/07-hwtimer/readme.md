启动顺序
start.S->mem.S->entry.S->  
kernel.c->uart.c->printf.c->page.c->sched.c->user.c->trap.c->plic.c->timer.c

新增
1. timer.c      -定义硬件定时器的相关函数




更新情况
1. kernel.c     -多了timer的初始化
2. trap.c       -多了timer_handle()
4. platfrom.h   -多了CLINT相关的宏


无改动
1. start.S
2. mem.S
3. entry.S
4. printf.c
5. page.c
6. sched.c
7. user.c       
8. riscv.h
9. types.h
10. os.ld


<!-- 一个任务的创建与切换时这样的

调度模块的初始化
1. sched_init()->w_mscratch(0)->csrw mscratch, %0   把mscratch寄存器写0

任务的创建
1. task_create(user_task0);创建任务     ->user.c
2. ctx_tasks[_top].sp =...;此时ctx_tasks[0]就已经初始化了，这个任务的栈有了空间和有自己地址了   ->sched.c

切换
1. task_yield()->schedule()  struct context *next= &(ctx_tasks[_current]);
此时，next存储着下一个任务的栈地址了            ->sched.c
2. switch_to(next)->       a0存储着next的值
3. csrrw   t6,mscratch,t6  把t6和mscratch的值交换
4. beqz    t6,1f           发现  t6是0 跳转到 1标签的程序段  
--1
5. csrw    mscratch,a0     把next给到 mscratch       
6. reg_restore t6          恢复上下文 以0为基指             ***！！！ 切换任务的核心语句 
7. ret        

--正常部分  
reg_save t6             # 保存上一个任务的上下文  
mv t5,t6                t5保存t6
csrr    t6,mscratch     t6写入mscratch（此时是将要被调度的任务的栈的地址）
sw t6,120(t5)           以t5(此时值为0)的值为基址+120的地方保存t6


补充：临时寄存器，清零没有用。对于临时寄存器坚持“先赋值再使用”的原则 -->


