// ==========================================================================
//				Class Implementation : COXIteratorUNC
// ==========================================================================

// Source file : OXIteratorUNC.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
			  
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXIteratorUNC.h"
#include "OXUNCStandardActor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// Data members -------------------------------------------------------------
// protected:

	// struct COXSearchData
	//	{
	//	COXUNC m_searchUNC;
	//		--- The directory (and file spec) from which the search is tarted
	//	HANDLE m_hFindFile;
	//		--- The hadnle used during file search
	//	WIN32_FIND_DATA m_findFileData;
	//		--- The last find result
	//	};

	// CList<COXSearchData*, COXSearchData*> m_dirSearchStack;
	// --- List of search data while recursively searching subdirectories
	//     this is only used to get the subdiretories themselves (not their contents)

	// COXSearchData m_fileSearch;
	// --- The search data used to iterate files in the current directory (m_searchUNC)

	// BOOL m_bIncludeFiles;
	// --- Whether to include files (non-directories) in the result

	// BOOL m_bIncludeDirs;
	// --- Whether to include directories in the result

	// BOOL m_bRecursive;
	// --- Whether to serach directories recursively

	// BOOL m_bDepthFirst;
	// --- Whether the directory are traversed in depth first (TRUE) or breadth first
	//     order.  This is only important during recursive searching.

// private:
	
// Member functions ---------------------------------------------------------
// public:

COXIteratorUNC::COXIteratorUNC(LPCTSTR pszUNC /* = NULL */, BOOL bURLPart /* = FALSE */)
	:
	COXUNC(pszUNC, bURLPart),
	m_bIncludeFiles(FALSE),
	m_bIncludeDirs(FALSE),
	m_bRecursive(FALSE),
	m_bDepthFirst(FALSE)
	{
	}

COXIteratorUNC::COXIteratorUNC(LPCTSTR pszServer, LPCTSTR pszShare, LPCTSTR pszDirectory, 
	LPCTSTR pszFile, BOOL bURLPart /* = FALSE */)
	:
	COXUNC(pszServer, pszShare, pszDirectory, pszFile, bURLPart),
	m_bIncludeFiles(FALSE),
	m_bIncludeDirs(FALSE),
	m_bRecursive(FALSE),
	m_bDepthFirst(FALSE)
	{
	}

COXIteratorUNC::COXIteratorUNC(const COXUNC& UNC)
	:
	COXUNC(UNC),
	m_bIncludeFiles(FALSE),
	m_bIncludeDirs(FALSE),
	m_bRecursive(FALSE),
	m_bDepthFirst(FALSE)
	{
	// Do not copy search specific data, because find handles cannot be copied
	}

COXIteratorUNC::COXIteratorUNC(const COXIteratorUNC& iterUNC)
	:
	COXUNC(iterUNC),
	m_bIncludeFiles(FALSE),
	m_bIncludeDirs(FALSE),
	m_bRecursive(FALSE),
	m_bDepthFirst(FALSE)
	{
	// Do not copy search specific data, because find handles cannot be copied
	// TRACE0("COXIteratorUNC::COXIteratorUNC : Warning search specific data is not copied.\n");
	}

BOOL COXIteratorUNC::Start(BOOL bIncludeFiles /* = TRUE */, BOOL bIncludeDirs /* = FALSE */, 
	BOOL bRecursive /* = FALSE */, BOOL bDepthFirst /* = FALSE */)
	{
	// Store required attributes
	m_bIncludeFiles = bIncludeFiles;
	m_bIncludeDirs = bIncludeDirs;
	m_bRecursive = bRecursive;
	m_bDepthFirst = bDepthFirst;

	// If an iteration is still running, end it
	End();

	// Start search with this UNC
	ASSERT(m_dirSearchStack.IsEmpty());
	ASSERT(m_fileSearch.m_hFindFile == NULL);
	m_fileSearch.m_searchUNC = *this;

	// If we want a recursive directory search with depth first: 
	// start from on the deepest dir level
	if (m_bRecursive && m_bDepthFirst)
		{
		COXSearchData* pDirSearchData = new COXSearchData;
		pDirSearchData->m_searchUNC = m_fileSearch.m_searchUNC;
		pDirSearchData->m_searchUNC.File() = _T("*.*");
		m_dirSearchStack.AddHead(pDirSearchData);

		CString sDir = GetNextSearchDirectory();
		if (!sDir.IsEmpty())
			{
			m_fileSearch.m_searchUNC.Directory() = sDir;
			}
		}

	// ... Get the first one
	return Next();
	}

