#include "nsp_node.h"
#include "NSPClient.h"

bool nspArrayAppendInt(nsp_node *nn,int i);
bool nspArrayAppendString(nsp_node *nn,char *str);
bool nspArrayAppendDouble(nsp_node *nn,double d);
bool nspArrayAppendArray(nsp_node *nn,nsp_node *arr);

//create new node and alloc memory for it
static nsp_node* nsp_new_node(){
    nsp_node *new_item=(nsp_node *)malloc(sizeof(nsp_node));
    if(new_item){memset(new_item,0,sizeof(nsp_node));}
    return new_item;
}
static valuemap* nsp_new_valuemap(){
    valuemap *new_vm=(valuemap *)malloc(sizeof(valuemap));
    if(new_vm){memset(new_vm,0,sizeof(valuemap));}
    return new_vm;
}

//free item memory recursively
void nsp_free_node(nsp_node *sn);
void __nsp_free_child(valuemap *vm)
{
    if(vm==NULL){
        return;
    }
    else if(vm->next!=NULL){
        __nsp_free_child(vm->next);
        vm->next=NULL;
    }
    nsp_free_node(vm->key);
    nsp_free_node(vm->value);
    free(vm);
    vm=NULL;
}
void nsp_free_node(nsp_node *sn){
    if(sn==NULL){
        return;
    }
    switch(sn->node_type){
        case NSP_NODE_NULL:
        case NSP_NODE_BOOL:
        case NSP_NODE_INT:
            sn->node_data.valueint=0;
        case NSP_NODE_DOUBLE:
            sn->node_data.valuedouble=0;
            break;
        case NSP_NODE_STRING:
            free(sn->node_data.valuestring);
            sn->node_data.valuestring=NULL;
            break;
        case NSP_NODE_MAP:
            __nsp_free_child(sn->node_data.child);
            sn->countmap=0;
            break;
    }
    sn->node_type=NSP_NODE_NULL;
    free(sn);
    sn=NULL;
}

//print node detail
void __nsp_print_node(nsp_node *sn,int level){
    if(sn==NULL){
        return;
    }
    switch(sn->node_type){
        case NSP_NODE_NULL:
            printf("NULL");break;
        case NSP_NODE_BOOL:
            if(sn->node_data.valueint==1){printf("TRUE");}
            else{printf("FALSE");}
            break;
        case NSP_NODE_INT:
            printf("%d",sn->node_data.valueint);
            break;
        case NSP_NODE_DOUBLE:
            printf("%f",sn->node_data.valuedouble);
            break;
        case NSP_NODE_STRING:
            printf("\"%s\"",sn->node_data.valuestring);
            break;
        case NSP_NODE_MAP:
            printf("Array[]\n");
            valuemap* vm=sn->node_data.child;
            while(vm!=NULL){
                int i;
                for(i=0;i<level;i++){
                    printf("\t");
                }
                printf("[");
                __nsp_print_node(vm->key,level+1);
                printf("]=> ");
                __nsp_print_node(vm->value,level+1);
                printf("\n");
                vm=vm->next;
            }
            break;
    }
}
void nsp_print_node(nsp_node *sn){
    printf("\n");
    __nsp_print_node(sn,1);
}

//
//unserialize char * and store into nsp_node
//
static const char* nsp_parse_value(nsp_node *nn,const char *str);

static const char* nsp_parse_null(nsp_node *nn,const char *str){
    nn->node_type=NSP_NODE_NULL;
    return str;
}

static const char* nsp_parse_bool(nsp_node *nn,const char *str){
    if(str==NULL){return NULL;}
    nn->node_type=NSP_NODE_BOOL;
    if(*str=='1'){
        nn->node_data.valueint=1;
    }
    else{
        nn->node_data.valueint=0;
    }
    return str+2;
}

