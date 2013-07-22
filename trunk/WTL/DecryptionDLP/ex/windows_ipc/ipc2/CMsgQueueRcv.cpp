/**
 *   Filename: CMsgQueueRcv.cpp
 *     Author: wujing
 * Created on: Mar 18, 2012
 *    Comment: 测试消息队列接收消息
 */

#include "CMsgQueue.cpp"

int main(){
	struct msg_buf msgbuf;
	msgbuf.mtype = 1;

	IIPC* pIIPC = new CMsgQueue;
	// 打开消息队列
	pIIPC->Open();
	// 接收消息
	pIIPC->RecvMsg(&msgbuf, sizeof(msgbuf.data));
	printf("rcv msg is [%s] \n", msgbuf.data);
	// 关闭消息队列
	pIIPC->Close();
	system("ipcs -q");
}
