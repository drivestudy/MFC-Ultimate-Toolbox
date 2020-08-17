// ==========================================================================
// 							   Class Implementation : 
//									COXMenuBar 
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

#include "OXMenuBar.h"
#include "OXSkins.h"
#include "OXBitmapMenuOrganizer.h"

#ifdef OX_CUSTOMIZE_COMMANDS
#include "OXDragDropCommands.h"
#include "..\include\oxmenubar.h"
#endif	//	OX_CUSTOMIZE_COMMANDS

#include "UTB64Bit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifndef IDT_OXCHECKFORDRAGDROPEVENT
#define IDT_OXCHECKFORDRAGDROPEVENT				412
#endif	//	IDT_OXCHECKFORDRAGDROPEVENT
#ifndef ID_OXCHECKFORDRAGDROPEVENT_DELAY
#define ID_OXCHECKFORDRAGDROPEVENT_DELAY		500
#endif	//	ID_OXCHECKFORDRAGDROPEVENT_DELAY


#if _MFC_VER>=0x0420


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// COXMenuBar
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(COXMenuBar,COXCoolToolBar)

HHOOK COXMenuBar::m_hMouseHook = NULL;

HWND COXMenuBar::m_hwndPrevMouseMoveWnd = NULL;

COXMenuBar::COXMenuBar()
{
	// only works when comctl32.dll v4.71>= is installed
	if(m_dwComCtlVersion<_IE40_COMCTL_VERSION)
	{
		TRACE(_T("In order to use Menu Bar in your application you have to have comctl32.dll v 4.71 or higher installed\n"));
		AfxThrowNotSupportedException();
	}

	// use entire row/column
	m_bTakeEntireRow=TRUE;

	m_hMenu=NULL;
	m_hMDIWindowMenu=NULL;

	m_nActiveMenuItem=-1;
	m_nActivateNextItem=-1;
	m_nForbiddenItem=-1;

	m_rectCloseBtn.SetRectEmpty();
	m_rectRestoreBtn.SetRectEmpty();
	m_rectMinimizeBtn.SetRectEmpty();
	m_pressedBtn=OX_MNU_NONE;

	m_cxLeftBorder=0;
	m_cyTopBorder=0;
	m_cxRightBorder=0;
	m_cyBottomBorder=0;

	m_bIsInSurfingMode=FALSE;

	m_bForceEntireRow=FALSE;

	m_bIsMenuMetricInitialized=FALSE;

	m_nCheckForDragDropEventTimerID=0;
	m_nWouldBeDraggedItem=-1;

	m_bStaySurfing=FALSE;
	m_pMenuSkin = NULL;
}


COXMenuBar::~COXMenuBar()
{
	for(int nIndex=PtrToInt(m_arrCreatedPopupMenus.GetSize()-1); nIndex>=0; nIndex--)
	{
		::DestroyMenu(m_arrCreatedPopupMenus[nIndex]);
	}
	m_arrCreatedPopupMenus.RemoveAll();

	if ( m_pMenuSkin != NULL )
		delete m_pMenuSkin;
}

BEGIN_MESSAGE_MAP(COXMenuBar, COXCoolToolBar)
	//{{AFX_MSG_MAP(COXMenuBar)
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_NCMOUSEMOVE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_UPDATE_COMMAND_UI_RANGE(0,0xffff,OnUpdateMenuItem)
	ON_NOTIFY_REFLECT_EX(TBN_DROPDOWN,OnMenuDropDown)
	ON_MESSAGE(WM_DISPLAYPOPUPMENU, OnDisplayPopupMenu)
	ON_COMMAND(ID_OXCUSTTB_DELETE,OnCustTBDelete)
	// reflect messages to make customization work
	ON_NOTIFY_REFLECT_EX(TBN_BEGINDRAG, OnTBNBeginDrag)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL COXMenuBar::Create(CWnd* pParentWnd, 
						DWORD dwStyle/*=WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_SIZE_DYNAMIC*/, 
						UINT nID/*=AFX_IDW_MENUBAR*/)
{
	ASSERT(pParentWnd!=NULL);
	ASSERT(::IsWindow(pParentWnd->m_hWnd));

	// CToolBar bug !!!!!
	// When menu bar is created again it uses data saved in m_sizeImage 
	// variable for initializing button sizes. CToolBar reset this value
	// as aresult of applying different operations. COXMenuBar doesn't use
	// images so m_sizeImage will be set to (0,0). The thing is CToolBar::CreateEx
	// doesn't like m_sizeImage being set to (0,0)!!!!
	// So we need to set this variable to some reasonable non-zero value.
	// We will use the same numbers as in CToolBar initialization part
	if(m_sizeImage.cx==0 || m_sizeImage.cy==0)
	{
		// not the first creation, reset variables

		// default image sizes
		m_sizeImage.cx = 16;
		m_sizeImage.cy = 15;

		// default button sizes
		m_sizeButton.cx = 23;
		m_sizeButton.cy = 22;
	}

	// try to create cool toolbar
	if(!COXCoolToolBar::Create(pParentWnd,dwStyle,nID))
	{
		return FALSE;
	}

	// make sure dropdown arrows are not appeared on the buttons
	SetDropDownArrow(FALSE);

	// set size of images to zero (cannot call SetSizes function
	// because it will assert if zero value is specified)
	SendMessage(TB_SETBITMAPSIZE,0,MAKELPARAM(0,0));

	// set window text
	CString sText;
	VERIFY(sText.LoadString(IDS_OX_MENUBARTITLE));//"Menu"
	SetWindowText(sText);

	// save pointer to parent window
	m_pFrameWnd=pParentWnd;

	return TRUE;
}

void COXMenuBar::OnFloatingDocking(BOOL bFloating) 
{
	// TODO: Add your message handler code here
	COXCoolToolBar::OnFloatingDocking(bFloating);
	
	if(bFloating)
	{
		// remove all items form mini dock frame window system menu
		CFrameWnd* pMiniDockFrame=(CFrameWnd*)GetDockingFrame();
		ASSERT(pMiniDockFrame!=NULL &&
			pMiniDockFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)));
		if(pMiniDockFrame!=NULL && pMiniDockFrame->
			IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)))
		{
			CMenu* pPopup=pMiniDockFrame->GetSystemMenu(FALSE);
			while(pPopup->DeleteMenu(0,MF_BYPOSITION));
		}
	}
}

void COXMenuBar::OnChangeDockSide(DWORD dwDockSide)
{
	UNREFERENCED_PARAMETER(dwDockSide);
	if(!IsFloating() && GetParentFrame()!=NULL)
	{
		GetParentFrame()->RecalcLayout();
	}
}

LONG COXMenuBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	// refresh icon if any is displayed
	UpdateIcon(TRUE);

	COXCoolToolBar::OnIdleUpdateCmdUI(wParam,lParam);

	return 0L;
}

LONG COXMenuBar::OnDisplayPopupMenu(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	MSG msg;
	// handle only the last one
	while(::PeekMessage(&msg,GetSafeHwnd(),
		WM_DISPLAYPOPUPMENU,WM_DISPLAYPOPUPMENU,PM_REMOVE))
	{
		wParam=msg.wParam;
	}

	ASSERT(wParam>=0 && wParam<=(WPARAM)::GetMenuItemCount(m_hMenu));

	DisplayPopupMenu(PtrToInt(wParam));

	return 0;
}

void COXMenuBar::OnUpdateMenuItem(CCmdUI* pCmd)
{
	// if there is nothing to update
	if(m_hMenu==NULL) 
		return;

	CMenu* pMenu=CMenu::FromHandle(m_hMenu);
	ASSERT(pMenu!=NULL);
	if(pMenu==NULL) 
		return;

	// enable menu items
	if(pCmd->m_nID-ID_CMDBASE<pMenu->GetMenuItemCount())
	{
		pCmd->Enable((pMenu->GetMenuState(pCmd->m_nID-ID_CMDBASE,
			MF_BYPOSITION)&(MF_DISABLED|MF_GRAYED))==0);
	}
}

BOOL COXMenuBar::OnMenuDropDown(NMHDR* pNotifyStruct, LRESULT* result)
{
	NMTOOLBAR* pNMTB=(NMTOOLBAR*)pNotifyStruct;

	*result=TBDDRET_DEFAULT;

	// don't call it twice for forbidden item
	if(m_nForbiddenItem!=pNMTB->iItem-ID_CMDBASE)
	{
		// analyze item
		CMenu* pMenu=CMenu::FromHandle(m_hMenu);
		ASSERT(pMenu!=NULL);
		UINT nCmdID=pMenu->GetMenuItemID(pNMTB->iItem-ID_CMDBASE);
		if(nCmdID==-1 || nCmdID==0xffff)
		{
			PostMessage(WM_DISPLAYPOPUPMENU,pNMTB->iItem-ID_CMDBASE);
		}
		else if(nCmdID>0)
		{
			if(!IsInAdvancedCustomizationMode())
			{
				// make sure the state of previously active button is restored
				RestoreMenuButton();

				// set current active item
				m_nActiveMenuItem=pNMTB->iItem-ID_CMDBASE;
				m_nActivateNextItem=-1;
				m_nForbiddenItem=m_nActiveMenuItem;

				// set button to pressed state
				CToolBarCtrl& tool=GetToolBarCtrl();
				tool.PressButton(m_nActiveMenuItem+ID_CMDBASE);

				ASSERT(m_pFrameWnd!=NULL);
				m_pFrameWnd->SendMessage(WM_COMMAND,nCmdID,(LPARAM)NULL);

				m_nForbiddenItem=-1;
				RestoreMenuButton();
			}
		}
	}
	else
	{
		m_nForbiddenItem=-1;
	}

	return FALSE;
}

#if _MSC_VER >= 1400
LRESULT COXMenuBar::OnNcHitTest(CPoint point)
#else
UINT COXMenuBar::OnNcHitTest(CPoint point)
#endif
{
	// TODO: Add your message handler code here and/or call default
	
	CRect rect;
	GetWindowRect(rect);
	CPoint ptTest=point;
	ptTest-=rect.TopLeft();

	// handle areas where icon and close, restore, minimize buttons
	// are located in a specific way
	if(GetIcon()!=NULL && (m_iconRect.PtInRect(ptTest) || 
		m_rectCloseBtn.PtInRect(ptTest) || m_rectRestoreBtn.PtInRect(ptTest) || 
		m_rectMinimizeBtn.PtInRect(ptTest)))
	{
		return HTCAPTION;
	}

	return CToolBar::OnNcHitTest(point);
}

void COXMenuBar::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	UNREFERENCED_PARAMETER(nHitTest);

	CRect rect;
	GetWindowRect(rect);
	CPoint ptTest=point;
	ptTest-=rect.TopLeft();

	ASSERT(GetIcon()!=NULL && (m_iconRect.PtInRect(ptTest) || 
		m_rectCloseBtn.PtInRect(ptTest) || m_rectRestoreBtn.PtInRect(ptTest) || 
		m_rectMinimizeBtn.PtInRect(ptTest)));

	if(GetIcon()==NULL)
		return;

	// handle mouse click over icon and close, restore, minimize buttons
	//

	m_pressedBtn=OX_MNU_NONE;

	MENUBARHITTEST hitTest=(MENUBARHITTEST)HitTest(ptTest);

	if(hitTest==OX_MNU_ICON)
	{
		PostMessage(WM_DISPLAYPOPUPMENU,::GetMenuItemCount(m_hMenu));
	}
	else if(hitTest==OX_MNU_CLOSEBTN || hitTest==OX_MNU_RESTOREBTN || hitTest==OX_MNU_MINIMIZEBTN)
	{
		SetCapture();
		m_pressedBtn=hitTest;
		RedrawToolBar(FALSE,TRUE);
	}
	else
	{
		COXCoolToolBar::OnNcLButtonDown(nHitTest,point);
	}
}

