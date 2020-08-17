// ==========================================================================
//				Class Implementation : OXTreeCtrl
// ==========================================================================

// Source file :OXTreeCtrl.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
		  
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXTreeCtrl.h"

#include "UTBStrOp.h"
#include "UTB64Bit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

#define _OX_MAX_ITEM_TEXT	300

/////////////////////////////////////////////////////////////////////////////
// COXTreeCtrl

IMPLEMENT_DYNAMIC(COXTreeCtrl, CListCtrl)

COXTreeCtrl::COXTreeCtrl():
m_ptLastClick(0,0),m_wndCal(),m_bInit(FALSE)
{
	m_nIndent= TV_MININDENT;
	m_dwExStyle=0;
	m_dwTCStyle=0;
	m_xtiRoot.SetExpand();
	m_pActiveEditWnd=NULL;
	m_nEditColumn=-1;
	m_nItemHeight=0;
	m_nItemsCount=0;

	m_bClick=FALSE;
	m_bCanEdit=FALSE;
	m_nLastIndex=-1;
	m_nLastColumn=-1;

	m_bCreatingDragImage=FALSE;
	m_hOldDropTarget=NULL;

	m_bFirstTimeHooking=TRUE;

	m_nRedrawFlag=0;

	m_nTimerCheckKeyboardInput=0;
	m_sSearchMask=_T("");

	m_clrHorizontalGrid = 0; // black is the default
	m_clrVerticalGrid = 0; // black is the default
	m_lpfnDefaultCompare = NULL;
}

COXTreeCtrl::~COXTreeCtrl()
{
	m_mapCompareFunctions.RemoveAll();
}


BEGIN_MESSAGE_MAP(COXTreeCtrl, CListCtrl)
	//{{AFX_MSG_MAP(COXTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_NOTIFY_REFLECT_EX(NM_KILLFOCUS, OnKillfocus)
	ON_NOTIFY_REFLECT_EX(NM_SETFOCUS, OnSetfocus)
	ON_NOTIFY_REFLECT_EX(LVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT_EX(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_PARENTNOTIFY()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_STYLECHANGED()
#ifndef OX_TREECTRL_NOITEMTIPS
	ON_MESSAGE(WM_DOYOUNEEDITEMTIP, OnNeedItemTip)
	ON_MESSAGE(WM_USER_ADJUSTLASTCOLUMN, OnUserAdjustLastColumn)
#endif
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXTreeCtrl message handlers

void COXTreeCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS)
	// --- In  :
	// --- Out : 
	// --- Returns :
	//--- Effect :draw item
{
	int itemID=lpDIS->itemID;
	if(itemID == -1)
		return;

	COXTreeItem *xti=(COXTreeItem*)CListCtrl::GetItemData(itemID);

	
	if( xti == NULL )
		return;


	int nLevel=xti->GetItemLevel();
	CRect rcItem(lpDIS->rcItem);

	CDC dc;
	dc.Attach(lpDIS->hDC);

	CRect rcGrid=rcItem;
	rcGrid.left += GetItemIndent(xti);
	switch(lpDIS->itemAction)
	{
	case  ODA_SELECT:
	case  ODA_DRAWENTIRE:
		{
			if(m_bCreatingDragImage)
			{
				COLORREF clrOldForground, clrOldBackground;
				rcItem.left=0;
				LV_COLUMN lvc;
				lvc.mask=LVCF_FMT|LVCF_WIDTH;
				int nCol=0;
				CFont *pOldFont=NULL;
				VERIFY(GetColumn(nCol, &lvc));
				rcItem.right=rcItem.left + lvc.cx;
				if(dc.RectVisible(rcItem))
				{
					DrawItemImages(&dc,xti,itemID,rcItem,0,FALSE);
					clrOldForground=dc.SetTextColor(xti->IsDisabled() ? 
						::GetSysColor(COLOR_3DSHADOW) : (xti->HasColor(nCol) ?
						xti->GetItemColor(nCol) : GetTextColor()));

					if (xti->HasBackColor(nCol))
						clrOldBackground=dc.SetBkColor((!IsWindowEnabled() ? 
							::GetSysColor(COLOR_BTNFACE) : xti->GetItemBackColor(nCol)));
					else
						clrOldBackground=dc.SetBkColor((!IsWindowEnabled() ? 
							::GetSysColor(COLOR_BTNFACE) : GetTextBkColor()));

					CString sTxt=GetItemText((HTREEITEM) xti,nCol);
					if(xti->HasFont(nCol))
						pOldFont=dc.SelectObject(xti->GetItemFont(nCol));
					UINT nEllipsisFormat;
					VERIFY(GetItemDrawEllipsis((HTREEITEM)xti,nEllipsisFormat,nCol));
					// draw item's text 
					DrawItemText(&dc,sTxt,rcItem,lvc.fmt,FALSE,
						(!IsWindowEnabled() ? 
						::GetSysColor(COLOR_BTNFACE) : GetTextBkColor()),
						nEllipsisFormat);
					dc.SetTextColor(clrOldForground);
					dc.SetBkColor(clrOldBackground);
		
					if(xti->HasFont(nCol))
					{
						dc.SelectObject(pOldFont);
					}
				}
			}
			else
			{
				BOOL bShowSelAlways=GetShowSelAlways();
				BOOL bFocus=(::GetFocus() == m_hWnd);
				BOOL bShowSel=bShowSelAlways | bFocus;
				BOOL bDropHilited=GetItemState(GetItemFromIndex(itemID),
					TVIS_DROPHILITED)==TVIS_DROPHILITED;
				BOOL bShowItemSel=bShowSel && (((lpDIS->itemState & 
					ODS_SELECTED)==ODS_SELECTED) ||	bDropHilited);
				COLORREF clrOldForground, clrOldBackground;
				int nOffset=GetItemIndent(xti);
				int nFocusRectOffset=rcItem.left + nOffset;
				CRect rcLines=rcItem;
				rcLines.right=rcLines.left + nOffset;
			
				DrawLines(&dc,rcLines,xti,nLevel);
	
				rcItem.left += nOffset;
				LV_COLUMN lvc;
				lvc.mask=LVCF_FMT|LVCF_WIDTH;
				int nCol=0;
				int nColCnt=GetColumnsCount();
				BOOL bRowSel=(BOOL)(m_dwExStyle & TVOXS_ROWSEL);
				for(nCol=0; nCol<nColCnt; nCol++)
				{
					CFont *pOldFont=NULL;
					VERIFY(GetColumn(nCol, &lvc));
					rcItem.right =rcItem.left + lvc.cx;
					if(nCol == 0)
						rcItem.right -= nOffset;
					if(dc.RectVisible(rcItem))
					{
						DrawItemImages(
							&dc,xti,itemID,rcItem,nCol,(bRowSel && bShowItemSel));

						if(nCol == 0)
						{
							nFocusRectOffset=rcItem.left;
						}
							
						clrOldForground=dc.SetTextColor(bShowItemSel && 
							((nCol == 0 || bRowSel) && GetFocus()==this) ?
							::GetSysColor(COLOR_HIGHLIGHTTEXT) : (xti->IsDisabled() ? 
							::GetSysColor(COLOR_3DSHADOW) : (xti->HasColor(nCol) ?
							xti->GetItemColor(nCol) : GetTextColor())));
						
						COLORREF clrBackground = bShowItemSel && (nCol==0 || bRowSel) ? 
							(GetFocus()==this ? ::GetSysColor(COLOR_HIGHLIGHT) : 
							::GetSysColor(COLOR_BTNFACE)) : 
							(!IsWindowEnabled() ? ::GetSysColor(COLOR_BTNFACE) : 
							((xti->HasBackColor(nCol) ? xti->GetItemBackColor(nCol) : GetTextBkColor())));
						
						clrOldBackground=dc.SetBkColor(clrBackground);

						CString sTxt=GetItemText((HTREEITEM) xti,nCol);
						if(xti->HasFont(nCol))
							pOldFont=dc.SelectObject(xti->GetItemFont(nCol));
						UINT nEllipsisFormat;
						VERIFY(GetItemDrawEllipsis((HTREEITEM)xti,nEllipsisFormat,nCol));
						
						dc.FillSolidRect(rcItem, clrBackground);

						// Draw item's text
						if (nCol != 0)
							rcItem.left += 1;

						DrawItemText(&dc,sTxt,rcItem,lvc.fmt,
							(lpDIS->itemState & ODS_FOCUS) && bFocus && nCol==0 && 
							!bRowSel, bRowSel && bShowItemSel ? 
							(GetFocus()==this ? ::GetSysColor(COLOR_HIGHLIGHT) : 
							::GetSysColor(COLOR_BTNFACE)) : 
							(!IsWindowEnabled() ? ::GetSysColor(COLOR_BTNFACE) : 
							((xti->HasBackColor(nCol) ? xti->GetItemBackColor(nCol) : GetTextBkColor()))),nEllipsisFormat);

						dc.SetTextColor(clrOldForground);
						dc.SetBkColor(clrOldBackground);

						if(xti->HasFont(nCol))
						{
							dc.SelectObject(pOldFont);
						}
						if(HasGrid(TVOXS_VGRID))
						{
							CPen pen;
							pen.CreatePen(PS_SOLID, 1, m_clrVerticalGrid);
							CPen* pOldPen = (CPen*) dc.SelectObject(&pen);
							dc.MoveTo(rcItem.right-1,rcItem.top);
							dc.LineTo(rcItem.right-1,rcItem.bottom);
							dc.SelectObject(pOldPen);
						}
					}
					rcItem.left =rcItem.right;
				}
				if(HasGrid(TVOXS_HGRID))
				{
					CPen pen;
					pen.CreatePen(PS_SOLID, 1, m_clrHorizontalGrid);
					CPen* pOldPen = (CPen*) dc.SelectObject(&pen);
					
					// draw horizontal grid line
					if(GetDrawGridFullLength())
					{
						CRect clientRect;
						GetClientRect(clientRect);
						dc.MoveTo(clientRect.left,rcGrid.bottom-1);
						dc.LineTo(clientRect.right,rcGrid.bottom-1);
					}
					else
					{
						dc.MoveTo(rcGrid.left,rcGrid.bottom-1);
						dc.LineTo(rcGrid.right,rcGrid.bottom-1);
					}

					dc.SelectObject(pOldPen);
				}
				if((lpDIS->itemState & ODS_FOCUS) && bFocus && bRowSel &&
					!(m_dwExStyle & TVOXS_NOFOCUSRECT))
				{
					rcGrid.left=nFocusRectOffset;
					dc.DrawFocusRect(rcGrid);
				}
			}
		}
		break;
	default:
		ASSERT(FALSE);
	}

	dc.Detach();
}

void COXTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(GetFocus()!=this)
	{
		SetFocus();
	}
	m_ptLastClick=point;
	UINT nFl;
	int idx=HitTest(point,&nFl);

	HTREEITEM hti=GetItemFromIndex(idx);
	ASSERT(hti);
	COXTreeItem *xti=GetXItem(hti);
	ASSERT(xti);
	m_bCanEdit=FALSE;
	if(idx!=-1)
	{
		if (nFl & TVHT_ONITEMBUTTON)
		{
			Expand(hti,TVE_TOGGLE);
			return;
		}
		if(nFl & TVHT_ONITEM)
		{
			CListCtrl::OnLButtonDown(nFlags,point);
		}
		else
		{
			if(GetPickAnywhere())
			{
				CRect rcItem;
				CListCtrl::GetItemRect(idx,&rcItem,LVIR_BOUNDS);
				rcItem.left+=GetItemIndent(xti);
				if(rcItem.PtInRect(point))
				{
					if((GetKeyState(VK_SHIFT)&0x8000) || 
						(GetKeyState(VK_CONTROL)&0x8000))
					{
						VERIFY(CListCtrl::GetItemPosition(idx,&point));
					}
					CListCtrl::OnLButtonDown(LVHT_ONITEM,point);
				}
			}
		}

		// try to detect subitem to edit label
		CRect rcEdit;
		int nCol=GetEditColumn(xti,&rcEdit);

		m_nLastIndex=idx;
		m_nLastColumn=nCol;
	}
	else
	{
		if((GetExStyle()&TVOXS_MULTISEL)!=0)
		{
			CWnd::OnLButtonDown(nFlags, point);
		}
		m_nLastIndex=-1;
		m_nLastColumn=-1;
	}

	m_bClick=!m_bClick;
}

HTREEITEM COXTreeCtrl::InsertItem(LPCTSTR lpszItem, HTREEITEM hParent, 
								  HTREEITEM hInsertAfter)
{
	if(hParent==NULL)
		hParent=TVI_ROOT;

	COXTreeItem *pParent=GetXItem(hParent);
	if(!pParent)
		return NULL;
	COXTreeItem *pInsAfter;
	if(hInsertAfter == TVI_FIRST)
	{
		pInsAfter=(COXTreeItem *) TVI_FIRST;
	}
	else if(hInsertAfter == TVI_SORT)
	{
		pInsAfter=pParent->FindItemToInsertAfter(lpszItem);
	}
	else
	{
		pInsAfter=GetXItem(hInsertAfter);
	}
	COXTreeItem * pNewItem=new COXTreeItem(lpszItem);
	ASSERT(pParent);
	BOOL bDrawBtn=pParent->NeedDrawButton();
	pParent->AddChild(pNewItem,pInsAfter);
	AddItemToCtrl(pNewItem);
	if(pParent->NeedDrawButton() != bDrawBtn && HasButtons())
	{
		// parent button's state changed, redraw parent
		int idx=GetItemIndexInternal(pParent);
		if(idx != -1)
			CListCtrl::RedrawItems(idx,idx);
	}
	m_nItemsCount++;
	return (HTREEITEM)pNewItem;
}

HTREEITEM COXTreeCtrl::InsertItem(LPTV_INSERTSTRUCT lpTVIS)
{
	if(lpTVIS->hParent==NULL)
		lpTVIS->hParent=TVI_ROOT;

	COXTreeItem *pParent=GetXItem(lpTVIS->hParent);
	if(!pParent)
		return NULL;
	COXTreeItem *pInsAfter;
	if(lpTVIS->hInsertAfter == TVI_FIRST)
	{
		pInsAfter=(COXTreeItem *) TVI_FIRST;
	}
	else if(lpTVIS->hInsertAfter == TVI_SORT)
	{
		pInsAfter=pParent->FindItemToInsertAfter(lpTVIS->item.pszText);
	}
	else
	{
		pInsAfter=GetXItem(lpTVIS->hInsertAfter);
	}
	
	COXTreeItem * pNewItem= new COXTreeItem(lpTVIS->item.pszText);

	BOOL bDrawBtn=pParent->NeedDrawButton();
	pParent->AddChild(pNewItem,pInsAfter);
	lpTVIS->item.mask |= TVIF_HANDLE;
	lpTVIS->item.hItem=(HTREEITEM) pNewItem;
	VERIFY(SetItem(&(lpTVIS->item)));
	AddItemToCtrl(pNewItem);
	if(pParent->NeedDrawButton() != bDrawBtn && HasButtons())
	{
		// parent button's state changed, redraw parent
		int idx=GetItemIndexInternal(pParent);
		if(idx != -1)
			CListCtrl::RedrawItems(idx,idx);
	}
	m_nItemsCount++;
	return (HTREEITEM) pNewItem;
}


HTREEITEM COXTreeCtrl::InsertItem(UINT nMask, LPCTSTR lpszItem, int nImage,
								  int nSelectedImage, UINT nState, 
								  UINT nStateMask, LPARAM lParam, 
								  HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	TV_INSERTSTRUCT tvis;
	tvis.hParent=hParent;
	tvis.hInsertAfter=hInsertAfter;
	tvis.item.mask=nMask;
	tvis.item.state=nState;
	tvis.item.stateMask=nStateMask;
	tvis.item.pszText=(LPTSTR)lpszItem;
	tvis.item.iImage=nImage;
	tvis.item.iSelectedImage=nSelectedImage;
	tvis.item.lParam=lParam;

	return InsertItem(&tvis);
}


HTREEITEM COXTreeCtrl::InsertItem(LPCTSTR lpszItem, 
								  int nImage, 
								  int nSelectedImage, 
								  HTREEITEM hParent/*=TVI_ROOT*/, 
								  HTREEITEM hInsertAfter/*=TVI_LAST*/)
{
	TV_INSERTSTRUCT tvis;
	tvis.hParent=hParent;
	tvis.hInsertAfter=hInsertAfter;
	tvis.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
	tvis.item.pszText=(LPTSTR)lpszItem;
	tvis.item.iImage=nImage;
	tvis.item.iSelectedImage=nSelectedImage;

	return InsertItem(&tvis);
}


int COXTreeCtrl::GetItemIndex(HTREEITEM hItem) const
{
	return GetItemIndexInternal(GetXItem(hItem));
}


int COXTreeCtrl::GetItemIndexInternal(COXTreeItem *pItem) const
{
	if(pItem == &m_xtiRoot)
		return -1;
	LV_FINDINFO lvfi;
	lvfi.flags=LVFI_PARAM;
	lvfi.lParam=(LPARAM)pItem;
	return CListCtrl::FindItem(&lvfi);
}

COXTreeItem* COXTreeCtrl::GetXItem(HTREEITEM hti) const
{
	if(hti == TVI_ROOT)
		return &m_xtiRoot;
	if(hti == TVI_FIRST || hti == TVI_LAST)
		return NULL;
#ifdef _DEBUG
	if(hti)
		ASSERT(_CrtIsValidPointer(hti,sizeof(COXTreeItem),FALSE));
#endif
	return (COXTreeItem*)hti;
}

void COXTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if(GetFocus() != this)
		SetFocus();

	UINT flgs = nFlags;
	int idx   = HitTest(point,&flgs);

   if (idx != -1)
      if (GetPickAnywhere() || (flgs & (TVHT_ONITEM | TVHT_ONITEMBUTTON)))
      {
         Expand((HTREEITEM )CListCtrl::GetItemData(idx),TVE_TOGGLE);
         CListCtrl::OnLButtonDblClk(nFlags,point);
      }
	m_bClick=FALSE;
}

void COXTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	UINT flgs = nFlags;
	int  idx  = HitTest(point,&flgs);

   if (idx != -1)
      if (GetPickAnywhere() || (flgs & (TVHT_ONITEM | TVHT_ONITEMBUTTON)))
         CListCtrl::OnRButtonDown(nFlags,point);
}

void COXTreeCtrl::OnMButtonDown(UINT nFlags, CPoint point) 
{
	UINT flgs = nFlags;
	int  idx  = HitTest(point,&flgs);

   if (idx != -1)
      if (GetPickAnywhere() || (flgs & (TVHT_ONITEM | TVHT_ONITEMBUTTON)))
         CListCtrl::OnMButtonDown(nFlags,point);
}

BOOL COXTreeCtrl::Expand(HTREEITEM hItem, UINT nCode)
{
	COXTreeItem *pItem=GetXItem(hItem);
	ASSERT(pItem);
	if(SendItemExpandingNotify(hItem,nCode))
	{
		return FALSE;// parent voted against!
	}

	SetRedraw(FALSE);
	
	pItem->Expand(nCode,this);

	SetRedraw(TRUE);
	
	CListCtrl::RedrawItems(GetItemIndex(hItem),GetItemIndex(hItem));
	
	SendItemExpandedNotify(hItem,nCode);
	
	return TRUE;
}

int COXTreeCtrl::SetItemAtPos(int pos,COXTreeItem *pItem)
{
	int idx=CListCtrl::InsertItem(pos,LPSTR_TEXTCALLBACK);
	if(idx == -1)
		return -1;
	CListCtrl::SetItemData(idx,(DWORD)PtrToUlong (pItem));
	for(int i=1;i < pItem->GetSubitemsCount();i++)
	{
		LPCTSTR lpszText=GetItemText((HTREEITEM)pItem,i);
		if(lpszText)
		{
			CListCtrl::SetItemText(pos,i,LPSTR_TEXTCALLBACK);
		}
	}

	TV_ITEM tvi;
	::ZeroMemory((void*)&tvi,sizeof(tvi));
	if(pItem->m_tvi.iImage>=0)
	{
		tvi.mask|=TVIF_IMAGE;
		tvi.iImage=pItem->m_tvi.iImage;
	}
	if(pItem->m_tvi.iSelectedImage>=0)
	{
		tvi.mask|=TVIF_SELECTEDIMAGE;
		tvi.iSelectedImage=pItem->m_tvi.iSelectedImage;
	}
	if((pItem->m_tvi.state & INDEXTOSTATEIMAGEMASK(0xF))!=0)
	{
		tvi.state=(pItem->m_tvi.state & INDEXTOSTATEIMAGEMASK(0xF));
		tvi.stateMask=tvi.state;
		tvi.mask|=TVIF_STATE;
	}
	if(tvi.mask!=0)
	{
		tvi.hItem=pItem->m_tvi.hItem;
		tvi.mask|=TVIF_HANDLE;
		VERIFY(SetItem(&tvi));
	}

	return idx;
}

BOOL COXTreeCtrl::AddItemToCtrl(COXTreeItem *pItem)
	// --- In  : item handle
	// --- Out : 
	// --- Returns :Nonzero if successful; otherwise 0
	//--- Effect :
{
	ASSERT(pItem!=NULL);

	if(pItem->pxParent!=NULL && pItem->pxParent->m_tvi.cChildren==0)
		pItem->pxParent->m_tvi.cChildren=1;

	if (pItem->IsAnyParentCollapsed())
		return TRUE;

	if(pItem->IsHidden())
		return TRUE;

	COXTreeItem *pPrevItem=pItem->GetPrevInTree();
	while(pPrevItem != &m_xtiRoot && !pPrevItem->IsVisible())
	{
		pPrevItem=pPrevItem->GetPrevInTree();
		if(!pPrevItem)
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	ASSERT(pPrevItem);
	int nIndex;
	if(pPrevItem == &m_xtiRoot)
		nIndex=0;//will be first item!
	else
	{
		nIndex=GetItemIndexInternal(pPrevItem) + 1;
		ASSERT(nIndex);
	}
	BOOL bRet=(-1 != SetItemAtPos(nIndex,pItem));
	return bRet;
}

void COXTreeCtrl::DeleteChildrenItems(COXTreeItem *pItem)
	// --- In  :handle of the item
	// --- Out : 
	// --- Returns :
	//--- Effect :delete children of specified item
{
	COXTreeItem *xti=pItem->pxFirstChild;
	while(xti)
	{
		DeleteChildrenItems(xti);
		int idx=GetItemIndexInternal(xti);
		if(idx!=-1)
		{
			CListCtrl::DeleteItem(idx);
			
		}
		xti=xti->pxNext;
	}
	pItem->DeleteChildren();
	pItem->m_tvi.cChildren=0;
}

void COXTreeCtrl::DrawItemText(CDC *pDC,LPCTSTR sText,CRect& rcTxt,UINT nJustify,
							   BOOL bDrawFocus,COLORREF clrFillOutside,
							   UINT nEllipsisMode)
{

	CRect rcText=rcTxt;
	CRect rcFill=rcTxt;
	// check rect need to be drawn
	if(!pDC->RectVisible(&rcText))
		return;
	// check text
	if(!sText || sText == LPSTR_TEXTCALLBACK)
		return;

	COLORREF clrBack=pDC->GetBkColor();
	if(!rcFill.IsRectEmpty())
		pDC->FillSolidRect(rcFill,clrFillOutside);
	pDC->SetBkColor(clrBack);

	UINT nFormat=DT_SINGLELINE|DT_NOPREFIX|DT_VCENTER;
	nFormat|=nEllipsisMode;

	rcText.DeflateRect(2,0);
	rcFill=rcText;
	pDC->DrawText(sText,rcFill,nFormat|DT_CALCRECT);
	rcFill.top=rcTxt.top;
	rcFill.bottom=rcTxt.bottom;
	rcFill.InflateRect(2,0);
	int nTextSize=rcFill.Width();

	switch(nJustify & LVCFMT_JUSTIFYMASK)
	{
		case LVCFMT_LEFT:
			nFormat|=DT_LEFT;
			rcFill.left=__max(rcFill.left,rcTxt.left);
			rcFill.right=__min(rcFill.right,rcTxt.right);
			break;
		case LVCFMT_RIGHT:
			nFormat|=DT_RIGHT;
			rcFill.right=rcTxt.right;
			rcFill.left=__max(rcFill.right-nTextSize,rcTxt.left);
			break;
		case LVCFMT_CENTER:
			nFormat|=DT_CENTER;
			rcFill.left=__max(rcTxt.left+((rcTxt.Width()-nTextSize)/2),rcTxt.left);
			rcFill.right=__min(rcTxt.right-((rcTxt.Width()-nTextSize)/2)+
				(rcTxt.Width()-nTextSize)%2,rcTxt.right);
			break;
		default:
			ASSERT(FALSE);
			break;
	}

	if(!rcFill.IsRectEmpty())
	{
		pDC->FillSolidRect(rcFill,clrBack);
	}

	// Draw the text
	pDC->SetBkMode(TRANSPARENT);					
	pDC->DrawText(sText,rcText,nFormat);

	if(bDrawFocus && !(m_dwExStyle & TVOXS_NOFOCUSRECT))
	{
		pDC->DrawFocusRect(rcFill);
	}

	return;
}

BOOL COXTreeCtrl::GetShowSelAlways() const
{
	return ((GetStyle() & TVS_SHOWSELALWAYS) == TVS_SHOWSELALWAYS);
}


BOOL COXTreeCtrl::OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult) 
	// --- In  :
	// --- Out : 
	// --- Returns :
	//--- Effect :
{
	UNREFERENCED_PARAMETER(pNMHDR);
	RefreshFocusItem();
	*pResult=0;

	return FALSE;
}

void COXTreeCtrl::RefreshFocusItem()
{
	int nFocusItem=CListCtrl::GetNextItem(-1,LVNI_SELECTED);
	while(nFocusItem != -1)
	{
		RedrawItems(nFocusItem,nFocusItem);
		nFocusItem=CListCtrl::GetNextItem(nFocusItem,LVNI_SELECTED);
	}
}

BOOL COXTreeCtrl::OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);
	RefreshFocusItem();
	*pResult=0;
	m_bClick=FALSE;

	return FALSE;
}

CImageList* COXTreeCtrl::SetImageList(CImageList* pImageList, int nImageList)
{
	switch(nImageList)
	{
	case TVSIL_NORMAL:
		nImageList=LVSIL_SMALL;
		break;
	case TVSIL_STATE:
		nImageList=LVSIL_STATE;
		break;
	default:
		TRACE(_T("COXTreeCtrl::SetImageList: unknown image list type!\n"));
		return NULL;
	}

	return CListCtrl::SetImageList(pImageList,nImageList);
}

BOOL COXTreeCtrl::HasButtons() const
{
	return (m_dwTCStyle & TVS_HASBUTTONS);
}

BOOL COXTreeCtrl::HasLines() const
{
	return (m_dwTCStyle & TVS_HASLINES);
}

BOOL COXTreeCtrl::HasLinesAtRoot() const
{
	return (m_dwTCStyle & TVS_LINESATROOT);
}


int COXTreeCtrl::GetItemStateIndex(int nItem) const
{
	LV_ITEM lvI;
	lvI.mask=LVIF_IMAGE | LVIF_STATE;
	lvI.iItem=nItem;
	lvI.iSubItem  =0;
	lvI.state=0;
	lvI.stateMask=INDEXTOSTATEIMAGEMASK(0xF);
	if (CListCtrl::GetItem(&lvI))
		return STATEIMAGEMASKTOINDEX(lvI.state) - 1;
	else
		return -1;
}

void COXTreeCtrl::DrawLines(CDC *pDC, LPRECT rLines, COXTreeItem *pItem, int nLev)
{
	int nLevel=nLev;
	CPen penBtn(PS_SOLID,1,::GetSysColor(COLOR_3DSHADOW));
	pDC->ExtTextOut(0,0,ETO_OPAQUE,rLines,_T(""),0,NULL);
	CRect rcLines(rLines);
	if(!rcLines.IsRectEmpty())
	{
		if (pItem != NULL && pItem->HasBackColor())
			pDC->FillSolidRect(rcLines,(!IsWindowEnabled() ? 
				::GetSysColor(COLOR_BTNFACE) : pItem->GetItemBackColor()));
		else
		{
			if (IsPropertiesWnd())
				pDC->FillSolidRect(rcLines, ::GetSysColor(COLOR_BTNFACE));
			else
				pDC->FillSolidRect(rcLines,(!IsWindowEnabled() ? 
					::GetSysColor(COLOR_BTNFACE) : GetTextBkColor()));
		}
	}
	CRect rcStep;
	rcStep=rcLines;
	rcStep.left=rcStep.right - m_nIndent;
	COXTreeItem *xti=pItem;
	int nLineMode; 
	if(xti->IsLastUnhidden() && GetItemIndexInternal(xti)==0)
		nLineMode=0;	// first & last node at the same time
	else if(GetItemIndexInternal(xti)==0)
		nLineMode=4;	// first node 
	else if(xti->IsLastUnhidden())
		nLineMode=2;	// last node 
	else
		nLineMode=3;	// there are node(s) below

	if(HasLines())
		DrawLine(pDC,&m_penLines,rcStep,nLineMode);
		
	CPen *pOld=pDC->SelectObject(&penBtn);
	if(HasButtons() && pItem->NeedDrawButton())
	{
		CRect rcButton;
		CalcBtnRect(&rcButton,rcStep);
		DrawButton(pDC,rcButton,pItem->IsExpanded());
	}
	pDC->SelectObject(pOld);
	if(HasLines())
	{
		while(nLevel > 0)
		{
			xti=xti->pxParent;
			rcStep.OffsetRect(-m_nIndent,0);
			if(!xti->IsLastUnhidden())
				DrawLine(pDC,&m_penLines,rcStep,1);
			nLevel--;
		}
	}
}

void COXTreeCtrl::DrawLine(CDC *pDC, CPen* pPen, CRect& rc, int nMode)
{
	CPen *penOld=pDC->SelectObject(pPen);

	switch(nMode)
	{
	case 0:
		pDC->MoveTo(rc.left + rc.Width()/2 - 1,rc.top + rc.Height()/2);
		pDC->LineTo(rc.right,rc.top + rc.Height()/2);
		break;

	case 1:
		pDC->MoveTo(rc.left + rc.Width()/2,rc.top - 1);
		pDC->LineTo(rc.left + rc.Width()/2,rc.bottom);
		break;
	
	case 2:
		pDC->MoveTo(rc.left + rc.Width()/2,rc.top - 1);
		pDC->LineTo(rc.left + rc.Width()/2,rc.top + rc.Height()/2);
		pDC->LineTo(rc.right,rc.top + rc.Height()/2);
		break;
	
	case 3:
		pDC->MoveTo(rc.left + rc.Width()/2,rc.top - 1);
		pDC->LineTo(rc.left + rc.Width()/2,rc.bottom);
		pDC->MoveTo(rc.left + rc.Width()/2,rc.top + rc.Height()/2);
		pDC->LineTo(rc.right,rc.top + rc.Height()/2);
		break;
	
	case 4:
		pDC->MoveTo(rc.left + rc.Width()/2,rc.bottom + 1);
		pDC->LineTo(rc.left + rc.Width()/2,rc.top + rc.Height()/2);
		pDC->LineTo(rc.right,rc.top + rc.Height()/2);
		break;
	
	default:
		ASSERT(FALSE);
	}

	if(penOld!=NULL)
	{
		pDC->SelectObject(penOld);
	}
}

void COXTreeCtrl::DrawButton(CDC *pDC,CRect& rcButton,BOOL bExpand)
{
	pDC->Rectangle(rcButton);

	CRect rcInnerSpace=rcButton;
	rcInnerSpace.DeflateRect(1,1);
	if(!rcInnerSpace.IsRectEmpty())
	{
		pDC->FillSolidRect(rcInnerSpace,(!IsWindowEnabled() ? 
			::GetSysColor(COLOR_BTNFACE) : GetTextBkColor()));
	}

	CPen penPlusMinus;
	penPlusMinus.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOWTEXT));
	CPen* pOldPen = pDC->SelectObject(&penPlusMinus);

	pDC->MoveTo(rcButton.left + 2,rcButton.top + rcButton.Height()/2);
	pDC->LineTo(rcButton.right-2,rcButton.top + rcButton.Height()/2);
	if(!bExpand)
	{
		pDC->MoveTo(rcButton.left + rcButton.Width()/2,rcButton.top +2);
		pDC->LineTo(rcButton.left + rcButton.Width()/2,rcButton.bottom -2);
	}

	pDC->SelectObject(pOldPen);
}

void COXTreeCtrl::CalcBtnRect(CRect *prcButton,CRect& rcStep)
{
	(*prcButton)=rcStep;
	int nSide=__max((rcStep.Height()*5)/10,9);
	int offx =(rcStep.Width() - nSide + 1)/2;
	int offy=(rcStep.Height() - nSide + 1)/2;
	prcButton->top += offy;
	prcButton->bottom=prcButton->top +nSide;
	prcButton->left += offx;
	prcButton->right=prcButton->left + nSide;
}

int COXTreeCtrl::HitTest(CPoint pt, UINT* pFlags) const
{
	UINT nListFlags;
	int idx=CListCtrl::HitTest(pt,&nListFlags);
	if(!pFlags)
		return idx;
	(*pFlags)=0;
	if(idx != -1)
	{
		CRect rcItem;
		COXTreeItem *xti=(COXTreeItem *)CListCtrl::GetItemData(idx);
		if(CListCtrl::GetItemRect(idx,&rcItem,LVIR_BOUNDS))
		{
			int nIndent=-1;
			CRect rcButton,rcText,rcImage,rcStateImage;
			GetItemPartRect(rButton,xti,&rcButton,&nIndent);
			GetItemPartRect(rImage,xti,&rcImage,&nIndent);
			GetItemPartRect(rStateImage,xti,&rcStateImage,&nIndent);
			GetItemPartRect(rText,xti,&rcText,&nIndent);
			*pFlags=TVHT_ONITEM;
			if(rcText.PtInRect(pt))
				(*pFlags) |= TVHT_ONITEMLABEL;
			else
			if(rcImage.PtInRect(pt))
				(*pFlags) |= TVHT_ONITEMICON;
			else
			if(rcStateImage.PtInRect(pt))
				(*pFlags) |= TVHT_ONITEMSTATEICON;
			else
			if(rcButton.PtInRect(pt))
				(*pFlags) |= TVHT_ONITEMBUTTON;
			else
			if(rcItem.PtInRect(pt))
				(*pFlags) |= TVHT_TORIGHT;
			else
			{
				(*pFlags) |= TVHT_NOWHERE;
				(*pFlags) &= ~TVHT_ONITEM;
			}
		}
	}
	if ((*pFlags)==0)
	{
		if (nListFlags&LVHT_ABOVE)
			*pFlags=TVHT_ABOVE;
		else if (nListFlags&LVHT_BELOW)
			*pFlags=TVHT_BELOW;
		else if (nListFlags&LVHT_TOLEFT)
			*pFlags=TVHT_TOLEFT;
		else if (nListFlags&LVHT_TORIGHT)
			*pFlags=TVHT_TORIGHT;
		else if (nListFlags&LVHT_NOWHERE)
			*pFlags=TVHT_NOWHERE;
	}

	return idx;
}


HTREEITEM COXTreeCtrl::HitTest(TVHITTESTINFO* pHitTestInfo) const
{
	ASSERT(pHitTestInfo!=NULL);
	pHitTestInfo->hItem=NULL;
	int nIndex=HitTest(pHitTestInfo->pt,&pHitTestInfo->flags);
	if(nIndex>=0)
	{
		pHitTestInfo->hItem=GetItemFromIndex(nIndex);
	}
	return pHitTestInfo->hItem;
}


