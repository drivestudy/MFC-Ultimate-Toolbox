// ==========================================================================
//				Class Implementation : COXMultiComboBox
// ==========================================================================

// Source file : OXMultiComboBox.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXMultiComboBox.h"

#include "UTBStrOp.h"
#include "UTB64Bit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXMultiComboBox, CComboBox)

BEGIN_MESSAGE_MAP(COXMultiComboBox, CComboBox)
	//{{AFX_MSG_MAP(COXMultiComboBox)
	ON_WM_CTLCOLOR()
	ON_WM_DELETEITEM()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_OX_LISTHSCROLL,OnListHScroll)
	ON_MESSAGE(CB_GETITEMDATA,OnGetItemData)
	ON_MESSAGE(CB_SETITEMDATA,OnSetItemData)
	ON_MESSAGE(WM_COMPAREITEM,OnCompareItem)
	ON_MESSAGE(WM_MEASUREITEM,OnMeasureItem)
	ON_MESSAGE(WM_DRAWITEM,OnDrawItem)
	ON_MESSAGE(CB_INSERTSTRING,OnInsertString)
	ON_MESSAGE(CB_ADDSTRING,OnAddString)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// Data members-------------------------------------------------------------
//Protected 
//	int m_nColumnCount;
//	---	Numbers of columns 

//	int m_nMasterColumn
//	---	Master column number (zero based index)

//	CArray<int,int> m_aColumnWidth;
//	--- Array used to store the width of each column

//	enum {DEFAULT_COLUMN_WIDTH=40};
//	---	Default column width

//	CPen m_GridPen
//	--- Pen to Draw the grid
//	class COXComboLBox : public CWnd
//	---	This CWnd class is used to subclass the listbox hadle of the combobox
//		to handle its WM_HSCROLL message, which will be send to combobox.

//	COXComboLBox m_ComboLBox;
//	---	This Window object is used to subclass the window handle of listbox of
//		the combobox.

//	class COXRowData 
//	---	This private class is used to store the each items column strings and
//		its data. All the members in this are self explainatory

//Private 
//	BOOL m_fSizeChanged
//	---	 This will be true when a column width is set and accordingly 
//		 the scrollbasr are not adjusted.

//	BOOL m_fMasterColumnChanging
//	---	 Flag used to avoid from deleting the itemdata when master column is 
//		 changing.

//	int m_nPageWidth;
//	---	Amount of Width to scroll on SB_PAGELEFT and SB_PAGERIGHT

//	int m_nLineWidth;
//	--- Line width to scroll, on SB_LINELEFT and SB_LINERIGHT

// Member functions ---------------------------------------------------------
// public:

COXMultiComboBox::COXMultiComboBox()
{
	// ... By default
	m_nColumnCount = 1; 
	// ... By default
	m_nMasterColumn = 0; 
	m_aColumnWidth.SetSize(1);
	m_aColumnWidth[0] = DEFAULT_COLUMN_WIDTH;
	m_fSizeChanged = TRUE;
	m_ComboLBox.Init(this);
	m_fMasterColumnChanging = FALSE;

	m_bFitToSize=FALSE;

	//	Create pen with alternating dot - space
	//	(PS_DOT does not give a good result, so we use
	//	PS_COSMETIC | PS_ALTERNATE)
	LOGBRUSH logBrush;
	logBrush.lbColor = RGB(0,0,0);
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbHatch = HS_HORIZONTAL;
	if (!m_GridPen.CreatePen(PS_COSMETIC | PS_ALTERNATE, 1, &logBrush, 0,NULL))
		if (!m_GridPen.CreatePen(PS_COSMETIC, 1, &logBrush, 0, NULL))
			TRACE0("COXMultiComboBox::COXMultiComboBox : Error creating the grid lines Pen\n");
}

BOOL COXMultiComboBox::SetColumnCount(int nCount)
{
	if(nCount == m_nColumnCount)
		return TRUE;

	// ... nCount should be more than or equal to Master column and -1
	if ((nCount <= m_nMasterColumn) || (nCount < 0))
		return FALSE;

	m_aColumnWidth.SetSize(nCount); 

	// If size grows initailize the new columns width
	for(int nColIndex = m_nColumnCount; nColIndex < nCount; nColIndex++)
		m_aColumnWidth[nColIndex] = DEFAULT_COLUMN_WIDTH; 


	// Resizes the number of columns in each rowdata object
	int nCurCount = GetCount();
	for(int nIndex = 0; nIndex < nCurCount; nIndex++)
	{
		COXRowData* pRowData = GetRowData(nIndex);
		if (pRowData != NULL)
			pRowData->SetColumnCount(nCount);
		else
			TRACE0("In COXMultiComboBox::SetColumnCount : GetRowData() returned NULL.\n");
	}

	m_nColumnCount = nCount;

	AdjustToFitSize();

	return TRUE;
}

