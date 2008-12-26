// dynamemlist.cpp
// Need to be able to map DISPID->Name
// Need to be able to map DISPID->Method/Property
// Need to be able to get next DISPID based on a DISPID
// Need to be able to remove method/property based on DISPID
// Need to be able to remove method/property based on Name
// Why not just use a vector of methods? Removing a DISPID
// doesn't really remove it anyway, i.e. GetNextDispID must
// still work, the DISPID can't be reused and if a property
// of the same name is added, it must have the same DISPID.

#include "stdafx.h"
#include "dynamemlist.h"
#include <dispex.h>
#include <algorithm>

#ifndef HR
#define HR(_ex) { HRESULT _hr = _ex; if(FAILED(_hr)) return _hr; }
#endif

#ifndef SafeBSTR
#define SafeBSTR(_bstr) (_bstr ? _bstr : OLESTR(""))
#endif

// Compile-time assert
#ifndef CTASSERT
#if defined(_DEBUG) || defined(_PSEUDO_DEBUG)   // Support MFC AppWizards Pseudo-Debug mode
#define CTASSERT(_ex) { int _rg[_ex ? 1 : -1]; _rg; }
#else
#define CTASSERT(_ex)
#endif
#endif

///////////////////////////////////////////////////////////////////
// CDynamicMemberList helpers

#ifdef _DEBUG

void TracePropPut(DWORD idExpando, DISPID id, const BSTR bstrName, const VARIANT& varValue)
{
    CComVariant varBstr;
    HRESULT     hr = varBstr.ChangeType(VT_BSTR, &varValue);
    CComBSTR    bstrValue = (SUCCEEDED(hr) ? varBstr.bstrVal : OLESTR("<value>"));

    ATLTRACE2(atlTraceUser, 2, "Expando (0x%x) propput: %S (%d)= '%S'\n", idExpando, bstrName, id, SafeBSTR(static_cast<BSTR>(bstrValue)));
}

void TracePropGet(DWORD idExpando, DISPID id, const BSTR bstrName, const VARIANT& varValue)
{
    CComVariant varBstr;
    HRESULT     hr = varBstr.ChangeType(VT_BSTR, &varValue);
    CComBSTR    bstrValue = (SUCCEEDED(hr) ? varBstr.bstrVal : OLESTR("<value>"));

    ATLTRACE2(atlTraceUser, 2, "Expando (0x%x) propget: %S (%d)= '%S'\n", idExpando, bstrName, id, SafeBSTR(static_cast<BSTR>(bstrValue)));
}

#define TRACEPROPPUT(_eid, _id, _name, _val) TracePropPut(_eid, _id, _name, _val);
#define TRACEPROPGET(_eid, _id, _name, _val) TracePropGet(_eid, _id, _name, _val);
#else
#define TRACEPROPPUT(_eid, _id, _name, _val)
#define TRACEPROPGET(_eid, _id, _name, _val)
#endif

///////////////////////////////////////////////////////////////////
// CDynamicMemberList

CDynamicMemberList::CDynamicMemberList() : m_idNext(1)
{}

DISPID CDynamicMemberList::GetNextDispID()
{
    // NOTE: Depending on ExcludeDispIDs to set m_idNext
    // to beginning of large range
    return m_idNext++;
}

