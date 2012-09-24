////////////////////////////////////////////////////////////////////////////////////////////////////////
 //
 // FileName:   CSqlite.h
 // Description:��װSQLite ��ʹ����, ��ǰʹ�õ�SQLite �汾��3.4.0
 // Function:   ����SQLite �ļ̳���
 // CreateTime:	2009.02.02
 // Author:     Zhang Pengfei
 // History:    2009.02.03 ���Ӻ���IsOpen()
 //             2009.02.03 ���Ӻ���FreeStmt(void)
 //             2009.02.03 ���Ӻ���Fetch( vector<string> & ioValues, vector<int>& ioNull ) �����п�ֵ��¼
 //				2009.04.22 ���Ӻ���ExecQuery(const char * inSql,  char **dbResult, int &nRow, int &nColumn, char *&errmsg);
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

//ͷ�ļ�����
#include "sqlite3.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////
// ����һЩ������
#define  DEF_INPUTISNULL        -1        /* �������Ϊ�� */
#define  DEF_TABLEEXIST         -2        /* ����� */
#define  DEF_TABLENOTEXIST      -3        /* ������ */

extern const char * STRBEGINTRANS;
extern const char * STRCOMMITTRANS;
extern const char * STRROLLBACK;

class CSQLite
{
///////////////////////////////////////// ��Ա��������////////////////////////////////////////////////
public:

private:
    sqlite3 *         m_pDb;             // SQLite ����ָ��
    sqlite3_stmt *    m_pStmt;           // ִ��SQL �ľ��
    int               m_nState;          // ��ǰSQLite ״̬
    string            m_strFileName;     // SQLite�Ŀ��ļ���
    string            m_strErrInfo;      // ������Ϣ
    unsigned long     m_nTimeoutMs;      // ���ó�ʱ������

    unsigned int      m_nCurFieldNum;    // ��ǰ��ѯ�ı����ֶε��ܸ���
    unsigned int      m_nLimitNum;       // ִ��SQLæʱ���ĳ��Դ���
    bool  m_bIsOpen;                     // ���ݿ��Ƿ��

/////////////////////////////////////////// �ӿں�������////////////////////////////////////////////
public:
    // ��������������
    CSQLite( void );
    virtual ~CSQLite( void );

	//////////////////////////////////////////////////////////////////////////////////////////
    // ����:	��SQLite
    // ����:	inDbName:          �򿪻򴴽������ݿ��ļ���
    //			inSynFlag:         �Ƿ��ͬ������
    // ����ֵ:	true:              �ɹ���SQLite
    //			false:             ��SQLiteʧ��
    bool Open( const char * inDbName, bool inSynFlag = false );

	/////////////////////////////////////////////////////////////////////////////////////////
    // ����:	���´�SQLite
    // ����:	inSynFlag:         �Ƿ��ͬ������
    // ����ֵ:	true:              �ɹ���SQLite
    //			false:             ��SQLiteʧ��
    bool Reopen( bool inSynFlag = false );

    // �ر�SQLite
    void Close( void );

	////////////////////////////////////////////////////////////////////////////////////////
    // ����:	���ó�ʱʱ��
    // ����:	inMs:              ���õĺ�����
    // ����ֵ:	SQLITE_OK:      ִ�гɹ�
    //			SQLITE_BUSY:    SQLITE�ļ�������
    //			other:          ����������Ϣ
    int SetBusyTimeout( unsigned int inMS ) ;
    // ��ȡ��ʱʱ�� 
    unsigned int GetBusyTimeout( void ) const { return m_nTimeoutMs; }

    // ��ȡ��ǰSQLite ��״̬
    inline int GetState( void ) const { return m_nState; }

    // ��ȡ���һ�εĴ�����Ϣ
    inline const char * GetLastError( void ) const { return m_strErrInfo.c_str(); }

    // ��ȡִ��æʱ������Դ���
    inline unsigned int GetLimitNum( void ) const { return m_nLimitNum; }
    // ����ִ��æʱ������Դ���
    inline void SetLimitNum( unsigned int inLimitNum ) { m_nLimitNum = inLimitNum; }

	// �ж����ݿ��Ƿ��
   inline bool IsOpen( void ) { return m_bIsOpen; }

    // ת��������Ϊ�ַ���
    const char *  ErrCodeAsString( int inErrCode );

