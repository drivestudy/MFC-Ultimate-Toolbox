// ==========================================================================
// 					Class Implementation : COXPopupBarCtrl
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

// //////////////////////////////////////////////////////////////////////////

// OXPopupBarCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "OXPopupBarCtrl.h"

#include <windowsx.h>
#include <afxpriv.h>

#include "UTB64Bit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define POPUPBAR_CLASSNAME	_T("PopupBarControl")

// Messages sent to popup bar window
// 
// m_nOkMessage			-	hide popup bar control and return TRUE from Pick function
// m_nCancelMessage		-	hide popup bar control and return FALSE from Pick function
// m_nCustomButtonPicked-	hide popup bar control and call virtual OnCustomButton 
//							function. Result of OnCustomButton function is used as a 
//							result of Pick function
// m_nTearoffMessage	-	convert popup bar to floating window with caption
//
UINT COXPopupBarCtrl::m_nOkMessage=::RegisterWindowMessage(_T("_POPUPBAR_OK_"));
UINT COXPopupBarCtrl::m_nCancelMessage=
::RegisterWindowMessage(_T("_POPUPBAR_CANCEL_"));
UINT COXPopupBarCtrl::m_nCustomButtonPicked=
::RegisterWindowMessage(_T("_POPUPBAR_CUSTOMBUTTONPICKED_"));
UINT COXPopupBarCtrl::m_nTearoffMessage=
::RegisterWindowMessage(_T("_POPUPBAR_TEAROFF_"));

/////////////////////////////////////////////////////////////////////////////
// COXPopupBarCtrl

COXPopupBarCtrl::COXPopupBarCtrl(UINT nButtons, UINT nRows, DWORD dwDefault,
								 CSize sizeButton, CString sDefaultButtonText,
								 CString sCustomButtonText) :
m_nButtons(nButtons),
m_nRows(nRows),
m_dwDefault(dwDefault),
m_sizeButton(sizeButton),
m_sDefaultButtonText(sDefaultButtonText),
m_sCustomButtonText(sCustomButtonText),
// Define margines used to draw buttons. 
// In your derived class you can redefine them.
m_rectEdgeMargin(4,2,4,2),
m_rectButtonMargin(0,0,0,0),
m_rectDefaultButtonMargin(0,0,0,2),
m_rectCustomButtonMargin(0,2,0,0),
m_rectTearoffBarMargin(2,-1,2,2),
// by default none button selected
m_dwSelected(ID_POPUPBAR_DATA_NONE),
m_pParentWnd(NULL),
m_bMouseButtonPressed(FALSE),
m_bClickedInTearoffBar(FALSE),
m_bFloating(FALSE),
m_nCheckMouseTimer(0),
m_bCheckMousePos(TRUE),
m_ptLastMousePos(-1,-1),
m_pTrackingDC(NULL),
m_ptLastTrack(0,0),
m_rectTrackBar(0,0,0,0),
m_rectLastTrackBar(0,0,0,0),
m_sizeLastTrackBar(0,0),
m_bDitherLast(FALSE)
{
	m_sTearoffText.LoadString(IDS_OX_POPUPBARTEAROFF);// "Drag to make this menu float"
	m_sCaption.LoadString(IDS_OX_POPUPBARCAPTION);// "Floating bar"
}


COXPopupBarCtrl::COXPopupBarCtrl(UINT nButtons, UINT nRows, DWORD dwDefault,
								 CSize sizeButton, int nIDdefault, int nIDcustom) :
m_nButtons(nButtons),
m_nRows(nRows),
m_dwDefault(dwDefault),
m_sizeButton(sizeButton),
// Define margines used to draw buttons. 
// In your derived class you can redefine them.
m_rectEdgeMargin(4,2,4,2),
m_rectButtonMargin(0,0,0,0),
m_rectDefaultButtonMargin(0,0,0,2),
m_rectCustomButtonMargin(0,2,0,0),
m_rectTearoffBarMargin(2,-1,2,2),
// by default none button selected
m_dwSelected(ID_POPUPBAR_DATA_NONE),
m_pParentWnd(NULL),
m_bMouseButtonPressed(FALSE),
m_bClickedInTearoffBar(FALSE),
m_bFloating(FALSE),
m_nCheckMouseTimer(0),
m_bCheckMousePos(TRUE),
m_ptLastMousePos(-1,-1),
m_pTrackingDC(NULL),
m_ptLastTrack(0,0),
m_rectTrackBar(0,0,0,0),
m_rectLastTrackBar(0,0,0,0),
m_sizeLastTrackBar(0,0),
m_bDitherLast(FALSE)
{
	m_sDefaultButtonText.LoadString(nIDdefault);
	m_sCustomButtonText.LoadString(nIDcustom);
}


COXPopupBarCtrl::~COXPopupBarCtrl()
{
	// clear all data
	m_arrData.RemoveAll();
	m_arrToolTipText.RemoveAll();
	m_arrButtonRect.RemoveAll();

	if((HFONT)m_font!=NULL)
	{
		m_font.DeleteObject();
	}

	if(::IsWindow(m_hWnd))
	{
		DestroyWindow();
	}
}


BEGIN_MESSAGE_MAP(COXPopupBarCtrl, CWnd)
	//{{AFX_MSG_MAP(COXPopupBarCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SYSKEYUP()
	ON_WM_SYSKEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_FLOATSTATUS, OnFloatStatus)
	ON_WM_NCCREATE()
	ON_WM_NCACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL COXPopupBarCtrl::Create(CWnd* pParentWnd, 
							 UINT nPopupBarStyle/*=POPUPBAR_NOTEAROFF*/, 
							 HBRUSH hbrBackground/*=NULL*/)
{
	ASSERT_VALID(pParentWnd);
	// parent window have to exist
	ASSERT(::IsWindow(pParentWnd->GetSafeHwnd()));

	m_pParentWnd=pParentWnd;

	m_nPopupBarStyle=nPopupBarStyle;

	// creation of window
	// 

	if(hbrBackground==NULL)
	{
		hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
	}

	// define our own window class 
	WNDCLASS wndClass;
	wndClass.style=CS_SAVEBITS; 
	wndClass.lpfnWndProc=AfxWndProc; 
	wndClass.cbClsExtra=0; 
	wndClass.cbWndExtra=0; 
	wndClass.hInstance=AfxGetInstanceHandle(); 
	wndClass.hIcon=0; 
	wndClass.hCursor=::LoadCursor(NULL,IDC_ARROW); 
	wndClass.hbrBackground=hbrBackground; 
	wndClass.lpszMenuName=NULL; 
	wndClass.lpszClassName=POPUPBAR_CLASSNAME;

	if(!AfxRegisterClass(&wndClass))
	{
		return FALSE;
	}

	CRect rect(0,0,0,0);
	DWORD dwStyle=WS_POPUP;
	DWORD dwExStyle=WS_EX_DLGMODALFRAME;
	if(IsFloating())
	{
		dwStyle|=WS_CAPTION|WS_VISIBLE|WS_SYSMENU|MFS_SYNCACTIVE; 
		dwExStyle|=WS_EX_TOOLWINDOW;
	}
	if(!CWnd::CreateEx(dwExStyle,wndClass.lpszClassName,m_sCaption,dwStyle,
		rect,m_pParentWnd,NULL,NULL))
	{
		TRACE(_T("COXPopupBarCtrl::Create: failed to create popup bar\n"));
		return FALSE;
	}

	// setup timer to check constantly mouse position
	m_nCheckMouseTimer=SetTimer(IDT_OXPB_CHECKMOUSE,ID_OXPB_CHECKMOUSE_DELAY,NULL);
	if(m_nCheckMouseTimer==0)
	{
		DestroyWindow();
		TRACE(_T("COXPopupBarCtrl::Create: failed to set timer to check mouse position\n"));
		return FALSE;
	}

	// create tooltip control
	if(!m_ctlToolTip.Create(this)) 
	{
		DestroyWindow();
		TRACE(_T("COXPopupBarCtrl::Create: failed to create tooltip control\n"));
		return FALSE;
	}

	// update system menu
	if(dwStyle & WS_SYSMENU)
	{
		CMenu* pSysMenu=GetSystemMenu(FALSE);

		// Remove standard Restore, Maximize and Minimize and Size items out of 
		// the system menu.
		if(pSysMenu!=NULL)
		{
			pSysMenu->RemoveMenu(SC_RESTORE,MF_BYCOMMAND);
			pSysMenu->RemoveMenu(SC_MINIMIZE,MF_BYCOMMAND);
			pSysMenu->RemoveMenu(SC_MAXIMIZE,MF_BYCOMMAND);
			pSysMenu->RemoveMenu(SC_SIZE,MF_BYCOMMAND);
		}
	}

	return TRUE;
}


