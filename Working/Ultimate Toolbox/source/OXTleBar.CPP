// ==========================================================================
// 					Class Implementation : COXTitleBar
// ==========================================================================

// Source file : oxtlebar.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXTleBar.h"

#include "OXRollup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXTitleBar,CWnd);

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// Data members -------------------------------------------------------------
// protected:
	// CString m_sWndClassTitlebar;
	// --- String containing the name of the window class for the titlebar

	// CSize m_bmSize;
	// --- dimensions of the titlebar

	// CBitmap	m_bitmapAll;
	// --- Bitmap containing all resource on the titlebar
	
	// BOOL	m_bSysmenuOpen;
	// --- system menu open or not
	
	// int m_nRollupState;
	// --- the state the Rollup is in.

	// CFont m_fntStandard;
	// --- Font to draw the caption with
	
	// int m_nActivation;
	// --- active or not
	
	// BOOL	m_bDragging;
	// --- dragging in progress or not

// private:
	
// Member functions ---------------------------------------------------------
// public:

COXTitleBar::COXTitleBar()
   : m_bmSize(0, 0),
   m_bSysmenuOpen(FALSE),
   m_nRollupState(SCROLL_UP),
   m_nActivation(active),
   m_bDragging(FALSE)                   
   {
   m_sWndClassTitlebar = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
	   ::LoadCursor(NULL, IDC_ARROW), NULL, NULL);
   }

COXTitleBar::~COXTitleBar()
   {
   // no cleanup needs to be done
   }

BOOL COXTitleBar::Create(LPCTSTR lpszCaption, CRect& rc, CWnd* pParent, UINT nID)
   {
   ASSERT(!::IsWindow(m_hWnd));
   ASSERT(pParent != NULL && pParent->IsKindOf(RUNTIME_CLASS(COXRollup)));
   ASSERT(lpszCaption != NULL && AfxIsValidString(lpszCaption));
   ASSERT(!rc.IsRectEmpty());
   
   return CWnd::Create(m_sWndClassTitlebar, lpszCaption, WS_CHILD | WS_VISIBLE,
	   rc, pParent, nID);
   }


BEGIN_MESSAGE_MAP(COXTitleBar, CWnd)
   //{{AFX_MSG_MAP(COXTitleBar)
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_MOUSEMOVE()
   ON_WM_LBUTTONDBLCLK()
   ON_WM_PAINT()
   ON_WM_CREATE()
   //}}AFX_MSG_MAP
   END_MESSAGE_MAP()
   
   /////////////////////////////////////////////////////////////////////////////
   // COXTitleBar message handlers
   
BOOL COXTitleBar::LoadBitmap(LPCTSTR lpszBitmap)
   {               
   ASSERT(lpszBitmap != NULL && AfxIsValidString(lpszBitmap));
   
   // delete old bitmap (if present)
   m_bitmapAll.DeleteObject();
   
   if (!m_bitmapAll.LoadBitmap(lpszBitmap))
	   {
	   TRACE0("Failed to load bitmap for Title bar\n");
	   return FALSE;   // need this one image
	   }
   
   // we store information about the bitmap
   BITMAP bmp;
   m_bitmapAll.GetObject(sizeof(BITMAP), &bmp);
   m_bmSize = CSize(bmp.bmWidth, bmp.bmHeight);
   
   return TRUE;
   }

WNDPROC* COXTitleBar::GetSuperWndProcAddr()
   {
   static WNDPROC pfnSuper;
   return &pfnSuper;
   }