BOOL COXIteratorUNC::Next()
	{
	// Get next find
	ASSERT_VALID(m_pActor);

	BOOL bKeepSearching = TRUE;
	while(bKeepSearching)
		{
		// Start or continue searching for the right file
		if (m_fileSearch.m_hFindFile == NULL)
			m_nLastError = m_pActor->FindFirstFile(m_fileSearch.m_searchUNC, m_fileSearch.m_hFindFile, 
				m_fileSearch.m_findFileData);
		else
			m_nLastError = m_pActor->FindNextFile(m_fileSearch.m_hFindFile, m_fileSearch.m_findFileData);

		if (SUCCEEDED(m_nLastError))
			{
			if (IsAcceptableFile())
				{
				// ... Use FileForm so that we can always overwrite tle last part of the UNC
				//     even if it is part of a directory 
				//     e.g. When we find Test when we are looking for C:\Test\ .
				//          This will be handled as C:\Test\ -> C:\Test -> C:\ ,
				//			then C:\ + Test\ = C:\Test\ .
				COXUNC::operator=(m_fileSearch.m_searchUNC.FileForm());
				if ((m_fileSearch.m_findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
					File() = m_fileSearch.m_findFileData.cFileName;
				else
					{
					Directory() += CString(m_fileSearch.m_findFileData.cFileName) + PreferedSlash();
					File().Empty();
					}
				bKeepSearching = FALSE;
				}
			}
		else
			{
			if (m_bRecursive)
				{
				// Get one level down the directory hierarchy
				CString sDir = GetNextSearchDirectory();
				if (!sDir.IsEmpty())
					{
					EndFileSearch();
					m_fileSearch.m_searchUNC.Directory() = sDir;
					}
				else
					{
					// ... End of recursive search
					Empty();
					bKeepSearching = FALSE;
					}
				}
			else
				{
				// ... Last file in specified dir and not searching recursively
				Empty();
				bKeepSearching = FALSE;
				}
			}
		} // while

	return (SUCCEEDED(m_nLastError));
	}

BOOL COXIteratorUNC::End()
	{
	// ... Assume success
	m_nLastError = ERROR_SUCCESS;

	// End a possible ongoing file search and all recursive searches
	// (will set m_nLastError)
	EndFileSearch();
	EndAllDirectorySearches();

	return (SUCCEEDED(m_nLastError));
	}

COXIteratorUNC& COXIteratorUNC::operator++()
	// Prefix increment operator.
	{
	Next();
	return *this;
	}

COXIteratorUNC COXIteratorUNC::operator++(int)
	// Postfix increment operator.
	{
	COXIteratorUNC tempIteratorUNC = *this;
	Next();
	return tempIteratorUNC;
	}

#ifdef _DEBUG
#endif //_DEBUG

COXIteratorUNC::~COXIteratorUNC()
	{
	// ... End a psssible ongoing search
	End();
	ASSERT(m_dirSearchStack.IsEmpty());
	}

// protected:
CString COXIteratorUNC::GetNextSearchDirectory()
	// --- In  :
	// --- Out : 
	// --- Returns : The new directory specification (absolute but with drive or share spec)
	// --- Effect : Gets the next directory in the recursive search
	//				Directories are searched depth first
	{
	// ... This function should only be called during recursive search
	ASSERT(m_bRecursive);

	if (m_dirSearchStack.IsEmpty())
		{
		if (m_bDepthFirst)
			{
			// We have been searching depth first and we are now back at the top level
			// this concludes aour recursive search
			return _T("");
			}
		else
			{
			// We are not searching depth first, so when the search stack is empty
			// we will push the starting directory (file search UNC) on the stack
			COXSearchData* pDirSearchData = new COXSearchData;
			pDirSearchData->m_searchUNC = m_fileSearch.m_searchUNC;
			pDirSearchData->m_searchUNC.File() = _T("*.*");
			m_dirSearchStack.AddHead(pDirSearchData);
			}
		}

	ASSERT(!m_dirSearchStack.IsEmpty());
	// ... Top of the stack is the directory we are searching now
	COXSearchData* pDirSearchData = m_dirSearchStack.GetHead();

	CString sDirectory;
	BOOL bKeepSearching = TRUE;
	while(bKeepSearching)
		{
		// Start or continue searching for the subdirectory
		if (pDirSearchData->m_hFindFile == NULL)
			m_nLastError = m_pActor->FindFirstFile(pDirSearchData->m_searchUNC, pDirSearchData->m_hFindFile, 
				pDirSearchData->m_findFileData, TRUE);
		else
			m_nLastError = m_pActor->FindNextFile(pDirSearchData->m_hFindFile, pDirSearchData->m_findFileData);
		
		if (SUCCEEDED(m_nLastError))
			{
			if (IsAcceptableDirectory())
				{
				COXSearchData* pNewDirSearchData = new COXSearchData;
				pNewDirSearchData->m_searchUNC = pDirSearchData->m_searchUNC;
				pNewDirSearchData->m_searchUNC.Directory() += CString(pDirSearchData->m_findFileData.cFileName) + PreferedSlash();
				ASSERT(pNewDirSearchData->m_searchUNC.File() == _T("*.*"));
				m_dirSearchStack.AddHead(pNewDirSearchData);
				// ... Found a good directory
				pDirSearchData = pNewDirSearchData;
				// ... Keep searching when we are traversing depth first, 
				//     otherwise stop looking
				if (!m_bDepthFirst)
					{
					sDirectory = pDirSearchData->m_searchUNC.Directory();
					bKeepSearching = FALSE;
					}
				}
			}
		else
			{
			if (!m_bDepthFirst)
				// ... No more directories found, get the next one one level up
				// ... Pop the current dir from the stack
				EndLastDirectorySearch();
			if (!m_dirSearchStack.IsEmpty())
				{
				if (m_bDepthFirst)
					{
					// .. Use the current top of the stackl before popping it
					//    and return this as result
					ASSERT(pDirSearchData == m_dirSearchStack.GetHead());
					sDirectory = pDirSearchData->m_searchUNC.Directory();
					bKeepSearching = FALSE;
					}
				else
					// .. Get the search which is now the new top of the stack
					//    and continue searching there
					pDirSearchData = m_dirSearchStack.GetHead();
				}
			else
				{
				// ... End of recursive search
				sDirectory.Empty();
				bKeepSearching = FALSE;
				}
			if (m_bDepthFirst)
				// ... Pop the current dir from the stack after we have used it
				EndLastDirectorySearch();
			}
		}

	return sDirectory;
	}

BOOL COXIteratorUNC::IsAcceptableFile() const
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the file is acceptable
	// --- Effect : Checks whether the current file specification
	//				is something we are interested in
	{
	// We accept everything, except those we explicitely reject
	BOOL bAccept = TRUE;

	// ... Reject names "." and ".."
	if ( ( m_fileSearch.m_findFileData.cFileName[0] == _T('.') ) &&
		 ( m_fileSearch.m_findFileData.cFileName[1] == _T('\0') ||
		   (m_fileSearch.m_findFileData.cFileName[1] == _T('.') &&
			m_fileSearch.m_findFileData.cFileName[2] == _T('\0') ) ) )
		bAccept = FALSE;

	// ... Reject files if not wanted
	if (!m_bIncludeFiles &&
	    ((m_fileSearch.m_findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) )
		bAccept = FALSE;

	// ... Reject directories if not wanted
	if (!m_bIncludeDirs && 
	    ((m_fileSearch.m_findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) )
		bAccept = FALSE;

	return bAccept;
	}

BOOL COXIteratorUNC::IsAcceptableDirectory() const
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the dir is acceptable
	// --- Effect : Checks whether the last directory specification (top of the stack)
	//				is something we are interested in
	{
	// We accept all directories except those we explicitely reject (. and ..)
	BOOL bAccept = m_bRecursive;

	if (!bAccept)
		return bAccept;

	// ... Start should have added an item to the top of the list
	ASSERT(!m_dirSearchStack.IsEmpty());
	COXSearchData* pDirSearchData = m_dirSearchStack.GetHead();

	// ... Reject names "." and ".."
	if ( ( pDirSearchData->m_findFileData.cFileName[0] == _T('.') ) &&
		 ( pDirSearchData->m_findFileData.cFileName[1] == _T('\0') ||
		   (pDirSearchData->m_findFileData.cFileName[1] == _T('.') &&
			pDirSearchData->m_findFileData.cFileName[2] == _T('\0') ) ) )
		bAccept = FALSE;

	// ... Reject files 
	if ((pDirSearchData->m_findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		bAccept = FALSE;

	return bAccept;
	}

BOOL COXIteratorUNC::EndFileSearch()
	// --- In  :
	// --- Out : 
	// --- Returns : Whether it succeeded (will set last error upon failure)
	// --- Effect : Ends a possible ongoing file search
	{
	HRESULT nLastError = ERROR_SUCCESS;
	if (m_fileSearch.m_hFindFile != NULL)
		{
		ASSERT_VALID(m_pActor);
		nLastError = m_pActor->FindClose(m_fileSearch.m_hFindFile);
		if (FAILED(nLastError))
			m_nLastError = nLastError;
		m_fileSearch.m_hFindFile = NULL;
		}

	return (SUCCEEDED(m_nLastError));
	}

BOOL COXIteratorUNC::EndAllDirectorySearches()
	// --- In  :
	// --- Out : 
	// --- Returns : Whether it succeeded (will set last error upon failure)
	// --- Effect : Ends possible ongoing directory searches
	{
	// Close directory find(s) if necessary
	if (!m_dirSearchStack.IsEmpty())
		{
		ASSERT_VALID(m_pActor);
		POSITION pos;
		COXSearchData* pDirSearchData = NULL;
		HRESULT nLastError = ERROR_SUCCESS;
		pos = m_dirSearchStack.GetHeadPosition();
		while (pos != NULL)
			{
			pDirSearchData = m_dirSearchStack.GetNext(pos);
			if (pDirSearchData->m_hFindFile != NULL)
				{
				nLastError = m_pActor->FindClose(pDirSearchData->m_hFindFile);
				if (FAILED(nLastError))
					m_nLastError = nLastError;
				}
			delete pDirSearchData;
			}
		m_dirSearchStack.RemoveAll();
		}

	return (SUCCEEDED(m_nLastError));
	}

BOOL COXIteratorUNC::EndLastDirectorySearch()
	// --- In  :
	// --- Out : 
	// --- Returns : Whether it succeeded (will set last error upon failure)
	// --- Effect : Ends the last (most recent) directory search
	{
	// Close directory find(s) if necessary
	if (!m_dirSearchStack.IsEmpty())
		{
		ASSERT_VALID(m_pActor);
		POSITION pos;
		COXSearchData* pDirSearchData = NULL;
		HRESULT nLastError = ERROR_SUCCESS;
		pos = m_dirSearchStack.GetHeadPosition();
		if (pos != NULL)
			{
			pDirSearchData = m_dirSearchStack.GetNext(pos);
			if (pDirSearchData->m_hFindFile != NULL)
				{
				nLastError = m_pActor->FindClose(pDirSearchData->m_hFindFile);
				if (FAILED(nLastError))
					m_nLastError = nLastError;
				}
			m_dirSearchStack.RemoveHead();
			delete pDirSearchData;
			}
		}

	return (SUCCEEDED(m_nLastError));
	}

// private:

// ==========================================================================