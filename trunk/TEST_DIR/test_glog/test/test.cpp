// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#pragma pack(1)
typedef struct CUSTOMER_LIGIN_OUT_LOG
{
	long lID;                   //ID（见属性）（改长整形）
	char szBarCode[16];         //场所编号
	char szCusName[32];         //用户姓名
	int iCusGender;             //用户性别
	int iCardType;              //卡类型
	int iCertType;              //证件类型
	char szCertNo[32];          //证件号码
	DATE dtLoginTime;           //上机时间
	char szHostIP[16];          //登录主机IP
	char szHostName[32];        //登录主机名称
	char szHostMac[32];         //主机MAC地址
	DATE dtRcvTime;             //接收时间
	DATE dtLogoutTime;          //下机时间
	int iIsUpdate;              //是否上传
	char szComment[256];        //备注
	char szCusBarCode[32];      //用户卡号
	char szCusPeople[32];       //用户民族
	CUSTOMER_LIGIN_OUT_LOG()
	{
		Reset();
	}
	~CUSTOMER_LIGIN_OUT_LOG()
	{
		Reset();
	}

	//重置数据包内容为0
	inline void Reset()
	{
		memset(this, 0, sizeof(*this));	
	}
}CUSTOMERLOGINOUTLOG, *PCUSTOMERLOGINOUTLOG;

typedef struct DATA_HEADTAG
{
	char  szHeadFlag[4];    // 头标识 ‘RS01’
	DWORD dwVersion;        // 版本号  0x0001
	DWORD dwSerialNo;       // 流水号  从1开始使用，到达最大值后，再从1开始【循环使用]
	DWORD dwPacketType;     // 包数据类型  最高位为1，为回复数据。
	DWORD dwPacketLen;      //sizeof(HEADTAG) + 包身
	DWORD dwReturn;     	//返回值  成功或失败
	WORD  wEncode;          // 加密算法[0=未加密；1=异或加密；]	
	WORD  wHeadLen;         // 数据头长度

	//检测数据头是否正常 包头正常返回true，否false
	//inline bool Check()
	//{
	//	return memcmp(this->szHeadFlag, PACKAGE_MARK, PACKAGE_MARK_LEN)?false: true;
	//}
	////重置包头结构为0
	//inline void Reset()
	//{
	//	memset(this, 0, sizeof(*this));
	//}
}DATAHEAD, *PDATAHEAD;
#pragma pack()
int _tmain(int argc, _TCHAR* argv[])
{
	size_t szLen = sizeof(CUSTOMERLOGINOUTLOG);
	size_t szLen2 = sizeof(DATAHEAD);

	char szTem[1024] = {0};
	//CUSTOMERLOGINOUTLOG stuCustomerLoginoutLog = {0};  //用户信息包
	//sprintf_s(szTem, 1024, "%l\a %s\a %s\a  %d\a%d\a%d\a  %s\a%s\a%s\a%s\a%s\a%s\a %s\a  %d\a %s\a%s\a%s\a\r\n",
	//	stuCustomerLoginoutLog.lID,
	//	stuCustomerLoginoutLog.szBarCode,
	//	stuCustomerLoginoutLog.szCusName,
	//	stuCustomerLoginoutLog.iCusGender,
	//	stuCustomerLoginoutLog.iCardType,
	//	stuCustomerLoginoutLog.iCertType,
	//	stuCustomerLoginoutLog.szCertNo,
	//	DateTimeToString(stuCustomerLoginoutLog.dtLoginTime),
	//	stuCustomerLoginoutLog.szHostIP,
	//	stuCustomerLoginoutLog.szHostName,
	//	stuCustomerLoginoutLog.szHostMac,
	//	DateTimeToString(stuCustomerLoginoutLog.dtRcvTime),
	//	DateTimeToString(stuCustomerLoginoutLog.dtLogoutTime),
	//	stuCustomerLoginoutLog.iIsUpdate,
	//	stuCustomerLoginoutLog.szComment,
	//	stuCustomerLoginoutLog.szCusBarCode,
	//	stuCustomerLoginoutLog.szCusPeople
	//	);
	return 0;
}

//
//
//#include <glog/logging.h>
//#include <gflags/gflags.h>
//
//DEFINE_int32(port, 0 ,"what port to listen on"); # 实际可以使用FLAGS_port 
//DEFINE_string(log_dir, "./", "path");#实际可以使用FLAGS_log_dir
//
//int main(int argc, char* argv[]) 
//{
//	//Initialize Google's logging library.
//	google::InitGoogleLogging(argv[0]);
//	int num_cookies = 10;
//	LOG(INFO) << "Found " << num_cookies << " cookies";
//	return 0;
//}