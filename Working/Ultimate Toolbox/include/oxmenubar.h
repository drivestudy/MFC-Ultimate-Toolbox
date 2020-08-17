// ==========================================================================
//						   Class Specification : 
//				COXMenuBar & COXMenuDockBar & COXMenuBarFrame
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

/*

Many products (Misrosoft Office, Visual Studio IDE to name just a few) 
use special replacement for standard menu bar in their frame window. 
It looks like common dockable flat toolbar while implements all the
functionality found in standard menu bar (accelerators, mouse and 
keyboard navigation). 

Due to similarity to standard toolbar control we thought it would be natural 
to try to use this control in order to implement the menubar.

But we have to note that only using toolbar control that is available
in comctl32.dll v4.71 and higher would allow us to implement the 
functionality of menubar to full extent (primarily, because new button
style TBSTYLE_AUTOSIZE, which allows to use buttons of different width 
in the toolbar, was introduced only in v4.71). As long as comctl32.dll
was freely redistributable we felt like it wasn't a big deal. 

Then, we already had the COXCoolToolBar class in our library that could be
good base class for menubar implementation. 

So we developed new class that is called COXMenuBar and is based on
COXCoolToolBar. If you try to use it with old version of comctl32.dll
we will throw unsupported exception.


The problem with replacing standard window menu with dockable menubar
had to be resolved in tight cooperation between frame window and 
menubar control. The solution should have worked with any CFrameWnd
derived window (CFrameWnd had to be used because it is support
docking functionality). The problem turned out to be more complicated
due to the fact that we had to provide our own CDockBar derived class
(COXMenuDockBar) in order to implement menu bar (it has to do with docking 
functionality). CDockBar is undocumented MFC class that is used 
internaly to provide docking functionality for any CControlBar derived
class within CFrameWnd derived framework window. 

Moreover, implementation for SDI and MDI main frame window was a little
bit different in both cases. And anyway it was highly possible to assume 
that programmers could use their own CFrameWnd derivates in their 
applications (e.g. they could have used our COXMDIFrameWndSizeDock class
in order to provide docking windows functionality).

So, we had major problem: implementation that will work with any 
CFrameWnd derived class. 

And we came up with the following solution.
We designed template class COXMenuBarFrame that defined as follows:


	template<class PARENTFRAME, class PARENTDOCKBAR>
	class COXMenuBarFrame : public PARENTFRAME
	{
		. . . . . . . . . . . . . . 
	};


where	PARENTFRAME is the name of the parent frame class (e.g. CFrameWnd)
		PARENTDOCKBAR is the name of parent dock bar class (e.g. CDockBar)

Using this approach you can define your mainframe window as:

	class CMainFrame : public COXMenuBarFrame<CMDIFrameWnd,CDockBar>

in order to implement standard MDI application with automatic menu bar. 
That's all what you have to do in order to include in your application 
support for menubar. We would like to note that in standard macros
like IMPLEMENT_DYNAMIC or BEGIN_MESSAGE_MAP you have to eplicitly use 
the name of the parent frame class:

	BEGIN_MESSAGE_MAP(CMainFrame,CMDIFrameWnd)


Although, COXMenuBarFrame, COXMenuDockBar and COXMenuBar classes define
a number of public functions you wouldn't normally use them because
the functionality that we provide is self sufficient and doesn't provide
any additional customization functionality.


*/

#ifndef _MENUBAR_H
#define _MENUBAR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"

#include <afxole.h>
#include <afxtempl.h>
#include <afxpriv.h>

#include "UTB64Bit.h"

#ifndef __OX_OLEIMPL2_H__
#ifdef MAP_LOGHIM_TO_PIX
#undef MAP_LOGHIM_TO_PIX
#endif
#ifdef MAP_PIX_TO_LOGHIM
#undef MAP_PIX_TO_LOGHIM
#endif

#if _MFC_VER < 0x0700
	#include "..\src\oleimpl2.h"
#else
	#include "..\src\mfc\oleimpl2.h"
#endif

#define __OX_OLEIMPL2_H__
#endif


#if _MFC_VER>=0x0420

#include "OXCoolToolBar.h"

// default ID for menubar
#define AFX_IDW_MENUBAR			0xE810


// base id for menubar buttons that correspond to the index
#define ID_CMDBASE				1
// the gap between close, restore and minimize buttons and menu items
#define ID_BUTTONSGAP			6

typedef struct _tagOXCUSTOMIZEPOPUPMENUINFO
{
	HMENU hMenu;
	UINT nFlags;
	POINT ptStart;
} OXCUSTOMIZEPOPUPMENUINFO;


// hit test values
typedef enum _tagMENUBARHITTEST
{
	OX_MNU_ICON=-5,
	OX_MNU_CLOSEBTN=-4,
	OX_MNU_RESTOREBTN=-3,
	OX_MNU_MINIMIZEBTN=-2,
	OX_MNU_NONE=-1
} MENUBARHITTEST;
//////////////////////

// test point origin
typedef enum _tagHITTESTORIGIN
{
	OX_MNU_CLIENT,
	OX_MNU_SCREEN,
	OX_MNU_ZEROBASED

} HITTESTORIGIN;
/////////////////////////
	

// ID of the expansion item
#ifndef ID_OX_SHOWALLITEMS
#define ID_OX_SHOWALLITEMS					0x0000ff56
#endif	//	ID_OX_SHOWALLITEMS

// Define the WM_QUERYSNAPPING user message used to query a frame window whether or not
// the snaping and tear-off behavior is enabled
#ifndef WM_QUERYSNAPPING
	#define WM_QUERYSNAPPING (WM_USER + 92)
#endif

/////////////////////////////////////////////////////////////////////////////
// COXMenuBar window

class COXMenuSkin;
class COXBitmapMenu;

class OX_CLASS_DECL COXMenuBar : public COXCoolToolBar
{
	friend class COXToolbarSkinClassic;
	friend class COXToolbarSkinXP;
	friend class COXToolbarSkin2003;

	DECLARE_DYNAMIC(COXMenuBar)
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXMenuBar();

public:
	// handle to the submenu that will contain the MRU MDIChild windows
	HMENU m_hMDIWindowMenu;

	// static function that will search for menubar 
	static COXMenuBar* FindMenuBar(CWnd* pStartWnd=NULL);

// Attributes
public:
	// handle to the menu that is currently displayed in the menu bar
	HMENU m_hMenu;

	// zero-based index of the menu item that is currently displayed
	// in the dropdown state (the corresponding popup menu is active)
	int m_nActiveMenuItem;
	// zero-based index of the menu item that will be activated (the 
	// corresponding popup menu will be active) after the currently 
	// active popup menu is deactivated
	int m_nActivateNextItem;
	// zero-based index of the menu item that cannot be set as 
	// m_nActivateNextItem
	int m_nForbiddenItem;

	// flag that specifies that menu should take entire row
	// while displayed
	BOOL m_bForceEntireRow;

	// flag that specifies that menu bar should state 
	// in surfing mode after Esc key was pressed when
	// top level popup menu was active
	BOOL m_bStaySurfing;

protected:
	// flag that specifies that menu bar will take the entire row or column 
	// while docked. At the moment this flag is always TRUE
	BOOL m_bTakeEntireRow;

	// pointer to the parent frame window
	CWnd* m_pFrameWnd;
	// accelerator table for menu items 
	CMap<TCHAR,TCHAR,int,int> m_accelTable;

	// rectangles for close, restore and minimize buttons on the bar
	CRect m_rectCloseBtn;
	CRect m_rectRestoreBtn;
	CRect m_rectMinimizeBtn;
	////////////////////////////////////////////////////////////////

	// currently pressed button (close, restore or minimize)
	MENUBARHITTEST m_pressedBtn;

	// flag that specifies that menu bar is in surfing mode when user can 
	// use keyboard to navigate through menu items.
	BOOL m_bIsInSurfingMode;

	// font used to draw the menu bar
	CFont m_fontMenu;
	// flag that specifies that current menu display settings has been 
	// retrieved and they are up-to-date
	BOOL m_bIsMenuMetricInitialized;

	// map of all menus associated with all document templates defined
	// in application. Only this menus are customizable
	CMap<HMENU,HMENU,CDocTemplate*,CDocTemplate*> m_mapCustomizableMenus;
	// map of all menus that has been customized but hasn't been saved yet
	CMap<HMENU,HMENU,int,int> m_mapChangedMenus;

	// array of created popup menus that should be destroyed at the end
	CArray<HMENU,HMENU> m_arrCreatedPopupMenus;

	// timer for drag'n'drop event
	UINT_PTR m_nCheckForDragDropEventTimerID;

	// item that will be dragged
	int m_nWouldBeDraggedItem;

private:
	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
	static HHOOK m_hMouseHook;
	static HWND m_hwndPrevMouseMoveWnd;
	COXMenuSkin* GetMenuSkin();
	COXMenuSkin* m_pMenuSkin;
	void RecalculateItemWidths();

// Operations

public:
	
	// --- In  :	pParentWnd	-	Pointer to the window that is the 
	//								menubar's parent
	//				dwStyle		-	The menubar style. Refer to decription
	//								of CToolBar::Create() function for details
	//				nID			-	The toolbar's child-window ID.
	// --- Out : 
	// --- Returns:	TRUE if menubar was successfully created, 
	//				or FALSE otherwise
	// --- Effect : Creates the menubar
	BOOL Create(CWnd* pParentWnd, 
		DWORD dwStyle=WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_SIZE_DYNAMIC,
		UINT nID=AFX_IDW_MENUBAR);


	// --- In  :	
	// --- Out : 
	// --- Returns:	Handle to the menu that is used in the menu bar
	// --- Effect : Retrieves handle to the menubar menu
	HMENU GetMenu() const { 
		ASSERT(::IsWindow(GetSafeHwnd()));
		return m_hMenu; 
	}


