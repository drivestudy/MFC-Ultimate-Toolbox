// ==========================================================================
//		Class Specification:	COXSplitterWnd
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

#ifndef _OXSPLITTERWND_H
#define _OXSPLITTERWND_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


/////////////////////////////////////////////////////////////////////////////
// COXSplitterWnd

class OX_CLASS_DECL COXSplitterWnd : public CSplitterWnd
{
	DECLARE_DYNCREATE(COXSplitterWnd)

public:
	COXSplitterWnd();
	virtual ~COXSplitterWnd();

#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

// Attributes
public:
	inline CSize GetSplitterBarSize() const 
	{ 
		return CSize(m_cxSplitter,m_cySplitter); 
	}

// Operations
public:
	BOOL SwapPanes(CWnd* pFirstWnd, CWnd* pSecondWnd);
	virtual BOOL InsertColumn(int nInsertBefore, int nWidth);
	virtual BOOL RemoveColumn(int nDelCol);
	virtual BOOL InsertRow(int nInsertBefore, int nHeight);
	virtual BOOL RemoveRow(int nDelRow);

	virtual void DeleteView(int row, int col);

protected:
	//{{AFX_MSG(COXSplitterWnd)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////

#endif // _OXSPLITTERWND_H

