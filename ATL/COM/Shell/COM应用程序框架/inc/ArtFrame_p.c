

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Mon Jun 13 16:59:16 2011
 */
/* Compiler settings for .\ArtFrame.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
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
#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "ArtFrame.h"

#define TYPE_FORMAT_STRING_SIZE   1201                              
#define PROC_FORMAT_STRING_SIZE   865                               
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   4            

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


extern const MIDL_SERVER_INFO IDataView_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IDataView_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO _COMFrameDispObj_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO _COMFrameDispObj_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IMDIFrame_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IMDIFrame_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IMDIChildFrame_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IMDIChildFrame_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IMDIChildFrames_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IMDIChildFrames_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need a Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure NotifyCommand */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 10 */	NdrFcShort( 0x2a ),	/* 42 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 16 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter codeNotify */

/* 24 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter cmdID */

/* 30 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bHandle */

/* 36 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
/* 38 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 40 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 42 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 44 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 46 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetWnd */

/* 48 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 50 */	NdrFcLong( 0x0 ),	/* 0 */
/* 54 */	NdrFcShort( 0x8 ),	/* 8 */
/* 56 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */
/* 60 */	NdrFcShort( 0x8 ),	/* 8 */
/* 62 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 64 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 66 */	NdrFcShort( 0x0 ),	/* 0 */
/* 68 */	NdrFcShort( 0x1 ),	/* 1 */
/* 70 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hWnd */

/* 72 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 74 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 76 */	NdrFcShort( 0x22 ),	/* Type Offset=34 */

	/* Return value */

/* 78 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 80 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 82 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnActiveWindow */

/* 84 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 86 */	NdrFcLong( 0x0 ),	/* 0 */
/* 90 */	NdrFcShort( 0x9 ),	/* 9 */
/* 92 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 94 */	NdrFcShort( 0x8 ),	/* 8 */
/* 96 */	NdrFcShort( 0x8 ),	/* 8 */
/* 98 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 100 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 102 */	NdrFcShort( 0x0 ),	/* 0 */
/* 104 */	NdrFcShort( 0x0 ),	/* 0 */
/* 106 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter nType */

/* 108 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 110 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 112 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 114 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 116 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 118 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnCloseWindow */

/* 120 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 122 */	NdrFcLong( 0x0 ),	/* 0 */
/* 126 */	NdrFcShort( 0xa ),	/* 10 */
/* 128 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 130 */	NdrFcShort( 0x1a ),	/* 26 */
/* 132 */	NdrFcShort( 0x8 ),	/* 8 */
/* 134 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 136 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 138 */	NdrFcShort( 0x0 ),	/* 0 */
/* 140 */	NdrFcShort( 0x0 ),	/* 0 */
/* 142 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter vbClose */

/* 144 */	NdrFcShort( 0x148 ),	/* Flags:  in, base type, simple ref, */
/* 146 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 148 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 150 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 152 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 154 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateWnd */

/* 156 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 158 */	NdrFcLong( 0x0 ),	/* 0 */
/* 162 */	NdrFcShort( 0xb ),	/* 11 */
/* 164 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */
/* 168 */	NdrFcShort( 0x8 ),	/* 8 */
/* 170 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 172 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 174 */	NdrFcShort( 0x0 ),	/* 0 */
/* 176 */	NdrFcShort( 0x1 ),	/* 1 */
/* 178 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hWndParent */

/* 180 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 182 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 184 */	NdrFcShort( 0x22 ),	/* Type Offset=34 */

	/* Return value */

/* 186 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 188 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 190 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_COMMDIFrame */

/* 192 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 194 */	NdrFcLong( 0x0 ),	/* 0 */
/* 198 */	NdrFcShort( 0x7 ),	/* 7 */
/* 200 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 202 */	NdrFcShort( 0x0 ),	/* 0 */
/* 204 */	NdrFcShort( 0x8 ),	/* 8 */
/* 206 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 208 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 210 */	NdrFcShort( 0x0 ),	/* 0 */
/* 212 */	NdrFcShort( 0x0 ),	/* 0 */
/* 214 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 216 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 218 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 220 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 222 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 224 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 226 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Run */

/* 228 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 230 */	NdrFcLong( 0x0 ),	/* 0 */
/* 234 */	NdrFcShort( 0x8 ),	/* 8 */
/* 236 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 238 */	NdrFcShort( 0x0 ),	/* 0 */
/* 240 */	NdrFcShort( 0x8 ),	/* 8 */
/* 242 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 244 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 246 */	NdrFcShort( 0x0 ),	/* 0 */
/* 248 */	NdrFcShort( 0x0 ),	/* 0 */
/* 250 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 252 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 254 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 256 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateWnd */

/* 258 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 260 */	NdrFcLong( 0x0 ),	/* 0 */
/* 264 */	NdrFcShort( 0x9 ),	/* 9 */
/* 266 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 268 */	NdrFcShort( 0x0 ),	/* 0 */
/* 270 */	NdrFcShort( 0x8 ),	/* 8 */
/* 272 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 274 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 276 */	NdrFcShort( 0x0 ),	/* 0 */
/* 278 */	NdrFcShort( 0x1 ),	/* 1 */
/* 280 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lpMDIStruct */

/* 282 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 284 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 286 */	NdrFcShort( 0x50 ),	/* Type Offset=80 */

	/* Return value */

/* 288 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 290 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 292 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Quit */

/* 294 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 296 */	NdrFcLong( 0x0 ),	/* 0 */
/* 300 */	NdrFcShort( 0xa ),	/* 10 */
/* 302 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 304 */	NdrFcShort( 0x0 ),	/* 0 */
/* 306 */	NdrFcShort( 0x8 ),	/* 8 */
/* 308 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 310 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 312 */	NdrFcShort( 0x0 ),	/* 0 */
/* 314 */	NdrFcShort( 0x0 ),	/* 0 */
/* 316 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 318 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 320 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 322 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ShowMe */

