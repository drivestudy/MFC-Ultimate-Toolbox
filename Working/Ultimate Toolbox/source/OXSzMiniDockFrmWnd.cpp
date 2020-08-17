// ===================================================================================
// 					Class Implementation : COXSizableMiniDockFrameWnd
// ===================================================================================

// Header file : OXSzMiniDockFrmWnd.cpp

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
#include <afxole.h>

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


#include "OXSzMiniDockFrmWnd.h"

#include "OXSizeCtrlBar.h"
#include "OXSizeDockBar.h"
#include "OXFrameWndDock.h"

#include "OXSkins.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(COXSizableMiniDockFrameWnd, CMiniDockFrameWnd)

#define new DEBUG_NEW

// Helper function: used below
void AdjustForBorders(CRect& rect, DWORD dwStyle);

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// Data members -------------------------------------------------------------
// protected:

// private:
	
// Member functions ---------------------------------------------------------
// public:

BEGIN_MESSAGE_MAP(COXSizableMiniDockFrameWnd, CMiniDockFrameWnd)
	//{{AFX_MSG_MAP(COXSizableMiniDockFrameWnd)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_CLOSE()
    ON_WM_NCLBUTTONDOWN()
    ON_WM_NCHITTEST()
	ON_WM_MOUSEACTIVATE()
    ON_WM_NCACTIVATE()
	ON_WM_PARENTNOTIFY()
	ON_WM_SYSCOMMAND()
    ON_WM_INITMENUPOPUP()
	ON_WM_ACTIVATE()
	ON_WM_NCPAINT()
	ON_WM_SETCURSOR()
	ON_WM_NCLBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_NCRBUTTONDOWN()
    ON_WM_WINDOWPOSCHANGED()
	ON_WM_NCMOUSEMOVE()
	//}}AFX_MSG_MAP
    // Global help commands
END_MESSAGE_MAP()

HHOOK COXSizableMiniDockFrameWnd::m_hMouseHook = NULL;
HWND COXSizableMiniDockFrameWnd::m_hwndPrevMouseMoveWnd = NULL;

COXSizableMiniDockFrameWnd::COXSizableMiniDockFrameWnd() :
	m_pMiniFrameSkin(NULL)
{

}

COXSizableMiniDockFrameWnd::~COXSizableMiniDockFrameWnd()
{
	if ( m_pMiniFrameSkin != NULL )
		delete m_pMiniFrameSkin;
}

int COXSizableMiniDockFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMiniDockFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_nContainedBarType = Unknown;

	m_bBeingDestroyed=FALSE;

	// Hook the mouse
	if (m_hMouseHook == NULL)
		m_hMouseHook = ::SetWindowsHookEx(WH_MOUSE, MouseProc, 0, AfxGetApp()->m_nThreadID);

	return 1;
}

