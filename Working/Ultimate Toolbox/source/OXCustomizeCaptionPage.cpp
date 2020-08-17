// OXCustomizeCaptionPage.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXCustomizeCaptionPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeCaptionPage dialog

IMPLEMENT_DYNCREATE(COXCustomizeCaptionPage, COXCustomizePage)

COXCustomizeCaptionPage::COXCustomizeCaptionPage()
{
	//{{AFX_DATA_INIT(COXCustomizeCaptionPage)
	m_bGradient = FALSE;
	m_nCaptionState = -1;
	m_nGradientAlgorithm = -1;
	m_nGradientAlignment = -1;
	m_nTextAlignment = -1;
	m_nTextEllipsis = -1;
	m_nNumberShade = 0;
	m_bPaintCaption = TRUE;
	//}}AFX_DATA_INIT

	m_bOnlyMainWindow=FALSE;

	m_nDialogID=IDD;
	m_sProfileName.Empty();
}

COXCustomizeCaptionPage::~COXCustomizeCaptionPage()
{
}


BOOL COXCustomizeCaptionPage::
InitializeCaptionPainter(BOOL bPaintCaption/*=TRUE*/,
						 LPCTSTR lpszProfileName/*=_T("CustomizeCaptionPainter")*/, 
						 BOOL bOnlyMainWindow/*=FALSE*/)
{
	m_bPaintCaption=bPaintCaption;
	m_bOnlyMainWindow=bOnlyMainWindow;

	if(lpszProfileName!=NULL)
	{
		m_sProfileName=lpszProfileName;

		CWinApp* pApp=AfxGetApp();
		ASSERT(pApp!=NULL);

		m_bPaintCaption=pApp->GetProfileInt(m_sProfileName,
			_T("PaintCaption"),m_bPaintCaption);
		m_bOnlyMainWindow=pApp->GetProfileInt(m_sProfileName,
			_T("OnlyMainWindow"),m_bOnlyMainWindow);
	}

	SetupCaptionPainting();

	return TRUE;
}


BOOL COXCustomizeCaptionPage::OnCloseManager(BOOL bIsOk) 
{	
	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL && ::IsWindow(pWnd->GetSafeHwnd()));

	CWinApp* pApp=AfxGetApp();
	ASSERT(pApp!=NULL);
	

	if(bIsOk)
	{
		if(!m_sProfileName.IsEmpty())
		{
			pApp->WriteProfileInt(m_sProfileName,
				_T("PaintCaption"),m_bPaintCaption);
			pApp->WriteProfileInt(m_sProfileName,
				_T("OnlyMainWindow"),m_bOnlyMainWindow);
			
			SetupCaptionPainting();
			m_captionPainterOrganizer.
				SetCaptionInfo(NULL,&m_ActiveInfo,TRUE,TRUE);
			m_captionPainterOrganizer.
				SetCaptionInfo(NULL,&m_PassiveInfo,FALSE,TRUE);
			//SetVars();

			COXCaptionPainter* pCaptionPainter=m_captionPainterOrganizer.
				GetPainter(pWnd);
			if(pCaptionPainter!=NULL)
			{
				pCaptionPainter->SaveState(m_sProfileName); 
			//	pCaptionPainter->LoadState(m_sProfileName);
			}
			//pWnd->PostMessage(WM_NCPAINT, 1);
		}
	}
	else
	{
		if(!m_sProfileName.IsEmpty())
		{
			m_bPaintCaption=pApp->GetProfileInt(m_sProfileName,
				_T("PaintCaption"),m_bPaintCaption);
			m_bOnlyMainWindow=pApp->GetProfileInt(m_sProfileName,
				_T("OnlyMainWindow"),m_bOnlyMainWindow);

			SetupCaptionPainting();
		}
	}

	return TRUE;
}


