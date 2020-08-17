// ==========================================================================
// 					Class Implementation : COXEditList
// ==========================================================================

// Source file : editlist.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "OXEditList.h"	// class specification
//#include "OXEditListRes.h"	// For toolbar
#include "commctrl.h"	// For toolbar text handler
#include <afxpriv.h>	// for WM_IDLEUPDATECMDUI
#include "UTB64Bit.h"

#ifdef _DEBUG
   #undef THIS_FILE
   static char BASED_CODE THIS_FILE[] =__FILE__;
   static char BASED_CODE _FILE_NAME_[]="OXEditList";
#else
   #define  _FILE_NAME_  __FILE__
#endif

#define new DEBUG_NEW

// Determine number of elements in an array (not bytes)
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif // _countof



/////////////////////////////////////////////////////////////////////////////
// COXEditListHeader

COXEditListHeader::COXEditListHeader()
{
	// set default values
	VERIFY(SetTextColor());
	VERIFY(SetBorderColors());
	SetVertOriented(FALSE);
	m_sText.Empty();
}

COXEditListHeader::~COXEditListHeader()
{
}


BEGIN_MESSAGE_MAP(COXEditListHeader, CStatic)
	//{{AFX_MSG_MAP(COXEditListHeader)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_SETTEXT,OnSetText)
	ON_MESSAGE(WM_GETTEXT,OnGetText)
	ON_MESSAGE(WM_GETTEXTLENGTH,OnGetTextLength)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXEditListHeader message handlers

void COXEditListHeader::PreSubclassWindow() 
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

int COXEditListHeader::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

void COXEditListHeader::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	CRect rectClient;
	GetClientRect(rectClient);

	// draw border
	dc.Draw3dRect(rectClient,m_clrTopLeft,m_clrBottomRight);
	
	// draw text
	//

	if(!m_sText.IsEmpty())
	{
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

		CRect rectText(0,0,0,0);

		rectClient.DeflateRect(2,2);
		dc.IntersectClipRect(rectClient);

		rectText=rectClient;
		CRect rectHelper;
		if(m_bVertOriented)
		{
			// adjust rectangle to display verical text
			rectHelper=rectText;
			rectText.top=rectHelper.left;
			rectText.bottom=rectHelper.right;
			rectText.left=rectHelper.top;
			rectText.right=rectHelper.bottom;
		}

		// calculate the rect to display text in
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
				(rectClient.Width()-rectHelper.Width())%2;
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
			if(m_bVertOriented)
			{
				rectText.top+=(rectClient.Height()-rectHelper.Height())/2+
					(rectClient.Height()-rectHelper.Height())%2;
				rectText.bottom=rectText.top+rectHelper.Height();
			}
			else
			{
				rectText.left+=(rectClient.Width()-rectHelper.Width())/2;
				rectText.right=rectText.left+rectHelper.Width();
			}
			break;
		// SS_RIGHT
		case 2:
			if(m_bVertOriented)
			{
				rectText.bottom=rectClient.bottom;
				rectText.top=rectText.bottom-rectHelper.Height();
			}
			else
			{
				rectText.right=rectClient.right;
				rectText.left=rectText.right-rectHelper.Width();
			}
			break;	
		// SS_LEFT
		default:
			if(m_bVertOriented)
			{
				rectText.top=rectClient.top;
				rectText.bottom=rectText.top+rectHelper.Height();
			}
			else
			{
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

		if(pOldFont!=NULL)
			dc.SelectObject(pOldFont);
	}

	// Do not call CStatic::OnPaint() for painting messages
}

LONG COXEditListHeader::OnSetText(UINT wParam, LONG_PTR lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	// save the window text
	m_sText=(LPCTSTR)lParam;

	RedrawWindow();

	return TRUE;
}

LONG COXEditListHeader::OnGetText(UINT wParam, LONG lParam)
{
	if(wParam>0)
	{
		wParam=wParam>(UINT)m_sText.GetLength()+1 ? 
			m_sText.GetLength()+1 : wParam;
		VERIFY(lstrcpyn((LPTSTR)(LONG_PTR)lParam,
			m_sText,wParam-1)!=NULL);
	}
	return (LONG)wParam;
}

LONG COXEditListHeader::OnGetTextLength(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return (LONG)m_sText.GetLength();
}


BOOL COXEditListHeader::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	// provide background filling (we don't want it to be transparent!!!)
	CRect rectClient;
	GetClientRect(rectClient);
	pDC->FillSolidRect(rectClient,::GetSysColor(COLOR_BTNFACE));

	return TRUE;
}

void COXEditListHeader::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	// redraw the control while resizing it
	RedrawWindow();
}


/////////////////////////////////////////////////////////////////////////////
// Definition of static members
COXEditList::EToolbarPosition COXEditList::TBP_FIRST=COXEditList::TBPNone;
COXEditList::EToolbarPosition COXEditList::TBP_LAST=COXEditList::TBPVerticalRightBottom;

///////////////////////////////////////////////////////////////////
// Data members -------------------------------------------------------------
// protected:
	// EToolbarPosition m_eToolbarPosition;
	// --- The current position of the toolbar seen from this control

	// BOOL m_bAllowDuplicates;
	// --- Whether entries with the same name are allowed in the list
	//     (When not the user will be warned by a message box when he
	//		tries to add an already existing name

	// BOOL m_bOrderedList;
	// --- Whether the user can move an item up and down (TRUE) or not (FALSE)

	// BOOL m_bIsDeleting
	// --- Whether the control is in a delete process or not

	// CToolBar m_toolbar;
	// --- The toolbar window
	//     This window iss a sibling of this control, but this control is its owner
	//     This way it receives important messages from the toolbar

	// BOOL m_bPostInitialized;
	// --- Whether the function PostInit has been executed

	// CString m_sOriginalEditText;
	// --- The content of the label when the user started editing it

	// CString m_sDuplicateErrorMsg;
	// --- The error text that should be shown when a duplicate name is
	//     detected and is not allowed (m_bAllowDuplicates==FALSE)

	// int m_nNewImageItem;
	// --- The image index that will be assgined to a new item

