/*
 * 文件名称 : ut_mem.h
 * 文件作用 : URL传输内存管理模块接口定义
 *
 * 文件作者 : 邱俊涛
 * 创建日期 : 2010-09-03
 *
 * 修改作者 : 邱俊涛
 * 修改内容 : 扩展内存管理模块接口
 * 修改日期 : 2010-09-06
 *
 */
#ifndef _UT_MEM_H_
#define _UT_MEM_H_

/**
 * 内存块数据结构
 */
typedef struct{
	unsigned char *memory; /* 内存块内容 */
	size_t size;  /* 内存块长度 */
}mem_block;

/**
 * 说明：初始化内存块(mem_block结构指针)
 *
 * 参数：
 * block : 内存块指针
 *
 * 返回值：
 * 无
 *
 */
void mem_block_init(mem_block *block);

/**
 * 说明：释放内存块(mem_block结构指针)
 *
 * 参数：
 * block : 内存块指针
 *
 * 返回值：
 * 无
 *
 */
void mem_block_free(mem_block *block);

/**
 * 说明：重新分配内存块大小
 *
 * 参数：
 * ptr  : 原始指针
 * size : 新的块大小
 *
 * 返回值：
 * void * : 分配成功
 * NULL   : 分配失败, 详细错误可查看errno
 */
void *mem_block_realloc(void *ptr, size_t size);

/**
 * 说明：根据block克隆一个新的mem_block
 *
 * 参数：
 * block  : 原始内存块
 *
 * 返回值：
 * mem_block * : 克隆成功
 * NULL        : 克隆失败, 详细错误可查看errno
 */
mem_block *mem_block_dup(const mem_block *block);

/**
 * 说明：根据memory和length来初始化block
 *
 * 参数：
 * block  : 原始内存块
 * memory : 数据块 
 * length : 数据块长度
 *
 * 返回值：
 * 无
 */
void mem_block_new(mem_block *block, char *memory, unsigned int length);

#endif