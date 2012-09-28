#include "stdafx.h"
#include "IsUTF_8.h"

//UTF8是以8bits即1Bytes为编码的最基本单位，当然也可以有基于16bits和32bits的形式，分别称为UTF16和UTF32，但目前用得不多，
// 而UTF8则被广泛应用在文件储存和网络传输中。
//编码原理
//先看这个模板：
//UCS-4 range (hex.)	UTF-8 octet sequence (binary)
//0000 0000-0000 007F	0xxxxxxx
//0000 0080-0000 07FF	110xxxxx 10xxxxxx
//0000 0800-0000 FFFF	1110xxxx 10xxxxxx 10xxxxxx
//0001 0000-001F FFFF	11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
//0020 0000-03FF FFFF	111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
//0400 0000-7FFF FFFF	1111110x 10xxxxxx ... 10xxxxxx
//编码步骤：
//1) 首先确定需要多少个8bits(octets)
//2) 按照上述模板填充每个octets的高位bits
//3) 把字符的bits填充至x中，字符顺序：低位→高位，UTF8顺序：最后一个octet的最末位x→第一个octet最高位x
//根据UTF8编码,最多可由6个字节组成,所以UTF8是1-6字节编码组成


int IsTextUTF8(char* str, ULONGLONG length)
{
	int i = 0;
	DWORD nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
	UCHAR chr = 0;
	BOOL bAllAscii = TRUE; //如果全部都是ASCII, 说明不是UTF-8
	for(i=0; i<length; i++)
	{
		chr= *(str+i);
		// 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
		if((chr&0x80) != 0) 
			bAllAscii= FALSE;
		//如果不是ASCII码,应该是多字节符,计算字节数
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
		else //多字节符的非首字节,应为 10xxxxxx
		{
			if( (chr&0xC0) != 0x80 )
			{
				return FALSE;
			}
			nBytes--;
		}
	}
	if( nBytes > 0 ) //违返规则
	{
		return FALSE;
	}
	if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8
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

//Notepad2中的


BOOL IsUnicode(const char* pBuffer,int cb/*,LPBOOL lpbBOM,LPBOOL lpbReverse*/)
{
	int i = 0xFFFF;

	BOOL bIsTextUnicode;

	BOOL bHasBOM;
	BOOL bHasRBOM;

	if (!pBuffer || cb < 2)
		return FALSE;
	//DWORD IsTextUnicode(CONST PVOID pvBuffer, int cb,PINT pResult);
	//文本文件存在的问题是，它们的内容没有严格和明确的规则，因此很难确定该文件是包含ANSI字符还是Unicode字符。
	// IsTextUnicode使用一系列统计方法和定性方法，以便猜测缓存的内容。由于这不是一种确切的科学方法，因此IsTextUnicode有可能返回不正确的结果。
	//第一个参数pvBuffer用于标识要测试的缓存的地址。该数据是个无效指针，因为你不知道你拥有的是ANSI字符数组还是Unicode字符数组。
	//第二个参数cb用于设定pvBuffer指向的字节数。同样，由于你不知道缓存中放的是什么，因此cb是个字节数，而不是字符数。
	//	请注意，不必设定缓存的整个长度。当然，IsTextUnicode能够测试的字节越多，得到的结果越准确。
	//第三个参数pResult是个整数的地址，必须在调用IsTextUnicode之前对它进行初始化。
	//	对该整数进行初始化后，就可以指明你要IsTextUnicode执行哪些测试。也可以为该参数传递NULL，
	//	在这种情况下，IsTextUnicode将执行它能够进行的所有测试（详细说明请参见PlatformSDK文档）。
	//如果IsTextUnicode认为缓存包含Unicode文本，便返回TRUE，否则返回FALSE。
	//	确实是这样，尽管Microsoft将该函数的原型规定为返回DWORD，但是它实际上返回一个布尔值。
	//	如果在pResult参数指向的整数中必须进行特定的测试，该函数就会在返回之前设定整数中的信息位，以反映每个测试的结果。

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
