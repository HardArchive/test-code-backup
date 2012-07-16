#include <iostream>
//你懂的...
#include "NSPClient.h"

//测试用户信息接口(返回值为map类型)
#include<map>

using namespace std;

//system level use appid
//...

//customer level use session

void lsdirtest(nspClient *nC)
{
    //构造Vfs操作对象
    Vfs *vfs = new Vfs(nC);

    if(vfs){
        //调用vfs成员函数(lsdir,movefile,copyfile,rmfile,getattr)
        string fields[4] = {"name","url","type","size"};
        LsResult *lsres = vfs->lsdir("/Netdisk/",fields,4,3);

        if(lsres){
            //打印列表信息
            for(unsigned int i =0;i<lsres->childList.size();i++){
                cout<<"lsdir:name=" << lsres->childList[i].name.c_str()<<"type="<<lsres->childList[i].type.c_str()<<"size="<<lsres->childList[i].size.c_str()<<endl;
            }
            //删除返回结果，删除操作vfs对象
            FreeLsResult(lsres);
        }
/*
        //使用重载的新接口
        map<string,string>options;
        options.insert(pair<string,string>("type","3"));
        lsres = vfs->lsdir("/Netdisk/",fields,4,options);
        if(lsres){
            //打印列表信息
            cout<<"lsdir:\nname=" << lsres->childList[0].name.c_str()<<"\ntype="<<lsres->childList[0].type.c_str()<<"\nurl="<<lsres->childList[0].url.c_str()<<endl;

            //删除返回结果，删除操作vfs对象
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
     //调用上传接口
     Result *res=nC->upload("/Netdisk/软件",3,"D://a.txt","D://b.doc","D://c.rar");

     if(res){
         //打印上传结果
         cout<<"\n\nupload:\nname:" << res->failList[0].name.c_str()<<"\nerror:"<<res->failList[0].errMsg.c_str()<<endl;

         //删除返回结果
         FreeResult(res);
         res = NULL;
     }

     //调用带进度回调的上传接口
     res=nC->upload_progress("/Netdisk/软件","D://d.tar.gz",(void *)&progress_bar);
     if(res){
         //打印上传结果
         cout<<"\n\nupload:\nname:" << res->failList[0].name.c_str()<<"\ntype:"<<res->failList[0].type.c_str()<<endl;

         //删除返回结果
         FreeResult(res);
     }
}

void downloadtest(nspClient *nC)
{
    if(nC->download("/Netdisk/文档/test.cpp","D://test.cpp",(void *)&progress_bar)){
        printf("\n\ndownload succeeded\n");
    }
    else{
        printf("\n\ndownload failed\n");
    }
}

void rmfiletest(nspClient *nC){

    //构造Vfs操作对象
    Vfs *vfs = new Vfs(nC);

    if(vfs){
        string files[] = {"/Netdisk/软件/nginx_pdf.tar.gz","/Netdisk/penjin.txt"};
        map<string,string>empty;
        Result *res = vfs->rmfile(files,2,false,empty);

        if(res){
            //输出返回结果
            cout<<"\n\nrmfile:\nname="<<res->failList[0].name.c_str()<<"\nerror:"<<res->failList[0].errMsg.c_str()<<endl;

            //删除返回结果
            FreeResult(res);
        }
        delete vfs;
    }
}

void getInfotest(nspClient *nC){

    //构造用户信息操作对象
    User *user = new User(nC);

    if(user){
        string attrs[] = {"user.username","product.productname","profile.usedspacecapacity"};
        map<string,string> *res = user->getInfo(attrs,3);
        if(res){
            //打印返回结果
            map<string,string>::iterator r;
            for(r=res->begin();r!=res->end();r++){
                cout << r->first << " " << r->second << endl;
            }
            //删除返回结果
            FreeMap(res);
        }
        delete user;
    }
}

void getDirectUrlTest(nspClient *nC){

    //构造直链操作对象
    Vfs_Link *vl = new Vfs_Link(nC);

    if(vl){
        map<string,string> *res = vl->getDirectUrl("/我的网盘/我的应用/PublicFiles/testlua.zip","");
        if(res){
            //打印返回结果
            map<string,string>::iterator r;
            cout << "直链信息：" << endl;

            for(r=res->begin();r!=res->end();r++){
                cout << r->first << " " << r->second << endl;
            }

            cout << endl << endl;
            //删除返回结果
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
    
    nC2 = NSPClient("51345", "6ykOuxbeL68502d9FVRI766W1drADlwn");
    if(nC2!=NULL)
    {
        //生成直链
        getDirectUrlTest(nC2);
    }

    //customer level
    //init sessionid and secret
    nspClient *nC;

    nC = NSPClient("iuTeAN9uaQ6xYuCt8f7uaL4Hwua5CgiU2J0kYJq01KtsA4DY","c94f61061b46668c25d377cead92f898");
    if(nC!=NULL)
    {
        //列举目录
        lsdirtest(nC);
        //下载文件
        downloadtest(nC);
        //删除文件
        rmfiletest(nC);
        //上传文件
        uploadtest(nC);

        //用户信息获取
        getInfotest(nC);
        
        //删除操作对象
        NSPClientDelete(nC);
    }

    system("pause");
    return 0;
}
