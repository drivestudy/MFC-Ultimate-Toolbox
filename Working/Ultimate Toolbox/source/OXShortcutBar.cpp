// ====================================================================================
//								Class Implementation : 
//	COXSHBDropSource & COXSHBDropTarget & COXSHBEdit & COXSHBListCtrl & COXShortcutBar
// ====================================================================================

// Source file : OXShortcutBar.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
//////////////////////////////////////////////////////////////////////////////
                         
#include "stdafx.h"
#include <windowsx.h>
#include "OXShortcutBar.h"
#include "OXSkins.h"

#include "UTBStrOp.h"

#pragma warning(disable : 4100 4786 4345)
#include <functional>
#include <set>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct LVITEM_less : public std::binary_function<LVITEM*, LVITEM*, bool>
{
    bool operator()(const LVITEM* pItem1, const LVITEM* pItem2) const
	{
		CString strItem1(pItem1->pszText);
		CString strItem2(pItem2->pszText);

		return (strItem1 < strItem2);
	}
};

// format for drag'n'drop item
CLIPFORMAT COXShortcutBar::m_nChildWndItemFormat=
	(CLIPFORMAT)::RegisterClipboardFormat(_T("Shortcut bar child window item"));

/////////////////////////////////////////////////////////////////////////////
// COXSHBEdit

COXSHBEdit::COXSHBEdit()
{
	m_pSHBED=NULL;
	// by default use standard window background color
	COLORREF clrBackground=::GetSysColor(COLOR_WINDOW);
	// create brush used to fill background
	VERIFY(m_brush.CreateSolidBrush(clrBackground));
	m_bIsEditing=FALSE;
}

COXSHBEdit::~COXSHBEdit()
{
	delete m_pSHBED;
}


BEGIN_MESSAGE_MAP(COXSHBEdit, CEdit)
	//{{AFX_MSG_MAP(COXSHBEdit)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXSHBEdit message handlers

LRESULT COXSHBEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL bFinishEdit=FALSE;

	// we finish editing if user click any mouse button or press Enter or ESC key
	switch(message)	
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:	
	case WM_MOUSEMOVE:	
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
		{
			// check if mouse cursor point is over the control's window
			CRect rectClient;
			GetClientRect(rectClient);
			POINTS points=MAKEPOINTS(lParam);	
			CPoint point(points.x,points.y);
			if(!rectClient.PtInRect(point))
			{
				::SetCursor(LoadCursor(NULL, IDC_ARROW));
				// if the mouse cursor is over some other window then we should forward the 
				// message to this window and if the mouse message is other than WM_MOUSEMOVE
				// then we should finish the editing
				bFinishEdit=(message==WM_MOUSEMOVE) ? FALSE : TRUE;

				// find target window
				ClientToScreen(&point);
				CWnd* pWnd=WindowFromPoint(point);
				ASSERT_VALID(pWnd);
				ASSERT(::IsWindow(pWnd->GetSafeHwnd()));
				ASSERT(pWnd!=this); 

				// clarify type of message that should be sent: WM_NC* and WM_* messages
				int nHitTest=(int)pWnd->SendMessage(
					WM_NCHITTEST,0,MAKELONG(point.x,point.y));	
				if(nHitTest==HTCLIENT) 
				{
					pWnd->ScreenToClient(&point);		
					lParam=MAKELONG(point.x,point.y);
				} 
				else 
				{
					// convert the messages in their WM_NC* counterparts
					switch(message) 
					{
					case WM_LBUTTONDOWN: 
						{
							message=WM_NCLBUTTONDOWN;		
							break;
						}
					case WM_RBUTTONDOWN: 
						{
							message=WM_NCRBUTTONDOWN;		
							break;
						}
					case WM_MBUTTONDOWN: 
						{
							message=WM_NCMBUTTONDOWN;		
							break;
						}
					case WM_LBUTTONDBLCLK: 
						{
							message=WM_NCLBUTTONDBLCLK;		
							break;
						}
					case WM_RBUTTONDBLCLK: 
						{
							message=WM_NCRBUTTONDBLCLK;		
							break;
						}
					case WM_MBUTTONDBLCLK: 
						{
							message=WM_NCMBUTTONDBLCLK;		
							break;
						}
					case WM_MOUSEMOVE: 
						{
							message=WM_NCMOUSEMOVE;		
							break;
						}
					}
					lParam=MAKELONG(point.x,point.y);
				}
				wParam=nHitTest;
		
				// finish editing if needed
				if(bFinishEdit)
				{
					FinishEdit(TRUE);
				}

				// forward message to underlaying window
				pWnd->SendMessage(message,wParam,lParam);

				// change cursor if needed
				pWnd->SendMessage(WM_SETCURSOR,(WPARAM)pWnd->GetSafeHwnd(),
					(LPARAM)MAKELONG(nHitTest,message));

/*				// if we haven't finished editing and we've lost capture then
				// recapture mouse messages
				if(m_bIsEditing && ::IsWindow(GetSafeHwnd()) && 
					::GetCapture()!=GetSafeHwnd())
				{
					SetCapture();
				}*/
			}
			else
			{
				::SetCursor(LoadCursor(NULL, IDC_IBEAM));
			}

			break;
		}

	case WM_KEYDOWN:
		{
			// successfully finish editing if Enter key was pressed
			if(wParam == VK_RETURN)
			{
				bFinishEdit=TRUE;
				FinishEdit(TRUE);
			}
			// cancel editing if ESC key was pressed
			else if(wParam == VK_ESCAPE)
			{
				bFinishEdit=TRUE;
				FinishEdit(FALSE);
			}
			break;
		}
	}	

	// if we've finished editing then there is no sense to handle message
	if(bFinishEdit)
	{
		return TRUE;	
	}

	LRESULT lResult=CEdit::WindowProc(message,wParam,lParam);

	// restore capture if we lost it as a result of 
	// processing the current message
	if(m_bIsEditing && ::IsWindow(GetSafeHwnd()) && ::GetCapture()!=GetSafeHwnd())
	{
		SetCapture();
	}

	return lResult;
}

void COXSHBEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here

	// finish editing if we haven't done it previously
	if(::GetCapture()==GetSafeHwnd())
	{
		FinishEdit(TRUE);	
	}
}

void COXSHBEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here

	// capture mouse messages
	SetCapture();
}

void COXSHBEdit::OnUpdate() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here

	// here is the place where we enlarge edit control window if needed
	//
	if(m_pSHBED && (m_pSHBED->nMask&SHBEIF_ENLARGE)!=0 && 
		(m_pSHBED->nMask&SHBEIF_RECTMAX)!=0 && m_pSHBED->bEnlargeAsTyping)
	{
		// get window rect
		CRect rectWindow;
		GetWindowRect(rectWindow);

		// get typed text
		CString sText;
		GetWindowText(sText);

		// get edit style to take into account text allignment while recalculating 
		// control's window coordinates
		DWORD dwStyle=GetStyle();

		// define min rect that edit control window can be (if min rect is not specified 
		// explicitly then we use as min the rect of window that was set in StartEdit
		// function)
		CRect rectMin;
		if((m_pSHBED->nMask&SHBEIF_RECTMIN)!=0)
		{
			rectMin=m_pSHBED->rectMin;
		}
		else
		{
			rectMin=m_pSHBED->rectDisplay;
		}
		// min rect mustn't be empty
		ASSERT(!rectMin.IsRectEmpty());

		if(!sText.IsEmpty())
		{
			// window rect mustn't be more than max rect
			ASSERT(rectWindow.Width()<=m_pSHBED->rectMax.Width() && 
				rectWindow.Height()<=m_pSHBED->rectMax.Height() && 
				rectWindow.Width()>=rectMin.Width() && 
				rectWindow.Height()>=rectMin.Height());

			// get the rect used to display text
			CRect rectDisplay;
			GetRect(rectDisplay);
			ClientToScreen(&rectDisplay);
			// get margins between display and window rects to keep them in 
			// enlarged/shrinked window
			CRect rectMargins(rectDisplay.left-rectWindow.left,
				rectWindow.right-rectDisplay.right,rectDisplay.top-rectWindow.top,
				rectWindow.bottom-rectDisplay.bottom);
	
			// set the font used to draw text
			CClientDC dc(this);
			CFont* pOldFont=NULL;
			if((HFONT)m_font)
			{
				pOldFont=dc.SelectObject(&m_font);
			}

			// calculate the size of updated window text constrained by the size 
			// of max rect
			CRect rectMax=m_pSHBED->rectMax;
			rectMax-=rectMax.TopLeft();
			CRect rectText=rectMax;
			rectText.right-=(rectMargins.left+rectMargins.right);
			rectText.bottom-=(rectMargins.top+rectMargins.bottom);
			// calculate
			dc.DrawText(sText,rectText,DT_CALCRECT|DT_VCENTER|DT_CENTER|
				((dwStyle&ES_MULTILINE)!=0 ? DT_WORDBREAK|DT_EDITCONTROL : 0));
			// take into account original margins
			rectText.right+=(rectMargins.left+rectMargins.right);
			rectText.bottom+=(rectMargins.top+rectMargins.bottom);
			// don't forget about max rect 
			VERIFY(rectText.IntersectRect(rectMax,rectText));

			// make sure text rect is no less than min rect
			//
			if(rectText.Width()<rectMin.Width())
			{
				rectText.right=rectText.left+rectMin.Width();
			}
			if(rectText.Height()<rectMin.Height())
			{
				rectText.bottom=rectText.top+rectMin.Height();
			}

			// calculate enlarged/shrinked edit control window rect taking into 
			// account text alignment
			//
			CRect rectEnlarged=m_pSHBED->rectMax;

			// width
			int nXMargin=rectMax.Width()-rectText.Width();
			ASSERT(nXMargin>=0);
			if(nXMargin>0)
			{
				if((dwStyle&ES_RIGHT)!=0)
				{
					rectEnlarged.left+=nXMargin;
				}
				else if((dwStyle&ES_CENTER)!=0)
				{
					rectEnlarged.right-=nXMargin/2;
					rectEnlarged.left=rectEnlarged.right-rectText.Width();
				}
				else
				{
					//ES_LEFT
					rectEnlarged.right-=nXMargin;
				}
			}

			// height
			int nYMargin=rectMax.Height()-rectText.Height();
			ASSERT(nYMargin>=0);
			if(nYMargin>=0)
			{
				rectEnlarged.bottom-=nYMargin;
			}

			// resize it!
			MoveWindow(rectEnlarged);

			if(pOldFont)
			{
				dc.SelectObject(pOldFont);
			}
		}
		else
		{
			// resize it!
			MoveWindow(rectMin);
		}

		// get client rect
		CRect rectClient;
		GetClientRect(rectClient);
		// set the rect used to display text
		SetRectNP(rectClient);
	}
}


HBRUSH COXSHBEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	UNREFERENCED_PARAMETER(nCtlColor);

	ASSERT(nCtlColor==CTLCOLOR_EDIT);
	
	// fill the background
	COLORREF clrBackground=::GetSysColor(COLOR_WINDOW);
	if((m_pSHBED->nMask&SHBEIF_CLRBACK))
	{
		clrBackground=m_pSHBED->clrBack;
	}
	pDC->SetBkColor(clrBackground);

	if((m_pSHBED->nMask&SHBEIF_CLRTEXT))
	{
		pDC->SetTextColor(m_pSHBED->clrText);
	}

	// use our brush to fill the part of the control that is not covered by text
	return (HBRUSH)m_brush;
	// TODO: Return a non-NULL brush if the parent's handler should not be called
}

BOOL COXSHBEdit::StartEdit(LPSHBE_DISPLAY pSHBED)
{
	// window have to be created at this moment
	ASSERT(::IsWindow(m_hWnd));

	// verify SHBE_DISPLAY structure
	if(pSHBED && !VerifyDisplayInfo(pSHBED))
	{
		TRACE(_T("COXSHBEdit::StartEdit: LPSHBE_DISPLAY is invalid"));
		return FALSE;
	}

	// save style, text, ctrlID, parent window and size
	DWORD dwStyle=GetStyle();
	CString sText;
	GetWindowText(sText);
	CRect rectWindow;
	GetWindowRect(rectWindow);
	UINT nCtrlID=GetDlgCtrlID();
	CWnd* pWnd=GetParent();
	ASSERT(pWnd);

	// if pSHBED is not NULL then get new attributes
	if(pSHBED)
	{
		// save SHBE_DISPLAY structure
		if(m_pSHBED==NULL)
		{
			m_pSHBED=new SHBE_DISPLAY;
		}
		m_pSHBED->Copy(pSHBED);

		// get new window text
		if((m_pSHBED->nMask&SHBEIF_TEXT)!=0)
		{
			sText=m_pSHBED->lpText;
		}

		// get new window rect
		if((m_pSHBED->nMask&SHBEIF_RECTDISPLAY)!=0)
		{
			rectWindow=m_pSHBED->rectDisplay;
		}

		// get new edit control style
		if((m_pSHBED->nMask&SHBEIF_STYLE)!=0)
		{
			dwStyle=m_pSHBED->dwStyle;
		}

		// get font to be used to draw text
		if((m_pSHBED->nMask&SHBEIF_FONT)!=0)
		{
			if((HFONT)m_font)
			{
				m_font.DeleteObject();
			}
			if(m_pSHBED->pFont)
			{
				LOGFONT lf;
				if(m_pSHBED->pFont->GetLogFont(&lf)!=0)
				{
					VERIFY(m_font.CreateFontIndirect(&lf));
				}
			}
		}

		// adjust window to be compliant with min rect
		if((m_pSHBED->nMask&SHBEIF_RECTMIN)!=0)
		{
			rectWindow.left=__min(m_pSHBED->rectMin.left,rectWindow.left);
			rectWindow.top=__min(m_pSHBED->rectMin.top,rectWindow.top);
			rectWindow.right=__max(m_pSHBED->rectMin.right,rectWindow.right);
			rectWindow.bottom=__max(m_pSHBED->rectMin.bottom,rectWindow.bottom);
		}

	}
	else
	{
		delete m_pSHBED;
	}

	// destroy the edit control
	if(DestroyWindow()==0)
	{
		TRACE(_T("COXSHBEdit::StartEdit: failed to destroy window"));
		return FALSE;
	}

	// recreate it using saved and updated properties
	if(!Create(dwStyle,rectWindow,pWnd,nCtrlID))
	{
		TRACE(_T("COXSHBEdit::StartEdit: failed to create window"));
		return FALSE;
	}

	// set new font to window
	if((HFONT)m_font)
	{
		SetFont(&m_font,TRUE);
	}

	// change the text of window
	SetWindowText(sText);

	// set background brush
	if((m_pSHBED->nMask&SHBEIF_CLRBACK))
	{
		COLORREF clrBackground=m_pSHBED->clrBack;
		if((HBRUSH)m_brush!=NULL)
		{
			m_brush.DeleteObject();
		}
		VERIFY(m_brush.CreateSolidBrush(clrBackground));
	}

	// set selection
	if(m_pSHBED && (m_pSHBED->nMask&SHBEIF_SELRANGE)!=0)
	{
		SetSel(m_pSHBED->ptSelRange.x,m_pSHBED->ptSelRange.y,TRUE);
	}

	// get client rect
	CRect rectClient;
	GetClientRect(rectClient);
	// set the rect used to display text
	SetRect(rectClient);

	// show edit control and set the focus to it
	m_bIsEditing=TRUE;
	ShowWindow(SW_SHOW);
	SetFocus();

	return TRUE;
}

void COXSHBEdit::FinishEdit(BOOL bOK)
{
	// window should be created at this moment
	ASSERT(::IsWindow(GetSafeHwnd()));

	m_bIsEditing=FALSE;

	// release capture if we've still got it
	if(GetCapture()==this)
	{
		ReleaseCapture();
	}

	// Send Notification to parent 
	NMSHBEDIT nmshbe;
	nmshbe.hdr.code=SHBEN_FINISHEDIT;
	nmshbe.bOK=bOK;
	SendSHBENotification(&nmshbe);

	// hide the window
	ShowWindow(SW_HIDE);
}

// helper function to send edit control notifications
LRESULT COXSHBEdit::SendSHBENotification(LPNMSHBEDIT pNMSHBE)
{
	// notify parent
	CWnd* pParentWnd=GetParent();
	VERIFY(pParentWnd);

	// fill notification structure
	pNMSHBE->hdr.hwndFrom=GetSafeHwnd();
	pNMSHBE->hdr.idFrom=GetDlgCtrlID();

	// send the notification message
	return (pParentWnd->SendMessage(
		WM_NOTIFY,(WPARAM)GetDlgCtrlID(),(LPARAM)pNMSHBE));
}

BOOL COXSHBEdit::VerifyDisplayInfo(LPSHBE_DISPLAY pSHBED)
{
	ASSERT(pSHBED);

	// verify mask 
	if((pSHBED->nMask&~(SHBEIF_TEXT|SHBEIF_RECTDISPLAY|SHBEIF_RECTMAX|
		SHBEIF_STYLE|SHBEIF_ENLARGE|SHBEIF_SELRANGE|SHBEIF_FONT|SHBEIF_RECTMIN|
		SHBEIF_CLRTEXT|SHBEIF_CLRBACK))!=0)
	{
		TRACE(_T("COXSHBEdit::VerifyDisplayInfo: unspecified mask used"));
		return FALSE;
	}

	// verify if max rect have to be specified
	if((pSHBED->nMask&SHBEIF_ENLARGE)!=0 && (pSHBED->nMask&SHBEIF_RECTMAX)==0)
	{
		TRACE(_T("COXSHBEdit::VerifyDisplayInfo: enlargement as typing is specified while max rect is not"));
		return FALSE;
	}

	// verify selection if any set
	if((pSHBED->nMask&SHBEIF_SELRANGE)!=0 && !(pSHBED->ptSelRange.x==0 && 
		pSHBED->ptSelRange.y==-1) && !(pSHBED->ptSelRange.x==-1))
	{
		CString sText;
		if((pSHBED->nMask&SHBEIF_TEXT)!=0)
		{
			sText=pSHBED->lpText;
		}
		else
		{
			GetWindowText(sText);
		}

		int nLength=sText.GetLength();
		if(pSHBED->ptSelRange.x>=nLength || pSHBED->ptSelRange.y>=nLength)
		{
			TRACE(_T("COXSHBEdit::VerifyDisplayInfo: invalid selection range"));
			return FALSE;
		}
	}

	// verify if rectDisplay is not empty
	CRect rectDisplay;
	GetClientRect(rectDisplay);
	if(((pSHBED->nMask&SHBEIF_RECTDISPLAY)!=0 && pSHBED->rectDisplay.IsRectEmpty()) || 
		((pSHBED->nMask&SHBEIF_RECTDISPLAY)==0 && rectDisplay.IsRectEmpty()))
	{
		TRACE(_T("COXSHBEdit::VerifyDisplayInfo: empty display rect specified"));
		return FALSE;
	}
	else
	{
		rectDisplay=pSHBED->rectDisplay;
	}

	// verify if display rect is within max rect
	CRect rectIntersect;
	if((pSHBED->nMask&SHBEIF_RECTMAX)!=0)
	{
		rectIntersect.IntersectRect(pSHBED->rectMax,rectDisplay);
		if(rectIntersect!=rectDisplay)
		{
			TRACE(_T("COXSHBEdit::VerifyDisplayInfo: display rect is bigger than specified max rect"));
			return FALSE;
		}
	}

	// verify if min rect is within max rect
	if((pSHBED->nMask&SHBEIF_RECTMIN)!=0 && (pSHBED->nMask&SHBEIF_RECTMAX)!=0) 
	{
		rectIntersect.IntersectRect(pSHBED->rectMax,pSHBED->rectMin);
		if(rectIntersect!=pSHBED->rectMin)
		{
			TRACE(_T("COXSHBEdit::VerifyDisplayInfo: max rect doesn't include min rect"));
			return FALSE;
		}
	}

	// verify if min rect is not empty
	if((pSHBED->nMask&SHBEIF_RECTMIN)!=0 && (pSHBED->rectMin.IsRectEmpty())) 
	{
		TRACE(_T("COXSHBEdit::VerifyDisplayInfo: min rect mustn't be empty"));
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// COXSHBListCtrl

DWORD COXSHBListCtrl::m_dwComCtlVersion=0;
const int IDT_OXSHBLIST_CHECKMOUSE=275;
const int ID_OXSHBLIST_CHECKMOUSE_DELAY=200;

IMPLEMENT_DYNCREATE(COXSHBListCtrl, CListCtrl)

COXSHBListCtrl::COXSHBListCtrl()
{
	m_pShortcutBar=NULL;
	m_hGroup=NULL;

	m_nSelectedItem=-1;
	m_nHotItem=-1;
	m_nLastHotItem=-1;
	m_nActiveItem=-1;
	m_nDropHilightItem=-1;
	m_nDragItem=-1;
	m_nEditItem=-1;

	m_rectTopScrollButton.SetRectEmpty();
	m_rectBottomScrollButton.SetRectEmpty();

	m_ptOrigin=CPoint(0,0);

	m_bMouseIsOver=FALSE;
	m_bScrollingUp=FALSE;
	m_bScrollingDown=FALSE;
	m_bAutoScrolling=FALSE;

	m_bCreatingDragImage=FALSE;
	m_pDragImage=NULL;
	m_nSuspectDragItem=-1;
	m_bDragDropOwner=FALSE;
	m_bDragDropOperation=FALSE;

	m_nScrollTimerID=0;
	m_nCheckMouseTimerID=0;

	m_nInsertItemBefore=-1;

	if(m_dwComCtlVersion==0)
	{
		DWORD dwMajor, dwMinor;
		if(SUCCEEDED(GetComCtlVersion(&dwMajor, &dwMinor)))
		{
			m_dwComCtlVersion=MAKELONG((WORD)dwMinor, (WORD)dwMajor);
		}
		else
		{
			// assume that neither IE 3.0 nor IE 4.0 installed
			m_dwComCtlVersion=0x00040000;
		}
	}
}

COXSHBListCtrl::~COXSHBListCtrl()
{
	// clean up maps of all rectangles
	m_mapItemToBoundRect.RemoveAll();
	m_mapItemToImageRect.RemoveAll();
	m_mapItemToTextRect.RemoveAll();

	delete m_pDragImage;

	if(::IsWindow(GetSafeHwnd()))
		DestroyWindow();
}


BEGIN_MESSAGE_MAP(COXSHBListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(COXSHBListCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_MESSAGE(SHBLCM_HANDLEDRAG, OnHandleDrag)
	ON_MESSAGE(SHBLCM_CHECKCAPTURE, OnCheckCapture)

	ON_NOTIFY_EX(TTN_NEEDTEXTA, 0, OnItemToolTip)
	ON_NOTIFY_EX(TTN_NEEDTEXTW, 0, OnItemToolTip)

	ON_MESSAGE(SHBDTM_DRAGENTER, OnDragEnter)
	ON_MESSAGE(SHBDTM_DRAGLEAVE, OnDragLeave)
	ON_MESSAGE(SHBDTM_DRAGOVER, OnDragOver)
	ON_MESSAGE(SHBDTM_DROP, OnDrop)

	ON_MESSAGE(SHBM_SETSHBGROUP, OnSetSHBGroup)
	ON_MESSAGE(SHBM_SHBINFOCHANGED, OnSHBInfoChanged)
	ON_MESSAGE(SHBM_GROUPINFOCHANGED, OnGroupInfoChanged)
	ON_MESSAGE(SHBM_POPULATECONTEXTMENU, OnPopulateContextMenu)
	ON_NOTIFY(SHBEN_FINISHEDIT, SHB_IDCEDIT, OnChangeItemText)

	ON_MESSAGE(LVM_CREATEDRAGIMAGE, OnCreateDragImage)
	ON_MESSAGE(LVM_DELETEALLITEMS, OnDeleteAllItems)
	ON_MESSAGE(LVM_DELETEITEM, OnDeleteItem)
	ON_MESSAGE(LVM_EDITLABEL, OnEditLabel)
	ON_MESSAGE(LVM_ENSUREVISIBLE, OnEnsureVisible)
	ON_MESSAGE(LVM_FINDITEM, OnFindItem)
	ON_MESSAGE(LVM_GETBKCOLOR, OnGetBkColor)
	ON_MESSAGE(LVM_GETCOUNTPERPAGE, OnGetCountPerPage)
	ON_MESSAGE(LVM_GETEDITCONTROL, OnGetEditControl)
	ON_MESSAGE(LVM_GETHOTITEM, OnGetHotItem)
	ON_MESSAGE(LVM_GETITEMPOSITION, OnGetItemPosition)
	ON_MESSAGE(LVM_GETITEMRECT, OnGetItemRect)
	ON_MESSAGE(LVM_GETORIGIN, OnGetOrigin)
	ON_MESSAGE(LVM_GETTEXTBKCOLOR, OnGetTextBkColor)
	ON_MESSAGE(LVM_GETTEXTCOLOR, OnGetTextColor)
	ON_MESSAGE(LVM_GETTOPINDEX, OnGetTopIndex)
	ON_MESSAGE(LVM_GETVIEWRECT, OnGetViewRect)
	ON_MESSAGE(LVM_HITTEST, OnHitTest)
	ON_MESSAGE(LVM_INSERTITEM, OnInsertItem)
	ON_MESSAGE(LVM_REDRAWITEMS, OnRedrawItems)
	ON_MESSAGE(LVM_SCROLL, OnScroll)
	ON_MESSAGE(LVM_SETBKCOLOR, OnSetBkColor)
	ON_MESSAGE(LVM_SETHOTITEM, OnSetHotItem)
	ON_MESSAGE(LVM_SETITEM, OnSetItem)
	ON_MESSAGE(LVM_SETITEMTEXT, OnSetItemText)
	ON_MESSAGE(LVM_SETTEXTBKCOLOR, OnSetTextBkColor)
	ON_MESSAGE(LVM_SETTEXTCOLOR, OnSetTextColor)
	ON_MESSAGE(LVM_SORTITEMS, OnSortItems)
	ON_MESSAGE(LVM_UPDATE, OnUpdate)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXSHBListCtrl message handlers

void COXSHBListCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	// Currently we support only LVS_ICON and LVS_SMALLICON views
	DWORD dwStyle=GetStyle();
	if((dwStyle&LVS_LIST)!=0)
	{
		ModifyStyle(LVS_LIST,0);
		ModifyStyle(0,LVS_ICON);
	}
	if((dwStyle&LVS_REPORT)!=0)
	{
		ModifyStyle(LVS_REPORT,0);
		ModifyStyle(0,LVS_ICON);
	}

	_AFX_THREAD_STATE* pThreadState=AfxGetThreadState();
	// hook not already in progress
	if(pThreadState->m_pWndInit==NULL)
	{
		if(!InitListControl())
		{
			TRACE(_T("COXSHBListCtrl::PreSubclassWindow: failed to initialize list control\n"));
		}
	}
	
	CListCtrl::PreSubclassWindow();
}


BOOL COXSHBListCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL bResult=CListCtrl::Create(dwStyle, rect, pParentWnd, nID);
	if(bResult)
	{
		// Initialize edit control
		if(!InitListControl())
		{
			TRACE(_T("COXSHBListCtrl::Create: failed to initialize list control"));
			return FALSE;
		}
	}

	return bResult;
}

int COXSHBListCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	if((GetBarStyle()&SHBS_INFOTIP)==0)
		return -1;

	// now hit test against COXSHBListCtrl items
	UINT nHitFlags;
	int nHit=HitTest(point,&nHitFlags,TRUE);
	if(nHitFlags==SHBLC_ONITEM)
		nHit=-1;

	if(nHit!=-1 && pTI!= NULL)
	{
#if _MFC_VER<=0x0421
		if(pTI->cbSize>=sizeof(TOOLINFO))
			return nHit;
#endif
		// set window handle
		pTI->hwnd=GetSafeHwnd();

		// get item bounding rect
		CRect rect;
		VERIFY(GetItemRect(nHit,rect,LVIR_BOUNDS));
		pTI->rect=rect;

		// found matching item, return its index + 1, we cannot use zero index
		nHit++;
		pTI->uId=nHit;

		// set text to LPSTR_TEXTCALLBACK in order to get TTN_NEEDTEXT message when 
		// it's time to display tool tip
		pTI->lpszText=LPSTR_TEXTCALLBACK;
	}

	return nHit;
}

BOOL COXSHBListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

void COXSHBListCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// We provide all drawing functionality. We optimized it as good as it possible.

	// get client rect
	CRect rect;
	GetClientRect(rect);

	// Save DC
	int nSavedDC=dc.SaveDC();
	ASSERT(nSavedDC);

	// draw scroll buttons if we have to
	if((GetBarStyle()&SHBS_NOSCROLL)==0)
	{
		DrawScrollButtons(&dc);
		dc.ExcludeClipRect(m_rectTopScrollButton);
		dc.ExcludeClipRect(m_rectBottomScrollButton);
	}

	// fill background with color assoicated with control
	FillBackground(&dc);

	// check integrity
	int nCount=GetItemCount();
	VERIFY(m_mapItemToBoundRect.GetCount()==nCount);
	VERIFY(m_mapItemToTextRect.GetCount()==nCount);

	// draw items
	for(int nIndex=0; nIndex<nCount; nIndex++)
	{
		VERIFY(m_mapItemToBoundRect.Lookup(nIndex,rect));

		// take into account view origin
		rect-=m_ptOrigin;

		// draw only if visible
		if(dc.RectVisible(&rect))
		{
			// to reduce flickering use compatible device context to draw in
			//

			CRect rectItem=rect;

			rectItem-=rect.TopLeft();
			CDC dcCompatible;
			if(!dcCompatible.CreateCompatibleDC(&dc))
			{
				TRACE(_T("COXSHBListCtrl::OnPaint:Failed to create compatible DC"));
				return;
			}
			CBitmap bitmap;
			if(!bitmap.CreateCompatibleBitmap(&dc, rectItem.Width(), 
				rectItem.Height()))
			{
				TRACE(_T("COXSHBListCtrl::OnPaint:Failed to create compatible bitmap"));
				return;
			}
			CBitmap* pOldBitmap=dcCompatible.SelectObject(&bitmap);

			// fill standard DRAWITEMSTRUCT struct
			DRAWITEMSTRUCT dis;
			dis.CtlType=ODT_LISTVIEW;
			dis.CtlID=GetDlgCtrlID();
			dis.itemID=(UINT)nIndex;
			dis.itemAction=ODA_DRAWENTIRE;
			dis.itemState=ODS_DEFAULT;
			dis.hwndItem=GetSafeHwnd();
			dis.hDC=dcCompatible.GetSafeHdc();
			dis.rcItem=rectItem;
			dis.itemData=(DWORD)0;

			if (m_pShortcutBar != NULL)
			{
				if (HasBkColor())
					m_pShortcutBar->GetShortcutBarSkin()->FillBackground(&dcCompatible, rect, this,
						TRUE, GetBkColor());
				else
					m_pShortcutBar->GetShortcutBarSkin()->FillBackground(&dcCompatible, rect, this);
			}
			else
			{
				COLORREF clrBackground=GetBkColor();
				CBrush brush(clrBackground);
				dc.FillRect(rectItem,&brush);
			}

			// this function is responsible for drawing of any item
			DrawItem(&dis);

			// copy drawn image 
			dc.BitBlt(rect.left, rect.top, rect.Width(), 
				rect.Height(), &dcCompatible, 0, 0, SRCCOPY);
	
			if(pOldBitmap)
				dcCompatible.SelectObject(pOldBitmap);
		}
	}

	DrawPlaceHolder(&dc);

	// Restore dc	
	dc.RestoreDC(nSavedDC);

	// Do not call CWnd::OnPaint() for painting messages
}

void COXSHBListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	// recalculate items positions
	CalculateBoundRects();
}

void COXSHBListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CListCtrl::OnMouseMove(nFlags, point);

	// as soon as mouse is over the control we capture all mouse messages
	// (don't do anything if we are editing group header text)
	if(GetEditItem()==-1 && ((m_pShortcutBar==NULL) ? TRUE : 
		(m_pShortcutBar->GetEditGroup()==NULL)))
	{
		CheckCapture();
	}
}

void COXSHBListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	// we don't handle double clicks
	COXSHBListCtrl::OnLButtonDown(nFlags, point);
}

void COXSHBListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UNREFERENCED_PARAMETER(nFlags);

	// we should probably activate underneath window
	CWnd* pWnd=GetParent();
	ASSERT(pWnd);
	pWnd->ShowWindow(SW_SHOW);

	// find if mouse left button was pressed over any item
	UINT nHitFlags;
	int nItem=HitTest(point,&nHitFlags);

	if(nItem!=-1 && nHitFlags!=SHBLC_ONITEM)
	{
		// mark this item as selected
		int nOldSelectedItem=SelectItem(nItem);

		// notify parent of starting drag'n'drop if needed
		if((GetBarStyle()&SHBS_DISABLEDRAGITEM)==0)
		{
			// in result of selecting we could have done any reposition
			// so we'd better double check if selected item is still there
			int nSuspectItem=HitTest(point,&nHitFlags);
			if(nItem!=-1 && nHitFlags!=SHBLC_ONITEM && nSuspectItem==nItem)
			{
				// there shouldn't be any item currently being dragged
				ASSERT(GetDragItem()==-1);

				m_ptClickedLButton=point;

				// define the point of hot spot on future drag image
				if((GetBarStyle()&SHBS_DRAWITEMDRAGIMAGE)!=0)
				{
					CRect rectImageText;
					VERIFY(GetItemRect(nItem,&rectImageText,LVIR_BOUNDS));
					ASSERT(rectImageText.PtInRect(m_ptClickedLButton));
					m_ptDragImage=m_ptClickedLButton-rectImageText.TopLeft();
				}

				// mark the item as possible drag item (we initialize drag and drop 
				// operation when user move cursor while mouse left button is down)
				m_nSuspectDragItem=nItem;
			}
		}

		// update selected and old selected items 
		Update(nItem);
		if(nOldSelectedItem!=-1 && nOldSelectedItem!=nItem)
			Update(nOldSelectedItem);
	}

	// check if we have to start scrolling
	if(nHitFlags==SHBLC_ONTOPSCROLLBUTTON)
	{
		StartScrolling(TRUE);
	}
	else if(nHitFlags==SHBLC_ONBOTTOMSCROLLBUTTON)
	{
		StartScrolling(FALSE);
	}
	else if(m_bAutoScrolling)
	{
		StopScrolling();
	}

	// check if we lost mouse capture
	PostCheckCapture();
}

void COXSHBListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnLButtonUp(nFlags, point);

	// any drag'n'drop operation must be finished at this moment
	ASSERT(GetDragItem()==-1);

	// reset suspect drag item index
	m_nSuspectDragItem=-1;

	// find if mouse left button was unpressed over any item
	UINT nHitFlags;
	int nItem=HitTest(point,&nHitFlags);

	if(nItem!=-1 && nHitFlags!=SHBLC_ONITEM)
	{
		// if it is the selected item then activate it
		if(GetSelectedItem()==nItem)
		{
			int nOldItem=ActivateItem(nItem);
			if(nOldItem!=-1 && nOldItem!=nItem)
				// redraw old active item (only one item can be active at the moment)
				Update(nOldItem);
		}
		// redraw new active item
		Update(nItem);
	}

	// double check
	ASSERT(!(m_bScrollingUp&m_bScrollingDown));

	// stop any scrolling
	if(m_bScrollingUp || m_bScrollingDown)
		StopScrolling();

	// check if we lost mouse capture
	PostCheckCapture();
}

void COXSHBListCtrl::OnMButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(point);

	// check if we lost mouse capture
	PostCheckCapture();
}

void COXSHBListCtrl::OnMButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CListCtrl::OnMButtonUp(nFlags, point);

	// check if we lost mouse capture
	PostCheckCapture();
}

void COXSHBListCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(point);

	// check if we lost mouse capture
	PostCheckCapture();
}

void COXSHBListCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl::OnRButtonUp(nFlags, point);

	if(m_dwComCtlVersion<_IE40_COMCTL_VERSION)
	{
		ClientToScreen(&point);
		PostMessage(WM_CONTEXTMENU,(WPARAM)GetSafeHwnd(),
			(LPARAM)MAKELONG(point.x,point.y));
	}

	// check if we lost mouse capture
	PostCheckCapture();
}

/////////////////////////////////////////////////////////////////////////////
// OnHandleDrag sets up the drag image, calls DoDragDrop and cleans up 
// after the drag drop operation finishes.

