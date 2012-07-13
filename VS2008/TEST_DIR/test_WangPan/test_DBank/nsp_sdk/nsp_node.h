#ifndef __NSP_NODE_H
#define __NSP_NODE_H

typedef enum{
    NSP_NODE_NULL,
    NSP_NODE_BOOL,
    NSP_NODE_INT,
    NSP_NODE_DOUBLE,
    NSP_NODE_STRING,
    NSP_NODE_ARRAY,
    NSP_NODE_MAP
}node_type;

//map
typedef struct valuemap
{
    struct nsp_node *key;
    struct nsp_node *value;
    struct valuemap *next;
}valuemap;

typedef struct nsp_node{
    int node_type;
    int countmap;
    union{
        int valueint;
        double valuedouble;
        char* valuestring;
        valuemap *child;
    }node_data;
}nsp_node;


//several actions for control nsp_node 
typedef struct nspAction{
    //
    nsp_node* (*createBool)(bool b);
    nsp_node* (*createInt)(int i);
    nsp_node* (*createDouble)(double d);
    nsp_node* (*createString)(char *str);
    //
    nsp_node* (*createArray)();    //empty array
    nsp_node* (*createIntArray)(int *numbers,int count);
    nsp_node* (*createDoubleArray)(double *numbers,int count);
    nsp_node* (*createStringArray)(int count,...);
    
    //make sure nn is an array
    bool (*appendBool)(nsp_node *nn,bool b);
    bool (*appendInt)(nsp_node *nn,int i);
    bool (*appendDouble)(nsp_node *nn,double d);
    bool (*appendString)(nsp_node *nn,char *str);
    bool (*appendArray)(nsp_node *nn,nsp_node *arr);
    bool (*appendMap_Node_Node)(nsp_node *nn,nsp_node *key,nsp_node *val);
    bool (*appendMap_Str_Node)(nsp_node *nn,char *key,nsp_node *val);
    bool (*appendMap_Str_Str)(nsp_node *nn,char *key,char *val);
    bool (*appendMap_Str_Int)(nsp_node *nn,char *key,int val);
    
    //get nsp_node info or child_node
    int (*getNodeType)(nsp_node *nn);
    bool (*getNodeBoolValue)(nsp_node *nn);
    int (*getNodeIntValue)(nsp_node *nn);
    double (*getNodeDoubleValue)(nsp_node *nn);
    char* (*getNodeStringValue)(nsp_node *nn);
    int (*getArrayNodeSize)(nsp_node *nn);
    nsp_node* (*searchNodeByStr)(nsp_node *nn,char *str);
    nsp_node* (*searchNodeByPos)(nsp_node *nn,int array_pos);
    
    //
    void (*deleteNode)(nsp_node *sn);
    void (*printNode)(nsp_node *sn);

}nspAction;

nspAction* NSPAction();
bool NSPActionDelete(nspAction *nA);
void nspNodeDelete(nsp_node *sn);

#endif//