void COXMenuBar::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	// handle mouse double click over icon
	//
	if(GetIcon()!=NULL)
	{
		ASSERT_KINDOF(CMDIFrameWnd,m_pFrameWnd);
	    BOOL bMaximize=FALSE;
		CMDIChildWnd* pChild=((CMDIFrameWnd*)m_pFrameWnd)->
			MDIGetActive(&bMaximize);
		ASSERT(pChild!=NULL && bMaximize);
		if(pChild==NULL || !bMaximize)
		{
			COXCoolToolBar::OnNcLButtonDblClk(nHitTest, point);
			return;
		}

		int hitTest=HitTest(point,OX_MNU_SCREEN);
		if(hitTest==OX_MNU_ICON)
			pChild->PostMessage(WM_SYSCOMMAND,SC_CLOSE);
	}

	COXCoolToolBar::OnNcLButtonDblClk(nHitTest, point);
}


void COXMenuBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(IsFloating())
	{
		CToolBarCtrl& tool=GetToolBarCtrl();
		tool.PressButton(ID_CMDBASE,FALSE);
	}


	if (!IsFloatingEnabled())
	{
		if (HitTest(point, OX_MNU_CLIENT) != OX_MNU_NONE)
			COXCoolToolBar::OnLButtonDblClk(nFlags, point);
	}
	else
		COXCoolToolBar::OnLButtonDblClk(nFlags, point);

	return;
}


void COXMenuBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(IsFloating())
	{
		CToolBarCtrl& tool=GetToolBarCtrl();
		tool.PressButton(ID_CMDBASE,FALSE);
	}
	
	if (!IsFloatingEnabled())
	{
		if (HitTest(point, OX_MNU_CLIENT) != OX_MNU_NONE)
			COXCoolToolBar::OnLButtonDown(nFlags, point);
	}
	else
		COXCoolToolBar::OnLButtonDown(nFlags, point);

	if(GetDraggedButton()==-1  &&  IsInAdvancedCustomizationMode())
	{
		if(m_nCheckForDragDropEventTimerID==0)
		{
			m_nWouldBeDraggedItem=COXCoolToolBar::HitTest(&point);
			m_nCheckForDragDropEventTimerID=SetTimer(IDT_OXCHECKFORDRAGDROPEVENT,
				ID_OXCHECKFORDRAGDROPEVENT_DELAY,NULL);
			ASSERT(m_nCheckForDragDropEventTimerID!=0);
		}
		else
		{
			KillTimer(m_nCheckForDragDropEventTimerID);
			m_nCheckForDragDropEventTimerID=0;
		}
	}

	return;
}


void COXMenuBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	// handle mouse click close, restore, minimize buttons
	//
	if (GetIcon() == NULL || ::GetCapture() != GetSafeHwnd() || m_bDragging)
	{
		COXCoolToolBar::OnLButtonUp(nFlags,point);
		return;
	}

	::ReleaseCapture();

    BOOL bMaximize=FALSE;
	ASSERT_KINDOF(CMDIFrameWnd,m_pFrameWnd);
	CMDIChildWnd* pChild=((CMDIFrameWnd*)m_pFrameWnd)->MDIGetActive(&bMaximize);
	ASSERT(pChild!=NULL && bMaximize);
	if(pChild==NULL || !bMaximize)
	{
		COXCoolToolBar::OnLButtonUp(nFlags,point);
		return;
	}

	CRect rect;
	GetWindowRect(rect);
	CPoint ptTest=point;
	ClientToScreen(&ptTest);
	ptTest-=rect.TopLeft();

	MENUBARHITTEST hitTest=(MENUBARHITTEST)HitTest(ptTest);

	// send corresponding messages

	if(m_pressedBtn==hitTest)
	{
		if(m_pressedBtn==OX_MNU_CLOSEBTN)
			pChild->PostMessage(WM_SYSCOMMAND,SC_CLOSE);
		else if(m_pressedBtn==OX_MNU_RESTOREBTN)
			pChild->PostMessage(WM_SYSCOMMAND,SC_RESTORE);
		else if(m_pressedBtn==OX_MNU_MINIMIZEBTN)
			pChild->PostMessage(WM_SYSCOMMAND,SC_MINIMIZE);
	}

	m_pressedBtn=OX_MNU_NONE;

	RedrawToolBar(FALSE,TRUE);
}


void COXMenuBar::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(IsFloating())
	{
		CToolBarCtrl& tool=GetToolBarCtrl();
		tool.PressButton(ID_CMDBASE,FALSE);
	}
	
	COXCoolToolBar::OnRButtonDown(nFlags,point);
}


void COXMenuBar::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(IsFloating())
	{
		CToolBarCtrl& tool=GetToolBarCtrl();
		tool.PressButton(ID_CMDBASE,FALSE);
	}
	
	COXCoolToolBar::OnRButtonDblClk(nFlags,point);
}


void COXMenuBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if (GetIcon())
	{
		CWindowDC dc(this);
		GetToolbarSkin()->DrawMenuButtons(&dc, this);
	}

	if(m_nCheckForDragDropEventTimerID!=0 && ::GetKeyState(VK_LBUTTON)>=0)
	{
		KillTimer(m_nCheckForDragDropEventTimerID);
		m_nCheckForDragDropEventTimerID=0;
	}

	if(GetIcon()==NULL || ::GetCapture()!=GetSafeHwnd())
	{
		COXCoolToolBar::OnMouseMove(nFlags, point);
		return;
	}

	CPoint ptTest=point;

	// handle mouse cursor movement over close, restore, minimize buttons
	//

	int hitTest=HitTest(ptTest,OX_MNU_CLIENT);
	hitTest=hitTest>=0 ? OX_MNU_NONE : hitTest;

	if(m_pressedBtn!=hitTest)
	{
		m_pressedBtn=(MENUBARHITTEST)hitTest;
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_FRAME);
	}

	COXCoolToolBar::OnMouseMove(nFlags, point);
}


void COXMenuBar::OnKillFocus(CWnd* pNewWnd) 
{
	COXCoolToolBar::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here

	// reset the surfing mode
	m_bIsInSurfingMode=FALSE;

}


void COXMenuBar::OnTimer(UINT nIDEvent)
{
	if((int)nIDEvent==m_nCheckForDragDropEventTimerID)
	{
		KillTimer(m_nCheckForDragDropEventTimerID);
		m_nCheckForDragDropEventTimerID=0;

		if(::IsWindow(GetSafeHwnd()))		
		{
			// do drag and drop
			//
			if(::GetKeyState(VK_LBUTTON)<0 && IsInAdvancedCustomizationMode())
			{
				if(m_nWouldBeDraggedItem>=0 && 
					m_nWouldBeDraggedItem<GetToolBarCtrl().GetButtonCount())
				{
					ASSERT(GetParent()!=NULL);
					NMTOOLBAR nmtb;
					nmtb.hdr.hwndFrom=GetSafeHwnd();
					nmtb.hdr.idFrom=GetDlgCtrlID();
					nmtb.hdr.code=TBN_BEGINDRAG;
					nmtb.iItem=m_nWouldBeDraggedItem+ID_CMDBASE;
					GetParent()->
						SendMessage(WM_NOTIFY,(WPARAM)nmtb.hdr.idFrom,(LPARAM)&nmtb);
					m_nWouldBeDraggedItem=-1;
				}
			}
			//
			////////////////////////////////////
		}

		return;
	}

	COXCoolToolBar::OnTimer(nIDEvent);
}


void COXMenuBar::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	UpdateMenuMetrics();

	COXCoolToolBar::OnSettingChange(uFlags, lpszSection);
}


BOOL COXMenuBar::OnTBNBeginDrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTOOLBAR* pNMToolBar=(NMTOOLBAR*)pNMHDR;

	int nIndex=CommandToIndex(pNMToolBar->iItem);

#ifdef OX_CUSTOMIZE_COMMANDS
	if(IsInAdvancedCustomizationMode())
	{
		int nState=GetToolBarCtrl().GetState(GetItemID(nIndex));
		if((nState&TBSTATE_PRESSED)!=0)
		{
			GetToolBarCtrl().SetState(GetItemID(nIndex),nState&~TBSTATE_PRESSED);
		}

		CWnd* pParentWnd=GetParent();
		ASSERT(pParentWnd!=NULL);
		pNMToolBar->hdr.code=TBN_QUERYDELETE;
		if(pParentWnd->SendMessage(WM_NOTIFY,pNMToolBar->hdr.idFrom,
			(LPARAM)pNMToolBar))
		{
			SendCustomizeNotification(ID_OXCUSTMB_RESET_POPUP_MENU);

			BOOL bWasInAdvancedCustomizationMode=IsInAdvancedCustomizationMode();
			if(GetCustomizedButton()!=nIndex)
			{
				SetCustomizedButton(nIndex);
			}
			SetDraggedButton(nIndex);

			// mark the control as the one that launched drag'n'drop operation
			m_bDragDropOwner=TRUE;

			CMenu* pMenu=CMenu::FromHandle(GetMenu());
			ASSERT(pMenu!=NULL);
			CString sText;
			pMenu->GetMenuString(nIndex,sText,MF_BYPOSITION);
			HMENU hSubmenu=::GetSubMenu(pMenu->GetSafeHmenu(),nIndex);
			UINT nCmdID=pMenu->GetMenuItemID(nIndex);
			if(nCmdID==0xffff)
			{
				nCmdID=(UINT)-1;
			}
			COleDataSource* pDataSource=COXDragDropCommands::PrepareDragDropData(
				sText,-1/*image index*/,nCmdID/*command ID*/,0/*button style*/,hSubmenu);
			ASSERT(pDataSource!=NULL);
			m_bDragDropOperation=TRUE;
			DROPEFFECT dropEffect=
				COXDragDropCommands::DoDragDrop(pDataSource,GetDropSource());
			if(DROPEFFECT_MOVE==dropEffect || (DROPEFFECT_NONE==dropEffect &&
				::GetKeyState(VK_LBUTTON)>=0 && !m_bDragDropOperation))
			{
				int nDraggedButtonIndex=GetDraggedButton();
				OnRemoveDraggedButton(nDraggedButtonIndex);
			}
			SetDraggedButton(-1);

			//delete drag source (we are responsible to do that)
			delete pDataSource;

			// unmark as the control which launched drag'n'drop operation
			m_bDragDropOwner=FALSE;


			if(!bWasInAdvancedCustomizationMode)
			{
				SetCustomizedButton(-1);
			}
		}
	}
#else
	UNREFERENCED_PARAMETER(nIndex);
#endif	//	OX_CUSTOMIZE_COMMANDS

	*pResult=0;
	return FALSE;
}


