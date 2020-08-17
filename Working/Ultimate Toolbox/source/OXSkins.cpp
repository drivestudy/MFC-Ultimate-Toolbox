// ==========================================================================
//					Class Implementation : COX*Skin
// ==========================================================================
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
// Version: 9.3

#include "stdafx.h"
#include "OXSkins.h"
#include "OXCoolToolBar.h"
#include "OXMenuBar.h"

#include "OXSizeDockBar.h"
#include "OXSzMiniDockFrmWnd.h"
#include "OXFrameWndDock.h"
#include "OXTabClientWnd.h"
#include "XStatus4.h"
#include "OXSplitterRect.h"
#include "OXShortcutBar.h"

BOOL IsThemed()
{
	BOOL ret = FALSE;
	OSVERSIONINFO ovi = {0};
	ovi.dwOSVersionInfoSize = sizeof ovi;
	GetVersionEx(&ovi);
	if(ovi.dwMajorVersion==5 && ovi.dwMinorVersion==1)
	{
		//Windows XP detected
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

// COXDockbarSkin
//


// COXDockbarSkinClassic
//

COXDockbarSkinClassic::COXDockbarSkinClassic()
{
	m_fontCaption.CreatePointFont(100, _T("Arial"));
	m_fontCaptionSolid.CreatePointFont(80, _T("Arial"));

	// Init the control bar constants
	m_ControlBarConstants.iButtonGap = 2;
}

COXDockbarSkinClassic::~COXDockbarSkinClassic()
{
}

void COXDockbarSkinClassic::DrawBackground(CDC* pDC, COXSizeDockBar* pSizeDockBar)
{
	CRect rectClient;
	pSizeDockBar->GetClientRect(rectClient);
	pDC->FillSolidRect(rectClient, ::GetSysColor(COLOR_3DFACE));
}

void COXDockbarSkinClassic::DrawGripper(CDC* pDC, COXSizeControlBar* pSizeControlBar)
{
    if(!pSizeControlBar->IsGripper() || (pSizeControlBar->m_dwStyle & CBRS_FLOATING))
	{
		return;
	}

	BOOL bHorz=(pSizeControlBar->m_dwStyle&CBRS_ORIENT_HORZ)==0;

	CRect rectWindow;
	pSizeControlBar->GetWindowRect(rectWindow);
	pSizeControlBar->ScreenToClient(rectWindow);
	CRect gripper=pSizeControlBar->m_rectGripper;
	gripper.OffsetRect(-rectWindow.left, -rectWindow.top);

	if(pSizeControlBar->IsSolidGripper())
	{
		CBrush brush((pSizeControlBar->IsActive() ? ::GetSysColor(COLOR_ACTIVECAPTION) : 
			::GetSysColor(COLOR_INACTIVECAPTION)));
		pDC->FillRect(gripper,&brush);
	}
	else
	{
		gripper.DeflateRect(2,2);
	}

	CString sBarTitle;
	pSizeControlBar->GetWindowText(sBarTitle);
	CRect rectText=CRect(0,0,0,0);

	//draw text
	if(pSizeControlBar->IsCaption() && !sBarTitle.IsEmpty())
	{
		rectText=gripper;
		if(!bHorz)
		{
			rectText.bottom-=ID_TEXT_OFFSET;
		}
		else
		{
			rectText.left+=ID_TEXT_OFFSET;
		}

		COLORREF oldColor;
		if(pSizeControlBar->IsSolidGripper())
		{
			oldColor=pDC->SetTextColor((pSizeControlBar->IsActive() ? 
				::GetSysColor(COLOR_CAPTIONTEXT) : 
				::GetSysColor(COLOR_INACTIVECAPTIONTEXT)));
		}
		else
		{
			oldColor=pDC->SetTextColor((pSizeControlBar->IsActive() ? 
				::GetSysColor(COLOR_ACTIVECAPTION) : 
				::GetSysColor(COLOR_INACTIVECAPTION)));
		}

		CFont* pOldFont=NULL;
		CFont fontVert;
		if(!bHorz)
		{
			// setup font for vertically oriented mode
			LOGFONT lf;
			if (pSizeControlBar->IsSolidGripper())
				VERIFY(m_fontCaptionSolid.GetLogFont(&lf));
			else
				VERIFY(m_fontCaption.GetLogFont(&lf));
			lf.lfEscapement=900;
			lf.lfOrientation=900;
			VERIFY(fontVert.CreateFontIndirect(&lf));
			pOldFont=pDC->SelectObject(&fontVert);
		}
		else
		{
			if (pSizeControlBar->IsSolidGripper())
				pOldFont=pDC->SelectObject(&m_fontCaptionSolid);
			else
				pOldFont=pDC->SelectObject(&m_fontCaption);
		}

		CRect rectHelper=rectText;
		if(!bHorz)
		{
			// adjust rectangle to display vertical text
			rectText.top=rectHelper.left;
			rectText.bottom=rectHelper.right;
			rectText.left=rectHelper.top;
			rectText.right=rectHelper.bottom;
		}

		// calculate the rect to display text in
		UINT nFormat=DT_LEFT|DT_SINGLELINE;
		pDC->DrawText(sBarTitle,&rectText,nFormat|DT_CALCRECT);
		if(!bHorz)
		{
			if(rectText.Width()>rectHelper.Height())
				rectText.right=rectText.left+rectHelper.Height();
			rectHelper=rectText;

			rectText.top=rectHelper.left;
			rectText.bottom=rectHelper.right;
			rectText.left=rectHelper.top;
			rectText.right=rectHelper.bottom;
			rectHelper=rectText;
		}
		else
		{
			if(rectText.Width()>rectHelper.Width())
				rectText.right=rectText.left+rectHelper.Width();
			rectHelper=rectText;
		}

		// adjust coordinates
		if(!bHorz)
		{
			rectText.left+=(gripper.Width()-rectHelper.Width())/2+
				(gripper.Width()-rectHelper.Width())%2;
			rectText.right=rectText.left+rectHelper.Width();
		}
		else
		{
			rectText.top+=(gripper.Height()-rectHelper.Height())/2+
				(gripper.Height()-rectHelper.Height())%2;
			rectText.bottom=rectText.top+rectHelper.Height();
		}

		if(!bHorz)
		{
			rectText.bottom=gripper.bottom-ID_TEXT_OFFSET;
			rectText.top=rectText.bottom-rectHelper.Height();
		}
		else
		{
			rectText.left=rectText.left;
			rectText.right=rectText.left+rectHelper.Width();
		}

		rectHelper=rectText;
		if(!bHorz)
		{
			rectHelper.bottom+=rectText.Width();
			rectHelper.right+=rectText.Height();
		}

		int nOldBkMode=0;
		if(pSizeControlBar->IsSolidGripper())
		{
			nOldBkMode=pDC->SetBkMode(TRANSPARENT);
		}
		// draw text
		pDC->DrawText(sBarTitle,&rectHelper,
			DT_BOTTOM|DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);
		if(pSizeControlBar->IsSolidGripper())
		{
			pDC->SetBkMode(nOldBkMode);
		}

		pDC->SetTextColor(oldColor);
		if(pOldFont!=NULL)
		{
			pDC->SelectObject(pOldFont);
		}
	}


	if(!pSizeControlBar->IsSolidGripper())
	{
		// paint the gripper
		if(!bHorz) 
		{
			if(!rectText.IsRectEmpty())
			{
				gripper.bottom=rectText.top-ID_TEXTGRIPPER_MARGIN;
			}
			
			// gripper at left
			if(gripper.bottom>gripper.top && gripper.Height()>=ID_GRIPPER_MINSIZE)
			{
				gripper.right = gripper.left + 3;
				pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
					::GetSysColor(COLOR_BTNSHADOW));
				gripper.OffsetRect(4, 0);
				pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
					::GetSysColor(COLOR_BTNSHADOW));
			}
		}
		else 
		{
			if(!rectText.IsRectEmpty())
			{
				gripper.left=rectText.right+ID_TEXTGRIPPER_MARGIN;
			}

			// gripper at top
			if(gripper.right>gripper.left && gripper.Width()>=ID_GRIPPER_MINSIZE)
			{
				gripper.bottom = gripper.top + 3;
				pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
					::GetSysColor(COLOR_BTNSHADOW));
				gripper.OffsetRect(0, 4);
				pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
					::GetSysColor(COLOR_BTNSHADOW));
			}
		}
	}
}

void COXDockbarSkinClassic::DrawCloseButton(CDC* pDC, COXSizeControlBar* pSizeControlBar)
{
    if(!pSizeControlBar->IsCloseBtn() || (pSizeControlBar->m_dwStyle & CBRS_FLOATING))
		return;

	// draw close button
    CRect rectWindow;
	pSizeControlBar->GetWindowRect(rectWindow);
	pSizeControlBar->ScreenToClient(rectWindow);
	CRect rcButton=pSizeControlBar->m_rectCloseBtn;
	rcButton.OffsetRect(-rectWindow.left, -rectWindow.top);

	// Draw to a memory dc to avoid flashing
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap bmpMem;
	CRect rectMem(rcButton);
	rectMem.OffsetRect(-rectMem.left, -rectMem.top);
	bmpMem.CreateCompatibleBitmap(pDC, rectMem.Width(), rectMem.Height());
	CBitmap* pOldBmp = dc.SelectObject(&bmpMem);

	dc.DrawFrameControl(CRect(0, 0, rcButton.Width(), rcButton.Height()), DFC_CAPTION,DFCS_CAPTIONCLOSE|
		(pSizeControlBar->m_pressedBtn==COXSizeControlBar::CLOSEBTN ? DFCS_PUSHED : 0));
	pDC->BitBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), &dc, 0, 0, SRCCOPY);
	dc.SelectObject(pOldBmp);
}

void COXDockbarSkinClassic::DrawResizeButton(CDC* pDC, COXSizeControlBar* pSizeControlBar)
{
    if(!pSizeControlBar->IsResizeBtn() || (pSizeControlBar->m_dwStyle & CBRS_FLOATING))
		return;

	// draw close button
    CRect rectWindow;
	pSizeControlBar->GetWindowRect(rectWindow);
	pSizeControlBar->ScreenToClient(rectWindow);
	CRect rcButton=pSizeControlBar->m_rectResizeBtn;
	rcButton.OffsetRect(-rectWindow.left, -rectWindow.top);

	// Draw to a memory dc to avoid flashing
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap bmpMem;
	CRect rectMem(rcButton);
	rectMem.OffsetRect(-rectMem.left, -rectMem.top);
	bmpMem.CreateCompatibleBitmap(pDC, rectMem.Width(), rectMem.Height());
	CBitmap* pOldBmp = dc.SelectObject(&bmpMem);

	dc.DrawFrameControl(CRect(0, 0, rcButton.Width(), rcButton.Height()),DFC_CAPTION,
		(pSizeControlBar->m_bMaximized ? DFCS_CAPTIONMIN : DFCS_CAPTIONMAX)|
		(pSizeControlBar->m_pressedBtn==COXSizeControlBar::RESIZEBTN ? DFCS_PUSHED : 0)|
		(pSizeControlBar->CanResize() ? 0 : DFCS_INACTIVE));
	pDC->BitBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), &dc, 0, 0, SRCCOPY);
	dc.SelectObject(pOldBmp);
}

void COXDockbarSkinClassic::DrawNonClientArea(CDC* pDC, LPCRECT lpRect, COXSizeControlBar* pSizeControlBar)
{
	CRect rect(lpRect);
	pSizeControlBar->DrawBorders(pDC, rect);
	pDC->FillSolidRect(lpRect, ::GetSysColor(COLOR_BTNFACE));
}

void COXDockbarSkinClassic::DrawSplitter(CDC* pDC, COXSplitterRect* pSplitterRect, COXSizeDockBar* /*pSizeDockBar*/)
{
    CRect rect=pSplitterRect->m_rect;
    switch(pSplitterRect->m_type)
	{
    case SPLITTER_VERT:
		{
			rect.left++;
			pDC->FillSolidRect(rect.left, rect.top, 1, rect.Height(), 
				::GetSysColor(COLOR_BTNFACE));
			rect.left++;
			pDC->FillSolidRect(rect.left, rect.top, 1, rect.Height(), 
				::GetSysColor(COLOR_BTNHILIGHT));
			
			rect.right--;
			pDC->FillSolidRect(rect.right, rect.top, 1, rect.Height(), 
				::GetSysColor(COLOR_WINDOWFRAME));
			rect.right--;
			pDC->FillSolidRect(rect.right, rect.top, 1, rect.Height(), 
				::GetSysColor(COLOR_BTNSHADOW));
		}
		break;
	
    case SPLITTER_HORZ:
		{
			rect.top++;
			pDC->FillSolidRect(rect.left, rect.top, rect.Width(), 1, 
				::GetSysColor(COLOR_BTNFACE));
			rect.top++;
			pDC->FillSolidRect(rect.left, rect.top, rect.Width(), 1, 
				::GetSysColor(COLOR_BTNHILIGHT));
			
			rect.bottom--;
			pDC->FillSolidRect(rect.left, rect.bottom, rect.Width(), 1,
				::GetSysColor(COLOR_WINDOWFRAME));
			rect.bottom--;
			pDC->FillSolidRect(rect.left, rect.bottom, rect.Width(), 1, 
				::GetSysColor(COLOR_BTNSHADOW));

		}
        break;
		
	default:
        ASSERT(FALSE);
        break;
	}
}

COLORREF COXDockbarSkinClassic::GetClientBorderColor()
{
	return ::GetSysColor(COLOR_WINDOWFRAME);
}

void COXDockbarSkinClassic::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* lpncsp, COXSizeControlBar* pSizeControlBar)
{
	CRect rect=lpncsp->rgrc[0];
	rect.DeflateRect(ID_CONTAINER_GAP, ID_CONTAINER_GAP);

	if(pSizeControlBar->IsGripper() && !(pSizeControlBar->m_dwStyle & CBRS_FLOATING))
	{
		if(pSizeControlBar->m_dwStyle&CBRS_ORIENT_HORZ) 
		{
			rect.DeflateRect(ID_CONTAINER_GAP,0,0,0);
			rect.left+=ID_BUTTON_SIDE;
			if(rect.left>rect.right)
			{
				rect.left=rect.right;
			}
		} 
		else 
		{
			rect.DeflateRect(0,ID_CONTAINER_GAP,0,0);
			rect.top+=ID_BUTTON_SIDE;
			if(rect.top>rect.bottom)
			{
				rect.top=rect.bottom;
			}
		}
		pSizeControlBar->m_bDelayRecalcLayout=TRUE;
	}

	lpncsp->rgrc[0]=rect;
}

void COXDockbarSkinClassic::RecalcLayout(COXSizeControlBar* pSizeControlBar)
{
	CRect rectWindow;
	pSizeControlBar->GetWindowRect(rectWindow);
	pSizeControlBar->ScreenToClient(rectWindow);
	rectWindow.DeflateRect(ID_CONTAINER_GAP, ID_CONTAINER_GAP);
	pSizeControlBar->m_rectGripper=rectWindow;
    
	if(pSizeControlBar->IsGripper() && !(pSizeControlBar->m_dwStyle & CBRS_FLOATING))
	{
		if(pSizeControlBar->m_dwStyle&CBRS_ORIENT_HORZ) 
		{
			pSizeControlBar->m_rectGripper.right=pSizeControlBar->m_rectGripper.left+ID_BUTTON_SIDE;
			
			if(pSizeControlBar->IsCloseBtn())
			{
				pSizeControlBar->m_rectCloseBtn=pSizeControlBar->m_rectGripper;
				pSizeControlBar->m_rectCloseBtn.bottom=pSizeControlBar->m_rectCloseBtn.top+ID_BUTTON_SIDE;
				pSizeControlBar->m_rectGripper.top+=ID_BUTTON_SIDE+GetControlBarConstants().iButtonGap;
			}

			if(pSizeControlBar->IsResizeBtn())
			{
				pSizeControlBar->m_rectResizeBtn=pSizeControlBar->m_rectGripper;
				pSizeControlBar->m_rectResizeBtn.bottom=pSizeControlBar->m_rectResizeBtn.top+ID_BUTTON_SIDE;
				pSizeControlBar->m_rectGripper.top+=ID_BUTTON_SIDE+GetControlBarConstants().iButtonGap;
			}
		} 
		else 
		{
			pSizeControlBar->m_rectGripper.bottom=pSizeControlBar->m_rectGripper.top+ID_BUTTON_SIDE;
			
			if(pSizeControlBar->IsCloseBtn())
			{
				pSizeControlBar->m_rectCloseBtn=pSizeControlBar->m_rectGripper;
				pSizeControlBar->m_rectCloseBtn.left=pSizeControlBar->m_rectCloseBtn.right-ID_BUTTON_SIDE;
				pSizeControlBar->m_rectGripper.right-=ID_BUTTON_SIDE+GetControlBarConstants().iButtonGap;
			}

			if(pSizeControlBar->IsResizeBtn())
			{
				pSizeControlBar->m_rectResizeBtn=pSizeControlBar->m_rectGripper;
				pSizeControlBar->m_rectResizeBtn.left=pSizeControlBar->m_rectResizeBtn.right-ID_BUTTON_SIDE;
				pSizeControlBar->m_rectGripper.right-=ID_BUTTON_SIDE+GetControlBarConstants().iButtonGap;
			}
		}
	}

	pSizeControlBar->m_bDelayRecalcLayout=FALSE;
}


// COXDockbarSkinXP
//

COXDockbarSkinXP::COXDockbarSkinXP(COXSkinXP* pSkinXP)
{
	m_pSkinXP = pSkinXP;
	m_fontCaption.CreatePointFont(85, _T("Tahoma BOLD"));

	// Init the control bar constants
	m_ControlBarConstants.iButtonGap = 3;
}

COXDockbarSkinXP::~COXDockbarSkinXP()
{
}

void COXDockbarSkinXP::DrawBackground(CDC* pDC, COXSizeDockBar* pSizeDockBar)
{
	CRect rectClient;
	pSizeDockBar->GetClientRect(rectClient);
	pDC->FillSolidRect(rectClient, ::GetSysColor(COLOR_3DFACE));
}

void COXDockbarSkinXP::DrawGripper(CDC* pDC, COXSizeControlBar* pSizeControlBar)
{
    if(!pSizeControlBar->IsGripper() || (pSizeControlBar->m_dwStyle & CBRS_FLOATING))
	{
		return;
	}

	BOOL bHorz=(pSizeControlBar->m_dwStyle&CBRS_ORIENT_HORZ)==0;

	CRect rectWindow;
	pSizeControlBar->GetWindowRect(rectWindow);
	pSizeControlBar->ScreenToClient(rectWindow);
	CRect gripper=pSizeControlBar->m_rectGripper;

	// If this is a solid griper extend it to include the buttons
	if (pSizeControlBar->IsSolidGripper())
	{
		gripper.UnionRect(gripper, pSizeControlBar->m_rectCloseBtn);
		gripper.UnionRect(gripper, pSizeControlBar->m_rectResizeBtn);
	}
	gripper.InflateRect(3, 3);	
	gripper.OffsetRect(-rectWindow.left, -rectWindow.top);

	if(pSizeControlBar->IsSolidGripper())
	{
		CBrush brush(::GetSysColor(COLOR_INACTIVECAPTION));
		CRect rectCaption(gripper);
		if (bHorz)
			rectCaption.DeflateRect(1, 2, 1, 1);
		else
			rectCaption.DeflateRect(2, 2, 1, 1);
		pDC->FillRect(rectCaption, &brush);
	}
	else
	{
		if (bHorz)
			gripper.DeflateRect(4, 2);
		else
			gripper.DeflateRect(2, 0);
	}

	CString sBarTitle;
	pSizeControlBar->GetWindowText(sBarTitle);
	CRect rectText=CRect(0,0,0,0);

	//draw text
	if(pSizeControlBar->IsCaption() && !sBarTitle.IsEmpty())
	{
		rectText=gripper;

		if (pSizeControlBar->IsSolidGripper())
		{
			if(!bHorz)
			{
				rectText.bottom-=4;
			}
			else
			{
				rectText.left+=4;
			}
		}

		COLORREF oldColor;
		if(pSizeControlBar->IsSolidGripper())
		{
			oldColor=pDC->SetTextColor(::GetSysColor(COLOR_CAPTIONTEXT));
		}
		else
		{
			oldColor=pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		}

		CFont* pOldFont=NULL;
		CFont fontVert;
		if(!bHorz)
		{
			// setup font for vertically oriented mode
			LOGFONT lf;
			VERIFY(m_fontCaption.GetLogFont(&lf));
			lf.lfEscapement=900;
			lf.lfOrientation=900;
			VERIFY(fontVert.CreateFontIndirect(&lf));
			pOldFont=pDC->SelectObject(&fontVert);
		}
		else
		{
			pOldFont=pDC->SelectObject(&m_fontCaption);
		}

		CRect rectHelper=rectText;
		if(!bHorz)
		{
			// adjust rectangle to display vertical text
			rectText.top=rectHelper.left;
			rectText.bottom=rectHelper.right;
			rectText.left=rectHelper.top;
			rectText.right=rectHelper.bottom;
		}

		// calculate the rect to display text in
		UINT nFormat=DT_LEFT|DT_SINGLELINE;
		pDC->DrawText(sBarTitle,&rectText,nFormat|DT_CALCRECT);
		if(!bHorz)
		{
			if(rectText.Width()>rectHelper.Height())
				rectText.right=rectText.left+rectHelper.Height();
			rectHelper=rectText;

			rectText.top=rectHelper.left;
			rectText.bottom=rectHelper.right;
			rectText.left=rectHelper.top;
			rectText.right=rectHelper.bottom;
			rectHelper=rectText;
		}
		else
		{
			if(rectText.Width()>rectHelper.Width())
				rectText.right=rectText.left+rectHelper.Width();
			rectHelper=rectText;
		}

		// adjust coordinates
		if(!bHorz)
		{
			rectText.left+=(gripper.Width()-rectHelper.Width())/2+
				(gripper.Width()-rectHelper.Width())%2;
			rectText.right=rectText.left+rectHelper.Width();
		}
		else
		{
			rectText.top+=(gripper.Height()-rectHelper.Height())/2+
				(gripper.Height()-rectHelper.Height())%2;
			rectText.bottom=rectText.top+rectHelper.Height();
		}

		if(!bHorz)
		{
			rectText.bottom=gripper.bottom-ID_TEXT_OFFSET;
			rectText.top=rectText.bottom-rectHelper.Height();
		}
		else
		{
			rectText.left=rectText.left;
			rectText.right=rectText.left+rectHelper.Width();
		}

		rectHelper=rectText;
		if(!bHorz)
		{
			rectHelper.bottom+=rectText.Width();
			rectHelper.right+=rectText.Height();
		}

		int nOldBkMode=0;
		if(pSizeControlBar->IsSolidGripper())
		{
			nOldBkMode=pDC->SetBkMode(TRANSPARENT);
		}
		// draw text
		pDC->DrawText(sBarTitle,&rectHelper,
			DT_BOTTOM|DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);
		if(pSizeControlBar->IsSolidGripper())
		{
			pDC->SetBkMode(nOldBkMode);
		}

		pDC->SetTextColor(oldColor);
		if(pOldFont!=NULL)
		{
			pDC->SelectObject(pOldFont);
		}
	}


	if(!pSizeControlBar->IsSolidGripper())
	{
		// paint the gripper

		CPen pen;
		pen.CreatePen(PS_SOLID, 1, m_pSkinXP->GetGripperColor());
		CPen* pOldPen = pDC->SelectObject(&pen);

		if(!bHorz) 
		{
			if(!rectText.IsRectEmpty())
			{
				gripper.bottom=rectText.top-ID_TEXTGRIPPER_MARGIN;
			}
			
			// gripper at left
			if(gripper.bottom>gripper.top && gripper.Height()>=ID_GRIPPER_MINSIZE)
			{
				gripper.left += 3;
				gripper.bottom -= 1;
				for (int i = 0; i < 4; i++)
				{
					pDC->MoveTo(gripper.left + i * 2, gripper.top);
					pDC->LineTo(gripper.left + i * 2, gripper.bottom);
				}
			}
		}
		else 
		{
			if(!rectText.IsRectEmpty())
			{
				gripper.left=rectText.right+ID_TEXTGRIPPER_MARGIN;
			}

			// gripper at top
			if(gripper.right>gripper.left && gripper.Width()>=ID_GRIPPER_MINSIZE)
			{
				gripper.top += 3;
				gripper.right -= 1;
				for (int i = 0; i < 4; i++)
				{
					pDC->MoveTo(gripper.left, gripper.top + i * 2);
					pDC->LineTo(gripper.right, gripper.top + i * 2);
				}
			}
		}

		pDC->SelectObject(pOldPen);
	}
}

void COXDockbarSkinXP::DrawCloseButton(CDC* pDC, COXSizeControlBar* pSizeControlBar)
{
    if(!pSizeControlBar->IsCloseBtn() || (pSizeControlBar->m_dwStyle & CBRS_FLOATING))
		return;

	// Determine the button rectangle
    CRect rectWindow;
	pSizeControlBar->GetWindowRect(rectWindow);
	pSizeControlBar->ScreenToClient(rectWindow);
	CRect rect = pSizeControlBar->m_rectCloseBtn;
	rect.OffsetRect(-rectWindow.left, -rectWindow.top);
	rect.InflateRect(1, 1, 2, 2);

	if (pSizeControlBar->IsSolidGripper())
		m_pSkinXP->DrawFrameButton(pDC, rect, DFCS_CAPTIONCLOSE, FALSE, TRUE,
			::GetSysColor(COLOR_INACTIVECAPTION), pSizeControlBar);
	else
		m_pSkinXP->DrawFrameButton(pDC, rect, DFCS_CAPTIONCLOSE, FALSE, FALSE,
			::GetSysColor(COLOR_3DFACE), pSizeControlBar);
}

void COXDockbarSkinXP::DrawResizeButton(CDC* pDC, COXSizeControlBar* pSizeControlBar)
{
    if(!pSizeControlBar->IsCloseBtn() || (pSizeControlBar->m_dwStyle & CBRS_FLOATING))
		return;

	// Determine the button rectangle
    CRect rectWindow;
	pSizeControlBar->GetWindowRect(rectWindow);
	pSizeControlBar->ScreenToClient(rectWindow);
	CRect rect = pSizeControlBar->m_rectResizeBtn;
	rect.OffsetRect(-rectWindow.left, -rectWindow.top);
	rect.InflateRect(1, 1, 2, 2);

	// Determine the flags
	UINT nState = pSizeControlBar->m_bMaximized ? DFCS_CAPTIONMIN : DFCS_CAPTIONMAX;
	BOOL bDisabled = !pSizeControlBar->CanResize();

	if (pSizeControlBar->IsSolidGripper())
		m_pSkinXP->DrawFrameButton(pDC, rect, nState, bDisabled, TRUE,
			::GetSysColor(COLOR_INACTIVECAPTION), pSizeControlBar);
	else
		m_pSkinXP->DrawFrameButton(pDC, rect, nState, bDisabled, FALSE,
			::GetSysColor(COLOR_3DFACE), pSizeControlBar);
}

void COXDockbarSkinXP::DrawNonClientArea(CDC* pDC, LPCRECT lpRect, COXSizeControlBar* /*pSizeControlBar*/)
{
	pDC->FillSolidRect(lpRect, ::GetSysColor(COLOR_3DFACE));
}

void COXDockbarSkinXP::DrawSplitter(CDC* pDC, COXSplitterRect* pSplitterRect, COXSizeDockBar* /*pSizeDockBar*/)
{
    CRect rect(pSplitterRect->m_rect);

	if (rect.bottom > 8000)
		rect.bottom = 8000;

	pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));
	
	if (pSplitterRect->m_type == SPLITTER_VERT)
	{
		rect.left = pSplitterRect->m_rect.CenterPoint().x;
		rect.right = rect.left + 1;
	}
	else
	{
		rect.top = pSplitterRect->m_rect.CenterPoint().y;
		rect.bottom = rect.top + 1;
	}

	pDC->FillSolidRect(rect, m_pSkinXP->GetSeparatorColor());
}

COLORREF COXDockbarSkinXP::GetClientBorderColor()
{
	return m_pSkinXP->GetBorderColor();
}

void COXDockbarSkinXP::OnNcPaintSizeDockBar(COXSizeDockBar* pSizeDockBar)
{
	CWindowDC dc(pSizeDockBar);
	CRect rectWindow;
	pSizeDockBar->GetWindowRect(rectWindow);
	CPoint ptOffset = rectWindow.TopLeft();
	rectWindow.OffsetRect(-ptOffset.x, -ptOffset.y);

	// Get the client rectangle in window coordionates
	CRect rectClient;
	pSizeDockBar->GetClientRect(rectClient);
	pSizeDockBar->ClientToScreen(rectClient);
	rectClient.OffsetRect(-ptOffset.x, -ptOffset.y);
	dc.ExcludeClipRect(rectClient);

	dc.FillSolidRect(rectWindow, m_pSkinXP->GetDockbarBackgroundColor());
}

void COXDockbarSkinXP::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* lpncsp, COXSizeControlBar* pSizeControlBar)
{
	CRect rect=lpncsp->rgrc[0];
	rect.DeflateRect(ID_CONTAINER_GAP, ID_CONTAINER_GAP);

	if(pSizeControlBar->IsGripper() && !(pSizeControlBar->m_dwStyle & CBRS_FLOATING))
	{
		if(pSizeControlBar->m_dwStyle&CBRS_ORIENT_HORZ) 
		{
			rect.DeflateRect(ID_CONTAINER_GAP,0,0,0);
			rect.left+=ID_BUTTON_SIDE;
			if(rect.left>rect.right)
			{
				rect.left=rect.right;
			}
		} 
		else 
		{
			rect.DeflateRect(0,ID_CONTAINER_GAP,0,0);
			rect.top+=ID_BUTTON_SIDE;
			if(rect.top>rect.bottom)
			{
				rect.top=rect.bottom;
			}
		}
		pSizeControlBar->m_bDelayRecalcLayout=TRUE;
	}

	lpncsp->rgrc[0]=rect;
}

void COXDockbarSkinXP::RecalcLayout(COXSizeControlBar* pSizeControlBar)
{
	CRect rectWindow;
	pSizeControlBar->GetWindowRect(rectWindow);
	pSizeControlBar->ScreenToClient(rectWindow);
	rectWindow.DeflateRect(ID_CONTAINER_GAP, ID_CONTAINER_GAP);
	pSizeControlBar->m_rectGripper=rectWindow;
    
	if(pSizeControlBar->IsGripper() && !(pSizeControlBar->m_dwStyle & CBRS_FLOATING))
	{
		if(pSizeControlBar->m_dwStyle&CBRS_ORIENT_HORZ) 
		{
			pSizeControlBar->m_rectGripper.right=pSizeControlBar->m_rectGripper.left+ID_BUTTON_SIDE;
			
			if(pSizeControlBar->IsCloseBtn())
			{
				pSizeControlBar->m_rectCloseBtn=pSizeControlBar->m_rectGripper;
				pSizeControlBar->m_rectCloseBtn.bottom=pSizeControlBar->m_rectCloseBtn.top+ID_BUTTON_SIDE;
				pSizeControlBar->m_rectGripper.top+=ID_BUTTON_SIDE+GetControlBarConstants().iButtonGap;
			}

			if(pSizeControlBar->IsResizeBtn())
			{
				pSizeControlBar->m_rectResizeBtn=pSizeControlBar->m_rectGripper;
				pSizeControlBar->m_rectResizeBtn.bottom=pSizeControlBar->m_rectResizeBtn.top+ID_BUTTON_SIDE;
				pSizeControlBar->m_rectGripper.top+=ID_BUTTON_SIDE+GetControlBarConstants().iButtonGap;
			}
		} 
		else 
		{
			pSizeControlBar->m_rectGripper.bottom=pSizeControlBar->m_rectGripper.top+ID_BUTTON_SIDE;
			
			if(pSizeControlBar->IsCloseBtn())
			{
				pSizeControlBar->m_rectCloseBtn=pSizeControlBar->m_rectGripper;
				pSizeControlBar->m_rectCloseBtn.left=pSizeControlBar->m_rectCloseBtn.right-ID_BUTTON_SIDE;
				pSizeControlBar->m_rectGripper.right-=ID_BUTTON_SIDE+GetControlBarConstants().iButtonGap;
			}

			if(pSizeControlBar->IsResizeBtn())
			{
				pSizeControlBar->m_rectResizeBtn=pSizeControlBar->m_rectGripper;
				pSizeControlBar->m_rectResizeBtn.left=pSizeControlBar->m_rectResizeBtn.right-ID_BUTTON_SIDE;
				pSizeControlBar->m_rectGripper.right-=ID_BUTTON_SIDE+GetControlBarConstants().iButtonGap;
			}
		}
	}

	pSizeControlBar->m_bDelayRecalcLayout=FALSE;
}

// COXDockbarSkin2003
//

COXDockbarSkin2003::COXDockbarSkin2003(COXSkin2003* pSkin2003) :
	COXDockbarSkinXP(pSkin2003)
{
}

COXDockbarSkin2003::~COXDockbarSkin2003()
{
}

void COXDockbarSkin2003::DrawBackground(CDC* pDC, COXSizeDockBar* pSizeDockBar)
{
	CRect rectClient;
	pSizeDockBar->GetClientRect(rectClient);

	int iWidth = ::GetSystemMetrics(SM_CXSCREEN);
	if (iWidth < rectClient.Width())
		iWidth = rectClient.Width();
	
	rectClient.right = rectClient.left + iWidth;

	CRect rectSolid(rectClient);
	rectClient.right = rectClient.CenterPoint().x;
	rectSolid.left = rectClient.right;

	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;
	pSkin2003->GradientFill(pDC, rectClient,
		pSkin2003->GetDockbarLeftColor(),
		pSkin2003->GetDockbarRightColor(),
		GRADIENT_FILL_RECT_H);
	pDC->FillSolidRect(rectSolid, pSkin2003->GetDockbarRightColor());
}

void COXDockbarSkin2003::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* lpncsp, COXSizeControlBar* pSizeControlBar)
{
	CRect rect=lpncsp->rgrc[0];
	rect.DeflateRect(ID_CONTAINER_GAP, ID_CONTAINER_GAP);

	if(pSizeControlBar->IsGripper() && !(pSizeControlBar->m_dwStyle & CBRS_FLOATING))
	{
		if(pSizeControlBar->m_dwStyle&CBRS_ORIENT_HORZ) 
		{
			rect.DeflateRect(ID_O11_GRIPPER + ID_BUTTON_SIDE,0,0,0);
			if(rect.left>rect.right)
			{
				rect.left=rect.right;
			}
		} 
		else 
		{
			rect.DeflateRect(0,ID_O11_GRIPPER + ID_BUTTON_SIDE,0,0);
			if(rect.top>rect.bottom)
			{
				rect.top=rect.bottom;
			}
		}
		pSizeControlBar->m_bDelayRecalcLayout=TRUE;
	}

	lpncsp->rgrc[0]=rect;
}

void COXDockbarSkin2003::DrawNonClientArea(CDC* pDC, LPCRECT lpRect, COXSizeControlBar* /*pSizeControlBar*/)
{
	pDC->FillSolidRect(lpRect, m_pSkinXP->GetDockbarBackgroundColor());
}

void COXDockbarSkin2003::DrawGripper(CDC* pDC, COXSizeControlBar* pSizeControlBar)
{
    if(!pSizeControlBar->IsGripper() || (pSizeControlBar->m_dwStyle & CBRS_FLOATING))
	{
		return;
	}

    CRect rectWindow;
	pSizeControlBar->GetWindowRect(rectWindow);
	pSizeControlBar->ScreenToClient(rectWindow);
	CPoint ptOffset(-rectWindow.left, -rectWindow.top);
	rectWindow.OffsetRect(ptOffset);

	CRect rectGripper;
	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;

	if (pSizeControlBar->m_dwStyle & CBRS_ORIENT_HORZ)
	{
		rectGripper = pSizeControlBar->m_rectGripper;
		rectGripper.OffsetRect(ptOffset);

		if (::GetVersion() >= 0x80000000)
			// Adoid a problem with the ::GradientFill() API in Win98 and ME
			pSkin2003->GradientFillManual(pDC, rectGripper,
				pSkin2003->GetControlBarLightColor(),
				pSkin2003->GetControlBarDarkColor(),
				GRADIENT_FILL_RECT_H);
		else
			pSkin2003->GradientFill(pDC, rectGripper,
				pSkin2003->GetControlBarLightColor(),
				pSkin2003->GetControlBarDarkColor(),
				GRADIENT_FILL_RECT_H);

		// If this is not the leftmost griper gover the top portion of it
		CRect rectThis;
		pSizeControlBar->GetWindowRect(rectThis);
		pSizeControlBar->m_pDockBar->ScreenToClient(&rectThis);
		if (rectThis.left > 0)
		{
			CRect rectSolid(rectGripper);
			rectSolid.right = rectSolid.left + 2;
			pDC->FillSolidRect(rectSolid, pSkin2003->GetDockbarBackgroundColor());
		}

		// Draw the stripes
		CRect rectStripes(rectGripper);
		rectStripes.DeflateRect(5, 5);
		rectStripes.top = rectStripes.bottom - 2;
		pSkin2003->DrawGripperStripes(pDC, rectStripes, FALSE);

		// Draw the text

		// Setup a font for vertically oriented mode
		LOGFONT lf;
		VERIFY(m_fontCaption.GetLogFont(&lf));
		lf.lfEscapement = 900;
		lf.lfOrientation = 900;
		CFont fontVert;
		VERIFY(fontVert.CreateFontIndirect(&lf));
		CFont* pOldFont = pDC->SelectObject(&fontVert);

		CString strBarTitle;
		pSizeControlBar->GetWindowText(strBarTitle);
		CRect rectText(rectGripper);

		rectText.left = 6;
		rectText.top = 0;
		rectText.right = rectGripper.Height();
		rectText.bottom = rectGripper.bottom + 2;

		// Crop the text if there are buttons
		CRect rectButton(0, 0, 0, 0);
		if (pSizeControlBar->IsResizeBtn())
			rectButton = pSizeControlBar->m_rectResizeBtn;
		else if (pSizeControlBar->IsCloseBtn())
			rectButton = pSizeControlBar->m_rectCloseBtn;
		if (!rectButton.IsRectEmpty())
		{
			rectButton.OffsetRect(ptOffset);
			rectText.top = rectButton.bottom + 1;
		}

		int iOldBkMode = pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(strBarTitle, &rectText,
			DT_LEFT|DT_BOTTOM|DT_SINGLELINE|DT_END_ELLIPSIS);
		pDC->SetBkMode(iOldBkMode);
		pDC->SelectObject(pOldFont);
	}
	else // vertical
	{
		rectGripper = pSizeControlBar->m_rectGripper;
		rectGripper.OffsetRect(ptOffset);

		if (::GetVersion() >= 0x80000000)
			// Adoid a problem with the ::GradientFill() API in Win98 and ME
			pSkin2003->GradientFillManual(pDC, rectGripper,
				pSkin2003->GetControlBarLightColor(),
				pSkin2003->GetControlBarDarkColor(),
				GRADIENT_FILL_RECT_V);
		else
			pSkin2003->GradientFill(pDC, rectGripper,
				pSkin2003->GetControlBarLightColor(),
				pSkin2003->GetControlBarDarkColor(),
				GRADIENT_FILL_RECT_V);

		// If this is not the topmost griper gover the top portion of it
		CRect rectThis;
		pSizeControlBar->GetWindowRect(rectThis);
		pSizeControlBar->m_pDockBar->ScreenToClient(&rectThis);
		if (rectThis.top > 0)
		{
			CRect rectSolid(rectGripper);
			rectSolid.bottom = rectSolid.top + 2;
			pDC->FillSolidRect(rectSolid, pSkin2003->GetDockbarBackgroundColor());
		}

		// Draw the stripes
		CRect rectStripes(rectGripper);
		rectStripes.DeflateRect(3, 6);
		rectStripes.right = rectStripes.left + 2;
		pSkin2003->DrawGripperStripes(pDC, rectStripes, TRUE);

		// Draw the text
		CFont* pOldFont = pDC->SelectObject(&m_fontCaption);
		CString strBarTitle;
		pSizeControlBar->GetWindowText(strBarTitle);
		CRect rectText(rectGripper);
		rectText.OffsetRect(13, 2);

		// Crop the text if there are buttons
		CRect rectButton(0, 0, 0, 0);
		if (pSizeControlBar->IsResizeBtn())
			rectButton = pSizeControlBar->m_rectResizeBtn;
		else if (pSizeControlBar->IsCloseBtn())
			rectButton = pSizeControlBar->m_rectCloseBtn;
		if (!rectButton.IsRectEmpty())
		{
			rectButton.OffsetRect(ptOffset);
			rectText.right = rectButton.left - 1;			
		}

		int iOldBkMode = pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(strBarTitle, &rectText,
			DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		pDC->SetBkMode(iOldBkMode);
		pDC->SelectObject(pOldFont);
	}
}

void COXDockbarSkin2003::DrawSplitter(CDC* pDC, COXSplitterRect* pSplitterRect, COXSizeDockBar* pSizeDockBar)
{
    CRect rect(pSplitterRect->m_rect);

	CRect rectDockBar;
	pSizeDockBar->GetClientRect(rectDockBar);

	if (rect.bottom > 8000)
		rect.bottom = 8000;

	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;
	pDC->FillSolidRect(rect, pSkin2003->GetDockbarBackgroundColor());

	if (pSplitterRect->m_type == SPLITTER_VERT)
	{
		rect.left = pSplitterRect->m_rect.CenterPoint().x;
		rect.right = rect.left + 1;
	}
	else
	{
		rect.top = pSplitterRect->m_rect.CenterPoint().y;
		rect.bottom = rect.top + 1;
	}

	pDC->FillSolidRect(rect, pSkin2003->GetSplitterLineColor());

}

void COXDockbarSkin2003::DrawCloseButton(CDC* pDC, COXSizeControlBar* pSizeControlBar)
{
    if(!pSizeControlBar->IsCloseBtn() || (pSizeControlBar->m_dwStyle & CBRS_FLOATING))
		return;

	// Determine the button rectangle
    CRect rectWindow;
	pSizeControlBar->GetWindowRect(rectWindow);
	pSizeControlBar->ScreenToClient(rectWindow);
	CRect rect = pSizeControlBar->m_rectCloseBtn;
	rect.OffsetRect(-rectWindow.left, -rectWindow.top);
	rect.InflateRect(1, 1, 2, 2);

	m_pSkinXP->DrawFrameButton(pDC, rect, DFCS_CAPTIONCLOSE, FALSE, FALSE,
		::GetSysColor(COLOR_3DFACE), pSizeControlBar);
}

void COXDockbarSkin2003::DrawResizeButton(CDC* pDC, COXSizeControlBar* pSizeControlBar)
{
    if(!pSizeControlBar->IsCloseBtn() || (pSizeControlBar->m_dwStyle & CBRS_FLOATING))
		return;

	// Determine the button rectangle
    CRect rectWindow;
	pSizeControlBar->GetWindowRect(rectWindow);
	pSizeControlBar->ScreenToClient(rectWindow);
	CRect rect = pSizeControlBar->m_rectResizeBtn;
	rect.OffsetRect(-rectWindow.left, -rectWindow.top);
	rect.InflateRect(1, 1, 2, 2);

	// Determine the flags
	UINT nState = pSizeControlBar->m_bMaximized ? DFCS_CAPTIONMIN : DFCS_CAPTIONMAX;
	BOOL bDisabled = !pSizeControlBar->CanResize();

	m_pSkinXP->DrawFrameButton(pDC, rect, nState, bDisabled, FALSE,
		::GetSysColor(COLOR_3DFACE), pSizeControlBar);
}

void COXDockbarSkin2003::RecalcLayout(COXSizeControlBar* pSizeControlBar)
{
	CRect rectWindow;
	pSizeControlBar->GetWindowRect(rectWindow);
	pSizeControlBar->ScreenToClient(rectWindow);
	pSizeControlBar->m_rectGripper = rectWindow;

	if(pSizeControlBar->IsGripper() && !(pSizeControlBar->m_dwStyle & CBRS_FLOATING))
	{
		if (pSizeControlBar->m_dwStyle & CBRS_ORIENT_HORZ) 
		{
			pSizeControlBar->m_rectGripper.DeflateRect(0, 2);

			pSizeControlBar->m_rectGripper.right = 
				pSizeControlBar->m_rectGripper.left + ID_O11_GRIPPER + ID_BUTTON_SIDE;
			
			if(pSizeControlBar->IsCloseBtn())
			{
				pSizeControlBar->m_rectCloseBtn = pSizeControlBar->m_rectGripper;
				pSizeControlBar->m_rectCloseBtn.bottom = pSizeControlBar->m_rectCloseBtn.top + pSizeControlBar->m_rectCloseBtn.Width();
				pSizeControlBar->m_rectCloseBtn.DeflateRect(7, 7, 6, 6);
			}

			if(pSizeControlBar->IsResizeBtn())
			{
				pSizeControlBar->m_rectResizeBtn = pSizeControlBar->m_rectGripper;
				pSizeControlBar->m_rectResizeBtn.bottom = pSizeControlBar->m_rectResizeBtn.top + pSizeControlBar->m_rectResizeBtn.Width();
				pSizeControlBar->m_rectResizeBtn.DeflateRect(7, 7, 6, 6);
				if (pSizeControlBar->IsCloseBtn())
					pSizeControlBar->m_rectResizeBtn.OffsetRect(0, pSizeControlBar->m_rectResizeBtn.Width() + 3);
			}
		} 
		else // vertical
		{
			pSizeControlBar->m_rectGripper.DeflateRect(2, 0);

			pSizeControlBar->m_rectGripper.bottom = 
				pSizeControlBar->m_rectGripper.top + ID_O11_GRIPPER + ID_BUTTON_SIDE;

			if (pSizeControlBar->IsCloseBtn())
			{
				pSizeControlBar->m_rectCloseBtn = pSizeControlBar->m_rectGripper;
				pSizeControlBar->m_rectCloseBtn.left = pSizeControlBar->m_rectCloseBtn.right - pSizeControlBar->m_rectCloseBtn.Height();
				pSizeControlBar->m_rectCloseBtn.DeflateRect(7, 7, 6, 6);
			}

			if (pSizeControlBar->IsResizeBtn())
			{
				pSizeControlBar->m_rectResizeBtn = pSizeControlBar->m_rectGripper;
				pSizeControlBar->m_rectResizeBtn.left = pSizeControlBar->m_rectResizeBtn.right - pSizeControlBar->m_rectResizeBtn.Height();
				pSizeControlBar->m_rectResizeBtn.DeflateRect(7, 7, 6, 6);
				if (pSizeControlBar->IsCloseBtn())
					pSizeControlBar->m_rectResizeBtn.OffsetRect(-pSizeControlBar->m_rectResizeBtn.Width() - 3, 0);
			}
		}
	}

	pSizeControlBar->m_bDelayRecalcLayout=FALSE;
}

void COXDockbarSkin2003::OnNcPaintSizeDockBar(COXSizeDockBar* pSizeDockBar)
{
	CWindowDC dc(pSizeDockBar);
	CRect rectWindow;
	pSizeDockBar->GetWindowRect(rectWindow);
	CPoint ptOffset = rectWindow.TopLeft();
	rectWindow.OffsetRect(-ptOffset.x, -ptOffset.y);

	// Get the client rectangle in window coordionates
	CRect rectClient;
	pSizeDockBar->GetClientRect(rectClient);
	pSizeDockBar->ClientToScreen(rectClient);
	rectClient.OffsetRect(-ptOffset.x, -ptOffset.y);
	dc.ExcludeClipRect(rectClient);

	CRect rectMain;
	pSizeDockBar->GetParentFrame()->GetWindowRect(rectMain);

	// Get the dockbar screen rectangle
	CRect rectDockbar;
	pSizeDockBar->GetWindowRect(rectDockbar);

	int iWidth = ::GetSystemMetrics(SM_CXSCREEN);
	if (iWidth < rectMain.Width())
		iWidth = rectMain.Width();

	CRect rectGradient;
	rectGradient.left = rectMain.left - rectDockbar.left;
	rectGradient.right = rectGradient.left + iWidth;
	rectGradient.top = 0;
	rectGradient.bottom = rectDockbar.Height();

	if (pSizeDockBar->GetFirstDockedSizeControlBar(NULL) != NULL)
	{
		dc.FillSolidRect(rectGradient, m_pSkinXP->GetDockbarBackgroundColor());
	}
	else
	{
		CRect rectSolid(rectGradient);
		rectGradient.right = rectGradient.CenterPoint().x;
		rectSolid.left = rectGradient.right;

		COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;
		pSkin2003->GradientFill(&dc, rectGradient,
			pSkin2003->GetDockbarLeftColor(),
			pSkin2003->GetDockbarRightColor(),
			GRADIENT_FILL_RECT_H);
		dc.FillSolidRect(rectSolid, pSkin2003->GetDockbarRightColor());
	}
}


// COXStatusbarSkin
//


// COXStatusbarSkinClassic
//

void COXStatusbarSkinClassic::OnPaintStatusbar( COXStatusBar* pStatusBar )
{
	pStatusBar->Default();

	CClientDC DC(pStatusBar);
	pStatusBar->UpdateAllPanes(FALSE, TRUE);

	// Draw the text for each pane, and the window resize gripper
	DrawStatusBarPanes(&DC, pStatusBar );
	DrawFlatPanes(&DC, pStatusBar);

	for (int i=0; i < pStatusBar->GetCount(); i++)
	{
		AFX_STATUSPANE* pSBP=pStatusBar->_GetPanePtr(i);
		ASSERT(pSBP!=NULL);

		if (!(pSBP->nStyle & SBPS_DISABLED))
		{
			COXStatusBar::CBmpInfo* info=(COXStatusBar::CBmpInfo*)pStatusBar->m_PaneBmp[i];
			if(info != NULL)
				// This pane is a pane with a bitmap, so draw the bitmap
				pStatusBar->DrawStatusBmp(&DC, i, pSBP->nStyle);
		}
	}
}

// COXStatusbarSkinXP
//

COXStatusbarSkinXP::COXStatusbarSkinXP(COXSkinXP* pSkinXP)
{
	m_pSkinXP = pSkinXP;
}

COXStatusbarSkinXP::~COXStatusbarSkinXP()
{
}

void COXStatusbarSkinXP::OnPaintStatusbar( COXStatusBar* pStatusBar )
{
	CPaintDC DC(pStatusBar);
	pStatusBar->UpdateAllPanes(FALSE, TRUE);

	// Draw the text for each pane, and the window resize gripper
	DrawStatusBarPanes(&DC, pStatusBar);
	DrawFlatPanes(&DC, pStatusBar);
	DrawGripper(&DC, pStatusBar);

	for (int i=0; i < pStatusBar->GetCount(); i++)
	{
		AFX_STATUSPANE* pSBP=pStatusBar->_GetPanePtr(i);
		ASSERT(pSBP!=NULL);

		if (!(pSBP->nStyle & SBPS_DISABLED))
		{
			COXStatusBar::CBmpInfo* info=(COXStatusBar::CBmpInfo*)pStatusBar->m_PaneBmp[i];
			if(info != NULL)
				// This pane is a pane with a bitmap, so draw the bitmap
				pStatusBar->DrawStatusBmp(&DC, i, pSBP->nStyle);
		}
	}
}

void COXStatusbarSkinXP::DrawStatusBarPanes(CDC* pDC, COXStatusBar* pStatusBar)
{
	// Draw the background of the entire status bar
	CRect rectClient;
	pStatusBar->GetClientRect(rectClient);
	pDC->FillSolidRect(rectClient, GetSysColor(COLOR_BTNFACE));

	DRAWITEMSTRUCT drawItemStruct;
	CRect rcItem;
	int cxWidth;
	UINT nID, nStyle;
	BYTE keyState[256];
	::GetKeyboardState( keyState );

	drawItemStruct.hDC = pDC->GetSafeHdc();

    for (int nIndex = 0; nIndex < pStatusBar->m_nCount; nIndex++)
    {
		pStatusBar->GetItemRect( nIndex, rcItem );
		pStatusBar->GetPaneInfo( nIndex, nID, nStyle, cxWidth );
		
		if (nIndex == pStatusBar->m_nCount - 1)
			rcItem.right = rcItem.left + cxWidth + 6;

		// prepare the background for each item on the statusbar
		switch( nID )
		{
		case ID_INDICATOR_CAPS:
			DrawPaneText( pDC, rcItem, pStatusBar, nIndex, keyState[VK_CAPITAL] & 1 );
			break;
		case ID_INDICATOR_NUM:
			DrawPaneText( pDC, rcItem, pStatusBar, nIndex, keyState[VK_NUMLOCK] & 1 );
			break;
		case ID_INDICATOR_SCRL:
			DrawPaneText( pDC, rcItem, pStatusBar, nIndex, keyState[VK_SCROLL] & 1 );
			break;
		default:
			// draw custom panes
			drawItemStruct.itemID = nIndex;
			drawItemStruct.rcItem = rcItem;
			pStatusBar->DrawItem( &drawItemStruct );
		}
	}
}

void COXStatusbarSkinXP::DrawPaneText( CDC *pDc, CRect rPane, COXStatusBar* pStatusBar, int nIndex, BOOL enabled )
{
	CFont *oldFont = pDc->SelectObject( pStatusBar->GetFont());

	if ( enabled == TRUE )
		pDc->SetTextColor( GetSysColor( COLOR_BTNTEXT ));
	else
		pDc->SetTextColor(m_pSkinXP->GetDisabledTextColor());

	pDc->SetBkMode( TRANSPARENT );
	pDc->DrawText( pStatusBar->GetPaneText( nIndex ), rPane, DT_CENTER|DT_VCENTER|DT_SINGLELINE );

	pDc->SelectObject( oldFont );
}

void COXStatusbarSkinXP::DrawFlatPanes(CDC* pDC, COXStatusBar* pStatusBar)
{
	CRect rcItem;
	CBrush brush;
	brush.CreateSolidBrush(m_pSkinXP->GetStatusPaneColor());
    for (int i = 0; i < pStatusBar->m_nCount; i++)
    {
		pStatusBar->GetItemRect(i, rcItem);
		if (i == pStatusBar->m_nCount - 1)
		{
			int cxWidth;
			UINT nID, nStyle;
			pStatusBar->GetPaneInfo(i, nID, nStyle, cxWidth );
			rcItem.right = rcItem.left + cxWidth + 6;
		}
		pDC->FrameRect(rcItem, &brush);
	}
}

void COXStatusbarSkinXP::DrawGripper(CDC* pDC, COXStatusBar* pStatusBar)
{
	CFrameWnd* pParentFrame = pStatusBar->GetParentFrame();
	if (pParentFrame != NULL && (pParentFrame->GetStyle() & WS_THICKFRAME) == 0)
		return; // we don't need a gripper since the thick frame style is not set
	
	if (pParentFrame != NULL && (pParentFrame->GetStyle() & WS_MAXIMIZE) != 0)
		return; // we don't need a gripper since the frame is maximized

	CRect rClient;
	pStatusBar->GetClientRect( rClient );
	int nSize = rClient.Height()%2 ? rClient.Height() - 1 : rClient.Height();
	// calculate the gripper area
	rClient.left = rClient.right - nSize;

	pDC->FillSolidRect(rClient, GetSysColor(COLOR_BTNFACE));

	// create the gray pen
	CPen grayPen( PS_SOLID, 1, GetSysColor( COLOR_3DSHADOW ));
	CPen *oldPen = NULL;
	
	for ( int nIndex = 0; nIndex < nSize; nIndex += 3 )
	{
		oldPen = pDC->SelectObject( &grayPen );
		pDC->MoveTo( rClient.right - nIndex, rClient.bottom );
		pDC->LineTo( rClient.right, rClient.bottom - nIndex );

		nIndex ++;
		pDC->SelectObject( oldPen );
		(CPen*)pDC->SelectObject( ::GetStockObject( WHITE_PEN ));
		pDC->MoveTo( rClient.right - nIndex, rClient.bottom );
		pDC->LineTo( rClient.right, rClient.bottom - nIndex );

		oldPen = NULL;
	}
}

// COXStatusbarSkin2003
//

COXStatusbarSkin2003::COXStatusbarSkin2003(COXSkin2003* pSkin2003) :
	COXStatusbarSkinXP(pSkin2003)
{
}

COXStatusbarSkin2003::~COXStatusbarSkin2003()
{
}

void COXStatusbarSkin2003::DrawGradientBackground(CDC* pDC, LPCRECT lpRect)
{
	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;

	CRect rectTop(lpRect);
	rectTop.bottom = rectTop.top + 3;
	pSkin2003->GradientFill(pDC, rectTop,
		pSkin2003->GetStatusbarTopColor(),
		pSkin2003->GetStatusbarMiddleColor(),
		GRADIENT_FILL_RECT_V);

	CRect rectBottom(lpRect);
	rectBottom.top = rectBottom.bottom - 5;
	pSkin2003->GradientFill(pDC, rectBottom,
		pSkin2003->GetStatusbarMiddleColor(),
		pSkin2003->GetStatusbarBottomColor(),
		GRADIENT_FILL_RECT_V);

	CRect rectMiddle(lpRect);
	rectMiddle.top = rectTop.bottom;
	rectMiddle.bottom = rectBottom.top;
	pDC->FillSolidRect(rectMiddle, pSkin2003->GetStatusbarMiddleColor());
}

void COXStatusbarSkin2003::DrawStatusBarPanes(CDC* pDC, COXStatusBar* pStatusBar)
{
	// Draw the background of the entire status bar
	CRect rectClient;
	pStatusBar->GetClientRect(rectClient);

	DrawGradientBackground(pDC, rectClient);

	DRAWITEMSTRUCT drawItemStruct;
	CRect rcItem;
	int cxWidth;
	UINT nID, nStyle;
	BYTE keyState[256];
	::GetKeyboardState( keyState );

	drawItemStruct.hDC = pDC->GetSafeHdc();

    for (int nIndex = 0; nIndex < pStatusBar->m_nCount; nIndex++)
    {
		pStatusBar->GetItemRect( nIndex, rcItem );
		pStatusBar->GetPaneInfo( nIndex, nID, nStyle, cxWidth );
		
		if (nIndex == pStatusBar->m_nCount - 1)
			rcItem.right = rcItem.left + cxWidth + 6;

		// prepare the background for each item on the statusbar
		switch( nID )
		{
		case ID_INDICATOR_CAPS:
			DrawPaneText( pDC, rcItem, pStatusBar, nIndex, keyState[VK_CAPITAL] & 1 );
			break;
		case ID_INDICATOR_NUM:
			DrawPaneText( pDC, rcItem, pStatusBar, nIndex, keyState[VK_NUMLOCK] & 1 );
			break;
		case ID_INDICATOR_SCRL:
			DrawPaneText( pDC, rcItem, pStatusBar, nIndex, keyState[VK_SCROLL] & 1 );
			break;
		default:
			// draw custom panes
			drawItemStruct.itemID = nIndex;
			drawItemStruct.rcItem = rcItem;
			pStatusBar->DrawItem( &drawItemStruct );
		}
	}
}

void COXStatusbarSkin2003::DrawGripper(CDC* pDC, COXStatusBar* pStatusBar)
{
	CFrameWnd* pParentFrame = pStatusBar->GetParentFrame();
	if (pParentFrame != NULL && (pParentFrame->GetStyle() & WS_THICKFRAME) == 0)
		return; // we don't need a gripper since the thick frame style is not set
	
	if (pParentFrame != NULL && (pParentFrame->GetStyle() & WS_MAXIMIZE) != 0)
		return; // we don't need a gripper since the frame is maximized

	CRect rClient;
	pStatusBar->GetClientRect( rClient );
	int nSize = rClient.Height()%2 ? rClient.Height() - 1 : rClient.Height();
	// calculate the gripper area
	rClient.left = rClient.right - nSize;

	CRect rectBackground(rClient);
	rectBackground.left += 3;
	DrawGradientBackground(pDC, rectBackground);

	// Draw the gripper
	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// Do not draw the top half
			if (i + j < 2)
				continue;

			CRect rectDot;
			rectDot.left = rClient.left + 7 + i * 4;
			rectDot.top = rClient.top + 7 + j * 4;
			rectDot.right = rectDot.left + 2;
			rectDot.bottom = rectDot.top + 2;

			pDC->FillSolidRect(rectDot,  pSkin2003->GetGripperStripesShadowColor());
			rectDot.OffsetRect(-1, -1);
			pDC->FillSolidRect(rectDot, pSkin2003->GetGripperColor());
		}
	}
}

void COXStatusbarSkin2003::DrawFlatPanes(CDC* pDC, COXStatusBar* pStatusBar)
{
	CRect rcItem;
    for (int i = 0; i < pStatusBar->m_nCount; i++)
    {
		pStatusBar->GetItemRect(i, rcItem);
		if (i == pStatusBar->m_nCount - 1)
		{
			int cxWidth;
			UINT nID, nStyle;
			pStatusBar->GetPaneInfo(i, nID, nStyle, cxWidth );
			rcItem.right = rcItem.left + cxWidth + 6;
		}

		rcItem.left = rcItem.right++;
		rcItem.DeflateRect(0, 2, 0, 2);
		pDC->FillSolidRect(rcItem,  m_pSkinXP->GetSeparatorColor());
		rcItem.OffsetRect(1, 1);
		pDC->FillSolidRect(rcItem,  RGB(255, 255, 255)); // This color is always white
	}
}

// COXFrameSkin
//


// COXFrameSkinClassic
//


// COXFrameSkinXP
//

COXFrameSkinXP::COXFrameSkinXP(COXSkinXP* pSkinXP)
{
	m_pSkinXP = pSkinXP;
}

void COXFrameSkinXP::DrawViewBorder(CView* pView)
{
	CWindowDC dc(pView);
	CRect rectWindow;
	pView->GetWindowRect(rectWindow);
	CPoint ptOffset = rectWindow.TopLeft();
	rectWindow.OffsetRect(-ptOffset.x, -ptOffset.y);

	CBrush brush;
	brush.CreateSolidBrush(m_pSkinXP->GetBorderColor());
	dc.FrameRect(rectWindow, &brush);
}

void COXFrameSkinXP::OnNcCalcSizeView(LPARAM lParam, CView* /*pView*/)
{
	NCCALCSIZE_PARAMS* lpncsp = (NCCALCSIZE_PARAMS*) lParam;
	lpncsp->rgrc[0].left -= 1;
	lpncsp->rgrc[0].top -= 1;
	lpncsp->rgrc[0].right += 1;
	lpncsp->rgrc[0].bottom += 1;
}

// COXFrameSkin2003
//

COXFrameSkin2003::COXFrameSkin2003(COXSkin2003* pSkin2003) :
	COXFrameSkinXP(pSkin2003)
{
}

COLORREF COXFrameSkin2003::GetBackgroundColor()
{
	return m_pSkinXP->GetDockbarBackgroundColor();
}

COLORREF COXFrameSkin2003::GetBorderColor()
{
	return m_pSkinXP->GetBorderColor();
}

// COXTabSkin
//


// COXTabSkinClassic
//

void COXTabSkinClassic::OnPaintTabCtrl(CTabCtrl* pTabCtrl)
{
	COXSkinnedTabCtrl* pSkinnedTab = DYNAMIC_DOWNCAST(COXSkinnedTabCtrl, pTabCtrl);
	if (pSkinnedTab != NULL)
		pSkinnedTab->Default(); 

// Code to handle vertical orientation in XP themed mode

	if( IsThemed() && (pTabCtrl->GetStyle()&TCS_VERTICAL) )
	{
		int nTabCount = pTabCtrl->GetItemCount();
		int nSel = pTabCtrl->GetCurSel();
	
		DRAWITEMSTRUCT dis;
		dis.CtlType = ODT_TAB;
		dis.CtlID = pTabCtrl->GetDlgCtrlID();
		dis.hwndItem = pTabCtrl->GetSafeHwnd();
		dis.hDC = GetDC(dis.hwndItem);
		dis.itemAction = ODA_DRAWENTIRE;

		for ( int nIndex = 0; nIndex < nTabCount; nIndex++ )
		{			
			if (nIndex != nSel)
				dis.itemState = ODS_SELECTED;
			else
				dis.itemState = 0;
			
			dis.itemID = nIndex;
			
			pTabCtrl->GetItemRect(nIndex, &dis.rcItem);	

			RECT r2 = dis.rcItem;
			int ht = r2.bottom - r2.top;

			TCHAR buf[255];
			TCITEM tabItem;
			tabItem.mask = TCIF_TEXT|TCIF_IMAGE;
			tabItem.pszText = buf;
			tabItem.cchTextMax = sizeof( buf );
			pTabCtrl->GetItem( nIndex, &tabItem );
			CString tempStr( tabItem.pszText );
			
			if ((tabItem.mask & TCIF_IMAGE) && tabItem.iImage >= 0)
			{
				CImageList* pImage = pTabCtrl->GetImageList();
				if ( pImage != NULL )
				{					
					IMAGEINFO iInfo;
					pImage->GetImageInfo( tabItem.iImage, &iInfo );						
					
					if( pTabCtrl->GetStyle()&TCS_BOTTOM )
						ht -= 10;
					else
						ht -= iInfo.rcImage.bottom - iInfo.rcImage.top + 10;
															
				}
			}

			r2.top += ht;
			r2.bottom += ht;

			NONCLIENTMETRICS nclim;
			nclim.cbSize=sizeof(NONCLIENTMETRICS);
			::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
				sizeof(NONCLIENTMETRICS),&nclim,0);
			LOGFONT lf = nclim.lfMenuFont;			
			lf.lfEscapement = 900;			 
			HFONT hf = CreateFontIndirect(&lf);

			HFONT oldf = (HFONT)SelectObject(dis.hDC,(HGDIOBJ)hf);
			SetBkMode(dis.hDC, TRANSPARENT);
			DrawText(dis.hDC,tempStr,-1,&r2,DT_NOCLIP);
			SelectObject(dis.hDC,(HGDIOBJ)oldf);
			DeleteObject(hf);			
		}
		
		ReleaseDC(dis.hwndItem,dis.hDC);
	}

// End of custom code to handle XP themed mode and vertical orientation

};

void COXTabSkinClassic::DrawMDITabBorder(CDC* pDC, LPCRECT lpRect, COXTabWorkspace* /*pTabWorkspace*/)
{
	pDC->Draw3dRect(lpRect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
}

// COXTabSkinXP
//

// predefined margin size
#define TAB_MARGIN 3

COXTabSkinXP::COXTabSkinXP(COXSkinXP* pSkinXP)
{
	m_pSkinXP = pSkinXP;

	m_sepPen.CreatePen( PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW) );
	NONCLIENTMETRICS nclim;
	nclim.cbSize=sizeof(NONCLIENTMETRICS);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
							sizeof(NONCLIENTMETRICS),
							&nclim,0);
	m_tabFont.CreateFontIndirect(&nclim.lfMenuFont);

	m_btnFill.CreateSolidBrush( m_pSkinXP->GetInactiveTabTextColor() );
	m_btnLine.CreatePen( PS_SOLID, 1, m_pSkinXP->GetInactiveTabTextColor() );
}

void COXTabSkinXP::DrawMDITabBorder(CDC* pDC, LPCRECT lpRect, COXTabWorkspace* /*pTabWorkspace*/)
{
	pDC->Draw3dRect(lpRect, m_pSkinXP->GetBorderColor(),m_pSkinXP->GetBorderColor());
}

void COXTabSkinXP::OnNcPaintTabClient(COXTabClientWnd* pTabClient)
{
	pTabClient->Default();
	
	CWindowDC dc(pTabClient);
	CRect rectWindow;
	pTabClient->GetWindowRect(rectWindow);
	CPoint ptOffset = rectWindow.TopLeft();
	rectWindow.OffsetRect(-ptOffset.x, -ptOffset.y);

	// Get the client rectangle in window coordionates
	CRect rectClient;
	pTabClient->GetClientRect(rectClient);
	pTabClient->ClientToScreen(rectClient);
	rectClient.OffsetRect(-ptOffset.x, -ptOffset.y);
	dc.ExcludeClipRect(rectClient);

	// Draw the border
	CBrush brBorder;
	brBorder.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
	dc.FrameRect(rectWindow, &brBorder);
	CBrush brBackground;
	brBackground.CreateSolidBrush(::GetSysColor(COLOR_3DDKSHADOW));
	rectWindow.DeflateRect(1, 1);
	dc.FrameRect(rectWindow, &brBackground);
}

void COXTabSkinXP::OnPaintTabCtrl(CTabCtrl* pTabCtrl)
{
	CPaintDC dc(pTabCtrl); // device context for painting
	// retrieve the client area size info
	CRect clientRect;
	pTabCtrl->GetClientRect(&clientRect);
	// create and initialize memory DC to draw on
	CDC memDc, itemMemDc;
	CBitmap bmpMem, bmpItemMem, *pOldItemMemBmp, *pOldMemBmp;
	bmpMem.CreateCompatibleBitmap( &dc, clientRect.right, clientRect.bottom );
	bmpItemMem.CreateCompatibleBitmap( &dc, clientRect.Width(), clientRect.Height());
	// create memory DC fot the entire tab control
	memDc.CreateCompatibleDC( &dc );
	pOldMemBmp = memDc.SelectObject( &bmpMem );
	// create memory DC for each item to draw on
	itemMemDc.CreateCompatibleDC( &dc );
	pOldItemMemBmp = itemMemDc.SelectObject( &bmpItemMem );

	// check if we have already subclassed the up-down control
	COXSkinnedSpinCtrl* pUpDown = GetSpinCtrl(pTabCtrl);
	if (pUpDown != NULL && pUpDown->GetSafeHwnd() == NULL )
	{
		// we have not, check if it was created by looping through all client windows
		int intnRet;
		CRect upDnRect;
		CWnd* pWnd = pTabCtrl->GetWindow(GW_CHILD);

		while(pWnd)
		{
			TCHAR buf[] = _T("msctls_updown32");
			intnRet= ::GetClassName(pWnd->m_hWnd,buf,sizeof(buf)/sizeof(buf[0]));
			if( _tcscmp(buf, _T("msctls_updown32")) != 0 )
			{
				pWnd=pWnd->GetWindow(GW_HWNDNEXT);
			}
			else
			{ 
				//The msctls_updown32 control isfound.
				//
				//Subclass common msctls_updown32 control
				pUpDown->SubclassWindow(pWnd->m_hWnd);

				pWnd=0;
			}
		}
	}

	// draw background of the entire control
	DrawBackground(&memDc, clientRect, pTabCtrl);
	
	// Draw the tab work area
	CRect rPage( clientRect );
	pTabCtrl->AdjustRect(FALSE, rPage);

	// Draw a border around the child window
	CRect rectChild;
	COX3DTabViewContainer* p3DTVC = DYNAMIC_DOWNCAST(COX3DTabViewContainer, pTabCtrl);
	if (p3DTVC != NULL)
	{
		rectChild = p3DTVC->m_rectPage;
		rectChild.InflateRect(1, 1);
	}
	else
	{
		rectChild = rPage;
		rectChild.bottom -= 1;
	}

	CBrush brushBorder;
	brushBorder.CreateSolidBrush(::GetSysColor(COLOR_3DSHADOW));
	memDc.FrameRect(rectChild, &brushBorder);
	
	// draw the solid line above the work area
	if ( pTabCtrl->GetRowCount() <= 1 )
	{
		if ( pTabCtrl->GetStyle()&TCS_BOTTOM )
		{
			// draw background for the button area
			memDc.FillSolidRect( clientRect.left, rPage.bottom + TAB_MARGIN,
							  clientRect.right, clientRect.bottom,
							  m_pSkinXP->GetTabButtonsBackgroundColor());
			// draw a separator line
			memDc.SelectObject( ::GetStockObject( BLACK_PEN ));
			memDc.MoveTo( clientRect.left, rPage.bottom + TAB_MARGIN - 1 );
			memDc.LineTo( clientRect.right, rPage.bottom + TAB_MARGIN - 1 );
		}
		else
		{
			// draw background for the button area
			memDc.FillSolidRect( clientRect.left, clientRect.top,
							  clientRect.right, rPage.top - TAB_MARGIN,
							  m_pSkinXP->GetTabButtonsBackgroundColor());
			// draw a separator line
			memDc.SelectObject( ::GetStockObject( WHITE_PEN ));
			memDc.MoveTo( clientRect.left, rPage.top - TAB_MARGIN );
			memDc.LineTo( clientRect.right, rPage.top - TAB_MARGIN );
		}
	}

	// also paint the tabs
	int nTabCount = pTabCtrl->GetItemCount();
	int nSel = pTabCtrl->GetCurSel();

	// the tab control does not contain any items
	if ( nTabCount == 0 )
	{
		memDc.SelectObject( pOldMemBmp );
		itemMemDc.SelectObject( pOldItemMemBmp );
		return;
	}

	// Draw individual tab items
	DRAWITEMSTRUCT dis;
	dis.CtlType = ODT_TAB;
	dis.CtlID = pTabCtrl->GetDlgCtrlID();
	dis.hwndItem = pTabCtrl->GetSafeHwnd();
	dis.hDC = itemMemDc.GetSafeHdc();
	dis.itemAction = ODA_DRAWENTIRE;

	for ( int nIndex = 0; nIndex < nTabCount; nIndex++ )
	{
		// initialize the draw item structure for each of the tabs
		if (nIndex != nSel)
		{
			dis.itemID = nIndex;
			dis.itemState = 0;
			pTabCtrl->GetItemRect(nIndex, &dis.rcItem);
			DrawItem(&dis, pTabCtrl);
			DrawItemBorder(&dis, pTabCtrl);

			if ( AdjustItemRect( nIndex, &dis.rcItem, pTabCtrl ))
			{
				int width = dis.rcItem.right - dis.rcItem.left;
				int height = dis.rcItem.bottom - dis.rcItem.top;
				//Copy drawing from memory device context to the screen
				memDc.BitBlt( dis.rcItem.left,dis.rcItem.top,width,
					height,&itemMemDc,dis.rcItem.left,dis.rcItem.top,SRCCOPY);
			}
		}
	}

	// draw the selected item
	dis.itemID = nSel;
	dis.itemState = ODS_SELECTED;
	pTabCtrl->GetItemRect(nSel, &dis.rcItem);
	DrawItem(&dis, pTabCtrl);
	DrawItemBorder(&dis, pTabCtrl);

	if ( AdjustItemRect( nSel, &dis.rcItem, pTabCtrl ))
	{
		int width = dis.rcItem.right - dis.rcItem.left;
		int height = dis.rcItem.bottom - dis.rcItem.top;
		//Copy drawing from memory device context to the screen
		memDc.BitBlt( dis.rcItem.left,dis.rcItem.top,width,
			height,&itemMemDc,dis.rcItem.left,dis.rcItem.top,SRCCOPY);
	}

	//Copy drawing from memory device context to the screen
	dc.BitBlt( clientRect.left, clientRect.top, clientRect.Width(),
			   clientRect.Height(), &memDc,
			   clientRect.left, clientRect.top, SRCCOPY);

	// a final clean up
	memDc.SelectObject( pOldMemBmp );
	itemMemDc.SelectObject( pOldItemMemBmp );

	// make sure that the spin control is also redrawn
	if (pUpDown != NULL && pUpDown->GetSafeHwnd() != NULL )
		if ( pUpDown->IsWindowVisible())
			pUpDown->Invalidate();
}

void COXTabSkinXP::DrawBackground(CDC* pDC, LPCRECT lpRect, CTabCtrl* /*pTabCtrl*/)
{
	pDC->FillSolidRect(lpRect, ::GetSysColor(COLOR_3DFACE));
}

void COXTabSkinXP::AdjustChildItemRect(LPRECT pRectChild, int iInflateValue, CTabCtrl* pTabCtrl)
{
	// Determine which side the tabs are on

	const int iMinMargin = 8;

	CRect rectClient;
	pTabCtrl->GetClientRect(rectClient);
	if (rectClient.IsRectEmpty())
		return;
	CRect rectItem(rectClient);
	pTabCtrl->AdjustRect(FALSE, rectItem);

	CRect rectMargins;
	rectMargins.left = rectItem.left - rectClient.left;
	rectMargins.top = rectItem.top - rectClient.top;
	rectMargins.right = rectClient.right - rectItem.right;
	rectMargins.bottom =  rectClient.bottom - rectItem.bottom;

	// Check each of the 4 sides and if we do not have buttons inflate it
	if (rectMargins.left < iMinMargin)
		pRectChild->left -= iInflateValue;
	if (rectMargins.top < iMinMargin)
		pRectChild->top -= iInflateValue;
	if (rectMargins.right < iMinMargin)
		pRectChild->right += iInflateValue;
	if (rectMargins.bottom < iMinMargin)
		pRectChild->bottom += iInflateValue;
}

void COXTabSkinXP::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct, CTabCtrl* pTabCtrl)
{
	CDC dc;
	dc.Attach( lpDrawItemStruct->hDC );
	CRect rcItem = lpDrawItemStruct->rcItem;

	if ( lpDrawItemStruct->itemState == ODS_SELECTED )
	{
		dc.FillSolidRect(&rcItem, ::GetSysColor(COLOR_3DFACE));
		dc.SetTextColor(::GetSysColor(COLOR_BTNTEXT));
	}
	else
	{
		dc.FillSolidRect(&rcItem, m_pSkinXP->GetTabButtonsBackgroundColor());
		dc.SetTextColor(m_pSkinXP->GetInactiveTabTextColor());
	}

	TCHAR buf[255];
	TCITEM tabItem;
	//Get text of item.
	tabItem.mask = TCIF_TEXT|TCIF_IMAGE;
	tabItem.pszText = buf;
	tabItem.cchTextMax = sizeof( buf );
	pTabCtrl->GetItem( lpDrawItemStruct->itemID, &tabItem );
	CString tempStr( tabItem.pszText );

	rcItem.top += 3;

	// draw the icon
	if ((tabItem.mask & TCIF_IMAGE) && tabItem.iImage >= 0)
	{
		CImageList* pImage = pTabCtrl->GetImageList();
		if ( pImage != NULL )
		{
			// get information about the icon to draw
			IMAGEINFO iInfo;
			pImage->GetImageInfo( tabItem.iImage, &iInfo );
			// calculate the statring point for the image to be drawn, try to vertically center
			CPoint ptStart( rcItem.left + 3, rcItem.top - 1 );

			pImage->Draw( &dc, tabItem.iImage, ptStart, ILD_TRANSPARENT );

			// reduce the rectangle by the size of the image
			rcItem.left += ( iInfo.rcImage.right - iInfo.rcImage.left ) + 3;
		}
	}

	// draw the tab's text
	CFont *pOldFont = dc.SelectObject( &m_tabFont );
	dc.DrawText( tempStr, &rcItem, DT_CENTER );
	dc.SelectObject( pOldFont );
	rcItem.top -= 3;

	// draw focus rectangle
	if ( lpDrawItemStruct->itemState == ODS_SELECTED &&
		 ( pTabCtrl->GetStyle()&TCS_FOCUSNEVER) == 0 )
	{
		CRect rect( rcItem );
		rect.DeflateRect( 2, 2 );
		dc.DrawFocusRect( rect );
	}

	dc.Detach();
}

void COXTabSkinXP::DrawItemBorder(LPDRAWITEMSTRUCT lpDrawItemStruct, CTabCtrl* pTabCtrl)
{
	CDC dc;
	dc.Attach( lpDrawItemStruct->hDC );

	if ( pTabCtrl->GetStyle()&TCS_BOTTOM )
	{
		if ( lpDrawItemStruct->itemState == ODS_SELECTED )
		{
			// draw the white line (on the left)
			dc.SelectObject( GetStockObject( WHITE_PEN ));
			dc.MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top );
			dc.LineTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom - 1 );
			dc.SelectObject( GetStockObject( BLACK_PEN ));
			// draw the bottom
			dc.LineTo( lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom - 1 );
			// draw the black outline on right
			dc.MoveTo( lpDrawItemStruct->rcItem.right - 1, lpDrawItemStruct->rcItem.top );
			dc.LineTo( lpDrawItemStruct->rcItem.right - 1, lpDrawItemStruct->rcItem.bottom - 1 );
		}
		else// draw the inactive button
		{
			if ( IsNextButtonSelected(lpDrawItemStruct->itemID, pTabCtrl) == FALSE )
			{	// draw the separators between the bottons
				CPen *pOldBrush = dc.SelectObject( &m_sepPen );
				dc.MoveTo( lpDrawItemStruct->rcItem.right - 1, lpDrawItemStruct->rcItem.top + 3 );
				dc.LineTo( lpDrawItemStruct->rcItem.right - 1, lpDrawItemStruct->rcItem.bottom - 3 );
				dc.SelectObject( pOldBrush );
			}

			// determine the position of this button in relation to the child window
			// it is important to know if the button is in the bottom or top and in which
			// row of tabs
			CRect rPage;
			pTabCtrl->GetClientRect( rPage );
			pTabCtrl->AdjustRect( FALSE, rPage );

			int nPosDifference = lpDrawItemStruct->rcItem.top - rPage.bottom;
			// Question 1, is the tab on top or bottom
			if ( nPosDifference > TAB_MARGIN )
			{	// the tab is on top, in a row other than the immediate below the client area
				// draw the separator line at the top of the item
				CPen *pOldBrush = dc.SelectObject( &m_sepPen );
				dc.MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top );
				dc.LineTo( lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.top );
				dc.SelectObject( pOldBrush );
			}
			else if ( nPosDifference >= 0 )
			{	// the tab is on top
				// draw the black line at the top
				dc.SelectObject( GetStockObject( BLACK_PEN ));
				dc.MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top );
				dc.LineTo( lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.top );
			}
			else
			{	// the tab is on the of the window
				// draw the separator line at the bottom of the item
				CPen *pOldBrush = dc.SelectObject( &m_sepPen );
				dc.MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom - 1 );
				dc.LineTo( lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom - 1 );
				dc.SelectObject( pOldBrush );
			}
		}
	}
	else
	{
		if ( lpDrawItemStruct->itemState == ODS_SELECTED )
		{
			// draw the white line (on the left)
			dc.SelectObject( GetStockObject( WHITE_PEN ));
			dc.MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom - 1 );
			dc.LineTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top );
			// draw the top
			dc.LineTo( lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.top );
			// draw the black outline on right
			dc.SelectObject( GetStockObject( BLACK_PEN ));
			dc.MoveTo( lpDrawItemStruct->rcItem.right - 1, lpDrawItemStruct->rcItem.top + 1 );
			dc.LineTo( lpDrawItemStruct->rcItem.right - 1, lpDrawItemStruct->rcItem.bottom - 1 );
		}
		else// draw the inactive button
		{
			if ( IsNextButtonSelected(lpDrawItemStruct->itemID, pTabCtrl) == FALSE )
			{
				CPen *pOldBrush = dc.SelectObject( &m_sepPen );
				dc.MoveTo( lpDrawItemStruct->rcItem.right - 1, lpDrawItemStruct->rcItem.top + 3 );
				dc.LineTo( lpDrawItemStruct->rcItem.right - 1, lpDrawItemStruct->rcItem.bottom - 3 );
				dc.SelectObject( pOldBrush );
			}

			// determine the position of this button in relation to the child window
			// it is important to know if the button is in the bottom or top and in which
			// row of tabs
			CRect rPage;
			pTabCtrl->GetClientRect( rPage );
			pTabCtrl->AdjustRect( FALSE, rPage );

			int nPosDifference = rPage.top - lpDrawItemStruct->rcItem.top;
			// Question 1, is the tab on top or bottom
			if ( nPosDifference > ( lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top + TAB_MARGIN ))
			{	// the tab is on top, in a row other than the immediate above the client area
				// draw the white line at the bottom
				CPen *pOldBrush = dc.SelectObject( &m_sepPen );
				dc.MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom - 1 );
				dc.LineTo( lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom - 1 );
				dc.SelectObject( pOldBrush );
			}
			else if ( nPosDifference > 0 )
			{	// the tab is on top
				// draw the white line at the bottom
				dc.SelectObject( GetStockObject( WHITE_PEN ));
				dc.MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom - 1 );
				dc.LineTo( lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom - 1 );
			}
			else
			{	// the tab is on the bottom
				// draw the separator line above
				CPen *pOldBrush = dc.SelectObject( &m_sepPen );
				dc.MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top );
				dc.LineTo( lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.top );
				dc.SelectObject( pOldBrush );
			}
		}
	}

	dc.Detach();
}

// COXTabSkin2003
//

COXTabSkin2003::COXTabSkin2003(COXSkin2003* pSkin2003) :
	COXTabSkinXP(pSkin2003)
{

}

COXTabSkin2003::~COXTabSkin2003()
{
}

void COXTabSkin2003::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct, CTabCtrl* pTabCtrl)
{
	CDC dc;
	dc.Attach( lpDrawItemStruct->hDC );
	CRect rcItem = lpDrawItemStruct->rcItem;

	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;

	if (lpDrawItemStruct->itemState == ODS_SELECTED)
	{
		if (pTabCtrl->GetStyle() & TCS_BOTTOM)
			pSkin2003->GradientFill(&dc, rcItem,
				pSkin2003->GetTabItemDarkColor(),
				pSkin2003->GetTabItemLightColor(),
				GRADIENT_FILL_RECT_V);
		else
			pSkin2003->GradientFill(&dc, rcItem,
				 pSkin2003->GetTabItemLightColor(),
				 pSkin2003->GetTabItemDarkColor(),
				GRADIENT_FILL_RECT_V);

		dc.SetTextColor(::GetSysColor(COLOR_BTNTEXT));
	}
	else
	{
		if (pTabCtrl->GetStyle() & TCS_BOTTOM)
			pSkin2003->GradientFill(&dc, rcItem,
				pSkin2003->GetTabBackgroundLightColor(),
				pSkin2003->GetTabBackgroundDarkColor(),
				GRADIENT_FILL_RECT_V);
		else
			pSkin2003->GradientFill(&dc, rcItem,
				pSkin2003->GetTabBackgroundDarkColor(),
				pSkin2003->GetTabBackgroundLightColor(),
				GRADIENT_FILL_RECT_V);

		dc.SetTextColor(m_pSkinXP->GetInactiveTabTextColor());
	}

	TCHAR buf[255];
	TCITEM tabItem;
	//Get text of item.
	tabItem.mask = TCIF_TEXT|TCIF_IMAGE;
	tabItem.pszText = buf;
	tabItem.cchTextMax = sizeof( buf );
	pTabCtrl->GetItem( lpDrawItemStruct->itemID, &tabItem );
	CString tempStr( tabItem.pszText );

	rcItem.top += 3;

	// draw the icon
	if ((tabItem.mask & TCIF_IMAGE) && tabItem.iImage >= 0)
	{
		CImageList* pImage = pTabCtrl->GetImageList();
		if ( pImage != NULL )
		{
			// get information about the icon to draw
			IMAGEINFO iInfo;
			pImage->GetImageInfo( tabItem.iImage, &iInfo );
			// calculate the statring point for the image to be drawn, try to vertically center
			CPoint ptStart( rcItem.left + 3, rcItem.top - 1 );

			pImage->Draw( &dc, tabItem.iImage, ptStart, ILD_TRANSPARENT );

			// reduce the rectangle by the size of the image
			rcItem.left += ( iInfo.rcImage.right - iInfo.rcImage.left ) + 3;
		}
	}

	// draw the tab's text
	CFont *pOldFont = dc.SelectObject( &m_tabFont );
	int iOldMode = dc.SetBkMode(TRANSPARENT);
	dc.DrawText( tempStr, &rcItem, DT_CENTER );
	dc.SetBkMode(iOldMode);
	dc.SelectObject( pOldFont );
	rcItem.top -= 3;

	// draw focus rectangle
	if ( lpDrawItemStruct->itemState == ODS_SELECTED &&
		 ( pTabCtrl->GetStyle()&TCS_FOCUSNEVER) == 0 )
	{
		CRect rect( rcItem );
		rect.DeflateRect( 2, 2 );
		dc.DrawFocusRect( rect );
	}

	dc.Detach();
}

void COXTabSkin2003::DrawBackground(CDC* pDC, LPCRECT lpRect, CTabCtrl* /*pTabCtrl*/)
{
	pDC->FillSolidRect(lpRect, m_pSkinXP->GetDockbarBackgroundColor());
}

void COXTabSkin2003::OnPaintTabCtrl(CTabCtrl* pTabCtrl)
{
	CPaintDC dc(pTabCtrl); // device context for painting
	// retrieve the client area size info
	CRect clientRect;
	pTabCtrl->GetClientRect(&clientRect);
	// create and initialize memory DC to draw on
	CDC memDc, itemMemDc;
	CBitmap bmpMem, bmpItemMem, *pOldItemMemBmp, *pOldMemBmp;
	bmpMem.CreateCompatibleBitmap( &dc, clientRect.right, clientRect.bottom );
	bmpItemMem.CreateCompatibleBitmap( &dc, clientRect.Width(), clientRect.Height());
	// create memory DC fot the entire tab control
	memDc.CreateCompatibleDC( &dc );
	pOldMemBmp = memDc.SelectObject( &bmpMem );
	// create memory DC for each item to draw on
	itemMemDc.CreateCompatibleDC( &dc );
	pOldItemMemBmp = itemMemDc.SelectObject( &bmpItemMem );

	// check if we have alredy subclassed the up-down control
	COXSkinnedSpinCtrl* pUpDown = GetSpinCtrl(pTabCtrl);
	if (pUpDown != NULL && pUpDown->GetSafeHwnd() == NULL )
	{
		// we have not, check if it was created by looping through all client windows
		int intnRet;
		CRect upDnRect;
		CWnd* pWnd = pTabCtrl->GetWindow(GW_CHILD);

		while(pWnd)
		{
			TCHAR buf[] = _T("msctls_updown32");
			intnRet= ::GetClassName(pWnd->m_hWnd,buf,sizeof(buf)/sizeof(buf[0]));
			if( _tcscmp(buf, _T("msctls_updown32")) != 0 )
			{
				pWnd=pWnd->GetWindow(GW_HWNDNEXT);
			}
			else
			{ 
				//The msctls_updown32 control isfound.
				//
				//Subclass common msctls_updown32 control
				pUpDown->SubclassWindow(pWnd->m_hWnd);

				pWnd=0;
			}
		}
	}

	// draw background of the entire control
	DrawBackground(&memDc, clientRect, pTabCtrl);
	
	// Draw the tab work area
	CRect rPage( clientRect );
	pTabCtrl->AdjustRect(FALSE, rPage);

	// Draw a border around the child window
	CRect rectChild;
	COX3DTabViewContainer* p3DTVC = DYNAMIC_DOWNCAST(COX3DTabViewContainer, pTabCtrl);
	if (p3DTVC != NULL)
	{
		rectChild = p3DTVC->m_rectPage;
		rectChild.InflateRect(1, 1);
	}
	else
	{
		rectChild = rPage;
		rectChild.bottom -= 1;
	}

	CBrush brushBorder;
	brushBorder.CreateSolidBrush(::GetSysColor(COLOR_3DSHADOW));
	memDc.FrameRect(rectChild, &brushBorder);
	
	// draw the solid line above the work area
	if ( pTabCtrl->GetRowCount() <= 1 )
	{
		COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;
		if ( pTabCtrl->GetStyle()&TCS_BOTTOM )
		{
			// draw background for the button area
			pSkin2003->GradientFill(&memDc, CRect(clientRect.left, rPage.bottom + TAB_MARGIN,
							  clientRect.right, clientRect.bottom),
				pSkin2003->GetTabBackgroundLightColor(),
				pSkin2003->GetTabBackgroundDarkColor(),
				GRADIENT_FILL_RECT_V);

			// draw a separator line
			memDc.SelectObject( ::GetStockObject( BLACK_PEN ));
			memDc.MoveTo( clientRect.left, rPage.bottom + TAB_MARGIN - 1 );
			memDc.LineTo( clientRect.right, rPage.bottom + TAB_MARGIN - 1 );
		}
		else
		{
			// draw background for the button area
			pSkin2003->GradientFill(&memDc, CRect(clientRect.left, clientRect.top,
							  clientRect.right, rPage.top - TAB_MARGIN),
				pSkin2003->GetTabBackgroundDarkColor(),
				pSkin2003->GetTabBackgroundLightColor(),
				GRADIENT_FILL_RECT_V);

			// draw a separator line
			memDc.SelectObject( ::GetStockObject( WHITE_PEN ));
			memDc.MoveTo( clientRect.left, rPage.top - TAB_MARGIN );
			memDc.LineTo( clientRect.right, rPage.top - TAB_MARGIN );
		}
	}

	// also paint the tabs
	int nTabCount = pTabCtrl->GetItemCount();
	int nSel = pTabCtrl->GetCurSel();

	// the tab control does not contain any items
	if ( nTabCount == 0 )
	{
		memDc.SelectObject( pOldMemBmp );
		itemMemDc.SelectObject( pOldItemMemBmp );
		return;
	}

	// Draw individual tab items
	DRAWITEMSTRUCT dis;
	dis.CtlType = ODT_TAB;
	dis.CtlID = pTabCtrl->GetDlgCtrlID();
	dis.hwndItem = pTabCtrl->GetSafeHwnd();
	dis.hDC = itemMemDc.GetSafeHdc();
	dis.itemAction = ODA_DRAWENTIRE;

	for ( int nIndex = 0; nIndex < nTabCount; nIndex++ )
	{
		// initialize the draw item structure for each of the tabs
		if (nIndex != nSel)
		{
			dis.itemID = nIndex;
			dis.itemState = 0;
			pTabCtrl->GetItemRect(nIndex, &dis.rcItem);
			DrawItem(&dis, pTabCtrl);
			DrawItemBorder(&dis, pTabCtrl);

			if ( AdjustItemRect( nIndex, &dis.rcItem, pTabCtrl ))
			{
				int width = dis.rcItem.right - dis.rcItem.left;
				int height = dis.rcItem.bottom - dis.rcItem.top;
				//Copy drawing from memory device context to the screen
				memDc.BitBlt( dis.rcItem.left,dis.rcItem.top,width,
					height,&itemMemDc,dis.rcItem.left,dis.rcItem.top,SRCCOPY);
			}
		}
	}

	// draw the selected item
	dis.itemID = nSel;
	dis.itemState = ODS_SELECTED;
	pTabCtrl->GetItemRect(nSel, &dis.rcItem);
	DrawItem(&dis, pTabCtrl);
	DrawItemBorder(&dis, pTabCtrl);

	if ( AdjustItemRect( nSel, &dis.rcItem, pTabCtrl ))
	{
		int width = dis.rcItem.right - dis.rcItem.left;
		int height = dis.rcItem.bottom - dis.rcItem.top;
		//Copy drawing from memory device context to the screen
		memDc.BitBlt( dis.rcItem.left,dis.rcItem.top,width,
			height,&itemMemDc,dis.rcItem.left,dis.rcItem.top,SRCCOPY);
	}

	//Copy drawing from memory device context to the screen
	dc.BitBlt( clientRect.left, clientRect.top, clientRect.Width(),
			   clientRect.Height(), &memDc,
			   clientRect.left, clientRect.top, SRCCOPY);

	// a final clean up
	memDc.SelectObject( pOldMemBmp );
	itemMemDc.SelectObject( pOldItemMemBmp );

	// make sure that the spin control is also redrawn
	if (pUpDown != NULL && pUpDown->GetSafeHwnd() != NULL )
		if ( pUpDown->IsWindowVisible())
			pUpDown->Invalidate();
}

void COXTabSkin2003::DrawItemBorder(LPDRAWITEMSTRUCT lpDrawItemStruct, CTabCtrl* pTabCtrl)
{
	CDC dc;
	dc.Attach( lpDrawItemStruct->hDC );

	if ( pTabCtrl->GetStyle()&TCS_BOTTOM )
	{
		if ( lpDrawItemStruct->itemState == ODS_SELECTED )
		{
			// draw the white line (on the left)
			dc.SelectObject( GetStockObject( WHITE_PEN ));
			dc.MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top );
			dc.LineTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom - 1 );
			dc.SelectObject( GetStockObject( BLACK_PEN ));
			// draw the bottom
			dc.LineTo( lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom - 1 );
			// draw the black outline on right
			dc.MoveTo( lpDrawItemStruct->rcItem.right - 1, lpDrawItemStruct->rcItem.top );
			dc.LineTo( lpDrawItemStruct->rcItem.right - 1, lpDrawItemStruct->rcItem.bottom - 1 );
		}
		else// draw the inactive button
		{
			if ( IsNextButtonSelected(lpDrawItemStruct->itemID, pTabCtrl) == FALSE )
			{	// draw the separators between the bottons
				CRect rectSep(lpDrawItemStruct->rcItem.right - 1,
					lpDrawItemStruct->rcItem.top + 3,
					lpDrawItemStruct->rcItem.right,
					lpDrawItemStruct->rcItem.bottom - 3);

				dc.FillSolidRect(rectSep,  m_pSkinXP->GetBorderColor());
			}

			// determine the position of this button in relation to the child window
			// it is important to know if the button is in the bottom or top and in which
			// row of tabs
			CRect rPage;
			pTabCtrl->GetClientRect( rPage );
			pTabCtrl->AdjustRect( FALSE, rPage );

			int nPosDifference = lpDrawItemStruct->rcItem.top - rPage.bottom;
			// Question 1, is the tab on top or bottom
			if ( nPosDifference > TAB_MARGIN )
			{	// the tab is on top, in a row other than the immediate below the client area
				// draw the separator line at the top of the item
				CPen *pOldBrush = dc.SelectObject( &m_sepPen );
				dc.MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top );
				dc.LineTo( lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.top );
				dc.SelectObject( pOldBrush );
			}
			else if ( nPosDifference >= 0 )
			{	// the tab is on top
				// draw the black line at the top
				dc.SelectObject( GetStockObject( BLACK_PEN ));
				dc.MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top );
				dc.LineTo( lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.top );
			}
			else
			{	// the tab is on the of the window
				// draw the separator line at the bottom of the item
				CPen *pOldBrush = dc.SelectObject( &m_sepPen );
				dc.MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom - 1 );
				dc.LineTo( lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom - 1 );
				dc.SelectObject( pOldBrush );
			}
		}
	}
	else
	{
		if ( lpDrawItemStruct->itemState == ODS_SELECTED )
		{
			// draw the white line (on the left)
			dc.SelectObject( GetStockObject( WHITE_PEN ));
			dc.MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom - 1 );
			dc.LineTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top );
			// draw the top
			dc.LineTo( lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.top );
			// draw the black outline on right
			dc.SelectObject( GetStockObject( BLACK_PEN ));
			dc.MoveTo( lpDrawItemStruct->rcItem.right - 1, lpDrawItemStruct->rcItem.top + 1 );
			dc.LineTo( lpDrawItemStruct->rcItem.right - 1, lpDrawItemStruct->rcItem.bottom - 1 );
		}
		else// draw the inactive button
		{
			if ( IsNextButtonSelected(lpDrawItemStruct->itemID, pTabCtrl) == FALSE )
			{
				CRect rectSep(lpDrawItemStruct->rcItem.right - 1,
					lpDrawItemStruct->rcItem.top + 3,
					lpDrawItemStruct->rcItem.right,
					lpDrawItemStruct->rcItem.bottom - 3);

				dc.FillSolidRect(rectSep,  m_pSkinXP->GetBorderColor());
			}

			// determine the position of this button in relation to the child window
			// it is important to know if the button is in the bottom or top and in which
			// row of tabs
			CRect rPage;
			pTabCtrl->GetClientRect( rPage );
			pTabCtrl->AdjustRect( FALSE, rPage );

			int nPosDifference = rPage.top - lpDrawItemStruct->rcItem.top;
			// Question 1, is the tab on top or bottom
			if ( nPosDifference > ( lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top + TAB_MARGIN ))
			{	// the tab is on top, in a row other than the immediate above the client area
				// draw the white line at the bottom
				CPen *pOldBrush = dc.SelectObject( &m_sepPen );
				dc.MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom - 1 );
				dc.LineTo( lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom - 1 );
				dc.SelectObject( pOldBrush );
			}
			else if ( nPosDifference > 0 )
			{	// the tab is on top
				// draw the white line at the bottom
				dc.SelectObject( GetStockObject( WHITE_PEN ));
				dc.MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.bottom - 1 );
				dc.LineTo( lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom - 1 );
			}
			else
			{	// the tab is on the bottom
				// draw the separator line above
				CPen *pOldBrush = dc.SelectObject( &m_sepPen );
				dc.MoveTo( lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top );
				dc.LineTo( lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.top );
				dc.SelectObject( pOldBrush );
			}
		}
	}

	dc.Detach();
}

void COXTabSkin2003::OnPaintSpinCtrl(COXSkinnedSpinCtrl* pSpinCtrl)
{
	CPaintDC dc(pSpinCtrl); // device context for painting
	// retrieve the client area size info
	CRect clientRect;
	pSpinCtrl->GetClientRect( clientRect );
	// create and initialize memory DC to draw on
	CDC itemMemDc;
	CBitmap bmpMem,*pOldItemMemBmp;
	itemMemDc.CreateCompatibleDC( &dc );
	bmpMem.CreateCompatibleBitmap( &dc,clientRect.Width(), clientRect.Height());
	pOldItemMemBmp = itemMemDc.SelectObject( &bmpMem );

	// Draw the background
	
	CTabCtrl* pTabCtrl = (CTabCtrl*) pSpinCtrl->GetParent();
	CRect rectClient;
	pTabCtrl->GetWindowRect(&rectClient);
	CRect rectPage(rectClient);
	pTabCtrl->AdjustRect(FALSE, rectPage);
	pSpinCtrl->ScreenToClient(rectClient);
	pSpinCtrl->ScreenToClient(rectPage);

	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;
	if (pTabCtrl->GetStyle() & TCS_BOTTOM)
	{
		// draw background for the button area
		pSkin2003->GradientFill(&itemMemDc, CRect(rectClient.left, rectPage.top + TAB_MARGIN,
						  rectClient.right, rectClient.bottom),
			pSkin2003->GetTabBackgroundLightColor(),
			pSkin2003->GetTabBackgroundDarkColor(),
			GRADIENT_FILL_RECT_V);
	}
	else
	{
		// draw background for the button area
		pSkin2003->GradientFill(&itemMemDc, CRect(rectClient.left, rectClient.top,
						  rectClient.right, rectPage.top),
			pSkin2003->GetTabBackgroundDarkColor(),
			pSkin2003->GetTabBackgroundLightColor(),
			GRADIENT_FILL_RECT_V);
	}

	// adjust the rect to provide extended border
	if (!( pTabCtrl->GetStyle()&TCS_BOTTOM ))
		clientRect.top += 2;
	else
		clientRect.bottom -= 2;

	clientRect.left += 2;
	clientRect.right -= 2;

	// calculate the sizes of the buttons
	int midPoint = ( clientRect.right - clientRect.left ) / 2;
	int btnWidth = midPoint - 1;
	CRect downRect( clientRect.left, clientRect.top, clientRect.left + btnWidth, clientRect.bottom );
	CRect upRect( midPoint, clientRect.top, midPoint + btnWidth, clientRect.bottom );

	DrawDownArrow(&itemMemDc, downRect, pSpinCtrl);
	DrawUpArrow(&itemMemDc, upRect, pSpinCtrl);

	CPoint mousePos;
	::GetCursorPos( &mousePos );
	pSpinCtrl->ScreenToClient( &mousePos );

	int lower, upper;
	pSpinCtrl->GetRange32( lower, upper );
	// draw frame
	if ( upRect.PtInRect( mousePos ) && LOWORD(pSpinCtrl->GetPos()) != upper )
	{
		DrawArrowBorder(&itemMemDc, upRect, pSpinCtrl);
	}
	else if ( downRect.PtInRect( mousePos ) && LOWORD(pSpinCtrl->GetPos()) != 0 )
	{
		DrawArrowBorder(&itemMemDc, downRect, pSpinCtrl);
	}
	//Copy drawing from memory device context to the screen
	dc.BitBlt( clientRect.left,clientRect.top,clientRect.Width(),
		clientRect.Height(),&itemMemDc,clientRect.left,clientRect.top,SRCCOPY);
	// a final clean up
	itemMemDc.SelectObject( pOldItemMemBmp );
}

// This function returns a pointer to the spin control of a tab container
COXSkinnedSpinCtrl* COXTabSkinXP::GetSpinCtrl(CTabCtrl* pTabContainer)
{
	COXSkinnedTabCtrl* pSkinnedTabCtrl = DYNAMIC_DOWNCAST(COXSkinnedTabCtrl, pTabContainer);
	if (pSkinnedTabCtrl != NULL)
		return &pSkinnedTabCtrl->m_updown;

	return NULL;
}

void COXTabSkinXP::OnPaintSpinCtrl(COXSkinnedSpinCtrl* pSpinCtrl)
{
	CPaintDC dc(pSpinCtrl); // device context for painting
	// retrieve the client area size info
	CRect clientRect;
	pSpinCtrl->GetClientRect( clientRect );
	// create and initialize memory DC to draw on
	CDC itemMemDc;
	CBitmap bmpMem,*pOldItemMemBmp;
	itemMemDc.CreateCompatibleDC( &dc );
	bmpMem.CreateCompatibleBitmap( &dc,clientRect.Width(), clientRect.Height());
	pOldItemMemBmp = itemMemDc.SelectObject( &bmpMem );

	// clear the background
	itemMemDc.FillSolidRect( clientRect,  m_pSkinXP->GetTabButtonsBackgroundColor() );
	// adjust the rect to provide extended border
	if (!( pSpinCtrl->GetParent()->GetStyle()&TCS_BOTTOM ))
		clientRect.top += 2;
	else
		clientRect.bottom -= 2;

	clientRect.left += 2;
	clientRect.right -= 2;

	// calculate the sizes of the buttons
	int midPoint = ( clientRect.right - clientRect.left ) / 2;
	int btnWidth = midPoint - 1;
	CRect downRect( clientRect.left, clientRect.top, clientRect.left + btnWidth, clientRect.bottom );
	CRect upRect( midPoint, clientRect.top, midPoint + btnWidth, clientRect.bottom );

	DrawDownArrow(&itemMemDc, downRect, pSpinCtrl);
	DrawUpArrow(&itemMemDc, upRect, pSpinCtrl);

	CPoint mousePos;
	::GetCursorPos( &mousePos );
	pSpinCtrl->ScreenToClient( &mousePos );

	int lower, upper;
	pSpinCtrl->GetRange32( lower, upper );
	// draw frame
	if ( upRect.PtInRect( mousePos ) && LOWORD(pSpinCtrl->GetPos()) != upper )
	{
		DrawArrowBorder(&itemMemDc, upRect, pSpinCtrl);
	}
	else if ( downRect.PtInRect( mousePos ) && LOWORD(pSpinCtrl->GetPos()) != 0 )
	{
		DrawArrowBorder(&itemMemDc, downRect, pSpinCtrl);
	}
	//Copy drawing from memory device context to the screen
	dc.BitBlt( clientRect.left,clientRect.top,clientRect.Width(),
		clientRect.Height(),&itemMemDc,clientRect.left,clientRect.top,SRCCOPY);
	// a final clean up
	itemMemDc.SelectObject( pOldItemMemBmp );
}

void COXTabSkinXP::DrawDownArrow(CDC* pDc, CRect btnRect, COXSkinnedSpinCtrl* pSpinCtrl)
{
	CBrush *pOldBrush = NULL;
	CPen *pOldPen = pDc->SelectObject( &m_btnLine );

	if ( LOWORD(pSpinCtrl->GetPos()) != 0 )
		pOldBrush = pDc->SelectObject( &m_btnFill );

	// draw the down arrow
	CSize arrowSize( 4, 8 );
	CPoint points[4];
	int xMargin = ( btnRect.right - btnRect.left - arrowSize.cx ) / 2;
	int yMargin = ( btnRect.bottom - btnRect.top - arrowSize.cy ) / 2;
	int midPoint = btnRect.top + yMargin + ( arrowSize.cy / 2 );
	points[0].x = btnRect.left + xMargin;
	points[0].y = midPoint - 1;
	points[1].x = btnRect.left + xMargin + arrowSize.cx;
	points[1].y = btnRect.top + yMargin - 1;
	points[2].x = btnRect.left + xMargin + arrowSize.cx;
	points[2].y = btnRect.top + yMargin + arrowSize.cy - 1;
	points[3].x = btnRect.left + xMargin;
	points[3].y = midPoint - 1;

	pDc->Polygon( points, 4 );

	// clean up
	if ( pOldBrush != NULL )
		pDc->SelectObject( pOldBrush );

	pDc->SelectObject( pOldPen );
}

void COXTabSkinXP::DrawUpArrow(CDC* pDc, CRect btnRect, COXSkinnedSpinCtrl* pSpinCtrl)
{
	int lower, upper;
	pSpinCtrl->GetRange32( lower, upper );
	CBrush *pOldBrush = NULL;
	CPen *pOldPen = pDc->SelectObject( &m_btnLine );

	if ( LOWORD(pSpinCtrl->GetPos()) != upper )
		pOldBrush = pDc->SelectObject( &m_btnFill );

	// draw the down arrow
	CSize arrowSize( 4, 8 );
	CPoint points[4];
	int xMargin = ( btnRect.right - btnRect.left - arrowSize.cx ) / 2;
	int yMargin = ( btnRect.bottom - btnRect.top - arrowSize.cy ) / 2;
	int midPoint = btnRect.top + yMargin + ( arrowSize.cy / 2 );
	points[0].x = btnRect.left + xMargin + arrowSize.cx;
	points[0].y = midPoint - 1;
	points[1].x = btnRect.left + xMargin;
	points[1].y = btnRect.top + yMargin - 1;
	points[2].x = btnRect.left + xMargin;
	points[2].y = btnRect.top + yMargin + arrowSize.cy - 1;
	points[3].x = btnRect.left + xMargin + arrowSize.cx;
	points[3].y = midPoint - 1;

	pDc->Polygon( points, 4 );

	// clean up
	if ( pOldBrush != NULL )
		pDc->SelectObject( pOldBrush );

	pDc->SelectObject( pOldPen );
}

// this function will get the proper rect for a tab, and it will check if it
// overlaps the up-down control.  If it does than the tab's rect will be adjusted.
BOOL COXTabSkinXP::AdjustItemRect(int /*nIndex*/, RECT *tabRect, CTabCtrl* pTabCtrl)
{
	// before drawing item, check if it overlaps the spin control
	COXSkinnedSpinCtrl* pUpDown = GetSpinCtrl(pTabCtrl);
	if (pUpDown != NULL && pUpDown->GetSafeHwnd() != NULL )
	{
		if ( pUpDown->IsWindowVisible())
		{
			CRect spinRect, tabclientRect;
			pUpDown->GetClientRect( spinRect );
			pTabCtrl->GetClientRect( tabclientRect );

			if ( tabRect->left > ( tabclientRect.right - spinRect.Width()))
				return FALSE;

			if ( tabRect->right <= ( tabclientRect.left + TAB_MARGIN ))
				return FALSE;

			if ( tabRect->right > ( tabclientRect.right - spinRect.Width()))
				// the right edge of the tab button extends over the up-down control
				tabRect->right = tabclientRect.right - spinRect.Width();
		}
	}
	
	return TRUE;
}

BOOL COXTabSkinXP::IsNextButtonSelected(int nItemID, CTabCtrl* pTabCtrl)
{
	int nCount = pTabCtrl->GetItemCount();

	if ( nItemID < nCount - 1 )
	{
		if ( pTabCtrl->GetItemState( nItemID + 1, TCIS_BUTTONPRESSED ) == TCIS_BUTTONPRESSED )
			return TRUE;
	}
	return FALSE;
}

void COXTabSkinXP::DrawArrowBorder(CDC* pDc, CRect btnRect, COXSkinnedSpinCtrl* /*pSpinCtrl*/)
{
	btnRect.DeflateRect( 1, 1 );

	if ( GetKeyState( VK_LBUTTON ) < 0 )
	{
		// 0,0,0
		pDc->SelectObject( GetStockObject( BLACK_PEN ));
		pDc->MoveTo( btnRect.left, btnRect.bottom );
		pDc->LineTo( btnRect.left, btnRect.top );
		pDc->LineTo( btnRect.right, btnRect.top );
		// 241,239,226
		pDc->SelectObject( GetStockObject( WHITE_PEN ));
		pDc->LineTo( btnRect.right, btnRect.bottom );
		pDc->LineTo( btnRect.left, btnRect.bottom );
	}
	else
	{
		// 241,239,226
		pDc->SelectObject( GetStockObject( WHITE_PEN ));
		pDc->MoveTo( btnRect.left, btnRect.bottom );
		pDc->LineTo( btnRect.left, btnRect.top );
		pDc->LineTo( btnRect.right, btnRect.top );
		// 0,0,0
		pDc->SelectObject( GetStockObject( BLACK_PEN ));
		pDc->LineTo( btnRect.right, btnRect.bottom );
		pDc->LineTo( btnRect.left, btnRect.bottom );
	}
}


// COXMenuSkin
//


// COXMenuSkinClassic
//

#include "OXBitmapMenu.h"
#include "OXBitmapMenuOrganizer.h"

COXMenuSkinClassic::COXMenuSkinClassic()
{
	// Initialize the dimension constants structure
	m_DimensionConstants.m_nGapLeftBitmap = 2;
	m_DimensionConstants.m_nGapBitmapText = 8;	
	m_DimensionConstants.m_nGapTextAcclrtr = 2;	
	m_DimensionConstants.m_nGapAcclrtrRight = 3;
	m_DimensionConstants.m_nGapVertText = 4;	
	m_DimensionConstants.m_nGapVertBitmap = 3;
	m_DimensionConstants.m_nSeparatorHeight = ID_OXBITMAPMENU_SEPARATOR_HEIGHT;
	m_DimensionConstants.m_nMinBitmapWidth = 0;
	m_DimensionConstants.m_nGapMenuItems = 12;
}

void COXMenuSkinClassic::DistributeSpace(UINT /*nState*/, COXImageInfo* pImageInfo, LPRECT lpItemRect,
										 CRect& rectButton, CRect& rectImage, CRect& rectText1,
										 CRect& rectText2, COXBitmapMenu* pBitmapMenu)
{
	CRect rectItem(lpItemRect);
	OXDIMENSIONCONSTANTS& oxdc = GetDimentionConstants();

	// Compute the image and button rectangle
	if(pImageInfo != NULL)
	{
		// Get the actual bitmap rect
		IMAGEINFO ii;
		::ZeroMemory(&ii, sizeof(ii));
		pImageInfo->GetImageList()->GetImageInfo(pImageInfo->GetIndex(), &ii);
		
		// Center it vertically in the item rect
		CRect bitmapRect= ii.rcImage;
		bitmapRect -= bitmapRect.TopLeft();
		int nVertGap=(rectItem.Height() - bitmapRect.Height()) / 2;

		ASSERT(bitmapRect.Width() <= rectItem.Width());

		rectImage=bitmapRect;
		rectImage += rectItem.TopLeft();
		rectImage += CPoint(oxdc.m_nGapLeftBitmap, nVertGap);

		// Compute the button rectangle
		rectButton=rectImage;
		rectButton.InflateRect(2, 2);
	}

	// Compute the rectangle for the text in front of the tab 
	rectText1=rectItem;
	rectText1.left=oxdc.m_nGapLeftBitmap + pBitmapMenu->m_nBitmapExtent + oxdc.m_nGapBitmapText;
	rectText1.right=rectText1.left + pBitmapMenu->m_nStringExtent;

	// Compute the rectangle for the text after the tab (shortcut key)
	rectText2=rectItem;
	rectText2.right -= oxdc.m_nGapAcclrtrRight;
	rectText2.left=rectText2.right - pBitmapMenu->m_nAcceleratorExtent;
	int nGapTextHorz=(rectText2.left - rectText1.right) / 2;
	rectText2 -= CPoint(nGapTextHorz, 0);

	// Center the text vertically
	int nGapTextVert=(rectItem.Height() - pBitmapMenu->m_nTextHeight) / 2;
	rectText1.InflateRect(0, -nGapTextVert);
	rectText2.top=rectText1.top;
	rectText2.bottom=rectText1.bottom;
}

void COXMenuSkinClassic::DrawBackground(CDC* pDC, UINT nState, COXImageInfo* pImageInfo, LPRECT lpItemRect, LPRECT lpButtonRect, COXBitmapMenu* pBitmapMenu)
{
	CRect itemRect(lpItemRect);
	CRect buttonRect(lpButtonRect);

	BOOL bSelected=nState & ODS_SELECTED;
	BOOL bChecked=nState & ODS_CHECKED;
	BOOL bHidden=nState & OXODS_HIDDEN;

	COLORREF clrBackground=::GetSysColor(bSelected ? COLOR_HIGHLIGHT : COLOR_MENU); 

	if((bHidden && !pImageInfo->IsDefaultImgList()) || (bChecked && !bSelected))
	{
		HBRUSH hBrush=pBitmapMenu->HBrushDitherCreate(::GetSysColor(COLOR_MENU),
			::GetSysColor(COLOR_BTNHIGHLIGHT));
		if(hBrush!=NULL)
		{
			::FillRect(pDC->GetSafeHdc(),buttonRect,hBrush);
			::DeleteObject((HGDIOBJ)hBrush);
		}
	}
	else
	{
		// Draw the image on a special background depending on selection, 
		// check state and Default image or not
		COLORREF clrBtnBackground=::GetSysColor(bSelected ? 
			(bChecked ? COLOR_MENU : (pImageInfo->IsDefaultImgList() ? 
			COLOR_HIGHLIGHT : COLOR_MENU)) : 
			(bChecked ? COLOR_3DHILIGHT : COLOR_MENU)); 
		pDC->FillSolidRect(&buttonRect, clrBtnBackground);
	}

	if(!bHidden || bSelected)
	{
		itemRect.left=__max(buttonRect.right, m_DimensionConstants.m_nGapLeftBitmap + pBitmapMenu->m_nBitmapExtent + 
			(!bChecked ? (pImageInfo->IsDefaultImgList() ? -1 : 1) : 2));
		itemRect.bottom -= 1;
		CBrush brush;
		brush.CreateSolidBrush(clrBackground);
		pDC->FillRect(&itemRect,&brush);
	}
	else
	{
		HBRUSH hBrush=pBitmapMenu->HBrushDitherCreate(::GetSysColor(COLOR_MENU),
			::GetSysColor(COLOR_BTNHIGHLIGHT));
		if(hBrush!=NULL)
		{
			::FillRect(pDC->GetSafeHdc(),itemRect,hBrush);
			::DeleteObject((HGDIOBJ)hBrush);
		}
	}

	if(bHidden && !bSelected)
	{
		if(nState & OXODS_HIDDENFIRST)
		{
			CPen pen(PS_SOLID,1,::GetSysColor(COLOR_3DSHADOW));
			CPen* pOldPen=pDC->SelectObject(&pen);
			pDC->MoveTo(itemRect.TopLeft());
			pDC->LineTo(itemRect.right,itemRect.top);
			pDC->SelectObject(pOldPen);
		}

		if(nState & OXODS_HIDDENLAST)
		{
			CPen pen(PS_SOLID,1,::GetSysColor(COLOR_3DHILIGHT));
			CPen* pOldPen=pDC->SelectObject(&pen);
			pDC->MoveTo(itemRect.left,itemRect.bottom-1);
			pDC->LineTo(itemRect.right,itemRect.bottom-1);
			pDC->SelectObject(pOldPen);
		}
	}
}

void COXMenuSkinClassic::DrawButton(CDC* pDC, UINT nState,  COXImageInfo* pImageInfo, LPRECT lpButtonRect, COXBitmapMenu* /*pBitmapMenu*/)
{
	BOOL bSelected=nState & ODS_SELECTED;
	BOOL bDisabled=nState & ODS_DISABLED;
	BOOL bChecked=nState & ODS_CHECKED;

	// Draw edge around the image according to the state of the item
	// (The background has already been filled with the right color)
	if(bSelected && !bDisabled && !bChecked && !pImageInfo->IsDefaultImgList())
	{
		pDC->DrawEdge(lpButtonRect, BDR_RAISEDINNER, BF_RECT);
	}
	else if(bChecked)
	{
		pDC->DrawEdge(lpButtonRect, BDR_SUNKENOUTER, BF_RECT);
	}
}

void COXMenuSkinClassic::DrawImage(CDC* pDC, UINT nState, COXImageInfo* pImageInfo, LPRECT lpImageRect, COXBitmapMenu* /*pBitmapMenu*/)
{
	CRect imageRect(lpImageRect);

	BOOL bDisabled=nState & ODS_DISABLED;
	BOOL bChecked=nState & ODS_CHECKED;
	BOOL bSelected=nState & ODS_SELECTED;

	if(pImageInfo->IsDefaultImgList() && !bChecked)
	{
		// The default checkmark is not drawn when it isn't checked
		return;
	}

	int nIndex=pImageInfo->GetIndex();
	if(bSelected && pImageInfo->GetHotIndex()!=-1)
	{
		nIndex=pImageInfo->GetHotIndex();
	}

	if(!bDisabled)
	{
		pImageInfo->GetImageList()->Draw(pDC, nIndex, 
			imageRect.TopLeft(), ILD_NORMAL|ILD_TRANSPARENT); 
	}
	else
	{
		HICON hIcon=pImageInfo->GetImageList()->ExtractIcon(nIndex);
		pDC->DrawState(
			imageRect.TopLeft(),imageRect.Size(),hIcon,DSS_DISABLED,(HBRUSH)NULL);
		VERIFY(::DestroyIcon(hIcon));
	}
}

void COXMenuSkinClassic::DrawText(CDC* pDC, UINT nState, LPCTSTR lpszText, LPRECT lpText1Rect, LPRECT lpText2Rect, COXBitmapMenu* /*pBitmapMenu*/)
{
	CRect text1Rect(lpText1Rect);
	CRect text2Rect(lpText2Rect);

	BOOL bSelected=nState & ODS_SELECTED;
	BOOL bDisabled=nState & ODS_DISABLED;

	// Split the text (in front of and after the tab)
	CString sText(lpszText);
	int nTabCharIndex=sText.Find(_T('\t'));
	CString sAfterTab;
	CString sBeforeTab;
	if(nTabCharIndex != -1)
	{
		sBeforeTab=sText.Left(nTabCharIndex);
		sAfterTab=sText.Mid(nTabCharIndex + 1);
	}
	else
	{
		sBeforeTab=sText;
		ASSERT(sAfterTab.IsEmpty());
	}

	// Draw the text
	// Sets the foreground color of the text based on its selection state
	COLORREF clrForeground=::GetSysColor(bDisabled ? COLOR_GRAYTEXT : 
		(bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT)); 
	COLORREF clrOldForeground=pDC->SetTextColor(clrForeground);

	// set background filling mode
	int nOldBkMode=pDC->SetBkMode(TRANSPARENT);

	UINT nFlags=0;
	CRect newText2Rect(0,0,0,0);
	int nHorzAlign=0;
	if(bDisabled && !bSelected)
		nFlags |= DSS_DISABLED;

	pDC->DrawState(text1Rect.TopLeft(), text1Rect.Size(), sBeforeTab.GetBuffer(0),
		nFlags, TRUE, sBeforeTab.GetLength(), (CBrush*)NULL);

	// ... First calculate text width so we can right-align it
	pDC->DrawText(sAfterTab, newText2Rect, DT_CALCRECT | DT_SINGLELINE);
	nHorzAlign=text2Rect.Width() - newText2Rect.Width();
	if(!sAfterTab.IsEmpty())
	{
		pDC->DrawState(text2Rect.TopLeft() + CPoint(nHorzAlign, 0), 
			newText2Rect.Size(), sAfterTab.GetBuffer(0),
			nFlags, TRUE, sAfterTab.GetLength(), (CBrush*)NULL);
	}

	pDC->SetTextColor(clrOldForeground); 
	pDC->SetBkMode(nOldBkMode);
}

void COXMenuSkinClassic::DrawSeparator(CDC* pDC, LPRECT lpItemRect, COXBitmapMenu* /*pBitmapMenu*/)
{
	CRect itemRect(lpItemRect);
	ASSERT(pDC!=NULL);
	itemRect.DeflateRect(2,0);
	itemRect.top+=itemRect.Height()/2-1;
	itemRect.bottom=itemRect.top+2;
	pDC->Draw3dRect(itemRect,::GetSysColor(COLOR_BTNSHADOW),
		::GetSysColor(COLOR_BTNHILIGHT));
}

void COXMenuSkinClassic::DrawCustomized(CDC* pDC, LPRECT lpItemRect, COXBitmapMenu* /*pBitmapMenu*/)
{
	ASSERT(pDC!=NULL);
	pDC->DrawDragRect(lpItemRect,CSize(2,2),CRect(0,0,0,0),CSize(0,0),NULL,NULL);
}

void COXMenuSkinClassic::DrawInsertMark(CDC* pDC, LPRECT lpItemRect, BOOL bBefore, COXBitmapMenu* /*pBitmapMenu*/)
{
	CRect itemRect(lpItemRect);
	itemRect.DeflateRect(2,0);

	ASSERT(pDC!=NULL);
	if(bBefore)
	{
		itemRect.top-=2;
		itemRect.bottom=itemRect.top+3;
	}
	else
	{
		itemRect.bottom+=1;
		itemRect.top=itemRect.bottom-3;
	}

	pDC->MoveTo(itemRect.TopLeft());
	pDC->LineTo(itemRect.left+1,itemRect.top+1);
	pDC->LineTo(itemRect.right-1,itemRect.top+1);
	pDC->LineTo(itemRect.right,itemRect.top);
	pDC->LineTo(itemRect.right,itemRect.bottom);
	pDC->LineTo(itemRect.right-1,itemRect.bottom-1);
	pDC->LineTo(itemRect.left+1,itemRect.bottom-1);
	pDC->LineTo(itemRect.left,itemRect.bottom);
	pDC->LineTo(itemRect.TopLeft());
}

void COXMenuSkinClassic::DrawSubmenuItem(CDC* pDC, UINT nState, LPCTSTR lpszText, COXImageInfo* pImageInfo, 
									LPRECT lpItemRect, LPRECT lpButtonRect, LPRECT lpText1Rect, LPRECT lpText2Rect, COXBitmapMenu* pBitmapMenu)
{
	ASSERT(pDC!=NULL);
	pBitmapMenu->DrawBackground(pDC,nState,pImageInfo,lpItemRect,lpButtonRect);

	if(pBitmapMenu->IsInCustomizationMode())
	{
		// draw right aligned arrow
		//
		COLORREF clrShadow=::GetSysColor(COLOR_BTNSHADOW);
		CFont font;
		CFont* pOldFont=NULL;
		if((font.CreateFont(GetSystemMetrics(SM_CYMENUCHECK), 0, 0, 0, 
			FW_NORMAL, 0, 0, 0, SYMBOL_CHARSET, 0,0,0,0, _T("Marlett"))) != NULL)
		{
			pOldFont=(CFont*)pDC->SelectObject(&font);
		}
		int nOldBkMode=pDC->SetBkMode(TRANSPARENT);
		COLORREF oldTextColor=CLR_NONE;
		if((nState&ODS_DISABLED)==ODS_DISABLED)
		{
			oldTextColor=pDC->SetTextColor(clrShadow);
		}
		// draw arrow symbol
		pDC->DrawText(_T("8"),lpText2Rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		if((nState&ODS_DISABLED)==ODS_DISABLED)
		{
			pDC->SetTextColor(oldTextColor);
		}
		pDC->SetBkMode(nOldBkMode);
		if(pOldFont != NULL)
		{
			pDC->SelectObject(pOldFont);
		}
		//
		//////////////////////////////
	}

	DrawText(pDC,nState,lpszText,lpText1Rect,lpText2Rect,pBitmapMenu);
}

LRESULT COXMenuSkinClassic::MenuPopupWndProc(WNDPROC origWndProc, HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	// Call the default
	return ::CallWindowProc(origWndProc, hWnd, nMsg, wParam, lParam);
}

void COXMenuSkinClassic::DrawExpansionItem(CDC* pDC, LPRECT lpItemRect, UINT nState, COXBitmapMenu* /*pBitmapMenu*/)
{
	ASSERT(pDC!=NULL);

	BOOL bSelected=nState & ODS_SELECTED;
	BOOL bDisabled=nState & ODS_DISABLED;

	CFont font;
	VERIFY(font.CreateFont(GetSystemMetrics(SM_CYMENUCHECK),0,0,0,
		FW_NORMAL,0,0,0,SYMBOL_CHARSET,0,0,0,0,_T("Marlett")));
	CFont* pOldFont=pDC->SelectObject(&font);
	int nOldBkMode=pDC->SetBkMode(TRANSPARENT);
	COLORREF clrText=::GetSysColor(bDisabled ? COLOR_GRAYTEXT : 
		(bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT)); 
	COLORREF oldTextColor=pDC->SetTextColor(clrText);

	pDC->DrawText(_T("6"),lpItemRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	pDC->SetTextColor(oldTextColor);
	pDC->SetBkMode(nOldBkMode);
	pDC->SelectObject(pOldFont);
}

// COXMenuSkinXP
//

COXMenuSkinXP::COXMenuSkinXP(COXSkinXP* pSkinXP)
{
	m_pSkinXP = pSkinXP;

	// Initialize the dimension constants structure
	m_DimensionConstants.m_nGapLeftBitmap = 3;
	m_DimensionConstants.m_nGapRightBitmap = 5;
	m_DimensionConstants.m_nGapBitmapText = 15;	
	m_DimensionConstants.m_nGapTextAcclrtr = 12;	
	m_DimensionConstants.m_nGapAcclrtrRight = 6;
	m_DimensionConstants.m_nGapVertText = 10;	
	m_DimensionConstants.m_nGapVertBitmap = 4;
	m_DimensionConstants.m_nSeparatorHeight = 3;
	m_DimensionConstants.m_nMinBitmapWidth = 16;
	m_DimensionConstants.m_nGapMenuItems = 16;
}

COXMenuSkinXP::~COXMenuSkinXP()
{
	m_mapHWNDtoPos.RemoveAll();
}

void COXMenuSkinXP::DistributeSpace(UINT /*nState*/, COXImageInfo* pImageInfo, LPRECT lpItemRect,
										 CRect& rectButton, CRect& rectImage, CRect& rectText1,
										 CRect& rectText2, COXBitmapMenu* pBitmapMenu)
{
	CRect rectItem(lpItemRect);
	OXDIMENSIONCONSTANTS& oxdc = GetDimentionConstants();

	// Compute the image and button rectangle
	if(pImageInfo != NULL)
	{
		// Get the actual bitmap rect
		IMAGEINFO ii;
		::ZeroMemory(&ii, sizeof(ii));
		pImageInfo->GetImageList()->GetImageInfo(pImageInfo->GetIndex(), &ii);
		
		// Center it vertically in the item rect
		CRect bitmapRect= ii.rcImage;
		bitmapRect -= bitmapRect.TopLeft();
		int nVertGap=(rectItem.Height() - bitmapRect.Height()) / 2;

		ASSERT(bitmapRect.Width() <= rectItem.Width());

		rectImage=bitmapRect;
		rectImage += rectItem.TopLeft();
		rectImage += CPoint(oxdc.m_nGapLeftBitmap, nVertGap);

		// Compute the button rectangle
		rectButton = rectImage;
		rectButton.left = rectItem.left + 1;
		rectButton.top = rectItem.top + 1;
		rectButton.bottom = rectItem.bottom - 1;
		rectButton.right = rectButton.left + pBitmapMenu->m_nBitmapExtent + 4;
		if (pImageInfo->IsDefaultImgList())
		{
			// This is a check box
			// The image rectangle needs to be in the middle of the button
			int iHorizontalDiff = rectButton.Width() - rectImage.Width();
			rectImage.OffsetRect(iHorizontalDiff / 2 - 1, 0);
		}
	}

	// Compute the rectangle for the text in front of the tab 
	rectText1=rectItem;
	rectText1.left=oxdc.m_nGapLeftBitmap + pBitmapMenu->m_nBitmapExtent + oxdc.m_nGapBitmapText;
	rectText1.right=rectText1.left + pBitmapMenu->m_nStringExtent;

	// Compute the rectangle for the text after the tab (shortcut key)
	rectText2=rectItem;
	rectText2.right -= oxdc.m_nGapAcclrtrRight;
	rectText2.left=rectText2.right - pBitmapMenu->m_nAcceleratorExtent;
	int nGapTextHorz=(rectText2.left - rectText1.right) / 2;
	rectText2 -= CPoint(nGapTextHorz, 0);

	// Center the text vertically
	int nGapTextVert=(rectItem.Height() - pBitmapMenu->m_nTextHeight) / 2;
	rectText1.InflateRect(0, -nGapTextVert);
	rectText2.top=rectText1.top;
	rectText2.bottom=rectText1.bottom;
}

void COXMenuSkinXP::DrawBackground(CDC* pDC, UINT nState, COXImageInfo* /*pImageInfo*/, LPRECT lpItemRect, LPRECT /*lpButtonRect*/, COXBitmapMenu* pBitmapMenu)
{
	bool bSelected = (nState & ODS_SELECTED) != 0;
	bool bHidden = (nState & OXODS_HIDDEN) != 0;
	bool bDisabled = (nState & ODS_DISABLED) != 0;

	if(bSelected && !bDisabled)
	{
		// Selected item
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, m_pSkinXP->GetActiveBorderColor());
		CBrush brush;
		brush.CreateSolidBrush(m_pSkinXP->GetHotItemColor());

		CPen* pOldPen = pDC->SelectObject(&pen);
		CBrush* pOldBrush = pDC->SelectObject(&brush);
		pDC->Rectangle(lpItemRect);
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
	else
	{
		// Normal item
		CBrush brushFace;
		brushFace.CreateSolidBrush(m_pSkinXP->GetMenuFaceColor());
		pDC->FillRect(lpItemRect, &brushFace);

		CBrush brushIconArea;
		if (bHidden)
			brushIconArea.CreateSolidBrush(m_pSkinXP->GetHiddenItemBackgroundColor());
		else
			brushIconArea.CreateSolidBrush(m_pSkinXP->GetBackgroundColor());
		CRect rectIconArea(lpItemRect);
		rectIconArea.right = m_DimensionConstants.m_nGapLeftBitmap + pBitmapMenu->m_nBitmapExtent + m_DimensionConstants.m_nGapRightBitmap;
		pDC->FillRect(rectIconArea, &brushIconArea);
	}
}

void COXMenuSkinXP::AdjustExpansionItemHeight(UINT& nItemHeight, COXBitmapMenu* /*pBitmapMenu*/)
{
	nItemHeight = nItemHeight / 2 + 1;
}

void COXMenuSkinXP::DrawButton(CDC* pDC, UINT nState,  COXImageInfo* /*pImageInfo*/, LPRECT lpButtonRect, COXBitmapMenu* /*pBitmapMenu*/)
{
	BOOL bSelected=nState & ODS_SELECTED;
	BOOL bChecked=nState & ODS_CHECKED;
	if (!bChecked)
		return;

	// Set up the brish and the pen
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, m_pSkinXP->GetActiveBorderColor());
	CBrush brush;
	if (bSelected)
		brush.CreateSolidBrush(m_pSkinXP->GetHotSelectedItemColor());
	else
		brush.CreateSolidBrush(m_pSkinXP->GetCheckedItemColor());

	// Draw the rectangle
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	pDC->Rectangle(lpButtonRect);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

void COXMenuSkinXP::DrawImage(CDC* pDC, UINT nState, COXImageInfo* pImageInfo, LPRECT lpImageRect, COXBitmapMenu* /*pBitmapMenu*/)
{
	BOOL bDisabled=nState & ODS_DISABLED;
	BOOL bChecked=nState & ODS_CHECKED;
	BOOL bSelected=nState & ODS_SELECTED;

	if(pImageInfo->IsDefaultImgList() && !bChecked)
	{
		// The default checkmark is not drawn when it isn't checked
		return;
	}

	CRect imageRect(lpImageRect);
	int nIndex=pImageInfo->GetIndex();
	if(bSelected && pImageInfo->GetHotIndex()!=-1)
	{
		nIndex=pImageInfo->GetHotIndex();
	}

	if (bDisabled)
	{
		// Draw a gray image
		CBrush brush;
		brush.CreateSolidBrush(::GetSysColor(COLOR_BTNSHADOW));
		HICON hIcon = pImageInfo->GetImageList()->ExtractIcon(nIndex);
		if (hIcon != NULL)
		{
			pDC->DrawState(imageRect.TopLeft(), imageRect.Size(), hIcon, DST_ICON | DSS_MONO, &brush);
			::DestroyIcon(hIcon);
		}
	}
	else if (bSelected && !bChecked)
	{
		// Raised with a shadow
		CImageList* pImageList = pImageInfo->GetImageList();
		CPoint ptTopLeft = imageRect.TopLeft();

		// Draw the image shadow first
		ptTopLeft.x += 1;
		ptTopLeft.y += 1;
		CBrush brush;
		brush.CreateSolidBrush(::GetSysColor(COLOR_BTNSHADOW));
		CBrush* pOldBrush = pDC->SelectObject(&brush);		
		COLORREF clrOldBkColor = pDC->SetBkColor(RGB(255, 255, 255));
		pImageList->DrawIndirect(pDC, nIndex, ptTopLeft, imageRect.Size(),
			CPoint(0, 0), ILD_ROP | ILD_MASK, 0x00B8074A);
		pDC->SetBkColor(clrOldBkColor);
		pDC->SelectObject(pOldBrush);
		
		ptTopLeft.x -= 2;
		ptTopLeft.y -= 2;

		// Draw the image
		pImageList->Draw(pDC, nIndex, ptTopLeft, ILD_TRANSPARENT);
	}
	else
	{
		// Normal image
		pImageInfo->GetImageList()->Draw(pDC, nIndex, imageRect.TopLeft(), ILD_TRANSPARENT);
	}
}


COLORREF COXMenuSkinXP::DarkenColor(long lScale, COLORREF lColor)
{ 
	long R = MulDiv(GetRValue(lColor), (255 - lScale), 255);
	long G = MulDiv(GetGValue(lColor), (255 - lScale), 255);
	long B = MulDiv(GetBValue(lColor), (255 - lScale), 255);

	return RGB(R, G, B); 
}

// Wrapper function for ::GetPixel(...) that facilitates the shadow buffering
COLORREF GetShadowPixel(HDC hDC, int iXPos, int iYPos, bool bUseSavedShadow)
{
	if (bUseSavedShadow)
		return 0; // we don't need this pixel

	return ::GetPixel(hDC, iXPos, iYPos);

}

// Wrapper function for ::SetPixelV(...) that facilitates the shadow buffering
void SetShadowPixel(HDC hDC, int iXPos, int iYPos, COLORREF clr, bool bSaveShadow, bool bUseSavedShadow, OXSHADOWARRAY* pShadowArray, int& iIndex)
{
	if (bUseSavedShadow)
		::SetPixelV(hDC, iXPos, iYPos, pShadowArray->GetAt(iIndex++));
	else
		::SetPixelV(hDC, iXPos, iYPos, clr);

	if (bSaveShadow)
		pShadowArray->Add(clr);
}

// Draws a menu shadow for the given rectangle
void COXMenuSkinXP::DrawMenuShadow(HDC hDC, LPRECT lpRect, LPRECT lpItemRect, OXSHADOWARRAY* pShadowArray)
{
	int iArrayIndex = 0;
	bool bSaveShadow, bUseSavedShadow;
	if (pShadowArray == NULL)
	{
		// We don't need to use the shadow array
		bSaveShadow = false;
		bUseSavedShadow = false;
	}
	else if (pShadowArray->GetSize() == 0)
	{
		// Save the shadow in the array
		bSaveShadow = true;
		bUseSavedShadow = false;
	}
	else
	{
		// Use the shadow from the array
		bSaveShadow = false;
		bUseSavedShadow = true;
	}

	CRect rectMenuItem(lpItemRect); // the rectangle of the menu item in screen coordinates
	CRect rectMenu(lpRect);

	// Draw the shadow - get the pixels from the desktop, darken them
	// and place them on the popup window
	HDC hDesktopDC = ::GetWindowDC(::GetDesktopWindow());

	// copy desktop to mem DC and work with that for Get/Set pixel (Vista bug)
	CDC* pDC = CDC::FromHandle(hDesktopDC);

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rectMenu.Width(), rectMenu.Height());
	CBitmap *pOldBitmap = dcMem.SelectObject(&bitmap);
	
	// working with a mem bitmap at 0,0 for GetPixel
	dcMem.BitBlt(0,0, rectMenu.Width(), rectMenu.Height(), pDC, rectMenu.left, rectMenu.top, SRCCOPY);

	int x, y;
	CPoint pt;
	COLORREF clr;

	int iLeft = 4;	
	int iTop =  4;
	int iRight = rectMenu.Width();
	int iBottom = rectMenu.Height();

	// Right shadow 
	for (x = 1; x <= 4; x++)
	{ 
		// Copy the top right pixels
		for (y = 1; y <= 4; y++)
		{
			pt.x = iRight - x;
			pt.y = iTop + y - 4 - 1;
			clr = GetShadowPixel(dcMem.m_hDC, pt.x, pt.y, bUseSavedShadow);
			SetShadowPixel(hDC, pt.x, pt.y, clr, bSaveShadow, bUseSavedShadow, pShadowArray, iArrayIndex);
		}

		// Top right corner
		for (y = 4; y >= 1; y--)
		{
			pt.x = iRight - x;
			pt.y = iTop + y - 1;
			clr = GetShadowPixel(dcMem.m_hDC, pt.x, pt.y, bUseSavedShadow);
			if (!rectMenuItem.PtInRect(pt))
			{
				// Darken the pixel
				SetShadowPixel(hDC, pt.x, pt.y, DarkenColor(3 * x * y, clr), bSaveShadow, bUseSavedShadow, pShadowArray, iArrayIndex);
			}
		}

		// Vertical line
		for (y = iTop + 4; y <= iBottom - 4 - 1; y++)
		{
			pt.x = iRight - x;
			pt.y = y;
			clr = GetShadowPixel(dcMem.m_hDC, pt.x, pt.y, bUseSavedShadow);
			// TD review this ...
//			if (!rectMenuItem.PtInRect(pt))
//			{
				// Darken the pixel
				SetShadowPixel(hDC, pt.x, pt.y, DarkenColor(15 * x, clr), bSaveShadow, bUseSavedShadow, pShadowArray, iArrayIndex);
//			}
		}

		// Bottom right corner
		for (y = 1; y <= 4; y++)
		{
			pt.x = iRight - x;
			pt.y = iBottom - y;
			clr = GetShadowPixel(dcMem.m_hDC,  pt.x, pt.y, bUseSavedShadow);
			SetShadowPixel(hDC, pt.x, pt.y, DarkenColor(3 * x * y,  clr), bSaveShadow, bUseSavedShadow, pShadowArray, iArrayIndex);
		}
	} 

	// Bottom shadow
	for (y = 1; y <= 4; y++)
	{
		// Copy the bottom left pixels
		for (x = 1; x <= 4; x++)
		{
			pt.x = iLeft - x;
			pt.y = iBottom - y;
			clr = GetShadowPixel(dcMem.m_hDC, pt.x, pt.y, bUseSavedShadow);
			SetShadowPixel(hDC, pt.x, pt.y, clr, bSaveShadow, bUseSavedShadow, pShadowArray, iArrayIndex);
		}

		// Bottom left corner
		for (x = 1; x <= 4; x++)
		{
			pt.x = iLeft - x + 4;
			pt.y = iBottom - y;
			clr = GetShadowPixel(dcMem.m_hDC, pt.x, pt.y, bUseSavedShadow);
			if (!rectMenuItem.PtInRect(pt))
			{
				// Darken the pixel
				SetShadowPixel(hDC, pt.x, pt.y, DarkenColor(3 * (5 - x) * y, clr), bSaveShadow, bUseSavedShadow, pShadowArray, iArrayIndex);
			}
		}
		
		// Horizontal line
		for (x = iLeft + 4; x <= iRight - 5; x++)
		{
			pt.x = x;
			pt.y = iBottom - y;
			clr = GetShadowPixel(dcMem.m_hDC, pt.x, pt.y, bUseSavedShadow);
			if (!rectMenuItem.PtInRect(pt))
			{
				// Darken the pixel
				SetShadowPixel(hDC, pt.x, pt.y, DarkenColor(15 * y, clr), bSaveShadow, bUseSavedShadow, pShadowArray, iArrayIndex);
			}
		}
	} 
	// done with bitmap here
	dcMem.SelectObject(pOldBitmap);

	// Draw the intersection of the item rectangle and the shadow
	{
		CRect rectIntersection;
		rectIntersection.IntersectRect(lpRect, lpItemRect);
		rectIntersection.OffsetRect(-lpRect->left, -lpRect->top);
		FillIntersectionRect(hDC, rectIntersection);
		HPEN hPen = ::CreatePen(PS_SOLID, 1, m_pSkinXP->GetMenuBorderColor());
		HPEN hOldPen = (HPEN) ::SelectObject(hDC, hPen);

		if (rectIntersection.Width() > rectIntersection.Height())
		{
			// We have a bottom menu
			::MoveToEx(hDC, rectIntersection.left, rectIntersection.top, NULL);
			::LineTo(hDC, rectIntersection.left, rectIntersection.bottom);
			::MoveToEx(hDC, rectIntersection.right - 1, rectIntersection.top, NULL);
			::LineTo(hDC, rectIntersection.right - 1, rectIntersection.bottom);
		}
		else
		{
			// We have a right menu
			::MoveToEx(hDC, rectIntersection.left, rectIntersection.top, NULL);
			::LineTo(hDC, rectIntersection.right, rectIntersection.top);
			::MoveToEx(hDC, rectIntersection.left, rectIntersection.bottom - 1, NULL);
			::LineTo(hDC, rectIntersection.right, rectIntersection.bottom - 1);
		}
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hPen);
	}

	::ReleaseDC(0, hDesktopDC);
}

/*
The four different types of menu item shadows are:
		 _______________
1.		|				| 
		|				|\
		|	  File		| |
		|				| |
		|_______________| |
 
		 _______________
2.		|				| 
		|				|
		|	  File		|
		|				|
		|_______________|
		   \_____________

	     _______________
3.		|				| 
		|				|\
		|	  File		| |
		|				| |
		|_______________| |
		 ________________/

		 _______________
4.		|				| | 
		|				| |
		|	  File		| |
		|				| |
		|_______________| |
		   \_____________/
*/
void COXMenuSkinXP::DrawMenuItemShadow(HDC hDC, LPRECT lpRect, UINT nMenuItemShadowType)
{
//	int iLeft = lpRect->left + 4;
//	int iTop = lpRect->top + 4;
//	int iRight = lpRect->right;
//	int iBottom = lpRect->bottom;

	// Draw the shadow - get the pixels from the desktop, darken them
	// and place them on the popup window
	HDC hDesktopDC = ::GetWindowDC(::GetDesktopWindow());
	int x, y;
	CPoint pt;
	COLORREF clr;

	CDC* pDC = CDC::FromHandle(hDesktopDC);

	CRect rectMenu(lpRect);

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rectMenu.Width(), rectMenu.Height());
	CBitmap *pOldBitmap = dcMem.SelectObject(&bitmap);
	
	// working with a mem bitmap at 0,0 for GetPixel
	dcMem.BitBlt(0,0, rectMenu.Width(), rectMenu.Height(), pDC, rectMenu.left, rectMenu.top, SRCCOPY);

	int iLeft = 4;	
	int iTop =  4;
	int iRight = rectMenu.Width();
	int iBottom = rectMenu.Height();

	if (nMenuItemShadowType != 2)
	{
		// Right shadow
		for (x = 1; x <= 4; x++)
		{ 
			if (nMenuItemShadowType != 4)
			{
				// Copy the top right pixels
				for (y = 1; y <= 4; y++)
				{
					pt.x = iRight - x;
					pt.y = iTop + y - 4 - 1;
					clr = GetPixel(dcMem.m_hDC, pt.x, pt.y);
//					pt.Offset(-lpRect->left, -lpRect->top);
					SetPixel(hDC, pt.x, pt.y, clr);
				}

				// Top right corner
				for (y = 4; y >= 1; y--)
				{
					pt.x = iRight - x;
					pt.y = iTop + y - 1;
					clr = GetPixel(dcMem.m_hDC, pt.x, pt.y);
//					pt.Offset(-lpRect->left, -lpRect->top);
					SetPixel(hDC, pt.x, pt.y, DarkenColor(3 * x * y, clr));
				}

				// Vertical line
				for (y = iTop + 4; y <= iBottom - 4 - 1; y++)
				{
					pt.x = iRight - x;
					pt.y = y;
					clr = GetPixel(dcMem.m_hDC, pt.x, pt.y);
//					pt.Offset(-lpRect->left, -lpRect->top);
					SetPixel(hDC, pt.x, pt.y, DarkenColor(15 * x, clr));
				}
			}
			else
			{
				// Just a vertical line from the top
				for (y = 1; y <= iBottom - 4 - 1; y++)
				{
					pt.x = iRight - x;
					pt.y = y;
					clr = GetPixel(dcMem.m_hDC, pt.x, pt.y);
//					pt.Offset(-lpRect->left, -lpRect->top);
					SetPixel(hDC, pt.x, pt.y, DarkenColor(15 * x, clr));
				}
			}

			// Bottom right corner
			if (nMenuItemShadowType != 1)
			{
				for (y = 1; y <= 4; y++)
				{
					pt.x = iRight - x;
					pt.y = iBottom - y;
					clr = GetPixel(dcMem.m_hDC,  pt.x, pt.y);
//					pt.Offset(-lpRect->left, -lpRect->top);
					SetPixel(hDC, pt.x, pt.y, DarkenColor(3 * x * y,  clr));
				}
			}
		} 
	}

	if (nMenuItemShadowType != 1)
	{
		// Bottom shadow
		for (y = 1; y <= 4; y++)
		{
			if (nMenuItemShadowType != 3)
			{
				// Copy the bottom left pixels
				for (x = 1; x <= 4; x++)
				{
					pt.x = iLeft - x;
					pt.y = iBottom - y;
					clr = GetPixel(dcMem.m_hDC, pt.x, pt.y);
//					pt.Offset(-lpRect->left, -lpRect->top);
					SetPixel(hDC, pt.x, pt.y, clr);
				}

				// Bottom left corner
				for (x = 1; x <= 4; x++)
				{
					pt.x = iLeft - x + 4;
					pt.y = iBottom - y;
					clr = GetPixel(dcMem.m_hDC, pt.x, pt.y);
//					pt.Offset(-lpRect->left, -lpRect->top);
					SetPixel(hDC, pt.x, pt.y, DarkenColor(3 * (5 - x) * y, clr));
				}
				
				// Horizontal line
				for (x = iLeft + 4; x <= iRight - 5; x++)
				{
					pt.x = x;
					pt.y = iBottom - y;
					clr = GetPixel(dcMem.m_hDC, pt.x, pt.y);
//					pt.Offset(-lpRect->left, -lpRect->top);
					SetPixel(hDC, pt.x, pt.y, DarkenColor(15 * y, clr));
				}
			}
			else
			{
				// Horizontal line stretching all the way from the left coordinate
				for (x = 1; x <= iRight - 5; x++)
				{
					pt.x = x;
					pt.y = iBottom - y;
					clr = GetPixel(dcMem.m_hDC, pt.x, pt.y);
//					pt.Offset(-lpRect->left, -lpRect->top);
					SetPixel(hDC, pt.x, pt.y, DarkenColor(15 * y, clr));
				}
			}
		}
	}

	dcMem.SelectObject(pOldBitmap);
	::ReleaseDC(0, hDesktopDC);
}


LRESULT COXMenuSkinXP::MenuPopupWndProc(WNDPROC origWndProc, HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	//TRACE(_T("\n%u for %u"), nMsg, hWnd);

	switch (nMsg)
	{
		case WM_CREATE:
		{
			//TRACE(_T("\nWM_DESTROY for %u"), hWnd);
			_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
			COXBitmapMenu* pBitmapMenu = DYNAMIC_DOWNCAST(COXBitmapMenu,
				CMenu::FromHandle(pThreadState->m_hTrackingMenu));
			if (pBitmapMenu != NULL)
			{
				// Add this container window to the stack of COXBitmapMenu
				pBitmapMenu->m_PopupWndStack.Push(hWnd);
			}
		}
		break;

		case WM_DESTROY:
		{
			//TRACE(_T("\nWM_DESTROY for %u"), hWnd);
			_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
			COXBitmapMenu* pBitmapMenu = DYNAMIC_DOWNCAST(COXBitmapMenu,
				CMenu::FromHandle(pThreadState->m_hTrackingMenu));
			if (pBitmapMenu != NULL)
			{
				// Remove this window from the stack of COXBitmapMenu
				pBitmapMenu->m_PopupWndStack.Remove(hWnd);

				if (GetVersion() >= 0x80000000)
				{
					// For Win9x if this is a popup menu we need to refresh the previous menu
					HWND hPreviousWnd = pBitmapMenu->m_PopupWndStack.Pop();
					if (hPreviousWnd != NULL)
						::SendMessage(hPreviousWnd, WM_NCPAINT, 1, 0);
				}
			}
		}
		break;

		case WM_ERASEBKGND:
		{
			//TRACE(_T("\nWM_ERASEBKGND for %u"), hWnd);
			_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
			COXBitmapMenu* pBitmapMenu = DYNAMIC_DOWNCAST(COXBitmapMenu,
				CMenu::FromHandle(pThreadState->m_hTrackingMenu));
			if (pBitmapMenu != NULL)
			{
				return 0;
			}
		}
		break;

		case WM_NCPAINT:
		{
			// For Windows95/98/ME/NT/XP we must perform the drawing here
			//TRACE(_T("\nWM_NCPAINT for %u"), hWnd);
			_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
			COXBitmapMenu* pBitmapMenu = DYNAMIC_DOWNCAST(COXBitmapMenu,
				CMenu::FromHandle(pThreadState->m_hTrackingMenu));
			if (pBitmapMenu != NULL)
			{
				// Get the window DC
				HDC hDC;
				if (wParam != 1)
					hDC = GetDCEx(hWnd, (HRGN)wParam, DCX_WINDOW | DCX_INTERSECTRGN);
				else
					hDC = GetWindowDC(hWnd);
				if (hDC == NULL)
					return 0;
				
				CDC* pDC = CDC::FromHandle(hDC);

				// Get the frame rectangle
				CRect rectFrame;
				::GetWindowRect(hWnd, rectFrame);
				CPoint ptOffset = rectFrame.TopLeft();
				rectFrame.OffsetRect(-ptOffset.x, -ptOffset.y);
				rectFrame.right -= 4;
				rectFrame.bottom -= 4;

				// Exclude the client rectangle
				CRect rectClient(rectFrame);
				rectClient.DeflateRect(3, 3, 3, 3);
				pDC->ExcludeClipRect(rectClient);

				// Draw the shadow
				CRect rectWnd;
				::GetWindowRect(hWnd, rectWnd);
				CPoint ptTopLeft;
				m_mapHWNDtoPos.Lookup(hWnd, ptTopLeft);
				m_mapHWNDtoPos.RemoveKey(hWnd);

				DrawMenuShadow(pDC->m_hDC, rectWnd, pBitmapMenu->m_rectDropDownItem,
					pBitmapMenu->m_PopupWndStack.GetShadowArray(hWnd));
				
				// Draw the menu frame
				pDC->FillSolidRect(rectFrame, m_pSkinXP->GetMenuFaceColor());
				DrawItemStrip(pDC, CRect(rectFrame.left, rectFrame.top + 2,
					rectFrame.left + m_DimensionConstants.m_nGapLeftBitmap + pBitmapMenu->m_nBitmapExtent + 8,
					rectFrame.bottom - 4));
				CBrush brush;
				brush.CreateSolidBrush(m_pSkinXP->GetMenuBorderColor());
				pDC->FrameRect(rectFrame, &brush);

				// Interrupt the menu frame where it meets the menu item

				CRect rectDropDownItem(pBitmapMenu->m_rectDropDownItem);
				rectDropDownItem.OffsetRect(-ptOffset.x, -ptOffset.y);
				if (rectDropDownItem.bottom == rectFrame.top || rectDropDownItem.top == rectFrame.bottom)
				{
					// The drop down item is either on top or at the bottom of the menu
					rectDropDownItem.InflateRect(-1, 1);
					DrawJoiningLine(pDC, rectDropDownItem);
				}
				else if (rectDropDownItem.right == rectFrame.left || rectDropDownItem.left == rectFrame.right)
				{
					// The drop down item is either on the left or on the right of the menu
					rectDropDownItem.InflateRect(1, -1);
					DrawJoiningLine(pDC, rectDropDownItem);
				}

				::ReleaseDC(hWnd, hDC);
				return 0;
			}
		}
		break;

		case WM_PRINT:
		{
			// For Windows 2000 we must perform the drawing here

			//TRACE(_T("\nWM_PRINT"));
			_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
			COXBitmapMenu* pBitmapMenu = DYNAMIC_DOWNCAST(COXBitmapMenu,
				CMenu::FromHandle(pThreadState->m_hTrackingMenu));
			if (pBitmapMenu != NULL)
			{
				MSG msg;
				while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}	

				// Get the window rectangle
				CRect rectWindow;
				::GetWindowRect(hWnd, rectWindow);
				rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);

				// Draw the client to a memory dc
				CDC* pDC = CDC::FromHandle((HDC) wParam);
				CDC dcMem;
				dcMem.CreateCompatibleDC(pDC);
				CBitmap bitmap;
				bitmap.CreateCompatibleBitmap(pDC, rectWindow.Width(), rectWindow.Height());
				CBitmap* pOldBitmap = dcMem.SelectObject(&bitmap);
				dcMem.IntersectClipRect(rectWindow);

				// Get the client to paint
				CPoint ptOldOrigin = dcMem.SetViewportOrg(3, 3);
				SendMessage(hWnd, WM_PRINTCLIENT, (WPARAM) dcMem.m_hDC, lParam);
				dcMem.SetViewportOrg(ptOldOrigin);

				// Draw the shadow if not drawn by XP
				CRect rectWnd;
				::GetWindowRect(hWnd, rectWnd);
				CPoint ptTopLeft;
				m_mapHWNDtoPos.Lookup(hWnd, ptTopLeft);
				m_mapHWNDtoPos.RemoveKey(hWnd);
				int iWidth = rectWnd.Width();
				int iHeight = rectWnd.Height();
				rectWnd.left = ptTopLeft.x;
				rectWnd.top = ptTopLeft.y;
				rectWnd.right = ptTopLeft.x + iWidth;
				rectWnd.bottom = ptTopLeft.y + iHeight;

				DrawMenuShadow(dcMem.m_hDC, rectWnd, pBitmapMenu->m_rectDropDownItem, NULL);

				// Determine the frame rectangle
				CRect rectFrame(rectWindow);
				rectFrame.DeflateRect(0, 0, 4, 4);

				// Exclude the client rectangle
				CRect rectClient(rectFrame);
				rectClient.DeflateRect(3, 3, 3, 3);
				dcMem.ExcludeClipRect(rectClient);

				// Draw the menu frame
				dcMem.FillSolidRect(rectFrame, m_pSkinXP->GetMenuFaceColor());
				DrawItemStrip(&dcMem, CRect(rectFrame.left, rectFrame.top + 2,
					rectFrame.left + m_DimensionConstants.m_nGapLeftBitmap + pBitmapMenu->m_nBitmapExtent + 8,
					rectFrame.bottom - 4));
				CBrush brush;
				brush.CreateSolidBrush(::GetSysColor(COLOR_BTNSHADOW));
				dcMem.FrameRect(rectFrame, &brush);

				// Interrupt the menu frame where it meets the menu item
				CRect rectDropDownItem(pBitmapMenu->m_rectDropDownItem);
				rectDropDownItem.OffsetRect(-rectWnd.left, -rectWnd.top);
				if (rectDropDownItem.bottom == rectFrame.top || rectDropDownItem.top == rectFrame.bottom)
				{
					// The drop down item is either on top or at the bottom of the menu
					rectDropDownItem.InflateRect(-1, 1);
					DrawJoiningLine(&dcMem, rectDropDownItem);
				}
				else if (rectDropDownItem.right == rectFrame.left || rectDropDownItem.left == rectFrame.right)
				{
					// The drop down item is either on the left or on the right of the menu
					rectDropDownItem.InflateRect(1, -1);
					DrawJoiningLine(&dcMem, rectDropDownItem);
				}

				// Copy the memory DC to the window DC
				pDC->BitBlt(0, 0, rectWindow.Width(), rectWindow.Height(), &dcMem, 0, 0, SRCCOPY);
				dcMem.SelectObject(pOldBitmap);

				return 0;
			}
		}
		break;

		case WM_WINDOWPOSCHANGING:
		{
			_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
			COXBitmapMenu* pBitmapMenu = DYNAMIC_DOWNCAST(COXBitmapMenu,
				CMenu::FromHandle(pThreadState->m_hTrackingMenu));
			if (pBitmapMenu != NULL)
			{
				// Make the window larger so it can fit the shadow
				WINDOWPOS* pWP = (WINDOWPOS*) lParam;
				pWP->cx += 4;
				pWP->cy += 4;

				// Save the top left point (only used for Windows 2000/XP)
				CPoint ptTopLeft;
				ptTopLeft.x = pWP->x;
				ptTopLeft.y = pWP->y;
				m_mapHWNDtoPos.SetAt(hWnd, ptTopLeft);
			}
		}
		break;

		default:
			break;
	}

	return ::CallWindowProc(origWndProc, hWnd, nMsg, wParam, lParam);	
}

void COXMenuSkinXP::DrawJoiningLine(CDC* pDC, LPRECT lpRect)
{
	CBrush brushBG;
	brushBG.CreateSolidBrush(m_pSkinXP->GetBackgroundColor());

	pDC->FrameRect(lpRect, &brushBG);
}

void COXMenuSkinXP::FillIntersectionRect(HDC hDC, LPRECT lpRect)
{
	CDC dc;
	dc.Attach(hDC);
	dc.FillSolidRect(lpRect, m_pSkinXP->GetBackgroundColor());
	dc.Detach();
}

void COXMenuSkinXP::DrawItemStrip(CDC* pDC, LPRECT lpStripRect)
{
	pDC->FillSolidRect(lpStripRect->left,
		lpStripRect->top,
		lpStripRect->right,
		lpStripRect->bottom,
		m_pSkinXP->GetBackgroundColor());
}

void COXMenuSkinXP::OnNcPaintCustomizePopupWnd(COXBitmapMenuPopupWnd* pPopupWnd)
{
	CWindowDC dc(pPopupWnd);

	CRect rectWindow;
	pPopupWnd->GetWindowRect(rectWindow);
	rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);

	// Draw the menu frame
	dc.FillSolidRect(rectWindow, m_pSkinXP->GetMenuFaceColor());
	dc.FillSolidRect(rectWindow.left, rectWindow.top + 2,
		rectWindow.left + m_DimensionConstants.m_nGapLeftBitmap + pPopupWnd->m_pBitmapMenu->m_nBitmapExtent + 7,
		rectWindow.bottom - 4, m_pSkinXP->GetBackgroundColor());
	CBrush brush;
	brush.CreateSolidBrush(::GetSysColor(COLOR_BTNSHADOW));
	dc.FrameRect(rectWindow, &brush);
}

void COXMenuSkinXP::OnPaintShadowedItemWnd(COXShadowedItemWnd* pShadowedItemWnd)
{
	CPaintDC dc(pShadowedItemWnd); // device context for painting

	// Get the rectangle of the selected item
	CRect rectItem;
	pShadowedItemWnd->GetClientRect(rectItem);
	rectItem.DeflateRect(0, 0, 4, 4);

	// Draw the frame of the selected item
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));
	CPen* pOldPen = dc.SelectObject(&pen);

	CRect rect;
	pShadowedItemWnd->GetWindowRect(rect);

	if (pShadowedItemWnd->m_nPosFlags & OX_TPM_BOTTOM)
	{
		// The popup menu is below the item
		dc.MoveTo(rectItem.left, rectItem.bottom);
		dc.LineTo(rectItem.left, rectItem.top);
		dc.LineTo(rectItem.right - 1, rectItem.top);
		dc.LineTo(rectItem.right - 1, rectItem.bottom + 1);
		dc.SelectObject(pOldPen);
		
		// Draw the shadow
		DrawMenuItemShadow(dc.m_hDC, rect, 1);
	}
	else if (pShadowedItemWnd->m_nPosFlags & OX_TPM_RIGHT)
	{
		// The popup menu is to the right of the item
		dc.MoveTo(rectItem.right, rectItem.top);
		dc.LineTo(rectItem.left, rectItem.top);
		dc.LineTo(rectItem.left, rectItem.bottom - 1);
		dc.LineTo(rectItem.right, rectItem.bottom - 1);
		dc.SelectObject(pOldPen);
		
		// Draw the shadow
		DrawMenuItemShadow(dc.m_hDC, rect, 2);	
	}
	else if (pShadowedItemWnd->m_nPosFlags & OX_TPM_TOP)
	{
		// The popup menu is above the item
		dc.MoveTo(rectItem.left, rectItem.top);
		dc.LineTo(rectItem.left, rectItem.bottom - 1);
		dc.LineTo(rectItem.right - 1, rectItem.bottom - 1);
		dc.LineTo(rectItem.right - 1, rectItem.top - 1);
		dc.SelectObject(pOldPen);
		
		// Draw the shadow
		DrawMenuItemShadow(dc.m_hDC, rect, 4);
	}
	else if (pShadowedItemWnd->m_nPosFlags & OX_TPM_LEFT)
	{
		// The popup menu is to the left of the item
		dc.MoveTo(rectItem.left, rectItem.top);
		dc.LineTo(rectItem.right, rectItem.top);
		dc.LineTo(rectItem.right, rectItem.bottom - 1);
		dc.LineTo(rectItem.left - 1, rectItem.bottom - 1);
		dc.SelectObject(pOldPen);
		
		// Draw the shadow
		DrawMenuItemShadow(dc.m_hDC, rect, 3);
	}
}

void COXMenuSkinXP::DrawText(CDC* pDC, UINT nState, LPCTSTR lpszText, LPRECT lpText1Rect, LPRECT lpText2Rect, COXBitmapMenu* /*pBitmapMenu*/)
{
	CRect text1Rect(lpText1Rect);
	CRect text2Rect(lpText2Rect);

	BOOL bDisabled=nState & ODS_DISABLED;

	// Split the text (in front of and after the tab)
	CString sText(lpszText);
	int nTabCharIndex=sText.Find(_T('\t'));
	CString sAfterTab;
	CString sBeforeTab;
	if(nTabCharIndex != -1)
	{
		sBeforeTab=sText.Left(nTabCharIndex);
		sAfterTab=sText.Mid(nTabCharIndex + 1);
	}
	else
	{
		sBeforeTab=sText;
		ASSERT(sAfterTab.IsEmpty());
	}

	// Draw the text
	// Sets the foreground color of the text based on its selection state
	COLORREF clrForeground;
	if (bDisabled)
		clrForeground = m_pSkinXP->GetDisabledMenuTextColor();
	else
		clrForeground = ::GetSysColor(COLOR_MENUTEXT);
	
	COLORREF clrOldForeground=pDC->SetTextColor(clrForeground);

	// set background filling mode
	int nOldBkMode=pDC->SetBkMode(TRANSPARENT);

	UINT nFlags=0;
	CRect newText2Rect(0,0,0,0);
	int nHorzAlign=0;
	pDC->DrawState(text1Rect.TopLeft(), text1Rect.Size(), sBeforeTab.GetBuffer(0),
		nFlags, TRUE, sBeforeTab.GetLength(), (CBrush*)NULL);

	// ... First calculate text width so we can right-align it
	pDC->DrawText(sAfterTab, newText2Rect, DT_CALCRECT | DT_SINGLELINE);
	nHorzAlign=text2Rect.Width() - newText2Rect.Width();
	if(!sAfterTab.IsEmpty())
	{
		pDC->DrawState(text2Rect.TopLeft() + CPoint(nHorzAlign, 0), 
			newText2Rect.Size(), sAfterTab.GetBuffer(0),
			nFlags, TRUE, sAfterTab.GetLength(), (CBrush*)NULL);
	}

	pDC->SetTextColor(clrOldForeground); 
	pDC->SetBkMode(nOldBkMode);
}

void COXMenuSkinXP::DrawSeparator(CDC* pDC, LPRECT lpItemRect, COXBitmapMenu* pBitmapMenu)
{
	CRect rectItem(lpItemRect);

	// First draw the background
	CBrush brushFace;
	brushFace.CreateSolidBrush(m_pSkinXP->GetMenuFaceColor());
	pDC->FillRect(lpItemRect, &brushFace);

	CBrush brushIconArea;
	brushIconArea.CreateSolidBrush(m_pSkinXP->GetBackgroundColor());
	CRect rectIconArea(lpItemRect);
	rectIconArea.right =m_DimensionConstants.m_nGapLeftBitmap + pBitmapMenu->m_nBitmapExtent + m_DimensionConstants.m_nGapRightBitmap;
	pDC->FillRect(rectIconArea, &brushIconArea);

	// Draw the separator line
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, m_pSkinXP->GetSeparatorColor());
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(m_DimensionConstants.m_nGapLeftBitmap + pBitmapMenu->m_nBitmapExtent + m_DimensionConstants.m_nGapBitmapText,
		rectItem.CenterPoint().y);
	pDC->LineTo(rectItem.right, rectItem.CenterPoint().y);
	pDC->SelectObject(pOldPen);
}

void COXMenuSkinXP::DrawCustomized(CDC* pDC, LPRECT lpItemRect, COXBitmapMenu* /*pBitmapMenu*/)
{
	// Draw a black rectangle, 2 pixels wide
	CBrush brush;
	brush.CreateSolidBrush(::GetSysColor(COLOR_BTNTEXT));

	CRect rectItem(lpItemRect);
	pDC->FrameRect(rectItem, &brush);
	rectItem.DeflateRect(1, 1);
	pDC->FrameRect(rectItem, &brush);
}

void COXMenuSkinXP::DrawInsertMark(CDC* pDC, LPRECT lpItemRect, BOOL bBefore, COXBitmapMenu* /*pBitmapMenu*/)
{
	CRect itemRect(lpItemRect);
	itemRect.DeflateRect(2,0);

	ASSERT(pDC!=NULL);
	if(bBefore)
	{
		itemRect.top-=2;
		itemRect.bottom=itemRect.top+3;
	}
	else
	{
		itemRect.bottom+=1;
		itemRect.top=itemRect.bottom-3;
	}

	pDC->MoveTo(itemRect.TopLeft());
	pDC->LineTo(itemRect.left+1,itemRect.top+1);
	pDC->LineTo(itemRect.right-1,itemRect.top+1);
	pDC->LineTo(itemRect.right,itemRect.top);
	pDC->LineTo(itemRect.right,itemRect.bottom);
	pDC->LineTo(itemRect.right-1,itemRect.bottom-1);
	pDC->LineTo(itemRect.left+1,itemRect.bottom-1);
	pDC->LineTo(itemRect.left,itemRect.bottom);
	pDC->LineTo(itemRect.TopLeft());
}

void COXMenuSkinXP::DrawSubmenuItem(CDC* pDC, UINT nState, LPCTSTR lpszText, COXImageInfo* pImageInfo, 
									LPRECT lpItemRect, LPRECT lpButtonRect, LPRECT lpText1Rect, LPRECT lpText2Rect, COXBitmapMenu* pBitmapMenu)
{
	ASSERT(pDC!=NULL);
	pBitmapMenu->DrawBackground(pDC,nState,pImageInfo,lpItemRect,lpButtonRect);

	/*
	// draw right aligned arrow
	//
	COLORREF clrShadow=::GetSysColor(COLOR_BTNSHADOW);
	CFont font;
	CFont* pOldFont=NULL;
	if((font.CreateFont(GetSystemMetrics(SM_CYMENUCHECK), 0, 0, 0, 
		FW_NORMAL, 0, 0, 0, SYMBOL_CHARSET, 0,0,0,0, _T("Marlett"))) != NULL)
	{
		pOldFont=(CFont*)pDC->SelectObject(&font);
	}
	int nOldBkMode=pDC->SetBkMode(TRANSPARENT);
	COLORREF oldTextColor=CLR_NONE;
	if((nState&ODS_DISABLED)==ODS_DISABLED)
	{
		oldTextColor=pDC->SetTextColor(clrShadow);
	}

	// draw arrow symbol
	CRect rectText2(lpText2Rect);
	rectText2.OffsetRect(lpItemRect->right - rectText2.right, 0);
	pDC->DrawText(_T("8"),rectText2,DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
	if((nState&ODS_DISABLED)==ODS_DISABLED)
	{
		pDC->SetTextColor(oldTextColor);
	}
	pDC->SetBkMode(nOldBkMode);
	if(pOldFont != NULL)
	{
		pDC->SelectObject(pOldFont);
	}
	//
	//////////////////////////////
	*/

	DrawText(pDC,nState,lpszText,lpText1Rect,lpText2Rect,pBitmapMenu);
}

void COXMenuSkinXP::DrawExpansionItem(CDC* pDC, LPRECT lpItemRect, UINT nState, COXBitmapMenu* /*pBitmapMenu*/)
{
	ASSERT(pDC!=NULL);

	BOOL bDisabled=nState & ODS_DISABLED;

	CFont font;
	VERIFY(font.CreateFont(GetSystemMetrics(SM_CYMENUCHECK),0,0,0,
		FW_NORMAL,0,0,0,SYMBOL_CHARSET,0,0,0,0,_T("Marlett")));
	CFont* pOldFont=pDC->SelectObject(&font);
	int nOldBkMode=pDC->SetBkMode(TRANSPARENT);
	
	COLORREF clrText;
	if (bDisabled)
		clrText = m_pSkinXP->GetSeparatorColor();
	else
		clrText = ::GetSysColor(COLOR_MENUTEXT);

	COLORREF oldTextColor=pDC->SetTextColor(clrText);

	pDC->DrawText(_T("6"),lpItemRect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	pDC->SetTextColor(oldTextColor);
	pDC->SetBkMode(nOldBkMode);
	pDC->SelectObject(pOldFont);
}


// COXMenuSkin2003
//

COXMenuSkin2003::COXMenuSkin2003(COXSkin2003* pSkin2003) :
	COXMenuSkinXP(pSkin2003)
{

}

COXMenuSkin2003::~COXMenuSkin2003()
{

}

void COXMenuSkin2003::DrawBackground(CDC* pDC, UINT nState, COXImageInfo* /*pImageInfo*/, LPRECT lpItemRect, LPRECT /*lpButtonRect*/, COXBitmapMenu* pBitmapMenu)
{
	bool bSelected = (nState & ODS_SELECTED) != 0;
	bool bHidden = (nState & OXODS_HIDDEN) != 0;
	bool bDisabled = (nState & ODS_DISABLED) != 0;

	if(bSelected && !bDisabled)
	{
		// Selected item
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, m_pSkinXP->GetActiveBorderColor());
		CBrush brush;
		brush.CreateSolidBrush(m_pSkinXP->GetHotItemColor());

		CPen* pOldPen = pDC->SelectObject(&pen);
		CBrush* pOldBrush = pDC->SelectObject(&brush);
		pDC->Rectangle(lpItemRect);
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
	else
	{
		// Normal item
		CBrush brushFace;
		brushFace.CreateSolidBrush(m_pSkinXP->GetMenuFaceColor());
		pDC->FillRect(lpItemRect, &brushFace);

		CRect rectIconArea(lpItemRect);
		rectIconArea.left -= 2;
		rectIconArea.right = m_DimensionConstants.m_nGapLeftBitmap + pBitmapMenu->m_nBitmapExtent + m_DimensionConstants.m_nGapRightBitmap;

		COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;
		COLORREF clrMenuStripLeftColor = pSkin2003->GetMenuStripLeftColor();
		COLORREF clrMenuStripRightColor = pSkin2003->GetMenuStripRightColor();
		if (bHidden)
		{
			// We need to darken the colors
			clrMenuStripLeftColor = pSkin2003->Blend3Colors(
				clrMenuStripLeftColor, 0, 0, 0.93f, 0.07f, 0);
			
			clrMenuStripRightColor = pSkin2003->Blend3Colors(
				clrMenuStripRightColor, 0, 0, 0.93f, 0.07f, 0);
		}

		pSkin2003->GradientFill(pDC, rectIconArea,
			clrMenuStripLeftColor,
			clrMenuStripRightColor,
			GRADIENT_FILL_RECT_H);
	}
}

void COXMenuSkin2003::DrawItemStrip(CDC* pDC, LPRECT lpStripRect)
{
	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;
	lpStripRect->bottom += 2;
	pSkin2003->GradientFill(pDC, lpStripRect,
		pSkin2003->GetMenuStripLeftColor(),
		pSkin2003->GetMenuStripRightColor(),
		GRADIENT_FILL_RECT_H);
}

void COXMenuSkin2003::DrawSeparator(CDC* pDC, LPRECT lpItemRect, COXBitmapMenu* pBitmapMenu)
{
	CRect rectItem(lpItemRect);

	// First draw the background
	CBrush brushFace;
	brushFace.CreateSolidBrush(m_pSkinXP->GetMenuFaceColor());
	pDC->FillRect(lpItemRect, &brushFace);

	CRect rectIconArea(lpItemRect);
	rectIconArea.left -= 2;
	rectIconArea.right = m_DimensionConstants.m_nGapLeftBitmap + pBitmapMenu->m_nBitmapExtent + m_DimensionConstants.m_nGapRightBitmap;

	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;
	pSkin2003->GradientFill(pDC, rectIconArea,
		pSkin2003->GetMenuStripLeftColor(),
		pSkin2003->GetMenuStripRightColor(),
		GRADIENT_FILL_RECT_H);

	// Draw the separator line
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, m_pSkinXP->GetSeparatorColor());
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(m_DimensionConstants.m_nGapLeftBitmap + pBitmapMenu->m_nBitmapExtent + m_DimensionConstants.m_nGapBitmapText,
		rectItem.CenterPoint().y);
	pDC->LineTo(rectItem.right, rectItem.CenterPoint().y);
	pDC->SelectObject(pOldPen);
}

void COXMenuSkin2003::DrawImage(CDC* pDC, UINT nState, COXImageInfo* pImageInfo, LPRECT lpImageRect, COXBitmapMenu* /*pBitmapMenu*/)
{
	BOOL bDisabled=nState & ODS_DISABLED;
	BOOL bChecked=nState & ODS_CHECKED;
	BOOL bSelected=nState & ODS_SELECTED;

	if(pImageInfo->IsDefaultImgList() && !bChecked)
	{
		// The default checkmark is not drawn when it isn't checked
		return;
	}

	CRect imageRect(lpImageRect);
	int nIndex=pImageInfo->GetIndex();
	if(bSelected && pImageInfo->GetHotIndex()!=-1)
	{
		nIndex=pImageInfo->GetHotIndex();
	}

	if (bDisabled)
	{
		// Draw a gray image
		CBrush brush;
		brush.CreateSolidBrush(::GetSysColor(COLOR_BTNSHADOW));
		HICON hIcon = pImageInfo->GetImageList()->ExtractIcon(nIndex);
		if (hIcon != NULL)
		{
			pDC->DrawState(imageRect.TopLeft(), imageRect.Size(), hIcon, DST_ICON | DSS_MONO, &brush);
			::DestroyIcon(hIcon);
		}
	}
	else
	{
		// Normal image
		pImageInfo->GetImageList()->Draw(pDC, nIndex, imageRect.TopLeft(), ILD_TRANSPARENT);
	}
}

void COXMenuSkin2003::DrawJoiningLine(CDC* pDC, LPRECT lpRect)
{
	CRect rect(lpRect);

	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;
	pSkin2003->GradientFill(pDC, rect,
		pSkin2003->GetMenubarItemLightColor(),
		pSkin2003->GetMenubarItemDarkColor(),
		GRADIENT_FILL_RECT_V);
}

void COXMenuSkin2003::OnPaintShadowedItemWnd(COXShadowedItemWnd* pShadowedItemWnd)
{
	CPaintDC dc(pShadowedItemWnd); // device context for painting

	TBBUTTON tbb;
	CToolBarCtrl& ctrl = pShadowedItemWnd->m_pCoolToolbar->GetToolBarCtrl();
	ctrl.GetButton(pShadowedItemWnd->m_iMenuItem, &tbb);
	CRect rectMenuItem;
	ctrl.GetItemRect(pShadowedItemWnd->m_iMenuItem, rectMenuItem);
	ctrl.ClientToScreen(rectMenuItem);
	pShadowedItemWnd->ScreenToClient(rectMenuItem);
	COXToolbarSkinXP* pToolbarSkinXP = (COXToolbarSkinXP*) pShadowedItemWnd->m_pCoolToolbar->GetToolbarSkin();
	
	// Draw the item gradient background
	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;
	pSkin2003->GradientFill(&dc, rectMenuItem,
		pSkin2003->GetMenubarItemLightColor(),
		pSkin2003->GetMenubarItemDarkColor(),
		GRADIENT_FILL_RECT_V);

	// Draw the item
	pToolbarSkinXP->DrawItem(&dc, &tbb, rectMenuItem, pShadowedItemWnd->m_pCoolToolbar, FALSE);

	// Get the rectangle of the selected item
	CRect rectItem;
	pShadowedItemWnd->GetClientRect(rectItem);
	rectItem.DeflateRect(0, 0, 4, 4);

	// Draw the frame of the selected item
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, pSkin2003->GetMenuBorderColor());
	CPen* pOldPen = dc.SelectObject(&pen);

	CRect rect;
	pShadowedItemWnd->GetWindowRect(rect);

	if (pShadowedItemWnd->m_nPosFlags & OX_TPM_BOTTOM)
	{
		// The popup menu is below the item
		dc.MoveTo(rectItem.left, rectItem.bottom);
		dc.LineTo(rectItem.left, rectItem.top);
		dc.LineTo(rectItem.right - 1, rectItem.top);
		dc.LineTo(rectItem.right - 1, rectItem.bottom + 1);
		dc.SelectObject(pOldPen);
		
		// Draw the shadow
		DrawMenuItemShadow(dc.m_hDC, rect, 1);
	}
	else if (pShadowedItemWnd->m_nPosFlags & OX_TPM_RIGHT)
	{
		// The popup menu is to the right of the item
		dc.MoveTo(rectItem.right, rectItem.top);
		dc.LineTo(rectItem.left, rectItem.top);
		dc.LineTo(rectItem.left, rectItem.bottom - 1);
		dc.LineTo(rectItem.right, rectItem.bottom - 1);
		dc.SelectObject(pOldPen);
		
		// Draw the shadow
		DrawMenuItemShadow(dc.m_hDC, rect, 2);	
	}
	else if (pShadowedItemWnd->m_nPosFlags & OX_TPM_TOP)
	{
		// The popup menu is above the item
		dc.MoveTo(rectItem.left, rectItem.top);
		dc.LineTo(rectItem.left, rectItem.bottom - 1);
		dc.LineTo(rectItem.right - 1, rectItem.bottom - 1);
		dc.LineTo(rectItem.right - 1, rectItem.top - 1);
		dc.SelectObject(pOldPen);
		
		// Draw the shadow
		DrawMenuItemShadow(dc.m_hDC, rect, 4);
	}
	else if (pShadowedItemWnd->m_nPosFlags & OX_TPM_LEFT)
	{
		// The popup menu is to the left of the item
		dc.MoveTo(rectItem.left, rectItem.top);
		dc.LineTo(rectItem.right, rectItem.top);
		dc.LineTo(rectItem.right, rectItem.bottom - 1);
		dc.LineTo(rectItem.left - 1, rectItem.bottom - 1);
		dc.SelectObject(pOldPen);
		
		// Draw the shadow
		DrawMenuItemShadow(dc.m_hDC, rect, 3);
	}
}

// COXShortcutBarSkin
//

void COXShortcutBarSkin::DrawHeader(LPDRAWITEMSTRUCT lpDrawItemStruct, COXShortcutBar * bar)
{
	HSHBGROUP hGroup=(HSHBGROUP)(INT_PTR)lpDrawItemStruct->itemID;
	ASSERT(hGroup);

	BOOL bFlat=((bar->GetBarStyle() & SHBS_FLATGROUPBUTTON)==SHBS_FLATGROUPBUTTON);

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);	

	// Get the header rect
	CRect rectHeader(lpDrawItemStruct->rcItem);	

	// Save DC
	int nSavedDC=dc.SaveDC();
	ASSERT(nSavedDC);

	// get background colors
	COLORREF clrBackground=bar->GetGroupHeaderBkColor(hGroup);
	VERIFY(clrBackground!=ID_COLOR_NONE);
	// invert colors for dragging group
	if(bar->GetDragGroup()==hGroup)
	{
		clrBackground=InvertColor(clrBackground);
	}

	// Draw FrameRect
	COLORREF clrTopLeft=UpdateColor(clrBackground,64);
	COLORREF clrBottomRight=UpdateColor(clrBackground,-128);
	if(bFlat)
	{
		if(bar->GetDragGroup()==hGroup || bar->m_bCreatingDragImage)
		{
			dc.Draw3dRect(rectHeader,clrTopLeft,clrBottomRight);
			rectHeader.DeflateRect(1,1);
			dc.Draw3dRect(rectHeader,clrBackground,clrBackground);
			rectHeader.DeflateRect(1,1);
		}
		else if(bar->m_hPressedGroup!=NULL)
		{
			if(hGroup==bar->m_hPressedGroup)
			{
				if(hGroup==bar->m_hLastMouseIsOverGroup)
				{
					dc.Draw3dRect(rectHeader,clrBottomRight,clrBackground);
					rectHeader.DeflateRect(1,1);
					dc.Draw3dRect(rectHeader,clrBottomRight,clrTopLeft);
					rectHeader.DeflateRect(1,1);
				}
				else
				{
					dc.Draw3dRect(rectHeader,clrTopLeft,clrBottomRight);
					rectHeader.DeflateRect(1,1);
					dc.Draw3dRect(rectHeader,clrBackground,clrBottomRight);
					rectHeader.DeflateRect(1,1);
				}
			}
			else
			{
				dc.Draw3dRect(rectHeader,clrTopLeft,clrBottomRight);
				rectHeader.DeflateRect(1,1);
				dc.Draw3dRect(rectHeader,clrBackground,clrBackground);
				rectHeader.DeflateRect(1,1);
			}
		}
		else
		{
			if(hGroup==bar->m_hLastMouseIsOverGroup)
			{
				dc.Draw3dRect(rectHeader,clrTopLeft,clrBottomRight);
				rectHeader.DeflateRect(1,1);
				dc.Draw3dRect(rectHeader,clrBackground,clrBottomRight);
				rectHeader.DeflateRect(1,1);
			}
			else
			{
				dc.Draw3dRect(rectHeader,clrTopLeft,clrBottomRight);
				rectHeader.DeflateRect(1,1);
				dc.Draw3dRect(rectHeader,clrBackground,clrBackground);
				rectHeader.DeflateRect(1,1);
			}
		}
	}
	else
	{
		dc.Draw3dRect(rectHeader,clrTopLeft,clrBottomRight);
		rectHeader.DeflateRect(1,1);
		dc.Draw3dRect(rectHeader,clrTopLeft,clrBottomRight);
		rectHeader.DeflateRect(1,1);
	}

	// Fill the background
	CBrush brush(clrBackground);
	dc.FillRect(rectHeader,&brush);	

	// Labels are offset by a certain amount  
	// This offset is related to the width of a space character
	int offset=dc.GetTextExtent(_T(" "),1).cx*2;
	rectHeader.left+=offset;
	rectHeader.right-=offset;	

	rectHeader.left+=DrawHeaderImage(hGroup, bar, &dc, rectHeader.TopLeft());

	DrawHeaderText(hGroup, bar, &dc, rectHeader, bar->GetGroupHeaderTextFormat(hGroup), false);

	DrawDropTarget(hGroup, bar, &dc, lpDrawItemStruct);

	// Restore dc	
	dc.RestoreDC(nSavedDC);
	// Detach the dc before returning	
	dc.Detach();	
}

void COXShortcutBarSkin::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
}

int COXShortcutBarSkin::DrawHeaderImage(HSHBGROUP hGroup, COXShortcutBar * bar, CDC *pDC, POINT topLeft )
{
	int width = 0;

	// Draw header image
	int nImage=(bar->GetExpandedGroup()==hGroup) ? 
		bar->GetGroupImageExpanded(hGroup) : bar->GetGroupImage(hGroup);
	if(nImage>=0)
	{
		ASSERT(bar->m_pImageList);
		if(bar->m_pImageList!=NULL)
		{
			bar->m_pImageList->Draw(pDC,nImage, topLeft,ILD_TRANSPARENT);
			// Adjust the rect further
			IMAGEINFO iInfo;
			VERIFY(bar->m_pImageList->GetImageInfo(nImage,&iInfo));
			width=iInfo.rcImage.right-iInfo.rcImage.left;	
		}
	}

	return width;
}

void COXShortcutBarSkin::DrawHeaderText(HSHBGROUP hGroup, COXShortcutBar * bar, CDC * pDC, RECT rectHeader, UINT nFormat, bool alwaysBold)
{
	// Get the text and format	
	CString sText=bar->GetGroupText(hGroup);
	if(rectHeader.right>rectHeader.left && !sText.IsEmpty())
	{
		// set transparent mode
		pDC->SetBkMode(TRANSPARENT);

		// set text color
		COLORREF clrText=bar->GetGroupHeaderTextColor(hGroup);
		VERIFY(clrText!=ID_COLOR_NONE);
		// invert colors for dragging group
		if(bar->GetDragGroup()==hGroup)
			clrText=InvertColor(clrText);
		pDC->SetTextColor(clrText);	

		// set text font
		CFont* pOldFont=NULL;
		LPLOGFONT pLF=bar->GetGroupHeaderTextFont(hGroup);
		ASSERT(pLF);
		CFont font;
		VERIFY(font.CreateFontIndirect(pLF));
		pOldFont=pDC->SelectObject(&font);

		// update font if specific SHBS_BOLDEXPANDEDGROUP style is set
		if(((bar->GetBarStyle()&SHBS_BOLDEXPANDEDGROUP)!=0 && bar->GetExpandedGroup()==hGroup) || alwaysBold)
		{
			// make it bold
			CFont* pFont=pDC->GetCurrentFont();
			ASSERT(pFont);
			LOGFONT lf;
			VERIFY(pFont->GetLogFont(&lf));
			lf.lfWeight=lf.lfWeight==0 ? FW_NORMAL : lf.lfWeight;
			lf.lfWeight+=FW_BOLD-FW_NORMAL;
			lf.lfWeight=lf.lfWeight>1000 ? 1000 : lf.lfWeight;

			if(pOldFont!=NULL)
				pDC->SelectObject(pOldFont);
			if((HFONT)font!=NULL)
				font.DeleteObject();
			VERIFY(font.CreateFontIndirect(&lf));
			pOldFont=pDC->SelectObject(&font);
		}

		pDC->DrawText(sText,&rectHeader,nFormat);

		if(pOldFont!=NULL)
		{
			pDC->SelectObject(pOldFont);
		}
	}
}

void COXShortcutBarSkin::DrawDropTarget(HSHBGROUP hGroup, COXShortcutBar * bar, CDC *pDC, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// get background colors
	COLORREF clrBackground=bar->GetGroupHeaderBkColor(hGroup);
	VERIFY(clrBackground!=ID_COLOR_NONE);
	// invert colors for dragging group
	if(bar->GetDragGroup()==hGroup)
	{
		clrBackground=InvertColor(clrBackground);
	}

	// drop target effect for drop target group
	HSHBGROUP hDragGroup=bar->GetDragGroup();
	HSHBGROUP hDropHilightGroup=bar->GetDropHilightGroup();
	if(hDropHilightGroup!=NULL && hDropHilightGroup==hGroup && 
		hDropHilightGroup!=hDragGroup)
	{
		ASSERT(hDragGroup);

		// use red color to draw the effect
		COLORREF clrDragDropEffect=RGB(255,0,0);
		// if the header background color is to close to red then use blue one
		if(IsColorCloseTo(clrDragDropEffect,clrBackground,64))
		{
			clrDragDropEffect=RGB(0,0,255);
		}

		// get orders of drop target and drag groups
		int nDropHilightOrder=bar->GetGroupOrder(hDropHilightGroup);
		ASSERT(bar->VerifyOrder(nDropHilightOrder));
		int nDragOrder=bar->GetGroupOrder(hDragGroup);
		ASSERT(bar->VerifyOrder(nDragOrder));

		CRect rect=lpDrawItemStruct->rcItem;	

		CBrush brush(clrDragDropEffect);
		pDC->FrameRect(rect,&brush);

		rect.DeflateRect(4,4);
		CPen pen(PS_SOLID,1,clrDragDropEffect);
		CPen* pOldPen=pDC->SelectObject(&pen);

		// drag group will be placed after drag target group
		if(nDropHilightOrder<nDragOrder)
		{
			// left up arrow
			pDC->MoveTo(rect.left+2,rect.top+9);
			pDC->LineTo(rect.left+2,rect.top+3);

			pDC->MoveTo(rect.left+3,rect.top+9);
			pDC->LineTo(rect.left+3,rect.top+3);

			pDC->MoveTo(rect.left+4,rect.top+9);
			pDC->LineTo(rect.left+4,rect.top+3);

			pDC->MoveTo(rect.left,rect.top+3);
			pDC->LineTo(rect.left+7,rect.top+3);

			pDC->MoveTo(rect.left+1,rect.top+2);
			pDC->LineTo(rect.left+6,rect.top+2);

			pDC->MoveTo(rect.left+2,rect.top+1);
			pDC->LineTo(rect.left+5,rect.top+1);

			pDC->MoveTo(rect.left+3,rect.top);
			pDC->LineTo(rect.left+4,rect.top);

			// right up arrow
			pDC->MoveTo(rect.right-2,rect.top+9);
			pDC->LineTo(rect.right-2,rect.top+3);

			pDC->MoveTo(rect.right-3,rect.top+9);
			pDC->LineTo(rect.right-3,rect.top+3);

			pDC->MoveTo(rect.right-4,rect.top+9);
			pDC->LineTo(rect.right-4,rect.top+3);

			pDC->MoveTo(rect.right,rect.top+3);
			pDC->LineTo(rect.right-7,rect.top+3);

			pDC->MoveTo(rect.right-1,rect.top+2);
			pDC->LineTo(rect.right-6,rect.top+2);

			pDC->MoveTo(rect.right-2,rect.top+1);
			pDC->LineTo(rect.right-5,rect.top+1);

			pDC->MoveTo(rect.right-3,rect.top);
			pDC->LineTo(rect.right-4,rect.top);
		}
		// drag group will be placed before drag target group
		else if(nDropHilightOrder>nDragOrder)
		{
			// left down arrow
			pDC->MoveTo(rect.left+2,rect.bottom-9);
			pDC->LineTo(rect.left+2,rect.bottom-3);

			pDC->MoveTo(rect.left+3,rect.bottom-9);
			pDC->LineTo(rect.left+3,rect.bottom-3);

			pDC->MoveTo(rect.left+4,rect.bottom-9);
			pDC->LineTo(rect.left+4,rect.bottom-3);

			pDC->MoveTo(rect.left,rect.bottom-3);
			pDC->LineTo(rect.left+7,rect.bottom-3);

			pDC->MoveTo(rect.left+1,rect.bottom-2);
			pDC->LineTo(rect.left+6,rect.bottom-2);

			pDC->MoveTo(rect.left+2,rect.bottom-1);
			pDC->LineTo(rect.left+5,rect.bottom-1);

			pDC->MoveTo(rect.left+3,rect.bottom);
			pDC->LineTo(rect.left+4,rect.bottom);

			// right down arrow
			pDC->MoveTo(rect.right-2,rect.bottom-9);
			pDC->LineTo(rect.right-2,rect.bottom-3);

			pDC->MoveTo(rect.right-3,rect.bottom-9);
			pDC->LineTo(rect.right-3,rect.bottom-3);

			pDC->MoveTo(rect.right-4,rect.bottom-9);
			pDC->LineTo(rect.right-4,rect.bottom-3);

			pDC->MoveTo(rect.right,rect.bottom-3);
			pDC->LineTo(rect.right-7,rect.bottom-3);

			pDC->MoveTo(rect.right-1,rect.bottom-2);
			pDC->LineTo(rect.right-6,rect.bottom-2);

			pDC->MoveTo(rect.right-2,rect.bottom-1);
			pDC->LineTo(rect.right-5,rect.bottom-1);

			pDC->MoveTo(rect.right-3,rect.bottom);
			pDC->LineTo(rect.right-4,rect.bottom);
		}
		else
		{
			ASSERT(FALSE);
		}

		if(pOldPen!=NULL)
		{
			pDC->SelectObject(pOldPen);
		}
	}
}


// COXShortcutBarSkinClassic
//
COLORREF COXShortcutBarSkinClassic::GetBackgroundColor(CWnd* /*pWnd*/)
{
	return GetSysColor(COLOR_3DSHADOW);
}

COLORREF COXShortcutBarSkinClassic::GetGroupTextColor(COXShortcutBar* /*pShortcutBar*/)
{
	return ::GetSysColor(COLOR_CAPTIONTEXT);
}

void COXShortcutBarSkinClassic::DrawItemBorder(CDC* pDC, LPCRECT lpRect, BOOL bHot, BOOL bSelected, COXSHBListCtrl* pSHBListCtrl)
{
	COLORREF clrTopLeft = UpdateColor(pSHBListCtrl->GetBkColor(), 64);
	COLORREF clrBottomRight = UpdateColor(pSHBListCtrl->GetBkColor(), -128);

	if (bSelected)
		pDC->Draw3dRect(lpRect, clrBottomRight, clrTopLeft); // sunken
	else if (bHot)
		pDC->Draw3dRect(lpRect, clrTopLeft, clrBottomRight); // raised
}

COLORREF COXShortcutBarSkinClassic::UpdateColor(COLORREF clr, int nOffset)
{
	ASSERT(abs(nOffset)<256);

	int rValue=GetRValue(clr)+nOffset;
	int gValue=GetGValue(clr)+nOffset;
	int bValue=GetRValue(clr)+nOffset;
	if(nOffset>0)
	{
		rValue=rValue>255 ? 255 : rValue;
		gValue=gValue>255 ? 255 : gValue;
		bValue=bValue>255 ? 255 : bValue;
	}
	else
	{
		rValue=rValue<0 ? 0 : rValue;
		gValue=gValue<0 ? 0 : gValue;
		bValue=bValue<0 ? 0 : bValue;
	}

	return RGB(rValue,gValue,bValue);
}

void COXShortcutBarSkinClassic::FillBackground(CDC* pDC, LPCRECT lpRect, CWnd* pWnd, BOOL bHasBackgroundColor, COLORREF clrBackgroundColor)
{
	CRect rect(lpRect);
	if (pWnd->IsKindOf(RUNTIME_CLASS(COXSHBListCtrl)))
			rect.OffsetRect(-rect.left, -rect.top);
	
	// Determine the background color
	if (bHasBackgroundColor)
		pDC->FillSolidRect(rect, clrBackgroundColor);
	else
		pDC->FillSolidRect(rect, GetBackgroundColor(pWnd));
}

// COXShortcutBarSkinXP
//

COXShortcutBarSkinXP::COXShortcutBarSkinXP(COXSkinXP* pSkinXP)
{
	m_pSkinXP = pSkinXP;
	m_bAnimate = true;
}

COLORREF COXShortcutBarSkinXP::GetBackgroundColor(CWnd* /*pWnd*/)
{
	return m_pSkinXP->GetBackgroundColor();
}

COLORREF COXShortcutBarSkinXP::GetGroupTextColor(COXShortcutBar* /*pShortcutBar*/)
{
	return ::GetSysColor(COLOR_WINDOWTEXT);
}

void COXShortcutBarSkinXP::DrawItemBorder(CDC* pDC, LPCRECT lpRect, BOOL bHot, BOOL bSelected, COXSHBListCtrl* /*pSHBListCtrl*/)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, m_pSkinXP->GetActiveBorderColor());
	CPen* pOldPen = pDC->SelectObject(&pen);

	CBrush brush;
	if (bHot && bSelected)
		brush.CreateSolidBrush(m_pSkinXP->GetHotSelectedItemColor());
	else if (bHot)
		brush.CreateSolidBrush(m_pSkinXP->GetHotItemColor());
	else if (bSelected)
		brush.CreateSolidBrush(m_pSkinXP->GetCheckedItemColor());

	CBrush* pOldBrush = pDC->SelectObject(&brush);

	pDC->Rectangle(lpRect);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

void COXShortcutBarSkinXP::FillBackground(CDC* pDC, LPCRECT lpRect, CWnd* pWnd, BOOL bHasBackgroundColor, COLORREF clrBackgroundColor)
{
	CRect rect(lpRect);
	if (pWnd->IsKindOf(RUNTIME_CLASS(COXSHBListCtrl)))
			rect.OffsetRect(-rect.left, -rect.top);
	
	if (bHasBackgroundColor)
		pDC->FillSolidRect(rect, clrBackgroundColor);	
	else
		pDC->FillSolidRect(rect, GetBackgroundColor(pWnd));
}

void COXShortcutBarSkinXP::DrawTopHeader(COXShortcutBar * /*bar*/, CDC * /*pDC*/) 
{
}


// COXShortcutBarSkin2003
//

COXShortcutBarSkin2003::COXShortcutBarSkin2003(COXSkin2003* pSkin2003) :
	COXShortcutBarSkinXP(pSkin2003)
{
	m_bAnimate = false;
}

COXShortcutBarSkin2003::~COXShortcutBarSkin2003()
{
	m_bAnimate = false;
}

void COXShortcutBarSkin2003::DrawHeader(LPDRAWITEMSTRUCT lpDrawItemStruct, COXShortcutBar * bar)
{
	HSHBGROUP hGroup=(HSHBGROUP)(INT_PTR)lpDrawItemStruct->itemID;
	ASSERT(hGroup);

	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);	

	// Get the header rect
	CRect rect(lpDrawItemStruct->rcItem);	

	// Save DC
	int nSavedDC=dc.SaveDC();
	ASSERT(nSavedDC);

//	if(bar->GetDragGroup()==hGroup || bar->m_bCreatingDragImage)

	if(bar->GetExpandedGroup()==hGroup)
	{
		if(hGroup==bar->m_hLastMouseIsOverGroup)
		{
		pSkin2003->FourPointGradientFill(&dc, rect,
			pSkin2003->GetSelShortcutBarDarkColor(), 
			pSkin2003->GetSelShortcutBarDarkColor(), 
			pSkin2003->GetSelShortcutBarLightColor(),
			pSkin2003->GetSelShortcutBarLightColor()); 
		}
		else
		{
			pSkin2003->FourPointGradientFill(&dc, rect,
				pSkin2003->GetSelShortcutBarLightColor(),
				pSkin2003->GetSelShortcutBarLightColor(), 
				pSkin2003->GetSelShortcutBarDarkColor(), 
				pSkin2003->GetSelShortcutBarDarkColor());
		}
	}
	else if(hGroup==bar->m_hLastMouseIsOverGroup)
{
		pSkin2003->FourPointGradientFill(&dc, rect,
			pSkin2003->GetHotShortcutBarLightColor(),
			pSkin2003->GetHotShortcutBarLightColor(), 
			pSkin2003->GetHotShortcutBarDarkColor(), 
			pSkin2003->GetHotShortcutBarDarkColor());
	}
	else
	{
		pSkin2003->FourPointGradientFill(&dc, rect,
			pSkin2003->GetShortcutBarLightColor(),
			pSkin2003->GetShortcutBarLightColor(), 
			pSkin2003->GetShortcutBarDarkColor(), 
			pSkin2003->GetShortcutBarDarkColor());
	}

	dc.FillSolidRect(rect.left, rect.bottom - 1, rect.right - rect.left, 1, RGB(0, 45, 150));

	CRect rectImage(rect);
	rectImage.OffsetRect(6,6);
	
	rectImage.left += DrawHeaderImage(hGroup, bar, &dc, rectImage.TopLeft()) + 6;
	DrawHeaderText(hGroup, bar, &dc, rectImage, DT_VCENTER, true);
	DrawDropTarget(hGroup, bar, &dc, lpDrawItemStruct);


	// Restore dc	
	dc.RestoreDC(nSavedDC);
	// Detach the dc before returning	
	dc.Detach();
}


void COXShortcutBarSkin2003::FillBackground(CDC* pDC, LPCRECT lpRect, CWnd* pWnd, BOOL bHasBackgroundColor, COLORREF clrBackgroundColor)
{
	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;
	CRect rect(lpRect);

	// If this DC belogs to the COXSHBListCtrl class we need to do some
	// coordinate transformations
	COXSHBListCtrl* pSHBListCtrl = DYNAMIC_DOWNCAST(COXSHBListCtrl, pWnd);
	if (pSHBListCtrl != NULL)
	{
		CRect rectClient;
		pSHBListCtrl->m_pShortcutBar->GetClientRect(rectClient);

		CRect rectShortcutBar, rectSHBListCtrl;
		pSHBListCtrl->m_pShortcutBar->GetWindowRect(rectShortcutBar);
		pSHBListCtrl->GetWindowRect(rectSHBListCtrl);

		CPoint ptOffset(rectSHBListCtrl.left - rectShortcutBar.left,
			rectSHBListCtrl.top - rectShortcutBar.top);

		rect.left = -rect.left - ptOffset.x;
		rect.top = -rect.top - ptOffset.y;
		rect.right = rect.left + rectClient.Width();
		rect.bottom = rect.top + rectClient.Height();
	}

	if (bHasBackgroundColor)
		pDC->FillSolidRect(rect, clrBackgroundColor);
	else
		pSkin2003->FourPointGradientFill(pDC, rect,
			pSkin2003->GetShortcutBarLightColor(),
			pSkin2003->GetShortcutBarDarkColor(), 
			pSkin2003->GetShortcutBarDarkColor(), 
			pSkin2003->GetShortcutBarDarkColor());
}

void COXShortcutBarSkin2003::DrawTopHeader(COXShortcutBar * bar, CDC * pDC) 
{	
	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;

	CRect rcHeader;
	bar->GetClientRect(&rcHeader);
	rcHeader.bottom = DFLT_TOPHDRHEIGHT2003;
	pSkin2003->FourPointGradientFill(pDC, &rcHeader,  RGB(85,131,211), RGB(85,131,211), RGB(3,56,147), RGB(3,56,147));

	// Get the text and format	
	HSHBGROUP hGroup=bar->GetExpandedGroup();
	CString sText=bar->GetGroupText(hGroup);
	if(!sText.IsEmpty())
	{
		// set transparent mode
		pDC->SetBkMode(TRANSPARENT);

		// set text color
		pDC->SetTextColor(RGB(255, 255, 255));	

		// set text font
		CFont* pOldFont=NULL;
		LPLOGFONT pLF=bar->GetGroupHeaderTextFont(hGroup);
		ASSERT(pLF);
		pLF->lfHeight = 22;
		pLF->lfWeight= FW_BOLD;
		CFont font;
		VERIFY(font.CreateFontIndirect(pLF));
		pOldFont=pDC->SelectObject(&font);

		CRect rcText(rcHeader);
		rcText.OffsetRect(6, 2);

		pDC->DrawText(sText,&rcText, DT_VCENTER);

		if(pOldFont!=NULL)
		{
			pDC->SelectObject(pOldFont);
		}
	}

	pDC->ExcludeClipRect(rcHeader);
}

// COXSkin
//

COXSkin::COXSkin() :
	m_pToolbarSkin(NULL),
	m_pMenuSkin(NULL),
	m_pMenubarSkin(NULL),
	m_pDockbarSkin(NULL),
	m_pTabSkin(NULL),
	m_pShortcutBarSkin(NULL),
	m_pMiniFrameSkin(NULL),
	m_pStatusbarSkin(NULL),
	m_pFrameSkin(NULL)
{

}

COXSkin::~COXSkin()
{
	if (m_pToolbarSkin != NULL)
		delete m_pToolbarSkin;

	if (m_pMenuSkin != NULL)
		delete m_pMenuSkin;

	if (m_pMenubarSkin != NULL)
		delete m_pMenubarSkin;

	if (m_pDockbarSkin != NULL)
		delete m_pDockbarSkin;

	if (m_pTabSkin != NULL)
		delete m_pTabSkin;

	if (m_pShortcutBarSkin != NULL)
		delete m_pShortcutBarSkin;

	if (m_pMiniFrameSkin != NULL)
		delete m_pMiniFrameSkin;
	
	if (m_pStatusbarSkin != NULL)
		delete m_pStatusbarSkin;

	if (m_pFrameSkin != NULL)
		delete m_pFrameSkin;
}

CString COXSkin::GetName() const
{
	return m_strName;
}

COXToolbarSkin* COXSkin::GetToolbarSkin() const
{
	return m_pToolbarSkin;
}

COXMenuSkin* COXSkin::GetMenuSkin() const
{
	return m_pMenuSkin;
}

COXDockbarSkin* COXSkin::GetDockbarSkin() const
{
	return m_pDockbarSkin;
}

COXTabSkin* COXSkin::GetTabSkin() const
{
	return m_pTabSkin;
}

COXShortcutBarSkin* COXSkin::GetShortcutBarSkin() const
{
	return m_pShortcutBarSkin;
}

COXToolbarSkin* COXSkin::GetMenubarSkin() const
{
	return m_pMenubarSkin;
}

COXMiniFrameSkin* COXSkin::GetMiniFrameSkin() const
{
	return m_pMiniFrameSkin;
}

COXStatusbarSkin* COXSkin::GetStatusbarSkin() const
{
	return m_pStatusbarSkin;
}

COXFrameSkin* COXSkin::GetFrameSkin() const
{
	return m_pFrameSkin;
}

void COXSkin::SetToolbarSkin(COXToolbarSkin* pToolbarSkin)
{
	if (m_pToolbarSkin != NULL)
		delete m_pToolbarSkin;

	m_pToolbarSkin = pToolbarSkin;
}

void COXSkin::SetTabSkin(COXTabSkin* pTabSkin)
{
	if (m_pTabSkin != NULL)
		delete m_pTabSkin;

	m_pTabSkin = pTabSkin;
}

void COXSkin::SetStatusbarSkin(COXStatusbarSkin* pStatusbarSkin)
{
	if (m_pStatusbarSkin != NULL)
		delete m_pStatusbarSkin;

	m_pStatusbarSkin = pStatusbarSkin;
}

void COXSkin::SetShortcutBarSkin(COXShortcutBarSkin* pShortcutBarSkin)
{
	if (m_pShortcutBarSkin != NULL)
		delete m_pShortcutBarSkin;

	m_pShortcutBarSkin = pShortcutBarSkin;
}

void COXSkin::SetMiniFrameSkin(COXMiniFrameSkin* pMiniFrameSkin)
{
	if (m_pMiniFrameSkin != NULL)
		delete m_pMiniFrameSkin;

	m_pMiniFrameSkin = pMiniFrameSkin;
}

void COXSkin::SetMenuSkin(COXMenuSkin* pMenuSkin)
{
	if (m_pMenuSkin != NULL)
		delete m_pMenuSkin;

	m_pMenuSkin = pMenuSkin;
}

void COXSkin::SetMenubarSkin(COXToolbarSkin* pMenubarSkin)
{
	if (m_pMenubarSkin != NULL)
		delete m_pMenubarSkin;

	m_pMenubarSkin = pMenubarSkin;
}

void COXSkin::SetFrameSkin(COXFrameSkin* pFrameSkin)
{
	if (m_pFrameSkin != NULL)
		delete m_pFrameSkin;

	m_pFrameSkin = pFrameSkin;
}

void COXSkin::SetDockbarSkin(COXDockbarSkin* pDockbarSkin)
{
	if (m_pDockbarSkin != NULL)
		delete m_pDockbarSkin;

	m_pDockbarSkin = pDockbarSkin;
}

CString COXSkin::GetProperty(LPCTSTR lpszName) const
{
	CString strValue;
	BOOL bResult = m_mapProperties.Lookup(lpszName, strValue);
	if (bResult)
		return strValue;
	else
		return _T("");
}

void COXSkin::SetProperty(LPCTSTR lpszName, LPCTSTR lpszValue)
{
	m_mapProperties.SetAt(lpszName, lpszValue);
}


// COXSkinClassic
//

COXSkinClassic::COXSkinClassic()
{
	m_strName = _T("Classic");
    m_pToolbarSkin = new COXToolbarSkinClassic();
    m_pMenuSkin = new COXMenuSkinClassic();
    m_pMenubarSkin = new COXToolbarSkinClassic();
    m_pDockbarSkin = new COXDockbarSkinClassic();
    m_pTabSkin = new COXTabSkinClassic();
    m_pShortcutBarSkin = new COXShortcutBarSkinClassic();
	m_pMiniFrameSkin = new COXMiniFrameSkinClassic();
	m_pStatusbarSkin = new COXStatusbarSkinClassic();
	m_pFrameSkin = new COXFrameSkinClassic();
}

COXSkinClassic::~COXSkinClassic()
{

}

/////////////////////////////////////////////////////////////////////////////
// COXSkinnedApp

IMPLEMENT_DYNCREATE(COXSkinnedApp, CWinApp)

COXSkinnedApp::COXSkinnedApp() :
	m_pCurrentSkin(NULL)
{
	// Add the skins
	AddSkin(new COXSkinClassic());
	AddSkin(new COXSkinXP());
	AddSkin(new COXSkin2003());
	SetCurrentSkin(_T("Office XP"));
}

COXSkinnedApp::~COXSkinnedApp()
{
	POSITION pos = m_mapSkins.GetStartPosition();
	while (pos != NULL)
	{
		CString strName;
		COXSkin* pSkin;
		m_mapSkins.GetNextAssoc(pos, strName, (void*&) pSkin);
		delete pSkin;
	}

	m_mapSkins.RemoveAll();
}

COXSkin* COXSkinnedApp::GetCurrentSkin()
{
	return m_pCurrentSkin;
}

void COXSkinnedApp::AddSkin(COXSkin* pSkin)
{

	m_mapSkins.SetAt(pSkin->GetName(), pSkin);
}

BOOL COXSkinnedApp::RemoveSkin(LPCTSTR lpszName)
{
	return m_mapSkins.RemoveKey(lpszName);
}

COXSkin* COXSkinnedApp::GetSkin(LPCTSTR lpszName)
{
	COXSkin* pSkin;
	BOOL bRes = m_mapSkins.Lookup(lpszName, (void*&) pSkin);
	if (bRes == FALSE)
		pSkin = NULL;

	return pSkin;
}

BOOL COXSkinnedApp::SetCurrentSkin(COXSkin* pSkin)
{
	COXSkin* pTempSkin = NULL;
	BOOL bRes = m_mapSkins.Lookup(pSkin->GetName(), (void*&) pTempSkin);
	if (bRes == FALSE)
	{
		m_pCurrentSkin = NULL;
		return FALSE;
	}

	m_currentSkin = pSkin->GetType();

	m_pCurrentSkin = pTempSkin;

	// Redraw the main window and all its descendents
	// Redraw the main window and all its descendents
	CWnd* pWnd = AfxGetMainWnd();
	if (pWnd != NULL && ::IsWindow(pWnd->m_hWnd))
	{
		UpdateFloatingWindows(pWnd);
		UpdateChildrenRec(pWnd);
	}

	return TRUE;
}

BOOL COXSkinnedApp::SetCurrentSkin(OXAppSkins skin)
{
	switch (skin)
	{
	case OXSkinClassic:
		return SetCurrentSkin(_T("Classic"));
		break;
	case OXSkinOfficeXP:
		return SetCurrentSkin(_T("Office XP"));
		break;
	case OXSkinOffice2003:
		return SetCurrentSkin(_T("Office 2003"));
		break;
	default:
		return FALSE;
	}
}


BOOL COXSkinnedApp::SetCurrentSkin(LPCTSTR lpszName)
{
	COXSkin* pTempSkin = NULL;
	BOOL bRes = m_mapSkins.Lookup(lpszName, (void*&) pTempSkin);
	if (bRes == FALSE)
	{
		m_pCurrentSkin = NULL;
		return FALSE;
	}

	m_pCurrentSkin = pTempSkin;
	m_currentSkin = pTempSkin->GetType();

	// Redraw the main window and all its descendents
	CWnd* pWnd = AfxGetMainWnd();
	if (pWnd != NULL && ::IsWindow(pWnd->m_hWnd))
	{
		UpdateFloatingWindows(pWnd);
		UpdateChildrenRec(pWnd);
	}

	return TRUE;
}

// Causes all direct and indirect children to recalculate their layouts and redraw.
// This is a recursive function.
void COXSkinnedApp::UpdateChildrenRec(CWnd* pWnd)
{
	// Update this window first
	CRect rectClient;
	pWnd->GetClientRect(rectClient);

	if ((pWnd->GetStyle() & WS_MAXIMIZE) == 0)
		pWnd->SendMessage(WM_SIZE, 0, MAKELONG(rectClient.right, rectClient.bottom));

	pWnd->SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOMOVE |
		SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
	pWnd->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);

	// Update all children
	CWnd* pChild = pWnd->GetWindow(GW_CHILD);
	while (pChild != NULL)
	{
		UpdateChildrenRec(pChild);
		pChild = pChild->GetWindow(GW_HWNDNEXT);
	}
}

// COXSkin2003
//

COXSkin2003::COXSkin2003() : COXSkinXP(TRUE)
{
	m_strName = _T("Office 2003");
    m_pToolbarSkin = new COXToolbarSkin2003(this);
    m_pMenuSkin = new COXMenuSkin2003(this);
    m_pMenubarSkin = new COXToolbarSkinXP(this);
    m_pDockbarSkin = new COXDockbarSkin2003(this);
    m_pTabSkin = new COXTabSkin2003(this);
    m_pShortcutBarSkin = new COXShortcutBarSkin2003(this);
	m_pMiniFrameSkin = new COXMiniFrameSkinXP(this);
	m_pStatusbarSkin = new COXStatusbarSkin2003(this);
	m_pFrameSkin = new COXFrameSkin2003(this);

	m_hMsimg32DLL = ::LoadLibrary(_T("Msimg32.dll"));
	if (m_hMsimg32DLL != NULL)
		m_fpGradientFill = ::GetProcAddress(m_hMsimg32DLL, "GradientFill");
}

COXSkin2003::~COXSkin2003()
{
	if (m_hMsimg32DLL != NULL)
		::FreeLibrary(m_hMsimg32DLL);
}

// This function wraps Msimg32.dll's ::GradientFill, so that we do not need SDK installed
// in order to use it.
BOOL COXSkin2003::GradientFill(CDC* pDC, LPCRECT lpRect, COLORREF clrLeft, COLORREF clrRight, ULONG dwMode)
{
	if (m_hMsimg32DLL == NULL)
		return GradientFillManual(pDC, lpRect, clrLeft, clrRight, dwMode);

	BOOL (PASCAL* pfnGradientFill)(
		HDC hdc,
		TRIVERTEX* pVertex,
		ULONG dwNumVertex, 
		PVOID pMesh,
		ULONG dwNumMesh,
		ULONG dwMode) = NULL;

	TRIVERTEX vert[2];
	GRADIENT_RECT grect;

	grect.UpperLeft = 0;
	vert[0].x = lpRect->left;
	vert[0].y = lpRect->top;
	vert[0].Alpha = 0;
	vert[0].Red = (COLOR16) (GetRValue(clrLeft) * 255);
	vert[0].Green = (COLOR16) (GetGValue(clrLeft) * 255);
	vert[0].Blue = (COLOR16) (GetBValue(clrLeft) * 255);

	grect.LowerRight = 1;
	vert[1].x = lpRect->right;
	vert[1].y = lpRect->bottom;
	vert[1].Alpha = 0;
	vert[1].Red = (COLOR16) (GetRValue(clrRight) * 255);
	vert[1].Green = (COLOR16) (GetGValue(clrRight) * 255);
	vert[1].Blue = (COLOR16) (GetBValue(clrRight) * 255);

	(FARPROC&) pfnGradientFill = m_fpGradientFill;
	return (*pfnGradientFill)(pDC->m_hDC, vert, 2, &grect, 1, dwMode);
}

// This function is just to accomodate platforms where the ::GradientFill() API is not present.
BOOL COXSkin2003::GradientFillManual(CDC* pDC, LPCRECT lpRect, COLORREF clrLeft, COLORREF clrRight, ULONG dwMode)
{
	CRect rect(lpRect);

	// First break down the colors
	int iLeftRed = GetRValue(clrLeft);
	int iLeftGreen = GetGValue(clrLeft);
	int iLeftBlue = GetBValue(clrLeft);

	int iRightRed = GetRValue(clrRight);
	int iRightGreen = GetGValue(clrRight);
	int iRightBlue = GetBValue(clrRight);

	if (dwMode == GRADIENT_FILL_RECT_H)
	{

		for (int i = 0; i < rect.Width(); i++)
		{
			// Determine the current color
			int iRed = iLeftRed + (i * (iRightRed - iLeftRed) / rect.Width());
			int iGreen = iLeftGreen + (i * (iRightGreen - iLeftGreen) / rect.Width());
			int iBlue = iLeftBlue + (i * (iRightBlue - iLeftBlue) / rect.Width());

			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB(iRed, iGreen, iBlue));
			CPen* pOldPen = pDC->SelectObject(&pen);

			pDC->MoveTo(rect.left + i, rect.top);
			pDC->LineTo(rect.left + i, rect.bottom);

			pDC->SelectObject(pOldPen);
		}
	}
	else // vertical
	{
		for (int i = 0; i < rect.Height(); i++)
		{
			// Determine the current color
			int iRed = iLeftRed + (i * (iRightRed - iLeftRed) / rect.Height());
			int iGreen = iLeftGreen + (i * (iRightGreen - iLeftGreen) / rect.Height());
			int iBlue = iLeftBlue + (i * (iRightBlue - iLeftBlue) / rect.Height());

			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB(iRed, iGreen, iBlue));
			CPen* pOldPen = pDC->SelectObject(&pen);

			pDC->MoveTo(rect.left, rect.top + i);
			pDC->LineTo(rect.right, rect.top + i);

			pDC->SelectObject(pOldPen);		
		}
	}
	
	return TRUE;
}

BOOL COXSkin2003::FourPointGradientFill(CDC* pDC, LPCRECT lpRect,
									  COLORREF clrTopLeft, COLORREF clrTopRight,
									  COLORREF clrBottomLeft, COLORREF clrBottomRight)
{
	if (m_hMsimg32DLL == NULL)
		return FourPointGradientFillManual(pDC, lpRect, clrTopLeft, clrTopRight,
			clrBottomLeft, clrBottomRight);

	BOOL (PASCAL* pfnGradientFill)(
		HDC hdc,
		TRIVERTEX* pVertex,
		ULONG dwNumVertex, 
		PVOID pMesh,
		ULONG dwNumMesh,
		ULONG dwMode) = NULL;

	TRIVERTEX vert[6];
	GRADIENT_TRIANGLE gtriangle[2];

	// First triangle	 _______
	//					|      /
	//					|    /
	//					|  /
	//					|/

	gtriangle[0].Vertex1 = 0;
	vert[0].x = lpRect->left;
	vert[0].y = lpRect->top;
	vert[0].Alpha = 0;
	vert[0].Red = (COLOR16) (GetRValue(clrTopLeft) * 255);
	vert[0].Green = (COLOR16) (GetGValue(clrTopLeft) * 255);
	vert[0].Blue = (COLOR16) (GetBValue(clrTopLeft) * 255);

	gtriangle[0].Vertex2 = 1;
	vert[1].x = lpRect->right;
	vert[1].y = lpRect->top;
	vert[1].Alpha = 0;
	vert[1].Red = (COLOR16) (GetRValue(clrTopRight) * 255);
	vert[1].Green = (COLOR16) (GetGValue(clrTopRight) * 255);
	vert[1].Blue = (COLOR16) (GetBValue(clrTopRight) * 255);

	gtriangle[0].Vertex3 = 2;
	vert[2].x = lpRect->left;
	vert[2].y = lpRect->bottom;
	vert[2].Alpha = 0;
	vert[2].Red = (COLOR16) (GetRValue(clrBottomLeft) * 255);
	vert[2].Green = (COLOR16) (GetGValue(clrBottomLeft) * 255);
	vert[2].Blue = (COLOR16) (GetBValue(clrBottomLeft) * 255);


	// Second triangle		    /|
	//						  /	 |
	//						/	 |
	//					  /______|
	//				

	gtriangle[1].Vertex1 = 3;
	vert[3].x = lpRect->right;
	vert[3].y = lpRect->top;
	vert[3].Alpha = 0;
	vert[3].Red = (COLOR16) (GetRValue(clrTopRight) * 255);
	vert[3].Green = (COLOR16) (GetGValue(clrTopRight) * 255);
	vert[3].Blue = (COLOR16) (GetBValue(clrTopRight) * 255);

	gtriangle[1].Vertex2 = 4;
	vert[4].x = lpRect->left;
	vert[4].y = lpRect->bottom;
	vert[4].Alpha = 0;
	vert[4].Red = (COLOR16) (GetRValue(clrBottomLeft) * 255);
	vert[4].Green = (COLOR16) (GetGValue(clrBottomLeft) * 255);
	vert[4].Blue = (COLOR16) (GetBValue(clrBottomLeft) * 255);

	gtriangle[1].Vertex3 = 5;
	vert[5].x = lpRect->right;
	vert[5].y = lpRect->bottom;
	vert[5].Alpha = 0;
	vert[5].Red = (COLOR16) (GetRValue(clrBottomRight) * 255);
	vert[5].Green = (COLOR16) (GetGValue(clrBottomRight) * 255);
	vert[5].Blue = (COLOR16) (GetBValue(clrBottomRight) * 255);


	(FARPROC&) pfnGradientFill = m_fpGradientFill;
	return (*pfnGradientFill)(pDC->m_hDC, vert, 6, gtriangle, 2, GRADIENT_FILL_TRIANGLE);
}

// This function is just to accomodate platforms where the ::GradientFill() API is not present.
BOOL COXSkin2003::FourPointGradientFillManual(CDC* pDC, LPCRECT lpRect,
									  COLORREF clrTopLeft, COLORREF clrTopRight,
									  COLORREF clrBottomLeft, COLORREF /*clrBottomRight*/)
{
	CRect rect(lpRect);

	if (rect.Width() < rect.Height())
		return GradientFillManual(pDC, lpRect, clrTopLeft, clrTopRight, GRADIENT_FILL_RECT_H);
	else
		return GradientFillManual(pDC, lpRect, clrTopLeft, clrBottomLeft, GRADIENT_FILL_RECT_V);
}

void COXSkin2003::DrawGripperStripes(CDC* pDC, CRect &rect, bool bHorizontal)
{
	CPen pen, penShadow;
	pen.CreatePen(PS_SOLID, 1, GetGripperColor());
	penShadow.CreatePen(PS_SOLID, 1, GetGripperStripesShadowColor());
	CPen* pOldPen = pDC->SelectObject(&pen);

	if (bHorizontal)
	{
		int iYPos = rect.top;
		while (iYPos <= rect.bottom)
		{
			pDC->MoveTo(rect.left, iYPos);
			pDC->LineTo(rect.right, iYPos);
			pDC->MoveTo(rect.left, iYPos + 1);
			pDC->LineTo(rect.right, iYPos + 1);

			CPen* pPrevPen = pDC->SelectObject(&penShadow);
			pDC->MoveTo(rect.left + 1, iYPos + 2);
			pDC->LineTo(rect.right + 1, iYPos + 1);
			pDC->MoveTo(rect.left + 1, iYPos + 2);
			pDC->LineTo(rect.right + 1, iYPos + 2);
			pDC->SelectObject(pPrevPen);

			iYPos += 4;
		}
	}
	else // vertical
	{
		int iXPos = rect.left;
		while (iXPos <= rect.right)
		{
			pDC->MoveTo(iXPos, rect.top);
			pDC->LineTo(iXPos, rect.bottom);
			pDC->MoveTo(iXPos + 1, rect.top);
			pDC->LineTo(iXPos + 1, rect.bottom);			
		
			CPen* pPrevPen = pDC->SelectObject(&penShadow);
			pDC->MoveTo(iXPos + 2, rect.top + 1);
			pDC->LineTo(iXPos + 1, rect.bottom + 1);
			pDC->MoveTo(iXPos + 2, rect.top + 1);
			pDC->LineTo(iXPos + 2, rect.bottom + 1);
			pDC->SelectObject(pPrevPen);			
			
			iXPos += 4;
		}
	}

	pDC->SelectObject(pOldPen);
}

// Draws an Office 2003 style frame button with a gradient background
void COXSkin2003::DrawFrameButton(CDC* pDC, CRect& rcButton, UINT uType, BOOL bDisabled, BOOL bInvertFace, COLORREF clrBackground, CWnd* pParentWnd)
{
	// First determine some colors
	COLORREF clrDisabled = 0;
	if (bDisabled)
	{
		if (bInvertFace)
		{
			bInvertFace = FALSE;
			clrDisabled = ModifyColor(clrBackground, 64, 64, 64);
		}
		else
			clrDisabled = ModifyColor(clrBackground, -96, -96, -96);
	}

	// Get the cursor position in window coordinates
	CPoint ptCursor;
	::GetCursorPos(&ptCursor);
	CRect rectWindow;
	pParentWnd->GetWindowRect(rectWindow);
	ptCursor.Offset(-rectWindow.left, -rectWindow.top);

	bool bLeftButtonDown = (pParentWnd->GetCapture() != NULL);

	// Determine the colors
	CBrush brush;
	brush.m_hObject = NULL;
	if (bLeftButtonDown && rcButton.PtInRect(ptCursor))
	{
		// Pressed
		brush.CreateSolidBrush(GetHotSelectedItemColor());
	}
	else if (rcButton.PtInRect(ptCursor))
	{
		// Hot
		brush.CreateSolidBrush(GetHotItemColor());
	}

	///////////////////////// Perform the drawing
	// Draw to a memory dc to avoid flashing
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap bmpMem;
	CRect rectMem(rcButton);
	rectMem.OffsetRect(-rectMem.left, -rectMem.top);
	bmpMem.CreateCompatibleBitmap(pDC, rectMem.Width(), rectMem.Height());
	CBitmap* pOldBmp = dc.SelectObject(&bmpMem);

	if (brush.m_hObject != NULL && !bDisabled)
	{
		// Draw a frame
		CBrush* pOldBrush = dc.SelectObject(&brush);
		CPen penBorder;
		penBorder.CreatePen(PS_SOLID, 1, GetActiveBorderColor());
		CPen* pOldPen = dc.SelectObject(&penBorder);
		dc.Rectangle(rectMem);
		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);
	}
	else
	{
		if (pParentWnd->IsKindOf(RUNTIME_CLASS(COXMenuBar)))
		{
			// Draw a gradient background
			CRect rectMain;
			pParentWnd->GetParentFrame()->GetWindowRect(rectMain);

			// Get the botton screen rectangle
			CRect rectButton(rcButton);
			pParentWnd->GetParentFrame()->ClientToScreen(rectButton);

			int iWidth = ::GetSystemMetrics(SM_CXSCREEN);
			if (iWidth < rectMain.Width())
				iWidth = rectMain.Width();

			CRect rectGradient;
			rectGradient.left = rectMain.left - rectButton.left;
			rectGradient.right = rectGradient.left + iWidth;
			rectGradient.top = 0;
			rectGradient.bottom = rectButton.Height();

			CRect rectSolid(rectGradient);
			rectGradient.right = rectGradient.CenterPoint().x;
			rectSolid.left = rectGradient.right;

			GradientFill(&dc, rectGradient,
				GetDockbarLeftColor(),
				GetDockbarRightColor(),
				GRADIENT_FILL_RECT_H);
			dc.FillSolidRect(rectSolid, GetDockbarRightColor());
		}
		else if (pParentWnd->IsKindOf(RUNTIME_CLASS(COXSizeControlBar)))
		{
			// Draw a gradient background
			
			COXSizeControlBar* pControlBar = (COXSizeControlBar*) pParentWnd;
			
			CRect rectMain = pControlBar->m_rectGripper;
			pControlBar->ClientToScreen(rectMain);

			// Get the botton screen rectangle
			CRect rectButton(rcButton);
			pParentWnd->ClientToScreen(rectButton);

			CRect rectGradient;
			if (pControlBar->m_dwStyle & CBRS_ORIENT_HORZ)
			{
				rectGradient.left = -rcButton.left;
				rectGradient.right = rectGradient.left + rectMain.Width();
				rectGradient.top = 0;
				rectGradient.bottom = rcButton.Height();

				GradientFill(&dc, rectGradient,
					GetControlBarLightColor(),
					GetControlBarDarkColor(),
					GRADIENT_FILL_RECT_H);
			}
			else
			{
				rectGradient.left = 0;
				rectGradient.right = rcButton.Width();
				rectGradient.top = -rcButton.top;
				rectGradient.bottom = rectGradient.top + rectMain.Height();

				GradientFill(&dc, rectGradient,
					GetControlBarLightColor(),
					GetControlBarDarkColor(),
					GRADIENT_FILL_RECT_V);
			}
		}
	}

	// Draw the symbol transparently
	CDC dcSymbol;
	dcSymbol.CreateCompatibleDC(pDC);
	CBitmap bmpSymbol;
	CRect rcSymbol(rectMem);
	rcSymbol.DeflateRect(2, 2);
	bmpSymbol.CreateCompatibleBitmap(pDC, rcSymbol.Width(), rcSymbol.Height());
	CBitmap* pOldBitmap = dcSymbol.SelectObject(&bmpSymbol);
	dcSymbol.DrawFrameControl(CRect(-2, -2, rcSymbol.Width() + 2, rcSymbol.Height() + 2),
		DFC_CAPTION, DFCS_FLAT | uType);

	COLORREF clrTransparent = ::GetSysColor(COLOR_BTNFACE);

	if (bLeftButtonDown && rcButton.PtInRect(ptCursor))
	{
		// Pressed, so invert the face color of the symbol
		dcSymbol.BitBlt(0, 0, rcSymbol.Width(), rcSymbol.Height(), &dcSymbol, 0, 0, PATINVERT);
		clrTransparent = InvertColor(clrTransparent);
	}
	else if (bInvertFace && !(rcButton.PtInRect(ptCursor)))
	{
		// Invert the face color
		dcSymbol.BitBlt(0, 0, rcSymbol.Width(), rcSymbol.Height(), &dcSymbol, 0, 0, PATINVERT);
		clrTransparent = InvertColor(clrTransparent);
	}

	dcSymbol.SelectObject(pOldBitmap);
	DrawTransparentBitmap(&bmpSymbol, &dc, 2, 2, rcSymbol.Width(), rcSymbol.Height(), clrTransparent);

	if (bDisabled)
	{
		// Draw a disabled button
		dc.SelectObject(pOldBmp);

		CBitmap bmpDisabled;
		bmpDisabled.CreateCompatibleBitmap(pDC, rectMem.Width(), rectMem.Height());
		pOldBmp = dc.SelectObject(&bmpDisabled);

		dc.FillSolidRect(CRect(0, 0, rcButton.Width(), rcButton.Height()), clrDisabled);
		DrawTransparentBitmap(&bmpMem, &dc, 0, 0, rcButton.Width(), rcButton.Height(), 0);

		// Copy the memory dc to the real dc
		pDC->BitBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), &dc, 0, 0, SRCCOPY);
		dc.SelectObject(pOldBmp);	
	}
	else
	{
		// Copy the memory dc to the real dc
		pDC->BitBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), &dc, 0, 0, SRCCOPY);
		dc.SelectObject(pOldBmp);
	}
}


COLORREF COXSkin2003::GetToolbarLightColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.95f, 0.05f, 0);
	}
	else
	{
		// 78% window background color, 22% 3DFace color
		return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DFACE), 0,
			0.78f, 0.22f, 0);
	}
}

COLORREF COXSkin2003::GetToolbarDarkColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.2f, 0.8f, 0);
	}
	else
	{
		// 4% window background color, 96% 3DFace color
		return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DFACE), 0,
			0.04f, 0.96f, 0);
	}
}

COLORREF COXSkin2003::GetMenuStripLeftColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.95f, 0.05f, 0);
	}
	else
	{
		// 86% window background color, 14% 3DFace color
		return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DFACE), 0,
			0.86f, 0.14f, 0);
	}
}

COLORREF COXSkin2003::GetMenuStripRightColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.2f, 0.8f, 0);
	}
	else
	{
		// 8% window background color, 92% 3DFace color
		return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DFACE), 0,
			0.08f, 0.92f, 0);
	}
}

// Returns the color of the gripper
COLORREF COXSkin2003::GetGripperColor()
{

	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT),0 , 0,
			0.7f, 0.3f, 0);
	}
	else
	{
		return COXSkinXP::GetGripperColor();
	}
}

COLORREF COXSkin2003::GetGripperStripesShadowColor()
{
	return ::GetSysColor(COLOR_WINDOW);
}

COLORREF COXSkin2003::GetControlBarLightColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.95f, 0.05f, 0);
	}
	else
	{
		// 73% window background color, 27% 3DFace color
		return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DFACE), 0,
			0.73f, 0.27f, 0);
	}
}

COLORREF COXSkin2003::GetControlBarDarkColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.2f, 0.8f, 0);
	}
	else
	{
		return ::GetSysColor(COLOR_3DFACE);
	}
}

COLORREF COXSkin2003::GetDockbarLeftColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.5f, 0.5f, 0);
	}
	else
	{
		return ::GetSysColor(COLOR_3DFACE);
	}
}

COLORREF COXSkin2003::GetDockbarRightColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.7f, 0.3f, 0);
	}
	else
	{
		// 80% window background color, 20% 3DFace color
		return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DFACE), 0,
			0.8f, 0.2f, 0);
	}
}

COLORREF COXSkin2003::GetTabBackgroundLightColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.95f, 0.05f, 0);
	}
	else
	{
		// 78% window background color, 22% 3DFace color
		return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DFACE), 0,
			0.78f, 0.22f, 0);
	}
}

COLORREF COXSkin2003::GetTabBackgroundDarkColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.2f, 0.8f, 0);
	}
	else
	{
		// 4% window background color, 96% 3DFace color
		return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DFACE), 0,
			0.04f, 0.96f, 0);
	}
}

COLORREF COXSkin2003::GetTabItemLightColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.95f, 0.05f, 0);
	}
	else
	{
		// 73% window background color, 27% 3DFace color
		return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DFACE), 0,
			0.73f, 0.27f, 0);
	}
}

COLORREF COXSkin2003::GetTabItemDarkColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return GetDockbarRightColor();
	}
	else
	{
		return ::GetSysColor(COLOR_3DFACE);
	}
}

COLORREF COXSkin2003::GetMenubarItemLightColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.95f, 0.05f, 0);
	}
	else
	{
		// 86% window background color, 14% 3DFace color
		return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DFACE), 0,
			0.86f, 0.14f, 0);
	}
}

COLORREF COXSkin2003::GetMenubarItemDarkColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.2f, 0.8f, 0);
	}
	else
	{
		// 58% window background color, 42% 3DFace color
		return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DFACE), 0,
			0.58f, 0.42f, 0);
	}
}

COLORREF COXSkin2003::GetShortcutBarLightColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.95f, 0.05f, 0);
	}
	else
	{
		// 86% window background color, 14% 3DFace color
		return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DFACE), 0,
			0.86f, 0.14f, 0);
	}
}

COLORREF COXSkin2003::GetShortcutBarDarkColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.5f, 0.5f, 0);
	}
	else
	{
		return GetBackgroundColor();
	}
}

COLORREF COXSkin2003::GetHotShortcutBarLightColor()
{
	return GetShortcutBarLightColor();
}

COLORREF COXSkin2003::GetHotShortcutBarDarkColor()
{
	return RGB(247,192,91);
}

COLORREF COXSkin2003::GetSelShortcutBarLightColor()
{
	return GetShortcutBarLightColor();
}

COLORREF COXSkin2003::GetSelShortcutBarDarkColor()
{
	return RGB(238,149,21);
}


COLORREF COXSkin2003::GetStatusbarTopColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.3f, 0.3f, 0.4f);
	}
	else
	{
		return ::GetSysColor(COLOR_3DSHADOW);
	}
}

COLORREF COXSkin2003::GetStatusbarMiddleColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.6f, 0.4f, 0);
	}
	else
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DFACE), 0,
			0.5f, 0.5f, 0);
	}
}

COLORREF COXSkin2003::GetStatusbarBottomColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.3f, 0.7f, 0);
	}
	else
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DFACE), 0,
			0.2f, 0.8f, 0);
	}	
}

COLORREF COXSkin2003::GetHotItemColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
		return RGB(255, 238, 194);
	else
		return COXSkinXP::GetHotItemColor();
}

COLORREF COXSkin2003::GetCheckedItemColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
		return RGB(255, 192, 111);
	else
		return COXSkinXP::GetCheckedItemColor();
}

COLORREF COXSkin2003::GetHotSelectedItemColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
		return RGB(254, 128, 62);
	else
		return COXSkinXP::GetHotSelectedItemColor();
}

COLORREF COXSkin2003::GetHotToolbarItemLightColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
		return RGB(255, 244, 204);
	else
		return COXSkinXP::GetHotItemColor();
}

COLORREF COXSkin2003::GetHotToolbarItemDarkColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
		return RGB(255, 208, 145);
	else
		return COXSkinXP::GetHotItemColor();
}

COLORREF COXSkin2003::GetCheckedToolbarItemLightColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
		return RGB(255, 213, 140);
	else
		return COXSkinXP::GetCheckedItemColor();
}

COLORREF COXSkin2003::GetCheckedToolbarItemDarkColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
		return RGB(255, 173, 85);
	else
		return COXSkinXP::GetCheckedItemColor();
}

COLORREF COXSkin2003::GetHotSelectedToolbarItemLightColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
		return RGB(254, 145, 78);
	else
		return COXSkinXP::GetHotSelectedItemColor();
}

COLORREF COXSkin2003::GetHotSelectedToolbarItemDarkColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
		return RGB(255, 211, 142);
	else
		return COXSkinXP::GetHotSelectedItemColor();
}

// Returns the color of the border of a hot item
COLORREF COXSkin2003::GetActiveBorderColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
		return ModifyColor(m_UxTheme.GetThemeSysColor(hTheme, COLOR_ACTIVECAPTION),
			0, -84, -99);
	else
		return ::GetSysColor(COLOR_HIGHLIGHT);
}

// Returns the face color of the menu
COLORREF COXSkin2003::GetMenuFaceColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return RGB(246, 246, 246);
	}
	else
	{
		// 86% window background color, 14% 3DFace color
		return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DFACE), 0,
			0.86f, 0.14f, 0);
	}
}

// Returns the border color of the menu
COLORREF COXSkin2003::GetMenuBorderColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(m_UxTheme.GetThemeSysColor(hTheme, COLOR_ACTIVECAPTION),
			0, 0,
			0.55f, 0.45f, 0);
	}
	else
	{
		return ::GetSysColor(COLOR_BTNSHADOW);
	}
}

// Returns the background color of the toolbar
COLORREF COXSkin2003::GetBackgroundColor()
{
	return COXSkinXP::GetBackgroundColor();
}

// Returns the background color of the dockbar
COLORREF COXSkin2003::GetDockbarBackgroundColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return GetDockbarRightColor();
	}
	else
	{
		return ::GetSysColor(COLOR_3DFACE);
	}
}

COLORREF COXSkin2003::GetInactiveTabTextColor()
{
	return ::GetSysColor(COLOR_BTNTEXT);
}

// Returns the color of the separators
COLORREF COXSkin2003::GetSeparatorColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(::GetSysColor(COLOR_WINDOW),
			m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0,
			0.2f, 0.8f, 0);
	}
	else
	{
		return COXSkinXP::GetSeparatorColor();
	}
}

// Returns the color of the splitter line
COLORREF COXSkin2003::GetSplitterLineColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return GetShortcutBarDarkColor();
	}
	else
	{
		return COXSkinXP::GetSeparatorColor();
	}
}

COLORREF COXSkin2003::GetBorderColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return Blend3Colors(m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT), 0, 0,
			0.5f, 0.5f, 0);
	}
	else
	{
		return COXSkinXP::GetSeparatorColor();
	}
}

COLORREF COXSkin2003::GetMiniFrameCaptionColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT);
	}
	else
	{
		return COXSkinXP::GetMiniFrameCaptionColor();
	}
}

COLORREF COXSkin2003::GetMiniFrameCaptionTextColor()
{
	return COXSkinXP::GetMiniFrameCaptionTextColor();
}

COLORREF COXSkin2003::GetMiniFrameBorderColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return m_UxTheme.GetThemeSysColor(hTheme, COLOR_HIGHLIGHT);
	}
	else
	{
		return COXSkinXP::GetMiniFrameBorderColor();
	}
}

COLORREF COXSkin2003::GetMiniFrameLightBorderColor()
{
	return GetDockbarBackgroundColor();
}

COLORREF COXSkin2003::GetDisabledTextColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return RGB(141, 141, 141);
	}
	else
	{
		return COXSkinXP::GetDisabledTextColor();
	}
}

COLORREF COXSkin2003::GetDisabledMenuTextColor()
{
	HTHEME hTheme = m_UxTheme.OpenThemeData(NULL, _T("button"));
	if (hTheme != NULL)
	{
		return RGB(141, 141, 141);
	}
	else
	{
		return COXSkinXP::GetDisabledMenuTextColor();
	}
}

// COXSkinXP
//

COXSkinXP::COXSkinXP()
{
	m_strName = _T("Office XP");
    m_pToolbarSkin = new COXToolbarSkinXP(this);
    m_pMenuSkin = new COXMenuSkinXP(this);
    m_pMenubarSkin = new COXToolbarSkinXP(this);
    m_pDockbarSkin = new COXDockbarSkinXP(this);
    m_pTabSkin = new COXTabSkinXP(this);
    m_pShortcutBarSkin = new COXShortcutBarSkinXP(this);
	m_pMiniFrameSkin = new COXMiniFrameSkinXP(this);
	m_pStatusbarSkin = new COXStatusbarSkinXP(this);
	m_pFrameSkin = new COXFrameSkinXP(this);
}

COXSkinXP::~COXSkinXP()
{

}

// This function modifies the given color by adding the values passed for red gree and blue.
COLORREF COXSkinXP::ModifyColor(COLORREF crBase, int iRed, int iGreen, int iBlue)
{
	int iBaseRed = GetRValue(crBase);
	int iBaseGreen = GetGValue(crBase);
	int iBaseBlue = GetBValue(crBase);

	iBaseRed += iRed;
	if (iBaseRed > 255)
		iBaseRed = 255;

	iBaseGreen += iGreen;
	if (iBaseGreen > 255)
		iBaseGreen = 255;

	iBaseBlue += iBlue;
	if (iBaseBlue > 255)
		iBaseBlue = 255;
	
	return RGB(iBaseRed, iBaseGreen, iBaseBlue);
}

COLORREF COXSkinXP::ModifyColor(COLORREF crBase, float fRed, float fGreen, float fBlue)
{
	int iBaseRed = GetRValue(crBase);
	int iBaseGreen = GetGValue(crBase);
	int iBaseBlue = GetBValue(crBase);

	iBaseRed = (int) (iBaseRed * fRed);
	if (iBaseRed > 255)
		iBaseRed = 255;

	iBaseGreen = (int) (iBaseGreen * fGreen);
	if (iBaseGreen > 255)
		iBaseGreen = 255;

	iBaseBlue = (int) (iBaseBlue * fBlue);
	if (iBaseBlue > 255)
		iBaseBlue = 255;
	
	return RGB(iBaseRed, iBaseGreen, iBaseBlue);
}

COLORREF COXSkinXP::Blend3Colors(COLORREF clrFirst, COLORREF clrSecond, COLORREF clrThird, float fFirts, float fSecond, float fThird)
{
	int iFirstRed = GetRValue(clrFirst);
	int iFirstGreen = GetGValue(clrFirst);
	int iFirstBlue = GetBValue(clrFirst);

	int iSecondRed = GetRValue(clrSecond);
	int iSecondGreen = GetGValue(clrSecond);
	int iSecondBlue = GetBValue(clrSecond);

	int iThirdRed = GetRValue(clrThird);
	int iThirdGreen = GetGValue(clrThird);
	int iThirdBlue = GetBValue(clrThird);

	int iRed = (int) (iFirstRed * fFirts + iSecondRed * fSecond + iThirdRed * fThird);
	if (iRed > 255)
		iRed = 255;

	int iGreen = (int) (iFirstGreen * fFirts + iSecondGreen * fSecond + iThirdGreen * fThird);
	if (iGreen > 255)
		iGreen = 255;

	int iBlue = (int) (iFirstBlue * fFirts + iSecondBlue * fSecond + iThirdBlue * fThird);
	if (iBlue > 255)
		iBlue = 255;
	
	return RGB(iRed, iGreen, iBlue);
}

// Returns the color of the gripper
COLORREF COXSkinXP::GetGripperColor()
{
	// The stripe color if the button shadow color incremented by 32
	return ModifyColor(::GetSysColor(COLOR_BTNSHADOW), 32, 32, 32);
}

// Returns the color of the separators
COLORREF COXSkinXP::GetSeparatorColor()
{
	// The separator color if the button shadow color incremented by 38
	return ModifyColor(::GetSysColor(COLOR_BTNSHADOW), 38, 38, 38);
}

COLORREF COXSkinXP::GetBorderColor()
{
	return ::GetSysColor(COLOR_3DSHADOW);
}

// Returns the color of the background color of the toolbar
COLORREF COXSkinXP::GetBackgroundColor()
{
	// 16% window background color, 84% 3DFace color
	return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DFACE), 0,
		0.16f, 0.84f, 0);
}

// Returns the background color of the dockbar
COLORREF COXSkinXP::GetDockbarBackgroundColor()
{
	return ::GetSysColor(COLOR_BTNFACE);
}

// Returns the background color of the icon of a hidden item
COLORREF COXSkinXP::GetHiddenItemBackgroundColor()
{
	// The button face color decremented by 8
	return ModifyColor(::GetSysColor(COLOR_BTNFACE), -8, -8, -8);
}

// Returns the color of a hot item
COLORREF COXSkinXP::GetHotItemColor()
{
	// 70% window background color, 30% selected item color
	return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_HIGHLIGHT), 0,
		0.7f, 0.3f, 0);
}

// Returns the color of a hot selected item
COLORREF COXSkinXP::GetHotSelectedItemColor()
{
	// 50% window background color, 50% selected item color
	return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_HIGHLIGHT), 0, 
		0.5f, 0.5f, 0);
}

// Returns the color of a checked item
COLORREF COXSkinXP::GetCheckedItemColor()
{
	// 42% 3DFace color, 48% window background color, 10% highlight color
	return Blend3Colors(::GetSysColor(COLOR_3DFACE), ::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_HIGHLIGHT), 
		0.42f, 0.48f, 0.10f);
}

// Returns the color of the border of a hot item
COLORREF COXSkinXP::GetActiveBorderColor()
{
	return ::GetSysColor(COLOR_HIGHLIGHT);
}

// Returns the color of the text of a normal item
COLORREF COXSkinXP::GetTextColor()
{
	return ::GetSysColor(COLOR_BTNTEXT);
}

// Returns the color of the text of a selected item
COLORREF COXSkinXP::GetSelectedTextColor()
{
	return ::GetSysColor(COLOR_HIGHLIGHTTEXT);
}

// Returns the color of the border of the mini frame window
COLORREF COXSkinXP::GetMiniFrameBorderColor()
{
	// The color is the button shadow color decremented by 9
	return ModifyColor(::GetSysColor(COLOR_BTNSHADOW), -9, -9, -9);
}

COLORREF COXSkinXP::GetMiniFrameLightBorderColor()
{
	return GetBackgroundColor();
}

// Returns the face color of the menu
COLORREF COXSkinXP::GetMenuFaceColor()
{
	// 86% window background color, 14% 3DFace color
	return Blend3Colors(::GetSysColor(COLOR_WINDOW), ::GetSysColor(COLOR_3DFACE), 0,
		0.86f, 0.14f, 0);
}

// Returns the face color of the menu
COLORREF COXSkinXP::GetMenuBorderColor()
{
	return ::GetSysColor(COLOR_BTNSHADOW);
}

// Returns the background color of the tab buttons is a tab control
COLORREF COXSkinXP::GetTabButtonsBackgroundColor()
{
	return ModifyColor(::GetSysColor(COLOR_3DFACE), 1.2f, 1.2f, 1.2f);
}

// Returns the text color of an inselected tab button
COLORREF COXSkinXP::GetInactiveTabTextColor()
{
	return ModifyColor(::GetSysColor(COLOR_GRAYTEXT), -8, -12, -22);
}

// Returns the color of border of a status bar pan
COLORREF COXSkinXP::GetStatusPaneColor()
{
	return ModifyColor(::GetSysColor(COLOR_BTNFACE), 17, 12, 16);
}

COLORREF COXSkinXP::InvertColor(COLORREF clr)
{
	int iBaseRed = GetRValue(clr);
	int iBaseGreen = GetGValue(clr);
	int iBaseBlue = GetBValue(clr);

	return RGB(255 - iBaseRed, 255 - iBaseGreen, 255 - iBaseBlue);
}

void COXSkinXP::DrawTransparentBitmap(CBitmap* pBitmap, CDC* pDestDC, int x, int y, int nWidth, int nHeight, COLORREF clrTransColor)
{
	COLORREF crOldBack = pDestDC->SetBkColor(RGB(255, 255, 255));
	COLORREF crOldText = pDestDC->SetTextColor(RGB(0, 0, 0));
	CDC dcImage, dcTrans;

	// Create two memory dcs for the image and the mask
	dcImage.CreateCompatibleDC(pDestDC);
	dcTrans.CreateCompatibleDC(pDestDC);

	// Select the image into the appropriate dc
	CBitmap* pOldBitmapImage = dcImage.SelectObject(pBitmap);

	// Create the mask bitmap
	CBitmap bitmapTrans;
	bitmapTrans.CreateBitmap(nWidth, nHeight, 1, 1, NULL);

	// Select the mask bitmap into the appropriate dc
	CBitmap* pOldBitmapTrans = dcTrans.SelectObject(&bitmapTrans);

	// Build mask based on transparent colour
	dcImage.SetBkColor(clrTransColor);
	dcTrans.BitBlt(0, 0, nWidth, nHeight, &dcImage, 0, 0, SRCCOPY);

	// Do the work - True Mask method - cool if not actual display
	pDestDC->BitBlt(x, y, nWidth, nHeight, &dcImage, 0, 0, SRCINVERT);
	pDestDC->BitBlt(x, y, nWidth, nHeight, &dcTrans, 0, 0, SRCAND);
	pDestDC->BitBlt(x, y, nWidth, nHeight, &dcImage, 0, 0, SRCINVERT);

	// Restore settings
	dcImage.SelectObject(pOldBitmapImage);
	dcTrans.SelectObject(pOldBitmapTrans);
	pDestDC->SetBkColor(crOldBack);
	pDestDC->SetTextColor(crOldText);
}

// Draws an Office XP style frame button
void COXSkinXP::DrawFrameButton(CDC* pDC, CRect& rcButton, UINT uType, BOOL bDisabled, BOOL bInvertFace, COLORREF clrBackground, CWnd* pParentWnd)
{
	// First determine some colors
	COLORREF clrDisabled = 0;
	if (bDisabled)
	{
		if (bInvertFace)
		{
			bInvertFace = FALSE;
			clrDisabled = ModifyColor(clrBackground, 64, 64, 64);
		}
		else
			clrDisabled = ModifyColor(clrBackground, -96, -96, -96);
	}

	// Get the cursor position in window coordinates
	CPoint ptCursor;
	::GetCursorPos(&ptCursor);
	CRect rectWindow;
	pParentWnd->GetWindowRect(rectWindow);
	ptCursor.Offset(-rectWindow.left, -rectWindow.top);

	bool bLeftButtonDown = (pParentWnd->GetCapture() != NULL);

	// Determine the colors
	CBrush brush;
	brush.m_hObject = NULL;
	if (bLeftButtonDown && rcButton.PtInRect(ptCursor))
	{
		// Pressed
		brush.CreateSolidBrush(GetHotSelectedItemColor());
	}
	else if (rcButton.PtInRect(ptCursor))
	{
		// Hot
		brush.CreateSolidBrush(GetHotItemColor());
	}

	///////////////////////// Perform the drawing
	// Draw to a memory dc to avoid flashing
	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap bmpMem;
	CRect rectMem(rcButton);
	rectMem.OffsetRect(-rectMem.left, -rectMem.top);
	bmpMem.CreateCompatibleBitmap(pDC, rectMem.Width(), rectMem.Height());
	CBitmap* pOldBmp = dc.SelectObject(&bmpMem);

	if (brush.m_hObject != NULL && !bDisabled)
	{
		// Draw a frame
		CBrush* pOldBrush = dc.SelectObject(&brush);
		CPen penBorder;
		penBorder.CreatePen(PS_SOLID, 1, GetActiveBorderColor());
		CPen* pOldPen = dc.SelectObject(&penBorder);
		dc.Rectangle(rectMem);
		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);
	}
	else
		dc.FillSolidRect(rectMem, clrBackground);

	// Draw the symbol transparently
	CDC dcSymbol;
	dcSymbol.CreateCompatibleDC(pDC);
	CBitmap bmpSymbol;
	CRect rcSymbol(rectMem);
	rcSymbol.DeflateRect(2, 2);
	bmpSymbol.CreateCompatibleBitmap(pDC, rcSymbol.Width(), rcSymbol.Height());
	CBitmap* pOldBitmap = dcSymbol.SelectObject(&bmpSymbol);
	dcSymbol.DrawFrameControl(CRect(-2, -2, rcSymbol.Width() + 2, rcSymbol.Height() + 2),
		DFC_CAPTION, DFCS_FLAT | uType);

	COLORREF clrTransparent = ::GetSysColor(COLOR_BTNFACE);

	if (bLeftButtonDown && rcButton.PtInRect(ptCursor))
	{
		// Pressed, so invert the face color of the symbol
		dcSymbol.BitBlt(0, 0, rcSymbol.Width(), rcSymbol.Height(), &dcSymbol, 0, 0, PATINVERT);
		clrTransparent = InvertColor(clrTransparent);
	}
	else if (bInvertFace && !(rcButton.PtInRect(ptCursor)))
	{
		// Invert the face color
		dcSymbol.BitBlt(0, 0, rcSymbol.Width(), rcSymbol.Height(), &dcSymbol, 0, 0, PATINVERT);
		clrTransparent = InvertColor(clrTransparent);
	}

	dcSymbol.SelectObject(pOldBitmap);
	DrawTransparentBitmap(&bmpSymbol, &dc, 2, 2, rcSymbol.Width(), rcSymbol.Height(), clrTransparent);

	if (bDisabled)
	{
		// Draw a disabled button
		dc.SelectObject(pOldBmp);

		CBitmap bmpDisabled;
		bmpDisabled.CreateCompatibleBitmap(pDC, rectMem.Width(), rectMem.Height());
		pOldBmp = dc.SelectObject(&bmpDisabled);

		dc.FillSolidRect(CRect(0, 0, rcButton.Width(), rcButton.Height()), clrDisabled);
		DrawTransparentBitmap(&bmpMem, &dc, 0, 0, rcButton.Width(), rcButton.Height(), 0);

		// Copy the memory dc to the real dc
		pDC->BitBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), &dc, 0, 0, SRCCOPY);
		dc.SelectObject(pOldBmp);	
	}
	else
	{
		// Copy the memory dc to the real dc
		pDC->BitBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), &dc, 0, 0, SRCCOPY);
		dc.SelectObject(pOldBmp);
	}
}

COLORREF COXSkinXP::GetMiniFrameCaptionColor()
{
	return ::GetSysColor(COLOR_BTNSHADOW);
}

COLORREF COXSkinXP::GetMiniFrameCaptionTextColor()
{
	return ::GetSysColor(COLOR_CAPTIONTEXT);
}

COLORREF COXSkinXP::GetDisabledTextColor()
{
	return ::GetSysColor(COLOR_GRAYTEXT);
}

COLORREF COXSkinXP::GetDisabledMenuTextColor()
{
	return GetSeparatorColor();
}

// COXToolbarSkin
//




// COXToolbarSkinClassic.cpp
//

void COXToolbarSkinClassic::DrawGripper(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar)
{
    if (pCoolToolbar->m_dwStyle & CBRS_FLOATING)
		return;		// no gripper if floating

	COLORREF clrHilight=::GetSysColor(COLOR_BTNHILIGHT);
	COLORREF clrShadow=::GetSysColor(COLOR_BTNSHADOW);
	
	CRect rect(lpRect);
	CRect gripperRect(lpRect);

	if (pCoolToolbar->m_dwStyle & CBRS_ORIENT_HORZ) 
	{
		gripperRect.DeflateRect(1, 1);
		gripperRect.OffsetRect(0, 1);
		// gripper at left
		if (pCoolToolbar->m_pDockContext != NULL)
		{
			// EnableDocking was called
			gripperRect.left += 2;
			gripperRect.bottom -= 2;
		}
		else
		{
			if (pCoolToolbar->m_dwStyle & CBRS_BORDER_BOTTOM)
			{
				gripperRect.top -= 2;
				gripperRect.bottom -= 2;
			}
		}
		gripperRect.right = gripperRect.left + 3;
        pDC->Draw3dRect(gripperRect, clrHilight, clrShadow);
		gripperRect.OffsetRect(3, 0);
        pDC->Draw3dRect(gripperRect, clrHilight, clrShadow);

		rect.left = gripperRect.right + 1;
	} 
	else 
	{
		gripperRect.DeflateRect(0, 1, 2, 1);
		gripperRect.OffsetRect(0, 1);
		// gripper at top
		if (pCoolToolbar->m_pDockContext == NULL)
		{
			// EnableDocking was not called
			gripperRect.right += 2;
		}
		else
		{
			gripperRect.left += 2;
		}
		gripperRect.bottom = gripperRect.top + 3;
		pDC->Draw3dRect(gripperRect, clrHilight, clrShadow);
		gripperRect.OffsetRect(0, 3);
        pDC->Draw3dRect(gripperRect, clrHilight, clrShadow);

		rect.top = gripperRect.bottom + 1;
	}
}

void COXToolbarSkinClassic::DrawNonClientArea(CDC* pDC, LPCRECT /*lpRect*/, COXCoolToolBar* pCoolToolbar)
{
	// get window DC that is clipped to the non-client area
	CRect rectClient;
	pCoolToolbar->GetClientRect(rectClient);
	CRect rectWindow;
	pCoolToolbar->GetWindowRect(rectWindow);
	pCoolToolbar->ScreenToClient(rectWindow);
	rectClient.OffsetRect(-rectWindow.left, -rectWindow.top);
	rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);

	// draw borders in non-client area
	if(!pCoolToolbar->IsCool())
	{
		// only if CoolToolBar is not used in CoolBar
		pCoolToolbar->DrawBorders(pDC, rectWindow);

		// fixing for spurious edges
		if((pCoolToolbar->m_dwStyle&CBRS_BORDER_3D)==0)
		{
			rectWindow.InflateRect(1,1);
		}
	}

	// erase parts not drawn
	pDC->IntersectClipRect(rectWindow);
	pDC->ExcludeClipRect(rectClient);
	pDC->FillSolidRect(rectWindow, ::GetSysColor(COLOR_3DFACE));
}

void COXToolbarSkinClassic::DrawSeparator(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar)
{
	CRect rc(lpRect);
	COLORREF clrHilight = ::GetSysColor(COLOR_BTNHILIGHT);
	COLORREF clrShadow = ::GetSysColor(COLOR_BTNSHADOW);
	
    BOOL bHorz= ((pCoolToolbar->m_dwStyle & CBRS_ORIENT_HORZ) !=0 ) ? TRUE : FALSE;
	// make sure, this separator is not a placeholder for
	// another control.
	if (rc.Width() <= 8) 
	{
		if (bHorz) 
		{
			// draw the separator bar in the middle
			int x = (rc.left + rc.right) / 2;
			rc.left = x - 1;
			rc.right = x + 1;
			pDC->Draw3dRect(rc, clrShadow, clrHilight);
		} 
		else 
		{
			// draw the separator bar in the middle
			UINT nButtonWidth = pCoolToolbar->GetButtonsSize().cx;
			rc.left = rc.left - nButtonWidth;
			rc.right = rc.left + nButtonWidth;
			rc.top=rc.bottom + 1;
			rc.bottom = rc.top + 3;
			int y = (rc.top+rc.bottom) / 2;
			rc.top = y - 1;
			rc.bottom = y + 1;
			pDC->Draw3dRect(rc, clrShadow, clrHilight);
		}
	}
}

void COXToolbarSkinClassic::DrawCustomizedButton(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* /*pCoolToolbar*/)
{
	pDC->DrawDragRect(lpRect, CSize(2,2), CRect(0,0,0,0), CSize(0,0), NULL, NULL);
}

void COXToolbarSkinClassic::OnCustomDraw(NMHDR*& pNotify, LRESULT*& pResult, COXCoolToolBar* pCoolToolbar)
{
	// in new version of comctl32.dll(4.71)
	// we use toolbar specific custom draw structure 
	LPNMTBCUSTOMDRAW pTBCDRW=NULL;
	LPNMCUSTOMDRAW pCDRW=NULL;
	if(pCoolToolbar->m_dwComCtlVersion>=_IE40_COMCTL_VERSION)
	{
		pTBCDRW=(LPNMTBCUSTOMDRAW)pNotify;
		pCDRW=&pTBCDRW->nmcd;
	}
	else
	{
		pCDRW=(LPNMCUSTOMDRAW)pNotify;
	}

	OXCTB_CUSTOMDRAW cd;
	if(pCoolToolbar->m_dwComCtlVersion>=_IE40_COMCTL_VERSION)
	{
		ASSERT(pTBCDRW!=NULL);
		cd.pCustomDraw=(LPNMCUSTOMDRAW)pTBCDRW;
	}
	else
	{
		cd.pCustomDraw=pCDRW;
	}
	cd.hdr.hwndFrom=pCDRW->hdr.hwndFrom;
	cd.hdr.idFrom=pCDRW->hdr.idFrom;
	cd.hdr.code=OXCTBN_CUSTOMDRAW;
	cd.result=CDRF_DODEFAULT;
	cd.bDoDefault=TRUE;

	CWnd* pParentWnd=NULL;
	if(pCoolToolbar->IsFloating())
	{
		pParentWnd=pCoolToolbar->GetTopLevelParent();
	}
	else
	{
		pParentWnd=pCoolToolbar->GetParentFrame();
	}

	ASSERT(pParentWnd != NULL);

	pParentWnd->SendMessage(WM_NOTIFY,cd.hdr.idFrom,(LPARAM)&cd);

	if(pCDRW->dwDrawStage==CDDS_PREPAINT && 
		pCoolToolbar->GetToolBarCtrl().GetButtonCount()>0)
	{
		CRect rect;
		pCoolToolbar->GetItemRect(0,&rect);
	}

	if(!cd.bDoDefault)
	{
		*pResult=cd.result;
	}
	else
	{
		CDC dc;
		dc.Attach(pTBCDRW->nmcd.hdc);

		*pResult = CDRF_DODEFAULT;
		switch( pCDRW->dwDrawStage )
		{
		case CDDS_PREPAINT:
			{
				// Draw the background
				DrawBackground(&dc, pCoolToolbar);

				/////////
				// draw separators
				/////////
				// in new version of comctl32.dll(4.71)
				// separators are drawn automatically
				// well, you have a chance to draw your own separator even
				// if new version of comctl32.dll installed. But you shouldn't 
				// forget that native separators will be drawn anyway
				if(pCoolToolbar->m_dwComCtlVersion<_IE40_COMCTL_VERSION)
				{
					if(pCoolToolbar->IsFlat() && pCoolToolbar->m_bSeparator)
					{
						int nBitmap; 
						UINT uID, uStyleState;
						CRect rc;
						for(int nIndex=0; 
							nIndex<pCoolToolbar->GetToolBarCtrl().GetButtonCount(); nIndex++)
						{
							pCoolToolbar->GetButtonInfo(nIndex, uID, uStyleState, nBitmap);
							WORD wStyle=LOWORD(uStyleState);
							WORD wState=HIWORD(uStyleState);
							if(wStyle==TBSTYLE_SEP && wState!=TBSTATE_HIDDEN) 
							{
								if(!(wState & TBSTATE_WRAP))
								{
									pCoolToolbar->GetItemRect(nIndex,&rc);
									rc.top-=1;
									pCoolToolbar->DrawSeparator(dc,rc);
								}
								else
								{
									CBrush brush(::GetSysColor(COLOR_BTNFACE));
									dc.FillRect(&rc,&brush);
								}
							}
						}
					}
				}

				// draw custom button in customized state if set
				int nCustomizedButton=pCoolToolbar->GetCustomizedButton();
				if(nCustomizedButton!=-1 && pCoolToolbar->IsCustomButton(nCustomizedButton))
				{
					CRect rect;
					pCoolToolbar->GetItemRect(nCustomizedButton,rect);
					pCoolToolbar->DrawCustomizedButton(dc,rect);
				}


				*pResult = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT | CDRF_NOTIFYITEMDRAW;
				break;
			}

		case CDDS_ITEMPREPAINT:
			{
				*pResult = CDRF_NOTIFYPOSTPAINT;	// request for ITEMPOSTPAINT
 
				CRect rectButton(pCDRW->rc);
				CRect rectBackground(pCDRW->rc);
				if(!pCoolToolbar->GetToolBarCtrl().IsButtonHidden(PtrToInt(pCDRW->dwItemSpec))) 
				{
					if(pCDRW->uItemState & CDIS_CHECKED)
					{
						if(pCDRW->uItemState & CDIS_HOT)
						{
							// set text color
							if(pCoolToolbar->GetSelectedTextColor() != CLR_DEFAULT)
							{
								if(pCoolToolbar->m_dwComCtlVersion >= _IE40_COMCTL_VERSION)
								{
									pTBCDRW->clrText = pCoolToolbar->GetSelectedTextColor();
								}
								else
								{
									::SetTextColor(pCDRW->hdc, pCoolToolbar->GetSelectedTextColor());
								}
							}

							// draw border
							if(pCoolToolbar->GetSelectedBorderColor() != CLR_DEFAULT)
							{
								CBrush brush(pCoolToolbar->GetSelectedBorderColor());
								dc.FrameRect(rectButton,&brush);
								rectBackground.DeflateRect(1,1,1,1);
								if(pCoolToolbar->m_dwComCtlVersion >= _IE40_COMCTL_VERSION)
								{
									*pResult |= TBCDRF_NOEDGES;
								}
							}

							// draw background
							if(pCoolToolbar->GetSelectedBkColor() != CLR_DEFAULT)
							{
								dc.FillSolidRect(rectBackground,pCoolToolbar->GetSelectedBkColor());
								if(pCoolToolbar->m_dwComCtlVersion >= _IE40_COMCTL_VERSION)
								{
									*pResult |= TBCDRF_NOETCHEDEFFECT;
								}
							}
						}
						else
						{
							// set text color
							if(pCoolToolbar->GetCheckedTextColor() != CLR_DEFAULT)
							{
								if(pCoolToolbar->m_dwComCtlVersion >= _IE40_COMCTL_VERSION)
								{
									pTBCDRW->clrText = pCoolToolbar->GetCheckedTextColor();
								}
								else
								{
									::SetTextColor(pCDRW->hdc, pCoolToolbar->GetCheckedTextColor());
								}
							}

							// draw border
							if(pCoolToolbar->GetCheckedBorderColor() != CLR_DEFAULT)
							{
								CBrush brush(pCoolToolbar->GetCheckedBorderColor());
								dc.FrameRect(rectButton,&brush);
								rectBackground.DeflateRect(1,1,1,1);
								if(pCoolToolbar->m_dwComCtlVersion >= _IE40_COMCTL_VERSION)
								{
									*pResult |= TBCDRF_NOEDGES;
								}
							}

							// draw background
							if(pCoolToolbar->GetCheckedBkColor() != CLR_DEFAULT)
							{
								dc.FillSolidRect(rectBackground,pCoolToolbar->GetCheckedBkColor());
								if(pCoolToolbar->m_dwComCtlVersion >= _IE40_COMCTL_VERSION)
								{
									*pResult |= TBCDRF_NOETCHEDEFFECT;
								}
							}
						}
					}
					else if(pCDRW->uItemState & CDIS_HOT)
					{
						if(pCDRW->uItemState & CDIS_SELECTED)
						{
							// set text color
							if(pCoolToolbar->GetSelectedTextColor() != CLR_DEFAULT)
							{
								if(pCoolToolbar->m_dwComCtlVersion >= _IE40_COMCTL_VERSION)
								{
									pTBCDRW->clrText = pCoolToolbar->GetSelectedTextColor();
								}
								else
								{
									::SetTextColor(pCDRW->hdc, pCoolToolbar->GetSelectedTextColor());
								}
							}

							// draw border
							if(pCoolToolbar->GetSelectedBorderColor() != CLR_DEFAULT)
							{
								CBrush brush(pCoolToolbar->GetSelectedBorderColor());
								dc.FrameRect(rectButton,&brush);
								rectBackground.DeflateRect(1,1,1,1);
								if(pCoolToolbar->m_dwComCtlVersion >= _IE40_COMCTL_VERSION)
								{
									*pResult |= TBCDRF_NOEDGES;
								}
							}

							// draw background
							if(pCoolToolbar->GetSelectedBkColor() != CLR_DEFAULT)
							{
								dc.FillSolidRect(rectBackground,pCoolToolbar->GetSelectedBkColor());
							}
						}
						else
						{
							// set text color
							if(pCoolToolbar->GetHotTextColor() != CLR_DEFAULT)
							{
								if(pCoolToolbar->m_dwComCtlVersion >= _IE40_COMCTL_VERSION)
								{
									pTBCDRW->clrText = pCoolToolbar->GetHotTextColor();
								}
								else
								{
									::SetTextColor(pCDRW->hdc, pCoolToolbar->GetHotTextColor());
								}
							}

							// draw border
							if(pCoolToolbar->GetHotBorderColor() != CLR_DEFAULT)
							{
								CBrush brush(pCoolToolbar->GetHotBorderColor());
								dc.FrameRect(rectButton,&brush);
								rectBackground.DeflateRect(1,1,1,1);
								if(pCoolToolbar->m_dwComCtlVersion >= _IE40_COMCTL_VERSION)
								{
									*pResult |= TBCDRF_NOEDGES;
								}
							}

							// draw background
							if(pCoolToolbar->GetHotBkColor() != CLR_DEFAULT)
							{
								dc.FillSolidRect(rectBackground,pCoolToolbar->GetHotBkColor());
							}
						}
					}
					else if(pCDRW->uItemState & CDIS_SELECTED)
					{
						// set text color
						if(pCoolToolbar->GetSelectedTextColor() != CLR_DEFAULT)
						{
							if(pCoolToolbar->m_dwComCtlVersion >= _IE40_COMCTL_VERSION)
							{
								pTBCDRW->clrText = pCoolToolbar->GetSelectedTextColor();
							}
							else
							{
								::SetTextColor(pCDRW->hdc, pCoolToolbar->GetSelectedTextColor());
							}
						}

						// draw border
						if(pCoolToolbar->GetSelectedBorderColor() != CLR_DEFAULT)
						{
							CBrush brush(pCoolToolbar->GetSelectedBorderColor());
							dc.FrameRect(rectButton,&brush);
							rectBackground.DeflateRect(1,1,1,1);
							if(pCoolToolbar->m_dwComCtlVersion >= _IE40_COMCTL_VERSION)
							{
								*pResult |= TBCDRF_NOEDGES;
							}
						}

						// draw background
						if(pCoolToolbar->GetSelectedBkColor() != CLR_DEFAULT)
						{
							dc.FillSolidRect(rectBackground,pCoolToolbar->GetSelectedBkColor());
						}
					}
					else
					{
						// set text color
						if(pCoolToolbar->GetDefaultTextColor() != CLR_DEFAULT)
						{
							if(pCoolToolbar->m_dwComCtlVersion >= _IE40_COMCTL_VERSION)
							{
								pTBCDRW->clrText = pCoolToolbar->GetDefaultTextColor();
							}
							else
							{
								::SetTextColor(pCDRW->hdc, pCoolToolbar->GetDefaultTextColor());
							}
						}

						// draw border
						if(pCoolToolbar->GetDefaultBorderColor() != CLR_DEFAULT)
						{
							CBrush brush(pCoolToolbar->GetDefaultBorderColor());
							dc.FrameRect(rectButton,&brush);
							rectBackground.DeflateRect(1,1,1,1);
							if(pCoolToolbar->m_dwComCtlVersion >= _IE40_COMCTL_VERSION)
							{
								*pResult |= TBCDRF_NOEDGES;
							}
						}

						// draw background
						if(pCoolToolbar->GetDefaultBkColor() != CLR_DEFAULT)
						{
							dc.FillSolidRect(rectBackground,pCoolToolbar->GetDefaultBkColor());
						}
					}
				}

				break;
			}

		case CDDS_ITEMPOSTPAINT:
			{
				int nIndex=pCoolToolbar->CommandToIndex(PtrToUint(pCDRW->dwItemSpec));

				if(!pCoolToolbar->GetToolBarCtrl().IsButtonHidden(PtrToInt(pCDRW->dwItemSpec)) && 
					pCoolToolbar->IsDropDownArrow() && (pCoolToolbar->GetButtonStyle(nIndex) & TBSTYLE_DROPDOWN))
				{
					CRect rectDropDownArrow(pCDRW->rc);
					rectDropDownArrow.left = 
						rectDropDownArrow.right - pCoolToolbar->GetDropDownArrowWidth();

					// draw border around drop down button
					if(pCDRW->uItemState & CDIS_CHECKED)
					{
						if(pCDRW->uItemState & CDIS_HOT)
						{
							if(pCoolToolbar->GetSelectedBorderColor() != CLR_DEFAULT)
							{
								CBrush brush(pCoolToolbar->GetSelectedBorderColor());
								dc.FrameRect(rectDropDownArrow,&brush);
							}
						}
						else
						{
							if(pCoolToolbar->GetCheckedBorderColor() != CLR_DEFAULT)
							{
								CBrush brush(pCoolToolbar->GetCheckedBorderColor());
								dc.FrameRect(rectDropDownArrow,&brush);
							}
						}
					}
					else if(pCDRW->uItemState & CDIS_HOT)
					{
						if(pCDRW->uItemState & CDIS_SELECTED)
						{
							if(pCoolToolbar->GetSelectedBorderColor() != CLR_DEFAULT)
							{
								CBrush brush(pCoolToolbar->GetSelectedBorderColor());
								dc.FrameRect(rectDropDownArrow,&brush);
							}
						}
						else
						{
							if(pCoolToolbar->GetHotBorderColor() != CLR_DEFAULT)
							{
								CBrush brush(pCoolToolbar->GetHotBorderColor());
								dc.FrameRect(rectDropDownArrow,&brush);
							}
						}
					}
					else if(pCDRW->uItemState & CDIS_SELECTED)
					{
						if(pCoolToolbar->GetSelectedBorderColor() != CLR_DEFAULT)
						{
							CBrush brush(pCoolToolbar->GetSelectedBorderColor());
							dc.FrameRect(rectDropDownArrow,&brush);
						}
					}
					else
					{
						if(pCoolToolbar->GetDefaultBorderColor() != CLR_DEFAULT)
						{
							CBrush brush(pCoolToolbar->GetDefaultBorderColor());
							dc.FrameRect(rectDropDownArrow,&brush);
						}
					}
				}

				if(pCoolToolbar->IsButtonCustomized(nIndex) && !pCoolToolbar->IsCustomButton(nIndex))
				{
					CRect rect;
					pCoolToolbar->GetItemRect(nIndex,rect);
					pCoolToolbar->DrawCustomizedButton(dc,rect);
				}
#if _MFC_VER<=0x0421
				else if(pCDRW->uItemState & CDIS_CHECKED)
				{
					CRect rect;
					GetItemRect(nIndex,rect);

					if(IsDropDownArrow() && GetButtonStyle(nIndex) & TBSTYLE_DROPDOWN)
					{
						rect.right-=m_nDropDownArrowWidth;
					}

					COLORREF clrHilight=::GetSysColor(COLOR_BTNHILIGHT);
					COLORREF clrShadow=::GetSysColor(COLOR_BTNSHADOW);
					dc.Draw3dRect(rect,clrShadow,clrHilight);
				}
#endif	//	_MFC_VER<=0x0421
				break;
			}

		case CDDS_POSTPAINT:
			{
				break;
			}

		default:
			{
				// CToolBar does not send other draw-stages than the above.
				//TRACE(_T("Unknown draw stage ...\n"));
				break;
			}
		}

		dc.Detach();
	}
}

void COXToolbarSkinClassic::DrawBackground(CDC* pDC, COXCoolToolBar* pCoolToolbar)
{
	CRect rcClient;
	pCoolToolbar->GetClientRect(rcClient);

	pDC->FillSolidRect(rcClient, ::GetSysColor(COLOR_BTNFACE));
}

void COXToolbarSkinClassic::DrawMenuButtons(CDC* pDC, COXMenuBar* pMenuBar)
{
	ASSERT(pMenuBar->m_pFrameWnd!=NULL);
	ASSERT(::IsWindow(pMenuBar->m_pFrameWnd->m_hWnd));
	ASSERT_KINDOF(CMDIFrameWnd,pMenuBar->m_pFrameWnd);

    BOOL bMaximize=FALSE;
	CMDIChildWnd* pChild=((CMDIFrameWnd*)pMenuBar->m_pFrameWnd)->MDIGetActive(&bMaximize);
	if(pChild==NULL || !bMaximize)
		return;

	ASSERT(!pMenuBar->m_rectCloseBtn.IsRectEmpty());
	if(pMenuBar->m_rectCloseBtn.IsRectEmpty())
		return;

	DWORD dwStyle=pChild->GetStyle();

	UINT nPushState;

	// close button
	nPushState=(pMenuBar->m_pressedBtn==OX_MNU_CLOSEBTN) ? DFCS_PUSHED : 0;
	pDC->DrawFrameControl(&pMenuBar->m_rectCloseBtn,DFC_CAPTION,
		DFCS_CAPTIONCLOSE|nPushState);

	// Max/restore button
	if(!pMenuBar->m_rectRestoreBtn.IsRectEmpty())
	{
		UINT nState=DFCS_CAPTIONRESTORE;
		if((dwStyle&WS_MAXIMIZEBOX)==0)
			nState|=DFCS_INACTIVE;
		nPushState=(pMenuBar->m_pressedBtn==OX_MNU_RESTOREBTN) ? DFCS_PUSHED : 0;
		pDC->DrawFrameControl(&pMenuBar->m_rectRestoreBtn,DFC_CAPTION,nState|nPushState);
	}

	// Minimize button
	if(!pMenuBar->m_rectMinimizeBtn.IsRectEmpty())
	{
		UINT nState=DFCS_CAPTIONMIN;
		if((dwStyle&WS_MINIMIZEBOX)==0)
			nState|=DFCS_INACTIVE;
		nPushState=(pMenuBar->m_pressedBtn==OX_MNU_MINIMIZEBTN) ? DFCS_PUSHED : 0;
		pDC->DrawFrameControl(&pMenuBar->m_rectMinimizeBtn,DFC_CAPTION,nState|nPushState);
	}
}

// COXToolbarSkinXP
//

COXToolbarSkinXP::COXToolbarSkinXP(COXSkinXP* pSkinXP)
{
	m_pSkinXP = pSkinXP;
}

void COXToolbarSkinXP::DrawGripper(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar)
{
    if (pCoolToolbar->m_dwStyle & CBRS_FLOATING)
		return;		// no gripper if floating

	CRect rect(lpRect);
	CRect gripperRect(lpRect);

	if (pCoolToolbar->m_dwStyle & CBRS_ORIENT_HORZ) 
	{
		gripperRect.DeflateRect(2, 6, 2, 5);
		// gripper at left
		if (pCoolToolbar->m_pDockContext != NULL)
		{
			// EnableDocking was called
			gripperRect.left += 2;
			gripperRect.bottom -= 2;
		}
		else
		{
			if (pCoolToolbar->m_dwStyle & CBRS_BORDER_BOTTOM)
			{
				gripperRect.top -= 2;
				gripperRect.bottom -= 2;
			}
		}
		gripperRect.right = gripperRect.left + 3;
        DrawGripperStripes(pDC, gripperRect, true);
	} 
	else // vertical
	{
		gripperRect.DeflateRect(3, 4, 7, 4);
		// gripper at top
		if (pCoolToolbar->m_pDockContext == NULL)
		{
			// EnableDocking was not called
			gripperRect.right += 2;
		}
		else
		{
			gripperRect.left += 2;
		}
		gripperRect.bottom = gripperRect.top + 3;
		DrawGripperStripes(pDC, gripperRect, false);
	}
}

void COXToolbarSkinXP::DrawNonClientArea(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar)
{
	CRect rcNonClient(lpRect);
	
	// Get the client rectangle in window coordinates
	CRect rcClient, rectWindow;
	pCoolToolbar->GetWindowRect(rectWindow);
	pCoolToolbar->GetClientRect(rcClient);
	pCoolToolbar->ClientToScreen(rcClient);
	rcClient.OffsetRect(-rectWindow.left, -rectWindow.top);
	
	// Clip out the client area
//	pDC->ExcludeClipRect(rcClient);

	// Fill the background
	if (pCoolToolbar->IsKindOf(RUNTIME_CLASS(COXMenuBar)))
		pDC->FillSolidRect(lpRect, ::GetSysColor(COLOR_BTNFACE));
	else
		pDC->FillSolidRect(lpRect, m_pSkinXP->GetBackgroundColor());

	// Draw the border
	CRect rcBorder(lpRect);
	CBrush brush;
	brush.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));

	pDC->FrameRect(rcBorder, &brush);
	rcBorder.DeflateRect(1, 1);
	pDC->FrameRect(rcBorder, &brush);
	rcBorder.DeflateRect(1, 1);

	// Draw the corner pixels
	COLORREF clrCorner = ::GetSysColor(COLOR_BTNFACE);
	pDC->SetPixelV(rcBorder.TopLeft(), clrCorner);
	pDC->SetPixelV(rcBorder.right - 1, rcBorder.top, clrCorner);
	pDC->SetPixelV(rcBorder.right - 1, rcBorder.bottom - 1, clrCorner);
	pDC->SetPixelV(rcBorder.left, rcBorder.bottom - 1, clrCorner);
}

void COXToolbarSkinXP::DrawSeparator(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* /*pCoolToolbar*/)
{
	CRect rect(lpRect);

	if (rect.Height() > rect.Width()) 
	{
		// Horizontal separator
		rect.left = rect.CenterPoint().x - 1;
		rect.right = rect.left + 1;
	} 
	else 
	{
		// Vertical separator
		rect.top = rect.CenterPoint().y - 1;
		rect.bottom = rect.top + 1;
	}

	pDC->FillSolidRect(rect,  m_pSkinXP->GetSeparatorColor());
}

// Draws the button when in customize mode
void COXToolbarSkinXP::DrawCustomizedButton(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* /*pCoolToolbar*/)
{
	// Draw a black rectangle, 2 pixels wide
	CBrush brush;
	brush.CreateSolidBrush(::GetSysColor(COLOR_BTNTEXT));

	CRect rectItem(lpRect);
	pDC->FrameRect(rectItem, &brush);
	rectItem.DeflateRect(1, 1);
	pDC->FrameRect(rectItem, &brush);
}

void COXToolbarSkinXP::DrawGripperStripes(CDC* pDC, CRect &rect, bool bHorizontal)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, m_pSkinXP->GetGripperColor());
	CPen* pOldPen = pDC->SelectObject(&pen);

	if (bHorizontal)
	{
		int iYPos = rect.top;
		while (iYPos <= rect.bottom)
		{
			pDC->MoveTo(rect.left, iYPos);
			pDC->LineTo(rect.right, iYPos);
			iYPos += 2;
		}
	}
	else // vertical
	{
		int iXPos = rect.left;
		while (iXPos <= rect.right)
		{
			pDC->MoveTo(iXPos, rect.top);
			pDC->LineTo(iXPos, rect.bottom);
			iXPos += 2;
		}
	}

	pDC->SelectObject(pOldPen);
}

void COXToolbarSkinXP::OnCustomDraw(NMHDR*& pNotify, LRESULT*& pResult, COXCoolToolBar* pCoolToolbar)
{
	// The toolbar needs to have the TBSTYLE_FLAT style in order for this skin to work
	// in new version of comctl32.dll(4.71)
	// we use toolbar specific custom draw structure 

	LPNMTBCUSTOMDRAW pTBCDRW=NULL;
	LPNMCUSTOMDRAW pCDRW=NULL;
	if(pCoolToolbar->m_dwComCtlVersion>=_IE40_COMCTL_VERSION)
	{
		pTBCDRW=(LPNMTBCUSTOMDRAW)pNotify;
		pCDRW=&pTBCDRW->nmcd;
	}
	else
	{
		pCDRW=(LPNMCUSTOMDRAW)pNotify;
	}

	OXCTB_CUSTOMDRAW cd;
	if(pCoolToolbar->m_dwComCtlVersion>=_IE40_COMCTL_VERSION)
	{
		ASSERT(pTBCDRW!=NULL);
		cd.pCustomDraw=(LPNMCUSTOMDRAW)pTBCDRW;
	}
	else
	{
		cd.pCustomDraw=pCDRW;
	}
	cd.hdr.hwndFrom=pCDRW->hdr.hwndFrom;
	cd.hdr.idFrom=pCDRW->hdr.idFrom;
	cd.hdr.code=OXCTBN_CUSTOMDRAW;
	cd.result=CDRF_DODEFAULT;
	cd.bDoDefault=TRUE;

	CWnd* pParentWnd=NULL;
	if(pCoolToolbar->IsFloating())
	{
		pParentWnd=pCoolToolbar->GetTopLevelParent();
	}
	else
	{
		pParentWnd=pCoolToolbar->GetParentFrame();
	}

	ASSERT(pParentWnd != NULL);

	pParentWnd->SendMessage(WM_NOTIFY,cd.hdr.idFrom,(LPARAM)&cd);

	if(pCDRW->dwDrawStage==CDDS_PREPAINT && 
		pCoolToolbar->GetToolBarCtrl().GetButtonCount()>0)
	{
		CRect rect;
		pCoolToolbar->GetItemRect(0,&rect);
	}

	if(!cd.bDoDefault)
	{
		*pResult=cd.result;
	}
	else
	{
		CDC dc;
		dc.Attach(pTBCDRW->nmcd.hdc);

		// Draw to a memory dc in order to avoid flashing
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmpMem;
		CRect rectMem;
		pCoolToolbar->GetClientRect(rectMem);
		rectMem.OffsetRect(-rectMem.left, -rectMem.top);
		bmpMem.CreateCompatibleBitmap(&dc, rectMem.Width(), rectMem.Height());
		CBitmap* pOldBmp = dcMem.SelectObject(&bmpMem);

		*pResult = CDRF_DODEFAULT;
		switch( pCDRW->dwDrawStage )
		{
		case CDDS_PREPAINT:
			{
				// Draw the background
				DrawBackground(&dcMem, pCoolToolbar);

				// Draw each item
				CToolBarCtrl& ctrl = pCoolToolbar->GetToolBarCtrl();
				int iButonCount = ctrl.GetButtonCount();
				for (int i = 0; i < iButonCount; i++)
				{
					TBBUTTON tbb;
					ctrl.GetButton(i, &tbb);

					// Do not draw hidden items
					if (tbb.fsState & TBSTATE_HIDDEN)
						continue;

					CRect rectItem;
					ctrl.GetItemRect(i, rectItem);

					// Draw either a button or a separator
					if (tbb.fsStyle & TBSTYLE_SEP)
					{
						if (tbb.fsState & TBSTATE_WRAP)
						{
							// We have a horizontal separator
							CRect rectSep;
							pCoolToolbar->GetClientRect(rectSep);
							rectSep.bottom = rectItem.bottom;
							rectSep.top = rectItem.bottom - rectItem.Width();

							pCoolToolbar->DrawSeparator(dcMem, rectSep);
						}
						else // vertical separator
						{
							if (rectItem.Height() > rectItem.Width())
								pCoolToolbar->DrawSeparator(dcMem, rectItem);
						}
					}
					else // button
						DrawItem(&dcMem, &tbb, rectItem, pCoolToolbar);

					// If this is a customized button draw a border around it
					if (pCoolToolbar->IsButtonCustomized(i) && !pCoolToolbar->IsCustomButton(i))
						pCoolToolbar->DrawCustomizedButton(dcMem, rectItem);
				}

				// Draw the insert mark if any
				DrawInsertMark(&dcMem, pCoolToolbar);

				*pResult = CDRF_SKIPDEFAULT;
				break;
			}

		}

		dc.BitBlt(0, 0, rectMem.right, rectMem.bottom, &dcMem, 0, 0, SRCCOPY);
		dcMem.SelectObject(pOldBmp);
		dc.Detach();
	}
}

void COXToolbarSkinXP::DrawItem(CDC* pDC, TBBUTTON* pTBB, LPCRECT lpRectItem, COXCoolToolBar* pCoolToolbar, BOOL bDrawBackground)
{
	// Determine the state flags
	bool bSelected = ((pTBB->fsState & TBSTATE_PRESSED) != 0);
	bool bHot = (pCoolToolbar->GetToolBarCtrl().GetHotItem() == pCoolToolbar->CommandToIndex(pTBB->idCommand));
	bool bDisabled = ((pTBB->fsState & TBSTATE_ENABLED) == 0);
	bool bChecked = ((pTBB->fsState & TBSTATE_CHECKED) != 0);

	DWORD dwStyleEx = pCoolToolbar->GetStyleEx();

	if (pCoolToolbar->m_iDropDownIndex == pTBB->idCommand)
	{
		// We have a droped drop down button
		pCoolToolbar->m_iDropDownIndex = -1;

		if (bDrawBackground)
			DrawDroppedItemBackground(pDC, lpRectItem);

		DrawItemIcon(pDC, pTBB, dwStyleEx, lpRectItem, true, true, false, pCoolToolbar);
		DrawItemText(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetTextColor(), pCoolToolbar);
	}
	else if (bSelected && bHot)
	{
		// Selected and hot
		if (bDrawBackground)
			DrawItemBackground(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetActiveBorderColor(),
				m_pSkinXP->GetHotSelectedItemColor());
		
		DrawItemIcon(pDC, pTBB, dwStyleEx, lpRectItem, false, true, false, pCoolToolbar);
		DrawItemText(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetSelectedTextColor(), pCoolToolbar);
	}
	else if (bChecked && bHot)
	{
		// Checked and hot - same as selected and hot
		if (bDrawBackground)
			DrawItemBackground(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetActiveBorderColor(),
				m_pSkinXP->GetHotSelectedItemColor());
		
		DrawItemIcon(pDC, pTBB, dwStyleEx, lpRectItem, false, true, false, pCoolToolbar);
		DrawItemText(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetSelectedTextColor(), pCoolToolbar);
	}
	else if (bChecked)
	{
		// Just chequed
		if (bDrawBackground)
			DrawItemBackground(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetActiveBorderColor(),
				m_pSkinXP->GetCheckedItemColor());
		
		DrawItemIcon(pDC, pTBB, dwStyleEx, lpRectItem, false, false, false, pCoolToolbar);
		DrawItemText(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetTextColor(), pCoolToolbar);
	}
	else if (bHot)
	{
		// Just hot
		if (bDrawBackground)
			DrawItemBackground(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetActiveBorderColor(),
				m_pSkinXP->GetHotItemColor());

		DrawItemIcon(pDC, pTBB, dwStyleEx, lpRectItem, true, true, false, pCoolToolbar);
		DrawItemText(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetTextColor(), pCoolToolbar);
	}
	else if (bSelected)
	{
		// This is a menu item, so decrease the height
		if (bDrawBackground)
		{
			CRect rectItem(lpRectItem);
			rectItem.DeflateRect(0, 1);
			DrawDroppedItemBackground(pDC, rectItem);
		}

		DrawItemIcon(pDC, pTBB, dwStyleEx, lpRectItem, true, true, false, pCoolToolbar);
		DrawItemText(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetTextColor(), pCoolToolbar);
	}
	else if (bDisabled)
	{
		// Disabled
		DrawItemIcon(pDC, pTBB, dwStyleEx, lpRectItem, false, false, true, pCoolToolbar);
		DrawItemText(pDC, pTBB, dwStyleEx, lpRectItem, ::GetSysColor(COLOR_GRAYTEXT), pCoolToolbar);
	}
	else
	{
		// Normal
		DrawItemIcon(pDC, pTBB, dwStyleEx, lpRectItem, false, false, false, pCoolToolbar);
		DrawItemText(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetTextColor(), pCoolToolbar);		
	}
}

void COXToolbarSkinXP::DrawItemBackground(CDC* pDC, TBBUTTON* pTBB, DWORD dwStyleEx, LPCRECT lpRect, COLORREF crBorder, COLORREF crInside)
{
	// Fill the inside
	CRect rect(lpRect);
	rect.DeflateRect(1, 1);
	pDC->FillSolidRect(rect, crInside);
	rect.InflateRect(1, 1);

	if (pTBB->iBitmap == -1 && (pTBB->fsStyle & TBSTYLE_DROPDOWN) && !(dwStyleEx & TBSTYLE_EX_DRAWDDARROWS))
	{
		// This is a menu item, so decrease the height
		rect.DeflateRect(0, 1);
	}

	// Draw the borders
	CBrush brush;
	brush.CreateSolidBrush(crBorder);
	pDC->FrameRect(rect, &brush);

	// If this is drop arrow button, frame the arrow part
	if ((pTBB->fsStyle & TBSTYLE_DROPDOWN) && (dwStyleEx & TBSTYLE_EX_DRAWDDARROWS))
		pDC->FrameRect(GetDropArrowRect(pDC, rect), &brush);
}

// Draws a raised background with a shadow
void COXToolbarSkinXP::DrawDroppedItemBackground(CDC* pDC, LPCRECT lpRect)
{
	CRect rectFill(lpRect);
	pDC->FillSolidRect(rectFill, m_pSkinXP->GetBackgroundColor());
}

// Draws the icon of the button according to the button's state
void COXToolbarSkinXP::DrawItemIcon(CDC* pDC, TBBUTTON* pTBB, DWORD dwStyleEx, LPCRECT lpRect, bool bRaised, bool bHot, bool bGrayed, COXCoolToolBar* pCoolToolbar)
{
	if (pCoolToolbar->IsKindOf(RUNTIME_CLASS(COXMenuBar)))
		return;

	if (pTBB->iBitmap == -1)
		return; // there is no icon

	CToolBarCtrl& ctrl = pCoolToolbar->GetToolBarCtrl();
	CImageList* pImageList = ctrl.GetImageList();
	CRect rcItem(lpRect);
	
	// If this is a drop down item the rectangle needs to be decremented
	if ((pTBB->fsStyle & TBSTYLE_DROPDOWN)  && (dwStyleEx & TBSTYLE_EX_DRAWDDARROWS))
	{
		CRect rectDropArrow = GetDropArrowRect(pDC, lpRect);
		rcItem.right -= rectDropArrow.Width();

		// Might as well draw the down arrow here
		CFont fontWebdings;
		fontWebdings.CreatePointFont(80, _T("Marlett"));
		CFont* pOldFont = pDC->SelectObject(&fontWebdings);
		int iOldBkMode = pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(_T("6"), rectDropArrow, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		pDC->SetBkMode(iOldBkMode);
		pDC->SelectObject(pOldFont);
	}

	// Get the image rectangle and size
	CRect rectImage = GetIconRect(pTBB->idCommand, pCoolToolbar);
	CSize sizeImage(rectImage.Width(), rectImage.Height());

	// Determine the top left coordinate of the icon
	CPoint ptTopLeft;
	CString strText = pCoolToolbar->GetButtonText(pCoolToolbar->CommandToIndex(pTBB->idCommand));
	if (strText.IsEmpty())
	{
		// There is not text, so the icon is in the middle of the button	
		ptTopLeft.x = rcItem.left + (rcItem.Width() - sizeImage.cx) / 2;
		ptTopLeft.y = rcItem.top + (rcItem.Height() - sizeImage.cy) / 2;
	}
	else // there is text
	{
		if (pCoolToolbar->IsList())
		{
			// The icon is on the left
			ptTopLeft.x = rcItem.left + 3;
			ptTopLeft.y = rcItem.top + (rcItem.Height() - sizeImage.cy) / 2;
		}
		else // not list
		{
			// The icon is on top
			ptTopLeft.x = rcItem.left + (rcItem.Width() - sizeImage.cx) / 2;
			ptTopLeft.y = rcItem.top + 3;
		}
	}

	if (bHot)
	{
		// Hot image - raised with a shadow

		CImageList* pHotImageList = ctrl.GetHotImageList();
		if (pHotImageList == NULL)
			pHotImageList = pImageList;

		if (bRaised) // raised icon with a shadow
		{
			// Draw the image shadow first
			ptTopLeft.x += 1;
			ptTopLeft.y += 1;
			CBrush brush;
			brush.CreateSolidBrush(::GetSysColor(COLOR_BTNSHADOW));
			CBrush* pOldBrush = pDC->SelectObject(&brush);
			COLORREF clrOldBkColor = pDC->SetBkColor(RGB(255, 255, 255));
			pHotImageList->DrawIndirect(pDC, pTBB->iBitmap, ptTopLeft, sizeImage,
				CPoint(0, 0), ILD_ROP | ILD_MASK, 0x00B8074A);
			pDC->SetBkColor(clrOldBkColor);
			pDC->SelectObject(pOldBrush);

			ptTopLeft.x -= 2;
			ptTopLeft.y -= 2;
		}

		// Draw the image
		pHotImageList->Draw(pDC, pTBB->iBitmap, ptTopLeft, ILD_TRANSPARENT);
	}
	else if (bGrayed)
	{
		// Disabled image - grayed

		CImageList* pDisabledImageList = ctrl.GetDisabledImageList();
		if (pDisabledImageList != NULL)
			pDisabledImageList->Draw(pDC, pTBB->iBitmap, ptTopLeft, ILD_TRANSPARENT);
		else
		{
			CBrush brush;
			brush.CreateSolidBrush(m_pSkinXP->GetDisabledTextColor());
			HICON hIcon = pImageList->ExtractIcon(pTBB->iBitmap);
			if (hIcon != NULL)
			{
				pDC->DrawState(ptTopLeft, sizeImage, hIcon, DST_ICON | DSS_MONO, &brush);
				::DestroyIcon(hIcon);
			}
		}
	}
	else
	{
		// Normal
		pImageList->Draw(pDC, pTBB->iBitmap, ptTopLeft, ILD_TRANSPARENT);
	}
}

void COXToolbarSkinXP::DrawBackground(CDC* pDC, COXCoolToolBar* pCoolToolbar)
{
	CRect rcClient;
	pCoolToolbar->GetClientRect(rcClient);

	if (pCoolToolbar->IsKindOf(RUNTIME_CLASS(COXMenuBar)))
		pDC->FillSolidRect(rcClient, ::GetSysColor(COLOR_BTNFACE));
	else
		pDC->FillSolidRect(rcClient, m_pSkinXP->GetBackgroundColor());
}

void COXToolbarSkinXP::OnPaintComboBox(COXSkinnedComboBox* pComboBox)
{
	pComboBox->Default();
	CPaintDC dc(pComboBox);
	DrawComboBox(pComboBox);
}

void COXToolbarSkinXP::DrawComboBox(COXSkinnedComboBox* pComboBox)
{
	CClientDC dc(pComboBox);

	CRect rect;
	pComboBox->GetClientRect(rect);
	POINT ptCursor;
	::GetCursorPos(&ptCursor);
	pComboBox->ScreenToClient(&ptCursor);

	// Determine and exclude the edit box rectangle
	CRect rectEdit(rect);
	rectEdit.DeflateRect(3, 3, rect.Height() - 2, 3);

	dc.ExcludeClipRect(rectEdit);
	rectEdit.InflateRect(1, 1);
	if (pComboBox->m_bCommonControls6)
		rectEdit.right += 3;

	// Determine the button rectangle
	CRect rectButton(rect);
	rectButton.DeflateRect(2, 2, 3, 2);
	rectButton.left = rectEdit.Width() + 2;

	// Fill the background
	dc.FillSolidRect(rect, m_pSkinXP->GetBackgroundColor());

	// Draw the inner white border
	CBrush brushWhite;
	COLORREF clrWhite = ::GetSysColor(COLOR_WINDOW);
	brushWhite.CreateSolidBrush(clrWhite);
	rectEdit.InflateRect(1, 1, 0, 0);
	dc.FillSolidRect(rectEdit, clrWhite);

	rectButton.InflateRect(1, 1, 0, 0);
	dc.FrameRect(rectButton, &brushWhite);

	// The combo box can have three states - normal, hot and dropped
	// Determine the state of the combo box
	CBrush brush;
	COLORREF clrArrow = ::GetSysColor(COLOR_BTNTEXT);
	if (pComboBox->GetDroppedState())
	{
		// The combo box is dropped

		brush.CreateSolidBrush(m_pSkinXP->GetHotSelectedItemColor());
		clrArrow = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	}
	else if (rect.PtInRect(ptCursor) || CWnd::GetFocus() == pComboBox ||
		(CWnd::GetFocus() && CWnd::GetFocus()->GetParent() == pComboBox))
	{
		// The combo box is hot
		brush.CreateSolidBrush(m_pSkinXP->GetHotItemColor());
	}

	// Perform the drawing of a droped or hot combo box
	if (brush.m_hObject != NULL)
	{
		rectEdit.InflateRect(1, 1, 0, 1);
		rectButton.InflateRect(0, 1, 1, 1);

		// Draw the button
		CPen penBorder;
		penBorder.CreatePen(PS_SOLID, 1, m_pSkinXP->GetActiveBorderColor());
		CPen* pOldPen = dc.SelectObject(&penBorder);
		CBrush* pOldBrush = dc.SelectObject(&brush);
		dc.Rectangle(rectButton);
		dc.SelectObject(pOldBrush);
		dc.Rectangle(rectEdit);
		dc.SelectObject(pOldPen);
	}
	
	// Draw the drop arrow
	CPoint ptCenter = rectButton.CenterPoint();
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, clrArrow);
	CPen* pOldPen = dc.SelectObject(&pen);
	dc.MoveTo(ptCenter.x - 2, ptCenter.y - 1);
	dc.LineTo(ptCenter.x + 3, ptCenter.y - 1);
	dc.MoveTo(ptCenter.x - 1, ptCenter.y);
	dc.LineTo(ptCenter.x + 2, ptCenter.y);
	dc.SetPixelV(ptCenter.x, ptCenter.y + 1, clrArrow);
	dc.SelectObject(pOldPen);
}

void COXToolbarSkinXP::DrawWndFrame(CWnd* pWnd)
{
	CWindowDC dc(pWnd);

	CRect rect;
	pWnd->GetWindowRect(rect);
	CRect rectWindow(rect);
	pWnd->ScreenToClient(&rect);
	CPoint ptOffset(-rect.left, -rect.top);
	rect.OffsetRect(ptOffset);

	// Exclude the client rectangle
	CRect rectClient;
	pWnd->GetClientRect(rectClient);
	rectClient.OffsetRect(ptOffset);
	dc.ExcludeClipRect(rectClient);

	// Fill the background
	dc.FillSolidRect(rect, m_pSkinXP->GetBackgroundColor());

	// Draw the inner white border
	CBrush brushWhite;
	COLORREF clrWhite = ::GetSysColor(COLOR_WINDOW);
	brushWhite.CreateSolidBrush(clrWhite);
	rect.DeflateRect(1, 1);
	dc.FillSolidRect(rect, clrWhite);

	// Determine the state of the edit box
	CPoint ptCursor;
	::GetCursorPos(&ptCursor);
	if (rectWindow.PtInRect(ptCursor) || CWnd::GetFocus() == pWnd)
	{
		// The combo box is hot - draw the border
		rect.InflateRect(1, 1);
		CBrush brush;
		brush.CreateSolidBrush(m_pSkinXP->GetActiveBorderColor());
		dc.FrameRect(rect, &brush);
	}
}

// Draw the text in the specified rectangle
void COXToolbarSkinXP::DrawItemText(CDC* pDC, TBBUTTON* pTBB, DWORD dwStyleEx, LPCRECT lpRectItem, COLORREF clrText, COXCoolToolBar* pCoolToolbar)
{
	CString strText;

	// This is either a toolbar or a menubar
	COXMenuBar* pMenuBar = DYNAMIC_DOWNCAST(COXMenuBar, pCoolToolbar);
	if (pMenuBar != NULL && (pTBB->fsStyle & TBSTYLE_DROPDOWN) && !(dwStyleEx & TBSTYLE_EX_DRAWDDARROWS))
		strText = pMenuBar->GetItemText(pTBB->idCommand - 1);
	else
		strText = pCoolToolbar->GetButtonText(pCoolToolbar->CommandToIndex(pTBB->idCommand));

	if (strText.IsEmpty())
		return; // there is no text
	
	CRect rectItem(lpRectItem);

	// Get the image rectangle and size
	CRect rectImage = GetIconRect(pTBB->idCommand, pCoolToolbar);
	CSize sizeImage(rectImage.Width(), rectImage.Height());
	
	// If this is a drop down item the rectangle needs to be decremented
	if ((pTBB->fsStyle & TBSTYLE_DROPDOWN)  && (dwStyleEx & TBSTYLE_EX_DRAWDDARROWS))
	{
		CRect rectDropArrow = GetDropArrowRect(pDC, lpRectItem);
		rectItem.right -= rectDropArrow.Width();
	}

	CRect rectIcon = GetIconRect(pTBB->idCommand, pCoolToolbar);

	// Determine the text rectangle
	if (pCoolToolbar->IsList())
	{
		// The text needs to be to the right of the icon
		rectItem.left += sizeImage.cx + 6;
		rectItem.right -= 3;
	}
	else // not list
	{
		// The text needs to be under the icon
		rectItem.top += rectIcon.Height() + 1;
		rectItem.DeflateRect(3, 3);
	}

	DWORD dwDTFlags = DT_WORDBREAK | DT_END_ELLIPSIS;
	if (pCoolToolbar->GetTextRows() == 1)
		dwDTFlags |= DT_SINGLELINE;
	
	if (pCoolToolbar->IsList())
		dwDTFlags |= DT_VCENTER;
	else
		dwDTFlags |= DT_CENTER;

	COLORREF clrOldColor = pDC->SetTextColor(clrText);
	CFont* pOldFont = pDC->SelectObject(pCoolToolbar->GetFont());
	int iOldBkMode = pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(strText, rectItem, dwDTFlags);
	pDC->SetBkMode(iOldBkMode);
	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(clrOldColor);
}

// Returns the rectangle of the item's icon
CRect COXToolbarSkinXP::GetIconRect(DWORD dwCommandID, COXCoolToolBar* pCoolToolbar)
{
	CImageList* pImageList = pCoolToolbar->GetToolBarCtrl().GetImageList();

	// Get the image index of the item
	int iImageIndex;
	UINT nDummy;
	pCoolToolbar->GetButtonInfo(pCoolToolbar->CommandToIndex(dwCommandID), nDummy, nDummy, iImageIndex);

	if (iImageIndex == -1)
		return CRect(0, 0, 0, 0); // no icon

	// Get the image rectangle and size
	IMAGEINFO img_info;
	pImageList->GetImageInfo(iImageIndex, &img_info);

	return img_info.rcImage;
}

// Determine the rectangle of the drop arrow button
CRect COXToolbarSkinXP::GetDropArrowRect(CDC* pDC, LPCRECT lpRectItem)
{
	CRect rectItem(lpRectItem);

	// Determine the width of the drop arrow character
	CFont fontWebdings;
	fontWebdings.CreatePointFont(80, _T("Marlett"));
	CFont* pOldFont = pDC->SelectObject(&fontWebdings);
	CRect rectChar(0, 0, 0, 0);
	pDC->DrawText(_T("6"), rectChar, DT_CALCRECT);
	pDC->SelectObject(pOldFont);

	rectItem.left = rectItem.right - rectChar.right - 4;
	return rectItem;
}

// Draws the insert mark of the toolbar when in customize mode
void COXToolbarSkinXP::DrawInsertMark(CDC* pDC, COXCoolToolBar* pCoolToolbar)
{
	// Get the insert mark
	CToolBarCtrl& ctrl = pCoolToolbar->GetToolBarCtrl();
	TBINSERTMARK tbim;
	ctrl.GetInsertMark(&tbim);

	if (tbim.iButton == -1)
		return; // there is no insert mark

	// Determine the position rectangle of the insert mark
	CRect rectMark;
	pCoolToolbar->GetItemRect(tbim.iButton, &rectMark);
	if (tbim.dwFlags == 0)
	{
		// The insert mark is before the button
		rectMark.right = rectMark.left + 3;
	}
	else if (tbim.dwFlags & TBIMHT_AFTER)
	{
		// The insert mark is after the button
		rectMark.left = rectMark.right - 3;
	}
	else
		return;

	// Do the actual drawing
	pDC->FillSolidRect(rectMark, ::GetSysColor(COLOR_BTNTEXT));
}

void COXToolbarSkinXP::DrawMenuButtons(CDC* pDC, COXMenuBar* pMenuBar)
{
	ASSERT(pMenuBar->m_pFrameWnd!=NULL);
	ASSERT(::IsWindow(pMenuBar->m_pFrameWnd->m_hWnd));
	ASSERT_KINDOF(CMDIFrameWnd,pMenuBar->m_pFrameWnd);

    BOOL bMaximize=FALSE;
	CMDIChildWnd* pChild=((CMDIFrameWnd*)pMenuBar->m_pFrameWnd)->MDIGetActive(&bMaximize);
	if(pChild==NULL || !bMaximize)
		return;

	ASSERT(!pMenuBar->m_rectCloseBtn.IsRectEmpty());
	if(pMenuBar->m_rectCloseBtn.IsRectEmpty())
		return;

	// close button
	m_pSkinXP->DrawFrameButton(pDC, pMenuBar->m_rectCloseBtn, DFCS_CAPTIONCLOSE,
		FALSE, FALSE, ::GetSysColor(COLOR_BTNFACE), pMenuBar);

	// Max/restore button
	if (!pMenuBar->m_rectRestoreBtn.IsRectEmpty())
		m_pSkinXP->DrawFrameButton(pDC, pMenuBar->m_rectRestoreBtn, DFCS_CAPTIONRESTORE,
		FALSE, FALSE, ::GetSysColor(COLOR_BTNFACE), pMenuBar);

	// Minimize button
	if (!pMenuBar->m_rectMinimizeBtn.IsRectEmpty())
		m_pSkinXP->DrawFrameButton(pDC, pMenuBar->m_rectMinimizeBtn, DFCS_CAPTIONMIN,
		FALSE, FALSE, ::GetSysColor(COLOR_BTNFACE), pMenuBar);
}

// COXToolbarSkin2003
//

COXToolbarSkin2003::COXToolbarSkin2003(COXSkin2003* pSkin2003) :
	COXToolbarSkinXP(pSkin2003)
{
}

COXToolbarSkin2003::~COXToolbarSkin2003()
{
}

void COXToolbarSkin2003::DrawBackground(CDC* pDC, COXCoolToolBar* pCoolToolbar)
{
	CRect rcClient;
	pCoolToolbar->GetWindowRect(rcClient);
	pCoolToolbar->ScreenToClient(rcClient);

	if (pCoolToolbar->IsKindOf(RUNTIME_CLASS(COXMenuBar)))
	{
		CRect rectMain;
		pCoolToolbar->GetParentFrame()->GetWindowRect(rectMain);

		// Get the toolbar screen rectangle
		CRect rectToolbar;
		pCoolToolbar->GetWindowRect(rectToolbar);

		int iWidth = ::GetSystemMetrics(SM_CXSCREEN);
		if (iWidth < rectMain.Width())
			iWidth = rectMain.Width();

		CRect rectGradient;
		rectGradient.left = rectMain.left - rectToolbar.left - 2;
		rectGradient.right = rectGradient.left + iWidth;
		rectGradient.top = 0;
		rectGradient.bottom = rectToolbar.Height();

		CRect rectSolid(rectGradient);
		rectGradient.right = rectGradient.CenterPoint().x;
		rectSolid.left = rectGradient.right;

		COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;
		pSkin2003->GradientFill(pDC, rectGradient,
			pSkin2003->GetDockbarLeftColor(),
			pSkin2003->GetDockbarRightColor(),
			GRADIENT_FILL_RECT_H);
		pDC->FillSolidRect(rectSolid, pSkin2003->GetDockbarRightColor());
	}
	else
	{
		COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;
		if (pCoolToolbar->m_dwStyle & CBRS_ORIENT_HORZ)
			pSkin2003->GradientFill(pDC, rcClient,
				pSkin2003->GetToolbarLightColor(),
				pSkin2003->GetToolbarDarkColor(),
				GRADIENT_FILL_RECT_V);
		else
			pSkin2003->GradientFill(pDC, rcClient,
				pSkin2003->GetToolbarLightColor(),
				pSkin2003->GetToolbarDarkColor(),
				GRADIENT_FILL_RECT_H);
	}
}

void COXToolbarSkin2003::DrawNonClientArea(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar)
{
	CRect rcNonClient(lpRect);

	// Get the client rectangle in window coordinates
	CRect rcClient, rectWindow;
	pCoolToolbar->GetWindowRect(rectWindow);
	pCoolToolbar->GetClientRect(rcClient);
	pCoolToolbar->ClientToScreen(rcClient);
	rcClient.OffsetRect(-rectWindow.left, -rectWindow.top);
	
	// Clip out the client area
	pDC->ExcludeClipRect(rcClient);

	// Fill the background

	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;

	// Draw the border gradient first, which should match the dockbar
	
	if (pCoolToolbar->m_pDockBar != NULL)
	{
		CRect rectMain;
		pCoolToolbar->GetParentFrame()->GetWindowRect(rectMain);

		// Get the toolbar screen rectangle
		CRect rectToolbar;
		pCoolToolbar->GetWindowRect(rectToolbar);

		int iWidth = ::GetSystemMetrics(SM_CXSCREEN);
		if (iWidth < rectMain.Width())
			iWidth = rectMain.Width();

		CRect rectGradient;
		rectGradient.left = rectMain.left - rectToolbar.left;
		rectGradient.right = rectGradient.left + iWidth;
		rectGradient.top = 0;
		rectGradient.bottom = rectToolbar.Height();

		CRect rectSolid(rectGradient);
		rectGradient.right = rectGradient.CenterPoint().x;
		rectSolid.left = rectGradient.right;

		if (::GetVersion() >= 0x80000000)
			// Adoid a problem with the ::GradientFill() API in Win98 and ME
			pSkin2003->GradientFillManual(pDC, rectGradient,
				pSkin2003->GetDockbarLeftColor(),
				pSkin2003->GetDockbarRightColor(),
				GRADIENT_FILL_RECT_H);
		else
			pSkin2003->GradientFill(pDC, rectGradient,
				pSkin2003->GetDockbarLeftColor(),
				pSkin2003->GetDockbarRightColor(),
				GRADIENT_FILL_RECT_H);

		pDC->FillSolidRect(rectSolid, pSkin2003->GetDockbarRightColor());
	}
	else
		pDC->FillSolidRect(rcNonClient, pSkin2003->GetDockbarBackgroundColor());


	if (!pCoolToolbar->IsKindOf(RUNTIME_CLASS(COXMenuBar)))
	{
		CRect rectInner(lpRect);
		rectInner.DeflateRect(2, 2);

		// Save the four corner pixels
		COLORREF clrPixels[4];
		clrPixels[0] = pDC->GetPixel(rectInner.TopLeft());
		clrPixels[1] = pDC->GetPixel(rectInner.right - 1, rectInner.top);
		clrPixels[2] = pDC->GetPixel(rectInner.left, rectInner.bottom - 1);
		clrPixels[3] = pDC->GetPixel(rectInner.right - 1, rectInner.bottom - 1);

		// Now draw the toolbar gradient
		ULONG ulGradientOrientation;
		if (pCoolToolbar->m_dwStyle & CBRS_ORIENT_HORZ)
			ulGradientOrientation = GRADIENT_FILL_RECT_V;
		else
			ulGradientOrientation = GRADIENT_FILL_RECT_H;

		if (::GetVersion() >= 0x80000000)
			// Adoid a problem with the ::GradientFill() API in Win98 and ME
			pSkin2003->GradientFillManual(pDC, rectInner,
				pSkin2003->GetToolbarLightColor(),
				pSkin2003->GetToolbarDarkColor(),
				ulGradientOrientation);
		else
			pSkin2003->GradientFill(pDC, rectInner,
				pSkin2003->GetToolbarLightColor(),
				pSkin2003->GetToolbarDarkColor(),
				ulGradientOrientation);

		// Restore the four corner pixels
		pDC->SetPixelV(rectInner.TopLeft(), clrPixels[0]);
		pDC->SetPixelV(rectInner.right - 1, rectInner.top, clrPixels[1]);
		pDC->SetPixelV(rectInner.left, rectInner.bottom - 1, clrPixels[2]);
		pDC->SetPixelV(rectInner.right - 1, rectInner.bottom - 1, clrPixels[3]);
	}
}

void COXToolbarSkin2003::DrawGripper(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar)
{
    if (pCoolToolbar->m_dwStyle & CBRS_FLOATING)
		return;		// no gripper if floating

	CRect rect(lpRect);
	CRect gripperRect(lpRect);
	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;

	if (pCoolToolbar->m_dwStyle & CBRS_ORIENT_HORZ) 
	{
		gripperRect.DeflateRect(2, 6, 2, 5);
		gripperRect.OffsetRect(1, 0);
		// gripper at left
		if (pCoolToolbar->m_pDockContext != NULL)
		{
			// EnableDocking was called
			gripperRect.left += 2;
			gripperRect.bottom -= 2;
		}
		else
		{
			if (pCoolToolbar->m_dwStyle & CBRS_BORDER_BOTTOM)
			{
				gripperRect.top -= 2;
				gripperRect.bottom -= 2;
			}
		}
		gripperRect.right = gripperRect.left + 2;
        pSkin2003->DrawGripperStripes(pDC, gripperRect, true);
	} 
	else // vertical
	{
		gripperRect.DeflateRect(3, 4, 7, 4);
		gripperRect.OffsetRect(0, 1);
		// gripper at top
		if (pCoolToolbar->m_pDockContext == NULL)
		{
			// EnableDocking was not called
			gripperRect.right += 2;
		}
		else
		{
			gripperRect.left += 2;
		}
		gripperRect.bottom = gripperRect.top + 2;
		pSkin2003->DrawGripperStripes(pDC, gripperRect, false);
	}
}

void COXToolbarSkin2003::DrawSeparator(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* /*pCoolToolbar*/)
{
	CRect rect(lpRect);

	if (rect.Height() > rect.Width()) 
	{
		// Horizontal separator
		rect.left = rect.CenterPoint().x - 1;
		rect.right = rect.left + 1;
		rect.top += 4;
		rect.bottom -= 4;
	} 
	else 
	{
		// Vertical separator
		rect.top = rect.CenterPoint().y - 1;
		rect.bottom = rect.top + 1;
		rect.left += 4;
		rect.right -= 4;
	}

	pDC->FillSolidRect(rect,  m_pSkinXP->GetSeparatorColor());
	rect.OffsetRect(1, 1);
	pDC->FillSolidRect(rect,  RGB(255,255,255)); // This color is always white
}

void COXToolbarSkin2003::DrawMenuButtons(CDC* pDC, COXMenuBar* pMenuBar)
{
	ASSERT(pMenuBar->m_pFrameWnd!=NULL);
	ASSERT(::IsWindow(pMenuBar->m_pFrameWnd->m_hWnd));
	ASSERT_KINDOF(CMDIFrameWnd,pMenuBar->m_pFrameWnd);

    BOOL bMaximize=FALSE;
	CMDIChildWnd* pChild=((CMDIFrameWnd*)pMenuBar->m_pFrameWnd)->MDIGetActive(&bMaximize);
	if(pChild==NULL || !bMaximize)
		return;

	ASSERT(!pMenuBar->m_rectCloseBtn.IsRectEmpty());
	if(pMenuBar->m_rectCloseBtn.IsRectEmpty())
		return;

	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;

	// close button
	pSkin2003->DrawFrameButton(pDC, pMenuBar->m_rectCloseBtn, DFCS_CAPTIONCLOSE,
		FALSE, FALSE, ::GetSysColor(COLOR_BTNFACE), pMenuBar);

	// Max/restore button
	if (!pMenuBar->m_rectRestoreBtn.IsRectEmpty())
		pSkin2003->DrawFrameButton(pDC, pMenuBar->m_rectRestoreBtn, DFCS_CAPTIONRESTORE,
			FALSE, FALSE, ::GetSysColor(COLOR_BTNFACE), pMenuBar);

	// Minimize button
	if (!pMenuBar->m_rectMinimizeBtn.IsRectEmpty())
		pSkin2003->DrawFrameButton(pDC, pMenuBar->m_rectMinimizeBtn, DFCS_CAPTIONMIN,
			FALSE, FALSE, ::GetSysColor(COLOR_BTNFACE), pMenuBar);
}

// Draws a raised background with a shadow
void COXToolbarSkin2003::DrawDroppedItemBackground(CDC* /*pDC*/, LPCRECT /*lpRect*/)
{

}

void COXToolbarSkin2003::DrawItem(CDC* pDC, TBBUTTON* pTBB, LPCRECT lpRectItem, COXCoolToolBar* pCoolToolbar, BOOL bDrawBackground)
{
	// Determine the state flags
	bool bSelected = ((pTBB->fsState & TBSTATE_PRESSED) != 0);
	bool bHot = (pCoolToolbar->GetToolBarCtrl().GetHotItem() == pCoolToolbar->CommandToIndex(pTBB->idCommand));
	bool bDisabled = ((pTBB->fsState & TBSTATE_ENABLED) == 0);
	bool bChecked = ((pTBB->fsState & TBSTATE_CHECKED) != 0);

	DWORD dwStyleEx = pCoolToolbar->GetStyleEx();
	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;

	if (pCoolToolbar->m_iDropDownIndex == pTBB->idCommand)
	{
		// We have a droped drop down button
		pCoolToolbar->m_iDropDownIndex = -1;

		if (bDrawBackground)
			DrawDroppedItemBackground(pDC, lpRectItem);

		DrawItemIcon(pDC, pTBB, dwStyleEx, lpRectItem, false, true, false, pCoolToolbar);
		DrawItemText(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetTextColor(), pCoolToolbar);
	}
	else if (bSelected && bHot)
	{
		// Selected and hot
		if (bDrawBackground)
			DrawGradientItemBackground(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetActiveBorderColor(),
				pSkin2003->GetHotSelectedToolbarItemLightColor(),
				pSkin2003->GetHotSelectedToolbarItemDarkColor());
		
		DrawItemIcon(pDC, pTBB, dwStyleEx, lpRectItem, false, true, false, pCoolToolbar);
		DrawItemText(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetSelectedTextColor(), pCoolToolbar);
	}
	else if (bChecked && bHot)
	{
		// Checked and hot - same as selected and hot
		if (bDrawBackground)
			DrawGradientItemBackground(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetActiveBorderColor(),
				pSkin2003->GetHotSelectedToolbarItemLightColor(),
				pSkin2003->GetHotSelectedToolbarItemDarkColor());
		
		DrawItemIcon(pDC, pTBB, dwStyleEx, lpRectItem, false, true, false, pCoolToolbar);
		DrawItemText(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetSelectedTextColor(), pCoolToolbar);
	}
	else if (bChecked)
	{
		// Just chequed
		if (bDrawBackground)
			DrawGradientItemBackground(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetActiveBorderColor(),
				pSkin2003->GetCheckedToolbarItemLightColor(),
				pSkin2003->GetCheckedToolbarItemDarkColor());
		
		DrawItemIcon(pDC, pTBB, dwStyleEx, lpRectItem, false, false, false, pCoolToolbar);
		DrawItemText(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetTextColor(), pCoolToolbar);
	}
	else if (bHot)
	{
		// Just hot
		if (bDrawBackground)
			DrawGradientItemBackground(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetActiveBorderColor(),
				pSkin2003->GetHotToolbarItemLightColor(),
				pSkin2003->GetHotToolbarItemDarkColor());

		DrawItemIcon(pDC, pTBB, dwStyleEx, lpRectItem, false, true, false, pCoolToolbar);
		DrawItemText(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetTextColor(), pCoolToolbar);
	}
	else if (bSelected)
	{
		// This can only be a menu item, so decrease the height
		if (bDrawBackground)
		{
			CRect rectItem(lpRectItem);
			rectItem.DeflateRect(0, 1);
			DrawDroppedItemBackground(pDC, rectItem);
		}

		DrawItemIcon(pDC, pTBB, dwStyleEx, lpRectItem, false, true, false, pCoolToolbar);
		DrawItemText(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetTextColor(), pCoolToolbar);
	}
	else if (bDisabled)
	{
		// Disabled
		DrawItemIcon(pDC, pTBB, dwStyleEx, lpRectItem, false, false, true, pCoolToolbar);
		DrawItemText(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetDisabledTextColor(), pCoolToolbar);
	}
	else
	{
		// Normal
		DrawItemIcon(pDC, pTBB, dwStyleEx, lpRectItem, false, false, false, pCoolToolbar);
		DrawItemText(pDC, pTBB, dwStyleEx, lpRectItem, m_pSkinXP->GetTextColor(), pCoolToolbar);		
	}
}

void COXToolbarSkin2003::DrawGradientItemBackground(CDC* pDC, TBBUTTON* pTBB, DWORD dwStyleEx, LPCRECT lpRect, COLORREF crBorder, COLORREF crInsideLight, COLORREF crInsideDark)
{
	// Fill the inside
	CRect rect(lpRect);
	rect.DeflateRect(1, 1);

	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;
	pSkin2003->GradientFill(pDC, rect, crInsideLight, crInsideDark, GRADIENT_FILL_RECT_V);
	rect.InflateRect(1, 1);

	if (pTBB->iBitmap == -1 && (pTBB->fsStyle & TBSTYLE_DROPDOWN) && !(dwStyleEx & TBSTYLE_EX_DRAWDDARROWS))
	{
		// This is a menu item, so decrease the height
		rect.DeflateRect(0, 1);
	}

	// Draw the borders
	CBrush brush;
	brush.CreateSolidBrush(crBorder);
	pDC->FrameRect(rect, &brush);

	// If this is drop arrow button, frame the arrow part
	if ((pTBB->fsStyle & TBSTYLE_DROPDOWN) && (dwStyleEx & TBSTYLE_EX_DRAWDDARROWS))
		pDC->FrameRect(GetDropArrowRect(pDC, rect), &brush);
}

void COXToolbarSkin2003::DrawComboBox(COXSkinnedComboBox* pComboBox)
{
	CClientDC dc(pComboBox);
	COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;

	CRect rect;
	pComboBox->GetClientRect(rect);
	POINT ptCursor;
	::GetCursorPos(&ptCursor);
	pComboBox->ScreenToClient(&ptCursor);

	// Determine and exclude the combo box rectangle
	CRect rectCombo(rect);
	rectCombo.DeflateRect(3, 3, rect.Height() - 2, 3);

	dc.ExcludeClipRect(rectCombo);
	rectCombo.InflateRect(1, 1);
	if (pComboBox->m_bCommonControls6)
		rectCombo.right += 3;

	// Determine the button rectangle
	CRect rectButton(rect);
	rectButton.DeflateRect(2, 2, 3, 2);
	rectButton.left = rectCombo.Width() + 2;

	// Fill the background - must match the toolbar background
	{
		COXCoolToolBar* pToolbar = (COXCoolToolBar*) pComboBox->GetParent();

		CRect rectToolbar;
		pToolbar->GetWindowRect(rectToolbar);

		// Get the combo box rectangle
		CRect rectCombo;
		pComboBox->GetWindowRect(rectCombo);
		rectCombo.OffsetRect(-rectToolbar.left, -rectToolbar.top);

		CRect rectGradient(rect);
		rectGradient.top = -rectCombo.top;
		rectGradient.bottom = rectGradient.top + rectToolbar.Height();

		// Limit the drawing to the combo box
		dc.IntersectClipRect(rect);

		pSkin2003->GradientFill(&dc, rectGradient,
			pSkin2003->GetTabBackgroundLightColor(),
			pSkin2003->GetTabBackgroundDarkColor(),
			GRADIENT_FILL_RECT_V);
	}

	// Draw the inner white border
	CBrush brushWhite;
	COLORREF clrWhite = ::GetSysColor(COLOR_WINDOW);
	brushWhite.CreateSolidBrush(clrWhite);
	rectCombo.InflateRect(1, 1, 0, 0);
	dc.FillSolidRect(rectCombo, clrWhite);

	rectButton.InflateRect(1, 1, 0, 0);
	dc.FrameRect(rectButton, &brushWhite);

	// The combo box can have three states - normal, hot and dropped
	// Determine the state of the combo box
	BOOL bHotOrSelected = FALSE;
	COLORREF clrLightColor = 0, clrDarkColor = 0;
	COLORREF clrArrow = ::GetSysColor(COLOR_BTNTEXT);
	if (pComboBox->GetDroppedState())
	{
		// The combo box is dropped
		bHotOrSelected = TRUE;
		clrLightColor = pSkin2003->GetHotSelectedToolbarItemLightColor();
		clrDarkColor = pSkin2003->GetHotSelectedToolbarItemDarkColor();
		clrArrow = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	}
	else if (rect.PtInRect(ptCursor) || CWnd::GetFocus() == pComboBox ||
		(CWnd::GetFocus() && CWnd::GetFocus()->GetParent() == pComboBox))
	{
		// The combo box is hot
		bHotOrSelected = TRUE;
		clrLightColor = pSkin2003->GetHotToolbarItemLightColor();
		clrDarkColor = pSkin2003->GetHotToolbarItemDarkColor();
	}

	// Perform the drawing of a droped or hot combo box
	if (bHotOrSelected)
	{
		rectCombo.InflateRect(1, 1, 0, 1);
		rectButton.InflateRect(0, 1, 1, 1);

		// Draw the button
		CPen penBorder;
		penBorder.CreatePen(PS_SOLID, 1, m_pSkinXP->GetActiveBorderColor());
		CPen* pOldPen = dc.SelectObject(&penBorder);
		dc.Rectangle(rectButton);

		CRect rectGradient(rectButton);
		rectGradient.DeflateRect(1, 1);
		pSkin2003->GradientFill(&dc, rectGradient, clrLightColor, clrDarkColor,
			GRADIENT_FILL_RECT_V);

		dc.Rectangle(rectCombo);
		dc.SelectObject(pOldPen);
	}
	
	// Draw the drop arrow
	CPoint ptCenter = rectButton.CenterPoint();
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, clrArrow);
	CPen* pOldPen = dc.SelectObject(&pen);
	dc.MoveTo(ptCenter.x - 2, ptCenter.y - 1);
	dc.LineTo(ptCenter.x + 3, ptCenter.y - 1);
	dc.MoveTo(ptCenter.x - 1, ptCenter.y);
	dc.LineTo(ptCenter.x + 2, ptCenter.y);
	dc.SetPixelV(ptCenter.x, ptCenter.y + 1, clrArrow);
	dc.SelectObject(pOldPen);
}

void COXToolbarSkin2003::DrawWndFrame(CWnd* pWnd)
{
	CWindowDC dc(pWnd);

	CRect rect;
	pWnd->GetWindowRect(rect);
	CRect rectWindow(rect);
	pWnd->ScreenToClient(&rect);
	CPoint ptOffset(-rect.left, -rect.top);
	rect.OffsetRect(ptOffset);

	// Exclude the client rectangle
	CRect rectClient;
	pWnd->GetClientRect(rectClient);
	rectClient.OffsetRect(ptOffset);
	dc.ExcludeClipRect(rectClient);

	// Fill the background
	// Fill the background - must match the toolbar background
	{
		COXCoolToolBar* pToolbar = (COXCoolToolBar*) pWnd->GetParent();

		CRect rectToolbar;
		pToolbar->GetWindowRect(rectToolbar);

		// Get the combo box rectangle
		CRect rectCombo;
		pWnd->GetWindowRect(rectCombo);
		rectCombo.OffsetRect(-rectToolbar.left, -rectToolbar.top);

		CRect rectGradient(rect);
		rectGradient.top = -rectCombo.top;
		rectGradient.bottom = rectGradient.top + rectToolbar.Height();

		COXSkin2003* pSkin2003 = (COXSkin2003*) m_pSkinXP;
		if (::GetVersion() >= 0x80000000)
			// Adoid a problem with the ::GradientFill() API in Win98 and ME
			pSkin2003->GradientFillManual(&dc, rectGradient,
				pSkin2003->GetTabBackgroundLightColor(),
				pSkin2003->GetTabBackgroundDarkColor(),
				GRADIENT_FILL_RECT_V);
		else
			pSkin2003->GradientFill(&dc, rectGradient,
				pSkin2003->GetTabBackgroundLightColor(),
				pSkin2003->GetTabBackgroundDarkColor(),
				GRADIENT_FILL_RECT_V);
	}

	// Draw the inner white border
	CBrush brushWhite;
	COLORREF clrWhite = ::GetSysColor(COLOR_WINDOW);
	brushWhite.CreateSolidBrush(clrWhite);
	rect.DeflateRect(1, 1);
	dc.FillSolidRect(rect, clrWhite);

	// Determine the state of the edit box
	CPoint ptCursor;
	::GetCursorPos(&ptCursor);
	if (rectWindow.PtInRect(ptCursor) || CWnd::GetFocus() == pWnd)
	{
		// The combo box is hot - draw the border
		rect.InflateRect(1, 1);
		CBrush brush;
		brush.CreateSolidBrush(m_pSkinXP->GetActiveBorderColor());
		dc.FrameRect(rect, &brush);
	}
}

// COXMiniFrameSkin
//



// COXMiniFrameSkinClassic
//

BOOL COXMiniFrameSkinClassic::OnNcActivate(BOOL bActive, COXSizableMiniDockFrameWnd* pMiniDockFrameWnd)
{
	// CMiniDockFrameWnd class assumes if there is only 1 bar, 
	// then it's at position 1 in the array
	CControlBar* pBar=((COXSizeDockBar *)&pMiniDockFrameWnd->m_wndDockBar)->GetFirstControlBar();
	if(pBar!=NULL)
	{
		ASSERT_KINDOF(CControlBar, pBar);
		ASSERT(pBar->m_pDockContext != NULL);
		if(pBar->IsKindOf(RUNTIME_CLASS(COXSizeViewBar)))
		{
			COXMDIChildWndSizeDock* pMDIChild=((COXSizeViewBar*)pBar)->m_pChildWnd;
			if(::IsWindow(pMDIChild->m_hWnd))
			{
				if(pMDIChild->MDIGetActive()!=pMDIChild && bActive)
					return FALSE;
				else
					return (BOOL)pMiniDockFrameWnd->Default();
			}
		}
	}

	return ((CMiniDockFrameWnd*) pMiniDockFrameWnd)->OnNcActivate(bActive);
}

void COXMiniFrameSkinClassic::OnNcPaint(COXSizableMiniDockFrameWnd* pMiniDockFrameWnd)
{
	pMiniDockFrameWnd->Default();
}


// COXMiniFrameSkinXP
//

COXMiniFrameSkinXP::COXMiniFrameSkinXP(COXSkinXP* pSkinXP)
{
	m_pSkinXP = pSkinXP;
	m_fontCaption.CreatePointFont(85, _T("Tahoma BOLD"));
}

BOOL COXMiniFrameSkinXP::OnNcActivate(BOOL /*bActive*/, COXSizableMiniDockFrameWnd* /*pMiniDockFrameWnd*/)
{
	return TRUE;
}

// Draw the Office XP style caption of the mini frame window
void COXMiniFrameSkinXP::OnNcPaint(COXSizableMiniDockFrameWnd* pMiniDockFrameWnd)
{
	// Get the window rectangle and the DC
	CWindowDC dc(pMiniDockFrameWnd);
	CRect rect(0,0,0,0);
	pMiniDockFrameWnd->GetWindowRect(rect);
	pMiniDockFrameWnd->ScreenToClient(rect);
	CPoint ptOffset = rect.TopLeft();
	rect.OffsetRect(-ptOffset.x, -ptOffset.y);

	// Excluse the client rectangle from the clipping region
	CRect rectClient;
	pMiniDockFrameWnd->GetClientRect(rectClient);
	rectClient.OffsetRect(-ptOffset.x, -ptOffset.y);
	dc.ExcludeClipRect(rectClient);

	// Fill the outer dark rectangle
	COLORREF clrOuter = m_pSkinXP->GetMiniFrameBorderColor();
	dc.FillSolidRect(rect, clrOuter);
	
	// Fill the inner light rectangle
	rect.DeflateRect(2, 2);
	dc.FillSolidRect(rect, m_pSkinXP->GetMiniFrameLightBorderColor());

	// Draw the 4 corner points
	dc.SetPixelV(rect.TopLeft(), clrOuter);
	dc.SetPixelV(rect.right - 1, rect.top, clrOuter);
	dc.SetPixelV(rect.right - 1, rect.bottom - 1, clrOuter);
	dc.SetPixelV(rect.left, rect.bottom - 1, clrOuter);

	DrawCaption(pMiniDockFrameWnd);
}

void COXMiniFrameSkinXP::DrawCaption(COXSizableMiniDockFrameWnd* pMiniDockFrameWnd)
{
	// Get the window rectangle and the DC
	CWindowDC dc(pMiniDockFrameWnd);
	CRect rectCaption = GetCaptionRect(pMiniDockFrameWnd);

	// Draw the backbround of the caption
	dc.FillSolidRect(rectCaption, m_pSkinXP->GetMiniFrameCaptionColor());

	// Get the caption text
	CString strCaption;
	pMiniDockFrameWnd->GetWindowText(strCaption);

	// Draw the text
	CFont* pOldFont = dc.SelectObject(&m_fontCaption);
	COLORREF clrOld = dc.SetTextColor(m_pSkinXP->GetMiniFrameCaptionTextColor());
	rectCaption.DeflateRect(3, 0);
	dc.DrawText(strCaption, rectCaption, DT_SINGLELINE | DT_VCENTER);
	dc.SetTextColor(clrOld);
	dc.SelectObject(pOldFont);

	DrawCaptionButtons(pMiniDockFrameWnd);
}

// Returns the rectangle of the caption bar
CRect COXMiniFrameSkinXP::GetCaptionRect(COXSizableMiniDockFrameWnd* pMiniDockFrameWnd)
{
	CRect rect(0,0,0,0);
	pMiniDockFrameWnd->GetWindowRect(rect);
	pMiniDockFrameWnd->ScreenToClient(rect);
	CPoint ptOffset = rect.TopLeft();
	rect.OffsetRect(-ptOffset.x, -ptOffset.y);

	// Get the caption rectangle
	rect.DeflateRect(2, 2);
	CRect rectCaption;
	rectCaption.left = rect.left + 1;
	rectCaption.top = rect.top + 1;
	rectCaption.right = rect.right - 1;
	rectCaption.bottom = rectCaption.top - ptOffset.y - 5;

	return rectCaption;
}

// Draws the maximize button in Office XP style
BOOL COXMiniFrameSkinXP::DrawCaptionButtons(COXSizableMiniDockFrameWnd* pMiniDockFrameWnd)
{
	if (pMiniDockFrameWnd->GetSystemMenu(FALSE)->GetMenuItemCount() == 0)
		return TRUE; // do not draw if the button is disabled

	// Determine the button rectangle
	CRect rect = GetCaptionRect(pMiniDockFrameWnd);
	rect.left = rect.right - rect.Height();

	// Get the cursor position in window coordinates
	POINT ptCursor;
	::GetCursorPos(&ptCursor);
	pMiniDockFrameWnd->ScreenToClient(&ptCursor);
	ptCursor.x += 4;
	ptCursor.y += rect.Height() + 4;
	
	// Determine the state of the button
	bool bLeftButtonDown = (::GetKeyState(VK_LBUTTON) < 0);
	CRect rectTest(rect);
	rectTest.DeflateRect(0, 0, 1, 1);
	BOOL bMouseOnClose = rectTest.PtInRect(ptCursor);

	// Determine the colors
	COLORREF clrX;
	CBrush brush;
	brush.m_hObject = NULL;
	if (bMouseOnClose && bLeftButtonDown)
	{
		// Pressed
		clrX = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
		brush.CreateSolidBrush(m_pSkinXP->GetHotSelectedItemColor());
	}
	else if (bMouseOnClose)
	{
		// Hot
		clrX = ::GetSysColor(COLOR_BTNTEXT);
		brush.CreateSolidBrush(m_pSkinXP->GetHotItemColor());
	}
	else
	{
		// Normal - just the X
		clrX = m_pSkinXP->GetMiniFrameCaptionTextColor();
	}

	// Draw to a memory dc to avoid flashing
	CWindowDC dcWindow(pMiniDockFrameWnd);
	CDC dc;
	dc.CreateCompatibleDC(&dcWindow);
	CBitmap bmpMem;
	CRect rectButton(rect);
	rectButton.OffsetRect(-rect.left, -rect.top);
	bmpMem.CreateCompatibleBitmap(&dcWindow, rectButton.Width(), rectButton.Height());
	CBitmap* pOldBmp = dc.SelectObject(&bmpMem);

	// Perform the drawing
	if (brush.m_hObject != NULL)
	{
		// Draw a frame
		CBrush* pOldBrush = dc.SelectObject(&brush);
		CPen penBorder;
		penBorder.CreatePen(PS_SOLID, 1, m_pSkinXP->GetActiveBorderColor());
		CPen* pOldPen = dc.SelectObject(&penBorder);
		dc.Rectangle(rectButton);
		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);
	}
	else
		dc.FillSolidRect(rectButton, m_pSkinXP->GetMiniFrameCaptionColor());

	// Draw the "x" inside the button
	CRect rectX(rectButton);
	rectX.DeflateRect(4, 4, 5, 5);
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, clrX);
	CPen* pOldPen = dc.SelectObject(&pen);
	for (int i = 0; i < 2; i++)
	{
		dc.MoveTo(rectX.left, rectX.top);
		dc.LineTo(rectX.right + 1, rectX.bottom + 1);
		dc.MoveTo(rectX.right, rectX.top);
		dc.LineTo(rectX.left - 1, rectX.bottom + 1);
		rectX.OffsetRect(1, 0);
	}

	dc.SelectObject(pOldPen);

	// Copy the memory dc to the window dc
	dcWindow.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);
	dc.SelectObject(pOldBmp);

	return TRUE;
}

// Floating windows are not always children of the main frame, so we need to iterate
// through all top level windows.
void COXSkinnedApp::UpdateFloatingWindows(CWnd* pMainWnd)
{
	HWND hMainWnd = pMainWnd->m_hWnd;

	// Walk through all top-level windows
	HWND hWnd = ::GetWindow(::GetDesktopWindow(), GW_CHILD);
	while (hWnd != NULL)
	{
		CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);
		if (pWnd != NULL && hMainWnd != hWnd && AfxIsDescendant(hMainWnd, hWnd))
			if (pWnd->IsKindOf(RUNTIME_CLASS(COXSizableMiniDockFrameWnd)))
				UpdateChildrenRec(pWnd);

		hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
	}
}
