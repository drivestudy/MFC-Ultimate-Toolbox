// ========================================================================================
// 					Class Implementation : COXSizeControlBar
// ========================================================================================

// Source file : OXSizeCtrlBar.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
// Some portions Copyright (C)1994-5	Micro Focus Inc, 2465 East Bayshore Rd, Palo Alto, CA 94303.
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXSizeCtrlBar.h"

#include "OXMainRes.h"  // for some resource strings
#include "OXDragDockContext.h"
#include "OXFrameWndDock.h"
#include "OXMDIFloatWnd.h"
#include "OXSizeDockBar.h"  
#include "OXSzMiniDockFrmWnd.h"
#include "OXSkins.h"

#ifndef __OXMFCIMPL_H__
#if _MFC_VER >= 0x0700
#if _MFC_VER >= 1400
#include <afxtempl.h>
#endif
#include <..\src\mfc\afximpl.h>
#else
#include <..\src\afximpl.h>
#endif
#define __OXMFCIMPL_H__
#endif


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// COXSizeControlBar

IMPLEMENT_DYNAMIC(COXSizeControlBar, CControlBar)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members
CObArray* COXSizeControlBar::m_parrAllocBars;

HHOOK COXSizeControlBar::m_hMouseHook = NULL;

HWND COXSizeControlBar::m_hwndPrevMouseMoveWnd = NULL;

// Data members -------------------------------------------------------------
// protected:

// private:
	
// Member functions ---------------------------------------------------------
// public:

COXSizeControlBar::COXSizeControlBar(int nStyle) :
	m_pDockbarSkin(NULL),
	m_bDragging(FALSE),
	m_bOkToDrag(FALSE),
	m_ptOffset(0, 0),
	m_bClientBorder(FALSE),
	m_iLastTabPosition(-1)
{
	m_Style=nStyle;						
    m_PrevSize=CSize(0xffff,0xffff);	// dummy values so WindowPosChanged will 
										// respond correctly
    m_bPrevFloating=3;					// neither TRUE not FALSE;
	
	m_FloatSize=CSize(0,0);				// size when floating
   	m_HorzDockSize=CSize(0,0);			// size when docked horizontal
	m_VertDockSize=CSize(0,0);			// size when docked vertical

   	m_SavedDockSize=CSize(0,0);			// size before maximizing

	m_FloatingPosition=CPoint(0,0);	
	m_dwAllowDockingState = 0;
	if (nStyle & SZBARF_AUTOTIDY)
	{
		if (m_parrAllocBars == NULL)
			m_parrAllocBars = new CObArray;
		
		m_parrAllocBars->Add(this);
	}

	m_rectGripper.SetRectEmpty();			// gripper rect
	m_rectCloseBtn.SetRectEmpty();			// close button rect
	m_rectResizeBtn.SetRectEmpty();		// restore button rect

	m_bDelayRecalcLayout=FALSE;

	m_pressedBtn=NONE;
	m_bMaximized=FALSE;
	m_bActive=FALSE;
}


COXSizeControlBar::~COXSizeControlBar()
{
	// if the bar was created with this flag, then ensure it is deleted with it also.
	if (m_Style & SZBARF_AUTOTIDY)
	{
		int i;
		for (i = PtrToInt(m_parrAllocBars->GetUpperBound()); i >= 0; i--)
			if ((*m_parrAllocBars)[i] == this)	
		{
			m_parrAllocBars->RemoveAt(i);
			break;
		}
		ASSERT(i >= 0);			// means we didn't delete this item from the list
	}
	
	// This loop of debug code checks that we don't have any other references in the array.
	// This happens if we changed the auto delete flag during the lifetime of the control bar.
#ifdef _DEBUG
	if (m_parrAllocBars != NULL)
	{
		for (int i = PtrToInt(m_parrAllocBars->GetUpperBound()); i >= 0; i--)
			ASSERT ((*m_parrAllocBars)[i] != this);	
	}
#endif

	if(m_pDockContext!=NULL)
	{
		// delete the dock context here - in an attempt to call the correct destructor
		delete (COXDragDockContext*)m_pDockContext;
		m_pDockContext = NULL;
	}

	// delete the classic skin
	if ( m_pDockbarSkin != NULL )
		delete m_pDockbarSkin;
}

void COXSizeControlBar::TidyUp(CFrameWnd* pTopLevelFrame)
{
	if(m_parrAllocBars!=NULL)
	{
		for (int i = PtrToInt(m_parrAllocBars->GetUpperBound()); i >= 0; i--)
		{
			ASSERT((*m_parrAllocBars)[i]->
				IsKindOf(RUNTIME_CLASS(COXSizeControlBar)));
			if(::IsWindow(((COXSizeControlBar*)(*m_parrAllocBars)[i])->GetSafeHwnd()) && 
				((COXSizeControlBar*)(*m_parrAllocBars)[i])->GetTopLevelFrame()==
				pTopLevelFrame)
			{

				((COXSizeControlBar*)(*m_parrAllocBars)[i])->DestroyWindow();
			}
			if(!::IsWindow(((COXSizeControlBar*)(*m_parrAllocBars)[i])->GetSafeHwnd()))
			{
				delete ((*m_parrAllocBars)[i]);
			}
		}
		if(m_parrAllocBars->GetSize()==0)
		{
			delete m_parrAllocBars;
			m_parrAllocBars=NULL;
		}
	}
}


