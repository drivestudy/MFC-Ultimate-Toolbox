// OXShellObjectList.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXShellObjectList.h"
#include "OXMainRes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// delimiter for filter
const TCHAR OXSHELLLIST_EXT_DELIMITER=_T('|');

/////////////////////////////////////////////////////////////////////////////
// COXShellObjectList

COXShellObjectList::COXShellObjectList(BOOL bEnableContextMenu/*=TRUE*/,
									   BOOL bOnlyFileSystemItems/*=TRUE*/,
									   BOOL bNotifyError/*=TRUE*/,
									   LPCTSTR lpszFilter/*=_T("")*/)
{
	SetEnableContextMenu(bEnableContextMenu);
	SetOnlyFileSystemItems(bOnlyFileSystemItems);
	SetNotifyError(bNotifyError);

	SetFilter(lpszFilter);

	m_lpsfCurrentParentFolder=NULL;
	m_lpFullIDLCurrentParentFolder=NULL;

	m_hShellImageListLarge=NULL;
	m_hShellImageListSmall=NULL;

	m_nRedraw=0;

	m_bInitialized=FALSE;
}

COXShellObjectList::~COXShellObjectList()
{
}


BEGIN_MESSAGE_MAP(COXShellObjectList, CListCtrl)
	//{{AFX_MSG_MAP(COXShellObjectList)
	ON_NOTIFY_REFLECT_EX(NM_DBLCLK, OnDblClick)
	ON_NOTIFY_REFLECT_EX(LVN_BEGINLABELEDIT, OnBeginLabelEdit)
	ON_NOTIFY_REFLECT_EX(LVN_ENDLABELEDIT, OnEndLabelEdit)
#ifdef OXSHELLLIST_WATCHFORDIR
	ON_MESSAGE(WM_OX_FILE_NOTIFY, OnDirChangeNotify)
#endif	//	OXSHELLLIST_WATCHFORDIR
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXShellObjectList message handlers

BOOL COXShellObjectList::PopulateList(const LPSHELLFOLDER lpFolder, 
									  const LPITEMIDLIST lpFullIDL)
{
	ASSERT_VALID(this);
	ASSERT(lpFolder!=NULL);

	if(!::IsWindow(GetSafeHwnd()))
		return FALSE;

	if(!m_bInitialized)
	{
		if(!Init())
			return FALSE;
	}

	// Clean up ListCtrl
	DeleteAllItems();

	// Fill ListCtrl with objects in lpFolder folder.
	FillListWithSubitems(lpFolder,lpFullIDL);

	// We cannot sort item using just their display names. We have to 
	// request shell to sort items.
	SortItems(COXShellNamespaceNavigator::CompareObjectsProc,NULL);

	return TRUE;
}

BOOL COXShellObjectList::PopulateList(CString sFolderStartFrom/*=_T("")*/)
{
	ASSERT_VALID(this);

	if(!::IsWindow(GetSafeHwnd()))
		return FALSE;

	if(!m_bInitialized)
	{
		if(!Init())
			return FALSE;
	}

	// now let's fill ListCtrl with shell namespace objects that is located 
	// in sFolderStartFrom folder. If sFolderStartFrom is NULL then we use 
	// Desktop as start folder

	// shell folder returned by that function have to be released afterwards
	LPSHELLFOLDER lpFolder=m_navigator.GetShellFolder(sFolderStartFrom);
	if(lpFolder==NULL)
		return FALSE;

	LPITEMIDLIST lpidlFull;
	if(!m_navigator.GetShellFolderFullIDL(sFolderStartFrom,&lpidlFull))
	{
		// Release the folder
		lpFolder->Release();
		return FALSE;
	}

	// Clean up ListCtrl
	DeleteAllItems();

	// Fill ListCtrl with objects in lpFolder folder.
	FillListWithSubitems(lpFolder,lpidlFull);

	lpFolder->Release();

	// We cannot sort item using just their display names. We have to 
	// request shell to sort items.
	SortItems(COXShellNamespaceNavigator::CompareObjectsProc,NULL);

	return TRUE;
}


CString COXShellObjectList::GetFullPath(int nItem) const
{
	ASSERT(nItem>=0 && nItem<GetItemCount());

	// retrieve associated structure
	LPNAMESPACEOBJECT lpNameSpaceObject=(LPNAMESPACEOBJECT)GetItemData(nItem);
	if(lpNameSpaceObject==NULL)
		return _T("");

	return m_navigator.GetFullPath(lpNameSpaceObject->lpFullIDL);
}


