#include<windows.h>
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF","adoEOF")

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int iCmdShow)
{

	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;
	BOOL m_fConnected;

	HRESULT hr;
	_bstr_t source("Driver={SQL Server};Server=LONELYLONG;Uid=sa;Pwd=;Database=pubs");

	CoInitialize(NULL);

	char szConn[512];
	sprintf_s(szConn,512,"driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s", "192.168.30.124", "book", "sa", "sa");

	try{

		hr=m_pConnection.CreateInstance(__uuidof(Connection));
		if(SUCCEEDED(hr))
			hr=m_pConnection->Open(source,"","",16);
		if(SUCCEEDED(hr))
			hr=m_pRecordset.CreateInstance(__uuidof(Recordset));
		if(SUCCEEDED(hr))
			m_fConnected=TRUE;
		else
			m_fConnected=FALSE;
	}
	catch(_com_error &e)
	{
		//	MessageBox(NULL,e.ErrorMessage(),"error",NULL);
		m_fConnected=FALSE;
	}

	if(!m_fConnected)
		MessageBox(NULL,"ADO 数据源初始化失败！","error",NULL);
	else
		MessageBox(NULL,"SUCCESS!","OK",NULL);

	//	if(m_pConnection->State)
	//		m_pConnection->Close();

	return 0;
}