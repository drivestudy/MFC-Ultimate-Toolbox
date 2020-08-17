// HyperBarOverlay.cpp : implementation file
//

#include "stdafx.h"
#include "OXHyperBarOverlay.h"
#include "OXHyperBarRender.h"
#pragma warning (push, 3)		// overkill - vc6 warn 4100 in xmemory
#include <algorithm>
#pragma warning (pop)

// COXHyperBarOverlay

IMPLEMENT_DYNAMIC(COXHyperBarOverlay, CWnd)

COXHyperBarOverlay::COXHyperBarOverlay(CToolBar * pParent, COXHyperBarRender * pRender, SIZE szBtn)
{
	m_pParent = pParent;
	m_pOverlay = NULL;
	m_pRender = pRender;
	m_szBtn = szBtn;
}

COXHyperBarOverlay::~COXHyperBarOverlay()
{
	// Delete the overlay bitmap
	// The other pointers are to objects that are cleaned up elsewhere 
	//( m_pRender in the COXHyperbar class that creates instances of this one ).
	if (m_pOverlay)
	{
		m_pOverlay->DeleteObject();
		delete m_pOverlay;
	}
}

BEGIN_MESSAGE_MAP(COXHyperBarOverlay, CWnd)
	//{{AFX_MSG_MAP(COXHyperBar)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAPEND_MESSAGE_MAP()
END_MESSAGE_MAP()

// COXHyperBarOverlay message handlers

void COXHyperBarOverlay::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	POINT ptMouse;
	::GetCursorPos(&ptMouse);

	ScreenToClient(&ptMouse);

	RECT rc;
	GetWindowRect(&rc);

	m_pRender->RenderToolbar(&dc, &rc, m_szBtn, &ptMouse, m_pOverlay);
}

// To eliminate flicker
BOOL COXHyperBarOverlay::OnEraseBkgnd(CDC* /* pDC */)
{
	return FALSE;
}

// The mouse move function works out if the mouse is moving out of the rectangle that  
// defines the parent toolbar, and if it does, closes the window.
void COXHyperBarOverlay::OnMouseMove(UINT /* nFlags */, CPoint point) 
{
	RECT rc;
	m_pParent->GetClientRect(&rc);

	POINT pt = point;

	ClientToScreen(&pt);
	m_pParent->ClientToScreen(&rc);

	if (!PtInRect(&rc, pt))
	{
		ReleaseCapture();
		ShowWindow(SW_HIDE);
		m_pParent->Invalidate();
	}
	else
	{
		Invalidate(FALSE);
	}

}

// m_pOverlay represents a screenshot of the area taken by this window, when the toolbar is dormant
// The toolbar area is drawn over, the rest of the screenshot serves to create the illusion that we are
// drawing outside the bounds of the toolbar.
void COXHyperBarOverlay::SetBitmap(CBitmap * pBmp)
{
	if (m_pOverlay)
	{
		m_pOverlay->DeleteObject();
		delete m_pOverlay;
	}

	m_pOverlay = pBmp;
	Invalidate();
}

// When a click event occurs, we check if the mouse was over a button
// GetActiveItemMsg returns the message that we want to send back to the main window,
// assuming there is one
void COXHyperBarOverlay::OnLButtonUp(UINT /* nFlags */, CPoint /* point */)
{
	int msg = m_pRender->GetActiveItemMsg();

	if (msg > -1)
	{
		ShowWindow(SW_HIDE);
		AfxGetMainWnd()->SendMessage(WM_COMMAND, msg, (LPARAM)m_pParent->m_hWnd);
	}
}

// Ensure that we have mouse capture when our window is shown ( so we get the mouse move message, and don't orphan this window )
BOOL COXHyperBarOverlay::ShowWindow(int nCmdShow)
{
	if (nCmdShow == SW_SHOW )
	{
		SetCapture();
	}
	else
	{
		ReleaseCapture();
	}

	return ((CWnd*)this)->ShowWindow(nCmdShow);
}

