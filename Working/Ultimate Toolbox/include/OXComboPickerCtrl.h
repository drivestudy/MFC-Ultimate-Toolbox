// ==========================================================================
//								Class Specification : 
//								COXComboPickerCtrl
// ==========================================================================

// Header file : OXComboPickerCtrl.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////


/*

	OVERVIEW
	The set of the classes to implement combo tree functionality
	contains 3 basic classes and a template class based on this classes
	to provide such functionality. These classes were designed in this way to
	allow more flexibility. So, we can use instead of dropdown 
	CTreeCtrl another dropdown controls like COXTreeCtrl or COXShellFolderTree
	etc.
	The main class in this set is a COXComboDropDown - this is an
	abstract class, where most function defined as pure virtual function.
	This class provides sizing, showing and another managment.
	Second, COXComboPickerCtrl is derived from CComboBox and has relationship
	with COXComboDropDown throw a pointer m_pDropDown to COXComboDropDown.
	This class handles "combo-part" stuff of this set of the clases.
	And last class, COXDropDownContainer is a parent for dropdown controls,
	(CTreeCtrl, for example).
	To create your own class based on this set, you have to derive
	your class from your dropdown control and from COXComboDropDown
	where you override pure virtual functions.
	To use CTreeCtrl derived classes you can use COXTreeComboDropDown<PARENTTREE>
	class.


Dependency:

	#include "OXComboPickerCtrl.h"


*/ 
#ifndef _OX_COMBOPICKERCTRL_H__
#define _OX_COMBOPICKERCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"
#include "OXMainRes.h"

#ifndef OXCOMBOPICKER_DEFAULTITEMHEIGHT
#define OXCOMBOPICKER_DEFAULTITEMHEIGHT		16
#endif


class COXComboPickerCtrl;

//////////////////////////////////////////////////////////////////////


class OX_CLASS_DECL COXDropDownContainer : public CWnd
{
	DECLARE_DYNAMIC(COXDropDownContainer);
// Construction
public:
	// --- In  : pAttachedWnd - A pointer to the CWnd object to be attached
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructor
	COXDropDownContainer(CWnd* pAttachedWnd=NULL) : m_pAttachedWnd(pAttachedWnd) {};

// Attributes
public:

protected:
	CWnd* m_pAttachedWnd;

// Operations
public:

	// --- In  :
	// --- Out : 
	// --- Returns : A pointer to the attached CWnd object
	// --- Effect : Returns a pointer to the attached object
	inline CWnd* GetAttachedWindow() const { return m_pAttachedWnd; }

	// --- In  : pWnd - A pointer to CWnd object to be attached
	// --- Out : 
	// --- Returns :
	// --- Effect : Attaches an object
	virtual BOOL AttachWindow(CWnd* pWnd);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXDropDownContainer)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXDropDownContainer() {};

	// Generated message map functions
protected:
	//{{AFX_MSG(COXDropDownContainer)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnFloatStatus(WPARAM wParam, LPARAM);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg BOOL OnItemExpanding(UINT id, NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg BOOL OnItemExpanded(UINT id, NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg BOOL OnSelectionChanging(UINT id, NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg BOOL OnSelectionChanged(UINT id, NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg BOOL OnItemChanging(UINT id, NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()
};


//////////////////////////////////////////////////////////////////


class OX_CLASS_DECL COXComboDropDown
{
public:
	
	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructor
	COXComboDropDown() : m_sizeMaxWidthHeight(-1,-1), m_sizeMinWidthHeight(-1,-1) {};

protected:
	// max width and height of the control (-1 means no limit)
	CSize m_sizeMaxWidthHeight;
	// min width and height of the control (-1 means no limit)
	CSize m_sizeMinWidthHeight;

public:
	// interface
	//

	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	virtual void SetOwnerCombo(COXComboPickerCtrl* pCombo)=0;

	// --- In  :
	// --- Out : 
	// --- Returns : 
	// --- Effect :  
	virtual COXDropDownContainer* GetContainer()=0;

	// --- In  :
	// --- Out : 
	// --- Returns : 
	// --- Effect : 
	virtual CWnd* GetWindow()=0;

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	virtual void ShowDropDown(BOOL bShowIt=TRUE)=0;

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	virtual void GetDroppedControlRect(LPRECT lprect) const=0;

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	virtual BOOL GetDroppedState() const=0;

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	virtual CRect GetBestRect()=0;

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	virtual int GetItemHeight()=0;

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	virtual BOOL IsThereSelectedItem()=0;

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	virtual CImageList* GetSelectedItemImage()=0;

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	virtual CString GetSelectedItemText()=0;

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	virtual CFont* GetSelectedItemFont()=0;

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	virtual COLORREF GetSelectedItemTextColor()=0;

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	virtual void ResetContent()=0;

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	virtual BOOL NeedFocus() const=0;

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	virtual void OnAttachToComboPicker()=0;

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	virtual void OnChangeMinMaxWidthHeight()=0;

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	virtual void OnChangeOwnerComboSize()=0;
	