/* 324 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 326 */	NdrFcLong( 0x0 ),	/* 0 */
/* 330 */	NdrFcShort( 0xb ),	/* 11 */
/* 332 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 334 */	NdrFcShort( 0x6 ),	/* 6 */
/* 336 */	NdrFcShort( 0x8 ),	/* 8 */
/* 338 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 340 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 342 */	NdrFcShort( 0x0 ),	/* 0 */
/* 344 */	NdrFcShort( 0x0 ),	/* 0 */
/* 346 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter vbShow */

/* 348 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 350 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 352 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 354 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 356 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 358 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_MDIChildFrames */

/* 360 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 362 */	NdrFcLong( 0x0 ),	/* 0 */
/* 366 */	NdrFcShort( 0xc ),	/* 12 */
/* 368 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 372 */	NdrFcShort( 0x8 ),	/* 8 */
/* 374 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 376 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 378 */	NdrFcShort( 0x0 ),	/* 0 */
/* 380 */	NdrFcShort( 0x0 ),	/* 0 */
/* 382 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 384 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 386 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 388 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 390 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 392 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 394 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AboutCOMFrame */

/* 396 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 398 */	NdrFcLong( 0x0 ),	/* 0 */
/* 402 */	NdrFcShort( 0xd ),	/* 13 */
/* 404 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 406 */	NdrFcShort( 0x0 ),	/* 0 */
/* 408 */	NdrFcShort( 0x8 ),	/* 8 */
/* 410 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 412 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 414 */	NdrFcShort( 0x0 ),	/* 0 */
/* 416 */	NdrFcShort( 0x0 ),	/* 0 */
/* 418 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 420 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 422 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 424 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure MDIWindows */

/* 426 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 428 */	NdrFcLong( 0x0 ),	/* 0 */
/* 432 */	NdrFcShort( 0xe ),	/* 14 */
/* 434 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 436 */	NdrFcShort( 0x0 ),	/* 0 */
/* 438 */	NdrFcShort( 0x8 ),	/* 8 */
/* 440 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 442 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 444 */	NdrFcShort( 0x0 ),	/* 0 */
/* 446 */	NdrFcShort( 0x0 ),	/* 0 */
/* 448 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 450 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 452 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 454 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PushDataView */

/* 456 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 458 */	NdrFcLong( 0x0 ),	/* 0 */
/* 462 */	NdrFcShort( 0x8 ),	/* 8 */
/* 464 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 466 */	NdrFcShort( 0x0 ),	/* 0 */
/* 468 */	NdrFcShort( 0x8 ),	/* 8 */
/* 470 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 472 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 474 */	NdrFcShort( 0x0 ),	/* 0 */
/* 476 */	NdrFcShort( 0x0 ),	/* 0 */
/* 478 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lpDataView */

/* 480 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 482 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 484 */	NdrFcShort( 0x66 ),	/* Type Offset=102 */

	/* Return value */

/* 486 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 488 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 490 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PopDataView */

/* 492 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 494 */	NdrFcLong( 0x0 ),	/* 0 */
/* 498 */	NdrFcShort( 0x9 ),	/* 9 */
/* 500 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 502 */	NdrFcShort( 0x0 ),	/* 0 */
/* 504 */	NdrFcShort( 0x8 ),	/* 8 */
/* 506 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 508 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 510 */	NdrFcShort( 0x0 ),	/* 0 */
/* 512 */	NdrFcShort( 0x0 ),	/* 0 */
/* 514 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lpCW */

/* 516 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 518 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 520 */	NdrFcShort( 0x66 ),	/* Type Offset=102 */

	/* Return value */

/* 522 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 524 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 526 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_ActiveChildWindow */

/* 528 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 530 */	NdrFcLong( 0x0 ),	/* 0 */
/* 534 */	NdrFcShort( 0xa ),	/* 10 */
/* 536 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 538 */	NdrFcShort( 0x0 ),	/* 0 */
/* 540 */	NdrFcShort( 0x8 ),	/* 8 */
/* 542 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 544 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 546 */	NdrFcShort( 0x0 ),	/* 0 */
/* 548 */	NdrFcShort( 0x0 ),	/* 0 */
/* 550 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 552 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 554 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 556 */	NdrFcShort( 0x78 ),	/* Type Offset=120 */

	/* Return value */

/* 558 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 560 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 562 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_ActiveChildWindow */

/* 564 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 566 */	NdrFcLong( 0x0 ),	/* 0 */
/* 570 */	NdrFcShort( 0xb ),	/* 11 */
/* 572 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 574 */	NdrFcShort( 0x0 ),	/* 0 */
/* 576 */	NdrFcShort( 0x8 ),	/* 8 */
/* 578 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 580 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 582 */	NdrFcShort( 0x0 ),	/* 0 */
/* 584 */	NdrFcShort( 0x0 ),	/* 0 */
/* 586 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 588 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 590 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 592 */	NdrFcShort( 0x66 ),	/* Type Offset=102 */

	/* Return value */

/* 594 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 596 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 598 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Title */

/* 600 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 602 */	NdrFcLong( 0x0 ),	/* 0 */
/* 606 */	NdrFcShort( 0xc ),	/* 12 */
/* 608 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 610 */	NdrFcShort( 0x0 ),	/* 0 */
/* 612 */	NdrFcShort( 0x8 ),	/* 8 */
/* 614 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 616 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 618 */	NdrFcShort( 0x1 ),	/* 1 */
/* 620 */	NdrFcShort( 0x0 ),	/* 0 */
/* 622 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 624 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 626 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 628 */	NdrFcShort( 0x9a ),	/* Type Offset=154 */

	/* Return value */

/* 630 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 632 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 634 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Title */

/* 636 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 638 */	NdrFcLong( 0x0 ),	/* 0 */
/* 642 */	NdrFcShort( 0xd ),	/* 13 */
/* 644 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 646 */	NdrFcShort( 0x0 ),	/* 0 */
/* 648 */	NdrFcShort( 0x8 ),	/* 8 */
/* 650 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 652 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 654 */	NdrFcShort( 0x0 ),	/* 0 */
/* 656 */	NdrFcShort( 0x1 ),	/* 1 */
/* 658 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 660 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 662 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 664 */	NdrFcShort( 0xa8 ),	/* Type Offset=168 */

	/* Return value */

