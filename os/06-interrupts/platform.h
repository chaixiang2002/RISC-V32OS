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
#define PLIC_PRIORITY(id) (PLIC_BASE + (id) * 4)
#define PLIC_PENDING(id) (PLIC_BASE + 0x1000 + ((id) / 32) * 4)
#define PLIC_MENABLE(hart) (PLIC_BASE + 0x2000 + (hart) * 0x80)
#define PLIC_MTHRESHOLD(hart) (PLIC_BASE + 0x200000 + (hart) * 0x1000)
#define PLIC_MCLAIM(hart) (PLIC_BASE + 0x200004 + (hart) * 0x1000)
#define PLIC_MCOMPLETE(hart) (PLIC_BASE + 0x200004 + (hart) * 0x1000)


#endif /* __PLATFORM_H__ */