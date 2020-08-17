// ==========================================================================
// 					Class Specification : COXContextHelpDialog
// ==========================================================================

// Header file : OXContextHelpDialog.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// Properties:
//		YES		 Abstract class (does not have any objects)
//		YES		 Derived from CDialog
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
//		This class enables the dialog classes derived from it, to have a small question
//		mark in the upper right-hand corner of their caption bar.  Context sensitive help
//		on every child control as well as toopltips for every child control are supported.
//
// Remark:

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXCONTEXTHELPDIALOG_H__
#define __OXCONTEXTHELPDIALOG_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "UTB64Bit.h"


class OX_CLASS_DECL COXContextHelpDialog : public CDialog
{
	DECLARE_DYNAMIC(COXContextHelpDialog)
// Data members -------------------------------------------------------------
public:
	CToolTipCtrl m_Tooltip ;

protected:
	BOOL		m_bTooltipActive;
private:
	
// Member functions ---------------------------------------------------------
public:
	COXContextHelpDialog();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object
	
	COXContextHelpDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	// --- In  : nIDTemplate : Resource ID of the dialog template
	//			 pParentWnd : pointer to the parent window
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object

	COXContextHelpDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	// --- In  : lpszTemplateName : Resource name of the dialog template
	//			 pParentWnd : pointer to the parent window
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object
	
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
	// --- Effect : activates or deactivates the tootips for this dialog

	BOOL GetTooltipActive();
	// --- In  :
	// --- Out : 
	// --- Returns : Whether tootips are activated for this dialog or not 
	// --- Effect : 

	virtual ~COXContextHelpDialog();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object

	//{{AFX_VIRTUAL(COXContextHelpDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	virtual const DWORD* GetHelpIDs() = 0;

	virtual BOOL AdjustToolTips();
	virtual BOOL SetupToolTips();

	//{{AFX_MSG(COXContextHelpDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	afx_msg LONG OnHelp(UINT_PTR wParam, LONG_PTR lParam);
	afx_msg LONG OnHelpContextMenu(UINT_PTR wParam, LONG_PTR lParam);
	
	DECLARE_MESSAGE_MAP()

private:

};

#endif
