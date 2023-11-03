#include "os.h"
#include "user_api.h"


#define DELAY 10000

extern void display_time();

// struct spinlock cpulock;//cpulock锁
// static int use_lock=0;//define

static char buf[1024];
static int state=7;//0无命令 1判断执行  5 打印  6 时间  -1error 7开机界面
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

 
int parse_command(const char *command) {
    char cmd[1024], arg[1024], rest[1024];
    int i, j;

    // 提取命令
    for (i = 0; command[i] != ' ' && command[i] != '\0'; i++) {
        cmd[i] = command[i];
    }
    cmd[i] = '\0';

    // 跳过空格
    while (command[i] == ' ') {
        i++;
    }

    // 提取参数
    if (command[i] == '-') {
        i++;  // 跳过'-'
        for (j = 0; command[i] != ' ' && command[i] != '\0'; i++, j++) {
            arg[j] = command[i];
        }
        arg[j] = '\0';
    } else {
        arg[0] = '\0';  // 没有参数
    }

    // 跳过空格
    while (command[i] == ' ') {
        i++;
    }

    // 提取剩余部分
    for (j = 0; command[i] != '\0'; i++, j++) {
        rest[j] = command[i];
    }
    rest[j] = '\0';

	if(strcmp(cmd, "print")==0){
		printf("\n> %s",rest);
		return 1;
	}
	return 0;
	printf("------------------asdasdasd");
    // printf("Command: %s\n", cmd);
    // printf("Argument: %s\n", arg);
    // printf("Rest: %s\n", rest);
}
void create_task_by_name(const char* function_name) {
    if (strcmp(function_name, "user_task1") == 0) {
        // 创建 user_task1 任务
        // task_create(user_task1);
        printf("Creating user_task1...\n");
    }else {
        printf("Function %s does not exist!\n", function_name);
    }
}

void to_handle(int num){//0无命令 1判断执行  5 打印  6 时间  -1error 7开机界面 8关机界面 9清屏 10显示状态码 11 单任务模式开启 12 单任务模式关闭 13创建task0
	if (strcmp(buf, "time")==0) {
		// printf("yes\n");
		state=6;
	}	else if (strcmp(buf, "to-gui")==0) {
		state=7;
	}else if(strcmp(buf, "shutdown")==0){
		state=8;
	}else if(strcmp(buf, "clear")==0){
		state=9;
	}else if(strcmp(buf, "show state")==0){
		state=10;
	}else if(strcmp(buf, "system single_task_mode on")==0){
		state=11;
	}else if(strcmp(buf, "system single_task_mode off")==0){
		state=12;
	}else if(strcmp(buf, "create task0")==0){
		state=13;
	}else if(strcmp(buf, "create your_want_task")==0){
		state=14;
	}
	// else if(parse_command(buf)==1){
	// 	state=0;
	// 	clear_buf();
	// }
	else {
		state=-1;
	}

	// printf("%d\n",state);
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
	// clear_screen();
	command_mode();
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

void your_want_task(void){
	printf("我的自定义任务: 开始运行了!\n");

	int i=10;
	while (i--) {
		printf("我的自定义任务: 运行中 %d!\n",i);
		task_delay(DELAY);// 暂停1秒
	}

	printf("我的自定义任务: 结束了!\n");

}


extern void welcome();
void user_main(void){

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
			break;
		case 6:
			// printf("to--->:\n");
			display_time();
			clear_buf();
			state=0;
			break;
		case 7://7开机界面
			welcome();
			state=0;
			break;
		case 8://8关机界面
			exit_gui();
			clear_buf();
			state=0;
			/////////////////////////////////////////////////////////////////关闭所有任务
			break;
		case 9://9清屏
			clear_screen();
			clear_buf();
			state=0;
			break;
		case 10://10显示状态码
			printf("//0无命令 1判断执行  5 打印  6 时间  -1error 7开机界面 8关机界面 9清屏 10显示状态码 11 单任务模式开启 12 单任务模式关闭 13创建task0\n");
			printf("状态码:%d",state);
			clear_buf();
			state=0;
			break;
		case 11://11 单任务模式开启 12 单任务模式关闭
			// spin_lock();
			set_single_task_mode(1);
			clear_buf();
			state=0;
			break;
		case 12://11 单任务模式开启 12 单任务模式关闭
			// spin_unlock();
			set_single_task_mode(0);
			clear_buf();
			state=0;
			break;
		case 13://13创建task0
			task_create(user_task0);
			clear_buf();
			state=0;
			break;
		case 14://13创建task0
			task_create(your_want_task);
			clear_buf();
			state=0;
			break;
		case -1:
			printf("%s is not recognized as an internal or external command,operable program or batch file.\n",buf);
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

void user_task1(void){
	printf("Task 1: Created!\n");

	while (1) {
		printf("Task 1: Running... \n");
		task_delay(DELAY);
	}
}



void os_main(void)
{
	task_create(user_main);
	// task_create(user_task0);
	// task_create(user_task1);
	// task_create(your_want_task);// 如果想要开机自启，请在这里写

}