	//
	/////////////////////////////////

	// helpers
	//

	// --- In  : sizeMinWidthHeight - a new minimum size
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets a new minimum size for the dropdown control
	inline void SetMinWidthHeight(CSize sizeMinWidthHeight)
	{
		m_sizeMinWidthHeight=sizeMinWidthHeight;
		OnChangeMinMaxWidthHeight();
	}

	// --- In  : nMinWidth - minimum width
	//			 nMinHeight- miminum height
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets a new minimum size for the dropdown control
	inline void SetMinWidthHeight(int nMinWidth, int nMinHeight)
	{
		m_sizeMinWidthHeight.cx=nMinWidth;
		m_sizeMinWidthHeight.cy=nMinHeight;
		OnChangeMinMaxWidthHeight();
	}

	// --- In  :
	// --- Out : 
	// --- Returns : minimum size for the dropdown control
	// --- Effect : Returns minimum size for the dropdown control
	inline CSize GetMinWidthHeight() const { return m_sizeMinWidthHeight; }

	// --- In  : sizeMaxWidthHeight - A new max size for the dropdown control
	//								  to be set
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets a new maximum size for the dropdown control
	inline void SetMaxWidthHeight(CSize sizeMaxWidthHeight)
	{
		m_sizeMaxWidthHeight=sizeMaxWidthHeight;
		OnChangeMinMaxWidthHeight();
	}

	// --- In  : nMaxWidth - A new max width for the dropdown control
	//			 nMaxHeight- A new max height for the dropdown control
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets a new maximum size for the dropdown control
	inline void SetMaxWidthHeight(int nMaxWidth, int nMaxHeight)
	{
		m_sizeMaxWidthHeight.cx=nMaxWidth;
		m_sizeMaxWidthHeight.cy=nMaxHeight;
		OnChangeMinMaxWidthHeight();
	}

	// --- In  :
	// --- Out : 
	// --- Returns : A max size of the dropdown control
	// --- Effect : Returns a max size of the dropdown control
	inline CSize GetMaxWidthHeight() const { return m_sizeMaxWidthHeight; }

	//
	////////////////////////////////
};


//////////////////////////////////////////////////////////////////


template<class PARENTTREE>
class COXTreeComboDropDown : public PARENTTREE, public COXComboDropDown
{
public:
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : 
	COXTreeComboDropDown(COXComboPickerCtrl* pCombo=NULL);

protected:
	COXDropDownContainer m_container;
	COXComboPickerCtrl* m_pCombo;

	// helper rectangle for flicker-free control
	CRect m_rectValid;

	// last selected item
	HTREEITEM m_hLastSelectedItem;

	// BOOL flag that specifies that only tree leaves are allowed to be selected
	BOOL m_bSelectOnlyLeaves;

	// BOOL flag that specifies that items will be selected on 
	// mouse move event (hot tracking)
	BOOL m_bSelectOnMouseMove;

public:
	void ForwardMouseMessage(UINT message, UINT nFlags, CPoint point);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	// --- In  : pCombo - A pointer to an owner combo
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets new owner-combo
	virtual void SetOwnerCombo(COXComboPickerCtrl* pCombo) { m_pCombo=pCombo; }

	// --- In  :
	// --- Out : 
	// --- Returns : A pointer to the owner combo
	// --- Effect : Returns a pointer to the owner combo
	inline COXComboPickerCtrl* GetOwnerCombo() const { return m_pCombo; }

	// --- In  : hItem - a handle of the item 
	// --- Out : 
	// --- Returns : TRUE to allow to select item, FALSE otherwise
	// --- Effect : Returns TRUE if is possible to select the item, FALSE otherwise
	virtual BOOL CanSelectItem(HTREEITEM hItem);

	// --- In  :
	// --- Out : 
	// --- Returns : A pointer to the parent window
	// --- Effect : Returns a pointer to the parent window
	virtual COXDropDownContainer* GetContainer() { return &m_container; }
	
	// --- In  :
	// --- Out : 
	// --- Returns : A pointer to the dropdown control
	// --- Effect : Returns a pointer to the dropdown control
	virtual CWnd* GetWindow() { return this; }

	// --- In  : bShowIt - BOOL variable, indicating to show or hide
	//					   dropdown control
	// --- Out : 
	// --- Returns :
	// --- Effect : Shows/hides dropdown control
	virtual void ShowDropDown(BOOL bShowIt=TRUE);

	// --- In  :
	// --- Out : lprect - A rectangle of the dropdown control
	// --- Returns :
	// --- Effect : Retrieves a dropdown control rectanle
	virtual void GetDroppedControlRect(LPRECT lprect) const;
	
	// --- In  :
	// --- Out : 
	// --- Returns : TRUE if the control is dropped, FALSE otherwise
	// --- Effect : Returns dropped state of the dropdown control
	virtual BOOL GetDroppedState() const;
	
