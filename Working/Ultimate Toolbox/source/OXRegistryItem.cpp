// ==========================================================================
//				Class Implementation : COXRegistryItem
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXRegistryItem.h"
#include "OXMainRes.h"

#include "UTBStrOp.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXRegistryItem, CObject)

#define new DEBUG_NEW

#ifdef _DEBUG
// Trace a message when the RESULT specifies failure
#define CONDITIONAL_TRACE_RESULT(TEXT, RESULT)							\
{ if (FAILED(RESULT)) {												\
	TRACE(_T("%s (%s): Failed (%u == 0x%X, Code : %u) :\n\t%s\n"),	\
	_T(TEXT), (LPCTSTR)GetFullRegistryItem(), RESULT, RESULT,		\
	HRESULT_CODE(RESULT), GetResultMessage(RESULT));				\
} }
#else
// Do not trace in Release build
#define CONDITIONAL_TRACE_RESULT(TEXT, RESULT)
#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////
// Definition of static variables
static const TCHAR szClassesRoot[] =	_T("\\ClassesRoot");
static const TCHAR szCurrentUser[] =	_T("\\CurrentUser");
static const TCHAR szLocalMachine[] =	_T("\\LocalMachine");
static const TCHAR szUsers[] =			_T("\\Users");
static const TCHAR szPerformanceData[] = _T("\\PerformanceData");
static const TCHAR szCurrentConfig[] =	_T("\\CurrentConfig");
static const TCHAR szDynData[] =		_T("\\DynData");
static const TCHAR szNoName[] =			_T("NoName");

static const TCHAR szBackslash[] = _T("\\");
static const TCHAR szDoubleBackslash[] = _T("\\\\");
static const TCHAR szBar[] = _T("|");
static const TCHAR szSoftware[] = _T("Software");
static const TCHAR szUnknownCompanyName[] = _T("UnknownCompany");
static const TCHAR szUnknownApplicationName[] = _T("UnknownApplication");

/////////////////////////////////////////////////////////////////////////////
// Definition of static members
const LPCTSTR COXRegistryItem::m_pszClassesRoot =		szClassesRoot;
const LPCTSTR COXRegistryItem::m_pszCurrentUser =		szCurrentUser;
const LPCTSTR COXRegistryItem::m_pszLocalMachine =		szLocalMachine;
const LPCTSTR COXRegistryItem::m_pszUsers =				szUsers;
const LPCTSTR COXRegistryItem::m_pszPerformanceData =	szPerformanceData;
const LPCTSTR COXRegistryItem::m_pszCurrentConfig =		szCurrentConfig;
const LPCTSTR COXRegistryItem::m_pszDynData =			szDynData;
const LPCTSTR COXRegistryItem::m_pszNoName =			szNoName;

const TCHAR COXRegistryItem::m_cNull = _T('\0');
const TCHAR COXRegistryItem::m_cBackslash = _T('\\');
const TCHAR COXRegistryItem::m_cBar = _T('|');
const LPCTSTR COXRegistryItem::m_pszBackslash = szBackslash;
const LPCTSTR COXRegistryItem::m_pszDoubleBackslash = szDoubleBackslash;
const LPCTSTR COXRegistryItem::m_pszBar = szBar;
const LPCTSTR COXRegistryItem::m_pszSoftware = szSoftware;
LPCTSTR COXRegistryItem::m_pszUnknownCompanyName = szUnknownCompanyName;
LPCTSTR COXRegistryItem::m_pszUnknownApplicationName = szUnknownApplicationName;

COXRegistryItem::COXRootRegistryKey COXRegistryItem::m_rgRootRegistryKey[]=
{
	{ szClassesRoot,	HKEY_CLASSES_ROOT	},
	{ szCurrentUser,	HKEY_CURRENT_USER	},
	{ szLocalMachine,	HKEY_LOCAL_MACHINE	},
	{ szUsers,			HKEY_USERS			},
	{ szPerformanceData, HKEY_PERFORMANCE_DATA },
	{ szCurrentConfig,	HKEY_CURRENT_CONFIG	},
	{ szDynData,		HKEY_DYN_DATA		},
	{ NULL,				(HKEY)NULL			}
};


// Data members -------------------------------------------------------------
// protected:
// CString m_sFullRegistryItem;
// --- The full registry item specification (e.g. "\\Comp1\LocalMachine\Software\Test")

// BOOL m_bMainBuilt;
// --- Whether the value in m_sFullRegistryItem is valid

// CString m_sMachineName;
// --- The machine name part of the registry item 
//     (empty or containing two leading backslashes)
// CString m_sRegistryName;
// --- The registry name part of the registry item 
//     (empty or containing one leading backslash)
// CString m_sKeyNames;
// --- The key name part of the registry item 
//     (empty or containing one leading and one trailing backslash)
// CString m_sValueName;
// --- The value name part of the registry item 
//     (never containing any backslash)

// BOOL m_bPartsBuilt;
// --- Whether all the part (specified above) are valid 

// HKEY m_hRootKey;
// --- Handle of the root of the registry (may be remote)

// HKEY m_hKey;
// --- Handle of the current key 

// HRESULT m_nLastError;
// --- Error code of the last error of the object

// static const TCHAR m_cNull;
// static const TCHAR m_cBackslash;
// static const TCHAR m_cBar;
// static const LPCTSTR m_pszBackslash;
// static const LPCTSTR m_pszDoubleBackslash;
// static const LPCTSTR m_pszBar;
// --- Constant strings and characters

// struct COXRootRegistryKey
//	{
//	LPCTSTR m_pszRegistryName;
//	HKEY	m_hRegistryKey;
//	};
// static COXRootRegistryKey m_rgRootRegistryKey[];
// --- Array of registry names and there handle 
//     Always starts with a backslash

// private:

// Member functions ---------------------------------------------------------
// public:

COXRegistryItem::COXRegistryItem(LPCTSTR pszFullRegistryItem /* = NULL */)
:
m_sFullRegistryItem(pszFullRegistryItem),
m_bMainBuilt(TRUE),
m_sMachineName(),
m_sRegistryName(),
m_sKeyNames(),
m_sValueName(),
m_bPartsBuilt(FALSE),
m_hRootKey(NULL),
m_hKey(NULL),
m_nLastError(NULL)
{
	ASSERT_VALID(this);
}

COXRegistryItem::COXRegistryItem(const COXRegistryItem& registryItem)
:
m_sFullRegistryItem(registryItem.m_sFullRegistryItem),
m_bMainBuilt(		registryItem.m_bMainBuilt),
m_sMachineName(		registryItem.m_sMachineName),
m_sRegistryName(	registryItem.m_sRegistryName),
m_sKeyNames(		registryItem.m_sKeyNames),
m_sValueName(		registryItem.m_sValueName),
m_bPartsBuilt(		registryItem.m_bPartsBuilt),
m_hRootKey(			NULL),
m_hKey(				NULL)
{
	ASSERT_VALID(&registryItem);

	// The registry key handles are set to NULL, so they will have to be opened
	// again before use

	ASSERT_VALID(this);
}

COXRegistryItem& COXRegistryItem::operator=(const COXRegistryItem& registryItem)
{
	ASSERT_VALID(&registryItem);

	if(this==&registryItem)
		return *this;

	// First clean current settings (and close possible open handles)
	Empty();

	// Copy simple data members
	m_sFullRegistryItem = registryItem.m_sFullRegistryItem;
	m_bMainBuilt =		registryItem.m_bMainBuilt;
	m_sMachineName =	registryItem.m_sMachineName;
	m_sRegistryName =	registryItem.m_sRegistryName;
	m_sKeyNames =		registryItem.m_sKeyNames;
	m_sValueName =		registryItem.m_sValueName;
	m_bPartsBuilt =		registryItem.m_bPartsBuilt;

	// The registry key handles are set to NULL, so they will have to be opened
	// again before use
	ASSERT(m_hRootKey == NULL);
	ASSERT(m_hKey == NULL);

	ASSERT_VALID(this);
	return *this;
}

CString COXRegistryItem::GetFullRegistryItem()
{
	BuildMain();
	ASSERT_VALID(this);
	return m_sFullRegistryItem;
}

CString COXRegistryItem::GetMachineName()
{
	BuildParts();
	ASSERT_VALID(this);
	return m_sMachineName;
}

