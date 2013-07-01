//file IconHelp.h
/***************************************************************************************************
* 1�� File       �� safestl.h
* 2�� Version    �� *.*
* 3�� Description�� ICONV����ת����������
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-4-28 15:42:35
* 6�� History    �� 
* 7�� Remark     �� 
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
	���ַ����������Ա���ת��
	param from    ԭʼ���룬����"GB2312",�İ���iconv֧�ֵ�д
	param to      ת����Ŀ�ı���
	param save    ת��������ݱ��浽���ָ�����Ҫ���ⲿ�����ڴ�
	param savelen �洢ת�������ݵ��ڴ��С
	param src     ԭʼ��Ҫת�����ַ���
	param srclen  ԭʼ�ַ�������
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

