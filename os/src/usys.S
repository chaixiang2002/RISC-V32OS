#include "syscall.h"

.global gethid
gethid:
    li a7,SYS_gethid    # 规定a7放系统调用号
    ecall
    ret