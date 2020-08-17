// OXFullScreenFrame.h: interface for the COXFullScreenFrame class.
//////////////////////////////////////////////////////////////////////
//
// ==========================================================================
// 					Class Specification : COXFullScreenFrame
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                                            

// //////////////////////////////////////////////////////////////////////////

/*

  OVERVIEW

	This is a simple template class that provides management of 
	fullscreen representation of a view in the Document/View architecture.
	Usually the only thing you have to do to use this class is
	take your main frame window as a PARENT_MAIN_FRAME for this 
	template and call SetFullScreen() function in an appropriate
	place. This class works with both, SDI and MDI types of applications.
	Some customization may be needed sometimes. While the class
	trying to switch to the fullscreen mode you can prevent it
	by overriding virtual function CanShowFullScreen() and return 
	FALSE to prevent from switching to the fullscreen mode.
	The same applies to switching to the normal mode - override
	CanShowNormalScreen() and return FALSE to prevent.
	While the app is switching to or from fullscreen mode it
	calls virtual function OnFullScreen() to inform about changing 
	the mode. 
	By default, for switching between the modes are used virtual keys
	VK_F8 (to switch to full screen mode) and VK_ESCAPE (to switch
	to normal mode).
	To define another keys , define 
	OX_FULLSCREEN_ESCAPE_KEY and OX_FULLSCREEN_ENTER_KEY in stdafx.h

*/


#if !defined(_OXFULLSCREENFRAME_H__)
#define _OXFULLSCREENFRAME_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "OXDllExt.h"

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#define __AFXTEMPL_H__
#endif


#ifndef OX_FULLSCREEN_ESCAPE_KEY
#define OX_FULLSCREEN_ESCAPE_KEY VK_ESCAPE
#endif

#ifndef OX_FULLSCREEN_ENTER_KEY
#define OX_FULLSCREEN_ENTER_KEY VK_F8
#endif



template <class PARENT_MAIN_FRAME>
class OX_CLASS_DECL COXFullScreenFrame : public PARENT_MAIN_FRAME
{
public:

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXFullScreenFrame();

	virtual ~COXFullScreenFrame();

	// --- In  : pFrame - A pointer to the frame to set fullscreen mode to.
	//					  To switch to the fullscreen mode must not be NULL.
	//					  To switch to the normal mode ignored
	//			 bSetFullScreen - TRUE to switch to the fullscreen mode,
	//							  FALSE to switch to the normal mode
	// --- Out : 
	// --- Returns: TRUE on success, FALSE otherwise. If the app was
	//				in requested mode so far, returns TRUE.
	// --- Effect :	Call this function to change the mode.
	virtual BOOL SetFullScreen(CFrameWnd* pFrame, BOOL bSetFullScreen=TRUE);
	
	// --- In  :
	// --- Out : 
	// --- Returns: TRUE if the app is in fullscreen mode,
	//				FALSE otherwise
	// --- Effect :	Determines fullscreen state of the app
	inline BOOL IsFullScreen() const
	{	
		return m_bFullScreen;	
	}

	// --- In  :
	// --- Out : 
	// --- Returns: A pointer to the frame window that is in fullscreen mode.
	// --- Effect : Retrieves a pointer to the fullscreen frame window
	//				If the app is not in fullscreen mode, returns NULL
	inline CFrameWnd* GetFullScreenFrame() const
	{
		return m_pFullScreenFrame;
	}

protected:
	
	// --- In  :
	// --- Out : 
	// --- Returns: TRUE to switch to fullscreen size, 
	//				FALSE to prevent exit from the normal mode
	// --- Effect : This virtual function is provided for 
	//				customizable of switching of modes.
	//				The default imlementation returns TRUE
	virtual BOOL CanShowFullScreen()
	{
		return TRUE;
	}

	// --- In  :
	// --- Out : 
	// --- Returns: TRUE to switch to normal size, 
	//				FALSE to prevent exit from the fullscreen mode
	// --- Effect : This virtual function is provided for 
	//				customizable of switching of modes.
	//				The default imlementation returns TRUE
	virtual BOOL CanShowNormalScreen()
	{
		return TRUE;
	}

	// --- In  : bFullScreen - TRUE if the appp has switched to the fullscreen mode,
	//						   FALSE if the app has switched to the normal mode	
	// --- Out : 
	// --- Returns:
	// --- Effect : This function is called every time the app changes 
	//				the mode
	virtual void OnFullScreen(BOOL bFullScreen)
	{
		UNREFERENCED_PARAMETER(bFullScreen);
	}