BOOL COXShellObjectList::FillListWithSubitems(const LPSHELLFOLDER lpFolder, 
											  const LPITEMIDLIST lpFullIDL)
{
	ASSERT(lpFolder);

	CWaitCursor wait;

	// Initialize enumeration of objects within given folder
	if(!m_navigator.InitObjectsEnumerator(lpFolder,lpFullIDL))
		return FALSE;

	// After we get pointer to IEnumIDList interface we can use its methods to
	// retrieve information about all objects of given lpFolder
	//

	// structures for inserting new items in our ListCtrl
	LV_ITEM lvi;

	BOOL bLast;
	// Loop through Shell Namespace items
	LPNAMESPACEOBJECT lpNameSpaceObject=m_navigator.EnumerateObjectNext(bLast);
	while(!bLast)
	{
		BOOL bQualified=lpNameSpaceObject!=NULL;
		if(bQualified && GetOnlyFileSystemItems())
		{
			bQualified=(lpNameSpaceObject->dwFlags&SFGAO_FILESYSANCESTOR ||
				lpNameSpaceObject->dwFlags&SFGAO_FILESYSTEM);
		}
		if(bQualified && (lpNameSpaceObject->dwFlags&SFGAO_FILESYSTEM) &&
			!(lpNameSpaceObject->dwFlags&SFGAO_FOLDER))
		{
			bQualified=IsMatchingFilter(lpNameSpaceObject->szDisplayName);
		}
		if(bQualified && IsQualified(lpNameSpaceObject))
		{
			// We define text, image (including selected) and lParam value
			// which is going to be a pointer to NAMESPACEOBJECT structure
			lvi.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
			// NAMESPACEOBJECT structure is already filled
			lvi.lParam=(LPARAM)lpNameSpaceObject;

			// display name
			lvi.pszText=lpNameSpaceObject->szDisplayName;
    
			// image
			int nListImageIndex=-1;
			if(!m_mapImageIndex.Lookup(lpNameSpaceObject->nImageSmall,nListImageIndex))
			{
				nListImageIndex=PtrToInt(m_mapImageIndex.GetCount());
				m_mapImageIndex.SetAt(lpNameSpaceObject->nImageSmall,nListImageIndex);
				HICON hIcon=ImageList_ExtractIcon(
					0,m_hShellImageListLarge,lpNameSpaceObject->nImageSmall);
				ASSERT(hIcon!=NULL);
				lvi.iImage=m_imageListLarge.Add(hIcon);
				ASSERT(lvi.iImage!=-1 && lvi.iImage==nListImageIndex);
				VERIFY(::DestroyIcon(hIcon));
				hIcon=ImageList_ExtractIcon(
					0,m_hShellImageListSmall,lpNameSpaceObject->nImageSmall);
				ASSERT(hIcon!=NULL);
				VERIFY(m_imageListSmall.Add(hIcon)==lvi.iImage);
				VERIFY(::DestroyIcon(hIcon));
			}
			else
			{
				lvi.iImage=nListImageIndex;
			}
			/////////////////////

			// index and subitem
			lvi.iItem=GetItemCount();
			lvi.iSubItem=0;

			// We collected all information we needed. It's time to insert 
			// new item in our ListCtrl.
			int nItem=InsertItem(&lvi);
			ASSERT(nItem!=-1);

			// retrieve info about size, type and modification date of the 
			// file system object
			if(lpNameSpaceObject->dwFlags&SFGAO_FILESYSTEM)
			{
				CString sFullPath=m_navigator.GetFullPath(lpNameSpaceObject->lpFullIDL);
				ASSERT(!sFullPath.IsEmpty());

				CFileStatus fileStatus;
				if(CFile::GetStatus(sFullPath,fileStatus))
				{
					if((lpNameSpaceObject->dwFlags&SFGAO_FOLDER)!=SFGAO_FOLDER)
					{
						// request size
						CString sFileSize;
						sFileSize.Format(_T("%dKB"),
							(fileStatus.m_size>1024 ? fileStatus.m_size/1024 : 1));
						SetItem(nItem,1,LVIF_TEXT,sFileSize,0,0,0,0);
					}

					// modified
					CString sModified=fileStatus.m_mtime.Format(_T("%c"));
					SetItem(nItem,3,LVIF_TEXT,sModified,0,0,0,0);
				}

				SHFILEINFO shfi;
				if(SHGetFileInfo(sFullPath,NULL,&shfi,sizeof(shfi),
					SHGFI_TYPENAME)!=0)
				{
					// file type
					CString sItemType(_T(""));
					sItemType=(LPCTSTR)shfi.szTypeName;
					SetItem(nItem,2,LVIF_TEXT,sItemType,0,0,0,0);
				}
			}
		}
		lpNameSpaceObject=m_navigator.EnumerateObjectNext(bLast);
	}

	// Release enumerator 
	m_navigator.ReleaseObjectsEnumerator();

#ifdef OXSHELLLIST_WATCHFORDIR
	if(m_lpFullIDLCurrentParentFolder!=NULL)
	{
		CString sCurrentFolder=m_navigator.GetFullPath(m_lpFullIDLCurrentParentFolder);
		if(!sCurrentFolder.IsEmpty())
			m_fileWatcher.RemoveWatch(sCurrentFolder);

		sCurrentFolder=m_navigator.GetFullPath(lpFullIDL);
		if(!sCurrentFolder.IsEmpty())
		{
			if(m_fileWatcher.AddWatch(sCurrentFolder,FALSE,
				COXFileWatcher::OXFileWatchChangeDirName|
				COXFileWatcher::OXFileWatchChangeFileName))
			{
				m_fileWatcher.EnableWindowNotification(sCurrentFolder,this,TRUE);
			}
			else
			{
				TRACE(_T("COXShellObjectList::FillListWithSubitems: failed to set a file watcher for the current parent folder\n"));
			}
		}
	}
#endif	//	OXSHELLLIST_WATCHFORDIR

	m_lpsfCurrentParentFolder=lpFolder;
	m_lpFullIDLCurrentParentFolder=lpFullIDL;

	return TRUE;
}


