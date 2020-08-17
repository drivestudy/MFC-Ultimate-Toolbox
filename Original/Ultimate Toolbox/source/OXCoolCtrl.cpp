// ==========================================================================
//						   Class Implementation : 
//		COXCoolButton, COXCoolScrollBar, COXCoolSpinButtonCtrl
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
#include "OXCoolCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// COXCoolButton

BEGIN_MESSAGE_MAP(COXCoolButton, CButton)
	//{{AFX_MSG_MAP(COXCoolButton)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXCoolButton message handlers

#define OX_RADIOCHECK_BUTTON_WIDTH			13

void COXCoolButton::OnPaint() 
{
	Default();
	if(!IsInCoolState())
		return;

    const BOOL bIsEnabled=IsWindowEnabled();
	const BOOL bIsHot=IsInHotState() & bIsEnabled;

	// draw the control only when it is not in hot state
	if(bIsHot)
		return;

	int nButtonStyle=GetStyle()&0x000f;
	
	// don't handle ownerdraw case
	if(nButtonStyle==BS_OWNERDRAW || nButtonStyle==BS_USERBUTTON)
		return;

	// don't handle group box
	if(nButtonStyle==BS_GROUPBOX)
		return;

	// don't handle flat button
	if((GetStyle()&BS_FLAT)==BS_FLAT)
		return;

	// push button
	const BOOL bPushButton=(nButtonStyle==BS_DEFPUSHBUTTON || 
		nButtonStyle==BS_PUSHBUTTON);
	// flag that specifies that push button is default one
    const BOOL bIsDefault=(nButtonStyle==BS_DEFPUSHBUTTON);
	// check box
	const BOOL bCheckBox=(nButtonStyle==BS_CHECKBOX || 
		nButtonStyle==BS_AUTOCHECKBOX || nButtonStyle==BS_3STATE || 
		nButtonStyle==BS_AUTO3STATE);
	// radio button
	const BOOL bRadioButton=(nButtonStyle==BS_RADIOBUTTON || 
		nButtonStyle==BS_AUTORADIOBUTTON);
	// pushlike radio button or check box
	const BOOL bIsPushLike=((GetStyle()&BS_PUSHLIKE)==BS_PUSHLIKE);
	// right alighed button
	const BOOL bIsRightAligned=((GetStyle()&BS_LEFTTEXT)==BS_LEFTTEXT);

	// sanity check
	ASSERT(!(bPushButton&bCheckBox) && !(bPushButton&bRadioButton) && 
		!(bRadioButton&bCheckBox));
	ASSERT((bIsPushLike && (bCheckBox || bRadioButton)) || !bIsPushLike);
	ASSERT((bIsDefault&bPushButton) || !bIsDefault);
	///////


	CClientDC dc(this);

	CRect rectClient;
	GetClientRect(rectClient);
	CRect rectInternal=rectClient;

	if(bPushButton || bIsPushLike)
	{
		if(bIsPushLike)
			rectInternal.DeflateRect(2,2);
		else
			rectInternal.DeflateRect(3,3);

		dc.SelectClipRgn(NULL);   // from drawing background
		dc.ExcludeClipRect(&rectInternal);

		// working rect
		CRect rect=rectClient;
		dc.FillSolidRect(rect,::GetSysColor(COLOR_3DFACE));

		// draw border
		if(bIsDefault)
		{
			dc.Draw3dRect(rect,::GetSysColor(COLOR_3DDKSHADOW),
				::GetSysColor(COLOR_3DDKSHADOW));
			rect.DeflateRect(1,1);
		}

		int nCheckState=(bPushButton ? 0 : GetCheck());
		if(nCheckState==0)
		{
			dc.Draw3dRect(rect,::GetSysColor(COLOR_3DHIGHLIGHT),
				::GetSysColor(COLOR_3DSHADOW));
		}
		else
		{
			dc.Draw3dRect(rect,::GetSysColor(COLOR_3DSHADOW),
				::GetSysColor(COLOR_3DHIGHLIGHT));
		}
	}
	else if(bCheckBox || bRadioButton)
	{
		CRect rectBox=rectClient;

		if(!bIsRightAligned)
			rectBox.right=rectBox.left+OX_RADIOCHECK_BUTTON_WIDTH;
		else
			rectBox.left=rectBox.right-OX_RADIOCHECK_BUTTON_WIDTH;

		rectBox.top=(rectClient.top+rectClient.bottom-OX_RADIOCHECK_BUTTON_WIDTH)/2;
		rectBox.bottom=rectBox.top+OX_RADIOCHECK_BUTTON_WIDTH;

		if(bCheckBox)
		{
			rectBox.DeflateRect(1,1);
			dc.Draw3dRect(rectBox,::GetSysColor(COLOR_3DHIGHLIGHT),
				::GetSysColor(COLOR_3DHIGHLIGHT));
		}
		else
		{
			CPen pen(PS_SOLID,2,::GetSysColor(COLOR_3DHIGHLIGHT));
			CPen* pOldPen=dc.SelectObject(&pen);

			CPoint ptStart(rectBox.CenterPoint().x+rectBox.Width()/4+1,
				rectBox.top+rectBox.Height()/4+1);
			CPoint ptEnd(rectBox.left+rectBox.Width()/4+1,
				rectBox.CenterPoint().y+rectBox.Height()/4+1);
			rectBox.DeflateRect(2,1,0,1);
			dc.Arc(rectBox,ptStart,ptEnd);

			if(pOldPen!=NULL)
				dc.SelectObject(pOldPen);
		}
	}
	else
	{
		ASSERT(FALSE);
	}

    // unselect the clip region
    dc.SelectClipRgn(NULL);
}
/////////////////////////////////////////////////////////////////////////////


