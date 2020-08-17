// ==========================================================================
// 					Class Specification : COXAppBar
// ==========================================================================
//
// OXAppBar.h
// Version: 9.3
//
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//
// Class COXAppBar provides functionality of Windows shell application bar  
// It is a template class and use CWnd derived class as a parent.
// COXAppBar has two constructors - one for generic CWnd derived class
// and another one for CDialog derived class. It is easy to use.
// To start use it call Register(TRUE). To Load prevous state call LoadState(),
// that returns TRUE on success, otherwise you can suply your own parameters.
// If you want to dock window on certain edge you can use function SetEdge().
// The two important state are Autohidden and AlwaysOnTop. To  set or remove these
// features call SetAutoHide() or SetAlwaysOnTop() with appropriate value - TRUE or FALSE
// Any edge can be excluded from docking state by function SetBarStyle()
// To see appbar styles use GetBarStyle(). You can also exclude ABE_FLOAT state,
// so your appbar always will be docked to some edge. Additional functionality is
// provided by  OX_APPBARS_DIFFERENT_DIMS. This flag (set on by default) indicate
// the appbar will provide different width and height for every state.
// Addidtional customization: set the width of the appbar in hidden state,
// time the autohidden appbar will wait while inactive until begin slide to hidden state,
// dimensions in any state and others .
// 
// To unregister appbar call Register(FALSE).
// Note: you must provide default constructor for your class will be used by COXAppBar


//Dependences:

//Header files
//#include "OXRegistryValFile.h"
//#include "OXAppBar.h"

//Cpp files:
//"OXRegistryValFile.cpp"

#if !defined(_OXAPPBAR_H__)
#define _OXAPPBAR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"
#include "OXRegistryValFile.h"
#include "OXMainRes.h"
#include "UTB64Bit.h"


///////////////////////////////////////////////////////////////////////////////
//definitions

const int OX_APPBAR_VERSION=0x0011;

//two additional styles
#define ABE_UNKNOWN 0x101
#define ABE_FLOAT 0x102


//default definitions
#define SLIDING_TIME 10		//time to slide window in autohide state
#define MIN_DIMENSION 2		//width of the appbar in hidden state
#define IDT_AUTOHIDE 1		//timer id for autohide state
#define DEFAULT_DIMENSION 28//default dimension of the appbar (width for right-left,
							//height for top-bottom)
#define DIM_TO_FLOAT 10		//how many pixels should be mouse away from docked
							//appbar to switch to float state
#define DIM_TO_DOCK 40		//how many pixels should be mouse near to an edge 
							//to switch from float state to docking state
#define DIM_CAPTION 6		//pixels from top of the window in float state to the cursor
#define TIMER_INTERVAL 100	//milliseconds - how much time will wait inactive autohidden
							//bar to start to move to hide

/////////////////////////////////////////////////////////////////////////////
//the styles can be set 

#define OX_APPBARS_LEFT 0x01 //can dock on the left
#define OX_APPBARS_TOP  0x02 //can dock on the top
#define OX_APPBARS_RIGHT 0x04 //can dock on the right
#define OX_APPBARS_BOTTOM 0x08 //can dock on the bottom
#define OX_APPBARS_ALL_EDGES (OX_APPBARS_LEFT | OX_APPBARS_TOP | OX_APPBARS_RIGHT | OX_APPBARS_BOTTOM)
#define OX_APPBARS_FLOAT 0x10 //can float
#define OX_APPBARS_DIFFERENT_DIMS 0x020 //remember different dimensions 
										//for every edge 
#define  OX_APPBARS_ALLS (OX_APPBARS_ALL_EDGES | OX_APPBARS_FLOAT | OX_APPBARS_DIFFERENT_DIMS)


typedef struct sMouse
{
	CPoint pt;//current mouse position
	BOOL bButtonDown;//TRUE on button down

	sMouse::sMouse():
	pt(0,0), bButtonDown(FALSE){}

}tMouse;

typedef struct sBarData 
{

	BOOL bRegistered;//TRUE when registered
	BOOL bOnTop;//TRUE when the bar has WS_EX_TOPMOST style
	BOOL bAutoHideDesired;//TRUE, if SHOULD BE in autohide state (may be in not
				   //the state due to error to set autohide state, but this
				   //variable means desired state that will be set on the edge)	
	BOOL bHiding;//TRUE while sliding the bar
	BOOL bHidden;//TRUE if hidden in autohide style, FALSE otherwise
	UINT nWidth;//default width of the bar
	UINT nHeight;//default height of the bar
	UINT nLeft;//width of the bar on the left edge
	UINT nTop;//height of the bar on the top edge
	UINT nRight;//width of the bar on the right edge
	UINT nBottom;//height of the bar on the bottom edge
	UINT nEdge;//edge the bar will be docked on or ABE_FLOAT, ABE_UNKNOWN
	UINT nDockEdge;//edge the bar is docked on, no ABE_FLOAT or ABE_UNKNOWN is allowed
	DWORD dwStyle;//styles applicable to the bar

	sBarData():
	bRegistered(FALSE), bOnTop(FALSE),
	bAutoHideDesired(FALSE), bHiding(FALSE), bHidden(FALSE),
	nWidth(DEFAULT_DIMENSION),
		nHeight(DEFAULT_DIMENSION), nLeft(DEFAULT_DIMENSION),
		nTop(DEFAULT_DIMENSION), nRight(DEFAULT_DIMENSION),
		nBottom(DEFAULT_DIMENSION), nEdge(ABE_UNKNOWN), nDockEdge(NULL),
		dwStyle(OX_APPBARS_ALLS)
	{}

}tBarData;


/////////////////////////////////////////////////////////////////////////////
// COXAppBar window

template <class PARENTWND>
class OX_CLASS_DECL COXAppBar : public PARENTWND
{
// public functions
public:

	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object
	COXAppBar();

	// --- In  : nId - resource ID of the dialog
	//			 pParent - pointer to the parent window
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object (for CDialog derived class)
	COXAppBar(UINT nId, CWnd* pParent=NULL);

	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor
	virtual ~COXAppBar();

	// --- In  : 
	// --- Out : 
	// --- Returns : pointer to structure tBarData
	// --- Effect : returns pointer to tBarData
	inline const tBarData* GetBarData() const {return &m_BarData;}

	// --- In  :	nWidth - new default width
	// --- Out : 
	// --- Returns :
	// --- Effect : sets new default width for the appbar
	inline void SetDefaultWidth(UINT nWidth) {m_BarData.nWidth=nWidth;}

	// --- In  :	nHeight - new default height
	// --- Out : 
	// --- Returns :
	// --- Effect : sets new default height for the appbar
	inline void SetDefaultHeight(UINT nHeight){m_BarData.nHeight=nHeight;}

	// --- In  : 
	// --- Out : 
	// --- Returns : a count of timer ticks the window slides on 1 pixel
	// --- Effect : call this function to retrieve the sliding time
	inline UINT GetSlidingTime() const {return m_nSlidingTime;} 

	// --- In  : nSlidingTime - a count of timer ticks the window slides on 1 pixel
	// --- Out : 
	// --- Returns :
	// --- Effect : call this function to change the sliding time
	inline void SetSlidingTime(UINT nSlidingTime) {m_nSlidingTime=nSlidingTime;}

