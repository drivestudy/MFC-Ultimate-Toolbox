// ==========================================================================
// 							Class Implementation : COXFileSpec
// ==========================================================================

// Source file : file.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "file.h"		// class specification

#include <direct.h>		// For directory functions (getdcwd(), ...)
#include <stdlib.h>		// For constant definitions  (_MAX_DIR, ...)
#include <string.h>		// For string search  (strspn, ...)

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#include "filelmt.h"	// for invalid chars

IMPLEMENT_SERIAL(COXFileSpec, CObject, 0)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members


// Data members -------------------------------------------------------------
// protected:

// private:
	// CString m_sFileName;
	// --- The file name of the file 
	//     (everything behind the last full stop is the extender-part)

	// CTime m_time;
	// --- Date and time of last modification
	// LONG m_lnLength;
	// --- File size in bytes
	// Attribute m_eAttributes;
	// --- DOS attributes (see enum Attribute of MFC)

// Member functions ---------------------------------------------------------
// public:

COXFileSpec::COXFileSpec()
	:
	m_sFileName(_T("")),
	m_time(),
	m_lnLength(0),
	m_eAttributes(CFile::normal)
	{
	}
	
COXFileSpec::COXFileSpec(LPCTSTR pszFileName)
	{
	// ... Must be valid pointer
	ASSERT(pszFileName != NULL);
	if (!SetFileName(pszFileName))
		{
		TRACE(_T("COXFileSpec::COXFileSpec : An invalid file name (%s) was specified, clearing object\n"),
			pszFileName);
		SetFileName(_T(""));
		}
	}
	
COXFileSpec::COXFileSpec(const COXFileSpec& fileSrc)
	:
	m_sFileName(fileSrc.m_sFileName),
	m_time(fileSrc.m_time),
	m_lnLength(fileSrc.m_lnLength),
	m_eAttributes(fileSrc.m_eAttributes)
	{
	}
	
COXFileSpec& COXFileSpec::operator=(const COXFileSpec& fileSrc)
{
	if(this==&fileSrc)
		return *this;
		
	m_sFileName = fileSrc.m_sFileName;
	return *this;
}
	
CString COXFileSpec::GetBaseName() const
	{
	int nFullStopPosition;
	if ((nFullStopPosition = m_sFileName.ReverseFind(_T('.'))) != -1)
		return m_sFileName.Left(nFullStopPosition);
	else
		return m_sFileName;
	}
	
BOOL COXFileSpec::SetBaseName(LPCTSTR pszBaseName)
	{
	// ... Must be valid pointer
	ASSERT(pszBaseName != NULL);
	CString sFileName(pszBaseName);
	CString sExtender(GetExtender());
#ifndef OX_FILEMNG_NOCHANGECASE
	sFileName.MakeLower();
#endif
	if ((int)_tcscspn(sFileName, INVALID_W_FILE_CHARS) != sFileName.GetLength())
		{
		TRACE(_T("COXFileSpec::SetBaseName : Basename (%s) contains illegal characters\n"), pszBaseName);
		return FALSE;
		}
	m_sFileName = sFileName;
	if (!sExtender.IsEmpty())
		m_sFileName += _T('.') + sExtender;
	return TRUE;
	}
	
void COXFileSpec::ForceSetBaseName(LPCTSTR pszBaseName)
	{
	// ... Must be valid pointer
	ASSERT(pszBaseName != NULL);
	CString sFileName(pszBaseName);
	CString sExtender(GetExtender());
	if (!sExtender.IsEmpty())
		sFileName = sFileName + _T('.') + sExtender;
	ForceSetFileName(sFileName);
	}
	
CString COXFileSpec::GetExtender() const
	{
	int nFullStopPosition;
	if ((nFullStopPosition = m_sFileName.ReverseFind(_T('.'))) != -1)
		return m_sFileName.Mid(nFullStopPosition + 1);
	else
		return _T("");
	}
	
BOOL COXFileSpec::SetExtender(LPCTSTR pszExtender)
	{
	// ... Must be valid pointer
	ASSERT(pszExtender != NULL);
	CString sFileName(GetBaseName());
	CString sExtender(pszExtender);
#ifndef OX_FILEMNG_NOCHANGECASE
	sExtender.MakeLower();
#endif
	if ((int)_tcscspn(sExtender, INVALID_W_FILE_CHARS) != sExtender.GetLength())
		{
		TRACE(_T("COXFileSpec::SetExtender : Extender (%s) contains illegal characters\n"), pszExtender);
		return FALSE;
		}
	if (!sExtender.IsEmpty())
		sFileName += _T('.') + sExtender;
	if (sFileName.GetLength() > MAX_FILE_NAME_LENGTH)
		{
		TRACE(_T("COXFileSpec::SetExtender : Setting extender would make file name too long (%)\n"), sFileName.GetLength());
		return FALSE;
		}
	m_sFileName = sFileName;
	return TRUE;
	}

void COXFileSpec::ForceSetExtender(LPCTSTR pszExtender)
	{
	// ... Must be valid pointer
	ASSERT(pszExtender != NULL);
	CString sFileName(GetBaseName());
	if (*pszExtender != _T('\0'))
		sFileName = sFileName + _T('.') + pszExtender;
	ForceSetFileName(sFileName);
	}
	