// private:
	
// Member functions ---------------------------------------------------------
// public:

BEGIN_MESSAGE_MAP(COXEditList, COXGridList)
   //{{AFX_MSG_MAP(COXEditList)
	ON_NOTIFY_REFLECT_EX(LVN_ITEMCHANGED, OnItemChanged)
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT_EX(LVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT_EX(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_COMMAND(ID_OX_EDITLIST_NEW, OnNewItem)
	ON_COMMAND(ID_OX_EDITLIST_DELETE, OnDeleteItem)
	ON_UPDATE_COMMAND_UI(ID_OX_EDITLIST_DELETE, OnUpdateDeleteItem)
	ON_COMMAND(ID_OX_EDITLIST_UP, OnMoveItemUp)
	ON_UPDATE_COMMAND_UI(ID_OX_EDITLIST_UP, OnUpdateMoveItemUp)
	ON_COMMAND(ID_OX_EDITLIST_DOWN, OnMoveItemDown)
	ON_UPDATE_COMMAND_UI(ID_OX_EDITLIST_DOWN, OnUpdateMoveItemDown)
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_SYSKEYDOWN()
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipText)
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
	ON_MESSAGE(LVM_INSERTITEM, OnInsertItem)
	ON_MESSAGE(LVM_DELETEITEM, OnDeleteItem)
	ON_MESSAGE(LVM_DELETEALLITEMS, OnDeleteAllItems)
	ON_MESSAGE(WM_POST_INIT, OnPostInit)
END_MESSAGE_MAP()

COXEditList::COXEditList(EToolbarPosition eToolbarPosition /*=TBPHorizontalTopRight*/,
						 BOOL bAllowDuplicates /*=FALSE*/, BOOL bOrderedList /*=TRUE*/)
	:
	m_eToolbarPosition(eToolbarPosition),
	m_bAllowDuplicates(bAllowDuplicates),
	m_bOrderedList(bOrderedList),
	m_bPostInitialized(FALSE),
	m_sOriginalEditText(),
	m_nNewImageItem(1),
	m_sHeaderText(_T("")),
	m_bEditable(TRUE)
{
	// ... eToolbarPosition must be valid
	ASSERT(TBP_FIRST <= eToolbarPosition);
	ASSERT(eToolbarPosition <= TBP_LAST);
	// ... Use a default error message
	VERIFY(m_sDuplicateErrorMsg.LoadString(IDS_OX_EDITLISTDUPLERROR)); //"This item is already part of the list, please use another name"
	ASSERT_VALID(this);
}

void COXEditList::InitGrid()
{
	// First call bass class implementation
	COXGridList::InitGrid();

	// Create a header as sibling of this control
	VERIFY(m_header.Create(NULL,WS_CHILD,CRect(0,0,0,0),GetParent()));
	static CFont fontVertEnabled;
	if((HFONT)fontVertEnabled==NULL)
		VERIFY(fontVertEnabled.CreatePointFont(100,_T("Arial")));
	m_header.SetFont(&fontVertEnabled);


	// Create a toolbar as sibling of this control
	// ... Invisible by default, we will show it when the correct dimensions are knwon
	//     in PositionToolbar
	VERIFY(m_toolbar.Create(&m_header,WS_CHILD|CBRS_TOOLTIPS));
	// ... Must find the resource
	//     (Make sure OXEdit.rc is included in your resource file)
	ASSERT(AfxFindResourceHandle(MAKEINTRESOURCE(IDR_OX_EDITLIST_TOOLS), RT_TOOLBAR) != NULL);
	VERIFY(m_toolbar.LoadToolBar(IDR_OX_EDITLIST_TOOLS));
	m_toolbar.SetOwner(this);
	if (!m_bOrderedList)
	{
		// Not an ordered list, remove the move up and down buttons
		SetButtonCount(m_toolbar, 2);
	}
#if _MFC_VER>0x0421
	m_toolbar.SetBorders(0,0,0,0);
#else
	m_toolbar.m_cxLeftBorder=m_toolbar.m_cxRightBorder=
		m_toolbar.m_cyTopBorder=m_toolbar.m_cyBottomBorder=0;
#endif
	// Move the toolbar to the right position
	PositionToolbar(m_eToolbarPosition);

	// Because subclassing is not finished yet
	// and so our message handlers are not functional
	// we defer some initialization to later
	VERIFY(m_bPostInitialized==FALSE);
	PostMessage(WM_POST_INIT);
}

void COXEditList::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	ASSERT_VALID(this);

	// First call bass class implementation
	COXGridList::DrawItem(lpDIS);

	// Draw an extra recatangle on top of the last (empty) item
	if((int)lpDIS->itemID==GetItemCount()-1 && IsWindowEnabled())
	{
		// We draw a rectangle of half the width of the item 
		CRect rect(lpDIS->rcItem);
		if (m_nImageColumn==0)
			// ... Images in first column : move rectangle
			rect.left += GetImagesWidth();
		rect.right=rect.left + rect.Width() / 2;
		rect.InflateRect(-1, -1);
		CDC* pDC=CDC::FromHandle(lpDIS->hDC);
		pDC->DrawFocusRect(rect);
	}
}

