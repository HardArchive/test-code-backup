
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


static void *ngx_palloc_block(ngx_pool_t *pool, size_t size);
static void *ngx_palloc_large(ngx_pool_t *pool, size_t size);

//ngx_create_pool()负责创建内存池，动作序列如下：
//
//向系统请求size参数指定大小的储备内存块（按NGX_POOL_ALIGNMENT对齐，默认16字节），逻辑上划分为（ngx_pool_t结构体＋剩余内存区）两部分；
//初始化ngx_pool_data_t结构体各个字段（参考前一节，last字段指向初始分配地址）；
//计算单次分配的内存块长度上限（max字段，最大不超过size参数值或NGX_MAX_ALLOC_FROM_POOL）；
//初始化其他字段（current字段指向自己）。

ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    if (p == NULL) {
        return NULL;
    }

    p->d.last = (u_char *) p + sizeof(ngx_pool_t);                         //初始状态：last指向ngx_pool_t结构体之后数据取起始位置
    p->d.end = (u_char *) p + size;                                        //end指向分配的整个size大小的内存的末尾
    p->d.next = NULL;
    p->d.failed = 0;

	//#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)
	//内存池最大不超过4095，x86中页的大小为4K
    size = size - sizeof(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = NULL;
    p->large = NULL;
    p->cleanup = NULL;
    p->log = log;

    return p;
}

//ngx_destroy_pool()负责销毁内存池，动作序列如下：
//
//调用各个清理回调函数；
//释放各个ngx_pool_large_t结构体管理的储备内存块；
//释放各个ngx_pool_data_t结构体管理的独立内存块。

void
ngx_destroy_pool(ngx_pool_t *pool)
{
    ngx_pool_t          *p, *n;
    ngx_pool_large_t    *l;
    ngx_pool_cleanup_t  *c;

    for (c = pool->cleanup; c; c = c->next) {
        if (c->handler) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "run cleanup: %p", c);
            c->handler(c->data);
        }
    }

    for (l = pool->large; l; l = l->next) {

        ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0, "free: %p", l->alloc);

        if (l->alloc) {
            ngx_free(l->alloc);
        }
    }

#if (NGX_DEBUG)

    /*
     * we could allocate the pool->log from this pool
     * so we cannot use this log while free()ing the pool
     */

    for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                       "free: %p, unused: %uz", p, p->d.end - p->d.last);

        if (n == NULL) {
            break;
        }
    }

#endif

    for (p = pool, n = pool->d.next; /* void */; p = n, n = n->d.next) {
        ngx_free(p);

        if (n == NULL) {
            break;
        }
    }
}
//ngx_reset_pool()负责释放零碎内存块，动作序列如下：
//
//释放各个ngx_pool_large_t结构体管理的内存块；
//简单地将各个ngx_pool_data_t结构体的last字段复原成初始分配地址，而不是释放。

//内存池重置
void
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

	 //释放所有大块内存
    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    pool->large = NULL;

	// 重置所有小块内存区  
    for (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + sizeof(ngx_pool_t);
    }
}

//ngx_palloc()负责分配零碎内存块，动作序列如下：
//
//若size参数大于max字段值，转发给ngx_palloc_large()处理（调用更底层的分配函数）；
//以current字段所指ngx_pool_data_t结构体为起点，在单向链表中搜索第一个能执行分配动作的结构体，完成分配（分配首址按NGX_ALIGNMENT对齐）；
//若以上动作均告失败，转发给ngx_palloc_block()处理（先分配新的ngx_pool_data_t结构体，再分配零碎内存块）。

void *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    ngx_pool_t  *p;

    if (size <= pool->max) {//如果申请的内存大小大于内存池的max值，则走另一条路，申请大内存

        p = pool->current;

        do {
            m = ngx_align_ptr(p->d.last, NGX_ALIGNMENT);//对内存地址进行对齐处理

            if ((size_t) (p->d.end - m) >= size) {//如果在当前内存块有效范围内，进行内存指针的移动
                p->d.last = m + size;

                return m;
            }

            p = p->d.next;//如果当前内存块有效容量不够分配，则移动到下一个内存块进行分配

        } while (p);

        return ngx_palloc_block(pool, size);
    }

    return ngx_palloc_large(pool, size);
}

//ngx_palloc()负责分配零碎内存块，动作序列如下：
//若size参数大于max字段值，转发给ngx_palloc_large()处理（调用更底层的分配函数）；
//以current字段所指ngx_pool_data_t结构体为起点，在单向链表中搜索第一个能执行分配动作的结构体，完成分配（分配首址按NGX_ALIGNMENT对齐）；
//若以上动作均告失败，转发给ngx_palloc_block()处理（先分配新的ngx_pool_data_t结构体，再分配零碎内存块）。
//
//ngx_pnalloc()动作与ngx_palloc()类似，但不对齐分配首址。
void *
ngx_pnalloc(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    ngx_pool_t  *p;

    if (size <= pool->max) {

        p = pool->current;

        do {
            m = p->d.last;

            if ((size_t) (p->d.end - m) >= size) {
                p->d.last = m + size;

                return m;
            }

            p = p->d.next;

        } while (p);

        return ngx_palloc_block(pool, size);//进入大内存块的分配流程
    }

    return ngx_palloc_large(pool, size);
}

