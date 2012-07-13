#include "NSPClient.h"

bool FreeResult(Result *r){
    if(r!=NULL){
        delete r;
        return true;
    }
    return false;
}

bool FreeLsResult(LsResult *lr){
    if(lr!=NULL){
        delete lr;
        return true;
    }
    return false;
}

bool FreeMap(map<string,string> *m){
    if(m!=NULL){
        m->clear();
        delete m;
        return true;
    }
    return false;
}

Result* NSPWrapper::convert_to_result(nspAction *nA,nsp_node *nn){
    if(nn==NULL || nn->node_type!=NSP_NODE_MAP){return NULL;}
    Result *res = new Result();
    if(res==NULL){return NULL;}

    int i=0,count=0;
    nsp_node *t_node;

    nsp_node *succ = nA->searchNodeByStr(nn,"successList");
    if(succ==NULL){
        delete res;return NULL;
    }
    count = nA->getArrayNodeSize(succ);
    for(i=0;i<count;i++){
        nsp_node *succ_node = nA->searchNodeByPos(succ,i);
        if(succ_node != NULL){
            File fil;
            t_node = nA->searchNodeByStr(succ_node,"name");
            if(t_node!=NULL){
                fil.name = nA->getNodeStringValue(t_node);
            }
            t_node = nA->searchNodeByStr(succ_node,"type");
            if(t_node!=NULL){
                fil.type = nA->getNodeStringValue(t_node);
            }
            t_node = nA->searchNodeByStr(succ_node,"createTime");
            if(t_node!=NULL){
                fil.createTime = nA->getNodeStringValue(t_node);
            }
            t_node = nA->searchNodeByStr(succ_node,"modifyTime");
            if(t_node!=NULL){
                fil.modifyTime = nA->getNodeStringValue(t_node);
            }
            t_node = nA->searchNodeByStr(succ_node,"url");
            if(t_node!=NULL){
                fil.url = nA->getNodeStringValue(t_node);
            }
            t_node = nA->searchNodeByStr(succ_node,"size");
            if(t_node!=NULL){
                fil.size = nA->getNodeStringValue(t_node);
            }
            res->successList.push_back(fil);
        }
    }

    nsp_node *fail = nA->searchNodeByStr(nn,"failList");
    if(fail==NULL){
        delete res;return NULL;
    }
    count = nA->getArrayNodeSize(fail);
    for(i=0;i<count;i++){
        nsp_node *fail_node = nA->searchNodeByPos(fail,i);
        if(fail_node != NULL){
            Error err;
            t_node = nA->searchNodeByStr(fail_node,"name");
            if(t_node!=NULL){
                err.name = nA->getNodeStringValue(t_node);
            }
            t_node = nA->searchNodeByStr(fail_node,"type");
            if(t_node!=NULL){
                err.type = nA->getNodeStringValue(t_node);
            }
            t_node = nA->searchNodeByStr(fail_node,"errCode");
            if(t_node!=NULL){
                if(t_node->node_type == NSP_NODE_INT){
                    err.errCode = nA->getNodeIntValue(t_node)+"";
                }
                else{
                    err.errCode = nA->getNodeStringValue(t_node);
                }
            }
            t_node = nA->searchNodeByStr(fail_node,"errMsg");
            if(t_node!=NULL){
                err.errMsg = nA->getNodeStringValue(t_node);
            }
            res->failList.push_back(err);
        }
    }
    return res;
}

LsResult* NSPWrapper::convert_to_lsresult(nspAction *nA,nsp_node *nn){
    if(nn==NULL || nn->node_type!=NSP_NODE_MAP){return NULL;}
    LsResult *lsres = new LsResult();
    if(lsres==NULL){return NULL;}

    int i=0,count=0;
    nsp_node *t_node;

    nsp_node *child = nA->searchNodeByStr(nn,"childList");
    if(child==NULL){
        delete lsres;return NULL;
    }
    count = nA->getArrayNodeSize(child);
    for(i=0;i<count;i++){
        nsp_node *child_node = nA->searchNodeByPos(child,i);
        if(child_node != NULL){
            File fil;
            t_node = nA->searchNodeByStr(child_node,"name");
            if(t_node!=NULL){
                fil.name = nA->getNodeStringValue(t_node);
            }
            t_node = nA->searchNodeByStr(child_node,"type");
            if(t_node!=NULL){
                fil.type = nA->getNodeStringValue(t_node);
            }
            t_node = nA->searchNodeByStr(child_node,"createTime");
            if(t_node!=NULL){
                fil.createTime = nA->getNodeStringValue(t_node);
            }
            t_node = nA->searchNodeByStr(child_node,"modifyTime");
            if(t_node!=NULL){
                fil.modifyTime = nA->getNodeStringValue(t_node);
            }
            t_node = nA->searchNodeByStr(child_node,"url");
            if(t_node!=NULL){
                fil.url = nA->getNodeStringValue(t_node);
            }
            t_node = nA->searchNodeByStr(child_node,"size");
            if(t_node!=NULL){
                fil.size = nA->getNodeStringValue(t_node);
            }
            lsres->childList.push_back(fil);
        }
    }
    t_node = nA->searchNodeByStr(nn,"errCode");
    if(t_node!=NULL){
        lsres->errCode = nA->getNodeStringValue(t_node);
    }
    t_node = nA->searchNodeByStr(nn,"errMsg");
    if(t_node!=NULL){
        lsres->errMsg = nA->getNodeStringValue(t_node);
    }
    return lsres;
}

