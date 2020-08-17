// ==========================================================================
// 							Class Implementation : COXStatusBar
// ==========================================================================

// Implementation file : status.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                         
// //////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "xstatus4.h"
#include "OXSkins.h"

#include <windowsx.h>
#include <malloc.h>

#include "UTBStrOp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[]=__FILE__;
#endif

#define CX_BORDER   1
#define CY_BORDER   1

#define SBPF_UPDATE 0x0001  // pending update of text
#define CX_PANE_BORDER 6    // 3 pixels on each side of each pane


IMPLEMENT_DYNAMIC(COXStatusBar, CStatusBar)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// Data members -------------------------------------------------------------
// protected:
	//  CObArray	m_PaneBmp;
	// --- Each item represents a pane. When there is
	// 	   a pointer stored in it, that pane contains a bitmap
    // 	   Each item represents a pane. When there is         
	//     a WORD stored in it, that pane contains a COLOR

    //  CDWordArray m_ColorArray;
	// --- An array containing possible textcolors when drawing a pane with Text in it.
	
	//  CPtrArray  	m_EvolArray;
	// --- An array containing possible Objects that configure each a Progress bar in a pane.

	//  CDC m_srcDC;
	// --- A memory DC used to bitblt the bitmaps from.

// Member functions ---------------------------------------------------------
// public:

COXStatusBar::COXStatusBar() :
	m_pStatusbarSkin(NULL)
	{
	}

COXStatusBar::~COXStatusBar()
	{     
	// free all bitmaps and their container objects
	int i(0);
	CBmpInfo* pBmpInfo=NULL;
	while(i < m_PaneBmp.GetSize())
		{
		pBmpInfo=(CBmpInfo*)m_PaneBmp[i];
		if(pBmpInfo != NULL)
			{
			pBmpInfo->m_pBitmap->DeleteObject();
			delete pBmpInfo->m_pBitmap;			
			delete pBmpInfo;
			}
		i++;
		}

	// free all pane fonts objects
	i=0;
	CFont* pFont=NULL;
	while(i < m_PaneFont.GetSize())
		{
		pFont=(CFont*)m_PaneFont[i];
		if(pFont != NULL)
			{
			pFont->DeleteObject();
			delete pFont;
			}
		i++;
		}

	// free all progress container objects
	i=0;
	CEvolInfo* pEvolInfo=NULL;
	while(i < m_EvolArray.GetSize())
		{
		pEvolInfo=(CEvolInfo*)m_EvolArray[i];
		if(pEvolInfo != NULL)
			{
			delete pEvolInfo;
			}
		i++;
		}

	// if necessary delete the classic skin
	if (m_pStatusbarSkin != NULL)
		delete m_pStatusbarSkin;

	// Workarround for destructor of CStatusBar where the text of a pane is not
	// freed correctly everytime.
	if(m_nCount>0)
		{
		AFX_STATUSPANE* pSBP=_GetPanePtr(0);
		for (i=0; i < m_nCount; i++)
			{
			pSBP->strText.Empty();
			pSBP->strText.FreeExtra();
			++pSBP;
			}
		}
	}

BOOL COXStatusBar::SetPaneCursor(int nIndex, UINT nCursorID /*=0 */)
	{
	// Check whether we are reseting the cursor
	if (nCursorID==0)
		{
		m_CursorArray[nIndex]=(DWORD)NULL;
		return TRUE;
		}

	// The cursor is read from resource now
	HCURSOR hNewCursor=AfxGetApp()->LoadCursor(nCursorID);
	if (hNewCursor != NULL)
		{
		m_CursorArray[nIndex]=(DWORD)(INT_PTR)hNewCursor;
		}
	else
		{
		TRACE1("COXStatusBar::SetPaneCursor : Failed to load cursor with ID %i\n",
			nCursorID);
		return FALSE;
		}

	return TRUE;
	}

BOOL COXStatusBar::SetPaneFont(int nIndex, CFont* pFont /*=NULL */)
{
	CFont* pPaneFont=(CFont*)m_PaneFont[nIndex];
	if(pPaneFont != NULL)
		pPaneFont->DeleteObject();

	// to restore the default font
	if (pFont==NULL)
	{
		// if pane font==NULL there was no special font selected, so just return
		if (pPaneFont != NULL)
		{
			if (m_ColorArray[nIndex]==::GetSysColor(COLOR_BTNTEXT))
			{
				UINT nID, nStyle;
				int cxWidth;
				GetPaneInfo(nIndex, nID, nStyle, cxWidth);
				nStyle &= ~SBT_OWNERDRAW;
				SetPaneInfo(nIndex, nID, nStyle, cxWidth);
			}

			delete pPaneFont;
			m_PaneFont[nIndex]=NULL;
		}

		return TRUE;
	}

	// Create a new font object for this pane
	pPaneFont=new CFont;
	// Assign this font to the pane
	m_PaneFont[nIndex]=pPaneFont;
	LOGFONT logFont;                                // Logical font struct
	pFont->GetObject(sizeof(LOGFONT), &logFont);
	if (!pPaneFont->CreateFontIndirect(&logFont))
	{
		TRACE(_T("In COXStatusBar::SetPaneFont : Failed to create the font for pane %d\n"), nIndex);
		delete pPaneFont;
		m_PaneFont[nIndex]=NULL;
		return FALSE;
	}

	UINT nID, nStyle;
	int cxWidth;
	GetPaneInfo(nIndex, nID, nStyle, cxWidth);
	nStyle |= SBT_OWNERDRAW;
	SetPaneInfo(nIndex, nID, nStyle, cxWidth);

	return TRUE;
}

