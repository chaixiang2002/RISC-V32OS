启动顺序
start.S->mem.S->entry.S->  
kernel.c->uart.c->printf.c->page.c->sched.c->user.c->trap.c->plic.c->timer.c

新增


更新情况
1. start.S      改动？？
2. entry.S      更改trap_vector，switch_to的逻辑
3. os.h         context结构里多了pc
4. sched.c      开启软件中断和触发一个软件中断
5. user.c       小改逻辑
6. trap.c       多了软件中断时的处理函数
7. plic.c       少了 开启Machine模式全局中断 的代码？？？为什么？
8. timer.c      少了 开启Machine模式全局中断 的代码？？？为什么？ 多了软件中断时的处理函数


无改动


修复抢占式bug，记一次重大bug修复  
起因：
1. entry.S的switch_to重写后不再用ret，而是mret（因为抢占式需要trap跳转）
2. 然而，sched.c的task_create()函数中，ctx_tasks[_top].ra = (reg_t) start_routin;该句子没有一起修改

解决方案：  
ctx_tasks[_top].ra = (reg_t) start_routin;  ——>     ctx_tasks[_top].pc = (reg_t) start_routin;

解释：
ret			    # ret时，pc寄存器 会设置为 ra寄存器存的地址  
mret 			# mret 将 PC 设置为 mepc的值




