

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Mon Dec 10 18:17:04 2012
 */
/* Compiler settings for .\Section4Demo1.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */
#pragma warning( disable: 4211 )  /* redefine extent to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "Section4Demo1.h"

#define TYPE_FORMAT_STRING_SIZE   59                                
#define PROC_FORMAT_STRING_SIZE   97                                
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   1            

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IHelloSimpleObject_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IHelloSimpleObject_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
#error You need a Windows NT 4.0 or later to run this stub because it uses these features:
#error   -Oif or -Oicf, [wire_marshal] or [user_marshal] attribute.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure SumLong */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 10 */	NdrFcShort( 0x10 ),	/* 16 */
/* 12 */	NdrFcShort( 0x24 ),	/* 36 */
/* 14 */	0x4,		/* Oi2 Flags:  has return, */
			0x4,		/* 4 */

	/* Parameter lA */

/* 16 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 18 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 20 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter lB */

/* 22 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 24 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 26 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter plSum */

/* 28 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 30 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 32 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 34 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 36 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 38 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Description */

/* 40 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 42 */	NdrFcLong( 0x0 ),	/* 0 */
/* 46 */	NdrFcShort( 0x8 ),	/* 8 */
/* 48 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 50 */	NdrFcShort( 0x0 ),	/* 0 */
/* 52 */	NdrFcShort( 0x8 ),	/* 8 */
/* 54 */	0x5,		/* Oi2 Flags:  srv must size, has return, */
			0x2,		/* 2 */

	/* Parameter pVal */

/* 56 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 58 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 60 */	NdrFcShort( 0x22 ),	/* Type Offset=34 */

	/* Return value */

/* 62 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 64 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 66 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Description */

/* 68 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 70 */	NdrFcLong( 0x0 ),	/* 0 */
/* 74 */	NdrFcShort( 0x9 ),	/* 9 */
/* 76 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 78 */	NdrFcShort( 0x0 ),	/* 0 */
/* 80 */	NdrFcShort( 0x8 ),	/* 8 */
/* 82 */	0x6,		/* Oi2 Flags:  clt must size, has return, */
			0x2,		/* 2 */

	/* Parameter newVal */

/* 84 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 86 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 88 */	NdrFcShort( 0x30 ),	/* Type Offset=48 */

	/* Return value */

/* 90 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 92 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 94 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  4 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/*  8 */	NdrFcShort( 0x1a ),	/* Offset= 26 (34) */
/* 10 */	
			0x13, 0x0,	/* FC_OP */
/* 12 */	NdrFcShort( 0xc ),	/* Offset= 12 (24) */
/* 14 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 16 */	NdrFcShort( 0x2 ),	/* 2 */
/* 18 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 20 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 22 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 24 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 26 */	NdrFcShort( 0x8 ),	/* 8 */
/* 28 */	NdrFcShort( 0xfff2 ),	/* Offset= -14 (14) */
/* 30 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 32 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 34 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 36 */	NdrFcShort( 0x0 ),	/* 0 */
/* 38 */	NdrFcShort( 0x4 ),	/* 4 */
/* 40 */	NdrFcShort( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0xffe0 ),	/* Offset= -32 (10) */
/* 44 */	
			0x12, 0x0,	/* FC_UP */
/* 46 */	NdrFcShort( 0xffea ),	/* Offset= -22 (24) */
/* 48 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 50 */	NdrFcShort( 0x0 ),	/* 0 */
/* 52 */	NdrFcShort( 0x4 ),	/* 4 */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (44) */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            }

        };



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IHelloSimpleObject, ver. 0.0,
   GUID={0xAD3ADBBB,0x002E,0x4AB0,{0xA2,0x3B,0x98,0x6A,0x63,0x75,0x85,0xCB}} */

#pragma code_seg(".orpc")
static const unsigned short IHelloSimpleObject_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    40,
    68
    };

static const MIDL_STUBLESS_PROXY_INFO IHelloSimpleObject_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IHelloSimpleObject_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IHelloSimpleObject_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IHelloSimpleObject_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) _IHelloSimpleObjectProxyVtbl = 
{
    &IHelloSimpleObject_ProxyInfo,
    &IID_IHelloSimpleObject,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IHelloSimpleObject::SumLong */ ,
    (void *) (INT_PTR) -1 /* IHelloSimpleObject::get_Description */ ,
    (void *) (INT_PTR) -1 /* IHelloSimpleObject::put_Description */
};


static const PRPC_STUB_FUNCTION IHelloSimpleObject_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IHelloSimpleObjectStubVtbl =
{
    &IID_IHelloSimpleObject,
    &IHelloSimpleObject_ServerInfo,
    10,
    &IHelloSimpleObject_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x600016e, /* MIDL Version 6.0.366 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
    };

const CInterfaceProxyVtbl * _Section4Demo1_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IHelloSimpleObjectProxyVtbl,
    0
};

const CInterfaceStubVtbl * _Section4Demo1_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IHelloSimpleObjectStubVtbl,
    0
};

PCInterfaceName const _Section4Demo1_InterfaceNamesList[] = 
{
    "IHelloSimpleObject",
    0
};

const IID *  _Section4Demo1_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _Section4Demo1_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _Section4Demo1, pIID, n)

int __stdcall _Section4Demo1_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_Section4Demo1_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo Section4Demo1_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _Section4Demo1_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _Section4Demo1_StubVtblList,
    (const PCInterfaceName * ) & _Section4Demo1_InterfaceNamesList,
    (const IID ** ) & _Section4Demo1_BaseIIDList,
    & _Section4Demo1_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

