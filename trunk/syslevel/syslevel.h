/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Nov 20 17:34:59 2003
 */
/* Compiler settings for E:\My Projects\IPM\syslevel\syslevel.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __syslevel_h__
#define __syslevel_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __Iprocess_FWD_DEFINED__
#define __Iprocess_FWD_DEFINED__
typedef interface Iprocess Iprocess;
#endif 	/* __Iprocess_FWD_DEFINED__ */


#ifndef __Isystem_FWD_DEFINED__
#define __Isystem_FWD_DEFINED__
typedef interface Isystem Isystem;
#endif 	/* __Isystem_FWD_DEFINED__ */


#ifndef __process_FWD_DEFINED__
#define __process_FWD_DEFINED__

#ifdef __cplusplus
typedef class process process;
#else
typedef struct process process;
#endif /* __cplusplus */

#endif 	/* __process_FWD_DEFINED__ */


#ifndef __system_FWD_DEFINED__
#define __system_FWD_DEFINED__

#ifdef __cplusplus
typedef class system system1;
#else
typedef struct system system1;
#endif /* __cplusplus */

#endif 	/* __system_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __Iprocess_INTERFACE_DEFINED__
#define __Iprocess_INTERFACE_DEFINED__

/* interface Iprocess */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_Iprocess;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E0306EDB-A7FE-4D73-BE55-982D1015EF1F")
    Iprocess : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get( 
            /* [in] */ BSTR name,
            /* [retval][out] */ LONG __RPC_FAR *dwProcessId) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE kill( 
            /* [in] */ LONG dwProcessId,
            /* [retval][out] */ LONG __RPC_FAR *result) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getCurrentProcessId( 
            /* [retval][out] */ LONG __RPC_FAR *pPID) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE loadUserProfile( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE unloadUserProfile( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE isProcessAlive( 
            /* [in] */ LONG dwProcessId,
            /* [retval][out] */ BOOL __RPC_FAR *bResult) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE setProcessPriority( 
            /* [in] */ LONG lPriority) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IprocessVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            Iprocess __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            Iprocess __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            Iprocess __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            Iprocess __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            Iprocess __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            Iprocess __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            Iprocess __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get )( 
            Iprocess __RPC_FAR * This,
            /* [in] */ BSTR name,
            /* [retval][out] */ LONG __RPC_FAR *dwProcessId);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *kill )( 
            Iprocess __RPC_FAR * This,
            /* [in] */ LONG dwProcessId,
            /* [retval][out] */ LONG __RPC_FAR *result);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getCurrentProcessId )( 
            Iprocess __RPC_FAR * This,
            /* [retval][out] */ LONG __RPC_FAR *pPID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *loadUserProfile )( 
            Iprocess __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *unloadUserProfile )( 
            Iprocess __RPC_FAR * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *isProcessAlive )( 
            Iprocess __RPC_FAR * This,
            /* [in] */ LONG dwProcessId,
            /* [retval][out] */ BOOL __RPC_FAR *bResult);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *setProcessPriority )( 
            Iprocess __RPC_FAR * This,
            /* [in] */ LONG lPriority);
        
        END_INTERFACE
    } IprocessVtbl;

    interface Iprocess
    {
        CONST_VTBL struct IprocessVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Iprocess_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define Iprocess_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define Iprocess_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define Iprocess_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define Iprocess_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define Iprocess_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define Iprocess_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define Iprocess_get(This,name,dwProcessId)	\
    (This)->lpVtbl -> get(This,name,dwProcessId)

#define Iprocess_kill(This,dwProcessId,result)	\
    (This)->lpVtbl -> kill(This,dwProcessId,result)

#define Iprocess_getCurrentProcessId(This,pPID)	\
    (This)->lpVtbl -> getCurrentProcessId(This,pPID)

#define Iprocess_loadUserProfile(This)	\
    (This)->lpVtbl -> loadUserProfile(This)

#define Iprocess_unloadUserProfile(This)	\
    (This)->lpVtbl -> unloadUserProfile(This)

#define Iprocess_isProcessAlive(This,dwProcessId,bResult)	\
    (This)->lpVtbl -> isProcessAlive(This,dwProcessId,bResult)

#define Iprocess_setProcessPriority(This,lPriority)	\
    (This)->lpVtbl -> setProcessPriority(This,lPriority)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Iprocess_get_Proxy( 
    Iprocess __RPC_FAR * This,
    /* [in] */ BSTR name,
    /* [retval][out] */ LONG __RPC_FAR *dwProcessId);


void __RPC_STUB Iprocess_get_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Iprocess_kill_Proxy( 
    Iprocess __RPC_FAR * This,
    /* [in] */ LONG dwProcessId,
    /* [retval][out] */ LONG __RPC_FAR *result);


void __RPC_STUB Iprocess_kill_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Iprocess_getCurrentProcessId_Proxy( 
    Iprocess __RPC_FAR * This,
    /* [retval][out] */ LONG __RPC_FAR *pPID);


void __RPC_STUB Iprocess_getCurrentProcessId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Iprocess_loadUserProfile_Proxy( 
    Iprocess __RPC_FAR * This);


void __RPC_STUB Iprocess_loadUserProfile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Iprocess_unloadUserProfile_Proxy( 
    Iprocess __RPC_FAR * This);


void __RPC_STUB Iprocess_unloadUserProfile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Iprocess_isProcessAlive_Proxy( 
    Iprocess __RPC_FAR * This,
    /* [in] */ LONG dwProcessId,
    /* [retval][out] */ BOOL __RPC_FAR *bResult);


void __RPC_STUB Iprocess_isProcessAlive_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Iprocess_setProcessPriority_Proxy( 
    Iprocess __RPC_FAR * This,
    /* [in] */ LONG lPriority);


void __RPC_STUB Iprocess_setProcessPriority_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __Iprocess_INTERFACE_DEFINED__ */


#ifndef __Isystem_INTERFACE_DEFINED__
#define __Isystem_INTERFACE_DEFINED__

/* interface Isystem */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_Isystem;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("97BB2E94-AF08-4AF9-8364-647DEF7297A2")
    Isystem : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getComputerName( 
            /* [retval][out] */ BSTR __RPC_FAR *pName) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IsystemVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            Isystem __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            Isystem __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            Isystem __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            Isystem __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            Isystem __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            Isystem __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            Isystem __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getComputerName )( 
            Isystem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pName);
        
        END_INTERFACE
    } IsystemVtbl;

    interface Isystem
    {
        CONST_VTBL struct IsystemVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Isystem_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define Isystem_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define Isystem_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define Isystem_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define Isystem_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define Isystem_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define Isystem_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define Isystem_getComputerName(This,pName)	\
    (This)->lpVtbl -> getComputerName(This,pName)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Isystem_getComputerName_Proxy( 
    Isystem __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pName);


void __RPC_STUB Isystem_getComputerName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __Isystem_INTERFACE_DEFINED__ */



#ifndef __SYSLEVELLib_LIBRARY_DEFINED__
#define __SYSLEVELLib_LIBRARY_DEFINED__

/* library SYSLEVELLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_SYSLEVELLib;

EXTERN_C const CLSID CLSID_process;

#ifdef __cplusplus

class DECLSPEC_UUID("5849BFBD-2B01-4CD0-AE0E-598881B9D592")
process;
#endif

EXTERN_C const CLSID CLSID_system;

#ifdef __cplusplus

class DECLSPEC_UUID("E1AB17F5-2C65-4EAD-887B-B801B93857DB")
system;
#endif
#endif /* __SYSLEVELLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
