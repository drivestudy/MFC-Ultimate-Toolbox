// ==========================================================================
//								Class Specification : 
//			COXTabClientWnd & COXTabWorkspace & COXTabWorkspaceDropTarget
// ==========================================================================

// Header file : OXTabClientWnd.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////

#ifndef _OXTABCLIENTWND_H__
#define _OXTABCLIENTWND_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#define __AFXTEMPL_H__
#endif

#ifndef __AFXOLE_H__
#include <afxole.h>
#define __AFXOLE_H__
#endif

#include "OXSkinnedTabCtrl.h"

#include "UTB64Bit.h"

/*

Description.

We introduce a new interface extension for MDI(multiple document interface) 
based applications - tabbed MDI interface or MTI (just make it sound 
familiar).

We haven't changed anything in the relationships between a frame window 
(MDIFrame window) and its children (MDIChild windows). We changed the 
functionality of the window that is usually overlooked while developing
MDI applications, being the MDIClient window which resides in the client 
area of the MDIFrame window and manages MDIChild windows. Instead of
just displaying the MDIClient window we also display a standard tab control
(that's why "tabbed" MDI) in which we create an item for every MDIChild
window. The window icon and text will be associated with the corresponding
tab item. 

Using the tab control you can switch very quickly between MDIChild windows
by just clicking on the tab item. If you double click over a tab item then
the corresponding MDIChild window will be maximized/restored. And, finally,
when any object is dragged over the tab control items (using standard 
OLE drag and drop) the corresponding MDIChild window will be activated.

Of course, as long as we use a standard tab control you can customize its
appearance using the standard set of applicable functions.  


We developed three classes in order to provide the above described 
functionality:

	COXTabWorkspaceDropTarget	-	COleDropTarget derived class used 
									to support the changing of active
									MDIChild when any object is dragged 
									over tab control items.

	COXTabWorkspace				-	CTabCtrl derived class. Covers 
									MDIClient area. For every MDIChild
									window there will be the tab item
									that will use the window text and icon
									as the item text and icon. Whenever you
									click on an item the corresponding
									child window will be activated.
									Whenever you double click on the item
									the corresponding MDIChild window
									will be maximized/restored.

	COXTabClientWnd				-	CWnd derived class. Subclasses MDIClient
									window. Manages the placement of the 
									MDIClient and tab control regarding each 
									other. 


Almost all the logic of the classes is implemented internally and there is not 
that many public members. Refer to the COXTabWorkspace reference for the list of 
functions available to customize the tab control appearance.


COXTabClientWnd class has a few public functions (refer to COXTabClientWnd
reference for details) but primarily you will be interested in the following ones:

	BOOL Attach(const CMDIFrameWnd* pParentFrame, 
		DWORD dwTabCtrlStyle=DEFAULT_TABCTRLSTYLE);
	// --- In  :	pParentFrame	-	pointer to MDIFrame window of 
	//									the application
	//				dwTabCtrlStyle	-	tab control styles that will be 
	//									used while creating the tab control.
	//									Refer to the Windows SDK documentation
	//									for list of all available styles.
	//									The following styles are used by 
	//									default:
	//
	//									TCS_MULTILINE
	//									TCS_BOTTOM
	//									TCS_HOTTRACK
	//									TCS_SCROLLOPPOSITE
	//									TCS_RIGHTJUSTIFY
	//
	// --- Out : 
	// --- Returns: TRUE if success or FALSE otherwise.
	// --- Effect : Substitutes the standard MDI interface with enhanced
	//				tabbed MDI
	
	BOOL Detach();
	// --- In  :
	// --- Out : 
	// --- Returns: TRUE if success or FALSE otherwise.
	// --- Effect : Restore the standard MDI interface


So in order to implement a tabbed MDI application you could take the 
following steps:

	1)	Create the standard MDI application or use the existing one. 
	2)	Define the object of COXTabClientWnd class in your 
		CMDIFrameWnd derived class (usually CMainFrame)

		// MTI client window
		COXTabClientWnd m_MTIClientWnd;

	3)	In the OnCreate() function of your CMDIFrameWnd derived class call
		the COXTabClientWnd::Attach function

		m_MTIClientWnd.Attach(this);

That's it. 



Example.
We updated the CoolToolBar sample that is found in the .\Samples\gui\CoolToolBar
subdirectory of your Ultimate Toolbox directory. There you will see how you 
can customize the tabbed MDI interface appearance.

*/


