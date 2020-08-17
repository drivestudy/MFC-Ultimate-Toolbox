// OXCustomizeTrayIconPage.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXCustomizeTrayIconPage.h"

#ifdef OX_CUSTOMIZE_COMMANDS
#include "OXBitmapMenuOrganizer.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// COXCustomizeTaskbarIcon dialog

BEGIN_MESSAGE_MAP(COXCustomizeTaskbarIcon, COXTaskbarIcon)
	//{{AFX_MSG_MAP(COXCustomizeTaskbarIcon)
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void COXCustomizeTaskbarIcon::OnLButtonUp(UINT nFlags, CPoint point) 
{
	COXTaskbarIcon::OnLButtonUp(nFlags,point);

	ASSERT(m_pTrayIconPage!=NULL);
	if(m_pTrayIconPage->m_nDisplayMenuEvent==0)
	{
		m_pTrayIconPage->DoPopupMenu();
	}
}

void COXCustomizeTaskbarIcon::OnRButtonUp(UINT nFlags, CPoint point) 
{
	COXTaskbarIcon::OnRButtonUp(nFlags,point);

	ASSERT(m_pTrayIconPage!=NULL);
	if(m_pTrayIconPage->m_nDisplayMenuEvent==1)
	{
		m_pTrayIconPage->DoPopupMenu();
	}
}

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeTrayIconPage dialog

IMPLEMENT_DYNCREATE(COXCustomizeTrayIconPage, COXCustomizePage)

COXCustomizeTrayIconPage::COXCustomizeTrayIconPage()
{
	//{{AFX_DATA_INIT(COXCustomizeTrayIconPage)
	m_bShowTrayIcon = TRUE;
	m_sIconTooltip = _T("");
	m_nDisplayMenuEvent = 1;
	m_nIconSourceType = 0;
	//}}AFX_DATA_INIT

	m_hOrigIcon=NULL;
	m_hUserIcon=NULL;
	m_sIconSourceFileName.Empty();
	m_nIconIndex = -1;
	m_bUseRightClick=TRUE;

	m_nDialogID=IDD;
	m_sProfileName.Empty();

	m_trayIcon.Create();
	// Note: you don't need to destroy a taskbar icon. 
	// It's done in its own destructor.
	m_trayIcon.m_pTrayIconPage=this;
}


COXCustomizeTrayIconPage::~COXCustomizeTrayIconPage()
{
	if((HMENU)m_contextMenu!=NULL)
		m_contextMenu.DestroyMenu();

	if(m_hUserIcon!=NULL)
	{
		::DestroyIcon(m_hUserIcon);
		m_hUserIcon=NULL;
	}
}


BOOL COXCustomizeTrayIconPage::
InitializeTrayIcon(HICON hIcon, UINT nContextMenuResourceID,
				   LPCTSTR lpszIconTooltip/*=NULL*/, 
				   BOOL bShowTrayIcon/*=TRUE*/, 
				   BOOL bUseRightClick/*=TRUE*/, 
				   LPCTSTR lpszIconSourceFileName/*=NULL*/,
				   LPCTSTR lpszProfileName/*=_T("CustomizeTrayIcon")*/)
{
	ASSERT(hIcon!=NULL);
	m_hOrigIcon=hIcon;

	HINSTANCE hMenuResourceInstance=
		AfxFindResourceHandle(MAKEINTRESOURCE(nContextMenuResourceID),RT_MENU);
	ASSERT(hMenuResourceInstance!=NULL);
	VERIFY(m_contextMenu.LoadMenu(nContextMenuResourceID));

	m_sIconTooltip=lpszIconTooltip;

	m_bShowTrayIcon=bShowTrayIcon;

	m_bUseRightClick=bUseRightClick;

	m_bUseDefaultIcon=TRUE;

	m_sIconSourceFileName=lpszIconSourceFileName;
	m_nIconIndex=-1;

	CWinApp* pApp=AfxGetApp();
	ASSERT(pApp!=NULL);

	if(lpszProfileName!=NULL)
	{
		m_sProfileName=lpszProfileName;
	
		m_sIconTooltip=pApp->GetProfileString(m_sProfileName,
			_T("TrayIconTooltip"),m_sIconTooltip);
		m_bShowTrayIcon=pApp->GetProfileInt(m_sProfileName,
			_T("ShowTrayIcon"),m_bShowTrayIcon);
		m_bUseRightClick=pApp->GetProfileInt(m_sProfileName,
			_T("UseRightClick"),m_bUseRightClick);
		m_bUseDefaultIcon=pApp->GetProfileInt(m_sProfileName,
			_T("UseDefaultIcon"),m_bUseDefaultIcon);
		m_sIconSourceFileName=pApp->GetProfileString(m_sProfileName,
			_T("TrayIconSourceFileName"),m_sIconSourceFileName);
		m_nIconIndex=pApp->GetProfileInt(m_sProfileName,
			_T("IconIndexFromSourceFile"),m_nIconIndex);
	}

	if(m_sIconSourceFileName.IsEmpty())
	{
		//get the application path
		::GetModuleFileName(NULL,m_sIconSourceFileName.GetBuffer(MAX_PATH),MAX_PATH);
		m_sIconSourceFileName.ReleaseBuffer();
	}

	// initialize tray icon
	//
	SetupTrayIcon();
	//
	//////////////////////////////
	
	return TRUE;
}

