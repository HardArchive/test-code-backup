// exp3.cpp : �������̨Ӧ�ó������ڵ㡣
//

//		================notice:================================
//		Project Name: exp2
//		exp2 ���ڲ��Կ�������api:
//��ȡ�û���Ϣ
//��ȡ�ļ�(��)��Ϣ
//��ȡ�ļ���������
//�����ļ���
//ɾ���ļ�(��)
//�ƶ��ļ�(��)
//�����ļ�(��)
//�ϴ��ļ�
//�����ļ�

//		�������б�����ǰ����ȷ���Ѿ���exp1�л�ȡ����ʽ��oauth_token��oauth_token_secret���������������޸ĺ�
//		ʹ��releaseģʽ
//		Author��Micahael Lee;		Email: michael.20@qq.com
//		================notice end ===============================

#include "stdafx.h"
#include <string> 
#include <Windows.h>
#include "kpSDK.h"
#pragma comment(lib,"kpSDK.lib")


//=============�����������������Ҫ��д���consumer_key��consumer_secret �Һ�exp1�еı���һ�£�������=========
string consumerKey="xcUL3I5GwqjJodlp";
string consumerSecret="A4t4FYDC3cn4HvJ7";
//=============�����������������Ҫ��д���consumer_key��consumer_secret �Һ�exp1�еı���һ�£�������=========