    //////////////////////////////////////////////////////////////////////////////////////////
    // ����:	Direct Execute SQL  DML  sentence
    // ����:	inSql:          ��Ҫִ�е�SQL���
    // ����ֵ:	SQLITE_OK:      ִ�гɹ�
    //			SQLITE_BUSY:    SQLITE�ļ�������
    //			other:          ����������Ϣ
    int ExecDML( const char * inSql );

    ////////////////////////////////////////////////////////////////////////////////////////
    // ����:	����ʽִ��һ��SQL���
    // ����:	inTransation:   ��Ҫִ�е�һ��SQL�������
    // ����ֵ:	SQLITE_OK:      ִ�гɹ�
    //			other:          ִ��ʧ��, ����������Ϣ
    int ExecDML(const vector<string>  & inTransation);

	int ExecQuery(const char * inSql, void * param, char * errmsg);

	////////////////////////////////////////////////////////////////////////////////////////
	// ����:	ִ��SQL��ѯ���
	// ����:	inSql:          ִ�в�ѯ��SQL���
	//			dbResult:		���ڱ����ѯ���
	//			nRow:			��ѯ��������
	//			nColumn:		��ѯ��������
	//			errmsg:			������Ϣ
	//����ֵ:	SQLITE_OK:      ִ�гɹ�
	//			other:          ִ��ʧ��, ����������Ϣ
	int ExecQuery(const char * inSql,  char **dbResult, int &nRow, int &nColumn, char *&errmsg);
    
	////////////////////////////////////////////////////////////////////////////////////////
    // ����:	ִ��SQL��ѯ���
    // ����:	inSql:          ִ�в�ѯ��SQL���
    // ����ֵ:	SQLITE_OK:      ִ�гɹ�
    //			other:          ִ��ʧ��, ����������Ϣ
    int ExecQuery(const char * inSql);

    ///////////////////////////////////////////////////////////////////////////////////////////
    // ����:	ȡһ����ѯ��¼
    // ����:	ioValues:      �������, ȡ�ò�ѯ�����Ҫ�õ����ֶ�ֵ
    //			ioNull,    �����ֵ�ı�־ 0  Ϊ��, 1 �ǿ�
    // ����ֵ:	SQLITE_BUSY:   ���ݿⱻ��
    //			SQLITE_ROW:    ����δȡ��, ��Ҫ������ȡ����
    //			SQLITE_DONE:   �����Ѿ���ȡ���
    //			other:         ��ȡ���ݿ��¼ʧ��
    int Fetch( vector<string> & ioValues );
    int Fetch( vector<string> & ioValues, vector<int>& ioNull );  //���ӿ�ֵ��¼, added by yinyong 2007.12.27
    
	///////////////////////////////////////////////////////////////////////////////////////////
	// ����:	���ָ���ı��Ƿ����
    // ����:	inTabName:             ��Ҫ���ı�
    // ����ֵ:	DEF_TABLEEXIST:        �����
    //			DEF_TABLENOTEXIST:     ������
    //			other:                 ִ��ʧ��, ����������Ϣ
    int TableExists( const char * inTabName );

	//////////////////////////////////////////////////////////////////////////////////////////
    // ����:	����һ������
    // ����ֵ:	SQLITE_OK:      ִ�гɹ�
    //			SQLITE_BUSY:    SQLITE�ļ�������
    //			other:          ����������Ϣ
    inline int BeginTrans( void )    { return ExecDML( STRBEGINTRANS );  }

	//////////////////////////////////////////////////////////////////////////////////////////
    // ����:	����һ������
    // ����ֵ:	SQLITE_OK:      ִ�гɹ�
    //			SQLITE_BUSY:    SQLITE�ļ�������
    //			other:          ����������Ϣ
    inline int CommitTrnas( void )   { return ExecDML( STRCOMMITTRANS ); }

	/////////////////////////////////////////////////////////////////////////////////////////
    // ����:	�ع�һ������
    // ����ֵ:	SQLITE_OK:      ִ�гɹ�
    //			SQLITE_BUSY:    SQLITE�ļ�������
    //			other:          ����������Ϣ
    inline int RollbackTrans( void ) { return ExecDML( STRROLLBACK );    }

    //Ĭ���ͷ����Աm_pStmt  //add 2007.12.3
   void FreeStmt( void );

private:
    CSQLite( const CSQLite & inSqlite );
    CSQLite & operator = ( const CSQLite & inSqlite );

};



