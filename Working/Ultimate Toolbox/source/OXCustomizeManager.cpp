// ==========================================================================
//			Class Implementation: COXCustomizeManager
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
#include "OXCustomizeManager.h"

#include "UTBStrOp.h"
#include "UTB64Bit.h"

const int OXCUTOMIZEMANAGER_MIN_WIDTH=520;
const int OXCUTOMIZEMANAGER_MIN_HEIGHT=450;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COXCustomizeManager::COXCustomizeManager(CWnd* pParent/*=NULL*/) : 
	CDialog(COXCustomizeManager::IDD, pParent),
	m_pActivePage(NULL),
	m_rectPage(0,0,0,0),
	m_bInitialized(FALSE)
{		
	// create image lists for shortcut bar
	VERIFY(m_ilShortcutBar.Create(32,32,ILC_COLOR24|ILC_MASK,0,0));

#ifdef OX_CUSTOMIZE_SHORTKEYS
	m_mapObjects.SetAt(GetShortkeysPage(),SHORTKEYS);
#endif	//	OX_CUSTOMIZE_SHORTKEYS

#ifdef OX_CUSTOMIZE_SPLASHWINDOW
	m_mapObjects.SetAt(GetSplashWindowPage(),SPLASHWINDOW);
#endif	//	OX_CUSTOMIZE_SPLASHWINDOW

#ifdef OX_CUSTOMIZE_INSTANCE_MANAGER
	m_mapObjects.SetAt(GetInstanceManagerPage(),INSTANCE_MANAGER);
#endif	//	OX_CUSTOMIZE_INSTANCE_MANAGER

#ifdef OX_CUSTOMIZE_TRAYICON
	m_mapObjects.SetAt(GetTrayIconPage(),TRAYICON);
#endif	//	OX_CUSTOMIZE_TRAYICON

#ifdef OX_CUSTOMIZE_COOLCONTROLS
	m_mapObjects.SetAt(GetCoolControlsPage(),COOLCONTROLS);
#endif	//	OX_CUSTOMIZE_COOLCONTROLS

#ifdef OX_CUSTOMIZE_INTELLIMOUSE
	m_mapObjects.SetAt(GetIntelliMousePage(),INTELLIMOUSE);
#endif	//	OX_CUSTOMIZE_INTELLIMOUSE

#ifdef OX_CUSTOMIZE_CAPTION
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&osvi);
	if (!(osvi.dwMajorVersion >= 5 && osvi.dwMinorVersion >= 1))
		m_mapObjects.SetAt(GetCaptionPage(),CAPTION_PAINTER); // only if version of Windows is lower than XP
#endif	//	OX_CUSTOMIZE_CAPTION

#ifdef OX_CUSTOMIZE_TABBEDMDI
	m_mapObjects.SetAt(GetTabbedMDIPage(),TABBEDMDI);
#endif	//	OX_CUSTOMIZE_TABBEDMDI

#ifdef OX_CUSTOMIZE_WORKSPACE_STATE
	m_mapObjects.SetAt(GetWorkspaceStatePage(),WORKSPACE_STATE);
#endif	//	OX_CUSTOMIZE_WORKSPACE_STATE

#ifdef OX_CUSTOMIZE_BACKGROUND
	m_mapObjects.SetAt(GetBackgroundPage(),BACKGROUND_PAINTER);
#endif	//	OX_CUSTOMIZE_BACKGROUND

#ifdef OX_CUSTOMIZE_TOOLBARS
	m_mapObjects.SetAt(GetToolbarsPage(),TOOLBARS);
#endif	//	OX_CUSTOMIZE_CAPTION

#ifdef OX_CUSTOMIZE_COMMANDS
	m_mapObjects.SetAt(GetCommandsPage(),COMMANDS);
#endif	//	OX_CUSTOMIZE_COMMANDS
}

COXCustomizeManager::~COXCustomizeManager()
{
	// means that object has been created but the dialog never been displayed
	m_mapObjects.RemoveAll();
}

void COXCustomizeManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXCustomizeManager)
	DDX_Control(pDX, IDC_OX_SEPARATOR_TITLE, m_sepTitle);
	DDX_Control(pDX, IDC_OX_SEPARATOR_BOTTOM, m_sepBottom);
	DDX_Control(pDX, IDC_OX_SHB_PAGES, m_shb);
	DDX_Control(pDX, IDC_OX_STATIC_TITLE, m_title);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COXCustomizeManager, CDialog)
	//{{AFX_MSG_MAP(COXCustomizeManager)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_OX_BUTTON_APPLY, OnButtonApply)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_NOTIFY(SHBN_ACTIVATEITEM,IDC_OX_SHB_PAGES,OnActivateItem)
	ON_NOTIFY(SHBN_GETITEMINFOTIP,IDC_OX_SHB_PAGES,OnGetItemInfoTip)
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////

BOOL COXCustomizeManager::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	// setup shortcut bar
	m_shb.SetBarStyle(SHBS_INFOTIP|SHBS_UNDERLINEHOTITEM|SHBS_SHOWACTIVEALWAYS|
		SHBS_BOLDEXPANDEDGROUP|SHBS_DISABLEDRAGDROPITEM|SHBS_DRAWHEADERDRAGIMAGE);

	// setup title
	m_title.SetTextColor(::GetSysColor(COLOR_CAPTIONTEXT));
	m_title.SetBkColor(::GetSysColor(COLOR_BTNFACE));
	m_title.SetBackgroundFillType(GRADIENT_LEFT);
	CFont fontTitle;
	VERIFY(fontTitle.CreatePointFont(120,_T("MS Sans Serif")));
	m_title.SetTextFont(&fontTitle);

	// layout manager 
	//
	if(m_LayoutManager.IsAttached())
		m_LayoutManager.Detach();
	m_LayoutManager.Attach(this);
	
	// shortcut bar control
	m_LayoutManager.TieChild(IDC_OX_SHB_PAGES,OX_LMS_TOP|OX_LMS_BOTTOM,OX_LMT_SAME);

	// title
	m_LayoutManager.TieChild(IDC_OX_STATIC_TITLE,OX_LMS_RIGHT|OX_LMS_LEFT,OX_LMT_SAME);

	// title separator
	m_LayoutManager.TieChild(IDC_OX_SEPARATOR_TITLE,
		OX_LMS_RIGHT|OX_LMS_LEFT,OX_LMT_SAME);

	// bottom separator
	m_LayoutManager.TieChild(IDC_OX_SEPARATOR_BOTTOM,
		OX_LMS_RIGHT|OX_LMS_LEFT|OX_LMS_BOTTOM,OX_LMT_SAME);

	// OK, Cancel, Apply buttons
	m_LayoutManager.TieChild(IDOK,OX_LMS_BOTTOM|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDCANCEL,OX_LMS_BOTTOM|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_BUTTON_APPLY,OX_LMS_BOTTOM|OX_LMS_RIGHT,OX_LMT_SAME);

	//////////////////////////////////////

	// populate the manager with standard and user defined pages
	PopulatePages();

	for(int nIndex=0; nIndex<m_shb.GetGroupCount(); nIndex++)
	{
		HSHBGROUP hGroup=m_shb.FindGroupByOrder(nIndex);
		ASSERT(hGroup!=NULL);
		m_shb.SortLCItems(hGroup);
	}

#ifdef OX_CUSTOMIZE_WORKSPACE_STATE
	ASSERT(GetWorkspaceStatePage()!=NULL);
	// go through all pages and check if it should be displayed
	POSITION pos=m_mapPages.GetStartPosition();
	while(pos!=NULL)
	{
		DWORD dwID=NULL;
		COXCustomizePage* pCustomizePage=NULL;
		m_mapPages.GetNextAssoc(pos,pCustomizePage,dwID);
		ASSERT(pCustomizePage!=NULL && dwID!=NULL);
		if(!GetWorkspaceStatePage()->IsViewablePage(pCustomizePage))
			VERIFY(DeletePage(pCustomizePage));
	}
