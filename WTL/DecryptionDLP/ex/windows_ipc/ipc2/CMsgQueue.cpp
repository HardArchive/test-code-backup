/**
 *   Filename: CMsgQueue.cpp
 *     Author: wujing
 * Created on: Mar 18, 2012
 *    Comment: 实现消息队列通信
 */

#include "IIPC.cpp"

#define MSG_BUFSZ 255 /* 消息长度  */

/**
 * 消息结构体
 */
struct msg_buf {
	int mtype;
	char data[MSG_BUFSZ];
};

class CMsgQueue : public IIPC{
public:
	CMsgQueue();
	virtual ~CMsgQueue(){};
	virtual int Open();
	virtual int Close();
	virtual int SendMsg(const void* buf, size_t len);
	virtual int RecvMsg(void* buf, size_t len);
private:
	int    msqid;
	key_t  msqkey;
	struct msg_buf* msgbuf;
};

CMsgQueue::CMsgQueue(){
}

/**
 * IPC_CREAT: 如果内核中存在与msgkey相等的详细队列则不创建，直接返回
 */
int CMsgQueue::Open(){
	msqkey = ftok(".", '1');
	if((msqid = msgget(msqkey, IPC_CREAT|0666)) == -1){
		perror("msgget error\n");
		return -1;
	}
	return 0;
}

int CMsgQueue::Close(){
	msgctl(msqid, IPC_RMID, NULL);
	return 0;
}

int CMsgQueue::SendMsg(const void* buf, size_t len){
	msgbuf = (struct msg_buf*)buf;
	if(msgsnd(msqid, msgbuf, sizeof(msgbuf->data), IPC_NOWAIT) == -1){
		perror("msgsnd error\n");
		return -1;
	}
	return 0;
}

int CMsgQueue::RecvMsg(void* buf, size_t len){
	msgbuf = (struct msg_buf*)buf;
	if(msgrcv(msqid, msgbuf, sizeof(msgbuf->data), msgbuf->mtype, IPC_NOWAIT) == -1){
		perror("msgsnd error\n");
		return -1;
	}
	buf = msgbuf;
	return 0;
}
