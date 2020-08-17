// ========================================================================================
// 					Class Implementation : COXSizeToolBar
// ========================================================================================

// Source file : OXSizeToolBar.cpp

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
#include <afxpriv.h>

#include "OXSizeToolBar.h"

#include "UTBStrOp.h"
#include "UTB64Bit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXSizeToolBar, COXSizeControlBar)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CToolBar idle update through COXToolCmdUI class. 

void COXToolCmdUI::Enable(BOOL bOn)
{
	m_bEnableChanged = TRUE;
	COXSizeToolBar* pSizeToolBar = (COXSizeToolBar*)m_pOther;
	COXToolBarCtrl* pToolBar = &pSizeToolBar->m_ToolCtrl;
	ASSERT(pToolBar != NULL);

	ASSERT(m_nIndex < m_nIndexMax);

	// Get toolbar button state
	TBBUTTON TB;
	pToolBar->GetButton(m_nIndex, &TB);
	UINT nNewState = TB.fsState & ~TBSTATE_ENABLED;
	if (bOn)
		nNewState |= TBSTATE_ENABLED;
	if (nNewState != TB.fsState)
		pToolBar->SetState(m_nID, nNewState);
}


void COXToolCmdUI::SetCheck(int nCheck)
{
	ASSERT(nCheck >= 0 && nCheck <= 2); // 0=>off, 1=>on, 2=>indeterminate
	COXSizeToolBar* pSizeToolBar = (COXSizeToolBar*)m_pOther;
	COXToolBarCtrl* pToolBar = &pSizeToolBar->m_ToolCtrl;
	ASSERT(pToolBar != NULL);

	ASSERT(m_nIndex < m_nIndexMax);

	// Get toolbar button state
	TBBUTTON TB;
	pToolBar->GetButton(m_nIndex, &TB);
	UINT nNewState = TB.fsState & ~ (TBSTATE_CHECKED | TBSTATE_INDETERMINATE);

	if (nCheck == 1)
		nNewState |= TBSTATE_CHECKED;
	else
		if (nCheck == 2)
			nNewState |= TBSTATE_INDETERMINATE;

	if (nNewState != TB.fsState)
		pToolBar->SetState(m_nID, nNewState);

	// should we set the button style too ?
	//	pToolBar->_SetButtonStyle(m_nIndex, nNewStyle | TBBS_CHECKBOX); */
}


