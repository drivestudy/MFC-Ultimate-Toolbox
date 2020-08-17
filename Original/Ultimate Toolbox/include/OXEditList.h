// ==========================================================================
// 					Class Specification : COXEditList
// ==========================================================================

// Header file : OXEditList.h

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
//	YES	Derived from COXGridList

//	YES	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	YES	Has a message map
//	YES	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption : 
//	Thsi class encapsulates a handy editable listbox like control
//  It creates a small toolbar with the buttons Add, Delete, Move Up and Move Down
//	The user can use this buttons or the keys Delete, Alt+Up, Alt+Down.
//	He can change already existing items by just entering the new text
//	The last entry is always empty and can be used to add new items

// Remark:
//	The last entry is always empty and should be regarded as part of the added items
//  So when retrieving information from this control you should keep this in mind

//	All the functionality of the toolbar is completely managed by this control
//	It is created as sibling of this control at the specified position

// Prerequisites (necessary conditions):
//	This control needs idle processing to update its GUI (enable/disabme buttons etc)
//	It uses the WM_IDLEUPDATECMDUI message for this.
//	This message is send during on idle to all the children of the main frame

//	This kind of idle processing is not available from a message box
//	Here you can use the not documented WM_KICKIDLE message of the dialog
//  to explicly call the OnIdle function of this control
//		#include <afxpriv.h>	// for WM_KICKIDLE
//		ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
//		LRESULT CMyDlg::OnKickIdle(WPARAM wParam, LPARAM lParam)
//			{
//			ASSERT(wParam == MSGF_DIALOGBOX);
//			LONG lIdleCount = lParam;
//			BOOL bContinueIdle = TRUE;
//		
//			// Update the edit list
//			m_wndEditList.OnIdle();
//		
//			return bContinueIdle;
//			}

// This control needs the toolbar resource with the ID IDR_EDITLIST_TOOLS
// It contains the buttons ID_EDITLIST_NEW, ID_EDITLIST_DELETE, ID_EDITLIST_UP and
// ID_EDITLIST_DOWN

//	This file uses resources.
//	The reserved ID ranges are : 23000 -> 23019 and 53000 -> 53019

/////////////////////////////////////////////////////////////////////////////

#ifndef __EDITLIST_H__
#define __EDITLIST_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include "OXGridList.h"
#include "OXMainRes.h"

#include "MFCToolBar.h"
#define CToolBar CMFCToolBar

class OX_CLASS_DECL COXEditListHeader : public CStatic
{
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXEditListHeader();

// Attributes
public:

protected:
	// text color
	COLORREF m_clrText;
	// color of top/left line of the border
	COLORREF m_clrTopLeft;
	// color of bottom/right line of the border
	COLORREF m_clrBottomRight;
	// flag that specifies whether the header should be drawn 
	// vertically (the control is horizontally oriented by default)
	BOOL m_bVertOriented;

private:
	// window's text
	CString m_sText;


// Operations
public:
	// --- In  :	clrText	-	color to be used to draw the control's text
	//				bRedraw	-	if TRUE then control will be repainted 
	//							right away
	// --- Out : 
	// --- Returns:	TRUE if success, otherwise FALSE
	// --- Effect : Sets the text color
	inline BOOL SetTextColor(COLORREF clrText=::GetSysColor(COLOR_WINDOWTEXT), 
		BOOL bRedraw=FALSE) 
	{ 
		if(bRedraw && m_clrText==clrText)
			bRedraw=FALSE;
		m_clrText=clrText;
		if(bRedraw)
			RedrawWindow();
		return TRUE;
	}

	// --- In  :	clrText	-	reference to COLORREF variable that will be set 
	//							to the color currently used to draw the 
	//							control's text
	// --- Out : 
	// --- Returns:	TRUE if success, otherwise FALSE
	// --- Effect : Retrieves the text color
	inline BOOL GetTextColor(COLORREF& clrText) const 
	{ 
		clrText=m_clrText; 
		return TRUE;
	}