//ngx_palloc_block()负责向系统请求新的储备内存块，最终完成分配，动作序列如下：
//
//向系统请求指定大小的储备内存块（按NGX_POOL_ALIGNMENT对齐，默认16字节），逻辑上划分为（ngx_pool_data_t结构体＋剩余内存区）两部分；
//初始化各个字段（包括分配首址，按NGX_ALIGNMENT对齐）；
//调整current字段的指向（实际上维持着一个“... 5 5 5 4 3 2 1 0 0”的failed字段值序列），将新的ngx_pool_data_t结构体挂入单向链表中。

//这个函数是用来分配新的内存块，为pool内存池开辟一个新的内存块，并申请使用size大小的内存
static void *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new, *current;

    psize = (size_t) (pool->d.end - (u_char *) pool);//计算内存池第一个内存块的大小

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);//分配和第一个内存块同样大小的内存块
    if (m == NULL) {
        return NULL;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;//设置新内存块的end
    new->d.next = NULL;
    new->d.failed = 0;

    m += sizeof(ngx_pool_data_t);//将指针m移动到d后面的一个位置，作为起始位置
    m = ngx_align_ptr(m, NGX_ALIGNMENT);//对m指针按4字节对齐处理
    new->d.last = m + size;;//设置新内存块的last，即申请使用size大小的内存

    current = pool->current;
	//这里的循环用来找最后一个链表节点，这里failed用来控制循环的长度，如果分配失败次数达到5次，
	//就忽略，不需要每次都从头找起
    for (p = current; p->d.next; p = p->d.next) {
        if (p->d.failed++ > 4) {
            current = p->d.next;
        }
    }

    p->d.next = new;

    pool->current = current ? current : new;

    return m;
}

//ngx_palloc_large(pool, size)/ngx_pmemalign(pool, size, alignment)
//
//ngx_palloc_large负责分配不归第一层结构管理的独立内存块，动作序列如下：
//
//调用ngx_alloc()分配新的独立内存块；
//搜索第一个空闲的ngx_pool_large_t结构体，保存并返回独立内存块首址；
//若前一步失败，从本内存池中分配新的ngx_pool_large_t结构体并加入单向链表中，保存并返回独立内存块首址。
//
//ngx_pmemalign()动作与ngx_palloc_large()类似，但会按alignment参数对齐独立内存块首址。
//在ngx_palloc中首先会判断申请的内存大小是否超过内存块的最大限值，如果超过，则直接调用ngx_palloc_large，进入大内存块的分配流程
static void *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    void              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

	// 直接在系统堆中分配一块空间  
    p = ngx_alloc(size, pool->log);
    if (p == NULL) {
        return NULL;
    }

    n = 0;

	 // 查找到一个空的large区，如果有，则将刚才分配的空间交由它管理  
    for (large = pool->large; large; large = large->next) {
        if (large->alloc == NULL) {
            large->alloc = p;
            return p;
        }

        if (n++ > 3) {
            break;
        }
    }

	//为了提高效率， 如果在三次内没有找到空的large结构体，则创建一个
    large = ngx_palloc(pool, sizeof(ngx_pool_large_t));
    if (large == NULL) {
        ngx_free(p);
        return NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}


void *
ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment)
{
    void              *p;
    ngx_pool_large_t  *large;

    p = ngx_memalign(alignment, size, pool->log);
    if (p == NULL) {
        return NULL;
    }

    large = ngx_palloc(pool, sizeof(ngx_pool_large_t));
    if (large == NULL) {
        ngx_free(p);
        return NULL;
    }

    large->alloc = p;
    large->next = pool->large;
    pool->large = large;

    return p;
}

//ngx_pfree(pool, p)
//ngx_pfree()负责“释放”内存块，动作序列如下：
//如果待“释放”对象是独立内存块，则转调用ngx_free()释放。
//实际上该函数并不真正释放零碎内存块，而是尽可能将释放动作推迟到ngx_reset_pool()/ngx_destroy_pool()中。
//内存池清理
//Nginx内存池中大内存块和小内存块的分配与释放是不一样的。我们在使用内存池时，可以使用ngx_palloc进行分配，使用ngx_pfree释放。
// 而对于大内存，这样做是没有问题的，而对于小内存就不一样了，分配的小内存，不会进行释放。
// 因为大内存块的分配只对前3个内存块进行检查，否则就直接分配内存，所以大内存块的释放必须及时。
ngx_int_t
ngx_pfree(ngx_pool_t *pool, void *p)
{
    ngx_pool_large_t  *l;

	//只检查是否是大内存块，如果是大内存块则释放
    for (l = pool->large; l; l = l->next) {
        if (p == l->alloc) {
            ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, pool->log, 0,
                           "free: %p", l->alloc);
            ngx_free(l->alloc);
            l->alloc = NULL;

            return NGX_OK;
        }
    }

    return NGX_DECLINED;
}

