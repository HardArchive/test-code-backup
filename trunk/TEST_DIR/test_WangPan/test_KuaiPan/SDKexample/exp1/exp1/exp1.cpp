// exp1.cpp : 定义控制台应用程序的入口点。
//

//		================notice:================================
//		Project Name: exp1

//		exp1 用于先获取临时oauth_token、临时oauth_token_secret和鉴权url；然后再获取正式的oauth_token信息

//		先填写全局变量consumerKey和consumerSecret的值

//		请保存好正式的oauth_token、oauth_token_secret，因为后面exp2所有api都会用到

//		获取到鉴权url后，请自行访问url进行鉴权，sdk不作鉴权处理

//		使用release模式
     
//		Author：Micahael Lee;		Email: michael.20@qq.com
//		================notice end ===============================


#include "stdafx.h"
#include "kpSDK.h"
#pragma comment(lib,"kpSDK.lib")

//=============！！！下面这里务必要填写你的consumer_key和consumer_secret！！！！=========
//string consumerKey="yourConsumerKey";
//string consumerSecret="yourConsumerSecret";
string consumerKey="xcUL3I5GwqjJodlp";
string consumerSecret="A4t4FYDC3cn4HvJ7";
//=============！！！上面这里务必要填写你的consumer_key和consumer_secret！！！！=========


int _tmain(int argc, _TCHAR* argv[])
{
	int ret=0;			//ret为HTTP状态码
	string tmpToken;	//临时oauth_token
	string tmpSecret;	//临时oauth_token_secret
	string url;			//鉴权url

	string oauthToken;  //正式oauth_token
	string oauthSecret; //正式oauth_token_secret
	string usrID;		//user_id
	string chargeDir;	//charged_dir

	printf(" -------获取临时oauth_token、临时oauth_token_secret和鉴权url \n");
	ret=kpReqTmpToken(tmpToken,tmpSecret,url,consumerKey,consumerSecret);

	printf(" ret:%d\n token:%s\n secret:%s\n",ret,tmpToken.c_str(),tmpSecret.c_str());
	printf(" \n请复制下面的url到浏览器进行授权:\n%s\n",url.c_str());
	printf(" \n\n\n当上述的授权完毕后，按下回车键继续....\n");
	getchar();

	//获取正式token  必须保证已经通过url网页授权!
	printf("--------获取accessToken	必须保证已经通过url网页授权 否则输出NULL\n");
	ret=kpAccessToken(oauthToken,oauthSecret,usrID,chargeDir,consumerKey,consumerSecret,tmpToken,tmpSecret);

	printf(" ret:%d\n oauth_token:%s\n oauth_token_secret:%s\n",ret, oauthToken.c_str(),oauthSecret.c_str());
	printf(" usrid:%s\n chargeDir:%d\n",usrID.c_str(),chargeDir.c_str());
	printf(" \n获取accessToken信息完毕 请手动记下oauth_token和oauth_token_secret信息，去编译运行exp2程序\n");

	getchar();
	return 0;
}
