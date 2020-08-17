// ==========================================================================
// 					Class Specification : COXBitmapMenu
// ==========================================================================

// Header file : OXBitmapMenu.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class 
//	YES	Derived from CMenu

//	NO	Is a Cwnd. (Its a CMenu)
//	NO	Two stage creation 
//	NO	Has a message map
//	NO	Needs a resource 

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
// COXBitmapMenu implements drawing of the menu items

// Remark:
// User doesn't need to use this object directly. COXBitmapMenuOrganizer
// handles creation and destruction of COXBitmapMenu Objects.

// Prerequisites (necessary conditions):
//
/////////////////////////////////////////////////////////////////////////////

#ifndef BITMAPMENU_H
#define BITMAPMENU_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#define __AFXTEMPL_H_H
#endif

#include "OXDllExt.h"

// for drag and drop support
#include "OXDragDropSupport.h"
#include "OXMainRes.h"
#include "OXToolTipCtrl.h"

//////////////////////////////////////////////////////////



typedef struct _tagNMBMCUSTOMIZE
{
	NMHDR nmhdr;
	UINT nCustomizeEventID;
	LPARAM lParam;
} NMBMCUSTOMIZE;

//////////////////////////////////////////////////////////////////////////


// ID of the expansion item
#ifndef ID_OX_SHOWALLITEMS
#define ID_OX_SHOWALLITEMS					0x0000ff56
#endif	//	ID_OX_SHOWALLITEMS

#ifndef ID_OXBITMAPMENU_SEPARATOR_HEIGHT
#define ID_OXBITMAPMENU_SEPARATOR_HEIGHT			8
#endif	//	ID_OXBITMAPMENU_SEPARATOR_HEIGHT

#ifndef ID_OXBITMAPMENUPOPUPWND_DEFAULT_WIDTH
#define ID_OXBITMAPMENUPOPUPWND_DEFAULT_WIDTH		50
#endif	//	ID_OXBITMAPMENUPOPUPWND_DEFAULT_WIDTH
#ifndef ID_OXBITMAPMENUPOPUPWND_DEFAULT_HEIGHT
#define ID_OXBITMAPMENUPOPUPWND_DEFAULT_HEIGHT		18
#endif	//	ID_OXBITMAPMENUPOPUPWND_DEFAULT_HEIGHT

#ifndef IDT_OXCHECKFORDRAGDROPEVENT
#define IDT_OXCHECKFORDRAGDROPEVENT					412
#endif	//	IDT_OXCHECKFORDRAGDROPEVENT
#ifndef ID_OXCHECKFORDRAGDROPEVENT_DELAY
#define ID_OXCHECKFORDRAGDROPEVENT_DELAY			200
#endif	//	ID_OXCHECKFORDRAGDROPEVENT_DELAY


#ifndef OXODS_HIDDEN
#define OXODS_HIDDEN								0x1000
#endif	//	OXODS_HIDDEN
#ifndef OXODS_HIDDENFIRST
#define OXODS_HIDDENFIRST							0x2000
#endif	//	OXODS_HIDDENFIRST
#ifndef OXODS_HIDDENLAST
#define OXODS_HIDDENLAST							0x4000
#endif	//	OXODS_HIDDENLAST

class COXImageInfo;
class COXItemInfo;
class COXBitmapMenuOrganizer;
class COXMenuSkin;
class COXShadowedItemWnd;
class COXCoolToolBar;

struct OXDIMENSIONCONSTANTS
{
	// The gap between left edge of menu item and bitmap in pixels
	int m_nGapLeftBitmap;
	// The gap between the right edge of the bitmap and the end of the icon area (XP skin only)
	int m_nGapRightBitmap;
	// The gap between bitmap and menu text in pixels
	int m_nGapBitmapText;
	// The gap between menu text and accelerator
	int m_nGapTextAcclrtr;
	// The gap between accelerator and right edge of menu item
	int m_nGapAcclrtrRight;
	// Extra vertical space on top and bottom of text in pixels
	int m_nGapVertText;	
	// --- Extra vertical space on top and bottom of bitmap in pixels
	int m_nGapVertBitmap;
	// Height of the menu separator
	int m_nSeparatorHeight;
	// Minimum width of the bitmap
	int m_nMinBitmapWidth;
	// Gap between two menu items
	int m_nGapMenuItems;
};