	// --- In  :
	// --- Out : 
	// --- Returns : The best rect to fit control in
	// --- Effect : Returns the best rect
	virtual CRect GetBestRect();
	
	// --- In  :
	// --- Out : 
	// --- Returns : An item height
	// --- Effect : Returns an item height
	virtual int GetItemHeight();

	// --- In  :
	// --- Out : 
	// --- Returns : TRUE if some item is selected, FALSE otherwise
	// --- Effect : Determines does some selected item exist
	virtual BOOL IsThereSelectedItem();
	
	// --- In  : 
	// --- Out : 
	// --- Returns : A pointer to CImageList object where image with index 0
	//				 represents selected item image
	// --- Effect : Returns a pointer to selected item image
	virtual CImageList* GetSelectedItemImage();
	
	// --- In  :
	// --- Out : 
	// --- Returns : Selected item text
	// --- Effect : Retrieves selected item text
	virtual CString GetSelectedItemText();
	
	// --- In  :
	// --- Out : 
	// --- Returns : A selected item font
	// --- Effect : Retrieves a selected item font
	virtual CFont* GetSelectedItemFont();
	
	// --- In  :
	// --- Out : 
	// --- Returns : A selected item text color
	// --- Effect : Retrieves an selected item text color
	virtual COLORREF GetSelectedItemTextColor();

	// --- In  : bSelectOnlyLeaves - set it to TRUE to disallow
	//								 to select item that have children,
	//								 otherwise, set it to FALSE
	// --- Out : 
	// --- Returns : 
	// --- Effect : Sets "select only leaves" mode 
	inline void SetSelectOnlyLeaves(BOOL bSelectOnlyLeaves) 
	{ 
		m_bSelectOnlyLeaves=bSelectOnlyLeaves; 
	}
	
	// --- In  :
	// --- Out : 
	// --- Returns : TRUE, if "select only leaves" mode is set, FALSE otherwise
	// --- Effect : Determines if the control in "selectonlyleaves" mode 
	inline BOOL IsSelectingOnlyLeaves() const { return m_bSelectOnlyLeaves; }


	// --- In  :	bSelectOnMouseMove	-	set it to TRUE to allow to select items
	//										on mouse move event
	// --- Out : 
	// --- Returns: 
	// --- Effect : Sets hot tracking mode 
	inline void SetSelectOnMouseMove(BOOL bSelectOnMouseMove) 
	{ 
		m_bSelectOnMouseMove=bSelectOnMouseMove; 
	}
	
	// --- In  :
	// --- Out : 
	// --- Returns : TRUE, if hot tracking mode is set on, FALSE otherwise
	// --- Effect : Determines if the control in hot tracking mode 
	inline BOOL IsSelectingOnMouseMove() const { return m_bSelectOnMouseMove; }


	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Delete contents of the dropdown control
	virtual void ResetContent();

	// --- In  :
	// --- Out : 
	// --- Returns : TRUE, if the dropdown control needs focus, FALSE otherwise
	// --- Effect : Determines if the dropdown control needs focus
	virtual BOOL NeedFocus() const { return TRUE; }

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : This function is called when the control is attached
	//				to the combopicker
	virtual void OnAttachToComboPicker();
	
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : This function called when minmax size
	//				of the control has been changed
	virtual void OnChangeMinMaxWidthHeight();
	
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : This function is called when combo size has been changed
	virtual void OnChangeOwnerComboSize();

protected:
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Recalculate size of the dropdown control
	virtual void AutoResize();