BOOL COXMultiComboBox::SetMasterColumn(int nCol)
{
	if(nCol == m_nMasterColumn)
		return TRUE;

	// Master Column index should be in between 0 and m_nColumnCount-1
	if ((m_nColumnCount <= nCol) || (nCol < 0))
		return FALSE;

	int nPrevMasterColumn = m_nMasterColumn;
	CString sPrevMasterColumnString;
	int nCurSel = GetCurSel();
	if(nCurSel!=-1)
		GetLBText(GetCurSel(),sPrevMasterColumnString);

	m_nMasterColumn = nCol;

	// Resorts based on newly set master column
	if(!ChangeMasterColumn(nCol))
	{
		m_nMasterColumn = nPrevMasterColumn;
		return FALSE;
	}

	// Selects the string in newly set master column into editbox of combo 
	if(nCurSel!=-1)
		if((SelectString(-1,nPrevMasterColumn,sPrevMasterColumnString.GetBuffer(0)))==CB_ERR)
			return FALSE;
	return TRUE;
}


BOOL COXMultiComboBox::SetColumnWidth(int nColIndex, int nWidth)
{
	if(nColIndex >= m_nColumnCount || nColIndex < -1)
		return FALSE;

	if(nColIndex == -1)
	{
		// Set the width of all the columns
		for(nColIndex =0; nColIndex<m_nColumnCount; nColIndex++)
			m_aColumnWidth[nColIndex] = nWidth;
	}
	else
		m_aColumnWidth[nColIndex] = nWidth;

	AdjustToFitSize();

	return TRUE;
}

int COXMultiComboBox::GetLBText(int nIndex,int nColIndex, LPTSTR lpszText) const
{
	if(nIndex >= GetCount() || nColIndex >= m_nColumnCount || nColIndex < 0 || nIndex < 0)
		return CB_ERR;

	// ... copies the string into buffer
	COXRowData* pRowData = GetRowData(nIndex);
	if (pRowData != NULL)
	{
		// ... returns the length
		UTBStr::tcscpy(lpszText, pRowData->GetColumnString(nColIndex).GetLength()+1, pRowData->GetColumnString(nColIndex).GetBuffer(0));
		return pRowData->GetColumnString(nColIndex).GetLength();
	}
	else
	{
		TRACE0("In COXMultiComboBox::GetLBText : GetRowData() returned NULL.\n");
		return CB_ERR;
	}
}

void COXMultiComboBox::GetLBText(int nIndex, int nColIndex, CString& rString) const
{
	if(nIndex < GetCount() && nColIndex < m_nColumnCount && nIndex >= 0 && nColIndex >=0 )
	{
		COXRowData* pRowData = GetRowData(nIndex);
		if (pRowData != NULL)
		{
			rString = pRowData->GetColumnString(nColIndex);
			return;
		}
		else
			TRACE0("In COXMultiComboBox::GetLBText : GetRowData() returned NULL.\n");
	}

	rString.Empty();
}

int COXMultiComboBox::GetLBTextLen(int nIndex,int nColIndex) const
{
	if(nIndex >= GetCount() || nColIndex >= m_nColumnCount || nColIndex < 0 || nIndex < 0)
		return CB_ERR;

	COXRowData* pRowData = GetRowData(nIndex);
	if (pRowData != NULL)
		return pRowData->GetColumnString(nColIndex).GetLength();
	else
	{
		TRACE0("In COXMultiComboBox::GetLBTextLen : GetRowData() returned NULL.\n");
		return CB_ERR;
	}
}

int COXMultiComboBox::FindStringExact(int nIndexStart, int nColIndex, LPCTSTR lpszFind, 
									  BOOL bCaseSensitive/*=FALSE*/) const
{
	if(nColIndex >= m_nColumnCount || nColIndex < 0)
		return CB_ERR;

	if(nIndexStart <= -1)
		nIndexStart = 0;

	int nItemCount = GetCount();
	for(int nIndex = nIndexStart; nIndex < nItemCount; nIndex++)
	{
		COXRowData* pRowData = GetRowData(nIndex);
		if (pRowData != NULL)
		{
			if(bCaseSensitive)
			{
				if(pRowData->GetColumnString(nColIndex) == lpszFind)
					return nIndex;
			}
			else
			{
				if(pRowData->GetColumnString(nColIndex).CompareNoCase(lpszFind) == 0)
					return nIndex;
			}
		}
		else
			TRACE0("In COXMultiComboBox::FindStringExact : GetRowData() returned NULL.\n");
	}

	return CB_ERR;
}

int COXMultiComboBox::FindString(int nStartAfter, int nColIndex, 
								 LPCTSTR lpszString, 
								 BOOL bCaseSensitive/*=FALSE*/) const
{
	if(nColIndex >= m_nColumnCount || nColIndex < 0)
		return CB_ERR;

	if(nStartAfter < -1)
		nStartAfter = -1;

	int nItemCount = GetCount();
	int nStrLen = (int)_tcslen(lpszString);

	if(nStrLen==0)
	{
		TRACE(_T("COXMultiComboBox::FindString : zero-length string was specified\n"));
		return CB_ERR;
	}

	for(int nIndex = nStartAfter+1; nIndex < nItemCount; nIndex++) 
	{
		COXRowData* pRowData = GetRowData(nIndex);
		if (pRowData != NULL)
		{
			if(bCaseSensitive)
			{
				if(pRowData->GetColumnString(nColIndex).Left(nStrLen) == lpszString)
					return nIndex;
			}
			else
			{
				if(pRowData->GetColumnString(nColIndex).Left(nStrLen).
					CompareNoCase(lpszString) == 0)
					return nIndex;
			}
		}
		else
			TRACE0("In COXMultiComboBox::FindString : GetRowData() returned NULL.\n");
	}

	return CB_ERR;
}


