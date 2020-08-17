// OXCustomizeSplashWindowPage.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXCustomizeSplashWindowPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeSplashWindowPage dialog

IMPLEMENT_DYNCREATE(COXCustomizeSplashWindowPage, COXCustomizePage)


COXCustomizeSplashWindowPage::COXCustomizeSplashWindowPage()
{
	//{{AFX_DATA_INIT(COXCustomizeSplashWindowPage)
	m_bInterruptable = FALSE;
	m_bDisplaySplashWindow = TRUE;
	m_nShowDelay = 0;
	m_nXCoord = 0;
	m_nYCoord = 0;
	m_nTransparentColorType = 2;
	m_nImageSourceType = 0;
	m_nDelayType = 1;
	m_sSplashImageFileName = _T("");
	m_nTolerance = 0;
	//}}AFX_DATA_INIT
	m_clrTransparent=::GetSysColor(COLOR_WINDOW);

	m_nDefaultBitmapID=NULL;   
	m_bUseDefaultImage=TRUE;
	m_bShowInfinitely=TRUE;

	m_nDialogID=IDD;
	m_sProfileName.Empty();
}


COXCustomizeSplashWindowPage::~COXCustomizeSplashWindowPage()
{
}


BOOL COXCustomizeSplashWindowPage::
InitializeSplashWindow(UINT nBitmapResourceID, BOOL bShowSplashWnd/*=TRUE*/,
					   int nDisplayFor/*=-1*/, BOOL bUserCanCancel/*=FALSE*/,
					   COLORREF clrTransparent/*=CLR_DEFAULT*/, 
					   LPPOINT pColorPoint/*=NULL*/, BYTE nTolerance/*=0*/, 
					   LPCTSTR lpszProfileName/*=_T("CustomizeSplashWindow")*/)
{
	ASSERT(nBitmapResourceID!=NULL);

#ifdef _DEBUG
	HINSTANCE hBitmapResourceInstance=
		AfxFindResourceHandle(MAKEINTRESOURCE(nBitmapResourceID),RT_BITMAP);
	ASSERT(hBitmapResourceInstance!=NULL);
#endif	//	_DEBUG

	m_nDefaultBitmapID=nBitmapResourceID;
	m_bDisplaySplashWindow=bShowSplashWnd;
	m_bUseDefaultImage=TRUE;
	m_bShowInfinitely=(nDisplayFor==-1);
	m_bInterruptable=bUserCanCancel;
	m_nShowDelay=(nDisplayFor>=0 ? nDisplayFor : 0);
	if(pColorPoint==NULL)
	{
		m_nXCoord=0;
		m_nYCoord=0;
	}
	else
	{
		m_nXCoord=pColorPoint->x;
		m_nYCoord=pColorPoint->y;
	}

	if(clrTransparent==CLR_DEFAULT)
	{
		m_nTransparentColorType=2;
	}
	else if(clrTransparent==CLR_NONE)
	{
		m_nTransparentColorType=1;
	}
	else 
	{
		m_clrTransparent=clrTransparent;
		m_nTransparentColorType=0;
	}

	m_nTolerance=nTolerance;
	
	m_sSplashImageFileName.Empty();


	if(lpszProfileName!=NULL)
	{
		m_sProfileName=lpszProfileName;

		CWinApp* pApp=AfxGetApp();
		ASSERT(pApp!=NULL);

		m_bDisplaySplashWindow=pApp->GetProfileInt(m_sProfileName,
			_T("DisplaySplashWindow"),m_bDisplaySplashWindow);
		m_bInterruptable=pApp->GetProfileInt(m_sProfileName,
			_T("SplashWindow_Interruptable"),m_bInterruptable);
		m_nShowDelay=pApp->GetProfileInt(m_sProfileName,
			_T("SplashWindowShowDelay"),m_nShowDelay);
		m_nXCoord=pApp->GetProfileInt(m_sProfileName,
			_T("TransparentColorPoint_CoordX"),m_nXCoord);
		m_nYCoord=pApp->GetProfileInt(m_sProfileName,
			_T("TransparentColorPoint_CoordY"),m_nYCoord);
		m_nTransparentColorType=pApp->GetProfileInt(m_sProfileName,
			_T("TransparentColorType"),m_nTransparentColorType);
		m_bUseDefaultImage=pApp->GetProfileInt(m_sProfileName,
			_T("UseDefaultImage"),m_bUseDefaultImage);
		m_bShowInfinitely=pApp->GetProfileInt(m_sProfileName,
			_T("SplashWindowShowInfinitely"),m_bShowInfinitely);
		m_sSplashImageFileName=pApp->GetProfileString(m_sProfileName,
			_T("SplashImageFileName"),m_sSplashImageFileName);
		m_clrTransparent=pApp->GetProfileInt(m_sProfileName,
			_T("TransparentColor"),m_clrTransparent);
		m_nTolerance=pApp->GetProfileInt(m_sProfileName,
			_T("ColorTolerance"),m_nTolerance);
	}

	m_nImageSourceType=(m_bUseDefaultImage ? 0 : 1);
	m_nDelayType=(m_bShowInfinitely ? 1 : 0);

	if(m_bDisplaySplashWindow)
		ShowSplashWindow(&m_splashWindow);

	return TRUE;
}


