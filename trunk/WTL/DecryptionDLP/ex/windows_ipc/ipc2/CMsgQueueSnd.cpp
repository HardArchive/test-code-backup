/**
 *   Filename: CMsgQueueSnd.cpp
 *     Author: wujing
 * Created on: Mar 18, 2012
 *    Comment: ������Ϣ���з�����Ϣ
 */

#include "CMsgQueue.cpp"

int main(){
	struct msg_buf msgbuf;
	char msg[] = "test msg queue.";
	msgbuf.mtype = 1;
	strcpy(msgbuf.data, msg);

	IIPC* pIIPC = new CMsgQueue;
	// ����Ϣ����
	pIIPC->Open();
	// ������Ϣ
	pIIPC->SendMsg(&msgbuf, sizeof(msg));
	printf("snd msg is [%s]\n", msg);
	system("ipcs -q");
}
