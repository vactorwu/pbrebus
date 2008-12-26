/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Nov 29 23:01:59 2005
 */
/* Compiler settings for C:\SAVE\REBUS\SRC\scc\rebusscc.idl:
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

#ifndef __rebusscc_h__
#define __rebusscc_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ISourceCodeControl_FWD_DEFINED__
#define __ISourceCodeControl_FWD_DEFINED__
typedef interface ISourceCodeControl ISourceCodeControl;
#endif 	/* __ISourceCodeControl_FWD_DEFINED__ */


#ifndef ___ISourceCodeControlEvents_FWD_DEFINED__
#define ___ISourceCodeControlEvents_FWD_DEFINED__
typedef interface _ISourceCodeControlEvents _ISourceCodeControlEvents;
#endif 	/* ___ISourceCodeControlEvents_FWD_DEFINED__ */


#ifndef __SourceCodeControl_FWD_DEFINED__
#define __SourceCodeControl_FWD_DEFINED__

#ifdef __cplusplus
typedef class SourceCodeControl SourceCodeControl;
#else
typedef struct SourceCodeControl SourceCodeControl;
#endif /* __cplusplus */

#endif 	/* __SourceCodeControl_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __ISourceCodeControl_INTERFACE_DEFINED__
#define __ISourceCodeControl_INTERFACE_DEFINED__

/* interface ISourceCodeControl */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISourceCodeControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4014EB55-4A22-425F-86F7-163FE6FCA755")
    ISourceCodeControl : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getVendors( 
            /* [retval][out] */ BSTR __RPC_FAR *szVendorList) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE connect( 
            /* [in] */ BSTR szVendor,
            /* [in] */ BSTR szUser,
            /* [in] */ BSTR szPassword) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE getFiles( 
            /* [in] */ BSTR szProject,
            /* [in] */ BSTR szLocal,
            /* [in] */ BSTR szMask) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISourceCodeControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISourceCodeControl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISourceCodeControl __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISourceCodeControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ISourceCodeControl __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ISourceCodeControl __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ISourceCodeControl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ISourceCodeControl __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getVendors )( 
            ISourceCodeControl __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *szVendorList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *connect )( 
            ISourceCodeControl __RPC_FAR * This,
            /* [in] */ BSTR szVendor,
            /* [in] */ BSTR szUser,
            /* [in] */ BSTR szPassword);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *getFiles )( 
            ISourceCodeControl __RPC_FAR * This,
            /* [in] */ BSTR szProject,
            /* [in] */ BSTR szLocal,
            /* [in] */ BSTR szMask);
        
        END_INTERFACE
    } ISourceCodeControlVtbl;

    interface ISourceCodeControl
    {
        CONST_VTBL struct ISourceCodeControlVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISourceCodeControl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISourceCodeControl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISourceCodeControl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISourceCodeControl_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISourceCodeControl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISourceCodeControl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISourceCodeControl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISourceCodeControl_getVendors(This,szVendorList)	\
    (This)->lpVtbl -> getVendors(This,szVendorList)

#define ISourceCodeControl_connect(This,szVendor,szUser,szPassword)	\
    (This)->lpVtbl -> connect(This,szVendor,szUser,szPassword)

#define ISourceCodeControl_getFiles(This,szProject,szLocal,szMask)	\
    (This)->lpVtbl -> getFiles(This,szProject,szLocal,szMask)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISourceCodeControl_getVendors_Proxy( 
    ISourceCodeControl __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *szVendorList);


void __RPC_STUB ISourceCodeControl_getVendors_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISourceCodeControl_connect_Proxy( 
    ISourceCodeControl __RPC_FAR * This,
    /* [in] */ BSTR szVendor,
    /* [in] */ BSTR szUser,
    /* [in] */ BSTR szPassword);


void __RPC_STUB ISourceCodeControl_connect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISourceCodeControl_getFiles_Proxy( 
    ISourceCodeControl __RPC_FAR * This,
    /* [in] */ BSTR szProject,
    /* [in] */ BSTR szLocal,
    /* [in] */ BSTR szMask);


void __RPC_STUB ISourceCodeControl_getFiles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISourceCodeControl_INTERFACE_DEFINED__ */



#ifndef __REBUSSCCLib_LIBRARY_DEFINED__
#define __REBUSSCCLib_LIBRARY_DEFINED__

/* library REBUSSCCLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_REBUSSCCLib;

#ifndef ___ISourceCodeControlEvents_DISPINTERFACE_DEFINED__
#define ___ISourceCodeControlEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ISourceCodeControlEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__ISourceCodeControlEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("167E69E4-B507-4351-BD3B-F8CE542064C8")
    _ISourceCodeControlEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _ISourceCodeControlEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            _ISourceCodeControlEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            _ISourceCodeControlEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            _ISourceCodeControlEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            _ISourceCodeControlEvents __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            _ISourceCodeControlEvents __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            _ISourceCodeControlEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            _ISourceCodeControlEvents __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } _ISourceCodeControlEventsVtbl;

    interface _ISourceCodeControlEvents
    {
        CONST_VTBL struct _ISourceCodeControlEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _ISourceCodeControlEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _ISourceCodeControlEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _ISourceCodeControlEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _ISourceCodeControlEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _ISourceCodeControlEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _ISourceCodeControlEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _ISourceCodeControlEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___ISourceCodeControlEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_SourceCodeControl;

#ifdef __cplusplus

class DECLSPEC_UUID("EAFDAA93-9A65-4267-B8FE-C31A061B3CCB")
SourceCodeControl;
#endif
#endif /* __REBUSSCCLib_LIBRARY_DEFINED__ */

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