HRESULT CDynamicMemberList::ExcludeDispIDs(ITypeInfo* pInfo)
{
    const DISPID    DISPID_MIN = 0;
    const DISPID    DISPID_MAX = LONG_MAX; CTASSERT(sizeof(DISPID) == sizeof(long));
    size_t          nids = 0;
    DISPID*         rgids = 0;

    // Determine the number of methods on the interface
	TYPEATTR* pta = 0;
    HR(pInfo->GetTypeAttr(&pta));

    if( pta->cFuncs )
    {
        nids = pta->cFuncs + 2;
        rgids = new DISPID[nids];
        if( !rgids ) return E_OUTOFMEMORY;
        ZeroMemory(rgids, sizeof(DISPID) * nids);

		// Loop through getting the information on each member
		for( UINT i = 0; i != pta->cFuncs; ++i )
		{
			// Get the function description for this member
			FUNCDESC* pfd = 0;
			HRESULT hr = pInfo->GetFuncDesc(i, &pfd);
            if( FAILED(hr) ) continue;

            // Add DISPID to list to be excluded
            rgids[i+1] = pfd->memid;

    		// Release the memory alloced on our behalf
			pInfo->ReleaseFuncDesc(pfd);
		}

        // Add min and max
        rgids[0] = DISPID_MIN;
        rgids[nids - 1] = DISPID_MAX;
    }

	// Release the memory alloced on our behalf
	pInfo->ReleaseTypeAttr(pta);

    if( !nids ) return S_OK;

    // Sort DISPIDs to exclude so largest range can be found
    std::sort(&rgids[0], &rgids[nids]);

    // Find the largest range of DISPIDs not to exclude
    // and set m_idNext to beginning of the range.
    ULONG   maxIdsInRange = 0;
    for( DISPID* pid = &rgids[0]; pid != &rgids[nids-1]; ++pid )
    {
        // Exclude standard DISPIDs from search
        if( *pid < 1 ) continue;

        ULONG   nIdsInRange = (*(pid + 1) - *pid);
        if( nIdsInRange > maxIdsInRange )
        {
            maxIdsInRange = nIdsInRange;
            m_idNext = *pid + 1;
        }
    }

    delete[] rgids;

    ATLTRACE2(atlTraceUser, 2, __T("CDynamicMemberList::ExcludeDispIDs -- picked m_idNext = %d\n"), m_idNext);
    return S_OK;
}

size_t CDynamicMemberList::Count()
{
    // Count undeleted members
    long    cMembers = 0;
    for( CMemberIterator it = m_vecMembers.begin(); it != m_vecMembers.end(); ++it )
    {
        if( !it->Deleted() ) ++cMembers;
    }

    return cMembers;
}

HRESULT CDynamicMemberList::AddMember(
    BSTR            bstrName,
    DWORD           grfdex,
    const VARIANT&  varValue,
    DISPID*         pid)
{
    // Check for existing member (even a deleted one)
    CMemberIterator    it = FindMember(bstrName, grfdex, true);
    if( it != m_vecMembers.end() )
    {
        // Undelete a deleted member
        it->SetDeleted(false);
        it->SetName(bstrName);
        it->SetFlags(grfdex);
        it->SetValue(varValue);
        *pid = it->ID();
        return S_OK;
    }

    // Add a new member
    DISPID  id = GetNextDispID();
    CMember member(id, bstrName, grfdex, varValue);
    try
    {
        m_vecMembers.push_back(member);
        *pid = id;

        TRACEPROPPUT((DWORD)this, member.ID(), member.Name(), member.Value());
        return S_OK;
    }
    catch(...) {}

    return E_OUTOFMEMORY;
}

HRESULT CDynamicMemberList::GetDispID(
    BSTR    bstrName,
    DWORD   grfdex,
    DISPID* pid)
{
    // Check for name of DISPID_VALUE
    if( wcsicmp(bstrName, L"value") == 0 )
    {
        *pid = DISPID_VALUE;
        return S_OK;
    }

    // Find an existing member
    CMemberIterator it = FindMember(bstrName, grfdex);
    if( it != m_vecMembers.end() ) 
    {
        *pid = it->ID();
        return S_OK;
    }

    return DISP_E_UNKNOWNNAME;
}

HRESULT CDynamicMemberList::GetProperty(
    DISPID      id,
    VARIANT*    pvarRes)
{
    if( !pvarRes ) return E_POINTER; VariantInit(pvarRes);

    // Find the property
    CMemberIterator it = FindMember(id);
    if( it == m_vecMembers.end() ) return DISP_E_MEMBERNOTFOUND;

    // Return the value
    HR(VariantCopy(pvarRes, const_cast<VARIANT*>(&it->Value())));

    TRACEPROPGET((DWORD)this, it->ID(), it->Name(), *pvarRes);
    return S_OK;
}

