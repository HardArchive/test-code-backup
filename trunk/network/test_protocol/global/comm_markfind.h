//file TraceUtil.h
/***************************************************************************************************
* 1�� Class      �� comm_define
* 2�� Version    �� *.*
* 3�� Description�� ��������Ĺ���ͷ�ļ�
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-10-21 14:14:03
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/

#ifndef __COMM_DEFINE_H__
#define __COMM_DEFINE_H__



enum AppProtoclType
{
	ApplicationUnknown=0, Http=1, Ftp=2, Smtp=3, Pop=4, WebMail = 6,	Telnet=7,Mail=9,
	Msn=1001,Oicq=1002,Icq=1003 ,YahooMsg=1004,Uc=1006,Fetion=1020,
	// �Ա�����
	WangWang=1021,
	// ������Ϸ
	OurGame = 2001, 
	// �й�������Ϸ 
	ChinaGame=2002,
	// �߷�
	Game=2004,
	// ħ������
	MoshouShijie=2012,
	// ��Ѫ����
	ReXueJiangHu=2031,
	// ��������
	ChuanqiShijie=2065,
	// A3
	A3Game = 2123,
	// �ɾ���˵(RO)
	FaeryStoryGame=2153,
	// ����
	WanMei= 2182,
	// ����UT
	SinaUt=2206,
	// Yahoo��Ϸ �����޸� 2010-06-25 Զ����Ϸ[�������Ϳⶨ��]
	YhGame=2211,
	FtpFile = 3001, HttpKey=3002, //modify by rg & 2013-5-2 13:30:17 ��ԭ��13��Ϊ3002
	// �Ʒ�
	HaoFangGame=3003,WebAccounts=3004, WebPhoneNumber=3005,
	// QQ��Ϸ
	QQGame =3011,BT=3012, 
	EMule=3013,
	//BitComet=3014, 
	BitComet=9112,
	BaiduTB=3014,
	Kaixin = 3401,
	SinaMB=3403,
	TxMB=3404, 

	Lou19 = 3002,
	HongDou = 3032,
	mop	  = 3045,
	QiangGuo = 3052,
	Sohu	= 3058,
	TianYa = 3061,
	KaiDi = 3039,
	TongCH51 = 3046,
	TaoB	= 3060,
	Xilu = 3064,
	HongW = 3115,
	Dahe = 3139,
	Xici = 3140,
	HuaShang = 3144,
	KDShan = 3145,
	Renren = 3402,
	DangDang = 3802,
	JingDong = 3803,
	YiHao = 3804,
	Socks = 4001,
	SMB=4003,
	BreakByOtherGame = 5000,

	// ���������� ��¼		�����޸� 2010-06-21 ��Ӻ�����������
	HaoLiaoLiaoTian=7878,
	//// Զ����Ϸ�Ľ�ȡ		�����޸� 2010-06-23 ���Զ����Ϸ����
	//// �����޸� 2010-06-25 �����빫����ͻ��3025 ��Ϊ 3028
	//YuanHang = 3028	

	// �����޸� 2010-07-14 �����ʵ���Ϸ�ʺŽ�ȡ
	WenDao=9010,

	// ���������163����
	Mail126_Com = 6008,
	MailSohu_Com = 6010,
	MailTom_Com = 6017,
	Mail163_Com = 6009,
	MailSina_Com = 6003,
	MailQQ_Com = 6018,
	Mail139_Com = 6032,

	Radius = 20000,
	// �����޸� 2010-08-03 ����δ֪����
	Unknow=-1	
};



enum Save_Options
{
	KeyWord1,          //��������tCompanyLog  sKeyWord1
	sKeyWord1,         //�������ݿ�tCompanyLog  sKeyWord2
	sMemoChar,         //�������ݿ�tCompanyLog  sMemoChar
	File               //�����ļ�
};

//���ұ�����ṹ��
typedef struct MARK_FIND
{
	int iCodeType; 	                      //�������� URL GB1212 UTF_8 UNICODE ��Ϊ�е�����ǰ���ǲ�һ��
	int iPacketNum;                       //�������ڵ�ǰ��֮��ڼ�����֮�ڣ� 
	char szMarkStart[64];                 //��ʼ���
	char szMarkEnd[16];                   //�������
	Save_Options enumSaveOption;          //����е����ݱ�������
	PMARKFIND pstuMarkFindNext;
}MFRKFIND, * PMARKFIND;

//�� ���ṹ ����
typedef struct FIND_TAG_INFO
{
	bool bIsExtendFind;           //�Ƿ���Ҫ�������(���ڵڶ���������)  true��  false��
	
	int iFindType;                //��������   Ĭ��0 �ʺ� eg;��ҳ��1���� 2���� -1��������
	char szHost[128];             //http���������� ������Ϊ��httpЭ��ʱΪ�� ���Ϊ128λ
	char szPacketHeadTag[64];     //��ͷ���  ���Ϊ64λ             ͷ����������ȷ�������ݵ����� 
	PMARKFIND pstuMarkFind;       //���ұ������
	PFINDTAGINFO pstuFindTagInfoNext;

	bool CheckPacketHeadTag(BYTE* pInBuffer, int iBufLen)
	{
		if (strlen(szPacketHeadTag)>iBufLen) return false;
		//�������ﻹ��Ҫ���ü���ͷ����

	}	
}FINDTAGINFO, *PFINDTAGINFO;

//������ṹ
typedef struct SERVICES_FIND_INFO
{
	int iServicesType;            //��������,��enum ApplicationProtocolType���Ӧ  eg baidu
	PFINDTAGINFO pstuFindTagInfo; //��ͷ���ṹ ��
}SERVICESFINDINFO, *PSERVICESFINDINFO;

typedef struct filter_FIND_INFO
{
	int iFilterType;                        //ɸѡ���� eg http  Smtp
	PSERVICESFINDINFO pstuServicesFindInfo; //���¶�Ӧ���ַ��� eg baidu mop 163 126
};

//��Χ����
#define LOGIN_NUM         8       //��½����
#define MAX_HOST_NUM      15      //�����������
#define MAX_DOMAIN_LEN    128     //�����������
#define MAX_HEADTAG_LEN   32      //����ͷ��ǩ��
#define MAX_KEY_FRONT_LEN 16      //�����ַ���֮ǰ�ؼ�����󳤶�
#define MAX_KEY_BACK_LEN  8       //�����ַ���֮��ؼ�����󳤶�


//���ұ�־�ṹ--������½��BBS���ݲ���
typedef struct MARK_FIND_INFO
{
	int iHostType;                     //��½��վ����,��enum ApplicationProtocolType���Ӧ
	int iFindType;                     //��������   Ĭ��0 �ʺ� eg;��ҳ��1���� 2���� -1��������
//	int iCodeType;                     //�������͡��ɶ���Ϊ���λ��   unicode url gb2312 utf_8
	char* pHost;                       //����
	char* pHeadTag;                    //��ͷ��ǩ
	char* pMarkStart1;                 //��½���ͷ������⿪ʼ
	char* pMarkEnd1;                   //��½���ͷ����������
	char* pMarkStart2;                 //����������ݿ�ʼ--��iFindTypeΪ-1ʱΪ������������
	char* pMarkEnd2;                   //����������ݽ���
}MARKFIND, * PMARKFIND;
extern const MARKFIND g_szstuMarkFind[] = 
{//˵�����������⡪��2��������Ҳ�����⣬��ע���޸�
	///////////////////////////////////////////////����Ϊ��½�ʺ������ȡ�ؼ���////////////////////////////////////////////////////////////////////
	//iHostType    pHost                               pHeadTag                      ��½��ǰ    ��½����    ����ǰ      �����        ��½����	
	//BBSMaopu,   0, "Host: passport.mop.com\r\n",       "/Login?url=http",            "user_name=",  "&",    "password=", "&",            //è����ҳ è������
	//BBSMaopu,   0, "Host: passport.mop.com\r\n",       "/login HTTP/1.1",            "user_name=",  "&",    "password=", "",             //è��ͨ��֤
	//BBSMaopu,   0, "Host: dzh.mop.com\r\n",            "/login.do HTTP/1.1\r\n",     "name=",       "&",    "password=", "&",            //è�˴��ӻ�
	//BBSMaopu,   0, "Host: login.hi.mop.com\r\n",       "/Login.do HTTP/1.1\r\n",     "nickname=",   "&",    "password=", "&",            //è��HI
	//BBSTianya,  0, "Host: passport.tianya.cn\r\n",     "/login",                     "vwriter=",    "&",    "",          "",             //����
	//Jingdong,   0, "Host: www.360buy.com\r\n",         "/ HTTP/1.1\r\n",             "pin=",        ";",    "",          "",             //����
	//Jingdong,   0, "Host: passport.360buy.com\r\n",    "/new/helloService.ashx?",    "pin=",        ";",    "",          "",             //����
	BaiduTB,    0, "Host: www.baidu.com",        "GET /cache/user/html/jump.html?",    "username=",   "&",    "",          "",             //�ٶ��������¼
	BaiduTB,    0, "Host: tieba.baidu.com",      "GET /tb/static-common/html/pass/",   "userName=",   "&", "",        "",                  //�����������¼
	//BaiduTB,    0, "Host: tieba.baidu.com",     "GET /f/user/json_userinfo?", "TIEBA_LOGINED_USER=", ";",     "",          "",             //�����������¼
	//BaiduTB,    0, "Host: www.baidu.com\r\n",          "/cache/user/html/jump.html", "username=",   "&",      "",          "",             //�ٶ���ҳ��¼
	//YiHaoDian,  0, "Host: tracker.yihaodian.com\r\n",  "/tracker/info.do",           "ac=",         ";",    "",          "",             //1�ŵ�
	//Dangdang,   0, "Host: www.dangdang.com\r\n",       "/ HTTP/1.1\r\n",             "; nickname=", ";",    "",          "",             //����
	//BBS19Floor, 0, "Host: www.19lou.com\r\n",          "/login HTTP/1.1",            "userName=",   "&",    "userPass=", "&",            //19¥
	//BBSDaHe,    0, "Host: bbs.dahe.cn\r\n",            "/login",                     "&pwuser=",    "&",    "pwpwd=",    "&",            //���
	//BBSPeople,  0, "Host: passport.people.com.cn\r\n", "/_login.do",                 "userName=",   "&",    "password=", "&",            //������ǿ����̳
	//BBSPeople,  0, "Host: bbs.people.com.cn\r\n",      "GET /postAction.do",         "userName=",   "&",    "password=", "",
	//RenRen,     0, "Host: www.renren.com\r\n",         "/ajaxLogin/login",           "email=",      "&",    "",          "",             //������
	Mail126_COM,   0, "Host: config.mail.126.com",  "GET /optimizer/optimize/get.do?", "uid=",        "&",    "",          "",             //126
	Mail163_Com,  0,  "Host: twebmail.mail.163.com",  "POST /contacts/call.do?",       "uid=",        "&",    "",          "",
	
	//IM
	Fetion,       0,  "Host: 221.176.31.145",          "POST /GetRobotInfo.ashx?",    "register-email=\"", "\"", "",       "",              //�����ʺ�-��½�����˺�
	Fetion,       0,  "Host: 221.176.31.145",          "POST /GetRobotInfo.ashx?",    "sid=\"",            "\"", "",       "",              //�����ʺ�-��½�˺�
	Fetion,       0,  "Host: 221.176.31.145",          "POST /GetRobotInfo.ashx?",    "mobile-no=\"",      "\"", "",       "",              //�����ʺ�-��½�ֻ��˺�
	Fetion,       0,  "Host: 221.176.31.145",          "POST /GetRobotInfo.ashx?",    "nickname=\"",       "\"", "",       "",              //�����ʺ�-��½�û��ǳ�

	////΢��
	//TecentWeibo, 0, "Host: ptlogin2.qq.com\r\n", "/jump?",         "&clientuin=",    "&", "", "",    //��Ѷ �ӿͻ��˵�½
	//TecentWeibo, 0, "t.qq.com\r\n",              "/ HTTP/1.1\r\n", "ptui_loginuin=", ";", "", "",    //��Ѷ ��webҳ���½
	//SinaWeibo,   0, "Host: weibo.com\r\n",       "/ajaxlogin.php", "un=",            ";", "", "",    //sina΢��

	///////////////////////////////////////////////����Ϊ���������ȡ�ؼ���////////////////////////////////////////////////////////////////////
	//iHostType         pHost                         pHeadTag    �ؼ���ǰ  �ؼ��ֺ�  ������վ                 ����
	HttpKey,    -1, "Host: c.baidu.com\r\n",       "/c.gif?",     "&q=", "&", "www.baidu.com",     "",    //�ٶ�����
	//SearchKeyWrod, -1, "Host: www.google.com.hk\r\n", "/search?hl=", "&q=", "&", "www.google.com.hk", "",    //�ȸ�����

	Unknow,     0,  NULL,                         NULL,                                   NULL,         NULL, NULL,      NULL        //��½��BBS�ķֽ��־

	///////////////////////////////////////////////����ΪBBS���ݻ�ȡ�ؼ���////////////////////////////////////////////////////////////////////
	//iHostType   pHost                           pHeadTag                                 ����ǰ     �����  ����ǰ      ���ݺ�           BBS����
	//BaiduTB,    1, "Host: tieba.baidu.com\r\n",   "POST /f/commit/thread/add",            "title=",     "&", "content=",  "&",            //�ٶȷ�����
	//BaiduTB,    2, "Host: tieba.baidu.com\r\n",   "POST /f/commit/post/add",              "",           "",  "content=",  "&",            //�ٶȻ���
	//BBS19Floor, 1, "Host: www.19lou.com\r\n",     "POST /thread/category/normal/publish", "subject=",   "&", "content=",  "&",            //19¥������
	//BBS19Floor, 2, "Host: www.19lou.com\r\n",     "POST /post/reply",                     "",           "",  "content=",  "&",            //19¥����
	//BBSTianya,  1, "Host: blog.renren.com\r\n",   "POST /NewEntry.do",                    "title=",     "&", "content1=", "%3C%2Fp%3E&",  //����
	//BBSTianya,  2, "Host: status.renren.com\r\n", "POST /feedcommentreply.do",            "",           "",  "c=",        "&",            //���Ļظ�
	//BBSPeople,  2, "Host: bbs.people.com.cn\r\n", "GET /postAction.do",                   "titleText=", "&", "content1=", "&",            //ǿ������
	//BBSMaopu,   2, "Host: dzh.mop.com\r\n",       "POST /topic/addReply.do",              "",           "",  "&body=",    "&",            //è�˴��ӻ�
 //   //����Ϊ���Ƚϳ���
	//BBSMaopu,   1, "Host: tt.mop.com\r\n", "POST /subject/post.do", "name=\"subTitle\"\x0d\x0a\x0d\x0a", "\x0d\x0a",  
	//"name=\"subBody\"\x0d\x0a\x0d\x0a","\x0d\x0a\x2d\x2d\x2d\x2d\x2d\x2d",                                                                //è����ҳ è������
	//BBSDaHe,    2, "Host: bbs.dahe.cn\r\n",       "POST /post.php", "name=\"atc_title\"\x0d\x0a\x0d\x0a", 
	//"\x0d\x0a\x2d\x2d\x2d\x2d\x2d\x2d", "name=\"atc_content\"\x0d\x0a\x0d\x0a", "\x0d\x0a\x2d\x2d\x2d\x2d\x2d\x2d"                        //����� ������
};

#endif /*__COMM_DEFINE_H__*/