BOOL COXShellObjectList::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);

	// TODO: Add your control notification handler code here
	*pResult = 0;

	// if double click over a folder we expand it or otherwise invoke 
	// the default command for a file
	//

	// Get the item that was double clicked in screen coordinates
	CPoint ptMouseCursor;
	if(!::GetCursorPos(&ptMouseCursor))
	{
		TRACE(_T("COXShellObjectList::OnDblClick: GetCursorPos() failed\n"));
		return FALSE;
	}
	ScreenToClient(&ptMouseCursor);

	// Use HitTest function to define the list item by coordinate
	UINT nFlags=NULL;
	int nItem=HitTest(ptMouseCursor,&nFlags);

	if(nItem!=-1 && (nFlags&LVHT_ONITEM)!=0)
	{
		// Get folder info  associated with item
		LPNAMESPACEOBJECT lpNameSpaceObject=
			(LPNAMESPACEOBJECT)GetItemData(nItem);
		ASSERT(lpNameSpaceObject!=NULL);

		if((lpNameSpaceObject->dwFlags&SFGAO_FOLDER)!=SFGAO_FOLDER) 
		{
			if(!m_navigator.InvokeDefaultCommand(lpNameSpaceObject->lpsfParent,
				lpNameSpaceObject->lpRelativeIDL))
			{
				TRACE(_T("COXShellObjectList::OnDblClick: InvokeDefaultCommand() failed"));
			}
		}
	}

	return FALSE;
}


