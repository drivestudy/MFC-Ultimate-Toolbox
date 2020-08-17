// ==========================================================================
//			Class Specification: COXCustomizePage
// ==========================================================================

// Header file : OXCustomizePage.h

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

COXCustomizePage is the base CWnd-derived class of any customize page. It defines the 
way interface of a page. Using this interface Customize manager 
(see COXCustomizeManager class documentation for details) can retrieve informaton on 
how this page will be displayed in the Customize manager shortcut bar control. Also
this interface defines a way of communication between the manager and customize pages
it includes.

COXCustomizePage interface is defined as a set of virtual functions that can be 
overridden in a derived class (and in some cases they must be overridden in order to
get a consumable results). The default implementation of these functions don't do 
a lot and, generally speaking, COXCustomizePage can be considered as a template 
for implementing any specific customize page. 

A customize page is usually responsible for customization of a particular aspect of 
an application and doesn't depend on other customize pages. It is always displayed 
as a child window of the customize manager window and it should support resizing 
functionality as long as customize manager is resizable. Customize page is created on 
the base of dialog template that is associated with it. COXCustomizePage includes
the following function that creates the page window:

		virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd, 
							CRect rect,	UINT nID, DWORD dwStyle=WS_VISIBLE|WS_CHILD);




  Usage

You will use COXCustomizePage in the case you need to create your own customize page.
In order to do that follow these steps:

1)	In the derived class implement the following virtual functions that define the 
	location of the page in the customize manager shortcut bar:

		virtual CString GetTitle() const;
		virtual LPCTSTR GetImageResource() const;
		virtual COLORREF GetImageMask() const;
		virtual CString GetTooltip() const;
		virtual CString GetGroup() const;


2)	Implement these functions in order to support "Apply" button in Customize manager:

		virtual BOOL ApplyChanges();
		virtual BOOL IsSupportingApplyChanges();


3)	Override this function:

		virtual void OnInitDialog();

	in order to initialize internal controls once right after the customize page 
	window was created (the same like for CDialog derived classes)


4)	Override the following two functions in order to handle the activation and
	deactivation events (Customize manager displays only one customize page at a 
	time, such page is called active):

		virtual BOOL Load(const COXCustomizeManager* pCustomizeManager);
		virtual void Unload();


5)	And, finally, if you need to save any settings defined in the page that has been 
	changed you must implement the following function:

		virtual BOOL SaveWorkspaceState();



These are the steps that you should take in order to implement any custom made 
customize page. 


Customize Manager Framework wouldn't be such a valuable part of the library if it 
didn't include some predefined customize pages (we call them standard customize 
pages) with implemented customization functionality. And we have plenty of them.
In order to include a standard page in customize manager you only have to add a 
corresponding define to your stdafx.h file. Below you will find brief description
of the standard pages and the name of the corresponding defines that should be used 
in order to include them in the customize manager. You have to refer to corresponding
classes documentation for the details on a particular customize page classes. This
information will only give you an overview of the available standard customize pages.

  
1)	Customize Toolbars
	
	Allows you to customize the appearance of the toolbars in the application. Also 
	you can add new toolbars, delete existing ones or rename them. 
	
	COXCustomizeToolbarsPage class implements this page. 

	The following define must be put in your stdafx.h file in order to include the 
	page in the customize manager
		#define OX_CUSTOMIZE_TOOLBARS

	The following function defined in COXCustomizeManager class must be called 
	from WM_CREATE handler of the application main window where customize manager 
	object is defined. It initializes toolbars page settings

		BOOL InitializeToolbars(BOOL bCoolLook=TRUE, BOOL bShowGripper=TRUE,
								BOOL bShowTooltips=TRUE, int nBorderSize=0, 
								LPCTSTR lpszProfileName=_T("CustomizeToolbars"));


2)	Customize Commands
	
	Allows you to customize the contents of the toolbars and menus in the application. 
	The apparance of the menu items and toolbar buttons can be changed as well.
	The updated contents can be saved to registry. Next time your application runs 
	the saved contents will be applied. 
	
	COXCustomizeCommandsPage class implements this page. 

	The following define must be put in your stdafx.h file in order to include the 
	page in the customize manager
		#define OX_CUSTOMIZE_COMMANDS

	The following function defined in COXCustomizeManager class must be called 
	from WM_CREATE handler of the application main window where customize manager 
	object is defined. It initializes commands set and applies the toolbars and menu 
	saved data

		BOOL InitializeCommands(LPCTSTR lpszCustomImagesResource,
								COLORREF clrMask=RGB(192,192,192), 
								BOOL bShowIconsInMenu=TRUE,
								LPCTSTR lpszProfileName=_T("CustomizeCommands")); 





The best way of learning about Customize manager capabilities is to take look at the 
VisualStudioLikeDemo sample that can be found in the 
.\Samples\Advanced\VisualStudioLikeDemo subdirectory of your Ultimate Toolbox 
directory. In the sample menu choose "View"-"Customize" in order to display the 
Customize manager window. 




Dependency:
	#include "OXCustomizePage.h"


Source code files:
	"OXCustomizePage.cpp"		
	"OXLayoutManager.cpp"		

*/

#if !defined(_OXCUSTOMIZEPAGE_H__)
#define _OXCUSTOMIZEPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"

#include "OXLayoutManager.h"
#include "OXMainRes.h"


#ifndef DEFAULT_CUSTOMIZE_GROUP
#define DEFAULT_CUSTOMIZE_GROUP		_T("Customize")
#endif	//	DEFAULT_CUSTOMIZE_GROUP


class COXCustomizeManager;