BOOL COXStatusBar::SetPaneText(int nIndex, LPCTSTR lpszNewText,
	COLORREF clrTextColor /*=::GetSysColor(COLOR_BTNTEXT) */, BOOL bUpdate /*=TRUE */)
{
	BOOL bSuccess=CStatusBar::SetPaneText(nIndex, lpszNewText, bUpdate);
	if (!bSuccess)
		return FALSE;

	// changing the textcolor requires an ownerdrawn style
	if (clrTextColor != m_ColorArray[nIndex] && m_PaneFont[nIndex]==NULL)
	{
		UINT nID, nStyle;
		int cxWidth;
		GetPaneInfo(nIndex, nID, nStyle, cxWidth);
		// if the color is changed back to the default stausbar text color,
		// remove the OWNERDRAWN style
		if (clrTextColor != ::GetSysColor(COLOR_BTNTEXT))
			nStyle |= SBT_OWNERDRAW;
		else
			nStyle &= ~SBT_OWNERDRAW;

		m_ColorArray[nIndex]=clrTextColor;

		SetPaneInfo(nIndex, nID, nStyle, cxWidth);
		}
	else
		m_ColorArray[nIndex]=clrTextColor;

	return TRUE;
	}

void COXStatusBar::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
	ASSERT(TRUE);
	
	// You should get the pointer to the device context from the "lpDrawItemStruct" ptr.
	CDC* pDC=CDC::FromHandle(lpDrawItemStruct->hDC);

	int nOldBkMode=pDC->SetBkMode(TRANSPARENT);
	CFont* pPaneFont=(CFont*)m_PaneFont[lpDrawItemStruct->itemID];
	CFont* pOldFont=NULL;

	if (pPaneFont == NULL)
	{
		pPaneFont = GetFont();
	}

	if (pPaneFont != NULL)
	{
		pOldFont=pDC->SelectObject(pPaneFont);
		ASSERT(pOldFont != NULL);
	}

	COLORREF clrOldTextColor=
		pDC->SetTextColor(m_ColorArray[lpDrawItemStruct->itemID]);

	CString PaneText=GetPaneText(lpDrawItemStruct->itemID);
	CSize sStrSize=pDC->GetTextExtent(PaneText);
	pDC->TextOut(lpDrawItemStruct->rcItem.left + 2, lpDrawItemStruct->rcItem.top +
		((lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top - sStrSize.cy) / 2), PaneText);
	pDC->SetTextColor(clrOldTextColor);
	if (pOldFont != NULL)
		pDC->SelectObject(pOldFont);

	pDC->SetBkMode(nOldBkMode);
	}

void COXStatusBar::OnPaint()
{
	GetStatusbarSkin()->OnPaintStatusbar( this );
}

AFX_STATUSPANE* COXStatusBar::_GetPanePtr(int nIndex) const
	{
	ASSERT(nIndex >= 0 && nIndex < m_nCount);
	ASSERT(m_pData != NULL);
	return ((AFX_STATUSPANE*)m_pData) + nIndex;
	}

BOOL COXStatusBar::SetIndicators(const UINT FAR* lpIDArray, int nIDCount)
	{
	// Init the Bitmap array
	int i=0;
	for(i=0; i < nIDCount;i++)
		{
		m_PaneBmp.SetAtGrow(i, NULL);
		}
		
	// Init the Bitmap array
	for(i=0; i < nIDCount;i++)
		{
		m_PaneFont.SetAtGrow(i, NULL);
		}

	// Init the Color array
	for(int j=0; j < nIDCount;j++)
		{
		m_ColorArray.SetAtGrow(j, ::GetSysColor(COLOR_BTNTEXT));
		}
		
	// Init the Progress array
	for(int k=0; k < nIDCount;k++)
		{
		m_EvolArray.SetAtGrow(k, NULL);
		}

	// Init the ToolTip array
	m_TipArray.SetSize(nIDCount);

	// Init the Cursor array
	for(i=0; i < nIDCount;i++)
		{
		m_CursorArray.SetAtGrow(i, NULL);
		}

	return CStatusBar::SetIndicators(lpIDArray, nIDCount);
	}  