void COXCustomizeCaptionPage::DoDataExchange(CDataExchange* pDX)
{
	COXCustomizePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXCustomizeCaptionPage)
	DDX_Control(pDX, IDC_OX_BUTTON_SMALLTEXTFONT, m_btnSmallTextFont);
	DDX_Control(pDX, IDC_OX_STATIC_PREVIEW, m_preview);
	DDX_Control(pDX, IDC_OX_EDIT_NUMSHADES, m_ctlEditNumberShade);
	DDX_Control(pDX, IDC_OX_COMBO_TEXT_ELLIPSIS, m_ctlComboTextEllipsis);
	DDX_Control(pDX, IDC_OX_COMBO_TEXT_ALIGNMENT, m_ctlComboTextAlignment);
	DDX_Control(pDX, IDC_OX_COMBO_GRADIENT_ALGORITHM, m_ctlComboGradientAlgorithm);
	DDX_Control(pDX, IDC_OX_COMBO_GRADIENT_ALIGNMENT, m_ctlComboGradientAlignment);
	DDX_Control(pDX, IDC_OX_COMBO_CAPTION_STATE, m_ctlComboCaptionState);
	DDX_Control(pDX, IDC_OX_SPIN_NUMSHADES, m_spinNumberShade);
	DDX_Control(pDX, IDC_OX_SEPARATOR_CAPTION, m_separator);
	DDX_Control(pDX, IDC_OX_BUTTON_TEXTFONT, m_btnTextFont);
	DDX_Control(pDX, IDC_OX_BUTTON_BACKCOLOR, m_btnBackColor);
	DDX_Check(pDX, IDC_OX_CHECK_GRADIENT, m_bGradient);
	DDX_CBIndex(pDX, IDC_OX_COMBO_CAPTION_STATE, m_nCaptionState);
	DDX_CBIndex(pDX, IDC_OX_COMBO_GRADIENT_ALGORITHM, m_nGradientAlgorithm);
	DDX_CBIndex(pDX, IDC_OX_COMBO_GRADIENT_ALIGNMENT, m_nGradientAlignment);
	DDX_CBIndex(pDX, IDC_OX_COMBO_TEXT_ALIGNMENT, m_nTextAlignment);
	DDX_CBIndex(pDX, IDC_OX_COMBO_TEXT_ELLIPSIS, m_nTextEllipsis);
	DDX_Text(pDX, IDC_OX_EDIT_NUMSHADES, m_nNumberShade);
	DDV_MinMaxUInt(pDX, m_nNumberShade, 1, 128);
	DDX_Check(pDX, IDC_OX_CHECK_PAINT_CAPTION, m_bPaintCaption);
	//}}AFX_DATA_MAP
	DDX_ColorPicker(pDX, IDC_OX_BUTTON_BACKCOLOR, m_clrBackground);
	DDX_FontPickerFont(pDX, IDC_OX_BUTTON_TEXTFONT, &m_font);
	DDX_FontPickerColor(pDX, IDC_OX_BUTTON_TEXTFONT, m_clrText);
	DDX_FontPickerFont(pDX, IDC_OX_BUTTON_SMALLTEXTFONT, &m_fontSmall);
	DDX_FontPickerColor(pDX, IDC_OX_BUTTON_SMALLTEXTFONT, m_clrTextSmall);
}