void COXEditList::SetDuplicateErrorMsg(LPCTSTR pszDuplicateErrorMsg)
{
	m_sDuplicateErrorMsg=pszDuplicateErrorMsg;
}

BOOL COXEditList::EditNewItem() 
{
	int nLastItemIndex=GetItemCount() - 1;
	// ... Last item should have an empty label
	ASSERT(GetItemText(nLastItemIndex, 0).IsEmpty());

	// Just start editing the last item
	SetFocus();
	EnsureVisible(nLastItemIndex, 0, FALSE);
	return (EditLabel(nLastItemIndex, 0) != NULL);
}

BOOL COXEditList::CanDelete() const
{
	// Check whether any item is selected (apart from the last one)
	int nSelectedIndex=GetCurSel();
	return (0 <= nSelectedIndex) && (nSelectedIndex < GetItemCount() - 1);
}

BOOL COXEditList::DeleteSelectedItems()
{
	// Should not call this function when it is of no use
	// GUI should be disabled when necessary
	if (!CanDelete())
	{
		TRACE0("COXEditList::DeleteSelectedItems called when NOT CanDelete()\n");
		return FALSE;
	}

	BOOL bSuccess=TRUE;
	// ... Store current focus item
	int nFocusItem=GetCurFocus();

	// Delete all the selected items
	// This might also include the empty (last) item, which deletion will fail
	int nItemIndex=-1;
	// while calling deleteItem an OnChangeState is called, this function is not permitted 
	// to set the focus again a that time, so exclude this behaviour
	m_bIsDeleting=TRUE;
	while (bSuccess && 
		   (nItemIndex=GetNextItem(nItemIndex, LVNI_SELECTED)) != -1)
	{
		// ... Delete may fail for the empty item
		bSuccess=DeleteItem(nItemIndex);
		// ... Requery this index again, 
		//     because now this index is used by the next item
		nItemIndex--;
	}
	// Reset the default OnChangeState behaviour
	m_bIsDeleting=FALSE;

	// ... Restore current focus item (by first removing the focus, like this we are
	//	   sure that OnChangeState will be called to set the selection also)
	if (SetCurFocus(nFocusItem, FALSE))
		SetCurFocus(nFocusItem);
	else
	{
		// ... If it failed put focus on the last item
		// Remove focus
		SetCurFocus(GetItemCount() - 1, FALSE);
		// Set focus
		SetCurFocus(GetItemCount() - 1);
	}

	// If we tried to delete the last item, everything succeeded
	// otherwise bSuccess contains the correct value
	return (nItemIndex==GetItemCount() - 2 ? TRUE : bSuccess);
}

BOOL COXEditList::CanMoveUp() const
{
	// prevent user's ability to move items while in edit mode
	if ( GetEditControl() != NULL )
		return FALSE;

	// Check whether selected item is not the first item and not the last (empty) item
	int nFocusIndex=GetCurFocus();
	return m_bOrderedList && (0 < nFocusIndex) && (nFocusIndex < GetItemCount() - 1);
}

BOOL COXEditList::MoveItemUp()
{
	// Should not call this function when it is of no use
	// GUI should be disabled when necessary
	if (!CanMoveUp())
	{
		TRACE0("COXEditList::MoveItemUp called when NOT CanMoveUp()\n");
		return FALSE;
	}

	int nFocusIndex=GetCurFocus();
	if (SwapItems(nFocusIndex - 1, nFocusIndex))
	{
		EnsureVisible(nFocusIndex - 1, 0, TRUE);
		return TRUE;
	}
	else 
		return FALSE;
}

BOOL COXEditList::CanMoveDown() const
{
	// prevent user's ability to move items while in edit mode
	if ( GetEditControl() != NULL )
		return FALSE;

	// Check whether selected item is not the last (empty) item or that before the last
	int nFocusIndex=GetCurFocus();
	return m_bOrderedList && (0 <= nFocusIndex) && 
		(nFocusIndex < GetItemCount() - 2);
}

BOOL COXEditList::MoveItemDown()
{
	// Should not call this function when it is of no use
	// GUI should be disabled when necessary
	if (!CanMoveDown())
	{
		TRACE0("COXEditList::MoveItemDown called when NOT CanMoveDown()\n");
		return FALSE;
	}

	int nFocusIndex=GetCurFocus();
	if (SwapItems(nFocusIndex, nFocusIndex + 1))
	{
		EnsureVisible(nFocusIndex + 1, 0, TRUE);
		return TRUE;
	}
	else 
		return FALSE;
}

