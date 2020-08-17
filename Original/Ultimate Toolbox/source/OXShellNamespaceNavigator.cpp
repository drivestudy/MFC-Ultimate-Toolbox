// OXShellNamespaceNavigator.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXShellNamespaceNavigator.h"

#include "UTBStrOp.h"

 #include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXShellNamespaceNavigator

COXShellNamespaceNavigator::COXShellNamespaceNavigator()
{
	//get the shell's IMalloc pointer
	//we'll keep this until we get destroyed
	if(FAILED(SHGetMalloc(&m_pMalloc)))
	{
		AfxThrowMemoryException();
		return;
	}

	m_pOwnerWnd=NULL;

	m_lpeidl=NULL;
	m_bEnumeratorInitialized=FALSE;
	m_lpEnumerateFolder=NULL;
	m_lpEnumerateParentFullIDL=NULL;

	m_bAutoCleanUp=FALSE;
}

COXShellNamespaceNavigator::~COXShellNamespaceNavigator()
{
	if(m_pMalloc)
	{
		if(m_bAutoCleanUp)
		{
			// free memory allocated for objects
			POSITION pos=m_mapObjectsToFree.GetStartPosition();
			while(pos!=NULL)
			{
				DWORD_PTR dwKey=NULL;
				DWORD_PTR dwData=NULL;
				m_mapObjectsToFree.GetNextAssoc(pos,dwKey,dwData);
				m_pMalloc->Free((void*)dwKey);
			}
		}

		// release IMalloc interface
		m_pMalloc->Release();
	}

	if(m_bAutoCleanUp)
	{
		// release requested interfaces
		POSITION pos=m_mapIShellFolderToRelease.GetStartPosition();
		while(pos!=NULL)
		{
			DWORD_PTR dwKey=NULL;
			DWORD_PTR dwData=NULL;
			m_mapIShellFolderToRelease.GetNextAssoc(pos,dwKey,dwData);
			((LPSHELLFOLDER)dwKey)->Release();
		}
	}

	// release enumerator
	if(m_bEnumeratorInitialized)
		ReleaseObjectsEnumerator();
}


/////////////////////////////////////////////////////////////////////////////

int CALLBACK COXShellNamespaceNavigator::CompareObjectsProc(LPARAM lParam1, 
															LPARAM lParam2, 
															LPARAM lParamSort)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	UNREFERENCED_PARAMETER(lParamSort);

    LPNAMESPACEOBJECT lpFolder1=(LPNAMESPACEOBJECT)lParam1;
    LPNAMESPACEOBJECT lpFolder2=(LPNAMESPACEOBJECT)lParam2;

	ASSERT(lpFolder1->lpsfParent==lpFolder2->lpsfParent);

	// Use IShellBrowser::CompareIDs method to compare two folders.
	// The first argument have to be zero.
	// If this method is successful, the CODE field of the status code (SCODE) 
	// contains one of the following values: 
	//		Less than zero		-	The first item should precede the second 
	//								(pidl1 < pidl2).  
	//		Greater than zero	-	The first item should follow the second 
	//								(pidl1 > pidl2).  
	//		Zero				-	The two items are the same (pidl1 = pidl2).  
	//
	// If this method is unsuccessful, it returns an OLE-defined error code. 
	HRESULT hResult=lpFolder1->lpsfParent->
		CompareIDs(0,lpFolder1->lpRelativeIDL,lpFolder2->lpRelativeIDL);

	// if above method failed let's take it as items are equal
    if(FAILED(hResult))
       return 0;

	// return CODE field of status code of result value
    return (short)SCODE_CODE(GetScode(hResult));
}

/////////////////////////////////////////////////////////////////////////////

HIMAGELIST COXShellNamespaceNavigator::
	GetShellImageList(BOOL bSmallIcon/*=TRUE*/) const
{
	// Retrieves the handle to the system image list
	// Use SHGetFileInfo function in order to do that
	//
/*
WINSHELLAPI DWORD WINAPI SHGetFileInfo(
    LPCTSTR pszPath, 	
    DWORD dwFileAttributes, 	
    SHFILEINFO FAR *psfi, 	
    UINT cbFileInfo, 	
    UINT uFlags	
);	

Retrieves information about an object in the file system, such as a file, a folder, 
a directory, or a drive root. 

If uFlags contains SHGFI_SYSICONINDEX, the return value is the handle to the system 
image list that contains the large icon images. If SHGFI_SMALLICON is included with 
SHGFI_SYSICONINDEX, the return value is the handle to the image list that contains 
the small icon images. 

pszPath			-	Address of a buffer that contains the path and file name. Both 
					absolute and relative paths are valid. This string can use either 
					short (the 8.3 form) or long file names. 

dwFileAttributes-	Combination of one or more file attribute flags (FILE_ATTRIBUTE_ 
					values). If uFlags does not include the SHGFI_USEFILEATTRIBUTES 
					flag, this parameter is ignored. 
psfi			-	Address of a SHFILEINFO structure to receive the file information. 
cbFileInfo		-	Size, in bytes, of the SHFILEINFO structure pointed to by the 
					psfi parameter. 
uFlags			-	Flags that specify the file information to retrieve. This 
					parameter can be a combination of the following values (not all
					of them are listed): 
					
					SHGFI_ICON			-	Retrieve the handle to the icon that 
											represents the file and the index of the 
											icon within the system image list. The 
											handle is copied to the hIcon member of 
											the structure specified by psfi, and the 
											index is copied to the iIcon member. The 
											return value is the handle to the system 
											image list.  
					SHGFI_ICONLOCATION	-	Retrieve the name of the file that 
											contains the icon representing the file. 
											The name is copied to the szDisplayName 
											member of the structure specified in psfi.  
					SHGFI_LARGEICON		-	Modify SHGFI_ICON, causing the function 
											to retrieve the file's large icon.  
					SHGFI_LINKOVERLAY	-	Modify SHGFI_ICON, causing the function 
											to add the link overlay to the file's icon.  
					SHGFI_OPENICON		-	Modify SHGFI_ICON, causing the function 
											to retrieve the file's open icon. 
											A container object displays an open icon 
											to indicate that the container is open.  
					SHGFI_SELECTED		-	Modify SHGFI_ICON, causing the function 
											to blend the file's icon with the system 
											highlight color.  
					SHGFI_SHELLICONSIZE	-	Modify SHGFI_ICON, causing the function 
											to retrieve a shell-sized icon. If this 
											flag is not specified the function sizes 
											the icon according to the system metric 
											values.  
					SHGFI_SMALLICON		-	Modify SHGFI_ICON, causing the function 
											to retrieve the file's small icon.  
					SHGFI_SYSICONINDEX	-	Retrieve the index of the icon within the 
											system image list. The index is copied to 
											the iIcon member of the structure 
											specified by psfi. The return value is 
											the handle to the system image list.  
*/

    SHFILEINFO shfi;
	// use fake path, ignore dwFileAttributes and set uFlags to retrieve 
	// small icons
    HIMAGELIST hImageList=(HIMAGELIST)SHGetFileInfo(/*_T("C:\\")*/_T(""),0,
		&shfi,sizeof(SHFILEINFO),SHGFI_SYSICONINDEX|
		(bSmallIcon ? SHGFI_SMALLICON : SHGFI_LARGEICON));

	return hImageList;
}