	void SetHook();
	void RemoveHook();
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);

protected:

	CPtrArray m_Bars;
	BOOL m_bFullScreen;
	WINDOWPLACEMENT m_plcMainFrame;
	WINDOWPLACEMENT m_plcChildFrame;
	CFrameWnd* m_pFullScreenFrame;
	BOOL m_bMDIApp;
 	HHOOK m_hHook;
	CToolBar* m_pToolBar;
	static COXFullScreenFrame* m_pFullScreen;
	CRect m_rctFullScreen;
};

template<class PARENT_MAIN_FRAME>
COXFullScreenFrame<PARENT_MAIN_FRAME>*   COXFullScreenFrame<PARENT_MAIN_FRAME>::m_pFullScreen; 


//////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////


template <class PARENT_MAIN_FRAME>
COXFullScreenFrame<PARENT_MAIN_FRAME>::COXFullScreenFrame() :
m_bFullScreen(FALSE), m_pFullScreenFrame(NULL), m_hHook(NULL),
m_pToolBar(NULL)
{
	m_pFullScreen=this;
	::ZeroMemory((void*) &m_plcMainFrame, sizeof(WINDOWPLACEMENT));
	::ZeroMemory((void*) &m_plcChildFrame, sizeof(WINDOWPLACEMENT));

	CFrameWnd* pFrame=DYNAMIC_DOWNCAST(CFrameWnd,this);
	
	//must be CFrameWnd derived 
	ASSERT(pFrame);

	//must not be child window
	CMDIChildWnd* pMdiChild=DYNAMIC_DOWNCAST(CMDIChildWnd,this);
	ASSERT(pMdiChild==NULL);

	//determine type of the main window
	CMDIFrameWnd* pMDIFrame=DYNAMIC_DOWNCAST(CMDIFrameWnd,this);
	m_bMDIApp=pMDIFrame?TRUE:FALSE;

	m_rctFullScreen.SetRectEmpty();

}

template <class PARENT_MAIN_FRAME>
COXFullScreenFrame<PARENT_MAIN_FRAME>::~COXFullScreenFrame()
{

}

template <class PARENT_MAIN_FRAME>
LRESULT COXFullScreenFrame<PARENT_MAIN_FRAME>::WindowProc(
			UINT message, WPARAM wParam, LPARAM lParam )
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
	case WM_DESTROY:
		RemoveHook();
		return PARENT_MAIN_FRAME::WindowProc(message, wParam, lParam);
	case WM_CREATE:
		SetHook();
		return PARENT_MAIN_FRAME::WindowProc(message, wParam, lParam);
	case WM_GETMINMAXINFO:
		{
			if (IsFullScreen())
			{
				LPMINMAXINFO lpMinMax=(LPMINMAXINFO) lParam;
				ASSERT(lpMinMax);
				lpMinMax->ptMaxSize.y = m_rctFullScreen.Height();
				lpMinMax->ptMaxTrackSize.y = lpMinMax->ptMaxSize.y;
				lpMinMax->ptMaxSize.x = m_rctFullScreen.Width();
				lpMinMax->ptMaxTrackSize.x = lpMinMax->ptMaxSize.x;
			}
		}
	default:
		return PARENT_MAIN_FRAME::WindowProc(message, wParam, lParam);
	}
}

template <class PARENT_MAIN_FRAME>
void COXFullScreenFrame<PARENT_MAIN_FRAME>::SetHook()
{
	if (m_hHook)
		RemoveHook();
	m_hHook=::SetWindowsHookEx(WH_KEYBOARD,KeyboardProc,NULL,
		GetCurrentThreadId());
	ASSERT(m_hHook);
}


template <class PARENT_MAIN_FRAME>
void COXFullScreenFrame<PARENT_MAIN_FRAME>::RemoveHook()
{
	if (m_hHook)
	{
		UnhookWindowsHookEx(m_hHook);
		m_hHook=NULL;
	}
}

