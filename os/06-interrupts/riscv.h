#ifndef __RISCV_H__
#define __RISCV_H__

#include "types.h"
/*
 * ref: https://github.com/mit-pdos/xv6-riscv/blob/riscv/kernel/riscv.h
 */


// 获得tp寄存器的值并返回	tp寄存器放的时系统启动的hartid
static inline reg_t r_tp()
{
	reg_t x;
	asm volatile("mv %0, tp" : "=r" (x) );
	return x;
}


// 获得mhartid寄存器的值并返回( 查看当前hart )
static inline reg_t r_mhartid()
{
	reg_t x;
	asm volatile("csrr %0, mhartid" : "=r" (x) );
	return x;
}

/* Machine模式下的寄存器 mstatus*/

// xPP：保存trap发生前的权限级别
/*
- 没有UPP
- SPP占1位
- MPP占2位
  当trap发生时，模式只会由高到低，不会切换到更高的模式，也因此取名“trap”，“陷入”的意思。
  U模式之前只能是U模式，只有这一个可能，所以省略
  S模式之前可能是U模式转的，或还是U模式，2可能，占1bit
  M模式3中可能，占2bit
*/
#define MSTATUS_MPP (3<<1)
#define MSTATUS_SPP (1<<8)

// xPIE：保存trap发生前的xIE值
#define MSTATUS_MPIE (1 << 7)
#define MSTATUS_SPIE (1 << 5)
#define MSTATUS_UPIE (1 << 4)

// xIE：M/S/U的全局中断是否打开，1开 0闭， tarp发生时，xIE自动设0
#define MSTATUS_MIE (1 << 3)
#define MSTATUS_SIE (1 << 1)
#define MSTATUS_UIE (1 << 0)

// 获得mstatus寄存器的值并返回
static inline reg_t r_mstatus()
{
	reg_t x;
	asm volatile("csrr %0, mstatus" : "=r" (x) );
	return x;
}

// 向mstatus寄存器写入值
static inline void w_mstatus(reg_t x)
{
	asm volatile("csrw mstatus, %0" : : "r" (x));
}

// machine异常程序计数器，保存异常返回的指令地址。
// 向 mepc寄存器 写入值
static inline void w_mepc(reg_t x)
{
	asm volatile("csrw mepc, %0" : : "r" (x));
}

// 获取 mepc寄存器 的值
static inline reg_t r_mepc()
{
	reg_t x;
	asm volatile("csrr %0, mepc" : "=r" (x));
	return x;
}

// 向 mscratch 写入值
static inline void w_mscratch(reg_t x)
{
	asm volatile("csrw mscratch, %0" : : "r" (x));
}

// 向 mtvec 写入值
static inline void w_mtvec(reg_t x)
{
	asm volatile("csrw mtvec, %0" : : "r" (x));
}

/* Machine-mode Interrupt 开启状态 */
#define MIE_MEIE (1 << 11) // external  外部
#define MIE_MTIE (1 << 7)  // timer     计时器
#define MIE_MSIE (1 << 3)  // software  软件

// 获取 mie 的值
static inline reg_t r_mie()
{
	reg_t x;
	asm volatile("csrr %0, mie" : "=r" (x) );
	return x;
}

// 写 mie 的值
//用于进一步控制（打开和关闭）software interrupt/timer interrupt/external interrupt
static inline void w_mie(reg_t x)
{
	asm volatile("csrw mie, %0" : : "r" (x));
}

// 获取 r_mcause 的值，中断码
static inline reg_t r_mcause()
{
	reg_t x;
	asm volatile("csrr %0, mcause" : "=r" (x) );
	return x;
}





#endif