// ==========================================================================
// 					Class Specification : COXContextHelpPropertySheet
// ==========================================================================

// Header file : OXContextHelpPropertySheet.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// Properties:
//		YES		 Abstract class (does not have any objects)
//		YES		 Derived from CPropertySheet
//
//		YES		 Is a CWnd.                     
//		YES		 Two stage creation (constructor & Create())
//		YES		 Has a message map
//		YES		 Needs a resource (template)
//
//		NO		 Persistent objects (saveable on disk)      
//		NO		 Uses exceptions
//
// Description:
//		This class enables the Propertysheet classes derived from it, to have a small question
//		mark in the upper right-hand corner of their caption bar.  Context sensitive help
//		on every child control as well as toopltips for every child control are supported.
//
// Remark:

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXCONTEXTHELPPROPSHEET_H__
#define __OXCONTEXTHELPPROPSHEET_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


class COXContextHelpPropertyPage;

class OX_CLASS_DECL COXContextHelpPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(COXContextHelpPropertySheet)

// Data members -------------------------------------------------------------
public:
	CToolTipCtrl m_Tooltip;

protected:
	BOOL m_bFirstTime;
	BOOL m_bTooltipActive;
private:
	
// Member functions ---------------------------------------------------------
public:
	COXContextHelpPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	// --- In  : nIDCaption : ID of the caption to be used for the property sheet.
	//			 pParentWnd : Points to the parent window of the property sheet. If NULL, 
	//						  the parent window will be the main window of the application.
	//			 iSelectPage : The index of the page that will initially be on top. Default is the first page added to the sheet.
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object

	COXContextHelpPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	// --- In  : lpszTemplateName : Points to a string containing the caption to be used for the property sheet. Cannot be NULL.
	//			 pParentWnd : Points to the parent window of the property sheet. If NULL, 
	//						  the parent window will be the main window of the application.
	//			 iSelectPage : The index of the page that will initially be on top. Default is the first page added to the sheet.
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object
	
	void AddPage(COXContextHelpPropertyPage* pPage);
	// --- In  : pPage : Points to the page to be added to the property sheet. Cannot be NULL.
	// --- Out : 
	// --- Returns :
	// --- Effect : 

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

	BOOL SetTooltipActive(BOOL bActive = TRUE);
	// --- In  :
	// --- Out : 
	// --- Returns : succeeded or not
	// --- Effect : activates or deactivates the tootips for this property sheet

	BOOL GetTooltipActive();
	// --- In  :
	// --- Out : 
	// --- Returns : Whether tootips are activated for this property sheet or not 
	// --- Effect : 

	virtual ~COXContextHelpPropertySheet();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object

protected:
	virtual const DWORD* GetHelpIDs() = 0;

	virtual BOOL AdjustToolTips();
	virtual BOOL SetupToolTips();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXContextHelpPropertySheet)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(COXContextHelpPropertySheet)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	afx_msg LONG OnHelp(UINT wParam, LONG lParam);
	afx_msg LONG OnHelpContextMenu(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()

private:

};

#endif