/////////////////////////////////////////////////////////////////////////////
// helper class

class COXHelperWnd : public CWnd
{
public: COXHelperWnd(){}
protected:
	virtual void PostNCDestroy()
	{ 
		delete this;
	}
};

/////////////////////////////////////////////////////////////////////////////
// COXCustomizePage window

class OX_CLASS_DECL COXCustomizePage : public CWnd
{
	DECLARE_DYNCREATE(COXCustomizePage)
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXCustomizePage();

// Attributes
public:

protected:
	// pointer to the owner
	COXCustomizeManager* m_pCustomizeManager;
	// dialog resource ID for the page
	UINT m_nDialogID;
	// layout manager object that can be used in order to resize child controls
	// on the page whenever size of the page is changed
	COXLayoutManager m_LayoutManager;

	// name of the profile name or registry hive where the page's state info 
	// might be saved
	CString m_sProfileName;
	// flag that is set when a page is fully initialized
	BOOL m_bInitialized;

// Operations
public:
	// --- In  :	lpszTemplateName	-	contains a null-terminated string that is 
	//										the name of a page template resource
	//				pParentWnd			-	points to the parent window object (of 
	//										type CWnd) to which the page object 
	//										belongs. If it is NULL, the page object’s 
	//										parent window is set to the main 
	//										application window
	//				rect				-	the size and position of the page, 
	//										in client coordinates of pParentWnd.
	//				nID					-	the ID of the child window
	//				dwStyle				-	specifies the window style attributes
	// --- Out : 
	// --- Returns:	TRUE if successful; FALSE otherwise
	// --- Effect : Create page window from specified template.
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd, 
		CRect rect,	UINT nID, DWORD dwStyle=WS_VISIBLE|WS_CHILD);


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



	// --- In  :	bIsOk	-	TRUE if the customize manager window is being closed 
	//							as a result of pressing "OK" button; FALSE otherwise
	// --- Out : 
	// --- Returns:	TRUE if the page was successfully closed; FALSE otherwise
	// --- Effect : Called by customize manager to close the page. Advanced 
	//				overridable, use your own implementation in the case you 
	//				need to handle the close event.
	virtual BOOL OnCloseManager(BOOL bIsOk) 
	{ 
		UNREFERENCED_PARAMETER(bIsOk); 
		return TRUE;
	}


	// --- In  :	
	// --- Out : 
	// --- Returns: TRUE if the state of the subject this customize page responsible 
	//				for was saved in the registry successfully; FALSE otherwise
	// --- Effect : Saves in the registry the state of the subject this customize 
	//				page responsible for. Called internally by customize manager.
	virtual BOOL SaveWorkspaceState();


	// --- In  :	
	// --- Out : 
	// --- Returns: TRUE if changes were applied successfully; FALSE otherwise
	// --- Effect : Applies the changes. Called internally by customize manager when 
	//				"Apply" button is pressed.
	virtual BOOL ApplyChanges() { return TRUE; }

	// --- In  :	
	// --- Out : 
	// --- Returns: TRUE if changes can be applied immediately; FALSE otherwise
	// --- Effect : Retrieves the flag that specifies that changes can be applied
	//				immediately to the subject this customize page responsible for. 
	//				Called internally by customize manager in order to enable/disable
	//				the "Apply" button.
	virtual BOOL IsSupportingApplyChanges() { return TRUE; }


	// --- In  :	
	// --- Out : 
	// --- Returns: The title of the page as it appears in customize manager's 
	//				shortcut bar
	// --- Effect : Retrieves the title of the page. This string will appear on the 
	//				corresponding item in the customize manager's shortcut bar
	virtual CString GetTitle() const { return _T(""); }

	// --- In  :	
	// --- Out : 
	// --- Returns: The resource string of the image of the page as it appears in 
	//				customize manager's shortcut bar
	// --- Effect : Retrieves the resource string of the image of the page. 
	//				This image will appear on the corresponding item in the customize 
	//				manager's shortcut bar
	virtual LPCTSTR GetImageResource() const { return NULL; }

	// --- In  :	
	// --- Out : 
	// --- Returns: The mask color for the bitmap that specifies the image of the page
	// --- Effect : Retrieves the mask color for the bitmap that specifies the image 
	//				of the page
	virtual COLORREF GetImageMask() const { return RGB(192,192,192); }

	// --- In  :	
	// --- Out : 
	// --- Returns: The tooltip for the page item as it appears in customize manager's 
	//				shortcut bar
	// --- Effect : Retrieves the tooltip for the page item. 
	virtual CString GetTooltip() const { return _T(""); }

	// --- In  :	
	// --- Out : 
	// --- Returns: The name of the group this page belongs to. 
	// --- Effect : Retrieves the name of the group this page belongs to. 
	virtual CString GetGroup() const { return DEFAULT_CUSTOMIZE_GROUP; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXCustomizePage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, 
		AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXCustomizePage();

protected:
	// --- In  :	
	// --- Out : 
	// --- Returns:	
	// --- Effect : Called internally just after the page window was created. 
	//				Override this function in order to initialize the controls 
	//				on the page. Don't forget to call the default implementation 
	//				unless you duplicate it in your code
	virtual void OnInitDialog() 
	{ 
		UpdateData(FALSE); 	
		m_LayoutManager.Attach(this);
	}

#ifndef _AFX_NO_OCC_SUPPORT
	// data and functions necessary for OLE control containment
	_AFX_OCC_DIALOG_INFO* m_pOccDialogInfo;
	virtual BOOL SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* pOccDialogInfo);
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(COXCustomizePage)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_OXCUSTOMIZEPAGE_H__)