/* 666 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 668 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 670 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_CommandBars */

/* 672 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 674 */	NdrFcLong( 0x0 ),	/* 0 */
/* 678 */	NdrFcShort( 0xe ),	/* 14 */
/* 680 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 682 */	NdrFcShort( 0x0 ),	/* 0 */
/* 684 */	NdrFcShort( 0x8 ),	/* 8 */
/* 686 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 688 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 690 */	NdrFcShort( 0x0 ),	/* 0 */
/* 692 */	NdrFcShort( 0x0 ),	/* 0 */
/* 694 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 696 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 698 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 700 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 702 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 704 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 706 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Count */

/* 708 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 710 */	NdrFcLong( 0x0 ),	/* 0 */
/* 714 */	NdrFcShort( 0x8 ),	/* 8 */
/* 716 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 718 */	NdrFcShort( 0x0 ),	/* 0 */
/* 720 */	NdrFcShort( 0x24 ),	/* 36 */
/* 722 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 724 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 726 */	NdrFcShort( 0x0 ),	/* 0 */
/* 728 */	NdrFcShort( 0x0 ),	/* 0 */
/* 730 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 732 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 734 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 736 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 738 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 740 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 742 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get__NewEnum */

/* 744 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 746 */	NdrFcLong( 0x0 ),	/* 0 */
/* 750 */	NdrFcShort( 0x9 ),	/* 9 */
/* 752 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 754 */	NdrFcShort( 0x0 ),	/* 0 */
/* 756 */	NdrFcShort( 0x8 ),	/* 8 */
/* 758 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 760 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 762 */	NdrFcShort( 0x0 ),	/* 0 */
/* 764 */	NdrFcShort( 0x0 ),	/* 0 */
/* 766 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 768 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 770 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 772 */	NdrFcShort( 0xb6 ),	/* Type Offset=182 */

	/* Return value */

/* 774 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 776 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 778 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Item */

/* 780 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 782 */	NdrFcLong( 0x0 ),	/* 0 */
/* 786 */	NdrFcShort( 0xa ),	/* 10 */
/* 788 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 790 */	NdrFcShort( 0x8 ),	/* 8 */
/* 792 */	NdrFcShort( 0x8 ),	/* 8 */
/* 794 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 796 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 798 */	NdrFcShort( 0x20 ),	/* 32 */
/* 800 */	NdrFcShort( 0x0 ),	/* 0 */
/* 802 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Index */

/* 804 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 806 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 808 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pVal */

