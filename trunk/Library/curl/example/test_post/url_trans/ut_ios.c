/*
 * �ļ����� : ut_ios.c
 * �ļ����� : URL��������/���ģ��ʵ��
 *
 * �ļ����� : ����
 * �������� : 2010-09-03
 *
 * �޸����� : ����
 * �޸����� : ��raw_write_n/raw_read_n��ʵ��
 * �޸����� : 2010-09-03
 *
 * �޸����� : ����
 * �޸����� : ��url_read_men/url_write_mem��ʵ��
 * �޸����� : 2010-09-06
 *
 * �޸����� : ����
 * �޸����� : ���raw_send_recv�ӿڵ�ʵ��
 * �޸����� : 2010-09-07
 *
 * �޸����� : ����
 * �޸����� : ��Ӵ�����Ϣ�ӿ�ʵ��
 * �޸����� : 2010-09-07
 *
 * �޸����� : ����
 * �޸����� : ����շ��ļ��ӿ�ʵ��
 * �޸����� : 2010-09-08 
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
 * ��ȡ���һ�η����Ĵ���
 */
const char *get_last_error(void){
	if(ut_status == UT_OK){
		return NULL;
	}
	return ut_error_message;
}

/* 
 * ˵����Ϊfor_recv���͵�socket�ȴ�timeout_ms���룬ָ����/д�˿���
 *
 * ������
 * sockfd     : ͨ���ŵ���Զ��socket
 * for_recv   : ͨ�����ͣ�Ϊ��/д�򿪵�socket
 * timeout_ms : ��λΪ����ĳ�ʱʱ��
 * 
 * ����ֵ��
 * -1     : ʧ��(��ʱ��select����ʧ��)
 * number : ��ǹ���socket��Ŀ
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
 * ��ӡ������Ϣ�����󻺳���(ut_error_message)��˽�к���
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
 * ����req_buf,Ȼ�󽫻�����Ϣ������res_buf,����ӿ������ο�ut_ios.h
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
		on_error("raw_send_recv : ��ʼ��CURLʧ��\n");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		on_error("raw_send_recv : ����CURLʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &socket);

	if(res != CURLE_OK){
		on_error("raw_send_recv : ��ȡ�׽���ʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	/* wait for the socket to become ready for sending */
	if(!wait_on_socket(socket, FD_SEND, ONE_MINUTE)){
		on_error("raw_send_recv : �׽������ӳ�ʱ(����ʱ)\n");
		return -1;
	}

	res = curl_easy_send(curl, req_buf, req_len, &ret_len);
	
	if(res != CURLE_OK){
		on_error("raw_send_recv : ��������ʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}
	
	if(req_len != ret_len){
		on_error("raw_send_recv : ����δ������ɣ�ʣ�ࣺ%d\n", \
			(req_len - ret_len));
		return -1;
	}

	new_size = 0;
	
	for(;;){
		if(!wait_on_socket(socket, FD_RECV, ONE_MINUTE)){
			on_error("raw_send_recv : �׽������ӳ�ʱ(����ʱ)\n");
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
			on_error("raw_send_recv : Ϊд��չ������� : %s\n", \
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
 * ˵����
 * �򿪳���Ϊfile_size���ļ�file_name,������urlָ����λ��, ͨ������
 * processing���½���
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
		on_error("raw_send_file : ��ʼ��CURLʧ��\n");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		on_error("raw_send_file : ����CURLʧ�ܣ�������Ϣ��%s\n", curl_easy_strerror(res));
		return -1;
	}

	res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &socket);

	if(res != CURLE_OK){
		on_error("raw_send_file : ��ȡ�׽���ʧ�ܣ�������Ϣ��%s\n", curl_easy_strerror(res));
		return -1;
	}

	buffer = (char *)malloc(sizeof(char) * len);
	memset(buffer, '\0', len);

	fp = fopen(file_name, "rb");
	
	if(fp == NULL){
		on_error("raw_send_file : Ϊ�����ļ�ʧ�ܣ�%s\n", strerror(errno));
		return -1;
	}
	
	size = file_size;
	
	bytes_send = 0;

	while(size > 0){
		if(!wait_on_socket(socket, FD_SEND, ONE_MINUTE)){
			on_error("raw_send_file : �׽������ӳ�ʱ\n");
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
			on_error("raw_send_file : ��������ʧ�ܣ�������Ϣ��%s\n", \
				curl_easy_strerror(res));
			fclose(fp);
			free(buffer);
			return -1;
		}
				
		if(ret_len != 1024 && ret_len != size){
			on_error("raw_send_file : ����δ������ɣ�ʣ�ࣺ%d\n", \
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
 * ˵����
 * ��urlָ����λ�ö��ļ����������ļ�file_name, ͨ������processing���½���
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
		on_error("raw_recv_file : ��ʼ��CURLʧ��\n");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		on_error("raw_recv_file : ����CURLʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &socket);

	if(res != CURLE_OK){
		on_error("raw_recv_file : ��ȡ�׽���ʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	fp = fopen(file_name, "wb");
	
	if(fp == NULL){
		on_error("raw_recv_file : ���ļ�ʧ�ܣ�������Ϣ��%s\n", strerror(errno));
		return -1;
	}

	buffer = (char *)malloc(sizeof(char) * len);
	memset(buffer, '\0', len);

	bytes_recv = 0;
	size = file_size;

	while(size > 0){
		if(!wait_on_socket(socket, FD_RECV, ONE_MINUTE)){
			on_error("raw_recv_file : �׽������ӳ�ʱ\n");
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
				on_error("raw_recv_file : �����ļ�����������Ϣ ��%s\n", \
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
 * ��URLд�������ݣ�����ӿ������ο�ut_ios.h
 */
int raw_write_n(const char *url, void *buffer, unsigned int length){
	CURL *curl;
	CURLcode res;

	int socket;
	size_t ret_len;

	curl = curl_easy_init();

	if(curl == NULL){
		on_error("url_write_n : ��ʼ��CURLʧ��\n");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		on_error("url_write_n : ����CURLʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &socket);

	if(res != CURLE_OK){
		on_error("url_write_n : ��ȡ�׽���ʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	/* wait for the socket to become ready for sending */
	if(!wait_on_socket(socket, FD_SEND, ONE_MINUTE)){
		on_error("url_write_n : �׽������ӳ�ʱ\n");
		return -1;
	}

	res = curl_easy_send(curl, buffer, length, &ret_len);

	if(res != CURLE_OK){
		on_error("url_write_n : ��������ʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}
	
	if(length != ret_len){
		on_error("url_write_n : ����δ������ɣ�ʣ�ࣺ%d\n", \
			(length - ret_len));
		return -1;
	}

	curl_easy_cleanup(curl);
	
	ut_status = UT_OK;
	return ret_len;
}

/**
 * д�붨�����ݿ飬���ǿ��ܸ�����Ҫ�����ݷֿ鷢�ͣ�����ӿ������ο�ut_ios.h
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
		on_error("raw_write_u : ��ʼ��CURLʧ��\n");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		on_error("raw_write_u : ����CURLʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &socket);

	if(res != CURLE_OK){
		on_error("raw_write_u : ��ȡ�׽���ʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	/* wait for the socket to become ready for sending */
	if(!wait_on_socket(socket, FD_SEND, ONE_MINUTE)){
		on_error("raw_write_u : �׽������ӳ�ʱ\n");
		return -1;
	}
	
	offset = 0;
	left = BLOCK_SIZE;
	for(;;){
		/* wait for the socket to become ready for sending */
		if(!wait_on_socket(socket, FD_SEND, ONE_MINUTE)){
			on_error("raw_write_u : �׽������ӳ�ʱ\n");
			return -1;
		}
		temp = (char *)malloc(sizeof(char) * BLOCK_SIZE);
		memcpy(temp, (char *)buffer+offset, left);
		
		res = curl_easy_send(curl, temp, left, &ret_len);

		//res = curl_easy_send(curl, buffer, length, &ret_len);

		if(res != CURLE_OK){
			on_error("raw_write_u : ��������ʧ�ܣ�������Ϣ��%s\n", \
				curl_easy_strerror(res));
			break;
		}
				
		if(left != ret_len){
			on_error("raw_write_u : ����δ������ɣ�ʣ�ࣺ%d\n", \
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
 * ��URL���������ݣ�����ӿ������ο�ut_ios.h
 */
int raw_read_n(const char *url, void *buffer, unsigned int length){
	CURL *curl;
	CURLcode res;

	int socket;
	size_t ret_len;

	curl = curl_easy_init();

	if(curl == NULL){
		on_error("url_read_n : ��ʼ��CURLʧ��\n");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		on_error("url_read_n : ����CURLʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &socket);

	if(res != CURLE_OK){
		on_error("url_read_n : ��ȡ�׽���ʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	/* wait for the socket to become ready for sending */
	if(!wait_on_socket(socket, FD_RECV, ONE_MINUTE)){
		on_error("url_read_n : �׽������ӳ�ʱ\n");
		return -1;
	}

	res = curl_easy_recv(curl, buffer, length, &ret_len);

	
	if(res != CURLE_OK){
		on_error("url_read_n : ��������ʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	curl_easy_cleanup(curl);

	ut_status = UT_OK;
	
	return ret_len;	
}

/**
 * ��url��ȡ�Ƕ����ַ���������ֵ����*buffer, �ڴ���Ҫ�������ͷţ�����ӿ������ο�ut_ios.h
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
		on_error("url_read_u : ��ʼ��CURLʧ��\n");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);

	res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		on_error("url_read_u : ����CURLʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	res = curl_easy_getinfo(curl, CURLINFO_LASTSOCKET, &socket);

	if(res != CURLE_OK){
		on_error("url_read_u : ��ȡ�׽���ʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	new_size = 0;
	
	for(;;){
		if(!wait_on_socket(socket, FD_RECV, ONE_MINUTE)){
			on_error("url_read_u : �׽������ӳ�ʱ\n");
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
			on_error("url_read_u : Ϊд��չ������� : %s\n", \
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
 * ������д��ʱ��CURL��ص���˽�к���
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
 * ��URLд��block�е����ݣ�����ӿ������ο�ut_ios.h
 */
int url_write_mem(const char *url, const mem_block *block){
	CURL *curl;
	CURLcode res;
	mem_block *mem;
	int length;

	if(url == NULL || block == NULL){
		on_error("url_write_mem : ��ڲ�������URL���ַΪ��\n");
		return -1;
	}
	
	mem = mem_block_dup(block);
	if(mem == NULL){
		on_error("url_write_mem : Ϊд�����ڴ�ʧ��\n");
		return -1;
	}

	length = mem->size;

	curl = curl_easy_init();
	
	if(curl == NULL){
		on_error("url_write_mem : ��ʼ��CURL����\n");
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
		on_error("url_write_mem : ����CURLʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

    /* always cleanup */
    curl_easy_cleanup(curl);

	ut_status = UT_OK;
	
	return length;
}

/**
 * �򱾵ش洢ʱ��CURL��ص���˽�к���
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
 * ��urlָ�����ļ�����block, ��������http://host/page����
 * ftp://user@pass:host/file
 * block��memory�ֶ�Ϊchar�����ݣ�����ת��Ϊ�������͡�����ӿ������ο�ut_ios.h
 */
int url_read_mem(const char *url, mem_block *block){
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	
	if(curl == NULL){
		on_error("url_write_mem : ��ʼ��CURLʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}
	
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)block);
	
	res = curl_easy_perform(curl);

	if(res != CURLE_OK){
		on_error("url_write_mem : ����CURLʧ�ܣ�������Ϣ��%s\n", \
			curl_easy_strerror(res));
		return -1;
	}

	curl_easy_cleanup(curl);

	ut_status = UT_OK;

	return 0;
}
