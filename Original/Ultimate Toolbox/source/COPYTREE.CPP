// ==========================================================================
// 						Class Implementation : COXCopyTree
// ==========================================================================

// Source file :copytree.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                         
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "cpystdlg.h"	// class specification
#include "copytree.h"	// class specification

#include "returns.h"	// internal return codes

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXCopyTree, CObject)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members


// Data members -------------------------------------------------------------
// protected:
	// COXDoubleStringList m_dsFiles;		
	// --- List of files created during copy process			
	// COXDoubleStringList m_dsDirs;		
	// --- List of dirs created during copy process

	// short m_nStatus;		
	// --- Status of the copy process

	// COXDirSpec m_SourceDir;
	// COXDirSpec m_DestDir;
	// COXDirSpec m_BufferDir;
	// --- Temporary dir spec to restore current directory
	
	// COXPathSpec m_SourcePath;
	// --- Path of source file
	// COXPathSpec m_DestPath;
	// --- Path of destination file

	// COXCopyStatusDialog* m_pCpyStatDlg;
	// --- Pointer to dialog which show the progress (or NULL for no UI)
						 

// private:
	HCURSOR m_hCursor;
	
// Member functions ---------------------------------------------------------
// public:

COXCopyTree::COXCopyTree()
	: 
	m_pCpyStatDlg(NULL)
	{
	}

BOOL COXCopyTree::DoCopyTree(COXDirSpec SourceDir, COXDirSpec DestDir, BOOL bOnlyContents /* = TRUE */,
	BOOL bCleanUp /* = FALSE */, COXCopyStatusDialog* pCpyStatDlg /* = NULL */)
	{
	static BOOL bBusyCopying = FALSE;
	CWnd* pMainWnd = NULL;

	if (bBusyCopying)
		return FALSE;

	bBusyCopying = TRUE;

	m_SourceDir = SourceDir;
	m_DestDir = DestDir;

	// Store the current directory of this application
	// We'll use it to set the original situation back when finished
	COXDirSpec CurrDir;
	CurrDir.DoGetCurrentDir();

 	if (!bOnlyContents)
		// First Create the source dir physically under the Destination Dir
		{
		COXDirSpec TempDir = m_SourceDir.GetLastSubdirectory();
		TempDir.SetDrive(_T(""));
		if (!m_DestDir.AppendDirectory(TempDir))
			{
			m_nStatus = F_CREATEDIR;
			bBusyCopying = FALSE;
			return FALSE;
			}
		
		if (!m_DestDir.Exists())
			{
			if (!m_DestDir.DoMakeNew())
				{
				TRACE(_T("COXCopyTree::DoCopyTree : Directory creation %s failed\n"), LPCTSTR(m_DestDir.GetDirectory()));

				m_nStatus = F_CREATEDIR;
				bBusyCopying = FALSE;
				return FALSE;
				}

			// add this directory to list of created dirs
		    short nReturn;
			if ((nReturn = m_dsDirs.Add(m_DestDir.GetDirectory())) != SUCCESS)
				{
		 		TRACE(_T("COXCopyTree::DoCopyTree : Could not add '%s' to recovery List\n"), LPCTSTR(m_DestDir.GetDirectory()));

				m_nStatus = nReturn;
				bBusyCopying = FALSE;
				return FALSE;
				}

			 }
		}
		 	
 	// The walkTree process needs that the source dir is the current dir when its
	// starts walking
 	if (m_SourceDir.IsEmpty() || !m_SourceDir.DoSetCurrentDir())
		{
 		TRACE(_T("COXCopyTree::DoCopyTree : Could not set '%s' as current dir \n"), LPCTSTR(m_SourceDir.GetDirectory()));

		m_nStatus = F_SETCURRENTDIR;
		bBusyCopying = FALSE;
		return FALSE;
		}

	// start status as SUCCESS, if anything fails while walking the tree
	// the status will be set and walk will return up the tree

	m_nStatus = SUCCESS;

	if (pCpyStatDlg != NULL)
		{
		// Try to disable the mainframe, to eliminate interference
		// Note that AfxGetApp() will fail in a DLL
		pMainWnd = AfxGetApp()->m_pMainWnd;
	 	if (pMainWnd != NULL)
			pMainWnd->EnableWindow(FALSE);

		// put up a modeless dialog for copy status
		if (!pCpyStatDlg->Create())
			{
			TRACE(_T("COXCopyTree::DoCopyTree : Creation of Status Dialog Failed"));
			}
		else
			m_pCpyStatDlg = pCpyStatDlg;	
		}

	WalkTree(0);

	// replace the current working dir
	if (!CurrDir.IsEmpty())
		CurrDir.DoSetCurrentDir();

	if (bCleanUp)
		// clean up the dirs and files double list and check for required removal of files based on status
		CleanUp();

	bBusyCopying = FALSE;

	if (m_pCpyStatDlg != NULL)
		m_pCpyStatDlg->DestroyWindow();

	// get rid of the status window
 	if (pMainWnd != NULL)
		pMainWnd->EnableWindow(TRUE);

	return (m_nStatus == SUCCESS);
	}

