// =============================================================================
// 							Class Implementation : COXContextHelpPropertyPage
// =============================================================================
//
// Source file : 		OXContextHelpPropertyPage.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXContextHelpPropertyPage.h"

#include "UTB64Bit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXContextHelpPropertyPage
IMPLEMENT_DYNAMIC(COXContextHelpPropertyPage,CPropertyPage);

COXContextHelpPropertyPage::COXContextHelpPropertyPage() 
: 	m_bTooltipActive(TRUE)
{
}

COXContextHelpPropertyPage::COXContextHelpPropertyPage(UINT nIDTemplate, UINT nIDCaption)
: CPropertyPage(nIDTemplate, nIDCaption),
m_bTooltipActive(TRUE)
{
}

COXContextHelpPropertyPage::COXContextHelpPropertyPage(LPCTSTR lpszTemplateName, UINT nIDCaption)
: CPropertyPage(lpszTemplateName, nIDCaption),
m_bTooltipActive(TRUE)
{
}

#ifdef _DEBUG
void COXContextHelpPropertyPage::AssertValid() const
{
	CPropertyPage::AssertValid();
}

void COXContextHelpPropertyPage::Dump(CDumpContext& dc) const
{
	CPropertyPage::Dump(dc);
}
#endif //_DEBUG

COXContextHelpPropertyPage::~COXContextHelpPropertyPage()
{
}

BEGIN_MESSAGE_MAP(COXContextHelpPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(COXContextHelpPropertyPage)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_HELP, OnHelp)
	ON_MESSAGE(WM_CONTEXTMENU, OnHelpContextMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXContextHelpPropertyPage message handlers
BOOL COXContextHelpPropertyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	SetupToolTips();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LONG COXContextHelpPropertyPage::OnHelp(UINT, LONG lParam)
{
	ASSERT(AfxGetApp() != NULL);

	DWORD_PTR dwID=(DWORD)(DWORD_PTR)(LPVOID)GetHelpIDs();
	ASSERT(dwID);

	::WinHelp( (HWND)((LPHELPINFO)(LONG_PTR)lParam)->hItemHandle, AfxGetApp()->m_pszHelpFilePath,
		HELP_WM_HELP, dwID);

	return 0;
}

LONG COXContextHelpPropertyPage::OnHelpContextMenu(UINT wParam, LONG)
{
	ASSERT(AfxGetApp() != NULL);
	DWORD_PTR dwID=(DWORD_PTR)(LPVOID)GetHelpIDs();
	ASSERT(dwID);

	::WinHelp((HWND)(UINT_PTR)wParam, AfxGetApp()->m_pszHelpFilePath,
		HELP_CONTEXTMENU, dwID);

	return 0;
}

BOOL COXContextHelpPropertyPage::PreTranslateMessage(MSG* pMsg) 
{
	// Is this already propertypage created ?
	if (m_hWnd != NULL)
	{
		// Is this message meant for this propertypage or for one of his child controls ?
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

	return CPropertyPage::PreTranslateMessage(pMsg);
}

BOOL COXContextHelpPropertyPage::SetTooltipActive(BOOL bActive /* = TRUE */)
{
	m_bTooltipActive = bActive;

	return TRUE;
}

BOOL COXContextHelpPropertyPage::GetTooltipActive()
{
	return m_bTooltipActive;
}

BOOL COXContextHelpPropertyPage::SetupToolTips()
// --- In  : 
// --- Out : 
// --- Returns : Succeeded or not
// --- Effect : Creates and initializes tooltips for this propertypage
{
	// Create/Setup the Tooltip
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

BOOL COXContextHelpPropertyPage::AdjustToolTips() 
// --- In  : 
// --- Out : 
// --- Returns : Succeeded or not
// --- Effect : Adjusts the tooltips for this propertypage
{
	return TRUE;
}

void COXContextHelpPropertyPage::Construct(UINT nIDTemplate, UINT nIDCaption)
{
	CPropertyPage::Construct(nIDTemplate, nIDCaption);
}

void COXContextHelpPropertyPage::Construct(LPCTSTR lpszTemplateName, UINT nIDCaption)
{
	CPropertyPage::Construct(lpszTemplateName, nIDCaption);
}


