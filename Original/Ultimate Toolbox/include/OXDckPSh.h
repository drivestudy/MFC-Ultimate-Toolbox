// ==========================================================================
// 							Class Specification : COXDockPropertySheet
// ==========================================================================

// Header file : OXDckPSh.h

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
//	YES	Derived from COXDialogBar

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	NO  Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	YES	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This class combines the functionality of a tabbed property sheet and a 
//	dockable control bar

// Remark:
//	

// Prerequisites (necessary conditions):
//	

/////////////////////////////////////////////////////////////////////////////
#ifndef __DPROPSHT_H__
#define __DPROPSHT_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

class COXDockPropertyPage;

#include "oxdlgbar.h"

#include "UTB64Bit.h"


class OX_CLASS_DECL COXDockPropertySheet : public COXDialogBar
{
// Data Members

public:
	//{{AFX_DATA(COXDockPropertySheet)
	//}}AFX_DATA

protected:
	CString			m_sCaption;
	CTabCtrl		m_tabCtrl;
	CObArray		m_pageArray;
	CObList			m_DynPageList;

	CRect			m_rectTabArea;
	int				m_nCurrentSelection;

private:

// Member Functions
public:
	COXDockPropertySheet(LPCTSTR pszCaption = NULL);
	// --- In  : pszCaption : title of the property sheet
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object
	//				It will initialize the internal state

	COXDockPropertySheet(UINT nIDCaption);  
	// --- In  : nIDCaption : title-ID of the property sheet
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object
	//				It will initialize the internal state

	BOOL Create(CWnd* pParentWnd, UINT nDlgStyle,
		UINT nTabStyle = WS_CHILD | WS_VISIBLE | TCS_TABS | TCS_SINGLELINE | TCS_RAGGEDRIGHT,
		UINT nID = 1);
	// --- In  : pParentWnd : parent window
	//			 nDlgStyle : styles for the child dialog underneath the tab control
	//			 nTabStyle : styles for the Tab control see CTabControl::Create()
	//			 nID : ID of the child dialog
	// --- Out : 
	// --- Returns : successful or not
	// --- Effect : Creates a dialog from a mem template and a Tab control (child if dialog)

	BOOL AddPage(COXDockPropertyPage* pPage, BOOL bAdjust = FALSE);
	// --- In  : pPage : page to add
	//			 bAdjust : if TRUE all currently added pages will be sized to have the 
	//					   same dimensions as the biggest page.
	// --- Out : 
	// --- Returns : successful or not
	// --- Effect : Adds a page to the tab control, resizes the tab and the control bar if neccessary

	void RemovePage(int nPage);
	// --- In  : nPage : index of page to remove (zero based)
	// --- Out : 
	// --- Returns : 
	// --- Effect : Removes a page from the tab control

	int GetPageCount() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : number of pages added

	COXDockPropertyPage* GetActivePage() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : current active page
	//				NULL if no active page

	int GetActiveIndex() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : index of current active page (zero based)

	int GetPageIndex(COXDockPropertyPage* pPage);
	// --- In  : pPage : pointer to Page
	// --- Out : 
	// --- Returns : index of specified page 
	// --- Effect : searches the tab control to find the index of the specified page
	//				-1 if not found

	BOOL SetActivePage(int nPage);
	// --- In  : nPage : index of new active page
	// --- Out : 
	// --- Returns : successful or not
	// --- Effect : sets a new active page

	int NextPageIndex(int nPage);
	int PrevPageIndex(int nPage);

	void SetTitle(LPCTSTR lpszText);
	// --- In  : lpszText : new sheet title
	// --- Out : 
	// --- Returns : 
	// --- Effect : sets a new sheet title

	CTabCtrl* GetTabControl()
		{ return &m_tabCtrl; }

	COXDockPropertyPage* GetPage(int nPage) const
		{ return (COXDockPropertyPage*)m_pageArray[nPage]; }

	BOOL PreTranslateMessage(MSG* pMsg);

	virtual ~COXDockPropertySheet();  
	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object


protected:
	void Construct();
	BOOL SelectPage(int nNextSel);
	void AdjustAllPages();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXDockPropertySheet)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
#if _MSC_VER < 1400
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
#else
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
#endif
	//}}AFX_VIRTUAL


	// Generated message map functions
	//{{AFX_MSG(COXDockPropertySheet)
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMove(int x, int y);
};

#endif //__DPROPSHT_H__
