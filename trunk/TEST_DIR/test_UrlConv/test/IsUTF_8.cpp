#include "stdafx.h"
#include "IsUTF_8.h"

//UTF8����8bits��1BytesΪ������������λ����ȻҲ�����л���16bits��32bits����ʽ���ֱ��ΪUTF16��UTF32����Ŀǰ�õò��࣬
// ��UTF8�򱻹㷺Ӧ�����ļ���������紫���С�
//����ԭ��
//�ȿ����ģ�壺
//UCS-4 range (hex.)	UTF-8 octet sequence (binary)
//0000 0000-0000 007F	0xxxxxxx
//0000 0080-0000 07FF	110xxxxx 10xxxxxx
//0000 0800-0000 FFFF	1110xxxx 10xxxxxx 10xxxxxx
//0001 0000-001F FFFF	11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
//0020 0000-03FF FFFF	111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
//0400 0000-7FFF FFFF	1111110x 10xxxxxx ... 10xxxxxx
//���벽�裺
//1) ����ȷ����Ҫ���ٸ�8bits(octets)
//2) ��������ģ�����ÿ��octets�ĸ�λbits
//3) ���ַ���bits�����x�У��ַ�˳�򣺵�λ����λ��UTF8˳�����һ��octet����ĩλx����һ��octet���λx
//����UTF8����,������6���ֽ����,����UTF8��1-6�ֽڱ������


int IsTextUTF8(char* str, ULONGLONG length)
{
	int i = 0;
	DWORD nBytes = 0;//UFT8����1-6���ֽڱ���,ASCII��һ���ֽ�
	UCHAR chr = 0;
	BOOL bAllAscii = TRUE; //���ȫ������ASCII, ˵������UTF-8
	for(i=0; i<length; i++)
	{
		chr= *(str+i);
		// �ж��Ƿ�ASCII����,�������,˵���п�����UTF-8,ASCII��7λ����,����һ���ֽڴ�,���λ���Ϊ0,o0xxxxxxx
		if((chr&0x80) != 0) 
			bAllAscii= FALSE;
		//�������ASCII��,Ӧ���Ƕ��ֽڷ�,�����ֽ���
		if(nBytes==0)
		{
			if(chr>=0x80)
			{
				if(chr>=0xFC&&chr<=0xFD)
					nBytes=6;
				else if(chr>=0xF8)
					nBytes=5;
				else if(chr>=0xF0)
					nBytes=4;
				else if(chr>=0xE0)
					nBytes=3;
				else if(chr>=0xC0)
					nBytes=2;
				else
				{
					return FALSE;
				}
				nBytes--;
			}
		}
		else //���ֽڷ��ķ����ֽ�,ӦΪ 10xxxxxx
		{
			if( (chr&0xC0) != 0x80 )
			{
				return FALSE;
			}
			nBytes--;
		}
	}
	if( nBytes > 0 ) //Υ������
	{
		return FALSE;
	}
	if( bAllAscii ) //���ȫ������ASCII, ˵������UTF-8
	{
		return FALSE;
	}
	return TRUE;
} 
//sina weibo SDK

bool IsUTF8_sina(const char *str)
{
	int count = 0;
	unsigned char chr;
	while (chr = *str++)
	{
		if (count == 0)
		{
			if (chr >= 0x80)
			{
				do
				{
					chr <<= 1;
					count++;
				} while ((chr & 0x80) != 0);
				count--;                
				if (count != 2)
				{
					return false;
				}
			}
		}
		else
		{
			if ((chr & 0xC0) != 0x80)
			{
				return false;
			}
			count--;
		}
	}
	return count == 0;
}

//Notepad2�е�


