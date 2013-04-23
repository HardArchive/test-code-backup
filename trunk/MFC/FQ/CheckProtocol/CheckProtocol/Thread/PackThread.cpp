#include "StdAfx.h"
#include "PackThread.h"
#include "../Dialog/LoginTestDlg.h"
#include "../Dialog/FindCaptureRuleDlg.h"

PackRuleFile CPackThread::m_PackRuleInfo;
pcap_t  *CPackThread::m_pRecvHandle = NULL;
pcap_if_t *CPackThread::m_pRecvAdapterInfo;
multimap<string,INTERCEPTEDINFO> CPackThread::m_MulMapInterceptedInfo;

char  CPackThread::m_strLocalIP[16];

CPackThread::CPackThread(void)
{
	memset(m_strLocalIP,0,sizeof(m_strLocalIP));
	memset(m_strSaveGetOrPostPackHead,0,sizeof(m_strSaveGetOrPostPackHead));
	if(!GetAdapterInfo())
		return;
	memset(&CPackThread::m_pRecvAdapterInfo,0,sizeof(CPackThread::m_pRecvAdapterInfo));

	calLoginSate = CLoginTestDlg::UpdatePackState;                                      //Xt: 回调对话框抓包状态"OK"
	calAddPackListInfo = CCaptruePackInfoDlg::UpdateNotice;                             //Xt: 
	callFindDlgStopCaptrue =  CFindCaptureRuleDlg::_CallStopCapture;      
	callLoginDlgStopCaptrue = CLoginTestDlg::_CallStopCapture; 
}

CPackThread::~CPackThread(void)
{
	Close();
}

bool CPackThread::Initialization()
{
	if(!OpenPacp())
		return false;
	return true;
}

void CPackThread::WorkThread(LPVOID pParam)
{
	int nOpertionType = *((int *)(pParam));	

	if(CAPTURE_INLINE_PACK == (nOpertionType & CAPTURE_INLINE_PACK))                    //Xt:捕获在线信息
	{
		OnLineCapturePack(nOpertionType);
	}

	if(CAPTURE_OFFLINE_PACK == (nOpertionType & CAPTURE_OFFLINE_PACK))                  //Xt: 捕获离线信息
	{
		if(CGlobalData::m_vtOfflinePackPath.empty())
		{
			CGlobalData::PrintLogInfo("CPackThrea","离线包路径错误!",'E');
			return;
		}
		else
		{
			for(int i = 0 ;i<(CGlobalData::m_vtOfflinePackPath).size();++i)
			{
				OffLineCapturePack(nOpertionType,CGlobalData::m_vtOfflinePackPath[i].c_str());    
			}
			if(CLoginTestDlg::m_pThis)
			{
				this->callLoginDlgStopCaptrue();
			}
			if(CFindCaptureRuleDlg::m_pThis)
			{
				//CFindCaptureRuleDlg::_CallStopCapture();
				this->callFindDlgStopCaptrue();
			}
		}
	}
}

bool CPackThread::GetAdapterInfo()
{
	pcap_if_t *pAdapterinfo = NULL;
	pcap_if_t *pAlldevs = NULL;
	char errbuf[PCAP_ERRBUF_SIZE] = {0};             //获得错误信息


	/* 获取本地机器设备列表 */
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &pAdapterinfo, errbuf) == -1)
	{
		CGlobalData::PrintLogInfo("CPacpPac","获取网信息卡错误",GetLastError());
		return true;
	} 

	if(!CGlobalData::m_vectAdapterName.empty())
	{
		CGlobalData::m_vectAdapterName.clear();
	}

	if(!CGlobalData::m_mapAdapterInfo.empty())
	{
		CGlobalData::m_mapAdapterInfo.clear();
	}
  
	/* 打印列表 */
	for(pAlldevs= pAdapterinfo; pAlldevs != NULL; pAlldevs= pAlldevs->next)
	{
		if (pAlldevs->description)
		{
//          printf(" (%s)\n", pAlldevs->description);
// 			printf(" 网卡IP地址：(%s)\n", inet_ntoa(((sockaddr_in *)(pAlldevs->addresses->addr))->sin_addr));
// 			printf(" 网卡子网掩码：(%s)\n", inet_ntoa(((sockaddr_in *)(pAlldevs->addresses->netmask))->sin_addr));
// 			printf(" 广播地址：(%s)\n", inet_ntoa(((sockaddr_in *)(pAlldevs->addresses->broadaddr))->sin_addr));
			CGlobalData::m_vectAdapterName.push_back(pAlldevs->name);
			CGlobalData::m_mapAdapterInfo.insert(make_pair(pAlldevs->name,pAlldevs));
		}
	}

	if(CGlobalData::m_vectAdapterName.empty())
	{
		CGlobalData::PrintLogInfo("CPackThread","网卡信息为空!",'E');
		return false;
	}
	return true;
}

bool CPackThread::SetAdapterInfo(string strAdapterName)
{
	if(strAdapterName.empty())	
	{
		CGlobalData::PrintLogInfo("CPackThread","没有选择网卡信息!",'E');
		return false;
	}
	map <string,pcap_if_t *>::iterator pMapAdapter = CGlobalData::m_mapAdapterInfo.find(strAdapterName);
	if( pMapAdapter == CGlobalData::m_mapAdapterInfo.end() )
	{
		CGlobalData::PrintLogInfo("CPackThread","网卡名错误!没有可选的网卡信息!",'E');
		return false;
	}
	else
	{
		m_pRecvAdapterInfo = pMapAdapter->second;
	    memcpy(m_strLocalIP,inet_ntoa(((sockaddr_in *)(m_pRecvAdapterInfo->addresses->addr))->sin_addr),sizeof(m_strLocalIP));
	}
	return true;
}

