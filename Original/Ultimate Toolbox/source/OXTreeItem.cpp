// ==========================================================================
//						COXTreeSubItem & COXTreeItem
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////

                         
#include "stdafx.h"
#include "OXTreeItem.h"
#include "OXTreeCtrl.h"

#include <malloc.h>

#include "UTBStrOp.h"
#include "UTB64Bit.h"


COXTreeItem::COXTreeItem()
{
	m_uEditMode=OXET_EDIT;
	pxNext=pxPrev=pxFirstChild=pxParent=NULL;
	m_bExpand=FALSE;
	m_bExpandedOnce=FALSE;
	dwStateEx=XTIS_VISIBLE;
	memset(&m_tvi,0,sizeof(m_tvi));
	m_tvi.mask = TVIF_HANDLE;
	m_tvi.hItem = (HTREEITEM) this;
	m_tvi.pszText=NULL;
	m_nEllipsisFormat=DT_END_ELLIPSIS;
	m_clr=::GetSysColor(COLOR_WINDOWTEXT);
	m_clrBackground=::GetSysColor(COLOR_WINDOW);
}


COXTreeItem::~COXTreeItem()
{
	Unlink();
	DeleteChildren();
	for(int i=0;i < m_Items.GetSize();i++)
		delete m_Items[i];
	if(m_tvi.pszText && m_tvi.pszText != LPSTR_TEXTCALLBACK)
		delete [] m_tvi.pszText;
}	


// Copy constructor
COXTreeItem& COXTreeItem::operator=( const COXTreeItem& xti )
{
	if(this==&xti)
		return *this;

	if((HFONT)m_font)
		m_font.DeleteObject();
	if((HFONT)xti.m_font)
	{
		LOGFONT lf;
		COXTreeItem* xtiCopy=(COXTreeItem*)&xti;
		VERIFY(xtiCopy->m_font.GetLogFont(&lf));
		VERIFY(m_font.CreateFontIndirect(&lf));
	}

	m_tvi.mask=xti.m_tvi.mask;
	m_tvi.hItem=(HTREEITEM)this;
	m_tvi.state=xti.m_tvi.state;
	m_tvi.stateMask=xti.m_tvi.stateMask;
	if(xti.m_tvi.pszText==LPSTR_TEXTCALLBACK)
	{
		if(m_tvi.pszText && m_tvi.pszText!=LPSTR_TEXTCALLBACK)
			delete[] m_tvi.pszText;
		m_tvi.pszText=LPSTR_TEXTCALLBACK;
	}
	else
	{
		// allocate space for new item text and copy string
		size_t len = _tcslen(xti.m_tvi.pszText)+1;
		TCHAR* pChar=new TCHAR[len];
		UTBStr::tcscpy(pChar, len, xti.m_tvi.pszText);
		if(m_tvi.pszText && m_tvi.pszText!=LPSTR_TEXTCALLBACK)
			delete[] m_tvi.pszText;
		m_tvi.pszText=pChar;
	}
	m_tvi.cchTextMax=xti.m_tvi.cchTextMax;
	m_tvi.iImage=xti.m_tvi.iImage;
	m_tvi.iSelectedImage=xti.m_tvi.iSelectedImage;
	m_tvi.cChildren=xti.m_tvi.cChildren;
	m_tvi.lParam=xti.m_tvi.lParam;

	m_clr=xti.m_clr;
	m_clrBackground = xti.m_clrBackground;
	m_nEllipsisFormat=xti.m_nEllipsisFormat;

	dwStateEx=xti.dwStateEx;
	m_bExpand=xti.m_bExpand;
	m_bExpandedOnce=xti.m_bExpandedOnce;
	pxParent=xti.pxParent;
	pxNext=xti.pxNext;
	pxPrev=xti.pxPrev;
	pxFirstChild=xti.pxFirstChild;
	
	m_saTextEx.RemoveAll();
	m_saTextEx.Append(xti.m_saTextEx);
	m_uEditMode=xti.m_uEditMode;

	int nIndex=0;
	for(nIndex=0; nIndex<m_Items.GetSize(); nIndex++)
	{
		COXTreeSubItem* pSubItem=m_Items.GetAt(nIndex);
		ASSERT(pSubItem!=NULL);
		delete pSubItem;
	}
	m_Items.RemoveAll();

	for(nIndex=0; nIndex<xti.m_Items.GetSize(); nIndex++)
	{
		COXTreeSubItem* pSubItem=xti.m_Items.GetAt(nIndex);
		ASSERT(pSubItem!=NULL);
		COXTreeSubItem* pNewSubItem=new COXTreeSubItem;
		ASSERT(pNewSubItem!=NULL);
		*pNewSubItem=*pSubItem;
		m_Items.Add(pNewSubItem);
	}

	return *this;
}