CString COXRegistryItem::GetRegistryName()
{
	BuildParts();
	ASSERT_VALID(this);
	return m_sRegistryName;
}

CString COXRegistryItem::GetKeyNames()
{
	BuildParts();
	ASSERT_VALID(this);
	return m_sKeyNames;
}

CString COXRegistryItem::GetValueName()
{
	BuildParts();
	ASSERT_VALID(this);
	return m_sValueName;
}

void COXRegistryItem::SetFullRegistryItem(LPCTSTR pszFullRegistryItem)
{
	ASSERT_VALID(this);
	m_sFullRegistryItem = pszFullRegistryItem;

	// ... We have changed main, so the parts are not up-to-date anymore
	DestroyParts();
	AdjustMain();

	// ... Close a possible open key
	Close();
	ASSERT_VALID(this);
}

void COXRegistryItem::SetMachineName(LPCTSTR pszMachineName)
{
	ASSERT_VALID(this);
	// ... Build the other parts before changing
	BuildParts();

	m_sMachineName = pszMachineName;

	// ... We have changed a part, so main is not up-to-date anymore
	DestroyMain();
	AdjustParts();

	// ... Close a possible open key
	Close();
	ASSERT_VALID(this);
}

void COXRegistryItem::SetRegistryName(LPCTSTR pszRegistryName)
{
	ASSERT_VALID(this);
	// ... Build the other parts before changing
	BuildParts();

	m_sRegistryName = pszRegistryName;

	// ... We have changed a part, so main is not up-to-date anymore
	DestroyMain();
	AdjustParts();

	// ... Close a possible open key
	Close();
	ASSERT_VALID(this);
}

void COXRegistryItem::SetKeyNames(LPCTSTR pszKeyNames)
{
	ASSERT_VALID(this);
	// ... Build the other parts before changing
	BuildParts();

	m_sKeyNames = pszKeyNames;

	// ... We have changed a part, so main is not up-to-date anymore
	DestroyMain();
	AdjustParts();

	// ... Close a possible open key
	Close();
	ASSERT_VALID(this);
}

void COXRegistryItem::SetValueName(LPCTSTR pszValueName)
{
	ASSERT_VALID(this);
	// ... Build the other parts before changing
	BuildParts();

	m_sValueName = pszValueName;

	// ... We have changed a part, so main is not up-to-date anymore
	DestroyMain();
	AdjustParts();

	// No need to close an open key, because changing the value name has no effect on the key
	ASSERT_VALID(this);
}

void COXRegistryItem::InitializeFromApplication(LPCTSTR pszSubkeyName /* = NULL */)
{
	ASSERT_VALID(this);

	CString sCompany;
	CString sApplication;
	CString sSubkeyName = pszSubkeyName;

	// Get correct values from application object
	if (AfxGetApp() != NULL)
	{
		sCompany = AfxGetApp()->m_pszRegistryKey;
		sApplication = AfxGetApp()->m_pszProfileName;
	}

	// Use default values if empty
	if (sCompany.IsEmpty())
	{
		TRACE1("COXRegistryItem::InitializeFromApplication : No company name is specified, using %s\n",
			m_pszUnknownCompanyName);
		sCompany = m_pszUnknownCompanyName;
	}
	if (sApplication.IsEmpty())
	{
		TRACE1("COXRegistryItem::InitializeFromApplication : No application name is specified, using %s\n",
			m_pszUnknownApplicationName);
		sApplication = m_pszUnknownApplicationName;
	}


	// Remove possible leading or trailing back slash from subkey name
	if (!sSubkeyName.IsEmpty() && (sSubkeyName.GetAt(0) == m_cBackslash))
		sSubkeyName = sSubkeyName.Mid(1);
	if (!sSubkeyName.IsEmpty() && (sSubkeyName.GetAt(sSubkeyName.GetLength() - 1) == m_cBackslash))
		sSubkeyName = sSubkeyName.Left(sSubkeyName.GetLength() - 1);

	// Build complete keyname
	// \Software\<sCompany>\<sApplication>\<sSubkeyName>
	CString sKeyNames;
	if (sSubkeyName.IsEmpty())
		sKeyNames.Format(_T("\\%s\\%s\\%s\\"), m_pszSoftware, sCompany, sApplication);
	else
		sKeyNames.Format(_T("\\%s\\%s\\%s\\%s\\"), m_pszSoftware, sCompany, sApplication, sSubkeyName);

	// Use only the key names to build a new registry item (rest is default)
	SetFullRegistryItem(sKeyNames);

	ASSERT_VALID(this);
}

void COXRegistryItem::Empty()
{
	ASSERT_VALID(this);

	Close();
	ASSERT(m_hRootKey == NULL);
	ASSERT(m_hKey == NULL);

	m_sFullRegistryItem.Empty();
	m_bMainBuilt = FALSE;

	m_sMachineName.Empty();
	m_sRegistryName.Empty();
	m_sKeyNames.Empty();
	m_sValueName.Empty();
	m_bPartsBuilt = FALSE;

	ASSERT_VALID(this);
}

void COXRegistryItem::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	// Call base class implementation
	CObject::Serialize(ar);

	if (ar.IsStoring())
		ar << GetFullRegistryItem();
	else
	{
		CString sNewFullRegistryItem;
		ar >> sNewFullRegistryItem;
		SetFullRegistryItem(sNewFullRegistryItem);
	}

	ASSERT_VALID(this);
}

BOOL COXRegistryItem::Open(BOOL bCreate /* = TRUE */, REGSAM samDesired /* = KEY_ALL_ACCESS */,
						   LPCTSTR pszClass /* = NULL */, DWORD dwOptions /* = REG_OPTION_NON_VOLATILE */,
						   LPSECURITY_ATTRIBUTES lpSecurityAttributes /* = NULL */, LPDWORD pdwDisposition /* = NULL */)
{
	ASSERT_VALID(this);

	// ... First close a possible open key
	Close();

	// Get the key name
	CString sKeyName = GetKeyNames();

	// Get the root key 
	if (!OpenRootKey())
	{
		ASSERT(!SUCCEEDED(m_nLastError));
		return SUCCEEDED(m_nLastError);
	}

	// Remove possible leading and trailing backslash from key names
	if (!sKeyName.IsEmpty())
	{
		// ... Non empty key name must begin with and end in back slash
		ASSERT(sKeyName.GetAt(0) == m_cBackslash);
		ASSERT(sKeyName.GetAt(sKeyName.GetLength() - 1) == m_cBackslash);
		sKeyName = sKeyName.Mid(1, sKeyName.GetLength() - 2);
	}

	// At the moment we are in an intermediate state (root already opened,
	// registry not yet) : ASSERT_VALID(this) does not hold at this position
	ASSERT(m_hRootKey != NULL);
	ASSERT(m_hKey == NULL);

	if (bCreate)
	{
		CString sClass(pszClass);
		m_nLastError = HResultFromWin32(::RegCreateKeyEx(m_hRootKey, sKeyName, 0, 
			sClass.GetBuffer(0), dwOptions, samDesired, lpSecurityAttributes, &m_hKey,
			pdwDisposition));
		sClass.ReleaseBuffer();
	}
	else
	{
		m_nLastError = HResultFromWin32(::RegOpenKeyEx(m_hRootKey, sKeyName, dwOptions, 
			samDesired, &m_hKey));
	}

	if (FAILED(m_nLastError))
	{
		// Close a possible open remote root key
		// ... Use the result of the failed Open (not the succeeded Close)
		HRESULT nLastError = m_nLastError;
		Close();
		m_nLastError = nLastError;
	}

#ifndef OX_REGITEM_DONTTRACE
	CONDITIONAL_TRACE_RESULT("COXRegistryItem::Open", m_nLastError);
#endif
	ASSERT_VALID(this);
	return SUCCEEDED(m_nLastError);
}

