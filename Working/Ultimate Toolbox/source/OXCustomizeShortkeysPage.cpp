// ==========================================================================
//			Class Implementation: COXCustomizeShortkeysPage
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "OXCustomizeShortkeysPage.h"
#include "OXCustomizeManager.h"

#include "OXBitmapMenuOrganizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int ID_OX_IMAGEOFFSET_COUNT=2;
const int ID_OX_IMAGEOFFSET_MENUGROUP=0;
const int ID_OX_IMAGEOFFSET_NOIMAGE=1;

#include "UTB64Bit.h"

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeShortkeysPage dialog

IMPLEMENT_DYNCREATE(COXCustomizeShortkeysPage, COXCustomizePage)

COXCustomizeShortkeysPage::COXCustomizeShortkeysPage()
{
	//{{AFX_DATA_INIT(COXCustomizeShortkeysPage)
	m_sAssignedTo = _T("");
	m_sCommandDescription = _T("");
	m_nAccelTableIndex = -1;
	m_nShortkeyIndex = -1;
	//}}AFX_DATA_INIT
	m_nDialogID=IDD;
	m_sProfileName.Empty();
	m_bReadOnly=FALSE;
}


COXCustomizeShortkeysPage::~COXCustomizeShortkeysPage()
{
}


BOOL COXCustomizeShortkeysPage::OnCloseManager(BOOL bIsOk) 
{	
	CWinApp* pApp=AfxGetApp();
	ASSERT(pApp!=NULL);

	if(bIsOk)
	{
		if(!ApplyChanges())
			return FALSE;
		m_shortkeysOrganizer.SaveState(m_sProfileName);
	}
	else
	{
		if(!m_sProfileName.IsEmpty())
		{
			m_shortkeysOrganizer.LoadState(m_sProfileName);
		}
	}

	return TRUE;
}


