#include "stdafx.h"
#include "ClientBusinessHelper.h"
#include "../global/ShareMemory.h"
#include "iconv/IconvHelp.h"
bool g_bIsRujia = false;

CClientBusinessHelper::CClientBusinessHelper(TPool<DATAPACKET>* pobjRecvPacketPool, CIocpServer* pclsIocpServer, CLIENTBASEINFO& stuClientBaseInfo)
	:m_pobjRecvPacketPool(pobjRecvPacketPool)
	,m_pclsIOCPServer(pclsIocpServer)
{
	m_pstuDataPacket = 0;
	ReceiveReset();
	m_pstuClientBaseInfo = &stuClientBaseInfo;
	//TRACE("客户端类构造函数; IP:%s; connID:%d\r\n", m_pstuClientBaseInfo->szIP, m_pstuClientBaseInfo->dwConnID);
	ASSERT(m_pobjRecvPacketPool);
	ASSERT(m_pclsIOCPServer);
}

CClientBusinessHelper::~CClientBusinessHelper(void)
{
	Release();
	m_pclsIOCPServer = NULL;
	m_pobjRecvPacketPool = NULL;
	m_pstuClientBaseInfo = NULL;
}

//退出清空  由于临界区在析构函数中不能使用  故此函数须在外部调用 
void CClientBusinessHelper::Release()
{
	ReceiveReset();
	m_pclsIOCPServer = NULL;
	m_pobjRecvPacketPool = NULL;
}

// 数据复位
void CClientBusinessHelper::ReceiveReset()
{		
	if (m_pstuDataPacket)
	{
		m_pstuDataPacket->Reset();
		m_pobjRecvPacketPool->Delete(m_pstuDataPacket);		
	}
	m_pstuDataPacket = NULL;
}


bool CClientBusinessHelper::Send(const BYTE* pBuffer, int iLen)
{
	if (m_pstuClientBaseInfo->dwConnID<0) return false;

	return m_pclsIOCPServer->Send(m_pstuClientBaseInfo->dwConnID, pBuffer, iLen);
}


//检查流水号  相同返回false 不同返回true
inline bool CClientBusinessHelper::CheckSerialNo(DWORD dwInSerialNo)
{
	if (m_dwInSerialNo == dwInSerialNo) return false;
	m_dwInSerialNo = dwInSerialNo;
	return true;
}

 //包头检查 -1指针错误  -2 包头错误 -3 申请分配空间失败 0 重复数据包 1 检查成功 分配正确
int CClientBusinessHelper::CheckHeadPaceket(const char* pRecv, const int nRcvLen)
{
	PDATAHEAD pstuDataHead = (PDATAHEAD)pRecv;
	if (!pstuDataHead) return -1;
	DWORD dwSerialNo = 0;

	//检查包头
	if (!pstuDataHead->Check()) 
	{
		//包头检查失败， 则检查是否是未接完的数据包  不是则应该断开连接
		if (m_pstuDataPacket) 
		{
			//TRACE("此包为后缀接收包,类型:%0X; 需接收数据长度:%d;已经接收长度:%d;本次接收长度%d;\r\n", m_pstuDataPacket->pstuDataHead->dwPacketType,
			//	m_pstuDataPacket->pstuDataHead->dwPacketLen, m_pstuDataPacket->dwCurrentSize, nRcvLen);
			return 1;
		}
		TRACE(_T("包头检查错误,也不是未接完的数据,此包为非正常数据包，应断开此连接;connID:%d;IP:%s;长度:%d;+++++\r\n"),
			m_pstuClientBaseInfo->dwConnID, m_pstuClientBaseInfo->szIP,nRcvLen);
		return -2;
	}	

	//检查流水号
	if (!CheckSerialNo(pstuDataHead->dwSerialNo))
	{
		TRACE(_T("+++++流水号相同,过滤此类数据包;connID:%d;IP:%s; 长度:%d;+++++\r\n"),
			m_pstuClientBaseInfo->dwConnID, m_pstuClientBaseInfo->szIP);
		return 0;
	}

	//包头检查成功 则申请新数据空间
	if (!m_pstuDataPacket)
	{
		int iCount = (int)ceil((double)(pstuDataHead->dwPacketLen+16)/(double)sizeof(DATAPACKET));

		m_pstuDataPacket = m_pobjRecvPacketPool->New(iCount);

		if (!m_pstuDataPacket)
		{
			return -3;
		}
		m_pstuDataPacket->Init(iCount*sizeof(DATAPACKET));
		m_pstuDataPacket->Reset();
		//TRACE("#####收到数据确认申请新空间，地址:%0X;iCount:%d\r\n", m_pstuDataPacket, iCount);
	}
	return 1;
}

