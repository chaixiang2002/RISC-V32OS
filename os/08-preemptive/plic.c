#include "os.h"

void plic_init(void){
    int hart=r_tp();// 获取hartid

    /*
    *设置UART0的优先级。
    ＊
    *每个PLIC中断源可以通过写入来分配优先级
    *到32位内存映射优先级寄存器。
    * QEMU-virt(与FU540-C000相同)支持7级优先级。
    *优先级值为0表示“永不中断”和
    *有效地禁用中断。
    *优先级1最低，优先级7最高。
    *具有相同优先级的全局中断之间的联系被
    *中断ID;具有最低ID的中断具有最高ID
    *有效的优先级。
    */
   *(uint32_t*)PLIC_PRIORITY(UART0_IRQ) = 1;//uart设备的优先级设为1

    /*
     *启用UART0
     ＊
     *每个全局中断可以通过在使能寄存器中设置相应的位来使可用。
     */
    *(uint32_t*)PLIC_MENABLE(hart)= (1 << UART0_IRQ);//启用uart的中断 ENABLE=1

    /*
    *设置UART0的优先级阈值。
    ＊
    * PLIC将屏蔽优先级小于或等于阈值的所有中断。
    *最大阈值为7。
    例如，阈值为0允许所有的中断
    *非零优先级，而7的值掩码所有中断。
    *注意，阈值是全局的PLIC，而不是每个中断源。
    */
   *(uint32_t*)PLIC_MTHRESHOLD(hart) = 0;//将该hart的阈值设为0，什么中断都响应

    // 开启Machine模式 外部中断
    w_mie(r_mie() | MIE_MEIE);

    // 开启Machine模式 全局中断
    // w_mstatus(r_mstatus() | MSTATUS_MIE);
}

/*
*描述:
*查询PLIC应该服务哪个中断。
*通过读取请求寄存器来执行中断请求
*返回最高优先级等待中断的ID，如果有则返回零
*没有挂起的中断。
请求成功也会自动清除相应的挂起位
*在中断源上。
*返回值:
*最高优先级等待中断的ID，如果有，则为零
*没有挂起的中断。
*/

// 通过读claim寄存器，知道哪个设备发出的中断，返回中断码
int plic_claim(void)
{
	int hart = r_tp();
	int irq = *(uint32_t*)PLIC_MCLAIM(hart);
	return irq;
}

/*
*描述:
写入从请求(irq)接收到的中断ID到
*完整的寄存器将向PLIC发出信号，我们已经为这个IRQ提供了服务。
* PLIC不会检查完成ID是否与
*该目标的最后索赔ID。如果完成ID不匹配
*中断源，当前为目标启用，完成
*被静默忽略。
*返回值:无
*/

// 对complete进行写入操作，该寄存器发出写入时，是通知 PLIC 对该路中断的处理
void plic_complete(int irq)
{
	int hart = r_tp();
	*(uint32_t*)PLIC_MCOMPLETE(hart) = irq;
}
