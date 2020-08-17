// ==========================================================================
//								Class Implementation : 
//								COXComboPickerCtrl
// ==========================================================================
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
// //////////////////////////////////////////////////////////////////////////

// Version: 9.3


#include "stdafx.h"
#include "OXComboPickerCtrl.h"

#include <afxpriv.h>


///////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(COXDropDownContainer, CWnd);

BEGIN_MESSAGE_MAP(COXDropDownContainer, CWnd)
	//{{AFX_MSG_MAP(COXDropDownContainer)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_MESSAGE(WM_FLOATSTATUS, OnFloatStatus)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TVN_ITEMEXPANDING,0,0xffff,OnItemExpanding)
	ON_NOTIFY_EX_RANGE(TVN_ITEMEXPANDED,0,0xffff,OnItemExpanded)
	ON_NOTIFY_EX_RANGE(TVN_SELCHANGING,0,0xffff,OnSelectionChanging)
	ON_NOTIFY_EX_RANGE(TVN_SELCHANGED,0,0xffff,OnSelectionChanged)
	ON_NOTIFY_EX_RANGE(LVN_ITEMCHANGING,0,0xffff,OnItemChanging)
END_MESSAGE_MAP()


BOOL COXDropDownContainer::AttachWindow(CWnd* pWnd)
{
	if(pWnd!=NULL && !::IsWindow(pWnd->GetSafeHwnd()))
	{
		TRACE(_T("COXDropDownContainer::AttachWindow: failed, invalid window has been specified as a parameter\n"));
		return FALSE;
	}

	if(m_pAttachedWnd!=NULL && ::IsWindow(m_pAttachedWnd->GetSafeHwnd()))
	{
		m_pAttachedWnd->ShowWindow(SW_HIDE);
	}

	m_pAttachedWnd=pWnd;

	if(m_pAttachedWnd!=NULL)
	{
		if(m_pAttachedWnd->GetParent()!=this)
		{
			m_pAttachedWnd->SetParent(this);
		}

		DWORD dwExStyleAdd=m_pAttachedWnd->GetExStyle() & 
			(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|WS_EX_WINDOWEDGE|WS_EX_DLGMODALFRAME);
		DWORD dwExStyleRemove=(GetExStyle() & 
			(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|WS_EX_WINDOWEDGE|WS_EX_DLGMODALFRAME)) &	
			~dwExStyleAdd;
		if(dwExStyleAdd!=0)
		{
			m_pAttachedWnd->ModifyStyleEx(dwExStyleAdd,0,SWP_DRAWFRAME|SWP_FRAMECHANGED);
			if(dwExStyleRemove!=dwExStyleAdd)
			{
				ModifyStyleEx(dwExStyleRemove,dwExStyleAdd,
					SWP_DRAWFRAME|SWP_FRAMECHANGED);
			}
		}

		CRect rect;
		GetClientRect(rect);
		m_pAttachedWnd->MoveWindow(rect);
	}

	return TRUE;
}


void COXDropDownContainer::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
    // move attached window
    if(m_pAttachedWnd!=NULL && ::IsWindow(m_pAttachedWnd->GetSafeHwnd()))
	{
		CRect rect;
		GetClientRect(&rect);
		m_pAttachedWnd->MoveWindow(rect);
    }
}



void COXDropDownContainer::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	// forward focus to child
	if(m_pAttachedWnd!=NULL)
    {
		ASSERT(::IsWindow(m_pAttachedWnd->GetSafeHwnd()));
		if(::IsWindow(m_pAttachedWnd->GetSafeHwnd()))
		{
	        m_pAttachedWnd->SetFocus();
		}
    }
}


