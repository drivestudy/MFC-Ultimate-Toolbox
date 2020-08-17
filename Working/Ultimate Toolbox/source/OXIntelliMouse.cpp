// ==========================================================================
// 							   Class Implementation : 
//								COXIntelliMouse
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
#include "OXIntelliMouse.h"
#include "UTB64Bit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(COXIntelliMouse, COXHookWnd);

// Constructor
COXIntelliMouse::COXIntelliMouse()
{
	// initialize variables
	Reset();

	m_nStartEventID=0;
	m_dwKeyState=0;

	// load splash window bitmap
	//
	m_splashWnd.LoadBitmap(IDB_OX_INTELLITARGET);

	// create cursors
	//
	HCURSOR hCursor;
	hCursor = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_OX_SCRL_LEFT));
	ASSERT(hCursor!=NULL);
	m_arrScrollCursor.SetAt(SCRL_LEFT,hCursor);

	hCursor = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_OX_SCRL_TOPLEFT));
	ASSERT(hCursor!=NULL);
	m_arrScrollCursor.SetAt(SCRL_TOPLEFT,hCursor);

	hCursor = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_OX_SCRL_TOP));
	ASSERT(hCursor!=NULL);
	m_arrScrollCursor.SetAt(SCRL_TOP,hCursor);

	hCursor = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_OX_SCRL_TOPRIGHT));
	ASSERT(hCursor!=NULL);
	m_arrScrollCursor.SetAt(SCRL_TOPRIGHT,hCursor);

	hCursor = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_OX_SCRL_RIGHT));
	ASSERT(hCursor!=NULL);
	m_arrScrollCursor.SetAt(SCRL_RIGHT,hCursor);

	hCursor = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_OX_SCRL_BOTTOMRIGHT));
	ASSERT(hCursor!=NULL);
	m_arrScrollCursor.SetAt(SCRL_BOTTOMRIGHT,hCursor);

	hCursor = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_OX_SCRL_BOTTOM));
	ASSERT(hCursor!=NULL);
	m_arrScrollCursor.SetAt(SCRL_BOTTOM,hCursor);

	hCursor = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_OX_SCRL_BOTTOMLEFT));
	ASSERT(hCursor!=NULL);
	m_arrScrollCursor.SetAt(SCRL_BOTTOMLEFT,hCursor);

	hCursor = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_OX_SCRL_LEFTRIGHT));
	ASSERT(hCursor!=NULL);
	m_arrScrollCursor.SetAt(SCRL_LEFTRIGHT,hCursor);

	hCursor = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_OX_SCRL_TOPBOTTOM));
	ASSERT(hCursor!=NULL);
	m_arrScrollCursor.SetAt(SCRL_TOPBOTTOM,hCursor);

	hCursor = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_OX_SCRL_ALL));
	ASSERT(hCursor!=NULL);
	m_arrScrollCursor.SetAt(SCRL_ALL,hCursor);

	ASSERT(m_arrScrollCursor.GetCount()==ID_ICONCOUNT);

	// create helper window
	//
	// make it transparent 
	if(!m_helperWnd.CreateEx(WS_EX_TRANSPARENT|WS_EX_TOOLWINDOW, 
		AfxRegisterWndClass(CS_DBLCLKS),_T(""),WS_POPUP,0,0,0,0,NULL,NULL))
	{
		TRACE(_T("COXIntelliMouse::COXIntelliMouse: unable to create helper window!\n"));
		AfxThrowResourceException();
	}
	ASSERT(::IsWindow(m_helperWnd.m_hWnd));

	m_pOldForegroundWnd=NULL;

}

// Destructor
COXIntelliMouse::~COXIntelliMouse()
{
	m_arrScrollCursor.RemoveAll();
	m_arrScrollRect.RemoveAll();

	if(::IsWindow(m_helperWnd.m_hWnd))
	{
		m_helperWnd.DestroyWindow();
	}
}

void COXIntelliMouse::Reset()
{
	m_bIsHandling=FALSE;
	m_dwScrollFlag=SCRL_NONE;
	m_nScrollTimerID=0;
	m_scrollDirection=SCRL_NONE;
	m_szScrollSize=CSize(0,0);
	m_ptStart=CPoint(-1,-1);
}

BOOL COXIntelliMouse::Attach(CWnd* pWnd, 
							 const UINT nStartEventID/*=WM_MBUTTONDOWN*/,
							 const DWORD dwKeyState/*=0*/)
{
	ASSERT(::IsWindow(pWnd->m_hWnd));

	HookWindow(pWnd);

	m_nStartEventID=nStartEventID;
	m_dwKeyState=dwKeyState;

	return TRUE;
}


void COXIntelliMouse::Detach()
{
	UnhookWindow();

	CancelIntelliMouseSupport();
}

