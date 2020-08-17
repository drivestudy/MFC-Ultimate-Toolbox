// ==========================================================================
// 					Class Specification : 
//						COXTreeCtrl
// ==========================================================================

// Header file : COXTreeCtrl.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CListCtrl

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions


// //////////////////////////////////////////////////////////////////////////
//Description:
//
// This class is a extension of a tree control
// It has the following features:
// + multicolumn support
// + multiple selections
// + entry row selection mode
// + bitmap support for each column
// + editable subitems
// + hide/unhide items
// + disable items
// + veritcal and horizontal grid lines
// + set font for individual item/subitem
// + set color for individual item/subitem
// + additional edit modes like for any item/subitem:
//  =dropdown combobox
//  =date picker (see COXCalendar control)
//
// //////////////////////////////////////////////////////////////////////////
//
// COXTreeCtrl is derived from CListCtrl not from CTreeCtrl. That means that if you 
// want to use dialog editor to create resource for your CDialog or CFormView 
// derived class you have to use CListCtrl with "Report" and 
// "OwnerDraw Fixed" style set. It also means that CListCtrl emulates CTreeCtrl 
// functionality although not to full extent (see function reference for details).
// COXTreeCtrl lets you operate with standard structures and types that have 
// to do with CTreeCtrl, but you still can use CListCtrl stuff to work with control
// (it could be useful feature if you really need to do some fancy stuff with 
// COXTreeCtrl). 
// 
// Taking into account all mentioned above, this is not a big surprise that 
// you have to use special technique to utilize COXTreeCtrl control. And all 
// in all you possibly cannot do that just changing the name of class from 
// CTreeCtrl to COXTreeCtrl. Below you will find description of common steps that 
// you have to take to implement COXTreeCtrl in your application :
//
//


//
//	COXTreeCtrl with Dialog Resource
//
// If you want to use it in a dialog or FormView and you are using dialog 
// resource, then the best way is to create a list control ,
// set up "Report" style and "OwnerDraw Fixed" style in resource editor 
// and map it to COXTreeCtrl member variable (using subclassing or ClassWizard and 
// DDX/DDV technique)
//
//


//
//  Initializing and populating of COXTreeCtrl
//	
// As soon as control is created you can populate it with columns, items and 
// set different styles to make it look the way you want. You can insert and delete
// columns, items and subitems at any time after control's creation. 
// Next function are supproted to populate the COXTreeCtrl control:
//
//		int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, 
//						 int nFormat=LVCFMT_LEFT, int nWidth=-1,int nSubItem =-1);
//		// similar to CListCtrl (note that column #0 is added automatically)
//		int InsertColumn(int nCol, const LV_COLUMN* pColumn);
//		// similar to CListCtrl (note that column #0 is added automatically)
//		call standard CListCtrl::DeleteColumn(int nCol) to delete any column
//
//		HTREEITEM InsertItem(LPCTSTR lpszItem, HTREEITEM hParent=TVI_ROOT, 
//							 HTREEITEM hInsertAfter=TVI_LAST);
//		HTREEITEM InsertItem(LPTV_INSERTSTRUCT lpInsertStruct);
//		BOOL DeleteItem(HTREEITEM hItem);
//
//


//
//	COXTreeCtrl styles
//
// You can set different style to COXTreeCtrl control to get the 
// functionality you need. This control supports next standard styles:
//
//		TVS_SHOWSELALWAYS
//		TVS_EDITLABELS
//		TVS_LINESATROOT
//		TVS_HASBUTTONS
//		TVS_HASLINES
//
// These could be get and modified using functions:
//
//		DWORD GetStyle();
//		BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags=0);
//
// Also we introduce extended styles:
//
//		TVOXS_MULTISEL		-	multiple selection
//		TVOXS_COLUMNHDR		-	column header
//		TVOXS_ROWSEL		-	row selection mode
//		TVOXS_HGRID			-	horizontal grid
//		TVOXS_VGRID			-	vertical grid
//		TVOXS_FLGRID		-	full length grid lines
//		TVOXS_PICKANYWHERE	-	select tree control item clicking 
//								anywhere within grid of control
//		TVOXS_NOSCROLL		-	disables scrolling. All items must be within the 
//								client area. 
//		TVOXS_NOSORTHEADER	-	specifies that column headers do not work like buttons. 
//								This style is useful if clicking a column header does not 
//								carry out an action, such as sorting.
//		TVOXS_SHAREIMAGELISTS-	specifies that the control does not take ownership of the 
//								image lists assigned to it (that is, it does not destroy 
//								the image lists when it is destroyed). This style enables 
//								the same image lists to be used with multiple extended
//								tree controls.
//		TVOXS_ITEMTIPS		-	item tips for partially hidden items
// 
// and functions to manipulate them
//		DWORD GetExStyle();
//		BOOL ModifyExStyle(DWORD dwExStyleRemove,DWORD dwExStyleAdd);
//
// By default we include item tips functionality, but you have to include in 
// your project next files in order to to get it:
//		"OXHookWnd.cpp"	
//		"OXItemTip.cpp"	
//		"OXItemTipWnd.cpp"	
// 
// If you are not interested in item tips then you can declare in your "stdafx.h"
// (or whatever place is appropriate) next definition:
//		#define OX_TREECTRL_NOITEMTIPS
//
// But if you still need item tips then taking into account that extended tree 
// control is ownerdrawfixed CListCtrl then we have to provide additional item 
// information for COXItemTip class by means of handling WM_DOYOUNEEDITEMTIP
// message. Next virtual function is defined as handler of that message:
//
//		virtual void OnNeedItemTip(WPARAM wParam, LPARAM lParam);
//
// 
//
// Besides there are a number of helper functions
//
//		BOOL GetPickAnywhere();
//		// --- Returns : whether TVOXS_PICKANYWHERE style set or not
//		BOOL GetDrawGridFullLength();
//		// --- Returns : whether TVOXS_FLGRID style set or not
//		BOOL HasGrid(UINT nGridType);
//		// --- In  :horizontal (TVOXS_HGRID) or vertical (TVOXS_VGRID) 
//					grid style
//		// --- Returns :whether this grid style applied (TRUE) or not (FALSE)
//		BOOL HasLinesAtRoot();
//		// --- Returns : whether TVS_LINESATROOT style set or not
//		BOOL HasLines();
//		// --- Returns :whether TVS_HASLINES style set or not
//		BOOL HasButtons();
//		// --- Returns :whether TVS_HASBUTTONS style set or not
//		BOOL GetShowSelAlways();
//		// --- Returns :whether TVS_SHOWSELALWAYS style set or not
//		BOOL IsHeaderSorting();
//		// --- Returns: TRUE if sorting operation is supported in extended tree control,
//		//				or FALSE otherwise
//
// Of course, all styles can be modified at run-time
//
//