static const char* nsp_parse_number(nsp_node *nn,const char *num){
    if(num==NULL){return 0;}
    double n=0,sign=1,scale=0;int subscale=0,signsubscale=1;

    if (*num=='-') sign=-1,num++;    /* Has sign? */
    if (*num=='0') num++;            /* is zero */
    if (*num>='1' && *num<='9')    do    n=(n*10.0)+(*num++ -'0');    while (*num>='0' && *num<='9');    /* Number? */
    if (*num=='.' && num[1]>='0' && num[1]<='9') {num++;        do    n=(n*10.0)+(*num++ -'0'),scale--; while (*num>='0' && *num<='9');}    /* Fractional part? */
    if (*num=='e' || *num=='E')        /* Exponent? */
    {    num++;if (*num=='+') num++;    else if (*num=='-') signsubscale=-1,num++;        /* With sign? */
        while (*num>='0' && *num<='9') subscale=(subscale*10)+(*num++ - '0');    /* Number? */
    }

    n=sign*n*pow(10.0,(scale+subscale*signsubscale));    /* number = +/- number.fraction * 10^+/- exponent */
    
    if(int(n)==n){
        nn->node_type=NSP_NODE_INT;
        nn->node_data.valueint=int(n);
    }
    else{
        nn->node_type=NSP_NODE_DOUBLE;
        nn->node_data.valuedouble=n;
    }
    return num+1;
}

static const char* nsp_parse_string(nsp_node *nn,const char *str){
    if(str==NULL){return 0;}
    //get string length
    //eg.    "s:123:"apple..."" means that string has a length of 123
    int len=0;
    while(*str!=':'){len=len*10+int(*str)-48;str++;}
    //skip ' :" '
    str+=2;
    if(str==NULL){return 0;}
    nn->node_type=NSP_NODE_STRING;
    nn->node_data.valuestring = (char *)malloc(len+1);
    if(nn->node_data.valuestring==NULL){return 0;}
    const char *start=str;
    len=0;
    while(*str!='"'){len++;str++;}
    strncpy(nn->node_data.valuestring,start,len);
    nn->node_data.valuestring[len]='\0';
    return str+2;
}

static const char* nsp_parse_map(nsp_node *nn,const char *str){
    if(str==NULL){return 0;}
    int count=0;
    //get array size information
    //eg.    "a:34:{..."    means array has 34 items
    while(*str!=':'){count=count*10+int(*str)-48;str++;}
    //skip ' :{ '
    str+=2;
    if(str==NULL){return 0;}
    nn->node_type=NSP_NODE_MAP;
    nn->countmap=count;
    //begin to create its child item
    valuemap *child=nsp_new_valuemap();
    if(child==NULL){nsplog("内部错误",RET_MEMORY_MSG);return 0;}
    nn->node_data.child=child;
    while(count--){
        valuemap *next_item;
        //analyse key part
        child->key=nsp_new_node();
        if(child->key==NULL){nsplog("内部错误",RET_MEMORY_MSG);return 0;}
        str=nsp_parse_value(child->key,str);
        if(str==NULL){return 0;}
        //analyse value part
        child->value=nsp_new_node();
        if(child->value==NULL){nsplog("内部错误",RET_MEMORY_MSG);return 0;}
        str=nsp_parse_value(child->value,str);
        if(str==NULL){nsplog("内部错误",RET_INVALID_DATA_MSG);return 0;}
        //for the last child , we no longer have to make a brother for him
        if(count!=0){
            next_item=nsp_new_valuemap();child->next=next_item;child=next_item;
        }
    }
    return str+1;
}

static const char* nsp_parse_value(nsp_node *nn,const char *str){
    
    if(!str)    return 0;
    if(!strncmp(str,"N;",2)){return nsp_parse_null(nn,str+2);}                //N;    NULL
    if(!strncmp(str,"b:",2)){return nsp_parse_bool(nn,str+2);}                //b:    Boolean
    if(!strncmp(str,"i:",2)){return nsp_parse_number(nn,str+2);}            //i:    Integer
    if(!strncmp(str,"d:",2)){return nsp_parse_number(nn,str+2);}            //d:    Double
    if(!strncmp(str,"s:",2)){return nsp_parse_string(nn,str+2);}            //s:    String
    if(!strncmp(str,"a:",2)){return nsp_parse_map(nn,str+2);}                //a:    Array or Map
    
    return 0;
}

