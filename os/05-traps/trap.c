#include "os.h"

extern void trap_vector(void);

// 把 trap处理函数（trap_handler()）的地址 写入 mtvec寄存器 
void trap_init()
{
	w_mtvec((reg_t)trap_vector);
}

reg_t trap_handler(reg_t epc, reg_t cause)
{
	reg_t return_pc = epc;   // epc记录着 trap前pc值
	reg_t cause_code = cause & 0xfff; //获得trap码
	
	if (cause & 0x80000000) {
		/* Asynchronous trap - interrupt */
		switch (cause_code) {
		case 3:
			uart_puts("software interruption!\n");
			break;
		case 7:
			uart_puts("timer interruption!\n");
			break;
		case 11:
			uart_puts("external interruption!\n");
			break;
		default:
			uart_puts("unknown async exception!\n");
			break;
		}
	} else {
		/* Synchronous trap - exception */
		printf("Sync exceptions!, code = %d\n", cause_code);
		panic("OOPS! What can I do!");
		//return_pc += 4;
	}

	return return_pc;
}

void trap_test(){
    /*
        *同步异常码= 7
        * Store/AMO访问故障
        * 写入非法
        */
    *(int *)0x00000000=100;
	uart_puts("Yeah! I'm return back from trap!\n");

}