map<string,string>* NSPWrapper::convert_to_map(nspAction *nA,nsp_node *nn){
    if(nn==NULL || nn->node_type!=NSP_NODE_MAP){
        return NULL;
    }
    map<string,string>* m = new map<string,string>();
    if(m==NULL){
        return NULL;
    }

    valuemap* vm=nn->node_data.child;
    while(vm!=NULL){
        m->insert(pair<string,string>(nA->getNodeStringValue(vm->key),nA->getNodeStringValue(vm->value)));
        vm=vm->next;
    }
    return m;
}


map<string,string>* User::getInfo(string attrs[],int attrs_count){
    //make send information
    nsp_node *send = nC->action->createArray();
    nsp_node *val=this->nC->action->createArray();
    for(int i=0;i<attrs_count;i++){
        this->nC->action->appendString(val,(char *)attrs[i].c_str());
    }
    this->nC->action->appendArray(send,val);

    nsp_node *recv = nC->invoke("nsp.user.getInfo",send);
    map<string,string>* res = NSPWrapper::convert_to_map(nC->action,recv);

    //free memory
    this->nC->action->deleteNode(send);
    this->nC->action->deleteNode(recv);

    return res;
}

Vfs::Vfs(nspClient *nC){
    this->nC = nC;
}

LsResult* Vfs::lsdir(string path,string fields[],int fields_count,int type){
    //make send information
    nsp_node *send = nC->action->createArray();
    nC->action->appendString(send,(char *)path.c_str());
    nsp_node *val=nC->action->createArray();
    for(int i=0;i<fields_count;i++){
        nC->action->appendString(val,(char *)fields[i].c_str());
    }
    nC->action->appendArray(send,val);
    nC->action->appendInt(send,type);

    nsp_node *recv = nC->invoke("nsp.vfs.lsdir",send);
    LsResult *lsres = NSPWrapper::convert_to_lsresult(nC->action,recv);
    //free memory
    nC->action->deleteNode(send);
    nC->action->deleteNode(recv);

    return lsres;
}

LsResult* Vfs::lsdir(string path,string fields[],int fields_count,map<string,string> &option){
    //make send information
    nsp_node *send = nC->action->createArray();
    nC->action->appendString(send,(char *)path.c_str());
    nsp_node *val=nC->action->createArray();
    for(int i=0;i<fields_count;i++){
        nC->action->appendString(val,(char *)fields[i].c_str());
    }
    nC->action->appendArray(send,val);
    if(option.size()>0){
        nsp_node *opt = nC->action->createArray();
        map<string,string>::iterator ite;
        for(ite=option.begin();ite!=option.end();ite++){
            nC->action->appendMap_Str_Str(opt,(char *)ite->first.c_str(),(char *)ite->second.c_str());
        }
        nC->action->appendMap_Str_Node(send,"option",opt);
    }

    nsp_node *recv = nC->invoke("nsp.vfs.lsdir",send);
    LsResult *lsres = NSPWrapper::convert_to_lsresult(nC->action,recv);
    //free memory
    nC->action->deleteNode(send);
    nC->action->deleteNode(recv);

    return lsres;
}
Result* Vfs::copyfile(string files[],int files_count,string path,map<string,string> &attribute){
    //make send information
    nsp_node *send = nC->action->createArray();
    nsp_node *val=nC->action->createArray();
    for(int i=0;i<files_count;i++){
        nC->action->appendString(val,(char *)files[i].c_str());
    }
    nC->action->appendMap_Str_Node(send,"files",val);
    nC->action->appendMap_Str_Str(send,"path",(char *)path.c_str());

    if(attribute.size()>0){
        nsp_node *att = nC->action->createArray();
        map<string,string>::iterator ite;
        for(ite=attribute.begin();ite!=attribute.end();ite++){
            nC->action->appendMap_Str_Str(att,(char *)ite->first.c_str(),(char *)ite->second.c_str());
        }
        nC->action->appendMap_Str_Node(send,"attribute",att);
    }

    //send information and receive result and print
    nsp_node *recv = nC->invoke("nsp.vfs.copyfile",send);
    Result *res = NSPWrapper::convert_to_result(nC->action,recv);

    //free memory
    nC->action->deleteNode(send);
    nC->action->deleteNode(recv);

    return res;
}
Result* Vfs::movefile(string files[],int files_count,string path,map<string,string> &attribute){
    //make send information
    nsp_node *send = nC->action->createArray();
    nsp_node *val=nC->action->createArray();
    for(int i=0;i<files_count;i++){
        nC->action->appendString(val,(char *)files[i].c_str());
    }
    nC->action->appendMap_Str_Node(send,"files",val);
    nC->action->appendMap_Str_Str(send,"path",(char *)path.c_str());

    if(attribute.size()>0){
        nsp_node *att = nC->action->createArray();
        map<string,string>::iterator ite;
        for(ite=attribute.begin();ite!=attribute.end();ite++){
            nC->action->appendMap_Str_Str(att,(char *)ite->first.c_str(),(char *)ite->second.c_str());
        }
        nC->action->appendMap_Str_Node(send,"attribute",att);
    }

    //send information and receive result and print
    nsp_node *recv = nC->invoke("nsp.vfs.movefile",send);
    Result *res = NSPWrapper::convert_to_result(nC->action,recv);

    //free memory
    nC->action->deleteNode(send);
    nC->action->deleteNode(recv);

    return res;
}