LONG COXMenuBar::OnDragOver(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	// toolbar must be in advanced customizable state
	if(!IsCustomizable(TRUE))
		return (LONG)FALSE;

	// lParam is the pointer to SHBDROPTARGETACTION structure
	LPSHBDROPTARGETACTION pSHBDTAction=(LPSHBDROPTARGETACTION)lParam;
	ASSERT(pSHBDTAction!=NULL);

	ASSERT(pSHBDTAction->pWnd);
	ASSERT(pSHBDTAction->pWnd->GetSafeHwnd()==GetSafeHwnd());

	pSHBDTAction->result=(LRESULT)DROPEFFECT_NONE;
#ifdef OX_CUSTOMIZE_COMMANDS
	// Can we use this object?
	if(pSHBDTAction->pDataObject->
		IsDataAvailable(COXDragDropCommands::m_cfCommandButton))
	{
		BOOL bQualified=!(m_bDragDropOwner && 
			(pSHBDTAction->dwKeyState & MK_CONTROL)==MK_CONTROL);
		if(bQualified)
		{
			BOOL bAllowToDrop=TRUE;
			if(!(m_bDragDropOwner && 
				(pSHBDTAction->dwKeyState & MK_CONTROL)!=MK_CONTROL))
			{
				// Get the drag item info
				//
				HGLOBAL hgData=pSHBDTAction->pDataObject->
					GetGlobalData(COXDragDropCommands::m_cfCommandButton);
				ASSERT(hgData!=NULL);
				// lock it
				BYTE* lpItemData=(BYTE*)::GlobalLock(hgData);

				// get button command ID
				int nCommandID=*(int*)lpItemData;
				lpItemData+=sizeof(int);
				
				// get button text
				CString sText((LPTSTR)lpItemData);
				lpItemData+=(sText.GetLength()+1)*sizeof(TCHAR);

				// unlock it
				::GlobalUnlock(hgData);
				// free it
				::GlobalFree(hgData);

				if(nCommandID==0)
				{
					bAllowToDrop=FALSE;
				}

				if(sText.IsEmpty())
				{
					bAllowToDrop=FALSE;
				}

				if(GetCustomButtonIndex(nCommandID)!=-1)
				{
					bQualified=FALSE;
				}
			}

			if(!bAllowToDrop && !IsInAdvancedCustomizationMode())
			{
				bQualified=FALSE;
			}

			if(bQualified)
			{
				// analize the current cursor position
				//
				CPoint point=pSHBDTAction->point;

				int nButtonIndex=COXCoolToolBar::HitTest(&point);
				BOOL bIsOut=(nButtonIndex<0);
				nButtonIndex=(bIsOut ? -nButtonIndex : nButtonIndex);
				if(nButtonIndex>=GetToolBarCtrl().GetButtonCount() && 
					GetToolBarCtrl().GetButtonCount()>0)
				{
					nButtonIndex=-1;
					bQualified=FALSE;
				}
				else
				{
					if(nButtonIndex!=GetCustomizedButton())
					{
						SetCustomizedButton(nButtonIndex);
						if(!(m_bDragDropOwner && nButtonIndex==GetDraggedButton()))
						{
							DisplayPopupMenu(nButtonIndex);
						}
					}

					if(bAllowToDrop)
					{
						CWnd* pParentWnd=GetParent();
						ASSERT(pParentWnd!=NULL);
						NMTOOLBAR nmtb;
						nmtb.hdr.hwndFrom=GetSafeHwnd();
						nmtb.hdr.idFrom=GetDlgCtrlID();
						nmtb.hdr.code=TBN_QUERYINSERT;
						nmtb.iItem=nButtonIndex;
						if(!pParentWnd->
							SendMessage(WM_NOTIFY,nmtb.hdr.idFrom,(LPARAM)&nmtb))
						{
							bQualified=FALSE;
						}
						else
						{
							TBINSERTMARK tbim;
							tbim.iButton=nButtonIndex;
							tbim.dwFlags=0;
							CRect rectItem;
							GetItemRect(tbim.iButton,rectItem);
							if(point.x>=rectItem.left+rectItem.Width()/2 || point.x<0)
							{
								tbim.dwFlags=TBIMHT_AFTER;
							}
							SetInsertMark(&tbim);

							// Check if the control key was pressed          
							if((pSHBDTAction->dwKeyState & MK_CONTROL)==MK_CONTROL)
							{
								pSHBDTAction->result=(LRESULT)DROPEFFECT_COPY;
							}
							else
							{
								pSHBDTAction->result=(LRESULT)DROPEFFECT_MOVE; 
							}
						}
					}
				}
			}
		}
			
		if(!bQualified)
		{
			SetInsertMark(-1);
		}
	}
#endif	//	OX_CUSTOMIZE_COMMANDS

	return (LONG)TRUE;
}


LONG COXMenuBar::OnDrop(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if(!IsCustomizable(TRUE))
		return (LONG)FALSE;

	// lParam is the pointer to SHBDROPTARGETACTION structure
	LPSHBDROPTARGETACTION pSHBDTAction=(LPSHBDROPTARGETACTION)lParam;
	ASSERT(pSHBDTAction!=NULL);

	ASSERT(pSHBDTAction->pWnd);
	ASSERT(pSHBDTAction->pWnd->GetSafeHwnd()==GetSafeHwnd());

	pSHBDTAction->result=(LRESULT)FALSE;

#ifdef OX_CUSTOMIZE_COMMANDS
	// if dragged item is to be copied or moved
	if((pSHBDTAction->dropEffect&DROPEFFECT_COPY)!=0 || 
		(pSHBDTAction->dropEffect&DROPEFFECT_MOVE)!=0)
	{
		// data must be in the specific format
		ASSERT(pSHBDTAction->pDataObject->
			IsDataAvailable(COXDragDropCommands::m_cfCommandButton));

		TBINSERTMARK tbim;
		GetInsertMark(&tbim);
		int nButtonIndex=tbim.iButton;
		// remove insert mark
		SetInsertMark(-1);
		if(nButtonIndex!=-1 || GetToolBarCtrl().GetButtonCount()==0)
		{
			int nDraggedButtonIndex=GetDraggedButton();

			if(m_bDragDropOwner && (pSHBDTAction->dropEffect&DROPEFFECT_COPY)==0)
			{
				ASSERT(GetToolBarCtrl().GetButtonCount()>0);

				ASSERT(nDraggedButtonIndex!=-1);
				if(nButtonIndex==nDraggedButtonIndex)
				{
					pSHBDTAction->result=(LRESULT)FALSE;
					return (LONG)TRUE;
				}
			}

			nButtonIndex=(tbim.dwFlags==0 ? nButtonIndex : nButtonIndex+1);
			int nButtonCount=GetToolBarCtrl().GetButtonCount();
			if(nButtonIndex==-1)
			{
				ASSERT(nButtonCount==0 && !m_bDragDropOwner);
				nButtonIndex=0;
			}
			else if(nButtonIndex>nButtonCount)
			{
				nButtonIndex=nButtonCount;
			}

			HMENU hMenu=GetMenu();
			ASSERT(hMenu!=NULL);

			// Get the drag item info
			//
			HGLOBAL hgData=pSHBDTAction->pDataObject->
				GetGlobalData(COXDragDropCommands::m_cfCommandButton);
			ASSERT(hgData!=NULL);
			// lock it
			BYTE* lpItemData=(BYTE*)::GlobalLock(hgData);

			RetrieveDragDropMenuItem(lpItemData,hMenu,nButtonIndex);

			// unlock it
			::GlobalUnlock(hgData);
			// free it
			::GlobalFree(hgData);

			// update dragged item position
			if(nButtonIndex<nDraggedButtonIndex)
			{
				SetDraggedButton(nDraggedButtonIndex+1);
			}

			// drag'n'drop operation completed successfully
			pSHBDTAction->result=(LRESULT)TRUE;

			MarkAsChanged();
			DrawMenuBar();
		}
		else
		{
			pSHBDTAction->result=(LRESULT)FALSE;
		}
	}
#endif	//	OX_CUSTOMIZE_COMMANDS

	m_bDragDropOperation=FALSE;

	// we handled the message
	return (LONG)TRUE;
}


void COXMenuBar::RetrieveDragDropMenuItem(BYTE*& lpData, HMENU hMenu, int nButtonIndex)
{
	ASSERT(lpData!=NULL);
	ASSERT(hMenu!=NULL);
	ASSERT(::IsMenu(hMenu));

	while(TRUE)
	{
		// get button command ID
		int nCommandID=*(int*)lpData;
		lpData+=sizeof(int);
		
		// get button text
		CString sText((LPTSTR)lpData);
		lpData+=(sText.GetLength()+1)*sizeof(TCHAR);

		// get button image index
		int nImageIndex=*(int*)lpData;
		lpData+=sizeof(int);
		UNREFERENCED_PARAMETER(nImageIndex);

		// get button style
		BYTE fsStyle=*(BYTE*)lpData;
		lpData+=sizeof(BYTE);
		UNREFERENCED_PARAMETER(fsStyle);

		// get finish flag
		int nFinish=*(int*)lpData;
		lpData+=sizeof(int);

		// update menu
		if(nCommandID==-1)
		{
			HMENU hSubMenu=::CreatePopupMenu();
			ASSERT(hSubMenu!=NULL);
			m_arrCreatedPopupMenus.Add(hSubMenu);
			VERIFY(::InsertMenu(hMenu,nButtonIndex,
				MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT_PTR)hSubMenu,sText)!=0);
			if(nFinish!=0)
			{
				RetrieveDragDropMenuItem(lpData,hSubMenu,0);
			}
		}
		else
		{
			VERIFY(::InsertMenu(hMenu,nButtonIndex,
				MF_BYPOSITION|(nCommandID==0 ? MF_SEPARATOR : MF_STRING),
				(UINT)nCommandID,sText)!=0);
		}
		nButtonIndex++;

		if(nFinish==0 || nFinish==2)
		{
			break;
		}
	}
}


void COXMenuBar::OnRemoveDraggedButton(int nButtonIndex)
{
	// update menu buttons ID
	int nButtonCount=GetToolBarCtrl().GetButtonCount();
	for(int nIndex=nButtonIndex+1; nIndex<nButtonCount; nIndex++)
	{
		UINT nID=GetItemID(nIndex);
		if(nID!=0)
		{
			GetToolBarCtrl().SetCmdID(nIndex,nID-1);
		}
	}

	COXCoolToolBar::OnRemoveDraggedButton(nButtonIndex);

	HMENU hMenu=GetMenu();
	ASSERT(hMenu!=NULL);
	VERIFY(::DeleteMenu(hMenu,nButtonIndex,MF_BYPOSITION));
	MarkAsChanged();
}


void COXMenuBar::OnCustTBDelete()
{
	ASSERT(IsInAdvancedCustomizationMode());
	int nCustomizedButtonIndex=GetCustomizedButton();
	ASSERT(nCustomizedButtonIndex!=-1);

	if(SendCustomizeNotification(ID_OXCUSTTB_DELETE))
		return;

	OnRemoveDraggedButton(nCustomizedButtonIndex);
}


////////////////////////////////////////////////////////////////////


CSize COXMenuBar::CalcLayout(DWORD dwMode, int nLength)
{
	CSize sizeResult=COXCoolToolBar::CalcLayout(dwMode,nLength);

	RecalculateItemWidths();

	// adjust the size of menu bar if it should take the entire 
	// row/column
	if(m_bTakeEntireRow)
	{
		CSize size=GetEntireRow((dwMode & LM_HORZ));
		if((dwMode&LM_HORZDOCK) || (dwMode&LM_VERTDOCK) || m_bForceEntireRow)
		{
			if(dwMode & LM_HORZ)
				sizeResult.cx=size.cy-size.cx;
			else
				sizeResult.cy=size.cy-size.cx;
		}

	}

	return sizeResult;
}