LPSHELLFOLDER COXShellNamespaceNavigator::
GetShellFolder(CString sFolderFullPath) const
{
	// First of all get the Desktop's IShellFolder interface. 
	LPSHELLFOLDER lpDesktopFolder;
    if(FAILED(SHGetDesktopFolder(&lpDesktopFolder)))
		return NULL;

	// If lpcsFolderFullPath is NULL then just return the Desktop's IShellFolder
	if(sFolderFullPath.IsEmpty())
	{
		ASSERT(lpDesktopFolder!=NULL);
		if(lpDesktopFolder!=NULL)
			m_mapIShellFolderToRelease.SetAt((DWORD_PTR)lpDesktopFolder,NULL);
		return lpDesktopFolder;
	}

	// Now we should try to get IShellFolder interface of the folder full path of 
	// which is lpcsFolderFullPath. In order to do that we use
	// IShellFolder::ParseDisplayName method:
	//
/*
HRESULT ParseDisplayName(
    HWND hwndOwner,
    LPBC pbcReserved,
    LPOLESTR lpszDisplayName,
    ULONG *pchEaten,
    LPITEMIDLIST *ppidl,
    ULONG *pdwAttributes
   );	

Translates a file object's or folder's display name into an item identifier list. 

Returns NOERROR if successful, or an OLE-defined error value otherwise. 

hwndOwner		-	Handle to the owner window that the client should specify if it 
					displays a dialog box or message box. 
pbcReserved		-	Reserved; this parameter is always NULL. 
lpszDisplayName	-	Address of a null-terminated UNICODE string specifying the display
					name. This parameter must be a display name for parsing—that is, 
					a display name retrieved using the SHGDN_FORPARSING value. 
pchEaten		-	Address of a ULONG value that receives the number of characters 
					of the display name that was parsed. 
ppidl			-	Address of a pointer to the new item identifier list for the 
					object. If an error occurs, NULL is returned in this address. 
					The returned item identifier list specifies the relative path 
					(from the parent folder) that corresponds to the specified 
					display name. It contains only one SHITEMID structure followed 
					by a terminating zero. 
pdwAttributes	-	Address of the attributes of the file object. Can be NULL if the 
					caller does not need attribute data. 
*/
	//
	// Note that the display name should be in UNICODE and that resulting PIDL is 
	// relative to parent folder
	//
    
	// The MultiByteToWideChar function maps a character string to a wide-character 
	// (Unicode) string. The character string mapped by this function is not 
	// necessarily from a multibyte character set. 
	// Use CP_ACP as indicator of translating from ANSI code page. Refer to the 
	// documentation to get info on other parameters.
	//
	OLECHAR unicodeFolderPath[MAX_PATH];
#ifndef _UNICODE
	UTBStr::mbstowcs(unicodeFolderPath,MAX_PATH,sFolderFullPath,MAX_PATH);
#else
	UTBStr::tcscpy(unicodeFolderPath, MAX_PATH, sFolderFullPath);
#endif


    ULONG chEaten;
	LPITEMIDLIST lpidl;
    ULONG dwAttributes;
    HRESULT hResult=lpDesktopFolder->
		ParseDisplayName(m_pOwnerWnd!=NULL ? m_pOwnerWnd->GetSafeHwnd() : NULL,
		NULL,unicodeFolderPath,&chEaten,&lpidl,&dwAttributes);
	if(FAILED(hResult))
	{
		m_pMalloc->Free(lpidl);

		// just return the desktop folder
		ASSERT(lpDesktopFolder!=NULL);
		if(lpDesktopFolder!=NULL)
			m_mapIShellFolderToRelease.SetAt((DWORD_PTR)lpDesktopFolder,NULL);
        return lpDesktopFolder;
	}


	// After we get IDL of lpcsFolderFullPath we should retrieve its IShellFolder
	// interface. We accomplish it using IShellFolder::BindToObject
	//
/*
HRESULT BindToObject(
    LPCITEMIDLIST pidl,
    LPBC pbcReserved,
    REFIID riid,
    LPVOID *ppvOut
   );	

Retrieves an IShellFolder object for a subfolder. 

Returns NOERROR if successful, or an OLE-defined error value otherwise. 

pidl		-	Address of an ITEMIDLIST structure that identifies the subfolder 
				relative to its parent folder. 
pbcReserved -	Reserved. Callers should specify NULL for this parameter; those 
				called should ignore it. 
riid		-	Identifier of the interface to return. This parameter must point to 
				the IID_IShellFolder interface identifier. 
ppvOut		-	Address that receives the interface pointer. If an error occurs, 
				a NULL pointer is returned in this address. 
*/
	// 
	// Note that pidl have to be relative to its parent folder but as long as we are
	// going to call this method of desktop folder interface then the fully qualified
	// pidl we got using ParseDisplayName method can be used.
	//

	LPSHELLFOLDER lpFolder;
	hResult=lpDesktopFolder->
		BindToObject(lpidl,0, IID_IShellFolder,(LPVOID*)&lpFolder);

	m_pMalloc->Free(lpidl);
	if(FAILED(hResult))
	{
		ASSERT(lpDesktopFolder!=NULL);
		if(lpDesktopFolder!=NULL)
			m_mapIShellFolderToRelease.SetAt((DWORD_PTR)lpDesktopFolder,NULL);
		return lpDesktopFolder;
	}
	else
	{
		// release desktop folder
		lpDesktopFolder->Release();

		ASSERT(lpFolder!=NULL);
		if(lpFolder!=NULL)
			m_mapIShellFolderToRelease.SetAt((DWORD_PTR)lpFolder,NULL);
		return lpFolder;
	}
}


LPSHELLFOLDER COXShellNamespaceNavigator::GetShellFolder(LPCITEMIDLIST pidlFull) const
{
	// First of all get the Desktop's IShellFolder interface. 
	LPSHELLFOLDER lpDesktopFolder;
    if(FAILED(SHGetDesktopFolder(&lpDesktopFolder)))
		return NULL;

	// If pidlFull is NULL then just return the Desktop's IShellFolder
	if(pidlFull==NULL)
	{
		ASSERT(lpDesktopFolder!=NULL);
		if(lpDesktopFolder!=NULL)
			m_mapIShellFolderToRelease.SetAt((DWORD_PTR)lpDesktopFolder,NULL);
		return lpDesktopFolder;
	}

	// If we have valid IDL we should retrieve its IShellFolder interface. We accomplish it 
	// using IShellFolder::BindToObject
	//
/*
HRESULT BindToObject(
    LPCITEMIDLIST pidl,
    LPBC pbcReserved,
    REFIID riid,
    LPVOID *ppvOut
   );	

Retrieves an IShellFolder object for a subfolder. 

Returns NOERROR if successful, or an OLE-defined error value otherwise. 

pidl		-	Address of an ITEMIDLIST structure that identifies the subfolder 
				relative to its parent folder. 
pbcReserved -	Reserved. Callers should specify NULL for this parameter; those 
				called should ignore it. 
riid		-	Identifier of the interface to return. This parameter must point to 
				the IID_IShellFolder interface identifier. 
ppvOut		-	Address that receives the interface pointer. If an error occurs, 
				a NULL pointer is returned in this address. 
*/
	// 
	// Note that pidl have to be relative to its parent folder but as long as we are
	// going to call this method of desktop folder interface then the fully qualified
	// pidl we got using ParseDisplayName method can be used.
	//

	LPSHELLFOLDER lpFolder;
	HRESULT hResult=lpDesktopFolder->
		BindToObject(pidlFull,0,IID_IShellFolder,(LPVOID*)&lpFolder);

	if(FAILED(hResult))
	{
		ASSERT(lpDesktopFolder!=NULL);
		if(lpDesktopFolder!=NULL)
			m_mapIShellFolderToRelease.SetAt((DWORD_PTR)lpDesktopFolder,NULL);
		return lpDesktopFolder;
	}
	else
	{
		// release desktop folder
		lpDesktopFolder->Release();

		ASSERT(lpFolder!=NULL);
		if(lpFolder!=NULL)
			m_mapIShellFolderToRelease.SetAt((DWORD_PTR)lpFolder,NULL);
		return lpFolder;
	}
}


