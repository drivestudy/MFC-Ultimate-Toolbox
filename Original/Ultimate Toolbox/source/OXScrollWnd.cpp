// ==========================================================================
// 						Class Implementation : COXScrollWnd
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "OXScrollWnd.h"
#include <afxpriv.h>
#include "UTB64Bit.h"

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

#ifndef SPI_GETWHEELSCROLLLINES
	#define SPI_GETWHEELSCROLLLINES   104
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Special mapping modes just for COXScrollWnd implementation
#ifndef MM_NONE
#define MM_NONE					0
#endif


/////////////////////////////////////////////////////////////////////////////
// COXScrollWnd

IMPLEMENT_DYNCREATE(COXScrollWnd, CWnd)

COXScrollWnd::COXScrollWnd() :
	m_nMapMode(MM_NONE),
	m_nZoomLevel(100),		// 100%
	m_align(ZA_TOPLEFT),
	m_alignContents(CA_TOPLEFT),
	m_bIsInSmoothScrolling(FALSE),
	m_bAlwaysFitWindow(FALSE),
	m_bDisplayContextMenu(TRUE),
	m_bUseTrackZoom(FALSE),
	m_totalLog(0,0),
	m_rectLog(0,0,0,0),
	m_totalDev(0,0),
	m_pageDev(0,0),
	m_lineDev(0,0),
	m_sizeDev(0,0),
	m_nPagePercent(0),
	m_nLinePercent(0),
	m_bUseRelativeScrollSizes(FALSE),
	m_bInsideUpdate(FALSE)
{
	SetSmoothScrolling(FALSE);
	SetSmoothEnvironment();
}

COXScrollWnd::~COXScrollWnd()
{
}


BOOL COXScrollWnd::Initialize()
{
	BOOL bResult=FALSE;

#ifdef OXSCRLWND_USE_RULER
	bResult=AttachRuler();
#else
	bResult=TRUE;
#endif	//	OXSCRLWND_USE_RULER

	return bResult;
}


BEGIN_MESSAGE_MAP(COXScrollWnd, CWnd)
	//{{AFX_MSG_MAP(COXScrollWnd)
	ON_WM_HSCROLL()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_VSCROLL()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEACTIVATE()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDM_OX_SETZOOM10,IDM_OX_SIZETOCONTENT,OnChangeZoom)
	ON_COMMAND(IDM_OX_ALWAYSFITWINDOW,OnAlwaysFitWindow)
	ON_COMMAND(IDM_OX_USETRACKZOOM,OnUseTrackZoom)
	ON_COMMAND(IDM_OX_SMOOTHSCROLLING,OnSmoothScrolling)
#ifdef OXSCRLWND_USE_RULER
	ON_COMMAND(IDM_OX_SHOWHORZRULERBAR,OnShowHorzRulerBar)
	ON_COMMAND(IDM_OX_SHOWVERTRULERBAR,OnShowVertRulerBar)
	ON_COMMAND(IDM_OX_RULERBAR_INCHES,OnRulerBarUseInches)
	ON_COMMAND(IDM_OX_RULERBAR_CENTIMETERS,OnRulerBarUseCentimeters)
#endif	//	OXSCRLWND_USE_RULER
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COXScrollWnd message handlers

void COXScrollWnd::PreSubclassWindow()
{
	_AFX_THREAD_STATE* pThreadState=AfxGetThreadState();
	// hook not already in progress
	if(pThreadState->m_pWndInit==NULL)
	{
		if(!Initialize())
		{
			TRACE(_T("COXScrollWnd::PreSubclassWindow: failed to initialize the object!\n"));
		}
	}

	CWnd::PreSubclassWindow();
}


BOOL COXScrollWnd::PreTranslateMessage(MSG* pMsg)
{
	BOOL bProcessed=FALSE;
	if(pMsg->message==WM_KEYDOWN)
	{
		bProcessed=
			HandleKeyInput(PtrToUint(pMsg->wParam),LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));
	}

	if(bProcessed)
	{
		return TRUE;
	}
	else
	{
		return CWnd::PreTranslateMessage(pMsg);
	}
}


BOOL COXScrollWnd::HandleKeyInput(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	UNREFERENCED_PARAMETER(nRepCnt);
	UNREFERENCED_PARAMETER(nFlags);

	BOOL bHandled=TRUE;
	switch(nChar)
	{
	case VK_UP:
		{
			SendMessage(WM_VSCROLL,MAKEWPARAM(SB_LINEUP,0),NULL);
			break;
		}
	case VK_DOWN:
		{
			SendMessage(WM_VSCROLL,MAKEWPARAM(SB_LINEDOWN,0),NULL);
			break;
		}
	case VK_PRIOR:
		{
			SendMessage(WM_VSCROLL,MAKEWPARAM(SB_PAGEUP,0),NULL);
			break;
		}
	case VK_NEXT:
		{
			SendMessage(WM_VSCROLL,MAKEWPARAM(SB_PAGEDOWN,0),NULL);
			break;
		}
	case VK_LEFT:
		{
			SendMessage(WM_HSCROLL,MAKEWPARAM(SB_LINELEFT,0),NULL);
			break;
		}
	case VK_RIGHT:
		{
			SendMessage(WM_HSCROLL,MAKEWPARAM(SB_LINERIGHT,0),NULL);
			break;
		}
	case VK_HOME:
		{
			if(::GetKeyState(VK_CONTROL)<0)
			{
				SendMessage(WM_VSCROLL,MAKEWPARAM(SB_TOP,0),NULL);
			}
			else
			{
				SendMessage(WM_HSCROLL,MAKEWPARAM(SB_LEFT,0),NULL);
			}
			break;
		}
	case VK_END:
		{
			if(::GetKeyState(VK_CONTROL)<0)
			{
				SendMessage(WM_VSCROLL,MAKEWPARAM(SB_BOTTOM,0),NULL);
			}
			else
			{
				SendMessage(WM_HSCROLL,MAKEWPARAM(SB_RIGHT,0),NULL);
			}
			break;
		}
	case VK_TAB:
		{
			if(::GetKeyState(VK_SHIFT)<0)
			{
				SendMessage(WM_HSCROLL,MAKEWPARAM(SB_PAGELEFT,0),NULL);
			}
			else
			{
				SendMessage(WM_HSCROLL,MAKEWPARAM(SB_PAGERIGHT,0),NULL);
			}
			break;
		}
	default:
		{
			bHandled=FALSE;
			break;
		}
	}

	return bHandled;
}


int COXScrollWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CWnd::OnCreate(lpCreateStruct)==-1)
		return -1;

	if(!Initialize())
	{
		TRACE(_T("COXScrollWnd::OnCreate: failed to initialize the object!\n"));
		return -1;
	}

	return 0;
}

void COXScrollWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
//	ASSERT(pScrollBar == GetScrollBarCtrl(SB_HORZ));    // may be null
//	UNUSED(pScrollBar);
	UNREFERENCED_PARAMETER(pScrollBar);

	OnScroll(MAKEWORD(nSBCode, -1), nPos);
}

void COXScrollWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
//	ASSERT(pScrollBar == GetScrollBarCtrl(SB_VERT));    // may be null
//	UNUSED(pScrollBar);
	UNREFERENCED_PARAMETER(pScrollBar);

	OnScroll(MAKEWORD(-1, nSBCode), nPos);
}

void COXScrollWnd::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here

	if((lpwndpos->flags & SWP_NOSIZE)==0)
	{
		if(IsAlwaysFitWindow())
		{
			ZoomToWindow();
		}
		else if(m_nMapMode!=MM_NONE)
		{
			UpdateBars(FALSE);
		}
	}
}

