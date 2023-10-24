#ifndef __OS_H__
#define __OS_H__

#include "types.h"
#include "platform.h"
#include "riscv.h"

// ？？ 可以自己到std库的东西吗？
#include<stddef.h>
#include<stdarg.h>

// uart
extern int uart_putc(char ch);
extern void uart_puts(char *s);
extern int uart_getc(void);

// printf
extern int printf(const char* s,...);// 什么高级用法? printf() 函数也支持变长参数列表，可以使用省略号 ... 来表示，用于指定要输出的多个值。
extern void panic(char *s);

// 内存管理
extern void *page_alloc(int npages);
extern void page_free(void *p);

// 任务管理
struct context
{
    /* 忽略 x0 */
	reg_t ra;
	reg_t sp;
	reg_t gp;
	reg_t tp;
	reg_t t0;
	reg_t t1;
	reg_t t2;
	reg_t s0;
	reg_t s1;
	reg_t a0;
	reg_t a1;
	reg_t a2;
	reg_t a3;
	reg_t a4;
	reg_t a5;
	reg_t a6;
	reg_t a7;
	reg_t s2;
	reg_t s3;
	reg_t s4;
	reg_t s5;
	reg_t s6;
	reg_t s7;
	reg_t s8;
	reg_t s9;
	reg_t s10;
	reg_t s11;
	reg_t t3;
	reg_t t4;
	reg_t t5;
	reg_t t6;

	// 保存下一个任务的pc
	reg_t pc; // offset: 31 *4 = 124
};

extern int task_create(void (*task)(void));
extern void task_delay(volatile int count);//任务延迟count 豪秒
extern void task_yield();//


// plic外部中断
extern int plic_claim(void);
extern void plic_complete(int irq);

/* 锁 */
extern int spin_lock(void);
extern int spin_unlock(void);

struct spinlock
{
    int locked;
};
extern void initlock(struct spinlock *lk);
extern void _spin_lock(struct spinlock *lk);
extern void _spin_unlock(struct spinlock *lk);


// 软件定时器
struct timer {
	void (*func)(void *arg);		// 执行函数的入口地址，			执行函数需要用户自己定义，是一个函数指针
	void *arg;						// 执行函数参数的指针， 		最终这个参数将传入 上面的执行函数里
	uint32_t timeout_tick;			// 超时计数器
};
extern struct timer *timer_create(void (*handler)(void *arg), void *arg, uint32_t timeout);
extern void timer_delete(struct timer *timer);


#endif