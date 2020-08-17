// OXCoolToolBar.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include <stdlib.h>

#include "OXCoolToolBar.h"

#include "UTBStrOp.h"

#ifdef OX_CUSTOMIZE_COMMANDS
#include "OXDragDropCommands.h"
#endif	//	OX_CUSTOMIZE_COMMANDS

#if _MFC_VER>0x0421
#include <afxdtctl.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int ID_OXGRIPPER_WIDTH = 0;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// COXCoolToolBar idle update through COXCoolToolBarCmdUI class. 

void COXCoolToolBarCmdUI::Enable(BOOL bOn)
{
	m_bEnableChanged = TRUE;
	COXCoolToolBar* pCoolToolBar = (COXCoolToolBar*)m_pOther;
	CToolBarCtrl* pToolBarCtrl = &pCoolToolBar->GetToolBarCtrl();
	ASSERT(pToolBarCtrl != NULL);
	
	ASSERT(m_nIndex < m_nIndexMax);
	
	// Get toolbar button state
	TBBUTTON TB;
	pToolBarCtrl->GetButton(m_nIndex, &TB);
	BYTE nNewState = (BYTE) (TB.fsState & ~TBSTATE_ENABLED);
	if (bOn)
		nNewState |= TBSTATE_ENABLED;
	if (nNewState != TB.fsState)
	{
#ifdef TBIF_BYINDEX
		TBBUTTONINFO	bi;
		ZeroMemory(&bi, sizeof(bi));
		bi.cbSize=sizeof(bi);
		bi.dwMask = TBIF_BYINDEX | TBIF_STATE;
		bi.fsState = nNewState;

		pToolBarCtrl->SetButtonInfo(m_nIndex, &bi);
#else
		pToolBarCtrl->SetState(m_nID, nNewState);
#endif
	}
}


void COXCoolToolBarCmdUI::SetCheck(int nCheck)
{
	ASSERT(nCheck >= 0 && nCheck <= 2); // 0=>off, 1=>on, 2=>indeterminate
	COXCoolToolBar* pCoolToolBar = (COXCoolToolBar*)m_pOther;
	CToolBarCtrl* pToolBarCtrl = &pCoolToolBar->GetToolBarCtrl();
	ASSERT(pToolBarCtrl != NULL);
	
	ASSERT(m_nIndex < m_nIndexMax);
	
	// Get toolbar button state
	TBBUTTON TB;
	pToolBarCtrl->GetButton(m_nIndex, &TB);	
	BYTE nNewState = (BYTE) (TB.fsState & ~ (TBSTATE_CHECKED | TBSTATE_INDETERMINATE));
	
	if (nCheck == 1)
		nNewState |= TBSTATE_CHECKED;
	else
		if (nCheck == 2)
		nNewState |= TBSTATE_INDETERMINATE;
	
	if (nNewState != TB.fsState)
	{
#ifdef TBIF_BYINDEX
		TBBUTTONINFO	bi;
		ZeroMemory(&bi, sizeof(bi));
		bi.cbSize=sizeof(bi);
		bi.dwMask = TBIF_BYINDEX | TBIF_STATE;
		bi.fsState = nNewState;

		pToolBarCtrl->SetButtonInfo(m_nIndex, &bi);
#else
		pToolBarCtrl->SetState(m_nID, nNewState);
#endif
	}
	
	// should we set the button style too ?
	//	pToolBarCtrl->_SetButtonStyle(m_nIndex, nNewStyle | TBBS_CHECKBOX); 
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// COXCoolToolBar
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include "OXSkins.h"

DWORD COXCoolToolBar::m_dwComCtlVersion=0;
CMap<int,int,OXCUSTOMBUTTONDESCRIPTOR,OXCUSTOMBUTTONDESCRIPTOR&> 
	COXCoolToolBar::m_mapAllCustomButtons;
CArray<int,int> COXCoolToolBar::m_arrAllCustomButtonIDs;
BOOL COXCoolToolBar::m_bCustomButtonsStateSaved=FALSE;
BOOL COXCoolToolBar::m_bCustomButtonsStateLoaded=FALSE;

IMPLEMENT_DYNAMIC(COXCoolToolBar,CToolBar)

COXCoolToolBar::COXCoolToolBar() :
#if _MFC_VER>=0x0420
	m_bCool(FALSE),
	// by default - flat mode
	m_bSeparator(TRUE), 
	m_nIndent(0),
	// means not set
	m_sizeMinMaxWidth(-1,-1),
	m_crDefaultTextColor(CLR_DEFAULT),
	m_crHotTextColor(CLR_DEFAULT),
	m_crSelectedTextColor(CLR_DEFAULT),
	m_crCheckedTextColor(CLR_DEFAULT),
	m_crDefaultBkColor(CLR_DEFAULT),
	m_crHotBkColor(CLR_DEFAULT),
	m_crSelectedBkColor(CLR_DEFAULT),
	m_crCheckedBkColor(CLR_DEFAULT),
	m_crDefaultBorderColor(CLR_DEFAULT),
	m_crHotBorderColor(CLR_DEFAULT),
	m_crSelectedBorderColor(CLR_DEFAULT),
	m_crCheckedBorderColor(CLR_DEFAULT),
	m_bDropDownArrow(FALSE),
#endif
	m_ttID(TTID_NOTSET),
	m_pBitmapIds(NULL),
	m_nBitmapButtons(0),
	m_hIcon(NULL),
    m_bPrevFloating(3),				// neither TRUE not FALSE;
	m_dwPrevDockSide((DWORD)-1),	// none of side
	m_nCustomizedButtonIndex(-1),
	m_bAdvancedCutomizable(FALSE),
	m_bInAdvancedCustomizationMode(FALSE),
	m_bDragDropOwner(FALSE),
	m_bDragDropOperation(FALSE),
	m_hWndCustomizeOrganizer(NULL),
	m_nDraggedButtonIndex(-1),
	m_bNoInternalRedraw(FALSE),
	m_bNoBkgndRedraw(FALSE),
	m_nIdleFlags(0),
	m_pToolbarSkin(NULL),
	m_iDropDownIndex(-1),
	m_iLastDropDownIndex(-1),
	m_bFloatingEnabled(TRUE),
	m_bSnapWhileDragging(FALSE),
	m_bDragging(FALSE),
	m_ptLButtonDown(0, 0)
{
	m_bWindowsNTRunning=IsWindowsNTRunning();
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

	m_nDropDownArrowWidth=GetDropDownArrowWidth();

	m_iconRect.SetRectEmpty();
}

COXCoolToolBar::~COXCoolToolBar()
{
	LPTSTR lpszResourceName;

	// delete all bitmaps that we associated with CoolToolBar
	HBITMAP hBitmap;
	POSITION pos=m_allBitmaps.GetStartPosition();
	while(pos!=NULL)
	{
		m_allBitmaps.GetNextAssoc(pos,lpszResourceName,hBitmap);
		::DeleteObject(hBitmap);
	}
	m_allBitmaps.RemoveAll();

	// delete all image lists that we associated with CoolToolBar
	HANDLE hImageList;
	pos=m_allImageLists.GetStartPosition();
	while(pos!=NULL)
	{
		m_allImageLists.GetNextAssoc(pos,lpszResourceName,hImageList);
		ImageList_Destroy((HIMAGELIST)hImageList);
	}
	m_allImageLists.RemoveAll();

	// delete the classic skin
	if (m_pToolbarSkin != NULL)
		delete m_pToolbarSkin;
}

BEGIN_MESSAGE_MAP(COXCoolToolBar, CToolBar)
	//{{AFX_MSG_MAP(COXCoolToolBar)
	ON_WM_NCCREATE()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCCALCSIZE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SETTINGCHANGE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_MESSAGE(TB_INSERTBUTTON, OnInsertButton)
	ON_MESSAGE(TB_DELETEBUTTON, OnDeleteButton)
#if _MFC_VER>=0x0420
	ON_MESSAGE(TB_MOVEBUTTON, OnMoveButton)
#endif

	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)

	// reflect messages to make customization work
	ON_NOTIFY_REFLECT_EX(TBN_BEGINDRAG, OnTBNBeginDrag)
	ON_NOTIFY_REFLECT_EX(TBN_ENDDRAG, OnTBNEndDrag)
	ON_NOTIFY_REFLECT_EX(TBN_QUERYINSERT, OnTBNQueryInsert)
	ON_NOTIFY_REFLECT_EX(TBN_QUERYDELETE, OnTBNQueryDelete)
	ON_NOTIFY_REFLECT_EX(TBN_TOOLBARCHANGE, OnTBNToolBarChange)
	ON_NOTIFY_REFLECT_EX(TBN_GETBUTTONINFO, OnTBNGetButtonInfo)

	// handle drag'n'drop messages
	ON_MESSAGE(SHBDTM_DRAGENTER, OnDragEnter)
	ON_MESSAGE(SHBDTM_DRAGLEAVE, OnDragLeave)
	ON_MESSAGE(SHBDTM_DRAGOVER, OnDragOver)
	ON_MESSAGE(SHBDTM_DROP, OnDrop)

	// handle advanced customization commands
	ON_COMMAND(ID_OXCUSTTB_DELETE,OnCustTBDelete)
	ON_COMMAND(ID_OXCUSTTB_APPEARANCE,OnCustTBAppearance)
	ON_COMMAND(ID_OXCUSTTB_IMAGEONLY,OnCustTBImageOnly)
	ON_COMMAND(ID_OXCUSTTB_IMAGETEXT,OnCustTBImageText)
	ON_COMMAND(ID_OXCUSTTB_SEPARATOR_BEFORE,OnCustTBSeparatorBefore)
	ON_COMMAND(ID_OXCUSTTB_SEPARATOR_AFTER,OnCustTBSeparatorAfter)

#if _MFC_VER>=0x0420
	// reflect message to provide custom draw functionality
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW,OnCustomDraw)
#endif

	ON_NOTIFY_REFLECT_EX(TBN_DROPDOWN, OnDropDownButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXCoolToolBar message handlers

BOOL COXCoolToolBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{
//#if _WIN32_IE>=0x0400
	if(m_dwComCtlVersion>=_IE40_COMCTL_VERSION)
	{
		ASSERT_VALID(pParentWnd);   // must have a parent
		ASSERT (!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)));

		dwStyle|=WS_CLIPCHILDREN;

#if _MFC_VER<=0x0421
		// save the style
		m_dwStyle = dwStyle;
		if (nID == AFX_IDW_TOOLBAR)
			m_dwStyle |= CBRS_HIDE_INPLACE;

		dwStyle &= ~CBRS_ALL;
		dwStyle |= CCS_NOPARENTALIGN|CCS_NODIVIDER|CCS_NORESIZE;

		// by default set flat style
		dwStyle|=TBSTYLE_FLAT;

		// Initialize bar common controls
		static BOOL bInitCoolToolBar = FALSE;
		if (!bInitCoolToolBar) 
		{
			INITCOMMONCONTROLSEX sex;
			sex.dwSize = sizeof(INITCOMMONCONTROLSEX);
			sex.dwICC = ICC_BAR_CLASSES;
			InitCommonControlsEx(&sex);
			bInitCoolToolBar = TRUE;
		}

		// Create the CoolToolBar using style and parent.
		CRect rc;
		rc.SetRectEmpty();
		if(CWnd::CreateEx(WS_EX_TOOLWINDOW, TOOLBARCLASSNAME, NULL,
			dwStyle, rc.left, rc.top, rc.Width(), rc.Height(), 
			pParentWnd->m_hWnd, (HMENU)nID))
#else
		if(CToolBar::Create(pParentWnd, dwStyle, nID))
#endif
		{
#if _MFC_VER>0x0421
			// by default set flat style
			ModifyStyle(0,TBSTYLE_FLAT|TBSTYLE_TRANSPARENT);
			SetBorders(0,0,0,0);
			SetDropDownArrow(TRUE);
#else
			SendMessage(TB_BUTTONSTRUCTSIZE,sizeof(TBBUTTON));
#endif
			if(pParentWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
				VERIFY(ModifyStyle(TBSTYLE_TRANSPARENT,0));
		}
		else
			return FALSE;
	}
//#else
	else
	{
		if(!CToolBar::Create(pParentWnd,dwStyle,nID))
			return FALSE;

#if _MFC_VER>=0x0420
		// by default set flat style
		SetFlat();
#endif //_MFC_VER>=0x0420
	}
//#endif //_WIN32_IE>=0x0400

	// register OLE Drag'n'Drop
	COleDropTarget* pOleDropTarget=GetDropTarget();
	ASSERT(pOleDropTarget!=NULL);
	if(!pOleDropTarget->Register(this))
	{
		TRACE(_T("COXCoolToolBar::Create: failed to register the control with COleDropTarget. You've probably forgotten to initialize OLE libraries using AfxOleInit function\n"));
	}

	// Add empty string
	VERIFY(GetToolBarCtrl().AddStrings(_T("\0"))==0);

	return TRUE;
}

BOOL COXCoolToolBar::OnNcCreate(LPCREATESTRUCT lpCreateStruct) 
{
#if _MFC_VER>=0x0420
	if(IsCool())
	{
		// bypass CToolBar/CControlBar
		return (BOOL)Default();
	}
	else
	{
		return CToolBar::OnNcCreate(lpCreateStruct);
	}
#else
	return CToolBar::OnNcCreate(lpCreateStruct);
#endif
}

void COXCoolToolBar::OnNcPaint()
{
	CWindowDC dc(this);

	// Exclude the client rectangle, but not the gripper
	CRect rectWindow, rectClient;
	GetWindowRect(rectWindow);
	GetClientRect(rectClient);
	ClientToScreen(rectClient);
	rectClient.OffsetRect(-rectWindow.left, -rectWindow.top);
	dc.ExcludeClipRect(rectClient);

	CRect rect(0, 0, 0, 0);
	GetWindowRect(rect);
	rect.OffsetRect(-rect.left, -rect.top);

	GetToolbarSkin()->DrawNonClientArea(&dc, rect, this);

#if _MFC_VER>=0x0420
	if(IsCool())
	{
		// bypass CToolBar/CControlBar
		Default();	
	}
	else
	{
		if(IsFlat() && IsGripper())
			DrawGripper(dc,rect);
	}
#endif

	if(m_hIcon!=NULL)
		DrawIcon(dc,rect);

	DrawInBookedSpace(dc,rect);
}

void COXCoolToolBar::OnPaint()
{
	if(m_dwComCtlVersion>=_IE40_COMCTL_VERSION)
	{
		CToolBar::OnPaint();
	}
	else
	{
		// CToolBar has some problems with painting when CToolBar is dockable
		// (due to a bug in comctl32.dll version 4.70 which is supplied with IE 3.0)
		CRect rectUpdate(0,0,0,0);
		GetUpdateRect(&rectUpdate,TRUE);

#if _MFC_VER>=0x0420
		if(IsCool())
		{
			// bypass CToolBar/CControlBar
			Default();	
		}
		else
		{
			if(IsFloating())
			{
				CToolBar::OnPaint();
			}
			else
			{
				Default();	
				if(!IsFlat())
				{
					InvalidateRect(&rectUpdate);
					CToolBar::OnPaint();
				}
			}
		}
#else
		if(IsFloating())
		{
			CToolBar::OnPaint();
		}
		else
		{
			Default();	
			InvalidateRect(&rectUpdate);
			CToolBar::OnPaint();
		}
#endif
	}
}

BOOL COXCoolToolBar::OnEraseBkgnd(CDC* pDC)
{
	if (GetToolbarSkin()->CallOnEraseBkgnd() == TRUE)
		return CToolBar::OnEraseBkgnd(pDC);
	else
		return FALSE;
}

void COXCoolToolBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
#if _MFC_VER>=0x0420
	if(IsCool())
	{
		// bypass CToolBar/CControlBar
		Default();	

		// Change the non-client area of CoolToolBar
		// to make it look pretty in CoolBar
		int nAddIntension=IsFlat() ? 2 : 0;
		if(m_dwStyle&CBRS_ORIENT_HORZ) 
		{
			lpncsp->rgrc[0].top+=nAddIntension;
			lpncsp->rgrc[0].bottom+=nAddIntension;
		} 
		else 
		{
			lpncsp->rgrc[0].left+=nAddIntension;
			lpncsp->rgrc[0].right+=nAddIntension;
		}
	}
	else
	{
		CToolBar::OnNcCalcSize(bCalcValidRects, lpncsp);

		if(IsFlat())
		{
			// adjust non-client area
#if _MFC_VER<=0x0421
			lpncsp->rgrc[0].top+=2;
			lpncsp->rgrc[0].bottom+=2;
#endif
			// adjust non-client area for gripper at left or top
			if(m_dwStyle&CBRS_ORIENT_HORZ) 
			{
				if(IsGripper() && !(m_dwStyle & CBRS_FLOATING))
				{
					lpncsp->rgrc[0].left+=ID_OXGRIPPER_WIDTH;
				}
			} 
			else 
			{
				if(IsGripper() && !(m_dwStyle & CBRS_FLOATING))
				{
					lpncsp->rgrc[0].top+=ID_OXGRIPPER_WIDTH;
				}
			}
		}
	}
#else
	CToolBar::OnNcCalcSize(bCalcValidRects, lpncsp);
#endif

	if(m_hIcon!=NULL)
	{
		// adjust non-client area for icon at left or top
		if(m_dwStyle&CBRS_ORIENT_HORZ) 
		{
			int nIconWidth=::GetSystemMetrics(SM_CXSMICON);
			lpncsp->rgrc[0].left+=nIconWidth+2;
		} 
		else 
		{
			int nIconHeight=::GetSystemMetrics(SM_CYSMICON);
			lpncsp->rgrc[0].top+=nIconHeight+2;
		}
	}

	CRect rectBookedSpace(0,0,0,0);
	BookSpace(rectBookedSpace,(m_dwStyle&CBRS_ORIENT_HORZ) ? LM_HORZ : 0);
	lpncsp->rgrc[0].left+=rectBookedSpace.left;
	lpncsp->rgrc[0].right-=rectBookedSpace.right;
	lpncsp->rgrc[0].top+=rectBookedSpace.top;
	lpncsp->rgrc[0].bottom-=rectBookedSpace.bottom;
}


void COXCoolToolBar::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
#if _MFC_VER>=0x0420
	if(IsCool())
	{
		// bypass CToolBar/CControlBar
		Default();	
	}
	else
	{
		CToolBar::OnWindowPosChanging(lpwndpos);
	}
	