/* 810 */	NdrFcShort( 0x4113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=16 */
/* 812 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 814 */	NdrFcShort( 0x490 ),	/* Type Offset=1168 */

	/* Return value */

/* 816 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 818 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 820 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateChildFrame */

/* 822 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 824 */	NdrFcLong( 0x0 ),	/* 0 */
/* 828 */	NdrFcShort( 0xb ),	/* 11 */
/* 830 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 832 */	NdrFcShort( 0x0 ),	/* 0 */
/* 834 */	NdrFcShort( 0x8 ),	/* 8 */
/* 836 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 838 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 840 */	NdrFcShort( 0x0 ),	/* 0 */
/* 842 */	NdrFcShort( 0x0 ),	/* 0 */
/* 844 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter lpdv */

/* 846 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 848 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 850 */	NdrFcShort( 0x66 ),	/* Type Offset=102 */

	/* Parameter lpcf */

/* 852 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 854 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 856 */	NdrFcShort( 0x49a ),	/* Type Offset=1178 */

	/* Return value */

/* 858 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 860 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 862 */	0x8,		/* FC_LONG */
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
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  4 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x11, 0x0,	/* FC_RP */
/*  8 */	NdrFcShort( 0x1a ),	/* Offset= 26 (34) */
/* 10 */	
			0x12, 0x0,	/* FC_UP */
/* 12 */	NdrFcShort( 0x2 ),	/* Offset= 2 (14) */
/* 14 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x48,		/* 72 */
/* 16 */	NdrFcShort( 0x4 ),	/* 4 */
/* 18 */	NdrFcShort( 0x2 ),	/* 2 */
/* 20 */	NdrFcLong( 0x48746457 ),	/* 1215587415 */
/* 24 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 26 */	NdrFcLong( 0x52746457 ),	/* 1383359575 */
/* 30 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 32 */	NdrFcShort( 0xffff ),	/* Offset= -1 (31) */
/* 34 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 36 */	NdrFcShort( 0x0 ),	/* 0 */
/* 38 */	NdrFcShort( 0x4 ),	/* 4 */
/* 40 */	NdrFcShort( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0xffe0 ),	/* Offset= -32 (10) */
/* 44 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 46 */	NdrFcShort( 0x2 ),	/* Offset= 2 (48) */
/* 48 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 50 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 60 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 62 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 64 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 66 */	
			0x11, 0x0,	/* FC_RP */
/* 68 */	NdrFcShort( 0xc ),	/* Offset= 12 (80) */
/* 70 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 72 */	NdrFcShort( 0x1 ),	/* 1 */
/* 74 */	NdrFcShort( 0x4 ),	/* 4 */
/* 76 */	NdrFcShort( 0x0 ),	/* 0 */
/* 78 */	NdrFcShort( 0xffbc ),	/* Offset= -68 (10) */
/* 80 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 82 */	NdrFcShort( 0x14 ),	/* 20 */
/* 84 */	NdrFcShort( 0x0 ),	/* 0 */
/* 86 */	NdrFcShort( 0xc ),	/* Offset= 12 (98) */
/* 88 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 90 */	0x36,		/* FC_POINTER */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 92 */	0x0,		/* 0 */
			NdrFcShort( 0xffe9 ),	/* Offset= -23 (70) */
			0x8,		/* FC_LONG */
/* 96 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 98 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 100 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 102 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 104 */	NdrFcLong( 0xe2e319a4 ),	/* -488433244 */
/* 108 */	NdrFcShort( 0xd764 ),	/* -10396 */
/* 110 */	NdrFcShort( 0x4dc1 ),	/* 19905 */
/* 112 */	0x95,		/* 149 */
			0xcd,		/* 205 */
/* 114 */	0xc9,		/* 201 */
			0x2d,		/* 45 */
/* 116 */	0x37,		/* 55 */
			0x54,		/* 84 */
/* 118 */	0xd9,		/* 217 */
			0x5b,		/* 91 */
/* 120 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 122 */	NdrFcShort( 0xffec ),	/* Offset= -20 (102) */
/* 124 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 126 */	NdrFcShort( 0x1c ),	/* Offset= 28 (154) */
/* 128 */	
			0x13, 0x0,	/* FC_OP */
/* 130 */	NdrFcShort( 0xe ),	/* Offset= 14 (144) */
/* 132 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 134 */	NdrFcShort( 0x2 ),	/* 2 */
/* 136 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 138 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 140 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 142 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 144 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 146 */	NdrFcShort( 0x8 ),	/* 8 */
/* 148 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (132) */
/* 150 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 152 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 154 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 156 */	NdrFcShort( 0x2 ),	/* 2 */
/* 158 */	NdrFcShort( 0x4 ),	/* 4 */
/* 160 */	NdrFcShort( 0x0 ),	/* 0 */
/* 162 */	NdrFcShort( 0xffde ),	/* Offset= -34 (128) */
/* 164 */	
			0x12, 0x0,	/* FC_UP */
/* 166 */	NdrFcShort( 0xffea ),	/* Offset= -22 (144) */
/* 168 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 170 */	NdrFcShort( 0x2 ),	/* 2 */
/* 172 */	NdrFcShort( 0x4 ),	/* 4 */
/* 174 */	NdrFcShort( 0x0 ),	/* 0 */
/* 176 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (164) */
/* 178 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 180 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 182 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 184 */	NdrFcShort( 0x2 ),	/* Offset= 2 (186) */
/* 186 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 188 */	NdrFcLong( 0x0 ),	/* 0 */
/* 192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 194 */	NdrFcShort( 0x0 ),	/* 0 */
/* 196 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 198 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 200 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 202 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 204 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 206 */	NdrFcShort( 0x3c2 ),	/* Offset= 962 (1168) */
/* 208 */	
			0x13, 0x0,	/* FC_OP */
/* 210 */	NdrFcShort( 0x3aa ),	/* Offset= 938 (1148) */
/* 212 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 214 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 216 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 218 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 220 */	NdrFcShort( 0x2 ),	/* Offset= 2 (222) */
/* 222 */	NdrFcShort( 0x10 ),	/* 16 */
/* 224 */	NdrFcShort( 0x2f ),	/* 47 */
/* 226 */	NdrFcLong( 0x14 ),	/* 20 */
/* 230 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 232 */	NdrFcLong( 0x3 ),	/* 3 */
/* 236 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 238 */	NdrFcLong( 0x11 ),	/* 17 */
/* 242 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 244 */	NdrFcLong( 0x2 ),	/* 2 */
/* 248 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 250 */	NdrFcLong( 0x4 ),	/* 4 */
/* 254 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 256 */	NdrFcLong( 0x5 ),	/* 5 */
/* 260 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 262 */	NdrFcLong( 0xb ),	/* 11 */
/* 266 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 268 */	NdrFcLong( 0xa ),	/* 10 */
/* 272 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 274 */	NdrFcLong( 0x6 ),	/* 6 */
/* 278 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (510) */
/* 280 */	NdrFcLong( 0x7 ),	/* 7 */
/* 284 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 286 */	NdrFcLong( 0x8 ),	/* 8 */
/* 290 */	NdrFcShort( 0xff5e ),	/* Offset= -162 (128) */
/* 292 */	NdrFcLong( 0xd ),	/* 13 */
/* 296 */	NdrFcShort( 0xff92 ),	/* Offset= -110 (186) */
/* 298 */	NdrFcLong( 0x9 ),	/* 9 */
/* 302 */	NdrFcShort( 0xff02 ),	/* Offset= -254 (48) */
/* 304 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 308 */	NdrFcShort( 0xd0 ),	/* Offset= 208 (516) */
/* 310 */	NdrFcLong( 0x24 ),	/* 36 */
/* 314 */	NdrFcShort( 0x2f8 ),	/* Offset= 760 (1074) */
/* 316 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 320 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (1074) */
/* 322 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 326 */	NdrFcShort( 0x2f0 ),	/* Offset= 752 (1078) */
/* 328 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 332 */	NdrFcShort( 0x2ee ),	/* Offset= 750 (1082) */
/* 334 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 338 */	NdrFcShort( 0x2ec ),	/* Offset= 748 (1086) */
/* 340 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 344 */	NdrFcShort( 0x2ea ),	/* Offset= 746 (1090) */
/* 346 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 350 */	NdrFcShort( 0x2e8 ),	/* Offset= 744 (1094) */
/* 352 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 356 */	NdrFcShort( 0x2e6 ),	/* Offset= 742 (1098) */
/* 358 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 362 */	NdrFcShort( 0x2d0 ),	/* Offset= 720 (1082) */
/* 364 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 368 */	NdrFcShort( 0x2ce ),	/* Offset= 718 (1086) */
/* 370 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 374 */	NdrFcShort( 0x2d8 ),	/* Offset= 728 (1102) */
/* 376 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 380 */	NdrFcShort( 0x2ce ),	/* Offset= 718 (1098) */
/* 382 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 386 */	NdrFcShort( 0x2d0 ),	/* Offset= 720 (1106) */
/* 388 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 392 */	NdrFcShort( 0x2ce ),	/* Offset= 718 (1110) */
/* 394 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 398 */	NdrFcShort( 0x2cc ),	/* Offset= 716 (1114) */
/* 400 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 404 */	NdrFcShort( 0x2ca ),	/* Offset= 714 (1118) */
/* 406 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 410 */	NdrFcShort( 0x2c8 ),	/* Offset= 712 (1122) */
/* 412 */	NdrFcLong( 0x10 ),	/* 16 */
/* 416 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 418 */	NdrFcLong( 0x12 ),	/* 18 */
/* 422 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 424 */	NdrFcLong( 0x13 ),	/* 19 */
/* 428 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 430 */	NdrFcLong( 0x15 ),	/* 21 */
/* 434 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 436 */	NdrFcLong( 0x16 ),	/* 22 */
/* 440 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 442 */	NdrFcLong( 0x17 ),	/* 23 */
/* 446 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 448 */	NdrFcLong( 0xe ),	/* 14 */
/* 452 */	NdrFcShort( 0x2a6 ),	/* Offset= 678 (1130) */
/* 454 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 458 */	NdrFcShort( 0x2aa ),	/* Offset= 682 (1140) */
/* 460 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 464 */	NdrFcShort( 0x2a8 ),	/* Offset= 680 (1144) */
/* 466 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 470 */	NdrFcShort( 0x264 ),	/* Offset= 612 (1082) */
/* 472 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 476 */	NdrFcShort( 0x262 ),	/* Offset= 610 (1086) */
/* 478 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 482 */	NdrFcShort( 0x260 ),	/* Offset= 608 (1090) */
/* 484 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 488 */	NdrFcShort( 0x256 ),	/* Offset= 598 (1086) */
/* 490 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 494 */	NdrFcShort( 0x250 ),	/* Offset= 592 (1086) */
/* 496 */	NdrFcLong( 0x0 ),	/* 0 */
/* 500 */	NdrFcShort( 0x0 ),	/* Offset= 0 (500) */
/* 502 */	NdrFcLong( 0x1 ),	/* 1 */
/* 506 */	NdrFcShort( 0x0 ),	/* Offset= 0 (506) */
/* 508 */	NdrFcShort( 0xffff ),	/* Offset= -1 (507) */
/* 510 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 512 */	NdrFcShort( 0x8 ),	/* 8 */
/* 514 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 516 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 518 */	NdrFcShort( 0x2 ),	/* Offset= 2 (520) */
/* 520 */	
			0x13, 0x0,	/* FC_OP */
/* 522 */	NdrFcShort( 0x216 ),	/* Offset= 534 (1056) */
/* 524 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 526 */	NdrFcShort( 0x18 ),	/* 24 */
/* 528 */	NdrFcShort( 0xa ),	/* 10 */
/* 530 */	NdrFcLong( 0x8 ),	/* 8 */
/* 534 */	NdrFcShort( 0x5a ),	/* Offset= 90 (624) */
/* 536 */	NdrFcLong( 0xd ),	/* 13 */
/* 540 */	NdrFcShort( 0x7e ),	/* Offset= 126 (666) */
/* 542 */	NdrFcLong( 0x9 ),	/* 9 */
/* 546 */	NdrFcShort( 0x9e ),	/* Offset= 158 (704) */
/* 548 */	NdrFcLong( 0xc ),	/* 12 */
/* 552 */	NdrFcShort( 0xc8 ),	/* Offset= 200 (752) */
/* 554 */	NdrFcLong( 0x24 ),	/* 36 */
/* 558 */	NdrFcShort( 0x124 ),	/* Offset= 292 (850) */
/* 560 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 564 */	NdrFcShort( 0x140 ),	/* Offset= 320 (884) */
/* 566 */	NdrFcLong( 0x10 ),	/* 16 */
/* 570 */	NdrFcShort( 0x15a ),	/* Offset= 346 (916) */
/* 572 */	NdrFcLong( 0x2 ),	/* 2 */
/* 576 */	NdrFcShort( 0x174 ),	/* Offset= 372 (948) */
/* 578 */	NdrFcLong( 0x3 ),	/* 3 */
/* 582 */	NdrFcShort( 0x18e ),	/* Offset= 398 (980) */
/* 584 */	NdrFcLong( 0x14 ),	/* 20 */
/* 588 */	NdrFcShort( 0x1a8 ),	/* Offset= 424 (1012) */
/* 590 */	NdrFcShort( 0xffff ),	/* Offset= -1 (589) */
/* 592 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 594 */	NdrFcShort( 0x4 ),	/* 4 */
/* 596 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 598 */	NdrFcShort( 0x0 ),	/* 0 */
/* 600 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 602 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 604 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 606 */	NdrFcShort( 0x4 ),	/* 4 */
/* 608 */	NdrFcShort( 0x0 ),	/* 0 */
/* 610 */	NdrFcShort( 0x1 ),	/* 1 */
/* 612 */	NdrFcShort( 0x0 ),	/* 0 */
/* 614 */	NdrFcShort( 0x0 ),	/* 0 */
/* 616 */	0x13, 0x0,	/* FC_OP */
/* 618 */	NdrFcShort( 0xfe26 ),	/* Offset= -474 (144) */
/* 620 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 622 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 624 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 626 */	NdrFcShort( 0x8 ),	/* 8 */
/* 628 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 630 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 632 */	NdrFcShort( 0x4 ),	/* 4 */
/* 634 */	NdrFcShort( 0x4 ),	/* 4 */
/* 636 */	0x11, 0x0,	/* FC_RP */
/* 638 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (592) */
/* 640 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 642 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 644 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 646 */	NdrFcShort( 0x0 ),	/* 0 */
/* 648 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 650 */	NdrFcShort( 0x0 ),	/* 0 */
/* 652 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 654 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 658 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 660 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 662 */	NdrFcShort( 0xfe24 ),	/* Offset= -476 (186) */
/* 664 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 666 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 668 */	NdrFcShort( 0x8 ),	/* 8 */
/* 670 */	NdrFcShort( 0x0 ),	/* 0 */
/* 672 */	NdrFcShort( 0x6 ),	/* Offset= 6 (678) */
/* 674 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 676 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 678 */	
			0x11, 0x0,	/* FC_RP */
/* 680 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (644) */
/* 682 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 684 */	NdrFcShort( 0x0 ),	/* 0 */
/* 686 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 688 */	NdrFcShort( 0x0 ),	/* 0 */
/* 690 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 692 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 696 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 698 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 700 */	NdrFcShort( 0xfd74 ),	/* Offset= -652 (48) */
/* 702 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 704 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 706 */	NdrFcShort( 0x8 ),	/* 8 */
/* 708 */	NdrFcShort( 0x0 ),	/* 0 */
/* 710 */	NdrFcShort( 0x6 ),	/* Offset= 6 (716) */
/* 712 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 714 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 716 */	
			0x11, 0x0,	/* FC_RP */
/* 718 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (682) */
/* 720 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 722 */	NdrFcShort( 0x4 ),	/* 4 */
/* 724 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 726 */	NdrFcShort( 0x0 ),	/* 0 */
/* 728 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 730 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 732 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 734 */	NdrFcShort( 0x4 ),	/* 4 */
/* 736 */	NdrFcShort( 0x0 ),	/* 0 */
/* 738 */	NdrFcShort( 0x1 ),	/* 1 */
/* 740 */	NdrFcShort( 0x0 ),	/* 0 */
/* 742 */	NdrFcShort( 0x0 ),	/* 0 */
/* 744 */	0x13, 0x0,	/* FC_OP */
/* 746 */	NdrFcShort( 0x192 ),	/* Offset= 402 (1148) */
/* 748 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 750 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 752 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 754 */	NdrFcShort( 0x8 ),	/* 8 */
/* 756 */	NdrFcShort( 0x0 ),	/* 0 */
/* 758 */	NdrFcShort( 0x6 ),	/* Offset= 6 (764) */
/* 760 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 762 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 764 */	
			0x11, 0x0,	/* FC_RP */
/* 766 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (720) */
/* 768 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 770 */	NdrFcLong( 0x2f ),	/* 47 */
/* 774 */	NdrFcShort( 0x0 ),	/* 0 */
/* 776 */	NdrFcShort( 0x0 ),	/* 0 */
/* 778 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 780 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 782 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 784 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 786 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 788 */	NdrFcShort( 0x1 ),	/* 1 */
/* 790 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 792 */	NdrFcShort( 0x4 ),	/* 4 */
/* 794 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 796 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 798 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 800 */	NdrFcShort( 0x10 ),	/* 16 */
/* 802 */	NdrFcShort( 0x0 ),	/* 0 */
/* 804 */	NdrFcShort( 0xa ),	/* Offset= 10 (814) */
/* 806 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 808 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 810 */	NdrFcShort( 0xffd6 ),	/* Offset= -42 (768) */
/* 812 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 814 */	
			0x13, 0x0,	/* FC_OP */
/* 816 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (786) */
/* 818 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 820 */	NdrFcShort( 0x4 ),	/* 4 */
/* 822 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 824 */	NdrFcShort( 0x0 ),	/* 0 */
/* 826 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 828 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 830 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 832 */	NdrFcShort( 0x4 ),	/* 4 */
/* 834 */	NdrFcShort( 0x0 ),	/* 0 */
/* 836 */	NdrFcShort( 0x1 ),	/* 1 */
/* 838 */	NdrFcShort( 0x0 ),	/* 0 */
/* 840 */	NdrFcShort( 0x0 ),	/* 0 */
/* 842 */	0x13, 0x0,	/* FC_OP */
/* 844 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (798) */
/* 846 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 848 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 850 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 852 */	NdrFcShort( 0x8 ),	/* 8 */
/* 854 */	NdrFcShort( 0x0 ),	/* 0 */
/* 856 */	NdrFcShort( 0x6 ),	/* Offset= 6 (862) */
/* 858 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 860 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 862 */	
			0x11, 0x0,	/* FC_RP */
/* 864 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (818) */
/* 866 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 868 */	NdrFcShort( 0x8 ),	/* 8 */
/* 870 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 872 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 874 */	NdrFcShort( 0x10 ),	/* 16 */
/* 876 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 878 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 880 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (866) */
			0x5b,		/* FC_END */
/* 884 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 886 */	NdrFcShort( 0x18 ),	/* 24 */
/* 888 */	NdrFcShort( 0x0 ),	/* 0 */
/* 890 */	NdrFcShort( 0xa ),	/* Offset= 10 (900) */
/* 892 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 894 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 896 */	NdrFcShort( 0xffe8 ),	/* Offset= -24 (872) */
/* 898 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 900 */	
			0x11, 0x0,	/* FC_RP */
/* 902 */	NdrFcShort( 0xfefe ),	/* Offset= -258 (644) */
/* 904 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 906 */	NdrFcShort( 0x1 ),	/* 1 */
/* 908 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 910 */	NdrFcShort( 0x0 ),	/* 0 */
/* 912 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 914 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 916 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 918 */	NdrFcShort( 0x8 ),	/* 8 */
/* 920 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 922 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 924 */	NdrFcShort( 0x4 ),	/* 4 */
/* 926 */	NdrFcShort( 0x4 ),	/* 4 */
/* 928 */	0x13, 0x0,	/* FC_OP */
/* 930 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (904) */
/* 932 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 934 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 936 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 938 */	NdrFcShort( 0x2 ),	/* 2 */
/* 940 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 942 */	NdrFcShort( 0x0 ),	/* 0 */
/* 944 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 946 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 948 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 950 */	NdrFcShort( 0x8 ),	/* 8 */
/* 952 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 954 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 956 */	NdrFcShort( 0x4 ),	/* 4 */
/* 958 */	NdrFcShort( 0x4 ),	/* 4 */
/* 960 */	0x13, 0x0,	/* FC_OP */
/* 962 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (936) */
/* 964 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 966 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 968 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 970 */	NdrFcShort( 0x4 ),	/* 4 */
/* 972 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 974 */	NdrFcShort( 0x0 ),	/* 0 */
/* 976 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 978 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 980 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 982 */	NdrFcShort( 0x8 ),	/* 8 */
/* 984 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 986 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 988 */	NdrFcShort( 0x4 ),	/* 4 */
/* 990 */	NdrFcShort( 0x4 ),	/* 4 */
/* 992 */	0x13, 0x0,	/* FC_OP */
/* 994 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (968) */
/* 996 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 998 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1000 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 1002 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1004 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1006 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1008 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1010 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1012 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1014 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1016 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1018 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1020 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1022 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1024 */	0x13, 0x0,	/* FC_OP */
/* 1026 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (1000) */
/* 1028 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1030 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1032 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 1034 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1036 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1038 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1040 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1042 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1044 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 1046 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 1048 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1050 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1052 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1032) */
/* 1054 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1056 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1058 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1060 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1040) */
/* 1062 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1062) */
/* 1064 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1066 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1068 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1070 */	NdrFcShort( 0xfdde ),	/* Offset= -546 (524) */
/* 1072 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1074 */	
			0x13, 0x0,	/* FC_OP */
/* 1076 */	NdrFcShort( 0xfeea ),	/* Offset= -278 (798) */
/* 1078 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1080 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 1082 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1084 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 1086 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1088 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 1090 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1092 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 1094 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1096 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 1098 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1100 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 1102 */	
			0x13, 0x0,	/* FC_OP */
/* 1104 */	NdrFcShort( 0xfdae ),	/* Offset= -594 (510) */
/* 1106 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1108 */	NdrFcShort( 0xfc2c ),	/* Offset= -980 (128) */
/* 1110 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1112 */	NdrFcShort( 0xfc62 ),	/* Offset= -926 (186) */
/* 1114 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1116 */	NdrFcShort( 0xfbd4 ),	/* Offset= -1068 (48) */
/* 1118 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1120 */	NdrFcShort( 0xfda4 ),	/* Offset= -604 (516) */
/* 1122 */	
			0x13, 0x10,	/* FC_OP [pointer_deref] */
/* 1124 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1126) */
/* 1126 */	
			0x13, 0x0,	/* FC_OP */
/* 1128 */	NdrFcShort( 0x14 ),	/* Offset= 20 (1148) */
/* 1130 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 1132 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1134 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 1136 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 1138 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1140 */	
			0x13, 0x0,	/* FC_OP */
/* 1142 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1130) */
/* 1144 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 1146 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 1148 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1150 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1152 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1154 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1154) */
/* 1156 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1158 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1160 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1162 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1164 */	NdrFcShort( 0xfc48 ),	/* Offset= -952 (212) */
/* 1166 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1168 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1170 */	NdrFcShort( 0x3 ),	/* 3 */
/* 1172 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1174 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1176 */	NdrFcShort( 0xfc38 ),	/* Offset= -968 (208) */
/* 1178 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1180 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1182) */
/* 1182 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1184 */	NdrFcLong( 0xa61bb8f9 ),	/* -1508132615 */
/* 1188 */	NdrFcShort( 0x6141 ),	/* 24897 */
/* 1190 */	NdrFcShort( 0x427c ),	/* 17020 */
/* 1192 */	0xaa,		/* 170 */
			0x31,		/* 49 */
/* 1194 */	0xcc,		/* 204 */
			0x48,		/* 72 */
/* 1196 */	0x88,		/* 136 */
			0x33,		/* 51 */
/* 1198 */	0x5e,		/* 94 */
			0x51,		/* 81 */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            HWND_UserSize
            ,HWND_UserMarshal
            ,HWND_UserUnmarshal
            ,HWND_UserFree
            },
            {
            HMENU_UserSize
            ,HMENU_UserMarshal
            ,HMENU_UserUnmarshal
            ,HMENU_UserFree
            },
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };



/* Standard interface: __MIDL_itf_ArtFrame_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDataView, ver. 0.0,
   GUID={0xE2E319A4,0xD764,0x4dc1,{0x95,0xCD,0xC9,0x2D,0x37,0x54,0xD9,0x5B}} */

#pragma code_seg(".orpc")
static const unsigned short IDataView_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    48,
    84,
    120,
    156
    };

static const MIDL_STUBLESS_PROXY_INFO IDataView_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IDataView_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IDataView_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IDataView_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(12) _IDataViewProxyVtbl = 
{
    &IDataView_ProxyInfo,
    &IID_IDataView,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IDataView::NotifyCommand */ ,
    (void *) (INT_PTR) -1 /* IDataView::GetWnd */ ,
    (void *) (INT_PTR) -1 /* IDataView::OnActiveWindow */ ,
    (void *) (INT_PTR) -1 /* IDataView::OnCloseWindow */ ,
    (void *) (INT_PTR) -1 /* IDataView::CreateWnd */
};


static const PRPC_STUB_FUNCTION IDataView_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IDataViewStubVtbl =
{
    &IID_IDataView,
    &IDataView_ServerInfo,
    12,
    &IDataView_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: _COMFrameDispObj, ver. 0.0,
   GUID={0x3B8C7933,0x4F37,0x4d83,{0x91,0x07,0xCF,0x68,0x58,0x61,0x55,0xE2}} */

#pragma code_seg(".orpc")
static const unsigned short _COMFrameDispObj_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    192
    };

