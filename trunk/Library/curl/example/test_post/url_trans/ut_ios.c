/*
 * 文件名称 : ut_ios.c
 * 文件作用 : URL传输输入/输出模块实现
 *
 * 文件作者 : 邱俊涛
 * 创建日期 : 2010-09-03
 *
 * 修改作者 : 邱俊涛
 * 修改内容 : 对raw_write_n/raw_read_n的实现
 * 修改日期 : 2010-09-03
 *
 * 修改作者 : 邱俊涛
 * 修改内容 : 对url_read_men/url_write_mem的实现
 * 修改日期 : 2010-09-06
 *
 * 修改作者 : 邱俊涛
 * 修改内容 : 添加raw_send_recv接口的实现
 * 修改日期 : 2010-09-07
 *
 * 修改作者 : 邱俊涛
 * 修改内容 : 添加错误信息接口实现
 * 修改日期 : 2010-09-07
 *
 * 修改作者 : 邱俊涛
 * 修改内容 : 添加收发文件接口实现
 * 修改日期 : 2010-09-08 
 *
 */
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "ut_ios.h"
#include "ut_mem.h"

static char ut_error_message[UT_ERROR_LEN];
static int ut_status = UT_OK;

/**
 * 获取最近一次发生的错误
 */
const char *get_last_error(void){
	if(ut_status == UT_OK){
		return NULL;
	}
	return ut_error_message;
}

/* 
 * 说明：为for_recv类型的socket等待timeout_ms毫秒，指导读/写端可用
 *
 * 参数：
 * sockfd     : 通信信道的远端socket
 * for_recv   : 通信类型，为读/写打开的socket
 * timeout_ms : 单位为毫秒的超时时间
 * 
 * 返回值：
 * -1     : 失败(超时或select调用失败)
 * number : 标记过的socket数目
 * 
 */
static int wait_on_socket(int sockfd, int for_recv, long timeout_ms){
	struct timeval tv;
	fd_set infd, outfd, errfd;
	int res;

	tv.tv_sec = timeout_ms / 1000;
	tv.tv_usec= (timeout_ms % 1000) * 1000;

	FD_ZERO(&infd);
	FD_ZERO(&outfd);
	FD_ZERO(&errfd);

	FD_SET(sockfd, &errfd); /* always check for error */

	if(for_recv){
		FD_SET(sockfd, &infd);
	}else{
		FD_SET(sockfd, &outfd);
	}

	/* select() returns the number of signalled sockets or -1 */
	res = select(sockfd + 1, &infd, &outfd, &errfd, &tv);
	return res;
}

/*
 * 打印错误信息到错误缓冲区(ut_error_message)，私有函数
 */
static int on_error(const char *format, ...){
	int bytes_written;
	va_list arg_ptr;
	
	va_start(arg_ptr, format);
	ut_status = UT_ERROR;

#ifdef DEBUG_MODE
	bytes_written = vfprintf(stderr, format, arg_ptr);
#else
	bytes_written = vsnprintf(ut_error_message, UT_ERROR_LEN, format, arg_ptr);
#endif
	
	va_end(arg_ptr);
	
	return bytes_written;
}

/**
 * 发送req_buf,然后将回馈信息回填如res_buf,具体接口描述参看ut_ios.h
 */