bool CPackThread::OpenPacp()
{
	if(!CPackThread::m_pRecvAdapterInfo)
	{
		CGlobalData::PrintLogInfo("CPackThread","没有设置网卡信息!",'E');
		return false;
	}

	char errbuf[PCAP_ERRBUF_SIZE] = {0};
	if ( (CPackThread::m_pRecvHandle= pcap_open((m_pRecvAdapterInfo->name+8),          // 设备名
		65536,                        // 要捕捉的数据包的部分 
		//                               65535保证能捕获到不同数据链路层上的每个数据包的全部内容
		PCAP_OPENFLAG_PROMISCUOUS,    // 混杂模式
		1,                            // 读取超时时间
		NULL,                         // 远程机器验证
		errbuf                        // 错误缓冲池
		) ) == NULL)
	{
	    pcap_freealldevs(m_pRecvAdapterInfo);                                   //关闭上次的连接设备 
		CGlobalData::PrintLogInfo("CPackThread","打开设备适配器错误!",'E');                                     //Xt:关闭设备列表
		return false;
	}

	pcap_freealldevs(m_pRecvAdapterInfo);

	char filterStr[4096]={0};
	sprintf_s(filterStr, 4096, "(tcp) or (udp) or (ether proto 0x8864) or (ether proto 0x8100)");

	u_int netmask=0xffffff;                                                                //子网设成255.255.255.0
	struct bpf_program fcode;

	if (pcap_compile(CPackThread::m_pRecvHandle, &fcode, filterStr, 1, netmask) < 0)
	{
		CGlobalData::PrintLogInfo("CPackThread","设置网络抓包过滤信息错误!",'E');
		return false;
	}

	if (pcap_setfilter(CPackThread::m_pRecvHandle, &fcode) < 0)
	{
		CGlobalData::PrintLogInfo("CPackThread","过滤器与内核捕获关联错误!",'E');
		return false;
	}

	if (pcap_setbuff(CPackThread::m_pRecvHandle, MOMERYSIZE) != 0)
	{
		CGlobalData::PrintLogInfo("CPackThread","设置缓冲区失败!",GetLastError());
		return false;
	}
	return true;
}

int  CPackThread::Decode(pack_info &pi)
{
	// 解析以太网数据
	ethernet_header* eth = (ethernet_header*)pi.pPackBuff;

	// 修改数据头部信息
	pi.PackHeadLength = sizeof(ethernet_header);                    // 从当前指针开始到结束的长度
	memcpy(pi.DestMac,eth->DestMac,6);
	memcpy(pi.SrcMac ,eth->SrcMac ,6);
	switch(eth->Type)
	{
	case ARP_Portocol_Type:
		return DECODE_DEFAULT;
		break;
	case IP_Portocol_Type:
		// 直接就是IP数据
		return DecodeIP(pi);

		break;
	case PPPoE_Protocol_Type:
		// PPPoE数据
		//return DecodePPPoE(di);
		break;
	case VLan_Protocol_Type:
		// VLAN数据
		//return DecodeVlan(di);
		break;
	default:
		return DECODE_DEFAULT;
		break;
	}
	return DECODE_DEFAULT;
}

int  CPackThread::DecodeIP(pack_info &pi )
{
	ip_header* ip = (ip_header*) (pi.pPackBuff+pi.PackHeadLength);
	int nHeadLen = 4 * ip->ip_hl;
	pi.PackHeadLength +=nHeadLen;
	pi.PackContentLength = pi.PackLength - pi.PackHeadLength;
	pi.ip_src = ip->ip_src;
	pi.ip_dst = ip->ip_dst;

	// 根据协议类型确定处理函数
	switch(ip->proto)
	{
	case 1:
		// ICMP协议
		break;
	case 2:
		// IGMP协议
	case 6:
		// TCP协议
		return DecodeTcp(pi);
		break;
	case 17:
		// UDP协议
		//return DecodeUdp(pi);
		break;
	}
	return DECODE_DEFAULT;
}

bool CPackThread::IsReservationIP( in_addr ip, bool bIsNetStream)
{
	if (bIsNetStream)
	{
		return (ip.S_un.S_un_b.s_b1 == 10) || (ip.S_un.S_un_b.s_b1 == 192 && ip.S_un.S_un_b.s_b2 == 168) || (ip.S_un.S_un_b.s_b1 == 172 && ip.S_un.S_un_b.s_b2 >= 16 && ip.S_un.S_un_b.s_b2 <= 31);
	}
	else
	{
		return (ip.S_un.S_un_b.s_b4 == 10) || (ip.S_un.S_un_b.s_b4 == 192 && ip.S_un.S_un_b.s_b3 == 168) || (ip.S_un.S_un_b.s_b4 == 172 && ip.S_un.S_un_b.s_b3 >= 16 && ip.S_un.S_un_b.s_b3 <= 31);
	}
}

int  CPackThread::DecodeTcp( pack_info &pi )
{
	tcp_header* tcp = (tcp_header*)(pi.pPackBuff+pi.PackHeadLength);
	pi.PackHeadLength += (tcp->off)<<2;
	pi.PackContentLength = pi.PackLength - pi.PackHeadLength;

	pi.SrcPort = ntohs(tcp->sport);
	pi.DestPort = ntohs(tcp->dport);
	pi.PackType = PACKE_TYPE_TCP;

	pi.pPackContentBuff = (pi.pPackBuff+pi.PackHeadLength);
	if(!memcmp(inet_ntoa(pi.ip_src),m_strLocalIP,strlen(m_strLocalIP)))
	{
		memcpy(pi.PackDirection,"OUT",sizeof("OUT")); 
	}
	else
	{
		memcpy(pi.PackDirection,"IN",sizeof("IN"));
	}	
	return DECODE_DEFAULT;
}