BOOL COXRegistryItem::Delete()
{
	ASSERT_VALID(this);

	m_nLastError = ERROR_SUCCESS;
	if (GetValueName().IsEmpty())
	{
		// Delete key

		// Get the key name
		CString sKeyName = GetKeyNames();

		// Get the root key 
		if ((m_hRootKey == NULL) && !OpenRootKey())
		{
			ASSERT(!SUCCEEDED(m_nLastError));
			return SUCCEEDED(m_nLastError);
		}

		// Remove possible leading and trailing backslash from key names
		if (!sKeyName.IsEmpty())
		{
			// ... Non empty key name must begin with and end in back slash
			ASSERT(sKeyName.GetAt(0) == m_cBackslash);
			ASSERT(sKeyName.GetAt(sKeyName.GetLength() - 1) == m_cBackslash);
			sKeyName = sKeyName.Mid(1, sKeyName.GetLength() - 2);
		}

		// Close the key (not the rootkey) before deleting it
		CloseKey();
		HRESULT nLastError = HResultFromWin32(::RegDeleteKey(m_hRootKey, sKeyName));
		// ... Close all keys now
		Close();
		// ... Use the result of the failed Open (not the succeeded Close)
		m_nLastError = nLastError;
	}
	else
	{
		// Delete value
		CString sValueName = GetValueName();
		// ... Use an empty string if the NoName constant is used
		if (sValueName.CompareNoCase(m_pszNoName) == 0)
			sValueName.Empty();

		// ... Open key if not yet open (first try to open with maximum
		//     access, then only with KEY_SET_VALUE to delete the value)
		if (IsOpen() || Open(FALSE) || Open(FALSE, KEY_SET_VALUE))
			m_nLastError = HResultFromWin32(::RegDeleteValue(m_hKey, sValueName));
	}

#ifndef OX_REGITEM_DONTTRACE
	CONDITIONAL_TRACE_RESULT("COXRegistryItem::Close", m_nLastError);
#endif
	ASSERT_VALID(this);
	return SUCCEEDED(m_nLastError);
}

BOOL COXRegistryItem::GetSecurity(SECURITY_INFORMATION SecurityInformation,
								  PSECURITY_DESCRIPTOR pSecurityDescriptor, LPDWORD pcbSecurityDescriptor)
{
	ASSERT_VALID(this);

	m_nLastError = ERROR_SUCCESS;
	// ... Open key if not yet open (first try to open with maximum
	//     access, then only with KEY_READ to read the security)
	if (IsOpen() || Open(FALSE) || Open(FALSE, KEY_READ))
		m_nLastError = HResultFromWin32(::RegGetKeySecurity(m_hKey, SecurityInformation,
		pSecurityDescriptor, pcbSecurityDescriptor));

#ifndef OX_REGITEM_DONTTRACE
	CONDITIONAL_TRACE_RESULT("COXRegistryItem::GetSecurity", m_nLastError);
#endif
	ASSERT_VALID(this);
	return SUCCEEDED(m_nLastError);
}

BOOL COXRegistryItem::SetSecurity(SECURITY_INFORMATION SecurityInformation,
								  PSECURITY_DESCRIPTOR pSecurityDescriptor)
{
	ASSERT_VALID(this);

	m_nLastError = ERROR_SUCCESS;
	// ... Open key if not yet open (first try to open with maximum
	//     access, then only with KEY_WRITE to set the security)
	if (IsOpen() || Open(FALSE) || Open(FALSE, KEY_WRITE))
		m_nLastError = HResultFromWin32(::RegSetKeySecurity(m_hKey, SecurityInformation,
		pSecurityDescriptor));

#ifndef OX_REGITEM_DONTTRACE
	CONDITIONAL_TRACE_RESULT("COXRegistryItem::SetSecurity", m_nLastError);
#endif
	ASSERT_VALID(this);
	return SUCCEEDED(m_nLastError);
}

BOOL COXRegistryItem::Flush()
{
	ASSERT_VALID(this);

	m_nLastError = ERROR_SUCCESS;
	if (m_hKey != NULL)
		m_nLastError = HResultFromWin32(::RegFlushKey(m_hKey));

#ifndef OX_REGITEM_DONTTRACE
	CONDITIONAL_TRACE_RESULT("COXRegistryItem::Flush", m_nLastError);
#endif
	ASSERT_VALID(this);
	return SUCCEEDED(m_nLastError);
}

BOOL COXRegistryItem::Close()
{
	m_nLastError = ERROR_SUCCESS;

	CloseKey();
	CloseRootKey();

	ASSERT(m_hKey == NULL);
	ASSERT(m_hRootKey == NULL);

#ifndef OX_REGITEM_DONTTRACE
	CONDITIONAL_TRACE_RESULT("COXRegistryItem::Close", m_nLastError);
#endif
	ASSERT_VALID(this);
	return SUCCEEDED(m_nLastError);
}

CString COXRegistryItem::GetStringValue(LPCTSTR pszValueName /* = NULL */, 
										BOOL bAllowUnexpanded /* = TRUE */, BOOL bAutoExpand /* = TRUE */,
										CString sDefault /* = _T("") */)
{
	ASSERT_VALID(this);

	// ... Default result to empty string
	CString sResultValue;

	DWORD nType = 0;
	DWORD nSize = 0;
	// ... If unexpanded string are not allowed, we only accept plain string type
	DWORD nRequestedType = bAllowUnexpanded ? 0 : REG_SZ;
	LPBYTE pStringData = NULL;

	///	GetValue(&pStringData, pszValueName, nRequestedType, &nType, &nSize);
	if (GetValue(&pStringData, pszValueName, nRequestedType, &nType, &nSize) == 0)
	{
		sResultValue = sDefault;
		nType = REG_EXPAND_SZ;
	}

	// ... Use returned data only if it is not empty
	if ((nSize != 0) && (pStringData != NULL))
	{
		if ((nType == REG_SZ) || (nType == REG_EXPAND_SZ))
			sResultValue = (LPCTSTR)pStringData;
		else
			// ... Data type does not match
			m_nLastError = HResultFromWin32(ERROR_INVALID_DATA);

		delete[] pStringData;
		pStringData = NULL;
	}

	// ... Auto expand if requested and of correct type
	if (bAllowUnexpanded && bAutoExpand && (nType == REG_EXPAND_SZ))
		sResultValue = ExpandString(sResultValue );

#ifndef	OX_REGITEM_DONTTRACE
	CONDITIONAL_TRACE_RESULT("COXRegistryItem::GetStringValue", m_nLastError);
#endif
	ASSERT_VALID(this);
	return sResultValue;
}

CString COXRegistryItem::GetMultiStringValue(LPCTSTR pszValueName /* = NULL */,
											 CString sDefault /* = _T("") */)
{
	ASSERT_VALID(this);

	// ... Default result to empty string
	CString sResultValue;
	LPBYTE pMultiStringData = NULL;
	DWORD nSize = 0;
	///	GetValue(&pMultiStringData, pszValueName, REG_MULTI_SZ, NULL, &nSize);
	if (GetValue(&pMultiStringData, pszValueName, REG_MULTI_SZ, NULL, &nSize) == 0)
	{
		sResultValue = sDefault;
	}


	// ... Parse returned data only if it is not empty
	if ((nSize != 0) && (pMultiStringData != NULL))
	{
		NullToBarSeparator((LPTSTR)pMultiStringData);
		sResultValue = (LPCTSTR)pMultiStringData;

		delete[] pMultiStringData;
		pMultiStringData = NULL;
	}

	// ... Possible error condition has already been traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::GetMultiStringValue", m_nLastError);
	ASSERT_VALID(this);
	return sResultValue;
}

BOOL COXRegistryItem::GetMultiStringValue(CStringArray& stringArray, LPCTSTR pszValueName /* = NULL */)
{
	ASSERT_VALID(this);

	CString sMultiStringValue = GetMultiStringValue(pszValueName);

	// Copy individual strings (delimited by a bar) to the string array
	// ... Remove possible previous contents
	stringArray.RemoveAll();

	LPTSTR pszMultiStringValue = sMultiStringValue.GetBuffer(0);
	TCHAR * p;
	LPCTSTR pszStringValue = UTBStr::tcstok(pszMultiStringValue, m_pszBar, &p);
	while(pszStringValue != NULL)
	{
		stringArray.Add(pszStringValue);
		pszStringValue = UTBStr::tcstok(NULL, m_pszBar, &p);
	}
	sMultiStringValue.ReleaseBuffer();

	// ... Possible error condition has already been traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::GetMultiStringValue", m_nLastError);

	ASSERT_VALID(this);
	return SUCCEEDED(m_nLastError);
}