BOOL COXShellObjectList::Init()
{
	ASSERT_VALID(this);

	if(!::IsWindow(GetSafeHwnd()))
		return FALSE;

	SetNotifyError(m_bNotifyError);

	// use system image list as tree control's image list
	//
	m_mapImageIndex.RemoveAll();

	// large images
	if((HIMAGELIST)m_imageListLarge!=NULL)
		m_imageListLarge.DeleteImageList();
    // retrieve Shell image list
    if(m_hShellImageListLarge==NULL)
	{
		m_hShellImageListLarge=m_navigator.GetShellImageList(FALSE);
		if(m_hShellImageListLarge==NULL)
			return FALSE;
	}
	IMAGEINFO imageInfo;
	VERIFY(ImageList_GetImageInfo(m_hShellImageListLarge,0,&imageInfo));
	CRect rect(imageInfo.rcImage);
	VERIFY(m_imageListLarge.Create(rect.Width(),rect.Height(),ILC_COLOR32|ILC_MASK,0,0));

	SetImageList(&m_imageListLarge,TVSIL_NORMAL);
	////////////////////////////////////////////////

	// small images
	if((HIMAGELIST)m_imageListSmall!=NULL)
		m_imageListSmall.DeleteImageList();
    // retrieve Shell image list
    if(m_hShellImageListSmall==NULL)
	{
		m_hShellImageListSmall=m_navigator.GetShellImageList(TRUE);
		if(m_hShellImageListSmall==NULL)
			return FALSE;
	}
	VERIFY(ImageList_GetImageInfo(m_hShellImageListSmall,0,&imageInfo));
	rect=imageInfo.rcImage;
	VERIFY(m_imageListSmall.Create(rect.Width(),rect.Height(),ILC_COLOR32|ILC_MASK,0,0));

	SetImageList(&m_imageListSmall,LVSIL_SMALL);
	////////////////////////////////////////////////


	// Clean up ListCtrl
	DeleteAllItems();

	// check the columns
	LVCOLUMN lvc={0};
	lvc.mask=LVCF_FMT;
	int nColumnCount=0;
	while(GetColumn(nColumnCount,&lvc))
		nColumnCount++;
	if(nColumnCount!=4)
	{
		for(int nIndex=0; nIndex<nColumnCount; nIndex++)
			DeleteColumn(0);
		CString sItem;
		VERIFY(sItem.LoadString(IDS_OX_SHELLOBJECTLISTNAME));
		InsertColumn(0,sItem,LVCFMT_LEFT,200);
		VERIFY(sItem.LoadString(IDS_OX_SHELLOBJECTLISTSIZE));
		InsertColumn(1,sItem,LVCFMT_LEFT,70);
		VERIFY(sItem.LoadString(IDS_OX_SHELLOBJECTLISTTYPE));
		InsertColumn(2,sItem,LVCFMT_LEFT,120);
		VERIFY(sItem.LoadString(IDS_OX_SHELLOBJECTLISTMODIFIED));
		InsertColumn(3,sItem,LVCFMT_LEFT,120);
	}

	m_bInitialized=TRUE;

	return TRUE;
}


void COXShellObjectList::SetRedraw(BOOL bRedraw/*=TRUE*/)
{
	ASSERT(m_nRedraw>=0);

	if(!bRedraw)
	{
		if(m_nRedraw==0)
		{
			CWnd::SetRedraw(FALSE);
		}
		m_nRedraw++;
	}
	else
	{
		if(m_nRedraw>=1)
			m_nRedraw--;
		if(m_nRedraw==0)
		{
			CWnd::SetRedraw(TRUE);
		}
	}
}


BOOL COXShellObjectList::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVDISPINFO* plvdi=(NMLVDISPINFO*)pNMHDR;
	ASSERT(plvdi!=NULL);
	// TODO: Add your control notification handler code here
	
	(*pResult)=FALSE;

	// retrieve associated structure
	LPNAMESPACEOBJECT lpNameSpaceObject=(LPNAMESPACEOBJECT)plvdi->item.lParam;
	if(lpNameSpaceObject==NULL)
	{
		(*pResult)=TRUE;
		return TRUE;
	}

	// check if item is allowed to be edited
	if((lpNameSpaceObject->dwFlags&SFGAO_CANRENAME)!=SFGAO_CANRENAME)
	{
		(*pResult)=TRUE;
		return TRUE;
	}

	return FALSE;
}


BOOL COXShellObjectList::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVDISPINFO* plvdi=(NMLVDISPINFO*)pNMHDR;
	ASSERT(plvdi!=NULL);
	// TODO: Add your control notification handler code here
	
	*pResult=TRUE;;

	CString sNewText=plvdi->item.pszText;
		
	CWaitCursor waitCusor;

	// retrieve associated structure
	LPNAMESPACEOBJECT lpNameSpaceObject=(LPNAMESPACEOBJECT)plvdi->item.lParam;
	ASSERT(lpNameSpaceObject!=NULL);

	// rename the item
	LPITEMIDLIST lpNewRelativeIDL=NULL;
	if(sNewText.IsEmpty() || 
		!m_navigator.RenameShellObject(lpNameSpaceObject->lpsfParent,
		lpNameSpaceObject->lpRelativeIDL,&lpNewRelativeIDL,sNewText))  
	{
		*pResult=FALSE;
	}
	else
	{
		ASSERT(lpNewRelativeIDL!=NULL);
		lpNameSpaceObject->lpRelativeIDL=lpNewRelativeIDL;
		*pResult=TRUE;
	}

	return TRUE;
}