int  CPackThread::DecodeUdp( pack_info &pi )
{
	udp_header* udp = (udp_header*)(pi.pPackBuff+pi.PackHeadLength);
	pi.PackHeadLength += sizeof(udp_header);
	pi.PackContentLength = pi.PackLength - pi.PackHeadLength;

	pi.SrcPort = ::ntohs(udp->sport);
	pi.DestPort = ::ntohs(udp->dport);
	pi.PackType = PACKE_TYPE_UDP;

	pi.pPackContentBuff = (pi.pPackBuff+pi.PackHeadLength);
	if(!memcmp(inet_ntoa(pi.ip_src),m_strLocalIP,strlen(m_strLocalIP)))
	{
		memcpy(pi.PackDirection,"OUT",sizeof("OUT")); 
	}
	else
	{
		memcpy(pi.PackDirection,"IN",sizeof("IN"));
	}
	return DECODE_DEFAULT;
}

u_short CPackThread::Checksum( const u_short* p, const int nLen )
{
	int count, sum = 0;

	for (count = nLen; --count >= 0; )
	{
		sum += *p++;
	}
	while (sum > 0xffff)
	{
		sum = (sum & 0xffff) + (sum >> 16);
	}
	sum = ~sum & 0xffff;

	return sum;
}

u_short CPackThread::TcpChecksum( const ip_header* p )
{
	tcp_header* pTcpHeader = (tcp_header*)( p+1 );
	u_short *pusPtr, usLen;
	ULONG ulTcpCksum = 0;
	int i;

	pusPtr = ( u_short * )(&p->ip_src);
	// 2 * IP_ALEN octets = IP_ALEN shorts..., they are in octets
	for( i = 0; i < 4; ++i )
	{
		ulTcpCksum += (*pusPtr++);
	}

	pusPtr = (u_short *)pTcpHeader;
	usLen = ntohs( p->tlen ) - sizeof(ip_header);

	ulTcpCksum += htons( 6 + usLen );

	if( usLen % 2 )
	{
		( ( PBYTE )pTcpHeader )[usLen] = 0;		// pad
		usLen += 1;							// for the following division
	}

	// get len in short
	usLen >>= 1;	

	for( i = 0; i < usLen; ++i )
	{
		ulTcpCksum += (*pusPtr++);
	}

	ulTcpCksum = ( ulTcpCksum >> 16 ) + ( ulTcpCksum & 0xffff );
	ulTcpCksum += ( ulTcpCksum >> 16 );

	return u_short(~ulTcpCksum & 0xffff);
}

void  CPackThread::Close()
{
	CloseThread();
}

bool  CPackThread::GetLocalIP()
{
	char strLocalName[256] = {0};
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,1),&wsaData)) //调用Windows Sockets DLL
	{ 
		printf("Winsock无法初始化!\n");
		WSACleanup();
		return false;
	}

	::gethostname(strLocalName,sizeof(strLocalName));
	if(!strLocalName)
		return false;

	hostent *pLocalInfo = ::gethostbyname(strLocalName);
	if(!pLocalInfo)
		return false;

	in_addr addr;
	for(int i = 0;;i++)
	{
		char *p = pLocalInfo->h_addr_list[i];
		if(NULL == p)
			break;
		memcpy(&addr.S_un.S_addr,p,pLocalInfo->h_length);
	}
	return true;
}