DWORD COXRegistryItem::GetNumberValue(LPCTSTR pszValueName /* = NULL */, 
									  BOOL bLittleEndian /* = TRUE */, DWORD dwDefault /* = 0 */)
{
	ASSERT_VALID(this);

	// ... Default result to 0
	DWORD nResultValue = 0;
	DWORD nSize = 0;

	DWORD nRequestedType = bLittleEndian ? REG_DWORD_LITTLE_ENDIAN : REG_DWORD_BIG_ENDIAN;
	LPBYTE pNumberData = NULL;
	///	GetValue(&pNumberData, pszValueName, nRequestedType, NULL, &nSize);
	if (GetValue(&pNumberData, pszValueName, nRequestedType, NULL, &nSize) == 0)
	{
		nResultValue = dwDefault;
	}

	// ... Use returned data only if it is not empty
	if ((nSize != 0) && (pNumberData != NULL))
	{
		nResultValue = *(LPDWORD)pNumberData;
		delete[] pNumberData;
		pNumberData = NULL;
	}

	// ... Possible error condition has already been traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::GetNumberValue", m_nLastError);
	ASSERT_VALID(this);
	return nResultValue;
}

BOOL COXRegistryItem::GetBinaryValue(CByteArray& binaryValue, 
									 LPCTSTR pszValueName /* = NULL */)
{
	ASSERT_VALID(this);

	DWORD nSize = 0;
	LPBYTE pBinaryData = NULL;
	GetValue(&pBinaryData, pszValueName, REG_BINARY, NULL, &nSize);

	// ... Use returned data only if it is not empty
	if ((nSize != 0) && (pBinaryData != NULL))
	{
		binaryValue.SetSize(nSize);
		::CopyMemory(binaryValue.GetData(), pBinaryData, nSize);
		delete[] pBinaryData;
		pBinaryData = NULL;
	}

	// ... Possible error condition has already been traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::GetBinaryValue", m_nLastError);
	ASSERT_VALID(this);
	return SUCCEEDED(m_nLastError);
}

BOOL COXRegistryItem::SetStringValue(LPCTSTR pszNewStringValue, 
									 LPCTSTR pszValueName /* = NULL */, BOOL bUnexpanded /* = FALSE */)
{
	ASSERT_VALID(this);

	DWORD nDataSize = (DWORD)(_tcslen(pszNewStringValue) + 1) * sizeof(TCHAR);
	const BYTE* pData = (const BYTE*)pszNewStringValue;
	DWORD nType = bUnexpanded ? REG_EXPAND_SZ : REG_SZ;

	SetValue(pszValueName, pData, nType, nDataSize);

	// ... Possible error condition has already been traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::SetStringValue", m_nLastError);
	ASSERT_VALID(this);
	return SUCCEEDED(m_nLastError);
}

BOOL COXRegistryItem::SetMultiStringValue(LPCTSTR pszNewMultiStringValue, 
										  LPCTSTR pszValueName /* = NULL */)
{
	ASSERT_VALID(this);

	if (!pszNewMultiStringValue)
		return FALSE;
	// ... Reserve space for double terminating null character in data copy
	DWORD nDataSize = (DWORD)(_tcslen(pszNewMultiStringValue) + 2) * sizeof(TCHAR);
	BYTE* pData = new BYTE[nDataSize];
	::CopyMemory(pData, pszNewMultiStringValue, nDataSize - 1);
	BarToNullSeparator((LPTSTR)pData);
	DWORD nType = REG_MULTI_SZ;

	SetValue(pszValueName, pData, nType, nDataSize);

	// ... Clean up copy of data
	delete[] pData;

	// ... Possible error condition has already been traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::SetMultiStringValue", m_nLastError);
	ASSERT_VALID(this);
	return SUCCEEDED(m_nLastError);
}

BOOL COXRegistryItem::SetMultiStringValue(const CStringArray& stringArray, 
										  LPCTSTR pszValueName /* = NULL */)
{
	ASSERT_VALID(this);

	CString sNewMultiStringValue;
	for(int nIndex = 0; nIndex < stringArray.GetSize(); nIndex++)
	{
		sNewMultiStringValue += stringArray.GetAt(nIndex);
		if (nIndex<stringArray.GetUpperBound())
			sNewMultiStringValue += m_pszBar;
	}

	// ... Possible error condition will already be traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::SetMultiStringValue", m_nLastError);

	ASSERT_VALID(this);
	return SetMultiStringValue(sNewMultiStringValue, pszValueName);
}

BOOL COXRegistryItem::SetNumberValue(DWORD nNewNumberValue, 
									 LPCTSTR pszValueName /* = NULL */, BOOL bLittleEndian /* = TRUE */)
{
	ASSERT_VALID(this);

	DWORD nDataSize = sizeof(DWORD);
	const BYTE* pData = (const BYTE*)&nNewNumberValue;
	DWORD nType = bLittleEndian ? REG_DWORD_LITTLE_ENDIAN : REG_DWORD_BIG_ENDIAN;

	SetValue(pszValueName, pData, nType, nDataSize);

	// ... Possible error condition has already been traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::SetNumberValue", m_nLastError);
	ASSERT_VALID(this);
	return SUCCEEDED(m_nLastError);
}

BOOL COXRegistryItem::SetBinaryValue(const CByteArray& newBinaryValue, 
									 LPCTSTR pszValueName /* = NULL */)
{
	ASSERT_VALID(this);

	DWORD nDataSize = (DWORD)newBinaryValue.GetSize();
	const BYTE* pData = newBinaryValue.GetData();;
	DWORD nType = REG_BINARY;

	SetValue(pszValueName, pData, nType, nDataSize);

	// ... Possible error condition has already been traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::SetBinaryValue", m_nLastError);
	ASSERT_VALID(this);
	return SUCCEEDED(m_nLastError);
}

DWORD COXRegistryItem::GetValueSize(LPCTSTR pszValueName /* = NULL */)
{
	ASSERT_VALID(this);

	DWORD nDataSize = 0;
	if (GetValue(NULL, pszValueName, 0, NULL, &nDataSize))
		return nDataSize;
	else
		return 0;

	// ... Possible error condition has already been traced
}

DWORD COXRegistryItem::GetValueType(LPCTSTR pszValueName /* = NULL */)
{
	ASSERT_VALID(this);

	DWORD nType = 0;
	if (GetValue(NULL, pszValueName, 0, &nType))
		return nType;
	else
		return REG_NONE;

	// ... Possible error condition has already been traced
}

BOOL COXRegistryItem::GetValue(LPBYTE* ppData, LPCTSTR pszValueName /* = NULL */, 
							   DWORD nRequestedType /* = 0 */,	LPDWORD pnType /* = NULL */, 
							   LPDWORD pnSize /* = NULL */)
{
	ASSERT_VALID(this);

	m_nLastError = ERROR_SUCCESS;

	// Get the name of the value
	CString sValueName = pszValueName;
	if (sValueName.IsEmpty())
		sValueName = GetValueName();
	// ... Use an empty string if the NoName constant is used
	if (sValueName.CompareNoCase(m_pszNoName) == 0)
		sValueName.Empty();

	LPBYTE pData = NULL;
	DWORD nType = 0;
	DWORD nDataSize = 0;
	// ... Open key if not yet open (first try to open with maximum
	//     access, then only with KEY_QUERY_VALUE to get the value)
	if (IsOpen() || Open(FALSE) || Open(FALSE, KEY_QUERY_VALUE))
	{
		// ... Get the value type and the needed buffer size
		m_nLastError = HResultFromWin32(::RegQueryValueEx(m_hKey, 
			sValueName.GetBuffer(0), NULL, &nType, pData, &nDataSize));
		sValueName.ReleaseBuffer();

		if (SUCCEEDED(m_nLastError) && (nRequestedType != 0) && 
			(nRequestedType != nType))
		{
			// ... Data type does not match
			m_nLastError = HResultFromWin32(ERROR_INVALID_DATA);
		}

		// Get the actual data if it is requested
		if (SUCCEEDED(m_nLastError) && (ppData != NULL) && nDataSize>0)
		{
			// ... Get the actual data
			pData = new BYTE[nDataSize];
			m_nLastError = HResultFromWin32(::RegQueryValueEx(m_hKey, 
				sValueName.GetBuffer(0), NULL, &nType, pData, &nDataSize));
			sValueName.ReleaseBuffer();

			// ... Discard the data if the call failed
			if (FAILED(m_nLastError))
			{
				delete[] pData;
				pData = NULL;
			}
		}
	}

	// Assign the result values
	if (ppData != NULL)
		*ppData = pData;
	if (pnType != NULL)
		*pnType = nType;
	if (pnSize != NULL)
		*pnSize = nDataSize;

#ifndef OX_REGITEM_DONTTRACE
	CONDITIONAL_TRACE_RESULT("COXRegistryItem::GetValue", m_nLastError);
#endif
	ASSERT_VALID(this);
	return SUCCEEDED(m_nLastError);
}

