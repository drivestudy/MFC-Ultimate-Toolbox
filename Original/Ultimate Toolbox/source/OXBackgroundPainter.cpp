// OXBackgroundPainter.cpp : implementation file
//

// Version: 9.3

#include "stdafx.h"
#include "OXBackgroundPainter.h"
#include "UTB64Bit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXBackgroundPainter

COXBackgroundPainter::COXBackgroundPainter()
{
	m_paintType=Tile;
	m_clrBk=CLR_DEFAULT;
	m_rectWindowSaved.SetRectEmpty();
	m_pOriginWnd=NULL;
	m_sImageFileName.Empty();
}

COXBackgroundPainter::~COXBackgroundPainter()
{
}

BOOL COXBackgroundPainter::Attach(CWnd* pWnd, 
								  const COXDIB* pDIB, 
								  PaintType paintType/*=Tile*/,
								  COLORREF clrBk/*=CLR_DEFAULT*/, 
								  CWnd* pOriginWnd/*=NULL*/)
{
	ASSERT(pWnd!=NULL);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	m_sImageFileName.Empty();

	HookWindow(pWnd);

	SetBkColor(clrBk,FALSE);
	SetWallpaper(pDIB,FALSE);
	SetPaintType(paintType,FALSE);
	SetOriginWnd(pOriginWnd,TRUE);

	return TRUE;
}

void COXBackgroundPainter::Detach(BOOL bRedraw/*=TRUE*/)
{
	CWnd* pWnd=GetHookedWnd();
	ASSERT(pWnd!=NULL);

	UnhookWindow();

	if(bRedraw && ::IsWindow(pWnd->m_hWnd))
	{
		pWnd->RedrawWindow(NULL,NULL,
			RDW_INVALIDATE|RDW_ERASE|RDW_FRAME|RDW_UPDATENOW);
	}
}

LRESULT COXBackgroundPainter::WindowProc(UINT msg, WPARAM wp, LPARAM lp)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	ASSERT(::IsWindow(m_hWndHooked));

	switch (msg) 
	{
	case WM_PALETTECHANGED:
	    if((HWND)wp!=m_hWndHooked)
		{
			DoRealizePalette(TRUE,FALSE);
		}
		return 0;

	case WM_QUERYNEWPALETTE:
		return DoRealizePalette(TRUE,TRUE)>0 ? TRUE : FALSE;

	case WM_SETFOCUS:
		DoRealizePalette(TRUE,TRUE);
		break;

	case WM_SIZE:
		if(m_paintType!=Tile && m_paintType!=TopLeft)
		{
			GetHookedWnd()->RedrawWindow();
		}
		break;

	case WM_ERASEBKGND:
		{
			CRect rect;
			::GetWindowRect(m_hWndHooked,rect);
			if(m_rectWindowSaved!=rect)
			{
				m_rectWindowSaved=rect;
				::RedrawWindow(m_hWndHooked,NULL,NULL,
					RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|
					RDW_FRAME|RDW_ERASENOW|RDW_INTERNALPAINT|RDW_ALLCHILDREN);
			}	
			else
			{
				HDC hdc=(HDC)wp;
				ASSERT(hdc!=NULL);

				CDC dc;
				dc.Attach(hdc);
				CRect rect;
				::GetWindowRect(m_hWndHooked,rect);
				GetHookedWnd()->ScreenToClient(rect);
				rect-=rect.TopLeft();
				dc.IntersectClipRect(rect);

				CWnd* pWndChild=GetHookedWnd()->GetWindow(GW_CHILD);
				while(pWndChild!=NULL)
				{
					CRect rect;
					pWndChild->GetWindowRect(rect);
					GetHookedWnd()->ScreenToClient(rect);
				//	dc.ExcludeClipRect(rect);

					pWndChild=pWndChild->GetWindow(GW_HWNDNEXT);
				}

				DrawWallpaper(&dc);
				dc.Detach();
			}
			return TRUE;
		}
		break;
	}

	return COXHookWnd::WindowProc(msg, wp, lp);
}