void COXCoolButton::OnChangeHotState(BOOL bInHotState) 
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	int nButtonStyle=GetStyle()&0x000f;
	// radio button
	if((nButtonStyle==BS_RADIOBUTTON || nButtonStyle==BS_AUTORADIOBUTTON))
	{
		if(m_bInHotState!=bInHotState && (!bInHotState || IsMouseOver()))
		{
			m_bInHotState=bInHotState;
			RedrawWindow();
		}
	}
	else
	{
		COXCoolCtrl<CButton>::OnChangeHotState(bInHotState);
		RedrawWindow();
	}
}


LRESULT COXCoolButton::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	if(message==WM_NCPAINT)
	{
		return CButton::WindowProc(message,wParam,lParam);
	}
	else if(message==BM_SETCHECK)
	{
		LRESULT lResult=COXCoolCtrl<CButton>::WindowProc(message,wParam,lParam);
		if((WPARAM)BST_UNCHECKED==wParam)
		{
			int nButtonStyle=GetStyle()&0x000f;
			// radio button
			if((nButtonStyle==BS_RADIOBUTTON || nButtonStyle==BS_AUTORADIOBUTTON))
				RedrawWindow();
		}
		return lResult;
	}
	else
	{
		return COXCoolCtrl<CButton>::WindowProc(message,wParam,lParam);
	}
}


/////////////////////////////////////////////////////////////////////////////
// COXCoolScrollBar

BEGIN_MESSAGE_MAP(COXCoolScrollBar, CScrollBar)
	//{{AFX_MSG_MAP(COXCoolScrollBar)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXCoolScrollBar message handlers

void COXCoolScrollBar::OnPaint() 
{
	Default();
	if(!IsInCoolState())
		return;

	// draw the control only when it is not in hot state
	if(IsInHotState())
		return;

	CClientDC dc(this);

	CRect rectClient;
	GetClientRect(rectClient);

	int nBarStyle=GetStyle();

	BOOL bVert=((nBarStyle&SBS_VERT)==SBS_VERT);

	DrawScrollBar(&dc,rectClient,!bVert);
}
/////////////////////////////////////////////////////////////////////////////


void COXCoolScrollBar::OnChangeHotState(BOOL bInHotState) 
{
	COXCoolCtrl<CScrollBar>::OnChangeHotState(bInHotState);
	RedrawWindow();
}


LRESULT COXCoolScrollBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	if(message==WM_NCPAINT)
	{
		return CScrollBar::WindowProc(message,wParam,lParam);
	}
	else
	{
		return COXCoolCtrl<CScrollBar>::WindowProc(message,wParam,lParam);
	}
}



/////////////////////////////////////////////////////////////////////////////
// COXCoolSpinButtonCtrl

BEGIN_MESSAGE_MAP(COXCoolSpinButtonCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(COXCoolSpinButtonCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXCoolSpinButtonCtrl message handlers

#define SPINBUTTON_MIN_LENGTH			4

void COXCoolSpinButtonCtrl::OnPaint() 
{
	Default();
	if(!IsInCoolState())
		return;

	CClientDC dc(this);

	CRect rectBar;
	GetClientRect(rectBar);

	int nBarStyle=GetStyle();
	BOOL bHorz=((nBarStyle&UDS_HORZ)==UDS_HORZ);
	int nAignment=((nBarStyle&UDS_ALIGNRIGHT)==UDS_ALIGNRIGHT ? 1 :
		((nBarStyle&UDS_ALIGNLEFT)==UDS_ALIGNLEFT ? 2 : 0));
	if(IsAttached())
	{
		CWnd* pWndBuddy=GetBuddy();
		ASSERT(pWndBuddy!=NULL && ::IsWindow(pWndBuddy->GetSafeHwnd()));
		CRect rectBuddyClient;
		pWndBuddy->GetClientRect(rectBuddyClient);
		pWndBuddy->ClientToScreen(rectBuddyClient);
		CRect rectBuddyWindow;
		pWndBuddy->GetWindowRect(rectBuddyWindow);
		int nBuddyBorderWidth=rectBuddyClient.top-rectBuddyWindow.top;
		if(nAignment==1)
		{
			rectBar.DeflateRect(0,nBuddyBorderWidth,
				nBuddyBorderWidth,nBuddyBorderWidth);
		}
		else
		{
			rectBar.DeflateRect(nBuddyBorderWidth,nBuddyBorderWidth,0,
				nBuddyBorderWidth);
		}

		if(CWnd::FromHandlePermanent(pWndBuddy->GetSafeHwnd())!=NULL)
		{
			CRect rectOuterBorder=rectBar;
			if(((COXCoolCtrl<CEdit>*)pWndBuddy)->IsInHotState() || IsInHotState())
			{
				rectOuterBorder.InflateRect(nBuddyBorderWidth,nBuddyBorderWidth);
				dc.Draw3dRect(rectOuterBorder,::GetSysColor(COLOR_3DSHADOW),
					::GetSysColor(COLOR_3DHIGHLIGHT));
				rectOuterBorder.DeflateRect(1,1);
				dc.Draw3dRect(rectOuterBorder,::GetSysColor(COLOR_3DDKSHADOW),
					::GetSysColor(COLOR_3DFACE));
			}
			else
			{
				rectOuterBorder.InflateRect(nBuddyBorderWidth,nBuddyBorderWidth);
				dc.Draw3dRect(rectOuterBorder,::GetSysColor(COLOR_3DHIGHLIGHT),
					::GetSysColor(COLOR_3DHIGHLIGHT));
				rectOuterBorder.DeflateRect(1,1);
				if(pWndBuddy->IsWindowEnabled())
					dc.Draw3dRect(rectOuterBorder,::GetSysColor(COLOR_3DHIGHLIGHT),
						::GetSysColor(COLOR_3DHIGHLIGHT));
				else
					dc.Draw3dRect(rectOuterBorder,::GetSysColor(COLOR_3DFACE),
						::GetSysColor(COLOR_3DFACE));
			}
		}
	}

	// draw the control only when it is not in hot state
	if(IsInHotState())
		return;

	CRect rectSpinButton1=rectBar;
	CRect rectSpinButton2=rectBar;
	int nSpinLength=(bHorz ? rectBar.Width() : rectBar.Height());
	int nButtonLength=nSpinLength/2;
	if(nButtonLength<SPINBUTTON_MIN_LENGTH)
		return;

	// draw spin control arrow buttons
	//
	if(bHorz)
	{
		rectSpinButton1.right=rectSpinButton1.left+nButtonLength;
		rectSpinButton2.left=rectSpinButton2.right-nButtonLength;
	}
	else
	{
		rectSpinButton1.bottom=rectSpinButton1.top+nButtonLength;
		rectSpinButton2.top=rectSpinButton2.bottom-nButtonLength;
	}
	
	dc.Draw3dRect(rectSpinButton1,::GetSysColor(COLOR_3DHIGHLIGHT),
		::GetSysColor(COLOR_3DSHADOW));
	rectSpinButton1.DeflateRect(1,1);
	dc.Draw3dRect(rectSpinButton1,::GetSysColor(COLOR_3DFACE),
		::GetSysColor(COLOR_3DFACE));

	dc.Draw3dRect(rectSpinButton2,::GetSysColor(COLOR_3DHIGHLIGHT),
		::GetSysColor(COLOR_3DSHADOW));
	rectSpinButton2.DeflateRect(1,1);
	dc.Draw3dRect(rectSpinButton2,::GetSysColor(COLOR_3DFACE),
		::GetSysColor(COLOR_3DFACE));
	//
	/////////////////////////////
}
/////////////////////////////////////////////////////////////////////////////


void COXCoolSpinButtonCtrl::OnChangeHotState(BOOL bInHotState) 
{
	COXCoolCtrl<CSpinButtonCtrl>::OnChangeHotState(bInHotState);
	RedrawWindow();
}


LRESULT COXCoolSpinButtonCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	if(message==WM_NCPAINT)
	{
		return CSpinButtonCtrl::WindowProc(message,wParam,lParam);
	}
	else
	{
		return COXCoolCtrl<CSpinButtonCtrl>::WindowProc(message,wParam,lParam);
	}
}


BOOL COXCoolSpinButtonCtrl::IsAttached() 
{
	int nBarStyle=GetStyle();
	int nAignment=((nBarStyle&UDS_ALIGNRIGHT)==UDS_ALIGNRIGHT ? 1 :
		((nBarStyle&UDS_ALIGNLEFT)==UDS_ALIGNLEFT ? 2 : 0));
	
	if(nAignment!=0)
	{
		CWnd* pWndBuddy=GetBuddy();
		if(pWndBuddy!=NULL && ::IsWindow(pWndBuddy->GetSafeHwnd()))
		{
			CString sClassName;
			::GetClassName(pWndBuddy->GetSafeHwnd(),
				sClassName.GetBuffer(256),256);
//			sClassName.MakeUpper();
			if(sClassName.Collate(_T("Edit"))==0)
//			if (sClassName == "Edit")
				return TRUE;
		}
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// COXCoolSliderCtrl

BEGIN_MESSAGE_MAP(COXCoolSliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(COXCoolSliderCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXCoolSliderCtrl message handlers

void COXCoolSliderCtrl::OnPaint() 
{
	Default();
	if(!IsInCoolState())
		return;

	// draw the control only when it is not in hot state
	if(IsInHotState())
		return;

	int nSliderCtrlStyle=GetStyle();
	
	// control orientation
	const BOOL bHorz=((nSliderCtrlStyle&TBS_VERT)==0);
	// point orientation of SliderCtrl: 0 - both, 1 - TopLeft, 2 - BottomRight
    const int nPointOrient=((nSliderCtrlStyle&TBS_BOTH)==TBS_BOTH ? 0 : 
		((nSliderCtrlStyle&TBS_TOP)==TBS_TOP ? 1 : 2));

	CClientDC dc(this);

    static CPen penHilite(PS_SOLID,1,GetSysColor(COLOR_3DHIGHLIGHT));
    static CPen penShadow(PS_SOLID,1,GetSysColor(COLOR_3DSHADOW));
    static CPen penFace3D(PS_SOLID,1,GetSysColor(COLOR_3DFACE));

	// draw thumb
	//
	CRect rectThumb;
	GetThumbRect(rectThumb);

	switch(nPointOrient)
	{
	// both
	case 0:
		{
			dc.Draw3dRect(rectThumb,::GetSysColor(COLOR_3DHIGHLIGHT),
				::GetSysColor(COLOR_3DSHADOW));
			rectThumb.DeflateRect(1,1);
			dc.Draw3dRect(rectThumb,::GetSysColor(COLOR_3DFACE),
				::GetSysColor(COLOR_3DFACE));
		}
		break;
	// TopLeft
	case 1:
		{
			if(bHorz)
			{
				dc.SelectObject(penShadow);
				dc.MoveTo(rectThumb.CenterPoint().x,rectThumb.top);
				dc.LineTo(rectThumb.right-1,rectThumb.top+rectThumb.Width()/2);
				dc.LineTo(rectThumb.right-1,rectThumb.bottom-1);
				dc.LineTo(rectThumb.left-1,rectThumb.bottom-1);

				dc.SelectObject(penHilite);
				dc.MoveTo(rectThumb.left,rectThumb.bottom-2);
				dc.LineTo(rectThumb.left,rectThumb.top+rectThumb.Width()/2);
				dc.LineTo(rectThumb.CenterPoint().x,rectThumb.top);

				rectThumb.DeflateRect(1,1);
				dc.SelectObject(penFace3D);
				dc.MoveTo(rectThumb.CenterPoint().x,rectThumb.top);
				dc.LineTo(rectThumb.right-1,rectThumb.top+rectThumb.Width()/2);
				dc.LineTo(rectThumb.right-1,rectThumb.bottom-1);
				dc.LineTo(rectThumb.left,rectThumb.bottom-1);
				dc.LineTo(rectThumb.left,rectThumb.top+rectThumb.Width()/2);
				dc.LineTo(rectThumb.CenterPoint().x,rectThumb.top);
			}
			else
			{
				dc.SelectObject(penShadow);
				dc.MoveTo(rectThumb.left,rectThumb.CenterPoint().y);
				dc.LineTo(rectThumb.left+rectThumb.Height()/2,rectThumb.bottom-1);
				dc.LineTo(rectThumb.right-1,rectThumb.bottom-1);
				dc.LineTo(rectThumb.right-1,rectThumb.top-1);

				dc.SelectObject(penHilite);
				dc.MoveTo(rectThumb.right-2,rectThumb.top);
				dc.LineTo(rectThumb.left+rectThumb.Height()/2,rectThumb.top);
				dc.LineTo(rectThumb.left,rectThumb.CenterPoint().y);

				rectThumb.DeflateRect(1,1);
				dc.SelectObject(penFace3D);
				dc.MoveTo(rectThumb.left,rectThumb.CenterPoint().y);
				dc.LineTo(rectThumb.left+rectThumb.Height()/2,rectThumb.bottom-1);
				dc.LineTo(rectThumb.right-1,rectThumb.bottom-1);
				dc.LineTo(rectThumb.right-1,rectThumb.top);
				dc.LineTo(rectThumb.left+rectThumb.Height()/2,rectThumb.top);
				dc.LineTo(rectThumb.left,rectThumb.CenterPoint().y);
			}
		}
		break;
	// BottomRight
	case 2:
		{
			if(bHorz)
			{
				dc.SelectObject(penHilite);
				dc.MoveTo(rectThumb.CenterPoint().x,rectThumb.bottom-1);
				dc.LineTo(rectThumb.left,rectThumb.bottom-rectThumb.Width()/2-1);
				dc.LineTo(rectThumb.left,rectThumb.top);
				dc.LineTo(rectThumb.right,rectThumb.top);

				dc.SelectObject(penShadow);
				dc.MoveTo(rectThumb.right-1,rectThumb.top);
				dc.LineTo(rectThumb.right-1,rectThumb.bottom-rectThumb.Width()/2-1);
				dc.LineTo(rectThumb.CenterPoint().x,rectThumb.bottom-1);

				rectThumb.DeflateRect(1,1);
				dc.SelectObject(penFace3D);
				dc.MoveTo(rectThumb.CenterPoint().x,rectThumb.bottom-1);
				dc.LineTo(rectThumb.left,rectThumb.bottom-rectThumb.Width()/2-1);
				dc.LineTo(rectThumb.left,rectThumb.top);
				dc.LineTo(rectThumb.right-1,rectThumb.top);
				dc.LineTo(rectThumb.right-1,rectThumb.bottom-rectThumb.Width()/2-1);
				dc.LineTo(rectThumb.CenterPoint().x,rectThumb.bottom-1);
			}
			else
			{
				dc.SelectObject(penHilite);
				dc.MoveTo(rectThumb.right-2,rectThumb.CenterPoint().y-1);
				dc.LineTo(rectThumb.right-rectThumb.Height()/2-1,rectThumb.top);
				dc.LineTo(rectThumb.left,rectThumb.top);
				dc.LineTo(rectThumb.left,rectThumb.bottom-1);

				dc.SelectObject(penShadow);
				dc.MoveTo(rectThumb.left,rectThumb.bottom-1);
				dc.LineTo(rectThumb.right-rectThumb.Height()/2-1,rectThumb.bottom-1);
				dc.LineTo(rectThumb.right,rectThumb.CenterPoint().y-1);

				rectThumb.DeflateRect(1,1);
				dc.SelectObject(penFace3D);
				dc.MoveTo(rectThumb.right-1,rectThumb.CenterPoint().y);
				dc.LineTo(rectThumb.right-rectThumb.Height()/2-1,rectThumb.top);
				dc.LineTo(rectThumb.left,rectThumb.top);
				dc.LineTo(rectThumb.left,rectThumb.bottom-1);
				dc.LineTo(rectThumb.right-rectThumb.Height()/2-1,rectThumb.bottom-1);
				dc.LineTo(rectThumb.right-1,rectThumb.CenterPoint().y);
			}
		}
		break;
	}
	rectThumb.InflateRect(1,1);
	dc.ExcludeClipRect(rectThumb);
	//
	////////////////////////////////

	// draw channel
	//
	CRect rectChannel;
	GetChannelRect(rectChannel);
	if(!bHorz)
	{
		CRect rect(rectChannel.top,rectChannel.left,
			rectChannel.bottom,rectChannel.right);
		rectChannel=rect;
	}
	dc.Draw3dRect(rectChannel,::GetSysColor(COLOR_3DSHADOW),
		::GetSysColor(COLOR_3DHIGHLIGHT));
	rectChannel.DeflateRect(1,1);
	dc.Draw3dRect(rectChannel,::GetSysColor(COLOR_3DFACE),
		::GetSysColor(COLOR_3DFACE));
	//
	////////////////////////////////
}
/////////////////////////////////////////////////////////////////////////////


void COXCoolSliderCtrl::OnChangeHotState(BOOL bInHotState) 
{
	COXCoolCtrl<CSliderCtrl>::OnChangeHotState(bInHotState);
	RedrawWindow();
}


LRESULT COXCoolSliderCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	if(message==WM_NCPAINT)
	{
		return CSliderCtrl::WindowProc(message,wParam,lParam);
	}
	else
	{
		return COXCoolCtrl<CSliderCtrl>::WindowProc(message,wParam,lParam);
	}
}


