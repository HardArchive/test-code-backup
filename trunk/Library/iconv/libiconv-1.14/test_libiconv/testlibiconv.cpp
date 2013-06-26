// testlibiconv.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
//��ǰ��д��һ��gb18030��utf-8����ת���ĳ�����δ��뻹����Щ����ģ���Ϊ��������Ҫ�����������iconv֧�ֵ����Ա���������ת����
//����GB2312, GBK, GB18030, UTF-8, UTF-16, BIG5�ȵȣ����Բ�������γ���ע���Ҳ����ˡ�
//Ŀǰ��δ����Ƿǳ����ȶ��������˳���10���е���ʮ�ֱ�����ı���ת����û�г����⡣  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>

#ifndef ICONV_CONST
# define ICONV_CONST const
#endif
#pragma comment(lib, "libiconv.lib");
/*!
 ���ַ����������Ա���ת��
 param from    ԭʼ���룬����"GB2312",�İ���iconv֧�ֵ�д
 param to      ת����Ŀ�ı���
 param save    ת��������ݱ��浽���ָ�����Ҫ���ⲿ�����ڴ�
 param savelen �洢ת�������ݵ��ڴ��С
 param src     ԭʼ��Ҫת�����ַ���
 param srclen  ԭʼ�ַ�������
 */
int convert(const char *from, const char *to, char* save, int savelen, char *src, int srclen)
{
    iconv_t cd;
    char   *inbuf = src;
    char *outbuf = save;
    size_t outbufsize = savelen;
    int status = 0;
    size_t  savesize = 0;
    size_t inbufsize = srclen;
    const char* inptr = inbuf;
    size_t      insize = inbufsize;
    char* outptr = outbuf;
    size_t outsize = outbufsize;
    
    cd = iconv_open(to, from);
    iconv(cd,NULL,NULL,NULL,NULL);
    if (inbufsize == 0)
	{
        status = -1;
        goto done;
    }
    while (insize > 0)
	{
        size_t res = iconv(cd,(ICONV_CONST char**)&inptr,&insize,&outptr,&outsize);
        if (outptr != outbuf)
		{
            int saved_errno = errno;
            int outsize = outptr - outbuf;
            strncpy(save+savesize, outbuf, outsize);
            errno = saved_errno;
        }
        if (res == (size_t)(-1)) 
		{
            if (errno == EILSEQ)
			{
                int one = 1;
                iconvctl(cd,ICONV_SET_DISCARD_ILSEQ,&one);
                status = -3;
            } 
			else if (errno == EINVAL)
			{
                if (inbufsize == 0)
				{
                    status = -4;
                    goto done;
                } 
				else 
				{
                    break;
                }
            }
			else if (errno == E2BIG) 
			{
                status = -5;
                goto done;
            } 
			else 
			{
                status = -6;
                goto done;
            }
        }
    }
    status = strlen(save);
done:
    iconv_close(cd);
    return status;
}



int _tmain(int argc, _TCHAR* argv[])
{
	char *input = "iconvʵ��ͨ�����Ա���ת�� ";
	size_t len = strlen(input);
	char szOut[260] = {0};
	convert("GBK", "UTF-8", szOut, 260, input, len);

	char *input2 = "RAIN%C0%B2%C0%B2%BA%DA%D2%C2%C8%CB";
	size_t len2 = strlen(input);
	char szOut2[260] = {0};
	convert("UTF-8", "GBK", szOut2, 260, input2, len2);
	printf("%s\n", szOut2);
	scanf("%s", szOut);
	return 0;
}