bool  CPackThread::GetResolvePackInfo(pack_info *pPackInfo)
{
	static int nMark = 0;
	static int nSeparateNum = 0;

	static INTERCEPTEDINFO iio;
 	static PACKRULEFILE *pCoyPackRuleFile = NULL;


	if((nMark != PACK_SEPARATE_NO) && (nSeparateNum <PACK_SEPARATE_NUMBER) && (pCoyPackRuleFile != NULL))						
	{
		if(nMark & PACK_SEPARATE_LOGIN)
		{
			if(GetPackUserInfo(pPackInfo,pCoyPackRuleFile,&iio))               //Xt: 查找是否有用户名信息，没有则到下一个包中找	
			{
				nMark &= ~(PACK_SEPARATE_LOGIN);
			}
		}
		if(nMark & PACK_SEPARATE_SEND)
		{
			if(GetPackContentInfo(pPackInfo,pCoyPackRuleFile,&iio))            //Xt: 如果有则查找信息
			{
				nMark &= ~(PACK_SEPARATE_SEND);
			}
		}
		if(nMark == PACK_SEPARATE_NO)
		{
			if(WAIT_OBJECT_0 == WaitForSingleObject(m_hMutex,INFINITE))
			{
				memcpy(&pPackInfo->interceptedInfo,&iio,sizeof(iio));
				m_MulMapInterceptedInfo.insert(make_pair(pCoyPackRuleFile->Item,pPackInfo->interceptedInfo));
				memset(&iio,0,sizeof(iio));
// 				memcpy(&(pPackInfo->interceptedInfo),&iio,sizeof(iio));
// 				m_MulMapInterceptedInfo.insert(make_pair(pCoyPackRuleFile->Item,pPackInfo));
// 				memset(&iio,0,sizeof(iio));
			}
			ReleaseMutex(m_hMutex);
			nMark = PACK_SEPARATE_NO;
			return true;
		}
		else
		{
			nSeparateNum++;
			if(nSeparateNum >= PACK_SEPARATE_NUMBER)	nSeparateNum = 0;
			return false;
		}
	}

	map <string,PACKRULEFILE>::iterator pMapPackRulefile = (CGlobalData::m_mapPackRuleFileInfo.begin());
	for(pMapPackRulefile;pMapPackRulefile != CGlobalData::m_mapPackRuleFileInfo.end();++pMapPackRulefile)
	{
		if(pMapPackRulefile->second.bSelect)                                                       //Xt: 判断是否选择了抓包
		{                                                                                         
			if(IsRulePack(pPackInfo,&(pMapPackRulefile->second)))                                  //Xt: 判断是否是规则包(头，域名)
			{
				if('\0' != (pMapPackRulefile->second.UserNameKey)[0])                              //Xt: 判断是否有用户名信息(发帖有的没用户名)
				{
					if(!GetPackUserInfo(pPackInfo,&(pMapPackRulefile->second),&iio))               //Xt: 查找是否有用户名信息，没有则到下一个包中找	
					{
						pCoyPackRuleFile = &(pMapPackRulefile->second);
						nMark |= PACK_SEPARATE_LOGIN;
					}
				}
				if('\0' != (pMapPackRulefile->second.SendContentKey)[0])                           //Xt: 查看有没有发送规则
				{
					if(!GetPackContentInfo(pPackInfo,&(pMapPackRulefile->second),&iio))            //Xt: 如果有则查找信息
					{
						pCoyPackRuleFile = &(pMapPackRulefile->second);
						nMark |= PACK_SEPARATE_SEND;
					}	
 				}
				if(nMark == PACK_SEPARATE_NO)
				{
					if('\0' != iio.UserName[0] || '\0' != iio.SendContent[0])
					{
						if(WAIT_OBJECT_0 == WaitForSingleObject(m_hMutex,INFINITE))
						{
                            memcpy(&(pPackInfo->interceptedInfo),&iio,sizeof(iio));
							m_MulMapInterceptedInfo.insert(make_pair(pMapPackRulefile->first,pPackInfo->interceptedInfo));
							memset(&iio,0,sizeof(iio));
						}
						ReleaseMutex(m_hMutex);
						return true;
					}		
 				}	
			}		
		}
	}
	return false;
}

bool  CPackThread::IsRulePack(pack_info *pPackInfo,PACKRULEFILE *pPackRuleFile)   
{
	int  nCurrent = 0;
	char *pBegin = NULL;
	char *pHost = NULL;
	pBegin = FindKey((char *)pPackInfo->pPackContentBuff,pPackRuleFile->PackHead,pPackInfo->PackContentLength-nCurrent,nCurrent);
	if(!pBegin)		return false;

	pHost = FindKey(pBegin,pPackRuleFile->Host,pPackInfo->PackContentLength-nCurrent,nCurrent);
	if(!pHost)		return false;

	return true;
}

bool  CPackThread::GetPackUserInfo(pack_info *pPackInfo,PACKRULEFILE *pPackRuleFile,INTERCEPTEDINFO *pInterceptedInfo)
{
	char *pBegin = NULL;
	char *pEnd = NULL;
	char *pName = NULL;                                  //Xt: 转码前用户信息
	char cEnd;                                           //Xt: 结束符标志
	int  nKeyLength = 0;                                 //Xt: 键值长度
	int  nCurrent = 0;                                   //Xt: 查找到关键字的偏移量
	int  FindLength = pPackInfo->PackContentLength;      //Xt: 查找的长度

	pBegin = FindKey((char *)pPackInfo->pPackContentBuff,pPackRuleFile->UserNameKey,FindLength,nCurrent);
	if(!pBegin)		return false;
	nKeyLength = strlen(pPackRuleFile->UserNameKey);
	FindLength = FindLength - nCurrent - nKeyLength;

	pEnd = FindKey(pBegin+nKeyLength, pPackRuleFile->UserEndNameKey,FindLength,nCurrent);
	if(!pEnd)	return false;
	FindLength = FindLength - nCurrent;

	nKeyLength = strlen(pPackRuleFile->UserNameKey);
	pName = (pBegin+nKeyLength);
	cEnd = *(pName+(pEnd-pBegin-nKeyLength));
	*(pName+(pEnd-pBegin-nKeyLength)) = '\0';
	CChangeCode::FindCode(pPackRuleFile->CodeType,pName,pInterceptedInfo->UserName);         //Xt: 转码并储存用户名
	*(pName+(pEnd-pBegin-nKeyLength)) = cEnd;

	pBegin = NULL;

	if('\0' != *(pPackRuleFile->Item))
		memcpy(pInterceptedInfo->Item,pPackRuleFile->Item,strlen(pPackRuleFile->Item));      //Xt: 存档

	if('\0' != *(pPackRuleFile->PassWordKey))
	{
		pBegin = FindKey(pEnd,pPackRuleFile->PassWordKey,FindLength,nCurrent);
		if(!pBegin)		return false;
		nKeyLength = strlen(pPackRuleFile->PassWordKey);
		FindLength = FindLength - nCurrent - nKeyLength;

		pEnd = NULL;
		pEnd = FindKey(pBegin+nKeyLength,pPackRuleFile->EndPassWordKey,FindLength,nCurrent);
		if(!pEnd)	
		{
			memcpy(pInterceptedInfo->Password,pBegin+nKeyLength,FindLength);
		}
		else
		{
			memcpy(pInterceptedInfo->Password,pBegin+nKeyLength,pEnd-pBegin-nKeyLength);
		}	
	}
	return true;
}