// Copy constructor
COXTreeSubItem& COXTreeSubItem::operator=( const COXTreeSubItem& xtsi )
{
	if(this==&xtsi)
		return *this;
		
	sItem=xtsi.sItem;
	nCol=xtsi.nCol;
	uFlags=xtsi.uFlags;
	nImage=xtsi.nImage;
	m_nEllipsisFormat=xtsi.m_nEllipsisFormat;
	m_uEditMode=xtsi.m_uEditMode;
	m_clr=xtsi.m_clr;
	m_clrBackground = xtsi.m_clrBackground;

	if((HFONT)m_font)
		m_font.DeleteObject();
	if((HFONT)xtsi.m_font)
	{
		LOGFONT lf;
		COXTreeSubItem* xtsiCopy=(COXTreeSubItem*)&xtsi;
		VERIFY(xtsiCopy->m_font.GetLogFont(&lf));
		VERIFY(m_font.CreateFontIndirect(&lf));
	}

	m_saTextEx.RemoveAll();
	m_saTextEx.Append(xtsi.m_saTextEx);

	return *this;
}

void COXTreeItem::DeleteChildren()
{
	COXTreeItem *xti = pxFirstChild;
	while(xti)
	{
		xti->Unlink();
		delete xti;
		xti = pxFirstChild;
	}
}	

BOOL COXTreeItem::DeleteSubitem(int nCol)
{
	if(nCol<=0 || nCol>=GetSubitemsCount())
		return FALSE;

	ASSERT(m_Items[nCol-1]!=NULL);
	delete m_Items[nCol-1];
	for(int nIndex=nCol; nIndex<GetSubitemsCount(); nIndex++)
	{
		ASSERT(m_Items[nIndex]!=NULL);
		m_Items[nIndex]->nCol--;
		m_Items[nIndex-1]=m_Items[nIndex];
	}
	m_Items.RemoveAt(GetSubitemsCount()-1);

	return TRUE;
}

COXTreeItem* COXTreeItem::GetLastChild()
{
	COXTreeItem *xti=pxFirstChild;
	if(!xti)
		return NULL;
	while(xti->pxNext)
		xti = xti->pxNext;
	return xti;
}

void COXTreeItem::Unlink()
{
	if(pxParent && pxParent->pxFirstChild == this)
	{
		pxParent->pxFirstChild = pxNext;
	}
	if(pxPrev)
	{
		pxPrev->pxNext = pxNext;
	}
	if(pxNext)
	{
		pxNext->pxPrev = pxPrev;
	}
	pxParent = pxPrev= pxNext=NULL;
}

void COXTreeItem::AddChild(COXTreeItem *pNewChild, COXTreeItem *pInsAfter)
	{
	ASSERT(!pNewChild->pxParent);
	ASSERT(!pNewChild->pxNext);
	ASSERT(!pNewChild->pxPrev);
	pNewChild->pxParent=this;
	if(!pxFirstChild)
		{
		pxFirstChild = pNewChild;
		pNewChild->pxPrev = pNewChild->pxNext = NULL;
		return;
		}

	if(pInsAfter && pInsAfter!=(COXTreeItem*)TVI_LAST)
		{
		if(pInsAfter == (COXTreeItem*) TVI_FIRST)
			{
			pNewChild->pxPrev = NULL;
			pNewChild->pxNext = pxFirstChild;
			if(pxFirstChild)
				pxFirstChild->pxPrev = pNewChild;
			pxFirstChild = pNewChild;
			}
		else
			{
			ASSERT(pInsAfter->pxParent==this);
			pNewChild->pxNext=pInsAfter->pxNext;
			pInsAfter->pxNext=pNewChild;
			pNewChild->pxPrev=pInsAfter;
			if(pNewChild->pxNext!=NULL)
				pNewChild->pxNext->pxPrev=pNewChild;
			}
		}
	else
		{
		pNewChild->pxPrev=GetLastChild();
		pNewChild->pxNext = NULL;
		GetLastChild()->pxNext = pNewChild;
		}
	}