LRESULT COXDropDownContainer::OnFloatStatus(WPARAM wParam, LPARAM)
{
	// these asserts make sure no conflicting actions are requested
	ASSERT(!((wParam & FS_SHOW) && (wParam & FS_HIDE)));
	ASSERT(!((wParam & FS_ENABLE) && (wParam & FS_DISABLE)));
	ASSERT(!((wParam & FS_ACTIVATE) && (wParam & FS_DEACTIVATE)));

	// FS_SYNCACTIVE is used to detect MFS_SYNCACTIVE windows
	LRESULT lResult=0;
	if((GetStyle() & MFS_SYNCACTIVE) && (wParam & FS_SYNCACTIVE))
	{
		lResult=1;
	}

	if(wParam & (FS_SHOW|FS_HIDE))
	{
		SetWindowPos(NULL,0,0,0,0,
			((wParam & FS_SHOW) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW)|
			SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
	}

	if(wParam&(FS_ENABLE|FS_DISABLE))
	{
		EnableWindow((wParam & FS_ENABLE)!=0);
	}

	if((wParam & (FS_ACTIVATE|FS_DEACTIVATE)) && (GetStyle() & MFS_SYNCACTIVE))
	{
		ModifyStyle(MFS_SYNCACTIVE,0);
		SendMessage(WM_NCACTIVATE,(wParam & FS_ACTIVATE)!=0);
		ModifyStyle(0,MFS_SYNCACTIVE);
	}

	return lResult;
}


BOOL COXDropDownContainer::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}


BOOL COXDropDownContainer::OnItemExpanding(UINT id, NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult=0;

	BOOL bReturn=FALSE;
	if(m_pAttachedWnd!=NULL && ::IsWindow(m_pAttachedWnd->GetSafeHwnd()) && 
		m_pAttachedWnd->GetDlgCtrlID()==(int)id)
	{
		bReturn=(BOOL)m_pAttachedWnd->SendMessage(
			WM_OXTREEDROPDOWN_ITEM_EXPANDING,(WPARAM)pResult,(LPARAM)pNMHDR);
	}

    return bReturn;   
}


BOOL COXDropDownContainer::OnItemExpanded(UINT id, NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult=0;

	BOOL bReturn=FALSE;
	if(m_pAttachedWnd!=NULL && ::IsWindow(m_pAttachedWnd->GetSafeHwnd()) && 
		m_pAttachedWnd->GetDlgCtrlID()==(int)id)
	{
		bReturn=(BOOL)m_pAttachedWnd->SendMessage(
			WM_OXTREEDROPDOWN_ITEM_EXPANDED,(WPARAM)pResult,(LPARAM)pNMHDR);
	}

    return bReturn;   
}


BOOL COXDropDownContainer::OnSelectionChanging(UINT id, NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult=0;

	BOOL bReturn=FALSE;
	if(m_pAttachedWnd!=NULL && ::IsWindow(m_pAttachedWnd->GetSafeHwnd()) && 
		m_pAttachedWnd->GetDlgCtrlID()==(int)id)
	{
		bReturn=(BOOL)m_pAttachedWnd->SendMessage(
			WM_OXTREEDROPDOWN_SELECTION_CHANGING,(WPARAM)pResult,(LPARAM)pNMHDR);
	}

    return bReturn;   
}


BOOL COXDropDownContainer::OnSelectionChanged(UINT id, NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult=0;

	BOOL bReturn=FALSE;
	if(m_pAttachedWnd!=NULL && ::IsWindow(m_pAttachedWnd->GetSafeHwnd()) && 
		m_pAttachedWnd->GetDlgCtrlID()==(int)id)
	{
		bReturn=(BOOL)m_pAttachedWnd->SendMessage(
			WM_OXTREEDROPDOWN_SELECTION_CHANGED,(WPARAM)pResult,(LPARAM)pNMHDR);
	}

    return bReturn;   
}


BOOL COXDropDownContainer::OnItemChanging(UINT id, NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult=0;

	BOOL bReturn=FALSE;
	if(m_pAttachedWnd!=NULL && ::IsWindow(m_pAttachedWnd->GetSafeHwnd()) && 
		m_pAttachedWnd->GetDlgCtrlID()==(int)id)
	{
		bReturn=(BOOL)m_pAttachedWnd->SendMessage(
			WM_OXLISTDROPDOWN_ITEM_CHANGING,(WPARAM)pResult,(LPARAM)pNMHDR);
	}

    return bReturn;   
}


