// ==========================================================================
//				Class Implementation : COXBitmapMenu
// ==========================================================================

// Source file : OXBitmapMenu.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#pragma warning(disable : 4706)
#include <multimon.h>

#include "OXBitmapMenu.h"
#include "OXBitmapMenuOrganizer.h"
#include "OXSkins.h"
#include "OXCoolToolBar.h"
#include "OXMenuBar.h"

#ifdef OX_CUSTOMIZE_COMMANDS
#include "OXDragDropCommands.h"
#endif	//	OX_CUSTOMIZE_COMMANDS


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

/////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(COXBitmapMenuPopupWnd,CWnd)


COXBitmapMenuPopupWnd::COXBitmapMenuPopupWnd() : 
	m_pBitmapMenu(NULL),
	m_nCheckForDragDropEventTimerID(0)
{
}


COXBitmapMenuPopupWnd::~COXBitmapMenuPopupWnd() 
{ 
	ResetPopupMenu();
}


BEGIN_MESSAGE_MAP(COXBitmapMenuPopupWnd,CWnd)
	//{{AFX_MSG_MAP(COXBitmapMenuPopupWnd)
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_MOUSEACTIVATE()
	ON_WM_SETTINGCHANGE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP

	// handle drag'n'drop messages
	ON_MESSAGE(SHBDTM_DRAGENTER, OnDragEnter)
	ON_MESSAGE(SHBDTM_DRAGLEAVE, OnDragLeave)
	ON_MESSAGE(SHBDTM_DRAGOVER, OnDragOver)
	ON_MESSAGE(SHBDTM_DROP, OnDrop)

	// handle advanced customization commands
	ON_COMMAND(ID_OXCUSTBM_DELETE,OnCustBMDelete)
	ON_COMMAND(ID_OXCUSTBM_APPEARANCE,OnCustBMAppearance)
	ON_COMMAND(ID_OXCUSTBM_SEPARATOR_BEFORE,OnCustBMSeparatorBefore)
	ON_COMMAND(ID_OXCUSTBM_SEPARATOR_AFTER,OnCustBMSeparatorAfter)
	ON_COMMAND(ID_OXCUSTBM_RECENTLY_USED,OnCustBMRecentlyUsed)

END_MESSAGE_MAP()


void COXBitmapMenuPopupWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	COXBitmapMenu* pMenu=GetBitmapMenu();
	if(m_nCheckForDragDropEventTimerID==0 && 
		pMenu!=NULL && pMenu->IsInCustomizationMode())
	{
		int nItemIndex=pMenu->HitTest(&point);
		if(nItemIndex>=0 && nItemIndex<(int)pMenu->GetMenuItemCount())
		{
			pMenu->SetCustomizedItem(nItemIndex);
		}
		else
		{
			pMenu->SetCustomizedItem(-1);
		}

		m_nCheckForDragDropEventTimerID=SetTimer(IDT_OXCHECKFORDRAGDROPEVENT,
			ID_OXCHECKFORDRAGDROPEVENT_DELAY,NULL);
		ASSERT(m_nCheckForDragDropEventTimerID!=0);

		return;
	}

	CWnd::OnLButtonDown(nFlags,point); 
}


void COXBitmapMenuPopupWnd::OnMButtonDown(UINT nFlags, CPoint point)
{
	COXBitmapMenu* pMenu=GetBitmapMenu();
	if(pMenu!=NULL && pMenu->IsInCustomizationMode())
	{
		int nItemIndex=pMenu->HitTest(&point);
		if(nItemIndex>=0 && nItemIndex<(int)pMenu->GetMenuItemCount())
		{
			pMenu->SetCustomizedItem(nItemIndex);
		}
		else
		{
			pMenu->SetCustomizedItem(-1);
		}

		return;
	}

	CWnd::OnMButtonDown(nFlags,point);
}


void COXBitmapMenuPopupWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	COXBitmapMenu* pMenu=GetBitmapMenu();
	if(pMenu!=NULL && pMenu->IsInCustomizationMode())
	{
		int nItemIndex=pMenu->HitTest(&point);
		if(nItemIndex>=0 && nItemIndex<(int)pMenu->GetMenuItemCount())
		{
			pMenu->SetCustomizedItem(nItemIndex);
		}
		else
		{
			pMenu->SetCustomizedItem(-1);
		}

		return;
	}

	CWnd::OnRButtonDown(nFlags,point);
}


void COXBitmapMenuPopupWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	COXBitmapMenu* pMenu=GetBitmapMenu();
	if(pMenu!=NULL && pMenu->IsInCustomizationMode())
	{
		int nCustomizedItemIndex=pMenu->GetCustomizedItem();
		if(nCustomizedItemIndex!=-1)
		{
			ClientToScreen(&point);
			pMenu->DisplayCustomizeItemContextMenu(nCustomizedItemIndex,point);
			return;
		}
	}

	CWnd::OnRButtonUp(nFlags,point);
}


void COXBitmapMenuPopupWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_nCheckForDragDropEventTimerID!=0 && ::GetKeyState(VK_LBUTTON)>=0)
	{
		KillTimer(m_nCheckForDragDropEventTimerID);
		m_nCheckForDragDropEventTimerID=0;
	}

	CWnd::OnMouseMove(nFlags,point); 
}


LONG COXBitmapMenuPopupWnd::OnDragEnter(WPARAM wParam, LPARAM lParam)
{
	COXBitmapMenu* pMenu=GetBitmapMenu();
	if(pMenu!=NULL && pMenu->IsInCustomizationMode())
	{
		// lParam is the pointer to SHBDROPTARGETACTION structure
		LPSHBDROPTARGETACTION pSHBDTAction=(LPSHBDROPTARGETACTION)lParam;
		ASSERT(pSHBDTAction!=NULL);

		ASSERT(pSHBDTAction->pWnd);
		ASSERT(pSHBDTAction->pWnd->GetSafeHwnd()==GetSafeHwnd());

		return pMenu->OnDragOver(wParam,lParam);
	}

	return (LONG)FALSE;
}

LONG COXBitmapMenuPopupWnd::OnDragOver(WPARAM wParam, LPARAM lParam)
{
	COXBitmapMenu* pMenu=GetBitmapMenu();
	if(pMenu!=NULL && pMenu->IsInCustomizationMode())
	{
		// lParam is the pointer to SHBDROPTARGETACTION structure
		LPSHBDROPTARGETACTION pSHBDTAction=(LPSHBDROPTARGETACTION)lParam;
		ASSERT(pSHBDTAction!=NULL);

		ASSERT(pSHBDTAction->pWnd);
		ASSERT(pSHBDTAction->pWnd->GetSafeHwnd()==GetSafeHwnd());

		return pMenu->OnDragOver(wParam,lParam);
	}

	return (LONG)FALSE;
}

LONG COXBitmapMenuPopupWnd::OnDragLeave(WPARAM wParam, LPARAM lParam)
{
	COXBitmapMenu* pMenu=GetBitmapMenu();
	if(pMenu!=NULL && pMenu->IsInCustomizationMode())
	{
		// lParam is the pointer to SHBDROPTARGETACTION structure
		LPSHBDROPTARGETACTION pSHBDTAction=(LPSHBDROPTARGETACTION)lParam;
		ASSERT(pSHBDTAction!=NULL);

		ASSERT(pSHBDTAction->pWnd);
		ASSERT(pSHBDTAction->pWnd->GetSafeHwnd()==GetSafeHwnd());

		return pMenu->OnDragLeave(wParam,lParam);
	}

	return (LONG)FALSE;
}

LONG COXBitmapMenuPopupWnd::OnDrop(WPARAM wParam, LPARAM lParam)
{
	COXBitmapMenu* pMenu=GetBitmapMenu();
	if(pMenu!=NULL && pMenu->IsInCustomizationMode())
	{
		// lParam is the pointer to SHBDROPTARGETACTION structure
		LPSHBDROPTARGETACTION pSHBDTAction=(LPSHBDROPTARGETACTION)lParam;
		ASSERT(pSHBDTAction!=NULL);

		ASSERT(pSHBDTAction->pWnd);
		ASSERT(pSHBDTAction->pWnd->GetSafeHwnd()==GetSafeHwnd());

		return pMenu->OnDrop(wParam,lParam);
	}

	return (LONG)FALSE;
}


void COXBitmapMenuPopupWnd::OnCustBMDelete()
{
	COXBitmapMenu* pMenu=GetBitmapMenu();
	if(pMenu!=NULL && pMenu->IsInCustomizationMode())
	{
		pMenu->OnCustBMDelete();
	}
}

void COXBitmapMenuPopupWnd::OnCustBMAppearance()
{
	COXBitmapMenu* pMenu=GetBitmapMenu();
	if(pMenu!=NULL && pMenu->IsInCustomizationMode())
	{
		pMenu->OnCustBMAppearance();
	}
}

void COXBitmapMenuPopupWnd::OnCustBMSeparatorBefore()
{
	COXBitmapMenu* pMenu=GetBitmapMenu();
	if(pMenu!=NULL && pMenu->IsInCustomizationMode())
	{
		pMenu->OnCustBMSeparatorBefore();
	}
}

void COXBitmapMenuPopupWnd::OnCustBMSeparatorAfter()
{
	COXBitmapMenu* pMenu=GetBitmapMenu();
	if(pMenu!=NULL && pMenu->IsInCustomizationMode())
	{
		pMenu->OnCustBMSeparatorAfter();
	}
}

void COXBitmapMenuPopupWnd::OnCustBMRecentlyUsed()
{
	COXBitmapMenu* pMenu=GetBitmapMenu();
	if(pMenu!=NULL && pMenu->IsInCustomizationMode())
	{
		pMenu->OnCustBMRecentlyUsed();
	}
}


