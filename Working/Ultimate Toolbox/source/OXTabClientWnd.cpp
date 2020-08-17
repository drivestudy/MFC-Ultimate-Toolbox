// ==========================================================================
//						Class Implementation : 
//		COXTabClientWnd & COXTabWorkspace & COXTabWorkspaceDropTarget
// ==========================================================================

// Source file : OXTabClientWnd.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXTabClientWnd.h"
#include "OXSkins.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Change tab on drag over handler
DROPEFFECT COXTabWorkspaceDropTarget::OnDragOver(CWnd* pWnd, 
												 COleDataObject* pDataObject,
												 DWORD dwKeyState, 
												 CPoint point)
{
	UNREFERENCED_PARAMETER(pDataObject);
	UNREFERENCED_PARAMETER(dwKeyState);

	// Get the pointer to tab control
    COXTabWorkspace* pTabWorkspace=(COXTabWorkspace*)pWnd;

    if (!pTabWorkspace || !pTabWorkspace->IsAcceptingDraggedObject())
        return DROPEFFECT_NONE;

    TC_HITTESTINFO hitTest;
    hitTest.pt=point;
    
	// Find the drop target item 
    int nItem=pTabWorkspace->HitTest(&hitTest);

	// If found ...
    if(nItem>=0 && m_nOldItem!=nItem)
    {
		// Get the pointer to the corresponding MDIChild 
		CWnd* pChildWnd=pTabWorkspace->m_arrTab[nItem].pWnd;

		if(::IsWindow(pChildWnd->GetSafeHwnd()))
		{
			// Activate it
			pTabWorkspace->GetParentFrame()->MDIActivate(pChildWnd);
		    m_nOldItem=pTabWorkspace->HitTest(&hitTest);
		}
    }

    return DROPEFFECT_NONE;
}


void COXTabWorkspaceDropTarget::OnDragLeave(CWnd* pWnd)
{
	m_nOldItem=-1;
	COleDropTarget::OnDragLeave(pWnd);
}


////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(COXTabWorkspace, COXSkinnedTabCtrl)

COXTabWorkspace::COXTabWorkspace()
{
	m_pTabClientWnd=NULL;
	m_dwOffset=ID_TABOFFSET;
	m_bAcceptDraggedObject=TRUE;
	m_pTabSkin = NULL;
}


COXTabWorkspace::~COXTabWorkspace()
{
}


BEGIN_MESSAGE_MAP(COXTabWorkspace, COXSkinnedTabCtrl)
	//{{AFX_MSG_MAP(COXTabWorkspace)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGE, OnSelchange)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


int COXTabWorkspace::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(CTabCtrl::OnCreate(lpCreateStruct)==-1)
		return -1;
	
    // Set status timer
    if(SetTimer(IDT_MDI_STATUS_TIMER,500,NULL)==0)
	{
		TRACE(_T("COXTabWorkspace::OnCreate: SetTimer() failed\n"));
		return -1;
	}
    
	// Create image list 
    if(!m_imageList.Create(GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),ILC_COLOR8|ILC_MASK,4,4))
	{
		TRACE(_T("COXTabWorkspace::OnCreate: Create() image list failed\n"));
		return -1;
	}
    
	// Set default font
	SendMessage(WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT));

	// Set image list so that it will be populated with MDIChild
	// window icons
    SetImageList(&m_imageList);

	// Register drop-target
	if(!m_dropTarget.Register(this))
	{
		TRACE(_T("COXTabWorkspace::OnCreate: failed to register the control with COleDropTarget\n"));
		TRACE(_T("you've probably forgot to initialize OLE libraries using AfxOleInit function\n"));
	}

	return 0;
}


void COXTabWorkspace::OnNcCalcSize(BOOL bCalcValidRects, 
								   NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	// TODO: Add your message handler code here and/or call default
	
	CTabCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);

	// Add an offset
	if(lpncsp->rgrc[0].bottom-lpncsp->rgrc[0].top>2*(int)m_dwOffset)
	{
		lpncsp->rgrc[0].top+=m_dwOffset;
		lpncsp->rgrc[0].bottom-=m_dwOffset;
	}
	else
	{
		lpncsp->rgrc[0].top=lpncsp->rgrc[0].bottom;
	}

	if(lpncsp->rgrc[0].right-lpncsp->rgrc[0].left>2*(int)m_dwOffset)
	{
		lpncsp->rgrc[0].left+=m_dwOffset;
		lpncsp->rgrc[0].right-=m_dwOffset;
	}
	else
	{
		lpncsp->rgrc[0].left=lpncsp->rgrc[0].right;
	}
}

