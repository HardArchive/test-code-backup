/**
 *   Filename: CFifo.cpp
 *     Author: wujing
 * Created on: Mar 18, 2012
 *    Comment: 实现FIFO
 */

#include "IIPC.cpp"

class CFifo : public IIPC{
public:
	CFifo();
	virtual ~CFifo(){};
	virtual int Open();
	virtual int Close();
	virtual int SendMsg(const void* buf, size_t len);
	virtual int RecvMsg(void* buf, size_t len);
private:
	int fd; /* 文件描述符 */
};

CFifo::CFifo(){
}

int CFifo::Open(){
	/* 创建管道, 存取权限为0666, 不处理”管道文件已存在“错误 */
	if((mkfifo(FIFO, S_IFIFO|0666) == -1)&& (errno != EEXIST)){
		perror("create FIFO error\n");
		return -1;
	}

	/* 以读写方式打开管道 */
	if((fd = open(FIFO, O_RDWR)) == -1){
		perror("open FIFO error\n");
		return -1;
	}
	return 0;
}

int CFifo::Close(){
	close(fd);
	if(unlink(FIFO) == -1){
		perror("unlink error\n");
		return -1;
	}
	return 0;
}

int CFifo::SendMsg(const void* buf, size_t len){
	if(write(fd, buf, len) == -1){
		perror("write FIFO error\n");
		return -1;
	}
	return 0;
}

int CFifo::RecvMsg(void* buf, size_t len){
	memset(buf, 0, sizeof(buf));
	if(read(fd, buf, len) == -1){
		perror("read FIFO error\n");
		return -1;
	}
	return 0;
}
