// ==========================================================================
//			Class Specification: COXCustomizeShortkeysPage
// ==========================================================================

// Header file : OXCustomizeShortkeysPage.h

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

COXCustomizeShortkeysPage is COXCustomizePage-derived class. It implements one of the 
predefined (standard) customize pages that is responsible for customizing of the 
contents of the application accelerator tables. Your application usually have at least 
one default accelerator table and in the document/view architecture every document 
template usually has its own accelerator table. Using this customize page you can 
associate new accelerators with any commands or delete existing ones. The changed 
accelerator tables can be saved in the registry when application is closed and when it 
is opened next time the last used accelerator tables will be retrieved from the 
registry and applied.

This standard page will be automatically inserted in the customize manager if you
add the following define to your stdafx.h file:

	#define OX_CUSTOMIZE_SHORTKEYS

In order to initialize the page you must call the following function:

	virtual BOOL InitializeShortkeys(CFrameWnd* pFrameWnd, 
									 LPCTSTR lpszProfileName=_T("CustomizeShortkeys"));

This class implements the set of virtual functions defined in the COXCustomizePage 
class that must be implemented by any page.

This class internally uses COXShortkeysOrganizer object for managing accelerators.
You can use this function in order to retrieve a pointer to this object:

	virtual COXShortkeysOrganizer* GetShortkeysOrganizer();


!!! NOTE !!!
Call InitializeShortkeys() function only after the main frame window was successfully
loaded and associated objects were created (like document and view). 
CMainFrame::OnCreate() function doesn't satisfy this requirement. But you can 
call it after CFrameWnd::LoadFrame() has been called (for MDI application) or after
CWinApp::ProcessShellCommand() has been called (for SDI application) in 
InitInstance() of your application CWinApp-derived class


The best way of learning about Customize manager capabilities is to take look at the 
VisualStudioLikeDemo sample that can be found in the 
.\Samples\Advanced\VisualStudioLikeDemo subdirectory of your Ultimate Toolbox 
directory. In the sample menu choose "View"-"Customize" in order to display the 
Customize manager window. 



Dependency:
	#include "OXCustomizeShortkeysPage.h"


Source code files:
	"OXCustomizeShortkeysPage.cpp"		
	"OXCustomizePage.cpp"		
	"OXLayoutManager.cpp"		
	"OXSeparator.cpp"		
	"OXListBoxEx.cpp"
	"OXShortkeysOrganizer.cpp"
	"OXStatic.cpp"		
	"OXBitmapMenu.cpp"		
	"OXBitmapMenuOrganizer.cpp"		
	

Resource files:
	"OXCustomizeManager.rc"		


*/

#if !defined(_OXCUSTOMIZESHORTKEYSPAGE_H__)
#define _OXCUSTOMIZESHORTKEYSPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#define __AFXTEMPL_H__
#endif

#include "OXCustomizePage.h"
#include "OXListBoxEx.h"
#include "OXSeparator.h"
#include "OXStatic.h"
#include "OXShortkeysOrganizer.h"
#include "OXMainRes.h"

#ifndef NM_KEYDOWN
#define NM_KEYDOWN              (NM_FIRST-15)   // uses NMKEY struct
#endif

#if _MFC_VER <= 0x0421
typedef struct tagNMKEY
{
    NMHDR hdr;
    UINT  nVKey;
    UINT  uFlags;
} NMKEY, FAR *LPNMKEY;
#endif


// Helper hot key control object. Notifies parent window about some events 
//
class OX_CLASS_DECL COXInteractiveHotKeyCtrl : public CHotKeyCtrl
{
public:
	COXInteractiveHotKeyCtrl() {};
	virtual ~COXInteractiveHotKeyCtrl() {};

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
#if defined (_WINDLL)
#if defined (_AFXDLL)
		AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

		LRESULT lResult=CHotKeyCtrl::WindowProc(message,wParam,lParam);
		
		if(!::IsWindow(GetSafeHwnd()))
			return lResult;

		CWnd* pParentWnd=GetParent();
		if(pParentWnd!=NULL)
		{
			switch(message)
			{
			case WM_KEYDOWN:
				{
					NMKEY nmKey;
					nmKey.hdr.code=NM_KEYDOWN;
					nmKey.hdr.hwndFrom=GetSafeHwnd();
					nmKey.hdr.idFrom=GetDlgCtrlID();
					nmKey.nVKey=(UINT)wParam;
					nmKey.uFlags=HIWORD(lParam);
					pParentWnd->SendMessage(WM_NOTIFY,(WPARAM)GetDlgCtrlID(),
						(LPARAM)&nmKey);
					break;
				}
			case WM_SETFOCUS:
				{
					NMHDR nmHdr;
					nmHdr.code=NM_SETFOCUS;
					nmHdr.hwndFrom=GetSafeHwnd();
					nmHdr.idFrom=GetDlgCtrlID();
					pParentWnd->SendMessage(WM_NOTIFY,(WPARAM)GetDlgCtrlID(),
						(LPARAM)&nmHdr);
					break;
				}
			case WM_KILLFOCUS:
				{
					NMHDR nmHdr;
					nmHdr.code=NM_KILLFOCUS;
					nmHdr.hwndFrom=GetSafeHwnd();
					nmHdr.idFrom=GetDlgCtrlID();
					pParentWnd->SendMessage(WM_NOTIFY,(WPARAM)GetDlgCtrlID(),
						(LPARAM)&nmHdr);
					break;
				}
			}
		}
		return lResult;
	}
};
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeShortkeysPage dialog