bool  CPackThread::GetPackContentInfo(pack_info *pPackInfo,PACKRULEFILE *pPackRuleFile,INTERCEPTEDINFO *pInterceptedInfo)
{
	char *pBegin = NULL;
	char *pHost = NULL;
	char *pEnd = NULL;
	char *pContent = NULL;
	int  nLength = 0;
	int  nCurrent = 0;
	if('\0' != *(pPackRuleFile->SendContentKey))
	{
		pBegin = FindKey((char*)pPackInfo->pPackContentBuff,pPackRuleFile->SendContentKey,pPackInfo->PackContentLength-nCurrent,nCurrent);
		if(!pBegin)		return false;

		pEnd = NULL;
		pEnd = FindKey(pBegin,pPackRuleFile->EndSengContentKey,pPackInfo->PackContentLength-nCurrent,nCurrent);
		if(!pEnd)		return false;

		nLength = strlen((char*)(pPackRuleFile->SendContentKey));
		pContent = pBegin+nLength;
		*(pContent+(pEnd-pBegin-nLength)) = '\0';
		CChangeCode::FindCode(pPackRuleFile->CodeType,pContent,pInterceptedInfo->SendContent);
	}
	return true;
}

char *CPackThread::FindKey(char *p1,char *p2,int nLength,int &nReturnOffset)
{
	char *pCopy1 = p1;
	char *pCopy2 = p2;

	char strCopyU2[1024] = {0};                 //大小写问题
	memcpy(strCopyU2,p2,strlen(p2));
	char strCopyL2[1024] = {0};
	memcpy(strCopyL2,p2,strlen(p2));

	char *pCopyU2 = strupr(strCopyU2);
	char *pCopyL2 = strlwr(strCopyL2);

	char *p2L = pCopyL2;
	char *p2U = pCopyU2;
	char *pCurrent = NULL;

	int i = 0;
	nReturnOffset = 0;

	while(i<nLength)
	{
		if('\0' != *pCopyU2)
		{
			if((*(pCopy1+i) == *pCopyU2) || (*(pCopy1+i) == *pCopyL2))
			{
				if(NULL == pCurrent)
				{
					pCurrent= (p1+i);
					nReturnOffset = i;
				}
				i++;
				pCopyL2++;
				pCopyU2++;
			}
			else
			{
				if(NULL != pCurrent)
				{
					pCopy1 = p1;
					pCurrent = NULL;
					i = nReturnOffset;
				}
				pCopyU2 = p2U;
				pCopyL2 = p2L;
				i++;
			}
		}
		else
		{
			return pCurrent;
		}
	}
	if((i == nLength) && ('\0' == *pCopyU2))
	{
		return pCurrent;
	}
	return NULL;
}

void  CPackThread::FindCode(char *pStrCodeType,char *strSrc,char *pStrDest)
{
	CChangeCode cc;
	if(!memcmp(pStrCodeType,"U-U",sizeof("U-U")))
	{
		cc.UrlUTF8Decode(strSrc,pStrDest);
		return;
	}
	if(!memcmp(pStrCodeType,"U-G",sizeof("U-G")))
	{
		cc.UrlGB2312Decode(strSrc,pStrDest);
		return;
	}
	if(!memcmp(pStrCodeType,"U-",sizeof("U-")))
	{
		cc.UTF_8ToGB2312(strSrc,pStrDest,strlen(pStrDest));	
		return;
	}
	if(!memcmp(pStrCodeType,"U-G-U",sizeof("U-G-U")))
	{
		cc.URLGB2312ToURLUTF8Decode(strSrc,pStrDest);
		return;
	}
	if('\0' == *pStrCodeType  )
	{
		memcpy(pStrDest,strSrc,strlen(strSrc));
		return;
	}
}
bool  CPackThread::OnLineCapturePack(int nOpertionType)
{
	CGlobalData::DeleteInterCeptedFile();
	pcap_pkthdr *header = NULL;                   
	const u_char *pkt_data = NULL;

	struct tm *ltime;
	time_t local_tv_sec;

	//CHostUser* hostUser = CHostUser::GetInstance();
	DWORD dwTick = GetTickCount();
	u_int nIndex = 0;
	int res = 0;
	char szKey[39]={0};
	//获取数据包 处理抓包数据
	while((!m_bStop) && res >= 0)
	{
		res = pcap_next_ex( CPackThread::m_pRecvHandle,
			                &header,                                 //Xt:包的时间，抓到包的长度、包的实际长度
			                &pkt_data);                              //Xt:包的内容
		if(res<0)
			CGlobalData::PrintLogInfo("WorkThread","获取包时出现错误!",GetLastError());
		/* 超时时间到 */
		if(res == 0) 
		{
			continue;
		}

		//超出最大值，则从1开始
		if (nIndex < INDEX_MAX_VALUE)
			nIndex++; 
		else nIndex = 1;

		//将时间戳转换成可识别的格式
		local_tv_sec = header->ts.tv_sec;
		//ltime = localtime(&local_tv_sec);

		tagDecodeInput di;
		di.Reset(local_tv_sec,header->len,const_cast<u_char *>(pkt_data));

		pack_info  pi;
		pi.Reset(local_tv_sec,header->len,const_cast<u_char *>(pkt_data));
		//解析数据包
		Decode(pi);
		if(pi.PackType == PACKE_TYPE_TCP)
		{
			if(((unsigned long)(pi.ip_src.S_un.S_addr) == (unsigned long)inet_addr(m_strLocalIP)) || ((unsigned long)(pi.ip_dst.S_un.S_addr) == (unsigned long)inet_addr(m_strLocalIP)))
			{
				if(CAPTURE_PACK_INFO == (nOpertionType & CAPTURE_PACK_INFO))
					ResolvePackRun(&pi);
				else if(CAPTURE_PACK_RULE == (nOpertionType & CAPTURE_PACK_RULE))
					ResolvePackRuleRun(&pi);
			}
		}
	}
	pcap_close(m_pRecvHandle);
	return true;
}