BOOL COXEditList::SwapItems(int nIndex1, int nIndex2)
{
	LV_ITEM lvi1;
	LV_ITEM lvi2;
	memset(&lvi1, 0, sizeof(lvi1));
	memset(&lvi2, 0, sizeof(lvi2));

	// Initialize the requested struct for both items
	// ... Get everything apart from the text (which we will get seperately)
	lvi1.mask=LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	lvi1.iItem=nIndex1;
	lvi1.iSubItem=0;
	lvi1.stateMask=0xFFFFFFFF;

	// ... Get everything apart from the text (which we will get seperately)
	lvi2.mask=LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
	lvi2.iItem=nIndex2;
	lvi2.iSubItem=0;
	lvi2.stateMask=0xFFFFFFFF;

	if (!GetItem(&lvi1) || !GetItem(&lvi2))
	{
		TRACE2("COXEditList::SwapItems : Failed to get the item %i or %i\n", nIndex1, nIndex2);
		return FALSE;
	}

	// We got the data, now lets swap them
	lvi1.iItem=nIndex2;
	lvi2.iItem=nIndex1;
	if (!SetItem(&lvi1) || !SetItem(&lvi2))
	{
		TRACE2("COXEditList::SwapItems : Failed to set the item %i or %i\n", nIndex1, nIndex2);
		return FALSE;
	}
	
	// Now we will swap the text of the label and of each subitem
	CString sText1;
	CString sText2;
	for (int nSubIndex=0; nSubIndex < GetNumCols(); nSubIndex++)
	{
		sText1=GetItemText(nIndex1, nSubIndex);
		sText2=GetItemText(nIndex2, nSubIndex);

		if (!SetItemText(nIndex2, nSubIndex, sText1) || !SetItemText(nIndex1, nSubIndex, sText2))
		{
			TRACE2("COXEditList::SwapItems : Failed to set the item text of %i or %i\n", nIndex1, nIndex2);
			return FALSE;
		}
	}

	// If we arrive here everything has gone OK
	return TRUE;
}

void COXEditList::PositionToolbar(EToolbarPosition eToolbarPosition/*=TBPHorizontalTopRight*/)
{
	ASSERT(TBP_FIRST <= eToolbarPosition);
	ASSERT(eToolbarPosition <= TBP_LAST);

	// Store new setting
	m_eToolbarPosition=eToolbarPosition;

	m_header.ShowWindow((eToolbarPosition==TBPNone) ? SW_HIDE : SW_SHOW);
	m_toolbar.ShowWindow((eToolbarPosition==TBPNone) ? SW_HIDE : SW_SHOW);
	if(eToolbarPosition==TBPNone)
	{
		// Invisible toolbar : nothing more to do
		ASSERT_VALID(this);
		return;
	}

	CRect rect;
	CRect rectToolbar(0,0,0,0);
	CRect rectHeader(0,0,0,0);
	CSize toolSize;
	CSize offsetHeader;
	CSize offsetToolbar;
	GetWindowRect(rect);
	GetParent()->ScreenToClient(rect);

	// First calculate the size of the toolbar
	BOOL bHorizontal=(eToolbarPosition==TBPHorizontalTopLeft) ||
		(eToolbarPosition==TBPHorizontalTopCenter) ||
		(eToolbarPosition==TBPHorizontalTopRight) ||
		(eToolbarPosition==TBPHorizontalBottomLeft) ||
		(eToolbarPosition==TBPHorizontalBottomCenter) ||
		(eToolbarPosition==TBPHorizontalBottomRight);

	// set the orientation of header control
	m_header.SetVertOriented(!bHorizontal);
	switch(eToolbarPosition)
	{
		// Horizontal
		case TBPHorizontalTopLeft:
		case TBPHorizontalBottomLeft:
		case TBPVerticalLeftTop:
		case TBPVerticalRightTop:
			m_header.ModifyStyle(NULL,SS_RIGHT);
			break;

		case TBPHorizontalTopCenter:
		case TBPHorizontalTopRight:
		case TBPHorizontalBottomCenter:
		case TBPHorizontalBottomRight:
		case TBPVerticalLeftCenter:
		case TBPVerticalLeftBottom:
		case TBPVerticalRightCenter:
		case TBPVerticalRightBottom:
			m_header.ModifyStyle(SS_RIGHT,NULL);
			break;

		default:
			TRACE1("COXEditList::PositionToolbar : Unexpected case in switch (%i)\n", eToolbarPosition);
			ASSERT(FALSE);
			break;
	}

	// ... Change bar style to reflect the correct orientation
	DWORD nOldStyle=m_toolbar.GetBarStyle();
	m_toolbar.SetBarStyle(bHorizontal ? (nOldStyle | CBRS_ORIENT_HORZ) : 
		(nOldStyle & ~CBRS_ORIENT_HORZ));
	// ... Get the bar size
	toolSize=m_toolbar.CalcFixedLayout(FALSE,bHorizontal);
	// adjust toolbar rect
	rectToolbar.right=rectToolbar.left + toolSize.cx;
	rectToolbar.bottom=rectToolbar.top + toolSize.cy;

	// ... Get the border size of the toolbar 
	//     If the bar is vertical m_cxLeftBorder is used to adjust the top (cy) and
	//	   m_cxRightBorder is used to adjust the bottom (cy) !
	int nLeftBorder=1;
	int nRightBorder=1;
	int nTopBorder=1;
	int nBottomBorder=1;

	// adjust header rect
	if(bHorizontal)
	{
		rectHeader.top-=nTopBorder;
		rectHeader.right=rectHeader.left+rect.Width();
		rectHeader.bottom=rectHeader.top+toolSize.cy+nTopBorder+nBottomBorder;
	}
	else
	{
		rectHeader.left-=nTopBorder;
		rectHeader.bottom=rectHeader.left+rect.Height();
		rectHeader.right=rectHeader.left+toolSize.cx+nLeftBorder+nRightBorder;
	}

	// Then adjust the position of the header and toolbar

	// ... Handle all possible cases for header
	switch(eToolbarPosition)
	{
		// Horizontal
		case TBPHorizontalTopLeft:
		case TBPHorizontalTopCenter:
		case TBPHorizontalTopRight:
			offsetHeader.cx=rect.left;
			offsetHeader.cy=rect.top - rectHeader.Height();
			break;
		case TBPHorizontalBottomLeft:
		case TBPHorizontalBottomCenter:
		case TBPHorizontalBottomRight:
			offsetHeader.cx=rect.left;
			offsetHeader.cy=rect.bottom+nTopBorder+nBottomBorder;
			break;
		// Vertical
		case TBPVerticalLeftTop:
		case TBPVerticalLeftCenter:
		case TBPVerticalLeftBottom:
			offsetHeader.cx=rect.left-rectHeader.Width();
			offsetHeader.cy=rect.top;
			break;
		case TBPVerticalRightTop:
		case TBPVerticalRightCenter:
		case TBPVerticalRightBottom:
			// ... Right (Horizontal)
			offsetHeader.cx=rect.right+nLeftBorder+nRightBorder;
			offsetHeader.cy=rect.top;
			break;
		default:
			TRACE1("COXEditList::PositionToolbar : Unexpected case in switch (%i)\n", eToolbarPosition);
			ASSERT(FALSE);
			break;
	}

	// ... Handle all possible cases for toolbar
	switch (eToolbarPosition)
	{
		// Horizontal
		case TBPHorizontalTopLeft:
		case TBPHorizontalBottomLeft:
			offsetToolbar.cx=nLeftBorder;
			offsetToolbar.cy=nTopBorder;
			break;
		case TBPHorizontalTopCenter:
		case TBPHorizontalBottomCenter:
			offsetToolbar.cx=((rectHeader.Width()>rectToolbar.Width()+
				nLeftBorder+nRightBorder) ? 
				(rectHeader.Width()-rectToolbar.Width())/2 : nLeftBorder);
			offsetToolbar.cy=nTopBorder;
			break;
		case TBPHorizontalTopRight:
		case TBPHorizontalBottomRight:
			offsetToolbar.cx=rectHeader.Width()-rectToolbar.Width()-nRightBorder;
			offsetToolbar.cy=nTopBorder;
			break;

		// Vertical
		case TBPVerticalLeftTop:
		case TBPVerticalRightTop:
			offsetToolbar.cy=nTopBorder;
			offsetToolbar.cx=nLeftBorder;
			break;
		case TBPVerticalLeftCenter:
		case TBPVerticalRightCenter:
			offsetToolbar.cy=((rectHeader.Height()>rectToolbar.Height()+
				nTopBorder+nBottomBorder) ? 
				(rectHeader.Height()-rectToolbar.Height())/2 : nTopBorder);
			offsetToolbar.cx=nLeftBorder;
			break;
		case TBPVerticalLeftBottom:
		case TBPVerticalRightBottom:
			offsetToolbar.cy=rectHeader.Height()-rectToolbar.Height()-nBottomBorder;
			offsetToolbar.cx=nLeftBorder;
			break;
		default:
			TRACE1("COXEditList::PositionToolbar : Unexpected case in switch (%i)\n", eToolbarPosition);
			ASSERT(FALSE);
			break;
	}
	rectHeader+=offsetHeader;
	rectToolbar+=offsetToolbar;

	// Reposition the header
	m_header.MoveWindow(rectHeader);
	m_header.Invalidate();
	// Reposition the toolbar
	m_toolbar.MoveWindow(rectToolbar);
	// ... If already visible : redraw
	m_toolbar.Invalidate();

	ASSERT_VALID(this);
}