CSize COXMenuBar::GetEntireRow(BOOL bHorz) const
{
	CWnd* pWndFrame=m_pDockSite;
	if(pWndFrame==NULL)
		pWndFrame=GetParentFrame();
	if(pWndFrame==NULL)
		pWndFrame=m_pFrameWnd;
	if(pWndFrame==NULL)
		pWndFrame=GetParent();
	ASSERT(pWndFrame!=NULL);


	CSize sizeResult(0,0);
	CRect rect;
	pWndFrame->GetClientRect(rect);
	if(bHorz)
	{
		sizeResult.cx=-afxData.cxBorder2;
		sizeResult.cy=rect.Width()+afxData.cxBorder2;
	}
	else
	{
		sizeResult.cx=-afxData.cyBorder2;
		sizeResult.cy=rect.Height()+afxData.cyBorder2;
	}

	if(pWndFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
	{
		// find corresponding dock bar 

		CRect rectDockBar;
		CWnd* pDockBar;

		if(bHorz)
		{
			pDockBar=((CFrameWnd*)pWndFrame)->
				GetControlBar(AFX_IDW_DOCKBAR_TOP);
			if(pDockBar!=NULL)
				pDockBar=((CFrameWnd*)pWndFrame)->
					GetControlBar(AFX_IDW_DOCKBAR_BOTTOM);
		}
		else
		{
			pDockBar=((CFrameWnd*)pWndFrame)->
				GetControlBar(AFX_IDW_DOCKBAR_LEFT);
			if(pDockBar!=NULL)
				pDockBar=((CFrameWnd*)pWndFrame)->
					GetControlBar(AFX_IDW_DOCKBAR_RIGHT);
		}

		if(pDockBar!=NULL)
		{
			pDockBar->GetWindowRect(rectDockBar);
			pDockBar->ScreenToClient(rectDockBar);
			if(bHorz)
			{
				sizeResult.cx=-afxData.cxBorder2;
				sizeResult.cy=rectDockBar.right+afxData.cyBorder2;
			}
			else
			{
				sizeResult.cx=-afxData.cxBorder2;
					sizeResult.cy=rectDockBar.bottom+afxData.cyBorder2;
			}
		}
	}

	return sizeResult;
}


BOOL COXMenuBar::IsCustomizable(BOOL bAdvanced/*=TRUE*/) const
{ 
	return (bAdvanced ? TRUE : FALSE);
}


// global handle of the window that had focus before the menu bar
// was set in the surfing mode
HWND g_hWndHadFocus=NULL;

BOOL COXMenuBar::SetSurfingMode(BOOL bIsInSurfingMode/*=TRUE*/, int nItem/*=0*/)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(nItem==-1 || (nItem>=0 && nItem<=GetToolBarCtrl().GetButtonCount()));
	if(nItem==GetToolBarCtrl().GetButtonCount())
	{
		nItem=-1;
	}

	if(bIsInSurfingMode==m_bIsInSurfingMode)
	{
		ASSERT(!m_bIsInSurfingMode || !IsInDisplayingMode());
		ASSERT((m_bIsInSurfingMode && ::GetFocus()==m_hWnd) || 
			(!m_bIsInSurfingMode && ::GetFocus()!=m_hWnd));
		// just reset the hot item
		if(m_bIsInSurfingMode)
		{
			SendMessage(TB_SETHOTITEM,nItem);
		}
		return TRUE;
	}

	// double check
	if(bIsInSurfingMode && IsInDisplayingMode())
	{
		TRACE(_T("COXMenuBar::SetSurfingMode: Cannot be set in the surfing mode while displaying popup menus\n"));
		return FALSE;
	}

	if(bIsInSurfingMode)
	{
		// save thae handle of the window that had focus
		g_hWndHadFocus=::GetFocus();
		SetFocus();
		// reset hot item
		SendMessage(TB_SETHOTITEM,nItem);

		if(!m_bStaySurfing)
		{
			// send WM_ENTERMENULOOP and WM_INITMENU messages. 
			m_pFrameWnd->SendMessage(WM_ENTERMENULOOP,(WPARAM)FALSE);
			m_pFrameWnd->SendMessage(WM_INITMENU,(WPARAM)m_hMenu);
			// send WM_MENUSELECT message
			m_pFrameWnd->SendMessage(WM_MENUSELECT,
				MAKEWPARAM(nItem,MF_POPUP|MF_HILITE),(LPARAM)m_hMenu);
		}
	}
	else		
	{
		g_hWndHadFocus=::IsWindow(g_hWndHadFocus) ? g_hWndHadFocus : NULL;
		// reset focus window
		::SetFocus(g_hWndHadFocus);
		g_hWndHadFocus=NULL;
		// reset hot item
		SendMessage(TB_SETHOTITEM,(WPARAM)-1);
	}

	m_bIsInSurfingMode=bIsInSurfingMode;

	return TRUE;
}

void COXMenuBar::BookSpace(CRect& rectBookedSpace, DWORD dwMode) 
{
	// book some space for close, restore, minimize buttons
	if(dwMode&LM_HORZ)
		rectBookedSpace.right=CalcButtonRects();
	else
		rectBookedSpace.bottom=CalcButtonRects();
}

