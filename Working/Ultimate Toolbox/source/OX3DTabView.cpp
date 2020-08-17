// ==========================================================================
//							Class Implementation : 
//				COX3DTabViewContainer & COX3DTabViewDropTarget
// ==========================================================================

// Source file : OX3DTabView.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OX3DTabView.h"
#include "OXSkins.h"

#include "UTB64Bit.h"

#ifndef __OXMFCIMPL_H__
#if _MFC_VER >= 0x0700
#if _MFC_VER >= 1400
#include <afxtempl.h>
#endif
#include <..\src\mfc\afximpl.h>
#else
#include <..\src\afximpl.h>
#endif
#define __OXMFCIMPL_H__
#endif

#ifndef __AFXPRIV_H__
#include <afxpriv.h>
#define __AFXPRIV_H__
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Change tab on drag over handler
DROPEFFECT COX3DTabViewDropTarget::OnDragOver(CWnd* pWnd, 
											  COleDataObject* pDataObject,
											  DWORD dwKeyState, 
											  CPoint point)
{
	UNREFERENCED_PARAMETER(pDataObject);
	UNREFERENCED_PARAMETER(dwKeyState);

	// get the pointer to tab control
    COX3DTabViewContainer* pTabViewContainer=(COX3DTabViewContainer*)pWnd;

    if (!pTabViewContainer || !pTabViewContainer->IsAcceptingDraggedObject())
        return DROPEFFECT_NONE;

    TC_HITTESTINFO hitTest;
    hitTest.pt=point;
    
	// find the drop target item 
    int nItem=pTabViewContainer->HitTest(&hitTest);

	// if found ...
    if(nItem>=0 && m_nOldItem!=nItem)
    {
		// activate corresponding page
		pTabViewContainer->SetActivePageIndex(nItem);
	    m_nOldItem=pTabViewContainer->HitTest(&hitTest);
    }	

    return DROPEFFECT_NONE;
}


void COX3DTabViewDropTarget::OnDragLeave(CWnd* pWnd)
{
	m_nOldItem=-1;
	COleDropTarget::OnDragLeave(pWnd);
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(COX3DTabViewContainer, COXSkinnedTabCtrl)

COX3DTabViewContainer::COX3DTabViewContainer() :
	m_rectPage(0,0,0,0),
	m_bAcceptDraggedObject(TRUE),
	m_dwOffsetExternal(ID_3DTABVIEW_OFFSET_EXTERNAL),
	m_dwOffsetInternal(ID_3DTABVIEW_OFFSET_INTERNAL),
	m_nActivePageIndex(-1),
	m_bDrawingNCBackground(FALSE),
	m_bOneTabMode(FALSE)
{
	m_arrUniqueIDs.Add(1);
}


COX3DTabViewContainer::~COX3DTabViewContainer()
{
}


BEGIN_MESSAGE_MAP(COX3DTabViewContainer, COXSkinnedTabCtrl)
	//{{AFX_MSG_MAP(COX3DTabViewContainer)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT_EX(TCN_SELCHANGE, OnSelchange)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL COX3DTabViewContainer::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
								   DWORD dwStyle, const RECT& rect, 
								   CWnd* pParentWnd, UINT nID,
								   CCreateContext* pContext/*=NULL*/)
{
	if(lpszClassName && lstrcmpi(lpszClassName,_T("SysTabControl32"))==0)
	{
		if(!CWnd::Create(lpszClassName,lpszWindowName,dwStyle,rect,
			pParentWnd,nID,pContext))
		{
			return FALSE;
		}
		// set default font
		SendMessage(WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT));

		// register drop-target
		if(!m_dropTarget.Register(this))
		{
			TRACE(_T("COX3DTabViewContainer::Create: failed to register the control with COleDropTarget\n"));
			TRACE(_T("you've probably forgot to initialize OLE libraries using AfxOleInit function\n"));
		}
		return TRUE;
	}
	else
	{
		TRACE(_T("COX3DTabViewContainer::Create: invalid window class name has been specified <%s>. Ignore\n"),lpszClassName);
		if(!Create(pParentWnd,rect,DEFAULT_3DTABCTRLSTYLE,nID))
		{
			return FALSE;
		}
		SetWindowText(lpszWindowName);
		return TRUE;
	}
}