	// --- In  : 
	// --- Out : 
	// --- Returns : width of the appbar while hidden
	// --- Effect : call this function to get width of the appbar in hidden state
	inline UINT GetHiddenWidth() const { return m_nHiddenWidth;}

	// --- In  : nHiddenWidth - new width of the appbar in hidden state
	// --- Out : 
	// --- Returns :
	// --- Effect : call this function to set new width of the appbar in hidden state
	inline void SetHiddenWidth(UINT nHiddenWidth) {m_nHiddenWidth=nHiddenWidth;}

	// --- In  : nToFloat - difference (pixels) between cursor and edge
	//						of the appbar when appbar will switch to the float state
	// --- Out : 
	// --- Returns :
	// --- Effect : call this function to set new value to switch to float state
	inline void SetPointToFloat(UINT nToFloat) {m_nToFloat=nToFloat;}

	// --- In  : 
	// --- Out : 
	// --- Returns : current difference (pixels) between cursor and edge
	//						of the appbar when appbar will switch to the float state
	// --- Effect : call this function to get current value to switch to float state
	inline UINT GetPointToFloat() const {return m_nToFloat;}

	// --- In  : nToDock - difference (pixels) between cursor and screen edge
	//						when appbar will switch to the docking state
	// --- Out : 
	// --- Returns :
	// --- Effect : call this function to set new value to switch to docking state
	inline void SetPointToDock(UINT nToDock) {m_nToDock=nToDock;}

	// --- In  : 
	// --- Out : 
	// --- Returns : difference (pixels) between cursor and screen edge
	//						when appbar will switch to the docking state
	// --- Effect : call this function to get the value the appbar 
	//				will switch to docking state
	inline UINT GetPointToDock() const {return m_nToDock;}

	// --- In  : nTimerInterval - time the appbar will remain unhidden 
	//								while inactive untill start to sliding
	// --- Out : 
	// --- Returns :
	// --- Effect : sets new timer interval value
	inline void SetTimerInterval(UINT nTimerInterval) {m_nTimerInterval=nTimerInterval;}

	// --- In  : 
	// --- Out : 
	// --- Returns : - time the appbar will remain unhidden 
	//					while inactive untill start to sliding
	// --- Effect : call this function to retrieve the value of timer interval
	inline UINT GetTimerInterval() const {return m_nTimerInterval;}

	// --- In  : dwStyle - new styles of the appbar
	//				The valid styles are:
	//					OX_APPBARS_LEFT - the appbar can dock 
	//						on the left edge of the screen
	//					OX_APPBARS_TOP - the appbar can dock
	//						on the top edge of the screen
	//					OX_APPBARS_RIGHT - the appbar can dock 
	//						on the right edge of the screen
	//					OX_APPBARS_BOTTOM - the appbar can dock
	//						on the bottom
	//					OX_APPBARS_ALL_EDGES - the appbar can dock
	//						on all edges of the screen 
	//					OX_APPBARS_FLOAT - the appbar can float
	//					OX_APPBARS_DIFFERENT_DIMS - the appbar has
	//						different dimensions for every edge 
	//					OX_APPBARS_ALLS - all posible styles
	// --- Out : 
	// --- Returns :
	// --- Effect : sets new styles for the appbar
	inline void SetBarStyle(DWORD dwStyle) 
	{
		m_BarData.dwStyle=(dwStyle & OX_APPBARS_ALLS);
	}

	// --- In  : 
	// --- Out : 
	// --- Returns : the appbar styles (See description function SetBarStyle())
	// --- Effect : returns the appbar styles
	inline DWORD GetBarStyle () const {return m_BarData.dwStyle;}

	// --- In  : 
	// --- Out : 
	// --- Returns : TRUE if autobar is in autohidden state. The appbar
	//				  may actually be hidden or unhidden at this moment (if is in
	//				  autohidden state).
	//				  To check actual state see variable 
	//				  bHidden in bar data (GetBarData()). There is a difference
	//				  between autohide state and desired autohide state.
	//				  Once you call SetAutoHide(TRUE), the desired state
	//				  (bAutoHideDesired in bar data - call GetBarData()) will
	//				  remain TRUE untill next call SetAutoHide(FALSE). This is
	//				  because the system can has only one autohidden bar per edge.
	//				  So, if there was not posibility to set autohidden state on
	//				  this edge, this function will return FALSE, while bAutoHideDesired 
	//				  will be TRUE. Next time the edge has been changed bAutoHideDesired
	//				  will forced to set autohide state on the new edge.
	// --- Effect : returns TRUE if the appbar is in autohidden state on this edge
	BOOL IsAutoHidden();

	// --- In  : 
	// --- Out : 
	// --- Returns : TRUE, if the appbar is floating, FALSE othewise.
	// --- Effect : call this function to check floating state of the appabar
	inline BOOL IsFloating() const {return (BOOL) (m_BarData.nEdge==ABE_FLOAT);}

	// --- In  : 
	// --- Out : 
	// --- Returns : TRUE, if the appbar is in topmost state
	// --- Effect : call this function to define if the appbar is
	//				in topmost state
	inline BOOL IsAlwaysOnTop() const {return m_BarData.bOnTop;}

	// --- In  : 
	// --- Out : 
	// --- Returns : TRUE, if appbar is registered, FALSE otherwise
	// --- Effect : call this function to define if the appbar is registered
    inline BOOL IsRegistered () const {return m_BarData.bRegistered;}

	// --- In  : 
	// --- Out : 
	// --- Returns : the edge appbar is docking on or ABE_FLOAT, if floating
	// --- Effect : call this function to get the edge 
	//				appbar is docking on, if any
	inline UINT GetEdge() const { return m_BarData.nEdge;}

	// --- In  : bOnTop - TRUE to set as topmost window, FALSE to remove
	// --- Out : 
	// --- Returns : 
	// --- Effect : call this function to change topmost style of the appbar
	void SetAlwaysOnTop(BOOL bOnTop);

	// --- In  : nEdge - the edge appbar should be docked on or float
	// --- Out : 
	// --- Returns :
	// --- Effect : call this function to change the edge of the appbar
	BOOL SetEdge(UINT nEdge);

	// --- In  : bHide - TRUE to set autohide style, FALSE to remove
	// --- Out : 
	// --- Returns : TRUE on success, FALSE otherwise.
	// --- Effect : Sets or removes autohidden state. The variable
	//				bHide will determine the autohidden state on all edges
	//				(See description of IsAutoHidden() function)
	BOOL SetAutoHide(BOOL bHide);

	// --- In  : bRegister - TRUE to register the appbar, FALSE to remove  
	// --- Out : 
	// --- Returns : TRUE on success, FALSE if the system 
	//				 could not register or the appbar has been registered
	//				 so far
	// --- Effect : Call this function to register or unregister appbar.
	//				The best way is before call this function
	//				call function IsRegistered() to check out the status 
	//				of the appbar
	BOOL Register(BOOL bRegister=TRUE);


	void SetDockDimension(UINT nEdge, UINT nWidth)
	{
		switch (nEdge)
		{
		case ABE_TOP:
			m_BarData.nTop=nWidth;
			break;
		case ABE_LEFT:
			m_BarData.nLeft=nWidth;
			break;
		case ABE_BOTTOM:
			m_BarData.nBottom=nWidth;
			break;
		case ABE_RIGHT:
			m_BarData.nRight=nWidth;
			break;
		default:
			ASSERT(FALSE);
		}
	}
//protected functions
protected:

	// --- In  : nEdge
	// --- Out : 
	// --- Returns :
	// --- Effect : Do nothing. Override this function 
	//				if you need to handle changes of the edges
	//				the appbar switching to
	virtual void OnSetEdge(UINT nEdge) { UNREFERENCED_PARAMETER(nEdge);}

	// --- In  : 
	// --- Out : 
	// --- Returns : TRUE on succes, FALSE otherwise
	// --- Effect : Call this function to restore last 
	//				saved state of the appbar
	virtual BOOL LoadState();

	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Call this function to save appbar to the registry
	//				before call Register(FALSE);
	virtual void SaveState();

	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : override this function if you want provide 
	//				popup menu on mouse right-click
	virtual void DoPopupMenu(){/* do nothing - override to provide your own menu*/}

	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	
	void Float(APPBARDATA* pData);
	void UnFloat(APPBARDATA* pData);
	void AppBarCallback(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void PosChanged();
	void SetPosition(APPBARDATA* pBarData,	BOOL bMove);
	void QueryPosition(RECT* pRect);
	void UnHide();
	void Hide();
	UINT GetProposedEdge();
	void GetEndRect(CRect& rect);
	void GetProposedRect(CRect& rect);
	BOOL CanContinue(CRect& rctCurrent, CRect& rctEnd);
	void SlideWindow();
	
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnDestroy();
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnMove(int x, int y);
	LRESULT OnNcHitTest(CPoint point);
	void OnRButtonDown(UINT nFlags, CPoint point);
	void OnTimer(UINT nIDEvent);
	void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	void OnExitSizeMoveLoop();

//protected members
protected:
	CRect m_rctFloat; //coordinates of the window in the float state
	int m_nOffset;//switches between -1 and 1 indicating in what side slide
	tMouse m_stMouse;//position and state of the cursor
	tBarData m_BarData;//data related to the bar
	BOOL m_bSizeable;//to remember WS_THICKFRAME style while the bar is hidden 
	UINT m_nSlidingTime;//defines speed to slide
	UINT m_nHiddenWidth;//defines width of the window in the hidden state
	UINT m_nTimerInterval;//time to wait to start sliding bar while the bar is inactive
	UINT m_nToDock;//offset cursor from an edge that will be treated as switch to docking state
	UINT m_nToFloat;//offset cursor from the docking frame to switch to float
	BOOL m_bFloating;//TRUE while floating
};

/*
=========================================================================
======================IMPLEMENTATION=====================================
=========================================================================
*/

/////////////////////////////////////////////////////////////////////////////
// COXAppBar

////////////////////////////////////////////
//constructor for generic CWnd derived class

template <class PARENTWND>
COXAppBar<PARENTWND>::COXAppBar()
{
	ASSERT(PARENTWND::IsKindOf(RUNTIME_CLASS(CWnd)));
	m_nOffset=0;
	m_bSizeable=FALSE;
	m_nSlidingTime=SLIDING_TIME;
	m_nHiddenWidth=MIN_DIMENSION;
	m_nTimerInterval=TIMER_INTERVAL;
	m_nToDock=DIM_TO_DOCK;
	m_nToFloat=DIM_TO_FLOAT;
	m_bFloating=FALSE;
}

////////////////////////////////////////////
//constructor for CDialog derived classes

template <class PARENTWND>
COXAppBar<PARENTWND>::COXAppBar(UINT nId, CWnd* pParent):
PARENTWND(nId, pParent)
{
	m_nOffset=0;
	m_bSizeable=FALSE;
	m_nSlidingTime=TIMER_INTERVAL;
	m_nHiddenWidth=MIN_DIMENSION;
	m_nTimerInterval=TIMER_INTERVAL;
	m_nToDock=DIM_TO_DOCK;
	m_nToFloat=DIM_TO_FLOAT;
	m_bFloating=FALSE;
}



///////////////////////////////////////
//destructor

template <class PARENTWND>
COXAppBar<PARENTWND>::~COXAppBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// COXAppBar message handlers

/////////////////////////////////////
//cannot define message handler in 
//template class - working with 
//WindowProc()

template <class PARENTWND>
LRESULT COXAppBar<PARENTWND>::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	switch (message)
	{
	case WM_ACTIVATE:
		{
			OnActivate(LOWORD(wParam),
				(CWnd*) CWnd::FromHandle((HWND) lParam),
				(BOOL) HIWORD(wParam));
			return NULL;
		}
	case WM_CREATE:
		{
			return OnCreate((LPCREATESTRUCT) lParam);
		}
	case WM_DESTROY:
		{
			OnDestroy();
			return NULL;
		}
	case WM_LBUTTONDOWN:
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));
			OnLButtonDown((UINT) wParam, point);
			return NULL;
		}
	case WM_LBUTTONUP:
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));
			OnLButtonUp((UINT) wParam, point);
			return NULL;
		}
	case WM_MOUSEMOVE:
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));
			OnMouseMove((UINT) wParam, point);
			return NULL;
		}
	case WM_MOVE:
		{
			OnMove(LOWORD(lParam), HIWORD(lParam));
			return NULL;
		}
	case WM_NCHITTEST:
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));
			return OnNcHitTest(point);
		}
	case WM_RBUTTONDOWN:
		{
			CPoint point(LOWORD(lParam), HIWORD(lParam));
			OnRButtonDown((UINT) wParam, point);
			return NULL;
		}
	case WM_EXITSIZEMOVE:
		{
			OnExitSizeMoveLoop();
			return NULL;
		}
		break;
	case WM_TIMER:
		{
			OnTimer((UINT) wParam);
			return NULL;
		}
	case WM_WINDOWPOSCHANGED:
		{
			OnWindowPosChanged((WINDOWPOS*) lParam);
			return NULL;
		}
	
	default:
		{
			return PARENTWND::WindowProc(message, wParam, lParam);
		}
	}
}


//////////////////////////////////////////////////
//handlers for WindowProc()

//////////////////////////////////////////////////
//OnActivate(): if the bar is autohidden, Hide()
//or UnHide() it depends on the message

template <class PARENTWND>
void COXAppBar<PARENTWND>::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	PARENTWND::OnActivate(nState, pWndOther, bMinimized);
	
	ASSERT(PARENTWND::m_hWnd);
	
	APPBARDATA data;
	ZeroMemory((void*) &data, sizeof(data));
	data.cbSize=sizeof(APPBARDATA);
	data.hWnd=PARENTWND::m_hWnd;

	SHAppBarMessage(ABM_ACTIVATE, &data);

	if (!IsAutoHidden())
		return;

	switch (nState)
	{
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
			if (m_BarData.bHidden)
				UnHide();
			break;

		case WA_INACTIVE:
			if (!m_BarData.bHidden)
				Hide();
			break;
	}
}

//////////////////////////////////////////
//OnCreate(): load default bar rect for
//floating state and SetTimer() for autohidden state

template <class PARENTWND>
int COXAppBar<PARENTWND>::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (PARENTWND::OnCreate(lpCreateStruct) == -1)
		return -1;

	PARENTWND::GetWindowRect(&m_rctFloat);

	PARENTWND::SetTimer(IDT_AUTOHIDE,m_nTimerInterval,NULL);
	return 0;
}

