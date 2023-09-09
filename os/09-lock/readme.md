启动顺序
start.S->mem.S->entry.S->  
kernel.c->uart.c->printf.c->page.c->sched.c->user.c->trap.c->plic.c->timer.c

新增
1.lock.c        全局中断锁
1.spinlock.c    自旋锁


更新情况
1. os.h
2. user.c


无改动