#if _MFC_VER<=0x0421
	// TODO: Add your message handler code here
	if (IsFlat() && !(lpwndpos->flags & SWP_NOMOVE)) 
	{										 // if moved:
		CRect rc;							 //   Fill rectangle with..
		GetWindowRect(&rc);					 //   ..my (toolbar) rectangle.
		CWnd* pParent = GetParent();		 //   get parent (dock bar/frame) win..
		pParent->ScreenToClient(&rc);		 //   .. and convert to parent coords

		// Ask parent window to paint the area beneath my old location.
		// Typically, this is just solid grey.
		//
		pParent->InvalidateRect(&rc);		 // paint old rectangle

		// Now paint my non-client area at the new location.
		// This is the extra bit of border space surrounding the buttons.
		// Without this, you will still have a partial display bug
		//
		if(m_bWindowsNTRunning)
		{
			// we need this code on NT systems 
			// because we've got problem with redrawing
			CRect rect;
			GetWindowRect(&rect);
			// redraw 
			SetWindowPos(NULL,0,0,rect.Width(),rect.Height(),
				SWP_NOMOVE|SWP_NOZORDER|SWP_DRAWFRAME|
				SWP_FRAMECHANGED|SWP_NOREDRAW);
		}
		else
		{
			PostMessage(WM_NCPAINT);
		}
	}
#endif	//	_MFC_VER<=0x0421
#else
	CToolBar::OnWindowPosChanging(lpwndpos);
#endif	//	_MFC_VER>=0x0420
}


void COXCoolToolBar::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	// TODO: Add your message handler code here
	
	if(m_pDockContext!=NULL)
	{
		m_pDockContext->m_bDragging=FALSE;
	}

	CToolBar::OnWindowPosChanged(lpwndpos);

    BOOL bFloating=IsFloating();

	if(bFloating!=m_bPrevFloating)
	{
		m_bPrevFloating=bFloating;
		OnFloatingDocking(bFloating);
	}
	else
	{
		DWORD dwDockSide=GetBarStyle()&CBRS_ALIGN_ANY;
		if(m_dwPrevDockSide!=dwDockSide)
		{
			m_dwPrevDockSide=dwDockSide;
			OnChangeDockSide(m_dwPrevDockSide);
		}
	}

	DelayUpdateCustomButtons();
	SendMessage(WM_NCPAINT, 0, 0);
}


LRESULT COXCoolToolBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	if(IsWindowVisible())
	{
		// redraw the toolbar if necessary
		if(m_nIdleFlags & oxidleRedrawToolbar)
		{
			RedrawToolBar(TRUE,TRUE);
		}
		
		if(m_nIdleFlags & oxidleUpdateCustomButtons)
		{
			UpdateCustomButtons();
		}

		m_nIdleFlags=0;
	}

	return CToolBar::OnIdleUpdateCmdUI(wParam,lParam);
}


void COXCoolToolBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	COXCoolToolBarCmdUI state;
	state.m_pOther = this;
	
	state.m_nIndexMax = GetToolBarCtrl().GetButtonCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++)
	{
		// get buttons state
		TBBUTTON TB;
		GetToolBarCtrl().GetButton(state.m_nIndex, &TB);
		state.m_nID = TB.idCommand;

		// ignore separators
		if (!(TB.fsStyle & TBSTYLE_SEP) || IsCustomButton(state.m_nIndex))
		{
			// allow the toolbar itself to have update handlers
			if (CWnd::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
				continue;

			// allow the owner to process the update
			state.DoUpdate(pTarget, bDisableIfNoHndler);
		}
	}

	// update any dialog controls added to the toolbar
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}


void COXCoolToolBar::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	UNREFERENCED_PARAMETER(uFlags);
	UNREFERENCED_PARAMETER(lpszSection);

	m_nDropDownArrowWidth=GetDropDownArrowWidth();
	RedrawToolBar();

	CToolBar::OnSettingChange(uFlags, lpszSection);
}


LONG COXCoolToolBar::OnDragEnter(WPARAM wParam, LPARAM lParam)
{
	// toolbar must be in advanced customizable state
	if(!IsCustomizable(TRUE))
		return (LONG)FALSE;

	// set flag that specifies that drag'n'drop operation is active
	m_bDragDropOperation=TRUE;

	// lParam is the pointer to SHBDROPTARGETACTION structure
	LPSHBDROPTARGETACTION pSHBDTAction=(LPSHBDROPTARGETACTION)lParam;
	ASSERT(pSHBDTAction!=NULL);

	ASSERT(pSHBDTAction->pWnd);
	ASSERT(pSHBDTAction->pWnd->GetSafeHwnd()==GetSafeHwnd());

	return OnDragOver(wParam,lParam);
}


LONG COXCoolToolBar::OnDragOver(WPARAM wParam, LPARAM lParam)
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
				
				// unlock it
				::GlobalUnlock(hgData);
				// free it
				::GlobalFree(hgData);

				if(nCommandID<0 || (nCommandID>0 && CommandToIndex(nCommandID)!=-1))
					bQualified=FALSE;
			}

			if(bQualified)
			{
				// analize the current cursor position
				//
				CPoint point=pSHBDTAction->point;

				int nButtonIndex=HitTest(&point);
				BOOL bIsOut=(nButtonIndex<0);
				nButtonIndex=(bIsOut ? -nButtonIndex : nButtonIndex);
				if(nButtonIndex>GetToolBarCtrl().GetButtonCount() && 
					GetToolBarCtrl().GetButtonCount()>0)
				{
					nButtonIndex=-1;
					bQualified=FALSE;
				}
				else
				{
					// check if we are really over custom button, 
					// which is interpreted as separator and never being 
					// reterned as legitimate value from HitTest() function
					CRect rectItem;
					GetItemRect(nButtonIndex,rectItem);
					if(!rectItem.PtInRect(point))
					{
						// check if previous button is custom one
						if(nButtonIndex>0 && IsCustomButton(nButtonIndex-1))
						{
							nButtonIndex--;
						}
					}

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
						if(nButtonIndex==GetToolBarCtrl().GetButtonCount() && 
							nButtonIndex>0)
						{
							tbim.iButton=nButtonIndex-1;
							tbim.dwFlags=TBIMHT_AFTER;
						}
						else
						{
							tbim.iButton=nButtonIndex;
							tbim.dwFlags=0;
							CRect rectItem;
							GetItemRect(tbim.iButton,rectItem);
							if(point.x>=rectItem.left+rectItem.Width()/2 || point.x<0)
							{
								tbim.dwFlags=TBIMHT_AFTER;
							}
						}
						SetInsertMark(&tbim);

						// Check if the control key was pressed          
						if((pSHBDTAction->dwKeyState & MK_CONTROL)==MK_CONTROL)
							pSHBDTAction->result=(LRESULT)DROPEFFECT_COPY;
						else
							pSHBDTAction->result=(LRESULT)DROPEFFECT_MOVE; 
					}
				}
			}
		}
			
		if(!bQualified)
			SetInsertMark(-1);
	}
#endif	//	OX_CUSTOMIZE_COMMANDS

	return (LONG)TRUE;
}


LONG COXCoolToolBar::OnDragLeave(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if(!IsCustomizable(TRUE))
		return (LONG)FALSE;

	// lParam is the pointer to SHBDROPTARGETACTION structure
	LPSHBDROPTARGETACTION pSHBDTAction=(LPSHBDROPTARGETACTION)lParam;
	ASSERT(pSHBDTAction!=NULL);

	ASSERT(pSHBDTAction->pWnd);
	ASSERT(pSHBDTAction->pWnd->GetSafeHwnd()==GetSafeHwnd());

	SetInsertMark(-1);

	if(!m_bDragDropOwner)
	{
		// reset flag that specifies that drag'n'drop operation is active
		m_bDragDropOperation=FALSE;
	}

	return (LONG)TRUE;
}


LONG COXCoolToolBar::OnDrop(WPARAM wParam, LPARAM lParam)
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
		if(nButtonIndex!=-1 || GetToolBarCtrl().GetButtonCount()==0)
		{
			BOOL bButtonRemainsTheSame=FALSE;
			if(m_bDragDropOwner && (pSHBDTAction->dropEffect&DROPEFFECT_COPY)==0)
			{
				ASSERT(GetToolBarCtrl().GetButtonCount()>0);

				int nDraggedButtonIndex=GetDraggedButton();
				ASSERT(nDraggedButtonIndex!=-1);
				if(nDraggedButtonIndex<GetToolBarCtrl().GetButtonCount()-1 &&
					((tbim.dwFlags==0 && nButtonIndex==nDraggedButtonIndex) ||
					(tbim.dwFlags==TBIMHT_AFTER && 
					(nButtonIndex+1)==nDraggedButtonIndex)))
				{
					if((GetButtonStyle(nDraggedButtonIndex+1)&TBSTYLE_SEP)==0 ||
						IsCustomButton(nDraggedButtonIndex+1))
					{
						TBBUTTON button={ 0 };
						button.fsStyle=TBSTYLE_SEP;
						button.iString=-1;
						button.iBitmap=-1;
						VERIFY(GetToolBarCtrl().
							InsertButton(nDraggedButtonIndex+1,&button));
					}
					bButtonRemainsTheSame=TRUE;
				}
				else if(nDraggedButtonIndex>0 && ((tbim.dwFlags==TBIMHT_AFTER && 
					nButtonIndex==nDraggedButtonIndex) ||
					(tbim.dwFlags==0 && (nButtonIndex-1)==nDraggedButtonIndex)))
				{
					if((GetButtonStyle(nDraggedButtonIndex-1)&TBSTYLE_SEP)==0 ||
						IsCustomButton(nDraggedButtonIndex-1))
					{
						TBBUTTON button={ 0 };
						button.fsStyle=TBSTYLE_SEP;
						button.iString=-1;
						button.iBitmap=-1;
						VERIFY(GetToolBarCtrl().
							InsertButton(nDraggedButtonIndex,&button));
					}
					bButtonRemainsTheSame=TRUE;
				}
				else if((nButtonIndex==nDraggedButtonIndex+2 && tbim.dwFlags==0) ||
					(nButtonIndex==nDraggedButtonIndex+1 && 
					tbim.dwFlags==TBIMHT_AFTER))
				{
					if((GetButtonStyle(nDraggedButtonIndex+1)&TBSTYLE_SEP)!=0 &&
						!IsCustomButton(nDraggedButtonIndex+1))
					{
						VERIFY(GetToolBarCtrl().DeleteButton(nDraggedButtonIndex+1));
						nButtonIndex--;
						bButtonRemainsTheSame=TRUE;
					}
				}
				else if((nButtonIndex==nDraggedButtonIndex-2 && 
					tbim.dwFlags==TBIMHT_AFTER) ||
					(nButtonIndex==nDraggedButtonIndex-1 && tbim.dwFlags==0))
				{
					if((GetButtonStyle(nDraggedButtonIndex-1)&TBSTYLE_SEP)!=0 &&
						!IsCustomButton(nDraggedButtonIndex-1))
					{
						VERIFY(GetToolBarCtrl().DeleteButton(nDraggedButtonIndex-1));
						bButtonRemainsTheSame=TRUE;
					}
				}
			}

			// remove insert mark
			SetInsertMark(-1);

			if(bButtonRemainsTheSame)
			{
				m_bDragDropOperation=FALSE;
				pSHBDTAction->result=(LRESULT)TRUE;
				return (LONG)TRUE;
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

			if(m_bDragDropOwner)
			{
#if _MFC_VER<0x0420
				if(!IsCustomButton(GetDraggedButton()))
#endif
				{
					int nOldButtonPos=GetDraggedButton();
					int nNewButtonPos=(nButtonIndex>GetDraggedButton() ? 
						nButtonIndex-1 : nButtonIndex);
					if(nOldButtonPos!=nNewButtonPos)
					{
						// just move button
						VERIFY(MoveButton(nOldButtonPos,nNewButtonPos));
						SetCustomizedButton(nNewButtonPos);
					}

					m_bDragDropOperation=FALSE;
					pSHBDTAction->result=(LRESULT)TRUE;
					return (LONG)TRUE;
				}
			}

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
			lpItemData+=sText.GetLength()*sizeof(TCHAR)+sizeof(TCHAR);
			int nPosition=sText.Find(_T('\t'));
			if(nPosition!=-1)
				sText=sText.Left(nPosition);
		

			// get button image index
			int nImageIndex=*(int*)lpItemData;
			lpItemData+=sizeof(int);

			// get button style
			BYTE fsStyle=*(BYTE*)lpItemData;
			lpItemData+=sizeof(BYTE);

			if(nCommandID>0 || (nCommandID==0 && nButtonCount>0))
			{
				// determine if dragged button is a custom one
				BOOL bCustomButton=(GetCustomButtonIndex(nCommandID)!=-1);

				if(!bCustomButton)
				{
					// insert new button
					//
					TBBUTTON button={ 0 };
					button.iBitmap=nImageIndex;
					button.idCommand=nCommandID;
					if((fsStyle&TBSTYLE_SEP)==TBSTYLE_SEP)
						fsStyle&=~TBSTYLE_AUTOSIZE;
					else
						fsStyle|=TBSTYLE_AUTOSIZE;
					button.fsStyle=fsStyle;
					button.iString=-1;

					// don't redraw the toolbar contents, we will do it later
					m_bNoInternalRedraw=TRUE;
					VERIFY(GetToolBarCtrl().InsertButton(nButtonIndex,&button));
					m_bNoInternalRedraw=FALSE;
					if(!sText.IsEmpty())
					{
						VERIFY(SetButtonText(nButtonIndex,sText));
					}
					RedrawToolBar(TRUE,TRUE);
					//////////////////////////////
				}
				else
				{
					int nCustomButtonIndex=GetCustomButtonIndex(nCommandID);
					ASSERT(nCustomButtonIndex!=-1);
					// insert new custom button
					VERIFY(InsertCustomButton(nButtonIndex,nCustomButtonIndex));
					if(IsInAdvancedCustomizationMode())
					{
						// set it into customization mode
						OXCUSTOMBUTTONDESCRIPTOR descriptor;
						VERIFY(GetCustomButton(nButtonIndex,descriptor));
						descriptor.m_pCBTemplate->CBSetCustomizationMode(TRUE);
					}
				}
			}

			// unlock it
			::GlobalUnlock(hgData);
			// free it
			::GlobalFree(hgData);

			// drag'n'drop operation completed successfully
			pSHBDTAction->result=(LRESULT)TRUE;
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


LRESULT COXCoolToolBar::OnInsertButton(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	LRESULT lResult=Default();

	if((BOOL)lResult)
	{
		CRect rect;
		GetClientRect(rect);
		ValidateRect(rect);
	
		if((int)wParam<=m_nCustomizedButtonIndex)
		{
			SetCustomizedButton(m_nCustomizedButtonIndex+1);
		}
		if((int)wParam<=m_nDraggedButtonIndex)
		{
			SetDraggedButton(m_nDraggedButtonIndex+1);
		}

		// update custom buttons positions
		for(int nButtonIndex=GetToolBarCtrl().GetButtonCount()-2; 
			nButtonIndex>=(int)wParam; nButtonIndex--)
		{
			OXCUSTOMBUTTONDESCRIPTOR descriptor;
			if(m_mapCustomButtons.Lookup(nButtonIndex,descriptor))
			{
				m_mapCustomButtons.SetAt(nButtonIndex+1,descriptor);
				m_mapCustomButtons.RemoveKey(nButtonIndex);
			}
		}
	}
	RedrawToolBar(TRUE,TRUE);
	
	return lResult;
}


LRESULT COXCoolToolBar::OnDeleteButton(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	LRESULT lResult=Default();
	
	if((BOOL)lResult)
	{
		CRect rect;
		GetClientRect(rect);
		ValidateRect(rect);
	
		int nButtonIndex=(int)wParam;

		// delete custom button if any found
		OXCUSTOMBUTTONDESCRIPTOR descriptor;
		if(GetCustomButton(nButtonIndex,descriptor))
		{
			descriptor.m_pCBTemplate->CBDelete();
			m_mapCustomButtons.RemoveKey(nButtonIndex);
		}
		// update custom buttons positions
		for(int nIndex=nButtonIndex+1; 
			nIndex<=GetToolBarCtrl().GetButtonCount(); nIndex++)
		{
			OXCUSTOMBUTTONDESCRIPTOR descriptor;
			if(m_mapCustomButtons.Lookup(nIndex,descriptor))
			{
				m_mapCustomButtons.SetAt(nIndex-1,descriptor);
				m_mapCustomButtons.RemoveKey(nIndex);
			}
		}

		if(nButtonIndex<m_nCustomizedButtonIndex)
			SetCustomizedButton(m_nCustomizedButtonIndex-1);
		else if(nButtonIndex==m_nCustomizedButtonIndex)
			SetCustomizedButton(-1);

		if(nButtonIndex<m_nDraggedButtonIndex)
			SetDraggedButton(m_nDraggedButtonIndex-1);
		else if(nButtonIndex==m_nDraggedButtonIndex)
			SetDraggedButton(-1);

		int nButtonCount=GetToolBarCtrl().GetButtonCount();
		if(nButtonIndex==nButtonCount && nButtonCount>0 &&
			GetButtonStyle(nButtonCount-1)&TBBS_SEPARATOR &&
			!IsCustomButton(nButtonCount-1))
		{
			m_bNoInternalRedraw=TRUE;
			VERIFY(GetToolBarCtrl().DeleteButton(nButtonCount-1));
			m_bNoInternalRedraw=FALSE;
		}
		nButtonCount=GetToolBarCtrl().GetButtonCount();
		if(nButtonIndex==0 && nButtonCount>0 &&	
			GetButtonStyle(0)&TBBS_SEPARATOR &&	!IsCustomButton(0))
		{
			m_bNoInternalRedraw=TRUE;
			VERIFY(GetToolBarCtrl().DeleteButton(0));
			m_bNoInternalRedraw=FALSE;
		}
	}

	RedrawToolBar(TRUE,TRUE);
	
	return lResult;
}


#if _MFC_VER>=0x0420
LRESULT COXCoolToolBar::OnMoveButton(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	LRESULT lResult=Default();
	
	if((BOOL)lResult && (int)wParam!=(int)lParam)
	{
		OXCUSTOMBUTTONDESCRIPTOR descriptorMoved;
		BOOL bCustomButton=GetCustomButton((int)wParam,descriptorMoved);
		BOOL bDeleteMovedDescriptor=FALSE;

		int nMinButtonIndex=__min((int)wParam,(int)lParam);
		int nMaxButtonIndex=__max((int)wParam,(int)lParam);

		// update custom buttons positions
		if((int)wParam<(int)lParam)
		{
			bDeleteMovedDescriptor=
				bCustomButton && !IsCustomButton(nMinButtonIndex+1);

			for(int nIndex=nMinButtonIndex+1; 
				nIndex<=nMaxButtonIndex; nIndex++)
			{
				OXCUSTOMBUTTONDESCRIPTOR descriptor;
				if(m_mapCustomButtons.Lookup(nIndex,descriptor))
				{
					m_mapCustomButtons.SetAt(nIndex-1,descriptor);
					m_mapCustomButtons.RemoveKey(nIndex);
				}
			}

			if(bCustomButton)
			{
				m_mapCustomButtons.SetAt(nMaxButtonIndex,descriptorMoved);
				if(bDeleteMovedDescriptor)
				{
					m_mapCustomButtons.RemoveKey(nMinButtonIndex);
				}
			}
		}
		else
		{
			bDeleteMovedDescriptor=
				bCustomButton && !IsCustomButton(nMaxButtonIndex-1);

			for(int nIndex=nMaxButtonIndex-1; 
				nIndex>=nMinButtonIndex; nIndex--)
			{
				OXCUSTOMBUTTONDESCRIPTOR descriptor;
				if(m_mapCustomButtons.Lookup(nIndex,descriptor))
				{
					m_mapCustomButtons.SetAt(nIndex+1,descriptor);
					m_mapCustomButtons.RemoveKey(nIndex);
				}
			}

			if(bCustomButton)
			{
				m_mapCustomButtons.SetAt(nMinButtonIndex,descriptorMoved);
				if(bDeleteMovedDescriptor)
				{
					m_mapCustomButtons.RemoveKey(nMaxButtonIndex);
				}
			}
		}

		UpdateCustomButtons(nMinButtonIndex,nMaxButtonIndex);
	}

	return lResult;
}
#endif


void COXCoolToolBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(IsInAdvancedCustomizationMode() && IsCustomizable(TRUE))
	{
		int nButtonIndex=HitTest(&point);
		if(nButtonIndex>=0 && nButtonIndex<GetToolBarCtrl().GetButtonCount())
		{
			SetCustomizedButton(nButtonIndex);
		}
		else
		{
			if(IsCustomButton(-nButtonIndex-1))
			{
				SetCustomizedButton(-nButtonIndex-1);
			}
			else
			{
				SetCustomizedButton(-1);
			}
		}

		CWnd::OnLButtonDown(nFlags,point);

		if(nButtonIndex>=0 && nButtonIndex<GetToolBarCtrl().GetButtonCount())
		{
			int nState=GetToolBarCtrl().GetState(GetItemID(nButtonIndex));
			if((nState&TBSTATE_PRESSED)!=0)
			{
				GetToolBarCtrl().SetState(GetItemID(nButtonIndex),
					nState&~TBSTATE_PRESSED);
			}
		}

		return;
	}

	if (IsFloatingEnabled() || (!IsFloatingEnabled() && HitTest(&point) >= 0))
	{
		if (AfxGetMainWnd()->SendMessage(WM_QUERYSNAPPING))
		{
			// only start dragging if clicked in "void" space
			if (m_pDockBar != NULL && OnToolHitTest(point, NULL) == -1)
			{
				// Start dragging
				SaveMouseOffset(point);
				m_bDragging = TRUE;
				SetCapture();
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
			}
			else
			{
				CWnd::OnLButtonDown(nFlags, point);
			}
		}
		else
			CToolBar::OnLButtonDown(nFlags, point);
	}
}


void COXCoolToolBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bDragging)
	{
		ReleaseCapture();
		m_bDragging = FALSE;
		return;
	}

	if(IsInAdvancedCustomizationMode() && IsCustomizable(TRUE))
	{
		return;
	}

	CToolBar::OnLButtonUp(nFlags,point);

	// update the state of the clicked button right away
	if(::IsWindow(GetSafeHwnd()))
	{
		SendMessage(WM_IDLEUPDATECMDUI);
	}
}


