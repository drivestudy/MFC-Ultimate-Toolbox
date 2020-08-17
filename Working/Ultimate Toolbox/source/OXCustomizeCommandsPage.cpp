// ==========================================================================
//			Class Implementation: COXCustomizeCommandsPage
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
#include "OXCustomizeCommandsPage.h"
#include "OXCustomizeManager.h"
#include "OXButtonAppearanceDlg.h"
#include "OXDragDropCommands.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int ID_OX_IMAGEOFFSET_MENUGROUP=0;
const int ID_OX_IMAGEOFFSET_NEWPOPUPMENU=1;
const int ID_OX_IMAGEOFFSET_SEPARATOR=2;
const int ID_OX_IMAGEOFFSET_NOIMAGE=3;
const int ID_OX_IMAGEOFFSET_CUSTOMBUTTON=4;
const int ID_OX_IMAGEOFFSET_COUNT=5;

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeCommandsPage dialog

IMPLEMENT_DYNCREATE(COXCustomizeCommandsPage, COXCustomizePage)

COXCustomizeCommandsPage::COXCustomizeCommandsPage()
{
	//{{AFX_DATA_INIT(COXCustomizeCommandsPage)
	m_sDescription = _T("");
	m_bShowIconsInMenu = FALSE;
	m_bShowOnlyRecentlyUsedMenuItems = FALSE;
	//}}AFX_DATA_INIT
	m_nDialogID=IDD;
	m_sProfileName.Empty();
	m_arrCommandIds=NULL;

	m_pCustomizedToolbar=NULL;
	m_nCustomizedPopupMenuIndex=-1;

	m_hCustomButtonWnd=NULL;
}


COXCustomizeCommandsPage::~COXCustomizeCommandsPage()
{
	if(m_arrCommandIds!=NULL)
	{
		delete[] m_arrCommandIds;
	}

	ResetPopupMenus(0);
	POSITION pos=m_mapBMPopupWnd.GetStartPosition();
	COXBitmapMenuPopupWnd* pBMPopupWnd=NULL;
	int nLevel=-1;
	while(pos!=NULL)
	{
		m_mapBMPopupWnd.GetNextAssoc(pos,pBMPopupWnd,nLevel);
		ASSERT(pBMPopupWnd!=NULL);
		ASSERT(nLevel>=0);
		delete pBMPopupWnd;
	}
	m_mapBMPopupWnd.RemoveAll();
}


BOOL COXCustomizeCommandsPage::OnCloseManager(BOOL bIsOk) 
{	
	// hide all menus
	ResetPopupMenus(0);
	m_nCustomizedPopupMenuIndex=-1;
	m_pCustomizedToolbar=NULL;
	m_hCustomButtonWnd=NULL;

	SaveWorkspaceState();

	if(bIsOk)
	{
		if(!UpdateData(TRUE))
			return FALSE;

		CWinApp* pApp=AfxGetApp();
		ASSERT(pApp!=NULL);

		if(!m_sProfileName.IsEmpty())
		{
#ifndef _MAC
			// save menu options
			pApp->WriteProfileInt(m_sProfileName,
				_T("ShowIconsInMenu"),m_bShowIconsInMenu);
			pApp->WriteProfileInt(m_sProfileName,
				_T("ShowOnlyRecentlyUsedMenuItems"),m_bShowOnlyRecentlyUsedMenuItems);
#endif
		}

		if(m_bShowIconsInMenu)
		{
			// Let the menu organizer fix our menus
			if(GetBMOrganizer()->IsAttached())
				GetBMOrganizer()->DetachFrameWnd();

			CWnd* pWnd=AfxGetMainWnd();
			ASSERT(pWnd!=NULL);
			CFrameWnd* pFrameWnd=DYNAMIC_DOWNCAST(CFrameWnd,pWnd);
			ASSERT(pFrameWnd!=NULL);

			// Let the menu organizer fix our menus
			GetBMOrganizer()->AttachFrameWnd(pFrameWnd);
			// use button from toolbars with corresponding items in menu
			GetBMOrganizer()->AutoSetMenuImage();

			GetBMOrganizer()->
				ShowOnlyRecentlyUsedItems(m_bShowOnlyRecentlyUsedMenuItems);
		}
		else if(!m_bShowIconsInMenu && GetBMOrganizer()->IsAttached())
		{
			CFrameWnd* pAttachedFrameWnd=GetBMOrganizer()->DetachFrameWnd();
			UNUSED(pAttachedFrameWnd);
#ifdef _DEBUG
			CWnd* pWnd=AfxGetMainWnd();
			ASSERT(pWnd!=NULL);
			CFrameWnd* pFrameWnd=DYNAMIC_DOWNCAST(CFrameWnd,pWnd);
			ASSERT(pFrameWnd!=NULL);
			ASSERT(pFrameWnd==pAttachedFrameWnd);
#endif
		}
	}

	return TRUE;
}


BOOL COXCustomizeCommandsPage::
InitializeCommands(LPCTSTR lpszCustomImagesResource,
				   COLORREF clrMask/*=RGB(192,192,192)*/,
				   BOOL bShowIconsInMenu/*=TRUE*/,
				   BOOL bShowOnlyRecentlyUsedMenuItems/*=TRUE*/,
				   LPCTSTR lpszProfileName/*=_T("CustomizeCommands")*/)
{
	ASSERT(lpszCustomImagesResource!=NULL);

	CWinApp* pApp=AfxGetApp();
	ASSERT(pApp!=NULL);

	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL);
	CFrameWnd* pFrameWnd=DYNAMIC_DOWNCAST(CFrameWnd,pWnd);
	ASSERT(pFrameWnd!=NULL);

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

	// save the flag that specifies if icon will be displayed in menus
	m_bShowIconsInMenu=bShowIconsInMenu;

	// save the flag that specifies if only recently used menu items 
	// should be displayed
	m_bShowOnlyRecentlyUsedMenuItems=bShowOnlyRecentlyUsedMenuItems;


	// populate image list with custom images
	VERIFY(m_ilCustomImages.Create(lpszCustomImagesResource,
		ID_OXCUSTOMIZE_TBIMAGE_WIDTH,0,clrMask));

	// build image list with all images used in the toolbars
	VERIFY(m_ilImages.Create(ID_OXCUSTOMIZE_TBIMAGE_WIDTH,
		ID_OXCUSTOMIZE_TBIMAGE_HEIGHT,ILC_COLOR24|ILC_MASK,0,0));

	// add to the image list images that would be used in tree of all available 
	// commands in order to represent: command groups, command without image,
	// new menu ite and separator item
	//
	HICON hIcon=pApp->LoadIcon(IDI_OX_CUSTOMIZE_COMMANDGROUP);
	ASSERT(hIcon!=NULL);
	VERIFY(m_ilImages.Add(hIcon)==ID_OX_IMAGEOFFSET_MENUGROUP);

	hIcon=pApp->LoadIcon(IDI_OX_CUSTOMIZE_NEWPOPUPMENU);
	ASSERT(hIcon!=NULL);
	VERIFY(m_ilImages.Add(hIcon)==ID_OX_IMAGEOFFSET_NEWPOPUPMENU);

	hIcon=pApp->LoadIcon(IDI_OX_CUSTOMIZE_SEPARATOR);
	ASSERT(hIcon!=NULL);
	VERIFY(m_ilImages.Add(hIcon)==ID_OX_IMAGEOFFSET_SEPARATOR);

	hIcon=pApp->LoadIcon(IDI_OX_CUSTOMIZE_NOIMAGE);
	ASSERT(hIcon!=NULL);
	VERIFY(m_ilImages.Add(hIcon)==ID_OX_IMAGEOFFSET_NOIMAGE);

	hIcon=pApp->LoadIcon(IDI_OX_CUSTOMIZE_CUSTOMBUTTON);
	ASSERT(hIcon!=NULL);
	VERIFY(m_ilImages.Add(hIcon)==ID_OX_IMAGEOFFSET_CUSTOMBUTTON);

	//
	/////////////////////////
	
	// Iterate all the control bars and use only the toolbars
	pos=pFrameWnd->m_listControlBars.GetHeadPosition();
	while(pos!=NULL)
	{
		CToolBar* pToolbar=DYNAMIC_DOWNCAST(CToolBar, 
			(CControlBar*)pFrameWnd->m_listControlBars.GetNext(pos));
		//If it is a CToolBar
		if(pToolbar!=NULL)
		{
			// Get the original imagelist of the toolbar
			HIMAGELIST hOrigImageList=
				(HIMAGELIST)pToolbar->SendMessage(TB_GETIMAGELIST);
			int nFirstImageIndex=0;
			if(hOrigImageList!=NULL)
			{
				CImageList origImageList;
				if(!origImageList.Attach(hOrigImageList))
				{
					TRACE(_T("COXCustomizeCommandsPage::InitializeCommands: failed to retrieve image list from toolbars\n"));
					return FALSE;
				}

				int nOrigImageCount=origImageList.GetImageCount();
				ASSERT(0<nOrigImageCount);

				nFirstImageIndex=m_ilImages.GetImageCount();

				for(int nButtonIndex=0; nButtonIndex<nOrigImageCount; nButtonIndex++)
				{
					HICON hIcon=origImageList.ExtractIcon(nButtonIndex);
					ASSERT(hIcon!=NULL);
					VERIFY(m_ilImages.Add(hIcon)!=-1);
					VERIFY(::DestroyIcon(hIcon));
				}

				// Close everything
				origImageList.Detach();
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
				pToolbar->SetButtonInfo(nButtonIndex,nID,nStyle,nImageIndexTest);
			}

			pToolbar->
				SendMessage(TB_SETIMAGELIST,NULL,(LPARAM)((HIMAGELIST)m_ilImages));
		}
	}

	// add custom images
	for(int nButtonIndex=0; 
		nButtonIndex<m_ilCustomImages.GetImageCount(); 
		nButtonIndex++)
	{
		HICON hIcon=m_ilCustomImages.ExtractIcon(nButtonIndex);
		ASSERT(hIcon!=NULL);
		VERIFY(m_ilImages.Add(hIcon)!=-1);
		VERIFY(::DestroyIcon(hIcon));
	}

	// build array of all command IDs
	BuildArrayCommandIDs();
	//
	//////////////////////////////

	// Iterate all the control bars and use only the toolbars
	pos=pFrameWnd->m_listControlBars.GetHeadPosition();
	while(pos!=NULL)
	{

		CToolBar* pToolbar=DYNAMIC_DOWNCAST(CToolBar, 
			(CControlBar*)pFrameWnd->m_listControlBars.GetNext(pos));
		//If it is a CToolBar
		if(pToolbar!=NULL)
		{
			COXCoolToolBar* pCoolToolbar=
				DYNAMIC_DOWNCAST(COXCoolToolBar,pToolbar);
			if(pCoolToolbar!=NULL)
			{
				pCoolToolbar->
					SetBitmapIds(m_arrCommandIds,PtrToInt(m_mapCommands.GetCount()));
				// make sure toolbar is in list mode
				pCoolToolbar->SetList(TRUE);
			}
			VERIFY(LoadToolbarState(pToolbar));

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

				// make sure every button has TBSTYLE_AUTOSIZE style set
				pToolbar->SetButtonInfo(
					nButtonIndex,nID,nStyle|TBSTYLE_AUTOSIZE,nImageIndex);
			}

		}
	}

	if(lpszProfileName!=NULL)
	{
		m_sProfileName=lpszProfileName;
	}