void COXBackgroundPainter::DrawWallpaper(CDC* pDC) 
{
	ASSERT(::IsWindow(m_hWndHooked));
	ASSERT(pDC!=NULL);
	ASSERT_VALID(pDC);

	if (pDC==NULL)
		return;

	if(m_dibWallpaper.IsEmpty())
	{
		FillBackground(pDC);
		TRACE(_T("COXBackgroundPainter::DrawWallpaper - empty DIB, no painting will occur.\n"));
		return;
	}

	if(m_paintType!=Tile && m_paintType!=Stretch)
		FillBackground(pDC);
	
	CRect rectWindow=GetPaintableRect(GetHookedWnd());

	CPoint ptStart;
	CRect rectContainer;
	GetWallpaperOrigin(ptStart,rectContainer);

	CPoint ptDCOrigin;
	::GetDCOrgEx(pDC->GetSafeHdc(),&ptDCOrigin);
	ptStart.x-=ptDCOrigin.x-rectWindow.left;
	ptStart.y-=ptDCOrigin.y-rectWindow.top;

	rectWindow-=rectWindow.TopLeft();

	CSize size=m_dibWallpaper.GetSize();
	int cx, cy;

	CRect rect;
	CRect rectImage=CRect(0,0,size.cx,size.cy);

	// adjust the start point
	if(m_paintType==Tile)
	{
		if(ptStart.x<0)
			ptStart.x+=((-ptStart.x)/size.cx)*size.cx;
		if(ptStart.y<0)
			ptStart.y+=((-ptStart.y)/size.cy)*size.cy;
	}

	// draw the bitmap
	switch (m_paintType)
	{
	case Tile:
		for(cx=ptStart.x; cx<rectWindow.Width(); cx+=size.cx)
		{
			for(cy=ptStart.y; cy<rectWindow.Height(); cy+=size.cy)
			{
				rect=CRect(cx,cy,cx+size.cx,cy+size.cy);
				m_dibWallpaper.Paint(pDC,rect,rectImage);
			}
		}
		break;

	case Stretch:
		rect=CRect(ptStart.x,ptStart.y,ptStart.x+rectContainer.Width(),
			ptStart.y+rectContainer.Height());
		m_dibWallpaper.Paint(pDC,rect,rectImage);
		break;

	case TopLeft:
		rect=CRect(ptStart.x,ptStart.y,size.cx+ptStart.x,
			size.cy+ptStart.y);
		m_dibWallpaper.Paint(pDC,rect,rectImage);
		break;

	case TopRight:
		rect=CRect(rectContainer.Width()+ptStart.x-size.cx,ptStart.y,
			rectContainer.Width()+ptStart.x,size.cy+ptStart.y);
		m_dibWallpaper.Paint(pDC,rect,rectImage);
		break;

	case TopCenter:
		rect.left=(rectContainer.Width()-size.cx)/2+ptStart.x;
		rect.top=ptStart.y;
		rect.right=rect.left+size.cx;
		rect.bottom=rect.top+size.cy;
		m_dibWallpaper.Paint(pDC,rect,rectImage);
		break;

	case LeftCenter:
		rect.left=ptStart.x;
		rect.top=(rectContainer.Height()-size.cy)/2+ptStart.y;
		rect.right=rect.left+size.cx;
		rect.bottom=rect.top+size.cy;
		m_dibWallpaper.Paint(pDC,rect,rectImage);
		break;

	case RightCenter:
		rect.left=rectContainer.Width()+ptStart.x-size.cx;
		rect.top=(rectContainer.Height()-size.cy)/2+ptStart.y;
		rect.right=rect.left+size.cx;
		rect.bottom=rect.top+size.cy;
		m_dibWallpaper.Paint(pDC,rect,rectImage);
		break;

	case Center:
		rect.left=(rectContainer.Width()-size.cx)/2+ptStart.x;
		rect.top=(rectContainer.Height()-size.cy)/2+ptStart.y;
		rect.right=rect.left+size.cx;
		rect.bottom=rect.top+size.cy;
		m_dibWallpaper.Paint(pDC,rect,rectImage);
		break;

	case BottomLeft:
		rect=CRect(ptStart.x,rectContainer.Height()+ptStart.y-size.cy,
			size.cx+ptStart.x,rectContainer.Height()+ptStart.y);
		m_dibWallpaper.Paint(pDC,rect,rectImage);
		break;

	case BottomRight:
		rect=CRect(rectContainer.Width()+ptStart.x-size.cx,
			rectContainer.Height()+ptStart.y-size.cy,
			rectContainer.Width()+ptStart.x,rectContainer.Height()+ptStart.y);
		m_dibWallpaper.Paint(pDC,rect,rectImage);
		break;

	case BottomCenter:
		rect.left=(rectContainer.Width()-size.cx)/2+ptStart.x;
		rect.top=rectContainer.Height()+ptStart.y-size.cy;
		rect.right=rect.left+size.cx;
		rect.bottom=rect.top+size.cy;
		m_dibWallpaper.Paint(pDC,rect,rectImage);
		break;

	default:
		ASSERT(FALSE);
		TRACE(_T("Unknown paint type used in COXCoolToolBar::DrawWallpaper function\n"));
		break;
	}
}


void COXBackgroundPainter::GetWallpaperOrigin(CPoint& ptOrigin, 
											  CRect& rectContainer) const 
{ 
	ptOrigin=CPoint(0,0);
	rectContainer.SetRectEmpty();

	CWnd* pOriginWnd=m_pOriginWnd;
	if(pOriginWnd!=GetHookedWnd() && 
		GetHookedWnd()->IsKindOf(RUNTIME_CLASS(CControlBar)) && 
		((CControlBar*)GetHookedWnd())->IsFloating())
	{
		pOriginWnd=((CControlBar*)GetHookedWnd())->GetDockingFrame();
	}
	ASSERT(pOriginWnd!=NULL);

	rectContainer=GetPaintableRect(pOriginWnd);
	if(pOriginWnd!=GetHookedWnd())
	{
		CRect rectWindow=GetPaintableRect(GetHookedWnd());
		ptOrigin=rectContainer.TopLeft()-rectWindow.TopLeft();
	}
}

	
void COXBackgroundPainter::FillBackground(CDC* pDC) const
{
	ASSERT(::IsWindow(m_hWndHooked));
	ASSERT(pDC!=NULL);
	ASSERT_VALID(pDC);

	if (pDC==NULL)
		return;

	CRect rectWindow;
	::GetWindowRect(m_hWndHooked,rectWindow);
	rectWindow-=rectWindow.TopLeft();

	BOOL bDestroyBrush=TRUE;
	HBRUSH hBrush=NULL;
	if(m_clrBk==CLR_DEFAULT)
	{
		hBrush=(HBRUSH)(DWORD_PTR)::GetClassLongPtr(m_hWndHooked,GCL_HBRBACKGROUND);
		if(hBrush==NULL)
			hBrush=CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
		else
			bDestroyBrush=FALSE;
	}
	else
		hBrush=CreateSolidBrush(m_clrBk);
	ASSERT(hBrush!=NULL);

	CBrush* pBrush=CBrush::FromHandle(hBrush);
	pDC->FillRect(rectWindow,pBrush);

	if(bDestroyBrush)
		VERIFY(pBrush->DeleteObject());
}