void COXScrollWnd::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	if(!GetDisplayContextMenu())
	{
		CWnd::OnContextMenu(pWnd,point);
		return;
	}

	if(pWnd==this)
	{
		// create popup menu
		//
		CMenu menu;
		CMenu menuZoom;
#ifdef OXSCRLWND_USE_RULER
		CMenu menuRulerBars;
#endif	//	OXSCRLWND_USE_RULER
		if(menu.CreatePopupMenu())
		{
			int nZoomLevel=GetZoomLevel();

			CString sItem;
			if(menuZoom.CreatePopupMenu())
			{
				VERIFY(sItem.LoadString(IDS_OX_SETZOOM10));
				menuZoom.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_SETZOOM10,
					sItem);
				VERIFY(sItem.LoadString(IDS_OX_SETZOOM25));
				menuZoom.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_SETZOOM25,
					sItem);
				VERIFY(sItem.LoadString(IDS_OX_SETZOOM50));
				menuZoom.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_SETZOOM50,
					sItem);
				VERIFY(sItem.LoadString(IDS_OX_SETZOOM100));
				menuZoom.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_SETZOOM100,
					sItem);
				VERIFY(sItem.LoadString(IDS_OX_SETZOOM200));
				menuZoom.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_SETZOOM200,
					sItem);
				VERIFY(sItem.LoadString(IDS_OX_SETZOOM300));
				menuZoom.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_SETZOOM300,
					sItem);
				VERIFY(sItem.LoadString(IDS_OX_SETZOOM400));
				menuZoom.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_SETZOOM400,
					sItem);
				VERIFY(sItem.LoadString(IDS_OX_SETZOOM500));
				menuZoom.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_SETZOOM500,
					sItem);
				VERIFY(sItem.LoadString(IDS_OX_SETZOOM600));
				menuZoom.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_SETZOOM600,
					sItem);
				VERIFY(sItem.LoadString(IDS_OX_SETZOOM700));
				menuZoom.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_SETZOOM700,
					sItem);
				VERIFY(sItem.LoadString(IDS_OX_SETZOOM800));
				menuZoom.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_SETZOOM800,
					sItem);
				VERIFY(sItem.LoadString(IDS_OX_SETZOOM900));
				menuZoom.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_SETZOOM900,
					sItem);
				VERIFY(sItem.LoadString(IDS_OX_SETZOOM1000));
				menuZoom.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_SETZOOM1000,
					sItem);

				VERIFY(sItem.LoadString(IDS_OX_ZOOMPOPUPMENU));
				menu.AppendMenu(MF_STRING|MF_POPUP,
					(UINT_PTR)(HMENU)menuZoom,sItem);

				menuZoom.CheckMenuItem(IDM_OX_SETZOOM10,MF_BYCOMMAND|
					((nZoomLevel==10) ? MF_CHECKED : MF_UNCHECKED));
				menuZoom.CheckMenuItem(IDM_OX_SETZOOM25,MF_BYCOMMAND|
					((nZoomLevel==25) ? MF_CHECKED : MF_UNCHECKED));
				menuZoom.CheckMenuItem(IDM_OX_SETZOOM50,MF_BYCOMMAND|
					((nZoomLevel==50) ? MF_CHECKED : MF_UNCHECKED));
				menuZoom.CheckMenuItem(IDM_OX_SETZOOM100,MF_BYCOMMAND|
					((nZoomLevel==100) ? MF_CHECKED : MF_UNCHECKED));
				menuZoom.CheckMenuItem(IDM_OX_SETZOOM200,MF_BYCOMMAND|
					((nZoomLevel==200) ? MF_CHECKED : MF_UNCHECKED));
				menuZoom.CheckMenuItem(IDM_OX_SETZOOM300,MF_BYCOMMAND|
					((nZoomLevel==300) ? MF_CHECKED : MF_UNCHECKED));
				menuZoom.CheckMenuItem(IDM_OX_SETZOOM400,MF_BYCOMMAND|
					((nZoomLevel==400) ? MF_CHECKED : MF_UNCHECKED));
				menuZoom.CheckMenuItem(IDM_OX_SETZOOM500,MF_BYCOMMAND|
					((nZoomLevel==500) ? MF_CHECKED : MF_UNCHECKED));
				menuZoom.CheckMenuItem(IDM_OX_SETZOOM600,MF_BYCOMMAND|
					((nZoomLevel==600) ? MF_CHECKED : MF_UNCHECKED));
				menuZoom.CheckMenuItem(IDM_OX_SETZOOM700,MF_BYCOMMAND|
					((nZoomLevel==700) ? MF_CHECKED : MF_UNCHECKED));
				menuZoom.CheckMenuItem(IDM_OX_SETZOOM800,MF_BYCOMMAND|
					((nZoomLevel==800) ? MF_CHECKED : MF_UNCHECKED));
				menuZoom.CheckMenuItem(IDM_OX_SETZOOM900,MF_BYCOMMAND|
					((nZoomLevel==900) ? MF_CHECKED : MF_UNCHECKED));
				menuZoom.CheckMenuItem(IDM_OX_SETZOOM1000,MF_BYCOMMAND|
					((nZoomLevel==1000) ? MF_CHECKED : MF_UNCHECKED));
			}
			
			VERIFY(sItem.LoadString(IDS_OX_SIZETOCONTENT));
			menu.AppendMenu(MF_STRING,IDM_OX_SIZETOCONTENT,sItem);
			if(nZoomLevel!=100 && !IsAlwaysFitWindow())
			{
				VERIFY(sItem.LoadString(IDS_OX_SETZOOM100));
				menu.AppendMenu(MF_STRING,IDM_OX_SETZOOM100,sItem);
			}

			menu.AppendMenu(MF_SEPARATOR);
			
			VERIFY(sItem.LoadString(IDS_OX_ALWAYSFITWINDOW));
			menu.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_ALWAYSFITWINDOW,
				sItem);
			menu.CheckMenuItem(IDM_OX_ALWAYSFITWINDOW,MF_BYCOMMAND|
					(IsAlwaysFitWindow() ? MF_CHECKED : MF_UNCHECKED));

			VERIFY(sItem.LoadString(IDS_OX_USETRACKZOOM));
			menu.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_USETRACKZOOM,
				sItem);
			menu.CheckMenuItem(IDM_OX_USETRACKZOOM,MF_BYCOMMAND|
					(GetUseTrackZoom() ? MF_CHECKED : MF_UNCHECKED));

			VERIFY(sItem.LoadString(IDS_OX_SMOOTHSCROLLING));
			menu.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_SMOOTHSCROLLING,
				sItem);
			menu.CheckMenuItem(IDM_OX_SMOOTHSCROLLING,MF_BYCOMMAND|
				(IsSmoothScrolling() ? MF_CHECKED : MF_UNCHECKED));

			if(IsAlwaysFitWindow())
			{
				menu.EnableMenuItem(0,MF_BYPOSITION|MF_GRAYED);
				menu.EnableMenuItem(IDM_OX_SIZETOCONTENT,MF_BYCOMMAND|MF_GRAYED);
			}

#ifdef OXSCRLWND_USE_RULER
			if(m_ruler.IsAttached() && menuRulerBars.CreatePopupMenu())
			{
				menu.AppendMenu(MF_SEPARATOR);
				VERIFY(sItem.LoadString(IDS_OX_SHOWHORZRULERBAR));
				menuRulerBars.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_SHOWHORZRULERBAR,
					sItem);
				VERIFY(sItem.LoadString(IDS_OX_SHOWVERTRULERBAR));
				menuRulerBars.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_SHOWVERTRULERBAR,
					sItem);
				menuRulerBars.AppendMenu(MF_SEPARATOR);
				VERIFY(sItem.LoadString(IDS_OX_RULERBAR_INCHES));
				menuRulerBars.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_RULERBAR_INCHES,
					sItem);
				VERIFY(sItem.LoadString(IDS_OX_RULERBAR_CENTIMETERS));
				menuRulerBars.AppendMenu(MF_STRING|MF_CHECKED,IDM_OX_RULERBAR_CENTIMETERS,
					sItem);

				VERIFY(sItem.LoadString(IDS_OX_RULERBARPOPUPMENU));
				menu.AppendMenu(MF_STRING|MF_POPUP,
					(UINT_PTR)(HMENU)menuRulerBars,sItem);

				menuRulerBars.CheckMenuItem(IDM_OX_SHOWHORZRULERBAR,MF_BYCOMMAND|
					(m_ruler.GetShowHorzRulerBar() ? MF_CHECKED : MF_UNCHECKED));
				menuRulerBars.CheckMenuItem(IDM_OX_SHOWVERTRULERBAR,MF_BYCOMMAND|
					(m_ruler.GetShowVertRulerBar() ? MF_CHECKED : MF_UNCHECKED));
				menuRulerBars.CheckMenuItem(IDM_OX_RULERBAR_INCHES,MF_BYCOMMAND|
					((m_ruler.GetHorzRulerBar()->GetCalibrate()==100 && 
					m_ruler.GetVertRulerBar()->GetCalibrate()==100) ? 
					MF_CHECKED : MF_UNCHECKED));
				menuRulerBars.CheckMenuItem(IDM_OX_RULERBAR_CENTIMETERS,MF_BYCOMMAND|
					((m_ruler.GetHorzRulerBar()->GetCalibrate()==254 && 
					m_ruler.GetVertRulerBar()->GetCalibrate()==254) ? 
					MF_CHECKED : MF_UNCHECKED));
			}
#endif	//	OXSCRLWND_USE_RULER
		}

		if(OnPopulateContextMenu(&menu,point))
		{
			menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,
				point.x,point.y,this);
		}
	}
}

void COXScrollWnd::OnChangeZoom(UINT nID)
{
	int nNewZoomLevel=0;
	switch(nID)
	{
	case IDM_OX_SETZOOM10:
		nNewZoomLevel=10;
		break;
	case IDM_OX_SETZOOM25:
		nNewZoomLevel=25;
		break;
	case IDM_OX_SETZOOM50:
		nNewZoomLevel=50;
		break;
	case IDM_OX_SETZOOM100:
		nNewZoomLevel=100;
		break;
	case IDM_OX_SETZOOM200:
		nNewZoomLevel=200;
		break;
	case IDM_OX_SETZOOM300:
		nNewZoomLevel=300;
		break;
	case IDM_OX_SETZOOM400:
		nNewZoomLevel=400;
		break;
	case IDM_OX_SETZOOM500:
		nNewZoomLevel=500;
		break;
	case IDM_OX_SETZOOM600:
		nNewZoomLevel=600;
		break;
	case IDM_OX_SETZOOM700:
		nNewZoomLevel=700;
		break;
	case IDM_OX_SETZOOM800:
		nNewZoomLevel=800;
		break;
	case IDM_OX_SETZOOM900:
		nNewZoomLevel=900;
		break;
	case IDM_OX_SETZOOM1000:
		nNewZoomLevel=1000;
		break;
	case IDM_OX_SIZETOCONTENT:
		nNewZoomLevel=0;
		break;
	default:
		ASSERT(FALSE);
	}

	if(nNewZoomLevel==0)
	{
		ZoomToWindow();
	}
	else
	{
		ASSERT(nNewZoomLevel>=MINZOOM && nNewZoomLevel<=MAXZOOM);
		SetZoomLevel(nNewZoomLevel);
	}
}


void COXScrollWnd::OnAlwaysFitWindow()
{
	SetAlwaysFitWindow(!IsAlwaysFitWindow());
}


void COXScrollWnd::OnUseTrackZoom()
{
	SetUseTrackZoom(!GetUseTrackZoom());
}


void COXScrollWnd::OnSmoothScrolling()
{
	SetSmoothScrolling(!IsSmoothScrolling());
}


#ifdef OXSCRLWND_USE_RULER
void COXScrollWnd::OnShowHorzRulerBar()
{
	m_ruler.SetShowHorzRulerBar(!GetShowHorzRulerBar());
}

void COXScrollWnd::OnShowVertRulerBar()
{
	m_ruler.SetShowVertRulerBar(!GetShowVertRulerBar());
}