void COXTabWorkspace::OnNcPaint() 
{
	// TODO: Add your message handler code here
	CWindowDC dc(this);
	CRect rectClient;
	GetClientRect(rectClient);
	CRect rectWindow;
	GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	rectClient.OffsetRect(-rectWindow.left, -rectWindow.top);
	if(rectClient.top<rectClient.bottom && rectClient.top<rectClient.bottom)
		dc.ExcludeClipRect(rectClient);

	// Erase parts not drawn
	SendMessage(WM_ERASEBKGND, (WPARAM)dc.m_hDC);

	// Draw borders in non-client area
	rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);
	GetTabSkin()->DrawMDITabBorder(&dc, rectWindow, this);

	// Do not call CTabCtrl::OnNcPaint() for painting messages
}

void COXTabWorkspace::OnDestroy() 
{
	CTabCtrl::OnDestroy();
	
	// TODO: Add your message handler code here

	// Delete image list
	VERIFY(m_imageList.DeleteImageList());
	// Kill update timer
	VERIFY(KillTimer(IDT_MDI_STATUS_TIMER));

	// Clean up internal arrays
	m_arrImage.RemoveAll();
	m_arrTab.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////////
// Description: Timer handler - periodically refreshes tabs, determines
//				active MDIChild and updates window names
//////////////////////////////////////////////////////////////////////////////
void COXTabWorkspace::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent!=IDT_MDI_STATUS_TIMER)
	{
		CTabCtrl::OnTimer(nIDEvent);
		return;
	}

	// Update active MDIChild and window text
	UpdateContents(TRUE);
}


//////////////////////////////////////////////////////////////////////////////
// Description: Selection change handler
//////////////////////////////////////////////////////////////////////////////
BOOL COXTabWorkspace::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);

    if(GetCurSel()>=0)
    {
		// Get the pointer to the MDIChild that will be activated
		CWnd* pChildWnd=m_arrTab[GetCurSel()].pWnd;

		if(::IsWindow(pChildWnd->GetSafeHwnd()))
		{
			GetParentFrame()->MDIActivate(pChildWnd);
		}
    }

	// Update the size and position of the tab control and MDIClient window
	ASSERT(::IsWindow(GetParentFrame()->GetSafeHwnd()));
	GetParentFrame()->RecalcLayout();

	*pResult = 0;

	return FALSE;
}


//////////////////////////////////////////////////////////////////////////////
// Description: Double click handler - maximizes / restores.
//////////////////////////////////////////////////////////////////////////////
void COXTabWorkspace::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(point);

    if(GetCurSel()>=0)
    {
		// Get the pointer to the MDIChild that will be maximized/restored
		CWnd* pChildWnd=m_arrTab[GetCurSel()].pWnd;

		if(::IsWindow(pChildWnd->GetSafeHwnd()))
		{
		    BOOL bMaximize=FALSE;
			CWnd* pActiveWnd=GetParentFrame()->MDIGetActive(&bMaximize);

			// Maximize or restore MDIChild window based on its current state
			if(pActiveWnd==pChildWnd && bMaximize)
				GetParentFrame()->MDIRestore(pChildWnd);
			else
				GetParentFrame()->MDIMaximize(pChildWnd);
		}
    }
}