BOOL COXTreeCtrl::Init()
{
	if(m_bInit)
		return TRUE;

	// create line pen
	LOGBRUSH logBrush;
	logBrush.lbColor=GetSysColor(COLOR_3DSHADOW);
	logBrush.lbStyle=BS_SOLID;
	logBrush.lbHatch=HS_HORIZONTAL;
	if (!m_penLines.CreatePen(PS_COSMETIC | PS_ALTERNATE, 1, &logBrush, 0, NULL))
	{
		if (!m_penLines.CreatePen(PS_COSMETIC, 1, &logBrush, 0, NULL))
		{
			TRACE(_T("COXTreeCtrl::Init: Error creating the lines Pen\n"));
			return FALSE;
		}
	}
	CRect rClient;
	GetClientRect(&rClient);
	// insert at least one column initially
	if(CListCtrl::InsertColumn(0,_T(""),LVCFMT_LEFT,rClient.Width()) == -1)
	{
		TRACE(_T("COXTreeCtrl::Init: Error inserting column\n"));
		return FALSE;
	}
	VERIFY(SetBkColor(::GetSysColor(COLOR_WINDOW)));
	VERIFY(SetTextBkColor(::GetSysColor(COLOR_WINDOW)));
	VERIFY(SetTextColor(::GetSysColor(COLOR_WINDOWTEXT)));
	
	// subclass header control
	HWND hWnd=GetHeaderCtrlHandle();
	if(hWnd!=NULL)
	{
		if(!::IsWindow(m_wndHdr.GetSafeHwnd()))
			VERIFY(m_wndHdr.SubclassWindow(hWnd));
		else
			ASSERT(m_wndHdr.GetSafeHwnd()==hWnd);
	}

	m_bInit=TRUE;

	return TRUE;
}

void COXTreeCtrl::PreSubclassWindow() 
{
	_AFX_THREAD_STATE* pThreadState=AfxGetThreadState();
	// hook not already in progress
	if(pThreadState->m_pWndInit==NULL)
	{
		VERIFY(Init());
	}

	CListCtrl::PreSubclassWindow();
	DWORD dwStyle=CListCtrl::GetStyle();

	ASSERT(dwStyle & LVS_REPORT);
	ASSERT(dwStyle & LVS_OWNERDRAWFIXED);
	ASSERT(!(dwStyle & LVS_SORTASCENDING));
	ASSERT(!(dwStyle & LVS_SORTDESCENDING));

	if(dwStyle & LVS_SHOWSELALWAYS)
		m_dwTCStyle |= TVS_SHOWSELALWAYS;
	if(dwStyle & LVS_EDITLABELS)
		m_dwTCStyle |= TVS_EDITLABELS;
	
	if(dwStyle & LVS_NOCOLUMNHEADER)
		m_dwExStyle &= ~TVOXS_COLUMNHDR;
	else
		m_dwExStyle |= TVOXS_COLUMNHDR;

	if(dwStyle & LVS_SINGLESEL)
		m_dwExStyle &= ~TVOXS_MULTISEL;
	else
		m_dwExStyle |= TVOXS_MULTISEL;

	if(dwStyle & LVS_NOSCROLL)
		m_dwExStyle |= TVOXS_NOSCROLL;
	else
		m_dwExStyle &= ~TVOXS_NOSCROLL;

	if(dwStyle & LVS_NOSORTHEADER)
		m_dwExStyle |= TVOXS_NOSORTHEADER;
	else
		m_dwExStyle &= ~TVOXS_NOSORTHEADER;

	if(dwStyle & LVS_SHAREIMAGELISTS)
		m_dwExStyle |= TVOXS_SHAREIMAGELISTS;
	else
		m_dwExStyle &= ~TVOXS_SHAREIMAGELISTS;

#ifndef OX_TREECTRL_NOITEMTIPS
	m_dwExStyle |= TVOXS_ITEMTIPS;
#endif
}

int COXTreeCtrl::GetItemIndent(COXTreeItem *pItem) const
{
	if (IsPropertiesWnd())
	{
		// We have a properies window
		if((HasLines() || HasButtons()) && HasLinesAtRoot())
			return m_nIndent;
		else
			return 0;
	}
	
	int nLevel=pItem->GetItemLevel();
	int nIndent;
	if((HasLines() || HasButtons()) && HasLinesAtRoot())
		nIndent=m_nIndent *nLevel;
	else
		nIndent=m_nIndent *(nLevel-1);
	return nIndent;
}

void COXTreeCtrl::GetItemPartRect(COXTreeCtrl::eItemPartRect eType,
								  COXTreeItem *pItem,CRect *rTarget,
								  int *nIndent) const
{
	ASSERT_POINTER(pItem,COXTreeItem);
	if((*nIndent) == -1)
		(*nIndent)=GetItemIndent(pItem);
	int nIndex=GetItemIndexInternal(pItem);

	CRect rcItem;
	CListCtrl::GetItemRect(nIndex,&rcItem,LVIR_BOUNDS);

	CImageList *pImgList=CListCtrl::GetImageList(LVSIL_SMALL);
	CImageList *pStateImgList=CListCtrl::GetImageList(LVSIL_STATE);

	CRect rcLastIndent=rcItem;
	if(*nIndent)
	{
		rcLastIndent.right=rcItem.left + *nIndent;
		rcLastIndent.left=rcLastIndent.right - m_nIndent;
	}
	else
	{
		rcLastIndent.SetRectEmpty();
	}
	

	switch(eType)
	{
	case rButton://button rect
		{
			if(!HasButtons())
			{
				rTarget->SetRectEmpty();
			}
			else
			{
				(*rTarget)=rcLastIndent;
				int nSide=(rcLastIndent.Height()*5)/10;
				int offx =(rcLastIndent.Width() - nSide)/2;
				int offy=(rcLastIndent.Height() - nSide)/2;
				rTarget->top += offy;
				rTarget->bottom -= offy;
				rTarget->left += offx;
				rTarget->right -= offx;
			}
		}
		break;

	case rImage:// item image rect
		{
			if(pImgList == NULL)
			{
				// no image list
				rTarget->SetRectEmpty();
				return;
			}
			int nStateOffsetX;
			IMAGEINFO imgInfo;
			if(pStateImgList && GetItemStateIndex(nIndex) != -1)
			{
				pStateImgList->GetImageInfo(0,&imgInfo);
				nStateOffsetX=imgInfo.rcImage.right - imgInfo.rcImage.left;
			}
			else
				nStateOffsetX=0;
			pImgList->GetImageInfo(0,&imgInfo);
			CRect rcImage(imgInfo.rcImage);
			rTarget->left=rcItem.left + *nIndent + nStateOffsetX;
			rTarget->top=rcItem.bottom - rcImage.Height();
			rTarget->right=rTarget->left + rcImage.Width();
			rTarget->bottom=rcItem.bottom;
		}
		break;

	case rStateImage:
		{
			if(pStateImgList == NULL)
				rTarget->SetRectEmpty();
			else
			{
				IMAGEINFO imgInfo;
				pStateImgList->GetImageInfo(0,&imgInfo);
				CRect rcStateImage(imgInfo.rcImage);
				rTarget->left=rcItem.left + *nIndent;
				rTarget->top=rcItem.bottom - rcStateImage.Height();
				rTarget->right=rTarget->left + rcStateImage.Width();
				rTarget->bottom=rcItem.bottom;
			}
		}
		break;

	case rText:
		{
			CSize szText=GetColTextExtent(nIndex,0);
			CRect rcText;
			LV_COLUMN lvc;
			memset(&lvc,0,sizeof(LV_COLUMN));
			lvc.mask=LVCF_FMT;
			ASSERT(GetColumn(0, &lvc));
			int nImgWidth=0;
			IMAGEINFO imgInfo;
			if(pImgList)
			{
				pImgList->GetImageInfo(0,&imgInfo);
				CRect rcImg(imgInfo.rcImage);
				nImgWidth += rcImg.Width();
			}
			if(pStateImgList && GetItemStateIndex(nIndex) != -1)
			{
				pStateImgList->GetImageInfo(0,&imgInfo);
				CRect rcImg(imgInfo.rcImage);
				nImgWidth += rcImg.Width();
			}
			rcText.top=rcItem.top;
			rcText.bottom=rcItem.bottom;
			szText.cx=__min(szText.cx,GetColumnWidth(0)-nImgWidth-(*nIndent));
			int fmt=lvc.fmt & LVCFMT_JUSTIFYMASK;
			switch(fmt)
			{
			case LVCFMT_LEFT:
				{
					rcText.left=rcItem.left + (*nIndent) + nImgWidth;
					rcText.right=rcText.left + szText.cx;
				}
				break;
			case LVCFMT_RIGHT:
				{
					rcText.right=rcItem.left + GetColumnWidth(0);
					rcText.left=rcText.right - szText.cx;
				}
				break;
			case LVCFMT_CENTER:
				{
					int dx=(GetColumnWidth(0)-nImgWidth-(*nIndent)-szText.cx)/2;
					if(dx < 0)
						dx=0;
					rcText.left=rcItem.left + (*nIndent) + nImgWidth + dx;
					rcText.right=rcText.left + GetColumnWidth(0) - dx;
				}
				break;
			default:
				ASSERT(FALSE);
			}

			(*rTarget)=rcText;
		}
		break;
	default:
		// unknown kind of rect passed!
		ASSERT(FALSE);
		break;
	}

}

CSize COXTreeCtrl::GetColTextExtent(int nIndex,int nCol) const
	// --- In  :
	// --- Out : 
	// --- Returns :dimension of specified item's text
	//--- Effect :
{
	ASSERT(nCol <= GetColumnsCount());//column number must be valid!

	COXTreeItem *xti=(COXTreeItem*)CListCtrl::GetItemData(nIndex);
	ASSERT_POINTER(xti,COXTreeItem);
	
	CString sText=GetItemText((HTREEITEM) xti,nCol);
	CWnd* pWnd=(CWnd*)this;
	CDC *pDC=pWnd->GetDC();
	CFont * pOldFont=NULL;
	if(xti->HasFont(nCol))
		pOldFont=pDC->SelectObject(xti->GetItemFont(nCol));

	CSize sz=pDC->GetTextExtent(sText);
	sz.cx=__min(sz.cx,GetColumnWidth(nCol));
	if(xti->HasFont(nCol))
		 pDC->SelectObject(pOldFont);
	pWnd->ReleaseDC(pDC);
	return sz;
}

int COXTreeCtrl::GetColumnsCount() const
{
	int cnt=0;
	LV_COLUMN lvc;
	memset(&lvc, 0, sizeof(LV_COLUMN));
	lvc.mask=LVCF_FMT;
	while(GetColumn(cnt, &lvc))
		cnt++;
	return cnt;
}

void COXTreeCtrl::DrawItemImages(CDC *pDC, COXTreeItem* xti, int nItem, 
								 CRect& rcItem, int nCol, BOOL bRowSel)
{
	UNREFERENCED_PARAMETER(nItem);

	CImageList *pStateImgList=CListCtrl::GetImageList(LVSIL_STATE);
	CImageList *pImgList=CListCtrl::GetImageList(LVSIL_SMALL);

	IMAGEINFO imi;
	CRect rcErase;
	CRect rcImage;
	CPoint ptStart;
	if(pStateImgList && nCol == 0)
	{
		int idx=STATEIMAGEMASKTOINDEX(xti->m_tvi.state)-1;
		if(idx != -1)
		{
			// ask the image size
			pStateImgList->GetImageInfo(idx,&imi);
			// clear the entry image rect
			rcErase=rcItem;
			rcImage=imi.rcImage;
			rcErase.right=rcErase.left + rcImage.Width();
/*			The COXTreeCtrl did not paint checkboxes properly when the
			control lost focus, in this situation all check boxes would
			be painted with the background of COLOR_BTNFACE.
			pDC->FillSolidRect(
				&rcErase,((GetFocus()!=this || !IsWindowEnabled()) ? 
				::GetSysColor(COLOR_BTNFACE) : GetTextBkColor()));*/
			ptStart=rcItem.TopLeft();
			if(rcImage.Height()<rcErase.Height())
			{
				ptStart.y+=(rcErase.Height()-rcImage.Height())/2;
			}
			// now draw the state img
			pStateImgList->Draw(pDC,idx,ptStart,ILD_TRANSPARENT);
			// Move left of the item rect to draw text later
			rcItem.left += CRect(imi.rcImage).Width();
		}
	}
	if(nCol == 0)
	{
		TV_ITEM tvi;
		memset(&tvi,0,sizeof(tvi));
		// retreive item's images and state
		tvi.mask=TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_HANDLE | TVIF_STATE;
		tvi.stateMask=TVIS_SELECTED;
		tvi.hItem=(HTREEITEM)xti;
		if(!GetItem(&tvi))
			return;
		if(pImgList)
		{
			int nImg;
			if(tvi.state & TVIS_SELECTED)
			{
				nImg=tvi.iSelectedImage;
			}
			else
			{
				nImg=tvi.iImage;
			}
			if(nImg==-1)
			{
				return;
			}
			// ask the image size
			pImgList->GetImageInfo(nImg,&imi);
			// clear the entry image rect
			rcErase=rcItem;
			rcImage=imi.rcImage;
			rcErase.right=rcErase.left + rcImage.Width();

			if (xti->HasBackColor(nCol))
				pDC->FillSolidRect(&rcErase,
					(!IsWindowEnabled() ? ::GetSysColor(COLOR_BTNFACE) : xti->GetItemBackColor(nCol)));
			else
				pDC->FillSolidRect(&rcErase,
					(!IsWindowEnabled() ? ::GetSysColor(COLOR_BTNFACE) : GetTextBkColor()));

			ptStart=rcItem.TopLeft();
			if(rcImage.Height()<rcErase.Height())
			{
				ptStart.y+=(rcErase.Height()-rcImage.Height())/2;
			}
			pImgList->Draw(pDC,nImg,ptStart,ILD_TRANSPARENT);
			rcItem.left += CRect(imi.rcImage).Width();
		}
	}
	else
	{
		int nImg=xti->GetSubItemImage(nCol);
		if(nImg >= 0)
		{
			ASSERT(nImg < pImgList->GetImageCount());
			COLORREF clrBack=CLR_NONE;
			pImgList->GetImageInfo(nImg,&imi);
			// ask the image size
			pImgList->GetImageInfo(nImg,&imi);
			// clear the entry image rect
			rcErase=rcItem;
			rcImage=imi.rcImage;
			rcErase.right=rcErase.left + CRect(imi.rcImage).Width();
			// if entry row selection mode set, set subitem image
			// background color
			if(bRowSel)
			{
				if (xti->HasBackColor(nCol))
				{
					clrBack=pImgList->SetBkColor(
						(GetFocus()!=this || !IsWindowEnabled()) ? 
						::GetSysColor(COLOR_BTNFACE) :  xti->GetItemBackColor(nCol));
					COLORREF clrBkDC=pDC->GetBkColor();
					pDC->FillSolidRect(&rcErase,
						(GetFocus()!=this || !IsWindowEnabled()) ? 
						::GetSysColor(COLOR_BTNFACE) :  xti->GetItemBackColor(nCol));
					pDC->SetBkColor(clrBkDC);
				}
				else
				{
					clrBack=pImgList->SetBkColor(
						(GetFocus()!=this || !IsWindowEnabled()) ? 
						::GetSysColor(COLOR_BTNFACE) : ::GetSysColor(COLOR_HIGHLIGHT));
					COLORREF clrBkDC=pDC->GetBkColor();
					pDC->FillSolidRect(&rcErase,
						(GetFocus()!=this || !IsWindowEnabled()) ? 
						::GetSysColor(COLOR_BTNFACE) : ::GetSysColor(COLOR_HIGHLIGHT));
					pDC->SetBkColor(clrBkDC);
				}
			}
			else
			{
				if (xti->HasBackColor(nCol))
					pDC->FillSolidRect(&rcErase,
						(!IsWindowEnabled() ? ::GetSysColor(COLOR_BTNFACE) : xti->GetItemBackColor(nCol)));
				else
					pDC->FillSolidRect(&rcErase,(!IsWindowEnabled() ? 
						::GetSysColor(COLOR_BTNFACE) : GetTextBkColor()));
			}

			ptStart=rcItem.TopLeft();
			if(rcImage.Height()<rcErase.Height())
			{
				ptStart.y+=(rcErase.Height()-rcImage.Height())/2;
			}
			pImgList->Draw(pDC,nImg,ptStart,bRowSel ? ILD_FOCUS : ILD_TRANSPARENT);
			if(bRowSel)
			{
				pImgList->SetBkColor(clrBack);
			}
			rcItem.left += CRect(imi.rcImage).Width();
		}
	}
}	


