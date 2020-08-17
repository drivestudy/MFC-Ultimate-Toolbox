// ==========================================================================
//			Class Specification: COXCustomizeInstanceManagerPage
// ==========================================================================

// Header file : OXCustomizeInstanceManagerPage.h

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

COXCustomizeInstanceManagerPage is COXCustomizePage-derived class. It implements one of the 
predefined (standard) customize pages that is responsible for customizing Instance 
Manager settings. Instance manager allows you to get information about all running 
instances of the application. You can specify the maximum number of instances running 
simultaneously. Also it allows you to apply some standard operations to the currently
running instances ("Close", "Switch to...", "Bring To Front", "Maximize", "Minimize",
"Restore").

This standard page will be automatically inserted in the customize manager if you
add the following define to your stdafx.h file:

	#define OX_CUSTOMIZE_INSTANCE_MANAGER

In order to initialize the page you must call the following function:

	virtual BOOL InitializeInstanceManager(
		int nMaxAllowedInstances=OX_MAX_NUM_INSTANCES_IN_LIST,
		LPCTSTR lpszProfileName=_T("CustomizeInstanceManager")) 

Internally we use COXInstanceManager object to handle all issues related to 
the functionality of this customize page. Refere to the documentation on this class
for more details.

!!!!!
In order to use Instance Manager in your application you have to call the 
COXCustomizeManager::CheckMaxAllowedInstances() function in OnCreate() handler 
of the main window of the application before creating it. In the case this
function returns FALSE you have to return -1 from OnCreate() right away; 
otherwise you can go on with creation of the main window of the application.

	int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		. . . . . . . . . . . . . . . . . . . . . . . . . .
		if(m_customizeManager.CheckMaxAllowedInstances())
		{
			TRACE(_T("CMainFrame::OnCreate: the maximum allowed number of instances has been reached. The application won't be loaded\n"));
			return -1;
		}
		. . . . . . . . . . . . . . . . . . . . . . . . . .
	}
!!!!!

This class implements the set of virtual functions defined in the COXCustomizePage 
class that must be implemented by any page.




The best way of learning about Customize manager capabilities is to take look at the 
VisualStudioLikeDemo sample that can be found in the 
.\Samples\Advanced\VisualStudioLikeDemo subdirectory of your Ultimate Toolbox 
directory. In the sample menu choose "View"-"Customize" in order to display the 
Customize manager window. 



Dependency:
	#include "OXCustomizeInstanceManagerPage.h"


Source code files:
	"OXCustomizeInstanceManagerPage.cpp"		
	"OXCustomizePage.cpp"		
	"OXLayoutManager.cpp"		
	"OXSeparator.cpp"		
	"OXStatic.cpp"
	"OXInstanceManager.cpp"
	

Resource files:
	"OXCustomizeManager.rc"		


*/

#if !defined(_OXCUSTOMIZEINSTANCEMANAGERPAGE_H__)
#define _OXCUSTOMIZEINSTANCEMANAGERPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"

#include "OXCustomizePage.h"
#include "OXSeparator.h"
#include "OXStatic.h"
#include "OXInstanceManager.h"
#include "OXMainRes.h"


/////////////////////////////////////////////////////////////////////////////
// COXCustomizeInstanceManagerPage dialog

class OX_CLASS_DECL COXCustomizeInstanceManagerPage : public COXCustomizePage
{
	DECLARE_DYNCREATE(COXCustomizeInstanceManagerPage)
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXCustomizeInstanceManagerPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(COXCustomizeInstanceManagerPage)
	enum { IDD = IDD_OX_PAGE_INSTANCE_MANAGER };
	CSpinButtonCtrl	m_spinMaxAllowedInstances;
	CListCtrl	m_listInstances;
	COXStatic	m_staticInstanceCount;
	COXSeparator	m_sepOptions;
	COXSeparator	m_sepOperations;
	COXSeparator	m_sepInstances;
	int		m_nInstanceLimitType;
	UINT	m_nMaxAllowedInstanceNumber;
	//}}AFX_DATA

protected:
	// instance manger object
	COXInstanceManager m_instanceManager;
	// timer for updating the list of currently active instances of the application
	UINT_PTR m_nTimerCheckInstancesID;

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
	virtual BOOL IsSupportingApplyChanges() { return TRUE; }