	virtual HTREEITEM GetLastSelectedItem();

public:
	virtual ~COXTreeComboDropDown();

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnKillFocus(CWnd* pNewWnd);
    virtual void OnLButtonDown(UINT nFlags, CPoint point);
    virtual void OnMouseMove(UINT nFlags, CPoint point);
    virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnSelectionChanging(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void NotifySelectionChanged(HTREEITEM hNewItem);
};

//////////////////////////////////////////////////////////////////

template<class PARENTTREE>
COXTreeComboDropDown<PARENTTREE>::
COXTreeComboDropDown(COXComboPickerCtrl* pCombo/*=NULL*/) 
	: m_container(this),
	m_pCombo(pCombo),
	m_rectValid(0,0,0,0),
	m_hLastSelectedItem(NULL),
	m_bSelectOnlyLeaves(FALSE),
	m_bSelectOnMouseMove(FALSE)
{
}


template<class PARENTTREE>
COXTreeComboDropDown<PARENTTREE>::~COXTreeComboDropDown()
{
	if(m_pCombo!=NULL)
	{
		m_pCombo->AttachDropDown(NULL);
	}
}


template<class PARENTTREE>
BOOL COXTreeComboDropDown<PARENTTREE>::PreCreateWindow(CREATESTRUCT& cs)
{
	// create container window
	//

	ASSERT(!::IsWindow(m_container.GetSafeHwnd()));
	if(::IsWindow(m_container.GetSafeHwnd()))
	{
		return FALSE;
	}

    static CString sContainerWndClass=AfxRegisterWndClass(CS_SAVEBITS|CS_DBLCLKS);

	if((cs.dwExStyle & 
		(WS_EX_CLIENTEDGE|WS_EX_STATICEDGE|WS_EX_WINDOWEDGE|WS_EX_DLGMODALFRAME))==0)
	{
		cs.dwExStyle|=WS_EX_DLGMODALFRAME;
	}
    
	if(!m_container.CreateEx(cs.dwExStyle,sContainerWndClass,_T(""),
		(cs.style & 0xffff0000 & ~(WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL))|
		WS_POPUP|MFS_SYNCACTIVE,cs.x,cs.y,cs.cx,cs.cy,cs.hwndParent,NULL))
	{
		TRACE(_T("COXTreeComboDropDown<PARENTTREE>::PreCreateWindow: failed to create the container window"));
		return FALSE;
	}
	//
	///////////////////////////////////////////////

#ifdef _DEBUG
	if((cs.style & TVS_HASBUTTONS)==0 || (cs.style & TVS_LINESATROOT)==0)
	{
		TRACE(_T("COXTreeComboDropDown<PARENTTREE>::PreCreateWindow: tree must have TVS_HASBUTTONS and TVS_LINESATROOT styles in order to expand/collapse items using mouse! Continue execution\n"));
	}
#endif

	// change create data
	cs.style&=~(WS_VISIBLE|WS_POPUP);
	// must have buttons
	cs.style|=WS_CHILD;
	cs.dwExStyle=0;
    cs.hwndParent=m_container.GetSafeHwnd();
    cs.x=cs.y=0;
	CRect rect;
	m_container.GetClientRect(rect);
	cs.cx=rect.Width();
	cs.cy=rect.Height();

	return PARENTTREE::PreCreateWindow(cs);
}


template<class PARENTTREE>
LRESULT COXTreeComboDropDown<PARENTTREE>::WindowProc(UINT message, 
													 WPARAM wParam, 
													 LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	LRESULT lResult=0;

	switch(message)
	{
	case WM_KILLFOCUS: 
		OnKillFocus(CWnd::FromHandle((HWND)wParam));
		break;

	case WM_LBUTTONDOWN: 
		OnLButtonDown((UINT)wParam,CPoint(LOWORD(lParam),HIWORD(lParam)));
		break;

	case WM_MOUSEMOVE: 
		OnMouseMove((UINT)wParam,CPoint(LOWORD(lParam),HIWORD(lParam)));
		break;

	case WM_KEYDOWN: 
		OnKeyDown((UINT)wParam,LOWORD(lParam),HIWORD(lParam));
		break;

	case WM_OXTREEDROPDOWN_ITEM_EXPANDING: 
		lResult=OnItemExpanding((NMHDR*)lParam,(LRESULT*)wParam);
		break;

	case WM_OXTREEDROPDOWN_ITEM_EXPANDED: 
		lResult=OnItemExpanded((NMHDR*)lParam,(LRESULT*)wParam);
		break;

	case WM_OXTREEDROPDOWN_SELECTION_CHANGING: 
		lResult=OnSelectionChanging((NMHDR*)lParam,(LRESULT*)wParam);
		break;

	case WM_OXTREEDROPDOWN_SELECTION_CHANGED: 
		lResult=OnSelectionChanged((NMHDR*)lParam,(LRESULT*)wParam);
		break;

	default:
		lResult=PARENTTREE::WindowProc(message,wParam,lParam);
		break;
	}

	return lResult;
}


template<class PARENTTREE>
void COXTreeComboDropDown<PARENTTREE>::OnKillFocus(CWnd* pNewWnd)
{
	PARENTTREE::OnKillFocus(pNewWnd);

	if(pNewWnd!=this && m_pCombo!=NULL)
	{
		m_pCombo->PostMessage(CB_SHOWDROPDOWN,FALSE);
	}
}


template<class PARENTTREE>
void COXTreeComboDropDown<PARENTTREE>::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bHide=FALSE;
	TVHITTESTINFO hitTest;
	hitTest.pt=point;
	HTREEITEM hItem=HitTest(&hitTest);
	if(hItem!=NULL && (hitTest.flags & TVHT_ONITEMBUTTON)==0)	
	{
		// selection might be disallowed
		if(CanSelectItem(hItem))
		{
			SelectItem(hItem);
			if(m_hLastSelectedItem!=hItem)
			{
				m_hLastSelectedItem=hItem;
				NotifySelectionChanged(hItem);
			}
		}
		bHide=TRUE;
	}

	if(bHide && m_pCombo!=NULL)
	{
		m_pCombo->ShowDropDown(FALSE);
		m_pCombo->SetFocus();
	}
	else
	{
		PARENTTREE::OnLButtonDown(nFlags,point);
	}
}