BOOL COXStatusBar::SetPaneBitmap(int nIndex, UINT nIDResource , 
								 EOrientation eBMPOrient /*=EO_CenterFit */,
								 COLORREF clrMask /*=RGB(255,255,255) */, 
								 BOOL bUpdate /*=TRUE */)
	{
	CRect rect;
	BOOL bSucces=TRUE;
	
	ASSERT((EO_FIRST <= (int)eBMPOrient) && ((int) eBMPOrient <= EO_LAST));
	CBmpInfo* pBmpInfo=(CBmpInfo*)m_PaneBmp[nIndex];

	// First Check whether we want to remove the bitmap from this pane
	if(nIDResource==0)
		{
		if(pBmpInfo != NULL)
			{
			m_PaneBmp[nIndex]=NULL;
			pBmpInfo->m_pBitmap->DeleteObject();
			delete pBmpInfo->m_pBitmap;
			delete pBmpInfo;
			}

		}
	else
		{
		if(pBmpInfo==NULL)
			// The pane we're referring to, has no bitmap assigned to it yet.
			{
			// The CBmpInfo is the class where all bitmap info will be stored
			pBmpInfo=new CBmpInfo;
			pBmpInfo->m_pBitmap=new CBitmap;
			if (!pBmpInfo->m_pBitmap->LoadBitmap(nIDResource))
				{
				delete pBmpInfo->m_pBitmap;
				delete pBmpInfo;
				pBmpInfo=NULL;
				//////////
				// if we set pBmpInfo to NULL we steel have some info 
				// in m_PaneBmp[nIndex]
				///////
				m_PaneBmp[nIndex]=NULL;
				bSucces=FALSE;
				TRACE0("COXStatusBar::SetPaneBitmap: Cannot load bitmap resource!");
				////////////////////////
				}
			//// 
			}
		else 
			{
			if (pBmpInfo->m_nIDResource != nIDResource)
				{
				pBmpInfo->m_pBitmap->DeleteObject();
				//////////
				// if we set pBmpInfo to NULL we steel have some info in m_PaneBmp[nIndex]
				///////
				if(!pBmpInfo->m_pBitmap->LoadBitmap(nIDResource))
					{
					delete pBmpInfo->m_pBitmap;
					delete pBmpInfo;
					pBmpInfo=NULL;
					m_PaneBmp[nIndex]=NULL;
					bSucces=FALSE;
					TRACE0("COXStatusBar::SetPaneBitmap: Cannot load bitmap resource!");
					// have to update pane
				    GetItemRect(nIndex, &rect);  // get pane rect
					InvalidateRect(rect, FALSE);
					}
				////////////////////////
				}
			}
			
		if(!bSucces)
    		return FALSE;
    
		// must exist and a pane with a bitmap does not have text
		ASSERT(pBmpInfo != NULL);
		CStatusBar::SetPaneText(nIndex, _T(""));

		// Set the background color
		pBmpInfo->m_clrMask=clrMask;
		// Store the resource ID of the loaded bitmap
		pBmpInfo->m_nIDResource=nIDResource;
		// Reset the size of the bitmap
		pBmpInfo->m_BmpSize.cx=0;
		pBmpInfo->m_BmpSize.cy=0;
    
		// Set bitmap Layout and determine true size of bitmap
		pBmpInfo->m_eBmpOrien=eBMPOrient;
		BITMAP bm;
		pBmpInfo->m_pBitmap->GetObject(sizeof(bm), &bm);
		pBmpInfo->m_BmpSize=CSize(bm.bmWidth, bm.bmHeight);

		// Assign this Bitmap Info to the pane
		m_PaneBmp[nIndex]=pBmpInfo;
		}


	// Adding, removing or changing a Bitmap can cause a pane to grow or shrink.
	// Could influence the stored rects for the evolution panes, so recalc them
	UpdateAllPanes(TRUE, FALSE);

	if(bUpdate)
	{
		// Changed because otherwise moving the orientation would not invalidate the old region
		Invalidate(TRUE);
//	    GetItemRect(nIndex, &rect);  // get pane rect
//		InvalidateRect(rect, FALSE);
	}
	
	return TRUE;
	}