CRect COXBackgroundPainter::GetPaintableRect(CWnd* pWnd) const
{
	ASSERT(pWnd!=NULL);

	CRect rect(0,0,0,0);
	if (!::IsWindow(pWnd->GetSafeHwnd()))
		return rect;

	pWnd->GetWindowRect(rect);

	DWORD dwStyle=pWnd->GetStyle();

	if(dwStyle & WS_BORDER)
	{
		// Get size of frame around window
		CSize szFrame=(dwStyle & WS_THICKFRAME) ?	
			CSize(GetSystemMetrics(SM_CXSIZEFRAME), GetSystemMetrics(SM_CYSIZEFRAME)) :
			CSize(GetSystemMetrics(SM_CXFIXEDFRAME), GetSystemMetrics(SM_CYFIXEDFRAME));
		rect.DeflateRect(szFrame);
	}

	if(dwStyle & WS_CAPTION)
	{
		rect.top+=(((pWnd->GetExStyle()&WS_EX_TOOLWINDOW)==WS_EX_TOOLWINDOW) ? 
			GetSystemMetrics(SM_CYSMCAPTION) : GetSystemMetrics(SM_CYCAPTION));				
	}

	return rect;
}


UINT COXBackgroundPainter::DoRealizePalette(BOOL bRedraw/*=TRUE*/, 
											BOOL bForeground/*=TRUE*/)
{
	if(m_dibWallpaper.IsEmpty())
		return 0;

	UINT nCount=0;
	// retrieve image palette
	CPalette* pPalette=m_dibWallpaper.GetPalette();
	if(pPalette!=NULL)
	{
		CClientDC dc(GetHookedWnd());
		CPalette* pOldPalette=dc.SelectPalette(pPalette,!bForeground);
		nCount=dc.RealizePalette();
		if(nCount>0)
		{
			// if image pallete is selected then
			// redraw image if needed
			if(bRedraw)
				::RedrawWindow(m_hWndHooked,NULL,NULL,
					RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|
					RDW_FRAME|RDW_ERASENOW|RDW_INTERNALPAINT|
					RDW_ALLCHILDREN);
			dc.SelectPalette(pOldPalette,!bForeground);
		}
	}

	return nCount;
}


BOOL COXBackgroundPainter::SaveState(LPCTSTR lpszProfileName)
{
	if(!IsHooked())
		return FALSE;

	CWinApp* pApp=AfxGetApp();

	// we use default registry key assigned to your application by MFC
	pApp->WriteProfileString(lpszProfileName,_T("ImageFileName"),
		GetWallpaperImageFileName());
	pApp->WriteProfileInt(lpszProfileName,_T("PaintType"),GetPaintType());
	pApp->WriteProfileInt(lpszProfileName,_T("BackgroundColor"),GetBkColor());

	return TRUE;
}