template<class PARENTTREE>
void COXTreeComboDropDown<PARENTTREE>::OnMouseMove(UINT nFlags, CPoint point)
{
	if(IsSelectingOnMouseMove())
	{
		CRect rect;
		m_container.GetClientRect(rect);
		if(rect.PtInRect(point))
		{
			TVHITTESTINFO hitTest;
			hitTest.pt=point;
			HTREEITEM hItem=HitTest(&hitTest);
			if(hItem!=NULL)
			{
				SelectItem(hItem);
			}
		}
	}

	PARENTTREE::OnMouseMove(nFlags,point);
}


template<class PARENTTREE>
void COXTreeComboDropDown<PARENTTREE>::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(m_pCombo!=NULL)
	{
		if(nChar==VK_ESCAPE || nChar==VK_RETURN)
		{
			if(nChar==VK_RETURN)
			{
				HTREEITEM hItem=GetSelectedItem();
				if(hItem!=NULL && CanSelectItem(hItem))
				{
					if(m_hLastSelectedItem!=hItem)
					{
						m_hLastSelectedItem=hItem;
						NotifySelectionChanged(hItem);
					}
				}
			}
			m_pCombo->ShowDropDown(FALSE);
			m_pCombo->SetFocus();
			return;
		}
	}

	PARENTTREE::OnKeyDown(nChar,nRepCnt,nFlags);
}


template<class PARENTTREE>
BOOL COXTreeComboDropDown<PARENTTREE>::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView=(NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult=0;

	if(m_sizeMaxWidthHeight.cy==-1)
	{
		if(!GetUpdateRect(NULL))
		{
			GetClientRect(m_rectValid);
			CRect rect;
			GetItemRect(pNMTreeView->itemNew.hItem,rect,FALSE);
			m_rectValid.bottom=rect.top-1;
		}
		else
		{
			m_rectValid.SetRectEmpty();
		}

		// avoid flicker, undone on expanded message since we may resize
		GetDesktopWindow()->LockWindowUpdate();    
	}

    return FALSE;   // give to parent
}


template<class PARENTTREE>
BOOL COXTreeComboDropDown<PARENTTREE>::OnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);

	*pResult=0;

	// resize if necessary
    AutoResize();  

	if(m_sizeMaxWidthHeight.cy==-1)
	{
		// reset update lock
		::LockWindowUpdate(NULL);
		ValidateRect(m_rectValid);
	}
    
    return FALSE;   // allow parent to have a look
}


template<class PARENTTREE>
BOOL COXTreeComboDropDown<PARENTTREE>::OnSelectionChanging(NMHDR* pNMHDR, 
														   LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);

	*pResult=0;

    return FALSE;   // give to parent
}


template<class PARENTTREE>
BOOL COXTreeComboDropDown<PARENTTREE>::OnSelectionChanged(NMHDR* pNMHDR, 
														  LRESULT* pResult) 
{
	NMTREEVIEW* pNMTV=(NMTREEVIEW*)pNMHDR;

	if(pNMTV->itemNew.hItem!=NULL && !CanSelectItem(pNMTV->itemNew.hItem))
	{
		SetItemState(pNMTV->itemNew.hItem,0,TVIS_SELECTED);
	}

	*pResult=0;

    return FALSE;   // give to parent
}


template<class PARENTTREE>
void COXTreeComboDropDown<PARENTTREE>::NotifySelectionChanged(HTREEITEM hNewItem)
{
	UNREFERENCED_PARAMETER(hNewItem);
}


