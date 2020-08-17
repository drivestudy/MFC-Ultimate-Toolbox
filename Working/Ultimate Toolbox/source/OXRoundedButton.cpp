// ==========================================================================
// 					Class Implementation : COXRoundedButton
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
#include "OXRoundedButton.h"

#include "OXGraphics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXRoundedButton

COXRoundedButton::COXRoundedButton()
{
	// colors
	m_clrButton=::GetSysColor(COLOR_BTNFACE);
	m_clrLight=::GetSysColor(COLOR_WINDOW);
	m_clrBk=::GetSysColor(COLOR_BTNFACE);

	// figure apperance
	m_nSphereExtRadius=8;
	m_nSphereIntRadius=3;

	// light source intensity
	m_fLightIntensityCoef=1.3f;

	// light angles
	m_fThetta=60;
	m_fPhi=-45;

	// surface parameters
	m_nPhong=15;
	m_fMirror=0.6f;
	m_fDiffuse=0.5f;
	m_fAmbient=0.9f;
}

COXRoundedButton::~COXRoundedButton()
{
}


BEGIN_MESSAGE_MAP(COXRoundedButton, COXBitmapButton)
	//{{AFX_MSG_MAP(COXRoundedButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_MESSAGE(WM_SETTINGCHANGE, OnChangeSettings)

//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void COXRoundedButton::SetButtonColor(COLORREF clrButton, 
									  BOOL bRedraw/*=TRUE*/)
{
	if(m_clrButton!=clrButton)
	{
		m_clrButton=clrButton;
		if(bRedraw && ::IsWindow(GetSafeHwnd()))
			RedrawWindow();
	}
}

void COXRoundedButton::SetLightColor(COLORREF clrLight, 
									 BOOL bRedraw/*=TRUE*/)
{
	if(m_clrLight!=clrLight)
	{
		m_clrLight=clrLight;
		if(bRedraw && ::IsWindow(GetSafeHwnd()))
			RedrawWindow();
	}
}

void COXRoundedButton::SetBkColor(COLORREF clrBk, 
								  BOOL bRedraw/*=TRUE*/)
{
	if(m_clrBk!=clrBk)
	{
		m_clrBk=clrBk;
		if(bRedraw && ::IsWindow(GetSafeHwnd()))
			RedrawWindow();
	}
}

void COXRoundedButton::SetSphereExternalRadius(int nSphereExtRadius, 
											   BOOL bRedraw/*=TRUE*/)
{
	if(nSphereExtRadius<=0)
	{
		TRACE(_T("COXRoundedButton::SetSphereExternalRadius: failed, non-positive sphere external radius has been specified"));
		return;
	}

	if(nSphereExtRadius<m_nSphereIntRadius)
	{
		TRACE(_T("COXRoundedButton::SetSphereExternalRadius: failed, smaller than internal external sphere radius has been specified"));
		return;
	}

	if(m_nSphereExtRadius!=nSphereExtRadius)
	{
		m_nSphereExtRadius=nSphereExtRadius;
		if(bRedraw && ::IsWindow(GetSafeHwnd()))
			RedrawWindow();
	}
}

void COXRoundedButton::SetSphereInternalRadius(int nSphereIntRadius, 
											   BOOL bRedraw/*=TRUE*/)
{
	if(nSphereIntRadius<0)
	{
		TRACE(_T("COXRoundedButton::SetSphereInternalRadius: failed, negative sphere internal radius has been specified"));
		return;
	}

	if(nSphereIntRadius>m_nSphereExtRadius)
	{
		TRACE(_T("COXRoundedButton::SetSphereInternalRadius: failed, bigger than external internal sphere radius has been specified"));
		return;
	}

	if(m_nSphereIntRadius!=nSphereIntRadius)
	{
		m_nSphereIntRadius=nSphereIntRadius;
		if(bRedraw && ::IsWindow(GetSafeHwnd()))
			RedrawWindow();
	}
}

void COXRoundedButton::SetLightIntensityCoef(float fLightIntensityCoef, 
											 BOOL bRedraw/*=TRUE*/)
{
	if(m_fLightIntensityCoef!=fLightIntensityCoef)
	{
		m_fLightIntensityCoef=fLightIntensityCoef;
		if(bRedraw && ::IsWindow(GetSafeHwnd()))
			RedrawWindow();
	}
}

void COXRoundedButton::SetThetta(float fThetta, BOOL bRedraw/*=TRUE*/)
{
	if(m_fThetta!=fThetta)
	{
		m_fThetta=fThetta;
		if(bRedraw && ::IsWindow(GetSafeHwnd()))
			RedrawWindow();
	}
}

void COXRoundedButton::SetPhi(float fPhi, BOOL bRedraw/*=TRUE*/)
{
	if(m_fPhi!=fPhi)
	{
		m_fPhi=fPhi;
		if(bRedraw && ::IsWindow(GetSafeHwnd()))
			RedrawWindow();
	}
}

void COXRoundedButton::SetPhong(int nPhong, BOOL bRedraw/*=TRUE*/)
{
	if(m_nPhong!=nPhong)
	{
		m_nPhong=nPhong;
		if(bRedraw && ::IsWindow(GetSafeHwnd()))
			RedrawWindow();
	}
}