void COXScrollWnd::OnRulerBarUseInches()
{
	m_ruler.GetHorzRulerBar()->SetCalibrate(100,FALSE);
	m_ruler.GetVertRulerBar()->SetCalibrate(100,FALSE);
	m_ruler.RedrawAttached();
}

void COXScrollWnd::OnRulerBarUseCentimeters()
{
	m_ruler.GetHorzRulerBar()->SetCalibrate(254,FALSE);
	m_ruler.GetVertRulerBar()->SetCalibrate(254,FALSE);
	m_ruler.RedrawAttached();
}
#endif	//	OXSCRLWND_USE_RULER


void COXScrollWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);
	if(GetUseTrackZoom() && !IsAlwaysFitWindow())
	{
		CRectTracker track;
		track.m_sizeMin=CSize(5,5);
		if(track.TrackRubberBand(this,point,TRUE))
		{
			CRect rectTrack=track.m_rect;
			rectTrack.NormalizeRect();

			CRect rectClient;
			GetClientRect(rectClient);
			CRect rectDisplayed=rectClient;
			if(rectDisplayed.Width()>m_totalDev.cx)
				rectDisplayed.right=rectDisplayed.left+m_totalDev.cx;
			if(rectDisplayed.Height()>m_totalDev.cy)
				rectDisplayed.bottom=rectDisplayed.top+m_totalDev.cy;

			rectTrack.IntersectRect(rectTrack,rectDisplayed);
			if(!rectTrack.IsRectEmpty() && rectTrack.Width()>=8 && rectTrack.Height()>=8)
			{
				CPoint ptScrollPos=GetScrollPosition(ZA_TOPLEFT);

				int nOldZoomLevel=GetZoomLevel();
				int nNewZoomLevel=
					__min((nOldZoomLevel*rectClient.Width())/rectTrack.Width(),
					(nOldZoomLevel*rectClient.Height())/rectTrack.Height());
				SetZoomLevel(nNewZoomLevel,ZA_TOPLEFT);
	
				ptScrollPos.x+=((rectTrack.left-rectClient.left)*100)/nOldZoomLevel;
				ptScrollPos.y+=((rectTrack.top-rectClient.top)*100)/nOldZoomLevel;
				ScrollToPosition(ptScrollPos,ZA_TOPLEFT);
			}
		}
	}
}


int COXScrollWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	UNREFERENCED_PARAMETER(pDesktopWnd);
	UNREFERENCED_PARAMETER(nHitTest);
	UNREFERENCED_PARAMETER(message);
	SetFocus();
	return MA_NOACTIVATE;
}


///////////////////////////////////////////////////////////////////////////

void COXScrollWnd::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	UNREFERENCED_PARAMETER(pInfo);

	ASSERT_VALID(pDC);

	ASSERT(m_totalDev.cx>=0 && m_totalDev.cx>=0);
	SetMapMode(pDC);

	CPoint ptVpOrg(0, 0);       // assume no shift for printing
	if (!pDC->IsPrinting())
	{
		ptVpOrg=GetOrigin(pDC);
		// Set the logical origin, do it yourself when printing !!!
		pDC->SetWindowOrg(m_rectLog.TopLeft());
	}
	pDC->SetViewportOrg(ptVpOrg);
}


CPoint COXScrollWnd::GetOrigin(CDC* pDC/*=NULL*/) const
{
	BOOL bDeleteDC=FALSE;
	if(pDC==NULL)
	{
		CWnd* pWnd=(CWnd*)this;
		pDC=new CClientDC(pWnd);
		bDeleteDC=TRUE;
	}
	
	ScaleViewport(pDC);

	CRect rect;
	GetClientRect(&rect);

	CPoint ptOrigin(0,0);
	// by default shift viewport origin in negative direction of scroll
	ptOrigin=-GetDeviceScrollPosition();

	switch(m_alignContents)
	{
	case CA_TOPLEFT:
		break;

	case CA_TOPCENTER:
		if(m_totalDev.cx<rect.Width())
		{
			ptOrigin.x=(rect.Width()-m_totalDev.cx)/2;
		}
		break;

	case CA_TOPRIGHT:
		if(m_totalDev.cx<rect.Width())
		{
			ptOrigin.x=rect.Width()-m_totalDev.cx;
		}
		break;

	case CA_CENTERLEFT:
		if(m_totalDev.cy<rect.Height())
		{
			ptOrigin.y=(rect.Height()-m_totalDev.cy)/2;
		}
		break;

	case CA_CENTER:
		if(m_totalDev.cx<rect.Width())
		{
			ptOrigin.x=(rect.Width()-m_totalDev.cx)/2;
		}
		if(m_totalDev.cy<rect.Height())
		{
			ptOrigin.y=(rect.Height()-m_totalDev.cy)/2;
		}
		break;

	case CA_CENTERRIGHT:
		if(m_totalDev.cy<rect.Height())
		{
			ptOrigin.y=(rect.Height()-m_totalDev.cy)/2;
		}
		if(m_totalDev.cx<rect.Width())
		{
			ptOrigin.x=rect.Width()-m_totalDev.cx;
		}
		break;

	case CA_BOTTOMLEFT:
		if(m_totalDev.cy<rect.Height())
		{
			ptOrigin.y=rect.Height()-m_totalDev.cy;
		}
		break;

	case CA_BOTTOMCENTER:
		if(m_totalDev.cx<rect.Width())
		{
			ptOrigin.x=(rect.Width()-m_totalDev.cx)/2;
		}
		if(m_totalDev.cy<rect.Height())
		{
			ptOrigin.y=rect.Height()-m_totalDev.cy;
		}
		break;

	case CA_BOTTOMRIGHT:
		if(m_totalDev.cy<rect.Height())
		{
			ptOrigin.y=rect.Height()-m_totalDev.cy;
		}
		if(m_totalDev.cx<rect.Width())
		{
			ptOrigin.x=rect.Width()-m_totalDev.cx;
		}
		break;

	default:
		ASSERT(FALSE);
	}

	if(bDeleteDC)
	{
		delete pDC;
	}

	return ptOrigin;
}


/////////////////////////////////////////////////////////////////////////////
// Set mode and scaling/scrolling sizes

void COXScrollWnd::SetDeviceScrollSizesRelative(int nMapMode, 
												SIZE sizeTotal,
												int nPagePercent, 
												int nLinePercent)
{
	// synthesize a rectangle from the size
	CRect rectLog( CPoint(0,0), sizeTotal );
	if( nMapMode != MM_TEXT )
		rectLog.bottom = -rectLog.bottom;
	// call overloaded function for real work
	SetDeviceScrollSizesRelative( nMapMode, rectLog, nPagePercent, 
		nLinePercent );
}

void COXScrollWnd::SetDeviceScrollSizesRelative(SIZE sizeDevice, 
												const CRect& rectContents,
												int nPagePercent, 
												int nLinePercent)
{
	ASSERT( sizeDevice.cy > 0 );
	if( rectContents.Height() < 0 )
		sizeDevice.cy = -sizeDevice.cy;
	m_sizeDev = sizeDevice;
	SetDeviceScrollSizesRelative(MM_ANISOTROPIC,rectContents,
		nPagePercent,nLinePercent);
}


void COXScrollWnd::SetDeviceScrollSizesRelative(int nMapMode, 
												const CRect& rectContents,
												int nPagePercent, 
												int nLinePercent)
{
	ASSERT( nLinePercent > 0 );
	ASSERT( nPagePercent >= nLinePercent );
	ASSERT( nLinePercent <= 100 );
	ASSERT( nPagePercent <= 100 );
	m_bUseRelativeScrollSizes = TRUE;
	m_nPagePercent = nPagePercent;
	m_nLinePercent = nLinePercent;
	m_rectLog = rectContents;
	m_totalLog.cx = m_rectLog.Width();
	m_totalLog.cy = m_rectLog.Height();
	if( m_totalLog.cy < 0 )
		m_totalLog.cy = -m_totalLog.cy;
	ASSERT(nMapMode > 0);
	ASSERT(nMapMode != MM_ISOTROPIC);
    
	int nOldMapMode = m_nMapMode;
	m_nMapMode = nMapMode;

	//BLOCK: convert logical coordinate space to device coordinates
	{
		CWindowDC dc(NULL);
		SetMapMode( &dc );
		ScaleViewport( &dc );
		m_totalDev = m_totalLog;
		dc.LPtoDP((LPPOINT)&m_totalDev);
		if (m_totalDev.cy < 0)
			m_totalDev.cy = -m_totalDev.cy;
	} // release DC here

	// now adjust device specific sizes
	ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);

	if (m_hWnd != NULL)
	{
		// window has been created, invalidate now
		UpdateBars();
		if (nOldMapMode != m_nMapMode)
			Invalidate(TRUE);
	}
}

// maintained for backward compatibility
void COXScrollWnd::SetScrollSizes(int nMapMode, SIZE sizeTotal,
								  const SIZE& sizePage, const SIZE& sizeLine)
{
	m_bUseRelativeScrollSizes = FALSE;
	ASSERT(sizeTotal.cx >= 0 && sizeTotal.cy >= 0);
	ASSERT(nMapMode > 0);
	ASSERT(nMapMode != MM_ISOTROPIC && nMapMode != MM_ANISOTROPIC);
    
	int nOldMapMode = m_nMapMode;
	m_nMapMode = nMapMode;
	m_totalLog = sizeTotal;
	// synthesize a rectangle from the size
	m_rectLog = CRect( CPoint(0,0), sizeTotal );
	if( m_nMapMode != MM_TEXT )
		m_rectLog.bottom = -m_rectLog.bottom;

	//BLOCK: convert logical coordinate space to device coordinates
	{
		CWindowDC dc(NULL);
		ASSERT(m_nMapMode > 0);
		SetMapMode( &dc );
        // leave page and line sizes unaffected from zooming
		m_pageDev = sizePage;
		dc.LPtoDP((LPPOINT)&m_pageDev);
		m_lineDev = sizeLine;
		dc.LPtoDP((LPPOINT)&m_lineDev);
		// total size
		ScaleViewport( &dc );
		m_totalDev = m_totalLog;
		dc.LPtoDP((LPPOINT)&m_totalDev);
		if (m_totalDev.cy < 0)
			m_totalDev.cy = -m_totalDev.cy;
		if (m_pageDev.cy < 0)
			m_pageDev.cy = -m_pageDev.cy;
		if (m_lineDev.cy < 0)
			m_lineDev.cy = -m_lineDev.cy;
	} // release DC here

	// now adjust device specific sizes
	ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);
	if (m_pageDev.cx == 0)
		m_pageDev.cx = m_totalDev.cx / 10;
	if (m_pageDev.cy == 0)
		m_pageDev.cy = m_totalDev.cy / 10;
	if (m_lineDev.cx == 0)
		m_lineDev.cx = m_pageDev.cx / 10;
	if (m_lineDev.cy == 0)
		m_lineDev.cy = m_pageDev.cy / 10;

	if (m_hWnd != NULL)
	{
		// window has been created, invalidate now
		UpdateBars();
		if (nOldMapMode != m_nMapMode)
			Invalidate(TRUE);
	}
}

