// UTSampleAbout.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <afxcmn.h>
#include <direct.h>

#include "UTSampleAbout.h"
#include "UTBStrOp.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static CString g_sDescriptionFile;
CString g_appPath;
BOOL g_bNewFile=TRUE;

//	static UINT g_nCallbackMsgID=RegisterWindowMessage(_T("HyperLinkCallbackMessage"));

/////////////////////////////////////////////////////////////////////////////
// CUTSampleAboutDlg dialog used for App About

CUTSampleAboutDlg::CUTSampleAboutDlg() : CDialog(CUTSampleAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CUTSampleAboutDlg)
	m_sTitle = _T("");
	//}}AFX_DATA_INIT
	g_sDescriptionFile = _T("");
}

CUTSampleAboutDlg::CUTSampleAboutDlg(CString sTitle, CString sDescriptionFile) : 
	CDialog(CUTSampleAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CUTSampleAboutDlg)
	m_sTitle = sTitle;
	//}}AFX_DATA_INIT
	g_sDescriptionFile = sDescriptionFile;
}

CUTSampleAboutDlg::CUTSampleAboutDlg(UINT nTitleID, UINT nDescriptionFileID) : 
	CDialog(CUTSampleAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CUTSampleAboutDlg)
	m_sTitle.LoadString(nTitleID);
	//}}AFX_DATA_INIT
	g_sDescriptionFile.LoadString(nDescriptionFileID);
}

void CUTSampleAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUTSampleAboutDlg)
	DDX_Text(pDX, IDC_SAMPLE_TITLE, m_sTitle);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUTSampleAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CUTSampleAboutDlg)
	//}}AFX_MSG_MAP
//	ON_REGISTERED_MESSAGE(g_nCallbackMsgID,OnFaxActivated)
END_MESSAGE_MAP()


BOOL CUTSampleAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString sCaption;
	sCaption.Format(_T("About %s"),m_sTitle);

	SetWindowText(sCaption);

	//get the application path
	TCHAR buf[_MAX_PATH];
	UTBStr::tcscpy(buf, _MAX_PATH, GetCommandLine());
	//remove the program name
	int len = (int)_tcslen(buf);
	int loop = 0;
	for(loop = len;loop > 0;loop --)
	{
		if(buf[loop] == _T('\\'))
		{
			buf[loop+1] = 0;
			break;
		}
	}
	if(loop == 0)
	{
		_tgetcwd(buf, sizeof(buf));
		UTBStr::tcscat(buf, _MAX_PATH, _T("\\"));
	}
	if(buf[0] == _T('\"'))
		g_appPath = &buf[1];
	else
		g_appPath = buf;

	VERIFY(m_page.CreateFromStatic(IDC_ABOUTSTATIC, this));
	m_page.Navigate2(_T("file://") + g_appPath + g_sDescriptionFile);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}