template <class PARENT_MAIN_FRAME>
LRESULT COXFullScreenFrame<PARENT_MAIN_FRAME>::KeyboardProc(
			int code, WPARAM wParam, LPARAM lParam)
{
	
	if (code<0)
		return ::CallNextHookEx(m_pFullScreen->m_hHook,code, wParam, lParam);

	if (wParam==OX_FULLSCREEN_ESCAPE_KEY && m_pFullScreen->IsFullScreen())
	{
		m_pFullScreen->SetFullScreen(NULL,FALSE);
		if (code==HC_NOREMOVE)
		{
			MSG Msg;
			::PeekMessage(&Msg,NULL,WM_KEYFIRST,WM_KEYLAST,PM_REMOVE);
		}
		return TRUE;
	}
	else
		if (wParam==OX_FULLSCREEN_ENTER_KEY && !m_pFullScreen->IsFullScreen())
		{
			CFrameWnd* pFrame=m_pFullScreen->PARENT_MAIN_FRAME::GetActiveFrame();
			if (pFrame)
			{
				m_pFullScreen->SetFullScreen(pFrame,TRUE);
				if (code==HC_NOREMOVE)
				{
					MSG Msg;
					::PeekMessage(&Msg,NULL,WM_KEYFIRST,WM_KEYLAST,PM_REMOVE);
				}
				return TRUE;
			}
		}
	return ::CallNextHookEx(m_pFullScreen->m_hHook,code, wParam, lParam);
}


template <class PARENT_MAIN_FRAME>
BOOL COXFullScreenFrame<PARENT_MAIN_FRAME>::SetFullScreen(
		CFrameWnd* pFrame, BOOL bSetFullScreen)
{
	ASSERT(PARENT_MAIN_FRAME::GetSafeHwnd());

	if (bSetFullScreen && IsFullScreen())
		return TRUE;

	if (!bSetFullScreen && !IsFullScreen())
		return TRUE;

	if (bSetFullScreen)
	{
		if (!CanShowFullScreen())
			return FALSE;
		ASSERT(pFrame->GetSafeHwnd());
#ifdef _DEBUG
		pFrame->AssertValid();
#endif
		if (pFrame->m_hWnd==PARENT_MAIN_FRAME::m_hWnd)
		{
			ASSERT(!m_bMDIApp);
			if (m_bMDIApp)
				return FALSE;
		}

		PARENT_MAIN_FRAME::GetWindowPlacement(&m_plcMainFrame);
		if (m_bMDIApp)
		{
			pFrame->GetWindowPlacement(&m_plcChildFrame);
		}
		
		::SystemParametersInfo(SPI_GETWORKAREA,NULL,&m_rctFullScreen, NULL);
		
		{
			// Iterate all the control bars and hide them
			POSITION pos=PARENT_MAIN_FRAME::m_listControlBars.GetHeadPosition();
			while(pos!=NULL)
			{
				CControlBar* pControlBar=
					(CControlBar*)PARENT_MAIN_FRAME::m_listControlBars.GetNext(pos);
				if(pControlBar!=NULL && pControlBar->GetStyle()&WS_VISIBLE)
				{
					m_Bars.Add(pControlBar);
					PARENT_MAIN_FRAME::ShowControlBar(pControlBar,FALSE,TRUE);
				}
			}
		}

		AdjustWindowRectEx(&m_rctFullScreen,
			PARENT_MAIN_FRAME::GetStyle(),
			PARENT_MAIN_FRAME::GetMenu()?TRUE:FALSE,
			PARENT_MAIN_FRAME::GetExStyle());

		WINDOWPLACEMENT plcUpdated;
		plcUpdated=m_plcMainFrame;
		plcUpdated.rcNormalPosition=m_rctFullScreen;
		plcUpdated.showCmd=SW_SHOWNORMAL;

		m_bFullScreen=TRUE;
		m_pFullScreenFrame=pFrame;

		PARENT_MAIN_FRAME::SetWindowPlacement(&plcUpdated);
		if (m_bMDIApp)
			pFrame->ShowWindow(SW_SHOWMAXIMIZED);
	}
	else
	{
		if (!CanShowNormalScreen())
			return FALSE;
		m_bFullScreen=FALSE;
		PARENT_MAIN_FRAME::SetWindowPlacement(&m_plcMainFrame);
		if (m_bMDIApp)
			m_pFullScreenFrame->SetWindowPlacement(&m_plcChildFrame);
		for (int n=0; n<m_Bars.GetSize(); n++)
		{
			CControlBar* pBar=(CControlBar*) m_Bars.GetAt(n);
			ASSERT(pBar);
			if (::IsWindow(pBar->m_hWnd))
				PARENT_MAIN_FRAME::ShowControlBar(pBar,TRUE,TRUE);
		}
		m_pFullScreenFrame=NULL;
	}

	OnFullScreen(m_bFullScreen);

	return TRUE;

}


#endif // !defined(_OXFULLSCREENFRAME_H__)