void COXStatusBar::UpdateAllPanes(BOOL bUpdateRects, BOOL bUpdateText)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	// update the status pane locations
	if (bUpdateRects)
	{
		// get border information and client work area
		CRect rect; GetWindowRect(rect);
		rect.OffsetRect(-rect.left, -rect.top);
		CalcInsideRect(rect, TRUE);
		int rgBorders[3];
		VERIFY((BOOL)DefWindowProc(SB_GETBORDERS, 0, (LPARAM)&rgBorders));

		// determine extra space for stretchy pane
		int cxExtra=rect.Width() + rgBorders[2];
		int nStretchyCount=0;
		AFX_STATUSPANE* pSBP=_GetPanePtr(0);
		int i=0;
		for (i=0; i < m_nCount; i++)
		{
			if (pSBP->nStyle & SBPS_STRETCH)
				++nStretchyCount;
			cxExtra -= (pSBP->cxText+CX_PANE_BORDER + rgBorders[2]);
			++pSBP;
		}

		// determine right edge of each pane
		int* rgRights=(int*)_alloca(m_nCount * sizeof(int));
		int right=rgBorders[0];
		pSBP=_GetPanePtr(0);
		for (i=0; i < m_nCount; i++)
		{
			// determine size of the pane
			ASSERT(pSBP->cxText >= 0);

			right += pSBP->cxText+CX_PANE_BORDER;
			if ((pSBP->nStyle & SBPS_STRETCH) && cxExtra > 0)
			{
				ASSERT(nStretchyCount != 0);
				int cxAddExtra=cxExtra / nStretchyCount;
				right += cxAddExtra;
				--nStretchyCount;
				cxExtra -= cxAddExtra;
			}
			rgRights[i]=right;

			// next pane
			++pSBP;
			right += rgBorders[2];
		}

		// set new right edges for all panes
		DefWindowProc(SB_SETPARTS, m_nCount, (LPARAM)rgRights);

		// Move all ProgressWindow Ctrls
		pSBP=_GetPanePtr(0);
		CEvolInfo* pEvolInfo=NULL;

		for (i=0; i < m_nCount; i++)
		{
			pEvolInfo=(CEvolInfo*)m_EvolArray[i];
			if (pEvolInfo != NULL)
			{
				if (pSBP->nStyle & SBPS_PERCENT)
				{
					CDC* pDC=GetDC();
					CFont* pPaneFont=(CFont*)m_PaneFont[i];
					CFont* pOldFont=NULL;
					if(pPaneFont!=NULL)
						pOldFont=pDC->SelectObject(pPaneFont);
					CRect rect(0, 0, 0, 0);
					pDC->DrawText(_T("999%"),rect,DT_CALCRECT|DT_SINGLELINE|DT_LEFT);
					if(pOldFont!=NULL)
						pDC->SelectObject(pOldFont);
					ReleaseDC(pDC);
		
					if (!pEvolInfo->m_bRectInitialized)
						pEvolInfo->m_bRectInitialized=CreateEvolutionPane(i,pEvolInfo);
					if (pEvolInfo->m_bRectInitialized && 
						CalcInsideBorder(i, pEvolInfo->m_ProgressRect, TRUE))
					{
						if (pEvolInfo->m_bPercentText)
							pEvolInfo->m_ProgressRect.left+=rect.Width(); 

						pEvolInfo->m_ProgressPane.SetWindowPos(NULL,
							pEvolInfo->m_ProgressRect.left, 
							pEvolInfo->m_ProgressRect.top,
							pEvolInfo->m_ProgressRect.Width(), 
							pEvolInfo->m_ProgressRect.Height(),
							SWP_NOZORDER|SWP_NOACTIVATE|SWP_SHOWWINDOW);
					}
				}
			}

			// next pane
			++pSBP;
		}

	}

	// update text in the status panes if specified
	if (bUpdateText)
	{
		AFX_STATUSPANE* pSBP=_GetPanePtr(0);
		for (int i=0; i < m_nCount; i++)
		{
			if (pSBP->nFlags & SBPF_UPDATE)
				CStatusBar::SetPaneText(i, pSBP->strText);

			++pSBP;
		}
		
	}
}

BOOL COXStatusBar::CalcInsideBorder(int nIndex, CRect& PaneRect, BOOL bEvolutionBar)
	// --- In  : nIndex : the pane specified
	// --- Out : PaneRect : will receive the coordinates of the space of a pane within its borders
	// --- Returns : succeeded or not
	// --- Effect : calculate the coordinates of the space of a pane within the borders of this pane
{
	int rgBorders[3];
	VERIFY((BOOL)DefWindowProc(SB_GETBORDERS, 0, (LPARAM)&rgBorders));

	GetItemRect(nIndex, PaneRect);  // get pane rect
	if (PaneRect.IsRectEmpty())
		return FALSE;

	// adjust the itemrect to exclude the borders
	PaneRect.left=PaneRect.left + rgBorders[2];
	PaneRect.top=PaneRect.top + rgBorders[0] + 1;
	PaneRect.right -= rgBorders[2];
	PaneRect.bottom -= rgBorders[0] + 1;
	if(bEvolutionBar)
	{
		PaneRect.bottom--;
	}

	return TRUE;
}