bool  CPackThread::OffLineCapturePack(int nOpertionType,const char *pPackPath)
{
	time_t local_tv_sec;

	pcap_t *fp;
	char errbuf[PCAP_ERRBUF_SIZE];
	char source[PCAP_BUF_SIZE];
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	u_int i=0;
	int res;

	/* 根据新WinPcap语法创建一个源字符串 */
	if ( pcap_createsrcstr( source,              // 源字符串
		PCAP_SRC_FILE,                           // 我们要打开的文件
		NULL,                                    // 远程主机
		NULL,                                    // 远程主机端口
		pPackPath,								 // 我们要打开的文件名
		errbuf                                   // 错误缓冲区
		) != 0)
	{
		fprintf(stderr,"\nError creating a source string\n");
		return false;
	}

	/* 打开捕获文件 */
	if ( (fp= pcap_open(source,         // 设备名
		65536,          // 要捕捉的数据包的部分
		// 65535保证能捕获到不同数据链路层上的每个数据包的全部内容
		PCAP_OPENFLAG_PROMISCUOUS,     // 混杂模式
		1000,              // 读取超时时间
		NULL,              // 远程机器验证
		errbuf         // 错误缓冲池
		) ) == NULL)
	{
		fprintf(stderr,"\nUnable to open the file %s.\n", source);
		return false;
	}

	/* 从文件获取数据包 */
	while((res = pcap_next_ex( fp, &header, &pkt_data)) >= 0)
	{
		if(res<0)
			CGlobalData::PrintLogInfo("WorkThread","获取包时出现错误!",GetLastError());
		/* 超时时间到 */
		if(res == 0) 
		{
			continue;
		}

		// 将时间戳转换成可识别的格式
		local_tv_sec = header->ts.tv_sec;
		//ltime = localtime(&local_tv_sec);

		tagDecodeInput di;
		di.Reset(local_tv_sec, header->len, const_cast<u_char*>(pkt_data));

		pack_info  pi;
		pi.Reset(local_tv_sec,header->len,const_cast<u_char*>(pkt_data));
		// 解析数据包
		Decode(pi);
		if(pi.PackType == PACKE_TYPE_TCP || pi.PackType == PACKE_TYPE_UDP)
		{
			if(CAPTURE_PACK_INFO == (nOpertionType & CAPTURE_PACK_INFO))
				ResolvePackRun(&pi);
			else if(CAPTURE_PACK_RULE == (nOpertionType & CAPTURE_PACK_RULE))
				ResolvePackRuleRun(&pi);
		}
	}
	if (res == -1)
	{
		printf("Error reading the packets: %s\n", pcap_geterr(fp));	
	}
	return true;
}

bool  CPackThread::ResolvePackRun(pack_info *pi)
{
	if(!memcmp(pi->pPackContentBuff,"GET /jump",strlen("GET /jump")))
	{
		int nna = 100;
	 	nna++;
	}
	char strFileHead[1024]  = {0};
	if(GetResolvePackInfo(pi))
	{
		//Xt:发送到CaptruePackInfoDlg添加列表信息
		if('\0' != *(CGlobalData::m_pGlbalData->m_strInterceptedInfoPath))
		{
			FILE *pFile = fopen((CGlobalData::m_pGlbalData->m_strInterceptedInfoPath),"a+");
			if(pFile)
			{
				fgets(strFileHead,sizeof(strFileHead),pFile);
				if(memcmp(".LOG",strFileHead,strlen(".LOG")))
				{
					fputs(".LOG\r\n",pFile);
				}
				fseek(pFile,0,SEEK_END);
				CGlobalData::printPcapMSG(pi,pFile);	                             //Xt:截获信息写入文件;
				fclose(pFile);
				if(CCaptruePackInfoDlg::m_pThis)
				{
					calAddPackListInfo();										     //Xt:发送给对话框重新加载信息
				}
			}	
		}
		if(CLoginTestDlg::m_pThis)
		{
			calLoginSate(pi->interceptedInfo.Item);
		}
	}
	return true;
}


