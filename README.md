这是一个基于RSIC-v指令集的32为操作系统，该项目尚处于学习模仿阶段，暂时没有自己新的东西，请见谅。     
有兴趣的朋友可以学习交流一下，看看每一节注释，毕竟我的理解可能有误。    

学习的对象是 -> https://www.bilibili.com/video/BV1Q5411w7z5?p=18&vd_source=f30dd07ede5a1255c0b1aaf0d2788938

11个部分
1. 引导部分
2. uart串口设备
3. 内存管理
4. 上下文切换和协作式多任务
5. Trap和Exception
6. 外部设备中断
7. 硬件定时器
8. 抢占式多任务
9. 任务同步和锁
10. 软件定时器
11. 内核态切换



环境
1. qemu-system-riscv32 
2. riscv64-unknown-elf-gdb
3. riscv64-unknown-elf-gcc
4. riscv64-unknown-elf-objcopy
5. riscv64-unknown-elf-objdump

运行
cd ~/os/src  
sudo make run




清除
cd ~/os/src  
make clean