void COXMenuBar::DrawInBookedSpace(CDC& dc, CRect& rectToDrawIn)
{
	if(GetIcon()!=NULL)
	{
		ASSERT(m_pFrameWnd!=NULL);
		if(m_pFrameWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
		{
			// recalculate close, restore, minimize buttons rectangles
			CalcButtonRects();

			// and draw them
			DrawButtons(dc);

			// update the rectangle that can be used by parent window to draw 
			// additional elements
			if(m_dwStyle&CBRS_ORIENT_HORZ)
				rectToDrawIn.right=(!m_rectMinimizeBtn.IsRectEmpty() ? 
					m_rectMinimizeBtn.left : (!m_rectRestoreBtn.IsRectEmpty() ? 
					m_rectRestoreBtn.left : m_rectCloseBtn.left))-6;
			else
				rectToDrawIn.bottom=(!m_rectMinimizeBtn.IsRectEmpty() ? 
					m_rectMinimizeBtn.top : (!m_rectRestoreBtn.IsRectEmpty() ? 
					m_rectRestoreBtn.top : m_rectCloseBtn.top))-6;
		}
	}
}

BOOL COXMenuBar::SetMenu(HMENU hMenu)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	if(m_hMenu==hMenu)
		return TRUE;

	SaveCustomizedMenu();

	m_hMenu=hMenu;

	if(IsCustomizable(TRUE))
	{
		CDocTemplate* pDocTemplate=NULL;
		if(!m_mapCustomizableMenus.Lookup(m_hMenu,pDocTemplate))
		{
			CDocument* pDocument=NULL;
			CWnd* pWnd=AfxGetMainWnd();
			CFrameWnd* pFrameWnd=DYNAMIC_DOWNCAST(CFrameWnd,pWnd);
			if(pFrameWnd!=NULL)
			{
				if(pFrameWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
				{
					CMDIChildWnd* pChildWnd=((CMDIFrameWnd*)pFrameWnd)->MDIGetActive();
					if(pChildWnd==NULL)
					{
						pDocument=NULL;
					}
					else
					{
						pDocument=pChildWnd->GetActiveDocument();
					}
				}
				else
				{
					pDocument=pFrameWnd->GetActiveDocument();
				}
			}
			pDocTemplate=(pDocument==NULL ? NULL : pDocument->GetDocTemplate());

			BOOL bAlreadyExist=FALSE;
			POSITION pos=m_mapCustomizableMenus.GetStartPosition();
			while(pos!=NULL)
			{
				CDocTemplate* pTestDocTemplate=NULL;
				HMENU hTestMenu=NULL;
				m_mapCustomizableMenus.GetNextAssoc(pos,hTestMenu,pTestDocTemplate);
				ASSERT(hTestMenu!=NULL);
				if(pTestDocTemplate==pDocTemplate)
				{
					bAlreadyExist=TRUE;
					break;
				}
			}

			if(!bAlreadyExist)
			{
				m_mapCustomizableMenus.SetAt(m_hMenu,pDocTemplate);
#ifndef _UNICODE
				VERIFY(LoadMenuState(m_hMenu,(pDocument==NULL ? NULL :
					pDocument->GetRuntimeClass()->m_lpszClassName)));
#else
				if(pDocument!=NULL)
				{
					LPCSTR lpszUniqueName=
						pDocument->GetRuntimeClass()->m_lpszClassName;
					int nLength=sizeof(lpszUniqueName)/sizeof(lpszUniqueName[0])+1;
					LPTSTR lpszUnicodeUniqueName=new TCHAR[nLength];
					memset(lpszUnicodeUniqueName,0,nLength*sizeof(TCHAR));
					_mbstowcsz(lpszUnicodeUniqueName,lpszUniqueName,nLength);
					VERIFY(LoadMenuState(m_hMenu,lpszUnicodeUniqueName));
					delete[] lpszUnicodeUniqueName;
				}
				else
				{
					VERIFY(LoadMenuState(m_hMenu,NULL));
				}
#endif
			}
		}
	}

	// repopulate menubar
	return RecreateMenuBar();
}

BOOL COXMenuBar::RecreateMenuBar(BOOL bRedraw/*=TRUE*/)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	// preserve old settings
	int nOldDraggedButton=GetDraggedButton();
	int nOldCustomizedButton=GetCustomizedButton();


	// delete existing buttons
	CToolBarCtrl& tool=GetToolBarCtrl();
	// don't redraw the menubar contents yet
	m_bNoInternalRedraw=TRUE;
	while(tool.DeleteButton(0));
	m_bNoInternalRedraw=FALSE;
	// clean up accelerator table
	m_accelTable.RemoveAll();


	// work out currently active MDIChild window icon (only for MDI applications)
	UpdateIcon(FALSE);

	BOOL bResult=TRUE;

	if(m_hMenu!=NULL)
	{

		// create one button for every menu item
		//

		CMenu menu;
		if(!menu.Attach(m_hMenu))
		{
			return FALSE;
		}

		// number of menu items 
		int nItems=menu.GetMenuItemCount();
		CString sMenuItem;
		TBBUTTON* pButtons=new TBBUTTON[nItems]; 

		// add buttons and set text to them
		for(int nIndex=0; nIndex<nItems; nIndex++)
		{
			// item name
			menu.GetMenuString(nIndex,sMenuItem,MF_BYPOSITION);
			// in AddStrings() parameter must ends with "\0\0"
			TCHAR* szMenuItemWithDoubleZero=new TCHAR[sMenuItem.GetLength()+2];
			::lstrcpyn(szMenuItemWithDoubleZero,sMenuItem,sMenuItem.GetLength()+1);
			szMenuItemWithDoubleZero[sMenuItem.GetLength()+1]=_T('\0');

			// work out accelerator
			int nPos=sMenuItem.Find(_T("&"));
			if(nPos!=-1)
			{
				CString sAccelSymbol=sMenuItem.Mid(nPos+1,1);
				sAccelSymbol.MakeUpper();
				m_accelTable.SetAt((TCHAR)*((LPCTSTR)sAccelSymbol),nIndex);
			}

			// setup button structure
			pButtons[nIndex].iBitmap=-1;
			pButtons[nIndex].idCommand=nIndex+ID_CMDBASE;
			pButtons[nIndex].fsState=TBSTATE_ENABLED;
			pButtons[nIndex].fsStyle=
				TBSTYLE_BUTTON|TBSTYLE_DROPDOWN|TBSTYLE_AUTOSIZE;
			pButtons[nIndex].dwData=nIndex;
			pButtons[nIndex].iString=tool.AddStrings(szMenuItemWithDoubleZero);

			delete[] szMenuItemWithDoubleZero;
		}

		bResult=tool.AddButtons(nItems,pButtons);
		delete[] pButtons;
		//////////////////////////////////////////////////////////////////////

		VERIFY(menu.Detach()!=NULL);
	}

	// reset old settings
	if(nOldDraggedButton>=0 && 
		nOldDraggedButton<GetToolBarCtrl().GetButtonCount())
	{
		SetDraggedButton(nOldDraggedButton);
	}
	if(nOldCustomizedButton>=0 && 
		nOldCustomizedButton<GetToolBarCtrl().GetButtonCount())
	{
		m_nCustomizedButtonIndex=nOldCustomizedButton;
	}


	if(!m_bIsMenuMetricInitialized)
	{
		UpdateMenuMetrics(FALSE);
		m_bIsMenuMetricInitialized=TRUE;
	}

	ASSERT(m_pFrameWnd!=NULL);
	if(m_hMenu==NULL)
	{
		if(m_pFrameWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
			((CFrameWnd*)m_pFrameWnd)->ShowControlBar(this,FALSE,FALSE);
		else
			ShowWindow(SW_HIDE);
	}
	else 
	{
		if(bRedraw)
		{
			// redraw the toolbar if specified
			RedrawToolBar(TRUE,TRUE);
			m_bDelayedButtonLayout=TRUE;
		}
		if(m_pFrameWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
		{
			((CFrameWnd*)m_pFrameWnd)->ShowControlBar(this,TRUE,FALSE);
		}
		else
		{
			CRect rect;
			m_pFrameWnd->GetClientRect(rect);
			CSize szToolbar=CalcFixedLayout(TRUE,TRUE);
			rect.top-=2;
			rect.bottom=rect.top+szToolbar.cy;
			MoveWindow(rect);
			ShowWindow(SW_SHOWNA);
		}
	}

	return bResult;
}

void COXMenuBar::UpdateIcon(BOOL bRedraw/*=TRUE*/)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	// refresh the icon
	HICON hIcon=NULL;
	if(m_pFrameWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
	{
	    BOOL bMaximize=FALSE;
		CMDIChildWnd* pChild=((CMDIFrameWnd*)m_pFrameWnd)->
			MDIGetActive(&bMaximize);
		if(pChild!=NULL && bMaximize)
		{
			hIcon=(HICON)(INT_PTR)::GetClassLongPtr(pChild->m_hWnd,GCL_HICONSM);
			if(hIcon==NULL)
				hIcon=(HICON)(INT_PTR)::GetClassLongPtr(pChild->m_hWnd,GCL_HICON);
			if(hIcon==NULL)
			{
				if(!::SendMessageTimeout(pChild->m_hWnd,
					WM_GETICON,ICON_SMALL,0,SMTO_ABORTIFHUNG,1000,
					(LPDWORD)&hIcon) || hIcon==NULL)
				{
					::SendMessageTimeout(pChild->m_hWnd,
						WM_GETICON,ICON_BIG,0,SMTO_ABORTIFHUNG,1000,
						(LPDWORD)&hIcon);
				}

				if(hIcon==NULL)
					hIcon=::LoadIcon(NULL,IDI_WINLOGO);
			}
		}
	}

	if(hIcon!=GetIcon())
		SetIcon(hIcon,bRedraw);
}

void COXMenuBar::DisplayPopupMenu(int nMenuItem)
{
	ASSERT(m_hMenu!=NULL);
	ASSERT((nMenuItem>=0 && nMenuItem<::GetMenuItemCount(m_hMenu)) ||
		(GetIcon()!=NULL && nMenuItem==::GetMenuItemCount(m_hMenu)));

	if (IsInAdvancedCustomizationMode())
		SaveCustomizedMenu();

	if(m_hMenu==NULL || nMenuItem<0 || nMenuItem>::GetMenuItemCount(m_hMenu) ||
		(GetIcon()==NULL && nMenuItem==::GetMenuItemCount(m_hMenu)) ||
		(::GetMenuItemID(m_hMenu,nMenuItem)!=-1 && 
		::GetMenuItemID(m_hMenu,nMenuItem)!=0xffff))
	{
		// make sure the state of previously active button is restored
		if (m_hMenu && IsInSurfingMode())
		{
			CMenu* pMenu=CMenu::FromHandle(m_hMenu);
			ASSERT(pMenu);
			CMenu* pSubMenu=pMenu->GetSubMenu(nMenuItem);
			ASSERT(pSubMenu==NULL);
			UINT nID=pMenu->GetMenuItemID(nMenuItem);
			CWnd* pMainWnd=AfxGetMainWnd();
			pMainWnd->SendMessage(WM_COMMAND,nID);
		}
		RestoreMenuButton();
		return;
	}

	BOOL bWasInDisplayingMode=IsInDisplayingMode();

	// reset the surfing mode
	BOOL bWasInSurfingMode=IsInSurfingMode();
	SetSurfingMode(FALSE);

	// if system menu should be displayed
	BOOL bSysMenu=(nMenuItem==::GetMenuItemCount(m_hMenu));

	CMenu* pPopup=NULL;
	if(bSysMenu)
	{
		if(IsInAdvancedCustomizationMode())
			return;

		// work out system menu
		ASSERT_KINDOF(CMDIFrameWnd,m_pFrameWnd);
	    BOOL bMaximize=FALSE;
		CMDIChildWnd* pChild=((CMDIFrameWnd*)m_pFrameWnd)->
			MDIGetActive(&bMaximize);
		ASSERT(pChild!=NULL && bMaximize);
		if(pChild==NULL || !bMaximize)
			return;

		pPopup=pChild->GetSystemMenu(FALSE);
	}
	else
	{
		// work out popup menu
		CMenu* pMenu=CMenu::FromHandle(m_hMenu);
		ASSERT(pMenu!=NULL);
		if(pMenu!=NULL && (pMenu->GetMenuState(nMenuItem,MF_BYPOSITION)&
			(MF_DISABLED|MF_GRAYED))==0)
		{
			pPopup=pMenu->GetSubMenu(nMenuItem);
		}
	}

	if(pPopup==NULL && !IsInAdvancedCustomizationMode()) 
	{
		// make sure the state of previously active button is restored
		RestoreMenuButton();
		return;
	}

	ASSERT(m_pFrameWnd!=NULL);

	if(!bWasInSurfingMode)
	{
		// send WM_ENTERMENULOOP and WM_INITMENU messages. 
		if(!bWasInDisplayingMode)
		{
			m_pFrameWnd->SendMessage(WM_ENTERMENULOOP,(WPARAM)FALSE);
			m_pFrameWnd->SendMessage(WM_INITMENU,(WPARAM)m_hMenu);
		}
		// send WM_MENUSELECT message
		m_pFrameWnd->SendMessage(WM_MENUSELECT,MAKEWPARAM((bSysMenu ? 0 : nMenuItem),
			((bSysMenu ? MF_SYSMENU : MF_POPUP|MF_HILITE))),(LPARAM)m_hMenu);
	}

	UINT nFlags=TPM_LEFTBUTTON;

	CRect rect;
	GetItemRect(nMenuItem,&rect);
	// translate the current toolbar item rectangle into screen coordinates
	// so that we'll know where to pop up the menu
	ClientToScreen(&rect);	

	// Attach the COXBitmapMenu object if not done already
	COXBitmapMenu* pBitmapMenu = DYNAMIC_DOWNCAST(COXBitmapMenu, pPopup);
	BOOL bConvert = FALSE;
	COXBitmapMenuOrganizer* pOrganizer = COXBitmapMenuOrganizer::FindOrganizer(AfxGetMainWnd()->GetSafeHwnd());
		
	if (pOrganizer != NULL)
	{
		if (pBitmapMenu == NULL)
		{
			pBitmapMenu	= new COXBitmapMenu();
			TRACE(_T("\nCreated menu at %x - Line: %d, %s"), pBitmapMenu, __LINE__, __FILE__);
			pBitmapMenu->Attach(pPopup->Detach());
			pPopup = (CMenu *) pBitmapMenu;
			pOrganizer->m_BitmapMenuList.AddTail(pBitmapMenu);	
			bConvert = TRUE;
		}
		
		pOrganizer->ConvertBitmapMenu(pBitmapMenu, bConvert);
	}

	

	// Determine top left point of the popup menu
	CPoint ptPos;
	UINT nDummy;
	COXBitmapMenu::DeterminePosition(pPopup, rect, m_dwStyle, ptPos, nFlags, nDummy);

	// make sure the state of previously active button is restored
	RestoreMenuButton();	

	// set current active item
	m_nActiveMenuItem=nMenuItem;
	m_nActivateNextItem=-1;

	// set button to pressed state
	CToolBarCtrl& tool=GetToolBarCtrl();
	if(!IsInAdvancedCustomizationMode())
	{
		tool.PressButton(m_nActiveMenuItem+ID_CMDBASE);
	}

	// display popup menu
	CWnd* pWndOwner=AfxGetMainWnd();
	if(pPopup!=NULL)
	{
		if(IsInAdvancedCustomizationMode())
		{
			OXCUSTOMIZEPOPUPMENUINFO cpmi;
			cpmi.hMenu=pPopup->GetSafeHmenu();
			cpmi.nFlags=nFlags;
			cpmi.ptStart = ptPos;
			SendCustomizeNotification(ID_OXCUSTMB_CUSTOMIZE_POPUP_MENU,(LPARAM)&cpmi);
			m_nForbiddenItem=-1;
			RestoreMenuButton();
		}
		else
		{
			if (pBitmapMenu != NULL)
				pBitmapMenu->TrackPopupMenu(this, pWndOwner);
			else
				pPopup->TrackPopupMenu(nFlags, ptPos.x, ptPos.y, pWndOwner, rect);

			// while deactivating popup menu the user could have set to
			// activate another menu item. In this case just post the 
			// corresponding message
			if(m_nActivateNextItem!=-1)
			{
				PostMessage(WM_DISPLAYPOPUPMENU,m_nActivateNextItem);
			}
			else
			{
				// Check whether we are still on the original button
				CPoint cursorPos;
				GetCursorPos(&cursorPos);
				ScreenToClient(&cursorPos);
#if _MFC_VER>0x0421
				int hitPos=tool.HitTest(&cursorPos);
#else
				int hitPos=tool.SendMessage(TB_HITTEST,0,(LPARAM)&cursorPos);
#endif
				if(hitPos==m_nActiveMenuItem)
				{
					// Don't allow it to drop down again
					m_nForbiddenItem=m_nActiveMenuItem; 
				}
				RestoreMenuButton();
			}
		}
	}
}

void COXMenuBar::RestoreMenuButton()
{
	// set button to unpressed state
	if(m_nActiveMenuItem!=-1)
	{
		CToolBarCtrl& tool=GetToolBarCtrl();
		tool.PressButton(m_nActiveMenuItem+ID_CMDBASE,FALSE);
		int nSavedActiveMenuItem=m_nActiveMenuItem;
		m_nActiveMenuItem=-1;
		if(m_nActivateNextItem==-1 && !m_bStaySurfing)
		{
			// send WM_EXITMENULOOP message
			m_pFrameWnd->SendMessage(WM_EXITMENULOOP,(WPARAM)FALSE);
		}
		if(m_bStaySurfing)
		{
			SetSurfingMode(TRUE,nSavedActiveMenuItem);
			// reset forbidden item
			m_nForbiddenItem=-1;
		}
		m_bStaySurfing=FALSE;
	}
}


int COXMenuBar::CalcButtonRects()
{
	m_rectCloseBtn.SetRectEmpty();
	m_rectRestoreBtn.SetRectEmpty();
	m_rectMinimizeBtn.SetRectEmpty();

	// total width of buttons
	int nButtonsWidth=0;

	if(GetIcon()!=NULL)
	{
		ASSERT(m_pFrameWnd!=NULL);
		ASSERT(::IsWindow(m_pFrameWnd->m_hWnd));
		ASSERT_KINDOF(CMDIFrameWnd,m_pFrameWnd);

	    BOOL bMaximize=FALSE;
		CMDIChildWnd* pChild=((CMDIFrameWnd*)m_pFrameWnd)->
			MDIGetActive(&bMaximize);
		if(pChild==NULL || !bMaximize)
			return nButtonsWidth;

		nButtonsWidth=2;

		DWORD dwStyle=pChild->GetStyle();

		// buttons metrics
		int cxIcon=18;
		int cyIcon=18;

		BOOL bHorz=(m_dwStyle&CBRS_ORIENT_HORZ); 

		CRect rectToDrawIn;
		GetWindowRect(rectToDrawIn);
		rectToDrawIn-=rectToDrawIn.TopLeft();

		// center the button images depending on orientation
		int nOffset=0;
		if(bHorz)
		{
			if(rectToDrawIn.Height()<cyIcon)
			{
				cyIcon=rectToDrawIn.Height();
				cxIcon=cyIcon;
			}
			else
				nOffset=(rectToDrawIn.Height()-cyIcon)/2+
					(rectToDrawIn.Height()-cyIcon)%2;
		}
		else
		{
			if(rectToDrawIn.Width()<cxIcon)
			{
				cxIcon=rectToDrawIn.Width();
				cyIcon=cxIcon;
			}
			else
				nOffset=(rectToDrawIn.Width()-cxIcon)/2+
					(rectToDrawIn.Width()-cxIcon)%2;
		}

		// flags that specify if corresponding buttons are displayed
		BOOL bCloseBox=TRUE;
		BOOL bMaxBox=bCloseBox & 
			((dwStyle&WS_MAXIMIZEBOX)!=0 || (dwStyle&WS_MINIMIZEBOX)!=0);

		// Calculate caption buttons. 
		CRect rect;
		if(bHorz)
			rect=CRect(rectToDrawIn.right-cxIcon-nButtonsWidth,nOffset, 
				rectToDrawIn.right-nButtonsWidth,nOffset+cyIcon);
		else
			rect=CRect(nOffset,rectToDrawIn.bottom-cxIcon-nButtonsWidth,
				nOffset+cxIcon,rectToDrawIn.bottom-nButtonsWidth);
	
		if(bCloseBox)
		{
			// Close box has a 2 pixel border on all sides but left/top, 
			// which is zero
			if(bHorz)
				rect.DeflateRect(0,2,2,2);
			else
				rect.DeflateRect(2,0,2,2);
			m_rectCloseBtn=rect;
			nButtonsWidth+=bHorz ? cxIcon : cyIcon;
		}

		// Max/restore button is like close box; just shift rectangle left
		if(bMaxBox) 
		{
			ASSERT(bCloseBox);

			rect-=bHorz ? CPoint(cxIcon,0) : CPoint(0,cyIcon);
			m_rectRestoreBtn=rect;
			nButtonsWidth+=bHorz ? cxIcon : cyIcon;
		}

		// Minimize button has 2 pixel border on all sides but right.
		if(bMaxBox) 
		{
			ASSERT(bCloseBox);

			rect-=bHorz ? CPoint(cxIcon-2,0) : CPoint(0,cyIcon-2);
			m_rectMinimizeBtn=rect;
			nButtonsWidth+=bHorz ? cxIcon : cyIcon;
		}

		nButtonsWidth+=ID_BUTTONSGAP;
	}

	return nButtonsWidth;
}


void COXMenuBar::DrawButtons(CDC& dc)
{
	GetToolbarSkin()->DrawMenuButtons(&dc, this);
}

int COXMenuBar::HitTest(const CPoint& ptTest, 
						HITTESTORIGIN Origin/*=OX_MNU_ZEROBASED*/) const
{
	CPoint point=ptTest;

	CRect rect;

	// update the test point depending on origin
	switch(Origin)
	{
	case OX_MNU_ZEROBASED:
		break;
	case OX_MNU_CLIENT:
		GetWindowRect(rect);
		ScreenToClient(rect);
		point-=rect.TopLeft();
		break;
	case OX_MNU_SCREEN:
		GetWindowRect(rect);
		point-=rect.TopLeft();
		break;
	}

	int hitTest;
	if(m_iconRect.PtInRect(point))
		hitTest=OX_MNU_ICON;
	else if(m_rectCloseBtn.PtInRect(point))
		hitTest=OX_MNU_CLOSEBTN;
	else if(m_rectRestoreBtn.PtInRect(point))
		hitTest=OX_MNU_RESTOREBTN;
	else if(m_rectMinimizeBtn.PtInRect(point))
		hitTest=OX_MNU_MINIMIZEBTN;
	else
	{
		// find corresponding menu item
		//

		hitTest=OX_MNU_NONE;

		CRect rectClient;
		GetClientRect(rectClient);
		if(rectClient.PtInRect(point))
		{
			GetWindowRect(rect);
			ScreenToClient(rect);
			point+=rect.TopLeft();

			CToolBarCtrl& tool=GetToolBarCtrl();
			for(int nIndex=0; nIndex<tool.GetButtonCount(); nIndex++)
			{
				tool.GetItemRect(nIndex,rect);
				if(rect.PtInRect(point))
				{
					hitTest=nIndex;
					break;
				}
			}
		}
	}

	return hitTest;
}


BOOL COXMenuBar::HandleMenuKey(UINT nChar)
{
	// handle keyboard messages
	//

	if(AfxGetMainWnd()->SendMessage(WM_KEYDOWN,nChar,1))
		return TRUE;
	
	ASSERT(::IsWindow(m_hWnd));

	BOOL bHandled=FALSE;

	if(nChar==0)
	{
		// ALT key was pressed
		ASSERT(!IsInDisplayingMode());
		int nHotItem=PtrToInt(SendMessage(TB_GETHOTITEM));
		nHotItem=nHotItem==-1 ? 0 : nHotItem;
		// set the menu  bar in surfing mode when user presses ALT key
		SetSurfingMode(!IsInSurfingMode(),nHotItem);
		if(!IsInSurfingMode())
		{
			// send WM_EXITMENULOOP message
			m_pFrameWnd->SendMessage(WM_EXITMENULOOP,(WPARAM)FALSE);
		}
		bHandled=TRUE;
	}
	else if(nChar==VK_ESCAPE && IsInSurfingMode())
	{
		// Esc key was pressed in surfing mode
		int nHotItem=PtrToInt(SendMessage(TB_GETHOTITEM));
		nHotItem=nHotItem==-1 ? 0 : nHotItem;
		SetSurfingMode(FALSE,nHotItem);
		// send WM_EXITMENULOOP message
		m_pFrameWnd->SendMessage(WM_EXITMENULOOP,(WPARAM)FALSE);
		bHandled=TRUE;
	}
	else
	{
		int nItemCount=GetToolBarCtrl().GetButtonCount();
		if(nItemCount==0)
			return FALSE;

		// handle keys  depending on their values, menu bar
		// orientation sand the mode it is currently in
		switch(nChar)
		{
		case VK_LEFT:
			if(m_dwStyle & CBRS_ORIENT_HORZ)
			{
				if(IsInSurfingMode())
				{
					// in surfing mode just navigate through menu items
					int nItem=PtrToInt(SendMessage(TB_GETHOTITEM));
					// retrieve the item that preceedes the current hot one
					nItem=GetPrevMenuItem(nItem);
					// set it to hot state
					SendMessage(TB_SETHOTITEM,
						(nItem>=0 && nItem<nItemCount) ? nItem : -1);

					// if system menu should be displayed
					BOOL bSysMenu=(nItem==::GetMenuItemCount(m_hMenu));
					// send WM_MENUSELECT message
					m_pFrameWnd->SendMessage(WM_MENUSELECT,
						MAKEWPARAM((bSysMenu ? 0 : nItem),
						((bSysMenu ? MF_SYSMENU : MF_POPUP|MF_HILITE))),
						(LPARAM)m_hMenu);

					bHandled=TRUE;
				}
				else if(IsInDisplayingMode())
				{
					// in displaying mode we activate the preceeding menu item
					int nItem=GetPrevMenuItem(m_nActiveMenuItem);
					if(nItem!=m_nActiveMenuItem)
					{
						if((::GetMenuItemID(m_hMenu,nItem)==-1 || 
							::GetMenuItemID(m_hMenu,nItem)==0xffff))
						{
							m_nActivateNextItem=nItem;

							// emulate mouse click 
							//
							CRect rect;
							if(GetIcon()!=NULL && nItem==nItemCount)
							{
								GetToolBarCtrl().GetItemRect(0,rect);
								rect.DeflateRect(1,1);
								rect.OffsetRect(-::GetSystemMetrics(SM_CXSMICON),0);
							}
							else
							{
								GetToolBarCtrl().GetItemRect(nItem,rect);
							}
							CPoint point(rect.left+rect.Width()/2,
								rect.top+rect.Height()/2);
							PostMessage(WM_LBUTTONDOWN,MK_LBUTTON,
								MAKELPARAM(point.x,point.y));
							PostMessage(WM_LBUTTONUP,MK_LBUTTON,
								MAKELPARAM(point.x,point.y));
							//////////////////////////////
						}
						bHandled=TRUE;
					}
				}
			}
			else
			{
				if(IsInSurfingMode())
				{
					// in surfing mode activate the currently hot item
					int nItem=PtrToInt(SendMessage(TB_GETHOTITEM));
					nItem=nItem==-1 ? nItemCount : nItem;
					DisplayPopupMenu(nItem);
					bHandled=TRUE;
				}
			}
			break;
		
		case VK_RIGHT:
			if(m_dwStyle & CBRS_ORIENT_HORZ)
			{
				if(IsInSurfingMode())
				{
					// in surfing mode just navigate through menu items
					int nItem=PtrToInt(SendMessage(TB_GETHOTITEM));
					// retrieve the item that followss the current hot one
					nItem=GetNextMenuItem(nItem);
					// set it to hot state
					SendMessage(TB_SETHOTITEM,
						(nItem>=0 && nItem<nItemCount) ? nItem : -1);

					// if system menu should be displayed
					BOOL bSysMenu=(nItem==::GetMenuItemCount(m_hMenu));
					// send WM_MENUSELECT message
					m_pFrameWnd->SendMessage(WM_MENUSELECT,
						MAKEWPARAM((bSysMenu ? 0 : nItem),
						((bSysMenu ? MF_SYSMENU : MF_POPUP|MF_HILITE))),
						(LPARAM)m_hMenu);

					bHandled=TRUE;
				}
				else if(IsInDisplayingMode())
				{
					// in displaying mode we activate the following menu item
					int nItem=GetNextMenuItem(m_nActiveMenuItem);
					if(nItem!=m_nActiveMenuItem)
					{
						if((::GetMenuItemID(m_hMenu,nItem)==-1 || 
							::GetMenuItemID(m_hMenu,nItem)==0xffff))
						{
							m_nActivateNextItem=nItem;

							// emulate mouse click 
							//
							CRect rect;
							if(GetIcon()!=NULL && nItem==nItemCount)
							{
								GetToolBarCtrl().GetItemRect(0,rect);
								rect.DeflateRect(1,1);
								rect.OffsetRect(-::GetSystemMetrics(SM_CXSMICON),0);
							}
							else
							{
								GetToolBarCtrl().GetItemRect(nItem,rect);
							}
							CPoint point(rect.left+rect.Width()/2,
								rect.top+rect.Height()/2);
							PostMessage(WM_LBUTTONDOWN,MK_LBUTTON,
								MAKELPARAM(point.x,point.y));
							PostMessage(WM_LBUTTONUP,MK_LBUTTON,
								MAKELPARAM(point.x,point.y));
							////////////////////////////
						}
						bHandled=TRUE;
					}
				}
			}
			else
			{
				if(IsInSurfingMode())
				{
					// in surfing mode activate the currently hot item
					int nItem=PtrToInt(SendMessage(TB_GETHOTITEM));
					nItem=nItem==-1 ? nItemCount : nItem;
					DisplayPopupMenu(nItem);
					bHandled=TRUE;
				}
			}
			break;

		case VK_UP:
			if(m_dwStyle & CBRS_ORIENT_VERT)
			{
				if(IsInSurfingMode())
				{
					// in surfing mode just navigate through menu items
					int nItem=PtrToInt(SendMessage(TB_GETHOTITEM));
					nItem=GetPrevMenuItem(nItem);
					SendMessage(TB_SETHOTITEM,
						(nItem>=0 && nItem<nItemCount) ? nItem : -1);

					// if system menu should be displayed
					BOOL bSysMenu=(nItem==::GetMenuItemCount(m_hMenu));
					// send WM_MENUSELECT message
					m_pFrameWnd->SendMessage(WM_MENUSELECT,
						MAKEWPARAM((bSysMenu ? 0 : nItem),
						((bSysMenu ? MF_SYSMENU : MF_POPUP|MF_HILITE))),
						(LPARAM)m_hMenu);

					bHandled=TRUE;
				}
			}
			else
			{
				if(IsInSurfingMode())
				{
					// in surfing mode activate the currently hot item
					int nItem=PtrToInt(SendMessage(TB_GETHOTITEM));
					nItem=nItem==-1 ? nItemCount : nItem;
					DisplayPopupMenu(nItem);
					bHandled=TRUE;
				}
			}
			break;

		case VK_DOWN:
			if(m_dwStyle & CBRS_ORIENT_VERT)
			{
				if(IsInSurfingMode())
				{
					// in surfing mode just navigate through menu items
					int nItem=PtrToInt(SendMessage(TB_GETHOTITEM));
					nItem=GetNextMenuItem(nItem);
					SendMessage(TB_SETHOTITEM,
						(nItem>=0 && nItem<nItemCount) ? nItem : -1);
					bHandled=TRUE;
				}
			}
			else
			{
				if(IsInSurfingMode())
				{
					// in surfing mode activate the currently hot item
					int nItem=PtrToInt(SendMessage(TB_GETHOTITEM));
					nItem=nItem==-1 ? nItemCount : nItem;
					DisplayPopupMenu(nItem);
					bHandled=TRUE;
				}
			}
			break;

		case VK_RETURN:
			if(IsInSurfingMode())
			{
				// in surfing mode activate the currently hot item
					int nItem=PtrToInt(SendMessage(TB_GETHOTITEM));
				nItem=nItem==-1 ? nItemCount : nItem;
				DisplayPopupMenu(nItem);
				bHandled=TRUE;
			}
			break;
		
		case VK_ESCAPE:
			if(IsInDisplayingMode())
			{
				// set the flag that specifies that menu bar should state 
				// in surfing mode after Esc key was pressed when
				// top level popup menu was active
				m_bStaySurfing=TRUE;

				bHandled=FALSE;
			}
			break;
		
		default:
			if(!IsInDisplayingMode() && ::GetAsyncKeyState(VK_CONTROL)>=0)
			{
				// try to find the matching key in accelerator table
				int nMenuItem=-1;
				TCHAR sHotKey=(TCHAR)nChar;


				// the key strokes passed in to the OnKeyDown handlers are always as
                // capital letters, there is no need to convert characters to capitals again
                // this will only cause problems with extended keyboard keys (ie num-pad)
                //_tcsupr(&sHotKey);

				if(m_accelTable.Lookup(sHotKey,nMenuItem))
				{
					ASSERT(nMenuItem>=0 && 
						nMenuItem<GetToolBarCtrl().GetButtonCount());
					// if found, activate it
					DisplayPopupMenu(nMenuItem);
					bHandled=TRUE;
				}
			}
		}
	}

	return bHandled;
}


int COXMenuBar::GetPrevMenuItem(int nItem, 
								BOOL bEnsureVisible/*=TRUE*/)
{
	int nItemCount=GetToolBarCtrl().GetButtonCount();

	nItem=nItem==-1 ? ((GetIcon()!=NULL) ? nItemCount : 0) : nItem;
	if(GetIcon()!=NULL)
		nItem=nItem==0 ? nItemCount : nItem-1;
	else
		nItem=nItem==0 ? nItemCount-1 : nItem-1;

	if(bEnsureVisible && nItem!=nItemCount)
	{
		// make sure the button rectangle is within menubar client area
		//
		CRect rectClient;
		GetClientRect(rectClient);
		CRect rectItem;
		GetItemRect(nItem, rectItem);
		if(rectItem.left>rectClient.right)
			nItem=GetIcon()!=NULL ? nItemCount : 0;
	}

	return nItem;
}

int COXMenuBar::GetNextMenuItem(int nItem, 
								BOOL bEnsureVisible/*=TRUE*/)
{
	int nItemCount=GetToolBarCtrl().GetButtonCount();

	nItem=nItem==-1 ? ((GetIcon()!=NULL) ? nItemCount : 0) : nItem;
	if(GetIcon()!=NULL)
		nItem=nItem==nItemCount ? 0 : nItem+1;
	else
		nItem=nItem==nItemCount-1 ? 0 : nItem+1;

	if(bEnsureVisible && nItem!=nItemCount)
	{
		// make sure the button rectangle is within menubar client area
		//
		CRect rectClient;
		GetClientRect(rectClient);
		CRect rectItem;
		GetItemRect(nItem,rectItem);
		if(rectItem.left>rectClient.right)
			nItem=GetIcon()!=NULL ? nItemCount : 0;
	}

	return nItem;
}


void COXMenuBar::UpdateMenuMetrics(BOOL bRedraw/*=TRUE*/)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	if((HFONT)m_fontMenu!=NULL)
		m_fontMenu.DeleteObject();
	// Menu font, height and color
	NONCLIENTMETRICS ncm={ sizeof(ncm) };
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,sizeof(ncm),&ncm,0);
	VERIFY(m_fontMenu.CreateFontIndirect(&ncm.lfMenuFont));
	SetFont(&m_fontMenu,TRUE);

	if(GetToolBarCtrl().GetButtonCount()>0)
	{
		GetToolBarCtrl().AutoSize();
		CRect rect;
		GetToolBarCtrl().GetItemRect(0,&rect);
#if _MFC_VER<=0x0421
		SetHeight(rect.Height()+6);
#else
		SetHeight(rect.Height()+3);
#endif
		GetToolBarCtrl().SetButtonSize(rect.Size());
	}

	SetDefaultTextColor(::GetSysColor(COLOR_MENUTEXT));

	if(bRedraw)
	{
		// redraw the toolbar if specified
		RedrawToolBar();
	}
}