////////////////////////////////////////////////////////////////////////////
//
LRESULT COXIntelliMouse::WindowProc(UINT msg, WPARAM wp, LPARAM lp)
{
#ifdef _WINDLL 
#ifndef _AFXEXT
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	ASSERT(::IsWindow(m_hWndHooked));

	if(msg==m_nStartEventID && !m_bIsHandling)	
	{
		BOOL bKeyState=TRUE;
		if(bKeyState && (m_dwKeyState&KEYPRESSED_CONTROL)!=0)
			bKeyState=(GetKeyState(VK_CONTROL)<0);
		if(bKeyState && (m_dwKeyState&KEYPRESSED_SHIFT)!=0)
			bKeyState=(GetKeyState(VK_SHIFT)<0);
		if(bKeyState && (m_dwKeyState&KEYPRESSED_ALT)!=0)
			bKeyState=(GetKeyState(VK_MENU)<0);

		if(bKeyState)
			if(LaunchIntelliMouseSupport())
				return (LRESULT)0;
	}


	if(m_bIsHandling && ::GetCapture()!=m_hWndHooked)
		CancelIntelliMouseSupport();


	if(m_bIsHandling)
	{
		switch(msg) 
		{
		case WM_NCMOUSEMOVE:	
		case WM_MOUSEMOVE: 
			{
				// define scroll direction
				//
				CPoint ptTest;
				::GetCursorPos(&ptTest);
				m_scrollDirection=FindScrollDirectionFromPoint(ptTest);
				ASSERT(m_scrollDirection!=SCRL_NONE);
				ShowScrollCursor(m_scrollDirection);

				// update scroll speed
				//
				CRect rect;
				VERIFY(m_arrScrollRect.Lookup(m_scrollDirection,rect));
				switch(m_scrollDirection)
				{
				case SCRL_LEFT:
					{
						m_szScrollSize=CSize((ptTest.x-rect.right)/ID_SCROLLCOEF,0);
						break;
					}
				case SCRL_TOPLEFT:
					{
						m_szScrollSize=CSize((ptTest.x-rect.right)/ID_SCROLLCOEF,
							(ptTest.y-rect.bottom)/10);
						break;
					}
				case SCRL_TOP:
					{
						m_szScrollSize=CSize(0,(ptTest.y-rect.bottom)/10);
						break;
					}
				case SCRL_TOPRIGHT:
					{
						m_szScrollSize=CSize((ptTest.x-rect.left)/ID_SCROLLCOEF,
							(ptTest.y-rect.bottom)/10);
						break;
					}
				case SCRL_RIGHT:
					{
						m_szScrollSize=CSize((ptTest.x-rect.left)/ID_SCROLLCOEF,0);
						break;
					}
				case SCRL_BOTTOMRIGHT:
					{
						m_szScrollSize=CSize((ptTest.x-rect.left)/ID_SCROLLCOEF,
							(ptTest.y-rect.top)/10);
						break;
					}
				case SCRL_BOTTOM:
					{
						m_szScrollSize=CSize(0,(ptTest.y-rect.top)/10);
						break;
					}
				case SCRL_BOTTOMLEFT:
					{
						m_szScrollSize=CSize((ptTest.x-rect.right)/ID_SCROLLCOEF,
							(ptTest.y-rect.top)/10);
						break;
					}
				case SCRL_TOPBOTTOM:
				case SCRL_LEFTRIGHT:
				case SCRL_ALL:
					{
						m_szScrollSize=CSize(0,0);
						break;
					}
				default:
					ASSERT(FALSE);
				}

				break;
			}
		case WM_SETCURSOR:
			{
				return TRUE;
			}
		case WM_TIMER:
			{
				if(wp==m_nScrollTimerID)
				{
					int nScrollCode;
					BOOL bScrollViewDerived=
						GetHookedWnd()->IsKindOf(RUNTIME_CLASS(CScrollView));
					if(m_szScrollSize.cx!=0)
					{
						nScrollCode=m_szScrollSize.cx>0 ? SB_LINERIGHT : SB_LINELEFT;
						for(int nIndex=0; nIndex<abs(m_szScrollSize.cx); nIndex++)
						{
							if(bScrollViewDerived)
							{
								((CScrollView*)GetHookedWnd())->
									OnScroll(MAKEWORD((BYTE)nScrollCode,-1),0);
							}
							else
							{
								GetHookedWnd()->SendMessage(WM_HSCROLL,
									MAKEWPARAM(nScrollCode,0),(LPARAM)NULL);
							}
						}
					}

					if(m_szScrollSize.cy!=0)
					{
						nScrollCode=m_szScrollSize.cy>0 ? SB_LINEDOWN : SB_LINEUP;
						for(int nIndex=0; nIndex<abs(m_szScrollSize.cy); nIndex++)
						{
							if(bScrollViewDerived)
							{
								((CScrollView*)GetHookedWnd())->
									OnScroll(MAKEWORD(-1,(BYTE)nScrollCode),0);
							}
							else
							{
								GetHookedWnd()->SendMessage(WM_VSCROLL,
									MAKEWPARAM(nScrollCode,0),(LPARAM)NULL);
							}
						}
					}

					// update scroll info
					GetHookedScrollInfo();
		
					return (LRESULT)0;
				}
				else
					break;
			}
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:	
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_NCMBUTTONDOWN:	
		case WM_NCLBUTTONDBLCLK:
		case WM_NCRBUTTONDBLCLK:
		case WM_NCMBUTTONDBLCLK:
			{
				CancelIntelliMouseSupport();
	
				CPoint point;
				::GetCursorPos(&point);
				CWnd* pWnd=CWnd::WindowFromPoint(point);
				ASSERT(pWnd);

				UINT nHitTest=(int)pWnd->SendMessage(WM_NCHITTEST,0,
					MAKELONG(point.x,point.y));	
				if(nHitTest==HTCLIENT) 
				{
					switch(msg) 
					{
					case WM_NCLBUTTONDOWN: 
						{
							msg=WM_LBUTTONDOWN;		
							break;
						}
					case WM_NCRBUTTONDOWN: 
						{
							msg=WM_RBUTTONDOWN;		
							break;
						}
					case WM_NCMBUTTONDOWN: 
						{
							msg=WM_MBUTTONDOWN;		
							break;
						}
					case WM_NCLBUTTONDBLCLK: 
						{
							msg=WM_LBUTTONDBLCLK;		
							break;
						}
					case WM_NCRBUTTONDBLCLK: 
						{
							msg=WM_RBUTTONDBLCLK;		
							break;
						}
					case WM_NCMBUTTONDBLCLK: 
						{
							msg=WM_MBUTTONDBLCLK;		
							break;
						}
					}
					pWnd->ScreenToClient(&point);		
				} 
				else 
				{	
					switch(msg) 
					{
					case WM_LBUTTONDOWN: 
						{
							msg=WM_NCLBUTTONDOWN;		
							break;
						}
					case WM_RBUTTONDOWN: 
						{
							msg=WM_NCRBUTTONDOWN;		
							break;
						}
					case WM_MBUTTONDOWN: 
						{
							msg=WM_NCMBUTTONDOWN;		
							break;
						}
					case WM_LBUTTONDBLCLK: 
						{
							msg=WM_NCLBUTTONDBLCLK;		
							break;
						}
					case WM_RBUTTONDBLCLK: 
						{
							msg=WM_NCRBUTTONDBLCLK;		
							break;
						}
					case WM_MBUTTONDBLCLK: 
						{
							msg=WM_NCMBUTTONDBLCLK;		
							break;
						}
					}
				}
				wp=nHitTest;
				lp=MAKELONG(point.x,point.y);

				if(pWnd->m_hWnd!=m_hWndHooked)
				{
//					pWnd->PostMessage(msg,wp,lp);
					return (LRESULT)0;
				}
				else
					break;
			}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			{
				CancelIntelliMouseSupport();
				return TRUE;	
			}
		}
	}

	// I don't handle it: pass along
	return COXHookWnd::WindowProc(msg,wp,lp);
}