BOOL COXCustomizeSplashWindowPage::OnCloseManager(BOOL bIsOk) 
{	
	if(bIsOk)
	{
		if(!ApplyChanges())
			return FALSE;
		if(!m_sProfileName.IsEmpty())
		{
			CWinApp* pApp=AfxGetApp();
			ASSERT(pApp!=NULL);

			pApp->WriteProfileInt(m_sProfileName,
				_T("DisplaySplashWindow"),m_bDisplaySplashWindow);
			pApp->WriteProfileInt(m_sProfileName,
				_T("SplashWindow_Interruptable"),m_bInterruptable);
			pApp->WriteProfileInt(m_sProfileName,
				_T("SplashWindowShowDelay"),m_nShowDelay);
			pApp->WriteProfileInt(m_sProfileName,
				_T("TransparentColorPoint_CoordX"),m_nXCoord);
			pApp->WriteProfileInt(m_sProfileName,
				_T("TransparentColorPoint_CoordY"),m_nYCoord);
			pApp->WriteProfileInt(m_sProfileName,
				_T("TransparentColorType"),m_nTransparentColorType);
			pApp->WriteProfileInt(m_sProfileName,
				_T("UseDefaultImage"),m_bUseDefaultImage);
			pApp->WriteProfileInt(m_sProfileName,
				_T("SplashWindowShowInfinitely"),m_bShowInfinitely);
			pApp->WriteProfileString(m_sProfileName,
				_T("SplashImageFileName"),m_sSplashImageFileName);
			pApp->WriteProfileInt(m_sProfileName,
				_T("TransparentColor"),m_clrTransparent);
			pApp->WriteProfileInt(m_sProfileName,
			_T("ColorTolerance"),m_nTolerance);
		}
	}
	else
	{
		if(!m_sProfileName.IsEmpty())
		{
		}
	}

	return TRUE;
}


void COXCustomizeSplashWindowPage::DoDataExchange(CDataExchange* pDX)
{
	COXCustomizePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXCustomizeSplashWindowPage)
	DDX_Control(pDX, IDC_OX_SPIN_COLOR_TOLERANCE, m_spinTolerance);
	DDX_Control(pDX, IDC_OX_COMBO_IMAGESOURCE_FILENAME, m_comboSplashImageFileName);
	DDX_Control(pDX, IDC_OX_SPIN_SHOW_FOR, m_spinShowFor);
	DDX_Control(pDX, IDC_OX_SEPARATOR_TRANSPARENT_COLOR, m_sepTransparentColor);
	DDX_Control(pDX, IDC_OX_SEPARATOR_PREVIEW, m_sepPreview);
	DDX_Control(pDX, IDC_OX_SEPARATOR_OPEN_IMAGE, m_sepOpenImage);
	DDX_Control(pDX, IDC_OX_SEPARATOR_DISPLAY_SETTINGS, m_sepDisplaySettings);
	DDX_Control(pDX, IDC_OX_BUTTON_TRANSPARENT_COLOR, m_btnTransparentColor);
	DDX_Check(pDX, IDC_OX_CHECK_INTERRUPTABLE, m_bInterruptable);
	DDX_Check(pDX, IDC_OX_CHECK_DISPLAY_SPLASHWINDOW, m_bDisplaySplashWindow);
	DDX_Text(pDX, IDC_OX_EDIT_SHOW_FOR, m_nShowDelay);
	DDX_Text(pDX, IDC_OX_EDIT_XCOORD, m_nXCoord);
	DDX_Text(pDX, IDC_OX_EDIT_YCOORD, m_nYCoord);
	DDX_Radio(pDX, IDC_OX_RADIO_COLOR, m_nTransparentColorType);
	DDX_Radio(pDX, IDC_OX_RADIO_IMAGE_SOURCE_DEFAULT, m_nImageSourceType);
	DDX_Radio(pDX, IDC_OX_RADIO_SHOW_FOR, m_nDelayType);
	DDX_CBString(pDX, IDC_OX_COMBO_IMAGESOURCE_FILENAME, m_sSplashImageFileName);
	DDX_Text(pDX, IDC_OX_EDIT_COLOR_TOLERANCE, m_nTolerance);
	DDV_MinMaxUInt(pDX, m_nTolerance, 0, 255);
	//}}AFX_DATA_MAP
	DDX_ColorPicker(pDX, IDC_OX_BUTTON_TRANSPARENT_COLOR, m_clrTransparent);
}