//
//	Manipulating Columns
//
// One of the new features of COXTreeCtrl is an option to use columns. 
// Besides standard CListCtrl functions:
//
//		GetColumn	-	Retrieves the attributes of a control's column.
//		SetColumn	-	Sets the attributes of a list view column.
//		GetColumnWidth	-	Retrieves the width of a column in report 
//							view or list view.
//		SetColumnWidth	-	Changes the width of a column in report 
//							view or list view.
//
// you could use next ones to manipulate columns in the COXTreeCtrl:
//
//		int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, 
//						 int nFormat=LVCFMT_LEFT, int nWidth=-1,int nSubItem =-1);
//		// similar to CListCtrl (note that column #0 is added automatically)
//		int InsertColumn(int nCol, const LV_COLUMN* pColumn);
//		// similar to CListCtrl (note that column #0 is added automatically)
//		BOOL ResizeColToFit(int nCol);
//		// --- In  : subitem (column) number
//		// --- Returns : Nonzero if successful; otherwise 0.
//		// --- Effect : Sets the width of specified column to the width 
//						of the widest cell in this column out of all items.
//
// ResizeColToFit function is automatically called when user doubleclicks 
// on the border of a column in a header control
//
//


//
//  Manipulating Items and Subitems
//
// Predictably this section includes most of functions from COXTreeCtrl class. 
//
// You can insert and delete items using next functions:
//
//		HTREEITEM InsertItem(LPCTSTR lpszItem, HTREEITEM hParent=TVI_ROOT, 
//							 HTREEITEM hInsertAfter=TVI_LAST);
//		HTREEITEM InsertItem(LPTV_INSERTSTRUCT lpInsertStruct);
//		BOOL DeleteItem(HTREEITEM hItem);
//
// You can manipulate items in ways you used to do with standard CTreeCtrl items.
// For this purpose we create wraper functions that emulate standard functionality
//
//		BOOL GetItemRect(HTREEITEM hItem, LPRECT lpRect, BOOL bTextOnly);
//		BOOL SetItemData(HTREEITEM hItem, DWORD dwData);
//		DWORD GetItemData(HTREEITEM hItem);
//		BOOL SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask);
//		UINT GetItemState(HTREEITEM hItem, UINT nStateMask);
//		HTREEITEM GetRootItem();
//		HTREEITEM GetDropHilightItem();
//		HTREEITEM GetParentItem(HTREEITEM hItem);
//		HTREEITEM GetPrevSiblingItem(HTREEITEM hItem);
//		HTREEITEM GetNextSiblingItem(HTREEITEM hItem);
//		HTREEITEM GetChildItem(HTREEITEM hItem);
//		BOOL ItemHasChildren(HTREEITEM hItem);
//		UINT GetIndent();
//		CImageList* GetImageList(UINT nImage);
//		HTREEITEM GetSelectedItem();
//		HTREEITEM GetPrevVisibleItem(HTREEITEM hItem);
//		HTREEITEM GetNextVisibleItem(HTREEITEM hItem);
//		HTREEITEM GetFirstVisibleItem();
//		CImageList* SetImageList(CImageList* pImageList, int nImageList);
//		HTREEITEM InsertItem(LPCTSTR lpszItem, HTREEITEM hParent=TVI_ROOT, HTREEITEM hInsertAfter=TVI_LAST);
//		HTREEITEM InsertItem(LPTV_INSERTSTRUCT lpInsertStruct);
//		BOOL GetItem(TV_ITEM *pTVI);
//		BOOL SetItem(HTREEITEM hItem, UINT nMask, LPCTSTR lpszItem, int nImage, int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam);
//		BOOL SetItem(TV_ITEM *pTVI);
//	
//
// Below you will find functions that could be found in CTreeCtrl but provide 
// additional functionality and some helper functions
//
//		HTREEITEM GetNextItem(HTREEITEM hItem, UINT nCode);
//		// --- In  : similar to CTreeCtrl but has extended codes
//					TVGN_FIRSTSELECTED
//					TVGN_NEXTSELECTED
//					TVGN_PREVSELECTED
//					TVGN_FIRSTDISABLED
//					TVGN_NEXTDISABLED
//					TVGN_PREVDISABLED
//					TVGN_FIRSTHIDDEN
//					TVGN_NEXTHIDDEN
//					TVGN_PREVHIDDEN
//		HTREEITEM GetPrevHiddenItem(HTREEITEM hItem);
//		// --- Returns :previous hidden item
//		HTREEITEM GetNextHiddenItem(HTREEITEM hItem);
//		// --- Returns :next hidden item
//		HTREEITEM GetFirstHiddenItem();
//		// --- Returns :first hidden item if any, otherwise NULL
//
//
// And plus you can hide items and make them disable, 
//
//		BOOL IsItemDisabled(HTREEITEM hItem);
//		// --- Returns :TRUE if the specified item is disabled
//						otherwise FALSE.
//		BOOL DisableItem(HTREEITEM hItem,BOOL bDisable);
//		BOOL IsItemHidden(HTREEITEM hItem);
//		// --- Returns :TRUE if the specified item is hidden
//						otherwise FALSE.
//		BOOL HideItem(HTREEITEM hItem,BOOL bHide=TRUE);
//
//
// As long as control provides multicolumn functionality, we introduce here
// subitems - elements located on the same row but in different columns. 
// You can set image, text color and font to any subitems in the control
// using next function:
//
//		BOOL SetSubItem(HTREEITEM hItem,int nColumn,UINT uFlags,
//						LPCTSTR lpszText,int nImage=0, CFont *pFont=0,COLORREF clr=0);
//		//uFlags: one or more of following:
//		//		OX_SUBITEM_IMAGE	- nImage parameter is valid
//		//		OX_SUBITEM_FONT		- pFont parameter is valid
//		//		OX_SUBITEM_COLOR	- clr parameter is valid
//		//		OX_SUBITEM_TEXT		- lpszText parameter is valid
//										subitem attributes
//
// Below you will find functions that could be used to manipulate items/subitems
//
//		CStringArray& GetItemTextEx(HTREEITEM hItem,int nCol=0);
//		// --- Returns : array of strings using in extended edit modes
//						as following:
//						if EditMode is OXET_COMBO this array contain strings to fill
//							in-place combobox
//						if EditMode is OXET_CALENDAR this array contain min and max dates
//							available to pick up throuth in-place date picker 
//							(COXTreeCal class)
//		void SetItemTextEx(HTREEITEM hItem,CStringArray& saTextEx,int nCol=0);
//		// --- In  : item's handle, array of strings using in 
//						extended edit modes (see GetItemTextEx function above)
//						and column (subitem) number 
//		void SetEditMode(HTREEITEM hItem,UINT uMode,CStringArray& saTextEx,int nCol=0);
//		// --- In  : Item's handle,
//					edit mode code as following:
//						OXET_EDIT - plain edit 
//						OXET_NOEDIT - editing is not allowed 
//						OXET_COMBO - in-place combobox
//						OXET_CALENDAR	- in-place calendar (date picker)
//							extended strings depending of edit mode 
//					array of strings that is used to fill combo box (OXET_COMBO) 
//						or to define min and max date for calendar control (OXET_CALENDAR),
//						if OXET_EDIT or OXET_NOEDIT is used than this array is ignored
//		void SetPlainEditMode(HTREEITEM hItem,int nCol=0);
//		// --- Effect : set item's edit mode to OXET_EDIT
//		void SetNoEditMode(HTREEITEM hItem,int nCol=0);
//		// --- Effect : set item's edit mode to OXET_NOEDIT
//		CString GetItemText(HTREEITEM hItem,int nCol=0);
//		// --- In  : similar to CTreeCtrl but has extra parameter 
//						specified column(subitem) - nCol
//		BOOL SetItemText(HTREEITEM hItem, LPCTSTR lpszItem ,int nCol=0);
//		// --- In  : similar to CTreeCtrl but receive subitem (column) number too
//		BOOL SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage,int nCol=0);
//		// --- In  : similar to CTreeCtrl but has extra parameter 
//					specified column(subitem) - nCol
//		BOOL GetItemImage(HTREEITEM hItem,int &nImg,int &nSelImg,int nCol=0);
//		// --- In  :  similar to CTreeCtrl but has extra parameter 
//						specified column(subitem) - nCol
//
//
// You can even set font and color of text to any item/subitem:
//		CFont* GetItemFont(HTREEITEM hItem,int nCol=0);
//		//--- Effect : font used to draw the item or NULL if font is not set
//		BOOL SetItemFont(HTREEITEM hItem,CFont *pFont,int nCol=0);
//		//--- Effect : set font to tree item
//		COLORREF GetItemColor(HTREEITEM hItem,int nCol=0);
//		// --- Returns :color for specified item
//		BOOL SetItemColor(HTREEITEM hItem,COLORREF clr,int nCol=0);
//		//--- Effect : set color for specified item
//
//
//	Sorting
// 
// If extended style TVOXS_COLUMNHDR is set and TVOXS_NOSORTHEADER isn't then 
// header control can be used to sort items forward & backward (just click on header). 
// Also we provide visual hints that show sort order. 
// 
// There are functions that let you sort items in COXTreeCtrl and get infornation about 
// currently sorted items:
//
//		BOOL SortChildren(HTREEITEM hti, int nCol=0, BOOL bAscending=TRUE, PFNTVCOMPARE lpfnCompare=NULL, LPARAM lParam=NULL);
//		// --- In  :	similar to CTreeCtrl plus next arguments:
//		//				nCol		-	number of column
//		//				bAscending	-	sort direction (TRUE - ascending, 
//		//												FALSE - descending)
//		//				lpfnCompare	-	Pointer to an application-defined comparison
//		//								function. It is called during a sort 
//		//								operation each time the relative order of 
//		//								two tree items needs to be compared. The 
//		//								comparison function has the following form:
//		//	
//		//								int CALLBACK CompareFunc(LPARAM lParam1, 
//		//														LPARAM lParam2, 
//		//														LPARAM lParamSort);
//		//
//		//								The comparison function must return a 
//		//								negative value if the first item should 
//		//								precede the second, a positive value if the 
//		//								first item should follow the second, or 
//		//								zero if the two items are equivalent. The 
//		//								lParam1 and lParam2 parameters correspond 
//		//								to the lParam member of the TV_ITEM 
//		//								structure for the two items being compared. 
//		//								The lParamSort parameter corresponds to the 
//		//								lParam argument of the SortChildren function.
//		//				lParam		-	parameter that will be sent to lpfnCompare
//		//								as lParamSort argument
//		BOOL SortChildrenCB(LPTV_SORTCB pSort, int nCol=0);
//		// --- In  :	similar to CTreeCtrl
//		//				nCol	-	number of 
//		// --- Out : 
//		// --- Returns:
//		// --- Effect : 
//
//		int GetSortCol();
//		// --- Returns: the number of sorted column if any exist, or -1 otherwise
//
//		int GetSortOrder();
//		// --- Returns: sort order:
//		//				1	-	ascending
//		//				-1	-	descending
//		//				0	-	not sorted
//
//
//
//
//	General Purpose
//
// Below you will find functions that could be useful while manipulating 
// COXTreeCtrl. E.g. if you change the font of some item(s) you will probably 
// want to change the height of items (unfortunately there is no way to set
// different height for different items). In that case SetItemHeight function
// is really useful. 
//
//		int GetItemIndex(HTREEITEM hItem);
//		// --- Returns:index of tree item in the control if this item is visible
//		int HitTest(CPoint pt, UINT* pFlags=NULL);
//		// --- In  : similar to CTreeCtrl
//		BOOL Expand(HTREEITEM hItem, UINT nCode);
//		// --- In  : similar to CTreeCtrl
//		UINT GetItemHeight();
//		// --- Returns : height of item (the same for all items)
//		void SetItemHeight(UINT nHeight);
//		// --- In  ::nHeight - item's height (the same for all items)
//		//--- Effect : set height of all items
//		COXTreeItem* GetXItem(HTREEITEM hti);
//		// --- Returns : pointer to the item that is used internally 
//							and provides all extended functionality
//		BOOL ResizeColToFit(int nCol);
//		// --- Returns : Nonzero if successful; otherwise 0.
//		// --- Effect : Sets the width of specified column to the width 
//						of the widest cell in this column out of all items.
//
//		CImageList* CreateDragImage(HTREEITEM hti, BOOL bCreateMultipleImage=TRUE);
//		// --- In  :	similar to CTreeCtrl
//		//				bCreateMultipleImage	-	if TRUE then when multiple items are 
//		//											selected all of them will be drawn in
//		//											resulting drag image
//		// --- Effect : creates drag image
//
/////////////////////////////////////////////////////////////////////////