BOOL COXIntelliMouse::GetHookedScrollInfo()
{
	ASSERT(m_hWndHooked!=NULL);
	ASSERT(::IsWindow(m_hWndHooked));

	CRect rect;
	::GetClientRect(m_hWndHooked,rect);

	::ZeroMemory((void*)&m_scrollHorzInfo,sizeof(m_scrollHorzInfo));
	m_scrollHorzInfo.cbSize=sizeof(m_scrollHorzInfo);
	m_scrollHorzInfo.fMask=SIF_ALL;
	if(!GetHookedWnd()->GetScrollInfo(SB_HORZ,&m_scrollHorzInfo) || 
		(m_scrollHorzInfo.nMax==0 || m_scrollHorzInfo.nPage==0 ||
		m_scrollHorzInfo.nMax<(int)m_scrollHorzInfo.nPage))
	{
		m_dwScrollFlag&=~(SCRL_LEFT|SCRL_RIGHT);
	}
	else
	{
		if(m_scrollHorzInfo.nPos>0)
			m_dwScrollFlag|=SCRL_LEFT;
		else
			m_dwScrollFlag&=~SCRL_LEFT;

		if(m_scrollHorzInfo.nPos+(int)m_scrollHorzInfo.nPage<=m_scrollHorzInfo.nMax)
			m_dwScrollFlag|=SCRL_RIGHT;
		else
			m_dwScrollFlag&=~SCRL_RIGHT;
	}

		
	::ZeroMemory((void*)&m_scrollVertInfo,sizeof(m_scrollVertInfo));
	m_scrollVertInfo.cbSize=sizeof(m_scrollVertInfo);
	m_scrollVertInfo.fMask=SIF_ALL;
	if(!GetHookedWnd()->GetScrollInfo(SB_VERT,&m_scrollVertInfo) || 
		m_scrollVertInfo.nMax==0 || m_scrollVertInfo.nPage==0 ||
		m_scrollVertInfo.nMax<(int)m_scrollVertInfo.nPage)
	{
		m_dwScrollFlag&=~(SCRL_TOP|SCRL_BOTTOM);
	}
	else
	{
		if(m_scrollVertInfo.nPos>0)
			m_dwScrollFlag|=SCRL_TOP;
		else
			m_dwScrollFlag&=~SCRL_TOP;

		if(m_scrollVertInfo.nPos+(int)m_scrollVertInfo.nPage<=m_scrollVertInfo.nMax)
			m_dwScrollFlag|=SCRL_BOTTOM;
		else
			m_dwScrollFlag&=~SCRL_BOTTOM;
	}

	if((m_dwScrollFlag&SCRL_LEFT)==0 && m_szScrollSize.cx<0)
		m_szScrollSize.cx=0;
	if((m_dwScrollFlag&SCRL_RIGHT)==0 && m_szScrollSize.cx>0)
		m_szScrollSize.cx=0;
	if((m_dwScrollFlag&SCRL_TOP)==0 && m_szScrollSize.cy<0)
		m_szScrollSize.cy=0;
	if((m_dwScrollFlag&SCRL_BOTTOM)==0 && m_szScrollSize.cy>0)
		m_szScrollSize.cy=0;
			

	return TRUE;
}