	// --- In  :	hMenu	-	handle to the valid menu that will be 
	//							displayed in the menubar
	// --- Out : 
	// --- Returns:	TRUE if the menu was successfully set,
	//				or FALSE otherwise
	// --- Effect : Sets menu to be displayed in the menubar
	BOOL SetMenu(HMENU hMenu);


	// --- In  :	bTakeEntireRow	-	flag that specifies whether the 
	//									menubar will take the entire 
	//									row/column
	//				bRedraw			-	flag that specifies whethern the
	//									menubar will be redrawn or not
	// --- Out : 
	// --- Returns:	
	// --- Effect : Sets the flag that specifies whether the menubar 
	//				will take the entire row/column
	inline void SetTakeEntireRow(BOOL bTakeEntireRow, BOOL bRedraw=TRUE) 
	{
		m_bTakeEntireRow=bTakeEntireRow;
		if(bRedraw)
		{
			RedrawToolBar();
		}
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	
	// --- Effect : Retrieves the flag that specifies whether the menubar 
	//				will take the entire row/column
	inline BOOL GetTakeEntireRow() const { return m_bTakeEntireRow; }


	// --- In  :	bHorz	-	TRUE if the size of row is retrieved, 
	//							FALSE if the size of column is retrieved
	// --- Out : 
	// --- Returns:	Size of row/column to display the menu bar in
	// --- Effect : Retrieves the size of row/column to display the menu bar in
	CSize GetEntireRow(BOOL bHorz) const;


	// --- In  :	bIsInSurfingMode	-	flag that specifies whether the 
	//										surfing mode is set/removed.
	//				nItem				-	if bIsInSurfingMode is TRUE then
	//										this variable specifies the index
	//										of the item that has to be 
	//										initially displayed as hot in the
	//										surfing mode.
	// --- Out :	
	// --- Returns:	TRUE if surfing mode was successfully set/removed
	// --- Effect : Sets/removes the surfing mode. If menu bar is in surfing 
	//				mode then user can use keyboard to navigate through menu 
	//				items
	BOOL SetSurfingMode(BOOL bIsInSurfingMode=TRUE, int nItem=0);

	// --- In  :	
	// --- Out :	
	// --- Returns:	TRUE if menubar is in surfing mode,	or FALSE otherwise
	// --- Effect : Retrives the flag that specifies whether the menubar is
	//				in the surfing mode or not. If menu bar is in surfing 
	//				mode then user can use keyboard to navigate through 
	//				menu items
	inline BOOL IsInSurfingMode() const 
	{ 
		ASSERT(::IsWindow(m_hWnd));
		return m_bIsInSurfingMode; 
	}


	// --- In  :	
	// --- Out :	
	// --- Returns:	TRUE if menubar is in displaying mode, or FALSE otherwise.
	// --- Effect : Retrives the flag that specifies whether the menubar is
	//				in the displaying mode or not. If menu bar is in 
	//				displaying mode then one of the popup menus is displayed
	inline BOOL IsInDisplayingMode() const 
	{ 
		ASSERT(::IsWindow(m_hWnd));
		return (m_nActiveMenuItem>=0 && 
			m_nActiveMenuItem<=GetToolBarCtrl().GetButtonCount()); 
	}


	// returns TRUE if menu content can be dynamically changed in Customize Manger
	virtual BOOL IsCustomizable(BOOL bAdvanced=TRUE) const;


	// Save to and load from registry state of application menus.
	// We use default registry key assigned to your application by MFC as
	// a storage. We distinguish 
	//
	////////////////////////
	virtual BOOL SaveBarState(LPCTSTR lpszSubKey, LPCTSTR lpszValueName, 
		BOOL bProperties=TRUE);
	virtual BOOL LoadBarState(LPCTSTR lpszSubKey, LPCTSTR lpszValueName, 
		BOOL bProperties=TRUE);

	// saves/restores the state of menu in/from registry
	BOOL SaveMenuState(HMENU hMenu, LPCTSTR lpszSubKey);
	BOOL LoadMenuState(HMENU hMenu, LPCTSTR lpszSubKey);


	virtual BOOL DisplayCustomizeButtonContextMenu(int nButtonIndex, CPoint point);

	// mark the menu as the changed one
	inline void MarkAsChanged()
	{
		// mark this menu as a changed one
		if(!IsMarkedAsChanged())
		{
			m_mapChangedMenus.SetAt(GetMenu(),PtrToInt(m_mapChangedMenus.GetCount()));
		}
	}

	// check if menu is marked as the changed one
	inline BOOL IsMarkedAsChanged()
	{
		int nIndex=-1;
		HMENU hMenu=GetMenu();
		return (m_mapChangedMenus.Lookup(hMenu,nIndex));
	}

	// --- In  :	
	// --- Out :	
	// --- Returns:
	// --- Effect : This member function fills the RECT structure whose address is 
	//				contained in lpRect with the coordinates of the button or separator
	//				specified by nIndex. Coordinates are in pixels relative to the
	//				upper-left corner of the toolbar.
	virtual void GetItemRect(int nIndex, LPRECT lpRect);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

protected:
	// repiopulate the menu bar
	BOOL RecreateMenuBar(BOOL bRedraw=TRUE);
	// refresh the menubar icon if any is displayed
	void UpdateIcon(BOOL bRedraw=TRUE);
	// sets the specified menu item to pressed state and
	// displays the corresponding popup menu
	void DisplayPopupMenu(int nMenuItem);
	// restores the state of the previously ative menu item
	void RestoreMenuButton();
	// calculates the size of rectangles for close, restore 
	// and minimize buttons
	int CalcButtonRects();
	// draws close, restore and minimize buttons
	virtual void DrawButtons(CDC& dc);

	// function that is called when the size of the menu bar 
	// is requested by dock bar or dock context
	virtual CSize CalcLayout(DWORD nMode, int nLength=-1);

	// returns the index of the previous (only visible if specified)
	// item in the menu bar
	int GetPrevMenuItem(int nItem, BOOL bEnsureVisile=TRUE);
	// returns the index of the next (only visible if specified)
	// item in the menu bar
	int GetNextMenuItem(int nItem, BOOL bEnsureVisible=TRUE);

// Implementation
public:
	virtual CSize CalcSize(TBBUTTON* pData, int nCount);
	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXMenuBar();

	
	// --- In  :	ptTest	-	point to test
	//				Origin	-	origin of the point. Can be one of the 
	//							following:
	//		
	//							OX_MNU_CLIENT		-	ptTest is in client coordinates
	//							OX_MNU_SCREEN		-	ptTest is in screen coordinates
	//							OX_MNU_ZEROBASED	-	ptTest is in coordinates where 
	//											TopLeft point of menubar window
	//											is the origin point.
	// --- Out :	
	// --- Returns:	the index of the corresponding menu item or one of the 
	//				following values:
	//
	//				OX_MNU_ICON		-	ptTest is over the icon rectangle
	//				OX_MNU_CLOSEBTN	-	ptTest is over the close button rectangle
	//				OX_MNU_RESTOREBTN	-	ptTest is over the restore button rectangle
	//				OX_MNU_MINIMIZEBTN	-	ptTest is over the minimize button rectangle
	//				OX_MNU_NONE		-	ptTest is in void space
	//
	// --- Effect : Retrieves the item that is under the specified point
	int HitTest(const CPoint& ptTest, HITTESTORIGIN Origin=OX_MNU_ZEROBASED) const;

	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Redraws the menu bar. If a menu bar is changed after 
	//				Windows has created the window, call this function to 
	//				draw the changed menu bar.
	inline void DrawMenuBar() 
	{ 
		if(::IsWindow(GetSafeHwnd()))
		{
			RecreateMenuBar(TRUE);
		}
	}

	
	// --- In  :	nChar	-	Specifies the virtual-key code of the given key.
	// --- Out :	
	// --- Returns:	TRUE if key was handled, or FALSE otherwise
	// --- Effect : Handles key pressed by user (accelerator key or 
	//				navigate keys)
	BOOL HandleMenuKey(UINT nChar);

protected:
	// virtual function for designating some space. We book space for
	// close, restore and minimize buttons.
	virtual void BookSpace(CRect& rectBookedSpace, DWORD dwMode);
	// Draw in Booked space 
	virtual void DrawInBookedSpace(CDC& dc, CRect& rectToDrawIn);

	// function called all the time when menubar changes its state from
	// docking to floating and vise versa
	virtual void OnFloatingDocking(BOOL bFloating);

	void UpdateMenuMetrics(BOOL bRedraw=TRUE);

	// function called everytime when toolbar changes its docking side
	virtual void OnChangeDockSide(DWORD dwDockSide);

	// saves/restores the contents of a menu in/from registry
	BOOL SaveMenuContents(HMENU hMenu, CArchive& ar);
	BOOL LoadMenuContents(HMENU hMenu, CArchive& ar);

	// Saves the customized menu in case it has been modified
	BOOL SaveCustomizedMenu();

	// Generated message map functions
protected:
	CString GetItemText(int iIndex);
	// overwrite standard handlers to overcome some problems with MFC
	// standard painting routine
	//{{AFX_MSG(COXMenuBar)
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
#if _MSC_VER >= 1400
	afx_msg LRESULT OnNcHitTest(CPoint point);
#else
	afx_msg UINT OnNcHitTest(CPoint point);
#endif
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LONG OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateMenuItem(CCmdUI* pCmd);
	afx_msg BOOL OnMenuDropDown(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg LONG OnDisplayPopupMenu(WPARAM wParam, LPARAM lParam);

	afx_msg void OnCustTBDelete();

	// reflect the messages to let the MenuBar provide 
	// customization functionality
	afx_msg BOOL OnTBNBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	// drag and drop support
	virtual LONG OnDragOver(WPARAM wParam, LPARAM lParam);
	virtual LONG OnDrop(WPARAM wParam, LPARAM lParam);

	// handler that is called when button is to be removed due to to 
	// drag'n'drop operation
	virtual void OnRemoveDraggedButton(int nButtonIndex);

	virtual void RetrieveDragDropMenuItem(BYTE*& lpData, HMENU hMenu, 
		int nButtonIndex);
};


/////////////////////////////////////////////////////////////////////////


template<class PARENTWND>
class COXMenuBarHost : public PARENTWND
{
public:
	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXMenuBarHost();


// Operations
protected:
	// --- In  :	hMenu		-	valid handle of the menu that will be set
	//								to menu bar
	//				pWnd		-	Pointer to the window that is the 
	//								menubar's parent.
	//				dwStyle		-	The menubar style. Refer to decription
	//								of CToolBar::Create() function for details
	//				nID			-	The toolbar's child-window ID.
	// --- Out : 
	// --- Returns:	TRUE if menubar was successfully created, 
	//				or FALSE otherwise
	// --- Effect : Creates the menubar
	virtual BOOL OnCreateMenuBar(HMENU hMenu, CWnd* pWnd, 
		DWORD dwStyle, UINT nID);

// Attributes
public:

	// global map of all menu bar frame windows
	static CMap<HWND,HWND,void*,void*> g_arrMenuBarFrames;

	// handle of the old mouse hook procedure
	static HHOOK g_pfnOldMouseHookProc;
	// handle of the old keyboard hook procedure
	static HHOOK g_pfnOldKeyboardHookProc;
	/////////////////

protected:
	// flag that specifies that the window is CFrameWnd derived 
	BOOL m_bIsFrame;

	// flag that specifies that the window is MDIFrame
	BOOL m_bIsMDI;

	// menubar object
	COXMenuBar m_menuBar;

	// default menu of MDIFrame window
	HMENU m_hDefaultMenu;

	// flag that specifies that currently selected item in popup menu is 
	// popup item
	BOOL m_bIsPopupMenuItem;
	// array of currently displayed popup menus
	CArray<HMENU,HMENU> m_arrPopupMenu;

public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXMenuBarHost();

	
	// --- In  :	nCode	-	Specifies a code the hook procedure uses 
	//							to determine how to process the mouse message. 
	//							If nCode is less than zero, the rocedure must 
	//							pass the message to the CallNextHookEx function 
	//							without further processing and should return 
	//							the value returned by CallNextHookEx. 
	//				wParam	-	Specifies the identifier of the mouse message. 
	//				lParam	-	Pointer to a MOUSEHOOKSTRUCT structure. 
	// --- Out : 
	// --- Returns:	TRUE if the message has been handled, or FALSE otherwise
	// --- Effect : Handles all mouse messages in the current thread
	virtual BOOL HandleMouseMsg(int nCode, WPARAM wParam, LPARAM lParam);

	
	// --- In  :	nCode	-	Specifies a code the hook procedure uses 
	//							to determine how to process the mouse message. 
	//							If nCode is less than zero, the rocedure must 
	//							pass the message to the CallNextHookEx function 
	//							without further processing and should return 
	//							the value returned by CallNextHookEx. 
	//				wParam	-	Specifies the virtual-key code of the key that 
	//							generated the keystroke message
	//				lParam	-	Specifies the repeat count, scan code, 
	//							extended-key flag, context code, previous 
	//							key-state flag, and transition-state flag
	// --- Out : 
	// --- Returns:	TRUE if the message has been handled, or FALSE otherwise
	// --- Effect : Handles all keyboard messages in the current thread
	virtual BOOL HandleKeyboardMsg(int nCode, WPARAM wParam, LPARAM lParam);

	// --- In  :	
	// --- Out : 

	// --- Returns:	reference to COXMenuBar object used to represent 
	//				the menu bar
	// --- Effect :	Retrieves menu bar object
	virtual COXMenuBar& GetMenuBar();

	// --- In  :	
	// --- Out : 
	// --- Returns:	
	// --- Effect :	Redraws menu bar
	virtual void DrawMenuBar();

	// --- In  :	
	// --- Out : 
	// --- Returns:	pointer to the window menu
	// --- Effect :	Retrieves window's menu
	virtual CMenu* GetMenu();

	// --- In  :	pMenu	-	pointer to the new window menu
	// --- Out : 
	// --- Returns:	TRUE id succeeded or FALSE otherwise
	// --- Effect :	Sets window's menu
	virtual BOOL SetMenu(CMenu* pMenu);


	// --- In  :	nHiliteItemIndex	-	Specifies the menu item to be highlighted
	//				bHilite				-	Specifies whether the menu item is 
	//										highlighted or the highlight is removed. 
	// --- Out : 
	// --- Returns:	TRUE if succeed; FALSE otherwise
	// --- Effect :	Highlights or removes the highlight from a top-level 
	//				(menu-bar) menu item
	BOOL HiliteMenuItem(int nHiliteItemIndex, BOOL bHilite);


	// mark the menu as the changed one
	void MarkMenuAsChanged();


protected:
	// --- In  :	msg	-	message ID
	//				wp	-	WPARAM
	//				lp	-	LPARAM
	// --- Out : 
	// --- Returns:	result of message handling. Different for different messages.
	// --- Effect :	Handle all messages that go to the window
	virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);	

	// helper function to add the object to global list of existing 
	// COXMenuBarHost objects
	void AddToGlobalList();

	// helper function to remove the object from the global list of existing 
	// COXMenuBarHost objects
	void RemoveFromGlobalList();

	// hook procedure for mouse messages
	static LRESULT CALLBACK MenuMouseHookProc(int nCode, WPARAM wParam, 
		LPARAM lParam);
	// hook procedure for keyboard messages
	static LRESULT CALLBACK MenuKeyboardHookProc(int nCode, WPARAM wParam, 
		LPARAM lParam);
};



// global map of all menu bar frame windows
template<class PARENTWND> 
CMap<HWND,HWND,void*,void*> COXMenuBarHost<PARENTWND>::g_arrMenuBarFrames;

// handle of the old mouse hook procedure
template<class PARENTWND> 
HHOOK COXMenuBarHost<PARENTWND>::g_pfnOldMouseHookProc=NULL;
// handle of the old keyboard hook procedure
template<class PARENTWND> 
HHOOK COXMenuBarHost<PARENTWND>::g_pfnOldKeyboardHookProc=NULL;
///////////////////////////////////////////


template<class PARENTWND> 
COXMenuBarHost<PARENTWND>::COXMenuBarHost()
{
	// has to be derived from CWnd or its derivates
	if(!IsKindOf(RUNTIME_CLASS(CWnd)))
	{
		TRACE(_T("COXMenuBarHost has to be derived from CFrameWnd or its derivates\n"));
		AfxThrowNotSupportedException();
	}

	// is it CFrameWnd derived
	m_bIsFrame=IsKindOf(RUNTIME_CLASS(CFrameWnd));

	// is it MDI
	m_bIsMDI=IsKindOf(RUNTIME_CLASS(CMDIFrameWnd));

	m_hDefaultMenu=NULL;

	m_bIsPopupMenuItem=FALSE;
}

template<class PARENTWND>
COXMenuBarHost<PARENTWND>::~COXMenuBarHost()
{
	if(::IsWindow(GetSafeHwnd()))
	{
		DestroyWindow();
	}

	ASSERT(g_arrMenuBarFrames.GetCount()>0 || g_pfnOldMouseHookProc==NULL);
	ASSERT(g_arrMenuBarFrames.GetCount()>0 || g_pfnOldKeyboardHookProc==NULL);
}


template<class PARENTWND>
BOOL COXMenuBarHost<PARENTWND>::OnCreateMenuBar(HMENU hMenu, CWnd* pWnd, 
												DWORD dwStyle, UINT nID)
{
	ASSERT(pWnd!=NULL);
	ASSERT(::IsWindow(pWnd->m_hWnd));

	if(hMenu==NULL)
		dwStyle&=~WS_VISIBLE;

	if(!GetMenuBar().Create(pWnd,dwStyle,nID))
	{
		TRACE(_T("COXMenuBarHost<PARENTWND>::OnCreateMenuBar: failed to create menu bar\n"));
		return FALSE;
	}
	if(!GetMenuBar().SetMenu(hMenu))
	{
		TRACE(_T("COXMenuBarHost<PARENTWND>::OnCreateMenuBar: failed to set menu to menu bar\n"));
		return FALSE;
	}

	if(m_bIsFrame)
	{
		((CFrameWnd*)this)->m_hMenuDefault=hMenu;
	}


	return TRUE;
}

template<class PARENTWND>
LRESULT COXMenuBarHost<PARENTWND>::WindowProc(UINT msg, WPARAM wp, LPARAM lp)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	ASSERT(::IsWindow(m_hWnd));