// Scans through all MDIChild windows and refreshes window names and 
// current active window. If bAddNewWindows is set to TRUE then if any
// new MDIChild is found it will be added to the tab control
void COXTabWorkspace::UpdateContents(BOOL bAddNewWindows/*=FALSE*/, 
									 BOOL bUpdateWindowsInfo/*=FALSE*/)
{
	ASSERT(m_pTabClientWnd!=NULL);

    // Check MDI windows
    CMDIFrameWnd* pFrameWnd=GetParentFrame();
    if(pFrameWnd==NULL)
	{
		return;
	}

	BOOL bRecalc=m_pTabClientWnd->m_bForceToRecalc;

	// Get pointer to currently active MDIChild
    CWnd* pActiveChildWnd=pFrameWnd->MDIGetActive(NULL);

    CMDIChildWnd* pChildWnd=NULL;
    int nActiveIndex=-1;

	// Start enumerating from currently active MDIChild
    if(pActiveChildWnd!=NULL)
	{
		pChildWnd=(CMDIChildWnd*)pActiveChildWnd->GetWindow(GW_HWNDFIRST);
	}

    // Flag all current tabs as unfound (for debug purposes in order to check
	// the integrity of the framework)
#ifdef _DEBUG
	int nIndex=0;
    for(nIndex=0; nIndex<m_arrTab.GetSize(); nIndex++)
	{
		m_arrTab[nIndex].bFound=FALSE;
	}
#endif

	int nInsertIndex= PtrToInt(m_arrTab.GetSize());

	// Enumerate all child windows
    while(pChildWnd!=NULL)
    {
		// Window text
		CString sWindowText=GetTextForTabItem(pChildWnd);

		// see if can find it
		int nFoundItem=FindTabItem(pChildWnd->GetSafeHwnd());

		if(nFoundItem!=-1)
		{
			if((pChildWnd->GetStyle()&WS_VISIBLE)==WS_VISIBLE)
			{
				if(pChildWnd==pActiveChildWnd)
				{
					// Found currently active MDIChild
					nActiveIndex=nFoundItem;
				}

#ifdef _DEBUG
				m_arrTab[nFoundItem].bFound=TRUE;
#endif

				// Update text if necessary
				if(m_arrTab[nFoundItem].sText!=sWindowText)
				{
					m_arrTab[nFoundItem].sText=sWindowText;

					//replace "&" characters for doubles "&&"
					int nFind=sWindowText.Find(TEXT('&'));
					while (nFind != -1)
					{
						if (nFind<sWindowText.GetLength())
						{
//							if (sWindowText.GetAt(nFind+1)!=TEXT('&'))
							sWindowText.Insert(nFind+1,TEXT('&'));
							nFind=sWindowText.Find(TEXT('&'),nFind+2);
						}
						else
							nFind=-1;
					}


					TC_ITEM tci;
					tci.mask=TCIF_TEXT;
					tci.pszText=(LPTSTR)(LPCTSTR)sWindowText;
					SetItem(nFoundItem,&tci);

					bRecalc=TRUE;
				}

				if(bUpdateWindowsInfo)
				{
					TC_ITEM tci;
					tci.mask=TCIF_IMAGE;
					GetItem(nFoundItem,&tci);
					// Update icon if necessary
					HICON hIcon=GetWindowIcon(pChildWnd->GetSafeHwnd());
					int nImageIndex=(hIcon==NULL ? -1 : AddTabItemIcon(hIcon));
					if(nImageIndex!=tci.iImage)
					{
						tci.iImage=nImageIndex;
						SetItem(nFoundItem,&tci);
					}
				}
			}
			else
			{
				nInsertIndex--;
				if(nActiveIndex!=-1 && nActiveIndex>nFoundItem)
				{
					nActiveIndex--;
				}
				RemoveTabItem(pChildWnd,FALSE);
				bRecalc=TRUE;
			}
		}
		else if(bAddNewWindows)
		{
			if(nActiveIndex!=-1 && nActiveIndex>=nInsertIndex)
			{
				nActiveIndex++;
			}
			// add item
			InsertTabItem(nInsertIndex,pChildWnd,FALSE);
			bRecalc=TRUE;
		}

		// Get next MDIChild
		pChildWnd=(CMDIChildWnd*)pChildWnd->GetWindow(GW_HWNDNEXT);
    }

#ifdef _DEBUG
    for(nIndex=0; nIndex<m_arrTab.GetSize(); nIndex++)
	{
		ASSERT(m_arrTab[nIndex].bFound);
	}
#endif

	// Set the active item
    if(nActiveIndex>=0 && GetCurSel()!=nActiveIndex)
	{
		SetCurSel(nActiveIndex);
		bRecalc=TRUE;
	}

	if(bRecalc)
	{
		// Update the size and position of the tab control and MDIClient window
		if(::IsWindow(GetParentFrame()->GetSafeHwnd()))
		{
			GetParentFrame()->RecalcLayout();
		}
		m_pTabClientWnd->m_bForceToRecalc=FALSE;
	}
}

// Retrieves pointer to the MDIFrame window
CMDIFrameWnd* COXTabWorkspace::GetParentFrame() const 
{ 
	ASSERT(m_pTabClientWnd!=NULL);
	ASSERT(m_pTabClientWnd->IsAttached());

	// Request MDIClient for the parent frame
	CMDIFrameWnd* pParentFrame=m_pTabClientWnd->GetParentFrame();
	ASSERT(pParentFrame!=NULL);

	return pParentFrame; 
}

// Finds the tab item for specified window. Returns -1 if not found
int COXTabWorkspace::FindTabItem(const HWND hWnd) const
{
	int nFoundItem=-1;
	// Loop through all tab items
	for(int nIndex=0; nIndex<m_arrTab.GetSize(); nIndex++)
	{
		// Check for window handle
		if(m_arrTab[nIndex].pWnd->GetSafeHwnd()==hWnd)
		{
			// Double check for window class name
			TCHAR sWndClass[512];
			GetClassName(hWnd,sWndClass,sizeof(sWndClass)/sizeof(TCHAR));
			if(m_arrTab[nIndex].sWndClass==sWndClass)
			{
				nFoundItem=nIndex;
				break;
			}
		}
	}

	return nFoundItem;
}