void COXToolCmdUI::SetText(LPCTSTR)
{
	// ignore it
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// COXToolBarCtrl
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

COXToolBarCtrl::COXToolBarCtrl()
{
}

COXToolBarCtrl::~COXToolBarCtrl()
{
}

BEGIN_MESSAGE_MAP(COXToolBarCtrl, CToolBarCtrl)
	//{{AFX_MSG_MAP(COXToolBarCtrl)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COXToolBarCtrl message handlers

// Returns the size that would bound all the buttons in the toolbar 
void COXToolBarCtrl::GetBoundingSize(CSize & size)
{
	size.cx = 0;
	size.cy = 0;

	for (int i = GetButtonCount(); i >= 0; i--)
	{
		CRect rect;
		if (GetItemRect(i, &rect))
		{
			size.cx = __max(size.cx, rect.right);
			size.cy = __max(size.cy, rect.bottom);
		}
	}
}


// returns index of button that lies at that point
int COXToolBarCtrl::HitTestButtons(CPoint point) 
{
	// Hit test all the buttons. It might be an idea to cache the rectangles for all the
	// items, but for now this loop doesn't seem too bad
	for (int i = GetButtonCount(); i >= 0; i--)
	{
		TBBUTTON tbButton;
		CRect rect;
		if (GetItemRect(i, &rect))
		{
			if (rect.PtInRect(point))
			{
				GetButton(i, &tbButton);
				if (tbButton.fsStyle == TBSTYLE_SEP)
					return -1;				// inside a separator		
				else
					return i;				// inside a button
			}
			if (point.y > rect.bottom)		// if point we're testing is below items in the
				// array, then stop (minor optimization)
				break;
		}
	}
	return -1;
}


void COXToolBarCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (HitTestButtons(point) != -1)
	{
		CToolBarCtrl::OnLButtonDown(nFlags, point);
	}
	else
	{
		CWnd* pParent = GetOwner();
		ASSERT(pParent != NULL);
		// map to co-ordinates of parent window
		ClientToScreen(&point);
		pParent->ScreenToClient(&point);
		// let the parent handle it
		pParent->SendMessage(WM_LBUTTONDOWN, nFlags, MAKELONG(point.x, point.y));  
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// COXSizeToolBar

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// Data members -------------------------------------------------------------
// protected:

// private:

// Member functions ---------------------------------------------------------
// public:

#define TOOLCTRL_ID 99

/////////////////////////////////////////////////////////////////////////////
// COXSizeToolBar

COXSizeToolBar::COXSizeToolBar(int nStyle)
: COXSizeControlBar(nStyle)
{
	// these are the default sizes for the toolbar bitmap
	m_ToolCtrlButtonSize.cx = 16;
	m_ToolCtrlButtonSize.cy = 15;

	m_pBitmapIds = NULL;
	m_nBitmapButtons = 0;
}


COXSizeToolBar::~COXSizeToolBar()
{
}


BEGIN_MESSAGE_MAP(COXSizeToolBar, COXSizeControlBar)
	//{{AFX_MSG_MAP(COXSizeToolBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_RANGE(TTN_NEEDTEXT, 0, 0xffff, OnTBNToolTip)
	ON_NOTIFY_RANGE(TBN_QUERYINSERT, 0, 0xffff, OnTBNQueryInsert)
	ON_NOTIFY_RANGE(TBN_QUERYDELETE, 0, 0xffff, OnTBNQueryDelete)
	ON_NOTIFY_RANGE(TBN_BEGINADJUST, 0, 0xffff, OnTBNBeginAdjust)
	ON_NOTIFY_RANGE(TBN_TOOLBARCHANGE, 0, 0xffff, OnTBNToolBarChange)
	ON_NOTIFY_RANGE(TBN_GETBUTTONINFO, 0, 0xffff, OnTBNGetButtonInfo)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COXSizeToolBar message handlers


#define DOCKED_HORZ_BORDER 4
#define DOCKED_VERT_BORDER 2

// respond to this event as we need to override it
void COXSizeToolBar::OnSizedOrDocked(int cx, int cy, BOOL /* bFloating */, int /* flags */)
{
	CRect rect(1, 1, cx, cy);		// rectangle for CToolbarCtrl

	// shrink rectangle if we're docked
	rect.InflateRect(-DOCKED_HORZ_BORDER, -DOCKED_VERT_BORDER);

	m_ToolCtrl.MoveWindow(&rect);
	m_ToolCtrl.AutoSize();
}

BOOL COXSizeToolBar::Create(CWnd* pParent, DWORD dwStyle, UINT nID, LPRECT pRect)
{
	m_ToolCtrlButtonSize.cx = 16;
	m_ToolCtrlButtonSize.cy = 15;

	// if no rectangle supplied, then what is hopefully as sensible default.
	// ie a single row of buttons
	CRect rect;
	if (pRect != NULL)
		rect.CopyRect(pRect);
	else
	{
		rect.left = 0;
		rect.top = 0;
		rect.bottom = m_ToolCtrlButtonSize.cy + 18;
		rect.right = m_ToolCtrlButtonSize.cx * 12;
	}

	BOOL status = COXSizeControlBar::Create(pParent, NULL, nID, dwStyle, rect); 
	if (status == TRUE)
	{
	}
	return status;
}



int COXSizeToolBar::OnCreate(LPCREATESTRUCT lpCS) 
{
	if (COXSizeControlBar::OnCreate(lpCS) == -1)
		return -1;

	CRect rect;
	GetClientRect(&rect);

	DWORD dwStyle = WS_VISIBLE | WS_CHILD | TBSTYLE_WRAPABLE | TBSTYLE_TOOLTIPS
		| CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER;  // | CCS_NOHILITE*  - CCS_NOHILITE doesn't exist in VC2.2 

	// If there a list of bitmap id's, then let the user configure the toolbar
	if (m_pBitmapIds != NULL)
		dwStyle |= CCS_ADJUSTABLE;

	if (!m_ToolCtrl.Create(dwStyle, rect, this, TOOLCTRL_ID))
	{
		TRACE0("Failed to create CToolBarCtrl\n");
		return -1;
	}
	return 0;
}



// These are functions designed as 1-1 replacements for existing CToolBar functions. They
// are not the most efficient way to make use of the CSizeToolBarCtrl control, but mean it'll
// work with standard AppWizard created applications.

BOOL COXSizeToolBar::LoadBitmap(LPCTSTR lpszResourceName)
{
	if (!m_Bitmap.LoadBitmap(lpszResourceName))
		return FALSE;

	// get the bitmap info, and use this to get the no of buttons in the bitmap
	BITMAP bm;
	m_Bitmap.GetObject(sizeof(BITMAP), &bm); 

	int nButtons = bm.bmWidth / m_ToolCtrlButtonSize.cx;

	if (m_ToolCtrl.AddBitmap(nButtons, &m_Bitmap) == -1)
		return FALSE;

	return TRUE;
}


void COXSizeToolBar::SetSizes(SIZE sizeButton, SIZE sizeImage)
{
	// if succeeded, keep track of it in the size of the control
	if (m_ToolCtrl.SetButtonSize(sizeButton))
		m_ToolCtrlButtonSize = sizeButton;

	m_ToolCtrl.SetBitmapSize(sizeImage);
}


void COXSizeToolBar::SetBitmapIds(UINT* pIds, int nButtons)
{
	m_pBitmapIds = pIds;
	m_nBitmapButtons = nButtons;
}


int COXSizeToolBar::FindBitmapIndex(UINT nID)
{
	ASSERT(m_pBitmapIds != NULL);
	for (int i = 0; i < m_nBitmapButtons ; i++)
	{
		if (m_pBitmapIds[i] == (int)nID)
			return i;
	}
	return -1;
}

// emulate CToolBar::SetButtons()
BOOL COXSizeToolBar::SetButtons(UINT* pButtons, int nButtons)
{
	// allocate an array of TBBUTTON's
	TBBUTTON* parrButtons = new TBBUTTON[nButtons];		// allocate an array

	int nImageNo = 0;		// no of image in bitmap (coun
	int nBtn = 0; 			// no of buttons we've actually created (including separators)
	// may not equal no of buttons supplied by user, as id's may not
	// be found
	int i = 0;
	for (i = 0; i < nButtons; i++)
	{
		UINT nID = pButtons[i];
		parrButtons[i].dwData =  NULL;
		parrButtons[i].iString = NULL;
		if (nID == ID_SEPARATOR)
		{
			parrButtons[nBtn].iBitmap = NULL;
			parrButtons[nBtn].idCommand = 0;
			parrButtons[nBtn].fsState = 0;
			parrButtons[nBtn].fsStyle = TBSTYLE_SEP;  
		} 
		else
		{
			if (m_pBitmapIds != NULL)		// if there's a list of bitmaps, then translate this
			{
				nImageNo = FindBitmapIndex(nID);
				if (nImageNo == -1)
				{
					TRACE1("Couldn't find bitmap for ID=%d\n", nID);
					continue;					// skip to next iteration
				}
				else
				{
					parrButtons[i].iBitmap = nImageNo;
				}
			}
			else
			{
				parrButtons[i].iBitmap = nImageNo;
				nImageNo++;
			}
			parrButtons[nBtn].idCommand = nID;
			parrButtons[nBtn].fsState = TBSTATE_ENABLED;
			parrButtons[nBtn].fsStyle = TBSTYLE_BUTTON; 
		}
		nBtn++;
	}

	BOOL status = m_ToolCtrl.AddButtons(nBtn, parrButtons);

	for (i = 0; i < nBtn; i++)
	{
		if (parrButtons[i].fsStyle == TBSTYLE_BUTTON)
		{
			CRect ButtonRect;
			m_ToolCtrl.GetItemRect(i, &ButtonRect);
			m_VertDockSize.cx = ButtonRect.Width() + (DOCKED_HORZ_BORDER + 1) * 2;

			// One set is enough
			break;
		}
	}

	delete parrButtons;

	return status;
}	


// tool tip notification handler for tool bar
afx_msg void COXSizeToolBar::OnTBNToolTip(UINT /* uID */, NMHDR* pNMHDR, LRESULT* /* pResult */)
{
	TOOLTIPTEXT* pTip = (TOOLTIPTEXT*)pNMHDR;
	pTip->hinst = NULL;
	pTip->lpszText = NULL;
	UINT ButtonId = PtrToUint(pTip->hdr.idFrom);	

	CString strText;
	if (strText.LoadString(ButtonId))
	{
		LPCTSTR pTipText = _tcschr(strText, _T('\n'));   // tool tip is after "\n" in the string
		if (pTipText != NULL)
		{	
			UTBStr::tcscpy(pTip->szText, 80, pTipText + 1);
			pTip->lpszText = pTip->szText;

			// try to ensure tool tip control and ensure it is top most in the Z-order
			CToolTipCtrl* pToolTipCtrl = m_ToolCtrl.GetToolTips();
			pToolTipCtrl->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

		}	
		// Set the text in the main window. Doesn't appear to be an easy way to check if
		// we're not over any particular bit of the tool tip.
		AfxGetMainWnd()->SendMessage(WM_SETMESSAGESTRING, ButtonId);
		return; 
	}	

	TRACE1("COXSizeToolBar:No Tooltip prompt for ID=%d\n", ButtonId);
	return;	
}


// Return information for bitmap indexes in the toolbar
afx_msg void COXSizeToolBar::OnTBNGetButtonInfo(UINT nID, NMHDR* pNMHDR, LRESULT* pResult)
{
	if (nID != TOOLCTRL_ID)
		return;
	TBNOTIFY * pTBN = (TBNOTIFY *)pNMHDR;

	int nIndex = pTBN->iItem;
	if (nIndex < m_nBitmapButtons)
	{
		*pResult = TRUE;

		UINT nButtonId = m_pBitmapIds[nIndex];
		pTBN->tbButton.iBitmap = nIndex;
		pTBN->tbButton.idCommand = nButtonId;
		pTBN->tbButton.fsState = TBSTATE_ENABLED;
		pTBN->tbButton.fsStyle = TBSTYLE_BUTTON;
		pTBN->tbButton.iString = 0;		
		if (pTBN->pszText != NULL)
		{		
			CString strText;
			if (strText.LoadString(nButtonId))
			{
				LPCTSTR pTipText = _tcschr(strText, _T('\n'));   // tool tip is after "\n" in the string
				if (pTipText != NULL)
				{	
					UTBStr::tcsncpy(pTBN->pszText, pTBN->cchText+1, pTipText + 1, pTBN->cchText);
					return;
				}
			}
			TRACE1("COXSizeToolBar:No Tooltip prompt for ID=%d\n", nButtonId);
			UTBStr::tcsncpy(pTBN->pszText, pTBN->cchText+1, _T("???"), pTBN->cchText);
		}
	}
	else 
		*pResult = FALSE;
}


afx_msg void COXSizeToolBar::OnTBNBeginAdjust(UINT /* uID */, NMHDR* /* pNMHDR */, LRESULT* /* pResult */)
{
}


afx_msg void COXSizeToolBar::OnTBNQueryInsert(UINT /* uID */, NMHDR* /* pNMHDR */, LRESULT* pResult)
{
	*pResult = TRUE;		// always allow buttons to be inserted
}


afx_msg void COXSizeToolBar::OnTBNQueryDelete(UINT /* uID */, NMHDR* /* pNMHDR */, LRESULT* pResult)
{
	*pResult = TRUE;		// always allow buttons to be deleted
}

afx_msg void COXSizeToolBar::OnTBNToolBarChange(UINT /* uID */, NMHDR* /* pNMHDR */, LRESULT* /* pResult */)
{
}


void COXSizeToolBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	COXSizeControlBar::OnUpdateCmdUI(pTarget,bDisableIfNoHndler);

	COXToolCmdUI state;
	state.m_pOther = this;

	state.m_nIndexMax = m_ToolCtrl.GetButtonCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		TBBUTTON TB;
		m_ToolCtrl.GetButton(state.m_nIndex, &TB);
		if (!(TB.fsStyle & TBSTYLE_SEP))
		{
			state.m_nID = TB.idCommand;
			state.DoUpdate(pTarget, bDisableIfNoHndler);
		}
	}
	// update any dialog controls added to the toolbar (probably unlikely in this case)
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}


CToolBarCtrl* COXSizeToolBar::GetToolBarCtrl()
{
	return &m_ToolCtrl;
}