// OXSeparator.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXSeparator.h"

#include "UTB64Bit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXSeparator

COXSeparator::COXSeparator()
{
	// set default values
	VERIFY(SetTextColor());
	VERIFY(SetSeparatorColors());
	SetVertOriented(FALSE);
	m_sText.Empty();
}

COXSeparator::~COXSeparator()
{
}


BEGIN_MESSAGE_MAP(COXSeparator, CStatic)
	//{{AFX_MSG_MAP(COXSeparator)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_SETTEXT,OnSetText)
	ON_MESSAGE(WM_GETTEXT,OnGetText)
	ON_MESSAGE(WM_GETTEXTLENGTH,OnGetTextLength)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXSeparator message handlers

void COXSeparator::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	// make sure there wasn't any border styles specified, 
	// we don't support them
	ModifyStyle(WS_BORDER,0);
	ModifyStyleEx(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|
		WS_EX_WINDOWEDGE|WS_EX_DLGMODALFRAME,0,SWP_FRAMECHANGED);
	/////////////////////////////////

	// save the window text
	GetWindowText(m_sText);
	
	CStatic::PreSubclassWindow();
}

int COXSeparator::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// make sure there wasn't any border styles specified, 
	// we don't support them
	ASSERT((lpCreateStruct->style&WS_BORDER)==0);
	ASSERT((lpCreateStruct->dwExStyle&(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|
		WS_EX_WINDOWEDGE|WS_EX_DLGMODALFRAME))==0);
	lpCreateStruct->style&=~WS_BORDER;
	lpCreateStruct->dwExStyle&=~(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|
		WS_EX_WINDOWEDGE|WS_EX_DLGMODALFRAME);
	/////////////////////////////////

	if(CStatic::OnCreate(lpCreateStruct)==-1)
		return -1;

	// save the window text
	m_sText=lpCreateStruct->lpszName;
	
	return 0;
}

void COXSeparator::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// draw text
	//

	// setup environment
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_clrText);

	CFont* pOldFont=NULL;
	CFont fontVert;
	CFont* pFont=GetFont();
	if(pFont)
	{
		if(m_bVertOriented)
		{
			// setup font for vertically oriented mode
			LOGFONT lf;
			VERIFY(pFont->GetLogFont(&lf));
			lf.lfEscapement=900;
			lf.lfOrientation=900;
			VERIFY(fontVert.CreateFontIndirect(&lf));
			pOldFont=dc.SelectObject(&fontVert);
		}
		else
			pOldFont=dc.SelectObject(pFont);
	}

	CRect rectClient;
	GetClientRect(&rectClient);
	CRect rectSeparator=rectClient;
	CRect rectText(0,0,0,0);

	// flag that specifies that separator is divided in two parts by 
	// centerd text
	BOOL bTwoParts=FALSE;
	if(!m_sText.IsEmpty())
	{
		rectText=rectClient;
		CRect rectHelper;
		if(m_bVertOriented)
		{
			// adjust rectangle to display verical texr
			rectHelper=rectText;
			rectText.top=rectHelper.left;
			rectText.bottom=rectHelper.right;
			rectText.left=rectHelper.top;
			rectText.right=rectHelper.bottom;
		}

		// calculate the rect to siplay text in
		UINT nFormat=DT_LEFT|DT_SINGLELINE;
		dc.DrawText(m_sText,&rectText,nFormat|DT_CALCRECT);
		rectHelper=rectText;
		if(m_bVertOriented)
		{
			rectText.top=rectHelper.left;
			rectText.bottom=rectHelper.right;
			rectText.left=rectHelper.top;
			rectText.right=rectHelper.bottom;
			rectHelper=rectText;
		}

		// adjust coordinates
		if(m_bVertOriented)
		{
			rectText.left+=(rectClient.Width()-rectHelper.Width())/2+
				(rectSeparator.Width()-rectHelper.Width())%2;
			rectText.right=rectText.left+rectHelper.Width();
		}
		else
		{
			rectText.top+=(rectClient.Height()-rectHelper.Height())/2+
				(rectClient.Height()-rectHelper.Height())%2;
			rectText.bottom=rectText.top+rectHelper.Height();
		}

		// adjust the text box depending on text alignment style
		DWORD dwStyle=GetStyle()&0x0000000f;
		switch(dwStyle)
		{
		// SS_CENTER
		case 1:
			bTwoParts=TRUE;
			if(m_bVertOriented)
			{
				rectSeparator.bottom=(rectSeparator.bottom-rectSeparator.top)/2-
					rectText.Height()/2-6;
				rectText.top+=(rectClient.Height()-rectHelper.Height())/2+
					(rectClient.Height()-rectHelper.Height())%2;
				rectText.bottom=rectText.top+rectHelper.Height();
			}
			else
			{
				rectSeparator.right=(rectSeparator.right-rectSeparator.left)/2-
					rectText.Width()/2-6;
				rectText.left+=(rectClient.Width()-rectHelper.Width())/2+
					(rectSeparator.Width()-rectHelper.Width())%2;
				rectText.right=rectText.left+rectHelper.Width();
			}
			break;
		// SS_RIGHT
		case 2:
			if(m_bVertOriented)
			{
				rectSeparator.bottom-=rectText.Height()+6;
				rectText.bottom=rectClient.bottom;
				rectText.top=rectText.bottom-rectHelper.Height();
			}
			else
			{
				rectSeparator.right-=rectText.Width()+6;
				rectText.right=rectClient.right;
				rectText.left=rectText.right-rectHelper.Width();
			}
			break;	
		// SS_LEFT
		default:
			if(m_bVertOriented)
			{
				rectSeparator.top=rectText.bottom+6;
				rectText.top=rectClient.top;
				rectText.bottom=rectText.top+rectHelper.Height();
			}
			else
			{
				rectSeparator.left=rectText.right+6;
				rectText.left=rectClient.left;
				rectText.right=rectText.left+rectHelper.Width();
			}
		}

		rectHelper=rectText;
		if(m_bVertOriented)
		{
			rectHelper.bottom+=rectText.Width();
			rectHelper.right+=rectText.Height();
		}
		// draw text
		dc.DrawText(m_sText,&rectHelper,DT_BOTTOM|DT_LEFT|DT_SINGLELINE);
	}

	// adjust rectangle for separator
	if(m_bVertOriented)
	{
		rectSeparator.left+=(rectSeparator.Width())/2+
			(rectSeparator.Width())%2-1;
		rectSeparator.right=rectSeparator.left+2;
	}
	else
	{
		rectSeparator.top+=(rectSeparator.Height())/2+
			(rectSeparator.Height())%2-1;
		rectSeparator.bottom=rectSeparator.top+2;
	}

	// draw separator
	if((m_bVertOriented && rectSeparator.top<rectSeparator.bottom) ||
		(!m_bVertOriented && rectSeparator.left<rectSeparator.right))
	{
		dc.Draw3dRect(&rectSeparator,m_clrTopLeft,m_clrBottomRight);

		if(bTwoParts)
		{
			if(m_bVertOriented)
			{
				rectSeparator.top=rectSeparator.bottom+rectText.Height()+12;
				rectSeparator.bottom=rectClient.bottom;
			}
			else
			{
				rectSeparator.left=rectSeparator.right+rectText.Width()+12;
				rectSeparator.right=rectClient.right;
			}

			dc.Draw3dRect(&rectSeparator,m_clrTopLeft,m_clrBottomRight);
		}
	}

	if(pOldFont!=NULL)
		dc.SelectObject(pOldFont);

	// Do not call CStatic::OnPaint() for painting messages
}

LONG COXSeparator::OnSetText(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	// save the window text
	m_sText=(LPCTSTR)(LONG_PTR)lParam;

	RedrawWindow();

	return TRUE;
}

LONG COXSeparator::OnGetText(UINT wParam, LONG lParam)
{
	if(wParam>0)
	{
		wParam=wParam>(UINT)m_sText.GetLength()+1 ? 
			m_sText.GetLength()+1 : wParam;
		VERIFY(lstrcpyn((LPTSTR)(LONG_PTR)lParam,m_sText,wParam-1)!=NULL);
	}
	return (LONG)wParam;
}

LONG COXSeparator::OnGetTextLength(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return (LONG)m_sText.GetLength();
}