BOOL COXCustomizeShortkeysPage::
InitializeShortkeys(CFrameWnd* pFrameWnd, 
					LPCTSTR lpszProfileName/*=_T("CustomizeShortkeys")*/)
{
	ASSERT(pFrameWnd!=NULL);

	CWinApp* pApp=AfxGetApp();
	ASSERT(pApp!=NULL);

	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL);
	CFrameWnd* pMainFrameWnd=DYNAMIC_DOWNCAST(CFrameWnd,pWnd);
	ASSERT(pMainFrameWnd!=NULL);

	// build array of all commands available in the system
	//
	// document menus
	if(m_pCustomizeManager->IsMDIFramework())
	{
		POSITION pos=pApp->GetFirstDocTemplatePosition();
		while(pos!=NULL)
		{
			CDocTemplate* pDocTemplate=pApp->GetNextDocTemplate(pos);
			ASSERT(pDocTemplate!=NULL);
			CMultiDocTemplate* pMultiDocTemplate=
				DYNAMIC_DOWNCAST(CMultiDocTemplate,pDocTemplate);
			if(pMultiDocTemplate!=NULL)
			{
				if(pMultiDocTemplate->m_hMenuShared!=NULL)
					RetrieveCommandsFromMenu(pMultiDocTemplate->m_hMenuShared);
			}
		}
	}

	// default menu
	HINSTANCE hInstance=AfxFindResourceHandle(MAKEINTRESOURCE(IDR_MAINFRAME),RT_MENU);
	if(hInstance!=NULL)
	{
		HMENU hMenu=::LoadMenu(hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));
		if(hMenu!=NULL)
		{
			RetrieveCommandsFromMenu(hMenu);
			::DestroyMenu(hMenu);
		}
	}

	// toolbars
	//
	// Iterate all the control bars and use only the toolbars
	POSITION pos=pFrameWnd->m_listControlBars.GetHeadPosition();
	while(pos!=NULL)
	{
		CToolBar* pToolbar=DYNAMIC_DOWNCAST(CToolBar, 
			(CControlBar*)pFrameWnd->m_listControlBars.GetNext(pos));
		//If it is a CToolBar
		if(pToolbar!=NULL)
		{
			// Get the original imagelist of the toolbar
			int nButtonCount=pToolbar->GetToolBarCtrl().GetButtonCount();
			// Iterate all buttons on the toolbar
			for(int nButtonIndex=0; nButtonIndex<nButtonCount; nButtonIndex++)
			{
				UINT nID=0;
				UINT nStyle=0;
				int nImageIndex=0;
				pToolbar->GetButtonInfo(nButtonIndex,nID,nStyle,nImageIndex);
				//If the Item is not a button
				if(nStyle&TBBS_SEPARATOR)
					continue;
				m_mapCommands.SetAt(nID,-1);
			}
		}
	}

	//
	////////////////////////////////////////

	// built image list with all images used in the toolbars
	VERIFY(m_ilImages.Create(ID_OXCUSTOMIZE_TBIMAGE_WIDTH,
		ID_OXCUSTOMIZE_TBIMAGE_HEIGHT,ILC_COLOR24|ILC_MASK,0,0));
	// Iterate all the control bars and use only the toolbars
	pos=pMainFrameWnd->m_listControlBars.GetHeadPosition();
	while(pos!=NULL)
	{
		CToolBar* pToolbar=DYNAMIC_DOWNCAST(CToolBar, 
			(CControlBar*)pMainFrameWnd->m_listControlBars.GetNext(pos));
		//If its a CToolBar
		if(pToolbar!=NULL)
		{
			// Get the original imagelist of the toolbar
			HIMAGELIST hOrigImageList=
				(HIMAGELIST)pToolbar->SendMessage(TB_GETIMAGELIST);
			int nFirstImageIndex=0;
			if(hOrigImageList!=NULL)
			{
				CImageList origImageList;
				CImageList* pOrigImageList=
					CImageList::FromHandlePermanent(hOrigImageList);
				if(pOrigImageList==NULL)
				{
					if(!origImageList.Attach(hOrigImageList))
					{
						TRACE(_T("COXCustomizeShortkeysPage::InitializeCommands: failed to retrieve image list from toolbars\n"));
						return FALSE;
					}
					pOrigImageList=&origImageList;
				}

				int nOrigImageCount=pOrigImageList->GetImageCount();
				ASSERT(0<nOrigImageCount);

				nFirstImageIndex=m_ilImages.GetImageCount();

				for(int nButtonIndex=0; nButtonIndex<nOrigImageCount; nButtonIndex++)
				{
					HICON hIcon=pOrigImageList->ExtractIcon(nButtonIndex);
					ASSERT(hIcon!=NULL);
					VERIFY(m_ilImages.Add(hIcon)!=-1);
					VERIFY(::DestroyIcon(hIcon));
				}

				// Close everything
				if((HIMAGELIST)origImageList!=NULL)
				{
					origImageList.Detach();
				}
			}

			int nButtonCount=pToolbar->GetToolBarCtrl().GetButtonCount();
			// Iterate all buttons on the toolbar
			for(int nButtonIndex=0; nButtonIndex<nButtonCount; nButtonIndex++)
			{
				UINT nID=0;
				UINT nStyle=0;
				int nImageIndex=0;
				pToolbar->GetButtonInfo(nButtonIndex,nID,nStyle,nImageIndex);
				//If the Item is not a button
				if(nStyle&TBBS_SEPARATOR || nImageIndex==-1)
					continue;

				int nImageIndexTest=-1;
				// check if this button is in the commands map
				VERIFY(m_mapCommands.Lookup(nID,nImageIndexTest));
				if(nImageIndexTest==-1)
				{
					nImageIndexTest=nImageIndex+nFirstImageIndex;
					m_mapCommands.SetAt(nID,nImageIndexTest);
				}
			}
		}
	}

	// add to the image list images that would be used in tree of all available 
	// commands in order to represent: command groups, command without image,
	// new menu ite and separator item
	//
	int nFirstImageIndex=m_ilImages.GetImageCount();
	HICON hIcon=pApp->LoadIcon(IDI_OX_CUSTOMIZE_COMMANDGROUP);
	ASSERT(hIcon!=NULL);
	VERIFY(m_ilImages.Add(hIcon)==nFirstImageIndex+ID_OX_IMAGEOFFSET_MENUGROUP);

	hIcon=pApp->LoadIcon(IDI_OX_CUSTOMIZE_NOIMAGE);
	ASSERT(hIcon!=NULL);
	VERIFY(m_ilImages.Add(hIcon)==nFirstImageIndex+ID_OX_IMAGEOFFSET_NOIMAGE);

	//
	/////////////////////////

	m_shortkeysOrganizer.Attach(pFrameWnd,TRUE);
	
	if(lpszProfileName!=NULL)
	{
		m_sProfileName=lpszProfileName;
		VERIFY(m_shortkeysOrganizer.LoadState(m_sProfileName));
	}

	return TRUE;
}


BOOL COXCustomizeShortkeysPage::ApplyChanges()
{
	if(!UpdateData(TRUE))
		return FALSE;

	VERIFY(m_shortkeysOrganizer.SetAccelerators());

	return TRUE;
}