BEGIN_MESSAGE_MAP(COXSizeControlBar, CControlBar)
//{{AFX_MSG_MAP(COXSizeControlBar)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
	ON_WM_SETCURSOR()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_OX_MRC_HIDE, OnHide)
	ON_COMMAND(ID_OX_MRC_ALLOWDOCKING, OnToggleAllowDocking)
	ON_COMMAND(ID_OX_MRC_MDIFLOAT,	OnFloatAsMDI)
	ON_MESSAGE(WM_ADDCONTEXTMENUITEMS, OnAddContextMenuItems)
	ON_MESSAGE(WM_OX_APP_AFTERFLOAT_MSG, OnAfterFloatMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COXSizeControlBar message handlers

CSize COXSizeControlBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
#ifdef _VERBOSE_TRACE
	CString strTitle;
	GetWindowText(strTitle);
	TRACE(_T("CalcFixedLayout: '%s' Horz(%d,%d)\n"), LPCTSTR(strTitle), 
		m_HorzDockSize.cx, m_HorzDockSize.cy);
#endif	
	CControlBar::CalcFixedLayout(bStretch, bHorz);
    if (IsFloating())
		return m_FloatSize;

    if (bHorz)
		return m_HorzDockSize;
	else
		return m_VertDockSize;
}

// need to supply this, or else we can't instantiate the class. Derived classes should
// subclass this if they need to update their gadgets using this interface
void COXSizeControlBar::OnUpdateCmdUI(CFrameWnd* pTarget, 
									  BOOL bDisableIfNoHndler)
{
	UNREFERENCED_PARAMETER(pTarget);
	UNREFERENCED_PARAMETER(bDisableIfNoHndler);
    CWnd* pFocusWnd=CWnd::GetFocus();
	BOOL bActive=FALSE;
    if(pFocusWnd!=NULL && (pFocusWnd==this || 
		AfxIsDescendant(GetSafeHwnd(),pFocusWnd->GetSafeHwnd())))
	{
		bActive=TRUE;
	}
	if(bActive!=IsActive())
	{
		SetActive(bActive);
	}
}


// CWnd-style create - need ability to specific window class in order to prevent 
// flicker during resizing. 
BOOL COXSizeControlBar::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
							   DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
							   UINT nID, CCreateContext* pContext)
{
    ASSERT(pParentWnd != NULL);
	
    // have to set the style here
#if _MFC_VER <= 0x0421
    m_dwStyle = dwStyle;
#else
    m_dwStyle = dwStyle&CBRS_ALL;
#endif
	
    CRect Rectx;
    Rectx = rect;
	
    // calculate a sensible default rectangle if that's what the user wanted...
    if (memcmp(&rect, &CFrameWnd::rectDefault, sizeof(RECT)) == 0)
	{
        pParentWnd->GetClientRect(&Rectx);
        CSize def;
        def.cx = Rectx.right / 2;
        def.cy = Rectx.bottom  / 4;
        Rectx.left = Rectx.right - def.cx;
        Rectx.top  = Rectx.bottom - def.cy;
	}
	
   	// the rectangle specifies the default floating size.
   	m_FloatSize = Rectx.Size();
	
	// set default values for the docked sizes, based on this size.
	m_HorzDockSize.cx = m_FloatSize.cx;
	m_HorzDockSize.cy = m_FloatSize.cy;	
	
	m_VertDockSize.cx = m_HorzDockSize.cy;
	m_VertDockSize.cy = m_HorzDockSize.cx;

	// prevents flashing
	dwStyle|=WS_CLIPCHILDREN; 
	
    return CControlBar::Create(lpszClassName, lpszWindowName, dwStyle, 
		Rectx, pParentWnd, nID, pContext);
}

BOOL COXSizeControlBar::Create(CWnd* pParentWnd, LPCTSTR lpszWindowName, UINT nID,
								DWORD dwStyle, const RECT& rect)
{
	return Create(NULL, lpszWindowName, dwStyle, rect, pParentWnd, nID);
}

void COXSizeControlBar::SetSizeDockStyle(DWORD dwStyle)
{
	m_Style=dwStyle;
	if(::IsWindow(GetSafeHwnd()))
	{
		SetWindowPos(NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_DRAWFRAME);
	}
}


