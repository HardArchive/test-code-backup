/*
 * 文件名称 : ut_ios.h
 * 文件作用 : URL传输输入/输出模块接口定义
 *
 * 文件作者 : 邱俊涛
 * 创建日期 : 2010-09-03
 *
 * 修改作者 : 邱俊涛
 * 修改内容 : 扩展输入/输出模块接口
 * 修改日期 : 2010-09-06
 *
 * 修改作者 : 邱俊涛
 * 修改内容 : 添加raw_send_recv接口
 * 修改日期 : 2010-09-07
 * 
 * 修改作者 : 邱俊涛
 * 修改内容 : 添加错误信息接口
 * 修改日期 : 2010-09-07
 *
 * 修改作者 : 邱俊涛
 * 修改内容 : 添加收发文件接口
 * 修改日期 : 2010-09-08
 */
#ifndef _UT_IOS_H_
#define _UT_IOS_H_

#include "ut_mem.h"

/**
 * 文件描述符类型：
 * FD_SEND : 用以发送数据的文件描述符
 * FS_RECV : 用于接收数据的文件描述符
 */
typedef enum fd_type{
	FD_SEND, /* 0-发送 */
	FD_RECV  /* 1-接收 */
};

#define ONE_MINUTE 60000L
#define BLOCK_SIZE 1024
#define UT_ERROR_LEN 1024

typedef enum ut_status{
	UT_OK,    /* 正常 */
	UT_ERROR  /* 错误 */
};

/**
 * 获取最近的一条错误信息，如果发生错误后，没有及时获取错误信息，该
 * 信息可能被下一个可能出错的调用的错误信息覆盖
 */
const char *get_last_error(void);

/**
 * 为UI提供的处理进度回调接口
 */
typedef void (*processing)(double current, double total);

/**
 * 说明：
 * 向url发送长度为req_len的req_buf，并接受数据至缓冲区res_buf, 长度
 * 为res_len, res_buf即res_len可以被服务端改写。
 *
 * 参数：
 * url     : 需要读取的URL(统一资源描述)
 * req_buf : 请求数据缓冲区
 * req_len : 请求数据缓冲区长度
 * res_buf : 响应数据缓冲区指针
 * res_len : 响应数据缓冲区长度指针
 * 
 * 返回值：
 * -1     : 操作失败
 * 0      : 操作成功
 */
int raw_send_recv(const char *url, void *req_buf, unsigned int req_len, \
				  void **res_buf, unsigned int *res_len);

/**
 * 说明：
 * 打开长度为file_size的文件file_name,发送至url指定的位置, 通过调用
 * processing更新进度
 *
 */
int raw_send_file(const char *url, const char *file_name, \
				  const unsigned int file_size, processing proc);

/**
 * 说明：
 * 从url指定的位置读文件，并存入文件file_name, 通过调用processing更新进度
 *
 */
int raw_recv_file(const char *url, const char *file_name, \
				  const unsigned int file_size, processing proc);

/**
 * 说明：
 * 从url上读取定常(n)个字符，将内容存入buffer, length标识实际读到的长度.
 *
 * 参数：
 * url    : 需要读取的URL(统一资源描述)
 * buffer : 读入缓冲区
 * length : 读入缓冲区长度
 * 
 * 返回值：
 * -1     : 读取失败
 * length : 读取成功
 */
int raw_read_n( const char *url, void *buffer, unsigned int length);

/**
 * 说明：
 * 从url上读取不定长的内容到buffer,如果buffer较小，则url_write_u负责分配新的
 * 内存并同时修改length值长度，内存由调用者负责释放
 *
 * 参数：
 * url      : 需要读取的URL(统一资源描述)
 * p_buffer : 读入缓冲区的指针
 * p_length : 读入缓冲区长度的指针
 * proc   : 为更新UI而定义的回调函数
 *
 * 返回值：
 *  -1    : 读取失败
 * !-1    : 读取成功
 */
int raw_read_u(const char *url, void **p_buffer, unsigned int *p_length, \
			   processing proc);

/**
 * 说明：
 * 向url上写入定长(length)个字符，写入内容在buffer内, length标识buffer长度.
 *
 * 参数：
 * url    : 需要写入的URL(统一资源描述)
 * buffer : 写入缓冲区
 * length : 写入缓冲区长度
 * 
 * 返回值：
 * -1     : 写入失败
 * length : 写入成功
 */
int raw_write_n(const char *url, void *buffer, unsigned int length);

/**
 * 说明：
 * 向URL写入定长字符串，写入的内容存储在buffer中，length为buffer的长度
 * raw_write_u与raw_write_n的区别为：raw_write_u的内部可能将buffer分
 * 多次，多块传输。
 *
 * 参数：
 * url    : 需要写入的URL(统一资源描述)
 * buffer : 写入缓冲区
 * length : 写入缓冲区长度
 * proc   : 为更新UI而定义的回调函数
 * 
 * 返回值：
 * -1     : 写入失败
 * length : 写入成功
 */
int raw_write_u(const char *url, void *buffer, unsigned int length, \
				processing proc);

/**
 * 说明：
 * 将url指定的文件读入内存块block
 *
 * 参数：
 * url   : 需要读取的URL(统一资源描述)
 * block : 内存块指针
 *
 * 返回值：
 * -1     : 读取失败
 * length : 读取成功，长度为block的size
 * 
 */
int url_read_mem(const char *url, mem_block *block);

/**
 * 说明：
 * 将block指定内存块写入url
 *
 * 参数：
 * url   : 需要写入的URL(统一资源描述)
 * block : 内存块指针
 *
 * 返回值：
 * -1     : 写入失败
 * length : 写入成功，长度为block的size
 */
int url_write_mem(const char *url, const mem_block *block);

#endif