int _tmain(int argc, _TCHAR* argv[])
{
	string oauthToken="0039635570062017b83c4d18"; //!!!!��дexp1�л�õ���ʽoauth_token
	string oauthSecret="3263dfcb6c534474a890b89236b8dda1"; //!!!!��дexp1�л�õ���ʽoauth_token_secret

	string	jsonStr;				//���ص�json
	int		ret=0;					//http״̬��
	bool	isAppPath=true;			//Ӧ��Ȩ�ޣ���������Ϊtrue,  Ӧ���ļ���Ϊfalse
	bool	isToRecyle=false;		//ɾ��ʱ���Ƿ�Ž����̻���վ��
	
	int		type;
	bool	stop=false; 
	while(!stop)
	{
		Sleep(500);
		printf("\n\n������Ҫִ�е�������:\n 0.�˳�\n 1.��ȡ�û���Ϣ\n 2.��ȡ�ļ�(��)��Ϣ\n 3.��ȡ�ļ���������\n 4.�����ļ���\n 5.ɾ���ļ�(��)\n 6.�ƶ��ļ�(��)\n 7.�����ļ�(��)\n 8.�ϴ��ļ�\n 9.�����ļ�\n");
		cin>>type;
		switch(type)
		{
		case 0:
			stop=true;
			break;
		case 1:
			{
				//kpUsrInfo�����Ѿ��Է��ص��û���Ϣjson���н��� ֱ�Ӵ�����ǰ���ĸ�����
				//��ȡ�û���Ϣ �������ĸ���������kpUsrInfo�����ɻ���û���Ϣ
				string max_file_size;
				string user_name;
				string quota_used;
				string quota_total;
				printf("---------���л�ȡ�û���Ϣ\n"); 
				ret=kpUsrInfo(max_file_size,user_name,quota_used,quota_total,consumerKey,consumerSecret,oauthToken,oauthSecret);
				printf(" ret:%d\n max_file_size:%s\n user_name:%s\n",ret, max_file_size.c_str(),user_name.c_str());
				printf(" quota_used:%s\n quota_total:%d\n",quota_used.c_str(),quota_total.c_str());
				break;
			}
		case 2:
			{
				//��ȡ�ļ�(��)��Ϣ	ֱ�ӽ��ļ����У���Ϣ ��json��ʽ���ص�jsonStr,������
				printf("������Ҫ��ȡ�ļ�(��)��Ϣ��·��:\n");
				string metaDataPath;
				cin>>metaDataPath;
				ret=kpMetaData(jsonStr,consumerKey,consumerSecret,oauthToken,oauthSecret,isAppPath,metaDataPath);
				printf(" ret:%d\n json:\n%s\n",ret,jsonStr.c_str());
				break;
			}
		case 3:
			{
				//�����ļ� ���ֱ�ӷ���json��jsonStr
				printf("�����������Ҫ������ļ�·��:\n");
				string fromPath;
				cin>>fromPath;
				ret=kpShareFile(jsonStr,consumerKey,consumerSecret,oauthToken,oauthSecret,isAppPath,fromPath);
				printf(" ret:%d\n jsonStr:\n%s\n",ret,jsonStr.c_str());
				break;
			}
		case 4:
			{
				//�½��ļ���	�����json��ʽ����jsonStr
				printf("������Ҫ�½����ļ���·����\n");
				string createFolderPath;
				cin>>createFolderPath;
				ret=kpCreateFolder(jsonStr,consumerKey,consumerSecret,oauthToken,oauthSecret,isAppPath,createFolderPath);
				printf(" ret:%d\n json:\n%s\n",ret,jsonStr.c_str());
				break;
			}
		case 5:
			{
				//ɾ���ļ�	ɾ�������json��ʽ����jsonStr
				printf("������Ҫɾ�����ļ�·����\n");
				string delFilePath;			
				cin>>delFilePath;
				ret=kpDelFile(jsonStr,consumerKey,consumerSecret,oauthToken,oauthSecret,isAppPath,delFilePath,isToRecyle);
				printf(" ret:%d\n json:\n%s\n",ret,jsonStr.c_str());
				break;
			}
		case 6:
			{
				//�����ļ� �����json��ʽ����jsonStr
				string fromPath;
				string toPath;
				printf("�������ļ�ԭ·��:\n");
				cin>>fromPath;
				printf("�������ļ�Ҫ�ƶ�����·��:\n");
				cin>>toPath;
				ret=kpMvFile(jsonStr,consumerKey,consumerSecret,oauthToken,oauthSecret,isAppPath,fromPath,toPath);
				printf(" ret:%d\n json:\n%s\n",ret,jsonStr.c_str());
				break;
			}
		case 7:
			{
				//�����ļ�	���ƽ����json��ʽ����jsonStr
				string fromPath;
				string toPath;
				printf("�������ļ�ԭ·��:\n");
				cin>>fromPath;
				printf("�������ļ�Ҫ���Ƶ���·��:\n");
				cin>>toPath;
				ret=kpCpFile(jsonStr,consumerKey,consumerSecret,oauthToken,oauthSecret,isAppPath,fromPath,toPath);
				printf(" ret:%d\n json:\n%s\n",ret,jsonStr.c_str());
				break;
			}
		case 8:
			{
				
				//��ȡ�ϴ��ڵ� ֱ�ӽ�����uploadNode�У�������json
				printf("��ȡ�ϴ��ڵ�...\n");//�ϴ��ڵ�ɱ������� ����ÿ�λ�ȡ
				string uploadNode;//�ϴ��ڵ� ���ϴ��ļ�ʱ����Ϊ�����õ�
				ret=kpUploadLocate(uploadNode,consumerKey,consumerSecret,oauthToken,oauthSecret);
				printf(" ret:%d\n uploadNode:\n%s\n",ret,uploadNode.c_str());


				//�ϴ��ļ�  �����json��ʽ���ص�jsonStr 
				string fromPath;
				string toPath;
				printf("�����뱾���ļ�·��:\n");
				cin>>fromPath;
				printf("�������ϴ������̵�·��:\n");
				cin>>toPath;
				bool isOverWrite=true; //����
				printf("\n\n\n---------�ϴ��ļ���...\n"); 
				ret=kpUploadFile(jsonStr,consumerKey,consumerSecret,oauthToken,oauthSecret,isOverWrite,isAppPath,uploadNode,fromPath,toPath);
				printf(" �ϴ����ret:%d\n json:\n%s\n",ret,jsonStr.c_str());
				break;
			}
		case 9:
			{
				//�����ļ�  ֱ�������ļ� �޽������ �ɿ�http״̬��ret
				string fromPath;
				string toPath;
				printf("������Ҫ���ص��ļ�·��:\n");
				cin>>fromPath;
				printf("������Ҫ���浽��·��:\n");
				cin>>toPath;
				printf("\n\n\n---------�����ļ���...\n");
				ret=kpDownloadFile(consumerKey,consumerSecret,oauthToken,oauthSecret,isAppPath,fromPath,toPath);
				printf("ret:%d �������\n ",ret);
				break;
			}
		default:
			break;
		}
	}
	return 0;
}