int COXTreeItem::GetChildOffset(COXTreeItem *pChild,BOOL bCalcHidden)
	{
	UNREFERENCED_PARAMETER(pChild);

	COXTreeItem * pxti = pxFirstChild;
	int cnt=0;
	while(pxti)
		{
		if(!pxti->IsHidden() || bCalcHidden)
			cnt++;
		pxti = pxti->pxNext;
		}
	ASSERT(cnt);
	return cnt;
	}

COXTreeItem::COXTreeItem(LPCTSTR lpszItem)
{
	m_uEditMode=OXET_EDIT;
	pxNext=pxPrev=pxFirstChild=pxParent=NULL;
	m_bExpand=FALSE;
	m_bExpandedOnce=FALSE;
	dwStateEx=XTIS_VISIBLE;
	memset(&m_tvi,0,sizeof(m_tvi));
	if(lpszItem == LPSTR_TEXTCALLBACK)
		m_tvi.pszText = (LPTSTR) lpszItem;
	else
	{
		size_t len = _tcslen(lpszItem)+1;
		m_tvi.pszText = new TCHAR[len];
		UTBStr::tcscpy(m_tvi.pszText, len, lpszItem);
	}
	m_tvi.mask = TVIF_HANDLE | TVIF_TEXT;
	m_tvi.hItem = (HTREEITEM) this;
	m_tvi.iImage=-1;
	m_tvi.iSelectedImage=-1;
	m_nEllipsisFormat=DT_END_ELLIPSIS;
	m_clr=::GetSysColor(COLOR_WINDOWTEXT);
	m_clrBackground=::GetSysColor(COLOR_WINDOW);
}

BOOL COXTreeItem::IsHidden() const
{
	return !(dwStateEx & XTIS_VISIBLE);
}

void COXTreeItem::RemoveChildrenFromCtrl(COXTreeCtrl *pCtrl)
{
//	TRACE(_T("RemoveChildrenFromCtrl\n"));

	COXTreeItem *xti=pxFirstChild;
	while(xti)
	{
		int idx = pCtrl->GetItemIndexInternal(xti);
		if(idx != -1)
		{
			xti->RemoveChildrenFromCtrl(pCtrl);
			pCtrl->CListCtrl::DeleteItem(idx);
		}
		xti = xti->pxNext;
	}
}

int COXTreeItem::AddChildrenToCtrl(COXTreeCtrl *pCtrl,int startPos)
{
//	TRACE(_T("AddChildrenToCtrl\n"));

	int pos = startPos;
	if(IsHidden() || !IsExpanded())
		return pos;
	COXTreeItem *xti = pxFirstChild;
	while(xti)
	{
		if(!xti->IsHidden())
		{
			pCtrl->SetItemAtPos(pos,xti);
			pos ++;
			pos = xti->AddChildrenToCtrl(pCtrl,pos);
		}
		xti = xti->pxNext;
	}
	return pos;
}

int COXTreeItem::GetSubitemsCount() const
{
	return PtrToInt(m_Items.GetSize());
}

BOOL COXTreeItem::Expand(UINT nCode,COXTreeCtrl *pCtrl)
{

	ASSERT(pCtrl!=NULL);

	BOOL bWasVisible=IsVisible();

	switch(nCode)
	{
		case TVE_TOGGLE:
			m_bExpand = !m_bExpand;
			break;

		case TVE_COLLAPSE:
			if(!m_bExpand)
			{
				TRACE(_T("COXTreeItem::Expand: the item is already in collapsed state!\n"));
				return TRUE;
			}
			m_bExpand = FALSE;
			break;

		case TVE_EXPAND:
			if(m_bExpand)
			{
				TRACE(_T("COXTreeItem::Expand: the item is already in expanded state!\n"));
				return TRUE;
			}
			m_bExpand = TRUE;
			break;
		
		case TVE_COLLAPSERESET:
			pCtrl->DeleteChildrenItems(this);
			m_bExpand=FALSE;
			m_bExpandedOnce=FALSE;
			m_tvi.cChildren=0;
			return TRUE;
		
		default:
			TRACE(_T("COXTreeItem::Expand: unexpected case found!\n"));
			return FALSE;
	}
	
	if(m_bExpand)
		m_bExpandedOnce=TRUE;

	if(!bWasVisible && !m_bExpand)
		return TRUE;

	if(!IsVisible() && m_bExpand)
	{
		COXTreeItem* xtiParent=pxParent;
		ASSERT(xtiParent!=NULL && xtiParent!=&pCtrl->m_xtiRoot);
		xtiParent->Expand(TVE_EXPAND,pCtrl);
	}
	else
		Exp(pCtrl);
	
	return TRUE;
}