// Largely a copy of CControlBar::EnableDocking() - but uses a different class for the
// m_pDockContext, to give us different (hopefully you'll think better) dragging 
// behaviour.
void COXSizeControlBar::EnableDocking(DWORD dwDockStyle)
{
	// must be CBRS_ALIGN_XXX or CBRS_FLOAT_MULTI only
    ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY | CBRS_FLOAT_MULTI)) == 0);
	
    m_dwDockStyle = dwDockStyle;
    if (m_pDockContext == NULL)
		m_pDockContext = new COXDragDockContext(this);
	
    // permanently wire the bar's owner to its current parent
	if (m_hWndOwner == NULL)
		m_hWndOwner = ::GetParent(m_hWnd);
}

// message handler. Force the parent of the control bar to update it's style
// after floating, otherwise we'll wait till an WM_NCHITTEST.
LONG COXSizeControlBar::OnAfterFloatMessage(UINT /* wParam */, LONG /* lParam */)
{
	CWnd* pFrame = GetParentFrame();
	if(pFrame != NULL && pFrame->IsKindOf(RUNTIME_CLASS(COXSizableMiniDockFrameWnd)))
	{
		((COXSizableMiniDockFrameWnd*)pFrame)->GetContainedBarType();
	}
	
	return TRUE;			// message handled.
}

// paint the background of the window - probably want a style flag to turn this
// off as for many control bars it won't be required.
BOOL COXSizeControlBar::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	pDC->GetClipBox(&rect);
	pDC->FillSolidRect(&rect,::GetSysColor(COLOR_BTNFACE)); 
    return TRUE;
}


// change the cursor 
BOOL COXSizeControlBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	UNREFERENCED_PARAMETER(nHitTest);
	UNREFERENCED_PARAMETER(message);

	if(pWnd==this)
	{
		HCURSOR hCursor;     // Load the predefined Windows standard cursor.
		hCursor=AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		ASSERT(hCursor);
		::SetCursor(hCursor);
		return TRUE;
	}

	return FALSE;
}

// Normally a CControlBar would just pass most of these messages through to
// the parent. We want to handle them properly though - again may be this should
// be a behaviour flag
LRESULT COXSizeControlBar::WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

    ASSERT_VALID(this);
	
    // We need to ensure WM_COMMAND and other messages get through to the derived class.
    // Primarily done so we receive notifications from child windows. The default CControlBar
    // code routes messsages through to the parent. This means WM_COMMANDs, etc make their
    // way to a FrameWnd eventually. This is needed for toolbar's, dialog bars, etc, but isn't
    // very useful if we want to put controls on a COXSizeControlBar and process them 
	// locally
	
    // In case any of these messages are actually needed by the owner window, we check to see
    // if CWnd would handle them first. If not, then we pass them through to the owner window,
    // as CControlBar would.
	
    switch (nMsg)
	{
	case WM_COMMAND:
		{
			if (OnCommand(wParam, lParam))          // post normal commands....
			{
				return 1L; // command handled
			}
			break;
		}

	case WM_DESTROY:
		{
			CFrameWnd* pParentFrameWnd=GetParentFrame();
			ASSERT(pParentFrameWnd!=NULL);
			if(IsFloating())
			{
				pParentFrameWnd=pParentFrameWnd->GetTopLevelFrame();
				ASSERT(pParentFrameWnd!=NULL);
			}

			// notify main frame window that the current active control bar
			// window has changed
			COXMDIFrameWndSizeDock* pMDIFrameWnd=
				DYNAMIC_DOWNCAST(COXMDIFrameWndSizeDock,pParentFrameWnd);
			if(pMDIFrameWnd!=NULL && pMDIFrameWnd->m_pLastActiveCtrlBar==this)
			{
				pMDIFrameWnd->m_pLastActiveCtrlBar=NULL;
			}
			break;
		}
	}

	return CControlBar::WindowProc(nMsg, wParam, lParam);
}


// This handler is used to notify sizeable bars if their size has
// changed, or if they are docked/undocked.
void COXSizeControlBar::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
    CControlBar::OnWindowPosChanged(lpwndpos);
    CSize NewSize(lpwndpos->cx, lpwndpos->cy);

	// This is meant to return "floating" if we're not docked yet...
    BOOL bFloating = IsProbablyFloating();
	
    int Flags = (NewSize == m_PrevSize ? 0 : 1);
    Flags |= (bFloating == m_bPrevFloating ? 0 : 2);
    if (Flags != 0)
	{
		m_PrevSize = NewSize;
        m_bPrevFloating = bFloating;
        OnSizedOrDocked(NewSize.cx, NewSize.cy, bFloating, Flags);
		RedrawWindow();
	}

	RecalcLayout();

	if(m_bMaximized)
	{
		SetMaximized(FALSE);
	}
}


// override this function to respond to a redraw as a result of a
// resize or docked/undocked notification
void COXSizeControlBar::OnSizedOrDocked(int /* cx */, int /* cy */, 
										BOOL /* bFloating */, int /* flags */)
{
}