COXEditList::EToolbarPosition COXEditList::GetToolbarPosition() const
{
	ASSERT_VALID(this);
	return m_eToolbarPosition;
}

BOOL COXEditList::OnIdle(LONG lCount /*=0 */)
{
	// Update the state of the GUI now
	// ... We only have to update the first time the idle loop is executed
	if (lCount==0)
	{
		SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE /* bDisableIfNoHandler */, 0);
	}

	// ... We do not need more idle processing
	return FALSE;
}

BOOL COXEditList::SortColumn(int nColumn /*=0 */)
{
	ASSERT((0 <= nColumn) && (nColumn < GetNumCols()));

	COXGridListSortInfo gridListSortInfo(this, nColumn);
	return SortItems(EditListCompareProc, (LPARAM)(&gridListSortInfo));
}

#ifdef _DEBUG
void COXEditList::AssertValid() const
{
	COXGridList::AssertValid();
	ASSERT(TBP_FIRST <= m_eToolbarPosition);
	ASSERT(m_eToolbarPosition <= TBP_LAST);
}

void COXEditList::Dump(CDumpContext& dc) const
{
	COXGridList::Dump(dc);
	dc << "\nm_eToolbarPosition : " << (LONG)m_eToolbarPosition;
	dc << "\nm_bAllowDuplicates : " << m_bAllowDuplicates;
	dc << "\nm_bOrderedList : " << m_bOrderedList;
	dc << "\nm_toolbar : " << m_toolbar;
	dc << "\nm_bPostInitialized : " << m_bPostInitialized;
	dc << "\nm_sOriginalEditText : " << m_sOriginalEditText;
	dc << "\nm_sDuplicateErrorMsg : " << m_sDuplicateErrorMsg;
	dc << "\nm_nNewImageItem : " << m_nNewImageItem;
	dc << "\n";
}
#endif //_DEBUG

COXEditList::~COXEditList()
{
	ASSERT_VALID(this);
}

BOOL COXEditList::IsFrameWnd() const
	// --- In  :
	// --- Out : 
	// --- Returns : Whether this window is a frame window
	// --- Effect : 
{
	// Mimic a frame window to get the idle Update CmdUI messages from the toolbar
	return TRUE;
}

