/**
 *   Filename: IIPC.cpp
 *     Author: wujing
 * Created on: Mar 18, 2012
 *    Comment: ��ɰ����ļ�����������������ӿڵȹ���
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

#define FIFO "/tmp/testfifo" /* FIFOͨ���еĹܵ��ļ� */

/**
 * �˽ӿ��ṩ�˴򿪡��رա�������Ϣ��������Ϣ�ĸ�����
 */
class IIPC{
public:
	IIPC(){};
	virtual ~IIPC(){};

	/**
	 * ����Դ
	 *
	 * @return -1: ����Դ����; 0: ��ȷ
	 */
	virtual int Open() = 0;

	/**
	 * �ر���Դ
	 *
	 * @return -1: �ر���Դ����; 0: ��ȷ
	 */
	virtual int Close() = 0;

	/**
	 * ������Ϣ
	 *
	 * @param buf: ������Ϣ����
	 * @param len: ������Ϣ����
	 * @return -1: ������Ϣ����; 0: ��ȷ
	 */
	virtual int SendMsg(const void *buf, size_t len) = 0;

	/**
	 * ������Ϣ
	 *
	 * @param buf��������Ϣ����
	 * @param len��������Ϣ����
	 * @return -1��������Ϣ����0: ��ȷ
	 */
	virtual int RecvMsg(void *buf, size_t len) = 0;
};