BOOL COXPopupBarCtrl::Pick(UINT nAlignment, CRect* pParentRect, CSize sizeOffset)
{
	ASSERT_VALID(m_pParentWnd);

	if(IsFloating())
	{
		ASSERT(::IsWindow(GetSafeHwnd()));

		DestroyWindow();
		m_bFloating=FALSE;

		// Get the background brush
		WNDCLASS wc;
		HBRUSH hBrush=NULL;
		if(GetClassInfo(AfxGetInstanceHandle(),POPUPBAR_CLASSNAME,&wc))
			hBrush=wc.hbrBackground;

		if(!Create(m_pParentWnd,m_nPopupBarStyle,hBrush))
			return FALSE;
	}

	ASSERT(::IsWindow(m_pParentWnd->GetSafeHwnd()));
	ASSERT(::IsWindow(m_hWnd));

	ASSERT(m_nButtons<=ID_POPUPBAR_MAX_BUTTONS && m_nButtons>0);
	ASSERT(m_nRows<=ID_POPUPBAR_MAX_BUTTONS && m_nRows>0 && m_nRows<=m_nButtons);

	// nobody has cared to fill array of common buttons
	for(int nIndex= PtrToInt(m_arrData.GetSize()); nIndex<(int)m_nButtons; nIndex++)
	{
		m_arrData.Add(0);
	}
	ASSERT(m_arrData.GetSize()>=(int)m_nButtons);


	// OK, we did all preparations
	//

	// now we've got to calculate size of control's window
	if(!CalcWindowSize())
	{
		return FALSE;
	}


	// a-ha, we've got size of window but we have to display it
	// in a nice spot (at least in the place user wants us to put in)
	if(!AdjustWindowPosition(nAlignment,pParentRect,sizeOffset))
	{
		return FALSE;
	}

	// current index by default is not set
	m_nCurrentIndex=ID_POPUPBAR_INDEX_NONE;
	// set selected index
	m_nSelectedIndex=ID_POPUPBAR_INDEX_NONE;
	if(m_dwSelected!=ID_POPUPBAR_DATA_NONE)
	{
		m_nSelectedIndex=GetIndexFromData(m_dwSelected);
	}


	// now we can populate the tooltip control
	if(!PopulateToolTip())
	{
		return FALSE;
	}

	// make sure to show/hide tooltip depending on the style
	m_ctlToolTip.Activate(IsToolTip());

	// it's show time!
	SetWindowPos(NULL,m_rect.left,m_rect.top,m_rect.Width(),m_rect.Height(),
		SWP_NOZORDER|SWP_NOACTIVATE);
	ShowWindow(SW_SHOWNA);
	HWND hFocusWnd=::GetFocus();
	if(hFocusWnd)
	{
		::SetFocus(NULL);
	}

	// Capture all mouse events for the life of this window
	HWND hOldCaptureOwner=::SetCapture(m_hWnd);

	m_bMouseButtonPressed=FALSE;

	BOOL bRouteLastMessage=FALSE;
	// init message loop
	// m_nState = 0 - control is active
	// m_nState = 1 - user selected some color
	// m_nState = 2 - user left control without selecting any color
	// m_nState = 3 - custom button was pressed
	// m_nState = 4 - menu has been torn off - need some restructing
	m_nState=0;
	while(m_nState==0)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		m_ctlToolTip.RelayEvent(&msg);

		if(::GetCapture() != m_hWnd)
		{
			break;
		}

		if(msg.message==WM_SYSKEYUP)
		{
			OnSysKeyUp(
				PtrToUint(msg.wParam),
				LOWORD(msg.lParam),
				HIWORD(msg.lParam));
			continue;
		}

		if(msg.message==WM_SYSKEYDOWN)
		{
			OnSysKeyDown(
				PtrToUint(msg.wParam),
				LOWORD(msg.lParam),
				HIWORD(msg.lParam));
			continue;
		}

		if(msg.message==m_nOkMessage)
		{
			m_nState=1;
		}
		else if(msg.message==m_nCancelMessage)
		{
			m_nState=2;
		}
		else if(msg.message==m_nCustomButtonPicked)
		{
			m_nState=3;
			break;
		}
		else if(msg.message==m_nTearoffMessage)
		{
			m_nState=4;
			break;
		}
		else
		{
			DispatchMessage(&msg);
		}

		if(msg.message==WM_LBUTTONDOWN)
		{
			m_lastMsg.message=msg.message;
			m_lastMsg.wParam=msg.wParam;
			m_lastMsg.lParam=msg.lParam;
			bRouteLastMessage=TRUE;
		}
		else if(m_nState==0)
		{
			bRouteLastMessage=FALSE;
		}

	}

	if(m_nState!=4)
		ShowWindow(SW_HIDE);

	BOOL bResult=m_nState==1 ? TRUE : FALSE;

	if(m_nState==3)
	{
		bResult=OnCustomButton();
	}

	// give back what took
	if(hFocusWnd)
	{
		::SetFocus(hFocusWnd);
	}
	if(::GetCapture()==m_hWnd)
	{
		::ReleaseCapture();
		if(hOldCaptureOwner!=NULL)
		{
			::SetCapture(hOldCaptureOwner);
		}
	}

	m_bMouseButtonPressed=FALSE;

	if(bRouteLastMessage)
	{
		POINTS points=MAKEPOINTS(m_lastMsg.lParam);	
		CPoint ptScreen(points.x,points.y);
		ClientToScreen(&ptScreen);
		CWnd* pWnd=WindowFromPoint(ptScreen);
		ASSERT(pWnd);

		int nHitTest=(int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(ptScreen.x,ptScreen.y));	
		if (nHitTest == HTCLIENT) 
		{
			pWnd->ScreenToClient(&ptScreen);		
			m_lastMsg.message=WM_LBUTTONDOWN;		
		} 
		else 
		{	
			m_lastMsg.message=WM_NCLBUTTONDOWN;		
		}
		m_lastMsg.wParam=nHitTest;
		m_lastMsg.lParam=MAKELONG(ptScreen.x,ptScreen.y);

		pWnd->PostMessage(m_lastMsg.message,m_lastMsg.wParam,m_lastMsg.lParam);

	}

	if(m_nState==4)
	{
		ConvertToTearOff();
	}

	return bResult;
}


////////////////////////////////////
// Attributes

BOOL COXPopupBarCtrl::SetTextFont(CFont* pFont)
{
	ASSERT_VALID(pFont);
	if((HFONT)m_font!=NULL)
	{
		m_font.DeleteObject();
	}

	LOGFONT lf;
	if(pFont->GetLogFont(&lf))
	{
		return m_font.CreateFontIndirect(&lf);
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// COXPopupBarCtrl message handlers

// handle it to pass a mouse message to a tool tip control for processing 
BOOL COXPopupBarCtrl::PreTranslateMessage(MSG* pMsg)
{
	if(::IsWindow(m_ctlToolTip.GetSafeHwnd()))
	{
		m_ctlToolTip.Activate(TRUE); 
		m_ctlToolTip.RelayEvent(pMsg);
	}
	return CWnd::PreTranslateMessage(pMsg);
}


void COXPopupBarCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnMouseButtonDown(point);

	CWnd::OnLButtonDown(nFlags, point);
}


void COXPopupBarCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnMouseButtonUp(point);

	CWnd::OnLButtonUp(nFlags, point);
}


void COXPopupBarCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnMouseButtonDown(point);

	CWnd::OnRButtonDown(nFlags, point);
}


void COXPopupBarCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnMouseButtonUp(point);

	CWnd::OnRButtonUp(nFlags, point);
}


void COXPopupBarCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if(IsTearOff() && m_bClickedInTearoffBar)
	{
		ASSERT(!IsFloating());
		if(!m_rectTearoffBar.PtInRect(point))
		{
			m_bClickedInTearoffBar=FALSE;
			PostMessage(m_nTearoffMessage);
			return;
		}
	}

	if(IsFloating())
	{
		m_bCheckMousePos=TRUE;
	}

	if(m_ptLastMousePos!=point)
	{
		m_ptLastMousePos=point;
		OnChangeCurrentIndex(GetIndexFromPoint(point));
	}

	CWnd::OnMouseMove(nFlags, point);
}


void COXPopupBarCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	OnSysKeyDown(nChar,nRepCnt,nFlags);
}


void COXPopupBarCtrl::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	UINT nCols=GetNumColumns();

	if(IsFloating() && (nChar==VK_DOWN || nChar==VK_UP || nChar==VK_RIGHT || 
		nChar==VK_TAB || nChar==VK_RETURN || nChar==VK_SPACE))
	{
		m_bCheckMousePos=FALSE;
	}

	if(nChar==VK_DOWN) 
	{
		if(m_nCurrentIndex==ID_POPUPBAR_INDEX_NONE || 
			m_nCurrentIndex==GetLastButtonIndex() || 
			((m_nCurrentIndex+nCols)>GetLastButtonIndex() && 
			GetLastButtonIndex()==m_nButtons))
		{
			OnChangeCurrentIndex(GetFirstButtonIndex());
		}
		else if(m_nCurrentIndex==0)
		{
			OnChangeCurrentIndex(m_nCurrentIndex+1);
		}
		else if(m_nCurrentIndex+nCols>=GetLastButtonIndex() && 
			GetLastButtonIndex()!=m_nButtons)
		{
			OnChangeCurrentIndex(GetLastButtonIndex());
		}
		else
		{
			OnChangeCurrentIndex(m_nCurrentIndex+nCols);
		}
	}

	if(nChar==VK_UP) 
	{
		if(m_nCurrentIndex==ID_POPUPBAR_INDEX_NONE || 
			m_nCurrentIndex==GetFirstButtonIndex() || 
			(m_nCurrentIndex<(GetFirstButtonIndex()+nCols) && 
			GetFirstButtonIndex()!=0))
		{
			OnChangeCurrentIndex(GetLastButtonIndex());
		}
		else if(m_nCurrentIndex==m_nButtons+1)
		{
			OnChangeCurrentIndex(m_nCurrentIndex-1);
		}
		else if(m_nCurrentIndex<=GetFirstButtonIndex()+nCols && 
			GetFirstButtonIndex()==0)
		{
			OnChangeCurrentIndex(GetFirstButtonIndex());
		}
		else
		{
			OnChangeCurrentIndex(m_nCurrentIndex-nCols);
		}
	}

	if(nChar==VK_RIGHT) 
	{
		if(m_nCurrentIndex==ID_POPUPBAR_INDEX_NONE || 
			m_nCurrentIndex==GetLastButtonIndex())
		{
			OnChangeCurrentIndex(GetFirstButtonIndex());
		}
		else
		{
			OnChangeCurrentIndex(m_nCurrentIndex+1);
		}
	}

	if(nChar==VK_LEFT) 
	{
		if(m_nCurrentIndex==ID_POPUPBAR_INDEX_NONE || 
			m_nCurrentIndex==GetFirstButtonIndex())
		{
			OnChangeCurrentIndex(GetLastButtonIndex());
		}
		else
		{
			OnChangeCurrentIndex(m_nCurrentIndex-1);
		}
	}

	if(nChar==VK_TAB) 
	{
		if(GetKeyState(VK_SHIFT) & 0x8000)
		{

			if(m_nCurrentIndex==ID_POPUPBAR_INDEX_NONE || 
				GetFirstButtonIndex()==m_nCurrentIndex || 
				(m_nCurrentIndex>0 && m_nCurrentIndex<=m_nButtons && 
				GetFirstButtonIndex()!=0))
			{
				if(GetLastButtonIndex()==m_nButtons+1)
				{
					OnChangeCurrentIndex(GetLastButtonIndex());
				}
				else
				{
					OnChangeCurrentIndex(1);
				}
			}
			else if(m_nCurrentIndex==m_nButtons+1)
			{
				OnChangeCurrentIndex(1);
			}
			else
			{
				OnChangeCurrentIndex(GetFirstButtonIndex());
			}
		}
		else
		{
			if(m_nCurrentIndex==ID_POPUPBAR_INDEX_NONE || 
				GetLastButtonIndex()==m_nCurrentIndex || 
				(m_nCurrentIndex>0 && m_nCurrentIndex<=m_nButtons && 
				GetLastButtonIndex()==m_nButtons))
			{
				OnChangeCurrentIndex(GetFirstButtonIndex());
			}
			else if(m_nCurrentIndex==0)
			{
				OnChangeCurrentIndex(m_nCurrentIndex+1);
			}
			else
			{
				OnChangeCurrentIndex(GetLastButtonIndex());
			}
		}
	}

	if(nChar==VK_RETURN)
	{
		if(m_nCurrentIndex!=ID_POPUPBAR_INDEX_NONE)
		{
			if(m_nCurrentIndex==m_nButtons+1)
			{
				// CustomButton is about to be activated
				//
				SendNotification(m_nCustomButtonPicked);
			}
			else
			{
				OnChangeSelectedIndex(m_nCurrentIndex);
				if(m_nSelectedIndex==0)
				{
					// DefaultButton is about to be activated
					//
					ASSERT(IsDefaultButton());
					m_dwSelected=m_dwDefault;
				}
				else
				{
					ASSERT((int)m_nButtons<=m_arrData.GetSize());
					m_dwSelected=m_arrData[m_nSelectedIndex-1];
				}
				SendNotification(m_nOkMessage);
			}
		}
		else
		{
			if(!IsFloating())
				PostMessage(m_nCancelMessage);
		}
	}

	if(nChar==VK_SPACE)
	{
		if(!m_bMouseButtonPressed && m_nCurrentIndex!=ID_POPUPBAR_INDEX_NONE)
		{
			m_bMouseButtonPressed=TRUE;
			RedrawElement(m_nCurrentIndex);
		}
	}

	CWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}


void COXPopupBarCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	OnSysKeyUp(nChar,nRepCnt,nFlags);
}


void COXPopupBarCtrl::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar==VK_ESCAPE) 
	{
		SendNotification(m_nCancelMessage);
	}

	if(nChar==VK_SPACE)
	{
		m_bMouseButtonPressed=FALSE;

		if(m_nCurrentIndex!=ID_POPUPBAR_INDEX_NONE)
		{
			if(m_nCurrentIndex==m_nButtons+1)
			{
				// CustomButton is about to be activated
				//
				SendNotification(m_nCustomButtonPicked);
			}
			else
			{
				OnChangeSelectedIndex(m_nCurrentIndex);
				if(m_nSelectedIndex==0)
				{
					// DefaultButton is about to be activated
					//
					ASSERT(IsDefaultButton());
					m_dwSelected=m_dwDefault;
				}
				else
				{
					ASSERT((int)m_nButtons<=m_arrData.GetSize());
					m_dwSelected=m_arrData[m_nSelectedIndex-1];
				}
				SendNotification(m_nOkMessage);
			}
		}
		else
		{
			if(!IsFloating())
				PostMessage(m_nCancelMessage);
		}
	}

	CWnd::OnSysKeyUp(nChar, nRepCnt, nFlags);
}


void COXPopupBarCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	// realize custom palette if any set
	CPalette* pOldPalette=NULL;
	if((HPALETTE)m_CustomPalette!=NULL)
	{
		pOldPalette=dc.SelectPalette(&m_CustomPalette,FALSE);
		dc.RealizePalette();
	}

	// Draw the background
	DrawBackground(&dc);

	// Draw common buttons
	for(int nIndex=0; nIndex<(int)m_nButtons; nIndex++)
	{
		DrawButton(&dc, nIndex);
	}

	// Draw Tear off bar
	if(IsTearOff())
	{
		DrawTearoffBar(&dc);
	}

	// Draw default button
	if(IsDefaultButton())
	{
		DrawDefaultButton(&dc);
	}

	// Draw custom button
	if(IsCustomButton())
	{
		DrawCustomButton(&dc);
	}

	if(pOldPalette!=NULL)
	{
		dc.SelectPalette(pOldPalette,FALSE);
	}

	// Do not call CWnd::OnPaint() for painting messages
}