BOOL COXRegistryItem::SetValue(LPCTSTR pszValueName, const BYTE* pData,
							   DWORD nType, DWORD nSize)
{
	ASSERT_VALID(this);

	m_nLastError = ERROR_SUCCESS;

	// Get the name of the value
	CString sValueName = pszValueName;
	if (sValueName.IsEmpty())
		sValueName = GetValueName();
	// ... Use an empty string if the NoName constant is used
	if (sValueName.CompareNoCase(m_pszNoName) == 0)
		sValueName.Empty();

	// ... Open key if not yet open (first try to open with maximum
	//     access, then only with KEY_SET_VALUE to set the value)
	// ... A key will be created if necessary
	if (IsOpen() || Open(TRUE) || Open(TRUE, KEY_SET_VALUE))
	{
		// ... Get the value type and the needed buffer size
		m_nLastError = HResultFromWin32(::RegSetValueEx(m_hKey, sValueName,
			0, nType, pData, nSize));
	}

#ifndef OX_REGITEM_DONTTRACE
	CONDITIONAL_TRACE_RESULT("COXRegistryItem::SetValue", m_nLastError);
#endif
	ASSERT_VALID(this);
	return SUCCEEDED(m_nLastError);
}

CString COXRegistryItem::ExpandString(LPCTSTR pszUnexpanded)
{
	DWORD nNeededSize = 0;
	nNeededSize = ::ExpandEnvironmentStrings(pszUnexpanded, NULL, nNeededSize);
	if (0 < nNeededSize)
	{
		CString sResult;
		TCHAR* buffer=new TCHAR[nNeededSize];
		// ... Note that the length in GetBufferSetLength should not include the terminating null char
		if (nNeededSize>=
			::ExpandEnvironmentStrings(pszUnexpanded,buffer,nNeededSize))
			sResult=buffer;
		else
		{
			TRACE1("Error %d in COXRegistryItem::ExpandString()\r",::GetLastError());
			sResult=_T("");
		}
		delete[] buffer;
		return sResult;
	}
	else
	{
		// Error or empty string
		return pszUnexpanded;
	}
}

BOOL COXRegistryItem::GetInfo(LPTSTR pClass, LPDWORD pcbClass /* = NULL */, 
							  LPDWORD pcSubKeys /* = NULL */,	LPDWORD pcbMaxSubKeyLen /* = NULL */, 
							  LPDWORD pcbMaxClassLen /* = NULL */, LPDWORD pcValues /* = NULL */, 
							  LPDWORD pcbMaxValueNameLen /* = NULL */, LPDWORD pcbMaxValueLen /* = NULL */, 
							  LPDWORD pcbSecurityDescriptor /* = NULL */,	PFILETIME lpftLastWriteTime /* = NULL */)
{
	ASSERT_VALID(this);

	// ... Open key if not yet open (first try to open with maximum
	//     access, then only with KEY_QUERY_VALUE to query the value)
	// ... A key will be created if necessary
	if (IsOpen() || Open(FALSE) || Open(FALSE, KEY_QUERY_VALUE))
	{
		// ... Get the value type and the needed buffer size
		m_nLastError = HResultFromWin32(::RegQueryInfoKey(m_hKey, pClass,
			pcbClass, NULL, pcSubKeys, pcbMaxSubKeyLen, pcbMaxClassLen, pcValues, 
			pcbMaxValueNameLen, pcbMaxValueLen, pcbSecurityDescriptor,
			lpftLastWriteTime));
	}

	// Do not TRACE a message when requesting more buffer space
#ifndef OX_REGITEM_DONTTRACE
	if (HRESULT_CODE(m_nLastError) != ERROR_MORE_DATA)
		CONDITIONAL_TRACE_RESULT("COXRegistryItem::GetInfo", m_nLastError);
#endif

	ASSERT_VALID(this);
	return SUCCEEDED(m_nLastError);
}

CString COXRegistryItem::GetClassName()
{
	ASSERT_VALID(this);

	CString sClassName;
	DWORD nClassNameLength = 0;

	GetInfo(NULL, &nClassNameLength);
	if (HRESULT_CODE(m_nLastError) == ERROR_MORE_DATA)
		GetInfo(sClassName.GetBufferSetLength(nClassNameLength));

	// ... Possible error condition has already been traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::GetClassName", m_nLastError);
	ASSERT_VALID(this);
	if (SUCCEEDED(m_nLastError))
		return sClassName;
	else
		return _T("");
}

int COXRegistryItem::GetNumberOfSubkeys()
{
	ASSERT_VALID(this);

	DWORD nNumberOfSubkeys = 0;
	GetInfo(NULL, NULL, &nNumberOfSubkeys);

	// ... Possible error condition has already been traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::GetNumberOfSubkeys", m_nLastError);
	ASSERT_VALID(this);
	if (SUCCEEDED(m_nLastError))
		return (int)nNumberOfSubkeys;
	else
		return -1;
}

int COXRegistryItem::GetLongestSubkeyNameLength()
{
	ASSERT_VALID(this);

	DWORD nLongestSubkeyNameLength = 0;
	GetInfo(NULL, NULL, NULL, &nLongestSubkeyNameLength);

	// ... Possible error condition has already been traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::GetLongestSubkeyNameLength", m_nLastError);
	ASSERT_VALID(this);
	if (SUCCEEDED(m_nLastError))
		return (int)nLongestSubkeyNameLength;
	else
		return -1;
}

int COXRegistryItem::GetLongestClassNameLength()
{
	ASSERT_VALID(this);

	DWORD nLongestClassNameLength = 0;
	GetInfo(NULL, NULL, NULL, NULL, &nLongestClassNameLength);

	// ... Possible error condition has already been traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::GetLongestClassNameLength", m_nLastError);
	ASSERT_VALID(this);
	if (SUCCEEDED(m_nLastError))
		return (int)nLongestClassNameLength;
	else
		return -1;
}

int COXRegistryItem::GetNumberOfValues()
{
	ASSERT_VALID(this);

	DWORD nNumberOfValues = 0;
	GetInfo(NULL, NULL, NULL, NULL, NULL, &nNumberOfValues);

	// ... Possible error condition has already been traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::GetNumberOfValues", m_nLastError);
	ASSERT_VALID(this);
	if (SUCCEEDED(m_nLastError))
		return (int)nNumberOfValues;
	else
		return -1;
}

int COXRegistryItem::GetLongestValueNameLength()
{
	ASSERT_VALID(this);

	DWORD nLongestValueNameLength = 0;
	GetInfo(NULL, NULL, NULL, NULL, NULL, NULL, &nLongestValueNameLength);

	// ... Possible error condition has already been traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::GetLongestValueNameLength", m_nLastError);
	ASSERT_VALID(this);
	if (SUCCEEDED(m_nLastError))
		return (int)nLongestValueNameLength;
	else
		return -1;
}

int COXRegistryItem::GetLongestValueDataLength()
{
	ASSERT_VALID(this);

	DWORD nLongestValueDataLength = 0;
	GetInfo(NULL, NULL, NULL, NULL, NULL, NULL, NULL, &nLongestValueDataLength);

	// ... Possible error condition has already been traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::GetLongestValueDataLength", m_nLastError);
	ASSERT_VALID(this);
	if (SUCCEEDED(m_nLastError))
		return (int)nLongestValueDataLength;
	else
		return -1;
}

int COXRegistryItem::GetSecurityDescriptorLength()
{
	ASSERT_VALID(this);

	DWORD nSecurityDescriptorLength = 0;
	GetInfo(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &nSecurityDescriptorLength);

	// ... Possible error condition has already been traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::GetSecurityDescriptorLength", m_nLastError);
	ASSERT_VALID(this);
	if (SUCCEEDED(m_nLastError))
		return (int)nSecurityDescriptorLength;
	else
		return -1;
}