BOOL COXMenuBar::SaveBarState(LPCTSTR lpszSubKey, LPCTSTR lpszValueName, 
							  BOOL bProperties)
{
	UNREFERENCED_PARAMETER(lpszSubKey);
	UNREFERENCED_PARAMETER(lpszValueName);
	UNREFERENCED_PARAMETER(bProperties);

#ifndef _MAC
	if(IsCustomizable(TRUE))
	{
		POSITION pos=m_mapChangedMenus.GetStartPosition();
		while(pos!=NULL)
		{
			HMENU hMenu=NULL;
			int nIndex=-1;
			m_mapChangedMenus.GetNextAssoc(pos,hMenu,nIndex);
			ASSERT(hMenu!=NULL);
			CDocTemplate* pDocTemplate=NULL;
			VERIFY(m_mapCustomizableMenus.Lookup(hMenu,pDocTemplate));
			CDocument* pDocument=NULL;
			if(pDocTemplate!=NULL)
			{
				POSITION pos=pDocTemplate->GetFirstDocPosition();
				ASSERT(pos!=NULL);
				pDocument=pDocTemplate->GetNextDoc(pos);
				ASSERT(pDocument!=NULL);
			}
#ifndef _UNICODE
			VERIFY(SaveMenuState(hMenu,(pDocument==NULL ? NULL :
				pDocument->GetRuntimeClass()->m_lpszClassName)));
#else
			if(pDocument!=NULL)
			{
				LPCSTR lpszUniqueName=pDocument->GetRuntimeClass()->m_lpszClassName;
				int nLength=sizeof(lpszUniqueName)/sizeof(lpszUniqueName[0])+1;
				LPTSTR lpszUnicodeUniqueName=new TCHAR[nLength];
				memset(lpszUnicodeUniqueName,0,nLength*sizeof(TCHAR));
				_mbstowcsz(lpszUnicodeUniqueName,lpszUniqueName,nLength);
				VERIFY(SaveMenuState(hMenu,lpszUnicodeUniqueName));
				delete[] lpszUnicodeUniqueName;
			}
			else
			{
				VERIFY(SaveMenuState(hMenu,NULL));
			}
#endif
			m_mapChangedMenus.RemoveKey(hMenu);
		}
	}
	return TRUE;
#else
	return FALSE;
#endif
}