typedef CArray<COLORREF, COLORREF> OXSHADOWARRAY;

struct OXPOPUPWND
{
	OXPOPUPWND(HWND hWindow)
	{
		hWnd = hWindow;
		arPixels.SetSize(0, 1024);
	};
	HWND hWnd;
	OXSHADOWARRAY arPixels;
};

class COXPopupWndStack : public CTypedPtrList<CPtrList, OXPOPUPWND*>
{
public:
	void Push(HWND hWnd) { AddHead(new OXPOPUPWND(hWnd)); };
	HWND Pop()
	{
		if (IsEmpty())
			return NULL;
		return GetHead()->hWnd;
	};

	// Remove the OXPOPUPWND from the stack
	void Remove(HWND hWnd)
	{
		POSITION pos = GetHeadPosition();
		while (pos != NULL)
		{
			OXPOPUPWND* pPW = GetAt(pos);
			if (pPW->hWnd == hWnd)
			{
				RemoveAt(pos);
				delete pPW;
				break;
			}
			GetNext(pos);
		}
	};

	// Returns the shadow array of the given window
	OXSHADOWARRAY* GetShadowArray(HWND hWnd)
	{
		POSITION pos = GetHeadPosition();
		while (pos != NULL)
		{
			OXPOPUPWND* pPW = GetNext(pos);
			if (pPW->hWnd == hWnd)
				return &pPW->arPixels;
		}
		return NULL;
	}
};

/////////////////////////////////////////////////////////////////////////////
// COXShadowedItemWnd window

class COXShadowedItemWnd : public CWnd
{
	friend class COXMenuSkinClassic;
	friend class COXMenuSkinXP;
	friend class COXMenuSkin2003;

// Construction
public:
	COXShadowedItemWnd(COXCoolToolBar* pCoolToolBar, int iMenuItem, UINT nFlags);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXShadowedItemWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXShadowedItemWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(COXShadowedItemWnd)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	COXCoolToolBar* m_pCoolToolbar;
	int m_iMenuItem;
	UINT m_nPosFlags;
	
};

// Flags to indicate the ralative position of the popup menu from the selected item
#define OX_TPM_BOTTOM		0x1	// the menu is displayed on the bottom of the selected item
#define OX_TPM_RIGHT		0x2
#define OX_TPM_LEFT			0x4
#define OX_TPM_TOP			0x8

class OX_CLASS_DECL COXBitmapMenu : public CMenu
{
	friend class COXBitmapMenuOrganizer;
	friend class COXMenuSkinClassic;
	friend class COXMenuSkinXP;
	friend class COXMenuSkin2003;
	friend class COXMenuBar;
	friend class COXBitmapMenuPopupWnd;
	friend class COXShadowedItemWnd;

DECLARE_DYNAMIC(COXBitmapMenu)

//Data members --------------------------------------------------------

protected:
	// map of accelerator keys
	CMap<TCHAR, TCHAR, int, int> m_KeyAccessMap;
	// holds the largest width of all the bitmaps in the menu
	int m_nBitmapExtent;
	// holds the height of the text
	int m_nTextHeight;
	// holds the largest width of the all the accelerator texts of menu items
	int m_nAcceleratorExtent;
	// holds the largest width of the texts of menu items
	int m_nStringExtent;
	// Maintains a list of item infos for cleaning purpose
	// while destructing the object
	CList<COXItemInfo*, COXItemInfo*> m_ItemInfoList;


	// flag that specifies that menu supports customization
	BOOL m_bCutomizable;
	// index of the menu item being currently customized
	int m_nCustomizedItemIndex;
	// flag that specifies that the menu is in the customization mode
	BOOL m_bInCutomizationMode;
	// recipient of customization related notifications 
	HWND m_hWndCustomizeOrganizer;