BEGIN_MESSAGE_MAP(COXCustomizeSplashWindowPage, COXCustomizePage)
	//{{AFX_MSG_MAP(COXCustomizeSplashWindowPage)
	ON_BN_CLICKED(IDC_OX_BUTTON_PREVIEW, OnButtonPreview)
	ON_BN_CLICKED(IDC_OX_RADIO_COLOR, OnRadioColor)
	ON_BN_CLICKED(IDC_OX_RADIO_COLOR_NONE, OnRadioColorNone)
	ON_BN_CLICKED(IDC_OX_RADIO_COLORATPOINT, OnRadioColoratpoint)
	ON_BN_CLICKED(IDC_OX_RADIO_IMAGE_SOURCE_DEFAULT, OnRadioImageSourceDefault)
	ON_BN_CLICKED(IDC_OX_RADIO_IMAGE_SOURCE_FILE, OnRadioImageSourceFile)
	ON_BN_CLICKED(IDC_OX_RADIO_SHOW_FOR, OnRadioShowFor)
	ON_BN_CLICKED(IDC_OX_RADIO_SHOW_INFINITELY, OnRadioShowInfinitely)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeSplashWindowPage message handlers

BOOL COXCustomizeSplashWindowPage::Load(const COXCustomizeManager* pCustomizeManager)
{
	// Call default implementation. It will load this demo dialog as the first 
	// page and will create About and CodeSample pages if specified.
	if(!COXCustomizePage::Load(pCustomizeManager))
		return FALSE;

	return TRUE;
}


void COXCustomizeSplashWindowPage::Unload()
{
	// add here code for cleaning up all objects created by demo
	//
	//
	//////////////////////////////////////////////////////////////////////////

	COXCustomizePage::Unload();
}

