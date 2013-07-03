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
	//TRACE("�ͻ����๹�캯��; IP:%s; connID:%d\r\n", m_pstuClientBaseInfo->szIP, m_pstuClientBaseInfo->dwConnID);
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

//�˳����  �����ٽ��������������в���ʹ��  �ʴ˺��������ⲿ���� 
void CClientBusinessHelper::Release()
{
	ReceiveReset();
	m_pclsIOCPServer = NULL;
	m_pobjRecvPacketPool = NULL;
}

// ���ݸ�λ
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


//�����ˮ��  ��ͬ����false ��ͬ����true
inline bool CClientBusinessHelper::CheckSerialNo(DWORD dwInSerialNo)
{
	if (m_dwInSerialNo == dwInSerialNo) return false;
	m_dwInSerialNo = dwInSerialNo;
	return true;
}

 //��ͷ��� -1ָ�����  -2 ��ͷ���� -3 �������ռ�ʧ�� 0 �ظ����ݰ� 1 ���ɹ� ������ȷ
int CClientBusinessHelper::CheckHeadPaceket(const char* pRecv, const int nRcvLen)
{
	PDATAHEAD pstuDataHead = (PDATAHEAD)pRecv;
	if (!pstuDataHead) return -1;
	DWORD dwSerialNo = 0;

	//����ͷ
	if (!pstuDataHead->Check()) 
	{
		//��ͷ���ʧ�ܣ� �����Ƿ���δ��������ݰ�  ������Ӧ�öϿ�����
		if (m_pstuDataPacket) 
		{
			//TRACE("�˰�Ϊ��׺���հ�,����:%0X; ��������ݳ���:%d;�Ѿ����ճ���:%d;���ν��ճ���%d;\r\n", m_pstuDataPacket->pstuDataHead->dwPacketType,
			//	m_pstuDataPacket->pstuDataHead->dwPacketLen, m_pstuDataPacket->dwCurrentSize, nRcvLen);
			return 1;
		}
		TRACE(_T("��ͷ������,Ҳ����δ���������,�˰�Ϊ���������ݰ���Ӧ�Ͽ�������;connID:%d;IP:%s;����:%d;+++++\r\n"),
			m_pstuClientBaseInfo->dwConnID, m_pstuClientBaseInfo->szIP,nRcvLen);
		return -2;
	}	

	//�����ˮ��
	if (!CheckSerialNo(pstuDataHead->dwSerialNo))
	{
		TRACE(_T("+++++��ˮ����ͬ,���˴������ݰ�;connID:%d;IP:%s; ����:%d;+++++\r\n"),
			m_pstuClientBaseInfo->dwConnID, m_pstuClientBaseInfo->szIP);
		return 0;
	}

	//��ͷ���ɹ� �����������ݿռ�
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
		//TRACE("#####�յ�����ȷ�������¿ռ䣬��ַ:%0X;iCount:%d\r\n", m_pstuDataPacket, iCount);
	}
	return 1;
}