// Zoom Ops

void COXScrollWnd::ZoomToWindow()
{
	int nZoomLevel=GetZoomLevel();
	{
		CClientDC dc(this);
		ASSERT(m_nMapMode > 0);
		SetMapMode( &dc );

		CSize	sizeSb, sizeZoom, sizeClient;
		long temp;
		GetTrueClientSize(sizeClient,sizeSb);
    
		CRect rectClient;
		GetClientRect(rectClient);
		rectClient.right=rectClient.left+sizeClient.cx;
		rectClient.bottom=rectClient.top+sizeClient.cy;
		dc.DPtoLP( rectClient );
		sizeClient=rectClient.Size();
		if( sizeClient.cy < 0 ) 
    		sizeClient.cy = -sizeClient.cy;

		temp = (long)((float)100 * (float)sizeClient.cx / 
			(float)m_totalLog.cx);
		if( temp > (long)MAXZOOM )
    		sizeZoom.cx = MAXZOOM;
		else
    		sizeZoom.cx = (int)temp;
		temp = (long)((float)100 * (float)sizeClient.cy / 
			(float)m_totalLog.cy);
		if( temp > (long)MAXZOOM )
    		sizeZoom.cy = MAXZOOM;
		else
    		sizeZoom.cy = (int)temp;
			
		// use the lower value
		if( sizeZoom.cy < sizeZoom.cx )
    		nZoomLevel = sizeZoom.cy;
		else
    		nZoomLevel = sizeZoom.cx;
		if ( nZoomLevel < MINZOOM )
    		nZoomLevel = MINZOOM;
	}

	SetZoomLevel(nZoomLevel);
}


void COXScrollWnd::ZoomToRectangle( CRect rectZoom, ZoomAlignment Align )
{
	int nZoomLevel=GetZoomLevel();
	{
		CClientDC dc(this);
		OnPrepareDC( &dc );

		// clip zoom rectangle to document size
		CRect rectDevDoc = m_rectLog;
		dc.LPtoDP( rectDevDoc );
		if( !rectZoom.IntersectRect( rectZoom, rectDevDoc ) )
			return;		// competely outside of the document, ignore
		dc.DPtoLP( rectZoom );

		// check again in logical coords
		if( 0 == rectZoom.Width() )
			return;
		if( 0 == rectZoom.Height() )
			return;

		CSize sizeZoom=rectZoom.Size();
		if( sizeZoom.cy < 0 ) 
    		sizeZoom.cy = -sizeZoom.cy;

		CSize	sizeSb, sizeClient;
		GetTrueClientSize(sizeClient,sizeSb);
    
		CRect rectClient;
		GetClientRect(rectClient);
		rectClient.right=rectClient.left+sizeClient.cx;
		rectClient.bottom=rectClient.top+sizeClient.cy;
	    SetMapMode( &dc );	// assume 100%
		dc.DPtoLP( rectClient );
		sizeClient=rectClient.Size();
		if( sizeClient.cy < 0 ) 
    		sizeClient.cy = -sizeClient.cy;

		long temp = (long)((float)100 * (float)sizeClient.cx / 
			(float)sizeZoom.cx);
		if( temp > (long)MAXZOOM )
    		sizeZoom.cx = MAXZOOM;
		else
    		sizeZoom.cx = (int)temp;
		temp = (long)((float)100 * (float)sizeClient.cy / 
			(float)sizeZoom.cy);
		if( temp > (long)MAXZOOM )
    		sizeZoom.cy = MAXZOOM;
		else
    		sizeZoom.cy = (int)temp;
		// use the lower value
		if( sizeZoom.cy < sizeZoom.cx )
    		nZoomLevel = sizeZoom.cy;
		else
    		nZoomLevel = sizeZoom.cx;
		if ( nZoomLevel < MINZOOM )
    		nZoomLevel = MINZOOM;
	}

	SetZoomLevel(nZoomLevel);

	CPoint	ptAlign;
	if (m_hWnd != NULL)
	{
		if( Align == ZA_DEFAULT )
			Align = m_align;
		switch( Align )
		{
			case ZA_TOPLEFT:
				ptAlign.x = rectZoom.left;
				ptAlign.y = rectZoom.top;
				break;
					
			case ZA_BOTTOMLEFT:
				ptAlign.x = rectZoom.left;
				ptAlign.y = rectZoom.bottom;
				break;
					
			case ZA_CENTER:
				ptAlign.x = (rectZoom.left + rectZoom.right)/2;
				ptAlign.y = (rectZoom.top + rectZoom.bottom)/2;
				break;
		}
		ScrollToPosition( ptAlign, Align );
		UpdateBars();
		Invalidate(TRUE);
	}
}


int COXScrollWnd::SetZoomLevel( int nNewLevel, ZoomAlignment Align )
{
	int nOldLevel=m_nZoomLevel;
	if(Align==ZA_DEFAULT)
		Align=m_align;
	CPoint ptCurrent=GetScrollPosition( Align );
	if( nNewLevel < MINZOOM )
		nNewLevel = MINZOOM;
	if( nNewLevel > MAXZOOM )
		nNewLevel = MAXZOOM;
	m_nZoomLevel = nNewLevel;
	if( nOldLevel != nNewLevel )
	{
		//BLOCK: convert logical coordinate space to device coordinates
		{
			CWindowDC dc(NULL);
			ASSERT(m_nMapMode > 0);
			SetMapMode( &dc );
			// total size
			ScaleViewport( &dc );
			m_totalDev = m_totalLog;
			dc.LPtoDP((LPPOINT)&m_totalDev);
			if (m_totalDev.cy < 0)
				m_totalDev.cy = -m_totalDev.cy;
		} // release DC here
	
		ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);
	
		if (m_hWnd != NULL)
		{
			// window exists, invalidate now
			ScrollToPosition( ptCurrent, Align );
			UpdateBars(FALSE);
			Invalidate(TRUE);
		}
	}

#ifdef OXSCRLWND_USE_RULER
	m_ruler.ZoomRuler(m_nZoomLevel,TRUE);
	m_ruler.ZoomRuler(m_nZoomLevel,FALSE);
#endif	//	OXSCRLWND_USE_RULER

	return nOldLevel;
}

// zoom helper functions

void COXScrollWnd::SetMapMode( CDC* pDC ) const
{
	ASSERT(m_nMapMode > 0);
	pDC->SetMapMode(m_nMapMode);
    if( m_nMapMode == MM_ANISOTROPIC )
    {                     
    	ASSERT( m_sizeDev.cx >= 10 );
    	ASSERT( m_sizeDev.cy >= 10 || m_sizeDev.cy <= -10 );
    	ASSERT( m_sizeDev.cx < 3200 );
    	ASSERT( m_sizeDev.cy < 3200 || m_sizeDev.cy >= -3200 );
    	pDC->SetWindowExt( m_totalLog );
    	pDC->SetViewportExt( m_sizeDev );
    }
    else
    {
		// unlock mapping mode for zooming, extents of previous map mode are inherited
		pDC->SetMapMode( MM_ANISOTROPIC );
		if( m_nMapMode == MM_TEXT )
		{
			// current extents are 1,1 which are too small to zoom down
			pDC->ScaleWindowExt( 1000, 1, 1000, 1 );
			pDC->ScaleViewportExt( 1000, 1, 1000, 1 );
		}
    }
}

void COXScrollWnd::SetZoomAlign( ZoomAlignment align )
{
#ifdef _DEBUG
	if( align == ZA_DEFAULT )
		TRACE(_T("You cannot set the ZoomAlignment to ZA_DEFAULT"));
#endif
	if( align != ZA_DEFAULT )
		m_align = align;
}

/////////////////////////////////////////////////////////////////////////////
// Getting information

CPoint COXScrollWnd::GetScrollPosition( ZoomAlignment Align ) const   // logical coordinates
{
	CPoint pt = GetDeviceScrollPosition();
	// pt may be negative if m_bCenter is set
	
	CRect	rectClient;
	GetClientRect( rectClient );
	DWORD dwStyle = GetStyle();
	BOOL bHasV, bHasH;
    bHasV = (dwStyle & WS_VSCROLL) != 0 || GetScrollBarCtrl(SB_VERT) != NULL;
    bHasH = (dwStyle & WS_HSCROLL) != 0 || GetScrollBarCtrl(SB_HORZ) != NULL;
    
	if( Align == ZA_DEFAULT )
		Align = m_align;
	switch ( Align )
	{
		case ZA_BOTTOMLEFT:
			if( bHasV )
				pt.y += rectClient.Height();
			else
				pt.y = m_totalDev.cy - 1;
			break;
			    
		case ZA_CENTER:
			if( bHasV )
				pt.y += rectClient.Height()/2;
			else
				pt.y = m_totalDev.cy/2;
				
			if( bHasH )
				pt.x += rectClient.Width()/2;
			else
				pt.x = m_totalDev.cx/2;
			break;
	}
	
	ASSERT(m_nMapMode > 0); // must be set
	CWindowDC dc(NULL);
	SetMapMode( &dc );
	ScaleViewport( &dc );
	// CRect::TopLeft() doesn't work with const CRects
	dc.SetWindowOrg( m_rectLog.left, m_rectLog.top );
	dc.DPtoLP((LPPOINT)&pt);
	return pt;
}