void COXBitmapMenuPopupWnd::OnPaint()
{
	CPaintDC dc(this);

	int nSavedDC=dc.SaveDC();
	ASSERT(nSavedDC!=0);

	dc.SetTextColor(::GetSysColor(COLOR_MENUTEXT));
	dc.SelectObject(&m_fontMenu);

	COXBitmapMenu* pMenu=GetBitmapMenu();
	if(pMenu!=NULL)
	{
		ASSERT((int)pMenu->GetMenuItemCount()==m_arrItemRects.GetSize());

		MENUITEMINFO mii={ sizeof(mii) };
		mii.fMask=MIIM_DATA;

		DRAWITEMSTRUCT dis;
		dis.CtlType=ODT_MENU;
		dis.hDC=dc.GetSafeHdc();
		dis.itemAction=ODA_DRAWENTIRE;
		dis.CtlID=0;
		dis.itemState=0;
		dis.hwndItem=(HWND)pMenu->GetSafeHmenu();
		for(int nIndex=0; nIndex<m_arrItemRects.GetSize(); nIndex++)
		{
			dis.itemID=pMenu->GetMenuItemID(nIndex);
			dis.rcItem=m_arrItemRects[nIndex];
			VERIFY(::GetMenuItemInfo(pMenu->GetSafeHmenu(),nIndex,TRUE,&mii));
			dis.itemData=mii.dwItemData;
			pMenu->DrawItem(&dis);
		}
	}

	VERIFY(dc.RestoreDC(nSavedDC));
}


int COXBitmapMenuPopupWnd::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest,
										   UINT message)
{
	UNREFERENCED_PARAMETER(pDesktopWnd);
	UNREFERENCED_PARAMETER(nHitTest);
	UNREFERENCED_PARAMETER(message);

	return MA_NOACTIVATE;
}


void COXBitmapMenuPopupWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	UpdateMenuMetrics();
	OnMenuChanged();

	CWnd::OnSettingChange(uFlags, lpszSection);
}


void COXBitmapMenuPopupWnd::OnTimer(UINT nIDEvent)
{
	if((int)nIDEvent==m_nCheckForDragDropEventTimerID)
	{
		KillTimer(m_nCheckForDragDropEventTimerID);
		m_nCheckForDragDropEventTimerID=0;

		if(::IsWindow(GetSafeHwnd()))		
		{
			// do drag and drop
			//
			COXBitmapMenu* pMenu=GetBitmapMenu();
			if(pMenu!=NULL && pMenu->IsInCustomizationMode())
			{
				if(::GetKeyState(VK_LBUTTON)<0)
				{
					pMenu->OnBeginDragDrop(pMenu->GetCustomizedItem());
				}
			}
			//
			////////////////////////////////////
		}

		return;
	}

	CWnd::OnTimer(nIDEvent);
}


BOOL COXBitmapMenuPopupWnd::TrackPopupMenu(COXBitmapMenu* pMenu, UINT nFlags, 
										   int x, int y, CWnd* pWnd)
{
	ASSERT(pMenu!=NULL);
	m_pBitmapMenu=pMenu;

	COXBitmapMenu* pBitmapMenu=GetBitmapMenu();
	pBitmapMenu->SetPopupWnd(this);

	if(!::IsWindow(GetSafeHwnd()))
	{
		if(!CreateEx(WS_EX_DLGMODALFRAME,AfxRegisterWndClass(
			CS_SAVEBITS|CS_DBLCLKS,0,(HBRUSH)(COLOR_BTNFACE+1),0),_T(""),
			WS_POPUP,m_rectWindow,pWnd,0,NULL))
		{
			return FALSE;
		}
	}

	// register OLE Drag'n'Drop
	COleDropTarget* pOleDropTarget=pBitmapMenu->GetDropTarget();
	ASSERT(pOleDropTarget!=NULL);
	pOleDropTarget->Revoke();
	if(!pOleDropTarget->Register(this))
	{
		TRACE(_T("COXBitmapMenuPopupWnd::Create: failed to register the control with COleDropTarget. You've probably forgotten to initialize OLE libraries using AfxOleInit function\n"));
	}

	pBitmapMenu->SetCutomizationMode(TRUE,pWnd->GetSafeHwnd());
	
	// update info about the font used to display the menu
	UpdateMenuMetrics();
	// calculate the rectangles for the menu items and for the popup menu itself
	VERIFY(CalcLayout(nFlags,x,y));

	SetWindowPos(&wndTop,m_rectWindow.left,m_rectWindow.top,m_rectWindow.Width(),
		m_rectWindow.Height(),SWP_NOACTIVATE|SWP_SHOWWINDOW);
	ShowWindow(SW_SHOWNA);

	return TRUE;
}


void COXBitmapMenuPopupWnd::ResetPopupMenu()
{
	COXBitmapMenu* pMenu=GetBitmapMenu();
	if(pMenu!=NULL)
	{
		if(!pMenu->IsDragDropOwner())
		{
			pMenu->SetCustomizedItem(-1);
			pMenu->SetPopupWnd(NULL);
			m_pBitmapMenu=NULL;
		}
	}

	if(::IsWindow(GetSafeHwnd()))
	{
		ShowWindow(SW_HIDE);
	}
}


BOOL COXBitmapMenuPopupWnd::CalcLayout(UINT nFlags, int x, int y)
{
	COXBitmapMenu* pMenu=GetBitmapMenu();
	if(pMenu==NULL)
	{
		return FALSE;
	}

	m_arrItemRects.RemoveAll();
	m_rectWindow.SetRectEmpty();

	int nWidth=0;
	int nHeight=0;

	int nIndex=0;
	for(nIndex=0; nIndex<(int)pMenu->GetMenuItemCount(); nIndex++)
	{
		MENUITEMINFO mii={ sizeof(mii) };
		mii.fMask=MIIM_DATA;
		VERIFY(::GetMenuItemInfo(pMenu->GetSafeHmenu(),nIndex,TRUE,&mii));

		MEASUREITEMSTRUCT mis;
		mis.CtlType=ODT_MENU;
		mis.itemData=mii.dwItemData;
		mis.itemID=pMenu->GetMenuItemID(nIndex);
		mis.itemHeight=0;
		mis.itemWidth=0;
		pMenu->MeasureItem(&mis);

		CRect rect(0,nHeight,mis.itemWidth,nHeight+mis.itemHeight);
		m_arrItemRects.Add(rect);

		nHeight+=mis.itemHeight;
		if(nWidth<(int)mis.itemWidth)
			nWidth=mis.itemWidth;
	}

	if(nWidth==0 && nHeight==0)
	{
		nWidth=ID_OXBITMAPMENUPOPUPWND_DEFAULT_WIDTH;
		nHeight=ID_OXBITMAPMENUPOPUPWND_DEFAULT_HEIGHT;
	}

	m_rectWindow.top=y;
	switch(nFlags)
	{
	case TPM_CENTERALIGN:
		m_rectWindow.left=x-nWidth/2;
		break;
	case TPM_LEFTALIGN:
		m_rectWindow.left=x;
		break;
	case TPM_RIGHTALIGN:
		m_rectWindow.left=x-nWidth;
		break;
	}
	m_rectWindow.right=m_rectWindow.left+nWidth;
	m_rectWindow.bottom=m_rectWindow.top+nHeight;

	CRect rectCopy=m_rectWindow;
	CalcWindowRect(m_rectWindow);
	CSize szOffset(rectCopy.left-m_rectWindow.left,rectCopy.top-m_rectWindow.top);
	m_rectWindow.OffsetRect(szOffset.cx,szOffset.cy);
	m_rectWindow.InflateRect(0,0,2*szOffset.cx,0);

	nWidth+=2*szOffset.cx;
	// adjust rectangles for items
	for(nIndex=0; nIndex<m_arrItemRects.GetSize(); nIndex++)
	{
		CRect& rect=m_arrItemRects[nIndex];
		if(rect.Width()<nWidth)
			rect.right=rect.left+nWidth;
	}

	return TRUE;
}


void COXBitmapMenuPopupWnd::UpdateMenuMetrics()
{
	if((HFONT)m_fontMenu!=NULL)
		m_fontMenu.DeleteObject();

	// Menu font, height and color
	NONCLIENTMETRICS ncm={ sizeof(ncm) };
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS,sizeof(ncm),&ncm,0);
	VERIFY(m_fontMenu.CreateFontIndirect(&ncm.lfMenuFont));
}


void COXBitmapMenuPopupWnd::OnMenuChanged()
{
	ASSERT(GetBitmapMenu()!=NULL);
	ASSERT(::IsWindow(GetSafeHwnd()));

	CRect rect;
	GetWindowRect(rect);
	CPoint ptStart=rect.TopLeft();
	GetBitmapMenu()->CalcExtents();
	VERIFY(CalcLayout(TPM_LEFTALIGN,ptStart.x,ptStart.y));

	if(IsWindowVisible())
	{
		SetWindowPos(&wndTop,m_rectWindow.left,m_rectWindow.top,m_rectWindow.Width(),
			m_rectWindow.Height(),SWP_NOACTIVATE|SWP_SHOWWINDOW);
		RedrawWindow();
	}
	else
	{
		ResetPopupMenu();
	}
}


void COXBitmapMenuPopupWnd::RedrawItem(int nIndex)
{
	ASSERT(GetBitmapMenu()!=NULL);
	ASSERT(::IsWindow(GetSafeHwnd()));

	ASSERT(nIndex>=0 && nIndex<m_arrItemRects.GetSize());
	CRect rect;
	rect=m_arrItemRects[nIndex];
	RedrawWindow(rect);
}