//-5之前为CheckComplete的返回值  -1包头错误 -2向处理缓冲区添加数据失败 -3包处理失败 >0成功
//数据数据，如果长度不够整个数据包，则返回已经接收数据的总长度
int CClientBusinessHelper::Decode(const char* pRecv, const int nRcvLen)
{
	int iRet = 0;
	int iCompleteLen = 0;    //已经处理完成的长度
	TRACE(_T("<-收到:%s;数据;长度:%d;内容:%s;\r\n"), m_pstuClientBaseInfo->szIP, nRcvLen, pRecv);

	//检查正确 则应该处理数据
	while(iCompleteLen < nRcvLen)
	{  
		//-2 检查包头失败  ---不符合类型的数据包  此处应断开连接
		iRet = CheckHeadPaceket(pRecv+iCompleteLen, nRcvLen-iCompleteLen);
		if (1 != iRet)
		{
			//TRACE(_T("CheckHeadPaceket返回值: %d(0重复包-1包头错误-2错误包-3内存申请失败);包类型:%X;包长度:%d;本次接收长度:%d;\r\n"), 
			//	iRet, m_pstuDataPacket->pstuDataHead->dwPacketType,m_pstuDataPacket->pstuDataHead->dwPacketLen, nRcvLen);
			return 0 == iRet? 0 : -1 ;
		}

		//向处理缓冲区添加数据  一次最多一个包
		iRet = m_pstuDataPacket->AddRecvData((PBYTE)pRecv+iCompleteLen, nRcvLen-iCompleteLen);

		if (iRet<0)
		{
			TRACE(_T("AddRecvData出错: %d;包长度:%d;本次接收长度:%d;IP:%s;\r\n"), 
				iRet, m_pstuDataPacket->pstuDataHead->dwPacketLen, nRcvLen,
				m_pstuClientBaseInfo->szIP);
			ReceiveReset();
			return -2;
		}
		//不够一个包,继续去接收
		if (0 == iRet)	break;

		//iRet>0 已经满一个包  数据接收完成 开始处理  一次只处理一个数据包
		if (HandlePacket()<=0)
		{
			//处理失败
			TRACE(_T("*HandlePacket*处理失败,包长度:%0X; IP:%s;\r\n"),  
				m_pstuDataPacket->pstuDataHead->dwPacketLen, m_pstuClientBaseInfo->szIP);	
			ReceiveReset();
			return -3;
		}
		else
		{
			//TRACE("#####处理完成则释放申请的内存，地址:%0X\r\n", m_pstuDataPacket);
			//TRACE("#####处理完成,包类型:%X包长度:%0X; IP:%s; Barcode:%s;\r\n", m_pstuDataPacket->pstuDataHead->dwPacketType, 
			//	m_pstuDataPacket->pstuDataHead->dwPacketLen, m_pstuClientBaseInfo->szIP, m_pstuClientBaseInfo->szBarCode);	
		}

		//处理完成则释放申请的内存
		ReceiveReset();	

		//超过一个包  继续循环
		iCompleteLen += iRet;		
	}
	//ReceiveReset();	
	//m_clsCriSecLock.Unlock();
	return iRet;
}