void COXCoolToolBar::OnMButtonDown(UINT nFlags, CPoint point)
{
	if(IsInAdvancedCustomizationMode() && IsCustomizable(TRUE))
	{
		int nButtonIndex=HitTest(&point);
		if(nButtonIndex>=0 && nButtonIndex<GetToolBarCtrl().GetButtonCount())
		{
			SetCustomizedButton(nButtonIndex);
		}
		else
		{
			if(IsCustomButton(-nButtonIndex-1))
			{
				SetCustomizedButton(-nButtonIndex-1);
			}
			else
			{
				SetCustomizedButton(-1);
			}
		}

		return;
	}

	CToolBar::OnMButtonDown(nFlags,point);
}


void COXCoolToolBar::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(IsInAdvancedCustomizationMode() && IsCustomizable(TRUE))
	{
		int nButtonIndex=HitTest(&point);
		if(nButtonIndex>=0 && nButtonIndex<GetToolBarCtrl().GetButtonCount())
		{
			SetCustomizedButton(nButtonIndex);
		}
		else
		{
			if(IsCustomButton(-nButtonIndex-1))
			{
				SetCustomizedButton(-nButtonIndex-1);
			}
			else
			{
				SetCustomizedButton(-1);
			}
		}

		return;
	}

	CToolBar::OnRButtonDown(nFlags,point);
}


void COXCoolToolBar::OnRButtonUp(UINT nFlags, CPoint point)
{
	if(IsInAdvancedCustomizationMode() && IsCustomizable(TRUE))
	{
		int nCustomizedButtonIndex=GetCustomizedButton();
		if(nCustomizedButtonIndex!=-1)
		{
			ClientToScreen(&point);
			VERIFY(DisplayCustomizeButtonContextMenu(nCustomizedButtonIndex,point));
			return;
		}
	}

	CToolBar::OnRButtonUp(nFlags,point);
}


void COXCoolToolBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if(IsInAdvancedCustomizationMode() && IsCustomizable(TRUE))
	{
		return;
	}

	if (m_bDragging)
	{
		// Make sure the mouse button is still down
		if (::GetKeyState(VK_LBUTTON) >= 0)
		{
			// No longer down
			ReleaseCapture();
			m_bDragging = FALSE;
			return;
		}

		CPoint ptScreen = point;
		ClientToScreen(&point);
		CRect rectWindow;
		GetWindowRect(rectWindow);
		CRect rectDock(point.x, point.y, point.x + rectWindow.Width(), point.y + rectWindow.Height());
		
		// Get the appropriate dock bar. If one is not found then float.
		CFrameWnd* pFrameWnd = DYNAMIC_DOWNCAST(CFrameWnd, ::AfxGetMainWnd());
		if (pFrameWnd == NULL)
		{
			ReleaseCapture();
			m_bDragging = FALSE;
			return;
		}

		// Handle pending WM_PAINT messages
		MSG msg;
		while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
			{
				ReleaseCapture();
				m_bDragging = FALSE;
				return;
			}
			DispatchMessage(&msg);
		}

		if (m_pDockContext == NULL)
			pFrameWnd->FloatControlBar(this, point);
		else
		{
			COXSizeDockBar* pDockBar = COXSizeDockBar::GetAppropriateDockBar(point, this);
			if (pDockBar == NULL || nFlags & MK_CONTROL)
				pFrameWnd->FloatControlBar(this, point - m_ptOffset);			
			else
				pDockBar->DockControlBar(this, rectDock);
		}
	}
	else
		CToolBar::OnMouseMove(nFlags,point);
}


void COXCoolToolBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(IsInAdvancedCustomizationMode() && IsCustomizable(TRUE))
	{
		return;
	}

	if (!IsFloatingEnabled())
	{
		if (HitTest(&point) >= 0)
			CToolBar::OnLButtonDblClk(nFlags,point);
	}
	else
		CToolBar::OnLButtonDblClk(nFlags,point);
}


int COXCoolToolBar::SetCustomizedButton(int nIndex) 
{ 
	int nOldCustomizedButtonIndex=m_nCustomizedButtonIndex;
	m_nCustomizedButtonIndex=nIndex;
	if(nOldCustomizedButtonIndex!=nIndex)
	{
		RedrawButton(nOldCustomizedButtonIndex);
	}
	if(nIndex!=-1)
	{
		int nState=GetToolBarCtrl().GetState(GetItemID(nIndex));
		if((nState&TBSTATE_PRESSED)!=0)
		{
			GetToolBarCtrl().SetState(GetItemID(nIndex),nState&~TBSTATE_PRESSED);
		}
		else
		{
			RedrawButton(nIndex);
		}
	}
	SendCustomizeNotification(ID_OXCUSTTB_SET_CUSTOMIZE_BUTTON);
	return nOldCustomizedButtonIndex;
}


BOOL COXCoolToolBar::DisplayCustomizeButtonContextMenu(int nButtonIndex, CPoint point)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(nButtonIndex>=0 && nButtonIndex<GetToolBarCtrl().GetButtonCount());

	CString sText=GetButtonText(nButtonIndex);
	TBBUTTON button;
	VERIFY(GetToolBarCtrl().GetButton(nButtonIndex,&button));
	ASSERT((button.fsStyle&TBSTYLE_SEP)==0 || IsCustomButton(nButtonIndex));
	ASSERT(!(sText.IsEmpty() && button.iBitmap==-1));

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
	if((button.fsStyle&TBSTYLE_SEP)==0 && !IsCustomButton(nButtonIndex))
	{
		sItem.LoadString(IDS_OX_CUSTTB_IMAGEONLY);
		VERIFY(menu.AppendMenu(MF_STRING|(sText.IsEmpty() ? MF_CHECKED : MF_UNCHECKED),
			ID_OXCUSTTB_IMAGEONLY,sItem));
		sItem.LoadString(IDS_OX_CUSTTB_IMAGETEXT);
		VERIFY(menu.AppendMenu(MF_STRING|(!sText.IsEmpty() && button.iBitmap!=-1 ? 
			MF_CHECKED : MF_UNCHECKED),ID_OXCUSTTB_IMAGETEXT,sItem));
	}
	VERIFY(menu.AppendMenu(MF_SEPARATOR));
	sItem.LoadString(IDS_OX_CUSTTB_SEPARATOR_BEFORE);
	VERIFY(menu.AppendMenu(MF_STRING|
		((nButtonIndex>0 && ((GetButtonStyle(nButtonIndex-1)&TBBS_SEPARATOR)==0 ||
		IsCustomButton(nButtonIndex-1))) ? 0 : MF_GRAYED),
		ID_OXCUSTTB_SEPARATOR_BEFORE,sItem));
	sItem.LoadString(IDS_OX_CUSTTB_SEPARATOR_AFTER);
	VERIFY(menu.AppendMenu(MF_STRING|
		((nButtonIndex<GetToolBarCtrl().GetButtonCount()-1 && 
		((GetButtonStyle(nButtonIndex+1)&TBBS_SEPARATOR)==0 ||
		IsCustomButton(nButtonIndex+1))) ? 0 : MF_GRAYED),
		ID_OXCUSTTB_SEPARATOR_AFTER,sItem));

	menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
		point.x,point.y,this);

	return TRUE;
}


void COXCoolToolBar::OnCustTBDelete()
{
	ASSERT(IsInAdvancedCustomizationMode());
	int nCustomizedButtonIndex=GetCustomizedButton();
	ASSERT(nCustomizedButtonIndex!=-1);

	if(SendCustomizeNotification(ID_OXCUSTTB_DELETE))
		return;

	VERIFY(GetToolBarCtrl().DeleteButton(nCustomizedButtonIndex));
}

void COXCoolToolBar::OnCustTBAppearance()
{
	ASSERT(IsInAdvancedCustomizationMode());
	int nCustomizedButtonIndex=GetCustomizedButton();
	ASSERT(nCustomizedButtonIndex!=-1);
	UNUSED(nCustomizedButtonIndex);

	SendCustomizeNotification(ID_OXCUSTTB_APPEARANCE);	
}

void COXCoolToolBar::OnCustTBImageOnly()
{
	ASSERT(IsInAdvancedCustomizationMode());
	int nCustomizedButtonIndex=GetCustomizedButton();
	ASSERT(nCustomizedButtonIndex!=-1);

	if(SendCustomizeNotification(ID_OXCUSTTB_IMAGEONLY))
		return;

	UINT nCommandID=GetItemID(nCustomizedButtonIndex);
#ifdef _DEBUG
	TBBUTTON button;
	VERIFY(GetToolBarCtrl().GetButton(nCustomizedButtonIndex,&button));
	ASSERT(button.iBitmap!=-1);
#endif

	CString sText=GetButtonText(nCustomizedButtonIndex);
	if(!sText.IsEmpty())
	{
		m_mapButtonText.SetAt(nCommandID,sText);
		SetButtonText(nCustomizedButtonIndex,_T(""));
		RedrawToolBar(TRUE,TRUE);
	}
}

void COXCoolToolBar::OnCustTBImageText()
{
	ASSERT(IsInAdvancedCustomizationMode());
	int nCustomizedButtonIndex=GetCustomizedButton();
	ASSERT(nCustomizedButtonIndex!=-1);

	if(SendCustomizeNotification(ID_OXCUSTTB_IMAGETEXT))
		return;

	UINT nCommandID=GetItemID(nCustomizedButtonIndex);
#ifdef _DEBUG
	TBBUTTON button;
	VERIFY(GetToolBarCtrl().GetButton(nCustomizedButtonIndex,&button));
	ASSERT(button.iBitmap!=-1);
#endif

	CString sText=GetButtonText(nCustomizedButtonIndex);
	if(sText.IsEmpty())
	{
		if(!m_mapButtonText.Lookup(nCommandID,sText))
		{
			HINSTANCE hInstance=
				AfxFindResourceHandle(MAKEINTRESOURCE(nCommandID),RT_STRING);
			ASSERT(hInstance!=NULL);
			sText.LoadString(nCommandID);
			int nPosition=sText.Find(_T('\n'));
			if(nPosition!=-1)
				sText=sText.Mid(nPosition+1);
		}
		if(!sText.IsEmpty())
		{
			SetButtonText(nCustomizedButtonIndex,sText);
			RedrawToolBar(TRUE,TRUE);
		}
	}
}

void COXCoolToolBar::OnCustTBSeparatorBefore()
{
	ASSERT(IsInAdvancedCustomizationMode());
	int nCustomizedButtonIndex=GetCustomizedButton();
	ASSERT(nCustomizedButtonIndex!=-1);

	if(SendCustomizeNotification(ID_OXCUSTTB_SEPARATOR_BEFORE))
		return;

	TBBUTTON button={ 0 };
	button.fsStyle=TBSTYLE_SEP;
	button.iString=-1;
	button.iBitmap=-1;
	VERIFY(GetToolBarCtrl().
		InsertButton(nCustomizedButtonIndex,&button));
}

void COXCoolToolBar::OnCustTBSeparatorAfter()
{
	ASSERT(IsInAdvancedCustomizationMode());
	int nCustomizedButtonIndex=GetCustomizedButton();
	ASSERT(nCustomizedButtonIndex!=-1);

	if(SendCustomizeNotification(ID_OXCUSTTB_SEPARATOR_AFTER))
		return;

	TBBUTTON button={ 0 };
	button.fsStyle=TBSTYLE_SEP;
	button.iString=-1;
	button.iBitmap=-1;
	VERIFY(GetToolBarCtrl().
		InsertButton(nCustomizedButtonIndex+1,&button));
}


LRESULT COXCoolToolBar::SendCustomizeNotification(UINT nCustomizeCmdID,
												  LPARAM lParam/*=NULL*/)
{
	HWND hWnd=m_hWndCustomizeOrganizer;
	if(hWnd==NULL)
	{
		hWnd=::GetParent(GetSafeHwnd());
	}
	if(hWnd==NULL)
		return (LRESULT)0;

	NMCTBCUSTOMIZE nmctbCustomize;
	nmctbCustomize.nmhdr.code=OXCTBN_CUSTOMIZECMD;
	nmctbCustomize.nmhdr.hwndFrom=GetSafeHwnd();
	nmctbCustomize.nmhdr.idFrom=GetDlgCtrlID();
	nmctbCustomize.nCustomizeEventID=nCustomizeCmdID;
	nmctbCustomize.lParam=lParam;

	return ::SendMessage(hWnd,WM_NOTIFY,(WPARAM)nmctbCustomize.nmhdr.idFrom,
		(LPARAM)&nmctbCustomize);
}


CString COXCoolToolBar::GetRecentButtonText(UINT nCommandID)
{
	CString sText(_T(""));
	m_mapButtonText.Lookup(nCommandID,sText);
	return sText;
}

#if _MFC_VER>=0x0420
////////////////////
void COXCoolToolBar::EraseNonClient()
{
	// get window DC that is clipped to the non-client area
	CWindowDC dc(this);
	CRect rectClient;
	GetClientRect(rectClient);
	CRect rectWindow;
	GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	rectClient.OffsetRect(-rectWindow.left, -rectWindow.top);
	rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);

	// draw borders in non-client area
	if(!IsCool())
	{
		// only if CoolToolBar is not used in CoolBar
		DrawBorders(&dc, rectWindow);

		// fixing for spurious edges
		if((m_dwStyle&CBRS_BORDER_3D)==0)
		{
			rectWindow.InflateRect(1,1);
		}
	}

	// erase parts not drawn
	dc.IntersectClipRect(rectWindow);
	dc.ExcludeClipRect(rectClient);
	SendMessage(WM_ERASEBKGND, (WPARAM) dc.m_hDC);
}

void COXCoolToolBar::DrawGripper(CDC& dc, CRect& rect)
{ 
	GetToolbarSkin()->DrawGripper(&dc, rect, this);
}

void COXCoolToolBar::DrawSeparator(CDC& dc, CRect& rc)
{
	GetToolbarSkin()->DrawSeparator(&dc, rc, this);
}

void COXCoolToolBar::DrawIcon(CDC& dc, CRect& rect)
{
	if(m_hIcon==NULL)
		return;

	m_iconRect=rect;

	if(m_dwStyle&CBRS_ORIENT_HORZ) 
	{
		if(m_iconRect.left==0 && !IsFloating())
		{
			m_iconRect.left += 10;
			m_iconRect.top=(rect.Height()-::GetSystemMetrics(SM_CYSMICON))/2;
		}
		else
		{
			m_iconRect.left += 3;
			m_iconRect.top = 3;
		}

		m_iconRect.right=m_iconRect.left+::GetSystemMetrics(SM_CXSMICON);
		m_iconRect.bottom=m_iconRect.top+::GetSystemMetrics(SM_CYSMICON);
		rect.left=m_iconRect.right+1;
	} 
	else 
	{
		if(m_iconRect.top==0 && !IsFloating())
			m_iconRect.top += 10;
		else
			m_iconRect.top += 3;

		m_iconRect.bottom=m_iconRect.top+::GetSystemMetrics(SM_CYSMICON);
		m_iconRect.left=(rect.Width()-::GetSystemMetrics(SM_CXSMICON))/2;
		m_iconRect.right=m_iconRect.left+::GetSystemMetrics(SM_CXSMICON);

		rect.top=m_iconRect.bottom+1;
	}

	::DrawIconEx(dc,m_iconRect.left,m_iconRect.top,m_hIcon,
		m_iconRect.Width(),m_iconRect.Height(),0,NULL,DI_NORMAL);
}

void COXCoolToolBar::DrawCustomizedButton(CDC& dc, CRect& rect)
{
	GetToolbarSkin()->DrawCustomizedButton(&dc, rect, this);
}

