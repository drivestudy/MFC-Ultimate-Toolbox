// OXSkinnedTabCtrl.cpp : implementation file
//
// Version: 9.3

#include "stdafx.h"
#include "OXSkinnedTabCtrl.h"
#include "OXSkins.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXSkinnedTabCtrl

IMPLEMENT_DYNAMIC(COXSkinnedTabCtrl, CTabCtrl)

COXSkinnedTabCtrl::COXSkinnedTabCtrl() :
	m_pTabSkin(NULL)
{
}

COXSkinnedTabCtrl::~COXSkinnedTabCtrl()
{
	if (m_pTabSkin != NULL)
		delete m_pTabSkin;
}


BEGIN_MESSAGE_MAP(COXSkinnedTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(COXSkinnedTabCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXSkinnedTabCtrl message handlers

COXTabSkin* COXSkinnedTabCtrl::GetTabSkin()
{
	// Check if the app is derived from COXSkinnedApp
	COXSkinnedApp* pSkinnedApp = DYNAMIC_DOWNCAST(COXSkinnedApp, AfxGetApp());
	if (pSkinnedApp != NULL && pSkinnedApp->GetCurrentSkin() != NULL)
		return pSkinnedApp->GetCurrentSkin()->GetTabSkin();
	else
	{
		// Create a classic skin for this class if not created already
		if (m_pTabSkin == NULL)
			m_pTabSkin = new COXTabSkinClassic();

		return m_pTabSkin;
	}
}

void COXSkinnedTabCtrl::OnPaint() 
{
	GetTabSkin()->OnPaintTabCtrl(this);
}