#include "file.h"
bool WriteXML(TCHAR* ptszInFileName, PBYTE pbyInBuf, int iBufLen)
{
	RG::CFile clsFile;
	if (!clsFile.Open(ptszInFileName, "wb"))
	{
		TRACE("^&^! OpenFile Open 打开文件:%s出错！！！\r\n", ptszInFileName);
		return false;
	}
	int iFileLen = clsFile.Write(pbyInBuf, iBufLen);
	clsFile.Flush();
	clsFile.Close();
	return true;
}

#include "../third_party/pugixml/pugixml.hpp"
#include "../third_party/pugixml/pugiconfig.hpp"

//开始处理数据  成功返回0  失败返回-5以后的值
int CClientBusinessHelper::HandlePacket()
{
	int iRet = 0;	
	TRACE(_T("<-收到数据包长度:%d;IP:%s;\r\n"), m_pstuDataPacket->pstuDataHead->dwPacketLen,
		m_pstuClientBaseInfo->szIP);

	//TRACE(_T("<-内容:%s;\r\n"), m_pstuDataPacket->szbyData+sizeof(DATAHEAD));
	PBYTE pbyTem = m_pstuDataPacket->szbyData+sizeof(DATAHEAD);
	
	USERSTATUSINFO stuUserStatusInfo;
	memset(&stuUserStatusInfo, 0, sizeof(USERSTATUSINFO));

	if (!GetUserStatusInfo(&stuUserStatusInfo))
	{
		m_pstuDataPacket->pstuDataHead->dwReturn = TYPE_ERROR;		
	}
	else
	{
		TRACE(_T("流水号:%d\r\n"), m_pstuDataPacket->pstuDataHead->dwSerialNo);
		TRACE(_T("Code:%d;description:%s;cardtype:%s;cardid:%s;name:%s;room:%d;ip:%s;mac:%s;time:%s\r\n"),
			stuUserStatusInfo.emUserStatus,
			stuUserStatusInfo.tszDescription,
			stuUserStatusInfo.tszCardType,
			stuUserStatusInfo.tszCardID,
			stuUserStatusInfo.tszUserName,
			stuUserStatusInfo.iRoomID,
			stuUserStatusInfo.tszIP,
			stuUserStatusInfo.tszMAC,
			stuUserStatusInfo.tszTime);
		iRet = 1;
		m_pstuDataPacket->pstuDataHead->dwReturn = TYPE_OK;
		g_queueUserStatusInfo.push(stuUserStatusInfo);
	}

	//到这里数据已经收取完成  需要作进一步处理
	
	//if (g_bIsRujia)	m_pstuDataPacket->pstuDataHead->HtonlEx();
	m_pstuDataPacket->pstuDataHead->dwPacketLen = sizeof(DATAHEAD);
	TRACE(_T("回复数据包；流水号:%d;返回值:%d\r\n"), m_pstuDataPacket->pstuDataHead->dwSerialNo, m_pstuDataPacket->pstuDataHead->dwReturn);
	Send(m_pstuDataPacket->szbyData, sizeof(DATAHEAD));

	return iRet;
}

