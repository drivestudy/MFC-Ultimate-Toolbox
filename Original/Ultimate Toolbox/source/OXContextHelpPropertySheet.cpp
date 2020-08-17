// =============================================================================
// 							Class Implementation : COXContextHelpPropertySheet
// =============================================================================
//
// Source file : 		OXContextHelpPropertySheet.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXContextHelpPropertySheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXContextHelpPropertySheet

IMPLEMENT_DYNAMIC(COXContextHelpPropertySheet, CPropertySheet);

COXContextHelpPropertySheet::COXContextHelpPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage /* = 0 */)
	: CPropertySheet(nIDCaption, pParentWnd, iSelectPage),
	m_bFirstTime(TRUE),
	m_bTooltipActive(TRUE)
	{
	}

COXContextHelpPropertySheet::COXContextHelpPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage /* = 0 */)
	: CPropertySheet(pszCaption, pParentWnd, iSelectPage),
	m_bFirstTime(TRUE),
	m_bTooltipActive(TRUE)
	{
	}

BEGIN_MESSAGE_MAP(COXContextHelpPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(COXContextHelpPropertySheet)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_HELP, OnHelp)
	ON_MESSAGE(WM_CONTEXTMENU, OnHelpContextMenu)
END_MESSAGE_MAP()


#ifdef _DEBUG
void COXContextHelpPropertySheet::AssertValid() const
	{
	CPropertySheet::AssertValid();
	}

void COXContextHelpPropertySheet::Dump(CDumpContext& dc) const
	{
	CPropertySheet::Dump(dc);
	}
#endif //_DEBUG

COXContextHelpPropertySheet::~COXContextHelpPropertySheet()
	{
	}

/////////////////////////////////////////////////////////////////////////////
// COXContextHelpPropertySheet message handlers

BOOL COXContextHelpPropertySheet::PreTranslateMessage(MSG* pMsg) 
	{
	// Is this propertysheet created ?
	ASSERT(m_hWnd != NULL);
	if (m_hWnd != NULL)
		{
		// Is this message meant for this propertysheet or for one of his child controls ?
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

	return CPropertySheet::PreTranslateMessage(pMsg);
	}

LONG COXContextHelpPropertySheet::OnHelp(UINT wParam, LONG lParam)
	{
	GetActivePage()->SendMessage(WM_HELP, wParam, lParam);
	return 0;
	}

LONG COXContextHelpPropertySheet::OnHelpContextMenu(UINT wParam, LONG lParam)
	{
	GetActivePage()->SendMessage(WM_CONTEXTMENU, wParam, lParam);
	return 0;
	}

void COXContextHelpPropertySheet::AddPage(COXContextHelpPropertyPage* pPage)
	{
	CPropertySheet::AddPage((CPropertyPage*)pPage);
	}

BOOL COXContextHelpPropertySheet::SetTooltipActive(BOOL bActive /* = TRUE */)
	{
	m_bTooltipActive = bActive;

	return TRUE;
	}

BOOL COXContextHelpPropertySheet::GetTooltipActive()
	{
	return m_bTooltipActive;
	}

BOOL COXContextHelpPropertySheet::SetupToolTips()
	// --- In  : 
	// --- Out : 
	// --- Returns : Succeeded or not
	// --- Effect : Creates and initializes tooltips for this propertysheet
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

BOOL COXContextHelpPropertySheet::AdjustToolTips() 
	// --- In  : 
	// --- Out : 
	// --- Returns : Succeeded or not
	// --- Effect : Adjusts the tooltips for this propertysheet
	{
	return TRUE;
	}

void COXContextHelpPropertySheet::OnShowWindow(BOOL bShow, UINT nStatus) 
	{
	if (m_bFirstTime && bShow)
		{
		// Do these initializations only once
		m_bFirstTime = FALSE;
		
		//  Because supplying an extended dialog resource to a proertysheet
		// is not yet supported by propertysheet, MFC ASSERTS.  That's why
		// we had to find some other place to fool MFC and tell him to show
		// a question mark.
		ModifyStyleEx(0, WS_EX_CONTEXTHELP);		

		// Because in a propertysheet DoDatExchange is not called automatically,
		// force it here.  We need the DDX't members in SetupTooltips()
		UpdateData(FALSE);
		SetupToolTips();
		}

	CPropertySheet::OnShowWindow(bShow, nStatus);
	}