int COXMultiComboBox::InsertString(int nIndex, LPCTSTR* lpszString, int nNumStrings)
{
	int nRetVal;

	// return CB_ERR if invalid number of strings
	if(nNumStrings > m_nColumnCount || nNumStrings < 1)
		return CB_ERR;

	// Actually Inserts the item with master column text
	if((nRetVal = CComboBox::InsertString(nIndex,lpszString[m_nMasterColumn])) != CB_ERR)
	{
		COXRowData* pRowData = new COXRowData(m_nColumnCount);
		// Initialize the column strings
		for(int nColIndex=0; nColIndex < nNumStrings; nColIndex++)
			pRowData->SetColumnString(nColIndex,lpszString[nColIndex]);
		SetRowData(nRetVal,pRowData);
		AdjustToFitSize();
	}

	return nRetVal;
}

int COXMultiComboBox::AddString(LPCTSTR* lpszString, int nNumStrings) 
{
	int nRetVal;

	// return CB_ERR if invalid number of strings
	if(nNumStrings > m_nColumnCount || nNumStrings < 1)
		return CB_ERR;

	// Adds the item with master column text
	if((nRetVal = CComboBox::AddString(lpszString[m_nMasterColumn])) != CB_ERR)
	{
		COXRowData* pRowData = new COXRowData(m_nColumnCount);
		// Initialize the column strings
		for(int nColIndex=0; nColIndex < nNumStrings; nColIndex++)
			pRowData->SetColumnString(nColIndex,lpszString[nColIndex]);
		SetRowData(nRetVal,pRowData);
		AdjustToFitSize();
	}

	return nRetVal;
}

int COXMultiComboBox::SelectString(int nStartAfter, int nColIndex, LPCTSTR lpszString)
{
	if(nColIndex >= m_nColumnCount || nColIndex < 0)
		return CB_ERR;

	int nIndex = FindStringExact(nStartAfter,nColIndex,lpszString);
	if(nIndex != CB_ERR)
	{
		COXRowData* pRowData = GetRowData(nIndex);
		if (pRowData != NULL)
			nIndex = CComboBox::SelectString(nStartAfter,pRowData->GetColumnString(m_nMasterColumn));
		else
		{
			TRACE0("In COXMultiComboBox::SelectString : GetRowData() returned NULL.\n");
			nIndex = CB_ERR;
		}
	}

	return nIndex;
}

int COXMultiComboBox::AddString(LPCTSTR lpszString)
{
	int nRetVal;

	if((nRetVal = CComboBox::AddString(lpszString)) != CB_ERR)
	{
		COXRowData* pRowData = new COXRowData(m_nColumnCount);
		pRowData->SetColumnString(m_nMasterColumn,lpszString);
		SetRowData(nRetVal,pRowData);
		AdjustToFitSize();
	}
	return nRetVal;

}

int COXMultiComboBox::InsertString(int nIndex, LPCTSTR lpszString)
{
	int nRetVal;

	if((nRetVal = CComboBox::InsertString(nIndex,lpszString)) != CB_ERR)
	{
		COXRowData* pRowData = new COXRowData(m_nColumnCount);
		pRowData->SetColumnString(m_nMasterColumn,lpszString);
		SetRowData(nRetVal,pRowData);
		AdjustToFitSize();
	}
	return nRetVal;
}

//Protected 

// This is handled to divide the drawing into individual columns. 
// Advised to handle 'DrawColumnItem' than handling this itself  in derived 
// classes unless you need to customize the lists more than just dividing 
// into column drawings
void COXMultiComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
//	effect : Number of calls to DrawColumnItems are made
{

	ASSERT(lpDrawItemStruct->CtlType==ODT_COMBOBOX);

	// In case of drawing the static Text area of dropdown-list combobox
	if(lpDrawItemStruct->itemState & ODS_COMBOBOXEDIT)
	{
		DrawColumnItem(lpDrawItemStruct->hDC,GetCurSel(),m_nMasterColumn,lpDrawItemStruct->rcItem,lpDrawItemStruct->itemState);
		return;
	}

	if(lpDrawItemStruct->itemID==-1 && GetCount() == 0)
	{
		DrawFocusRect(lpDrawItemStruct->hDC,&(lpDrawItemStruct->rcItem));
		return;
	}

	// Draw columns one by one until last but one
	CRect rectColumn = lpDrawItemStruct->rcItem;
	int nColIndex=0;
	for(nColIndex=0; nColIndex< m_nColumnCount-1; nColIndex++)
	{
		rectColumn.right = rectColumn.left + m_aColumnWidth[nColIndex]-1;
		DrawColumnItem(lpDrawItemStruct->hDC,lpDrawItemStruct->itemID,
			nColIndex,rectColumn,lpDrawItemStruct->itemState);
		rectColumn.left = rectColumn.right+1;
	}

	// draws last column
	rectColumn.right = __max(lpDrawItemStruct->rcItem.right,
		rectColumn.left + m_aColumnWidth[nColIndex]-1);

	DrawColumnItem(lpDrawItemStruct->hDC,lpDrawItemStruct->itemID,
		nColIndex,rectColumn,lpDrawItemStruct->itemState);
}

