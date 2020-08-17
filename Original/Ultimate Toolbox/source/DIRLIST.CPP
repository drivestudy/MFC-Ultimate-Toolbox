// ==========================================================================
// 							Class Implementation : COXDirList
// ==========================================================================

// Source file : dirlist.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                         
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "dirlist.h"	// class specification
#include "dos.h"		// for _dos_findfirst, ...
#include "UTB64Bit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXDirList, CObject)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members


// Data members -------------------------------------------------------------
// protected:
	// COXPathSpec m_path;
	// --- The path specification where to look for Dirs
	
	// CObList m_dirList;			// Contains objects of class COXDirSpec
	// --- The list of dir specifications found in the above specified path

// private:
	
// Member functions ---------------------------------------------------------
// public:

COXDirList::COXDirList()
	:
	m_path()
	{
	}
	
COXPathSpec COXDirList::GetPath() const
	{
	return m_path;
	}
	
BOOL COXDirList::SetPath(COXPathSpec path)
	{
	if (path.GetFileName().IsEmpty())
		path.SetFileName(_T("*.*"));
	if (path.MakeAbsolute())
		{
		m_path = path;
		return TRUE;
		}
	else
		{
		TRACE(_T("COXDirList::SetPath : Path spec is invalid : %s\n"), path.GetPath());
		return FALSE;
		}
	}   
	
BOOL COXDirList::Search()
	{
#ifdef WIN32
	COXDirSpec* pDir;
	WIN32_FIND_DATA fileData;
	HANDLE hFindFile;
	BOOL bFileFound(TRUE);
		
	hFindFile = FindFirstFile(m_path.GetPath(), &fileData);
	if(hFindFile != INVALID_HANDLE_VALUE)
		{
		while (bFileFound)
			{
			if (((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) &&
				(fileData.cFileName[0] != _T('.')))
				{
				pDir = new COXDirSpec(m_path.GetDirectory());
				pDir->AppendDirectory(COXDirSpec(fileData.cFileName));
				m_dirArray.Add(pDir);
				}

			bFileFound = FindNextFile(hFindFile, &fileData);
			}
		FindClose(hFindFile);
		}
	return TRUE;

#else
	COXDirSpec* pDir;
	_find_t fileInfo;
	BOOL bFileFound;
	
	bFileFound = (_dos_findfirst(m_path.GetPath(), _A_NORMAL | _A_ARCH | _A_SUBDIR, &fileInfo) == 0);
	while (bFileFound)
		{
		if (((fileInfo.attrib & _A_SUBDIR) == _A_SUBDIR) && (strcmp(fileInfo.name, _T(".")) != 0) &&
			(strcmp(fileInfo.name, _T("..")) != 0))
			{
			pDir = new COXDirSpec(m_path.GetDirectory());
			pDir->AppendDirectory(COXDirSpec(fileInfo.name));
			m_dirArray.Add(pDir);
			}

		bFileFound = (_dos_findnext(&fileInfo) == 0);
		}		
	return TRUE;
#endif
	}
	
const CObArray* COXDirList::GetList() const
	{
	return &m_dirArray;
	}

const COXDirSpec* COXDirList::GetAt(int nIndex) const
{
	return (COXDirSpec*) m_dirArray.GetAt(nIndex);
}
	
void COXDirList::Sort()
	{
	// This algorithm will sort a list using selection sort
	// The array contains objects from position 0 to END
	// nIndexNotSorted points to the first not yet sorted object,
	//  so the set [0..nIndexNotSorted - 1] is already sorted
	// This algorithm searches the not yet sorted objects in the set
	//  [nIndexNotSorted..END] for the smallest object and adds this 
	//  to the sorted objects
	// nIndexCompare point to the last object in this set that has 
	//  already been examined
	// The algorithm ends when all objects are sorted
	int nIndexNotSorted = 0;
	int nIndexCompare = 0;
	COXDirSpec* pSmallestDir = NULL;	// point to array[nIndexSorted]
	COXDirSpec* pCompareDir = NULL;		// point to array[nIndexCompare]
	COXDirSpec* pSwapDir;
	
	while (nIndexNotSorted <= m_dirArray.GetUpperBound())
		{
		ASSERT(m_dirArray[nIndexNotSorted]->IsKindOf(RUNTIME_CLASS(COXDirSpec)));
		pSmallestDir = (COXDirSpec*)m_dirArray[nIndexNotSorted];
		nIndexCompare = nIndexNotSorted;
		nIndexCompare++;
		
		while (nIndexCompare <= m_dirArray.GetUpperBound()) 
			{
			ASSERT(m_dirArray[nIndexCompare]->IsKindOf(RUNTIME_CLASS(COXDirSpec)));
			pCompareDir = (COXDirSpec*)m_dirArray[nIndexCompare];
			if (*pCompareDir < *pSmallestDir)
				{
				pSwapDir = pSmallestDir;
				m_dirArray[nIndexNotSorted] = m_dirArray[nIndexCompare];
				m_dirArray[nIndexCompare] = pSwapDir;
				pSmallestDir = (COXDirSpec*)m_dirArray[nIndexNotSorted];
				}
			nIndexCompare++;
			}
		nIndexNotSorted++;
		}
	}
	
void COXDirList::ClearList()
	{
	COXDirSpec* pDir;
	int nIndex = 0;
	int nMaxIndex = PtrToInt(m_dirArray.GetUpperBound());
	while (nIndex <= nMaxIndex) 
		{
		ASSERT(m_dirArray[nIndex]->IsKindOf(RUNTIME_CLASS(COXDirSpec)));
		pDir = (COXDirSpec*)m_dirArray[nIndex];
		delete pDir;
		nIndex++;
		}
	m_dirArray.RemoveAll();
	}
	
#ifdef _DEBUG
void COXDirList::Dump(CDumpContext& dc) const
	{
	CObject::Dump(dc);
	dc << _T("\nm_path : ");
	m_path.Dump(dc);
	dc << _T("\nm_dirArray : ") << m_dirArray;	
	}

void COXDirList::AssertValid() const
	{
	CObject::AssertValid();
	}
#endif

COXDirList::~COXDirList()
	{
	ClearList();
	}
	
// protected:

// private:

// Message handlers ---------------------------------------------------------

// ==========================================================================
