/**
 *   Filename: CSharedMemRcv.cpp
 *     Author: wujing
 * Created on: Mar 18, 2012
 *    Comment: ���Թ����ڴ������Ϣ
 */

#include "CSharedMem.cpp"

int main(){
	char msg[1024];

	IIPC* pIIPC = new CSharedMem;
	// ���������ڴ�
	pIIPC->Open();
	// ������Ϣ
	pIIPC->RecvMsg(msg, sizeof(msg));
	printf("rcv msg is [%s] \n", msg);
	// ɾ�������ڴ�
	pIIPC->Close();
	system("ipcs -m");
}