BOOL COXSizeControlBar::IsProbablyFloating()
{ 
	// used to check the dock bar status, but this has problems when we 
	// docking/undocking - so check the actual bar style instead
	return (m_pDockBar == NULL || (GetBarStyle() & CBRS_FLOATING));
}


LONG COXSizeControlBar::OnAddContextMenuItems(WPARAM /* wParam */, LPARAM lParam)
{
	HMENU hMenu = (HMENU)lParam;		// handle of menu.
	CMenu Menu;
	Menu.Attach(hMenu);
	
	DWORD dwDockStyle = m_dwDockStyle & CBRS_ALIGN_ANY;
	DWORD style;
	CString strMenu;
	
	BOOL bMDIFloating = FALSE;
	CFrameWnd* pParentFrame = GetParentFrame();
	if (IsFloating())
	{
		if (pParentFrame != NULL && 
			pParentFrame->IsKindOf(RUNTIME_CLASS(COXMDIFloatWnd)))
		{
			bMDIFloating = TRUE;
		}
	}
	style = (bMDIFloating ? MF_STRING | MF_CHECKED : MF_STRING);
	
	// if allowed - add the float as MDI floating window
	if ((m_Style&SZBARF_ALLOW_MDI_FLOAT)!=0 && m_pDockContext!=NULL)
	{	
		VERIFY(strMenu.LoadString(ID_OX_MRC_MDIFLOAT));
		Menu.AppendMenu(style, ID_OX_MRC_MDIFLOAT, strMenu);
	}
	
	if (!bMDIFloating && (dwDockStyle != 0 || m_dwAllowDockingState != 0))	// ie docking is actually allowed ...
	{
		DWORD style = (dwDockStyle != 0 ? MF_STRING | MF_CHECKED : MF_STRING);
		VERIFY(strMenu.LoadString(ID_OX_MRC_ALLOWDOCKING));
		Menu.AppendMenu(style, ID_OX_MRC_ALLOWDOCKING, strMenu);
	}
	VERIFY(strMenu.LoadString(ID_OX_MRC_HIDE));
	Menu.AppendMenu(MF_STRING, ID_OX_MRC_HIDE, strMenu);
	
	Menu.Detach();	// detatch MFC object
	return TRUE;
}


void COXSizeControlBar::OnHide()
{
	CFrameWnd* pParentFrameWnd = GetParentFrame();
	BOOL bMDIFloating = (IsFloating() && pParentFrameWnd != NULL && 
		pParentFrameWnd->IsKindOf(RUNTIME_CLASS(COXMDIFloatWnd)));
	if (bMDIFloating)
		// Have to activate another MDIChildFrame Wnd
	{
		((COXMDIFloatWnd*)pParentFrameWnd)->ShowControlBar(this, FALSE, FALSE);
		CFrameWnd* pTopParentFrameWnd = pParentFrameWnd->GetTopLevelFrame();
		if (pTopParentFrameWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
		{
			((CMDIFrameWnd*)pTopParentFrameWnd)->MDINext();
		}
	}
	else
		pParentFrameWnd->ShowControlBar(this, FALSE, FALSE);
}


void COXSizeControlBar::OnToggleAllowDocking()
{
	if ((m_dwDockStyle & CBRS_ALIGN_ANY) != 0)
	{											   // docking currently allowed - disable it
		m_dwAllowDockingState = m_dwDockStyle & CBRS_ALIGN_ANY;	// save previous state
		if (!IsFloating())
		{					// if docked, then force it to be floating...
			ASSERT(m_pDockContext != NULL);
			m_pDockContext->ToggleDocking();
		}
		EnableDocking(0);				// disable docking
	}
	else
	{
		EnableDocking (m_dwAllowDockingState);	// re-enable docking...
	}
}

void COXSizeControlBar::OnFloatAsMDI()
{
	ASSERT(m_Style & SZBARF_ALLOW_MDI_FLOAT);		// must have specified this
	
	COXMDIFrameWndSizeDock* pFrame = (COXMDIFrameWndSizeDock*)AfxGetMainWnd();
	ASSERT(pFrame != NULL);
	ASSERT(pFrame->IsKindOf(RUNTIME_CLASS(COXMDIFrameWndSizeDock)));
	ASSERT(m_pDockContext!=NULL);
	
	CFrameWnd* pParentFrame = GetParentFrame();
	BOOL bMDIFloating = (IsFloating() && pParentFrame != NULL && 
		pParentFrame->IsKindOf(RUNTIME_CLASS(COXMDIFloatWnd)));
	
	if (bMDIFloating)
	{
		pFrame->UnFloatInMDIChild(this, m_pDockContext->m_ptMRUFloatPos);
	}
	else
	{
		pFrame->FloatControlBarInMDIChild(this, m_pDockContext->m_ptMRUFloatPos);
	}		
}


// Now run off WM_CONTEXTMENU: if user wants standard handling, then let him have it
void COXSizeControlBar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (m_Style & SZBARF_STDMOUSECLICKS)
	{
		CMenu menu;
		if (menu.CreatePopupMenu())
		{
			OnAddContextMenuItems(0,(LPARAM)menu.m_hMenu);
			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		}
	} 
	else
	{
		CControlBar::OnContextMenu(pWnd, point);
	}
}