#endif	//	OX_CUSTOMIZE_WORKSPACE_STATE

	// activate first page
	HSHBGROUP hGroup=m_shb.FindGroupByOrder(0);
	ASSERT(hGroup!=NULL);
	COXCustomizePage* pCustomizePage=GetPageByLocation(hGroup,0);
	ASSERT(pCustomizePage!=NULL);
	VERIFY(ActivatePage(pCustomizePage));

	// calculate the position of the active page window
	RecalcPageRect();
	// adjust the position of the current active page 
	if(m_pActivePage!=NULL)
		m_pActivePage->MoveWindow(GetPageRect());

	// mark the dialog as initialized
	m_bInitialized=TRUE;

	return FALSE; 
}


BOOL COXCustomizeManager::InsertPage(COXCustomizePage* pCustomizePage, 
									 int nPageIndex)
{
	ASSERT(pCustomizePage!=NULL);

	CString sTitle=pCustomizePage->GetTitle();
	LPCTSTR lpszImageResource=pCustomizePage->GetImageResource(); 
	COLORREF clrMask=pCustomizePage->GetImageMask();
	CString sTooltip=pCustomizePage->GetTooltip();
	CString sGroup=pCustomizePage->GetGroup();

#ifdef _DEBUG
	ASSERT(nPageIndex>=0 && nPageIndex<=GetPageCount(sGroup));

	HSHBGROUP hGroupTest=NULL;
	int nIndexTest=-1;
	ASSERT(!FindPage(pCustomizePage,hGroupTest,nIndexTest));
	ASSERT(!FindPage(sTitle,sGroup,hGroupTest,nIndexTest));
#endif

	// find/create the corresponding shortcut bar group
	HSHBGROUP hGroup=m_shb.FindGroupByTitle(sGroup);
	BOOL bNewGroup=FALSE;
	if(hGroup==NULL)
	{
		hGroup=m_shb.InsertGroup(sGroup);
		bNewGroup=TRUE;
	}
	if(hGroup==NULL)
	{
		TRACE(_T("COXCustomizeManager::InsertPage: failed to create group for the specified page\n"));
		if(bNewGroup)
			m_shb.DeleteGroup(hGroup);
		return FALSE;
	}

	// associate image list with the created group
	m_shb.SetLCImageList(hGroup,&m_ilShortcutBar,LVSIL_NORMAL);

	// get image index for new page
	int nImageIndex=-1;
	if(lpszImageResource!=NULL)
	{
		CImageList imageList;
		if(!imageList.Create(lpszImageResource,32,0,clrMask))
		{
			TRACE(_T("COXCustomizeManager::InsertPage: failed to extract image for new page\n"));
			if(bNewGroup)
			{
				m_shb.DeleteGroup(hGroup);
			}
			return FALSE;
		}
		HICON hIcon=imageList.ExtractIcon(0);
		ASSERT(hIcon!=NULL);
		nImageIndex=m_ilShortcutBar.Add(hIcon);
		ASSERT(nImageIndex!=-1);
		VERIFY(::DestroyIcon(hIcon));
	}

	int nItem=m_shb.InsertLCItem(hGroup,nPageIndex,sTitle,nImageIndex,
		(LPARAM)pCustomizePage);
	if(nItem==-1)
	{
		TRACE(_T("COXCustomizeManager::InsertPage: failed to insert new item into the shortcut bar\n"));
		if(bNewGroup)
		{
			m_shb.DeleteGroup(hGroup);
		}
		return FALSE;
	}

	m_mapPages.SetAt(pCustomizePage,((PtrToLong(hGroup))<<16)+nItem);
	m_mapTooltips.SetAt(pCustomizePage,sTooltip);

	return TRUE;
}


BOOL COXCustomizeManager::DeletePage(COXCustomizePage* pCustomizePage)
{
	HSHBGROUP hGroup=NULL;
	int nIndex=-1;
	if(!FindPage(pCustomizePage,hGroup,nIndex))
	{
		return FALSE;
	}
	ASSERT(hGroup!=NULL && nIndex!=-1);
	if(!m_shb.DeleteLCItem(hGroup,nIndex))
	{
		return FALSE;
	}

	m_mapTooltips.RemoveKey(pCustomizePage);

	return TRUE;
}