#ifndef _MAC
	if(!m_sProfileName.IsEmpty())
	{
		// retrieve menu options
		m_bShowIconsInMenu=(BOOL)pApp->GetProfileInt(m_sProfileName,
			_T("ShowIconsInMenu"),m_bShowIconsInMenu);
		m_bShowOnlyRecentlyUsedMenuItems=(BOOL)pApp->GetProfileInt(m_sProfileName,
			_T("ShowOnlyRecentlyUsedMenuItems"),m_bShowOnlyRecentlyUsedMenuItems);
	}
#endif

	if(m_bShowIconsInMenu)
	{
		// Let the menu organizer fix our menus
		GetBMOrganizer()->AttachFrameWnd(pFrameWnd);
		// use button from toolbars with corresponding items in menu
		GetBMOrganizer()->AutoSetMenuImage();

		GetBMOrganizer()->
			ShowOnlyRecentlyUsedItems(m_bShowOnlyRecentlyUsedMenuItems);
	}
	VERIFY(GetBMOrganizer()->LoadRUCommandsState(_T("RecentlyUsedMenuItems")));

	return TRUE;
}


void COXCustomizeCommandsPage::DoDataExchange(CDataExchange* pDX)
{
	COXCustomizePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXCustomizeCommandsPage)
	DDX_Control(pDX, IDC_OX_SEPARATOR_DESCRIPTION, m_sepDescription);
	DDX_Control(pDX, IDC_OX_SEPARATOR_COMMANDS, m_sepCommands);
	DDX_Control(pDX, IDC_OX_SEPARATOR_OPTIONS, m_sepOptions);
	DDX_Control(pDX, IDC_OX_BUTTON_MODIFY_SELECTION, m_btnModifySelection);
	DDX_Control(pDX, IDC_OX_TREE_COMMANDS, m_treeCommands);
	DDX_Text(pDX, IDC_OX_STATIC_DESCRIPTION, m_sDescription);
	DDX_Check(pDX, IDC_OX_CHECK_ICONS_IN_MENU, m_bShowIconsInMenu);
	DDX_Check(pDX, IDC_OX_CHECK_SHOW_ONLY_RECENTLY_USED, m_bShowOnlyRecentlyUsedMenuItems);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COXCustomizeCommandsPage, COXCustomizePage)
	//{{AFX_MSG_MAP(COXCustomizeCommandsPage)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_OX_TREE_COMMANDS, OnBegindragTreeCommands)
	ON_NOTIFY(TVN_SELCHANGED, IDC_OX_TREE_COMMANDS, OnSelchangedTreeCommands)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_RANGE(OXCTBN_CUSTOMIZECMD, 0, 0xffff, OnTBCustomizeCommand)
	ON_NOTIFY_RANGE(OXBMN_CUSTOMIZECMD, 0, 0xffff, OnBMCustomizeCommand)
	ON_NOTIFY(OXBBN_DROPDOWN,IDC_OX_BUTTON_MODIFY_SELECTION,OnDropDownModifySelection)
	ON_BN_CLICKED(IDC_OX_CHECK_ICONS_IN_MENU, OnCheckIconsInMenu)
	ON_BN_CLICKED(IDC_OX_CHECK_SHOW_ONLY_RECENTLY_USED, OnCheckShowOnlyRecentlyUsed)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeCommandsPage message handlers

BOOL COXCustomizeCommandsPage::Load(const COXCustomizeManager* pCustomizeManager)
{
	// Call default implementation. It will load this demo dialog as the first 
	// page and will create About and CodeSample pages if specified.
	if(!COXCustomizePage::Load(pCustomizeManager))
		return FALSE;

	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL);
	CFrameWnd* pFrameWnd=DYNAMIC_DOWNCAST(CFrameWnd,pWnd);
	ASSERT(pFrameWnd!=NULL);

	// set all COXCoolToolBar derived toolbars in advanced customization mode
	// Iterate all control bars and use only the toolbars that are derived from 
	// COXCoolToolBar class
	POSITION pos=pFrameWnd->m_listControlBars.GetHeadPosition();
	while(pos!=NULL)
	{
		COXCoolToolBar* pToolbar=DYNAMIC_DOWNCAST(COXCoolToolBar, 
			(CControlBar*)pFrameWnd->m_listControlBars.GetNext(pos));
		//If its a COXCoolToolBar
		if(pToolbar!=NULL)
		{
			pToolbar->SetAdvancedCustomizationMode(TRUE,GetSafeHwnd());
		}
	}

	GetBMOrganizer()->SetCutomizationMode(TRUE,GetSafeHwnd());

	// set current customized toolbar to NULL
	m_pCustomizedToolbar=NULL;
	m_nCustomizedPopupMenuIndex=-1;

	return TRUE;
}


void COXCustomizeCommandsPage::Unload()
{
	// add here code for cleaning up all objects created by demo
	//
	//
	//////////////////////////////////////////////////////////////////////////
	
	// reset all COXCoolToolBar derived toolbars state ( remove advanced 
	// customization mode flag). Iterate all control bars and use only the 
	// toolbars that are derived from COXCoolToolBar class
	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL);
	CFrameWnd* pFrameWnd=DYNAMIC_DOWNCAST(CFrameWnd,pWnd);
	ASSERT(pFrameWnd!=NULL);

	POSITION pos=pFrameWnd->m_listControlBars.GetHeadPosition();
	while(pos!=NULL)
	{
		COXCoolToolBar* pToolbar=DYNAMIC_DOWNCAST(COXCoolToolBar, 
			(CControlBar*)pFrameWnd->m_listControlBars.GetNext(pos));
		//If its a COXCoolToolBar
		if(pToolbar!=NULL)
		{
			pToolbar->SetAdvancedCustomizationMode(FALSE);
		}
	}

	// hide all menus
	ResetPopupMenus(0);
	// set current customized toolbar to NULL
	m_pCustomizedToolbar=NULL;
	m_nCustomizedPopupMenuIndex=-1;

	GetBMOrganizer()->SetCutomizationMode(FALSE);

	COXCustomizePage::Unload();
}

void COXCustomizeCommandsPage::OnInitDialog()
{
#ifndef _MAC
	if(!m_sProfileName.IsEmpty())
	{
		// retrieve menu options
		m_bShowIconsInMenu=(BOOL)::AfxGetApp()->GetProfileInt(m_sProfileName,
			_T("ShowIconsInMenu"),m_bShowIconsInMenu);
		m_bShowOnlyRecentlyUsedMenuItems=(BOOL)::AfxGetApp()->GetProfileInt(m_sProfileName,
			_T("ShowOnlyRecentlyUsedMenuItems"),m_bShowOnlyRecentlyUsedMenuItems);
	}
#endif

	// must call default implementation
	COXCustomizePage::OnInitDialog();

	// add here initialization code for your demo dialog. Treat it as a
	// normal CDialog::OnInitDialog function

	// separators
	COLORREF clr=::GetSysColor(COLOR_WINDOW);
	m_sepDescription.SetTextColor(clr);
	m_sepCommands.SetTextColor(clr);
	m_sepOptions.SetTextColor(clr);

	// bitmap button
	m_btnModifySelection.SetStyleEx(OXBB_EX_DROPDOWN);
	m_btnModifySelection.SetDrawDropdownSeparator(FALSE);
	// set custom text font to button
	CFont* pFont=m_btnModifySelection.GetTextFont();
	ASSERT(pFont!=NULL);
	LOGFONT lf;
	VERIFY(pFont->GetLogFont(&lf));
	lf.lfWeight=FW_BOLD;
	VERIFY(pFont->DeleteObject());
	VERIFY(pFont->CreateFontIndirect(&lf));
	m_btnModifySelection.SetTextFont(pFont);

	// populate tree
	GetVars();

	// layout
	//
	m_LayoutManager.TieChild(&m_treeCommands,OX_LMS_ANY,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_sepCommands,OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_sepDescription,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_sepOptions,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_STATIC_DESCRIPTION,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_btnModifySelection,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_CHECK_ICONS_IN_MENU,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_CHECK_SHOW_ONLY_RECENTLY_USED,
		OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_STATIC_CUSTOM_BUTTON_PREVIEW,
		OX_LMS_LEFT|OX_LMS_RIGHT|OX_LMS_BOTTOM,OX_LMT_SAME);
	//
	///////////////////////////////////////

	m_btnModifySelection.EnableWindow(m_pCustomizedToolbar!=NULL);

	m_bInitialized=TRUE;
}