#define ID_TABOFFSET				1

#define IDT_MDI_STATUS_TIMER		333
#define IDC_TABWORKSPACE			1000


const DWORD DEFAULT_TABCTRLSTYLE = TCS_HOTTRACK | TCS_RIGHTJUSTIFY;

/////////////////////////////////////////////////////////////////////////////
// COXTabWorkspaceDropTarget drop target for COXTabWorkspace, used to facilitate 
// auto-selection on drag over.
class OX_CLASS_DECL COXTabWorkspaceDropTarget : public COleDropTarget
{
public:
	COXTabWorkspaceDropTarget() : m_nOldItem(-1) {};
	virtual ~COXTabWorkspaceDropTarget() {};

protected:
	int m_nOldItem;

protected:
	// Change tab on drag over handler.
    virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, 
		DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave( CWnd* pWnd );
};


// Structure used internally to represent information about any created
// MDIChild window. 
typedef struct _tagTAB_ITEM_ENTRY
{
	CString sText;
	CString sWndClass;
	CWnd* pWnd;
	BOOL bFound;

} TAB_ITEM_ENTRY;
//
//	sText		-	MDIChild window text.
//	sWndClass	-	Name of the window class. 
//	pWnd		-	Pointer to MDIChild window object.
//	bFound		-	Parameter used for integrity testing. Set to TRUE if 
//					corresponding MDIChild window is still active.
//
//////////////////////////////////////////////////////////////////////////


class COXTabClientWnd;
class COXTabSkin;

/////////////////////////////////////////////////////////////////////////////
// COXTabWorkspace window
class OX_CLASS_DECL COXTabWorkspace : public COXSkinnedTabCtrl
{
	friend class COXTabSkinClassic;

	DECLARE_DYNAMIC(COXTabWorkspace)

// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object.
	COXTabWorkspace();


// Attributes
public:

protected:
	// Array of tab items (every item corresponds to a MDIChild window)
    CArray<TAB_ITEM_ENTRY, TAB_ITEM_ENTRY> m_arrTab;
	// Array of MDIChild windows icons used in the tab control
    CArray<HICON, HICON> m_arrImage;

	// Image list associated with the tab control
    CImageList m_imageList;
    
	// Drop target object for OLE dragging support (when any object is
	// dragged over tab items the corresponding MDIChild windows will be
	// activated).
	COXTabWorkspaceDropTarget m_dropTarget;
	
	// Pointer to our substitute for the MDIClient window. If it is NULL then
	// the tab control is not defined and there shouldn't be any action 
	// taken on it.
	COXTabClientWnd* m_pTabClientWnd;

	// Offset from the borders of the client area of the MDIFrame window
	// where the tab control will be displayed.
	DWORD m_dwOffset;

	// If TRUE then when any object is dragged over tab items the 
	// corresponding MDIChild windows will be activated
	BOOL m_bAcceptDraggedObject;

// Operations
public:

	// --- In  :	dwOffset		-	Offset in points from the MDIFrame window
	//									client area where the tab control will be 
	//									displayed.
	//				bRecalcLayout	-	If TRUE then layout of the parent frame window 
	//									will be recalculated right away.
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets the tab control offset from MDIFrame borders
	inline void SetOffset(DWORD dwOffset, BOOL bRecalcLayout=TRUE) 
	{ 
		m_dwOffset=dwOffset; 
		if(::IsWindow(GetSafeHwnd()))
		{
			SetWindowPos(NULL,0,0,0,0,
				SWP_NOMOVE|SWP_DRAWFRAME|SWP_NOSIZE|SWP_NOZORDER);
			if(bRecalcLayout)
			{
				CMDIFrameWnd* pFrame=GetParentFrame();
				if(pFrame!=NULL)
					pFrame->RecalcLayout();
			}
		}
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	Offset in points from the MDIFrame window client area 
	//				where the tab control will be displayed
	// --- Effect : Retrieves the tab control offset from MDIFrame borders
	inline DWORD GetOffset() const { return m_dwOffset; }


	// --- In  :	bAccept	- If TRUE then when any object is dragged 
	//						  over tab items the corresponding MDIChild 
	//						  windows will be activated.
	// --- Out : 
	// --- Returns:	
	// --- Effect : Set/Remove drag object over support for the tab control.
	inline void AcceptDraggedObject(BOOL bAccept=TRUE) {
		m_bAcceptDraggedObject=bAccept;
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if dragging an object over tab items causes the 
	//				corresponding MDIChild windows will be activated.
	// --- Effect : Retrieves the flag that specifies whether the control 
	//				activates the corresponding MDIChild window when an object 
	//				is dragged over tab control items.
	inline BOOL IsAcceptingDraggedObject() const { 
		return m_bAcceptDraggedObject; 
	}


	// Scan through all MDIChild windows and update corresponding 
	// tab items if any changes occurred (e.g. window text or active MDIChild).
	// If bAddNewWindows is set to TRUE then for any new found MDIChild
	// window the new tab item will be created (this option is useful when
	// it is called for the first time and there are already some MDIChild windows
	// created). 
	void UpdateContents(BOOL bAddNewWindows=FALSE, BOOL bUpdateWindowsInfo=FALSE);


protected:
	// Returns the pointer for MDIFrame window
	CMDIFrameWnd* GetParentFrame() const;

	// Returns text for child window to be displayed in corresponding
	// tab item
	virtual CString GetTextForTabItem(const CWnd* pChildWnd) const;


	// Finds the tab item that corresponds to the specified window
	inline int FindTabItem(const CWnd* pWnd) const 
	{
		ASSERT(pWnd!=NULL);
		ASSERT(::IsWindow(pWnd->GetSafeHwnd()));
		return FindTabItem(pWnd->GetSafeHwnd());
	}

	// Finds the tab item that corresponds to the specified window
	int FindTabItem(HWND hWnd) const;
	// Adds new tab item for the specified window
	inline BOOL AddTabItem(const CWnd* pChildWnd, BOOL bRedraw=TRUE, 
		BOOL bOnlyVisible=TRUE)
	{
		return InsertTabItem(PtrToInt(m_arrTab.GetSize()),pChildWnd,bRedraw,bOnlyVisible);
	}
	// Adds new tab item for the specified window
	BOOL InsertTabItem(int nIndex, const CWnd* pChildWnd, BOOL bRedraw=TRUE, 
		BOOL bOnlyVisible=TRUE);
	// Removes the tab item for the specified window
	BOOL RemoveTabItem(const CWnd* pChildWnd, BOOL bRedraw=TRUE);

	// Retrieves an icon associated with the specified window
	static HICON GetWindowIcon(HWND hWnd);
	// Saves specified icon into internal image list
	int AddTabItemIcon(HICON hIcon);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXTabWorkspace)
	//}}AFX_VIRTUAL

// Implementation
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destroys the object.
	virtual ~COXTabWorkspace();

	// Generated message map functions
protected:
	//{{AFX_MSG(COXTabWorkspace)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    friend class COXTabWorkspaceDropTarget;
    friend class COXTabClientWnd;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


/////////////////////////////////////////////////////////////////////////////
// COXTabClientWnd window

class OX_CLASS_DECL COXTabClientWnd : public CWnd
{
	friend class COXTabSkinClassic;
	friend class COXTabSkinXP;

// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object.
	COXTabClientWnd();

// Attributes
public:

protected:
	// Tab control
	COXTabWorkspace m_tab;
	// Pointer to the corresponding parent MDIFrame window.
	CMDIFrameWnd* m_pParentFrame;

	// Flag which specifies that the layout of the tab control and MDIClient 
	// must be recalculated
	BOOL m_bForceToRecalc;

	// There is only one tab
	BOOL m_bOneTabMode;

// Operations
public:
	// --- In  :	pParentFrame	-	Pointer to MDIFrame window of 
	//									the application.
	//				dwTabCtrlStyle	-	Tab control styles that will be 
	//									used while creating the tab control.
	//									Refer to the Windows SDK documentation
	//									for a list of all available styles.
	//									The following styles are used by 
	//									default:
	//
	//									TCS_MULTILINE
	//									TCS_BOTTOM
	//									TCS_HOTTRACK
	//									TCS_SCROLLOPPOSITE
	//									TCS_RIGHTJUSTIFY
	//
	// --- Out : 
	// --- Returns: TRUE if success or FALSE otherwise.
	// --- Effect : Substitutes the standard MDI interface with enhanced
	//				tabbed MDI.
	BOOL Attach(const CMDIFrameWnd* pParentFrame, 
		DWORD dwTabCtrlStyle=DEFAULT_TABCTRLSTYLE);
	
	// --- In  :
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise.
	// --- Effect : Restores the standard MDI interface.
	BOOL Detach();

	// --- In  :
	// --- Out : 
	// --- Returns: TRUE if the tabbed MDI interface is active.
	// --- Effect : Retrieves the flag that specifies whether the 
	//				standard MDI interface was substituted with enhanced
	//				tabbed MDI or not.
	inline BOOL IsAttached() const { 
		return (m_pParentFrame!=NULL ? TRUE : FALSE); 
	}

	// --- In  :
	// --- Out : 
	// --- Returns: Pointer to the tab control.
	// --- Effect : Retrieves a pointer to the tab control.
	inline COXTabWorkspace* GetTabCtrl() {
		if(!IsAttached())
			return NULL;
		return &m_tab;
	}

	// --- In  :
	// --- Out : 
	// --- Returns: Pointer to the parent MDIFrame window or NULL if none
	//				was attached.
	// --- Effect : Retrieves a pointer to the parent MDIFrame window.
	inline CMDIFrameWnd* GetParentFrame() 
	{ 
#ifdef _DEBUG
		if(!IsAttached())
			ASSERT(m_pParentFrame==NULL);
		else
		{
			ASSERT(m_pParentFrame!=NULL);
			ASSERT(m_pParentFrame->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)));
		}
#endif
		return m_pParentFrame; 
	}

