启动顺序
start.S->mem.S->entry.S->  
kernel.c->uart.c->printf.c->page.c->sched.c->user.c
新增
1. entry.S  定义通用寄存器的保存与恢复，任务的切换
2. sched.c  任务创建调度的初始化
3. user.c




更新情况
1. kernel.c 新增任务切换相关函数
2. os.h     新增任务切换的数据结构和函数
3. types.h  多定义了一个reg_t的数据结构

无改动
1. platform.h
2. start.S
3. mem.S
4. os.ld
5. page.c
6. printf.c
7. uart.c