static const MIDL_STUBLESS_PROXY_INFO _COMFrameDispObj_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &_COMFrameDispObj_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO _COMFrameDispObj_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &_COMFrameDispObj_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(8) __COMFrameDispObjProxyVtbl = 
{
    &_COMFrameDispObj_ProxyInfo,
    &IID__COMFrameDispObj,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* _COMFrameDispObj::get_COMMDIFrame */
};


static const PRPC_STUB_FUNCTION _COMFrameDispObj_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl __COMFrameDispObjStubVtbl =
{
    &IID__COMFrameDispObj,
    &_COMFrameDispObj_ServerInfo,
    8,
    &_COMFrameDispObj_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_ArtFrame_0258, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IMDIFrame, ver. 0.0,
   GUID={0xD3137FF6,0x206B,0x4767,{0xA9,0x05,0x5D,0x25,0x2B,0x3D,0x93,0x25}} */

#pragma code_seg(".orpc")
static const unsigned short IMDIFrame_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    192,
    228,
    258,
    294,
    324,
    360,
    396,
    426
    };

static const MIDL_STUBLESS_PROXY_INFO IMDIFrame_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IMDIFrame_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IMDIFrame_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IMDIFrame_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(15) _IMDIFrameProxyVtbl = 
{
    &IMDIFrame_ProxyInfo,
    &IID_IMDIFrame,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* _COMFrameDispObj::get_COMMDIFrame */ ,
    (void *) (INT_PTR) -1 /* IMDIFrame::Run */ ,
    (void *) (INT_PTR) -1 /* IMDIFrame::CreateWnd */ ,
    (void *) (INT_PTR) -1 /* IMDIFrame::Quit */ ,
    (void *) (INT_PTR) -1 /* IMDIFrame::ShowMe */ ,
    (void *) (INT_PTR) -1 /* IMDIFrame::get_MDIChildFrames */ ,
    (void *) (INT_PTR) -1 /* IMDIFrame::AboutCOMFrame */ ,
    (void *) (INT_PTR) -1 /* IMDIFrame::MDIWindows */
};