BOOL COX3DTabViewContainer::Create(CWnd* pParentWnd, CRect rect/*=CRect(0,0,0,0)*/,
								   DWORD dwStyle/*=DEFAULT_3DTABCTRLSTYLE*/, 
								   UINT nID/*=AFX_IDW_PANE_FIRST*/)
{
	ASSERT(pParentWnd != NULL);
	ASSERT(dwStyle & WS_CHILD);
	ASSERT(nID!=0);

	if(!CTabCtrl::Create(dwStyle,rect,pParentWnd,nID))
	{
		TRACE(_T("COX3DTabViewContainer::Create: Failed to create the tab control\n"));
		return FALSE;      
	}

	return TRUE;
}


int COX3DTabViewContainer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CTabCtrl::OnCreate(lpCreateStruct)==-1)
	{
		return -1;
	}

	ModifyStyleEx(0,WS_EX_CONTROLPARENT);

	return 0;
}

	
void COX3DTabViewContainer::OnNcCalcSize(BOOL bCalcValidRects, 
										 NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	// TODO: Add your message handler code here and/or call default
	
	CTabCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);

	// add an offset
	if(lpncsp->rgrc[0].bottom-lpncsp->rgrc[0].top>2*(int)m_dwOffsetExternal)
	{
		lpncsp->rgrc[0].top+=m_dwOffsetExternal;
		lpncsp->rgrc[0].bottom-=m_dwOffsetExternal;
	}
	else
	{
		lpncsp->rgrc[0].top=lpncsp->rgrc[0].bottom;
	}

	if(lpncsp->rgrc[0].right-lpncsp->rgrc[0].left>2*(int)m_dwOffsetExternal)
	{
		lpncsp->rgrc[0].left+=m_dwOffsetExternal;
		lpncsp->rgrc[0].right-=m_dwOffsetExternal;
	}
	else
	{
		lpncsp->rgrc[0].left=lpncsp->rgrc[0].right;
	}
}

void COX3DTabViewContainer::OnNcPaint() 
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
	{
		dc.ExcludeClipRect(rectClient);
	}

	m_bDrawingNCBackground=TRUE;
	// erase parts not drawn
	SendMessage(WM_ERASEBKGND,(WPARAM)dc.m_hDC);
	m_bDrawingNCBackground=FALSE;

	if(GetExStyle() & WS_EX_CLIENTEDGE)
	{
		// draw borders in non-client area
		rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);
		dc.Draw3dRect(rectWindow,::GetSysColor(COLOR_BTNSHADOW),
			::GetSysColor(COLOR_BTNHILIGHT));
	}

	// Do not call CTabCtrl::OnNcPaint() for painting messages
}


void COX3DTabViewContainer::OnSize(UINT nType, int cx, int cy) 
{
	CTabCtrl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(nType!=SIZE_MINIMIZED && cx>0 && cy>0)
	{
		UpdatePages();
	}
}


BOOL COX3DTabViewContainer::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	if(GetPageCount()!=0 && !m_bDrawingNCBackground)
	{
		CRect rectPage=GetPageRect();
		pDC->ExcludeClipRect(rectPage);
	} 
	
	CRect rect;
	GetWindowRect(rect);
	ScreenToClient(rect);
	rect.right += m_dwOffsetExternal;
	rect.bottom += m_dwOffsetExternal;

	CBrush brush;
	brush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	brush.UnrealizeObject();

	pDC->FillRect(rect, &brush);
	return TRUE;
}


void COX3DTabViewContainer::OnDestroy() 
{
	SetActivePageIndex(-1);
	int nCount=GetPageCount();
	for(int nIndex=nCount-1; nIndex>=0; nIndex--)
	{
		DeletePage(nIndex,TRUE);
	}
	CTabCtrl::OnDestroy();
}