template<class PARENTTREE>
void COXTreeComboDropDown<PARENTTREE>::AutoResize()
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	if(!::IsWindow(GetSafeHwnd()))
	{
		return;
	}

	CRect rectCurrent;
	GetWindowRect(rectCurrent);
	CRect rectBest=GetBestRect();
	CRect rect=rectBest;
	ASSERT(m_sizeMinWidthHeight.cx<=m_sizeMaxWidthHeight.cx || 
		m_sizeMinWidthHeight.cx==-1 || m_sizeMaxWidthHeight.cx==-1);
	ASSERT(m_sizeMinWidthHeight.cy<=m_sizeMaxWidthHeight.cy || 
		m_sizeMinWidthHeight.cy==-1 || m_sizeMaxWidthHeight.cy==-1);
	if(m_sizeMaxWidthHeight.cx>=0 && rect.Width()>m_sizeMaxWidthHeight.cx)
	{
		rect.right=rect.left+m_sizeMaxWidthHeight.cx;
	}
	if(m_sizeMaxWidthHeight.cy>=0 && rect.Height()>m_sizeMaxWidthHeight.cy)
	{
		rect.bottom=rect.top+m_sizeMaxWidthHeight.cy;
	}
	if(m_sizeMinWidthHeight.cx>=0 && rect.Width()<m_sizeMinWidthHeight.cx)
	{
		rect.right=rect.left+m_sizeMinWidthHeight.cx;
	}
	if(m_sizeMinWidthHeight.cy>=0 && rect.Height()<m_sizeMinWidthHeight.cy)
	{
		rect.bottom=rect.top+m_sizeMinWidthHeight.cy;
	}

	if(rectBest.Height()==rect.Height())
	{
		// we need to trick the tree control in order to make sure 
		// that scroll bars will not be displayed. This will not cause any
		// fickering because the window is not visible at the momemnt or 
		// all updates are locked
		SetWindowPos(NULL,0,0,rect.Width()+::GetSystemMetrics(SM_CXVSCROLL),
			rect.Height(),SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
	}

	if(rectBest.Height()>rect.Height())
	{
		// control will have vertical scroll bar
		// we might try to adjust control's width 
		// to take into account the area that will 
		// be taken by the scroll bar
		if(rectBest.Width()<=rect.Width() && 
			rect.Width()-rectBest.Width()<::GetSystemMetrics(SM_CXVSCROLL) &&
			(m_sizeMaxWidthHeight.cx==-1 || rectBest.Width()+
			::GetSystemMetrics(SM_CXVSCROLL)<=m_sizeMaxWidthHeight.cx))
		{
			rect.right+=rectBest.Width()-rect.Width()+
				::GetSystemMetrics(SM_CXVSCROLL);
		}
	}
	if(rectBest.Width()>rect.Width())
	{
		// control will have horizontal scroll bar
		// we might try to adjust control's height 
		// to take into account the area that will 
		// be taken by the scroll bar
		if(rectBest.Height()<=rect.Height() && 
			rect.Height()-rectBest.Height()<::GetSystemMetrics(SM_CYHSCROLL) &&
			(m_sizeMaxWidthHeight.cy==-1 || rectBest.Height()+
			::GetSystemMetrics(SM_CYHSCROLL)<=m_sizeMaxWidthHeight.cy))
		{
			rect.bottom+=rectBest.Height()-rect.Height()+
				::GetSystemMetrics(SM_CYHSCROLL);
		}
	}

	if(::IsWindow(m_container.GetSafeHwnd()))
	{
		m_container.CalcWindowRect(rect);
		m_container.SetWindowPos(NULL,0,0,rect.Width(),rect.Height(),
			SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
	}
	else
	{
		SetWindowPos(NULL,0,0,rect.Width(),rect.Height(),
			SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);
	}
}


template<class PARENTTREE>
void COXTreeComboDropDown<PARENTTREE>::GetDroppedControlRect(LPRECT lprect) const
{
	if(::IsWindow(GetSafeHwnd()))
	{
		ASSERT(::IsWindow(m_container.GetSafeHwnd()));
		if(::IsWindow(m_container.GetSafeHwnd()))
		{
			m_container.GetWindowRect(lprect);
		}
		else
		{
			GetWindowRect(lprect);
		}
	}
}


template<class PARENTTREE>
BOOL COXTreeComboDropDown<PARENTTREE>::GetDroppedState() const
{
	if(::IsWindow(GetSafeHwnd()))
	{
		return IsWindowVisible();
	}

	return FALSE;
}


template<class PARENTTREE>
CRect COXTreeComboDropDown<PARENTTREE>::GetBestRect()
{
	CRect rect(0,0,0,0);
	// loop through all visible items and calculate their height and width
	//
	HTREEITEM hItem=GetRootItem();
	CRect rectItem;
	while(hItem!=NULL)
	{
		if(GetItemRect(hItem,rectItem,FALSE))
		{
			rect.bottom+=rectItem.Height();
		}

		if(GetItemRect(hItem,rectItem,TRUE))
		{
			if(rect.right<rectItem.right)
			{
				rect.right=rectItem.right;
			}
		}

		hItem=GetNextVisibleItem(hItem);
	}
	//
	////////////////////////////////////////

	// take into account window borders if any exist
	CalcWindowRect(rect);

	return rect;
}


template<class PARENTTREE>
int COXTreeComboDropDown<PARENTTREE>::GetItemHeight()
{
	if(::IsWindow(GetSafeHwnd()) && GetCount()>0)
	{
		CRect rect;
		VERIFY(GetItemRect(0,rect,FALSE));
		return rect.Height();
	}
	else
	{
		return OXCOMBOPICKER_DEFAULTITEMHEIGHT;
	}
}


template<class PARENTTREE>
BOOL COXTreeComboDropDown<PARENTTREE>::IsThereSelectedItem()
{
	if(!::IsWindow(GetSafeHwnd()))
	{
		return FALSE;
	}

	return (GetLastSelectedItem()!=NULL);
}


template<class PARENTTREE>
CImageList* COXTreeComboDropDown<PARENTTREE>::GetSelectedItemImage()
{
	if(!IsThereSelectedItem())
	{
		return NULL;
	}

	int nItemImage=-1;
	int nItemSelectedImage=-1;
	GetItemImage(GetLastSelectedItem(),nItemImage,nItemSelectedImage);
	if(nItemImage==-1)
	{
		return NULL;
	}

	CImageList* pImageList=GetImageList(TVSIL_NORMAL);
	if(pImageList==NULL)
	{
		return NULL;
	}
	ASSERT(nItemImage<pImageList->GetImageCount());

	CImageList* pSelectedItemImageList=new CImageList();
	if(!pSelectedItemImageList->Create(pImageList))
	{
		delete pSelectedItemImageList;
		return NULL;
	}

	for(int nIndex=pSelectedItemImageList->GetImageCount()-1; nIndex>=0; nIndex--)
	{
		if(nIndex!=nItemImage)
		{
			VERIFY(pSelectedItemImageList->Remove(nIndex));
		}
	}

	return pSelectedItemImageList;
}


template<class PARENTTREE>
CString COXTreeComboDropDown<PARENTTREE>::GetSelectedItemText()
{
	if(!IsThereSelectedItem())
	{
		return _T("");
	}

	return GetItemText(GetLastSelectedItem());
}


template<class PARENTTREE>
CFont* COXTreeComboDropDown<PARENTTREE>::GetSelectedItemFont()
{
	if(!IsThereSelectedItem())
	{
		return NULL;
	}

	return GetFont();
}


template<class PARENTTREE>
COLORREF COXTreeComboDropDown<PARENTTREE>::GetSelectedItemTextColor()
{
	return ::GetSysColor(COLOR_WINDOWTEXT);
}


template<class PARENTTREE>
void COXTreeComboDropDown<PARENTTREE>::ResetContent()
{
	if(::IsWindow(GetSafeHwnd()))
	{
		DeleteAllItems();
	}
}


template<class PARENTTREE>
void COXTreeComboDropDown<PARENTTREE>::OnAttachToComboPicker()
{
	ASSERT(m_pCombo!=NULL);
	if(m_pCombo!=NULL && ::IsWindow(m_pCombo->GetSafeHwnd()) && 
		::IsWindow(GetSafeHwnd()))
	{
		ASSERT(::IsWindow(m_container.GetSafeHwnd()));
		CRect rect;
		m_pCombo->GetWindowRect(rect);
		CRect rectCalc=rect;
		m_container.CalcWindowRect(rectCalc);
		// set the minimum width for the control
		CSize sizeMinWidthHeight=GetMinWidthHeight();
		sizeMinWidthHeight.cx=2*rect.Width()-rectCalc.Width();
		SetMinWidthHeight(sizeMinWidthHeight);
	}
}


template<class PARENTTREE>
void COXTreeComboDropDown<PARENTTREE>::OnChangeMinMaxWidthHeight()
{
	if(::IsWindow(GetSafeHwnd()))
	{
		AutoResize();
	}
}


template<class PARENTTREE>
void COXTreeComboDropDown<PARENTTREE>::OnChangeOwnerComboSize()
{
	ASSERT(m_pCombo!=NULL);
	if(m_pCombo!=NULL && ::IsWindow(m_pCombo->GetSafeHwnd()) && 
		::IsWindow(GetSafeHwnd()))
	{
		// set the minimum width for the control
		ASSERT(::IsWindow(m_container.GetSafeHwnd()));
		CRect rect;
		m_pCombo->GetWindowRect(rect);
		CRect rectCalc=rect;
		m_container.CalcWindowRect(rectCalc);
		SetMinWidthHeight(2*rect.Width()-rectCalc.Width(),-1);
	}
}


template<class PARENTTREE>
void COXTreeComboDropDown<PARENTTREE>::ShowDropDown(BOOL bShowIt/*=TRUE*/)
{
	if(bShowIt)
	{
		m_hLastSelectedItem=GetSelectedItem();
		if(m_hLastSelectedItem!=NULL && !CanSelectItem(m_hLastSelectedItem))
		{
			m_hLastSelectedItem=NULL;
		}

		// adjust size of window
		AutoResize();
		
		// make sure the selected item is visible
		if(m_hLastSelectedItem!=NULL)
		{
			EnsureVisible(m_hLastSelectedItem);
		}
	}
	else
	{
		if(m_hLastSelectedItem!=NULL && !CanSelectItem(m_hLastSelectedItem))
		{
			m_hLastSelectedItem=NULL;
		}
		SelectItem(m_hLastSelectedItem);
	}
}


template<class PARENTTREE>
BOOL COXTreeComboDropDown<PARENTTREE>::CanSelectItem(HTREEITEM hItem)
{
	if(m_bSelectOnlyLeaves && ItemHasChildren(hItem))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


template<class PARENTTREE>
HTREEITEM COXTreeComboDropDown<PARENTTREE>::GetLastSelectedItem()
{
	if(GetDroppedState())
	{
		return m_hLastSelectedItem;
	}
	else
	{
		if(::IsWindow(GetSafeHwnd()))
		{
			HTREEITEM hItem=GetSelectedItem();
			if(hItem!=NULL && CanSelectItem(hItem))
			{
				return hItem;
			}
		}

		return NULL;
	}
}


template<class PARENTTREE>
void COXTreeComboDropDown<PARENTTREE>::
ForwardMouseMessage(UINT message, UINT nFlags, CPoint point)
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	if(!::IsWindow(GetSafeHwnd()))
	{
		return;
	}

	CPoint ptScreen=point;
	ClientToScreen(&ptScreen);
	CWnd* pTargetWnd=CWnd::WindowFromPoint(ptScreen);
	if(pTargetWnd==NULL)
	{
		return;
	}

	ASSERT(pTargetWnd!=this);
	if(pTargetWnd==this)
	{
		return;
	}

	WPARAM wParam=nFlags;

	int nHitTest=
		(int)pTargetWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(ptScreen.x,ptScreen.y));	
	if(nHitTest==HTCLIENT) 
	{
		pTargetWnd->ScreenToClient(&ptScreen);	
	} 
	else 
	{	
		switch(message) 
		{
		case WM_LBUTTONDOWN: 
			{
				message=WM_NCLBUTTONDOWN;		
				break;
			}
		case WM_RBUTTONDOWN: 
			{
				message=WM_NCRBUTTONDOWN;		
				break;
			}
		case WM_MBUTTONDOWN: 
			{
				message=WM_NCMBUTTONDOWN;		
				break;
			}
		case WM_LBUTTONUP: 
			{
				message=WM_NCLBUTTONUP;		
				break;
			}
		case WM_RBUTTONUP: 
			{
				message=WM_NCRBUTTONUP;		
				break;
			}
		case WM_MBUTTONUP: 
			{
				message=WM_NCMBUTTONUP;		
				break;
			}
		case WM_LBUTTONDBLCLK: 
			{
				message=WM_NCLBUTTONDBLCLK;		
				break;
			}
		case WM_RBUTTONDBLCLK: 
			{
				message=WM_NCRBUTTONDBLCLK;		
				break;
			}
		case WM_MBUTTONDBLCLK: 
			{
				message=WM_NCMBUTTONDBLCLK;		
				break;
			}
		case WM_MOUSEMOVE: 
			{
				message=WM_NCMOUSEMOVE;		
				break;
			}
		}
		wParam=nHitTest;
	}

	LPARAM lParam=MAKELONG(ptScreen.x,ptScreen.y);

	pTargetWnd->SendMessage(message,wParam,lParam);
}