BOOL IsUnicode(const char* pBuffer,int cb/*,LPBOOL lpbBOM,LPBOOL lpbReverse*/)
{
	int i = 0xFFFF;

	BOOL bIsTextUnicode;

	BOOL bHasBOM;
	BOOL bHasRBOM;

	if (!pBuffer || cb < 2)
		return FALSE;
	//DWORD IsTextUnicode(CONST PVOID pvBuffer, int cb,PINT pResult);
	//�ı��ļ����ڵ������ǣ����ǵ�����û���ϸ����ȷ�Ĺ�����˺���ȷ�����ļ��ǰ���ANSI�ַ�����Unicode�ַ���
	// IsTextUnicodeʹ��һϵ��ͳ�Ʒ����Ͷ��Է������Ա�²⻺������ݡ������ⲻ��һ��ȷ�еĿ�ѧ���������IsTextUnicode�п��ܷ��ز���ȷ�Ľ����
	//��һ������pvBuffer���ڱ�ʶҪ���ԵĻ���ĵ�ַ���������Ǹ���Чָ�룬��Ϊ�㲻֪����ӵ�е���ANSI�ַ����黹��Unicode�ַ����顣
	//�ڶ�������cb�����趨pvBufferָ����ֽ�����ͬ���������㲻֪�������зŵ���ʲô�����cb�Ǹ��ֽ������������ַ�����
	//	��ע�⣬�����趨������������ȡ���Ȼ��IsTextUnicode�ܹ����Ե��ֽ�Խ�࣬�õ��Ľ��Խ׼ȷ��
	//����������pResult�Ǹ������ĵ�ַ�������ڵ���IsTextUnicode֮ǰ�������г�ʼ����
	//	�Ը��������г�ʼ���󣬾Ϳ���ָ����ҪIsTextUnicodeִ����Щ���ԡ�Ҳ����Ϊ�ò�������NULL��
	//	����������£�IsTextUnicode��ִ�����ܹ����е����в��ԣ���ϸ˵����μ�PlatformSDK�ĵ�����
	//���IsTextUnicode��Ϊ�������Unicode�ı����㷵��TRUE�����򷵻�FALSE��
	//	ȷʵ������������Microsoft���ú�����ԭ�͹涨Ϊ����DWORD��������ʵ���Ϸ���һ������ֵ��
	//	�����pResult����ָ��������б�������ض��Ĳ��ԣ��ú����ͻ��ڷ���֮ǰ�趨�����е���Ϣλ���Է�ӳÿ�����ԵĽ����

	bIsTextUnicode = IsTextUnicode(pBuffer, cb, &i);

	bHasBOM  = (*((UNALIGNED PWCHAR)pBuffer) == 0xFEFF);
	bHasRBOM = (*((UNALIGNED PWCHAR)pBuffer) == 0xFFFE);

	if (i == 0xFFFF) // i doesn't seem to have been modified ...
		i = 0;

	if (bIsTextUnicode || bHasBOM || bHasRBOM ||
		((i & (IS_TEXT_UNICODE_UNICODE_MASK | IS_TEXT_UNICODE_REVERSE_MASK)) &&
		!((i & IS_TEXT_UNICODE_UNICODE_MASK) && (i & IS_TEXT_UNICODE_REVERSE_MASK)) &&
		!(i & IS_TEXT_UNICODE_ODD_LENGTH) &&
		!(i & IS_TEXT_UNICODE_ILLEGAL_CHARS && !(i & IS_TEXT_UNICODE_REVERSE_SIGNATURE)) &&
		!((i & IS_TEXT_UNICODE_REVERSE_MASK) == IS_TEXT_UNICODE_REVERSE_STATISTICS))) 
	{

			//if (lpbBOM)
			//	*lpbBOM = (bHasBOM || bHasRBOM ||
			//	(i & (IS_TEXT_UNICODE_SIGNATURE | IS_TEXT_UNICODE_REVERSE_SIGNATURE)))
			//	? TRUE : FALSE;

			//if (lpbReverse)
			//	*lpbReverse = (bHasRBOM || (i & IS_TEXT_UNICODE_REVERSE_MASK)) ? TRUE : FALSE;

			return TRUE;
	}
	else
		return FALSE;
}


BOOL IsUTF8_Notepad2(const char* pTest,int nLength)
{
	static int byte_class_table[256] = {
		/*       00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F  */
		/* 00 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/* 10 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/* 20 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/* 30 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/* 40 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/* 50 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/* 60 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/* 70 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		/* 80 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		/* 90 */ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
		/* A0 */ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		/* B0 */ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		/* C0 */ 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		/* D0 */ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		/* E0 */ 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 7, 7,
		/* F0 */ 9,10,10,10,11, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
		/*       00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F  */ };

	/* state table */
	typedef enum {
		kSTART = 0,kA,kB,kC,kD,kE,kF,kG,kERROR,kNumOfStates } utf8_state;

		static utf8_state state_table[] = {
			/*                            kSTART, kA,     kB,     kC,     kD,     kE,     kF,     kG,     kERROR */
			/* 0x00-0x7F: 0            */ kSTART, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR,
			/* 0x80-0x8F: 1            */ kERROR, kSTART, kA,     kERROR, kA,     kB,     kERROR, kB,     kERROR,
			/* 0x90-0x9f: 2            */ kERROR, kSTART, kA,     kERROR, kA,     kB,     kB,     kERROR, kERROR,
			/* 0xa0-0xbf: 3            */ kERROR, kSTART, kA,     kA,     kERROR, kB,     kB,     kERROR, kERROR,
			/* 0xc0-0xc1, 0xf5-0xff: 4 */ kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR,
			/* 0xc2-0xdf: 5            */ kA,     kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR,
			/* 0xe0: 6                 */ kC,     kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR,
			/* 0xe1-0xec, 0xee-0xef: 7 */ kB,     kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR,
			/* 0xed: 8                 */ kD,     kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR,
			/* 0xf0: 9                 */ kF,     kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR,
			/* 0xf1-0xf3: 10           */ kE,     kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR,
			/* 0xf4: 11                */ kG,     kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR, kERROR };

#define BYTE_CLASS(b) (byte_class_table[(unsigned char)b])
#define NEXT_STATE(b,cur) (state_table[(BYTE_CLASS(b) * kNumOfStates) + (cur)])

			utf8_state current = kSTART;
			int i;

			const char* pt = pTest;
			int len = nLength;

			for(i = 0; i < len ; i++, pt++) 
			{

				current = NEXT_STATE(*pt,current);
				if (kERROR == current)
					break;
			}

			return (current == kSTART) ? TRUE : FALSE;
}


BOOL IsUTF7(const char* pTest,int nLength)
{
	int i;
	const char *pt = pTest;

	for (i = 0; i < nLength; i++) 
	{
		if (*pt & 0x80 || !*pt)
			return FALSE;
		pt++;
	}

	return TRUE;
}