/////////////////////////////////////////
//ONDestroy(): Unregister and KillTimer()

template <class PARENTWND>
void COXAppBar<PARENTWND>::OnDestroy() 
{
	Register(FALSE);
	PARENTWND::KillTimer(IDT_AUTOHIDE);
	PARENTWND::OnDestroy();
}

//////////////////////////////////////////
//OnLButtonDown(): start capture if registered

template <class PARENTWND>
void COXAppBar<PARENTWND>::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (IsRegistered())
	{
		m_stMouse.bButtonDown=TRUE;
		::GetCursorPos(&m_stMouse.pt);
		SetCapture();	
	}
	PARENTWND::OnLButtonDown(nFlags, point);
}

///////////////////////////////////////////////////
//OnLButtonUp(): ReleaseCapture() if was registared

template <class PARENTWND>
void COXAppBar<PARENTWND>::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (IsRegistered())
	{
		m_stMouse.bButtonDown=FALSE;
		::GetCursorPos(&m_stMouse.pt);
		ReleaseCapture();
	}
	PARENTWND::OnLButtonUp(nFlags, point);
}

////////////////////////////////////////////////////
//OnMouseMove() if registered:
//move window if is in float state,
//UnHide() if in hidden state.
//Check for new appropriate edge.

template <class PARENTWND>
void COXAppBar<PARENTWND>::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!IsRegistered())
	{
		PARENTWND::OnMouseMove(nFlags, point);
		return;
	}

	if (IsAutoHidden() && m_BarData.bHidden)
	{
		UnHide();
		return;
	}
 	if (!m_stMouse.bButtonDown || m_BarData.nEdge==ABE_UNKNOWN)
		return;

	//get current position 
	CPoint pt;
	::GetCursorPos(&pt);

	if (m_BarData.nEdge==ABE_FLOAT)
	{
		APPBARDATA data;
		ZeroMemory((void*) &data, sizeof(data));
		data.cbSize=sizeof(data);
		data.hWnd=PARENTWND::m_hWnd;
		data.uEdge=m_BarData.nEdge;
		CRect rect;
		PARENTWND::GetWindowRect(&rect);
		CSize size;
		size=pt-m_stMouse.pt;
		rect.OffsetRect(size);
		m_stMouse.pt=pt;
		data.rc=rect;
		SetPosition(&data,TRUE);
		return;
	}

	m_stMouse.pt=pt;

	UINT nNewEdge=GetProposedEdge();

	if (nNewEdge!=m_BarData.nEdge)
		SetEdge(nNewEdge);

	PARENTWND::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////
//OnMove(): update cursor position

template <class PARENTWND>
void COXAppBar<PARENTWND>::OnMove(int x, int y) 
{

	CWnd::OnMove(x, y);

	CPoint pt;
	::GetCursorPos(&pt);

	m_stMouse.pt=pt;

}

//////////////////////////////////////////////
//OnNcHitTest(): handling to allow move window
//by mouse not only with caption, but all client area

template <class PARENTWND>
LRESULT COXAppBar<PARENTWND>::OnNcHitTest(CPoint point) 
{
	UINT  nHitTest;

	nHitTest = PtrToUint(CWnd::OnNcHitTest(point));
	
	if (!IsRegistered())
		return nHitTest;

	if (m_BarData.nEdge==ABE_FLOAT)
	{
		if (nHitTest==HTCLIENT || nHitTest==HTCAPTION)
			return HTCLIENT;
		else
			return nHitTest;
	}
	if ((m_BarData.nEdge == ABE_TOP) && (nHitTest == HTBOTTOM))
		return HTBOTTOM;
	if ((m_BarData.nEdge == ABE_BOTTOM) && (nHitTest == HTTOP))
		return HTTOP;
	if ((m_BarData.nEdge == ABE_LEFT) && (nHitTest == HTRIGHT))
		return HTRIGHT;
	if ((m_BarData.nEdge == ABE_RIGHT) && (nHitTest == HTLEFT))
		return HTLEFT;

	return HTCLIENT;							
}

/////////////////////////////////////////////////
//OnRButtonDown() Popup menu if cursor is in client area

template <class PARENTWND>
void COXAppBar<PARENTWND>::OnRButtonDown(UINT nFlags, CPoint point) 
{

	CRect rct;
	PARENTWND::GetClientRect(&rct);

	if (rct.PtInRect(point))
	{
		DoPopupMenu();
		PARENTWND::OnRButtonDown(nFlags, point);
	}
}

///////////////////////////////////////////////
//OnExitSizeMoveLoop(): Update current position

template <class PARENTWND>
void COXAppBar<PARENTWND>::OnExitSizeMoveLoop()
{
	if (!m_BarData.bHiding && !m_BarData.bHidden)
	{
		CRect rect;
		PARENTWND::GetWindowRect(&rect);
		
		switch (m_BarData.nEdge)
		{
			case ABE_LEFT:
				m_BarData.nWidth=rect.Width();
				if (m_BarData.dwStyle & OX_APPBARS_DIFFERENT_DIMS)
					m_BarData.nLeft=rect.Width();
				break;
			case ABE_TOP:
				m_BarData.nHeight=rect.Height();
				if (m_BarData.dwStyle & OX_APPBARS_DIFFERENT_DIMS)
					m_BarData.nTop=rect.Height();
				break;
			case ABE_RIGHT:
				m_BarData.nWidth=rect.Width();
				if (m_BarData.dwStyle & OX_APPBARS_DIFFERENT_DIMS)
					m_BarData.nRight=rect.Width();
				break;
			case ABE_BOTTOM:
				m_BarData.nHeight=rect.Height();
				if (m_BarData.dwStyle & OX_APPBARS_DIFFERENT_DIMS)
					m_BarData.nBottom=rect.Height();
				break;
		}
		if (!IsAutoHidden())
		{
			APPBARDATA data;
			ZeroMemory((void*) &data,sizeof(data));
			data.cbSize=sizeof(data);
			data.hWnd=PARENTWND::m_hWnd;
			data.uEdge=m_BarData.nEdge;
			data.rc=rect;
			SetPosition(&data, TRUE);
		}
	}

}


/////////////////////////////////////////////////
//OnTimer(): In autohidden state hide window, if 
//is inactive

template <class PARENTWND>
void COXAppBar<PARENTWND>::OnTimer(UINT nIDEvent) 
{
	if (IsAutoHidden() && !m_BarData.bHidden && !m_BarData.bHiding)
	{
		if (::GetActiveWindow()!=PARENTWND::m_hWnd)
		{
			CPoint pt;
			::GetCursorPos(&pt);
			CRect rct;
			PARENTWND::GetWindowRect(&rct);
			rct.InflateRect(MIN_DIMENSION,MIN_DIMENSION);
			if (!rct.PtInRect(pt))
				Hide();
		}
	}
	PARENTWND::OnTimer(nIDEvent);
}

///////////////////////////////////////////
//OnWindowPosChanged() - update current position