	switch(msg) 
	{
	// handle the creation of the frame
	case WM_CREATE:
		{
			// call parent implementation
			LRESULT lResult=PARENTWND::WindowProc(msg,wp,lp);
			if(lResult!=-1)
			{
				// there shouldn't be any menu bar created at the moment
				ASSERT(!::IsWindow(GetMenuBar().GetSafeHwnd()));

				AddToGlobalList();

				if(::IsWindow(GetMenuBar().GetSafeHwnd()))
				{
					GetMenuBar().DestroyWindow();
				}

				// retrieve default menu
				HMENU hMenu=::GetMenu(m_hWnd);
				if(hMenu==NULL || !::IsMenu(hMenu))
				{
					if(m_bIsMDI)
					{
						TRACE(_T("COXMenuBarHost<PARENTWND>::WindowProc: There is no menu associated with frame window\n"));
					}
					m_hDefaultMenu=NULL;
				}
				else
				{
					// save default menu
					m_hDefaultMenu=hMenu;
					// remove menu from window
					SetMenu(NULL);
				}

				// for SDI frame window it's important to set application
				// main window right now
				if(AfxGetApp()->m_pMainWnd==NULL && !m_bIsMDI && m_bIsFrame && 
					(GetStyle() & WS_CHILD)!=WS_CHILD)
				{
					AfxGetApp()->m_pMainWnd=this;
				}

				// create menu bar and set the menu
				if(!OnCreateMenuBar(m_hDefaultMenu,this,
					WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_SIZE_DYNAMIC|CBRS_GRIPPER,AFX_IDW_MENUBAR))
				{
					TRACE(_T("COXMenuBarHost<PARENTWND>::WindowProc: failed to initialize COXMenuBar object\n"));
					return lResult;
				}
	
				// setup hooks for mouse and keyboard messages
				if(g_pfnOldMouseHookProc==NULL)
				{
					g_pfnOldMouseHookProc=::SetWindowsHookEx(
						WH_MOUSE,MenuMouseHookProc,NULL,::GetCurrentThreadId());
				}
				if(g_pfnOldKeyboardHookProc==NULL)
				{
					g_pfnOldKeyboardHookProc=::SetWindowsHookEx(
						WH_KEYBOARD,MenuKeyboardHookProc,NULL,::GetCurrentThreadId());
				}
				//////////////////////////////////////////////
			}

			return lResult;
		}
	
	case WM_DESTROY:
		{
			RemoveFromGlobalList();

			// reset the menu customized mode
			if(::IsWindow(GetMenuBar().GetSafeHwnd()))
			{
				GetMenuBar().SetAdvancedCustomizationMode(FALSE);
			}

			// destroy menu bar
			if(::IsWindow(GetMenuBar().GetSafeHwnd()))
			{
				GetMenuBar().DestroyWindow();
			}

			if(g_arrMenuBarFrames.GetCount()==0)
			{
				// unhook mouse messages
				if(g_pfnOldMouseHookProc!=NULL)
				{
					VERIFY(::UnhookWindowsHookEx(g_pfnOldMouseHookProc));
					g_pfnOldMouseHookProc=NULL;
				}

				// unhook keyboard messages
				if(g_pfnOldKeyboardHookProc!=NULL)
				{
					VERIFY(::UnhookWindowsHookEx(g_pfnOldKeyboardHookProc));
					g_pfnOldKeyboardHookProc=NULL;
				}
			}

			break;
		}
	case WM_INITMENUPOPUP:
		{
			m_bIsPopupMenuItem=FALSE;
			m_arrPopupMenu.Add((HMENU)wp);

			WORD hiWord=HIWORD(lp);
			WORD loWord=LOWORD(lp);
			if(hiWord==0)
			{
				if(::IsWindow(GetMenuBar().GetSafeHwnd()))
				{
					loWord=(WORD)GetMenuBar().m_nActiveMenuItem;
				}
			}
			lp=MAKELPARAM(loWord,hiWord);
			
#if _MFC_VER>0x0421
#ifndef _AFX_NO_OLE_SUPPORT
			if(m_bIsFrame)
			{
				CFrameWnd* pFrameWnd=DYNAMIC_DOWNCAST(CFrameWnd,this);
				ASSERT(pFrameWnd!=NULL);
				if(pFrameWnd->m_pNotifyHook!=NULL)
				{
					if(pFrameWnd->m_pNotifyHook->OnInitMenuPopup(
						CMenu::FromHandle((HMENU)wp),LOWORD(lp),HIWORD(lp)))
					{
						return 0;
					}

					if(pFrameWnd->m_pNotifyHook->m_pActiveItem!=NULL)
					{
						CWnd* pWnd=pFrameWnd->m_pNotifyHook->
							m_pActiveItem->GetInPlaceWindow();
						ASSERT(pWnd!=NULL);
						pWnd->SendMessage(msg,wp,lp);
						return 0;
					}
				}
			}
#endif	//	_AFX_NO_OLE_SUPPORT
#endif	//	_MFC_VER>0x0421
			break;
		}

	case WM_MENUSELECT:
		{
			UINT nFlags=HIWORD(wp);
			m_bIsPopupMenuItem=((nFlags&MF_POPUP)!=0 && 
				!(nFlags==0xFFFF && lp==NULL) ? TRUE : FALSE);

#if _MFC_VER>0x0421
#ifndef _AFX_NO_OLE_SUPPORT
			if(m_bIsFrame)
			{
				CFrameWnd* pFrameWnd=DYNAMIC_DOWNCAST(CFrameWnd,this);
				ASSERT(pFrameWnd!=NULL);
				if(pFrameWnd->m_pNotifyHook!=NULL)
				{
					if(pFrameWnd->m_pNotifyHook->m_pActiveItem!=NULL)
					{
						CWnd* pWnd=pFrameWnd->m_pNotifyHook->
							m_pActiveItem->GetInPlaceWindow();
						ASSERT(pWnd!=NULL);
						pWnd->SendMessage(msg,wp,lp);
					}
				}
			}
#endif	//	_AFX_NO_OLE_SUPPORT
#endif	//	_MFC_VER>0x0421

			if((nFlags&MF_HILITE)!=0)
			{
				while(m_arrPopupMenu.GetSize()>0 && 
					m_arrPopupMenu[m_arrPopupMenu.GetSize()-1]!=(HMENU)lp)
				{
					m_arrPopupMenu.RemoveAt(m_arrPopupMenu.GetSize()-1);
				}
			}

			break;
		}

	case WM_EXITMENULOOP:
		{
			if(wp==0)
			{
				m_arrPopupMenu.RemoveAll();
			}
			break;
		}

	case WM_COMMAND:
		{
			if(HIWORD(wp)==0)
			{
				int nCmdID=LOWORD(wp);
				if(nCmdID==ID_OX_SHOWALLITEMS)
				{
					GetMenuBar().m_nActivateNextItem=GetMenuBar().m_nActiveMenuItem;
				}
			}
			break;
		}

	case WM_OX_MENUCHANGED:
		{
			MarkMenuAsChanged();
			break;
		}
	}

