/**
 *   Filename: CFifoRcv.cpp
 *     Author: wujing
 * Created on: Mar 18, 2012
 *    Comment: ����FIFO������Ϣ
 */

#include "CFifo.cpp"

int main(){
	char msg[1024];

	IIPC* pIIPC = new CFifo;
	// �򿪹ܵ�
	pIIPC->Open();
	// ������Ϣ
	pIIPC->RecvMsg(msg, sizeof(msg));
	printf("rcv msg is %s \n", msg);
	// �رչܵ�
	pIIPC->Close();
}