int COXEditList::GetImagesWidth()
	// --- In  :
	// --- Out : 
	// --- Returns : The width in pixels of the images in the first column
	//               (small image and state image)
	// --- Effect : 
{
	if (GetImageColumn() != 0)
		// First column does not contain any images
		return 0;

	// Only small images can be shown in report mode
	CImageList* pSmallImageList=GetImageList(LVSIL_SMALL);
	CImageList* pStateImageList=GetImageList(LVSIL_STATE);

	LV_ITEM lvI;
	lvI.mask=LVIF_IMAGE | LVIF_STATE;
	lvI.iItem=0;
	lvI.iSubItem  =0;
	lvI.state=0;
	lvI.stateMask=ALLSTATEIMAGEMASKS;

	CSize stateImageSize(0,0);
	CSize smallImageSize(0,0);

	// Query the image and state this item is linked with
	if (GetItem(&lvI))
	{
		// We need the width of the images.  It will be used to determine from
		// where we can begin writing the text
		IMAGEINFO imageInfo;

		// First get the size of the small image and the state image
		// ... Convert from one-based to zero-based index
		int nStateIndex=STATEIMAGEMASKTOINDEX(lvI.state) - 1;
		if ((pStateImageList != NULL) && (0 <= nStateIndex))
		{
			pStateImageList->GetImageInfo(nStateIndex, &imageInfo);
			stateImageSize=CRect(imageInfo.rcImage).Size();
		}
		if (pSmallImageList != NULL)
		{
			pSmallImageList->GetImageInfo(lvI.iImage, &imageInfo);
			smallImageSize=CRect(imageInfo.rcImage).Size();
		}
	}

	return stateImageSize.cx + smallImageSize.cx;
}

void COXEditList::AdjustEmptyChanged()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Adjust the control after the the last (empty) item has
	//              been filled out
{
	int nLastItemIndex=GetItemCount() - 1;
	ASSERT(!GetItemText(nLastItemIndex, 0).IsEmpty());

	// Change the image of this item from 0 (none) to m_nNewImageItem if that image exists
	IMAGEINFO imageInfo;
	CImageList* pSmallImageList=GetImageList(LVSIL_SMALL);
	if ((pSmallImageList != NULL) && (pSmallImageList->GetImageInfo(m_nNewImageItem, &imageInfo)))
	{
		LV_ITEM lvi;
		memset(&lvi, 0, sizeof(lvi));
		lvi.iItem=nLastItemIndex;
		lvi.mask=LVIF_IMAGE;
		lvi.iImage=m_nNewImageItem;
		VERIFY(SetItem(&lvi));
	}

	// ... Remove selection of item
	VERIFY(SetItemState(nLastItemIndex, 0, LVIS_SELECTED));
	// Last item has been filled out, add a new empty one
	VERIFY(InsertItem(nLastItemIndex + 1, _T(""), 0)==nLastItemIndex + 1);
	// ... Give it focus
	VERIFY(SetItemState(nLastItemIndex + 1, LVIS_FOCUSED, LVIS_FOCUSED));

	// If the new item is not visible, make it visible
	if (GetTopIndex() + GetCountPerPage() - 1 < nLastItemIndex + 1)
	{
		VERIFY(EnsureVisible(nLastItemIndex + 1, 0, FALSE));

		// Because the window scrolled up by making the new item visible
		// we need to invalidate an extra region above the edit box
		// (bacause part of edit box has scrolled up as well)
		CRect editRect;
		VERIFY(GetItemRect(nLastItemIndex + 1,&editRect,LVIR_BOUNDS));
		//ASSERT_VALID(GetEditControl());
		//GetEditControl()->GetWindowRect(editRect);
		ScreenToClient(editRect);
		editRect.top -= editRect.Height();
		InvalidateRect(editRect);
	}
}

BOOL COXEditList::SetButtonCount(CToolBar& toolbar, int nCount)
	// --- In  : toolbar : The toolbar to use
	//			 nCount : The number of buttons that have to remain
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Removes some of the buttons of the toolbar
	//				This function will never add new buttons
{
	if (toolbar.GetCount() < nCount)
	{
		TRACE0("COXEditList::SetButtonCount : Cannot be used to add new buttons\n");
		return FALSE;
	}

	// Get the present buttons
	UINT* rgID=new UINT[nCount];
	// ... We are not interested in the style and image, but we have to provide a parameter
	UINT nStyle;
	int nImage;
	for (int nIndex=0; nIndex < nCount; nIndex++)
	{
		toolbar.GetButtonInfo(nIndex, rgID[nIndex], nStyle, nImage);
	}

	// Set the new buttons
	BOOL bSuccess=toolbar.SetButtons(rgID, nCount);
	if (!bSuccess)
		TRACE0("COXEditList::SetButtonCount : Failed to set the new buttons\n");

	// ... Clean up array of IDs
	delete[] rgID;

	return bSuccess;
}

