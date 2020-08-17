// =============================================================================
// 							Class Implementation : COXSplashWnd
// =============================================================================
//

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                                              
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXSplashWnd.h"

#include <afxtempl.h>
#include "UTB64Bit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(COXSplashWnd, CWnd)
	//{{AFX_MSG_MAP(COXSplashWnd)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// class COXSplashWnd

int COXSplashWnd::m_nDefaultTimeout = 750;

COXSplashWnd* COXSplashWnd::c_pGlobalWnd = NULL;
// --- A pointer to the one global splash window object (may be NULL)

BOOL COXSplashWnd::c_bShowSplashWnd = TRUE;
// --- Whether the global splash window is enabled

// Data members -------------------------------------------------------------
// protected:
	// int m_nTimeout;
	// --- The time out value used to let the splash window disappear automatically

	// BOOL m_bAutoDelete;
	// --- Whether this object should be deleted when the window is destroyed

	// COXSplashWndDIB m_dib;
	// --- The DIB used for this splash window

	// CRgn m_rgn;
	// --- The region of this splash window

	// COLORREF m_crBorder;
	// --- Color of the border

	// BYTE m_crBorderR;
	// --- Red value of the border color (if valid color)
	// BYTE m_crBorderG;
	// --- Green value of the border color (if valid color)
	// BYTE m_crBorderB;
	// --- Blue value of the border color (if valid color)

	// BYTE m_nTolerance;
	// --- Tolerance used to decide whether a color belongs to the border or not
// Member functions ---------------------------------------------------------

COXSplashWnd::COXSplashWnd()
	{
	m_bAutoDelete = FALSE;
	m_bUserCancel = TRUE;
	}

COXSplashWnd::~COXSplashWnd()
	{
	if (c_pGlobalWnd == this) 
		c_pGlobalWnd = NULL;
	}

BOOL COXSplashWnd::LoadGlobalBitmap(LPCTSTR lpszResourceName, 
									COLORREF crBorder/* = CLR_DEFAULT */, 
									LPPOINT pStartPoint /* = NULL */, 
									BYTE nTolerance /* = 0 */)
{
	if(c_pGlobalWnd==NULL)
	{
		c_pGlobalWnd = new COXSplashWnd;
		c_pGlobalWnd->SetAutoDelete();
	}
	return c_pGlobalWnd->LoadBitmap(lpszResourceName, crBorder, 
		pStartPoint, nTolerance);
}


BOOL COXSplashWnd::LoadGlobalBitmapFile(LPCTSTR lpszFileName, 
										COLORREF crBorder/* = CLR_DEFAULT */, 
										LPPOINT pStartPoint /* = NULL */, 
										BYTE nTolerance /* = 0 */)
{
	if(c_pGlobalWnd==NULL)
	{
		c_pGlobalWnd = new COXSplashWnd;
		c_pGlobalWnd->SetAutoDelete();
	}
	return c_pGlobalWnd->LoadBitmapFile(lpszFileName, crBorder, 
		pStartPoint, nTolerance);
}


BOOL COXSplashWnd::LoadBitmap(LPCTSTR lpszResourceName, 
							  COLORREF crBorder /* = CLR_DEFAULT */, 
							  LPPOINT pStartPoint /* = NULL */, 
							  BYTE nTolerance /* = 0 */)
{
	if(!m_dib.LoadResource(lpszResourceName, TRUE))
		return FALSE;

	// (2) determine bitmap's shaping region
	m_crBorder = crBorder;
	m_nTolerance = nTolerance;

	if(m_crBorder==CLR_NONE)
	{
		m_rgn.DeleteObject();
		return TRUE;
	}

	CPoint ptStart(0,0);
	if (pStartPoint != NULL)
		ptStart = *pStartPoint;

	if (m_crBorder == CLR_DEFAULT)
	{
		m_crBorder = m_dib.GetPixel(ptStart);
		if (m_crBorder == CLR_INVALID)
		{
			TRACE0("COXSplashWnd::LoadBitmap(): invalid starting point, (0,0) will be used instead.\r\n");
			m_crBorder = m_dib.GetPixel(0,0);
		}
	}

	if (m_crBorder != CLR_INVALID && m_crBorder != CLR_NONE)
	{
		m_crBorderR = GetRValue(m_crBorder);
		m_crBorderG = GetGValue(m_crBorder);
		m_crBorderB = GetBValue(m_crBorder);
		if (BuildRegion(ptStart))
			return TRUE;
	}

	m_rgn.DeleteObject();
	return FALSE;
}


