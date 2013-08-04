//�ַ���ת���� 
// ���ַ�  խ�ַ�  
// ��string խstring  tstring
// 
// 



//������������(hex)ת���ַ���(16����)
//pInstr ��Ҫת�����ַ���, pbyOuthex ת������16�������ݵĴ���ָ��, iInMaxHexLen ��ſռ������С �����������С���ᱻ�ض�
//����ת�������ݵ�ʵ�ʳ��ȣ�  ���������󳤶Ƚ��ᱻ�ض� 
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

//���ַ������������ת��Ϊ��ֵ
int char2int(char ch)
{
	if(ch>='0' && ch<='9') return (char)(ch-'0');
	if(ch>='a' && ch<='f') return (char)(ch-'a'+10);
	if(ch>='A' && ch<='F') return (char)(ch-'A'+10);
	return -1;
}

//��16�����ַ���ת���ɶ���������(hex)
//pbyInHex ���������ݴ���ָ�� iHexLen���ݳ��� pOutStrת����16�����ַ�������ָ�� iMaxStrLen����ַ������ݵ���󳤶� ������󳤶Ƚ��ᱻ�ض�
//����ת�������ݵ�ʵ�ʳ��� ���������󳤶Ƚ��ᱻ�ض� ע�����ת��������Ϊ�ַ�����ע�ⲹ��
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
	BYTE szbyTem[MAX_PATH] = {"Any problem in computer science can be solved by another layer of indirection/�糵"};

	iStrLen = hex2str(szbyTem, strlen((char*)szbyTem), szTem2, MAX_PATH);

	BYTE szbyTem2[MAX_PATH] = {0};
	iStrLen = str2hex(szTem2, szbyTem2, MAX_PATH);

}