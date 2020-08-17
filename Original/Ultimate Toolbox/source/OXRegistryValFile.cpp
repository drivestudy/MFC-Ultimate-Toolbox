// ==========================================================================
//				Class Implementation : COXRegistryValFile
// ==========================================================================

// Source file : COXRegistryValFile.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXRegistryValFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COXRegistryValFile::COXRegistryValFile()
	: CMemFile(1024), m_key(0)
	{
	}

COXRegistryValFile::COXRegistryValFile(HKEY hkey, LPCTSTR lpszKey, LPCTSTR lpszValue)
	: CMemFile(1024), m_key(0)
	{
	LONG error;
	if (!Open(hkey, lpszKey, lpszValue, error))
		AfxThrowFileException(CFileException::accessDenied, error, lpszKey);
	}

COXRegistryValFile::~COXRegistryValFile()
	{
	if (m_key != 0)
		Close();
	}

////////////////////////////////////////////////////////////////////////////
// COXRegistryValFile implementation

BOOL COXRegistryValFile::Open(HKEY hkey, LPCTSTR lpszKey, LPCTSTR lpszValue, LONG& error)
	{
	ASSERT(AfxIsValidString(lpszKey));
	ASSERT(AfxIsValidString(lpszValue));
	m_value = lpszValue;

	error = ::RegCreateKey(hkey, lpszKey, &m_key);
	if (ERROR_SUCCESS != error)
		return FALSE;
	DWORD dwType;
	DWORD dwSize;
	if (ERROR_SUCCESS == ::RegQueryValueEx(m_key, m_value, NULL, &dwType, NULL, &dwSize))
		{
		// the value already exists, check the type
		if (dwType != REG_BINARY)
			{
			error = 0;		// wrong type exists
			return FALSE;
			}

		// the value exists and has the right type
		BYTE * pData = NULL;
		try
			{
			pData = new BYTE[dwSize];
			}
		catch(...)
			{
			error = 0;		// memory low
			return FALSE;
			}

		error = ::RegQueryValueEx(m_key, m_value, NULL, &dwType, pData, &dwSize);
		if (ERROR_SUCCESS != error)
			{
			delete [] pData;
			return FALSE;
			}

		Write(pData, dwSize);
		SeekToBegin();

		delete [] pData;
		return TRUE;
		}

	error = 0;
	return TRUE;
	}

void COXRegistryValFile::Flush()
	{
	ASSERT_VALID(this);

	if (m_key == 0)
		return;

	::RegSetValueEx(m_key, m_value, 0, REG_BINARY, m_lpBuffer, (DWORD) GetLength());
	}

void COXRegistryValFile::Close()
	{
	ASSERT_VALID(this);
	ASSERT(m_key != 0);

	Flush();
	::RegCloseKey(m_key);

	m_key = 0;
	m_value.Empty();
	}

void COXRegistryValFile::Abort()
	{
	// close but ignore errors
	ASSERT_VALID(this);

	Flush();
	ASSERT(m_key != 0);

	::RegCloseKey(m_key);

	m_key = 0;
	m_value.Empty();
	}

IMPLEMENT_DYNAMIC(COXRegistryValFile, CMemFile)
////////////////////////////////////////////////////////////////////////////