///////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(COXComboPickerCtrl, CComboBox);

COXComboPickerCtrl::COXComboPickerCtrl(COXComboDropDown* pDropDown/*=NULL*/) 
	: m_pDropDown(pDropDown)
{
	if(m_pDropDown!=NULL)
	{
		m_pDropDown->OnAttachToComboPicker();
	}
}


COXComboPickerCtrl::~COXComboPickerCtrl()
{
	if(m_pDropDown!=NULL)
	{
		if(m_pDropDown->GetWindow()!=NULL && 
			::IsWindow(m_pDropDown->GetWindow()->GetSafeHwnd()) && GetDroppedState())
		{
			ShowDropDown(FALSE);
		}
		m_pDropDown->SetOwnerCombo(NULL);
	}
}


BEGIN_MESSAGE_MAP(COXComboPickerCtrl, CComboBox)
	//{{AFX_MSG_MAP(COXComboPickerCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void COXComboPickerCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	_AFX_THREAD_STATE* pThreadState=AfxGetThreadState();

	// hook not already in progress
	if(pThreadState->m_pWndInit==NULL)
	{
		DWORD dwStyle=GetStyle();
		// must be ownerdraw
		// must be dropdown list
		if(!(dwStyle & CBS_DROPDOWNLIST) || 
			!((dwStyle & CBS_OWNERDRAWFIXED) || (dwStyle & CBS_OWNERDRAWVARIABLE)))
		{
			ASSERT(FALSE);
			TRACE(_T("COXComboPickerCtrl::PreSubclassWindow: control must have CBS_OWNERDRAWFIXED or CBS_OWNERDRAWVARIABLE and CBS_DROPDOWNLIST styles\n"));
		}
	}

	CComboBox::PreSubclassWindow();
}


BOOL COXComboPickerCtrl::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN && GetDroppedState())
	{
		if(pMsg->wParam==VK_RETURN || 
			pMsg->wParam==VK_ESCAPE || 
			pMsg->wParam==VK_CANCEL)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
	}

	return CComboBox::PreTranslateMessage(pMsg);
}


LRESULT COXComboPickerCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	LRESULT lResult=0;
	
	switch(message)
	{
	case CB_GETDROPPEDSTATE:
		lResult=GetDroppedState();
		break;

	case CB_SHOWDROPDOWN:
		ShowDropDown((BOOL)wParam);
		break;

	default:
		lResult=CComboBox::WindowProc(message,wParam,lParam);
		break;
	}

	return lResult;
}


int COXComboPickerCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(!(lpCreateStruct->style & CBS_DROPDOWNLIST) || 
		!((lpCreateStruct->style & CBS_OWNERDRAWFIXED) || 
		(lpCreateStruct->style & CBS_OWNERDRAWVARIABLE)))
	{
		ASSERT(FALSE);
		TRACE(_T("COXComboPickerCtrl::OnCreate: control must have CBS_OWNERDRAWFIXED or CBS_OWNERDRAWVARIABLE and CBS_DROPDOWNLIST styles\n"));
		return -1;
	}

	return CComboBox::OnCreate(lpCreateStruct);
}


void COXComboPickerCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_pDropDown!=NULL)
	{
		if(!GetDroppedState())
		{
			ShowDropDown(TRUE);
		}
	}
	else
	{
		CComboBox::OnLButtonDown(nFlags,point);
	}
}


void COXComboPickerCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if(m_pDropDown!=NULL)
	{
		if(!GetDroppedState())
		{
			ShowDropDown(TRUE);
		}
	}
	else
	{
		CComboBox::OnLButtonDblClk(nFlags,point);
	}
}


void COXComboPickerCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_pDropDown==NULL)
	{
		CComboBox::OnLButtonUp(nFlags,point);
	}
}
    
     
void COXComboPickerCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(m_pDropDown!=NULL)
	{
		if(!GetDroppedState() && (nChar==VK_UP || nChar==VK_DOWN || nChar==VK_RETURN))
		{
			ShowDropDown(TRUE);
			return;
		}

		if(GetDroppedState())
		{
			CWnd* pDropdownCtrl=m_pDropDown->GetContainer()->GetAttachedWindow();
			ASSERT(::IsWindow(pDropdownCtrl->GetSafeHwnd()));
			if(::IsWindow(pDropdownCtrl->GetSafeHwnd()))
			{
				pDropdownCtrl->SendMessage(WM_KEYDOWN,nChar,MAKELONG(nRepCnt,nFlags));
				return;
			}
		}
	}

	CComboBox::OnKeyDown(nChar,nRepCnt,nFlags);
}


void COXComboPickerCtrl::OnKillFocus(CWnd* pNewWnd)
{
	if(pNewWnd!=this && m_pDropDown!=NULL && pNewWnd!=m_pDropDown->GetContainer() && 
		pNewWnd!=m_pDropDown->GetContainer()->GetAttachedWindow())
	{
		if(GetDroppedState())
		{
			ShowDropDown(FALSE);
		}
	}

	CComboBox::OnKillFocus(pNewWnd);
}


void COXComboPickerCtrl::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CComboBox::OnWindowPosChanged(lpwndpos);

	if((lpwndpos->flags & SWP_NOSIZE)==0 && m_pDropDown!=NULL)
	{
		m_pDropDown->OnChangeOwnerComboSize();
	}
}


void COXComboPickerCtrl::OnPaint() 
{
	// Detect OS. If the OS is XP or higher we use the OS painting routines,
	// else we do custom painting.
	OSVERSIONINFO ovi = {0};
	ovi.dwOSVersionInfoSize = sizeof OSVERSIONINFO;
	GetVersionEx(&ovi);
	bool bXPOrHigher = (ovi.dwMajorVersion >= 5) && (ovi.dwMinorVersion >= 1);

	if( (m_pDropDown==NULL) || bXPOrHigher )
	{
		Default();
		return;
	}

    CPaintDC dcPaint(this);

	BOOL bIsEnabled=IsWindowEnabled();

	CRect rectClient;
    GetClientRect(rectClient);

	// draw dropdown button
	CRect rectDropdownButton=GetDropdownButtonRect();
	dcPaint.DrawFrameControl(rectDropdownButton,DFC_SCROLL,
		DFCS_SCROLLDOWN | (GetDroppedState() ? DFCS_PUSHED|DFCS_FLAT : 0) | 
		(bIsEnabled ? 0 : DFCS_INACTIVE));

	// draw border
	dcPaint.Draw3dRect(rectClient,::GetSysColor(COLOR_3DSHADOW),
		::GetSysColor(COLOR_3DHILIGHT));
	rectClient.DeflateRect(1,1);
	dcPaint.Draw3dRect(rectClient,::GetSysColor(COLOR_3DDKSHADOW),
		::GetSysColor(COLOR_3DFACE));
	rectClient.DeflateRect(1,1);

	// adjust for dropdown button
	rectClient.right=rectDropdownButton.left;
	rectClient.DeflateRect(1,1);
    // do default painting
    DRAWITEMSTRUCT dis={ sizeof(&dis) };
    dis.CtlType=ODT_COMBOBOX;
    dis.CtlID=GetDlgCtrlID();
    dis.itemID=0;
    dis.itemAction=ODA_DRAWENTIRE;
    dis.hwndItem=GetSafeHwnd();
    dis.hDC=dcPaint.GetSafeHdc();
    dis.rcItem=rectClient;
    dis.itemData=0;
    dis.itemState=((::GetFocus()==GetSafeHwnd() && !GetDroppedState()) ? ODS_FOCUS : 0);
    DrawItem(&dis);
}


void COXComboPickerCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct->CtlType==ODT_COMBOBOX);
	ASSERT((lpDrawItemStruct->CtlType & ODS_COMBOBOXEDIT)==0);

	BOOL bHasFocus=(GetFocus()==this && !GetDroppedState());
	CDC* pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	ASSERT(pDC!=NULL);
	int nSavedDC=pDC->SaveDC();

	CRect rectItem=lpDrawItemStruct->rcItem;
	// adjustment for focus rectangle
	rectItem.InflateRect(1,1);
	// draw background
	pDC->FillSolidRect(rectItem,(IsWindowEnabled() ? 
		::GetSysColor(COLOR_WINDOW) : ::GetSysColor(COLOR_BTNFACE)));

	rectItem.DeflateRect(1,1);
	if(m_pDropDown==NULL || !m_pDropDown->IsThereSelectedItem())
	{
		return;
	}

	// draw image
	//
	CImageList* pImageList=m_pDropDown->GetSelectedItemImage();
	if(pImageList!=NULL)
	{
		ASSERT(pImageList->GetImageCount()>0);
		if(pImageList->GetImageCount()>0)
		{
			CRect rectImage=rectItem;
			IMAGEINFO imageInfo;
			VERIFY(pImageList->GetImageInfo(0,&imageInfo));
			CRect rectImageSize=imageInfo.rcImage;
			if(rectImageSize.Height()<rectImage.Height())
			{
				rectImage.top+=(rectImage.Height()-rectImageSize.Height())/2;
				rectImage.bottom=rectImage.top+rectImageSize.Height();
			}

			rectImage.right=rectImage.left+rectImageSize.Width();

			pImageList->Draw(pDC,0,rectImage.TopLeft(),
				ILD_TRANSPARENT/*|(bHasFocus ? ILD_FOCUS : 0)*/);

			// update rectItem
			rectItem.left=rectImage.right+1;
		}

		// we are responsible for deleting the image list object
		delete pImageList;
	}
	//
	////////////////////////////////////////

	// draw text
	//
	CString sText=m_pDropDown->GetSelectedItemText();
	if(!sText.IsEmpty())
	{
		// set colors
		if(bHasFocus)
		{
			pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
			pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		}
		else
		{
			pDC->SetTextColor(m_pDropDown->GetSelectedItemTextColor());
		}

		// set font
		CFont* pFont=m_pDropDown->GetSelectedItemFont();
		if(pFont!=NULL)
		{
			pDC->SelectObject(pFont);
		}
		
		CRect rectText=rectItem;
		rectText.left++;
		CRect rectTextSize=rectText;
		pDC->DrawText(sText,rectTextSize,DT_LEFT|DT_CALCRECT|DT_SINGLELINE);
		if(rectTextSize.Height()<rectText.Height())
		{
			rectText.top+=(rectText.Height()-rectTextSize.Height())/2;
			rectText.bottom=rectText.top+rectTextSize.Height();
		}
		if(rectText.Width()>rectTextSize.Width())
		{
			rectText.right=rectText.left+rectTextSize.Width();
		}

		if(bHasFocus)
		{
			CRect rectTextFocus=rectText;
			rectTextFocus.InflateRect(1,1);
			pDC->DrawFocusRect(rectTextFocus);
		}

		pDC->DrawText(sText,rectText,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}
	//
	////////////////////////////////////////

	if(nSavedDC!=0)
	{
		pDC->RestoreDC(nSavedDC);
	}
}


void COXComboPickerCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if(m_pDropDown!=NULL)
	{
		lpMeasureItemStruct->itemHeight=m_pDropDown->GetItemHeight();
	}
	else
	{
		lpMeasureItemStruct->itemHeight=OXCOMBOPICKER_DEFAULTITEMHEIGHT;
	}
}


BOOL COXComboPickerCtrl::AttachDropDown(COXComboDropDown* pDropDown)
{
	if(m_pDropDown!=NULL && m_pDropDown->GetDroppedState())
	{
		m_pDropDown->ShowDropDown(FALSE);
	}

	m_pDropDown=pDropDown;

	if(m_pDropDown!=NULL)
	{
		m_pDropDown->OnAttachToComboPicker();
	}

	return TRUE;
}


