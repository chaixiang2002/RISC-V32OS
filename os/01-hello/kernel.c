// 声明 uart设备的初始化函数 和 uart设备的字符串 发送函数
extern void uart_init(void);
extern void uart_puts(char *s);


void start_kernel(void)
{
    uart_init();
    uart_puts("hello,SNowOS!\n");

    while (1)
    {    
    }
    
}