BOOL COXPopupBarCtrl::OnQueryNewPalette() 
{
	Invalidate();    
	return CWnd::OnQueryNewPalette();
}


void COXPopupBarCtrl::OnPaletteChanged(CWnd* pFocusWnd)
{
	CWnd::OnPaletteChanged(pFocusWnd);

	if(pFocusWnd!=this)
	{
		Invalidate();
	}
}


void COXPopupBarCtrl::OnClose() 
{
	if(IsFloating())
		ShowWindow(SW_HIDE);
	else
		CWnd::OnClose();
}


void COXPopupBarCtrl::OnDestroy()
{
	if(m_nCheckMouseTimer!=0)
		KillTimer(m_nCheckMouseTimer);

	CWnd::OnDestroy();
}	


void COXPopupBarCtrl::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	UNREFERENCED_PARAMETER(pWndOther);
	if(nState!=WA_INACTIVE && !bMinimized)
	{
		SetFocus();
	}
}


void COXPopupBarCtrl::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==(UINT)m_nCheckMouseTimer)
	{
		if(m_bCheckMousePos && IsFloating())
		{
			CPoint point;
			::GetCursorPos(&point);
			ScreenToClient(&point);
			int nIndex=GetIndexFromPoint(point);
			if(nIndex==ID_POPUPBAR_INDEX_NONE)
				OnChangeCurrentIndex(nIndex);
		}
		return;
	}

	CWnd::OnTimer(nIDEvent);
}


BOOL COXPopupBarCtrl::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

//////////////////////////////////////////////////////////////

void COXPopupBarCtrl::OnMouseButtonDown(CPoint point) 
{
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(rect);

	CPoint ptScreen=point;
	ClientToScreen(&ptScreen);
	if(rect.PtInRect(ptScreen))
	{
		m_bMouseButtonPressed=TRUE;
		RedrawElement(GetIndexFromPoint(point));
	}
	else
	{
		// mouse is out of control area
		if(!IsFloating())
			PostMessage(m_nCancelMessage);
	}

	// Check if the tear off bar was clicked
	ScreenToClient(&ptScreen);
	if(IsTearOff() && m_rectTearoffBar.PtInRect(ptScreen))
	{
		m_bClickedInTearoffBar=TRUE;
	}
}


void COXPopupBarCtrl::OnMouseButtonUp(CPoint point) 
{
	UNREFERENCED_PARAMETER(point);

	m_bClickedInTearoffBar=FALSE;

	if(m_bMouseButtonPressed && m_nCurrentIndex!=ID_POPUPBAR_INDEX_NONE)
	{
		if(m_nCurrentIndex==m_nButtons+1)
		{
			// mouse is over CustomButton
			//
			SendNotification(m_nCustomButtonPicked);
		}
		else
		{
			OnChangeSelectedIndex(m_nCurrentIndex);
			if(m_nSelectedIndex==0)
			{
				// mouse is over DefaultButton
				//
				ASSERT(IsDefaultButton());
				m_dwSelected=m_dwDefault;
			}
			else
			{
				ASSERT((int)m_nButtons<=m_arrData.GetSize());
				m_dwSelected=m_arrData[m_nSelectedIndex-1];
			}
			SendNotification(m_nOkMessage);
		}
	}
	else
	{
		if(m_bMouseButtonPressed && !IsFloating())
		{
			PostMessage(m_nCancelMessage);
		}
	}
	m_bMouseButtonPressed=FALSE;
}


BOOL COXPopupBarCtrl::OnCustomButton()
{
	ASSERT(IsCustomButton());

	return FALSE;
}


void COXPopupBarCtrl::OnChangeCurrentIndex(UINT newCurrentIndex)
{
	if(newCurrentIndex!=m_nCurrentIndex)
	{
		UINT oldCurrentIndex=m_nCurrentIndex;
		m_nCurrentIndex=newCurrentIndex;
		RedrawElement(oldCurrentIndex);
		RedrawElement(m_nCurrentIndex);
	}
}


void COXPopupBarCtrl::OnChangeSelectedIndex(UINT newSelectedIndex)
{
	if(newSelectedIndex!=m_nSelectedIndex)
	{
		UINT oldSelectedIndex=m_nSelectedIndex;
		m_nSelectedIndex=newSelectedIndex;
		RedrawElement(oldSelectedIndex);
		RedrawElement(m_nSelectedIndex);
	}
}


////////////////////////////////////////////////////////////////

// sets array of data of common buttons
BOOL COXPopupBarCtrl::SetButtonDataTable(DWORD* arrData, UINT nElements)
{
	if(IsFloating())
		SendNotification(m_nCancelMessage);

	if(nElements>0)
	{
		ASSERT(arrData);
	}

	m_arrData.RemoveAll();
	for(int nIndex=0; nIndex<(int)nElements; nIndex++)
	{
		m_arrData.Add(arrData[nIndex]);
	}
	return TRUE;
}


// set array of data of common buttons
BOOL COXPopupBarCtrl::SetButtonDataTable(ButtonDataTable& arrData)
{
	if(IsFloating())
		SendNotification(m_nCancelMessage);

	UINT_PTR nElements=arrData.GetSize();

	m_arrData.RemoveAll();
	for(int nIndex=0; nIndex<(int)nElements; nIndex++)
	{
		m_arrData.Add(arrData[nIndex]);
	}
	return TRUE;
}


// sets array of tooltips associated with common buttons
BOOL COXPopupBarCtrl::SetToolTipTextTable(ButtonToolTipTable* arrToolTipText, 
										  UINT nElements)
{
	if(nElements>0)
	{
		ASSERT(arrToolTipText);
	}

	m_arrToolTipText.RemoveAll();
	CString sToolTipText;
	for(int nIndex=0; nIndex<(int)nElements; nIndex++)
	{
		sToolTipText=arrToolTipText[nIndex].pszToolTipText;
		m_arrToolTipText.SetAt(arrToolTipText[nIndex].dwData,sToolTipText);
	}
	return TRUE;
}


// sets array of tooltips associated with common buttons
BOOL COXPopupBarCtrl::SetToolTipTextIDTable(ButtonToolTipIDTable* arrToolTipText, 
											UINT nElements)
{
	if(nElements>0)
	{
		ASSERT(arrToolTipText);
	}

	m_arrToolTipText.RemoveAll();
	CString sToolTipText;
	for(int nIndex=0; nIndex<(int)nElements; nIndex++)
	{
		sToolTipText.LoadString(arrToolTipText[nIndex].nID);
		m_arrToolTipText.SetAt(arrToolTipText[nIndex].dwData, sToolTipText);
	}
	return TRUE;
}


// sets array of tooltips associated with common buttons
BOOL COXPopupBarCtrl::SetToolTipTextTable(ButtonToolTipMap& arrToolTipText)
{
	UINT_PTR nElements=arrToolTipText.GetCount();

	m_arrToolTipText.RemoveAll();

	POSITION pos=arrToolTipText.GetStartPosition();
	DWORD dwData;
	CString sToolTipText;
	BOOL bResult=TRUE;
	for(int nIndex=0; nIndex<(int)nElements; nIndex++)
	{
		if(pos==NULL)
		{
			bResult=FALSE;
			m_arrToolTipText.RemoveAll();
			break;
		}
		arrToolTipText.GetNextAssoc(pos,dwData,sToolTipText);
		m_arrToolTipText.SetAt(dwData,sToolTipText);
	}
	return bResult;
}