// Inserts new item into the tab control for specified MDIChild. If bRedraw is
// set to TRUE then framework will be redrawn in order to show new item.
BOOL COXTabWorkspace::InsertTabItem(int nIndex, const CWnd* pChildWnd, 
									BOOL bRedraw/*=TRUE*/, BOOL bOnlyVisible/*=TRUE*/)
{
	ASSERT(pChildWnd!=NULL);
	ASSERT(::IsWindow(pChildWnd->GetSafeHwnd()));
	ASSERT(nIndex>=0 && nIndex<=m_arrTab.GetSize());

	// Make sure we add MDIChild window
	if((pChildWnd->GetExStyle()&WS_EX_MDICHILD)==0)
		return FALSE;

	// Make sure it is visible at the moment
	if(bOnlyVisible && (pChildWnd->GetStyle()&WS_VISIBLE)==0)
		return FALSE;

	// Work out window class
	TCHAR sWndClass[512];
	::GetClassName(pChildWnd->GetSafeHwnd(),sWndClass,
		sizeof(sWndClass)/sizeof(TCHAR));
//	WNDCLASSEX classInfo={ sizeof(WNDCLASSEX) };
//	::GetClassInfoEx(AfxGetInstanceHandle(),sWndClass,&classInfo);
    
	// Get icon
	HICON hIcon=GetWindowIcon(pChildWnd->GetSafeHwnd());
	int nImageIndex=(hIcon==NULL ? -1 : AddTabItemIcon(hIcon));

	// Set item text to window text
	CString sWindowText=GetTextForTabItem(pChildWnd);

	TC_ITEM tci;
	tci.mask=TCIF_TEXT|TCIF_IMAGE;
	tci.pszText=(TCHAR*)(void*)(const TCHAR*)sWindowText;
	tci.iImage=nImageIndex;

	// Insert new tab control item
	VERIFY(InsertItem(nIndex,&tci)!=-1);

	// Redraw the tab control
	if(!m_arrTab.GetSize() && bRedraw)
		InvalidateRect(NULL);

	// Save information about new entry
	TAB_ITEM_ENTRY newTabItemEntry;
	m_arrTab.Add(newTabItemEntry);
	for(int nTabItemIndex=PtrToInt(m_arrTab.GetSize())-1; nTabItemIndex>nIndex; nTabItemIndex--)
	{
		m_arrTab[nTabItemIndex]=m_arrTab[nTabItemIndex-1];
	}
	newTabItemEntry.sText=sWindowText;
	newTabItemEntry.pWnd=(CWnd*)pChildWnd;
	newTabItemEntry.bFound=TRUE;
	newTabItemEntry.sWndClass=sWndClass;
	m_arrTab.SetAt(nIndex,newTabItemEntry);

	// Update the size and position of the tab control and MDIClient window
	if(bRedraw)
	{
		ASSERT(::IsWindow(GetParentFrame()->GetSafeHwnd()));
		GetParentFrame()->RecalcLayout();
	}

	return TRUE;
}

// Remove item from the tab control that corresponds to specified MDIChild
BOOL COXTabWorkspace::RemoveTabItem(const CWnd* pChildWnd, BOOL bRedraw/*=TRUE*/)
{
	ASSERT(pChildWnd!=NULL);
	ASSERT(::IsWindow(pChildWnd->GetSafeHwnd()));
	ASSERT((pChildWnd->GetExStyle()&WS_EX_MDICHILD)!=0);

	// Find the item
	int nTabItem=FindTabItem(pChildWnd);

	if(nTabItem==-1)
		return FALSE;

	// Delete the item
	DeleteItem(nTabItem);
	// Remove entry from the internal array of created items
	m_arrTab.RemoveAt(nTabItem);

	// Update the size and position of the tab control and MDIClient window
	if(bRedraw)
	{
		if(::IsWindow(GetParentFrame()->GetSafeHwnd()))
			GetParentFrame()->RecalcLayout();
	}

	return TRUE;
}


CString COXTabWorkspace::GetTextForTabItem(const CWnd* pChildWnd) const
{
	ASSERT(pChildWnd!=NULL);
	ASSERT(pChildWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)));

	CString sWindowText=(LPCTSTR)((CWnd*)pChildWnd)->
		SendMessage(OXWM_MTI_GETWINDOWTEXT);

	if(sWindowText.IsEmpty())
	{
		if(sWindowText.IsEmpty())
			pChildWnd->GetWindowText(sWindowText);
	}

	return sWindowText;
}