static const PRPC_STUB_FUNCTION IMDIFrame_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IMDIFrameStubVtbl =
{
    &IID_IMDIFrame,
    &IMDIFrame_ServerInfo,
    15,
    &IMDIFrame_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IMDIChildFrame, ver. 0.0,
   GUID={0xA61BB8F9,0x6141,0x427C,{0xAA,0x31,0xCC,0x48,0x88,0x33,0x5E,0x51}} */

#pragma code_seg(".orpc")
static const unsigned short IMDIChildFrame_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    192,
    456,
    492,
    528,
    564,
    600,
    636,
    672
    };

static const MIDL_STUBLESS_PROXY_INFO IMDIChildFrame_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IMDIChildFrame_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IMDIChildFrame_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IMDIChildFrame_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(15) _IMDIChildFrameProxyVtbl = 
{
    &IMDIChildFrame_ProxyInfo,
    &IID_IMDIChildFrame,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* _COMFrameDispObj::get_COMMDIFrame */ ,
    (void *) (INT_PTR) -1 /* IMDIChildFrame::PushDataView */ ,
    (void *) (INT_PTR) -1 /* IMDIChildFrame::PopDataView */ ,
    (void *) (INT_PTR) -1 /* IMDIChildFrame::get_ActiveChildWindow */ ,
    (void *) (INT_PTR) -1 /* IMDIChildFrame::put_ActiveChildWindow */ ,
    (void *) (INT_PTR) -1 /* IMDIChildFrame::get_Title */ ,
    (void *) (INT_PTR) -1 /* IMDIChildFrame::put_Title */ ,
    (void *) (INT_PTR) -1 /* IMDIChildFrame::get_CommandBars */
};