void COXRoundedButton::SetMirrorCoef(float fMirror, BOOL bRedraw/*=TRUE*/)
{
	if(m_fMirror!=fMirror)
	{
		m_fMirror=fMirror;
		if(bRedraw && ::IsWindow(GetSafeHwnd()))
			RedrawWindow();
	}
}

void COXRoundedButton::SetDiffuseCoef(float fDiffuse, BOOL bRedraw/*=TRUE*/)
{
	if(m_fDiffuse!=fDiffuse)
	{
		m_fDiffuse=fDiffuse;
		if(bRedraw && ::IsWindow(GetSafeHwnd()))
			RedrawWindow();
	}
}

void COXRoundedButton::SetAmbientCoef(float fAmbient, BOOL bRedraw/*=TRUE*/)
{
	if(m_fAmbient!=fAmbient)
	{
		m_fAmbient=fAmbient;
		if(bRedraw && ::IsWindow(GetSafeHwnd()))
			RedrawWindow();
	}
}

CSize COXRoundedButton::CalcSphereRadius()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	CRect rect;
	GetClientRect(rect);
	int nMaxRadius=__min(rect.Width(),rect.Height())/2;
	CSize szRadius(0,0);

	if(m_nSphereExtRadius>0)
		szRadius.cx=m_nSphereExtRadius;

	if(m_nSphereIntRadius>0)
		szRadius.cy=m_nSphereIntRadius;

	if(szRadius.cx==0)
		szRadius.cx=1;

	if(szRadius.cx>nMaxRadius)
		szRadius.cx=nMaxRadius;
	
	if(szRadius.cy>nMaxRadius)
		szRadius.cy=nMaxRadius;
	
	return szRadius;
}

/////////////////////////////////////////////////////////////////////////////
// COXRoundedButton message handlers

void COXRoundedButton::DistributeSpace(UINT nState, CRect itemRect, 
									   CRect& buttonRect, CRect& imageRect, 
									   CRect& textRect)
{
	CSize szRadius=CalcSphereRadius();

	// adjust the size depending on the sphere radius size
	m_ptImageOffset-=CPoint(szRadius.cx-szRadius.cy,szRadius.cx-szRadius.cy);
	m_ptTextOffset-=CPoint(szRadius.cx-szRadius.cy,szRadius.cx-szRadius.cy);

	COXBitmapButton::DistributeSpace(nState,itemRect,buttonRect,imageRect,textRect);

	// reset settings
	m_ptImageOffset+=CPoint(szRadius.cx-szRadius.cy,szRadius.cx-szRadius.cy);
	m_ptTextOffset+=CPoint(szRadius.cx-szRadius.cy,szRadius.cx-szRadius.cy);
}

CSize COXRoundedButton::GetFitButtonSize()
{
	CSize szRadius=CalcSphereRadius();

	// adjust the size depending on the sphere radius size
	m_ptImageOffset-=CPoint(szRadius.cx-szRadius.cy,szRadius.cx-szRadius.cy);
	m_ptTextOffset-=CPoint(szRadius.cx-szRadius.cy,szRadius.cx-szRadius.cy);
	
	CSize szFit=COXBitmapButton::GetFitButtonSize();
	
	// reset settings
	m_ptImageOffset+=CPoint(szRadius.cx-szRadius.cy,szRadius.cx-szRadius.cy);
	m_ptTextOffset+=CPoint(szRadius.cx-szRadius.cy,szRadius.cx-szRadius.cy);

	return szFit;
}

void COXRoundedButton::DrawButton(CDC* pDC, UINT nState, CRect buttonRect)
{
	CPalette palette;
	DrawButtonBackground(pDC,buttonRect,nState,&palette);
	if (HPALETTE(palette)!=NULL)
	{
		pDC->SelectPalette(&palette,FALSE);
		pDC->RealizePalette();
	}
}

void COXRoundedButton::DrawFocusRectangle(CDC* pDC, UINT nState, CRect buttonRect, 
										  CRect bitmapRect)
{
	// Draw inner focus rect
	CRect textRect;
	DistributeSpace(nState,buttonRect,buttonRect,bitmapRect,textRect);
	if(!textRect.IsRectEmpty())
	{
		textRect.DeflateRect(m_ptInnerFocusOffset.x,m_ptInnerFocusOffset.y);
		pDC->DrawFocusRect(textRect);
	}
	else if(!bitmapRect.IsRectEmpty())
	{
		pDC->DrawFocusRect(bitmapRect);
	}
}


