
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PALLOC_H_INCLUDED_
#define _NGX_PALLOC_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)   //��x86��ϵ�ṹ�£���ֵһ��Ϊ4096B����4K 

#define NGX_DEFAULT_POOL_SIZE    (16 * 1024)

#define NGX_POOL_ALIGNMENT       16
#define NGX_MIN_POOL_SIZE                                                     \
    ngx_align((sizeof(ngx_pool_t) + 2 * sizeof(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


typedef void (*ngx_pool_cleanup_pt)(void *data);

typedef struct ngx_pool_cleanup_s  ngx_pool_cleanup_t;

//Nginx�ڴ��֧��ͨ���ص����������ⲿ��Դ������
// ngx_pool_cleanup_t�ǻص������ṹ�壬�����ڴ������������ʽ���棬���ڴ�ؽ�������ʱ��ѭ��������Щ�ص����������ݽ�������
struct ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler; //�ǻص�����ָ�룻
    void                 *data;    //�ص�ʱ���������ݴ���ص�������
    ngx_pool_cleanup_t   *next;    //ָ����һ���ص������ṹ�壻
};


typedef struct ngx_pool_large_s  ngx_pool_large_t;

struct ngx_pool_large_s {
    ngx_pool_large_t     *next;
    void                 *alloc;
};

//ngx_pool_data_t���ڴ�����ݿ�ṹ��
typedef struct {
    u_char               *last;    //��һ��unsigned char ���͵�ָ�룬�������/��ǰ�ڴ�ط��䵽ĩλ��ַ������һ�η���Ӵ˴���ʼ
    u_char               *end;     //�ڴ�ؽ���λ�ã�
    ngx_pool_t           *next;    //�ڴ�������кܶ���ڴ棬��Щ�ڴ�����ͨ����ָ����������ģ�nextָ����һ���ڴ�
    ngx_uint_t            failed;  //�ڴ�ط���ʧ�ܴ�����
} ngx_pool_data_t;

//ngx_pool_s���ڴ��ͷ���ṹ��
struct ngx_pool_s {
    ngx_pool_data_t       d;           //�ڴ�ص����ݿ飻
    size_t                max;         //�ڴ�����ݿ�����ֵ��
    ngx_pool_t           *current;     //ָ��ǰ�ڴ�أ�
    ngx_chain_t          *chain;       //��ָ��ҽ�һ��ngx_chain_t�ṹ��
    ngx_pool_large_t     *large;       //����ڴ�����������ռ䳬��max�����ʹ�ã�
    ngx_pool_cleanup_t   *cleanup;     //�ͷ��ڴ�ص�callback
    ngx_log_t            *log;         //��־��Ϣ
};


typedef struct {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *log;
} ngx_pool_cleanup_file_t;


void *ngx_alloc(size_t size, ngx_log_t *log);
void *ngx_calloc(size_t size, ngx_log_t *log);

//�����ڴ��
ngx_pool_t *ngx_create_pool(size_t size, ngx_log_t *log);
//�����ڴ��
void ngx_destroy_pool(ngx_pool_t *pool);
//�����ڴ��
void ngx_reset_pool(ngx_pool_t *pool);

//�ڴ����루���룩
void *ngx_palloc(ngx_pool_t *pool, size_t size);
//�ڴ����루�����룩
void *ngx_pnalloc(ngx_pool_t *pool, size_t size);
void *ngx_pcalloc(ngx_pool_t *pool, size_t size);
void *ngx_prealloc(ngx_pool_t *pool, void *p, size_t old_size, size_t new_size);
void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
//�ڴ����
ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p);


ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);
void ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
void ngx_pool_cleanup_file(void *data);
void ngx_pool_delete_file(void *data);


#endif /* _NGX_PALLOC_H_INCLUDED_ */