void COXSizableMiniDockFrameWnd::OnSysCommand(UINT nID, LPARAM lParam)
{
	// Call DrawCaption to prevent the original caption from drawing
	GetMiniFrameSkin()->DrawCaption(this);

	// CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at position 1
	// in the array
	CControlBar* pBar=((COXSizeDockBar *)&m_wndDockBar)->GetFirstControlBar();
	if(pBar!=NULL)
	{
		ASSERT_KINDOF(CControlBar, pBar);
		ASSERT(pBar->m_pDockContext != NULL);
		if(pBar->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
		{
			COXMDIChildWndSizeDock* pMDIChild=((COXSizeViewBar*)pBar)->m_pChildWnd;
			if(::IsWindow(pMDIChild->m_hWnd))
			{
				if(nID==pMDIChild->m_nDockMessageID)
				{
					pMDIChild->OnMakeItDockable();
					return;
				}
			}
		}
	}

	CMiniDockFrameWnd::OnSysCommand(nID, lParam);
}

// respond to the miniframe resizing. If we've got a sizeable control
// bar in the window, then we set it's size. Need to adjust for the
// window borders. The window will then get repositioned by a ReCalcLayout()
void COXSizableMiniDockFrameWnd::OnSize(UINT /* nType */, int cx, int cy)
	{
    if (cx == 0 && cy == 0)
		return;
	
    // We don't support CBRS_FLOAT_MULTI
    if ((m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0)
		{
        // CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at position 1
        // in the array
        COXSizeControlBar* pBar = ((COXSizeDockBar*)(&m_wndDockBar))->GetFirstControlBar();
        // ignore size request if not visible....
        if (pBar != NULL && COXSizeDockBar::IsSizeable(pBar) && pBar->IsVisible())
			{
			CRect rect(0, 0, cx, cy);
			AdjustForBorders(rect, pBar->m_dwStyle);
			
            pBar->m_FloatSize.cx = rect.Width();
            pBar->m_FloatSize.cy = rect.Height();
			}
		}
    RecalcLayout();
	}


BOOL COXSizableMiniDockFrameWnd::PreCreateWindow(CREATESTRUCT &cs)
	{
	// modify frame style so it is fully sizeable - we will modify this again later
	// if we discover that we have a standard MFC control bar inside the frame
	// turn on thick frame styles. MFS_THICKFRAME is what's expected, but also need WS_THICKFRAME,
	// as NT expects this to be able to do the resizing.
	cs.style |= MFS_THICKFRAME | WS_THICKFRAME; 
    cs.style &= ~( MFS_MOVEFRAME | MFS_4THICKFRAME );		
    return CMiniDockFrameWnd::PreCreateWindow(cs);
	}



// over-ridden so we can find out the type of the bar in this window
#if _MSC_VER >= 1400
LRESULT COXSizableMiniDockFrameWnd::OnNcHitTest(CPoint point)
#else
UINT COXSizableMiniDockFrameWnd::OnNcHitTest(CPoint point)
#endif
{
	return CMiniDockFrameWnd::OnNcHitTest(point);
}


// if we've got a COXSizeControlBar inside this frame, and it's a resize,
// then we can use the default window's sizing. 
void COXSizableMiniDockFrameWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{

	enum ContainedBarType Type = GetContainedBarType();
	if (Type == MRCSizeBar)
	{
		if (nHitTest >= HTSIZEFIRST && nHitTest <= HTSIZELAST)
		{
			// special activation for floating toolbars
			ActivateTopParent();
			CMiniFrameWnd::OnNcLButtonDown(nHitTest, point);
			return;
		}
		else if (nHitTest == HTSYSMENU)
		{
			// do the system menu - ie give move,size,hide, etc options as would Win95.						
#if _MFC_VER < 0x0700		
			InvertSysMenu();
#endif
			CPoint pt(0,0);
			ClientToScreen(&pt);
			CRect rcSysIcon;
			GetWindowRect(&rcSysIcon);
			rcSysIcon.right = rcSysIcon.left + 12;		// NB:hard-coded value for width of system icon
			rcSysIcon.bottom = pt.y;					
			
			CMenu* pSysMenu = GetSystemMenu(FALSE);
			int nCmd = pSysMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, this, & rcSysIcon);
#if _MFC_VER < 0x0700	
			InvertSysMenu();
#endif	
			if (nCmd != 0)
			{
				SendMessage(WM_SYSCOMMAND, nCmd, 0);
			}
			return;
		}
	}

	if (nHitTest == HTCLOSE)
	{
		if (!GetMiniFrameSkin()->DrawCaptionButtons(this))
			CMiniDockFrameWnd::OnNcLButtonDown(nHitTest, point);
		else
		{
			// If the close button is hidden emulate a caption click
			if (GetSystemMenu(FALSE)->GetMenuItemCount() == 0)
				CMiniDockFrameWnd::OnNcLButtonDown(HTCAPTION, point);
		}
	}
	else
	{
		if (nHitTest == HTCAPTION)
		{
			// special activation for floating toolbars
			ActivateTopParent();

			// initiate toolbar drag for non-CBRS_FLOAT_MULTI toolbars
			if ((m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0)
			{
				// Get a pointer to the control bar
				int nPos = 1;	
				CControlBar* pBar = NULL;
				while (pBar == NULL && nPos < m_wndDockBar.m_arrBars.GetSize())
					pBar = (CControlBar*) m_wndDockBar.m_arrBars[nPos++];

				if (AfxGetMainWnd()->SendMessage(WM_QUERYSNAPPING))
				{
					// Snaping is enabled

					// Handle COXCoolToolBar
					COXCoolToolBar* pToolBar = DYNAMIC_DOWNCAST(COXCoolToolBar, pBar);
					if (pToolBar)
					{
						// We have a toolbar
						CRect rectThis, rectToolBar;
						GetWindowRect(rectThis);
						pToolBar->GetWindowRect(rectToolBar);
						
						CPoint ptClient(point);
						pToolBar->ScreenToClient(&ptClient);
						ptClient.x += rectToolBar.left - rectThis.left;
						ptClient.y += rectToolBar.top - rectThis.top;
						
						pToolBar->SaveMouseOffset(ptClient);

						pToolBar->SetCapture();
						pToolBar->m_bDragging = true;
						::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
						
						return;
					}

					// Handle COXSizeControlBar
					COXSizeControlBar* pControlBar = DYNAMIC_DOWNCAST(COXSizeControlBar, pBar);
					if (pControlBar)
					{
						// We have a docking window
						CRect rectThis, rectControlBar;
						GetWindowRect(rectThis);
						pControlBar->GetWindowRect(rectControlBar);
						
						CPoint ptClient(point);
						pControlBar->ScreenToClient(&ptClient);
						ptClient.x += rectControlBar.left - rectThis.left;
						ptClient.y += rectControlBar.top - rectThis.top;
						
						pControlBar->SaveMouseOffset(ptClient);
						pControlBar->SetCapture();
						pControlBar->m_bDragging = true;
						::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));

						return;
					}
				}
				else // snaping is disabled
				{
					CMiniDockFrameWnd::OnNcLButtonDown(nHitTest, point);
					return;
				}
			}
		}
		else if (nHitTest >= HTSIZEFIRST && nHitTest <= HTSIZELAST)
		{

			// special activation for floating toolbars
			ActivateTopParent();

			// Get a pointer to the control bar
			int nPos = 1;	
			CControlBar* pBar = NULL;
			while (pBar == NULL && nPos < m_wndDockBar.m_arrBars.GetSize())
				pBar = (CControlBar*) m_wndDockBar.m_arrBars[nPos++];

			ASSERT(pBar != NULL);
			ASSERT_KINDOF(CControlBar, pBar);
			ASSERT(pBar->m_pDockContext != NULL);

			// CBRS_SIZE_DYNAMIC toolbars cannot have the CBRS_FLOAT_MULTI style
			ASSERT((m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0);
			pBar->m_pDockContext->StartResize(nHitTest, point);
			return;

		}
		CMiniFrameWnd::OnNcLButtonDown(nHitTest, point);
	}
}


int COXSizableMiniDockFrameWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// activate this window if necessary
	// CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at position 1
	// in the array
	CControlBar* pBar = ((COXSizeDockBar *)&m_wndDockBar)->GetFirstControlBar();
	if(pBar->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
	{
		pBar->SendMessage(WM_ACTIVATEVIEWBAR,TRUE,ID_SOURCE_MINIDOCK);
	}

	enum ContainedBarType Type = GetContainedBarType();
	if (Type == MRCSizeBar)
	{
		if (nHitTest >= HTSIZEFIRST && nHitTest <= HTSIZELAST)
			return CMiniFrameWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return CMiniDockFrameWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}



// returns the type of the contained bar
// Floating frames are created from CFrameWnd::CreateFloatingFrame(), and at this point,
// we don't know what type of control bar will eventually be in this frame. This routine
// determines the type of control bar, and sets the neccessary styles. Generally this routine gets
// called as soon as we do a WM_NCHITTEST on the window - ie before the user can get a chance to
// manipulate it with the mouse
//
// COXSizeControlBar - default style of WS_THICKFRAME is ok (we've overridden the create too)
//					    but we need to ensure "SC_SIZE" is on the menu, otherwise we can't
//						the default WM_NCLBUTTONDOWN won't generate an SC_SIZE.
//
// CControlBar, CBRS_SIZE_DYNAMIC
//					  - set MFS_4THICKFRAME (disallow diagonal sizing)
//
// CControlBar, not CBRS_SIZE_DYNAMIC
//					  - remove WS_THICKFRAME & add MFS_MOVEFRAME (we don't want sizing hit test values)
enum COXSizableMiniDockFrameWnd::ContainedBarType 
COXSizableMiniDockFrameWnd::GetContainedBarType()
	{
	if (m_nContainedBarType == Unknown)
		{
		m_nContainedBarType = MFCBase;
		if ((m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0) // don't handle this
			{
			COXSizeControlBar* pBar = ((COXSizeDockBar*)(&m_wndDockBar))->GetFirstControlBar();
			if (pBar != NULL)
				{
				if (COXSizeDockBar::IsSizeable(pBar))
					{		
					m_nContainedBarType = MRCSizeBar;
					GetSystemMenu(TRUE);					// reset the system menu
					
					// delete sys menu items that might be present....
					CMenu* pSysMenu = GetSystemMenu(FALSE);
					if(pSysMenu!=NULL)
					{
						pSysMenu->DeleteMenu(SC_MAXIMIZE, MF_BYCOMMAND);
						pSysMenu->DeleteMenu(SC_MINIMIZE, MF_BYCOMMAND);
						pSysMenu->DeleteMenu(SC_RESTORE, MF_BYCOMMAND);
						pSysMenu->DeleteMenu(SC_TASKLIST, MF_BYCOMMAND);
						// remove anything with ID=0
						while (pSysMenu->DeleteMenu(0, MF_BYCOMMAND));	
					}
					
					// if window is meant to close, ensure sysmenu has "Close" on it, not "Hide"
					// MFC4.0 replaces "Close" with "Hide"
					// extract close text from ID_FILE_CLOSE after the "\n" for the tooltip prompt
					if (!(pBar->m_Style & SZBARF_DESTROY_ON_CLOSE))
						{
						CString strHide;
						if (strHide.LoadString(AFX_IDS_HIDE))
							{
							// modify menu text to be "Hide" instrad of close
							VERIFY(pSysMenu->ModifyMenu(SC_CLOSE, MF_BYCOMMAND | MF_STRING, SC_CLOSE, strHide));  
							} 
						}
					}
				else
					{
					if ((pBar->m_dwStyle & CBRS_SIZE_DYNAMIC))	 // dynamic bar - turn on MFS_4THICKFRAME
						{
						ModifyStyle(0, MFS_4THICKFRAME);	
						return m_nContainedBarType;
						}
					
					}
				}
			}
		if (m_nContainedBarType == MFCBase)
			{
			ModifyStyle(WS_THICKFRAME, MFS_MOVEFRAME);
			}
		}		
	// if bar is MFC bar (and not CBRS_SIZE_DYNAMIC, turn on MFS_MOVEFRAME)
	return m_nContainedBarType;
	}


void COXSizableMiniDockFrameWnd::OnClose()
{
	if(m_bBeingDestroyed)
	{
		return;
	}

	m_bBeingDestroyed=TRUE;

	if ((m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0)
	{
		CControlBar* pBar = NULL;
		// CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at position 1
		// in the array
		pBar = ((COXSizeDockBar *)&m_wndDockBar)->GetFirstControlBar();
		if(pBar!=NULL && pBar->IsKindOf(RUNTIME_CLASS(COXSizeControlBar)))
		{
			if(((COXSizeControlBar*)pBar)->m_Style & SZBARF_DESTROY_ON_CLOSE)
            {
				// close the Frame Window
				CFrameWnd::OnClose();       // close the window
				delete pBar;            // now explicitly delete the control bar
				return;
			}
		}
		
		ASSERT(pBar != NULL);
		ASSERT_KINDOF(CControlBar, pBar);
		ASSERT(pBar->m_pDockContext != NULL);
		if(pBar->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
		{
			COXSizeViewBar* pViewBar=(COXSizeViewBar*)pBar;
			if(::IsWindow(pViewBar->m_hWnd) && !pViewBar->IsBeingDestroyed())
			{
				if(!pViewBar->QueryCloseView())
				{
					// document can't be closed right now -- don't close it
					m_bBeingDestroyed=FALSE;
					return;
				}
				
				pViewBar->m_pChildWnd->GetMDIFrame()->
					ShowControlBar(pViewBar,FALSE,FALSE);
				pViewBar->m_pDockContext->ToggleDocking();
				pViewBar->m_WrapperWnd.SendMessage(WM_CLOSE);
				return;
			}
		}
	}

    // otherwise just hide it.
    CMiniDockFrameWnd::OnClose();

	m_bBeingDestroyed=FALSE; 

    return;
}

BOOL COXSizableMiniDockFrameWnd::OnNcActivate(BOOL bActive)
{
	return GetMiniFrameSkin()->OnNcActivate(bActive, this);
}

void COXSizableMiniDockFrameWnd::OnParentNotify(UINT message, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	if(message==WM_LBUTTONDOWN || message==WM_RBUTTONDOWN || message==WM_MBUTTONDOWN)
	{
		// activate this window if necessary
		// CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at position 1
		// in the array
		CControlBar* pBar = ((COXSizeDockBar *)&m_wndDockBar)->GetFirstControlBar();
		pBar->SendMessage(WM_ACTIVATEVIEWBAR,TRUE,ID_SOURCE_MINIDOCK);
	}
}

void COXSizableMiniDockFrameWnd::OnInitMenuPopup(CMenu* pMenu, UINT, BOOL bSysMenu)
{
	AfxCancelModes(m_hWnd);

	if (bSysMenu)
	{
		// CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at position 1
		// in the array
		CControlBar* pBar=((COXSizeDockBar *)&m_wndDockBar)->GetFirstControlBar();
		if(pBar!=NULL)
		{
			ASSERT_KINDOF(CControlBar, pBar);
			if(pBar->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
			{
				COXMDIChildWndSizeDock* pMDIChild=((COXSizeViewBar*)pBar)->m_pChildWnd;
				if(::IsWindow(pMDIChild->m_hWnd))
				{
					if(pMenu->GetMenuState(pMDIChild->m_nDockMessageID,	
						MF_BYCOMMAND)==0xffffffff)
					{
						//HACK
//						pMenu->RemoveMenu(MF_BYPOSITION,2);

						pMenu->AppendMenu(MF_SEPARATOR);
						pMenu->AppendMenu(pMDIChild->m_bDockable ? 
							MF_CHECKED : MF_UNCHECKED, pMDIChild->m_nDockMessageID, 
							pMDIChild->m_sDockMenuItem);
					}
				}
			}
		}
	}
}

void COXSizableMiniDockFrameWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CMiniDockFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at 
	// position 1 in the array
	CControlBar* pBar=((COXSizeDockBar *)&m_wndDockBar)->GetFirstControlBar();
	if(pBar!=NULL)
	{
		ASSERT_KINDOF(CControlBar, pBar);
		ASSERT(pBar->m_pDockContext != NULL);
		if(pBar->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
		{
			pBar->SendMessage(WM_ACTIVATEVIEWBAR,nState==WA_INACTIVE ? FALSE : TRUE,
				ID_SOURCE_MINIDOCK);
		}
	}
}


BOOL COXSizableMiniDockFrameWnd::IsDockingViewBar()
{
	ASSERT_VALID(this);

	// CMiniDockFrameWnd class assumes if there is only 1 bar, then it's at 
	// position 1 in the array
	CControlBar* pBar=((COXSizeDockBar *)&m_wndDockBar)->GetFirstControlBar();
	if(pBar!=NULL)
	{
		ASSERT_KINDOF(CControlBar, pBar);
		ASSERT(pBar->m_pDockContext != NULL);
		if(pBar->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
			return TRUE;
	}

	return FALSE;
}

// convert MiniDock size to a client size.. or vice versa.
void MiniDockToClient(CRect & rect, BOOL bConvertToClient)
{
	static int nCaptionY = -1;
	static int nBorderX, nBorderY;
	
	// if not set up, create a temporary floating frame to see how big it is.
	if (nCaptionY == -1)
	{
		CFrameWnd* pMainFrame = (CFrameWnd*)AfxGetMainWnd();
		CMiniDockFrameWnd* pTmpFloatFrame = pMainFrame->CreateFloatingFrame(0);
		
		// calculate frame dragging rectangle
		CRect rcFloat(0,0,0,0);
		
		pTmpFloatFrame->CalcWindowRect(&rcFloat);
		
		rcFloat.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
		
		nCaptionY = - rcFloat.top;
		nBorderY = rcFloat.bottom;
		nBorderX = rcFloat.right;
		
		pTmpFloatFrame->DestroyWindow();
	}		
	
	if (bConvertToClient)
	{
		rect.left += nBorderX;
		rect.right -= nBorderX;
		rect.top += nCaptionY;
		rect.bottom -= nBorderY;
	}
	else
	{
		rect.left -= nBorderX;
		rect.right += nBorderX;
		rect.top -= nCaptionY;
		rect.bottom += nBorderY;
	}
}


// Helper function: used below
// adjusts the size, depending on the borders specified by the CControlBar style
void AdjustForBorders(CRect& rect, DWORD dwStyle)
{
	if (dwStyle & CBRS_BORDER_LEFT)
		rect.left -= afxData.cxBorder2;
	if (dwStyle & CBRS_BORDER_TOP)
		rect.top -= afxData.cyBorder2;
	if (dwStyle & CBRS_BORDER_RIGHT)
		rect.right += afxData.cxBorder2;
	if (dwStyle & CBRS_BORDER_BOTTOM)
		rect.bottom += afxData.cyBorder2;
}

void COXSizableMiniDockFrameWnd::OnNcPaint() 
{
	GetMiniFrameSkin()->OnNcPaint(this);
}

COXMiniFrameSkin* COXSizableMiniDockFrameWnd::GetMiniFrameSkin()
{
	// Check if the app is derived from COXSkinnedApp
	COXSkinnedApp* pSkinnedApp = DYNAMIC_DOWNCAST(COXSkinnedApp, AfxGetApp());
	if (pSkinnedApp != NULL && pSkinnedApp->GetCurrentSkin() != NULL)
		return pSkinnedApp->GetCurrentSkin()->GetMiniFrameSkin();
	else
	{
		// Create a classic skin for this class if not created already
		if (m_pMiniFrameSkin == NULL)
			m_pMiniFrameSkin = new COXMiniFrameSkinClassic();

		return m_pMiniFrameSkin;
	}
}


BOOL COXSizableMiniDockFrameWnd::OnSetCursor(CWnd* /*pWnd*/, UINT nHitTest, UINT /*message*/) 
{
	// Since the default implementation of CWnd::OnSetCursor(...) draws the close button
	// in the caption, we need to re-implement its logic here and not draw that button.
	
	// Code from CMiniDockFrameWnd::OnSetCursor(...)
	CFrameWnd* pFrameWnd = GetTopLevelFrame();
	ASSERT_VALID(pFrameWnd);
	if (pFrameWnd->m_bHelpMode)
	{
		SetCursor(afxData.hcurHelp);
		return TRUE;
	}
	// End of code from CMiniDockFrameWnd::OnSetCursor(...)
	
	// Implementation of the logic of CWnd::OnSetCursor(...)
	// Get the appropriate cursor
	HCURSOR hCursor;

	if (nHitTest == HTBOTTOM || nHitTest == HTTOP)
		hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZENS);

	else if (nHitTest == HTBOTTOMLEFT || nHitTest == HTTOPRIGHT)
		hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZENESW);

	else if (nHitTest == HTBOTTOMRIGHT || nHitTest == HTTOPLEFT)
		hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE);

	else if (nHitTest == HTRIGHT || nHitTest == HTLEFT)
		hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);

	else
		hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

	::SetCursor(hCursor);
	// End of implementation of the logic of CWnd::OnSetCursor(...)

	return TRUE;
}

void COXSizableMiniDockFrameWnd::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if (nHitTest == HTCLOSE && GetSystemMenu(FALSE)->GetMenuItemCount() > 0)
	{
		SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
	}
	else
		CMiniDockFrameWnd::OnNcLButtonUp(nHitTest, point);
}

// Update the buttons of the mini frame window when the mouse leaves
LRESULT CALLBACK COXSizableMiniDockFrameWnd::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return ::CallNextHookEx(m_hMouseHook, nCode, wParam, lParam);

	if (nCode == HC_ACTION && (wParam == WM_MOUSEMOVE || wParam == WM_NCMOUSEMOVE))
	{

		MOUSEHOOKSTRUCT* pMH = (MOUSEHOOKSTRUCT*) lParam;

		// If the previous message was for COXSizableMiniDockFrameWnd and the current is not
		// we need to update the caption buttons
		COXSizableMiniDockFrameWnd* pPrev = DYNAMIC_DOWNCAST(COXSizableMiniDockFrameWnd,
			CWnd::FromHandlePermanent(m_hwndPrevMouseMoveWnd));
		COXSizableMiniDockFrameWnd* pCurrent = DYNAMIC_DOWNCAST(COXSizableMiniDockFrameWnd,
			CWnd::FromHandlePermanent(pMH->hwnd));

		if (pPrev != NULL && pCurrent != pPrev)
		{
			// The mouse just left the mini frame window
			pPrev->GetMiniFrameSkin()->DrawCaptionButtons(pPrev);
		}
		m_hwndPrevMouseMoveWnd = pMH->hwnd;
	}

	return CallNextHookEx(m_hMouseHook, nCode, wParam, lParam);
}

void COXSizableMiniDockFrameWnd::OnDestroy() 
{
	// Unhook the mouse
	if (m_hMouseHook)
	{
		::UnhookWindowsHookEx(m_hMouseHook);
		m_hMouseHook = NULL;
	}

	CMiniDockFrameWnd::OnDestroy();
}

void COXSizableMiniDockFrameWnd::OnNcRButtonDown(UINT /*nHitTest*/, CPoint point) 
{
	// Display the system menu					
#if _MFC_VER < 0x0700		
	InvertSysMenu();
#endif
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	int nCmd = pSysMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, point.x, point.y, this);
#if _MFC_VER < 0x0700	
	InvertSysMenu();
#endif	
	if (nCmd != 0)
	{
		SendMessage(WM_SYSCOMMAND, nCmd, 0);
	}
	return;
}

void COXSizableMiniDockFrameWnd::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	GetMiniFrameSkin()->DrawCaptionButtons(this);	
	CMiniDockFrameWnd::OnNcMouseMove(nHitTest, point);
}