HICON COXTabWorkspace::GetWindowIcon(HWND hWnd)
{
	ASSERT(::IsWindow(hWnd));

	// Work out icon...
	CWnd* pWnd=CWnd::FromHandle(hWnd);
	// Work out icon...
	HICON hIcon=(HICON)pWnd->SendMessage(WM_GETICON,ICON_SMALL);
	if(hIcon==NULL)
	{
		hIcon=(HICON)(INT_PTR)::GetClassLongPtr(hWnd,GCL_HICONSM);
		if(hIcon==NULL)
		{
			hIcon=(HICON)pWnd->SendMessage(WM_GETICON,ICON_BIG);
			if(hIcon==NULL)
			{
				hIcon=(HICON)(INT_PTR)::GetClassLongPtr(hWnd,GCL_HICON);
			}
		}
	}

	return hIcon;
}


int COXTabWorkspace::AddTabItemIcon(HICON hIcon)
{
	ASSERT(hIcon!=NULL);

	// Check if we already included the specified item into the tab control
	// image list
	int nImageIndex=0;
	for(nImageIndex=0; nImageIndex<m_arrImage.GetSize(); nImageIndex++)
	{
		if(m_arrImage[nImageIndex]==hIcon)
		{
			// Found
			break;
		}
	}

	if(nImageIndex==m_arrImage.GetSize())
	{
		// Add if not found
		m_imageList.Add(hIcon);
		m_arrImage.Add(hIcon);
	}

	return nImageIndex;
}

/////////////////////////////////////////////////////////////////////////////
// COXTabClientWnd

COXTabClientWnd::COXTabClientWnd()
{
	m_pParentFrame=NULL;
	m_bForceToRecalc=FALSE;
	m_bOneTabMode=FALSE;
	m_pTabSkin = NULL;

}

COXTabClientWnd::~COXTabClientWnd()
{
	if (m_pTabSkin != NULL)
		delete m_pTabSkin;
}


BEGIN_MESSAGE_MAP(COXTabClientWnd, CWnd)
	//{{AFX_MSG_MAP(COXTabClientWnd)
	ON_MESSAGE(WM_MDIACTIVATE,OnMDIActivate)
	ON_MESSAGE(WM_MDICREATE,OnMDICreate)
	ON_MESSAGE(WM_MDIDESTROY,OnMDIDestroy)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// Subclasses MDIClient window of the specified MDIFrame window and 
