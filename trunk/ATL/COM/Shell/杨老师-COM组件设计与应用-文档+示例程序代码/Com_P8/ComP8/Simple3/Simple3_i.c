/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Jul 19 00:28:17 2005
 */
/* Compiler settings for E:\ComP8\Simple3\Simple3.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


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

const IID IID_IMathe = {0x072EA6CA,0x7D08,0x4E7E,{0xB2,0xB7,0xB2,0xFB,0x0B,0x87,0x55,0x95}};


const IID IID_IStr = {0x072EA6CB,0x7D08,0x4E7E,{0xB2,0xB7,0xB2,0xFB,0x0B,0x87,0x55,0x95}};


const IID IID_IMathe2 = {0x072EA6CC,0x7D08,0x4E7E,{0xB2,0xB7,0xB2,0xFB,0x0B,0x87,0x55,0x95}};


const IID LIBID_SIMPLE3Lib = {0xCD7672F7,0xC0B4,0x4090,{0xA2,0xF8,0x23,0x4C,0x00,0x62,0xF4,0x2C}};


const CLSID CLSID_Mathe = {0xC6F241E2,0x43F6,0x4449,{0xA0,0x24,0xB7,0x34,0x05,0x53,0x22,0x1E}};


#ifdef __cplusplus
}
#endif

