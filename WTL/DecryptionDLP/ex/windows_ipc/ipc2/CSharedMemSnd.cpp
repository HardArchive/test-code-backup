/**
 *   Filename: CSharedMemRcv.cpp
 *     Author: wujing
 * Created on: Mar 18, 2012
 *    Comment: 测试共享内存接收消息
 */

#include "CSharedMem.cpp"

int main(){
	char msg[] = "test shared memeory.";

	IIPC* pIIPC = new CSharedMem;
	// 创建共享内存
	pIIPC->Open();
	// 发送消息
	pIIPC->SendMsg(msg, sizeof(msg));
	printf("snd msg is [%s]\n", msg);
	system("ipcs -m");
}