BOOL COXPopupBarCtrl::PopulateToolTip()
{
	ASSERT(::IsWindow(m_ctlToolTip.GetSafeHwnd()));

	ASSERT(m_nButtons>0 && m_nButtons<=ID_POPUPBAR_MAX_BUTTONS);

	ASSERT(m_arrButtonRect.GetSize()==(int)m_nButtons);
	ASSERT((int)m_nButtons<=m_arrData.GetSize());

	// delete all previously created tools
	int nTools=m_ctlToolTip.GetToolCount();
	int nIndex=0;
	for(nIndex=0; nIndex<nTools; nIndex++)
	{
		m_ctlToolTip.DelTool(this,nIndex+1);
	}

	CString sToolTipText;
	// Add a tool for each Color Button
	for(nIndex=0; nIndex<(int)m_nButtons; nIndex++)
	{
		if(m_arrToolTipText.Lookup(m_arrData[nIndex],sToolTipText))
		{
			m_ctlToolTip.AddTool(this,sToolTipText,m_arrButtonRect[nIndex],nIndex+1);
		}
	}

	if(IsDefaultButton())
	{
		m_ctlToolTip.AddTool(this,m_sDefaultButtonText,m_rectDefaultButton,nIndex+1);
	}
	if(IsCustomButton())
	{
		m_ctlToolTip.AddTool(this,m_sCustomButtonText,m_rectCustomButton,nIndex+2);
	}
	if(IsTearOff())
	{
		m_ctlToolTip.AddTool(this,m_sTearoffText,m_rectTearoffBar,nIndex+3);
	}

	return TRUE;
}


BOOL COXPopupBarCtrl::ConvertToTearOff()
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(IsTearOff());

	// Remember the location of the window
	CRect rect;
	GetWindowRect(rect);

	// Ensure that the mouse is still over this window, otherwise the ButtonUp
	// message will not get to this window, and when the user clicks up the 
	// mouse will still be dragging this window around. Trust me - it looks 
	// pretty silly.
	DWORD dwMessagePos=::GetMessagePos();
	CPoint ptMouse(GET_X_LPARAM(dwMessagePos),GET_Y_LPARAM(dwMessagePos));
	if (!rect.PtInRect(ptMouse))
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	// track the window
	//

	// initialize tracking state
	m_rectLastTrackBar.SetRectEmpty();
	m_sizeLastTrackBar=CSize(0,0);
	m_bDitherLast=FALSE;

	m_rectTrackBar=rect;

	// lock window update while tracking
	ASSERT(m_pTrackingDC==NULL);
	CWnd* pWnd=CWnd::GetDesktopWindow();
#ifndef _MAC
	if(pWnd->LockWindowUpdate()) 
		m_pTrackingDC=pWnd->GetDCEx(NULL,DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE);
	else
#endif
		m_pTrackingDC=pWnd->GetDCEx(NULL,DCX_WINDOW|DCX_CACHE);
	ASSERT(m_pTrackingDC!=NULL); 

	m_ptLastTrack=ptMouse;
	MoveTrackBar(ptMouse);
	if(!TrackBar())
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	rect=m_rectTrackBar;
	//
	///////////////////////////////////////

	// Destroy this window so we can create a new one 
	DestroyWindow();
	m_ctlToolTip.DestroyWindow();

	m_bFloating=TRUE;

	// Get the background brush
	WNDCLASS wc;
	HBRUSH hBrush = NULL;
	if(GetClassInfo(AfxGetInstanceHandle(),POPUPBAR_CLASSNAME,&wc))
	{
		hBrush=wc.hbrBackground;
	}


	if(!Create(m_pParentWnd,m_nPopupBarStyle,hBrush))
	{
		return FALSE;
	}

	// Place everything in the right position
	CalcWindowSize();

	if(!PopulateToolTip())
	{
		return FALSE;
	}

	m_rect.OffsetRect(-m_rect.TopLeft());
	m_rect.OffsetRect(rect.TopLeft());

	// it's show time!
	SetWindowPos(NULL,m_rect.left,m_rect.top,m_rect.Width(),m_rect.Height(),
		SWP_NOZORDER|SWP_NOACTIVATE);

	RedrawWindow();

	return TRUE;
}


void COXPopupBarCtrl::MoveTrackBar(CPoint pt)
{
	CPoint ptOffset=pt-m_ptLastTrack;
	// offset drag rect to new position
	m_rectTrackBar.OffsetRect(ptOffset);
	m_ptLastTrack = pt;
	// update feedback
	DrawTrackingRect(FALSE);
}