BEGIN_MESSAGE_MAP(COXCustomizeCaptionPage, COXCustomizePage)
	//{{AFX_MSG_MAP(COXCustomizeCaptionPage)
	ON_BN_CLICKED(IDC_OX_BUTTON_BACKCOLOR, OnButtonBackcolor)
	ON_BN_CLICKED(IDC_OX_BUTTON_TEXTFONT, OnButtonTextfont)
	ON_BN_CLICKED(IDC_OX_CHECK_GRADIENT, OnCheckGradient)
	ON_CBN_SELCHANGE(IDC_OX_COMBO_CAPTION_STATE, OnSelchangeComboCaptionState)
	ON_CBN_SELCHANGE(IDC_OX_COMBO_GRADIENT_ALGORITHM, OnSelchangeComboGradientAlgorithm)
	ON_CBN_SELCHANGE(IDC_OX_COMBO_GRADIENT_ALIGNMENT, OnSelchangeComboGradientAlignment)
	ON_CBN_SELCHANGE(IDC_OX_COMBO_TEXT_ALIGNMENT, OnSelchangeComboTextAlignment)
	ON_CBN_SELCHANGE(IDC_OX_COMBO_TEXT_ELLIPSIS, OnSelchangeComboTextEllipsis)
	ON_EN_CHANGE(IDC_OX_EDIT_NUMSHADES, OnChangeEditNumshades)
	ON_BN_CLICKED(IDC_OX_BUTTON_SMALLTEXTFONT, OnButtonSmalltextfont)
	ON_BN_CLICKED(IDC_OX_CHECK_PAINT_CAPTION, OnCheckPaintCaption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeCaptionPage message handlers

BOOL COXCustomizeCaptionPage::Load(const COXCustomizeManager* pCustomizeManager)
{
	// Call default implementation. It will load this demo dialog as the first 
	// page and will create About and CodeSample pages if specified.
	if(!COXCustomizePage::Load(pCustomizeManager))
		return FALSE;

	return TRUE;
}


void COXCustomizeCaptionPage::Unload()
{
	// add here code for cleaning up all objects created by demo
	//
	//
	//////////////////////////////////////////////////////////////////////////

	COXCustomizePage::Unload();
}

void COXCustomizeCaptionPage::OnInitDialog()
{
	// must call default implementation
	COXCustomizePage::OnInitDialog();

	// add here initialization code for your demo dialog. Treat it as a
	// normal CDialog::OnInitDialog function

	m_btnTextFont.SetBuddy(IDC_OX_STATIC_TEXTFONT);
	m_btnSmallTextFont.SetBuddy(IDC_OX_STATIC_SMALLTEXTFONT);

	CString sText;
	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_ACTIVESTATE));
	m_ctlComboCaptionState.AddString(sText);//"Caption in active state"
	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_INACTIVESTATE));
	m_ctlComboCaptionState.AddString(sText);//"Caption in inactive state"

	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_LEFT));
	m_ctlComboGradientAlignment.AddString(sText);//"Left"
	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_CENTER));
	m_ctlComboGradientAlignment.AddString(sText);//"Center"
	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_RIGHT));
	m_ctlComboGradientAlignment.AddString(sText);//"Right"

	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_LINEAR));
	m_ctlComboGradientAlgorithm.AddString(sText);//"Linear"
	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_SQUARE));
	m_ctlComboGradientAlgorithm.AddString(sText);//"Square"

	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_TOPLEFT));
	m_ctlComboTextAlignment.AddString(sText);//"TopLeft"
	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_TOPCENTER));
	m_ctlComboTextAlignment.AddString(sText);//"TopCenter"
	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_TOPRIGHT));
	m_ctlComboTextAlignment.AddString(sText);//"TopRight"
	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_CENTERLEFT));
	m_ctlComboTextAlignment.AddString(sText);//"CenterLeft"
	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_CENTER));
	m_ctlComboTextAlignment.AddString(sText);//"Center"
	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_CENTERRIGHT));
	m_ctlComboTextAlignment.AddString(sText);//"CenterRight"
	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_BOTTOMLEFT));
	m_ctlComboTextAlignment.AddString(sText);//"BottomLeft"
	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_BOTTOMCENTER));
	m_ctlComboTextAlignment.AddString(sText);//"BottomCenter"
	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_BOTTOMRIGHT));
	m_ctlComboTextAlignment.AddString(sText);//"BottomRight"

	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_NONE));
	m_ctlComboTextEllipsis.AddString(sText);//"None"
	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_END));
	m_ctlComboTextEllipsis.AddString(sText);//"End"
	VERIFY(sText.LoadString(IDS_OX_CSMZECAPTION_PATH));
	m_ctlComboTextEllipsis.AddString(sText);//"Path"

	m_nCaptionState=0;

	// spin control
	GetVars();
	m_spinNumberShade.SetRange(1,128);


	// layout
	//
	m_LayoutManager.TieChild(&m_ctlComboCaptionState,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_separator,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);

	m_LayoutManager.TieChild(IDC_OX_STATIC_TEXTFONT,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_STATIC_SMALLTEXTFONT,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);

	m_LayoutManager.TieChild(IDC_OX_GRADIENT_GROUP,OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_ctlComboGradientAlignment,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_ctlComboGradientAlgorithm,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);

	m_LayoutManager.TieChild(IDC_OX_PREVIEW_GROUP,
		OX_LMS_LEFT|OX_LMS_RIGHT|OX_LMS_BOTTOM,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_STATIC_PREVIEW,
		OX_LMS_LEFT|OX_LMS_RIGHT|OX_LMS_BOTTOM,OX_LMT_SAME);
	//
	///////////////////////////////////////
	OnCheckPaintCaption();
	UpdateData();
	GetVars();

	m_bInitialized=TRUE;
}

