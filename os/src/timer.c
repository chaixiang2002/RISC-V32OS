#include "os.h"

extern void schedule(void);
/* interval ~= 1s */
#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ

static uint32_t _tick = 0;

static int single_task_mode =0;//
void set_single_task_mode(int on){// 确保在切换单任务模式时发生任务切换，
	__sync_lock_test_and_set(&single_task_mode,on);
}

// 软件定时器
#define MAX_TIMER 10
static struct timer timer_list[MAX_TIMER];			// 定时器数组

/* 写入下一个时间中断时刻 */
void timer_load(int interval)
{
	/* 每个CPU都有一个单独的定时器中断源。 */
	int id = r_mhartid();
	
	*(uint64_t*)CLINT_MTIMECMP(id) = *(uint64_t*)CLINT_MTIME + interval;
}

void timer_init()
{

	// 软件定时器初始化
	struct timer *t=&(timer_list[0]);
	for(int i=0;i<MAX_TIMER;i++){
		t->func=NULL;
		t->arg=NULL;
		t++;
	}

    /*
    在复位时，mtime被清除为零，但mtimecmp寄存器不复位。
    所以我们必须手动初始化mtimecmp。
    */
	timer_load(TIMER_INTERVAL);

	/* 开启 machine-mode timer interrupts. */
	w_mie(r_mie() | MIE_MTIE);

	/* 开启 machine-mode global interrupts. */
	// w_mstatus(r_mstatus() | MSTATUS_MIE);
}



struct timer *timer_create(void (*handler)(void *arg), void *arg, uint32_t timeout)
{
	/* TBD: params should be checked more, but now we just simplify this */
	if (NULL == handler || 0 == timeout) {
		return NULL;
	}

	/* use lock to protect the shared timer_list between multiple tasks */
	spin_lock();

	struct timer *t = &(timer_list[0]);
	for (int i = 0; i < MAX_TIMER; i++) {
		if (NULL == t->func) {				//通过回调函数的指针是否为空判断这个定时器是否被使用了
			break;
		}
		t++;
	}
	if (NULL != t->func) {
		spin_unlock();
		return NULL;
	}

	t->func = handler;
	t->arg = arg;
	t->timeout_tick = _tick + timeout;

	spin_unlock();

	return t;
}

void timer_delete(struct timer *timer)
{
	spin_lock();

	struct timer *t = &(timer_list[0]);
	for (int i = 0; i < MAX_TIMER; i++) {
		if (t == timer) {
			t->func = NULL;
			t->arg = NULL;
			break;
		}
		t++;
	}

	spin_unlock();
}

static inline void timer_check()
{
	struct timer *t = &(timer_list[0]);
	for (int i = 0; i < MAX_TIMER; i++) {
		if (NULL != t->func) {
			if (_tick >= t->timeout_tick) {			// —tick 定时器计数器 与 定好的时间 做比较
				t->func(t->arg);

				/* once time, just delete it after timeout */
				t->func = NULL;						// 因为我们做的第一个单次触发的定时器，触发一次后，定时器就释放了   
				t->arg = NULL;

				break;
			}
		}
		t++;
	}
}


// 定时器处理函数，重新定时
void timer_handler() 
{
	_tick++;
	// if(_tick%10==0){
	// 	printf("tick: %d\n", _tick);
	// 	// display_time();
	// }

	timer_check();

	timer_load(TIMER_INTERVAL);

	// spin_lock();
	if (single_task_mode==0) {// 单任务模式关闭，不停切换
		schedule();
	}
	// spin_unlock();

}

void display_time() {
	unsigned int seconds=_tick;
    unsigned int baseTime = 0; // 2023年10月10日0点0分0秒的时间戳
    unsigned int eventTime = baseTime + seconds;
    unsigned int years, months, days, hours, minutes, secs;

    // 将总秒数转换为年、月、日、时、分、秒
    secs = eventTime % 60;
    minutes = (eventTime / 60) % 60;
    hours = (eventTime / (60 * 60)) % 24;
    days = (eventTime / (60 * 60 * 24)) % 30; // 假设一个月为30天
    months = (eventTime / (60 * 60 * 24 * 30)) % 12;
    years = (eventTime / (60 * 60 * 24 * 30 * 12));

    // 输出日期和时间
    printf("\rThe time is: %d-%d-%d %d:%d:%d\n", 
        2023 + years, 10 + months, 10 + days, hours, minutes, secs);
}