#ifndef _OXTREECTRL_H
#define _OXTREECTRL_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include <afxtempl.h>

#include "OXTreeItem.h"
#include "OXTreeEdit.h"
#include "OXTreeHeader.h"
#include "OXTreeCombo.h"
#include "OXTreeCal.h"

// By default we include item tips functionality, but you have to include in 
// your project next files in order to to get it:
//		"OXHookWnd.cpp"	
//		"OXItemTip.cpp"	
//		"OXItemTipWnd.cpp"	
// 
// If you are not interested in item tips then you can declare in your "stdafx.h"
// (or whatever place is appropriate) next definition:
//		#define OX_TREECTRL_NOITEMTIPS
// 
#ifndef OX_TREECTRL_NOITEMTIPS
#include "OXItemTip.h"
#endif


#define IDT_OXTREECTRL_CHECKFORKEYBOARDINPUT		237
#define ID_OXTREECTRL_CHECKFORKEYBOARDINPUT_DELAY	500

#define STATEIMAGEMASKTOINDEX(i) (((i) >> 12) & 0xF)

// extended tree control style

// multiple selection
#define TVOXS_MULTISEL			0x00080000
// column header 
#define TVOXS_COLUMNHDR			0x00100000
// row selection mode
#define TVOXS_ROWSEL			0x00200000
// horizontal grid
#define TVOXS_HGRID				0x00400000
// vertical grid
#define TVOXS_VGRID				0x00800000
// full length grid lines
#define TVOXS_FLGRID			0x01000000
// select tree control item clicking anywhere
#define TVOXS_PICKANYWHERE		0x02000000
// disables scrolling. All items must be within the client area. 
#define TVOXS_NOSCROLL			0x04000000
// specifies that column headers do not work like buttons. This style is useful 
// if clicking a column header does not carry out an action, such as sorting.
#define TVOXS_NOSORTHEADER		0x08000000
// specifies that the control does not take	ownership of the image lists assigned to
// it (that is, it does not destroy the image lists when it is destroyed). This 
// style enables the same image lists to be used with multiple extended tree controls.
#define TVOXS_SHAREIMAGELISTS	0x10000000
//item tips for partially hidden items
#define TVOXS_ITEMTIPS			0x20000000
// columns extend horizontally to fill the entire area
#define TVOXS_EXTENDCOLUMNS		0x40000000
// Do not draw the focus rectangle of the item
#define TVOXS_NOFOCUSRECT		0x80000000
// Force the edit control to take up entire column width
#define TVOXS_KEEPCOLUMNSIZE	0x00040000