int raw_send_recv(const char *url, void *req_buf, unsigned int req_len, \
				  void **res_buf, unsigned int *res_len){
	CURL *curl;
	CURLcode res;

	int socket;
	int offset;

	size_t ret_len;
	size_t new_size;
	
	char *temp = (char *)malloc(sizeof(char)*1024);
	char *term = (char *)malloc(sizeof(char)*1);

	curl = curl_easy_init();

	if(curl == NULL){
		on_error("raw_send_recv : 初始化CURL失败\n");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		on_error("raw_send_recv : 启动CURL失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &socket);

	if(res != CURLE_OK){
		on_error("raw_send_recv : 获取套接字失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	/* wait for the socket to become ready for sending */
	if(!wait_on_socket(socket, FD_SEND, ONE_MINUTE)){
		on_error("raw_send_recv : 套接字连接超时(发送时)\n");
		return -1;
	}

	res = curl_easy_send(curl, req_buf, req_len, &ret_len);
	
	if(res != CURLE_OK){
		on_error("raw_send_recv : 传输数据失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}
	
	if(req_len != ret_len){
		on_error("raw_send_recv : 数据未发送完成，剩余：%d\n", \
			(req_len - ret_len));
		return -1;
	}

	new_size = 0;
	
	for(;;){
		if(!wait_on_socket(socket, FD_RECV, ONE_MINUTE)){
			on_error("raw_send_recv : 套接字连接超时(接受时)\n");
			return -1;
		}

		memset(temp, '\0', 1024);
		res = curl_easy_recv(curl, temp, 1024, &ret_len);
		
		if(res != CURLE_OK){
			break;
		}

		new_size += ret_len;

		if(new_size > *res_len){
			*res_buf = realloc(*res_buf, new_size);
		}

		if(*res_buf == NULL){
			on_error("raw_send_recv : 为写扩展缓存出错 : %s\n", \
				new_size, strerror(errno));
			break;
		}
		
		offset = new_size - ret_len;
		memcpy((char*)*res_buf+offset, temp, ret_len);

		*res_len = new_size;
	}
	//terminal character
	memset(term, '\0', 1);
	memcpy((char*)*res_buf+*res_len, term, 1);
	curl_easy_cleanup(curl);
	
	ut_status = UT_OK;
	return 0;	
}

/**
 * 说明：
 * 打开长度为file_size的文件file_name,发送至url指定的位置, 通过调用
 * processing更新进度
 *
 */
int raw_send_file(const char *url, const char *file_name, \
				  const unsigned int file_size, processing proc){
	CURL *curl;
	CURLcode res;

	char *buffer;
	int len = 1024;
	int bytes_send, size;
	FILE *fp;

	int socket;
	size_t ret_len;

	curl = curl_easy_init();

	if(curl == NULL){
		on_error("raw_send_file : 初始化CURL失败\n");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		on_error("raw_send_file : 启动CURL失败，错误信息：%s\n", curl_easy_strerror(res));
		return -1;
	}

	res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &socket);

	if(res != CURLE_OK){
		on_error("raw_send_file : 获取套接字失败，错误信息：%s\n", curl_easy_strerror(res));
		return -1;
	}

	buffer = (char *)malloc(sizeof(char) * len);
	memset(buffer, '\0', len);

	fp = fopen(file_name, "rb");
	
	if(fp == NULL){
		on_error("raw_send_file : 为读打开文件失败：%s\n", strerror(errno));
		return -1;
	}
	
	size = file_size;
	
	bytes_send = 0;

	while(size > 0){
		if(!wait_on_socket(socket, FD_SEND, ONE_MINUTE)){
			on_error("raw_send_file : 套接字连接超时\n");
			fclose(fp);
			free(buffer);
			return -1;
		}
		
		if(size >= 1024){
			fread(buffer, 1024, 1, fp);
			res = curl_easy_send(curl, buffer, 1024, &ret_len);
		}else{
			fread(buffer, size, 1, fp);
			res = curl_easy_send(curl, buffer, size, &ret_len);
		}

		if(res != CURLE_OK){
			on_error("raw_send_file : 传输数据失败，错误信息：%s\n", \
				curl_easy_strerror(res));
			fclose(fp);
			free(buffer);
			return -1;
		}
				
		if(ret_len != 1024 && ret_len != size){
			on_error("raw_send_file : 数据未发送完成，剩余：%d\n", \
				(size - ret_len));
			fclose(fp);
			free(buffer);
			return -1;
		}

		bytes_send += ret_len;
		if(proc != NULL){
			proc((double)bytes_send, (double)file_size);
		}
		size -= 1024;

	}
	
	fclose(fp);
	free(buffer);

	curl_easy_cleanup(curl);
	
	return 0;
}

/**
 * 说明：
 * 从url指定的位置读文件，并存入文件file_name, 通过调用processing更新进度
 *
 */
int raw_recv_file(const char *url, const char *file_name, \
				  const unsigned int file_size, processing proc){
	CURL *curl;
	CURLcode res;

	int socket;
	int offset;

	char *buffer;
	int len = 1024;
	int bytes_recv, size;
	FILE *fp;

	size_t ret_len;

	curl = curl_easy_init();

	if(curl == NULL){
		on_error("raw_recv_file : 初始化CURL失败\n");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		on_error("raw_recv_file : 启动CURL失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &socket);

	if(res != CURLE_OK){
		on_error("raw_recv_file : 获取套接字失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	fp = fopen(file_name, "wb");
	
	if(fp == NULL){
		on_error("raw_recv_file : 打开文件失败，错误信息：%s\n", strerror(errno));
		return -1;
	}

	buffer = (char *)malloc(sizeof(char) * len);
	memset(buffer, '\0', len);

	bytes_recv = 0;
	size = file_size;

	while(size > 0){
		if(!wait_on_socket(socket, FD_RECV, ONE_MINUTE)){
			on_error("raw_recv_file : 套接字连接超时\n");
			fclose(fp);
			free(buffer);
			return -1;
		}

		if(size >= 1024){
			res = curl_easy_recv(curl, buffer, 1024, &ret_len);
			fwrite(buffer, 1024, 1, fp);
		}else{
			res = curl_easy_recv(curl, buffer, size, &ret_len);
			fwrite(buffer, size, 1, fp);
		}

		if(res != CURLE_OK){
			if(res == CURLE_AGAIN){
				//reach file end
				break;
			}else{
				//real error occured
				on_error("raw_recv_file : 接收文件出错，错误信息 ：%s\n", \
					curl_easy_strerror(res));
				fclose(fp);
				free(buffer);
				return -1;			
			}
		}

		bytes_recv += ret_len;
		if(proc != NULL){
			proc(bytes_recv, file_size);
		}

		size -= 1024;
	}

	curl_easy_cleanup(curl);
	fclose(fp);
	free(buffer);

	return 0;	
}

/**
 * 向URL写定长数据，具体接口描述参看ut_ios.h
 */
int raw_write_n(const char *url, void *buffer, unsigned int length){
	CURL *curl;
	CURLcode res;

	int socket;
	size_t ret_len;

	curl = curl_easy_init();

	if(curl == NULL){
		on_error("url_write_n : 初始化CURL失败\n");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		on_error("url_write_n : 启动CURL失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &socket);

	if(res != CURLE_OK){
		on_error("url_write_n : 获取套接字失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	/* wait for the socket to become ready for sending */
	if(!wait_on_socket(socket, FD_SEND, ONE_MINUTE)){
		on_error("url_write_n : 套接字连接超时\n");
		return -1;
	}

	res = curl_easy_send(curl, buffer, length, &ret_len);

	if(res != CURLE_OK){
		on_error("url_write_n : 传输数据失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}
	
	if(length != ret_len){
		on_error("url_write_n : 数据未发送完成，剩余：%d\n", \
			(length - ret_len));
		return -1;
	}

	curl_easy_cleanup(curl);
	
	ut_status = UT_OK;
	return ret_len;
}

/**
 * 写入定长数据块，但是可能根据需要将数据分块发送，具体接口描述参看ut_ios.h
 */
int raw_write_u(const char *url, void *buffer, unsigned int length, \
				processing proc){
	CURL *curl;
	CURLcode res;

	int socket;
	size_t ret_len;
	size_t offset, left, diff;
	char *temp;

	curl = curl_easy_init();

	if(curl == NULL){
		on_error("raw_write_u : 初始化CURL失败\n");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		on_error("raw_write_u : 启动CURL失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &socket);

	if(res != CURLE_OK){
		on_error("raw_write_u : 获取套接字失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	/* wait for the socket to become ready for sending */
	if(!wait_on_socket(socket, FD_SEND, ONE_MINUTE)){
		on_error("raw_write_u : 套接字连接超时\n");
		return -1;
	}
	
	offset = 0;
	left = BLOCK_SIZE;
	for(;;){
		/* wait for the socket to become ready for sending */
		if(!wait_on_socket(socket, FD_SEND, ONE_MINUTE)){
			on_error("raw_write_u : 套接字连接超时\n");
			return -1;
		}
		temp = (char *)malloc(sizeof(char) * BLOCK_SIZE);
		memcpy(temp, (char *)buffer+offset, left);
		
		res = curl_easy_send(curl, temp, left, &ret_len);

		//res = curl_easy_send(curl, buffer, length, &ret_len);

		if(res != CURLE_OK){
			on_error("raw_write_u : 传输数据失败，错误信息：%s\n", \
				curl_easy_strerror(res));
			break;
		}
				
		if(left != ret_len){
			on_error("raw_write_u : 数据未发送完成，剩余：%d\n", \
				(length - ret_len));
			return -1;
		}

		offset += ret_len;
		diff = length - offset;
		left = diff > BLOCK_SIZE ? BLOCK_SIZE : diff;
		free(temp);

		if(proc != NULL){
			proc(offset, length);
		}

		if(left == 0){
			break;
		}
	}


	curl_easy_cleanup(curl);
	
	ut_status = UT_OK;

	return ret_len;
}

/**
 * 从URL读定长数据，具体接口描述参看ut_ios.h
 */
int raw_read_n(const char *url, void *buffer, unsigned int length){
	CURL *curl;
	CURLcode res;

	int socket;
	size_t ret_len;

	curl = curl_easy_init();

	if(curl == NULL){
		on_error("url_read_n : 初始化CURL失败\n");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		on_error("url_read_n : 启动CURL失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &socket);

	if(res != CURLE_OK){
		on_error("url_read_n : 获取套接字失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	/* wait for the socket to become ready for sending */
	if(!wait_on_socket(socket, FD_RECV, ONE_MINUTE)){
		on_error("url_read_n : 套接字连接超时\n");
		return -1;
	}

	res = curl_easy_recv(curl, buffer, length, &ret_len);

	
	if(res != CURLE_OK){
		on_error("url_read_n : 传输数据失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	curl_easy_cleanup(curl);

	ut_status = UT_OK;
	
	return ret_len;	
}

/**
 * 从url读取非定常字符串，返回值存入*buffer, 内存需要调用者释放，具体接口描述参看ut_ios.h
 */
int raw_read_u(const char *url, void **buffer, unsigned int *length, \
			   processing proc){
	CURL *curl;
	CURLcode res;

	int socket;
	int offset;

	size_t ret_len;
	size_t new_size;

	char *temp = (char *)malloc(sizeof(char)*1024);

	curl = curl_easy_init();

	if(curl == NULL){
		on_error("url_read_u : 初始化CURL失败\n");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		on_error("url_read_u : 启动CURL失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &socket);

	if(res != CURLE_OK){
		on_error("url_read_u : 获取套接字失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	new_size = 0;
	
	for(;;){
		if(!wait_on_socket(socket, FD_RECV, ONE_MINUTE)){
			on_error("url_read_u : 套接字连接超时\n");
			return -1;
		}
		memset(temp, '\0', 1024);
		res = curl_easy_recv(curl, temp, 1024, &ret_len);
		
		if(res != CURLE_OK){
			break;
		}
		
		new_size += ret_len;

		if(new_size > *length){
			*buffer = realloc(*buffer, new_size);
		}

		if(*buffer == NULL){
			on_error("url_read_u : 为写扩展缓存出错 : %s\n", \
				new_size, strerror(errno));
			break;
		}
		
		//on_error("buffer now is %d bytes\n", new_size);
		
		offset = new_size - ret_len;
		memcpy((char*)*buffer+offset, temp, ret_len);

		*length = new_size;
		if(proc != NULL){
			proc(new_size, -1.0);
		}
	}

	curl_easy_cleanup(curl);

	ut_status = UT_OK;

	return ret_len;
}

/**
 * 向网络写入时由CURL库回调，私有函数
 */
static size_t read_data(void *ptr, size_t size, size_t nmemb, void *data){
	size_t real_size = size * nmemb;
	mem_block *block = (mem_block *)data;
	
	if(real_size < 1){
		return 0;
	}
	
	if(block->size){
		*(char *)ptr = block->memory[0];/* copy one byte */
		block->memory++;                /* increase pointer */
		block->size--;                  /* decrease size */
		return 1;                       /* return one byte one time */
	}
	
	return 0;                           /* no more data left */
}

/**
 * 想URL写入block中的数据，具体接口描述参看ut_ios.h
 */
int url_write_mem(const char *url, const mem_block *block){
	CURL *curl;
	CURLcode res;
	mem_block *mem;
	int length;

	if(url == NULL || block == NULL){
		on_error("url_write_mem : 入口参数错误，URL或地址为空\n");
		return -1;
	}
	
	mem = mem_block_dup(block);
	if(mem == NULL){
		on_error("url_write_mem : 为写分配内存失败\n");
		return -1;
	}

	length = mem->size;

	curl = curl_easy_init();
	
	if(curl == NULL){
		on_error("url_write_mem : 初始化CURL错误\n");
		return -1;
	}
	
	//set url
	curl_easy_setopt(curl, CURLOPT_URL, url);

	//enable post
	curl_easy_setopt(curl, CURLOPT_POST, 1L);

	//register read callback
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_data);

	//set read data
	curl_easy_setopt(curl, CURLOPT_READDATA, mem);
	
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, (curl_off_t)mem->size);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		on_error("url_write_mem : 启动CURL失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

    /* always cleanup */
    curl_easy_cleanup(curl);

	ut_status = UT_OK;
	
	return length;
}

/**
 * 向本地存储时由CURL库回调，私有函数
 */
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *data){
	size_t real_size = size * nmemb;
	mem_block *block = (mem_block *)data;

	block->memory = 
		mem_block_realloc(block->memory, block->size + real_size + 1);
	if(block->memory != NULL){
		memcpy(&(block->memory[block->size]), ptr, real_size);
		block->size += real_size;
		block->memory[block->size] = 0;
	}

	return real_size;
}

/**
 * 将url指定的文件存入block, 用于下载http://host/page或者
 * ftp://user@pass:host/file
 * block的memory字段为char型数据，可以转储为其他类型。具体接口描述参看ut_ios.h
 */
int url_read_mem(const char *url, mem_block *block){
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	
	if(curl == NULL){
		on_error("url_write_mem : 初始化CURL失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}
	
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)block);
	
	res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		on_error("url_write_mem : 启动CURL失败，错误信息：%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	curl_easy_cleanup(curl);

	ut_status = UT_OK;

	return 0;
}
