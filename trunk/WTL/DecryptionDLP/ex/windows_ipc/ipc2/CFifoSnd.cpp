/**
 *   Filename: CFifoSnd.cpp
 *     Author: wujing
 * Created on: Mar 18, 2012
 *    Comment: ����FIFO������Ϣ
 */

#include "CFifo.cpp"

int main(){
	char msg[] = "test FIFO.";

	IIPC* pIIPC = new CFifo;
	// �򿪹ܵ�
	pIIPC->Open();
	// ������Ϣ
	pIIPC->SendMsg(msg, sizeof(msg));
	printf("snd msg is [%s]\n", msg);
}
