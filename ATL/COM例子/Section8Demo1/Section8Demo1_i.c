

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Tue Dec 11 09:22:57 2012
 */
/* Compiler settings for .\Section8Demo1.idl:
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

MIDL_DEFINE_GUID(IID, IID_IPasswordSpObj,0xA8E69F07,0x3692,0x4DC9,0x87,0x5D,0x9D,0xE4,0x6E,0xC9,0x0D,0xB9);


MIDL_DEFINE_GUID(IID, LIBID_Section8Demo1Lib,0x3BB25B73,0xB767,0x4C88,0x82,0x96,0x5C,0xF2,0x66,0x48,0xF7,0xBF);


MIDL_DEFINE_GUID(IID, DIID__IPasswordSpObjEvents,0x9ECC6F13,0x726F,0x4BE8,0xA9,0x61,0xD6,0x14,0x65,0x36,0x73,0x3D);


MIDL_DEFINE_GUID(CLSID, CLSID_PasswordSpObj,0x08D3A036,0xB259,0x4201,0x9B,0x02,0x0A,0x65,0x3D,0xB1,0xF4,0xC0);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