void COXCustomizeShortkeysPage::DoDataExchange(CDataExchange* pDX)
{
	COXCustomizePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXCustomizeShortkeysPage)
	DDX_Control(pDX, IDC_OX_STATIC_DESCRIPTION, m_textDescription);
	DDX_Control(pDX, IDC_OX_HOTKEY_SHORTKEY, m_hotKey);
	DDX_Control(pDX, IDC_OX_COMBO_TABLE, m_comboTable);
	DDX_Control(pDX, IDC_OX_TREE_COMMANDS, m_treeCommands);
	DDX_Control(pDX, IDC_OX_SEPARATOR_SHORTKEYS, m_sepShortkeys);
	DDX_Control(pDX, IDC_OX_SEPARATOR_DESCRIPTION, m_sepDescription);
	DDX_Control(pDX, IDC_OX_SEPARATOR_CONFLICTS, m_sepConflict);
	DDX_Control(pDX, IDC_OX_SEPARATOR_COMMANDS, m_sepCommands);
	DDX_Control(pDX, IDC_OX_LIST_SHORTKEYS, m_listShortkeys);
	DDX_Text(pDX, IDC_OX_STATIC_ASSIGNED_TO, m_sAssignedTo);
	DDX_Text(pDX, IDC_OX_STATIC_DESCRIPTION, m_sCommandDescription);
	DDX_CBIndex(pDX, IDC_OX_COMBO_TABLE, m_nAccelTableIndex);
	DDX_LBIndex(pDX, IDC_OX_LIST_SHORTKEYS, m_nShortkeyIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COXCustomizeShortkeysPage, COXCustomizePage)
	//{{AFX_MSG_MAP(COXCustomizeShortkeysPage)
	ON_BN_CLICKED(IDC_OX_BUTTON_ASSIGN, OnButtonAssign)
	ON_BN_CLICKED(IDC_OX_BUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(IDC_OX_BUTTON_REMOVE_ALL, OnButtonRemoveAll)
	ON_BN_CLICKED(IDC_OX_BUTTON_RESET_ALL, OnButtonResetAll)
	ON_CBN_SELCHANGE(IDC_OX_COMBO_TABLE, OnSelchangeComboTable)
	ON_LBN_SELCHANGE(IDC_OX_LIST_SHORTKEYS, OnSelchangeListShortkeys)
	ON_NOTIFY(TVN_SELCHANGED, IDC_OX_TREE_COMMANDS, OnSelchangedTreeCommands)
	ON_NOTIFY(NM_KEYDOWN, IDC_OX_HOTKEY_SHORTKEY, OnKeydownHotkey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeShortkeysPage message handlers

BOOL COXCustomizeShortkeysPage::Load(const COXCustomizeManager* pCustomizeManager)
{
	// Call default implementation. It will load this demo dialog as the first 
	// page and will create About and CodeSample pages if specified.
	if(!COXCustomizePage::Load(pCustomizeManager))
		return FALSE;

	return TRUE;
}


void COXCustomizeShortkeysPage::Unload()
{
	// add here code for cleaning up all objects created by demo
	//
	//
	//////////////////////////////////////////////////////////////////////////

	COXCustomizePage::Unload();
}

void COXCustomizeShortkeysPage::OnInitDialog()
{
	// must call default implementation
	COXCustomizePage::OnInitDialog();

	// add here initialization code for your demo dialog. Treat it as a
	// normal CDialog::OnInitDialog function

	// separators
	COLORREF clr=::GetSysColor(COLOR_WINDOW);
	m_sepShortkeys.SetTextColor(clr);
	m_sepDescription.SetTextColor(clr);
	m_sepConflict.SetTextColor(clr);
	m_sepCommands.SetTextColor(clr);

	// populate tree
	GetVars();

	// populate table of accelerator contexts
	COXArrAccelTables* pAccelTables=m_shortkeysOrganizer.GetAccelTables();
	ASSERT(pAccelTables!=NULL);
	if(pAccelTables->GetSize()==0)
	{
		// there is no accelerator tables defined for this application
		// as aresult all controls on this dialog must be disabled
		CWnd* pWndChild=GetWindow(GW_CHILD);
		while(pWndChild!=NULL)
		{
			pWndChild->EnableWindow(FALSE);
			pWndChild=pWndChild->GetNextWindow();
		}
		m_bReadOnly=TRUE;
	}
	else
	{
		for(int nIndex=0; nIndex<pAccelTables->GetSize(); nIndex++)
		{
			CString sText=OXDEFAULTACCEL;
			if(pAccelTables->GetAt(nIndex).m_docTemplate!=NULL)
			{
				pAccelTables->GetAt(nIndex).m_docTemplate->
					GetDocString(sText,CDocTemplate::docName);
			}
			m_comboTable.AddString(sText);
		}
	}

	m_nAccelTableIndex=0;

	// layout
	//
	m_LayoutManager.TieChild(&m_treeCommands,OX_LMS_ANY,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_sepCommands,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_sepDescription,
		OX_LMS_LEFT|OX_LMS_RIGHT|OX_LMS_BOTTOM,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_STATIC_DESCRIPTION,
		OX_LMS_LEFT|OX_LMS_RIGHT|OX_LMS_BOTTOM,OX_LMT_SAME);

	m_LayoutManager.TieChild(IDC_OX_SEPARATOR_SHORTKEYS,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_STATIC_FOR,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_COMBO_TABLE,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_STATIC_CURRENT_KEYS,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_LIST_SHORTKEYS,
		OX_LMS_RIGHT|OX_LMS_BOTTOM|OX_LMS_TOP,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_BUTTON_ASSIGN,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_BUTTON_REMOVE,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_BUTTON_REMOVE_ALL,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_BUTTON_RESET_ALL,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_STATIC_NEW_KEY,
		OX_LMS_RIGHT|OX_LMS_BOTTOM,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_HOTKEY_SHORTKEY,
		OX_LMS_RIGHT|OX_LMS_BOTTOM,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_SEPARATOR_CONFLICTS,
		OX_LMS_RIGHT|OX_LMS_BOTTOM,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_STATIC_ASSIGNED_TO,
		OX_LMS_RIGHT|OX_LMS_BOTTOM,OX_LMT_SAME);
	//
	///////////////////////////////////////

	UpdateData(FALSE);
	GetShortkeys();
	ShowVars();

	m_bInitialized=TRUE;
}


void COXCustomizeShortkeysPage::ShowVars()
{
	if(!UpdateData(TRUE))
		return;

	if(m_bReadOnly)
		return;

	HTREEITEM hSelectedItem=m_treeCommands.GetSelectedItem();
	UINT nCommandID=(hSelectedItem==NULL ? (UINT)-1 :
		(UINT)m_treeCommands.GetItemData(hSelectedItem));
	GetDlgItem(IDC_OX_HOTKEY_SHORTKEY)->EnableWindow(nCommandID!=(UINT)-1);
	GetDlgItem(IDC_OX_BUTTON_REMOVE_ALL)->EnableWindow(nCommandID!=(UINT)-1 && 
		m_listShortkeys.GetCount()>0);
	GetDlgItem(IDC_OX_BUTTON_RESET_ALL)->EnableWindow(!m_sProfileName.IsEmpty());

	WORD wVirtualKeyCode;
	WORD wModifiers;
	m_hotKey.GetHotKey(wVirtualKeyCode,wModifiers);
	GetDlgItem(IDC_OX_BUTTON_ASSIGN)->EnableWindow(wVirtualKeyCode!=0);

	BOOL bUpdatable=(m_nShortkeyIndex!=-1);
	if(bUpdatable)
	{
		bUpdatable&=(m_listShortkeys.GetItemData(m_nShortkeyIndex)==0);
	}
	GetDlgItem(IDC_OX_BUTTON_REMOVE)->EnableWindow(bUpdatable);
}


void COXCustomizeShortkeysPage::GetVars()
{
	CWinApp* pApp=AfxGetApp();
	ASSERT(pApp!=NULL);

	// populate tree control
	//
	m_treeCommands.DeleteAllItems();

	// associate image list with tree
	m_treeCommands.SetImageList(&m_ilImages,TVSIL_NORMAL);

	// build tree on the base of all menus available in the system
	//
	HINSTANCE hInstance=AfxFindResourceHandle(MAKEINTRESOURCE(IDR_MAINFRAME),RT_MENU);
	if(hInstance!=NULL)
	{
		HMENU hMenu=::LoadMenu(hInstance,MAKEINTRESOURCE(IDR_MAINFRAME));
		if(hMenu!=NULL)
		{
			AddCommandsFromMenuToTree(hMenu,TVI_ROOT);
			::DestroyMenu(hMenu);
		}
	}

	if(m_pCustomizeManager->IsMDIFramework())
	{
		POSITION pos=pApp->GetFirstDocTemplatePosition();
		while(pos!=NULL)
		{
			CDocTemplate* pDocTemplate=pApp->GetNextDocTemplate(pos);
			ASSERT(pDocTemplate!=NULL);
			CMultiDocTemplate* pMultiDocTemplate=
				DYNAMIC_DOWNCAST(CMultiDocTemplate,pDocTemplate);
			if(pMultiDocTemplate!=NULL)
			{
				if(pMultiDocTemplate->m_hMenuShared!=NULL)
				{
					AddCommandsFromMenuToTree(pMultiDocTemplate->m_hMenuShared,
						TVI_ROOT);
				}
			}
		}
	}
}


void COXCustomizeShortkeysPage::RetrieveCommandsFromMenu(HMENU hMenu)
{
	ASSERT(hMenu!=NULL);

	CMenu* pMenu=CMenu::FromHandle(hMenu);
	ASSERT(pMenu!=NULL);
	
	int nItemCount=pMenu->GetMenuItemCount();
	for(int nIndex=0; nIndex<nItemCount; nIndex++)
	{
		if(pMenu->GetMenuItemID(nIndex)==-1)
		{
			// popup menu
			CMenu* pPopupMenu=pMenu->GetSubMenu(nIndex);
			ASSERT(pPopupMenu!=NULL);
			RetrieveCommandsFromMenu((HMENU)*pPopupMenu);
		}
		else
		{
			UINT nCommandID=pMenu->GetMenuItemID(nIndex);
			if(nCommandID!=0)
			{
				// don't add most recent files used 
				if(nCommandID>=ID_FILE_MRU_FILE1 && nCommandID<=ID_FILE_MRU_FILE16)
					continue;

				// don't add MDIChild windows menu items 
				if(m_pCustomizeManager->IsMDIFramework() && 
					nCommandID>=AFX_IDM_FIRST_MDICHILD)
					continue;

				HINSTANCE hInstance=
					AfxFindResourceHandle(MAKEINTRESOURCE(nCommandID),RT_STRING);
				if(hInstance!=NULL)
					m_mapCommands.SetAt(nCommandID,-1);
			}
		}
	}
}


void COXCustomizeShortkeysPage::AddCommandsFromMenuToTree(HMENU hMenu, 
														  HTREEITEM hParentItem)
{
	ASSERT(hMenu!=NULL);
	ASSERT(hParentItem!=NULL);

	CMenu* pMenu=CMenu::FromHandle(hMenu);
	ASSERT(pMenu!=NULL);
	BOOL bIsBitmapMenu=pMenu->IsKindOf(RUNTIME_CLASS(COXBitmapMenu));
	
	int nItemCount=pMenu->GetMenuItemCount();
	for(int nIndex=0; nIndex<nItemCount; nIndex++)
	{
		UINT nCommandID=pMenu->GetMenuItemID(nIndex);
		if(nCommandID==0)
			continue;

		if(nCommandID!=-1)
		{
			// don't add most recent files used 
			if(nCommandID>=ID_FILE_MRU_FILE1 && nCommandID<=ID_FILE_MRU_FILE16)
				continue;

			// don't add MDIChild windows menu items 
			if(m_pCustomizeManager->IsMDIFramework() && 
				nCommandID>=AFX_IDM_FIRST_MDICHILD)
				continue;

			HINSTANCE hInstance=
				AfxFindResourceHandle(MAKEINTRESOURCE(nCommandID),RT_STRING);
			if(hInstance==NULL)
				continue;
		}

		CString sText(_T(""));
		if(bIsBitmapMenu)
		{
			MENUITEMINFO mii={ sizeof(mii) };
			mii.fMask=MIIM_TYPE|MIIM_DATA;
			::GetMenuItemInfo(pMenu->GetSafeHmenu(),nIndex,TRUE,&mii);

			if(mii.fType==MFT_OWNERDRAW)
			{
				COXItemInfo* pItemInfo=(COXItemInfo*)(mii.dwItemData);
				ASSERT(pItemInfo!=NULL);
				sText=pItemInfo->GetText();
			}
		}
		if(sText.IsEmpty())
		{
			pMenu->GetMenuString(nIndex,sText,MF_BYPOSITION);
		}
		ASSERT(!sText.IsEmpty());

		int nPosition=sText.Find(_T('\t'));
		if(nPosition!=-1)
			sText=sText.Left(nPosition);
		
		nPosition=sText.Find(_T('&'));
		if(nPosition!=-1)
			sText=sText.Left(nPosition)+sText.Mid(nPosition+1);

		HTREEITEM hItem=FindMenuItemInTree(sText,hParentItem);
		if(hItem==NULL)
		{
			int nFirstSpecialImage=m_ilImages.GetImageCount()-ID_OX_IMAGEOFFSET_COUNT;
			int nImageIndex=-1;
			if(nCommandID!=-1)
			{
				VERIFY(m_mapCommands.Lookup(nCommandID,nImageIndex));
				if(nImageIndex==-1)
					nImageIndex=nFirstSpecialImage+ID_OX_IMAGEOFFSET_NOIMAGE;
			}
			else
			{
				nImageIndex=nFirstSpecialImage+ID_OX_IMAGEOFFSET_MENUGROUP;
			}

			hItem=m_treeCommands.InsertItem(sText,nImageIndex,nImageIndex,
				hParentItem,TVI_LAST);
			ASSERT(hItem!=NULL);
			m_treeCommands.SetItemData(hItem,(LPARAM)nCommandID);
		}

		if(nCommandID==-1)
		{
			// popup menu
			m_treeCommands.SetItemState(hItem,TVIS_BOLD,TVIS_BOLD);

			CMenu* pPopupMenu=pMenu->GetSubMenu(nIndex);
			ASSERT(pPopupMenu!=NULL);
			AddCommandsFromMenuToTree((HMENU)*pPopupMenu,hItem);
		}
	}
}


HTREEITEM COXCustomizeShortkeysPage::FindMenuItemInTree(CString sText, 
														HTREEITEM hParentItem) const
{
	ASSERT(!sText.IsEmpty());
	ASSERT(hParentItem!=NULL);

	HTREEITEM hChildItem=m_treeCommands.GetNextItem(hParentItem,TVGN_CHILD);

	HTREEITEM hFoundItem=NULL;
	while(hChildItem!=NULL)
	{
		CString sTextToCompare=m_treeCommands.GetItemText(hChildItem);
		if(sText.CompareNoCase(sTextToCompare)==0)
		{
			hFoundItem=hChildItem;
			break;
		}

		hChildItem=m_treeCommands.GetNextItem(hChildItem,TVGN_NEXT);
	}

	return hFoundItem;
}



void COXCustomizeShortkeysPage::OnButtonAssign() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData(TRUE))
		return;

	HTREEITEM hSelectedItem=m_treeCommands.GetSelectedItem();
	ASSERT(hSelectedItem!=NULL);
	UINT nCommandID=(UINT)m_treeCommands.GetItemData(hSelectedItem);
	ASSERT(nCommandID!=(UINT)-1);

	WORD wVirtualKeyCode;
	WORD wModifiers;
	m_hotKey.GetHotKey(wVirtualKeyCode,wModifiers);

	BYTE fVirt=FVIRTKEY;
	if(wModifiers&HOTKEYF_SHIFT)
		fVirt|=FSHIFT;
	if(wModifiers&HOTKEYF_CONTROL)
		fVirt|=FCONTROL;
	if(wModifiers&HOTKEYF_ALT)
		fVirt|=FALT;

	COXArrAccelTables* pAccelTables=m_shortkeysOrganizer.GetAccelTables();
	ASSERT(pAccelTables!=NULL);
	ASSERT(m_nAccelTableIndex>=0 && m_nAccelTableIndex<pAccelTables->GetSize());
	VERIFY(m_shortkeysOrganizer.AddAccelerator(fVirt,wVirtualKeyCode,
		(WORD)nCommandID,
		pAccelTables->GetAt(m_nAccelTableIndex).m_docTemplate,TRUE));

	m_hotKey.SetHotKey(0,0);
	m_sAssignedTo.Empty();
	UpdateData(FALSE);

	GetShortkeys();
	ShowVars();
}