void COXScrollWnd::ScrollToPosition(POINT pt, ZoomAlignment Align)   // logical coordinates
{
	ASSERT(m_nMapMode > 0);     // not allowed
	CWindowDC dc(NULL);
	SetMapMode( &dc );
	ScaleViewport( &dc );
	dc.SetWindowOrg( m_rectLog.TopLeft() );
	dc.LPtoDP((LPPOINT)&pt);
	
	CRect rectClient;
	GetClientRect( rectClient );

	// now in device coordinates
	if( Align == ZA_DEFAULT )
		Align = m_align;
	switch ( Align )
	{
		case ZA_BOTTOMLEFT:
			pt.y -= rectClient.Height();
			break;
		case ZA_CENTER:
			pt.y -= rectClient.Height()/2;
			pt.x -= rectClient.Width()/2;
			break;
	}
	
	//limit if out of range
	CSize sizeClient=rectClient.Size();
	CSize needSb;
	CSize sizeRange;
	CPoint ptMove;
	GetScrollBarState(sizeClient,needSb,sizeRange,ptMove,FALSE);
	if (pt.x > sizeRange.cx)
		pt.x = sizeRange.cx;
	if (pt.x < 0)
		pt.x = 0;
	if (pt.y > sizeRange.cy)
		pt.y = sizeRange.cy;
	if (pt.y < 0)
		pt.y = 0;

	ScrollToDevicePosition(pt);
}

