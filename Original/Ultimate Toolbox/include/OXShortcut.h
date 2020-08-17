// ==========================================================================
// 					Class Specification : COXShortcut
// ==========================================================================
// Header file :		OXShortcut.h
//
// Version: 9.3
//
// Properties:
//		NO		 Abstract class (does not have any objects)
//		YES		 Derived from CObject
//
//		NO		 Is a CWnd.                     
//		NO		 Two stage creation (constructor & Create())
//		NO		 Has a message map
//		NO		 Needs a resource (template)
//
//		NO		 Persistent objects (saveable on disk)      
//		YES		 Uses exceptions
//
// Description:
//
//		The COXShortcut class wraps Win32's IShellLink interface to let you
//		easily create, open, examine, update or resolve a shell link 
//		(also known as a shortcut) without writing any COM codes (except
//		AfxOleInit(), or their equivalence).
//
// Prerequisite:
//
//		AfxOleInit() has to be called directly or indirectly previously
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __OXSHORTCUT_H__
#define __OXSHORTCUT_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#ifndef __AFXOLE_H__
#include <afxole.h> 
#define __AFXOLE_H__
#endif


/////////////////////////////////////////////////////////////////////////////

class OX_CLASS_DECL COXShortcut : public CObject
{
	DECLARE_DYNCREATE(COXShortcut)

// Data members -------------------------------------------------------------
public:
protected:
	CString			m_sCurFileName;
	IShellLink*		m_psl; 
	IPersistFile*	m_ppf; 
	HRESULT			m_hres;
	BOOL			m_bThrowException;

private:

// Member functions ---------------------------------------------------------
public:
	COXShortcut();
	// --- In      :
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Constructor of object

	virtual ~COXShortcut();
	// --- In      :
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Destructor of object

#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

	virtual BOOL Open(LPCTSTR pszPathLink, LPCTSTR pszPathObj = NULL, BOOL bThrowException = FALSE);
	// --- In      : pszPathLink, the pathname of the shortcut file to open or create
	//				 pszPathObj, NULL if opening a shortcut; when creating, the pathname of
	//				 the object file this shortcut points to
	//				 bThrowException, if TRUE, all subsequent member function
	//					calls will throw a COleException when an error occurred (including
	//					this Open()). if FALSE, no exceptions will be thrown (you still
	//					can call GetLastError() to check the error status).
	// --- Out     : 
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : Open or create a shortcut file.

	virtual void Close(BOOL bSave = TRUE);
	// --- In      : bSave, if TRUE, the current shortcut file will be saved before
	//					closing; if FALSE, all changes will be discarded.
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Close a shortcut file.

	CString GetCurFile() const;
	// --- In      :
	// --- Out     :
	// --- Returns : the currently opening shortcut file, if empty, no file opening
	// --- Effect  : retrieve the absolute path to the currently opening shortcut file.

	virtual BOOL IsDirty();
	// --- In      :
	// --- Out     : 
	// --- Returns : TRUE if the shortcut has been modified, FALSE otherwise
	// --- Effect  : determine whether the shortcut has been modified since last save

	HRESULT GetLastError() const;
	// --- In      :
	// --- Out     : 
	// --- Returns : NOERROR if no error, or an HRESULT value indicating the last error
	//				 (could be S_FALSE).
	// --- Effect  : retrieve the HRESULT from the last operation (not thread-safe).

	virtual BOOL Save(LPCTSTR pszFileName = NULL, BOOL fRemember = TRUE);
	// --- In      : pszFileName, pointer to absolute path of the file where this shortcut
	//					is saved, if NULL, use current file name
	//				 fRemember, specifies whether the file is to be the current working 
	//					file or not
	// --- Out     : 
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : "Save" or "Save as" a shortcut.

	BOOL GetArguments(CString& rString);
	// --- In      :
	// --- Out     : rString, the buffer to receive the command-line arguments 
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : retrieves the command-line arguments associated with a shortcut.

	BOOL GetDescription(CString& rString);
	// --- In      :
	// --- Out     : rString, the buffer to receive the description 
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : retrieves the description string for a shortcut.

	BOOL GetHotkey(WORD& wHotKey);
	// --- In      :
	// --- Out     : wHotKey, address of the hot key. The virtual-key code is in the
	//				 low-order byte, and the modifier flags are in the high-order byte.
	//				 The modifier flags can be a combination of these values:
	//
	//						HOTKEYF_ALT		ALT key
	//						HOTKEYF_CONTROL	CTRL key
	//						HOTKEYF_EXT		Extended key
	//						HOTKEYF_SHIFT	SHIFT key
	//
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : retrieves the hot key for a shortcut.

	BOOL GetIconPath(CString& rString);
	// --- In      :
	// --- Out     : rString, the buffer to receive the path name of the icon file 
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : retrieves the path and filename of the icon file for a shortcut.

