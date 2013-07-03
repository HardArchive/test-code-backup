#ifndef __TPCDATA__H__
#define __TPCDATA__H__
// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� TPCDATA_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// TPCDATA_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
//#ifdef TPCDATA_EXPORTS
//#define TPCDATA_API __declspec(dllexport)     //��������DLL����
//#else
//#define TPCDATA_API __declspec(dllimport)     //��������exe��dll����
//#endif

#ifdef TPCDATA_EXPORTS
#define TPCDATA_API extern "C" __declspec(dllexport)
#else
#define TPCDATA_API extern "C" __declspec(dllimport)
#endif

//// �����Ǵ� tpcData.dll ������
//class TPCDATA_API CtpcData {
//public:
//	CtpcData(void);
//	// TODO: �ڴ�������ķ�����
//};

//extern TPCDATA_API int ntpcData;

//TPCDATA_API int fntpcData(void);

#include "../global/comm_protocol.h"

//��ʼ��dll����  ���������IP �˿� �������յ���������
TPCDATA_API bool Init(LPCTSTR pszBindAddress, USHORT usPort);

//�رշ����� 
TPCDATA_API bool UnInit();
//��ȡ�û�״̬��Ϣ  ����ֵ 1��ȡ�ɹ� 0 ����������� -1����Ϊ�� û��ֵ  -2ָ����ָ�ռ�̫С  ��������
TPCDATA_API int GetUserStatusInfo(PUSERSTATUSINFO pstuOutUserStatusInfo);

//������������
typedef bool (*fn_Init)(LPCTSTR pszBindAddress, USHORT usPort);
typedef bool (*fn_UnInit)();
typedef bool (*fn_GetUserStatusInfo)(PUSERSTATUSINFO pstuOutUserStatusInfo);

//����������
////I_DLLInterface����
//typedef I_DLLInterface* (*PF_GetDLLInterface)();
// 

#endif /*__TPCDATA__H__*/