bool  CPackThread::ResolvePackRuleRun(pack_info *pi)
{
	if(!memcmp(pi->pPackContentBuff,"POST /newreply.php",strlen("POST /newreply.php")))
	{
		int nna = 100;
		nna++;
	}
	char strFileHead[1024]  = {0};
	if(GetResolvePackRuleInfo(pi))
	{
		if('\0' != *(CGlobalData::m_pGlbalData->m_strRulePackInfoPath))                      //Xt: 打印包规则内容文件信息
		{
			FILE *pRuleFile = fopen((CGlobalData::m_pGlbalData->m_strRulePackInfoPath),"a+");
			if(pRuleFile)
			{     
				fgets(strFileHead,sizeof(strFileHead),pRuleFile);
				if(memcmp(".LOG",strFileHead,strlen(".LOG")))
				{
					fputs(".LOG\r\n",pRuleFile);
				}
				memset(strFileHead,0,sizeof(strFileHead));
				fseek(pRuleFile,0,SEEK_END);
				CGlobalData::printPackRule(&CPackThread::m_PackRuleInfo,pRuleFile);	         //Xt:截获信息写入文件;
				fclose(pRuleFile);
				if(CFindCaptureRuleDlg::m_pThis)
				{
					CFindCaptureRuleDlg::_CallAddPackRuleInfo(&CPackThread::m_PackRuleInfo); //Xt:发送给对话框重新加载信息
				}
				memset(&CPackThread::m_PackRuleInfo,0,sizeof(CPackThread::m_PackRuleInfo));
			}
		}

		if('\0' != *(CGlobalData::m_pGlbalData->m_strRulePackPath))                          //Xt: 打印规则包文件信息
		{
			FILE *pRulePackFile = fopen(CGlobalData::m_pGlbalData->m_strRulePackPath,"a+");
			if(pRulePackFile)
			{
				fgets(strFileHead,sizeof(strFileHead),pRulePackFile);
				if(memcmp(".LOG",strFileHead,strlen(".LOG")))
				{
					fputs(".LOG\r\n",pRulePackFile);
				}
				memset(strFileHead,0,sizeof(strFileHead));
				fseek(pRulePackFile,0,SEEK_END);
				if('\0'!= *m_strSaveGetOrPostPackHead)
				{
					fprintf(pRulePackFile,"%s\r\n\r\n",m_strSaveGetOrPostPackHead);
				}
				char cEnd = pi->pPackContentBuff[pi->PackContentLength+1];
				pi->pPackContentBuff[pi->PackContentLength+1] = '\0';
				fprintf(pRulePackFile,"%s\r\n\r\n",pi->pPackContentBuff);
				fprintf(pRulePackFile,"*****************************************************************************************\r\n\r\n");
				pi->pPackContentBuff[pi->PackContentLength+1] = cEnd;
				fclose(pRulePackFile);
				memset(m_strSaveGetOrPostPackHead,0,sizeof(m_strSaveGetOrPostPackHead));
			}
		}
	}
	return true;
}

bool  CPackThread::GetResolvePackRuleInfo(pack_info *pi)
{
	static int nSeparate =0;                                                    //Xt：分包标志保留查找分包的个数(post或Get包的保鲜时间)
	if(nSeparate <=0)                                                           //Xt: 过了分包个数则删除保留的包头，和包中查找到的信息
	{
		if('\0' != *m_strSaveGetOrPostPackHead)
			memset(m_strSaveGetOrPostPackHead,0,sizeof(m_strSaveGetOrPostPackHead));
			memset(&m_PackRuleInfo,0,sizeof(m_PackRuleInfo));
	}
	else
	{
		nSeparate--;
	}

	int nType = 0;
	vector <FindInfo>::iterator pFindInfo = CGlobalData::m_vtFindInfo.begin();
	for(pFindInfo;pFindInfo != CGlobalData::m_vtFindInfo.end();++pFindInfo)
	{
		nType = FindPackUserInfo(pi,&(*pFindInfo),&m_PackRuleInfo,CGlobalData::m_nPackRuleLen);
		if(nType)																//Xt: 没截获到信息则继续循环
		{ 
			if((PACK_FIND_USERNAME_KEY | PACK_FIND_SENDINFO_KEY) == nType )	    //Xt: 用户名和发送信息在同一个包中则保存
			{
				if(('\0' == *pFindInfo->SendContent))
				{
					if(!FindPackHostInfo((char *)pi->pPackContentBuff,&m_PackRuleInfo,CGlobalData::m_nPackHeadRuleLen))  //Xt:查找当前包中有无HOST
					{
						if(FindPackHostInfo(m_strSaveGetOrPostPackHead,&m_PackRuleInfo,CGlobalData::m_nPackHeadRuleLen)) //Xt:查找前一包中有无HOST
						{
							nSeparate = 0;
							return true;
						}
						else
							break;
					}
					else
						return true;
				}
			}
			if( '\0' != *pFindInfo->userName)                                   //Xt: 只截获有用户名信息并不查找发送信息则保存
			{ 
				if((nType & PACK_FIND_USERNAME_KEY) == PACK_FIND_USERNAME_KEY)
				{
					if(!FindPackHostInfo((char *)pi->pPackContentBuff,&m_PackRuleInfo,CGlobalData::m_nPackHeadRuleLen))
					{
						FindPackHostInfo(m_strSaveGetOrPostPackHead,&m_PackRuleInfo,CGlobalData::m_nPackHeadRuleLen);					
					}
					if('\0' != *(m_PackRuleInfo.Host))                   
					{
						if('\0' == *pFindInfo->SendContent)
						{
							nSeparate = 0;
							return true;
						}
					}
				}
			}
			if('\0' != *pFindInfo->SendContent)
			{ 
				if((nType & PACK_FIND_SENDINFO_KEY) == PACK_FIND_SENDINFO_KEY)
				{   
                    if('\0' == *pFindInfo->userName)                              //Xt: 只截获发送信息并不查找用户信息的保存
					{
						if(!FindPackHostInfo((char *)pi->pPackContentBuff,&m_PackRuleInfo,CGlobalData::m_nPackHeadRuleLen))
						{
							if(FindPackHostInfo(m_strSaveGetOrPostPackHead,&m_PackRuleInfo,CGlobalData::m_nPackHeadRuleLen))
							{
								nSeparate = 0;
								return true;
							}
							continue;
						}
						else
							nSeparate = 0;
							return true;
					}
					else  if(('\0' != *m_PackRuleInfo.UserNameKey) && ('\0' != *m_PackRuleInfo.UserEndNameKey)) //Xt: 即截获用户名又截获发送信息(分包)
					{
						nSeparate = 0;
						return true;	
					}
				}
			}
		}
	}
	//Xt: 保存GET或POST的包
	if(!memcmp(pi->pPackContentBuff,"GET",strlen("GET")) || (!memcmp(pi->pPackContentBuff,"POST",strlen("POST"))))  
	{
		memcpy(m_strSaveGetOrPostPackHead,pi->pPackContentBuff,pi->PackContentLength);
		memset(&m_PackRuleInfo,0,sizeof(m_PackRuleInfo));
		nSeparate = CGlobalData::m_nSeparatePackNum;
	}
	return false;
}