	// I don't handle it: pass along
	return PARENTWND::WindowProc(msg,wp,lp);
}


template<class PARENTWND>
void COXMenuBarHost<PARENTWND>::MarkMenuAsChanged()
{
	// mark the current menu as a changed one
	GetMenuBar().MarkAsChanged();
}


template<class PARENTWND>
void COXMenuBarHost<PARENTWND>::AddToGlobalList()
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	g_arrMenuBarFrames.SetAt(GetSafeHwnd(),(void*)this);
}


template<class PARENTWND>
void COXMenuBarHost<PARENTWND>::RemoveFromGlobalList()
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	g_arrMenuBarFrames.RemoveKey(GetSafeHwnd());
}

// last mouse cursor position when hew menubar item was activated 
static CPoint g_ptLast(-1,-1);

template<class PARENTWND>
BOOL COXMenuBarHost<PARENTWND>::HandleMouseMsg(int nCode, 
											   WPARAM wParam, 
											   LPARAM lParam)
{
	if(!::IsWindow(m_hWnd) || GetMenuBar().IsInAdvancedCustomizationMode())
	{
		return FALSE;
	}

	if(nCode==HC_ACTION && ::IsWindow(GetMenuBar().GetSafeHwnd()))
	{
		MOUSEHOOKSTRUCT* pMHS=(MOUSEHOOKSTRUCT*)lParam;
		// there should be an active menu bar at the moment
		if(GetMenuBar().IsInDisplayingMode() && 
			pMHS->hwnd!=GetMenuBar().GetSafeHwnd())
		{
			CRect rect;
			GetMenuBar().GetWindowRect(rect);
			// handle WM_MOUSEMOVE and WM_LBUTTONDOWN messages
			if((wParam==WM_MOUSEMOVE ||	wParam==WM_LBUTTONDOWN) &&
				rect.PtInRect(pMHS->pt) &&  
				::WindowFromPoint(pMHS->pt)==GetMenuBar().GetSafeHwnd())
			{
				CPoint point=pMHS->pt;
				GetMenuBar().ScreenToClient(&point);
				// test the mouse cursor point
				int hitTest=GetMenuBar().HitTest(point,OX_MNU_CLIENT);

				if(wParam==WM_MOUSEMOVE && g_ptLast!=point &&
					hitTest!=GetMenuBar().m_nActiveMenuItem && 
					(hitTest>=0 || hitTest==OX_MNU_ICON) &&
					!(GetMenuBar().m_nActiveMenuItem==
					::GetMenuItemCount(GetMenuBar().m_hMenu) && 
					hitTest==OX_MNU_ICON))
				{
					// set the menu item that will be activated next
					GetMenuBar().m_nActivateNextItem=(hitTest!=OX_MNU_ICON) ? hitTest : 
						::GetMenuItemCount(GetMenuBar().m_hMenu);

					// emulate mouse pressing
					::PostMessage(pMHS->hwnd,WM_LBUTTONDOWN,MK_LBUTTON,
						MAKELPARAM(point.x,point.y));
					::PostMessage(pMHS->hwnd,WM_LBUTTONUP,MK_LBUTTON,
						MAKELPARAM(point.x,point.y));
	
					return TRUE;
				}
				else if(wParam==WM_LBUTTONDOWN && (hitTest>=0 || hitTest==OX_MNU_ICON))
				{
					// set the forbidden item
					GetMenuBar().m_nForbiddenItem=(hitTest!=OX_MNU_ICON) ? hitTest : 
						::GetMenuItemCount(GetMenuBar().m_hMenu);
				}

				g_ptLast=point;
			}
		}
	}

    return FALSE;
}


// flag that ALT was pressed down
static BOOL g_altDownNoOtherKey=FALSE;

