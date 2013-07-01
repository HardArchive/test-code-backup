//file IconHelp.h
/***************************************************************************************************
* 1、 File       ： safestl.h
* 2、 Version    ： *.*
* 3、 Description： ICONV编码转换辅助函数
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-4-28 15:42:35
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/
#ifndef __ICONV_HELP_UTIL_H__
#define __ICONV_HELP_UTIL_H__

//#include <iconv.h>
#include "iconv/iconv.h"

#ifndef ICONV_CONST
# define ICONV_CONST const
#endif
#pragma comment(lib, "libiconv.lib")

namespace RG
{


	/*!
	对字符串进行语言编码转换
	param from    原始编码，比如"GB2312",的按照iconv支持的写
	param to      转换的目的编码
	param save    转换后的数据保存到这个指针里，需要在外部分配内存
	param savelen 存储转换后数据的内存大小
	param src     原始需要转换的字符串
	param srclen  原始字符串长度
	*/
	inline int Convert(const char *from, const char *to, char* save, int savelen, char *src, int srclen)
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

}
#endif /*__ICONV_HELP_UTIL_H__*/

