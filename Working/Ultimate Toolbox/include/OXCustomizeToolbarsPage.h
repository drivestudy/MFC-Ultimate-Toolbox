// ==========================================================================
//			Class Specification: COXCustomizeToolbarsPage
// ==========================================================================

// Header file : OXCustomizeToolbarsPage.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////

/*
	Overview

COXCustomizeToolbarsPage is COXCustomizePage-derived class. It implements one of the 
predefined (standard) customize pages that is responsible for customizing of the 
appearance of the application toolbars. It also provides a functionality of creating
new toolbars, deleting existing ones and hiding.showing and renaming them. 
Also using this customize page you can reset the contents of the toolbar(s) to match 
the default settings.

Note, that some of the settings can be applied only if you use COXCoolToolBar derived 
toolbars in your application.

This standard page will be automatically inserted in the customize manager if you
add the following define to your stdafx.h file:

	#define OX_CUSTOMIZE_TOOLBARS

In order to initialize the page you must call the following function:

	virtual BOOL InitializeToolbars(BOOL bCoolLook=TRUE, BOOL bShowGripper=TRUE,
									BOOL bShowTooltips=TRUE, int nBorderSize=0, 
									LPCTSTR lpszProfileName=_T("CustomizeToolbars"));

This class implements the set of virtual functions defined in the COXCustomizePage 
class that must be implemented by any page.




The best way of learning about Customize manager capabilities is to take look at the 
VisualStudioLikeDemo sample that can be found in the 
.\Samples\Advanced\VisualStudioLikeDemo subdirectory of your Ultimate Toolbox 
directory. In the sample menu choose "View"-"Customize" in order to display the 
Customize manager window. 



Dependency:
	#include "OXCustomizeToolbarsPage.h"


Source code files:
	"OXCustomizeToolbarsPage.cpp"		
	"OXCustomizePage.cpp"		
	"OXLayoutManager.cpp"		
	"OXSeparator.cpp"		
	"OXOptionTreeCtrl.cpp"
	"OXCoolToolBar.cpp"
	"OXMenuBar.cpp"
	

Resource files:
	"OXCustomizeManager.rc"		
	"OXOptionTreeCtrl.rc"		


*/

#if !defined(_OXCUSTOMIZETOOLBARSPAGE_H__)
#define _OXCUSTOMIZETOOLBARSPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#define __AFXTEMPL_H__
#endif

#include "OXCustomizePage.h"
#include "OXOptionTreeCtrl.h"
#include "OXSeparator.h"
#include "OXMenuBar.h"
#include "OXMainRes.h"

#include "MFCToolBar.h"

#define CToolBar CMFCToolBar

typedef struct _tagOXCUSTOMIZE_TOOLBARINFO
{
	// pointer to a toolbar object
	CToolBar* m_pToolBar;
	// flag that specifies if this toolbar was created manually (and therefore 
	// can be deleted)
	BOOL m_bCustomMade;

	// default constructor
	_tagOXCUSTOMIZE_TOOLBARINFO(CToolBar* pToolBar=NULL, BOOL bCustomMade=FALSE)
	{
		m_pToolBar=pToolBar;
		m_bCustomMade=bCustomMade;
	}

	// assignment operator
	_tagOXCUSTOMIZE_TOOLBARINFO operator=(const _tagOXCUSTOMIZE_TOOLBARINFO& info)
	{
		if(&info==this)
			return *this;
		
		m_pToolBar=info.m_pToolBar;
		m_bCustomMade=info.m_bCustomMade;

		return *this;
	}

} OXCUSTOMIZE_TOOLBARINFO;

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeToolbarsPage dialog

class OX_CLASS_DECL COXCustomizeToolbarsPage : public COXCustomizePage
{
	DECLARE_DYNCREATE(COXCustomizeToolbarsPage)
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXCustomizeToolbarsPage();

// Dialog Data
	//{{AFX_DATA(COXCustomizeToolbarsPage)
	enum { IDD = IDD_OX_PAGE_TOOLBARS };
	COXOptionTreeCtrl	m_treeToolbars;
	CSpinButtonCtrl	m_spinBorderSize;
	COXSeparator	m_sepToolbars;
	COXSeparator	m_sepOptions;
	COXSeparator	m_sepOperations;
	BOOL	m_bCoolLook;
	BOOL	m_bShowTooltips;
	DWORD	m_nBorderSize;
	BOOL	m_bShowGripper;
	//}}AFX_DATA


protected:
	// array of all custom made toolbars (they can be deleted)
	CArray<COXCoolToolBar*,COXCoolToolBar*> m_arrCustomMadeToolbars;
	// array of all application's toolbars
	CArray<OXCUSTOMIZE_TOOLBARINFO,OXCUSTOMIZE_TOOLBARINFO&> m_arrToolbars;
	// map of all toolbars that have associated info
	CMap<CToolBar*,CToolBar*,int,int> m_mapAssociatedToolbars;

public:
	// --- In  :	pCustomizeManager	-	a pointer to COXCustomizeManager object
	//										that contains this page and is about to 
	//										load it
	// --- Out : 
	// --- Returns:	TRUE if page was successfully loaded; FALSE otherwise
	// --- Effect : Called by customize manager while activating the page. 
	//				This function might be called multiple times. Before 
	//				activating another page customize manager will call
	//				Unload() function
	virtual BOOL Load(const COXCustomizeManager* pCustomizeManager);

