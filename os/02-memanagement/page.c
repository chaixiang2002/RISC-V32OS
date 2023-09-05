#include "os.h"

// 使用 mem.S的定义的变量
extern uint32_t TEXT_START;
extern uint32_t TEXT_END;
extern uint32_t DATA_START;
extern uint32_t DATA_END;
extern uint32_t RODATA_START;
extern uint32_t RODATA_END;
extern uint32_t BSS_START;
extern uint32_t BSS_END;
extern uint32_t HEAP_START;
extern uint32_t HEAP_SIZE;

/*
 * _alloc_start指向堆池的实际起始地址
 * _alloc_end指向堆池的实际结束地址
 * _num_pages保存我们可以分配的实际最大页面数。
 */
static uint32_t _alloc_start=0;
static uint32_t _alloc_end=0;
static uint32_t _num_pages=0;

#define PAGE_SIZE 4096
#define PAGE_ORDER 12   //？？？

#define PAGE_TAKEN (uint8_t)(1<<0) //page_token:用第一位表示自己是否被用掉 ,这是一个掩码，
#define PAGE_LAST (uint8_t)(1<<1)//page_last:当好几个page被化成一块给用户时，用第二位表示自己是不是 该内存块的 最后一个

/*
*页面描述符
*标志:
* -位0:表示该页已被占用(已分配)
* -位1:表示该页是分配的内存块的最后一页
*/
struct Page
{
    uint8_t flags;// 8位
};

static inline void _clear(struct Page *page){
    page->flags=0;
};

static inline int _is_free(struct Page *page)
{
    if(page->flags & PAGE_TAKEN){
        return 0;
    }else{
        return 1;
    }
}

static inline void _set_flag(struct Page *page,uint8_t flags)
{
    page->flags |=flags;
};

static inline int _is_last(struct Page *page)
{
    if(page->flags & PAGE_LAST){
        return 1;
    }else{
        return 0;
    }
}

// ????????????????
static inline uint32_t _align_page(uint32_t address)//将地址与页面边框对齐(4K)
{
    uint32_t order=(1<< PAGE_ORDER)-1;
    return (address+order) & (~order);// ~ 按位取反运算符
}

void page_init()
{
    _num_pages=(HEAP_SIZE / PAGE_SIZE)-8;//计算 我们可以分配的实际最大页面数
    printf("HEAP_START = %x, HEAP_SIZE = %x, num of pages = %d\n", HEAP_START, HEAP_SIZE, _num_pages);

    struct Page *page=(struct Page *)HEAP_START;//定义初始化一个 页指针 指向 HEAP_START
    for(int i=0;i<_num_pages;i++){  // 把所有的页 全部清零
        _clear(page);
        page++;
    }

    _alloc_start=_align_page(HEAP_START+8*PAGE_SIZE);//动态堆可分配的  开始
    _alloc_end=_alloc_start+(PAGE_SIZE*_num_pages);//动态堆可分配的  结束

	printf("TEXT:   0x%x -> 0x%x\n", TEXT_START, TEXT_END);
	printf("RODATA: 0x%x -> 0x%x\n", RODATA_START, RODATA_END);
	printf("DATA:   0x%x -> 0x%x\n", DATA_START, DATA_END);
	printf("BSS:    0x%x -> 0x%x\n", BSS_START, BSS_END);
	printf("HEAP:   0x%x -> 0x%x\n", _alloc_start, _alloc_end);
}

void *page_alloc(int npages)
{
    int found=0;
    struct Page *page_i = (struct Page *)HEAP_START;
    for(int i = 0; i <= (_num_pages - npages); i++){
        if(_is_free(page_i)){
            found=1;

            struct Page *page_j = page_i + 1;
            for(int j = i + 1; j < (i + npages); j++){
                if(!_is_free(page_j)){  // 该段连续的可分配空间不够，放弃，继续重新找
                    found=0;
                    break;
                }
                page_j++;
            }

            if(found){
                struct Page *page_k=page_i;
                for (int k = i; k < (i+npages); k++)
                {   
                    _set_flag(page_k,PAGE_TAKEN);
                    page_k++;
                }
                page_k--;
                _set_flag(page_k,PAGE_LAST);
                return (void *)(_alloc_start+i*PAGE_SIZE);
            }
        }
        page_i++;
    }
    return NULL;
}

void page_free(void *p)
{
    if(!p || (uint32_t)p >= _alloc_end){
        return;
    }
	struct Page *page = (struct Page *)HEAP_START;
    while (!_is_free(page))
    {
        if (_is_last(page)){
            _clear(page);
            break;
        }else{
            _clear(page);
            page++;
        }
    }
}

void page_test()
{
    void *p = page_alloc(2);
	printf("p = 0x%x\n", p);
	//page_free(p);

	void *p2 = page_alloc(7);
	printf("p2 = 0x%x\n", p2);
	page_free(p2);

	void *p3 = page_alloc(4);
	printf("p3 = 0x%x\n", p3);
}