LPSHELLFOLDER COXShellNamespaceNavigator::
GetShellFolder(const LPSHELLFOLDER lpParentFolder, 
			   const LPITEMIDLIST lpRelativeIDL) const
{
	ASSERT(lpParentFolder!=NULL);

	LPSHELLFOLDER lpsf;
	// We use IShellFolder::BindToObject method to get IShellFolder interface
	// of expanded item. Don't forget that this interface take relative PIDL as
	// an argument
	HRESULT hResult=lpParentFolder->BindToObject(lpRelativeIDL,0,
		IID_IShellFolder,(LPVOID*)&lpsf);

	if(SUCCEEDED(hResult))
		return lpsf;
	else
		return NULL;
}


BOOL COXShellNamespaceNavigator::
GetShellFolderFullIDL(CString sFolderFullPath,
					  LPITEMIDLIST* ppidlFull) const
{
	ASSERT(ppidlFull!=NULL);

	// If lpcsFolderFullPath is NULL then just return the Desktop's IDL which is NULL
	if(sFolderFullPath.IsEmpty())
	{
		*ppidlFull=NULL;
		return TRUE;
	}

	// First of all get the Desktop's IShellFolder interface. 
	LPSHELLFOLDER lpDesktopFolder;
    if(FAILED(SHGetDesktopFolder(&lpDesktopFolder)))
        return FALSE;


	OLECHAR unicodeFolderPath[MAX_PATH];

#ifndef _UNICODE
	UTBStr::mbstowcs(unicodeFolderPath,MAX_PATH,sFolderFullPath,MAX_PATH);
#else
	UTBStr::tcscpy(unicodeFolderPath, MAX_PATH, sFolderFullPath);
#endif

	LPITEMIDLIST lpidl;
    ULONG chEaten;
    ULONG dwAttributes;
    HRESULT hResult=lpDesktopFolder->
		ParseDisplayName(m_pOwnerWnd!=NULL ? m_pOwnerWnd->GetSafeHwnd() : NULL,
		NULL,unicodeFolderPath,&chEaten,&lpidl,&dwAttributes);
	if(FAILED(hResult))
	{
		lpDesktopFolder->Release();
		m_pMalloc->Free(lpidl);
        return FALSE;
	}

	*ppidlFull=CopyPIDL(lpidl);
	lpDesktopFolder->Release();
	m_pMalloc->Free(lpidl);

	return TRUE;
}

BOOL COXShellNamespaceNavigator::
GetShellFolderRelativeIDL(const LPSHELLFOLDER lpParentFolder,
						  CString sFolderRelativePath, 
						  LPITEMIDLIST* ppidlRelative) const
{
	ASSERT(lpParentFolder!=NULL);
	ASSERT(ppidlRelative!=NULL);

	OLECHAR unicodeFolderPath[MAX_PATH];

#ifndef _UNICODE
	UTBStr::mbstowcs(unicodeFolderPath,MAX_PATH,sFolderRelativePath,MAX_PATH);
#else
	UTBStr::tcscpy(unicodeFolderPath, MAX_PATH, sFolderRelativePath);
#endif

	LPITEMIDLIST lpidl;
    ULONG chEaten;
    ULONG dwAttributes;
    HRESULT hResult=lpParentFolder->
		ParseDisplayName(m_pOwnerWnd!=NULL ? m_pOwnerWnd->GetSafeHwnd() : NULL,
		NULL,unicodeFolderPath,&chEaten,&lpidl,&dwAttributes);
	if(FAILED(hResult))
	{
		m_pMalloc->Free(lpidl);
        return FALSE;
	}

	*ppidlRelative=CopyPIDL(lpidl);
	m_pMalloc->Free(lpidl);

	return TRUE;
}


BOOL COXShellNamespaceNavigator::
InitObjectsEnumerator(const LPSHELLFOLDER lpParentFolder, 
					  const LPITEMIDLIST lpParentFullIDL,
					  DWORD dwFlags/*=SHCONTF_FOLDERS|SHCONTF_NONFOLDERS|
									SHCONTF_INCLUDEHIDDEN*/)
{
	ASSERT(lpParentFolder!=NULL);

	if(m_bEnumeratorInitialized)
	{
		TRACE(_T("COXShellNamespaceNavigator::InitObjectsEnumerator: folder items enumerator has already been initialized.\n"));
		TRACE(_T("call COXShellNamespaceNavigator::ReleaseObjectsEnumerator() before.\n"));
		return FALSE;
	}

	// To enumerate object within given folder we use IShellFolder::EnumObjects
	// method to get a pointer to IEnumIDList interface. 
	//
/*
HRESULT EnumObjects(
    HWND hwndOwner,
    DWORD grfFlags,
    LPENUMIDLIST *ppenumIDList
   );	

Determines the contents of a folder by creating an item enumeration object (a set of 
item identifiers) that can be retrieved using the IEnumIDList interface. 

Returns NOERROR if successful, or an OLE-defined error value otherwise. 

hwndOwner		-	Handle to the owner window that the client should specify if it 
					displays a dialog box or message box. 
grfFlags		-	Flags indicating which items to include in the enumeration. 
					SHCONTF_FOLDERS			-	Include items that are folders in the 
												enumeration. 
					SHCONTF_NONFOLDERS		-	Include items that are not folders in 
												the enumeration. 
					SHCONTF_INCLUDEHIDDEN	-	Include hidden items in the 
												enumeration. 
ppenumIDList	-	Address that receives a pointer to the IEnumIDList interface 
					created by this method. If an error occurs, a NULL pointer is 
					returned in this address. 
*/
	ASSERT(m_lpeidl==NULL);
	ASSERT(m_lpEnumerateFolder==NULL);

	HRESULT hResult=lpParentFolder->
		EnumObjects(m_pOwnerWnd!=NULL ? m_pOwnerWnd->GetSafeHwnd() : NULL,
		dwFlags,&m_lpeidl);

	if(FAILED(hResult))
		return FALSE;

	// Call IEnumIDList::Reset() method to make sure we are at the begining of
	// the enumerating list
	m_lpeidl->Reset();

	// After we get pointer to IEnumIDList interface we can use its methods to
	// retrieve information about all objects of given lpFolder
	//
	m_lpEnumerateFolder=lpParentFolder;
	m_bEnumeratorInitialized=TRUE;
	m_lpEnumerateParentFullIDL=lpParentFullIDL;

	return TRUE;
}