void COXCustomizeManager::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_bInitialized)
	{
		RecalcPageRect();
		
		if(m_pActivePage!=NULL)
		{
			m_pActivePage->MoveWindow(GetPageRect());
		}
	}
}


void COXCustomizeManager::RecalcPageRect()
{
	CRect rectSepTitle;
	m_sepTitle.GetWindowRect(rectSepTitle);
	ScreenToClient(rectSepTitle);

	CRect rectSepBottom;
	m_sepBottom.GetWindowRect(rectSepBottom);
	ScreenToClient(rectSepBottom);

	CRect rectPage(rectSepTitle.left,rectSepTitle.bottom,
		rectSepTitle.right,rectSepBottom.top);

	m_rectPage=rectPage;
}


void COXCustomizeManager::PopulatePages()
{
	POSITION pos=m_mapObjects.GetStartPosition();
	while(pos!=NULL)
	{
		DWORD dwType=0;
		COXCustomizePage* pCustomizePage=NULL;
		m_mapObjects.GetNextAssoc(pos,pCustomizePage,dwType);
		ASSERT(pCustomizePage!=NULL);
		if(IsIncludingStandardPage((CustomizeManagerPage)dwType))
			VERIFY(AddPage(pCustomizePage));
	}
}


int COXCustomizeManager::GetAllPageCount() const 
{
	int nPageCount=0;
	int nGroupCount= PtrToInt(m_shb.GetGroupCount());
	for(int nIndex=0; nIndex<nGroupCount; nIndex++)
	{
		HSHBGROUP hGroup=m_shb.FindGroupByOrder(nIndex);
		ASSERT(hGroup!=NULL);
		nPageCount+=GetPageCount(hGroup);
	}
	return nPageCount;
}


BOOL COXCustomizeManager::FindPage(CustomizeManagerPage pageType, 
								   HSHBGROUP& hGroupTest, int& nIndexTest) const
{
	POSITION pos=m_mapObjects.GetStartPosition();
	while(pos!=NULL)
	{
		DWORD dwType=0;
		COXCustomizePage* pCustomizePage=NULL;
		m_mapObjects.GetNextAssoc(pos,pCustomizePage,dwType);
		ASSERT(pCustomizePage!=NULL);
		if((CustomizeManagerPage)dwType==pageType)
			return FindPage(pCustomizePage,hGroupTest,nIndexTest);
	}
	return FALSE;
}

BOOL COXCustomizeManager::FindPage(COXCustomizePage* pCustomizePage, 
								   HSHBGROUP& hGroupTest, int& nIndexTest) const
{
	if(pCustomizePage==NULL)
		return FALSE;

	BOOL bFound=FALSE;
	int nGroupCount= PtrToInt(m_shb.GetGroupCount());
	for(int nIndex=0; nIndex<nGroupCount; nIndex++)
	{
		HSHBGROUP hGroup=m_shb.FindGroupByOrder(nIndex);
		ASSERT(hGroup!=NULL);
		COXSHBListCtrl* pListCtrl=m_shb.GetGroupListCtrl(hGroup);
		if(pListCtrl!=NULL)
		{
			LV_FINDINFO findInfo;
			findInfo.flags=LVFI_PARAM;
			findInfo.lParam=(LPARAM)pCustomizePage;
			nIndexTest=pListCtrl->FindItem(&findInfo);
			if(nIndexTest!=-1)
			{
				hGroupTest=hGroup;
				bFound=TRUE;
				break;
			}

		}
	}

	return bFound;
}

BOOL COXCustomizeManager::FindPage(LPCTSTR lpszTitle, LPCTSTR lpszGroup, 
								   HSHBGROUP& hGroupTest, int& nIndexTest) const
{
	hGroupTest=m_shb.FindGroupByTitle(lpszGroup);
	if(hGroupTest==NULL)
		return FALSE;
	COXSHBListCtrl* pListCtrl=m_shb.GetGroupListCtrl(hGroupTest);
	if(pListCtrl==NULL)
		return FALSE;

	LV_FINDINFO findInfo;
	findInfo.flags=LVFI_STRING;
	findInfo.psz=lpszTitle;
	nIndexTest=pListCtrl->FindItem(&findInfo);
	if(nIndexTest==-1)
		return FALSE;

	return TRUE;
}


