/**
 *   Filename: CFifoSnd.cpp
 *     Author: wujing
 * Created on: Mar 18, 2012
 *    Comment: 测试FIFO发送消息
 */

#include "CFifo.cpp"

int main(){
	char msg[] = "test FIFO.";

	IIPC* pIIPC = new CFifo;
	// 打开管道
	pIIPC->Open();
	// 发送消息
	pIIPC->SendMsg(msg, sizeof(msg));
	printf("snd msg is [%s]\n", msg);
}
