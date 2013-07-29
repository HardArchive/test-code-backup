//file TraceUtil.h
/***************************************************************************************************
* 1、 Class      ： comm_define
* 2、 Version    ： *.*
* 3、 Description： 网络解析的公共头文件
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-10-21 14:14:03
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/

#ifndef __COMM_DEFINE_H__
#define __COMM_DEFINE_H__



enum AppProtoclType
{
	ApplicationUnknown=0, Http=1, Ftp=2, Smtp=3, Pop=4, WebMail = 6,	Telnet=7,Mail=9,
	Msn=1001,Oicq=1002,Icq=1003 ,YahooMsg=1004,Uc=1006,Fetion=1020,
	// 淘宝旺旺
	WangWang=1021,
	// 联众游戏
	OurGame = 2001, 
	// 中国在线游戏 
	ChinaGame=2002,
	// 边锋
	Game=2004,
	// 魔兽世界
	MoshouShijie=2012,
	// 热血江湖
	ReXueJiangHu=2031,
	// 传奇世界
	ChuanqiShijie=2065,
	// A3
	A3Game = 2123,
	// 仙境传说(RO)
	FaeryStoryGame=2153,
	// 完美
	WanMei= 2182,
	// 新浪UT
	SinaUt=2206,
	// Yahoo游戏 李照修改 2010-06-25 远航游戏[公安类型库定义]
	YhGame=2211,
	FtpFile = 3001, HttpKey=3002, //modify by rg & 2013-5-2 13:30:17 将原本13改为3002
	// 浩方
	HaoFangGame=3003,WebAccounts=3004, WebPhoneNumber=3005,
	// QQ游戏
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

	// 好聊聊天室 登录		李照修改 2010-06-21 添加好聊天室类型
	HaoLiaoLiaoTian=7878,
	//// 远航游戏的截取		李照修改 2010-06-23 添加远航游戏类型
	//// 李照修改 2010-06-25 类型与公安冲突，3025 改为 3028
	//YuanHang = 3028	

	// 李照修改 2010-07-14 增加问道游戏帐号截取
	WenDao=9010,

	// 新浪邮箱和163邮箱
	Mail126_Com = 6008,
	MailSohu_Com = 6010,
	MailTom_Com = 6017,
	Mail163_Com = 6009,
	MailSina_Com = 6003,
	MailQQ_Com = 6018,
	Mail139_Com = 6032,

	Radius = 20000,
	// 李照修改 2010-08-03 增加未知类型
	Unknow=-1	
};



enum Save_Options
{
	KeyWord1,          //保存类型tCompanyLog  sKeyWord1
	sKeyWord1,         //保存数据库tCompanyLog  sKeyWord2
	sMemoChar,         //保存数据库tCompanyLog  sMemoChar
	File               //保存文件
};

//查找标记链结构表
typedef struct MARK_FIND
{
	int iCodeType; 	                      //编码类型 URL GB1212 UTF_8 UNICODE 因为有的数据前后标记不一样
	int iPacketNum;                       //该内容在当前包之后第几个包之内？ 
	char szMarkStart[64];                 //开始标记
	char szMarkEnd[16];                   //结束标记
	Save_Options enumSaveOption;          //标记中的数据保存类型
	PMARKFIND pstuMarkFindNext;
}MFRKFIND, * PMARKFIND;

//包 检查结构 链表
typedef struct FIND_TAG_INFO
{
	bool bIsExtendFind;           //是否需要延伸查找(即在第二个包查找)  true是  false否
	
	int iFindType;                //查找类型   默认0 帐号 eg;首页，1回帖 2发贴 -1搜索引擎
	char szHost[128];             //http类型用域名 当类型为非http协议时为空 最长度为128位
	char szPacketHeadTag[64];     //包头标记  最长度为64位             头和域名用来确定包数据的类型 
	PMARKFIND pstuMarkFind;       //查找标记链表
	PFINDTAGINFO pstuFindTagInfoNext;

	bool CheckPacketHeadTag(BYTE* pInBuffer, int iBufLen)
	{
		if (strlen(szPacketHeadTag)>iBufLen) return false;
		//……这里还需要调用检查包头函数

	}	
}FINDTAGINFO, *PFINDTAGINFO;

//服务检查结构
typedef struct SERVICES_FIND_INFO
{
	int iServicesType;            //服务类型,与enum ApplicationProtocolType相对应  eg baidu
	PFINDTAGINFO pstuFindTagInfo; //包头检查结构 链
}SERVICESFINDINFO, *PSERVICESFINDINFO;

typedef struct filter_FIND_INFO
{
	int iFilterType;                        //筛选类型 eg http  Smtp
	PSERVICESFINDINFO pstuServicesFindInfo; //其下对应多种服务 eg baidu mop 163 126
};

//范围常量
#define LOGIN_NUM         8       //登陆数量
#define MAX_HOST_NUM      15      //最大域名数量
#define MAX_DOMAIN_LEN    128     //最大域名长度
#define MAX_HEADTAG_LEN   32      //最大包头标签长
#define MAX_KEY_FRONT_LEN 16      //查找字符串之前关键字最大长度
#define MAX_KEY_BACK_LEN  8       //查找字符串之后关键字最大长度


//查找标志结构--包括登陆、BBS内容查找
typedef struct MARK_FIND_INFO
{
	int iHostType;                     //登陆网站类型,与enum ApplicationProtocolType相对应
	int iFindType;                     //查找类型   默认0 帐号 eg;首页，1回帖 2发贴 -1搜索引擎
//	int iCodeType;                     //编码类型。可定义为多项按位或   unicode url gb2312 utf_8
	char* pHost;                       //域名
	char* pHeadTag;                    //包头标签
	char* pMarkStart1;                 //登陆名和发贴标题开始
	char* pMarkEnd1;                   //登陆名和发贴标题结束
	char* pMarkStart2;                 //密码或发贴内容开始--当iFindType为-1时为搜索引擎域名
	char* pMarkEnd2;                   //密码或发贴内容结束
}MARKFIND, * PMARKFIND;
extern const MARKFIND g_szstuMarkFind[] = 
{//说明京东有问题——2当当可能也有问题，需注意修改
	///////////////////////////////////////////////以下为登陆帐号密码获取关键字////////////////////////////////////////////////////////////////////
	//iHostType    pHost                               pHeadTag                      登陆名前    登陆名后    密码前      密码后        登陆查找	
	//BBSMaopu,   0, "Host: passport.mop.com\r\n",       "/Login?url=http",            "user_name=",  "&",    "password=", "&",            //猫扑首页 猫扑贴贴
	//BBSMaopu,   0, "Host: passport.mop.com\r\n",       "/login HTTP/1.1",            "user_name=",  "&",    "password=", "",             //猫扑通行证
	//BBSMaopu,   0, "Host: dzh.mop.com\r\n",            "/login.do HTTP/1.1\r\n",     "name=",       "&",    "password=", "&",            //猫扑大杂烩
	//BBSMaopu,   0, "Host: login.hi.mop.com\r\n",       "/Login.do HTTP/1.1\r\n",     "nickname=",   "&",    "password=", "&",            //猫扑HI
	//BBSTianya,  0, "Host: passport.tianya.cn\r\n",     "/login",                     "vwriter=",    "&",    "",          "",             //天涯
	//Jingdong,   0, "Host: www.360buy.com\r\n",         "/ HTTP/1.1\r\n",             "pin=",        ";",    "",          "",             //京东
	//Jingdong,   0, "Host: passport.360buy.com\r\n",    "/new/helloService.ashx?",    "pin=",        ";",    "",          "",             //京东
	BaiduTB,    0, "Host: www.baidu.com",        "GET /cache/user/html/jump.html?",    "username=",   "&",    "",          "",             //百度主界面登录
	BaiduTB,    0, "Host: tieba.baidu.com",      "GET /tb/static-common/html/pass/",   "userName=",   "&", "",        "",                  //贴吧主界面登录
	//BaiduTB,    0, "Host: tieba.baidu.com",     "GET /f/user/json_userinfo?", "TIEBA_LOGINED_USER=", ";",     "",          "",             //贴吧主界面登录
	//BaiduTB,    0, "Host: www.baidu.com\r\n",          "/cache/user/html/jump.html", "username=",   "&",      "",          "",             //百度首页登录
	//YiHaoDian,  0, "Host: tracker.yihaodian.com\r\n",  "/tracker/info.do",           "ac=",         ";",    "",          "",             //1号店
	//Dangdang,   0, "Host: www.dangdang.com\r\n",       "/ HTTP/1.1\r\n",             "; nickname=", ";",    "",          "",             //当当
	//BBS19Floor, 0, "Host: www.19lou.com\r\n",          "/login HTTP/1.1",            "userName=",   "&",    "userPass=", "&",            //19楼
	//BBSDaHe,    0, "Host: bbs.dahe.cn\r\n",            "/login",                     "&pwuser=",    "&",    "pwpwd=",    "&",            //大河
	//BBSPeople,  0, "Host: passport.people.com.cn\r\n", "/_login.do",                 "userName=",   "&",    "password=", "&",            //人民网强国论坛
	//BBSPeople,  0, "Host: bbs.people.com.cn\r\n",      "GET /postAction.do",         "userName=",   "&",    "password=", "",
	//RenRen,     0, "Host: www.renren.com\r\n",         "/ajaxLogin/login",           "email=",      "&",    "",          "",             //人人网
	Mail126_COM,   0, "Host: config.mail.126.com",  "GET /optimizer/optimize/get.do?", "uid=",        "&",    "",          "",             //126
	Mail163_Com,  0,  "Host: twebmail.mail.163.com",  "POST /contacts/call.do?",       "uid=",        "&",    "",          "",
	
	//IM
	Fetion,       0,  "Host: 221.176.31.145",          "POST /GetRobotInfo.ashx?",    "register-email=\"", "\"", "",       "",              //飞信帐号-登陆邮箱账号
	Fetion,       0,  "Host: 221.176.31.145",          "POST /GetRobotInfo.ashx?",    "sid=\"",            "\"", "",       "",              //飞信帐号-登陆账号
	Fetion,       0,  "Host: 221.176.31.145",          "POST /GetRobotInfo.ashx?",    "mobile-no=\"",      "\"", "",       "",              //飞信帐号-登陆手机账号
	Fetion,       0,  "Host: 221.176.31.145",          "POST /GetRobotInfo.ashx?",    "nickname=\"",       "\"", "",       "",              //飞信帐号-登陆用户昵称

	////微博
	//TecentWeibo, 0, "Host: ptlogin2.qq.com\r\n", "/jump?",         "&clientuin=",    "&", "", "",    //腾讯 从客户端登陆
	//TecentWeibo, 0, "t.qq.com\r\n",              "/ HTTP/1.1\r\n", "ptui_loginuin=", ";", "", "",    //腾讯 从web页面登陆
	//SinaWeibo,   0, "Host: weibo.com\r\n",       "/ajaxlogin.php", "un=",            ";", "", "",    //sina微博

	///////////////////////////////////////////////以下为搜索引擎获取关键字////////////////////////////////////////////////////////////////////
	//iHostType         pHost                         pHeadTag    关键字前  关键字后  搜索网站                 搜索
	HttpKey,    -1, "Host: c.baidu.com\r\n",       "/c.gif?",     "&q=", "&", "www.baidu.com",     "",    //百度搜索
	//SearchKeyWrod, -1, "Host: www.google.com.hk\r\n", "/search?hl=", "&q=", "&", "www.google.com.hk", "",    //谷歌搜索

	Unknow,     0,  NULL,                         NULL,                                   NULL,         NULL, NULL,      NULL        //登陆和BBS的分界标志

	///////////////////////////////////////////////以下为BBS内容获取关键字////////////////////////////////////////////////////////////////////
	//iHostType   pHost                           pHeadTag                                 标题前     标题后  内容前      内容后           BBS内容
	//BaiduTB,    1, "Host: tieba.baidu.com\r\n",   "POST /f/commit/thread/add",            "title=",     "&", "content=",  "&",            //百度发新贴
	//BaiduTB,    2, "Host: tieba.baidu.com\r\n",   "POST /f/commit/post/add",              "",           "",  "content=",  "&",            //百度回贴
	//BBS19Floor, 1, "Host: www.19lou.com\r\n",     "POST /thread/category/normal/publish", "subject=",   "&", "content=",  "&",            //19楼发新贴
	//BBS19Floor, 2, "Host: www.19lou.com\r\n",     "POST /post/reply",                     "",           "",  "content=",  "&",            //19楼回贴
	//BBSTianya,  1, "Host: blog.renren.com\r\n",   "POST /NewEntry.do",                    "title=",     "&", "content1=", "%3C%2Fp%3E&",  //天涯
	//BBSTianya,  2, "Host: status.renren.com\r\n", "POST /feedcommentreply.do",            "",           "",  "c=",        "&",            //天涯回复
	//BBSPeople,  2, "Host: bbs.people.com.cn\r\n", "GET /postAction.do",                   "titleText=", "&", "content1=", "&",            //强国社区
	//BBSMaopu,   2, "Host: dzh.mop.com\r\n",       "POST /topic/addReply.do",              "",           "",  "&body=",    "&",            //猫扑大杂烩
 //   //以下为长度较长的
	//BBSMaopu,   1, "Host: tt.mop.com\r\n", "POST /subject/post.do", "name=\"subTitle\"\x0d\x0a\x0d\x0a", "\x0d\x0a",  
	//"name=\"subBody\"\x0d\x0a\x0d\x0a","\x0d\x0a\x2d\x2d\x2d\x2d\x2d\x2d",                                                                //猫扑首页 猫扑贴贴
	//BBSDaHe,    2, "Host: bbs.dahe.cn\r\n",       "POST /post.php", "name=\"atc_title\"\x0d\x0a\x0d\x0a", 
	//"\x0d\x0a\x2d\x2d\x2d\x2d\x2d\x2d", "name=\"atc_content\"\x0d\x0a\x0d\x0a", "\x0d\x0a\x2d\x2d\x2d\x2d\x2d\x2d"                        //大河网 发回贴
};

#endif /*__COMM_DEFINE_H__*/