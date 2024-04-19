#ifndef __PLATFORM_H__
#define __PLATFORM_H__


//  定义cpu(heart) 8个核
#define MAXNUM_CPU 8

// 定义UART0的物理地址          uart设备在内存上的映射
#define UART0   0x10000000L

/*
* UART0中断源
*参见https://github.com/qemu/qemu/blob/master/include/hw/riscv/virt.h
* enum {
* uart0_irq = 10;
*     ......
*};
*/
#define UART0_IRQ 10
/*
*这台机器在这里放置了平台级中断控制器(PLIC)。
*此处仅列出机器模式下的PLIC寄存器。
*参见https://github.com/qemu/qemu/blob/master/include/hw/riscv/virt.h
* #define VIRT_PLIC_HART_CONFIG     "MS"
* #define VIRT_PLIC_NUM_SOURCES     127
* #define VIRT_PLIC_NUM_PRIORITIES  7
* #define VIRT_PLIC_PRIORITY_BASE   0x04
* #define VIRT_PLIC_PENDING_BASE    0x1000
* #define VIRT_PLIC_ENABLE_BASE     0x2000
  #define VIRT_PLIC_ENABLE_STRIDE   0x80
* #define VIRT_PLIC_CONTEXT_BASE    0x200000
  #define VIRT_PLIC_CONTEXT_STRIDE  0x1000
* #define VIRT_PLIC_SIZE(__num_context)  (VIRT_PLIC_CONTEXT_BASE + (__num_context) * VIRT_PLIC_CONTEXT_STRIDE)
*/

#define PLIC_BASE 0x0c000000L
#define PLIC_PRIORITY(id) (PLIC_BASE + (id) * 4)  //设置某一路中断源的优先级。
#define PLIC_PENDING(id) (PLIC_BASE + 0x1000 + ((id) / 32) * 4) //用于指示某一路中断源是否发生， 1 表示发生了中断，可以通过 claim 方式清除
#define PLIC_MENABLE(hart) (PLIC_BASE + 0x2000 + (hart) * 0x80) // 针对某个 hart 开启或者关闭某一路中断源， 1 表示使用能该中断源
#define PLIC_MTHRESHOLD(hart) (PLIC_BASE + 0x200000 + (hart) * 0x1000) //针对某个 hart 设置中断源优先级的阈值。0允许所有，7丢弃所有中断
#define PLIC_MCLAIM(hart) (PLIC_BASE + 0x200004 + (hart) * 0x1000)  
#define PLIC_MCOMPLETE(hart) (PLIC_BASE + 0x200004 + (hart) * 0x1000)
//Claim 和 Complete 是同一个寄存器，每个 Hart 一个。
//对该寄存器执行读操作称之为 Claim，即获取当前发生的最高优先级的中断源 ID。Claim 成功后会清除对应的 Pending 位。
//对该寄存器执行写操作称之为 Complete。所谓 Complete 指的是通知 PLIC 对该路中断的处理已经结束。

/*
核心本地中断器(CLINT)块保存内存映射控制和
*与软件和定时器中断相关的状态寄存器。
* QEMU-virt为CLINT重用5个配置。
*参见https://gitee.com/qemu/qemu/blob/master/include/hw/riscv/sifive_clint.h
* enum {
* SIFIVE_SIP_BASE = 0，
* SIFIVE_TIMECMP_BASE = 0x4000，
* SIFIVE_TIME_BASE = 0xBFF8
*};
＊
* enum {
* sifive_clint_timebase_freq = 10000000
*};
＊
*注意:
* mip寄存器的机器级MSIP位通过访问来写入
*内存映射控制寄存器，它被远程hart用来提供
*机器模式处理器间中断。
*对于QEMU-virt机器，每个msip寄存器是一个32位宽的WARL寄存器
*上面的31位被绑定为0。最低有效位是
*反映在mip CSR的MSIP位。我们可以编写msip来生成
*机器模式软件中断。一个待定的机器级软件
*中断可以通过在mip的MSIP位上写0来清除。
*复位时，每个msip寄存器被清除为零。
*/
#define CLINT_BASE 0x2000000L
#define CLINT_MSIP(hartid) (CLINT_BASE + 4 * (hartid)) //每个处理器核心的软件中断控制寄存器（MSIP寄存器）。1是中断，0是解决了
#define CLINT_MTIMECMP(hartid) (CLINT_BASE + 0x4000 + 8 * (hartid)) //比较计数器
#define CLINT_MTIME (CLINT_BASE + 0xBFF8)   //计数器
/* 10000000次/秒 */
#define CLINT_TIMEBASE_FREQ 10000000 //定时间隔

#endif 