void COXCustomizeCaptionPage::OnButtonBackcolor() 
{
	// TODO: Add your control notification handler code here
	
	SetVars();
}

void COXCustomizeCaptionPage::OnButtonTextfont() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	if(m_clrTextSmall!=m_clrText)
	{
		m_clrTextSmall=m_clrText;
	}
	UpdateData(FALSE);
	SetVars();
}

void COXCustomizeCaptionPage::OnCheckGradient() 
{
	// TODO: Add your control notification handler code here

	ShowControls();
	SetVars();
}

void COXCustomizeCaptionPage::OnSelchangeComboCaptionState() 
{
	// TODO: Add your control notification handler code here

	if(!UpdateData(TRUE))
		return;
//	SetVars();
	GetVars(FALSE);
//	UpdateData(FALSE);
}

void COXCustomizeCaptionPage::OnSelchangeComboGradientAlgorithm() 
{
	// TODO: Add your control notification handler code here
	
	SetVars();
}

void COXCustomizeCaptionPage::OnSelchangeComboGradientAlignment() 
{
	// TODO: Add your control notification handler code here
	
	SetVars();
}

void COXCustomizeCaptionPage::OnSelchangeComboTextAlignment() 
{
	// TODO: Add your control notification handler code here
	
	SetVars();
}

void COXCustomizeCaptionPage::OnSelchangeComboTextEllipsis() 
{
	// TODO: Add your control notification handler code here
	
	SetVars();
}

void COXCustomizeCaptionPage::OnChangeEditNumshades() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the COXCustomizePage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	if(m_bInitialized)
		SetVars();
}


