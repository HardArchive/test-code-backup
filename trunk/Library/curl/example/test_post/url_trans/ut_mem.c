/*
 * 文件名称 : ut_mem.c
 * 文件作用 : URL传输内存管理模块实现
 *
 * 文件作者 : 邱俊涛
 * 创建日期 : 2010-09-03
 *
 * 修改作者 : 邱俊涛
 * 修改内容 : 添加内存块克隆，内存块初始化
 * 修改日期 : 2010-09-06
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "ut_mem.h"

void mem_block_init(mem_block *block){
	block->memory = NULL;
	block->size = 0;
}

void mem_block_free(mem_block *block){
	if(block != NULL && block->memory){
		free(block->memory);
	}
}

void *mem_block_realloc(void *ptr, size_t size){
	if(ptr != NULL){
		return realloc(ptr, size);
	}else{
		return malloc(size);
	}
}

mem_block *mem_block_dup(const mem_block *block){
	mem_block *node;

	if(block == NULL){
		return NULL;
	}

	node = (mem_block *)malloc(sizeof(mem_block));
	node->memory = (char *)malloc(block->size);
	
	if(node->memory == NULL){
		return NULL;
	}
	
	memcpy(node->memory, block->memory, block->size);
	node->size = block->size;

	return node;
}

void mem_block_new(mem_block *block, char *memory, unsigned int length){
	if(block == NULL){
		return;
	}

	if(memory == NULL || length <= 0){
		return;
	}
	block->memory = 
		mem_block_realloc(block->memory, block->size+strlen(memory)+1);
	strncpy(block->memory, memory, length);
	block->size = length;
}
/*
// unit test only
int main(int argc, char *argv[]){
	return 0;
}
*/