BOOL COXTreeCtrl::SetItem(TV_ITEM *pTVI)
{
	// user must provide a valid handle
	ASSERT(pTVI->mask & TVIF_HANDLE);
	COXTreeItem *xti=GetXItem(pTVI->hItem);
	ASSERT(xti);
	UINT uMaskCb=0;// mask for send TVN_SETDISPINFO
	
	if(pTVI->mask & TVIF_IMAGE)
	{
		xti->m_tvi.iImage=pTVI->iImage;
		if(pTVI->iImage == I_IMAGECALLBACK)
			uMaskCb |= TVIF_IMAGE;
	}

	if(pTVI->mask & TVIF_TEXT)
	{
		if(pTVI->pszText == LPSTR_TEXTCALLBACK)
		{
			if(xti->m_tvi.pszText && xti->m_tvi.pszText != LPSTR_TEXTCALLBACK)
				delete [] xti->m_tvi.pszText;

			xti->m_tvi.pszText=LPSTR_TEXTCALLBACK;
			uMaskCb |= TVIF_TEXT;
		}
		else
		{
			// allocate space for new item text and copy string
			TCHAR * pChar=new TCHAR[_tcslen(pTVI->pszText) + 1];
			UTBStr::tcscpy(pChar, _tcslen(pTVI->pszText) + 1, pTVI->pszText);
			// The user specified new text for item
			if(xti->m_tvi.pszText && xti->m_tvi.pszText != LPSTR_TEXTCALLBACK)
				delete [] xti->m_tvi.pszText;
			xti->m_tvi.pszText=pChar;
		}
	}

	if (pTVI->mask & TVIF_STATE)
	{
		xti->m_tvi.state &= ~(pTVI->stateMask);
		xti->m_tvi.state |= pTVI->state;
		if (pTVI->stateMask & TVIS_STATEIMAGEMASK)
		{
			xti->m_tvi.state |= (pTVI->state & TVIS_STATEIMAGEMASK);
		}			
		if (pTVI->stateMask & TVIS_OVERLAYMASK)
		{
			xti->m_tvi.state |= (pTVI->state & TVIS_OVERLAYMASK) ? TVIS_OVERLAYMASK : 0;
		}
		if (pTVI->stateMask & TVIS_CUT)
		{
			xti->m_tvi.state |= (pTVI->state & TVIS_CUT) ? TVIS_CUT : 0;
		}
		if (pTVI->stateMask & TVIS_DROPHILITED)
		{
			xti->m_tvi.state |= (pTVI->state & TVIS_DROPHILITED) ? TVIS_DROPHILITED : 0;
		}
		if(pTVI->stateMask & TVIS_EXPANDED)
		{
			xti->m_tvi.state |= (pTVI->state & TVIS_EXPANDED) ? TVIS_EXPANDED : 0;
		}
	}
	
	if(pTVI->mask & TVIF_PARAM)
	{
		xti->m_tvi.lParam=pTVI->lParam;
	}

	if(pTVI->mask & TVIF_SELECTEDIMAGE)
	{
		xti->m_tvi.iSelectedImage=pTVI->iSelectedImage;
		if(pTVI->iSelectedImage == I_IMAGECALLBACK)
			uMaskCb |= TVIF_SELECTEDIMAGE;
	}

	if(pTVI->mask & TVIF_CHILDREN)
	{
		xti->m_tvi.cChildren=pTVI->cChildren;
	}

	if(uMaskCb != 0)
	{
		// If any callback detected, send TVN_SETDISPINFO notification to parent
		pTVI->mask=uMaskCb;
		SendSetDispInfoNotify(pTVI);
	}

	// Now translate TVIF_xxx to LVIF_xxx

	int nItem=GetItemIndexInternal(xti);
	if(nItem != -1) // item currently in list ctrl
	{
		LV_ITEM lvi;
		memset(&lvi, 0, sizeof(LV_ITEM));
		lvi.iItem=nItem;
		lvi.iSubItem=0;
 		if (pTVI->mask & TVIF_IMAGE)
		{
			lvi.mask |= LVIF_IMAGE;
			lvi.iImage=pTVI->iImage;
		}
		if (pTVI->mask & TVIF_STATE)
		{
			lvi.mask |= LVIF_STATE;
			if (pTVI->stateMask & TVIS_STATEIMAGEMASK)
			{
				lvi.state |= (pTVI->state & TVIS_STATEIMAGEMASK);
				lvi.stateMask |= LVIS_STATEIMAGEMASK;
			}			
			if (pTVI->stateMask & TVIS_SELECTED)
			{
				lvi.state |= (pTVI->state & TVIS_SELECTED) ? LVIS_SELECTED : 0;
				lvi.stateMask |= LVIS_SELECTED;
			}
			if (pTVI->stateMask & TVIS_OVERLAYMASK)
			{
				lvi.state |= (pTVI->state & TVIS_OVERLAYMASK) ? LVIS_OVERLAYMASK : 0;
				lvi.stateMask |= LVIS_OVERLAYMASK;
			}
			if (pTVI->stateMask & TVIS_CUT)
			{
				lvi.state |= (pTVI->state & TVIS_CUT) ? LVIS_CUT : 0;
				lvi.stateMask |= LVIS_CUT;
			}
			if (pTVI->stateMask & TVIS_DROPHILITED)
			{
				lvi.state |= (pTVI->state & TVIS_DROPHILITED) ? LVIS_DROPHILITED : 0;
				lvi.stateMask |= LVIS_DROPHILITED;
			}
			if(pTVI->stateMask & TVIS_EXPANDED)
			{
				UINT nCode=(pTVI->state & TVIS_EXPANDED)? TVE_EXPAND : TVE_COLLAPSE;
				Expand(pTVI->hItem,nCode);
			}
		}
		if(!CListCtrl::SetItem(&lvi))
			return FALSE;
	}

	if((m_hOldDropTarget!=pTVI->hItem) && (nItem!=-1) && 
		(pTVI->mask&TVIF_STATE) && (pTVI->stateMask&TVIS_DROPHILITED) && 
		(pTVI->state&TVIS_DROPHILITED))
	{
		if(m_hOldDropTarget!=NULL)
		{
			SetItemState(m_hOldDropTarget,0,TVIS_DROPHILITED);
		}
		m_hOldDropTarget=pTVI->hItem;
		UpdateWindow();
	}

	return TRUE;
}

void COXTreeCtrl::SendSetDispInfoNotify(TV_ITEM* pTVI)
{
	TV_DISPINFO di;

	di.hdr.code	 =TVN_SETDISPINFO;
	di.hdr.hwndFrom=GetSafeHwnd();
	di.hdr.idFrom  =GetDlgCtrlID();

	di.item=*pTVI;

	//Now send TVN_SETDISPINFO notification message to parent 
	GetParent()->SendMessage(WM_NOTIFY,di.hdr.idFrom,(LPARAM)&di);
}

BOOL COXTreeCtrl::SetItem(HTREEITEM hItem, UINT nMask, LPCTSTR lpszItem, int nImage, 
						  int nSelectedImage, UINT nState, UINT nStateMask, 
						  LPARAM lParam)
{
	UNREFERENCED_PARAMETER(nStateMask);
	UNREFERENCED_PARAMETER(nState);
	TV_ITEM tvi;
	memset(&tvi,0,sizeof(tvi));
	tvi.mask=nMask;
	tvi.hItem=hItem;
	tvi.pszText=(LPTSTR)lpszItem;
	tvi.iImage=nImage;
	tvi.iSelectedImage=nSelectedImage;
	tvi.lParam=lParam;
	return SetItem(&tvi);
}


BOOL COXTreeCtrl::GetItem(TV_ITEM *pTVI) const
{
	COXTreeItem *xti=GetXItem(pTVI->hItem);
	ASSERT(xti);
	UINT uMaskCb=0;// mask for send TVN_GEIDISPINFO if nesessary
	
	if(pTVI->mask & TVIF_TEXT)
	{
		if(xti->m_tvi.pszText == LPSTR_TEXTCALLBACK)
		{
			uMaskCb |= TVIF_TEXT;
		}
		else if(xti->m_tvi.pszText)
		{
			UTBStr::tcsncpy(pTVI->pszText, pTVI->cchTextMax, xti->m_tvi.pszText,pTVI->cchTextMax);
		}
	}
	
	if(pTVI->mask & TVIF_IMAGE)
	{
		if(xti->m_tvi.iImage == I_IMAGECALLBACK)
			uMaskCb |= TVIF_IMAGE;
		pTVI->iImage=xti->m_tvi.iImage;
	}

	if(pTVI->mask & TVIF_PARAM)
		pTVI->lParam=xti->m_tvi.lParam;
	
	if(pTVI->mask & TVIF_SELECTEDIMAGE)
	{
		if(xti->m_tvi.iSelectedImage == I_IMAGECALLBACK)
			uMaskCb |= TVIF_SELECTEDIMAGE;
		pTVI->iSelectedImage=xti->m_tvi.iSelectedImage;
	}

	if (pTVI->mask & TVIF_STATE)
	{
		int idx=GetItemIndexInternal(xti);
		xti->m_tvi.state &= ~(TVIS_SELECTED | LVIS_FOCUSED | TVIS_EXPANDED | 
			TVIS_EXPANDEDONCE);
		if(idx != -1)
		{
			UINT uStt=CListCtrl::GetItemState(idx,LVIS_SELECTED | LVIS_FOCUSED);
			if(uStt & LVIS_SELECTED)
				xti->m_tvi.state |= TVIS_SELECTED;
			if(uStt & LVIS_FOCUSED)
				xti->m_tvi.state |= LVIS_FOCUSED;
		}
		if(xti->IsExpanded())
			xti->m_tvi.state |= TVIS_EXPANDED;
		if(xti->IsExpandedOnce())
			xti->m_tvi.state |= TVIS_EXPANDEDONCE;
		pTVI->state=(xti->m_tvi.state & pTVI->stateMask);
	}
	
	if(pTVI->mask & TVIF_CHILDREN)
	{
		pTVI->cChildren=xti->m_tvi.cChildren;
	}

	if(uMaskCb != 0)
	{
		pTVI->mask=uMaskCb;
		pTVI->lParam=xti->m_tvi.lParam;
		SendGetDispInfoNotify(pTVI);
	}
	return TRUE;
}

void COXTreeCtrl::SendGetDispInfoNotify(TV_ITEM *pTVI) const
{
	TV_DISPINFO di;
	di.hdr.code	 =TVN_GETDISPINFO;
	di.hdr.hwndFrom=GetSafeHwnd();
	di.hdr.idFrom  =GetDlgCtrlID();
	di.item=*pTVI;

	//Now send TVN_GETDISPINFO notification message to parent 
	GetParent()->SendMessage(WM_NOTIFY,di.hdr.idFrom,(LPARAM)&di);
	// save the changed item info
	*pTVI=di.item;
}

CString COXTreeCtrl::GetItemText(HTREEITEM hItem,int nCol) const
{
	static TCHAR t[_OX_MAX_ITEM_TEXT];
	ASSERT(nCol >= 0);

	// make sure that the string buffer is empty
	t[0] = 0;

	if(nCol == 0)
	{
		TV_ITEM item;
		item.hItem=hItem;
		item.mask=TVIF_TEXT;
		item.cchTextMax=sizeof(t)/sizeof(TCHAR);
		item.pszText=t;
		GetItem(&item);
		return t;
	}
	else
	{
		COXTreeItem * xti=GetXItem(hItem);
		if(xti)
			return xti->GetSubItemText(nCol);
	}
	return _T("");
}

BOOL COXTreeCtrl::GetItemImage(HTREEITEM hItem,int &nImg,int &nSelImg,int nCol)
{
	if(nCol == 0)
	{
		TV_ITEM tvi;
		memset(&tvi,0,sizeof(tvi));
		tvi.mask=TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_HANDLE;
		tvi.hItem=hItem;
		if(!GetItem(&tvi))
			return FALSE;
		nImg=tvi.iImage;
		nSelImg=tvi.iSelectedImage;
		return TRUE;
	}
	else
	{
		COXTreeItem* xti=GetXItem(hItem);
		if(!xti)
			return FALSE;
		int nIm=xti->GetSubItemImage(nCol);
		if(nIm == -1)
			return FALSE;
		nImg=nIm;
		nSelImg=0;
		return TRUE;
	}
}

BOOL COXTreeCtrl::SetItemImage(HTREEITEM hItem, int nImage, 
							   int nSelectedImage, int nCol)
{
	if(nCol == 0)
	{
		TV_ITEM tvi;
		memset(&tvi,0,sizeof(tvi));
		tvi.mask=TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_HANDLE;
		tvi.hItem=hItem;
		tvi.iImage=nImage;
		tvi.iSelectedImage=nSelectedImage;
		return SetItem(&tvi);
	}
	else
	{
		if(nImage!=nSelectedImage)
		{
			TRACE(_T("COXTreeCtrl::SetItemImage: the selected image is not supported for subitems, ignored\n"));
		}
		return SetSubItem(hItem,nCol,OX_SUBITEM_IMAGE,NULL,nImage);
	}
}

BOOL COXTreeCtrl::GetItemDrawEllipsis(HTREEITEM hItem, UINT& nEllipsisFormat, 
									  int nCol/*=0*/) const
{
	COXTreeItem* xti=GetXItem(hItem);
	if(!xti)
		return FALSE;
	nEllipsisFormat=xti->GetDrawEllipsis(nCol);
	return TRUE;
}

BOOL COXTreeCtrl::SetItemDrawEllipsis(HTREEITEM hItem, UINT nEllipsisFormat, 
									  int nCol/*=0*/)
{
	COXTreeItem* xti=GetXItem(hItem);
	if(!xti)
		return FALSE;
	xti->SetDrawEllipsis(nEllipsisFormat,nCol);
	return TRUE;
}

BOOL COXTreeCtrl::ModifyStyle(DWORD dwRemove, DWORD dwAdd, 
							  UINT nFlags/*=0*/)
{
	DWORD dwLCStyleAdd=0;// list control style to add
	DWORD dwLCStyleRemove=0;// list control style to remove
	
	if(dwRemove & TVS_SHOWSELALWAYS)
		dwLCStyleRemove |= LVS_SHOWSELALWAYS;

	if(dwRemove & TVS_EDITLABELS)
		dwLCStyleRemove |= LVS_EDITLABELS;
	
	if(dwAdd & TVS_SHOWSELALWAYS)
		dwLCStyleAdd |= LVS_SHOWSELALWAYS;
	
	if(dwAdd & TVS_EDITLABELS)
		dwLCStyleAdd |= LVS_EDITLABELS;
	// add common window styles
	
	dwLCStyleAdd |= (dwAdd & 0xffff0000);
	dwLCStyleRemove |= (dwRemove & 0xffff0000);


	if(dwLCStyleRemove || dwLCStyleAdd)
		if(!CListCtrl::ModifyStyle(dwLCStyleRemove,dwLCStyleAdd,nFlags))
			return FALSE;
	DWORD dwStyleOld=m_dwTCStyle;
	m_dwTCStyle |= dwAdd;
	m_dwTCStyle &= ~dwRemove;
	if(dwStyleOld != m_dwTCStyle)
		{
		Invalidate();
		}
	return TRUE;
}

DWORD COXTreeCtrl::GetStyle() const
{
	DWORD dwStyle=m_dwTCStyle | (IsWindowVisible()? 0x10000000L :0);
	return dwStyle;
}

BOOL COXTreeCtrl::ModifyExStyle(DWORD dwExStyleRemove,DWORD dwExStyleAdd, 
								UINT nFlags/*=0*/)
{
	DWORD dwLCStyleAdd=0;// list control style to add
	DWORD dwLCStyleRemove=0;// list control style to remove

	if(dwExStyleAdd & TVOXS_MULTISEL)
		dwLCStyleRemove |= LVS_SINGLESEL;
	
	if(dwExStyleRemove & TVOXS_MULTISEL)
		dwLCStyleAdd |= LVS_SINGLESEL;

	if(dwExStyleAdd & TVOXS_COLUMNHDR)
		dwLCStyleRemove |= LVS_NOCOLUMNHEADER;

	if(dwExStyleRemove & TVOXS_COLUMNHDR)
		dwLCStyleAdd |= LVS_NOCOLUMNHEADER;

	if(dwExStyleAdd & TVOXS_NOSCROLL)
		dwLCStyleAdd |= LVS_NOSCROLL;

	if(dwExStyleRemove & TVOXS_NOSCROLL)
		dwLCStyleRemove |= LVS_NOSCROLL;

	if(dwExStyleAdd & TVOXS_NOSORTHEADER)
		dwLCStyleAdd |= LVS_NOSORTHEADER;

	if(dwExStyleRemove & TVOXS_NOSORTHEADER)
		dwLCStyleRemove |= LVS_NOSORTHEADER;

	if(dwExStyleAdd & TVOXS_SHAREIMAGELISTS)
		dwLCStyleAdd |= LVS_SHAREIMAGELISTS;

	if(dwExStyleRemove & TVOXS_SHAREIMAGELISTS)
		dwLCStyleRemove |= LVS_SHAREIMAGELISTS;


	if(dwLCStyleRemove || dwLCStyleAdd)
		if(!CListCtrl::ModifyStyle(dwLCStyleRemove,dwLCStyleAdd,nFlags))
			return FALSE;

	if((dwExStyleAdd&TVOXS_ITEMTIPS) && !(m_dwExStyle&TVOXS_ITEMTIPS))
	{
#ifndef OX_TREECTRL_NOITEMTIPS
		m_ItemTip.Attach(this);
#endif
	}
	
	if((dwExStyleRemove&TVOXS_ITEMTIPS) && (m_dwExStyle&TVOXS_ITEMTIPS))
	{
#ifndef OX_TREECTRL_NOITEMTIPS
		m_ItemTip.Detach();
#endif
	}
	
	DWORD dwOld=m_dwExStyle;
	m_dwExStyle |= dwExStyleAdd;
	m_dwExStyle &= ~dwExStyleRemove;
	if(m_dwExStyle != dwOld)
		Invalidate();

	return TRUE;
}

DWORD COXTreeCtrl::GetExStyle() const
{
	return m_dwExStyle;
}

int COXTreeCtrl::InsertColumn(int nCol, const LV_COLUMN* pColumn)
{
	BOOL bRet= CListCtrl::InsertColumn(nCol,pColumn);
	return bRet;
}

int COXTreeCtrl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, 
							  int nFormat, int nWidth, int nSubItem)
{
	BOOL bRet=CListCtrl::InsertColumn(nCol,lpszColumnHeading,nFormat,
		nWidth,nSubItem);
	return bRet;
}

BOOL COXTreeCtrl::SetSubItem(HTREEITEM hItem, int nColumn, UINT uFlags,
							 LPCTSTR lpszText, int nImage, CFont *pFont, COLORREF clr, COLORREF clrBack)
{
	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return FALSE;
	if(nColumn <= 0)
		return FALSE;
	BOOL bRet=xti->SetSubItem(nColumn,uFlags,lpszText,nImage,pFont,clr,clrBack);
	int idx=GetItemIndexInternal(xti);
	if(bRet && idx != -1)
	{
		CListCtrl::RedrawItems(idx,idx);
	}
	return bRet;
}

BOOL COXTreeCtrl::HideItem(HTREEITEM hItem,BOOL bHide)
{
	ASSERT(hItem != TVI_ROOT);
	if (hItem==TVI_ROOT)
		return FALSE;

	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return FALSE;
	if(xti->IsHidden() == bHide)
		return TRUE; // already hidden

	SetRedraw(FALSE);

	BOOL bResult=TRUE;

	HTREEITEM hPrevVisibleItem=GetPrevVisibleItem(hItem);
	HTREEITEM hPrevSiblingItem=GetPrevSiblingItem(hItem);

	xti->SetHidden(bHide);
	int idx=GetItemIndexInternal(xti);
	if(idx == -1 && bHide)
	{
		bResult=TRUE;
	}
	else
	{
		BOOL bItemExp=xti->IsExpanded();
		if(bHide && !bItemExp)
		{
			bResult=CListCtrl::DeleteItem(idx);
		}
		else if(bHide && bItemExp)
		{
			xti->RemoveChildrenFromCtrl(this);
			bResult=CListCtrl::DeleteItem(idx);
		}
		else if(xti->IsVisible())
		{
			if(!AddItemToCtrl(xti))
			{
				bResult=FALSE;
			}
			else
			{
				idx=GetItemIndexInternal(xti);
				ASSERT(idx != -1);
				if(xti->IsExpanded())
				{
					xti->AddChildrenToCtrl(this,idx + 1);
				}
			}
		}
	}

	if(bResult && hPrevVisibleItem!=NULL && hPrevSiblingItem!=NULL)
		RedrawTreeItems(hPrevSiblingItem,hPrevVisibleItem);

	SetRedraw(TRUE);
	
	return bResult;
}