void COXSizeControlBar::OnNcCalcSize(BOOL bCalcValidRects, 
									 NCCALCSIZE_PARAMS* lpncsp)
{
	CControlBar::OnNcCalcSize(bCalcValidRects, lpncsp);
	GetDockbarSkin()->OnNcCalcSize(bCalcValidRects, lpncsp, this);
}


void COXSizeControlBar::OnNcPaint() 
{
	EraseNonClient();
}

void COXSizeControlBar::OnPaint() 
{
    // overridden to skip border painting based on clientrect
    CPaintDC dc(this);
}

void COXSizeControlBar::EraseNonClient()
{
    // get window DC that is clipped to the non-client area
    CWindowDC dc(this);
    CRect rectClient;
    GetClientRect(rectClient);
    CRect rectWindow;
    GetWindowRect(rectWindow);
    ScreenToClient(rectWindow);
	CSize sizeOffset(-rectWindow.left, -rectWindow.top);
	rectClient+=sizeOffset;

    rectWindow+=sizeOffset;

    // erase parts not drawn
	dc.ExcludeClipRect(rectClient);
	dc.IntersectClipRect(rectWindow);
	CRect rect;
	dc.GetClipBox(&rect);

	GetDockbarSkin()->DrawNonClientArea(&dc, rectWindow, this);

	RecalcLayout();

	if(IsGripper())
	{
		DrawGripper(&dc);
	}
	if(IsCloseBtn())
	{
		DrawCloseBtn(&dc);
	}
	if(IsResizeBtn())
	{
		DrawResizeBtn(&dc);
	}

	// Draw the border
	if (m_bClientBorder)
	{
		CBrush brBorder;
		brBorder.CreateSolidBrush(GetDockbarSkin()->GetClientBorderColor());
		rectClient.InflateRect(1, 1);
		dc.FrameRect(rectClient, &brBorder);
	}
}

void COXSizeControlBar::DrawGripper(CDC* pDC)
{
	GetDockbarSkin()->DrawGripper(pDC, this);
}

void COXSizeControlBar::DrawCloseBtn(CDC* pDC)
{
	GetDockbarSkin()->DrawCloseButton(pDC, this);
}

void COXSizeControlBar::DrawResizeBtn(CDC* pDC)
{
	GetDockbarSkin()->DrawResizeButton(pDC, this);
}

void COXSizeControlBar::RecalcLayout()
{
	GetDockbarSkin()->RecalcLayout(this);
}

#if _MSC_VER >= 1400
LRESULT COXSizeControlBar::OnNcHitTest(CPoint point)
#else
UINT COXSizeControlBar::OnNcHitTest(CPoint point)
#endif
{
    if((m_dwStyle & CBRS_FLOATING))
	{
        return CControlBar::OnNcHitTest(point);
	}

    CRect rectWindow;
    GetWindowRect(rectWindow);
    if (rectWindow.PtInRect(point))
	{
        return HTCLIENT;
	}
    else
	{
        return CControlBar::OnNcHitTest(point);
	}
}


void COXSizeControlBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(!IsFloating())
	{
		CPoint ptTest=point;
		// handle mouse click over close, restore buttons
		//
		m_pressedBtn=NONE;

		if(m_rectCloseBtn.PtInRect(ptTest))
		{
			SetCapture();
			m_pressedBtn=CLOSEBTN;
			RedrawCloseBtn();
			return;
		}
		else if(m_rectResizeBtn.PtInRect(ptTest))
		{
			if(CanResize())
			{
				SetCapture();
				m_pressedBtn=RESIZEBTN;
				RedrawResizeBtn();
			}
			return;
		}
		else if (AfxGetMainWnd()->SendMessage(WM_QUERYSNAPPING))
		{
			m_ptLButtonDown = point;
			m_bOkToDrag = TRUE;
			return;
		}
	}

	CControlBar::OnLButtonDown(nFlags, point);
}


void COXSizeControlBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if (m_bDragging)
	{
		ReleaseCapture();
		m_bDragging = FALSE;
		return;
	}

	SIZEBARBTN pressedBtn=m_pressedBtn;
	m_pressedBtn=NONE;
	if(pressedBtn==CLOSEBTN)
		RedrawCloseBtn();
	else if(pressedBtn==RESIZEBTN)
		RedrawResizeBtn();

	if(::GetCapture()!=GetSafeHwnd())
	{
		CControlBar::OnLButtonUp(nFlags,point);
		return;
	}

	::ReleaseCapture();

	CPoint ptTest=point;
	
	if(pressedBtn==CLOSEBTN && m_rectCloseBtn.PtInRect(ptTest))
	{
		// close the bar
		ShowBar(FALSE);
	}
	else if(m_pDockBar!=NULL && pressedBtn==RESIZEBTN && 
		m_rectResizeBtn.PtInRect(ptTest))
	{
		// restore the bar
		ASSERT(m_pDockBar->IsKindOf(RUNTIME_CLASS(COXSizeDockBar)));
		((COXSizeDockBar*)m_pDockBar)->ResizeBar(this,!m_bMaximized);
	}
}

void COXSizeControlBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(::GetCapture()==GetSafeHwnd())
	{
		SIZEBARBTN pressedBtn=NONE;
		if(m_rectCloseBtn.PtInRect(point))
		{
			pressedBtn=CLOSEBTN;
		}
		else if(m_rectResizeBtn.PtInRect(point) && CanResize())
		{
			pressedBtn=RESIZEBTN;
		}

		if(m_pressedBtn!=pressedBtn)
			m_pressedBtn=pressedBtn;
	}

	RedrawButtons();

	// In order to start dragging the left button must be down and the current point
	// must be at least 3 pixels away from the point where the button was pressed
	if (!m_bDragging && m_bOkToDrag && (nFlags & MK_LBUTTON) && 
		(abs(point.x - m_ptLButtonDown.x) > 3 || abs(point.y - m_ptLButtonDown.y) > 3))
	{
		m_bOkToDrag = FALSE; // reset the flag

		// Start dragging
		SaveMouseOffset(point);
		m_bDragging = TRUE;
		SetCapture();
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
		return;
	}

	// If we are already dragging
	if (m_bDragging)
	{
		CPoint ptScreen = point;
		ClientToScreen(&point);

		// Get the appropriate dock bar. If one is not found then float.
		CFrameWnd* pFrameWnd = DYNAMIC_DOWNCAST(CFrameWnd, ::AfxGetMainWnd());
		if (pFrameWnd == NULL)
		{
			ReleaseCapture();
			m_bDragging = FALSE;
			return;
		}

		// Handle pending WM_PAINT messages
		MSG msg;
		while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
			{
				ReleaseCapture();
				m_bDragging = FALSE;
				return;
			}
			DispatchMessage(&msg);
		}

		if (m_pDockContext == NULL)
			pFrameWnd->FloatControlBar(this, point);
		else
		{
			COXSizeDockBar* pDockBar = COXSizeDockBar::GetAppropriateDockBar(point, this);
			if (pDockBar == NULL || nFlags & MK_CONTROL)
			{
				if (m_pDockBar != NULL)
				{
					COXSizeDockBar* pSizeDockBar = DYNAMIC_DOWNCAST(COXSizeDockBar, m_pDockBar);
					if (pSizeDockBar != NULL)
						pSizeDockBar->m_wndDockTabCtrl.RemoveTab(this);
				}
				pFrameWnd->FloatControlBar(this, point - m_ptOffset);
			}
			else
			{
				// Determine the docking rectangle
				CRect rectWindow;
				GetWindowRect(rectWindow);
				CRect rectDock;
				rectDock.SetRect(point.x, point.y, point.x + rectWindow.Width(), point.y + rectWindow.Height());

				pDockBar->DockControlBar(this, rectDock);
			}
		}
	}
	else
		CControlBar::OnMouseMove(nFlags, point);
}


void COXSizeControlBar::RedrawCloseBtn()
{
	CRect rect=m_rectCloseBtn;
	RedrawWindow(rect,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE);
}


void COXSizeControlBar::RedrawResizeBtn()
{
	CRect rect=m_rectResizeBtn;
	RedrawWindow(rect,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE);
}


BOOL COXSizeControlBar::CanResize() const
{
	BOOL bCanResize=FALSE;
	if(m_pDockBar!=NULL && m_pDockBar->IsKindOf(RUNTIME_CLASS(COXSizeDockBar)))
	{

		if (((COXSizeDockBar*)m_pDockBar)->m_wndDockTabCtrl.GetItemCount() > 0)
			return FALSE;

		CRect rect;
		GetWindowRect(rect);
		if(((COXSizeDockBar*)m_pDockBar)->
			BarsOnThisRow((CControlBar*)this,rect)>0)
		{
			bCanResize=TRUE;
		}
	}

	return bCanResize;
}