///////////////////////////////////////////////////////////////////////


class OX_CLASS_DECL COXComboPickerCtrl : public CComboBox
{
	DECLARE_DYNAMIC(COXComboPickerCtrl);
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect:	Constructs the object
	COXComboPickerCtrl(COXComboDropDown* pDropDown=NULL);

// Attributes
public:

protected:
	// dropdown interface
	COXComboDropDown* m_pDropDown;

// Operations
public:
	// --- In  :
	// --- Out : 
	// --- Returns: Pointer to the internal picker control
	// --- Effect:	Retrieves a pointer to the internal picker control
	virtual COXComboDropDown* GetDropDown() const { return m_pDropDown; }

	// --- In  : pDropDown - a pointer to COXComboDropDown 
	//						 object to be attached
	// --- Out : 
	// --- Returns : TRUE on success, FALSE otherwise
	// --- Effect : Attaches COXComboDropDown object
	virtual BOOL AttachDropDown(COXComboDropDown* pDropDown);

	// --- In  : bShowIt - TRUE to show dropdown control, FALSE to hide
	// --- Out : 
	// --- Returns :
	// --- Effect : Shows/hides dropdown control
	virtual void ShowDropDown(BOOL bShowIt=TRUE);

	// --- In  :
	// --- Out : 
	// --- Returns : TRUE if the dropdown control is dropped, FALSE otherwise
	// --- Effect : Determines dropped state of the dropdown control
	virtual BOOL GetDroppedState() const;

	// --- In  :
	// --- Out : 
	// --- Returns : A rect of the dropdown button in the combo
	// --- Effect : Retrieves rect of the dropdown button
	virtual CRect GetDropdownButtonRect() const;

	// --- In  : nHeight - a new max height for the dropdown control
	// --- Out : 
	// --- Returns : TRUE on success, FALSE otherwise
	// --- Effect : Sets a new max height for the dropdown control 
	BOOL SetMaxDropdownHeight(int nHeight);

	// --- In  : nHeight - a new min height for the dropdown control
	// --- Out : 
	// --- Returns : TRUE on success, FALSE otherwise
	// --- Effect : Sets a new min height for the dropdown control 
	BOOL SetMinDropdownHeight(int nHeight);

// Implementation
public:

	virtual ~COXComboPickerCtrl();

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);


	// Generated message map functions
protected:
	//{{AFX_MSG(COXComboPickerCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////

#endif	//	#ifndef _OX_COMBOPICKERCTRL_H__