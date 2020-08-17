// ==========================================================================
// 					Class Specification : 
//						COXTreeHeader
// ==========================================================================

// Header file : COXTreeHeader.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
#ifndef __OXTREE_HEADER_H
#define __OXTREE_HEADER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXUxTheme.h"

/////////////////////////////////////////////////////////////////////////////
// COXTreeHeader window


class OX_CLASS_DECL COXTreeHeader : public CHeaderCtrl
{
// Construction
public:
	COXTreeHeader();
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : constructor

// Attributes
public:

protected:
	int m_nSortCol;
	int m_nSortOrder;

	BOOL m_bLButtonDown;
// Operations
public:
	inline int GetSortCol() { return m_nSortCol; }
	// --- In  :
	// --- Out : 
	// --- Returns: number of column the tree control is sorted on
	// --- Effect : 
	inline int GetSortOrder() { return m_nSortOrder; }
	// --- In  :
	// --- Out : 
	// --- Returns: sorting order:	1	-	ascending
	//								0	-	none
	//								-1	-	descending
	// --- Effect : 

	BOOL SortColumn(int nCol, int nSortOrder);
	// --- In  :	nCol		-	number of column to sort on
	// --- Out :	nSortOrder	-	sorting order:	1	-	ascending
	//												0	-	none
	//												-1	-	descending
	// --- Returns: TRUE if success, or FALSE otherwise
	// --- Effect : 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXTreeHeader)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXTreeHeader();
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : destructor

protected:
	virtual void  DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	virtual void PreSubclassWindow();
	DECLARE_DYNAMIC(COXTreeHeader)

	// Generated message map functions
protected:
	COXUxTheme m_UxTheme;
	//{{AFX_MSG(COXTreeHeader)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
	static HHOOK m_hMouseHook;
	static HWND m_hwndPrevMouseMoveWnd;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////
#endif
