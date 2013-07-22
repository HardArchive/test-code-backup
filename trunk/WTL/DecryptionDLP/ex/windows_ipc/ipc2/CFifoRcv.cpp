/**
 *   Filename: CFifoRcv.cpp
 *     Author: wujing
 * Created on: Mar 18, 2012
 *    Comment: 测试FIFO接受消息
 */

#include "CFifo.cpp"

int main(){
	char msg[1024];

	IIPC* pIIPC = new CFifo;
	// 打开管道
	pIIPC->Open();
	// 接收消息
	pIIPC->RecvMsg(msg, sizeof(msg));
	printf("rcv msg is %s \n", msg);
	// 关闭管道
	pIIPC->Close();
}
