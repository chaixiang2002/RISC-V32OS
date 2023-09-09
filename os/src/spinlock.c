#include "os.h"

// 0可用  1被占用


void initlock(struct spinlock *lk){
    lk->locked=0;
}

void _spin_lock(struct spinlock *lk){
    while (__sync_lock_test_and_set(&lk->locked,1)!=0);    
}

void _spin_unlock(struct spinlock *lk){
    lk->locked=0;
}