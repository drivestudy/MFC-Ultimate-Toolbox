// OXShellFolderTree.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXShellFolderTree.h"
#include "OXMainRes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// image index for the desktop item
const int IDI_DESKTOPIMAGE_INDEX=34;
// delimiter for filter
const TCHAR OXSHELLTREE_EXT_DELIMITER=_T('|');


/////////////////////////////////////////////////////////////////////////////
// COXShellFolderTree

COXShellFolderTree::COXShellFolderTree(BOOL bEnableContextMenu/*=TRUE*/,
									   BOOL bOnlyFileSystemFolders/*=TRUE*/,
									   BOOL bNotifyError/*=TRUE*/,
									   BOOL bShowDesktopItem/*=TRUE*/,
									   BOOL bShowFiles/*=FALSE*/,
									   LPCTSTR lpszFilter/*=_T("")*/) :
	m_hShellImageList(NULL),
	m_bEditingItem(FALSE),
	m_nRedraw(0),
	m_hContextMenuItem(NULL),
	m_bNoRestoreContextMenuItem(FALSE)
{
	SetEnableContextMenu(bEnableContextMenu);
	SetOnlyFileSystemFolders(bOnlyFileSystemFolders);
	SetNotifyError(bNotifyError);

	SetShowFiles(bShowFiles);
	SetShowDesktopItem(bShowDesktopItem);

	SetFilter(lpszFilter);
}

COXShellFolderTree::~COXShellFolderTree()
{
}


BEGIN_MESSAGE_MAP(COXShellFolderTree, CTreeCtrl)
	//{{AFX_MSG_MAP(COXShellFolderTree)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CANCELMODE()
	ON_WM_CAPTURECHANGED()
//	ON_NOTIFY_REFLECT_EX(NM_RCLICK, OnRClick)
	ON_NOTIFY_REFLECT_EX(NM_DBLCLK, OnDblClick)
	ON_NOTIFY_REFLECT_EX(TVN_ITEMEXPANDING, OnItemExpanding)
	ON_NOTIFY_REFLECT_EX(TVN_BEGINLABELEDIT, OnBeginLabelEdit)
	ON_NOTIFY_REFLECT_EX(TVN_ENDLABELEDIT, OnEndLabelEdit)
#ifdef OXSHELLTREE_WATCHFORDIR
	ON_MESSAGE(WM_OX_FILE_NOTIFY, OnDirChangeNotify)
#endif	//	OXSHELLTREE_WATCHFORDIR
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXShellFolderTree message handlers

BOOL COXShellFolderTree::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CTreeCtrl::PreCreateWindow(cs);
}

void COXShellFolderTree::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CTreeCtrl::PreSubclassWindow();
}


BOOL COXShellFolderTree::InitializeTree(CString sFolderStartFrom/*=_T("")*/)
{
	ASSERT_VALID(this);

	if(!::IsWindow(GetSafeHwnd()))
		return FALSE;

	SetNotifyError(m_bNotifyError);

	// use system image list as tree control's image list
	//
	int nDesktopItemImageIndex=0;
	if((HIMAGELIST)m_imageList!=NULL)
		m_imageList.DeleteImageList();
	m_mapImageIndex.RemoveAll();
    // retrieve Shell image list
    if(m_hShellImageList==NULL)
	{
		m_hShellImageList=m_navigator.GetShellImageList();
		if(m_hShellImageList==NULL)
			return FALSE;
	}

	IMAGEINFO imageInfo;
	VERIFY(ImageList_GetImageInfo(m_hShellImageList,0,&imageInfo));

	CRect rect(imageInfo.rcImage);
	VERIFY(m_imageList.Create(rect.Width(),rect.Height(),ILC_COLOR32|ILC_MASK,0,0));
	HICON hIcon=::ExtractIcon(
		AfxGetInstanceHandle(),_T("shell32.dll"),IDI_DESKTOPIMAGE_INDEX);
	ASSERT(hIcon!=NULL);
	nDesktopItemImageIndex=m_imageList.Add(hIcon);
	ASSERT(nDesktopItemImageIndex!=-1);
	VERIFY(::DestroyIcon(hIcon));


	SetImageList(&m_imageList,TVSIL_NORMAL);


	// now let's fill TreeCtrl with shell namespace objects that is located 
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

	// unselect item
	SelectItem(NULL);

	// Clean up TreeCtrl
	DeleteAllItems();

	HTREEITEM htiRoot=TVI_ROOT;
	if(m_bShowDesktopItem)
	{
		// structures for inserting new items in our TreeCtrl
		TV_INSERTSTRUCT tvins;

		tvins.item.mask=TVIF_TEXT|TVIF_STATE|TVIF_IMAGE|TVIF_SELECTEDIMAGE|
			TVIF_PARAM|TVIF_CHILDREN;
		// NAMESPACEOBJECT object is not defined for Desktop item
		tvins.item.lParam=(LPARAM)NULL;
		// Desktop item always has subfolders
		tvins.item.cChildren=1;
		// display name
		CString sItem;
		VERIFY(sItem.LoadString(IDS_OX_SHELLFOLDERTREEDESKTOP));//"Desktop"
		tvins.item.pszText=(LPTSTR) (LPCTSTR) sItem;

		// Request icon index for normal state
		tvins.item.iImage=nDesktopItemImageIndex;
		tvins.item.iSelectedImage=nDesktopItemImageIndex;
		// expanded from the beginning
	    tvins.item.state=TVIS_EXPANDED|TVIS_EXPANDEDONCE;
		tvins.item.stateMask=tvins.item.state;

		// We collected all information we needed. It's time to insert 
		// new item in our TreeCtrl.
		tvins.hInsertAfter=TVI_LAST;
		tvins.hParent=TVI_ROOT;
		htiRoot=InsertItem(&tvins);
		ASSERT(htiRoot!=NULL);
	}

	// Fill TreeCtrl with objects in lpFolder folder.
	FillTreeWithSubfolders(htiRoot,lpFolder,lpidlFull);

	lpFolder->Release();

	// We cannot sort item using just their display names. We have to 
	// request shell to sort items.
	SortChildren(TVI_ROOT);

	return TRUE;
}


