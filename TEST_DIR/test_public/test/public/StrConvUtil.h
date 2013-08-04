//字符串转换类 
// 宽字符  窄字符  
// 宽string 窄string  tstring
// 
// 



//将二进制数据(hex)转成字符串(16进制)
//pInstr 需要转化的字符串, pbyOuthex 转换后存放16进制数据的传出指针, iInMaxHexLen 存放空间的最大大小 如果超过最大大小将会被截断
//返回转换后数据的实际长度，  如果超过最大长度将会被截断 
int hex2str(BYTE* pbyInHex, int iHexLen, char* pOutStr, int iMaxStrLen)
{
	const char* cHex = "0123456789ABCDEF";
	int i = 0;
	for(int j = 0; j < iHexLen; j++)
	{
		unsigned int a = (unsigned int) pbyInHex[j];
		if (i+1>=iMaxStrLen) break;;
		pOutStr[i++] = cHex[(a & 0xf0) >> 4];
		pOutStr[i++] = cHex[(a & 0x0f)];
	}
	pOutStr[i] = '\0';
	return i;
}

//将字符所代表的数字转化为数值
int char2int(char ch)
{
	if(ch>='0' && ch<='9') return (char)(ch-'0');
	if(ch>='a' && ch<='f') return (char)(ch-'a'+10);
	if(ch>='A' && ch<='F') return (char)(ch-'A'+10);
	return -1;
}

//将16进制字符串转换成二进制数据(hex)
//pbyInHex 二进制数据传入指针 iHexLen数据长度 pOutStr转换的16进制字符串传出指针 iMaxStrLen存放字符串数据的最大长度 超过最大长度将会被截断
//返回转换后数据的实际长度 如果超过最大长度将会被截断 注意如果转出的数据为字符串需注意补零
int str2hex(char* pInstr, BYTE* pbyOuthex, int iInMaxHexLen)
{
	int i=0;
	int iTem = strlen(pInstr);
	for(int j = 0; j < iTem; )
	{
		if (i+1 >= iInMaxHexLen) break;
		unsigned int a =  char2int(pInstr[j++]);
		unsigned int b =  char2int(pInstr[j++]);
		pbyOuthex[i++] = (a << 4) | b;
	}
	return i;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int iStrLen = 0;
	BYTE szbyTem1[MAX_PATH] = {0};
	char szTem[MAX_PATH] = {"00000001140000c800000020009050ffffffff"};
	iStrLen = str2hex(szTem, szbyTem1, MAX_PATH);

	char szTem2[MAX_PATH] = {0};
	BYTE szbyTem[MAX_PATH] = {"Any problem in computer science can be solved by another layer of indirection/风车"};

	iStrLen = hex2str(szbyTem, strlen((char*)szbyTem), szTem2, MAX_PATH);

	BYTE szbyTem2[MAX_PATH] = {0};
	iStrLen = str2hex(szTem2, szbyTem2, MAX_PATH);

}