LONG COXSHBListCtrl::OnHandleDrag(WPARAM wParam, LPARAM lParam)
{
	// In the case this control was created as a child window of shortcut bar group
	// fill structure for notification of parent window of this shortcut bar
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_HANDLEDRAGITEM;
	nmshb.hGroup=m_hGroup;
	nmshb.nItem=(int)wParam;
	nmshb.lParam=lParam;

	// check if parent of the shortcut bar would like to handle drag'n'drop itself
	if(SendSHBNotification(&nmshb))
		return (LONG)0;

	// get data source object that is represented by lParam. It's our responsibility
	// to eventually delete this object
	COleDataSource* pDataSource=(COleDataSource*)lParam;
	ASSERT(pDataSource!=NULL);
	// get item index which is represented by wParam
	int nItem=(int)wParam;
	// validate item index
	ASSERT(GetDragItem()==nItem);
	ASSERT(nItem>=0 && nItem<GetItemCount());

	// unselect the item that will be dragged
	VERIFY(SelectItem(-1)==nItem);
	Update(nItem);

	// get current cursor position
	POINT point;
	GetCursorPos(&point);

	// get the drop source object
	COleDropSource* pOleDropSource=GetDropSource();
	ASSERT(pOleDropSource!=NULL);

	// mark the control as the one which launched drag'n'drop operation
	m_bDragDropOwner=TRUE;

	// create drag image and start dragging
	if((GetBarStyle()&SHBS_DRAWITEMDRAGIMAGE)!=0)
	{
		// delete previously created drag image
		if(m_pDragImage)
		{
			delete m_pDragImage;
			m_pDragImage=NULL;
		}

		m_pDragImage=CreateDragImage(nItem);
		ASSERT(m_pDragImage);
	
		// changes the cursor to the drag image
		VERIFY(m_pDragImage->BeginDrag(0,m_ptDragImage));
	}

	// start drag'n'drop operation
	DROPEFFECT dropEffect=((COleDataSource*)pDataSource)->
		DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE,NULL,pOleDropSource);
	if(DROPEFFECT_MOVE==dropEffect)
	{
		// if item was moved in to theCalendar same list control and inserted index
		// is less or equal to the dragged one then increase the index by one to 
		// remove the item with right index
		if(m_nInsertItemBefore!=-1 && m_nInsertItemBefore<=GetDragItem())
			nItem++;
		// delete item if it was moved
		DeleteItem(nItem);
	}

	int nOldInsertItemBefore=m_nInsertItemBefore;
	m_nInsertItemBefore=-1;

	// unmark as the control which launched drag'n'drop operation
	m_bDragDropOwner=FALSE;

	// remove drag image
	if((GetBarStyle()&SHBS_DRAWITEMDRAGIMAGE)!=0)
	{
		ASSERT(m_pDragImage);
		// end dragging
		m_pDragImage->EndDrag();
	}

	// reset drag item
	SetDragItem(-1);

	//delete drag source (we are responsible to do that)
	delete pDataSource;

	// redraw the list control if any item was deleted and/or inserted
	if(DROPEFFECT_MOVE==dropEffect || 
		(DROPEFFECT_COPY==dropEffect && nOldInsertItemBefore!=-1))
		RedrawWindow();

	GetCursorPos(&point);
	ScreenToClient(&point);

	// send WM_LBUTTONUP message
	SendMessage(WM_LBUTTONUP,((GetKeyState(VK_CONTROL)<0) ? MK_CONTROL : 0)|
		((GetKeyState(VK_SHIFT)<0) ? MK_SHIFT : 0),(LPARAM)MAKELONG(point.x,point.y));

	// In the case this control was created as a child window of shortcut bar group
	// fill structure for notification of parent window of this shortcut bar
	::ZeroMemory((void*)&nmshb,sizeof(nmshb));
	nmshb.hdr.code=SHBN_ENDDRAGDROPITEM;
	nmshb.hGroup=m_hGroup;
	nmshb.lParam=dropEffect;

	// notify parent
	SendSHBNotification(&nmshb);

	return (LONG)0;
}

LONG COXSHBListCtrl::OnCheckCapture(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	// the only purpose of this handler is to set mouse capture back in case we lost it
	return ((LONG)CheckCapture());
}

BOOL COXSHBListCtrl::OnItemToolTip(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{   
	UNREFERENCED_PARAMETER(id);

	ASSERT(pNMHDR->code==TTN_NEEDTEXTA || pNMHDR->code==TTN_NEEDTEXTW);

	if(pNMHDR->idFrom>0)
	{
		// need to handle both ANSI and UNICODE versions of the message
		TOOLTIPTEXTA* pTTTA=(TOOLTIPTEXTA*)pNMHDR;
		TOOLTIPTEXTW* pTTTW=(TOOLTIPTEXTW*)pNMHDR;
    
		// idFrom must be the item index
		if (pNMHDR->code==TTN_NEEDTEXTA)
			ASSERT((pTTTA->uFlags&TTF_IDISHWND)==0);
		else
			ASSERT((pTTTW->uFlags&TTF_IDISHWND)==0);

		SHBINFOTIP shbit;
		::ZeroMemory(&shbit,sizeof(shbit));
		// fill structure for notification
		NMSHORTCUTBAR nmshb;
		nmshb.hdr.code=SHBN_GETITEMINFOTIP;
		nmshb.hGroup=m_hGroup;
		nmshb.nItem=(int)pNMHDR->idFrom-1;
		nmshb.lParam=(LPARAM)(&shbit);

		*pResult=SendSHBNotification(&nmshb);

		// copy the text
#ifndef _UNICODE
		if(pNMHDR->code==TTN_NEEDTEXTA)
			lstrcpyn(pTTTA->szText,shbit.szText,countof(pTTTA->szText));
		else
			_mbstowcsz(pTTTW->szText,shbit.szText,countof(pTTTW->szText));
#else
		if (pNMHDR->code==TTN_NEEDTEXTA)
			_wcstombsz(pTTTA->szText,shbit.szText,countof(pTTTA->szText));
		else
			lstrcpyn(pTTTW->szText,shbit.szText,countof(pTTTW->szText));
#endif

		if(pNMHDR->code==TTN_NEEDTEXTA)
		{
			if(shbit.lpszText==NULL)
				pTTTA->lpszText=pTTTA->szText;
			else
				pTTTA->lpszText=(LPSTR)shbit.lpszText;
			pTTTA->hinst=shbit.hinst;
		}
		else
		{
			if(shbit.lpszText==NULL)
				pTTTW->lpszText=pTTTW->szText;
			else
				pTTTW->lpszText=(LPWSTR)shbit.lpszText;
			pTTTW->hinst=shbit.hinst;
		}

		return TRUE;    // message was handled
	}
	else
	{
		*pResult=0;
		return TRUE;
	}
}

LONG COXSHBListCtrl::OnDragEnter(WPARAM wParam, LPARAM lParam)
{
	// list control should be valid drop target
	if((GetBarStyle()&SHBS_DISABLEDROPITEM)!=0)
		return (LONG)FALSE;

	// set flag that specifies that drag'n'drop operation is active
	m_bDragDropOperation=TRUE;

	// reset the hot item
	int nOldHotItem=SetHotItem(-1);
	if(nOldHotItem!=-1)
		Update(nOldHotItem);

	// In the case this control was created as a child window of shortcut bar group
	// fill structure for notification of parent window of this shortcut bar
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_DRAGENTER;
	nmshb.hGroup=m_hGroup;
	nmshb.lParam=lParam;

	// check if parent of the shortcut bar would like to handle drag'n'drop itself
	if(SendSHBNotification(&nmshb))
		return (LONG)TRUE;

	// lParam is the pointer to SHBDROPTARGETACTION structure
	LPSHBDROPTARGETACTION pSHBDTAction=(LPSHBDROPTARGETACTION)lParam;
	ASSERT(pSHBDTAction!=NULL);

	ASSERT(pSHBDTAction->pWnd);
	ASSERT(pSHBDTAction->pWnd->GetSafeHwnd()==GetSafeHwnd());

	// if this control launched the drag'n'drop operation then
	// display the drag image if needed
	if((GetBarStyle()&SHBS_DRAWITEMDRAGIMAGE)!=0 && m_bDragDropOwner)
	{
		CPoint point=pSHBDTAction->point;
		ClientToScreen(&point);

		ASSERT(m_pDragImage);
		VERIFY(m_pDragImage->DragEnter(GetDesktopWindow(),point));
	}

	return (LONG)OnDragOver(wParam,lParam);
}

LONG COXSHBListCtrl::OnDragOver(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if((GetBarStyle()&SHBS_DISABLEDROPITEM)!=0)
		return (LONG)FALSE;

	// In the case this control was created as a child window of shortcut bar group
	// fill structure for notification of parent window of this shortcut bar
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_DRAGOVER;
	nmshb.hGroup=m_hGroup;
	nmshb.lParam=lParam;

	// check if parent of the shortcut bar would like to handle drag'n'drop itself
	if(SendSHBNotification(&nmshb))
		return (LONG)TRUE;

	// lParam is the pointer to SHBDROPTARGETACTION structure
	LPSHBDROPTARGETACTION pSHBDTAction=(LPSHBDROPTARGETACTION)lParam;
	ASSERT(pSHBDTAction!=NULL);

	ASSERT(pSHBDTAction->pWnd);
	ASSERT(pSHBDTAction->pWnd->GetSafeHwnd()==GetSafeHwnd());

	BOOL bDrawDragImage=FALSE;

	// if we launched the drag'n'drop operation then move drag image if needed
	if((GetBarStyle()&SHBS_DRAWITEMDRAGIMAGE)!=0 && m_bDragDropOwner)
	{
		ASSERT(m_pDragImage);
		CPoint ptScreen=pSHBDTAction->point;
		ClientToScreen(&ptScreen);
		// move the drag image
		VERIFY(m_pDragImage->DragMove(ptScreen));
		bDrawDragImage=TRUE;
	}

	// analize the current cursor position
	//

	// it must be within client area of the control
	CRect rectClient;
	GetClientRect(rectClient);
	ASSERT(rectClient.PtInRect(pSHBDTAction->point));
	// deflate the client rectangle to match the valid area where we can
	// display items
	rectClient.DeflateRect(ID_EDGELEFTMARGIN,ID_EDGETOPMARGIN,ID_EDGERIGHTMARGIN,
		ID_EDGEBOTTOMMARGIN);

	CPoint point=pSHBDTAction->point;
	// take into account the control's origin
	point+=m_ptOrigin;
	// anylize view rectangle
	CRect rectView;
	GetViewRect(rectView);

	int nInsertItemBefore=-1;
	// Can we use this object?
	if(!pSHBDTAction->pDataObject->
		IsDataAvailable(COXShortcutBar::m_nChildWndItemFormat))
		pSHBDTAction->result=(LRESULT)DROPEFFECT_NONE;
	// if cursor is located higher then view rectangle or there is no items 
	// in the control
	else if(point.y<=rectView.top || GetItemCount()==0)
	{
		// Check if the control key was pressed          
		if((pSHBDTAction->dwKeyState & MK_CONTROL)==MK_CONTROL)
			pSHBDTAction->result=(LRESULT)DROPEFFECT_COPY;
		else
			pSHBDTAction->result=(LRESULT)DROPEFFECT_MOVE; 

		// if droped item must be inserted at the top
		nInsertItemBefore=0;
	}
	// if cursor is located lower than last item
	else if(point.y>rectView.bottom)
	{
		// Check if the control key was pressed          
		if((pSHBDTAction->dwKeyState & MK_CONTROL)==MK_CONTROL)
			pSHBDTAction->result=(LRESULT)DROPEFFECT_COPY;
		else
			pSHBDTAction->result=(LRESULT)DROPEFFECT_MOVE; 

		// if droped item must be inserted at the bottom
		nInsertItemBefore=GetItemCount();
	}
	// if cursor is out of deflated client rectangle 
	else if(!rectClient.PtInRect(pSHBDTAction->point))
		pSHBDTAction->result=(LRESULT)DROPEFFECT_NONE;
	// otherwise cursor is over list control view area
	else
	{
		// discover the location
		UINT nHitFlags;
		int nItem=HitTest(pSHBDTAction->point,&nHitFlags,TRUE);

		if(nItem==-1)
		{
			// Check if the control key was pressed          
			if((pSHBDTAction->dwKeyState & MK_CONTROL)==MK_CONTROL)
			{
				pSHBDTAction->result=(LRESULT)DROPEFFECT_COPY;
			}
			else
			{
				pSHBDTAction->result=(LRESULT)DROPEFFECT_MOVE; 
			}

			// find the closest item 
			LV_FINDINFO lvfi;
			lvfi.flags=LVFI_NEARESTXY;
			lvfi.pt=pSHBDTAction->point;
			lvfi.vkDirection=VK_DOWN;
			int nItem=FindItem(&lvfi);

			// assume that cursor is located lower than last item
			if(nItem==-1)
			{
				nItem=GetItemCount();
			}
			ASSERT(nItem>=0 && nItem<=GetItemCount());

			nInsertItemBefore=nItem;
		}
		else
		{
			pSHBDTAction->result=(LRESULT)DROPEFFECT_NONE;
		}
	}


	// check if we need to scroll the list control in order to show more items
	//

	// autoscrolling margins
	rectClient.DeflateRect(ID_DRAGDROPLEFTMARGIN,ID_DRAGDROPTOPMARGIN,
		ID_DRAGDROPRIGHTMARGIN,ID_DRAGDROPBOTTOMMARGIN);

	if(!rectClient.PtInRect(pSHBDTAction->point))
	{
		// check autoscrolling
		if(!m_bScrollingUp && pSHBDTAction->point.y<rectClient.top && 
			!m_rectTopScrollButton.IsRectEmpty())
		{
			// scroll up
			m_bAutoScrolling=TRUE;
			StartScrolling(TRUE);
		}
		else if(!m_bScrollingDown && pSHBDTAction->point.y>=rectClient.bottom && 
			!m_rectBottomScrollButton.IsRectEmpty())
		{
			// scroll down
			m_bAutoScrolling=TRUE;
			StartScrolling(FALSE);
		}
	}
	// check conditions to stop autoscrolling
	else if(m_bAutoScrolling)
	{
		StopScrolling();
	}

	// redraw placeholders on the list control
	if(m_nInsertItemBefore!=nInsertItemBefore)
	{
		if(bDrawDragImage)
		{
			// unlock window updates
			VERIFY(m_pDragImage->DragShowNolock(FALSE));
		}

		// remove old placeholder
		CRect rectPlaceHolder;
		if(m_nInsertItemBefore!=-1)
		{
			rectPlaceHolder=GetPlaceHolderRect(m_nInsertItemBefore);
			m_nInsertItemBefore=-1;
			RedrawWindow(rectPlaceHolder);
		}
		// draw new placeholder
		m_nInsertItemBefore=nInsertItemBefore;
		if(m_nInsertItemBefore!=-1)
		{
			rectPlaceHolder=GetPlaceHolderRect(m_nInsertItemBefore);
			RedrawWindow(rectPlaceHolder);
		}

		if(bDrawDragImage)
		{
			// lock window updates
			VERIFY(m_pDragImage->DragShowNolock(TRUE));
		}
	}

	return (LONG)TRUE;
}

LONG COXSHBListCtrl::OnDragLeave(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if((GetBarStyle()&SHBS_DISABLEDROPITEM)!=0)
		return (LONG)FALSE;

	// In the case this control was created as a child window of shortcut bar group
	// fill structure for notification of parent window of this shortcut bar
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_DRAGLEAVE;
	nmshb.hGroup=m_hGroup;
	nmshb.lParam=lParam;

	// check if parent of the shortcut bar would like to handle drag'n'drop itself
	if(SendSHBNotification(&nmshb))
	{
		// reset flag that specifies that drag'n'drop operation is active
		m_bDragDropOperation=FALSE;
		return (LONG)TRUE;
	}

	// lParam is the pointer to SHBDROPTARGETACTION structure
	LPSHBDROPTARGETACTION pSHBDTAction=(LPSHBDROPTARGETACTION)lParam;
	ASSERT(pSHBDTAction!=NULL);

	ASSERT(pSHBDTAction->pWnd);
	ASSERT(pSHBDTAction->pWnd->GetSafeHwnd()==GetSafeHwnd());

	// if we launched the drag'n'drop operation then remove drag image if it was used
	if((GetBarStyle()&SHBS_DRAWITEMDRAGIMAGE)!=0 && m_bDragDropOwner)
	{
		ASSERT(m_pDragImage);
		// remove dragging image
		VERIFY(m_pDragImage->DragLeave(GetDesktopWindow())); 
	}

	// remove placeholder image if any was drawn
	if(m_nInsertItemBefore!=-1)
	{
		CRect rectPlaceHolder=GetPlaceHolderRect(m_nInsertItemBefore);
		m_nInsertItemBefore=-1;
		RedrawWindow(rectPlaceHolder);
	}

	// check conditions to stop autoscrolling
	if(m_bAutoScrolling)
		StopScrolling();

	// reset flag that specifies that drag'n'drop operation is active
	m_bDragDropOperation=FALSE;

	return (LONG)TRUE;
}

LONG COXSHBListCtrl::OnDrop(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if((GetBarStyle()&SHBS_DISABLEDROPITEM)!=0)
		return (LONG)FALSE;

	// In the case this control was created as a child window of shortcut bar group
	// fill structure for notification of parent window of this shortcut bar
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_DROP;
	nmshb.hGroup=m_hGroup;
	nmshb.lParam=lParam;

	// check if parent of the shortcut bar would like to handle drag'n'drop itself
	if(SendSHBNotification(&nmshb))
	{
		m_bDragDropOperation=FALSE;
		return (LONG)TRUE;
	}

	// lParam is the pointer to SHBDROPTARGETACTION structure
	LPSHBDROPTARGETACTION pSHBDTAction=(LPSHBDROPTARGETACTION)lParam;
	ASSERT(pSHBDTAction!=NULL);

	ASSERT(pSHBDTAction->pWnd);
	ASSERT(pSHBDTAction->pWnd->GetSafeHwnd()==GetSafeHwnd());

	// if dragged item is to be copied or moved
	if((pSHBDTAction->dropEffect&DROPEFFECT_COPY)!=0 || 
		(pSHBDTAction->dropEffect&DROPEFFECT_MOVE)!=0)
	{
		// data must be in the specific format
		ASSERT(pSHBDTAction->pDataObject->
			IsDataAvailable(COXShortcutBar::m_nChildWndItemFormat));

		if(m_nInsertItemBefore!=-1)
		{
			// Get the drag item info
			//

			HGLOBAL hgData=pSHBDTAction->pDataObject->
				GetGlobalData(COXShortcutBar::m_nChildWndItemFormat);
			ASSERT(hgData);
			// lock it
			BYTE* lpItemData=(BYTE*)GlobalLock(hgData);

			// get item text
			CString sText((LPTSTR)lpItemData);
			lpItemData+=(sText.GetLength()+1)*sizeof(TCHAR);

			// get item lParam
			DWORD lParam=*(DWORD*)lpItemData;
			lpItemData+=sizeof(DWORD);

			// get the count of images associated with the draged item
			DWORD nImageCount=*(DWORD*)lpItemData;
			lpItemData+=sizeof(DWORD);
			
			int nImageIndex=-1;
			for(int nIndex=0; nIndex<(int)nImageCount; nIndex++)
			{
				// get image info from dragged data
				SHBIMAGEDROPINFO imageInfo;
				memcpy((void*)&imageInfo,(void*)lpItemData,sizeof(SHBIMAGEDROPINFO));
				lpItemData+=sizeof(SHBIMAGEDROPINFO);

				DWORD nImageSize=*(DWORD*)lpItemData;
				ASSERT(nImageSize>0);
				lpItemData+=sizeof(DWORD);

				void* pImageBuffer=malloc(nImageSize);
				ASSERT(pImageBuffer!=NULL);
				memcpy(pImageBuffer,(void*)lpItemData,nImageSize);
				ASSERT(pImageBuffer!=NULL);

				// move to the next image info structure
				lpItemData+=nImageSize;

				// we are interested only in "NORMAL" images
				if(imageInfo.imageType==SHBIT_NORMAL)
				{
					if(imageInfo.imageState==SHBIS_LARGE)
					{
						CImageList* pil=GetImageList(LVSIL_NORMAL);
						if(pil==NULL)
							pil=&m_ilLarge;
						int nNewItemIndex=CopyImageToIL(pil,pImageBuffer,nImageSize);
						ASSERT(nNewItemIndex!=-1);
						ASSERT(nImageIndex==-1 || nNewItemIndex==nImageIndex);
						nImageIndex=nNewItemIndex;

						if(GetImageList(LVSIL_NORMAL)==NULL)
							SetImageList(&m_ilLarge,LVSIL_NORMAL);
					}
					else if(imageInfo.imageState==SHBIS_SMALL)
					{
						CImageList* pil=GetImageList(LVSIL_SMALL);
						if(pil==NULL)
							pil=&m_ilSmall;
						int nNewItemIndex=CopyImageToIL(pil,pImageBuffer,nImageSize);
						ASSERT(nNewItemIndex!=-1);
						ASSERT(nImageIndex==-1 || nNewItemIndex==nImageIndex);
						nImageIndex=nNewItemIndex;

						if(GetImageList(LVSIL_SMALL)==NULL)
							SetImageList(&m_ilSmall,LVSIL_SMALL);
					}
				}

				ASSERT(pImageBuffer!=NULL);
				free(pImageBuffer);
			}

			// insert new item
			//
			LV_ITEM lvi;
			ZeroMemory((void*)&lvi,sizeof(lvi));
			lvi.mask=LVIF_TEXT;
			lvi.pszText=(LPTSTR)((LPCTSTR)sText);
			lvi.iSubItem=0;
			lvi.iItem=m_nInsertItemBefore;

			// set image info
			if(nImageIndex!=-1)
			{
				lvi.mask|=LVIF_IMAGE;
				lvi.iImage=nImageIndex;
			}
			
			// set lParam info
			if(m_bDragDropOwner)
			{
				lvi.mask|=LVIF_PARAM;
				lvi.lParam=lParam;
			}
			
			int nInsertedItem=InsertItem(&lvi);
			ASSERT(nInsertedItem!=-1);
			//////////////////////////////

			// get the amount of bytes of additional info
			SHBADDITIONALDROPINFO shbadi;
			shbadi.nBufferLength=*(DWORD*)lpItemData;
			lpItemData+=sizeof(DWORD);
			if(shbadi.nBufferLength>0)
			{
				// get additional info
				shbadi.pBuffer=(void*)lpItemData;

				// In the case this control was created as a child window 
				// of shortcut bar group fill structure for notification of 
				// parent window of this shortcut bar
				NMSHORTCUTBAR nmshb;
				nmshb.hdr.code=SHBN_SETADDITIONALDROPINFO;
				nmshb.hGroup=m_hGroup;
				nmshb.nItem=m_nInsertItemBefore;
				nmshb.lParam=(LPARAM)&shbadi;
				SendSHBNotification(&nmshb);

				lpItemData+=shbadi.nBufferLength;
			}

			// unlock it
			GlobalUnlock(hgData);
			// free it
			GlobalFree(hgData);

			// if we haven't launched the drag'n'drop operation then we have to redraw 
			// the control here to display newly inserted item
			if(!m_bDragDropOwner)
			{
				m_nInsertItemBefore=-1;
				RedrawWindow();
			}

			// drag'n'drop operation completed successfully
			pSHBDTAction->result=(LRESULT)TRUE;
		}
		else
			pSHBDTAction->result=(LRESULT)FALSE;
	}
	else
		pSHBDTAction->result=(LRESULT)FALSE;

	m_bDragDropOperation=FALSE;

	if(!m_bDragDropOwner)
	{
		// In the case this control was created as a child window of shortcut bar group
		// fill structure for notification of parent window of this shortcut bar
		NMSHORTCUTBAR nmshb;
		nmshb.hdr.code=SHBN_ENDDRAGDROPITEM;
		nmshb.hGroup=m_hGroup;
		nmshb.lParam=pSHBDTAction->result!=0 ? 
			pSHBDTAction->dropEffect : DROPEFFECT_NONE;

		// notify parent
		SendSHBNotification(&nmshb);
	}

	// we handled the message
	return (LONG)TRUE;
}

void COXSHBListCtrl::OnChangeItemText(NMHDR* pNotifyStruct, LRESULT* result)
{
	// this function handles SHBEN_FINISHEDIT that is sent by edit control 
	// to notify its parent that editing was finished
	ASSERT(::IsWindow(m_edit.GetSafeHwnd()));
	ASSERT(m_nEditItem>=0 && m_nEditItem<GetItemCount());

	LPNMSHBEDIT lpNMSHBE=(LPNMSHBEDIT)pNotifyStruct;
	// check if editing wasn't canceled
	if(lpNMSHBE->bOK)
	{
		// get the edit control text
		CString sText;
		m_edit.GetWindowText(sText);

		// In the case this control was created as a child window 
		// of shortcut bar group fill structure for notification of 
		// parent window of this shortcut bar
		NMSHORTCUTBAR nmshb;
		nmshb.hdr.code=SHBN_ENDITEMEDIT;
		nmshb.hGroup=m_hGroup;
		nmshb.nItem=m_nEditItem;
		nmshb.lParam=(LPARAM)((LPCTSTR)sText);

		// check if parent rejects the updated text
		if(!SendSHBNotification(&nmshb))
		{
			// change the text of item
			if(!SetItemText(m_nEditItem,0,sText.GetBuffer(sText.GetLength())))
			{
				TRACE(_T("COXSHBListCtrl::OnChangeItemText: failed to set text to item: %s"),sText);
			}
			sText.ReleaseBuffer();
		}
	}

	// clear edit item index
	m_nEditItem=-1;

	*result=0;
}

LONG COXSHBListCtrl::OnPopulateContextMenu(WPARAM wParam, LPARAM lParam)
{
	// Handles SHBM_POPULATECONTEXTMENU message that is fired by parent shortcut bar
	// in order to populate context menu with our own items
	UNREFERENCED_PARAMETER(wParam);

	LPSHBCONTEXTMENU pSHBCM=(LPSHBCONTEXTMENU)lParam;

	// double check that the message came from its parent shortcut bar and
	// corresponding group
	if(pSHBCM->pShortcutBar==m_pShortcutBar && pSHBCM->hGroup==m_hGroup)
	{
		CPoint point=pSHBCM->point;
		ScreenToClient(&point);

		// find if right mouse was clicked over any item
		UINT nFlags;
		int nItem=HitTest(point,&nFlags);
		if(nItem!=-1 && nFlags!=SHBLC_ONITEM)
		{
			ASSERT(nItem>=0 && nItem<GetItemCount());

			// add two items to menu to edit and remove items from the control
			CMenu* pMenu=pSHBCM->pMenu;
			if((GetBarStyle()&SHBS_EDITITEMS)!=0)
			{
				CString sItem;
				VERIFY(sItem.LoadString(IDS_OX_SHBLC_IDMRENAMEITEM_TEXT));
				if(pMenu->GetMenuItemCount()>0)
					pMenu->AppendMenu(MF_SEPARATOR);
				pMenu->AppendMenu(MF_STRING,SHBLC_IDMRENAMEITEM,
					sItem);

				VERIFY(sItem.LoadString(IDS_OX_SHBLC_IDMREMOVEITEM_TEXT));
				pMenu->AppendMenu(MF_STRING,SHBLC_IDMREMOVEITEM,
					sItem);
			}
		}
	}

	return ((LONG)0);
}

LONG COXSHBListCtrl::OnSetSHBGroup(WPARAM wParam, LPARAM lParam)
{
	// Handles SHBM_SETSHBGROUP message that is fired by parent shortcut bar
	// as soon as this control associated with any group 
	UNREFERENCED_PARAMETER(wParam);

	// cast parent to shortcut bar
	CWnd* pWnd=GetParent();
	ASSERT(pWnd);
	// check if it's really shortcut bar
	if(pWnd->IsKindOf(RUNTIME_CLASS(COXShortcutBar)))
	{
		m_hGroup=(HSHBGROUP)lParam;
		ASSERT(m_hGroup);

		m_pShortcutBar=(COXShortcutBar*)pWnd;
	}

	return ((LONG)0);
}

LONG COXSHBListCtrl::OnSHBInfoChanged(WPARAM wParam, LPARAM lParam)
{
	// Handles SHBM_SHBINFOCHANGED message that is fired by parent shortcut bar
	// to notify its child window that some of shortcut bar properties have changed.
	// lParam specifies mask of updated properties

	UNREFERENCED_PARAMETER(wParam);

	ASSERT(m_pShortcutBar!=NULL);
	ASSERT(m_hGroup!=NULL);

	UINT nMask=(UINT)lParam;

	// if size of scroll buttons has changed then we should recalculate 
	// corresponding rectangles
	if((nMask&SHBIF_SCRLBTNSIZE)!=0)
	{
		CalculateScrollRects();
	}

	// if style of shortcut bar has been changed then we should recalculate 
	// corresponding rectangles and redraw the window
	if((nMask&SHBIF_SHBSTYLE)!=0)
	{
		CalculateScrollRects();
		RedrawWindow();
	}

	return ((LONG)0);
}

LONG COXSHBListCtrl::OnGroupInfoChanged(WPARAM wParam, LPARAM lParam)
{
	// Handles SHBM_GROUPINFOCHANGED message that is fired by parent shortcut bar
	// to notify its child window that some of associated with this control
	// shortcut bar group properties have changed. lParam specifies mask of updated 
	// properties

	UNREFERENCED_PARAMETER(wParam);

	ASSERT(m_pShortcutBar!=NULL);
	ASSERT(m_hGroup!=NULL);

	UINT nMask=(UINT)lParam;

	// if descriptor changed then reposition all items
	if((nMask&SHBIF_DESCRIPTOR)!=0)
	{
		CalculateBoundRects();
	}

	// if view chnged then reset the view origin and redraw the control
	if((nMask&SHBIF_VIEW)!=0)
	{
		Scroll(-m_ptOrigin);			
		CalculateBoundRects();
	}

	return ((LONG)0);
}

LRESULT COXSHBListCtrl::OnCreateDragImage(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)CreateDragImage((int)wParam,(LPPOINT)lParam);
}

LRESULT COXSHBListCtrl::OnDeleteAllItems(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	// fill structure for notification
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_DELETEALLITEMS;
	nmshb.hGroup=m_hGroup;

	SendSHBNotification(&nmshb);

	LRESULT result=Default();

	// recalculate all item position (actually remove all information about items)
	if((BOOL)result)
	{
		m_ptOrigin=CPoint(0,0);

		SelectItem(-1);
		ActivateItem(-1);

		CalculateBoundRects();
		RedrawWindow();
	}

	return result;
}

LRESULT COXSHBListCtrl::OnDeleteItem(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	// In the case this control was created as a child window of shortcut bar group
	// fill structure for notification of parent window of this shortcut bar
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_DELETEITEM;
	nmshb.hGroup=m_hGroup;
	nmshb.nItem=(int)wParam;

	SendSHBNotification(&nmshb);


	LRESULT result=Default();

	// reposition items and redraw the control
	if((BOOL)result)
	{
		// change the origin of the control if the last item was deleted 
		// and it was the top item at the moment
		if(GetTopIndex()==-1)
			m_ptOrigin=CPoint(0,0);

		// if deleted item had lower index than the selected item index
		// then decrease it by one
		if(GetSelectedItem()>(int)wParam)
			m_nSelectedItem--;
		// if deleted item was the selected one then reset the index of selected item
		else if(GetSelectedItem()==(int)wParam)
			m_nSelectedItem=-1;

		// if deleted item had lower index than the active item index
		// then decrease it by one
		if(GetActiveItem()>(int)wParam)
			m_nActiveItem--;
		// if deleted item was the active one then reset the index of active item item
		else if(GetActiveItem()==(int)wParam)
			m_nActiveItem=-1;

		// reposition the items and redraw the control if there is no active 
		// drag'n'drop operation
		CalculateBoundRects();
		if(!m_bDragDropOperation)
		{
			RedrawWindow();
		}
	}

	return result;
}

LRESULT COXSHBListCtrl::OnEditLabel(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	return (LRESULT)EditLabel((int)wParam);
}

LRESULT COXSHBListCtrl::OnEnsureVisible(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)EnsureVisible((int)wParam,(BOOL)lParam);
}

LRESULT COXSHBListCtrl::OnFindItem(WPARAM wParam, LPARAM lParam)
{
	// we provide our own functionality to find items dpecified by their position
	if(((LV_FINDINFO*)lParam)->flags&LVFI_NEARESTXY)
		return (LONG)FindItem((LV_FINDINFO*)lParam,(int)wParam);
	else
		return (LRESULT)Default();
}

LRESULT COXSHBListCtrl::OnGetBkColor(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	UNREFERENCED_PARAMETER(wParam);

	if(m_pShortcutBar)
		return (LRESULT)GetBkColor();
	else
		return Default();
}

LRESULT COXSHBListCtrl::OnGetCountPerPage(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	UNREFERENCED_PARAMETER(wParam);
	return (LRESULT)GetCountPerPage();
}

LRESULT COXSHBListCtrl::OnGetEditControl(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	UNREFERENCED_PARAMETER(wParam);
	return (LRESULT)GetEditControl();
}

LONG COXSHBListCtrl::OnGetHotItem(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	UNREFERENCED_PARAMETER(wParam);
	return (LONG)GetHotItem();
}

LRESULT COXSHBListCtrl::OnGetItemPosition(WPARAM wParam, LPARAM lParam)
{
	return GetItemPosition((int)wParam,(LPPOINT)lParam);
}

LRESULT COXSHBListCtrl::OnGetItemRect(WPARAM wParam, LPARAM lParam)
{
	return GetItemRect((int)wParam,(LPRECT)lParam,((LPRECT)lParam)->left);
}

LRESULT COXSHBListCtrl::OnGetOrigin(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	return GetOrigin((LPPOINT)lParam);
}

LRESULT COXSHBListCtrl::OnGetTextBkColor(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	UNREFERENCED_PARAMETER(wParam);

	if(m_pShortcutBar)
		return GetTextBkColor();
	else
		return Default();
}

LRESULT COXSHBListCtrl::OnGetTextColor(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	UNREFERENCED_PARAMETER(wParam);

	if(m_pShortcutBar)
		return GetTextColor();
	else
		return Default();
}

LRESULT COXSHBListCtrl::OnGetTopIndex(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	UNREFERENCED_PARAMETER(wParam);
	return (LRESULT)GetTopIndex();
}

LRESULT COXSHBListCtrl::OnGetViewRect(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	return (LRESULT)GetViewRect((LPRECT)lParam);
}

LRESULT COXSHBListCtrl::OnHitTest(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	return (LRESULT)HitTest((LPLVHITTESTINFO)lParam);
}

LRESULT COXSHBListCtrl::OnInsertItem(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	LRESULT result=Default();

	// reposition items and redraw the control
	if((int)result!=-1)
	{
		// In the case this control was created as a child window of shortcut bar group
		// fill structure for notification of parent window of this shortcut bar
		NMSHORTCUTBAR nmshb;
		nmshb.hdr.code=SHBN_INSERTITEM;
		nmshb.hGroup=m_hGroup;
		nmshb.nItem=((LV_ITEM*)lParam)->iItem;
		nmshb.lParam=lParam;

		SendSHBNotification(&nmshb);

		// if inserted item had lower or equal index as the selected item index
		// then increase the last by one
		if(GetSelectedItem()>=(int)result)
			m_nSelectedItem++;

		// if inserted item had lower or equal index as the active item index
		// then increase the last by one
		if(GetActiveItem()>=(int)result)
			m_nActiveItem++;

		// reposition the items and redraw the control if there is no active 
		// drag'n'drop operation
		CalculateBoundRects();
		if(!m_bDragDropOperation)
		{
			RedrawWindow();
		}
	}

	return result;
}

LRESULT COXSHBListCtrl::OnRedrawItems(WPARAM wParam, LPARAM lParam)
{
	return RedrawItems((int)wParam,(int)lParam);
}

LRESULT COXSHBListCtrl::OnScroll(WPARAM wParam, LPARAM lParam)
{
	return Scroll((int)wParam,(int)lParam);
}

LRESULT COXSHBListCtrl::OnSetBkColor(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if(m_pShortcutBar)
		return SetBkColor((COLORREF)lParam);
	else
		return Default();
}

LRESULT COXSHBListCtrl::OnSetHotItem(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	return SetHotItem((int)wParam);
}

LRESULT COXSHBListCtrl::OnSetItem(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	LRESULT result=Default();

	if((BOOL)result)
	{
		// reposition items and redraw the control
		CalculateBoundRects();
		RedrawWindow();
	}

	return result;
}

LRESULT COXSHBListCtrl::OnSetItemText(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	LRESULT result= Default();

	if((BOOL)result)
	{
		// reposition items and redraw the control
		CalculateBoundRects();
		RedrawWindow();
	}

	return result;
}

LRESULT COXSHBListCtrl::OnSetTextBkColor(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if(m_pShortcutBar)
		return SetTextBkColor((COLORREF)lParam);
	else
		return Default();
}

LRESULT COXSHBListCtrl::OnSetTextColor(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if(m_pShortcutBar)
		return SetTextColor((COLORREF)lParam);
	else
		return Default();
}

LRESULT COXSHBListCtrl::OnSortItems(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	LRESULT result=Default();

	// reposition items and redraw the control
	if((BOOL)result)
	{
		// reset active and selected items
		SelectItem(-1);
		ActivateItem(-1);

		// reposition items and redraw the control
		CalculateBoundRects();
		RedrawWindow();
	}

	return result;
}

LRESULT COXSHBListCtrl::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	return Update((int)wParam);
}