void COXCopyTree::WalkTree(WORD wLevel)
	{
	static COXDirSpec UpDir(_T(".."));

    short 			  nReturn;
#ifndef WIN32
	struct _find_t findStruct;
#else
  	HANDLE            hSearch;
  	WIN32_FIND_DATA   w32FindBuf;
#endif          
	// make sure the getcurrentdirectory worked correctly
	if (!m_SourceDir.DoGetCurrentDir())
		{
		m_nStatus = F_GETCURRENTDIR;
		return;
		}

#ifndef WIN32
	UINT uReturn = _dos_findfirst(_T("*.*"),_A_SUBDIR|_A_NORMAL,&findStruct);
	if (uReturn != 0)
#else
	hSearch=FindFirstFile(_T("*"),&w32FindBuf);
  	if (hSearch == (HANDLE)INVALID_HANDLE_VALUE)
#endif
   		{
     	if (wLevel)
			{
			m_DestDir.RemoveLastSubdirectory();
			if (!UpDir.DoSetCurrentDir())
				{
				m_nStatus = F_SETCURRENTDIR;
				}
			}
     	return;
   		}
	
  	for (;;)
  		{
		// if this file is .. or . then get next and continue
#ifndef WIN32
		if (strcmp(findStruct.name,_T(".")) == 0 || strcmp(findStruct.name,_T("..")) == 0)
			{
			if (_dos_findnext(&findStruct))
#else
		if (_tcscmp(w32FindBuf.cFileName,_T(".")) == 0 || _tcscmp(w32FindBuf.cFileName,_T("..")) == 0)
			{
			if (FindNextFile(hSearch, &w32FindBuf) == FALSE)
#endif
				{
				if (!UpDir.DoSetCurrentDir())
					{
					m_nStatus = F_SETCURRENTDIR;
					}
#ifdef WIN32
    	    	FindClose(hSearch);
#endif
				m_DestDir.RemoveLastSubdirectory();
				return;
				}
			continue;
			}

		// if this is a directory walk it
#ifndef WIN32
	    if (findStruct.attrib & _A_SUBDIR)
        	{
			if (!m_SourceDir.AppendDirectory(COXDirSpec(findStruct.name)) ||
				!m_SourceDir.DoSetCurrentDir())
#else
     	if (w32FindBuf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
       	 	{
			if (!m_SourceDir.AppendDirectory(COXDirSpec(w32FindBuf.cFileName)) ||
				!m_SourceDir.DoSetCurrentDir())
#endif
				{
		 		TRACE(_T("COXCopyTree::WalkTree : Could not set '%s' as current dir \n"), LPCTSTR(m_SourceDir.GetDirectory()));

				m_nStatus = F_SETCURRENTDIR;
#ifdef WIN32
    	    	FindClose(hSearch);
#endif
				return;
				}					

#ifndef WIN32
			m_DestDir.AppendDirectory(COXDirSpec(findStruct.name));
#else
			m_DestDir.AppendDirectory(COXDirSpec(w32FindBuf.cFileName));
#endif      
			m_BufferDir.DoGetCurrentDir();
			if (!m_DestDir.Exists())
				{
				if (!m_DestDir.DoMakeNew())
					{
			 		TRACE(_T("COXCopyTree::WalkTree : Could not create '%s' \n"), LPCTSTR(m_DestDir.GetDirectory()));

					m_nStatus = F_CREATEDIR;
#ifdef WIN32
	    	    	FindClose(hSearch);
#endif
					return;
					}
				else
					// add this directory to list of created dirs
					{
					if ((nReturn = m_dsDirs.Add(m_DestDir.GetDirectory())) != SUCCESS)
						{
				 		TRACE(_T("COXCopyTree::WalkTree : Could not add '%s' to recovery List\n"), LPCTSTR(m_DestDir.GetDirectory()));

						m_nStatus = nReturn;
#ifdef WIN32
		    	    	FindClose(hSearch);
#endif
						return;
						}
					}
				}

			m_BufferDir.DoSetCurrentDir();
	     	WalkTree(++wLevel);
			// if anything failed then walk back up the tree
			if (m_nStatus != SUCCESS)
			if (m_nStatus != SUCCESS)
				{
#ifdef WIN32
    	    	FindClose(hSearch);
#endif
				return;
				}

			m_SourceDir.RemoveLastSubdirectory();
        	}
		else
			{
			// else it is a file copy it
			COXFileSpec FileSpec;

#ifndef WIN32
			FileSpec.SetFileName(findStruct.name);
#else
			FileSpec.SetFileName(w32FindBuf.cFileName);
#endif
			m_SourcePath.SetPath(m_SourceDir, FileSpec);
			m_DestPath.SetPath(m_DestDir, FileSpec);

			// Check if the user hit the cancel button

			if (m_pCpyStatDlg != NULL && m_pCpyStatDlg->IsCancelled())
				{
		 		TRACE(_T("COXCopyTree::WalkTree : User Cancelled copy process, returning...\n"));

				m_nStatus = F_CANCEL;
#ifdef WIN32
    	    	FindClose(hSearch);
#endif
				return;
				}
			else	
				{
				if (m_pCpyStatDlg != NULL)
					{
					m_pCpyStatDlg->SetStatusText(COXCopyStatusDialog::CSCopying, m_SourcePath.GetPath(),
						m_DestPath.GetPath());
					}

				if (!m_SourcePath.DoCopy(m_DestPath))
					{
			 		TRACE(_T("COXCopyTree::WalkTree : Could not copy '%s' to '´%s'\n"),
			 			LPCTSTR(m_SourcePath.GetPath()), LPCTSTR(m_DestPath.GetPath()));

					m_nStatus = F_COPYFILE;
#ifdef WIN32
	    	    	FindClose(hSearch);
#endif
					return;
					}

				// add this file to list of created files
				if ((nReturn = m_dsFiles.Add(m_DestPath.GetPath())) != SUCCESS)
					{
			 		TRACE(_T("COXCopyTree::WalkTree : Could not add '%s' to recovery List\n"), LPCTSTR(m_DestPath.GetPath()));

					m_nStatus = nReturn;
#ifdef WIN32
	    	    	FindClose(hSearch);
#endif
					return;
					}
				}
			}

#ifndef WIN32
		if (_dos_findnext(&findStruct))
#else
		if (FindNextFile(hSearch, &w32FindBuf) == FALSE)
#endif
			{
			if (!UpDir.DoSetCurrentDir())
				{
				m_nStatus = F_SETCURRENTDIR;
				}
#ifdef WIN32
   	    	FindClose(hSearch);
#endif
			m_DestDir.RemoveLastSubdirectory();
   			return;
			}
    	}
	}

void COXCopyTree::CleanUp()
	{
	LPTSTR lpsz;
	COXPathSpec CreatedFile;
	COXDirSpec CreatedDir;
  
	// check for required clean up
	if (m_nStatus != SUCCESS)
		{
		m_nStatus = F_CANCEL;
		if (m_dsDirs.GetCount() || m_dsFiles.GetCount())
			{
			TRACE(_T("COXCopyTree::CleanUp : Several Files and/or dirs found to remove\n	STARTING CLEAN PROCESS\n\n"));
			// hourglass
			SetHourGlass();			
			// update the look of the copystat dialog to remove format
			if (m_pCpyStatDlg != NULL)
				{
				m_pCpyStatDlg->SetWindowText(_T("Removal Status"));
				}

			// iterate the files
			for (lpsz = m_dsFiles.GetFirst(); lpsz; lpsz = m_dsFiles.GetNext())
				{
				// tell the user what is being removed
				if (m_pCpyStatDlg != NULL)
					{
					m_pCpyStatDlg->SetStatusText(COXCopyStatusDialog::CSRemoving, lpsz);
					}
				
				CreatedFile.SetPath(lpsz);
		 		TRACE(_T("\tCOXCopyTree::CleanUp : Removing File '%s'\n"), LPCTSTR(CreatedFile.GetPath()));
				if (CreatedFile.DoRemove() == FALSE)
					{
					// remove dirs and files for double list
					m_dsDirs.RemoveAll();
					m_dsFiles.RemoveAll();
					SetArrow();
					return;
					}
				}

			// iterate the dirs backwards
			for (lpsz = m_dsDirs.GetLast(); lpsz; lpsz = m_dsDirs.GetPrev())
				{
				// tell the user what is being removed
				if (m_pCpyStatDlg != NULL)
					{
					m_pCpyStatDlg->SetStatusText(COXCopyStatusDialog::CSRemoving, lpsz);
					}

				CreatedDir.SetDirectory(lpsz);
		 		TRACE(_T("\tCOXCopyTree::CleanUp : Removing Dir '%s'\n"), LPCTSTR(CreatedDir.GetDirectory()));
				if (CreatedDir.DoRemove() == FALSE)
					{
					// remove dirs and files for double list
					m_dsDirs.RemoveAll();
					m_dsFiles.RemoveAll();
					SetArrow();
					return;
					}
				}	
			}
		}

	// remove dirs and files for double list 
	m_dsDirs.RemoveAll();
	m_dsFiles.RemoveAll();

	SetArrow();

	return;
	}

void COXCopyTree::SetHourGlass()
	{
    m_hCursor = LoadCursor(NULL, IDC_WAIT);
    m_hCursor = SetCursor(m_hCursor);
	}

void COXCopyTree::SetArrow()
	{
    m_hCursor = SetCursor(m_hCursor);                                           
	}
		
#ifdef _DEBUG
void COXCopyTree::Dump(CDumpContext& dc) const
	{
	CObject::Dump(dc);
	dc << _T("\nm_dsFiles: ") << (const void*)&m_dsFiles;
	dc << _T("\nm_dsDirs: ") << (const void*)&m_dsDirs;
	dc << _T("\nm_nStatus : ") << (int)m_nStatus;

	dc << _T("\nm_SourceDir: ") << m_SourceDir.GetDirectory();
	dc << _T("\nm_DestDir: ") << m_DestDir.GetDirectory();
	dc << _T("\nm_BufferDir: ") << m_BufferDir.GetDirectory();

	dc << _T("\nm_SourcePath: ") << m_SourcePath.GetPath();
	dc << _T("\nm_DestPath: ") << m_DestPath.GetPath();

	dc << _T("\nm_pCpyStatDlg : ") << (const void*)m_pCpyStatDlg;

	dc << _T("\nm_hCursor : ") << (const void*)m_hCursor;
	}
	
void COXCopyTree::AssertValid() const
	{
	CObject::AssertValid();
	}
#endif

COXCopyTree::~COXCopyTree()
	{
	// Don't delete it, we didn't construct tist object
	m_pCpyStatDlg = NULL; 
	}
	
// protected:

// private:

// ==========================================================================