// create a COXTabWorkspace control to manage MDIChild window
BOOL COXTabClientWnd::Attach(const CMDIFrameWnd* pParentFrame, 
							 DWORD dwTabCtrlStyle/*=DEFAULT_TABCTRLSTYLE*/)
{
	ASSERT(pParentFrame!=NULL);
	ASSERT(::IsWindow(pParentFrame->GetSafeHwnd()));

	// Check if already attached
	if(IsAttached())
	{
		TRACE(_T("COXTabClientWnd::Attach: window has already been attached. Call Detach() function before!\n"));
		return FALSE;
	}

	// Make sure the specified window is/derived from CMDIFrameWnd class
	if(!pParentFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
	{
		TRACE(_T("COXTabClientWnd::Attach: specified frame window is not of CMDIFrameWnd class (or derived)!\n"));
		return FALSE;
	}

	// Check if there is CWnd object attached to MDIClient
	CWnd* pMDIClientWnd=CWnd::FromHandlePermanent(pParentFrame->m_hWndMDIClient);
	if(pMDIClientWnd!=NULL)
	{
		TRACE(_T("COXTabClientWnd::Attach: MDIClient window has already been subclassed\n"));
		return FALSE;
	}


	// Try to sublass MDIClient window
	if(!SubclassWindow(pParentFrame->m_hWndMDIClient))
	{
		TRACE(_T("COXTabClientWnd::Attach: failed to subclass MDI Client window\n"));
		return FALSE;
	}

	// Save the pointer to parent MDIFrame
	m_pParentFrame=(CMDIFrameWnd*)pParentFrame;


	// Create tab control
	//
	
	ASSERT(!::IsWindow(m_tab.GetSafeHwnd()));
	CRect rect(0,0,0,0);

	// Make sure WS_POPUP style is not used
	ASSERT((dwTabCtrlStyle&WS_POPUP)==0);
	// Make sure the following styles are used 
	dwTabCtrlStyle|=(TCS_FOCUSNEVER|WS_VISIBLE|WS_CHILD);
	m_tab.m_pTabClientWnd=this;
	// Try to create the tab control
    if(!m_tab.Create(dwTabCtrlStyle,rect,m_pParentFrame,IDC_TABWORKSPACE))
	{
		TRACE(_T("COXTabClientWnd::Attach: failed to create tab control\n"));
		// If failed we detach the parent frame
		Detach();
		return FALSE;
	}

	// Update the size and position of the tab control and MDIClient window
	m_pParentFrame->RecalcLayout();

	// Populate tab control with MDIChild windows if any exist at the moment
	m_tab.UpdateContents(TRUE);

	return TRUE;
}


// Unsubclasses MDIClient window and destroys the COXTabWorkspace control 
// that was used to manage MDIChild window
BOOL COXTabClientWnd::Detach()
{
	// Check if any attached
	if(!IsAttached())
	{
		TRACE(_T("COXTabClientWnd::Attach: there is nothing to detach! Window hasn't been attached!\n"));
		return FALSE;
	}

	// Destroy tab control
	if(::IsWindow(m_tab.GetSafeHwnd()))
		m_tab.DestroyWindow();

	m_tab.m_pTabClientWnd=NULL;

	// Unsubclass MDIClient window
	UnsubclassWindow();

	// Update the size and position of the MDIClient window
	if(::IsWindow(m_pParentFrame->GetSafeHwnd()))
		m_pParentFrame->RecalcLayout();

	m_pParentFrame=NULL;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// COXTabClientWnd message handlers


// Crucial function that calculates the size of MDIClient window. Called
// by parent MDIFrame window
void COXTabClientWnd::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class

    // Now do the laying out
    HDWP dwh=BeginDeferWindowPos(2);

    // Move tab window
    if(::IsWindow(m_tab.m_hWnd) && (m_tab.GetStyle()&WS_VISIBLE)==WS_VISIBLE)
    {
		// Get the size of the MDIClient the way it fits into the client area of
		// the tab control
		DWORD dwTabStyle=m_tab.GetStyle();
		if((dwTabStyle&TCS_BUTTONS)==TCS_BUTTONS && 
			(dwTabStyle&TCS_VERTICAL)==TCS_VERTICAL)
		{
			CRect rectTab=lpClientRect;

			CRect rect(lpClientRect->left,lpClientRect->top,
				lpClientRect->left+lpClientRect->bottom-lpClientRect->top,
				lpClientRect->top+lpClientRect->right-lpClientRect->left);

			// Move tab control
			::SetWindowPos(m_tab.m_hWnd,NULL,
				rect.left,rect.top,rect.Width(),
				rect.Height(),SWP_NOZORDER|SWP_NOREDRAW);

			rect.DeflateRect(m_tab.GetOffset(),m_tab.GetOffset());
			CRect rectCopy=rect;

			// Adjust the size of tab control
			lpClientRect->left+=m_tab.GetOffset();       
			lpClientRect->top+=m_tab.GetOffset();       
			lpClientRect->right-=m_tab.GetOffset();       
			lpClientRect->bottom-=m_tab.GetOffset();       

			if((dwTabStyle&TCS_RIGHT)==TCS_RIGHT)
			{
				m_tab.ModifyStyle(TCS_RIGHT|TCS_VERTICAL,0,SWP_NOREDRAW);
				m_tab.AdjustRect(FALSE,rect);
				m_tab.ModifyStyle(0,TCS_RIGHT|TCS_VERTICAL,SWP_NOREDRAW);
			}
			else
			{
				m_tab.ModifyStyle(TCS_VERTICAL,0,SWP_NOREDRAW);
				m_tab.AdjustRect(FALSE,rect);
				m_tab.ModifyStyle(0,TCS_VERTICAL,SWP_NOREDRAW);
				lpClientRect->left+=rect.top-rectCopy.top;
			}

			lpClientRect->top+=rect.left-rectCopy.left;
			lpClientRect->bottom=lpClientRect->top+rect.Width();
			lpClientRect->right=lpClientRect->left+rect.Height();

			// Move tab control
			dwh=::DeferWindowPos(dwh,m_tab.m_hWnd,NULL,rectTab.left,
				rectTab.top,rectTab.Width(),
				rectTab.Height(),SWP_NOZORDER);
		}
		else
		{
			CRect rectTab=lpClientRect;

			// Move tab control
			dwh=::DeferWindowPos(dwh,m_tab.m_hWnd,NULL,lpClientRect->left,
				lpClientRect->top,
				lpClientRect->right-lpClientRect->left,
				lpClientRect->bottom-lpClientRect->top, SWP_NOZORDER);

			// Adjust the size of tab control
			if(lpClientRect->bottom-lpClientRect->top>2*(int)m_tab.GetOffset())
			{
				lpClientRect->top+=m_tab.GetOffset();
				lpClientRect->bottom-=m_tab.GetOffset();
			}
			else
			{
				lpClientRect->top=lpClientRect->bottom;
			}

			if(lpClientRect->right-lpClientRect->left>2*(int)m_tab.GetOffset())
			{
				lpClientRect->left+=m_tab.GetOffset();
				lpClientRect->right-=m_tab.GetOffset();
			}
			else
			{
				lpClientRect->left=lpClientRect->right;
			}

			if((dwTabStyle&TCS_BUTTONS)==TCS_BUTTONS && 
				(dwTabStyle&TCS_VERTICAL)!=TCS_VERTICAL && 
					(dwTabStyle&TCS_BOTTOM)==TCS_BOTTOM)
			{
				int nTop=lpClientRect->top;

				m_tab.ModifyStyle(TCS_BOTTOM,0,SWP_NOREDRAW);
				m_tab.AdjustRect(FALSE,lpClientRect);
				m_tab.ModifyStyle(0,TCS_BOTTOM,SWP_NOREDRAW);

				lpClientRect->bottom-=lpClientRect->top-nTop;
				lpClientRect->top-=lpClientRect->top-nTop;

			}
			else
			{
				// This code is introduced because of severe bug in CTabCtrl MFC 
				// class, which caused a crash. Crash will appear if only one tab 
				// is present. This code will add temporarily one more tab and 
				// hide tabs.


				// The number of tabs
				int nNumOfItems = m_tab.GetItemCount();

				// Find the size of Tabs
				RECT itemRect;
				m_tab.GetItemRect(nNumOfItems-1, &itemRect);
				if (nNumOfItems == 1 && !m_bOneTabMode || nNumOfItems == 2 && m_bOneTabMode)
				{
					if( itemRect.right - itemRect.left + 50 < lpClientRect->right - lpClientRect->left )
					{
			
						if( m_bOneTabMode == TRUE )
						{
							m_tab.DeleteItem(nNumOfItems-1);
							m_bOneTabMode = FALSE;
						}
				
						m_tab.AdjustRect(FALSE,lpClientRect);
			
					}
					else
					{
						if( nNumOfItems == 1 )
						{
							m_tab.InsertItem(nNumOfItems,_T(""));
							m_bOneTabMode = TRUE;
						}
					}
				}
				else
					m_tab.AdjustRect(FALSE,lpClientRect);
			}
		}

		if(lpClientRect->bottom<lpClientRect->top || 
			lpClientRect->right<lpClientRect->left)
			::memset(lpClientRect,0,sizeof(RECT));

		::ShowWindow(m_tab.m_hWnd,SW_SHOWNA);
    }

//lpClientRect->left -= 5;
//lpClientRect->right += 5;
//lpClientRect->bottom += 5;

	GetTabSkin()->AdjustChildItemRect(lpClientRect, 5, &m_tab);

	// Move MDIClient window
    dwh=::DeferWindowPos(dwh,m_hWnd,NULL,lpClientRect->left,lpClientRect->top,
		lpClientRect->right-lpClientRect->left,
		lpClientRect->bottom-lpClientRect->top,
		SWP_NOZORDER);

    EndDeferWindowPos(dwh);

	CWnd::CalcWindowRect(lpClientRect, nAdjustType);
}

// Handler for a WM_MDIACTIVATE message. Will select corresponding 
// tab control item
LRESULT COXTabClientWnd::OnMDIActivate(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	LRESULT lResult=Default();
	
	HWND hActiveWnd=(HWND)(INT_PTR)wParam;
	// Find corresponding tab control item 
	int nTabItem=m_tab.FindTabItem(hActiveWnd);
	if(nTabItem!=-1)
	{
		ASSERT(nTabItem>=0 && nTabItem<m_tab.GetItemCount());
		m_tab.SetCurSel(nTabItem);
		if((m_tab.GetStyle() & TCS_SCROLLOPPOSITE)!=0)
		{
			m_bForceToRecalc=TRUE;
		}
	}

	// update the contents of the tab control afterwards
	m_tab.PostMessage(WM_TIMER,IDT_MDI_STATUS_TIMER);

	return lResult;
}


// Handler for WM_MDICREATE message. Will add new item to the 
// tab control
LRESULT COXTabClientWnd::OnMDICreate(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	LRESULT lResult=Default();

	// lResult is a handle to the new MDIChild window if it was
	// successfully created
	if(lResult!=NULL)
	{
		// There shouldn't be such an item in the tab control
		ASSERT(m_tab.FindTabItem((HWND)lResult)==-1);

		// Update the tab control contents
		m_tab.SetCurSel(-1);
	    m_tab.PostMessage(WM_TIMER,IDT_MDI_STATUS_TIMER);
	}

	return lResult;
}


// Handler for WM_MDIDESTROY message. Will remove the correspondent item 
// from the tab control
LRESULT COXTabClientWnd::OnMDIDestroy(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	// Remove the item from the tab control
	CWnd* pChildWnd=CWnd::FromHandle((HWND)(INT_PTR)wParam);
	m_tab.RemoveTabItem(pChildWnd);

	LRESULT lResult=Default();

	// Update the contents of the tab control afterwards
	m_tab.PostMessage(WM_TIMER,IDT_MDI_STATUS_TIMER);

	return lResult;
}

// Update the size of the tab control and the MDIClient window
void COXTabClientWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	// Update the contents of the tab control afterwards
	m_bForceToRecalc=TRUE;
	m_tab.PostMessage(WM_TIMER,IDT_MDI_STATUS_TIMER);
}


