/**
 *   Filename: CMsgQueueSnd.cpp
 *     Author: wujing
 * Created on: Mar 18, 2012
 *    Comment: 测试消息队列发送消息
 */

#include "CMsgQueue.cpp"

int main(){
	struct msg_buf msgbuf;
	char msg[] = "test msg queue.";
	msgbuf.mtype = 1;
	strcpy(msgbuf.data, msg);

	IIPC* pIIPC = new CMsgQueue;
	// 打开消息队列
	pIIPC->Open();
	// 发送消息
	pIIPC->SendMsg(&msgbuf, sizeof(msg));
	printf("snd msg is [%s]\n", msg);
	system("ipcs -q");
}
