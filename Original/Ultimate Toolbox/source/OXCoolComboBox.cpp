// ==========================================================================
//						   Class Implementation : 
//					COXCoolComboEdit & COXCoolComboBox
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
#include "OXCoolComboBox.h"
#include "UTB64Bit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// COXCoolComboEdit

void COXCoolComboEdit::SetCombo(COXCoolComboBox* pCombo)
{
	ASSERT(pCombo!=NULL);
	m_pCombo=pCombo;
}

COXCoolComboBox* COXCoolComboEdit::GetCombo()
{
	return m_pCombo;
}

void COXCoolComboEdit::OnFocusChange(CWnd* pWndGainFocus, CWnd* pWndLooseFocus) 
{
	ASSERT(m_pCombo!=NULL);
	if((pWndGainFocus==this && !m_bInHotState) ||
		(pWndLooseFocus==this && m_bInHotState))
	{
		m_pCombo->OnChangeHotState(!m_bInHotState);
	}
	COXCoolCtrl<CEdit>::OnFocusChange(pWndGainFocus, pWndLooseFocus);
}

BEGIN_MESSAGE_MAP(COXCoolComboEdit, CEdit)
	//{{AFX_MSG_MAP(COXCoolComboEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




/////////////////////////////////////////////////////////////////////////////
// COXCoolComboBox

COXCoolComboBox::COXCoolComboBox()
{
    m_nDefaultFontHeight=0;
    m_nDefaultFontHeightSansLeading=0;
}

COXCoolComboBox::~COXCoolComboBox()
{
}


BEGIN_MESSAGE_MAP(COXCoolComboBox, CComboBox)
	//{{AFX_MSG_MAP(COXCoolComboBox)
	ON_WM_PAINT()
	ON_WM_PARENTNOTIFY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT_EX(CBN_CLOSEUP,OnUpdateCombo)   
	ON_MESSAGE(WM_SETFONT,OnSetFont)   
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXCoolComboBox message handlers

BOOL COXCoolComboBox::OnUpdateCombo()
{
	CheckMousePos();
	RedrawWindow();
	return TRUE;
}


// This message is handle to get the HWND of the listbox of the combobox
// as there is no otherway to get it(refer to Q65881, Q131845 in Knowledge Base)
HBRUSH COXCoolComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hBrush = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor); 
	
	// Handle only if the type of control is Listbox only
	if((nCtlColor == CTLCOLOR_LISTBOX) && 
		!(GetStyle()&CBS_OWNERDRAWFIXED || GetStyle()&CBS_OWNERDRAWVARIABLE))
	{
		if(!::IsWindow(m_listBox.GetSafeHwnd()))
		{
			if(m_listBox.SubclassWindow(pWnd->GetSafeHwnd()))
			{
				CString sTooltipText;
				if(GetTooltipText(sTooltipText))
					VERIFY(m_listBox.SetTooltipText(sTooltipText));
				m_listBox.RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_FRAME);
			}
		}
	}

	return hBrush; 
}


void COXCoolComboBox::OnParentNotify(UINT message, LPARAM lParam) 
{
	COXCoolCtrl<CComboBox>::OnParentNotify(message, lParam);
	
	// TODO: Add your message handler code here
	if(LOWORD(message)==WM_CREATE)	
	{
		if(HIWORD(message)==IDW_OXCOMBOLISTBOX)
			SubclassListBoxCtrl();
		if(HIWORD(message)==IDW_OXCOMBOEDIT)
			SubclassEditCtrl();
	}
}	


LRESULT COXCoolComboBox::OnSetFont(WPARAM wParam,LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

    Default();

    CDC* pDC=GetDC();
    pDC->SelectObject(GetFont());
    TEXTMETRIC tm;
    pDC->GetTextMetrics(&tm);
    m_nDefaultFontHeight=tm.tmHeight;
    m_nDefaultFontHeightSansLeading=pDC->GetTextExtent(_T("ygaQ!|")).cy;
    ReleaseDC(pDC);    

    int nCurrentHeight=PtrToInt(SendMessage(CB_GETITEMHEIGHT,(WPARAM)-1,0));

    if (nCurrentHeight!=m_nDefaultFontHeight+1)
        SendMessage(CB_SETITEMHEIGHT,(WPARAM)-1,m_nDefaultFontHeight+1);        

	return 0;
}