void COXSHBListCtrl::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	// we use timer to provide scrolling functionality
	if(m_nScrollTimerID==nIDEvent)
	{
		// for pshycho
		ASSERT(m_bScrollingUp || m_bScrollingDown);

		CRect rectClient;
		GetClientRect(rectClient);
		// deflate the client rectangle to match the valid area where we can
		// display items
		rectClient.DeflateRect(ID_EDGELEFTMARGIN,ID_EDGETOPMARGIN,ID_EDGERIGHTMARGIN,
			ID_EDGEBOTTOMMARGIN);
		ASSERT(rectClient.top<rectClient.bottom);

		UINT nHitFlags;

		// get cursor position when the last mouse message was fired
		DWORD dwMessagePos=::GetMessagePos();
		CPoint point(GET_X_LPARAM(dwMessagePos),GET_Y_LPARAM(dwMessagePos));
		ScreenToClient(&point);
		HitTest(point,&nHitFlags);

		// if any drag and drop operation is undergoing then cheat the control
		if(m_bDragDropOperation)
		{
			if(m_bScrollingUp)
				nHitFlags=SHBLC_ONTOPSCROLLBUTTON;
			else 
				nHitFlags=SHBLC_ONBOTTOMSCROLLBUTTON;

			if((GetBarStyle()&SHBS_DRAWITEMDRAGIMAGE)!=0 && m_bDragDropOwner)
			{
				ASSERT(m_pDragImage);
				// unlock window updates
				VERIFY(m_pDragImage->DragShowNolock(FALSE));
			}
		}

		// if we are still over the scroll butoon
		if(m_bScrollingUp && nHitFlags==SHBLC_ONTOPSCROLLBUTTON)
		{
			if(!m_bDragDropOperation)
				ASSERT(!m_rectTopScrollButton.IsRectEmpty());
			RedrawWindow(m_rectTopScrollButton);

			// scroll up
			CRect rect;

			int nItem=GetTopIndex();
			if(nItem==-1)
				nItem=GetItemCount()-1;
			else 
			{
				VERIFY(m_mapItemToBoundRect.Lookup(nItem,rect));
				if(m_ptOrigin.y+rectClient.top<=rect.top)
				{
					ASSERT(nItem>0);
					nItem--;
				}
			}

			ASSERT((nItem>=0 && nItem<GetItemCount()));

			VERIFY(m_mapItemToBoundRect.Lookup(nItem,rect));
			int nOffset=rect.top-rectClient.top-m_ptOrigin.y;
			Scroll(CSize(0,nOffset));

			// check if we scrolled to the top
			if(m_rectTopScrollButton.IsRectEmpty())
				StopScrolling();
		}
		else if(m_bScrollingDown && nHitFlags==SHBLC_ONBOTTOMSCROLLBUTTON)
		{
			if(!m_bDragDropOperation)
				ASSERT(!m_rectBottomScrollButton.IsRectEmpty());
			RedrawWindow(m_rectBottomScrollButton);

			// scroll down
			CRect rect;

			int nItem=GetTopIndex();
			ASSERT(nItem>=0 && nItem<GetItemCount());

			if(nItem==GetItemCount()-1)
			{
				VERIFY(m_mapItemToBoundRect.Lookup(nItem,rect));
				int nOffset=rect.bottom-rectClient.bottom-m_ptOrigin.y;
				Scroll(CSize(0,nOffset));
			}
			else
			{
				nItem++;
				VERIFY(m_mapItemToBoundRect.Lookup(nItem,rect));
				int nOffset=rect.top-rectClient.top-m_ptOrigin.y;
				Scroll(CSize(0,nOffset));
			}

			// check if we scrolled to the bottom
			if(m_rectBottomScrollButton.IsRectEmpty())
				StopScrolling();
		}

		if(m_bDragDropOperation && (GetBarStyle()&SHBS_DRAWITEMDRAGIMAGE)!=0 && 
			m_bDragDropOwner)
		{
			ASSERT(m_pDragImage);
			// lock window updates
			VERIFY(m_pDragImage->DragShowNolock(TRUE));
		}
	}
	else if(nIDEvent==m_nCheckMouseTimerID)
	{
		if(!m_bDragDropOperation)
			PostMessage(SHBLCM_CHECKCAPTURE);
	}
	else
		CListCtrl::OnTimer(nIDEvent);
}


void COXSHBListCtrl::OnDestroy() 
{
	CListCtrl::OnDestroy();
	
	// TODO: Add your message handler code here

	// just kill timers
	if(m_nScrollTimerID!=0)
	{
		KillTimer(m_nScrollTimerID);
		m_nScrollTimerID=0;
	}
	if(m_nCheckMouseTimerID!=0)
	{
		KillTimer(m_nCheckMouseTimerID);
		m_nCheckMouseTimerID=0;
	}
}

/////////////////////////////////////////////////////////////////////////////

BOOL COXSHBListCtrl::CreateEditControl()
{
	if(::IsWindow(m_edit.GetSafeHwnd()))
		return TRUE;

	// Create an invisible edit control
	CRect rect(0,0,0,0);
	BOOL bResult=m_edit.Create(WS_BORDER,rect,this,SHB_IDCEDIT);

	return bResult;
}

BOOL COXSHBListCtrl::CheckCapture()
{
	// As soon as we get first mouse message we have to capture all of them in order
	// to handle hot items

	// Check whether the mouse is over the list control
	CRect rectWindow;
	GetWindowRect(rectWindow);
	CPoint point;
	::GetCursorPos(&point);
	CWnd* pWnd=CWnd::WindowFromPoint(point);
	ASSERT(pWnd!=NULL);
	BOOL bIsMouseOver=(pWnd->GetSafeHwnd()==GetSafeHwnd());

	if(bIsMouseOver && IsWindowEnabled())
	{
		// Moved inside this control window. Capture the mouse 
//		SetCapture();
		if(m_nCheckMouseTimerID==0)
		{
			m_nCheckMouseTimerID=SetTimer(IDT_OXSHBLIST_CHECKMOUSE,
				ID_OXSHBLIST_CHECKMOUSE_DELAY,NULL);
			ASSERT(m_nCheckMouseTimerID!=0);
		}
		m_bMouseIsOver=TRUE;
	}
	else 
	{
		// ... We release the mouse capture (may have already lost it)
		// we could have dragged something
		if(!IsLButtonDown())
		{
//			ReleaseCapture();
			if(m_nCheckMouseTimerID!=0)
			{
				KillTimer(m_nCheckMouseTimerID);
				m_nCheckMouseTimerID=0;
			}
			m_bMouseIsOver=FALSE;
			int nOldHotItem=SetHotItem(-1);
			if(nOldHotItem!=-1)
				Update(nOldHotItem);
			return FALSE;
		}

		m_bMouseIsOver=FALSE;
	}

	// handle all mouse messages
	AnticipateMouseMessages();

	// return TRUE if we have all mouse input
//	return (GetCapture()==this);
	return TRUE;
}

void COXSHBListCtrl::PostCheckCapture()
{
	// check if mouse capture was lost during last mouse message handling
}

void COXSHBListCtrl::CalculateBoundRects()
{
	// clean up map of all bound rectangles
	m_mapItemToBoundRect.RemoveAll();
	m_mapItemToImageRect.RemoveAll();
	m_mapItemToTextRect.RemoveAll();
	// clean up scroll buttons rectangles
	m_rectTopScrollButton.SetRectEmpty();
	m_rectBottomScrollButton.SetRectEmpty();

	int nCount=GetItemCount();
	// if there is no item then our mission is completed
	if(nCount==0)
	{
		return;
	}

	CRect rect;
	GetClientRect(rect);
	// if client rect is empty then our mission is completed too
	if(rect.IsRectEmpty())
	{
		for(int nIndex=0; nIndex<nCount; nIndex++)
		{
			m_mapItemToBoundRect.SetAt(nIndex,rect);
			m_mapItemToImageRect.SetAt(nIndex,rect);
			m_mapItemToTextRect.SetAt(nIndex,rect);
		}
		return;
	}

	// deflate rect to leave some space that won't be taken by items
	rect.DeflateRect(
		ID_EDGELEFTMARGIN,ID_EDGETOPMARGIN,ID_EDGERIGHTMARGIN,ID_EDGEBOTTOMMARGIN);

	// item positions depend on type of view
	int nView=GetView();

	// get corresponding item list to show items
	CImageList* pil=NULL;
	if(nView==SHB_LARGEICON)
	{
		pil=GetImageList(LVSIL_NORMAL);
	}
	else if(nView==SHB_SMALLICON)
	{
		pil=GetImageList(LVSIL_SMALL);
	}

	IMAGEINFO imageInfo;
	CRect rectImage;
	CRect rectText;
	CString sItemText;
	
	// loop through all items
	for(int nIndex=0; nIndex<nCount; nIndex++)
	{
		// get text
		sItemText=GetItemText(nIndex,0);

		// get image index
		int nImage=GetItemImage(nIndex);
		if(pil!=NULL && nImage>=0)
		{
			VERIFY(pil->GetImageInfo(nImage,&imageInfo));
			rectImage=imageInfo.rcImage;
			rectImage.InflateRect(3,3);
			rectImage-=rectImage.TopLeft();
		}
		else
		{
			rectImage.SetRectEmpty();
		}

		// calculate text rect and adjust it with image rect
		rectText=AdjustTextRect(sItemText,rectImage,rect.Width());
		if(nView==SHB_LARGEICON)
		{
			rect.bottom=rect.top+rectImage.Height()+rectText.Height()+
				ID_IMAGETEXTVERTMARGIN;
		}
		else if(nView==SHB_SMALLICON)
		{
			rect.bottom=rect.top+__max(rectImage.Height(),rectText.Height());
		}
		else
		{
			ASSERT(FALSE);
		}

		if(nView==SHB_LARGEICON)
		{
			int nMargin=rectImage.Width()-rect.Width();
			rectImage.left=rect.left-nMargin/2;
			rectImage.right=rect.right+nMargin/2+nMargin%2;
		}

		// save calculated rects
		m_mapItemToBoundRect.SetAt(nIndex,rect);
		m_mapItemToImageRect.SetAt(nIndex,rectImage);
		m_mapItemToTextRect.SetAt(nIndex,rectText);

		rect.top=rect.bottom+((nIndex==nCount-1) ? 0 : ID_ITEMMARGIN);
	}

	// recalculate scroll buttons rects
	CalculateScrollRects();
}

CRect COXSHBListCtrl::AdjustTextRect(CString sText,CRect& rectImage, int nMaxWidth)
{
	// takes on input text, image rect an max width that it can take
	//

	CRect rect;
	rect.SetRectEmpty();

	if(sText.IsEmpty() || nMaxWidth<=0)
		return rect;

	CClientDC dc(this);

	// Save DC
	int nSavedDC=dc.SaveDC();
	ASSERT(nSavedDC);

	// set font
	CFont* pOldFont=NULL;
	CFont font;
	if(m_pShortcutBar)
	{
		LPLOGFONT pLF=m_pShortcutBar->GetGroupTextFont(m_hGroup);
		ASSERT(pLF);
		VERIFY(font.CreateFontIndirect(pLF));
		pOldFont=dc.SelectObject(&font);
	}
	else
	{
		CFont* pFont=GetFont();
		if(pFont!=NULL)
			pOldFont=dc.SelectObject(pFont);
	}

	int nView=GetView();

	// depending on type of view calculate the text rect
	if(nView==SHB_LARGEICON)
	{
		rect.right=nMaxWidth-(ID_TEXTLEFTMARGIN+ID_TEXTRIGHTMARGIN);
		dc.DrawText(sText,rect,DT_VCENTER|DT_CENTER|DT_WORDBREAK|DT_CALCRECT|
			DT_EDITCONTROL);
		rect.InflateRect(0,0,ID_TEXTLEFTMARGIN+ID_TEXTRIGHTMARGIN,
			ID_TEXTTOPMARGIN+ID_TEXTBOTTOMMARGIN);

		// adjust text and image rects
		//

		int nHeight=rect.Height();
		rect.top+=rectImage.bottom+
			(rectImage.bottom==rectImage.top ? 0 : ID_IMAGETEXTVERTMARGIN);
		rect.bottom=rect.top+nHeight;

		int nWidth=rect.Width();
		if(nWidth>nMaxWidth)
			rect.right=nMaxWidth;
		else
		{
			int nMargin=nMaxWidth-nWidth;
			rect.left=nMargin/2;
			rect.right=rect.left+nWidth;
		}
	}
	else if(nView==SHB_SMALLICON && rectImage.Width()<nMaxWidth)
	{
		dc.DrawText(sText,rect,DT_VCENTER|DT_LEFT|DT_CALCRECT);
		rect.InflateRect(0,0,ID_TEXTLEFTMARGIN+ID_TEXTRIGHTMARGIN,
			ID_TEXTTOPMARGIN+ID_TEXTBOTTOMMARGIN);

		// adjust text and image rects
		//

		int nHeight=rect.Height();
		rect.top=rectImage.top;
		rect.bottom=rect.top+nHeight;

		int nMargin=nHeight-rectImage.Height();
		if(nMargin>0)
		{
			rectImage.OffsetRect(0,nMargin/2);
		}
		else if(nMargin<0)
		{
			rect.top-=nMargin/2;
			rect.bottom=rect.top+nHeight;
		}

		if(rectImage.left!=rectImage.right)
		{
			int nWidth=rect.Width();
			rect.left+=rectImage.right+ID_IMAGETEXTHORZMARGIN;
			rect.right=rect.left+nWidth;
		}
		// if there is no any image associated with the item thn in small icon view 
		// offset the text rectangle a little bit to make it look nicer
		else
			rect.left+=ID_TEXTLEFTMARGIN;

		if(rect.right>nMaxWidth)
			rect.right=nMaxWidth;
	}

	if(pOldFont!=NULL)
		dc.SelectObject(pOldFont);

	// Restore dc	
	dc.RestoreDC(nSavedDC);

	return rect;
}

void COXSHBListCtrl::CalculateScrollRects()
{
	// clean up scroll buttons rectangles
	m_rectTopScrollButton.SetRectEmpty();
	m_rectBottomScrollButton.SetRectEmpty();

	// define scroll buttons rectangles
	//
	CRect rect;
	GetClientRect(rect);
	rect.DeflateRect(
		ID_EDGELEFTMARGIN,ID_EDGETOPMARGIN,ID_EDGERIGHTMARGIN,ID_EDGEBOTTOMMARGIN);
	if(rect.top>=rect.bottom)
	{
		return;
	}


	CSize sizeScrollButton=GetScrollButtonSize();

	if(m_ptOrigin.y!=0)
	{
		// define rect for top scroll button
		m_rectTopScrollButton.top=rect.top;
		m_rectTopScrollButton.right=rect.right;
		m_rectTopScrollButton.bottom=rect.top+sizeScrollButton.cy;
		m_rectTopScrollButton.left=rect.right-sizeScrollButton.cx;
	}

	CRect rectScrollable;
	if(GetScrollableRect(rectScrollable))
	{
		if(rect.bottom+m_ptOrigin.y<rectScrollable.bottom)
		{
			// define rect for bottom scroll button
			m_rectBottomScrollButton.bottom=rect.bottom;
			m_rectBottomScrollButton.right=rect.right;
			m_rectBottomScrollButton.top=rect.bottom-sizeScrollButton.cy;
			m_rectBottomScrollButton.left=rect.right-sizeScrollButton.cx;
		}
	}
}

void COXSHBListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// Draws any item. It defined as virtual in case you want to draw items yourself.
	// But you should be careful - this control is highly optimized to be drawn in the 
	// most efficient way. Don't ruin it!
	int nItem=(int)lpDrawItemStruct->itemID;
	ASSERT(nItem>=0 && nItem<GetItemCount());

	// we use this routine to create drag image too
	if(!m_bCreatingDragImage)
	{
		// In the case this control was created as a child window 
		// of shortcut bar group fill structure for notification of 
		// parent window of this shortcut bar
		NMSHORTCUTBAR nmshb;
		nmshb.hdr.code=SHBN_DRAWITEM;
		nmshb.hGroup=m_hGroup;
		nmshb.nItem=nItem;
		nmshb.lParam=(LPARAM)lpDrawItemStruct;

		// In the case you handled this notification you could have provided all 
		// the drawing functionality so we check if you don't want us to run 
		// the standard implementation
		if(SendSHBNotification(&nmshb))
		{
			return;
		}
	}

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);	

	// Get the header rect
	CRect rectItem(lpDrawItemStruct->rcItem);	

	// Save DC
	int nSavedDC=dc.SaveDC();
	ASSERT(nSavedDC);
	int nView=GetView();

	// Draw Image
	//

	int nImage=GetItemImage(nItem);
	if(nImage>=0)
	{
		CImageList* pil=NULL;
		if(nView==SHB_LARGEICON)
		{
			pil=GetImageList(LVSIL_NORMAL);
		}
		else if(nView==SHB_SMALLICON)
		{
			pil=GetImageList(LVSIL_SMALL);
		}
		
		if(pil!=NULL)
		{
			CRect rectImage;
			VERIFY(m_mapItemToImageRect.Lookup(nItem,rectImage));

			rectImage.DeflateRect(1,1);
			if(!m_bCreatingDragImage)
			{
				COLORREF clrTopLeft=UpdateColor(GetBkColor(),64);
				COLORREF clrBottomRight=UpdateColor(GetBkColor(),-128);
				// Draw bounding rectangles if needed
				if(GetHotItem()==nItem)
				{
					if (m_pShortcutBar != NULL)
					{
						// Part if a shorcut bar
						BOOL bSelected = (GetSelectedItem() == nItem && IsLButtonDown()) ? TRUE : FALSE;
						m_pShortcutBar->GetShortcutBarSkin()->DrawItemBorder(&dc, rectImage, TRUE, bSelected, this);
					}
					else
					{
						// Not part of a shortcut bar
						if(IsLButtonDown() && GetSelectedItem()==nItem)
							dc.Draw3dRect(rectImage,clrBottomRight,clrTopLeft);
						else
							dc.Draw3dRect(rectImage,clrTopLeft,clrBottomRight);
					}
				}
				else if(GetActiveItem()==nItem && 
					(GetBarStyle() & SHBS_SHOWACTIVEALWAYS)!=0)
				{
					if (m_pShortcutBar != NULL)
						m_pShortcutBar->GetShortcutBarSkin()->DrawItemBorder(&dc, rectImage, FALSE, TRUE, this);
					else					
						dc.Draw3dRect(rectImage,clrBottomRight,clrTopLeft);
				}
			}

			// Draw icon
			rectImage.DeflateRect(2,2);
			pil->Draw(&dc,nImage,rectImage.TopLeft(),ILD_TRANSPARENT);
		}
	}

	// Draw text
	//

	CString sText=GetItemText(nItem,0);
	CRect rectText;
	VERIFY(m_mapItemToTextRect.Lookup(nItem,rectText));
	if(!rectText.IsRectEmpty() && !sText.IsEmpty())
	{
		// set transparent mode
		dc.SetBkMode(TRANSPARENT);

		// set text color
		COLORREF clrText=GetTextColor();
		VERIFY(clrText!=ID_COLOR_NONE);
		dc.SetTextColor(clrText);	

		// set font
		CFont* pOldFont=NULL;
		CFont font;
		if(m_pShortcutBar)
		{
			LPLOGFONT pLF=m_pShortcutBar->GetGroupTextFont(m_hGroup);
			ASSERT(pLF);
			VERIFY(font.CreateFontIndirect(pLF));
			pOldFont=dc.SelectObject(&font);
		}
		else
		{
			CFont* pFont=GetFont();
			if(pFont!=NULL)
			{
				pOldFont=dc.SelectObject(pFont);
			}
		}

		if(!m_bCreatingDragImage)
		{
			// update font if special style was set
			if((GetBarStyle()&SHBS_UNDERLINEHOTITEM)!=0 && 
				GetHotItem()==nItem)
			{
				CFont* pFont=dc.GetCurrentFont();
				ASSERT(pFont);
				LOGFONT lf;
				VERIFY(pFont->GetLogFont(&lf));
				lf.lfUnderline=TRUE;

				if(pOldFont!=NULL)
					dc.SelectObject(pOldFont);
				if((HFONT)font!=NULL)
					font.DeleteObject();
				VERIFY(font.CreateFontIndirect(&lf));
				pOldFont=dc.SelectObject(&font);
			}
		}

		// draw text
		if(nView==SHB_LARGEICON)
		{
			rectText.DeflateRect(ID_TEXTLEFTMARGIN,ID_TEXTTOPMARGIN,
				ID_TEXTRIGHTMARGIN,ID_TEXTBOTTOMMARGIN);
			dc.DrawText(sText,rectText,DT_CENTER|DT_VCENTER|DT_WORDBREAK|
				DT_EDITCONTROL|DT_END_ELLIPSIS);
		}
		else if(nView==SHB_SMALLICON)
		{
			dc.DrawText(sText,rectText,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}
		else
		{
			ASSERT(FALSE);
		}

		if(pOldFont!=NULL)
		{
			dc.SelectObject(pOldFont);
		}
	}

	// Restore dc	
	dc.RestoreDC(nSavedDC);
	// Detach the dc before returning	
	dc.Detach();
}

void COXSHBListCtrl::DrawScrollButtons(CDC* pDC)
{
	// draw scroll buttons only if needed
	//

	if((GetBarStyle()&SHBS_NOSCROLL)!=0)
	{
		TRACE(_T("COXSHBListCtrl::DrawScrollButtons: scroll buttons won't be displayed - SHBS_NOSCROLL style is set"));
		return ;
	}

	// check if we need to draw any of scroll buttons
	BOOL m_bDrawTopButton=((!m_rectTopScrollButton.IsRectEmpty()) & 
		pDC->RectVisible(&m_rectTopScrollButton));
	BOOL m_bDrawBottomButton=((!m_rectBottomScrollButton.IsRectEmpty()) & 
		pDC->RectVisible(&m_rectBottomScrollButton));

	if(!m_bDrawTopButton && !m_bDrawBottomButton)
	{
		return;
	}

	// Save DC
	int nSavedDC=pDC->SaveDC();
	ASSERT(nSavedDC);

	// check if the curent cursor position is over one of the scroll button
	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);
	UINT nFlags;
	HitTest(point,&nFlags);

	// use standard frame control as scroll button
	if(m_bDrawBottomButton)
		pDC->DrawFrameControl(m_rectBottomScrollButton,DFC_SCROLL,DFCS_SCROLLDOWN|
			((nFlags==SHBLC_ONBOTTOMSCROLLBUTTON && m_bScrollingDown && 
			(IsLButtonDown() || m_bAutoScrolling)) ? DFCS_PUSHED : 0));

	if(m_bDrawTopButton)
		pDC->DrawFrameControl(m_rectTopScrollButton,DFC_SCROLL,DFCS_SCROLLUP|
			((nFlags==SHBLC_ONTOPSCROLLBUTTON && m_bScrollingUp && 
			(IsLButtonDown() || m_bAutoScrolling)) ? DFCS_PUSHED : 0));

	// Restore dc	
	pDC->RestoreDC(nSavedDC);
}

void COXSHBListCtrl::DrawPlaceHolder(CDC* pDC)
{
	// draw place holder to drop dragged item only if needed
	//

	if(m_nInsertItemBefore<0 || m_nInsertItemBefore>GetItemCount())
		return ;

	// get rectangle that can be used to draw placeholder
	CRect rectPlaceHolder=GetPlaceHolderRect(m_nInsertItemBefore);
	if(!pDC->RectVisible(&rectPlaceHolder))
		return;

	ASSERT(!rectPlaceHolder.IsRectEmpty());

	// Save DC
	int nSavedDC=pDC->SaveDC();
	ASSERT(nSavedDC);

	// by default use black color
	COLORREF clr=RGB(0,0,0);
	// if the background color is too close to the black then use white color 
	// to draw the placeholder
	if(IsColorCloseTo(clr,GetBkColor(),64))
		clr=RGB(255,255,255);

	CPen pen(PS_SOLID,1,clr);
	CPen* pOldPen=pDC->SelectObject(&pen);

	// place holder for the top item
	if(m_nInsertItemBefore==0)
	{
		pDC->MoveTo(rectPlaceHolder.left,rectPlaceHolder.top);
		pDC->LineTo(rectPlaceHolder.right,rectPlaceHolder.top);

		int nHeight=2*ID_HEIGHTPLACEHOLDER/3;
		for(int nIndex=1; nIndex<=nHeight; nIndex++)
		{
			pDC->MoveTo(rectPlaceHolder.left,rectPlaceHolder.top+nIndex);
			pDC->LineTo(rectPlaceHolder.left+(nHeight-nIndex),
				rectPlaceHolder.top+nIndex);
			pDC->MoveTo(rectPlaceHolder.right-(nHeight-nIndex),
				rectPlaceHolder.top+nIndex);
			pDC->LineTo(rectPlaceHolder.right,rectPlaceHolder.top+nIndex);
		}
	}
	// place holder for the bottom item
	else if(m_nInsertItemBefore==GetItemCount())
	{
		pDC->MoveTo(rectPlaceHolder.left,rectPlaceHolder.bottom-1);
		pDC->LineTo(rectPlaceHolder.right,rectPlaceHolder.bottom-1);

		int nHeight=2*ID_HEIGHTPLACEHOLDER/3;
		for(int nIndex=1; nIndex<=nHeight; nIndex++)
		{
			pDC->MoveTo(rectPlaceHolder.left,rectPlaceHolder.bottom-nIndex);
			pDC->LineTo(rectPlaceHolder.left+(nHeight-nIndex),
				rectPlaceHolder.bottom-nIndex);
			pDC->MoveTo(rectPlaceHolder.right-(nHeight-nIndex),
				rectPlaceHolder.bottom-nIndex);
			pDC->LineTo(rectPlaceHolder.right,rectPlaceHolder.bottom-nIndex);
		}
	}
	// place holder is located between two existing items
	else
	{
		pDC->MoveTo(rectPlaceHolder.left,(rectPlaceHolder.top+rectPlaceHolder.bottom)/2);
		pDC->LineTo(rectPlaceHolder.right,(rectPlaceHolder.top+rectPlaceHolder.bottom)/2);

		int nHeight=ID_HEIGHTPLACEHOLDER/2;
		for(int nIndex=1; nIndex<=nHeight; nIndex++)
		{
			pDC->MoveTo(rectPlaceHolder.left,
				(rectPlaceHolder.top+rectPlaceHolder.bottom)/2+nIndex);
			pDC->LineTo(rectPlaceHolder.left+nHeight-nIndex,
				(rectPlaceHolder.top+rectPlaceHolder.bottom)/2+nIndex);
			pDC->MoveTo(rectPlaceHolder.right-nHeight+nIndex,
				(rectPlaceHolder.top+rectPlaceHolder.bottom)/2+nIndex);
			pDC->LineTo(rectPlaceHolder.right,
				(rectPlaceHolder.top+rectPlaceHolder.bottom)/2+nIndex);

			pDC->MoveTo(rectPlaceHolder.left,
				(rectPlaceHolder.top+rectPlaceHolder.bottom)/2-nIndex);
			pDC->LineTo(rectPlaceHolder.left+nHeight-nIndex,
				(rectPlaceHolder.top+rectPlaceHolder.bottom)/2-nIndex);
			pDC->MoveTo(rectPlaceHolder.right-nHeight+nIndex,
				(rectPlaceHolder.top+rectPlaceHolder.bottom)/2-nIndex);
			pDC->LineTo(rectPlaceHolder.right,
				(rectPlaceHolder.top+rectPlaceHolder.bottom)/2-nIndex);
		}
	}

	if(pOldPen!=NULL)
		pDC->SelectObject(pOldPen);

	// Restore dc	
	pDC->RestoreDC(nSavedDC);
}

void COXSHBListCtrl::FillBackground(CDC* pDC)
{
	// just fill background with preset background color

	COLORREF clrBackground = GetBkColor();
	VERIFY(clrBackground!=ID_COLOR_NONE);

	CBrush brush(clrBackground);

	CRect rect;
	GetClientRect(rect);
	int iSavedDC = pDC->SaveDC();

	// Exclude parts that will be covered by items
	int nItem;
	POSITION pos=m_mapItemToBoundRect.GetStartPosition();
	while(pos!=NULL)
	{
		CRect rectExclude;
		m_mapItemToBoundRect.GetNextAssoc(pos,nItem,rectExclude);
		rectExclude -= m_ptOrigin;

		ASSERT(nItem>=0 && nItem<GetItemCount());
		pDC->ExcludeClipRect(rectExclude);
	}

	// Fill only region that wasn't taken by items
	if (m_pShortcutBar != NULL)
	{
		if (HasBkColor())
			m_pShortcutBar->GetShortcutBarSkin()->FillBackground(pDC, rect, this, TRUE, GetBkColor());			
		else
			m_pShortcutBar->GetShortcutBarSkin()->FillBackground(pDC, rect, this);
	}
	else
		pDC->FillSolidRect(rect, clrBackground);

	pDC->RestoreDC(iSavedDC);
}

int COXSHBListCtrl::HitTest(CPoint pt, UINT* pFlags, BOOL bOnlyItems/* = FALSE*/) const
{
	// Scroll buttons have the highest priority (top buttom is prior to bottom button).
	// As long as scroll buttons may cover list control items you can specify 
	// bOnlyItems argument as TRUE to test the specified point only on item locations.
	//
	if(!bOnlyItems && (GetBarStyle()&SHBS_NOSCROLL)==0)
	{
		// point is over the top scroll button
		if(!m_rectTopScrollButton.IsRectEmpty() && 
			m_rectTopScrollButton.PtInRect(pt))
		{
			*pFlags=SHBLC_ONTOPSCROLLBUTTON;
			return -1;
		}

		// point is over the bottom scroll button
		if(!m_rectBottomScrollButton.IsRectEmpty() && 
			m_rectBottomScrollButton.PtInRect(pt))
		{
			*pFlags=SHBLC_ONBOTTOMSCROLLBUTTON;
			return -1;
		}
	}

	int nItem=-1;
	*pFlags=0;

	// adjust point 
	pt+=m_ptOrigin;

	// get rect that covers all current visible items
	CRect rectView;
	GetViewRect(rectView,TRUE);

	if(rectView.PtInRect(pt))
	{
		*pFlags=LVHT_NOWHERE;
		CRect rect;
		
		// try to find item rect that include the point
		POSITION pos=m_mapItemToBoundRect.GetStartPosition();
		while(pos!=NULL)
		{
			m_mapItemToBoundRect.GetNextAssoc(pos,nItem,rect);

			ASSERT(nItem>=0 && nItem<GetItemCount());

			if(rect.PtInRect(pt))
			{
				// point can be within image rect, text rect, on image and text
				// united rect or on item rect
				CRect rectImage;
				VERIFY(m_mapItemToImageRect.Lookup(nItem,rectImage));
				rectImage+=rect.TopLeft();
				// point is on the image
				if(rectImage.PtInRect(pt))
					*pFlags=LVHT_ONITEMICON;
				else
				{
					CRect rectText;
					VERIFY(m_mapItemToTextRect.Lookup(nItem,rectText));
					rectText+=rect.TopLeft();
					// point is on the text
					if(rectText.PtInRect(pt))
						*pFlags=LVHT_ONITEMLABEL;
					else
					{
						CRect rectImageAndText;
						rectImageAndText.left=__min(rectImage.left,rectText.left);
						rectImageAndText.right=__max(rectImage.right,rectText.right);
						rectImageAndText.top=__min(rectImage.top,rectText.top);
						rectImageAndText.bottom=__max(rectImage.bottom,rectText.bottom);
						// point is on the item bounding rectangle
						if(rectImageAndText.PtInRect(pt))
							*pFlags=SHBLC_ONIMAGEANDTEXT;
						// point is on the entire item rectangle
						else
							*pFlags=SHBLC_ONITEM;
					}
				}
				break;
			}
		}
		if(*pFlags==LVHT_NOWHERE)
			nItem=-1;
	}
	else
	{
		if(pt.x<rectView.left)
			*pFlags|=LVHT_TOLEFT;
		else if(pt.x>rectView.right)
			*pFlags|=LVHT_TORIGHT;
		if(pt.y<rectView.top)
			*pFlags|=LVHT_ABOVE;
		else if(pt.y>rectView.bottom)
			*pFlags|=LVHT_BELOW;
	}

	return nItem;
}

BOOL COXSHBListCtrl::GetViewRect(LPRECT lpRectView, 
								 BOOL bOnlyVisible/* = FALSE*/) const
{
	// try to find rect that cover all list control items (if bOnlyVisible is TRUE,
	// then only visible items)
	CRect rectClient;
	GetClientRect(rectClient);
	// deflate client rect by margins so there is some space in the control that 
	// is not covered by items
	rectClient.DeflateRect(ID_EDGELEFTMARGIN,ID_EDGETOPMARGIN,ID_EDGERIGHTMARGIN,
		ID_EDGEBOTTOMMARGIN);

	CRect rectView=rectClient;

	int nBottom=0;
	CRect rect;
	int nItem;
	POSITION pos=m_mapItemToBoundRect.GetStartPosition();
	while(pos!=NULL)
	{
		m_mapItemToBoundRect.GetNextAssoc(pos,nItem,rect);

		ASSERT(nItem>=0 && nItem<GetItemCount());

		if(nBottom<rect.bottom)
			nBottom=rect.bottom;

		if(bOnlyVisible && nBottom>rectClient.bottom+m_ptOrigin.y)
			break;
	}
	rectView.bottom=nBottom;

	if(bOnlyVisible)
	{
		rectView.top=__max(rectView.top,rectView.top+m_ptOrigin.y);
		rectView.bottom=__min(rectView.bottom,rectClient.bottom+m_ptOrigin.y);
	}

	lpRectView->left=rectView.left;
	lpRectView->right=rectView.right;
	lpRectView->top=rectView.top;
	lpRectView->bottom=rectView.bottom;

	return TRUE;
}

BOOL COXSHBListCtrl::GetItemRect(int nItem, LPRECT lpRect, UINT nCode) const
{
	ASSERT(nItem>=0 && nItem<GetItemCount());

	CRect rect;
	VERIFY(m_mapItemToBoundRect.Lookup(nItem,rect));
	rect-=m_ptOrigin;

	switch(nCode)
	{
	case LVIR_ICON:
		{
			// image rect
			CRect rectImage;
			rectImage.SetRectEmpty();
			m_mapItemToImageRect.Lookup(nItem,rectImage);
			rectImage+=rect.TopLeft();
			lpRect->left=rectImage.left;
			lpRect->top=rectImage.top;
			lpRect->right=rectImage.right;
			lpRect->bottom=rectImage.bottom;

			break;
		}
	case LVIR_LABEL:
		{
			// text rect
			CRect rectText;
			rectText.SetRectEmpty();
			VERIFY(m_mapItemToTextRect.Lookup(nItem,rectText));
			rectText+=rect.TopLeft();
			lpRect->left=rectText.left;
			lpRect->top=rectText.top;
			lpRect->right=rectText.right;
			lpRect->bottom=rectText.bottom;

			break;
		}
	case LVIR_BOUNDS:
		{
			// image and text united rect
			CRect rectImage;
			rectImage.SetRectEmpty();
			m_mapItemToImageRect.Lookup(nItem,rectImage);
			rectImage+=rect.TopLeft();

			CRect rectText;
			rectText.SetRectEmpty();
			VERIFY(m_mapItemToTextRect.Lookup(nItem,rectText));
			rectText+=rect.TopLeft();

			lpRect->left=__min(rectText.left,rectImage.left);
			lpRect->top=__min(rectText.top,rectImage.top);
			lpRect->right=__max(rectText.right,rectImage.right);
			lpRect->bottom=__max(rectText.bottom,rectImage.bottom);

			break;
		}
	case SHBLC_ENTIREITEM:
		{
			// entire item rect
			lpRect->left=rect.left;
			lpRect->top=rect.top;
			lpRect->right=rect.right;
			lpRect->bottom=rect.bottom;

			break;
		}
	default:
		ASSERT(FALSE);
	}

	return TRUE;
}

BOOL COXSHBListCtrl::GetItemPosition(int nItem, LPPOINT lpPoint) const
{
	ASSERT(nItem>=0 && nItem<GetItemCount());

	CRect rect;
	VERIFY(m_mapItemToBoundRect.Lookup(nItem,rect));
	// adjust rect
	rect-=m_ptOrigin;

	lpPoint->x=rect.left;
	lpPoint->y=rect.right;

	return TRUE;
}

BOOL COXSHBListCtrl::GetScrollableRect(CRect& rectScrollable, int nTopItem /*=-1*/) const
{
	// return the size of rect that list control can be scrolled to
	//

	CRect rectClient;
	GetClientRect(rectClient);
	// deflate client rect by margins so there is some space in the control that 
	// is not covered by items
	rectClient.DeflateRect(ID_EDGELEFTMARGIN,ID_EDGETOPMARGIN,ID_EDGERIGHTMARGIN,
		ID_EDGEBOTTOMMARGIN);
	if(rectClient.top>=rectClient.bottom)
		return FALSE;

	CRect rectView;
	GetViewRect(rectView);

	rectScrollable=rectView;

	// if all items fit the client area then just return view rect as the
	// scrollable one
	if((rectView.Height()+m_ptOrigin.y)<=rectClient.Height())
		return TRUE;

	// if the last item is visible then just return client rect plus origin offset
	// as scrollable one
	if(rectView.Height()<=(rectClient.Height()+m_ptOrigin.y))
	{
		rectScrollable.bottom+=rectClient.Height()+m_ptOrigin.y-rectView.Height();
		return TRUE;
	}

	CRect rect;

	// we can specify the top item that should be taken into account while 
	// calculating the scrollable rect as argument or if it is set to -1 then
	// we will found it using GetTopIndex function
	if(nTopItem==-1)
	{
		nTopItem=GetTopIndex();
		VERIFY(m_mapItemToBoundRect.Lookup(nTopItem,rect));
		if(m_ptOrigin.y+rectClient.top>rect.top)
			if(nTopItem==GetItemCount()-1)
				return TRUE;
			else
				nTopItem++;
	}

	// resulted top item must be in the valid range
	ASSERT(nTopItem>=0 && nTopItem<GetItemCount());



	VERIFY(m_mapItemToBoundRect.Lookup(nTopItem,rect));
	int nTop=rect.top;
	VERIFY(m_mapItemToBoundRect.Lookup(GetItemCount()-1,rect));
	int nBottom=rect.bottom;

	if(nBottom-nTop<rectClient.Height())
		rectScrollable.bottom+=rectClient.Height()-(nBottom-nTop);

	ASSERT(rectScrollable.Height()>=rectView.Height());

	return TRUE;
}

