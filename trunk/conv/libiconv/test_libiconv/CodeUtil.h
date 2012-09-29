//file CodeUtil.h
/*************************************************************************************************** 
* 1、 Class      ： CCodeUtil
* 2、 Version    ： 1.1
* 3、 Description： 编码转换辅助类支持Url Unicode、Gb2312、UTF_8
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-7-18 10:30:07
* 6、 History    ： 
* 7、 Remark     ： 支持宽字节和多字节
做一编码转换工具，使用iconv库，支持所有编码转换  
****************************************************************************************************/  
#ifndef __CODE_UNTIL_H__
#define __CODE_UNTIL_H__

#define 

namespace QNA
{
#include "iconv.h"
#pragma comment(lib, "libiconv.lib");
	int covert(char *desc, char *src, const char *input, size_t ilen, char *output, size_t olen)
	{
		const char **pin = &input;
		char **pout = &output;
		iconv_t cd = iconv_open(desc, src);
		if (cd == (iconv_t)-1)
		{
			return -1;
		}
		memset(output, 0, olen);
		if (iconv(cd, pin, &ilen, pout, &olen)) return -1;
		iconv_close(cd);
		return 0;
	}
}
#endif