#ifndef _datahelper_h_
#define _datahelper_h_
class CDataHelper
{
private:
	static _ConnectionPtr m_pConnPtr;
	static char szConn[512];
public:
	static BOOL InitConnection();
	static void ExitConnection();
	static void GetRegConfig(LPCTSTR pKey,char *pValue);
	static BOOL ExecuteSql(LPCTSTR lpSql/*, int iType = 0*/);
	static BOOL TestConnection();
	static BOOL OpenConnection();
	static BOOL AddPhoneAndPwd(const char *pszPhone,const char *pszPwd);
};
#endif