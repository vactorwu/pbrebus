/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Nov 28 23:08:20 2005
 */
/* Compiler settings for C:\save\rebus\src\pbcompiler\pbcompiler.idl:
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

const IID IID_IPBCompiler = {0x63DD36AF,0x75F1,0x49DE,{0x82,0x77,0x36,0x17,0xC4,0x2C,0xD2,0x6A}};


const IID LIBID_PBCOMPILERLib = {0xB39E12B5,0xDEB4,0x43a4,{0xAF,0x5B,0x1E,0xD2,0x53,0x7E,0xE6,0x82}};


const IID DIID__IcompilerEvents = {0xCF0E19AB,0xF60D,0x43E0,{0xBF,0x60,0x2D,0x48,0x4D,0xE3,0x42,0x01}};


const CLSID CLSID_compiler = {0x77765AE8,0x6757,0x4ad0,{0x87,0x41,0xFC,0xC9,0x9D,0xE0,0xE0,0xA4}};


#ifdef __cplusplus
}
#endif

