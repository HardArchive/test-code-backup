// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� TPCDATA_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// TPCDATA_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef TPCDATA_EXPORTS
#define TPCDATA_API __declspec(dllexport)
#else
#define TPCDATA_API __declspec(dllimport)
#endif


//// �����Ǵ� tpcData.dll ������
//class TPCDATA_API CtpcData {
//public:
//	CtpcData(void);
//	// TODO: �ڴ�������ķ�����
//};

extern TPCDATA_API int ntpcData;

TPCDATA_API int fntpcData(void);

#include "../global/comm_protocol.h"
TPCDATA_API bool Init(LPCTSTR pszBindAddress, USHORT usPort);

TPCDATA_API bool UnInit();
//��ȡ�û�״̬��Ϣ
TPCDATA_API int GetUserStatusInfo(PUSERSTATUSINFO pstuOutUserStatusInfo);