BOOL COXCustomizeTrayIconPage::OnCloseManager(BOOL bIsOk) 
{	
	CWinApp* pApp=AfxGetApp();
	ASSERT(pApp!=NULL);

	if(bIsOk)
	{
		if(!ApplyChanges())
			return FALSE;
		if(!m_sProfileName.IsEmpty())
		{
			pApp->WriteProfileString(m_sProfileName,
				_T("TrayIconTooltip"),m_sIconTooltip);
			pApp->WriteProfileInt(m_sProfileName,
				_T("ShowTrayIcon"),m_bShowTrayIcon);
			pApp->WriteProfileInt(m_sProfileName,
				_T("UseRightClick"),m_bUseRightClick);
			pApp->WriteProfileInt(m_sProfileName,
				_T("UseDefaultIcon"),m_bUseDefaultIcon);
			pApp->WriteProfileString(m_sProfileName,
				_T("TrayIconSourceFileName"),m_sIconSourceFileName);
			pApp->WriteProfileInt(m_sProfileName,
				_T("IconIndexFromSourceFile"),m_nIconIndex);
		}
	}
	else
	{
		if(!m_sProfileName.IsEmpty())
		{
			m_sIconTooltip=pApp->GetProfileString(m_sProfileName,
				_T("TrayIconTooltip"),m_sIconTooltip);
			m_bShowTrayIcon=pApp->GetProfileInt(m_sProfileName,
				_T("ShowTrayIcon"),m_bShowTrayIcon);
			m_bUseRightClick=pApp->GetProfileInt(m_sProfileName,
				_T("UseRightClick"),m_bUseRightClick);
			m_bUseDefaultIcon=pApp->GetProfileInt(m_sProfileName,
				_T("UseDefaultIcon"),m_bUseDefaultIcon);
			m_sIconSourceFileName=pApp->GetProfileString(m_sProfileName,
				_T("TrayIconSourceFileName"),m_sIconSourceFileName);
			m_nIconIndex=pApp->GetProfileInt(m_sProfileName,
				_T("IconIndexFromSourceFile"),m_nIconIndex);

			if(m_sIconSourceFileName.IsEmpty())
			{
				//get the application path
				::GetModuleFileName(NULL,
					m_sIconSourceFileName.GetBuffer(MAX_PATH),MAX_PATH);
				m_sIconSourceFileName.ReleaseBuffer();
			}

			SetupTrayIcon();
		}
	}

	return TRUE;
}


void COXCustomizeTrayIconPage::DoPopupMenu()
{
	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL);

#ifdef OX_CUSTOMIZE_COMMANDS
	COXBitmapMenuOrganizer* pBMOrganizer=
		COXBitmapMenuOrganizer::FindOrganizer(pWnd->GetSafeHwnd());
	BOOL bRestoreMRUFlag=FALSE;
	if(pBMOrganizer!=NULL && pBMOrganizer->IsShowOnlyRecentlyUsedItems())
	{
		pBMOrganizer->ShowOnlyRecentlyUsedItems(FALSE);
		bRestoreMRUFlag=TRUE;
	}
