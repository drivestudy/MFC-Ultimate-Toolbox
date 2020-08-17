// ==========================================================================
// 					Class Specification : COXScrollTipOwner
// ==========================================================================

// Header file : OXScrollTipOwner.h

// Version: 9.3

// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CObject

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption
//	This class organizes tooltips used with scrollbars : scroll tips
//	A scroll tip shows the current position of the tracking thumb of the scrollbar
//	This tip is shown while the user moves the thumb with his mouse pointer.

//	This class can be used with individual scroll bars or with windows that
//	 have the WS_VSCROLL or WS_HSCROLL style (edit controls etc.)

//	You can specify the text that should be shown in the scroll tip.
//	The following parts have a specifial meaning and will be replaced by
//	 the current value during runtime :
//		"%p" : The current page number 
//		"%l" : The current position (line) relative to the beginning of the page
//		"%a" : The current absolute position
//	All numbers are 1-based

// Remark:
//	Thie class uses dynamic subclassing.  When a scroll bar control is used,
//	 it's parent is subclassed.  Otherwise the window itself is subclassed.
//	Windowss subclassing is used. which is completely seperate from MFC subclassing.
//	A window may be subclassed more than once (e.g. to use both the horizontal and
//	 vertical scrollbar of an edit control).  You will have to use different
//	 COXScrollTipOwner objects for this.

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXSCROLLTIPOWNER_H__
#define __OXSCROLLTIPOWNER_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#ifndef __AFXTEMPL_H__
#include <AfxTempl.h>
#define __AFXTEMPL_H__
#endif

#include "OXScrollTip.h"


class OX_CLASS_DECL COXScrollTipOwner : public CObject
{
DECLARE_DYNAMIC(COXScrollTipOwner);

// Data members -------------------------------------------------------------
public:
	static LPCTSTR m_pszPageCode;
	// --- The part that will be replaced by the current page number ("%p")
	static LPCTSTR m_pszPosInPageCode;
	// --- The part that will be replaced by relative position ("%l")
	static LPCTSTR m_pszAbsPosCode;
	// --- The part that will be replaced by absolute position ("%a")

protected:
	HWND	m_hWnd;
	WNDPROC m_pfnSuper;
	BOOL	m_bHorizontal;
	HWND	m_hScrollBar;

	COXScrollTip		m_scrollTip;
	CString				m_sMask;
	BOOL				m_bLeftTop;
	BOOL				m_bPageAlign;

	COXScrollTipOwner* m_pPrevHandler;
	COXScrollTipOwner* m_pNextHandler;
	static CMap<HWND, HWND, COXScrollTipOwner*, COXScrollTipOwner*> m_allScrollTipOwners;

	static const int m_nHorizontalGap;
	static const int m_nVerticalGap;

private:
	
// Member functions ---------------------------------------------------------
public:
	COXScrollTipOwner();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object

	BOOL InterceptHorizontalScroll(CWnd* pWnd, BOOL bForceStyle = FALSE);
	// --- In  : pWnd : The window of which the horizontal scrollbar should be used
	//					or the horizontal scrollbar itself
	//			 bForceStyle : Whether the window should be accepted as having the
	//                         WS_HSCROLL style even if it does not.
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Subclasses the window (or the scrollbar's parent) to intercept the
	//				scrolling messages

	BOOL InterceptVerticalScroll(CWnd* pWnd, BOOL bForceStyle = FALSE);
	// --- In  : pWnd : The window of which the vertical scrollbar should be used
	//					or the vertical scrollbar itself
	//			 bForceStyle : Whether the window should be accepted as having the
	//                         WS_VSCROLL style even if it does not.
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Subclasses the window (or the scrollbar's parent) to intercept the
	//				scrolling messages

	void RemoveScrollTipSupport();
	// --- In  : 
	// --- Out : 
	// --- Returns:	
	// --- Effect :	Unsubclasses the window (or the scrollbar's parent)

	void SetMask(LPCTSTR pszMask);
	// --- In  : pszMask : The mask to set
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets the mask for the scrolltip text, it may include the parts
	//				m_pszPageCode, m_pszPosInPageCode and m_pszAbsPosCode
	
	CString GetMask() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : The mask of the scroll tip text
	// --- Effect : 

	void SetLeftTop(BOOL bLeftTop = TRUE);
	// --- In  : bLeftTop : Whether the scrolltip should be shown left (vertical) or
	//						at the top (horizontal).  When FALSE is specified it is shown
	//						at the right hand side (vertical) or at the bottom (horizontal)
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets scroll tip's position

	BOOL GetLeftTop() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether the scrolltip should be shown left (vertical) or
	//				 at the top (horizontal).  When FALSE is returned it is shown
	//				 at the right hand side (vertical) or at the bottom (horizontal)
	// --- Effect : 

	void SetPageAlign(BOOL bPageAlign = TRUE);
	// --- In  : bPageAlign : Whether the tooltip should be aligned to the 
	//						  beginning of the current page or net
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets scroll tip's alignment

	BOOL GetPageAlign() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether the tooltip is aligned to the beginning of the 
	//				 current page or net
	// --- Effect : 

	void EnableFastBackgroundRepaint(BOOL bEnable = TRUE);
	// --- In  : bEnable : Whether to enable or disable
	// --- Out : 
	// --- Returns : 
	// --- Effect : The scroll tip can request the background to be repainted 
	//              when it is moved (during scrolling)
	//				When it is enabled WM_PAINTs will be send, otherwise they will be
	//				posted (will introduces some delay)
	//				This feature is enabled by default

#ifdef _DEBUG
	virtual void AssertValid() const;
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : AssertValid performs a validity check on this object 
	//				by checking its internal state. 
	//				In the Debug version of the library, AssertValid may assert and 
	//				thus terminate the program.
	
	virtual void Dump(CDumpContext& dc) const;
	// --- In  : dc : The diagnostic dump context for dumping, usually afxDump.
	// --- Out : 
	// --- Returns :
	// --- Effect : Dumps the contents of the object to a CDumpContext object. 
	//				It provides diagnostic services for yourself and 
	//				 other users of your class. 
	//				Note  The Dump function does not print a newline character
	//				 at the end of its output. 
#endif

	virtual ~COXScrollTipOwner();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object

protected:
	BOOL SubclassScrollTipOwner(CWnd* pWnd, BOOL bHorizontal, BOOL bForceStyle);
	void UnsubclassScrollTipOwner();

	static LRESULT CALLBACK GlobalScrollTipOwnerProc(HWND hWnd, UINT uMsg, 
		WPARAM wParam, LPARAM lParam);
	virtual LRESULT ScrollTipOwnerProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnHScroll(UINT nSBCode, int nPos, HWND hScrollbar);
	virtual BOOL OnVScroll(UINT nSBCode, int nPos, HWND hScrollbar);

	BOOL CreateScrollTip();
	void DestroyScrollTip();
	BOOL AdjustScrollTip();
	BOOL GetScrollInfo(int& nPage, int& nPagePos, int& nAbsPos, int& nPageSize);
	virtual CString FillMask(int nPage, int nPagePos, int nAbsPos);
	CPoint ComputeScrollTipPosition(int nPage, int nPagePos, int nAbsPos, 
		int nPageSize, CSize newSize);

private:
                   
};

#endif // __OXSCROLLTIPOWNER_H__
// ==========================================================================