CString COXFileSpec::GetFileName() const
	{
	return m_sFileName;
	}
	
BOOL COXFileSpec::SetFileName(LPCTSTR pszFileName)
	{
	// ... Must be valid pointer
	ASSERT(pszFileName != NULL);
	if (_tcscspn(pszFileName, INVALID_W_FILE_CHARS) != _tcsclen(pszFileName))
		{
		TRACE(_T("COXFileSpec::SetFileName : File name (%s) contains illegal characters\n"), pszFileName);
		return FALSE;
		}
	if (_tcsclen(pszFileName) > MAX_FILE_NAME_LENGTH)
		{
		TRACE(_T("COXFileSpec::SetFileName : File name too long (%u)\n"), _tcsclen(pszFileName));
		return FALSE;
		}
	m_sFileName = pszFileName;
#ifndef OX_FILEMNG_NOCHANGECASE
	m_sFileName.MakeLower();
#endif
	return TRUE;
	}
	
void COXFileSpec::ForceSetFileName(LPCTSTR pszFileName)
	{
	// ... Must be valid pointer
	ASSERT(pszFileName != NULL);
	CString sFileName(pszFileName);
	int nIndexWrongChar;
	while ((nIndexWrongChar = (int)_tcscspn(sFileName, INVALID_W_FILE_CHARS)) != sFileName.GetLength())
		{
		sFileName = sFileName.Left(nIndexWrongChar) + sFileName.Mid(nIndexWrongChar + 1);
		}
	if (sFileName.GetLength() > MAX_FILE_NAME_LENGTH)
		sFileName = sFileName.Left(MAX_FILE_NAME_LENGTH);
#ifndef OX_FILEMNG_NOCHANGECASE
	sFileName.MakeLower();
#endif
	VERIFY(SetFileName(sFileName));
	}
	
CTime COXFileSpec::GetTime() const
	{
	return m_time;
	}
	
BOOL COXFileSpec::SetTime(CTime time)
	{
	m_time = time;
	return TRUE;
	}
	
LONG COXFileSpec::GetLength() const
	{
	return m_lnLength;
	}
	
BOOL COXFileSpec::SetLength(LONG lnLength)
	{
	if (0 <= lnLength)
		{
		m_lnLength = lnLength;
		return TRUE;
		}
	else
		{
		TRACE(_T("COXFileSpec::SetLength : Invalid length : %ln\n"), lnLength);
		return FALSE;
		}
	}
	
BYTE COXFileSpec::GetAttributes() const
	{
	return m_eAttributes;
	}
	
BOOL COXFileSpec::SetAttributes(CFile::Attribute attributes)
	{
	m_eAttributes = (BYTE)attributes;
	return TRUE;
	}

BOOL COXFileSpec::IsEmpty() const
	{
	return GetFileName().IsEmpty();
	}
	
void COXFileSpec::Empty() 
	{
	m_sFileName.Empty();
	}

void COXFileSpec::Serialize(CArchive& ar)
	{
	CObject::Serialize(ar);
	
	TRY
		{
		if (ar.IsStoring())
			{
			ar << m_sFileName;
			ar << m_time;
			ar << m_lnLength;
			ar << m_eAttributes;
			}	
		else
			{
			ar >> m_sFileName;
			ar >> m_time;
			ar >> m_lnLength;
			ar >> m_eAttributes;
			}
		}
	CATCH(CException, e)
		{
		if (ar.IsStoring())
			TRACE0("An Exception was thrown while storing a COXFileSpec");
		else
			TRACE0("An Exception was thrown while loading a COXFileSpec");

		THROW_LAST();
		}
	END_CATCH
	}
	
BOOL COXFileSpec::operator==(const COXFileSpec& fileSpec) const
	{
	return (m_sFileName == fileSpec.m_sFileName);
	}
	
BOOL COXFileSpec::operator!=(const COXFileSpec& fileSpec) const
	{
	return (m_sFileName != fileSpec.m_sFileName);
	}
	
BOOL COXFileSpec::operator<=(const COXFileSpec& fileSpec) const
	{
	return (m_sFileName <= fileSpec.m_sFileName);
	}
	
BOOL COXFileSpec::operator<(const COXFileSpec& fileSpec) const
	{
	return (m_sFileName < fileSpec.m_sFileName);
	}
	
BOOL COXFileSpec::operator>=(const COXFileSpec& fileSpec) const
	{
	return (m_sFileName >= fileSpec.m_sFileName);
	}
	
BOOL COXFileSpec::operator>(const COXFileSpec& fileSpec) const
	{
	return (m_sFileName > fileSpec.m_sFileName);
	}
	
#ifdef _DEBUG
void COXFileSpec::Dump(CDumpContext& dc) const
	{
	CObject::Dump(dc);
	dc << _T("\nm_sFileName : ") << m_sFileName;
	dc << _T("\nm_time : ") << m_time;
	dc << _T("\nm_lnLength : ") << m_lnLength;
	dc << _T("\nm_eAttributes : ") << m_eAttributes;
	}

void COXFileSpec::AssertValid() const
	{
	CObject::AssertValid();
	}
#endif

COXFileSpec::~COXFileSpec()
	{
	}
	
// protected:

// private:

// ==========================================================================