void COXCoolComboBox::OnPaint() 
{
	if(!IsInCoolState())
	{
		Default();
		return;
	}

	// set up all the pens we are likely to need
    HPEN penDefault=(HPEN)GetStockObject(BLACK_PEN);
    static CPen penHilite(PS_SOLID,1,GetSysColor(COLOR_3DHIGHLIGHT));
    static CPen penShadow(PS_SOLID,1,GetSysColor(COLOR_3DSHADOW));
    static CPen penFace3D(PS_SOLID,1,GetSysColor(COLOR_3DFACE));
    static CPen penDkShad3D(PS_SOLID,1,GetSysColor(COLOR_3DDKSHADOW));

    // store margins for future use
    int nLeftMargin=GetLeftMargin(); 
	int nRightMargin=GetRightMargin();
	int nTopMargin=GetTopMargin();
	int nBottomMargin=GetBtmMargin();
    CRect button_rect=GetButtonRect();

	const BOOL bSimpleCombo=((GetStyle()&0x000f)==CBS_SIMPLE);
    const BOOL bIsEnabled=IsWindowEnabled();
	const BOOL bIsHot=IsInHotState() & bIsEnabled;

    // paint background
    CPaintDC paintdc(this);
    CDC* dc=&paintdc;

    CRect rect, rectClient, rectInternal; // temp rect, client rect, internal rect respectively

    GetClientRect(rectClient);
	rectClient.right-=::GetSystemMetrics(SM_CXVSCROLL)-GetButtonWidth();

	if(bSimpleCombo)
	{
		ASSERT(::IsWindow(m_edit.GetSafeHwnd()));
		CRect rectEdit;
		m_edit.GetWindowRect(rectEdit);
		rectClient.bottom=rectClient.top+rectEdit.Height()+5;
	}

    // set up internal rectangle
    rectInternal=rectClient;
    rectInternal.left+=nLeftMargin;
    rectInternal.top+=nTopMargin;
    rectInternal.right-=nRightMargin;
    rectInternal.bottom-=nBottomMargin;

    rect=rectInternal;
	if(!button_rect.IsRectEmpty())
		rect.right=button_rect.left;
    if(!bIsEnabled)
        ::FillRect(dc->m_hDC,&rect,GetSysColorBrush(COLOR_3DFACE));
    else
        ::FillRect(dc->m_hDC,&rect,GetSysColorBrush(COLOR_WINDOW));

    // do default painting
    if(!::IsWindow(m_edit.GetSafeHwnd()))
    {
        DRAWITEMSTRUCT dis={ sizeof(&dis) };
        dis.CtlType=ODT_COMBOBOX;
        dis.CtlID=GetWindowLongPtr(m_hWnd, GWL_ID);
        dis.itemID=GetCurSel();
        dis.itemAction=ODA_DRAWENTIRE;
        dis.hwndItem=m_hWnd;
        dis.hDC=dc->m_hDC;
        dis.rcItem=CRect(nLeftMargin+1,nTopMargin+1, 
			button_rect.left-1,rectClient.bottom-nBottomMargin-1);
        dis.itemData=GetItemData(GetCurSel());
        dis.itemState=((::GetFocus()==GetSafeHwnd() && !IsDroppedDown()) ? 
			ODS_FOCUS : 0);

        DrawItem(&dis);
    }

    // now do our painting - first borderect...
    // but first exclude the 'real' client area from the DC
    dc->SelectClipRgn(NULL);   // from drawing background
    dc->ExcludeClipRect(&rectInternal);

    // draw border
	if(!bIsHot)
	{
		// now paint background 
	    if(bSimpleCombo)
			FillRect(dc->m_hDC, &rectClient, ::GetSysColorBrush(COLOR_WINDOW));
		else
			FillRect(dc->m_hDC, &rectClient, ::GetSysColorBrush(COLOR_3DFACE));

		// fill in small area to the left of edit window in appropriate colour, 
		// to make window look bigger
		rect=rectClient;
		rect.top+=nTopMargin;
		rect.left+=nLeftMargin-1;
		rect.right=rect.left+1;
		rect.bottom-=nBottomMargin;

		if(bIsEnabled)
			FillRect(dc->m_hDC, &rect, GetSysColorBrush(COLOR_WINDOW));
		else
			FillRect(dc->m_hDC, &rect, GetSysColorBrush(COLOR_3DFACE));
	}
	else
	{
		// HOT case - difficult to describe, but reasonably obvious
		rect=rectClient;
		rect.left++;
		rect.right--;
		rect.top++;
		rect.bottom-=1+EXTRA_BOTTOM_MARGIN;            

		// paint background 
		FillRect(dc->m_hDC, &rect, GetSysColorBrush(COLOR_3DFACE));

		// and border
		rect=rectClient;
		SelectObject(dc->m_hDC,penShadow);
		dc->MoveTo(0,0);
		dc->LineTo(rect.right,0);
		dc->MoveTo(0,0);
		dc->LineTo(0,rect.bottom-1-EXTRA_BOTTOM_MARGIN);

		SelectObject(dc->m_hDC,penHilite);
		dc->LineTo(rect.right-1,rect.bottom-1-EXTRA_BOTTOM_MARGIN);
		dc->LineTo(rect.right-1,rect.top);

		// and paint extra margin
		if(EXTRA_BOTTOM_MARGIN)
		{
			rect=rectClient;
			rect.top=rect.bottom-EXTRA_BOTTOM_MARGIN;        

			FillRect(dc->m_hDC, &rect, GetSysColorBrush(COLOR_3DFACE));
		}
	}

    // unselect the clip region
    SelectClipRgn(dc->m_hDC, NULL);

    // now draw highlight border, if disabled
    if(!bIsEnabled)
    {
        SelectObject(dc->m_hDC, GetStockObject(NULL_BRUSH));
        SelectObject(dc->m_hDC, penHilite);
        dc->Rectangle(rectInternal.left-1, rectInternal.top-1, 
			rectInternal.right+1, rectInternal.bottom+1);
    }

    // and now for the button
    BOOL pressed=IsDroppedDown();
    rect=button_rect;

    // do the background
    if(bIsHot)
    {
        rect.bottom--;
        rect.right++;
    }

    if(bIsHot || pressed || !bIsEnabled)
    {
        ::FillRect(dc->m_hDC, &rect, GetSysColorBrush(COLOR_3DFACE));
    }
    else
    {
		if(bIsEnabled)
		{
			// cold and unpressed
			SelectObject(dc->m_hDC, penHilite);
			SelectObject(dc->m_hDC, GetSysColorBrush(COLOR_3DFACE));
			::Rectangle(dc->m_hDC, rect.left, rect.top, rect.right, rect.bottom);
		}
    }

    rect=button_rect; // set again

    if(bIsHot || pressed)
    {
        if(pressed)
        {
            SelectObject(dc->m_hDC, penShadow);
            dc->MoveTo(rect.left , rect.top);
            dc->LineTo(rect.right, rect.top);
            dc->MoveTo(rect.left , rect.top);
            dc->LineTo(rect.left , rect.bottom);
        }
        else
        {
            SelectObject(dc->m_hDC, penHilite);
            dc->MoveTo(rect.right,  rect.top);
            dc->LineTo(rect.left+1, rect.top);
            dc->LineTo(rect.left+1, rect.bottom);
            SelectObject(dc->m_hDC, penFace3D);
            dc->MoveTo(rect.left, rect.top);
            dc->LineTo(rect.left, rect.bottom);
            SelectObject(dc->m_hDC, penShadow);
            dc->MoveTo(rect.left , rect.bottom - 1);
            dc->LineTo(rect.right-1, rect.bottom - 1);
            dc->LineTo(rect.right-1, rect.top);
        }

        SelectObject(dc->m_hDC, penDefault);
    }

    // now draw shadowy arrow
    SelectObject(dc->m_hDC, penDkShad3D);
    SelectObject(dc->m_hDC, ::GetSysColorBrush(COLOR_3DDKSHADOW));

    POINT p[3];

    p[0].x=rect.left +((rect.right - rect.left) / 3);
    p[1].x=rect.left +(2 *(rect.right - rect.left) / 3) + 1;
    p[2].x=(p[0].x + p[1].x) / 2;

    if(((p[1].x - p[0].x) % 2))
        p[1].x--;

    p[0].y=(rect.top + rect.bottom / 2) -(p[1].x - p[0].x) / 4 - 1;
    p[1].y=p[0].y;
    p[2].y=p[0].y +(p[1].x - p[0].x) / 2;

    if(pressed)    // offset by a pixel
    {
        p[0].x++;
        p[0].y++;
        p[1].x++;
        p[1].y++;
        p[2].x++;
        p[2].y++;
    }

    Polygon(dc->m_hDC, p, 3);

}