void COXStatusBar::SetBarProgress(int nIndex, BOOL bUpdate/*=TRUE*/, 
								  DWORD dwTotal/*=100*/, DWORD dwDone/*=0*/)
{
	// bUpdate not used anymore, but still there to provide compatibility
	// this line was added to avoid compiler warning
	UNREFERENCED_PARAMETER(bUpdate);

	ASSERT(dwDone <= dwTotal);

	CEvolInfo* pEvolInfo=(CEvolInfo*)m_EvolArray[nIndex];
	if (pEvolInfo==NULL)
	{
		TRACE(_T("The pane '%i'has never been SetUp as a progress pane\n"), nIndex);
		return; 
	}

	pEvolInfo->m_bRectInitialized=CreateEvolutionPane(nIndex, pEvolInfo);
	if (!pEvolInfo->m_bRectInitialized)
		 return;

	// Adjust the progresscontrol
#if _MFC_VER>0x0421
	pEvolInfo->m_ProgressPane.SetRange32(0,dwTotal);
#else
	pEvolInfo->m_ProgressPane.SetRange(0,dwTotal);
#endif
	pEvolInfo->m_ProgressPane.SetPos(dwDone);

	if (dwTotal > 0)
		pEvolInfo->m_nPercentDone=(unsigned short)((dwDone * 100) / dwTotal);
	else
		pEvolInfo->m_nPercentDone=0;
	
	if (pEvolInfo->m_bPercentText)
	{
		// Setting the text for this caused the entire pane to,be invalidated
		// Invalidate only the part designated for the text, so we avoid nasty 
		// screen flashes
		//
		SetRedraw(FALSE);
		// Draw the percentage text
		TCHAR tmpBuf[5];
		UTBStr::stprintf(tmpBuf, 5, _T("%d%%"),pEvolInfo->m_nPercentDone);
		CStatusBar::SetPaneText(nIndex, tmpBuf);

		SetRedraw(TRUE);

		CRect rect;
		GetItemRect(nIndex,rect);
		rect.right=pEvolInfo->m_ProgressRect.left-1;
		InvalidateRect(rect);
		//
		////////////////////////////////
	}
}

void COXStatusBar::ResetBar(int nIndex, BOOL bDestroy /*=FALSE */)
{
	if(m_EvolArray[nIndex] != NULL)
	{
		if (bDestroy)
		{
			CEvolInfo* pEvolInfo=(CEvolInfo*)m_EvolArray[nIndex];
			delete pEvolInfo;
			m_EvolArray[nIndex]=NULL;
			CStatusBar::SetPaneText(nIndex, _T(""));
		}

		//	Redraw the frame control under the gauge
		SetBarProgress(nIndex, TRUE, 100, 0);
	}
}

BOOL COXStatusBar::SetUpBar(int nIndex, BOOL bUpdate /*=TRUE */, 
							BOOL bPercentText /*=TRUE */)
{
	UNREFERENCED_PARAMETER(bUpdate);

	CEvolInfo*	pEvolutionInfo;
	if(m_EvolArray[nIndex] != NULL)
	{
		pEvolutionInfo=(CEvolInfo*)m_EvolArray[nIndex];
		delete pEvolutionInfo;
		m_EvolArray[nIndex]=NULL;
	}
	
	if(m_EvolArray[nIndex]==NULL)
		// a new progress ctrl has to be created
	{
		pEvolutionInfo=new CEvolInfo;
		m_EvolArray[nIndex]=pEvolutionInfo;
		pEvolutionInfo->m_nPercentDone=0;
		pEvolutionInfo->m_ProgressRect.SetRectEmpty();
		pEvolutionInfo->m_bPercentText=bPercentText;
		pEvolutionInfo->m_bRectInitialized=FALSE;

		pEvolutionInfo->m_bRectInitialized=CreateEvolutionPane(nIndex, pEvolutionInfo);
    }
	else 
		ResetBar(nIndex);

	return TRUE;
}

BOOL COXStatusBar::CreateEvolutionPane(int nIndex, CEvolInfo* pEvolInfo)
{
	if (pEvolInfo->m_bRectInitialized)
		return TRUE;

	// subtract the borders
	if (!CalcInsideBorder(nIndex, pEvolInfo->m_ProgressRect, TRUE))
		return FALSE;

	if (pEvolInfo->m_bPercentText)
	{
		CDC* pDC=GetDC();
		CFont* pPaneFont=(CFont*)m_PaneFont[nIndex];
		CFont* pOldFont=NULL;
		if(pPaneFont!=NULL)
			pOldFont=pDC->SelectObject(pPaneFont);
		CRect rect(0, 0, 0, 0);
		pDC->DrawText(_T("999%"),rect,DT_CALCRECT|DT_SINGLELINE|DT_LEFT);
		if(pOldFont!=NULL)
			pDC->SelectObject(pOldFont);
		pEvolInfo->m_ProgressRect.left+=rect.Width();

		ReleaseDC(pDC);
	}

	CStatusBar::SetPaneText(nIndex, _T(""));

	if (!pEvolInfo->m_ProgressPane.Create(WS_CHILD | WS_VISIBLE,
		pEvolInfo->m_ProgressRect, this, nIndex))
	{
		TRACE(_T("In COXStatusBar::SetUpBar : Could not create a progressctrl for pane %i\n"), nIndex);
		return FALSE;
	}

	//Remove the WS_EX_STATICEDGE extended style that's automatically
	//added by internal Win32 drawing code - Nish Feb 10, 2005
	pEvolInfo->m_ProgressPane.ModifyStyleEx(WS_EX_STATICEDGE, NULL, SWP_FRAMECHANGED);

	return TRUE;
}

BOOL COXStatusBar::ActivatePaneTips(BOOL bActivate)
{
	EnableToolTips(bActivate);
	
	return TRUE;
}

BOOL COXStatusBar::SetPaneTip(int nIndex, LPCTSTR lpszNewPaneTip)
{
	ASSERT(lpszNewPaneTip != NULL);

	//add the text to the tooltip array
	m_TipArray.SetAt(nIndex, lpszNewPaneTip);
	
	return TRUE;
}