BOOL COXShellFolderTree::OpenFolder(CString sFolderToOpen, 
									BOOL bExpand/*=FALSE*/)
{
	if(sFolderToOpen.IsEmpty())
		return TRUE;

	LPITEMIDLIST lpidlFull;
	if(!m_navigator.GetShellFolderFullIDL(sFolderToOpen,&lpidlFull))
		return FALSE;
	return OpenFolder(lpidlFull,bExpand);
}


BOOL COXShellFolderTree::OpenFolder(const LPITEMIDLIST lpFullIDL, 
									BOOL bExpand/*=FALSE*/)
{
	if(lpFullIDL==NULL)
		return FALSE;

	HTREEITEM hItem=GetRootItem();
	ASSERT(hItem!=NULL);
	if(m_bShowDesktopItem)
	{
		hItem=GetNextItem(hItem,TVGN_CHILD);
		ASSERT(hItem!=NULL);
	}

	LPITEMIDLIST pidlCopy=lpFullIDL;
	LPITEMIDLIST pidlNext=NULL;
	BOOL bFound=FALSE;
	while(pidlCopy!=NULL)
	{
		pidlNext=m_navigator.GetNextIDLItem(pidlCopy);

		bFound=FALSE;
		while(hItem!=NULL)
		{
			// Get folder info  associated with item
			LPNAMESPACEOBJECT lpNameSpaceObject=
				(LPNAMESPACEOBJECT)GetItemData(hItem);
			ASSERT(lpNameSpaceObject!=NULL);
			if(pidlCopy->mkid.cb==lpNameSpaceObject->
				lpRelativeIDL->mkid.cb &&
				memcmp((void*)pidlCopy,(void*)lpNameSpaceObject->
				lpRelativeIDL,pidlCopy->mkid.cb)==0)
			{
				bFound=TRUE;
				if(pidlNext!=NULL)
				{
					Expand(hItem,TVE_EXPAND);
					hItem=GetNextItem(hItem,TVGN_CHILD);
				}
				else
				{
					SelectItem(hItem);
					if(bExpand)
						Expand(hItem,TVE_EXPAND);
					EnsureVisible(hItem);
				}
				break;
			}
			else
				hItem=GetNextItem(hItem,TVGN_NEXT);
		}

		if(!bFound)
			break;

		pidlCopy=pidlNext;
	}

	return bFound;
}

	
HTREEITEM COXShellFolderTree::FindFolder(CString sFolderToFind)
{
	if(sFolderToFind.IsEmpty())
		return NULL;

	LPITEMIDLIST lpidlFull;
	if(!m_navigator.GetShellFolderFullIDL(sFolderToFind,&lpidlFull))
		return NULL;
	return FindFolder(lpidlFull);
}