BOOL COXPopupBarCtrl::TrackBar()
{
	// don't handle if capture already set
	if(::GetCapture()!=NULL)
		return FALSE;

	// set capture
	SetCapture();
	ASSERT(this==CWnd::GetCapture());

	BOOL bCancelled=FALSE;
	// get messages until capture lost or cancelled/accepted
	while(CWnd::GetCapture()==this && !bCancelled)
	{
		MSG msg;
#ifndef _MAC
		if(!::GetMessage(&msg,NULL,0,0))
#else
		// don't allow yielding while tracking since we don't have LockWindowUpdate
		if(!PeekMessage(&msg,NULL,0,0,PM_REMOVE|PM_NOYIELD))
			continue;
		if(msg.message==WM_QUIT)
#endif
		{
			AfxPostQuitMessage(PtrToInt(msg.wParam));
			break;
		}

		switch(msg.message)
		{
		case WM_LBUTTONUP:		// drag finished 
			EndTrackBar();
			return TRUE;

		case WM_MOUSEMOVE:
			MoveTrackBar(msg.pt);
			break;

		case WM_KEYDOWN:
			if(msg.wParam==VK_ESCAPE)
				bCancelled=TRUE;
			break;

		case WM_RBUTTONDOWN:
			bCancelled=TRUE;
			break;

			// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

	CancelTrackBar();

	return FALSE;
}


void COXPopupBarCtrl::EndTrackBar()
{
	CancelTrackBar();
}


void COXPopupBarCtrl::CancelTrackBar()
{
	DrawTrackingRect(TRUE);    // gets rid of focus rect
	ReleaseCapture();

	CWnd* pWnd = CWnd::GetDesktopWindow();
#ifndef _MAC
	pWnd->UnlockWindowUpdate();
#endif
	if(m_pTrackingDC!=NULL)
	{
		pWnd->ReleaseDC(m_pTrackingDC);
		m_pTrackingDC=NULL;
	}
}


const int CX_TRACKBORDER=3;
const int CY_TRACKBORDER=3;

void COXPopupBarCtrl::DrawTrackingRect(BOOL bRemoveRect)
{
	ASSERT(m_pTrackingDC!=NULL);

	// default to thin frame
	CSize size(CX_TRACKBORDER, CY_TRACKBORDER);

	// determine new rect and size
	CRect rect;
	CBrush* pWhiteBrush=CBrush::FromHandle((HBRUSH)::GetStockObject(WHITE_BRUSH));
	CBrush* pDitherBrush=CDC::GetHalftoneBrush();
	CBrush* pBrush=pWhiteBrush;

	rect=m_rectTrackBar;
	if(bRemoveRect)
	{
		size.cx=0;
		size.cy=0;
	}

	// draw it and remember last size
	m_pTrackingDC->DrawDragRect(&rect,size,&m_rectLastTrackBar,
		m_sizeLastTrackBar,pBrush,(m_bDitherLast ? pDitherBrush : pWhiteBrush));
	m_rectLastTrackBar=rect;
	m_sizeLastTrackBar = size;
	m_bDitherLast=(pBrush==pDitherBrush);
}


// if the menu is floating then the "Pick" message loop will have been exited.
// We need to let the parent know what is happening. If the menu is not floating
// then we just send messages back to this window so the "Pick" loop knows
// what is going on.
void COXPopupBarCtrl::SendNotification(UINT nMessage)
{
	if(IsFloating())
	{
		BOOL bResult=TRUE;
		if(nMessage==m_nCustomButtonPicked)
			bResult=OnCustomButton();
		else if(nMessage==m_nCancelMessage)
		{
			m_bCheckMousePos=FALSE;
			ShowWindow(SW_HIDE);
			bResult=FALSE;
		}

		if(bResult)
		{
			m_pParentWnd->PostMessage(OXPBN_SELCHANGED,NULL,(LPARAM)GetSafeHwnd());
		}
	}
	else
	{
		PostMessage(nMessage);
	}
}

//////////////////////////////////////////////////////////////

// calculate size of window and location of default and 
// custom buttons within the control
BOOL COXPopupBarCtrl::CalcWindowSize()
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(m_nRows<=m_nButtons);

	m_rect.SetRectEmpty();
	m_rectTearoffBar.SetRectEmpty();
	m_rectDefaultButton.SetRectEmpty();
	m_rectCustomButton.SetRectEmpty();
	m_arrButtonRect.RemoveAll();

	// number of columns
	int nCols=GetNumColumns();
	// sum the size of window edges, all common buttons and margins
	m_rect.right=2*::GetSystemMetrics(SM_CXDLGFRAME)+
		m_rectEdgeMargin.left+m_rectEdgeMargin.right+
		nCols*(m_sizeButton.cx+m_rectButtonMargin.left+
		m_rectButtonMargin.right);
	// sum the size of window edges, all common buttons and margins
	m_rect.bottom=2*::GetSystemMetrics(SM_CYDLGFRAME)+
		m_rectEdgeMargin.top+m_rectEdgeMargin.bottom+
		m_nRows*(m_sizeButton.cy+m_rectButtonMargin.top+
		m_rectButtonMargin.bottom);

	if (IsFloating())
		m_rect.bottom +=::GetSystemMetrics(SM_CYSMCAPTION);

	if((HFONT)m_font==NULL)
	{
		// Create the font
		NONCLIENTMETRICS ncm;
		ncm.cbSize = sizeof(NONCLIENTMETRICS);
		VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 
			sizeof(NONCLIENTMETRICS), &ncm, 0));
		VERIFY(m_font.CreateFontIndirect(&ncm.lfMessageFont));
	}


	if(IsTearOff())
	{
		m_rectTearoffBar.left  = m_rectEdgeMargin.left + m_rectTearoffBarMargin.left;
		m_rectTearoffBar.right = m_rectTearoffBar.left + 
			m_rect.right - (2*::GetSystemMetrics(SM_CXDLGFRAME) +
			m_rectEdgeMargin.left + m_rectTearoffBarMargin.left +
			m_rectEdgeMargin.right + m_rectTearoffBarMargin.right);

		m_rectTearoffBar.top    = m_rectEdgeMargin.top + m_rectTearoffBarMargin.top;
		m_rectTearoffBar.bottom = m_rectTearoffBar.top + 6;

		m_rect.bottom += m_rectTearoffBar.Height() +
			m_rectTearoffBarMargin.top + m_rectTearoffBarMargin.bottom;
	}


	CClientDC dc(this);
	if(IsDefaultButton())
	{
		m_rectDefaultButton.left=m_rectEdgeMargin.left+m_rectDefaultButtonMargin.left;
		m_rectDefaultButton.right=m_rectDefaultButton.left+
			m_rect.right-(2*::GetSystemMetrics(SM_CXDLGFRAME)+
			m_rectEdgeMargin.left+m_rectDefaultButtonMargin.left+
			m_rectEdgeMargin.right+m_rectDefaultButtonMargin.right);
		m_rectDefaultButton.top=m_rectEdgeMargin.top+m_rectDefaultButtonMargin.top;

		// Get the size of the custom text
		CFont* pOldFont=(CFont*)dc.SelectObject(&m_font);
		CRect rect(0, 0, 0, 0);
		dc.DrawText(m_sDefaultButtonText,&rect,DT_CALCRECT);
		rect.InflateRect(4,4);
		dc.SelectObject(pOldFont);

		m_rectDefaultButton.bottom=m_rectDefaultButton.top+rect.Height();

		if(IsTearOff())
		{
			int nShiftDown=m_rectTearoffBar.Height()+
				m_rectTearoffBarMargin.top+m_rectTearoffBarMargin.bottom;
			m_rectDefaultButton.OffsetRect(0, nShiftDown);
		}

		m_rect.bottom+=m_rectDefaultButton.Height()+
			m_rectDefaultButtonMargin.top+m_rectDefaultButtonMargin.bottom;
	}

	if(IsCustomButton())
	{
		m_rectCustomButton.left=m_rectEdgeMargin.left+m_rectCustomButtonMargin.left;
		m_rectCustomButton.right=m_rectCustomButton.left+
			m_rect.right-(2*::GetSystemMetrics(SM_CXDLGFRAME)+
			m_rectEdgeMargin.left+m_rectCustomButtonMargin.left+
			m_rectEdgeMargin.right+m_rectCustomButtonMargin.right);
		m_rectCustomButton.top=m_rect.bottom-(2*::GetSystemMetrics(SM_CYDLGFRAME)+
			m_rectEdgeMargin.top)+m_rectCustomButtonMargin.top;

		if(IsFloating())
			m_rectCustomButton.top-=::GetSystemMetrics(SM_CYSMCAPTION);

		// Get the size of the custom text
		CFont* pOldFont=(CFont*)dc.SelectObject(&m_font);
		CRect rect(0, 0, 0, 0);
		dc.DrawText(m_sCustomButtonText,&rect,DT_CALCRECT);
		rect.InflateRect(4,4);
		dc.SelectObject(pOldFont);

		m_rectCustomButton.bottom=m_rectCustomButton.top+rect.Height();

		m_rect.bottom+=m_rectCustomButton.Height()+
			m_rectCustomButtonMargin.top+m_rectCustomButtonMargin.bottom;
	}



	CSize sizeIniShift;
	sizeIniShift.cx=m_rectEdgeMargin.left+m_rectButtonMargin.left;
	sizeIniShift.cy=m_rectEdgeMargin.top+m_rectButtonMargin.top;
	if(IsDefaultButton())
	{
		sizeIniShift.cy+=m_rectDefaultButtonMargin.top+
			m_rectDefaultButtonMargin.bottom+m_rectDefaultButton.Height();
	}
	if(IsTearOff())
	{
		sizeIniShift.cy+=m_rectTearoffBarMargin.top+
			m_rectTearoffBarMargin.bottom+m_rectTearoffBar.Height();
	}

	CRect rect;
	for(int i=0; i<(int)m_nRows; i++)
	{
		for(int j=0; j<nCols; j++)
		{
			if(i*nCols+j<(int)m_nButtons)
			{
				rect.left=sizeIniShift.cx+(m_rectButtonMargin.left+
					m_sizeButton.cx+m_rectButtonMargin.right)*j;
				rect.right=rect.left+m_sizeButton.cx;
				rect.top=sizeIniShift.cy+(m_rectButtonMargin.top+
					m_sizeButton.cy+m_rectButtonMargin.bottom)*i;
				rect.bottom=rect.top+m_sizeButton.cy;

				m_arrButtonRect.Add(rect);
			}
		}
	}

	return TRUE;
}


// user for sure want to control displaying of the control 
BOOL COXPopupBarCtrl::AdjustWindowPosition(UINT nAlignment, CRect* pParentRect, 
										   CSize sizeMargin)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(nAlignment==ID_POPUPBAR_ALIGNLEFT || 
		nAlignment==ID_POPUPBAR_ALIGNRIGHT || 
		nAlignment==ID_POPUPBAR_ALIGNTOP || 
		nAlignment==ID_POPUPBAR_ALIGNBOTTOM ||
		nAlignment==ID_POPUPBAR_ALIGNLEFTBOTTOM || 
		nAlignment==ID_POPUPBAR_ALIGNRIGHTBOTTOM || 
		nAlignment==ID_POPUPBAR_ALIGNTOPRIGHT || 
		nAlignment==ID_POPUPBAR_ALIGNBOTTOMRIGHT);

	BOOL bTopMostParent=(AfxGetMainWnd()!=NULL &&
		(AfxGetMainWnd()->GetExStyle() & WS_EX_TOPMOST)==WS_EX_TOPMOST);
	CRect rect;
	// check if item rectangle fits into the screen
#if(defined(GetMonitorInfo) && defined(MonitorFromPoint))
	// if we use Win 98/NT5 we have to take into account multiple monitors
	OSVERSIONINFO osvi={ sizeof(OSVERSIONINFO) };
	VERIFY(::GetVersionEx(&osvi)!=0);
	if(osvi.dwMajorVersion>4 || 
		(osvi.dwMajorVersion==4 && osvi.dwMinorVersion>0))
	{
		CPoint pt;
		::GetCursorPos(&pt);
		HMONITOR hMonitor=::MonitorFromPoint(pt,MONITOR_DEFAULTTONEAREST);
		ASSERT(hMonitor!=NULL);
		MONITORINFO monitorInfo={ sizeof(MONITORINFO) };
		VERIFY(::GetMonitorInfo(hMonitor,&monitorInfo));
		if(bTopMostParent)
		{
			rect=monitorInfo.rcMonitor;
		}
		else
		{
			rect=monitorInfo.rcWork;
		}
	}
	else
	{
#endif
		if(bTopMostParent)
		{
			CWnd::GetDesktopWindow()->GetWindowRect(rect);
		}
		else
		{
			::SystemParametersInfo(SPI_GETWORKAREA,NULL,&rect,NULL);
		}
#if(defined(GetMonitorInfo) && defined(MonitorFromPoint))
	}
