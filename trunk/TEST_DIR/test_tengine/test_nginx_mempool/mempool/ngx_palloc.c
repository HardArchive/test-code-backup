
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


static void *ngx_palloc_block(ngx_pool_t *pool, size_t size);
static void *ngx_palloc_large(ngx_pool_t *pool, size_t size);

//ngx_create_pool()���𴴽��ڴ�أ������������£�
//
//��ϵͳ����size����ָ����С�Ĵ����ڴ�飨��NGX_POOL_ALIGNMENT���룬Ĭ��16�ֽڣ����߼��ϻ���Ϊ��ngx_pool_t�ṹ�士ʣ���ڴ����������֣�
//��ʼ��ngx_pool_data_t�ṹ������ֶΣ��ο�ǰһ�ڣ�last�ֶ�ָ���ʼ�����ַ����
//���㵥�η�����ڴ�鳤�����ޣ�max�ֶΣ���󲻳���size����ֵ��NGX_MAX_ALLOC_FROM_POOL����
//��ʼ�������ֶΣ�current�ֶ�ָ���Լ�����

ngx_pool_t *
ngx_create_pool(size_t size, ngx_log_t *log)
{
    ngx_pool_t  *p;

    p = ngx_memalign(NGX_POOL_ALIGNMENT, size, log);
    if (p == NULL) {
        return NULL;
    }

    p->d.last = (u_char *) p + sizeof(ngx_pool_t);                         //��ʼ״̬��lastָ��ngx_pool_t�ṹ��֮������ȡ��ʼλ��
    p->d.end = (u_char *) p + size;                                        //endָ����������size��С���ڴ��ĩβ
    p->d.next = NULL;
    p->d.failed = 0;

	//#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)
	//�ڴ����󲻳���4095��x86��ҳ�Ĵ�СΪ4K
    size = size - sizeof(ngx_pool_t);
    p->max = (size < NGX_MAX_ALLOC_FROM_POOL) ? size : NGX_MAX_ALLOC_FROM_POOL;

    p->current = p;
    p->chain = NULL;
    p->large = NULL;
    p->cleanup = NULL;
    p->log = log;

    return p;
}

//ngx_destroy_pool()���������ڴ�أ������������£�
//
//���ø�������ص�������
//�ͷŸ���ngx_pool_large_t�ṹ�����Ĵ����ڴ�飻
//�ͷŸ���ngx_pool_data_t�ṹ�����Ķ����ڴ�顣

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
//ngx_reset_pool()�����ͷ������ڴ�飬�����������£�
//
//�ͷŸ���ngx_pool_large_t�ṹ�������ڴ�飻
//�򵥵ؽ�����ngx_pool_data_t�ṹ���last�ֶθ�ԭ�ɳ�ʼ�����ַ���������ͷš�

//�ڴ������
void
ngx_reset_pool(ngx_pool_t *pool)
{
    ngx_pool_t        *p;
    ngx_pool_large_t  *l;

	 //�ͷ����д���ڴ�
    for (l = pool->large; l; l = l->next) {
        if (l->alloc) {
            ngx_free(l->alloc);
        }
    }

    pool->large = NULL;

	// ��������С���ڴ���  
    for (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *) p + sizeof(ngx_pool_t);
    }
}

//ngx_palloc()������������ڴ�飬�����������£�
//
//��size��������max�ֶ�ֵ��ת����ngx_palloc_large()�������ø��ײ�ķ��亯������
//��current�ֶ���ָngx_pool_data_t�ṹ��Ϊ��㣬�ڵ���������������һ����ִ�з��䶯���Ľṹ�壬��ɷ��䣨������ַ��NGX_ALIGNMENT���룩��
//�����϶�������ʧ�ܣ�ת����ngx_palloc_block()�����ȷ����µ�ngx_pool_data_t�ṹ�壬�ٷ��������ڴ�飩��

void *
ngx_palloc(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    ngx_pool_t  *p;

    if (size <= pool->max) {//���������ڴ��С�����ڴ�ص�maxֵ��������һ��·��������ڴ�

        p = pool->current;

        do {
            m = ngx_align_ptr(p->d.last, NGX_ALIGNMENT);//���ڴ��ַ���ж��봦��

            if ((size_t) (p->d.end - m) >= size) {//����ڵ�ǰ�ڴ����Ч��Χ�ڣ������ڴ�ָ����ƶ�
                p->d.last = m + size;

                return m;
            }

            p = p->d.next;//�����ǰ�ڴ����Ч�����������䣬���ƶ�����һ���ڴ����з���

        } while (p);

        return ngx_palloc_block(pool, size);
    }

    return ngx_palloc_large(pool, size);
}

//ngx_palloc()������������ڴ�飬�����������£�
//��size��������max�ֶ�ֵ��ת����ngx_palloc_large()�������ø��ײ�ķ��亯������
//��current�ֶ���ָngx_pool_data_t�ṹ��Ϊ��㣬�ڵ���������������һ����ִ�з��䶯���Ľṹ�壬��ɷ��䣨������ַ��NGX_ALIGNMENT���룩��
//�����϶�������ʧ�ܣ�ת����ngx_palloc_block()�����ȷ����µ�ngx_pool_data_t�ṹ�壬�ٷ��������ڴ�飩��
//
//ngx_pnalloc()������ngx_palloc()���ƣ��������������ַ��
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

        return ngx_palloc_block(pool, size);//������ڴ��ķ�������
    }

    return ngx_palloc_large(pool, size);
}

//ngx_palloc_block()������ϵͳ�����µĴ����ڴ�飬������ɷ��䣬�����������£�
//
//��ϵͳ����ָ����С�Ĵ����ڴ�飨��NGX_POOL_ALIGNMENT���룬Ĭ��16�ֽڣ����߼��ϻ���Ϊ��ngx_pool_data_t�ṹ�士ʣ���ڴ����������֣�
//��ʼ�������ֶΣ�����������ַ����NGX_ALIGNMENT���룩��
//����current�ֶε�ָ��ʵ����ά����һ����... 5 5 5 4 3 2 1 0 0����failed�ֶ�ֵ���У������µ�ngx_pool_data_t�ṹ����뵥�������С�

//������������������µ��ڴ�飬Ϊpool�ڴ�ؿ���һ���µ��ڴ�飬������ʹ��size��С���ڴ�
static void *
ngx_palloc_block(ngx_pool_t *pool, size_t size)
{
    u_char      *m;
    size_t       psize;
    ngx_pool_t  *p, *new, *current;

    psize = (size_t) (pool->d.end - (u_char *) pool);//�����ڴ�ص�һ���ڴ��Ĵ�С

    m = ngx_memalign(NGX_POOL_ALIGNMENT, psize, pool->log);//����͵�һ���ڴ��ͬ����С���ڴ��
    if (m == NULL) {
        return NULL;
    }

    new = (ngx_pool_t *) m;

    new->d.end = m + psize;//�������ڴ���end
    new->d.next = NULL;
    new->d.failed = 0;

    m += sizeof(ngx_pool_data_t);//��ָ��m�ƶ���d�����һ��λ�ã���Ϊ��ʼλ��
    m = ngx_align_ptr(m, NGX_ALIGNMENT);//��mָ�밴4�ֽڶ��봦��
    new->d.last = m + size;;//�������ڴ���last��������ʹ��size��С���ڴ�

    current = pool->current;
	//�����ѭ�����������һ������ڵ㣬����failed��������ѭ���ĳ��ȣ��������ʧ�ܴ����ﵽ5�Σ�
	//�ͺ��ԣ�����Ҫÿ�ζ���ͷ����
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
//ngx_palloc_large������䲻���һ��ṹ����Ķ����ڴ�飬�����������£�
//
//����ngx_alloc()�����µĶ����ڴ�飻
//������һ�����е�ngx_pool_large_t�ṹ�壬���沢���ض����ڴ����ַ��
//��ǰһ��ʧ�ܣ��ӱ��ڴ���з����µ�ngx_pool_large_t�ṹ�岢���뵥�������У����沢���ض����ڴ����ַ��
//
//ngx_pmemalign()������ngx_palloc_large()���ƣ����ᰴalignment������������ڴ����ַ��
//��ngx_palloc�����Ȼ��ж�������ڴ��С�Ƿ񳬹��ڴ��������ֵ�������������ֱ�ӵ���ngx_palloc_large��������ڴ��ķ�������
static void *
ngx_palloc_large(ngx_pool_t *pool, size_t size)
{
    void              *p;
    ngx_uint_t         n;
    ngx_pool_large_t  *large;

	// ֱ����ϵͳ���з���һ��ռ�  
    p = ngx_alloc(size, pool->log);
    if (p == NULL) {
        return NULL;
    }

    n = 0;

	 // ���ҵ�һ���յ�large��������У��򽫸ղŷ���Ŀռ佻��������  
    for (large = pool->large; large; large = large->next) {
        if (large->alloc == NULL) {
            large->alloc = p;
            return p;
        }

        if (n++ > 3) {
            break;
        }
    }

	//Ϊ�����Ч�ʣ� �����������û���ҵ��յ�large�ṹ�壬�򴴽�һ��
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
//ngx_pfree()�����ͷš��ڴ�飬�����������£�
//��������ͷš������Ƕ����ڴ�飬��ת����ngx_free()�ͷš�
//ʵ���ϸú������������ͷ������ڴ�飬���Ǿ����ܽ��ͷŶ����Ƴٵ�ngx_reset_pool()/ngx_destroy_pool()�С�
//�ڴ������
//Nginx�ڴ���д��ڴ���С�ڴ��ķ������ͷ��ǲ�һ���ġ�������ʹ���ڴ��ʱ������ʹ��ngx_palloc���з��䣬ʹ��ngx_pfree�ͷš�
// �����ڴ��ڴ棬��������û������ģ�������С�ڴ�Ͳ�һ���ˣ������С�ڴ棬��������ͷš�
// ��Ϊ���ڴ��ķ���ֻ��ǰ3���ڴ����м�飬�����ֱ�ӷ����ڴ棬���Դ��ڴ����ͷű��뼰ʱ��
ngx_int_t
ngx_pfree(ngx_pool_t *pool, void *p)
{
    ngx_pool_large_t  *l;

	//ֻ����Ƿ��Ǵ��ڴ�飬����Ǵ��ڴ�����ͷ�
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

//ngx_pcalloc()����������ת����ngx_palloc()�������Է��ص������ڴ����������ʼ����
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

//ngx_pool_cleanup_add()��������µ�ngx_pool_cleanup_t�ṹ�壬�Ա���ö�ע��ص������������������£�
//
//�ӱ��ڴ���з���һ��ngx_pool_cleanup_t�ṹ�壻
//��size������Ϊ0�����Դӱ��ڴ�ط�����Ӧ��С��һ���ڴ沢��Ϊ�ص�������
//���µĽṹ��ҵ����������У�
//����cleanup�ֶΣ����ֻص����������Ƚ�����������ʣ�������ѹջ��������

//���������Ҫ����Լ��Ļص�����������Ҫ����ngx_pool_cleanup_add���õ�һ��ngx_pool_cleanup_t��
// Ȼ������handlerΪ���ǵ���������������dataΪ����Ҫ��������ݡ�
// ������ngx_destroy_pool�л�ѭ������handler�������ݣ�
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

//ngx_pool_run_cleanup_file()��������ע���ڻص����������еġ���fd����ָ���ļ������Ӧ�Ļص�����������֮��
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

//ngx_pool_cleanup_file()�ṩһ���ر�ָ���ļ������ȱʡʵ�ֺ�����
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

//ngx_pool_delete_file()�ṩһ��ɾ���ļ����ر�����ļ������ȱʡʵ�ֺ����� 
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