static const PRPC_STUB_FUNCTION IMDIChildFrame_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IMDIChildFrameStubVtbl =
{
    &IID_IMDIChildFrame,
    &IMDIChildFrame_ServerInfo,
    15,
    &IMDIChildFrame_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IMDIChildFrames, ver. 0.0,
   GUID={0xDBDC4265,0x324B,0x4E01,{0x9B,0xBE,0x8A,0xB7,0x6F,0xBB,0x8C,0xF6}} */

#pragma code_seg(".orpc")
static const unsigned short IMDIChildFrames_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    192,
    708,
    744,
    780,
    822
    };

static const MIDL_STUBLESS_PROXY_INFO IMDIChildFrames_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IMDIChildFrames_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IMDIChildFrames_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IMDIChildFrames_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(12) _IMDIChildFramesProxyVtbl = 
{
    &IMDIChildFrames_ProxyInfo,
    &IID_IMDIChildFrames,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* _COMFrameDispObj::get_COMMDIFrame */ ,
    (void *) (INT_PTR) -1 /* IMDIChildFrames::get_Count */ ,
    (void *) (INT_PTR) -1 /* IMDIChildFrames::get__NewEnum */ ,
    (void *) (INT_PTR) -1 /* IMDIChildFrames::get_Item */ ,
    (void *) (INT_PTR) -1 /* IMDIChildFrames::CreateChildFrame */
};


static const PRPC_STUB_FUNCTION IMDIChildFrames_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IMDIChildFramesStubVtbl =
{
    &IID_IMDIChildFrames,
    &IMDIChildFrames_ServerInfo,
    12,
    &IMDIChildFrames_table[-3],
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
    0x50002, /* Ndr library version */
    0,
    0x6000169, /* MIDL Version 6.0.361 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
    };

const CInterfaceProxyVtbl * _ArtFrame_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &__COMFrameDispObjProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IMDIChildFramesProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IDataViewProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IMDIFrameProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IMDIChildFrameProxyVtbl,
    0
};

const CInterfaceStubVtbl * _ArtFrame_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &__COMFrameDispObjStubVtbl,
    ( CInterfaceStubVtbl *) &_IMDIChildFramesStubVtbl,
    ( CInterfaceStubVtbl *) &_IDataViewStubVtbl,
    ( CInterfaceStubVtbl *) &_IMDIFrameStubVtbl,
    ( CInterfaceStubVtbl *) &_IMDIChildFrameStubVtbl,
    0
};

PCInterfaceName const _ArtFrame_InterfaceNamesList[] = 
{
    "_COMFrameDispObj",
    "IMDIChildFrames",
    "IDataView",
    "IMDIFrame",
    "IMDIChildFrame",
    0
};

const IID *  _ArtFrame_BaseIIDList[] = 
{
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _ArtFrame_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _ArtFrame, pIID, n)

int __stdcall _ArtFrame_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _ArtFrame, 5, 4 )
    IID_BS_LOOKUP_NEXT_TEST( _ArtFrame, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _ArtFrame, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _ArtFrame, 5, *pIndex )
    
}

const ExtendedProxyFileInfo ArtFrame_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _ArtFrame_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _ArtFrame_StubVtblList,
    (const PCInterfaceName * ) & _ArtFrame_InterfaceNamesList,
    (const IID ** ) & _ArtFrame_BaseIIDList,
    & _ArtFrame_IID_Lookup, 
    5,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