// extended GetNextItem codes
//
#define TVGN_FIRSTSELECTED	0x30
#define TVGN_NEXTSELECTED	0x31
#define TVGN_PREVSELECTED	0x32
#define TVGN_FIRSTDISABLED	0x33
#define TVGN_NEXTDISABLED	0x34
#define TVGN_PREVDISABLED	0x35
#define TVGN_FIRSTHIDDEN	0x36
#define TVGN_NEXTHIDDEN		0x37
#define TVGN_PREVHIDDEN		0x38

#define WM_USER_ADJUSTLASTCOLUMN		(WM_USER + 91)

#define WM_USER_QUERY_PROPERTIESWND		(WM_USER + 92)	// This message is sent to query
														// whether this is a properties window

// minimum indent width
#define TV_MININDENT		15//19
 

class OX_CLASS_DECL COXTreeCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(COXTreeCtrl)

	friend class COXTreeItem;
	friend class COXPropertiesWnd;

public:
	// --- In  : 
	// --- Out : 
	// --- Returns :
	//--- Effect : constructor
	COXTreeCtrl();

	// --- In  : 
	// --- Out : 
	// --- Returns :
	//--- Effect : destructor
	virtual ~COXTreeCtrl();


	// --- In  :	dwStyle		-	Specifies the extended tree control's style. Apply any 
	//								combination of extended tree control styles to the 
	//								control. Next styles could be applied:
	//
	//						TVS_HASLINES		-	The extended tree control has lines 
	//												linking child items to their 
	//												corresponding parent items.
	//						TVS_LINESATROOT		-	The extended tree control has lines 
	//												linking child items to the root of the 
	//												hierarchy.
	//						TVS_HASBUTTONS		-	The extended tree control adds a button 
	//												to the left of each parent item.
	//						TVS_EDITLABELS		-	The extended tree control allows the 
	//												user to edit the labels of tree view 
	//												items.
	//						TVS_SHOWSELALWAYS	-	Causes a selected item to remain 
	//												selected when the extended tree control 
	//												loses focus.
	//
	//				rect		-	Specifies the extended tree control's size and position. 
	//								It can be either a CRect object or a RECT structure.
	//				pParentWnd	-	Specifies the extended tree control's parent window, 
	//								usually a CDialog. It must not be NULL.
	//				nID			-	Specifies the extended tree control's ID.
	//				dwExStyle	-	Specifies the extended tree control's extended style. 
	//								Apply any combination of extended tree control extended 
	//								styles to the control. Next styles could be applied:
	//
	//						TVOXS_MULTISEL		-	Multiple selection support
	//						TVOXS_COLUMNHDR		-	Specifies that a column header is 
	//												displayed. By default, columns have 
	//												headers.
	//						TVOXS_ROWSEL		-	Full row selection mode
	//						TVOXS_HGRID			-	If set then horizontal grid will be drawn
	//						TVOXS_VGRID			-	If set then vertical grid will be drawn
	//						TVOXS_FLGRID		-	If TVOXS_HGRID style is set then lines 
	//												will be drawn to full extent of the 
	//												control
	//						TVOXS_PICKANYWHERE	-	Provides selecting tree control item 
	//												clicking anywhere within item rectangle 
	//						TVOXS_NOSCROLL		-	Disables scrolling. All items must be 
	//												within the client area. 
	//						TVOXS_NOSORTHEADER	-	Specifies that column headers do not 
	//												work like buttons. This style is useful 
	//												if clicking a column header does not 
	//												carry out an action, such as sorting.
	//						TVOXS_SHAREIMAGELISTS-	Specifies that the control does not take
	//												ownership of the image lists assigned to
	//												it (that is, it does not destroy the 
	//												image lists when it is destroyed). This 
	//												style enables the same image lists to 
	//												be used with multiple list view controls.
	//						TVOXS_ITEMTIPS		-	Item tips for partially hidden items
	//
	// --- Out : 
	// --- Returns:	TRUE if successful; otherwise FALSE
	// --- Effect : create extended tree control. 
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, 
		DWORD dwExStyle=0);


	// --- In  : item's handle and column(subitem) number to locate tree item
	// --- Out : 
	// --- Returns : array of strings using in extended edit modes
	//					as following:
	//					if EditMode is OXET_COMBO this array contain strings to fill
	//						in-place combobox
	//					if EditMode is OXET_CALENDAR this array contain min and max dates
	//						available to pick up throuth in-place date 
	//						picker (COXTreeCal class)
	// --- Effect : 
	CStringArray& GetItemTextEx(HTREEITEM hItem,int nCol=0);

	// --- In  : item's handle, array of strings using in 
	//			extended edit modes (see GetItemTextEx function above)
	//			and column (subitem) number 
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	void SetItemTextEx(HTREEITEM hItem,CStringArray& saTextEx,int nCol=0);


	// --- In  : subitem (column) number
	// --- Out : 
	// --- Returns : Nonzero if successful; otherwise 0.
	// --- Effect : Sets the width of specified column to the width of the widest cell
	//				in this column out of all items.
	BOOL ResizeColToFit(int nCol);


	// --- In  : Item's handle,
	//			edit mode code as following:
	//				OXET_EDIT - plain edit 
	//				OXET_NOEDIT - editing is not allowed 
	//				OXET_COMBO - in-place combobox
	//				OXET_CALENDAR	- in-place calendar (date picker)
	//					extended strings depending of edit mode 
	//				array of strings that is used to fill combo box (OXET_COMBO) 
	//				or to define min and max date for calendar control (OXET_CALENDAR),
	//				if OXET_EDIT or OXET_NOEDIT is used than this array is ignored
	//			column(subitem) number
	// --- Out : 
	// --- Returns :
	// --- Effect : set item's edit mode
	void SetEditMode(HTREEITEM hItem,UINT uMode,CStringArray& saTextEx,int nCol=0);

	// --- In  : Item's handle,
	//			column(subitem) number
	// --- Out : 
	// --- Returns :
	// --- Effect : set item's edit mode to OXET_EDIT
	void SetPlainEditMode(HTREEITEM hItem,int nCol=0);

	// --- In  : Item's handle,
	//			column(subitem) number
	// --- Out : 
	// --- Returns :
	// --- Effect : set item's edit mode to OXET_NOEDIT
	void SetNoEditMode(HTREEITEM hItem,int nCol=0);


	// --- In  : similar to CTreeCtrl but has extra parameter specified column(subitem) - nCol
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	CString GetItemText(HTREEITEM hItem,int nCol=0) const;
	
	// --- In  : similar to CTreeCtrl but receive subitem (column) number too
	// --- Out : 
	// --- Returns : Nonzero if successful; otherwise 0.
	//--- Effect : Sets the text of the item specified by hItem.
	BOOL SetItemText(HTREEITEM hItem, LPCTSTR lpszItem ,int nCol=0);


	// --- In  : Handle of the item
	// --- Out : 
	// --- Returns :TRUE if the specified item is disabled
	//				otherwise FALSE.
	//--- Effect :
	BOOL IsItemDisabled(HTREEITEM hItem) const;

	// --- In  :  hItem -Handle of the item
				//bDisable - disable (TRUE) or undisable (FALSE)
				//of specified item
	// --- Out : 
	// --- Returns : Nonzero if successful; otherwise 0.
	//--- Effect : disable or undisable specified tree item
	BOOL DisableItem(HTREEITEM hItem,BOOL bDisable);

	
	// --- In  : Handle of the item
	// --- Out : 
	// --- Returns :TRUE if the specified item is hidden
	// otherwise FALSE.
	//--- Effect :
	BOOL IsItemHidden(HTREEITEM hItem) const;

	// --- In  : hItem - handle of item
				// bHide TRUE to hide and FALSE to unhide specified item
	// --- Out : 
	// --- Returns :
	//--- Effect : hide/unhide specified item
	BOOL HideItem(HTREEITEM hItem,BOOL bHide=TRUE);


	// --- In  :	hParentItem	-	handle to parent item
	//				hItem		-	handle to descendant item
	// --- Out : 
	// --- Returns:
	// --- Effect:	TRUE if hItem is descendant of hParentItem
	BOOL IsItemDescendant(HTREEITEM hParentItem, HTREEITEM hItem);


	// --- In  : hItem -Handle of the item
	//			 nCol - column(subitem) number 
	// --- Out : 
	// --- Returns :
	//--- Effect : font used to draw the item or NULL if font is not set
	CFont* GetItemFont(HTREEITEM hItem,int nCol=0);

	// --- In  : hItem -Handle of the item
			//pFont - specifies the new font for item
	// --- Out : 
	// --- Returns :
	//--- Effect : set font to tree item
	BOOL SetItemFont(HTREEITEM hItem,CFont *pFont,int nCol=0);

	// --- In  :hItem -Handle of the item
			//  nCol - column(subitem) number
	// --- Out :
	// --- Returns :color for specified item
	//--- Effect : 
	COLORREF GetItemColor(HTREEITEM hItem,int nCol=0) const;

	// --- In  ::hItem -Handle of the item
	//			 clr - color for specified item
	//			 nCol - column(subitem) number 
	// --- Out : 
	// --- Returns : Nonzero if successful; otherwise 0.
	//--- Effect : set color for specified item
	BOOL SetItemColor(HTREEITEM hItem,COLORREF clr,int nCol=0);

	// --- In  ::hItem -Handle of the item
	//			 clr - color for specified item
	//			 nCol - column(subitem) number 
	// --- Out : 
	// --- Returns : Nonzero if successful; otherwise 0.
	//--- Effect : set background color for specified item
	BOOL SetItemBackColor(HTREEITEM hItem,COLORREF clr,int nCol=0);

	
	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	BOOL GetItemRect(HTREEITEM hItem, LPRECT lpRect, BOOL bTextOnly) const;

	
	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	BOOL SetItemData(HTREEITEM hItem, DWORD dwData);

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	DWORD GetItemData(HTREEITEM hItem) const;


	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	BOOL SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask);

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	UINT GetItemState(HTREEITEM hItem, UINT nStateMask) const;

	
	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	HTREEITEM GetRootItem() const;

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	HTREEITEM GetDropHilightItem();

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	HTREEITEM GetParentItem(HTREEITEM hItem);

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	HTREEITEM GetPrevSiblingItem(HTREEITEM hItem);

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	HTREEITEM GetNextSiblingItem(HTREEITEM hItem);

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	HTREEITEM GetChildItem(HTREEITEM hItem);


	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	BOOL ItemHasChildren(HTREEITEM hItem) const;
	
	
	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	UINT GetIndent() const;

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	void SetIndent(UINT nIndent);

	// --- In  :	hItem	-	handle of tree item
	// --- Out : 
	// --- Returns:
	// --- Effect : 
	inline int GetItemIndent(HTREEITEM hItem) const
	{
		ASSERT(hItem!=NULL);
		return GetItemIndent(GetXItem(hItem));
	}

	// --- In  :
	// --- Out : 
	// --- Returns : A reference to the COXTreeEdit which is used for text editing
	//--- Effect : 
	inline COXTreeEdit& GetTreeEdit()
	{
		return m_wndEdit;
	}

	// --- In  :  similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	CImageList* GetImageList(UINT nImage);

	
	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	HTREEITEM GetSelectedItem();

	
	// --- In  : handle of item
	// --- Out : 
	// --- Returns :previous hidden item
	//--- Effect : 
	HTREEITEM GetPrevHiddenItem(HTREEITEM hItem);

	// --- In  : handle of item
	// --- Out : 
	// --- Returns :next hidden item
	//--- Effect : 
	HTREEITEM GetNextHiddenItem(HTREEITEM hItem);

	// --- In  : 
	// --- Out : 
	// --- Returns :first hidden item if any, otherwise NULL
	//--- Effect : 
	HTREEITEM GetFirstHiddenItem();

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	HTREEITEM GetPrevVisibleItem(HTREEITEM hItem, BOOL bCurrentlyViewable=FALSE);

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	HTREEITEM GetNextVisibleItem(HTREEITEM hItem, BOOL bCurrentlyViewable=FALSE);

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	HTREEITEM GetFirstVisibleItem();

	// --- In  : similar to CTreeCtrl but has extended codes
	//			(see above)
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	HTREEITEM GetNextItem(HTREEITEM hItem, UINT nCode);

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	BOOL DeleteItem(HTREEITEM hItem);

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns:
	// --- Effect : 
	BOOL DeleteAllItems();


	// --- In  :	similar to CTreeCtrl
	// --- Out : 
	// --- Returns:
	// --- Effect : 
	BOOL DeleteColumn(int nCol);


	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns:
	// --- Effect : 
	UINT GetVisibleCount() const;

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns:
	// --- Effect : 
	UINT GetCount() const;
	
	
	// --- In :	hItem - handle of item
	//			nColumn - column(subitem) number
	//			uFlags: one or more of following:
	//				OX_SUBITEM_IMAGE	- nImage parameter is valid
	//				OX_SUBITEM_FONT		- pFont parameter is valid
	//				OX_SUBITEM_COLOR	- clr parameter is valid
	//				OX_SUBITEM_BGCOLOR	- clrBack parameter is valid
	//				OX_SUBITEM_TEXT		- lpszText parameter is valid
	//									subitem attributes
	//			lpszText	- text
	//			nImage		- image
	//			pFont		- font
	//			clr 		- color
	// --- Out : 
	// --- Returns : Nonzero if successful; otherwise 0.
	//--- Effect : set specified attributes for subitem
	BOOL SetSubItem(HTREEITEM hItem,int nColumn,UINT uFlags,LPCTSTR lpszText,
		int nImage=0,CFont *pFont=0,COLORREF clr=0, COLORREF clrBack = 0);


	// --- In  : similar to CListCtrl (note that column #0 is added automatically)
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat=LVCFMT_LEFT, 
		int nWidth=-1,int nSubItem =-1);

	// --- In  : similar to CListCtrl (note that column #0 is added automatically)
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	int InsertColumn(int nCol, const LV_COLUMN* pColumn);


	// --- In  : 
	// --- Out : 
	// --- Returns :extended tree control style (see above)
	//--- Effect : 
	DWORD GetExStyle() const;

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : modified the extended tree control style 
	BOOL ModifyExStyle(DWORD dwExStyleRemove,DWORD dwExStyleAdd,UINT nFlags=0);

	// --- In  :similar to CTreeCtrl 
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	DWORD GetStyle() const;

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags=0);


	// --- In  : similar to CTreeCtrl but has extra parameter specified column(subitem) - nCol
	// --- Out : 
	// --- Returns : Nonzero if successful; otherwise 0.
	//--- Effect : set image(s) to specified item/subitem
	BOOL SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage,int nCol=0);

	// --- In  :  similar to CTreeCtrl but has extra parameter specified column(subitem) - nCol
	// --- Out : similar to CTreeCtrl
	// --- Returns :Nonzero if successful; otherwise 0.
	//--- Effect : 
	BOOL GetItemImage(HTREEITEM hItem,int &nImg,int &nSelImg,int nCol=0);


	// --- In  :	hItem			-	tree item handle
	//				nCol			-	index of column
	//				nEllipsisFormat	-	ellipsis format used to draw the specified 
	//									item in the column nCol, can be one of:
	//
	//									DT_END_ELLIPSIS		Replaces the end of the 
	//														item text with ellipses, 
	//														if necessary, so that the 
	//														result fits in the item
	//														rectangle	
	//									DT_PATH_ELLIPSIS	Replaces the middle of the 
	//														item text with ellipses, 
	//														if necessary, so that the 
	//														result fits in the item
	//														rectangle
	//									DT_WORD_ELLIPSIS	the same as 
	//														DT_END_ELLIPSIS
	// --- Out : 
	// --- Returns:	TRUE if success, or FALSE otherwise
	// --- Effect : set ellipsis format used when item is being drawn
	BOOL SetItemDrawEllipsis(HTREEITEM hItem, UINT nEllipsisFormat, int nCol=0);

	// --- In  :	hItem			-	tree item handle
	//				nCol			-	index of column
	// --- Out :	nEllipsisFormat	-	ellipsis format used to draw the specified 
	//									item in the column nCol, can be one of:
	//
	//									DT_END_ELLIPSIS		Replaces the end of the 
	//														item text with ellipses, 
	//														if necessary, so that the 
	//														result fits in the item
	//														rectangle	
	//									DT_PATH_ELLIPSIS	Replaces the middle of the 
	//														item text with ellipses, 
	//														if necessary, so that the 
	//														result fits in the item
	//														rectangle
	//									DT_WORD_ELLIPSIS	the same as 
	//														DT_END_ELLIPSIS
	// --- Returns:	TRUE if success, or FALSE otherwise
	// --- Effect : get ellipsis format used when item is being drawn
	BOOL GetItemDrawEllipsis(HTREEITEM hItem, UINT& nEllipsisFormat, int nCol=0) const;


	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	CImageList* SetImageList(CImageList* pImageList, int nImageList);


	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns:
	// --- Effect : 
	HTREEITEM InsertItem(LPCTSTR lpszItem, HTREEITEM hParent=TVI_ROOT, 
		HTREEITEM hInsertAfter=TVI_LAST);

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns:
	// --- Effect : 
	HTREEITEM InsertItem(LPCTSTR lpszItem, int nImage, int nSelectedImage, 
		HTREEITEM hParent=TVI_ROOT, HTREEITEM hInsertAfter=TVI_LAST);

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns:
	// --- Effect : 
	HTREEITEM InsertItem(UINT nMask, LPCTSTR lpszItem, int nImage, 
		int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam, 
		HTREEITEM hParent, HTREEITEM hInsertAfter);

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns:
	// --- Effect : 
	HTREEITEM InsertItem(LPTV_INSERTSTRUCT lpInsertStruct);

	
	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	BOOL GetItem(TV_ITEM *pTVI) const;

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	BOOL SetItem(HTREEITEM hItem, UINT nMask, LPCTSTR lpszItem, int nImage, int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam);

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	BOOL SetItem(TV_ITEM *pTVI);
	

	// --- In  : handle to the item 
	// --- Out : 
	// --- Returns :index of tree item inlist control if this item is visible
	//--- Effect : for internal usage of helper classes
	int GetItemIndex(HTREEITEM hItem) const;

	
	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	int HitTest(CPoint pt, UINT* pFlags=NULL) const;

	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	HTREEITEM HitTest(TVHITTESTINFO* pHitTestInfo) const;


	// --- In  :
	// --- Out : 
	// --- Returns : whether user can click anywhere to select an item (TRUE) 
	//				or only by clicking on the items in the first column (FALSE)
	// --- Effect : 
	BOOL GetPickAnywhere() const;

	// --- In  :
	// --- Out : 
	// --- Returns : whether draw horizontal gridlines to full extent (TRUE) 
	//				or to size of all items (FALSE)
	// --- Effect : 
	BOOL GetDrawGridFullLength() const;

	// --- In  :horizontal (TVOXS_HGRID) or vertical (TVOXS_VGRID) 
	//			grid style
	// --- Out : 
	// --- Returns :whether this grid style applied (TRUE) or not (FALSE)
	// --- Effect : 
	BOOL HasGrid(UINT nGridType) const;

	// --- In  : 
	// --- Out : 
	// --- Returns : whether TVS_LINESATROOT style set or not
	//--- Effect : 
	BOOL HasLinesAtRoot() const;

	// --- In  : 
	// --- Out : 
	// --- Returns :whether TVS_HASLINES style set or not
	//--- Effect : 
	BOOL HasLines() const;

	// --- In  : 
	// --- Out : 
	// --- Returns :whether TVS_HASBUTTONS style set or not
	//--- Effect : 
	BOOL HasButtons() const;

	// --- In  : 
	// --- Out : 
	// --- Returns :whether TVS_SHOWSELALWAYS style set or not
	//--- Effect : 
	BOOL GetShowSelAlways() const;


	// --- In  : similar to CTreeCtrl
	// --- Out : 
	// --- Returns :
	//--- Effect : 
	virtual BOOL Expand(HTREEITEM hItem, UINT nCode);


	// --- In  :
	// --- Out :
	// --- Returns : height of item (the same for all items)
	//--- Effect : 
	UINT GetItemHeight() const;

	// --- In  ::nHeight - item's height (the same for all items)
	// --- Out : 
	// --- Returns : 
	//--- Effect : set height of all items
	void SetItemHeight(UINT nHeight);


	// --- In  :: handle to the item
	// --- Out : 
	// --- Returns : pointer to the item that is used internally 
	//					and provides all extended functionality
	//--- Effect : set height of all items
	COXTreeItem* GetXItem(HTREEITEM hti) const;

	
	// --- In  :	hti	-	handle to the item;
	//				nCol-	number of column
	// --- Out : 
	// --- Returns: If successful, a pointer to the CEdit object that is used 
	//				to edit the item text; otherwise NULL
	// --- Effect : starts editing text in designated item and column
	CEdit* EditLabel(HTREEITEM hti, int nCol=0);


	// --- In  :	hti	-	handle to the tree item that will be made first visible
	// --- Out : 
	// --- Returns:
	// --- Effect : scroll the tree control in order to display the specified item 
	//				at the top
	BOOL EnsureFirstVisible(HTREEITEM hti);

	// --- In  :	similar to CTreeCtrl
	// --- Out : 
	// --- Returns:
	// --- Effect : 
	BOOL EnsureVisible(HTREEITEM hti);


	// --- In  :	similar to CTreeCtrl
	// --- Out : 
	// --- Returns:
	// --- Effect : if nCode == TVGN_FIRSTVISIBLE then function just 
	//				returns FALSE
	BOOL Select(HTREEITEM hti, UINT nCode);

	// --- In  :	similar to CTreeCtrl
	// --- Out : 
	// --- Returns:
	// --- Effect : 
	BOOL SelectItem(HTREEITEM hti);

	// --- In  :	similar to CTreeCtrl
	// --- Out : 
	// --- Returns:
	// --- Effect : 
	BOOL SelectDropTarget(HTREEITEM hti);

	// --- In  :	similar to CTreeCtrl
	// --- Out : 
	// --- Returns:	this function is added only for compability. Actually
	//				it always return FALSE.	
	// --- Effect : 
	BOOL SelectSetFirstVisible(HTREEITEM hti);

	// --- In  :	similar to CTreeCtrl
	//				bCreateMultipleImage	-	if TRUE then when multiple items are 
	//											selected all of them will be drawn in
	//											resulting drag image
	// --- Out : 
	// --- Returns:
	// --- Effect : 
	CImageList* CreateDragImage(HTREEITEM hti, BOOL bCreateMultipleImage=TRUE);


	// --- In  :	similar to CTreeCtrl plus next arguments:
	//				nCol		-	number of column
	//				bAscending	-	sort direction (TRUE - ascending, 
	//												FALSE - descending)
	//				lpfnCompare	-	Pointer to an application-defined comparison
	//								function. It is called during a sort 
	//								operation each time the relative order of 
	//								two tree items needs to be compared. The 
	//								comparison function has the following form:
	//	
	//								int CALLBACK CompareFunc(LPARAM lParam1, 
	//														LPARAM lParam2, 
	//														LPARAM lParamSort);
	//
	//								The comparison function must return a 
	//								negative value if the first item should 
	//								precede the second, a positive value if the 
	//								first item should follow the second, or 
	//								zero if the two items are equivalent. The 
	//								lParam1 and lParam2 parameters correspond 
	//								to the lParam member of the TV_ITEM 
	//								structure for the two items being compared. 
	//								The lParamSort parameter corresponds to the 
	//								lParam argument of the SortChildren function.
	//								
	//								If NULL is passed this function will use the
	//								m_lpfnDefaultCompare function. If
	//								m_lpfnDefaultCompare is NULL as well then a 
	//								simple text comparison will be used,
	//
	//				lParam		-	parameter that will be sent to lpfnCompare
	//								as lParamSort argument
	// --- Out : 
	// --- Returns:
	// --- Effect : 
	virtual BOOL SortChildren(HTREEITEM hti, int nCol=0, BOOL bAscending=TRUE, 
		PFNTVCOMPARE lpfnCompare=NULL, LPARAM lParam=NULL);

	// --- In  : lpfnDefaultCompare	-
	//								Pointer to an application-defined comparison
	//								function. It is called during a sort 
	//								operation each time the relative order of 
	//								two tree items needs to be compared. The 
	//								comparison function has the following form:
	//	
	//								int CALLBACK CompareFunc(LPARAM lParam1, 
	//														LPARAM lParam2, 
	//														LPARAM lParamSort);
	//
	//								The comparison function must return a 
	//								negative value if the first item should 
	//								precede the second, a positive value if the 
	//								first item should follow the second, or 
	//								zero if the two items are equivalent. The 
	//								lParam1 and lParam2 parameters correspond 
	//								to the lParam member of the TV_ITEM 
	//								structure for the two items being compared. 
	//								The lParamSort parameter corresponds to the 
	//								lParam argument of the SortChildren function.
	//
	//								If a NULL value is passed a string comparison
	//								will be performed.
	// --- Out : 
	// --- Returns:
	// --- Effect : Allows you to specify a default compare function for all columns
	void SetDefaultCompareFunction(PFNTVCOMPARE lpfnDefaultCompare)
	{
		m_lpfnDefaultCompare = lpfnDefaultCompare;
	}

	// --- In  :	nCol	-		The column for which to use this compare function
	//				lpfnCompare	-	The compare function - same as SetDefaultCompareFunction 
	// --- Out : 
	// --- Returns:
	// --- Effect :  Allows you to specify a compare function for a column
	void SetColumnCompareFunction(int nCol, PFNTVCOMPARE lpfnDefaultCompare);
	
	// --- In  :	similar to CTreeCtrl
	//				nCol	-	number of 
	// --- Out : 
	// --- Returns:
	// --- Effect :
	BOOL SortChildrenCB(LPTV_SORTCB pSort, int nCol=0);

	// --- In  :	
	// --- Out : 
	// --- Returns: TRUE if sorting operation is supported in extended tree control,
	//				or FALSE otherwise
	// --- Effect : 
	BOOL IsHeaderSorting() const;

	// --- In  :	nIndex	-	index of tree item inlist control if this 
	//							item is visible;
	// --- Out : 
	// --- Returns: If successful, a pointer to the tree item, otherwise NULL
	// --- Effect : 
	HTREEITEM GetItemFromIndex(int nIndex) const;


	// --- In  :	
	// --- Out : 
	// --- Returns: the number of sorted column if any exist, or -1 otherwise
	// --- Effect : 
	int GetSortCol() const;

	// --- In  :	
	// --- Out : 
	// --- Returns: sort order:
	//				1	-	ascending
	//				-1	-	descending
	//				0	-	not sorted
	// --- Effect : 
	int GetSortOrder() const;


	// --- In  : 
	// --- Out : 
	// --- Returns:	The handle of the header control of this extended tree control
	//              or NULL when no header control exists
	// --- Effect : 
	HWND GetHeaderCtrlHandle();

	
	// --- In  : 
	// --- Out : 
	// --- Returns:	The number of columns in the extended tree control
	// --- Effect : 
	int GetColumnsCount() const;


	// --- In  :	hItemToCopy			-	handle of the item to copy
	//				hParent				-	future parent of the copying item
	//				hInsertAfter		-	item to insert after
	//				bCopyDescendants	-	if TRUE then all hItemToCopy descendants 
	//										will be copied too
	//				pDestinationTree	-	if not NULL the item will be copied
	//										to another instance of COXTreeCtrl
	// --- Out : 
	// --- Returns:	TRUE if succeeds, or FALSE otherwise
	// --- Effect : make copy of specified item and its descendants
	BOOL CopyItem(HTREEITEM hItemToCopy, HTREEITEM hParent=NULL, 
		HTREEITEM hInsertAfter=TVI_LAST, BOOL bCopyDescendants=TRUE,
		HTREEITEM* pNewItem=NULL, COXTreeCtrl* pDestinationTree = NULL);

	// --- In  :	hItemToMove			-	handle of the item to move
	//				hParent				-	future parent of the moved item
	//				hInsertAfter		-	item to insert after
	//				pDestinationTree	-	if not NULL the item will be moved
	//										to another instance of COXTreeCtrl
	// --- Out : 
	// --- Returns:	TRUE if succeeds, or FALSE otherwise
	// --- Effect : moves specified item and its descendants to new location
	BOOL MoveItem(HTREEITEM hItemToMove, HTREEITEM hParent=NULL, 
		HTREEITEM hInsertAfter=TVI_LAST,
		COXTreeCtrl* pDestinationTree=NULL);


	// --- In  :	hItemFirst	-	handle of the item from which start to redraw
	//								tree item(s)
	//				hItemLast	-	handle of the item that will be redrawn last
	// --- Out : 
	// --- Returns: Nonzero if successful; otherwise 0.
	// --- Effect : Redraw tree items
	inline void SetRedraw(BOOL bRedraw=TRUE)
	{
		if(bRedraw && m_nRedrawFlag>0)
		{
			m_nRedrawFlag--;
			if(m_nRedrawFlag==0)
				CListCtrl::SetRedraw(bRedraw);
		}

		if(!bRedraw)
		{
			m_nRedrawFlag++;
			CListCtrl::SetRedraw(bRedraw);
		}
	}

	
	// --- In  :	hItemFirst	-	handle to the first tree item to be redrawn
	//				hItemLast	-	handle of the last tree item to be redrawn
	// --- Out : 
	// --- Returns: Nonzero if successful; otherwise 0.
	// --- Effect : Redraws group of specified tree items
	BOOL RedrawTreeItems(HTREEITEM hItemFirst, HTREEITEM hItemLast);

	// --- In  :	hItem		-	handle to the tree item which 
	//								children must be deleted
	// --- Out : 
	// --- Returns: 
	// --- Effect : Deletes all childrens of the specified item
	inline void DeleteChildrenItems(HTREEITEM hItem) {
		DeleteChildrenItems(GetXItem(hItem));
	}

	// --- In  :	pItem		-	pointer to internal COXTreeItem object which 
	//								children must be deleted
	// --- Out : 
	// --- Returns: 
	// --- Effect : Deletes all childrens of the specified item
	void DeleteChildrenItems(COXTreeItem *pItem);

	// --- In  :	clr		-	the color of the horizontal grid lines
	//
	// --- Out : 
	// --- Returns: 
	// --- Effect : Sets the color of the horizontal grid lines
	void SetHorizontalGridColor(COLORREF clr);
	
	// --- In  :	clr		-	the color of the vertical grid lines
	//
	// --- Out : 
	// --- Returns: 
	// --- Effect : Sets the color of the vertical grid lines	
	void SetVerticalGridColor(COLORREF clr);

	// --- In  :	pDC		-	Device context
	//				rcButton-	The rectangle of the button to be drawn
	//				bExpand	-	True if the item is to be expanded
	//
	// --- Out : 
	// --- Returns: 
	// --- Effect : Override this function to draw the plus/minus button
	virtual void DrawButton(CDC *pDC,CRect& rcButton,BOOL bExpand=FALSE);


	// item part codes
	typedef enum {rButton,rImage,rStateImage,rText} eItemPartRect;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXTreeCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	afx_msg void OnStyleChanged( int nStyleType, LPSTYLESTRUCT lpStyleStruct );

	// Generated message map functions