#endif

	int nScreenWidth=rect.Width();
	int nScreenHeight=rect.Height();

	if(m_rect.Width()>nScreenWidth || m_rect.Height()>nScreenHeight)
	{
		return FALSE;
	}

	CSize sizeOffset;

	CRect rectParent;
	if(pParentRect==NULL)
	{
		m_pParentWnd->GetWindowRect(rectParent);
	}
	else
	{
		rectParent=*pParentRect;
	}

	switch(nAlignment)
	{
	case ID_POPUPBAR_ALIGNLEFT:
		{
			sizeOffset.cx=rectParent.left-sizeMargin.cx-m_rect.Width();
			sizeOffset.cy=rectParent.top+sizeMargin.cy;
#if(WINVER < 0x0500)
			if(sizeOffset.cx<0)
			{
				sizeOffset.cx=rectParent.right+sizeMargin.cx;
				if(sizeOffset.cx+m_rect.Width()>nScreenWidth)
				{
					return FALSE;
				}
			}
#endif
			m_rect.OffsetRect(sizeOffset);
			break;
		}
	case ID_POPUPBAR_ALIGNLEFTBOTTOM:
		{
			sizeOffset.cx=rectParent.left-sizeMargin.cx-m_rect.Width();
			sizeOffset.cy=rectParent.bottom-sizeMargin.cy-m_rect.Height();
#if(WINVER < 0x0500)
			if(sizeOffset.cx<0)
			{
				sizeOffset.cx=rectParent.right+sizeMargin.cx;
				if(sizeOffset.cx+m_rect.Width()>nScreenWidth)
				{
					return FALSE;
				}
			}
#endif
			m_rect.OffsetRect(sizeOffset);
			break;
		}
	case ID_POPUPBAR_ALIGNRIGHT:
		{
			sizeOffset.cx=rectParent.right+sizeMargin.cx;
			sizeOffset.cy=rectParent.top+sizeMargin.cy;
#if(WINVER < 0x0500)
			if(sizeOffset.cx+m_rect.Width()>nScreenWidth)
			{
				sizeOffset.cx=rectParent.left-sizeMargin.cx-m_rect.Width();
				if(sizeOffset.cx<0)
				{
					return FALSE;
				}
			}
#endif
			m_rect.OffsetRect(sizeOffset);
			break;
		}
	case ID_POPUPBAR_ALIGNRIGHTBOTTOM:
		{
			sizeOffset.cx=rectParent.right+sizeMargin.cx;
			sizeOffset.cy=rectParent.bottom-sizeMargin.cy-m_rect.Height();
#if(WINVER < 0x0500)
			if(sizeOffset.cx+m_rect.Width()>nScreenWidth)
			{
				sizeOffset.cx=rectParent.left-sizeMargin.cx-m_rect.Width();
				if(sizeOffset.cx<0)
				{
					return FALSE;
				}
			}
#endif
			m_rect.OffsetRect(sizeOffset);
			break;
		}
	case ID_POPUPBAR_ALIGNTOP:
		{
			sizeOffset.cx=rectParent.left+sizeMargin.cx;
			sizeOffset.cy=rectParent.top-sizeMargin.cy-m_rect.Height();
#if(WINVER < 0x0500)
			if(sizeOffset.cy<0)
			{
				sizeOffset.cy=rectParent.bottom+sizeMargin.cy;
				if(sizeOffset.cy+m_rect.Height()>nScreenHeight)
				{
					return FALSE;
				}
			}
#endif
			m_rect.OffsetRect(sizeOffset);
			break;
		}
	case ID_POPUPBAR_ALIGNTOPRIGHT:
		{
			sizeOffset.cx=rectParent.right-sizeMargin.cx-m_rect.Width();
			sizeOffset.cy=rectParent.top-sizeMargin.cy-m_rect.Height();
#if(WINVER < 0x0500)
			if(sizeOffset.cy<0)
			{
				sizeOffset.cy=rectParent.bottom+sizeMargin.cy;
				if(sizeOffset.cy+m_rect.Height()>nScreenHeight)
				{
					return FALSE;
				}
			}
#endif
			m_rect.OffsetRect(sizeOffset);
			break;
		}
	case ID_POPUPBAR_ALIGNBOTTOM:
		{
			sizeOffset.cx=rectParent.left+sizeMargin.cx;
			sizeOffset.cy=rectParent.bottom+sizeMargin.cy;
#if(WINVER < 0x0500)
			if(sizeOffset.cy+m_rect.Height()>nScreenHeight)
			{
				sizeOffset.cy=rectParent.top-sizeMargin.cy-m_rect.Height();
				if(sizeOffset.cy<0)
				{
					return FALSE;
				}
			}
#endif
			m_rect.OffsetRect(sizeOffset);
			break;
		}
	case ID_POPUPBAR_ALIGNBOTTOMRIGHT:
		{
			sizeOffset.cx=rectParent.right-sizeMargin.cx-m_rect.Width();
			sizeOffset.cy=rectParent.bottom+sizeMargin.cy;
#if(WINVER < 0x0500)
			if(sizeOffset.cy+m_rect.Height()>nScreenHeight)
			{
				sizeOffset.cy=rectParent.top-sizeMargin.cy-m_rect.Height();
				if(sizeOffset.cy<0)
				{
					return FALSE;
				}
			}
#endif
			m_rect.OffsetRect(sizeOffset);
			break;
		}
	}

	sizeOffset.cx=0;
	sizeOffset.cy=0;

#if(WINVER < 0x0500)

	if(m_rect.left<0)
	{
		sizeOffset.cx=-m_rect.left;
	}
	else if(m_rect.right>nScreenWidth)
	{
		sizeOffset.cx=nScreenWidth-m_rect.right;
	}
	if(m_rect.top<0)
	{
		sizeOffset.cy=-m_rect.top;
	}
	else if(m_rect.bottom>nScreenHeight)
	{
		sizeOffset.cy=nScreenHeight-m_rect.bottom;
	}
	m_rect.OffsetRect(sizeOffset);
#endif
	return TRUE;
}


// gets index of button base on color
UINT COXPopupBarCtrl::GetIndexFromData(DWORD dwData)
{
	ASSERT(m_nButtons>0 && m_nButtons<=ID_POPUPBAR_MAX_BUTTONS);
	UINT_PTR nElements=m_arrData.GetSize();
	ASSERT(m_nButtons<=nElements);

	if(dwData==ID_POPUPBAR_DATA_NONE)
	{
		return ID_POPUPBAR_INDEX_NONE;
	}
	if(IsDefaultButton() && m_dwDefault==dwData)
	{
		return 0;
	}
	for(int nIndex=0; nIndex<(int)m_nButtons; nIndex++)
	{
		if(m_arrData[nIndex]==dwData)
		{
			return nIndex+1;
		}
	}
	if(IsCustomButton())
	{
		return m_nButtons+1;
	}

	return ID_POPUPBAR_INDEX_NONE;
}


// gets index of button base on point
UINT COXPopupBarCtrl::GetIndexFromPoint(CPoint point)
{
	ASSERT(m_nButtons>0 && m_nButtons<=ID_POPUPBAR_MAX_BUTTONS);
	UINT_PTR nElements=m_arrButtonRect.GetSize();
	ASSERT(m_nButtons==nElements);

	if(IsDefaultButton() && m_rectDefaultButton.PtInRect(point))
	{
		return 0;
	}
	for(int nIndex=0; nIndex<(int)m_nButtons; nIndex++)
	{
		if(m_arrButtonRect[nIndex].PtInRect(point))
		{
			return nIndex+1;
		}
	}
	if(IsCustomButton() && m_rectCustomButton.PtInRect(point))
	{
		return m_nButtons+1;
	}

	return ID_POPUPBAR_INDEX_NONE;
}


// returns number of columns of common buttons in the control
UINT COXPopupBarCtrl::GetNumColumns() const
{
	ASSERT(m_nButtons>0 && m_nButtons<=ID_POPUPBAR_MAX_BUTTONS);
	ASSERT(m_nRows>0 && m_nRows<=m_nButtons);

	return m_nButtons/m_nRows+(m_nButtons%m_nRows>0 ? 1 : 0);
}