template<class PARENTWND>
BOOL COXMenuBarHost<PARENTWND>::
HandleKeyboardMsg(int nCode, WPARAM wParam, LPARAM lParam)
{
	ASSERT(::IsWindow(m_hWnd));

	if(nCode==HC_ACTION && ::IsWindow(GetMenuBar().GetSafeHwnd()) && 
		IsWindowEnabled() && ::GetActiveWindow()==GetSafeHwnd())
	{
		// handle differently keyboard messages when menubar is in 
		// displaying mode
		if(!GetMenuBar().IsInDisplayingMode())
		{
			if(wParam==VK_MENU)
			{
				if(lParam>=0)
				{
					if(!(lParam&(1<<30)))
					{
						g_altDownNoOtherKey=TRUE;
						return TRUE;
					}
				}
				else if(g_altDownNoOtherKey)
				{
					// ALT key was pressed
					GetMenuBar().HandleMenuKey(0);
					g_altDownNoOtherKey=FALSE;
					return TRUE;
				}
			}

			if(wParam!=VK_MENU && (::GetAsyncKeyState(VK_MENU)<0 || 
				GetMenuBar().IsInSurfingMode()))
			{
				g_altDownNoOtherKey=FALSE;
				if((lParam&0x80000000)==0 && int(lParam)>=0 && 
					::GetAsyncKeyState(VK_F4)>=0)
				{
					// handle the key 
					if(GetMenuBar().HandleMenuKey(PtrToUint(wParam)))
						return TRUE;
				}
			}
		}
		else
		{
			// handle only VK_LEFT, VK_RIGHT and VK_ESCAPE in the specific way 
			// while in displaying mode
			if(int(lParam)>=0 && ((wParam==VK_LEFT && m_arrPopupMenu.GetSize()<=1) || 
				(wParam==VK_RIGHT && !m_bIsPopupMenuItem) || 
				(wParam==VK_ESCAPE && m_arrPopupMenu.GetSize()==1)))
			{
				// handle the key 
				if(GetMenuBar().HandleMenuKey(PtrToUint(wParam)))
					return TRUE;
			}
		}
	}

    return FALSE;
}


template<class PARENTWND>
COXMenuBar& COXMenuBarHost<PARENTWND>::GetMenuBar()
{
	return m_menuBar;
}


template<class PARENTWND>
void COXMenuBarHost<PARENTWND>::DrawMenuBar()
{
	if(::IsWindow(GetMenuBar().GetSafeHwnd()))
		GetMenuBar().DrawMenuBar();
	else
		PARENTWND::DrawMenuBar();
}


template<class PARENTWND>
CMenu* COXMenuBarHost<PARENTWND>::GetMenu()
{
	if(::IsWindow(GetMenuBar().GetSafeHwnd()))
		return CMenu::FromHandle(GetMenuBar().GetMenu());
	else
		return PARENTWND::GetMenu();
}


template<class PARENTWND>
BOOL COXMenuBarHost<PARENTWND>::SetMenu(CMenu* pMenu)
{
	if(::IsWindow(GetMenuBar().GetSafeHwnd()))
		return GetMenuBar().SetMenu(pMenu==NULL ? NULL : pMenu->GetSafeHmenu());
	else
		return PARENTWND::SetMenu(pMenu);
}


template<class PARENTWND>
BOOL COXMenuBarHost<PARENTWND>::HiliteMenuItem(int nHiliteItemIndex, BOOL bHilite)
{
	if(GetMenuBar().IsInDisplayingMode())
	{
		return FALSE;
	}
	
	return GetMenuBar().SetSurfingMode(bHilite,nHiliteItemIndex);
}


template<class PARENTWND>
LRESULT CALLBACK COXMenuBarHost<PARENTWND>::
MenuMouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	ASSERT(g_pfnOldMouseHookProc!=NULL);

	if(nCode<0)
	{
		// have to call the default implementation
	    return ::CallNextHookEx(g_pfnOldMouseHookProc,nCode,wParam,lParam);
	}

	DWORD dwMessagePos=::GetMessagePos();
	POINTS points=MAKEPOINTS(dwMessagePos);
	CPoint ptLast(points.x,points.y);
	CWnd* pWnd=WindowFromPoint(ptLast);
	ASSERT(pWnd!=NULL);
	CWnd* pTopLevelWnd=pWnd->GetTopLevelParent();
	ASSERT(pTopLevelWnd!=NULL);

	ASSERT(g_arrMenuBarFrames.GetCount()>0);
	POSITION pos=g_arrMenuBarFrames.GetStartPosition();
	while(pos!=NULL)
	{
		HWND hWnd=NULL;
		void* pMenuBarFrame=NULL;
		g_arrMenuBarFrames.GetNextAssoc(pos,hWnd,pMenuBarFrame);
		ASSERT(pMenuBarFrame!=NULL);
		// call COXMenuBarHost implementation
		if(::IsWindow(((COXMenuBarHost<PARENTWND>*)pMenuBarFrame)->m_hWnd) && 
			(pMenuBarFrame==pTopLevelWnd))
		{
			if(((COXMenuBarHost<PARENTWND>*)pMenuBarFrame)->
				HandleMouseMsg(nCode,wParam,lParam))
			{
				return 0;
			}
		}
	}

    return ::CallNextHookEx(g_pfnOldMouseHookProc,nCode,wParam,lParam);
}


template<class PARENTWND>
LRESULT CALLBACK COXMenuBarHost<PARENTWND>::
MenuKeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	ASSERT(g_pfnOldKeyboardHookProc!=NULL);

	if(nCode<0)
	{
		// have to call the default implementation
	    return ::CallNextHookEx(g_pfnOldKeyboardHookProc,nCode,wParam,lParam);
	}

	CWnd* pTopLevelWnd=NULL;
	CWnd* pWnd=GetFocus();
	if(pWnd!=NULL)
	{
		pTopLevelWnd=pWnd->GetTopLevelParent();
	}
	else
	{
		pTopLevelWnd=CWnd::GetDesktopWindow();
	}
	ASSERT(pTopLevelWnd!=NULL);

	ASSERT(g_arrMenuBarFrames.GetCount()>0);
	POSITION pos=g_arrMenuBarFrames.GetStartPosition();
	while(pos!=NULL)
	{
		HWND hWnd=NULL;
		void* pMenuBarFrame=NULL;
		g_arrMenuBarFrames.GetNextAssoc(pos,hWnd,pMenuBarFrame);
		ASSERT(pMenuBarFrame!=NULL);
		// call COXMenuBarHost implementation
		if(::IsWindow(((COXMenuBarHost<PARENTWND>*)pMenuBarFrame)->m_hWnd) && 
			pMenuBarFrame==pTopLevelWnd)
		{
			if(((COXMenuBarHost<PARENTWND>*)pMenuBarFrame)->
				HandleKeyboardMsg(nCode,wParam,lParam))
			{
				return 1;
			}
		}
	}

    return ::CallNextHookEx(g_pfnOldKeyboardHookProc,nCode,wParam,lParam);
}


/////////////////////////////////////////////////////////////////////////

template<class PARENTDOCKBAR>
class COXMenuDockBar : public PARENTDOCKBAR
{
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXMenuDockBar();


// Operations

	// --- In  :	bStretch	-	Indicates whether the bar should be 
	//								stretched to the size of the frame. 
	//				bHorz		-	Indicates that the bar is horizontally 
	//								or vertically oriented. The bHorz parameter 
	//								is nonzero if the bar is horizontally 
	//								oriented and is 0 if it is vertically 
	//								oriented.
	// --- Out : 
	// --- Returns:	The dock bar size, in pixels, of a CSize object
	// --- Effect : Calculates size of the dock bar
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);

public:
	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXMenuDockBar();
};


// constructor
template<class PARENTDOCKBAR>
COXMenuDockBar<PARENTDOCKBAR>::COXMenuDockBar()
{
	// has to be derived from CDockBar or its derivates
	if(!IsKindOf(RUNTIME_CLASS(CDockBar)))
	{
		TRACE(_T("COXMenuDockBar has to be derived from CDockBar or its derivations\n"));
		AfxThrowNotSupportedException();
	}
}

// destructor
template<class PARENTDOCKBAR>
COXMenuDockBar<PARENTDOCKBAR>::~COXMenuDockBar()
{
}

