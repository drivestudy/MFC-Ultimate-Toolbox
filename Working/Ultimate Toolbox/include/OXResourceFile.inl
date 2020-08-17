// ==========================================================================
// 					Inline functions : COXResourceFile
// ==========================================================================
//
// Inline file : OXResourceFile.inl
// Source : Written by: Haiping Zhao
// Version: 9.3

#include "UTB64Bit.h"

inline BOOL COXResourceFile::Open(UINT nOpenFlags, COXResourceLibrary* pResLib, 
	BOOL bAutoDelete, LPCTSTR pszType, UINT nID, 
	WORD nLanguage /* = OXRESOURCE_DEFLANGID */, BOOL bMakeUniqueName /* = FALSE */)
	{
	return Open(nOpenFlags, pResLib, bAutoDelete, pszType, MAKEINTRESOURCE(nID),
		nLanguage, bMakeUniqueName);
	}

inline BOOL COXResourceFile::SetResName(WORD nID)
	{
	m_sResName = OXResIntToCString((WORD)nID); return TRUE;
	}

inline BOOL COXResourceFile::SetResType(LPCTSTR pszType)
	{
	m_sResType = ValidateResString(pszType); return TRUE;
	}

inline BOOL COXResourceFile::SetResName(LPCTSTR pszName)
	{
	m_sResName = ValidateResString(pszName); return TRUE;
	}

inline BOOL COXResourceFile::SetResLanguage(WORD nLanguage)
	{
	m_nResLanguage = nLanguage; return TRUE;
	}

inline BOOL COXResourceFile::IsAutoDeleteByLib() const
	{
	return m_bAutoDeleteByLib;
	}

inline CString COXResourceFile::GetResType() const
	{
	return m_sResType;
	}

inline CString COXResourceFile::GetResName() const
	{
	return m_sResName;
	}

inline WORD COXResourceFile::GetResLanguage() const
	{
	return m_nResLanguage;
	}

inline COXResourceLibrary* COXResourceFile::GetResourceLibrary() const
	{
	return m_pResLib;
	}

inline BOOL COXResourceFile::OXResCStringIsInt(LPCTSTR lpszCStringTypeOrName)
	{
	return (*lpszCStringTypeOrName == _T('#'));
	}

inline BOOL COXResourceFile::OXResItemIsInt(LPCTSTR lpszTypeOrName)
	{
	return !((DWORD)(DWORD_PTR)lpszTypeOrName & 0xFFFF0000);
	}

inline WORD COXResourceFile::OXResCStringToInt(LPCTSTR lpszCStringTypeOrName)
	{
	return OXResCStringIsInt(lpszCStringTypeOrName) ? 
		(WORD)OX_LPCTSTR2UL(lpszCStringTypeOrName + 1, NULL, 10) : (WORD)0;
	}

inline CString COXResourceFile::OXResCStringToString(LPCTSTR lpszCStringTypeOrName)
	{
	return OXResCStringIsInt(lpszCStringTypeOrName) ?
		CString(_T("")) : CString(lpszCStringTypeOrName);
	}

inline CString COXResourceFile::OXResItemToCString(LPCTSTR lpszTypeOrName)
	{
	return OXResItemIsInt(lpszTypeOrName) ?
		OXResIntToCString((WORD)(DWORD_PTR)lpszTypeOrName) : CString(lpszTypeOrName);
	}

inline WORD COXResourceFile::OXResItemToInt(LPCTSTR lpszTypeOrName)
	{
	return OXResItemIsInt(lpszTypeOrName) ? (WORD)(DWORD_PTR)lpszTypeOrName : (WORD)0;
	}

inline CString COXResourceFile::OXResItemToString(LPCTSTR lpszTypeOrName)
	{
	return OXResItemIsInt(lpszTypeOrName) ?  CString(_T("")) : CString(lpszTypeOrName);
	}

// end of OXResourceFile.inl