CTime COXRegistryItem::GetLastWriteTime()
{
	ASSERT_VALID(this);

	FILETIME lastWriteTime;
	GetInfo(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &lastWriteTime);

	// ... Possible error condition has already been traced
	// CONDITIONAL_TRACE_RESULT("COXRegistryItem::GetLastWriteTime", m_nLastError);
	ASSERT_VALID(this);
	if (SUCCEEDED(m_nLastError))
		return CTime(lastWriteTime);
	else
		return CTime();
}

BOOL COXRegistryItem::NotifyChange(BOOL bWatchSubtree /* = FALSE */, 
								   DWORD dwNotifyFilter /* = REG_NOTIFY_CHANGE_NAME */, HANDLE hEvent /* = NULL */)
{
	ASSERT_VALID(this);

	m_nLastError = ERROR_SUCCESS;

	// ... Open key if not yet open (first try to open with maximum
	//     access, then only with KEY_NOTIFY to get notifications)
	if (IsOpen() || Open(FALSE) || Open(FALSE, KEY_NOTIFY))
	{
		// ... Get the value type and the needed buffer size
		m_nLastError = HResultFromWin32(::RegNotifyChangeKeyValue(m_hKey, 
			bWatchSubtree, dwNotifyFilter, hEvent, (hEvent != NULL)));
	}

#ifndef OX_REGITEM_DONTTRACE
	CONDITIONAL_TRACE_RESULT("COXRegistryItem::NotifyChange", m_nLastError);
#endif
	ASSERT_VALID(this);
	return SUCCEEDED(m_nLastError);
}

CString COXRegistryItem::EnumerateSubkey(DWORD nIndex)
{
	ASSERT_VALID(this);

	m_nLastError = ERROR_SUCCESS;
	CString sSubkeyName;

	// ... Open key if not yet open (first try to open with maximum
	//     access, then only with KEY_ENUMERATE_SUB_KEYS to enumerate)
	if (IsOpen() || Open(FALSE) || Open(FALSE, KEY_ENUMERATE_SUB_KEYS))
	{
		// ... Start with a buffer that will satisfy most requests
		DWORD nSubkeyNameLength = 300;
		m_nLastError = HResultFromWin32(::RegEnumKeyEx(m_hKey, nIndex, 
			sSubkeyName.GetBuffer(nSubkeyNameLength), &nSubkeyNameLength, NULL, NULL, NULL, NULL));

		if (HRESULT_CODE(m_nLastError) == ERROR_MORE_DATA)
		{
			// Buffer was not large enough, try again with correct size
			sSubkeyName.ReleaseBuffer(0);
			// ... Note that the next function may fail because it is possible 
			//      that the key was not opened with KEY_QUERY_VALUE.
			//     To resolve this open the key explicitly with KEY_QUERY_VALUE.
			nSubkeyNameLength = GetLongestSubkeyNameLength() + 1;
			m_nLastError = HResultFromWin32(::RegEnumKeyEx(m_hKey, nIndex, 
				sSubkeyName.GetBuffer(nSubkeyNameLength), &nSubkeyNameLength, NULL, NULL, NULL, NULL));
		}

		if (SUCCEEDED(m_nLastError))
			sSubkeyName.ReleaseBuffer();
		else
			sSubkeyName.ReleaseBuffer(0);
	}

	// ... Do not trace for end of iteration (expected condition)
#ifndef OX_REGITEM_DONTTRACE
	if (HRESULT_CODE(m_nLastError) != ERROR_NO_MORE_ITEMS)
		CONDITIONAL_TRACE_RESULT("COXRegistryItem::EnumerateSubkey", m_nLastError);
#endif

	ASSERT_VALID(this);
	if (SUCCEEDED(m_nLastError))
		return sSubkeyName;
	else
		return _T("");
}

CString COXRegistryItem::EnumerateValue(DWORD nIndex)
{
	ASSERT_VALID(this);

	m_nLastError = ERROR_SUCCESS;
	CString sValueName;

	// ... Open key if not yet open (first try to open with maximum
	//     access, then only with KEY_ENUMERATE_SUB_KEYS  and KEY_QUERY_VALUE to enumerate)
	if (IsOpen() || Open(FALSE) || Open(FALSE, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE))
	{
		// ... Start with a buffer that will satisfy most requests
		DWORD nValueNameLength = 300;
		m_nLastError = HResultFromWin32(::RegEnumValue(m_hKey, nIndex, 
			sValueName.GetBuffer(nValueNameLength), &nValueNameLength, NULL, NULL, NULL, NULL));

		if (HRESULT_CODE(m_nLastError) == ERROR_MORE_DATA)
		{
			// Buffer was not large enough, try again with correct size
			sValueName.ReleaseBuffer(0);
			nValueNameLength = GetLongestValueNameLength() + 1;
			m_nLastError = HResultFromWin32(::RegEnumValue(m_hKey, nIndex, 
				sValueName.GetBuffer(nValueNameLength), &nValueNameLength, NULL, NULL, NULL, NULL));
		}

		if (SUCCEEDED(m_nLastError))
		{
			sValueName.ReleaseBuffer();
			// ... Replace an empty value name by the NoName constant
			if (sValueName.IsEmpty())
				sValueName = m_pszNoName;
		}
		else
			sValueName.ReleaseBuffer(0);
	}

	// ... Do not trace for end of iteration (expected condition)
#ifndef OX_REGITEM_DONTTRACE
	if (HRESULT_CODE(m_nLastError) != ERROR_NO_MORE_ITEMS)
		CONDITIONAL_TRACE_RESULT("COXRegistryItem::EnumerateValue", m_nLastError);
#endif
	ASSERT_VALID(this);
	if (SUCCEEDED(m_nLastError))
		return sValueName;
	else
		return _T("");
}

BOOL COXRegistryItem::GetSortedValueList(CStringArray& arrValueName)
{
	// delete all existing elements
	arrValueName.RemoveAll();

	// find out how many values we'll be dealing with
	int nCount=GetNumberOfValues();
	// enumerate the key's values using existing function
	CString sValueName;
	for (int nIndex=0; nIndex<nCount; nIndex++)
	{
		sValueName=EnumerateValue(nIndex);

		if(!SUCCEEDED(GetLastError()))
			return FALSE;

		int nInsertPos= (int)arrValueName.GetSize();
		if(sValueName==_T("NoName"))
			nInsertPos=0;
		else
		{
			CString sNameToCompare;
			for(int nArrayIndex=0; nArrayIndex<arrValueName.GetSize(); nArrayIndex++)
			{
				sNameToCompare=arrValueName[nArrayIndex];
				if(sValueName.CompareNoCase(sNameToCompare)<1)
				{
					nInsertPos=nArrayIndex;
				}
			}
		}

		arrValueName.InsertAt(nInsertPos,sValueName);
	}

	return TRUE;
}

BOOL COXRegistryItem::IsDefaultValueSet()
{
	BOOL bDefaultExist=TRUE;
	// specifting NULL as first parameter you will request default value
	GetStringValue(NULL);
	if(!SUCCEEDED(GetLastError()) && HRESULT_CODE(GetLastError())==2)
		bDefaultExist=FALSE;

	return bDefaultExist;
}

#ifdef _DEBUG
void COXRegistryItem::AssertValid() const
{
	// ... First call base class implementation
	CObject::AssertValid();

	// Both key handles (root key and current registry key) should be
	// both NULL or both not NULL
	ASSERT( ((m_hRootKey == NULL) && (m_hKey == NULL)) ||
		((m_hRootKey != NULL) && (m_hKey != NULL)) );

}

void COXRegistryItem::Dump(CDumpContext& dc) const
{
	// ... First call base class implementation
	CObject::Dump(dc);

	// Then do our thing
	dc << "\nm_sFullRegistryItem = " << m_sFullRegistryItem;
	dc << "\nm_bMainBuilt = " << m_bMainBuilt;

	dc << "\nm_sMachineName = " << m_sMachineName;
	dc << "\nm_sRegistryName = " << m_sRegistryName;
	dc << "\nm_sKeyNames = " << m_sKeyNames;
	dc << "\nm_sValueName = " << m_sValueName;
	dc << "\nm_bPartsBuilt = " << m_bPartsBuilt;

	dc << "\nm_hRootKey = " << (void*)m_hRootKey;
	dc << "\nm_hKey = " << (void*)m_hKey;
	dc << "\nm_nLastError = " << m_nLastError;

	dc << "\n";
}
#endif //_DEBUG