//-5֮ǰΪCheckComplete�ķ���ֵ  -1��ͷ���� -2���������������ʧ�� -3������ʧ�� >0�ɹ�
//�������ݣ�������Ȳ����������ݰ����򷵻��Ѿ��������ݵ��ܳ���
int CClientBusinessHelper::Decode(const char* pRecv, const int nRcvLen)
{
	int iRet = 0;
	int iCompleteLen = 0;    //�Ѿ�������ɵĳ���
	TRACE(_T("<-�յ�:%s;����;����:%d;����:%s;\r\n"), m_pstuClientBaseInfo->szIP, nRcvLen, pRecv);

	//�����ȷ ��Ӧ�ô�������
	while(iCompleteLen < nRcvLen)
	{  
		//-2 ����ͷʧ��  ---���������͵����ݰ�  �˴�Ӧ�Ͽ�����
		iRet = CheckHeadPaceket(pRecv+iCompleteLen, nRcvLen-iCompleteLen);
		if (1 != iRet)
		{
			//TRACE(_T("CheckHeadPaceket����ֵ: %d(0�ظ���-1��ͷ����-2�����-3�ڴ�����ʧ��);������:%X;������:%d;���ν��ճ���:%d;\r\n"), 
			//	iRet, m_pstuDataPacket->pstuDataHead->dwPacketType,m_pstuDataPacket->pstuDataHead->dwPacketLen, nRcvLen);
			return 0 == iRet? 0 : -1 ;
		}

		//���������������  һ�����һ����
		iRet = m_pstuDataPacket->AddRecvData((PBYTE)pRecv+iCompleteLen, nRcvLen-iCompleteLen);

		if (iRet<0)
		{
			TRACE(_T("AddRecvData����: %d;������:%d;���ν��ճ���:%d;IP:%s;\r\n"), 
				iRet, m_pstuDataPacket->pstuDataHead->dwPacketLen, nRcvLen,
				m_pstuClientBaseInfo->szIP);
			ReceiveReset();
			return -2;
		}
		//����һ����,����ȥ����
		if (0 == iRet)	break;

		//iRet>0 �Ѿ���һ����  ���ݽ������ ��ʼ����  һ��ֻ����һ�����ݰ�
		if (HandlePacket()<=0)
		{
			//����ʧ��
			TRACE(_T("*HandlePacket*����ʧ��,������:%0X; IP:%s;\r\n"),  
				m_pstuDataPacket->pstuDataHead->dwPacketLen, m_pstuClientBaseInfo->szIP);	
			ReceiveReset();
			return -3;
		}
		else
		{
			//TRACE("#####����������ͷ�������ڴ棬��ַ:%0X\r\n", m_pstuDataPacket);
			//TRACE("#####�������,������:%X������:%0X; IP:%s; Barcode:%s;\r\n", m_pstuDataPacket->pstuDataHead->dwPacketType, 
			//	m_pstuDataPacket->pstuDataHead->dwPacketLen, m_pstuClientBaseInfo->szIP, m_pstuClientBaseInfo->szBarCode);	
		}

		//����������ͷ�������ڴ�
		ReceiveReset();	

		//����һ����  ����ѭ��
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
		TRACE("^&^! OpenFile Open ���ļ�:%s��������\r\n", ptszInFileName);
		return false;
	}
	int iFileLen = clsFile.Write(pbyInBuf, iBufLen);
	clsFile.Flush();
	clsFile.Close();
	return true;
}

#include "../third_party/pugixml/pugixml.hpp"
#include "../third_party/pugixml/pugiconfig.hpp"

