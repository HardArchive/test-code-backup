// exp3.cpp : 定义控制台应用程序的入口点。
//

//		================notice:================================
//		Project Name: exp2
//		exp2 用于测试快盘以下api:
//获取用户信息
//获取文件(夹)信息
//获取文件分享链接
//创建文件夹
//删除文件(夹)
//移动文件(夹)
//复制文件(夹)
//上传文件
//下载文件

//		编译运行本程序前，请确定已经在exp1中获取到正式的oauth_token和oauth_token_secret，并在主函数中修改好
//		使用release模式
//		Author：Micahael Lee;		Email: michael.20@qq.com
//		================notice end ===============================

#include "stdafx.h"
#include <string> 
#include <Windows.h>
#include "kpSDK.h"
#pragma comment(lib,"kpSDK.lib")


//=============！！！下面这里务必要填写你的consumer_key和consumer_secret 且和exp1中的保持一致！！！！=========
string consumerKey="xcUL3I5GwqjJodlp";
string consumerSecret="A4t4FYDC3cn4HvJ7";
//=============！！！上面这里务必要填写你的consumer_key和consumer_secret 且和exp1中的保持一致！！！！=========


int _tmain(int argc, _TCHAR* argv[])
{
	string oauthToken="0039635570062017b83c4d18"; //!!!!填写exp1中获得的正式oauth_token
	string oauthSecret="3263dfcb6c534474a890b89236b8dda1"; //!!!!填写exp1中获得的正式oauth_token_secret

	string	jsonStr;				//返回的json
	int		ret=0;					//http状态码
	bool	isAppPath=true;			//应用权限：整个快盘为true,  应用文件夹为false
	bool	isToRecyle=false;		//删除时候是否放进快盘回收站？
	
	int		type;
	bool	stop=false; 
	while(!stop)
	{
		Sleep(500);
		printf("\n\n请输入要执行的任务编号:\n 0.退出\n 1.获取用户信息\n 2.获取文件(夹)信息\n 3.获取文件分享链接\n 4.创建文件夹\n 5.删除文件(夹)\n 6.移动文件(夹)\n 7.复制文件(夹)\n 8.上传文件\n 9.下载文件\n");
		cin>>type;
		switch(type)
		{
		case 0:
			stop=true;
			break;
		case 1:
			{
				//kpUsrInfo函数已经对返回的用户信息json进行解析 直接传出到前面四个参数
				//获取用户信息 将以下四个参数传入kpUsrInfo函数可获得用户信息
				string max_file_size;
				string user_name;
				string quota_used;
				string quota_total;
				printf("---------进行获取用户信息\n"); 
				ret=kpUsrInfo(max_file_size,user_name,quota_used,quota_total,consumerKey,consumerSecret,oauthToken,oauthSecret);
				printf(" ret:%d\n max_file_size:%s\n user_name:%s\n",ret, max_file_size.c_str(),user_name.c_str());
				printf(" quota_used:%s\n quota_total:%d\n",quota_used.c_str(),quota_total.c_str());
				break;
			}
		case 2:
			{
				//获取文件(夹)信息	直接将文件（夹）信息 以json形式返回到jsonStr,不解析
				printf("请输入要获取文件(夹)信息的路径:\n");
				string metaDataPath;
				cin>>metaDataPath;
				ret=kpMetaData(jsonStr,consumerKey,consumerSecret,oauthToken,oauthSecret,isAppPath,metaDataPath);
				printf(" ret:%d\n json:\n%s\n",ret,jsonStr.c_str());
				break;
			}
		case 3:
			{
				//分享文件 结果直接返回json到jsonStr
				printf("请输入快盘上要分享的文件路径:\n");
				string fromPath;
				cin>>fromPath;
				ret=kpShareFile(jsonStr,consumerKey,consumerSecret,oauthToken,oauthSecret,isAppPath,fromPath);
				printf(" ret:%d\n jsonStr:\n%s\n",ret,jsonStr.c_str());
				break;
			}
		case 4:
			{
				//新建文件夹	结果以json形式返回jsonStr
				printf("请输入要新建的文件夹路径：\n");
				string createFolderPath;
				cin>>createFolderPath;
				ret=kpCreateFolder(jsonStr,consumerKey,consumerSecret,oauthToken,oauthSecret,isAppPath,createFolderPath);
				printf(" ret:%d\n json:\n%s\n",ret,jsonStr.c_str());
				break;
			}
		case 5:
			{
				//删除文件	删除结果以json形式返回jsonStr
				printf("请输入要删除的文件路径：\n");
				string delFilePath;			
				cin>>delFilePath;
				ret=kpDelFile(jsonStr,consumerKey,consumerSecret,oauthToken,oauthSecret,isAppPath,delFilePath,isToRecyle);
				printf(" ret:%d\n json:\n%s\n",ret,jsonStr.c_str());
				break;
			}
		case 6:
			{
				//剪切文件 结果以json形式返回jsonStr
				string fromPath;
				string toPath;
				printf("请输入文件原路径:\n");
				cin>>fromPath;
				printf("请输入文件要移动到的路径:\n");
				cin>>toPath;
				ret=kpMvFile(jsonStr,consumerKey,consumerSecret,oauthToken,oauthSecret,isAppPath,fromPath,toPath);
				printf(" ret:%d\n json:\n%s\n",ret,jsonStr.c_str());
				break;
			}
		case 7:
			{
				//复制文件	复制结果以json形式返回jsonStr
				string fromPath;
				string toPath;
				printf("请输入文件原路径:\n");
				cin>>fromPath;
				printf("请输入文件要复制到的路径:\n");
				cin>>toPath;
				ret=kpCpFile(jsonStr,consumerKey,consumerSecret,oauthToken,oauthSecret,isAppPath,fromPath,toPath);
				printf(" ret:%d\n json:\n%s\n",ret,jsonStr.c_str());
				break;
			}
		case 8:
			{
				
				//获取上传节点 直接解析到uploadNode中，不返回json
				printf("获取上传节点...\n");//上传节点可保存下来 不用每次获取
				string uploadNode;//上传节点 在上传文件时会作为参数用到
				ret=kpUploadLocate(uploadNode,consumerKey,consumerSecret,oauthToken,oauthSecret);
				printf(" ret:%d\n uploadNode:\n%s\n",ret,uploadNode.c_str());


				//上传文件  结果以json形式返回到jsonStr 
				string fromPath;
				string toPath;
				printf("请输入本地文件路径:\n");
				cin>>fromPath;
				printf("请输入上传到快盘的路径:\n");
				cin>>toPath;
				bool isOverWrite=true; //覆盖
				printf("\n\n\n---------上传文件中...\n"); 
				ret=kpUploadFile(jsonStr,consumerKey,consumerSecret,oauthToken,oauthSecret,isOverWrite,isAppPath,uploadNode,fromPath,toPath);
				printf(" 上传完成ret:%d\n json:\n%s\n",ret,jsonStr.c_str());
				break;
			}
		case 9:
			{
				//下载文件  直接下载文件 无结果返回 可看http状态码ret
				string fromPath;
				string toPath;
				printf("请输入要下载的文件路径:\n");
				cin>>fromPath;
				printf("请输入要保存到的路径:\n");
				cin>>toPath;
				printf("\n\n\n---------下载文件中...\n");
				ret=kpDownloadFile(consumerKey,consumerSecret,oauthToken,oauthSecret,isAppPath,fromPath,toPath);
				printf("ret:%d 下载完成\n ",ret);
				break;
			}
		default:
			break;
		}
	}
	return 0;
}