void COXCustomizeSplashWindowPage::OnInitDialog()
{
	// must call default implementation
	COXCustomizePage::OnInitDialog();

	// add here initialization code for your demo dialog. Treat it as a
	// normal CDialog::OnInitDialog function

	m_comboSplashImageFileName.
		SetAutoPersistent(_T("CustomizeSplashWindow_ImageFileName"));
	for(int nIndex=4; nIndex<OX_HISTORY_COMBO_MAX_TOOLBUTTONS; nIndex++)
	{
		m_comboSplashImageFileName.ShowButton(nIndex,FALSE);
	}
	CToolBar* pToolbar=m_comboSplashImageFileName.GetToolBar();
	pToolbar->ModifyStyle(NULL,TBSTYLE_FLAT);
	m_comboSplashImageFileName.RefreshToolbar();

	m_btnTransparentColor.SetToolTip(TRUE);
	m_btnTransparentColor.SetDefaultColor(::GetSysColor(COLOR_BTNFACE));

	m_nImageSourceType=(m_bUseDefaultImage ? 0 : 1);
	m_nDelayType=(m_bShowInfinitely ? 1 : 0);

	m_spinShowFor.SetRange(0,32000);
	m_spinTolerance.SetRange(0,255);

	// layout
	//
	m_LayoutManager.TieChild(&m_sepDisplaySettings,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_sepOpenImage,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_sepPreview,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_sepTransparentColor,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);

	m_LayoutManager.TieChild(IDC_OX_BUTTON_PREVIEW,
		OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_comboSplashImageFileName,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	//
	///////////////////////////////////////

	m_bInitialized=TRUE;

	UpdateData(FALSE);
	ShowVars();
}


BOOL COXCustomizeSplashWindowPage::ApplyChanges()
{
	if(!UpdateData(TRUE))
		return FALSE;

	m_bUseDefaultImage=(m_nImageSourceType==0);
	m_bShowInfinitely=(m_nDelayType==1);

	return TRUE;
}


void COXCustomizeSplashWindowPage::ShowVars()
{
	if(!m_bInitialized)
		return;

	if(!UpdateData(TRUE))
		return;

	GetDlgItem(IDC_OX_EDIT_SHOW_FOR)->EnableWindow(m_nDelayType==0);
	GetDlgItem(IDC_OX_SPIN_SHOW_FOR)->EnableWindow(m_nDelayType==0);

	m_comboSplashImageFileName.EnableWindow(m_nImageSourceType==1);
	m_comboSplashImageFileName.RefreshToolbar();

	m_btnTransparentColor.EnableWindow(m_nTransparentColorType==0);
	GetDlgItem(IDC_OX_EDIT_XCOORD)->EnableWindow(m_nTransparentColorType==2);
	GetDlgItem(IDC_OX_EDIT_YCOORD)->EnableWindow(m_nTransparentColorType==2);
	GetDlgItem(IDC_OX_EDIT_COLOR_TOLERANCE)->EnableWindow(m_nTransparentColorType!=1);
	GetDlgItem(IDC_OX_SPIN_COLOR_TOLERANCE)->EnableWindow(m_nTransparentColorType!=1);
}


LRESULT COXCustomizeSplashWindowPage::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	// Update the context list
	m_comboSplashImageFileName.OnIdle();

	return TRUE;
}


void COXCustomizeSplashWindowPage::OnButtonPreview() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData(TRUE))
		return;

	ShowSplashWindow(&m_splashWindowTest,TRUE);
}

void COXCustomizeSplashWindowPage::OnRadioColor() 
{
	// TODO: Add your control notification handler code here
	ShowVars();
}

void COXCustomizeSplashWindowPage::OnRadioColorNone() 
{
	// TODO: Add your control notification handler code here
	ShowVars();
}

void COXCustomizeSplashWindowPage::OnRadioColoratpoint() 
{
	// TODO: Add your control notification handler code here
	ShowVars();
}

void COXCustomizeSplashWindowPage::OnRadioImageSourceDefault() 
{
	// TODO: Add your control notification handler code here
	ShowVars();
	
	m_sSplashImageFileName = _T("");
	UpdateData(FALSE);
}

void COXCustomizeSplashWindowPage::OnRadioImageSourceFile() 
{
	// TODO: Add your control notification handler code here
	ShowVars();
}

void COXCustomizeSplashWindowPage::OnRadioShowFor() 
{
	// TODO: Add your control notification handler code here
	ShowVars();
}

void COXCustomizeSplashWindowPage::OnRadioShowInfinitely() 
{
	// TODO: Add your control notification handler code here
	ShowVars();
}


void COXCustomizeSplashWindowPage::ShowSplashWindow(COXSplashWnd* pSplashWindow,
													BOOL bForceUserCancel/*=FALSE*/)
{
	ASSERT(pSplashWindow!=NULL);

	COLORREF clr=(m_nTransparentColorType==0 ?
		m_clrTransparent : (m_nTransparentColorType==1 ? CLR_NONE : CLR_DEFAULT));
	CPoint point(m_nXCoord,m_nYCoord);
	if(m_nImageSourceType==0)
	{
		VERIFY(pSplashWindow->LoadBitmap(m_nDefaultBitmapID,
			clr,&point,(BYTE)m_nTolerance));
	}
	else
	{
		if(!pSplashWindow->LoadBitmapFile(m_sSplashImageFileName,
			clr,&point,(BYTE)m_nTolerance))
		{
			TRACE(_T("Failed to load %s bitmap file"),m_sSplashImageFileName);
			return;
		}

	}

	pSplashWindow->SetUserCancel(m_bInterruptable || bForceUserCancel);
	pSplashWindow->Show((m_nDelayType==0 ? m_nShowDelay : SPLASH_NOTIMER),NULL);
}
