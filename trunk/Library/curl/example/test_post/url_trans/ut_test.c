/*
 * 文件名称 : ut_test.c
 * 文件作用 : URL传输单元测试 
 *
 * 文件作者 : 邱俊涛
 * 创建日期 : 2010-09-03
 *
 * 修改作者 : 
 * 修改内容 : 
 * 修改日期 : 
 *
 */
#include <stdio.h>
#include <string.h>

#include <sys/stat.h>

#include <fcntl.h>

#include "ut_ios.h"
#include "ut_mem.h"


static void update(double current, double total){
	if(total < current){
		fprintf(stderr, "read : %f bytes\n", current);
	}else{
		fprintf(stderr, "%% %.2f processed\n", (current/total)*100);
	}
}

static int load_file_to_mem(const char *file, char **buffer, int *length){
    struct stat file_info;
    FILE *fp;
    size_t file_size, bytes_read;

    fp = fopen(file, "rb");

    if(fp == NULL){
        return -1;
    }

    if(fstat(fileno(fp), &file_info) != 0){
        fclose(fp);
        return -1;
    }

    file_size = file_info.st_size;
    *length = file_size;

    *buffer = realloc(*buffer, file_size);

    if(*buffer == NULL){
        fclose(fp);
        return -1;
    }

    if((bytes_read = fread(*buffer, file_size, 1, fp)) < 0){
        fclose(fp);
        return -1;
    }

    fclose(fp);

    return 0;
}

size_t get_file_len(const char *file){
    struct stat file_info;
    FILE *fp;
    size_t file_size, bytes_read;

    fp = fopen(file, "rb");

    if(fp == NULL){
        return -1;
    }

    if(fstat(fileno(fp), &file_info) != 0){
        fclose(fp);
        return -1;
    }

    file_size = file_info.st_size;
	fprintf(stderr, "file size = %d\n", file_size);
	fclose(fp);

	return file_size;
}

int main(int argc, char *argv[]){
	int res = 0;
	res = raw_send_file("192.168.137.2:9527", "us4.jpg", get_file_len("us4.jpg"), update);
	
	if(res < 0){
		fprintf(stderr, "发送文件错误 : %s\n", get_last_error());
	}else{
		fprintf(stderr, "发送文件成功\n");
	}
	
	getchar();
	

	/*
	int res = 0;
	res = raw_recv_file("192.168.21.2:8384", "us4.jpg", 5470971, update);
	
	if(res < 0){
		fprintf(stderr, "接收文件错误 : %s\n", get_last_error());
	}else{
		fprintf(stderr, "接收文件成功\n");
	}
	
	getchar();
	*/

	/*
	int res = 0;
	int len = 1024;

	char *buffer = (char *)malloc(sizeof(char)*len);
	
	res = raw_read_u("192.168.21.2:8384", &buffer, &len, update);
	
	if(res < 0){
		fprintf(stderr, "接收文件错误 : %s\n", get_last_error());
	}else{
		fprintf(stderr, "接收文件成功\n");
	}
	
	free(buffer);

	getchar();
	*/

	return 0;

	/*
	int res = 0, fd = 0;
	char *req_buf;
	char *res_buf;
	char *buffer;

	int req_len = 1024;
	int res_len = 1024;
	int len = 1024;

	mem_block block;

	req_buf = (char *)malloc(sizeof(char) * req_len);
	res_buf = (char *)malloc(sizeof(char) * res_len);

	if(req_buf == NULL){
		fprintf(stderr, "为读缓冲区分配内存失败\n");
		return -1;
	}
	
	fd = load_file_to_mem("CXSJK.xml.bak", &req_buf, &req_len);

	if(fd < 0){
		fprintf(stderr, "读取文件至buffer失败\n");
		free(req_buf);
		return -1;
	}

	res = 
		raw_send_recv("10.111.43.145:8384", req_buf, req_len, &res_buf, &res_len);
	if(res < 0){
		fprintf(stderr, "读写错误：%s\n", get_last_error());
	}else{
		fprintf(stderr, "读取长度[%d]\n读取内容：%s\n", res_len, res_buf);
		fprintf(stderr, "错误信息：%s\n", get_last_error());
	}

	

	//libcurl();

	mem_block block;
	mem_block_init(&block);

	int res = url_read_mem("http://10.111.43.145:9999/request.xml", &block);

	if(res != UT_OK){
		fprintf(stderr, "%s\n", get_last_error());
	}else{
		block.size;//
		block.memory;//
	}

	mem_block_free(&block);
	return 0;

	
	buffer = (char *)malloc(sizeof(char) * len);
	load_file_to_mem("CXSJK.xml.bak", &buffer, &len);

	mem_block_init(&block);
	mem_block_new(&block, buffer, len);
	
	res = url_write_mem("http://10.111.43.145:9999/req.idp", &block);
	
	//res = url_write_mem("http://10.111.43.145:9999/req.idp", &block);
	//res = raw_write_n("http://10.111.43.145:9999/req.idp", buffer, len);
	
	if(res < 0){
		fprintf(stderr, "向URL写入数据失败%s\n", get_last_error());
		return -1;
	}else{
		fprintf(stderr, "向URL写入数据成功，数据长度：%d\n", res);
	}
	
	raw_read_u("10.111.43.145:8384", &buffer, &len, update);

	fd = load_file_to_mem("debugit", &buffer, &len);

	if(fd < 0){
		fprintf(stderr, "读取文件至buffer失败\n");
		free(buffer);
		return -1;
	}

	raw_write_u("10.111.43.145:9527", buffer, len, update);

	free(buffer);

	mem_block_free(&block);

	return 0;
	*/
}