BOOL COXShellNamespaceNavigator::ReleaseObjectsEnumerator()
{

	if(!m_bEnumeratorInitialized)
	{
		TRACE(_T("COXShellNamespaceNavigator::ReleaseObjectsEnumerator: there is nothing to release.\n"));
		return FALSE;
	}

	ASSERT(m_lpeidl!=NULL);
	m_lpeidl->Release();
	m_lpeidl=NULL;

	ASSERT(m_lpEnumerateFolder!=NULL);
	m_lpEnumerateFolder=NULL;
	m_lpEnumerateParentFullIDL=NULL;

	m_bEnumeratorInitialized=FALSE;

	return TRUE;
}


LPNAMESPACEOBJECT COXShellNamespaceNavigator::
EnumerateObjectNext(BOOL& bLastReached)
{
	ASSERT(m_bEnumeratorInitialized);
	ASSERT(m_lpeidl!=NULL);

	bLastReached=FALSE;

	// Loop throught the list using IEnumIDList::Next method
/*
HRESULT Next(
    ULONG celt,	
    LPITEMIDLIST *rgelt,	
    ULONG *pceltFetched	
   );

Retrieves the specified number of item identifiers in the enumeration sequence and 
advances the current position by the number of items retrieved. 

Returns NOERROR if successful, S_FALSE if there are no more items in the enumeration 
sequence, or an OLE-defined error value otherwise. 

celt			-	Specifies the number of elements in the array pointed to by the 
					rgelt parameter. 
rgelt			-	Address of an array of ITEMIDLIST pointers that receives the 
					item identifiers. The implementation must allocate these item 
					identifiers using the shell's allocator (retrieved by the 
					SHGetMalloc function). The calling application is responsible 
					for freeing the item identifiers using the shell's allocator. 
pceltFetched	-	Address of a value that receives a count of the item identifiers 
					actually returned in rgelt. The count can be smaller than the 
					value specified in the celt parameter. This parameter can be 
					NULL only if celt is one. 

If this method returns any value other than NOERROR, no entries in the rgelt array 
are valid on exit. They are all in an indeterminate state. 
*/

    LPITEMIDLIST lpidl;
    ULONG celtFetched;
	if(m_lpeidl->Next(1,&lpidl,&celtFetched)!=S_OK)
	{
		bLastReached=TRUE;
		return NULL;
	}

	LPNAMESPACEOBJECT lpNameSpaceObject=
		GetNameSpaceObject(m_lpEnumerateFolder,lpidl,
		ConcatenatePIDLs(m_lpEnumerateParentFullIDL,lpidl));

	m_pMalloc->Free(lpidl);  

	return lpNameSpaceObject;
}


LPNAMESPACEOBJECT COXShellNamespaceNavigator::
GetNameSpaceObject(const LPSHELLFOLDER lpsfParent,
				   const LPITEMIDLIST lpRelativeIDL, 
				   const LPITEMIDLIST lpFullIDL) const
{
	ASSERT(lpsfParent!=NULL);
	ASSERT(lpRelativeIDL!=NULL);

	// Information about currently enumerated object can be retrieved using
	// IShellFolder::GetAttributesOf method
	//
/*
HRESULT GetAttributesOf(
    UINT cidl,
    LPCITEMIDLIST *apidl,
    ULONG *rgfInOut
   );	

Retrieves the attributes of one or more file objects or subfolders. 

Returns NOERROR if successful, or an OLE-defined error value otherwise. 

cidl			-	Number of file objects from which to retrieve attributes. 
apidl			-	Address of an array of pointers to ITEMIDLIST structures, each 
					of which uniquely identifies a file object relative to the parent 
					folder. Each ITEMIDLIST structure must contain exactly one 
					SHITEMID structure followed by a terminating zero. 
rgfInOut		-	Address of a single ULONG value that, on entry, contains the 
					attributes that the caller is requesting. On exit, this value 
					contains the requested attributes that are common to all of the 
					specified objects. Note that this is the address of a single 
					ULONG value, not an array of ULONG values. The lists below 
					describe the possible flags for this parameter. 

For list of flags refer to the documentation. I list here only ones we are
particular interested in:

SFGAO_HASSUBFOLDER		-	The specified folders have subfolders (and are, therefore, 
							expandable in the left pane of Windows Explorer).  
SFGAO_FILESYSTEM		-	The specified folders or file objects are part of the 
							file system (that is, they are files, directories, or 
							root directories).  
SFGAO_FILESYSANCESTOR	-	The specified folders contain one or more file system 
							folders.  
SFGAO_FOLDER			-	The specified items are folders.  
SFGAO_REMOVABLE			-	The specified file objects or folders are on removable 
							media.  
SFGAO_VALIDATE			-	Validate cached information. The shell will validate that 
							the objects specified in a pidl still exist and will not 
							used cached information when retrieving the attributes. 
							If one or more of the items specified in a pidl no longer 
							exist, this method will return an error code. If cidl is 
							zero, the shell will discard all cached information for 
							the shell folder. This is similar to doing a refresh of 
							the folder.  
*/
	DWORD dwFilter=0xffffffff;
	lpsfParent->GetAttributesOf(1,(LPCITEMIDLIST*)&lpRelativeIDL,&dwFilter);

	// First of all, allocate memory for NAMESPACEOBJECT structure
	// using our m_pMalloc object
	LPNAMESPACEOBJECT lpNameSpaceObject=(LPNAMESPACEOBJECT)m_pMalloc->
		Alloc(sizeof(NAMESPACEOBJECT));
	// IMalloc::Alloc returns NULL if insufficient memory
	if(lpNameSpaceObject==NULL)
		return NULL;

	// store parent folder's IShellFolder
	lpNameSpaceObject->lpsfParent=lpsfParent;
	// whenever you save a pointer to COM object don't forget to 
	// add reference
	lpsfParent->AddRef();

	// make copy of enumerated shell object relative PIDL to save 
	// it in NAMESPACEOBJECT structure
	lpNameSpaceObject->lpRelativeIDL=CopyPIDL(lpRelativeIDL);
	// Create full qualified PIDL by concatenating parent's full
	// qualified PIDL lpParentFullIDL with current relative PIDL lpidl
	lpNameSpaceObject->lpFullIDL=CopyPIDL(lpFullIDL);
					
	// We save text, images (including selected) and filter flags 
	// for the object
	lpNameSpaceObject->dwFlags=dwFilter;

	// The item text is the display name of the object. 
	CString sDisplayName=GetDisplayName(lpsfParent,lpRelativeIDL);
	if(sDisplayName.IsEmpty())
		return NULL;

	lstrcpy(lpNameSpaceObject->szDisplayName,(LPCTSTR)sDisplayName);

	// The last thing we have to get are item's icons for normal
	// and selected state. Use COXShellNamespaceNavigator::GetShellImageList
	// function in order to retrieve image list of all icons used to 
	// display shell objects. Now we need to request item index of 
	// particular shell object. To accomplish this task we use 
	// SHGetFileInfo function (remember that we have to use fully qualified 
	// PIDL with that function)
	//

	// SHFILEINFO structure to be used with SH* functions
	SHFILEINFO sfi;

	// Request icon index for normal state
	SHGetFileInfo((LPCTSTR)lpNameSpaceObject->lpFullIDL,0,&sfi,
		sizeof(SHFILEINFO),SHGFI_PIDL|SHGFI_SYSICONINDEX|SHGFI_SMALLICON);
	lpNameSpaceObject->nImageSmall=sfi.iIcon;

	// Request icon index for selected state
	SHGetFileInfo((LPCTSTR)lpNameSpaceObject->lpFullIDL,0,&sfi,
		sizeof(SHFILEINFO),SHGFI_PIDL|SHGFI_SYSICONINDEX|
		SHGFI_SMALLICON|SHGFI_OPENICON);
	lpNameSpaceObject->nImageSelectedSmall=sfi.iIcon;

	// Request icon index for normal state
	SHGetFileInfo((LPCTSTR)lpNameSpaceObject->lpFullIDL,0,&sfi,
		sizeof(SHFILEINFO),SHGFI_PIDL|SHGFI_SYSICONINDEX|SHGFI_LARGEICON);
	lpNameSpaceObject->nImage=sfi.iIcon;

	// Request icon index for selected state
	SHGetFileInfo((LPCTSTR)lpNameSpaceObject->lpFullIDL,0,&sfi,
		sizeof(SHFILEINFO),SHGFI_PIDL|SHGFI_SYSICONINDEX|
		SHGFI_LARGEICON|SHGFI_OPENICON);
	lpNameSpaceObject->nImageSelected=sfi.iIcon;

	return lpNameSpaceObject;
}