///////////////////////////////////////////////////////
// COXShadowedItemWnd
COXShadowedItemWnd::COXShadowedItemWnd(COXCoolToolBar* pCoolToolbar, int iMenuItem, UINT nPosFlags)
{
	m_pCoolToolbar = pCoolToolbar;
	m_iMenuItem = iMenuItem;
	m_nPosFlags = nPosFlags;
}

COXShadowedItemWnd::~COXShadowedItemWnd()
{
}


BEGIN_MESSAGE_MAP(COXShadowedItemWnd, CWnd)
	//{{AFX_MSG_MAP(COXShadowedItemWnd)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////

WNDPROC COXBitmapMenu::m_origWndProc = NULL;

IMPLEMENT_DYNAMIC(COXBitmapMenu, CMenu)

COXMenuSkin* COXBitmapMenu::m_pMenuSkin = NULL;

COXBitmapMenu::COXBitmapMenu()
	:
	m_nBitmapExtent(0),
	m_nTextHeight(0),
	m_nAcceleratorExtent(0),
	m_nStringExtent(0),
	m_nCustomizedItemIndex(-1),
	m_bCutomizable(FALSE),
	m_bInCutomizationMode(FALSE),
	m_bDragDropOwner(FALSE),
	m_bDragDropOperation(FALSE),
	m_hWndCustomizeOrganizer(NULL),
	m_nInsertMarkIndex(-1),
	m_pPopupWnd(NULL),
	m_rectDropDownItem(0, 0, 0, 0)
{
	// Sublass the menus
	RegisterWindowClass(AfxGetInstanceHandle());
}


COXBitmapMenu::~COXBitmapMenu()
{
	m_KeyAccessMap.RemoveAll();
	COXItemInfo* pItemInfo;
	while(!m_ItemInfoList.IsEmpty())
	{
		pItemInfo=m_ItemInfoList.RemoveHead();
		delete pItemInfo;
		pItemInfo = NULL;
	}
	TRACE(_T("\n"));
	// delete the classic skin
	if (m_pMenuSkin != NULL)
	{
		delete m_pMenuSkin;
		m_pMenuSkin = NULL;
	}

	COleDropTarget* pOleDropTarget=GetDropTarget();
	ASSERT(pOleDropTarget!=NULL);
	pOleDropTarget->Revoke();
}


void COXBitmapMenu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(m_hMenu!=NULL);
	ASSERT(lpDrawItemStruct != NULL);
	ASSERT(lpDrawItemStruct->CtlType == ODT_MENU);

	UINT nState=lpDrawItemStruct->itemState;

	CRect itemRect(lpDrawItemStruct->rcItem);
	CRect buttonRect(0,0,0,0);
	CRect imageRect(0,0,0,0);
	CRect text1Rect(0,0,0,0);
	CRect text2Rect(0,0,0,0);

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	COXItemInfo* pItemInfo=(COXItemInfo*)lpDrawItemStruct->itemData;
	ASSERT(AfxIsValidAddress(pItemInfo, sizeof(COXItemInfo)));
	COXImageInfo* pImageInfo=pItemInfo->GetImageInfo();
	CString sText=pItemInfo->GetText();

	if(lpDrawItemStruct->itemID!=0 && lpDrawItemStruct->itemID!=0xffff)
	{
		// Compute the space for each menu item part
		DistributeSpace(
			nState,pImageInfo,itemRect,buttonRect,imageRect,text1Rect,text2Rect);
	}

	COXBitmapMenuOrganizer* pOrganizer=
		COXBitmapMenuOrganizer::FindOrganizer(AfxGetMainWnd()->GetSafeHwnd());
	ASSERT(pOrganizer!=NULL);
	BOOL bBefore=TRUE;
	CPoint ptTest=itemRect.CenterPoint();
	int nItemIndex=HitTest(&ptTest,&bBefore);
	if(pOrganizer->IsShowOnlyRecentlyUsedItems() && 
		!pOrganizer->IsRecentlyUsed(this,nItemIndex))
	{
		nState|=OXODS_HIDDEN;
		if(nItemIndex>0 && pOrganizer->IsRecentlyUsed(this,nItemIndex-1))
		{
			nState|=OXODS_HIDDENFIRST;
		}
		if(nItemIndex<(int)GetMenuItemCount()-1 && 
			pOrganizer->IsRecentlyUsed(this,nItemIndex+1))
		{
			nState|=OXODS_HIDDENLAST;
		}
	}

	// Draw every part of the menu item
	if(lpDrawItemStruct->itemID==0 || lpDrawItemStruct->itemID==0xffff)
	{
		DrawSeparator(&dc,itemRect);
	}
	else if(IsPopupItem(lpDrawItemStruct->itemID))
	{
		DrawSubmenuItem(&dc,nState,sText,pImageInfo,
			itemRect,buttonRect,text1Rect,text2Rect);
	}
	else
	{
		DrawBackground(&dc,nState,pImageInfo,itemRect,buttonRect);
		DrawButton(&dc,nState,pImageInfo,buttonRect);
		DrawImage(&dc,nState,pImageInfo,imageRect);
		if((((int)lpDrawItemStruct->itemID)&0x0000ffff)==ID_OX_SHOWALLITEMS)
		{
			ASSERT(!IsInCustomizationMode());
			DrawExpansionItem(&dc,itemRect,nState);
		}
		else
		{
			DrawText(&dc,nState,sText,text1Rect,text2Rect);
		}
	}


	// draw customized item
	int nCustomizedItem=GetCustomizedItem();
	if(nCustomizedItem!=-1 && 
		GetMenuItemID(nCustomizedItem)==lpDrawItemStruct->itemID)
	{
		CPoint pt=itemRect.CenterPoint();
		if(HitTest(&pt)==nCustomizedItem)
		{
			DrawCustomized(&dc,itemRect);
		}
	}


	// draw insert mark
	int nInsertMark=GetInsertMark();
	if(nInsertMark!=-1)
	{
		ASSERT(nInsertMark>=0 && nInsertMark<=(int)GetMenuItemCount());
		BOOL bBefore=(nInsertMark<(int)GetMenuItemCount());
		if(!bBefore)
			nInsertMark--;
		CPoint pt=itemRect.CenterPoint();
		if(HitTest(&pt)==nInsertMark)
		{
			DrawInsertMark(&dc,itemRect,bBefore);
		}
	}

	dc.Detach(); 
}


void COXBitmapMenu::DistributeSpace(UINT nState, 
									COXImageInfo* pImageInfo, 
									CRect itemRect, 
									CRect& buttonRect, 
									CRect& imageRect, 
									CRect& text1Rect, 
									CRect& text2Rect)
{
	GetMenuSkin()->DistributeSpace(nState, pImageInfo, itemRect, buttonRect, imageRect,
		text1Rect, text2Rect, this);
}


HBRUSH COXBitmapMenu::HBrushDitherCreate(COLORREF rgbFace, COLORREF rgbHilight)
{     
	struct  //BITMAPINFO with 16 colors         
	{         
		BITMAPINFOHEADER    bmiHeader; 
        RGBQUAD             bmiColors[16];         
	} bmi;      
	HBRUSH hBrush=NULL;     
	DWORD patGray[8];     
	HDC hDC;     
	HBITMAP hBmp;     
	static COLORREF rgbFaceOld=0xFFFFFFFF;  //Initially impossible     
	static COLORREF rgbHilightOld=0xFFFFFFFF;  //Initially impossible      

   /*
	* We're going to create an 8*8 brush for PatBlt using the      
	* face color and highlight color.
	*/     
	bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);     
	bmi.bmiHeader.biWidth=8;     
	bmi.bmiHeader.biHeight=8;     
	bmi.bmiHeader.biPlanes=1;     
	bmi.bmiHeader.biBitCount=1;     
	bmi.bmiHeader.biCompression=BI_RGB;     
	bmi.bmiHeader.biSizeImage=0;     
	bmi.bmiHeader.biXPelsPerMeter=0;     
	bmi.bmiHeader.biYPelsPerMeter=0;     
	bmi.bmiHeader.biClrUsed=0;     
	bmi.bmiHeader.biClrImportant=0;

	bmi.bmiColors[0].rgbBlue=GetBValue(rgbFace);     
	bmi.bmiColors[0].rgbGreen=GetGValue(rgbFace);     
	bmi.bmiColors[0].rgbRed=GetRValue(rgbFace);     
	bmi.bmiColors[0].rgbReserved=0;      
	bmi.bmiColors[1].rgbBlue=GetBValue(rgbHilight);     
	bmi.bmiColors[1].rgbGreen=GetGValue(rgbHilight);     
	bmi.bmiColors[1].rgbRed=GetRValue(rgbHilight);     
	bmi.bmiColors[1].rgbReserved=0;     
	
	//Create the byte array for CreateDIBitmap.     
	patGray[6]=patGray[4]=patGray[2]=patGray[0]=0x5555AAAAL;     
	patGray[7]=patGray[5]=patGray[3]=patGray[1]=0xAAAA5555L;      

	//Create the bitmap     
	hDC=::GetDC(NULL);
	hBmp=::CreateDIBitmap(hDC,&bmi.bmiHeader,CBM_INIT,patGray,
		(LPBITMAPINFO)&bmi,DIB_RGB_COLORS);
    ::ReleaseDC(NULL,hDC);      
	//Create the brush from the bitmap     
	if(NULL!=hBmp)         
	{ 
        hBrush=CreatePatternBrush(hBmp);
		DeleteObject(hBmp);
    } 
    return hBrush;     
}      


void COXBitmapMenu::DrawBackground(CDC* pDC, UINT nState, COXImageInfo* pImageInfo,
								   CRect itemRect, CRect buttonRect)
{
	GetMenuSkin()->DrawBackground(pDC, nState, pImageInfo, itemRect, buttonRect, this);
}