HRESULT CDynamicMemberList::PutProperty(
    DISPID          id,
    const VARIANT&  var)
{
    // Find the property
    CMemberIterator it = FindMember(id);
    if( it == m_vecMembers.end() ) return DISP_E_MEMBERNOTFOUND;

    // Cache the new value
    it->SetValue(var);

    TRACEPROPPUT((DWORD)this, it->ID(), it->Name(), var);
    return S_OK;
}

/*
IDispatchEx allows JScript clients to use expandos like this:

    var expando = new ActiveXObject("genx.expandoProperties");
    expando.key_foo = "value_foo";
    expando.key_bar = "value_bar";
    expando["key_quux"] = "value_quux";

    var s = "";
    for( key in expando ) {
        s += key + "= '" + expando[key] + "'\n";
    }

    alert(s);

Specialized handling of DISPID_NEWENUM and DISPID_VALUE allows VBScript clients
to use expandos in the same way:

    dim expando: set expando = CreateObject("genx.expandoProperties")
    expando.key_foo = "value_foo"
    expando.key_bar = "value_bar"
    expando("key_quux") = "value_quux"

    dim s
    for each key in expando
        s = s & key & "= '" & expando(key) & "'" & vbCR & vbLF
    next

    msgbox s
*/
HRESULT CDynamicMemberList::InvokeEx(DISPID id, LCID lcid, WORD wFlags, DISPPARAMS* pdp, VARIANT* pvarRes,	EXCEPINFO* pei, IServiceProvider* pspCaller)
{
	HRESULT hr = E_FAIL;

    switch( wFlags )
    {
    case DISPATCH_PROPERTYGET:
    case DISPATCH_PROPERTYGET | DISPATCH_METHOD:
    {
        // Special-case DISPID_NEWENUM
        if( id == DISPID_NEWENUM )
        {
            if( !pvarRes ) return E_POINTER;
            VariantClear(pvarRes);

            CComPtr<IUnknown>   punkEnum;
            HR(this->GetNewEnum(&punkEnum));
            pvarRes->vt = VT_UNKNOWN;
            pvarRes->punkVal = punkEnum.Detach();
            return S_OK;
        }
        // Special-case DISPID_VALUE
        // Single argument is key name
        else if( id == DISPID_VALUE )
        {
            if( !pdp ) return E_UNEXPECTED;
            if( pdp->cArgs != 1 ) return DISP_E_BADPARAMCOUNT;

            // 1st arg is the key name
            CComVariant varKey;
            HR(varKey.ChangeType(VT_BSTR, &pdp->rgvarg[0]));
    
            // Find or add the property
            // NOTE: Side effect -- id changed from DISPID_VALUE
            //       to the DISPID of the key passed as the 1st arg.
            HRESULT hr = this->GetDispID(varKey.bstrVal, 0, &id);
            if( FAILED(hr) )
            {
                HR(this->AddMember(varKey.bstrVal, 0, CComVariant(), &id));
            }
        }

        return this->GetProperty(id, pvarRes);
    }
    break;

    case DISPATCH_PROPERTYPUT:
    case DISPATCH_PROPERTYPUTREF:
    case DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF:
    {
        if( pvarRes ) VariantClear(pvarRes);

        // Special-case DISPID_VALUE
        // 1st arg is key name
        // 2nd arg is new value
        if( id == DISPID_VALUE )
        {
            if( !pdp ) return E_INVALIDARG;
            if( pdp->cArgs != 2 ) return DISP_E_BADPARAMCOUNT;

            // 1st arg is the key name
            CComVariant varKey;
            HR(varKey.ChangeType(VT_BSTR, &pdp->rgvarg[1]));

            // Lookup key's DISPID and set the value
            HRESULT hr = this->GetDispID(varKey.bstrVal, 0, &id);
            if( FAILED(hr) )
            {
                return this->AddMember(varKey.bstrVal, 0, pdp->rgvarg[0], &id);
            }
            else
            {
                return this->PutProperty(id, pdp->rgvarg[0]);
            }
        }
        // 1st arg is new value of property
        else
        {
            if( !pdp ) return E_INVALIDARG;
            if( pdp->cArgs != 1 ) return DISP_E_BADPARAMCOUNT;
            return this->PutProperty(id, pdp->rgvarg[0]);
        }
    }
    break;

    // Invoke DISPID_VALUE on function object
    case DISPATCH_METHOD:
    {
        UINT        uArgErr;
        CComVariant var;
        HR(this->GetProperty(id, &var));
        HR(var.ChangeType(VT_DISPATCH));
        HR(var.pdispVal->Invoke(DISPID_VALUE, IID_NULL, lcid, DISPATCH_METHOD, pdp, pvarRes, pei, &uArgErr));
        return S_OK;
    }
    break;
    }

    return E_UNEXPECTED;
}

