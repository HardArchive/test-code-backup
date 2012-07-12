#ifndef __NSPCLIENT_H__
#define __NSPCLIENT_H__

#include"md5.h"
#include"nsp_node.h"
#include"nsp_global.h"
#include"nsp_service.h"

NSP_EXPORT_DLL nspClient* NSPClient(char *str1,char *str2);
NSP_EXPORT_DLL bool NSPClientDelete(nspClient *nC);

NSP_EXPORT_DLL bool FreeResult(Result *);
NSP_EXPORT_DLL bool FreeLsResult(LsResult *);
NSP_EXPORT_DLL bool FreeMap(map<string,string> *);

#endif//

