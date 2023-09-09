#include "os.h"

#define DELAY 1000
#define USE_LOCK//

extern void initlock(struct spinlock *lk);
extern void _spin_lock(struct spinlock *lk);
extern void _spin_unlock(struct spinlock *lk);
struct spinlock cpulock;

void user_task0(void)
{
	uart_puts("Task 0: Created!\n");

	// task_yield();
	uart_puts("Task 0: I'm back!\n");
	int i=20;

#ifdef USE_LOCK
		// spin_lock();
	struct spinlock *p=&cpulock;
	initlock(p);
	_spin_lock(p);	
#endif

	while (i--) {

		printf("Task 0: Running...   %d/20  \n",20-i);
		task_delay(DELAY);
		if(i==0){
			i=20;
			printf("Task 0 is run over\n");
			task_delay(10*DELAY);
		}


	}

#ifdef USE_LOCK
		// spin_unlock();
		_spin_unlock(p);
#endif

}

void user_task1(void)
{
	uart_puts("Task 1: Created!\n");
	int i=15;

#ifdef USE_LOCK
		// spin_lock();
	struct spinlock *p=&cpulock;
	// initlock(p);
	_spin_lock(p);	
#endif
	while (i--) {
		printf("Task 1: Running...   %d/20  \n",15-i);
		task_delay(DELAY);
		if(i==0){
			i=15;
			printf("Task 1 is run over\n");
			task_delay(10*DELAY);
		}
	}

#ifdef USE_LOCK
		// spin_unlock();
		_spin_unlock(p);
#endif
}


// 系统的主程序
//注意:不要在main()中无限循环
void os_main(void){
    task_create(user_task0);
    task_create(user_task1);
}