COXCustomizePage* COXCustomizeManager::GetPageByLocation(HSHBGROUP hGroup, 
														 int nIndex) const 
{
	ASSERT(hGroup!=NULL && nIndex!=-1);
	COXSHBListCtrl* pListCtrl=m_shb.GetGroupListCtrl(hGroup);
	if(pListCtrl==NULL)
	{
		TRACE(_T("COXCustomizeManager::GetPageByLocation: failed to retrieve group's list control\n"));
		return NULL;
	}
	return (COXCustomizePage*)pListCtrl->GetItemData(nIndex);
}


BOOL COXCustomizeManager::ActivatePage(COXCustomizePage* pCustomizePage) 
{
	HSHBGROUP hGroup=NULL;
	int nIndex=-1;
	if(!FindPage(pCustomizePage,hGroup,nIndex))
		return FALSE;
	ASSERT(hGroup!=NULL && nIndex!=-1);
	return ActivatePage(hGroup,nIndex);
}


BOOL COXCustomizeManager::ActivatePage(LPCTSTR lpszTitle, LPCTSTR lpszGroup) 
{
	HSHBGROUP hGroup=NULL;
	int nIndex=-1;
	if(!FindPage(lpszTitle,lpszGroup,hGroup,nIndex))
		return FALSE;
	ASSERT(hGroup!=NULL && nIndex!=-1);
	return ActivatePage(hGroup,nIndex);
}


BOOL COXCustomizeManager::ActivatePage(CustomizeManagerPage pageType) 
{
	POSITION pos=m_mapObjects.GetStartPosition();
	while(pos!=NULL)
	{
		DWORD dwType=0;
		COXCustomizePage* pCustomizePage=NULL;
		m_mapObjects.GetNextAssoc(pos,pCustomizePage,dwType);
		ASSERT(pCustomizePage!=NULL);
		if((CustomizeManagerPage)dwType==pageType)
			return ActivatePage(pCustomizePage);
	}
	return FALSE;
}


BOOL COXCustomizeManager::ActivatePage(HSHBGROUP hGroup, int nIndex)
{
	ASSERT(hGroup!=NULL && nIndex!=-1);

	COXCustomizePage* pCustomizePage=GetPageByLocation(hGroup,nIndex);
	if(pCustomizePage==NULL)
	{
		TRACE(_T("COXCustomizeManager::ActivatePage: failed to retrieve page object\n"));
		return FALSE;
	}

	COXCustomizePage* pActivePage=GetActivePage();
	if(pCustomizePage==pActivePage)
	{
		return TRUE;
	}

	if(pActivePage!=NULL)
	{
		HSHBGROUP hGroupTest=NULL;
		int nIndexTest=-1;
		VERIFY(FindPage(pActivePage,hGroupTest,nIndexTest));
		ASSERT(hGroupTest!=NULL && nIndexTest!=-1);
		if(hGroupTest!=hGroup)
		{
			COXSHBListCtrl* pListCtrl=m_shb.GetGroupListCtrl(hGroupTest);
			ASSERT(pListCtrl!=NULL);
			pListCtrl->ActivateItem(-1);
		}

		pActivePage->Unload();
		m_pActivePage=NULL;
	}

	if(!pCustomizePage->Load(this))
	{
		TRACE(_T("COXCustomizeManager::ActivatePage: failed to load the page\n"));
		return FALSE;
	}
	pCustomizePage->MoveWindow(GetPageRect());
	pCustomizePage->SetFocus();
	m_pActivePage=pCustomizePage;

	// expand corresponding group
	m_shb.ExpandGroup(hGroup);
	// activate corresponding item in the shortcut bar control
	COXSHBListCtrl* pListCtrl=m_shb.GetGroupListCtrl(hGroup);
	ASSERT(pListCtrl!=NULL);
	pListCtrl->ActivateItem(nIndex);
	pListCtrl->EnsureVisible(nIndex,FALSE);

	// change the title text
	m_title.SetWindowText(pListCtrl->GetItemText(nIndex,0));

	// update the state of the "Apply Changes" button
	GetDlgItem(IDC_OX_BUTTON_APPLY)->
		EnableWindow(pCustomizePage->IsSupportingApplyChanges());

	// send initial update to all controls in the activated page
	SendMessageToDescendants(WM_INITIALUPDATE,0,0,TRUE,TRUE);

	return (pListCtrl->GetActiveItem()==nIndex);
}


