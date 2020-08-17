// ==========================================================================
// 							Class Implementation : COXDockPropertySheet
// ==========================================================================

// Source file : OXDckPSh.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "OXDckPSh.h"		// class specification
#include "OXDckPPg.h"
#include "..\include\oxdckpsh.h"
#include "OXSzMiniDockFrmWnd.h"

#include "UTB64Bit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define TABBORDER_OPENSPACE		6
#define TAB_ID					1000

/////////////////////////////////////////////////////////////////////////////
// Definition of static members


// Data members -------------------------------------------------------------
// protected:

//	CString			m_sCaption;
//	---				Title of this sheet

//	CTabCtrl		m_tabCtrl;
//	---				The tabcontrol that will recieve all the pages of this sheet

//	CObArray		m_pageArray;
//	---				An array where all added pages will be stored

//  CObList			m_DynPageList
//	---				A list to keep track of all dynamic added pages.

//	CRect			m_rectTabArea;
//	---				The rectangle wherein all pages will be shown

//	int				m_nCurrentSelection;
//	---				The index of the currently selected page

// private:

// Member functions ---------------------------------------------------------
// public:

COXDockPropertySheet::COXDockPropertySheet(LPCTSTR pszCaption /*= NULL*/)
: COXDialogBar(),
m_rectTabArea(0,0,0,0),
m_nCurrentSelection(-1)
{
	if (pszCaption)
		m_sCaption = pszCaption;
	Construct();
}

COXDockPropertySheet::COXDockPropertySheet(UINT nIDCaption)
: COXDialogBar(),
m_rectTabArea(0,0,0,0),
m_nCurrentSelection(-1)
{
	m_sCaption.LoadString(nIDCaption);
	Construct();
}

