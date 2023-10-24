#include "os.h"
#include "user_api.h"

#define DELAY 4000

extern void display_time();

static char buf[1024];
static int state=0;//0无命令 1判断执行  5 打印  6 时间  -1error
void setbuf(const char *str) {
    int i;
    for (i = 0; i < sizeof(buf) - 1 && str[i] != '\0'; i++) {
        buf[i] = str[i];
    }
    buf[i] = '\0';
}
int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

void to_go(){
	state=1;
}
void to_handle(int num){//0无命令 1判断执行  5 打印  6 时间  -1error
	if (strcmp(buf, "time")==0) {
		// printf("yes\n");
		state=6;
	}	else {
		state=-1;
	}
}


void append_char(char c) {
    int i;
    // 找到buf的当前结束位置
    for (i = 0; i < sizeof(buf) - 1 && buf[i] != '\0'; i++);
    // 如果buf没有满，将字符追加到buf中
    if (i < sizeof(buf) - 1) {
        buf[i] = c;
        buf[i + 1] = '\0';
    }
}


const char* getbuf(void) {
    return buf;
}

int is_empty(void) {
			// uart_putc('9');

    return buf[0] == '\0';
}

void clear_buf(void) {
    buf[0] = '\0';
}

void user_task0(void)
{
	uart_puts("Task 0:Created!\n");
	unsigned int hid=-1;

	/*
	 * if syscall is supported, this will trigger exception, 
	 * code = 2 (Illegal instruction)
	 */
	//hid = r_mhartid();
	//printf("hart id is %d\n", hid);

	int ret = -1;
	ret =gethid(&hid);
	if(!ret){
		printf("system call returned!, hart id is %d\n", hid);
	}else {
		printf("gethid() failed, return: %d\n", ret);
	}

	while (1)
	{
		printf("Task 0: Running... \n");
		task_delay(DELAY);
	}
	
	
}

void user_task1(void){
	uart_puts("Task 1: Created!\n");
	while (1) {
		uart_puts("Task 1: Running... \n");
		task_delay(DELAY);
	}
}

void user_task2(void){
	
	while (1) {
		// printf("%d\n",state);//0无命令 1判断执行  5 打印  6 时间  -1error
		switch (state) {
		case 0://无命令
			break;
		case 1:
			to_handle(1);//1判断执行
			break;
		case 5:// 
			printf(buf);
			clear_buf();
			state=0;
			break;
		case 6:
			// printf("to--->:\n");
			display_time();
			clear_buf();
			state=0;
			break;
		default:
			printf("%s is not recognized as an internal or external command,operable program or batch file.\n",buf);
			clear_buf();
			state=0;
			break;
		}
	}
}

void os_main(void)
{
	// task_create(user_task0);
	// task_create(user_task1);
	task_create(user_task2);
}