HTREEITEM COXShellFolderTree::FindFolder(const LPITEMIDLIST lpFullIDL)
{
	if(lpFullIDL==NULL)
		return NULL;

	HTREEITEM hItem=GetRootItem();
	ASSERT(hItem!=NULL);
	if(m_bShowDesktopItem)
	{
		hItem=GetNextItem(hItem,TVGN_CHILD);
		ASSERT(hItem!=NULL);
	}

	LPITEMIDLIST pidlCopy=lpFullIDL;
	LPITEMIDLIST pidlNext=NULL;
	HTREEITEM hFoundItem=NULL;
	while(pidlCopy!=NULL)
	{
		pidlNext=m_navigator.GetNextIDLItem(pidlCopy);

		hFoundItem=NULL;
		while(hItem!=NULL )
		{
			// Get folder info  associated with item
			LPNAMESPACEOBJECT lpNameSpaceObject=
				(LPNAMESPACEOBJECT)GetItemData(hItem);
			ASSERT(lpNameSpaceObject!=NULL);
			if(pidlCopy->mkid.cb==lpNameSpaceObject->
				lpRelativeIDL->mkid.cb &&
				memcmp((void*)pidlCopy,(void*)lpNameSpaceObject->
				lpRelativeIDL,pidlCopy->mkid.cb)==0)
			{
				hFoundItem=hItem;
				if(pidlNext!=NULL)
				{
					if(GetItemState(hItem,TVIS_EXPANDED)&TVIS_EXPANDED)
						hItem=GetNextItem(hItem,TVGN_CHILD);
					else
						hFoundItem=NULL;
				}
				break;
			}
			else
				hItem=GetNextItem(hItem,TVGN_NEXT);
		}

		if(hFoundItem==NULL)
			break;

		pidlCopy=pidlNext;
	}

	return hFoundItem;
}

	
CString COXShellFolderTree::GetFullPath(HTREEITEM hItem) const
{
	ASSERT(hItem!=NULL);

	// retrieve associated structure
	LPNAMESPACEOBJECT lpNameSpaceObject=(LPNAMESPACEOBJECT)GetItemData(hItem);
	if(lpNameSpaceObject==NULL)
		return _T("");

	return m_navigator.GetFullPath(lpNameSpaceObject->lpFullIDL);
}


BOOL COXShellFolderTree::FillTreeWithSubfolders(HTREEITEM htiParent,
												const LPSHELLFOLDER lpFolder, 
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

	// structures for inserting new items in our TreeCtrl
	TV_ITEM tvi;
	TV_INSERTSTRUCT tvins;

	BOOL bLast;
	// Loop through folders and objects that have subfolders
	LPNAMESPACEOBJECT lpNameSpaceObject=
		m_navigator.EnumerateObjectNext(bLast);
	while(!bLast)
	{
		BOOL bQualified=TRUE;
		if(GetOnlyFileSystemFolders())
		{
			if(GetShowFiles())
				bQualified=lpNameSpaceObject!=NULL && 
					(lpNameSpaceObject->dwFlags&SFGAO_FILESYSANCESTOR ||
					lpNameSpaceObject->dwFlags&SFGAO_FILESYSTEM);
			else
				bQualified=lpNameSpaceObject!=NULL && 
					((lpNameSpaceObject->dwFlags&SFGAO_FILESYSANCESTOR && 
					lpNameSpaceObject->dwFlags&SFGAO_HASSUBFOLDER) ||
					(lpNameSpaceObject->dwFlags&SFGAO_FILESYSTEM &&
					lpNameSpaceObject->dwFlags&SFGAO_FOLDER));
		}
		else
		{
			if(GetShowFiles())
				bQualified=lpNameSpaceObject!=NULL;
			else
				bQualified=lpNameSpaceObject!=NULL && 
					(lpNameSpaceObject->dwFlags&SFGAO_HASSUBFOLDER ||
					lpNameSpaceObject->dwFlags&SFGAO_FOLDER);
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
			tvi.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM;
			// NAMESPACEOBJECT structure is already filled
			tvi.lParam=(LPARAM)lpNameSpaceObject;

			// if object has subfolders then we define the corresponding
			// tree item as one that has children
			if(lpNameSpaceObject->dwFlags&SFGAO_FILESYSANCESTOR || 
				lpNameSpaceObject->dwFlags&SFGAO_HASSUBFOLDER ||
				(GetShowFiles() && lpNameSpaceObject->dwFlags&SFGAO_FOLDER))
			{
				tvi.cChildren=1;
				tvi.mask|=TVIF_CHILDREN;
			}
                  
			// display name
			tvi.pszText=lpNameSpaceObject->szDisplayName;

			// images
			int nTreeImageIndex=-1;
			if(!m_mapImageIndex.Lookup(lpNameSpaceObject->nImageSmall,nTreeImageIndex))
			{
				nTreeImageIndex=PtrToInt(m_mapImageIndex.GetCount())+1;
				m_mapImageIndex.SetAt(lpNameSpaceObject->nImageSmall,nTreeImageIndex);
				HICON hIcon=ImageList_ExtractIcon(
					0,m_hShellImageList,lpNameSpaceObject->nImageSmall);
				ASSERT(hIcon!=NULL);
				tvi.iImage=m_imageList.Add(hIcon);
				ASSERT(tvi.iImage!=-1 && tvi.iImage==nTreeImageIndex);
				VERIFY(::DestroyIcon(hIcon));
			}
			else
			{
				tvi.iImage=nTreeImageIndex;
			}

			nTreeImageIndex=-1;
			if(!m_mapImageIndex.Lookup(lpNameSpaceObject->nImageSelectedSmall,
				nTreeImageIndex))
			{
				nTreeImageIndex=PtrToInt(m_mapImageIndex.GetCount())+1;
				m_mapImageIndex.SetAt(lpNameSpaceObject->nImageSelectedSmall,
					nTreeImageIndex);
				HICON hIcon=ImageList_ExtractIcon(
					0,m_hShellImageList,lpNameSpaceObject->nImageSelectedSmall);
				ASSERT(hIcon!=NULL);
				tvi.iSelectedImage=m_imageList.Add(hIcon);
				ASSERT(tvi.iSelectedImage!=-1 && tvi.iSelectedImage==nTreeImageIndex);
				VERIFY(::DestroyIcon(hIcon));
			}
			else
			{
				tvi.iSelectedImage=nTreeImageIndex;
			}
			///////////////////////////

			// We collected all information we needed. It's time to insert 
			// new item in our TreeCtrl.
			tvins.item=tvi;
			tvins.hInsertAfter=TVI_LAST;
			tvins.hParent=htiParent;
			VERIFY(InsertItem(&tvins)!=NULL);
		}
		lpNameSpaceObject=m_navigator.EnumerateObjectNext(bLast);
	}

	// Release enumerator 
	m_navigator.ReleaseObjectsEnumerator();

	return TRUE;
}


