/**
 *   Filename: CMsgQueueRcv.cpp
 *     Author: wujing
 * Created on: Mar 18, 2012
 *    Comment: ������Ϣ���н�����Ϣ
 */

#include "CMsgQueue.cpp"

int main(){
	struct msg_buf msgbuf;
	msgbuf.mtype = 1;

	IIPC* pIIPC = new CMsgQueue;
	// ����Ϣ����
	pIIPC->Open();
	// ������Ϣ
	pIIPC->RecvMsg(&msgbuf, sizeof(msgbuf.data));
	printf("rcv msg is [%s] \n", msgbuf.data);
	// �ر���Ϣ����
	pIIPC->Close();
	system("ipcs -q");
}