BOOL COXShellObjectList::Refresh()
{
	SetRedraw(FALSE);
	PopulateList(m_lpsfCurrentParentFolder,m_lpFullIDLCurrentParentFolder);
	SetRedraw(TRUE);
	return TRUE;
}


#ifdef OXSHELLLIST_WATCHFORDIR

LRESULT COXShellObjectList::OnDirChangeNotify(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	CString str;

	COXFileWatchNotifier fwNotifier;
	fwNotifier=m_fileWatcher.GetFileWatchNotifier(wParam);

	// If the wParam is an invalid ID then the notifier is empty
	if(!fwNotifier.IsEmpty())
	{
		CString sDirPath=fwNotifier.GetPath();   
		ASSERT(!sDirPath.IsEmpty());
		VERIFY(Refresh());
	}
	return 0;
}

#endif	//	OXSHELLLIST_WATCHFORDIR


void COXShellObjectList::SetFilter(LPCTSTR lpszExtentions)
{
	m_arrFilter.RemoveAll();

	CString sExtentions=lpszExtentions;
	if(sExtentions.IsEmpty())
		return;

	for(;;)
	{
		int nDelimiterPos=sExtentions.Find(OXSHELLLIST_EXT_DELIMITER);
		if(nDelimiterPos==-1)
		{
			m_arrFilter.Add(sExtentions);
			break;
		}
		else
		{
			if(nDelimiterPos>0)
				m_arrFilter.Add(sExtentions.Left(nDelimiterPos));
			sExtentions=sExtentions.Mid(nDelimiterPos+1);
		}
	}
}


CString COXShellObjectList::GetFilter() const
{
	TCHAR sDelimiter[2];
	sDelimiter[0]=OXSHELLLIST_EXT_DELIMITER;
	sDelimiter[1]=_T('\0');
	// special case
	if(m_arrFilter.GetSize()==1 && m_arrFilter[0].IsEmpty())
		return sDelimiter;

	CString sExtentions(_T(""));
	for(int nIndex=0; nIndex<m_arrFilter.GetSize(); nIndex++)
	{
		sExtentions=(nIndex==0 ? _T("") : sDelimiter)+m_arrFilter[nIndex];
	}
	return sExtentions;
}


BOOL COXShellObjectList::IsMatchingFilter(LPCTSTR lpszFileName)
{
	ASSERT(lpszFileName!=NULL);

	if(m_arrFilter.GetSize()==0)
		return TRUE;

	CString sFileName=lpszFileName;
	CString sExtention(_T(""));
	int nDelimeterPos=sFileName.Find(_T('.'));
	if(nDelimeterPos!=0)
		sExtention=sFileName.Mid(nDelimeterPos+1);

	BOOL bFoundMatch=FALSE;
	for(int nIndex=0; nIndex<m_arrFilter.GetSize(); nIndex++)
	{
		if(sExtention.CompareNoCase(m_arrFilter[nIndex])==0)
		{
			bFoundMatch=TRUE;
			break;
		}
	}
	return bFoundMatch;
}


