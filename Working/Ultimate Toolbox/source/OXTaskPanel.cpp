// OXTaskPanel.cpp : implementation file
//

#include "stdafx.h"
#include "OXTaskPanel.h"

//Theme API wrapper declarations

HMODULE g_huxtheme = NULL;

bool G_LoadThemeLibrary()
{
	if (!g_huxtheme) 
		g_huxtheme = ::LoadLibrary(_T("uxtheme.dll"));
	return (g_huxtheme != NULL);
}

HTHEME G_OpenThemeData(HWND hwnd, LPCTSTR pszClassList)
{
	G_LoadThemeLibrary();
	OPENTHEMEDATA pOpenThemeData = (OPENTHEMEDATA)GetProcAddress(g_huxtheme, "OpenThemeData");
	if (pOpenThemeData != NULL)
		return (*pOpenThemeData)(hwnd, pszClassList);		
	else
		return NULL;
}

HRESULT G_CloseThemeData(HTHEME hTheme)
{
	G_LoadThemeLibrary();
	CLOSETHEMEDATA pCloseThemeData = (CLOSETHEMEDATA)GetProcAddress(g_huxtheme, "CloseThemeData");
	if (pCloseThemeData != NULL)
		return (*pCloseThemeData)(hTheme);		
	else
		return E_FAIL;
}

HRESULT G_DrawThemeBackground(HTHEME hTheme, HDC hdc, 
    int iPartId, int iStateId, const RECT *pRect, OPTIONAL const RECT *pClipRect)
{
	G_LoadThemeLibrary();
	DRAWTHEMEBACKGROUND pDrawThemeBackground = 
		(DRAWTHEMEBACKGROUND)GetProcAddress(g_huxtheme, "DrawThemeBackground");
	if (pDrawThemeBackground != NULL)
		return (*pDrawThemeBackground)(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);		
	else
		return E_FAIL;
}

HRESULT G_DrawThemeText(HTHEME hTheme, HDC hdc, int iPartId, 
    int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, 
    DWORD dwTextFlags2, const RECT *pRect)
{
	G_LoadThemeLibrary();
	DRAWTHEMETEXT pDrawThemeText = (DRAWTHEMETEXT)GetProcAddress(g_huxtheme, "DrawThemeText");
	if (pDrawThemeText != NULL)
		return (*pDrawThemeText)(hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwTextFlags, dwTextFlags2, pRect);		
	else
		return E_FAIL;
}

HRESULT G_DrawThemeEdge(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, 
                       const RECT *pDestRect, UINT uEdge, UINT uFlags, OPTIONAL OUT RECT *pContentRect)
{
	G_LoadThemeLibrary();
	DRAWTHEMEEDGE pDrawThemeEdge = (DRAWTHEMEEDGE)GetProcAddress(g_huxtheme, "DrawThemeEdge");
	if (pDrawThemeEdge != NULL)
		return (*pDrawThemeEdge)(hTheme, hdc, iPartId, iStateId, pDestRect, uEdge, uFlags, pContentRect);
	else
		return E_FAIL;
}

HRESULT G_GetThemeColor(HTHEME hTheme, int iPartId, 
    int iStateId, int iPropId, OUT COLORREF *pColor)
{
	G_LoadThemeLibrary();
	GETTHEMECOLOR pGetThemeColor = (GETTHEMECOLOR)GetProcAddress(g_huxtheme, "GetThemeColor");
	if (pGetThemeColor != NULL)
		return (*pGetThemeColor)(hTheme, iPartId, iStateId, iPropId, pColor);
	else
		return E_FAIL;
}

HRESULT G_GetThemeFont(HTHEME hTheme, OPTIONAL HDC hdc, int iPartId, 
    int iStateId, int iPropId, OUT LOGFONT *pFont)
{
	G_LoadThemeLibrary();
	GETTHEMEFONT pGetThemeFont = (GETTHEMEFONT)GetProcAddress(g_huxtheme, "GetThemeFont");
	if (pGetThemeFont != NULL)
		return (*pGetThemeFont)(hTheme, hdc, iPartId, iStateId, iPropId, pFont);
	else
		return E_FAIL;
}

