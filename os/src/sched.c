#include "os.h"

extern void switch_to(struct context *next);// 实现在entry.S

// 定义任务最大数量和 每个任务的栈的大小
#define MAX_TASKS 10
#define STACK_SIZE 1024  //？？？？

//在标准的RISC-V调用约定中，堆栈指针sp总是16字节对齐

uint8_t __attribute__((aligned(16))) task_stack[MAX_TASKS][STACK_SIZE];// __attribute__()用法 https://blog.csdn.net/zhizhengguan/article/details/111470648

struct context ctx_tasks[MAX_TASKS];
/*
* _top      用于标记ctx_tasks的最大可用位置
* _current  用于指向当前任务的上下文
*/
static int _top=0;
static int _current =-1;

static void w_mscratch(reg_t x) // 写入x到mscratch寄存器
{
	asm volatile("csrw mscratch, %0" : : "r" (x));
}

void sched_init()
{
    w_mscratch(0);//任务切换初始化时 ,写入0到mscratch寄存器
}

/*
*实现一个简单的周期FIFO调度（先进先出）
*/

void schedule()
{
    if(_top<=0){
        panic("error：任务号 小于等于 0！！");
    }

    _current=(_current+1)%_top;
    struct context *next= &(ctx_tasks[_current]); // next是上下文类型指针 ，指向当前任务的上下文
    switch_to(next);
}

// 通过id切换任务
void schedule_id(volatile int id)
{
    if(id<=0 || id>=_top){
        panic("error: id<=0 || id>=_top !!");
    }

    _current=id;
    struct context *next= &(ctx_tasks[_current]); // next是上下文类型指针 ，指向当前任务的上下文
    switch_to(next);
}

/*
*描述
*   创建任务。
*   - start_routin:任务例程入口
*返回值
*   0:成功
*   -1:表示错误发生
*/

/*
在这个特定的上下文中，`start_routin` 是一个函数指针参数，用于指定任务的入口点
当调用 `task_create` 函数时，可以将一个函数的地址作为参数传递给 `start_routin`，这个函数就会成为新创建的任务的入口点。
*/
int task_create(void (*start_routin)(void))
{
	if (_top < MAX_TASKS) {
		ctx_tasks[_top].sp = (reg_t) &task_stack[_top][STACK_SIZE];	//在当前的任务上下文数组，初始化一个新任务栈，
		ctx_tasks[_top].ra = (reg_t) start_routin;	//start_routin是一个函数指针
		_top++;
		return 0;
	} else {
		return -1;
	}
}

/*
* task_yield()使调用任务放弃CPU和一个新的线程
*任务开始运行。
*/
void task_yield(){
    schedule();
}

/*
*一个非常粗糙的实现，只是为了消耗CPU
*/
void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}