// Hand out a collection of keys
HRESULT CDynamicMemberList::GetNewEnum(IUnknown** ppunkEnum)
{
    if( !ppunkEnum ) return E_INVALIDARG;
    *ppunkEnum = 0;

    // Count members
    ULONG   cMembers = this->Count();

    // Create array of VARIANTs
    VARIANT*    rgvar = new VARIANT[cMembers];
    if( !rgvar ) return E_OUTOFMEMORY;
    ZeroMemory(rgvar, sizeof(VARIANT) * cMembers);

    ULONG   i = 0;
    for( CMemberIterator it = m_vecMembers.begin(); it != m_vecMembers.end(); ++it )
    {
        if( !it->Deleted() )
        {
            //HR(VariantCopy(&rgvar[i], const_cast<VARIANT*>(&it->Value())));
            rgvar[i].vt = VT_BSTR;
            rgvar[i].bstrVal = SysAllocString(it->Name());
            ++i;
        }
    }

    // Create enumerator
    CComObject< CComEnum<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT> > >*  pEnum = 0;
    HR(pEnum->CreateInstance(&pEnum));
    CComPtr<IUnknown>   spunkEnum = pEnum;  // Stablize the pointer
    HR(pEnum->Init(&rgvar[0], &rgvar[cMembers], 0, AtlFlagTakeOwnership));
    *ppunkEnum = spunkEnum.Detach();

    return S_OK;
}

HRESULT CDynamicMemberList::DeleteMember(
    BSTR    bstrName,
    DWORD   grfdex)
{
    // Find the dynamic property
    CMemberIterator it = FindMember(bstrName, grfdex, true);
    if( it == m_vecMembers.end() ) return S_OK;

    // Set it "deleted"
    it->SetDeleted(true);
    return S_OK;
}

HRESULT CDynamicMemberList::DeleteMember(DISPID id)
{
    // Find the dynamic property
    CMemberIterator it = FindMember(id, true);
    if( it == m_vecMembers.end() ) return S_OK;

    // Set it "deleted"
    it->SetDeleted(true);
    return S_OK;
}

HRESULT CDynamicMemberList::GetMemberProperties(
    DISPID          id,
    BSTR*           pbstrName,
    VARTYPE*        pvt,
    DWORD*          pgrfdex)
{
    // Find the dynamic property
    CMemberIterator it = FindMember(id);
    if( it == m_vecMembers.end() ) return DISP_E_MEMBERNOTFOUND;

    if( pbstrName ) *pbstrName = SysAllocString(it->Name());
    if( pvt ) *pvt = it->Value().vt;
    if( pgrfdex ) *pgrfdex = it->Flags();
    return S_OK;
}