	// default COleDropSource for drag'n'drop operation
	COXSHBDropSource m_oleDropSource;
	// default COleDropTarget for drag'n'drop operation
	COXSHBDropTarget m_oleDropTarget;
	// flag that specifies whether we launched drag'n'drop operation or not
	BOOL m_bDragDropOwner;
	// flag that specifies if any drag and drop operation is undergoing
	BOOL m_bDragDropOperation;

	// popup window that displays the menu in customization mode
	class COXBitmapMenuPopupWnd* m_pPopupWnd;

	// index of the item before which the insertion mark is drawn
	int m_nInsertMarkIndex;

	// index of the dragged item
	int m_nDraggedItemIndex;

	// The rectangle of the toolbar or menubar drop down button which is
	// responsible for displaying the menu. This rectangle is in screen coordinates.
	CRect m_rectDropDownItem;

	static LRESULT CALLBACK PopupWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	COXPopupWndStack m_PopupWndStack; // a stack of all popup container windows

	COXToolTipCtrl m_ToolTipCtrl;

private:
	BOOL IsPopupItem(UINT nItemID);
	static BOOL RegisterWindowClass(HINSTANCE hInstance);
	static CSize GetPopupMenuSize(CMenu* pMenu);
	static void DeterminePosition(CMenu* pMenu, LPCRECT lpItemRect, DWORD dwStyle, CPoint& ptTopLeft, UINT& nFlags, UINT& nPosFlags);
	static COXMenuSkin* m_pMenuSkin;
	static COXMenuSkin* COXBitmapMenu::GetMenuSkin();

//Member functions-----------------------------------------------------------------------
public:
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Object Constructions and defaults are initialised
	COXBitmapMenu();

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Cleanup and Object Destruction
	virtual ~COXBitmapMenu();


	// --- In  :	lpDrawItemStruct	-	A pointer to a DRAWITEMSTRUCT 
	//										containing information about 
	//										the item to draw.
	// --- Out : 
	// --- Returns:
	// --- Effect : This function is called by the MFC framework when a visual 
	//				aspect of the menu has changed. If you override this function 
	//				to implement drawing of the menu you must restore all GDI 
	//				object selected for the display context supplied in 
	//				lpDrawItemStruct before the member function exits
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// --- In  :	lpMeasureItemStruct	-	A pointer to a MEASUREITEMSTRUCT structure
	// --- Out : 
	// --- Returns: 
	// --- Effect : This function is called by the framework to determine menu 
	//				dimensions the menu is created
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	
	// --- In  : 
	// --- Out : 
	// --- Returns:
	// --- Effect : Calculates the largest extents of bitmaps, menu texts, 
	//				and accelerator texts
	void CalcExtents();


	// --- In  :	ppt			-	pointer to POINT structure that defines the point 
	//								to test	in client coordinates of the menu popup 
	//								window
	// --- Out :	pbBefore	-	pointer to BOOL variable that will be set to 
	//								TRUE if specified point is located over the top 
	//								part of found menu item; othrwise it will be set
	//								to FALSE
	// --- Returns:	The index of the found menu item
	// --- Effect : Finds menu item that is located under the specified point
	int HitTest(LPPOINT ppt, BOOL* pbBefore=NULL); 


	// --- In  :	bCustomizable	-	if TRUE the menu ca be customized
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets the flag that specifies whether the menu can be 
	//				customized or not
	inline void SetCustomizable(BOOL bCustomizable) { m_bCutomizable=bCustomizable; }

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if the menu can be customized; otherwise FALSE 
	// --- Effect:	Retrieves the flag that specifies whether the menu can be 
	//				customized or not
	virtual BOOL IsCustomizable() const { return m_bCutomizable; }