BOOL COXTreeCtrl::DeleteItem(HTREEITEM hItem)
{
	HTREEITEM hParentItem=GetParentItem(hItem);

	TV_ITEM tvi;
	COXTreeItem *xti=GetXItem(hItem);
	if(xti != &m_xtiRoot && !HideItem(hItem))
		return FALSE;
	if(xti == &m_xtiRoot)
	{
		::ZeroMemory(&tvi, sizeof(tvi));
		tvi.hItem=xti->m_tvi.hItem;
		SendDeleteItemNotify(&tvi);
		xti->DeleteChildren();
		m_nItemsCount=0;
		return CListCtrl::DeleteAllItems();
	}
	
	if(hParentItem!=NULL)
	{
		COXTreeItem *xtiParent=GetXItem(hParentItem);
		ASSERT(xtiParent!=NULL);
		if(xtiParent->GetChildrenCount()<=1 && xtiParent->m_tvi.cChildren==1)
		{
			xtiParent->m_tvi.cChildren=0;
			if(xtiParent!=&m_xtiRoot)
			{
				xtiParent->m_bExpand=FALSE;
				xtiParent->m_bExpandedOnce=FALSE;
			}
		}
	}

	memset(&tvi,0,sizeof(tvi));
	tvi=xti->m_tvi;
	tvi.mask |= TVIF_HANDLE;
	tvi.hItem=(HTREEITEM) xti;

	SendDeleteItemNotify(&tvi);
	xti->RemoveChildrenFromCtrl(this);

	// item should have been already deleted from list
	ASSERT(GetItemIndexInternal(xti)==-1);
	delete xti;
	ASSERT(m_nItemsCount);
	m_nItemsCount--;

	if (hParentItem != NULL)
	{
		COXTreeItem* pParent = GetXItem(hParentItem);
		if (pParent->GetChildrenCount() == 0)
			RedrawTreeItems(hParentItem, hParentItem);
	}

	return TRUE;
}

BOOL COXTreeCtrl::DeleteAllItems()
{
	ASSERT(&m_xtiRoot);
	return DeleteItem(GetRootItem());
}

BOOL COXTreeCtrl::DeleteColumn(int nCol)
{
	if(nCol==0)
	{
		if(!DeleteAllItems())
		{
			return FALSE;
		}
		while(CListCtrl::DeleteColumn(0));
		return TRUE;
	}

	if(DeleteSubitems(GetRootItem(),nCol))
	{
		return CListCtrl::DeleteColumn(nCol);
	}
	else
	{
		return FALSE;
	}
}

BOOL COXTreeCtrl::DeleteSubitems(HTREEITEM hParentItem, int nCol)
{
	BOOL bSuccess=TRUE;
	HTREEITEM hItem=GetNextItem(hParentItem,TVGN_CHILD);	
	while(hItem!=NULL)
	{
		COXTreeItem* pItem=GetXItem(hItem);
		ASSERT(pItem!=NULL);
		pItem->DeleteSubitem(nCol);
		if(!DeleteSubitems(hItem,nCol))
		{
			bSuccess=FALSE;
			break;
		}
		hItem=GetNextItem(hItem,TVGN_NEXT);
	}

	return (bSuccess);
}


UINT COXTreeCtrl::GetVisibleCount() const
{
	return CListCtrl::GetItemCount();
}

UINT COXTreeCtrl::GetCount() const
{
	return m_nItemsCount;
}

HTREEITEM COXTreeCtrl::GetNextItem(HTREEITEM hItem, UINT nCode)
{
	COXTreeItem * xti=GetXItem(hItem);
	int idx;
	switch(nCode)
	{
	case TVGN_CHILD:
		if(!xti)
			return NULL;
		return (HTREEITEM)(xti->pxFirstChild);

	case TVGN_FIRSTVISIBLE:
		return GetFirstVisibleItem();

	case TVGN_NEXTVISIBLE:
		return GetNextVisibleItem(hItem,FALSE);

	case TVGN_PREVIOUSVISIBLE:
		return GetPrevVisibleItem(hItem,FALSE);

	case TVGN_DROPHILITE:
		{
			if(!xti)
				idx=-1;
			else
				idx=GetItemIndexInternal(xti);
			int idNext=CListCtrl::GetNextItem(idx,LVNI_DROPHILITED);
			if(idNext == -1 || idNext >= CListCtrl::GetItemCount())
				return NULL;
			return (HTREEITEM) CListCtrl::GetItemData(idNext);
		}

	case TVGN_NEXT:
		if(!xti)
			return NULL;
		return (HTREEITEM)(xti->pxNext);

	case TVGN_CARET:
		idx=CListCtrl::GetNextItem(-1,LVNI_FOCUSED);
		if(idx == -1)
		{
			return NULL;
		}
		else
		{
			HTREEITEM hti=GetItemFromIndex(idx);
			if(hti && GetItemState(hti,LVIS_SELECTED)==LVIS_SELECTED)
			{
				return hti;
			}
			else
			{
				return NULL;
			}
		}
	
	case TVGN_PARENT:
		if(!xti)
			return NULL;
		return (HTREEITEM) (xti->pxParent);
	
	case TVGN_PREVIOUS:
		if(!xti)
			return NULL;
		return (HTREEITEM) (xti->pxPrev);
	
	case TVGN_ROOT:
		if(!xti)
			return NULL;
		return (HTREEITEM)(xti->pxParent ? xti->pxParent->pxFirstChild : NULL);
	
	case TVGN_FIRSTSELECTED:
		idx=CListCtrl::GetNextItem(-1,LVNI_SELECTED);
		if(idx == -1)
			return NULL;
		return (HTREEITEM) CListCtrl::GetItemData(idx);
	
	case TVGN_NEXTSELECTED:
		if(!xti)
			return NULL;
		idx=GetItemIndexInternal(xti);
		if(idx == -1)
			return NULL;
		idx=CListCtrl::GetNextItem(idx,LVNI_SELECTED);
		if(idx == -1)
			return NULL;
		return (HTREEITEM) CListCtrl::GetItemData(idx);
	
	default:
		ASSERT(FALSE);
		return NULL;

	}
}

HTREEITEM COXTreeCtrl::GetFirstVisibleItem()
{
	int idx=CListCtrl::GetTopIndex();
	if(idx == -1 || idx >= CListCtrl::GetItemCount())
		return NULL;
	return (HTREEITEM) CListCtrl::GetItemData(idx);
}

HTREEITEM COXTreeCtrl::GetNextVisibleItem(HTREEITEM hItem, 
										  BOOL bCurrentlyViewable/*=FALSE*/)
{
	COXTreeItem *xti=GetXItem(hItem);
	// walk down throuth tree structure and find the next item in list ctrl
	if(!xti)
	{
		return NULL;
	}

	CRect rect;
	GetClientRect(rect);
	HTREEITEM hItemFirstVisible=GetFirstVisibleItem();
	ASSERT(hItemFirstVisible!=NULL);
	CRect rectFistVisible;
	GetItemRect(hItemFirstVisible,rectFistVisible,FALSE);
	rect.top=rectFistVisible.top;

	xti=xti->GetNextInTree();
	for(; xti ; xti=xti->GetNextInTree())
	{
		int idx=GetItemIndexInternal(xti);
		if(idx != -1)
		{
			if(bCurrentlyViewable)
			{
				CRect rectItem;
				CListCtrl::GetItemRect(idx,rectItem,LVIR_BOUNDS);
				if((rectItem.top>=rect.top && rectItem.top<=rect.bottom) ||
					(rectItem.bottom>=rect.top && rectItem.bottom<=rect.bottom))
				{
					return (HTREEITEM) CListCtrl::GetItemData(idx);
				}
				break;
			}
			else
			{
				return (HTREEITEM) CListCtrl::GetItemData(idx);
			}
		}
	}
	return NULL;
}

HTREEITEM COXTreeCtrl::GetPrevVisibleItem(HTREEITEM hItem, 
										  BOOL bCurrentlyViewable/*=FALSE*/)
{
	COXTreeItem *xti=GetXItem(hItem);
	// walk up throuth tree structure and find the item in list ctrl
	if(!xti)
		return NULL;

	CRect rect;
	GetClientRect(rect);
	HTREEITEM hItemFirstVisible=GetFirstVisibleItem();
	if(hItemFirstVisible==NULL)
		return NULL;

	CRect rectFistVisible;
	GetItemRect(hItemFirstVisible,rectFistVisible,FALSE);
	rect.top=rectFistVisible.top;

	xti=xti->GetPrevInTree();
	for(; xti ; xti=xti->GetPrevInTree())
	{
		int idx=GetItemIndexInternal(xti);
		if(idx != -1)
		{
			if(bCurrentlyViewable)
			{
				CRect rectItem;
				CListCtrl::GetItemRect(idx,rectItem,LVIR_BOUNDS);
				if((rectItem.top>=rect.top && rectItem.top<=rect.bottom) ||
					(rectItem.bottom>=rect.top && rectItem.bottom<=rect.bottom))
				{
					return (HTREEITEM) CListCtrl::GetItemData(idx);
				}
				break;
			}
			else
			{
				return (HTREEITEM) CListCtrl::GetItemData(idx);
			}
		}
	}
	return NULL;
}

HTREEITEM COXTreeCtrl::GetFirstHiddenItem()
{
	COXTreeItem *xti=&m_xtiRoot;// start form root item

	xti=xti->GetNextInTree();
	// now iterate the tree and find first hidden item
	for(; xti ; xti=xti->GetNextInTree())
	{
		if(xti->IsHidden())
			return (HTREEITEM) xti;
	}
	return NULL;
}

HTREEITEM COXTreeCtrl::GetNextHiddenItem(HTREEITEM hItem)
{
	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return NULL;
	xti=xti->GetNextInTree();
	// now iterate the tree and stop when a hidden item found
	for(; xti ; xti=xti->GetNextInTree())
	{
		if(xti->IsHidden())
			return (HTREEITEM) xti;
	}
	return NULL;
}

HTREEITEM COXTreeCtrl::GetPrevHiddenItem(HTREEITEM hItem)
{
	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return NULL;
	xti=xti->GetPrevInTree();
	// now iterate the tree and stop when a hidden item found
	for(; xti ; xti=xti->GetPrevInTree())
	{
		if(xti->IsHidden())
			return (HTREEITEM) xti;
	}
	return NULL;
}

HTREEITEM COXTreeCtrl::GetSelectedItem()
{
	return GetNextItem(TVI_ROOT,TVGN_FIRSTSELECTED);
}

CImageList* COXTreeCtrl::GetImageList(UINT nImage)
{
	UINT nListImg;
	switch(nImage)
	{
	case TVSIL_NORMAL:
		nListImg=LVSIL_SMALL;
		break;
	case TVSIL_STATE:
		nListImg=LVSIL_STATE;
	default:
		ASSERT(FALSE);
		return NULL;
	}
	return CListCtrl::GetImageList(nListImg);
}

UINT COXTreeCtrl::GetIndent() const
{
	return m_nIndent;
}

void COXTreeCtrl::SetIndent(UINT nIndent)
{
	nIndent=nIndent>TV_MININDENT ? nIndent : TV_MININDENT;
	m_nIndent=nIndent;
	RedrawWindow();
}

BOOL COXTreeCtrl::ItemHasChildren(HTREEITEM hItem) const
{
	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return FALSE;
	return xti->ItemHasChildren();
}

HTREEITEM COXTreeCtrl::GetChildItem(HTREEITEM hItem)
{
	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return NULL;
	return (HTREEITEM) (xti->pxFirstChild);
}

HTREEITEM COXTreeCtrl::GetNextSiblingItem(HTREEITEM hItem)
{
	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return NULL;
	return (HTREEITEM) (xti->pxNext);
}

HTREEITEM COXTreeCtrl::GetPrevSiblingItem(HTREEITEM hItem)
{
	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return NULL;
	return (HTREEITEM) (xti->pxPrev);
}

HTREEITEM COXTreeCtrl::GetParentItem(HTREEITEM hItem)
{
	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return NULL;
	return (HTREEITEM) (xti->pxParent);
}

HTREEITEM COXTreeCtrl::GetDropHilightItem()
{
	int idx=CListCtrl::GetNextItem(-1,LVNI_DROPHILITED);
	if(idx == -1 || idx >= CListCtrl::GetItemCount())
		return NULL;
	return (HTREEITEM) CListCtrl::GetItemData(idx);
}

HTREEITEM COXTreeCtrl::GetRootItem() const
{
	return (HTREEITEM)(&m_xtiRoot);
}

UINT COXTreeCtrl::GetItemState(HTREEITEM hItem, UINT nStateMask) const
{
	TV_ITEM tvi;
	memset(&tvi,0,sizeof(tvi));
	tvi.mask=TVIF_HANDLE | TVIF_STATE;
	tvi.hItem=hItem;
	tvi.stateMask=nStateMask;
	VERIFY(GetItem(&tvi));
	return tvi.state;
}

BOOL COXTreeCtrl::SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask)
{
	TV_ITEM tvi;
	memset(&tvi,0,sizeof(tvi));
	tvi.mask=TVIF_HANDLE | TVIF_STATE;
	tvi.hItem=hItem;
	tvi.stateMask=nStateMask;
	tvi.state=nState;
	return SetItem(&tvi);
}

DWORD COXTreeCtrl::GetItemData(HTREEITEM hItem) const
{
	TV_ITEM tvi;
	memset(&tvi,0,sizeof(tvi));
	tvi.mask=TVIF_HANDLE | TVIF_PARAM;
	tvi.hItem=hItem;
	VERIFY(GetItem(&tvi));
	return (DWORD)tvi.lParam;
}

BOOL COXTreeCtrl::SetItemData(HTREEITEM hItem, DWORD dwData)
{
	TV_ITEM tvi;
	memset(&tvi,0,sizeof(tvi));
	tvi.mask=TVIF_HANDLE | TVIF_PARAM;
	tvi.hItem=hItem;
	tvi.lParam=dwData;
	return SetItem(&tvi);
}

BOOL COXTreeCtrl::GetItemRect(HTREEITEM hItem, LPRECT lpRect, BOOL bTextOnly) const
{
	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return FALSE;
	int idx=GetItemIndexInternal(xti);
	if(idx == -1 || idx >= CListCtrl::GetItemCount())
		return FALSE;
	CRect r;
	if(bTextOnly)
	{
		int nIndent=-1;
		GetItemPartRect(rText,xti,&r,&nIndent);
		::SetRect(lpRect,r.left,r.top,r.right,r.bottom);
		return TRUE;
	}
	return CListCtrl::GetItemRect(idx,lpRect,LVIR_BOUNDS);
}

BOOL COXTreeCtrl::SetItemColor(HTREEITEM hItem,COLORREF clr,int nCol)
{
	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return FALSE;
	int idx=GetItemIndexInternal(xti);
	if(!nCol)
	{
		xti->SetColor(clr);
	}
	else
	{
		COXTreeSubItem *pSubItem=xti->GetSubItem(nCol);
		if(!pSubItem)
			return FALSE;
		pSubItem->SetColor(clr);
	}
	if(idx != -1)
		CListCtrl::RedrawItems(idx,idx);
	return TRUE;
}

BOOL COXTreeCtrl::SetItemBackColor(HTREEITEM hItem,COLORREF clr,int nCol)
{
	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return FALSE;
	int idx=GetItemIndexInternal(xti);
	if(!nCol)
	{
		xti->SetBackColor(clr);
	}
	else
	{
		COXTreeSubItem *pSubItem=xti->GetSubItem(nCol);
		if(!pSubItem)
			return FALSE;
		pSubItem->SetBackColor(clr);
	}
	if(idx != -1)
		CListCtrl::RedrawItems(idx,idx);
	return TRUE;
}

COLORREF COXTreeCtrl::GetItemColor(HTREEITEM hItem,int nCol) const
{
	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return 0;
	if(xti->HasColor(nCol))
		return xti->GetItemColor(nCol);
	return ::GetSysColor(COLOR_BTNTEXT);
}

CFont* COXTreeCtrl::GetItemFont(HTREEITEM hItem,int nCol)
{
	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return NULL;
	return xti->GetItemFont(nCol);
}

BOOL COXTreeCtrl::SetItemFont(HTREEITEM hItem,CFont *pFont,int nCol)
{
	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return FALSE;
	BOOL bRes=xti->SetFont(nCol,pFont);
	int idx=GetItemIndexInternal(xti);
	if(bRes && idx != -1 && idx < CListCtrl::GetItemCount())
		CListCtrl::RedrawItems(idx,idx);
	return bRes;
}

