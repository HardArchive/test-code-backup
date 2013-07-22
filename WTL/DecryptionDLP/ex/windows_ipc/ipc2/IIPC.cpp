/**
 *   Filename: IIPC.cpp
 *     Author: wujing
 * Created on: Mar 18, 2012
 *    Comment: 完成包含文件、声明常量、定义接口等功能
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstddef>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define FIFO "/tmp/testfifo" /* FIFO通信中的管道文件 */

/**
 * 此接口提供了打开、关闭、发送消息、接受消息四个方法
 */
class IIPC{
public:
	IIPC(){};
	virtual ~IIPC(){};

	/**
	 * 打开资源
	 *
	 * @return -1: 打开资源出错; 0: 正确
	 */
	virtual int Open() = 0;

	/**
	 * 关闭资源
	 *
	 * @return -1: 关闭资源出错; 0: 正确
	 */
	virtual int Close() = 0;

	/**
	 * 发送消息
	 *
	 * @param buf: 发送消息内容
	 * @param len: 发送消息长度
	 * @return -1: 发送消息出错; 0: 正确
	 */
	virtual int SendMsg(const void *buf, size_t len) = 0;

	/**
	 * 接收消息
	 *
	 * @param buf：接收消息内容
	 * @param len：接收消息长度
	 * @return -1：接收消息出错；0: 正确
	 */
	virtual int RecvMsg(void *buf, size_t len) = 0;
};