CPoint COXScrollWnd::GetDeviceScrollPosition() const
{
	CPoint pt(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
	ASSERT(pt.x >= 0 && pt.y >= 0);


	return pt;
}

void COXScrollWnd::GetDeviceScrollSizes(int& nMapMode, SIZE& sizeTotal,
										SIZE& sizePage, SIZE& sizeLine) const
{
	if (m_nMapMode <= 0)
		TRACE(_T("Warning: COXScrollWnd::GetDeviceScrollSizes returning invalid mapping mode\n"));
	nMapMode = m_nMapMode;
	sizeTotal = m_totalDev;
	sizePage = m_pageDev;
	sizeLine = m_lineDev;
}

void COXScrollWnd::ScrollToDevicePosition(POINT ptDev)
{
	ASSERT(ptDev.x >= 0);
	ASSERT(ptDev.y >= 0);
	int dx=0;
	int dy=0;

	// Note: ScrollToDevicePosition can and is used to scroll out-of-range
	//  areas as far as COXZoomView is concerned -- specifically in
	//  the print-preview code.  Since OnScrollBy makes sure the range is
	//  valid, ScrollToDevicePosition does not vector through OnScrollBy.

	CRect rect;
	GetClientRect(rect);
	CSize sizeClient=rect.Size();
	CSize needSb;
	CSize sizeRange;
	CPoint ptMove;
	GetScrollBarState(sizeClient,needSb,sizeRange,ptMove,FALSE);

	int xOrig=GetScrollPos(SB_HORZ);
	int yOrig=GetScrollPos(SB_VERT);

	if(needSb.cx || xOrig!=0)
	{
		SetScrollPos(SB_HORZ,ptDev.x);
	}
	dx=xOrig-ptDev.x;

	if(needSb.cy || yOrig!=0)
	{
		SetScrollPos(SB_VERT,ptDev.y);
	}
	dy=yOrig-ptDev.y;

	ScrollWindow( dx, dy );
}

/////////////////////////////////////////////////////////////////////////////
// Other helpers
// retained for compatibility
void COXScrollWnd::FillOutsideRect(CDC* pDC, CBrush* pBrush) const
{
	ASSERT_VALID(pDC);
	ASSERT_VALID(pBrush);

	// Fill Rect outside the contents
	//
	CPoint ptOrigin=GetOrigin(pDC);

	CRect rectClient;
	GetClientRect(rectClient);
	CRect rect=rectClient;

	rect.left=m_totalDev.cx+ptOrigin.x;
	if(!rect.IsRectEmpty())
	{
		pDC->FillRect(rect, pBrush);    // vertical strip along the side
	}

	rect.left=0;
	rect.right=m_totalDev.cx+ptOrigin.x;
	rect.top=m_totalDev.cy+ptOrigin.y;
	if(!rect.IsRectEmpty())
	{
		pDC->FillRect(rect,pBrush);    // horizontal strip along the bottom
	}

	if(ptOrigin.y>0)
	{
		rect.top=0;
		rect.bottom=ptOrigin.y;
		rect.right=rectClient.right-ptOrigin.x;
		if(!rect.IsRectEmpty())
		{
			pDC->FillRect(rect, pBrush);    // horizontal strip along the top
		}
	}

	if(ptOrigin.x>0)
	{
		rect.left=0;
		rect.right=ptOrigin.x;
		rect.top=ptOrigin.y;
		rect.bottom=rectClient.bottom-ptOrigin.y;
		if(!rect.IsRectEmpty())
		{
			pDC->FillRect(rect, pBrush);    // horizontal strip along the top
		}
	}
	//
	//////////////////////////
}


/////////////////////////////////////////////////////////////////////////////
// Tie to scrollbars and CWnd behaviour

void COXScrollWnd::GetScrollBarSizes(CSize& sizeSb)
{
	sizeSb.cx=sizeSb.cy=0;
	DWORD dwStyle=GetStyle();

	if (GetScrollBarCtrl(SB_VERT) == NULL)
	{
		// vert scrollbars will impact client area of this window
		sizeSb.cx=GetSystemMetrics(SM_CXVSCROLL);
		if (dwStyle & WS_BORDER)
			sizeSb.cx-=GetSystemMetrics(SM_CXBORDER);
	}
	if (GetScrollBarCtrl(SB_HORZ)==NULL)
	{
		// horz scrollbars will impact client area of this window
		sizeSb.cy=GetSystemMetrics(SM_CYHSCROLL);
		if(dwStyle & WS_BORDER)
			sizeSb.cy-=GetSystemMetrics(SM_CYBORDER);
	}
}

BOOL COXScrollWnd::GetTrueClientSize(CSize& size, CSize& sizeSb)
	// return TRUE if enough room to add scrollbars if needed
{
	CRect rect;
	GetClientRect(&rect);
	ASSERT(rect.top == 0 && rect.left == 0);
	size.cx = rect.right;
	size.cy = rect.bottom;
	DWORD dwStyle = GetStyle();

	// first get the size of the scrollbars for this window
	GetScrollBarSizes(sizeSb);

	// first calculate the size of a potential scrollbar
	// (scroll bar controls do not get turned on/off)
	if (sizeSb.cx != 0 && (dwStyle & WS_VSCROLL))
	{
		// vert scrollbars will impact client area of this window
		size.cx += sizeSb.cx;   // currently on - adjust now
	}
	if (sizeSb.cy != 0 && (dwStyle & WS_HSCROLL))
	{
		// horz scrollbars will impact client area of this window
		size.cy += sizeSb.cy;   // currently on - adjust now
	}

	// return TRUE if enough room
	return (size.cx > sizeSb.cx && size.cy > sizeSb.cy);
}

// helper to return the state of the scrollbars without actually changing
//  the state of the scrollbars
void COXScrollWnd::GetScrollBarState(CSize sizeClient, CSize& needSb,
									 CSize& sizeRange, CPoint& ptMove, 
									 BOOL bInsideClient)
{
	// get scroll bar sizes (the part that is in the client area)
	CSize sizeSb;
	GetScrollBarSizes(sizeSb);

	// enough room to add scrollbars
	sizeRange = m_totalDev - sizeClient;
		// > 0 => need to scroll
	ptMove = GetDeviceScrollPosition();
		// point to move to (start at current scroll pos)

	BOOL bNeedH = sizeRange.cx > 0;
	if (!bNeedH)
		ptMove.x = 0;                       // jump back to origin
	else if (bInsideClient)
		sizeRange.cy += sizeSb.cy;          // need room for a scroll bar

	BOOL bNeedV = sizeRange.cy > 0;
	if (!bNeedV)
		ptMove.y = 0;                       // jump back to origin 
		//ptMove.y =  m_totalDev.cy - sizeClient.cy;
	else if (bInsideClient)
		sizeRange.cx += sizeSb.cx;          // need room for a scroll bar

	if (bNeedV && !bNeedH && sizeRange.cx > 0)
	{
		ASSERT(bInsideClient);
		// need a horizontal scrollbar after all
		bNeedH = TRUE;
		sizeRange.cy += sizeSb.cy;
	}

	// if current scroll position will be past the limit, scroll to limit
	if (sizeRange.cx > 0 && ptMove.x >= sizeRange.cx)
		ptMove.x = sizeRange.cx;
	if (sizeRange.cy > 0 && ptMove.y >= sizeRange.cy)
		ptMove.y = sizeRange.cy;

	// now update the bars as appropriate
	needSb.cx = bNeedH;
	needSb.cy = bNeedV;

	// needSb, sizeRange, and ptMove area now all updated
}

void COXScrollWnd::UpdateBars(BOOL /*bSendRecalc*/)
{
	// UpdateBars may cause window to be resized - ignore those resizings
	if (m_bInsideUpdate)
		return;         // Do not allow recursive calls

	// Lock out recursion
	m_bInsideUpdate = TRUE;

	// update the horizontal to reflect reality
	// NOTE: turning on/off the scrollbars will cause 'OnSize' callbacks
	ASSERT(m_totalDev.cx >= 0 && m_totalDev.cy >= 0);

	CRect rectClient;
	BOOL bCalcClient = TRUE;

	// allow parent to do inside-out layout first
	CWnd* pParentWnd = GetParent();
	if (pParentWnd != NULL)
	{
		// if parent window responds to this message, use just
		//  client area for scroll bar calc -- not "true" client area
		if ((BOOL)pParentWnd->SendMessage(WM_RECALCPARENT, 0,
			(LPARAM)(LPCRECT)&rectClient) != 0)
		{
			// use rectClient instead of GetTrueClientSize for
			//  client size calculation.
			bCalcClient = FALSE;
		}
	}

	CSize sizeClient;
	CSize sizeSb;

	if (bCalcClient)
	{
		// get client rect
		if (!GetTrueClientSize(sizeClient, sizeSb))
		{
			// no room for scroll bars (common for zero sized elements)
			CRect rect;
			GetClientRect(&rect);
			if (rect.right > 0 && rect.bottom > 0)
			{
				// if entire client area is not invisible, assume we have
				//  control over our scrollbars
				EnableScrollBarCtrl(SB_BOTH, FALSE);
			}
			m_bInsideUpdate = FALSE;
			return;
		}
	}
	else
	{
		// let parent window determine the "client" rect
		sizeClient.cx = rectClient.right - rectClient.left;
		sizeClient.cy = rectClient.bottom - rectClient.top;
	}

	// enough room to add scrollbars
	CSize sizeRange;
	CPoint ptMove;
	CSize needSb;
    
    // if scrolling-sizes are relative to client area update them NOW
    if( m_bUseRelativeScrollSizes )
    {
    	m_pageDev.cx = MulDiv( m_nPagePercent, sizeClient.cx, 100);
    	m_pageDev.cy = MulDiv( m_nPagePercent, sizeClient.cy, 100);
    	m_lineDev.cx = MulDiv( m_nLinePercent, sizeClient.cx, 100);
    	m_lineDev.cy = MulDiv( m_nLinePercent, sizeClient.cy, 100);
    }
    
	// get the current scroll bar state given the true client area
	GetScrollBarState(sizeClient, needSb, sizeRange, ptMove, bCalcClient);
	if (needSb.cx)
		sizeClient.cy -= sizeSb.cy;
	if (needSb.cy)
		sizeClient.cx -= sizeSb.cx;

	// first scroll the window as needed
	ScrollToDevicePosition(ptMove); // will set the scroll bar positions too

	// this structure needed to update the scrollbar page range
	SCROLLINFO info;
	info.fMask = SIF_PAGE|SIF_RANGE;
	info.nMin = 0;

	// now update the bars as appropriate
	EnableScrollBarCtrl(SB_HORZ, needSb.cx);
	if (needSb.cx)
	{
		info.nPage = sizeClient.cx;
		info.nMax = m_totalDev.cx-1;
		if(!SetScrollInfo(SB_HORZ, &info, TRUE))
			SetScrollRange(SB_HORZ, 0, sizeRange.cx, TRUE);
	}
	EnableScrollBarCtrl(SB_VERT, needSb.cy);
	if (needSb.cy)
	{
		info.nPage = sizeClient.cy;
		info.nMax = m_totalDev.cy-1;
		if(!SetScrollInfo(SB_VERT, &info, TRUE))
			SetScrollRange(SB_VERT, 0, sizeRange.cy, TRUE);
	}

	// Remove recursion lockout
	m_bInsideUpdate = FALSE;
}

void COXScrollWnd::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	if (nAdjustType == adjustOutside)
	{
		// if the control is being used in-place, add scrollbar sizes
		//  (scollbars should appear on the outside when in-place editing)
		CSize sizeClient(
			lpClientRect->right - lpClientRect->left,
			lpClientRect->bottom - lpClientRect->top);

		CSize sizeRange = m_totalDev - sizeClient;
			// > 0 => need to scroll

		// get scroll bar sizes (used to adjust the window)
		CSize sizeSb;
		GetScrollBarSizes(sizeSb);

		// adjust the window size based on the state
		if (sizeRange.cy > 0)
		{   // vertical scroll bars take up horizontal space
			lpClientRect->right += sizeSb.cx;
		}
		if (sizeRange.cx > 0)
		{   // horizontal scroll bars take up vertical space
			lpClientRect->bottom += sizeSb.cy;
		}
	}
	else
	{
		// call default to handle other non-client areas
		::AdjustWindowRect(lpClientRect, GetStyle(), FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// COXScrollWnd scrolling

BOOL COXScrollWnd::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
//	if(LOBYTE(nScrollCode)!=SB_THUMBTRACK && HIBYTE(nScrollCode)!=SB_THUMBTRACK &&
//		IsSmoothScrolling() && !m_bIsInSmoothScrolling)
	if(IsSmoothScrolling() && !m_bIsInSmoothScrolling && 
		(LOBYTE(nScrollCode)==SB_PAGEDOWN || LOBYTE(nScrollCode)==SB_PAGEUP ||
		HIBYTE(nScrollCode)==SB_PAGEDOWN || HIBYTE(nScrollCode)==SB_PAGEUP))
	{
		return OnSmoothScroll(nScrollCode,nPos, bDoScroll);
	}

	// calc new x position
	int x = GetScrollPos(SB_HORZ);
	int xOrig = x;

	switch (LOBYTE(nScrollCode))
	{
	case SB_TOP:
		x = 0;
		break;
	case SB_BOTTOM:
		x = INT_MAX;
		break;
	case SB_LINEUP:
		x -= m_lineDev.cx;
		break;
	case SB_LINEDOWN:
		x += m_lineDev.cx;
		break;
	case SB_PAGEUP:
		x -= m_pageDev.cx;
		break;
	case SB_PAGEDOWN:
		x += m_pageDev.cx;
		break;
	case SB_THUMBTRACK:
		x = nPos;
		break;
	}

	// calc new y position
	int y = GetScrollPos(SB_VERT);
	int yOrig = y;

	switch (HIBYTE(nScrollCode))
	{
	case SB_TOP:
		y = 0;
		break;
	case SB_BOTTOM:
		y = INT_MAX;
		break;
	case SB_LINEUP:
		y -= m_lineDev.cy;
		break;
	case SB_LINEDOWN:
		y += m_lineDev.cy;
		break;
	case SB_PAGEUP:
		y -= m_pageDev.cy;
		break;
	case SB_PAGEDOWN:
		y += m_pageDev.cy;
		break;
	case SB_THUMBTRACK:
		y = nPos;
		break;
	}

	BOOL bResult = OnScrollBy(CSize(x - xOrig, y - yOrig), bDoScroll);
	if (bResult && bDoScroll)
		UpdateWindow();

	return bResult;
}

BOOL COXScrollWnd::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	int xOrig, x;
	int yOrig, y;

	// don't scroll if there is no valid scroll range (ie. no scroll bar)
	CScrollBar* pBar;
	DWORD dwStyle = GetStyle();
	pBar = GetScrollBarCtrl(SB_VERT);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_VSCROLL)))
	{
		// vertical scroll bar not enabled
		sizeScroll.cy = 0;
	}
	pBar = GetScrollBarCtrl(SB_HORZ);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_HSCROLL)))
	{
		// horizontal scroll bar not enabled
		sizeScroll.cx = 0;
	}

	// adjust current x position
	xOrig = x = GetScrollPos(SB_HORZ);
	int xMax = GetScrollLimit(SB_HORZ);
	x += sizeScroll.cx;
	if (x < 0)
		x = 0;
	else if (x > xMax)
		x = xMax;

	// adjust current y position
	yOrig = y = GetScrollPos(SB_VERT);
	int yMax = GetScrollLimit(SB_VERT);
	y += sizeScroll.cy;
	if (y < 0)
		y = 0;
	else if (y > yMax)
		y = yMax;

	// did anything change?
	if (x == xOrig && y == yOrig)
		return FALSE;

	if (bDoScroll)
	{
		// do scroll and update scroll positions
		ScrollWindow(-(x-xOrig), -(y-yOrig));
		if (x != xOrig)
		{
			SetScrollPos(SB_HORZ, x);
		}
		if (y != yOrig)
		{
			SetScrollPos(SB_VERT, y);
		}
	}
	return TRUE;
}

