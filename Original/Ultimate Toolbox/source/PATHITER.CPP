// ==========================================================================
// 							Class Implementation : COXPathIterator
// ==========================================================================

// Source file : pathiter.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                         
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "pathiter.h"		// class specification

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXPathIterator, CObject)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members


// Data members -------------------------------------------------------------
// protected:

// private:

// Member functions ---------------------------------------------------------
// public:

COXPathIterator::COXPathIterator()
	:	m_bValid(FALSE)
	{
#ifdef WIN32
		m_hFindFile=NULL;
#endif
	}
	
BOOL COXPathIterator::IsItValid()
{
	return m_bValid;
}               
	
#ifdef _DEBUG
void COXPathIterator::Dump(CDumpContext& dc) const
	{
	CObject::Dump(dc);
#ifdef WIN32
	dc << TEXT("\nm_hFindFile : ") << m_hFindFile;
	dc << TEXT("\nm_FindFileData : ") << (void*)&m_FindFileData;	
#else
    dc << TEXT("\nm_FileInfo : ") << (void*)&m_FileInfo;
#endif
	dc << TEXT("\nm_bValid : ") << (WORD)m_bValid;
	}

void COXPathIterator::AssertValid() const
	{
	CObject::AssertValid();
	}
#endif

COXPathIterator::~COXPathIterator()
	{
#ifdef WIN32	
	if (m_hFindFile != NULL)
		FindClose(m_hFindFile);
#endif
	}

// private:

// Message handlers ---------------------------------------------------------

// ==========================================================================