BOOL COXStatusBar::DeletePaneTip(int nIndex)
{
	//remove the text out of the tooltip array
	m_TipArray.SetAt(nIndex, _T(""));
	
	return TRUE;
}

void COXStatusBar::SetPaneStyle(int nIndex, UINT nStyle)
{
	AFX_STATUSPANE* pSBP=_GetPanePtr(nIndex);
	if (pSBP->nStyle != nStyle)
	{
		pSBP->nFlags |= SBPF_UPDATE;
		// use SetPaneText, since it updates the style and text
		if ((pSBP->nStyle ^ nStyle) & SBPS_STRETCH)
		{
			pSBP->nStyle=nStyle;
			UpdateAllPanes(TRUE, /*FALSE*/TRUE);
		}
		else
		{
			pSBP->nStyle=nStyle;
//			pSBP->nFlags |= SBPF_UPDATE;
			CStatusBar::SetPaneText(nIndex, pSBP->strText);
		}
		RedrawWindow();
	}
}

void COXStatusBar::SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth)
{
	ASSERT_VALID(this);

	BOOL bChanged=FALSE;
	AFX_STATUSPANE* pSBP=_GetPanePtr(nIndex);
	pSBP->nID=nID;
	if (pSBP->nStyle != nStyle)
	{
		if ((pSBP->nStyle ^ nStyle) & SBPS_STRETCH)
			bChanged=TRUE;
		else
		{
			pSBP->nStyle=nStyle;
			pSBP->nFlags |= SBPF_UPDATE;
			CStatusBar::SetPaneText(nIndex, pSBP->strText);
		}
		pSBP->nStyle=nStyle;
	}
	
	if (cxWidth != pSBP->cxText)
	{
		// change width of one pane -> invalidate the entire status bar
		pSBP->cxText=cxWidth;
		bChanged=TRUE;
	}
	
	if (bChanged)
		UpdateAllPanes(TRUE, FALSE);
}

void PASCAL COXStatusBar::DrawStatusBmp(CDC* pDC,int nPane , UINT nStyle)
	// --- In  : pDC : the device context to draw on
	//			 rect : the client coordinates of the pane to draw
	//			 nPane : the index of the pane to draw
	//			 nStyle : the style of the pane
	// --- Out : 
	// --- Returns :
	// --- Effect : Draws a standard 3D pane with a Bitmap in it
	{
	ASSERT(pDC->m_hDC != NULL);
	CBmpInfo* pBMPInfo=(CBmpInfo*)m_PaneBmp[nPane];
	ASSERT(pBMPInfo != NULL);
	
    if(!(nStyle & SBPS_DISABLED))
    	{
		CRect rectPane, rcDestBMP;
		CSize szSourceBMP;

		CalcInsideBorder(nPane, rectPane);

		// Calculate origin of bitmap drawing within panerect
		int WidthDiff=rectPane.Width() - pBMPInfo->m_BmpSize.cx;
		int HeightDiff=rectPane.Height() - pBMPInfo->m_BmpSize.cy;
		CSize BMPDrawSize (WidthDiff > 0 ? pBMPInfo->m_BmpSize.cx : rectPane.Width(),
						   HeightDiff > 0 ? pBMPInfo->m_BmpSize.cy : rectPane.Height());

		CPoint BMPDrawOrigin=rectPane.TopLeft();
		BMPDrawOrigin.y=HeightDiff > 0 ? BMPDrawOrigin.y + HeightDiff / 2 : BMPDrawOrigin.y;
		rcDestBMP=CRect(BMPDrawOrigin, BMPDrawSize);
		szSourceBMP=pBMPInfo->m_BmpSize;

		switch(pBMPInfo->m_eBmpOrien)
			{
			case EO_Stretch:
				rcDestBMP=rectPane;
				break;
			case EO_CenterClip:
				szSourceBMP=rcDestBMP.Size();
			case EO_CenterFit:
				rcDestBMP.OffsetRect(WidthDiff > 0 ? WidthDiff / 2 : 0, 0);
				break;
			case EO_RightClip:
				szSourceBMP=rcDestBMP.Size();
			case EO_RightFit:
				rcDestBMP.OffsetRect(WidthDiff > 0 ? WidthDiff : 0, 0);
				break;
			case EO_LeftClip:
				szSourceBMP=rcDestBMP.Size();
			case EO_LeftFit:
				break;
			default:
				ASSERT(FALSE);
			}

		DrawTranspBitmap(pDC, pBMPInfo, rcDestBMP, szSourceBMP);
		}
		
	}

