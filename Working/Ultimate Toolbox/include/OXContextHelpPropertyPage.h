// ==========================================================================
// 					Class Specification : COXContextHelpPropertyPage
// ==========================================================================

// Header file : OXContextHelpPropertyPage.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// Properties:
//		YES		 Abstract class (does not have any objects)
//		YES		 Derived from CPropertyPage
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
//		This class enables the PropertyPage classes derived from it, to have Context sensitive help
//		on every child control as well as toopltips for every child control.
//
// Remark:

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXCONTEXTHELPPROPPAGE_H__
#define __OXCONTEXTHELPPROPPAGE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


class OX_CLASS_DECL COXContextHelpPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(COXContextHelpPropertyPage)
// Data members -------------------------------------------------------------
public:
	CToolTipCtrl m_Tooltip ;

protected:
	BOOL m_bTooltipActive;
private:
	
// Member functions ---------------------------------------------------------
public:
	COXContextHelpPropertyPage();

	COXContextHelpPropertyPage(UINT nIDTemplate, UINT nIDCaption = 0);
	// --- In  : nIDTemplate : Resource ID of the dialog template
	//			 nIDCaption : ID of the name to be placed in the tab for this page.
	//						  If 0, the name will be taken from the dialog template for this page
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object

	COXContextHelpPropertyPage(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);
	// --- In  : lpszTemplateName : Points to a string containing the name of
	//								the template for this page. Cannot be NULL.
	//			 nIDCaption : ID of the name to be placed in the tab for this page.
	//						  If 0, the name will be taken from the dialog template for this page
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object

	void Construct(UINT nIDTemplate, UINT nIDCaption = 0);
	void Construct(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);

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
	// --- Effect : activates or deactivates the tootips for this property page

	BOOL GetTooltipActive();
	// --- In  :
	// --- Out : 
	// --- Returns : Whether tootips are activated for this property page or not 
	// --- Effect : 

	virtual ~COXContextHelpPropertyPage() ;
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
	//{{AFX_VIRTUAL(COXContextHelpPropertyPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COXContextHelpPropertyPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	afx_msg LONG OnHelp(UINT wParam, LONG lParam);
	afx_msg LONG OnHelpContextMenu(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()

private:

};

#endif
