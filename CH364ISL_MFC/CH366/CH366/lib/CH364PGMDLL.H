
#ifndef		_CH36X_PGM_H
#define		_CH36X_PGM_H

#ifdef __cplusplus
extern "C" {
#endif

#define CH364CHIP 1
#define CH366CHIP 2
#define MAX_LID 0x1800
#define MAX_CART_COUNT 32
	
typedef	struct	_PCI_CONFIG {					// 定义PCI配置空间数据结构
	USHORT			mPcVendorId;				// 00H 供应商标识
	USHORT			mPcDeviceId;				// 02H 设备标识
	USHORT			mPcCommandReg;				// 04H 命令寄存器
	USHORT			mPcStatusReg;				// 06H 状态寄存器
	UCHAR			mPcRevisionId;				// 08H 修改标识
	UCHAR			mPcProgramIf;				// 09H 寄存器级编程接口
	UCHAR			mPcSubClass;				// 0AH 子类代码
	UCHAR			mPcBaseClass;				// 0BH 基本分类代码
	UCHAR			mPcCacheLine;				// 0CH 缓存行长度
	UCHAR			mPcLatenTimer;				// 0DH 延迟计数器
	UCHAR			mPcHeaderType;				// 0EH 头标类型
	UCHAR			mPcBistReg;					// 0FH 内含自测试寄存器
	ULONG			mPcBaseAddr0;				// 10H 基址寄存器0
	ULONG			mPcBaseAddr1;				// 14H 基址寄存器1
	ULONG			mPcBaseAddr2;				// 18H 基址寄存器2
	ULONG			mPcBaseAddr3;				// 1CH 基址寄存器3
	ULONG			mPcBaseAddr4;				// 20H 基址寄存器4
	ULONG			mPcBaseAddr5;				// 24H 基址寄存器5
	ULONG			mPcCardCis;					// 28H
	USHORT			mPcSubSysVen;				// 2CH 子系统供应商标识
	USHORT			mPcSubSysDev;				// 2EH 子系统设备标识
	ULONG			mPcExpansRom;				// 30H 扩展ROM基址寄存器
	UCHAR			mPcCapPtr;					// 34H
	UCHAR			mPcReserved1[3];			// 35H
	ULONG			mPcReserved2;				// 38H
	UCHAR			mPcInterLine;				// 3CH 中断线寄存器
	UCHAR			mPcInterPin;				// 3DH 中断引脚寄存器
	UCHAR			mPcMinGrant;				// 3EH
	UCHAR			mPcMaxLatency;				// 3FH
} mPCI_CONFIG, *mPPCI_CONFIG;

typedef	struct	_CH364_CFG_REG {	   // CH364芯片的配置寄存器
	mPCI_CONFIG		mCH364CfgPci;	   // 00H-3FH,共64字节为标准PCI配置空间
	UCHAR CH364CfgDout;                // 40H 输出寄存器
	UCHAR CH364CfgCtrl;                // 41H 控制寄存器
	UCHAR CH364CfgDin;                 // 42H 输入寄存器
	UCHAR CH364CfgSet;                 // 43H 配置寄存器
	//别名寄存器
	UCHAR AliaCH364CfgDout;            // 44H 输出寄存器别名
	UCHAR AliaCH364CfgCtrl;            // 45H 控制寄存器别名
	UCHAR AliaCH364CfgDin;             // 46H 输入寄存器别名
	UCHAR AliaCH364CfgSet;             // 47H 配置寄存器别名
	//4FH-44H 是43H-40H 的别名地址；7FH-50H 保留，只读00H
} mCH364_CFG_REG, *mPCH364_CFG_REG;

typedef	struct	_CH366_CFG_REG {	   // CH364芯片的配置寄存器
	mPCI_CONFIG		mCH366CfgPci;	   // 00H-3FH,共64字节为标准PCI配置空间
	UCHAR           ReservedBuf[192];  // 40-FFFH共4032字节为保留区,只保存至0XFF
} mCH366_CFG_REG, *mPCH366_CFG_REG;





//与应用层交互的卡的信息结构
typedef struct _CardContext
{
	ULONG CardID;         //卡位置
	UCHAR CardType;       //卡类型
	ULONG CfgRegBaseAddr; //卡基址
}mCardContext,*mPCardContext;

HANDLE	WINAPI	InitCH364IODrv(    //在使用CH364函数之前先用此函数先做初始化操作,初始化成功后返回一个有效的句柄.
	HWND            WindowsHandle);//程序主窗体名柄

HANDLE WINAPI InitCH364IODrv_Ex(
					HWND WindowsHandle,
					PCHAR SYSFilePath   //sys文件的路径
					);                  //在程序使用前加载打开驱动程序,返回当前被打开的驱动句柄.

BOOL	WINAPI	CloseCH364IODrv(
	HANDLE          DrvHandle);     //在结束程序前务必请先使用该函数来做关闭操作

BOOL	WINAPI	CH364mReadIoByte(   // 从I/O端口读取一个字节
	HANDLE          DrvHandle,
	PVOID			iAddr,          // 指定I/O端口的地址
	PUCHAR			oByte );        // 指向一个字节单元,用于保存读取的字节数据

BOOL	WINAPI	CH364mReadIoWord(   // 从I/O端口读取一个字
	HANDLE          DrvHandle,
	PVOID			iAddr,          // 指定I/O端口的地址
	PUSHORT			oWord );        // 指向一个字单元,用于保存读取的字数据

BOOL	WINAPI	CH364mReadIoDword(  // 从I/O端口读取一个双字
	HANDLE          DrvHandle,
	PVOID			iAddr,          // 指定I/O端口的地址
	PULONG			oDword );       // 指向一个双字单元,用于保存读取的双字数据

BOOL	WINAPI	CH364mWriteIoByte(  // 向I/O端口写入一个字节
	HANDLE          DrvHandle,
	PVOID			iAddr,          // 指定I/O端口的地址
	UCHAR			iByte );        // 待写入的字节数据

BOOL	WINAPI	CH364mWriteIoWord(  // 向I/O端口写入一个字
	HANDLE          DrvHandle,
	PVOID			iAddr,          // 指定I/O端口的地址
	USHORT			iWord );        // 待写入的字数据

BOOL	WINAPI	CH364mWriteIoDword(  // 向I/O端口写入一个双字
	HANDLE          DrvHandle,
	PVOID			iAddr,           // 指定I/O端口的地址
	ULONG			iDword );        // 待写入的双字数据

BOOL	WINAPI	CH364_FLASH_LOCK(  //对Flash-ROM存储器进行写保护设定
	HANDLE	        DrvHandle,     //初始化函数时返回的有效句柄
	UCHAR           WRLock)  ;     //为00H则解除写保护,为8CH则允许写保护/只读锁定,为0FFH则读写保护/完全锁定,只在复位后才能解锁

BOOL	WINAPI	FindCH364(         //搜索CH364芯片,获取相关信息
	HANDLE          DrvHandle,     //初始化函数时返回的有效句柄
	PULONG          CH364_ID,      //CH364芯片的位置号,可以事先指定起始芯片位置号
	PULONG          IS_CH364,      //是否是CH364卡     
	PVOID           CfgRegBaseAddr);//CH364配置寄存器基址

BOOL	WINAPI	CH364_FLASH_READ(  // 从Flash-ROM存储器读取数据块,输入时,读取的地址存放在mbuffer[0]..mbuffer[3]
	HANDLE          DrvHandle,	   //初始化函数时返回的有效句柄
	ULONG			iStartAddr,    // 指定读入的数据的地址
	PVOID			oBuffer,       // 指向一个足够大的缓冲区,用于保存读取的数据
	PULONG			oLength );     // 指向长度单元,输入时为准备读取的长度,返回后为实际读取的长度

BOOL	WINAPI	CH364_FLASH_ERASE( // 对Flash-ROM存储器的指定区域进行擦除
	HANDLE          DrvHandle,     //初始化函数时返回的有效句柄
	ULONG			iStartAddr,    // 要擦除的起始地址
	ULONG           iLength );     // 要擦除的字节数

BOOL	WINAPI	CH364_FLASH_WRITE( // 向存储器写入一个字节
	HANDLE          DrvHandle,     //初始化函数时返回的有效句柄
	ULONG           iStartAddr,    //写入flash内的起始地址
	PVOID           iBuffer,       //写入flash内的数据缓冲区
	PULONG          iLength);      //写入flash内的数据长度

//CH366接口函数
BOOL	WINAPI	CH366_FLASH_LOCK(  //对Flash-ROM存储器进行写保护设定
	HANDLE	        DrvHandle,     //初始化函数时返回的有效句柄
	UCHAR           WRLock)  ;     //为00H则解除写保护,为8CH则允许写保护/只读锁定,为0FFH则读写保护/完全锁定,只在复位后才能解锁

BOOL	WINAPI	FindCH366(         //搜索CH364芯片,获取相关信息
	HANDLE          DrvHandle,     //初始化函数时返回的有效句柄
	PULONG          CH364_ID,      //CH364芯片的位置号,可以事先指定起始芯片位置号
	PULONG          IS_CH366,           
	PVOID           CfgRegBaseAddr);//CH364配置寄存器基址

BOOL	WINAPI	CH366_FLASH_READ(  // 从Flash-ROM存储器读取数据块,输入时,读取的地址存放在mbuffer[0]..mbuffer[3]
	HANDLE          DrvHandle,	   //初始化函数时返回的有效句柄
	ULONG			iStartAddr,    // 指定读入的数据的地址
	PVOID			oBuffer,       // 指向一个足够大的缓冲区,用于保存读取的数据
	PULONG			oLength );     // 指向长度单元,输入时为准备读取的长度,返回后为实际读取的长度

BOOL	WINAPI	CH366_FLASH_ERASE( // 对Flash-ROM存储器的指定区域进行擦除
	HANDLE          DrvHandle,     //初始化函数时返回的有效句柄
	ULONG			iStartAddr,    // 要擦除的起始地址
	ULONG           iLength );     // 要擦除的字节数

BOOL	WINAPI	CH366_FLASH_WRITE( // 向存储器写入一个字节
	HANDLE          DrvHandle,     //初始化函数时返回的有效句柄
	ULONG           iStartAddr,    //写入flash内的起始地址
	PVOID           iBuffer,       //写入flash内的数据缓冲区
	PULONG          iLength);      //写入flash内的数据长度

BOOL	WINAPI	CH364_FLASH_LOCK(  //对Flash-ROM存储器进行写保护设定
	HANDLE	        DrvHandle,     //初始化函数时返回的有效句柄
	UCHAR           WRLock)  ;     //为00H则解除写保护,为8CH则允许写保护/只读锁定,为0FFH则读写保护/完全锁定,只在复位后才能解锁

BOOL	WINAPI	FindCH364Ex(  // 搜索CH364芯片,一次只能搜一次
							HANDLE DrvHandle,
							PULONG CH364_ID, //输入时是搜索的起始位置号,返回是存在的CH364芯片的位置
							PULONG IS_CH364,
							PVOID CfgRegBaseAddr,
							USHORT VendorID,     //在搜索卡时需匹配厂商ID
							USHORT DeviceID,     //在搜索卡时需匹配设备ID
							ULONG ClassID);      //在搜索卡时需匹配的类ID

BOOL	WINAPI	FindCH366Ex(  // 搜索CH366芯片,一次只能搜一次
							HANDLE          DrvHandle,  //驱动句柄
							PULONG          CH366_ID,   //CH364芯片的位置号,可以事先指定起始芯片位置号
							PULONG          IS_CH366,
							PVOID           CfgRegBaseAddr,   //卡的基址 
							USHORT VendorID,     //在搜索卡时需匹配厂商ID
							USHORT DeviceID,     //在搜索卡时需匹配设备ID
							ULONG ClassID);      //在搜索卡时需匹配的类ID

	
//支持CH36X多块卡接口函数
BOOL	WINAPI	CH36X_FLASH_LOCK(  //对Flash-ROM存储器进行写保护设定
	ULONG			mIndex,        //CH364卡序号
	UCHAR           WRLock);       //为00H则解除写保护,为8CH则允许写保护/只读锁定,为0FFH则读写保护/完全锁定,只在复位后才能解锁

BOOL	WINAPI	FindAllCH36X(      //搜索CH364芯片,获取相关信息
	HANDLE          DrvHandle,     //驱动句柄
	mPCardContext   CardContext,   //已搜索到的卡信息
	PULONG CardCount);             //PC上搜索到的卡的总数

BOOL	WINAPI	CH36X_FLASH_READ(  // 从Flash-ROM存储器读取数据块,输入时,读取的地址存放在mbuffer[0]..mbuffer[3]
	ULONG			mIndex,        // CH366卡序号
	ULONG			iStartAddr,    // 指定读入的数据的地址
	PVOID			oBuffer,       // 指向一个足够大的缓冲区,用于保存读取的数据
	PULONG			oLength );     // 指向长度单元,输入时为准备读取的长度,返回后为实际读取的长度

BOOL	WINAPI	CH36X_FLASH_ERASE( // 对Flash-ROM存储器的指定区域进行擦除
	ULONG			mIndex,        //CH366卡序号
	ULONG			iStartAddr,    // 要擦除的起始地址
	ULONG           iLength );     // 要擦除的字节数

BOOL	WINAPI	CH36X_FLASH_WRITE( // 向存储器写入一个字节
	ULONG			mIndex,        //CH366卡序号
	ULONG           iStartAddr,    //写入flash内的起始地址
	PVOID           iBuffer,       //写入flash内的数据缓冲区
	PULONG          iLength);      //写入flash内的数据长度

#ifdef __cplusplus
}
#endif

#endif		// _CH36X_PGM_H
