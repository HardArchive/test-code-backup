//
void CCodeHelper::CheckAndChangeUtf8ToGbk(char* pSrc, int nSrcLen)
{	
	u_char Writebuf[MAX_PATH];
	memcpy( Writebuf, pSrc, sizeof(Writebuf) );
	//检查是否是utf8表示的中文 true:utf8  false:gbk
	if ( check_utf8_chinese(Writebuf, sizeof(Writebuf)) )
	{								
		UTF8ToGB(pSrc, pSrc, nSrcLen);								
	}
	else//检查是否为gbk
	{								
		if (!CCodeHelper::IsGBK(pSrc))
		{
			CCodeHelper::UTF8ToGB(pSrc, pSrc, nSrcLen);									
		}							
	}
}


// 检查字符串是不是用utf8表示的中文 ture:utf8 false:gbk
bool CCodeHelper::check_utf8_chinese(u_char *data, size_t len)  
{  
#define ASCII__MASK     0x80   
#define CHINESE_MASK1   0xF0   
#define CHINESE_MASK2   0xC0   
#define CHINESE_MASK3   0xC0   

#define ASCII_VALUE     0x00   
#define CHINESE_VALUE1  0xE0   
#define CHINESE_VALUE2  0x80   
#define CHINESE_VALUE3  0x80

	u_char ch;  
	size_t i, count, fail;  

	enum {  
		chinese1 = 0,  
		chinese2,  
		chinese3  
	} state;  

	state = chinese1;  
	count = 0;  
	fail = 0;  

	for (i = 0; i < len; i++) 
	{  
		ch = *(data + i);  
		if (ch <= 127) 
		{  
			continue;  
		}  

		switch (state)
		{  
		case chinese1:  
			if (!((ch & CHINESE_MASK1) ^ CHINESE_VALUE1)) 
			{  
				state = chinese2;  
			} 
			else 
			{  
				fail++;  
			}  
			break;  
		case chinese2:  
			if (!((ch & CHINESE_MASK2) ^ CHINESE_VALUE2)) 
			{  
				state = chinese3;  
			} 
			else 
			{  
				state = chinese1;  
				fail++;  
			}  
			break;  
		case chinese3:  
			if (!((ch & CHINESE_MASK3) ^ CHINESE_VALUE3)) 
			{  
				count++;  
			} 
			else 
			{  
				fail++;  
			}  
			state = chinese1;  
		}  
	}  

	if (0 != fail)
	{
		return false;
	}
	return true;
}  


int CCodeHelper::UTF8ToGB( char* pDst, const char* pSrc, const int nSrcLen )
{
	int ret = 0;
	WCHAR* strA;
	int i= MultiByteToWideChar ( CP_UTF8, 0 , pSrc, -1, NULL, 0 );
	if( i<=0 ){
		TracePrint(LOG_DEBUG, "LZSniffer ERROR. FUN: %s, LINE: %d\r\n", __FUNCTION__, __LINE__); return 0;
	}
	strA = (WCHAR*)malloc( i*2 );
	MultiByteToWideChar ( CP_UTF8 , 0 , pSrc, -1, strA , i);
	i= WideCharToMultiByte(CP_ACP,0,strA,-1,NULL,0,NULL,NULL);
	if( nSrcLen >= i ){
		// 如果是UTF8编码，则转换
		ret = WideCharToMultiByte (CP_ACP,0,strA,-1,pDst,i,NULL,NULL);
		pDst[i] = 0;
	}
	else
	{
		// 不是UTF8编码，直接复制输出数据
		if (pSrc != pDst) 
		{
			memcpy(pDst, pSrc, nSrcLen);
			pDst[nSrcLen] = 0;
		}
		else
		{
			// 这是源和目的是同一地址，则不做处理
		}
	}
	if( ret<=0 ){
		free( strA ); return 0;
	}
	free( strA );
	return ret;
}

bool CCodeHelper::IsGBK( char* str )
{	
	bool isgbk=true;
	for (unsigned int i=0;i<strlen(str);i++)
	{
		u_char ch_first=*(str+i);
		if (ch_first>=0x01&&ch_first<=0x7f)
		{
			continue;
		}
		else
		{
			if (ch_first>=0x81&&ch_first<=0xfe)
			{
				u_char ch_second=*(str+i+1);
				if (ch_second>=0x40&&ch_second<=0xfe)
				{
					i++;
					continue;
				}
				else
				{
					isgbk=false;
					break;
				}
			}
			else
			{
				isgbk=false;
				break;
			}
		}
	}
	return isgbk;
}