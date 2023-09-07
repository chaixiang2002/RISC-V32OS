#include "os.h"

#define DELAY 1000

void user_task0(void)
{
    printf("Task 0: Created!\n");
    while (1)
    {
        printf("Task 0: Running...\n");

        trap_test();// 异常测试

        task_delay(DELAY);
        task_yield();// 切换任务
    }
}

void user_task1(void)
{
    printf("Task 1: Created!\n");
    while (1)
    {
        printf("Task 1: Running...\n");
		task_delay(DELAY);
		task_yield();
    }
    
}


// 系统的主程序
//注意:不要在main()中无限循环
void os_main(void){
    task_create(user_task0);
    task_create(user_task1);
}