void COXCustomizeShortkeysPage::OnButtonRemove() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData(TRUE))
		return;

	ASSERT(m_nShortkeyIndex>=0 && m_nShortkeyIndex<m_listShortkeys.GetCount());
	ASSERT(m_listShortkeys.GetItemData(m_nShortkeyIndex)==0);

	HTREEITEM hSelectedItem=m_treeCommands.GetSelectedItem();
	ASSERT(hSelectedItem!=NULL);
	WORD nCommandID=(WORD)m_treeCommands.GetItemData(hSelectedItem);
	ASSERT(nCommandID!=(WORD)-1);

	CString sKey(_T(""));
	m_listShortkeys.GetText(m_nShortkeyIndex,sKey);
	ACCEL accel;
	VERIFY(m_shortkeysOrganizer.GetAccelKeyFromString(sKey,&accel));
	accel.cmd=nCommandID;

	COXArrAccelTables* pAccelTables=m_shortkeysOrganizer.GetAccelTables();
	ASSERT(pAccelTables!=NULL);
	ASSERT(m_nAccelTableIndex>=0 && m_nAccelTableIndex<pAccelTables->GetSize());
	VERIFY(m_shortkeysOrganizer.RemoveAccelerator(&accel,
		pAccelTables->GetAt(m_nAccelTableIndex).m_docTemplate));

	GetShortkeys();
	ShowVars();
}