// mostly MFC code
template<class PARENTDOCKBAR>
CSize COXMenuDockBar<PARENTDOCKBAR>::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	// scan all the control bars
	int nPos=0;
	for (nPos=0; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar=GetDockedControlBar(nPos);
		if(pBar!=NULL && ::IsWindow(pBar->m_hWnd) && pBar->IsVisible() && 
			pBar->IsKindOf(RUNTIME_CLASS(COXMenuBar)) &&
			((COXMenuBar*)pBar)->GetTakeEntireRow())
		{
			((COXMenuBar*)pBar)->m_bForceEntireRow=TRUE;
		}
	}

	// call the parent implementation
	CSize sizeFixed=PARENTDOCKBAR::CalcFixedLayout(bStretch,bHorz);
	CPoint pt(-afxData.cxBorder2, -afxData.cyBorder2);

	// get max size
	CSize sizeMax;
	if(!m_rectLayout.IsRectEmpty())
	{
		sizeMax=m_rectLayout.Size();
	}
	else
	{
		CRect rectFrame;
		CFrameWnd* pFrame=GetParentFrame();
		ASSERT(pFrame!=NULL);
		pFrame->GetClientRect(rectFrame);
		pFrame->ClientToScreen(rectFrame);

		for(int nID=AFX_IDW_CONTROLBAR_FIRST; nID<=AFX_IDW_CONTROLBAR_LAST; nID++)
		{
			CControlBar* pBar=m_pDockSite->GetControlBar(nID);

			if(pBar!=NULL && pBar->IsDockBar() && 
				pBar->IsVisible() && !pBar->IsFloating())
			{
				CRect rectBar;
				pBar->GetWindowRect(rectBar);
				if(pBar->GetStyle() & CBRS_TOP)
				{
					rectFrame.top=__max(rectFrame.top,rectBar.bottom);
				}
				else if(pBar->GetStyle() & CBRS_BOTTOM)
				{
					rectFrame.bottom=__min(rectFrame.bottom,rectBar.top);
				}
			}
		}

		sizeMax=rectFrame.Size();
	}

	// prepare for layout
    AFX_SIZEPARENTPARAMS layout;
    layout.hDWP=m_bLayoutQuery ? NULL : 
		::BeginDeferWindowPos(PtrToInt(m_arrBars.GetSize()));
	
	BOOL bAdjustBars=FALSE;
	CSize sizeOffset(0,0);
	// scan all the control bars
	for (nPos=0; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar=GetDockedControlBar(nPos);
		if(pBar!=NULL && ::IsWindow(pBar->m_hWnd) && pBar->IsVisible() && 
			pBar->IsKindOf(RUNTIME_CLASS(COXMenuBar)) &&
			((COXMenuBar*)pBar)->GetTakeEntireRow())
		{
			// get ideal rect for bar
			DWORD dwMode=0;
			if((pBar->m_dwStyle & CBRS_SIZE_DYNAMIC) &&
				(pBar->m_dwStyle & CBRS_FLOATING))
				dwMode |= LM_HORZ | LM_MRUWIDTH;
			else if(pBar->m_dwStyle & CBRS_ORIENT_HORZ)
				dwMode |= LM_HORZ | LM_HORZDOCK;
			else
				dwMode |=  LM_VERTDOCK;

			if((dwMode&LM_HORZDOCK) || (dwMode&LM_VERTDOCK))
			{
				// get current rect for bar
				CRect rectBar;
				pBar->GetWindowRect(&rectBar);
				ScreenToClient(&rectBar);

				CSize sizeBar(rectBar.Width(),rectBar.Height());
				CRect rect(pt,sizeBar);

				if(bHorz)
				{
					rect.right=sizeMax.cx+afxData.cxBorder2;
					rect.OffsetRect(0,rectBar.top-rect.top);

					// check if menubar was droped on the same row with other
					// control bar(s)
					if(rectBar.left>rect.left)
					{
						ASSERT(nPos>0);
						int nPrevPos=nPos-1;
						CControlBar* pPrevBar=NULL;
						while(TRUE)
						{
							ASSERT(nPrevPos>=0);
							pPrevBar=GetDockedControlBar(nPrevPos);
							if(pPrevBar==NULL || 
								!::IsWindow(pPrevBar->GetSafeHwnd()) || 
								pPrevBar->IsVisible())
							{
								break;
							}
							nPrevPos--;
						}

						if(pPrevBar!=NULL)
						{
							CRect rectPrevBar;
							pPrevBar->GetWindowRect(&rectPrevBar);
							rect.OffsetRect(0,rectPrevBar.Height()-
								afxData.cyBorder2);
							sizeOffset.cy+=__min(rectBar.Height(),
								rectPrevBar.Height())-afxData.cyBorder2;
							m_arrBars.InsertAt(nPos, (CObject*)NULL);
							nPos++;
						}
					}

					// mark the end of the row
					CControlBar* pNextBar=NULL;
					int nNextPos=nPos+1;
					while(nNextPos<m_arrBars.GetSize())
					{
						pNextBar=GetDockedControlBar(nNextPos);
						if(pNextBar==NULL || !::IsWindow(pNextBar->GetSafeHwnd()) || 
							pNextBar->IsVisible())
							break;
						nNextPos++;
					}

					if(pNextBar!=NULL)
					{
						m_arrBars.InsertAt(nPos+1, (CObject*)NULL);
						CRect rectNextBar;
						pNextBar->GetWindowRect(&rectNextBar);
						sizeOffset.cy+=rectNextBar.Height()-afxData.cyBorder2;
					}
				}
				else
				{
					rect.bottom=sizeMax.cy;
					rect.OffsetRect(rectBar.left-rect.left,0);

					// check if menubar was droped on the same row with other
					// control bar(s)
					if(rectBar.top>rect.top)
					{
						ASSERT(nPos>0);
						int nPrevPos=nPos-1;
						CControlBar* pPrevBar=NULL;
						while(TRUE)
						{
							ASSERT(nPrevPos>=0);
							pPrevBar=GetDockedControlBar(nPrevPos);
							if(pPrevBar==NULL || 
								!::IsWindow(pPrevBar->GetSafeHwnd()) || 
								pPrevBar->IsVisible())
								break;
							nPrevPos--;
						}

						if(pPrevBar!=NULL)
						{
							CRect rectPrevBar;
							pPrevBar->GetWindowRect(&rectPrevBar);
							rect.OffsetRect(rectPrevBar.Width()-afxData.cxBorder2,0);
							sizeOffset.cx+=__min(rectBar.Width(),
								rectPrevBar.Width())-afxData.cxBorder2;
							m_arrBars.InsertAt(nPrevPos+1, (CObject*)NULL);
							nPos++;
						}
					}

					// mark the end of row
					CControlBar* pNextBar=NULL;
					int nNextPos=nPos+1;
					while(nNextPos<m_arrBars.GetSize())
					{
						CControlBar* pNextBar=GetDockedControlBar(nNextPos);
						if(pNextBar==NULL || !::IsWindow(pNextBar->GetSafeHwnd()) || 
							pNextBar->IsVisible())
							break;
						nNextPos++;
					}

					if(pNextBar!=NULL)
					{
						m_arrBars.InsertAt(nPos+1, (CObject*)NULL);
					}
				}

				if(sizeOffset.cx!=0 || sizeOffset.cy!=0)
				{
					bAdjustBars=TRUE;
				}

				if((bHorz && rectBar.Width()!=rect.Width() && 
					!(rectBar.Width()>rect.Width())) || 
					(!bHorz && rectBar.Height()!=rect.Height() && 
					!(rectBar.Height()>rect.Height())))
				{
					AfxRepositionWindow(&layout, pBar->m_hWnd, &rect);
				}
			}
		}
		else if(pBar!=NULL && bAdjustBars)
		{
			// get current rect for bar
			CRect rectBar;
			pBar->GetWindowRect(&rectBar);
			ScreenToClient(&rectBar);
			rectBar+=sizeOffset;
			AfxRepositionWindow(&layout, pBar->m_hWnd, &rectBar);
		}
	}

	if(!m_bLayoutQuery)
	{
		if(layout.hDWP==NULL || !::EndDeferWindowPos(layout.hDWP))
		{
			TRACE(_T("Warning: DeferWindowPos failed - low system resources\n"));
		}
	}

	if(bAdjustBars)
	{
		sizeFixed+=sizeOffset;
	}


	// scan all the control bars
	for (nPos=0; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar=GetDockedControlBar(nPos);
		if(pBar!=NULL && ::IsWindow(pBar->m_hWnd) && pBar->IsVisible() && 
			pBar->IsKindOf(RUNTIME_CLASS(COXMenuBar)) &&
			((COXMenuBar*)pBar)->GetTakeEntireRow())
		{
			((COXMenuBar*)pBar)->m_bForceEntireRow=FALSE;
		}
	}

	return sizeFixed;
}


template<class PARENTFRAME, class PARENTDOCKBAR>
class COXMenuBarFrame : public COXMenuBarHost<PARENTFRAME>
{
public:
	// dock bar control used internally
	typedef COXMenuDockBar<PARENTDOCKBAR> CInternalDockBar;

	// --- In  :	dwDockStyle		-	The desired styles for the control bar. 
	//									menu bar style must be CBRS_ALIGN_XXX 
	//									or 0 only. Refer to the description of 
	//									CControlBar::SetBarStyle function 
	//									for details.
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXMenuBarFrame(DWORD dwDockStyle=CBRS_ALIGN_ANY, 
		CRuntimeClass* pFloatingFrameClass=RUNTIME_CLASS(CMiniDockFrameWnd));


// Operations

	// --- In  :	dwDockStyle		-	Specifies which sides of the frame 
	//									window can serve as docking sites for 
	//									control bars. Refer to the description
	//									of CFrameWnd::EnableDocking function 
	//									for details.
	// --- Out :	
	// --- Returns:
	// --- Effect : Call this function to enable dockable control bars in a 
	//				frame window. By default, control bars will be docked to a 
	//				side of the frame window in the following order: 
	//				top, bottom, left, right.
	virtual void EnableDocking(DWORD dwDockStyle);


	virtual void RecalcLayout(BOOL bNotify=TRUE);
	

	virtual BOOL LoadFrame(UINT nIDResource, 
		DWORD dwDefaultStyle=WS_OVERLAPPEDWINDOW|FWS_ADDTOTITLE,
		CWnd* pParentWnd=NULL, CCreateContext* pContext=NULL);

	virtual void OnUpdateFrameMenu(HMENU hMenuAlt);

	// --- In  :	bEnable		-	Specifies whether then snapping behaviour
	//								should be turned on or off
	// --- Out :	
	// --- Returns:
	// --- Effect : Call this function to turn on the snaping and tear-off behaviour
	//				of toolbars, menubars and docking windows
	void EnableSnapping(BOOL bEnable = TRUE);

protected:
	// --- In  :	lpcs		-	A pointer to a Windows CREATESTRUCT 
	//								structure.
	//				pContext	-	A pointer to a CCreateContext structure
	// --- Out :	
	// --- Returns:	Nonzero if successful; otherwise 0
	// --- Effect : 
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// Attributes
public:

protected:

	// Specifies whether the snaping and tear-off behaviour of toolbars,
	// menubars and docking windows is enabled
	BOOL m_bEnableSnapping;

	// Specifies which sides of the frame window can serve as docking 
	// sites for menu bar.
	DWORD m_dwDockStyle;

	// address of the window procedure, or a handle representing the address 
	// of the window procedure of MDIClient window
	WNDPROC m_pfnOldMDIClientProc;

	// flag that specifies that MDIClient window is subclassed
	BOOL m_bIsMDISubclassed;

public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXMenuBarFrame();

	
	// --- In  :	hWnd	-	Handle to the window to receive the message. 
	//				msg		-	Specifies the message. 
	//				wp		-	Specifies additional message-specific 
	//							information. The contents of this parameter 
	//							depend on the value of the msg parameter. 
	//				lp		-	Specifies additional message-specific 
	//							information. The contents of this parameter 
	//							depend on the value of the Msg parameter. 
	// --- Out : 
	// --- Returns:	the result of handling of message that was sent to 
	//				MDIClient window
	// --- Effect : Handles all messages that go to MDIClient window
	virtual LRESULT HandleMDIClientMsg(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

protected:
	// --- In  :	msg	-	message ID
	//				wp	-	WPARAM
	//				lp	-	LPARAM
	// --- Out : 
	// --- Returns:	result of message handling. Different for different messages.
	// --- Effect :	Handle all messages that go to the window
	virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);	

