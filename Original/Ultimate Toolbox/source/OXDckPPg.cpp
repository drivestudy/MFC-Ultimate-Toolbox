// ==========================================================================
// 							Class Implementation : COXDockPropertyPage
// ==========================================================================

// Source file : OXDckPPg.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "oxdckppg.h"	// class specification

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Definition of static members


// Data members -------------------------------------------------------------
// protected:

//	CString		m_sCaption;
//	---			The title of this property-page

//	UINT 		m_nID;
//	---			The ID of this prperty-page

//  BOOL		m_bDynamic
//	---			TRUE if this page was created using new rather than as a fixed mamber
//				in a sheet class

// private:

// Member functions ---------------------------------------------------------
// public:

IMPLEMENT_DYNAMIC(COXDockPropertyPage, CPropertyPage)

COXDockPropertyPage::COXDockPropertyPage(LPCTSTR pszCaption /*= NULL*/, BOOL bDynamic /* =FALSE */)
	: CPropertyPage(),
	m_bDynamic(bDynamic)
	{
	if (pszCaption != NULL)
		m_sCaption = pszCaption;
	}

COXDockPropertyPage::COXDockPropertyPage(UINT nIDCaption, BOOL bDynamic /* =FALSE */)
	: CPropertyPage(),
	m_bDynamic(bDynamic)
	{
	m_sCaption.LoadString(nIDCaption);
	}

BOOL COXDockPropertyPage::OnSetActive()
	{
	return CPropertyPage::OnSetActive();
	}

BOOL COXDockPropertyPage::OnKillActive()
	{
	return CPropertyPage::OnKillActive();
	}

BOOL COXDockPropertyPage::PreTranslateMessage(MSG* pMsg)
	{
	if (CDialog::PreTranslateMessage(pMsg))
		{	
		if (pMsg->message == WM_KEYDOWN && GetAsyncKeyState(VK_CONTROL) < 0 &&
			(pMsg->wParam == VK_TAB || pMsg->wParam == VK_PRIOR || pMsg->wParam == VK_NEXT))
			return CPropertyPage::PreTranslateMessage(pMsg);
		else
			return TRUE;
		}

	return CPropertyPage::PreTranslateMessage(pMsg);
	}
