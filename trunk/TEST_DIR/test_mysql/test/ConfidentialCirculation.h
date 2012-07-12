

//密件表（confidential）
typedef struct _CONFIDENTIAL
{
	char szCID[50];        //密件id(条码)
	char szTitle[50];      //密件标题
	int  iPID;             //所属密件员ID(pid)
	int  iReadTime;        //可借阅时间(readtime)
	char szCreatTime[25];  //创建时间
	char szEndTime[25];    //归结时间
}stuConfidential,*pstuConfidential;

//级别表（level）
typedef struct _LEVEL
{
	int iID;            //级别id
	char szName[50];     //级别名称
}stuLevel,*pstuLevel;


//签收表（sign）
typedef struct _SIGN
{
	int iSID;                    //签收表id（sid）
	int iCID;                    //密件id（cid）
	int iSID;                    //领导id（lsid）
	char szSignTime[25];         //签收时间（signtime）
	char szReturnTime[25];       //归还时间（returntime）
	char szLateReturnTime[25];   //最晚归档时间（latereturntime）
}stuSign,*pstuSign;

//密件-查阅级别表（conf_accesslevel）
typedef struct _CONF_ACCESSLEVEL
{
	int iALID;   //级别表id（alid）
	char szCID;  //密件id（cid）
	int iLID;    //级别id（lid）
};

//除级别之外的指定领导
//密件-权限表（conf_ authority）
typedef struct _CONF_AUTHORITY
{
	int iATID;    //权限表id（atid）
	char szCID;   //密件id（cid）
	int iLSID;    //领导id（lsid）
};

//用户类别表（persontype）
typedef struct _PERSONTYPE
{
	int iPTID;     //类别表id（ptid）
	char szPTName; //类别名称（ptname）
}stuPersonType, *pstuPersonType;


//房间表（home）
typedef struct _HOME
{
	int iHID;              //房间id（hid）
	char szHomeNum[50];    //房间号（hnumber）
	char szHomeName[50];   //房间名称（hname）
};