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

	calLoginSate = CLoginTestDlg::UpdatePackState;                                      //Xt: �ص��Ի���ץ��״̬"OK"
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

	if(CAPTURE_INLINE_PACK == (nOpertionType & CAPTURE_INLINE_PACK))                    //Xt:����������Ϣ
	{
		OnLineCapturePack(nOpertionType);
	}

	if(CAPTURE_OFFLINE_PACK == (nOpertionType & CAPTURE_OFFLINE_PACK))                  //Xt: ����������Ϣ
	{
		if(CGlobalData::m_vtOfflinePackPath.empty())
		{
			CGlobalData::PrintLogInfo("CPackThrea","���߰�·������!",'E');
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
	char errbuf[PCAP_ERRBUF_SIZE] = {0};             //��ô�����Ϣ


	/* ��ȡ���ػ����豸�б� */
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &pAdapterinfo, errbuf) == -1)
	{
		CGlobalData::PrintLogInfo("CPacpPac","��ȡ����Ϣ������",GetLastError());
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
  
	/* ��ӡ�б� */
	for(pAlldevs= pAdapterinfo; pAlldevs != NULL; pAlldevs= pAlldevs->next)
	{
		if (pAlldevs->description)
		{
//          printf(" (%s)\n", pAlldevs->description);
// 			printf(" ����IP��ַ��(%s)\n", inet_ntoa(((sockaddr_in *)(pAlldevs->addresses->addr))->sin_addr));
// 			printf(" �����������룺(%s)\n", inet_ntoa(((sockaddr_in *)(pAlldevs->addresses->netmask))->sin_addr));
// 			printf(" �㲥��ַ��(%s)\n", inet_ntoa(((sockaddr_in *)(pAlldevs->addresses->broadaddr))->sin_addr));
			CGlobalData::m_vectAdapterName.push_back(pAlldevs->name);
			CGlobalData::m_mapAdapterInfo.insert(make_pair(pAlldevs->name,pAlldevs));
		}
	}

	if(CGlobalData::m_vectAdapterName.empty())
	{
		CGlobalData::PrintLogInfo("CPackThread","������ϢΪ��!",'E');
		return false;
	}
	return true;
}

bool CPackThread::SetAdapterInfo(string strAdapterName)
{
	if(strAdapterName.empty())	
	{
		CGlobalData::PrintLogInfo("CPackThread","û��ѡ��������Ϣ!",'E');
		return false;
	}
	map <string,pcap_if_t *>::iterator pMapAdapter = CGlobalData::m_mapAdapterInfo.find(strAdapterName);
	if( pMapAdapter == CGlobalData::m_mapAdapterInfo.end() )
	{
		CGlobalData::PrintLogInfo("CPackThread","����������!û�п�ѡ��������Ϣ!",'E');
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
		CGlobalData::PrintLogInfo("CPackThread","û������������Ϣ!",'E');
		return false;
	}

	char errbuf[PCAP_ERRBUF_SIZE] = {0};
	if ( (CPackThread::m_pRecvHandle= pcap_open((m_pRecvAdapterInfo->name+8),          // �豸��
		65536,                        // Ҫ��׽�����ݰ��Ĳ��� 
		//                               65535��֤�ܲ��񵽲�ͬ������·���ϵ�ÿ�����ݰ���ȫ������
		PCAP_OPENFLAG_PROMISCUOUS,    // ����ģʽ
		1,                            // ��ȡ��ʱʱ��
		NULL,                         // Զ�̻�����֤
		errbuf                        // ���󻺳��
		) ) == NULL)
	{
	    pcap_freealldevs(m_pRecvAdapterInfo);                                   //�ر��ϴε������豸 
		CGlobalData::PrintLogInfo("CPackThread","���豸����������!",'E');                                     //Xt:�ر��豸�б�
		return false;
	}

	pcap_freealldevs(m_pRecvAdapterInfo);

	char filterStr[4096]={0};
	sprintf_s(filterStr, 4096, "(tcp) or (udp) or (ether proto 0x8864) or (ether proto 0x8100)");

	u_int netmask=0xffffff;                                                                //�������255.255.255.0
	struct bpf_program fcode;

	if (pcap_compile(CPackThread::m_pRecvHandle, &fcode, filterStr, 1, netmask) < 0)
	{
		CGlobalData::PrintLogInfo("CPackThread","��������ץ��������Ϣ����!",'E');
		return false;
	}

	if (pcap_setfilter(CPackThread::m_pRecvHandle, &fcode) < 0)
	{
		CGlobalData::PrintLogInfo("CPackThread","���������ں˲����������!",'E');
		return false;
	}

	if (pcap_setbuff(CPackThread::m_pRecvHandle, MOMERYSIZE) != 0)
	{
		CGlobalData::PrintLogInfo("CPackThread","���û�����ʧ��!",GetLastError());
		return false;
	}
	return true;
}

