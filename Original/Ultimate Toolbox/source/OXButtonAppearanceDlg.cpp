// OXButtonAppearanceDlg.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXMainRes.h"
#include "OXButtonAppearanceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXButtonAppearanceDlg dialog


COXButtonAppearanceDlg::COXButtonAppearanceDlg(CWnd* pParent /*=NULL*/)
	:	CDialog(COXButtonAppearanceDlg::IDD, pParent),
		m_pImageList(NULL),
		m_bOnlyText(FALSE),
		m_bStyleReadOnly(FALSE),
		m_bShortkeyReadOnly(TRUE),
		m_bCustomButton(FALSE)
{
	//{{AFX_DATA_INIT(COXButtonAppearanceDlg)
	m_sText = _T("");
	m_nCustomImageIndex = -1;
	m_nButtonType = -1;
	m_bDropDown = FALSE;
	m_nShortkeyIndex = 0;
	m_bDisplayHorzDock = FALSE;
	m_bDisplayVertDock = FALSE;
	m_bDisplayFloat = FALSE;
	m_nHeight = 0;
	m_nWidth = 0;
	//}}AFX_DATA_INIT
	m_arrShortkeyEntries.Add(_T("None"));
}


void COXButtonAppearanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXButtonAppearanceDlg)
	DDX_Control(pDX, IDC_OX_STATIC_TEXT, m_sepText);
	DDX_Control(pDX, IDC_OX_STATIC_IMAGES, m_sepImages);
	DDX_Control(pDX, IDC_OX_STATIC_SIZES, m_sepSizes);
	DDX_Control(pDX, IDC_OX_STATIC_DISPLAY_SETTINGS, m_sepDisplaySettings);
	DDX_Control(pDX, IDC_OX_LISTBOX_CUSTOM_IMAGES, m_listCustomImages);
	DDX_Text(pDX, IDC_OX_EDIT_TEXT, m_sText);
	DDX_LBIndex(pDX, IDC_OX_LISTBOX_CUSTOM_IMAGES, m_nCustomImageIndex);
	DDX_Radio(pDX, IDC_OX_RADIO_IMAGE_ONLY, m_nButtonType);
	DDX_Check(pDX, IDC_OX_CHECK_DROPDOWN, m_bDropDown);
	DDX_Control(pDX, IDC_OX_COMBO_SHORTKEY, m_comboShortkeys);
	DDX_CBIndex(pDX, IDC_OX_COMBO_SHORTKEY, m_nShortkeyIndex);
	DDX_Text(pDX, IDC_OX_EDIT_BUTTON_HEIGHT, m_nHeight);
	DDX_Text(pDX, IDC_OX_EDIT_BUTTON_WIDTH, m_nWidth);
	DDX_Check(pDX, IDC_OX_CHECK_SHOW_HORZDOCK, m_bDisplayHorzDock);
	DDX_Check(pDX, IDC_OX_CHECK_SHOW_VERTDOCK, m_bDisplayVertDock);
	DDX_Check(pDX, IDC_OX_CHECK_SHOW_FLOAT, m_bDisplayFloat);
	DDX_Control(pDX, IDC_OX_SPIN_BUTTON_HEIGHT, m_spinHeight);
	DDX_Control(pDX, IDC_OX_SPIN_BUTTON_WIDTH, m_spinWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COXButtonAppearanceDlg, CDialog)
	//{{AFX_MSG_MAP(COXButtonAppearanceDlg)
	ON_BN_CLICKED(IDC_OX_RADIO_IMAGE_ONLY, OnRadioImageOnly)
	ON_BN_CLICKED(IDC_OX_RADIO_IMAGE_AND_TEXT, OnRadioImageAndText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXButtonAppearanceDlg message handlers

BOOL COXButtonAppearanceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_listCustomImages.SetImageList(m_pImageList);
	m_listCustomImages.SetBkColor(::GetSysColor(COLOR_BTNFACE));

	for(int nIndex=0; nIndex<m_arrShortkeyEntries.GetSize(); nIndex++)
	{
		m_comboShortkeys.AddString(m_arrShortkeyEntries[nIndex]);
	}

	UpdateData(FALSE);

	m_spinHeight.SetRange(0,1000);
	m_spinWidth.SetRange(0,1000);

	ShowControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COXButtonAppearanceDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}


void COXButtonAppearanceDlg::ShowControls()
{
	if(!UpdateData(TRUE))
		return;

	GetDlgItem(IDC_OX_EDIT_TEXT)->
		EnableWindow(!m_bCustomButton && (m_nButtonType!=0 || m_bOnlyText));
	GetDlgItem(IDC_OX_RADIO_IMAGE_ONLY)->EnableWindow(!m_bOnlyText && !m_bCustomButton);
	GetDlgItem(IDC_OX_RADIO_IMAGE_AND_TEXT)->
		EnableWindow(!m_bOnlyText && !m_bCustomButton);
	GetDlgItem(IDC_OX_LISTBOX_CUSTOM_IMAGES)->
		EnableWindow(!m_bOnlyText || m_bCustomButton);
	GetDlgItem(IDC_OX_CHECK_DROPDOWN)->
		EnableWindow(!m_bStyleReadOnly && !m_bCustomButton);
	GetDlgItem(IDC_OX_COMBO_SHORTKEY)->
		EnableWindow(!m_bShortkeyReadOnly && !m_bCustomButton);
	GetDlgItem(IDC_OX_EDIT_BUTTON_HEIGHT)->EnableWindow(m_bCustomButton);
	GetDlgItem(IDC_OX_SPIN_BUTTON_HEIGHT)->EnableWindow(m_bCustomButton);
	GetDlgItem(IDC_OX_EDIT_BUTTON_WIDTH)->EnableWindow(m_bCustomButton);
	GetDlgItem(IDC_OX_SPIN_BUTTON_WIDTH)->EnableWindow(m_bCustomButton);
	GetDlgItem(IDC_OX_CHECK_SHOW_HORZDOCK)->EnableWindow(m_bCustomButton);
	GetDlgItem(IDC_OX_CHECK_SHOW_VERTDOCK)->EnableWindow(m_bCustomButton);
	GetDlgItem(IDC_OX_CHECK_SHOW_FLOAT)->EnableWindow(m_bCustomButton);
}

void COXButtonAppearanceDlg::OnRadioImageOnly() 
{
	// TODO: Add your control notification handler code here
	ShowControls();
}

void COXButtonAppearanceDlg::OnRadioImageAndText() 
{
	// TODO: Add your control notification handler code here
	ShowControls();
}