void COXCustomizeCaptionPage::GetVars(BOOL bLoad)
{
	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL && ::IsWindow(pWnd->GetSafeHwnd()));

	COXCaptionPainter* pCaptionPainter=m_captionPainterOrganizer.
		GetPainter(pWnd);
	BOOL bActive=(m_ctlComboCaptionState.GetCurSel()==NULL)?TRUE:FALSE;
	COXCaptionInfo* pCI=NULL;

	if(bLoad && (pCaptionPainter!=NULL))
	{
		pCI=pCaptionPainter->GetCaptionInfo(TRUE);
		ASSERT(pCI);
		m_ActiveInfo=*pCI;
		pCI=pCaptionPainter->GetCaptionInfo(FALSE);
		ASSERT(pCI);
		m_PassiveInfo=*pCI;
	}
	pCI=bActive?&m_ActiveInfo:&m_PassiveInfo;

	m_bGradient=pCI->GetGradient();
	m_nGradientAlgorithm=pCI->GetGradientAlgorithm();
	m_nGradientAlignment=pCI->GetGradientAlignment();

	UINT nTextFormat=pCI->GetTextFormat();
	if(nTextFormat&DT_END_ELLIPSIS)
		m_nTextEllipsis=1;
	else if(nTextFormat&DT_PATH_ELLIPSIS)
		m_nTextEllipsis=2;
	else
		m_nTextEllipsis=0;

	if(nTextFormat&DT_VCENTER)
	{
		if(nTextFormat&DT_CENTER)
			m_nTextAlignment=4;
		else if(nTextFormat&DT_RIGHT)
			m_nTextAlignment=5;
		else
			m_nTextAlignment=3;
	}
	else if(nTextFormat&DT_BOTTOM)
	{
		if(nTextFormat&DT_CENTER)
			m_nTextAlignment=7;
		else if(nTextFormat&DT_RIGHT)
			m_nTextAlignment=8;
		else
			m_nTextAlignment=6;
	}
	else
	{
		if(nTextFormat&DT_CENTER)
			m_nTextAlignment=1;
		else if(nTextFormat&DT_RIGHT)
			m_nTextAlignment=2;
		else
			m_nTextAlignment=0;
	}

	m_clrBackground=pCI->GetBackgroundColor(m_nCaptionState==0 ? TRUE : FALSE);
	

	m_clrText=pCI->GetTextColor(m_nCaptionState==0 ? TRUE : FALSE);
	
	m_clrTextSmall=m_clrText;

	m_nNumberShade=pCI->GetNumberShade();

	LOGFONT lf;
	VERIFY(pCI->GetCaptionLogFont(&lf));
	if((HFONT)m_font!=NULL)
		m_font.DeleteObject();
	VERIFY(m_font.CreateFontIndirect(&lf));

	VERIFY(pCI->GetSmCaptionLogFont(&lf));
	if((HFONT)m_fontSmall!=NULL)
		m_fontSmall.DeleteObject();
	VERIFY(m_fontSmall.CreateFontIndirect(&lf));

	UpdateData(FALSE);

	ShowControls();
}

void COXCustomizeCaptionPage::SetVars()
{
	if(!UpdateData())
		return;

	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL && ::IsWindow(pWnd->GetSafeHwnd()));

	BOOL bActive=(m_ctlComboCaptionState.GetCurSel()==NULL)?TRUE:FALSE;
	COXCaptionInfo* pCI=NULL;
	
	pCI=bActive?&m_ActiveInfo:&m_PassiveInfo;

	pCI->SetGradient(m_bGradient);
	pCI->SetGradientAlgorithm(m_nGradientAlgorithm==0 ? ID_GRADIENT_LINEAR : 
		ID_GRADIENT_SQUARE);
	pCI->SetGradientAlignment(m_nGradientAlignment==0 ? ID_GRADIENT_LEFT :
		(m_nGradientAlignment==1 ? ID_GRADIENT_CENTER : ID_GRADIENT_RIGHT));

	UINT nTextFormat=DT_SINGLELINE;
	switch(m_nTextEllipsis)
	{
	case 0:
		break;
	case 1:
		nTextFormat|=DT_END_ELLIPSIS;
		break;
	case 2:
		nTextFormat|=DT_PATH_ELLIPSIS;
		break;
	default:
		ASSERT(FALSE);
	}

	switch(m_nTextAlignment)
	{
	case 0:
		nTextFormat|=DT_TOP|DT_LEFT;
		break;
	case 1:
		nTextFormat|=DT_TOP|DT_CENTER;
		break;
	case 2:
		nTextFormat|=DT_TOP|DT_RIGHT;
		break;
	case 3:
		nTextFormat|=DT_VCENTER|DT_LEFT;
		break;
	case 4:
		nTextFormat|=DT_VCENTER|DT_CENTER;
		break;
	case 5:
		nTextFormat|=DT_VCENTER|DT_RIGHT;
		break;
	case 6:
		nTextFormat|=DT_BOTTOM|DT_LEFT;
		break;
	case 7:
		nTextFormat|=DT_BOTTOM|DT_CENTER;
		break;
	case 8:
		nTextFormat|=DT_BOTTOM|DT_RIGHT;
		break;
	}
	pCI->SetTextFormat(nTextFormat);

	pCI->SetBackgroundColor(m_clrBackground);
	pCI->SetTextColor(m_clrText);
	pCI->SetNumberShade(m_nNumberShade);


	LOGFONT lf;
	if(m_font.GetLogFont(&lf))
		pCI->SetCaptionLogFont(&lf);

	if(m_fontSmall.GetLogFont(&lf))
		pCI->SetSmCaptionLogFont(&lf);

	ShowControls();
}