void COXCustomizeCommandsPage::GetVars()
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

	// add toolbars' custom buttons
	AddCustomButtonsFromToolbars(TVI_ROOT);

	// add custom commands
	AddCustomCommandsToTree();


	// add items for new popup menu and separator
	//
	int nImageIndex=ID_OX_IMAGEOFFSET_NEWPOPUPMENU;
	CString sItem;
	VERIFY(sItem.LoadString(IDS_OX_CSTMZECOMMANDSNEWMENU));//"New Menu"
	HTREEITEM hItem=m_treeCommands.InsertItem(sItem,nImageIndex,
		nImageIndex,TVI_ROOT,TVI_LAST);
	ASSERT(hItem!=NULL);
	m_treeCommands.SetItemState(hItem,TVIS_BOLD,TVIS_BOLD);
	m_treeCommands.SetItemData(hItem,(DWORD)-2);

	nImageIndex=ID_OX_IMAGEOFFSET_SEPARATOR;
	VERIFY(sItem.LoadString(IDS_OX_CSTMZECOMMANDSSEPARATOR));//"Separator"
	hItem=m_treeCommands.InsertItem(sItem,nImageIndex,
		nImageIndex,TVI_ROOT,TVI_LAST);
	ASSERT(hItem!=NULL);
	m_treeCommands.SetItemState(hItem,TVIS_BOLD,TVIS_BOLD);
	m_treeCommands.SetItemData(hItem,0);
	//
	/////////////////////////////////////
}


void COXCustomizeCommandsPage::AddCustomButtonsFromToolbars(HTREEITEM hParentItem)
{
	int nCustomButtonCount=PtrToInt(COXCoolToolBar::m_mapAllCustomButtons.GetCount());
	if(nCustomButtonCount>0)
	{
		CString sItem;
		VERIFY(sItem.LoadString(IDS_OX_CSTMZECOMMANDSCUSTOMBUTTONS));//"Custom Buttons"
		HTREEITEM hCustomButtonsItem=m_treeCommands.InsertItem(sItem,
			ID_OX_IMAGEOFFSET_CUSTOMBUTTON,ID_OX_IMAGEOFFSET_CUSTOMBUTTON,
			hParentItem,TVI_LAST);
		ASSERT(hCustomButtonsItem!=NULL);
		m_treeCommands.SetItemState(hCustomButtonsItem,TVIS_BOLD,TVIS_BOLD);
		m_treeCommands.SetItemData(hCustomButtonsItem,(DWORD)-3);

		POSITION pos=COXCoolToolBar::m_mapAllCustomButtons.GetStartPosition();
		while(pos!=NULL)
		{
			int nCtrlID=-1;
			OXCUSTOMBUTTONDESCRIPTOR descriptor;
			COXCoolToolBar::m_mapAllCustomButtons.GetNextAssoc(pos,nCtrlID,descriptor);
			ASSERT(nCtrlID!=-1);
			if(!::IsWindow(descriptor.m_pCBTemplate->CBGetWindow()->GetSafeHwnd()))
			{
				TRACE(_T("COXCustomizeCommandsPage::AddCustomButtonsFromToolbars: registered custom button with ID %d found that hasn't been inserted in any toolbar. Therefore it is ignored here!\n"),nCtrlID);
				continue;
			}

			CString sText(_T(""));
			HINSTANCE hInstance=
				AfxFindResourceHandle(MAKEINTRESOURCE(nCtrlID),RT_STRING);
			if(hInstance!=NULL)
			{
				sText.LoadString(nCtrlID);
				int nPosition=sText.Find(_T('\n'));
				if(nPosition!=-1)
				{
					sText=sText.Mid(nPosition+1);
				}

				nPosition=sText.Find(_T('\t'));
				if(nPosition!=-1)
					sText=sText.Left(nPosition);
				
				nPosition=sText.Find(_T('&'));
				if(nPosition!=-1)
					sText=sText.Left(nPosition)+sText.Mid(nPosition+1);

			}
			
			if(sText.IsEmpty())
			{
				CString sClassName;
				VERIFY(::GetClassName(
					descriptor.m_pCBTemplate->CBGetWindow()->GetSafeHwnd(),
					sClassName.GetBuffer(MAX_PATH),MAX_PATH)>0);
				sClassName.ReleaseBuffer();
				sText.Format(_T("Window class '%s'"),sClassName);
			}
			ASSERT(!sText.IsEmpty());

			HTREEITEM hItem=m_treeCommands.InsertItem(sText,ID_OX_IMAGEOFFSET_CUSTOMBUTTON,
				ID_OX_IMAGEOFFSET_CUSTOMBUTTON,hCustomButtonsItem,TVI_LAST);
			ASSERT(hItem!=NULL);
			m_treeCommands.SetItemData(hItem,(LPARAM)nCtrlID);
		}
	}
}


void COXCustomizeCommandsPage::RetrieveCommandsFromMenu(HMENU hMenu)
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