void COXTitleBar::DrawBar(CDC* pDC, BOOL bWithCaption)
   {
   ASSERT(pDC != NULL);
   ASSERT(m_bitmapAll.m_hObject != NULL);     // required
   
   CRect crCaption;
   GetClientRect(&crCaption);
   
   if (bWithCaption) // sports some "hinting" if only buttons need redraw
	   {
	   // Fill in the caption color
	   CBrush brCaption;
	   brCaption.CreateSolidBrush(::GetSysColor( ActiveStyle()));
	   pDC->FillRect(&crCaption, &brCaption);
	   
	   CString str;
	   GetWindowText(str);
	   // pDC->SetBkColor(::GetSysColor( ActiveStyle() ));
	   // above function fails with non-standard colors
	   // thanks to Brian for his bug fix here
	   pDC->SetBkMode(TRANSPARENT);
	   pDC->SetTextColor(::GetSysColor(active == ActiveStyle()? COLOR_CAPTIONTEXT : COLOR_INACTIVECAPTIONTEXT));
	   CFont* pOldFnt = pDC->SelectObject(&m_fntStandard);
	   pDC->DrawText(str, str.GetLength(), &crCaption, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	   if (pOldFnt != NULL) 
		   pDC->SelectObject(pOldFnt);
	   }
   
   // now draw the bitmaps...
   CDC memDC;
   int nBmpSymStart;
   int nTileSizeHorz = m_bmSize.cx / 4;
   int nTileSizeVert = m_bmSize.cy;
   
   memDC.CreateCompatibleDC(pDC);
   CBitmap* pOld = memDC.SelectObject(&m_bitmapAll);
   if (pOld == NULL)
	   return;     // destructors will clean up
   
   // system menu; open or not? bitmap selection
   if (!m_bSysmenuOpen) 
	   nBmpSymStart = 0;	// closed sys menu; 1st bitmap
   else
	   nBmpSymStart = nTileSizeHorz; 	// open sys; 2nd bitmap
   pDC->BitBlt(0, 0, nTileSizeHorz, nTileSizeVert, &memDC, nBmpSymStart, 0, SRCCOPY);
   
   // roll up or down; bitmap selection
   if (!IsRolledUp())
	   nBmpSymStart = nTileSizeHorz * 2;	// 3rd bitmap
   else 
	   nBmpSymStart = nTileSizeHorz * 3;	// last bitmap
   pDC->BitBlt(crCaption.right - nTileSizeHorz, 0, nTileSizeHorz, nTileSizeVert,
	   &memDC, nBmpSymStart, 0, SRCCOPY);
   // reselect old bitmap
   memDC.SelectObject(pOld);
   
   // draw black line at bottom only when showing roll up bottom
   if (!IsRolledUp())
	   pDC->PatBlt(crCaption.left, crCaption.bottom - 1, crCaption.Width(), 1, BLACKNESS);
   }

void COXTitleBar::SetActivation(int nActivation)
   {
   ASSERT(nActivation == active || nActivation == inactive);

   if (nActivation == m_nActivation)
	   return;	// do nothing
   
   m_nActivation = nActivation;
   CClientDC dc(this);
   DrawBar(&dc, TRUE);
   }

void COXTitleBar::SetRollupState(int nState)
   {
   ASSERT(SCROLL_UP == nState || SCROLL_DOWN == nState);
   
   if (nState == m_nRollupState)
	   return;	// do nothing
  
   m_nRollupState = nState;
   if (::IsWindow(GetSafeHwnd()))
	   {
	   CClientDC dc(this);
	   DrawBar(&dc, FALSE);
	   }
   }

void COXTitleBar::SetSysMenuOpen(BOOL bOpen)
   {
   if (bOpen == m_bSysmenuOpen)
	   return;	// do nothing
   
   m_bSysmenuOpen = bOpen;
   // security for dblclicks
   if (::IsWindow(GetSafeHwnd()))
	   {
	   CClientDC dc(this);
	   DrawBar(&dc, FALSE);
	   }
   }

void COXTitleBar::OnPaint()
   {
   CPaintDC dc(this);
   DrawBar(&dc, TRUE);
   }


LRESULT	COXTitleBar::HitTest(UINT /* nFlags */, CPoint& point)
   {
   CRect rectCaption;
   GetClientRect(&rectCaption);
   
   ASSERT(m_bitmapAll.m_hObject != NULL);     // we have only tiles when bitmap is valid
   int nTileSizeHorz = m_bmSize.cx / 4;
   
   // start dragging, open sys menu, roll up/down
   if ((rectCaption.right - point.x) < nTileSizeHorz)
	   // clicked in the rollup button
	   return HTSIZE;
   else if (point.x < nTileSizeHorz)
	   // clicked in the sys menu
	   return HTSYSMENU;
   else
	   return HTCAPTION;
   }

void COXTitleBar::OnLButtonDown(UINT nFlags, CPoint point)
   {
   // get parent for notification
   CWnd* pCWnd = GetParent();
   ASSERT(pCWnd != NULL);
   ASSERT(pCWnd->IsKindOf(RUNTIME_CLASS(COXRollup)));
   LRESULT nHitResult;
   
   if (!((COXRollup*)pCWnd)->IsRolling())
	   {
	   nHitResult = HitTest(nFlags,point);
	   switch (nHitResult)
		   {
		   case HTSIZE:	
			   // clicked in the rollup button, notify parent
			   ((COXRollup*)pCWnd)->OnClickedRoll();
			   break;
		   case HTCAPTION:
			   // we start dragging
			   StartDragging();
			   m_ptMouse = point;
			   SetCapture();
			   ClientToScreen(&point);
			   InvertTracker(point);
			   m_ptLast = point;
			   break;
		   case HTSYSMENU:
			   // clicked in the sys menu, notify parent
			   SetSysMenuOpen(TRUE);
			   ((COXRollup*)pCWnd)->OnProcessSysMenu();
			   SetSysMenuOpen(FALSE);
			   break;
		   default:
			   DerivedHitProcessing(nHitResult, nFlags, point);
		   }
	   }
   }

void COXTitleBar::OnLButtonDblClk(UINT nFlags, CPoint point)
   {
   CWnd* pCWnd = GetParent();
   ASSERT(pCWnd != NULL);
   ASSERT(pCWnd->IsKindOf(RUNTIME_CLASS(COXRollup)));

   if (!((COXRollup*)pCWnd)->IsRolling())
	   {
	   if (HitTest(nFlags, point) == HTSYSMENU)
		   {
		   // get parent for notification
		   CWnd* pCWnd = GetParent();
		   ASSERT(pCWnd != NULL);
		   ASSERT(pCWnd->IsKindOf(RUNTIME_CLASS(COXRollup)));
		   // double-clicked in the sys menu, notify parent
		   ((COXRollup*)pCWnd)->OnCloseRollup();
		   }
	   }
   }


void COXTitleBar::OnLButtonUp(UINT /* nFlags */, CPoint point)
   {
   CWnd* pCWnd = GetParent();
   ASSERT(pCWnd != NULL);
   ASSERT(pCWnd->IsKindOf(RUNTIME_CLASS(COXRollup)));
   if (IsDragging() && !((COXRollup*)pCWnd)->IsRolling())
	   {
	   EndDragging();
	   ReleaseCapture();
	   InvertTracker(m_ptLast);
	   
	   CWnd* pCWnd = GetParent();
	   ASSERT(pCWnd != NULL);
	   ASSERT(pCWnd->IsKindOf(RUNTIME_CLASS(COXRollup)));
	   
	   if (m_ptMouse != point)
			{
			ClientToScreen(&point);
			pCWnd->SetWindowPos(NULL, point.x - m_ptMouse.x, point.y - m_ptMouse.y, 0, 0,
				SWP_NOZORDER | SWP_NOSIZE);
	
			((COXRollup*)pCWnd)->UnArrange();	// we are no longer arranged
			}

		// Now make Windows redraw the window.
		pCWnd->SetWindowPos(NULL, 0, 0, 0, 0,
			 SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER |
			 SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		pCWnd->UpdateWindow();

	   }
   }


void COXTitleBar::OnMouseMove(UINT /* nFlags */, CPoint point)
   {
   CWnd* pCWnd = GetParent();
   ASSERT(pCWnd != NULL);
   ASSERT(pCWnd->IsKindOf(RUNTIME_CLASS(COXRollup)));

   if (IsDragging() && !((COXRollup*)pCWnd)->IsRolling())
	   {
	   ClientToScreen(&point);
	   InvertTracker(m_ptLast);
	   m_ptLast = point;
	   InvertTracker(m_ptLast);
	   }
   }

// Intent: Show the moving dialog
//
void COXTitleBar::InvertTracker(CPoint point)
   {
   CRect rectFrame, rectClient;
   GetParent()->GetClientRect(&rectClient);

   CDC dc;
   dc.Attach(::GetDC(NULL));

   
   // topleft to top right
   dc.PatBlt(point.x - m_ptMouse.x - 2, point.y - m_ptMouse.y - 2, rectClient.Width() + 4,
	   2, PATINVERT);
  
   // topright to bottom right
   dc.PatBlt(point.x - m_ptMouse.x + rectClient.Width(), point.y - m_ptMouse.y,
	   2, rectClient.Height(), PATINVERT);

   // topleft to bottom left
   dc.PatBlt(point.x - m_ptMouse.x - 2, point.y - m_ptMouse.y, 2,
	   rectClient.Height(), PATINVERT);
   
   // bottom left to bottom right
   dc.PatBlt(point.x - m_ptMouse.x - 2, point.y - m_ptMouse.y + rectClient.Height(),
	   rectClient.Width() + 4, 2, PATINVERT);


   ::ReleaseDC(NULL, dc.Detach());
   }

int COXTitleBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
   {
   if (CWnd::OnCreate(lpCreateStruct) == -1)
	   return -1;
   
   // we create the font here
   m_fntStandard.DeleteObject();
   CClientDC dc(this);
   VERIFY(m_fntStandard.CreateFont(-MulDiv( 9 , ::GetDeviceCaps(dc.m_hDC, LOGPIXELSY), 72), 
	   0, 0, 0, FW_DONTCARE, FALSE, FALSE, 0,
	   ANSI_CHARSET, OUT_DEFAULT_PRECIS,
	   CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	   DEFAULT_PITCH | FF_SWISS, _T("Arial")));
   
   m_bSysmenuOpen	= FALSE;
   m_nRollupState	= SCROLL_UP;
   
return 0;
}