// Show/hide the tab control if MDIClient is shown/hidden
void COXTabClientWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow,nStatus);
	
	// TODO: Add your message handler code here

	if(nStatus==0 && ::IsWindow(m_tab.GetSafeHwnd()))
	{
		if(bShow)
			m_tab.ModifyStyle(NULL,WS_VISIBLE);
		else
			m_tab.ModifyStyle(WS_VISIBLE,NULL);
	}
}


BOOL COXTabClientWnd::SaveState(LPCTSTR lpszProfileName)
{
#ifndef _MAC
	CWinApp* pApp=AfxGetApp();
	ASSERT(pApp!=NULL);

	ASSERT(!IsAttached() || GetTabCtrl()!=NULL);

	pApp->WriteProfileInt(lpszProfileName,_T("IsAttached"),IsAttached());
	pApp->WriteProfileInt(lpszProfileName,_T("Offset"),
		(IsAttached() ? GetTabCtrl()->GetOffset() : ID_TABOFFSET));
	pApp->WriteProfileInt(lpszProfileName,_T("Style"),
		(IsAttached() ? GetTabCtrl()->GetStyle() : DEFAULT_TABCTRLSTYLE));

	return TRUE;
#else
	return FALSE;
#endif
}


BOOL COXTabClientWnd::LoadState(LPCTSTR lpszProfileName)
{
#ifndef _MAC
	CWinApp* pApp=AfxGetApp();
	ASSERT(pApp!=NULL);

	ASSERT(!IsAttached() || GetTabCtrl()!=NULL);

	BOOL bIsAttached=(BOOL)pApp->GetProfileInt(lpszProfileName,_T("IsAttached"),TRUE);
	if(bIsAttached && !IsAttached())
	{
		TRACE(_T("COXTabClientWnd::LoadState: fail to load state\n"));
		return FALSE;
	}

	if(!bIsAttached && !IsAttached())
		return TRUE;

	DWORD dwOffset=(DWORD)pApp->GetProfileInt(lpszProfileName,
		_T("Offset"),ID_TABOFFSET);
	DWORD dwStyle=(DWORD)pApp->GetProfileInt(lpszProfileName,
		_T("Style"),DEFAULT_TABCTRLSTYLE);

	if(GetTabCtrl()->GetOffset()==dwOffset && 
		GetTabCtrl()->GetStyle()==dwStyle && bIsAttached==IsAttached())
		return TRUE;

	CMDIFrameWnd* pParentFrame=GetParentFrame();
	ASSERT(pParentFrame!=NULL);

	VERIFY(Detach());

	if(!bIsAttached)
		return TRUE;

	VERIFY(Attach(pParentFrame,dwStyle));
	ASSERT(IsAttached() && GetTabCtrl()!=NULL);
	GetTabCtrl()->SetOffset(dwOffset);

	return TRUE;
#else
	return FALSE;
#endif
}

BOOL COXTabWorkspace::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);
	rect.right += m_dwOffset;
	rect.bottom += m_dwOffset;

	CBrush brush;
	brush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	brush.UnrealizeObject();

	pDC->FillRect(rect, &brush);
	return TRUE;
}

void COXTabClientWnd::OnNcPaint() 
{
	GetTabSkin()->OnNcPaintTabClient(this);
}

COXTabSkin* COXTabClientWnd::GetTabSkin()
{
	// Check if the app is derived from COXSkinnedApp
	COXSkinnedApp* pSkinnedApp = DYNAMIC_DOWNCAST(COXSkinnedApp, AfxGetApp());
	if (pSkinnedApp != NULL && pSkinnedApp->GetCurrentSkin() != NULL)
		return pSkinnedApp->GetCurrentSkin()->GetTabSkin();
	else
	{
		// Create a classic skin for this class if not created already
		if (m_pTabSkin == NULL)
			m_pTabSkin = new COXTabSkinClassic();

		return m_pTabSkin;
	}
}