void COX3DTabViewContainer::OnSetFocus(CWnd* pOldWnd)
{
	UNREFERENCED_PARAMETER(pOldWnd);

	CWnd* pActivePageWnd=GetActivePage();
	if(pActivePageWnd!=NULL)
	{
		ASSERT(::IsWindow(pActivePageWnd->GetSafeHwnd()));
		pActivePageWnd->SetFocus();
	}
}

//////////////////////////////////////////////////////////////////////////////
// Description: Selection change handler
//////////////////////////////////////////////////////////////////////////////
BOOL COX3DTabViewContainer::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);

	int nItem=GetCurSel();
    if(nItem>=0)
    {
		SetActivePageIndex(nItem);
    }

	*pResult = 0;

	return FALSE;
}



BOOL COX3DTabViewContainer::InsertPage(int nIndex, 
									   CRuntimeClass* pClass, 
									   CCreateContext* pContext,
									   LPCTSTR lpszTitle/*=NULL*/, 
									   int nImage/*=-1*/,
									   DWORD dwExStyle/*=OX3DTABVIEWCONTAINER_EX_STYLE*/, 
									   BOOL bActivate/*=TRUE*/)
{
	ASSERT_VALID(this);
	ASSERT(nIndex>=0 && nIndex<=GetPageCount());
	ASSERT(pClass!=NULL);
	ASSERT(pClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pClass,sizeof(CRuntimeClass),FALSE));

	if(!(nIndex>=0 && nIndex<=GetPageCount()) || pClass==NULL)
		return FALSE;

	CCreateContext context;
	if(pContext==NULL)
	{
		// if no context specified, generate one from the currently active
		// view if possible
		CView* pOldView=(CView*)GetActivePage();
		if(pOldView!=NULL && pOldView->IsKindOf(RUNTIME_CLASS(CView)))
		{
			// set info about last pane
			ASSERT(context.m_pCurrentFrame==NULL);
			context.m_pLastView=pOldView;
			context.m_pCurrentDoc=pOldView->GetDocument();
			if(context.m_pCurrentDoc!=NULL)
			{
				context.m_pNewDocTemplate=context.m_pCurrentDoc->
					GetDocTemplate();
			}
		}
		pContext=&context;
	}

	CWnd* pWnd;
	TRY
	{
		pWnd=(CWnd*)pClass->CreateObject();
		if(pWnd==NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE(_T("COX3DTabViewContainer::InsertPage: Out of memory inserting new page\n"));
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL

	ASSERT_KINDOF(CWnd,pWnd);
	ASSERT(pWnd->m_hWnd==NULL);       // not yet created

	DWORD dwStyle=AFX_WS_DEFAULT_VIEW;
	if(!afxData.bWin95)
	{
		dwStyle&=~WS_BORDER;
	}

	DWORD dwID=GetUniqueId();

	// Create with the right size
	if(!pWnd->Create(NULL,NULL,dwStyle,CRect(0,0,0,0),this,dwID,pContext))
	{
		TRACE(_T("COX3DTabViewContainer::InsertPage: couldn't create new page\n"));
		// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}

	if(InsertPage(nIndex,pWnd,lpszTitle,nImage,dwExStyle,bActivate))
	{
		CWnd* pWnd=GetPage(nIndex);
		ASSERT(pWnd!=NULL);
		ASSERT(::IsWindow(pWnd->m_hWnd));
		if(pWnd->IsKindOf(RUNTIME_CLASS(CView)))
		{
			// send initial notification message
			pWnd->SendMessage(WM_INITIALUPDATE);
		}
		return TRUE;
	}

	return FALSE;
}

BOOL COX3DTabViewContainer::InsertPage(int nIndex, CWnd* pWnd, 
									   LPCTSTR lpszTitle/*=NULL*/, 
									   int nImage/*=-1*/,
									   DWORD dwExStyle/*=OX3DTABVIEWCONTAINER_EX_STYLE*/, 
									   BOOL bActivate/*=TRUE*/)
{
	ASSERT_VALID(this);
	ASSERT(nIndex>=0 && nIndex<=GetPageCount());
	ASSERT(pWnd!=NULL);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	if(!(nIndex>=0 && nIndex<=GetPageCount()) || 
		pWnd==NULL || !::IsWindow(pWnd->m_hWnd))
	{
		TRACE(_T("COX3DTabViewContainer::InsertPage: failed to insert page, some of the parameters has been specified wrong\n"));
		return FALSE;
	}

	// set this container as parent window of the specified page
	pWnd->SetParent(this);

	TCITEM tcItem;
	tcItem.mask=TCIF_IMAGE|TCIF_PARAM|TCIF_TEXT;
	tcItem.pszText=(LPTSTR)lpszTitle;
	tcItem.iImage=nImage;
	tcItem.lParam=(LPARAM)pWnd;
	if(InsertItem(nIndex,&tcItem)!=nIndex)
	{
		TRACE(_T("COX3DTabViewContainer::InsertPage: failed to insert new button in the tab control\n"));
		return FALSE;
	}

	if(nIndex==m_nActivePageIndex)
	{
		m_nActivePageIndex++;
	}

	// set additional extended style if any specified
	if(dwExStyle!=0)
	{
		pWnd->ModifyStyleEx(0,dwExStyle);
	}
	pWnd->ModifyStyle(0,WS_TABSTOP);

	if(bActivate)
	{
		SetActivePageIndex(nIndex);
	}

	return TRUE;
}


BOOL COX3DTabViewContainer::DeletePage(const CWnd* pWnd, BOOL bDestroy/*=TRUE*/) 
{
	ASSERT(pWnd!=NULL);
	ASSERT(IsPage(pWnd));
	
	int nIndex=-1;
	if(FindPage(pWnd,nIndex))
	{
		return DeletePage(nIndex,bDestroy);
	}

	return FALSE;
}


BOOL COX3DTabViewContainer::DeletePage(int nIndex, BOOL bDestroy/*=TRUE*/)
{
	ASSERT_VALID(this);
	ASSERT(nIndex>=0 && nIndex<GetPageCount());

	if(nIndex>=0 && nIndex<GetPageCount())
	{
		// if active page is being deleted - activate next
		int nActivePage=GetActivePageIndex();
		
		BOOL bUpdateActivePage=FALSE;

		if(nActivePage==nIndex)
		{
			if(nActivePage==GetPageCount()-1)
			{
				nActivePage=(GetPageCount()==1 ? -1 : 0);
			}
			else
			{
				ASSERT(GetPageCount()>1);
				nActivePage++;
			}
			SetActivePageIndex(nActivePage);

			bUpdateActivePage=(nActivePage>=0);
		}

		CWnd* pWnd=GetPage(nIndex);
		ASSERT(pWnd!=NULL);

		if(bDestroy && ::IsWindow(pWnd->m_hWnd))
		{
			m_arrUniqueIDs.Add(pWnd->GetDlgCtrlID());
			VERIFY(pWnd->DestroyWindow());
		}

		DeleteItem(nIndex);

		if(bUpdateActivePage)
		{
			m_nActivePageIndex=nActivePage ? (nActivePage-1) : 0;
		}
		
		return TRUE;
	}

	return FALSE;
}


BOOL COX3DTabViewContainer::SetActivePageIndex(int nIndex)
{
	if(nIndex==m_nActivePageIndex)
	{
		return TRUE;
	}

	CWnd* pWndOld=GetActivePage();
	if(pWndOld!=NULL)
	{
		ASSERT(::IsWindow(pWndOld->m_hWnd));
		if(::IsWindow(pWndOld->m_hWnd))
		{
			pWndOld->ShowWindow(SW_HIDE);
		}
	}

	m_nActivePageIndex=nIndex;
	SetCurSel(nIndex);

	if(m_nActivePageIndex>=0 && m_nActivePageIndex<GetPageCount())
	{
		CWnd* pWnd=GetPage(m_nActivePageIndex);
		if(pWnd!=NULL)
		{
			ASSERT(::IsWindow(pWnd->m_hWnd));
			if(::IsWindow(pWnd->m_hWnd))
			{
				UpdatePages();
				pWnd->ShowWindow(SW_SHOW);

				BOOL bSetFocus=IsChild(CWnd::GetFocus());

				// set the focus to the page
				CFrameWnd* pFrameWnd=(CFrameWnd*)GetParent();
				ASSERT(pFrameWnd!=NULL);
				if(pFrameWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
				{
					if(pWnd->IsKindOf(RUNTIME_CLASS(CView)))
					{
						pFrameWnd->SetActiveView((CView*)pWnd);
					}
					else
					{
						if(pWndOld!=NULL && 
							pWndOld->IsKindOf(RUNTIME_CLASS(CView)))
						{
							pFrameWnd->SetActiveView(NULL);
						}
						if(bSetFocus)
						{
							pWnd->SetFocus();
						}
					}
				}
				else
				{
					if(bSetFocus)
					{
						pWnd->SetFocus();
					}
				}

				pWnd->RedrawWindow();
			}
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}


void COX3DTabViewContainer::RecalcPageRect() 
{
	CRect rectPage(0,0,0,0);
	GetWindowRect(rectPage);
	rectPage.DeflateRect(GetOffsetExternal(),GetOffsetExternal());
	ScreenToClient(rectPage);

	// This code is introduced because of severe bug in CTabCtrl MFC 
	// class, which caused a crash. Crash will appear if only one tab 
	// is present. This code will add temporarily one more tab and 
	// hide tabs.
	if(!rectPage.IsRectEmpty() && rectPage.left<rectPage.right && 
		rectPage.top<rectPage.bottom)
	{
		// The number of tabs
		int nNumOfItems = this->GetItemCount();

		// Find the size of Tabs
		RECT itemRect;
		this->GetItemRect(nNumOfItems-1, &itemRect);
		if (nNumOfItems == 1 && !m_bOneTabMode || nNumOfItems == 2 && m_bOneTabMode)
		{
			if( itemRect.right - itemRect.left + 50 < rectPage.right - rectPage.left )
			{
			
				if( m_bOneTabMode == TRUE )
				{
					this->DeleteItem(nNumOfItems-1);
					m_bOneTabMode = FALSE;
				}
				
				AdjustRect(FALSE,rectPage);
			
				rectPage.DeflateRect(GetOffsetInternal(),GetOffsetInternal());
			}
			else
			{
				if( nNumOfItems == 1 )
				{
					//this->InsertItem(nNumOfItems,_T(""));
					m_bOneTabMode = TRUE;
				}
			}
		}
		else
		{
			AdjustRect(FALSE,rectPage);
			rectPage.DeflateRect(GetOffsetInternal(),GetOffsetInternal());
		}
	}

	m_rectPage=rectPage;
	m_rectPage.DeflateRect(1, 1, 1, 2);
	GetTabSkin()->AdjustChildItemRect(m_rectPage, 4, this);
}


DWORD COX3DTabViewContainer::GetUniqueId() 
{ 
	int nCount=PtrToInt(m_arrUniqueIDs.GetSize());
	ASSERT(nCount>0);
	DWORD dwUniqueID=m_arrUniqueIDs[nCount-1]; 
	if(nCount==1)
	{
		m_arrUniqueIDs.SetAt(nCount-1,dwUniqueID+1);
	}
	return dwUniqueID; 
}


void COX3DTabViewContainer::UpdatePages() 
{
	CWnd* pWnd=GetActivePage();
	if(pWnd!=NULL)
	{
		ASSERT(::IsWindow(pWnd->m_hWnd));
		if(::IsWindow(pWnd->m_hWnd))
		{
			RecalcPageRect();
			pWnd->MoveWindow(GetPageRect());
		}
	}
}