DWORD COXSHBListCtrl::GetBarStyle() const
{
	if(m_pShortcutBar)
		return m_pShortcutBar->GetBarStyle();
	else
	{
		// we use the set of our styles to define the list control functionality
		//

		DWORD dwBarStyle=0;
		DWORD dwStyle=GetStyle();

		if((dwStyle&LVS_EDITLABELS)!=0)
			dwBarStyle|=SHBS_EDITITEMS;

		if((dwStyle&LVS_NOSCROLL)!=0)
			dwBarStyle|=SHBS_NOSCROLL;

		if((dwStyle&LVS_SHOWSELALWAYS)!=0)
			dwBarStyle|=SHBS_SHOWACTIVEALWAYS;

		if((dwStyle&LVS_NOSCROLL)!=0)
			dwBarStyle|=SHBS_NOSCROLL;

		DWORD dwExStyle=GetExStyle();

		if((dwExStyle&LVS_EX_UNDERLINEHOT)!=0)
			dwBarStyle|=SHBS_UNDERLINEHOTITEM;

		if((dwExStyle&LVS_EX_INFOTIP)!=0)
			dwBarStyle|=SHBS_INFOTIP;
		 
		return dwBarStyle;
	}
}

LRESULT COXSHBListCtrl::SendSHBNotification(LPNMSHORTCUTBAR pNMSHB) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	// if parent window of that control is shortcut bar then send notification to 
	// its parent
	if(m_pShortcutBar)
	{
		return m_pShortcutBar->SendSHBNotification(pNMSHB);
	}
	else
	{
		// send standard shortcut bar notification to list control parent using 
		// NMSHORTCUTBAR structure

		// notify parent
		CWnd* pParentWnd=GetOwner();
		if(pParentWnd)
		{
			// fill notification structure
			pNMSHB->hdr.hwndFrom=GetSafeHwnd();
			pNMSHB->hdr.idFrom=GetDlgCtrlID();

			return (pParentWnd->SendMessage(
				WM_NOTIFY,(WPARAM)GetDlgCtrlID(),(LPARAM)pNMSHB));
		}
		else
		{
			return (LRESULT)0;
		}
	}
}

int COXSHBListCtrl::GetView() const
{
	// At the moment we defined only two views: Large Icons & Small Icons
	if(m_pShortcutBar)
	{
		int nView=m_pShortcutBar->GetGroupView(m_hGroup);
		VERIFY(m_pShortcutBar->VerifyView(nView));
		return nView;
	}
	else
	{
		DWORD dwStyle=GetStyle();

		if((dwStyle&LVS_SMALLICON)!=0)
			return SHB_SMALLICON;
		else
			return SHB_LARGEICON;
	}
}

CSize COXSHBListCtrl::GetScrollButtonSize() const
{
	if(m_pShortcutBar)
		return m_pShortcutBar->GetScrollButtonSize();
	else
		return CSize(DFLT_SCROLLBUTTONWIDTH,DFLT_SCROLLBUTTONHEIGHT);
}

UINT COXSHBListCtrl::GetScrollingDelay() const
{
	if(m_pShortcutBar)
		return m_pShortcutBar->GetScrollingDelay();
	else
		return DFLT_SCROLLINGDELAY;
}

UINT COXSHBListCtrl::GetAutoScrollingDelay() const
{
	if(m_pShortcutBar)
		return m_pShortcutBar->GetAutoScrollingDelay();
	else
		return DFLT_AUTOSCROLLINGDELAY;
}

int COXSHBListCtrl::SelectItem(int nItem) 
{ 
	ASSERT(nItem<GetItemCount());

	if(nItem==m_nSelectedItem)
		return m_nSelectedItem;

	int nOldItem=m_nSelectedItem;

	// In the case this control was created as a child window 
	// of shortcut bar group fill structure for notification of 
	// parent window of this shortcut bar
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_SELECTITEM;
	nmshb.hGroup=m_hGroup;
	nmshb.nItem=nItem;

	if(SendSHBNotification(&nmshb))
		return m_nSelectedItem;

	m_nSelectedItem=nItem; 

	return nOldItem;
}

int COXSHBListCtrl::SetHotItem(int nItem) 
{ 
	ASSERT(nItem<GetItemCount());

	if(nItem==m_nHotItem)
		return m_nHotItem;

	int nOldItem=m_nHotItem;

	// In the case this control was created as a child window 
	// of shortcut bar group fill structure for notification of 
	// parent window of this shortcut bar
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_HOTITEM;
	nmshb.hGroup=m_hGroup;
	nmshb.nItem=nItem;

	if(SendSHBNotification(&nmshb))
		return m_nHotItem;

	m_nHotItem=nItem; 
	m_nLastHotItem=nOldItem;

	return nOldItem;
}

int COXSHBListCtrl::ActivateItem(int nItem) 
{ 
	ASSERT(nItem<GetItemCount());

	if(nItem==m_nActiveItem)
	{
		return m_nActiveItem;
	}

	int nOldItem=m_nActiveItem;

	// In the case this control was created as a child window 
	// of shortcut bar group fill structure for notification of 
	// parent window of this shortcut bar
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_ACTIVATEITEM;
	nmshb.hGroup=m_hGroup;
	nmshb.nItem=nItem;

	if(SendSHBNotification(&nmshb))
	{
		return m_nActiveItem;
	}

	m_nActiveItem=nItem; 
	Update(nOldItem);
	Update(m_nActiveItem);

	return nOldItem;
}

int COXSHBListCtrl::SetDropTargetItem(int nItem) 
{ 
	ASSERT(nItem<GetItemCount());

	int nOldItem=m_nDropHilightItem;
	m_nDropHilightItem=nItem; 

	return nOldItem;
}

int COXSHBListCtrl::SetDragItem(int nItem) 
{ 
	ASSERT(nItem<GetItemCount());

	if(nItem==m_nDragItem)
		return m_nDragItem;

	int nOldItem=m_nDragItem;

	m_nDragItem=nItem; 

	if(nItem!=-1)
	{
		// In the case this control was created as a child window 
		// of shortcut bar group fill structure for notification of 
		// parent window of this shortcut bar
		NMSHORTCUTBAR nmshb;
		nmshb.hdr.code=SHBN_BEGINDRAGITEM;
		nmshb.hGroup=m_hGroup;
		nmshb.nItem=nItem;
		nmshb.lParam=(LPARAM)&m_ptDragImage;

		SendSHBNotification(&nmshb);

		// CF_TEXT format
		//
		// get our data ready to cache
		CString sText=GetItemText(nItem,0);
	
		// allocate our data object
		COleDataSource* pDataSource;
		pDataSource = new COleDataSource;
		
		// Load a CF_TEXT to the data object 
	    HGLOBAL hgCF_TEXTData=GlobalAlloc(GPTR,(sText.GetLength()+1)*sizeof(TCHAR));   
		BYTE* lpCF_TEXTData=(BYTE*)GlobalLock(hgCF_TEXTData);
                                                 
		lstrcpy((LPTSTR)lpCF_TEXTData,sText);
		GlobalUnlock(hgCF_TEXTData);                                                 

		// save the text data
		pDataSource->CacheGlobalData(CF_TEXT,hgCF_TEXTData);

		//////////////////////
		// internal COXShortcutBar::m_nChildWndItemFormat format
		//
		// (text) + (NULL) + lParam + (number of images elements) +
		// (SHBIMAGEDROPINFO + size of image list + ImageList) + 
		// (SHBIMAGEDROPINFO + size of image list + ImageList) + ...+
		// (size of additional information) + (additional information)
		//
		// in this case we use only two images (large and small)
		//

		// In the case this control was created as a child window 
		// of shortcut bar group fill structure for notification of 
		// parent window of this shortcut bar
		SHBADDITIONALDROPINFO shbadi;
		::ZeroMemory(&nmshb,sizeof(NMSHORTCUTBAR));
		nmshb.hdr.code=SHBN_GETADDITIONALDROPINFO;
		nmshb.hGroup=m_hGroup;
		nmshb.nItem=nItem;
		nmshb.lParam=(LPARAM)&shbadi;

		SendSHBNotification(&nmshb);

		// check the integrity
		ASSERT((shbadi.nBufferLength>0 && shbadi.pBuffer!=NULL) || 
			(shbadi.nBufferLength==0 && shbadi.pBuffer==NULL));


		// get image index
		int nImage=GetItemImage(nItem);

		DWORD nLargeImageSize=0;
		void* pilLargeBuffer=NULL;
		DWORD nSmallImageSize=0;
		void* pilSmallBuffer=NULL;

		// create image lists for large and small button correspondingly
		if(nImage>=0)
		{
			CImageList* pil=GetImageList(LVSIL_NORMAL);
			if(pil!=NULL)
				pilLargeBuffer=CopyImageFromIL(pil,nImage,nLargeImageSize);

			pil=GetImageList(LVSIL_SMALL);
			if(pil!=NULL)
				pilSmallBuffer=CopyImageFromIL(pil,nImage,nSmallImageSize);
		}

		int nEntryMemSize=(sText.GetLength()+1)*sizeof(TCHAR)+
			sizeof(DWORD)+sizeof(DWORD);
		DWORD nImageCount=0;
		if(nImage>=0)
		{
			if(nLargeImageSize>0)
			{
				nEntryMemSize+=sizeof(SHBIMAGEDROPINFO)+sizeof(DWORD)+nLargeImageSize;
				nImageCount++;
			}
			if(nSmallImageSize>0)
			{
				nEntryMemSize+=sizeof(SHBIMAGEDROPINFO)+sizeof(DWORD)+nSmallImageSize;
				nImageCount++;
			}
		}
		nEntryMemSize+=sizeof(DWORD)+shbadi.nBufferLength;

		// allocate the memory
		HGLOBAL hgItemData=GlobalAlloc(GPTR,nEntryMemSize);   
		BYTE* lpItemData=(BYTE*)GlobalLock(hgItemData);
         
		ZeroMemory(lpItemData,nEntryMemSize);

		// write item text into global memory
		lstrcpy((LPTSTR)lpItemData,sText);
		lpItemData+=(sText.GetLength()+1)*sizeof(TCHAR);

		// write item lParam into global memory
		(*(DWORD*)lpItemData)=(DWORD)GetItemData(nItem);
		lpItemData+=sizeof(DWORD);

		// write the item image info to gloabal memory
		//

		// set number of images associated with the item
		(*(DWORD*)lpItemData)=nImageCount;
		lpItemData+=sizeof(DWORD);

		if(nLargeImageSize>0)
		{
			// fill special image info structure
			SHBIMAGEDROPINFO imageInfo;
			// our image is the "NORMAL" one
			imageInfo.imageType=SHBIT_NORMAL;
			imageInfo.imageState=SHBIS_LARGE;

			// write SHBIMAGEDROPINFO structure
			memcpy((void*)lpItemData,(void*)&imageInfo,sizeof(SHBIMAGEDROPINFO));
			lpItemData+=sizeof(SHBIMAGEDROPINFO);

			// write the size of image
			(*(DWORD*)lpItemData)=nLargeImageSize;
			lpItemData+=sizeof(DWORD);

			// write image list
			memcpy((void*)lpItemData,pilLargeBuffer,nLargeImageSize);
			lpItemData+=nLargeImageSize;
		}

		if(nSmallImageSize>0)
		{
			// fill special image info structure
			SHBIMAGEDROPINFO imageInfo;
			// our image is the "NORMAL" one
			imageInfo.imageType=SHBIT_NORMAL;
			imageInfo.imageState=SHBIS_SMALL;

			// write SHBIMAGEDROPINFO structure
			memcpy((void*)lpItemData,(void*)&imageInfo,sizeof(SHBIMAGEDROPINFO));
			lpItemData+=sizeof(SHBIMAGEDROPINFO);

			// write the size of image
			(*(DWORD*)lpItemData)=nSmallImageSize;
			lpItemData+=sizeof(DWORD);

			// write image list
			memcpy((void*)lpItemData,pilSmallBuffer,nSmallImageSize);
			lpItemData+=nSmallImageSize;
		}


		// write the additional item info to gloabal memory
		//

		// set the amount of bytes of additional info
		(*(DWORD*)lpItemData)=shbadi.nBufferLength;
		lpItemData+=sizeof(DWORD);
		if(shbadi.nBufferLength>0)
		{
			// write additional info
			memcpy((void*)lpItemData,shbadi.pBuffer,shbadi.nBufferLength);
			lpItemData+=shbadi.nBufferLength;
		}


		GlobalUnlock(hgItemData); 
	
		// save the item data
		pDataSource->CacheGlobalData(COXShortcutBar::m_nChildWndItemFormat,hgItemData);

		if(pilLargeBuffer!=NULL)
			free(pilLargeBuffer);
		if(pilSmallBuffer!=NULL)
			free(pilSmallBuffer);
		
		// post special message to handle drag'n'drop operation
		PostMessage(SHBLCM_HANDLEDRAG,(WPARAM)nItem,(LPARAM)pDataSource);
	}

	return nOldItem;
}

BOOL COXSHBListCtrl::Update(int nItem)
{
	if(nItem<0 || nItem>=GetItemCount())
		return FALSE;

	// get the item rect
	CRect rectItem;
	VERIFY(m_mapItemToBoundRect.Lookup(nItem,rectItem));
	// adjust it
	rectItem-=m_ptOrigin;

	CRect rect;
	if(m_mapItemToImageRect.Lookup(nItem,rect))
	{
		rect+=rectItem.TopLeft();
		// redraw image rect
		RedrawWindow(rect);
	}

	VERIFY(m_mapItemToTextRect.Lookup(nItem,rect));
	rect+=rectItem.TopLeft();
	// redraw text rect
	RedrawWindow(rect);

	return TRUE;
}

BOOL COXSHBListCtrl::RedrawItems(int nFirst, int nLast)
{
	for(int nIndex=nFirst; nIndex<=nLast; nIndex++)
		if(!Update(nIndex))
			return FALSE;

	return TRUE;
}

void COXSHBListCtrl::AnticipateMouseMessages()
{
	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);

	// test the cursor position
	UINT nHitFlags;
	int nItem=HitTest(point,&nHitFlags);
	if(!m_bMouseIsOver || (nItem!=-1 && 
		(nHitFlags==SHBLC_ONITEM || nHitFlags==SHBLC_ONIMAGEANDTEXT)))
	{
		nItem=-1;
	}
	int nOldHotItem=SetHotItem(nItem);
	
	if(GetDragItem()==-1 && nItem!=-1 && 
		(GetBarStyle()&SHBS_DISABLEDRAGITEM)==0 && m_nSuspectDragItem==nItem)
	{
		if(abs(point.x-m_ptClickedLButton.x)+abs(point.y-m_ptClickedLButton.y)>8)
		{
			m_ptDragImage+=point-m_ptClickedLButton;
			// assign item being dragged
			SetDragItem(nItem);
		}
	}

	// reset suspect drag item index
	if(!IsLButtonDown() || GetDragItem()!=-1)
		m_nSuspectDragItem=-1;

	// update changed items 
	//
	if(nItem!=-1)
		Update(nItem);
	if(nItem!=nOldHotItem && nOldHotItem!=-1)
		Update(nOldHotItem);

	// update scroll button images 
	//
	if(m_bScrollingUp && nHitFlags!=SHBLC_ONTOPSCROLLBUTTON)
		RedrawWindow(m_rectTopScrollButton);
	if(m_bScrollingDown && nHitFlags!=SHBLC_ONBOTTOMSCROLLBUTTON)
		RedrawWindow(m_rectBottomScrollButton);

	// check autoscrolling
	if((GetBarStyle()&SHBS_AUTOSCROLL)!=0 && GetDragItem()==-1)
	{
		if(!m_bScrollingUp && nHitFlags==SHBLC_ONTOPSCROLLBUTTON)
		{
			m_bAutoScrolling=TRUE;
			StartScrolling(TRUE);
		}
		else if(!m_bScrollingDown && nHitFlags==SHBLC_ONBOTTOMSCROLLBUTTON)
		{
			m_bAutoScrolling=TRUE;
			StartScrolling(FALSE);
		}
	}

	// check conditions to stop autoscrolling
	if(m_bAutoScrolling && ((m_bScrollingUp && nHitFlags!=SHBLC_ONTOPSCROLLBUTTON) || 
		(m_bScrollingDown && nHitFlags!=SHBLC_ONBOTTOMSCROLLBUTTON)))
	{
		StopScrolling();
	}
}

BOOL COXSHBListCtrl::Scroll(CSize size)
{
	// origin mustn't be negative
	ASSERT(m_ptOrigin.x>=0 || m_ptOrigin.y>=0);

	if(GetItemCount()==0)
	{
		return FALSE;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	if(rectClient.IsRectEmpty())
	{
		return FALSE;
	}
	// deflate client rect to leave some space free of items
	rectClient.DeflateRect(
		ID_EDGELEFTMARGIN,ID_EDGETOPMARGIN,ID_EDGERIGHTMARGIN,ID_EDGEBOTTOMMARGIN);

	// get current top index
	int nTopItem=GetTopIndex();
	ASSERT(nTopItem>=0 && nTopItem<GetItemCount());
	// scroll up
	if(size.cy<0)
		nTopItem=(nTopItem>0) ? nTopItem-1 : nTopItem;
	else 	
	// scroll down
	if(size.cy>0)
		nTopItem=(nTopItem==GetItemCount()-1) ? nTopItem : nTopItem+1;

	// get scrollbale rect to check validility of scroll operation
	CRect rectScrollable;
	if(!GetScrollableRect(rectScrollable,nTopItem))
		return FALSE;

	CRect rectAggregate=rectClient;
	rectAggregate+=m_ptOrigin;
	rectAggregate+=size;

	// scroll up
	if(size.cy<0)
	{
		int nTopMargin=rectAggregate.top-rectScrollable.top;
		if(nTopMargin<=size.cy)
		{
			size.cy=0;
		}
		else
		{
			size.cy-=(nTopMargin<0) ? nTopMargin : 0;
			ASSERT(size.cy<0);
		}
	}
	else 	
	// scroll down
	if(size.cy>0)
	{
		int nBottomMargin=rectAggregate.bottom-rectScrollable.bottom;
		if(nBottomMargin>=size.cy)
			size.cy=0;
		else
		{
			size.cy-=(nBottomMargin>0) ? nBottomMargin : 0;
			ASSERT(size.cy>0);
		}
	}

	// scroll left
	if(size.cx<0)
	{
		int nLeftMargin=rectAggregate.left-rectScrollable.left;
		if(nLeftMargin<=size.cx)
		{
			size.cx=0;
		}
		else
		{
			size.cx-=(nLeftMargin<0) ? nLeftMargin : 0;
			ASSERT(size.cx<0);
		}
	}
	else 	
	// scroll right
	if(size.cx>0)
	{
		int nRightMargin=rectAggregate.right-rectScrollable.right;
		if(nRightMargin>=size.cx)
		{
			size.cx=0;
		}
		else
		{
			size.cx-=(nRightMargin>0) ? nRightMargin : 0;
			ASSERT(size.cx>0);
		}
	}

	if(size.cx==0 && size.cy==0)
		return FALSE;

	// update view origin
	m_ptOrigin.x+=size.cx;
	m_ptOrigin.y+=size.cy;

	// reposition items and redraw the control
	CalculateScrollRects();
	RedrawWindow();

	return TRUE;
}

int COXSHBListCtrl::FindItem(LV_FINDINFO* pFindInfo, int nStart/* = -1*/) const
{
	int nCount=GetItemCount();
	if(nCount==0)
		return -1;

	// verify nStart argument
	ASSERT(nStart+1>=0 && nStart<nCount);

	int nItem=-1;

	// only if searching by coordinates specified we provide our own implementation
	if(pFindInfo->flags&LVFI_NEARESTXY)
	{
		CPoint point=pFindInfo->pt;
		// adjuxt the coordinates
		point+=m_ptOrigin;

		int nNearestDistance=-1;
		int nNearestItem=-1;
		CRect rect;

		for(int nIndex=nStart+1; nIndex<nCount+
			((pFindInfo->flags&LVFI_WRAP) ? nStart+1 : 0); nIndex++)
		{
			nItem=nIndex;
			if(nItem>=nCount)
				nItem-=nCount;

			VERIFY(m_mapItemToBoundRect.Lookup(nItem,rect));
			// bottom mustn't be include (the same way PtInRect works)
			if(rect.PtInRect(point) || (point.y>=rect.top && point.y<rect.bottom))
			{
				// if the point is in entire item rect then the search is completed
				nNearestItem=nItem;
				break;
			}

			int nMargin=-1;
			switch(pFindInfo->vkDirection)
			{
			// search for upper items
			case VK_UP:
				{
					nMargin=point.y-rect.bottom;
					break;
				}
			// search for down items
			case VK_DOWN:
				{
					nMargin=rect.top-point.y;
					break;
				}
			}
			if(nMargin>0 && (nNearestDistance==-1 || nNearestDistance>nMargin))
			{
				nNearestDistance=nMargin;
				nNearestItem=nItem;
			}
		}

		nItem=nNearestItem;
	}
	else
		nItem=CListCtrl::FindItem(pFindInfo,nStart);


	return nItem;
}

int COXSHBListCtrl::GetTopIndex() const
{
	LV_FINDINFO lvfi;
	lvfi.flags=LVFI_NEARESTXY;
	lvfi.pt.x=0;
	lvfi.pt.y=ID_EDGETOPMARGIN;
	lvfi.vkDirection=VK_DOWN;

	return FindItem(&lvfi);
}

BOOL COXSHBListCtrl::EnsureVisible(int nItem, BOOL bPartialOK)
{
	if(nItem<0 || nItem>=GetItemCount())
		return FALSE;

	// entire item rectangle
	CRect rect;
	VERIFY(m_mapItemToBoundRect.Lookup(nItem,rect));

	// visible view rectangle
	CRect rectViewVisible;
	GetViewRect(rectViewVisible,TRUE);

	// if entire item rectangle height is more the than the visible rectangle height
	// and partial visibility is not Ok then we've got Mission Impossible
	if(rect.Height()>=rectViewVisible.Height() && !bPartialOK)
		return FALSE;

	// if their intersection is not empty and partial visibility is Ok 
	// then the mission is completed
	CRect rectIntersection;
	if(rectIntersection.IntersectRect(rectViewVisible,rect) && bPartialOK)
		return TRUE;

	// if their intersection is equal to the entire item rectangle
	// then the mission is completed either
	if(rectIntersection==rect)
		return TRUE;

	// scroll the control to update view origin
	//
	if(rect.bottom<rectViewVisible.top)
		Scroll(CSize(0,rect.top-rectViewVisible.top));
	else if(rect.top>rectViewVisible.bottom)
		Scroll(CSize(0,rect.bottom-rectViewVisible.bottom));
	else if(rect.bottom>rectViewVisible.bottom)
		Scroll(CSize(0,rect.bottom-rectViewVisible.bottom));
	else if(rect.top<rectViewVisible.top)
		Scroll(CSize(0,rect.top-rectViewVisible.top));

	if(bPartialOK)
		return TRUE;

	GetViewRect(rectViewVisible,TRUE);

	// check if size of item is more than visible rect
	if(rectIntersection.IntersectRect(rectViewVisible,rect) && rectIntersection==rect)
		return TRUE;
	else
		return FALSE;
}

COXSHBEdit* COXSHBListCtrl::EditLabel(int nItem)
{
	ASSERT(nItem>=0 && nItem<GetItemCount());
	// validate edit control
	ASSERT(::IsWindow(m_edit.GetSafeHwnd()));

	// check if editing is allowed
	if((GetBarStyle()&SHBS_EDITITEMS)==0)
	{
		TRACE(_T("COXSHBListCtrl::EditLabel: cannot edit item - SHBS_EDITITEMS style wasn't set"));
		return NULL;
	}

	// finish edit if any is undergoing
	if(m_nEditItem!=-1)
	{
		m_edit.FinishEdit(TRUE);
		m_nEditItem=-1;
	}

	CString sText=GetItemText(nItem,0);
	
	// In the case this control was created as a child window 
	// of shortcut bar group fill structure for notification of 
	// parent window of this shortcut bar
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_BEGINITEMEDIT;
	nmshb.hGroup=m_hGroup;
	nmshb.nItem=nItem;
	nmshb.lParam=(LPARAM)((LPCTSTR)sText);

	// check if parent doesn't let us to edit item text
	if(SendSHBNotification(&nmshb))
		return NULL;

	CRect rectClient;
	GetClientRect(rectClient);
	rectClient.DeflateRect(
		ID_EDGELEFTMARGIN,ID_EDGETOPMARGIN,ID_EDGERIGHTMARGIN,ID_EDGEBOTTOMMARGIN);

	CRect rectText;
	if(!GetItemRect(nItem,rectText,LVIR_LABEL))
	{
		TRACE(_T("COXSHBListCtrl::EditLabel: failed to get item text rectangle"));
		return NULL;
	}

	if(rectText.IsRectEmpty())
	{
		rectText.SetRectEmpty();

		CRect rectImage;
		if(!GetItemRect(nItem,rectImage,LVIR_ICON))
		{
			TRACE(_T("COXSHBListCtrl::EditLabel: failed to get item image rectangle"));
			return NULL;
		}

		CRect rectItem;
		if(!GetItemRect(nItem,rectItem,SHBLC_ENTIREITEM))
		{
			TRACE(_T("COXSHBListCtrl::EditLabel: failed to get full item rectangle"));
			return NULL;
		}

		rectText=AdjustTextRect(_T(" "),rectImage,rectItem.Width());

		ASSERT(!rectText.IsRectEmpty());
	}

	CRect rectIntersect;
	if(!rectIntersect.IntersectRect(rectClient,rectText) || 
		rectIntersect!=rectText)
	{
		TRACE(_T("COXSHBListCtrl::EditLabel: failed to edit - item text rectangle is not entirely visible"));
		return NULL;
	}

	int nView=GetView();

	// make some adjustments to make our edit rect look prettier
	if(nView==SHB_SMALLICON)
	{
		rectText.OffsetRect(-ID_TEXTLEFTMARGIN,0);
		VERIFY(rectIntersect.IntersectRect(rectClient,rectText));
	}

	// get font to use in edit control
	CFont font;
	LPLOGFONT pLF=NULL;
	LOGFONT lf;
	if(m_pShortcutBar)
	{
		pLF=m_pShortcutBar->GetGroupTextFont(m_hGroup);
	}
	else
	{
		CFont* pFont=GetFont();
		ASSERT(pFont!=NULL);
		VERIFY(pFont->GetLogFont(&lf));
		pLF=&lf;
	}
	ASSERT(pLF!=NULL);
	VERIFY(font.CreateFontIndirect(pLF));

	CRect rectMax;
	CRect rectMin;

	// fill SHBE_DISPLAY structure to fill editing properties depending on view
	SHBE_DISPLAY shbed;
	shbed.nMask=SHBEIF_TEXT|SHBEIF_RECTDISPLAY|SHBEIF_RECTMAX|SHBEIF_STYLE|
		SHBEIF_ENLARGE|SHBEIF_SELRANGE|SHBEIF_FONT|SHBEIF_RECTMIN;
	shbed.lpText=sText;
	shbed.rectDisplay=rectText;
	if(nView==SHB_LARGEICON)
	{
		shbed.dwStyle=WS_BORDER|ES_CENTER|ES_MULTILINE|ES_AUTOVSCROLL;
		rectMax=rectClient;
		rectMax.top=rectText.top;

		rectMin=rectText;
		if(rectText.Width()<ID_EDITMINWIDTH)
		{
			rectMin.left=(rectText.left+rectText.right-ID_EDITMINWIDTH)/2;
			rectMin.right=rectMin.left+ID_EDITMINWIDTH;

			if(rectMin.left<rectMax.left)
				rectMax.left=rectMin.left;
			if(rectMin.right>rectMax.right)
				rectMax.right=rectMin.right;
		}
	}
	else if(nView==SHB_SMALLICON)
	{
		shbed.dwStyle=WS_BORDER|ES_AUTOHSCROLL|ES_LEFT;
		rectMax=rectText;
		rectMax.right=rectClient.right;

		rectMin=rectText;
		if(rectText.Width()<ID_EDITMINWIDTH)
		{
			rectMin.right=rectMin.left+ID_EDITMINWIDTH;

			if(rectMin.right>rectMax.right)
				rectMax.right=rectMin.right;
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	shbed.rectMax=rectMax;
	shbed.rectMin=rectMin;
	shbed.bEnlargeAsTyping=TRUE;
	shbed.ptSelRange=CPoint(0,-1);
	shbed.pFont=&font;

	if(!m_edit.StartEdit(&shbed))
	{
		TRACE(_T("COXSHBListCtrl::EditLabel: COXSHBEdit::StartEdit failed"));
		return NULL;
	}

	m_nEditItem=nItem;

	return &m_edit;
}

COLORREF COXSHBListCtrl::GetBkColor() const
{
	if(m_pShortcutBar)
	{
		ASSERT(m_hGroup);

		COLORREF clrBackground=m_pShortcutBar->GetGroupBkColor(m_hGroup);
		ASSERT(clrBackground!=ID_COLOR_NONE);

		return clrBackground;
	}
	else
	{
		return CListCtrl::GetBkColor();
	}
}

BOOL COXSHBListCtrl::HasBkColor() const
{
	if(m_pShortcutBar)
	{
		ASSERT(m_hGroup);
		return m_pShortcutBar->HasGroupBkColor(m_hGroup);
	}
	else
		return FALSE;
}

BOOL COXSHBListCtrl::SetBkColor(COLORREF clr)
{
	if(m_pShortcutBar)
	{
		ASSERT(m_hGroup);
		return m_pShortcutBar->SetGroupBkColor(m_hGroup,clr);
	}
	else
		return CListCtrl::SetBkColor(clr);
}

COLORREF COXSHBListCtrl::GetTextColor() const
{
	if(m_pShortcutBar)
	{
		ASSERT(m_hGroup);
	
		COLORREF clrText=m_pShortcutBar->GetGroupTextColor(m_hGroup);
		ASSERT(clrText!=ID_COLOR_NONE);

		return clrText;
	}
	else
		return CListCtrl::GetTextColor();
}

BOOL COXSHBListCtrl::SetTextColor(COLORREF clr)
{
	if(m_pShortcutBar)
	{
		ASSERT(m_hGroup);
		return m_pShortcutBar->SetGroupTextColor(m_hGroup,clr);
	}
	else
		return CListCtrl::SetTextColor(clr);
}

COLORREF COXSHBListCtrl::GetTextBkColor() const
{
	if(m_pShortcutBar)
		return GetBkColor();
	else
		return CListCtrl::GetTextBkColor();
}

BOOL COXSHBListCtrl::SetTextBkColor(COLORREF clr)
{
	if(m_pShortcutBar)
	{
		return FALSE;
	}
	else
		return CListCtrl::SetTextBkColor(clr);
}

int COXSHBListCtrl::GetCountPerPage() const
{
	// result of this function depends on current origin and may vary
	//

	CRect rectView;
	GetViewRect(rectView,TRUE);

	int nCount=0;

	if(!rectView.IsRectEmpty())
	{
		CRect rect;
		int nItem;
		POSITION pos=m_mapItemToBoundRect.GetStartPosition();
		while(pos!=NULL)
		{
			m_mapItemToBoundRect.GetNextAssoc(pos,nItem,rect);

			ASSERT(nItem>=0 && nItem<GetItemCount());

			if(rect.IntersectRect(rectView,rect))
				nCount++;
		}
	}

	return nCount;
}

CImageList* COXSHBListCtrl::CreateDragImage(int nItem, LPPOINT lpPoint /*= NULL*/)
{
	UNREFERENCED_PARAMETER(lpPoint);
	ASSERT(nItem>=0 && nItem<GetItemCount());

	m_bCreatingDragImage=TRUE;

	// IMPORTANT //
	// it's caller responsibility to delete created image list
	CImageList* m_pDragImageList=new CImageList;

	CRect rectImageText;
	VERIFY(GetItemRect(nItem,&rectImageText,LVIR_BOUNDS));
	CRect rectItem;
	VERIFY(GetItemRect(nItem,&rectItem,SHBLC_ENTIREITEM));

	// create image list
	m_pDragImageList->Create(rectImageText.Width(),rectImageText.Height(),TRUE,0,1);

	CClientDC dcClient(this);
	CDC dc;
	VERIFY(dc.CreateCompatibleDC(&dcClient));
	CDC dcCopy;
	VERIFY(dcCopy.CreateCompatibleDC(&dcClient));
	
	CBitmap bitmapItem;
	VERIFY(bitmapItem.CreateCompatibleBitmap(&dcClient,rectItem.Width(),
		rectItem.Height()));
	CBitmap* pOldBitmap=dc.SelectObject(&bitmapItem);

	rectImageText-=rectItem.TopLeft();
	rectItem-=rectItem.TopLeft();

	DRAWITEMSTRUCT dis;
	dis.CtlType=ODT_LISTVIEW;
	dis.CtlID=GetDlgCtrlID();
	dis.itemID=nItem;
	dis.itemAction=ODA_DRAWENTIRE;
	dis.itemState=ODS_DEFAULT;
	dis.hwndItem=GetSafeHwnd();
	dis.hDC=dc;
	dis.rcItem=rectItem;
	dis.itemData=(DWORD)0;

	// use DawItem to create drag image
	DrawItem(&dis);

	CBitmap bitmapImageText;
	VERIFY(bitmapImageText.CreateCompatibleBitmap(&dcClient,rectImageText.Width(),
		rectImageText.Height()));
	CBitmap* pOldCopyBitmap=dcCopy.SelectObject(&bitmapImageText);

	dcCopy.BitBlt(0,0,rectImageText.Width(),rectImageText.Height(),
		&dc,rectImageText.left,rectImageText.top,SRCCOPY);
	if(pOldCopyBitmap)
		dcCopy.SelectObject(pOldCopyBitmap);

	if(pOldBitmap)
		dc.SelectObject(pOldBitmap);

	VERIFY(m_pDragImageList->Add(&bitmapImageText,GetBkColor())!=-1);

	m_bCreatingDragImage=FALSE;

	return m_pDragImageList;
}

void COXSHBListCtrl::StartScrolling(BOOL bScrollingUp)
{
	if(m_bScrollingUp || m_bScrollingDown)
		StopScrolling();

	ASSERT(m_nScrollTimerID==0);
	ASSERT(!m_bScrollingUp && !m_bScrollingDown);

	ASSERT(GetItemCount()>0);

	int nOldSelectedItem=SelectItem(-1);
	if(nOldSelectedItem!=-1)
		Update(nOldSelectedItem);

	CRect rectClient;
	GetClientRect(rectClient);
	// deflate the client rectangle to match the valid area where we can
	// display items
	rectClient.DeflateRect(ID_EDGELEFTMARGIN,ID_EDGETOPMARGIN,ID_EDGERIGHTMARGIN,
		ID_EDGEBOTTOMMARGIN);
	ASSERT(rectClient.top<rectClient.bottom);

	// if any drag'n'drop operation is active and the control has launched it
	// then we need to apply some additional effort to redraw things smoothly
	if(m_bDragDropOperation && (GetBarStyle()&SHBS_DRAWITEMDRAGIMAGE)!=0 && 
		m_bDragDropOwner)
	{
		ASSERT(m_pDragImage);
		// unlock window updates
		VERIFY(m_pDragImage->DragShowNolock(FALSE));
	}

	if(bScrollingUp)
	{
		m_bScrollingUp=TRUE;
		RedrawWindow(m_rectTopScrollButton);

		if(!m_bDragDropOperation)
		{
			// scroll up once
			CRect rect;

			int nItem=GetTopIndex();
			if(nItem==-1)
				nItem=GetItemCount()-1;
			else 
			{
				VERIFY(m_mapItemToBoundRect.Lookup(nItem,rect));
				if(m_ptOrigin.y+rectClient.top<=rect.top)
				{
					ASSERT(nItem>0);
					nItem--;
				}
			}

			ASSERT((nItem>=0 && nItem<GetItemCount()));

			VERIFY(m_mapItemToBoundRect.Lookup(nItem,rect));
			int nOffset=rect.top-rectClient.top-m_ptOrigin.y;
			Scroll(CSize(0,nOffset));
		}
	}
	else 
	{
		m_bScrollingDown=TRUE;
		RedrawWindow(m_rectBottomScrollButton);

		if(!m_bDragDropOperation)
		{
			// scroll down once
			CRect rect;

			int nItem=GetTopIndex();
			ASSERT(nItem>=0 && nItem<GetItemCount());

			if(nItem==GetItemCount()-1)
			{
				VERIFY(m_mapItemToBoundRect.Lookup(nItem,rect));
				int nOffset=rect.bottom-rectClient.bottom-m_ptOrigin.y;
				Scroll(CSize(0,nOffset));
			}
			else
			{
				nItem++;
				VERIFY(m_mapItemToBoundRect.Lookup(nItem,rect));
				int nOffset=rect.top-rectClient.top-m_ptOrigin.y;
				Scroll(CSize(0,nOffset));
			}
		}
	}

	if(m_bDragDropOperation && (GetBarStyle()&SHBS_DRAWITEMDRAGIMAGE)!=0 && 
		m_bDragDropOwner)
	{
		ASSERT(m_pDragImage);
		// lock window updates
		VERIFY(m_pDragImage->DragShowNolock(TRUE));
	}

	// set timer
	//
	UINT nDelay=(m_bAutoScrolling ? GetAutoScrollingDelay() : GetScrollingDelay());
	// adjustment for drag'n'drop operation
	if(m_bDragDropOperation)
		nDelay/=4;
	m_nScrollTimerID=SetTimer(ID_SCROLLINGTIMER,nDelay,NULL);
	ASSERT(m_nScrollTimerID!=0);
}

void COXSHBListCtrl::StopScrolling()
{
	ASSERT(m_nScrollTimerID!=0);

	// kill timer
	KillTimer(m_nScrollTimerID);
	m_nScrollTimerID=0;

	// redraw scroll buttons
	//

	if(!m_rectTopScrollButton.IsRectEmpty())
		RedrawWindow(m_rectTopScrollButton);

	if(!m_rectBottomScrollButton.IsRectEmpty())
		RedrawWindow(m_rectBottomScrollButton);

	m_bScrollingUp=FALSE;
	m_bScrollingDown=FALSE;
	m_bAutoScrolling=FALSE;
}

BOOL COXSHBListCtrl::InitListControl()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	// create edit control used to edit header text
	if(!CreateEditControl())
	{
		TRACE(_T("COXSHBListCtrl::InitListControl: failed to create edit control"));
		return FALSE;
	}

	// tooltips is always enabled
	EnableToolTips(TRUE);

	// register OLE Drag'n'Drop
	COleDropTarget* pOleDropTarget=GetDropTarget();
	ASSERT(pOleDropTarget!=NULL);
	if(!pOleDropTarget->Register(this))
	{
		TRACE(_T("COXSHBListCtrl::InitListControl: failed to register the control with COleDropTarget\n"));
		TRACE(_T("COXSHBListCtrl: you've probably forgot to initialize OLE libraries using AfxOleInit function"));
	}

	return TRUE;
}

COleDropSource* COXSHBListCtrl::GetDropSource() 
{ 
	// In the case this control was created as a child window 
	// of shortcut bar group fill structure for notification of 
	// parent window of this shortcut bar
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_GETDROPSOURCE;
	nmshb.hGroup=m_hGroup;
	nmshb.lParam=(LPARAM)NULL;

	COleDropSource* pOleDropSource=NULL;
	// check if parent want to set its own drop target object
	if(SendSHBNotification(&nmshb) && nmshb.lParam!=NULL)
		pOleDropSource=(COleDropSource*)nmshb.lParam;
	else
	{
		// we have to call this function, otherwise assertions will happen all
		// the time COXSHBDropSource specific message will be fired
		m_oleDropSource.SetOwner(this);
		pOleDropSource=&m_oleDropSource;
	}

	ASSERT(pOleDropSource!=NULL);

	return pOleDropSource; 
}

COleDropTarget* COXSHBListCtrl::GetDropTarget() 
{ 
	// In the case this control was created as a child window 
	// of shortcut bar group fill structure for notification of 
	// parent window of this shortcut bar
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_GETDROPTARGET;
	nmshb.hGroup=m_hGroup;
	nmshb.lParam=(LPARAM)NULL;

	COleDropTarget* pOleDropTarget=NULL;
	// check if parent want to set its own drop target object
	if(SendSHBNotification(&nmshb) && nmshb.lParam!=NULL)
		pOleDropTarget=(COleDropTarget*)nmshb.lParam;
	else
		pOleDropTarget=(COleDropTarget*)&m_oleDropTarget;

	ASSERT(pOleDropTarget!=NULL);

	return pOleDropTarget; 
}

CRect COXSHBListCtrl::GetGapBetweenItems(int nItemBefore)
{
	ASSERT(nItemBefore>=0 && nItemBefore<=GetItemCount());

	// returns the gap before nItemBefore item
	//

	CRect rect;
	GetClientRect(rect);
	// if the requested gap is between top border and top item or
	// bottom border and bottom item
	if(nItemBefore==GetItemCount() || nItemBefore==0)
	{
		if(GetItemCount()==0 || nItemBefore==0)
			rect.bottom=rect.top+ID_EDGETOPMARGIN;
		else
		{
			CRect rectItem;
			VERIFY(GetItemRect(nItemBefore-1,rectItem,SHBLC_ENTIREITEM));
			rect.top=rectItem.bottom+m_ptOrigin.y;
			rect.bottom=rect.top+ID_EDGEBOTTOMMARGIN;
		}
	}
	// if the requested gap is between items
	else
	{
		CRect rectItem;
		VERIFY(GetItemRect(nItemBefore,rectItem,SHBLC_ENTIREITEM));
		rect.bottom=rectItem.top+m_ptOrigin.y;
		rect.top=rect.bottom-ID_ITEMMARGIN;
	}

	return rect;
}

CRect COXSHBListCtrl::GetPlaceHolderRect(int nItemBefore)
{
	ASSERT(nItemBefore>=0 && nItemBefore<=GetItemCount());

	// place holder rectangle must lay around or within the gap between items
	CRect rect=GetGapBetweenItems(nItemBefore);
	rect-=m_ptOrigin;

	// if the requested place holder is between top border and top item or
	// bottom border and bottom item
	if(nItemBefore==GetItemCount() || nItemBefore==0)
	{
		if(GetItemCount()==0 || nItemBefore==0)
			rect.bottom=rect.top+2*ID_HEIGHTPLACEHOLDER/3;
		else
			rect.top=rect.bottom-2*ID_HEIGHTPLACEHOLDER/3;
	}
	// if the requested place holder is between items
	else
	{
		int nGapHeight=rect.Height();
		rect.bottom+=ID_HEIGHTPLACEHOLDER/2+ID_HEIGHTPLACEHOLDER%2-nGapHeight/2;
		rect.top-=ID_HEIGHTPLACEHOLDER/2-nGapHeight/2;
	}

	return rect;
}

int COXSHBListCtrl::GetItemImage(const int nItem)
{
	ASSERT(nItem>=0 && nItem<GetItemCount());

	// image list used to display item depends on type of view
	int nView=GetView();

	// get corresponding item list to show items
	CImageList* pil=NULL;
	if(nView==SHB_LARGEICON)
		pil=GetImageList(LVSIL_NORMAL);
	else if(nView==SHB_SMALLICON)
		pil=GetImageList(LVSIL_SMALL);
	
	// if there is no any image list associated with the control then always return -1
	if(pil==NULL)
		return -1;

	LV_ITEM lvi;

	lvi.mask=LVIF_IMAGE;
	lvi.iItem=nItem;
	lvi.iSubItem=0;
	VERIFY(GetItem(&lvi));

	if(lvi.iImage<pil->GetImageCount())
		return lvi.iImage;
	else
		return -1;
}

HRESULT COXSHBListCtrl::GetComCtlVersion(LPDWORD pdwMajor, LPDWORD pdwMinor)
{
	if(IsBadWritePtr(pdwMajor, sizeof(DWORD)) || 
		IsBadWritePtr(pdwMinor, sizeof(DWORD)))
	{
		return E_INVALIDARG;
	}

	// get handle of the common control DLL
	BOOL bAlreadyLoaded=TRUE;
	HINSTANCE hComCtl=::GetModuleHandle(_T("comctl32.dll"));
	if(hComCtl==NULL)
	{
		// load the DLL
		hComCtl=::LoadLibrary(_T("comctl32.dll"));
		bAlreadyLoaded=FALSE;
	}

	if(hComCtl)
	{
		HRESULT           hr=S_OK;
		DLLGETVERSIONPROC pDllGetVersion;
   
		/*
		You must get this function explicitly because earlier versions of the DLL 
		don't implement this function. That makes the lack of implementation of the 
		function a version marker in itself.
		*/
		pDllGetVersion=(DLLGETVERSIONPROC)GetProcAddress(hComCtl, "DllGetVersion");
   
		if(pDllGetVersion)
		{
			DLLVERSIONINFO dvi;
      
			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize=sizeof(dvi);
   
			hr = (*pDllGetVersion)(&dvi);
      
			if(SUCCEEDED(hr))
			{
				*pdwMajor = dvi.dwMajorVersion;
				*pdwMinor = dvi.dwMinorVersion;
			}
			else
			{
				hr = E_FAIL;
			}   
		}
		else
		{
			// If GetProcAddress failed, then the DLL is a version previous 
			// to the one shipped with IE 3.x.
			*pdwMajor = 4;
			*pdwMinor = 0;
		}
   
		if(!bAlreadyLoaded)
			::FreeLibrary(hComCtl);

		return hr;
	}

	return E_FAIL;
}


/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// COXShortcutBar

IMPLEMENT_DYNCREATE(COXShortcutBar, CWnd)

COXShortcutBar::COXShortcutBar()
{
	m_pImageList=NULL;

	m_hDropHilightGroup=NULL;
	m_hExpandedGroup=NULL;
	m_hEditGroup=NULL;
	m_hHotGroup=NULL;
	m_hPressedGroup=NULL;
	m_hDragGroup=NULL;
	m_pShortcutBarSkin = NULL;
	
	m_nSortOrder=0;
	m_dwBarStyle=0;

	m_rectChildWnd.SetRectEmpty();

	m_nLastHandle=0;

	// create default font used to draw text in headers
	if((HFONT)m_fontDefault==NULL)
	{
		if(!m_fontDefault.CreateFont(
			-11,0,0,0,FW_REGULAR,0,0,0,0,0,0,0,0,_T("MS Sans Serif")))
		{
			TRACE(_T("COXShortcutBar::COXShortcutBar: failed to create default font"));
			ASSERT(FALSE);
			AfxThrowMemoryException();
		}
	}

	m_bCreatingDragImage=FALSE;
	m_pDragImage=NULL;

	m_nGroupMargin=DFLT_GROUPMARGIN;
	m_rectChildWndMargins=CRect(DFLT_LEFTMARGIN,DFLT_TOPMARGIN,
		DFLT_RIGHTMARGIN,DFLT_BOTTOMMARGIN);

	m_sizeMin=CSize(DFLT_MINWIDTH,DFLT_MINHEIGHT);

	m_sizeScrollButton=CSize(DFLT_SCROLLBUTTONWIDTH,DFLT_SCROLLBUTTONHEIGHT);

	m_nScrollingDelay=DFLT_SCROLLINGDELAY;
	m_nAutoScrollingDelay=DFLT_AUTOSCROLLINGDELAY;

	m_nAutoExpandDelay=DFLT_WAITFORAUTOEXPAND;

	m_nWaitForDragHeaderID=0;
	m_hSuspectDragGroup=NULL;
	m_nWaitForAutoExpandID=0;
	m_hSuspectAutoExpandGroup=NULL;

	m_nCheckMouseIsOverGroupID=0;
	m_hLastMouseIsOverGroup=NULL;
}


COXShortcutBar::~COXShortcutBar()
{
	HSHBGROUP hGroup;

	// if window is not destroyed yet then dlete all groups in the way the parent
	// window will get notification
	if(::IsWindow(GetSafeHwnd()))
		VERIFY(DeleteAllGroups());

	// delete all SHB_DESCRIPTOR structures we allocated
	LPSHB_DESCRIPTOR pDescriptor;
	int nOrder;
	POSITION pos=m_mapDescriptors.GetStartPosition();
	while(pos!=NULL)
	{
		m_mapDescriptors.GetNextAssoc(pos,pDescriptor,nOrder);

		ASSERT(pDescriptor!=NULL);

		delete pDescriptor;
	}
	m_mapDescriptors.RemoveAll();

	// delete all SHB_GROUPINFO structures we allocated
	LPSHB_GROUPINFO pGroupInfo;
	pos=m_mapHandleToInfo.GetStartPosition();
	while(pos!=NULL)
	{
		m_mapHandleToInfo.GetNextAssoc(pos,hGroup,pGroupInfo);

		ASSERT(hGroup!=NULL);
		ASSERT(pGroupInfo!=NULL);

		delete pGroupInfo;
	}

	m_mapHandleToInfo.RemoveAll();

	// delete all COXSHBListCtrl controls we created
	COXSHBListCtrl* pListCtrl;
	pos=m_mapHandleToListCtrl.GetStartPosition();
	while(pos!=NULL)
	{
		m_mapHandleToListCtrl.GetNextAssoc(pos,hGroup,pListCtrl);

		ASSERT(hGroup!=NULL);
		ASSERT(pListCtrl!=NULL);
		ASSERT_VALID(pListCtrl);

		delete pListCtrl;
	}
	m_mapHandleToListCtrl.RemoveAll();

	// clean up map of all bound rectangles
	m_mapHandleToBoundRect.RemoveAll();

	// dlete drag image if any was created
	delete m_pDragImage;

	// destroy window if it hasn't been destroyed yet
	if(::IsWindow(GetSafeHwnd()))
	{
		DestroyWindow();
	}

	if (m_pShortcutBarSkin != NULL)
		delete m_pShortcutBarSkin;
}


BEGIN_MESSAGE_MAP(COXShortcutBar, CWnd)
	//{{AFX_MSG_MAP(COXShortcutBar)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX(TTN_NEEDTEXTA, 0, OnHeaderToolTip)
	ON_NOTIFY_EX(TTN_NEEDTEXTW, 0, OnHeaderToolTip)

	ON_NOTIFY(SHBEN_FINISHEDIT, SHB_IDCEDIT, OnChangeGroupHeaderText)

	ON_MESSAGE(SHBDTM_DRAGOVER, OnDragOver)
	ON_MESSAGE(SHBDTM_DRAGLEAVE, OnDragLeave)

	ON_COMMAND(SHB_IDMLARGEICON, OnLargeIcon)
	ON_COMMAND(SHB_IDMSMALLICON, OnSmallIcon)
	ON_COMMAND(SHB_IDMADDNEWGROUP, OnAddNewGroup)
	ON_COMMAND(SHB_IDMREMOVEGROUP, OnRemoveGroup)
	ON_COMMAND(SHB_IDMRENAMEGROUP, OnRenameGroup)
	ON_COMMAND(SHBLC_IDMREMOVEITEM, OnRemoveLCItem)
	ON_COMMAND(SHBLC_IDMRENAMEITEM, OnRenameLCItem)
	ON_UPDATE_COMMAND_UI(SHB_IDMLARGEICON, OnUpdateLargeIcon)
	ON_UPDATE_COMMAND_UI(SHB_IDMSMALLICON, OnUpdateSmallIcon)

END_MESSAGE_MAP()


BOOL COXShortcutBar::Create(CWnd* pParentWnd, const RECT& rect, 
							DWORD dwBarStyle/* = SHBS_EDITHEADERS|SHBS_EDITITEMS|
							SHBS_DISABLEDRAGDROPITEM|SHBS_DISABLEDRAGDROPHEADER*/, 
							UINT nID/* = 0xffff*/, 
							DWORD dwStyle/* = WS_CHILD|WS_VISIBLE*/, 
							DWORD dwExStyle/* = 0*/)
{
	// create our own window class
	WNDCLASS wndClass;
	wndClass.style=CS_DBLCLKS; 
    wndClass.lpfnWndProc=AfxWndProc; 
    wndClass.cbClsExtra=0; 
    wndClass.cbWndExtra=0; 
    wndClass.hInstance=AfxGetInstanceHandle(); 
    wndClass.hIcon=NULL; 
    wndClass.hCursor=::LoadCursor(NULL,IDC_ARROW); 
    wndClass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1); 
    wndClass.lpszMenuName=NULL; 
	wndClass.lpszClassName=_T("OXShortcutBar");
	
	if(!AfxRegisterClass(&wndClass))
	{
		TRACE(_T("COXShortcutBar::Create: failed to register shortcut bar window class"));
		return FALSE;
	}

	// force WS_CHILD style
	dwStyle|=WS_CHILD;

	// create window
    if(!CWnd::CreateEx(
		dwExStyle,wndClass.lpszClassName,_T(""),dwStyle,rect,pParentWnd,nID,NULL))
	{
		TRACE(_T("COXShortcutBar::Create: failed to create the shortcut bar control\n"));
		return FALSE;
	}

	// set Shortcut bar specific style
	if(!SetBarStyle(dwBarStyle))
	{
		return FALSE;
	}

	// initialize shortcut bar
	if(!InitShortcutBar())
	{
		TRACE(_T("COXShortcutBar::Create: failed to initialize the shortcut bar"));
		return FALSE;
	}

    return TRUE;
}


int COXShortcutBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(GetSafeHwnd()));

	// the control should be designated to provide tooltips
	if((GetBarStyle()&SHBS_INFOTIP)==0)
		return -1;

	int nHit=-1;
	// now hit test against COXSHBListCtrl items
	UINT nHitFlags;
	HSHBGROUP hGroup=HitTest(point,&nHitFlags);
	if(nHitFlags!=SHB_ONHEADER)
		hGroup=NULL;

	if(hGroup!=NULL && pTI!= NULL)
	{
#if _MFC_VER<=0x0421
		if(pTI->cbSize>=sizeof(TOOLINFO))
			return nHit;
#endif
		pTI->hwnd=GetSafeHwnd();

		CRect rect;
		VERIFY(GetGroupHeaderRect(hGroup,rect));
		pTI->rect=rect;

		// 
		pTI->uId=(UINT)PtrToUint(hGroup);

		// set text to LPSTR_TEXTCALLBACK in order to get TTN_NEEDTEXT message when 
		// it's time to display tool tip
		pTI->lpszText=LPSTR_TEXTCALLBACK;

		// return found group handle
		nHit=(int)PtrToInt(hGroup);
	}
	return nHit;
}