void COXIntelliMouse::CalculateScrollRects(const CSize& szSplashWindowBitmap, 
										   const CPoint& ptStart,
										   SCROLLDIRECTION scrollDirection)
{
	CRect rect(-1,-1,-1,-1);
	m_arrScrollRect.SetAt(SCRL_NONE,rect);
	m_arrScrollRect.SetAt(SCRL_LEFT,rect);
	m_arrScrollRect.SetAt(SCRL_TOPLEFT,rect);
	m_arrScrollRect.SetAt(SCRL_TOP,rect);
	m_arrScrollRect.SetAt(SCRL_TOPRIGHT,rect);
	m_arrScrollRect.SetAt(SCRL_RIGHT,rect);
	m_arrScrollRect.SetAt(SCRL_BOTTOMRIGHT,rect);
	m_arrScrollRect.SetAt(SCRL_BOTTOM,rect);
	m_arrScrollRect.SetAt(SCRL_BOTTOMLEFT,rect);
	m_arrScrollRect.SetAt(SCRL_ALL,rect);
	m_arrScrollRect.SetAt(SCRL_LEFTRIGHT,rect);
	m_arrScrollRect.SetAt(SCRL_TOPBOTTOM,rect);

	// screen's dimension (big enough :-))
	int nWidthScreen=::GetSystemMetrics(SM_CXSCREEN);
	int nHeightScreen=::GetSystemMetrics(SM_CYSCREEN);

	if((m_dwScrollFlag&SCRL_LEFT)!=0 || (m_dwScrollFlag&SCRL_RIGHT)!=0)
	{
		// rectangle for left scrolling area
		rect.left=0;
		rect.right=ptStart.x-szSplashWindowBitmap.cx/2;
		if((m_dwScrollFlag&SCRL_TOP)!=0 || (m_dwScrollFlag&SCRL_BOTTOM)!=0)
		{
			rect.top=ptStart.y-szSplashWindowBitmap.cy/2;
			rect.bottom=rect.top+szSplashWindowBitmap.cy;
		}
		else
		{
			rect.top=0;
			rect.bottom=nHeightScreen;
		}
		m_arrScrollRect.SetAt(SCRL_LEFT,rect);

		// rectangle for right scrolling area
		rect.right=nWidthScreen;
		rect.left=ptStart.x+szSplashWindowBitmap.cx/2+szSplashWindowBitmap.cx%2;
		if((m_dwScrollFlag&SCRL_TOP)!=0 || (m_dwScrollFlag&SCRL_BOTTOM)!=0)
		{
			rect.top=ptStart.y-szSplashWindowBitmap.cy/2;
			rect.bottom=rect.top+szSplashWindowBitmap.cy;
		}
		else
		{
			rect.top=0;
			rect.bottom=nHeightScreen;
		}
		m_arrScrollRect.SetAt(SCRL_RIGHT,rect);
	}

	if((m_dwScrollFlag&SCRL_TOP)!=0 || (m_dwScrollFlag&SCRL_BOTTOM)!=0)
	{
		// rectangle for top scrolling area
		rect.top=0;
		rect.bottom=ptStart.y-szSplashWindowBitmap.cy/2;
		if((m_dwScrollFlag&SCRL_LEFT)!=0 || (m_dwScrollFlag&SCRL_RIGHT)!=0)
		{
			rect.left=ptStart.x-szSplashWindowBitmap.cx/2;
			rect.right=rect.left+szSplashWindowBitmap.cx;
		}
		else
		{
			rect.left=0;
			rect.right=nWidthScreen;
		}
		m_arrScrollRect.SetAt(SCRL_TOP,rect);

		// rectangle for bottom scrolling area
		rect.bottom=nHeightScreen;
		rect.top=ptStart.y+szSplashWindowBitmap.cy/2+szSplashWindowBitmap.cy%2;
		if((m_dwScrollFlag&SCRL_LEFT)!=0 || (m_dwScrollFlag&SCRL_RIGHT)!=0)
		{
			rect.left=ptStart.x-szSplashWindowBitmap.cx/2;
			rect.right=rect.left+szSplashWindowBitmap.cx;
		}
		else
		{
			rect.left=0;
			rect.right=nWidthScreen;
		}
		m_arrScrollRect.SetAt(SCRL_BOTTOM,rect);
	}

	if(((m_dwScrollFlag&SCRL_TOP)!=0 || (m_dwScrollFlag&SCRL_BOTTOM)!=0) && 
		((m_dwScrollFlag&SCRL_LEFT)!=0 || (m_dwScrollFlag&SCRL_RIGHT)!=0))
	{
		// rectangle for topleft scrolling area
		rect.top=0;
		rect.bottom=ptStart.y-szSplashWindowBitmap.cy/2;
		rect.left=0;
		rect.right=ptStart.x-szSplashWindowBitmap.cx/2;
		m_arrScrollRect.SetAt(SCRL_TOPLEFT,rect);

		// rectangle for topright scrolling area
		rect.left=ptStart.x+szSplashWindowBitmap.cx/2+szSplashWindowBitmap.cx%2;
		rect.right=nWidthScreen;
		m_arrScrollRect.SetAt(SCRL_TOPRIGHT,rect);

		// rectangle for bottomleft scrolling area
		rect.top=ptStart.y+szSplashWindowBitmap.cy/2+szSplashWindowBitmap.cy%2;
		rect.bottom=nHeightScreen;
		rect.left=0;
		rect.right=ptStart.x-szSplashWindowBitmap.cx/2;
		m_arrScrollRect.SetAt(SCRL_BOTTOMLEFT,rect);

		// rectangle for bottomright scrolling area
		rect.left=ptStart.x+szSplashWindowBitmap.cx/2+szSplashWindowBitmap.cx%2;
		rect.right=nWidthScreen;
		m_arrScrollRect.SetAt(SCRL_BOTTOMRIGHT,rect);

		// rectangle for non-scrolling area which equals to the size
		// of splash window
		rect.left=ptStart.x-szSplashWindowBitmap.cx/2;
		rect.top=ptStart.y-szSplashWindowBitmap.cy/2;
		rect.right=rect.left+szSplashWindowBitmap.cx;
		rect.bottom=rect.top+szSplashWindowBitmap.cy;
		m_arrScrollRect.SetAt(scrollDirection,rect);
	}
	else
	{
		// rectangle for non-scrolling
		if((m_dwScrollFlag&SCRL_LEFT)!=0 || (m_dwScrollFlag&SCRL_RIGHT)!=0)
		{
			rect.left=ptStart.x-szSplashWindowBitmap.cx/2;
			rect.top=0;
			rect.right=rect.left+szSplashWindowBitmap.cx;
			rect.bottom=nHeightScreen;
			m_arrScrollRect.SetAt(scrollDirection,rect);
		}
		else if((m_dwScrollFlag&SCRL_TOP)!=0 || (m_dwScrollFlag&SCRL_BOTTOM)!=0)
		{
			rect.left=0;
			rect.top=ptStart.y-szSplashWindowBitmap.cy/2;
			rect.right=nWidthScreen;
			rect.bottom=rect.top+szSplashWindowBitmap.cy;
			m_arrScrollRect.SetAt(scrollDirection,rect);
		}
		else
			ASSERT(FALSE);
	}

}


void COXIntelliMouse::ShowScrollCursor(SCROLLDIRECTION scrollDirection) const
{
	ASSERT(scrollDirection!=SCRL_NONE);

	HANDLE hCursor=NULL;
	VERIFY(m_arrScrollCursor.Lookup(scrollDirection,hCursor));
	ASSERT(hCursor!=NULL);

	::SetCursor((HCURSOR)hCursor);
}


SCROLLDIRECTION COXIntelliMouse::
	FindScrollDirectionFromPoint(const CPoint& ptTest) const
{
	CRect rect;
	SCROLLDIRECTION scrollDirection=SCRL_NONE;
	BOOL bSuccess=FALSE;
	POSITION pos=m_arrScrollRect.GetStartPosition();
	while(pos!=NULL)
	{
		m_arrScrollRect.GetNextAssoc(pos,scrollDirection,rect);
		if(rect.PtInRect(ptTest))
		{
			bSuccess=TRUE;
			break;
		}
	}

	if(!bSuccess)
		scrollDirection=SCRL_NONE;

	return scrollDirection;
}