void COXComboPickerCtrl::ShowDropDown(BOOL bShowIt/*=TRUE*/)
{
	if(m_pDropDown==NULL || GetDroppedState()==bShowIt)
	{
		return;
	}

	COXDropDownContainer* pDropdownCtrl=m_pDropDown->GetContainer();
	ASSERT(::IsWindow(pDropdownCtrl->GetSafeHwnd()));
	ASSERT(pDropdownCtrl->GetAttachedWindow()==m_pDropDown->GetWindow());

	if(bShowIt)
	{
		ASSERT(::IsWindow(GetSafeHwnd()));

		// move dropdown control
		CRect rect;
		GetWindowRect(rect);
		pDropdownCtrl->SetWindowPos(NULL,rect.left,rect.bottom-1,0,0,
			SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);

		m_pDropDown->ShowDropDown(bShowIt);

		// emulate CBN_DROPDOWN notification
		CWnd* pParentWnd=GetParent();
		if(pParentWnd!=NULL)
		{
			pParentWnd->SendMessage(
				WM_COMMAND,MAKEWPARAM(CBN_DROPDOWN,GetDlgCtrlID()));
		}

		// bring dropdown control to the top
		::SetWindowPos(pDropdownCtrl->GetSafeHwnd(),
			HWND_TOPMOST,0,0,0,0,SWP_NOREDRAW|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);

		pDropdownCtrl->ShowWindow(SW_SHOWNA);
		pDropdownCtrl->GetAttachedWindow()->ShowWindow(SW_SHOWNA);
		if(m_pDropDown->NeedFocus())
		{
			pDropdownCtrl->GetAttachedWindow()->SetFocus();
		}

		// redraw the contents
		RedrawWindow();
	}
	else
	{
		if(::IsWindow(GetSafeHwnd()))
		{
			// emulate CBN_CLOSEUP notification
			CWnd* pParentWnd=GetParent();
			if(pParentWnd!=NULL)
			{
				pParentWnd->SendMessage(
					WM_COMMAND,MAKEWPARAM(CBN_CLOSEUP,GetDlgCtrlID()));
			}
		}
		
		m_pDropDown->ShowDropDown(bShowIt);

		// hide dropdown control
		pDropdownCtrl->ShowWindow(SW_HIDE);

		if(::IsWindow(GetSafeHwnd()))
		{
			// redraw the contents
			RedrawWindow();
		}
	}
}


BOOL COXComboPickerCtrl::GetDroppedState() const
{
	if(m_pDropDown!=NULL)
	{
		return m_pDropDown->GetDroppedState();
	}

	return FALSE;
}


CRect COXComboPickerCtrl::GetDropdownButtonRect() const 
{
    CRect rect;
    GetClientRect(&rect);
	rect.DeflateRect(2,2);
	rect.left=rect.right-::GetSystemMetrics(SM_CXVSCROLL);
    return rect;
}


BOOL COXComboPickerCtrl::SetMaxDropdownHeight(int nHeight)
{
	if(m_pDropDown!=NULL)
	{
		CSize sizeMaxWidthHeight=m_pDropDown->GetMaxWidthHeight();
		sizeMaxWidthHeight.cy=nHeight;
		m_pDropDown->SetMaxWidthHeight(sizeMaxWidthHeight);

		return TRUE;
	}

	return FALSE;
}


BOOL COXComboPickerCtrl::SetMinDropdownHeight(int nHeight)
{
	if(m_pDropDown!=NULL)
	{
		CSize sizeMinWidthHeight=m_pDropDown->GetMinWidthHeight();
		sizeMinWidthHeight.cy=nHeight;
		m_pDropDown->SetMinWidthHeight(sizeMinWidthHeight);

		return TRUE;
	}

	return FALSE;
}