void COXShellObjectList::OnContextMenu(CWnd* pWnd, CPoint pos) 
{
	UNREFERENCED_PARAMETER(pWnd);

	// TODO: Add your control notification handler code here
	if(GetEnableContextMenu())
	{
		// On right click we display context menu for any item
		//

		// Get the item that was right clicked in screen coordinates
		CPoint ptMouseCursor=pos;
		ScreenToClient(&ptMouseCursor);

		// Use HitTest function to define the list item by coordinate
		UINT nFlags=NULL;
		int nItem=HitTest(ptMouseCursor,&nFlags);

		if(nItem!=-1 && (nFlags&LVHT_ONITEM)!=0)
		{
			// select the item
			VERIFY(SetItemState(nItem,LVIS_SELECTED,LVIS_SELECTED));
			// Get folder info  associated with item
			LPNAMESPACEOBJECT lpNameSpaceObject=
				(LPNAMESPACEOBJECT)GetItemData(nItem);
			ASSERT(lpNameSpaceObject!=NULL);

			DWORD dwMenuFlags=CMF_EXPLORE |
				((((GetStyle()&LVS_EDITLABELS)==LVS_EDITLABELS) && 
				((lpNameSpaceObject->dwFlags&SFGAO_CANRENAME)==SFGAO_CANRENAME)) ? 
				CMF_CANRENAME : NULL);
			HMENU hMenu=m_navigator.GetObjectContextMenu(lpNameSpaceObject->lpsfParent,
				lpNameSpaceObject->lpRelativeIDL,dwMenuFlags);
			if(hMenu==NULL)
			{
				TRACE(_T("COXShellObjectList::OnContextMenu: GetObjectContextMenu() failed\n"));
				return;
			}

			// Display popup menu using Windows API TrackPopupMenu function because
			// it provides easy way of getting selected menu item. We need to set 
			// screen coordinates of the point where popup menu should be displayed.
			ClientToScreen(&ptMouseCursor);
			BOOL nCmdID=::TrackPopupMenu(hMenu,
				TPM_LEFTALIGN|TPM_RETURNCMD|TPM_RIGHTBUTTON, 
				ptMouseCursor.x,ptMouseCursor.y,0,GetSafeHwnd(),NULL);

			if(nCmdID)
			{
				switch(nCmdID)
				{
				case IDCMD_RENAME:
					{
						EditLabel(nItem);
						break;
					}
				default:
					{
						if(!m_navigator.InvokeCommand(lpNameSpaceObject->lpsfParent,
							lpNameSpaceObject->lpRelativeIDL,nCmdID,dwMenuFlags))  
						{
							TRACE(_T("COXShellObjectList::OnContextMenu: InvokeCommand() failed"));
						}
						else
						{
							switch(nCmdID)
							{
							case IDCMD_DELETE:
								{
									DeleteItem(nItem);
									break;
								}
							case IDCMD_PASTE:
								{
									Refresh();
									break;
								}
							}
						}
						break;
					}
				}
			}
			::DestroyMenu(hMenu);
		}
		else if(m_lpsfCurrentParentFolder!=NULL)
		{
			LPITEMIDLIST lpRelativeIDL=NULL;
			LPSHELLFOLDER pShellFolder=	m_navigator.GetParentShellFolder(
				m_lpFullIDLCurrentParentFolder,&lpRelativeIDL);
			if(pShellFolder==NULL)
			{
				TRACE(_T("COXShellObjectList::OnContextMenu: GetParentShellFolder() failed\n"));
				return;
			}
			ASSERT(lpRelativeIDL!=NULL || m_lpFullIDLCurrentParentFolder==NULL);

			DWORD dwMenuFlags=CMF_EXPLORE;
			HMENU hMenu=m_navigator.GetObjectContextMenu(
				pShellFolder,lpRelativeIDL,dwMenuFlags);
			if(hMenu==NULL)
			{
				TRACE(_T("COXShellObjectList::OnContextMenu: GetObjectContextMenu() failed\n"));
				return;
			}

			CMenu menuPopup;
			VERIFY(menuPopup.Attach(hMenu));

			// Display popup menu using Windows API TrackPopupMenu function because
			// it provides easy way of getting selected menu item. We need to set 
			// screen coordinates of the point where popup menu should be displayed.
			ClientToScreen(&ptMouseCursor);
			BOOL nCmdID=::TrackPopupMenu(menuPopup.m_hMenu,
				TPM_LEFTALIGN|TPM_RETURNCMD|TPM_RIGHTBUTTON, 
				ptMouseCursor.x,ptMouseCursor.y,0,GetSafeHwnd(),NULL);

			if(nCmdID)
			{
				switch(nCmdID)
				{
				case IDCMD_RENAME:
					{
						ASSERT(FALSE);
						break;
					}
				default:
					{
						if(!m_navigator.InvokeCommand(pShellFolder,lpRelativeIDL,
							nCmdID,dwMenuFlags))  
						{
							TRACE(_T("COXShellObjectList::OnContextMenu: InvokeCommand() failed"));
						}
						else
						{
							switch(nCmdID)
							{
							case IDCMD_DELETE:
								{
									ASSERT(FALSE);
									break;
								}
							case IDCMD_PASTE:
								{
									Refresh();
									break;
								}
							}
						}
						break;
					}
				}
			}
			VERIFY(menuPopup.DestroyMenu());
		}
    } 
}


