// ===================================================================================
// 					Class Specification : COXSizableMiniDockFrameWnd
// ===================================================================================

// Header file : OXSzMiniDockFrmWnd.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
// Some portions Copyright (C)1994-5	Micro Focus Inc, 2465 East Bayshore Rd, Palo Alto, CA 94303.
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CMiniDockFrameWnd

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	NO 	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//
// This class implements all the sizeable functionality a mini frame window needs.
// Normally a CMiniDockFrameWindow is not resizeable.

// Remark:
//

// Prerequisites (necessary conditions):
//		***

/////////////////////////////////////////////////////////////////////////////

#ifndef __SIZEMINIDOCKFRM_H__
#define __SIZEMINIDOCKFRM_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include <afxpriv.h>
#include "OXDragDockContext.h"
#include "OXCoolToolBar.h"
#include "OXSizeDockBar.h"	// Added by ClassView

// Define the WM_QUERYSNAPPING user message used to query a frame window whether or not
// the snaping and tear-off behavior is enabled
#ifndef WM_QUERYSNAPPING
	#define WM_QUERYSNAPPING (WM_USER + 92)
#endif

class COXMiniFrameSkin;

class OX_CLASS_DECL COXSizableMiniDockFrameWnd : public CMiniDockFrameWnd
{
	friend COXDragDockContext; 		// access to IgnoreSysMove flag
	friend class COXMiniFrameSkinClassic;
	friend class COXMiniFrameSkinXP;

// Data members -------------------------------------------------------------
public:

	enum ContainedBarType { Unknown, MFCBase, MRCSizeBar } ;

	BOOL m_bBeingDestroyed;

protected:
	// Deternines if the mini frame is currently being dragger
	enum ContainedBarType m_nContainedBarType;
	COXMiniFrameSkin* GetMiniFrameSkin();
	COXSizableMiniDockFrameWnd();
	~COXSizableMiniDockFrameWnd();

private :
	COXMiniFrameSkin* m_pMiniFrameSkin;
	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
	static HHOOK m_hMouseHook;
	static HWND m_hwndPrevMouseMoveWnd;

// Member functions ---------------------------------------------------------
public:

	enum ContainedBarType GetContainedBarType();
	// --- In  :
	// --- Out : 
	// --- Returns : the type of the contained bar
	// --- Effect : Floating frames are created from CFrameWnd::CreateFloatingFrame(), and at
	//				this point, we don't know what type of control bar will eventually be in
	//				this frame. This routine determines the type of control bar, and sets the
	//				neccessary styles. Generally this routine gets called as soon as we do a
	//				WM_NCHITTEST on the window - ie before the user can get a chance to
	//				manipulate it with the mouse
	//
	//				COXSizeControlBar - default style of WS_THICKFRAME is ok (we've overridden
	//									the create too) but we need to ensure "SC_SIZE" is on
	//									the menu, otherwise we can't the default WM_NCLBUTTONDOWN
	//									won't generate an SC_SIZE.
	//
	//				CControlBar, CBRS_SIZE_DYNAMIC
	//								  - set MFS_4THICKFRAME (disallow diagonal sizing)
	//
	//				CControlBar, not CBRS_SIZE_DYNAMIC
	//								  - remove WS_THICKFRAME & add MFS_MOVEFRAME (we don't want
	//									sizing hit test values)

	inline BOOL IsBeingDestroyed() { return m_bBeingDestroyed; }
	BOOL IsDockingViewBar();

protected:
//{{AFX_MSG(COXSizableMiniDockFrameWnd)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnClose();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint pt);
#if _MSC_VER >= 1400
	afx_msg LRESULT OnNcHitTest(CPoint point);
#else
	afx_msg UINT OnNcHitTest(CPoint point);
#endif	
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnInitMenuPopup(CMenu*, UINT, BOOL);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    DECLARE_DYNCREATE(COXSizableMiniDockFrameWnd)

//{{AFX_VIRTUAL(COXSizableMiniDockFrameWnd)
    protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//}}AFX_VIRTUAL

};

#endif // __SIZEMINIDOCKFRM_H__

