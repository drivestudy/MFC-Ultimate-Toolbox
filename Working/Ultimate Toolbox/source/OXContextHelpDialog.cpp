// =============================================================================
// 							Class Implementation : COXContextHelpDialog
// =============================================================================
//
// Source file : 		OXContextHelpDialog.cpp
// Version: 9.3
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXContextHelpDialog.h"
#include "UTB64Bit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OXContextHelpDialog
IMPLEMENT_DYNAMIC(COXContextHelpDialog, CDialog)

COXContextHelpDialog::COXContextHelpDialog()
	: m_bTooltipActive(TRUE)
	{
	}

COXContextHelpDialog::COXContextHelpDialog(UINT nIDTemplate, CWnd* pParentWnd /* = NULL */)
	: CDialog(nIDTemplate, pParentWnd),
	  m_bTooltipActive(TRUE)
	{
	}

COXContextHelpDialog::COXContextHelpDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd /* = NULL */)
	: CDialog(lpszTemplateName, pParentWnd),
	  m_bTooltipActive(TRUE)
	{
	}

COXContextHelpDialog::~COXContextHelpDialog()
	{
	}

#ifdef _DEBUG
void COXContextHelpDialog::AssertValid() const
	{
	CDialog::AssertValid();
	}

void COXContextHelpDialog::Dump(CDumpContext& dc) const
	{
	CDialog::Dump(dc);
	}
#endif //_DEBUG

BOOL COXContextHelpDialog::SetTooltipActive(BOOL bActive /* = TRUE */)
	{
	m_bTooltipActive = bActive;

	return TRUE;
	}

BOOL COXContextHelpDialog::GetTooltipActive()
	{
	return m_bTooltipActive;
	}

BEGIN_MESSAGE_MAP(COXContextHelpDialog, CDialog)
	//{{AFX_MSG_MAP(COXContextHelpDialog)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_HELP, OnHelp)
	ON_MESSAGE(WM_CONTEXTMENU, OnHelpContextMenu)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COXContextHelpDialog message handlers

BOOL COXContextHelpDialog::PreTranslateMessage(MSG* pMsg) 
	{
	// Is this dialog created ?
	if (m_hWnd != NULL)
		{
		// Is this message meant for this dialog or for one of his child controls ?
		if (pMsg->hwnd == m_hWnd || ::IsChild(m_hWnd, pMsg->hwnd))
			{
			// Is this tooltip already created && do we really want tooltips ?
			if (m_Tooltip.m_hWnd != NULL && m_bTooltipActive)
				{
				// PSS ID Number: Q143313 for VC++ 4.0 & 4.1
				// After a modal dilaog has been popped up, MFC disables the tooltips
				// but does not activated them again, so activate explicitely every time
				m_Tooltip.Activate(TRUE);
				m_Tooltip.RelayEvent(pMsg) ;
				}
			}
		}
	
	return CDialog::PreTranslateMessage(pMsg);
	}

LONG COXContextHelpDialog::OnHelp(UINT_PTR, LONG_PTR lParam)
	{
	ASSERT(AfxGetApp() != NULL);
	DWORD_PTR dwID=(DWORD_PTR)(LPVOID)GetHelpIDs();
	ASSERT(dwID);

	::WinHelp( (HWND)((LPHELPINFO)lParam)->hItemHandle, AfxGetApp()->m_pszHelpFilePath,
		HELP_WM_HELP, dwID);

	return 0;
	}

LONG COXContextHelpDialog::OnHelpContextMenu(UINT_PTR wParam, LONG_PTR)
	{
	ASSERT(AfxGetApp() != NULL);
	DWORD_PTR dwID=(DWORD_PTR)(LPVOID)GetHelpIDs();
	ASSERT(dwID);

	::WinHelp((HWND)wParam, AfxGetApp()->m_pszHelpFilePath,
		HELP_CONTEXTMENU, dwID);

	return 0;
	}

BOOL COXContextHelpDialog::OnInitDialog() 
	{
	CDialog::OnInitDialog();
	
	// Deriving from this class only makes sense if the extended style WS_EX_CONTEXTHELP
	// was used to create this dialog
	ASSERT((::GetWindowLongPtr(m_hWnd, GWL_EXSTYLE) & WS_EX_CONTEXTHELP) ==  WS_EX_CONTEXTHELP);
	
	VERIFY(SetupToolTips());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
	}

BOOL COXContextHelpDialog::SetupToolTips()
	// --- In  : 
	// --- Out : 
	// --- Returns : Succeeded or not
	// --- Effect : Creates and initializes tooltips for this dialog
	{
	// Create/Setup Tooltips
	BOOL bSuccess = m_Tooltip.Create(this);
	ASSERT(bSuccess);
	if (bSuccess)
		{
		if (m_bTooltipActive)
			m_Tooltip.Activate(TRUE);
		
		AdjustToolTips();
		}

	return bSuccess;
	}

BOOL COXContextHelpDialog::AdjustToolTips() 
	// --- In  : 
	// --- Out : 
	// --- Returns : Succeeded or not
	// --- Effect : Adjusts the tooltips for this dialog
	{
	return TRUE;
	}