COXRegistryItem::~COXRegistryItem()
{
	// ... Close a possible open key before destruction
	Close();
}

// protected:
void COXRegistryItem::BuildMain()
// --- In  :
// --- Out : 
// --- Returns :
// --- Effect : Builds the main registry item from the parts
{
	ASSERT_VALID(this);

	if (IsMainBuilt())
	{
		// ... Nothing to do
		return;
	}

	if (!ArePartsBuilt())
	{
		// Nothing has been built yet, clean everything and set to built
		Empty();
		SetMainBuilt();
		SetPartsBuilt();

		ASSERT_VALID(this);
		return;
	}

	// Build main data
	m_sFullRegistryItem = m_sMachineName + m_sRegistryName + m_sKeyNames + m_sValueName;

	// ... Adjust main after we have built it
	AdjustMain();
	SetMainBuilt();

	ASSERT_VALID(this);
	return;
}

void COXRegistryItem::BuildParts()
// --- In  :
// --- Out : 
// --- Returns :
// --- Effect : Builds the different parts from the full registry item
{
	ASSERT_VALID(this);
	if (ArePartsBuilt())
	{
		// ... Nothing to do
		return;
	}

	if (!IsMainBuilt())
	{
		// Nothing has been built yet, clean everything and set to built
		Empty();
		SetMainBuilt();
		SetPartsBuilt();
		ASSERT_VALID(this);
		return;
	}

	// Build the parts

	// All parts are empty by default
	m_sMachineName.Empty();
	m_sRegistryName.Empty();
	m_sKeyNames.Empty();
	m_sValueName.Empty();

	// ... The part that still has to be parsed
	LPCTSTR pszRegistryItemParts = m_sFullRegistryItem;

	// ... First character is back slash or terminating zero-character 
	ASSERT((*pszRegistryItemParts == m_cNull) || (*pszRegistryItemParts == m_cBackslash));

	// If the full registry items starts with two backslashes, a computer name
	// is specified (in front of but not including of the first single back slash)
	if ((pszRegistryItemParts[0] == m_cBackslash) && 
		(pszRegistryItemParts[1] == m_cBackslash))
	{
		LPCTSTR pszRest = _tcschr(pszRegistryItemParts + 2, m_cBackslash);
		if (pszRest != NULL)
		{
			// ... Subtract the machine name
			int nMachineNameLength = (int) (pszRest - pszRegistryItemParts);
			UTBStr::tcsncpy(m_sMachineName.GetBufferSetLength(nMachineNameLength), nMachineNameLength + 1,
				pszRegistryItemParts, nMachineNameLength);
			pszRegistryItemParts = pszRest;
		}
		else
		{
			// ... Machine name is all the rest
			m_sMachineName = pszRegistryItemParts;
			// ... Let the rest point to zero-terminating char
			pszRegistryItemParts = pszRegistryItemParts + _tcslen(pszRegistryItemParts);
		}
	}

	// ... First character is back slash or terminating zero-character 
	ASSERT((*pszRegistryItemParts == m_cNull) || (*pszRegistryItemParts == m_cBackslash));

	// Get the registry name (next part)
	// (between the first (including) and the second back slash (not including)
	// The only possible values are the those in the m_rgRootRegistryKey array.
	int nRegistryIndex = GetRegistryIndex(pszRegistryItemParts);
	if (0 <= nRegistryIndex)
	{
		ASSERT(pszRegistryItemParts[0] == m_cBackslash);
		LPCTSTR pszRest = _tcschr(pszRegistryItemParts + 1, m_cBackslash);
		if (pszRest != NULL)
		{
			// ... Subtract the registry name
			int nRegistryNameLength = (int)(pszRest - pszRegistryItemParts);
			UTBStr::tcsncpy(m_sRegistryName.GetBufferSetLength(nRegistryNameLength), nRegistryNameLength + 1,
				pszRegistryItemParts, nRegistryNameLength);
			pszRegistryItemParts = pszRest;
		}
		else
		{
			// ... Registry name is all the rest
			m_sRegistryName = pszRegistryItemParts;
			// ... Let the rest point to zero-terminating char
			pszRegistryItemParts = pszRegistryItemParts + _tcslen(pszRegistryItemParts);
		}
	}

	// ... First character is back slash or terminating zero-character 
	ASSERT((*pszRegistryItemParts == m_cNull) || (*pszRegistryItemParts == m_cBackslash));

	// Get the key names (next parts)
	// (between the first and the last back slash (both including)
	if (pszRegistryItemParts[0] == m_cBackslash)
	{
		LPCTSTR pszRest = _tcsrchr(pszRegistryItemParts + 1, m_cBackslash);
		if (pszRest != NULL)
		{
			// ... Include terminating back slash
			pszRest++;
			// ... Subtract the registry name
			int nKeyNamesLength = (int)(pszRest - pszRegistryItemParts);
			UTBStr::tcsncpy(m_sKeyNames.GetBufferSetLength(nKeyNamesLength), nKeyNamesLength + 1,
				pszRegistryItemParts, nKeyNamesLength);
			pszRegistryItemParts = pszRest;
		}
		else
		{
			// ... Registry name is all the rest
			m_sKeyNames = pszRegistryItemParts;
			// ... Add terminating backslash if necessary
			if (pszRegistryItemParts[_tcslen(pszRegistryItemParts) - 1] != m_cBackslash)
				m_sKeyNames += m_cBackslash;
			// ... Let the rest point to zero-terminating char
			pszRegistryItemParts = pszRegistryItemParts + _tcslen(pszRegistryItemParts);
		}
	}

	// Get the key names (next parts)
	// (Everything that is still not parsed)
	if (pszRegistryItemParts[0] != m_cNull)
	{
		// ... Cannot include a back slash
		ASSERT(_tcsrchr(pszRegistryItemParts, m_cBackslash) == NULL);

		// ... Value name is all the rest
		m_sValueName = pszRegistryItemParts;
		// ... Let the rest point to zero-terminating char
		pszRegistryItemParts = pszRegistryItemParts + _tcslen(pszRegistryItemParts);
	}

	// Everything should be parsed (nothing left)
	ASSERT(*pszRegistryItemParts == m_cNull);

	// ... Adjust parts after we have built them
	AdjustParts();
	SetPartsBuilt();

	// ... Rebuild main
	DestroyMain();
	BuildMain();

	ASSERT_VALID(this);
	return;
}

void COXRegistryItem::AdjustMain()
// --- In  :
// --- Out : 
// --- Returns :
// --- Effect : Adjust the main (full registry item)
{
	if (!m_sFullRegistryItem.IsEmpty() && (m_sFullRegistryItem.Left(1) != CString(m_cBackslash)))
		// ... If full registry item is not empty it should start with backslash
		m_sFullRegistryItem = m_cBackslash + m_sFullRegistryItem;
	if (_tcsspnp(m_sFullRegistryItem, m_pszBackslash) == NULL)
		// ... Full registry item only contains backslashes : clear it
		m_sFullRegistryItem.Empty();
}

