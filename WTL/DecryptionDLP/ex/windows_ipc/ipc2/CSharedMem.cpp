/**
 *   Filename: CSharedMem.cpp
 *     Author: wujing
 * Created on: Mar 18, 2012
 *    Comment: 实现共享内存通信
 */

#include "IIPC.cpp"

#define SHM_BUFSZ 1024 /* 共享内存大小 */

class CSharedMem : public IIPC{
public:
	CSharedMem();
	virtual ~CSharedMem(){};
	virtual int Open();
	virtual int Close();
	virtual int SendMsg(const void* buf, size_t len);
	virtual int RecvMsg(void* buf, size_t len);
private:
	int   shmid;
	key_t shmkey;
	char* s_addr;
	char* r_addr;
};

CSharedMem::CSharedMem(){
}

int CSharedMem::Open(){
	shmkey = ftok(".", '1');
	/* shmget */
	/* 如果存在与shmkey相等的共享内存，则使用已存在的 */
	if((shmid = shmget(shmkey, SHM_BUFSZ, IPC_CREAT|0666)) == -1){
		perror("shmget error\n");
		return -1;
	}
	return 0;
}

int CSharedMem::Close(){
	/* shmctl */
	if(shmctl(shmid, IPC_RMID, 0) == -1){
		perror("shmctl error\n");
		return -1;
	}
	return 0;
}

int CSharedMem::SendMsg(const void* buf, size_t len){
	/* shmat */
	if((s_addr = (char*)shmat(shmid, 0, 0)) == NULL){
		perror("shmat error\n");
		return -1;
	}
	strncpy(s_addr, (char*)buf, SHM_BUFSZ);
	/* shmdt */
	if(shmdt(s_addr) == -1){
		perror("shmdt error1\n");
		return -1;
	}
	return 0;
}

int CSharedMem::RecvMsg(void* buf, size_t len){
	/* shmat */
	if((r_addr = (char*)shmat(shmid, 0, 0)) == NULL){
		perror("shmat error\n");
		return -1;
	}
	strncpy((char*)buf, r_addr, len);
	/* shmdt */
	if(shmdt(r_addr) == -1){
		perror("shmdt error2\n");
		return -1;
	}
	return 0;
}
