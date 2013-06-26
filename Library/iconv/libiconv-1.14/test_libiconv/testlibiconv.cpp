// testlibiconv.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//以前我写了一个gb18030到utf-8编码转换的程序，这段代码还是有些问题的，因为现在我需要对任意的两个iconv支持的语言编码做互相转换，
//比如GB2312, GBK, GB18030, UTF-8, UTF-16, BIG5等等，所以才有了这段程序，注释我不加了。
//目前这段代码是非常的稳定，测试了超过10万行的数十种编码的文本的转换都没有出问题。  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>

#ifndef ICONV_CONST
# define ICONV_CONST const
#endif
#pragma comment(lib, "libiconv.lib");
/*!
 对字符串进行语言编码转换
 param from    原始编码，比如"GB2312",的按照iconv支持的写
 param to      转换的目的编码
 param save    转换后的数据保存到这个指针里，需要在外部分配内存
 param savelen 存储转换后数据的内存大小
 param src     原始需要转换的字符串
 param srclen  原始字符串长度
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
	char *input = "iconv实现通用语言编码转换 ";
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