//This virtual function is called to draw the individual columns. In the case of
// non-CBS_HASSTRINGS this has to be implimented in derived classes
void COXMultiComboBox::DrawColumnItem(HDC hDC, int nIndex, int nColIndex, const CRect& rectColumn, int ItemState)
// --- In  : hDC : the DC into which to draw the column
//			 nIndex : The row index
//			 nColIndex : The Column Index
//			 rectColumn : The rectangle into which we can draw
//			 ItemState : Gives the state of the item(Selected or not)
// --- Out : 
// --- Returns : 
// --- Effect : The column text is drawn in the case of CBS_HASSTRINGS
{
	ASSERT(GetStyle()&CBS_HASSTRINGS);

	CDC dc;
	dc.Attach(hDC);
	// Sets the foreground color of the text based on its selection state
	COLORREF clrOldForeground = dc.SetTextColor(::GetSysColor(IsWindowEnabled() ? 
		(ItemState&ODS_SELECTED ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT) : 
		COLOR_GRAYTEXT)); 

	COLORREF clrBackground = ::GetSysColor(IsWindowEnabled() ? (ItemState & ODS_SELECTED ? 
COLOR_HIGHLIGHT : COLOR_WINDOW) : COLOR_BTNFACE); 

	// Sets the background color of text based on selection state
	COLORREF clrOldBackground = dc.SetBkColor( clrBackground);

	// Gets  the column text to draw
	CString strText;
	if(nIndex!=-1)
		GetLBText(nIndex,nColIndex,strText);
	else
		strText.Empty();

	CRect cellRect(rectColumn);
	dc.FillSolidRect(cellRect, clrBackground);

	// give left margin
	cellRect.left +=  LOWORD(GetDialogBaseUnits()) / 2;
	// Draw text
	dc.DrawText(strText, cellRect, DT_NOPREFIX | DT_SINGLELINE |
		DT_VCENTER | DT_END_ELLIPSIS);

	// Seperators are drawn in list boxe between columns and rows
	DrawColumnBorder(&dc,rectColumn,ItemState);

	// Reset the actual DC colors
	dc.SetTextColor(clrOldForeground); 
	dc.SetBkColor(clrOldBackground); 
	dc.Detach();
}

//override this function to draw you own border
void COXMultiComboBox::DrawColumnBorder(CDC* pDC,const CRect& rectColumn,int ItemState)
// --- In  : pDC : the pointer to the DC into which to draw the column border
//			 rectColumn : The rectangle into which we can draw
//			 ItemState : Gives the state of the item(Selected or not)
// --- Out : 
// --- Returns : 
// --- Effect : The column borderis drawn like ' _______| '
{
	if(!(ItemState & ODS_COMBOBOXEDIT))
	{
		CPen* pOldPen = pDC->SelectObject(&m_GridPen);
		CPoint ptOld = pDC->MoveTo(rectColumn.right,rectColumn.top);
		pDC->LineTo(rectColumn.right,rectColumn.bottom-1);
		pDC->LineTo(rectColumn.left,rectColumn.bottom-1);
		pDC->MoveTo(ptOld);
		pDC->SelectObject(pOldPen);
	}
}

void COXMultiComboBox::MeasureItem(LPMEASUREITEMSTRUCT /* lpMeasureItemStruct */)
{
	// In case of CBS_OWNERDRAWNVARIABLE,to be handle in derived classes
	// to get the size of items

	// The documentation of CComboBox::MeasureItem()  states
	// "If the combo box is created with the CBS_OWNERDRAWVARIABLE style, 
	//  the framework calls this member function for each item in the list box. 
	//  Otherwise, this member is called only once."
	// So even with CBS_OWNERDRAWFIXED this function is called once.
	// In this case we do nothing

	return;
}

int COXMultiComboBox::CompareItem(LPCOMPAREITEMSTRUCT /* lpCompareItemStruct */)
{
	// In case of non-CBS_HASSTRINGS, to be handle in derived classes
	// to compare the items
	ASSERT(FALSE);
	return 0;
}



/////////////////////////////////////////////////////////////////////////////
// COXMultiComboBox message handlers

//This is handle to delete rowdata.
void COXMultiComboBox::OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{
	if(!m_fMasterColumnChanging && lpDeleteItemStruct->CtlType == ODT_LISTBOX)
	{
		COXRowData* pRowData = GetRowData(lpDeleteItemStruct->itemID);
		if (pRowData != NULL)
		{
			pRowData->SetColumnCount(0);
			delete pRowData;
		}
		else
			TRACE0("In COXMultiComboBox::OnDeleteItem : GetRowData() returned NULL.\n");
	}

	CComboBox::OnDeleteItem(nIDCtl, lpDeleteItemStruct);
}