void COXRoundedButton::DrawButtonBackground(CDC* pDC, CRect buttonRect, 
											UINT nState, CPalette* pPalette/*=NULL*/)
{
	// back color, may be used as transparent color
	COLORREF BackColor=m_clrBk;

	CSize szRadius=CalcSphereRadius();

	COXGraphics graphics;

	if (!(IsChecked() || (nState & ODS_SELECTED) == ODS_SELECTED))
	{
		graphics.DrawRoundedButton(pDC,buttonRect.left,buttonRect.top,
			buttonRect.Width(),buttonRect.Height(),
			szRadius.cx,szRadius.cy,pPalette,
			m_clrButton,m_clrLight,m_fThetta, 
			m_fPhi,m_fLightIntensityCoef,
			m_nPhong,m_fMirror,m_fDiffuse,
			m_fAmbient,BackColor);
	}
	else
	{
		graphics.DrawRoundedButton(pDC,buttonRect.left,buttonRect.top,
			buttonRect.Width(),buttonRect.Height(),
			szRadius.cx,szRadius.cy+(szRadius.cx-szRadius.cy)/3,pPalette,
			m_clrButton,m_clrLight,m_fThetta, 
			m_fPhi+180,m_fLightIntensityCoef,
			m_nPhong,0,m_fDiffuse,
			m_fAmbient,BackColor);
	}
}

void COXRoundedButton::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	if (IsInClientArea(point))
		COXBitmapButton::OnLButtonDblClk(nFlags, point);
}

void COXRoundedButton::OnRButtonDblClk( UINT nFlags, CPoint point )
{
	if (IsInClientArea(point))
		COXBitmapButton::OnRButtonDblClk(nFlags, point);

}

void COXRoundedButton::OnMButtonDblClk( UINT nFlags, CPoint point )
{
	if (IsInClientArea(point))
		COXBitmapButton::OnMButtonDblClk(nFlags, point);

}

void COXRoundedButton::OnLButtonDown( UINT nFlags, CPoint point )
{
	if (IsInClientArea(point))
		COXBitmapButton::OnLButtonDown(nFlags, point);

}

void COXRoundedButton::OnRButtonDown( UINT nFlags, CPoint point )
{
	if (IsInClientArea(point))
		COXBitmapButton::OnRButtonDown(nFlags, point);

}

void COXRoundedButton::OnMButtonDown( UINT nFlags, CPoint point )
{
	if (IsInClientArea(point))
		COXBitmapButton::OnMButtonDown(nFlags, point);

}

void COXRoundedButton::OnLButtonUp( UINT nFlags, CPoint point )
{
//	if (IsInClientArea(point))
		COXBitmapButton::OnLButtonUp(nFlags, point);

}

void COXRoundedButton::OnRButtonUp( UINT nFlags, CPoint point )
{
//	if (IsInClientArea(point))
		COXBitmapButton::OnRButtonUp(nFlags, point);

}

void COXRoundedButton::OnMButtonUp( UINT nFlags, CPoint point )
{
//	if (IsInClientArea(point))
		COXBitmapButton::OnMButtonUp(nFlags, point);

}



BOOL COXRoundedButton::IsInClientArea(CPoint pt)
{

	CRect rect;
	GetClientRect(&rect);

	//get real sphere radius that should not be
	//larger than half of least side of the clientrect
	int nSphere=min(m_nSphereExtRadius,
		min(rect.Width(),rect.Height())/2);

	CRect rectHight(rect);
	rectHight.left+=nSphere;
	rectHight.right-=nSphere;
	if (rectHight.PtInRect(pt))
		return TRUE;

	CRect rectWide(rect);
	rectWide.top+=nSphere;
	rectWide.bottom-=nSphere;
	if (rectWide.PtInRect(pt))
		return TRUE;

	CPoint ptLeftTop(rectHight.left, rectWide.top);
	CPoint ptLeftBottom(rectHight.left, rectWide.bottom);
	CPoint pRightTop(rectHight.right, rectWide.top);
	CPoint ptRightBottom(rectHight.right, rectWide.bottom);

	if (((pt.x-ptLeftTop.x)*(pt.x-ptLeftTop.x)+
		(pt.y-ptLeftTop.y)*(pt.y-ptLeftTop.y))<
		(nSphere*nSphere))
		return TRUE;

	if (((pt.x-ptLeftBottom.x)*(pt.x-ptLeftBottom.x)+
		(pt.y-ptLeftBottom.y)*(pt.y-ptLeftBottom.y))<
		(nSphere*nSphere))
		return TRUE;

	if (((pt.x-pRightTop.x)*(pt.x-pRightTop.x)+
		(pt.y-pRightTop.y)*(pt.y-pRightTop.y))<
		(nSphere*nSphere))
		return TRUE;
	
	if (((pt.x-ptRightBottom.x)*(pt.x-ptRightBottom.x)+
		(pt.y-ptRightBottom.y)*(pt.y-ptRightBottom.y))<
		(nSphere*nSphere))
		return TRUE;
	
	return FALSE;
}

LRESULT COXRoundedButton::OnChangeSettings(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(wParam);
	UNUSED_ALWAYS(lParam);
	m_clrBk=::GetSysColor(COLOR_BTNFACE);
	RedrawWindow();
	return TRUE;
}

#if _MSC_VER >= 1400
LRESULT COXRoundedButton::OnNcHitTest(CPoint point)
#else
UINT COXRoundedButton::OnNcHitTest(CPoint point)
#endif
{
	CPoint pt(point);
	ScreenToClient(&pt);
	if (IsInClientArea(pt))
		return COXBitmapButton::OnNcHitTest(point);
	else
		return HTNOWHERE;
}