BOOL COXCustomizeManager::SetPageTooltip(COXCustomizePage* pCustomizePage, 
										 LPCTSTR lpszTooltip) 
{
	HSHBGROUP hGroup=NULL;
	int nIndex=-1;
	if(!FindPage(pCustomizePage,hGroup,nIndex))
		return FALSE;
	ASSERT(hGroup!=NULL && nIndex!=-1);
	m_mapTooltips.SetAt(pCustomizePage,lpszTooltip);
	return TRUE;
}


void COXCustomizeManager::OnActivateItem(NMHDR* pNotifyStruct, LRESULT* result)
{
	LPNMSHORTCUTBAR pNMSHB=(LPNMSHORTCUTBAR)pNotifyStruct;
	ASSERT(pNMSHB!=NULL);

	if(pNMSHB->nItem>=0)
	{
		if(!ActivatePage(pNMSHB->hGroup,pNMSHB->nItem))
		{
			TRACE(_T("COXCustomizeManager::OnActivateItem: failed to activate page\n"));
		}
	}
	*result=0;
}


void COXCustomizeManager::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnGetMinMaxInfo(lpMMI);

	lpMMI->ptMinTrackSize.x=OXCUTOMIZEMANAGER_MIN_WIDTH;
	lpMMI->ptMinTrackSize.y=OXCUTOMIZEMANAGER_MIN_HEIGHT;
}


void COXCustomizeManager::OnGetItemInfoTip(NMHDR* pNotifyStruct, LRESULT* result)
{
	LPNMSHORTCUTBAR pNMSHB=(LPNMSHORTCUTBAR)pNotifyStruct;
	ASSERT(pNMSHB!=NULL);

	COXCustomizePage* pCustomizePage=GetPageByLocation(pNMSHB->hGroup,pNMSHB->nItem);
	ASSERT(pCustomizePage!=NULL);

	CString sTooltip;
	VERIFY(m_mapTooltips.Lookup(pCustomizePage,sTooltip));
	LPSHBINFOTIP pSHBIT=(LPSHBINFOTIP)pNMSHB->lParam; 
	UTBStr::tcsncpy(pSHBIT->szText, 80, sTooltip.GetBuffer(sTooltip.GetLength()),
		sizeof(pSHBIT->szText)/sizeof(pSHBIT->szText[0]));
	sTooltip.ReleaseBuffer();

	*result=1;
}


void COXCustomizeManager::OnButtonApply() 
{
	// TODO: Add your control notification handler code here
	COXCustomizePage* pCustomizePage=GetActivePage();
	if(pCustomizePage!=NULL)
	{
		ASSERT(pCustomizePage->IsSupportingApplyChanges());
		pCustomizePage->ApplyChanges();
	}
}

void COXCustomizeManager::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(!CloseManager(FALSE))
		return;

//	CDialog::OnCancel();
	DestroyWindow();
}

void COXCustomizeManager::OnOK() 
{
	// TODO: Add extra validation here
	if(!CloseManager(TRUE))
		return;

//	CDialog::OnOK();
	DestroyWindow();
}


void COXCustomizeManager::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_bInitialized)
	{
		CloseManager(FALSE);
	}
}


LRESULT COXCustomizeManager::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
	COXCustomizePage* pActivePage=GetActivePage();
	if(pActivePage!=NULL)
	{
		return pActivePage->SendMessage(WM_KICKIDLE,wParam,lParam);
	}

	return FALSE;
}


