// Result.cpp: implementation of the CResult class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimpleScanner.h"
#include "Result.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResult::CResult()
{
	char buffer[1024];
	GetModuleFileName(NULL,buffer,MAX_PATH); //get the current program path
	CString str = CString(buffer);
	str = str.Left(str.ReverseFind('\\'));
	m_strFileName = str +"\\ScanResult.htm"; 
	CFile file(m_strFileName,CFile::modeCreate);

}

CResult::~CResult()
{

}

//write the summary to the file
void CResult::WriteHeader(int HostNum,int TcpNum, int UdpNum)
{
	CFile file(m_strFileName,CFile::modeWrite);
    CString summary = "<title>ɨ����</title><H1 Align=center>SimpleScanner ɨ����</H1>";
	CString strTemp;
	strTemp.Format("<h3><font color=blue>һ����%d���������</font></h2>",HostNum);
	summary += strTemp;

    strTemp.Format("<h3><font color=blue>���ŵ�TCP�˿���:%d</font></h3>",TcpNum);
	summary += strTemp;

	strTemp.Format("<h3><font color=blue>���ŵ�UDP�˿���:%d</font></h3>",UdpNum);
	summary += strTemp;

	m_strWriteToFile = summary;

	file.Write(m_strWriteToFile,m_strWriteToFile.GetLength());
}

void CResult::WriteString(CString str)
{
  CFile file(m_strFileName,CFile::modeWrite);
  m_strWriteToFile += str;
  file.Write(m_strWriteToFile,m_strWriteToFile.GetLength()); 
}

void CResult::WriteDetailLine(CString strHost, int TcpNum, long TcpPort[], int UdpNum, long UdpPort[])
{
	CString strTemp;
	strTemp.Format("������ַ��<font color=red>%s</font><br>",strHost);

	//write the TCP ports 
	m_strWriteToFile += strTemp;
	strTemp.Format("���ŵ�TCP�Ķ˿���Ŀ��%d<br>", TcpNum);
	m_strWriteToFile += strTemp;
	strTemp = "���ŵ�TCP�˿�:<br>";
	m_strWriteToFile += strTemp;
    
	
	for( int i = 0; i < TcpNum; i++)
	{
		strTemp.Format("%ld ", TcpPort[i]);
		strTemp += strTemp;
		if( i%5 == 0)
		{
			strTemp = strTemp + "<br>";
		}
	}
    m_strWriteToFile += strTemp;
    
	//Write the udp ports 
	strTemp.Format("���ŵ�UDP�Ķ˿���Ŀ��%d<br>", UdpNum);
	m_strWriteToFile += strTemp;
	strTemp = "���ŵ�Udp�˿�:<br>";
	m_strWriteToFile += strTemp;
	for( int j = 0; j < UdpNum; j++)
	{
		strTemp.Format("%ld ", UdpPort[j]);
		strTemp += strTemp;
		if( j%5 == 0)
		{
			strTemp = strTemp + "<br>";
		}
	}
    m_strWriteToFile += strTemp;
	
}

void CResult::WritePort(CString strHost, long existTcpPort[],long TcpLength,long existUdpPort[],long UdpLength)
{
  CFile file(m_strFileName,CFile::modeWrite);
  CString strTemp;
  strTemp.Format("<br><font size = +2 color= blue> ������%s</font><br>",strHost);
  m_strWriteToFile += strTemp;
  

  //write the TCP ports to file 
  strTemp.Format("<font size = +1 color = green> ���ŵ�TCP�˿ڣ�</font><br>");
  m_strWriteToFile += strTemp;
  
  
  for( long i = 0; i <TcpLength; i ++ )
  {
	  if( existTcpPort[i] != -1 )
	  {
        strTemp.Format("<font size = 3 color = red> %ld</font><br>", existTcpPort[i]);
		m_strWriteToFile += strTemp;
	  }
  }

  //write the UDP ports to file 
  strTemp.Format("<font size = +1 color = green> ���ŵ�UDP�˿ڣ�</font><br>");
  m_strWriteToFile += strTemp;

  for( long j = 0; j < UdpLength; j ++ )
  {
	  if( existUdpPort[j] != -1 )
	  {
        strTemp.Format("<font size = 3 color = red> %ld</font><br>", existUdpPort[j]);
		m_strWriteToFile += strTemp;
	  }

  }
 
 file.Write(m_strWriteToFile,m_strWriteToFile.GetLength()); 


}

void CResult::WritePingHeader(int hostNum)
{
 	CFile file(m_strFileName,CFile::modeWrite);
    CString summary = "<title>ɨ����</title><H1 Align=center>SimpleScanner ɨ����</H1>";
	CString strTemp;
	strTemp.Format("<h3><font color=blue>һ����%d���������</font></h2>",hostNum);
	summary += strTemp;
    

	strTemp.Format("<font size=3 color=blue>���������б����£�</font><br>");
	summary += strTemp;

	m_strWriteToFile = summary;

	file.Write(m_strWriteToFile,m_strWriteToFile.GetLength());
}

void CResult::SignTime()
{
 CFile file(m_strFileName,CFile::modeWrite);
 CTime tm = CTime::GetCurrentTime();
 CString strTime;
 strTime.Format("<h4 align=right><font color=green><i>ɨ��ʱ��:%d-%d-%d,%d:%d:%d</font></h4>",tm.GetYear(),tm.GetMonth(),
	 tm.GetDay(),tm.GetHour(),tm.GetMinute(),tm.GetSecond());
 m_strWriteToFile += strTime;
 file.Write(m_strWriteToFile,m_strWriteToFile.GetLength());
}