void COXIntelliMouse::CancelIntelliMouseSupport()
{
	if(m_bIsHandling)
	{
		m_bIsHandling=FALSE;

		// hide splash window
		m_splashWnd.Hide();

		// hide helper window
		m_helperWnd.SetWindowPos(&CWnd::wndBottom,0,0,0,0,
			SWP_HIDEWINDOW|SWP_NOREDRAW);
		ASSERT(::IsWindow(m_pOldForegroundWnd->m_hWnd));
		m_pOldForegroundWnd->SetForegroundWindow();

		// release capture
		::ReleaseCapture();

		// kill scrolling timer
		if(m_nScrollTimerID!=0)
		{
			::KillTimer(m_hWndHooked,m_nScrollTimerID);
		}
	}

	// reset all internal variables
	Reset();
}

BOOL COXIntelliMouse::LaunchIntelliMouseSupport()
{
	if(GetHookedScrollInfo() && m_dwScrollFlag!=SCRL_NONE)
	{
		ASSERT(m_nScrollTimerID==0);
		// setup timer
		m_nScrollTimerID=::SetTimer(m_hWndHooked,
			ID_INTELLIMOUSESCROLLTIMER,ID_INTELLIMOUSESCROLLDELAY,NULL);
		if(m_nScrollTimerID!=0)
		{
			// start point
			::GetCursorPos(&m_ptStart);

			// get size of splash window bitmap
			//
			m_splashWnd.SetUserCancel(FALSE);
			COXSplashWndDIB* pDIB=m_splashWnd.GetDIB();
			ASSERT(pDIB!=NULL);
			ASSERT(pDIB->GetBitmap()!=NULL);
			BITMAP bm;
			pDIB->GetBitmapInfo(bm);

			// use helper window
			//
			m_pOldForegroundWnd=CWnd::GetForegroundWindow();
			if(m_pOldForegroundWnd==NULL)
				m_pOldForegroundWnd=CWnd::GetDesktopWindow();
			m_helperWnd.SetWindowPos(&CWnd::wndTopMost,0,0,
				::GetSystemMetrics(SM_CXSCREEN),
				::GetSystemMetrics(SM_CYSCREEN),
				SWP_SHOWWINDOW|SWP_NOACTIVATE|SWP_NOREDRAW);

			// show splash window
			//
			CPoint ptDisplayAt(m_ptStart.x-bm.bmWidth/2,
				m_ptStart.y-bm.bmHeight/2);
			if(!m_splashWnd.Show(SPLASH_NOTIMER,NULL,&ptDisplayAt))
			{
				// kill scrolling timer
				::KillTimer(m_hWndHooked,m_nScrollTimerID);
				m_nScrollTimerID=0;
				// hide helper window
				m_helperWnd.SetWindowPos(&CWnd::wndBottom,0,0,0,0,
					SWP_HIDEWINDOW|SWP_NOREDRAW);
				ASSERT(::IsWindow(m_pOldForegroundWnd->m_hWnd));
				m_pOldForegroundWnd->SetForegroundWindow();
			}
			else
			{
				// current scroll direction
				if(((m_dwScrollFlag&SCRL_TOP)!=0 || 
					(m_dwScrollFlag&SCRL_BOTTOM)!=0) && 
					((m_dwScrollFlag&SCRL_LEFT)!=0 || 
					(m_dwScrollFlag&SCRL_RIGHT)!=0))
					m_scrollDirection=SCRL_ALL;
				else if((m_dwScrollFlag&SCRL_TOP)!=0 || 
					(m_dwScrollFlag&SCRL_BOTTOM)!=0)
					m_scrollDirection=SCRL_TOPBOTTOM;
				else if((m_dwScrollFlag&SCRL_LEFT)!=0 || 
					(m_dwScrollFlag&SCRL_RIGHT)!=0)
					m_scrollDirection=SCRL_LEFTRIGHT;
				else
					ASSERT(FALSE);

				// calculate rectangles
				//
				CalculateScrollRects(CSize(bm.bmWidth,bm.bmHeight),m_ptStart,
					m_scrollDirection);

				// scroll sizes
				m_szScrollSize=CSize(0,0);

				// set mouse cursor 
				ShowScrollCursor(m_scrollDirection);

				// capture all mouse messages
				::SetCapture(m_hWndHooked);

				// we are in the scrolling mode
				m_bIsHandling=TRUE;

				return TRUE;
			}
		}
	}

	return FALSE;
}


///////////////////////////////////////////////////////////

// static variables
CMap<DWORD,DWORD,COXIntelliMouseOrganizer*,COXIntelliMouseOrganizer*> 
COXIntelliMouseOrganizer::m_arrThreadOrganizers;

HHOOK COXIntelliMouseOrganizer::m_pfnOriginalCBTHookProc=NULL;
HHOOK COXIntelliMouseOrganizer::m_pfnOriginalGetMessageHookProc=NULL;
/////////////////


// Constructor
COXIntelliMouseOrganizer::COXIntelliMouseOrganizer() : 
	m_pfnOldCBTHookProc(NULL), 
	m_dwThreadID(NULL),
	m_pfnOldGetMessageHookProc(NULL),
	m_nThreadOrigStartEventID(NULL),
	m_dwThreadOrigKeyState(NULL)
{
}

// Destructor
COXIntelliMouseOrganizer::~COXIntelliMouseOrganizer()
{
	if(IsAttachedAllInThread())
		VERIFY(DetachAllInThread());
	else
		VERIFY(Detach(NULL)); 

	int nCount=PtrToInt(m_arrUsedObjects.GetSize());
	for(int nIndex=0; nIndex<nCount; nIndex++)
	{
		COXIntelliMouse* pIntelliMouseObject=m_arrUsedObjects[nIndex];
		ASSERT(pIntelliMouseObject!=NULL);
		ASSERT(!pIntelliMouseObject->IsHooked());
		delete pIntelliMouseObject;
	}
	m_arrUsedObjects.RemoveAll();

	ASSERT(m_pfnOriginalCBTHookProc==NULL);
	ASSERT(m_pfnOldCBTHookProc==NULL);
	ASSERT(m_pfnOriginalGetMessageHookProc==NULL);
}