HMENU COXShellNamespaceNavigator::
	GetObjectContextMenu(const LPSHELLFOLDER lpParentFolder,
	const LPITEMIDLIST lpRelativeIDL, DWORD dwFlags) const
{
	// We can request IContextMenu interface using 
	// IShellFolder::GetUIObjectOf method.
	//
/*
HRESULT GetUIObjectOf(
    HWND hwndOwner,
    UINT cidl,
    LPCITEMIDLIST *apidl,
    REFIID riid,
    UINT *prgfInOut,
    LPVOID *ppvOut
   );	

Retrieves an OLE interface that can be used to carry out actions on the specified 
file objects or folders. 

Returns NOERROR if successful, E_NOINTERFACE if the interface is not supported, or an 
OLE-defined error value otherwise. 

hwndOwner		-	Handle to the owner window that the client should specify if it 
					displays a dialog box or message box. 
cidl			-	Number of file objects or subfolders specified in the apidl 
					parameter. 
apidl			-	Address of an array of pointers to ITEMIDLIST structures, each of 
					which uniquely identifies a file object or subfolder relative to 
					the parent folder. Each item identifier list must contain exactly 
					one SHITEMID structure followed by a terminating zero. 
riid			-	Identifier of the COM interface object to return. This can be any 
					valid interface identifier that can be created for an item. The 
					most common identifiers used by the shell are listed in the 
					comments at the end of this reference. 
prgfInOut		-	Reserved. 
ppvOut			-	Address that receives the interface pointer. If an error occurs, 
					a NULL pointer is returned in this address. 

If cidl is greater than one, the GetUIObjectOf implementation should only succeed if 
it can create one object for all items specified in apidl. If the implementation 
cannot create one object for all items, this method should fail. 

The following are the most common interface identifiers the shell uses when 
requesting an interface from this method. The list also indicates if cidl can be 
greater than one for the requested interface. 

Interface Identifier	Allowed cidl Value  
IContextMenu			The cidl parameter can be greater than or equal to one.  
IContextMenu2			The cidl parameter can be greater than or equal to one.  
IDataObject				The cidl parameter can be greater than or equal to one.  
IDropTarget				The cidl parameter can only be one.  
IExtractIcon			The cidl parameter can only be one.  
IQueryInfo				The cidl parameter can only be one.  
*/
	LPCONTEXTMENU lpcm;
    HRESULT hResult=lpParentFolder->
		GetUIObjectOf(m_pOwnerWnd!=NULL ? m_pOwnerWnd->GetSafeHwnd() : NULL, 1,
		(const struct _ITEMIDLIST**)&(lpRelativeIDL),IID_IContextMenu,0,
		(LPVOID*)&lpcm);
	if(FAILED(hResult))  
		return NULL;

	// Use IContextMenu::QueryContextMenu method to get handle to popup menu.
	//
/*
HRESULT QueryContextMenu(
    HMENU hmenu,
    UINT indexMenu,
    UINT idCmdFirst,
    UINT idCmdLast,
    UINT uFlags
   );

Adds menu items to the specified menu. The menu items should be inserted in the menu 
at the position specified by indexMenu, and their menu item identifiers must be 
between the idCmdFirst and idCmdLast parameter values. 

Returns an HRESULT structure in which, if the method is successful, the code member 
contains the menu identifier of the last menu item added plus one. 

hmenu		-	Handle to the menu. The handler should specify this handle when 
				adding menu items. 
indexMenu	-	Zero-based position at which to insert the first menu item. 
idCmdFirst	-	Minimum value that the handler can specify for a menu item identifier. 
idCmdLast	-	Maximum value that the handler can specify for a menu item identifier. 
uFlags		-	Optional flags specifying how the context menu can be changed. Can be 
				any combination of the following values: 
				CMF_CANRENAME		-	This flag is set if the calling application 
										supports renaming of items. A context menu 
										extension or drag-and-drop handler should 
										ignore this flag. A namespace extension should 
										add a rename item to the menu if applicable.  
				CMF_DEFAULTONLY		-	This flag is set when the user is activating 
										the default action, typically by 
										double-clicking. This flag provides a hint for 
										the context menu extension to add nothing if 
										it does not modify the default item in the 
										menu. A context menu extension or 
										drag-and-drop handler should not add any menu 
										items if this value is specified. A namespace 
										extension should add only the default item 
										(if any).  
				CMF_EXPLORE			-	This flag is set when Windows Explorer's tree 
										window is present. Context menu handlers 
										should ignore this value.  
				CMF_INCLUDESTATIC	-	This flag is set when a static menu is being 
										constructed. Only the browser should use this 
										flag. All other context menu extensions should 
										ignore this flag.  
				CMF_NODEFAULT		-	This flag is set if no item in the menu should 
										be the default item. A context menu extension 
										or drag-and-drop handler should ignore this 
										flag. A namespace extension should not set 
										any of the menu items to the default.  
				CMF_NORMAL			-	Indicates normal operation. A context menu 
										extension, namespace extension, or 
										drag-and-drop handler can add all menu items.  
				CMF_NOVERBS			-	This flag is set for items displayed in the 
										"Send To:" menu. Context menu handlers should 
										ignore this value.  
				CMF_VERBSONLY		-	This flag is set if the context menu is for a 
										shortcut object. Context menu handlers should 
										ignore this value.  

The remaining bits of the low-order word are reserved by the system. The high-order 
word may be used for context-specific communications. The CMF_RESERVED value can be 
used to mask out the low-order word. 

The actual identifier of each menu item should be idCmdFirst plus a menu identifier 
offset in the range zero through (idCmdLast ... idCmdFirst). 
*/
	//
	// Create popup menu and populate it using above described method

	HMENU hMenu=::CreatePopupMenu();
	if(hMenu==NULL)
	{
		TRACE(_T("COXShellNamespaceNavigator::GetObjectContextMenu: CreatePopupMenu() failed\n"));
		return NULL;
	}

	hResult=lpcm->QueryContextMenu(hMenu,0,1,0x7fff,dwFlags);
	if(FAILED(hResult))  
	{
		TRACE(_T("COXShellNamespaceNavigator::GetObjectContextMenu: QueryContextMenu() failed\n"));
		return NULL;
	}

	lpcm->Release();

	return hMenu;
}