	// helper for resizing the menubar
	BOOL CheckMenuBarSize(BOOL bNotify=TRUE);

	// helper function to subclass MDIClient window
	void SubclassMDIClient();

	// handles context help
	afx_msg void OnContextHelp();
	// handles all messages in context help mode
	BOOL ProcessHelpMsg(MSG& msg, DWORD* pContext);

	// subclass window procedure for MDIClient window
	static LRESULT CALLBACK MDISubclassProc(HWND hWnd, UINT msg, 
		WPARAM wp, LPARAM lp);
};



// analogue of CFrameWnd::dwDockBarMap
static const DWORD g_dwDockBarMap[4][2] =
{
	{ AFX_IDW_DOCKBAR_TOP,      CBRS_TOP    },
	{ AFX_IDW_DOCKBAR_BOTTOM,   CBRS_BOTTOM },
	{ AFX_IDW_DOCKBAR_LEFT,     CBRS_LEFT   },
	{ AFX_IDW_DOCKBAR_RIGHT,    CBRS_RIGHT  },
};


template<class PARENTFRAME, class PARENTDOCKBAR>
COXMenuBarFrame<PARENTFRAME,PARENTDOCKBAR>::
COXMenuBarFrame(DWORD dwDockStyle/*=CBRS_ALIGN_ANY*/, 
				CRuntimeClass* pFloatingFrameClass/*=RUNTIME_CLASS(CMiniDockFrameWnd)*/)
{
	// has to be derived from CframeWnd or its derivates
	if(!IsKindOf(RUNTIME_CLASS(CFrameWnd)))
	{
		TRACE(_T("COXMenuBarFrame has to be derived from CFrameWnd or its derivates\n"));
		AfxThrowNotSupportedException();
	}

	// menu bar style must be CBRS_ALIGN_XXX or 0 only
	ASSERT(dwDockStyle==0 || (dwDockStyle & ~CBRS_ALIGN_ANY)==0);
	m_dwDockStyle=dwDockStyle;

	m_pfnOldMDIClientProc=NULL;

	m_bIsMDISubclassed=FALSE;

	m_pFloatingFrameClass=pFloatingFrameClass;

	m_bEnableSnapping = FALSE;
}

template<class PARENTFRAME, class PARENTDOCKBAR>
COXMenuBarFrame<PARENTFRAME,PARENTDOCKBAR>::~COXMenuBarFrame()
{
}


template<class PARENTFRAME, class PARENTDOCKBAR>
BOOL COXMenuBarFrame<PARENTFRAME,PARENTDOCKBAR>::
OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	BOOL bResult=COXMenuBarHost<PARENTFRAME>::OnCreateClient(lpcs,pContext);
	
	AddToGlobalList();

	if(bResult && m_bIsMDI)
		SubclassMDIClient();

	return bResult;
}


template<class PARENTFRAME, class PARENTDOCKBAR>
void COXMenuBarFrame<PARENTFRAME,PARENTDOCKBAR>::RecalcLayout(BOOL bNotify/*=TRUE*/)
{
	COXMenuBarHost<PARENTFRAME>::RecalcLayout(bNotify);
	CheckMenuBarSize(bNotify);
}


template<class PARENTFRAME, class PARENTDOCKBAR>
BOOL COXMenuBarFrame<PARENTFRAME,PARENTDOCKBAR>::
LoadFrame(UINT nIDResource, 
		  DWORD dwDefaultStyle/*=WS_OVERLAPPEDWINDOW|FWS_ADDTOTITLE*/,
		  CWnd* pParentWnd/*=NULL*/, 
		  CCreateContext* pContext/*=NULL*/)
{
	if(COXMenuBarHost<PARENTFRAME>::LoadFrame(nIDResource,dwDefaultStyle,
		pParentWnd,pContext))
	{
		if(::IsWindow(GetMenuBar().GetSafeHwnd()))
			m_hMenuDefault=GetMenuBar().GetMenu();
		return TRUE;
	}

	return FALSE;
}


template<class PARENTFRAME, class PARENTDOCKBAR>
void COXMenuBarFrame<PARENTFRAME,PARENTDOCKBAR>::OnUpdateFrameMenu(HMENU hMenuAlt)
{
	if(!m_bIsMDI)
	{
		if(hMenuAlt==NULL)
		{
			// attempt to get default menu from document
			CDocument* pDoc=GetActiveDocument();
			if(pDoc!=NULL)
				hMenuAlt=pDoc->GetDefaultMenu();
			// use default menu stored in frame if none from document
			if(hMenuAlt==NULL)
				hMenuAlt=m_hMenuDefault;
		}
		// finally, set the menu
		SetMenu(CMenu::FromHandle(hMenuAlt));
	}
	else
	{
		PARENTFRAME::OnUpdateFrameMenu(hMenuAlt);
	}
}


template<class PARENTFRAME, class PARENTDOCKBAR>
void COXMenuBarFrame<PARENTFRAME,PARENTDOCKBAR>::EnableSnapping(BOOL bEnable/*=TRUE*/)
{
	m_bEnableSnapping = bEnable;
}

template<class PARENTFRAME, class PARENTDOCKBAR>
LRESULT COXMenuBarFrame<PARENTFRAME,PARENTDOCKBAR>::WindowProc(UINT msg, 
															   WPARAM wp, 
															   LPARAM lp)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	ASSERT(::IsWindow(((CFrameWnd*)this)->m_hWnd));

	switch(msg) 
	{
	// handle the creation of the frame
	case WM_CREATE:
		{
			// call parent implementation
			LRESULT lResult=COXMenuBarHost<PARENTFRAME>::WindowProc(msg,wp,lp);
			if(lResult!=-1)
			{
				// menu must be already created at the moment
				ASSERT(::IsWindow(GetMenuBar().GetSafeHwnd()));

				if(m_bIsMDI)
					// in order to subclass MDIClient if it's not done yet
					SendMessage(WM_IDLEUPDATECMDUI);

				// setup docking environment
				GetMenuBar().EnableDocking(m_dwDockStyle);
				if(m_dwDockStyle!=0)
				{
					EnableDocking(m_dwDockStyle);
					CRect rect(0,0,0,0);
					DockControlBar(&GetMenuBar(),(UINT)0,&rect);
				}
				//////////////////////////////
			}

			return lResult;
		}
	case WM_COMMAND:
		{
			WORD loWord=LOWORD(wp);
			if(loWord==ID_CONTEXT_HELP)
			{
				OnContextHelp();
				return 0;
			}

#if _MFC_VER>0x0421
#ifndef _AFX_NO_OLE_SUPPORT
			if(m_pNotifyHook!=NULL)
			{
				if(m_pNotifyHook->m_pActiveItem!=NULL)
				{
					CWnd* pWnd=m_pNotifyHook->m_pActiveItem->GetInPlaceWindow();
					ASSERT(pWnd!=NULL);
					if(pWnd->SendMessage(msg,wp,lp))
					{
						return TRUE;
					}
				}
			}
#endif	//	_AFX_NO_OLE_SUPPORT
#endif	//	_MFC_VER>0x0421

			break;
		}
	case WM_IDLEUPDATECMDUI:
		if(m_bIsMDI)
		{
			m_bIsMDISubclassed=FALSE;
			::SendMessage(((CMDIFrameWnd*)this)->m_hWndMDIClient,WM_NULL,0,0);
			if(!m_bIsMDISubclassed)
			{
				TRACE(_T("COXMenuBarFrame: menubar has been removed out of the list of objects that subclass MDIClient\n"));
				SubclassMDIClient();
				if(::GetMenu(m_hWnd)!=NULL)
				{
					SetMenu(NULL);
				}
			}
		}
		break;

	case WM_INITMENU:
		{
			if (GetMenuBar().m_hMenu != NULL)
				wp = (WPARAM) GetMenuBar().m_hMenu;
		}
		break;

	case WM_QUERYSNAPPING:
		{
			return m_bEnableSnapping;
		}
		break;
	}

	// I don't handle it: pass along
		return COXMenuBarHost<PARENTFRAME>::WindowProc(msg,wp,lp);
}


template<class PARENTFRAME, class PARENTDOCKBAR>
void COXMenuBarFrame<PARENTFRAME,PARENTDOCKBAR>::OnContextHelp()
{
	// don't enter twice, and don't enter if initialization fails
	if(m_bHelpMode==HELP_ACTIVE || !CanEnterHelpMode())
		return;

	// don't enter help mode with pending WM_EXITHELPMODE message
	MSG msg;
	if(PeekMessage(&msg, m_hWnd, WM_EXITHELPMODE, WM_EXITHELPMODE,
		PM_REMOVE|PM_NOYIELD))
	{
		return;
	}

	BOOL bHelpMode=m_bHelpMode;
	ASSERT(m_bHelpMode==HELP_INACTIVE || m_bHelpMode==HELP_ENTERING);
	m_bHelpMode=HELP_ACTIVE;

#ifndef _AFX_NO_OLE_SUPPORT
	// allow any in-place active servers to go into help mode
	if(bHelpMode!=HELP_ENTERING && m_pNotifyHook!=NULL &&
		!m_pNotifyHook->OnContextHelp(TRUE))
	{
		TRACE0("Error: an in-place server failed to enter context help mode.\n");
		m_pNotifyHook->OnContextHelp(FALSE);    // undo partial help mode
		m_bHelpMode=HELP_INACTIVE;
		return;
	}
#endif

	if(bHelpMode==HELP_INACTIVE)
	{
		// need to delay help startup until later
		PostMessage(WM_COMMAND, ID_CONTEXT_HELP);
		m_bHelpMode=HELP_ENTERING;
		return;
	}

	ASSERT(m_bHelpMode==HELP_ACTIVE);

	// display special help mode message on status bar
	UINT nMsgSave=(UINT)SendMessage(WM_SETMESSAGESTRING,
		(WPARAM)AFX_IDS_HELPMODEMESSAGE);
	if(nMsgSave==0)
	{
		nMsgSave=AFX_IDS_IDLEMESSAGE;
	}

	DWORD   dwContext=0;
	POINT   point;

	GetCursorPos(&point);
	SetHelpCapture(point, NULL);
	LONG lIdleCount=0;
	CWinApp* pApp=AfxGetApp();

	while(m_bHelpMode)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(!ProcessHelpMsg(msg, &dwContext))
			{
				break;
			}
			ASSERT(dwContext==0);
		}
		else if(!pApp->OnIdle(lIdleCount++))
		{
			lIdleCount=0;
			WaitMessage();
		}
	}

	m_bHelpMode=HELP_INACTIVE;
	ReleaseCapture();

	// make sure the cursor is set appropriately
	SetCapture();
	ReleaseCapture();

	// restore original status bar text
	SendMessage(WM_SETMESSAGESTRING, (WPARAM)nMsgSave);

