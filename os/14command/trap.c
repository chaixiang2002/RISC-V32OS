#include "os.h"

extern void trap_vector(void);
extern void uart_isr(void);
extern void timer_handler(void);
extern void schedule(void);
extern void do_syscall(struct context *cxt);

// 把 trap处理函数（trap_handler()）的地址 写入 mtvec寄存器 
void trap_init()
{
	w_mtvec((reg_t)trap_vector);
}

void external_interrupt_handler()
{
	int irq = plic_claim();

	if (irq == UART0_IRQ){
      		uart_isr();
	} else if (irq) {
		printf("unexpected interrupt irq = %d\n", irq);
	}
	
	if (irq) {
		plic_complete(irq);
	}
}

reg_t trap_handler(reg_t epc, reg_t cause,struct context *cxt)
{
	reg_t return_pc = epc;   // epc记录着 trap前pc值
	reg_t cause_code = cause & 0xfff; //获得trap码
	
	if (cause & 0x80000000) {
		/* Asynchronous trap - 中断 */
		switch (cause_code) {
		case 3:
			uart_puts("software interruption!\n");
			/*
			*通过清除确认软件中断
			* mip中的MSIP位。
			*/
			int id = r_mhartid();
    		*(uint32_t*)CLINT_MSIP(id) = 0;

			schedule();

			break;
		case 7:
			// uart_puts("timer interruption!\n");
			timer_handler();
			break;
		case 11:
			// uart_puts("external interruption!\n");
			external_interrupt_handler();
			break;
		default:
			uart_puts("unknown async exception!\n");
			break;
		}
	} else {
		/* Synchronous trap - 异常 */
		printf("Sync exceptions!, code = %d\n", cause_code);
		switch (cause_code)
		{
			case 8:
				printf("System call from U-mode\n");
				do_syscall(cxt);
				return_pc+=4;
				break;
			
			default:
				panic("OOPS! What can I do!");
				//return_pc += 4;
				break;
		}	
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