BOOL COXShortcutBar::OnCmdMsg(UINT nID, int nCode, void* pExtra, 
							  AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	ASSERT(::IsWindow(GetSafeHwnd()));

	// pump through parent
	CWnd* pParentWnd=GetParent();
	if(pParentWnd!=NULL && !pParentWnd->IsKindOf(RUNTIME_CLASS(COXShortcutBar)) && 
		pParentWnd->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	{
		return TRUE;
	}

	// then pump through itself
	if (CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	{
		return TRUE;
	}

	// pump through child window of expanded group
	HSHBGROUP hGroup=GetExpandedGroup();
	if(hGroup!=NULL)
	{
		HWND hwndChild=GetGroupChildWndHandle(hGroup);
		ASSERT(hwndChild);
		CWnd* pChildWnd=CWnd::FromHandle(hwndChild);
		if(pChildWnd!=NULL && !pChildWnd->IsKindOf(RUNTIME_CLASS(COXShortcutBar)) &&
			pChildWnd->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		{
			return TRUE;
		}
	}

	// last but not least, pump through app
	CWinApp* pApp=AfxGetApp();
	if(pApp!=NULL && pApp->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	{
		return TRUE;
	}

	return FALSE;
}

void COXShortcutBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	UINT nHitFlags;
	HSHBGROUP hGroup=HitTest(point,&nHitFlags);

	// reset handle to the group that is positioned under mouse cursor
	SetMouseIsOverGroup((nHitFlags==SHB_ONHEADER ? hGroup : NULL));

	// reset pressed group handle
	if(m_hPressedGroup!=NULL && !IsLButtonDown())
	{
		BOOL bFlat=((GetBarStyle() & SHBS_FLATGROUPBUTTON)==SHBS_FLATGROUPBUTTON);
		// reset pressed group
		HSHBGROUP hOldPressedGroup=m_hPressedGroup;
		m_hPressedGroup=NULL;
		if(bFlat)
		{
			// redraw header
			UpdateHeader(hOldPressedGroup);
		}
	}

	// if dragging any group
	if(GetDragGroup()!=NULL)
	{
		if(nHitFlags==SHB_ONCHILD)
		{
			hGroup=NULL;
		}

		BOOL bDrawDragImage=FALSE;

		ASSERT((GetBarStyle()&SHBS_DISABLEDRAGDROPHEADER)==0);
		ASSERT((nFlags&MK_LBUTTON)!=0);

		// move drag image if needed 
		if((GetBarStyle()&SHBS_DRAWHEADERDRAGIMAGE)!=0)
		{
			ASSERT(m_pDragImage);
			CPoint ptScreen=point;
			ClientToScreen(&ptScreen);
			// move the drag image
			VERIFY(m_pDragImage->DragMove(ptScreen));

			bDrawDragImage=TRUE;
		}

		// Drag image could be over of one of headers while cursor not. Assuming
		// that our drag image width is less or equals to the width of the
		// client window of the shortcut bar control
		if(hGroup==NULL)
		{
			CRect rectDragImage;

			if((GetBarStyle()&SHBS_DRAWHEADERDRAGIMAGE)!=0)
			{
				// get drag image bounding rectangle
				IMAGEINFO imageInfo;
				m_pDragImage->GetImageInfo(0,&imageInfo);
				rectDragImage=imageInfo.rcImage;
				rectDragImage-=rectDragImage.TopLeft();

				// get client coordinate of drag image bounding rectangle
				CPoint ptTopLeft=point;
				ptTopLeft-=m_ptDragImage;
				rectDragImage+=ptTopLeft;
			}
			else
			{
				VERIFY(GetGroupHeaderRect(GetDragGroup(),rectDragImage));
			}

			CRect rect;
			GetClientRect(rect);
			CRect rectIntersect;
			// check if make sense to search for intersection
			if(rectIntersect.IntersectRect(rect,rectDragImage))
			{
				rectIntersect.IntersectRect(m_rectChildWnd,rectDragImage);
				if(!(rectIntersect==rectDragImage))
				{
					int nMaxSpace=0;
					HSHBGROUP hSuspectGroup;
					POSITION pos=m_mapHandleToBoundRect.GetStartPosition();
					while(pos!=NULL)
					{
						m_mapHandleToBoundRect.
							GetNextAssoc(pos,hSuspectGroup,rect);
						if(rectIntersect.IntersectRect(rect,rectDragImage) && 
							rectIntersect.Width()*rectIntersect.Height()>nMaxSpace)
						{
							hGroup=hSuspectGroup;
							nMaxSpace=rectIntersect.Width()*rectIntersect.Height();
						}
					}
				}
			}
		}

		// set drop target group
		HSHBGROUP hOldDropHilightGroup=SetDropTargetGroup(hGroup);
		BOOL bRedrawOldDropTarget=((hGroup!=hOldDropHilightGroup) &
			(GetDragGroup()!=hOldDropHilightGroup) & (hOldDropHilightGroup!=NULL));
		BOOL bRedrawNewDropTarget=((hGroup!=hOldDropHilightGroup) &
			(GetDragGroup()!=hGroup) & (hGroup!=NULL));
		BOOL bUnlock=bDrawDragImage && (bRedrawOldDropTarget || bRedrawNewDropTarget);
		
		if(bUnlock)
		{
			// unlock window updates
			VERIFY(m_pDragImage->DragShowNolock(FALSE));
		}

		// redraw affected headers
		if(bRedrawOldDropTarget) 
		{
			UpdateHeader(hOldDropHilightGroup);
		}
		if(bRedrawNewDropTarget)
		{
			UpdateHeader(hGroup);
		}

		// set corresponding cursor
		if(hGroup==NULL || hGroup==GetDragGroup())
		{
			SetCursor(LoadCursor(NULL,IDC_NO));
		}
		else
		{
			SetCursor(LoadCursor(NULL,IDC_ARROW));
		}

		if(bUnlock)
		{
			// lock window updates
			VERIFY(m_pDragImage->DragShowNolock(TRUE));
		}
	}
	// about to drag the group 
	else if(nHitFlags==SHB_ONHEADER && m_nWaitForDragHeaderID==0 && 
		IsLButtonDown() && hGroup==m_hSuspectDragGroup && hGroup!=NULL)
	{
		if((GetBarStyle()&SHBS_DISABLEDRAGDROPHEADER)==0)
		{
			// there shouldn't be any group currently being dragged
			ASSERT(GetDragGroup()==NULL);

			// create drag image and start dragging
			if((GetBarStyle() & SHBS_DRAWHEADERDRAGIMAGE)!=0)
			{
				// delete previously created drag image
				if(m_pDragImage)
				{
					delete m_pDragImage;
					m_pDragImage=NULL;
				}

				m_pDragImage=CreateGroupDragImage(hGroup);
				ASSERT(m_pDragImage);

				// changes the cursor to the drag image
				VERIFY(m_pDragImage->BeginDrag(0,m_ptDragImage));
				CPoint ptScreen=point;
				ClientToScreen(&ptScreen);
				VERIFY(m_pDragImage->DragEnter(GetDesktopWindow(),ptScreen));
	
				// unlock window updates
				VERIFY(m_pDragImage->DragShowNolock(FALSE));
			}

			// capture all mouse messages
			SetCapture();

			// assign group being dragged
			SetDragGroup(hGroup);

			// redraw dragged group header
			UpdateHeader(hGroup);
			
			if((GetBarStyle()&SHBS_DRAWHEADERDRAGIMAGE)!=0)
			{
				// lock window updates
				VERIFY(m_pDragImage->DragShowNolock(TRUE));
			}
		}
	}
	// about to auto expand the collapsed group
	else if(m_nWaitForDragHeaderID==0 && !IsLButtonDown() && hGroup!=NULL && 
		hGroup!=GetExpandedGroup() && (GetBarStyle()&SHBS_AUTOEXPAND)!=0 &&
		GetEditGroup()==NULL)
	{
		if(m_hSuspectAutoExpandGroup!=hGroup || nHitFlags!=SHB_ONHEADER)
		{
			// kill timer
			if(m_nWaitForAutoExpandID!=0)
			{
				ASSERT(m_hSuspectAutoExpandGroup!=NULL);

				KillTimer(m_nWaitForAutoExpandID);
				m_nWaitForAutoExpandID=0;
				m_hSuspectAutoExpandGroup=NULL;
			}
			else
			{
				ASSERT(m_hSuspectAutoExpandGroup==NULL);
			}
		}

		if(m_hSuspectAutoExpandGroup!=hGroup && nHitFlags==SHB_ONHEADER)
		{
			ASSERT(m_hSuspectAutoExpandGroup==NULL);

			m_hSuspectAutoExpandGroup=hGroup;
			// set timer
			m_nWaitForAutoExpandID=SetTimer(
				ID_WAITFORAUTOEXPANDTIMER,GetAutoExpandDelay(),NULL);
			ASSERT(m_nWaitForAutoExpandID!=0);
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void COXShortcutBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	VERIFY(m_mapHandleToBoundRect.GetCount()==m_mapHandleToInfo.GetCount());

	UINT nHitFlags;
	HSHBGROUP hGroup=HitTest(point,&nHitFlags);

	if(nHitFlags==SHB_ONHEADER)
	{
		BOOL bFlat=((GetBarStyle() & SHBS_FLATGROUPBUTTON)==SHBS_FLATGROUPBUTTON);

		// user clicked on header
		//

		ASSERT(hGroup!=NULL);

		ASSERT(m_hPressedGroup==NULL);
		m_hPressedGroup=hGroup;
		if(bFlat)
		{
			UpdateHeader(m_hPressedGroup);
		}
		else
		{
			ExpandGroup(hGroup);
		}

		// group drag'n'drop 
		if((GetBarStyle() & SHBS_DISABLEDRAGDROPHEADER)==0)
		{
			// in result of group expanding the header cordinates could have changed
			HSHBGROUP hSuspectGroup=HitTest(point,&nHitFlags);
			if(nHitFlags==SHB_ONHEADER && hSuspectGroup==hGroup)
			{
				// kill timer
				if(m_nWaitForDragHeaderID!=0)
				{
					KillTimer(m_nWaitForDragHeaderID);
					m_nWaitForDragHeaderID=0;
				}
				// set timer
				m_nWaitForDragHeaderID=SetTimer(
					ID_WAITFORDRAGHEADERTIMER,DFLT_WAITFORDRAGHEADER,NULL);
				ASSERT(m_nWaitForDragHeaderID!=0);

				// set the group as a suspect for drag'n'drop operation
				m_hSuspectDragGroup=hGroup;

				// define the point of hot spot on future drag image
				if((GetBarStyle()&SHBS_DRAWHEADERDRAGIMAGE)!=0)
				{
					CRect rectHeader;
					VERIFY(GetGroupHeaderRect(hGroup,&rectHeader));
					m_ptDragImage=point;
					ASSERT(rectHeader.PtInRect(m_ptDragImage));
					m_ptDragImage-=rectHeader.TopLeft();
				}

			}
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void COXShortcutBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	// if dragging any group
	if(GetDragGroup()!=NULL)
	{
		if((GetBarStyle()&SHBS_DRAWHEADERDRAGIMAGE)!=0)
		{
			ASSERT(m_pDragImage!=NULL);
			// end dragging
			VERIFY(m_pDragImage->DragLeave(GetDesktopWindow())); 
			m_pDragImage->EndDrag();
		}

		// reset drag group
		HSHBGROUP hOldDragGroup=SetDragGroup(NULL);
		ASSERT(hOldDragGroup!=NULL);

		HSHBGROUP hOldDropTargetGroup=SetDropTargetGroup(NULL);
		if(hOldDropTargetGroup!=NULL)
		{
			// notify parent
			// fill structure for notification
			NMSHORTCUTBAR nmshb;
			nmshb.hdr.code=SHBN_DROPGROUP;
			nmshb.hGroup=hOldDragGroup;
			nmshb.lParam=(LPARAM)hOldDropTargetGroup;
			if(!SendSHBNotification(&nmshb))
			{
				// all the corresponding actions
				//
				// reodering of drag and drop target items
				int nOrder=GetGroupOrder(hOldDropTargetGroup);
				ASSERT(nOrder>=0 && nOrder<GetGroupCount());
				VERIFY(SetGroupOrder(hOldDragGroup,nOrder));
				RedrawBar();
			}
			else
			{
				UpdateHeader(hOldDropTargetGroup);
				UpdateHeader(hOldDragGroup);
			}
		}
		else
		{
			UpdateHeader(hOldDragGroup);
		}

		if(::GetCapture()==GetSafeHwnd())
		{
			// stop intercepting all mouse messages
			::ReleaseCapture();
		}
	}
	else if(m_hPressedGroup!=NULL)
	{
		BOOL bFlat=((GetBarStyle() & SHBS_FLATGROUPBUTTON)==SHBS_FLATGROUPBUTTON);

		// reset pressed group
		HSHBGROUP hOldPressedGroup=m_hPressedGroup;
		m_hPressedGroup=NULL;

		if(bFlat)
		{
			UINT nHitFlags;
			HSHBGROUP hGroup=HitTest(point,&nHitFlags);
			if(nHitFlags==SHB_ONHEADER && hGroup==hOldPressedGroup)
			{
				if(GetExpandedGroup()!=hGroup)
				{
					// expand 
					ExpandGroup(hGroup);
				}
				else
				{
					// redraw header
					UpdateHeader(hGroup);
				}
			}
		}
	}


	
	CWnd::OnLButtonUp(nFlags, point);
}

void COXShortcutBar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	UNREFERENCED_PARAMETER(pWnd);
	
	// kill timer for group auto expanding
	if(m_nWaitForAutoExpandID!=0)
	{
		KillTimer(m_nWaitForAutoExpandID);
		m_nWaitForAutoExpandID=0;
		m_hSuspectAutoExpandGroup=NULL;
	}

	// create popup menu
	//
	CMenu menu;
	if(menu.CreatePopupMenu())
	{
		// we populate context menu depending on the place where right click happened 
		ScreenToClient(&point);
		UINT nFlags;
		HSHBGROUP hGroup=HitTest(point,&nFlags);
		if(nFlags==SHB_ONHEADER)
		{
			ASSERT(hGroup);
		}
		else if(nFlags==SHB_ONCHILD)
		{
			hGroup=GetExpandedGroup();
			ASSERT(hGroup);
		}
		else if(nFlags==SHB_NOWHERE)
		{
			hGroup=GetExpandedGroup();
		}
		else
		{
			ASSERT(FALSE);
		}

		// assign it as current hot group
		m_hHotGroup=hGroup;

		ClientToScreen(&point);

		// fill in SHBCONTEXTMENU structure to use it in message and notification
		SHBCONTEXTMENU shbcm;
		shbcm.pMenu=&menu;
		shbcm.pShortcutBar=this;
		shbcm.hGroup=hGroup;
		shbcm.point=point;

		// Add New Group item
		CString sItem;
		VERIFY(sItem.LoadString(IDS_OX_SHB_IDMADDNEWGROUP_TEXT));
		if((GetBarStyle()&SHBS_EDITHEADERS)!=0)
		{
			menu.AppendMenu(MF_STRING,SHB_IDMADDNEWGROUP,sItem);
		}

		// if any group exist
		if(hGroup)
		{
			VERIFY(sItem.LoadString(IDS_OX_SHB_IDMLARGEICON_TEXT));
			// view type items
			if((GetBarStyle()&SHBS_EDITHEADERS)!=0)
			{
				menu.AppendMenu(MF_SEPARATOR);
			}
			menu.AppendMenu(MF_STRING|MF_CHECKED,SHB_IDMLARGEICON,sItem);
			VERIFY(sItem.LoadString(IDS_OX_SHB_IDMSMALLICON_TEXT));
			menu.AppendMenu(MF_STRING|MF_CHECKED,SHB_IDMSMALLICON,sItem);
			if((GetBarStyle()&SHBS_EDITHEADERS)!=0)
			{
				menu.AppendMenu(MF_SEPARATOR);
				// remove/rename group
				VERIFY(sItem.LoadString(IDS_OX_SHB_IDMREMOVEGROUP_TEXT));
				menu.AppendMenu(MF_STRING,SHB_IDMREMOVEGROUP,sItem);
				VERIFY(sItem.LoadString(IDS_OX_SHB_IDMRENAMEGROUP_TEXT));
				menu.AppendMenu(MF_STRING,SHB_IDMRENAMEGROUP,sItem);
			}
	

			// check/uncheck view type menu items
			HSHBGROUP hExpandedGroup=GetExpandedGroup();
			if(hExpandedGroup)
			{
				int nView=GetGroupView(hExpandedGroup);
				menu.CheckMenuItem(SHB_IDMLARGEICON,MF_BYCOMMAND|
					((nView==SHB_LARGEICON) ? MF_CHECKED : MF_UNCHECKED));
				menu.CheckMenuItem(SHB_IDMSMALLICON,MF_BYCOMMAND|
					((nView==SHB_LARGEICON) ? MF_UNCHECKED : MF_CHECKED));
			}

			// if right click was over child window the we have to give it a chance to 
			// populate context menu with its items
			if(nFlags==SHB_ONCHILD)
			{
				ASSERT(hGroup==hExpandedGroup);

				HWND hWnd=GetGroupChildWndHandle(hGroup);
				ASSERT(hWnd);

				::SendMessage(hWnd,SHBM_POPULATECONTEXTMENU,
					(WPARAM)0,(LPARAM)&shbcm);
			}
		}

		// fill structure for notification
		NMSHORTCUTBAR nmshb;
		nmshb.hdr.code=SHBN_CONTEXTMENU;
		nmshb.hGroup=hGroup;
		nmshb.lParam=(LPARAM)(&shbcm);

		// if parent didn't reject to display menu then do that
		if(!SendSHBNotification(&nmshb) && menu.GetMenuItemCount()>0)
		{
			menu.TrackPopupMenu(
				TPM_LEFTALIGN|TPM_LEFTBUTTON,point.x,point.y, this);
		}

	}

	menu.DestroyMenu();
}

void COXShortcutBar::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	_AFX_THREAD_STATE* pThreadState=AfxGetThreadState();
	// hook not already in progress
	if(pThreadState->m_pWndInit==NULL)
	{
		// set default shortcut bar style
		m_dwBarStyle=SHBS_EDITHEADERS|SHBS_EDITITEMS|
			SHBS_DISABLEDRAGDROPITEM|SHBS_DISABLEDRAGDROPHEADER;

		DWORD dwStyle=GetStyle();
		// check if subclassed window has WS_CHILD style
		VERIFY((dwStyle & WS_CHILD)==WS_CHILD);

		if(!InitShortcutBar())
		{
			TRACE(_T("COXShortcutBar::PreSubclassWindow: failed to initialize shortcut bar\n"));
		}
	}

	CWnd::PreSubclassWindow();
}

void COXShortcutBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	CRect rect;
	GetClientRect(rect);

	// Save DC
	int nSavedDC=dc.SaveDC();
	ASSERT(nSavedDC);

	// Set clipping region
	dc.IntersectClipRect(rect);
	if(!m_rectChildWnd.IsRectEmpty())
	{
		dc.ExcludeClipRect(m_rectChildWnd);
	}

	// fill background with expanded group color
	FillBackground(&dc);

	int nCount= PtrToInt(GetGroupCount());
	VERIFY(m_mapHandleToBoundRect.GetCount()==nCount);

	HSHBGROUP hGroup;
	// draw group headers one after another
	for(int nIndex=0; nIndex<nCount; nIndex++)
	{
		hGroup=FindGroupByOrder(nIndex);
		VERIFY(hGroup!=NULL);

		VERIFY(m_mapHandleToBoundRect.Lookup(hGroup,rect));

		// draw header only if it's visible
		if(dc.RectVisible(&rect))
		{
			CRect rectHeader=rect;
			rectHeader-=rect.TopLeft();
			CDC dcCompatible;
			if(!dcCompatible.CreateCompatibleDC(&dc))
			{
				TRACE(_T("COXShortcutBar::OnPaint:Failed to create compatible DC\n"));
				return;
			}
			CBitmap bitmap;
			if(!bitmap.CreateCompatibleBitmap(&dc, rectHeader.Width(), 
				rectHeader.Height()))
			{
				TRACE(_T("COXShortcutBar::OnPaint:Failed to create compatible bitmap\n"));
				return;
			}
			CBitmap* pOldBitmap=dcCompatible.SelectObject(&bitmap);

			// fill standard DRAWITEMSTRUCT to send it to DrawHeader function
			//
			DRAWITEMSTRUCT dis;
			dis.CtlType=0;
			dis.CtlID=GetDlgCtrlID();
			dis.itemID=(UINT) PtrToUint(hGroup);
			dis.itemAction=ODA_DRAWENTIRE;
			dis.itemState=ODS_DEFAULT;
			dis.hwndItem=GetSafeHwnd();
			dis.hDC=dcCompatible.GetSafeHdc();
			dis.rcItem=rectHeader;
			dis.itemData=(DWORD)PtrToUlong(hGroup);

			DrawHeader(&dis);

			dc.BitBlt(rect.left, rect.top, rect.Width(), 
				rect.Height(), &dcCompatible, 0, 0, SRCCOPY);
	
			if(pOldBitmap)
			{
				dcCompatible.SelectObject(pOldBitmap);
			}
		}
	}
	
	// Restore dc	
	dc.RestoreDC(nSavedDC);

	// Do not call CWnd::OnPaint() for painting messages
}

void COXShortcutBar::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	// reposition groups
	CalculateBoundRects();
	// redraw control
	RedrawBar();
}

BOOL COXShortcutBar::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

#if _MSC_VER >= 1400
LRESULT COXShortcutBar::OnNcHitTest(CPoint point)
#else
UINT COXShortcutBar::OnNcHitTest(CPoint point)
#endif
{
	UNREFERENCED_PARAMETER(point);
	return HTCLIENT;
}

void COXShortcutBar::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
	// just kill timers
	if(m_nWaitForDragHeaderID!=0)
	{
		KillTimer(m_nWaitForDragHeaderID);
	}
	if(m_nWaitForAutoExpandID!=0)
	{
		KillTimer(m_nWaitForAutoExpandID);
	}
	if(m_nCheckMouseIsOverGroupID!=0)
	{
		KillTimer(m_nCheckMouseIsOverGroupID);
	}
}

void COXShortcutBar::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	// reset pressed group handle
	if(m_hPressedGroup!=NULL && !IsLButtonDown())
	{
		BOOL bFlat=((GetBarStyle() & SHBS_FLATGROUPBUTTON)==SHBS_FLATGROUPBUTTON);
		// reset pressed group
		HSHBGROUP hOldPressedGroup=m_hPressedGroup;
		m_hPressedGroup=NULL;
		if(bFlat)
		{
			// redraw header
			UpdateHeader(hOldPressedGroup);
		}
	}

	if(m_nWaitForDragHeaderID==nIDEvent)
	{
		ASSERT(GetDragGroup()==NULL);
		// kill time after time has expired
		KillTimer(m_nWaitForDragHeaderID);
		m_nWaitForDragHeaderID=0;
	}
	else if(m_nWaitForAutoExpandID==nIDEvent)
	{
		ASSERT(m_hSuspectAutoExpandGroup!=NULL);
		ASSERT(GetExpandedGroup()!=m_hSuspectAutoExpandGroup);

		// kill time after time has expired
		KillTimer(m_nWaitForAutoExpandID);
		m_nWaitForAutoExpandID=0;

		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);

		// test the current cursor point 
		UINT nHitFlags;
		HSHBGROUP hGroup=HitTest(point,&nHitFlags);

		// expand the m_hSuspectAutoExpandGroup group if the mouse is still over it
		if(nHitFlags==SHB_ONHEADER && m_hSuspectAutoExpandGroup==hGroup)
		{
			ExpandGroup(hGroup);
		}
		else
		{
			m_hSuspectAutoExpandGroup=NULL;
		}
	}
	else if(m_nCheckMouseIsOverGroupID==nIDEvent)
	{
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);

		// test the current cursor point 
		UINT nHitFlags;
		HSHBGROUP hGroup=HitTest(point,&nHitFlags);

		// reset handle to the group that is positioned under mouse cursor
		SetMouseIsOverGroup((nHitFlags==SHB_ONHEADER ? hGroup : NULL));
	}
	else
	{
		CWnd::OnTimer(nIDEvent);
	}
}