BOOL COXShellNamespaceNavigator::InvokeCommand(const LPSHELLFOLDER lpParentFolder, 
											   const LPITEMIDLIST lpRelativeIDL, 
											   UINT nCmdID, DWORD dwFlags) const
{

	// If any command from popup menu was selected then it should be 
	// applied to corresponding shell object. 
	// So we have to fill CMINVOKECOMMANDINFO structure

	CMINVOKECOMMANDINFO cmici={ sizeof(CMINVOKECOMMANDINFO) };
	cmici.fMask=0;
	cmici.hwnd=m_pOwnerWnd!=NULL ? m_pOwnerWnd->GetSafeHwnd() : NULL;
	cmici.lpVerb=LPCSTR((UINT_PTR)nCmdID-1);
	cmici.lpParameters=NULL;
	cmici.lpDirectory=NULL;
	cmici.nShow=SW_SHOWNORMAL;
	cmici.dwHotKey=0;
	cmici.hIcon=NULL;

	// We can request IContextMenu interface using 
	// IShellFolder::GetUIObjectOf method.
	//
/*
HRESULT GetUIObjectOf(
    HWND hwndOwner,
    UINT cidl,
    LPCITEMIDLIST *apidl,
    REFIID riid,
    UINT *prgfInOut,
    LPVOID *ppvOut
   );	

Retrieves an OLE interface that can be used to carry out actions on the specified 
file objects or folders. 

Returns NOERROR if successful, E_NOINTERFACE if the interface is not supported, or an 
OLE-defined error value otherwise. 

hwndOwner		-	Handle to the owner window that the client should specify if it 
					displays a dialog box or message box. 
cidl			-	Number of file objects or subfolders specified in the apidl 
					parameter. 
apidl			-	Address of an array of pointers to ITEMIDLIST structures, each of 
					which uniquely identifies a file object or subfolder relative to 
					the parent folder. Each item identifier list must contain exactly 
					one SHITEMID structure followed by a terminating zero. 
riid			-	Identifier of the COM interface object to return. This can be any 
					valid interface identifier that can be created for an item. The 
					most common identifiers used by the shell are listed in the 
					comments at the end of this reference. 
prgfInOut		-	Reserved. 
ppvOut			-	Address that receives the interface pointer. If an error occurs, 
					a NULL pointer is returned in this address. 

If cidl is greater than one, the GetUIObjectOf implementation should only succeed if 
it can create one object for all items specified in apidl. If the implementation 
cannot create one object for all items, this method should fail. 

The following are the most common interface identifiers the shell uses when 
requesting an interface from this method. The list also indicates if cidl can be 
greater than one for the requested interface. 

Interface Identifier	Allowed cidl Value  
IContextMenu			The cidl parameter can be greater than or equal to one.  
IContextMenu2			The cidl parameter can be greater than or equal to one.  
IDataObject				The cidl parameter can be greater than or equal to one.  
IDropTarget				The cidl parameter can only be one.  
IExtractIcon			The cidl parameter can only be one.  
IQueryInfo				The cidl parameter can only be one.  
*/
	LPCONTEXTMENU lpcm;
    HRESULT hResult=lpParentFolder->
		GetUIObjectOf(m_pOwnerWnd!=NULL ? m_pOwnerWnd->GetSafeHwnd() : NULL, 1,
		(const struct _ITEMIDLIST**)&(lpRelativeIDL), IID_IContextMenu, 0,
		(LPVOID*)&lpcm);
	if(FAILED(hResult))  
		return FALSE;

	// In order to apply specified command we use 
	// IContextMenu::InvokeCommand() method.
/*
HRESULT InvokeCommand(
    LPCMINVOKECOMMANDINFO lpici
   );

Carries out the command associated with a context menu item. 

Returns NOERROR if successful, or an OLE-defined error code otherwise. 

lpici	-	Address of a CMINVOKECOMMANDINFO structure containing information about 
			the command. 
			
			 typedef struct _CMInvokeCommandInfo{ 
				DWORD cbSize; 
				DWORD fMask; 
				HWND hwnd; 
				LPCSTR lpVerb; 
				LPCSTR lpParameters; 
				LPCSTR lpDirectory; 
				int nShow; 
				DWORD dwHotKey; 
				HANDLE hIcon; 
			} CMINVOKECOMMANDINFO, *LPCMINVOKECOMMANDINFO; 

			Contains information about a context menu command. 

			cbSize			-	Contains the size of this structure, in bytes. 
			fMask			-	Zero, or one or more of the following flags: 
								CMIC_MASK_HOTKEY	-	The dwHotKey member is valid.  
								CMIC_MASK_ICON		-	The hIcon member is valid.  
								CMIC_MASK_FLAG_NO_UI-	The system is prevented from 
														displaying user interface 
														elements (for example, error 
														messages) while carrying out 
														a command.  

			hwnd			-	Handle of the window that is the owner of the context 
								menu. An extension can also use this handle as the 
								owner of any message boxes or dialog boxes it displays. 
			lpVerb			-	A 32-bit value that contains zero in the high-order 
								word and a menu-identifier offset of the command to 
								carry out in the low-order word. The shell specifies 
								this value (using the MAKEINTRESOURCE macro) when the 
								user chooses a menu command. If the high-order word 
								is not zero, this member is the address of a 
								null-terminated string specifying the 
								language-independent name of the command to carry out. 
								This member is typically a string when a command is 
								being activated by an application. The system provides 
								predefined constant values for the following command 
								strings. 

								Value				String  
								CMDSTR_NEWFOLDER	"NewFolder"  
								CMDSTR_VIEWDETAILS  "ViewDetails"  
								CMDSTR_VIEWLIST		"ViewList"  

			lpParameters	-	Optional parameters. This member is always NULL for 
								menu items inserted by a shell extension. 
			lpDirectory		-	Optional working directory name. This member is always 
								NULL for menu items inserted by a shell extension. 
			nShow			-	Set of SW_ values to pass to the ShowWindow function 
								if the command displays a window or starts an 
								application. 
			dwHotKey		-	Optional hot key to assign to any application 
								activated by the command. If the fMask parameter does 
								not specify CMIC_MASK_HOTKEY, this member is ignored. 
			hIcon			-	Icon to use for any application activated by the 
								command. If the fMask member does not specify 
								CMIC_MASK_ICON, this member is ignored. 


The shell calls this method when the user chooses a command that the handler added 
to a context menu. This method may also be called by an application without any 
corresponding user action. 
*/
	HMENU hMenu=::CreatePopupMenu();
	if(hMenu==NULL)
	{
		TRACE(_T("COXShellNamespaceNavigator::GetObjectContextMenu: CreatePopupMenu() failed\n"));
		return NULL;
	}

	hResult=lpcm->QueryContextMenu(hMenu,0,1,0x7fff,dwFlags);
	if(FAILED(hResult))  
	{
		TRACE(_T("COXShellNamespaceNavigator::GetObjectContextMenu: QueryContextMenu() failed\n"));
		::DestroyMenu(hMenu);
		return NULL;
	}
	
	hResult=lpcm->InvokeCommand(&cmici);

	::DestroyMenu(hMenu);

	lpcm->Release();

	if(FAILED(hResult))  
	{
		TRACE(_T("COXShellNamespaceNavigator::InvokeCommand: InvokeCommand() failed\n"));
		return FALSE;
	}

	return TRUE;
}


