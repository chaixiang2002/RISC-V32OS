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
};

extern int task_create(void (*task)(void));
extern void task_delay(volatile int count);//任务延迟count 豪秒
extern void task_yield();//


#endif