BOOL COXBackgroundPainter::LoadState(LPCTSTR lpszProfileName, 
									 BOOL bApply/*=TRUE*/)
{
	if(!IsHooked())
		return FALSE;

	CWinApp* pApp=AfxGetApp();

	// we use default registry key assigned to your application by MFC
	CString sImageFileName=pApp->GetProfileString(lpszProfileName,
		_T("ImageFileName"),_T(""));
	int nPaintType=pApp->GetProfileInt(lpszProfileName,_T("PaintType"),-1);
	COLORREF clrBk=pApp->GetProfileInt(lpszProfileName,
		_T("BackgroundColor"),CLR_NONE);

	if(clrBk!=CLR_NONE)
		SetBkColor(clrBk,FALSE);
	if(nPaintType!=-1)
		SetPaintType((PaintType)nPaintType,FALSE);
	if(!sImageFileName.IsEmpty())
		SetWallpaper(sImageFileName,FALSE);

	if(bApply && ::IsWindow(m_hWndHooked))
	{
		::RedrawWindow(m_hWndHooked,NULL,NULL,
			RDW_INVALIDATE|RDW_ERASE|RDW_FRAME|RDW_UPDATENOW);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////


// static variables
CMap<DWORD,DWORD,COXBackgroundPainterOrganizer*,COXBackgroundPainterOrganizer*> 
COXBackgroundPainterOrganizer::m_arrFrameOrganizers;

HHOOK COXBackgroundPainterOrganizer::m_pfnOriginalCBTHookProc=NULL;
HHOOK COXBackgroundPainterOrganizer::m_pfnOriginalGetMessageHookProc=NULL;
/////////////////

COXBackgroundPainterOrganizer::~COXBackgroundPainterOrganizer() 
{ 
	if(IsAttachedFrame())
	{
		DetachFrame();
	}
	else
	{
		Detach(NULL); 
	}

	int nCount= PtrToInt(m_arrUsedPainters.GetSize());
	for(int nIndex=0; nIndex<nCount; nIndex++)
	{
		COXBackgroundPainter* pPainter=m_arrUsedPainters[nIndex];
		ASSERT(pPainter!=NULL);
		ASSERT(!pPainter->IsHooked());
		delete pPainter;
	}
	m_arrUsedPainters.RemoveAll();

	ASSERT(m_pfnOriginalCBTHookProc==NULL);
	ASSERT(m_pfnOldCBTHookProc==NULL);
	ASSERT(m_pfnOriginalGetMessageHookProc==NULL);
}


BOOL COXBackgroundPainterOrganizer::Attach(CWnd* pWnd, 
										   const COXDIB* pDIB, 
										   PaintType paintType/*=Tile*/,
										   COLORREF clrBk/*=CLR_DEFAULT*/,
										   CWnd* pOriginWnd/*=NULL*/)
{
	ASSERT(pWnd!=NULL);
	if(pWnd==NULL)
		return FALSE;

	CWnd* pWndAttached=pWnd;
	COXBackgroundPainter* pPainter=NULL;
	if(m_arrAttachedWnd.Lookup(pWndAttached->GetSafeHwnd(),pPainter))
	{
		ASSERT(pPainter!=NULL);
		TRACE(_T("COXBackgroundPainterOrganizer::Attach: specified window already attached to a background painter object\n"));
		return FALSE;
	}

	if(m_arrUsedPainters.GetSize()>0)
	{
		pPainter=m_arrUsedPainters[0];
		ASSERT(pPainter!=NULL);
		ASSERT(!pPainter->IsHooked());
		m_arrUsedPainters.RemoveAt(0);
	}
	else
	{
		pPainter=new COXBackgroundPainter;
	}
	if(pPainter->Attach(pWnd,pDIB,paintType,clrBk,pOriginWnd))
	{
		m_arrAttachedWnd.SetAt(pWnd->GetSafeHwnd(),pPainter);
		return TRUE;
	}
	else
	{
		// save the object in the array of COXBackgroundPainter objects 
		// that can be used later
		m_arrUsedPainters.Add(pPainter);
		return FALSE;
	}
}

BOOL COXBackgroundPainterOrganizer::Detach(const CWnd* pWnd/*=NULL*/,
										   BOOL bRedraw/*=TRUE*/)
{
	if(pWnd==NULL)
	{
		POSITION pos=m_arrAttachedWnd.GetStartPosition();
		HWND hAttachedWnd=NULL;
		COXBackgroundPainter* pPainter=NULL;
		while(pos!=NULL)
		{
			m_arrAttachedWnd.GetNextAssoc(pos,hAttachedWnd,pPainter);
			ASSERT(hAttachedWnd!=NULL);
			ASSERT(pPainter!=NULL);
			if(pPainter!=NULL)
			{
				if(pPainter->IsHooked())
				{
					pPainter->Detach(bRedraw);
				}
				// save the object in the array of COXBackgroundPainter objects 
				// that can be used later
				m_arrUsedPainters.Add(pPainter);
			}
		}
		m_arrAttachedWnd.RemoveAll();
	}
	else
	{
		COXBackgroundPainter* pPainter=NULL;
		CWnd* pAttachedWnd=(CWnd*)pWnd;
		if(!m_arrAttachedWnd.Lookup(pAttachedWnd->GetSafeHwnd(),pPainter))
			return FALSE;
		ASSERT(pPainter!=NULL);
		m_arrAttachedWnd.RemoveKey(pAttachedWnd->GetSafeHwnd());
		if(pPainter!=NULL)
		{
			if(pPainter->IsHooked())
			{
				pPainter->Detach(bRedraw);
			}
			// save the object in the array of COXBackgroundPainter objects 
			// that can be used later
			m_arrUsedPainters.Add(pPainter);
		}
	}

	return TRUE;
}

BOOL COXBackgroundPainterOrganizer::
SetWallpaper(const CWnd* pWnd, const CString& sDIBFileName, BOOL bRedraw/*=TRUE*/)
{
	if(pWnd==NULL)
	{
		POSITION pos=m_arrAttachedWnd.GetStartPosition();
		HWND hAttachedWnd=NULL;
		COXBackgroundPainter* pPainter=NULL;
		while(pos!=NULL)
		{
			m_arrAttachedWnd.GetNextAssoc(pos,hAttachedWnd,pPainter);
			ASSERT(hAttachedWnd!=NULL);
			ASSERT(pPainter!=NULL);
			if(pPainter!=NULL && pPainter->IsHooked())
				pPainter->SetWallpaper(sDIBFileName,bRedraw);
		}
	}
	else
	{
		COXBackgroundPainter* pPainter=NULL;
		CWnd* pAttachedWnd=(CWnd*)pWnd;
		if(!m_arrAttachedWnd.Lookup(pAttachedWnd->GetSafeHwnd(),pPainter))
			return FALSE;
		ASSERT(pPainter!=NULL);
		if(pPainter!=NULL && pPainter->IsHooked())
			pPainter->SetWallpaper(sDIBFileName,bRedraw);
	}

	return TRUE;
}

BOOL COXBackgroundPainterOrganizer::
SetWallpaper(const CWnd* pWnd, const COXDIB* pDIB, BOOL bRedraw/*=TRUE*/)
{
	if(pWnd==NULL)
	{
		if(IsAttachedFrame())
		{
			if(pDIB!=NULL)
				m_threadOrigDIB=*pDIB;
			else
				m_threadOrigDIB.Empty();
		}
		POSITION pos=m_arrAttachedWnd.GetStartPosition();
		HWND hAttachedWnd=NULL;
		COXBackgroundPainter* pPainter=NULL;
		while(pos!=NULL)
		{
			m_arrAttachedWnd.GetNextAssoc(pos,hAttachedWnd,pPainter);
			ASSERT(hAttachedWnd!=NULL);
			ASSERT(pPainter!=NULL);
			if(pPainter!=NULL && pPainter->IsHooked())
				pPainter->SetWallpaper(pDIB,bRedraw);
		}
	}
	else
	{
		COXBackgroundPainter* pPainter=NULL;
		CWnd* pAttachedWnd=(CWnd*)pWnd;
		if(!m_arrAttachedWnd.Lookup(pAttachedWnd->GetSafeHwnd(),pPainter))
			return FALSE;
		ASSERT(pPainter!=NULL);
		if(pPainter!=NULL && pPainter->IsHooked())
			pPainter->SetWallpaper(pDIB,bRedraw);
	}

	return TRUE;
}

BOOL COXBackgroundPainterOrganizer::SetPaintType(const CWnd* pWnd,
												 PaintType paintType, 
												 BOOL bRedraw/*=TRUE*/) 
{
	if(pWnd==NULL)
	{
		if(IsAttachedFrame())
		{
			m_threadOrigPaintType=paintType;
		}
		POSITION pos=m_arrAttachedWnd.GetStartPosition();
		HWND hAttachedWnd=NULL;
		COXBackgroundPainter* pPainter=NULL;
		while(pos!=NULL)
		{
			m_arrAttachedWnd.GetNextAssoc(pos,hAttachedWnd,pPainter);
			ASSERT(hAttachedWnd!=NULL);
			ASSERT(pPainter!=NULL);
			if(pPainter!=NULL && pPainter->IsHooked())
				pPainter->SetPaintType(paintType,bRedraw);
		}
	}
	else
	{
		COXBackgroundPainter* pPainter=NULL;
		CWnd* pAttachedWnd=(CWnd*)pWnd;
		if(!m_arrAttachedWnd.Lookup(pAttachedWnd->GetSafeHwnd(),pPainter))
			return FALSE;
		ASSERT(pPainter!=NULL);
		if(pPainter!=NULL && pPainter->IsHooked())
			pPainter->SetPaintType(paintType,bRedraw);
	}

	return TRUE;
}

BOOL COXBackgroundPainterOrganizer::SetBkColor(const CWnd* pWnd,
											   COLORREF clrBk, 
											   BOOL bRedraw/*=TRUE*/) 
{
	if(pWnd==NULL)
	{
		if(IsAttachedFrame())
		{
			m_threadOrigBkColor=clrBk;
		}
		POSITION pos=m_arrAttachedWnd.GetStartPosition();
		HWND hAttachedWnd=NULL;
		COXBackgroundPainter* pPainter=NULL;
		while(pos!=NULL)
		{
			m_arrAttachedWnd.GetNextAssoc(pos,hAttachedWnd,pPainter);
			ASSERT(hAttachedWnd!=NULL);
			ASSERT(pPainter!=NULL);
			if(pPainter!=NULL && pPainter->IsHooked())
				pPainter->SetBkColor(clrBk,bRedraw);
		}
	}
	else
	{
		COXBackgroundPainter* pPainter=NULL;
		CWnd* pAttachedWnd=(CWnd*)pWnd;
		if(!m_arrAttachedWnd.Lookup(pAttachedWnd->GetSafeHwnd(),pPainter))
			return FALSE;
		ASSERT(pPainter!=NULL);
		if(pPainter!=NULL && pPainter->IsHooked())
			pPainter->SetBkColor(clrBk,bRedraw);
	}

	return TRUE;
}

BOOL COXBackgroundPainterOrganizer::SetOriginWnd(const CWnd* pWnd,
												 CWnd* pOriginWnd, 
												 BOOL bRedraw/*=TRUE*/) 
{
	if(pWnd==NULL)
	{
		if(IsAttachedFrame())
		{
			m_threadOriginWnd=pOriginWnd;
		}
		POSITION pos=m_arrAttachedWnd.GetStartPosition();
		HWND hAttachedWnd=NULL;
		COXBackgroundPainter* pPainter=NULL;
		while(pos!=NULL)
		{
			m_arrAttachedWnd.GetNextAssoc(pos,hAttachedWnd,pPainter);
			ASSERT(hAttachedWnd!=NULL);
			ASSERT(pPainter!=NULL);
			if(pPainter!=NULL && pPainter->IsHooked())
				pPainter->SetOriginWnd(pOriginWnd,bRedraw);
		}
	}
	else
	{
		COXBackgroundPainter* pPainter=NULL;
		CWnd* pAttachedWnd=(CWnd*)pWnd;
		if(!m_arrAttachedWnd.Lookup(pAttachedWnd->GetSafeHwnd(),pPainter))
			return FALSE;
		ASSERT(pPainter!=NULL);
		if(pPainter!=NULL && pPainter->IsHooked())
			pPainter->SetOriginWnd(pOriginWnd,bRedraw);
	}

	return TRUE;
}

COXBackgroundPainter* COXBackgroundPainterOrganizer::
GetPainter(const CWnd* pWnd) const
{
	ASSERT(pWnd!=NULL);
	if(pWnd==NULL)
		return NULL;

	COXBackgroundPainter* pPainter=NULL;
	CWnd* pAttachedWnd=(CWnd*)pWnd;
	if(!m_arrAttachedWnd.Lookup(pAttachedWnd->GetSafeHwnd(),pPainter))
		pPainter=NULL;

	return pPainter;
}

BOOL COXBackgroundPainterOrganizer::IsAttached(const CWnd* pWnd) const
{
	ASSERT(pWnd!=NULL);
	if(pWnd==NULL)
		return FALSE;

	COXBackgroundPainter* pPainter=NULL;
	CWnd* pAttachedWnd=(CWnd*)pWnd;
	if(m_arrAttachedWnd.Lookup(pAttachedWnd->GetSafeHwnd(),pPainter))
	{
		ASSERT(pPainter!=NULL);
		if(pPainter!=NULL && pPainter->IsHooked())
			return TRUE;
	}

	return FALSE;
}


BOOL COXBackgroundPainterOrganizer::LoadState(const CWnd* pWnd,
											  LPCTSTR lpszProfileName, 
											  BOOL bApply/*=TRUE*/)
{
	if(pWnd!=NULL)
	{
		if(!IsAttached(pWnd))
			return FALSE;
		
		COXBackgroundPainter* pPainter=GetPainter(pWnd);
		ASSERT(pPainter!=NULL);

		return pPainter->LoadState(lpszProfileName,bApply);
	}
	else
	{
		POSITION pos=m_arrAttachedWnd.GetStartPosition();
		HWND hAttachedWnd=NULL;
		COXBackgroundPainter* pPainter=NULL;
		while(pos!=NULL)
		{
			m_arrAttachedWnd.GetNextAssoc(pos,hAttachedWnd,pPainter);
			ASSERT(hAttachedWnd!=NULL);
			ASSERT(pPainter!=NULL);
			if(pPainter!=NULL && pPainter->IsHooked())
				VERIFY(pPainter->LoadState(lpszProfileName,bApply));
		}
		return TRUE;
	}
}

BOOL COXBackgroundPainterOrganizer::
AttachFrame(CFrameWnd* pFrameWnd, const COXDIB* pDIB, 
			PaintType paintType/*=Tile*/, COLORREF clrBk/*=CLR_DEFAULT*/)
{
	ASSERT(pFrameWnd!=NULL);
	if(pFrameWnd==NULL)
		return FALSE;

	if(IsAttachedFrame())
	{
		TRACE(_T("COXBackgroundPainterOrganizer::AttachFrame: this object already attached to a frame window\n"));
		return FALSE;
	}

	COXBackgroundPainterOrganizer* pOrganizer=NULL;
	DWORD dwFrameThreadID=
		::GetWindowThreadProcessId(pFrameWnd->GetSafeHwnd(),NULL);
	if(m_arrFrameOrganizers.Lookup(dwFrameThreadID,pOrganizer))
	{
		ASSERT(pOrganizer!=NULL);
		TRACE(_T("COXBackgroundPainterOrganizer::AttachFrame: specified frame already attached to a COXBackgroundPainterOrganizer object\n"));
		return FALSE;
	}
	m_arrFrameOrganizers.SetAt(dwFrameThreadID,this);

	m_pFrameWnd=pFrameWnd;
	m_dwFrameThreadID=::GetWindowThreadProcessId(m_pFrameWnd->GetSafeHwnd(),NULL);

	if(pDIB!=NULL)
		m_threadOrigDIB=*pDIB;
	else
		m_threadOrigDIB.Empty();
	m_threadOrigPaintType=paintType;
	m_threadOrigBkColor=clrBk;
	m_threadOriginWnd=pFrameWnd;

	// go through all windows and attach them
	::EnumWindows(&EnumThreadWindows,(LPARAM)this);

	// setup hooks for Computer Based Training
	if(m_pfnOriginalCBTHookProc==NULL)
	{
		m_pfnOriginalCBTHookProc=::SetWindowsHookEx(WH_CBT,
			BackgroundPainterCBTHookProc,NULL,m_dwFrameThreadID);
		m_pfnOldCBTHookProc=m_pfnOriginalCBTHookProc;
	}
	else
	{
		m_pfnOldCBTHookProc=::SetWindowsHookEx(WH_CBT,
			BackgroundPainterCBTHookProc,NULL,m_dwFrameThreadID);
	}

	// setup hooks for GetMessage
	m_pfnOldGetMessageHookProc=::SetWindowsHookEx(WH_GETMESSAGE,
		BackgroundPainterGetMessageHookProc,NULL,m_dwFrameThreadID);
	if(m_pfnOriginalGetMessageHookProc==NULL)
	{
		m_pfnOriginalGetMessageHookProc=::SetWindowsHookEx(WH_GETMESSAGE,
			BackgroundPainterGetMessageHookProc,NULL,m_dwFrameThreadID);
		m_pfnOldGetMessageHookProc=m_pfnOriginalGetMessageHookProc;
	}
	else
	{
		m_pfnOldGetMessageHookProc=::SetWindowsHookEx(WH_GETMESSAGE,
			BackgroundPainterGetMessageHookProc,NULL,m_dwFrameThreadID);
	}

	SetBkColor(NULL,clrBk,FALSE);
	SetOriginWnd(NULL,m_pFrameWnd,FALSE);
	SetPaintType(NULL,paintType,FALSE);
	SetOriginWnd(NULL,m_pFrameWnd,FALSE);
	SetWallpaper(NULL,pDIB,TRUE);


	return TRUE;
}

void COXBackgroundPainterOrganizer::AttachAllWindows(HWND hWndStartFrom)
{
	ASSERT(hWndStartFrom!=NULL);

	HWND hWnd=hWndStartFrom;
	while(hWnd!=NULL)
	{
		CWnd* pWnd=CWnd::FromHandlePermanent(hWnd);
		if(pWnd!=NULL && !IsAttached(pWnd))
		{
			// analyze the window
			if(IsQualified(pWnd))
			{
				if(Attach(pWnd,(COXDIB*)NULL)==NULL)
				{
					TRACE(_T("COXBackgroundPainterOrganizer::AttachAllWindows: failed to attach a window\n"));
				}
			}
		}

		// loop through children
		HWND hWndChild=::GetWindow(hWnd,GW_CHILD);
		if(hWndChild!=NULL)
			AttachAllWindows(hWndChild);

		// loop through windows
		hWnd=::GetWindow(hWnd,GW_HWNDNEXT);
	}
}


LRESULT CALLBACK COXBackgroundPainterOrganizer::
BackgroundPainterCBTHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	if(nCode>=0 && ::IsWindow((HWND)wParam))
	{
		DWORD dwThreadID=::GetWindowThreadProcessId((HWND)wParam,NULL);
		COXBackgroundPainterOrganizer* pOrganizer=NULL;
		if(COXBackgroundPainterOrganizer::m_arrFrameOrganizers.
			Lookup(dwThreadID,pOrganizer))
		{
			ASSERT(pOrganizer!=NULL);
			ASSERT(pOrganizer->IsAttachedFrame());

			if(nCode==HCBT_DESTROYWND)
			{
				// check if the window that is about to be destroyed
				// had been added to Background organizer list
				CWnd* pWnd=CWnd::FromHandlePermanent((HWND)wParam);
				if(pWnd!=NULL && pOrganizer->IsAttached(pWnd))
					pOrganizer->Detach(pWnd);
			}
			else
			{
				// check if new window is created and attach it.
				CWnd* pWnd=CWnd::FromHandlePermanent((HWND)wParam);
				if(pWnd!=NULL && !pOrganizer->IsAttached(pWnd) && 
					pOrganizer->IsQualified(pWnd))
				{
					POSITION pos=NULL;
					COXBackgroundPainter* pPainter=pOrganizer->GetFirstPainter(pos);
					BOOL bResult=FALSE;
					if(pPainter!=NULL)
					{
						bResult=(pOrganizer->
							Attach(pWnd,pPainter->GetWallpaperImage(),
							pPainter->GetPaintType(),pPainter->GetBkColor(),
							pPainter->GetOriginWnd())!=NULL);
					}
					else
					{
						bResult=(pOrganizer->
							Attach(pWnd,&pOrganizer->m_threadOrigDIB,
							pOrganizer->m_threadOrigPaintType,
							pOrganizer->m_threadOrigBkColor,
							pOrganizer->m_threadOriginWnd)!=NULL);
					}
					if(!bResult)
					{
						TRACE(_T("COXBackgroundPainterOrganizer::BackgroundPainterCBTHookProc: failed to attach window\n"));
					}
				}
			}

			return ::CallNextHookEx(pOrganizer->GetSavedCBTHookProc(),
				nCode,wParam,lParam);
		}
	}
	return ::CallNextHookEx(
		COXBackgroundPainterOrganizer::GetOriginalCBTHookProc(),
		nCode,wParam,lParam);
}


LRESULT CALLBACK COXBackgroundPainterOrganizer::
BackgroundPainterGetMessageHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	if(nCode>=0 && ::IsWindow(((MSG*)lParam)->hwnd))
	{
		DWORD dwThreadID=::GetWindowThreadProcessId(((MSG*)lParam)->hwnd,NULL);
		COXBackgroundPainterOrganizer* pOrganizer=NULL;
		if(COXBackgroundPainterOrganizer::
			m_arrFrameOrganizers.Lookup(dwThreadID,pOrganizer))
		{
			ASSERT(pOrganizer!=NULL);
			ASSERT(pOrganizer->IsAttachedFrame());

			// check if new window is created and attach it.
			CWnd* pWnd=CWnd::FromHandlePermanent(((MSG*)lParam)->hwnd);
			if(pWnd!=NULL && !pOrganizer->IsAttached(pWnd) && 
				pOrganizer->IsQualified(pWnd))
			{
				POSITION pos=NULL;
				COXBackgroundPainter* pPainter=pOrganizer->GetFirstPainter(pos);
				ASSERT(pPainter!=NULL);
				BOOL bResult=FALSE;
				if(pPainter!=NULL)
				{
					bResult=(pOrganizer->
						Attach(pWnd,pPainter->GetWallpaperImage(),
						pPainter->GetPaintType(),pPainter->GetBkColor(),
						pPainter->GetOriginWnd())!=NULL);
				}
				else
				{
					bResult=(pOrganizer->Attach(pWnd,&pOrganizer->m_threadOrigDIB,
						pOrganizer->m_threadOrigPaintType,pOrganizer->m_threadOrigBkColor,
						pOrganizer->m_threadOriginWnd)!=NULL);
				}
				if(!bResult)
				{
					TRACE(_T("COXBackgroundPainterOrganizer::BackgroundPainterGetMessageHookProc: failed to attach window\n"));
				}
			}

			return ::CallNextHookEx(pOrganizer->GetSavedGetMessageHookProc(),
				nCode,wParam,lParam);
		}
	}

	return ::CallNextHookEx(
		COXBackgroundPainterOrganizer::GetOriginalGetMessageHookProc(),
		nCode,wParam,lParam);
}