void COXSizeControlBar::SetActive(BOOL bActive, BOOL bResetAllCtrlBars/*=FALSE*/)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	m_bActive=bActive;

	CFrameWnd* pParentFrameWnd=GetParentFrame();
	ASSERT(pParentFrameWnd!=NULL);
	if(IsFloating())
	{
		pParentFrameWnd=pParentFrameWnd->GetTopLevelFrame();
		ASSERT(pParentFrameWnd!=NULL);
	}

	if(m_bActive)
	{
		CWnd* pFocusWnd=CWnd::GetFocus();
		if(pFocusWnd!=this && !IsChild(pFocusWnd))
		{
			// try to set focus to first child window
			CWnd* pChildWnd=this;
			while(pChildWnd!=NULL)
			{
				pChildWnd->SetFocus();
				pFocusWnd=GetFocus();
				if(pFocusWnd!=NULL && (pFocusWnd==pChildWnd || IsChild(pFocusWnd)))
				{
					break;
				}
				else
				{
					pChildWnd=pChildWnd->GetWindow(GW_CHILD);
				}
			}
		}

		if(bResetAllCtrlBars)
		{
			POSITION pos=pParentFrameWnd->m_listControlBars.GetHeadPosition();
			while(pos!=NULL)
			{
				COXSizeControlBar* pBar=DYNAMIC_DOWNCAST(COXSizeControlBar,
					(CControlBar*)pParentFrameWnd->m_listControlBars.GetNext(pos));
				if(pBar!=NULL && pBar!=this && pBar->IsActive() && 
					!pBar->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
				{
					pBar->SetActive(FALSE);
				}
			}
		}
	}	

	// notify main frame window that the current active control bar
	// window has changed
	COXMDIFrameWndSizeDock* pMDIFrameWnd=
		DYNAMIC_DOWNCAST(COXMDIFrameWndSizeDock,pParentFrameWnd);
	if(pMDIFrameWnd!=NULL)
	{
		if(m_bActive)
		{
			pMDIFrameWnd->m_pLastActiveCtrlBar=this;
		}
		else
		{
			if(pMDIFrameWnd->m_pLastActiveCtrlBar==this)
			{
				CMDIChildWnd* pMDIChildWnd=pMDIFrameWnd->MDIGetActive();
				if(pMDIChildWnd!=NULL)
				{
					CWnd* pFocusWnd=GetFocus();
					if(pFocusWnd!=NULL && 
						(pMDIChildWnd->IsChild(pFocusWnd) || pMDIChildWnd==pFocusWnd))
					{
						//pMDIFrameWnd->m_pLastActiveCtrlBar=NULL;
					}
				}
			}
		}
	}	

	RedrawWindow(m_rectGripper,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE);
}

COXDockbarSkin* COXSizeControlBar::GetDockbarSkin()
{
	// Check if the app is derived from COXSkinnedApp
	COXSkinnedApp* pSkinnedApp = DYNAMIC_DOWNCAST(COXSkinnedApp, AfxGetApp());
	if (pSkinnedApp != NULL && pSkinnedApp->GetCurrentSkin() != NULL)
		return pSkinnedApp->GetCurrentSkin()->GetDockbarSkin();
	else
	{
		// Create a classic skin for this class if not created already
		if (m_pDockbarSkin == NULL)
			m_pDockbarSkin = new COXDockbarSkinClassic();

		return m_pDockbarSkin;
	}
}

// Update the buttons of dockbar when the mouse leaves
LRESULT CALLBACK COXSizeControlBar::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return ::CallNextHookEx(m_hMouseHook, nCode, wParam, lParam);

	if (nCode == HC_ACTION && (wParam == WM_MOUSEMOVE || wParam == WM_NCMOUSEMOVE))
	{

		MOUSEHOOKSTRUCT* pMH = (MOUSEHOOKSTRUCT*) lParam;

		// If the previous message was for COXSizableMiniDockFrameWnd and the current is not
		// we need to update the caption buttons
		COXSizeControlBar* pPrev = DYNAMIC_DOWNCAST(COXSizeControlBar,
			CWnd::FromHandlePermanent(m_hwndPrevMouseMoveWnd));
		COXSizeControlBar* pCurrent = DYNAMIC_DOWNCAST(COXSizeControlBar,
			CWnd::FromHandlePermanent(pMH->hwnd));

		if (pPrev != NULL && pCurrent != pPrev)
		{
			// The mouse just left the window
			pPrev->RedrawButtons();
		}
		m_hwndPrevMouseMoveWnd = pMH->hwnd;
	}

	return CallNextHookEx(m_hMouseHook, nCode, wParam, lParam);
}

int COXSizeControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Hook the mouse
	if (m_hMouseHook == NULL)
		m_hMouseHook = ::SetWindowsHookEx(WH_MOUSE, MouseProc, 0, AfxGetApp()->m_nThreadID);
	
	return 0;
}

void COXSizeControlBar::OnDestroy() 
{
	// Unhook the mouse
	if (m_hMouseHook)
	{
		::UnhookWindowsHookEx(m_hMouseHook);
		m_hMouseHook = NULL;
	}

	CControlBar::OnDestroy();
}

void COXSizeControlBar::RedrawButtons()
{
	CWindowDC dc(this);
	DrawCloseBtn(&dc);//GetDockbarSkin()->DrawCloseButton(&dc, this);
	DrawResizeBtn(&dc);//GetDockbarSkin()->DrawResizeButton(&dc, this);
}