int CPackThread::FindPackUserInfo(pack_info *pi,FindInfo *pFindInfo,PackRuleFile *pPackRuleInfo,int nRuleLen)
{
	int nLen = nRuleLen;  
	int nType = 0;																 //Xt: 截取类型
	int nPackPos = 0;															 //Xt: 包偏移量
	int nSurplusLen = 0;                                                         //Xt: 包的剩余长度
	                           
	if('\0' != *pFindInfo->userName)
	{
		if(FindKey((char *)pi->pPackContentBuff,pFindInfo->userName,pi->PackContentLength,nPackPos))
		{
			nPackPos>nRuleLen?	nLen = nRuleLen:nLen = nPackPos;
			memcpy(pPackRuleInfo->UserNameKey,pi->pPackContentBuff+nPackPos-nLen,nLen);
			nSurplusLen = nPackPos+strlen(pFindInfo->userName);
			(pi->PackContentLength-nSurplusLen)-nRuleLen>0?	nLen=nRuleLen:nLen=(pi->PackContentLength-nSurplusLen);        //Xt: 判断截取长度是否为到包尾
			memcpy(pPackRuleInfo->UserEndNameKey,pi->pPackContentBuff+nSurplusLen,nLen);
			memcpy(pPackRuleInfo->CodeType,pFindInfo->Type,strlen(pFindInfo->Type));
			nType |= PACK_FIND_USERNAME_KEY;
		}
	}

	nPackPos = 0;
	nSurplusLen = 0;
	if('\0' != *pFindInfo->Password)
	{
		if(FindKey(((char *)pi->pPackContentBuff),pFindInfo->Password,pi->PackContentLength,nPackPos))
		{
			nPackPos>nRuleLen?	nLen = nRuleLen:nLen = nPackPos;
			memcpy(pPackRuleInfo->PassWordKey,pi->pPackContentBuff+nPackPos-nLen,nLen);
			nSurplusLen = nPackPos+strlen(pFindInfo->Password);
			(pi->PackContentLength-nSurplusLen)-nRuleLen>0?	nLen=nRuleLen:nLen=(pi->PackContentLength-nSurplusLen);        //Xt: 判断截取长度是否为到包尾
			memcpy(pPackRuleInfo->EndPassWordKey,pi->pPackContentBuff+nSurplusLen,nLen);
			memcpy(pPackRuleInfo->CodeType,pFindInfo->Type,strlen(pFindInfo->Type));
			nType |= PACK_FIND_PASSWORD_KEY;
		}
	}

	nPackPos = 0;
	nSurplusLen = 0;
	if('\0' != *(pFindInfo->SendContent))
	{
		if(FindKey(((char *)pi->pPackContentBuff),pFindInfo->SendContent,pi->PackContentLength,nPackPos))
		{
			nPackPos>nRuleLen?	nLen = nRuleLen:nLen = nPackPos;                                    
			memcpy(pPackRuleInfo->SendContentKey,pi->pPackContentBuff+nPackPos-nLen,nLen);
			nSurplusLen = nPackPos+strlen(pFindInfo->SendContent);
			(pi->PackContentLength-nSurplusLen)-nRuleLen>0?	nLen=nRuleLen:nLen=(pi->PackContentLength-nSurplusLen);      //Xt: 判断截取长度是否为到包尾
			if(0 == nLen)
				memcpy(pPackRuleInfo->EndSengContentKey,"以包尾结束",strlen("以包尾结束"));
			else
				memcpy(pPackRuleInfo->EndSengContentKey,pi->pPackContentBuff+nSurplusLen,nLen);
			memcpy(pPackRuleInfo->CodeType,pFindInfo->Type,strlen(pFindInfo->Type));
			nType |= PACK_FIND_SENDINFO_KEY;
		}	
	}
	return nType;
}

bool  CPackThread::FindPackHostInfo(char *pBuffer,PackRuleFile *pPackRuleInfo,int nRuleLen)
{
	char *pStrBegin = NULL;
	char *pStrEnd   = NULL;
	if(!memcmp(pBuffer,"GET",strlen("GET")) || (!memcmp(pBuffer,"POST",strlen("POST"))))
	{
		pStrEnd = StrStrA((LPCSTR)pBuffer,"\x20HTTP/");                    //Xt:获取包头
		if(pStrEnd)
			//memcpy(pPackRuleInfo->PackHead,pBuffer,pStrEnd-pBuffer);     //Xt: 获取规定长度
			memcpy(pPackRuleInfo->PackHead,pBuffer,nRuleLen);
		else
			return false;

		pStrBegin = StrStrA(pStrEnd,"Host:\x20");                          //Xt:获取域名
		if(pStrBegin)
		{
			pStrEnd = NULL;
			pStrEnd = StrStrA(pStrBegin,"\x0d\x0a");
			if(pStrEnd)
			{
				memcpy(pPackRuleInfo->Host,pStrBegin,pStrEnd-pStrBegin);
				pStrBegin = NULL;
				pStrEnd   = NULL;
			}
			else
			{
				return false;
			}
		}
		pStrBegin = StrStrA(pBuffer,"Referer:\x20");
		if(pStrBegin)
		{
			pStrEnd = StrStrA(pStrBegin,"\x0d\x0a");
			if(pStrEnd)
			{
				memcpy(pPackRuleInfo->Referer,pStrBegin+strlen("Referer:\x20"),pStrEnd-pStrBegin-strlen("Referer:\x20"));
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}