BOOL COXScrollWnd::OnSmoothScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	// TODO: Add your specialized code here and/or call the base class

	ASSERT(IsSmoothScrolling());

	if(m_bIsInSmoothScrolling)
		return TRUE;

	m_bIsInSmoothScrolling=TRUE;
	//
	// First handle left/right scroll messages. If scrolling by page,
	// scroll m_nPageSlices times rather than 1. If scrolling by line,
	// scroll m_nLineSlices times.
	//
	BYTE nCode=LOBYTE(nScrollCode);

	if((nCode==SB_PAGELEFT) ||(nCode==SB_PAGERIGHT) ||
		(nCode==SB_LINELEFT) ||(nCode==SB_LINERIGHT)) 
	{
		int nCount=0;
		int nInc=0;
		int nFinalInc=0;
		int nLineCode=0;

		switch(nCode) 
		{
		case SB_PAGELEFT:
			{
				nLineCode=SB_LINELEFT;
				nInc=m_pageDev.cx/m_nPageSlices;
				nFinalInc=m_pageDev.cx%m_nPageSlices;
				nCount=m_nPageSlices;
				break;
			}
		case SB_PAGERIGHT:
			{
				nLineCode=SB_LINERIGHT;
				nInc=m_pageDev.cx/m_nPageSlices;
				nFinalInc=m_pageDev.cx%m_nPageSlices;
				nCount=m_nPageSlices;
				break;
			}

		case SB_LINELEFT:
			{
				nLineCode=SB_LINELEFT;
				nInc=m_lineDev.cx/m_nLineSlices;
				nFinalInc=m_lineDev.cx%m_nLineSlices;
				nCount=m_nLineSlices;
				break;
			}
		case SB_LINERIGHT:
			{
				nLineCode=SB_LINERIGHT;
				nInc=m_lineDev.cx/m_nLineSlices;
				nFinalInc=m_lineDev.cx%m_nLineSlices;
				nCount=m_nLineSlices;
				break;
			}
		}

		int nOldLineSize=m_lineDev.cx;
		BOOL bResult=FALSE;
		DWORD dwTime=0;

		while(nInc>0 && nCount--) 
		{
			DWORD dwCurrentTime=::GetCurrentTime();
			DWORD dwElapsedTime=dwCurrentTime-dwTime;
			if(dwElapsedTime<m_dwWaitingTime)
			{
				::Sleep(m_dwWaitingTime-dwElapsedTime);
			}
			dwTime=dwCurrentTime;

			m_lineDev.cx=nInc;
			BOOL bScrolled=
				COXScrollWnd::OnScroll(MAKEWORD(nLineCode,-1),nPos,bDoScroll);
			m_lineDev.cx=nOldLineSize;

			if(!bScrolled)
			{
				m_bIsInSmoothScrolling=FALSE;
				return bResult;
			}

			bResult=TRUE;
		}

		if(nFinalInc) 
		{
			m_lineDev.cx=nFinalInc;
			if(!COXScrollWnd::OnScroll(MAKEWORD(nLineCode,-1),nPos))
				bResult=TRUE;
		}

		m_lineDev.cx=nOldLineSize;

		m_bIsInSmoothScrolling=FALSE;
		return bResult;
	}

	//
	// Next handle up/down scroll messages. If scrolling by page,
	// scroll m_nPageSlices times rather than 1. If scrolling by line,
	// scroll m_nLineSlices times.
	//
	nCode=HIBYTE(nScrollCode);

	if((nCode==SB_PAGEUP) ||(nCode==SB_PAGEDOWN) ||
		(nCode==SB_LINEUP) ||(nCode==SB_LINEDOWN)) 
	{
		int nCount=0;
		int nInc=0;
		int nFinalInc=0;
		int nLineCode=0;

		switch(nCode) 
		{
		case SB_PAGEUP:
			{
				nLineCode=SB_LINEUP;
				nInc=m_pageDev.cy/m_nPageSlices;
				nFinalInc=m_pageDev.cy%m_nPageSlices;
				nCount=m_nPageSlices;
				break;
			}
		case SB_PAGEDOWN:
			{
				nLineCode=SB_LINEDOWN;
				nInc=m_pageDev.cy/m_nPageSlices;
				nFinalInc=m_pageDev.cy%m_nPageSlices;
				nCount=m_nPageSlices;
				break;
			}

		case SB_LINEUP:
			{
				nLineCode=SB_LINEUP;
				nInc=m_lineDev.cy/m_nLineSlices;
				nFinalInc=m_lineDev.cy%m_nLineSlices;
				nCount=m_nLineSlices;
				break;
			}
		case SB_LINEDOWN:
			{
				nLineCode=SB_LINEDOWN;
				nInc=m_lineDev.cy/m_nLineSlices;
				nFinalInc=m_lineDev.cy%m_nLineSlices;
				nCount=m_nLineSlices;
				break;
			}
		}

		int nOldLineSize=m_lineDev.cy;
		BOOL bResult=FALSE;
		DWORD dwTime=0;

		while(nInc>0 && nCount--) 
		{
			DWORD dwCurrentTime=::GetCurrentTime();
			DWORD dwElapsedTime=dwCurrentTime-dwTime;
			if(dwElapsedTime<m_dwWaitingTime)
			{
				::Sleep(m_dwWaitingTime-dwElapsedTime);
			}
			dwTime=dwCurrentTime;

			m_lineDev.cy=nInc;
			BOOL bScrolled=COXScrollWnd::OnScroll(MAKEWORD(-1,nLineCode),nPos,bDoScroll);
			m_lineDev.cy=nOldLineSize;

			if(!bScrolled)
			{
				m_bIsInSmoothScrolling=FALSE;
				return bResult;
			}

			bResult=TRUE;
		}

		if(nFinalInc) 
		{
			m_lineDev.cy=nFinalInc;
			if(!COXScrollWnd::OnScroll(MAKEWORD(-1,nLineCode),nPos,bDoScroll))
				bResult=TRUE;
		}

		m_lineDev.cy=nOldLineSize;

		m_bIsInSmoothScrolling=FALSE;
		return bResult;
	}

	m_bIsInSmoothScrolling=FALSE;
	return TRUE;
}


BOOL COXScrollWnd::OnMouseWheel(UINT fFlags, short zDelta, CPoint point)
{
	// we don't handle anything but scrolling just now
	if (fFlags & (MK_SHIFT | MK_CONTROL))
		return FALSE;

	// we can't get out of it--perform the scroll ourselves
	return DoMouseWheel(fFlags, zDelta, point);
}

// This function isn't virtual. If you need to override it,
// you really need to override OnMouseWheel() here or in
// CSplitterWnd.
BOOL COXScrollWnd::DoMouseWheel(UINT fFlags, short zDelta, CPoint point)
{
	UNUSED_ALWAYS(point);
	UNUSED_ALWAYS(fFlags);

	// if we have a vertical scroll bar, the wheel scrolls that
	// if we have _only_ a horizontal scroll bar, the wheel scrolls that
	// otherwise, don't do any work at all

	DWORD dwStyle = GetStyle();
	CScrollBar* pBar = GetScrollBarCtrl(SB_VERT);
	BOOL bHasVertBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
					(dwStyle & WS_VSCROLL);

	pBar = GetScrollBarCtrl(SB_HORZ);
	BOOL bHasHorzBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
					(dwStyle & WS_HSCROLL);

	if (!bHasVertBar && !bHasHorzBar)
		return FALSE;

	BOOL bResult = FALSE;
	UINT uWheelScrollLines=GetMouseScrollLines();
	int nToScroll;
	int nDisplacement;

	if (bHasVertBar)
	{
		nToScroll = ::MulDiv(-zDelta, uWheelScrollLines, WHEEL_DELTA);
		if (nToScroll == -1 || uWheelScrollLines == WHEEL_PAGESCROLL)
		{
			nDisplacement = m_pageDev.cy;
			if (zDelta > 0)
				nDisplacement = -nDisplacement;
		}
		else
		{
			nDisplacement = nToScroll * m_lineDev.cy;
			nDisplacement = min(nDisplacement, m_pageDev.cy);
		}
		bResult = OnScrollBy(CSize(0, nDisplacement), TRUE);
	}
	else if (bHasHorzBar)
	{
		nToScroll = ::MulDiv(-zDelta, uWheelScrollLines, WHEEL_DELTA);
		if (nToScroll == -1 || uWheelScrollLines == WHEEL_PAGESCROLL)
		{
			nDisplacement = m_pageDev.cx;
			if (zDelta > 0)
				nDisplacement = -nDisplacement;
		}
		else
		{
			nDisplacement = nToScroll * m_lineDev.cx;
			nDisplacement = min(nDisplacement, m_pageDev.cx);
		}
		bResult = OnScrollBy(CSize(nDisplacement, 0), TRUE);
	}

	if (bResult)
		UpdateWindow();

	return bResult;
}


static BOOL g_bGotScrollLines = FALSE;