#endif

	ASSERT((HMENU)m_contextMenu!=NULL);
	CMenu* pPopupMenu=m_contextMenu.GetSubMenu(0);

	pWnd->SetForegroundWindow(); 
	CPoint posMouse;
	GetCursorPos(&posMouse);
	pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_LEFTBUTTON,
		posMouse.x,posMouse.y,pWnd);

#ifdef OX_CUSTOMIZE_COMMANDS
	if(pBMOrganizer!=NULL && bRestoreMRUFlag)
	{
		pBMOrganizer->ShowOnlyRecentlyUsedItems(TRUE);
	}
#endif
}


void COXCustomizeTrayIconPage::DoDataExchange(CDataExchange* pDX)
{
	COXCustomizePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXCustomizeTrayIconPage)
	DDX_Control(pDX, IDC_OX_STATIC_DEFAULT_ICON, m_staticDefaultIcon);
	DDX_Control(pDX, IDC_OX_SEPARATOR_SELECT_ICON, m_sepSelectIcon);
	DDX_Control(pDX, IDC_OX_SEPARATOR_DISPLAY_SETTINGS, m_sepDisplayMenuEvent);
	DDX_Control(pDX, IDC_OX_LIST_ICON, m_listIcons);
	DDX_Control(pDX, IDC_OX_COMBO_ICONSOURCE_FILENAME, m_comboIconSource);
	DDX_Check(pDX, IDC_OX_CHECK_SHOW_TRAYICON, m_bShowTrayIcon);
	DDX_Text(pDX, IDC_OX_EDIT_ICON_TOOLTIP, m_sIconTooltip);
	DDX_Radio(pDX, IDC_OX_RADIO_DISPLAY_MENU_LEFTCLICK, m_nDisplayMenuEvent);
	DDX_Radio(pDX, IDC_OX_RADIO_ICON_SOURCE_DEFAULT, m_nIconSourceType);
	DDX_CBString(pDX, IDC_OX_COMBO_ICONSOURCE_FILENAME, m_sIconSourceFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COXCustomizeTrayIconPage, COXCustomizePage)
	//{{AFX_MSG_MAP(COXCustomizeTrayIconPage)
	ON_BN_CLICKED(IDC_OX_RADIO_ICON_SOURCE_DEFAULT, OnRadioIconSourceDefault)
	ON_BN_CLICKED(IDC_OX_RADIO_ICON_SOURCE_FILE, OnRadioIconSourceFile)
	ON_CBN_SELCHANGE(IDC_OX_COMBO_ICONSOURCE_FILENAME, OnSelchangeComboIconsourceFilename)
	ON_CBN_EDITCHANGE(IDC_OX_COMBO_ICONSOURCE_FILENAME, OnEditchangeComboIconsourceFilename)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeTrayIconPage message handlers

BOOL COXCustomizeTrayIconPage::Load(const COXCustomizeManager* pCustomizeManager)
{
	// Call default implementation. It will load this demo dialog as the first 
	// page and will create About and CodeSample pages if specified.
	if(!COXCustomizePage::Load(pCustomizeManager))
		return FALSE;

	return TRUE;
}


void COXCustomizeTrayIconPage::Unload()
{
	// add here code for cleaning up all objects created by demo
	//
	//
	//////////////////////////////////////////////////////////////////////////

	COXCustomizePage::Unload();
}

