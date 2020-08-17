// OXFontComboBox.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXFontComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNAMIC(COXFontListBoxHelper, COXFontListBox)
/////////////////////////////////////////////////////////////////////////////

COXFontListBoxHelper::COXFontListBoxHelper() :
	m_pComboBox(NULL)
{
}

COXFontListBoxHelper::~COXFontListBoxHelper()
{
}


BEGIN_MESSAGE_MAP(COXFontListBoxHelper, COXFontListBox)
	//{{AFX_MSG_MAP(COXFontListBoxHelper)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXFontListBox message handlers
BOOL COXFontListBoxHelper::FilterFont(OXLBFONTINFO* pLBFI) 
{ 
	if(GetAssociateComboBox()!=NULL)
		return (GetAssociateComboBox()->FilterFont(pLBFI));
	else
		return (COXFontListBox::FilterFont(pLBFI)); 
}


BOOL COXFontListBoxHelper::InitializeListBox()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	int nIndexSelected=-1;
	BOOL bIsMultipleSel=(GetStyle()&LBS_MULTIPLESEL);
	if(!bIsMultipleSel)
	{
		nIndexSelected=GetCurSel();
	}

	if(GetCount()>0)
	{
		ResetContent();
	}

	if(!COXListBoxEx::InitializeListBox())
		return FALSE;

	SetRedraw(FALSE);

	PopulateWithFonts();
	if(GetSaveRestoreMRUState())
		RestoreMRUState();

	SetCurSel(nIndexSelected);

	SetRedraw(TRUE);


	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// COXFontComboBox

IMPLEMENT_DYNAMIC(COXFontComboBox, CComboBox)

void AFXAPI DDX_FontComboFont(CDataExchange *pDX, int nIDC, CFont* pFont)
{
	ASSERT(pFont!=NULL);

    HWND hWnd=pDX->PrepareCtrl(nIDC);
    ASSERT(hWnd!=NULL);                
    
    COXFontComboBox* pComboBox=
		DYNAMIC_DOWNCAST(COXFontComboBox,CWnd::FromHandle(hWnd));
	ASSERT(pComboBox!=NULL);
    if(pDX->m_bSaveAndValidate)
    {
		if((HFONT)*pFont!=NULL)
			pFont->DeleteObject();
		CFont* pSelectedFont=pComboBox->GetSelectedFont();
		if(pSelectedFont!=NULL)
		{
			LOGFONT lf;
			VERIFY(pSelectedFont->GetLogFont(&lf));
			VERIFY(pFont->CreateFontIndirect(&lf));
		}
    }
    else 
    {
        pComboBox->SelectFont(pFont);
    }
}


COXFontComboBox::COXFontComboBox() : 
	m_bAllowToUpdateMRU(FALSE)
{
}

COXFontComboBox::~COXFontComboBox()
{
}


BEGIN_MESSAGE_MAP(COXFontComboBox, CComboBox)
	//{{AFX_MSG_MAP(COXFontComboBox)
	ON_WM_ERASEBKGND()
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE, OnSelChange)
	ON_CONTROL_REFLECT_EX(CBN_SELENDOK, OnSelEndOk)
	ON_CONTROL_REFLECT_EX(CBN_CLOSEUP, OnCloseUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXFontComboBox message handlers

int COXFontComboBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	// TODO: Add your code to determine the sorting order of the specified items
	// return -1 = item 1 sorts before item 2
	// return 0 = item 1 and item 2 sort the same
	// return 1 = item 1 sorts after item 2
	
	ASSERT(GetListBoxCtrl()!=NULL);
	return GetListBoxCtrl()->CompareItem(lpCompareItemStruct);
}

void COXFontComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	if(lpDrawItemStruct->itemState&ODS_COMBOBOXEDIT)
	{
		lpDrawItemStruct->itemID=GetCurSel();
		CString sText(_T(""));
		if(lpDrawItemStruct->itemID!=CB_ERR)
		{
			GetListBoxCtrl()->GetText(lpDrawItemStruct->itemID,sText);
		}
		CDC* pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
		CRect rect(lpDrawItemStruct->rcItem);

		if((lpDrawItemStruct->itemAction & (ODA_SELECT|ODA_DRAWENTIRE)))
		{
			CBrush brush(lpDrawItemStruct->itemState & ODS_SELECTED ?
				::GetSysColor(COLOR_HIGHLIGHT) : ::GetSysColor(COLOR_WINDOW));
			pDC->FillRect(rect,&brush);
		}
		else if((lpDrawItemStruct->itemAction & (ODA_FOCUS|ODA_DRAWENTIRE)) &&
			!(lpDrawItemStruct->itemState & ODS_FOCUS))
		{
			CBrush brush(lpDrawItemStruct->itemState & ODS_SELECTED ?
				::GetSysColor(COLOR_HIGHLIGHT) : ::GetSysColor(COLOR_WINDOW));
			pDC->FrameRect(rect,&brush);
		}

		CRect rectText=rect;
		rectText.DeflateRect(2,0);
		int nOldBkMode=pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(sText,rectText,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
		pDC->SetBkMode(nOldBkMode);

		if((lpDrawItemStruct->itemAction & (ODA_FOCUS|ODA_DRAWENTIRE)) &&
			(lpDrawItemStruct->itemState & ODS_FOCUS))
		{
			pDC->DrawFocusRect(rect);
		}
	}
	else
	{
		ASSERT(GetListBoxCtrl()!=NULL);
		GetListBoxCtrl()->DrawItem(lpDrawItemStruct);
	}
}

void COXFontComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your code to determine the size of specified item
	UNREFERENCED_PARAMETER(lpMeasureItemStruct);
}

BOOL COXFontComboBox::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}


CFont* COXFontComboBox::GetSelectedFont()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	int nIndex=GetCurSel();
	if(nIndex==CB_ERR)
	{
		return NULL;
	}
	else
	{
		ASSERT(GetListBoxCtrl()!=NULL);
		return ((COXListBoxEx*)GetListBoxCtrl())->GetItemFont(nIndex);
	}
}


int COXFontComboBox::SelectFont(CFont* pFont)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(pFont!=NULL);

	LOGFONT lf;
	if(!pFont->GetLogFont(&lf))
	{
		TRACE(_T("COXFontComboBox::SelectFont: failed to retrieve log info for specified CFont object\n"));
		return CB_ERR;
	}
	return SelectFont(&lf);
}


int COXFontComboBox::SelectFont(LOGFONT* pLF)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(pLF!=NULL);

	return SelectFont(pLF->lfFaceName);
}


int COXFontComboBox::SelectFont(LPCTSTR lpszFontName)
{
	ASSERT(lpszFontName!=NULL);

	CString sFontName=lpszFontName;
	if(sFontName.IsEmpty())
		return SetCurSel(-1);
	else
		return SelectString(-1,sFontName);
}

BOOL COXFontComboBox::OnCloseUp() 
{
	// TODO: Add your control notification handler code here
	m_bAllowToUpdateMRU=TRUE;
	return FALSE;
}


BOOL COXFontComboBox::OnSelEndOk() 
{
	// TODO: Add your control notification handler code here
	return FALSE;
}


BOOL COXFontComboBox::OnSelChange() 
{
	// TODO: Add your control notification handler code here
	if(m_bAllowToUpdateMRU)
	{
		ASSERT(GetListBoxCtrl()!=NULL);
		if(((COXListBoxEx*)GetListBoxCtrl())->GetMaxItemsBeforeSeparator()>0)
		{
			int nIndex=GetCurSel();
			if(nIndex!=CB_ERR)
			{
				VERIFY(((COXListBoxEx*)GetListBoxCtrl())->MRUInsert(nIndex,0)!=LB_ERR);
			}
		}

		m_bAllowToUpdateMRU=FALSE;
	}

	return FALSE;
}


void COXFontComboBox::SetSaveRestoreMRUState(BOOL bSaveRestore)
{
	ASSERT(GetListBoxCtrl()!=NULL);
	((COXListBoxEx*)GetListBoxCtrl())->SetSaveRestoreMRUState(bSaveRestore);
}


BOOL COXFontComboBox::GetSaveRestoreMRUState()
{
	ASSERT(GetListBoxCtrl()!=NULL);
	return ((COXListBoxEx*)GetListBoxCtrl())->GetSaveRestoreMRUState();
}


void COXFontComboBox::SubclassListBoxCtrl(CWnd* pWnd)
{
	TRACE(_T("COXFontComboBox::SubclassListBoxCtrl\n"));
	ASSERT(GetListBoxCtrl()!=NULL);
	if(::IsWindow(GetListBoxCtrl()->GetSafeHwnd()))
	{
		VERIFY(GetListBoxCtrl()->DestroyWindow());
	}

	COXBaseSubclassedComboBox<CComboBox,COXFontListBoxHelper,CEdit>::
		SubclassListBoxCtrl(pWnd);
	
	((COXFontListBoxHelper*)GetListBoxCtrl())->AssociateComboBox(this);
}


BOOL COXFontComboBox::InitializeComboBox()
{
	if(!COXBaseSubclassedComboBox<CComboBox,COXFontListBoxHelper,CEdit>::
		InitializeComboBox())
	{
		return FALSE;
	}

	COXListBoxEx* pListBox=(COXListBoxEx*)GetListBoxCtrl();
	ASSERT(pListBox!=NULL);
	ASSERT(!::IsWindow(pListBox->GetSafeHwnd()));
	VERIFY(pListBox->CreateEx(WS_EX_TOOLWINDOW,_T("ComboLBox"),_T(""),
		WS_CHILD|LBS_OWNERDRAWVARIABLE|LBS_HASSTRINGS|LBS_NOINTEGRALHEIGHT|WS_VISIBLE,
		CRect(0,0,0,0),GetParent(),0));
	for(int nIndex=0; nIndex<pListBox->GetCount(); nIndex++)
	{
		CString sText;
		pListBox->GetText(nIndex,sText);
		VERIFY(AddString(sText)>=0);
	}

	return TRUE;
}