//��ʼ��������  �ɹ�����0  ʧ�ܷ���-5�Ժ��ֵ
int CClientBusinessHelper::HandlePacket()
{
	int iRet = 0;	
	TRACE(_T("<-�յ����ݰ�����:%d;IP:%s;\r\n"), m_pstuDataPacket->pstuDataHead->dwPacketLen,
		m_pstuClientBaseInfo->szIP);

	//TRACE(_T("<-����:%s;\r\n"), m_pstuDataPacket->szbyData+sizeof(DATAHEAD));
	PBYTE pbyTem = m_pstuDataPacket->szbyData+sizeof(DATAHEAD);
	
	USERSTATUSINFO stuUserStatusInfo;
	memset(&stuUserStatusInfo, 0, sizeof(USERSTATUSINFO));

	if (!GetUserStatusInfo(&stuUserStatusInfo))
	{
		m_pstuDataPacket->pstuDataHead->dwReturn = TYPE_ERROR;		
	}
	else
	{
		TRACE(_T("��ˮ��:%d\r\n"), m_pstuDataPacket->pstuDataHead->dwSerialNo);
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

	//�����������Ѿ���ȡ���  ��Ҫ����һ������
	
	//if (g_bIsRujia)	m_pstuDataPacket->pstuDataHead->HtonlEx();
	m_pstuDataPacket->pstuDataHead->dwPacketLen = sizeof(DATAHEAD);
	TRACE(_T("�ظ����ݰ�����ˮ��:%d;����ֵ:%d\r\n"), m_pstuDataPacket->pstuDataHead->dwSerialNo, m_pstuDataPacket->pstuDataHead->dwReturn);
	Send(m_pstuDataPacket->szbyData, sizeof(DATAHEAD));

	return iRet;
}

bool CClientBusinessHelper::GetUserStatusInfo(PUSERSTATUSINFO pstuOutUserStatusInfo)
{
	if(!pstuOutUserStatusInfo)	return false;

	int iXmlUtf8Len = m_pstuDataPacket->pstuDataHead->dwPacketLen - sizeof(DATAHEAD);
	PBYTE pbyTem = m_pstuDataPacket->szbyData+sizeof(DATAHEAD);

	//����XML������
	int iXmlLen = (int)ceil((double)(iXmlUtf8Len*2+16)/(double)sizeof(DATAPACKET));
	char* pXmlTem = (char*)m_pobjRecvPacketPool->New(iXmlLen);
	if (!pXmlTem)
	{
		TRACE(_T("CClientBusinessHelper::GetUserStatusInfo;���뻺��������\r\n"));
		return false;
	}
	iXmlLen = iXmlLen*sizeof(DATAPACKET);
	memset(pXmlTem, 0, iXmlLen);

	//ת����ʽΪUTF-8
	iXmlLen = RG::Convert("UTF-8", "GBK", pXmlTem, iXmlLen, (char*)pbyTem, iXmlUtf8Len);

	//XML����
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
	//case "GID":   //����֤
	//	pstuOutUserStatusInfo->emCardType = GID;break;
	//case "ID":    //���֤
	//	pstuOutUserStatusInfo->emCardType = ID;break;
	//case "JID":   //����֤
	//	pstuOutUserStatusInfo->emCardType = JID;break;
	//case "JLZ":   //����˾���֤
	//	pstuOutUserStatusInfo->emCardType = JLZ;break;
	//case "JZ":    //����
	//	pstuOutUserStatusInfo->emCardType = JZ;break;
	//case "TXZ":   //ͨ��֤
	//	pstuOutUserStatusInfo->emCardType = TXZ;break;
	//case "VSA":   //����
	//	pstuOutUserStatusInfo->emCardType = VSA;break;
	//case "XZ":    //����֤
	//	pstuOutUserStatusInfo->emCardType = XZ;break;
	//case "ZQT":     //����֤��
	//	pstuOutUserStatusInfo->emCardType = ZQT;break;
	//}
	strcpy_s(pstuOutUserStatusInfo->tszCardType, 8*sizeof(TCHAR), strCardType.c_str());
	strcpy_s(pstuOutUserStatusInfo->tszCardID, 128*sizeof(TCHAR), strcardid.c_str());
	strcpy_s(pstuOutUserStatusInfo->tszUserName, MAX_PATH*sizeof(TCHAR), strName.c_str());
	pstuOutUserStatusInfo->iRoomID = atoi(xml_Node_Data.child("room").first_child().value());
	strcpy_s(pstuOutUserStatusInfo->tszIP, 32*sizeof(TCHAR), strIP.c_str());
	strcpy_s(pstuOutUserStatusInfo->tszMAC, 32*sizeof(TCHAR), strMAC.c_str());
	strcpy_s(pstuOutUserStatusInfo->tszTime, 32*sizeof(TCHAR), strTime.c_str());


	//�����ļ�
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

	//�ͷſռ�
	m_pobjRecvPacketPool->Delete((PDATAPACKET)pXmlTem);
	pXmlTem = NULL;

	return pstuOutUserStatusInfo->Check();
}