void COXStatusBar::DrawTranspBitmap(CDC* pDC, CBmpInfo* pInfo, CRect& rcDrawBMP, CSize& szSrcBMP)
	{
	ASSERT(pDC != NULL);
	ASSERT(pInfo != NULL);

	CBitmap    bmAndBack, bmAndObject, bmAndMem, bmSave;
	CDC        dcMem, dcBack, dcObject, dcTemp, dcSave;

	VERIFY(dcTemp.CreateCompatibleDC(pDC));
	dcTemp.SelectObject(pInfo->m_pBitmap);   // Select the bitmap

	CSize TempSize=pInfo->m_BmpSize;
	dcTemp.DPtoLP(&TempSize);     // Convert from device
		   						  // to logical points

	// Create some DCs to hold temporary data.
	VERIFY(dcBack.CreateCompatibleDC(pDC));
	VERIFY(dcObject.CreateCompatibleDC(pDC));
	VERIFY(dcMem.CreateCompatibleDC(pDC));
	VERIFY(dcSave.CreateCompatibleDC(pDC));

	// Create a bitmap for each DC. DCs are required for a number of
	// GDI functions.

	// Monochrome DC
	VERIFY(bmAndBack.CreateBitmap(TempSize.cx, TempSize.cy, 1, 1, NULL));

	// Monochrome DC
	VERIFY(bmAndObject.CreateBitmap(TempSize.cx, TempSize.cy, 1, 1, NULL));

	VERIFY(bmAndMem.CreateCompatibleBitmap(pDC, TempSize.cx, TempSize.cy));
	VERIFY(bmSave.CreateCompatibleBitmap(pDC, TempSize.cx, TempSize.cy));

	// Each DC must select a bitmap object to store pixel data.
	CBitmap* pbmBackOld  =dcBack.SelectObject(&bmAndBack);
	CBitmap* pbmObjectOld=dcObject.SelectObject(&bmAndObject);
	CBitmap* pbmMemOld   =dcMem.SelectObject(&bmAndMem);
	CBitmap* pbmSaveOld  =dcSave.SelectObject(&bmSave);

	// Set proper mapping mode.
	dcTemp.SetMapMode(pDC->GetMapMode());

	// Save the bitmap sent here, because it will be overwritten.
	dcSave.BitBlt(0, 0, TempSize.cx, TempSize.cy, &dcTemp, 0, 0, SRCCOPY);

	// Set the background color of the source DC to the color.
	// contained in the parts of the bitmap that should be transparent
	COLORREF clrOldBkColor=dcTemp.SetBkColor(pInfo->m_clrMask);

	// Create the object mask for the bitmap by performing a BitBlt
	// from the source bitmap to a monochrome bitmap.
	dcObject.BitBlt(0, 0, TempSize.cx, TempSize.cy, &dcTemp, 0, 0, SRCCOPY);

	// Set the background color of the source DC back to the original
	// color.
	dcTemp.SetBkColor(clrOldBkColor);

	// Create the inverse of the object mask.
	dcBack.BitBlt(0, 0, TempSize.cx, TempSize.cy, &dcObject, 0, 0, NOTSRCCOPY);

	// Copy the background of the main DC to the destination.
//	dcMem.BitBlt(0, 0, TempSize.cx, TempSize.cy, pDC, rcDrawBMP.left, rcDrawBMP.top-2, SRCCOPY);
	dcMem.StretchBlt(0, 0, TempSize.cx, TempSize.cy, pDC, 
		rcDrawBMP.left, rcDrawBMP.top, rcDrawBMP.Width(), rcDrawBMP.Height(), SRCCOPY);

	// Mask out the places where the bitmap will be placed.
	dcMem.BitBlt(0, 0, TempSize.cx, TempSize.cy, &dcObject, 0, 0, SRCAND);

	// Mask out the transparent colored pixels on the bitmap.
	dcTemp.BitBlt(0, 0, TempSize.cx, TempSize.cy, &dcBack, 0, 0, SRCAND);

	// XOR the bitmap with the background on the destination DC.
	dcMem.BitBlt(0, 0, TempSize.cx, TempSize.cy, &dcTemp, 0, 0, SRCPAINT);

	// Copy the destination to the screen.
	pDC->StretchBlt(rcDrawBMP.left, rcDrawBMP.top, rcDrawBMP.Width(), rcDrawBMP.Height(), 
		&dcMem, 0, 0 ,szSrcBMP.cx, szSrcBMP.cy, SRCCOPY);

	// Place the original bitmap back into the bitmap sent here.
	dcTemp.BitBlt(0, 0, TempSize.cx, TempSize.cy, &dcSave, 0, 0, SRCCOPY);

	// Delete the memory bitmaps.
	CBitmap* pTempBmp=NULL;
	pTempBmp=dcBack.SelectObject(pbmBackOld);
	ASSERT(pTempBmp != NULL);
	pTempBmp->DeleteObject();
	pTempBmp=dcObject.SelectObject(pbmObjectOld);
	ASSERT(pTempBmp != NULL);
	pTempBmp->DeleteObject();
	pTempBmp=dcMem.SelectObject(pbmMemOld);
	ASSERT(pTempBmp != NULL);
	pTempBmp->DeleteObject();
	pTempBmp=dcSave.SelectObject(pbmSaveOld);
	ASSERT(pTempBmp != NULL);
	pTempBmp->DeleteObject();

	// Delete the memory DCs.
	dcMem.DeleteDC();
	dcBack.DeleteDC();
	dcObject.DeleteDC();
	dcSave.DeleteDC();
	dcTemp.DeleteDC();
	}