BOOL COXSplashWnd::LoadBitmapFile(LPCTSTR lpszFileName, 
								  COLORREF crBorder /* = CLR_DEFAULT */, 
								  LPPOINT pStartPoint /* = NULL */, 
								  BYTE nTolerance /* = 0 */)
{
	if(!m_dib.LoadFile(lpszFileName,TRUE))
		return FALSE;

	// (2) determine bitmap's shaping region
	m_crBorder = crBorder;
	m_nTolerance = nTolerance;

	if(m_crBorder==CLR_NONE)
	{
		m_rgn.DeleteObject();
		return TRUE;
	}

	CPoint ptStart(0,0);
	if (pStartPoint != NULL)
		ptStart = *pStartPoint;

	if (m_crBorder == CLR_DEFAULT)
	{
		m_crBorder = m_dib.GetPixel(ptStart);
		if (m_crBorder == CLR_INVALID)
		{
			TRACE0("COXSplashWnd::LoadBitmap(): invalid starting point, (0,0) will be used instead.\r\n");
			m_crBorder = m_dib.GetPixel(0,0);
		}
	}

	if(m_crBorder != CLR_INVALID)
	{
		m_crBorderR = GetRValue(m_crBorder);
		m_crBorderG = GetGValue(m_crBorder);
		m_crBorderB = GetBValue(m_crBorder);
		if (BuildRegion(ptStart))
			return TRUE;
	}

	m_rgn.DeleteObject();
	return FALSE;
}