#ifndef _AFX_NO_OLE_SUPPORT
	// tell in-place servers to exit Shift+F1 help mode
	if(m_pNotifyHook!=NULL)
	{
		m_pNotifyHook->OnContextHelp(FALSE);
	}
#endif

	if(dwContext!=0)
	{
		if(dwContext==-1)
		{
			SendMessage(WM_COMMAND, ID_DEFAULT_HELP);
		}
		else
		{
			pApp->WinHelp(dwContext);
		}
	}
	PostMessage(WM_KICKIDLE);    // trigger idle update
}


template<class PARENTFRAME, class PARENTDOCKBAR>
BOOL COXMenuBarFrame<PARENTFRAME,PARENTDOCKBAR>::ProcessHelpMsg(MSG& msg, 
																DWORD* pContext)
{
	ASSERT(pContext!=NULL);

	if(msg.message==WM_EXITHELPMODE ||
		(msg.message==WM_KEYDOWN && msg.wParam==VK_ESCAPE))
	{
		PeekMessage(&msg, NULL, msg.message, msg.message, PM_REMOVE);
		return FALSE;
	}

	CPoint point;
	if((msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST) ||
		(msg.message >= WM_NCMOUSEFIRST && msg.message <= WM_NCMOUSELAST))
	{
		BOOL bDescendant;
		HWND hWndHit=SetHelpCapture(msg.pt, &bDescendant);
		if(hWndHit==NULL)
			return TRUE;

		BOOL bHitMenuBar=(hWndHit==GetMenuBar().GetSafeHwnd());

		if(bDescendant)
		{
			if(bHitMenuBar)
			{
				if(msg.message==WM_LBUTTONDOWN || GetMenuBar().IsInDisplayingMode())
				{
					ASSERT(::GetCapture()==m_hWnd);
					ReleaseCapture();
					GetMessage(&msg, NULL, msg.message, msg.message);
					DispatchMessage(&msg);
					GetCursorPos(&point);
					SetHelpCapture(point, NULL);
				}
				else
				{
					// Hit one of our owned windows -- eat the message.
					PeekMessage(&msg, NULL, msg.message, msg.message, PM_REMOVE);
					return TRUE;
				}
			}
			else
			{
				return COXMenuBarHost<PARENTFRAME>::ProcessHelpMsg(msg,pContext);
			}
		}
		else
		{
			// Hit one of our apps windows (or desktop) -- dispatch the message.
			PeekMessage(&msg, NULL, msg.message, msg.message, PM_REMOVE);

			// Dispatch mouse messages that hit the desktop!
			DispatchMessage(&msg);
		}
	}
	else
	{
		return COXMenuBarHost<PARENTFRAME>::ProcessHelpMsg(msg,pContext);
	}

	return TRUE;
}


template<class PARENTFRAME, class PARENTDOCKBAR>
BOOL COXMenuBarFrame<PARENTFRAME,PARENTDOCKBAR>::
CheckMenuBarSize(BOOL bNotify/*=TRUE*/)
{
	BOOL bLayoutRecalculated=FALSE;

	COXMenuBar& menuBar=GetMenuBar();
	if(::IsWindow(menuBar.GetSafeHwnd()) && !menuBar.IsFloating())
	{
		CRect rectMenuBar;
		menuBar.GetWindowRect(rectMenuBar);
		BOOL bHorz=(menuBar.m_dwStyle & CBRS_ORIENT_HORZ);
		CControlBar* pDockBar=NULL;
		if(bHorz)
		{
			pDockBar=GetControlBar(AFX_IDW_DOCKBAR_TOP);
			if(pDockBar!=NULL)
				pDockBar=GetControlBar(AFX_IDW_DOCKBAR_BOTTOM);
		}
		else
		{
			pDockBar=GetControlBar(AFX_IDW_DOCKBAR_LEFT);
			if(pDockBar!=NULL)
				pDockBar=GetControlBar(AFX_IDW_DOCKBAR_RIGHT);
		}
		if(pDockBar!=NULL)
		{
			CRect rectDockBar;
			pDockBar->GetWindowRect(rectDockBar);
			if((bHorz && rectDockBar.Width()!=rectMenuBar.Width()) || 
				(!bHorz && rectDockBar.Height()!=rectMenuBar.Height()))
			{
				COXMenuBarHost<PARENTFRAME>::RecalcLayout(bNotify);
				bLayoutRecalculated=TRUE;
			}
		}
	}

	return bLayoutRecalculated;
}


template<class PARENTFRAME, class PARENTDOCKBAR>
void COXMenuBarFrame<PARENTFRAME,PARENTDOCKBAR>::SubclassMDIClient()
{
	ASSERT(::IsWindow(m_hWnd));

	if(m_bIsMDI)
	{
		// if MDI application then we have to handle MDIClient messages
		WNDPROC pfnOldMDIClientProc=
			(WNDPROC)::GetWindowLongPtr(((CMDIFrameWnd*)this)->
			m_hWndMDIClient,GWL_WNDPROC);
		if(m_pfnOldMDIClientProc!=pfnOldMDIClientProc)
		{
			m_pfnOldMDIClientProc=pfnOldMDIClientProc;
			::SetWindowLongPtr(((CMDIFrameWnd*)this)->m_hWndMDIClient,GWL_WNDPROC,
				(LONG_PTR)MDISubclassProc);
			m_bIsMDISubclassed=TRUE;
		} 
	}
}


// dock bars will be created in the order specified by g_dwDockBarMap
// this also controls which gets priority during layout
// this order can be changed by calling EnableDocking repetitively
// with the exact order of priority
template<class PARENTFRAME, class PARENTDOCKBAR>
void COXMenuBarFrame<PARENTFRAME,PARENTDOCKBAR>::
EnableDocking(DWORD dwDockStyle)
{
	// must be CBRS_ALIGN_XXX or CBRS_FLOAT_MULTI only
	ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI))==0);

	ASSERT(m_pFloatingFrameClass!=NULL);
	for (int i=0; i < 4; i++)
	{
		if(g_dwDockBarMap[i][1] & dwDockStyle & CBRS_ALIGN_ANY)
		{
			CInternalDockBar* pDock=
				(CInternalDockBar*)GetControlBar(g_dwDockBarMap[i][0]);
			if(pDock==NULL)
			{
				// use our internal implementation of the dock bar
				pDock=new CInternalDockBar;
				if(!pDock->Create(this,
					WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE|
					g_dwDockBarMap[i][1], g_dwDockBarMap[i][0]))
				{
					AfxThrowResourceException();
				}
			}
		}
	}
}


template<class PARENTFRAME, class PARENTDOCKBAR>
LRESULT COXMenuBarFrame<PARENTFRAME,PARENTDOCKBAR>::HandleMDIClientMsg(HWND hWnd, 
																	UINT msg, 
																	WPARAM wp, 
																	LPARAM lp)
{
	// only in MDIFrame
	ASSERT(m_bIsMDI);
	ASSERT(::IsWindow(m_hWnd));

    switch(msg)
    {
	case WM_MDISETMENU:
		ASSERT(::IsWindow(GetMenuBar().GetSafeHwnd()));
		// set new menu to the menubar
		if(NULL!=wp && ::IsWindow(GetMenuBar().GetSafeHwnd()) && 
			::IsWindow(GetMenuBar().GetToolBarCtrl().GetSafeHwnd()))
		{
			GetMenuBar().SetMenu((HMENU)wp);
		}
	    wp=NULL;
		if(GetMenuBar().m_hMDIWindowMenu!=NULL)
		{
			lp=(LPARAM)GetMenuBar().m_hMDIWindowMenu;
		}

		break;

	case WM_MDIACTIVATE:
	case WM_MDICREATE:
	case WM_MDIDESTROY:
		// the icon of the MDIChild window could have changed
		if(::IsWindow(GetMenuBar().GetSafeHwnd()))
			GetMenuBar().PostMessage(WM_IDLEUPDATECMDUI);
		break;
	case WM_NULL:
		m_bIsMDISubclassed=TRUE;
		break;
    }

	// handle other messages
    if(m_pfnOldMDIClientProc!=NULL)
        return ::CallWindowProc(m_pfnOldMDIClientProc,hWnd,msg,wp,lp);

    return 0;
}


template<class PARENTFRAME, class PARENTDOCKBAR>
LRESULT CALLBACK COXMenuBarFrame<PARENTFRAME,PARENTDOCKBAR>::
MDISubclassProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	HWND hWndParent=::GetParent(hWnd);
	ASSERT(hWndParent!=NULL);

	void* pMenuBarFrame=NULL;
	if(!g_arrMenuBarFrames.Lookup(hWndParent,pMenuBarFrame))
		return 0;
	ASSERT(pMenuBarFrame!=NULL);
	// call COXMenuBarFrame implementation
	if(::IsWindow(((COXMenuBarFrame<PARENTFRAME,PARENTDOCKBAR>*)
		pMenuBarFrame)->m_hWnd))
	{
		return ((COXMenuBarFrame<PARENTFRAME,PARENTDOCKBAR>*)pMenuBarFrame)->
			HandleMDIClientMsg(hWnd,msg,wp,lp);
	}

	return 0;
}



#endif	//	_MFC_VER>=0x0420


#endif // _MENUBAR_H