HRESULT CDynamicMemberList::GetNextDispID(
    DWORD   grfdex,
    DISPID  id,
    DISPID* pid)
{
    if( !pid ) return E_POINTER;

    CMemberIterator it;

    // Are we starting the search?
    if( id == DISPID_STARTENUM )
    {
        // Get the first potential candidate
        it = m_vecMembers.begin();
    }
    // Are we continuing the search?
    else
    {
        // Find the current property (even if it's deleted)
        it = FindMember(id, true);
        
        // Get the next one (which is the first potential candidate
        if( it != m_vecMembers.end() ) ++it;
    }

    // Check for the next non-deleted member
    while( it != m_vecMembers.end() && it->Deleted() ) ++it;

    // Are we at the end?
    if( it == m_vecMembers.end() ) return S_FALSE;

    // Return the ID of the next one
    *pid = it->ID();
    return S_OK;
}

CDynamicMemberList::CMemberIterator CDynamicMemberList::FindMember(
    BSTR    bstrName,
    DWORD   grfdex,
    bool    bFindDeleted)
{
    CMemberIterator it = std::find_if(m_vecMembers.begin(), m_vecMembers.end(), FindMemberByName(bstrName, grfdex));
    if( it == m_vecMembers.end() ) return it;
    if( it->Deleted() && !bFindDeleted ) return m_vecMembers.end();
    return it;
}

CDynamicMemberList::CMemberIterator CDynamicMemberList::FindMember(
    DISPID  id,
    bool    bFindDeleted)
{
    CMemberIterator it = std::find_if(m_vecMembers.begin(), m_vecMembers.end(), FindMemberByID(id));
    if( it == m_vecMembers.end() ) return it;
    if( it->Deleted() && bFindDeleted ) return m_vecMembers.end();
    return it;
}

///////////////////////////////////////////////////////////////////
// FindMemberByName

CDynamicMemberList::FindMemberByName::FindMemberByName(
    const BSTR  bstrName,
    DWORD       grfdex)
:   m_bstrName(bstrName),
    m_grfdex(grfdex)
{}

bool CDynamicMemberList::FindMemberByName::operator()(const CMember& member)
{
    if( m_grfdex & fdexNameCaseSensitive )
    {
        return (wcscmp(member.Name(), m_bstrName) == 0);
    }
    else
    {
        return (wcsicmp(member.Name(), m_bstrName) == 0);
    }
}

///////////////////////////////////////////////////////////////////
// FindMemberByID

CDynamicMemberList::FindMemberByID::FindMemberByID(DISPID id)
:   m_id(id)
{}

bool CDynamicMemberList::FindMemberByID::operator()(const CMember& member)
{
    return (member.ID() == m_id);
}

///////////////////////////////////////////////////////////////////
// CMember

CDynamicMemberList::CMember::CMember(
    DISPID          id,
    BSTR            bstrName,
    DWORD           grfdex,
    const VARIANT&  varValue)
:   m_id(id),
    m_bstrName(bstrName),
    m_grfdex(grfdex),
    m_varValue(varValue),
    m_bDeleted(false)
{}

DISPID CDynamicMemberList::CMember::ID() const
{ return m_id; }

BSTR CDynamicMemberList::CMember::Name() const
{ return m_bstrName; }

DWORD CDynamicMemberList::CMember::Flags() const
{ return m_grfdex; }

const VARIANT& CDynamicMemberList::CMember::Value() const
{ return m_varValue; }

bool CDynamicMemberList::CMember::Deleted() const
{ return m_bDeleted; }    

void CDynamicMemberList::CMember::SetID(DISPID id)
{ m_id = id; }

void CDynamicMemberList::CMember::SetName(BSTR bstrName)
{ m_bstrName = bstrName; }

void CDynamicMemberList::CMember::SetFlags(DWORD grfdex)
{ m_grfdex = grfdex; }

void CDynamicMemberList::CMember::SetValue(const VARIANT& varValue)
{ m_varValue = varValue; }

void CDynamicMemberList::CMember::SetDeleted(bool bDeleted)
{
    m_varValue.Clear();
    m_bDeleted = bDeleted;
}
