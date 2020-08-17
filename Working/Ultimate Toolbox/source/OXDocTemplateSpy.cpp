// ==========================================================================
//				Class Implementation : COXDocTemplateSpy
// ==========================================================================

// Source file : OXDocTemplateSpy.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
			  
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#if (_MFC_VER < 0x0420)
// This file uses classes that were introduced in MFC Version 4.2
// These classes are now officially documented by Microsoft, but did not exist in previous versions
// Therefore this file will be completely excluded for older versions of MFC
#pragma message("Warning : OXDocTemplateSpy.cpp not included because MFC Version < 4.2")
#else
// The entire file


#include "OXDocTemplateSpy.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#include "UTBStrOp.h"

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members
// Data members -------------------------------------------------------------
// protected:

// private:
	
// Member functions ---------------------------------------------------------
// public:

BOOL COXDocTemplateSpy::CheckMatch(CRuntimeClass* pDocClass, 
	CRuntimeClass* pFrameClass /* = NULL */, CRuntimeClass* pViewClass /* = NULL */) const
	{
	return ((m_pDocClass == pDocClass) || (pDocClass == NULL)) && 
		   ((m_pFrameClass == pFrameClass) || (pFrameClass == NULL)) && 
		   ((m_pViewClass == pViewClass) || (pViewClass == NULL));
	}

BOOL COXDocTemplateSpy::CheckMatch(LPCTSTR pszDocClass, LPCTSTR pszFrameClass /* = NULL */, 
		LPCTSTR pszViewClass /* = NULL */) const
	{
#ifdef _UNICODE
	LPSTR pszAnsiDocClass = NULL;
	LPSTR pszAnsiFrameClass = NULL;
	LPSTR pszAnsiViewClass = NULL;

	if (pszDocClass != NULL)
		{
		const size_t nMaxAnsiDocClassLength = _tcslen(pszDocClass) * 2 + 1;
		pszAnsiDocClass = new char[nMaxAnsiDocClassLength];
		size_t t;
		UTBStr::wcstombs(&t, pszAnsiDocClass, nMaxAnsiDocClassLength+1, pszDocClass, nMaxAnsiDocClassLength);
		}
	if (pszFrameClass != NULL)
		{
		const size_t nMaxAnsiFrameClassLength = _tcslen(pszFrameClass) * 2 + 1;
		pszAnsiFrameClass = new char[nMaxAnsiFrameClassLength];
		size_t t;
		UTBStr::wcstombs(&t, pszAnsiFrameClass, nMaxAnsiFrameClassLength+1, pszFrameClass, nMaxAnsiFrameClassLength);
		}
	if (pszViewClass != NULL)
		{
		const size_t nMaxAnsiDocClassLength = _tcslen(pszViewClass) * 2 + 1;
		pszAnsiViewClass = new char[nMaxAnsiDocClassLength];
		size_t len;
		UTBStr::wcstombs(&len, pszAnsiViewClass, nMaxAnsiDocClassLength+1, pszViewClass, nMaxAnsiDocClassLength);
		}
#else
	LPCSTR pszAnsiDocClass = pszDocClass;
	LPCSTR pszAnsiFrameClass = pszFrameClass;
	LPCSTR pszAnsiViewClass = pszViewClass;
#endif

	BOOL bMatched =  ((pszDocClass == NULL) || (strcmp(m_pDocClass->m_lpszClassName, pszAnsiDocClass) == 0)) && 
		   ((pszFrameClass == NULL) || (strcmp(m_pFrameClass->m_lpszClassName, pszAnsiFrameClass) == 0)) && 
		   ((pszViewClass == NULL) || (strcmp(m_pViewClass->m_lpszClassName, pszAnsiViewClass) == 0));

#ifdef _UNICODE
	delete [] pszAnsiDocClass;
	pszAnsiDocClass = NULL;
	delete [] pszAnsiFrameClass;
	pszAnsiFrameClass = NULL;
	delete[] pszAnsiViewClass;
	pszAnsiViewClass = NULL;
#endif

	return bMatched;
	}

#ifdef _DEBUG
void COXDocTemplateSpy::AssertValid() const
	{
	CDocTemplate::AssertValid();
	}

void COXDocTemplateSpy::Dump(CDumpContext& dc) const
	{
	CDocTemplate::Dump(dc);
	}
#endif //_DEBUG

// protected:
COXDocTemplateSpy::COXDocTemplateSpy()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object
	:
	CDocTemplate(0 /* nIDResource */, NULL /* pDocClass */, NULL /* pFrameClass*/ , 
	NULL /* pViewClass */)
	{
	// ... Never construct an object of this class
	ASSERT(FALSE);
	}

COXDocTemplateSpy::~COXDocTemplateSpy()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object
	{
	// ... Never destruct an object of this class
	ASSERT(FALSE);
	}

// private:

#endif // _MFC_VER
// ==========================================================================
