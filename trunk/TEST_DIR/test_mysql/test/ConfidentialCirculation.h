

//�ܼ���confidential��
typedef struct _CONFIDENTIAL
{
	char szCID[50];        //�ܼ�id(����)
	char szTitle[50];      //�ܼ�����
	int  iPID;             //�����ܼ�ԱID(pid)
	int  iReadTime;        //�ɽ���ʱ��(readtime)
	char szCreatTime[25];  //����ʱ��
	char szEndTime[25];    //���ʱ��
}stuConfidential,*pstuConfidential;

//�����level��
typedef struct _LEVEL
{
	int iID;            //����id
	char szName[50];     //��������
}stuLevel,*pstuLevel;


//ǩ�ձ�sign��
typedef struct _SIGN
{
	int iSID;                    //ǩ�ձ�id��sid��
	int iCID;                    //�ܼ�id��cid��
	int iSID;                    //�쵼id��lsid��
	char szSignTime[25];         //ǩ��ʱ�䣨signtime��
	char szReturnTime[25];       //�黹ʱ�䣨returntime��
	char szLateReturnTime[25];   //����鵵ʱ�䣨latereturntime��
}stuSign,*pstuSign;

//�ܼ�-���ļ����conf_accesslevel��
typedef struct _CONF_ACCESSLEVEL
{
	int iALID;   //�����id��alid��
	char szCID;  //�ܼ�id��cid��
	int iLID;    //����id��lid��
};

//������֮���ָ���쵼
//�ܼ�-Ȩ�ޱ�conf_ authority��
typedef struct _CONF_AUTHORITY
{
	int iATID;    //Ȩ�ޱ�id��atid��
	char szCID;   //�ܼ�id��cid��
	int iLSID;    //�쵼id��lsid��
};

//�û�����persontype��
typedef struct _PERSONTYPE
{
	int iPTID;     //����id��ptid��
	char szPTName; //������ƣ�ptname��
}stuPersonType, *pstuPersonType;


//�����home��
typedef struct _HOME
{
	int iHID;              //����id��hid��
	char szHomeNum[50];    //����ţ�hnumber��
	char szHomeName[50];   //�������ƣ�hname��
};