nsp_node* nsp_unserialize(char buf[]){
    nsp_node *nn=nsp_new_node();
    if(nn==NULL){return NULL;}

    if(nsp_parse_value(nn,buf)){
        return nn;
    }
    else{
        nsp_free_node(nn);
        return NULL;
    }
}

//
//serialize nsp_node to str
//
void __nsp_serialize(nsp_node *nn,char seribuf[]){
    if(nn==NULL || strlen(seribuf) > SMALLBUFSIZE ){return;}
    char buf[32];
     switch(nn->node_type){
        case NSP_NODE_NULL:
            strcat(seribuf,"N;");
            break;
        case NSP_NODE_BOOL:
            sprintf(buf,"b:%d;",nn->node_data.valueint);
            strcat(seribuf,buf);
            break;
        case NSP_NODE_INT:
            sprintf(buf,"i:%d;",nn->node_data.valueint);
            strcat(seribuf,buf);
            break;
        case NSP_NODE_DOUBLE:
            sprintf(buf,"d:%f;",nn->node_data.valuedouble);
            strcat(seribuf,buf);
            break;
        case NSP_NODE_STRING:
//change chinese from gbk to utf8 for http transport 
#if defined(WIN32)
            {
            int string_length=strlen(nn->node_data.valuestring);
            char *utf8string =(char *)malloc(string_length*2);
            if(utf8string!=NULL){
                convert("UTF-8","GBK",nn->node_data.valuestring,string_length,utf8string,2*string_length);
                sprintf(buf,"s:%d:\"",strlen(utf8string));
                strcat(seribuf,buf);
                strcat(seribuf,utf8string);
                free(utf8string);utf8string=NULL;
            }
            else{nsplog("内部错误",RET_MEMORY_MSG);return;}
            }
#else
            sprintf(buf,"s:%d:\"",strlen(nn->node_data.valuestring));
            strcat(seribuf,buf);
            strcat(seribuf,nn->node_data.valuestring);
#endif
            strcat(seribuf,"\";");
            break;
        case NSP_NODE_MAP:
            sprintf(buf,"a:%d:{",nn->countmap);
            strcat(seribuf,buf);
            valuemap* vm=nn->node_data.child;
            while(vm!=NULL){
                __nsp_serialize(vm->key,seribuf);
                __nsp_serialize(vm->value,seribuf);
                vm=vm->next;
            }
            strcat(seribuf,"}");
            break;         
     }
}


char* nsp_serialize(nsp_node *nn){
    char *seri_buf=(char *)malloc(SMALLBUFSIZE);
    if(seri_buf==NULL){nsplog("内部错误",RET_MEMORY_MSG);return NULL;}
    memset(seri_buf,0,sizeof(seri_buf));
    __nsp_serialize(nn,seri_buf);
    return seri_buf;
}

//some functions for handling nsp_node
nsp_node* nspCreateBool(bool b){
    nsp_node *item=nsp_new_node();
    if(item!=NULL)
    {
        item->node_type=NSP_NODE_BOOL;
        item->node_data.valueint=(int)b;
    }
    return item;
}
nsp_node* nspCreateInt(int i){
    nsp_node* item=nsp_new_node();
    if(item!=NULL)
    {
        item->node_type=NSP_NODE_INT;
        item->node_data.valueint=i;
    }
    return item;
}
nsp_node* nspCreateDouble(double d){
    nsp_node* item=nsp_new_node();
    if(item!=NULL)
    {
        item->node_type=NSP_NODE_DOUBLE;
        item->node_data.valuedouble=d;
    }
    return item;
}
nsp_node* nspCreateString(char *str){
    nsp_node* item=nsp_new_node();
    if(item!=NULL)
    {
        item->node_type=NSP_NODE_STRING;
        item->node_data.valuestring=(char *)malloc(strlen(str)+1);
        if(item->node_data.valuestring==NULL){return NULL;}
        strcpy(item->node_data.valuestring,str);
    }
    return item;
}
nsp_node* nspCreateArray(){
    nsp_node* item=nsp_new_node();
    if(item!=NULL)
    {
        item->node_type=NSP_NODE_MAP;
    }
    return item;
}