int CALLBACK COXEditList::EditListCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
	// --- In  : lParam1 : lParam of the first item to compare
	//			 lParam2 : lParam of the second item to compare
	//			 lParamSort : parem of the sort object (COXGridListSortInfo)
	// --- Out : 
	// --- Returns : A negative value if the first item should precede the second, 
	//				 a positive value if the first item should follow the second, 
	//				 or zero if the two items are equivalent.
	// --- Effect : Compares two items of the control
	//				The COXGridListSortInfo contains information about which subitem to use
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	COXGridListSortInfo* pGridListSortInfo=(COXGridListSortInfo*)lParamSort;
	ASSERT(pGridListSortInfo != NULL);
	ASSERT(AfxIsValidAddress(pGridListSortInfo, sizeof(COXGridListSortInfo)));

	int nIndex1, nIndex2;
	CString sItemText1, sItemText2;

	// Lookup the first and second Item
	nIndex1= PtrToInt(pGridListSortInfo->m_pThis->FindOriginalItemData(lParam1));
	nIndex2= PtrToInt(pGridListSortInfo->m_pThis->FindOriginalItemData(lParam2));

	// Make sure the last (empty) row stays always last
	int iResult;
	int nLastItem=pGridListSortInfo->m_pThis->GetItemCount() - 1; 
	if (nIndex1==nLastItem)
		iResult=1;
	else if (nIndex2==nLastItem)
		iResult=-1;
	else
	{
		// query the text
		sItemText1=pGridListSortInfo->m_pThis->GetItemText(nIndex1, pGridListSortInfo->m_nSubIndex);
		sItemText2=pGridListSortInfo->m_pThis->GetItemText(nIndex2, pGridListSortInfo->m_nSubIndex);
		// Compare both string on a No Case basis
		iResult=sItemText1.CompareNoCase(sItemText2);
	}

	return(iResult);
}

BOOL COXEditList::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	*pResult=0;

	// If this item changed state and has focus and is not yet selected, we select it 
	if (!m_bIsDeleting &&
		((pNMListView->uChanged & LVIF_STATE)==LVIF_STATE) &&
		((pNMListView->uNewState & LVIS_FOCUSED)==LVIS_FOCUSED) &&
		((pNMListView->uNewState & LVIS_SELECTED) != LVIS_SELECTED) )
	{
		SetCurSel(pNMListView->iItem);
	}

	if ((pNMListView->uChanged & LVIF_TEXT)==LVIF_TEXT)
	{
		if (pNMListView->iItem < GetItemCount() - 1)
		{
			if (GetItemText(pNMListView->iItem, 0).IsEmpty())
			{
				// If the text of this item changed to an empty text and 
				// it is not the last item : delete it
				VERIFY(DeleteItem(pNMListView->iItem));
				VERIFY(SetItemState(pNMListView->iItem, LVIS_FOCUSED, LVIS_FOCUSED));
			}
		}
		else
		{
			if (!GetItemText(pNMListView->iItem, 0).IsEmpty())
			{
				// If the text of the last item was filled out, add a new empty item
				ASSERT(pNMListView->iItem==GetItemCount() - 1);
				// ... SSet image of new item and add a new empty item
				AdjustEmptyChanged();
			}
		}
	}

	// Call the base class implementation
	return COXGridList::OnListCtrlNotify(pNMHDR, pResult);
}

void COXEditList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	COXGridList::OnKeyDown(nChar, nRepCnt, nFlags);

	if((nChar==VK_DELETE) && CanDelete())
	{
		DeleteSelectedItems();
	}

}

void COXEditList::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// ... Call base class implementation
	COXGridList::OnSysKeyDown(nChar, nRepCnt, nFlags);

	// Move item when Alt + arrow Up/Down was pressed
	if((nChar==VK_UP) && CanMoveUp())
	{
		MoveItemUp();	
	}
	else if((nChar==VK_DOWN) && CanMoveDown())
	{
		MoveItemDown();	
	}
}

BOOL COXEditList::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// First call the base class implementation (TRUE=eaten)
	if (COXGridList::OnBeginlabeledit(pNMHDR, pResult))
		return TRUE;

	LV_DISPINFO* pDispInfo=(LV_DISPINFO*)pNMHDR;
	// ... Never eat this message
	BOOL bEaten=FALSE;
	int nItem=pDispInfo->item.iItem;

	// Store the old text of the item so we can restore it if necessary
	m_sOriginalEditText=GetItemText(nItem,0);

	// set internal edit control to use whole cient area
	COXGridEdit* pGridEdit=GetGridEditCtrl();
	ASSERT(pGridEdit!=NULL);
	pGridEdit->SetFitToClient(TRUE);


	// Do not change the result (already filled out by the base class)
	// *pResult;
	return bEaten;
}

BOOL COXEditList::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// First call the base class implementation (TRUE=eaten)
	if (COXGridList::OnEndlabeledit(pNMHDR, pResult))
		return TRUE;

	LV_DISPINFO* pDispInfo=(LV_DISPINFO*)pNMHDR;
	// ... Never eat this message
	BOOL bEaten=FALSE;

	// Check whether the user cancelled editing
	if ((pDispInfo->item.pszText==NULL) || (pDispInfo->item.iItem==-1))
		return bEaten;

	int nItem=pDispInfo->item.iItem;
	CString sNewText=pDispInfo->item.pszText;
	// ... Only check for duplicates in column 0
	if ((0==m_nEditSubItem) && !sNewText.IsEmpty())
	{
		// Check for duplicate names
		LV_FINDINFO lvfi;
		memset(&lvfi, 0, sizeof(lvfi));
		lvfi.flags=LVFI_STRING | LVFI_WRAP;
		lvfi.psz=pDispInfo->item.pszText;
		if (!m_bAllowDuplicates && (FindItem(&lvfi, nItem) != nItem))
		{
			// Duplicate names detected, restore old name
			AfxMessageBox(m_sDuplicateErrorMsg, MB_ICONEXCLAMATION);
			SetItemText(nItem, 0, m_sOriginalEditText);
		}
	}

	// Do not change the result (already filled out by the base class)
	// *pResult;
	return bEaten;
}