// Load from registry state of menus
BOOL COXMenuBar::LoadBarState(LPCTSTR lpszSubKey, LPCTSTR lpszValueName, 
							  BOOL bProperties)
{
	UNREFERENCED_PARAMETER(lpszSubKey);
	UNREFERENCED_PARAMETER(lpszValueName);
	UNREFERENCED_PARAMETER(bProperties);

#ifndef _MAC
	return TRUE;
#else
	return FALSE;
#endif
}


BOOL COXMenuBar::SaveMenuState(HMENU hMenu, LPCTSTR lpszSubKey)
{
#ifndef _MAC
	ASSERT(hMenu!=NULL);
	ASSERT(::IsMenu(hMenu)!=NULL);

	if(!IsCustomizable(TRUE))
		return FALSE;

	CWinApp* pApp=AfxGetApp();
	CString sProfileName=lpszSubKey;
	if(sProfileName.IsEmpty())
		sProfileName=_T("Menu_Default~!@#$#$%^");
	else
		sProfileName=_T("Menu_")+sProfileName;
	CString sValueName=_T("MenuItems");
	
	CMemFile memFile;
	CArchive ar(&memFile,CArchive::store);
	VERIFY(SaveMenuContents(hMenu,ar));
	ar.Close();
	int nBufferLength=(int)memFile.GetLength();
	ASSERT(nBufferLength>0);
	BYTE* pBuffer=memFile.Detach();

	pApp->WriteProfileBinary(sProfileName,sValueName,pBuffer,nBufferLength);
	::free((void*)pBuffer);

	sValueName=_T("MenuDataSize");
	pApp->WriteProfileInt(sProfileName,sValueName,nBufferLength);

	if(m_pFrameWnd!=NULL)
	{
		// notify parent that the state of menu is being saved. Parent window 
		// might save any additional info
		m_pFrameWnd->SendMessage(WM_OX_SAVEMENUSTATE,(WPARAM)hMenu,
			(LPARAM)(LPCTSTR)sProfileName);
	}

	return TRUE;
#else
	return FALSE;
#endif
}


BOOL COXMenuBar::LoadMenuState(HMENU hMenu, LPCTSTR lpszSubKey)
{
#ifndef _MAC
	ASSERT(hMenu!=NULL);
	ASSERT(::IsMenu(hMenu)!=NULL);

	if(!IsCustomizable(TRUE))
		return FALSE;

	CWinApp* pApp=AfxGetApp();
	CString sProfileName=lpszSubKey;
	if(sProfileName.IsEmpty())
		sProfileName=_T("Menu_Default~!@#$#$%^");
	else
		sProfileName=_T("Menu_")+sProfileName;
	CString sValueName=_T("MenuDataSize");
	int nSavedBufferLength=
		pApp->GetProfileInt(sProfileName,sValueName,-1);
	sValueName=_T("MenuItems");
	if(nSavedBufferLength!=-1)
	{
		ASSERT(nSavedBufferLength>0);
		UINT nBufferLength=0;
		BYTE* pBuffer=NULL;
		if(pApp->GetProfileBinary(sProfileName,sValueName,&pBuffer,&nBufferLength))
		{
			m_hMDIWindowMenu=NULL;

			ASSERT(nBufferLength>0);
			ASSERT(pBuffer!=NULL);
			CMemFile memFile(pBuffer,nBufferLength);
			CArchive ar(&memFile,CArchive::load);
			VERIFY(LoadMenuContents(hMenu,ar));
			ar.Close();
			memFile.Detach();
			delete[] pBuffer;
		}
	}

	if(m_pFrameWnd!=NULL)
	{
		// notify parent that the state of menu is being restored from registry. 
		// Parent window might apply any saved info
		m_pFrameWnd->SendMessage(WM_OX_LOADMENUSTATE,(WPARAM)hMenu,
			(LPARAM)(LPCTSTR)sProfileName);
	}

	return TRUE;
#else
	return FALSE;
#endif
}


BOOL COXMenuBar::SaveMenuContents(HMENU hMenu, CArchive& ar)
{
	ASSERT(hMenu!=NULL);
	ASSERT(::IsMenu(hMenu)!=NULL);

	int nItemCount=::GetMenuItemCount(hMenu);
	ar<<nItemCount;
	for(int nIndex=0; nIndex<nItemCount; nIndex++)
	{
		MENUITEMINFO menuItemInfo;
		::memset(&menuItemInfo, 0, sizeof(MENUITEMINFO));
		menuItemInfo.cbSize = sizeof(MENUITEMINFO);
		menuItemInfo.fMask=MIIM_ID|MIIM_SUBMENU|MIIM_TYPE;
		menuItemInfo.cch=1024;
		TCHAR tszBuffer[1024];
		menuItemInfo.dwTypeData = tszBuffer;
		VERIFY(::GetMenuItemInfo(hMenu,nIndex,TRUE,&menuItemInfo));

		CString strText(tszBuffer, menuItemInfo.cch);
		
		ASSERT(!strText.IsEmpty() || (menuItemInfo.fType&MFT_SEPARATOR)!=0);
		ar<<menuItemInfo.fType;
		ar<<(BOOL)(menuItemInfo.hSubMenu!=NULL);
		ar<<(menuItemInfo.hSubMenu!=NULL ? (UINT)-1 : menuItemInfo.wID);
		ar<<strText;

		if (menuItemInfo.hSubMenu != NULL)
		{
			VERIFY(SaveMenuContents(menuItemInfo.hSubMenu,ar));
		}
	}
	return TRUE;
}


BOOL COXMenuBar::LoadMenuContents(HMENU hMenu, CArchive& ar)
{
	ASSERT(hMenu!=NULL);
	ASSERT(::IsMenu(hMenu)!=NULL);

	// remove all items from menu
	while(::DeleteMenu(hMenu,0,MF_BYPOSITION));

	DWORD dwItemCount=0;
	ar>>dwItemCount;
	for(int nIndex=0; nIndex<(int)dwItemCount; nIndex++)
	{
		MENUITEMINFO menuItemInfo={ sizeof(MENUITEMINFO) };
		menuItemInfo.fMask=MIIM_ID|MIIM_SUBMENU|MIIM_TYPE;
		ar>>menuItemInfo.fType;
		BOOL bPopupMenu=FALSE;
		ar>>bPopupMenu;
		ar>>menuItemInfo.wID;

		CString sText;
		ar>>sText;
		menuItemInfo.hSubMenu=NULL;
		if(bPopupMenu)
		{
			menuItemInfo.wID=(UINT)-1;
			// create new popup menu
			menuItemInfo.hSubMenu=::CreatePopupMenu();
			ASSERT(menuItemInfo.hSubMenu!=NULL);
			m_arrCreatedPopupMenus.Add(menuItemInfo.hSubMenu);
			VERIFY(LoadMenuContents(menuItemInfo.hSubMenu,ar));
		}

		// if this item is first MDIChild then we have to remove the separator 
		// that we put before
		if(menuItemInfo.wID==AFX_IDM_FIRST_MDICHILD)
		{
			CWnd* pWnd=AfxGetMainWnd();
			CMDIFrameWnd* pMDIFrameWnd=DYNAMIC_DOWNCAST(CMDIFrameWnd,pWnd);
			if(pMDIFrameWnd!=NULL)
			{
				// check if the previous item is separator
				MENUITEMINFO menuItemInfo={ sizeof(MENUITEMINFO) };
				menuItemInfo.fMask=MIIM_TYPE;
				VERIFY(::GetMenuItemInfo(hMenu,::GetMenuItemCount(hMenu)-1,
					TRUE,&menuItemInfo));
				if((menuItemInfo.fType&MFT_SEPARATOR)!=0)
				{
					VERIFY(::DeleteMenu(hMenu,::GetMenuItemCount(hMenu)-1,
						MF_BYPOSITION));
				}

				m_hMDIWindowMenu=hMenu;
			}
		}

		// don't add MDIChild windows menu items 
		if(bPopupMenu || menuItemInfo.wID<AFX_IDM_FIRST_MDICHILD)
		{
			menuItemInfo.cch=sText.GetLength();
			menuItemInfo.dwTypeData=(LPTSTR)sText.GetBuffer(menuItemInfo.cch);
			VERIFY(::InsertMenuItem(hMenu,::GetMenuItemCount(hMenu),TRUE,
				&menuItemInfo));
			sText.ReleaseBuffer();
		}
	}

	return TRUE;
}