LRESULT COXCoolComboBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	if(message==WM_NCPAINT)
	{
		return CComboBox::WindowProc(message,wParam,lParam);
	}
	else
	{
		return COXCoolCtrl<CComboBox>::WindowProc(message,wParam,lParam);
	}
}


void COXCoolComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	
    int nItem=GetCurSel();
    CString sText(_T(""));

    if(lpDrawItemStruct->itemID!=-1)
        nItem=lpDrawItemStruct->itemID;

    if(nItem!=-1)
        GetLBText(nItem,sText);

    BOOL bHighlight=(lpDrawItemStruct->itemState & (ODS_FOCUS | ODS_SELECTED));
    BOOL bFocus=(lpDrawItemStruct->itemState & ODS_FOCUS);

    CRect rect=lpDrawItemStruct->rcItem;

    // fill background
    ::FillRect(lpDrawItemStruct->hDC,rect, 
		::GetSysColorBrush(!IsWindowEnabled() ? COLOR_3DFACE : 
		(bHighlight ? COLOR_HIGHLIGHT : COLOR_WINDOW)));

	if(!sText.IsEmpty())
	{
		if(bFocus)
			::DrawFocusRect(lpDrawItemStruct->hDC,rect);

	    // select font
		::SelectObject(lpDrawItemStruct->hDC,(HFONT)SendMessage(WM_GETFONT,0,0));

		// set text attributes
		::SetBkMode(lpDrawItemStruct->hDC,TRANSPARENT);

		if(bHighlight)
			::SetTextColor(lpDrawItemStruct->hDC,
				::GetSysColor(COLOR_HIGHLIGHTTEXT));
		else if(IsWindowEnabled())
			::SetTextColor(lpDrawItemStruct->hDC,
				::GetSysColor(COLOR_WINDOWTEXT));
		else 
			::SetTextColor(lpDrawItemStruct->hDC,
				::GetSysColor(COLOR_3DSHADOW));

		rect.DeflateRect(1,1);
		// draw the text
		::DrawText(lpDrawItemStruct->hDC,sText,sText.GetLength(),rect,
			DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	}
}


void COXCoolComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpms)
{
    if(lpms->itemID<0)
        lpms->itemHeight=m_nDefaultFontHeight+1; 
    else
        lpms->itemHeight=m_nDefaultFontHeightSansLeading+1; 
}


int COXCoolComboBox::CompareItem(LPCOMPAREITEMSTRUCT lpcs)
{
    // default behaviour
    CString sText1(_T(""));
	CString sText2(_T(""));
	if(lpcs->itemID1!=-1)
		GetLBText(lpcs->itemID1,sText1);
	if(lpcs->itemID2!=-1)
	    GetLBText(lpcs->itemID2,sText2);

    return sText1.CompareNoCase(sText2);
}


void COXCoolComboBox::OnChangeHotState(BOOL bInHotState) 
{
	if(!bInHotState && ::GetFocus()==m_edit.GetSafeHwnd())
		return;
	COXCoolCtrl<CComboBox>::OnChangeHotState(bInHotState);
	if(!IsDroppedDown())
	{
		RedrawWindow();
	}
}


BOOL COXCoolComboBox::OnChangeTooltipText(const CString& sTooltipText)
{
	BOOL bResult=TRUE;
	if(::IsWindow(m_edit.GetSafeHwnd()))
		bResult&=m_edit.SetTooltipText(sTooltipText); 
	if(::IsWindow(m_listBox.GetSafeHwnd()))
		bResult&=m_listBox.SetTooltipText(sTooltipText); 

	return bResult;
}