void COXCustomizeShortkeysPage::OnButtonRemoveAll() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData(TRUE))
		return;

	HTREEITEM hSelectedItem=m_treeCommands.GetSelectedItem();
	ASSERT(hSelectedItem!=NULL);
	WORD nCommandID=(WORD)m_treeCommands.GetItemData(hSelectedItem);
	ASSERT(nCommandID!=(WORD)-1);

	COXArrAccelTables* pAccelTables=m_shortkeysOrganizer.GetAccelTables();
	ASSERT(pAccelTables!=NULL);
	ASSERT(m_nAccelTableIndex>=0 && m_nAccelTableIndex<pAccelTables->GetSize());
	VERIFY(m_shortkeysOrganizer.RemoveAllAccelerators(nCommandID,
		pAccelTables->GetAt(m_nAccelTableIndex).m_docTemplate));

	GetShortkeys();
	ShowVars();
}

void COXCustomizeShortkeysPage::OnButtonResetAll() 
{
	// TODO: Add your control notification handler code here
	
	if(!m_sProfileName.IsEmpty())
	{
		m_shortkeysOrganizer.LoadState(m_sProfileName);
		GetShortkeys();
		ShowVars();
	}
}

void COXCustomizeShortkeysPage::OnSelchangeComboTable() 
{
	// TODO: Add your control notification handler code here
	
	GetShortkeys();
	ShowVars();
}