void COXTreeItem::Exp(COXTreeCtrl *pCtrl)
{
	COXTreeItem *xti = pxFirstChild;
	int pos = pCtrl->GetItemIndexInternal(this) + 1;
	ASSERT(pos);
	if(m_bExpand)
	{
		while(xti)
		{
			if(!xti->IsHidden())
			{
				pCtrl->SetItemAtPos(pos,xti);
				pos ++;
				pos = xti->AddChildrenToCtrl(pCtrl,pos);
			}
			xti=xti->pxNext;
		}
	}
	else
	{
		while(xti)
		{
			int idx = pCtrl->GetItemIndexInternal(xti);
			if(idx != -1)
			{
				xti->RemoveChildrenFromCtrl(pCtrl);
				pCtrl->CListCtrl::DeleteItem(idx);
			}
			xti = xti->pxNext;
		}
	}
}

int COXTreeItem::GetItemLevel() const
{
	int nLevel = 0;
	COXTreeItem *xti = pxParent;
	while(xti)
	{
		nLevel++;
		xti = xti->pxParent;
	}
	return nLevel;
}

BOOL COXTreeItem::IsLastUnhidden() const
{
	COXTreeItem *xti = pxNext;
	while(xti)
	{
		if(!xti->IsHidden())
			return FALSE;
		xti=xti->pxNext;
	}
	return TRUE;
}

BOOL COXTreeItem::ItemHasChildren() const
{
	return pxFirstChild != NULL;
}

COXTreeSubItem* COXTreeItem::GetSubItem(int nCol) const
{
	ASSERT(nCol != -1 && nCol != 0);
	for(int i=0;i < m_Items.GetSize();i++)
	{
		if(m_Items[i]->nCol == nCol)
			return m_Items[i];
	}
	return NULL;
}

int COXTreeItem::GetItemImage(int nCol,BOOL bSelImg) const
{
	if(nCol == 0)
		return bSelImg ? m_tvi.iImage:m_tvi.iSelectedImage;
	
	for(int i=0;i < m_Items.GetSize();i++)
	{
		if(m_Items[i]->nCol == nCol)
		{
			if(m_Items[i]->HasImage())
				return m_Items[i]->nImage;
			else
				break;
		}
	}
	return -1;
}


BOOL COXTreeSubItem::SetFont(CFont *pFont)
{
	if(pFont)
	{
		if((HFONT)m_font!=NULL)
		{
			m_font.DeleteObject();
		}
		LOGFONT lf;
		BOOL bRet = pFont->GetLogFont(&lf);
		if(bRet)
		{
			bRet=m_font.CreateFontIndirect(&lf);
		}
		if(bRet)
			uFlags |= OX_SUBITEM_FONT;
		else
			uFlags &= ~OX_SUBITEM_FONT;
		return bRet;
	}
	else
	{
		uFlags &= ~OX_SUBITEM_FONT;
	}

	return TRUE;
}