BOOL COXTreeCtrl::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo= (LV_DISPINFO*) pNMHDR;
	*pResult = 0;

	// We need to fire TVN_BEGINLABELEDIT Notification
	NMTVDISPINFO tvdi;
	::memset(&tvdi, 0, sizeof(NMTVDISPINFO));
	tvdi.hdr.hwndFrom = pNMHDR->hwndFrom;
    tvdi.hdr.idFrom = pNMHDR->idFrom;
    tvdi.hdr.code = pNMHDR->code;
	tvdi.item.mask = TVIF_HANDLE | TVIF_PARAM;
	tvdi.item.hItem = (HTREEITEM)(INT_PTR) pDispInfo->item.iItem;
	tvdi.item.lParam = pDispInfo->item.lParam;
	*pResult = GetParent()->SendMessage(TVN_BEGINLABELEDIT, pNMHDR->idFrom, (LPARAM) &tvdi);
	
	int nItem=pDispInfo->item.iItem;
	HWND hWndEdit=(HWND)::SendMessage(m_hWnd, LVM_GETEDITCONTROL, 0, 0L);
	
	if(m_pActiveEditWnd!=NULL && m_pActiveEditWnd->GetSafeHwnd())
		return FALSE;

	ASSERT(nItem != -1);
	COXTreeItem *xti=(COXTreeItem *) CListCtrl::GetItemData(nItem);
	ASSERT(xti);
	if(xti->IsDisabled())
	{
		if(pResult != NULL)
			*pResult = TRUE;
		return TRUE;
	}
	CRect rcEdit;
	int nCol=GetEditColumn(xti,&rcEdit);
	if(nCol == -1)
	{
		// No editable column found
		if(pResult != NULL)
			*pResult = TRUE;
		return TRUE;
	}

	if(xti->GetEditMode(nCol)==OXET_NOEDIT)
	{
		// No editable column found
		if(pResult != NULL)
			*pResult = TRUE;
		return TRUE;
	}

	m_nEditColumn=nCol;
	m_pActiveEditWnd=StartEditSubItem(hWndEdit,xti,rcEdit,nCol,pResult);

	if (GetParent() != NULL && GetParent()->SendMessage(WM_USER_QUERY_PROPERTIESWND))
		return FALSE;
	else
		return TRUE;
}

int COXTreeCtrl::GetEditColumn(COXTreeItem *xti,CRect * rcColText)
	// --- In  :
	// --- Out : 
	// --- Returns : specified item's column to edit
	//--- Effect :
{
	// fill rcItem with entry item rect
	CRect rcItem;
	int nIndex=GetItemIndexInternal(xti);
	if(nIndex == -1)
		return -1;
	CListCtrl::GetItemRect(nIndex,&rcItem,LVIR_BOUNDS);

	CImageList *pImgList=CListCtrl::GetImageList(LVSIL_SMALL);
	CImageList *pStateImgList=CListCtrl::GetImageList(LVSIL_STATE);

	// Now find the column
	int nColumnsCount=GetColumnsCount();
	int nCol=-1;
	for(int colnum=0; colnum < nColumnsCount; colnum++)
	{
		int nColWidth=GetColumnWidth(colnum);
		if(m_ptLastClick.x >= rcItem.left && 
			m_ptLastClick.x <= (rcItem.left + nColWidth))
		{
			nCol=colnum;
			break;
		}
		rcItem.left += nColWidth;
	}
	if(nCol == -1)
		return -1;// no column found
	if(nCol && !xti->GetSubItem(nCol))
		return -1; // this item has no subitems at given column

	rcItem.right=rcItem.left + GetColumnWidth(nCol);
	
	int nIndent=nCol == 0 ? GetItemIndent(xti) : 0;

//	CSize szText=GetColTextExtent(nIndex,nCol);
	CSize szText=CSize(rcItem.Width(),rcItem.Height());
	CRect rcText;
	
	LV_COLUMN lvc;
	memset(&lvc, 0, sizeof(LV_COLUMN));
	lvc.mask=LVCF_FMT;
	VERIFY(GetColumn(nCol, &lvc));

	int nImgWidth=0;
	IMAGEINFO imgInfo;
	int nImg=xti->GetItemImage(nCol);

	if(pImgList && nImg != -1)
	{
		pImgList->GetImageInfo(0,&imgInfo);
		CRect rcImg(imgInfo.rcImage);
		nImgWidth += rcImg.Width();
	}

	if(pStateImgList && nCol == 0 && GetItemStateIndex(nIndex) != -1)
	{
		pStateImgList->GetImageInfo(0,&imgInfo);
		CRect rcImg(imgInfo.rcImage);
		nImgWidth += rcImg.Width();
	}

	rcText.top=rcItem.top;
	rcText.bottom=rcItem.bottom;
	szText.cx=__min(szText.cx,GetColumnWidth(nCol)-nImgWidth-nIndent);
	int fmt=lvc.fmt & LVCFMT_JUSTIFYMASK;
	switch(fmt)
	{
	case LVCFMT_LEFT:
		{
			rcText.left=rcItem.left + nIndent + nImgWidth;
			rcText.right=rcText.left + szText.cx;
		}
		break;
	case LVCFMT_RIGHT:
		{
			rcText.right=rcItem.left + GetColumnWidth(nCol);
			rcText.left=rcText.right - szText.cx;
		}
		break;
	case LVCFMT_CENTER:
		{
			rcItem.left += nIndent + nImgWidth;
			int nCenter=rcItem.left + (rcItem.right - rcItem.left)/2;
			rcText.left=nCenter - szText.cx/2;
			rcText.right=nCenter + szText.cx/2;
		}
		break;
	default:
		ASSERT(FALSE);
	}
	(*rcColText)=rcText;
	if(rcText.Width() < 6)
		rcText.InflateRect(4,4);
	if(!rcText.PtInRect(m_ptLastClick))
		return -1;
	return nCol;
}


CEdit* COXTreeCtrl::EditLabel(HTREEITEM hti, int nCol/*=0*/)
{
	if(!(GetStyle()&TVS_EDITLABELS))
		return NULL;

	COXTreeItem* xti=GetXItem(hti);
	if(!xti)
		return NULL;

	int nIndex=GetItemIndex(hti);
	if(nIndex<0)
		return NULL;

	CRect rcItem;
	if(!GetItemRect(hti,&rcItem,TRUE))
		return NULL;
	int nColumnsCount=GetColumnsCount();
	if(nCol>=nColumnsCount)
		return NULL;

	if(nCol>0)
		rcItem.left=0;
	for(int colnum=0; colnum < nCol; colnum++)
		rcItem.left += GetColumnWidth(colnum);
	if(nCol>0)
		rcItem.right=rcItem.left + GetColumnWidth(nCol);
	
	m_ptLastClick=rcItem.CenterPoint()-CPoint(GetScrollPos(SB_HORZ),0);

	if(GetFocus() != this)
		SetFocus();

	return (CEdit*)CWnd::FromHandle((HWND)SendMessage(LVM_EDITLABEL,nIndex));
}

CWnd* COXTreeCtrl::StartEditSubItem(HWND hWndEdit,COXTreeItem *xti,CRect& rcEdit,
									int nCol,LRESULT* pResult)
	// --- In  :
	// --- Out : 
	// --- Returns :
	//--- Effect :
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int nEditType=xti->GetEditMode(nCol);
	switch(nEditType)
	{
	case OXET_COMBO:
		{
			int nLeft=-GetScrollPos(SB_HORZ);
			for(int i=0;i < nCol;i++)
				nLeft += GetColumnWidth(i);
			CRect rcCombo=rcEdit;
			rcCombo.left=nLeft;
			CImageList *pImgList=GetImageList(TVSIL_NORMAL);
			rcCombo.right=rcCombo.left + GetColumnWidth(nCol);
			if(xti->GetItemImage(nCol) != -1 &&pImgList)
			{
				IMAGEINFO ii;
				pImgList->GetImageInfo(0,&ii);
				rcCombo.left += CRect(ii.rcImage).Width();
			}
			if(rcCombo.right > rcClient.right)
			{
				// perform scrolling
				CSize szScroll;
				szScroll.cx=rcCombo.right  - rcClient.right;
				szScroll.cy=0;
				Scroll(szScroll);
				rcCombo.right -= szScroll.cx;
				rcCombo.left -= szScroll.cx;
			}
			rcCombo.bottom += rcEdit.Height()*5;
			(*pResult)=1;
			m_wndCombo.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST|WS_VSCROLL|WS_HSCROLL,
			rcCombo,this,100);
			m_wndCombo.SetFont(GetFont());

			m_wndCombo.Init((HTREEITEM) xti,GetItemIndexInternal(xti),nCol);

			m_wndCombo.SetFocus();
		}
		return &m_wndCombo;
	case OXET_EDIT:
		{
			m_wndEdit.Init();
			LV_COLUMN lvc;
			lvc.mask=LVCF_FMT;
			VERIFY(CListCtrl::GetColumn(nCol,&lvc));
			DWORD dwAlign=ES_LEFT;
			CPoint pos;
			switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
			{
			case LVCFMT_LEFT:
				pos=rcEdit.TopLeft();
				dwAlign=ES_LEFT;
				break;
			case LVCFMT_RIGHT:
				pos.x=rcEdit.right;
				pos.y=rcEdit.top;
				dwAlign=ES_RIGHT;
				break;
			case LVCFMT_CENTER:
				pos.x=rcEdit.left + rcEdit.Width()/2;
				pos.y=rcEdit.top;
				dwAlign=ES_CENTER;
				break;
			}
			// scroll control if column been edited is not visible
			if(pos.x < 0 || pos.x > rcClient.right)	
			{
				// perform scrolling
				CSize szScroll;
				szScroll.cx=pos.x  - rcClient.right;
				szScroll.cy=0;
				Scroll(szScroll);
				pos.x -= szScroll.cx;
			}
			m_wndEdit.SubclassWindow(hWndEdit);	
			m_wndEdit.KeepPos(pos,dwAlign);

			if ( m_dwExStyle & TVOXS_KEEPCOLUMNSIZE )
				m_wndEdit.KeepColumnSize(CSize(rcEdit.right,rcEdit.bottom));
			else
				m_wndEdit.KeepBounds(CSize(rcClient.Width(),rcClient.Height()));

			m_wndEdit.SetWindowText(GetItemText((HTREEITEM)xti,nCol));
			return &m_wndEdit;
		}
	case OXET_CALENDAR:
		{
			int nLeft=-GetScrollPos(SB_HORZ);
			for(int i=0;i < nCol;i++)
				nLeft += GetColumnWidth(i);
			CRect rcCal=rcEdit;
			rcCal.left=nLeft;
			CImageList *pImgList=GetImageList(TVSIL_NORMAL);
			rcCal.right=rcCal.left + GetColumnWidth(nCol);
			if(xti->GetItemImage(nCol) != -1 &&pImgList)
			{
				IMAGEINFO ii;
				pImgList->GetImageInfo(0,&ii);
				rcCal.left += CRect(ii.rcImage).Width();
			}
			if(rcCal.right > rcClient.right)
			{
				// perform scrolling
				CSize szScroll;
				szScroll.cx=rcCal.right  - rcClient.right;
				szScroll.cy=0;
				Scroll(szScroll);
				rcCal.right -= szScroll.cx;
				rcCal.left -= szScroll.cx;
			}
			(*pResult)=1;
			m_wndCal.Create(WS_CHILD | WS_VISIBLE |WS_BORDER,rcCal,this,7);
			m_wndCal.Init((HTREEITEM)xti,GetItemIndexInternal(xti),nCol);
			m_wndCal.SetFocus();
			return &m_wndCal;
		}
	default:
		return NULL;
	}
}

BOOL COXTreeCtrl::IsItemHidden(HTREEITEM hItem) const
{
	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return FALSE;
	return xti->IsHidden();
}

BOOL COXTreeCtrl::DisableItem(HTREEITEM hItem,BOOL bDisable)
{
	COXTreeItem *xti=GetXItem(hItem);
	if(xti->IsDisabled() == bDisable)
		return TRUE; // already disabled or undisabled
	xti->SetDisabled(bDisable);
	int idx=GetItemIndexInternal(xti);
	if(idx == -1)
		return TRUE;
	CListCtrl::RedrawItems(idx,idx);
	return TRUE;
}

BOOL COXTreeCtrl::IsItemDisabled(HTREEITEM hItem) const
{
	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return FALSE;
	return xti->IsDisabled();
}

BOOL COXTreeCtrl::IsItemDescendant(HTREEITEM hParentItem, HTREEITEM hItem)
{
	ASSERT(hParentItem!=NULL);
	ASSERT(hItem!=NULL);
	if(hParentItem==NULL || hItem==NULL)
	{
		return FALSE;
	}

	HTREEITEM hDirectParent=GetParentItem(hItem);
	if(hDirectParent==NULL)
	{
		return FALSE;
	}
	else
	{
		return (hParentItem==hDirectParent || 
			IsItemDescendant(hParentItem,hDirectParent));
	}
}


void COXTreeCtrl::SendDeleteItemNotify(TV_ITEM* ptvi)
{
	NM_TREEVIEW nmt;
	memset(&nmt,0,sizeof(nmt));
	
	nmt.itemOld=*ptvi;
	nmt.hdr.code=TVN_DELETEITEM;
	nmt.hdr.idFrom=GetDlgCtrlID();
	nmt.hdr.hwndFrom=GetSafeHwnd();
	GetParent()->SendMessage(WM_NOTIFY,nmt.hdr.idFrom , (LPARAM)&nmt);
}

BOOL COXTreeCtrl::SendItemExpandingNotify(HTREEITEM hItem,UINT nCode)
{
	COXTreeItem *xti=GetXItem(hItem);
	ASSERT(xti);
	NM_TREEVIEW nmt;

	memset(&nmt, 0, sizeof(nmt));
	
	nmt.hdr.idFrom=GetDlgCtrlID();
	nmt.hdr.hwndFrom=GetSafeHwnd();
	nmt.hdr.code=TVN_ITEMEXPANDING;
	nmt.action=nCode;

	nmt.itemNew=xti->m_tvi;
	nmt.itemNew.mask |= TVIF_HANDLE;
	nmt.itemNew.hItem=(HTREEITEM) xti;
	
	return (BOOL)(GetParent()->SendMessage(WM_NOTIFY,nmt.hdr.idFrom,(LPARAM)(&nmt)));
}

void COXTreeCtrl::SendItemExpandedNotify(HTREEITEM hItem,UINT nCode)
{
	COXTreeItem *xti=GetXItem(hItem);
	ASSERT(xti);

	NM_TREEVIEW nmt;
	memset(&nmt, 0, sizeof(nmt));
	
	nmt.hdr.idFrom=GetDlgCtrlID();
	nmt.hdr.hwndFrom=GetSafeHwnd();
	nmt.hdr.code=TVN_ITEMEXPANDED;
	nmt.action=nCode;

	nmt.itemNew=xti->m_tvi;
	nmt.itemNew.mask |= TVIF_HANDLE;
	nmt.itemNew.hItem=(HTREEITEM) xti;
	GetParent()->SendMessage(WM_NOTIFY,nmt.hdr.idFrom,(LPARAM)(&nmt));
}

BOOL COXTreeCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo=(LV_DISPINFO*)pNMHDR;
	if ((pDispInfo->item.pszText != NULL) && (pDispInfo->item.iItem != -1))
	{
		HTREEITEM hItem=(HTREEITEM) CListCtrl::GetItemData(pDispInfo->item.iItem);
		VERIFY(SetItemText(hItem,pDispInfo->item.pszText,m_nEditColumn));
		return FALSE;
	}

	*pResult=0;
	return FALSE;
}

BOOL COXTreeCtrl::SetItemText(HTREEITEM hItem, LPCTSTR lpszItem ,int nCol)
{
	COXTreeItem *xti=GetXItem(hItem);
	if(!xti)
		return FALSE;
	if(nCol == 0)
	{
		TV_ITEM tvi;
		memset(&tvi,0,sizeof(tvi));
		tvi.mask=TVIF_HANDLE | TVIF_TEXT;
		tvi.hItem=hItem;
		tvi.pszText=(LPTSTR)lpszItem;
		return SetItem(&tvi);
	}
	return xti->SetSubItemText(lpszItem,nCol);
}

BOOL COXTreeCtrl::GetPickAnywhere() const
{
	return (m_dwExStyle & TVOXS_PICKANYWHERE);
}

BOOL COXTreeCtrl::GetDrawGridFullLength() const
{
	return (m_dwExStyle & TVOXS_FLGRID);
}

BOOL COXTreeCtrl::HasGrid(UINT nGridType) const
{
	return (m_dwExStyle & nGridType);
}

BOOL COXTreeCtrl::ResizeColToFit(int nCol)
{
	if(nCol >= GetColumnsCount())
		return FALSE;
	int nWidth=GetFullColumnWidth(nCol);

	return SetColumnWidth(nCol,nWidth);
}


int COXTreeCtrl::GetFullColumnWidth(int nCol)
{
	if(GetItemCount()==0)
		return 6;

	int nImgWidth=0,nStateImgWidth=0;
	CImageList* pImgList=CListCtrl::GetImageList(LVSIL_SMALL);
	CImageList* pStateImgList=CListCtrl::GetImageList(LVSIL_STATE);
	
	// calc image width 
	if(pImgList && pImgList->GetImageCount() > 0)
	{
		IMAGEINFO imi;
		VERIFY(pImgList->GetImageInfo(0,&imi));
		nImgWidth=imi.rcImage.right - imi.rcImage.left;
	}

	// calc state image width 
	if(pStateImgList && pStateImgList->GetImageCount() > 0)
	{
		IMAGEINFO imi;
		VERIFY(pStateImgList->GetImageInfo(0,&imi));
		nStateImgWidth=imi.rcImage.right - imi.rcImage.left;
	}

	int nIndex=CListCtrl::GetNextItem(-1,LVNI_ALL);

	int nMaxWidth=0;
	CDC *pDC=GetDC();
	// look throuth currently visible items and find max width
	while(nIndex != -1)
	{
		int nWidth=0;
		COXTreeItem *xti=GetXItem((HTREEITEM) CListCtrl::GetItemData(nIndex));

		if(nCol == 0)
		{
			// add item indent
			nWidth += GetItemIndent(xti);
			// add item's state image width, if any
			if(pStateImgList)
			{
				int idx=STATEIMAGEMASKTOINDEX(xti->m_tvi.state)-1;
					if(idx != -1)
						nWidth += nStateImgWidth;
			}
			// add main image width
			nWidth += nImgWidth;
		}
		else
		{
			if(xti->GetSubItemImage(nCol) != -1)
				nWidth += nImgWidth;
		}
		// finally calc and add item's text width
		CFont *pFont=xti->HasFont(nCol) ? xti->GetItemFont(nCol) : GetFont();
		CFont *pOldFont=pDC->SelectObject(pFont);
		CSize szText=pDC->GetTextExtent(GetItemText((HTREEITEM) xti,nCol));
		pDC->SelectObject(pOldFont);
		nWidth += szText.cx + 6;
		if(nWidth > nMaxWidth)
			nMaxWidth=nWidth;
		// retreive next item
		nIndex=CListCtrl::GetNextItem(nIndex,LVNI_ALL);
	}
	ReleaseDC(pDC);
	return nMaxWidth;
}

void COXTreeCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(GetFocus() != this)
		SetFocus();
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void COXTreeCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
	// --- In  :
	// --- Out : 
	// --- Returns :
	//--- Effect :
{
	if(GetFocus() != this)
		SetFocus();

	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void COXTreeCtrl::SetEditMode(HTREEITEM hItem,UINT uMode,CStringArray& saTextEx,int nCol)
{
	COXTreeItem *xti=GetXItem(hItem);
	ASSERT(xti);
	xti->SetEditMode(uMode,saTextEx,nCol);
}

void COXTreeCtrl::SetPlainEditMode(HTREEITEM hItem,int nCol)
{
	CStringArray sArray;
	SetEditMode(hItem,OXET_EDIT,sArray,nCol);
}

void COXTreeCtrl::SetNoEditMode(HTREEITEM hItem,int nCol)
{
	CStringArray sArray;
	SetEditMode(hItem,OXET_NOEDIT,sArray,nCol);
}

void COXTreeCtrl::SetItemTextEx(HTREEITEM hItem,CStringArray& saTextEx,int nCol)
{
	COXTreeItem* xti=GetXItem(hItem);
	ASSERT(xti);
	if(xti)
		xti->SetTextEx(saTextEx,nCol);
}

CStringArray& COXTreeCtrl::GetItemTextEx(HTREEITEM hItem,int nCol)
{
	COXTreeItem *xti=GetXItem(hItem);
	ASSERT(xti);
	return xti->GetTextEx(nCol);
}

void COXTreeCtrl::PostNcDestroy() 
{
	m_bInit=FALSE;
	CListCtrl::PostNcDestroy();
}

void COXTreeCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if(m_nItemHeight)
	{
		lpMeasureItemStruct->itemHeight=max(m_nItemHeight,
			lpMeasureItemStruct->itemHeight); 
	}
	else
	{
		m_nItemHeight=lpMeasureItemStruct->itemHeight; 
	}
}

UINT COXTreeCtrl::GetItemHeight() const
{
	return m_nItemHeight;
}

void COXTreeCtrl::SetItemHeight(UINT nHeight)
{
	m_nItemHeight=nHeight;
	////
	CRect rect;
	GetWindowRect(rect);
	WINDOWPOS wp;
	wp.hwnd=m_hWnd;
	wp.cx=rect.Width();
	wp.cy=rect.Height();
	wp.flags=SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED,0,(LPARAM)&wp);
	////
}


BOOL COXTreeCtrl::EnsureVisible(HTREEITEM hti)
{
	COXTreeItem* xti=GetXItem(hti);
	if(!xti || xti==&m_xtiRoot)
	{
		return FALSE;
	}

	if(!xti->IsVisible())
	{
		if(xti->IsHidden())
		{
			xti->SetHidden(FALSE);
		}

		CPtrArray arrItemsToExpand;
		HTREEITEM htiParent=GetParentItem(hti);
		COXTreeItem* xtiParent;
		BOOL bFlag=FALSE;
		while(!bFlag && htiParent!=NULL)
		{
			xtiParent=GetXItem(htiParent);
			ASSERT(xtiParent);
			if(xtiParent==&m_xtiRoot ||
				(xtiParent->IsVisible() && xtiParent->IsExpanded()))
			{
				bFlag=TRUE;
			}
			else 
			{
				if(xtiParent->IsHidden())
				{
					xtiParent->SetHidden(FALSE);
				}
				if(!xtiParent->IsExpanded())
				{
					arrItemsToExpand.Add(htiParent);
				}
				htiParent=GetParentItem(htiParent);
			}
		}
		if(arrItemsToExpand.GetSize()>0)
		{
			for(int nIndex=(int)arrItemsToExpand.GetSize()-1; nIndex>=0; nIndex--)
				Expand((HTREEITEM)arrItemsToExpand[nIndex],TVE_EXPAND);
		}
	}

	int nIndex=GetItemIndex(hti);
	if(nIndex<0)
	{
		return FALSE;
	}
	return CListCtrl::EnsureVisible(nIndex,FALSE);
}

BOOL COXTreeCtrl::EnsureFirstVisible(HTREEITEM hti)
{
	if(!EnsureVisible(hti))
	{
		return FALSE;
	}

	HTREEITEM hItemFirstVisible=GetFirstVisibleItem();
	if(hItemFirstVisible==hti)
	{
		return TRUE;
	}

	int nIndex=GetItemIndex(hti);
	int nIndexFirstVisible=GetItemIndex(hItemFirstVisible);
	ASSERT(nIndex!=nIndexFirstVisible);

	CRect rect;
	GetItemRect(hti,rect,FALSE);
	CSize sizeScroll(0,(nIndex-nIndexFirstVisible)*rect.Height());
	CListCtrl::Scroll(sizeScroll);
	
	return TRUE;
}

BOOL COXTreeCtrl::Select(HTREEITEM hti, UINT nCode)
{
	if(nCode==TVGN_FIRSTVISIBLE)
	{
		return FALSE;
	}

	if(hti==NULL)
	{
		switch(nCode)
		{
		case TVGN_CARET:
			{
				HTREEITEM hItem=GetNextItem(TVI_ROOT,TVGN_FIRSTSELECTED);
				while(hItem)
				{
					SetItemState(hItem,0,TVIS_SELECTED);
					hItem=GetNextItem(hItem,TVGN_NEXTSELECTED);
				}

				break;
			}
		case TVGN_DROPHILITE:
			{
				if(m_hOldDropTarget!=NULL)
				{
					SetItemState(m_hOldDropTarget,0,TVIS_DROPHILITED);
					UpdateWindow();
					m_hOldDropTarget = NULL;
				}
			}
		}
		return TRUE;
	}

	COXTreeItem* xti=GetXItem(hti);
	if(xti==&m_xtiRoot && nCode==TVGN_DROPHILITE)
	{
		if(m_hOldDropTarget!=NULL)
		{
			SetItemState(m_hOldDropTarget,0,TVIS_DROPHILITED);
			UpdateWindow();
			m_hOldDropTarget = NULL;
		}
	}
	if(!xti || xti==&m_xtiRoot)
	{
		return FALSE;
	}

	int nIndex=GetItemIndexInternal(xti);

	BOOL bResult=FALSE;
	switch(nCode)
	{
	case TVGN_CARET:
		{
			bResult=SetItemState(hti,TVIS_SELECTED,TVIS_SELECTED) & 
				CListCtrl::SetItemState(nIndex,LVIS_FOCUSED,LVIS_FOCUSED);
			break;
		}
	case TVGN_DROPHILITE:
		{
			bResult=SetItemState(hti,TVIS_DROPHILITED,TVIS_DROPHILITED);
			break;
		}
	}

	return bResult;
}

BOOL COXTreeCtrl::SelectItem(HTREEITEM hti)
{
	return Select(hti,TVGN_CARET);
}

BOOL COXTreeCtrl::SelectDropTarget(HTREEITEM hti)
{
	return Select(hti,TVGN_DROPHILITE);
}

BOOL COXTreeCtrl::SelectSetFirstVisible(HTREEITEM hti)
{
	return Select(hti,TVGN_FIRSTVISIBLE);
}

CImageList* COXTreeCtrl::CreateDragImage(HTREEITEM hti, BOOL bCreateMultipleImage)
{
	int nIndex=GetItemIndex(hti);
	if(nIndex<0)
	{
		return NULL;
	}

	m_bCreatingDragImage=TRUE;

	CImageList* m_pDragImageList=new CImageList;

	UINT nCount=GetSelectedCount();
	nCount=(nCount>0 && bCreateMultipleImage) ? nCount : 1;

	CRect rectItem;
	GetItemRect(hti,&rectItem,FALSE);
	UINT nItemHeight=rectItem.Height();
	rectItem.bottom=nItemHeight*nCount;
	int cyScreen=GetSystemMetrics(SM_CYSCREEN);
	rectItem.bottom=rectItem.bottom>cyScreen ? cyScreen : rectItem.bottom;
	rectItem.top=rectItem.left=0;
	rectItem.right=GetColumnWidth(0);
	m_pDragImageList->Create(rectItem.Width(),rectItem.Height(),TRUE,0,1);

	CClientDC dcClient(this);
	CDC dc;
	dc.CreateCompatibleDC(&dcClient);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dcClient,rectItem.Width(),rectItem.Height());
	CBitmap* pOldBitmap=dc.SelectObject(&bitmap);
	CFont* pOldFont=dc.SelectObject(GetFont());

	HTREEITEM hSelectedItem=NULL;
	if(nCount>1)
	{
		hSelectedItem=GetNextItem(TVI_ROOT,TVGN_FIRSTSELECTED);
		ASSERT(hSelectedItem);
		nIndex=GetItemIndex(hSelectedItem);
		rectItem.bottom=rectItem.top+nItemHeight;
	}

	while(TRUE)
	{
		DRAWITEMSTRUCT dis;
		dis.CtlType=ODT_LISTVIEW;
		dis.CtlID=0;
		dis.itemID=nIndex;
		dis.itemAction=ODA_DRAWENTIRE;
		dis.itemState=ODS_DEFAULT;
		dis.hwndItem=NULL;
		dis.hDC=dc;
		dis.rcItem=rectItem;
		dis.itemData=GetXItem(hti)->m_tvi.lParam;
		DrawItem(&dis);

		if(nCount==1)
		{
			break;
		}
		else
		{
			hSelectedItem=GetNextItem(hSelectedItem,TVGN_NEXTSELECTED);
			if(hSelectedItem)
			{
				nIndex=GetItemIndex(hSelectedItem);
				rectItem.top+=nItemHeight;
				rectItem.bottom+=nItemHeight;
			}
			else
			{
				break;
			}
		}
	}
	
	if(pOldBitmap)
	{
		dc.SelectObject(pOldBitmap);
	}
	if(pOldFont)
	{
		dc.SelectObject(pOldFont);
	}
	m_pDragImageList->Add(&bitmap,(!IsWindowEnabled() ? 
		::GetSysColor(COLOR_BTNFACE) : GetTextBkColor()));

	m_bCreatingDragImage=FALSE;
	return m_pDragImageList;
}

BOOL COXTreeCtrl::SortChildren(HTREEITEM hti, int nCol/*=0*/, 
							   BOOL bAscending/*=TRUE*/, 
							   PFNTVCOMPARE lpfnCompare/*=NULL*/, 
							   LPARAM lParam/*=NULL*/)
{
	COXTreeHeader* pHeader=(COXTreeHeader*)GetDlgItem(0);
	ASSERT(pHeader);
	pHeader->SortColumn(nCol,bAscending ? 1 : -1);

	CWaitCursor waitCursor;
	SetRedraw(FALSE);

	hti=hti==NULL ? GetRootItem() : hti;
	COXTreeItem* xti=GetXItem(hti);
	ASSERT(xti);
	BOOL bOldExpand=xti->IsExpanded();
	if(bOldExpand)
	{
		xti->SetExpand(FALSE);
		COXTreeItem *xtiTemp=xti->pxFirstChild;
		while(xtiTemp)
		{
			if(!xtiTemp->IsHidden())
			{
				int nIndex=GetItemIndexInternal(xtiTemp);
				ASSERT(nIndex!=-1);
				xtiTemp->RemoveChildrenFromCtrl(this);
				CListCtrl::DeleteItem(nIndex);
			}
			xtiTemp=xtiTemp->pxNext;
		}
	}



	if (lpfnCompare != NULL)
		xti->SortChildren(nCol,(hti==GetRootItem() ? FALSE : TRUE),
			bAscending,lpfnCompare,lParam);
	else
	{
		// Find out if a compare function for that column
		// has been specified.
		PFNTVCOMPARE lpfnColumnCompare = NULL;
		m_mapCompareFunctions.Lookup(nCol, lpfnColumnCompare);

		if (lpfnColumnCompare != NULL)
			xti->SortChildren(nCol,(hti==GetRootItem() ? FALSE : TRUE),
				bAscending,lpfnColumnCompare,lParam);
		else // default compare function
			xti->SortChildren(nCol,(hti==GetRootItem() ? FALSE : TRUE),
				bAscending,m_lpfnDefaultCompare,lParam);
	}

	if(bOldExpand)
	{
		xti->SetExpand(TRUE);
		COXTreeItem *xtiTemp=xti->pxFirstChild;
		int pos=GetItemIndexInternal(xti) + 1;
		while(xtiTemp)
		{
			if(!xtiTemp->IsHidden())
			{
				SetItemAtPos(pos,xtiTemp);
				pos ++;
				pos=xtiTemp->AddChildrenToCtrl(this,pos);
			}
			xtiTemp=xtiTemp->pxNext;
		}
	}

	SetRedraw(TRUE);
	waitCursor.Restore();

	return TRUE;
}

BOOL COXTreeCtrl::SortChildrenCB(LPTV_SORTCB pSort, int nCol/*=0*/)
{
	return SortChildren(pSort->hParent,nCol,TRUE,pSort->lpfnCompare,pSort->lParam);
}

HTREEITEM COXTreeCtrl::GetItemFromIndex(int nIndex) const
{
	ASSERT(nIndex>=-1);
	if(nIndex==-1)
	{
		return GetRootItem();
	}

	return (HTREEITEM)((CListCtrl*)this)->GetItemData(nIndex);
}


LRESULT COXTreeCtrl::OnNeedItemTip(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
#ifndef OX_TREECTRL_NOITEMTIPS
	LPNEEDITEMTIPINFO pnitInfo=(LPNEEDITEMTIPINFO)lParam;
	int nResult=pnitInfo->result;

	if(nResult!=ID_NIT_WRONGFORMAT && nResult!=ID_NIT_OUTOFCONTROLBORDER)
	{
		CPoint point;
		point.x=pnitInfo->point.x;
		point.y=pnitInfo->point.y;

		UINT nFl;
		int idx=HitTest(point,&nFl);

		HTREEITEM hti=GetItemFromIndex(idx);
		ASSERT(hti);
		COXTreeItem *xti=GetXItem(hti);
		ASSERT(xti);

		if(idx==-1)
		{
			pnitInfo->result=ID_NIT_OUTOFCONTROLBORDER;
			return 0;
		}
		pnitInfo->row=idx;

		CRect rcItem;
		CListCtrl::GetItemRect(idx,&rcItem,LVIR_BOUNDS);

		// Now find the column
		int nCol=0;
		while(TRUE)
		{
			int nColWidth=GetColumnWidth(nCol);
			if(nColWidth<0)
			{
				pnitInfo->result=ID_NIT_OUTOFCONTROLBORDER;
				return 0;
			}

			rcItem.right=rcItem.left+nColWidth;
			if(rcItem.PtInRect(point))
			{
				break;
			}
			rcItem.left=rcItem.right;
			nCol++;
		}
		if(nCol && !xti->GetSubItem(nCol))
		{
			// this item has no subitems at given column
			pnitInfo->result=ID_NIT_CUSTOMREJECT;
			return 0;
		}
		pnitInfo->col=nCol;

		CImageList *pImgList=CListCtrl::GetImageList(LVSIL_SMALL);
		CImageList *pStateImgList=CListCtrl::GetImageList(LVSIL_STATE);

		LV_COLUMN lvc;
		memset(&lvc, 0, sizeof(LV_COLUMN));
		lvc.mask=LVCF_FMT;
		VERIFY(GetColumn(nCol, &lvc));

		int nImgWidth=0;
		IMAGEINFO imgInfo;
		int nImg=xti->GetItemImage(nCol);

		if(pImgList && nImg!=-1)
		{
			pImgList->GetImageInfo(0,&imgInfo);
			CRect rcImg(imgInfo.rcImage);
			nImgWidth += rcImg.Width();
		}

		if(pStateImgList && nCol==0 && 
			GetItemStateIndex(GetItemIndexInternal(xti))!=-1)
		{
			pStateImgList->GetImageInfo(0,&imgInfo);
			CRect rcImg(imgInfo.rcImage);
			nImgWidth += rcImg.Width();
		}

		rcItem.left+=nImgWidth;
		rcItem.left+=nCol==0 ? GetItemIndent(xti) : 0;
		if(!rcItem.PtInRect(point))
		{
			pnitInfo->result=ID_NIT_OUTOFITEMBORDER;
			return 0;
		}
	
		pnitInfo->clrText=xti->IsDisabled() ? ::GetSysColor(COLOR_3DSHADOW) : 
			xti->HasColor(nCol) ? xti->GetItemColor(nCol) : GetTextColor();
//		pnitInfo->clrBackground=GetTextBkColor();
		pnitInfo->clrBackground=ID_OX_COLOR_NONE;

		CString sText=GetItemText((HTREEITEM)xti,nCol);
		if(sText.IsEmpty())
		{
			pnitInfo->result=ID_NIT_NOTHIDDEN;
			return 0;
		}
		lstrcpyn(pnitInfo->itemText,sText,pnitInfo->sizeText);
		pnitInfo->pFont=(LPARAM)GetFont();
		if(xti->HasFont(nCol))
		{
			pnitInfo->pFont=(LPARAM)xti->GetItemFont(nCol);
		}

		int nOffset=2;
		rcItem.DeflateRect(nOffset,0);

		CClientDC dc(this);
		CFont* pOldFont=pnitInfo->pFont==NULL ? NULL : 
			dc.SelectObject((CFont*)pnitInfo->pFont);
		CRect rectText(0, 0, 0, 0);
		dc.DrawText(sText,&rectText,DT_CALCRECT|DT_LEFT|DT_SINGLELINE);
		if(pOldFont)
		{
			dc.SelectObject(pOldFont);
		}

		CRect rectClient;
		GetClientRect(&rectClient);

		int fmt=lvc.fmt&LVCFMT_JUSTIFYMASK;
		int nTextWidth=rectText.Width();
		switch(fmt)
		{
		case LVCFMT_LEFT:
			{
				rectText.left=rcItem.left;
				rectText.right=rectText.left+nTextWidth;
				pnitInfo->alignment=DT_LEFT;
				break;
			}
		case LVCFMT_RIGHT:
			{
				rectText.right=rcItem.right;
				rectText.left=rectText.right-nTextWidth;
				pnitInfo->alignment=DT_RIGHT;
				break;
			}
		case LVCFMT_CENTER:
			{
				int nCenter=rcItem.left+(rcItem.right-rcItem.left)/2;
				rectText.left=nCenter-nTextWidth/2;
				rectText.right=nCenter+nTextWidth/2;
				rectText.right+=nTextWidth%2;
				pnitInfo->alignment=DT_CENTER;
				break;
			}
		default:
			ASSERT(FALSE);
		}

		if(rectText.Width()<=rcItem.Width() &&
			rectText.right<=rectClient.right && rectText.left>=rectClient.left)
		{
			pnitInfo->result=ID_NIT_NOTHIDDEN;
			return 0;
		}

		if(rcItem.Width()<rectText.Width())
		{
			rcItem.right=rcItem.left+rectText.Width();
		}
		rcItem.InflateRect(nOffset,0);

		pnitInfo->rectItem.left=rcItem.left;
		pnitInfo->rectItem.right=rcItem.right;
		pnitInfo->rectItem.top=rcItem.top;
		pnitInfo->rectItem.bottom=rcItem.bottom;

		pnitInfo->offset=nOffset;

		pnitInfo->result=ID_NIT_SUCCESS;
	}
#else
	UNREFERENCED_PARAMETER(lParam);
#endif
	return 0;
}