BOOL COXCoolComboBox::InitializeCoolCtrl()
{
	if(!COXCoolCtrl<CComboBox>::InitializeCoolCtrl())
		return FALSE;

    CDC* pDC=GetDC();
    pDC->SelectObject(GetFont());
    TEXTMETRIC tm;
    pDC->GetTextMetrics(&tm);
    m_nDefaultFontHeight=tm.tmHeight;
    m_nDefaultFontHeightSansLeading=pDC->GetTextExtent(_T("ygaQ!|")).cy;
    ReleaseDC(pDC);    

	if(!SubclassEditCtrl())
		return FALSE;
	if(!SubclassListBoxCtrl())
		return FALSE;

	return TRUE;
}


void COXCoolComboBox::PreSubclassWindow() 
{
	COXCoolCtrl<CComboBox>::PreSubclassWindow();

	if(m_bInitializedCoolCtrl)
	{
		if(GetStyle()&(CBS_OWNERDRAWFIXED|CBS_OWNERDRAWVARIABLE))
		{
			_AFX_THREAD_STATE* pThreadState=AfxGetThreadState();
			// hook not already in progress
			if(pThreadState->m_pWndInit==NULL)
			{
				SendMessage(CB_SETITEMHEIGHT,(WPARAM)-1,m_nDefaultFontHeight+1);        
			}
		}
	}
}


int COXCoolComboBox::GetButtonWidth() const
{
	return ::GetSystemMetrics(SM_CXVSCROLL);
}

CRect COXCoolComboBox::GetButtonRect() const 
{
    CRect rect(0,0,0,0);
	BOOL bSimpleCombo=((GetStyle()&0x000f)==CBS_SIMPLE);
	if(bSimpleCombo)
		return rect;

    GetClientRect(&rect);

    int nRightMargin=GetRightMargin();
	int nTopMargin=GetTopMargin();
	int nBottomMargin=GetBtmMargin();
	int nButtonWidth=GetButtonWidth();

    rect.left=rect.right-nRightMargin-::GetSystemMetrics(SM_CXVSCROLL);
    rect.top+=nTopMargin;
    rect.right=rect.left+nButtonWidth;
    rect.bottom-=nBottomMargin;

    if(rect.left<0)
        rect.left=0;

    return rect;
}


BOOL COXCoolComboBox::SubclassEditCtrl()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	HWND hWnd;
	GetDlgItem(IDW_OXCOMBOEDIT,&hWnd);
	if(hWnd!=NULL)
	{
		m_edit.SetCombo(this);
		if(!m_edit.SubclassWindow(hWnd))
			return FALSE;

		CString sTooltipText;
		if(GetTooltipText(sTooltipText))
			VERIFY(m_edit.SetTooltipText(sTooltipText));
	}

	return TRUE;
}


BOOL COXCoolComboBox::SubclassListBoxCtrl()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	if(GetStyle()&CBS_OWNERDRAWFIXED ||	GetStyle()&CBS_OWNERDRAWVARIABLE)
		return TRUE;

	HWND hWnd;
	GetDlgItem(IDW_OXCOMBOLISTBOX,&hWnd);
	if(hWnd!=NULL)
	{
		if(!m_listBox.SubclassWindow(hWnd))
			return FALSE;

		CString sTooltipText;
		if(GetTooltipText(sTooltipText))
			VERIFY(m_listBox.SetTooltipText(sTooltipText));
	}

	return TRUE;
}


