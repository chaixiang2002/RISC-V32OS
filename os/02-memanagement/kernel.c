#include "os.h"
// 声明 uart设备的初始化函数 和 uart设备的字符串 发送函数
extern void uart_init(void);
// extern void uart_puts(char *s);
extern void page_init(void);


void start_kernel(void)
{
    uart_init();
    uart_puts("hello,SNowOS!\n");

    page_init();//

    while (1)
    {    
    }
    
}