#include "IOData.h"

CIOData::CIOData(void)
{
}

CIOData::~CIOData(void)
{
}

bool CIOData::DeletePlayer(const char* pPlayerNick)
{
	char szFileName[MAX_FILENAME_BUFF] = {'\0'};
	bool blState = GetFileName(pPlayerNick, szFileName);
	if(false == blState)
	{
		return false;
	}

	remove(szFileName);
	return true;
}

bool CIOData::SavePlayer(CPlayerData* pPlayerData)
{
	char szFileName[MAX_FILENAME_BUFF] = {'\0'};
	char szFileLine[MAX_FILEBUFF_500]  = {'\0'};

	bool blState = GetFileName(pPlayerData->GetPlayerName(), szFileName);
	if(false == blState)
	{
		return false;
	}

	//ÿ���ļ�����д�룬����ԭ���ļ�
	FILE* fp = fopen(szFileName, "w+");
	if(NULL == fp)
	{
		return false;
	}

	//д��ID
	sprintf_safe(szFileLine, MAX_FILENAME_BUFF,  "ID=%d\n", pPlayerData->GetPlayerID());
	size_t szWriteLen = fwrite(szFileLine, strlen(szFileLine), 1, fp);
	if(szWriteLen != 1)
	{
		fclose(fp);
		return false;
	}

	//д���ǳ�����
	sprintf_safe(szFileLine, MAX_FILENAME_BUFF,  "Nike=%s\n", pPlayerData->GetPlayerName());
	szWriteLen = fwrite(szFileLine, strlen(szFileLine), 1, fp);
	if(szWriteLen != 1)
	{
		fclose(fp);
		return false;
	}

	//д��Level
	sprintf_safe(szFileLine, MAX_FILENAME_BUFF,  "level=%d\n", pPlayerData->GetLevel());
	szWriteLen = fwrite(szFileLine, strlen(szFileLine), 1, fp);
	if(szWriteLen != 1)
	{
		fclose(fp);
		return false;
	}

	fclose(fp);

	return true;
}

CPlayerData* CIOData::GetPlayer(const char* pPlayerNick)
{
	char szFileName[MAX_FILENAME_BUFF] = {'\0'};

	bool blState = GetFileName(pPlayerNick, szFileName);
	if(false == blState)
	{
		return NULL;
	}

	FILE* fp = fopen(szFileName, "r");
	if(NULL == fp)
	{
		return NULL;
	}

	//�õ���ǰ�ļ�����
	int nFileLen = fseek(fp, 0, SEEK_END);
	if(0 >= nFileLen)
	{
		fclose(fp);
		return NULL;
	}
	
	//���������ļ��ڴ滺����
	char* pFileBuff = new char[nFileLen + 1];
	size_t stReadLen = fread(pFileBuff, nFileLen, 1, fp);
	pFileBuff[nFileLen] = '\0';          //�����һ���ֽ�ǿ��\0��β
	if((int)stReadLen != nFileLen)
	{
		fclose(fp);
		return NULL;
	}

	//�ر��ļ�
	fclose(fp);

	//�����ļ�
	CPlayerData* pPlayerData = ParseFile(pFileBuff);
	delete[] pFileBuff;

	return pPlayerData;
}

CPlayerData* CIOData::ParseFile(char* pBuff)
{
	CPlayerData* pPlayerData = new CPlayerData();
	if(NULL == pPlayerData)
	{
		return NULL;
	}

	char* pBin = pBuff;

	//��������
	char szValue[MAX_FILEBUFF_500] = {'\0'};
	int nValue = 0;

	GetData(pBin, "ID=", "\n", szValue);  //�õ�ID
	nValue = atoi(szValue);
	pPlayerData->SetPlayerID(nValue);
	GetData(pBin, "Nike=", "\n", szValue);  //�õ�Nick
	pPlayerData->SetPlayerName(szValue);
	GetData(pBin, "level=", "\n", szValue);  //�õ�Level
	nValue = atoi(szValue);
	pPlayerData->SetPlayerLevel(nValue);

	return pPlayerData;
}

bool CIOData::GetData(char*& pBin, const char* pTagBegin, const char* pTagEnd, char* pValue)
{

	char* pBegin = strstr(pBin, pTagBegin);
	if(pBegin ==  NULL)
	{
		return false;
	}

	char* pEnd = strstr(pBegin + (int)strlen(pTagBegin), pTagEnd);
	if(pEnd ==  NULL)
	{
		return false;
	}
	int nLen = (int)(pEnd - pBegin - (int)strlen(pTagBegin));

	if(nLen >= MAX_FILEBUFF_500)
	{
		nLen = MAX_FILEBUFF_500;
	}

	memcpy(pValue, pBegin + (int)strlen(pTagBegin), nLen);
	pValue[nLen] = '\0';

	pBin = pEnd;

	return true;
}

bool CIOData::GetFileName(const char* pPlayerNick, char* pFileName)
{
	if(pFileName == NULL)
	{
		return false;
	}

	sprintf_safe(pFileName, MAX_FILENAME_BUFF,  "%s\\%s.txt", DB_FILE_PATH, pPlayerNick);
	return true;
}

