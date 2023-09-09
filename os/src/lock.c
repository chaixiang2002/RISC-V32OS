#include "os.h"

int spin_lock()
{
    w_mstatus(r_mstatus() & ~MSTATUS_MIE);// 11111111011, 写0，关闭中断
    return 0;
}

int spin_unlock()
{
    w_mstatus(r_mstatus() | MSTATUS_MIE);// 0000000100    写1，开启中断
    return 0;
}