BOOL COXShellFolderTree::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;

	ASSERT(pNMTreeView->itemNew.hItem!=NULL);

	// If folder at least once was expanded then it was already populated 
	if((pNMTreeView->itemNew.state & TVIS_EXPANDEDONCE))
		return TRUE;
		
	// retrieve associated structure
	LPNAMESPACEOBJECT lpNameSpaceObject=
		(LPNAMESPACEOBJECT)pNMTreeView->itemNew.lParam;
	ASSERT(lpNameSpaceObject!=NULL || GetShowDesktopItem());


	CWaitCursor waitCusor;

	// get shell folder object from saved PIDLs
	LPSHELLFOLDER lpsfExpanded=m_navigator.
		GetShellFolder(lpNameSpaceObject->lpsfParent,
		lpNameSpaceObject->lpRelativeIDL);
	if(lpsfExpanded!=NULL)
	{
		// add new items
		FillTreeWithSubfolders(pNMTreeView->itemNew.hItem,lpsfExpanded,
			lpNameSpaceObject->lpFullIDL);
		SortChildren(pNMTreeView->itemNew.hItem);
	}


#ifdef OXSHELLTREE_WATCHFORDIR
	if(lpNameSpaceObject!=NULL && 
		((lpNameSpaceObject->dwFlags&SFGAO_FILESYSANCESTOR)==SFGAO_FILESYSANCESTOR)
		||(lpNameSpaceObject->dwFlags&SFGAO_FILESYSTEM)==SFGAO_FILESYSTEM)
	{
		CString sCurrentFolder=GetFullPath(pNMTreeView->itemNew.hItem);
		if(!sCurrentFolder.IsEmpty())
		{
			if(m_fileWatcher.AddWatch(sCurrentFolder,/*FALSE*/TRUE,
				COXFileWatcher::OXFileWatchChangeDirName|(GetShowFiles() ? 
				COXFileWatcher::OXFileWatchChangeFileName : NULL)))
			{
				m_fileWatcher.EnableWindowNotification(sCurrentFolder,this,TRUE);
			}
			else
			{
				TRACE(_T("COXShellFolderTree::OnItemExpanding: failed to set a file watcher for the expanding folder\n"));
			}
		}
		else
		{
			HTREEITEM hChildItem=this->GetNextItem(
				pNMTreeView->itemNew.hItem,TVGN_CHILD);
			while (hChildItem)
			{
				
				CString sFolder=GetFullPath(hChildItem);
				if (!sFolder.IsEmpty())
				{
					if(m_fileWatcher.AddWatch(sFolder,/*FALSE*/TRUE,
						COXFileWatcher::OXFileWatchChangeDirName|(GetShowFiles() ? 
						COXFileWatcher::OXFileWatchChangeFileName : NULL)))
					{
						m_fileWatcher.EnableWindowNotification(sFolder,this,TRUE);
					}
					else
					{
						TRACE(_T("COXShellFolderTree::OnItemExpanding: failed to set a file watcher for the expanding folder\n"));
					}

				}
				hChildItem=this->GetNextItem(
					hChildItem,TVGN_NEXT);
			}

		}
	}
