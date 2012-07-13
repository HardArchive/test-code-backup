#include "stdafx.h"

//UTF-8:   3�ֽ�һ���ַ�
//UNICODE: 2�ֽ�һ���ַ�
//GB2312:  1�ֽ�һ���ַ�
//
//		 ���ӣ�
//
//"��"�ֵ�UTF-8����: E4 BD A0����������������11100100 10111101 10100000
//"��"��Unicode����: 4F 60������������������   01001111 01100000
// ����UTF-8�ı�����򣬷ֽ����£�xxxx0100 xx111101 xx100000
//�ѳ���x֮�������ƴ����һ�𣬾ͱ�ɡ��㡱��Unicode�����ˡ�
//ע��UTF-8����ǰ�棳��1����ʾ����UTF-8�����ɣ����ֽڹ��ɵġ�
//����UTF-8����֮����Ҳ������������ַ��ˣ���Ϊ���λʼ��Ϊ1��
//


//�ඨ��
class CChineseCode
{
public:
	static void UTF_8ToUnicode(wchar_t* pOut,char *pText);  // ��UTF-8ת����Unicode
	static void UnicodeToUTF_8(char* pOut,wchar_t* pText);  //Unicode ת����UTF-8
	static void UnicodeToGB2312(char* pOut,wchar_t uData);  // ��Unicode ת���� GB2312 
	static void Gb2312ToUnicode(wchar_t* pOut,char *gbBuffer);// GB2312 ת���ɡ�Unicode
	static void GB2312ToUTF_8(string& pOut,char *pText, int pLen);//GB2312 תΪ UTF-8
	static void UTF_8ToGB2312(string &pOut, char *pText, int pLen);//UTF-8 תΪ GB2312
};

//��ʵ��

void CChineseCode::UTF_8ToUnicode(wchar_t* pOut,char *pText)
{
	char* uchar = (char *)pOut;

	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);

	return;
}

void CChineseCode::UnicodeToUTF_8(char* pOut,wchar_t* pText)
{
	// ע�� WCHAR�ߵ��ֵ�˳��,���ֽ���ǰ�����ֽ��ں�
	char* pchar = (char *)pText;

	pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
	pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
	pOut[2] = (0x80 | (pchar[0] & 0x3F));

	return;
}

void CChineseCode::UnicodeToGB2312(char* pOut,wchar_t uData)
{
	WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(wchar_t),NULL,NULL);
	return;
}     

void CChineseCode::Gb2312ToUnicode(wchar_t* pOut,char *gbBuffer)
{
	::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
	return ;
}

void CChineseCode::GB2312ToUTF_8(string& pOut,char *pText, int pLen)
{
	char buf[4];
	int nLength = pLen* 3;
	char* rst = new char[nLength];

	memset(buf,0,4);
	memset(rst,0,nLength);

	int i = 0;
	int j = 0;      
	while(i < pLen)
	{
		//�����Ӣ��ֱ�Ӹ��ƾ���
		if( *(pText + i) >= 0)
		{
			rst[j++] = pText[i++];
		}
		else
		{
			wchar_t pbuffer;
			Gb2312ToUnicode(&pbuffer,pText+i);

			UnicodeToUTF_8(buf,&pbuffer);

			unsigned short int tmp = 0;
			tmp = rst[j] = buf[0];
			tmp = rst[j+1] = buf[1];
			tmp = rst[j+2] = buf[2];    

			j += 3;
			i += 2;
		}
	}
	rst[j] = '\0';

	//���ؽ��
	pOut = rst;             
	delete []rst;   

	return;
}

void CChineseCode::UTF_8ToGB2312(string &pOut, char *pText, int pLen)
{
	char * newBuf = new char[pLen];
	char Ctemp[4];
	memset(Ctemp,0,4);
	ZeroMemory(newBuf, pLen);

	int i =0;
	int j = 0;

	while(i < pLen)
	{
		if(pText > 0)
		{
			newBuf[j++] = pText[i++];                       
		}
		else                 
		{
			WCHAR Wtemp;
			UTF_8ToUnicode(&Wtemp,pText + i);

			UnicodeToGB2312(Ctemp,Wtemp);

			newBuf[j] = Ctemp[0];
			newBuf[j + 1] = Ctemp[1];

			i += 3;    
			j += 2;   
		}
	}
	newBuf[j] = '\0';

	pOut = newBuf;
	//delete []newBuf;

	return; 
}  

