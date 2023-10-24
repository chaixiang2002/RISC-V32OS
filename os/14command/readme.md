启动顺序
start.S->mem.S->entry.S->usys.S

- 内核
kernel.c->

- IO
uart.c
printf.c

- 内存
page.c

- 调度
sched.c
user.c

- 陷入和中断
trap.c
plic.c

- 定时器
timer.c

- 锁
lock.c
spinlock.c

- 系统调用
syscall.c
systask.c


- 其他
os.h
os.ld
platform.h
riscv.h



<!-- 增加系统任务
1. 时间任务
2. 输入响应 -->
!!!!!!!
新增系统调用

注意！！！ trap_handler函数多了一个参数（当前上下文地址！！）

新增
1. usys.S       -系统调用的转跳，不同的系统调用有不同的号码
2. syscall.c    -系统调用函数的具体实现和 之间调度函数
3. syscall.h    -纪录所有的系统调用号
4. user_api.h   -用户模式系统调用api


更新情况
1. start.S      -设置mstatus寄存器MPP和MPIE位，使得在进入内核时 为用户态
1. entry.S      !!!!!修改了
2. trap.c       -根据异常码，调用相应的系统函数。
3. user.c       -测试系统调用功能



无改动