COXIntelliMouse* COXIntelliMouseOrganizer::Attach(CWnd* pWnd, 
												  UINT nStartEventID/*=WM_MBUTTONDOWN*/, 
												  DWORD dwKeyState/*=0*/)
{
	ASSERT(pWnd!=NULL);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	HWND hWndAttached=pWnd->GetSafeHwnd();
	ASSERT(::IsWindow(hWndAttached));
	COXIntelliMouse* pIntelliMouseObject=NULL;
	if(m_arrAttachedWnd.Lookup(hWndAttached,pIntelliMouseObject))
	{
		ASSERT(pIntelliMouseObject!=NULL);
		ASSERT(pIntelliMouseObject->IsHooked());
		ASSERT(pIntelliMouseObject->GetHookedWnd()->GetSafeHwnd()==hWndAttached);
		TRACE(_T("COXIntelliMouseOrganizer::Attach: specified window already attached to an intelli mouse object\n"));
		return pIntelliMouseObject;
	}

	if(IsRejected(hWndAttached))
	{
		return FALSE;
	}


	if(IsAttachedAllInThread())
	{
		CString sWndClass;
		::GetClassName(pWnd->GetSafeHwnd(),sWndClass.GetBuffer(512),512);

		if(!IsQualified(pWnd->GetSafeHwnd(),sWndClass))
		{
			m_mapRejectedControls.SetAt(pWnd->GetSafeHwnd(),1);
			return NULL;
		}
	}

	if(m_arrUsedObjects.GetSize()>0)
	{
		pIntelliMouseObject=m_arrUsedObjects[0];
		ASSERT(pIntelliMouseObject!=NULL);
		ASSERT(!pIntelliMouseObject->IsHooked());
		m_arrUsedObjects.RemoveAt(0);
	}
	else
	{
		pIntelliMouseObject=new COXIntelliMouse;
	}

	if(pIntelliMouseObject->Attach(pWnd,nStartEventID,dwKeyState))
	{
		m_arrAttachedWnd.SetAt(hWndAttached,pIntelliMouseObject);
		return pIntelliMouseObject;
	}
	else
	{
		// save the object in the array of COXIntelliMouse objects 
		// that can be used later
		m_arrUsedObjects.Add(pIntelliMouseObject);
		return NULL;
	}
}

BOOL COXIntelliMouseOrganizer::Detach(CWnd* pWnd/*=NULL*/)
{
	if(pWnd==NULL)
	{
		POSITION pos=m_arrAttachedWnd.GetStartPosition();
		HWND hAttachedWnd=NULL;
		COXIntelliMouse* pIntelliMouseObject=NULL;
		while(pos!=NULL)
		{
			m_arrAttachedWnd.GetNextAssoc(pos,hAttachedWnd,pIntelliMouseObject);
			if(pIntelliMouseObject!=NULL && pIntelliMouseObject->IsHooked() && 
				pIntelliMouseObject->GetHookedWnd()->GetSafeHwnd()!=hAttachedWnd)
			{

				COXIntelliMouse* pTheSameIntelliMouseObject=NULL;
				if(m_arrAttachedWnd.
					Lookup(pIntelliMouseObject->GetHookedWnd()->GetSafeHwnd(),
					pTheSameIntelliMouseObject))
				{
					ASSERT(pIntelliMouseObject==pTheSameIntelliMouseObject);
				}
				else
				{
					if(pIntelliMouseObject->IsHooked())
						pIntelliMouseObject->Detach();
					// save the object in the array of COXIntelliMouse objects 
					// that can be used later
					m_arrUsedObjects.Add(pIntelliMouseObject);
				}
			}
			else
			{
				ASSERT(pIntelliMouseObject!=NULL);
				ASSERT(::IsWindow(hAttachedWnd) || pIntelliMouseObject==NULL ||
					(pIntelliMouseObject!=NULL && !pIntelliMouseObject->IsHooked()));
				if(pIntelliMouseObject!=NULL)
				{
					if(pIntelliMouseObject->IsHooked())
						pIntelliMouseObject->Detach();
					// save the object in the array of COXIntelliMouse objects 
					// that can be used later
					m_arrUsedObjects.Add(pIntelliMouseObject);
				}
			}
		}
		m_arrAttachedWnd.RemoveAll();
	}
	else
	{
		COXIntelliMouse* pIntelliMouseObject=NULL;
		CWnd* pAttachedWnd=(CWnd*)pWnd;
		if(!m_arrAttachedWnd.Lookup(pAttachedWnd->GetSafeHwnd(),pIntelliMouseObject))
			return FALSE;
		ASSERT(pIntelliMouseObject!=NULL);
		m_arrAttachedWnd.RemoveKey(pAttachedWnd->GetSafeHwnd());
		if(pIntelliMouseObject!=NULL)
		{
			if(pIntelliMouseObject->IsHooked())
				pIntelliMouseObject->Detach();
			// save the object in the array of COXIntelliMouse objects 
			// that can be used later
			m_arrUsedObjects.Add(pIntelliMouseObject);
		}
	}

	return TRUE;
}


UINT COXIntelliMouseOrganizer::GetStartEventID(CWnd* pWnd) const
{
	ASSERT(pWnd!=NULL);
	ASSERT(IsAttached(pWnd));
	return GetIntelliMouseObject(pWnd)->GetStartEventID();
}