	BOOL GetIconIndex(int& iIcon);
	// --- In      :
	// --- Out     : iIcon, address of a value that receives the index of the icon
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : retrieves the index of the icon in the icon file for a shortcut.

	BOOL GetIDList(LPITEMIDLIST& ridl);
	// --- In      :
	// --- Out     : ridl, address of a pointer to a list of item identifiers.
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : retrieves the list of item identifiers for a shortcut.
	//				 NOTE: it is your responsibility to call SHGetMalloc() to get an
	//				 allocator pointer to free the memory allocation afterwards.

	BOOL GetPath(CString& rString, BOOL bUNC = TRUE);
	// --- In      : bUNC, if TRUE, retrieves the Universal Naming Convention (UNC)
	//					path for the file; if FALSE, retrieves the standard short (8.3)
	//					filename.
	// --- Out     : rString, the buffer to receive the path name of the shortcut
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : retrieves the path and filename of a shortcut.

	BOOL GetShowCmd(int& iShowCmd);
	// --- In      :
	// --- Out     : iShowCmd, address of the show command
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : retrieves the show command for a shortcut.

	BOOL GetWorkingDirectory(CString& rString);
	// --- In      :
	// --- Out     : rString, the buffer to receive the name of the working directory
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : retrieves the name of the working directory for a shortcut.

	BOOL Resolve(CWnd* pWnd = NULL, DWORD fFlags = SLR_NO_UI | SLR_UPDATE);
	// --- In      : pWnd, a window that the shell uses as the parent window for a dialog
	//					box. the shell displays the dialog box if it needs to prompt the
	//					user for more information while resolving a shortcut
	//				 fFlags, action flags. this parameter can be a combination of these
	//					values: SLR_ANY_MATCH, SLR_NO_UI and SLR_UPDATE (they have the same
	//					meaning as in IShellLink::Resolve()).
	// --- Out     : 
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : resolves a shortcut. the system searches for the shortcut-pointing
	//				 object and, if necessary, updates the shortcut path and its
	//				 list of identifiers.

	BOOL SetArguments(LPCTSTR pszArguments);
	// --- In      : pszArguments, address of a buffer that contains the new command-line
	//					arguments.
	// --- Out     : 
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : sets the command-line arguments for a shortcut. 

	BOOL SetDescription(LPCTSTR pszDescription);
	// --- In      : pszDescription, address of a buffer containing the new description
	//					string.
	// --- Out     : 
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : sets the description for a shortcut.

	BOOL SetHotkey(WORD nHotkey);
	// --- In      : nHotkey, the hot key to set, see GetHotKey() for a detailed description
	// --- Out     : 
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : sets a hot key for a shortcut.

	BOOL SetIconPath(LPCTSTR pszPath);
	// --- In      : pszPath, address of a buffer containing the new path name of the 
	//					icon file 
	// --- Out     : 
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : sets the path and filename of the icon file for a shortcut.

	BOOL SetIconIndex(int nIconIndex = 0);
	// --- In      : nIconIndex, the index of the icon to set
	// --- Out     : 
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : sets the index of the icon in the icon file for a shortcut.

	BOOL SetIDList(LPCITEMIDLIST pidl);
	// --- In      : LPCITEMIDLIST, a pointer to a list of item identifiers to set
	// --- Out     : 
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : sets the list of item identifiers for a shortcut.

	BOOL SetPath(LPCTSTR pszPath);
	// --- In      : pszPath, address of a buffer containing the new path name of the 
	//					shortcut 
	// --- Out     : 
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : sets the path and filename of a shortcut.
	//				 CAUTION: may return S_OK even when the supplied path name is not really
	//				 set as is, if it is not a valid path (non-existing path will be set as is).

	BOOL SetRelativePath(LPCTSTR pszRelPath);
	// --- In      : pszRelPath, address of a buffer containing the relative path to set
	// --- Out     : 
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : sets the relative path to the shortcut. When SetPath() cannot resolve
	//				 the path as an absolute path name, this relative path will be added to the
	//				 front to try once again.

	BOOL SetShowCmd(int nShowCmd);
	// --- In      : nShowCmd, the show command to set
	// --- Out     : 
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : sets the show command for a shortcut.

	BOOL SetWorkingDirectory(LPCTSTR pszWorkingDirectory);
	// --- In      : pszWorkingDirectory, address of a buffer containing the new 
	//					working directory
	// --- Out     : 
	// --- Returns : TRUE if successful, FALSE otherwise
	// --- Effect  : sets the name of the working directory for a shortcut.

protected:
	virtual BOOL Validate(void* pv);
	virtual void ThrowExceptionIfNeccessary();

#ifdef _DEBUG
	virtual void TraceFailure(LPCTSTR sFunctionName) const;
	static CString GetResultMessage(HRESULT hResult);
#endif

private:
};

/////////////////////////////////////////////////////////////////////////////

#include "OXShortcut.inl"

#endif	// __OXSHORTCUT_H__

// end of OXShortcut.h