BOOL COXTreeItem::SetSubItem(int nCol,UINT uMask,LPCTSTR lpszText,int nImage, 
							 CFont * pFont,COLORREF clr, COLORREF clrBack)
{
	if(nCol == -1 || nCol == 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	COXTreeSubItem *pSubItem = GetSubItem(nCol);
	if(!pSubItem)
	{
		pSubItem = new COXTreeSubItem(nCol);
		m_Items.Add(pSubItem);
	}
	ASSERT(pSubItem);
	if(uMask & OX_SUBITEM_IMAGE)
	{
		pSubItem->nImage = nImage;
		pSubItem->uFlags |= OX_SUBITEM_IMAGE;
	}
	if(uMask & OX_SUBITEM_TEXT)
	{
		pSubItem->sItem = lpszText;
		pSubItem->uFlags |= OX_SUBITEM_TEXT;
	}
	if(uMask & OX_SUBITEM_FONT)
		pSubItem->SetFont(pFont);
	if(uMask & OX_SUBITEM_COLOR)
	{
		pSubItem->m_clr = clr;
		pSubItem->uFlags |= OX_SUBITEM_COLOR;
	}
	if(uMask & OX_SUBITEM_BGCOLOR)
	{
		pSubItem->m_clrBackground = clrBack;
		pSubItem->uFlags |= OX_SUBITEM_BGCOLOR;
	}
	return TRUE;
}

LPCTSTR COXTreeItem::GetSubItemText(int nCol) const
{
	COXTreeSubItem *pSubItem = GetSubItem(nCol);
	if(pSubItem)
		return pSubItem->sItem;
	else
		return _T("");
}

int COXTreeItem::GetSubItemImage(int nCol) const
{
	COXTreeSubItem *pSubItem = GetSubItem(nCol);
	if(pSubItem)
		return pSubItem->HasImage() ? pSubItem->nImage:-1;
	else
		return -1;
}


void COXTreeItem::SetHidden(BOOL bHide)
	{
	if(bHide)
		dwStateEx &= ~XTIS_VISIBLE;
	else
		dwStateEx |= XTIS_VISIBLE;
	}


COXTreeItem* COXTreeItem::GetNextInTree()
{
	//get next item in the tree structure
	if(pxFirstChild)
		return pxFirstChild;
	if(pxNext)
		return pxNext;
	COXTreeItem * xti = pxParent;
	while(xti)
	{
		if(xti->pxNext)
			return xti->pxNext;
		xti = xti->pxParent;
	}
	return NULL;
}

COXTreeItem* COXTreeItem::GetPrevInTree()
{
	//get prev item in the tree structure
	if(pxPrev)
	{
		COXTreeItem* pItemTemp=pxPrev;
		COXTreeItem* pLastChild;
		while((pLastChild=pItemTemp->GetLastChild())!=NULL)
			pItemTemp=pLastChild;
		return pItemTemp;
	}
	// if there is no prev sibling, return parent
	return pxParent;
}

BOOL COXTreeItem::IsVisible() const
{
	// check visible 
	if(IsHidden())
		return FALSE;
	COXTreeItem *xtiParent = pxParent;
	while(xtiParent)
	{
		if(xtiParent->IsHidden() || !xtiParent->IsExpanded())
			return FALSE;
		xtiParent=xtiParent->pxParent;
	}
	// otherwise it is visible
	return TRUE;
}

BOOL COXTreeItem::NeedDrawButton() const
{
	if(m_tvi.cChildren>0)
		return TRUE;

	COXTreeItem *xti = pxFirstChild;
	while(xti)
	{
		if(!xti->IsHidden())
			return TRUE;
		xti = xti->pxNext;
	}
	return FALSE;
}


BOOL COXTreeItem::HasColor(int nCol) const
{
	if(!nCol)
		return (BOOL)(dwStateEx & XTIS_HASCOLOR);
	COXTreeSubItem *pSubItem = GetSubItem(nCol);
	if(!pSubItem)
		return FALSE;
	return pSubItem->HasColor();
}

BOOL COXTreeItem::HasBackColor(int nCol) const
{
	if(!nCol)
		return (BOOL)(dwStateEx & XTIS_HASBGCOLOR);
	COXTreeSubItem *pSubItem = GetSubItem(nCol);
	if(!pSubItem)
		return FALSE;
	return pSubItem->HasBackColor();
}

void COXTreeItem::SetColor(COLORREF clr)
{
	m_clr = clr;
	dwStateEx |= XTIS_HASCOLOR;
}

void COXTreeItem::SetBackColor(COLORREF clr)
{
	m_clrBackground = clr;
	dwStateEx |= XTIS_HASBGCOLOR;
}

void COXTreeSubItem::SetColor(COLORREF clr)
{
	m_clr = clr;
	uFlags |= OX_SUBITEM_COLOR;
}

void COXTreeSubItem::SetBackColor(COLORREF clr)
{
	m_clrBackground = clr;
	uFlags |= OX_SUBITEM_BGCOLOR;
}

COLORREF COXTreeItem::GetItemColor(int nCol) const
{
	ASSERT(HasColor(nCol));
	if(!nCol)
		return m_clr;
	COXTreeSubItem *pSubItem = GetSubItem(nCol);
	if(!pSubItem)
	{
		ASSERT(FALSE);
		return 0;
	}
	return pSubItem->m_clr;
}

COLORREF COXTreeItem::GetItemBackColor(int nCol) const
{
	ASSERT(HasBackColor(nCol));
	if(!nCol)
		return m_clrBackground;
	COXTreeSubItem *pSubItem = GetSubItem(nCol);
	if(!pSubItem)
	{
		ASSERT(FALSE);
		return 0;
	}
	return pSubItem->m_clrBackground;
}

void COXTreeItem::SetDrawEllipsis(UINT nEllipsisFormat, int nCol/*=0*/)
{
	ASSERT(nEllipsisFormat==DT_END_ELLIPSIS || nEllipsisFormat==DT_PATH_ELLIPSIS || 
		nEllipsisFormat==DT_WORD_ELLIPSIS);
	if(nCol==0)
		m_nEllipsisFormat = nEllipsisFormat;
	else
	{
		COXTreeSubItem *pSubItem = GetSubItem(nCol);
		ASSERT(pSubItem!=NULL);
		pSubItem->SetDrawEllipsis(nEllipsisFormat);
	}
}

void COXTreeSubItem::SetDrawEllipsis(UINT nEllipsisFormat)
{
	ASSERT(nEllipsisFormat==DT_END_ELLIPSIS || nEllipsisFormat==DT_PATH_ELLIPSIS || 
		nEllipsisFormat==DT_WORD_ELLIPSIS);
	m_nEllipsisFormat = nEllipsisFormat;
}

UINT COXTreeItem::GetDrawEllipsis(int nCol/*=0*/) const
{
	if(nCol==0)
		return m_nEllipsisFormat;
	else
	{
		COXTreeSubItem *pSubItem = GetSubItem(nCol);
		if(pSubItem!=NULL)
			return pSubItem->GetDrawEllipsis();
		else
			return DT_END_ELLIPSIS;
	}
}

BOOL COXTreeItem::HasFont(int nCol) const
{
	if(!nCol)
		return (BOOL)(dwStateEx & XTIS_HASFONT);
	COXTreeSubItem *pSubItem = GetSubItem(nCol);
	if(!pSubItem)
		return FALSE;
	return pSubItem->HasFont();
}

BOOL COXTreeItem::SetFont(int nCol, CFont* pFont)
{
	if(nCol == 0)
	{
		if(pFont==NULL)
		{
			dwStateEx &= ~XTIS_HASFONT;
			return TRUE;
		}

		LOGFONT lf;
		if(!pFont->GetLogFont(&lf))
		{
			return FALSE;
		}
		if((HFONT)m_font!=NULL)
		{
			m_font.DeleteObject();
		}
		if(m_font.CreateFontIndirect(&lf))
		{
			dwStateEx |= XTIS_HASFONT;
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		COXTreeSubItem *pSubItem = GetSubItem(nCol);
		if(!pSubItem)
			return FALSE;
		return pSubItem->SetFont(pFont);
	}
}

CFont* COXTreeItem::GetItemFont(int nCol)
{
	if(!HasFont(nCol))
		return NULL;
	if(!nCol)
	{
		return &m_font;
	}
	else
	{
		COXTreeSubItem *pSubItem = GetSubItem(nCol);
		if(!pSubItem)
			return NULL;
		return &(pSubItem->m_font);
	}
}

void COXTreeItem::SetDisabled(BOOL bDisable)
{
	if(bDisable)
		dwStateEx |= XTIS_DISABLED;
	else
		dwStateEx &= ~XTIS_DISABLED;

}

BOOL COXTreeItem::IsDisabled() const
{
	return (dwStateEx & XTIS_DISABLED);
}

BOOL COXTreeItem::SetSubItemText(LPCTSTR lpszText,int nCol)
	{
	if(nCol <= 0)
		{
		// column must be > 0
		ASSERT(FALSE);
		return FALSE;
		}
	return SetSubItem(nCol,OX_SUBITEM_TEXT,lpszText);
	}

void COXTreeItem::SetEditMode(UINT uMode,CStringArray& saTextEx,int nCol)
	{
	if(nCol == 0)
		{
		//set main item
		switch(uMode)
			{
			case OXET_EDIT:
			case OXET_NOEDIT:
			case OXET_COMBO:
			case OXET_CALENDAR:
				m_uEditMode = uMode;
				break;
			default:
				// Undefined edit mode found!
				ASSERT(FALSE);
				break;
			}
		m_saTextEx.RemoveAll();
		for(int i=0;i < saTextEx.GetSize();i++)
			m_saTextEx.SetAtGrow(i,saTextEx[i]);
		}
	else
		{
		COXTreeSubItem *pSubItem = GetSubItem(nCol);
		ASSERT(pSubItem);
		if(pSubItem)
			pSubItem->SetEditMode(uMode,saTextEx);
		}
	}

void COXTreeSubItem::SetEditMode(UINT uMode,CStringArray& saTextEx)
{
	switch(uMode)
	{
	case OXET_EDIT:
	case OXET_NOEDIT:
	case OXET_COMBO:
	case OXET_CALENDAR:
		m_uEditMode = uMode;
		break;
	default:
		// Undefined edit mode found!
		ASSERT(FALSE);
		break;
	}
	m_saTextEx.RemoveAll();
	for(int i=0;i < saTextEx.GetSize();i++)
		m_saTextEx.SetAtGrow(i,saTextEx[i]);
}

UINT COXTreeItem::GetEditMode(int nCol) const
{
	if(nCol == 0)
		return m_uEditMode;
	COXTreeSubItem *pSubItem = GetSubItem(nCol);
	ASSERT(pSubItem);
	if(pSubItem)
		return pSubItem->GetEditMode();
	return 0;
}

UINT COXTreeSubItem::GetEditMode() const
{
	return m_uEditMode;
}

CStringArray& COXTreeSubItem::GetTextEx()
{
	return m_saTextEx;
}

CStringArray& COXTreeItem::GetTextEx(int nCol)
{
	if(nCol == 0)
		return m_saTextEx;

	COXTreeSubItem *pSubItem = GetSubItem(nCol);
	ASSERT(pSubItem);

	return pSubItem->GetTextEx();
}

void COXTreeItem::SetTextEx(CStringArray& saText,int nCol)
	{
	if(nCol == 0)
		{
		m_saTextEx.RemoveAll();
		for(int i=0;i < saText.GetSize();i++)
			m_saTextEx.SetAtGrow(i,saText[i]);
		}
	else
		{
		COXTreeSubItem *pSubItem = GetSubItem(nCol);
		ASSERT(pSubItem);
		pSubItem->SetTextEx(saText);
		}
	}

void COXTreeSubItem::SetTextEx(CStringArray& saText)
	{	
		m_saTextEx.RemoveAll();
		for(int i=0;i < saText.GetSize();i++)
			m_saTextEx.SetAtGrow(i,saText[i]);
	}

COXTreeItem* COXTreeItem::FindItemToInsertAfter(LPCTSTR lpszItem, 
												BOOL bAscending/*=TRUE*/) const
{
	ASSERT(this);

	CString sItemText=lpszItem;
	if(sItemText.IsEmpty())
	{
		return bAscending ? NULL : (COXTreeItem*)TVI_FIRST;
	}

	int nKoef=bAscending ? 1 : -1;
	COXTreeItem* pInsertAfter=NULL;
	COXTreeItem* xti=pxFirstChild;
	CString sTextToCompare;
	CString sClosest=sItemText;
	BOOL bFirstTime=TRUE;
	while(xti)
	{
		if (m_tvi.pszText)
		{
			sTextToCompare=xti->m_tvi.pszText;
			int result=sItemText.Collate(sTextToCompare)*nKoef;
			if(result>0)
			{
				result=sClosest.Collate(sTextToCompare)*nKoef;
				if(result<0 || bFirstTime)
				{
					bFirstTime=FALSE;
					pInsertAfter=xti;
					sClosest=sTextToCompare;
				}
			}
		}
	
		xti = xti->pxNext;
	}

	return pInsertAfter==NULL ? (COXTreeItem*)TVI_FIRST : pInsertAfter;
}

UINT COXTreeItem::GetChildrenCount() const
{
	UINT result=0;
	COXTreeItem* xti=pxFirstChild;
	while(xti)
	{
		result++;
		xti = xti->pxNext;
	}

	return result;
}

BOOL COXTreeItem::SortChildren(int nCol/*=0*/, 
							   BOOL bOnlyChildren/*=TRUE*/, 
							   BOOL bAscending/*=TRUE*/, 
							   PFNTVCOMPARE lpfnCompare/*=NULL*/, 
							   LPARAM lParam/*=NULL*/)
{
	if(!ItemHasChildren())
	{
		return TRUE;
	}

	BOOL bNotDone = TRUE;
	while (bNotDone)
	{      
		bNotDone = FALSE;	
		COXTreeItem* xti=pxFirstChild;
		BOOL bFirstChild=TRUE;
		while(xti && xti->pxNext)
		{
			BOOL bResult;
			if(lpfnCompare==NULL)
			{
				bResult=Compare(xti,nCol,bAscending);
			}
			else
			{
				bResult=lpfnCompare(xti->m_tvi.lParam,xti->pxNext->m_tvi.lParam,lParam)*
					(bAscending ? 1 : -1)>0 ? TRUE : FALSE;
			}
			bNotDone|=bResult;
			if(bResult)
			{
				Swap(xti);
				if(bFirstChild)
				{
					pxFirstChild=xti->pxPrev;
				}
				xti=xti->pxPrev;
			}
			xti=xti->pxNext;
			bFirstChild=FALSE;
		}
	}

	if(!bOnlyChildren)
	{
		COXTreeItem* xti=pxFirstChild;
		while(xti)
		{
			if(xti->ItemHasChildren())
			{
				xti->SortChildren(nCol,bOnlyChildren,bAscending,
					lpfnCompare,lParam);
			}
			xti=xti->pxNext;
		}
	}

	return TRUE;
}

BOOL COXTreeItem::Compare(COXTreeItem* xti, int nCol/*=0*/, BOOL bAscending/*=TRUE*/) 
{ 
	COXTreeItem* xtiNext=xti->pxNext;
	if(xtiNext==NULL || xti->m_tvi.pszText == LPSTR_TEXTCALLBACK || xtiNext->m_tvi.pszText == LPSTR_TEXTCALLBACK)
	{
		return FALSE;
	}

	CString sText=nCol==0 ? xti->m_tvi.pszText : xti->GetSubItemText(nCol);
	CString sTextToCompare=nCol==0 ? xtiNext->m_tvi.pszText : 
		xtiNext->GetSubItemText(nCol);
	int nKoef=bAscending ? 1 : -1;
	return sText.Collate(sTextToCompare)*nKoef>0;
} 

void COXTreeItem::Swap(COXTreeItem* xti) 
{ 

	COXTreeItem* xtiNext=xti->pxNext;
	if(xtiNext==NULL)
	{
		return;
	}

	COXTreeItem* xtiTemp=xti->pxPrev;
	if(xti->pxPrev)
	{
		xti->pxPrev->pxNext=xti->pxNext;
	}
	if(xtiNext->pxNext)
	{
		xtiNext->pxNext->pxPrev=xti;
	}
	xti->pxPrev=xtiNext;
	xti->pxNext=xtiNext->pxNext;
	xtiNext->pxPrev=xtiTemp;
	xtiNext->pxNext=xti;
}

COXTreeItem* COXTreeItem::CopyChild(COXTreeItem* pChildToCopy, 
									COXTreeItem* pInsAfter/*=NULL*/, 
									BOOL bCopyDescendants/*=TRUE*/)
{
	COXTreeItem* pNewChild=new COXTreeItem;
	ASSERT(pNewChild!=NULL);

	*pNewChild=*pChildToCopy;
	pNewChild->pxParent=NULL;
	pNewChild->pxNext=NULL;
	pNewChild->pxPrev=NULL;
	pNewChild->pxFirstChild=NULL;

	AddChild(pNewChild,pInsAfter);

	if(bCopyDescendants && pChildToCopy->pxFirstChild!=NULL)
	{
		pNewChild->pxFirstChild=pNewChild->CopyChild(pChildToCopy->pxFirstChild,
			(COXTreeItem*)TVI_FIRST,bCopyDescendants);
		ASSERT(pNewChild->pxFirstChild!=NULL);

		pInsAfter=pNewChild->pxFirstChild;
		COXTreeItem* pNextChild=pChildToCopy->pxFirstChild->pxNext;
		while(pNextChild!=NULL)
		{
			pInsAfter=pNewChild->CopyChild(pNextChild,pInsAfter,bCopyDescendants);
			ASSERT(pInsAfter!=NULL);
			pNextChild=pNextChild->pxNext;
		}
	}

	return pNewChild;
}


BOOL COXTreeItem::IsAnyParentCollapsed()
{
	// Walk up the parent tree and return FALSE if a collapsed parent if found

	COXTreeItem* pParent = pxParent;
	while (pParent != NULL)
	{
		if (!pParent->IsExpanded())
			return TRUE;
		pParent = pParent->pxParent;
	}

	return FALSE;
}