BOOL COXCustomizeManager::CloseManager(BOOL bIsOk)
{
	ASSERT(m_bInitialized);

	// loop through pages and call OnCloseManager
	POSITION pos=m_mapPages.GetStartPosition();
	while(pos!=NULL)
	{
		COXCustomizePage* pCustomizePage=NULL;
		DWORD dwID=NULL;
		m_mapPages.GetNextAssoc(pos,pCustomizePage,dwID);
		ASSERT(pCustomizePage!=NULL);
		if(::IsWindow(pCustomizePage->GetSafeHwnd()))
		{
			if(!pCustomizePage->OnCloseManager(bIsOk))
			{
				ActivatePage(pCustomizePage);
				return FALSE;
			}
		}
	}

	COXCustomizePage* pActivePage=GetActivePage();
	if(pActivePage!=NULL)
	{
		pActivePage->Unload();
		m_pActivePage=NULL;
	}

	m_shb.DeleteAllGroups();

	// loop through pages and destroy them
	pos=m_mapPages.GetStartPosition();
	while(pos!=NULL)
	{
		COXCustomizePage* pCustomizePage=NULL;
		DWORD dwID=NULL;
		m_mapPages.GetNextAssoc(pos,pCustomizePage,dwID);
		ASSERT(pCustomizePage!=NULL);
		if(::IsWindow(pCustomizePage->GetSafeHwnd()))
		{
			if(!pCustomizePage->DestroyWindow())
			{
				TRACE(_T("COXCustomizeManager::CloseManager: failed to destroy customize page %s"),pCustomizePage->GetTitle());
				return FALSE;
			}
		}
	}

	m_mapPages.RemoveAll();
	m_mapTooltips.RemoveAll();

	m_bInitialized=FALSE;

	return TRUE;
}


BOOL COXCustomizeManager::SaveWorkspaceState() 
{ 

#ifdef OX_CUSTOMIZE_WORKSPACE_STATE
	ASSERT(GetWorkspaceStatePage()!=NULL);
#ifndef OX_CUSTOMIZE_NOTSUPPORTING_DOCKING_FRAMEWORK
	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL);
	if(IsDockingFramework() && GetWorkspaceStatePage()->m_bIncludeBars)
	{
		((COXFrameWndSizeDock*)pWnd)->SaveSizeBarState(_T("SizeBarState"));
	}
#endif	//	OX_CUSTOMIZE_NOTSUPPORTING_DOCKING_FRAMEWORK
#endif	//	OX_CUSTOMIZE_WORKSPACE_STATE

	POSITION pos=m_mapObjects.GetStartPosition();
	while(pos!=NULL)
	{
		DWORD dwType=0;
		COXCustomizePage* pCustomizePage=NULL;
		m_mapObjects.GetNextAssoc(pos,pCustomizePage,dwType);
		ASSERT(pCustomizePage!=NULL);
		VERIFY(pCustomizePage->SaveWorkspaceState());
	}

	return TRUE;
}


#ifdef OX_CUSTOMIZE_WORKSPACE_STATE
BOOL COXCustomizeManager::LoadWorkspaceState(BOOL bIncludeMainWnd/*=TRUE*/, 
											 BOOL bIncludeBars/*=TRUE*/,
											 BOOL bIncludeChildFrames/*=TRUE*/, 
											 BOOL bIncludeSplitterPanes/*=TRUE*/,
											 LPCTSTR lpszProfileName/*=_T("CustomizeWorkspaceState")*/) 
{ 
	ASSERT(GetWorkspaceStatePage()!=NULL);
	if(!GetWorkspaceStatePage()->LoadWorkspaceState(bIncludeMainWnd,
		bIncludeBars,bIncludeChildFrames,bIncludeSplitterPanes,lpszProfileName))
	{
		return FALSE;
	}
#ifndef OX_CUSTOMIZE_NOTSUPPORTING_DOCKING_FRAMEWORK
	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL);
	if(IsDockingFramework() && GetWorkspaceStatePage()->m_bIncludeBars)
	{
		((COXFrameWndSizeDock*)pWnd)->LoadSizeBarState(_T("SizeBarState"));
	}
#endif	//	OX_CUSTOMIZE_NOTSUPPORTING_DOCKING_FRAMEWORK
	return TRUE;
	}
#endif	//	OX_CUSTOMIZE_WORKSPACE_STATE
