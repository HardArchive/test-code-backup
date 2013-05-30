
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_ALLOC_H_INCLUDED_
#define _NGX_ALLOC_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>

//ngx_alloc：（只是对malloc进行了简单的封装）
void *ngx_alloc(size_t size, ngx_log_t *log);
//ngx_calloc：（调用malloc并初始化为0）
void *ngx_calloc(size_t size, ngx_log_t *log);

#define ngx_free          free
#define ngx_memalign(alignment, size, log)  ngx_alloc(size, log)

extern ngx_uint_t  ngx_pagesize;
extern ngx_uint_t  ngx_pagesize_shift;
extern ngx_uint_t  ngx_cacheline_size;


#endif /* _NGX_ALLOC_H_INCLUDED_ */