void COXDockPropertySheet::Construct()
{
	//{{AFX_DATA_INIT(COXDockPropertySheet)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


COXDockPropertySheet::~COXDockPropertySheet()
{
	for (int nPage = 0 ; nPage < m_pageArray.GetSize() ; nPage++)
	{
		COXDockPropertyPage* pPage = (COXDockPropertyPage*) m_pageArray[nPage];

		if (m_DynPageList.Find(pPage) != NULL)
		{
			ASSERT(pPage->IsKindOf(RUNTIME_CLASS(COXDockPropertyPage)));
			if (pPage->IsDynamicPage())
			{
				pPage->DestroyWindow();
				delete pPage;
			}
		}
	}
}

void COXDockPropertySheet::AdjustAllPages()
{
	// position all child pages, in the tabcontrol
	for (int nPage = 0 ; nPage < m_pageArray.GetSize() ; nPage++)
	{
		// position the dialog/property-page within the tab control
		((CWnd*)(m_pageArray[nPage]))->SetWindowPos(NULL, m_rectTabArea.left, m_rectTabArea.top, 0, 0,
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);

	}

	if (m_pageArray.GetSize() > 0)
		SelectPage(m_nCurrentSelection != -1 ? m_nCurrentSelection : 0);
}


BOOL COXDockPropertySheet::SelectPage(int nNextSel)
{
	if (GetPageCount() == 0)
	{
		m_nCurrentSelection = -1;
		return FALSE;
	}

	if (nNextSel < -1 || nNextSel >= GetPageCount())
		return FALSE;

	if (nNextSel != -1)
	{
		COXDockPropertyPage* pNextPage = (COXDockPropertyPage*)m_pageArray[nNextSel];

		if (m_nCurrentSelection != nNextSel)
		{
			if (m_nCurrentSelection > -1)		// Thanks Franco
				if (!((COXDockPropertyPage*)m_pageArray[m_nCurrentSelection])->OnKillActive())
					return FALSE;

			pNextPage->EnableWindow(TRUE);
			if (!pNextPage->OnSetActive())
			{
				pNextPage->EnableWindow(FALSE);
				return FALSE;
			}

			for (int nPage = 0 ; nPage < m_pageArray.GetSize() ; nPage++)
			{
				if (nPage != nNextSel)
				{
					((COXDockPropertyPage*)m_pageArray[nPage])->ShowWindow(SW_HIDE);
					((COXDockPropertyPage*)m_pageArray[nPage])->EnableWindow(FALSE);
				}
			}

			pNextPage->ShowWindow(SW_SHOW);
			m_nCurrentSelection = nNextSel;
			m_tabCtrl.SetCurSel(m_nCurrentSelection);
		}

		pNextPage->SetFocus();
	}

	return TRUE;
}

BOOL COXDockPropertySheet::Create(CWnd* pParentWnd, UINT nDlgStyle,
								  UINT nTabStyle /* = WS_CHILD | WS_VISIBLE | TCS_TABS | TCS_SINGLELINE | TCS_RAGGEDRIGHT */,
								  UINT nID /* = 1 */)
{
	BOOL bSuccess = COXDialogBar::Create(pParentWnd, nDlgStyle, nID);

	if (bSuccess)
	{
		CRect ClientRect(0,0,0,0);
		bSuccess = m_tabCtrl.Create(nTabStyle, ClientRect, this, TAB_ID);	

		for (int nPage=0 ; nPage < m_pageArray.GetSize() ; nPage++)
			AddPage((COXDockPropertyPage*) m_pageArray[nPage]);

		AdjustAllPages();
	}

	return bSuccess;
}

COXDockPropertyPage* COXDockPropertySheet::GetActivePage() const
{
	ASSERT_VALID(this);
	if (m_nCurrentSelection == -1)
		return NULL;

	return GetPage(GetActiveIndex());
}

int COXDockPropertySheet::GetPageCount() const
{
	ASSERT_VALID(this);

	if (m_hWnd == NULL || m_tabCtrl.m_hWnd == NULL)
		return PtrToInt(m_pageArray.GetSize());

	return m_tabCtrl.GetItemCount();
}

int COXDockPropertySheet::GetActiveIndex() const
{
	if (m_hWnd == NULL || m_tabCtrl.m_hWnd == NULL)
		return m_nCurrentSelection;

	return m_tabCtrl.GetCurSel();
}

BOOL COXDockPropertySheet::SetActivePage(int nPage)
{
	if (m_hWnd == NULL || m_tabCtrl.m_hWnd == NULL)
	{
		m_nCurrentSelection = nPage;
		return TRUE;
	}

	return SelectPage(nPage);
}

int COXDockPropertySheet::GetPageIndex(COXDockPropertyPage* pPage)
{
	for (int i = 0; i < GetPageCount(); i++)
	{
		if (GetPage(i) == pPage)
			return i;
	}
	return -1;  // pPage not found
}

void COXDockPropertySheet::SetTitle(LPCTSTR lpszText)
{
	if (m_hWnd != NULL)
		SetWindowText(lpszText);

	m_sCaption = lpszText;
}

BOOL COXDockPropertySheet::AddPage(COXDockPropertyPage* pPage,
								   BOOL bAdjust /* = FALSE */)
{
	// when tab control not yet created, just add pages to an array
	// pages will be created later.
	if (m_tabCtrl.m_hWnd != NULL)
	{
		CRect PageRect;
		TC_ITEM tie; 

		pPage->Create(pPage->m_nID, &m_tabCtrl);
		if (pPage->m_sCaption.IsEmpty())
			pPage->GetWindowText(pPage->m_sCaption);
		pPage->ModifyStyle(WS_BORDER | WS_CAPTION, 0);

		// Add a tab for each of the child dialog boxes. 
		tie.mask = TCIF_TEXT | TCIF_IMAGE; 
		tie.iImage = -1; 
		tie.pszText = pPage->m_sCaption.GetBufferSetLength(pPage->m_sCaption.GetLength());
		pPage->m_sCaption.ReleaseBuffer();

		// already add the tab to the tabctrl otherwise, the Function AdjustRect
		// returns a adjusted rect without leaving space for the TABS. This is specifically
		// seen when adding the first page.
		int count = GetPageCount();
		m_tabCtrl.InsertItem(count, &tie);

		// Calculate the biggest dialog and size the tabcontrol, control bar
		// accordingly
		pPage->GetWindowRect(&PageRect);
		PageRect.OffsetRect(-PageRect.left, -PageRect.top);
		PageRect.OffsetRect(m_rectTabArea.left, m_rectTabArea.top);
		PageRect.right = __max(PageRect.right, m_rectTabArea.right);
		PageRect.bottom = __max(PageRect.bottom, m_rectTabArea.bottom);
		if (PageRect != m_rectTabArea)
		{
			// size the tab control
			CRect TabCtrlArea(PageRect);
			m_tabCtrl.AdjustRect(TRUE, TabCtrlArea);
			m_tabCtrl.SetWindowPos(NULL, TABBORDER_OPENSPACE, TABBORDER_OPENSPACE,
				TabCtrlArea.Width(), TabCtrlArea.Height(),
				SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
			m_tabCtrl.GetWindowRect(&TabCtrlArea);
			TabCtrlArea.OffsetRect(-TabCtrlArea.left, -TabCtrlArea.top);
			m_tabCtrl.AdjustRect(FALSE, TabCtrlArea);
			m_rectTabArea = TabCtrlArea;

			// Size the control bar
			m_tabCtrl.GetWindowRect(&TabCtrlArea);
			TabCtrlArea.InflateRect(TABBORDER_OPENSPACE, TABBORDER_OPENSPACE);
			m_sizeDefault.cx = TabCtrlArea.Width();
			m_sizeDefault.cy = TabCtrlArea.Height();

		}

		int size = PtrToInt(m_pageArray.GetSize());
		if (count + 1 > size)
		{
			m_pageArray.Add(pPage);
			if (pPage->IsDynamicPage())
				m_DynPageList.AddTail(pPage);
		}

		pPage->SendMessage(WM_NCACTIVATE, TRUE);

		if (bAdjust)
			AdjustAllPages();
	}
	else
	{
		m_pageArray.Add(pPage);
		if (pPage->IsDynamicPage())
			m_DynPageList.AddTail(pPage);
	}

	return TRUE;
}

void COXDockPropertySheet::RemovePage(int nPage)
{
	ASSERT_VALID(this);

	if (0 <= nPage && nPage < GetPageCount())
	{
		::LockWindowUpdate(m_tabCtrl.m_hWnd);
		// remove the page externally
		if (m_hWnd != NULL && m_tabCtrl.m_hWnd != NULL)
		{
			COXDockPropertyPage* pPage = (COXDockPropertyPage*) m_pageArray[nPage];
			ASSERT(pPage->GetRuntimeClass()->IsDerivedFrom(RUNTIME_CLASS(COXDockPropertyPage)));

			m_tabCtrl.DeleteItem(nPage);
			pPage->ShowWindow(SW_HIDE);
			pPage->EnableWindow(FALSE);
			POSITION PagePos;
			if ((PagePos = m_DynPageList.Find(pPage)) != NULL && pPage->IsDynamicPage())
			{
				m_DynPageList.RemoveAt(PagePos);
				pPage->DestroyWindow();
				delete pPage;
			}
		}

		// remove the page from internal list
		m_pageArray.RemoveAt(nPage);

		int nNewCurSel(-1);
		if (nPage < m_nCurrentSelection)
		{
			// should there be no more items, then SelectPage will reset the current selction
			nNewCurSel = m_nCurrentSelection - 1;
		}
		else if (nPage == m_nCurrentSelection)
		{
			if (nPage == 0)
				nNewCurSel = 0; 
			else
				nNewCurSel = m_nCurrentSelection - 1;

			// To force SelectPage to select a new page
			m_nCurrentSelection = -1;
		}

		SelectPage(nNewCurSel);
		::LockWindowUpdate(NULL);
	}
}

int COXDockPropertySheet::NextPageIndex(int nPage)
{
	return (nPage + 1) % GetPageCount();
}

int COXDockPropertySheet::PrevPageIndex(int nPage)
{
	return ((nPage - 1) % GetPageCount()) >= 0 ? (nPage - 1) % GetPageCount(): ((nPage - 1) % GetPageCount()) + GetPageCount();
}

void COXDockPropertySheet::DoDataExchange(CDataExchange* pDX)
{
	COXDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXDockPropertySheet)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COXDockPropertySheet, COXDialogBar)
	//{{AFX_MSG_MAP(COXDockPropertySheet)
	ON_NOTIFY(TCN_SELCHANGING, TAB_ID, OnSelchangingTab)
	ON_NOTIFY(TCN_SELCHANGE, TAB_ID, OnSelchangeTab)
	//}}AFX_MSG_MAP
	ON_WM_MOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXDockPropertySheet message handlers

void COXDockPropertySheet::OnSelchangingTab(NMHDR* /* pNMHDR */, LRESULT* pResult) 
{
	COXDockPropertyPage* pPage = GetActivePage();
	if (pPage != NULL)
		*pResult = !pPage->OnKillActive();
}

void COXDockPropertySheet::OnSelchangeTab(NMHDR* /* pNMHDR */, LRESULT* pResult) 
{
	int nSel = m_tabCtrl.GetCurSel();
	if (nSel != -1)
		*pResult = SelectPage(nSel);
}

#if _MSC_VER < 1400
int COXDockPropertySheet::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
#else
INT_PTR COXDockPropertySheet::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
#endif
{
	TC_HITTESTINFO HitInfo;

	CRect TabRect;
	m_tabCtrl.GetWindowRect(TabRect);
	ClientToScreen(&point);
	if (TabRect.PtInRect(point))
	{
		m_tabCtrl.ScreenToClient(&point);

		HitInfo.pt = point;
		m_tabCtrl.HitTest(&HitInfo);

		if (HitInfo.flags == TCHT_NOWHERE)
			return -1;
	}

	return CWnd::OnToolHitTest(point, pTI);
}

BOOL COXDockPropertySheet::PreTranslateMessage(MSG* pMsg)
{
	ASSERT_VALID(this);

	// allow tooltip messages to be filtered
	if (CWnd::PreTranslateMessage(pMsg))
		return TRUE;

	// allow sheet to translate Ctrl+Tab, Shift+Ctrl+Tab,
	//  Ctrl+PageUp, and Ctrl+PageDown
	if (pMsg->message == WM_KEYDOWN && GetAsyncKeyState(VK_CONTROL) < 0)
	{
		switch (pMsg->wParam)
		{
		case VK_TAB:
		case VK_PRIOR:
		case VK_NEXT:

			if (GetAsyncKeyState(VK_SHIFT) < 0 || pMsg->wParam == VK_PRIOR)
				SetActivePage(PrevPageIndex(GetActiveIndex()));
			else
				SetActivePage(NextPageIndex(GetActiveIndex()));

			break;
		default:
			break;
		}

		return TRUE;
	}

	// handle rest with IsDialogMessage
	return PreTranslateInput(pMsg);
}

void COXDockPropertySheet::OnMove(int x, int y)
{
	COXDialogBar::OnMove(x, y);

	COXSizableMiniDockFrameWnd* pMiniFrame = DYNAMIC_DOWNCAST(COXSizableMiniDockFrameWnd, GetParent()->GetParent());
	if (pMiniFrame != NULL)
		pMiniFrame->GetContainedBarType();
}