	// --- In  :	clrTopLeft		-	color to be used to draw the top/left
	//									side of the header
	//				clrBottomRight	-	color to be used to draw the 
	//									bottom/right side of the header
	//				bRedraw			-	if TRUE then control will be repainted 
	//									right away
	// --- Out : 
	// --- Returns:	TRUE if success, otherwise FALSE
	// --- Effect : Sets the header color
	inline BOOL SetBorderColors(COLORREF clrTopLeft=::GetSysColor(COLOR_BTNSHADOW), 
		COLORREF clrBottomRight=::GetSysColor(COLOR_BTNHILIGHT), BOOL bRedraw=FALSE) 
	{ 
		if(bRedraw && m_clrTopLeft==clrTopLeft && 
			m_clrBottomRight==clrBottomRight)
			bRedraw=FALSE;
		m_clrTopLeft=clrTopLeft; 
		m_clrBottomRight=clrBottomRight;
		if(bRedraw)
			RedrawWindow();
		return TRUE;
	}

	// --- In  :	clrTopLeft		-	reference to COLORREF variable that 
	//									will be set to the color currently 
	//									used to draw the top/left side of the 
	//									header
	//				clrBottomRight	-	reference to COLORREF variable that 
	//									will be set to the color currently 
	//									used to draw the bottom/right side 
	//									of the header
	// --- Out : 
	// --- Returns:	TRUE if success, otherwise FALSE
	// --- Effect : Retrieves the header color
	inline BOOL GetBorderColors(COLORREF& clrTopLeft, 
		COLORREF& clrBottomRight) const 
	{ 
		clrTopLeft=m_clrTopLeft; 
		clrBottomRight=m_clrBottomRight;
		return TRUE;
	}

	// --- In  :	bVertOriented	-	if TRUE then header and control's text 
	//									will be drawn vertically, otherwise -
	//									horizontally (used by default).
	//				bRedraw			-	if TRUE then control will be repainted 
	//									right away
	// --- Out : 
	// --- Returns:	TRUE if success, otherwise FALSE
	// --- Effect : Sets the control orientation (vertical or horizontal)
	inline void SetVertOriented(BOOL bVertOriented=TRUE, BOOL bRedraw=FALSE)
	{
		if(bRedraw && m_bVertOriented==bVertOriented)
			bRedraw=FALSE;
		m_bVertOriented=bVertOriented;
		if(bRedraw)
			RedrawWindow();
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if the text and header will be drawn vertically, 
	//				FALSE if the text and header will be drawn horizontally
	// --- Effect : Retrieves the flag that specifies whether the control 
	//				is vertically or horizontally oriented
	inline BOOL IsVertOriented() const { return m_bVertOriented; }


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXEditListHeader)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXEditListHeader();
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Object's destructor