BOOL CALLBACK COXBackgroundPainterOrganizer::EnumThreadWindows(HWND hWnd, 
															   LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	ASSERT(lParam!=NULL);
	ASSERT(::IsWindow(hWnd));
	COXBackgroundPainterOrganizer* pOrganizer=
		(COXBackgroundPainterOrganizer*)lParam;
	ASSERT(pOrganizer->IsAttachedFrame());

	DWORD dwThreadID=::GetWindowThreadProcessId(hWnd,NULL);
	DWORD dwFrameThreadID=::GetWindowThreadProcessId(pOrganizer->
		GetAttachedFrame()->GetSafeHwnd(),NULL);
	if(dwThreadID==dwFrameThreadID)
	{
		pOrganizer->AttachAllWindows(hWnd);
		return FALSE;
	}
	return TRUE;
}


BOOL COXBackgroundPainterOrganizer::DetachFrame(BOOL bRedraw/*=TRUE*/)
{
	if(!IsAttachedFrame())
		return FALSE;

	ASSERT(m_pFrameWnd!=NULL);
	ASSERT(m_dwFrameThreadID!=NULL);
	ASSERT(m_pfnOldCBTHookProc!=NULL);
	ASSERT(m_pfnOriginalCBTHookProc!=NULL);
	ASSERT(m_pfnOldGetMessageHookProc!=NULL);
	ASSERT(m_pfnOriginalGetMessageHookProc!=NULL);

	// unhook CBT
	if(m_pfnOldCBTHookProc!=NULL)
	{
		VERIFY(::UnhookWindowsHookEx(m_pfnOldCBTHookProc));
		m_pfnOldCBTHookProc=NULL;
		m_pfnOriginalCBTHookProc=NULL;
	}

	// unhook GetMessage
	if(m_pfnOldGetMessageHookProc!=NULL)
	{
		VERIFY(::UnhookWindowsHookEx(m_pfnOldGetMessageHookProc));
		m_pfnOldGetMessageHookProc=NULL;
		m_pfnOriginalGetMessageHookProc=NULL;
	}

	m_arrFrameOrganizers.RemoveKey(m_dwFrameThreadID);

	m_pFrameWnd=NULL;
	m_dwFrameThreadID=NULL;

	return Detach(NULL,bRedraw);
}