BOOL COXShortcutBar::OnHeaderToolTip(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{   
	UNREFERENCED_PARAMETER(id);

	ASSERT(pNMHDR->code==TTN_NEEDTEXTA || pNMHDR->code==TTN_NEEDTEXTW);

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA=(TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW=(TOOLTIPTEXTW*)pNMHDR;
    
	// idFrom must be the handle to the group
	if (pNMHDR->code==TTN_NEEDTEXTA)
		ASSERT((pTTTA->uFlags&TTF_IDISHWND)==0);
	else
		ASSERT((pTTTW->uFlags&TTF_IDISHWND)==0);

	SHBINFOTIP shbit;
	// fill structure for notification
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_GETHEADERINFOTIP;
	nmshb.hGroup=(HSHBGROUP)pNMHDR->idFrom;
	nmshb.lParam=(LPARAM)(&shbit);

	*pResult=SendSHBNotification(&nmshb);

#ifndef _UNICODE
	if(pNMHDR->code==TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText,shbit.szText,countof(pTTTA->szText));
	else
		_mbstowcsz(pTTTW->szText,shbit.szText,countof(pTTTW->szText));
#else
	if (pNMHDR->code==TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText,shbit.szText,countof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText,shbit.szText,countof(pTTTW->szText));
#endif

	if(pNMHDR->code==TTN_NEEDTEXTA)
	{
		if(shbit.lpszText==NULL)
			pTTTA->lpszText=pTTTA->szText;
		else
			pTTTA->lpszText=(LPSTR)shbit.lpszText;
		pTTTA->hinst=shbit.hinst;
	}
	else
	{
		if(shbit.lpszText==NULL)
			pTTTW->lpszText=pTTTW->szText;
		else
			pTTTW->lpszText=(LPWSTR)shbit.lpszText;
		pTTTW->hinst=shbit.hinst;
	}

	return TRUE;    // message was handled
}

void COXShortcutBar::OnChangeGroupHeaderText(NMHDR* pNotifyStruct, LRESULT* result)
{
	// handle SHBEN_FINISHEDIT notification from COXSHBEdit control
	ASSERT(::IsWindow(m_edit.GetSafeHwnd()));
	ASSERT(m_hEditGroup);

	LPNMSHBEDIT lpNMSHBE=(LPNMSHBEDIT)pNotifyStruct;

	// fill structure for notification
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_ENDHEADEREDIT;
	nmshb.hGroup=m_hEditGroup;

	// if editing wasn't cancelled
	if(lpNMSHBE->bOK)
	{
		// get updated text
		CString sText;
		m_edit.GetWindowText(sText);

		nmshb.lParam=(LPARAM)((LPCTSTR)sText);

		if(!SendSHBNotification(&nmshb))
		{
			// change header text
			if(!SetGroupText(m_hEditGroup,sText))
			{
				TRACE(_T("COXSHBListCtrl::OnChangeGroupHeaderText: failed to set text to item: %s"),sText);
			}
			else
			{
				CRect rectHeader;
				if(GetGroupHeaderRect(m_hEditGroup,rectHeader))
					RedrawWindow(rectHeader);
			}
			sText.ReleaseBuffer();
		}
	}
	else
	{
		nmshb.lParam=(LPARAM)((LPCTSTR)GetGroupText(nmshb.hGroup));
		SendSHBNotification(&nmshb);
	}

	m_hEditGroup=NULL;

	*result=0;
}

LONG COXShortcutBar::OnDragOver(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if((GetBarStyle()&SHBS_DISABLEDROPITEM)!=0)
	{
		return (LONG)FALSE;
	}

	// fill structure for notification of parent window of this shortcut bar
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_DRAGOVER;
	nmshb.lParam=lParam;
	// check if parent of the shortcut bar would like to handle drag'n'drop itself
	if(SendSHBNotification(&nmshb))
	{
		return (LONG)TRUE;
	}

	// lParam is the pointer to SHBDROPTARGETACTION structure
	LPSHBDROPTARGETACTION pSHBDTAction=(LPSHBDROPTARGETACTION)lParam;
	ASSERT(pSHBDTAction!=NULL);

	ASSERT(pSHBDTAction->pWnd);
	ASSERT(pSHBDTAction->pWnd->GetSafeHwnd()==GetSafeHwnd());

	// set returned drop effect
	pSHBDTAction->result=(LRESULT)DROPEFFECT_NONE;

	// we are not dragging any group
	ASSERT(GetDragGroup()==NULL);
	// and are not waiting for drag any group
	ASSERT(m_nWaitForDragHeaderID==0);

	// test current cursor position
	UINT nHitFlags;
	HSHBGROUP hGroup=HitTest(pSHBDTAction->point,&nHitFlags);

	if(hGroup!=NULL && hGroup!=GetExpandedGroup())
	{
		// kill timer for group autoexpanding
		if(m_hSuspectAutoExpandGroup!=hGroup || nHitFlags!=SHB_ONHEADER)
		{
			// kill timer
			if(m_nWaitForAutoExpandID!=0)
			{
				ASSERT(m_hSuspectAutoExpandGroup!=NULL);

				KillTimer(m_nWaitForAutoExpandID);
				m_nWaitForAutoExpandID=0;
				m_hSuspectAutoExpandGroup=NULL;
			}
			else
			{
				ASSERT(m_hSuspectAutoExpandGroup==NULL);
			}
		}

		// set timer for group autoexpanding
		if(m_hSuspectAutoExpandGroup!=hGroup && nHitFlags==SHB_ONHEADER)
		{
			ASSERT(m_hSuspectAutoExpandGroup==NULL);

			m_hSuspectAutoExpandGroup=hGroup;
			// set timer
			m_nWaitForAutoExpandID=SetTimer(
				ID_WAITFORAUTOEXPANDTIMER,GetAutoExpandDelay()/4,NULL);
			ASSERT(m_nWaitForAutoExpandID!=0);
		}
	}

	return (LONG)TRUE;
}

LONG COXShortcutBar::OnDragLeave(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if((GetBarStyle()&SHBS_DISABLEDROPITEM)!=0)
	{
		return (LONG)FALSE;
	}

	// fill structure for notification of parent window of this shortcut bar
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_DRAGLEAVE;
	nmshb.lParam=lParam;
	// check if parent of the shortcut bar would like to handle drag'n'drop itself
	if(SendSHBNotification(&nmshb))
	{
		return (LONG)TRUE;
	}

	// lParam is the pointer to SHBDROPTARGETACTION structure
	LPSHBDROPTARGETACTION pSHBDTAction=(LPSHBDROPTARGETACTION)lParam;
	ASSERT(pSHBDTAction!=NULL);

	ASSERT(pSHBDTAction->pWnd);
	ASSERT(pSHBDTAction->pWnd->GetSafeHwnd()==GetSafeHwnd());

	// kill timer
	if(m_nWaitForAutoExpandID!=0)
	{
		ASSERT(m_hSuspectAutoExpandGroup!=NULL);

		KillTimer(m_nWaitForAutoExpandID);
		m_nWaitForAutoExpandID=0;
		m_hSuspectAutoExpandGroup=NULL;
	}

	return (LONG)TRUE;
}

void COXShortcutBar::OnLargeIcon()
{
	HSHBGROUP hGroup=GetExpandedGroup();
	if(hGroup==NULL)
		return;

	int nView=GetGroupView(hGroup);
	if(nView!=SHB_LARGEICON)
	{
		// change view in result of selecting context menu item
		VERIFY(SetGroupView(hGroup,SHB_LARGEICON));
		ShowChildWnd(hGroup);
	}
}

void COXShortcutBar::OnSmallIcon()
{
	HSHBGROUP hGroup=GetExpandedGroup();
	if(hGroup==NULL)
		return;

	int nView=GetGroupView(hGroup);
	if(nView!=SHB_SMALLICON)
	{
		// change view in result of selecting context menu item
		VERIFY(SetGroupView(hGroup,SHB_SMALLICON));
		ShowChildWnd(hGroup);
	}
}

void COXShortcutBar::OnAddNewGroup()
{
	SHB_GROUPINFO shbGroup;
	shbGroup.nMask=SHBIF_TEXT;
	shbGroup.nTextMax=0;
	shbGroup.pszText=_T("");

	// add empty group
	HSHBGROUP hGroup=InsertGroup(&shbGroup);
	if(hGroup!=NULL)
	{
		if(GetGroupCount()==1)
			ExpandGroup(hGroup);
		else
		{
			CalculateBoundRects();
			RedrawBar();
		}

		// edit header
		EditGroupHeader(hGroup);
	}
}

void COXShortcutBar::OnRemoveGroup()
{
	HSHBGROUP hGroup=GetHotGroup();
	if(hGroup!=NULL)
	{
		VERIFY(DeleteGroup(hGroup));
	}
}

void COXShortcutBar::OnRenameGroup()
{
	HSHBGROUP hGroup=GetHotGroup();
	if(hGroup!=NULL)
	{
		EditGroupHeader(hGroup);
	}
}

void COXShortcutBar::OnRemoveLCItem()
{
	HSHBGROUP hGroup=GetExpandedGroup();
	if(hGroup==NULL || GetGroupChildWnd(hGroup)!=NULL)
		return;

	// remove COXSHBListCtrl item in result of selecting context menu item
	//
	COXSHBListCtrl* pListCtrl=GetGroupListCtrl(hGroup);
	ASSERT(pListCtrl);
	ASSERT(::IsWindow(pListCtrl->GetSafeHwnd()));

	int nItem=pListCtrl->GetHotItem();
	if(nItem==-1)
		nItem=pListCtrl->GetLastHotItem();
	if(nItem!=-1)
		DeleteLCItem(hGroup,nItem);
}

void COXShortcutBar::OnRenameLCItem()
{
	HSHBGROUP hGroup=GetExpandedGroup();
	if(hGroup==NULL || GetGroupChildWnd(hGroup)!=NULL)
		return;

	// edit COXSHBListCtrl item text in result of selecting context menu item
	//
	COXSHBListCtrl* pListCtrl=GetGroupListCtrl(hGroup);
	ASSERT(pListCtrl);
	ASSERT(::IsWindow(pListCtrl->GetSafeHwnd()));

	int nItem=pListCtrl->GetHotItem();
	if(nItem==-1)
		nItem=pListCtrl->GetLastHotItem();
	if(nItem!=-1)
	{
		pListCtrl->EnsureVisible(nItem,FALSE);
		EditLCItem(hGroup,nItem);
	}
}

// Update handlers
//
void COXShortcutBar::OnUpdateLargeIcon(CCmdUI* pCmdUI)
{
	HSHBGROUP hGroup=GetExpandedGroup();
	if(hGroup==NULL)
		return;

	int nView=GetGroupView(hGroup);
	pCmdUI->SetCheck((nView==SHB_LARGEICON));
}

void COXShortcutBar::OnUpdateSmallIcon(CCmdUI* pCmdUI)
{
	HSHBGROUP hGroup=GetExpandedGroup();
	if(hGroup==NULL)
		return;

	int nView=GetGroupView(hGroup);
	pCmdUI->SetCheck((nView==SHB_SMALLICON));
}

/////////////////////////////////////////////////////////////////////////////

LPSHB_DESCRIPTOR COXShortcutBar::AddDescriptor()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	// Internal function to dynamically allocate descriptor structure.
	// We keep track of all created descriptors and delete them in destructor
	LPSHB_DESCRIPTOR pDescriptor=NULL;
	try
	{
		pDescriptor=new SHB_DESCRIPTOR;
	}
	catch (CMemoryException* pException)
	{
		UNREFERENCED_PARAMETER(pException);
		TRACE(_T("COXShortcutBar::AddDescriptor: failed to allocate memory for SHB_DESCRIPTOR"));
		return NULL;
	}

	ASSERT(pDescriptor!=NULL);

	m_mapDescriptors.SetAt(pDescriptor,m_mapDescriptors.GetCount());

	return pDescriptor;
}

void COXShortcutBar::DrawHeader(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	HSHBGROUP hGroup=(HSHBGROUP)(INT_PTR)lpDrawItemStruct->itemID;
	ASSERT(hGroup);

	// fill structure for notification
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_DRAWHEADER;
	nmshb.hGroup=hGroup;
	nmshb.lParam=(LPARAM)lpDrawItemStruct;

	if(SendSHBNotification(&nmshb))
	{
		return;
	}

	GetShortcutBarSkin()->DrawHeader(lpDrawItemStruct, this);
}

/////////////////////////////////////////////////////////////////////////////

BOOL COXShortcutBar::SetBarStyle(DWORD dwBarStyle)
{
	if(!VerifyBarStyle(dwBarStyle))
	{
		return FALSE;
	}

	m_dwBarStyle=dwBarStyle;

	if(::IsWindow(GetSafeHwnd()))
	{
		// Notify child windows that style have been changed 
		SendMessageToDescendants(
			SHBM_SHBINFOCHANGED,(WPARAM)0,(LPARAM)SHBIF_SHBSTYLE);
	}

    return TRUE;
}

CImageList* COXShortcutBar::SetImageList(CImageList* pImageList)
{
	// it's progammer's responsibility to create and keep image list alive
	CImageList* pOldImageList=m_pImageList;
	m_pImageList=pImageList;

	return pOldImageList;
}

// functions to work with groups
//

BOOL COXShortcutBar::ChildWndIsLC(HSHBGROUP hGroup) const
{
	ASSERT(hGroup);
	ASSERT(::IsWindow(GetSafeHwnd()));

	// we assume child window always exist
	//
	HWND hwndChild=GetGroupChildWnd(hGroup);
	if(hwndChild)
	{
		return FALSE;
	}
	else
	{
		COXSHBListCtrl* pListCtrl=GetGroupListCtrl(hGroup);
		ASSERT(pListCtrl!=NULL);
		return TRUE;
	}
}

COXSHBListCtrl* COXShortcutBar::GetGroupListCtrl(HSHBGROUP hGroup) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	COXSHBListCtrl* pListCtrl;
	if(!m_mapHandleToListCtrl.Lookup(hGroup,pListCtrl))
	{
		return NULL;
	}

	ASSERT(pListCtrl!=NULL);
	ASSERT_VALID(pListCtrl);

	return pListCtrl;
}

BOOL COXShortcutBar::GetGroupInfo(HSHBGROUP hGroup, 
								  LPSHB_GROUPINFO pGroupInfo, 
								  BOOL bSubstitute/* = FALSE*/)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);
	ASSERT(pGroupInfo);

	if(!VerifyGroupInfo(pGroupInfo))
	{
		return FALSE;
	}

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo))
	{
		TRACE(_T("COXShortcutBar::GetGroupInfo: group wasn't created"));
		return FALSE;
	}

	ASSERT(pSavedGroupInfo!=NULL);

	// CopyGroupInfo provide all we need to copy information
	return CopyGroupInfo(pGroupInfo,pSavedGroupInfo,
		(bSubstitute ? SHB_CPYINFO_COPY|SHB_CPYINFO_GET : SHB_CPYINFO_GET));
}

BOOL COXShortcutBar::SetGroupInfo(HSHBGROUP hGroup, 
								  const LPSHB_GROUPINFO pGroupInfo, 
								  BOOL bSubstitute/* = FALSE*/)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);
	ASSERT(pGroupInfo);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo))
	{
		TRACE(_T("COXShortcutBar::SetGroupInfo: group wasn't created"));
		return FALSE;
	}

	ASSERT(pSavedGroupInfo!=NULL);

	if(CopyGroupInfo(pSavedGroupInfo,pGroupInfo,
		(bSubstitute ? SHB_CPYINFO_COPY|SHB_CPYINFO_SET : SHB_CPYINFO_SET)))
	{
		// recalc rectangles if changed order
		if((pGroupInfo->nMask&(SHBIF_ORDER)))
			CalculateBoundRects();

		// recalc rectangles if changed descriptor both of shortcut bar and child window
		if((pGroupInfo->nMask&(SHBIF_DESCRIPTOR)))
			CalculateBoundRects();

		// if changed view then scroll list control to the top

		// if changed child window
		if((pGroupInfo->nMask&SHBIF_CHILDWND))
		{
			if(pGroupInfo->hwndChild==NULL)
			{
				// if child window was removed we have to create COXSHBListCrl
				// for such window because every group have to have child window
				COXSHBListCtrl* pListCtrl=GetGroupListCtrl(hGroup);
				if(pListCtrl!=NULL)
				{
					try
					{
						pListCtrl=new COXSHBListCtrl();
					}
					catch (CMemoryException* pException)
					{
						UNREFERENCED_PARAMETER(pException);
						TRACE(_T("COXShortcutBar::SetGroupInfo: failed to allocate memory for COXSHBListCtrl"));
						return FALSE;
					}
					m_mapHandleToListCtrl.SetAt(hGroup,pListCtrl);

					ASSERT(pListCtrl);
	
					if(!CreateListControl(hGroup))
					{
						m_mapHandleToListCtrl.RemoveKey(hGroup);
						delete pListCtrl;
						return FALSE;
					}
				}
			}
			else
			{
				// send message to child window about aknowledging it and set group to it  
				ASSERT(::IsWindow(pGroupInfo->hwndChild));
				::SendMessage(pGroupInfo->hwndChild,SHBM_SETSHBGROUP,(WPARAM)0,
					(LPARAM)hGroup);
			}
		}

		//send message to child window so it has chance to react on changed group info
		HWND hwndChild=GetGroupChildWndHandle(hGroup);
		ASSERT(hwndChild);
		::SendMessage(hwndChild,SHBM_GROUPINFOCHANGED,(WPARAM)0,
			(LPARAM)pGroupInfo->nMask);
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// All following SetGroup* functions eventually call SetGroupInfo function
//

///////////////////////////////////////////////////////////////////////////////
int COXShortcutBar::GetGroupImage(HSHBGROUP hGroup) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo) ||
		(pSavedGroupInfo->nMask&SHBIF_IMAGE)==0)
	{
		return -1;
	}

	ASSERT(pSavedGroupInfo!=NULL);

	return pSavedGroupInfo->nImage;
}

BOOL COXShortcutBar::SetGroupImage(HSHBGROUP hGroup, int nImage)
{
	ASSERT(hGroup);

	SHB_GROUPINFO shbGroup;
	shbGroup.nMask=SHBIF_IMAGE;
	shbGroup.nImage=nImage;

	return SetGroupInfo(hGroup,&shbGroup);
}


int COXShortcutBar::GetGroupImageExpanded(HSHBGROUP hGroup) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo))
	{
		return -1;
	}

	ASSERT(pSavedGroupInfo!=NULL);

	if((pSavedGroupInfo->nMask&SHBIF_IMAGEEXPANDED)!=0)
	{
		return pSavedGroupInfo->nImageExpanded;
	}
	else
	{
		return GetGroupImage(hGroup);
	}
}
	
BOOL COXShortcutBar::SetGroupImageExpanded(HSHBGROUP hGroup, int nImageExpanded)
{
	ASSERT(hGroup);

	SHB_GROUPINFO shbGroup;
	shbGroup.nMask=SHBIF_IMAGEEXPANDED;
	shbGroup.nImageExpanded=nImageExpanded;

	return SetGroupInfo(hGroup,&shbGroup);
}

CString COXShortcutBar::GetGroupText(HSHBGROUP hGroup) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo) ||
		(pSavedGroupInfo->nMask&SHBIF_TEXT)==0)
	{
		return _T("");
	}

	ASSERT(pSavedGroupInfo!=NULL);

	// copy text
	return CString(pSavedGroupInfo->pszText, pSavedGroupInfo->nTextMax);
}

BOOL COXShortcutBar::SetGroupText(HSHBGROUP hGroup, CString sText)
{
	ASSERT(hGroup);

	SHB_GROUPINFO shbGroup;
	shbGroup.nMask=SHBIF_TEXT;
	shbGroup.pszText=sText.GetBuffer(sText.GetLength());
	shbGroup.nTextMax=sText.GetLength();
	sText.ReleaseBuffer();

	return SetGroupInfo(hGroup,&shbGroup);
}
	
BOOL COXShortcutBar::SetGroupText(HSHBGROUP hGroup, LPCTSTR pszText, 
								  int nTextMax)
{
	ASSERT(hGroup);

	SHB_GROUPINFO shbGroup;
	shbGroup.nMask=SHBIF_TEXT;
	shbGroup.pszText=(LPTSTR)pszText;
	shbGroup.nTextMax=nTextMax;

	return SetGroupInfo(hGroup,&shbGroup);
}
	
LPARAM COXShortcutBar::GetGroupData(HSHBGROUP hGroup) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo) ||
		(pSavedGroupInfo->nMask&SHBIF_PARAM)==0)
	{
		return NULL;
	}

	ASSERT(pSavedGroupInfo!=NULL);

	return pSavedGroupInfo->lParam;
}

BOOL COXShortcutBar::SetGroupData(HSHBGROUP hGroup, LPARAM lParam)
{
	ASSERT(hGroup);

	SHB_GROUPINFO shbGroup;
	shbGroup.nMask=SHBIF_PARAM;
	shbGroup.lParam=lParam;

	return SetGroupInfo(hGroup,&shbGroup);
}

int COXShortcutBar::GetGroupOrder(HSHBGROUP hGroup) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo) ||
		(pSavedGroupInfo->nMask&SHBIF_ORDER)==0)
	{
		return -1;
	}

	ASSERT(pSavedGroupInfo!=NULL);

	return pSavedGroupInfo->nOrder;
}
	
BOOL COXShortcutBar::SetGroupOrder(HSHBGROUP hGroup, int nOrder)
{
	ASSERT(hGroup);

	SHB_GROUPINFO shbGroup;
	shbGroup.nMask=SHBIF_ORDER;
	shbGroup.nOrder=nOrder;

	return SetGroupInfo(hGroup,&shbGroup);

}

int COXShortcutBar::GetGroupView(HSHBGROUP hGroup) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo))
	{
		return -1;
	}

	ASSERT(pSavedGroupInfo!=NULL);

	if((pSavedGroupInfo->nMask&SHBIF_VIEW)==0)
	{
		return SHB_LARGEICON;
	}

	return pSavedGroupInfo->nView;
}
	
BOOL COXShortcutBar::SetGroupView(HSHBGROUP hGroup, int nView)
{
	// this function can be called with hGroup=NULL. In this case we 
	// set specified view to every group in shortcut bar
	SHB_GROUPINFO shbGroup;
	shbGroup.nMask=SHBIF_VIEW;
	shbGroup.nView=nView;

	if(hGroup!=NULL)
		return SetGroupInfo(hGroup,&shbGroup);
	else
	{
		if(!VerifyView(nView))
			return FALSE;

		LPSHB_GROUPINFO pGroupInfo;
		POSITION pos=m_mapHandleToInfo.GetStartPosition();
		while(pos!=NULL)
		{
			m_mapHandleToInfo.GetNextAssoc(pos,hGroup,pGroupInfo);

			ASSERT(hGroup!=NULL);
			ASSERT(pGroupInfo!=NULL);

			if(!SetGroupInfo(hGroup,&shbGroup))
				return FALSE;
		}
	}

	return TRUE;

}

LPSHB_DESCRIPTOR COXShortcutBar::GetGroupDescriptor(HSHBGROUP hGroup) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo) ||
		(pSavedGroupInfo->nMask&SHBIF_DESCRIPTOR)==0)
	{
		return NULL;
	}

	ASSERT(pSavedGroupInfo!=NULL);

	return pSavedGroupInfo->pDescriptor;
}
	
BOOL COXShortcutBar::SetGroupDescriptor(HSHBGROUP hGroup, 
										const LPSHB_DESCRIPTOR pDescriptor)
{
	// this function can be called with hGroup=NULL. In this case we 
	// set specified view to every group in shortcut bar
	SHB_GROUPINFO shbGroup;
	shbGroup.nMask=SHBIF_DESCRIPTOR;
	shbGroup.pDescriptor=pDescriptor;

	if(hGroup!=NULL)
	{
		return SetGroupInfo(hGroup,&shbGroup);
	}
	else
	{
		if(!VerifyDescriptor(pDescriptor))
			return FALSE;

		LPSHB_GROUPINFO pGroupInfo;
		POSITION pos=m_mapHandleToInfo.GetStartPosition();
		while(pos!=NULL)
		{
			m_mapHandleToInfo.GetNextAssoc(pos,hGroup,pGroupInfo);

			ASSERT(hGroup!=NULL);
			ASSERT(pGroupInfo!=NULL);

			if(!SetGroupInfo(hGroup,&shbGroup))
				return FALSE;
		}
	}

	return TRUE;
}

HWND COXShortcutBar::GetGroupChildWnd(HSHBGROUP hGroup) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo) ||
		(pSavedGroupInfo->nMask&SHBIF_CHILDWND)==0)
	{
		return NULL;
	}

	ASSERT(pSavedGroupInfo!=NULL);

	return pSavedGroupInfo->hwndChild;
}
	
BOOL COXShortcutBar::SetGroupChildWnd(HSHBGROUP hGroup, HWND hwndChild)
{
	ASSERT(hGroup);

	SHB_GROUPINFO shbGroup;
	shbGroup.nMask=SHBIF_CHILDWND;
	shbGroup.hwndChild=hwndChild;

	return SetGroupInfo(hGroup,&shbGroup);
}


///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// All following SetGroup* functions eventually call SetGroupDescriptor function
//

///////////////////////////////////////////////////////////////////////////////

COLORREF COXShortcutBar::GetGroupBkColor(HSHBGROUP hGroup)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo))
	{
		return ID_COLOR_NONE;
	}

	ASSERT(pSavedGroupInfo!=NULL);

	if(pSavedGroupInfo->nMask&SHBIF_DESCRIPTOR && 
		pSavedGroupInfo->pDescriptor->nMask&SHBIF_CLRBACK)
	{
		return pSavedGroupInfo->pDescriptor->clrBackground;
	}
	else
	{
		return GetShortcutBarSkin()->GetBackgroundColor(this);
	}
}

BOOL COXShortcutBar::HasGroupBkColor(HSHBGROUP hGroup)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo))
		return FALSE;

	ASSERT(pSavedGroupInfo!=NULL);

	if(pSavedGroupInfo->nMask&SHBIF_DESCRIPTOR && 
		pSavedGroupInfo->pDescriptor->nMask&SHBIF_CLRBACK)
		return TRUE;
	else
		return FALSE;
}

BOOL COXShortcutBar::SetGroupBkColor(HSHBGROUP hGroup, COLORREF clr)
{
	SHB_DESCRIPTOR descriptor;
	descriptor.nMask=SHBIF_CLRBACK;
	descriptor.clrBackground=clr;

	return SetGroupDescriptor(hGroup,&descriptor);
}

COLORREF COXShortcutBar::GetGroupTextColor(HSHBGROUP hGroup)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo))
	{
		return ID_COLOR_NONE;
	}

	ASSERT(pSavedGroupInfo!=NULL);

	if(pSavedGroupInfo->nMask&SHBIF_DESCRIPTOR && 
		pSavedGroupInfo->pDescriptor->nMask&SHBIF_CLRTEXT)
		return pSavedGroupInfo->pDescriptor->clrText;
	else
		return GetShortcutBarSkin()->GetGroupTextColor(this);
}

BOOL COXShortcutBar::SetGroupTextColor(HSHBGROUP hGroup, COLORREF clr)
{
	SHB_DESCRIPTOR descriptor;
	descriptor.nMask=SHBIF_CLRTEXT;
	descriptor.clrText=clr;

	return SetGroupDescriptor(hGroup,&descriptor);
}

COLORREF COXShortcutBar::GetGroupHeaderBkColor(HSHBGROUP hGroup) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo))
		return ID_COLOR_NONE;

	ASSERT(pSavedGroupInfo!=NULL);

	if(pSavedGroupInfo->nMask&SHBIF_DESCRIPTOR && 
		pSavedGroupInfo->pDescriptor->nMask&SHBIF_HDRCLRBACK)
	{
		return pSavedGroupInfo->pDescriptor->clrHeaderBackground;
	}
	else
	{
		return ::GetSysColor(COLOR_BTNFACE);
	}
}

BOOL COXShortcutBar::SetGroupHeaderBkColor(HSHBGROUP hGroup, COLORREF clr)
{
	SHB_DESCRIPTOR descriptor;
	descriptor.nMask=SHBIF_HDRCLRBACK;
	descriptor.clrHeaderBackground=clr;

	return SetGroupDescriptor(hGroup,&descriptor);
}

COLORREF COXShortcutBar::GetGroupHeaderTextColor(HSHBGROUP hGroup) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo))
	{
		return ID_COLOR_NONE;
	}

	ASSERT(pSavedGroupInfo!=NULL);

	if(pSavedGroupInfo->nMask&SHBIF_DESCRIPTOR && 
		pSavedGroupInfo->pDescriptor->nMask&SHBIF_HDRCLRTEXT)
	{
		return pSavedGroupInfo->pDescriptor->clrHeaderText;
	}
	else
	{
		return ::GetSysColor(COLOR_BTNTEXT);
	}
}

BOOL COXShortcutBar::SetGroupHeaderTextColor(HSHBGROUP hGroup, COLORREF clr)
{
	SHB_DESCRIPTOR descriptor;
	descriptor.nMask=SHBIF_HDRCLRTEXT;
	descriptor.clrHeaderText=clr;

	return SetGroupDescriptor(hGroup,&descriptor);
}

UINT COXShortcutBar::GetGroupHeaderTextFormat(HSHBGROUP hGroup) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo))
	{
		return 0;
	}

	ASSERT(pSavedGroupInfo!=NULL);

	if(pSavedGroupInfo->nMask&SHBIF_DESCRIPTOR && 
		pSavedGroupInfo->pDescriptor->nMask&SHBIF_HDRTEXTFMT)
	{
		return pSavedGroupInfo->pDescriptor->nHeaderTextFormat;
	}
	else
	{
		return DFLT_HDRTEXTFMT;
	}
}

BOOL COXShortcutBar::SetGroupHeaderTextFormat(HSHBGROUP hGroup, UINT nFormat)
{
	SHB_DESCRIPTOR descriptor;
	descriptor.nMask=SHBIF_HDRTEXTFMT;
	descriptor.nHeaderTextFormat=nFormat;

	return SetGroupDescriptor(hGroup,&descriptor);
}

LPLOGFONT COXShortcutBar::GetGroupTextFont(HSHBGROUP hGroup) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo))
	{
		return NULL;
	}

	ASSERT(pSavedGroupInfo!=NULL);

	if(!(pSavedGroupInfo->nMask&SHBIF_DESCRIPTOR && 
		pSavedGroupInfo->pDescriptor->nMask&SHBIF_FONT))
	{
		// we have to declare it as const to comply with const function definition
		const CWnd* pWnd=this;
		// get child window handle
		HWND hwndChild=GetGroupChildWndHandle(hGroup);
		if(hwndChild!=NULL)
		{
			// try child window's font first of all
			pWnd=CWnd::FromHandle(hwndChild);
			if(!pWnd->GetFont())
			{
				pWnd=this;
			}
		}
		// get font of child or this window
		CFont* pFont=pWnd->GetFont();
		if(pFont)
		{
			static LOGFONT lf;
			VERIFY(pFont->GetLogFont(&lf));
			return &lf;
		}
		else
		{
			return NULL;
		}
	}

	return &pSavedGroupInfo->pDescriptor->lfText;
}

BOOL COXShortcutBar::SetGroupTextFont(HSHBGROUP hGroup, const LPLOGFONT pLF)
{
	ASSERT(pLF);

	SHB_DESCRIPTOR descriptor;
	descriptor.nMask=SHBIF_FONT;

	if(!CopyLogFont(&descriptor.lfText,pLF))
	{
		TRACE(_T("COXShortcutBar::SetGroupTextFont: failed to get log font"));
		return FALSE;
	}

	return SetGroupDescriptor(hGroup,&descriptor);
}

BOOL COXShortcutBar::SetGroupTextFont(HSHBGROUP hGroup, CFont* pFont)
{
	ASSERT(pFont);

	SHB_DESCRIPTOR descriptor;
	descriptor.nMask=SHBIF_FONT;

	if(!pFont->GetLogFont(&descriptor.lfText))
	{
		TRACE(_T("COXShortcutBar::SetGroupTextFont: failed to get log font"));
		return FALSE;
	}

	return SetGroupDescriptor(hGroup,&descriptor);
}

LPLOGFONT COXShortcutBar::GetGroupHeaderTextFont(HSHBGROUP hGroup)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo))
	{
		return NULL;
	}

	ASSERT(pSavedGroupInfo!=NULL);

	if(!(pSavedGroupInfo->nMask&SHBIF_DESCRIPTOR && 
		pSavedGroupInfo->pDescriptor->nMask&SHBIF_HDRFONT))
	{
		// use default font
		ASSERT((HFONT)m_fontDefault!=NULL);
		static LOGFONT lf;
		VERIFY(m_fontDefault.GetLogFont(&lf));
		return &lf;
	}

	return &pSavedGroupInfo->pDescriptor->lfHeader;
}

BOOL COXShortcutBar::SetGroupHeaderTextFont(HSHBGROUP hGroup, const LPLOGFONT pLF)
{
	ASSERT(pLF);

	SHB_DESCRIPTOR descriptor;
	descriptor.nMask=SHBIF_HDRFONT;

	if(!CopyLogFont(&descriptor.lfHeader,pLF))
	{
		TRACE(_T("COXShortcutBar::SetGroupHeaderTextFont: failed to get log font"));
		return FALSE;
	}

	return SetGroupDescriptor(hGroup,&descriptor);
}

BOOL COXShortcutBar::SetGroupHeaderTextFont(HSHBGROUP hGroup, CFont* pFont)
{
	ASSERT(pFont);

	SHB_DESCRIPTOR descriptor;
	descriptor.nMask=SHBIF_HDRFONT;

	if(!pFont->GetLogFont(&descriptor.lfHeader))
	{
		TRACE(_T("COXShortcutBar::SetGroupHeaderTextFont: failed to get log font"));
		return FALSE;
	}

	return SetGroupDescriptor(hGroup,&descriptor);
}

int COXShortcutBar::GetGroupHeaderHeight(HSHBGROUP hGroup) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo))
	{
		return -1;
	}

	ASSERT(pSavedGroupInfo!=NULL);

	if(pSavedGroupInfo->nMask&SHBIF_DESCRIPTOR && 
		pSavedGroupInfo->pDescriptor->nMask&SHBIF_HDRHEIGHT)
	{
		return pSavedGroupInfo->pDescriptor->nHeaderHeight;
	}
	else
	{
		COXShortcutBar * me = const_cast<COXShortcutBar *>(this);
		COXShortcutBarSkin * skin = me->GetShortcutBarSkin();
		switch(skin->GetType())
		{
		case OXSkinClassic:
	        	return DFLT_HDRHEIGHT;
			break;
		case OXSkinOffice2003:
			return DFLT_HDRHEIGHT2003;
			break;
		case OXSkinOfficeXP:
			return DFLT_HDRHEIGHTXP;
			break;
		default:
			return DFLT_HDRHEIGHT;
			break;
		}
	}
}