	// --- In  :	bInCutomizationMode		-	if TRUE the menu will be set in to 
	//											customization mode
	//				hWndCustomizeOrganizer	-	handle to window that will be a
	//											recipient of customization related 
	//											notifications
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets the menu in to customization/normal mode
	inline void SetCutomizationMode(BOOL bInCutomizationMode,
		HWND hWndCustomizeOrganizer=NULL) 
	{ 
		if(bInCutomizationMode && !IsCustomizable())
			SetCustomizable(TRUE);
		if(!bInCutomizationMode)
			SetCustomizedItem(-1);
		else
			m_hWndCustomizeOrganizer=hWndCustomizeOrganizer;
		m_bInCutomizationMode=bInCutomizationMode;
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if the menu is in customization mode; FALSE if it is in 
	//				normal mode
	// --- Effect:	Retrieves the flag that specifies whether the menu is in 
	//				customization or normal mode
	inline BOOL IsInCustomizationMode() const { return m_bInCutomizationMode; }


	// --- In  :	nIndex	-	menu item index to be set as customized
	// --- Out : 
	// --- Returns:	The index of previous customized menu item. -1 if there were 
	//				no customized menu item before
	// --- Effect : Sets customized menu item. Specify -1 as a parameter in order 
	//				to reset customized menu item
	int SetCustomizedItem(int nIndex);

	// --- In  :	
	// --- Out : 
	// --- Returns:	The index of the customized menu item. -1 if there is no 
	//				customized menu item
	// --- Effect : Retrieves customized menu item
	inline int GetCustomizedItem() const { return m_nCustomizedItemIndex; }

	// --- In  :	nIndex	-	menu item index to test
	// --- Out : 
	// --- Returns:	TRUE if specified menu item is customized; otherwise FALSE
	// --- Effect : Retrieves the flag that specifies if given item is customized 
	//				one or not
	inline BOOL IsItemCustomized(int nIndex) const 
	{ 
		return (nIndex==m_nCustomizedItemIndex);
	}


	// --- In  :	nIndex	-	menu item index before which the insert mark 
	//							will be set
	// --- Out : 
	// --- Returns:	The insert mark previous position. -1 if there were no 
	//				insert mark set before
	// --- Effect : Sets insert mark at specified position. Specify -1 as a 
	//				parameter in order to reset insert mark position
	int SetInsertMark(int nItemIndex);

	// --- In  :	
	// --- Out : 
	// --- Returns:	The insert mark position. -1 if there is no insert mark set
	// --- Effect : Retrives insert mark position
	inline int GetInsertMark() const { return m_nInsertMarkIndex; }


	// --- In  :	nIndex	-	menu item index to be set as dragged
	// --- Out : 
	// --- Returns:	The index of previous dragged menu item. -1 if there were 
	//				no dragged menu item before
	// --- Effect : Sets dragged menu item. Specify -1 as a parameter in order 
	//				to reset dragged menu item
	inline void SetDraggedItem(int nIndex) { m_nDraggedItemIndex=nIndex; }

	// --- In  :	
	// --- Out : 
	// --- Returns:	The index of the dragged menu item. -1 if there is no 
	//				dragged menu item
	// --- Effect : Retrieves dragged menu item
	inline int GetDraggedItem() const { return m_nDraggedItemIndex; }

	// --- In  :	nIndex	-	menu item index to test
	// --- Out : 
	// --- Returns:	TRUE if specified menu item is dragged; otherwise FALSE
	// --- Effect : Retrieves the flag that specifies if given item is dragged 
	//				one or not
	inline BOOL IsItemDragged(int nIndex) const 
	{ 
		return (nIndex==m_nDraggedItemIndex);
	}


	// --- In  :	nIndex	-	menu item index to display customize menu for
	//				point	-	screen coordinates of the Top/Left point at which 
	//							the context menu should be displayed
	// --- Out : 
	// --- Returns:	TRUE if context menu was successfully created and displayed; 
	//				otherwise FALSE
	// --- Effect : Display context menu with customization commands for the 
	//				specified menu item
	virtual BOOL DisplayCustomizeItemContextMenu(int nItemIndex, CPoint point);


	// --- In  :	pWndOwner	-	pointer to a window that will be assigned as 
	//								an owner of returned COleDropSource object
	// --- Out : 
	// --- Returns:	A pointer to internal COleDropSource object
	// --- Effect : Retrieves a pointer to internal COleDropSource object that will
	//				allow this object to handle drag'n'drop operation
	virtual COleDropSource* GetDropSource(CWnd* pWndOwner) 
	{ 
		ASSERT(pWndOwner!=NULL);
		// owner window (recipient window) must exist at that moment
		ASSERT(::IsWindow(pWndOwner->GetSafeHwnd()));
		m_oleDropSource.SetOwner(pWndOwner);
		return &m_oleDropSource; 
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	A pointer to internal COleDropTarget object
	// --- Effect : Retrieves a pointer to internal COleDropTarget object that will
	//				allow this object to handle drag'n'drop operation
	virtual COleDropTarget* GetDropTarget() { return &m_oleDropTarget; }

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if this object has started current drag'n'drop operation;
	//				otherwise FALSE
	// --- Effect : Retrieves the flag that specifies whether this object has 
	//				started current drag'n'drop operation 
	inline BOOL IsDragDropOwner() const { return m_bDragDropOwner; }


	// --- In  :	pPopupWnd	-	a pointer to COXBitmapMenuPopupWnd object that 
	//								will be used to display the menu in customization
	//								mode
	// --- Out : 
	// --- Returns:	
	// --- Effect : Sets a pointer to COXBitmapMenuPopupWnd object that will be used 
	//				to display the menu in customization mode
	inline void SetPopupWnd(COXBitmapMenuPopupWnd* pPopupWnd) 
	{ 
		m_pPopupWnd=pPopupWnd; 
	}

	// --- In  :	pPopupWnd	-	a pointer to COXBitmapMenuPopupWnd object that 
	//								will be used to display the menu in customization
	//								mode
	// --- Out : 
	// --- Returns:	A pointer to COXBitmapMenuPopupWnd object that will be used to 
	//				display the menu in customization mode
	// --- Effect : Retrieves a pointer to COXBitmapMenuPopupWnd object that will be used 
	//				to display the menu in customization mode
	virtual COXBitmapMenuPopupWnd* GetPopupWnd() const { return m_pPopupWnd; }


	// --- In  :	nIndex	-	index of the menu item that is about to become the 
	//							dragged one
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Initializes drag'n'drop operation
	virtual void OnBeginDragDrop(int nIndex);
	

	// --- In  :	rgbFace		-	base color 
	//				rgbHilight	-	hilight color
	// --- Out : 
	// --- Returns:	A handle to created brush object that combines base and highlight 
	//				colors in a specific way
	// --- Effect:	Creates new brush object for drawing routines (allows to draw
	//				the background in the way similar to drawing the background
	//				of pressed toolbar button). Caller is responsible for deleting
	//				thie returned brush
	static HBRUSH HBrushDitherCreate(COLORREF rgbFace, COLORREF rgbHilight);

	BOOL TrackPopupMenu(COXCoolToolBar* pCoolToolBar, CWnd* pWnd, LPCRECT lpRect = NULL);

	// --- Effect:	This method should be called by a CMDMSG handler in the main app,
	//               and it's job is to handle menu buttons for restore/close/minimize in an MDI app
	static BOOL HandleMDICommandMessage(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

private:
	static void RestoreMDI();
	static void CloseMDI();
	static void MinimizeMDI();

protected:
	static WNDPROC m_origWndProc;
	// calculates rectangles for displaying item's  image, text and accelerator text
	virtual void DistributeSpace(UINT nState, COXImageInfo* pImageInfo, 
		CRect itemRect, CRect& buttonRect, CRect& imageRect, CRect& text1Rect, 
		CRect& text2Rect);
	// draws the background of menu item
	virtual void DrawBackground(CDC* pDC, UINT nState, COXImageInfo* pImageInfo, 
		CRect itemRect, CRect buttonRect);
	// draws the button that frames menu item image
	virtual void DrawButton(CDC* pDC, UINT nState,  COXImageInfo* pImageInfo, 
		CRect buttonRect);
	// draws menu item image
	virtual void DrawImage(CDC* pDC, UINT nState, COXImageInfo* pImageInfo, 
		CRect imageRect);
	// draws menu item text
	virtual void DrawText(CDC* pDC, UINT nState, CString sText, CRect text1Rect, 
		CRect text2Rect);
	// draws separator menu item
	virtual void DrawSeparator(CDC* pDC, CRect itemRect);
	// draws customized menu item
	virtual void DrawCustomized(CDC* pDC, CRect itemRect);
	// draws insert mark
	virtual void DrawInsertMark(CDC* pDC, CRect itemRect, BOOL bBefore);
	// draws submenu item
	virtual void DrawSubmenuItem(CDC* pDC, UINT nState, CString sText, 
		COXImageInfo* pImageInfo, CRect itemRect, CRect buttonRect, 
		CRect text1Rect, CRect text2Rect);
	// draws ezpansion item
	virtual void DrawExpansionItem(CDC* pDC, CRect itemRect, UINT nState);

	// Saves new created COXItemInfo object in the internal array
	void AddItemInfo(COXItemInfo* pItemInfo);

	// handle update routines for the menu commands
	void OnUpdateCmdUI(CWnd* pWnd, UINT nIndex, BOOL bSysMenu);

	// sends customization related notifications
	LRESULT SendCustomizeNotification(UINT nCustomizeCmdID);

	// update the contents of the displayed menu
	void UpdateContents();

public:
	// drag and drop support
	virtual LONG OnDragEnter(WPARAM wParam, LPARAM lParam);
	virtual LONG OnDragOver(WPARAM wParam, LPARAM lParam);
	virtual LONG OnDragLeave(WPARAM wParam, LPARAM lParam);
	virtual LONG OnDrop(WPARAM wParam, LPARAM lParam);


	// --- In  :	lpData		-	reference to a pointer of data that contains 
	//								item info in specific drag'n'drop format
	//				hMenu		-	handle to a menu where dropped item will be 
	//								inserted
	//				nItemIndex	-	position at which dropped item will be inserted
	//				
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Retrieves item info from dropped data and insert it at specified 
	//				position in specified menu
	virtual void RetrieveDragDropMenuItem(BYTE*& lpData, HMENU hMenu, int nItemIndex);

	// advanced customization commands handlers
	virtual void OnCustBMDelete();
	virtual void OnCustBMAppearance();
	virtual void OnCustBMSeparatorBefore();
	virtual void OnCustBMSeparatorAfter();
	virtual void OnCustBMRecentlyUsed();
};

/////////////////////////////////////////////////////////////////////////////


class OX_CLASS_DECL COXBitmapMenuPopupWnd : public CWnd
{
	friend class COXMenuSkinClassic;
	friend class COXMenuSkinXP;

DECLARE_DYNAMIC(COXBitmapMenuPopupWnd)

//Data members --------------------------------------------------------
public:

protected:
	COXBitmapMenu* m_pBitmapMenu;

	CRect m_rectWindow;
	CArray<CRect,CRect&> m_arrItemRects;

	CFont m_fontMenu;

	INT_PTR m_nCheckForDragDropEventTimerID;

//Member functions-----------------------------------------------------------------------
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXBitmapMenuPopupWnd();

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXBitmapMenuPopupWnd();


	// --- In  :	
	// --- Out : 
	// --- Returns:	pointer to associated bitmap menu object
	// --- Effect : retrieves the associated bitmap menu object
	virtual COXBitmapMenu* GetBitmapMenu() const { return m_pBitmapMenu; }

	
	// --- In  :	pMenu		-	pointer to the popup bitmap menu to be displayed
	//				nFlags		-	Specifies a screen-position flag. It can be 
	//								one of the following: 
	//						
	//						TPM_CENTERALIGN		Centers the pop-up menu horizontally 
	//											relative to the coordinate specified 
	//											by x.
	//						TPM_LEFTALIGN		Positions the pop-up menu so that its 
	//											left side is aligned with the 
	//											coordinate specified by x.
	//						TPM_RIGHTALIGN		Positions the pop-up menu so that its 
	//											right side is aligned with the 
	//											coordinate specified by x.
	//
	//				x			-	Specifies the horizontal position in screen 
	//								coordinates of the pop-up menu. Depending on the 
	//								value of the nFlags parameter, the menu can be 
	//								left-aligned, right-aligned, or centered relative 
	//								to this position
	//				y			-	Specifies the vertical position in screen 
	//								coordinates of the top of the menu on the screen
	//				pWnd		-	Identifies the window that owns the pop-up menu.
	// --- Out : 
	// --- Returns:	TRUE if the function is successful; otherwise FALSE
	// --- Effect : Displays a floating pop-up menu at the specified location and 
	//				tracks the selection of items on the pop-up menu. A floating 
	//				pop-up menu can appear anywhere on the screen
	virtual BOOL TrackPopupMenu(COXBitmapMenu* pMenu, UINT nFlags, 
		int x, int y, CWnd* pWnd);


	// --- In  :	
	// --- Out : 
	// --- Returns: 
	// --- Effect : Hides the popup window and reset all internal data
	virtual void ResetPopupMenu();


	// --- In  :	
	// --- Out : 
	// --- Returns: 
	// --- Effect : Called by bitmap menu object in order to notify that  
	//				the contents of the menu have been changed
	virtual void OnMenuChanged();


	// --- In  :	nIndex	-	index of the item in the associated menu that 
	//							must be redrawn
	// --- Out : 
	// --- Returns: 
	// --- Effect : Redraws the specified menu item
	void RedrawItem(int nIndex);


	// --- In  :	nIndex	-	index of the item in the associated menu which 
	//							rectangle in client coordinates is being retrieved
	// --- Out : 
	// --- Returns: rectangle taken by the specified menu item
	// --- Effect : Retrieves the rectangle taken by the specified menu item
	inline CRect GetItemRect(int nIndex) const 
	{
		ASSERT(nIndex>=0 && nIndex<m_arrItemRects.GetSize());
		return m_arrItemRects[nIndex];
	}


protected:
	// --- In  :	nFlags	-	Specifies a screen-position flag. It can be one of the 
	//							following: 
	//						
	//						TPM_CENTERALIGN		Centers the pop-up menu horizontally 
	//											relative to the coordinate specified 
	//											by x.
	//						TPM_LEFTALIGN		Positions the pop-up menu so that its 
	//											left side is aligned with the 
	//											coordinate specified by x.
	//						TPM_RIGHTALIGN		Positions the pop-up menu so that its 
	//											right side is aligned with the 
	//											coordinate specified by x.
	//
	//				x		-	Specifies the horizontal position in screen 
	//							coordinates of the pop-up menu. Depending on the 
	//							value of the nFlags parameter, the menu can be 
	//							left-aligned, right-aligned, or centered relative 
	//							to this position
	//				y		-	Specifies the vertical position in screen 
	//							coordinates of the top of the menu on the screen
	// --- Out : 
	// --- Returns:	TRUE if the function is successful; otherwise FALSE
	// --- Effect : Displays a floating pop-up menu at the specified location and 
	//				tracks the selection of items on the pop-up menu. A floating 
	//				pop-up menu can appear anywhere on the screen
	virtual BOOL CalcLayout(UINT nFlags, int x, int y);


protected:
	void UpdateMenuMetrics();

	// Generated message map functions
protected:
	//{{AFX_MSG(COXBitmapMenuPopupWnd)
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
	//}}AFX_MSG

	// drag and drop support
	virtual LONG OnDragEnter(WPARAM wParam, LPARAM lParam);
	virtual LONG OnDragOver(WPARAM wParam, LPARAM lParam);
	virtual LONG OnDragLeave(WPARAM wParam, LPARAM lParam);
	virtual LONG OnDrop(WPARAM wParam, LPARAM lParam);

	// advanced customization commands handlers
	afx_msg void OnCustBMDelete();
	afx_msg void OnCustBMAppearance();
	afx_msg void OnCustBMSeparatorBefore();
	afx_msg void OnCustBMSeparatorAfter();
	afx_msg void OnCustBMRecentlyUsed();

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

#endif