template <class PARENTWND>
void COXAppBar<PARENTWND>::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	PARENTWND::OnWindowPosChanged(lpwndpos);
	
	if (!IsRegistered() || m_BarData.bHiding)
		return;
	
	APPBARDATA data;

	CRect rect;
	PARENTWND::GetWindowRect(&rect);

	ZeroMemory(&data, sizeof(data));
	data.cbSize = sizeof(APPBARDATA);
	data.rc=rect;
	data.uEdge = m_BarData.nEdge;
	data.hWnd=PARENTWND::m_hWnd;

	SHAppBarMessage (ABM_WINDOWPOSCHANGED, &data);

	switch(m_BarData.nEdge)
	{
	case ABE_FLOAT:
		{
			m_rctFloat=rect;
			UINT nNewEdge=GetProposedEdge();
			if (nNewEdge!=m_BarData.nEdge)
				SetEdge(nNewEdge);
		}
		break;
	case ABE_LEFT:
		if (!m_BarData.bHidden)
		{
		m_BarData.nWidth=rect.Width();
		if (m_BarData.dwStyle & OX_APPBARS_DIFFERENT_DIMS)
			m_BarData.nLeft=m_BarData.nWidth;
		}
		break;
	case ABE_TOP:
		if (!m_BarData.bHidden)
		{
			m_BarData.nHeight=rect.Height();
		if (m_BarData.dwStyle & OX_APPBARS_DIFFERENT_DIMS)
			m_BarData.nTop=m_BarData.nHeight;
		}
		break;
	case ABE_RIGHT:
		if (!m_BarData.bHidden)
		{
			m_BarData.nWidth=rect.Width();
		if (m_BarData.dwStyle & OX_APPBARS_DIFFERENT_DIMS)
			m_BarData.nRight=m_BarData.nWidth;
		}
		break;
	case ABE_BOTTOM:
		if (!m_BarData.bHidden)
		{
			m_BarData.nHeight=rect.Height();
		if (m_BarData.dwStyle & OX_APPBARS_DIFFERENT_DIMS)
			m_BarData.nBottom=m_BarData.nHeight;
		}
		break;
	}

	
}

////////////////////////////////////////
//IsAutoHidden(): defines is bar in autohidden state
//There is a difference between autohidden state and
//variable m_BarData.bAutoHideDesired - the variable
//is always TRUE if last call was SetAutoHide(TRUE)
//and FALSE if last call was SetAutoHide(FALSE) regardless
//of actual autohidden state that returns IsAutoHidden().
//Every time the docked edge is changed the program will try
//to make the bar autohidden if the variable is TRUE 

template <class PARENTWND>
BOOL COXAppBar<PARENTWND>::IsAutoHidden() 
{
	ASSERT(::IsWindow(PARENTWND::m_hWnd));

	if (!IsRegistered())
		return FALSE;

	APPBARDATA data;

	ZeroMemory(&data, sizeof(data));
	data.cbSize = sizeof(APPBARDATA);
	data.uEdge = m_BarData.nEdge;
	data.hWnd=PARENTWND::m_hWnd;

	HWND hWnd=(HWND) SHAppBarMessage(ABM_GETAUTOHIDEBAR, &data);
	
	return (BOOL) (hWnd==data.hWnd); 
}


/////////////////////////////////////////////
//QueryPosition(): query position for appbar

template <class PARENTWND>
void COXAppBar<PARENTWND>::QueryPosition(RECT* pRect)
{
	ASSERT(::IsWindow(PARENTWND::m_hWnd));

	APPBARDATA data;

	ZeroMemory(&data, sizeof(data));
	data.cbSize = sizeof(APPBARDATA);
	data.rc=*pRect;
	data.uEdge = m_BarData.nEdge;
	data.hWnd=PARENTWND::m_hWnd;

	SHAppBarMessage(ABM_QUERYPOS, &data);

	switch (data.uEdge)
	{
		case ABE_LEFT:
			data.rc.right = data.rc.left+pRect->right-pRect->left;
			break;

		case ABE_RIGHT:
			data.rc.left = data.rc.right-pRect->right+pRect->left;
			break;

		case ABE_TOP:
			data.rc.bottom = data.rc.top + pRect->bottom-pRect->top;
			break;

		case ABE_BOTTOM:
			data.rc.top = data.rc.bottom - pRect->bottom+pRect->top;
			break;
	}
	*pRect = data.rc;	
}

/////////////////////////////////////////////////////
//SetPosition(): sets the appbar in specified position 
//regarding with result of query position

template <class PARENTWND>
void COXAppBar<PARENTWND>::SetPosition(APPBARDATA* pBarData,	BOOL bMove)
{

	ASSERT(::IsWindow(PARENTWND::m_hWnd));

	APPBARDATA data;

	ZeroMemory(&data, sizeof(data));
	data.cbSize = sizeof(APPBARDATA);
	data.rc=pBarData->rc;
	data.uEdge = pBarData->uEdge;
	data.hWnd=PARENTWND::m_hWnd;
	
	QueryPosition(&data.rc);

	if (data.uEdge !=ABE_FLOAT && data.uEdge!=ABE_UNKNOWN)
		SHAppBarMessage(ABM_SETPOS, &data);
	if (bMove)
	{
    	
		PARENTWND::MoveWindow(pBarData->rc.left, pBarData->rc.top, 
			   	   pBarData->rc.right - pBarData->rc.left,
			   	   pBarData->rc.bottom - pBarData->rc.top, TRUE);
	}


}

//////////////////////////////////////////////////
//PosChanged(): handler for notification message -
//windows tell us move the appbar

template <class PARENTWND>
void COXAppBar<PARENTWND>::PosChanged()
{
	SetEdge(m_BarData.nEdge);
}

//////////////////////////////////////////////////
//AppBarCallback(): handler for notification messages