	// Generated message map functions
protected:
	//{{AFX_MSG(COXEditListHeader)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg LONG OnSetText(UINT wParam, LONG lParam);
	afx_msg LONG OnGetText(UINT wParam, LONG lParam);
	afx_msg LONG OnGetTextLength(UINT wParam, LONG lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



class OX_CLASS_DECL COXEditList : public COXGridList
{
// Data members -------------------------------------------------------------
public:
	enum EToolbarPosition
		{
		TBPNone = -1,
		TBPHorizontalTopLeft,
		TBPHorizontalTopCenter,
		TBPHorizontalTopRight,
		TBPHorizontalBottomLeft,
		TBPHorizontalBottomCenter,
		TBPHorizontalBottomRight,
		TBPVerticalLeftTop,
		TBPVerticalLeftCenter,
		TBPVerticalLeftBottom,
		TBPVerticalRightTop,
		TBPVerticalRightCenter,
		TBPVerticalRightBottom
		};
	static EToolbarPosition TBP_FIRST;
	static EToolbarPosition TBP_LAST;

protected:
	EToolbarPosition m_eToolbarPosition;
	BOOL m_bAllowDuplicates;
	BOOL m_bOrderedList;
	BOOL m_bIsDeleting;

	CToolBar m_toolbar;
	COXEditListHeader m_header;
	CString m_sHeaderText;

	BOOL m_bPostInitialized;
	CString m_sOriginalEditText;
	CString m_sDuplicateErrorMsg;
	int m_nNewImageItem;

	BOOL m_bEditable;

private:
	
// Member functions ---------------------------------------------------------
public:
	// --- In  : eToolbarPosition : The initial position of the toolbar
	//			 bAllowDuplicates : Whether duplicate text entries are allowwed or not
	//			 bOrderedList : Whether this is an ordered list 
	//				of which the user can move the items up or down
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object
	//				It will initialize the internal state
	COXEditList(EToolbarPosition eToolbarPosition = TBPHorizontalTopRight,
		BOOL bAllowDuplicates = FALSE, BOOL bOrderedList = TRUE);


	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Initializes this control
	virtual void InitGrid();

	// --- In  : lpDIS : A long pointer to a DRAWITEMSTRUCT structure that contains
	// 				information about the type of drawing required.
	// --- Out : 
	// --- Returns :
	// --- Effect : Called by CDialog::OnDrawItem for an ownerdrawn listview
	//				for every item that needs redrawing.
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);


	// --- In  : pszDuplicateErrorMsg : The new error text
	// --- Out : 
	// --- Returns : 
	// --- Effect : Sets a new error text which will be shown when a user adds
	//				an item whith a name that is already in use
	//				This message will only be shown if bAllowDuplicates == FALSE
	void SetDuplicateErrorMsg(LPCTSTR pszDuplicateErrorMsg);



	// --- In  :
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Start editing a new item
	//				This will popup an edit box on top of the last (empty) item
	BOOL EditNewItem();


	// --- In  :
	// --- Out : 
	// --- Returns : Whether you can call DeleteSelectedItems() to delete items
	//               E.g. you cannot delete items when nothing is selected
	// --- Effect : This function can be used to decide whether or not
	//              to enable certain parts of the GUI
	virtual BOOL CanDelete() const;

	// --- In  :
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Deletes all the items that are selected
	BOOL DeleteSelectedItems();


	// --- In  :
	// --- Out : 
	// --- Returns : Whether you can call MoveItemUp() to move an item up
	//               E.g. you move an item when no item has focus
	// --- Effect : This function can be used to decide whether or not
	//              to enable certain parts of the GUI
	virtual BOOL CanMoveUp() const;

	// --- In  :
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Moves the focussed item one position to the top
	BOOL MoveItemUp();


	// --- In  :
	// --- Out : 
	// --- Returns : Whether you can call MoveItemDown() to move an item down
	//               E.g. you move an item when no item has focus
	// --- Effect : This function can be used to decide whether or not
	//              to enable certain parts of the GUI
	virtual BOOL CanMoveDown() const;

	// --- In  :
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Moves the focussed item one position to the bottom
	BOOL MoveItemDown();


	// --- In  : nIndex1 : The zero-based index of the first item
	//			 nIndex2 : The zero-based index of the second item
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Swaps two items
	BOOL SwapItems(int nIndex1, int nIndex2);


	// --- In  : eToolbarPosition : The new toolbar position
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Sets the toolbar to a new position or 
	//              hides it (when eToolbarPosition == TBPNone)
	void PositionToolbar(EToolbarPosition eToolbarPosition = TBPHorizontalTopRight);


	// --- In  :
	// --- Out : 
	// --- Returns : The current toolbar position
	// --- Effect : 
	EToolbarPosition GetToolbarPosition() const;


	// --- In  :
	// --- Out : 
	// --- Returns : Whether furter idle processing is requested
	// --- Effect : This function should be called so that this control
	//              can update its GUI (enable/disable buttons etc.)
	BOOL OnIdle(LONG lCount = 0);


	// --- In  : nColumn : Zero-based index of the column used to sort upon
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Sorts the contents of the control by sorting a specific column
	virtual BOOL SortColumn(int nColumn = 0);


	// --- In		:	bAllowDuplicates	-	flag that specifies whether the 
	//											duplicate items are allowed to be
	//											typed in or not
	// --- Out		: 
	// --- Returns	:	
	// --- Effect	:	Sets the flag that specifies whether the duplicate items 
	//					are allowed to be typed in or not
	inline void SetAllowDuplication(BOOL bAllowDuplicates) { 
		m_bAllowDuplicates=bAllowDuplicates; 
	}

	// --- In		:	
	// --- Out		: 
	// --- Returns	:	Flag that specifies whether the duplicate items 
	//					are allowed to be typed in or not
	// --- Effect	:	Retrieves the flag that specifies whether the duplicate items 
	//					are allowed to be typed in or not
	inline BOOL GetAllowDupplication() const { return m_bAllowDuplicates; } 


	// --- In  : 
	// --- Out : 
	// --- Returns	: Pointer to CToolBar object
	// --- Effect	: Retrieves pointer to internal toolbar
	inline CToolBar* GetToolBar() { return &m_toolbar; }


	// --- In		:	bFlatToolBar	-	flag that specifies whether the internal 
	//										toolbar will be displayed flat or not
	// --- Out		: 
	// --- Returns	:	TRUE if function succeeds, or FALSE otherwise
	// --- Effect	:	Sets/Removes internal toolbar flat style
	inline BOOL SetFlatToolbar(BOOL bFlatToolBar) 
	{
		BOOL bResult=FALSE;
		if(::IsWindow(m_toolbar.GetSafeHwnd()))
		{
			if(bFlatToolBar)
				bResult=m_toolbar.ModifyStyle(NULL,TBSTYLE_FLAT);
			else
				bResult=m_toolbar.ModifyStyle(TBSTYLE_FLAT,NULL);
			if(bResult)
				m_toolbar.RedrawWindow();
		}
		return bResult;
	}


	// --- In  :	lpszHeaderText	-	list control header text
	// --- Out : 
	// --- Returns: 
	// --- Effect:	Sets text that will be displayed in the header
	inline void SetHeaderText(LPCTSTR lpszHeaderText) 
	{
		ASSERT(::IsWindow(m_header.GetSafeHwnd()));
		m_sHeaderText=lpszHeaderText;
		m_header.SetWindowText(m_sHeaderText);
	}

	// --- In  :	
	// --- Out : 
	// --- Returns: text that is displayed in the control's header
	// --- Effect:	Sets text that will be displayed in the header
	inline CString GetHeaderText() { return m_sHeaderText; }


	// --- In  :	bEdit	-	Allow edit or not
	//				nColumn	-	The column on which to allow editing (-1 = all columns).
	//							As long as edit list must have only one column this 
	//							argument is ignored
	// --- Out : 
	// --- Returns: 
	// --- Effect : Whether to allow items to be edited or not.	By default editing 
	//				is not allowed
	virtual void SetEditable(BOOL bEdit = TRUE, int nColumn = -1);

#ifdef _DEBUG
	virtual void Dump(CDumpContext&) const;
	virtual void AssertValid() const;
#endif //_DEBUG

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object
	virtual ~COXEditList();
													 
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(COXEditList)
	//}}AFX_VIRTUAL

protected:
	virtual BOOL IsFrameWnd() const;
	int GetImagesWidth();
	virtual void AdjustEmptyChanged();
	static BOOL SetButtonCount(CToolBar& toolbar, int nCount);
	static int CALLBACK EditListCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

// Generated message map functions
	//{{AFX_MSG(COXEditList)
	afx_msg BOOL OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNewItem();
	afx_msg void OnDeleteItem();
	afx_msg void OnUpdateDeleteItem(CCmdUI* pCmdUI);
	afx_msg void OnMoveItemUp();
	afx_msg void OnUpdateMoveItemUp(CCmdUI* pCmdUI);
	afx_msg void OnMoveItemDown();
	afx_msg void OnUpdateMoveItemDown(CCmdUI* pCmdUI);
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnToolTipText(UINT nControlID, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	//}}AFX_MSG
	afx_msg LRESULT OnPostInit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInsertItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteAllItems(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////

#endif      // __EDITLIST_H__