void COXCustomizeCommandsPage::AddCommandsFromMenuToTree(HMENU hMenu, 
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
			int nImageIndex=-1;
			if(nCommandID!=-1)
			{
				VERIFY(m_mapCommands.Lookup(nCommandID,nImageIndex));
				if(nImageIndex==-1)
					nImageIndex=ID_OX_IMAGEOFFSET_NOIMAGE;
			}
			else
			{
				nImageIndex=ID_OX_IMAGEOFFSET_MENUGROUP;
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


HTREEITEM COXCustomizeCommandsPage::FindMenuItemInTree(CString sText, 
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


void COXCustomizeCommandsPage::OnBegindragTreeCommands(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView=(NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	m_treeCommands.SelectItem(pNMTreeView->itemNew.hItem);

	CString sText(_T(""));
	int nImageIndex=-1;
	int nCommandID=0;
	BYTE fsStyle=0;
	HMENU hSubmenu=NULL;
	DWORD_PTR dwData=m_treeCommands.GetItemData(pNMTreeView->itemNew.hItem);
	if(dwData==0)
	{
		// separator
		fsStyle=TBSTYLE_SEP;
	}
	else if(dwData==-1)
	{
		// existing popup menu
		sText=m_treeCommands.GetItemText(pNMTreeView->itemNew.hItem);
		nCommandID=-1;
		fsStyle=TBSTYLE_AUTOSIZE|TBSTYLE_DROPDOWN;
		hSubmenu=BuildMenuFromTree(pNMTreeView->itemNew.hItem);
	}
	else if(dwData==-2)
	{
		// new popup menu
		VERIFY(sText.LoadString(IDS_OX_CSTMZECOMMANDSNEWMENU)); //"New menu"
		nCommandID=-1;
		fsStyle=TBSTYLE_AUTOSIZE|TBSTYLE_DROPDOWN;
	}
	else if(dwData==-3)
	{
		// set of toolbars' custom buttons
		// doesn't support drag'n'drop
		return;
	}
	else 
	{
		nCommandID=(int)dwData;
		int nSelectedImage=0;
		VERIFY(m_treeCommands.GetItemImage(pNMTreeView->itemNew.hItem,
			nImageIndex,nSelectedImage));
		ASSERT(nSelectedImage==nImageIndex);

		HINSTANCE hInstance=
			AfxFindResourceHandle(MAKEINTRESOURCE(nCommandID),RT_STRING);
		if(hInstance!=NULL)
		{
			sText.LoadString(nCommandID);
			int nPosition=sText.Find(_T('\n'));
			if(nPosition!=-1)
				sText=sText.Mid(nPosition+1);
		}
		if(sText.IsEmpty())
		{
			sText=m_treeCommands.GetItemText(pNMTreeView->itemNew.hItem);
		}
		ASSERT(!sText.IsEmpty());
	}

	COleDataSource* pDataSource=
		COXDragDropCommands::PrepareDragDropData(sText,nImageIndex,
		nCommandID,fsStyle,hSubmenu);
	ASSERT(pDataSource!=NULL);
	COleDropSource dropSource;
	COXDragDropCommands::DoDragDrop(pDataSource,&dropSource);

	//delete drag source (we are responsible for doing that)
	delete pDataSource;

	if(hSubmenu!=NULL)
	{
		::DestroyMenu(hSubmenu);
	}

	*pResult=0;
}


void COXCustomizeCommandsPage::OnSelchangedTreeCommands(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView=(NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	// destroy currently displayed custom button window
	if(m_hCustomButtonWnd!=NULL)
	{
		VERIFY(::DestroyWindow(m_hCustomButtonWnd));
		m_hCustomButtonWnd=NULL;
	}

	HTREEITEM hSelectedItem=pNMTreeView->itemNew.hItem;
	if(hSelectedItem!=NULL)
	{
		DWORD_PTR dwData=m_treeCommands.GetItemData(hSelectedItem);
		if(dwData==0)
		{
			// separator
			VERIFY(m_sDescription.LoadString(IDS_OX_CSTMZECOMMANDSDESCSEPARATOR)); //"Menu or toolbar separator"
		}
		else if(dwData==-1)
		{
			// existing popup menu
			m_sDescription.Format(IDS_OX_CSTMZECOMMANDSDESCGROUP,//"%s group"
				m_treeCommands.GetItemText(hSelectedItem));
		}
		else if(dwData==-2)
		{
			// new popup menu
			VERIFY(m_sDescription.LoadString(IDS_OX_CSTMZECOMMANDSDESCNEWGROUP)); //"New menu group"
		}
		else if(dwData==-3)
		{
			// set of custom buttons
			VERIFY(m_sDescription.LoadString(IDS_OX_CSTMZECOMMANDSDESCSET)) ;//"Set of toolbars' Custom Buttons"
		}
		else 
		{
			UINT nCommandID=(UINT)dwData;
			HINSTANCE hInstance=
				AfxFindResourceHandle(MAKEINTRESOURCE(nCommandID),RT_STRING);
			if(hInstance==NULL)
			{
				m_sDescription.Empty();
			}
			else
			{
				m_sDescription.LoadString(nCommandID);
				int nPosition=m_sDescription.Find(_T('\n'));
				if(nPosition!=-1)
					m_sDescription=m_sDescription.Left(nPosition);
			}

			// check for toollbar custom button
			OXCUSTOMBUTTONDESCRIPTOR descriptor;
			if(COXCoolToolBar::m_mapAllCustomButtons.Lookup(nCommandID,descriptor))
			{
				if(m_sDescription.IsEmpty())
				{
					VERIFY(m_sDescription.LoadString(IDS_OX_CSTMZECOMMANDSCUSTOMBUTTON)); //"Custom Button"
				}

				// create custom button window and display it
				//
				COXCustomTBButtonTemplate* pCBTemplate=
					descriptor.m_pCBTemplate->CBCreateCopy(this);
				ASSERT(pCBTemplate!=NULL);
				ASSERT(::IsWindow(pCBTemplate->CBGetWindow()->GetSafeHwnd()));
				pCBTemplate->CBSetCustomizationMode(TRUE);
				m_hCustomButtonWnd=pCBTemplate->CBGetWindow()->GetSafeHwnd();
				PreviewCustomButtonWindow();
				//
				//////////////////////////////////////////////
			}
		}
	}
	else
	{
		m_sDescription.Empty();
	}

	UpdateData(FALSE);
	
	*pResult=0;
}


void COXCustomizeCommandsPage::OnSize(UINT nType, int cx, int cy)
{
	COXCustomizePage::OnSize(nType,cx,cy);

	// reposition the custom button preview window if any displayed
	if(::IsWindow(m_hCustomButtonWnd))
	{
		PreviewCustomButtonWindow();
	}
}


void COXCustomizeCommandsPage::PreviewCustomButtonWindow()
{
	if(m_hCustomButtonWnd==NULL)
		return;

	ASSERT(::IsWindow(m_hCustomButtonWnd));

	CWnd* pCBWnd=CWnd::FromHandle(m_hCustomButtonWnd);
	// custom button descriptor
	OXCUSTOMBUTTONDESCRIPTOR descriptor;
	VERIFY(COXCoolToolBar::m_mapAllCustomButtons.
		Lookup(pCBWnd->GetDlgCtrlID(),descriptor));

	CRect rect;
	GetDlgItem(IDC_OX_STATIC_CUSTOM_BUTTON_PREVIEW)->GetClientRect(rect);
	GetDlgItem(IDC_OX_STATIC_CUSTOM_BUTTON_PREVIEW)->ClientToScreen(rect);
	// deflate it by some reasonable margin
	rect.DeflateRect(5,17,5,5);

	// based on descriptor info and wrapping rectangle define 
	// the rectangle for custom button window
	int nCBHeight=descriptor.m_nHeight;
	if(nCBHeight>ID_OXCUSTOMIZE_TBIMAGE_HEIGHT+6)
	{
		nCBHeight=ID_OXCUSTOMIZE_TBIMAGE_HEIGHT+6;
	}
	if(nCBHeight>rect.Height())
	{
		nCBHeight=rect.Height();
	}
	else
	{
		// center the custom button window vertically
		rect.top+=(rect.Height()-nCBHeight)/2;
		rect.bottom=rect.top+nCBHeight;
	}
	int nCBWidth=descriptor.m_nWidth;
	if(nCBWidth>rect.Width())
	{
		nCBWidth=rect.Width();
	}
	else
	{
		// center the custom button window horizontally
		rect.left+=(rect.Width()-nCBWidth)/2;
		rect.right=rect.left+nCBWidth;
	}
	ScreenToClient(rect);

	pCBWnd->MoveWindow(rect);
	pCBWnd->ShowWindow(SW_SHOWNA);
}


BOOL COXCustomizeCommandsPage::SaveToolbarState(CToolBar* pToolbar)
{
	ASSERT(pToolbar!=NULL);

	// save the state into the registry
	BOOL bResult=TRUE;
	CString sRegistryKey;
	sRegistryKey.Format(_T("Toolbar_ID_%d"),pToolbar->GetDlgCtrlID());
	if(pToolbar->IsKindOf(RUNTIME_CLASS(COXCoolToolBar)))
	{
		COXCoolToolBar* pCoolToolbar=
			DYNAMIC_DOWNCAST(COXCoolToolBar,pToolbar);
		ASSERT(pCoolToolbar!=NULL);
		bResult=pCoolToolbar->SaveBarState(_T("Toolbars State"),sRegistryKey,TRUE);
	}
	else
	{
#ifndef _MAC
		CWinApp* pApp=AfxGetApp();
		ASSERT(pApp!=NULL);
		// make sure you called CWinApp::SetRegistryKey() functions before
		if(pApp->m_pszRegistryKey!=NULL && pApp->m_pszProfileName!=NULL)
		{
			// we use default registry key assigned to your application by MFC
			HKEY hSecKey=pApp->GetSectionKey(_T(""));
			if(hSecKey!=NULL)
			{
				pToolbar->GetToolBarCtrl().SaveState(hSecKey,
					_T("Toolbars State"),sRegistryKey);
				::RegCloseKey(hSecKey);
			}
			else
			{
				TRACE(_T("COXCustomizeToolbarsPage::SaveToolbarState: unable to get the section key\n"));
				return FALSE;
			}
		}
		else
		{
			TRACE(_T("COXCustomizeToolbarsPage::SaveToolbarState: failed to save the toolbars state, SetRegistryKey() function hasn't been called\n"));
			return FALSE;
		}
#else
		return FALSE;
#endif
	}

	return bResult;
}

BOOL COXCustomizeCommandsPage::LoadToolbarState(CToolBar* pToolbar)
{
	ASSERT(pToolbar!=NULL);

	BOOL bResult=TRUE;
	CString sRegistryKey;
	sRegistryKey.Format(_T("Toolbar_ID_%d"),pToolbar->GetDlgCtrlID());
	if(pToolbar->IsKindOf(RUNTIME_CLASS(COXCoolToolBar)))
	{
		COXCoolToolBar* pCoolToolbar=DYNAMIC_DOWNCAST(COXCoolToolBar,pToolbar);
		ASSERT(pCoolToolbar!=NULL);
		bResult=pCoolToolbar->LoadBarState(_T("Toolbars State"),sRegistryKey,TRUE);
	}
	else
	{
#ifndef _MAC
		CWinApp* pApp=AfxGetApp();
		ASSERT(pApp!=NULL);
		// make sure you called CWinApp::SetRegistryKey() functions before
		if(pApp->m_pszRegistryKey==NULL || pApp->m_pszProfileName==NULL)
		{
			TRACE(_T("COXCustomizeToolbarsPage::LoadToolbarState: failed to restore the toolbars state, SetRegistryKey() function hasn't been called\n"));
			return FALSE;
		}
		// we use default registry key assigned to your application by MFC
		HKEY hSecKey=pApp->GetSectionKey(_T(""));
		if (hSecKey==NULL)
		{
			TRACE(_T("COXCustomizeToolbarsPage::LoadToolbarState: unable to get the section key\n"));
			return FALSE;
		}
		pToolbar->GetToolBarCtrl().RestoreState(hSecKey,
			_T("Toolbars State"),sRegistryKey);
		::RegCloseKey(hSecKey);
#endif
	}

	return bResult;
}


BOOL COXCustomizeCommandsPage::SaveWorkspaceState()
{
	if(!COXCustomizePage::SaveWorkspaceState())
		return FALSE;

	// save info about recently used menu commands
	VERIFY(GetBMOrganizer()->SaveRUCommandsState(_T("RecentlyUsedMenuItems")));
	GetBMOrganizer()->Empty();


	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL);
	CFrameWnd* pFrameWnd=DYNAMIC_DOWNCAST(CFrameWnd,pWnd);
	ASSERT(pFrameWnd!=NULL);

	// Iterate all the control bars and use only the toolbars
	POSITION pos=pFrameWnd->m_listControlBars.GetHeadPosition();
	while(pos!=NULL)
	{
		CToolBar* pToolbar=DYNAMIC_DOWNCAST(CToolBar, 
			(CControlBar*)pFrameWnd->m_listControlBars.GetNext(pos));
		//If its a CToolBar
		if(pToolbar!=NULL)
		{
			// save the state into the registry
			VERIFY(SaveToolbarState(pToolbar));
		}
	}

	return TRUE;
}


void COXCustomizeCommandsPage::OnTBCustomizeCommand(UINT nID, NMHDR* pNMHDR, 
													LRESULT* pResult)
{
	UNUSED(nID);

	*pResult=0;
	
	NMCTBCUSTOMIZE* pNMCTBCustomize=(NMCTBCUSTOMIZE*)pNMHDR;

	ASSERT(::GetDlgCtrlID(pNMCTBCustomize->nmhdr.hwndFrom)==(int)nID);

	CWnd* pWnd=CWnd::FromHandlePermanent(pNMCTBCustomize->nmhdr.hwndFrom);
	ASSERT(pWnd!=NULL);
	COXCoolToolBar* pCoolToolBar=DYNAMIC_DOWNCAST(COXCoolToolBar,pWnd);
	ASSERT(pCoolToolBar!=NULL);
	ASSERT(pCoolToolBar->IsInAdvancedCustomizationMode());
	int nCustomizedButtonIndex=pCoolToolBar->GetCustomizedButton();

	COXMenuBar* pMenuBar=DYNAMIC_DOWNCAST(COXMenuBar,pCoolToolBar);
	if(pMenuBar!=NULL)
	{
		ResetPopupMenus(0);
	}

	switch(pNMCTBCustomize->nCustomizeEventID)
	{
	case ID_OXCUSTTB_APPEARANCE:
		{
			ASSERT(nCustomizedButtonIndex!=-1);
			UINT nButtonCmdID=pCoolToolBar->GetItemID(nCustomizedButtonIndex);

			COXButtonAppearanceDlg dlgAppearance;
			BOOL bCustomButton=pCoolToolBar->IsCustomButton(nCustomizedButtonIndex);
			dlgAppearance.SetCustomButton(bCustomButton);
			
			OXCUSTOMBUTTONDESCRIPTOR descriptor;
			int nImageIndex=-1;
			TBBUTTON button;
			VERIFY(pCoolToolBar->
				GetToolBarCtrl().GetButton(nCustomizedButtonIndex,&button));
			CImageList imageList;
			if(!pCoolToolBar->IsKindOf(RUNTIME_CLASS(COXMenuBar)))
			{
				if(!bCustomButton)
				{
					nImageIndex=button.iBitmap;
				}
				else
				{
					VERIFY(pCoolToolBar->
						GetCustomButton(nCustomizedButtonIndex,descriptor));
					nImageIndex=descriptor.m_nImageIndex;
				}
				ASSERT(nImageIndex!=-1);
				VERIFY(imageList.Create(ID_OXCUSTOMIZE_TBIMAGE_WIDTH,
					ID_OXCUSTOMIZE_TBIMAGE_HEIGHT,ILC_COLOR24|ILC_MASK,0,0));
				// first of all add the current button image
				HICON hIcon=m_ilImages.ExtractIcon(nImageIndex);
				ASSERT(hIcon!=NULL);
				VERIFY(imageList.Add(hIcon)!=-1);
				VERIFY(::DestroyIcon(hIcon));
				// then add all images
				for(int nButtonIndex=ID_OX_IMAGEOFFSET_COUNT; 
					nButtonIndex<m_ilImages.GetImageCount(); 
					nButtonIndex++)
				{
					HICON hIcon=m_ilImages.ExtractIcon(nButtonIndex);
					ASSERT(hIcon!=NULL);
					VERIFY(imageList.Add(hIcon)!=-1);
					VERIFY(::DestroyIcon(hIcon));
				}
				// set the image list to be displayed in the dialog
				dlgAppearance.SetImageList(&imageList);
				dlgAppearance.m_nCustomImageIndex=0;

				if(!bCustomButton)
				{
					// set dropdown flag
					dlgAppearance.m_bDropDown=
						((button.fsStyle&TBSTYLE_DROPDOWN)==TBSTYLE_DROPDOWN);
					
					// get button text (or recent used text or text assigned to the
					// corresponding command ID)
					CString sText=pCoolToolBar->GetButtonText(nCustomizedButtonIndex);
					dlgAppearance.m_nButtonType=(sText.IsEmpty() ? 0 : 1);
					if(sText.IsEmpty())
					{
						sText=pCoolToolBar->GetRecentButtonText(nButtonCmdID);
						if(sText.IsEmpty())
						{
							HINSTANCE hInstance=
								AfxFindResourceHandle(MAKEINTRESOURCE(nButtonCmdID),
								RT_STRING);
							if(hInstance!=NULL)
							{
								sText.LoadString(nButtonCmdID);
								int nPosition=sText.Find(_T('\n'));
								if(nPosition!=-1)
									sText=sText.Mid(nPosition+1);
							}
						}
					}
					dlgAppearance.m_sText=sText;
				}
				else
				{
					dlgAppearance.m_nHeight=descriptor.m_nHeight;
					dlgAppearance.m_nWidth=descriptor.m_nWidth;
					dlgAppearance.m_bDisplayFloat=
						((descriptor.m_nFlags&OXCBD_SHOWFLOAT)==OXCBD_SHOWFLOAT);
					dlgAppearance.m_bDisplayHorzDock=
						((descriptor.m_nFlags&OXCBD_SHOWHORZ)==OXCBD_SHOWHORZ);
					dlgAppearance.m_bDisplayVertDock=
						((descriptor.m_nFlags&OXCBD_SHOWVERT)==OXCBD_SHOWVERT);
				}
			}
			else
			{
				ASSERT(!bCustomButton);
				HMENU hMenu=pMenuBar->GetMenu();
				ASSERT(hMenu!=NULL);
				MENUITEMINFO menuItemInfo={ sizeof(MENUITEMINFO) };
				menuItemInfo.fMask=MIIM_TYPE;
				menuItemInfo.cch=256;
				CString sText;
				menuItemInfo.dwTypeData=sText.GetBuffer(menuItemInfo.cch);
				VERIFY(::GetMenuItemInfo(hMenu,nCustomizedButtonIndex,TRUE,
					&menuItemInfo));
				sText.ReleaseBuffer();
				ASSERT(!sText.IsEmpty());
				dlgAppearance.m_sText=sText;

				dlgAppearance.m_nButtonType=-1;

				dlgAppearance.SetModifyTextOnly(TRUE);
				dlgAppearance.SetStyleReadOnly(TRUE);
			}

			if(dlgAppearance.DoModal()==IDOK)
			{
				if(pMenuBar==NULL)
				{
					if(!bCustomButton)
					{
						BOOL bRedraw=FALSE;

						CString sText=
							pCoolToolBar->GetButtonText(nCustomizedButtonIndex);
						if(dlgAppearance.m_nButtonType==0 && !sText.IsEmpty())
						{
							VERIFY(pCoolToolBar->
								SetButtonText(nCustomizedButtonIndex,_T("")));
							bRedraw=TRUE;
						}
						else if(dlgAppearance.m_nButtonType==1)
						{
							VERIFY(pCoolToolBar->SetButtonText(nCustomizedButtonIndex,
								dlgAppearance.m_sText));
							bRedraw=TRUE;
						}

						BOOL bChangeButtonInfo=FALSE;
						if(((button.fsStyle&TBSTYLE_DROPDOWN)==TBSTYLE_DROPDOWN)!=
							dlgAppearance.m_bDropDown)
						{
							if(dlgAppearance.m_bDropDown)
							{
								button.fsStyle|=TBSTYLE_DROPDOWN;
							}
							else
							{
								button.fsStyle&=~TBSTYLE_DROPDOWN;
							}
							bChangeButtonInfo=TRUE;
						}
						if(dlgAppearance.m_nCustomImageIndex!=0 &&
							dlgAppearance.m_nCustomImageIndex!=nImageIndex+1)
						{
							button.iBitmap=dlgAppearance.m_nCustomImageIndex-1+
								ID_OX_IMAGEOFFSET_COUNT;
							bChangeButtonInfo=TRUE;
						}
						if(bChangeButtonInfo)
						{
							pCoolToolBar->SetButtonInfo(nCustomizedButtonIndex,
								button.idCommand,button.fsStyle,button.iBitmap);
							bRedraw=TRUE;
						}

						if(bRedraw)
						{
							pCoolToolBar->RedrawToolBar(TRUE,TRUE);
						}
					}
					else
					{
						int nFlags=
							(dlgAppearance.m_bDisplayFloat ? OXCBD_SHOWFLOAT : 0) |
							(dlgAppearance.m_bDisplayHorzDock ? OXCBD_SHOWHORZ : 0) |
							(dlgAppearance.m_bDisplayVertDock ? OXCBD_SHOWVERT : 0);
						if(descriptor.m_nFlags!=nFlags || 
							descriptor.m_nHeight!=dlgAppearance.m_nHeight || 
							descriptor.m_nWidth!=dlgAppearance.m_nWidth ||
							(dlgAppearance.m_nCustomImageIndex!=0 &&
							dlgAppearance.m_nCustomImageIndex!=nImageIndex+1))
						{
							descriptor.m_nFlags=nFlags;
							descriptor.m_nHeight=dlgAppearance.m_nHeight;
							descriptor.m_nWidth=dlgAppearance.m_nWidth;
							if(dlgAppearance.m_nCustomImageIndex!=0)
							{
								descriptor.m_nImageIndex=
									dlgAppearance.m_nCustomImageIndex-1;
							}

							int nCustomButtonIndex=
								pCoolToolBar->GetCustomButtonIndex(descriptor.m_nID);
							ASSERT(nCustomButtonIndex!=-1);
							VERIFY(pCoolToolBar->
								SetCustomButton(nCustomButtonIndex,descriptor));
						}
					}
				}
				else
				{
					CString sText=pCoolToolBar->GetButtonText(nCustomizedButtonIndex);
					if(!dlgAppearance.m_sText.IsEmpty() && 
						sText.Compare(dlgAppearance.m_sText)!=0)
					{
						HMENU hMenu=pMenuBar->GetMenu();
						ASSERT(hMenu!=NULL);
						MENUITEMINFO menuItemInfo={ sizeof(MENUITEMINFO) };
						menuItemInfo.fMask=MIIM_TYPE;
						menuItemInfo.cch=256;
						CString sText;
						menuItemInfo.dwTypeData=sText.GetBuffer(menuItemInfo.cch);
						VERIFY(::GetMenuItemInfo(hMenu,nCustomizedButtonIndex,TRUE,
							&menuItemInfo));
						sText.ReleaseBuffer();
						sText=dlgAppearance.m_sText;
						menuItemInfo.cch=sText.GetLength();
						menuItemInfo.dwTypeData=sText.GetBuffer(menuItemInfo.cch);
						VERIFY(::SetMenuItemInfo(hMenu,nCustomizedButtonIndex,TRUE,
							&menuItemInfo));
						sText.ReleaseBuffer();
						pMenuBar->DrawMenuBar();
						pMenuBar->MarkAsChanged();
					}
				}
			}
	
			*pResult=1;
			break;
		}

	case ID_OXCUSTTB_SET_CUSTOMIZE_BUTTON:
		{
			if(nCustomizedButtonIndex==-1 && m_pCustomizedToolbar!=NULL)
			{
				if(pCoolToolBar!=m_pCustomizedToolbar)
					m_pCustomizedToolbar->SetCustomizedButton(-1);
				
				m_pCustomizedToolbar=NULL;
			}
			else if(pCoolToolBar!=m_pCustomizedToolbar && 
				nCustomizedButtonIndex!=-1)
			{
				if(m_pCustomizedToolbar!=NULL)
					m_pCustomizedToolbar->SetCustomizedButton(-1);
				m_pCustomizedToolbar=pCoolToolBar;
			}
			m_btnModifySelection.EnableWindow(m_pCustomizedToolbar!=NULL);
			break;
		}

	case ID_OXCUSTMB_CUSTOMIZE_POPUP_MENU:
		{
			if(nCustomizedButtonIndex!=-1 && GetBMOrganizer()->IsAttached())
			{
				ASSERT(pMenuBar!=NULL);

				HMENU hMenu=pMenuBar->GetMenu();
				ASSERT(hMenu!=NULL);
				CMenu* pMenu=CMenu::FromHandle(hMenu);
				ASSERT(pMenu!=NULL);
				ASSERT(nCustomizedButtonIndex>=0 && 
					nCustomizedButtonIndex<(int)pMenu->GetMenuItemCount());
				CMenu* pPopupMenu=pMenu->GetSubMenu(nCustomizedButtonIndex);

				OXCUSTOMIZEPOPUPMENUINFO* pCPMI=
					(OXCUSTOMIZEPOPUPMENUINFO*)pNMCTBCustomize->lParam;
				ASSERT(pCPMI!=NULL);
				ASSERT(pPopupMenu->GetSafeHmenu()==pCPMI->hMenu);

				if(m_nCustomizedPopupMenuIndex!=nCustomizedButtonIndex)
				{
					COXBitmapMenuPopupWnd* pBMPopupWnd=GetFirstFreeBMPopupWnd(0);
					ASSERT(pBMPopupWnd!=NULL);
					m_nCustomizedPopupMenuIndex=nCustomizedButtonIndex;

					COXBitmapMenu* pBMSubmenu=
						DYNAMIC_DOWNCAST(COXBitmapMenu,pPopupMenu);
					if(pBMSubmenu==NULL)
					{
						HMENU hMenu=pPopupMenu->GetSafeHmenu();
						GetBMOrganizer()->OnInitMenuPopup(pPopupMenu,0,FALSE);
						pPopupMenu=CMenu::FromHandle(hMenu);
						pBMSubmenu=DYNAMIC_DOWNCAST(COXBitmapMenu,pPopupMenu);
						ASSERT(pBMSubmenu!=NULL);
					}

					pBMPopupWnd->TrackPopupMenu(pBMSubmenu,
						pCPMI->nFlags&(TPM_CENTERALIGN|TPM_LEFTALIGN|TPM_RIGHTALIGN),
						pCPMI->ptStart.x,pCPMI->ptStart.y,this);
				}
				else
				{
					m_nCustomizedPopupMenuIndex=-1;
				}
			}
			break;
		}

	case ID_OXCUSTTB_DELETE:
		{
			if(pMenuBar!=NULL && m_nCustomizedPopupMenuIndex==nCustomizedButtonIndex)
			{
				m_nCustomizedPopupMenuIndex=-1;
			}
			break;
		}
	}
}


void COXCustomizeCommandsPage::OnBMCustomizeCommand(UINT nID, NMHDR* pNMHDR, 
													LRESULT* pResult)
{
	UNUSED(nID);

	*pResult=0;
	
	NMBMCUSTOMIZE* pNMBMCustomize=(NMBMCUSTOMIZE*)pNMHDR;

	ASSERT(::GetDlgCtrlID(pNMBMCustomize->nmhdr.hwndFrom)==(int)nID);

	CWnd* pWnd=CWnd::FromHandlePermanent(pNMBMCustomize->nmhdr.hwndFrom);
	ASSERT(pWnd!=NULL);
	COXBitmapMenuPopupWnd* pPopupWnd=DYNAMIC_DOWNCAST(COXBitmapMenuPopupWnd,pWnd);
	ASSERT(pPopupWnd!=NULL);
	COXBitmapMenu* pBitmapMenu=pPopupWnd->GetBitmapMenu();
	ASSERT(pBitmapMenu!=NULL);
	int nCustomizedItemIndex=pBitmapMenu->GetCustomizedItem();

	if(pNMBMCustomize->nCustomizeEventID!=ID_OXCUSTBM_SET_CUSTOMIZE_ITEM && 
		pNMBMCustomize->nCustomizeEventID!=ID_OXCUSTBM_SET_INSERT_MARK)
	{
		int nLevel=GetBMPopupWndLevel(pPopupWnd);
		ASSERT(nLevel>=0 || nLevel==-1);
		if(nLevel>=0)
		{
			ResetPopupMenus(nLevel+1);
		}
	}

	switch(pNMBMCustomize->nCustomizeEventID)
	{
	case ID_OXCUSTBM_APPEARANCE:
		{
			ASSERT(nCustomizedItemIndex!=-1);

			MENUITEMINFO mii={ sizeof(MENUITEMINFO) };
			mii.fMask=MIIM_DATA|MIIM_TYPE;
			::GetMenuItemInfo(pBitmapMenu->GetSafeHmenu(),
				nCustomizedItemIndex,TRUE,&mii);

			if(mii.fType & MFT_SEPARATOR)
				break;

			COXItemInfo* pItemInfo=(COXItemInfo*)(mii.dwItemData);
			CString sText=pItemInfo->GetText();
			ASSERT(!sText.IsEmpty());
			COXImageInfo* pImageInfo=pItemInfo->GetImageInfo();

			COXButtonAppearanceDlg dlgAppearance;
			
			CImageList imageList;
			VERIFY(imageList.Create(ID_OXCUSTOMIZE_TBIMAGE_WIDTH,
				ID_OXCUSTOMIZE_TBIMAGE_HEIGHT,ILC_COLOR24|ILC_MASK,0,0));
			// first of all add the current button image
			HICON hIcon=pImageInfo->GetImageList()->
				ExtractIcon(pImageInfo->GetIndex());
			ASSERT(hIcon!=NULL);
			VERIFY(imageList.Add(hIcon)!=-1);
			VERIFY(::DestroyIcon(hIcon));
			// then add all images
			for(int nItemIndex=ID_OX_IMAGEOFFSET_COUNT; 
				nItemIndex<m_ilImages.GetImageCount(); 
				nItemIndex++)
			{
				HICON hIcon=m_ilImages.ExtractIcon(nItemIndex);
				ASSERT(hIcon!=NULL);
				VERIFY(imageList.Add(hIcon)!=-1);
				VERIFY(::DestroyIcon(hIcon));
			}
			// set the image list to be displayed in the dialog
			dlgAppearance.SetImageList(&imageList);
			dlgAppearance.m_nCustomImageIndex=0;

			// set dropdown flag
			dlgAppearance.m_bDropDown=FALSE;

			CString sShortkeyText(_T(""));
			// only for commands try to retrieve accelerator key strings
			if((int)pBitmapMenu->GetMenuItemID(nCustomizedItemIndex)>0)
			{
				int nShortkeyTextPos=sText.Find(_T('\t'));
				if(nShortkeyTextPos!=-1)
				{
					sShortkeyText=sText.Mid(nShortkeyTextPos+1);
					sText=sText.Left(nShortkeyTextPos);
				}
				dlgAppearance.SetShortkeyReadOnly(FALSE);
#ifdef OX_CUSTOMIZE_SHORTKEYS
				BOOL bFoundShortkey=FALSE;
				COXShortkeysOrganizer* pShortkeysOrganizer=
					m_pCustomizeManager->GetShortkeysOrganizer();
				if(pShortkeysOrganizer!=NULL)
				{
					CMultiDocTemplate* pDocTemplate=NULL;
					if(m_pCustomizeManager->IsMDIFramework())
					{
						CMDIFrameWnd* pFrameWnd=
							DYNAMIC_DOWNCAST(CMDIFrameWnd,AfxGetMainWnd());
						ASSERT(pFrameWnd!=NULL);
						CMDIChildWnd* pChildWnd=pFrameWnd->MDIGetActive();
						if(pChildWnd!=NULL)
						{
							CDocument* pDocument=pChildWnd->GetActiveDocument();
							if(pDocument!=NULL)
							{
								pDocTemplate=DYNAMIC_DOWNCAST(CMultiDocTemplate,
									pDocument->GetDocTemplate());
							}
						}
					}
					COXArrAccelerators arrAccels;
					pShortkeysOrganizer->FindAllAccelerators(
						(WORD)pBitmapMenu->GetMenuItemID(nCustomizedItemIndex),
						pDocTemplate,arrAccels);
					for(int nIndex=0; nIndex<arrAccels.GetSize(); nIndex++)
					{
						ACCEL accel=arrAccels[nIndex];
						CString sNewShortkeyText=
							COXShortkeysOrganizer::GetAccelKeyString(&accel);
						if(!bFoundShortkey && 
							sShortkeyText.CompareNoCase(sNewShortkeyText)==0)
						{
							bFoundShortkey=TRUE;
							dlgAppearance.m_nShortkeyIndex=
   								PtrToInt(dlgAppearance.m_arrShortkeyEntries.Add(sNewShortkeyText));
						}
						else
						{
							dlgAppearance.m_arrShortkeyEntries.Add(sNewShortkeyText);
						}
					}
				}
				if(!bFoundShortkey)
#endif	//	OX_CUSTOMIZE_SHORTKEYS
				{
					if(!sShortkeyText.IsEmpty())
					{
						dlgAppearance.m_nShortkeyIndex=
							PtrToInt(dlgAppearance.m_arrShortkeyEntries.Add(sShortkeyText));
					}
				}
			}
		
			dlgAppearance.m_sText=sText;
			dlgAppearance.SetStyleReadOnly(TRUE);
			dlgAppearance.SetModifyTextOnly(TRUE);

			if(dlgAppearance.DoModal()==IDOK)
			{
				CString sNewShortkeyText=dlgAppearance.GetShortkeyText();
				if(!dlgAppearance.m_sText.IsEmpty() && 
					(sText.Compare(dlgAppearance.m_sText)!=0 || 
					sShortkeyText.Compare(sNewShortkeyText)!=0))
				{
					if(sNewShortkeyText.IsEmpty())
					{
						pItemInfo->SetText(dlgAppearance.m_sText);
					}
					else
					{
						pItemInfo->SetText(dlgAppearance.m_sText+_T("\t")+
							sNewShortkeyText);
					}
					pPopupWnd->OnMenuChanged();

					COXMenuBar* pMenuBar=COXMenuBar::FindMenuBar();
					if(pMenuBar!=NULL)
					{
						pMenuBar->MarkAsChanged();
					}
				}
			}
	
			*pResult=1;
			break;
		}

	case ID_OXCUSTBM_SET_CUSTOMIZE_ITEM:
		{
			if(nCustomizedItemIndex!=-1 && 
				pBitmapMenu->GetMenuItemID(nCustomizedItemIndex)==-1)
			{
				CMenu* pSubmenu=pBitmapMenu->GetSubMenu(nCustomizedItemIndex);
				COXBitmapMenu* pBMSubmenu=DYNAMIC_DOWNCAST(COXBitmapMenu,pSubmenu);
				BOOL bDisplaySubmenu=(pBMSubmenu==NULL || 
					pBMSubmenu->GetPopupWnd()==NULL ||
					!::IsWindow(pBMSubmenu->GetPopupWnd()->GetSafeHwnd()) ||
					!pBMSubmenu->GetPopupWnd()->IsWindowVisible());
				// hide all existing submenus on this level
				int nLevel=GetBMPopupWndLevel(pPopupWnd);
				ASSERT(nLevel>=0 || nLevel==-1);
				if(nLevel>=0)
				{
					ResetPopupMenus(nLevel+1);
				}
				// display new submenu
				if(bDisplaySubmenu)
				{
					// submenu item has been marked for customization. As a result
					// child popup menu must be displayed
					COXBitmapMenuPopupWnd* pBMPopupWnd=
						(pBMSubmenu==NULL ? NULL : pBMSubmenu->GetPopupWnd());
					if(pBMPopupWnd==NULL)
					{
						pBMPopupWnd=GetFirstFreeBMPopupWnd(nLevel+1);
					}
					ASSERT(pBMPopupWnd!=NULL);
					CRect rect=pPopupWnd->GetItemRect(nCustomizedItemIndex);
					pPopupWnd->ClientToScreen(rect);
					pPopupWnd->CalcWindowRect(rect);
					CRect rectWindow;
					pPopupWnd->GetWindowRect(rectWindow);
					
					if(pBMSubmenu==NULL)
					{
						HMENU hMenu=pSubmenu->GetSafeHmenu();
						GetBMOrganizer()->OnInitMenuPopup(pSubmenu,0,FALSE);
						pSubmenu=CMenu::FromHandle(hMenu);
						pBMSubmenu=DYNAMIC_DOWNCAST(COXBitmapMenu,pSubmenu);
						ASSERT(pBMSubmenu!=NULL);
					}

					pBMPopupWnd->TrackPopupMenu(pBMSubmenu,TPM_LEFTALIGN,
						rectWindow.right,rect.top,this);
				}
			}
			else
			{
				// hide all existing submenus on this level
				int nLevel=GetBMPopupWndLevel(pPopupWnd);
				ASSERT(nLevel>=0 || nLevel==-1);
				if(nLevel>=0)
				{
					ResetPopupMenus(nLevel+1);
				}
			}
			break;
		}

	case ID_OXCUSTBM_SET_INSERT_MARK:
		{
			CPoint ptMouse;
			::GetCursorPos(&ptMouse);
			pPopupWnd->ScreenToClient(&ptMouse);
			BOOL bBefore=TRUE;
			int nInsertMarkIndex=pBitmapMenu->HitTest(&ptMouse,&bBefore);
			if(nInsertMarkIndex!=-1 && 
				pBitmapMenu->GetMenuItemID(nInsertMarkIndex)==-1 &&
				pBitmapMenu->GetDraggedItem()!=nInsertMarkIndex)
			{
				CMenu* pSubmenu=pBitmapMenu->GetSubMenu(nInsertMarkIndex);
				COXBitmapMenu* pBMSubmenu=DYNAMIC_DOWNCAST(COXBitmapMenu,pSubmenu);
				BOOL bDisplaySubmenu=(pBMSubmenu==NULL || 
					pBMSubmenu->GetPopupWnd()==NULL ||
					!::IsWindow(pBMSubmenu->GetPopupWnd()->GetSafeHwnd()) ||
					!pBMSubmenu->GetPopupWnd()->IsWindowVisible());
				// display new submenu
				if(bDisplaySubmenu)
				{
					// hide all existing submenus on this level
					int nLevel=GetBMPopupWndLevel(pPopupWnd);
					ASSERT(nLevel>=0 || nLevel==-1);
					if(nLevel>=0)
					{
						ResetPopupMenus(nLevel+1);
					}

					// child popup menu must be displayed
					COXBitmapMenuPopupWnd* pBMPopupWnd=
						(pBMSubmenu==NULL ? NULL : pBMSubmenu->GetPopupWnd());
					if(pBMPopupWnd==NULL)
					{
						pBMPopupWnd=GetFirstFreeBMPopupWnd(nLevel+1);
					}
					ASSERT(pBMPopupWnd!=NULL);
					CRect rect=pPopupWnd->GetItemRect(nInsertMarkIndex);
					pPopupWnd->ClientToScreen(rect);
					pPopupWnd->CalcWindowRect(rect);
					CRect rectWindow;
					pPopupWnd->GetWindowRect(rectWindow);

					if(pBMSubmenu==NULL)
					{
						HMENU hMenu=pSubmenu->GetSafeHmenu();
						GetBMOrganizer()->OnInitMenuPopup(pSubmenu,0,FALSE);
						pSubmenu=CMenu::FromHandle(hMenu);
						pBMSubmenu=DYNAMIC_DOWNCAST(COXBitmapMenu,pSubmenu);
						ASSERT(pBMSubmenu!=NULL);
					}

					pBMPopupWnd->TrackPopupMenu(pBMSubmenu,TPM_LEFTALIGN,
						rectWindow.right,rect.top,this);
				}
			}
			break;
		}

	case ID_OXCUSTBM_RECENTLY_USED:
	case ID_OXCUSTBM_DELETE:
	case ID_OXCUSTBM_SEPARATOR_BEFORE:
	case ID_OXCUSTBM_SEPARATOR_AFTER:
	case ID_OXCUSTBM_DELETE_ITEM:
	case ID_OXCUSTBM_INSERT_ITEM:
		{
			COXMenuBar* pMenuBar=COXMenuBar::FindMenuBar();
			if(pMenuBar!=NULL)
			{
				pMenuBar->MarkAsChanged();
			}
			break;
		}
	}
}


void COXCustomizeCommandsPage::OnDropDownModifySelection(NMHDR* pNotifyStruct, 
														 LRESULT* result)
{
	UNREFERENCED_PARAMETER(pNotifyStruct);

	ASSERT(m_pCustomizedToolbar!=NULL);
	int nCustomizedButtonIndex=m_pCustomizedToolbar->GetCustomizedButton();
	ASSERT(nCustomizedButtonIndex!=-1);

	CRect rect;
	m_btnModifySelection.GetWindowRect(rect);
	m_pCustomizedToolbar->
		DisplayCustomizeButtonContextMenu(nCustomizedButtonIndex,
		CPoint(rect.left,rect.bottom+1));

	*result=0;
}


COXBitmapMenuPopupWnd* COXCustomizeCommandsPage::
GetFirstFreeBMPopupWnd(int nMenuLevel/*=0*/)
{
	BOOL bFound=FALSE;
	COXBitmapMenuPopupWnd* pBMPopupWnd=NULL;
	POSITION pos=m_mapBMPopupWnd.GetStartPosition();
	int nLevel=-1;
	while(pos!=NULL)
	{
		m_mapBMPopupWnd.GetNextAssoc(pos,pBMPopupWnd,nLevel);
		ASSERT(pBMPopupWnd!=NULL);
		ASSERT(nLevel>=0);
		if(pBMPopupWnd->GetBitmapMenu()==NULL)
		{
			bFound=FALSE;
			break;
		}
	}
	if(!bFound)
	{
		pBMPopupWnd=new COXBitmapMenuPopupWnd;
	}
	ASSERT(pBMPopupWnd!=NULL);

	m_mapBMPopupWnd.SetAt(pBMPopupWnd,nMenuLevel);

	return pBMPopupWnd;
}


void COXCustomizeCommandsPage::ResetPopupMenus(int nMenuLevel/*=0*/)
{
	BOOL bOwnDragAndDrop=FALSE;
	POSITION pos=m_mapBMPopupWnd.GetStartPosition();
	COXBitmapMenuPopupWnd* pBMPopupWnd=NULL;
	int nLevel=-1;
	while(pos!=NULL)
	{
		m_mapBMPopupWnd.GetNextAssoc(pos,pBMPopupWnd,nLevel);
		ASSERT(pBMPopupWnd!=NULL);
		ASSERT(nLevel>=0);

		COXBitmapMenu* pMenu=pBMPopupWnd->GetBitmapMenu();
		if(pMenu!=NULL)
		{
			bOwnDragAndDrop|=pMenu->IsDragDropOwner();
		}

		if(nLevel>=nMenuLevel && ::IsWindowVisible(pBMPopupWnd->GetSafeHwnd()))
		{
			pBMPopupWnd->ResetPopupMenu();
		}
	}
	
	if(!bOwnDragAndDrop && nMenuLevel==0 && GetBMOrganizer()->IsAttached())
	{
		GetBMOrganizer()->Empty();
		GetBMOrganizer()->AutoSetMenuImage();
	}
}


int COXCustomizeCommandsPage::GetBMPopupWndLevel(COXBitmapMenuPopupWnd* pBMPopupWnd)
{
	ASSERT(pBMPopupWnd!=NULL);

	POSITION pos=m_mapBMPopupWnd.GetStartPosition();
	COXBitmapMenuPopupWnd* pTempBMPopupWnd=NULL;
	int nLevel=-1;
	while(pos!=NULL)
	{
		m_mapBMPopupWnd.GetNextAssoc(pos,pTempBMPopupWnd,nLevel);
		ASSERT(pTempBMPopupWnd!=NULL);
		ASSERT(nLevel>=0);
		if(pTempBMPopupWnd==pBMPopupWnd && ::IsWindow(pTempBMPopupWnd->GetSafeHwnd()))
		{
			return nLevel;
		}
	}

	return -1;
}


void COXCustomizeCommandsPage::ConvertToolbar(CToolBar* pToolbar)
{
	ASSERT(pToolbar!=NULL);
	ASSERT(pToolbar->GetToolBarCtrl().GetButtonCount()==0);

	pToolbar->
		SendMessage(TB_SETIMAGELIST,NULL,(LPARAM)((HIMAGELIST)m_ilImages));


	COXCoolToolBar* pCoolToolbar=
		DYNAMIC_DOWNCAST(COXCoolToolBar,pToolbar);
	if(pCoolToolbar!=NULL)
	{
		pCoolToolbar->
			SetBitmapIds(m_arrCommandIds,PtrToInt(m_mapCommands.GetCount()));
		// make sure toolbar is in list mode
		pCoolToolbar->SetList(TRUE);
		pCoolToolbar->IniSizes(CSize(ID_OXCUSTOMIZE_TBIMAGE_WIDTH,
			ID_OXCUSTOMIZE_TBIMAGE_HEIGHT));
	}
}


HMENU COXCustomizeCommandsPage::BuildMenuFromTree(HTREEITEM hParentItem)
{
	ASSERT(hParentItem!=NULL);
	HTREEITEM hChildItem=m_treeCommands.GetNextItem(hParentItem,TVGN_CHILD);
	if(hChildItem==NULL)
	{
		return NULL;
	}

	HMENU hMenu=::CreatePopupMenu();
	ASSERT(hMenu!=NULL);

	while(hChildItem!=NULL)
	{
		CString sText(_T(""));
		int nCommandID=0;
		HMENU hSubmenu=NULL;
		DWORD_PTR dwData=m_treeCommands.GetItemData(hChildItem);
		if(dwData==0 || dwData==-2)
		{
			ASSERT(FALSE);
		}
		else if(dwData==-1)
		{
			// existing popup menu
			sText=m_treeCommands.GetItemText(hChildItem);
			nCommandID=-1;
			hSubmenu=BuildMenuFromTree(hChildItem);
		}
		else 
		{
			nCommandID=(int)dwData;

			HINSTANCE hInstance=
				AfxFindResourceHandle(MAKEINTRESOURCE(nCommandID),RT_STRING);
			ASSERT(hInstance!=NULL);
			sText.LoadString(nCommandID);
			int nPosition=sText.Find(_T('\n'));
			if(nPosition!=-1)
				sText=sText.Mid(nPosition+1);
			if(sText.IsEmpty())
				sText=m_treeCommands.GetItemText(hChildItem);
		}
		ASSERT(!sText.IsEmpty());

		if(hSubmenu==NULL)
		{
			VERIFY(::AppendMenu(hMenu,MF_STRING,nCommandID,sText));
		}
		else
		{
			VERIFY(::AppendMenu(hMenu,MF_STRING|MF_POPUP,(UINT_PTR)hSubmenu,sText));
		}

		hChildItem=m_treeCommands.GetNextItem(hChildItem,TVGN_NEXT);
	}

	return hMenu;
}


void COXCustomizeCommandsPage::BuildArrayCommandIDs()
{
	int nCommandCount=PtrToInt(m_mapCommands.GetCount());
	if(nCommandCount!=0)
	{
		if(m_arrCommandIds!=NULL)
		{
			delete[] m_arrCommandIds;
		}

		m_arrCommandIds=new UINT[nCommandCount];
		ASSERT(m_arrCommandIds!=NULL);
		
		int nCommandWithoutImageCount=0;
		POSITION pos=m_mapCommands.GetStartPosition();
		UINT nCommandID=NULL;
		int nImageIndex=-1;
		while(pos!=NULL)
		{
			m_mapCommands.GetNextAssoc(pos,nCommandID,nImageIndex);
			ASSERT(nCommandID>0);
			if(nImageIndex!=-1)
			{
				ASSERT(nImageIndex-ID_OX_IMAGEOFFSET_COUNT<
					nCommandCount-nCommandWithoutImageCount);
				m_arrCommandIds[nImageIndex-ID_OX_IMAGEOFFSET_COUNT]=nCommandID;
			}
			else
			{
				nCommandWithoutImageCount++;
				m_arrCommandIds[nCommandCount-nCommandWithoutImageCount]=nCommandID;
			}
		}
	}
}


BOOL COXCustomizeCommandsPage::IsCustomCommand(UINT nCmdID)
{
	CString sText;
	return (m_mapCustomCommands.Lookup(nCmdID,sText));
}


BOOL COXCustomizeCommandsPage::GetCustomCommand(UINT nCmdID, CString& sText, 
												int& nImageIndex, CString& sHierarchy)
{
	if(!m_mapCustomCommands.Lookup(nCmdID,sText))
	{
		sText.Empty();
		nImageIndex=-1;
		sHierarchy.Empty();
		return FALSE;
	}

	VERIFY(m_mapCommands.Lookup(nCmdID,nImageIndex));
	VERIFY(m_mapCustomCommandsHierarchy.Lookup(nCmdID,sHierarchy));

	return TRUE;
}

BOOL COXCustomizeCommandsPage::AddCustomCommand(UINT nCmdID, int nIDCmd, 
		int nImageIndex, int nIDHierarchy)
{
	CString sCommand, sHierarchy;
	VERIFY(sCommand.LoadString(nIDCmd));
	VERIFY(sHierarchy.LoadString(nIDHierarchy));
	return AddCustomCommand(nCmdID, sCommand,nImageIndex, sHierarchy);
}

BOOL COXCustomizeCommandsPage::AddCustomCommand(UINT nCmdID, 
												LPCTSTR pszCmdText, 
												int nImageIndex/*=-1*/, 
												LPCTSTR pszHierarchy/*=_T("Custom commands")*/)
{
	if(nCmdID==0 || lstrlen(pszCmdText)==0 || lstrlen(pszHierarchy)==0)	
	{
		TRACE(_T("COXCustomizeCommandsPage::AddCustomCommand: invalid parameters\n"));
		return FALSE;
	}

	int nFoundImageIndex=-1;
	if(m_mapCommands.Lookup(nCmdID,nFoundImageIndex))
	{
		TRACE(_T("COXCustomizeCommandsPage::AddCustomCommand: command with specified ID exists already\n"));
		return FALSE;
	}

	m_mapCustomCommands.SetAt(nCmdID,pszCmdText);
	m_mapCustomCommandsHierarchy.SetAt(nCmdID,pszHierarchy);
	m_mapCommands.SetAt(nCmdID,nImageIndex);
	// rebuild array of all command IDs
	BuildArrayCommandIDs();

	// Iterate all the control bars and use only the toolbars
	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL);
	CFrameWnd* pFrameWnd=DYNAMIC_DOWNCAST(CFrameWnd,pWnd);
	ASSERT(pFrameWnd!=NULL);
	POSITION pos=pFrameWnd->m_listControlBars.GetHeadPosition();
	while(pos!=NULL)
	{
		CToolBar* pToolbar=DYNAMIC_DOWNCAST(CToolBar, 
			(CControlBar*)pFrameWnd->m_listControlBars.GetNext(pos));
		//If it is a CToolBar
		if(pToolbar!=NULL)
		{
			COXCoolToolBar* pCoolToolbar=
				DYNAMIC_DOWNCAST(COXCoolToolBar,pToolbar);
			if(pCoolToolbar!=NULL)
			{
				pCoolToolbar->
					SetBitmapIds(m_arrCommandIds,PtrToInt(m_mapCommands.GetCount()));
			}
		}
	}

	return TRUE;
}


BOOL COXCustomizeCommandsPage::AddCustomCommandsToTree()
{
	ASSERT(m_mapCustomCommands.GetCount()==m_mapCustomCommandsHierarchy.GetCount());

	POSITION pos=m_mapCustomCommands.GetStartPosition();
	while(pos!=NULL)
	{
		UINT nCmdID;
		CString sText;
		CString sHierarchy;
		m_mapCustomCommands.GetNextAssoc(pos,nCmdID,sText);
		ASSERT(!sText.IsEmpty());
		ASSERT(nCmdID>0);
		if(!FindCommandInTree(nCmdID,TVI_ROOT))
		{
			VERIFY(m_mapCustomCommandsHierarchy.Lookup(nCmdID,sHierarchy));
			ASSERT(!sHierarchy.IsEmpty());

			// analyze hierarchy (hierarchy items are divided using '\t' character)
			HTREEITEM hParentItem=TVI_ROOT;
			int nPos=sHierarchy.Find(_T('\t'));
			CString sHierarchyItem;
			while(TRUE)
			{
				if(nPos!=-1)
				{
					sHierarchyItem=sHierarchy.Left(nPos);
					sHierarchy=sHierarchy.Mid(nPos+1);
				}
				else
				{
					sHierarchyItem=sHierarchy;
				}

				HTREEITEM hFoundItem=FindMenuItemInTree(sHierarchyItem,hParentItem);
				if(hFoundItem!=NULL)
				{
					hParentItem=hFoundItem;
				}
				else
				{
					hParentItem=m_treeCommands.
						InsertItem(sHierarchyItem,ID_OX_IMAGEOFFSET_MENUGROUP,
						ID_OX_IMAGEOFFSET_MENUGROUP,hParentItem,TVI_LAST);
					ASSERT(hParentItem!=NULL);
					m_treeCommands.SetItemData(hParentItem,(DWORD)-1);
					m_treeCommands.SetItemState(hParentItem,TVIS_BOLD,TVIS_BOLD);
				}

				if(nPos!=-1)
				{
					nPos=sHierarchy.Find(_T('\t'));
				}
				else
				{
					break;
				}
			}

			// insert item in the tree 
			//

			// retrieve image
			int nImageIndex=-1;
			VERIFY(m_mapCommands.Lookup(nCmdID,nImageIndex));
			if(nImageIndex==-1)
			{
				nImageIndex=ID_OX_IMAGEOFFSET_NOIMAGE;
			}

			HTREEITEM hItem=m_treeCommands.
				InsertItem(sText,nImageIndex,nImageIndex,hParentItem,TVI_LAST);
			ASSERT(hItem!=NULL);
			m_treeCommands.SetItemData(hItem,(LPARAM)nCmdID);

			//
			////////////////////////////////
		}
	}

	return TRUE;
}


HTREEITEM COXCustomizeCommandsPage::FindCommandInTree(UINT nCmdID, 
													  HTREEITEM hParentItem) const
{
	ASSERT(nCmdID>0);
	ASSERT(hParentItem!=NULL);

	HTREEITEM hChildItem=m_treeCommands.GetNextItem(hParentItem,TVGN_CHILD);

	HTREEITEM hFoundItem=NULL;
	while(hChildItem!=NULL)
	{
		if(m_treeCommands.ItemHasChildren(hChildItem))
		{
			hFoundItem=FindCommandInTree(nCmdID,hChildItem);
			if(hFoundItem!=NULL)
			{
				break;
			}
		}
		else
		{
			DWORD_PTR dwData=m_treeCommands.GetItemData(hChildItem);
			if((UINT_PTR)dwData==nCmdID)
			{
				hFoundItem=hChildItem;
				break;
			}
		}

		hChildItem=m_treeCommands.GetNextItem(hChildItem,TVGN_NEXT);
	}

	return hFoundItem;
}

void COXCustomizeCommandsPage::OnCheckIconsInMenu() 
{
	UpdateData();	
}

void COXCustomizeCommandsPage::OnCheckShowOnlyRecentlyUsed() 
{
	UpdateData();
}