template <class PARENTWND>
void COXAppBar<PARENTWND>::AppBarCallback (UINT uMsg, WPARAM wParam, LPARAM lParam)
{
 	static HWND hwndZOrder = NULL;
    
	APPBARDATA data;
	ZeroMemory((void*) &data,sizeof(data));
	data.cbSize=sizeof(data);
	data.hWnd=PARENTWND::m_hWnd;

    
    switch (wParam) 
    {
        case ABN_STATECHANGE:
	        break;

	    case ABN_FULLSCREENAPP:
	        if (lParam) 
	        {
                //Move the appbar to the bottom of the z-order.  

				hwndZOrder = GetWindow (m_data.hWnd, GW_HWNDPREV);
				SetWindowPos(data.hWnd, HWND_BOTTOM, 0, 0, 0, 0, 
	            	 SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);            
	        } 
	        else 
	        {
                //Restore the Z-order			   
                SetWindowPos(m_data.hWnd, IsAlwaysOnTop() ? HWND_TOPMOST : hwndZOrder,
	                         0, 0, 0, 0, 
	                         SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
				
				hwndZOrder = NULL;
	        }
			break;
        
    	case ABN_POSCHANGED:
        	PosChanged ();
        	break;
    }
}

////////////////////////////////////////////////////////////
//Register(): this function registers/unregisters the appbar. 

template <class PARENTWND>
BOOL COXAppBar<PARENTWND>::Register(BOOL bRegister)
{

	ASSERT(PARENTWND::m_hWnd);
	
	APPBARDATA data;
	ZeroMemory((void*) &data, sizeof(data));
	data.cbSize=sizeof(APPBARDATA);
	data.hWnd=PARENTWND::m_hWnd;

	if (bRegister)
	{
		data.uCallbackMessage = OX_APPBAR;
		m_BarData.bRegistered = PtrToInt(SHAppBarMessage (ABM_NEW, &data));
		if (m_BarData.bRegistered)
		{
			m_bFloating=FALSE;
			if (m_BarData.nEdge==ABE_UNKNOWN )
				SetEdge(ABE_FLOAT);
			else
			{
				int nEdge=m_BarData.nEdge;
				m_BarData.nEdge=ABE_UNKNOWN;
				SetEdge(nEdge);
			}
		}
		return m_BarData.bRegistered;
	}
	else
	{
		SaveState();
		m_BarData.bRegistered = !SHAppBarMessage(ABM_REMOVE, &data);
		m_bFloating=FALSE;
		return !m_BarData.bRegistered;
	}
}

///////////////////////////////////////////////////
//SetAutoHide(): sets or remove autohide mode

template <class PARENTWND>
BOOL COXAppBar<PARENTWND>::SetAutoHide(BOOL bHide)
{

	ASSERT(PARENTWND::m_hWnd);
	
	CRect rect;
	APPBARDATA data;
	ZeroMemory((void*) &data, sizeof(data));
	data.cbSize=sizeof(APPBARDATA);
	data.hWnd=PARENTWND::m_hWnd;
	data.uEdge = m_BarData.nEdge;
	
	m_BarData.bAutoHideDesired=bHide;

	HWND hwndAutoHide = (HWND) SHAppBarMessage(ABM_GETAUTOHIDEBAR, &data);
	
	if (bHide)
    {
		if (hwndAutoHide != NULL)
		{
			TRACE0("The system has autohidden appbar on this edge so far\r\n");
			return FALSE;
		}

		data.lParam = TRUE;			
		if (!SHAppBarMessage(ABM_SETAUTOHIDEBAR, &data))
		{
			TRACE0("Error to set autohidden bar\r\n");
			return FALSE;
		}
		if (m_BarData.nEdge!=ABE_FLOAT)
		{
			PARENTWND::GetWindowRect(&rect);
			GetEndRect(rect);
			data.rc=rect;
			SetPosition(&data, FALSE);
		}
	}
	else
    {
		if (hwndAutoHide != data.hWnd)
		{
			TRACE0 ("The window is not hidden\r\n");
			return FALSE;
		}

		data.lParam = FALSE;			

		if (!SHAppBarMessage(ABM_SETAUTOHIDEBAR, &data))
		{
			TRACE0 ("Error to unhide appbar\r\n");
			return FALSE;
		}
		PARENTWND::GetWindowRect(&rect);
		switch (m_BarData.nEdge)
		{
		case ABE_LEFT:
			rect.right=rect.left+m_BarData.nWidth;
			break;
		case ABE_TOP:
			rect.bottom=rect.top+m_BarData.nHeight;
			break;
		case ABE_RIGHT:
			rect.left=rect.right-m_BarData.nWidth;
			break;
		case ABE_BOTTOM:
			rect.top=rect.bottom-m_BarData.nHeight;
			break;
		default:
			return TRUE;
		}
		data.rc=rect;
		SetPosition(&data,TRUE);
    }
	return TRUE;
}

///////////////////////////////////////////////
//SetEdge(): call this function to set desired edge

template <class PARENTWND>
BOOL COXAppBar<PARENTWND>::SetEdge(UINT nEdge)
{
	//only for changing edge
	if (nEdge==m_BarData.nEdge || !IsRegistered())
		return FALSE;

	switch (nEdge)
	{
	case ABE_UNKNOWN:
		ASSERT(FALSE);
		return FALSE;
	case ABE_FLOAT:
		if (!(m_BarData.dwStyle & OX_APPBARS_FLOAT))
			return FALSE;
		break;
	case ABE_TOP:
		if (!(m_BarData.dwStyle & OX_APPBARS_TOP))
			return FALSE;
		m_BarData.nDockEdge=ABE_TOP;
		break;
	case ABE_LEFT:
		if (!(m_BarData.dwStyle & OX_APPBARS_LEFT))
			return FALSE;
		m_BarData.nDockEdge=ABE_LEFT;
		break;
	case ABE_RIGHT:
		if (!(m_BarData.dwStyle & OX_APPBARS_RIGHT))
			return FALSE;
		m_BarData.nDockEdge=ABE_RIGHT;
		break;
	case ABE_BOTTOM:
		if (!(m_BarData.dwStyle & OX_APPBARS_BOTTOM))
			return FALSE;
		m_BarData.nDockEdge=ABE_BOTTOM;
		break;
	}

	if (IsAutoHidden())
	{
		SetAutoHide(FALSE);
		m_BarData.bAutoHideDesired=TRUE;
	}
	
	m_BarData.nEdge=nEdge;

	CRect rect;

	ASSERT(PARENTWND::m_hWnd);
	PARENTWND::GetWindowRect(&rect);

	APPBARDATA data;
	ZeroMemory((void*) &data, sizeof(data));
	data.cbSize=sizeof(APPBARDATA);
	data.hWnd=PARENTWND::m_hWnd;
	data.uEdge=m_BarData.nEdge;
	

	if ( nEdge==ABE_FLOAT)
		Float(&data);
	else 
		UnFloat(&data);


	GetProposedRect(rect);
	data.rc=rect;

	if (nEdge!=ABE_FLOAT)
	{
		SetPosition(&data, TRUE);
		if (m_BarData.bAutoHideDesired)
			SetAutoHide(TRUE);
	}
	else
		PARENTWND::MoveWindow(rect.left, rect.top, 
	   	   rect.right - rect.left,
	   	   rect.bottom - rect.top, TRUE);

	OnSetEdge(m_BarData.nEdge);
	return TRUE;
}

//////////////////////////////////////////////////////
//SetAlwaysOnTop(): sets or remove always_on_top style

template <class PARENTWND>
void COXAppBar<PARENTWND>::SetAlwaysOnTop(BOOL bOnTop)
{

	::SetWindowPos(PARENTWND::m_hWnd, (bOnTop) ? HWND_TOPMOST : HWND_NOTOPMOST,
	             0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	m_BarData.bOnTop = bOnTop;
}

///////////////////////////////////////////////////
//Hide(): hides the window in autohidden state

template <class PARENTWND>
void COXAppBar<PARENTWND>::Hide()
{
	if (!IsAutoHidden())
		return;

	CRect rect;
	PARENTWND::GetWindowRect(&rect);
	GetEndRect(rect);

	m_nOffset=-1;
	m_BarData.nEdge=m_BarData.nEdge;
	m_BarData.bHiding=TRUE;
	SlideWindow();
	m_BarData.bHidden=TRUE;
	m_BarData.bHiding=FALSE;
	DWORD dwStyle=PARENTWND::GetStyle();
	if (dwStyle  & WS_THICKFRAME)
	{
		m_bSizeable=TRUE;
		PARENTWND::ModifyStyle(WS_THICKFRAME,0);
	}
	if (IsAutoHidden())
	{
		APPBARDATA data;
		ZeroMemory((void*) &data, sizeof(data));
		data.cbSize=sizeof(data);
		data.hWnd=PARENTWND::m_hWnd;
		data.rc=rect;
		data.uEdge=m_BarData.nEdge;
		SetPosition(&data, TRUE);
	}
}

////////////////////////////////////
//UnHide(): unhidethe window in autohidden state

template <class PARENTWND>
void COXAppBar<PARENTWND>::UnHide()
{

	if (!IsAutoHidden())
		return;
	m_nOffset=1;
	m_BarData.nEdge=m_BarData.nEdge;
	m_BarData.bHiding=TRUE;
	SlideWindow();
    m_BarData.bHiding = FALSE;
	m_BarData.bHidden=FALSE;
	if (m_bSizeable)
		PARENTWND::ModifyStyle(0,WS_THICKFRAME);

}

///////////////////////////////////////////////////////
//LoadState(): loads last saved state from the registry

template <class PARENTWND>
BOOL COXAppBar<PARENTWND>::LoadState()
{

	COXRegistryValFile regfile(
		HKEY_CURRENT_USER, AfxGetAppName(), _T("AppBar"));
	CArchive ar(&regfile, CArchive::load);

	BOOL bRet=FALSE;

	
	if (regfile.GetLength()>0)
	{
		int nVersion;
		ar >> nVersion;
		if (nVersion!=OX_APPBAR_VERSION)
			return FALSE;

		BOOL bRegistered;
		ar >> bRegistered;

		if (bRegistered)
		{
			if(!IsRegistered())
				Register(TRUE);
		}
		else
		{
			if (IsRegistered())
				Register(FALSE);
		}

		BOOL bOnTop;
		BOOL bAutoHide;
		
		ar >> bOnTop;
		ar >> bAutoHide;
		ar >> m_BarData.nWidth;
		ar >> m_BarData.nHeight;
		ar >> m_BarData.nLeft;
		ar >> m_BarData.nTop;
		ar >> m_BarData.nRight;
		ar >> m_BarData.nBottom;

		UINT nEdge;
		//UINT nDockEdge;

		ar >> nEdge;//m_BarData.nEdge;
		ar >> m_BarData.nDockEdge;
		ar >> m_BarData.dwStyle;
		ar >> m_nSlidingTime;
		ar >> m_nHiddenWidth;
		ar >> m_nTimerInterval;

		ar >> m_nToDock;
		ar >> m_nToFloat;
		ar >> m_rctFloat.left;
		ar >> m_rctFloat.top;
		ar >> m_rctFloat.right;
		ar >> m_rctFloat.bottom;

		if (IsRegistered())
		{
			SetEdge(nEdge);
			SetAutoHide(bAutoHide);
			SetAlwaysOnTop(bOnTop);
		}
		bRet=TRUE;
	}

	ar.Close();
	return bRet;
}

///////////////////////////////////////////////////
//SaveState(): save the state to the registry

template <class PARENTWND>
void COXAppBar<PARENTWND>::SaveState()
{
	COXRegistryValFile regfile(
		HKEY_CURRENT_USER, AfxGetAppName(), _T("AppBar"));
	CArchive ar(&regfile, CArchive::store);
	ar << OX_APPBAR_VERSION;
	ar << m_BarData.bRegistered;
	ar << m_BarData.bOnTop;
	ar << m_BarData.bAutoHideDesired;
	ar << m_BarData.nWidth;
	ar << m_BarData.nHeight;
	ar << m_BarData.nLeft;
	ar << m_BarData.nTop;
	ar << m_BarData.nRight;
	ar << m_BarData.nBottom;
	ar << m_BarData.nEdge;
	ar << m_BarData.nDockEdge;
	ar << m_BarData.dwStyle;
	ar << m_nSlidingTime;
	ar << m_nHiddenWidth;
	ar << m_nTimerInterval;
	ar << m_nToDock;
	ar << m_nToFloat;
	ar << m_rctFloat.left;
	ar << m_rctFloat.top;
	ar << m_rctFloat.right;
	ar << m_rctFloat.bottom;

	ar.Close();
}

////////////////////////////////////////////////////
//SlideWindow(): slides the window in autohidden state 
//hiding/unhiding

template <class PARENTWND>
void COXAppBar<PARENTWND>::SlideWindow()
{
	CRect rctCurrent;
	PARENTWND::GetWindowRect(&rctCurrent);
	CRect rctEnd=rctCurrent;

	GetEndRect(rctEnd);


	DWORD dwCount=::GetTickCount();

	while(CanContinue(rctCurrent,rctEnd))
	{
		if ((dwCount-::GetTickCount())>m_nSlidingTime)
		{
			dwCount=::GetTickCount();
			switch (m_BarData.nEdge)
			{
			case ABE_TOP:
				rctCurrent.bottom+=m_nOffset;
				break;
			case ABE_BOTTOM:
				rctCurrent.top-=m_nOffset;	
				break;
			case ABE_LEFT:
				rctCurrent.right+=m_nOffset;
				break;
			case ABE_RIGHT:
				rctCurrent.left-=m_nOffset;
				break;
			}
			PARENTWND::MoveWindow(&rctCurrent);
		}
	}

	PARENTWND::MoveWindow(&rctEnd);
	
}

////////////////////////////////////////////////////
//CanContinue(): helper function for SlideWindow()
//returns FALSE when appbar rect reached final position 

template <class PARENTWND>
BOOL COXAppBar<PARENTWND>::CanContinue(CRect& rctCurrent, CRect& rctEnd)
{
	switch (m_BarData.nEdge)
	{
	case ABE_TOP:
	case ABE_BOTTOM:
		if (m_BarData.bHidden)
			return (BOOL) (rctCurrent.Height()<rctEnd.Height());
		else
			return (BOOL) (rctCurrent.Height()>rctEnd.Height());
	case ABE_LEFT:
	case ABE_RIGHT:
		if (m_BarData.bHidden)
			return (BOOL) (rctCurrent.Width()<rctEnd.Width());
		else
			return (BOOL) (rctCurrent.Width()>rctEnd.Width());
	default:
		ASSERT(FALSE);
	}
	return FALSE;
}

////////////////////////////////////////////////////
//GetEndRect(): helper function returns rectangle
//appbar should reach while hiding/unhiding depends
//on docking edge

template <class PARENTWND>
void COXAppBar<PARENTWND>::GetEndRect(CRect& rect)
{
	switch (m_BarData.nEdge)
	{
	case ABE_TOP:
		if (m_BarData.bHidden)
			rect.bottom=rect.top+m_BarData.nHeight;
		else
			rect.bottom=rect.top+GetHiddenWidth();
	break;
	case ABE_BOTTOM:
		if (m_BarData.bHidden)
			rect.top=rect.bottom-m_BarData.nHeight;
		else
			rect.top=rect.bottom-GetHiddenWidth();
	break;
	case ABE_LEFT:
		if (m_BarData.bHidden)
			rect.right=rect.left+m_BarData.nWidth;
		else
			rect.right=rect.left+GetHiddenWidth();
	break;
	case ABE_RIGHT:
		if (m_BarData.bHidden)
			rect.left=rect.right-m_BarData.nWidth;
		else
			rect.left=rect.right-GetHiddenWidth();
		
	break;
	}
}

//////////////////////////////////////////////////////
//GetProposedRect(): returns proposed rect for the edge
//the appbar will switch to 

template <class PARENTWND>
void COXAppBar<PARENTWND>::GetProposedRect(CRect& rect)
{
	rect.left=0;
	rect.top=0;
	rect.right=::GetSystemMetrics(SM_CXSCREEN);
	rect.bottom=::GetSystemMetrics(SM_CYSCREEN);

	if (m_BarData.dwStyle & OX_APPBARS_DIFFERENT_DIMS)
	{
		switch (m_BarData.nEdge)
		{
		case ABE_UNKNOWN:
			ASSERT(FALSE);
			break;
		case ABE_FLOAT:
			{
				CPoint pt;
				::GetCursorPos(&pt);
				rect.left=pt.x-(m_rctFloat.right-m_rctFloat.left)/2;
				rect.right=rect.left+m_rctFloat.right-m_rctFloat.left;
				rect.top=pt.y-DIM_CAPTION;
				rect.bottom=rect.top+m_rctFloat.bottom-m_rctFloat.top;
			}
			break;
		case ABE_LEFT:
			rect.right=m_BarData.nLeft;
			break;
		case ABE_TOP:
			rect.bottom=m_BarData.nTop;
			break;
		case ABE_RIGHT:
			rect.left=rect.right-m_BarData.nRight;
			break;
		case ABE_BOTTOM:
			rect.top=rect.bottom-m_BarData.nBottom;
			break;
		}
		return;
	}

	switch (m_BarData.nEdge)
	{
	case ABE_UNKNOWN:
		ASSERT(FALSE);
		break;
	case ABE_FLOAT:
		PARENTWND::GetWindowRect(&rect);
		break;
	case ABE_LEFT:
		rect.right=m_BarData.nWidth;
		break;
	case ABE_TOP:
		rect.bottom=m_BarData.nHeight;
		break;
	case ABE_RIGHT:
		rect.left=rect.right-m_BarData.nWidth;
		break;
	case ABE_BOTTOM:
		rect.top=rect.bottom-m_BarData.nHeight;
		break;
	}
}

////////////////////////////////////////////////////
//GetProposedEdge(): propose the edge depends on current
//placement of the appbar and position of the mouse,
//while button pressed

template <class PARENTWND>
UINT COXAppBar<PARENTWND>::GetProposedEdge()
{

	POINT pt;
	CRect rectScreen;
	rectScreen.top=0;
	rectScreen.left=0;
	rectScreen.right = GetSystemMetrics(SM_CXSCREEN);
	rectScreen.bottom = GetSystemMetrics(SM_CYSCREEN);

	VERIFY(GetCursorPos(&pt));
	CRect  rectCurrent;
	PARENTWND::GetWindowRect(&rectCurrent);

	UINT nNewEdge=m_BarData.nEdge;
	UINT nLeftRight;
	UINT nTopBottom;
	UINT nDiffr=rectScreen.right;

	switch (m_BarData.nEdge)
	{
	case ABE_UNKNOWN:
		return ABE_UNKNOWN;
	case ABE_TOP:
	case ABE_LEFT:
	case ABE_RIGHT:
	case ABE_BOTTOM:
		if (!(m_BarData.dwStyle & OX_APPBARS_FLOAT))
		{
			if (m_BarData.dwStyle & (OX_APPBARS_LEFT | OX_APPBARS_RIGHT)) 
				nLeftRight=(pt.x>(rectScreen.right/2))?ABE_RIGHT:ABE_LEFT;
			else
				if (m_BarData.dwStyle & OX_APPBARS_LEFT)
					nLeftRight=ABE_LEFT;
				else
					if (m_BarData.dwStyle & OX_APPBARS_RIGHT)
						nLeftRight=ABE_RIGHT;
					else
						nLeftRight=ABE_UNKNOWN;
			if (m_BarData.dwStyle & (OX_APPBARS_TOP | OX_APPBARS_RIGHT))
				nTopBottom=(pt.y>(rectScreen.bottom/2))?ABE_BOTTOM:ABE_TOP;
			else
				if (m_BarData.dwStyle & OX_APPBARS_TOP)
					nTopBottom=ABE_TOP;
				else
					if (m_BarData.dwStyle & OX_APPBARS_BOTTOM)
						nTopBottom=ABE_BOTTOM;
					else
						nTopBottom=ABE_UNKNOWN;
			switch( nLeftRight)
			{
			case ABE_UNKNOWN:
				if (nTopBottom==ABE_UNKNOWN)
				{
					ASSERT(FALSE);
					return m_BarData.nEdge;
				}
				return nTopBottom;
			case ABE_LEFT:
				if (nTopBottom==ABE_UNKNOWN)
					return ABE_LEFT;
				if (nTopBottom==ABE_TOP)
				{
					return (pt.x*rectScreen.Height()<pt.y*rectScreen.Width())?ABE_LEFT:ABE_TOP;
				}
				else
					return (pt.x*rectScreen.Height()<(rectScreen.Height()-pt.y)*rectScreen.Width())?ABE_LEFT:ABE_BOTTOM;
			case ABE_RIGHT:
				if (nTopBottom==ABE_UNKNOWN)
					return ABE_RIGHT;
				if (nTopBottom==ABE_TOP)
				{
					return ((rectScreen.Width()-pt.x)*rectScreen.Height()<pt.y*rectScreen.Width())?ABE_RIGHT:ABE_TOP;
				}
				else
					return ((rectScreen.Width()-pt.x)*rectScreen.Height()<(rectScreen.Height()-pt.y)*rectScreen.Width())?ABE_RIGHT:ABE_BOTTOM;
			}

		}
		rectCurrent.InflateRect(m_nToFloat,m_nToFloat);
		if (rectCurrent.PtInRect(pt))
			return m_BarData.nEdge;
		nNewEdge=ABE_FLOAT;
		//no break here!
	case ABE_FLOAT:
		if (pt.x<(int) m_nToDock && (m_BarData.dwStyle & OX_APPBARS_LEFT))
		{
			nNewEdge=ABE_LEFT;
			nDiffr=pt.x;
		}
		if (pt.y<(int) m_nToDock && ((UINT) abs(pt.y)<nDiffr) && (m_BarData.dwStyle & OX_APPBARS_TOP))
		{
			nNewEdge=ABE_TOP;
			nDiffr=pt.y;
		}
		if ((UINT) abs(rectScreen.right-pt.x)<nDiffr && rectScreen.right-pt.x<(int) m_nToDock &&
			(m_BarData.dwStyle & OX_APPBARS_RIGHT))
		{
			nNewEdge=ABE_RIGHT;
			nDiffr=rectScreen.right-pt.x;
		}
		if ((UINT) abs(rectScreen.bottom-pt.y)<nDiffr && rectScreen.bottom-pt.y<(int) m_nToDock &&
			(m_BarData.dwStyle & OX_APPBARS_BOTTOM))
		{
			nNewEdge=ABE_BOTTOM;
		}
		return nNewEdge;

	}

	return m_BarData.nEdge;
}

///////////////////////////////////////////////////
//Float() unregisters appbar temporary for floating

template <class PARENTWND>
void COXAppBar<PARENTWND>::Float(APPBARDATA* pData)
{
	SHAppBarMessage(ABM_REMOVE, pData);
	m_bFloating=TRUE;
}

//////////////////////////////////////////////////
//UnFloat() registers the appbar after floating

template <class PARENTWND>
void COXAppBar<PARENTWND>::UnFloat(APPBARDATA* pData)
{
	ASSERT(IsRegistered());

	if (m_bFloating)
		m_bFloating=!SHAppBarMessage(ABM_NEW,pData);

}


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_OXAPPBAR_H__)
