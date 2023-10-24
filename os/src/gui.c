#include "os.h"
void clear_screen() {
    // ANSI转义序列 "\033[2J\033[H" 用于清除屏幕并将光标移动到左上角
    printf("\033[2J\033[H");
}

void welcome(){
    printf("-------------------------------------------------------------------------\n");
    printf("                                                                         \n");
    printf("                                                                         \n");
    printf("                                                                         \n");
    printf("                              RISC-V-32操作系统                           \n");
    printf("                                                                         \n");
    printf("                                                                         \n");
    printf("                                            --chaixiang2002 · GitHub     \n");
    printf("按n+回车键进入命令行                                                       \n");
    printf("-------------------------------------------------------------------------\n");
    printf("root@snow:/$ ");
}
void command_mode(){
    printf("\rroot@snow:/$ ");
}

void exit_gui(){
    printf("-------------------------------------------------------------------------\n");
    printf("                                                                         \n");
    printf("                                                                         \n");
    printf("                                                                         \n");
    printf("                                   再见                                  \n");
    printf("                                                                         \n");
    printf("                                                                         \n");
    printf("                                            --chaixiang2002 · GitHub     \n");
    printf("                                                                         \n");
    printf("-------------------------------------------------------------------------\n");
}

void welcome1(){
    printf("-------------------------------------------------------------------------\n");
    printf("                                                                         \n");
    printf("                                                                         \n");
    printf("                                                                         \n");
    printf("                                                                         \n");
    printf("                                                                         \n");
    printf("                                                                         \n");
    printf("                                                                         \n");
    printf("                                                                         \n");
    printf("                                                                         \n");
    printf("------------------------------------------------------------------------\n");
}