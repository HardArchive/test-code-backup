// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� THIRDPARTYCOMMUNICATION_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// THIRDPARTYCOMMUNICATION_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef THIRDPARTYCOMMUNICATION_EXPORTS
#define THIRDPARTYCOMMUNICATION_API __declspec(dllexport)
#else
#define THIRDPARTYCOMMUNICATION_API __declspec(dllimport)
#endif

// �����Ǵ� ThirdPartyCommunication.dll ������
class THIRDPARTYCOMMUNICATION_API CThirdPartyCommunication {
public:
	CThirdPartyCommunication(void);
	// TODO: �ڴ�������ķ�����

};

extern THIRDPARTYCOMMUNICATION_API int nThirdPartyCommunication;

THIRDPARTYCOMMUNICATION_API int fnThirdPartyCommunication(void);