// COXTaskPanel

IMPLEMENT_DYNAMIC(COXTaskPanel, CWnd)

COXTaskPanel::COXTaskPanel():m_hTheme(NULL),m_bLastHover(false),
m_ActiveItem(NULL),m_bMouseOnControl(false),pOldFocus(NULL),
m_bIgnoreButtonUp(false),m_nAnimSteps(0)
{

}

COXTaskPanel::~COXTaskPanel()
{
}


BEGIN_MESSAGE_MAP(COXTaskPanel, CWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_THEMECHANGED, OnThemeChanged)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// COXTaskPanel message handlers

BOOL COXTaskPanel::Create(RECT rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{	
	rect.right = rect.left + PDPanelFixedWidth;
	return CWnd::Create(NULL, _T("Panel"), WS_VISIBLE|WS_CHILD|WS_VSCROLL, rect, pParentWnd, nID, pContext);
}

int COXTaskPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_hTheme = G_OpenThemeData(m_hWnd, _T("EXPLORERBAR"));

	return 0;
}

void COXTaskPanel::OnClose()
{
	G_CloseThemeData(m_hTheme);

	CWnd::OnClose();
}

void COXTaskPanel::OnPaint()
{
	CPaintDC dc(this); 

	int yPos = (GetWindowLong(m_hWnd, GWL_STYLE) & WS_VSCROLL) ? GetScrollPos(SB_VERT) : 0;	
	dc.SetWindowOrg(0, yPos);

	CRect rect;
	GetClientRect(&rect);
	rect.bottom += yPos;

	CDC memdc;
	memdc.CreateCompatibleDC(&dc);	

	CBitmap bmap;
	bmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	CBitmap* pOldBitmap = memdc.SelectObject(&bmap);

	if(COXTaskPanel::IsThemed())
		G_DrawThemeBackground(m_hTheme, memdc.m_hDC, 1, 0,rect, NULL);
	else
		memdc.FillSolidRect(rect,GetSysColor(COLOR_WINDOW));

	DrawGroups(memdc.m_hDC);	

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memdc, 0, 0, SRCCOPY);	

	memdc.SelectObject(pOldBitmap);
}

BOOL COXTaskPanel::OnEraseBkgnd(CDC* /*pDC*/)
{	
	return FALSE;
}

LRESULT COXTaskPanel::OnThemeChanged(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	G_CloseThemeData(m_hTheme);
	m_hTheme = G_OpenThemeData(m_hWnd, _T("EXPLORERBAR"));
	return 1;
}

void COXTaskPanel::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* /*pScrollBar*/)
{
	switch(nSBCode)
	{
	case SB_LINEDOWN:
		nPos = GetScrollPos(SB_VERT) + 1;
		break;
	case SB_LINEUP:
		nPos = GetScrollPos(SB_VERT) - 1;
		break;
	case SB_PAGEDOWN:
		nPos = GetScrollPos(SB_VERT) + MulDiv(GetSystemMetrics(SM_CYMENU), 9, 8);
		break;
	case SB_PAGEUP:
		nPos = GetScrollPos(SB_VERT) - MulDiv(GetSystemMetrics(SM_CYMENU), 9, 8);
		break;
	case SB_TOP:
		nPos = 0;
		break;
	case SB_BOTTOM:
		nPos = GetScrollLimit(SB_VERT);
		break;
	case SB_ENDSCROLL: 
		return;
	}
	SetScrollPos(SB_VERT, nPos, TRUE);
	Invalidate();
}

