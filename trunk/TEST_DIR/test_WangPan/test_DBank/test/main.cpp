#include "stdafx.h"
#include <iostream>
//�㶮��...
//#include "NSPClient.h"
#include <NSPClient.h>

//�����û���Ϣ�ӿ�(����ֵΪmap����)
#include<map>

using namespace std;

//system level use appid
//...

//customer level use session
//���þ������Vfs���ṩ���̶�������ӿڣ�User�ṩ�û���Ϣ��ȡ�ӿڣ� 
void lsdirtest(nspClient *nC)
{
    //����Vfs��������
    Vfs *vfs = new Vfs(nC);

    if(vfs)
	{
        //����vfs��Ա����(lsdir,movefile,copyfile,rmfile,getattr)
        //string fields[1] = {"name"};
        //LsResult *lsres = vfs->lsdir("/Netdisk/",fields,1,3);
		string fields[4] = {"name","url","type","size"};
		LsResult *lsres = vfs->lsdir("/Netdisk/",fields,4,3);

        if(lsres)
		{
            //��ӡ�б���Ϣ
            for(unsigned int i =0;i<lsres->childList.size();i++)
			{
                cout<<"lsdir:name=" << lsres->childList[i].name.c_str()<<"    type="<<lsres->childList[i].type.c_str()<<"    size="<<lsres->childList[i].size.c_str()<<endl;
            }
            //ɾ�����ؽ����ɾ������vfs����
            FreeLsResult(lsres);
        }
/*
        //ʹ�����ص��½ӿ�
        map<string,string>options;
        options.insert(pair<string,string>("type","3"));
        lsres = vfs->lsdir("/Netdisk/",fields,4,options);
        if(lsres){
            //��ӡ�б���Ϣ
            cout<<"lsdir:\nname=" << lsres->childList[0].name.c_str()<<"\ntype="<<lsres->childList[0].type.c_str()<<"\nurl="<<lsres->childList[0].url.c_str()<<endl;

            //ɾ�����ؽ����ɾ������vfs����
            FreeLsResult(lsres);
        }*/
        delete vfs;
    }
}

void progress_bar(double donepart,double allparts){
    printf("<<< %.2f%%\n",donepart/allparts);
}

void uploadtest(nspClient *nC)
{
     //�����ϴ��ӿ�
     Result *res=nC->upload("/Netdisk/���",3,"D://a.txt","D://b.doc","D://c.rar");

     if(res)
	 {
         //��ӡ�ϴ����
		  // ��������
         //cout<<"\n\nupload:\nname:" << res->failList[0].name.c_str()<<"\nerror:"<<res->failList[0].errMsg.c_str()<<endl;

         //ɾ�����ؽ��
         FreeResult(res);
         res = NULL;
     }

     //���ô����Ȼص����ϴ��ӿ�
     res=nC->upload_progress("/Netdisk/���","D://d.tar.gz",(void *)&progress_bar);
     if(res)
	 {
         //��ӡ�ϴ���� 
         // ��������
         //cout<<"\n\nupload:\nname:" << res->failList[0].name.c_str()<<"\ntype:"<<res->failList[0].type.c_str()<<endl;

         //ɾ�����ؽ��
         FreeResult(res);
     }

	 //�����ϴ��ӿ�
	 res=nC->upload("/Netdisk/�ĵ�",1,"D://test.cpp");

	 if(res)
	 {
		 //��ӡ�ϴ����
		  // ��������
		 //cout<<"\n\nupload:\nname:" << res->failList[0].name.c_str()<<"\nerror:"<<res->failList[0].errMsg.c_str()<<endl;

		 //ɾ�����ؽ��
		 FreeResult(res);
		 res = NULL;
	 }
}

void downloadtest(nspClient *nC)
{
    if(nC->download("/Netdisk/�ĵ�/test.cpp","D://test.cpp",(void *)&progress_bar))
	{
        printf("\n\ndownload  <Netdisk/�ĵ�/test.cpp> succeeded\n");
    }
    else
	{
        printf("\n\ndownload <Netdisk/�ĵ�/test.cpp> failed\n");
    }
}

void rmfiletest(nspClient *nC){

    //����Vfs��������
    Vfs *vfs = new Vfs(nC);

    if(vfs)
	{
        string files[] = {"/Netdisk/���/Settimer.rar","/Netdisk/�ĵ�/test.cpp"};
        map<string,string>empty;
        Result *res = vfs->rmfile(files,2,false,empty);

        if(res)
		{
            //������ؽ��
			 // ��������
   //         cout<<"\nrmfile:\r\nname = "<<endl;
			//cout<<res->failList[0].name.c_str()<<endl;
			//cout<<"\nerror:"<<res->failList[0].errMsg.c_str()<<endl;

            //ɾ�����ؽ��
            FreeResult(res);
        }
        delete vfs;
    }
}

void getInfotest(nspClient *nC){

    //�����û���Ϣ��������
    User *user = new User(nC);

    if(user)
	{
        string attrs[] = {"user.username","product.productname","profile.usedspacecapacity"};
        map<string,string> *res = user->getInfo(attrs,3);
        if(res)
		{
            //��ӡ���ؽ��
            map<string,string>::iterator r;
            for(r=res->begin();r!=res->end();r++)
			{
                cout << r->first << " " << r->second << endl;
            }
            //ɾ�����ؽ��
            FreeMap(res);
        }
        delete user;
    }
}

void getDirectUrlTest(nspClient *nC){

    //����ֱ����������
    Vfs_Link *vl = new Vfs_Link(nC);

    if(vl){
        map<string,string> *res = vl->getDirectUrl("/�ҵ�����/�ҵ�Ӧ��/PublicFiles/testlua.zip","");
        if(res){
            //��ӡ���ؽ��
            map<string,string>::iterator r;
            cout << "ֱ����Ϣ��" << endl;

            for(r=res->begin();r!=res->end();r++){
                cout << r->first << " " << r->second << endl;
            }

            cout << endl << endl;
            //ɾ�����ؽ��
            FreeMap(res);
        }
        delete vl;
    }
}

int main(int argc, char *argv[])
{
    //system level
    //...
    nspClient *nC2;
    
    nC2 = NSPClient("53120", "00zY5fgDkZqo1X9cOpl2N7T5a8GMgQZw");
	//nC2 = NSPClient("51345", "6ykOuxbeL68502d9FVRI766W1drADlwn");
    if(nC2!=NULL)
    {
        //����ֱ��
        getDirectUrlTest(nC2);

		//lsdirtest(nC2);
		//NSPClientDelete(nC);
    }

    //customer level
    //init sessionid and secret
    nspClient *nC;

    nC = NSPClient("KuCAhtkuCROWUuTRQKWuaF1E5uuXyZGHzH4Ll7p9HRYO98Qp","ba61b1c6df4d918030e2985e26a73b78");
	//nC = NSPClient("iuTeAN9uaQ6xYuCt8f7uaL4Hwua5CgiU2J0kYJq01KtsA4DY","c94f61061b46668c25d377cead92f898");
   if(nC!=NULL)
    {
        //�о�Ŀ¼
        lsdirtest(nC);
        //�����ļ�
        downloadtest(nC);
        //ɾ���ļ�
        rmfiletest(nC);
        //�ϴ��ļ�
        uploadtest(nC);

        //�û���Ϣ��ȡ
        getInfotest(nC);
        
        //ɾ����������
        NSPClientDelete(nC);
    }

    system("pause");
    return 0;
}