protected:
	BOOL IsPropertiesWnd() const;
	BOOL m_bInit;
	COXTreeCal m_wndCal;
	COXTreeCombo m_wndCombo;
	COXTreeHeader m_wndHdr;
	int m_nEditColumn;
	CWnd* m_pActiveEditWnd;
	CPoint m_ptLastClick;
	COXTreeEdit m_wndEdit;
	DWORD m_dwTCStyle;
	CString m_sTempStr;
	CPen m_penLines;
	DWORD m_dwExStyle;
	int m_nIndent;
	mutable COXTreeItem m_xtiRoot;
	UINT m_nItemHeight;
	boolean m_bClick;
	BOOL m_bCanEdit;
	int m_nLastIndex;
	int m_nLastColumn;
	UINT m_nItemsCount;
	BOOL m_bCreatingDragImage;
	HTREEITEM m_hOldDropTarget;
	COLORREF m_clrHorizontalGrid;
	COLORREF m_clrVerticalGrid;
	PFNTVCOMPARE m_lpfnDefaultCompare;
	CMap<int, int, PFNTVCOMPARE, PFNTVCOMPARE> m_mapCompareFunctions;

	BOOL m_bFirstTimeHooking;
#ifndef OX_TREECTRL_NOITEMTIPS
	COXItemTip m_ItemTip;