BOOL COXTaskPanel::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(GetWindowLong(m_hWnd, GWL_STYLE) & WS_VSCROLL)
	{
		OnVScroll(zDelta > 0 ? SB_PAGEUP : SB_PAGEDOWN, 0, NULL);
		return TRUE;
	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void COXTaskPanel::OnMouseMove(UINT nFlags, CPoint point)
{	
	if(!m_bMouseOnControl)
	{
		m_bMouseOnControl = true;
		TRACKMOUSEEVENT tme = {0};
		tme.cbSize = sizeof TRACKMOUSEEVENT;
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		TrackMouseEvent(&tme);
		pOldFocus = SetFocus();
	}

	int yPos = (GetWindowLong(m_hWnd, GWL_STYLE) & WS_VSCROLL) ? GetScrollPos(SB_VERT) : 0;
	point.y += yPos;

	bool bFoundHover = false;
	for(vector<TaskGroup>::iterator it = m_vecGroups.begin();
		it != m_vecGroups.end(); it++)
	{		
		if(it->m_HeaderRect.PtInRect(point) )
		{
			SetCursor(AfxGetApp()->LoadOEMCursor(OCR_HAND));
			if(!it->m_bIsMouseHovered)
			{
				it->m_bIsMouseHovered = true;
				Invalidate();
			}					
			bFoundHover = true;
			break;
		}
		else
		{
			Item* pItem = NULL;
			pItem = it->FindItemWithPt(point);
			if(!it->m_bIsCollapsed && (pItem) )
			{	
				InvalidateRect(m_ActiveItem->m_ItemRect);
				m_ActiveItem = pItem;
				Invalidate();
				SetCursor(AfxGetApp()->LoadOEMCursor(OCR_HAND));
			}
			it->m_bIsMouseHovered = false;
		}	
	}	
	if(!bFoundHover)
	{
		if(m_bLastHover)
		{
			Invalidate();
			m_bLastHover = false;			
		}
	}

	point.y -= yPos;
	CWnd::OnMouseMove(nFlags, point);
}

LRESULT COXTaskPanel::OnMouseLeave(WPARAM /* wParam */, LPARAM /* lParam */)
{
	m_bMouseOnControl = false;
	if(pOldFocus)
	{
		pOldFocus->SetFocus();
		pOldFocus = NULL;
	}
	return 0;
}

void COXTaskPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	int yPos = (GetWindowLong(m_hWnd, GWL_STYLE) & WS_VSCROLL) ? GetScrollPos(SB_VERT) : 0;
	point.y += yPos;

	for(vector<TaskGroup>::iterator it = m_vecGroups.begin();
		it != m_vecGroups.end(); it++)
	{	
		// Check to see if an animation is already active for this task group
		if(it->m_bAnimationEnabled)
		{
			continue;
		}

		if(it->m_HeaderRect.PtInRect(point))
		{	
			if(it->m_bIsCollapsed)
			{				
				SetTimer(ANIMATIONTIMERID, ANIMATIONINTERVAL, NULL);
				m_nAnimSteps = NUMANIMSTEPS;
			}	
			else
			{				
				SetTimer(REVANIMATIONTIMERID, ANIMATIONINTERVAL, NULL);
				m_nAnimSteps = -NUMANIMSTEPS;
			}

			it->m_bAnimationEnabled = true;
			it->m_bIsCollapsed = !it->m_bIsCollapsed;
			Invalidate();
			m_bIgnoreButtonUp = true;
			break;
		}		
	}
	point.y -= yPos;
	CWnd::OnLButtonDown(nFlags, point);
}

void COXTaskPanel::OnLButtonUp(UINT nFlags, CPoint point)
{	
	if(!m_bIgnoreButtonUp)
	{
		int yPos = (GetWindowLong(m_hWnd, GWL_STYLE) & WS_VSCROLL) ? GetScrollPos(SB_VERT) : 0;
		point.y += yPos;

		for(vector<TaskGroup>::iterator it = m_vecGroups.begin();
			it != m_vecGroups.end(); it++)
		{
			Item* pItem = NULL;
			bool bItemValid = false;
			pItem = it->FindItemWithPt(point);
			bItemValid = !it->m_bIsCollapsed;
			if( it->m_HeaderRect.PtInRect(point) || (bItemValid && pItem ) )
			{
				if(bItemValid && pItem->m_strCmd.GetLength())
				{
					OnMouseLeave(0,0);
					ShellExecute(m_hWnd,_T("open"),pItem->m_strCmd,
						NULL,NULL,SW_SHOW);		
				}
				SetCursor(AfxGetApp()->LoadOEMCursor(OCR_HAND));
			}
		}
		point.y -= yPos;
	}
	else
	{
		m_bIgnoreButtonUp = false;
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void COXTaskPanel::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == ANIMATIONTIMERID)
	{
		if(m_nAnimSteps--)
			Invalidate();
		else
		{
			m_nAnimSteps = 0;
			KillTimer(ANIMATIONTIMERID);
		}
	}
	if(nIDEvent == REVANIMATIONTIMERID)
	{
		if(m_nAnimSteps++)
			Invalidate();
		else
		{
			m_nAnimSteps = 0;
			KillTimer(REVANIMATIONTIMERID);
		}
	}
	CWnd::OnTimer(nIDEvent);
}

bool COXTaskPanel::IsThemed()
{
	bool ret = false;
	OSVERSIONINFO ovi = {0};
	ovi.dwOSVersionInfoSize = sizeof ovi;
	GetVersionEx(&ovi);
	if( (ovi.dwMajorVersion==5 && ovi.dwMinorVersion>=1) || (ovi.dwMajorVersion>5) )
	{
		//Windows XP or later detected
		typedef BOOL WINAPI ISAPPTHEMED();
		typedef BOOL WINAPI ISTHEMEACTIVE();
		ISAPPTHEMED* pISAPPTHEMED = NULL;
		ISTHEMEACTIVE* pISTHEMEACTIVE = NULL;
		HMODULE hMod = LoadLibrary(_T("uxtheme.dll"));
		if(hMod)
		{
			pISAPPTHEMED = reinterpret_cast<ISAPPTHEMED*>(GetProcAddress(hMod,"IsAppThemed"));
			pISTHEMEACTIVE = reinterpret_cast<ISTHEMEACTIVE*>(GetProcAddress(hMod,"IsThemeActive"));
			if(pISAPPTHEMED && pISTHEMEACTIVE)
			{
				if(pISAPPTHEMED() && pISTHEMEACTIVE())                
				{                
					typedef HRESULT CALLBACK DLLGETVERSION(DLLVERSIONINFO*);
					DLLGETVERSION* pDLLGETVERSION = NULL;

					HMODULE hModComCtl = LoadLibrary(_T("comctl32.dll"));
					if(hModComCtl)
					{
						pDLLGETVERSION = reinterpret_cast<DLLGETVERSION*>(
							GetProcAddress(hModComCtl,"DllGetVersion"));
						if(pDLLGETVERSION)
						{
							DLLVERSIONINFO dvi = {0};
							dvi.cbSize = sizeof dvi;
							if(pDLLGETVERSION(&dvi) == NOERROR )
							{
								ret = dvi.dwMajorVersion >= 6;
							}
						}
						FreeLibrary(hModComCtl);                    
					}
				}
			}
			FreeLibrary(hMod);
		}
	}    
	return ret;
}
TaskGroup* COXTaskPanel::AddGroup(LPCTSTR szGroupName)
{
	m_vecGroups.push_back(TaskGroup(szGroupName));
	return &(*(m_vecGroups.end() - 1));
}

void COXTaskPanel::DrawGroupItem(HDC hDC, const CRect& recGroupItem, vector<Item>::iterator it2)
{
	it2->m_ItemRect = recGroupItem;
	HICON hIcon = (HICON)LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(it2->m_nIconID), IMAGE_ICON,16,16,LR_DEFAULTCOLOR);
	DrawIconEx(hDC,recGroupItem.left - (PDGroupItemLeftMargin-PDGroupIconLeftMargin), 
		recGroupItem.top + PDGroupIconVerticalDisplacement, hIcon, 16, 16,0, NULL,DI_NORMAL);	
	DestroyIcon(hIcon);

	if(&(*it2) == m_ActiveItem)
	{
		m_ActiveItem = NULL;
		TCHAR buff[128];
		_tcscpy_s(buff,it2->m_strText);
		int oldMode = SetBkMode(hDC, TRANSPARENT);
		COLORREF color;
		if(COXTaskPanel::IsThemed())
		{
			G_GetThemeColor(m_hTheme,5,0,TMT_TEXTCOLOR,&color);
			int shift = 48;
			color = RGB(min(255,GetRValue(color) + shift),
				min(255,GetGValue(color) + shift), min(255,GetBValue(color) + shift));
		}
		else
		{
			color = GetSysColor(COLOR_HOTLIGHT);
		}

		COLORREF oldcolor = SetTextColor(hDC, color);
		LOGFONT font;					
		G_GetThemeFont(m_hTheme, hDC,5,0,TMT_FONT,&font);
		font.lfUnderline = TRUE;
		HFONT hFont = NULL;

		if(COXTaskPanel::IsThemed())
			hFont = CreateFontIndirect(&font);
		else
			hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

		HFONT hFontOld = (HFONT)SelectObject(hDC, hFont);

		DrawTextEx(hDC,buff,it2->m_strText.GetLength(),
			(LPRECT)&recGroupItem,DT_LEFT|DT_VCENTER|DT_SINGLELINE,NULL);

		SetTextColor(hDC, oldcolor);
		if(oldMode)
			SetBkMode(hDC, oldMode);
		SelectObject(hDC,hFontOld);
		DeleteObject(hFont);
	}
	else
	{
		if(COXTaskPanel::IsThemed())
		{	
			G_DrawThemeText(m_hTheme, hDC, 5, 0, it2->m_strText, it2->m_strText.GetLength(), 
				DT_LEFT|DT_VCENTER|DT_SINGLELINE,NULL,recGroupItem);
		}
		else
		{
			HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
			LOGFONT lf = {0};
			GetObject(hFont, sizeof(lf), &lf);
			DeleteObject(hFont);
			hFont = CreateFontIndirect(&lf);
			HFONT hFontOld = (HFONT)SelectObject(hDC, hFont);
			int oldMode = SetBkMode(hDC, TRANSPARENT);
			DrawText(hDC,it2->m_strText,it2->m_strText.GetLength(),
				(RECT*)&recGroupItem,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			if(oldMode)
				SetBkMode(hDC, oldMode);
			SelectObject(hDC,hFontOld);
			DeleteObject(hFont);	
		}
	}
}

void COXTaskPanel::DrawGroupHeader(HDC hDC, const CRect& recGroupHeader, vector<TaskGroup>::iterator it)
{
	if(COXTaskPanel::IsThemed())
	{
		G_DrawThemeBackground(m_hTheme, hDC, 8, 0, recGroupHeader, NULL);
		G_DrawThemeEdge(m_hTheme, hDC, 8, 0, recGroupHeader,EDGE_SUNKEN,BF_FLAT,NULL);
	}
	else
		FillRect(hDC,recGroupHeader,(HBRUSH)(COLOR_3DFACE + 1));

	CString grpText = *it;
	CRect recGroupText(recGroupHeader);
	recGroupText.MoveToX(recGroupText.left + PDHeaderTextDisplacement);
	if(COXTaskPanel::IsThemed())
		G_DrawThemeText(m_hTheme, hDC, 8, 0, grpText, grpText.GetLength(),
			DT_LEFT|DT_VCENTER|DT_SINGLELINE,NULL,recGroupText);
	else
	{
		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		LOGFONT lf = {0};
		GetObject(hFont, sizeof(lf), &lf);
		DeleteObject(hFont);
		lf.lfWeight = FW_BOLD;
		hFont = CreateFontIndirect(&lf);
		HFONT hFontOld = (HFONT)SelectObject(hDC, hFont);
		int oldMode = SetBkMode(hDC, TRANSPARENT);
		DrawText(hDC,grpText,grpText.GetLength(),recGroupText,DT_VCENTER|DT_SINGLELINE);
		if(oldMode)
			SetBkMode(hDC, oldMode);
		SelectObject(hDC,hFontOld);
		DeleteObject(hFont);	
	}

	CRect recGroupIcon(recGroupHeader.left + PDHeaderIconDisplacement, 
		recGroupHeader.top, recGroupHeader.right, recGroupHeader.bottom);
	int iPart = 0;
	if(it->m_bIsCollapsed)
		iPart = 7;
	else
		iPart = 6;
	if(it->m_bIsMouseHovered)
	{
		if(COXTaskPanel::IsThemed())
			G_DrawThemeBackground(m_hTheme, hDC, iPart, 2, recGroupIcon, NULL);			
		else
		{
			CDC pDC;
			pDC.Attach(hDC);
			pDC.Draw3dRect(recGroupIcon,GetSysColor(COLOR_3DHILIGHT), GetSysColor(COLOR_3DSHADOW));
			pDC.Detach();
			if(it->m_bIsCollapsed)
			{
				DrawDownArrow(hDC, recGroupIcon);
			}
			else
			{
				DrawUpArrow(hDC, recGroupIcon);
			}
		}
		m_bLastHover = true;
	}
	else
	{
		if(COXTaskPanel::IsThemed())
			G_DrawThemeBackground(m_hTheme, hDC, iPart, 0, recGroupIcon, NULL);
		else
		{
			if(it->m_bIsCollapsed)
			{	
				DrawDownArrow(hDC, recGroupIcon);
			}
			else
			{
				DrawUpArrow(hDC, recGroupIcon);
			}
		}
	}
}

void COXTaskPanel::DrawDownArrow(HDC hDC, const CRect& recGroupIcon)
{
	LOGFONT lf = {0};
	_tcscpy_s(lf.lfFaceName,LF_FACESIZE, _T("Marlett"));
	lf.lfCharSet = DEFAULT_CHARSET;
	HFONT hFont = CreateFontIndirect(&lf);
	HFONT hFontOld = (HFONT)SelectObject(hDC, hFont);
	int oldMode = SetBkMode(hDC, TRANSPARENT);
	DrawText(hDC,_T("6"),1,(RECT*)&recGroupIcon,DT_CENTER|DT_VCENTER|DT_SINGLELINE);	
	if(oldMode)
		SetBkMode(hDC, oldMode);
	SelectObject(hDC,hFontOld);
	DeleteObject(hFont);	
}

void COXTaskPanel::DrawUpArrow(HDC hDC, const CRect& recGroupIcon)
{
	LOGFONT lf = {0};
	_tcscpy_s(lf.lfFaceName,LF_FACESIZE,_T("Marlett"));
	lf.lfCharSet = DEFAULT_CHARSET;
	HFONT hFont = CreateFontIndirect(&lf);
	HFONT hFontOld = (HFONT)SelectObject(hDC, hFont);
	int oldMode = SetBkMode(hDC, TRANSPARENT);
	DrawText(hDC,_T("5"),1,(RECT*)&recGroupIcon,DT_CENTER|DT_VCENTER|DT_SINGLELINE);	
	if(oldMode)
		SetBkMode(hDC, oldMode);
	SelectObject(hDC,hFontOld);
	DeleteObject(hFont);
}

void COXTaskPanel::DrawGroups(HDC hDC)
{
	CRect recGroupHeader(PDLeftMargin, PDTopMargin,PDLeftMargin + PDHeaderWidth,PDTopMargin + PDHeaderHeight);
	for(vector<TaskGroup>::iterator it = m_vecGroups.begin(); it != m_vecGroups.end(); it++)
	{
		it->m_HeaderRect = recGroupHeader;
		DrawGroupHeader(hDC, recGroupHeader, it);

		int sz = 0;

		if((!it->m_bIsCollapsed) || (it->m_bIsCollapsed && it->m_bAnimationEnabled))
		{
			sz = (int)it->m_vecItems.size();

			CRect recGroupBodySource(recGroupHeader.left, recGroupHeader.bottom,
				recGroupHeader.right, recGroupHeader.bottom + (sz * PDGroupItemHeight));

			if(it->m_bAnimationEnabled)
			{
				if(m_nAnimSteps == 0)
					it->m_bAnimationEnabled = false;
				else
				{
					if(m_nAnimSteps>0)
						sz = (int) ((float)sz / (float)max(m_nAnimSteps,1));
					else
						sz = (int) ((float)sz / (float)max(NUMANIMSTEPS + m_nAnimSteps,1));
				}
			}
			
			CRect recGroupBodyTarget(recGroupHeader.left, recGroupHeader.bottom,
				recGroupHeader.right, recGroupHeader.bottom + (sz * PDGroupItemHeight));

			HDC hDrawDC = hDC;
			HDC tmpDC = NULL;
			HBITMAP  hbmap = NULL;
			HBITMAP oldbmap = NULL;

			CRect rect;
			GetClientRect(&rect);

			if(it->m_bAnimationEnabled)
			{				
				tmpDC = CreateCompatibleDC(hDC);					
				hbmap = CreateCompatibleBitmap(hDC, rect.Width(), rect.Height());
				oldbmap = (HBITMAP)SelectObject(tmpDC,hbmap);
				hDrawDC = tmpDC;
			}

			if(!(it->m_bIsCollapsed && !it->m_bAnimationEnabled))
			{
				if(COXTaskPanel::IsThemed())
					G_DrawThemeBackground(m_hTheme, hDrawDC, 5, 0, recGroupBodySource, NULL);
				else
				{
					FillRect(hDrawDC,recGroupBodySource,(HBRUSH)(COLOR_WINDOW + 1));
					HBRUSH hBrushFrame = (HBRUSH)GetSysColorBrush(COLOR_BTNFACE);
					FrameRect(hDrawDC,recGroupBodySource,hBrushFrame);				
				}			

				CRect recGroupItem(recGroupBodySource.left + PDGroupItemLeftMargin, 
					recGroupBodySource.top,recGroupBodySource.right, recGroupBodySource.top + PDGroupItemHeight);			

				for(vector<Item>::iterator it2 = it->m_vecItems.begin();
					it2 != it->m_vecItems.end(); it2++)	
				{	
					if(!recGroupBodyTarget.PtInRect(CPoint(recGroupItem.left,recGroupItem.top)))
						break;

					DrawGroupItem(hDrawDC,recGroupItem, it2);		
					recGroupItem.MoveToY(recGroupItem.bottom);				
				}
			}	
			else
			{
				sz = 0;
			}

			if(it->m_bAnimationEnabled)
			{
				RenderAlphaBlend(hDC, tmpDC, recGroupBodyTarget, recGroupBodySource);
				SelectObject(tmpDC,oldbmap);
				DeleteObject(hbmap);
				DeleteDC(tmpDC);
			}			
		}		
		recGroupHeader.MoveToY(recGroupHeader.top + PDGroupSeparatorWidth + (sz * PDGroupItemHeight));		
	}	

	AdjustScrollBars(recGroupHeader);
}

void COXTaskPanel::AdjustScrollBars(CRect recGroupHeader)
{
	CRect rect;
	GetClientRect(&rect);	

	if(!(recGroupHeader.top + PDHeaderHeight < rect.bottom))
	{
		ShowScrollBar(SB_VERT,TRUE);		
		SCROLLINFO si = {0};
		si.cbSize = sizeof SCROLLBARINFO;
		si.fMask = SIF_PAGE|SIF_RANGE;
		si.nMin = 0;
		si.nMax = recGroupHeader.top;
		si.nPage = rect.Height();
		SetScrollInfo(SB_VERT, &si, TRUE);	
	}
	else
		ShowScrollBar(SB_VERT,FALSE);
}

bool COXTaskPanel::RenderAlphaBlend(HDC hDC, HDC tmpDC, CRect recGroupBodyTarget, CRect recGroupBodySource)
{
	BLENDFUNCTION bf = {0};
	bf.BlendOp = AC_SRC_OVER;
	if(m_nAnimSteps>0)
		bf.SourceConstantAlpha = (BYTE)((float)255 * (float)(NUMANIMSTEPS-m_nAnimSteps)/(float)NUMANIMSTEPS);
	else
		bf.SourceConstantAlpha = (BYTE)((float)255 * (float)(-m_nAnimSteps)/(float)NUMANIMSTEPS);
	bf.SourceConstantAlpha = min(bf.SourceConstantAlpha, 128);
	return !(FALSE == AlphaBlend(hDC,recGroupBodyTarget.left,recGroupBodyTarget.top,
		recGroupBodyTarget.Width(),recGroupBodyTarget.Height(),
		tmpDC,recGroupBodySource.left,recGroupBodySource.top,
		recGroupBodySource.Width(),recGroupBodySource.Height(),bf));	
}