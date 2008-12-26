/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Nov 29 23:01:59 2005
 */
/* Compiler settings for C:\SAVE\REBUS\SRC\scc\rebusscc.idl:
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

const IID IID_ISourceCodeControl = {0x4014EB55,0x4A22,0x425F,{0x86,0xF7,0x16,0x3F,0xE6,0xFC,0xA7,0x55}};


const IID LIBID_REBUSSCCLib = {0xA12BE2EB,0x7530,0x40D3,{0xB1,0x37,0x99,0x90,0xD3,0x18,0x49,0x00}};


const IID DIID__ISourceCodeControlEvents = {0x167E69E4,0xB507,0x4351,{0xBD,0x3B,0xF8,0xCE,0x54,0x20,0x64,0xC8}};


const CLSID CLSID_SourceCodeControl = {0xEAFDAA93,0x9A65,0x4267,{0xB8,0xFE,0xC3,0x1A,0x06,0x1B,0x3C,0xCB}};


#ifdef __cplusplus
}
#endif

