
#include "stdio.h"
#include "reec.h"

#pragma comment(lib,"REEC.lib") 
int main(int argc , char** argv)
{
	//����ʽ��<(a-z)^3>��<(a-z)^*>��<(a-z){2,3}>��<(a-z){2,*}>
	//<(a-z)^3>
	//ƥ��3����ĸ������λ�ò�������
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z)^3>","���a������bͻ��c��Դ����ӭʹ��1.0���ǵĲ�Ʒ��",RS_LEVEL_0));
	//��� abc

	//ƥ��3����ĸ������λ�ñ�������
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z)^3>","���a������bͻ��c��Դ����ӭʹ��1.0���ǵĲ�Ʒ��",RS_LEVEL_1));
	//����������� null

	//<(a-z)^*>
	//ƥ��������ĸ������λ�ò�������
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z)^*>","���a������bͻ��c��Դ����ӭʹ��1.0���ǵĲ�Ʒ��",RS_LEVEL_0));
	//��� abc

	//ƥ��n����ĸ������λ�ñ�������
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z)^*>","���a������bͻ��c��Դ����ӭʹ��1.0���ǵĲ�Ʒ��",RS_LEVEL_1));
	//��� a

	//<(a-z){2,3}>
	//ƥ������2����ĸ������3����ĸ������λ�ò�������
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z){2,3}>","���a������bͻ��c��Դ����eӭdʹf��1.0���ǵ�f��Ʒ��",RS_LEVEL_0));
	//��� abc

	//ƥ������2����ĸ������3����ĸ������λ�ò�������
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z){2,3}>","���a������ͻ�ǿ�Դ����ӭʹ��1.0���ǵĲ�Ʒ��",RS_LEVEL_0));
	//����2����ĸ����� null

	//ƥ������2����ĸ������3����ĸ������λ�ò�������
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z){2,3}>","���a������bͻ��c��Դ����ӭʹ��1.0���ǵĲ�Ʒ��",RS_LEVEL_1));
	//����������� null

	//<(a-z){2,*}>
	//ƥ������2��Сд��ĸ������n��Сд��ĸ������λ�ò�������
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z){2,*}>","���a������bͻA��c��Դ����FӭGʹG��1.0��d��e��f��Ʒ��",RS_LEVEL_0));
	//��� abcdef

	//ƥ������2��Сд��ĸ������n��Сд��ĸ������λ�ò�������
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z){2,*}>","���A������BͻA��C��Դ����FӭGʹG��1.0��D��E��F��Ʒ��",RS_LEVEL_0));
	//��� null

	//ƥ������2����ĸ������n����ĸ������λ�ñ�������
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z){2,*}>","���a������bͻA��c��Դ����FӭGʹG��1.0��d��e��f��Ʒ��",RS_LEVEL_1));
	//��� null

	//1������0������
	//������ƥ���д��ĸ������ʽ��<(A-Z)^3>��<(A-Z)^*>��<(A-Z){2,3}>��<(A-Z){2,*}>
	//<(A-Z)^3>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z)^3>","���a������bͻA��c��Դ����BӭCʹ��1.0��d��e��f��Ʒ��",RS_LEVEL_0));
	//��� ABC

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z)^3>","���a������bͻA��c��Դ����BӭCʹ��1.0��d��e��f��Ʒ��",RS_LEVEL_1));
	//��� null

	//<(A-Z)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z)^*>","���a������bͻA��c��Դ����BӭCʹ��1.0��d��e��f��Ʒ��",RS_LEVEL_0));
	//��� ABC

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z)^*>","���a������bͻA��c��Դ����BӭCʹ��1.0��d��e��f��Ʒ��",RS_LEVEL_1));
	//��� A

	//<(A-Z){2,3}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z){2,3}>","���a������bͻA��c��Դ����BӭCʹD��1.0��d��e��f��Ʒ��",RS_LEVEL_0));
	//��� ABC

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z){2,3}>","���a������bͻA��c��Դ����ӭʹ��1.0��d��e��f��Ʒ��",RS_LEVEL_0));
	//��� null

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z){2,3}>","���a������bͻA��c��Դ����BӭCʹ��1.0��d��e��f��Ʒ��",RS_LEVEL_1));
	//��� null

	//<(A-Z){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z){2,*}>","���a������bͻA��c��Դ����BӭCʹ��1.0��d��e��f��DƷ��",RS_LEVEL_0));
	//��� ABCD

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z){2,*}>","���a������bͻA��c��Դ����ӭʹ��1.0��d��e��f��Ʒ��",RS_LEVEL_0));
	//��� null

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z){2,*}>","���a������bͻA��c��Դ����BӭCʹ��1.0��d��e��f��Ʒ��",RS_LEVEL_1));
	//��� null

	//������ƥ���Сд��ĸ������ʽ��<(a-zA-Z)^3>��<(a-zA-Z)^*>��<(a-zA-Z){2,3}>��<(a-zA-Z){2,*}>
	//<(a-zA-Z)^3>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z)^3>","���a������ͻA��c��Դ����BӭCʹ��1.0��d��e��f��Ʒ��",RS_LEVEL_0));
	//��� aAc

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z)^3>","���a������ͻA��c��Դ����BӭCʹ��1.0��d��e��f��Ʒ��",RS_LEVEL_1));
	//��� null

	//<(a-zA-Z)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z)^*>","���a������ͻA��c��Դ����BӭCʹ��1.0��d��e��f��Ʒ��",RS_LEVEL_0));
	//��� aAcBCdef

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z)^*>","���a������ͻA��c��Դ����BӭCʹ��1.0��d��e��f��Ʒ��",RS_LEVEL_1));
	//��� a

	//<(a-zA-Z){2,3}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z){2,3}>","���a������ͻA��c��Դ����BӭCʹ��1.0��d��e��f��Ʒ��",RS_LEVEL_0));
	//��� aAc

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z){2,3}>","���a������ͻ�ǿ�Դ����ӭʹ��1.0���ǵĲ�Ʒ��",RS_LEVEL_0));
	//��� null

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z){2,3}>","���a������ͻA��c��Դ����BӭCʹ��1.0��d��e��f��Ʒ��",RS_LEVEL_1));
	//��� null

	//<(a-zA-Z){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z){2,*}>","���a������ͻA��c��Դ����BӭCʹ��1.0��d��e��f��Ʒ��",RS_LEVEL_0));
	//��� aAcBCdef

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z){2,*}>","���a������ͻ�ǿ�Դ����ӭʹ��1.0���ǵĲ�Ʒ��",RS_LEVEL_0));
	//��� null

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z){2,*}>","���a������ͻA��c��Դ����BӭCʹ��1.0��d��e��f��Ʒ��",RS_LEVEL_1));
	//��� null

	//������ƥ�����֣�����ʽ��<(0-9)^3>��<(0-9)^*>��<(0-9){2,3}>��<(0-9){2,*}>
	//<(0-9)^3>
	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9)^3>","�������1��2ͻ��3��Դ����ӭʹ��1.0���ǵĲ�Ʒ��",RS_LEVEL_0));
	//��� 123

	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9)^3>","�������1��2ͻ��3��Դ����ӭʹ��1.0���ǵĲ�Ʒ��",RS_LEVEL_1));
	//��� null

	//<(0-9)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9)^*>","�������1��2ͻ��3��Դ����ӭʹ��1.0���ǵĲ�Ʒ��",RS_LEVEL_0));
	//��� 12310

	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9)^*>","�������1��2ͻ��3��Դ����ӭʹ��1.0���ǵĲ�Ʒ��",RS_LEVEL_1));
	//��� 1

	//<(0-9){2,3}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9){2,3}>","�������1��2ͻ��3��Դ����ӭʹ��1.0���ǵĲ�Ʒ��",RS_LEVEL_0));
	//��� 123

	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9){2,3}>","�������1��ͻ�ǿ�Դ����ӭʹ�����ǵĲ�Ʒ��",RS_LEVEL_0));
	//��� null

	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9){2,3}>","�������12��2ͻ��3��Դ����ӭʹ��1.0���ǵĲ�Ʒ��",RS_LEVEL_1));
	//��� 12

	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9){2,3}>","�������1��ͻ�ǿ�Դ����ӭʹ�����ǵĲ�Ʒ��",RS_LEVEL_1));
	//��� null

	//<(0-9){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9){2,*}>","�������1��2ͻ��3��Դ����ӭʹ��1.0����1��2��3Ʒ��",RS_LEVEL_0));
	//��� 12310123

	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9){2,*}>","�������1��ͻ�ǿ�Դ����ӭʹ�����ǵĲ�Ʒ��",RS_LEVEL_1));
	//��� null

	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9){2,3}>","�������12345��ͻ�ǿ�Դ����ӭʹ��1.0����1��2��3Ʒ��",RS_LEVEL_1));
	//��� 123

	printf("%s\n",TOKRegularExpressionEngineRun("<(0-9){2,3}>","�������1��ͻ�ǿ�Դ����ӭʹ��1.0����1��2��3Ʒ��",RS_LEVEL_1));
	//��� null

	//������ƥ�������Լ�Сд��ĸ������ʽ��<(a-z0-9)^3>��<(a-z0-9)^*>��<(a-z0-9){2,3}>��<(a-z0-9){2,*}>
	//<(a-z0-9)^3>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9)^3>","�������12a3��ͻ�ǿ�Դ����ӭʹ��1.0����1��2��3Ʒ��",RS_LEVEL_0));
	//��� 12a

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9)^3>","�������12��aͻ�ǿ�Դ����ӭʹ��1.0����1��2��3Ʒ��",RS_LEVEL_1));
	//��� null

	//<(a-z0-9)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9)^*>","�������12a3��ͻa�ǿ�dԴ����ӭbʹf��1.0��c��1��2��3Ʒ��",RS_LEVEL_0));
	//��� 12a3adbf10c123

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9)^*>","�������a3��ͻa�ǿ�dԴ����ӭbʹf��1.0��c��1��2��3Ʒ��",RS_LEVEL_1));
	//��� a3

	//<(a-z0-9){2,3}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,3}>","�������1a��ͻa�ǿ�dԴ����ӭbʹf��1.0��c��1��2��3Ʒ��",RS_LEVEL_0));
	//��� 1aa

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,3}>","�������1��ͻ�ǿ�Դ����ӭʹ�����ǵĲ�Ʒ��",RS_LEVEL_0));
	//��� null

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,3}>","�������1a��ͻa�ǿ�dԴ����ӭbʹf��1.0��c��1��2��3Ʒ��",RS_LEVEL_1));
	//��� 1a

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,3}>","�������1��aͻa�ǿ�dԴ����ӭbʹf��1.0��c��1��2��3Ʒ��",RS_LEVEL_1));
	//��� null

	//<(a-z0-9){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,*}>","�������12a3��ͻa�ǿ�dԴ����ӭbʹf��1.0��c��1��2��3Ʒ��",RS_LEVEL_0));
	//��� 12a3adbf10c123

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,*}>","�������12A3��ͻB�ǿ�CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_0));
	//��� 12310123

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,*}>","�������1a3��ͻB�ǿ�CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_1));
	//��� 1a3

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,3}>","�������12a3��ͻB�ǿ�CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_1));
	//��� 12a

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-z0-9){2,3}>","�������1��aͻB�ǿ�CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_1));
	//��� null

	//������ƥ�������Լ���д��ĸ������ʽ��<(A-Z0-9)^3>��<(A-Z0-9)^*>��<(A-Z0-9){2,3}>��<(A-Z0-9){2,*}>
	//<(A-Z0-9)^3>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9)^3>","�������12A3��ͻB�ǿ�CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_0));
	//��� 12A

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9)^3>","�������1A��ͻB�ǿ�CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_1));
	//��� null

	//<(A-Z0-9)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9)^*>","�������1a2A3��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_0));
	//��� 12A3BCDE10F123

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9)^*>","�������1a2A3��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_1));
	//��� 1

	//<(A-Z0-9){2,3}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,3}>","�������1a2A3��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_0));
	//��� 12A

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,3}>","�������1a23��eͻ��f��Դ����ӭʹ��1.0��F��1��2��3Ʒ��",RS_LEVEL_0));
	//��� 123

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,3}>","�������1A23��eͻ��f��Դ����ӭʹ��1.0��F��1��2��3Ʒ��",RS_LEVEL_1));
	//��� 1A2

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,3}>","�������1a23��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_1));
	//��� 23

	//<(A-Z0-9){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,*}>","�������1a2A3��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_0));
	//��� 12A3BCDE10F123

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,*}>","�������1a23��eͻ��f��Դ����ӭʹ��1.0��F��1��2��3Ʒ��",RS_LEVEL_0));
	//��� 12310F123

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,*}>","�������1aA3��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_1));
	//��� A3

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,3}>","�������1aA123��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_1));
	//��� A12

	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z0-9){2,3}>","�������1a23��eͻ��f��Դ����ӭʹ��1.0��F��1��2��3Ʒ��",RS_LEVEL_1));
	//��� 23

	//������ƥ�������Լ���Сд��ĸ������ʽ��<(a-zA-Z0-9)^3>��<(a-zA-Z0-9)^*>��<(a-zA-Z0-9){2,3}>��<(a-zA-Z0-9){2,*}>
	//<(a-zA-Z0-9)^3>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9)^3>","�������1aA3��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_0));
	//��� 1aA

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9)^3>","�������1a��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_1));
	//��� null

	//<(a-zA-Z0-9)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9)^*>","�������1a2A3��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_0));
	//��� 1a2A3eBfCDE10F123

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9)^*>","�������1a2A3��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_1));
	//��� 1a2A3

	//<(a-zA-Z0-9){2,3}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,3}>","�������1a2A3��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_0));
	//��� 1a2

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,3}>","���������ͻ�ǿ�Դ����ӭʹ�����ǵĲ�3Ʒ��",RS_LEVEL_0));
	//��� null

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,3}>","�������1a2A3��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_1));
	//��� 1a2

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,3}>","�������1��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_1));
	//��� null

	//<(a-zA-Z0-9){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,*}>","�������1a2A3��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_0));
	//��� 1a2A3eBfCDE10F123

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,*}>","���������ͻ�ǿ�Դ����ӭʹ�����ǵĲ�3Ʒ��",RS_LEVEL_0));
	//��� null

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,*}>","�������1a2A3��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_1));
	//��� 1a2A3

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,3}>","�������1a2A3��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_1));
	//��� 1a2

	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z0-9){2,3}>","�������1��eͻB��f��CԴ����ӭDʹE��1.0��F��1��2��3Ʒ��",RS_LEVEL_1));
	//��� null

	//������ƥ�������Լ���Сд��ĸ������ʽ��<(symbol)^3>��<(symbol)^*>��<(symbol){2,3}>��<(symbol){2,*}>
	//<(ͻ�ǿ�Դ)^3>
	printf("%s\n",TOKRegularExpressionEngineRun("<(ͻ�ǿ�Դ)^3>","ͻ�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���ͻ�ǿ�Դ��",RS_LEVEL_0));
	//��� ͻ�ǿ�Դͻ�ǿ�Դͻ�ǿ�Դ

	printf("%s\n",TOKRegularExpressionEngineRun("<(ͻ�ǿ�Դ)^3>","ͻ�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���ͻ�ǿ�Դ��",RS_LEVEL_1));
	//��� null

	//<(ͻ�ǿ�Դ)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(ͻ�ǿ�Դ)^*>","ͻ�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���ͻ�ǿ�Դ��",RS_LEVEL_0));
	//��� ͻ�ǿ�Դͻ�ǿ�Դͻ�ǿ�Դͻ�ǿ�Դͻ�ǿ�Դ

	printf("%s\n",TOKRegularExpressionEngineRun("<(ͻ�ǿ�Դ)^*>","ͻ�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���ͻ�ǿ�Դ��",RS_LEVEL_1));
	//��� ͻ�ǿ�Դ

	//<(ͻ�ǿ�Դ){2,3}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(ͻ�ǿ�Դ){2,3}>","ͻ�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���ͻ�ǿ�Դ��",RS_LEVEL_0));
	//��� ͻ�ǿ�Դͻ�ǿ�Դͻ�ǿ�Դ

	printf("%s\n",TOKRegularExpressionEngineRun("<(ͻ�ǿ�Դ){2,3}>","ͻa�ǿ�Դ����ӭʹ��ͻ��d��Դ��Ʒ����ӭ����ͻd�ǿ�Դ��ͻ��d��Դ�����ڻ�������������о���ͻ��d��Դ��",RS_LEVEL_0));
	//��� null

	printf("%s\n",TOKRegularExpressionEngineRun("<(ͻ�ǿ�Դ){2,3}>","ͻ�ǿ�Դͻ�ǿ�Դͻ�ǿ�Դͻ�ǿ�Դ����ӭʹ�ã���ӭ������ͻ�ǿ�Դ�����ڻ�������������о���",RS_LEVEL_1));
	//��� ͻ�ǿ�Դͻ�ǿ�Դͻ�ǿ�Դ

	printf("%s\n",TOKRegularExpressionEngineRun("<(ͻ�ǿ�Դ){2,3}>","ͻ�ǿ�Դͻ�ǿ�Դͻ�ǿ�Դͻ�ǿ�Դ����ӭʹ�ã���ӭ������ͻ�ǿ�Դ�����ڻ�������������о���",RS_LEVEL_1));
	//��� ͻ�ǿ�Դͻ�ǿ�Դͻ�ǿ�Դ

	//<(ͻ�ǿ�Դ){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(ͻ�ǿ�Դ){2,*}>","ͻ�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���ͻ�ǿ�Դ��",RS_LEVEL_0));
	//��� ͻ�ǿ�Դͻ�ǿ�Դͻ�ǿ�Դͻ�ǿ�Դͻ�ǿ�Դ

	printf("%s\n",TOKRegularExpressionEngineRun("<(ͻ�ǿ�Դ){2,*}>","ͻa�ǿ�Դ����ӭʹ��ͻ��d��Դ��Ʒ����ӭ����ͻd�ǿ�Դ��ͻ��d��Դ�����ڻ�������������о���ͻ��d��Դ��",RS_LEVEL_0));
	//��� null

	printf("%s\n",TOKRegularExpressionEngineRun("<(ͻ�ǿ�Դ){2,*}>","��ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���",RS_LEVEL_1));
	//��� null

	printf("%s\n",TOKRegularExpressionEngineRun("<(ͻ�ǿ�Դ){2,3}>","ͻ�ǿ�Դͻ�ǿ�Դͻ�ǿ�Դͻ�ǿ�Դ����ӭʹ�ã���ӭ������ͻ�ǿ�Դ�����ڻ�������������о���",RS_LEVEL_1));
	//��� ͻ�ǿ�Դͻ�ǿ�Դͻ�ǿ�Դ

	printf("%s\n",TOKRegularExpressionEngineRun("<(ͻ�ǿ�Դ){2,3}>","ͻ�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���ͻ�ǿ�Դ��",RS_LEVEL_1));
	//��� null

	//������ƥ�������ַ�������ʽ��<(*)^4>��<(*)^*>��<(*){2,4}>��<(*){2,*}>
	//<(*)^4>
	printf("%s\n",TOKRegularExpressionEngineRun("<(*)^4>","ͻ�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���ͻ�ǿ�Դ��",RS_LEVEL_0));
	//��� ͻ��

	printf("%s\n",TOKRegularExpressionEngineRun("<(*)^4>","ͻ",RS_LEVEL_1));
	//��� null

	//<(*)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(*)^*>","ͻ�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���ͻ�ǿ�Դ��",RS_LEVEL_0));
	//��� ͻ�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���ͻ�ǿ�Դ��

	printf("%s\n",TOKRegularExpressionEngineRun("<(*)^*>","ͻ�ǿ�Դ",RS_LEVEL_1));
	//��� ͻ�ǿ�Դ

	//<(*){2,4}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(*){2,4}>","ͻ�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���ͻ�ǿ�Դ��",RS_LEVEL_0));
	//��� ͻ��

	printf("%s\n",TOKRegularExpressionEngineRun("<(*){4,6}>","ͻ",RS_LEVEL_0));
	//��� null

	//<(*){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(*){2,*}>","ͻ�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���ͻ�ǿ�Դ��",RS_LEVEL_0));
	//��� ͻ�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���ͻ�ǿ�Դ��

	printf("%s\n",TOKRegularExpressionEngineRun("<(*){4,*}>","ͻ",RS_LEVEL_0));
	//��� null

	//<(*){2,*}>
	printf("%s\n",TOKRegularExpressionEngineRun("<(*){2,*}>","ͻ�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���ͻ�ǿ�Դ��",RS_LEVEL_0));
	//��� ͻ�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���ͻ�ǿ�Դ��

	printf("%s\n",TOKRegularExpressionEngineRun("<(*){4,*}>","ͻ",RS_LEVEL_0));
	//��� null

	//�ۺϲ��ԣ�
	//<(A-Z)^2><(a-z)^3>
	printf("%s\n",TOKRegularExpressionEngineRun("<(A-Z)^2><(a-z)^3>","ͻTOKSmart Compiler�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���",RS_LEVEL_1));
	//�����KSmar

	//[<(0-9)^5>]
	printf("%s\n",TOKRegularExpressionEngineRun("[<(0-9)^5>]","ͻ�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����9.00ӭ����ͻ�ǿ�Դ��ͻ��123345��Դ�����ڻ�������������о���ͻ�ǿ�Դ��",RS_LEVEL_0));
	//�����90012

	//[<(0-9)^5>]
	printf("%s\n",TOKRegularExpressionEngineRun("[<(0-9)^5>]","ͻ�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����9.00ӭ����ͻ�ǿ�Դ��ͻ��123345��Դ�����ڻ�������������о���ͻ�ǿ�Դ��",RS_LEVEL_1));
	//�����12334

	//<(a-zA-Z)^6>
	printf("%s\n",TOKRegularExpressionEngineRun("<(a-zA-Z)^6>","ͻTOKSmart Compiler�ǿ�Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���",RS_LEVEL_1));
	//�����TOKSma

	//<(ͻ��)^1><(0-9)^*>
	printf("%s\n",TOKRegularExpressionEngineRun("<(ͻ��)^1><(0-9)^*>","ͻ��2008��Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���",RS_LEVEL_1));
	//�����ͻ��2008

	//[<(ͻ��)^1><(0-9){3,6}>][<(0-9){3,6}>]
	printf("%s\n",TOKRegularExpressionEngineRun("[<(ͻ��)^1><(0-9){3,6}>][<(0-9){3,6}>]","ͻ��2008��Դ����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���",RS_LEVEL_1));
	//�����2008

	//[<(ͻ�ǿ�Դ)^1><(���)^1><(0-9)^*>][<(���)^1><(0-9)^*>][<(0-9)^*>]
	printf("%s\n",TOKRegularExpressionEngineRun("[<(ͻ�ǿ�Դ)^1><(���)^1><(0-9)^*>][<(���)^1><(0-9)^*>][<(0-9)^*>]","ͻ�ǿ�Դ���2008����ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���",RS_LEVEL_1));
	//�����2008

	//[<(ͻ�ǿ�Դ)^1>|<(��Դͻ��)^1><(0-9)^*>][<(ͻ��)^1>|<(��Դ)^1><(0-9)^*>][<(0-9)^*>]
	printf("%s\n",TOKRegularExpressionEngineRun("[<(ͻ�ǿ�Դ)^1>|<(��Դͻ��)^1><(0-9)^*>][<(ͻ��)^1>|<(���)^1><(0-9)^*>][<(0-9)^*>]","ͻ�Ǻ�2008����Դͻ��2008��ӭʹ��ͻ�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���",RS_LEVEL_1));
	//�����2008

	//<(http://)^1><(www.)^1><(a-z)^*>|<(A-Z)^*>|<(0-9)^*><(.com)^1>|<(.net)^1>|<(.cc)^1>
	printf("%s\n",TOKRegularExpressionEngineRun("<(http://)^1><(www.)^1><(a-z)^*>|<(A-Z)^*>|<(0-9)^*><(.cc)^1>|<(.com)^1>|<(.net)^1>","ͻ�ǿ�Դ���2008����http://www.gtkings.cӭʹ��ͻhttp://www.tok.cc�ǿ�Դ��Ʒ����ӭ����ͻ�ǿ�Դ��ͻ�ǿ�Դ�����ڻ�������������о���",RS_LEVEL_1));
	//�����http://www.tok.cc


	return 0 ;
}