#endif

	int m_nRedrawFlag;

	UINT m_nTimerCheckKeyboardInput;
	CString m_sSearchMask;

	BOOL SendItemExpandingNotify(HTREEITEM hitem,UINT nCode);
	void SendItemExpandedNotify(HTREEITEM hItem,UINT nCode);
	virtual CWnd* StartEditSubItem(HWND hWndEdit,COXTreeItem *xitem,
		CRect& rcEdit,int nCol,LRESULT* pResult);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	int GetEditColumn(COXTreeItem *xItem,CRect *rcColText);
	int GetFullColumnWidth(int nCol);
	void SendGetDispInfoNotify(TV_ITEM *pTVI) const;
	void SendSetDispInfoNotify(TV_ITEM* pTVI);
	void SendDeleteItemNotify(TV_ITEM* ptvi);
	void DrawItemImages(CDC *pDC,COXTreeItem* xti,int nItem,
		CRect& rcItem,int nCol,BOOL bRowSel);
	CSize GetColTextExtent(int nIndex,int nCol=0) const;
	void GetItemPartRect(COXTreeCtrl::eItemPartRect eType,
		COXTreeItem *pItem,CRect *rTarget,int *nIndent) const;
	int GetItemIndent(COXTreeItem *pItem) const;
	virtual BOOL Init();
	void CalcBtnRect(CRect * prcButton,CRect& rcStep);
	void DrawLine(CDC *pDC,CPen* pPen,CRect& rc,int nMode);
	void DrawLines(CDC *pDC,LPRECT rLines,COXTreeItem *pItem,int nLevel);
	int GetItemStateIndex(int nItem) const;
	void RefreshFocusItem();
	void DrawItemText(CDC *pDC,LPCTSTR sText,CRect& rcTxt,UINT nJustify,
		BOOL bDrawFocus,COLORREF clrFillOutside,UINT nEllipsisFormat);
	BOOL AddItemToCtrl(COXTreeItem *pItem);
	int SetItemAtPos(int pos,COXTreeItem *pItem);
	int GetItemIndexInternal(COXTreeItem *pItem) const;
	BOOL DeleteSubitems(HTREEITEM hParentItem, int nCol);
	// --- In  :	pointer to inner used item object 
	// --- Out : 
	// --- Returns:	index of tree item inlist control if this item is visible
	// --- Effect : for internal usage of helper classes
	virtual void  DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage( MSG* pMsg );


	//{{AFX_MSG(COXTreeCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnParentNotify(UINT message, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual LRESULT OnNeedItemTip(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserAdjustLastColumn(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif
