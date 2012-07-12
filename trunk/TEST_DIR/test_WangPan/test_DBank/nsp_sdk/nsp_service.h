//封装了C++风格接口，提供VFS，USER操作类以及Result，LsResult结果对象
#ifndef __NSP_SERVICE_H
#define __NSP_SERVICE_H

#include<vector>
#include<map>
#include<string>
#include"nsp_node.h"

using namespace std;

//this struct is highly recomended for use
struct nspClient{
    nsp_node* (*invoke)(char *,void *);
    Result* (*upload)(char *,int,...);
    Result* (*upload_progress)(char *,char *,void *);
    bool (*download)(char *,char *,void *);

    nspAction *action;
};

//return result
class File{
public:
    string type;
    string name;
    string createTime;
    string modifyTime;
    string url;
    string size;
};

class Error{
public:
    string name;
    string type;
    string errCode;
    string errMsg;
};

class LsResult{
public:
    vector<File>childList;
    string errCode;
    string errMsg;
};

class Result{
public:
    ~Result(){}
    vector<File>successList;
    vector<Error>failList;
};

class NSPWrapper{
public:
    static Result *convert_to_result(nspAction *nA,nsp_node *nn);
    static LsResult *convert_to_lsresult(nspAction *nA,nsp_node *nn);
    static map<string,string> *convert_to_map(nspAction *nA,nsp_node *nn);
};

class NSP_EXPORT_DLL User{

    nspClient *nC;
public:
    User(nspClient *nC){
        this->nC = nC;
    }
    map<string,string>* getInfo(string attrs[],int attrs_count);
};

class NSP_EXPORT_DLL Vfs{
    nspClient *nC;
public:
    Vfs(nspClient *nC);
    LsResult* lsdir(string path,string fields[],int fields_count,int type);
	LsResult* lsdir(string path,string fields[],int fields_count,map<string,string> &option);
    Result* copyfile(string files[],int files_count,string path,map<string,string> &attribute);
    Result* movefile(string files[],int files_count,string path,map<string,string> &attribute);
    Result* rmfile(string files[],int files_count,bool reverse,map<string,string> &attribute);
    Result* getattr(string files[],int files_count,string fields[],int fields_count);
	Result* mkdir(string path,string dirs[],int dirs_count);
};

class NSP_EXPORT_DLL Vfs_Link{

    nspClient *nC;
public:
    Vfs_Link(nspClient *nC){
        this->nC = nC;
    }
    map<string,string>* getDirectUrl(string path,string cliendIp);
};
#endif