// This message is handle to get the HWND of the listbox of the combobox
// as there is no otherway to get it(refer to Q65881, Q131845 in Knowledge Base)
// The listbox handle is needed to set the scrollbar to it if needed, and to 
// handle its WM_HSCROLL message.
HBRUSH COXMultiComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
// --- In  : pDC : device context (See standard help for detaled info)
//			 pWnd : Pointer Cwnd obejct of the window whose background 
//				    has to be painted.
//			 nCtlColor : Type of the control
// --- Out : 
// --- Returns : Brush to supply to paint the background
// --- Effect : The listbox's HWND is got and sunclassed to COXComboLBox 
//				window object. The listbox style is modified to 
//				have WS_HSCROLL. The horizontal extent of the listbox is 
//				set and scrollbars are shown are hided depending on the 
//				width and height. Scroll range for horizantal scrollbar 
//				is set and also m_NumCharsPerPage is set.
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor); 

	// Handle only if the type of control is Listbox only
	if ((nCtlColor == CTLCOLOR_LISTBOX) && m_fSizeChanged)
	{
		// ... To stop this code run unneccessorily
		m_fSizeChanged = FALSE; 
		if(m_ComboLBox.GetSafeHwnd()==NULL)
		{
			m_ComboLBox.SubclassWindow(pWnd->GetSafeHwnd()); 
			m_ComboLBox.ModifyStyle(0,WS_HSCROLL); 
			TEXTMETRIC tm;
			GetTextMetrics(pDC->m_hDC,&tm); 
			m_nLineWidth = tm.tmAveCharWidth;
		}
		int nTotalWidth = GetTotalWidth();
		// ... Sets horizontal extent to total width
		m_ComboLBox.SetHorizontalExtent(nTotalWidth);

		CRect rcDrop;
		GetDroppedControlRect(&rcDrop);
		int iTotalItemHeight = 0;
		for (int i = 0; i < m_ComboLBox.GetCount(); i++)
			iTotalItemHeight += m_ComboLBox.GetItemHeight(i);
		if (rcDrop.Height() < iTotalItemHeight || m_bFitToSize)
			m_ComboLBox.AdjustHScroll();

		if(GetStyle()&CBS_DROPDOWN)
		{
			CRect rcWnd;
			m_ComboLBox.GetWindowRect(&rcWnd);
			m_ComboLBox.MoveWindow(&rcWnd,TRUE);
		}

		if(m_ComboLBox.m_fHorzScrollVisible)
		{
			CRect rcClient;
			m_ComboLBox.GetClientRect(&rcClient);
			m_nPageWidth = rcClient.Width();

			// setting sroll info
			SCROLLINFO si;
			si.cbSize = sizeof(si);
			si.nPos = 0;
			si.nMin = 0;
			si.nMax = nTotalWidth;
			si.nPage = m_nPageWidth;
			si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS ;
			m_ComboLBox.SetScrollInfo(SB_HORZ,&si,TRUE); 
			m_ComboLBox.SendMessage(WM_HSCROLL,SB_TOP,0L); 
		}

		m_ComboLBox.RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_FRAME);
	}
	return hbr; 
}


BEGIN_MESSAGE_MAP(COXMultiComboBox::COXComboLBox, CListBox)
	//{{AFX_MSG_MAP(COXMultiComboBox::COXComboLBox)
	ON_MESSAGE(WM_HSCROLL,COXMultiComboBox::COXComboLBox::OnHScroll)
	ON_MESSAGE(WM_WINDOWPOSCHANGING,COXMultiComboBox::COXComboLBox::OnWindowPosChanging)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXComboLBox message handlers

LRESULT COXMultiComboBox::COXComboLBox::OnHScroll(WPARAM wParam, LPARAM lParam) 
// --- In  : wParam : Hiword contains scroll position and loward scroll code.
//			 lParam : the handle of scrollbar control 
// --- Out : 
// --- Returns : Has to return 0 to stop from processing further
// --- Effect : A WM_OX_LISTHSCROLL message is sent to Combobox Window
{
	//Sends a message to combo box to notify it to change scroll position
	//If the message is not processes in Combo it will be sent to window 
	//proc of listbox
	if(!m_pWndCombo->SendMessage(WM_OX_LISTHSCROLL,wParam,lParam))
		return DefWindowProc(WM_HSCROLL,wParam,lParam);
	else 
		// ... to stop further processing
		return 0l; 
}

LRESULT COXMultiComboBox::OnListHScroll(WPARAM wParam,LPARAM /* lParam */)
// --- In  : wParam : Hiword contains scroll position and loward scroll code.
//			 lParam : the handle of scrollbar control 
// --- Out : 
// --- Returns : Has to return 0 to stop from processing further
// --- Effect : The scrollbar thumb positions are set 
{
	int nSBCode	= (int) LOWORD(wParam);
	int nPos = (short int) HIWORD(wParam);
	CRect Rect;	
	switch(nSBCode)
	{
	case SB_LINELEFT : 
		// Decrease the position by one char
		m_ComboLBox.SetScrollPos(SB_HORZ,m_ComboLBox.GetScrollPos(SB_HORZ)-m_nLineWidth,TRUE);
		return 0l;
	case SB_LINERIGHT : 
		// Increase the position by one char
		m_ComboLBox.SetScrollPos(SB_HORZ,m_ComboLBox.GetScrollPos(SB_HORZ)+m_nLineWidth,TRUE);
		return 0l;
	case SB_PAGELEFT :
		// Decrease the position by 2/3rds of Number of chasr per page
		m_ComboLBox.SetScrollPos(SB_HORZ,m_ComboLBox.GetScrollPos(SB_HORZ)-((2*m_nPageWidth)/3),TRUE);
		return 0l;
	case SB_PAGERIGHT :
		// Increase the position by 2/3rds of Number of chasr per page
		m_ComboLBox.SetScrollPos(SB_HORZ,m_ComboLBox.GetScrollPos(SB_HORZ)+((2*m_nPageWidth)/3),TRUE);
		return 0l;
	case SB_THUMBTRACK : 
		m_ComboLBox.SetScrollPos(SB_HORZ,nPos,TRUE);
		return 0l;
	} 
	return 0l;
}


