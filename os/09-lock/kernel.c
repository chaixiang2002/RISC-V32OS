#include "os.h"
// 声明 uart设备的初始化函数 和 uart设备的字符串 发送函数
extern void uart_init(void);
extern void page_init(void);
extern void sched_init(void);
extern void schedule(void);
extern void os_main(void);
// extern void uart_puts(char *s); 被封装到printf（）
extern void trap_init(void);
extern void plic_init(void);
extern void timer_init(void);

void start_kernel(void)
{
    uart_init();// uart 串口设备初始化
    uart_puts("hello,SNowOS!\n");

    page_init();    // 内存页的初始化

    trap_init();    // trap初始化

    plic_init();    // plic设备初始化

    timer_init();   // 硬件定时器初始化

    sched_init();// 任务调度初始化

    os_main();//     user的任务创建

    schedule();//     任务切换
    printf("hello,i am chaixiang!");
    printf("Would not go here!\n");

    while (1)
    {    
    }
    
}