#endif	//	OXSHELLTREE_WATCHFORDIR


	return TRUE;
}


BOOL COXShellFolderTree::OnRClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);

	// TODO: Add your control notification handler code here
	*pResult = 0;

	if(GetEnableContextMenu())
	{
		// On right click we display context menu for any item
		//

		// Get the item that was right clicked in screen coordinates
		CPoint ptMouseCursor;
		if(!::GetCursorPos(&ptMouseCursor))
		{
			TRACE(_T("COXShellFolderTree::OnRClick: GetCursorPos() failed\n"));
			return FALSE;
		}
		ScreenToClient(&ptMouseCursor);

		// Use HitTest function to define the tree item by coordinate
		TV_HITTESTINFO tvhti;
		tvhti.pt=ptMouseCursor;
		HitTest(&tvhti);

		if(tvhti.hItem)
		{
			// Get folder info  associated with item
			LPNAMESPACEOBJECT lpNameSpaceObject=
				(LPNAMESPACEOBJECT)GetItemData(tvhti.hItem);
			if(m_bShowDesktopItem && lpNameSpaceObject==NULL)
			{
				return FALSE;
			}
			ASSERT(lpNameSpaceObject!=NULL);

			DWORD dwMenuFlags=CMF_EXPLORE |
				((((GetStyle()&TVS_EDITLABELS)==TVS_EDITLABELS) && 
				((lpNameSpaceObject->dwFlags&SFGAO_CANRENAME)==SFGAO_CANRENAME)) ? 
				CMF_CANRENAME : NULL);
			HMENU hMenu=
				m_navigator.GetObjectContextMenu(lpNameSpaceObject->lpsfParent,
				lpNameSpaceObject->lpRelativeIDL,dwMenuFlags);
			if(hMenu==NULL)
			{
				TRACE(_T("COXShellFolderTree::OnRClick: GetObjectContextMenu() failed\n"));
				return FALSE;
			}

			// Display popup menu using Windows API TrackPopupMenu function because
			// it provides easy way of getting selected menu item. We need to set 
			// screen coordinates of the point where popup menu should be displayed.
			ClientToScreen(&ptMouseCursor);
			BOOL nCmdID=::TrackPopupMenu(hMenu,
				TPM_LEFTALIGN|TPM_RETURNCMD|TPM_RIGHTBUTTON, 
				ptMouseCursor.x,ptMouseCursor.y,0,GetSafeHwnd(),NULL);

			if(nCmdID!=0)
			{
				switch(nCmdID)
				{
				case IDCMD_RENAME:
					{
						EditLabel(tvhti.hItem);
						break;
					}
				default:
					{
						if(!m_navigator.InvokeCommand(lpNameSpaceObject->lpsfParent,
							lpNameSpaceObject->lpRelativeIDL,nCmdID,dwMenuFlags))  
						{
							TRACE(_T("COXShellFolderTree::OnRClick: InvokeCommand() failed\n"));
						}
						else
						{
							switch(nCmdID)
							{
							case IDCMD_DELETE:
								{
									DeleteItem(tvhti.hItem);
									break;
								}
							case IDCMD_PASTE:
								{
									Refresh(tvhti.hItem);
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
		return TRUE;
    } 

	return FALSE;
}


void COXShellFolderTree::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(GetEnableContextMenu())
	{
		ASSERT(m_hContextMenuItem==NULL);

		TV_HITTESTINFO tvhti;
		tvhti.pt=point;
		HitTest(&tvhti);

		if(tvhti.hItem!=NULL)
		{
			m_hContextMenuItem=tvhti.hItem;

			// remove highlight from the currently selected item
			HTREEITEM hSelectedItem=GetSelectedItem();
			if(hSelectedItem!=m_hContextMenuItem)
			{
				if(hSelectedItem!=NULL)
				{
					SetItemState(hSelectedItem,0,TVIS_SELECTED);
				}

				// highlight the item under mouse cursor
				SetItemState(m_hContextMenuItem,TVIS_DROPHILITED,TVIS_DROPHILITED);
			}
		}

		::SetCapture(GetSafeHwnd());
	}
	else
	{
		CTreeCtrl::OnRButtonDown(nFlags,point);
	}
}


void COXShellFolderTree::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if(GetEnableContextMenu())
	{
		// On right click we display context menu for any item
		//

		// set handle to context menu item to NULL in order to avoid triggering
		// preliminary restore routine in OnCancelMode() and OnCaptureChanged()
		m_bNoRestoreContextMenuItem=TRUE;

		if(m_hContextMenuItem!=NULL)
		{
			ASSERT(::GetCapture()==GetSafeHwnd());

			// Get the item that was right clicked in screen coordinates
			CPoint ptMouseCursor=point;

#ifdef _DEBUG
			// double check for context menu item
			TV_HITTESTINFO tvhti;
			tvhti.pt=ptMouseCursor;
			HitTest(&tvhti);
			VERIFY(tvhti.hItem==m_hContextMenuItem);
#endif

			// Get folder info  associated with item
			LPNAMESPACEOBJECT lpNameSpaceObject=
				(LPNAMESPACEOBJECT)GetItemData(m_hContextMenuItem);
			if(!m_bShowDesktopItem || lpNameSpaceObject!=NULL)
			{
				ASSERT(lpNameSpaceObject!=NULL);

				DWORD dwMenuFlags=CMF_EXPLORE | 
					((((GetStyle()&TVS_EDITLABELS)==TVS_EDITLABELS) && 
					((lpNameSpaceObject->dwFlags&SFGAO_CANRENAME)==SFGAO_CANRENAME)) ? 
					CMF_CANRENAME : 0);
				HMENU hMenu=m_navigator.GetObjectContextMenu(
					lpNameSpaceObject->lpsfParent,
					lpNameSpaceObject->lpRelativeIDL,dwMenuFlags);
				if(hMenu!=NULL)
				{
					// Display popup menu using Windows API TrackPopupMenu function 
					// because it provides an e+asy way of getting selected menu item. 
					// We need to set screen coordinates of the point where popup menu 
					// should be displayed.
					ClientToScreen(&ptMouseCursor);
					BOOL nCmdID=::TrackPopupMenu(
						hMenu,TPM_LEFTALIGN|TPM_RETURNCMD|TPM_RIGHTBUTTON, 
						ptMouseCursor.x,ptMouseCursor.y,0,GetSafeHwnd(),NULL);

					if(nCmdID!=0)
					{
						switch(nCmdID)
						{
						case IDCMD_RENAME:
							{
								EditLabel(m_hContextMenuItem);
								break;
							}
						default:
							{
								if(!m_navigator.InvokeCommand(
									lpNameSpaceObject->lpsfParent,
									lpNameSpaceObject->lpRelativeIDL,
									nCmdID,dwMenuFlags))  
								{
									TRACE(_T("COXShellFolderTree::OnContextMenu: InvokeCommand() failed\n"));
								}
								else
								{
									switch(nCmdID)
									{
									case IDCMD_DELETE:
										{
											DeleteItem(m_hContextMenuItem);
											break;
										}
									case IDCMD_PASTE:
										{
											Refresh(m_hContextMenuItem);
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
				else
				{
					TRACE(_T("COXShellFolderTree::OnContextMenu: GetObjectContextMenu() failed\n"));
				}
			}
		}

		m_bNoRestoreContextMenuItem=FALSE;
		if(::GetCapture()==GetSafeHwnd())
		{
			::ReleaseCapture();
		}
		else
		{
			RestoreStateAfterContextMenu();
		}
		ASSERT(m_hContextMenuItem==NULL);
    } 
	else
	{
		CTreeCtrl::OnRButtonUp(nFlags,point);
	}
}


void COXShellFolderTree::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(GetEnableContextMenu() && (nFlags & MK_RBUTTON)!=0)
	{
		TV_HITTESTINFO tvhti;
		tvhti.pt=point;
		HitTest(&tvhti);

		if(tvhti.hItem!=m_hContextMenuItem)
		{
			HTREEITEM hSelectedItem=GetSelectedItem();

			if(m_hContextMenuItem!=NULL)
			{
				if(m_hContextMenuItem!=hSelectedItem)
				{
					SetItemState(m_hContextMenuItem,0,TVIS_DROPHILITED);
				}
				else
				{
					SetItemState(m_hContextMenuItem,0,TVIS_SELECTED);
				}
			}

			m_hContextMenuItem=tvhti.hItem;

			if(m_hContextMenuItem!=NULL)
			{
				if(m_hContextMenuItem!=hSelectedItem)
				{
					SetItemState(m_hContextMenuItem,TVIS_DROPHILITED,TVIS_DROPHILITED);
				}
				else
				{
					SetItemState(m_hContextMenuItem,TVIS_SELECTED,TVIS_SELECTED);
				}
			}
		}
	}

	CTreeCtrl::OnMouseMove(nFlags,point);
}


void COXShellFolderTree::OnCancelMode() 
{
	CTreeCtrl::OnCancelMode();

	RestoreStateAfterContextMenu();
}


void COXShellFolderTree::OnCaptureChanged(CWnd* pWnd) 
{
	CTreeCtrl::OnCaptureChanged(pWnd);
	
	RestoreStateAfterContextMenu();
}


BOOL COXShellFolderTree::OnDblClick(NMHDR* pNMHDR, LRESULT* pResult) 
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
		TRACE(_T("COXShellFolderTree::OnDblClick: GetCursorPos() failed\n"));
		return FALSE;
	}
	ScreenToClient(&ptMouseCursor);

	// Use HitTest function to define the tree item by coordinate
	TV_HITTESTINFO tvhti;
	tvhti.pt=ptMouseCursor;
	HitTest(&tvhti);

	if(tvhti.hItem)
	{
		// Get folder info  associated with item
		LPNAMESPACEOBJECT lpNameSpaceObject=
			(LPNAMESPACEOBJECT)GetItemData(tvhti.hItem);
		if(m_bShowDesktopItem && lpNameSpaceObject==NULL)
		{
			return FALSE;
		}
		ASSERT(lpNameSpaceObject!=NULL);

		if((lpNameSpaceObject->dwFlags&SFGAO_FOLDER)!=SFGAO_FOLDER) 
		{
			if(!m_navigator.InvokeDefaultCommand(lpNameSpaceObject->lpsfParent,
				lpNameSpaceObject->lpRelativeIDL))
			{
				TRACE(_T("COXShellFolderTree::OnDblClick: InvokeDefaultCommand() failed"));
			}
		}
	}

	return FALSE;
}


void COXShellFolderTree::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!m_bEditingItem)
	{
		CTreeCtrl::OnLButtonDown(nFlags,point);
	}
	else
	{
		SetFocus();
	}
}


BOOL COXShellFolderTree::SortChildren(const HTREEITEM htiParent)
{
    TV_SORTCB tvscb;
    tvscb.hParent=htiParent;
    tvscb.lParam=0;
    tvscb.lpfnCompare=COXShellNamespaceNavigator::CompareObjectsProc;

	return CTreeCtrl::SortChildrenCB(&tvscb);
}


void COXShellFolderTree::SetRedraw(BOOL bRedraw/*=TRUE*/)
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


BOOL COXShellFolderTree::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMTVDISPINFO ptvdi=(LPNMTVDISPINFO)pNMHDR;
	ASSERT(ptvdi!=NULL);
	// TODO: Add your control notification handler code here
	
	(*pResult)=FALSE;

	// retrieve associated structure
	LPNAMESPACEOBJECT lpNameSpaceObject=(LPNAMESPACEOBJECT)ptvdi->item.lParam;
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

	m_bEditingItem=TRUE;

	return FALSE;
}


BOOL COXShellFolderTree::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMTVDISPINFO ptvdi=(LPNMTVDISPINFO)pNMHDR;
	ASSERT(ptvdi!=NULL);
	// TODO: Add your control notification handler code here
	
	*pResult=TRUE;;

	CString sNewText=ptvdi->item.pszText;
		
	CWaitCursor waitCusor;

	// retrieve associated structure
	LPNAMESPACEOBJECT lpNameSpaceObject=(LPNAMESPACEOBJECT)ptvdi->item.lParam;
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
		m_navigator.FreeShellObject((void*)lpNameSpaceObject->lpFullIDL);

		HTREEITEM hItem=GetParentItem(ptvdi->item.hItem);
		if(hItem==NULL || GetItemData(hItem)==NULL)
		{
			lpNameSpaceObject->lpFullIDL=m_navigator.
				CopyPIDL(lpNameSpaceObject->lpRelativeIDL);
		}
		else
		{
			LPNAMESPACEOBJECT lpNameSpaceObjectParent=
				(LPNAMESPACEOBJECT)GetItemData(hItem);
			ASSERT(lpNameSpaceObjectParent!=NULL);
			lpNameSpaceObject->lpFullIDL=m_navigator.
				ConcatenatePIDLs(lpNameSpaceObjectParent->lpFullIDL,
				lpNameSpaceObject->lpRelativeIDL);
			ASSERT(lpNameSpaceObject->lpFullIDL!=NULL);
		}

		*pResult=TRUE;
	}

	m_bEditingItem=FALSE;

	return TRUE;
}


BOOL COXShellFolderTree::Refresh(LPCTSTR lpszFolder)
{
	HTREEITEM hItem=FindFolder(lpszFolder);
	if(hItem==NULL)
	{
		return FALSE;
	}

	return Refresh(hItem);
}


BOOL COXShellFolderTree::Refresh(HTREEITEM htiParent/*=TVI_ROOT*/)
{
	if(htiParent==NULL)
	{
		return FALSE;
	}

	// save currently selected folder
	CString sCurrentFolder(_T(""));
	HTREEITEM hItem=GetSelectedItem();
	if(hItem!=NULL)
	{
		sCurrentFolder=GetFullPath(hItem);
		SelectItem(NULL);
	}
	//////////////////

	if(htiParent!=TVI_ROOT && 
		(GetItemState(htiParent,TVIS_EXPANDED)&TVIS_EXPANDED)!=TVIS_EXPANDED)
	{
		return (SetItemState(htiParent,NULL,TVIS_EXPANDEDONCE)!=0);
	}

	if(htiParent==TVI_ROOT)
	{
		hItem=GetRootItem();
		ASSERT(hItem!=NULL);
		if(m_bShowDesktopItem && GetItemData(hItem)==NULL)
		{
			hItem=GetNextItem(hItem,TVGN_CHILD);
		}
	}
	else
	{
		hItem=GetNextItem(htiParent,TVGN_CHILD);
	}
	if(hItem==NULL)
	{
		return TRUE;
	}

	// save info about curently expanded folders
	CArray<CString,CString> m_arrExpandedFolders;
	while(hItem!=NULL)
	{
		// Get full path for the item
		if(GetItemState(hItem,TVIS_EXPANDED)&TVIS_EXPANDED)
		{
			CString sPath=GetFullPath(hItem);
			if(!sPath.IsEmpty())
				m_arrExpandedFolders.Add(sPath);
		}

		hItem=GetNextItem(hItem,TVGN_NEXTVISIBLE);
	}
	///////////////////////


	SetRedraw(FALSE);

	if(htiParent==TVI_ROOT)
	{
		InitializeTree();
	}
	else
	{
		BOOL bWasExpanded=(GetItemState(htiParent,TVIS_EXPANDED)!=0);
		// repopulate the branch
		for(;;)
		{
			HTREEITEM htiChild=GetNextItem(htiParent,TVGN_CHILD);
			if(htiChild==NULL)
				break;
			DeleteItem(htiChild);
		}
		SetItemState(htiParent,NULL,TVIS_EXPANDEDONCE);
		if(bWasExpanded)
		{
			Expand(htiParent,TVE_COLLAPSE);
			Expand(htiParent,TVE_EXPAND);
		}
	}

	// expand the previously expanded folders
	for(int nIndex=0; nIndex<m_arrExpandedFolders.GetSize(); nIndex++)
	{
		CString sPath=m_arrExpandedFolders[nIndex];
		ASSERT(!sPath.IsEmpty());
		OpenFolder(sPath,TRUE);
	}

	// select current folder
	if(!sCurrentFolder.IsEmpty())
		OpenFolder(sCurrentFolder);

	SetRedraw(TRUE);

	return TRUE;
}


#ifdef OXSHELLTREE_WATCHFORDIR

LRESULT COXShellFolderTree::OnDirChangeNotify(WPARAM wParam, LPARAM lParam)
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
		if(!Refresh(sDirPath))
		{
			TRACE(_T("COXShellFolderTree::OnDirChangeNotify: failed to refresh folder '%s'"),sDirPath);
		}
	}
	return 0;
}

#endif	//	OXSHELLTREE_WATCHFORDIR


void COXShellFolderTree::SetFilter(LPCTSTR lpszExtentions)
{
	m_arrFilter.RemoveAll();

	CString sExtentions=lpszExtentions;
	if(sExtentions.IsEmpty())
		return;

	for(;;)
	{
		int nDelimiterPos=sExtentions.Find(OXSHELLTREE_EXT_DELIMITER);
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


CString COXShellFolderTree::GetFilter() const
{
	TCHAR sDelimiter[2];
	sDelimiter[0]=OXSHELLTREE_EXT_DELIMITER;
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


BOOL COXShellFolderTree::IsMatchingFilter(LPCTSTR lpszFileName)
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


void COXShellFolderTree::RestoreStateAfterContextMenu()
{
	if(!m_bNoRestoreContextMenuItem)
	{
		HTREEITEM hSelectedItem=GetSelectedItem();

		// restore selection to the focused item
		if(hSelectedItem!=m_hContextMenuItem)
		{
			if(hSelectedItem!=NULL)
			{
				SetItemState(hSelectedItem,TVIS_SELECTED,TVIS_SELECTED);
			}

			if(m_hContextMenuItem!=NULL)
			{
				// remove highlight from the item under mouse cursor
				SetItemState(m_hContextMenuItem,0,TVIS_DROPHILITED);
			}
		}

		m_hContextMenuItem=NULL;
	}
}
