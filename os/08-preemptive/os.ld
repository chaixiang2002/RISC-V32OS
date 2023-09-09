/* GNU ld 使用 Linker Script 来描述和控制链接过程。
Ø Linker Script 是简单的纯文本文件，采用特定的脚本描述语言编写。
Ø 每个 Linker Script 中包含有多条命令（Command)

Ø gcc -T os.ld  */

/*
* https://sourceware.org/binutils/docs/ld/Miscellaneous-Commands.html
* OUTPUT_ARCH命令指定一个特定的输出机器架构。
*“riscv”是64位和32位的体系结构名称
* RISC-V目标。我们将通过使用-march=rv32ima进一步细化它
*和-mabi=ilp32时调用gcc。
*/

/* 指定输出的架构 */
OUTPUT_ARCH( "riscv" ) 

ENTRY( _start )			/* 设置 _start 为入口 ,ENTRY：指定入口地址，注意这里使用的是代码中定义的_start符号，也就是说脚本中可以直接访问符号表中的符号*/

/*
* https://sourceware.org/binutils/docs/ld/MEMORY.html
* MEMORY命令描述内存块的位置和大小目标。
* MEMORY的语法是:
*内存
* {
* name [(attr)]: ORIGIN = ORIGIN, LENGTH = len
*     ......
*}
*每一行定义一个内存区域。
在memory命令中，每个内存区域必须有一个不同的名称。在这里我们只定义一个区域命名为“ram”。 "attr"字符串是一个可选的属性列表，用来指定是否
*使用一个特定的内存区域的输入部分，这是不显式的
*在链接器脚本中映射。这里我们赋值'w'(可写)，'x'(可执行)，
*和'a'(可分配的)。我们用'!'反转'r'(只读)和
* 'i'(初始化)。
*“ORIGIN”用于设置内存区域的起始地址。在这里,我们
*把它放在0x8000_0000的开头，因为这是
QEMU-virt机器将开始执行。
*最后LENGTH = 128M告诉链接器我们有128mb的RAM。
*链接器将仔细检查，以确保一切都可以安装。
*/



/* MEMORY：描述板上的存储器位置，ORIGIN为起始地址，LENGTH为字节数，其中ORIGIN可缩写为org或o，LENGTH可缩写为len或l */
MEMORY 
{
    /* RAM是随机存取存储器, ROM是只读存储器（Read-Only Memory）*/
    ram   (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 128M /* 申请 起始地址0x80000000，，长度128M 的ri权限的内存 */
    /* 命名为ram的一块内存 */
    /*  R 只读section
        W 读/写section
        X 可执行section
        A 可分配的section
        I 初始化了的section
        L 同I
        ! 不满足该字符之后的任何一个属性的section 
    */
}

/*
* https://sourceware.org/binutils/docs/ld/SECTIONS.html
* SECTIONS命令告诉链接器如何将输入节映射到输出节，以及如何在内存中放置输出节。
* SECTIONS命令的格式为:
*部分
* {
* sections-command
* sections-command
*     ......
*}
＊
*每一节命令可以是下列命令之一:
*(1)一个ENTRY命令
*(2)符号赋值
*(3)输出节描述
*(4)覆盖描述
我们这里只演示(2)和(3)。
＊
*我们使用提供命令定义符号。
* https://sourceware.org/binutils/docs/ld/PROVIDE.html
*提供关键字可以用来定义一个符号。
*语法为提供(symbol = expression)。
*诸如“_text_start”、“_text_end”…将用于mems。
*注意句号'。'告诉链接器设置符号(例如:_text_start)
*当前位置('。' =当前内存位置)。当前内存
*位置随着我们添加东西而移动。
*/



SECTIONS
{
    /*
    *我们将在.text output section中布局所有文本部分，
    *以.text开头.前面的星号(*)
    *括号表示匹配任何目标文件的.text部分。
    */
    .text : {
        
        PROVIDE(_text_start = .); /* PROVIDE,相当于定义一个全局变量, .代表当前地址， _text_start记录这个当前地址*/
        *(.text .text.*)        /* 把输入setion 的.text 放入 目标saction的 .test */
        PROVIDE(_text_end = .);   /*  把 当前指针地址 记录为 _text_end */

    } >ram                      /* .text的全部 放在 ram这块内存 */

    .rodata : {
        PROVIDE(_rodata_start = .);
		*(.rodata .rodata.*)
		PROVIDE(_rodata_end = .);
    } >ram

    .data : {
        /*
        *.= ALIGN(4096)告诉链接器对齐当前内存
        *位置为4096字节.这将插入填充字节，直到
        *当前位置在4096字节边界上对齐。
        这是因为我们的分页系统的分辨率是4096字节。
        */
        . = ALIGN(4096);
        PROVIDE(_data_start = .);

        /*
        * sdata和data本质上是一样的。我们不需要
        *区分sdata和data。
        */
        *(.sdata .sdata.*)
        *(.data .data.*)
        PROVIDE(_data_end = .);

    }>ram

    .bss : {
        /*
        * https://sourceware.org/binutils/docs/ld/Input-Section-Common.html
        *在大多数情况下，将放置输入文件中的通用符号
        *在'.输出文件中Bss的部分。
        */
        PROVIDE(_bss_start = .);
		*(.sbss .sbss.*)
		*(.bss .bss.*)
		*(COMMON)
		PROVIDE(_bss_end = .);

    }>ram

/* 定义_memory_start变量，存储 名为ram这块内存开始的地址 */
/* 定义_memory_end变量，存储 名为ram这块内存结束的地址 */
    PROVIDE(_memory_start = ORIGIN(ram)); 
    PROVIDE(_memory_end = ORIGIN(ram)+LENGTH(ram));

    PROVIDE(_heap_start = _bss_end);						/* ram最后的节是.bss，.bss的结束地址就是  就是 heap的开始 */
	PROVIDE(_heap_size = _memory_end - _heap_start);		/* heap的大小 是 .bss的结束 到 memory的结束 的所有内存  */
}