void COXEditList::OnNewItem() 
{
	EditNewItem();
}

void COXEditList::OnDeleteItem() 
{
	DeleteSelectedItems();
}

void COXEditList::OnUpdateDeleteItem(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanDelete());
}

void COXEditList::OnMoveItemUp() 
{
	MoveItemUp();
}

void COXEditList::OnUpdateMoveItemUp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanMoveUp());
}

void COXEditList::OnMoveItemDown() 
{
	MoveItemDown();
}

void COXEditList::OnUpdateMoveItemDown(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanMoveDown());
}

LRESULT COXEditList::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	// Pass on to the toolbar
	m_toolbar.SendMessage(WM_IDLEUPDATECMDUI, wParam, lParam);
	return 0L;
}

void COXEditList::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	if (m_bPostInitialized)
	{
		// Make the column of the list as wide as possible but 
		// leave space for scroll bar
		CRect listRect;
		GetClientRect(listRect);
		SetColumnWidth(0, listRect.Width());

		EnsureVisible(GetCurFocus(),0,FALSE);
	}

	PositionToolbar(m_eToolbarPosition);

	COXGridList::OnWindowPosChanged(lpwndpos);
}

BOOL COXEditList::OnToolTipText(UINT /* nControlID */, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT_VALID(this);
	// This code is almost the same as in CFrameWnd::OnToolTipText
	// see WinFrm.cpp
	ASSERT(pNMHDR->code==TTN_NEEDTEXTA || pNMHDR->code==TTN_NEEDTEXTW);

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA=(TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW=(TOOLTIPTEXTW*)pNMHDR;
	TCHAR szFullText[256];
	CString strTipText;
	UINT_PTR nID=pNMHDR->idFrom;
	if (pNMHDR->code==TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code==TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// ... idFrom is actually the HWND of the tool
		nID=((UINT_PTR)(WORD)::GetDlgCtrlID((HWND)nID));
	}

	// ... nID will be zero on a separator
	if (nID != 0) 
	{
		// AfxLoadString is an undocumented function, it still takes a UINT in the latest SDK.
		AfxLoadString((UINT)nID, szFullText);
		// ... this is the command id, not the button index
		AfxExtractSubString(strTipText, szFullText, 1, '\n');
	}
#ifndef _UNICODE
	if (pNMHDR->code==TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		_mbstowcsz(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#else
	if (pNMHDR->code==TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#endif

	// Bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
		SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE);

	// Message was handled
	// ... Message does not use result
	*pResult=0;
	return TRUE;    
}

LRESULT COXEditList::OnPostInit(WPARAM /* wParam */, LPARAM /* lParam */)
{
	// ... Window must be valid
	ASSERT(m_hWnd != NULL);
	ASSERT(::IsWindow(m_hWnd));

	if (m_bPostInitialized)
		// Already initialized
		return TRUE;

	// Mark that PostInitialize has been entered
	m_bPostInitialized=TRUE;

	// Set initial values
	InsertColumn(0, _T(""));
	SetEditable(m_bEditable);
	SetAutoEdit();
	SetMultipleSelection();
	SetBkColor(::GetSysColor(COLOR_WINDOW));

	// Make the column of the list as wide as possible but leave space for scroll bar
	CRect listRect;
	GetClientRect(listRect);
	SetColumnWidth(0, listRect.Width());

	// .. Add one extra (empty column)
	InsertItem(GetItemCount(), _T(""), 0);
	Invalidate();

	// ... Set the focus and selection to the first item
	SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);

	return TRUE;
}


LRESULT COXEditList::OnDeleteItem(WPARAM wParam, LPARAM lParam)
{
	// Do not delete the last item
	int nItem=(int)wParam;
	if(nItem==GetItemCount()-1)
	{
		return (LRESULT)FALSE;
	}

	// Pass the message to the base class
	return COXGridList::OnDeleteItem(wParam, lParam);
}

LRESULT COXEditList::OnDeleteAllItems(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	
	// Do not delete the last item
	int nCount=GetItemCount();
	for(int nItem=0; nItem<nCount-1; nItem++)
	{
		DeleteItem(0);
	}

	return (LRESULT)TRUE;
}

LRESULT COXEditList::OnInsertItem(WPARAM wParam, LPARAM lParam)
{
	const LV_ITEM* pLviOriginal=(const LV_ITEM*)lParam;
	BOOL bUseCopy=FALSE;
	LV_ITEM lviCopy;

	// Do not insert a non-empty item after last (empty) item
	int nLastItemIndex=GetItemCount();
	if (m_bPostInitialized && (nLastItemIndex <= pLviOriginal->iItem) &&
		( ((pLviOriginal->mask & LVIF_TEXT) != LVIF_TEXT) ||
		  !CString(pLviOriginal->pszText).IsEmpty()))
	{
		ASSERT(AfxIsValidAddress(pLviOriginal, sizeof(LV_ITEM)));
		memcpy(&lviCopy, pLviOriginal, sizeof(LV_ITEM));
		bUseCopy=TRUE;
		lviCopy.iItem=nLastItemIndex - 1;
	}
	
	// Pass the message to the base class
	LRESULT result=COXGridList::OnInsertItem(wParam, bUseCopy ? 
		(LPARAM)&lviCopy : lParam);
	return result;
}


void COXEditList::SetEditable(BOOL bEdit/*=TRUE*/, int nColumn/*=-1*/)
{
	if(m_bPostInitialized)
	{
		COXGridList::SetEditable(bEdit,nColumn);
	}
	else
	{
		m_bEditable=bEdit;
	}
}