void COXTreeCtrl::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==m_nTimerCheckKeyboardInput)
	{
		m_sSearchMask=_T("");
		return;
	}

	CListCtrl::OnTimer(nIDEvent);
}

void COXTreeCtrl::OnDestroy()
{
	if(m_nTimerCheckKeyboardInput!=NULL)
		KillTimer(m_nTimerCheckKeyboardInput);

	CListCtrl::OnDestroy();
}

void COXTreeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);

	if (m_dwExStyle & TVOXS_EXTENDCOLUMNS)
		PostMessage(WM_USER_ADJUSTLASTCOLUMN);
}

LRESULT COXTreeCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
#ifndef OX_TREECTRL_NOITEMTIPS
	if(m_bFirstTimeHooking && (m_dwExStyle&TVOXS_ITEMTIPS)==TVOXS_ITEMTIPS && 
		m_ItemTip.GetHookedWnd()!=this)
	{
		m_bFirstTimeHooking=FALSE;
		m_ItemTip.Attach(this);
	}
#endif
	return CListCtrl::DefWindowProc(message, wParam, lParam);
}

BOOL COXTreeCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
						 UINT nID, DWORD dwExStyle/*=0*/)
{
	DWORD dwWndStyle=dwStyle&0xffff0000;
	m_dwTCStyle=dwStyle&0x0000ffff;
	m_dwExStyle=dwExStyle;
	DWORD dwListStyle=LVS_REPORT|LVS_OWNERDRAWFIXED;

	if(m_dwTCStyle & TVS_SHOWSELALWAYS)
		dwListStyle |= LVS_SHOWSELALWAYS;
	if(m_dwTCStyle & TVS_EDITLABELS)
		dwListStyle |= LVS_EDITLABELS;

	if(!(m_dwExStyle & TVOXS_MULTISEL))
		dwListStyle |= LVS_SINGLESEL;
	if(!(m_dwExStyle & TVOXS_COLUMNHDR))
		dwListStyle |= LVS_NOCOLUMNHEADER;
	if(m_dwExStyle & TVOXS_NOSCROLL)
	{
		dwListStyle |= LVS_NOSCROLL;
	}
	else
		dwWndStyle |= WS_VSCROLL | WS_HSCROLL;
	if(m_dwExStyle & TVOXS_NOSORTHEADER)
		dwListStyle |= LVS_NOSORTHEADER;
	if(m_dwExStyle & TVOXS_SHAREIMAGELISTS)
		dwListStyle |= LVS_SHAREIMAGELISTS;

	if(!CListCtrl::Create(dwListStyle|dwWndStyle,rect,pParentWnd,nID))
	{
		return FALSE;
	}
	DWORD dwAddStyle=::GetWindowLongPtr(m_hWnd,GWL_STYLE);	
	m_dwTCStyle|=(0xFFFF0000&dwAddStyle);
	
	return Init();
}

BOOL COXTreeCtrl::IsHeaderSorting() const
{
	return ((m_dwExStyle&TVOXS_NOSORTHEADER)!=TVOXS_NOSORTHEADER && 
		(m_dwExStyle&TVOXS_COLUMNHDR)==TVOXS_COLUMNHDR);
}

int COXTreeCtrl::GetSortCol() const
{
	COXTreeHeader* pHeader=(COXTreeHeader*)GetDlgItem(0);
	ASSERT(pHeader);
	return pHeader->GetSortCol();
}

int COXTreeCtrl::GetSortOrder() const
{
	COXTreeHeader* pHeader=(COXTreeHeader*)GetDlgItem(0);
	ASSERT(pHeader);
	return pHeader->GetSortOrder();
}

void COXTreeCtrl::OnParentNotify(UINT message, LPARAM lParam)
{
	CListCtrl::OnParentNotify(message,lParam);

	if(LOWORD(message) == WM_CREATE)
	{
		HWND hWnd=GetHeaderCtrlHandle();
		if(hWnd==(HWND)lParam)
		{
			if(!::IsWindow(m_wndHdr.GetSafeHwnd()))
				VERIFY(m_wndHdr.SubclassWindow(hWnd));
			else
				ASSERT(m_wndHdr.GetSafeHwnd()==hWnd);
		}

	}
}

HWND COXTreeCtrl::GetHeaderCtrlHandle()
{
	ASSERT_VALID(this);
	if (m_hWnd == NULL)
		// ... This control has not been created yet
		return NULL;

	// Get the first child of the list control
	// Normally the list only has one child window : the header control
	HWND hHeaderWnd=::GetDlgItem(m_hWnd, 0);
	if(hHeaderWnd != NULL)
	{
		// Make extra sure we actually have a header ctrl
		const int nMaxClassNameLength=50;
		TCHAR szClass[nMaxClassNameLength + 1];
		::GetClassName(hHeaderWnd, szClass, nMaxClassNameLength);
		if(_tcscmp(szClass, _T("SysHeader32")) == 0) 
		{
			return hHeaderWnd;
		}
		else 
		{
			return NULL;
		}

	}
	else
	{
		TRACE(_T("COXTreeCtrl::GetHeaderCtrlHandle : No child window found\n"));
		return NULL;
	}
}

BOOL COXTreeCtrl::CopyItem(HTREEITEM hItemToCopy, HTREEITEM hParent/*=NULL*/, 
						   HTREEITEM hInsertAfter/*=TVI_LAST*/, 
						   BOOL bCopyDescendants/*=TRUE*/,
						   HTREEITEM* pNewItem/*=NULL*/,
						   COXTreeCtrl* /*pDestinationTree=NULL*/)
{
	ASSERT(hItemToCopy!=NULL);

	// define if we can copy an item: we cannot copy an item with its descendants 
	// to itself or its descendants
	BOOL bCanCopy=TRUE;
	if(bCopyDescendants)
	{
		HTREEITEM hTestItem=hParent;
		while(hTestItem!=NULL)
		{
			if(hTestItem==hItemToCopy)
			{
				bCanCopy=FALSE;
				break;
			}
			hTestItem=GetParentItem(hTestItem);
		}
	}

	if(!bCanCopy)
		return FALSE;

	COXTreeItem* pxParent=GetXItem(hParent);
	ASSERT(pxParent!=NULL);

	COXTreeItem* pxItemToCopy=GetXItem(hItemToCopy);
	ASSERT(pxItemToCopy!=NULL);

	COXTreeItem* pxInsertAfter=NULL;
	if(hInsertAfter==TVI_FIRST || hInsertAfter==TVI_LAST)
		pxInsertAfter=(COXTreeItem*)hInsertAfter;
	else
		pxInsertAfter=GetXItem(hInsertAfter);

	COXTreeItem* pTempNewItem=pxParent->CopyChild(pxItemToCopy,pxInsertAfter,bCopyDescendants);

	if (pNewItem)
		*pNewItem=(HTREEITEM) pTempNewItem;

	BOOL bWasHidden=IsItemHidden(hParent);
	if(!bWasHidden)
	{
		// lock the control updates
		SetRedraw(FALSE);

		// hiding of item will remove it from list control while 
		// saving internal logic
		HideItem(hParent,TRUE);
				
		// restore the visibility of hidden item
		HideItem(hParent,FALSE);

		// unlock the control updates
		SetRedraw(TRUE);
	}
				
	return TRUE;
}

BOOL COXTreeCtrl::MoveItem(HTREEITEM hItemToMove, HTREEITEM hParent/*=NULL*/, 
						   HTREEITEM hInsertAfter/*=TVI_LAST*/,
						   COXTreeCtrl* pDestinationTree/*=NULL*/)
{
	if(hItemToMove==hInsertAfter)
		return TRUE;

	// define if we can move item: we cannot move dragged item to its
	// descendants
	BOOL bCanMove=TRUE;
	HTREEITEM hTestItem=hParent;
	while(hTestItem!=NULL)
	{
		if(hTestItem==hItemToMove)
		{
			bCanMove=FALSE;
			break;
		}
		hTestItem=GetParentItem(hTestItem);
	}

	if(!bCanMove)
		return FALSE;


	COXTreeItem* pxParent;
	if (pDestinationTree != NULL)
		pxParent = pDestinationTree->GetXItem(hParent);
	else	
		pxParent = GetXItem(hParent);
	ASSERT(pxParent!=NULL);

	COXTreeItem* pxItemToMove=GetXItem(hItemToMove);
	ASSERT(pxItemToMove!=NULL);

	COXTreeItem* pxInsertAfter=NULL;
	if(hInsertAfter==TVI_FIRST || hInsertAfter==TVI_LAST)
		pxInsertAfter=(COXTreeItem*)hInsertAfter;
	else
		pxInsertAfter=GetXItem(hInsertAfter);


	// lock the control updates
	SetRedraw(FALSE);
	if (pDestinationTree != NULL)
		pDestinationTree->SetRedraw(FALSE);

	// hiding of item will remove it from list control while 
	// saving internal logic
	HideItem(hItemToMove,TRUE);
				
	// remove the moved item and its descendants out of internal chain 
	pxItemToMove->Unlink();
	// insert moved item and its descendants
	pxParent->AddChild(pxItemToMove,pxInsertAfter);


	// show the reallocated item
	if (pDestinationTree != NULL)
		pDestinationTree->HideItem(hItemToMove,FALSE);
	else
		HideItem(hItemToMove,FALSE);

	// unlock the control updates
	SetRedraw(TRUE);
	if (pDestinationTree != NULL)
		pDestinationTree->SetRedraw(TRUE);

	return TRUE;
}


BOOL COXTreeCtrl::RedrawTreeItems(HTREEITEM hItemFirst, HTREEITEM hItemLast)
{
	if(hItemFirst==NULL || GetItemIndex(hItemFirst)==-1 || 
		hItemLast==NULL || GetItemIndex(hItemLast)==-1)
		return FALSE;

	CListCtrl::RedrawItems(GetItemIndex(hItemFirst),GetItemIndex(hItemLast));
	return TRUE;
}


void COXTreeCtrl::OnStyleChanged( int nStyleType, LPSTYLESTRUCT lpStyleStruct )
{
	if (nStyleType==GWL_STYLE)
		m_dwTCStyle=(lpStyleStruct->styleNew & 0xFFFF0000) | (m_dwTCStyle & 0x0000FFFF);
}

BOOL COXTreeCtrl::PreTranslateMessage( MSG* pMsg )
{
	return CListCtrl::PreTranslateMessage(pMsg);
}

void COXTreeCtrl::SetHorizontalGridColor(COLORREF clr)
{
	m_clrHorizontalGrid = clr;
}

void COXTreeCtrl::SetVerticalGridColor(COLORREF clr)
{
	m_clrVerticalGrid = clr;
}

BOOL COXTreeCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (m_dwExStyle & TVOXS_EXTENDCOLUMNS)
	{
		HD_NOTIFY *pHDN = (HD_NOTIFY*) lParam;

		if ((pHDN->hdr.code == HDN_BEGINTRACKA || pHDN->hdr.code == HDN_BEGINTRACKW) &&
			pHDN->iItem == GetColumnsCount() - 1)
		{
			*pResult = TRUE;	// disable tracking
			return TRUE;  
		}

		else if (pHDN->hdr.code == HDN_ITEMCHANGINGA || pHDN->hdr.code == HDN_ITEMCHANGINGW ||
			pHDN->hdr.code == HDN_TRACKA || pHDN->hdr.code == HDN_TRACKW)
		{
			// Adjust the last column
			PostMessage(WM_USER_ADJUSTLASTCOLUMN);
		}
	}

	return CListCtrl::OnNotify(wParam, lParam, pResult);
}

LRESULT COXTreeCtrl::OnUserAdjustLastColumn(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (!m_bInit)
		return 0;

	// Make the last column ends where the window ends
	CRect rectClient;
	GetClientRect(rectClient);

	int iCount = GetColumnsCount();//m_wndHdr.GetItemCount();
	if (iCount <= 0)
		return 0;

	// Loop through all columns and make sure none is larger than the window
	int iAccumulatedWidth = 0;
	for (int i = 0; i < iCount; i++)
	{
		int iCurrentWidth = GetColumnWidth(i);

		if (iCurrentWidth + iAccumulatedWidth > rectClient.Width())
		{
			// The current column is too wide
			iCurrentWidth = rectClient.Width() - iAccumulatedWidth;
			if (iCurrentWidth < 0)
				iCurrentWidth = 0;
			SetColumnWidth(i, iCurrentWidth);
		}
		else if (i == iCount - 1 && iCurrentWidth + iAccumulatedWidth < rectClient.Width())
		{
			// This is the last column and it is not wide enough
			iCurrentWidth = rectClient.Width() - iAccumulatedWidth;
			SetColumnWidth(i, iCurrentWidth);
		}
		
		iAccumulatedWidth += iCurrentWidth;
	}

	// Send the the message to the parent
	CWnd* pParent = GetParent();
	if (pParent!= NULL)
		pParent->SendMessage(WM_USER_ADJUSTLASTCOLUMN, 0, 0);

	return 0;
}

void COXTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_CONTROL || nChar == VK_MENU)
		return;

	// Navigation support
	if (nChar == VK_UP ||
		nChar == VK_DOWN ||
		nChar == VK_HOME ||
		nChar == VK_END ||
		nChar == VK_PRIOR ||
		nChar == VK_NEXT)
	{
		CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
	else if (nChar == VK_RIGHT)
	{
		// Try to expand the selected item
		HTREEITEM hSelectedItem = GetSelectedItem();
		if (hSelectedItem != NULL)
		{
			COXTreeItem* pItem = GetXItem(hSelectedItem);
			if (!pItem->IsExpanded())
				Expand(hSelectedItem, TVE_EXPAND);
			else if (pItem->pxFirstChild != NULL)
			{
				// Already expanded, so select the first child
				SelectItem(NULL);
				SelectItem((HTREEITEM) pItem->pxFirstChild);
			}
		}
	}
	else if (nChar == VK_LEFT)
	{
		// Try to collapse the selected item
		HTREEITEM hSelectedItem = GetSelectedItem();
		if (hSelectedItem != NULL)
		{
			COXTreeItem* pItem = GetXItem(hSelectedItem);
			if (pItem->IsExpanded())
				Expand(hSelectedItem, TVE_COLLAPSE);
			else if (pItem->pxParent != NULL)
			{
				// Already collapsed, so select parent
				SelectItem(NULL);
				SelectItem((HTREEITEM) pItem->pxParent);
			}
		}
	}

	int nCount=GetVisibleCount();
	if(nCount>1)
	{
 		if(m_nTimerCheckKeyboardInput!=NULL)
 			KillTimer(m_nTimerCheckKeyboardInput);

 		TCHAR chSymbol[2];
 		chSymbol[0]=(TCHAR)nChar;
 		chSymbol[1]=0;
 		CString sNewSymbol=chSymbol;
 		m_sSearchMask+=sNewSymbol;
 		CString sMask=m_sSearchMask;
 		sMask.MakeUpper();

 		int nStartIndex=CListCtrl::GetNextItem(-1,LVNI_ALL|LVNI_FOCUSED);

 		HTREEITEM hti;
 		CString sText;
 		int nIndex=nStartIndex;
 		BOOL bIncludeCurrent=FALSE;
 		if(nStartIndex!=-1 && sMask.GetLength()>1)
 		{
 			nIndex--;
 			bIncludeCurrent=TRUE;
 		}
 		while(TRUE)
 		{
 			nIndex++;
 			if(nIndex>=nCount)
 			{
 				nIndex=0;
 				if(nStartIndex==-1)
 				{
 					break;
 				}
 			}
 			if(nIndex==nStartIndex)
 			{
 				if(!bIncludeCurrent)
 					break;
 				else
 					bIncludeCurrent=FALSE;
 			}

 			hti=GetItemFromIndex(nIndex);
 			ASSERT(hti);
 			sText=GetItemText(hti);
 			sText.MakeUpper();
 			if(!sText.IsEmpty() && sText.Find(sMask)==0)
 			{
 				SelectItem(NULL);
 				SelectItem(hti);
 				EnsureVisible(hti);
 				break;
 			}
 		}

 		// timer for keyboard input
 		m_nTimerCheckKeyboardInput= (UINT)SetTimer(IDT_OXTREECTRL_CHECKFORKEYBOARDINPUT,
 			ID_OXTREECTRL_CHECKFORKEYBOARDINPUT_DELAY,NULL);
 		if(m_nTimerCheckKeyboardInput==0)
 		{
 			TRACE(_T("COXTreeCtrl::OnKeydown: Unable to set timer for keyboard input\n"));
 		}
	}
}

BOOL COXTreeCtrl::IsPropertiesWnd() const
{
	if (GetParent() != NULL && GetParent()->SendMessage(WM_USER_QUERY_PROPERTIESWND))
		return TRUE;
	else
		return FALSE;
}

void COXTreeCtrl::SetColumnCompareFunction(int nCol, PFNTVCOMPARE lpfnDefaultCompare)
{
	m_mapCompareFunctions.SetAt(nCol, lpfnDefaultCompare);
}
