// ==========================================================================
// 					Class Specification : COXDockTabCtrl
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OXDOCKTABCTRL_H__D22FB90C_7531_4161_83D6_697ED0C33F7B__INCLUDED_)
#define AFX_OXDOCKTABCTRL_H__D22FB90C_7531_4161_83D6_697ED0C33F7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OXDockTabCtrl.h : header file
//

#include "OXDllExt.h"

#include "OXSkinnedTabCtrl.h"
#include "OXSizeCtrlBar.h"	// Added by ClassView


/////////////////////////////////////////////////////////////////////////////
// COXDockTabCtrl window

class OX_CLASS_DECL COXDockTabCtrl : public COXSkinnedTabCtrl
{
	friend class COXWorkspaceState;
	friend class COXSizeDockBar;
	friend class COXSizeControlBar;

// Construction
public:
	COXDockTabCtrl(COXSizeDockBar* pSizeDockBar);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXDockTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXDockTabCtrl();

	// Generated message map functions
protected:
	CControlBar* GetBar(int iIndex);
	void InsertTab(CControlBar* pBar, int iIndex, BOOL bShowSelectedTab = TRUE);
	void RemoveTab(CControlBar* pBar);
	int FindTab(CControlBar* pBar);
	void ShowSelectedTab();
	int HitTestTabControl(CPoint point, CControlBar* pBar);
	void RepositionTabs(int iOldIndex, int iNewIndex, CPoint point);

	//{{AFX_MSG(COXDockTabCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CFont m_font;
	COXSizeControlBar* m_pLastSelectedBar;
	COXSizeDockBar* m_pSizeDockBar;
	CPoint m_ptLButtonDown;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OXDOCKTABCTRL_H__D22FB90C_7531_4161_83D6_697ED0C33F7B__INCLUDED_)