int getArrayPos(nsp_node *nn){
    if(nn==NULL || nn->node_type!=NSP_NODE_MAP){
        return -1;
    }
    valuemap *vm=nn->node_data.child;
    int pos=-1;
    while(vm!=NULL && vm->key!=NULL){
        if(vm->key->node_type==NSP_NODE_INT && vm->key->node_data.valueint>pos){
            pos=vm->key->node_data.valueint;
        }
        vm=vm->next;
    }
    return pos+1;
}

nsp_node* nspCreateIntArray(int *numbers,int count){
    nsp_node* item=nsp_new_node();
    if(item==NULL){return NULL;}

    item->node_type=NSP_NODE_MAP;
    int i=0;
    for(;i<count;i++){
        nspArrayAppendInt(item,numbers[i]);
    }
    return item;
}
nsp_node* nspCreateDoubleArray(double *numbers,int count){
    nsp_node* item=nsp_new_node();
    if(item==NULL){return NULL;}

    item->node_type=NSP_NODE_MAP;
    int i=0;
    for(;i<count;i++){
        nspArrayAppendDouble(item,numbers[i]);
    }
    return item;    
}
nsp_node* nspCreateStringArray(int count,...){
    nsp_node* item=nsp_new_node();
    if(item==NULL){return NULL;}

    item->node_type=NSP_NODE_MAP;
    va_list lst;
    char *arg=NULL;
    va_start(lst, count);
    int tag=0;
    while((arg=va_arg(lst,char *))!=NULL && tag<count)
    {
        nspArrayAppendString(item,arg);
        tag++;
    }

    return item;    
}

bool nspArrayAppendMap(nsp_node *nn,nsp_node *key,nsp_node *val){
    if(nn==NULL || nn->node_type!=NSP_NODE_MAP){return false;}
    valuemap *vm=nsp_new_valuemap();
    if(vm==NULL){return false;}

    vm->key=key;
    vm->value=val;
    vm->next=NULL;
    valuemap *curr=nn->node_data.child;
    nn->countmap++;
    
    if(curr==NULL){
        nn->node_data.child=vm;
        return false;
    }
    while(curr->next!=NULL){
        curr=curr->next;
    }
    curr->next=vm;
    return true;
}
bool nspArrayAppendMapStrNode(nsp_node *nn,char *key,nsp_node *val){
    if(nn==NULL || nn->node_type!=NSP_NODE_MAP){return false;}
    nsp_node *k = nspCreateString(key);
    if(k==NULL){return false;}
    if(nspArrayAppendMap(nn,k,val)){return true;}
    return false;
}
bool nspArrayAppendMapStrStr(nsp_node *nn,char *key,char *val){
    if(nn==NULL || nn->node_type!=NSP_NODE_MAP){return false;}
    nsp_node *k = nspCreateString(key);
    if(k==NULL){return false;}
    nsp_node *v = nspCreateString(val);
    if(v==NULL){nsp_free_node(k);return false;}
    if(nspArrayAppendMap(nn,k,v)){return true;}
    return false;
}
bool nspArrayAppendMapStrInt(nsp_node *nn,char *key,int val){
    if(nn==NULL || nn->node_type!=NSP_NODE_MAP){return false;}
    nsp_node *k = nspCreateString(key);
    if(k==NULL){return false;}
    nsp_node *v = nspCreateInt(val);
    if(v==NULL){nsp_free_node(k);return false;}
    if(nspArrayAppendMap(nn,k,v)){return true;}
    return false;
}
bool nspArrayAppendBool(nsp_node *nn,bool b){
    if(nn==NULL || nn->node_type!=NSP_NODE_MAP){return false;}
    int p=getArrayPos(nn);
    if(p<0){return false;}
    nsp_node *k = nspCreateInt(p);
    if(k==NULL){return false;}
    nsp_node *v = nspCreateBool(b);
    if(v==NULL){nsp_free_node(k);return false;}
    if(nspArrayAppendMap(nn,k,v)){return true;}
    return false;
}
bool nspArrayAppendInt(nsp_node *nn,int i){
    if(nn==NULL || nn->node_type!=NSP_NODE_MAP){return false;}
    int p=getArrayPos(nn);
    if(p<0){return false;}
    nsp_node *k = nspCreateInt(p);
    if(k==NULL){return false;}
    nsp_node *v = nspCreateInt(i);
    if(v==NULL){nsp_free_node(k);return false;}
    if(nspArrayAppendMap(nn,k,v)){return true;}
    return false;
}
bool nspArrayAppendDouble(nsp_node *nn,double d){
    if(nn==NULL || nn->node_type!=NSP_NODE_MAP){return false;}
    int p=getArrayPos(nn);
    if(p<0){return false;}
    nsp_node *k = nspCreateInt(p);
    if(k==NULL){return false;}
    nsp_node *v = nspCreateDouble(d);
    if(v==NULL){nsp_free_node(k);return false;}
    if(nspArrayAppendMap(nn,k,v)){return true;}
    return false;
}
bool nspArrayAppendString(nsp_node *nn,char *str){
    if(nn==NULL || nn->node_type!=NSP_NODE_MAP){return false;}
    int p=getArrayPos(nn);
    if(p<0){return false;}
    nsp_node *k = nspCreateInt(p);
    if(k==NULL){return false;}
    nsp_node *v = nspCreateString(str);
    if(v==NULL){nsp_free_node(k);return false;}
    if(nspArrayAppendMap(nn,k,v)){return true;}
    return false;
}
bool nspArrayAppendArray(nsp_node *nn,nsp_node *arr){
    if(nn==NULL || nn->node_type!=NSP_NODE_MAP){return false;}
    int p=getArrayPos(nn);
    if(p<0){return false;}
    nsp_node *k = nspCreateInt(p);
    if(k==NULL){return false;}
    nsp_node *v = arr;
    if(nspArrayAppendMap(nn,k,v)){return true;}
    return false;
}