void COXCustomizeTrayIconPage::OnInitDialog()
{
	// must call default implementation
	COXCustomizePage::OnInitDialog();

	// add here initialization code for your demo dialog. Treat it as a
	// normal COXCustomizePage::OnInitDialog function

	m_comboIconSource.
		SetAutoPersistent(_T("CustomizeTrayIcon_IconSourceFileName"));
	for(int nIndex=4; nIndex<OX_HISTORY_COMBO_MAX_TOOLBUTTONS; nIndex++)
	{
		m_comboIconSource.ShowButton(nIndex,FALSE);
	}
	CToolBar* pToolbar=m_comboIconSource.GetToolBar();
	pToolbar->ModifyStyle(NULL,TBSTYLE_FLAT);
	m_comboIconSource.RefreshToolbar();

	m_nDisplayMenuEvent=(m_bUseRightClick ? 1 : 0);

	m_nIconSourceType=(m_bUseDefaultIcon ? 0 : 1);

	m_staticDefaultIcon.SetIcon(m_hOrigIcon);

	// layout
	//
	m_LayoutManager.TieChild(&m_sepSelectIcon,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_sepDisplayMenuEvent,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_EDIT_ICON_TOOLTIP,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_comboIconSource,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_listIcons,OX_LMS_ANY,OX_LMT_SAME);
	//
	///////////////////////////////////////

	m_bInitialized=TRUE;

	UpdateData(FALSE);

	m_listIcons.EnableWindow(!m_bUseDefaultIcon);
	m_comboIconSource.EnableWindow(!m_bUseDefaultIcon);

	OnSelchangeComboIconsourceFilename();
	m_listIcons.SetCurSel(m_nIconIndex);
}


LRESULT COXCustomizeTrayIconPage::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	// Update the context list
	m_comboIconSource.OnIdle();

	return TRUE;
}

BOOL COXCustomizeTrayIconPage::ApplyChanges()
{
	if(!UpdateData(TRUE))
		return FALSE;

	m_nIconIndex=m_listIcons.GetCurSel();

	m_bUseRightClick=(m_nDisplayMenuEvent==1);
	m_bUseDefaultIcon=(m_nIconSourceType==0);

	SetupTrayIcon();

	return TRUE;
}

void COXCustomizeTrayIconPage::SetupTrayIcon()
{
	m_trayIcon.SetTooltipText(m_sIconTooltip);

	if(m_bShowTrayIcon)
	{
		m_trayIcon.Show();
	}
	else
	{
		m_trayIcon.Hide();
	}

	if(m_bUseDefaultIcon)
	{
		m_trayIcon.SetIcon(m_hOrigIcon);
	}
	else
	{
		if(m_hUserIcon!=NULL)
		{
			::DestroyIcon(m_hUserIcon);
			m_hUserIcon=NULL;
		}
		m_hUserIcon=::ExtractIcon(
			AfxGetInstanceHandle(),m_sIconSourceFileName,m_nIconIndex);
		if(m_hUserIcon==NULL || m_hUserIcon==(HICON)1)
		{
			TRACE(_T("COXCustomizeTrayIconPage::SetupTrayIcon: failed to load icon %d from %s. Use default icon\n"),m_nIconIndex,m_sIconSourceFileName);
			m_trayIcon.SetIcon(m_hOrigIcon);
			m_bUseDefaultIcon=TRUE;
			m_nIconIndex=-1;
			m_hUserIcon=NULL;
		}
		else
		{
			m_trayIcon.SetIcon(m_hUserIcon);
		}
	}
}

void COXCustomizeTrayIconPage::OnRadioIconSourceDefault() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	m_listIcons.EnableWindow(FALSE);
	m_comboIconSource.EnableWindow(FALSE);

	m_sIconSourceFileName = _T("");
	m_bUseDefaultIcon = TRUE;
	UpdateData(FALSE);
}

void COXCustomizeTrayIconPage::OnRadioIconSourceFile() 
{
	// TODO: Add your control notification handler code here
	m_listIcons.EnableWindow(TRUE);
	m_comboIconSource.EnableWindow(TRUE);
	m_comboIconSource.RefreshToolbar();
}

void COXCustomizeTrayIconPage::OnSelchangeComboIconsourceFilename() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData(TRUE))
		return;

	if(m_comboIconSource.GetCurSel()!=CB_ERR)
	{
		m_comboIconSource.GetLBText(m_comboIconSource.GetCurSel(),
			m_sIconSourceFileName);
		m_listIcons.LoadIconsFromFile(m_sIconSourceFileName,FALSE);
		m_listIcons.SetCurSel(0);
	}
}

void COXCustomizeTrayIconPage::OnEditchangeComboIconsourceFilename() 
{
	// TODO: Add your control notification handler code here
	
	if(!UpdateData(TRUE))
		return;

	m_listIcons.LoadIconsFromFile(m_sIconSourceFileName,FALSE);
	m_listIcons.SetCurSel(0);
}