BOOL COXShortcutBar::SetGroupHeaderHeight(HSHBGROUP hGroup, int nHeight)
{
	ASSERT(nHeight>=0);

	SHB_DESCRIPTOR descriptor;
	descriptor.nMask=SHBIF_HDRHEIGHT;
	descriptor.nHeaderHeight=nHeight;

	return SetGroupDescriptor(hGroup,&descriptor);
}
///////////////////////////////////////////////////////////////////////////////

HSHBGROUP COXShortcutBar::InsertGroup(const LPSHB_GROUPINFO pGroupInfo)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(pGroupInfo);

	// fill structure for notification
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_INSERTGROUP;
	nmshb.lParam=(LPARAM)pGroupInfo;

	if(SendSHBNotification(&nmshb))
	{
		return NULL;
	}

	// verify specified structure
	if(!VerifyGroupInfo(pGroupInfo))
	{
		return NULL;
	}

	// request unique handle for new group
	HSHBGROUP hGroup=GetUniqueGroupHandle();
	if(hGroup==NULL)
	{
		return NULL;
	}

	LPSHB_GROUPINFO pSavedGroupInfo;
	try
	{
		pSavedGroupInfo=new SHB_GROUPINFO;
	}
	catch (CMemoryException* pException)
	{
		UNREFERENCED_PARAMETER(pException);
		TRACE(_T("COXShortcutBar::InsertGroup: failed to allocate memory for SHB_GROUPINFO"));
		return NULL;
	}

	ASSERT(pSavedGroupInfo!=NULL);

	// Save supplied group info. Note that the third parameter is set to TRUE, 
	// which means that pGroupInfo will be completely copied to pSavedGroupInfo
	if(!CopyGroupInfo(pSavedGroupInfo,pGroupInfo,SHB_CPYINFO_GET|SHB_CPYINFO_COPY))
	{
		delete pSavedGroupInfo;
		return NULL;
	}

	// check if we have to reorder existing groups
	BOOL bNeedSorting=FALSE;
	if(pSavedGroupInfo->nMask&SHBIF_ORDER)
	{
		switch(pSavedGroupInfo->nOrder)
		{
		case SHBI_FIRST:
			{
				UpdateHeaderOrder(0,-1,1);
				pSavedGroupInfo->nOrder=0;
				break;
			}
		case SHBI_LAST:
			{
				pSavedGroupInfo->nOrder=PtrToInt(GetGroupCount());
				break;
			}
		case SHBI_SORT:
			{
				pSavedGroupInfo->nOrder=PtrToInt(GetGroupCount());
				bNeedSorting=TRUE;
				break;
			}
		default:
			{
				UpdateHeaderOrder(pSavedGroupInfo->nOrder,-1,1);
				pSavedGroupInfo->nOrder=0;
				break;
			}
		}
	}
	else
	{
		pSavedGroupInfo->nMask|=SHBIF_ORDER;
		pSavedGroupInfo->nOrder=PtrToInt(GetGroupCount());
	}

	m_mapHandleToInfo.SetAt(hGroup,pSavedGroupInfo);

	// if we inserted group using SHBI_SORT constant then we have to resort all groups
	if(bNeedSorting)
	{
		if(!SortGroups(1))
		{
			VERIFY(DeleteGroup(hGroup));
			return FALSE;
		}
	}

	if((pSavedGroupInfo->nMask&SHBIF_CHILDWND)==0)
	{
		COXSHBListCtrl* pListCtrl=NULL;
		try
		{
			pListCtrl=NewSHBListCtrl();
		}
		catch (CMemoryException* pException)
		{
			UNREFERENCED_PARAMETER(pException);
			TRACE(_T("COXShortcutBar::InsertGroup: failed to allocate memory for COXSHBListCtrl"));
			VERIFY(DeleteGroup(hGroup));
			return NULL;
		}
	
		m_mapHandleToListCtrl.SetAt(hGroup,pListCtrl);

		// if child window wasn't specified then we have to create default one - 
		// COXSHBListCtrl 
		if(!CreateListControl(hGroup))
		{
			VERIFY(DeleteGroup(hGroup));
			return NULL;
		}
	}
	else
	{
		// send message to child window about aknowledging it and set group to it  
		ASSERT(pSavedGroupInfo->hwndChild);
		::SendMessage(pSavedGroupInfo->hwndChild,SHBM_SETSHBGROUP,
			(WPARAM)0,(LPARAM)hGroup);
	}

	// reposition groups
	CalculateBoundRects();

	return hGroup;
}

HSHBGROUP COXShortcutBar::InsertGroup(LPCTSTR pszText, 
									  int nOrder,  
									  int nImage, 
									  int nImageExpanded, 
									  LPARAM lParam, 
									  int nView, 
									  const LPSHB_DESCRIPTOR pDescriptor, 
									  HWND hwndChild)
{
	SHB_GROUPINFO shbGroup;

	// label text
	if(pszText!=NULL)
	{
		CString sText(pszText);
		shbGroup.nMask|=SHBIF_TEXT;
		shbGroup.nTextMax=sText.GetLength();
		shbGroup.pszText=(LPTSTR)pszText;
	}

	// order of insertion
	if(nOrder!=-1)
	{
		shbGroup.nMask|=SHBIF_ORDER;
		shbGroup.nOrder=nOrder;
	}

	// label image
	if(nImage!=-1)
	{
		shbGroup.nMask|=SHBIF_IMAGE;
		shbGroup.nImage=nImage;
	}

	// label image (expanded)
	if(nImage!=-1)
	{
		shbGroup.nMask|=SHBIF_IMAGEEXPANDED;
		shbGroup.nImageExpanded=nImageExpanded;
	}

	// user defined data
	if(lParam!=NULL)
	{
		shbGroup.nMask|=SHBIF_PARAM;
		shbGroup.lParam=lParam;
	}

	// view type
	if(nView!=-1)
	{
		shbGroup.nMask|=SHBIF_VIEW;
		shbGroup.nView=nView;
	}

	// group description data
	if(pDescriptor!=NULL)
	{
		shbGroup.nMask|=SHBIF_DESCRIPTOR;
		shbGroup.pDescriptor=pDescriptor;
	}

	// child window
	if(hwndChild!=NULL)
	{
		shbGroup.nMask|=SHBIF_CHILDWND;
		shbGroup.hwndChild=hwndChild;
	}

	return InsertGroup(&shbGroup);
}

BOOL COXShortcutBar::DeleteGroup(HSHBGROUP hGroup)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	// fill structure for notification
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_DELETEGROUP;
	nmshb.hGroup=hGroup;

	if(SendSHBNotification(&nmshb))
	{
		return FALSE;
	}

	LPSHB_GROUPINFO pSavedGroupInfo;
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo))
	{
		return FALSE;
	}
	ASSERT(pSavedGroupInfo!=NULL);
	
	int nOrder=GetGroupOrder(hGroup);
	ASSERT(nOrder!=-1);
	UpdateHeaderOrder(nOrder+1,-1,-1);

	// hide any associated child window 
	HideChildWnd(hGroup);

	// notify child window that group is about to be deleted
	HWND hwndChild=GetGroupChildWndHandle(hGroup);
	ASSERT(hwndChild);
	if(::IsWindow(hwndChild))
	{
		::SendMessage(hwndChild,SHBM_DELETESHBGROUP,(WPARAM)0,(LPARAM)0);
	}

	// delete associated group info
	delete pSavedGroupInfo;
	m_mapHandleToInfo.RemoveKey(hGroup);
	m_mapHandleToBoundRect.RemoveKey(hGroup);

	// if we deleted expanded group then we have to expand the next one
	if(GetExpandedGroup()==hGroup)
	{
		int nGroupCount=PtrToInt(GetGroupCount());
		if(nGroupCount==0)
		{
			// if the last group was deleted then we just clean up all info
			m_hExpandedGroup=NULL;
			CalculateBoundRects();
			RedrawWindow();
		}
		else
		{
			// searching for next group to expand
			if(nOrder==nGroupCount)
			{
				nOrder=0;
			}
			else if(nOrder>nGroupCount)
			{
				ASSERT(FALSE);
			}
				
			HSHBGROUP hGroup=FindGroupByOrder(nOrder);
			ASSERT(hGroup);
			ExpandGroup(hGroup);
		}
	}
	else
	{
		// reposition groups and redraw the control
		CalculateBoundRects();
		RedrawBar();
	}

	// delete list control associated with deleted group
	COXSHBListCtrl* pListCtrl=NULL;
	if(m_mapHandleToListCtrl.Lookup(hGroup,pListCtrl) && pListCtrl)
	{
		delete pListCtrl;
		m_mapHandleToListCtrl.RemoveKey(hGroup);
	}

	return TRUE;
}

BOOL COXShortcutBar::DeleteAllGroups()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	// delete all groups in cycle calling DeleteGroup function
	BOOL bResult=TRUE;
	HSHBGROUP hGroup=NULL;
	LPSHB_GROUPINFO pGroupInfo;
	POSITION pos=m_mapHandleToInfo.GetStartPosition();
	while(pos!=NULL)
	{
		m_mapHandleToInfo.GetNextAssoc(pos,hGroup,pGroupInfo);

		ASSERT(hGroup!=NULL);
		ASSERT(pGroupInfo!=NULL);

		bResult=DeleteGroup(hGroup) ? bResult : FALSE;
	}

	VERIFY(m_mapHandleToInfo.GetCount()==0);

	return bResult;
}

HSHBGROUP COXShortcutBar::ExpandGroup(HSHBGROUP hGroup)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	// reset autoexpand timer
	if(m_nWaitForAutoExpandID!=0)
	{
		KillTimer(m_nWaitForAutoExpandID);
		m_nWaitForAutoExpandID=0;
	}
	m_hSuspectAutoExpandGroup=NULL;

	if(hGroup==m_hExpandedGroup)
	{
		return hGroup;
	}

	// fill structure for notification
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_GROUPEXPANDING;
	nmshb.hGroup=hGroup;
	if(SendSHBNotification(&nmshb))
	{
		return m_hExpandedGroup;
	}

	HSHBGROUP hOldExpandedGroup=m_hExpandedGroup;
	m_hExpandedGroup=hGroup;

	COXShortcutBar * me = const_cast<COXShortcutBar *>(this);
	COXShortcutBarSkin * skin = me->GetShortcutBarSkin();
	bool bExpandCells = (skin->AnimateSelection());

	// animation effect
	if(bExpandCells && (GetBarStyle()&SHBS_ANIMATEEXPAND)!=0 && hOldExpandedGroup!=NULL && 
		m_hExpandedGroup!=NULL && !GetChildWndRect().IsRectEmpty())
	{
		int nOrder=GetGroupOrder(m_hExpandedGroup);
		// nOldOrder can be -1 in the case the old expanded group was just deleted
		int nOldOrder=GetGroupOrder(hOldExpandedGroup);
		if(nOldOrder!=-1)
		{
			// get the direction of animated group expanding (up or down)
			BOOL bMovingUp=(nOldOrder<nOrder);

			// get child window handle
			HWND hOldWnd=GetGroupChildWndHandle(hOldExpandedGroup);
			ASSERT(hOldWnd);
			ASSERT(::IsWindow(hOldWnd));

			// get child window rectangle
			CRect rectOldChild=GetChildWndRect();
			CRect rectToRedraw;

			// animation group expanding step size (in pixels)
			int nOffset=DFLT_ANIMATIONOFFSET;
			// the number of steps we need to take to fully animate group expanding
			int nSteps=rectOldChild.Height()/nOffset;

			for(int nIndex=0; nIndex<nSteps-1; nIndex++)
			{
				// speed up expanding after we made it half way
				if(nIndex>nSteps/2)
				{
					nIndex++;
					nOffset=2*DFLT_ANIMATIONOFFSET;
				}

				// recalculate headers rectangles
				rectToRedraw.left=0xffff;
				rectToRedraw.top=0xffff;
				rectToRedraw.right=0;
				rectToRedraw.bottom=0;
				for(int nGroupOrder=__min(nOldOrder,nOrder)+1; 
					nGroupOrder<=__max(nOldOrder,nOrder); nGroupOrder++)
				{
					HSHBGROUP hGroup=FindGroupByOrder(nGroupOrder);
					ASSERT(hGroup);
					CRect rectHeader;
					VERIFY(GetGroupHeaderRect(hGroup,rectHeader));
		
					rectToRedraw.left=__min(rectToRedraw.left,rectHeader.left);
					rectToRedraw.top=__min(rectToRedraw.top,rectHeader.top);
					rectToRedraw.right=__max(rectToRedraw.right,rectHeader.right);
					rectToRedraw.bottom=__max(rectToRedraw.bottom,rectHeader.bottom);

					rectHeader.OffsetRect(0,(bMovingUp) ? -nOffset : nOffset);

					rectToRedraw.top=__min(rectToRedraw.top,rectHeader.top);
					rectToRedraw.bottom=__max(rectToRedraw.bottom,rectHeader.bottom);

					m_mapHandleToBoundRect.SetAt(hGroup,rectHeader);
				}

				// recalculate child window coordinates
				if(bMovingUp)
				{
					rectOldChild.bottom-=nOffset;
				}
				else
				{
					rectOldChild.top+=nOffset;
				}
			
				// set expanded child window rectangle to updated one
				m_rectChildWnd=rectOldChild;

				// resize child window
				::MoveWindow(hOldWnd,rectOldChild.left,rectOldChild.top,
					rectOldChild.Width(),rectOldChild.Height(),FALSE);

				// redraw shortcut bar
				RedrawWindow(rectToRedraw);

				::Sleep(DFLT_ANIMATIONDELAY/nSteps);
			}
		}
	}

	// reposition groups
	CalculateBoundRects();

	// hide child window of previous expanded group
	HideChildWnd(hOldExpandedGroup);
	RedrawWindow();
	// show child window of newly appointed expanded group
	ShowChildWnd(m_hExpandedGroup);

	// notify parent
	// fill structure for notification
	nmshb.hdr.code=SHBN_GROUPEXPANDED;
	nmshb.hGroup=hGroup;
	SendSHBNotification(&nmshb);

	return hOldExpandedGroup;
}

HSHBGROUP COXShortcutBar::SetDropTargetGroup(HSHBGROUP hGroup) 
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	HSHBGROUP hOldDropHighlightGroup=m_hDropHilightGroup;

	m_hDropHilightGroup=hGroup;

	return hOldDropHighlightGroup;
}

HSHBGROUP COXShortcutBar::SetDragGroup(HSHBGROUP hGroup)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	if(m_hDragGroup==hGroup)
		return m_hDragGroup;

	HSHBGROUP hOldDragGroup=m_hDragGroup;

	// fill structure for notification
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_BEGINDRAGHEADER;
	nmshb.hGroup=hGroup;
	nmshb.lParam=(LPARAM)&m_ptDragImage;

	SendSHBNotification(&nmshb);

	m_hDragGroup=hGroup;

	return hOldDragGroup;
}

COXSHBEdit* COXShortcutBar::EditGroupHeader(HSHBGROUP hGroup)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);
	ASSERT(::IsWindow(m_edit.GetSafeHwnd()));

	// check if we can edit headers
	if((GetBarStyle()&SHBS_EDITHEADERS)==0)
	{
		TRACE(_T("COXShortcutBar::EditGroupHeader: cannot edit header - SHBS_EDITHEADERS style wasn't set"));
		return NULL;
	}

	// stop editing any group if any is undergoing
	if(m_hEditGroup!=NULL)
	{
		m_edit.FinishEdit(TRUE);
		m_hEditGroup=NULL;
	}


	// get text
	CString sText=GetGroupText(hGroup);

	// fill structure for notification
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_BEGINHEADEREDIT;
	nmshb.hGroup=hGroup;
	nmshb.lParam=(LPARAM)((LPCTSTR)sText);

	if(SendSHBNotification(&nmshb))
	{
		return NULL;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	CRect rectHeader;
	if(!GetGroupHeaderRect(hGroup,rectHeader))
	{
		TRACE(_T("COXShortcutBar::EditGroupHeader: failed to get header rectangle"));
		return NULL;
	}

	if(rectHeader.IsRectEmpty())
	{
		TRACE(_T("COXShortcutBar::EditGroupHeader: header rect is empty"));
		return NULL;
	}

	CRect rectIntersect;
	rectIntersect.IntersectRect(rectClient,rectHeader);
	if(rectIntersect!=rectHeader)
	{
		TRACE(_T("COXShortcutBar::EditGroupHeader: failed to edit - header rectangle is not entirely visible"));
		return NULL;
	}

	// get font to be used in COXSHBEdit control
	LPLOGFONT pLF=GetGroupHeaderTextFont(hGroup);
	ASSERT(pLF);
	CFont font;
	VERIFY(font.CreateFontIndirect(pLF));

	// fill SHBE_DISPLAY structure to specify COXSHBEdit control options
	SHBE_DISPLAY shbed;
	shbed.nMask=SHBEIF_TEXT|SHBEIF_RECTDISPLAY|SHBEIF_STYLE|
		SHBEIF_SELRANGE|SHBEIF_FONT;
	shbed.lpText=sText;
	shbed.rectDisplay=rectHeader;
	shbed.dwStyle=WS_BORDER|ES_AUTOHSCROLL|ES_LEFT;
	shbed.ptSelRange=CPoint(0,-1);
	shbed.pFont=&font;

	if(!m_edit.StartEdit(&shbed))
	{
		TRACE(_T("COXShortcutBar::EditGroupHeader: COXSHBEdit::StartEdit failed"));
		return NULL;
	}

	m_hEditGroup=hGroup;

	return &m_edit;
}

HSHBGROUP COXShortcutBar::HitTest(CPoint pt, UINT* pFlags) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	*pFlags=SHB_NOWHERE;
	HSHBGROUP hGroup=NULL;

	// first of all check if point is within child window rect
	if(!m_rectChildWnd.IsRectEmpty() && m_rectChildWnd.PtInRect(pt))
	{
		hGroup=GetExpandedGroup();
		ASSERT(hGroup);
		*pFlags=SHB_ONCHILD;
	}
	else
	{
		// check if point is on some of headers
		CRect rect;
		POSITION pos=m_mapHandleToBoundRect.GetStartPosition();
		while(pos!=NULL)
		{
			m_mapHandleToBoundRect.GetNextAssoc(pos,hGroup,rect);

			ASSERT(hGroup!=NULL);

			if(rect.PtInRect(pt))
			{
				*pFlags=SHB_ONHEADER;
				break;
			}
			else
			{
				hGroup=NULL;
			}
		}
	}

	return hGroup;
}

CImageList* COXShortcutBar::CreateGroupDragImage(HSHBGROUP hGroup)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	m_bCreatingDragImage=TRUE;

	// IMPORTANT //
	// caller of this function have to eventually destroy the image list
	//
	CImageList* m_pDragImageList=new CImageList;

	CRect rectHeader;
	VERIFY(GetGroupHeaderRect(hGroup,&rectHeader));

	m_pDragImageList->Create(rectHeader.Width(),rectHeader.Height(),ILC_COLOR,0,1);

	CClientDC dcClient(this);
	CDC dc;
	VERIFY(dc.CreateCompatibleDC(&dcClient));
	
	CBitmap bitmap;
	VERIFY(bitmap.CreateCompatibleBitmap(&dcClient,rectHeader.Width(),
		rectHeader.Height()));
	CBitmap* pOldBitmap=dc.SelectObject(&bitmap);

	rectHeader-=rectHeader.TopLeft();

	// use header drawing routine to get drag image
	//
	DRAWITEMSTRUCT dis;
	dis.CtlType=0;
	dis.CtlID=GetDlgCtrlID();
	dis.itemID=(UINT)PtrToUint(hGroup);
	dis.itemAction=ODA_DRAWENTIRE;
	dis.itemState=ODS_DEFAULT;
	dis.hwndItem=GetSafeHwnd();
	dis.hDC=dc;
	dis.rcItem=rectHeader;
	dis.itemData=(DWORD)PtrToUlong(hGroup);
	DrawHeader(&dis);

	if(pOldBitmap)
		dc.SelectObject(pOldBitmap);

	VERIFY(m_pDragImageList->Add(&bitmap,(CBitmap*)NULL)!=-1);

	m_bCreatingDragImage=FALSE;

	return m_pDragImageList;
}

BOOL COXShortcutBar::SortGroups(int nSortOrder)
{
	// use CompareHeaders function to sort groups in alphabetical order
	return SortGroupsCB(CompareHeaders,(LPARAM)this,nSortOrder);
}
	
BOOL COXShortcutBar::SortGroupsCB(PFNSHBCOMPARE lpfnCompare, 
								  LPARAM lParamSort, int nSortOrder)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(lpfnCompare);
	ASSERT(nSortOrder==0 || nSortOrder==1 || nSortOrder==-1);

	m_nSortOrder=nSortOrder;

	int nCount=PtrToInt(GetGroupCount());
	if(GetGroupCount()<2 || nSortOrder==0)
	{
		return TRUE;
	}

	HSHBGROUP hGroup1;
	HSHBGROUP hGroup2;

	BOOL bNotDone=TRUE;
	while(bNotDone)
	{
		bNotDone=FALSE;
		// loop through all groups
		for(int nIndex=0; nIndex<nCount-1; nIndex++)
		{
			hGroup1=FindGroupByOrder(nIndex);
			hGroup2=FindGroupByOrder(nIndex+1);

			ASSERT(hGroup1!=NULL && hGroup2!=NULL);

			int nResult=nSortOrder*
				lpfnCompare((LPARAM)hGroup1,(LPARAM)hGroup2,lParamSort);
			if(nResult>0)
			{
				// actually we swap items in this function
				SetGroupOrder(hGroup1,nIndex+1);
				bNotDone=TRUE;
			}
		}
	}

	return TRUE;
}

	
// functions to work with group list controls (defined only if programmer didn't set 
// hwndChild member of SHB_GROUPINFO structure)
//

CImageList* COXShortcutBar::SetLCImageList(HSHBGROUP hGroup, 
										   CImageList* pImageList, 
										   int nImageList)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	COXSHBListCtrl* pListCtrl=GetGroupListCtrl(hGroup);
	if(pListCtrl==NULL)
	{
		TRACE(_T("COXShortcutBar::SetLCImageList: list control wasn't created"));
		return NULL;
	}

	ASSERT(GetGroupChildWnd(hGroup)==NULL);
	ASSERT_VALID(pListCtrl);

	return pListCtrl->SetImageList(pImageList,nImageList);
}

CImageList* COXShortcutBar::GetLCImageList(HSHBGROUP hGroup, int nImageList) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	COXSHBListCtrl* pListCtrl=GetGroupListCtrl(hGroup);
	if(pListCtrl==NULL)
	{
		TRACE(_T("COXShortcutBar::GetLCImageList: list control wasn't created"));
		return NULL;
	}

	ASSERT(GetGroupChildWnd(hGroup)==NULL);
	ASSERT_VALID(pListCtrl);

	return pListCtrl->GetImageList(nImageList);
}

BOOL COXShortcutBar::GetLCItem(HSHBGROUP hGroup, LV_ITEM* pItem) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);
	ASSERT(pItem);

	COXSHBListCtrl* pListCtrl=GetGroupListCtrl(hGroup);
	if(pListCtrl==NULL)
	{
		TRACE(_T("COXShortcutBar::GetLCItem: list control wasn't created"));
		return FALSE;
	}

	ASSERT(GetGroupChildWnd(hGroup)==NULL);
	ASSERT_VALID(pListCtrl);

	return pListCtrl->GetItem(pItem);
}

int COXShortcutBar::GetLCItemCount(HSHBGROUP hGroup) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	COXSHBListCtrl* pListCtrl=GetGroupListCtrl(hGroup);
	if(pListCtrl==NULL)
	{
		TRACE(_T("COXShortcutBar::GetLCItemCount: list control wasn't created"));
		return FALSE;
	}

	ASSERT(GetGroupChildWnd(hGroup)==NULL);
	ASSERT_VALID(pListCtrl);

	return pListCtrl->GetItemCount();
}

BOOL COXShortcutBar::SetLCItem(HSHBGROUP hGroup, const LV_ITEM* pItem)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);
	ASSERT(pItem);

	COXSHBListCtrl* pListCtrl=GetGroupListCtrl(hGroup);
	if(pListCtrl==NULL)
	{
		TRACE(_T("COXShortcutBar::SetLCItem: list control wasn't created"));
		return FALSE;
	}

	ASSERT(GetGroupChildWnd(hGroup)==NULL);
	ASSERT_VALID(pListCtrl);

	return pListCtrl->SetItem(pItem);
}

BOOL COXShortcutBar::SetLCItem(HSHBGROUP hGroup, int nItem, int nSubItem, 
							   UINT nMask, LPCTSTR lpszItem, int nImage, 
							   UINT nState, UINT nStateMask, LPARAM lParam)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	COXSHBListCtrl* pListCtrl=GetGroupListCtrl(hGroup);
	if(pListCtrl==NULL)
	{
		TRACE(_T("COXShortcutBar::SetLCItem: list control wasn't created"));
		return FALSE;
	}

	ASSERT(GetGroupChildWnd(hGroup)==NULL);
	ASSERT_VALID(pListCtrl);

	return pListCtrl->SetItem(nItem,nSubItem,nMask,lpszItem,nImage,
		nState,nStateMask,lParam);
}

int COXShortcutBar::InsertLCItem(HSHBGROUP hGroup, const LV_ITEM* pItem)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);
	ASSERT(pItem);

	COXSHBListCtrl* pListCtrl=GetGroupListCtrl(hGroup);
	if(pListCtrl==NULL)
	{
		TRACE(_T("COXShortcutBar::InsertLCItem: list control wasn't created"));
		return -1;
	}

	ASSERT(GetGroupChildWnd(hGroup)==NULL);
	ASSERT_VALID(pListCtrl);

	return pListCtrl->InsertItem(pItem);
}

int COXShortcutBar::InsertLCItem(HSHBGROUP hGroup, UINT nMask, int nItem, 
								 LPCTSTR lpszItem, UINT nState, UINT nStateMask,
								 int nImage, LPARAM lParam)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	COXSHBListCtrl* pListCtrl=GetGroupListCtrl(hGroup);
	if(pListCtrl==NULL)
	{
		TRACE(_T("COXShortcutBar::InsertLCItem: list control wasn't created"));
		return -1;
	}

	ASSERT(GetGroupChildWnd(hGroup)==NULL);
	ASSERT_VALID(pListCtrl);

	return pListCtrl->InsertItem(
		nMask,nItem,lpszItem,nState,nStateMask,nImage,lParam);
}

BOOL COXShortcutBar::DeleteLCItem(HSHBGROUP hGroup, int nItem)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	COXSHBListCtrl* pListCtrl=GetGroupListCtrl(hGroup);
	if(pListCtrl==NULL)
	{
		TRACE(_T("COXShortcutBar::InsertLCItem: list control wasn't created"));
		return FALSE;
	}

	ASSERT(GetGroupChildWnd(hGroup)==NULL);
	ASSERT_VALID(pListCtrl);
	ASSERT(nItem>=0 && nItem<pListCtrl->GetItemCount());

	return pListCtrl->DeleteItem(nItem);
}

COXSHBEdit* COXShortcutBar::EditLCItem(HSHBGROUP hGroup, int nItem)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	// check if we are allowed to edit items
	if((GetBarStyle()&SHBS_EDITITEMS)==0)
	{
		TRACE(_T("COXShortcutBar::EditLCItem: cannot edit item - SHBS_EDITITEMS style wasn't set"));
		return NULL;
	}

	COXSHBListCtrl* pListCtrl=GetGroupListCtrl(hGroup);
	if(pListCtrl==NULL)
	{
		TRACE(_T("COXShortcutBar::EditLCItem: list control wasn't created"));
		return NULL;
	}

	ASSERT(GetGroupChildWnd(hGroup)==NULL);
	ASSERT_VALID(pListCtrl);
	ASSERT(nItem>=0 && nItem<pListCtrl->GetItemCount());

	return pListCtrl->EditLabel(nItem);
}

BOOL COXShortcutBar::ScrollExpandedLC(CSize size)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	// get expanded group
	HSHBGROUP hGroup=GetExpandedGroup();
	if(hGroup==NULL)
		return FALSE;

	HWND hWnd=GetGroupChildWnd(hGroup);
	if(hWnd==NULL)
	{
		COXSHBListCtrl* pListCtrl=GetGroupListCtrl(hGroup);
		ASSERT(pListCtrl);
		return pListCtrl->Scroll(size);
	}

	TRACE(_T("COXShortcutBar::ScrollExpanded: child window is not COXSHBListCtrl\n"));
	return FALSE;
}

