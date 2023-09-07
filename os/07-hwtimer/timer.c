#include "os.h"

/* interval ~= 1s */
#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ

static uint32_t _tick = 0;


/* 写入下一个时间中断时刻 */
void timer_load(int interval)
{
	/* 每个CPU都有一个单独的定时器中断源。 */
	int id = r_mhartid();
	
	*(uint64_t*)CLINT_MTIMECMP(id) = *(uint64_t*)CLINT_MTIME + interval;
}

void timer_init()
{
    /*
    在复位时，mtime被清除为零，但mtimecmp寄存器不复位。
    所以我们必须手动初始化mtimecmp。
    */
	timer_load(TIMER_INTERVAL);

	/* 开启 machine-mode timer interrupts. */
	w_mie(r_mie() | MIE_MTIE);

	/* 开启 machine-mode global interrupts. */
	w_mstatus(r_mstatus() | MSTATUS_MIE);
}

// 定时器处理函数，重新定时
void timer_handler() 
{
	_tick++;
	printf("tick: %d\n", _tick);

	timer_load(TIMER_INTERVAL);
}