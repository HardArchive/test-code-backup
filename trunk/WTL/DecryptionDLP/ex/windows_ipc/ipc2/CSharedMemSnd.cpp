/**
 *   Filename: CSharedMemRcv.cpp
 *     Author: wujing
 * Created on: Mar 18, 2012
 *    Comment: ���Թ����ڴ������Ϣ
 */

#include "CSharedMem.cpp"

int main(){
	char msg[] = "test shared memeory.";

	IIPC* pIIPC = new CSharedMem;
	// ���������ڴ�
	pIIPC->Open();
	// ������Ϣ
	pIIPC->SendMsg(msg, sizeof(msg));
	printf("snd msg is [%s]\n", msg);
	system("ipcs -m");
}