BOOL COXMenuBar::DisplayCustomizeButtonContextMenu(int nButtonIndex, CPoint point)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(nButtonIndex>=0 && nButtonIndex<GetToolBarCtrl().GetButtonCount());
	UNUSED(nButtonIndex);

	CMenu menu;
	if(!menu.CreatePopupMenu())
		return FALSE;
	// populate menu
	CString sItem;
	sItem.LoadString(IDS_OX_CUSTTB_DELETE);
	VERIFY(menu.AppendMenu(MF_STRING,ID_OXCUSTTB_DELETE,sItem));
	VERIFY(menu.AppendMenu(MF_SEPARATOR));
	sItem.LoadString(IDS_OX_CUSTTB_APPEARANCE);
	VERIFY(menu.AppendMenu(MF_STRING,ID_OXCUSTTB_APPEARANCE,sItem));

	menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
		point.x,point.y,this);

	return TRUE;
}


COXMenuBar* COXMenuBar::FindMenuBar(CWnd* pStartWnd/*=NULL*/)
{
	if(pStartWnd==NULL)
		pStartWnd=AfxGetMainWnd();
	if(pStartWnd==NULL)
		return NULL;

	if(pStartWnd->IsKindOf(RUNTIME_CLASS(COXMenuBar)))
		return DYNAMIC_DOWNCAST(COXMenuBar,pStartWnd);

	CWnd* pWnd=pStartWnd->GetWindow(GW_CHILD);
	while(pWnd!=NULL)
	{
		COXMenuBar* pMenuBar=COXMenuBar::FindMenuBar(pWnd);
		if(pMenuBar!=NULL)
			return pMenuBar;

		pWnd=pWnd->GetWindow(GW_HWNDNEXT);
	}

	return NULL;
}

BOOL COXMenuBar::SaveCustomizedMenu()
{
	if(!IsCustomizable(TRUE))
		return TRUE;

	BOOL bRes = TRUE;
	int nIndex=-1;
	if(m_mapChangedMenus.Lookup(m_hMenu,nIndex))
	{
		CDocTemplate* pDocTemplate=NULL;
		VERIFY(m_mapCustomizableMenus.Lookup(m_hMenu,pDocTemplate));
		CDocument* pDocument=NULL;
		if(pDocTemplate!=NULL)
		{
			POSITION pos=pDocTemplate->GetFirstDocPosition();
			ASSERT(pos!=NULL);
			pDocument=pDocTemplate->GetNextDoc(pos);
			ASSERT(pDocument!=NULL);
		}
#ifndef _UNICODE
		bRes = SaveMenuState(m_hMenu,(pDocument==NULL ? NULL :
			pDocument->GetRuntimeClass()->m_lpszClassName));
#else
		if(pDocument!=NULL)
		{
			LPCSTR lpszUniqueName=pDocument->GetRuntimeClass()->m_lpszClassName;
			int nLength=sizeof(lpszUniqueName)/sizeof(lpszUniqueName[0])+1;
			LPTSTR lpszUnicodeUniqueName=new TCHAR[nLength];
			memset(lpszUnicodeUniqueName,0,nLength*sizeof(TCHAR));
			_mbstowcsz(lpszUnicodeUniqueName,lpszUniqueName,nLength);
			bRes = SaveMenuState(m_hMenu,lpszUnicodeUniqueName);
			delete[] lpszUnicodeUniqueName;
		}
		else
		{
			VERIFY(SaveMenuState(m_hMenu,NULL));
		}
#endif
		m_mapChangedMenus.RemoveKey(m_hMenu);
	}

	return bRes;
}

///////////////////////////////////////////////////////////////////////////


#endif	//	_MFC_VER>=0x0420

CString COXMenuBar::GetItemText(int iIndex)
{
	CMenu menu;
	menu.Attach(m_hMenu);

	CString strText;
	menu.GetMenuString(iIndex, strText, MF_BYPOSITION);
	menu.Detach();
	return strText;
}

int COXMenuBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COXCoolToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Hook the mouse
	if (m_hMouseHook == NULL)
		m_hMouseHook = ::SetWindowsHookEx(WH_MOUSE, MouseProc, 0, AfxGetApp()->m_nThreadID);

	return 0;
}

void COXMenuBar::OnDestroy() 
{
	// Unhook the mouse
	if (m_hMouseHook)
	{
		::UnhookWindowsHookEx(m_hMouseHook);
		m_hMouseHook = NULL;
	}

	COXCoolToolBar::OnDestroy();
}

// Update the buttons of the mini frame window when the mouse leaves
LRESULT CALLBACK COXMenuBar::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return ::CallNextHookEx(m_hMouseHook, nCode, wParam, lParam);

	if (nCode == HC_ACTION && (wParam == WM_MOUSEMOVE || wParam == WM_NCMOUSEMOVE))
	{

		MOUSEHOOKSTRUCT* pMH = (MOUSEHOOKSTRUCT*) lParam;

		// If the previous message was for COXSizableMiniDockFrameWnd and the current is not
		// we need to update the caption buttons
		COXMenuBar* pPrev = DYNAMIC_DOWNCAST(COXMenuBar,
			CWnd::FromHandlePermanent(m_hwndPrevMouseMoveWnd));
		COXMenuBar* pCurrent = DYNAMIC_DOWNCAST(COXMenuBar,
			CWnd::FromHandlePermanent(pMH->hwnd));

		if (pPrev != NULL && pCurrent != pPrev)
		{
			if (pPrev->m_pFrameWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
			{
				// The mouse just left the mini frame window
				CWindowDC dc(pPrev);
				pPrev->GetToolbarSkin()->DrawMenuButtons(&dc, pPrev);
			}
		}
		m_hwndPrevMouseMoveWnd = pMH->hwnd;
	}

	return CallNextHookEx(m_hMouseHook, nCode, wParam, lParam);
}

void COXMenuBar::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	CWindowDC dc(this);
	GetToolbarSkin()->DrawMenuButtons(&dc, this);
	
	COXCoolToolBar::OnNcMouseMove(nHitTest, point);
}

COXMenuSkin* COXMenuBar::GetMenuSkin()
{
	// Check if the app is derived from COXSkinnedApp
	COXSkinnedApp* pSkinnedApp = DYNAMIC_DOWNCAST(COXSkinnedApp, AfxGetApp());
	if (pSkinnedApp != NULL && pSkinnedApp->GetCurrentSkin() != NULL)
		return pSkinnedApp->GetCurrentSkin()->GetMenuSkin();
	else
	{
		// Create a classic skin for this class if not created already
		if (m_pMenuSkin == NULL)
			m_pMenuSkin = new COXMenuSkinClassic();

		return m_pMenuSkin;
	}
}

void COXMenuBar::RecalculateItemWidths()
{
	// Go though all the buttons and determine their widths
	CToolBarCtrl& ctrl = GetToolBarCtrl();
	int iCount = ctrl.GetButtonCount();
	int iMaxTextLength = 0;
	int i = 0;
	for (i = 0; i < iCount; i++)
	{
		CString strText = GetItemText(i);
		if (strText.IsEmpty())
			continue;

		// Determine the length of the text
		CDC* pDC = GetDC();
		CRect rectText(0, 0, 0, 0);
		CFont* pOldFont = pDC->SelectObject(GetFont());
		pDC->DrawText(strText, rectText, DT_CALCRECT);
		pDC->SelectObject(pOldFont);
		ReleaseDC(pDC);

		if (rectText.right + 8 > iMaxTextLength)
			iMaxTextLength = rectText.right + 8;

		if (m_dwStyle & CBRS_ORIENT_HORZ)
		{
			// Horizontal
			TBBUTTONINFO tbi;
			tbi.cbSize = sizeof(TBBUTTONINFO);
			tbi.dwMask = TBIF_SIZE;
			tbi.cx = (BYTE) (rectText.right + GetMenuSkin()->GetDimentionConstants().m_nGapMenuItems);

			ctrl.SetButtonInfo(GetItemID(i), &tbi);
		}
	}

	// If we have a vertical menubar make the items as wide as the widest item
	if ((m_dwStyle & CBRS_ORIENT_HORZ) == 0)
	{
		for (i = 0; i < iCount; i++)
		{
			TBBUTTONINFO tbi;
			tbi.cbSize = sizeof(TBBUTTONINFO);
			tbi.dwMask = TBIF_SIZE;
			tbi.cx = (WORD) iMaxTextLength;

			ctrl.SetButtonInfo(GetItemID(i), &tbi);
		}
	}
}

void COXShadowedItemWnd::OnPaint() 
{
	COXBitmapMenu::GetMenuSkin()->OnPaintShadowedItemWnd(this);
}

void COXMenuBar::GetItemRect(int nIndex, LPRECT lpRect)
{
	// Handle the system menu case, otherwise call the default implementation
	bool bSysMenu = (nIndex == (int) ::GetMenuItemCount(m_hMenu));
	if (bSysMenu)
	{
		CRect rectWindow;
		GetWindowRect(rectWindow);
		ScreenToClient(&rectWindow);

		CRect rectClient;
		GetClientRect(rectClient);
		
		lpRect->left = m_iconRect.left;
		lpRect->top = m_iconRect.top;
		lpRect->right = m_iconRect.right;
		lpRect->bottom = m_iconRect.bottom;

		// Offset the rectangle
		lpRect->left += rectWindow.left - rectClient.left;
		lpRect->right += rectWindow.left - rectClient.left;
		lpRect->top += rectWindow.top - rectClient.top;
		lpRect->bottom += rectWindow.top - rectClient.top;
	}
	else
		COXCoolToolBar::GetItemRect(nIndex, lpRect);
}

CSize COXMenuBar::CalcSize(TBBUTTON *pData, int nCount)
{
	CSize size = COXCoolToolBar::CalcSize(pData, nCount);

	// If the menu bar if horizontally docked make it as wide as the frame, so that it would
	// wrap and become multiline
	if (!IsFloating())
	{
		CFrameWnd* pDockingFrame = GetDockingFrame();
		CRect rectClient;
		pDockingFrame->GetClientRect(rectClient);
		
		int iFrameWidth;
		if (m_rectCloseBtn.IsRectEmpty())
			iFrameWidth = rectClient.right;
		else
			iFrameWidth = rectClient.right - m_rectCloseBtn.Width() - m_rectMinimizeBtn.Width()
				- m_rectRestoreBtn.Width() - m_iconRect.Width() - 14;

		if (iFrameWidth < size.cx)
			size.cx = iFrameWidth;
	}

	return size;
}

void COXMenuBar::OnSize(UINT nType, int cx, int cy) 
{
	COXCoolToolBar::OnSize(nType, cx, cy);
	
	if (!IsFloating() && IsWindowVisible())
	{
		m_bDelayedButtonLayout = TRUE;
		Layout();
	}
}