BEGIN_MESSAGE_MAP(COXStatusBar, CStatusBar)
	//{{AFX_MSG_MAP(COXStatusBar)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_MESSAGE(SB_SETMINHEIGHT, OnSetMinHeight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

LRESULT COXStatusBar::OnSetMinHeight(WPARAM wParam, LPARAM)
	{
	LRESULT lResult=Default();
	
	// MFC does not allow a height smaller than the font height
	m_nMinHeight=PtrToInt(wParam);
	
	return lResult;
	}

void COXStatusBar::OnSize(UINT nType, int cx, int cy)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	CControlBar::OnSize(nType, cx, cy);

	// need to adjust pane right edges (because of stretchy pane)
	if (m_nCount > 0)
		UpdateAllPanes(TRUE, TRUE);

	/////////////
	// fixed: 15.01.1998
	// bitmap does not refresh correctly if you size
	// (horizontally) the application window.  If you reduce the width of the
	// window to hide the bitmap and then expand the width to show it, the bitmap
	// is not always refreshed
	///////////
	// redraw panes with bitmaps
	CRect rect;
	for (int nIndex=0; nIndex < m_nCount; nIndex++)
	{
		GetItemRect(nIndex, &rect);  // get pane rect
		InvalidateRect(rect, FALSE);
	}
	/////////////////////
}

void COXStatusBar::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
	UNREFERENCED_PARAMETER(nFlags);

	CRect PaneRect;
	// first walk through to calculate extra space
	for (int i=0; i < m_nCount; i++)
		{
		GetItemRect(i, PaneRect);
		if (PaneRect.PtInRect(point) != 0)
			{
			GetParent()->SendMessage(WM_STAT_DBLCLICK, (WPARAM)GetItemID(i));
			break;
			}
		}
	
	}

int COXStatusBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
	{

	int key=GetAsyncKeyState(VK_LBUTTON);
	if(key&0x8001) 
	{
		return -1;
	}

	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));
	CRect PaneRect;

	// check child windows first by calling CControlBar
	int nHit= PtrToInt(CControlBar::OnToolHitTest(point, pTI));
	if (nHit != -1)
		{
		if (pTI != NULL)
			{
			pTI->uFlags &= ~TTF_NOTBUTTON;
			pTI->uFlags &= ~TTF_CENTERTIP;

			pTI->lpszText=_tcsdup(m_TipArray.GetAt(nHit));
			}

		return nHit + m_nCount;  // register a different toolinfo struct than the percent text
		}

	// now hit test against Panes of Statusbar
	for (int i=0; i < m_nCount; i++)
		{
		GetItemRect(i, PaneRect);
		if (PaneRect.PtInRect(point))
			{
			if (pTI != NULL)
				{
				pTI->hwnd=m_hWnd;
				pTI->rect=PaneRect;
				pTI->lpszText=_tcsdup(m_TipArray.GetAt(i));
				}
			
			// found matching rect, return the ID of the Pane
			return GetItemID(i);
			}
		}
	
	return -1;
	}

BOOL COXStatusBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
	{
	if (nHitTest==HTCLIENT)
		{
		DWORD dwMessagePos=::GetMessagePos();
		CPoint point(GET_X_LPARAM(dwMessagePos),GET_Y_LPARAM(dwMessagePos));
		ScreenToClient(&point);

		// Walk through all panes to find match
		CRect PaneRect;
		int i=0;
		for (i=0; i < m_nCount; i++)
			{
			GetItemRect(i, PaneRect);
			if (PaneRect.PtInRect(point) != 0)
				break;
			}

		if (i < m_nCount && m_CursorArray[i] != NULL)
			{
			::SetCursor((HCURSOR)(INT_PTR)m_CursorArray[i]);
			// ... We handled the message
			return TRUE;
			}
		}

	// ... Call base class implementation
	return CStatusBar::OnSetCursor(pWnd, nHitTest, message);
	}

int COXStatusBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
	{
	if (CStatusBar::OnCreate(lpCreateStruct)==-1)
		return -1;
	
	if (!m_srcDC.CreateCompatibleDC(NULL))
		return -1;
	
	return 0;
	}

COXStatusbarSkin* COXStatusBar::GetStatusbarSkin()
{
	// Check if the app is derived from COXSkinnedApp
	COXSkinnedApp* pSkinnedApp = DYNAMIC_DOWNCAST(COXSkinnedApp, AfxGetApp());
	if (pSkinnedApp != NULL && pSkinnedApp->GetCurrentSkin() != NULL)
		return pSkinnedApp->GetCurrentSkin()->GetStatusbarSkin();
	else
	{
		// Create a classic skin for this class if not created already
		if (m_pStatusbarSkin == NULL)
			m_pStatusbarSkin = new COXStatusbarSkinClassic();

		return m_pStatusbarSkin;
	}
}