void COXCustomizeCaptionPage::ShowControls() 
{
	if(!UpdateData(TRUE))
		return;

	m_ctlComboGradientAlignment.EnableWindow(m_bGradient & m_bPaintCaption);
	m_ctlComboGradientAlgorithm.EnableWindow(m_bGradient & m_bPaintCaption);
	m_ctlEditNumberShade.EnableWindow(m_bGradient & m_bPaintCaption);

	m_btnBackColor.SetDefaultColor(m_nCaptionState==0 ? 
		RGB(0,0,128) : RGB(128, 128, 128));

	// update Preview Pane
	if(m_bGradient)
	{
		m_preview.SetBackgroundFillType(m_nGradientAlignment==0 ? GRADIENT_LEFT :
			(m_nGradientAlignment==1 ? GRADIENT_CENTER : GRADIENT_RIGHT));
	}
	else
	{
		m_preview.SetBackgroundFillType(NOGRADIENT);
	}

	m_preview.SetBkColor(m_clrBackground);
	/////////////////////////

	UpdateData(FALSE);
}



void COXCustomizeCaptionPage::OnButtonSmalltextfont() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	if(m_clrTextSmall!=m_clrText)
	{
		m_clrText=m_clrTextSmall;
	}
	UpdateData(FALSE);
	SetVars();
}


void COXCustomizeCaptionPage::SetupCaptionPainting()
{
	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL && ::IsWindow(pWnd->GetSafeHwnd()));

	if(m_captionPainterOrganizer.IsAttachedAllInThread())
	{
		VERIFY(m_captionPainterOrganizer.DetachAllInThread());
	}
	else if(m_captionPainterOrganizer.IsAttached(pWnd))
	{
		VERIFY(m_captionPainterOrganizer.Detach(pWnd));
	}

	if(m_bPaintCaption)
	{
		if(m_bOnlyMainWindow)
		{
			VERIFY(m_captionPainterOrganizer.Attach(pWnd));
		}
		else
		{
			VERIFY(m_captionPainterOrganizer.AttachAllInThread());
		}

		if(!m_sProfileName.IsEmpty())
		{
			Sleep(10);
			m_captionPainterOrganizer.LoadState(NULL,m_sProfileName);
		}
	}
}

void COXCustomizeCaptionPage::OnCheckPaintCaption() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData(TRUE))
		return;

	m_btnSmallTextFont.EnableWindow(m_bPaintCaption);
	m_ctlEditNumberShade.EnableWindow(m_bPaintCaption);
	m_ctlComboTextEllipsis.EnableWindow(m_bPaintCaption);
	m_ctlComboTextAlignment.EnableWindow(m_bPaintCaption);
	m_ctlComboGradientAlgorithm.EnableWindow(m_bPaintCaption);
	m_ctlComboGradientAlignment.EnableWindow(m_bPaintCaption);
	m_ctlComboCaptionState.EnableWindow(m_bPaintCaption);
	m_spinNumberShade.EnableWindow(m_bPaintCaption);
	m_btnTextFont.EnableWindow(m_bPaintCaption);
	m_btnBackColor.EnableWindow(m_bPaintCaption);

	GetDlgItem(IDC_OX_CHECK_GRADIENT)->EnableWindow(m_bPaintCaption);

}
