#ifndef __TPCDATA__H__
#define __TPCDATA__H__
// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 TPCDATA_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// TPCDATA_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
//#ifdef TPCDATA_EXPORTS
//#define TPCDATA_API __declspec(dllexport)     //这里用于DLL导出
//#else
//#define TPCDATA_API __declspec(dllimport)     //这里用于exe从dll导入
//#endif

#ifdef TPCDATA_EXPORTS
#define TPCDATA_API extern "C" __declspec(dllexport)
#else
#define TPCDATA_API extern "C" __declspec(dllimport)
#endif

//// 此类是从 tpcData.dll 导出的
//class TPCDATA_API CtpcData {
//public:
//	CtpcData(void);
//	// TODO: 在此添加您的方法。
//};

//extern TPCDATA_API int ntpcData;

//TPCDATA_API int fntpcData(void);

#include "../global/comm_protocol.h"

//初始化dll函数  传入服务器IP 端口 用来接收第三的数据
TPCDATA_API bool Init(LPCTSTR pszBindAddress, USHORT usPort);

//关闭服务器 
TPCDATA_API bool UnInit();
//获取用户状态信息  返回值 1获取成功 0 输入参数错误 -1队列为空 没有值  -2指针所指空间太小  拷贝出错
TPCDATA_API int GetUserStatusInfo(PUSERSTATUSINFO pstuOutUserStatusInfo);

//导出函数类型
typedef bool (*fn_Init)(LPCTSTR pszBindAddress, USHORT usPort);
typedef bool (*fn_UnInit)();
typedef bool (*fn_GetUserStatusInfo)(PUSERSTATUSINFO pstuOutUserStatusInfo);

//导出类类型
////I_DLLInterface类名
//typedef I_DLLInterface* (*PF_GetDLLInterface)();
// 

#endif /*__TPCDATA__H__*/