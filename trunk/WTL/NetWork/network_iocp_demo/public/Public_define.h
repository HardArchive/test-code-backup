//file Public_define.h

#ifndef __PUBLIC_DEFINE_H__
#define __PUBLIC_DEIFNE_H__
/***************************************************************************************************
* 1、 File       ： Public_define
* 2、 Version    ： *.*
* 3、 Description： 公共头定义
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-11-29 17:26:51
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/

typedef struct MSG_TAG
{
	DWORD dwLen;
	char szContent[1024];
	CHARFORMAT cfFont;
}MSGTAG, *PMSGTAG;

#endif /*__PUBLIC_DEIFNE_H__*/