void COXRegistryItem::AdjustParts()
// --- In  :
// --- Out : 
// --- Returns :
// --- Effect : Adjust the registry parts 
{
	// Machine name
	if (!m_sMachineName.IsEmpty())
	{
		// ... If not empty it should start with two backslashes
		if (m_sMachineName.Left(1) != CString(m_cBackslash))
			m_sMachineName = m_cBackslash + m_sMachineName;
		if (m_sMachineName.Left(2) != m_pszDoubleBackslash)
			m_sMachineName = m_cBackslash + m_sMachineName;
	}
	if (_tcsspnp(m_sMachineName, m_pszBackslash) == NULL)
		// ... Machine name only contains backslashes : clear it
		m_sMachineName.Empty();

	// Registry name
	if (!m_sRegistryName.IsEmpty())
	{
		// ... If not empty it should start with a backslashes
		if (m_sRegistryName.Left(1) != CString(m_cBackslash))
			m_sRegistryName = m_cBackslash + m_sRegistryName;
	}
	if (_tcsspnp(m_sRegistryName, m_pszBackslash) == NULL)
		// ... Registry name only contains backslashes : clear it
		m_sRegistryName.Empty();

	// Key names
	if (!m_sKeyNames.IsEmpty())
	{
		// ... If not empty it should start with a backslashes
		if (m_sKeyNames.Left(1) != CString(m_cBackslash))
			m_sKeyNames = m_cBackslash + m_sKeyNames;
		// ... If not empty it should end with a backslashes
		if (m_sKeyNames.Right(1) != CString(m_cBackslash))
			m_sKeyNames += m_cBackslash;
	}
	if (_tcsspnp(m_sKeyNames, m_pszBackslash) == NULL)
		// ... Key names name only contains backslashes : clear it
		m_sKeyNames.Empty();

	// Value name
	if (!m_sValueName.IsEmpty())
	{
		// ... It shoulkd not contain a backslash
	}

	// Registry and Key names
	if (!m_sKeyNames.IsEmpty() && m_sRegistryName.IsEmpty())
		// ... If key is specified without registry use default (CurrentUser)
		m_sRegistryName = m_pszCurrentUser;

}

BOOL COXRegistryItem::OpenRootKey()
// --- In  :
// --- Out : 
// --- Returns : Whether the operation succeeded or not
// --- Effect : Opens the root key (may connect to remote computer)
{
	HRESULT nLastError = ERROR_SUCCESS;

	// Get the root key (default to CurrentUser)
	HKEY hRootKey = HKEY_CURRENT_USER;
	int nRegistryIndex = GetRegistryIndex(GetRegistryName());
	if (0 <= nRegistryIndex)
		hRootKey = m_rgRootRegistryKey[nRegistryIndex].m_hRegistryKey;

	// Connect to a remote computer if necessary
	if (!GetMachineName().IsEmpty())
	{
		HKEY hRemoteRootKey = NULL;
		CString sMachineName = GetMachineName();
		nLastError = HResultFromWin32(::RegConnectRegistry(sMachineName.GetBuffer(0),
			hRootKey, &hRemoteRootKey));
		sMachineName.ReleaseBuffer();
		if (SUCCEEDED(nLastError))
			hRootKey = hRemoteRootKey;
		else
		{
			hRootKey = NULL;
		}
	}

	// Assign the result
	// ... Should have been closed previously
	ASSERT(m_hRootKey == NULL);
	m_hRootKey = hRootKey;

	// Record last error when failed
	if (FAILED(nLastError))
		m_nLastError = nLastError;

#ifndef OX_REGITEM_DONTTRACE
	CONDITIONAL_TRACE_RESULT("COXRegistryItem::OpenRootKey", nLastError);
#endif
	return SUCCEEDED(nLastError);
}

void COXRegistryItem::CloseKey()
// --- In  :
// --- Out : 
// --- Returns : 
// --- Effect : Closes the registry key if it still open (root key is not changed)
{
	if (m_hKey != NULL)
		m_nLastError = HResultFromWin32(::RegCloseKey(m_hKey));
	m_hKey = NULL;
}

void COXRegistryItem::CloseRootKey()
// --- In  :
// --- Out : 
// --- Returns : 
// --- Effect : Closes the root key if it still open (registry key is not changed)
{
	if (m_hRootKey != NULL)
		m_nLastError = HResultFromWin32(::RegCloseKey(m_hRootKey));
	m_hRootKey = NULL;
}

int COXRegistryItem::GetRegistryIndex(LPCTSTR pszRegistryName)
// --- In  : pszRegistryName : String that begins with a registry name
// --- Out : 
// --- Returns : The index of the regsitry or -1 when no was found
// --- Effect : 
{
	int nRootRegistryIndex = -1;

	// Search for a matching name in the array of possible values
	int nRegistryNameLength = 0;
	int nRegistryIndex = 0;
	COXRootRegistryKey* pRootRegistryKey = m_rgRootRegistryKey;
	while ((nRootRegistryIndex == -1) && 
		(pRootRegistryKey->m_pszRegistryName != NULL))
	{
		nRegistryNameLength = (int)_tcslen(pRootRegistryKey->m_pszRegistryName);
		if ((_tcsnicmp(pszRegistryName, pRootRegistryKey->m_pszRegistryName, 
			nRegistryNameLength) == 0) && 
			((pszRegistryName[nRegistryNameLength] == m_cBackslash) ||
			(pszRegistryName[nRegistryNameLength] == m_cNull)) )
			nRootRegistryIndex = nRegistryIndex;
		pRootRegistryKey++;
		nRegistryIndex++;
	}

	return nRootRegistryIndex;
}

void COXRegistryItem::NullToBarSeparator(LPTSTR pStringData)
// --- In  : pStringData : Pointer to string with parts seperated by null chars
//							and concluded by two null chars
// --- Out : pStringData : The same string but bars '|' used to seperate the parts
//						   The string still end in a null char (without a bar)
// --- Returns :
// --- Effect : Converts the string
{
	BOOL bPreviousWasNull = FALSE;
	BOOL bEnd = FALSE;

	while (!bEnd)
	{
		// ... String pointer should point to a valid string att all times
		ASSERT(AfxIsValidString(pStringData));
		if (*pStringData == m_cNull)
		{
			if (bPreviousWasNull)
			{
				// ... This char is NULL and so was the previous :
				//     zero-terminate the string and end the loop
				*(pStringData - 1) = m_cNull;
				bEnd = TRUE;
			}
			else
			{
				// ... This char is NULL but the previous was not:
				//     replace the zero-char by a bar
				*pStringData = m_cBar;
				bPreviousWasNull = TRUE;
			}
		}
		else
			// ... Encountered a normal (non-NULL-char)
			bPreviousWasNull = FALSE;

		pStringData++;
	}
}

void COXRegistryItem::BarToNullSeparator(LPTSTR pStringData)
// --- In  : pStringData : A string with bars '|' used to seperate the parts
//						   The string end in a null char (without a bar)
// --- Out : pStringData : The same string but with parts seperated by null chars
//						   and concluded by two null chars
// --- Returns :
// --- Effect : Converts the string
//				Note that the string will grow with one character.
//				Enough space should have been allocated before callinbg this function
{
	// ... The pStringData should have enough space one additional (null) characters
	ASSERT(AfxIsValidAddress(pStringData, (_tcslen(pStringData) + 2) * sizeof(TCHAR)));

	// First add an extra NULL character at the end
	*(pStringData + _tcslen(pStringData) + 1) = m_cNull;

	// Now replace all bars by a NULL character (use _tcstok)
	TCHAR * p;
	LPCTSTR pNewStringData = UTBStr::tcstok(pStringData, m_pszBar, &p);
	while(pNewStringData != NULL)
	{
		pNewStringData = UTBStr::tcstok(NULL, m_pszBar, &p);
	}
}

// protected:
#ifdef _DEBUG

static TCHAR szUnknownError[] = _T("*** Unknown Error ***");
static DWORD dwLangID = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT); 

CString COXRegistryItem::GetResultMessage(HRESULT hResult)
// --- In  : hResult : The result code
// --- Out : 
// --- Returns : A string containg a message of the specified code
// --- Effect : 
{
	CString sResultMessage;
	LPTSTR pszMsgBuf = NULL;
	BOOL bUnknown = FALSE;
	DWORD dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM;

	// ... Remove the facility part if FACILITY_WIN32
	if (HRESULT_FACILITY(hResult) == FACILITY_WIN32)
		hResult = HRESULT_CODE(hResult);

	// ... Get the actual message 
	if (::FormatMessage(dwFlags, NULL, hResult, dwLangID,
		(LPTSTR)&pszMsgBuf, 0, NULL) == 0)
	{
		TRACE2("COXRegistryItem::GetResultMessage : No message was found for result code %i == 0x%8.8X\n",
			hResult, hResult);
		//pszMsgBuf = szUnknownError;
		VERIFY(sResultMessage.LoadString(IDS_OX_REGITEMUNKERROR));
		bUnknown = TRUE;
	}
	else
		sResultMessage = pszMsgBuf;

	// ... Clean up
	if (!bUnknown)
		LocalFree(pszMsgBuf);

	return sResultMessage;
}
#endif //_DEBUG

// private:

// ==========================================================================