void COXBitmapMenu::DrawButton(CDC* pDC, UINT nState,  COXImageInfo* pImageInfo, 
							   CRect buttonRect)
{
	GetMenuSkin()->DrawButton(pDC, nState, pImageInfo, buttonRect, this);
}

void COXBitmapMenu::DrawImage(CDC* pDC, UINT nState, COXImageInfo* pImageInfo, 
							  CRect imageRect)
{
	GetMenuSkin()->DrawImage(pDC, nState, pImageInfo, imageRect, this);
}

void COXBitmapMenu::DrawText(CDC* pDC, UINT nState, 
							 CString sText, CRect text1Rect, CRect text2Rect)
{
	GetMenuSkin()->DrawText(pDC, nState, sText, text1Rect, text2Rect, this);
}

void COXBitmapMenu::DrawSeparator(CDC* pDC, CRect itemRect)
{
	GetMenuSkin()->DrawSeparator(pDC, itemRect, this);
}

void COXBitmapMenu::DrawCustomized(CDC* pDC, CRect itemRect)
{
	GetMenuSkin()->DrawCustomized(pDC, itemRect, this);
}

void COXBitmapMenu::DrawInsertMark(CDC* pDC, CRect itemRect, BOOL bBefore)
{
	GetMenuSkin()->DrawInsertMark(pDC, itemRect, bBefore, this);
}

void COXBitmapMenu::DrawSubmenuItem(CDC* pDC, UINT nState, CString sText, 
									COXImageInfo* pImageInfo, 
									CRect itemRect, CRect buttonRect, 
									CRect text1Rect, CRect text2Rect)
{
	GetMenuSkin()->DrawSubmenuItem(pDC, nState, sText, pImageInfo, itemRect, buttonRect,
		text1Rect, text2Rect, this);
}

void COXBitmapMenu::DrawExpansionItem(CDC* pDC, CRect itemRect, UINT nState)
{
	GetMenuSkin()->DrawExpansionItem(pDC, itemRect, nState, this);
}


void COXBitmapMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	OXDIMENSIONCONSTANTS& oxdc = GetMenuSkin()->GetDimentionConstants();

	ASSERT(lpMeasureItemStruct != NULL);
	COXItemInfo* pItemInfo=(COXItemInfo*)lpMeasureItemStruct->itemData;
	if(pItemInfo)
	{
		ASSERT(AfxIsValidAddress(pItemInfo, sizeof(COXItemInfo)));
		COXImageInfo* pImageInfo=pItemInfo->GetImageInfo();

		int nImageWidth=0;
		int nImageHeight=0;
		if(pImageInfo != NULL)
		{
			IMAGEINFO ii;
			::ZeroMemory(&ii, sizeof(ii));

			pImageInfo->GetImageList()->GetImageInfo(pImageInfo->GetIndex(),&ii);
			nImageWidth=ii.rcImage.right - ii.rcImage.left + 2;
			nImageHeight=ii.rcImage.bottom - ii.rcImage.top + 2;
		}
		
		lpMeasureItemStruct->itemWidth=
			oxdc.m_nGapLeftBitmap+m_nBitmapExtent+oxdc.m_nGapBitmapText+
			m_nStringExtent+(m_nAcceleratorExtent ? oxdc.m_nGapTextAcclrtr : 0)+
			m_nAcceleratorExtent+oxdc.m_nGapAcclrtrRight;
		if(lpMeasureItemStruct->itemID==0 || lpMeasureItemStruct->itemID==0xffff)
		{
			// separator
			lpMeasureItemStruct->itemHeight=oxdc.m_nSeparatorHeight;
		}
		else
		{
			// item with text
			lpMeasureItemStruct->itemHeight=__max(nImageHeight+oxdc.m_nGapVertBitmap, 
				m_nTextHeight+oxdc.m_nGapVertText);

			if ((((int)lpMeasureItemStruct->itemID)&0x0000ffff)==ID_OX_SHOWALLITEMS)
				GetMenuSkin()->AdjustExpansionItemHeight(lpMeasureItemStruct->itemHeight, this);
		}
	}
	else if (IsPopupItem(lpMeasureItemStruct->itemID))
		lpMeasureItemStruct->itemHeight = m_nTextHeight + oxdc.m_nGapVertText; 
}

void COXBitmapMenu::CalcExtents()
{
	UINT nItemCount=GetMenuItemCount();

	CString sText;
	CString sBeforeTab;
	CString sAfterTab;
	int nTabCharIndex=0;

	m_nStringExtent=0;
	m_nAcceleratorExtent=0;
	m_nBitmapExtent=0;

	MENUITEMINFO mii={ sizeof(MENUITEMINFO) };

	CFont Font, boldFont;
	NONCLIENTMETRICS ncm={ sizeof(NONCLIENTMETRICS) };

	COXItemInfo* pItemInfo=NULL;
	COXImageInfo* pImageInfo=NULL;
	IMAGEINFO ii={ 0 };

	VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(ncm), &ncm, 0));
	VERIFY(Font.CreateFontIndirect(&ncm.lfMenuFont));
	ncm.lfMenuFont.lfWeight = FW_BOLD; // make the font bold
	VERIFY(boldFont.CreateFontIndirect(&ncm.lfMenuFont));

	CWnd* pMainWnd=AfxGetThread()->GetMainWnd();
	CDC* pDC=pMainWnd->GetDC();

	CSize TextExt;
	for(UINT nItemIndex=0; nItemIndex < nItemCount; nItemIndex++)
	{
		CFont* pOldFont;
		if (GetDefaultItem(GMDI_USEDISABLED, MF_BYPOSITION) == nItemIndex)
			pOldFont = pDC->SelectObject(&boldFont);
		else	
			pOldFont = pDC->SelectObject(&Font);
		
		mii.fMask=MIIM_TYPE|MIIM_DATA|MIIM_SUBMENU;
		mii.cch=300;
		mii.dwTypeData=sText.GetBuffer(mii.cch);
		// ... zero-terminate string
		mii.dwTypeData[0]=_T('\0');
		::GetMenuItemInfo(GetSafeHmenu(), nItemIndex, TRUE, &mii);
		sText.ReleaseBuffer();

		if(mii.fType & MFT_SEPARATOR)
			continue;

		pItemInfo=(COXItemInfo*)(mii.dwItemData);
		if(pItemInfo)
		{
			pImageInfo=pItemInfo->GetImageInfo();
			if(pImageInfo)
			{
				pImageInfo->GetImageList()->GetImageInfo(pImageInfo->GetIndex(),&ii);
				m_nBitmapExtent=__max(m_nBitmapExtent, __max(GetMenuSkin()->GetDimentionConstants().m_nMinBitmapWidth,ii.rcImage.right-ii.rcImage.left));
			}
			sText=pItemInfo->GetText();
		}

		nTabCharIndex=sText.Find(_T('\t'));
		if(nTabCharIndex != -1)
		{
			sBeforeTab=sText.Left(nTabCharIndex);
			sAfterTab=sText.Mid(nTabCharIndex + 1);
		}
		else
		{
			sBeforeTab=sText;
			if(mii.hSubMenu!=NULL)
				sAfterTab=_T("W");
			else	
				sAfterTab.Empty();
		}

		CRect text1Rect(0,0,0,0);
		CRect text2Rect(0,0,0,0);
		pDC->DrawText(sBeforeTab, text1Rect, DT_CALCRECT | DT_SINGLELINE);
		pDC->DrawText(sAfterTab, text2Rect, DT_CALCRECT | DT_SINGLELINE);

		m_nStringExtent=__max(m_nStringExtent, text1Rect.Width());
		m_nAcceleratorExtent=__max(m_nAcceleratorExtent, text2Rect.Width());
	
		pDC->SelectObject(pOldFont);
	}

	TextExt=pDC->GetTextExtent(_T("A"));
	m_nTextHeight=TextExt.cy;

	pMainWnd->ReleaseDC(pDC);
}

void COXBitmapMenu::AddItemInfo(COXItemInfo* pItemInfo)
{
	m_ItemInfoList.AddTail(pItemInfo);
}

/////////////////////////////////////////////////////////////////////////////
// COXBitmapMenu idle update through CBitmapMenuCmdUI class

class OX_CLASS_DECL CBitmapMenuCmdUI : public CCmdUI        // class private to this file !
{
public: // re-implementations only
	virtual void SetText(LPCTSTR lpszText);
};

void CBitmapMenuCmdUI::SetText(LPCTSTR lpszText)
{
	ASSERT(lpszText != NULL);
	ASSERT(AfxIsValidString(lpszText));
	
	ASSERT(m_pMenu->IsKindOf(RUNTIME_CLASS(COXBitmapMenu)));
	if(m_pMenu != NULL)
	{
		if(m_pSubMenu != NULL)
			return; // don't change popup menus indirectly

		MENUITEMINFO mii={ sizeof(mii) };
		mii.fMask=MIIM_TYPE|MIIM_DATA;
		::GetMenuItemInfo(m_pMenu->GetSafeHmenu(),m_nIndex,TRUE,&mii);

		if(mii.fType == MFT_OWNERDRAW)
		{
			COXItemInfo* pItemInfo=(COXItemInfo*)(mii.dwItemData);
			ASSERT(pItemInfo != NULL);
			pItemInfo->SetText(lpszText);
		}
		else
		{
			CCmdUI::SetText(lpszText);
		}
	}
}