void COXSizeControlBar::SaveMouseOffset(CPoint point)
{
	// Calculate and save the offset
	CRect rectWindow;
	GetWindowRect(rectWindow);
	ScreenToClient(&rectWindow);

	// Since the offset will be used only for floating windows we need
	// to scale it accordingly
	if (m_dwStyle & CBRS_ORIENT_HORZ && !IsFloating())
	{
		m_ptOffset.x = rectWindow.bottom - point.y;
		m_ptOffset.y = point.x - rectWindow.left;

		m_ptOffset.x = (long) (m_ptOffset.x * 
			((double) m_FloatSize.cx / (double) rectWindow.Height()));
	}
	else
	{
		m_ptOffset = point - rectWindow.TopLeft();
		m_ptOffset.x = (long) (m_ptOffset.x * 
			((double) m_FloatSize.cx / (double) rectWindow.Width()));
	}

	if (m_ptOffset.x < 5)
		m_ptOffset.x = 5;
	if (m_ptOffset.x > m_FloatSize.cx - 20)
		m_ptOffset.x = m_FloatSize.cx - 20;

	if (!IsFloating())
		m_ptOffset.y = 10; // always use 10
}

void COXSizeControlBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// only toggle docking if clicked in "void" space
	if (m_pDockBar != NULL && OnToolHitTest(point, NULL) == -1)
	{
		COXSizeDockBar* pSzDockBar = DYNAMIC_DOWNCAST(COXSizeDockBar, m_pDockBar);
		if (pSzDockBar != NULL && AfxGetMainWnd()->SendMessage(WM_QUERYSNAPPING))
		{
			// If this was tabbed we need to untab it and select something else
			int iOldIndex = pSzDockBar->m_wndDockTabCtrl.FindTab(this);
			if (iOldIndex != -1)
				pSzDockBar->m_wndDockTabCtrl.RemoveTab(this);			
		}

		// start the drag
		ASSERT(m_pDockContext != NULL);
		m_pDockContext->ToggleDocking();	
	}
	else
	{
		CWnd::OnLButtonDblClk(nFlags, point);
	}
}

void COXSizeControlBar::ShowBar(BOOL bShow)
{
	if (IsWindowVisible() == bShow)
		return; // already shown or already hidden
	

	CFrameWnd* pParentFrame = GetParentFrame();
	ASSERT(pParentFrame != NULL); // this bar must have a parent frame

	// If foating or there are no tabbed controls simply call CFrameWnd::ShowControlBar()
	COXSizeDockBar* pDockBar = (COXSizeDockBar*) m_pDockBar;
	
	if (bShow)
	{
		// If the control bar is tabbed we need to just select it
		COXDockTabCtrl* pDockTabCtrl = GetDockTabCtrl();
		if (pDockTabCtrl != NULL)
		{
			pDockTabCtrl->SetCurSel(pDockTabCtrl->FindTab(this));
			pDockTabCtrl->ShowSelectedTab();
			return;
		}

		pParentFrame->ShowControlBar(this, bShow, TRUE);
		
		// Figure out if we need to tab the control bar
		if (!IsFloating() && pDockBar != NULL &&
			m_iLastTabPosition != -1 &&
			pDockBar->GetVisibleSizeControlBarCount(this) > 1)
		{
			int iTabIndex = pDockBar->m_wndDockTabCtrl.FindTab(this);
			if (iTabIndex == -1)
			{
				// Tab is not there, so insert it
				pDockBar->m_wndDockTabCtrl.InsertTab(this, m_iLastTabPosition, TRUE);
			}
			else
			{
				// Just select it
				pDockBar->m_wndDockTabCtrl.SetCurSel(iTabIndex);
				pDockBar->m_wndDockTabCtrl.ShowSelectedTab();
			}
		}
	}
	else // hide
	{
		pParentFrame->ShowControlBar(this, bShow, TRUE);

		if (!IsFloating() && pDockBar != NULL &&
			pDockBar->m_wndDockTabCtrl.GetItemCount() != 0)
		{
			// Save the last tab position
			m_iLastTabPosition = pDockBar->m_wndDockTabCtrl.FindTab(this);
			
			// Remove the tab
			pDockBar->m_wndDockTabCtrl.RemoveTab(this);
		}
	}
}

COXDockTabCtrl* COXSizeControlBar::GetDockTabCtrl()
{
	COXSizeDockBar* pDockBar = DYNAMIC_DOWNCAST(COXSizeDockBar, m_pDockBar);
	if (pDockBar == NULL)
		return NULL;

	if (pDockBar->m_wndDockTabCtrl.FindTab(this) == -1)
		return NULL;
	else
		return &pDockBar->m_wndDockTabCtrl;
}

BOOL COXSizeControlBar::IsShown()
{
	if (IsVisible())
		return TRUE;
	else
		return FALSE;
}