void COXCustomizeShortkeysPage::OnSelchangeListShortkeys() 
{
	// TODO: Add your control notification handler code here
	
	ShowVars();
}

void COXCustomizeShortkeysPage::OnSelchangedTreeCommands(NMHDR* pNMHDR, 
														 LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView=(NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	HTREEITEM hSelectedItem=pNMTreeView->itemNew.hItem;
	if(hSelectedItem!=NULL)
	{
		DWORD_PTR dwData=m_treeCommands.GetItemData(hSelectedItem);
		if(dwData==-1)
		{
			// existing popup menu
			m_sCommandDescription.Format(IDS_OX_CSTMZESHORTKEYSGROUP,//"%s group"
				m_treeCommands.GetItemText(hSelectedItem));
		}
		else 
		{
			UINT nCommandID=(UINT)dwData;
			HINSTANCE hInstance=
				AfxFindResourceHandle(MAKEINTRESOURCE(nCommandID),RT_STRING);
			if(hInstance==NULL)
			{
				m_sCommandDescription.Empty();
			}
			else
			{
				m_sCommandDescription.LoadString(nCommandID);
				int nPosition=m_sCommandDescription.Find(_T('\n'));
				if(nPosition!=-1)
					m_sCommandDescription=m_sCommandDescription.Left(nPosition);
			}
		}
	}
	else
	{
		m_sCommandDescription.Empty();
	}

	m_hotKey.SetHotKey(0,0);
	m_sAssignedTo.Empty();
	
	UpdateData(FALSE);

	GetShortkeys();
	ShowVars();

	*pResult=0;
}


void COXCustomizeShortkeysPage::GetShortkeys()
{
	if(!UpdateData(TRUE))
		return;

	HTREEITEM hSelectedItem=m_treeCommands.GetSelectedItem();
	UINT nCommandID=(hSelectedItem==NULL ? (UINT)-1 :
		(UINT)m_treeCommands.GetItemData(hSelectedItem));

	m_listShortkeys.ResetContent();

	if(m_nAccelTableIndex==-1 || nCommandID==(UINT)-1)
	{
		return;
	}

	ASSERT(nCommandID>0);
	COXArrAccelTables* pAccelTables=m_shortkeysOrganizer.GetAccelTables();
	ASSERT(pAccelTables!=NULL);
	ASSERT(m_nAccelTableIndex<pAccelTables->GetSize());
	CMultiDocTemplate* pDocTemplate=
		pAccelTables->GetAt(m_nAccelTableIndex).m_docTemplate;
	COXArrAccelerators arrAccels;
	if(pDocTemplate!=NULL)
	{
		VERIFY(m_shortkeysOrganizer.
			FindAllAccelerators((WORD)nCommandID,NULL,arrAccels));
		for(int nIndex=0; nIndex<arrAccels.GetSize(); nIndex++)
		{
			ACCEL accel=arrAccels[nIndex];
			CString sKey=m_shortkeysOrganizer.GetAccelKeyString(&accel);
			int nNewItemIndex=m_listShortkeys.
				AddString(sKey,OXLBI_TEXTCOLOR,NULL,RGB(128,128,128));
			ASSERT(nNewItemIndex!=LB_ERR);
			// couldn't be removed or reset
			m_listShortkeys.SetItemData(nNewItemIndex,(DWORD)-1);
		}		
		arrAccels.RemoveAll();
	}
	VERIFY(m_shortkeysOrganizer.
		FindAllAccelerators((WORD)nCommandID,pDocTemplate,arrAccels));
	for(int nIndex=0; nIndex<arrAccels.GetSize(); nIndex++)
	{
		ACCEL accel=arrAccels[nIndex];
		CString sKey=m_shortkeysOrganizer.GetAccelKeyString(&accel);
		int nNewItemIndex=m_listShortkeys.AddString(sKey);
		ASSERT(nNewItemIndex!=LB_ERR);
		// can be removed or reset
		m_listShortkeys.SetItemData(nNewItemIndex,0);
	}		
}

void COXCustomizeShortkeysPage::OnKeydownHotkey(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UNREFERENCED_PARAMETER(pNMHDR);

	ShowVars();

	WORD wVirtualKeyCode;
	WORD wModifiers;
	m_hotKey.GetHotKey(wVirtualKeyCode,wModifiers);

	BYTE fVirt=FVIRTKEY;
	if(wModifiers&HOTKEYF_SHIFT)
		fVirt|=FSHIFT;
	if(wModifiers&HOTKEYF_CONTROL)
		fVirt|=FCONTROL;
	if(wModifiers&HOTKEYF_ALT)
		fVirt|=FALT;

	COXArrAccelTables* pAccelTables=m_shortkeysOrganizer.GetAccelTables();
	ASSERT(pAccelTables!=NULL);
	ASSERT(m_nAccelTableIndex>=0 && m_nAccelTableIndex<pAccelTables->GetSize());
	WORD nFoundCommandID=m_shortkeysOrganizer.FindAccelerator(fVirt,wVirtualKeyCode,
		pAccelTables->GetAt(m_nAccelTableIndex).m_docTemplate);
	if(nFoundCommandID!=0)
	{
		HINSTANCE hInstance=
			AfxFindResourceHandle(MAKEINTRESOURCE(nFoundCommandID),RT_STRING);
		if(hInstance==NULL)
		{
			m_sAssignedTo.Format(IDS_OX_CSTMZESHORTKEYSASSIGNED,//"Currently assigned to the command:\nwith ID=%d"
				nFoundCommandID);
		}
		else
		{
			CString sCommandDescription(_T(""));
			sCommandDescription.LoadString(nFoundCommandID);
			int nPosition=sCommandDescription.Find(_T('\n'));
			if(nPosition!=-1)
			{
				sCommandDescription=sCommandDescription.Mid(nPosition+1);
			}
			m_sAssignedTo.Format(IDS_OX_CSTMZESHORTKEYSASSIGNEDS,//"Currently assigned to the command:\n<%s>"
				sCommandDescription);
		}
	}
	else
	{
		m_sAssignedTo.Empty();
	}
	UpdateData(FALSE);

	*pResult = 0;
}

BOOL COXCustomizeShortkeysPage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(m_shortkeysOrganizer.IsAttached() &&
		pMsg->message>=WM_KEYFIRST && pMsg->message<=WM_KEYLAST)
	{
		COXArrAccelerators arrAccels;
		BYTE fVirt=FVIRTKEY;
		if(::GetKeyState(VK_SHIFT)<0)
			fVirt|=FSHIFT;
		if(::GetKeyState(VK_CONTROL)<0)
			fVirt|=FCONTROL;
		if(::GetKeyState(VK_MENU)<0)
			fVirt|=FALT;

		VERIFY(m_shortkeysOrganizer.
			FindAllAccelerators(fVirt,(WORD)pMsg->wParam,arrAccels));
		if(arrAccels.GetSize()>0)
			return FALSE;

		VERIFY(m_shortkeysOrganizer.
			FindAllRemovedAccelerators(fVirt,(WORD)pMsg->wParam,arrAccels));
		if(arrAccels.GetSize()>0)
			return FALSE;
	}
	return COXCustomizePage::PreTranslateMessage(pMsg);
}
