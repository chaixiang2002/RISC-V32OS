.section .rodata
.global HEAP_START
HEAP_START: .word _heap_start   # 定义 堆的开始

.global HEAP_SIZE
HEAP_SIZE: .word _heap_size     # 定义 堆的大小

.global TEXT_START
TEXT_START: .word _text_start   # 定义 _text段的开始

.global TEXT_END
TEXT_END: .word _text_end       # 定义 _text段的结束

.global DATA_START
DATA_START: .word _data_start   # 定义 _data段的开始

.global DATA_END
DATA_END: .word _data_end       # 定义 _data段的结束

.global RODATA_START
RODATA_START: .word _rodata_start # 定义 _rodata段的开始

.global RODATA_END
RODATA_END: .word _rodata_end       # 定义 _rodata段的结束

.global BSS_START
BSS_START: .word _bss_start     # 定义 _bss段的开始

.global BSS_END
BSS_END: .word _bss_end         # 定义 _bss段的结束

