#include   "os.h"

// file:///C:/SnowChar/AutumnRecruitment/OS/RISC-V/riscv-operating-system-mooc/refs/td16550.pdf  -----------------------------------------------


// 这里是定义了一个宏，作用是：根据偏移量，获得某个UART寄存器的指针
// UART0是UART的物理地址，reg是偏移量，
// (volatile uint8_t *)是  强制转成uint8_t 类型指针uint8_t 类型
#define UART_REG(reg) ((volatile uint8_t *)(UART0+reg))

/**
 * 1. Uart设备有自己的一套寄存器，各种读写操作本质上是通过写入它的这些专属寄存器实现的
 * 2. 这些专属寄存器是8位的，有读模式(read mode)和写模式(write mode)。
 * 3. 下面这些宏就是每个寄存器相对于UART的物理地址的 偏移量
 * 4. 有一些功能是公用一个寄存器，所以  是相同的偏移量
*/
#define RHR 0	// Receive Holding Register (read mode)
#define THR 0	// Transmit Holding Register (write mode)
#define DLL 0	// LSB of Divisor Latch (write mode)
#define IER 1	// Interrupt Enable Register (write mode)
#define DLM 1	// MSB of Divisor Latch (write mode)
#define FCR 2	// FIFO Control Register (write mode)
#define ISR 2	// Interrupt Status Register (read mode)
#define LCR 3	// Line Control Register
#define MCR 4	// Modem Control Register
#define LSR 5	// Line Status Register
#define MSR 6	// Modem Status Register
#define SPR 7	// ScratchPad Register

/*
 * 通电默认 各个寄存器状态
 * IER = 0: TX/RX holding register interrupts are both disabled     // TX/RX保持寄存器中断都被禁用
 * ISR = 1: no interrupt penting                                    // 没有中断
 * LCR = 0
 * MCR = 0
 * LSR = 60 HEX
 * MSR = BITS 0-3 = 0, BITS 4-7 = inputs
 * FCR = 0
 * TX = High
 * OP1 = High
 * OP2 = High
 * RTS = High
 * DTR = High
 * RXRDY = High
 * TXRDY = Low
 * INT = Low
 */

/**
 * LINE STATUS REGISTER (LSR寄存器)
 * 该寄存器的 第0位     LSR_RX_READY
 * 为0时，  代表 在接收保持寄存器或FIFO中没有数据。
 * 为1时，  代表 数据已被接收并保存在接收保持寄存器或FIFO中。
 * 
 * 该寄存器的 第5位     LSR_TX_IDLE
 * 为0时，  代表 发送保持寄存器已满。16550将不接受任何传输数据。
 * 为1时，  代表 发送器保持寄存器(或FIFO)为空。CPU可以加载下一个字符。
*/
#define LSR_RX_READY  (1<<0)
#define LSR_TX_IDLE   (1<<5)

#define uart_read_reg(reg)  (*(UART_REG(reg)))          // 读某个寄存器 的值
#define uart_write_reg(reg,v) (*(UART_REG(reg)) = (v))  // 向某个寄存器 写入 v


void uart_init()
{
    uart_write_reg(IER,0x00);    // 在IER写入0 ，关闭中断， 因为冲突？？？？？？
    
// *波特率设置。这里只是一个演示如果我们关心除数，但对于我们的目的[QEMU-virt]，这并没有真正做任何事情。
// ＊
// *注意除数寄存器DLL(除数锁存最小)和DLM(除数锁存最小)
// *锁存器大多数)有相同的基址作为接收器/发射器和
// * interrupt enable register。要更改基址所指向的内容，我们
// *通过将1写入除数锁存访问位打开“除数锁存”
// * (DLAB)，它是行控制寄存器(LCR)的位索引7。
// ＊
// *关于波特率值，请参见[1]“波特率生成器编程表”。
// *我们在1.8432 MHZ晶体时使用38.4K，所以对应的值为3。
// 由于除数寄存器是两个字节(16位)，所以我们需要
// *将3(0x0003)的值拆分为两个字节，DLL存储低字节;
// * DLM存储高字节。


// 为什么需要写LCR寄存器，因为DLL、DLM和读和写寄存器 复用了地址，
// 要么使人中断的效果，要不使用设置波特率的效果，功能只能2选1.
// 我们通过拨动一个开关来选择我们要的效果（设置LCR寄存器的第7位）

    uint8_t lcr=uart_read_reg(LCR); //
    uart_write_reg(LCR,lcr | (1<<7));
    uart_write_reg(DLL,0x03);   // Uart寄存器是8位，放一个16位数，需要两个寄存器 ，DLL，是低位
    uart_write_reg(DLM,0x00);   // DLM是高位

    /*
    *继续设置异步数据通信格式。
    * -字长数:8位
    * -停止位数:当字长为8位时为1位
    * -无奇偶校验
    * -无中断控制
    * -关闭波特锁
    */
   lcr=0;                       // 设置奇偶校验位
   uart_write_reg(LCR,lcr|(3<<0));// 这里就是不奇偶校验
   // 通过这个位可以选择奇偶校验或不奇偶校验。
    //    0没有
    // 1奇偶校验在发送时产生，在接收时校验。

    // 开启中断！！！！！！！
	uint8_t ier = uart_read_reg(IER);
	uart_write_reg(IER, ier | (1 << 0));
    
}

int uart_putc(char ch){
    /**
	 *  不断的读LSR寄存器的值 ，同时 与操作不断这个寄存器第5位是否为1，不满足条件说明 发送寄存器不空闲
	 * 如果满足条件，就把一个字符写入 发送寄存器
	*/
    while ((uart_read_reg(LSR) & LSR_TX_IDLE)==0);
    
    return uart_write_reg(THR,ch);
}

void uart_puts(char *s){
    while (*s)
    {
        while (*s)
        {
            uart_putc(*s++);
        }
        
    }
    
}

//读LSR寄存器第5位，为1时， 代表 数据已被接收并保存在接收保持寄存器或FIFO中。
int uart_getc(void)
{
	if (uart_read_reg(LSR) & LSR_RX_READY){
		return uart_read_reg(RHR);
	} else {
		return -1;
	}
}

/*
    处理一个uart中断，因为输入已经接受而触发发，从trap.c调用。
*/
void uart_isr(void)
{
	while (1) {
		int c = uart_getc();
		if (c == -1) {
			break;
		} else {
			uart_putc((char)c);
			uart_putc('\n');
		}
	}
}