// returns index of the first button on the control
UINT COXPopupBarCtrl::GetFirstButtonIndex()
{
	ASSERT(m_nButtons>0 && m_nButtons<=ID_POPUPBAR_MAX_BUTTONS);
	ASSERT(m_nRows>0 && m_nRows<=m_nButtons);

	return IsDefaultButton() ? 0 : 1;
}

// returns index of the last button on the control
UINT COXPopupBarCtrl::GetLastButtonIndex()
{
	ASSERT(m_nButtons>0 && m_nButtons<=ID_POPUPBAR_MAX_BUTTONS);
	ASSERT(m_nRows>0 && m_nRows<=m_nButtons);

	return IsCustomButton() ? m_nButtons+1 : m_nButtons;
}


/////////////////////////////////////////////////////////////////
void COXPopupBarCtrl::RedrawElement(UINT nIndex)
{
	if(nIndex==0)
	{
		// default button
		ASSERT(IsDefaultButton());
		InvalidateRect(m_rectDefaultButton);
	} 
	else if(nIndex==m_nButtons+1)
	{
		// custom button
		ASSERT(IsCustomButton());
		InvalidateRect(m_rectCustomButton);
	} 
	else if(nIndex>=1 && nIndex<=m_nButtons)
	{
		// common buttons
		ASSERT((int)m_nButtons==m_arrButtonRect.GetSize());
		InvalidateRect(m_arrButtonRect[nIndex-1]);
	}
}


// draw color button
void COXPopupBarCtrl::DrawButton(CDC* pDC, UINT nIndex)
{
	ASSERT(m_nButtons>0 && m_nButtons<=ID_POPUPBAR_MAX_BUTTONS);
	ASSERT((int)m_nButtons==m_arrButtonRect.GetSize());
	ASSERT((int)m_nButtons<=m_arrData.GetSize());
	ASSERT(nIndex<=m_nButtons);

	CRect rect=m_arrButtonRect[nIndex];

	DrawFrameRect(pDC,rect,nIndex+1);

}


// Draw Tear off bar
void COXPopupBarCtrl::DrawTearoffBar(CDC* pDC)
{
	ASSERT(IsTearOff());

	DWORD clrBar=::GetSysColor(COLOR_HIGHLIGHT);
	CRect rect=m_rectTearoffBar;

	CBrush brushBar(clrBar);
	pDC->FillRect(rect, &brushBar);
}


// draw default button
void COXPopupBarCtrl::DrawDefaultButton(CDC* pDC)
{
	ASSERT(IsDefaultButton());

	DWORD clrShadow=::GetSysColor(COLOR_BTNSHADOW);
	CRect rect=m_rectDefaultButton;

	DrawFrameRect(pDC,rect,0);

	CBrush brushFrame(clrShadow);
	rect.DeflateRect(2,2);
	pDC->FrameRect(&rect,&brushFrame);
	rect.DeflateRect(1,1);

	CFont* pOldFont=(CFont*)pDC->SelectObject(&m_font);
	int oldBkMode=pDC->SetBkMode(TRANSPARENT);					
	pDC->DrawText(m_sDefaultButtonText,&rect,DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS);
	pDC->SetBkMode(oldBkMode);					
	pDC->SelectObject(pOldFont);
}


// draw custom button
void COXPopupBarCtrl::DrawCustomButton(CDC* pDC)
{
	ASSERT(IsCustomButton());

	CRect rect=m_rectCustomButton;

	DrawFrameRect(pDC,rect,m_nButtons+1);

	rect.DeflateRect(3,3);
	CFont* pOldFont=(CFont*)pDC->SelectObject(&m_font);
	int oldBkMode=pDC->SetBkMode(TRANSPARENT);					
	pDC->DrawText(m_sCustomButtonText,&rect,DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS);
	pDC->SetBkMode(oldBkMode);					
	pDC->SelectObject(pOldFont);
}


// draw frame rect around button
void COXPopupBarCtrl::DrawFrameRect(CDC* pDC, CRect& rect, UINT nIndex)
{
	DWORD clrHilight=::GetSysColor(COLOR_BTNHILIGHT);
	DWORD clrShadow=::GetSysColor(COLOR_BTNSHADOW);
	DWORD clrLight=::GetSysColor(COLOR_3DLIGHT);

	if(m_nSelectedIndex==nIndex || 
		(m_nCurrentIndex==nIndex && m_bMouseButtonPressed))
	{
		// draw sunken rect
		pDC->Draw3dRect(rect,clrShadow,clrHilight);
	}
	else if(m_nCurrentIndex==nIndex)
	{
		// draw raised rect
		pDC->Draw3dRect(rect,clrHilight,clrShadow);
	}

	rect.DeflateRect(1,1);
	if(m_nSelectedIndex==nIndex && m_nCurrentIndex!=nIndex)
	{
		// fill rect with lighter color
		CBrush brush(clrLight);
		pDC->FillRect(&rect,&brush);
	}
}


void COXPopupBarCtrl::DrawBackground(CDC* pDC)
{
	ASSERT(pDC!=NULL);

	WNDCLASS wc;
	HBRUSH hBrush = NULL;
	if(GetClassInfo(AfxGetInstanceHandle(),POPUPBAR_CLASSNAME,&wc))
		hBrush=wc.hbrBackground;
	CRect rect;
	GetClientRect(rect);
	::FillRect(pDC->GetSafeHdc(),rect,hBrush);
}


LRESULT COXPopupBarCtrl::OnFloatStatus(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	if(!IsFloating())
	{
		return Default();
	}

	// these asserts make sure no conflicting actions are requested
	ASSERT(!((wParam & FS_SHOW) && (wParam & FS_HIDE)));
	ASSERT(!((wParam & FS_ENABLE) && (wParam & FS_DISABLE)));
	ASSERT(!((wParam & FS_ACTIVATE) && (wParam & FS_DEACTIVATE)));

	// FS_SYNCACTIVE is used to detect MFS_SYNCACTIVE windows
	LRESULT lResult=0;
	if((GetStyle() & MFS_SYNCACTIVE) && (wParam & FS_SYNCACTIVE))
	{
		lResult=1;
	}

	if(wParam & (FS_SHOW|FS_HIDE))
	{
		SetWindowPos(NULL,0,0,0,0,
			((wParam & FS_SHOW) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW)|
			SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
	}

	if(wParam&(FS_ENABLE|FS_DISABLE))
	{
		EnableWindow((wParam & FS_ENABLE)!=0);
	}

	if((wParam & (FS_ACTIVATE|FS_DEACTIVATE)) && (GetStyle() & MFS_SYNCACTIVE))
	{
		ModifyStyle(MFS_SYNCACTIVE,0);
		SendMessage(WM_NCACTIVATE,(wParam & FS_ACTIVATE)!=0);
		ModifyStyle(0,MFS_SYNCACTIVE);
	}

	return lResult;
}


BOOL COXPopupBarCtrl::OnNcCreate(LPCREATESTRUCT lpcs)
{
	if(!CWnd::OnNcCreate(lpcs))
	{
		return FALSE;
	}

	if(GetStyle() & MFS_SYNCACTIVE)
	{
		// syncronize activation state with top level parent
		CWnd* pParentWnd=GetTopLevelParent();
		ASSERT(pParentWnd!=NULL);
		CWnd* pActiveWnd=GetForegroundWindow();
		BOOL bActive=(pParentWnd==pActiveWnd) ||
			(pParentWnd->GetLastActivePopup()==pActiveWnd &&
			pActiveWnd->SendMessage(WM_FLOATSTATUS,FS_SYNCACTIVE)!=0);

		// the WM_FLOATSTATUS does the actual work
		SendMessage(WM_FLOATSTATUS, bActive ? FS_ACTIVATE : FS_DEACTIVATE);
	}

	return TRUE;
}


BOOL COXPopupBarCtrl::OnNcActivate(BOOL bActive)
{
	UNREFERENCED_PARAMETER(bActive);

	if((GetStyle() & MFS_SYNCACTIVE)==0)
	{
		return (BOOL)Default();
	}
	else if(m_nFlags & WF_KEEPMINIACTIVE)
	{
		return FALSE;
	}
	return TRUE;
}