int  CPackThread::Decode(pack_info &pi)
{
	// ������̫������
	ethernet_header* eth = (ethernet_header*)pi.pPackBuff;

	// �޸�����ͷ����Ϣ
	pi.PackHeadLength = sizeof(ethernet_header);                    // �ӵ�ǰָ�뿪ʼ�������ĳ���
	memcpy(pi.DestMac,eth->DestMac,6);
	memcpy(pi.SrcMac ,eth->SrcMac ,6);
	switch(eth->Type)
	{
	case ARP_Portocol_Type:
		return DECODE_DEFAULT;
		break;
	case IP_Portocol_Type:
		// ֱ�Ӿ���IP����
		return DecodeIP(pi);

		break;
	case PPPoE_Protocol_Type:
		// PPPoE����
		//return DecodePPPoE(di);
		break;
	case VLan_Protocol_Type:
		// VLAN����
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

	// ����Э������ȷ��������
	switch(ip->proto)
	{
	case 1:
		// ICMPЭ��
		break;
	case 2:
		// IGMPЭ��
	case 6:
		// TCPЭ��
		return DecodeTcp(pi);
		break;
	case 17:
		// UDPЭ��
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
	if (WSAStartup(MAKEWORD(2,1),&wsaData)) //����Windows Sockets DLL
	{ 
		printf("Winsock�޷���ʼ��!\n");
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
			if(GetPackUserInfo(pPackInfo,pCoyPackRuleFile,&iio))               //Xt: �����Ƿ����û�����Ϣ��û������һ��������	
			{
				nMark &= ~(PACK_SEPARATE_LOGIN);
			}
		}
		if(nMark & PACK_SEPARATE_SEND)
		{
			if(GetPackContentInfo(pPackInfo,pCoyPackRuleFile,&iio))            //Xt: ������������Ϣ
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
		if(pMapPackRulefile->second.bSelect)                                                       //Xt: �ж��Ƿ�ѡ����ץ��
		{                                                                                         
			if(IsRulePack(pPackInfo,&(pMapPackRulefile->second)))                                  //Xt: �ж��Ƿ��ǹ����(ͷ������)
			{
				if('\0' != (pMapPackRulefile->second.UserNameKey)[0])                              //Xt: �ж��Ƿ����û�����Ϣ(�����е�û�û���)
				{
					if(!GetPackUserInfo(pPackInfo,&(pMapPackRulefile->second),&iio))               //Xt: �����Ƿ����û�����Ϣ��û������һ��������	
					{
						pCoyPackRuleFile = &(pMapPackRulefile->second);
						nMark |= PACK_SEPARATE_LOGIN;
					}
				}
				if('\0' != (pMapPackRulefile->second.SendContentKey)[0])                           //Xt: �鿴��û�з��͹���
				{
					if(!GetPackContentInfo(pPackInfo,&(pMapPackRulefile->second),&iio))            //Xt: ������������Ϣ
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
	char *pName = NULL;                                  //Xt: ת��ǰ�û���Ϣ
	char cEnd;                                           //Xt: ��������־
	int  nKeyLength = 0;                                 //Xt: ��ֵ����
	int  nCurrent = 0;                                   //Xt: ���ҵ��ؼ��ֵ�ƫ����
	int  FindLength = pPackInfo->PackContentLength;      //Xt: ���ҵĳ���

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
	CChangeCode::FindCode(pPackRuleFile->CodeType,pName,pInterceptedInfo->UserName);         //Xt: ת�벢�����û���
	*(pName+(pEnd-pBegin-nKeyLength)) = cEnd;

	pBegin = NULL;

	if('\0' != *(pPackRuleFile->Item))
		memcpy(pInterceptedInfo->Item,pPackRuleFile->Item,strlen(pPackRuleFile->Item));      //Xt: �浵

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

	char strCopyU2[1024] = {0};                 //��Сд����
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
	//��ȡ���ݰ� ����ץ������
	while((!m_bStop) && res >= 0)
	{
		res = pcap_next_ex( CPackThread::m_pRecvHandle,
			                &header,                                 //Xt:����ʱ�䣬ץ�����ĳ��ȡ�����ʵ�ʳ���
			                &pkt_data);                              //Xt:��������
		if(res<0)
			CGlobalData::PrintLogInfo("WorkThread","��ȡ��ʱ���ִ���!",GetLastError());
		/* ��ʱʱ�䵽 */
		if(res == 0) 
		{
			continue;
		}

		//�������ֵ�����1��ʼ
		if (nIndex < INDEX_MAX_VALUE)
			nIndex++; 
		else nIndex = 1;

		//��ʱ���ת���ɿ�ʶ��ĸ�ʽ
		local_tv_sec = header->ts.tv_sec;
		//ltime = localtime(&local_tv_sec);

		tagDecodeInput di;
		di.Reset(local_tv_sec,header->len,const_cast<u_char *>(pkt_data));

		pack_info  pi;
		pi.Reset(local_tv_sec,header->len,const_cast<u_char *>(pkt_data));
		//�������ݰ�
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

	/* ������WinPcap�﷨����һ��Դ�ַ��� */
	if ( pcap_createsrcstr( source,              // Դ�ַ���
		PCAP_SRC_FILE,                           // ����Ҫ�򿪵��ļ�
		NULL,                                    // Զ������
		NULL,                                    // Զ�������˿�
		pPackPath,								 // ����Ҫ�򿪵��ļ���
		errbuf                                   // ���󻺳���
		) != 0)
	{
		fprintf(stderr,"\nError creating a source string\n");
		return false;
	}

	/* �򿪲����ļ� */
	if ( (fp= pcap_open(source,         // �豸��
		65536,          // Ҫ��׽�����ݰ��Ĳ���
		// 65535��֤�ܲ��񵽲�ͬ������·���ϵ�ÿ�����ݰ���ȫ������
		PCAP_OPENFLAG_PROMISCUOUS,     // ����ģʽ
		1000,              // ��ȡ��ʱʱ��
		NULL,              // Զ�̻�����֤
		errbuf         // ���󻺳��
		) ) == NULL)
	{
		fprintf(stderr,"\nUnable to open the file %s.\n", source);
		return false;
	}

	/* ���ļ���ȡ���ݰ� */
	while((res = pcap_next_ex( fp, &header, &pkt_data)) >= 0)
	{
		if(res<0)
			CGlobalData::PrintLogInfo("WorkThread","��ȡ��ʱ���ִ���!",GetLastError());
		/* ��ʱʱ�䵽 */
		if(res == 0) 
		{
			continue;
		}

		// ��ʱ���ת���ɿ�ʶ��ĸ�ʽ
		local_tv_sec = header->ts.tv_sec;
		//ltime = localtime(&local_tv_sec);

		tagDecodeInput di;
		di.Reset(local_tv_sec, header->len, const_cast<u_char*>(pkt_data));

		pack_info  pi;
		pi.Reset(local_tv_sec,header->len,const_cast<u_char*>(pkt_data));
		// �������ݰ�
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
		//Xt:���͵�CaptruePackInfoDlg����б���Ϣ
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
				CGlobalData::printPcapMSG(pi,pFile);	                             //Xt:�ػ���Ϣд���ļ�;
				fclose(pFile);
				if(CCaptruePackInfoDlg::m_pThis)
				{
					calAddPackListInfo();										     //Xt:���͸��Ի������¼�����Ϣ
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
		if('\0' != *(CGlobalData::m_pGlbalData->m_strRulePackInfoPath))                      //Xt: ��ӡ�����������ļ���Ϣ
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
				CGlobalData::printPackRule(&CPackThread::m_PackRuleInfo,pRuleFile);	         //Xt:�ػ���Ϣд���ļ�;
				fclose(pRuleFile);
				if(CFindCaptureRuleDlg::m_pThis)
				{
					CFindCaptureRuleDlg::_CallAddPackRuleInfo(&CPackThread::m_PackRuleInfo); //Xt:���͸��Ի������¼�����Ϣ
				}
				memset(&CPackThread::m_PackRuleInfo,0,sizeof(CPackThread::m_PackRuleInfo));
			}
		}

		if('\0' != *(CGlobalData::m_pGlbalData->m_strRulePackPath))                          //Xt: ��ӡ������ļ���Ϣ
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
	static int nSeparate =0;                                                    //Xt���ְ���־�������ҷְ��ĸ���(post��Get���ı���ʱ��)
	if(nSeparate <=0)                                                           //Xt: ���˷ְ�������ɾ�������İ�ͷ���Ͱ��в��ҵ�����Ϣ
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
		if(nType)																//Xt: û�ػ���Ϣ�����ѭ��
		{ 
			if((PACK_FIND_USERNAME_KEY | PACK_FIND_SENDINFO_KEY) == nType )	    //Xt: �û����ͷ�����Ϣ��ͬһ�������򱣴�
			{
				if(('\0' == *pFindInfo->SendContent))
				{
					if(!FindPackHostInfo((char *)pi->pPackContentBuff,&m_PackRuleInfo,CGlobalData::m_nPackHeadRuleLen))  //Xt:���ҵ�ǰ��������HOST
					{
						if(FindPackHostInfo(m_strSaveGetOrPostPackHead,&m_PackRuleInfo,CGlobalData::m_nPackHeadRuleLen)) //Xt:����ǰһ��������HOST
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
			if( '\0' != *pFindInfo->userName)                                   //Xt: ֻ�ػ����û�����Ϣ�������ҷ�����Ϣ�򱣴�
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
                    if('\0' == *pFindInfo->userName)                              //Xt: ֻ�ػ�����Ϣ���������û���Ϣ�ı���
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
					else  if(('\0' != *m_PackRuleInfo.UserNameKey) && ('\0' != *m_PackRuleInfo.UserEndNameKey)) //Xt: ���ػ��û����ֽػ�����Ϣ(�ְ�)
					{
						nSeparate = 0;
						return true;	
					}
				}
			}
		}
	}
	//Xt: ����GET��POST�İ�
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
	int nType = 0;																 //Xt: ��ȡ����
	int nPackPos = 0;															 //Xt: ��ƫ����
	int nSurplusLen = 0;                                                         //Xt: ����ʣ�೤��
	                           
	if('\0' != *pFindInfo->userName)
	{
		if(FindKey((char *)pi->pPackContentBuff,pFindInfo->userName,pi->PackContentLength,nPackPos))
		{
			nPackPos>nRuleLen?	nLen = nRuleLen:nLen = nPackPos;
			memcpy(pPackRuleInfo->UserNameKey,pi->pPackContentBuff+nPackPos-nLen,nLen);
			nSurplusLen = nPackPos+strlen(pFindInfo->userName);
			(pi->PackContentLength-nSurplusLen)-nRuleLen>0?	nLen=nRuleLen:nLen=(pi->PackContentLength-nSurplusLen);        //Xt: �жϽ�ȡ�����Ƿ�Ϊ����β
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
			(pi->PackContentLength-nSurplusLen)-nRuleLen>0?	nLen=nRuleLen:nLen=(pi->PackContentLength-nSurplusLen);        //Xt: �жϽ�ȡ�����Ƿ�Ϊ����β
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
			(pi->PackContentLength-nSurplusLen)-nRuleLen>0?	nLen=nRuleLen:nLen=(pi->PackContentLength-nSurplusLen);      //Xt: �жϽ�ȡ�����Ƿ�Ϊ����β
			if(0 == nLen)
				memcpy(pPackRuleInfo->EndSengContentKey,"�԰�β����",strlen("�԰�β����"));
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
		pStrEnd = StrStrA((LPCSTR)pBuffer,"\x20HTTP/");                    //Xt:��ȡ��ͷ
		if(pStrEnd)
			//memcpy(pPackRuleInfo->PackHead,pBuffer,pStrEnd-pBuffer);     //Xt: ��ȡ�涨����
			memcpy(pPackRuleInfo->PackHead,pBuffer,nRuleLen);
		else
			return false;

		pStrBegin = StrStrA(pStrEnd,"Host:\x20");                          //Xt:��ȡ����
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