//this message to handle to control the height of the window depending on 
//the visibility of the horizontal scrollbar when ever it is shown
//Horz Scrollbar Visibility is decided by the difference between 
//the WindowRect and ClentRect
LRESULT COXMultiComboBox::COXComboLBox::OnWindowPosChanging(WPARAM wParam, 
															LPARAM lParam)
{	

	LPWINDOWPOS lpWP = (LPWINDOWPOS) lParam;	

	if( (!(lpWP->flags & SWP_NOSIZE))) 
	{

		int nTotalHeight = ((COXMultiComboBox*)m_pWndCombo)->GetTotalHeight();
		int nBorderHeight = GetSystemMetrics(SM_CYBORDER);
		int nScrollbarHeight = GetSystemMetrics(SM_CYHSCROLL);
		CRect rcDrop;
		((COXMultiComboBox*)m_pWndCombo)->GetDroppedControlRect(&rcDrop);

		//Check and see if listbox appears
		//in a random position and if so, fix it - Nish, Feb 24, 2005
		int orig_cy = lpWP->cy;
		if(m_fHorzScrollVisible)
		{
			if(nTotalHeight+nScrollbarHeight+2*nBorderHeight<=rcDrop.Height())
				lpWP->cy = nTotalHeight+nScrollbarHeight + 2*nBorderHeight;
			else
				lpWP->cy = GetMaxHeight();
		}
		else
		{
			if(nTotalHeight + 2*nBorderHeight <= rcDrop.Height())
				lpWP->cy = nTotalHeight + 2*nBorderHeight;
			else
				lpWP->cy = GetMaxHeight();
		}
		//Fixing listbox position - Nish
		if(orig_cy != lpWP->cy)
		{
			CRect rCbEdit;
			m_pWndCombo->GetWindowRect(&rCbEdit);				
			lpWP->y = lpWP->y + (orig_cy - lpWP->cy);
			if(lpWP->y > rCbEdit.bottom)
			{
				lpWP->y = rCbEdit.bottom;
			}
			else {
				if(lpWP->y < rCbEdit.bottom) {
					lpWP->y = rCbEdit.bottom;
				}
			}

		}			
	}				

	// check if the right side of the combo list box is out of screen coordinates
	int nScreenWidth=::GetSystemMetrics(SM_CXSCREEN);
	if(lpWP->x+lpWP->cx>nScreenWidth)
	{
		lpWP->x=nScreenWidth-lpWP->cx;
		lpWP->x=lpWP->x>0 ? lpWP->x : 0;

		// remove SWP_NOMOVE flag
		lpWP->flags&=~SWP_NOMOVE;
	}	

	return DefWindowProc(WM_WINDOWPOSCHANGING,wParam,lParam);

}

//To on and off the scrollbars of the listbox
void COXMultiComboBox::COXComboLBox::AdjustHScroll()
{
	int nTotalHeight = ((COXMultiComboBox*)m_pWndCombo)->GetTotalHeight();
	int nTotalWidth = ((COXMultiComboBox*)m_pWndCombo)->GetTotalWidth();
	int nBorderWidth = GetSystemMetrics(SM_CXBORDER);
	int nBorderHeight = GetSystemMetrics(SM_CYBORDER);
	int nScrollbarWidth = GetSystemMetrics(SM_CXVSCROLL);
	int nScrollbarHeight = GetSystemMetrics(SM_CYHSCROLL);
	CRect rcDrop;
	((COXMultiComboBox*)m_pWndCombo)->GetDroppedControlRect(&rcDrop);
	if( (nTotalHeight > rcDrop.Height()-2*nBorderHeight) ||
		((nTotalWidth > rcDrop.Width() - 2*nBorderWidth) &&
		(nTotalHeight > rcDrop.Height()-nScrollbarHeight - 2*nBorderHeight)) )
		m_fVertScrollVisible = TRUE;
	else
		m_fVertScrollVisible = FALSE;

	int nMargin=nTotalWidth-rcDrop.Width()+2*nBorderWidth;
	if(m_fVertScrollVisible)
		nMargin+=nScrollbarWidth;
	if( nMargin>0 )
		m_fHorzScrollVisible = TRUE;
	else
		m_fHorzScrollVisible = FALSE;

	if(m_fHorzScrollVisible)
		ShowScrollBar(SB_BOTH,TRUE);
	else
		ShowScrollBar(SB_HORZ,FALSE);
	ShowScrollBar(SB_VERT,m_fVertScrollVisible);

}

// The following five message handlers are defined to replace the Itemdata which we are using 
// to store the COXRowData with the actual item data the user set
LRESULT COXMultiComboBox::OnGetItemData(WPARAM wParam,LPARAM lParam)
{
	LRESULT result = DefWindowProc(CB_GETITEMDATA, wParam, lParam);

	// Replace the value by the correct one (own data structure)
	if (result != CB_ERR)
	{
		COXRowData* pRowData = (COXRowData*)result;
		ASSERT(AfxIsValidAddress(pRowData, sizeof(COXRowData)));
		result = pRowData->GetItemData();
	}
	return result;
}

