// ==========================================================================
// 					Class Specification : COXAutoListBox
// ==========================================================================
// Header file : OXAutoListBox.h
// Version: 9.3
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
//
// //////////////////////////////////////////////////////////////////////////
//
// Internal helper class
//
// //////////////////////////////////////////////////////////////////////////

#if !defined(_OXAUTOLISTBOX_H__)
#define _OXAUTOLISTBOX_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "oxdllext.h"

class COXAutoComplete;

#ifndef OX_AUTOCOMPLETE_IDC_LIST
#define OX_AUTOCOMPLETE_IDC_LIST 24510
#endif

/////////////////////////////////////////////////////////////////////////////
// COXAutoListBox window

class OX_CLASS_DECL COXAutoListBox : public CListBox
{ 
// Construction
public:
	COXAutoListBox(COXAutoComplete* pComplete);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXAutoListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetComplete(COXAutoComplete* pComplete);
	COXAutoComplete* m_pComplete;
	UINT m_nHeight;
	UINT m_nWidth;
	BOOL m_bDraw;
	virtual ~COXAutoListBox();

	// Generated message map functions
protected:
	int GetItemFromPoint(POINT pt);
	//{{AFX_MSG(COXAutoListBox)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);	
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_OXAUTOLISTBOX_H__)
  