void COXCoolToolBar::SetDropDownArrow(BOOL bDropDownArrow) 
{ 
	if(m_dwComCtlVersion>=_IE40_COMCTL_VERSION)
	{
		DWORD dwStyleEx=GetStyleEx();
		dwStyleEx=bDropDownArrow ? dwStyleEx|TBSTYLE_EX_DRAWDDARROWS : 
			dwStyleEx&~TBSTYLE_EX_DRAWDDARROWS;
		SetStyleEx(dwStyleEx); 
	}

	m_bDropDownArrow=bDropDownArrow;
}

BOOL COXCoolToolBar::IsDropDownArrow() const
{ 
	if(m_dwComCtlVersion>=_IE40_COMCTL_VERSION)
	{
		return GetStyleEx()&TBSTYLE_EX_DRAWDDARROWS ? TRUE : FALSE; 
	}

	return m_bDropDownArrow;
}

////////////////////
#endif

void COXCoolToolBar::SetCustomizable(BOOL bCustomizable/*=TRUE*/,
									 BOOL bAdvanced/*=TRUE*/)
{
	// set CoolToolBar customizable only if array of bitmap IDs was previously set 
	// by function SetBitmapIds()

	if(m_pBitmapIds==NULL)
	{
		bCustomizable=FALSE;
	}
	
	if(bCustomizable && !bAdvanced)
		ModifyStyle(0,CCS_ADJUSTABLE);
	else
		ModifyStyle(CCS_ADJUSTABLE,0);

	m_bAdvancedCutomizable=bCustomizable&bAdvanced;
}


void COXCoolToolBar::SetBitmapIds(UINT* pIds, int nButtons)
{
	m_pBitmapIds=pIds;
	m_nBitmapButtons=nButtons;

	// if we call this function then we probably want our CoolToolBar 
	// to be customizable
	SetCustomizable();
}


int COXCoolToolBar::FindBitmapIndex(UINT nID) const
{
	// helper function to find the number of element in array of bitmap IDs
	// by its ID
	ASSERT(m_pBitmapIds != NULL);
	for (int i = 0; i < m_nBitmapButtons ; i++)
	{
		if (m_pBitmapIds[i] == (int)nID)
			return i;
	}
	return -1;
}


void COXCoolToolBar::SetAdvancedCustomizationMode(BOOL bInAdvancedCustomizationMode,
												  HWND hWndCustomizeOrganizer/*=NULL*/) 
{ 
	if(bInAdvancedCustomizationMode && !IsCustomizable(TRUE))
	{
		SetCustomizable(TRUE,TRUE);
	}
	if(!bInAdvancedCustomizationMode)
	{
		SetCustomizedButton(-1);
	}
	m_hWndCustomizeOrganizer=hWndCustomizeOrganizer;
	m_bInAdvancedCustomizationMode=bInAdvancedCustomizationMode;

	// reset the customization mode for custom button windows
	POSITION pos=m_mapCustomButtons.GetStartPosition();
	while(pos!=NULL)
	{
		int nIndex=-1;
		OXCUSTOMBUTTONDESCRIPTOR descriptor;
		m_mapCustomButtons.GetNextAssoc(pos,nIndex,descriptor);
		ASSERT(nIndex>=0 && nIndex<GetToolBarCtrl().GetButtonCount());
		descriptor.m_pCBTemplate->CBSetCustomizationMode(bInAdvancedCustomizationMode);
	}
}


// Return information for bitmap indexes in the toolbar
BOOL COXCoolToolBar::OnTBNGetButtonInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	TBNOTIFY* pTBN=(TBNOTIFY*)pNMHDR;
	
	int nIndex=pTBN->iItem;
	if(nIndex<m_nBitmapButtons)
	{
		*pResult=TRUE;

		// find button's ID by its image index
		UINT nButtonId=m_pBitmapIds[nIndex];
		pTBN->tbButton.iBitmap=nIndex;
		pTBN->tbButton.idCommand=nButtonId;
		pTBN->tbButton.fsState=TBSTATE_ENABLED;
		pTBN->tbButton.fsStyle=TBSTYLE_BUTTON;
		pTBN->tbButton.iString=-1;		
		// use as tooltip as text associated with button shown in Customize dialog
		if(pTBN->pszText!=NULL)
		{		
			CString strText;
			if(strText.LoadString(nButtonId))
			{
				// tool tip is after "\n" in the string
				LPCTSTR pTipText=_tcschr(strText,_T('\n'));   
				if(pTipText!=NULL)
				{	
					const int len = (int)_tcslen(pTipText+1);

					UTBStr::tcsncpy(pTBN->pszText,len+1,pTipText+1,len);
					return TRUE;
				}
			}
			TRACE(_T("COXCoolToolBar:No Tooltip prompt for ID=%d\n"),nButtonId);
			UTBStr::tcsncpy(pTBN->pszText,4,_T("???"),3);
		}
	}
	else 
	{
		*pResult=FALSE;
	}

	return TRUE;
}


BOOL COXCoolToolBar::OnTBNBeginDrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTOOLBAR* pNMToolBar=(NMTOOLBAR*)pNMHDR;

	BOOL bCustomButton=IsCustomButtonWnd(pNMToolBar->iItem);
	int nIndex=(!bCustomButton ? CommandToIndex(pNMToolBar->iItem) : 
		GetCustomButtonPosition(pNMToolBar->iItem));
	ASSERT((nIndex>=0 && nIndex<GetToolBarCtrl().GetButtonCount()));