	// --- In  :	lpszProfileName		-	The name of the .ini file group or 
	//										hive in registry where all information 
	//										about the TabbedMDI workspace state will be saved.
	// --- Out :	
	// --- Returns: TRUE if successful, FALSE otherwise.
	// --- Effect:	Saves the TabbedMDI workspace state into the registry or *.ini file.
	//				In order to forward all output in the registry you have to call the
	//				CWinApp::SetRegistryKey() function while initializing your 
	//				application (usually in InitInstance() function).
	BOOL SaveState(LPCTSTR lpszProfileName);

	// --- In  :	lpszProfileName		-	The name of the .ini file group or 
	//										hive in registry from which all information 
	//										about TabbedMDI workspace state will be 
	//										retrieved
	// --- Out :	
	// --- Returns: TRUE if successful, or FALSE otherwise.
	// --- Effect:	Loads the TabbedMDI workspace state from the registry or *.ini file.
	//				In order to read saved info. from the registry you have to call the
	//				CWinApp::SetRegistryKey() function while initializing your 
	//				application (usually in InitInstance() function).
	BOOL LoadState(LPCTSTR lpszProfileName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXTabClientWnd)
	protected:
	virtual void CalcWindowRect(LPRECT lpClientRect,UINT nAdjustType=adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destroys the object
	virtual ~COXTabClientWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(COXTabClientWnd)
	afx_msg LRESULT OnMDIActivate(UINT wParam, LONG lParam);
	afx_msg LRESULT OnMDICreate(UINT wParam, LONG lParam);
	afx_msg LRESULT OnMDIDestroy(UINT wParam, LONG lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnNcPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class COXTabWorkspace;

private:
	COXTabSkin* m_pTabSkin;
	COXTabSkin* GetTabSkin();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // _OXTABCLIENTWND_H__
