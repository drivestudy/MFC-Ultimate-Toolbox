// OXAutoListBox.cpp : implementation file
//

// Version: 9.3


#include "stdafx.h"
#include "OXAutoListBox.h"
#include "OXAutoComplete.h"
#include "UTB64Bit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXAutoListBox

COXAutoListBox::COXAutoListBox(COXAutoComplete* pComplete) :
m_bDraw(FALSE), m_nHeight(NULL), m_nWidth(NULL),
m_pComplete(pComplete)
{
}

COXAutoListBox::~COXAutoListBox()
{
}


BEGIN_MESSAGE_MAP(COXAutoListBox, CListBox)
	//{{AFX_MSG_MAP(COXAutoListBox)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR( )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXAutoListBox message handlers

void COXAutoListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (m_bDraw)
	{
		CRect rct=lpDrawItemStruct->rcItem;
		CDC* pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
		int nIndex=(lpDrawItemStruct->itemState & ODS_SELECTED)?
		COLOR_HIGHLIGHT:COLOR_WINDOW;
		CBrush* pBrush=CBrush::FromHandle(::GetSysColorBrush(nIndex));
		pDC->FillRect(rct,pBrush);

		int nMode=pDC->SetBkMode(TRANSPARENT);
		if (lpDrawItemStruct->itemID>=0)
		{
			CString sText;
			COLORREF clrText=::GetSysColor((lpDrawItemStruct->itemState & ODS_SELECTED)?
			COLOR_HIGHLIGHTTEXT:COLOR_WINDOWTEXT);
			COLORREF clrOld=pDC->SetTextColor(clrText);
			GetText(lpDrawItemStruct->itemID, sText);
			pDC->TextOut(rct.left,rct.top,sText);
			pDC->SetTextColor(clrOld);
		}
		pDC->SetBkMode(nMode);
	}
}

BOOL COXAutoListBox::PreCreateWindow(CREATESTRUCT& cs) 
{
	ASSERT(cs.style & LBS_OWNERDRAWFIXED);

	
	return CListBox::PreCreateWindow(cs);
}

void COXAutoListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->CtlID=::GetWindowLongPtr(
		GetSafeHwnd(),GWL_ID);
	lpMeasureItemStruct->CtlType=ODT_LISTBOX;
	lpMeasureItemStruct->itemData=1;
	lpMeasureItemStruct->itemWidth=m_nWidth;
	lpMeasureItemStruct->itemHeight=m_nHeight;
	lpMeasureItemStruct->itemID=0;
}

BOOL COXAutoListBox::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif
	
	return CWnd::Create(_T("LISTBOX"), NULL, dwStyle, rect, pParentWnd, nID,NULL);
}

void COXAutoListBox::OnMouseMove(UINT nFlags, CPoint point) 
{

	int nIndex=GetItemFromPoint(point);
	if (nIndex!=-1)
	{
	if (GetCurSel()!=nIndex)
		SetCurSel(nIndex);
	}
	CListBox::OnMouseMove(nFlags, point);
}

void COXAutoListBox::SetComplete(COXAutoComplete *pComplete)
{
	m_pComplete=pComplete;
}

void COXAutoListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	UNUSED_ALWAYS(nFlags);

	ASSERT(m_pComplete);

	int nIndex=GetItemFromPoint(point);
	if (nIndex!=-1)
	{
		CString sText;
		GetText(nIndex,sText);
		::SendMessage( m_pComplete->m_hAttached, WM_SETTEXT, NULL,(LPARAM)(LPCTSTR) sText);
	}
	CListBox::OnLButtonDown(nFlags, point);
	::PostMessage( m_pComplete->m_hAttached, WM_SETFOCUS, (WPARAM)::GetFocus(), NULL);
}

int COXAutoListBox::GetItemFromPoint(POINT pt)
{
	CRect rct;
	GetClientRect(&rct);

	
	if (rct.PtInRect(pt))
	{
		int nTop=GetTopIndex();
		
		int nCount=rct.Height()/GetItemHeight(nTop);
		for (int n=nTop;n<nTop+nCount;n++)
		{
			//find  hot item to select
			GetItemRect(n,&rct);
			if (rct.PtInRect(pt))
				return n;
		
		}
	}
	return -1;

}

BOOL COXAutoListBox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	return CListBox::OnSetCursor(pWnd, nHitTest, message);
}