// ==========================================================================
// 					Class Specification : COXTitleBar
// ==========================================================================

// Header file : oxtlebar.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CWnd

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	YES Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//		This class is used as the special caption of a COXRollup dialog

// Remark:
//		This file is part of Rollup dialogs implementation. It implements the
//		"non-client-area" or the TitleBar (therefore the classname). It supports
//		a system menu, which notifies the parent on opening, a rollup and rolldown
//		"button" on the right side of the titlebar. Moving around the rollup is
//		also implemented in this class, because dragging starts with clicking in
//		the titlebar area. You can extend the Titlebar with your own Buttons by
//		overriding DrawBar and HitTest with your own versions.

// Prerequisites (necessary conditions):
//		***

/////////////////////////////////////////////////////////////////////////////

#ifndef __TITLEBAR_H__
#define __TITLEBAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


// really arbitrary values; don´t rely on
#define SCROLL_DOWN		12
#define SCROLL_UP		27

class OX_CLASS_DECL COXTitleBar : public CWnd
{
DECLARE_DYNAMIC(COXTitleBar);

// Data members -------------------------------------------------------------
public:
	enum
		{
		active		= COLOR_ACTIVECAPTION,
		inactive    = COLOR_INACTIVECAPTION
		};

protected:
	CString		m_sWndClassTitlebar;
	CSize		m_bmSize;

	CBitmap		m_bitmapAll;
	BOOL		m_bSysmenuOpen;
	int	 		m_nRollupState;
    CFont		m_fntStandard;
    int			m_nActivation;

	BOOL 		m_bDragging;		// are we in drag mode?
	CPoint		m_ptLast;			// Last point
	CPoint		m_ptMouse;			// Mouse position

private:

// Member functions ---------------------------------------------------------
public:
	COXTitleBar();
	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object
	//				It will initialize the internal state

	BOOL Create(LPCTSTR lpszCaption, CRect& rc, CWnd* pParent, UINT nID);
	// --- In  : lpszCaption : Window text of the Rollup
	//			 rc : Window rectangle of Titlebar
	//			 pParent : Pointer to parent COXRollup derived dialog
	//			 nID : Dialog ID (IDC_CAPTION is used by default)
	// --- Out : 
	// --- Returns : succeeded or not
	// --- Effect : This function is called to create the titlebar for the rollup.

	virtual BOOL LoadBitmap(LPCTSTR lpctstrRes);
	// --- In  : lpctstrRes : Resource string for the bitmap
	// --- Out : 
	// --- Returns : suceeded or not
	// --- Effect : Loads bitmap for system menu and rollup/down states

	int ActiveStyle();
	// --- In  : 
	// --- Out : 
	// --- Returns : Returns COXTitleBar::active or COXTitleBar::inactive
	// --- Effect : 

	void SetActivation(int nActivation);
	// --- In  : nActivation : COXTitleBar::active or COXTitleBar::inactive
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets activation style of the Titlebar

	virtual void DrawBar(CDC* pDC, BOOL bWithCaption);
	// --- In  : pDC : device context to draw titlebar on
	//			 bWithCaption : needs caption a redraw?
	// --- Out : 
	// --- Returns :
	// --- Effect : Override if you want to draw the titlebar yourself

	virtual LRESULT HitTest(UINT nFlags, CPoint& point);
	// --- In  : nFlags : Like OnLButtonDown nFlags
	//			 point : Like OnLButtonDown point
	// --- Out : 
	// --- Returns :HTSYSMENU, HTCAPTION, HTSIZE and HTNOWHERE.
	// --- Effect : Override this function if you want to provide some special
	//				„hot areas" in the titlebar. Predefined area codes are HTSYSMENU,
	//				HTCAPTION and HTSIZE. (same as in OnNcHitTest)

	virtual void DerivedHitProcessing(LRESULT nHitResult, UINT nFlags, CPoint& point);
	// --- In  : nHitResult : one of the codes the base class couldn´t interpret
	//			 nFlags : key modifiers used
	//			 point : exact hit point
	// --- Out : 
	// --- Returns :
	// --- Effect : Called from base class COXRollup if it encounters a hit result it
	//				can´t interpret (if you have added other hot areas in your titlebar)

	void SetRollupState(int nState); 
	// --- In  : nState : Either SCROLL_UP or SCROLL_DOWN (will be asserted!)
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets the Rollup „Button" to either scroll up or down bitmap

	BOOL IsRolledUp();
	// --- In  : 
	// --- Out : 
	// --- Returns : whether the COXRollup is rolled up or down
	// --- Effect : 

	void SetSysMenuOpen(BOOL bOpen);
	// --- In  : bOPen : Open or close system menu (light/dark grayed)
	// --- Out : 
	// --- Returns : 
	// --- Effect : Sets the Rollup sysmenu to either enabled or disabled

	BOOL IsDragging();
	// --- In  : 
	// --- Out : 
	// --- Returns : whether the COXRollup is being dragged or not
	// --- Effect : 

	virtual ~COXTitleBar();
	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : destructor of the object

protected:
	
	void	InvertTracker(CPoint); 
	void	StartDragging();
	void	EndDragging();

	virtual WNDPROC* GetSuperWndProcAddr();	

	// Generated message map functions
	//{{AFX_MSG(CTitleBar)
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk( UINT nFlags, CPoint point );
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

};

/////////////////////////////////////////////////////////////////////////////

inline BOOL COXTitleBar::IsDragging()
	{	return m_bDragging; } 

inline void COXTitleBar::StartDragging()
	{	m_bDragging = TRUE; } 

inline void COXTitleBar::EndDragging()
	{	m_bDragging = FALSE; } 

inline int COXTitleBar::ActiveStyle()
	{	return m_nActivation; }

inline void COXTitleBar::DerivedHitProcessing(LRESULT /* nHitResult */, UINT /* nFlags */, CPoint& /* point */)
	{	TRACE0("DerivedHitProcessing;no implementation!\n"); }

inline BOOL COXTitleBar::IsRolledUp()
	{	return (SCROLL_DOWN == m_nRollupState); }

#endif
//===============================================================================