BOOL COXShellNamespaceNavigator::InvokeDefaultCommand(const LPSHELLFOLDER lpParentFolder, 
													  const LPITEMIDLIST lpRelativeIDL) const
{
	// retrieve the default command ID
	HMENU hMenu=GetObjectContextMenu(lpParentFolder,lpRelativeIDL,CMF_DEFAULTONLY);
	if(hMenu==NULL)
	{
		TRACE(_T("COXShellNamespaceNavigator::InvokeDefaultCommand: GetObjectContextMenu() failed\n"));
		return FALSE;
	}

	CMenu menuPopup;
	VERIFY(menuPopup.Attach(hMenu));
	if(menuPopup.GetMenuItemCount()==0)
	{
		TRACE(_T("COXShellNamespaceNavigator::InvokeDefaultCommand: there is no context menu for the specified object\n"));
		return FALSE;
	}

	int nDefaultCmdID=-1;
#if _MFC_VER > 0x0421
	nDefaultCmdID=menuPopup.GetDefaultItem(GMDI_GOINTOPOPUPS,FALSE);
#else
	nDefaultCmdID=::GetMenuDefaultItem(menuPopup.GetSafeHmenu(),
		FALSE,GMDI_GOINTOPOPUPS);
#endif
	if(nDefaultCmdID==-1)
	{
		TRACE(_T("COXShellNamespaceNavigator::InvokeDefaultCommand: there is no default menu item for the specified object\n"));
		return FALSE;
	}
	VERIFY(menuPopup.DestroyMenu());

	return InvokeCommand(lpParentFolder,lpRelativeIDL,nDefaultCmdID,CMF_DEFAULTONLY);
}


CString COXShellNamespaceNavigator::GetDisplayName(const LPSHELLFOLDER lpParentFolder, 
												   const LPITEMIDLIST lpRelativeIDL) const
{
	ASSERT(lpParentFolder!=NULL);

	// To get the display name we
	// can use IShellFolder::GetDisplayNameOf method.
/*
HRESULT GetDisplayNameOf(
    LPCITEMIDLIST pidl,
    DWORD uFlags,
    LPSTRRET lpName
   );

Retrieves the display name for the specified file object or subfolder, returning it 
in a STRRET structure. 

Returns NOERROR if successful, or an OLE-defined error value otherwise. 

pidl	-	Address of an ITEMIDLIST structure that uniquely identifies the file 
			object or subfolder relative to the parent folder. 
uFlags	-	Flags indicating the type of display name to return. This enumeration 
			consists of two sets of values. The first set is contained in bits 0 
			through 7 and specifies if the name is relative to this folder or if the 
			name is a full name. The following are valid values in this set: 

			SHGDN_NORMAL			-	Name is a full name. That is, the name is 
										relative to the desktop and not to any 
										specific folder.  
			SHGDN_INFOLDER			-	Name is relative to the folder that is 
										processing the name.  

			The second set of values is contained in bits 8 through 15. These values 
			are modifiers to the first set and specify the name retrieval options. 
			The following are valid values in this set: 

			SHGDN_NORMAL			-	Name will be used for generic display.  
			SHGDN_FORADDRESSBAR		-	Name will be used for display in the address 
										bar combo box.  
			SHGDN_FORPARSING		-	Name will be used for parsing. That is, it can 
										be passed to ParseDisplayName.  
			SHGDN_INCLUDE_NONFILESYS-	If this flag is set, the calling application 
										is interested in names of any types of items. 
										If this flag is not set, the calling 
										application is only interested in items that 
										are part of the file system. If this flag is 
										not set and the item is not part of the file 
										system, this method should fail.  

lpName	-	Address of a STRRET structure in which to return the display name. The 
			string returned in this structure depends on the type of display name 
			requested. 
			
If the identifier contains the display name (in the local character set), it returns 
the offset to the name. If not, it returns a pointer to the display name string 
(UNICODE) allocated by the task allocator, or it fills in a buffer. The type of string 
returned depends on the type of display specified. 
*/
	//
	// Note that result display name could be in different 
	// character set so we have to convert it.
	STRRET strRet;
	HRESULT hResult=lpParentFolder->
		GetDisplayNameOf(lpRelativeIDL,SHGDN_NORMAL,&strRet);

	if(FAILED(hResult))
	{
		return _T("");
	}

	TCHAR szDisplayName[MAX_PATH];
	// buffer for getting display name of shell object
	switch (strRet.uType)
	{
#ifndef _UNICODE
	case STRRET_WSTR:
		{
			WideCharToMultiByte(CP_ACP,0,strRet.pOleStr,-1,
				szDisplayName,MAX_PATH,NULL,NULL);
			break;
		}
	case STRRET_OFFSET:
		{
			lstrcpy(szDisplayName,(char*)lpRelativeIDL+
				strRet.uOffset);
			break;
		}
	case STRRET_CSTR:
		{
			lstrcpy(szDisplayName,strRet.cStr);
			break;
		}
#else
	case STRRET_WSTR:
		{
			lstrcpyn(szDisplayName,strRet.pOleStr,MAX_PATH);
			break;
		}
	case STRRET_OFFSET:
		{
			_mbstowcsz(szDisplayName,(char*)lpRelativeIDL+
				strRet.uOffset,MAX_PATH);
			break;
		}
	case STRRET_CSTR:
		{
			_mbstowcsz(szDisplayName,strRet.cStr,MAX_PATH);
			break;
		}
#endif
	default:
		{
			ASSERT(FALSE);
			break;
		}
	}

	CString sResult=szDisplayName;
	return sResult;
}

////////////////////////////////////////////////////////////////////////////
/*
LPITEMIDLIST COXShellNamespaceNavigator::ConcatenatePIDLs(const LPCITEMIDLIST pidl1, 
														  const LPCITEMIDLIST pidl2) const
{
	LPITEMIDLIST pidlNew;
	UINT cb1, cb2;
	cb1=cb2=0;

	//are both of these NULL?
	if(!pidl1 && !pidl2)
		return NULL;

	//if pidl1 is NULL, just return a copy of pidl2
	if(!pidl1)
	{
		pidlNew=CopyPIDL(pidl2);
		return pidlNew;
	}

	//if pidl2 is NULL, just return a copy of pidl1
	if(!pidl2)
	{
		pidlNew=CopyPIDL(pidl1);
		return pidlNew;
	}

	cb1=GetIDLSize(pidl1)-sizeof(ITEMIDLIST);
	cb2=GetIDLSize(pidl2);

	//create the new PIDL
	pidlNew=(LPITEMIDLIST)m_pMalloc->Alloc(cb1+cb2);

	if(pidlNew)
	{
		//copy the first PIDL
		CopyMemory(pidlNew,pidl1,cb1);
		//copy the second PIDL
		CopyMemory(((LPBYTE)pidlNew)+cb1,pidl2,cb2);
	}

	return pidlNew;
}

UINT COXShellNamespaceNavigator::GetIDLSize(const LPCITEMIDLIST pidl) const
{
	UINT cbTotal=0;
	LPITEMIDLIST pidlTemp=(LPITEMIDLIST)pidl;

	while(pidlTemp)
	{
		cbTotal+=pidlTemp->mkid.cb;
		pidlTemp=GetNextIDLItem(pidlTemp);
	}  

	//add the size of the NULL terminating ITEMIDLIST
	cbTotal+=sizeof(ITEMIDLIST);

	return (cbTotal);
}
*/