LRESULT COXMultiComboBox::OnSetItemData(WPARAM wParam,LPARAM lParam)
{
	LRESULT result = DefWindowProc(CB_GETITEMDATA, wParam, lParam);

	// Replace the value by the correct one (own data structure)
	if (result != CB_ERR)
	{
		COXRowData* pRowData = (COXRowData*)result;
		ASSERT(AfxIsValidAddress(pRowData, sizeof(COXRowData)));
		pRowData->SetItemData((DWORD)lParam);
	}
	return result;
}

LRESULT COXMultiComboBox::OnCompareItem(WPARAM wParam,LPARAM lParam)
{
	LPCOMPAREITEMSTRUCT pCompareItemStruct = (LPCOMPAREITEMSTRUCT)lParam;
	COXRowData* pRowData;
	if(pCompareItemStruct->itemID1!=-1)
	{
		pRowData = (COXRowData*)pCompareItemStruct->itemData1;
		ASSERT(AfxIsValidAddress(pRowData, sizeof(COXRowData)));
		pCompareItemStruct->itemData1 = pRowData->GetItemData();
	}
	if(pCompareItemStruct->itemID2!=-1)
	{
		pRowData = (COXRowData*)pCompareItemStruct->itemData2;
		ASSERT(AfxIsValidAddress(pRowData, sizeof(COXRowData)));
		pCompareItemStruct->itemData2 = pRowData->GetItemData();
	}
	return DefWindowProc(WM_COMPAREITEM,wParam,lParam);
}

LRESULT COXMultiComboBox::OnMeasureItem(WPARAM wParam,LPARAM lParam)
{
	LPMEASUREITEMSTRUCT pMeasureItemStruct = (LPMEASUREITEMSTRUCT)lParam;
	// ... Ignore if CBS_OWNERDRAWFIXED
	if( (pMeasureItemStruct->itemID!=-1) && 
		((GetStyle() & CBS_OWNERDRAWFIXED) != CBS_OWNERDRAWFIXED) )
	{
		COXRowData* pRowData = (COXRowData*)(pMeasureItemStruct->itemData);
		ASSERT(AfxIsValidAddress(pRowData, sizeof(COXRowData)));
		pMeasureItemStruct->itemData = pRowData->GetItemData();
	}
	return DefWindowProc(WM_MEASUREITEM,wParam,lParam);
}

LRESULT COXMultiComboBox::OnDrawItem(WPARAM wParam,LPARAM lParam)
{
	LPDRAWITEMSTRUCT pDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
	if(pDrawItemStruct->itemID!=-1)
	{
		COXRowData* pRowData = (COXRowData*)pDrawItemStruct->itemData;
		ASSERT(AfxIsValidAddress(pRowData, sizeof(COXRowData)));
		pDrawItemStruct->itemData = pRowData->GetItemData();
	}
	return DefWindowProc(WM_DRAWITEM,wParam,lParam);
}

//CB_ADDSTRING and CB_INSERTSTRING are handled to notify that Size is changed
LRESULT COXMultiComboBox::OnAddString(WPARAM wParam,LPARAM lParam)
{

	m_fSizeChanged = TRUE;
	//if(::IsWindow(m_ComboLBox.m_hWnd))
	//	m_ComboLBox.InvalidateRect(NULL);
	return DefWindowProc(CB_ADDSTRING,wParam,lParam);
}

LRESULT COXMultiComboBox::OnInsertString(WPARAM wParam,LPARAM lParam)
{
	m_fSizeChanged = TRUE;
	//	if(::IsWindow(m_ComboLBox.m_hWnd))
	//		m_ComboLBox.InvalidateRect(NULL);
	return DefWindowProc(CB_INSERTSTRING,wParam,lParam);
}

int COXMultiComboBox::GetTotalHeight()
// --- In  : 
// --- Out : 
// --- Returns : returns the sum of hieghts of all the items in the combobox
// --- Effect : 
{
	int nTotalHeight =0;
	int nCount = GetCount();
	if(!(GetStyle()&CBS_OWNERDRAWVARIABLE))
		nTotalHeight = nCount * GetItemHeight(0);
	else
		for(int nIndex=0; nIndex < nCount; nIndex++)
			nTotalHeight+=GetItemHeight(nIndex);
	return nTotalHeight;
}

int COXMultiComboBox::GetTotalWidth() 
// --- In  : 
// --- Out : 
// --- Returns : Returns the sum of widths of all columns
// --- Effect : 
{
	int nTotalWidth =0;
	int nColumnCount = GetColumnCount();
	for(int nColIndex=0; nColIndex < nColumnCount; nColIndex++)
		nTotalWidth += GetColumnWidth(nColIndex);
	return nTotalWidth;
}

BOOL COXMultiComboBox::SetRowData(int nIndex, COXRowData* pRowData)
// --- In  : Index whose Rowdata has to be get
//			 pRowData : RowData to set to the item
// --- Out : 
// --- Returns : TRUE if successfull
// --- Effect : to set the RowData to an Item
{
	if(DefWindowProc(CB_SETITEMDATA,nIndex,(DWORD)PtrToUint(pRowData))==CB_ERR)
		return FALSE;
	else
		return TRUE;
}

