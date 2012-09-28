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
#include <Windows.h>

int IsTextUTF8(char* str, ULONGLONG length);
//sina weibo SDK
bool IsUTF8_sina(const char *str);

//Notepad2中的
BOOL IsUnicode(const char* pBuffer,int cb/*,LPBOOL lpbBOM,LPBOOL lpbReverse*/);
BOOL IsUTF8_Notepad2(const char* pTest,int nLength);
BOOL IsUTF7(const char* pTest,int nLength);
