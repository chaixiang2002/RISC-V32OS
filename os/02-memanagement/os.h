#ifndef __OS_H__
#define __OS_H__

#include "types.h"
#include "platform.h"


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

#endif