UINT COXIntelliMouseOrganizer::GetKeyPressedState(CWnd* pWnd) const
{
	ASSERT(pWnd!=NULL);
	ASSERT(IsAttached(pWnd));
	return GetIntelliMouseObject(pWnd)->GetKeyPressedState();
}

	
BOOL COXIntelliMouseOrganizer::SetStartEventID(CWnd* pWnd, 
											   UINT nStartEventID, 
											   DWORD dwKeyState/*=0*/)
{
	if(pWnd==NULL)
	{
		if(IsAttachedAllInThread())
		{
			m_nThreadOrigStartEventID=nStartEventID;
			m_dwThreadOrigKeyState=dwKeyState;
		}
		POSITION pos=m_arrAttachedWnd.GetStartPosition();
		HWND hAttachedWnd=NULL;
		COXIntelliMouse* pIntelliMouseObject=NULL;
		while(pos!=NULL)
		{
			m_arrAttachedWnd.GetNextAssoc(pos,hAttachedWnd,pIntelliMouseObject);
			ASSERT(::IsWindow(hAttachedWnd));
			ASSERT(pIntelliMouseObject!=NULL);
			pIntelliMouseObject->SetStartEventID(nStartEventID,dwKeyState);
		}
	}
	else
	{
		COXIntelliMouse* pIntelliMouseObject=GetIntelliMouseObject(pWnd);
		if(pIntelliMouseObject==NULL)
			return FALSE;
		pIntelliMouseObject->SetStartEventID(nStartEventID,dwKeyState);
	}

	return TRUE;
}


COXIntelliMouse* COXIntelliMouseOrganizer::
GetIntelliMouseObject(const CWnd* pWnd) const
{
	ASSERT(pWnd!=NULL);

	COXIntelliMouse* pIntelliMouseObject=NULL;
	CWnd* pAttachedWnd=(CWnd*)pWnd;
	if(!m_arrAttachedWnd.Lookup(pAttachedWnd->GetSafeHwnd(),pIntelliMouseObject))
	{
		pIntelliMouseObject=NULL;
	}
	else
	{
		ASSERT(pIntelliMouseObject->IsHooked());
		ASSERT(pIntelliMouseObject->GetHookedWnd()==pAttachedWnd);
	}

	return pIntelliMouseObject;
}

BOOL COXIntelliMouseOrganizer::IsAttached(const CWnd* pWnd) const
{
	ASSERT(pWnd!=NULL);
	if(pWnd==NULL)
		return FALSE;

	COXIntelliMouse* pIntelliMouseObject=NULL;
	CWnd* pAttachedWnd=(CWnd*)pWnd;
	if(m_arrAttachedWnd.Lookup(pAttachedWnd->GetSafeHwnd(),pIntelliMouseObject))
	{
		ASSERT(pIntelliMouseObject!=NULL);
		ASSERT(pIntelliMouseObject->IsHooked());
		ASSERT(pIntelliMouseObject->GetHookedWnd()==pAttachedWnd);
		if(pIntelliMouseObject!=NULL)
			return TRUE;
	}

	return FALSE;
}


BOOL COXIntelliMouseOrganizer::IsRejected(HWND hWnd) const
{
	DWORD dwReason=0;
	BOOL bResult=m_mapRejectedControls.Lookup(hWnd,dwReason);
	ASSERT(!bResult || dwReason!=0);
	return bResult;
}


BOOL COXIntelliMouseOrganizer::
AttachAllInThread(DWORD dwThreadID/*=::GetCurrentThreadId()*/,
				  UINT nStartEventID/*=WM_MBUTTONDOWN*/, 
				  DWORD dwKeyState/*=0*/)
{
	if(IsAttachedAllInThread())
	{
		TRACE(_T("COXIntelliMouseOrganizer::AttachAllInThread: this object already attached to a thread\n"));
		return FALSE;
	}

	COXIntelliMouseOrganizer* pOrganizer=NULL;
	if(m_arrThreadOrganizers.Lookup(dwThreadID,pOrganizer))
	{
		ASSERT(pOrganizer!=NULL);
		TRACE(_T("COXIntelliMouseOrganizer::AttachAllInThread: specified thread already attached to a COXIntelliMouseOrganizer object\n"));
		return FALSE;
	}
	m_arrThreadOrganizers.SetAt(dwThreadID,this);

	m_dwThreadID=dwThreadID;

	m_nThreadOrigStartEventID=nStartEventID;
	m_dwThreadOrigKeyState=dwKeyState;

	// go through all windows and attach them
	::EnumWindows(&EnumThreadWindows,(LPARAM)this);

	// setup hooks for Computer Based Training
	if(m_pfnOriginalCBTHookProc==NULL)
	{
		m_pfnOriginalCBTHookProc=
			::SetWindowsHookEx(WH_CBT,IntelliMouseCBTHookProc,NULL,dwThreadID);
		m_pfnOldCBTHookProc=m_pfnOriginalCBTHookProc;
	}
	else
	{
		m_pfnOldCBTHookProc=
			::SetWindowsHookEx(WH_CBT,IntelliMouseCBTHookProc,NULL,dwThreadID);
	}

	// setup hooks for GetMessage
	if(m_pfnOriginalGetMessageHookProc==NULL)
	{
		m_pfnOriginalGetMessageHookProc=::SetWindowsHookEx(WH_GETMESSAGE,
			IntelliMouseGetMessageHookProc,NULL,dwThreadID);
		m_pfnOldGetMessageHookProc=m_pfnOriginalGetMessageHookProc;
	}
	else
	{
		m_pfnOldGetMessageHookProc=::SetWindowsHookEx(WH_GETMESSAGE,
			IntelliMouseGetMessageHookProc,NULL,dwThreadID);
	}

	SetStartEventID(NULL,nStartEventID,dwKeyState);

	return TRUE;
}


void COXIntelliMouseOrganizer::AttachAllWindows(HWND hWndStartFrom)
{
	ASSERT(hWndStartFrom!=NULL);

	HWND hWnd=hWndStartFrom;
	while(hWnd!=NULL)
	{
		DWORD dwThreadID=::GetWindowThreadProcessId(hWnd,NULL);
		if(dwThreadID!=GetAttachedThread())
			break;

		CWnd* pWnd=CWnd::FromHandlePermanent(hWnd);
		if(pWnd!=NULL && !IsAttached(pWnd) && !IsRejected(hWnd))
		{
			Attach(pWnd);
		}

		// loop through children
		HWND hWndChild=::GetWindow(hWnd,GW_CHILD);
		if(hWndChild!=NULL && !IsRejected(hWnd) && !IsAttached(pWnd))
			AttachAllWindows(hWndChild);

		// loop through windows
		hWnd=::GetWindow(hWnd,GW_HWNDNEXT);
	}
}