int COXShortcutBar::SortLCItems(HSHBGROUP hGroup)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	COXSHBListCtrl* pListCtrl=GetGroupListCtrl(hGroup);
	if(pListCtrl==NULL)
	{
		TRACE(_T("COXShortcutBar::SortLCItems: list control wasn't created\n"));
		return -1;
	}

	ASSERT(GetGroupChildWnd(hGroup)==NULL);
	ASSERT_VALID(pListCtrl);

	// Sort all items in ascending aphabetical order using an STL set
	typedef std::set<LVITEM*, LVITEM_less> ItemSet;
	ItemSet setItems;
	int iCount = pListCtrl->GetItemCount();
	for (int i = 0; i < iCount; i++)
	{
		LVITEM* pLVI = new LVITEM();
		::memset(pLVI, 0, sizeof(LVITEM));
		pLVI->iItem = i;
		pLVI->mask = LVIF_IMAGE | LVIF_INDENT | LVIF_PARAM | LVIF_STATE | LVIF_TEXT;
		pLVI->pszText = new TCHAR[256];
		pLVI->cchTextMax = 256;
		pListCtrl->GetItem(pLVI);

		setItems.insert(pLVI);
	}

	// Remove all items from the list control
	pListCtrl->DeleteAllItems();

	// Put the items back in the list control
	int iIndex = 0;
	for (ItemSet::iterator it = setItems.begin(); it != setItems.end(); ++it)
	{
		(*it)->iItem = iIndex++;
		pListCtrl->InsertItem(*it);
		delete [] (*it)->pszText;
		delete *it;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////

// Set of verification functions that check different parameters on consistent
////////////////////////////////////////////////////////////////////

BOOL COXShortcutBar::VerifyBarStyle(DWORD dwBarStyle) const
{
	if(dwBarStyle & ~(SHBS_EDITHEADERS|SHBS_EDITITEMS|SHBS_INFOTIP|
		SHBS_DISABLEDRAGDROPITEM|SHBS_NOSCROLL|SHBS_BOLDEXPANDEDGROUP|
		SHBS_UNDERLINEHOTITEM|SHBS_SHOWACTIVEALWAYS|
		SHBS_DISABLEDRAGDROPHEADER|SHBS_DRAWHEADERDRAGIMAGE|
		SHBS_DRAWITEMDRAGIMAGE|SHBS_AUTOSCROLL|
		SHBS_ANIMATEEXPAND|SHBS_AUTOEXPAND|SHBS_FLATGROUPBUTTON))
	{
		TRACE(_T("COXShortcutBar::VerifyBarStyle: unspecified Shortcut Bar style was used\n"));
		return FALSE;
	}

	return TRUE;
}

BOOL COXShortcutBar::VerifyWindowStyle(DWORD dwStyle) const
{
	if((dwStyle & WS_CHILD)==0)
	{
		TRACE(_T("COXShortcutBar::VerifyWindowStyle: WS_CHILD style must be specified\n"));
		return FALSE;
	}

	return TRUE;
}

BOOL COXShortcutBar::VerifyImage(int nImage) const
{
	if(!m_pImageList)
	{
		TRACE(_T("COXShortcutBar::VerifyImage: image list wasn't set"));
		return FALSE;
	}
	// image index has to be in the range of possible values
	if(nImage<0 || nImage>=m_pImageList->GetImageCount())
	{
		TRACE(_T("COXShortcutBar::VerifyImage: image number %d is out of range: 0 ... %d\n"),
			nImage,m_pImageList->GetImageCount());
		return FALSE;
	}

	return TRUE;
}

BOOL COXShortcutBar::VerifyView(int nView) const
{
	// only two types of view at the moment available
	if(nView&~(SHB_LARGEICON|SHB_SMALLICON))
	{
		TRACE(_T("COXShortcutBar::VerifyView: unknown view specified"));
		return FALSE;
	}

	return TRUE;
}

BOOL COXShortcutBar::VerifyOrder(int nOrder) const
{
	if((nOrder!=SHBI_FIRST && nOrder!=SHBI_LAST && nOrder!=SHBI_SORT) && 
		(nOrder<0 || nOrder>=GetGroupCount()))
	{
		TRACE(_T("COXShortcutBar::VerifyOrder: order number %d is out of range: 0 ... %d\n"),
			nOrder,GetGroupCount());
		return FALSE;
	}

	return TRUE;
}

BOOL COXShortcutBar::VerifyDescriptor(LPSHB_DESCRIPTOR pDescriptor) const
{
	UNREFERENCED_PARAMETER(pDescriptor);

	ASSERT(pDescriptor);
	return TRUE;
}

BOOL COXShortcutBar::VerifyChildWnd(HWND hwndChild) const
{
	if(!::IsWindow(hwndChild))
	{
		TRACE(_T("COXShortcutBar::VerifyChildWnd: window wasn't created\n"));
		return FALSE;
	}

	DWORD dwStyle=::GetWindowLongPtr(hwndChild,GWL_STYLE);
	if((dwStyle&WS_CHILD)==0)
	{
		TRACE(_T("COXShortcutBar::VerifyChildWnd: WS_CHILD style must be specified\n"));
		return FALSE;
	}

	HWND hwndParent=::GetParent(hwndChild);
	if(hwndParent!=GetSafeHwnd())
	{
		TRACE(_T("COXShortcutBar::VerifyChildWnd: child window parent window is not <this>\n"));
		return FALSE;
	}

	return TRUE;
}

BOOL COXShortcutBar::VerifyMask(UINT nMask) const
{
	if(nMask&~(SHBIF_TEXT|SHBIF_IMAGE|SHBIF_IMAGEEXPANDED|SHBIF_PARAM|
		SHBIF_ORDER|SHBIF_VIEW|SHBIF_DESCRIPTOR|SHBIF_CHILDWND))
	{
		TRACE(_T("COXShortcutBar::VerifyMask: unknown mask specified"));
		return FALSE;
	}

	return TRUE;
}

BOOL COXShortcutBar::VerifyGroupInfo(LPSHB_GROUPINFO pGroupInfo)  const
{
	if(!VerifyMask(pGroupInfo->nMask))
		return FALSE;

	if(pGroupInfo->nMask&SHBIF_IMAGE && !VerifyImage(pGroupInfo->nImage))
		return FALSE;

	if(pGroupInfo->nMask&SHBIF_IMAGEEXPANDED && 
		!VerifyImage(pGroupInfo->nImageExpanded))
		return FALSE;

	if(pGroupInfo->nMask&SHBIF_ORDER && !VerifyOrder(pGroupInfo->nOrder))
		return FALSE;

	if(pGroupInfo->nMask&SHBIF_VIEW && !VerifyView(pGroupInfo->nView))
		return FALSE;

	if(pGroupInfo->nMask&SHBIF_DESCRIPTOR && 
		!VerifyDescriptor(pGroupInfo->pDescriptor))
		return FALSE;

	if(pGroupInfo->nMask&SHBIF_CHILDWND && !VerifyChildWnd(pGroupInfo->hwndChild))
		return FALSE;

	return TRUE;
}

HSHBGROUP COXShortcutBar::GetUniqueGroupHandle()
{
	// we just keep track of all created groups and just increment the number 
	// of created groups to get a unique handle
	m_nLastHandle++;
	return (HSHBGROUP)m_nLastHandle;
}

BOOL COXShortcutBar::CopyGroupInfo(LPSHB_GROUPINFO pDest, 
								   const LPSHB_GROUPINFO pSource, 
								   int nCopyFlag/* = SHB_CPYINFO_SET*/)
{
	ASSERT(pDest);
	ASSERT(pSource);
	// only these flags can be set
	ASSERT((nCopyFlag&~(SHB_CPYINFO_COPY|SHB_CPYINFO_GET|SHB_CPYINFO_SET))==0);
	// these two flags cannot be set simultenuosly
	ASSERT((nCopyFlag&SHB_CPYINFO_SET)==0 || (nCopyFlag&SHB_CPYINFO_GET)==0);

	// verify dest if we are going to get info from source
	if((nCopyFlag&SHB_CPYINFO_GET)!=0)
		VERIFY(VerifyGroupInfo(pDest));

	// check if we copy source to dest with removing all existing dest contents
	if((nCopyFlag&SHB_CPYINFO_COPY)!=0)
	{
		// delete text if it was dynamically allocated
		if(pDest->IsDynCreated() && pDest->pszText!=NULL)
		{
			delete[] pDest->pszText;
			pDest->pszText=NULL;
			pDest->DynCreated(FALSE);
		}
		ZeroMemory(pDest,sizeof(SHB_GROUPINFO));
	}

	// define the group which mask will be used to copy information
	LPSHB_GROUPINFO pMainGroup=(((nCopyFlag&SHB_CPYINFO_SET)!=0 || 
		(nCopyFlag&SHB_CPYINFO_COPY)!=0) ? pSource : pDest);
	// define whether we are going to get or set information  
	BOOL bGetInfo=((nCopyFlag&SHB_CPYINFO_GET)!=0 ? TRUE : FALSE);

	// set header text
	if(pMainGroup->nMask&SHBIF_TEXT)
	{
		// delete any text that was previously dynamically allocated
		if(pDest->IsDynCreated() && pDest->pszText!=NULL)
		{
			delete[] pDest->pszText;
			pDest->pszText=NULL;
			pDest->DynCreated(FALSE);
		}

		// copy text
		if(pSource->pszText!=NULL && pSource->nTextMax>0)
		{
			try
			{
				pDest->pszText=new TCHAR[pSource->nTextMax + 1];
			}
			catch (CMemoryException* pException)
			{
				UNREFERENCED_PARAMETER(pException);
				TRACE(_T("COXShortcutBar::CopyGroupInfo: failed to allocate memory for text"));
				pDest->pszText=NULL;
				return FALSE;
			}
			ASSERT(pDest->pszText);
			pDest->DynCreated();
			UTBStr::tcsncpy(pDest->pszText, pSource->nTextMax + 1, pSource->pszText, pSource->nTextMax);
			pDest->nTextMax=pSource->nTextMax;
		}
		pDest->nMask|=SHBIF_TEXT;
	}

	// set header image
	if(pMainGroup->nMask&SHBIF_IMAGE)
	{
		if(pSource->nImage>=0 && !VerifyImage(pSource->nImage))
			return FALSE;

		// check if property is to set/remove
		if(pSource->nImage<0)
		{
			pDest->nImage=-1;
			pDest->nMask&=~SHBIF_IMAGE;
		}
		else
		{
			pDest->nImage=pSource->nImage;
			pDest->nMask|=SHBIF_IMAGE;
		}
	}

	// set header expanded image
	if(pMainGroup->nMask&SHBIF_IMAGEEXPANDED)
	{
		if(pSource->nImageExpanded>=0 && !VerifyImage(pSource->nImageExpanded))
			return FALSE;

		// check if property is to set/remove
		if(pSource->nImageExpanded<0)
		{
			pDest->nImageExpanded=-1;
			pDest->nMask&=~SHBIF_IMAGEEXPANDED;
		}
		else
		{
			pDest->nImageExpanded=pSource->nImageExpanded;
			pDest->nMask|=SHBIF_IMAGEEXPANDED;
		}
	}

	// set user specific data
	if(pMainGroup->nMask&SHBIF_PARAM)
	{
		pDest->lParam=pSource->lParam;
		pDest->nMask|=SHBIF_PARAM;
	}

	// set order
	if(pMainGroup->nMask&SHBIF_ORDER)
	{
		if(!VerifyOrder(pSource->nOrder))
			return FALSE;

		// reorder groups if infromation is to be set
		if(!bGetInfo)
		{
			int nOldOrder=pDest->nOrder;

			if(nOldOrder!=pSource->nOrder)
			{
				HSHBGROUP hSwapGroup=FindGroupByOrder(pSource->nOrder);
				ASSERT(hSwapGroup!=NULL);

				LPSHB_GROUPINFO pSwapSavedGroupInfo;
				VERIFY(m_mapHandleToInfo.Lookup(hSwapGroup,pSwapSavedGroupInfo));
				ASSERT(pSwapSavedGroupInfo!=NULL);

				pSwapSavedGroupInfo->nMask|=SHBIF_ORDER;
				pSwapSavedGroupInfo->nOrder=nOldOrder;
			}
		}

		pDest->nOrder=pSource->nOrder;
		pDest->nMask|=SHBIF_ORDER;
	}

	// set view
	if(pMainGroup->nMask&SHBIF_VIEW )
	{
		if(!VerifyView(pSource->nView))
			return FALSE;

		pDest->nView=pSource->nView;
		pDest->nMask|=SHBIF_VIEW;
	}

	// child window
	if(pMainGroup->nMask&SHBIF_CHILDWND)
	{
		// check if property is to set/remove
		if(pSource->hwndChild==NULL)
		{
			pDest->nMask&=~SHBIF_CHILDWND;
			pDest->hwndChild=pSource->hwndChild;
		}
		else
		{
			// verify child window
			if(!VerifyChildWnd(pSource->hwndChild))
				return FALSE;

			pDest->nMask|=SHBIF_CHILDWND;
			pDest->hwndChild=pSource->hwndChild;
		}
	}

	// set descriptor 
	if(pMainGroup->nMask&SHBIF_DESCRIPTOR)
	{
		// check if property is to set/remove
		if(pSource->pDescriptor!=NULL)
		{
			int nDescriptorCopyFlag=nCopyFlag&~SHB_CPYINFO_COPY;

			if(pDest->pDescriptor==NULL)
			{
				// create descriptor if haven't been created yet
				pDest->pDescriptor=AddDescriptor();
				nDescriptorCopyFlag|=SHB_CPYINFO_COPY;
			}
			ASSERT(pDest->pDescriptor);

			// copy sorce descriptor to dest decriptor
			if(!CopyDescriptor(pDest->pDescriptor,pSource->pDescriptor,
				nDescriptorCopyFlag))
				return FALSE;
			pDest->nMask|=SHBIF_DESCRIPTOR;
		}
		else
		{
			// remove descriptor
			if(pDest->pDescriptor!=NULL)
			{
				// try to find group by descriptor
				SHB_GROUPINFO shbGroup;
				shbGroup.nMask=SHBIF_DESCRIPTOR;
				shbGroup.pDescriptor=pDest->pDescriptor;
				HSHBGROUP hFindGroup=FindGroup(&shbGroup);
				// if find any 
				if(hFindGroup==NULL)
				{
					int nOrder;
					UNREFERENCED_PARAMETER(nOrder);
					// double check that descriptor was created using AddDescriptor
					ASSERT(m_mapDescriptors.Lookup(pDest->pDescriptor,nOrder));
					delete pDest->pDescriptor;
					m_mapDescriptors.RemoveKey(pDest->pDescriptor);
				}
			}

			pDest->pDescriptor=NULL;
			pDest->nMask&=~SHBIF_DESCRIPTOR;
		}
	}

	return TRUE;
}

BOOL COXShortcutBar::CopyDescriptor(LPSHB_DESCRIPTOR pDest, 
									const LPSHB_DESCRIPTOR pSource, 
									int nCopyFlag/* = SHB_CPYINFO_SET*/) const
{
	ASSERT(pDest);
	ASSERT(pSource);
	// only these flags can be set
	ASSERT((nCopyFlag&~(SHB_CPYINFO_COPY|SHB_CPYINFO_GET|SHB_CPYINFO_SET))==0);
	// these two flags cannot be set simultenuosly
	ASSERT((nCopyFlag&SHB_CPYINFO_SET)==0 || (nCopyFlag&SHB_CPYINFO_GET)==0);

	// verify source and if needed dest descriptors
	VERIFY(VerifyDescriptor(pSource));
	if((nCopyFlag&SHB_CPYINFO_GET)!=0)
		VERIFY(VerifyDescriptor(pDest));

	// clean up the dest if SHB_CPYINFO_COPY flag is set
	if((nCopyFlag&SHB_CPYINFO_COPY)!=0)
		ZeroMemory(pDest,sizeof(SHB_DESCRIPTOR));

	// define descriptor which mask will be used to copy information
	LPSHB_DESCRIPTOR pMainDescriptor=(((nCopyFlag&SHB_CPYINFO_SET)!=0 || 
		(nCopyFlag&SHB_CPYINFO_COPY)!=0) ? pSource : pDest);

	if(pMainDescriptor->nMask&SHBIF_CLRBACK)
	{
		pDest->clrBackground=pSource->clrBackground;
		pDest->nMask|=SHBIF_CLRBACK;
	}

	if(pMainDescriptor->nMask&SHBIF_CLRTEXT)
	{
		pDest->clrText=pSource->clrText;
		pDest->nMask|=SHBIF_CLRTEXT;
	}

	if(pMainDescriptor->nMask&SHBIF_HDRCLRBACK)
	{
		pDest->clrHeaderBackground=pSource->clrHeaderBackground;
		pDest->nMask|=SHBIF_HDRCLRBACK;
	}

	if(pMainDescriptor->nMask&SHBIF_HDRCLRTEXT)
	{
		pDest->clrHeaderText=pSource->clrHeaderText;
		pDest->nMask|=SHBIF_HDRCLRTEXT;
	}

	if(pMainDescriptor->nMask&SHBIF_HDRTEXTFMT)
	{
		pDest->nHeaderTextFormat=pSource->nHeaderTextFormat;
		pDest->nMask|=SHBIF_HDRTEXTFMT;
	}

	if(pMainDescriptor->nMask&SHBIF_HDRHEIGHT)
	{
		pDest->nHeaderHeight=pSource->nHeaderHeight;
		pDest->nMask|=SHBIF_HDRHEIGHT;
	}

	if(pMainDescriptor->nMask&SHBIF_FONT)
	{
		VERIFY(CopyLogFont(&pDest->lfText,&pSource->lfText));
		pDest->nMask|=SHBIF_FONT;
	}

	if(pMainDescriptor->nMask&SHBIF_HDRFONT)
	{
		VERIFY(CopyLogFont(&pDest->lfHeader,&pSource->lfHeader));
		pDest->nMask|=SHBIF_HDRFONT;
	}

	
	return TRUE;
}

BOOL COXShortcutBar::CopyLogFont(LPLOGFONT pDest, const LPLOGFONT pSource) const
{
	ASSERT(pDest);
	ASSERT(pSource);

	// copy all elements of LOGFONT structure
	pDest->lfHeight=pSource->lfHeight;
	pDest->lfWidth=pSource->lfWidth;
	pDest->lfEscapement=pSource->lfEscapement; 
	pDest->lfOrientation=pSource->lfOrientation; 
	pDest->lfWeight=pSource->lfWeight;
	pDest->lfItalic=pSource->lfItalic;
	pDest->lfUnderline=pSource->lfUnderline;
	pDest->lfStrikeOut=pSource->lfStrikeOut;
	pDest->lfCharSet=pSource->lfCharSet;
	pDest->lfOutPrecision=pSource->lfOutPrecision;
	pDest->lfClipPrecision=pSource->lfClipPrecision;
	pDest->lfQuality=pSource->lfQuality;
	pDest->lfPitchAndFamily=pSource->lfPitchAndFamily;
	UTBStr::tcsncpy(pDest->lfFaceName, LF_FACESIZE, pSource->lfFaceName,LF_FACESIZE);

	return TRUE;
}

BOOL COXShortcutBar::CreateEditControl()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	if(::IsWindow(m_edit.GetSafeHwnd()))
	{
		return TRUE;
	}

	// make sure it's invisible from the beginning
	CRect rect(0,0,0,0);
	BOOL bResult=m_edit.Create(WS_BORDER,rect,this,SHB_IDCEDIT);

	return bResult;
}

BOOL COXShortcutBar::CreateListControl(HSHBGROUP hGroup)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(hGroup);

	LPSHB_GROUPINFO pSavedGroupInfo;
	// group has to be created at the moment
	if(!m_mapHandleToInfo.Lookup(hGroup,pSavedGroupInfo))
		return FALSE;
	ASSERT(pSavedGroupInfo!=NULL);

	// double check group info
	ASSERT(VerifyGroupInfo(pSavedGroupInfo));

	// if list control already exist then destroy it
	COXSHBListCtrl* pListCtrl=NULL;
	m_mapHandleToListCtrl.Lookup(hGroup,pListCtrl);
	if(pListCtrl)
		pListCtrl->DestroyWindow();

	// default styles that are compatible with default shortcut bar styles
	DWORD dwStyle=LVS_SINGLESEL|LVS_SHAREIMAGELISTS|LVS_NOSCROLL|LVS_ICON;

	// create initially invisible COXSHBListCtrl
	CRect rect(0,0,0,0);
	if(pListCtrl->Create(dwStyle,rect,this,(UINT)PtrToUint(hGroup)))
	{
		// notify list control that we aknowledge it as child window
		// and send as lParam the handle of the corresponding group
		pListCtrl->SendMessage(SHBM_SETSHBGROUP,(WPARAM)0,(LPARAM)hGroup);
		return TRUE;
	}
	else
		return FALSE;
}

COXSHBListCtrl* COXShortcutBar::NewSHBListCtrl()
{
	return (new COXSHBListCtrl());
}

void COXShortcutBar::UpdateHeaderOrder(int nFirst, int nLast, int nMargin)
{
	// if nLast==-1 then we loop till the last group
	if(nLast==-1)
		nLast=PtrToInt(GetGroupCount()-1);

	// scan through all groups and update order of them correspondingly
	HSHBGROUP hGroup;
	LPSHB_GROUPINFO pGroupInfo;
	POSITION pos=m_mapHandleToInfo.GetStartPosition();
	while(pos!=NULL)
	{
		m_mapHandleToInfo.GetNextAssoc(pos,hGroup,pGroupInfo);

		ASSERT(hGroup!=NULL);
		ASSERT(pGroupInfo!=NULL);

		if(pGroupInfo->nOrder>=nFirst && pGroupInfo->nOrder<=nLast)
			pGroupInfo->nOrder+=nMargin;
	}
}

HSHBGROUP COXShortcutBar::FindGroup(const LPSHB_GROUPINFO pGroupInfo) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	// scan through all groups and check if pGroupInfo is subset of one of them
	HSHBGROUP hGroup;
	LPSHB_GROUPINFO pSavedGroupInfo;
	POSITION pos=m_mapHandleToInfo.GetStartPosition();
	while(pos!=NULL)
	{
		m_mapHandleToInfo.GetNextAssoc(pos,hGroup,pSavedGroupInfo);

		ASSERT(hGroup!=NULL);
		ASSERT(pSavedGroupInfo!=NULL);

		// check if pSavedGroupInfo contains pGroupInfo as subset
		if(CompareGroups(pGroupInfo,pSavedGroupInfo))
			return hGroup;
	}

	return NULL;
}

HSHBGROUP COXShortcutBar::FindGroupByOrder(int nOrder) const
{
	SHB_GROUPINFO shbGroup;
	shbGroup.nMask=SHBIF_ORDER;
	shbGroup.nOrder=nOrder;

	return FindGroup(&shbGroup);
}

HSHBGROUP COXShortcutBar::FindGroupByParam(LPARAM lParam) const
{
	SHB_GROUPINFO shbGroup;
	shbGroup.nMask=SHBIF_PARAM;
	shbGroup.lParam=lParam;

	return FindGroup(&shbGroup);
}

HSHBGROUP COXShortcutBar::FindGroupByTitle(LPCTSTR lpszTitle) const
{
	SHB_GROUPINFO shbGroup;
	shbGroup.pszText=(LPTSTR)lpszTitle;
	shbGroup.nTextMax=lstrlen(lpszTitle);
	shbGroup.nMask=SHBIF_TEXT;

	return FindGroup(&shbGroup);
}

// returns TRUE if all of elements from pGroup are the same in pGroupCompareTo
BOOL COXShortcutBar::CompareGroups(const LPSHB_GROUPINFO pGroup, 
								   const LPSHB_GROUPINFO pGroupCompareTo) const
{
	ASSERT(pGroup);
	ASSERT(pGroupCompareTo);

	// verify both groups
	if(!VerifyGroupInfo(pGroup) || !VerifyGroupInfo(pGroupCompareTo))
		return FALSE;

	// pGroup should be a subset of pGroupCompareTo
	if((pGroup->nMask&~pGroupCompareTo->nMask)!=0)
		return FALSE;

	// text
	if(pGroup->nMask&SHBIF_TEXT)
		if(_tcsncmp(pGroup->pszText,pGroupCompareTo->pszText,pGroup->nTextMax)!=0)
			return FALSE;

	// image
	if(pGroup->nMask&SHBIF_IMAGE)
		if(pGroup->nImage!=pGroupCompareTo->nImage)
			return FALSE;

	// image expanded
	if(pGroup->nMask&SHBIF_IMAGEEXPANDED)
		if(pGroup->nImageExpanded!=pGroupCompareTo->nImageExpanded)
			return FALSE;

	// lParam
	if(pGroup->nMask&SHBIF_PARAM)
		if(pGroup->lParam!=pGroupCompareTo->lParam)
			return FALSE;

	// order
	if(pGroup->nMask&SHBIF_ORDER)
		if(pGroup->nOrder!=pGroupCompareTo->nOrder)
			return FALSE;

	// view
	if(pGroup->nMask&SHBIF_VIEW)
		if(pGroup->nView!=pGroupCompareTo->nView)
			return FALSE;

	// descriptor - we check only pointers to SHB_DESCRIPTOR structures if they are the same
	if(pGroup->nMask&SHBIF_DESCRIPTOR)
		if(pGroup->pDescriptor!=pGroupCompareTo->pDescriptor)
			return FALSE;

	// child window
	if(pGroup->nMask&SHBIF_CHILDWND)
		if(pGroup->hwndChild!=pGroupCompareTo->hwndChild)
			return FALSE;

	return TRUE;
}

void COXShortcutBar::CalculateBoundRects()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	// clean up map of all bound rectangles
	m_rectChildWnd.SetRectEmpty();
	m_mapHandleToBoundRect.RemoveAll();

	CRect rect;
	GetClientRect(rect);

	int nBottom=rect.bottom;

	int nCount=PtrToInt(GetGroupCount());
	if(nCount==0)
		return;

	COXShortcutBar * me = const_cast<COXShortcutBar *>(this);
	COXShortcutBarSkin * skin = me->GetShortcutBarSkin();
	bool bExpandCells = (skin->AnimateSelection());

	// loop through all groups by their order
	CRect rectOld;
	HSHBGROUP hGroup;
	SHB_GROUPINFO shbGroup;
	for(int nIndex=0; nIndex<nCount; nIndex++)
	{
		hGroup=FindGroupByOrder(nIndex);
		VERIFY(hGroup!=NULL);

		// double check that everything alright with group info integrity
		ASSERT(GetGroupInfo(hGroup,&shbGroup,TRUE));
		ASSERT(VerifyGroupInfo(&shbGroup));
	
		// just add the height of the group
		rect.bottom=rect.top+GetGroupHeaderHeight(hGroup);

		m_mapHandleToBoundRect.SetAt(hGroup,rect);

		// don't forget about margin between groups
		rect.top=rect.bottom+m_nGroupMargin;
	}
	// if there is space left to display child window then try to define
	// the coordinates of its rectangle  
	if(rect.bottom<nBottom || bExpandCells)
	{
		// we display only child window of expanded group
		hGroup=GetExpandedGroup();
		if(hGroup!=NULL)
		{
			CRect rectExpanded;
			VERIFY(m_mapHandleToBoundRect.Lookup(hGroup,rectExpanded));
			m_rectChildWnd=rectExpanded;
			m_rectChildWnd.top= bExpandCells ? m_rectChildWnd.bottom : ( (skin->AnimateSelection()) ? 0 : DFLT_TOPHDRHEIGHT2003);
			m_rectChildWnd.bottom+=nBottom-rect.bottom - (bExpandCells ? ( (skin->AnimateSelection()) ? 0 : DFLT_TOPHDRHEIGHT2003 ) : m_rectChildWnd.bottom);

			// deflate by margins
			m_rectChildWnd.DeflateRect(m_rectChildWndMargins);
			
			if(m_rectChildWnd.top>=m_rectChildWnd.bottom || 
				m_rectChildWnd.left>=m_rectChildWnd.right)
				m_rectChildWnd.SetRectEmpty();
			// update coordinates of header rectangles of all groups that are placed
			// to the bottom of shortcut bar of expanded group

			UpdateBoundRects((bExpandCells ? rectExpanded.bottom+m_nGroupMargin : 0),nBottom-rect.bottom);
		}
	}
}

void COXShortcutBar::UpdateBoundRects(int nStartFrom, int nMargin)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	VERIFY(nMargin>=0);
	CPoint ptMargin(0,nMargin);

	// check shortcut bar integrity
	ASSERT(m_mapHandleToBoundRect.GetCount()==m_mapHandleToInfo.GetCount());

	// scan through all groups
	HSHBGROUP hGroup;
	CRect rect;
	POSITION pos=m_mapHandleToBoundRect.GetStartPosition();
	while(pos!=NULL)
	{
		m_mapHandleToBoundRect.GetNextAssoc(pos,hGroup,rect);

		ASSERT(hGroup!=NULL);

		// update those header rectangles that are placed down to the bottom of 
		// nStartFrom coordinate
		if(rect.top>=nStartFrom)
		{
			rect+=ptMargin;
			m_mapHandleToBoundRect.SetAt(hGroup,rect);
		}
	}
}

void COXShortcutBar::HideChildWnd(HSHBGROUP hGroup)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	if(hGroup==NULL)
		return;

	// check if the group is already deleted
	LPSHB_GROUPINFO lpshb;
	if(!m_mapHandleToInfo.Lookup(hGroup,lpshb))
		return;

	// get handle of any child window associated with any group (whether it is
	// COXSHBListCtrl window or application defined window)
	HWND hWnd=GetGroupChildWndHandle(hGroup);
	ASSERT(hWnd);

	// just hide it
	if(::IsWindow(hWnd))
		::ShowWindow(hWnd,SW_HIDE);
}

void COXShortcutBar::ShowChildWnd(HSHBGROUP hGroup, BOOL bRedraw/* = TRUE*/)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	if(hGroup==NULL)
		return;

	// get handle of any child window associated with any group (whether it is
	// COXSHBListCtrl window or application defined window)
	HWND hWnd=GetGroupChildWndHandle(hGroup);
	ASSERT(hWnd);
	// window has to exist at the moment
	ASSERT(::IsWindow(hWnd));

	// check if there is any space where we can display child window 
	// of expanded group
	if(!m_rectChildWnd.IsRectEmpty())
	{
		CRect rectWindow;
		::GetWindowRect(hWnd,rectWindow);
		// probably resize it
		::MoveWindow(hWnd,m_rectChildWnd.left,m_rectChildWnd.top,
			m_rectChildWnd.Width(),m_rectChildWnd.Height(),FALSE);
		// redraw if specified
		if(bRedraw)
			::RedrawWindow(hWnd,NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
		// and show it
		::ShowWindow(hWnd,SW_SHOW);
	}
	else
		// there is no space to display it
		::ShowWindow(hWnd,SW_HIDE);
}

void COXShortcutBar::FillBackground(CDC* pDC)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	// get background color of expanded group
	COLORREF clrBackground;
	HSHBGROUP hGroup=GetExpandedGroup();
	if(hGroup!=NULL)
	{
		clrBackground=GetGroupBkColor(hGroup);
		VERIFY(clrBackground!=ID_COLOR_NONE);
	}
	else
		clrBackground = GetShortcutBarSkin()->GetBackgroundColor(this);
	CBrush brush(clrBackground);

	// optimize filling of background by excluding from client rectangle
	// all group headers rect and child window rectangle of expanded group
	CRect rect;
	GetClientRect(rect);

	int iSavedDC = pDC->SaveDC();

	// exclude header's rectangles
	POSITION pos=m_mapHandleToBoundRect.GetStartPosition();
	while(pos!=NULL)
	{
		CRect rectExclude;
		m_mapHandleToBoundRect.GetNextAssoc(pos,hGroup,rectExclude);

		ASSERT(hGroup!=NULL);
		pDC->ExcludeClipRect(rectExclude);
	}

	// exclude child window's rectangle
	if(!m_rectChildWnd.IsRectEmpty())
		pDC->ExcludeClipRect(m_rectChildWnd);


	COXShortcutBar * me = const_cast<COXShortcutBar *>(this);
	COXShortcutBarSkin * skin = me->GetShortcutBarSkin();
	skin->DrawTopHeader(this, pDC);

	// fill the rest of background
	hGroup = GetExpandedGroup();
	if (hGroup != NULL)
	{
		if (HasGroupBkColor(hGroup))
			skin->FillBackground(pDC, rect, this, TRUE, clrBackground);
		else
			skin->FillBackground(pDC, rect, this);
	}
	else
		skin->FillBackground(pDC, rect, this);

	pDC->RestoreDC(iSavedDC);
}

LRESULT COXShortcutBar::SendSHBNotification(LPNMSHORTCUTBAR pNMSHB)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	// send standard shortcut bar notification to its parent using 
	// NMSHORTCUTBAR structure

	// notify parent
	CWnd* pParentWnd=GetOwner();
	if(pParentWnd)
	{
		// fill notification structure
		pNMSHB->hdr.hwndFrom=GetSafeHwnd();
		pNMSHB->hdr.idFrom=GetDlgCtrlID();

		return (pParentWnd->SendMessage(
			WM_NOTIFY,(WPARAM)GetDlgCtrlID(),(LPARAM)pNMSHB));
	}
	else
	{
		return (LRESULT)0;
	}
}

BOOL COXShortcutBar::GetGroupHeaderRect(HSHBGROUP hGroup, LPRECT lpRect) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	CRect rect;
	if(!m_mapHandleToBoundRect.Lookup(hGroup,rect))
	{
		TRACE(_T("COXShortcutBar::GetGroupHeaderRect: bound rect not found"));
		return FALSE;
	}

	lpRect->left=rect.left;
	lpRect->right=rect.right;
	lpRect->top=rect.top;
	lpRect->bottom=rect.bottom;

	return TRUE;
}

BOOL COXShortcutBar::UpdateHeader(HSHBGROUP hGroup)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	// redraw header of specified group
	//

	if(hGroup==NULL)
	{
		return FALSE;
	}

	CRect rectHeader;
	VERIFY(GetGroupHeaderRect(hGroup,rectHeader));

	RedrawWindow(rectHeader);

	return TRUE;
}

HWND COXShortcutBar::GetGroupChildWndHandle(HSHBGROUP hGroup) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	// Get window's handle of any child window associated with specified group.
	// COXSHBListCtrl or any application defined window have to be set to every group
	// in any shortcut bar control
	ASSERT(hGroup);

	// first check for application defined child window
	HWND hWnd=GetGroupChildWnd(hGroup);
	if(hWnd==NULL)
	{
		// after that look for COXSHBListCtrl
		COXSHBListCtrl* pListCtrl=GetGroupListCtrl(hGroup);
		if(pListCtrl!=NULL)
		{
			hWnd=pListCtrl->GetSafeHwnd();
		}
	}
	
	return (hWnd);
}

int COXShortcutBar::SetGroupMargin(int nGroupMargin) 
{
	int nOldGroupMargin=m_nGroupMargin;
	if(m_nGroupMargin!=nGroupMargin)
	{
		m_nGroupMargin=nGroupMargin;
		if(::IsWindow(GetSafeHwnd()))
		{
			// reposition groups
			CalculateBoundRects();
			// notify all childrens about changing in properties of their 
			// parent shortcut bar
			SendMessageToDescendants(
				SHBM_SHBINFOCHANGED,(WPARAM)0,(LPARAM)SHBIF_GROUPMARGIN);
		}
	}
	return nOldGroupMargin;
}

CRect COXShortcutBar::SetChildWndMargins(CRect rectChildWndMargins) 
{
	CRect rectOldChildWndMargins=m_rectChildWndMargins;
	if(m_rectChildWndMargins!=rectChildWndMargins)
	{
		m_rectChildWndMargins=rectChildWndMargins;
		if(::IsWindow(GetSafeHwnd()))
		{
			// reposition groups
			CalculateBoundRects();
			// notify all childrens about changing in properties of their 
			// parent shortcut bar
			SendMessageToDescendants(
				SHBM_SHBINFOCHANGED,(WPARAM)0,(LPARAM)SHBIF_CHILDMARGINS);
		}
	}
	return rectOldChildWndMargins;
}

CSize COXShortcutBar::SetBarMinSize(CSize sizeMin) 
{
	CSize sizeOldMin=m_sizeMin;
	m_sizeMin=sizeMin;
	return sizeOldMin;
}

CSize COXShortcutBar::SetScrollButtonSize(CSize sizeScrollButton) 
{
	CSize sizeOldScrollButton=m_sizeScrollButton;
	if(m_sizeScrollButton!=sizeScrollButton)
	{
		m_sizeScrollButton=sizeScrollButton;
		if(::IsWindow(GetSafeHwnd()))
		{
			// notify all childrens about changing in properties of their 
			// parent shortcut bar
			SendMessageToDescendants(
				SHBM_SHBINFOCHANGED,(WPARAM)0,(LPARAM)SHBIF_SCRLBTNSIZE);
		}
	}
	return sizeOldScrollButton;
}

UINT COXShortcutBar::SetScrollingDelay(UINT nScrollingDelay) 
{
	UINT nOldScrollingDelay=m_nScrollingDelay;
	if(m_nScrollingDelay!=nScrollingDelay)
	{
		m_nScrollingDelay=nScrollingDelay;
		if(::IsWindow(GetSafeHwnd()))
		{
			// notify all childrens about changing in properties of their 
			// parent shortcut bar
			SendMessageToDescendants(
				SHBM_SHBINFOCHANGED,(WPARAM)0,(LPARAM)SHBIF_SCRLDELAY);
		}
	}
	return nOldScrollingDelay;
}

UINT COXShortcutBar::SetAutoScrollingDelay(UINT nAutoScrollingDelay) 
{
	UINT nOldAutoScrollingDelay=m_nAutoScrollingDelay;
	if(m_nAutoScrollingDelay!=nAutoScrollingDelay)
	{
		m_nAutoScrollingDelay=nAutoScrollingDelay;
		// notify all childrens about changing in properties of their 
		// parent shortcut bar
		if(::IsWindow(GetSafeHwnd()))
		{
			SendMessageToDescendants(
				SHBM_SHBINFOCHANGED,(WPARAM)0,(LPARAM)SHBIF_AUTOSCRLDELAY);
		}
	}
	return nOldAutoScrollingDelay;
}

UINT COXShortcutBar::SetAutoExpandDelay(UINT nAutoExpandDelay) 
{
	UINT nOldAutoExpandDelay=m_nAutoExpandDelay;
	if(m_nAutoExpandDelay!=nAutoExpandDelay)
	{
		m_nAutoExpandDelay=nAutoExpandDelay;
		if(::IsWindow(GetSafeHwnd()))
		{
			// notify all childrens about change in properties of their 
			// parent shortcut bar
			SendMessageToDescendants(
				SHBM_SHBINFOCHANGED,(WPARAM)0,(LPARAM)SHBIF_AUTOEXPANDDELAY);
		}
	}
	return nOldAutoExpandDelay;
}

BOOL COXShortcutBar::InitShortcutBar()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	// create edit control used to edit header text
	if(!CreateEditControl())
	{
		TRACE(_T("COXShortcutBar::InitShortcutBar: failed to create edit control"));
		return FALSE;
	}

	// tooltips must be always enabled
	EnableToolTips(TRUE);

	// set timer for checking which group the mouse cursor is over
	m_nCheckMouseIsOverGroupID=SetTimer(
		ID_CHECKMOUSEISOVERGROUP,DFLT_CHECKMOUSEISOVERGROUP,NULL);
	ASSERT(m_nCheckMouseIsOverGroupID!=0);

	// register OLE Drag'n'Drop
	COleDropTarget* pOleDropTarget=GetDropTarget();
	ASSERT(pOleDropTarget!=NULL);
	if(!pOleDropTarget->Register(this))
	{
		TRACE(_T("COXShortcutBar::InitShortcutBar: failed to register the shortcut bar with COleDropTarget\n"));
		TRACE(_T("COXShortcutBar: you've probably forgot to initialize OLE libraries using AfxOleInit function\n"));
	}

	return TRUE;
}

COleDropTarget* COXShortcutBar::GetDropTarget() 
{ 
	// notify parent to give it a chance to provide application specific 
	// COleDropTarget
	NMSHORTCUTBAR nmshb;
	nmshb.hdr.code=SHBN_GETDROPTARGET;
	nmshb.lParam=(LPARAM)NULL;

	COleDropTarget* pOleDropTarget=NULL;
	// check if parent want to set its own drop target object
	if(SendSHBNotification(&nmshb) && nmshb.lParam!=NULL)
		pOleDropTarget=(COleDropTarget*)nmshb.lParam;
	else
		pOleDropTarget=(COleDropTarget*)&m_oleDropTarget;

	ASSERT(pOleDropTarget!=NULL);

	return pOleDropTarget; 
}

void COXShortcutBar::SetMouseIsOverGroup(HSHBGROUP hGroup)
{
	// reset handle to the group that is positioned under mouse cursor
	if(m_hLastMouseIsOverGroup!=hGroup)
	{
		BOOL bFlat=((GetBarStyle() & SHBS_FLATGROUPBUTTON)==SHBS_FLATGROUPBUTTON);

		HSHBGROUP hOldMouseIsOverGroup=m_hLastMouseIsOverGroup;
		m_hLastMouseIsOverGroup=hGroup;
		if(bFlat)
		{
			UpdateHeader(hOldMouseIsOverGroup);
			UpdateHeader(m_hLastMouseIsOverGroup);
		}
	}
}

COXShortcutBarSkin* COXShortcutBar::GetShortcutBarSkin()
{
	// Check if the app is derived from COXSkinnedApp
	COXSkinnedApp* pSkinnedApp = DYNAMIC_DOWNCAST(COXSkinnedApp, AfxGetApp());
	if (pSkinnedApp != NULL && pSkinnedApp->GetCurrentSkin() != NULL)
		return pSkinnedApp->GetCurrentSkin()->GetShortcutBarSkin();
	else
	{
		// Create a classic skin for this class if not created already
		if (m_pShortcutBarSkin == NULL)
			m_pShortcutBarSkin = new COXShortcutBarSkinClassic();

		return m_pShortcutBarSkin;
	}
}

int CALLBACK COXShortcutBar::CompareHeaders(LPARAM lParam1, LPARAM lParam2, 
										   LPARAM lParamSort)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	// lParamSort is pointer to shortcut bar control
	COXShortcutBar* pShortcutBar=(COXShortcutBar*)lParamSort;
	ASSERT(pShortcutBar);

	// while lParam1 and lParam2 are handles of compared groups
	HSHBGROUP hGroup1=(HSHBGROUP)lParam1;
	HSHBGROUP hGroup2=(HSHBGROUP)lParam2;
	ASSERT(hGroup1 && hGroup2);

	// get the text of groups
	CString sText1=pShortcutBar->GetGroupText(hGroup1);
	CString sText2=pShortcutBar->GetGroupText(hGroup2);

	// just use alphabetical, nocase compare function to compare group headers text
	return sText1.CompareNoCase(sText2);
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


COLORREF UpdateColor(COLORREF clr, int nOffset)
{
	ASSERT(abs(nOffset)<256);

	int rValue=GetRValue(clr)+nOffset;
	int gValue=GetGValue(clr)+nOffset;
	int bValue=GetRValue(clr)+nOffset;
	if(nOffset>0)
	{
		rValue=rValue>255 ? 255 : rValue;
		gValue=gValue>255 ? 255 : gValue;
		bValue=bValue>255 ? 255 : bValue;
	}
	else
	{
		rValue=rValue<0 ? 0 : rValue;
		gValue=gValue<0 ? 0 : gValue;
		bValue=bValue<0 ? 0 : bValue;
	}

	return RGB(rValue,gValue,bValue);
}

COLORREF InvertColor(COLORREF clr)
{
	return RGB(255-GetRValue(clr),255-GetGValue(clr),255-GetBValue(clr));
}

BOOL IsColorCloseTo(COLORREF clr,COLORREF clrCompareTo,UINT nMaxMargin)
{
	UINT nMargin=(abs(GetRValue(clr)-GetRValue(clrCompareTo))+
		abs(GetGValue(clr)-GetGValue(clrCompareTo))+
		abs(GetBValue(clr)-GetBValue(clrCompareTo)))/3;

	return (nMaxMargin>=nMargin);
}

void* CopyImageFromIL(CImageList* pil, int nImage, DWORD& nImageSize)
{
	ASSERT(pil!=NULL);

	ASSERT(nImage>=0 && nImage<pil->GetImageCount());

	IMAGEINFO imageInfo;
	VERIFY(pil->GetImageInfo(nImage,&imageInfo));
	CRect rect=imageInfo.rcImage;
	ASSERT(!rect.IsRectEmpty());

	CImageList il;
	VERIFY(il.Create(rect.Width(),rect.Height(),ILC_MASK,1,0));
				
	HICON hIcon=pil->ExtractIcon(nImage);
	ASSERT(hIcon!=NULL);
	VERIFY(il.Add(hIcon)!=-1);
	VERIFY(::DestroyIcon(hIcon));

	CMemFile memFile;
	CArchive ar(&memFile,CArchive::store);
	VERIFY(il.Write(&ar));
	ar.Close();

	nImageSize = (DWORD) memFile.GetLength();

	void* pBuffer=malloc(nImageSize);
	ASSERT(pBuffer!=NULL);

	BYTE* buf=memFile.Detach();
	memcpy(pBuffer,(void*)buf,nImageSize);
	ASSERT(pBuffer!=NULL);

	free(buf);

	return pBuffer;
}

int CopyImageToIL(CImageList* pil, void* pBuffer, DWORD nBufferLength)
{
///	ASSERT(pil!=NULL && pil->GetImageCount()>0);
	ASSERT(pil!=NULL);
	ASSERT(pBuffer!=NULL && nBufferLength>0);

	CMemFile memFile((BYTE*)pBuffer,(UINT)nBufferLength);
	CArchive ar(&memFile,CArchive::load);

	CImageList il;
	VERIFY(il.Read(&ar));
	ar.Close();

	ASSERT(il.GetImageCount()==1);
	HICON hIcon=il.ExtractIcon(0);
	ASSERT(hIcon!=NULL);

	if(pil->GetSafeHandle()==NULL)
	{
		IMAGEINFO imageInfo;
		VERIFY(il.GetImageInfo(0,&imageInfo));
		CRect rect=imageInfo.rcImage;
		ASSERT(!rect.IsRectEmpty());

		VERIFY(pil->Create(rect.Width(),rect.Height(),ILC_MASK,1,0));
	}

	int nImageIndex=pil->Add(hIcon);
	VERIFY(::DestroyIcon(hIcon));

	return nImageIndex;
}



