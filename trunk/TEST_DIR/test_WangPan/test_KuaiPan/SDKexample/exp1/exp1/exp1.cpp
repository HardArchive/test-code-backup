// exp1.cpp : �������̨Ӧ�ó������ڵ㡣
//

//		================notice:================================
//		Project Name: exp1

//		exp1 �����Ȼ�ȡ��ʱoauth_token����ʱoauth_token_secret�ͼ�Ȩurl��Ȼ���ٻ�ȡ��ʽ��oauth_token��Ϣ

//		����дȫ�ֱ���consumerKey��consumerSecret��ֵ

//		�뱣�����ʽ��oauth_token��oauth_token_secret����Ϊ����exp2����api�����õ�

//		��ȡ����Ȩurl�������з���url���м�Ȩ��sdk������Ȩ����

//		ʹ��releaseģʽ
     
//		Author��Micahael Lee;		Email: michael.20@qq.com
//		================notice end ===============================


#include "stdafx.h"
#include "kpSDK.h"
#pragma comment(lib,"kpSDK.lib")

//=============�����������������Ҫ��д���consumer_key��consumer_secret��������=========
//string consumerKey="yourConsumerKey";
//string consumerSecret="yourConsumerSecret";
string consumerKey="xcUL3I5GwqjJodlp";
string consumerSecret="A4t4FYDC3cn4HvJ7";
//=============�����������������Ҫ��д���consumer_key��consumer_secret��������=========


int _tmain(int argc, _TCHAR* argv[])
{
	int ret=0;			//retΪHTTP״̬��
	string tmpToken;	//��ʱoauth_token
	string tmpSecret;	//��ʱoauth_token_secret
	string url;			//��Ȩurl

	string oauthToken;  //��ʽoauth_token
	string oauthSecret; //��ʽoauth_token_secret
	string usrID;		//user_id
	string chargeDir;	//charged_dir

	printf(" -------��ȡ��ʱoauth_token����ʱoauth_token_secret�ͼ�Ȩurl \n");
	ret=kpReqTmpToken(tmpToken,tmpSecret,url,consumerKey,consumerSecret);

	printf(" ret:%d\n token:%s\n secret:%s\n",ret,tmpToken.c_str(),tmpSecret.c_str());
	printf(" \n�븴�������url�������������Ȩ:\n%s\n",url.c_str());
	printf(" \n\n\n����������Ȩ��Ϻ󣬰��»س�������....\n");
	getchar();

	//��ȡ��ʽtoken  ���뱣֤�Ѿ�ͨ��url��ҳ��Ȩ!
	printf("--------��ȡaccessToken	���뱣֤�Ѿ�ͨ��url��ҳ��Ȩ �������NULL\n");
	ret=kpAccessToken(oauthToken,oauthSecret,usrID,chargeDir,consumerKey,consumerSecret,tmpToken,tmpSecret);

	printf(" ret:%d\n oauth_token:%s\n oauth_token_secret:%s\n",ret, oauthToken.c_str(),oauthSecret.c_str());
	printf(" usrid:%s\n chargeDir:%d\n",usrID.c_str(),chargeDir.c_str());
	printf(" \n��ȡaccessToken��Ϣ��� ���ֶ�����oauth_token��oauth_token_secret��Ϣ��ȥ��������exp2����\n");

	getchar();
	return 0;
}
