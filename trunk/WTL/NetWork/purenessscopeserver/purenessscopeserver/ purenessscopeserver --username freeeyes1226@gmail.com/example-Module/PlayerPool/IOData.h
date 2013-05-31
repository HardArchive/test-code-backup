#ifndef _IODATA_H
#define _IODATA_H

//����ฺ�������ڴ����ݺ�IO����֮��Ľ���

#include "PlayerObject.h"

//�û����ݴ��·����������һ����Ŀ¼���������Ҫһ���洢���Եķ������ṩ�ļ��洢��
#define DB_FILE_PATH  "D:\\IOData"

#define MAX_FILENAME_BUFF  200
#define MAX_FILEBUFF_500   500

class CIOData
{
public:
	CIOData(void);
	~CIOData(void);

	bool DeletePlayer(const char* pPlayerNick);          //ɾ��һ���û������ļ�
	bool SavePlayer(CPlayerData* pPlayerData);           //���洴���û�������
	CPlayerData* GetPlayer(const char* pPlayerNick);     //������IO������ң��ҵ��˾�newһ��CPlayerData������������ظ��ϲ㣬���ϲ����긺��ɾ��

private:
	bool GetFileName(const char* pPlayerNick, char* pFileName);   //�ļ����������
	CPlayerData* ParseFile(char* pBuff);                          //�����ļ��е����ݣ������CPlayerData�ṹ
	bool GetData(char*& pBin, const char* pTagBegin, const char* pTagEnd, char* pValue);
};
#endif
