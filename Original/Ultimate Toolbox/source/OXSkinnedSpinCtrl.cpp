// OXSkinnedSpinCtrl.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXSkinnedSpinCtrl.h"
#include "OXSkins.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(COXSkinnedSpinCtrl, CSpinButtonCtrl)
/////////////////////////////////////////////////////////////////////////////
// COXSkinnedSpinCtrl
HHOOK COXSkinnedSpinCtrl::m_hMouseHook = NULL;

HWND COXSkinnedSpinCtrl::m_hwndPrevMouseMoveWnd = NULL;

COXSkinnedSpinCtrl::COXSkinnedSpinCtrl() :
	m_pTabSkin(NULL)
{
}

COXSkinnedSpinCtrl::~COXSkinnedSpinCtrl()
{
	if ( m_pTabSkin != NULL )
		delete m_pTabSkin;
}


BEGIN_MESSAGE_MAP(COXSkinnedSpinCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(COXSkinnedSpinCtrl)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXSkinnedSpinCtrl message handlers

COXTabSkin* COXSkinnedSpinCtrl::GetTabSkin()
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

void COXSkinnedSpinCtrl::OnPaint() 
{
	GetTabSkin()->OnPaintSpinCtrl(this);
}


// Update the buttons of the mini frame window when the mouse leaves
LRESULT CALLBACK COXSkinnedSpinCtrl::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return ::CallNextHookEx(m_hMouseHook, nCode, wParam, lParam);

	if (nCode == HC_ACTION && (wParam == WM_MOUSEMOVE || wParam == WM_NCMOUSEMOVE))
	{

		MOUSEHOOKSTRUCT* pMH = (MOUSEHOOKSTRUCT*) lParam;

		// If the previous message was for COXSizableMiniDockFrameWnd and the current is not
		// we need to update the caption buttons
		COXSkinnedSpinCtrl* pPrev = DYNAMIC_DOWNCAST(COXSkinnedSpinCtrl,CWnd::FromHandlePermanent(m_hwndPrevMouseMoveWnd));
		COXSkinnedSpinCtrl* pCurrent = DYNAMIC_DOWNCAST(COXSkinnedSpinCtrl,CWnd::FromHandlePermanent(pMH->hwnd));

		if (pPrev != NULL && pCurrent != pPrev)
		{
			// The mouse just left the mini frame window
			::SendMessage( m_hwndPrevMouseMoveWnd, WM_MOUSEMOVE, 0, 0 );
		}
		m_hwndPrevMouseMoveWnd = pMH->hwnd;
	}

	return CallNextHookEx(m_hMouseHook, nCode, wParam, lParam);
}

void COXSkinnedSpinCtrl::PreSubclassWindow()
{
	// Hook the mouse
	if (m_hMouseHook == NULL)
		m_hMouseHook = ::SetWindowsHookEx(WH_MOUSE, MouseProc, 0, AfxGetApp()->m_nThreadID);

	CSpinButtonCtrl::PreSubclassWindow();
}

void COXSkinnedSpinCtrl::OnDestroy()
{
	// Unhook the mouse
	if (m_hMouseHook)
	{
		::UnhookWindowsHookEx(m_hMouseHook);
		m_hMouseHook = NULL;
	}

	CSpinButtonCtrl::OnDestroy();
}

void COXSkinnedSpinCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	Invalidate();
	
	CSpinButtonCtrl::OnMouseMove(nFlags, point);
}

BOOL COXSkinnedSpinCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}
