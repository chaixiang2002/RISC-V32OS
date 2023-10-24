#include "os.h"

#define DELAY 1000
#define USE_LOCK//

extern void initlock(struct spinlock *lk);
extern void _spin_lock(struct spinlock *lk);
extern void _spin_unlock(struct spinlock *lk);

// void user_task0(void)
// {
// 	printf("The system timer is start..")
//     task_delay(2*DELAY);


// }


// // 系统的主程序
// //注意:不要在main()中无限循环
// void os_main(void){
//     task_create(user_task0);
// }