	// --- In  :	
	// --- Out : 
	// --- Returns: TRUE if changes were applied successfully; FALSE otherwise
	// --- Effect : Applies the changes. Called internally by customize manager when 
	//				"Apply" button is pressed.
	virtual BOOL ApplyChanges();

	
	// --- In  :	
	// --- Out : 
	// --- Returns: The title of the page as it appears in customize manager's 
	//				shortcut bar
	// --- Effect : Retrieves the title of the page. This string will appear on the 
	//				corresponding item in the customize manager's shortcut bar
	virtual CString GetTitle() const { 
		CString sRet;
		VERIFY(sRet.LoadString(IDS_OX_CSTMZEINSTANCETITLE));
		return sRet;
		/*_T("Instance Manager"); */
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
		return MAKEINTRESOURCE(IDB_OX_CUSTOMIZE_INSTANCE_MANAGER); 
	}

	// --- In  :	
	// --- Out : 
	// --- Returns: The tooltip for the page item as it appears in customize manager's 
	//				shortcut bar
	// --- Effect : Retrieves the tooltip for the page item. 
	virtual CString GetTooltip() const 
	{ 
		CString sRet;
		VERIFY(sRet.LoadString(IDS_OX_CSTMZEINSTANCETOOLTIP));
		return sRet;
		// _T("Instance Manager allows you to limit the number of running instances"); 
	}


	// --- In  :	bIsOk	-	TRUE if the customize manager window is being closed 
	//							as a result of pressing "OK" button; FALSE otherwise
	// --- Out : 
	// --- Returns:	TRUE if the page was successfully closed; FALSE otherwise
	// --- Effect : Called by customize manager to close the page. Advanced 
	//				overridable, use your own implementation in the case you 
	//				need to handle the close event.
	virtual BOOL OnCloseManager(BOOL bIsOk);


	// --- In  :	nMaxAllowedInstances	-	the maximum number of instances
	//											of the application allowed to run 
	//											simultaneously (-1 means there is 
	//											no limit)
	//				lpszProfileName			-	the name of the hive in the registry 
	//											where instance manager settings is 
	//											saved. If non-NULL value is set then 
	//											settings is retrieved from the 
	//											registry right away. These can 
	//											overwrite the value specified in 
	//											the previous parameter
	// --- Out : 
	// --- Returns: TRUE if instance manager was initialized successfully; 
	//				FALSE otherwise
	// --- Effect : Initializes and retrieve settings for instance manager object
	//				that can be used in order to access instances of the same 
	//				application running simultaneously
	virtual BOOL InitializeInstanceManager(
		int nMaxAllowedInstances=OX_MAX_NUM_INSTANCES_IN_LIST,
		LPCTSTR lpszProfileName=_T("CustomizeInstanceManager"));


	// --- In  :	
	// --- Out : 
	// --- Returns: A pointer to internal COXInstanceManager object
	// --- Effect : Retrieves internal COXInstanceManager object
	virtual COXInstanceManager* GetInstanceManager() { return &m_instanceManager; }


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXCustomizeInstanceManagerPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXCustomizeInstanceManagerPage();   // destructor

protected:
	// --- In  :	
	// --- Out : 
	// --- Returns:	
	// --- Effect : Called internally just after the page window was created. 
	//				Override this function in order to initialize the controls 
	//				on the page. Don't forget to call the default implementation 
	//				unless you duplicate it in your code
	virtual void OnInitDialog();

	// populates list control with info on currently running instances of the 
	// application
	virtual void PopulateInstancesList();

	// displays the page controls
	void ShowVars();

	// retrieves current instance name
	static CString GetCurrentInstanceName();

	// Generated message map functions
	//{{AFX_MSG(COXCustomizeInstanceManagerPage)
	afx_msg void OnRadioMaxnumberInstanceLimitto();
	afx_msg void OnRadioMaxnumberInstanceNolimit();
	afx_msg void OnRadioMaxnumberInstanceOnlyone();
	afx_msg void OnChangeEditLimitNumber();
	afx_msg void OnItemchangedListInstances(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnButtonBringToFront();
	afx_msg void OnButtonClose();
	afx_msg void OnButtonMaximize();
	afx_msg void OnButtonMinimize();
	afx_msg void OnButtonRestore();
	afx_msg void OnButtonSwitchTo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_OXCUSTOMIZEINSTANCEMANAGERPAGE_H__)
