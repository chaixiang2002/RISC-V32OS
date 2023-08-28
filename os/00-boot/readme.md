内容介绍

1. 设置引导过程，地址
2. 上电、判断hartid，初始化栈，转跳c函数入口


启动顺序
1. start.S   引入   platform.h 
2. start.S   跳转到 kernel.c
3. kernel.c  不停循环