bool CClientBusinessHelper::GetUserStatusInfo(PUSERSTATUSINFO pstuOutUserStatusInfo)
{
	if(!pstuOutUserStatusInfo)	return false;

	int iXmlUtf8Len = m_pstuDataPacket->pstuDataHead->dwPacketLen - sizeof(DATAHEAD);
	PBYTE pbyTem = m_pstuDataPacket->szbyData+sizeof(DATAHEAD);

	//申请XML缓冲区
	int iXmlLen = (int)ceil((double)(iXmlUtf8Len*2+16)/(double)sizeof(DATAPACKET));
	char* pXmlTem = (char*)m_pobjRecvPacketPool->New(iXmlLen);
	if (!pXmlTem)
	{
		TRACE(_T("CClientBusinessHelper::GetUserStatusInfo;申请缓冲区出错\r\n"));
		return false;
	}
	iXmlLen = iXmlLen*sizeof(DATAPACKET);
	memset(pXmlTem, 0, iXmlLen);

	//转换格式为UTF-8
	iXmlLen = RG::Convert("UTF-8", "GBK", pXmlTem, iXmlLen, (char*)pbyTem, iXmlUtf8Len);

	//XML解析
	pugi::xml_document doc;
	doc.load((char*)pXmlTem);
	pugi::xml_node xml_Node_Data = doc.child("data");

	std::string strCode = xml_Node_Data.child("code").first_child().value();
	std::string strDescription = xml_Node_Data.child("description").first_child().value();
	std::string strCardType = xml_Node_Data.child("cardtype").first_child().value();
	std::string strcardid = xml_Node_Data.child("cardid").first_child().value();
	std::string strName = xml_Node_Data.child("name").first_child().value();
	std::string strRoom = xml_Node_Data.child("room").first_child().value();
	std::string strIP = xml_Node_Data.child("ip").first_child().value();
	std::string strMAC = xml_Node_Data.child("mac").first_child().value();
	std::string strTime = xml_Node_Data.child("time").first_child().value();


	pstuOutUserStatusInfo->emUserStatus = (USER_STATUS)atoi(xml_Node_Data.child("code").first_child().value());
	strcpy_s(pstuOutUserStatusInfo->tszDescription, 128*sizeof(TCHAR), strDescription.c_str());

	//switch(strCardType.c_str())
	//{
	//case "GID":   //工作证
	//	pstuOutUserStatusInfo->emCardType = GID;break;
	//case "ID":    //身份证
	//	pstuOutUserStatusInfo->emCardType = ID;break;
	//case "JID":   //军人证
	//	pstuOutUserStatusInfo->emCardType = JID;break;
	//case "JLZ":   //外国人居留证
	//	pstuOutUserStatusInfo->emCardType = JLZ;break;
	//case "JZ":    //驾照
	//	pstuOutUserStatusInfo->emCardType = JZ;break;
	//case "TXZ":   //通行证
	//	pstuOutUserStatusInfo->emCardType = TXZ;break;
	//case "VSA":   //护照
	//	pstuOutUserStatusInfo->emCardType = VSA;break;
	//case "XZ":    //回乡证
	//	pstuOutUserStatusInfo->emCardType = XZ;break;
	//case "ZQT":     //其他证件
	//	pstuOutUserStatusInfo->emCardType = ZQT;break;
	//}
	strcpy_s(pstuOutUserStatusInfo->tszCardType, 8*sizeof(TCHAR), strCardType.c_str());
	strcpy_s(pstuOutUserStatusInfo->tszCardID, 128*sizeof(TCHAR), strcardid.c_str());
	strcpy_s(pstuOutUserStatusInfo->tszUserName, MAX_PATH*sizeof(TCHAR), strName.c_str());
	pstuOutUserStatusInfo->iRoomID = atoi(xml_Node_Data.child("room").first_child().value());
	strcpy_s(pstuOutUserStatusInfo->tszIP, 32*sizeof(TCHAR), strIP.c_str());
	strcpy_s(pstuOutUserStatusInfo->tszMAC, 32*sizeof(TCHAR), strMAC.c_str());
	strcpy_s(pstuOutUserStatusInfo->tszTime, 32*sizeof(TCHAR), strTime.c_str());


	//保存文件
	TCHAR tszTem[MAX_PATH] = {0};
	TCHAR tszXMLPath[MAX_PATH] = {0};
	if (GetExePath(tszTem))
	{
		
		_stprintf_s(tszXMLPath, MAX_PATH, "%s\\xml\\%d_%s(%s).xml", tszTem,
			pstuOutUserStatusInfo->emUserStatus, pstuOutUserStatusInfo->tszUserName, pstuOutUserStatusInfo->tszCardID);
		doc.save_file(tszXMLPath);

		//_stprintf_s(tszXMLPath, MAX_PATH, "%s\\%d_(%s).xml", tszTem,
		//	pstuOutUserStatusInfo->emUserStatus, pstuOutUserStatusInfo->tszCardID);
		//WriteXML(tszXMLPath, (PBYTE)pXmlTem, iXmlLen);
	}	

	//释放空间
	m_pobjRecvPacketPool->Delete((PDATAPACKET)pXmlTem);
	pXmlTem = NULL;

	return pstuOutUserStatusInfo->Check();
}
