#if !defined(AFX_OXSKINNEDTABCTRL_H__3F035654_296A_4612_B183_025642EEA95E__INCLUDED_)
#define AFX_OXSKINNEDTABCTRL_H__3F035654_296A_4612_B183_025642EEA95E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OXSkinnedTabCtrl.h : header file
//
// Version: 9.3

#include "OXDllExt.h"

#include "OXSkinnedSpinCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// COXSkinnedTabCtrl window

class COXTabSkin;

class OX_CLASS_DECL COXSkinnedTabCtrl : public CTabCtrl
{
	friend class COXTabSkinXP;
	friend class COXTabSkinClassic;

// Construction
public:
	DECLARE_DYNAMIC(COXSkinnedTabCtrl)
	COXSkinnedTabCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXSkinnedTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXSkinnedTabCtrl();

	// Generated message map functions
protected:
	COXTabSkin* m_pTabSkin;
	COXTabSkin* GetTabSkin();

	//{{AFX_MSG(COXSkinnedTabCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	COXSkinnedSpinCtrl m_updown;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OXSKINNEDTABCTRL_H__3F035654_296A_4612_B183_025642EEA95E__INCLUDED_)