int nspNodeType(nsp_node *nn){
    if(nn==NULL){return 0;}
    return nn->node_type;
}
int nspArrayNodeSize(nsp_node *nn){
    if(nn!=NULL && nn->node_type==NSP_NODE_MAP){
        int count=0;
        valuemap *vm=nn->node_data.child;
        while(vm!=NULL){
            count++;
            vm=vm->next;
        }
        return count;
    }
    return 0;    
}

bool nspNodeBoolValue(nsp_node *nn){
    if(nn!=NULL && nn->node_type==NSP_NODE_BOOL){
        return nn->node_data.valueint;
    }
    return false;    
}
int nspNodeIntValue(nsp_node *nn){
    if(nn!=NULL && nn->node_type==NSP_NODE_INT){
        return nn->node_data.valueint;
    }
    return 0;    
}
double nspNodeDoubleValue(nsp_node *nn){
    if(nn!=NULL && nn->node_type==NSP_NODE_DOUBLE){
        return nn->node_data.valuedouble;
    }
    return 0;    
}
char* nspNodeStringValue(nsp_node *nn){
    if(nn!=NULL && nn->node_type==NSP_NODE_STRING){
        return nn->node_data.valuestring;    
    }
    return NULL;    
}
nsp_node* nspNodeMapByKey(nsp_node *nn,nsp_node *key){
    if(nn==NULL || nn->node_type!=NSP_NODE_MAP){return NULL;}
    valuemap *vm=nn->node_data.child;
    while(vm!=NULL){
        if(vm->key->node_type==key->node_type){
            if(key->node_type==NSP_NODE_STRING){
                if(strcmp(vm->key->node_data.valuestring,key->node_data.valuestring)==0){
                    return vm->value;
                }
            }
            else if(key->node_type==NSP_NODE_DOUBLE){
                if(vm->key->node_data.valuedouble==key->node_data.valuedouble){
                    return vm->value;
                }
            }
            else if(key->node_type==NSP_NODE_BOOL || key->node_type==NSP_NODE_INT){
                if(vm->key->node_data.valueint==key->node_data.valueint){
                    return vm->value;
                }
            }
            else{
                return NULL;    //fail to support complex key            
            }
        }
        vm=vm->next;
    }
    return NULL;
}
/**
*    nn=array("apple"=>4,"banana"=>"I like it","pear"=>"I don't like it");
*
*    nspNodeMapByStr(nn,"banana") shall return "I like it"(nsp_node)
*/
nsp_node* nspNodeMapByStr(nsp_node *nn,char *key){
    if(nn==NULL || nn->node_type!=NSP_NODE_MAP){return NULL;}
    valuemap *vm=nn->node_data.child;
    while(vm!=NULL){
        if(vm->key!=NULL && vm->key->node_type==NSP_NODE_STRING){
            if(strcmp(vm->key->node_data.valuestring,key)==0){
                return vm->value;
            }
        }
        vm=vm->next;
    }
    return NULL;
}

