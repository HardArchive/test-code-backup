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
#include <Windows.h>

int IsTextUTF8(char* str, ULONGLONG length);
//sina weibo SDK
bool IsUTF8_sina(const char *str);

//Notepad2�е�
BOOL IsUnicode(const char* pBuffer,int cb/*,LPBOOL lpbBOM,LPBOOL lpbReverse*/);
BOOL IsUTF8_Notepad2(const char* pTest,int nLength);
BOOL IsUTF7(const char* pTest,int nLength);