Result* Vfs::rmfile(string files[],int files_count,bool reverse,map<string,string> &attribute){
    //make send information
    nsp_node *send = nC->action->createArray();
    nsp_node *val=nC->action->createArray();
    for(int i=0;i<files_count;i++){
        nC->action->appendString(val,(char *)files[i].c_str());
    }
    nC->action->appendMap_Str_Node(send,"files",val);
    nC->action->appendMap_Str_Int(send,"reverse",reverse);

    if(attribute.size()>0){
        nsp_node *att = nC->action->createArray();
        map<string,string>::iterator ite;
        for(ite=attribute.begin();ite!=attribute.end();ite++){
            nC->action->appendMap_Str_Str(att,(char *)ite->first.c_str(),(char *)ite->second.c_str());
        }
        nC->action->appendMap_Str_Node(send,"attribute",att);
    }

    //send information and receive result and print
    nsp_node *recv = nC->invoke("nsp.vfs.rmfile",send);
    Result *res = NSPWrapper::convert_to_result(nC->action,recv);
    //free memory
    nC->action->deleteNode(send);
    nC->action->deleteNode(recv);

    return res;
}

Result* Vfs::mkdir(string path,string dirs[],int dirs_count){
    //make send information
    nsp_node *send = nC->action->createArray();
    nsp_node *mkfile=nC->action->createArray();

    for(int i=0;i<dirs_count;i++){

        nsp_node *mkf=nC->action->createArray();
        nC->action->appendMap_Str_Str(mkf, "type", "Directory");
        nC->action->appendMap_Str_Str(mkf, "name", (char *)dirs[i].c_str());

        nC->action->appendArray(mkfile, mkf);
    }
    nC->action->appendMap_Str_Node(send,"files",mkfile);
    nC->action->appendMap_Str_Str(send,"path",(char *)path.c_str());

    nsp_node *recv = nC->invoke("nsp.vfs.mkfile",send);
    Result *res = NSPWrapper::convert_to_result(nC->action,recv);
    //free memory
    nC->action->deleteNode(send);
    nC->action->deleteNode(recv);

    return res;
}

Result* Vfs::getattr(string files[],int files_count,string fields[],int fields_count){
    //make send information
    nsp_node *send = nC->action->createArray();
    nsp_node *val1=nC->action->createArray();
    int i;
    for(i=0;i<files_count;i++){
        nC->action->appendString(val1,(char *)files[i].c_str());
    }
    nC->action->appendMap_Str_Node(send,"files",val1);
    nsp_node *val2=nC->action->createArray();
    for(i=0;i<fields_count;i++){
        nC->action->appendString(val2,(char *)fields[i].c_str());
    }
    nC->action->appendMap_Str_Node(send,"fields",val2);

    //send information and receive result and print
    nsp_node *recv = nC->invoke("nsp.vfs.getattr",send);
    Result *res = NSPWrapper::convert_to_result(nC->action,recv);

    //free memory
    nC->action->deleteNode(send);
    nC->action->deleteNode(recv);

    return res;
}

map<string,string>* Vfs_Link::getDirectUrl(string path,string clientIp){
    //make send information
    nsp_node *send = nC->action->createArray();
    
    this->nC->action->appendMap_Str_Str(send,"path",(char *)path.c_str());
    if(clientIp!=""){
        this->nC->action->appendMap_Str_Str(send,"clientIp",(char *)clientIp.c_str());
    }

    nsp_node *recv = nC->invoke("nsp.vfs.link.getDirectUrl",send);
    map<string,string>* res = NSPWrapper::convert_to_map(nC->action,recv);

    //free memory
    this->nC->action->deleteNode(send);
    this->nC->action->deleteNode(recv);

    return res;
}