LRESULT CALLBACK COXIntelliMouseOrganizer::
IntelliMouseCBTHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	if(nCode>=0 && ::IsWindow((HWND)wParam))
	{
		DWORD dwThreadID=::GetWindowThreadProcessId((HWND)wParam,NULL);
		COXIntelliMouseOrganizer* pOrganizer=NULL;
		if(COXIntelliMouseOrganizer::m_arrThreadOrganizers.
			Lookup(dwThreadID,pOrganizer))
		{
			ASSERT(pOrganizer!=NULL);
			ASSERT(pOrganizer->IsAttachedAllInThread());

			if(nCode==HCBT_DESTROYWND)
			{
				// check if the window that is about to be destroyed
				// had been added to intelli mouse organizer list
				CWnd* pWnd=CWnd::FromHandle((HWND)wParam);
				if(pWnd!=NULL && pOrganizer->IsAttached(pWnd))
					VERIFY(pOrganizer->Detach(pWnd));
			}
			else
			{
				// check if new window is created and attach it.
				CWnd* pWnd=CWnd::FromHandlePermanent((HWND)wParam);
				if(pWnd!=NULL && !pOrganizer->IsAttached(pWnd))
				{
					POSITION pos=NULL;
					COXIntelliMouse* pIntelliMouseObject=
						pOrganizer->GetFirstIntelliMouseObject(pos);
					if(pIntelliMouseObject!=NULL)
					{
						pOrganizer->
							Attach(pWnd,pIntelliMouseObject->GetStartEventID(),
							pIntelliMouseObject->GetKeyPressedState());
					}
					else
					{
						pOrganizer->
							Attach(pWnd,pOrganizer->m_nThreadOrigStartEventID,
							pOrganizer->m_dwThreadOrigKeyState);
					}
				}
			}

			return ::CallNextHookEx(pOrganizer->GetSavedCBTHookProc(),
				nCode,wParam,lParam);
		}
	}
	return ::CallNextHookEx(COXIntelliMouseOrganizer::GetOriginalCBTHookProc(),
		nCode,wParam,lParam);
}


LRESULT CALLBACK COXIntelliMouseOrganizer::
IntelliMouseGetMessageHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	if(nCode>=0 && ::IsWindow(((MSG*)lParam)->hwnd))
	{
		DWORD dwThreadID=::GetWindowThreadProcessId(((MSG*)lParam)->hwnd,NULL);
		COXIntelliMouseOrganizer* pOrganizer=NULL;
		if(COXIntelliMouseOrganizer::
			m_arrThreadOrganizers.Lookup(dwThreadID,pOrganizer))
		{
			ASSERT(pOrganizer!=NULL);
			ASSERT(pOrganizer->IsAttachedAllInThread());

			// check if new window is created and attach it.
			CWnd* pWnd=CWnd::FromHandlePermanent(((MSG*)lParam)->hwnd);
			if(pWnd!=NULL && !pOrganizer->IsAttached(pWnd))
			{
				POSITION pos=NULL;
				COXIntelliMouse* pIntelliMouseObject=
					pOrganizer->GetFirstIntelliMouseObject(pos);
				if(pIntelliMouseObject!=NULL)
				{
					pOrganizer->
						Attach(pWnd,pIntelliMouseObject->GetStartEventID(),
						pIntelliMouseObject->GetKeyPressedState());
				}
				else
				{
					pOrganizer->
						Attach(pWnd,pOrganizer->m_nThreadOrigStartEventID,
						pOrganizer->m_dwThreadOrigKeyState);
				}
			}

			return ::CallNextHookEx(pOrganizer->GetSavedGetMessageHookProc(),
				nCode,wParam,lParam);
		}
	}

	return ::CallNextHookEx(COXIntelliMouseOrganizer::GetOriginalGetMessageHookProc(),
		nCode,wParam,lParam);
}


BOOL CALLBACK COXIntelliMouseOrganizer::EnumThreadWindows(HWND hWnd, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	ASSERT(lParam!=NULL);
	ASSERT(::IsWindow(hWnd));
	COXIntelliMouseOrganizer* pOrganizer=(COXIntelliMouseOrganizer*)lParam;
	ASSERT(pOrganizer->IsAttachedAllInThread());

	DWORD dwThreadID=::GetWindowThreadProcessId(hWnd,NULL);
	if(dwThreadID==pOrganizer->GetAttachedThread())
	{
		pOrganizer->AttachAllWindows(hWnd);
		return FALSE;
	}
	return TRUE;
}


BOOL COXIntelliMouseOrganizer::DetachAllInThread()
{
	if(!IsAttachedAllInThread())
		return FALSE;

	ASSERT(m_dwThreadID!=NULL);
	ASSERT(m_pfnOldCBTHookProc!=NULL);
	ASSERT(m_pfnOriginalCBTHookProc!=NULL);
	ASSERT(m_pfnOldGetMessageHookProc!=NULL);
	ASSERT(m_pfnOriginalGetMessageHookProc!=NULL);

	// unhook CBT
	if(m_pfnOldCBTHookProc!=NULL)
	{
		VERIFY(::UnhookWindowsHookEx(m_pfnOldCBTHookProc));
		m_pfnOldCBTHookProc=NULL;
		m_pfnOriginalCBTHookProc=NULL;
	}

	// unhook GetMessage
	if(m_pfnOldGetMessageHookProc!=NULL)
	{
		VERIFY(::UnhookWindowsHookEx(m_pfnOldGetMessageHookProc));
		m_pfnOldGetMessageHookProc=NULL;
		m_pfnOriginalGetMessageHookProc=NULL;
	}

	// clear map of rejected windows
	m_mapRejectedControls.RemoveAll();

	m_arrThreadOrganizers.RemoveKey(m_dwThreadID);

	m_dwThreadID=NULL;

	return Detach(NULL);
}