BOOL COXBackgroundPainterOrganizer::IsQualified(CWnd* pWnd)
{
	ASSERT(IsAttachedFrame());

	// toolbars (including menubar)
	if(pWnd->IsKindOf(RUNTIME_CLASS(CToolBar)))
	{
		CWnd* pParentWnd=pWnd->GetParent();
		if(pParentWnd!=NULL && 
			pParentWnd->IsKindOf(RUNTIME_CLASS(CControlBar)) &&
			((CControlBar*)pParentWnd)->IsDockBar())
		{
			return TRUE;
		}
	}
	
	// docking bars for the toolbars
	if(pWnd->IsKindOf(RUNTIME_CLASS(CControlBar)) && 
		((CControlBar*)pWnd)->IsDockBar())
	{
		return TRUE;
	}

	// MDI interface
	if(GetAttachedFrame()->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
	{
		// MDIClient window 
		if(pWnd->GetSafeHwnd()==
			((CMDIFrameWnd*)GetAttachedFrame())->m_hWndMDIClient)
		{
			return TRUE;
		}

		// MTI interface tab control
		if(pWnd->IsKindOf(RUNTIME_CLASS(CTabCtrl)) && 
			pWnd->GetParent()==GetAttachedFrame())
		{
			return TRUE;
		}
	}

	return FALSE;
}