class OX_CLASS_DECL COXCustomizeShortkeysPage : public COXCustomizePage
{
	DECLARE_DYNCREATE(COXCustomizeShortkeysPage)
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXCustomizeShortkeysPage();

// Dialog Data
	//{{AFX_DATA(COXCustomizeShortkeysPage)
	enum { IDD = IDD_OX_PAGE_SHORTKEYS };
	COXStatic	m_textDescription;
	COXInteractiveHotKeyCtrl	m_hotKey;
	CComboBox	m_comboTable;
	CTreeCtrl	m_treeCommands;
	COXSeparator	m_sepShortkeys;
	COXSeparator	m_sepDescription;
	COXSeparator	m_sepConflict;
	COXSeparator	m_sepCommands;
	COXListBoxEx	m_listShortkeys;
	CString	m_sAssignedTo;
	CString	m_sCommandDescription;
	int		m_nAccelTableIndex;
	int		m_nShortkeyIndex;
	//}}AFX_DATA



protected:
	// map of command IDs and corresponding image indexes. If there is no
	// image associated with a command then image index equals -1
	CMap<UINT,UINT,int,int> m_mapCommands;

	// image list of all images for all commands
	CImageList m_ilImages;

	// organizer responsible for editing accellerator tables
	COXShortkeysOrganizer m_shortkeysOrganizer;

	// flag that specifies that shortkeys tables are not editable
	BOOL m_bReadOnly;

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
		VERIFY(sRet.LoadString(IDS_OX_CSTMZESHORTKEYSTITLE));
		return sRet;
		//_T("Shortkeys"); 
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
		return MAKEINTRESOURCE(IDB_OX_CUSTOMIZE_SHORTKEYS); 
	}

	// --- In  :	
	// --- Out : 
	// --- Returns: The tooltip for the page item as it appears in customize manager's 
	//				shortcut bar
	// --- Effect : Retrieves the tooltip for the page item. 
	virtual CString GetTooltip() const {
		CString sRet;
		VERIFY(sRet.LoadString(IDS_OX_CSTMZESHORTKEYSTOOLTIP));
		return sRet;
		// _T("Customize accelerator tables"); 
	}


	// --- In  :	bIsOk	-	TRUE if the customize manager window is being closed 
	//							as a result of pressing "OK" button; FALSE otherwise
	// --- Out : 
	// --- Returns:	TRUE if the page was successfully closed; FALSE otherwise
	// --- Effect : Called by customize manager to close the page. Advanced 
	//				overridable, use your own implementation in the case you 
	//				need to handle the close event.
	virtual BOOL OnCloseManager(BOOL bIsOk);


	// --- In  :	pFrameWnd		-	pointer to main frame window of the 
	//									application
	//				lpszProfileName	-	the name of the hive in the registry 
	//									where updated accelerator tables contents
	//									is saved. If non-NULL value is set then 
	//									the contents is retrieved from the registry 
	//									and applied right away.
	// --- Out : 
	// --- Returns: TRUE if information on the application's accelerator tables was 
	//				initialized successfully; FALSE otherwise
	// --- Effect : Initializes the information on the application's accelerator 
	//				tables and retrieves their saved in the registry contents and 
	//				applies it
	virtual BOOL InitializeShortkeys(CFrameWnd* pFrameWnd, 
		LPCTSTR lpszProfileName=_T("CustomizeShortkeys"));


	// --- In  :	
	// --- Out : 
	// --- Returns: A pointer to COXShortkeysOrganizer object that is responsible for 
	//				saving/loading accelerator table contents and applying it to the 
	//				application
	// --- Effect : Retrieves a pointer to Customize Shortkeys page. This page can 
	//				be used to customize the contents of application's accelerator
	//				table. New shortkeys can be assigned to existing commands
	//				or removed from accelerator table.
	virtual COXShortkeysOrganizer* GetShortkeysOrganizer() 
	{
		return &m_shortkeysOrganizer;
	}


protected:
	// retrieves internal variables
	void GetVars();
	// updates page controls state
	void ShowVars();
	// retrieves the set of command from a menu (recursive function)
	void RetrieveCommandsFromMenu(HMENU hMenu);
	// adds the contents of the specified menu to tree (recursive function)
	void AddCommandsFromMenuToTree(HMENU hMenu, HTREEITEM hParentItem);
	// searches for specified item in the tree
	HTREEITEM FindMenuItemInTree(CString sText, HTREEITEM hParentItem) const;

	// retrieves the info about all accelerators defined in application
	void GetShortkeys();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXCustomizeShortkeysPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXCustomizeShortkeysPage();

protected:
	// --- In  :	
	// --- Out : 
	// --- Returns:	
	// --- Effect : Called internally just after the page window was created. 
	//				Override this function in order to initialize the controls 
	//				on the page. Don't forget to call the default implementation 
	//				unless you duplicate it in your code
	virtual void OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(COXCustomizeShortkeysPage)
	afx_msg void OnButtonAssign();
	afx_msg void OnButtonRemove();
	afx_msg void OnButtonRemoveAll();
	afx_msg void OnButtonResetAll();
	afx_msg void OnSelchangeComboTable();
	afx_msg void OnSelchangeListShortkeys();
	afx_msg void OnSelchangedTreeCommands(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownHotkey(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_OXCUSTOMIZESHORTKEYSPAGE_H__)