UINT COXScrollWnd::GetMouseScrollLines()
{
	static UINT uCachedScrollLines;

	// if we've already got it and we're not refreshing,
	// return what we've already got

	if (g_bGotScrollLines)
		return uCachedScrollLines;

	// see if we can find the mouse window

	g_bGotScrollLines = TRUE;

	static UINT msgGetScrollLines;
	static WORD nRegisteredMessage;

	if (nRegisteredMessage == 0)
	{
		msgGetScrollLines = ::RegisterWindowMessage(MSH_SCROLL_LINES);
		if (msgGetScrollLines == 0)
			nRegisteredMessage = 1;     // couldn't register!  never try again
		else
			nRegisteredMessage = 2;     // it worked: use it
	}

	if (nRegisteredMessage == 2)
	{
		HWND hwMouseWheel = NULL;
		hwMouseWheel = ::FindWindow(MSH_WHEELMODULE_CLASS, MSH_WHEELMODULE_TITLE);
		if (hwMouseWheel && msgGetScrollLines)
		{
			uCachedScrollLines = (UINT)
				::SendMessage(hwMouseWheel, msgGetScrollLines, 0, 0);
			return uCachedScrollLines;
		}
	}

	// couldn't use the window -- try system settings
	uCachedScrollLines = 3; // reasonable default
	
	DWORD dwWinVersion = ::GetVersion();
	if (dwWinVersion < 0x80000000)              // Windows NT/2000/XP
	{
		HKEY hKey;
		if (RegOpenKeyEx(HKEY_CURRENT_USER,  _T("Control Panel\\Desktop"),
				0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
		{
			TCHAR szData[128];
			DWORD dwKeyDataType;
			DWORD dwDataBufSize = _countof(szData);

			if (RegQueryValueEx(hKey, _T("WheelScrollLines"), NULL, &dwKeyDataType,
					(LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
			{
				uCachedScrollLines = _tcstoul(szData, NULL, 10);
			}
			RegCloseKey(hKey);
		}
	}
#if _MFC_VER>0x0421
	else if (!afxData.bWin95)
#else
	else if (!afxData.bWin32s)
#endif
	{
		::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &uCachedScrollLines, 0);
	}

	return uCachedScrollLines;
}


void COXScrollWnd::NormalToScaled(CRect* pRect)
{
	int nZoomLevel=GetZoomLevel();

	// scale coordinates from normal to scaled 
	// taking into account current zoom level
	if(nZoomLevel!=100)
	{
		pRect->left=((long)((long)pRect->left*(long)nZoomLevel))/(100L);
		pRect->top=((long)((long)pRect->top*(long)nZoomLevel))/(100L);
		pRect->right=((long)((long)pRect->right*(long)nZoomLevel))/(100L);
		pRect->bottom=((long)((long)pRect->bottom*(long)nZoomLevel))/(100L);
	}
}

void COXScrollWnd::NormalToScaled(CPoint* pPoint)
{
	int nZoomLevel=GetZoomLevel();

	// scale coordinates from normal to scaled 
	// taking into account current zoom level
	if(nZoomLevel!=100)
	{
		pPoint->x=((long)((long)pPoint->x*(long)nZoomLevel))/(100L);
		pPoint->y=((long)((long)pPoint->y*(long)nZoomLevel))/(100L);
	}
}

void COXScrollWnd::ScaledToNormal(CRect* pRect)
{
	int nZoomLevel=GetZoomLevel();

	// scale coordinates from scaled to normal
	// taking into account current zoom level
	if(nZoomLevel!=100)
	{
		pRect->left=((long)((long)pRect->left*100L))/((long)nZoomLevel);
		pRect->top=((long)((long)pRect->top*100L))/((long)nZoomLevel);
		pRect->right=((long)((long)pRect->right*100L))/((long)nZoomLevel);
		pRect->bottom=((long)((long)pRect->bottom*100L))/((long)nZoomLevel);
	}
}

void COXScrollWnd::ScaledToNormal(CPoint* pPoint)
{
	int nZoomLevel=GetZoomLevel();

	// scale coordinates from scaled to normal
	// taking into account current zoom level
	if(nZoomLevel!=100)
	{
		pPoint->x=((long)((long)pPoint->x*100L))/((long)nZoomLevel);
		pPoint->y=((long)((long)pPoint->y*100L))/((long)nZoomLevel);
	}
}

void COXScrollWnd::NormalToPrinted(CDC* pDC, CRect* pRect)
{
	// scale coordinates from display to printer
	// taking into account screen and current printer DPI
	int xPrinterDPI=pDC->GetDeviceCaps(LOGPIXELSX);
	int yPrinterDPI=pDC->GetDeviceCaps(LOGPIXELSY);

	CClientDC dc(this);
	int xScreenDPI=dc.GetDeviceCaps(LOGPIXELSX);
	int yScreenDPI=dc.GetDeviceCaps(LOGPIXELSY);

	if(xPrinterDPI!=xScreenDPI || yPrinterDPI!=yScreenDPI)
	{
		pRect->left=((long)((long)pRect->left*(long)xPrinterDPI))/((long)xScreenDPI);
		pRect->top=((long)((long)pRect->top*(long)yPrinterDPI))/((long)yScreenDPI);
		pRect->right=((long)((long)pRect->right*(long)xPrinterDPI))/((long)xScreenDPI);
		pRect->bottom=((long)((long)pRect->bottom*(long)yPrinterDPI))/((long)yScreenDPI);
	}
}

void COXScrollWnd::NormalToPrinted(CDC* pDC, CPoint* pPoint)
{
	// scale coordinates from display to printer
	// taking into account screen and current printer DPI
	int xPrinterDPI=pDC->GetDeviceCaps(LOGPIXELSX);
	int yPrinterDPI=pDC->GetDeviceCaps(LOGPIXELSY);

	CClientDC dc(this);
	int xScreenDPI=dc.GetDeviceCaps(LOGPIXELSX);
	int yScreenDPI=dc.GetDeviceCaps(LOGPIXELSY);

	if(xPrinterDPI!=xScreenDPI || yPrinterDPI!=yScreenDPI)
	{
		pPoint->x=((long)((long)pPoint->x*(long)xPrinterDPI))/
			((long)xScreenDPI);
		pPoint->y=((long)((long)pPoint->y*(long)yPrinterDPI))/
			((long)yScreenDPI);
	}
}

void COXScrollWnd::PrintedToNormal(CDC* pDC, CRect* pRect)
{
	// scale coordinates from printer to display
	// taking into account screen and current printer DPI
	int xPrinterDPI=pDC->GetDeviceCaps(LOGPIXELSX);
	int yPrinterDPI=pDC->GetDeviceCaps(LOGPIXELSY);

	CClientDC dc(this);
	int xScreenDPI=dc.GetDeviceCaps(LOGPIXELSX);
	int yScreenDPI=dc.GetDeviceCaps(LOGPIXELSY);

	if(xPrinterDPI!=xScreenDPI || yPrinterDPI!=yScreenDPI)
	{
		pRect->left=((long)((long)pRect->left*(long)xScreenDPI))/
			((long)xPrinterDPI);
		pRect->top=((long)((long)pRect->top*(long)yScreenDPI))/
			((long)yPrinterDPI);
		pRect->right=((long)((long)pRect->right*(long)xScreenDPI))/
			((long)xPrinterDPI);
		pRect->bottom=((long)((long)pRect->bottom*(long)yScreenDPI))/
			((long)yPrinterDPI);
	}
}

void COXScrollWnd::PrintedToNormal(CDC* pDC, CPoint* pPoint)
{
	// scale coordinates from printer to display
	// taking into account screen and current printer DPI
	int xPrinterDPI=pDC->GetDeviceCaps(LOGPIXELSX);
	int yPrinterDPI=pDC->GetDeviceCaps(LOGPIXELSY);

	CClientDC dc(this);
	int xScreenDPI=dc.GetDeviceCaps(LOGPIXELSX);
	int yScreenDPI=dc.GetDeviceCaps(LOGPIXELSY);

	if(xPrinterDPI!=xScreenDPI || yPrinterDPI!=yScreenDPI)
	{
		pPoint->x=((long)((long)pPoint->x*(long)xScreenDPI))/
			((long)xPrinterDPI);
		pPoint->y=((long)((long)pPoint->y*(long)yScreenDPI))/
			((long)yPrinterDPI);
	}
}


BOOL COXScrollWnd::SizeToContent()
{
	CRect rectWindow;
	GetWindowRect(rectWindow);
	if(GetParent()!=NULL)
		GetParent()->ScreenToClient(rectWindow);

	CRect rectClient;
	GetClientRect(rectClient);
	
	CSize szTotal=GetTotalSize();
	CRect rect(0,0,szTotal.cx,szTotal.cy);
	NormalToScaled(&rect);
	szTotal=rect.Size();
	rectWindow.right=rectWindow.right+szTotal.cx-rectClient.Width();
	rectWindow.bottom=rectWindow.bottom+szTotal.cy-rectClient.Height();
	
	MoveWindow(rectWindow);

	return TRUE;
}


void COXScrollWnd::SetContentsAlign(ContentsAlignment align)
{
	m_alignContents=align;
	RedrawWindow();
}


/////////////////////////////////////////////////////////////////////////////
// COXScrollWnd diagnostics

#ifdef _DEBUG
void COXScrollWnd::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CWnd::Dump(dc);

	AFX_DUMP1(dc, "\nm_totalLog = ", m_totalLog);
	AFX_DUMP1(dc, "\nm_totalDev = ", m_totalDev);
	AFX_DUMP1(dc, "\nm_pageDev = ", m_pageDev);
	AFX_DUMP1(dc, "\nm_lineDev = ", m_lineDev);
	AFX_DUMP1(dc, "\nm_bInsideUpdate = ", m_bInsideUpdate);
	AFX_DUMP0(dc, "\nm_nMapMode = ");
	switch (m_nMapMode)
	{
	case MM_NONE:
		AFX_DUMP0(dc, "MM_NONE");
		break;
	case MM_TEXT:
		AFX_DUMP0(dc, "MM_TEXT");
		break;
	case MM_LOMETRIC:
		AFX_DUMP0(dc, "MM_LOMETRIC");
		break;
	case MM_HIMETRIC:
		AFX_DUMP0(dc, "MM_HIMETRIC");
		break;
	case MM_LOENGLISH:
		AFX_DUMP0(dc, "MM_LOENGLISH");
		break;
	case MM_HIENGLISH:
		AFX_DUMP0(dc, "MM_HIENGLISH");
		break;
	case MM_TWIPS:
		AFX_DUMP0(dc, "MM_TWIPS");
		break;
	case MM_ANISOTROPIC:
		AFX_DUMP0(dc, "MM_ANISOTROPIC");
		AFX_DUMP1(dc, "\nm_sizeDev = ", m_sizeDev);
		break;
	default:
		AFX_DUMP0(dc, "*unknown*");
		break;
	}
	AFX_DUMP1(dc, "\nm_nZoomLevel = ", m_nZoomLevel);
	AFX_DUMP1(dc, "\nm_bUseRelativeScrollSizes = ", m_bUseRelativeScrollSizes);
	if( m_bUseRelativeScrollSizes )
	{
		AFX_DUMP1(dc, "\nm_nPagePercent = ", m_nPagePercent);
		AFX_DUMP1(dc, "\nm_nLinePercent = ", m_nLinePercent);
	}
	AFX_DUMP0(dc, "\nm_align = ");
	switch (m_align)
	{
	case ZA_TOPLEFT:
		AFX_DUMP0(dc, "TOPLEFT");
		break;
	case ZA_BOTTOMLEFT:
		AFX_DUMP0(dc, "BOTTOMLEFT");
		break;
	case ZA_CENTER:
		AFX_DUMP0(dc, "CENTER");
		break;
	default:
		AFX_DUMP0(dc, "*illegal*");
		break;
    }
}

void COXScrollWnd::AssertValid() const
{
	CWnd::AssertValid();

	switch (m_nMapMode)
	{
	case MM_NONE:
	case MM_TEXT:
	case MM_LOMETRIC:
	case MM_HIMETRIC:
	case MM_LOENGLISH:
	case MM_HIENGLISH:
	case MM_TWIPS:
	case MM_ANISOTROPIC:
		break;
	case MM_ISOTROPIC:
		ASSERT(FALSE); // illegal mapping mode
	default:
		ASSERT(FALSE); // unknown mapping mode
	}
}
#endif //_DEBUG





