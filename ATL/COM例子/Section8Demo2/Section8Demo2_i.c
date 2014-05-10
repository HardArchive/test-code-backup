

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Tue Dec 11 10:01:44 2012
 */
/* Compiler settings for .\Section8Demo2.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_ILoginAx,0x79A3FF78,0x2A01,0x401F,0x93,0xE6,0x65,0x8B,0x74,0xC9,0x88,0xE4);


MIDL_DEFINE_GUID(IID, LIBID_Section8Demo2Lib,0x46FF9709,0x26F1,0x4582,0x9F,0x0C,0xA0,0x03,0xB5,0xCD,0x27,0x74);


MIDL_DEFINE_GUID(IID, DIID__ILoginAxEvents,0xCB61249D,0xBF62,0x4101,0xB5,0xBF,0xD9,0xFB,0x59,0x4F,0x2A,0xC8);


MIDL_DEFINE_GUID(CLSID, CLSID_LoginAx,0x9B90E487,0xAD06,0x48EF,0x90,0x64,0x73,0xDE,0x57,0x52,0xB3,0x98);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