void COXBitmapMenu::OnUpdateCmdUI(CWnd* pWnd, UINT /*nIndex*/, BOOL bSysMenu)
{
	// Code almost entirely copied from CFrameWnd::OnInitPopup
	if(bSysMenu)
	{
		return;     // don't support system menu
	}

	// check the enabled state of various menu items

	CBitmapMenuCmdUI state;
	state.m_pMenu=this;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// determine if menu is popup in top-level menu and set m_pOther to
	//  it if so (m_pParentMenu == NULL indicates that it is secondary popup)
	HMENU hParentMenu;
	if(AfxGetThreadState()->m_hTrackingMenu == m_hMenu)
	{
		state.m_pParentMenu=this;    // parent == child for tracking popup
	}
	else if((hParentMenu=::GetMenu(pWnd->m_hWnd)) != NULL)
	{
		CWnd* pParent=pWnd->GetTopLevelParent();
			// child windows don't have menus -- need to go to the top!
		if(pParent != NULL &&
			(hParentMenu=::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax=::GetMenuItemCount(hParentMenu);
			for (int nIndex=0; nIndex < nIndexMax; nIndex++)
			{
				if(::GetSubMenu(hParentMenu, nIndex) == m_hMenu)
				{
					// when popup is found, m_pParentMenu is containing menu
					state.m_pParentMenu=CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax=GetMenuItemCount();
	for (state.m_nIndex=0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++)
	{
		state.m_nID=GetMenuItemID(state.m_nIndex);
		if(state.m_nID == 0)
			continue; // menu separator or invalid cmd - ignore it

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if(state.m_nID == (UINT)-1)
		{
			// possibly a popup menu, route to first item of that popup
			state.m_pSubMenu=GetSubMenu(state.m_nIndex);
			if(state.m_pSubMenu == NULL ||
				(state.m_nID=state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // first item of popup can't be routed to
			}
			state.DoUpdate(pWnd, FALSE);    // popups are never auto disabled
		}
		else
		{
			// normal menu item
			// Auto enable/disable if frame window has 'm_bAutoMenuEnable'
			//    set and command is _not_ a system command.
			state.m_pSubMenu=NULL;
			state.DoUpdate(pWnd,
				((CFrameWnd*)pWnd)->m_bAutoMenuEnable && state.m_nID<0xF000);
		}

		// adjust for menu deletions and additions
		UINT nCount=GetMenuItemCount();
		if(nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax=nCount;
	}
}


int COXBitmapMenu::SetCustomizedItem(int nIndex) 
{ 
	int nOldCustomizedItemIndex=m_nCustomizedItemIndex;
	m_nCustomizedItemIndex=nIndex;
	if(m_pPopupWnd!=NULL && ::IsWindow(m_pPopupWnd->GetSafeHwnd()))
	{
		if(nOldCustomizedItemIndex!=m_nCustomizedItemIndex)
		{
			if(nOldCustomizedItemIndex!=-1)
				m_pPopupWnd->RedrawItem(nOldCustomizedItemIndex);
			if(m_nCustomizedItemIndex!=-1)
				m_pPopupWnd->RedrawItem(m_nCustomizedItemIndex);
		}
		SendCustomizeNotification(ID_OXCUSTBM_SET_CUSTOMIZE_ITEM);
	}
	return nOldCustomizedItemIndex;
}


BOOL COXBitmapMenu::DisplayCustomizeItemContextMenu(int nItemIndex, CPoint point)
{
	ASSERT(nItemIndex>=0 && nItemIndex<(int)GetMenuItemCount());

	COXBitmapMenuOrganizer* pOrganizer=
		COXBitmapMenuOrganizer::FindOrganizer(AfxGetMainWnd()->GetSafeHwnd());
	ASSERT(pOrganizer!=NULL);

	CString sText;
	MENUITEMINFO mii={ sizeof(MENUITEMINFO) };
	mii.fMask=MIIM_TYPE|MIIM_DATA|MIIM_ID|MIIM_SUBMENU;
	mii.cch=300;
	mii.dwTypeData=sText.GetBuffer(mii.cch);
	// ... zero-terminate string
	mii.dwTypeData[0]=_T('\0');
	VERIFY(::GetMenuItemInfo(GetSafeHmenu(),nItemIndex,TRUE,&mii));
	sText.ReleaseBuffer();
	COXItemInfo* pItemInfo=(COXItemInfo*)(mii.dwItemData);
	ASSERT(AfxIsValidAddress(pItemInfo,sizeof(COXItemInfo)));
	sText=pItemInfo->GetText();

	// don't add most recent files used 
	if((int)mii.wID>=ID_FILE_MRU_FILE1 && (int)mii.wID<=ID_FILE_MRU_FILE16)
	{
		return FALSE;
	}

	// don't add MDIChild windows menu items 
	if((int)mii.wID>=AFX_IDM_FIRST_MDICHILD && mii.hSubMenu==NULL)
	{
		return FALSE;
	}


	CMenu menu;
	if(!menu.CreatePopupMenu())
		return FALSE;

	// populate menu
	CString sItem;
	VERIFY(sItem.LoadString(IDS_OX_CUSTBM_DELETE));
	VERIFY(menu.AppendMenu(MF_STRING,ID_OXCUSTBM_DELETE,sItem));
	if((mii.fType&MFT_SEPARATOR)==0)
	{
		VERIFY(menu.AppendMenu(MF_SEPARATOR));
		VERIFY(sItem.LoadString(IDS_OX_CUSTBM_APPEARANCE));
		VERIFY(menu.AppendMenu(MF_STRING,ID_OXCUSTBM_APPEARANCE,
			sItem));
		VERIFY(menu.AppendMenu(MF_SEPARATOR));
		if(pOrganizer->IsShowOnlyRecentlyUsedItems()) 
		{
			VERIFY(sItem.LoadString(IDS_OX_CUSTBM_RECENTLY_USED));
			VERIFY(menu.AppendMenu(MF_STRING|
				(pOrganizer->IsRecentlyUsed(this,nItemIndex) ? MF_CHECKED : 
				MF_UNCHECKED),ID_OXCUSTBM_RECENTLY_USED,sItem));
			VERIFY(menu.AppendMenu(MF_SEPARATOR));
		}

		BOOL bSeparatorBefore=FALSE;
		if(nItemIndex>0)
		{
			MENUITEMINFO mii={ sizeof(MENUITEMINFO) };
			mii.fMask=MIIM_TYPE;
			mii.cch=300;
			mii.dwTypeData=sText.GetBuffer(mii.cch);
			// ... zero-terminate string
			mii.dwTypeData[0]=_T('\0');
			VERIFY(::GetMenuItemInfo(GetSafeHmenu(),nItemIndex-1,TRUE,&mii));
			sText.ReleaseBuffer();

			bSeparatorBefore=((mii.fType&MFT_SEPARATOR)==0);
		}
		VERIFY(sItem.LoadString(IDS_OX_CUSTBM_SEPARATOR_BEFORE));
		VERIFY(menu.AppendMenu(MF_STRING|(bSeparatorBefore ? 0 : MF_GRAYED),
			ID_OXCUSTBM_SEPARATOR_BEFORE,sItem));

		BOOL bSeparatorAfter=FALSE;
		if(nItemIndex<(int)(GetMenuItemCount()-1))
		{
			MENUITEMINFO mii={ sizeof(MENUITEMINFO) };
			mii.fMask=MIIM_TYPE;
			mii.cch=300;
			mii.dwTypeData=sText.GetBuffer(mii.cch);
			// ... zero-terminate string
			mii.dwTypeData[0]=_T('\0');
			VERIFY(::GetMenuItemInfo(GetSafeHmenu(),nItemIndex+1,TRUE,&mii));
			sText.ReleaseBuffer();

			bSeparatorAfter=((mii.fType&MFT_SEPARATOR)==0);
		}
		VERIFY(sItem.LoadString(IDS_OX_CUSTBM_SEPARATOR_AFTER));
		VERIFY(menu.AppendMenu(MF_STRING|(bSeparatorAfter ? 0 : MF_GRAYED),
			ID_OXCUSTBM_SEPARATOR_AFTER,sItem));
	}

	CWnd* pWndOwner=NULL;
	if(m_pPopupWnd!=NULL && ::IsWindow(m_pPopupWnd->GetSafeHwnd()))
		pWndOwner=m_pPopupWnd;

	menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
		point.x,point.y,pWndOwner);

	return TRUE;
}


void COXBitmapMenu::OnCustBMDelete()
{
	ASSERT(IsInCustomizationMode());
	int nCustomizedItemIndex=GetCustomizedItem();
	ASSERT(nCustomizedItemIndex!=-1);

	if(SendCustomizeNotification(ID_OXCUSTBM_DELETE))
		return;

	VERIFY(DeleteMenu(nCustomizedItemIndex,MF_BYPOSITION));
	m_nCustomizedItemIndex=-1;
	UpdateContents();
}

void COXBitmapMenu::OnCustBMAppearance()
{
	ASSERT(IsInCustomizationMode());
	int nCustomizedItemIndex=GetCustomizedItem();
	ASSERT(nCustomizedItemIndex!=-1);
	UNUSED(nCustomizedItemIndex);

	SendCustomizeNotification(ID_OXCUSTBM_APPEARANCE);	
}

void COXBitmapMenu::OnCustBMSeparatorBefore()
{
	ASSERT(IsInCustomizationMode());
	int nCustomizedItemIndex=GetCustomizedItem();
	ASSERT(nCustomizedItemIndex!=-1);

	if(SendCustomizeNotification(ID_OXCUSTBM_SEPARATOR_BEFORE))
		return;

	VERIFY(InsertMenu(nCustomizedItemIndex,MF_SEPARATOR|MF_BYPOSITION));
	UpdateContents();
	SetCustomizedItem(nCustomizedItemIndex+1);
}

void COXBitmapMenu::OnCustBMSeparatorAfter()
{
	ASSERT(IsInCustomizationMode());
	int nCustomizedItemIndex=GetCustomizedItem();
	ASSERT(nCustomizedItemIndex!=-1);

	if(SendCustomizeNotification(ID_OXCUSTBM_SEPARATOR_AFTER))
		return;

	VERIFY(InsertMenu(nCustomizedItemIndex+1,MF_SEPARATOR|MF_BYPOSITION));
	UpdateContents();
}

void COXBitmapMenu::OnCustBMRecentlyUsed()
{
	ASSERT(IsInCustomizationMode());
	int nCustomizedItemIndex=GetCustomizedItem();
	ASSERT(nCustomizedItemIndex!=-1);

	if(SendCustomizeNotification(ID_OXCUSTBM_RECENTLY_USED))
		return;

	COXBitmapMenuOrganizer* pOrganizer=
		COXBitmapMenuOrganizer::FindOrganizer(AfxGetMainWnd()->GetSafeHwnd());
	ASSERT(pOrganizer!=NULL);
	if(pOrganizer->IsRecentlyUsed(this,nCustomizedItemIndex))
	{
		VERIFY(pOrganizer->
			ExcludeFromRecentlyUsed(this,nCustomizedItemIndex));
	}
	else
	{
		VERIFY(pOrganizer->
			AddToRecentlyUsed(this,nCustomizedItemIndex));
	}
	UpdateContents();
}


LRESULT COXBitmapMenu::SendCustomizeNotification(UINT nCustomizeCmdID)
{
	ASSERT(GetPopupWnd()!=NULL);

	HWND hWnd=m_hWndCustomizeOrganizer;
	if(hWnd==NULL)
	{
		hWnd=AfxGetMainWnd()->GetSafeHwnd();
	}
	if(hWnd==NULL)
		return (LRESULT)0;

	NMBMCUSTOMIZE nmbmCustomize;
	nmbmCustomize.nmhdr.code=OXBMN_CUSTOMIZECMD;
	nmbmCustomize.nmhdr.hwndFrom=GetPopupWnd()->GetSafeHwnd();
	nmbmCustomize.nmhdr.idFrom=::GetDlgCtrlID(hWnd);
	nmbmCustomize.nCustomizeEventID=nCustomizeCmdID;

	return ::SendMessage(hWnd,WM_NOTIFY,(WPARAM)nmbmCustomize.nmhdr.idFrom,
		(LPARAM)&nmbmCustomize);
}


LONG COXBitmapMenu::OnDragEnter(WPARAM wParam, LPARAM lParam)
{
	// toolbar must be in customizable state
	if(!IsCustomizable())
		return (LONG)FALSE;

	// set flag that specifies that drag'n'drop operation is active
	m_bDragDropOperation=TRUE;

	return OnDragOver(wParam,lParam);
}

LONG COXBitmapMenu::OnDragOver(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	// toolbar must be in customizable state
	if(!IsCustomizable())
		return (LONG)FALSE;

	// lParam is the pointer to SHBDROPTARGETACTION structure
	LPSHBDROPTARGETACTION pSHBDTAction=(LPSHBDROPTARGETACTION)lParam;
	ASSERT(pSHBDTAction!=NULL);

	pSHBDTAction->result=(LRESULT)DROPEFFECT_NONE;
#ifdef OX_CUSTOMIZE_COMMANDS
	// Can we use this object?
	if(pSHBDTAction->pDataObject->
		IsDataAvailable(COXDragDropCommands::m_cfCommandButton))
	{
		// analize the current cursor position
		//
		BOOL bBefore=TRUE;
		int nItemIndex=HitTest(&pSHBDTAction->point,&bBefore);
		int nInsertMarkIndex=(bBefore ? nItemIndex : nItemIndex+1);

		SetInsertMark(nInsertMarkIndex);

		// Check if the control key was pressed          
		if((pSHBDTAction->dwKeyState & MK_CONTROL)==MK_CONTROL)
			pSHBDTAction->result=(LRESULT)DROPEFFECT_COPY;
		else
			pSHBDTAction->result=(LRESULT)DROPEFFECT_MOVE; 
	}
#endif	//	OX_CUSTOMIZE_COMMANDS

	return (LONG)TRUE;
}

LONG COXBitmapMenu::OnDragLeave(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if(!IsCustomizable())
		return (LONG)FALSE;

	// lParam is the pointer to SHBDROPTARGETACTION structure
	LPSHBDROPTARGETACTION pSHBDTAction=(LPSHBDROPTARGETACTION)lParam;
	ASSERT(pSHBDTAction!=NULL);

	SetInsertMark(-1);

	// reset flag that specifies that drag'n'drop operation is active
	m_bDragDropOperation=FALSE;

	return (LONG)TRUE;
}

LONG COXBitmapMenu::OnDrop(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	if(!IsCustomizable())
		return (LONG)FALSE;

	// lParam is the pointer to SHBDROPTARGETACTION structure
	LPSHBDROPTARGETACTION pSHBDTAction=(LPSHBDROPTARGETACTION)lParam;
	ASSERT(pSHBDTAction!=NULL);

	pSHBDTAction->result=(LRESULT)FALSE;

#ifdef OX_CUSTOMIZE_COMMANDS
	// if dragged item is to be copied or moved
	if((pSHBDTAction->dropEffect&DROPEFFECT_COPY)!=0 || 
		(pSHBDTAction->dropEffect&DROPEFFECT_MOVE)!=0)
	{
		// data must be in the specific format
		ASSERT(pSHBDTAction->pDataObject->
			IsDataAvailable(COXDragDropCommands::m_cfCommandButton));

		int nItemIndex=GetInsertMark();
		if(nItemIndex!=-1 || GetMenuItemCount()==0)
		{
			// Get the drag item info
			//
			HGLOBAL hgData=pSHBDTAction->pDataObject->
				GetGlobalData(COXDragDropCommands::m_cfCommandButton);
			ASSERT(hgData!=NULL);
			// lock it
			BYTE* lpItemData=(BYTE*)::GlobalLock(hgData);

			if(!SendCustomizeNotification(ID_OXCUSTBM_INSERT_ITEM))
			{
				RetrieveDragDropMenuItem(lpItemData,GetSafeHmenu(),nItemIndex);

				if(m_bDragDropOwner && m_nCustomizedItemIndex>=nItemIndex)
				{
					ASSERT(m_nCustomizedItemIndex!=-1);
					m_nCustomizedItemIndex++;
					ASSERT(GetDraggedItem()!=-1);
					SetDraggedItem(GetDraggedItem()+1);
				}
			}


			// unlock it
			::GlobalUnlock(hgData);
			// free it
			::GlobalFree(hgData);

			// remove insert mark
			SetInsertMark(-1);

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


void COXBitmapMenu::RetrieveDragDropMenuItem(BYTE*& lpData, HMENU hMenu, 
											 int nItemIndex)
{
	ASSERT(lpData!=NULL);
	ASSERT(hMenu!=NULL);
	ASSERT(::IsMenu(hMenu));

	COXBitmapMenuOrganizer* pOrganizer=
		COXBitmapMenuOrganizer::FindOrganizer(AfxGetMainWnd()->GetSafeHwnd());
	ASSERT(pOrganizer!=NULL);

	while(TRUE)
	{
		// get button command ID
		int nCommandID=*(int*)lpData;
		lpData+=sizeof(int);
		
		// get button text
		CString sText((LPTSTR)lpData);
		lpData+=sText.GetLength()+sizeof(TCHAR);

		if(sText.IsEmpty() && nCommandID!=0)
		{
			HINSTANCE hInstance=
				AfxFindResourceHandle(MAKEINTRESOURCE(nCommandID),RT_STRING);
			ASSERT(hInstance!=NULL);
			sText.LoadString(nCommandID);
			int nPosition=sText.Find(_T('\n'));
			if(nPosition!=-1)
				sText=sText.Mid(nPosition+1);
		}
		ASSERT(!sText.IsEmpty() || nCommandID==0);

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
			pOrganizer->m_arrCreatedPopupMenus.Add(hSubMenu);
			VERIFY(::InsertMenu(hMenu,nItemIndex,
				MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT_PTR)hSubMenu,sText)!=0);
/*
			CMenu* pMenu=CMenu::FromHandle(hMenu);
			ASSERT(pMenu!=NULL);
			COXBitmapMenu* pBitmapMenu=DYNAMIC_DOWNCAST(COXBitmapMenu,pMenu);
			if(pBitmapMenu!=NULL)
			{
				pOrganizer->ConvertBitmapMenu(pBitmapMenu,FALSE);
			}
*/
			if(nFinish!=0)
			{
				RetrieveDragDropMenuItem(lpData,hSubMenu,0);
			}
		}
		else
		{
			VERIFY(::InsertMenu(hMenu,nItemIndex,
				MF_BYPOSITION|(nCommandID==0 ? MF_SEPARATOR : MF_STRING),
				(UINT)nCommandID,sText)!=0);
		}
		nItemIndex++;

		if(nFinish==0 || nFinish==2)
			break;
	}

	CMenu* pMenu=CMenu::FromHandle(hMenu);
	ASSERT(pMenu!=NULL);
	COXBitmapMenu* pBitmapMenu=DYNAMIC_DOWNCAST(COXBitmapMenu,pMenu);
	if(pBitmapMenu==NULL)
	{
		pOrganizer->OnInitMenuPopup(pMenu,0,FALSE);
		pMenu=CMenu::FromHandle(hMenu);
		pBitmapMenu=DYNAMIC_DOWNCAST(COXBitmapMenu,pMenu);
	}
	ASSERT(pBitmapMenu!=NULL);
	pBitmapMenu->UpdateContents();
}


int COXBitmapMenu::SetInsertMark(int nItemIndex) 
{
	ASSERT(nItemIndex>=-1 && nItemIndex<=(int)GetMenuItemCount());
	int nOldInsertMark=m_nInsertMarkIndex;
	m_nInsertMarkIndex=nItemIndex;
	if(nOldInsertMark!=m_nInsertMarkIndex)
	{
		if(m_pPopupWnd!=NULL && ::IsWindow(m_pPopupWnd->GetSafeHwnd()))
		{
			if(nOldInsertMark!=-1)
			{
				int nInsertMark=nOldInsertMark;
				if(nInsertMark==(int)GetMenuItemCount())
				{
					nInsertMark--;
				}
				m_pPopupWnd->RedrawItem(nInsertMark);
				if(nOldInsertMark!=0 && nOldInsertMark!=(int)GetMenuItemCount() &&
					nOldInsertMark!=m_nInsertMarkIndex+1)
				{
					m_pPopupWnd->RedrawItem(nOldInsertMark-1);
				}
			}
			if(m_nInsertMarkIndex!=-1)
			{
				int nInsertMark=m_nInsertMarkIndex;
				if(nInsertMark==(int)GetMenuItemCount())
					nInsertMark--;
				m_pPopupWnd->RedrawItem(nInsertMark);
				if(m_nInsertMarkIndex!=0 && 
					m_nInsertMarkIndex!=(int)GetMenuItemCount() &&
					m_nInsertMarkIndex!=nOldInsertMark+1)
				{
					m_pPopupWnd->RedrawItem(m_nInsertMarkIndex-1);
				}
			}
		}
		SendCustomizeNotification(ID_OXCUSTBM_SET_INSERT_MARK);
	}
	return nOldInsertMark;
}


int COXBitmapMenu::HitTest(LPPOINT ppt, BOOL* pbBefore/*=NULL*/)
{
	int nItemIndex=-1;
	int nHeight=0;
	for(int nIndex=0; nIndex<(int)GetMenuItemCount(); nIndex++)
	{
		MENUITEMINFO mii={ sizeof(mii) };
		mii.fMask=MIIM_DATA;
		VERIFY(::GetMenuItemInfo(GetSafeHmenu(),nIndex,TRUE,&mii));

		MEASUREITEMSTRUCT mis;
		mis.CtlType=ODT_MENU;
		mis.itemData=mii.dwItemData;
		mis.itemID=GetMenuItemID(nIndex);
		mis.itemHeight=(UINT)-1;
		mis.itemWidth=(UINT)-1;
		MeasureItem(&mis);
		
		nHeight+=mis.itemHeight;
		if(ppt->y<=nHeight)
		{
			nItemIndex=nIndex;
			if(pbBefore!=NULL)
				*pbBefore=(ppt->y<=nHeight-(int)mis.itemHeight/2);
			break;
		}
	}

	return nItemIndex;
}


void COXBitmapMenu::UpdateContents()
{
	COXBitmapMenuOrganizer* pOrganizer=
		COXBitmapMenuOrganizer::FindOrganizer(AfxGetMainWnd()->GetSafeHwnd());
	if(pOrganizer!=NULL)
	{
		pOrganizer->ConvertBitmapMenu(this,FALSE);
	}
	if(m_pPopupWnd!=NULL && ::IsWindow(m_pPopupWnd->GetSafeHwnd()))
	{
		m_pPopupWnd->OnMenuChanged();
	}
}


void COXBitmapMenu::OnBeginDragDrop(int nIndex)
{
	ASSERT(nIndex>=0 && nIndex<(int)GetMenuItemCount());
	ASSERT(m_pPopupWnd!=NULL && ::IsWindow(m_pPopupWnd->GetSafeHwnd()));

	UNUSED(nIndex);

	// mark the control as the one that launched drag'n'drop operation
	m_bDragDropOwner=TRUE;

#ifdef OX_CUSTOMIZE_COMMANDS
	CString sText;
	MENUITEMINFO mii={ sizeof(mii) };
	mii.fMask=MIIM_DATA|MIIM_TYPE|MIIM_ID|MIIM_SUBMENU;
	mii.cch=300;
	mii.dwTypeData=sText.GetBuffer(mii.cch);
	VERIFY(::GetMenuItemInfo(GetSafeHmenu(),nIndex,TRUE,&mii));
	sText.ReleaseBuffer();

	// don't allow to drag most recent files used 
	if((int)mii.wID>=ID_FILE_MRU_FILE1 && (int)mii.wID<=ID_FILE_MRU_FILE16)
	{
		// unmark as the control which launched drag'n'drop operation
		m_bDragDropOwner=FALSE;
		return;
	}

	// don't allow to drag MDIChild windows menu items 
	if((int)mii.wID>=AFX_IDM_FIRST_MDICHILD && mii.hSubMenu==NULL)
	{
		// unmark as the control which launched drag'n'drop operation
		m_bDragDropOwner=FALSE;
		return;
	}


	COXItemInfo* pItemInfo=(COXItemInfo*)(mii.dwItemData);
	ASSERT(AfxIsValidAddress(pItemInfo,sizeof(COXItemInfo)));
	ASSERT(!pItemInfo->GetText().IsEmpty() || mii.fType&MFT_SEPARATOR);

	CMenu* pSubmenu=GetSubMenu(nIndex);
	if(pSubmenu!=NULL)
	{
		COXBitmapMenu* pBMSubmenu=DYNAMIC_DOWNCAST(COXBitmapMenu,pSubmenu);
		if(pBMSubmenu!=NULL)
		{
			if(pBMSubmenu->GetPopupWnd()!=NULL)
			{
				pBMSubmenu->GetPopupWnd()->ResetPopupMenu();
			}
		}
	}

	COleDataSource* pDataSource=
		COXDragDropCommands::PrepareDragDropData(pItemInfo->GetText(),
		pItemInfo->GetImageInfo()->GetIndex(),GetMenuItemID(nIndex),0,
		GetSubMenu(nIndex)->GetSafeHmenu());
	ASSERT(pDataSource!=NULL);

	m_bDragDropOperation=TRUE;
	SetDraggedItem(nIndex);
	DROPEFFECT dropEffect=
		COXDragDropCommands::DoDragDrop(pDataSource,GetDropSource(m_pPopupWnd));
	if(DROPEFFECT_MOVE==dropEffect || (DROPEFFECT_NONE==dropEffect &&
		::GetKeyState(VK_LBUTTON)>=0 && !m_bDragDropOperation))
	{
		if(!SendCustomizeNotification(ID_OXCUSTBM_DELETE_ITEM))
		{
			int nDraggedItemIndex=GetDraggedItem();
			// delete item if it was moved
			VERIFY(DeleteMenu(nDraggedItemIndex,MF_BYPOSITION));
			m_nCustomizedItemIndex=-1;
			UpdateContents();
		}
	}
	SetDraggedItem(-1);

	//delete drag source (we are responsible to do that)
	delete pDataSource;
#endif	//	OX_CUSTOMIZE_COMMANDS

	// unmark as the control which launched drag'n'drop operation
	m_bDragDropOwner=FALSE;
}

COXMenuSkin* COXBitmapMenu::GetMenuSkin()
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

// Returns TRUE if the given item is a popup menu (has a submenu)
BOOL COXBitmapMenu::IsPopupItem(UINT nItemID)
{
	if (nItemID == (UINT)-1)
		return TRUE;

	MENUITEMINFO mii;
	::memset(&mii, 0, sizeof(MENUITEMINFO));
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_SUBMENU;

	GetMenuItemInfo(nItemID, &mii);
	if (mii.hSubMenu != NULL)
		return TRUE;
	else
		return FALSE;
}

void COXBitmapMenu::RestoreMDI()
{
	CMDIFrameWnd* pMainWnd = (CMDIFrameWnd*)AfxGetMainWnd();
	CMDIChildWnd * active = pMainWnd->MDIGetActive();	
	
	active->MDIRestore();

	pMainWnd->MDINext();

	while(active != pMainWnd->MDIGetActive())
	{
		pMainWnd->MDIGetActive()->MDIRestore();
		pMainWnd->MDINext();
	}
}

void COXBitmapMenu::CloseMDI()
{
	CMDIFrameWnd* pMainWnd = (CMDIFrameWnd*)AfxGetMainWnd();

	CMDIChildWnd * active = pMainWnd->MDIGetActive();

	if (active->GetActiveDocument()->IsModified())
	{
		CString sTitle;
		active->GetActiveFrame()->GetWindowText(sTitle);

		int ID = AfxMessageBox(_T("Save changes to ") + sTitle +  _T(" ?"), MB_YESNOCANCEL);

		switch(ID)
		{
		case IDCANCEL:
			// Do nothing
			break;
		case IDYES:
			active->GetActiveDocument()->DoFileSave();
			// Fall through intended
		case IDNO:
			active->DestroyWindow();
		}
	}
	else
	{
		active->DestroyWindow();
	}
}

void COXBitmapMenu::MinimizeMDI()
{
	CMDIFrameWnd* pMainWnd = (CMDIFrameWnd*)AfxGetMainWnd();

	CMDIChildWnd * active = pMainWnd->MDIGetActive();

	active->MDIRestore();
	active->ShowWindow(SW_MINIMIZE);}


BOOL COXBitmapMenu::HandleMDICommandMessage(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	UNREFERENCED_PARAMETER(pHandlerInfo);

	if (nCode == CN_UPDATE_COMMAND_UI)
    {
		switch(nID)
		{
		case ID_OXBITMAPMENU_CLOSE:
		case ID_OXBITMAPMENU_MINIMIZE:
		case ID_OXBITMAPMENU_RESTORE:
			((CCmdUI *) pExtra)->Enable (TRUE);
			return TRUE;
		}
    }
    else
    if (nCode == CN_COMMAND)
    {		
		bool bHandled = false;
		switch(nID)
		{	
		case ID_OXBITMAPMENU_CLOSE:
			CloseMDI();
			bHandled = true;
			break;
		case ID_OXBITMAPMENU_MINIMIZE:
			MinimizeMDI();
			bHandled = true;
			break;
		case ID_OXBITMAPMENU_RESTORE:
			RestoreMDI();
			bHandled = true;
			break;	
		case SC_MAXIMIZE:
			return TRUE;
		}	
		if(bHandled)
		{
			CMDIFrameWnd* pMainWnd = static_cast<CMDIFrameWnd*>(AfxGetMainWnd());
			if(pMainWnd)
			{
				CMDIChildWnd* pActiveWnd = pMainWnd->MDIGetActive();
				if(pActiveWnd)
				{
					pActiveWnd->GetSystemMenu(TRUE);
				}
			}
			return TRUE;
		}
	}
	
	return FALSE;
}


#ifndef CS_DROPSHADOW
#define CS_DROPSHADOW       0x00020000
#endif

BOOL COXBitmapMenu::RegisterWindowClass(HINSTANCE hInstance)
{
	if (m_origWndProc != NULL)
		return TRUE; // already registed

	WNDCLASS wndclass;

    if (GetClassInfo(hInstance,_T("#32768"),&wndclass))
		m_origWndProc = wndclass.lpfnWndProc;
    wndclass.lpfnWndProc = PopupWndProc;
	wndclass.style &= ~CS_DROPSHADOW;
	wndclass.style |= CS_SAVEBITS;
    wndclass.hInstance = hInstance ;
    RegisterClass(&wndclass);

    if (GetClassInfo(hInstance,_T("#32768"),&wndclass))
        return TRUE;
	else
		return FALSE;
}

LRESULT CALLBACK COXBitmapMenu::PopupWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	return GetMenuSkin()->MenuPopupWndProc(m_origWndProc, hwnd, nMsg, wParam, lParam);
}

void COXBitmapMenuPopupWnd::OnNcPaint() 
{
	COXBitmapMenu::GetMenuSkin()->OnNcPaintCustomizePopupWnd(this);
}

BOOL COXBitmapMenu::TrackPopupMenu(COXCoolToolBar* pCoolToolBar, CWnd* pWnd, LPCRECT lpRect)
{
	// Determine the selected item
	int iSelectedItem = -1;
	CToolBarCtrl& ctrl = pCoolToolBar->GetToolBarCtrl();
	int iButtonCount = ctrl.GetButtonCount();
	for (int i = 0; i < iButtonCount; i++)
	{
		if (ctrl.IsButtonPressed(pCoolToolBar->GetItemID(i)))
		{
			iSelectedItem = i;
			break;
		}
	}

	if (iSelectedItem == -1)
		iSelectedItem = ctrl.CommandToIndex(pCoolToolBar->m_iLastDropDownIndex);

	// Determine the rectangle of the selected item
	CRect rectItem;
	pCoolToolBar->GetItemRect(iSelectedItem, &rectItem);

	COXMenuBar* pMenuBar = DYNAMIC_DOWNCAST(COXMenuBar, pCoolToolBar);

	if (pMenuBar != NULL && iSelectedItem == -1)
	{
		// This is the icon item

		rectItem = pMenuBar->m_iconRect;

		CRect rectWindow;
		pMenuBar->GetWindowRect(rectWindow);
		pMenuBar->ScreenToClient(&rectWindow);

		CRect rectClient;
		pMenuBar->GetClientRect(rectClient);

		rectItem.left += rectWindow.left - rectClient.left;
		rectItem.right += rectWindow.left - rectClient.left;
		rectItem.top += rectWindow.top - rectClient.top;
		rectItem.bottom += rectWindow.top - rectClient.top;

		rectItem.InflateRect(3, 3);
	}
	pCoolToolBar->ClientToScreen(&rectItem);

	// Determine the position of the popup menu relative to the selected item
	CPoint ptTopLeft;
	UINT nFlags, nPosFlags;
	DeterminePosition(this, rectItem, pCoolToolBar->m_dwStyle, ptTopLeft, nFlags, nPosFlags);

	// Clear the queue of any pending messages
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}	

	COXShadowedItemWnd* pSIW = new COXShadowedItemWnd(pCoolToolBar, iSelectedItem, nPosFlags);
	CRect rect;
	pCoolToolBar->GetItemRect(iSelectedItem, rect);
	rect.DeflateRect(0, 1);

	// Save the item rectangle in screen coordinates
	m_rectDropDownItem = rect;
	pCoolToolBar->ClientToScreen(m_rectDropDownItem);

	// Create the transparent window which will be responsible for drawing the shadow
	// The transparent window must be a child of the main window
	rect.InflateRect(0, 0, 4, 4);
	pCoolToolBar->ClientToScreen(rect);
	pSIW->CreateEx(WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW, NULL, NULL, WS_CHILD | WS_VISIBLE, 
		rect, CWnd::GetDesktopWindow(), 0, NULL);

	BOOL bResult = CMenu::TrackPopupMenu(nFlags, ptTopLeft.x, ptTopLeft.y, pWnd, lpRect);

	pSIW->DestroyWindow();
	delete pSIW;
	pSIW = NULL;

	m_rectDropDownItem.SetRectEmpty();

	return bResult;
}

// Returns the size of the popup menu before it is displayed
CSize COXBitmapMenu::GetPopupMenuSize(CMenu* pMenu)
{
	CSize szMenu(0, 0);

	int iCount = pMenu->GetMenuItemCount();
	for (int i = 0; i < iCount; i++)
	{
		UINT nItemID = pMenu->GetMenuItemID(i);

		// Get the data member
		MENUITEMINFO miinfo;
		::memset(&miinfo, 0, sizeof(MENUITEMINFO));
		miinfo.cbSize = sizeof(MENUITEMINFO);
		miinfo.fMask = MIIM_DATA;
		pMenu->GetMenuItemInfo(nItemID, &miinfo);

		// Call CMenu::MeasureItem() to determine the item's dimensions
		MEASUREITEMSTRUCT mi;
		::memset(&mi, 0, sizeof(MEASUREITEMSTRUCT));
		mi.CtlType = ODT_MENU;
		mi.itemID = nItemID;
		mi.itemData = miinfo.dwItemData;

		pMenu->MeasureItem(&mi);

		if (mi.itemHeight == 0 && DYNAMIC_DOWNCAST(COXBitmapMenu, pMenu) == NULL)
		{
			// This is not a COXBitmapMenu
			if (nItemID == 0)
				mi.itemHeight = 8; // separator
			else
				mi.itemHeight = 17; // normal item
		}

		if ((int) mi.itemWidth > szMenu.cx)
			szMenu.cx = mi.itemWidth;

		szMenu.cy += mi.itemHeight;
	}

	// Compensate for the non-client area
	szMenu.cx += 18;
	szMenu.cy += 6;

	return szMenu;
}

void COXBitmapMenu::DeterminePosition(CMenu* pMenu, LPCRECT lpItemRect, DWORD dwStyle, CPoint& ptTopLeft, UINT& nFlags, UINT& nPosFlags)
{
	// Get the rectangle of the monitor closest to the menu rectangle
	HMONITOR hMonitor = ::MonitorFromRect(lpItemRect, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi;
	mi.cbSize = sizeof(MONITORINFO);
	::GetMonitorInfo(hMonitor, &mi);

	const int iMixScreenX = mi.rcMonitor.left;
	const int iMinScreenY = mi.rcMonitor.top;
	const int iMaxScreenX = mi.rcMonitor.right;
	const int iMaxScreenY = mi.rcMonitor.bottom;

	CSize sizeMenu = GetPopupMenuSize(pMenu);
	ptTopLeft.x = 0;
	ptTopLeft.y = 0;
	nFlags = 0;
	if (dwStyle & CBRS_ORIENT_HORZ)
	{
		// Horizontal menu
		ptTopLeft.x = lpItemRect->left;
		if (ptTopLeft.x < iMixScreenX)
			ptTopLeft.x = iMixScreenX;
		if (ptTopLeft.x + sizeMenu.cx > iMaxScreenX)
			ptTopLeft.x = iMaxScreenX - sizeMenu.cx;

		if (lpItemRect->bottom + sizeMenu.cy > iMaxScreenY)
		{
			// The popup menu should be above the item
			nFlags |= TPM_BOTTOMALIGN;
			nPosFlags = OX_TPM_TOP;
			ptTopLeft.y = lpItemRect->top + 1;
		}
		else
		{
			// The popup menu should be below the item
			nFlags |= TPM_TOPALIGN;
			nPosFlags = OX_TPM_BOTTOM;
			ptTopLeft.y = lpItemRect->bottom - 1;
		}
	}
	else
	{
		// Vertical menu
		if (lpItemRect->right + sizeMenu.cx > iMaxScreenX)
		{
			// The popup menu should be left of the item
			nFlags |= TPM_RIGHTALIGN;
			nPosFlags = OX_TPM_LEFT;
			ptTopLeft.x = lpItemRect->left;
		}
		else
		{
			// The popup menu should be right of the item
			nFlags |= TPM_LEFTALIGN;
			nPosFlags = OX_TPM_RIGHT;
			ptTopLeft.x = lpItemRect->right;
		}

		ptTopLeft.y = lpItemRect->top + 1;
		if (ptTopLeft.y < iMinScreenY)
			ptTopLeft.y = iMinScreenY;
		if (ptTopLeft.y + sizeMenu.cy > iMaxScreenY)
			ptTopLeft.y = iMaxScreenY - sizeMenu.cy - 1;
	}
}