#ifdef OX_CUSTOMIZE_COMMANDS
	if(IsInAdvancedCustomizationMode() || (IsCustomizable(TRUE) && 
		((GetStyle()&TBSTYLE_ALTDRAG && ::GetKeyState(VK_MENU)<0) ||
		(!(GetStyle()&TBSTYLE_ALTDRAG) && ::GetKeyState(VK_SHIFT)<0))))
	{
		if(!bCustomButton)
		{
			int nState=GetToolBarCtrl().GetState(GetItemID(nIndex));
			if((nState&TBSTATE_PRESSED)!=0)
			{
				GetToolBarCtrl().SetState(GetItemID(nIndex),nState&~TBSTATE_PRESSED);
			}
		}

		CWnd* pParentWnd=GetParent();
		ASSERT(pParentWnd!=NULL);
		pNMToolBar->hdr.code=TBN_QUERYDELETE;
		if(pParentWnd->
			SendMessage(WM_NOTIFY,pNMToolBar->hdr.idFrom,(LPARAM)pNMToolBar))
		{
			BOOL bWasInAdvancedCustomizationMode=IsInAdvancedCustomizationMode();
			if(GetCustomizedButton()!=nIndex)
			{
				SetCustomizedButton(nIndex);
			}
			SetDraggedButton(nIndex);

			// mark the control as the one that launched drag'n'drop operation
			m_bDragDropOwner=TRUE;

			TBBUTTON button;
			VERIFY(GetToolBarCtrl().GetButton(nIndex,&button));
			COleDataSource* pDataSource=
				COXDragDropCommands::PrepareDragDropData(GetButtonText(nIndex),
				button.iBitmap,button.idCommand,button.fsStyle);
			ASSERT(pDataSource!=NULL);

			// analyze current mouse position
			CPoint ptMouse;
			::GetCursorPos(&ptMouse);
			CRect rectWindow;
			GetWindowRect(rectWindow);
			if(!rectWindow.PtInRect(ptMouse))
			{
				m_bDragDropOperation=TRUE;
			}
			
			DROPEFFECT dropEffect=
				COXDragDropCommands::DoDragDrop(pDataSource,GetDropSource());
			if((DROPEFFECT_MOVE==dropEffect && m_bDragDropOperation) || 
				(DROPEFFECT_NONE==dropEffect &&	::GetKeyState(VK_LBUTTON)>=0 && 
				m_bDragDropOperation))
			{
				int nDraggedButtonIndex=GetDraggedButton();
				OnRemoveDraggedButton(nDraggedButtonIndex);
			}
			SetDraggedButton(-1);
			m_bDragDropOperation=FALSE;

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


BOOL COXCoolToolBar::OnTBNEndDrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTOOLBAR* pNMToolBar=(NMTOOLBAR*)pNMHDR;
	UNREFERENCED_PARAMETER(pNMToolBar);
	*pResult=0;
	return FALSE;
}


BOOL COXCoolToolBar::OnTBNQueryInsert(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTOOLBAR* pNMToolBar=(NMTOOLBAR*)pNMHDR;
	UNREFERENCED_PARAMETER(pNMToolBar);
	*pResult=TRUE;		// always allow buttons to be inserted
	return TRUE;
}


BOOL COXCoolToolBar::OnTBNQueryDelete(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	*pResult=TRUE;		// always allow buttons to be deleted
	return TRUE;
}


BOOL COXCoolToolBar::OnTBNToolBarChange(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	SetButtonTextFromID(m_ttID);
	return FALSE;
}

#if _MFC_VER>=0x0420
//////////////////////
// custom draw of a toolbar is available since MFC 4.2
// in you derived class you can provide your own custom draw routines
void COXCoolToolBar::OnCustomDraw(NMHDR* pNotify, LRESULT* pResult)
{
	GetToolbarSkin()->OnCustomDraw(pNotify, pResult, this);
}
//////////////////////
#endif

void COXCoolToolBar::OnRemoveDraggedButton(int nButtonIndex)
{
	// delete button if it was moved
	VERIFY(GetToolBarCtrl().DeleteButton(nButtonIndex));
}

// helper function to keep array of all used bitmaps
HBITMAP COXCoolToolBar::AddBitmap(LPTSTR lpszResourceName)
{
	HBITMAP hBitmap=NULL;
	if(!m_allBitmaps.Lookup(lpszResourceName,hBitmap))
	{
		if((hBitmap=::LoadBitmap(AfxGetResourceHandle(),
			lpszResourceName))==NULL)
		{
			TRACE(_T("COXCoolToolBar::AddBitmap: unable to load bitmap\n"));
			return NULL;
		}
		m_allBitmaps.SetAt(lpszResourceName,hBitmap);
	}

	return hBitmap;
}

// helper function to keep array of all used image lists
HIMAGELIST COXCoolToolBar::AddImageList(LPTSTR lpszResourceName, int cx, int cGrow, 
										COLORREF crMask, UINT uType, UINT uFlags)
{
	HANDLE hImageList=NULL;
	if(!m_allImageLists.Lookup(lpszResourceName,hImageList))
	{
		if((hImageList=(HANDLE)::ImageList_LoadImage(AfxGetResourceHandle(),
			lpszResourceName,cx,cGrow,crMask,uType,uFlags))==NULL)
		{
			TRACE(_T("COXCoolToolBar::AddImageList: unable to load image list\n"));
			return NULL;
		}
		m_allImageLists.SetAt(lpszResourceName,hImageList);
	}

	return (HIMAGELIST)hImageList;
}

// helper function to keep array of all used image lists
HIMAGELIST COXCoolToolBar::AddImageList(LPTSTR lpszResourceName, int cx, 
										COLORREF crMask, UINT uFlags, 
										int cInitial, int cGrow)
{
	HANDLE hImageList=NULL;
	if(!m_allImageLists.Lookup(lpszResourceName,hImageList))
	{
		CBitmap bitmap;
		if(!bitmap.LoadBitmap(lpszResourceName))
		{
			TRACE(_T("COXCoolToolBar::AddImageList: unable to load image\n"));
			return NULL;
		}
		BITMAP bitmapInfo;
		VERIFY(bitmap.GetBitmap(&bitmapInfo)!=0);
		if((hImageList=(HANDLE)::ImageList_Create(cx,bitmapInfo.bmHeight,
			uFlags,cInitial,cGrow))==NULL)
		{
			TRACE(_T("COXCoolToolBar::AddImageList: unable to create image list\n"));
			return NULL;
		}
		if(ImageList_AddMasked((HIMAGELIST)hImageList,bitmap,crMask)==-1)
		{
			TRACE(_T("COXCoolToolBar::AddImageList: unable to populate image list\n"));
			ImageList_Destroy((HIMAGELIST)hImageList);
			return NULL;
		}
		m_allImageLists.SetAt(lpszResourceName,hImageList);
	}

	return (HIMAGELIST)hImageList;
}


// helper function to automatically set text to buttons
//	TTID_PLAIN - lookup string resource for var equals button's ID
//			if found, set text in button to string text 
//			(not includded tool tip text)
//	TTID_TOOLTIP - lookup string resource for var equals button's ID
//			if found, set text in button to tooltip text 
//	TTID_MENU - lookup menu resource for var equals button's ID
//			if found, set text in button to text of corresponding Item in menu
//	TTID_NONE - remove all text associated with buttons
//	TTID_NOTSET - default value (there is no any text associated with toolbar)
void COXCoolToolBar::SetButtonTextFromID(UINT nFirstButtonID, 
										 UINT nLastButtonID, TextTypeFromID ttID)
{
	m_ttID=ttID;

	if(ttID==TTID_NOTSET)
	{
		return;
	}

	ASSERT(nFirstButtonID<=nLastButtonID);
	CToolBarCtrl* pToolBarCtrl = &GetToolBarCtrl();
	ASSERT(pToolBarCtrl != NULL);

	BOOL bMenuLoaded=FALSE;
	CMenu* pMenu=NULL;
	if(ttID==TTID_MENU)
	{
		// try to get a menu associated with framework
		if(AfxGetApp()->m_pMainWnd!=NULL)
		{
			if(AfxGetApp()->m_pMainWnd->GetMenu()!=NULL)
			{
				pMenu=AfxGetApp()->m_pMainWnd->GetMenu();
				bMenuLoaded=TRUE;
			}
			else
			{
				TRACE(_T("COXCoolToolBar::SetButtonTextFromID: cannot get framework menu!/n"));
			}
		}
		else
		{
			TRACE(_T("COXCoolToolBar::SetButtonTextFromID: m_pMainWnd of app is not defined yet menu!/n"));
		}
	}
	
	UINT nID;
	CString strText;
	for(int i=0; i<pToolBarCtrl->GetButtonCount(); i++)
	{
		nID=GetItemID(i);
		if(nID>=nFirstButtonID && nID<=nLastButtonID)
		{
			switch(ttID)
			{
			// text in string resourse (not included tooltip)
			case TTID_PLAIN:
				{
					if (strText.LoadString(nID))
					{
						int nPlacement=strText.Find(_T('\n'));   // tool tip is after "\n" in the string
						if (nPlacement!=-1)
						{	
							strText=strText.Left(nPlacement);
						}
					}
					break;
				}
			// text in string resourse (only tooltip)
			case TTID_TOOLTIP:
				{
					if (strText.LoadString(nID))
					{
						int nPlacement=strText.Find(_T('\n'));   // tool tip is after "\n" in the string
						if (nPlacement!=-1)
						{	
							strText=strText.Mid(nPlacement+1);
						}
						else
						{
							strText=_T("");
						}
					}
					break;
				}
			// text in menu item resourse
			case TTID_MENU:
				{
					if(bMenuLoaded)
					{
						ASSERT(pMenu!=NULL);
						pMenu->GetMenuString(nID,strText,MF_BYCOMMAND);
						if(!strText.IsEmpty())
						{
							int nPlacement=strText.Find(_T('\t'));   // accelerator keys are placed after "\t" 
							if (nPlacement!=-1)
							{	
								strText=strText.Left(nPlacement);
							}
						}
					}
					break;
				}
			// remove any text
			case TTID_NONE:
				{
					strText=_T("");
					break;
				}
			default:
				{
					m_ttID=TTID_NONE;
					strText=_T("");
					break;
				}
			}
			SetButtonText(i,strText);
		}
	}
}

// Save to registry state of buttons in CoolToolBar
BOOL COXCoolToolBar::SaveBarState(LPCTSTR lpszSubKey, LPCTSTR lpszValueName, 
								  BOOL bProperties)
{
#ifndef _MAC
	CWinApp* pApp=AfxGetApp();
	CString sProfileName;
	sProfileName.Format(_T("%s_%s"),lpszSubKey,lpszValueName);

	// make sure you called SetButtonsIds()  
	// makes no sense to load/store buttons state info for uncustomizable CoolToolBar
	if(IsCustomizable(TRUE) || IsCustomizable(FALSE))
	{
		ASSERT(m_pBitmapIds!=NULL);

		// make sure you called CWinApp::SetRegistryKey() functions before
		if(pApp->m_pszRegistryKey==NULL || pApp->m_pszProfileName==NULL)
		{
			TRACE(_T("COXCoolToolBar::SaveBarState: haven't called	SetRegistryKey()\n"));
			return FALSE;
		}
		// we use default registry key assigned to your application by MFC
		HKEY hSecKey=pApp->GetSectionKey(_T(""));
		if (hSecKey==NULL)
		{
			TRACE(_T("COXCoolToolBar::SaveBarState: unable to get section key\n"));
			return FALSE;
		}

		// save buttons settings
		//
		CString sValueName;
		sValueName.Format(_T("%s_ButtonsSettings"),lpszValueName);
		
		CMemFile memFile;
		CArchive ar(&memFile,CArchive::store);
		int nButtonCount=GetToolBarCtrl().GetButtonCount();
		ar<<(DWORD)nButtonCount;
		for(int nIndex=0; nIndex<nButtonCount; nIndex++)
		{
			TBBUTTON button;
			VERIFY(GetToolBarCtrl().GetButton(nIndex,&button));

			ar<<(DWORD)button.idCommand;
			ar<<(DWORD)button.iBitmap;
			ar<<button.fsStyle;

			CString sText=GetButtonText(nIndex);
			ar<<(sText.IsEmpty() ? FALSE : TRUE);
			if(sText.IsEmpty())
				sText=GetRecentButtonText(button.idCommand);
			ar<<sText;
		}
		ar.Close();
		int nBufferLength=(int)memFile.GetLength();
		ASSERT(nBufferLength>0);
		BYTE* pBuffer=memFile.Detach();

		pApp->WriteProfileBinary(lpszSubKey,sValueName,pBuffer,nBufferLength);
		::free((void*)pBuffer);

		sValueName+=_T("_Size");
		pApp->WriteProfileInt(lpszSubKey,sValueName,nBufferLength);


		// save custom buttons positions
		//
		sValueName.Format(_T("%s_CustomButtonsPositions"),lpszValueName);
		CMemFile memFileCustom;
		CArchive arCustom(&memFileCustom,CArchive::store);
		int nCustomButtonCount=PtrToInt(m_mapCustomButtons.GetCount());
		arCustom<<(DWORD)nCustomButtonCount;
		for(int nCustomButtonIndex=0; 
			nCustomButtonIndex<GetToolBarCtrl().GetButtonCount(); 
			nCustomButtonIndex++)
		{
			OXCUSTOMBUTTONDESCRIPTOR descriptor;
			if(GetCustomButton(nCustomButtonIndex,descriptor))
			{
				arCustom<<(DWORD)nCustomButtonIndex;
				arCustom<<
					(DWORD)descriptor.m_pCBTemplate->CBGetWindow()->GetDlgCtrlID();
			}
		}

		arCustom.Close();
		nBufferLength=(int)memFileCustom.GetLength();
		ASSERT(nBufferLength>0);
		pBuffer=memFileCustom.Detach();

		pApp->WriteProfileBinary(lpszSubKey,sValueName,pBuffer,nBufferLength);
		::free((void*)pBuffer);

		sValueName+=_T("_Size");
		pApp->WriteProfileInt(lpszSubKey,sValueName,nBufferLength);


		// save buttons state
		GetToolBarCtrl().SaveState(hSecKey,lpszSubKey,lpszValueName);
		::RegCloseKey(hSecKey);
	}

	// save custom buttons settings
	//
	if(!m_bCustomButtonsStateSaved)
	{
		CString sValueName=_T("Toolbars_CustomButtonsSettings");
		
		CMemFile memFile;
		CArchive ar(&memFile,CArchive::store);
		int nButtonCount=PtrToInt(m_mapAllCustomButtons.GetCount());
		ar<<(DWORD)nButtonCount;
		POSITION pos=m_mapAllCustomButtons.GetStartPosition();
		while(pos!=NULL)
		{
			int nCtrlID=-1;
			OXCUSTOMBUTTONDESCRIPTOR descriptor;
			m_mapAllCustomButtons.GetNextAssoc(pos,nCtrlID,descriptor);
			ASSERT(nCtrlID>0);
			ar<<(DWORD)nCtrlID;
			descriptor.SaveState(ar);
			descriptor.m_pCBTemplate->CBSaveState(ar);
		}
		ar.Close();
		int nBufferLength=(int)memFile.GetLength();
		ASSERT(nBufferLength>0);
		BYTE* pBuffer=memFile.Detach();

		pApp->WriteProfileBinary(lpszSubKey,sValueName,pBuffer,nBufferLength);
		::free((void*)pBuffer);

		sValueName+=_T("_Size");
		pApp->WriteProfileInt(lpszSubKey,sValueName,nBufferLength);

		m_bCustomButtonsStateSaved=TRUE;
	}


	// save CoolToolBar properties
	if(bProperties)
	{
#if _MFC_VER>=0x0420
		pApp->WriteProfileInt(sProfileName,szCool,IsCool());
		pApp->WriteProfileInt(sProfileName,szGripper,IsGripper());
		pApp->WriteProfileInt(sProfileName,szSeparator,IsSeparator());
		pApp->WriteProfileInt(sProfileName,szFlat,IsFlat());
		pApp->WriteProfileInt(sProfileName,szList,IsList());
		pApp->WriteProfileInt(sProfileName,szDropDownArrow,IsDropDownArrow());
		pApp->WriteProfileInt(sProfileName,szIndent,GetIndent());
		pApp->WriteProfileInt(sProfileName,szDefaultTextColor,GetDefaultTextColor());
		pApp->WriteProfileInt(sProfileName,szHotTextColor,GetHotTextColor());
		pApp->WriteProfileInt(sProfileName,szSelectedTextColor,GetSelectedTextColor());
		pApp->WriteProfileInt(sProfileName,szCheckedTextColor,GetCheckedTextColor());
		pApp->WriteProfileInt(sProfileName,szDefaultBkColor,GetDefaultBkColor());
		pApp->WriteProfileInt(sProfileName,szHotBkColor,GetHotBkColor());
		pApp->WriteProfileInt(sProfileName,szSelectedBkColor,GetSelectedBkColor());
		pApp->WriteProfileInt(sProfileName,szCheckedBkColor,GetCheckedBkColor());
		pApp->WriteProfileInt(sProfileName,szDefaultBorderColor,GetDefaultBorderColor());
		pApp->WriteProfileInt(sProfileName,szHotBorderColor,GetHotBorderColor());
		pApp->WriteProfileInt(sProfileName,szSelectedBorderColor,GetSelectedBorderColor());
		pApp->WriteProfileInt(sProfileName,szCheckedBorderColor,GetCheckedBorderColor());
		pApp->WriteProfileInt(sProfileName,szTextType,GetTextType());
		pApp->WriteProfileInt(sProfileName,szTextRows,GetTextRows());
		pApp->WriteProfileInt(sProfileName,szMinButtonsWidth,GetButtonsMinMaxWidth().cx);
		pApp->WriteProfileInt(sProfileName,szMaxButtonsWidth,GetButtonsMinMaxWidth().cy);
#endif
		pApp->WriteProfileInt(sProfileName,szCustomizable,IsCustomizable());
	}

	return TRUE;
#else
	return FALSE;
#endif
}

// Load from registry state of buttons in CoolToolBar
BOOL COXCoolToolBar::LoadBarState(LPCTSTR lpszSubKey, LPCTSTR lpszValueName, 
								  BOOL bProperties)
{
#ifndef _MAC
	CWinApp* pApp=AfxGetApp();
	CString sProfileName;
	sProfileName.Format(_T("%s_%s"),lpszSubKey,lpszValueName);
	int nResult;

	if(bProperties)
	{
		nResult=pApp->GetProfileInt(sProfileName,szCustomizable,-1);
		if(nResult!=-1)
		{
			SetCustomizable(nResult);
		}
	}

	BOOL bUpdate=FALSE;

	// load and apply CoolToolBar properties
	if(bProperties)
	{
#if _MFC_VER>=0x0420
		nResult=pApp->GetProfileInt(sProfileName,szCool,-1);
		if(nResult!=-1)
		{
			SetCool(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szGripper,-1);
		if(nResult!=-1)
		{
			SetGripper(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szSeparator,-1);
		if(nResult!=-1)
		{
			SetSeparator(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szFlat,-1);
		if(nResult!=-1)
		{
			SetFlat(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szList,-1);
		if(nResult!=-1)
		{
			SetList(nResult);
			bUpdate=TRUE;
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szDropDownArrow,-1);
		if(nResult!=-1)
		{
			SetDropDownArrow(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szIndent,-1);
		if(nResult!=-1)
		{
			SetIndent(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szDefaultTextColor,-1);
		if(nResult!=-1)
		{
			SetDefaultTextColor(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szHotTextColor,-1);
		if(nResult!=-1)
		{
			SetHotTextColor(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szSelectedTextColor,-1);
		if(nResult!=-1)
		{
			SetSelectedTextColor(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szCheckedTextColor,-1);
		if(nResult!=-1)
		{
			SetCheckedTextColor(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szDefaultBkColor,-1);
		if(nResult!=-1)
		{
			SetDefaultBkColor(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szHotBkColor,-1);
		if(nResult!=-1)
		{
			SetHotBkColor(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szSelectedBkColor,-1);
		if(nResult!=-1)
		{
			SetSelectedBkColor(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szCheckedBkColor,-1);
		if(nResult!=-1)
		{
			SetCheckedBkColor(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szDefaultBorderColor,-1);
		if(nResult!=-1)
		{
			SetDefaultBorderColor(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szHotBorderColor,-1);
		if(nResult!=-1)
		{
			SetHotBorderColor(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szSelectedBorderColor,-1);
		if(nResult!=-1)
		{
			SetSelectedBorderColor(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szCheckedBorderColor,-1);
		if(nResult!=-1)
		{
			SetCheckedBorderColor(nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szTextType,-1);
		if(nResult!=-1 && nResult!=GetTextType())
		{
			if(nResult==TTID_NOTSET)
			{
				SetButtonTextFromID(TTID_NONE);
			}
			bUpdate=TRUE;
			SetButtonTextFromID((TextTypeFromID)nResult);
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szTextRows,-1);
		if(nResult!=-1 && nResult!=GetTextRows())
		{
			SetMaxTextRows(nResult);
			bUpdate=TRUE;
		}
	
		nResult=pApp->GetProfileInt(sProfileName,szMinButtonsWidth,-1);
		if(nResult!=-1 && nResult!=m_sizeMinMaxWidth.cx)
		{
			m_sizeMinMaxWidth.cx=nResult;
			SetButtonsMinMaxWidth(m_sizeMinMaxWidth.cx,m_sizeMinMaxWidth.cy);
		}
		nResult=pApp->GetProfileInt(sProfileName,szMaxButtonsWidth,-1);
		if(nResult!=-1 && nResult!=m_sizeMinMaxWidth.cy)
		{
			m_sizeMinMaxWidth.cy=nResult;
			SetButtonsMinMaxWidth(m_sizeMinMaxWidth.cx,m_sizeMinMaxWidth.cy);
			bUpdate=TRUE;
		}

#endif
	}


	// load custom button settings
	//
	if(!m_bCustomButtonsStateLoaded)
	{
		CString sValueName=_T("Toolbars_CustomButtonsSettings_Size");
		int nSavedBufferLength=pApp->GetProfileInt(lpszSubKey,sValueName,-1);
		if(nSavedBufferLength!=-1)
		{
			ASSERT(nSavedBufferLength>0);
			UINT nBufferLength=0;
			BYTE* pBuffer=NULL;
			sValueName=_T("Toolbars_CustomButtonsSettings");
			if(pApp->GetProfileBinary(lpszSubKey,sValueName,&pBuffer,&nBufferLength))
			{
				ASSERT(nBufferLength>0);
				ASSERT(pBuffer!=NULL);
				CMemFile memFile(pBuffer,nBufferLength);
				CArchive ar(&memFile,CArchive::load);
				DWORD dwButtonCount=0;
				ar>>dwButtonCount;
				for(int nIndex=0; nIndex<(int)dwButtonCount; nIndex++)
				{
					DWORD dwCtrlID;
					ar>>dwCtrlID;
					OXCUSTOMBUTTONDESCRIPTOR descriptor;
					VERIFY(m_mapAllCustomButtons.Lookup((int)dwCtrlID,descriptor));
					descriptor.LoadState(ar);
					descriptor.m_pCBTemplate->CBLoadState(ar);
					m_mapAllCustomButtons.SetAt((int)dwCtrlID,descriptor);
				}
				ar.Close();
				memFile.Detach();

				delete[] pBuffer;
			}
		}

		m_bCustomButtonsStateLoaded=TRUE;
	}


	// make sure you called SetButtonsIds()  
	// makes no sense to load/store buttons state info for uncustomizable CoolToolBar
	//
	// load and apply button settings
	if(IsCustomizable(FALSE) || IsCustomizable(TRUE))
	{
		ASSERT(m_pBitmapIds!=NULL);
		// make sure you called CWinApp::SetRegistryKey() functions before
		if(pApp->m_pszRegistryKey==NULL || pApp->m_pszProfileName==NULL)
		{
			TRACE(_T("COXCoolToolBar::LoadBarState: haven't called	SetRegistryKey()\n"));
			return FALSE;
		}
		// we use default registry key assigned to your application by MFC
		HKEY hSecKey=pApp->GetSectionKey(_T(""));
		if (hSecKey==NULL)
		{
			TRACE(_T("COXCoolToolBar::LoadBarState: unable to get section key\n"));
			return FALSE;
		}

		// before restoring state of buttons we should remove all custom buttons
		// and store them temporary in mapDeletedCustomButtons map
		CMap<int,int,OXCUSTOMBUTTONDESCRIPTOR,OXCUSTOMBUTTONDESCRIPTOR&> 
			mapDeletedCustomButtons;
		// check if any custom buttons info has been saved
		CString sValueName;
		sValueName.Format(_T("%s_CustomButtonsPositions_Size"),lpszValueName);
		int nSavedBufferLength=pApp->GetProfileInt(lpszSubKey,sValueName,-1);
		if(nSavedBufferLength>0)
		{
			for(int nIndex=GetToolBarCtrl().GetButtonCount()-1; nIndex>=0; nIndex--)
			{
				OXCUSTOMBUTTONDESCRIPTOR descriptor;
				if(GetCustomButton(nIndex,descriptor))
				{
					ASSERT(::IsWindow(
						descriptor.m_pCBTemplate->CBGetWindow()->GetSafeHwnd()));
					int nButtonCtrlID=
						descriptor.m_pCBTemplate->CBGetWindow()->GetDlgCtrlID();
					if(descriptor.m_pCBTemplate->CBIsDynamicallyCreated())
					{
						// this button will be deleted later
						descriptor.m_pCBTemplate=NULL;
					}
					mapDeletedCustomButtons.SetAt(nButtonCtrlID,descriptor);
					GetToolBarCtrl().DeleteButton(nIndex);
				}
			}
		}


		m_bNoInternalRedraw=TRUE;

		// restore the toolbar contents
		GetToolBarCtrl().RestoreState(hSecKey,lpszSubKey,lpszValueName);
		::RegCloseKey(hSecKey);


		// apply saved button settings
		//
		sValueName.Format(_T("%s_ButtonsSettings_Size"),lpszValueName);
		nSavedBufferLength=pApp->GetProfileInt(lpszSubKey,sValueName,-1);
		if(nSavedBufferLength!=-1)
		{
			ASSERT(nSavedBufferLength>0);
			UINT nBufferLength=0;
			BYTE* pBuffer=NULL;
			sValueName.Format(_T("%s_ButtonsSettings"),lpszValueName);
			if(pApp->GetProfileBinary(lpszSubKey,sValueName,&pBuffer,&nBufferLength))
			{
				ASSERT(nBufferLength>0);
				ASSERT(pBuffer!=NULL);
				CMemFile memFile(pBuffer,nBufferLength);
				CArchive ar(&memFile,CArchive::load);
				DWORD dwButtonCount=0;
				ar>>dwButtonCount;
				for(int nIndex=0; nIndex<(int)dwButtonCount; nIndex++)
				{
					DWORD dwCommandID;
					ar>>dwCommandID;
					DWORD dwImageIndex;
					ar>>dwImageIndex;
					BYTE fsStyle;
					ar>>fsStyle;

					BOOL bSetText;
					ar>>bSetText;
					CString sText;
					ar>>sText;

					if(fsStyle&TBSTYLE_SEP)
						continue;

					int nButtonIndex=CommandToIndex((UINT)dwCommandID);
					if(nButtonIndex!=-1)
					{
						SetButtonInfo(nButtonIndex,(UINT)dwCommandID,
							fsStyle,(int)dwImageIndex);
						if(bSetText)
						{
							SetButtonText(nButtonIndex,sText);
						}
						else
						{
							if(!sText.IsEmpty())
								m_mapButtonText.SetAt((UINT)dwCommandID,sText);
						}
					}
				}
				ar.Close();
				memFile.Detach();

				delete[] pBuffer;
			}
		}
	
		// apply saved custom button positions
		//
		sValueName.Format(_T("%s_CustomButtonsPositions_Size"),lpszValueName);
		nSavedBufferLength=pApp->GetProfileInt(lpszSubKey,sValueName,-1);
		if(nSavedBufferLength!=-1)
		{
			ASSERT(nSavedBufferLength>0);
			UINT nBufferLength=0;
			BYTE* pBuffer=NULL;
			sValueName.Format(_T("%s_CustomButtonsPositions"),lpszValueName);
			if(pApp->GetProfileBinary(lpszSubKey,sValueName,&pBuffer,&nBufferLength))
			{
				ASSERT(nBufferLength>0);
				ASSERT(pBuffer!=NULL);
				CMemFile memFile(pBuffer,nBufferLength);
				CArchive ar(&memFile,CArchive::load);
				DWORD dwButtonCount=0;
				ar>>dwButtonCount;
				for(int nIndex=0; nIndex<(int)dwButtonCount; nIndex++)
				{
					DWORD dwIndex;
					ar>>dwIndex;
					DWORD dwCtrlID;
					ar>>dwCtrlID;

					ASSERT((int)dwIndex>=0 && 
						(int)dwIndex<=GetToolBarCtrl().GetButtonCount());
					OXCUSTOMBUTTONDESCRIPTOR descriptor;
					if(mapDeletedCustomButtons.Lookup((int)dwCtrlID,descriptor) && 
						descriptor.m_pCBTemplate!=NULL && ::IsWindow(
						descriptor.m_pCBTemplate->CBGetWindow()->GetSafeHwnd()))
					{
						OXCUSTOMBUTTONDESCRIPTOR descriptorOrig;
						VERIFY(m_mapAllCustomButtons.
							Lookup((int)dwCtrlID,descriptorOrig));
						// add placeholder
						TBBUTTON button={ 0 };
						button.iBitmap=-1;
						button.idCommand=0;
						button.fsStyle=TBSTYLE_SEP;
						button.iString=-1;
						VERIFY(GetToolBarCtrl().InsertButton((int)dwIndex,&button));
						// add new custom button
						m_mapCustomButtons.SetAt((int)dwIndex,descriptorOrig);
					}
					else
					{
						int nCustomButtonIndex=GetCustomButtonIndex((int)dwCtrlID);
						ASSERT(nCustomButtonIndex!=-1);
						VERIFY(InsertCustomButton((int)dwIndex,nCustomButtonIndex));
					}
				}
				ar.Close();
				memFile.Detach();

				delete[] pBuffer;
			}
		}
	
		SetButtonTextFromID(m_ttID);
	}

	m_bNoInternalRedraw=FALSE;

	// change the size of CoolToolBar
	if(bUpdate)
	{
		if(GetTextRows()==0)
		{
			IniSizes(m_sizeImage);
		}
		else
		{
			UpdateSizes();
		}

		RedrawToolBar();
	}
	else
	{
		UpdateCustomButtons();
	}


	return TRUE;

#else
	return FALSE;
#endif
}

void COXCoolToolBar::DockControlBarLeftOf(CToolBar *leftOf)
{
	ASSERT_VALID(this);
	// make sure CControlBar::EnableDocking has been called
	ASSERT(m_pDockContext!=NULL);

	if(leftOf!=NULL)
	{
		ASSERT_VALID(leftOf);
	}

	CFrameWnd* pFrameWnd=GetDockingFrame();
	ASSERT_VALID(pFrameWnd);

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	pFrameWnd->RecalcLayout();

	UINT nDockBarID=0;
	CRect rect(0,0,0,0);
	if(leftOf!=NULL)
	{
		leftOf->GetWindowRect(&rect);
		rect.OffsetRect(1,0);

		DWORD dwStyle=leftOf->GetBarStyle();
		nDockBarID=(dwStyle & CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP :
			(dwStyle & CBRS_ALIGN_BOTTOM) ? AFX_IDW_DOCKBAR_BOTTOM :
			(dwStyle & CBRS_ALIGN_LEFT) ? AFX_IDW_DOCKBAR_LEFT :
			(dwStyle & CBRS_ALIGN_RIGHT) ? AFX_IDW_DOCKBAR_RIGHT : 0;
	}


	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line. Calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	pFrameWnd->DockControlBar(this, nDockBarID, &rect);
}

#if _MFC_VER<=0x0421

#define OXCY_BORDER		1

void COXCoolToolBar::SetHeight(int cyHeight)
{
	ASSERT_VALID(this);

	int nHeight = cyHeight;
	if (m_dwStyle & CBRS_BORDER_TOP)
		cyHeight -= OXCY_BORDER;
	if (m_dwStyle & CBRS_BORDER_BOTTOM)
		cyHeight -= OXCY_BORDER;
	m_cyBottomBorder = (cyHeight - m_sizeButton.cy) / 2;
	// if there is an extra pixel, m_cyTopBorder will get it
	m_cyTopBorder = cyHeight - m_sizeButton.cy - m_cyBottomBorder;
	if (m_cyTopBorder < 0)
	{
		TRACE(_T("Warning: COXCoolToolBar::SetHeight(%d) is smaller than button.\n"),
			nHeight);
		m_cyBottomBorder += m_cyTopBorder;
		m_cyTopBorder = 0;  // will clip at bottom
	}

	// recalculate the non-client region
	SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_DRAWFRAME|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
	Invalidate();   // just to be nice if called when toolbar is visible
}
#endif

BOOL COXCoolToolBar::IsWindowsNTRunning()
{
	BOOL bResult=FALSE;

	OSVERSIONINFO verInfo;
	verInfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	if (::GetVersionEx(&verInfo))
	{
		if (verInfo.dwPlatformId==VER_PLATFORM_WIN32_NT && 
			verInfo.dwMajorVersion>=4)
		{
			bResult=TRUE;
		}
	}

	return bResult;
}

int COXCoolToolBar::GetDropDownArrowWidth() const
{
#if _MFC_VER>=0x0420
	HDC hDC = ::GetDC(NULL);
	ASSERT(hDC != NULL);

	HFONT hFont;
	HFONT oldFont=NULL;
	int nDropDownArrowWidth=0;
	if((hFont=CreateFont(GetSystemMetrics(SM_CYMENUCHECK), 0, 0, 0, 
		FW_NORMAL, 0, 0, 0, SYMBOL_CHARSET, 0, 0, 0, 0, _T("Marlett")))!=NULL)
	{
		oldFont = (HFONT)SelectObject(hDC, hFont);
	}
	
	VERIFY(GetCharWidth(hDC, '6', '6', &nDropDownArrowWidth));

	if(oldFont!=NULL)
	{
		SelectObject(hDC, oldFont);
	}
	if(hFont!=NULL)
	{
		DeleteObject(hFont);
	}
	::ReleaseDC(NULL, hDC);
	
	return nDropDownArrowWidth;
#else
	return 0;
#endif
}

#if _MFC_VER>=0x0420
////////////////////////////////
void COXCoolToolBar::_GetButton(int nIndex, TBBUTTON* pButton) const
{
	COXCoolToolBar* pBar = (COXCoolToolBar*)this;
	VERIFY(pBar->DefWindowProc(TB_GETBUTTON, nIndex, (LPARAM)pButton));
	// TBSTATE_ENABLED == TBBS_DISABLED so invert it
	pButton->fsState ^= TBSTATE_ENABLED;
}

void COXCoolToolBar::_SetButton(int nIndex, TBBUTTON* pButton)
{
	// get original button state
	TBBUTTON button;
	VERIFY(DefWindowProc(TB_GETBUTTON, nIndex, (LPARAM)&button));

	// prepare for old/new button comparsion
	button.bReserved[0] = 0;
	button.bReserved[1] = 0;
	// TBSTATE_ENABLED == TBBS_DISABLED so invert it
	pButton->fsState ^= TBSTATE_ENABLED;
	pButton->bReserved[0] = 0;
	pButton->bReserved[1] = 0;

	// nothing to do if they are the same
	if (memcmp(pButton, &button, sizeof(TBBUTTON)) != 0)
	{
		// don't redraw anything while setting the button
		BOOL bWasVisible=(GetStyle()&WS_VISIBLE);
		if(bWasVisible)
			ModifyStyle(WS_VISIBLE, 0);
		VERIFY(DefWindowProc(TB_DELETEBUTTON, nIndex, 0));
		VERIFY(DefWindowProc(TB_INSERTBUTTON, nIndex, (LPARAM)pButton));
		if(bWasVisible)
		{
			ModifyStyle(0,WS_VISIBLE);
		}

		// invalidate appropriate parts
		if (((pButton->fsStyle ^ button.fsStyle) & TBSTYLE_SEP) ||
			((pButton->fsStyle & TBSTYLE_SEP) && pButton->iBitmap != button.iBitmap))
		{
			// changing a separator
			Invalidate(FALSE);
		}
		else
		{
			// invalidate just the button
			CRect rect;
			if (DefWindowProc(TB_GETITEMRECT, nIndex, (LPARAM)&rect))
				InvalidateRect(rect, FALSE);    // don't erase background
		}
	}
}

#ifdef _MAC
	#define CX_OVERLAP  1
#else
	#define CX_OVERLAP  0
#endif

int COXCoolToolBar::WrapToolBar(TBBUTTON* pData, int nCount, int nWidth)
{
	ASSERT(pData != NULL && nCount > 0);

	int nResult = 0;
	int x = 0;
	for (int i = 0; i < nCount; i++)
	{
		pData[i].fsState &= ~TBSTATE_WRAP;

		if (pData[i].fsState & TBSTATE_HIDDEN)
			continue;

		int dx, dxNext;

		// take into account custom button style
		if(pData[i].fsStyle & TBSTYLE_SEP)
		{
			dx = pData[i].iBitmap;
			dxNext = dx;
		}
		else
		{
			dx = m_sizeButton.cx;
			if(pData[i].fsStyle & TBSTYLE_AUTOSIZE)
			{
				CRect rect;
				GetToolBarCtrl().GetItemRect(i,&rect);
				dx=rect.Width();
			}
			// check for dropdown style, but only if the buttons are being drawn
			if((pData[i].fsStyle & TBSTYLE_DROPDOWN) && IsDropDownArrow() &&
				(pData[i].fsStyle & TBSTYLE_AUTOSIZE)==0)
			{
				if(!IsFlat() || m_dwComCtlVersion>=_IE40_COMCTL_VERSION)
				{
					// add size of drop down
					if(m_dwComCtlVersion<_IE40_COMCTL_VERSION)
					{
						m_nDropDownArrowWidth=GetDropDownArrowWidth();
					}
					dx += m_nDropDownArrowWidth;
				}
			}
			dxNext = dx - CX_OVERLAP;
		}

		if(x + dx > nWidth)
		{
			BOOL bFound = FALSE;
			for (int j = i; j >= 0  &&  !(pData[j].fsState & TBSTATE_WRAP); j--)
			{
				// Find last separator that isn't hidden
				// a separator that has a command ID is not
				// a separator, but a custom control.
				if((pData[j].fsStyle & TBSTYLE_SEP) &&
					(pData[j].idCommand == 0) &&
					!(pData[j].fsState & TBSTATE_HIDDEN))
				{
					bFound = TRUE; i = j; x = 0;
					pData[j].fsState |= TBSTATE_WRAP;
					nResult++;
					break;
				}
			}
			if(!bFound)
			{
				for (int j = i - 1; j >= 0 && 
					!(pData[j].fsState & TBSTATE_WRAP); j--)
				{
					// Never wrap anything that is hidden,
					// or any custom controls
					if ((pData[j].fsState & TBSTATE_HIDDEN) || 
						((pData[j].fsStyle & TBSTYLE_SEP) && 
						(pData[j].idCommand != 0)))
					{
						continue;
					}

					bFound = TRUE; i = j; x = 0;
					pData[j].fsState |= TBSTATE_WRAP;
					nResult++;
					break;
				}
				if(!bFound)
				{
					x += dxNext;
				}
			}
		}
		else
		{
			x += dxNext;
		}
	}
	return nResult + 1;
}

void COXCoolToolBar::SizeToolBar(TBBUTTON* pData, int nCount, 
								 int nLength, BOOL bVert/*=FALSE*/)
{
	ASSERT(pData != NULL && nCount > 0);

	if (!bVert)
	{
		int nMin, nMax, nTarget, nCurrent, nMid;

		// Wrap ToolBar as specified
		nMax = nLength;
		nTarget = WrapToolBar(pData, nCount, nMax);

		// Wrap ToolBar vertically
		nMin = 0;
		nCurrent = WrapToolBar(pData, nCount, nMin);

		if (nCurrent != nTarget)
		{
			while (nMin < nMax)
			{
				nMid = (nMin + nMax) / 2;
				nCurrent = WrapToolBar(pData, nCount, nMid);

				if (nCurrent == nTarget)
					nMax = nMid;
				else
				{
					if (nMin == nMid)
					{
						WrapToolBar(pData, nCount, nMax);
						break;
					}
					nMin = nMid;
				}
			}
		}
		CSize size = CalcSize(pData, nCount);
		WrapToolBar(pData, nCount, size.cx);
	}
	else
	{
		CSize sizeMax, sizeMin, sizeMid;

		// Wrap ToolBar vertically
		WrapToolBar(pData, nCount, 0);
		sizeMin = CalcSize(pData, nCount);

		// Wrap ToolBar horizontally
		WrapToolBar(pData, nCount, 32767);
		sizeMax = CalcSize(pData, nCount);

		while (sizeMin.cx < sizeMax.cx)
		{
			sizeMid.cx = (sizeMin.cx + sizeMax.cx) / 2;
			WrapToolBar(pData, nCount, sizeMid.cx);
			sizeMid = CalcSize(pData, nCount);

			if (nLength < sizeMid.cy)
			{
				if (sizeMin == sizeMid)
				{
					WrapToolBar(pData, nCount, sizeMax.cx);
					return;
				}
				sizeMin = sizeMid;
			}
			else if (nLength > sizeMid.cy)
				sizeMax = sizeMid;
			else
				return;
		}
	}
}

CSize COXCoolToolBar::CalcSize(TBBUTTON* pData, int nCount)
{
	ASSERT(pData != NULL && nCount > 0);

	CPoint cur(0,0);
	CSize sizeResult(0,0);

	for (int i = 0; i < nCount; i++)
	{
		int cySep = pData[i].iBitmap;
		if(!IsFlat() || m_dwComCtlVersion<_IE40_COMCTL_VERSION)
		{
			cySep = cySep * 2 / 3;
		}

		if(pData[i].fsState & TBSTATE_HIDDEN)
			continue;

		int cx = m_sizeButton.cx;
		if(pData[i].fsStyle & TBSTYLE_AUTOSIZE)
		{
			CRect rect;
			GetToolBarCtrl().GetItemRect(i,&rect);
			cx=rect.Width();
		}

		if(pData[i].fsStyle & TBSTYLE_SEP)
		{
			// a separator represents either a height or width
			if (pData[i].fsState & TBSTATE_WRAP)
			{
				sizeResult.cy = __max(cur.y + m_sizeButton.cy + cySep, 
					sizeResult.cy);
			}
			else
			{
				sizeResult.cx = __max(cur.x + pData[i].iBitmap, sizeResult.cx);
				sizeResult.cy = __max(cur.y + m_sizeButton.cy, sizeResult.cy);
			}
		}
		else
		{
			// check for dropdown style, but only if the buttons are being drawn
			if((pData[i].fsStyle & TBSTYLE_DROPDOWN) && IsDropDownArrow() &&
				(pData[i].fsStyle & TBSTYLE_AUTOSIZE)==0)
			{
				if(!IsFlat() || m_dwComCtlVersion>=_IE40_COMCTL_VERSION)
				{
					// add size of drop down
					if(m_dwComCtlVersion<_IE40_COMCTL_VERSION)
					{
						m_nDropDownArrowWidth=GetDropDownArrowWidth();
					}
					cx += m_nDropDownArrowWidth;
				}
			}
			sizeResult.cx = __max(cur.x + cx, sizeResult.cx);
			sizeResult.cy = __max(cur.y + m_sizeButton.cy, sizeResult.cy);
		}

		if(pData[i].fsStyle & TBSTYLE_SEP)
		{
			cur.x += pData[i].iBitmap;
		}
		else
		{
			cur.x += cx - CX_OVERLAP;
		}

		if(pData[i].fsState & TBSTATE_WRAP)
		{
			cur.x = 0;
			cur.y += m_sizeButton.cy;
			if(pData[i].fsStyle & TBSTYLE_SEP)
				cur.y += cySep;
		}
	}

	// add indention
	sizeResult.cx+=m_nIndent;
	return sizeResult;
}

struct _AFX_CONTROLPOS
{
	int nIndex, nID;
	CRect rectOldPos;
};

CSize COXCoolToolBar::CalcLayout(DWORD dwMode, int nLength)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	if(dwMode & LM_HORZDOCK)
	{
		ASSERT(dwMode & LM_HORZ);		
	}

	int nCount;
	TBBUTTON* pData=NULL;
	CMap<int,int,TBBUTTON*,TBBUTTON*> mapIndexToIgnore;
	CSize sizeResult(0,0);

	// Load Buttons
	{
		nCount = PtrToInt(DefWindowProc(TB_BUTTONCOUNT, 0, 0));
		if (nCount != 0)
		{
			int nFlags=(((dwMode & LM_HORZDOCK)==0 && (dwMode & LM_VERTDOCK)==0) ? 
				OXCBD_SHOWFLOAT : ((dwMode & LM_HORZDOCK)!=0 ? 
				OXCBD_SHOWHORZ : OXCBD_SHOWVERT));
			int i;
			pData=new TBBUTTON[nCount];
			for(i=0; i<nCount; i++)
			{
				_GetButton(i,&pData[i]);

				// take into account custom button style
				OXCUSTOMBUTTONDESCRIPTOR descriptor;
				if(GetCustomButton(i,descriptor))
				{
					if(((descriptor.m_nFlags & OXCBD_SHOWFLOAT)==0 && 
						nFlags==OXCBD_SHOWFLOAT) ||
						((descriptor.m_nFlags & OXCBD_SHOWHORZ)==0 && 
						nFlags==OXCBD_SHOWHORZ) ||
						((descriptor.m_nFlags & OXCBD_SHOWVERT)==0 && 
						nFlags==OXCBD_SHOWVERT))
					{
						TBBUTTON* pButton=new TBBUTTON;
						*pButton=pData[i];
						pData[i].fsStyle&=~TBSTYLE_SEP;
						pData[i].fsStyle|=TBSTYLE_BUTTON;
						pData[i].iBitmap=descriptor.m_nImageIndex;
						mapIndexToIgnore.SetAt(i,pButton);
					}
				}
			}
		}
	}

	if (nCount > 0)
	{
		if (!(m_dwStyle & CBRS_SIZE_FIXED))
		{
			BOOL bDynamic = m_dwStyle & CBRS_SIZE_DYNAMIC;

			ASSERT(pData!=NULL);

			if (bDynamic && (dwMode & LM_MRUWIDTH))
				SizeToolBar(pData, nCount, m_nMRUWidth);
			else if (bDynamic && (dwMode & LM_HORZDOCK))
				SizeToolBar(pData, nCount, 32767);
			else if (bDynamic && (dwMode & LM_VERTDOCK))
				SizeToolBar(pData, nCount, 0);
			else if (bDynamic && (nLength != -1))
			{
				CRect rect; rect.SetRectEmpty();
				CalcInsideRect(rect, (dwMode & LM_HORZ));
				BOOL bVert = (dwMode & LM_LENGTHY);
				int nLen = nLength + (bVert ? rect.Height() : rect.Width());

				SizeToolBar(pData, nCount, nLen, bVert);
			}
			else if (bDynamic && (m_dwStyle & CBRS_FLOATING))
				SizeToolBar(pData, nCount, m_nMRUWidth);
			else
				SizeToolBar(pData, nCount, (dwMode & LM_HORZ) ? 32767 : 0);
		}

		sizeResult = CalcSize(pData, nCount);

		if (dwMode & LM_COMMIT)
		{
			_AFX_CONTROLPOS* pControl = NULL;
			int nControlCount = 0;
			BOOL bIsDelayed = m_bDelayedButtonLayout;
			m_bDelayedButtonLayout = FALSE;

			int i = 0;
			for(i = 0; i < nCount; i++)
			{
				if ((pData[i].fsStyle & TBSTYLE_SEP) && (pData[i].idCommand != 0))
				{
					nControlCount++;
				}
			}

			if (nControlCount > 0)
			{
				pControl = new _AFX_CONTROLPOS[nControlCount];
				nControlCount = 0;

				for(int i = 0; i < nCount; i++)
				{
					if ((pData[i].fsStyle & TBSTYLE_SEP) && (pData[i].idCommand != 0))
					{
						pControl[nControlCount].nIndex = i;
						pControl[nControlCount].nID = pData[i].idCommand;

						CRect rect;
						GetItemRect(i, &rect);
						ClientToScreen(&rect);
						pControl[nControlCount].rectOldPos = rect;

						nControlCount++;
					}
				}
			}

			if ((m_dwStyle & CBRS_FLOATING) && (m_dwStyle & CBRS_SIZE_DYNAMIC))
			{
				m_nMRUWidth = sizeResult.cx;
			}
			for (i = 0; i < nCount; i++)
			{
				TBBUTTON* pButton=NULL;
				if(mapIndexToIgnore.Lookup(i,pButton))
				{
					ASSERT(pButton!=NULL);
					if(pData[i].fsState & TBSTATE_WRAP)
					{
						pButton->fsState|=TBSTATE_WRAP;
					}
					else
					{
						pButton->fsState&=~TBSTATE_WRAP;
					}
					_SetButton(i, pButton);
				}
				else
				{
					_SetButton(i, &pData[i]);
				}
			}

			if (nControlCount > 0)
			{
				for (int i = 0; i < nControlCount; i++)
				{
					CWnd* pWnd = GetDlgItem(pControl[i].nID);
					if (pWnd != NULL)
					{
						CRect rect;
						pWnd->GetWindowRect(&rect);
						CPoint pt = rect.TopLeft() - pControl[i].rectOldPos.TopLeft();
						GetItemRect(pControl[i].nIndex, &rect);
						pt = rect.TopLeft() + pt;
						pWnd->SetWindowPos(NULL, pt.x, pt.y, 0, 0, 
							SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
					}
				}
				delete[] pControl;
			}
			m_bDelayedButtonLayout = bIsDelayed;
		}
		delete[] pData;
	}
	else
	{
		CSize sizeButtons=GetButtonsSize();
		if(dwMode & LM_HORZ)
		{
			if(IsFloating())
			{
				sizeResult.cx=sizeButtons.cx;
			}
			else
			{
				sizeResult.cx=sizeButtons.cx/2;
			}
			sizeResult.cy=sizeButtons.cy;
		}
		else
		{
			sizeResult.cx=sizeButtons.cx;
			if(IsFloating())
			{
				sizeResult.cy=sizeButtons.cy;
			}
			else
			{
				sizeResult.cy=sizeButtons.cy/2;
			}
		}
	}

	POSITION pos=mapIndexToIgnore.GetStartPosition();
	while(pos!=NULL)
	{
		int nIndex=0;
		TBBUTTON* pButton=NULL;
		mapIndexToIgnore.GetNextAssoc(pos,nIndex,pButton);
		ASSERT(pButton!=NULL);
		delete pButton;
	}
	mapIndexToIgnore.RemoveAll();

	//BLOCK: Adjust Margins
	{
		CRect rect; rect.SetRectEmpty();
		CalcInsideRect(rect, (dwMode & LM_HORZ));
		sizeResult.cy -= rect.Height();
		sizeResult.cx -= rect.Width();

		CSize size = CControlBar::CalcFixedLayout((dwMode & LM_STRETCH), 
			(dwMode & LM_HORZ));
		sizeResult.cx = __max(sizeResult.cx, size.cx);
		sizeResult.cy = __max(sizeResult.cy, size.cy);
	}

	return sizeResult;
}

CSize COXCoolToolBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	DWORD dwMode = bStretch ? LM_STRETCH : 0;
	dwMode |= bHorz ? LM_HORZ : 0;

	return CalcLayout(dwMode);
}

CSize COXCoolToolBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	if ((nLength == -1) && !(dwMode & LM_MRUWIDTH) && !(dwMode & LM_COMMIT) &&
		((dwMode & LM_HORZDOCK) || (dwMode & LM_VERTDOCK)))
	{
		return CalcFixedLayout(dwMode & LM_STRETCH, dwMode & LM_HORZDOCK);
	}
	return CalcLayout(dwMode, nLength);
}

void COXCoolToolBar::CalcInsideRect(CRect& rect, BOOL bHorz)
{
	CToolBar::CalcInsideRect(rect,bHorz);

	if(m_hIcon)
	{
		// Icon size
		CSize szIcon(::GetSystemMetrics(SM_CXSMICON),
			::GetSystemMetrics(SM_CYSMICON));
		if(bHorz)
			rect.left+=szIcon.cx+2;
		else
			rect.top+=szIcon.cy+2;
	}

	CRect rectBookedSpace(0,0,0,0);
	BookSpace(rectBookedSpace,(bHorz ? LM_HORZ : 0));
	rect.left+=rectBookedSpace.left;
	rect.top+=rectBookedSpace.top;
	rect.right-=rectBookedSpace.right;
	rect.bottom-=rectBookedSpace.bottom;

	if(IsFlat() && IsGripper() && !(m_dwStyle & CBRS_FLOATING))
	{
		if(bHorz)
		{
			rect.left+=ID_OXGRIPPER_WIDTH;
		}
		else
		{
			rect.top+=ID_OXGRIPPER_WIDTH;
		}
	}
}


void COXCoolToolBar::RedrawToolBar(BOOL bRecalcLayout/*=TRUE*/, 
								   BOOL bOnlyFrame/*=FALSE*/)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	if(m_bNoInternalRedraw)
	{
		return;
	}

	// update custom buttons state
	UpdateCustomButtons();

	if(!IsWindowVisible())
	{
		return;
	}

	if(bRecalcLayout)
	{
		CFrameWnd* pFrameWnd=GetDockingFrame();
		if(pFrameWnd!=NULL)
		{
			m_bNoBkgndRedraw=TRUE;
			pFrameWnd->RecalcLayout();
			m_bNoBkgndRedraw=FALSE;
			for(int nIndex=0; nIndex<GetToolBarCtrl().GetButtonCount(); nIndex++)
			{
				RedrawButton(nIndex);
				CRect rect;
				GetItemRect(nIndex,rect);
				ValidateRect(rect);
			}
		}
	}
	else
	{
		if(!bOnlyFrame)
		{
			RedrawWindow(NULL,NULL,
				RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_FRAME|RDW_ALLCHILDREN);
		}
	}

	if(bOnlyFrame)
	{
		SetWindowPos(NULL,0,0,0,0,
			SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_DRAWFRAME);
	}
}


void COXCoolToolBar::RedrawButton(int nIndex)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	if(nIndex<0 || nIndex>GetToolBarCtrl().GetButtonCount())
	{
		return;
	}

	CRect rect;
	GetItemRect(nIndex,rect);
	RedrawWindow(rect);
	if(IsCustomButton(nIndex))
	{
		GetCustomButtonWnd(nIndex)->RedrawWindow(rect);
	}
}


CString COXCoolToolBar::GetButtonTooltip(int nButtonIndex) const
{ 
	ASSERT(::IsWindow(m_hWnd));

	CString sTooltip(_T(""));
	UINT nID=GetItemID(nButtonIndex); 
	if(nID!=0) // will be zero on a separator
	{
		TCHAR szFullText[256];
		// don't handle the message if no string resource found
		if(AfxLoadString(nID,szFullText))
		{
			AfxExtractSubString(sTooltip,szFullText,1,'\n');
		}
	}

	return sTooltip;
}


BOOL COXCoolToolBar::MoveButton(UINT nOldPos, UINT nNewPos)
{ 
#if _MFC_VER>=0x0420
	ASSERT(::IsWindow(m_hWnd)); 
	if((BOOL)::SendMessage(m_hWnd,TB_MOVEBUTTON,nOldPos,nNewPos))
	{
		if(IsCustomButton(nNewPos))
		{
			RedrawToolBar(FALSE,TRUE);
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
#else
	if(nOldPos==nNewPos)
		return TRUE;
	TBBUTTON button;
	if(!GetToolBarCtrl().GetButton(nOldPos,&button))
		return FALSE;
	if(!GetToolBarCtrl().DeleteButton(nOldPos))
		return FALSE;
	if(nNewPos>nOldPos)
		nNewPos--;
	return (GetToolBarCtrl().InsertButton(nNewPos,&button))
#endif
}


int COXCoolToolBar::AddCustomButton(COXCustomTBButtonTemplate* pCBTemplate, int nID, 
									int nWidth, int nHeight/*=-1*/, 
									int nFlags/*=OXCBD_SHOWANY*/, 
									int nImageIndex/*=-1*/)
{
	OXCUSTOMBUTTONDESCRIPTOR 
		descriptor(pCBTemplate,nID,nWidth,nHeight,nFlags,nImageIndex);
	return AddCustomButton(descriptor);
}


int COXCoolToolBar::AddCustomButton(OXCUSTOMBUTTONDESCRIPTOR& descriptor)
{
	ASSERT(descriptor.m_pCBTemplate!=NULL);
	ASSERT(!::IsWindow(descriptor.m_pCBTemplate->CBGetWindow()->GetSafeHwnd()));
	
	OXCUSTOMBUTTONDESCRIPTOR descriptorTest;
	if(!m_mapAllCustomButtons.Lookup(descriptor.m_nID,descriptorTest))
	{
		// add new custom button
		m_mapAllCustomButtons.SetAt(descriptor.m_nID,descriptor);
		return PtrToInt(m_arrAllCustomButtonIDs.Add(descriptor.m_nID));
	}
	else
	{
		// custom button has already been inserted
		int nCustomButtonIndex=GetCustomButtonIndex(descriptor.m_nID);
		ASSERT(nCustomButtonIndex!=-1);
		if(!SetCustomButton(nCustomButtonIndex,descriptor))
		{
			TRACE(_T("COXCoolToolBar::AddCustomButton: failed to update the descriptor of existing custom button\n"));
			return -1;
		}
		else
		{
			return nCustomButtonIndex;
		}
	}
}


BOOL COXCoolToolBar::SetCustomButton(int nCustomButtonIndex, 
									 OXCUSTOMBUTTONDESCRIPTOR& descriptor)
{
	if(nCustomButtonIndex<0 || nCustomButtonIndex>=m_arrAllCustomButtonIDs.GetSize())
	{
		TRACE(_T("COXCoolToolBar::SetCustomButton: specified custom button index is out of range\n"));
		return FALSE;
	}

	int nID=m_arrAllCustomButtonIDs[nCustomButtonIndex];
	ASSERT(nID==descriptor.m_nID);
	OXCUSTOMBUTTONDESCRIPTOR descriptorOrig;
	VERIFY(m_mapAllCustomButtons.Lookup(nID,descriptorOrig));
	// copy properties to original button descriptor
	VERIFY(descriptorOrig.CopyProperties(descriptor));
	m_mapAllCustomButtons.SetAt(nID,descriptorOrig);

	// apply the same properties for all copies of specified button
	// Iterate all the control bars and use only the toolbars
	CFrameWnd* pFrameWnd=DYNAMIC_DOWNCAST(CFrameWnd,AfxGetMainWnd());
	if(pFrameWnd!=NULL)
	{
		POSITION pos=pFrameWnd->m_listControlBars.GetHeadPosition();
		while(pos!=NULL)
		{
			COXCoolToolBar* pCoolToolbar=DYNAMIC_DOWNCAST(COXCoolToolBar, 
				(CControlBar*)pFrameWnd->m_listControlBars.GetNext(pos));
			//If it is a COXCoolToolBar
			if(pCoolToolbar!=NULL)
			{
				POSITION pos=pCoolToolbar->m_mapCustomButtons.GetStartPosition();
				while(pos!=NULL)
				{
					int nIndex=-1;
					OXCUSTOMBUTTONDESCRIPTOR descriptorExisting;
					pCoolToolbar->
						m_mapCustomButtons.GetNextAssoc(pos,nIndex,descriptorExisting);
					ASSERT(nIndex!=-1);
					if(descriptorExisting.m_nID==descriptor.m_nID)
					{
						VERIFY(descriptorExisting.CopyProperties(descriptor));
						pCoolToolbar->
							m_mapCustomButtons.SetAt(nIndex,descriptorExisting);
						pCoolToolbar->RedrawToolBar(TRUE,TRUE);
						break;
					}
				}
			}
		}
	}

	return TRUE;
}	


BOOL COXCoolToolBar::CreateCustomButton(int nCustomButtonIndex, 
										OXCUSTOMBUTTONDESCRIPTOR& descriptor,
										DWORD dwStyle/*=WS_CHILD|WS_VISIBLE*/, 
										DWORD dwStyleEx/*=0*/, 
										LPCTSTR lpszClassName/*=AfxRegisterWndClass(CS_DBLCLKS)*/,
										LPCTSTR lpszWindowName/*=_T("")*/)
{
	if(nCustomButtonIndex<0 || nCustomButtonIndex>=m_arrAllCustomButtonIDs.GetSize())
	{
		TRACE(_T("COXCoolToolBar::CreateCustomButton: specified custom button index is out of range\n"));
		return FALSE;
	}

	int nID=m_arrAllCustomButtonIDs[nCustomButtonIndex];
	OXCUSTOMBUTTONDESCRIPTOR descriptorOrig;
	VERIFY(m_mapAllCustomButtons.Lookup(nID,descriptorOrig));

	// check if any instance of this button has already been created
	if(!::IsWindow(descriptorOrig.m_pCBTemplate->CBGetWindow()->GetSafeHwnd()))
	{
		if(!descriptorOrig.m_pCBTemplate->CBCreate(descriptorOrig.m_nID,this,dwStyle,
			dwStyleEx,lpszClassName,lpszWindowName))
		{
			TRACE(_T("COXCoolToolBar::CreateCustomButton: failed to create custom button\n"));
			return FALSE;
		}
		m_mapAllCustomButtons.SetAt(descriptorOrig.m_nID,descriptorOrig);

		descriptor=descriptorOrig;
	}
	else
	{
		descriptor=descriptorOrig;
		// create a copy of the existing button
		descriptor.m_pCBTemplate=
			(COXCustomTBButtonWnd<CWnd>*)descriptorOrig.m_pCBTemplate->
			CBCreateCopy(this);
		if(descriptor.m_pCBTemplate==NULL)
		{
			TRACE(_T("COXCoolToolBar::CreateCustomButton: failed to create custom button\n"));
			return FALSE;
		}
	}

	return TRUE;
}


BOOL COXCoolToolBar::InsertCustomButton(int nIndex, int nCustomButtonIndex,
										DWORD dwStyle/*=WS_CHILD|WS_VISIBLE*/, 
										DWORD dwStyleEx/*=0*/, 
										LPCTSTR lpszClassName/*=AfxRegisterWndClass(CS_DBLCLKS)*/,
										LPCTSTR lpszWindowName/*=_T("")*/)
{
	if(nIndex<0 || nIndex>GetToolBarCtrl().GetButtonCount())
	{
		TRACE(_T("COXCoolToolBar::InsertCustomButton: specified index is out of range\n"));
		return FALSE;
	}

	if(nCustomButtonIndex<0 || nCustomButtonIndex>=m_arrAllCustomButtonIDs.GetSize())
	{
		TRACE(_T("COXCoolToolBar::InsertCustomButton: specified custom button index is out of range\n"));
		return FALSE;
	}

	// add placeholder
	TBBUTTON button={ 0 };
	button.iBitmap=-1;
	button.idCommand=0;
	button.fsStyle=TBSTYLE_SEP;
	button.iString=-1;
	// don't redraw the toolbar contents, we will do it later
	m_bNoInternalRedraw=TRUE;
	if(!GetToolBarCtrl().InsertButton(nIndex,&button))
	{
		m_bNoInternalRedraw=FALSE;
		TRACE(_T("COXCoolToolBar::InsertCustomButton: failed to insert new button\n"));
		return FALSE;
	}
	m_bNoInternalRedraw=FALSE;

	// create new custom button
	OXCUSTOMBUTTONDESCRIPTOR descriptorCreated;
	if(!CreateCustomButton(nCustomButtonIndex,descriptorCreated,dwStyle,
		dwStyleEx,lpszClassName,lpszWindowName))
	{
		TRACE(_T("COXCoolToolBar::InsertCustomButton: failed to create custom button\n"));
		// don't redraw the toolbar contents, we just rollback to the original state
		m_bNoInternalRedraw=TRUE;
		GetToolBarCtrl().DeleteButton(nIndex);
		m_bNoInternalRedraw=FALSE;
		return FALSE;
	}

	// add new custom button
	m_mapCustomButtons.SetAt(nIndex,descriptorCreated);

	RedrawToolBar(TRUE,TRUE);

	return TRUE;
}


int COXCoolToolBar::GetCustomButtonPosition(int nCtrlID) const
{
	POSITION pos=m_mapCustomButtons.GetStartPosition();
	while(pos!=NULL)
	{
		int nIndex=-1;
		OXCUSTOMBUTTONDESCRIPTOR descriptor;
		m_mapCustomButtons.GetNextAssoc(pos,nIndex,descriptor);
		ASSERT(nIndex!=-1);
		ASSERT(::IsWindow(descriptor.m_pCBTemplate->CBGetWindow()->GetSafeHwnd()));
		if(descriptor.m_pCBTemplate->CBGetWindow()->GetDlgCtrlID()==nCtrlID)
		{
			return nIndex;
		}
	}

	return -1;
}


BOOL COXCoolToolBar::IsCustomButtonWnd(int nCtrlID) const
{
	POSITION pos=m_mapCustomButtons.GetStartPosition();
	while(pos!=NULL)
	{
		int nIndex=-1;
		OXCUSTOMBUTTONDESCRIPTOR descriptor;
		m_mapCustomButtons.GetNextAssoc(pos,nIndex,descriptor);
		ASSERT(nIndex!=-1);
		ASSERT(::IsWindow(descriptor.m_pCBTemplate->CBGetWindow()->GetSafeHwnd()));
		if(descriptor.m_pCBTemplate->CBGetWindow()->GetDlgCtrlID()==nCtrlID)
		{
			return TRUE;
		}
	}

	return FALSE;
}


BOOL COXCoolToolBar::InsertComboBox(int nIndex, int nCustomButtonIndex, 
									DWORD dwStyle/*=WS_CHILD|WS_VISIBLE|CBS_DROPDOWN|WS_VSCROLL*/, 
									DWORD dwStyleEx/*=0*/)
{
#ifdef _DEBUG
	int nID=m_arrAllCustomButtonIDs[nCustomButtonIndex];
	OXCUSTOMBUTTONDESCRIPTOR descriptorOrig;
	VERIFY(m_mapAllCustomButtons.Lookup(nID,descriptorOrig));
	ASSERT_KINDOF(CComboBox,descriptorOrig.m_pCBTemplate->CBGetWindow());
#endif	//	_DEBUG
	return InsertCustomButton(
		nIndex,nCustomButtonIndex,dwStyle,dwStyleEx,_T("ComboBox"));
}


BOOL COXCoolToolBar::InsertEditBox(int nIndex, int nCustomButtonIndex, 
								   LPCTSTR lpszWindowName/*=_T("")*/,
								   DWORD dwStyle/*=WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL*/, 
								   DWORD dwStyleEx/*=WS_EX_CLIENTEDGE*/)
{
#ifdef _DEBUG
	int nID=m_arrAllCustomButtonIDs[nCustomButtonIndex];
	OXCUSTOMBUTTONDESCRIPTOR descriptorOrig;
	VERIFY(m_mapAllCustomButtons.Lookup(nID,descriptorOrig));
	ASSERT_KINDOF(CEdit,descriptorOrig.m_pCBTemplate->CBGetWindow());
#endif	//	_DEBUG
	return InsertCustomButton(
		nIndex,nCustomButtonIndex,dwStyle,dwStyleEx,_T("Edit"),lpszWindowName);
}


BOOL COXCoolToolBar::InsertRichEditBox(int nIndex, int nCustomButtonIndex,
									   DWORD dwStyle/*=WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL*/, 
									   DWORD dwStyleEx/*=WS_EX_CLIENTEDGE*/)
{
#ifdef _DEBUG
	int nID=m_arrAllCustomButtonIDs[nCustomButtonIndex];
	OXCUSTOMBUTTONDESCRIPTOR descriptorOrig;
	VERIFY(m_mapAllCustomButtons.Lookup(nID,descriptorOrig));
	ASSERT_KINDOF(CRichEditCtrl,descriptorOrig.m_pCBTemplate->CBGetWindow());
#endif	//	_DEBUG
	return InsertCustomButton(
		nIndex,nCustomButtonIndex,dwStyle,dwStyleEx,_T("RICHEDIT"));
}


BOOL COXCoolToolBar::InsertStaticCtrl(int nIndex, int nCustomButtonIndex, 
									  LPCTSTR lpszWindowName, 
									  DWORD dwStyle/*=WS_CHILD|WS_VISIBLE|SS_LEFT|SS_SIMPLE*/, 
									  DWORD dwStyleEx/*=0*/)
{
#ifdef _DEBUG
	int nID=m_arrAllCustomButtonIDs[nCustomButtonIndex];
	OXCUSTOMBUTTONDESCRIPTOR descriptorOrig;
	VERIFY(m_mapAllCustomButtons.Lookup(nID,descriptorOrig));
	ASSERT_KINDOF(CStatic,descriptorOrig.m_pCBTemplate->CBGetWindow());
#endif	//	_DEBUG
	return InsertCustomButton(
		nIndex,nCustomButtonIndex,dwStyle,dwStyleEx,_T("Static"),lpszWindowName);
}


BOOL COXCoolToolBar::InsertHotKeyCtrl(int nIndex, int nCustomButtonIndex, 
									  DWORD dwStyle/*=WS_CHILD|WS_VISIBLE*/, 
									  DWORD dwStyleEx/*=0*/)
{
#ifdef _DEBUG
	int nID=m_arrAllCustomButtonIDs[nCustomButtonIndex];
	OXCUSTOMBUTTONDESCRIPTOR descriptorOrig;
	VERIFY(m_mapAllCustomButtons.Lookup(nID,descriptorOrig));
	ASSERT_KINDOF(CHotKeyCtrl,descriptorOrig.m_pCBTemplate->CBGetWindow());
#endif	//	_DEBUG
	return InsertCustomButton(
		nIndex,nCustomButtonIndex,dwStyle,dwStyleEx,_T("msctls_hotkey32"));
}


BOOL COXCoolToolBar::InsertButtonCtrl(int nIndex, int nCustomButtonIndex, 
									  LPCTSTR lpszWindowName, 
									  DWORD dwStyle/*=WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON*/, 
									  DWORD dwStyleEx/*=0*/)
{
#ifdef _DEBUG
	int nID=m_arrAllCustomButtonIDs[nCustomButtonIndex];
	OXCUSTOMBUTTONDESCRIPTOR descriptorOrig;
	VERIFY(m_mapAllCustomButtons.Lookup(nID,descriptorOrig));
	ASSERT_KINDOF(CButton,descriptorOrig.m_pCBTemplate->CBGetWindow());
#endif	//	_DEBUG
	return InsertCustomButton(
		nIndex,nCustomButtonIndex,dwStyle,dwStyleEx,_T("Button"),lpszWindowName);
}


BOOL COXCoolToolBar::InsertCheckBox(int nIndex, int nCustomButtonIndex, 
									LPCTSTR lpszWindowName, 
									DWORD dwStyle/*=WS_CHILD|WS_VISIBLE|BS_CHECKBOX*/, 
									DWORD dwStyleEx/*=0*/)
{
#ifdef _DEBUG
	int nID=m_arrAllCustomButtonIDs[nCustomButtonIndex];
	OXCUSTOMBUTTONDESCRIPTOR descriptorOrig;
	VERIFY(m_mapAllCustomButtons.Lookup(nID,descriptorOrig));
	ASSERT_KINDOF(CButton,descriptorOrig.m_pCBTemplate->CBGetWindow());
#endif	//	_DEBUG
	return InsertCustomButton(
		nIndex,nCustomButtonIndex,dwStyle,dwStyleEx,_T("Button"),lpszWindowName);
}


BOOL COXCoolToolBar::InsertProgressBar(int nIndex, int nCustomButtonIndex, 
									   DWORD dwStyle/*=WS_CHILD|WS_VISIBLE*/, 
									   DWORD dwStyleEx/*=0*/)
{
#ifdef _DEBUG
	int nID=m_arrAllCustomButtonIDs[nCustomButtonIndex];
	OXCUSTOMBUTTONDESCRIPTOR descriptorOrig;
	VERIFY(m_mapAllCustomButtons.Lookup(nID,descriptorOrig));
	ASSERT_KINDOF(CProgressCtrl,descriptorOrig.m_pCBTemplate->CBGetWindow());
#endif	//	_DEBUG
	return InsertCustomButton(
		nIndex,nCustomButtonIndex,dwStyle,dwStyleEx,_T("msctls_progress32"));
}


BOOL COXCoolToolBar::InsertSlider(int nIndex, int nCustomButtonIndex, 
								  DWORD dwStyle/*=WS_CHILD|WS_VISIBLE|TBS_HORZ*/, 
								  DWORD dwStyleEx/*=0*/)
{
#ifdef _DEBUG
	int nID=m_arrAllCustomButtonIDs[nCustomButtonIndex];
	OXCUSTOMBUTTONDESCRIPTOR descriptorOrig;
	VERIFY(m_mapAllCustomButtons.Lookup(nID,descriptorOrig));
	ASSERT_KINDOF(CSliderCtrl,descriptorOrig.m_pCBTemplate->CBGetWindow());
#endif	//	_DEBUG
	return InsertCustomButton(
		nIndex,nCustomButtonIndex,dwStyle,dwStyleEx,_T("msctls_trackbar32"));
}


#if _MFC_VER>0x0421
/////////////////////////////////////////////////////////////////////////////////
BOOL COXCoolToolBar::InsertDateTimePicker(int nIndex, int nCustomButtonIndex, 
										  COleDateTime date, 
										  DWORD dwStyle/*=WS_CHILD|WS_VISIBLE*/, 
										  DWORD dwStyleEx/*=0*/)
{
	// register date-time and mont calendar window classes
	INITCOMMONCONTROLSEX icex;
	icex.dwSize=sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC=ICC_DATE_CLASSES;
	::InitCommonControlsEx(&icex);

	if(!InsertCustomButton(
		nIndex,nCustomButtonIndex,dwStyle,dwStyleEx,_T("SysDateTimePick32")))
	{
		return FALSE;
	}
	else
	{
		CWnd* pWnd=GetCustomButtonWnd(nIndex);
		ASSERT(pWnd!=NULL);
		CDateTimeCtrl* pDateTimeCtrl=(CDateTimeCtrl*)(pWnd);
		pDateTimeCtrl->SetTime(date);

		return TRUE;
	}
}


BOOL COXCoolToolBar::InsertIPAddressCtrl(int nIndex, int nCustomButtonIndex, 
										 DWORD dwStyle/*=WS_CHILD|WS_VISIBLE*/, 
										 DWORD dwStyleEx/*=0*/)
{
#ifdef _DEBUG
	int nID=m_arrAllCustomButtonIDs[nCustomButtonIndex];
	OXCUSTOMBUTTONDESCRIPTOR descriptorOrig;
	VERIFY(m_mapAllCustomButtons.Lookup(nID,descriptorOrig));
	ASSERT_KINDOF(CIPAddressCtrl,descriptorOrig.m_pCBTemplate->CBGetWindow());
#endif	//	_DEBUG
	
	// register date-time and mont calendar window classes
	INITCOMMONCONTROLSEX icex;
	icex.dwSize=sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC=ICC_INTERNET_CLASSES;
	::InitCommonControlsEx(&icex);

	return InsertCustomButton(
		nIndex,nCustomButtonIndex,dwStyle,dwStyleEx,_T("SysIPAddress32"));
}


BOOL COXCoolToolBar::InsertComboBoxEx(int nIndex, int nCustomButtonIndex, 
									  DWORD dwStyle/*=WS_CHILD|WS_VISIBLE|CBS_DROPDOWN*/, 
									  DWORD dwStyleEx/*=0*/)
{
#ifdef _DEBUG
	int nID=m_arrAllCustomButtonIDs[nCustomButtonIndex];
	OXCUSTOMBUTTONDESCRIPTOR descriptorOrig;
	VERIFY(m_mapAllCustomButtons.Lookup(nID,descriptorOrig));
	ASSERT_KINDOF(CComboBoxEx,descriptorOrig.m_pCBTemplate->CBGetWindow());
#endif	//	_DEBUG
	return InsertCustomButton(
		nIndex,nCustomButtonIndex,dwStyle,dwStyleEx,WC_COMBOBOXEX);
}
/////////////////////////////////////////////////////////////////////////////////
#endif	//	_MFC_VER>0x0421


BOOL COXCoolToolBar::ShowCustomButton(int nIndex)
{
	OXCUSTOMBUTTONDESCRIPTOR descriptor;
	if(!GetCustomButton(nIndex,descriptor))
	{
		TRACE(_T("COXCoolToolBar::ShowCustomButton: there is no custom button at specified index\n"));
		return FALSE;
	}

	BOOL bIsVisible=IsWindowVisible();

	BOOL bShowCustomButton=((descriptor.m_nFlags&(OXCBD_SHOWANY))==(OXCBD_SHOWANY));
	if(!bShowCustomButton)
	{
		if(IsFloating())
		{
			bShowCustomButton=(descriptor.m_nFlags&OXCBD_SHOWFLOAT);
		}
		else
		{
			if(m_dwStyle&CBRS_ORIENT_HORZ)
			{
				bShowCustomButton=(descriptor.m_nFlags&OXCBD_SHOWHORZ);
			}
			else
			{
				bShowCustomButton=(descriptor.m_nFlags&OXCBD_SHOWVERT);
			}
		}
	}

	if(bShowCustomButton)
	{
		UINT nIDTest;
		UINT nStyleTest;
		int nWidthTest;
		GetButtonInfo(nIndex,nIDTest,nStyleTest,nWidthTest);
		if((nStyleTest&TBBS_SEPARATOR)!=TBBS_SEPARATOR || 
			nWidthTest!=descriptor.m_nWidth)
		{
			SetButtonInfo(
				nIndex,descriptor.m_pCBTemplate->CBGetWindow()->GetDlgCtrlID(),
				TBBS_SEPARATOR,descriptor.m_nWidth);
		}

		CRect rect;
		GetItemRect(nIndex,&rect);
		rect.DeflateRect(2,0);
		if(descriptor.m_nHeight>0)
		{
			if(descriptor.m_nHeight<rect.Height())
			{
				rect.top+=(rect.Height()-descriptor.m_nHeight)/2;
			}
			rect.bottom=rect.top+descriptor.m_nHeight;
		}
		CWnd* pWnd = descriptor.m_pCBTemplate->CBGetWindow();
		if (DYNAMIC_DOWNCAST(CComboBox, pWnd))
		{
			// Center the combo box
			RECT rectButton;
			GetItemRect(0, &rectButton);
			rect.OffsetRect(0, (rectButton.bottom - 22) / 2 + 1);
		}
		pWnd->MoveWindow(rect,bIsVisible);
		pWnd->ShowWindow(SW_SHOWNA);
	}	
	else
	{
		UINT nIDTest;
		UINT nStyleTest;
		int nWidthTest;
		GetButtonInfo(nIndex,nIDTest,nStyleTest,nWidthTest);
		if((nStyleTest&TBBS_SEPARATOR)==TBBS_SEPARATOR || 
			(int)nIDTest!=descriptor.m_pCBTemplate->CBGetWindow()->GetDlgCtrlID())
		{
#if _MFC_VER > 0x0421
			SetButtonInfo(
				nIndex,descriptor.m_pCBTemplate->CBGetWindow()->GetDlgCtrlID(),
				TBBS_BUTTON,descriptor.m_nImageIndex);
			TBBUTTONINFO buttonInfo={ sizeof(TBBUTTONINFO) };
			buttonInfo.dwMask=TBIF_STYLE;
			if(GetToolBarCtrl().
				GetButtonInfo(descriptor.m_pCBTemplate->CBGetWindow()->GetDlgCtrlID(),
				&buttonInfo))
			{
				buttonInfo.fsStyle|=TBSTYLE_AUTOSIZE;
			}
			else
			{
				buttonInfo.fsStyle=TBSTYLE_AUTOSIZE;
			}
			VERIFY(GetToolBarCtrl().
				SetButtonInfo(descriptor.m_pCBTemplate->CBGetWindow()->GetDlgCtrlID(),
				&buttonInfo));
#else
			SetButtonInfo(
				nIndex,descriptor.m_pCBTemplate->CBGetWindow()->GetDlgCtrlID(),
				TBBS_BUTTON,descriptor.m_nImageIndex);
			TBBUTTONINFO buttonInfo={ sizeof(TBBUTTONINFO) };
			buttonInfo.dwMask=TBIF_STYLE;
			if(GetToolBarCtrl().SendMessage(TB_GETBUTTONINFO,
				(WPARAM) descriptor.m_pCBTemplate->CBGetWindow()->GetDlgCtrlID(),
				(LPARAM)(LPTBBUTTONINFO) &buttonInfo))
			{
				buttonInfo.fsStyle|=TBSTYLE_AUTOSIZE;
			}
			else
			{
				buttonInfo.fsStyle=TBSTYLE_AUTOSIZE;
			}
			VERIFY(GetToolBarCtrl().SendMessage(TB_GETBUTTONINFO,
				(WPARAM) descriptor.m_pCBTemplate->CBGetWindow()->GetDlgCtrlID(),
				(LPARAM)(LPTBBUTTONINFO) &buttonInfo));

#endif
		}
		descriptor.m_pCBTemplate->CBGetWindow()->ShowWindow(SW_HIDE);
	}

	if(bIsVisible!=IsWindowVisible())
	{
		if(bIsVisible)
		{
			ModifyStyle(0,WS_VISIBLE,SWP_NOREDRAW);
		}
		else
		{
			ModifyStyle(WS_VISIBLE,0,SWP_NOREDRAW);
		}
	}

	return TRUE;
}


int COXCoolToolBar::GetCustomButtonIndex(int nCtrlID)
{
	for(int nIndex=0; nIndex<m_arrAllCustomButtonIDs.GetSize(); nIndex++)
	{
		if(m_arrAllCustomButtonIDs[nIndex]==nCtrlID)
		{
			return nIndex;
		}
	}

	return -1;
}


int COXCoolToolBar::GetCustomButtonIndex(CWnd* pWnd)
{
	ASSERT(::IsWindow(pWnd->GetSafeHwnd()));
	return GetCustomButtonIndex(pWnd->GetDlgCtrlID());
}


void COXCoolToolBar::UpdateCustomButtons(int nFirstButtonIndex/*=0*/, 
										 int nLastButtonIndex/*=-1*/)
{
	if(nLastButtonIndex==-1)
	{
		nLastButtonIndex=GetToolBarCtrl().GetButtonCount()-1;
	}

	for(int nIndex=nFirstButtonIndex; nIndex<=nLastButtonIndex; nIndex++)
	{
		OXCUSTOMBUTTONDESCRIPTOR descriptor;
		if(m_mapCustomButtons.Lookup(nIndex,descriptor))
		{
			ShowCustomButton(nIndex);
		}
	}
}


HRESULT COXCoolToolBar::GetComCtlVersion(LPDWORD pdwMajor, LPDWORD pdwMinor) const
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

#if _MFC_VER >= 0x0700
void COXCoolToolBar::DrawBorders(CDC* pDC, CRect& rect)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	DWORD dwStyle = m_dwStyle;
	if (!(dwStyle & CBRS_BORDER_ANY))
		return;

	// prepare for dark lines
	ASSERT(rect.top == 0 && rect.left == 0);
	CRect rect1, rect2;
	rect1 = rect;
	rect2 = rect;
	COLORREF clr = afxData.clrBtnShadow;


	// draw dark line one pixel back/up
	if (dwStyle & CBRS_BORDER_3D)
	{
		rect1.right -= CX_BORDER;
		rect1.bottom -= CY_BORDER;
	}
	if (dwStyle & CBRS_BORDER_TOP)
		rect2.top += afxData.cyBorder2;
	if (dwStyle & CBRS_BORDER_BOTTOM)
		rect2.bottom -= afxData.cyBorder2;

	// draw left and top
	if (dwStyle & CBRS_BORDER_LEFT)
		pDC->FillSolidRect(0, rect2.top, CX_BORDER, rect2.Height(), clr);
	if (dwStyle & CBRS_BORDER_TOP)
		pDC->FillSolidRect(0, 0, rect.right, CY_BORDER, clr);

	// draw right and bottom
	if (dwStyle & CBRS_BORDER_RIGHT)
		pDC->FillSolidRect(rect1.right, rect2.top, -CX_BORDER, rect2.Height(), clr);
	if (dwStyle & CBRS_BORDER_BOTTOM)
		pDC->FillSolidRect(0, rect1.bottom, rect.right, -CY_BORDER, clr);


	if (dwStyle & CBRS_BORDER_3D)
	{
		// prepare for hilite lines
		clr = afxData.clrBtnHilite;

		// draw left and top
		if (dwStyle & CBRS_BORDER_LEFT)
			pDC->FillSolidRect(1, rect2.top, CX_BORDER, rect2.Height(), clr);
		if (dwStyle & CBRS_BORDER_TOP)
			pDC->FillSolidRect(0, 1, rect.right, CY_BORDER, clr);

		// draw right and bottom
		if (dwStyle & CBRS_BORDER_RIGHT)
			pDC->FillSolidRect(rect.right, rect2.top, -CX_BORDER, rect2.Height(), clr);
		if (dwStyle & CBRS_BORDER_BOTTOM)
			pDC->FillSolidRect(0, rect.bottom, rect.right, -CY_BORDER, clr);
	}

	if (dwStyle & CBRS_BORDER_LEFT)
		rect.left += afxData.cxBorder2;
	if (dwStyle & CBRS_BORDER_TOP)
		rect.top += afxData.cyBorder2;
	if (dwStyle & CBRS_BORDER_RIGHT)
		rect.right -= afxData.cxBorder2;
	if (dwStyle & CBRS_BORDER_BOTTOM)
		rect.bottom -= afxData.cyBorder2;

}
#endif // _MFC_VER >= 0x0700

////////////////////////////////
#endif //_MFC_VER>=0x0420

COXToolbarSkin* COXCoolToolBar::GetToolbarSkin()
{
	// Check if the app is derived from COXSkinnedApp
	COXSkinnedApp* pSkinnedApp = DYNAMIC_DOWNCAST(COXSkinnedApp, AfxGetApp());
	if (pSkinnedApp != NULL && pSkinnedApp->GetCurrentSkin() != NULL)
		return pSkinnedApp->GetCurrentSkin()->GetToolbarSkin();
	else
	{
		// Create a classic skin for this class if not created already
		if (m_pToolbarSkin == NULL)
			m_pToolbarSkin = new COXToolbarSkinClassic();

		return m_pToolbarSkin;
	}
}

BOOL COXCoolToolBar::OnDropDownButton(NMHDR* pNMHDR, LRESULT* result)
{
	// Save the index of the last drop down button
	LPNMTOOLBAR pNMToolBar = (LPNMTOOLBAR) pNMHDR;
	m_iDropDownIndex = pNMToolBar->iItem;
	m_iLastDropDownIndex = pNMToolBar->iItem;
	result = 0;
	return FALSE;
}

BOOL COXCoolToolBar::IsFloatingEnabled()
{
	return m_bFloatingEnabled;
}

void COXCoolToolBar::EnableFloating(BOOL bEnable)
{
	m_bFloatingEnabled = bEnable;
}

void COXCoolToolBar::EnableDocking(DWORD dwDockStyle, BOOL bSnapWhileDragging)
{
	// must be CBRS_ALIGN_XXX or CBRS_FLOAT_MULTI only
	ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI)) == 0);
	// CBRS_SIZE_DYNAMIC toolbar cannot have the CBRS_FLOAT_MULTI style
	ASSERT(((dwDockStyle & CBRS_FLOAT_MULTI) == 0) || ((m_dwStyle & CBRS_SIZE_DYNAMIC) == 0));

	m_dwDockStyle = dwDockStyle;
	if (m_pDockContext == NULL)
		m_pDockContext = new CDockContext(this);

	// permanently wire the bar's owner to its current parent
	if (m_hWndOwner == NULL)
		m_hWndOwner = ::GetParent(m_hWnd);

	m_bSnapWhileDragging = bSnapWhileDragging;
}

void COXCoolToolBar::SaveMouseOffset(CPoint point)
{
	// Calculate and save the offset
	CRect rectWindow;
	GetWindowRect(rectWindow);
	ScreenToClient(&rectWindow);

	m_ptOffset = point - rectWindow.TopLeft();
}