//ngx_pcalloc()将分配请求转发给ngx_palloc()处理，并对返回的零碎内存块进行清零初始化。
void *
ngx_pcalloc(ngx_pool_t *pool, size_t size)
{
    void *p;

    p = ngx_palloc(pool, size);
    if (p) {
        ngx_memzero(p, size);
    }

    return p;
}


void *
ngx_prealloc(ngx_pool_t *pool, void *p, size_t old_size, size_t new_size)
{
    void *new;
    ngx_pool_t *node;

    if (p == NULL) {
        return ngx_palloc(pool, new_size);
    }

    if (new_size == 0) {
        if ((u_char *) p + old_size == pool->d.last) {
           pool->d.last = p;
        } else {
           ngx_pfree(pool, p);
        }

        return NULL;
    }
	
    if (old_size <= pool->max) {
        for (node = pool; node; node = node->d.next) {
            if ((u_char *)p + old_size == node->d.last
                && (u_char *)p + new_size <= node->d.end) {
                node->d.last = (u_char *)p + new_size;
                return p;
            }
        }		
    }
		
    if (new_size <= old_size) {
       return p;
    }
	
    new = ngx_palloc(pool, new_size);
    if (new == NULL) {
        return NULL;
    }

    ngx_memcpy(new, p, old_size);

    ngx_pfree(pool, p);

    return new;
}

//ngx_pool_cleanup_add()负责分配新的ngx_pool_cleanup_t结构体，以便调用端注册回调函数。动作序列如下：
//
//从本内存池中分配一个ngx_pool_cleanup_t结构体；
//若size参数不为0，尝试从本内存池分配相应大小的一块内存并作为回调参数；
//将新的结构体挂到单向链表中；
//调整cleanup字段，保持回调函数按“先进后出”的性质（类似于压栈操作）。

//如果我们需要添加自己的回调函数，则需要调用ngx_pool_cleanup_add来得到一个ngx_pool_cleanup_t，
// 然后设置handler为我们的清理函数，并设置data为我们要清理的数据。
// 这样在ngx_destroy_pool中会循环调用handler清理数据；
ngx_pool_cleanup_t *
ngx_pool_cleanup_add(ngx_pool_t *p, size_t size)
{
    ngx_pool_cleanup_t  *c;

    c = ngx_palloc(p, sizeof(ngx_pool_cleanup_t));
    if (c == NULL) {
        return NULL;
    }

    if (size) {
        c->data = ngx_palloc(p, size);
        if (c->data == NULL) {
            return NULL;
        }

    } else {
        c->data = NULL;
    }

    c->handler = NULL;
    c->next = p->cleanup;

    p->cleanup = c;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, p->log, 0, "add cleanup: %p", c);

    return c;
}

//ngx_pool_run_cleanup_file()负责搜索注册在回调函数链表中的、与fd参数指定文件句柄对应的回调函数并调用之。
void
ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd)
{
    ngx_pool_cleanup_t       *c;
    ngx_pool_cleanup_file_t  *cf;

    for (c = p->cleanup; c; c = c->next) {
        if (c->handler == ngx_pool_cleanup_file) {

            cf = c->data;

            if (cf->fd == fd) {
                c->handler(cf);
                c->handler = NULL;
                return;
            }
        }
    }
}

//ngx_pool_cleanup_file()提供一个关闭指定文件句柄的缺省实现函数。
void
ngx_pool_cleanup_file(void *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_log_debug1(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d",
                   c->fd);

    if (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}

//ngx_pool_delete_file()提供一个删除文件并关闭相关文件句柄的缺省实现函数。 
void
ngx_pool_delete_file(void *data)
{
    ngx_pool_cleanup_file_t  *c = data;

    ngx_err_t  err;

    ngx_log_debug2(NGX_LOG_DEBUG_ALLOC, c->log, 0, "file cleanup: fd:%d %s",
                   c->fd, c->name);

    if (ngx_delete_file(c->name) == NGX_FILE_ERROR) {
        err = ngx_errno;

        if (err != NGX_ENOENT) {
            ngx_log_error(NGX_LOG_CRIT, c->log, err,
                          ngx_delete_file_n " \"%s\" failed", c->name);
        }
    }

    if (ngx_close_file(c->fd) == NGX_FILE_ERROR) {
        ngx_log_error(NGX_LOG_ALERT, c->log, ngx_errno,
                      ngx_close_file_n " \"%s\" failed", c->name);
    }
}


#if 0

static void *
ngx_get_cached_block(size_t size)
{
    void                     *p;
    ngx_cached_block_slot_t  *slot;

    if (ngx_cycle->cache == NULL) {
        return NULL;
    }

    slot = &ngx_cycle->cache[(size + ngx_pagesize - 1) / ngx_pagesize];

    slot->tries++;

    if (slot->number) {
        p = slot->block;
        slot->block = slot->block->next;
        slot->number--;
        return p;
    }

    return NULL;
}

#endif