	// --- In  :	
	// --- Out : 
	// --- Returns:	
	// --- Effect : Called by customize manager when currently active page is about 
	//				to be deactivated. Matches Load() function call
	virtual void Unload();
	
	
	// --- In  :	
	// --- Out : 
	// --- Returns: TRUE if changes can be applied immediately; FALSE otherwise
	// --- Effect : Retrieves the flag that specifies that changes in toolbar 
	//				appearance settingd can be applied immediately. 
	//				Called internally by customize manager in order to enable/disable
	//				the "Apply" button.
	virtual BOOL IsSupportingApplyChanges() { return FALSE; }


	// --- In  :	
	// --- Out : 
	// --- Returns: The title of the page as it appears in customize manager's 
	//				shortcut bar
	// --- Effect : Retrieves the title of the page. This string will appear on the 
	//				corresponding item in the customize manager's shortcut bar
	virtual CString GetTitle() const {
		CString sRet;
		VERIFY(sRet.LoadString(IDS_OX_CSTMZETOOLBARTITLE));
		return sRet;
		//_T("Toolbars"); 
	}

	// --- In  :	
	// --- Out : 
	// --- Returns: The resource string of the image of the page as it appears in 
	//				customize manager's shortcut bar
	// --- Effect : Retrieves the resource string of the image of the page. 
	//				This image will appear on the corresponding item in the customize 
	//				manager's shortcut bar
	virtual LPCTSTR GetImageResource() const 
	{ 
		return MAKEINTRESOURCE(IDB_OX_CUSTOMIZE_TOOLBARS); 
	}

	// --- In  :	
	// --- Out : 
	// --- Returns: The tooltip for the page item as it appears in customize manager's 
	//				shortcut bar
	// --- Effect : Retrieves the tooltip for the page item. 
	virtual CString GetTooltip() const {
		CString sRet;
		VERIFY(sRet.LoadString(IDS_OX_CSTMZETOOLBARTOOLTIP));
		return sRet;
		//_T("Customize the appearance of application's toolbars"); 
	}


	// --- In  :	bIsOk	-	TRUE if the customize manager window is being closed 
	//							as a result of pressing "OK" button; FALSE otherwise
	// --- Out : 
	// --- Returns:	TRUE if the page was successfully closed; FALSE otherwise
	// --- Effect : Called by customize manager to close the page. Advanced 
	//				overridable, use your own implementation in the case you 
	//				need to handle the close event.
	virtual BOOL OnCloseManager(BOOL bIsOk);


	// --- In  :	bCoolLook		-	if TRUE the toolbars will be displayed in
	//									cool mode (flat buttons). By default set 
	//									to TRUE
	//				bShowGripper	-	if TRUE a gripper will be displayed on the
	//									top/left side of application's toolbars. 
	//									By default set to TRUE
	//				bShowTooltips	-	if TRUE a tooltip will be displayed for
	//									toolbars buttons. By default set to TRUE
	//				nBorderSize		-	the border size in pixels for toolbars. 
	//									By default set to 0
	//				lpszProfileName	-	the name of the hive in the registry where 
	//									toolbars' appearance settings is saved. If
	//									non-NULL value is set then settings is 
	//									retrieved from the registry right away. 
	//									These can overwrite the values specified in 
	//									the previous parameters
	// --- Out : 
	// --- Returns: TRUE if toolbars were initialized successfully; FALSE otherwise
	// --- Effect : Initializes the settings that specifies the appearance of the 
	//				application's toolbars
	virtual BOOL InitializeToolbars(BOOL bCoolLook=TRUE, BOOL bShowGripper=TRUE,
		BOOL bShowTooltips=TRUE, int nBorderSize=0, 
		LPCTSTR lpszProfileName=_T("CustomizeToolbars"));

protected:
	// retrives the current state of the internal variables
	void GetVars();
	// show the page controls
	void UpdateState();

	// build a unique registry key name for a toolbar
	virtual CString BuildToolbarKey(int nIndex) const 
	{
		CString sKey;
		sKey.Format(_T("%d~!@#$CustomMadeToolbar"),nIndex);
		return sKey;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXCustomizeToolbarsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXCustomizeToolbarsPage();   // destructor

protected:
	// --- In  :	
	// --- Out : 
	// --- Returns:	
	// --- Effect : Called internally just after the page window was created. 
	//				Override this function in order to initialize the controls 
	//				on the page. Don't forget to call the default implementation 
	//				unless you duplicate it in your code
	virtual void OnInitDialog();

protected:
	// Generated message map functions
	//{{AFX_MSG(COXCustomizeToolbarsPage)
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonRename();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonResetAll();
	afx_msg void OnCheckCool();
	afx_msg void OnCheckTooltips();
	afx_msg void OnChangeEditBorderSize();
	afx_msg void OnEndlabeleditTreeToolbars(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOptionChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckGripper();
	afx_msg void OnBeginlabeleditTreeToolbars(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreeToolbars(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_OXCUSTOMIZETOOLBARSPAGE_H__)