LPITEMIDLIST COXShellNamespaceNavigator::ConcatenatePIDLs(LPCITEMIDLIST pidl1, 
														  LPCITEMIDLIST pidl2) const
{
	LPITEMIDLIST pidlNew;
	UINT cb1, cb2;
	cb1=cb2=0;

	//are both of these NULL?
	if(!pidl1 && !pidl2)
		return NULL;

	//if pidl1 is NULL, just return a copy of pidl2
	if(!pidl1)
	{
		pidlNew=CopyPIDL(pidl2);
		return pidlNew;
	}

	//if pidl2 is NULL, just return a copy of pidl1
	if(!pidl2)
	{
		pidlNew=CopyPIDL(pidl1);
		return pidlNew;
	}


	cb1=GetIDLSize(pidl1)-sizeof(pidl1->mkid.cb);
	cb2=GetIDLSize(pidl2);

	//create the new PIDL
	pidlNew=(LPITEMIDLIST)m_pMalloc->Alloc(cb1+cb2);

	if(pidlNew)
	{
		//copy the first PIDL
		CopyMemory(pidlNew,pidl1,cb1);
		//copy the second PIDL
		CopyMemory(((LPBYTE)pidlNew)+cb1,pidl2,cb2);
	}

	return pidlNew;
}

UINT COXShellNamespaceNavigator::GetIDLSize(LPCITEMIDLIST pidl) const
{
	UINT cbTotal=0;
	LPITEMIDLIST pidlTemp=(LPITEMIDLIST)pidl;

	if(pidlTemp!=NULL)
	{
		// add the size of the NULL terminating mkid.cb  - !!!!2 bytes!!!!
		cbTotal += sizeof(pidlTemp->mkid.cb);
		while(pidlTemp && pidlTemp->mkid.cb)
		{
            cbTotal += pidlTemp->mkid.cb;
			pidlTemp = GetNextIDLItem(pidlTemp);
		}  
	}
    
	return cbTotal;
}


LPITEMIDLIST COXShellNamespaceNavigator::CopyPIDL(LPCITEMIDLIST pidlSource) const
{
	LPITEMIDLIST pidlTarget=NULL;
	UINT cbSource=0;

	if(NULL==pidlSource)
		return (NULL);

	// Allocate the new pidl
	cbSource=GetIDLSize(pidlSource);
	pidlTarget=(LPITEMIDLIST)m_pMalloc->Alloc(cbSource);
	if(!pidlTarget)
		return (NULL);

	// Copy the source to the target
	CopyMemory(pidlTarget,pidlSource,cbSource);

	return pidlTarget;
}


LPITEMIDLIST COXShellNamespaceNavigator::GetNextIDLItem(LPCITEMIDLIST pidl) const
{
	if(pidl && pidl->mkid.cb!=0)
	{
		LPITEMIDLIST pidlNext=(LPITEMIDLIST)(LPBYTE)(((LPBYTE)pidl) + 
			pidl->mkid.cb);
		if(pidlNext->mkid.cb==0)
			return NULL;
		else
			return pidlNext;
	}
	else
		return (NULL);
}


void COXShellNamespaceNavigator::FreeShellObject(void* pVoid) const
{
	ASSERT(m_pMalloc!=NULL);
	m_pMalloc->Free(pVoid);
}


CString COXShellNamespaceNavigator::GetSpecialFolderPath(int nFolder, 
														 CWnd* pWndOwner/*=NULL*/) const
{
	CString sPath(_T(""));

	LPITEMIDLIST lpFullIDL;
	if(FAILED(SHGetSpecialFolderLocation(
		pWndOwner==NULL ? NULL : pWndOwner->GetSafeHwnd(),nFolder,&lpFullIDL)))
	{
		return sPath;
	}

	TCHAR buffer[MAX_PATH];
	if(SHGetPathFromIDList(lpFullIDL,buffer)) 
	{
		sPath=(LPCTSTR)buffer;
	}

	m_pMalloc->Free(lpFullIDL);
	return sPath;
}


BOOL COXShellNamespaceNavigator::RenameShellObject(const LPSHELLFOLDER lpParentFolder,
												   const LPITEMIDLIST lpRelativeIDL, 
												   LPITEMIDLIST* lppNewRelativeIDL, 
												   LPCTSTR lpszNewName) const
{
	ASSERT(lpParentFolder!=NULL);
	ASSERT(lpRelativeIDL!=NULL);
	ASSERT(lppNewRelativeIDL!=NULL);

	OLECHAR unicodeNewName[MAX_PATH];

#ifndef _UNICODE
	UTBStr::mbstowcs(unicodeNewName,MAX_PATH,lpszNewName,MAX_PATH);
#else
	UTBStr::tcscpy(unicodeNewName, MAX_PATH, lpszNewName);
#endif

	if(FAILED(lpParentFolder->
		SetNameOf((m_pOwnerWnd==NULL ? NULL : m_pOwnerWnd->GetSafeHwnd()),
		lpRelativeIDL,unicodeNewName,SHGDN_INFOLDER/*|SHGDN_INCLUDE_NONFILESYS*/,
		lppNewRelativeIDL)))
	{
		TRACE(_T("COXShellNamespaceNavigator::RenameShellObject: failed to set new name for the specified object\n"));
		return FALSE;
	}

	ASSERT(*lppNewRelativeIDL!=NULL);
	return TRUE;
}


LPSHELLFOLDER COXShellNamespaceNavigator::
GetParentShellFolder(LPCITEMIDLIST pidlFull, LPITEMIDLIST* lppRelativeIDL) const
{
	ASSERT(lppRelativeIDL!=NULL);

	if(pidlFull==NULL)
	{
		// Retrieve the Desktop's IShellFolder interface. 
		LPSHELLFOLDER lpDesktopFolder;
		if(FAILED(SHGetDesktopFolder(&lpDesktopFolder)))
			return NULL;

		ASSERT(lpDesktopFolder!=NULL);
		if(lpDesktopFolder!=NULL)
			m_mapIShellFolderToRelease.SetAt((DWORD_PTR)lpDesktopFolder,NULL);
		return lpDesktopFolder;
	}

	UINT cbTotal=0;
	LPITEMIDLIST pidlTemp=(LPITEMIDLIST)pidlFull;

	// add the size of the NULL terminating mkid.cb  - !!!!2 bytes!!!!
	cbTotal+=sizeof(pidlTemp->mkid.cb);
	while(pidlTemp && pidlTemp->mkid.cb)
	{
		LPITEMIDLIST pidlLast=pidlTemp;
		UINT cb=pidlTemp->mkid.cb;
        cbTotal+=cb;
		pidlTemp=GetNextIDLItem(pidlTemp);
		if(pidlTemp==NULL)
		{
	        cbTotal-=cb;
			*lppRelativeIDL=CopyPIDL(pidlLast);
		}
	}  

	LPITEMIDLIST pidlTarget=(LPITEMIDLIST)m_pMalloc->Alloc(cbTotal);
	if(!pidlTarget)
		return (NULL);
	::memset(pidlTarget,0,cbTotal);
	
	// Copy the source to the target
	CopyMemory(pidlTarget,(LPITEMIDLIST)pidlFull,cbTotal-sizeof(pidlTemp->mkid.cb));

	LPSHELLFOLDER pShellFolder=GetShellFolder(pidlTarget);

	m_pMalloc->Free(pidlTarget);

	return pShellFolder;
}


