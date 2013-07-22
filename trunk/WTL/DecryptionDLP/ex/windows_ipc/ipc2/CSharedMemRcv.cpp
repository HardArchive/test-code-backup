/**
 *   Filename: CSharedMemRcv.cpp
 *     Author: wujing
 * Created on: Mar 18, 2012
 *    Comment: 测试共享内存接收消息
 */

#include "CSharedMem.cpp"

int main(){
	char msg[1024];

	IIPC* pIIPC = new CSharedMem;
	// 创建共享内存
	pIIPC->Open();
	// 接受消息
	pIIPC->RecvMsg(msg, sizeof(msg));
	printf("rcv msg is [%s] \n", msg);
	// 删除共享内存
	pIIPC->Close();
	system("ipcs -m");
}