COXMultiComboBox::COXRowData* COXMultiComboBox::GetRowData(int nIndex) const
// --- In  : Index whose Rowdata has to be get
// --- Out : 
// --- Returns : Returns row data if pointer to sucessfull, otherwise NULL.
// --- Effect : 
{
	LRESULT lResult; 
	//	casting is to convert this from 'const COXMultiComboBox*' to 
	//	'COXMultiComboBox*'
	lResult = ((COXMultiComboBox*)this)->DefWindowProc(CB_GETITEMDATA, nIndex, 0l);
	if(lResult!=CB_ERR)
		return (COXRowData*)lResult;
	else
	{
		TRACE0("In COXMultiComboBox::GetRowData : GetItemData() returned CB_ERR. Returning NULL.\n");
		return NULL;
	}
}

//Private
BOOL COXMultiComboBox::ChangeMasterColumn(int /* nCol */)
{
	int nNumItems = GetCount();
	CArray<COXRowData*,COXRowData*> aPtrsRowData; 

	aPtrsRowData.SetSize(nNumItems);
	int nIndex=0;
	for(nIndex=0; nIndex < nNumItems; nIndex++)
	{
		COXRowData* pRowData = GetRowData(nIndex);
		aPtrsRowData[nIndex] = pRowData;
		if (pRowData == NULL)
			TRACE0("In COXMultiComboBox::ChangeMasterColumn : GetRowData() returned NULL.\n");
	}

	// ... To avoid deleting the Rowdata in DeleteItem
	m_fMasterColumnChanging = TRUE; 
	// ... Delete all the Items
	ResetContent();		
	// ... Resets the flag
	m_fMasterColumnChanging = FALSE; 

	int nRetVal;
	//	Again add all the items. This deletion and addition is to effect 
	//	the sorting order based on current master column
	for(nIndex=0; nIndex < nNumItems; nIndex++)
	{
		if((nRetVal = CComboBox::AddString((aPtrsRowData[nIndex])->GetColumnString(m_nMasterColumn))) != CB_ERR)
			SetRowData(nRetVal,aPtrsRowData[nIndex]);
		else 
			return FALSE;
	}

	return TRUE;
}

int COXMultiComboBox::COXComboLBox::GetMaxHeight()
{
	CRect rcDrop;
	((COXMultiComboBox*)m_pWndCombo)->GetDroppedControlRect(&rcDrop);
	int nScrollHeight = m_fHorzScrollVisible ? GetSystemMetrics(SM_CYHSCROLL) : 0;
	int nBorderHeight = GetSystemMetrics(SM_CYBORDER);
	int nTmpHeight = rcDrop.Height()-2*nBorderHeight-nScrollHeight;
	int nAbsHeight;
	if(((COXMultiComboBox*)m_pWndCombo)->GetStyle() & CBS_OWNERDRAWVARIABLE)
	{
		nAbsHeight = 0;
		int nCount = ((COXMultiComboBox*)m_pWndCombo)->GetCount();
		int nItemHeight;
		for(int nIndex=0; nIndex < nCount; nIndex++)
		{
			nItemHeight = ((COXMultiComboBox*)m_pWndCombo)->GetItemHeight(nIndex);
			if(nAbsHeight + nItemHeight <= nTmpHeight)
				nAbsHeight += nItemHeight;
			else
				break;
		}
	}
	else
		nAbsHeight = nTmpHeight - nTmpHeight%((COXMultiComboBox*)m_pWndCombo)->GetItemHeight(0);
	return nAbsHeight + 2*nBorderHeight + nScrollHeight;
}

void COXMultiComboBox::SetFitToSize(BOOL bFitToSize)
{
	m_bFitToSize=bFitToSize;
	AdjustToFitSize();
}

int COXMultiComboBox::AdjustToFitSize()
{
	int nResult;
	if(m_bFitToSize)
	{
		int nTotalWidth=GetTotalWidth()+2*GetSystemMetrics(SM_CXBORDER);

		int nTotalHeight=GetTotalHeight();
		int nBorderHeight=GetSystemMetrics(SM_CYBORDER);
		int nScrollbarHeight=GetSystemMetrics(SM_CYHSCROLL);
		CRect rcDrop;
		GetDroppedControlRect(&rcDrop);
		if((nTotalHeight > rcDrop.Height()-2*nBorderHeight) ||	
			((nTotalWidth > rcDrop.Width()) &&
			(nTotalHeight > rcDrop.Height()-nScrollbarHeight-2*nBorderHeight)))
		{
			nTotalWidth+=::GetSystemMetrics(SM_CXVSCROLL);
		}

		int nScreenWidth=::GetSystemMetrics(SM_CXSCREEN);
		nTotalWidth=nTotalWidth>nScreenWidth ? nScreenWidth : nTotalWidth;

		nResult=SetDroppedWidth(nTotalWidth);
	}
	else
		nResult=SetDroppedWidth(1);

	if(nResult!=CB_ERR)
	{
		// ... To indicate that the scrollbars has to be re-adjusted
		m_fSizeChanged = TRUE; 

		// Redraws the listbox
		if(::IsWindow(m_ComboLBox.m_hWnd))
		{
			m_ComboLBox.RedrawWindow(NULL,NULL,
				RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW|RDW_UPDATENOW|RDW_FRAME);
		}
	}

	return nResult;
}