/**
*    nn=array("apple","banana","pear");
*
*    nspNodeMapByPos(nn,2) shall return "banana"(nsp_node)
*/
nsp_node* nspNodeMapByPos(nsp_node *nn,int array_pos){
    if(nn==NULL || nn->node_type!=NSP_NODE_MAP){return NULL;}
    valuemap *vm=nn->node_data.child;
    while(vm!=NULL){
        if(vm->key!=NULL && vm->key->node_type==NSP_NODE_INT){
            if(vm->key->node_data.valueint==array_pos){
                return vm->value;
            }
        }
        vm=vm->next;
    }
    return NULL;
}


void nspNodeDelete(nsp_node *sn){
    nsp_free_node(sn);
    sn=NULL;
}
void nspNodeClear(nsp_node *sn){
    nsp_free_node(sn);
    nsp_node *nn = nspCreateArray();
    sn=nn;
}
void nspNodePrint(nsp_node *sn){
    nsp_print_node(sn);
}

nsp_node* nspUnserialize(char buf[]){
    return nsp_unserialize(buf);
}
char* nspSerialize(nsp_node *nn){
    return nsp_serialize(nn);
}


nspAction* NSPAction(){
    nspAction *nA= (nspAction *)malloc(sizeof(nspAction));
    if(nA==NULL){return NULL;}
    //
    nA->createArray=&nspCreateArray;
    nA->createBool=&nspCreateBool;
    nA->createInt=&nspCreateInt;
    nA->createDouble=&nspCreateDouble;
    nA->createString=&nspCreateString;
    nA->createIntArray=&nspCreateIntArray;
    nA->createDoubleArray=&nspCreateDoubleArray;
    nA->createStringArray=&nspCreateStringArray;

    //
    nA->appendBool=&nspArrayAppendBool;
    nA->appendInt=&nspArrayAppendInt;
    nA->appendDouble=&nspArrayAppendDouble;
    nA->appendString=&nspArrayAppendString;
    nA->appendArray=&nspArrayAppendArray;
    nA->appendMap_Str_Str=&nspArrayAppendMapStrStr;
    nA->appendMap_Node_Node=&nspArrayAppendMap;
    nA->appendMap_Str_Int=&nspArrayAppendMapStrInt;
    nA->appendMap_Str_Node=&nspArrayAppendMapStrNode;

    //
    nA->getNodeType=&nspNodeType;
    nA->getNodeBoolValue=&nspNodeBoolValue;
    nA->getNodeIntValue=&nspNodeIntValue;
    nA->getNodeDoubleValue=&nspNodeDoubleValue;
    nA->getNodeStringValue=&nspNodeStringValue;
    nA->getArrayNodeSize=&nspArrayNodeSize;
    nA->searchNodeByPos=&nspNodeMapByPos;
    nA->searchNodeByStr=&nspNodeMapByStr;
    //
    nA->printNode=&nspNodePrint;
    nA->deleteNode=&nspNodeDelete;
    return nA;
}
bool NSPActionDelete(nspAction *nA){
    if(nA!=NULL){
        free(nA);nA=NULL;
    }
    return true;
}//