BOOL COXSplashWnd::Show(int nTimeOut /* = m_nDefaultTimeout */,
	CWnd* pParentWnd /* = NULL */, CPoint* pptDisplayAt /* = NULL */)
{
	if ((c_pGlobalWnd == this) && (!c_bShowSplashWnd))
	{
		TRACE0("COXSplashWnd::Show(): The global splash window was disabled.\r\n");
		return FALSE;
	}

	if (m_dib.GetBitmap() == NULL)
	{
		TRACE0("COXSplashWnd::Show(): Failed: no valid bitmap was loaded.");
		return FALSE;
	}

	m_nTimeout = nTimeOut;
	if (m_nTimeout <= 0 && m_nTimeout != SPLASH_NOTIMER)
	{
		TRACE1("COXSplashWnd::Show(): Invalid timeout value has been corrected to %d.\r\n", m_nDefaultTimeout);
		m_nTimeout = m_nDefaultTimeout;
	}

	// destroy it if already created
	if (m_hWnd != NULL && ::IsWindow(m_hWnd))
		OnTimer(OXSPLASHWND_TIMERID_HIDE);

	CWnd* pTempParent = pParentWnd;
	DWORD dwExtraStyle(0);
	if (pTempParent == NULL)
	{
		if(!m_invisParent.CreateEx(WS_EX_TOPMOST, AfxRegisterWndClass(CS_CLASSDC),
			_T(""), WS_POPUP, 0, 0, 1, 1, NULL, NULL))
		{
			TRACE0("COXSplashWnd::Show(): fail to create invisible parent window. Using WS_EX_TOOLWINDOW\r\n");
			dwExtraStyle = WS_EX_TOOLWINDOW;
		}

		pTempParent = &m_invisParent;
	}

	BITMAP bm;
	m_dib.GetBitmapInfo(bm);
	if (!CreateEx(WS_EX_TOPMOST | dwExtraStyle,
		AfxRegisterWndClass(CS_CLASSDC, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		_T(""), WS_POPUP | WS_VISIBLE, 0, 0, bm.bmWidth, bm.bmHeight, 
		(pTempParent != NULL ? pTempParent->GetSafeHwnd() : NULL), NULL))
	{
		TRACE0("COXSplashWnd::Show(): fail to create splash window.\r\n");
		if (m_bAutoDelete) 
			delete this;
		
		if(pTempParent==&m_invisParent)
			m_invisParent.DestroyWindow();

		return FALSE;
	}

	SetFocus();
	if(pptDisplayAt==NULL)
		UpdateWindow();
	else
		MoveWindow(pptDisplayAt->x,pptDisplayAt->y,bm.bmWidth,bm.bmHeight);

	return TRUE;
}

BOOL COXSplashWnd::Hide(int nTimeOut /* = 0 */)
	{
	if (nTimeOut <= 0)
		{
		// destroy splash
		if (m_hWnd != NULL && ::IsWindow(m_hWnd))
			OnTimer(OXSPLASHWND_TIMERID_HIDE);

		m_nTimeout = 0;
		return TRUE;
		}

	m_nTimeout = nTimeOut;
	return (BOOL)SetTimer(OXSPLASHWND_TIMERID_HIDE, m_nTimeout, NULL);
	}


BOOL COXSplashWnd::SetRegion (CRgn& rgn)
	{
	m_rgn.DeleteObject();
	m_rgn.CreateRectRgn(0,0,1,1);
	int nResult = m_rgn.CopyRgn(&rgn);

#ifdef _DEBUG
	if (nResult == NULLREGION)
		TRACE0("COXSplashWnd::SetRegion(): an empty region was set.\r\n");
#endif

	if (nResult == ERROR)
		{
		TRACE0("COXSplashWnd::SetRegion(): failed to set region.\r\n");
		m_rgn.DeleteObject();
		return FALSE;
		}

	return TRUE;
	}

// protected:
BOOL COXSplashWnd::BuildRegion(POINT ptStart)
	// --- In      : ptStart, the starting point to search for region boundary
	// --- Out     :
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : calculate the region to be visible in a bitmap
{
	CArray<CPoint, CPoint&> vertexs;
	CPoint ptMax(0,0);
	CPoint pt0(0,0);
	CPoint pt1(0,0);
	CPoint pt(0,0);
	CPoint ptPrev(0,0);
	CSize sizeInc(0,0);
	int i = 0;
	int iFirst = 0;
	int iLast = 0;
	int iPrev = 0;

	// move directions: index (xInc, yInc) alternate_index
	//        0 (-1,-1) 8    1 ( 0,-1) 9    2 ( 1,-1) 10
	//		  7 (-1, 0)	15		            3 ( 1, 0) 11
	//		  6 (-1, 1) 14   5 ( 0, 1) 13   4 ( 1, 1) 12
	const int xd[] = {-1,0,1,1,1,0,-1,-1,-1,0,1,1,1,0,-1,-1};
	const int yd[] = {-1,-1,-1,0,1,1,1,0,-1,-1,-1,0,1,1,1,0};
	const int nextdir[] = {6,0,0,2,2,4,4,6,6,0,0,2,2,4,4,6};

	BITMAP bm;
	m_dib.GetBitmapInfo(bm);
	ptMax = CPoint(bm.bmWidth - 1, bm.bmHeight - 1);

	// find a start point that's outside the region
	UNREFERENCED_PARAMETER(ptStart);
	pt0 = CPoint(0,0);
	if (!IsBorder(pt0)) 
		pt0 = CPoint(-1,-1);

	// search diagonally for first point that's within the region
	sizeInc.cx = (pt0.x > (ptMax.x / 2)) ? -1 : 1;
	sizeInc.cy = (pt0.y > (ptMax.y / 2)) ? -1 : 1;
	for (pt1 = pt0 + sizeInc; IsBorder(pt1, FALSE); pt1 += sizeInc)
		;
	pt0 = pt1 - sizeInc;
	
	// if not found after hitting the boundary, search by scan lines
	if (m_dib.GetPixel(pt1) == CLR_INVALID)
	{
		for (pt1.y = 0; pt1.y <= ptMax.y; pt1.y++)
		{
			for (pt1.x = 0; pt1.x <= ptMax.x && IsBorder(pt1); pt1.x++)
				;
			if (pt1.x <= ptMax.x) 
				break;
		}
		if (ptMax.y < pt1.y) 
			return FALSE;
		pt0 = pt1 - CSize(0, 1);
	}

	// now pt1 should be the starting point that's within the region
	// and pt0 should be a neigboring point that's outside the region
	ASSERT(IsBorder(pt0) && !IsBorder(pt1));

	// clockwise find border/region boundary
	for (i = 0; i <= 7; i++)
	{
		pt = pt1 + CSize(xd[i], yd[i]);
		if (!IsBorder(pt)) 
			break;
	}
	if (i == 8) 
		return FALSE;

	// important: assign second point as the start point to prevent
	//			  diagonal circumvent
	pt1 = pt;
	vertexs.Add(pt);
	
	// cycle until the most beginning point is found back again
	do 
	{
		iPrev = i;
		ptPrev = pt;
		iFirst = nextdir[i];
		iLast = iFirst + 6;
		for (i = iFirst; i <= iLast; i++)
		{
			pt = ptPrev + CSize(xd[i], yd[i]);
			if (!IsBorder(pt)) 
				break;
		}
		if (i == iPrev)
		{
			// moving forward on the same direction
			// replace the last point with this new one, so that region
			// definition could be simpler
			vertexs[vertexs.GetSize()-1] = pt;
		}
		else
		{
			// direction changed, has to add a new vertex
			vertexs.Add(pt);
		}
		if(vertexs.GetSize()%3000==0)
			TRACE(_T("Add point: %d,%d, number %d\n"),pt.x,pt.y,vertexs.GetSize());
	} while (pt != pt1);

	m_rgn.DeleteObject();
	if (!m_rgn.CreatePolygonRgn(vertexs.GetData(), PtrToInt(vertexs.GetSize()), ALTERNATE))
	{
		ASSERT((HRGN)m_rgn == NULL);
		return FALSE;
	}

	return TRUE;
}

BOOL COXSplashWnd::IsBorder(POINT pt, BOOL bConsiderInvalidAsBorder /* = TRUE */)
	// --- In      : pt, the point to examine
	// --- Out     :
	// --- Returns : TRUE if point is in border; FALSE if point is in visible region
	// --- Effect  : determine whether a point should be part of the border
	{
	COLORREF cr;
	
	cr = m_dib.GetPixel(pt);
	if (cr == CLR_INVALID) 
		return bConsiderInvalidAsBorder;

	if (m_nTolerance < abs(GetRValue(cr) - m_crBorderR)) 
		return FALSE;
	if (m_nTolerance < abs(GetGValue(cr) - m_crBorderG)) 
		return FALSE;
	if (m_nTolerance < abs(GetBValue(cr) - m_crBorderB)) 
		return FALSE;
	
	return TRUE;
	}

void COXSplashWnd::PostNcDestroy() 
	{
	if (m_bAutoDelete)
		delete this;
	else
		CWnd::PostNcDestroy();
	}

void COXSplashWnd::OnTimer(UINT nIDEvent) 
	{
	if (nIDEvent == OXSPLASHWND_TIMERID_HIDE)
		{
		ShowWindow(SW_HIDE);
		KillTimer(nIDEvent);
		ReleaseCapture();
		// If we made our own invisible parent, let's destroy it and
		// it will take care of destroying us
		if (::IsWindow(m_invisParent.m_hWnd))
//			m_invisParent.DestroyWindow();
			::DestroyWindow(m_invisParent.GetSafeHwnd());
		else
			// We didn't provide our own parent, so we must destroy ourselves
//			DestroyWindow();
			::DestroyWindow(GetSafeHwnd());
		}
	}

int COXSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
	{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if ((HRGN)m_rgn != NULL)
		{
		CRgn rgn;
		rgn.CreateRectRgn(0,0,1,1);
		switch(rgn.CopyRgn(&m_rgn))
			{
			case ERROR:
				TRACE0("COXSplashWnd::OnCreate(): fail to load the calculated region.\r\n");
				rgn.DeleteObject();
				break;
			case NULLREGION:
				TRACE0("COXSplashWnd::OnCreate(): empty region was encountered.\r\n");
				rgn.DeleteObject();
				break;
			default:
				::SetWindowRgn(m_hWnd, (HRGN)rgn.Detach(), TRUE);
			}
		}
	
	CenterWindow();
	if (m_nTimeout != SPLASH_NOTIMER)
		SetTimer(OXSPLASHWND_TIMERID_HIDE, m_nTimeout, NULL);

	SetCapture();
	::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	
	return 0;
	}

void COXSplashWnd::OnPaint() 
{
	CPaintDC dc(this);
	m_dib.Draw(&dc);
}

BOOL COXSplashWnd::PreTranslateMessage(MSG* pMsg)
{
	if(m_bUserCancel)
	{
		if((pMsg->message>=WM_MOUSEFIRST && pMsg->message<=WM_MOUSELAST && 
			pMsg->message!=WM_MOUSEMOVE) || (pMsg->message>=WM_KEYFIRST && 
			pMsg->message<=WM_KEYLAST))
		{
			OnTimer(OXSPLASHWND_TIMERID_HIDE);
			return TRUE;
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

// end of OXSplashWnd.cpp
