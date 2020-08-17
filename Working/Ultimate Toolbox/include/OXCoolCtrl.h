// ==========================================================================
//						   Class Specification : 
//								COXCoolCtrl
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

	DESCRIPTION


Almost every application uses some of common controls in its implementation. 
Users used to see these common controls and khow how to use them. 

This the primary reason why you usually don't want to change the look 
and functionality of existing controls too much. But sometimes even by slightly 
changing the appearance of a control you can achive good results in terms
of enhancing the feel and look of your application's user interface. 

We suggest the following new feature for common controls: every control
(generally speaking, any window) can be in normal or hot state. By definition,
window is in hot state when mouse cursor is over it or it has focus. Otherwise 
window is in normal state. Naturally, in order to distinguish window in 
different states some graphical effects should be applied. 
E.g. the drawing of window borders in different states might be different.

We call controls that support normal and hot state - cool controls.

We designed template class COXCoolCtrl that simplifies process of 
converting of common controls to cool ones.

COXCoolCtrl template has one argument that specifies the parent class 
of the new cool control. E.g. if you would like to design your own cool
listbox class you can declare it as following:

	class CCoolListBox : public COXCoolCtrl<CListBox>


While implementing cool control using COXCoolCtrl derivation you might would 
like to override the following protected virtual functions:


	virtual void OnChangeHotState(BOOL bInHotState);
	virtual void OnFocusChange(CWnd* pWndGainFocus, CWnd* pWndLooseFocus);
	virtual void OnMouseEnter(const CPoint& ptCursor);
	virtual void OnMouseLeave(const CPoint& ptCursor);
	virtual BOOL HandleKey(int nVirtKey);
	virtual BOOL OnChangeTooltipText(const CString& sTooltipText);


Refer to the class reference for detailed function explanations. 

Almost always you would like to override OnChangeHotState() function which is 
called when window is about to change its state from hot to normal and vise versa. 
n your implementation of this function you might redraw the window in order 
to show that window has changed its state.

OnFocusChange() is called every time window looses or gains focus and 
OnMouseEnter() and OnMouseLeave() functions are called every time mouse enters
or leaves the screen space taken by the window correspondingly.

HandleKey() function will be called every time when user presses any key down. 
Overriding this function you can handle pressed key before control has a chance 
to do that. By returning TRUE you will notify control that you handled the event
yourself and don't want to run default handler implementation.


One of the additional useful feature that COXCoolCtrl supports is tooltip 
for the control. You can set/retrieve tooltip text using the following functions:

	BOOL SetTooltipText(const CString& sTooltipText);
	BOOL GetTooltipText(CString& sTooltipText) const;


Every time you change the tooltip text OnChangeTooltipText() function will be called.
By overriding this function you can control the process of setting of tooltip text.
By returning FALSE you specify that you don't allow to change the tooltip text.


There are a few additional public functions that you might find useful:

	BOOL IsInHotState() const;
	BOOL IsMouseOver() const;

IsInHotState() function retrieves the flag that specifies whether the window is 
in hot or normal state. IsMouseOver() function retrieves the flag that specifies 
whether the current mouse position is over the window or not.

Although it's not a must requirement but it would be nice if control was able
to support standard and new cool state. By default when control is created of 
subclassed the existing one it is set to cool state. But calling the following 
function you can set it back to statndard state:

	virtual void SetCoolState(BOOL bCoolState);

You can retrieve the flag that specifies whether the control is in cool or 
normal state

	virtual BOOL IsInCoolState() const;

As you can see both of these functions are declared as virtual so if you don't want 
to support standard appearance of the control you might override this function 
(SetCoolState() would do nothing and IsInCoolState() would always return TRUE).



Again we would like to emphasize that COXCoolCtrl class is designed specifically 
in order to help you to create cool common controls. We are going to come up
with classes that implements some common controls as cool ones (take look at the 
COXCoolComboBox class which implements cool combobox).



================================================================================


								Cool Controls


Just as a reminder we assume that a common control can be called the "cool" one
if it can take normal or hot state. By definition, window is in hot state when 
mouse cursor is over it or it has focus. Otherwise window is in normal state. 
Naturally, in order to distinguish window in different states some graphical effects 
should be applied. E.g. the drawing of window borders in different states might be 
different.

COXCoolCtrl class is the basic class that is used to create cool controls. 
This is a template class and it can be easily used in order to create "cool" versions
of common controls. Actually our implementation of classes for cool common controls 
doesn't have any public functions, so it's strongly recommended to refer to 
COXCoolCtrl reference for the functionality available for all cool controls (e.g. 
all cool controls support tooltips).

When we were talking about implementation of "cool" versions of cool controls we 
mentioned that it didn't require too much job to be done because almost everything
is covered by COXCoolCtrl. Actually for the following common controls "cool" state
will be supported automatically if you use corresponding MFC class as 
template argument.

  Common Control		MFC	class			UTB class

  Edit control			CEdit				COXCoolCtrl<CEdit>
  Rich edit control		CRichEditCtrl		COXCoolCtrl<CRichEditCtrl>
  Tree view				CTreeCtrl			COXCoolCtrl<CTreeCtrl>
  List view				CListCtrl			COXCoolCtrl<CListCtrl>
  List box				CListBox			COXCoolCtrl<CListBox>
  Progress bar			CProgressCtrl		COXCoolCtrl<CProgressCtrl>
  Hot key control		CHotKeyCtrl			COXCoolCtrl<CHotKeyCtrl>
  Date-time picker		CDateTimeCtrl		COXCoolCtrl<CDateTimeCtrl>
  Month calendar		CMonthCalCtrl		COXCoolCtrl<CMonthCalCtrl>
  IP Address control	CIPAddressCtrl		COXCoolCtrl<CIPAddressCtrl>


For the other common controls we had to come up with new classes as long
as additional drawing routines have been required. Below you will find the list
of all such controls and the corresponding Ultimate Toolbox classes. 

  Common Control		MFC	class			UTB class

  Push button
  Check button			CButton				COXCoolButton
  Radio button controls
  Spin button			CSpinButtonCtrl		COXCoolSpinButtonCtrl
  Combo box				CComboBox			COXCoolComboBox
  Slider				CSliderCtrl			COXCoolSliderCtrl
  Scroll bar			CScrollBar			COXCoolScrollBar



We would like to emphasize again that these classes doesn't have any public functions
of their own. Refer to the COXCoolCtrl function reference for details on the 
functionality available for all cool controls.


All above mentioned classes can be used without any restrictions instead the
standard ones. Take look at the CoolControls sample that demonstrates the use
of all cool controls classes. This sample can be found in .\Samples\gui\CoolControls



================================================================================

Dependencies:

	#include "OXCoolCtrl.h"


Source code files:

	"OXCoolCtrl.h"


*/



#if !defined(_OXCOOLCTRL_H__)
#define _OXCOOLCTRL_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#define __AFXTEMPL_H__
#endif

#include "UTB64Bit.h"


#define IDT_COOLCTRL_CHECKMOUSETIMER		210
// default delay
#ifndef ID_COOLCTRL_DFLT_CHECKMOUSEDELAY
#define ID_COOLCTRL_DFLT_CHECKMOUSEDELAY	150
#endif	// DFLT_CHECKMOUSEDELAY


#define OXCOOLCTRL_TOOLTIP_ID				1000

/////////////////////////////////////////////////////////////////////////////
// COXCoolCtrl window

template<class PARENTWND>
class COXCoolCtrl : public PARENTWND
{
// Construction
public:
	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXCoolCtrl();

	// helper for detecting whether child descendent of parent
	// (works with owned popups as well)
	static BOOL IsDescendant(CWnd* pWndParent, CWnd* pWndChild);

	static BOOL IsEditControl(HWND hWnd);
	static BOOL IsComboBox(HWND hWnd);
	static BOOL IsListBox(HWND hWnd);
	static BOOL IsSpinButtonControl(HWND hWnd);
	static HWND FindAttachedSpinButton(HWND hWnd);

// Attributes
public:

protected:
	// TRUE if window in cool state and FALSE if in normal state
	BOOL m_bCoolState;

	// timer id for checking mouse position
	UINT_PTR m_nCheckMouseTimerID;
	// flag that specifies if mouse is over the window
	BOOL m_bMouseIsOver;
	// flag that specifies if window in a hot state
	BOOL m_bInHotState;

	// tooltip control
	CToolTipCtrl m_toolTip;

	// internal flag that specifies whether the window is needed to be
	// properly initialized
	BOOL m_bInitializedCoolCtrl;

// Operations
public:

	// --- In  :	
	// --- Out : 
	// --- Returns: TRUE if window is in cool state, or FALSE if it is 
	//				in normal state
	// --- Effect : Retrieves the flag that specifies whether the object is 
	//				in cool state or not
	virtual BOOL IsInCoolState() const;

	// --- In  :	bCoolState	-	TRUE if window will be set to cool state, 
	//								or FALSE if it will be set to normal state
	// --- Out : 
	// --- Returns: 
	// --- Effect : Sets the object in cool state, or restore the normal 
	//				state of the object
	virtual void SetCoolState(BOOL bCoolState);


	// --- In  :	
	// --- Out : 
	// --- Returns: TRUE if window is in hot state, or FALSE otherwise
	// --- Effect : Retrieves the flag that specifies whether the object is 
	//				in hot state or not
	BOOL IsInHotState() const;

	// --- In  :	
	// --- Out : 
	// --- Returns: TRUE if mouse cursor is currently over the window, 
	//				or FALSE otherwise
	// --- Effect : Retrieves the flag that specifies whether the mouse 
	//				cursor is currently over the window or not 
	BOOL IsMouseOver() const;


	// --- In  :	sTooltipText	-	tooltip text
	// --- Out : 
	// --- Returns: TRUE if new tooltip text was successfully set
	// --- Effect : Sets new tooltip text
	BOOL SetTooltipText(const CString& sTooltipText);

	// --- In  :	
	// --- Out :	sTooltipText	-	reference to string variable that will be
	//									populated with tooltip text
	// --- Returns: TRUE if tooltip text was successfully retrieved
	// --- Effect : Retrieves tooltip text
	BOOL GetTooltipText(CString& sTooltipText) const;


// Implementation
public:
	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXCoolCtrl();

	// pretranslates messages in internal purposes
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	// check if mouse is over the window or not
	virtual void CheckMousePos(BOOL bChildren=TRUE);

	// initialize mouse timer and tooltip control
	virtual BOOL InitializeCoolCtrl();

	// virtual functions 
	//
	// The following virtual functions are called every time corresponding 
	// event happens. In derived classes you can override them in order to 
	// handle these events in specific way
	//
	//

	// --- In  :	bInHotState	-	TRUE if window's state is about to be changed 
	//								from normal to hot, FALSE if window's state is 
	//								about to be changed from hot to normal 
	// --- Out : 
	// --- Returns:
	// --- Effect : Called when object is about to change its state from 
	//				hot to normal or from normal to hot
	virtual void OnChangeHotState(BOOL bInHotState);

	// --- In  :	pWndGainFocus	-	pointer to the window that gains focus
	//				pWndLooseFocus	-	pointer to the window that looses focus
	// --- Out : 
	// --- Returns:
	// --- Effect : Called when window is loosing or gaining focus
	virtual void OnFocusChange(CWnd* pWndGainFocus, CWnd* pWndLooseFocus);

	// --- In  :	nVirtKey	-	virtual-key code of the pressed key
	// --- Out : 
	// --- Returns:	TRUE if you don't want to call default handler for pressed key,
	//				or FALSE otherwise
	// --- Effect : Called when any key is pressed
	virtual BOOL HandleKey(int nVirtKey);

	// --- In  :	ptCursor	-	mouse cursor coordinates when it entered 
	//								the screen space taken by the object
	// --- Out : 
	// --- Returns:
	// --- Effect : Called when mouse enteres the screen space taken by the object 
	virtual void OnMouseEnter(const CPoint& ptCursor);

	// --- In  :	ptCursor	-	mouse cursor coordinates when it left 
	//								the screen space taken by the object
	// --- Out : 
	// --- Returns:
	// --- Effect : Called when mouse leaves the screen space taken by the object 
	virtual void OnMouseLeave(const CPoint& ptCursor);

	// --- In  :	sTooltipText	-	new tooltip text
	// --- Out : 
	// --- Returns:	FALSE if you don't want to set the specified text as new tooltip,
	//				or TRUE otherwise
	// --- Effect : Called when new tooltip text is about to be set
	virtual BOOL OnChangeTooltipText(const CString& sTooltipText);

	//
	/////////////////////////////////

	void DrawNcBorders();
	void DrawScrollBar(CDC* pDC,CRect rectBar,BOOL bHorz);


	// is handled in order to properly initialize the subclassed window
	virtual void PreSubclassWindow();
	// handles some messages that are important for internal implementation
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};



/////////////////////////////////////////////////////////////////////////////
// COXCoolCtrl implementation

template<class PARENTWND>
COXCoolCtrl<PARENTWND>::COXCoolCtrl() 
{
	m_bCoolState=TRUE;

	m_nCheckMouseTimerID=0;
	m_bMouseIsOver=FALSE;
	m_bInHotState=FALSE;

	m_bInitializedCoolCtrl=FALSE;
}


template<class PARENTWND>
COXCoolCtrl<PARENTWND>::~COXCoolCtrl() 
{
}


template<class PARENTWND>
BOOL COXCoolCtrl<PARENTWND>::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	// ... Pass the message on to the tool tip
	if(::IsWindow(m_toolTip.GetSafeHwnd()))
	{
		m_toolTip.Activate(TRUE);
		m_toolTip.RelayEvent(pMsg);
	}

	if(pMsg->message==WM_KEYDOWN)
	{
		if(HandleKey(PtrToInt(pMsg->wParam)))
			return TRUE;
    }

	return PARENTWND::PreTranslateMessage(pMsg);
}

template<class PARENTWND>
LRESULT COXCoolCtrl<PARENTWND>::WindowProc(UINT message, WPARAM wParam, 
										   LPARAM lParam) 
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	// TODO: Add your specialized code here and/or call the base class
	ASSERT_VALID(this);

	if(!m_bInitializedCoolCtrl && message==WM_CREATE)
	{
		m_bInitializedCoolCtrl=TRUE;
		InitializeCoolCtrl();
	}
	
	switch(message) 
	{
	case WM_DESTROY:
		{
			if(m_nCheckMouseTimerID!=0)
			{
				ASSERT(::IsWindow(GetSafeHwnd()));
				VERIFY(KillTimer(m_nCheckMouseTimerID));
				m_nCheckMouseTimerID=0;
			}

			if(::IsWindow(m_toolTip.GetSafeHwnd()))
				m_toolTip.DestroyWindow();

			break;
		}

	case WM_NCDESTROY:
		{
			LRESULT lResult=PARENTWND::WindowProc(message,wParam,lParam);

			m_nCheckMouseTimerID=0;
			m_bMouseIsOver=FALSE;
			m_bInitializedCoolCtrl=FALSE;

			return lResult;
		}

	case WM_NCPAINT:
		{
			// draw the non-client area using default implementation
			LRESULT lResult=PARENTWND::WindowProc(message,wParam,lParam);
			DrawNcBorders();			
			return lResult;
		}

	case WM_TIMER:
		{
			if(IDT_COOLCTRL_CHECKMOUSETIMER==(UINT)wParam)
			{
				if(IsInCoolState())
				{
					CheckMousePos();
				}
				return 0;
			}
			break;
		}

	case WM_SETFOCUS:
		{
			if(IsInCoolState())
			{
				OnFocusChange(this,CWnd::FromHandle((HWND)wParam));
			}
			break;
		}

	case WM_KILLFOCUS:
		{
			if(IsInCoolState())
			{
				OnFocusChange(CWnd::FromHandle((HWND)wParam),this);
			}
			break;
		}

	case WM_SIZE:
		{
			LRESULT lResult=PARENTWND::WindowProc(message,wParam,lParam);

			CString sTooltipText;
			if(GetTooltipText(sTooltipText))
			{
				m_toolTip.DelTool(this,OXCOOLCTRL_TOOLTIP_ID);
				CRect rect;
				GetClientRect(rect);
				VERIFY(m_toolTip.AddTool(
					this,sTooltipText,rect,OXCOOLCTRL_TOOLTIP_ID));
			}

			return lResult;
		}

	case WM_ENABLE:
		{
			if(IsInCoolState())
			{
				LRESULT lResult=PARENTWND::WindowProc(message,wParam,lParam);
				RedrawWindow(NULL,NULL,
					RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_FRAME);
				return lResult;
			}
			break;
		}

	default:
		{
			if(!IsMouseOver() && IsInCoolState() &&
				((message>=WM_MOUSEFIRST && message<=WM_MOUSELAST) ||
				(message>=WM_NCMOUSEMOVE && message<=WM_NCMBUTTONDBLCLK)))
			{
				CheckMousePos();
			}

			break;
		}
	}

	return PARENTWND::WindowProc(message, wParam, lParam);
}


template<class PARENTWND>
void COXCoolCtrl<PARENTWND>::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	_AFX_THREAD_STATE* pThreadState=AfxGetThreadState();
	// hook not already in progress
	if(pThreadState->m_pWndInit==NULL && !m_bInitializedCoolCtrl)
	{
		m_bInitializedCoolCtrl=TRUE;
		InitializeCoolCtrl();
	}
	
	PARENTWND::PreSubclassWindow();
}


template<class PARENTWND>
void COXCoolCtrl<PARENTWND>::CheckMousePos(BOOL bChildren/*=TRUE*/)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	// Check whether the mouse is over the hooked window
	CPoint point;
	::GetCursorPos(&point);
	CWnd* pWnd=CWnd::WindowFromPoint(point);
	BOOL bIsMouseOver=(pWnd->GetSafeHwnd()==GetSafeHwnd());

	if(!bIsMouseOver && bChildren)
	{
		bIsMouseOver=IsDescendant(this,pWnd);
	}

	if(IsEditControl(GetSafeHwnd()) && IsSpinButtonControl(pWnd->GetSafeHwnd()))
	{
		if((HWND)::SendMessage(pWnd->GetSafeHwnd(),
			UDM_GETBUDDY,NULL,NULL)==GetSafeHwnd())
		{
			bIsMouseOver=TRUE;
		}
	}

	if(IsSpinButtonControl(GetSafeHwnd()))
	{
		HWND hWndBuddy=(HWND)::SendMessage(pWnd->GetSafeHwnd(),
			UDM_GETBUDDY,NULL,NULL);
		if(hWndBuddy!=NULL && IsEditControl(hWndBuddy))
		{
			::SendMessage(hWndBuddy,WM_TIMER,IDT_COOLCTRL_CHECKMOUSETIMER,NULL);
		}
	}

	if(bIsMouseOver!=m_bMouseIsOver)
	{
		m_bMouseIsOver=bIsMouseOver;
		if(m_bMouseIsOver)
		{
			OnMouseEnter(point);
		}
		else
		{
			OnMouseLeave(point);
		}

		if(m_bMouseIsOver)
		{
			if(m_nCheckMouseTimerID==0)
			{
				m_nCheckMouseTimerID=SetTimer(IDT_COOLCTRL_CHECKMOUSETIMER,
					ID_COOLCTRL_DFLT_CHECKMOUSEDELAY,NULL);
				if(m_nCheckMouseTimerID==0)
				{
					TRACE(_T("COXCoolCtrl::CheckMousePos: failed to set timer\n"));
				}
			}
		}
		else
		{
			if(m_nCheckMouseTimerID!=0)
			{
				ASSERT(::IsWindow(GetSafeHwnd()));
				VERIFY(KillTimer(m_nCheckMouseTimerID));
				m_nCheckMouseTimerID=0;
			}
		}
	}
}	


template<class PARENTWND>
BOOL COXCoolCtrl<PARENTWND>::IsDescendant(CWnd* pWndParent, CWnd* pWndChild)
	// helper for detecting whether child descendent of parent
	//  (works with owned popups as well)
{
	ASSERT(pWndParent!=NULL);
	ASSERT(::IsWindow(pWndParent->GetSafeHwnd()));
	if(pWndChild==NULL)
		return FALSE;
	ASSERT(::IsWindow(pWndChild->GetSafeHwnd()));

	HWND hWndParent=pWndParent->GetSafeHwnd();
	HWND hWndChild=pWndChild->GetSafeHwnd();
	do
	{
		if (hWndParent == hWndChild)
			return TRUE;

		// check for permanent-owned window first
		CWnd* pWnd=CWnd::FromHandlePermanent(hWndChild);
		if(pWnd!=NULL)
			hWndChild=pWnd->GetOwner()->GetSafeHwnd();
		else
			hWndChild=(::GetWindowLong(hWndChild,GWL_STYLE)&WS_CHILD) ?
				::GetParent(hWndChild) : ::GetWindow(hWndChild,GW_OWNER);

	} while (hWndChild != NULL);

	return FALSE;
}


template<class PARENTWND>
BOOL COXCoolCtrl<PARENTWND>::IsEditControl(HWND hWnd)
{
	CString sClassName;
	::GetClassName(hWnd,sClassName.GetBuffer(256),256);
	if(sClassName.CompareNoCase(_T("Edit"))==0)
		return TRUE;

	return FALSE;
}


template<class PARENTWND>
BOOL COXCoolCtrl<PARENTWND>::IsComboBox(HWND hWnd)
{
	CString sClassName;
	::GetClassName(hWnd,sClassName.GetBuffer(256),256);
	if(sClassName.CompareNoCase(_T("ComboBox"))==0)
		return TRUE;

	return FALSE;
}


template<class PARENTWND>
BOOL COXCoolCtrl<PARENTWND>::IsListBox(HWND hWnd)
{
	CString sClassName;
	::GetClassName(hWnd,sClassName.GetBuffer(256),256);
	if(sClassName.CompareNoCase(_T("LISTBOX"))==0)
		return TRUE;

	return FALSE;
}


template<class PARENTWND>
BOOL COXCoolCtrl<PARENTWND>::IsSpinButtonControl(HWND hWnd)
{
	CString sClassName;
	::GetClassName(hWnd,sClassName.GetBuffer(256),256);
	if(sClassName.CompareNoCase(_T("msctls_updown32"))==0)
		return TRUE;

	return FALSE;
}


template<class PARENTWND>
HWND COXCoolCtrl<PARENTWND>::FindAttachedSpinButton(HWND hWnd)
{
	if(!IsEditControl(hWnd))
		return NULL;

	CWnd* pParentWnd=CWnd::FromHandle(::GetParent(hWnd));
	if(pParentWnd==NULL)
		return NULL;

	CWnd* pChildWnd=pParentWnd->GetWindow(GW_CHILD);
	while(pChildWnd!=NULL)
	{
		if(IsSpinButtonControl(pChildWnd->GetSafeHwnd()))
		{
			if(CWnd::FromHandlePermanent(pChildWnd->GetSafeHwnd())!=NULL)
			{
				if((HWND)pChildWnd->SendMessage(UDM_GETBUDDY,NULL,NULL)==hWnd)
				{
					if(pChildWnd->GetStyle()&UDS_ALIGNRIGHT || 
						pChildWnd->GetStyle()&UDS_ALIGNLEFT)
					{
						return pChildWnd->GetSafeHwnd();
					}
				}
			}
		}
		pChildWnd=pChildWnd->GetWindow(GW_HWNDNEXT);
	}

	return NULL;
}


template<class PARENTWND>
BOOL COXCoolCtrl<PARENTWND>::InitializeCoolCtrl()
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	ASSERT(m_nCheckMouseTimerID==0);

	OnChangeHotState(IsDescendant(this,GetFocus()));

	ASSERT(!::IsWindow(m_toolTip.GetSafeHwnd()));
	if(!m_toolTip.Create(this))
	{
		TRACE(_T("COXCoolCtrl::Initialize: Failed to create tool tip control\n"));
		return FALSE;
	}
	m_toolTip.Activate(TRUE);

	return TRUE;
}


template<class PARENTWND>
BOOL COXCoolCtrl<PARENTWND>::SetTooltipText(const CString& sTooltipText)
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	if(::IsWindow(m_toolTip.GetSafeHwnd()))
	{
		if(!OnChangeTooltipText(sTooltipText))
			return FALSE;

		if(!sTooltipText.IsEmpty())
		{
			CString sOldTooltipText;
			if(GetTooltipText(sOldTooltipText))
			{
				if(sTooltipText.Collate(sOldTooltipText)==0)
					return TRUE;
				m_toolTip.DelTool(this,OXCOOLCTRL_TOOLTIP_ID);
			}
			CRect rect;
			GetClientRect(rect);
			VERIFY(m_toolTip.AddTool(this, sTooltipText, rect, 
				OXCOOLCTRL_TOOLTIP_ID));
		}
		else
		{
			m_toolTip.DelTool(this,OXCOOLCTRL_TOOLTIP_ID);
		}

		return TRUE;
	}

	return FALSE;
}


template<class PARENTWND>
BOOL COXCoolCtrl<PARENTWND>::GetTooltipText(CString& sTooltipText) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	if(::IsWindow(m_toolTip.GetSafeHwnd()))
	{
		CToolInfo toolInfo;
		if(m_toolTip.GetToolInfo(toolInfo,(CWnd*)this,OXCOOLCTRL_TOOLTIP_ID))
			sTooltipText=toolInfo.lpszText;
		else
			sTooltipText=_T("");
		return TRUE;
	}
	
	return FALSE;
}


template<class PARENTWND>
BOOL COXCoolCtrl<PARENTWND>::IsInCoolState() const 
{ 
	return m_bCoolState; 
}


template<class PARENTWND>
void COXCoolCtrl<PARENTWND>::SetCoolState(BOOL bCoolState) 
{ 
	if(m_bCoolState!=bCoolState)
	{
		m_bCoolState=bCoolState;
		if(m_bCoolState)
		{
			CheckMousePos();
			if(!IsInHotState() && IsDescendant(this,GetFocus()))
				OnChangeHotState(TRUE);
		}

		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_FRAME);
	}
}


template<class PARENTWND>
BOOL COXCoolCtrl<PARENTWND>::IsInHotState() const 
{ 
	return m_bInHotState; 
}


template<class PARENTWND>
BOOL COXCoolCtrl<PARENTWND>::IsMouseOver() const 
{ 
	return m_bMouseIsOver; 
}


template<class PARENTWND>
void COXCoolCtrl<PARENTWND>::OnChangeHotState(BOOL bInHotState) 
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	if(!bInHotState && IsDescendant(this,GetFocus()))
		return;
	if(m_bInHotState!=bInHotState)
	{
		m_bInHotState=bInHotState;
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_FRAME);

		HWND hAttachedSpinButton=FindAttachedSpinButton(GetSafeHwnd());
		if(hAttachedSpinButton!=NULL)
		{
			::RedrawWindow(hAttachedSpinButton,NULL,NULL,
				RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_FRAME);
		}
	}
}

template<class PARENTWND>
void COXCoolCtrl<PARENTWND>::OnFocusChange(CWnd* pWndGainFocus, 
										   CWnd* pWndLooseFocus) 
{
	if((pWndGainFocus==this && !m_bInHotState) ||
		(pWndLooseFocus==this && m_bInHotState))
	{
		if(pWndLooseFocus!=NULL && pWndGainFocus!=NULL)
		{
			if(FindAttachedSpinButton(pWndLooseFocus->GetSafeHwnd())==
				pWndGainFocus->GetSafeHwnd())
			{
				ASSERT(IsSpinButtonControl(pWndGainFocus->GetSafeHwnd()));
				// don't reset hot state
				return;
			}
			else if(IsDescendant(pWndLooseFocus,pWndGainFocus))
			{
				// don't reset hot state
				return;
			}
		}
		OnChangeHotState(!m_bInHotState);
	}
}
	
template<class PARENTWND>
BOOL COXCoolCtrl<PARENTWND>::HandleKey(int nVirtKey) 
{ 
	UNREFERENCED_PARAMETER(nVirtKey);
	return FALSE; 
}

template<class PARENTWND>
void COXCoolCtrl<PARENTWND>::OnMouseEnter(const CPoint& ptCursor) 
{
	UNREFERENCED_PARAMETER(ptCursor);
	if(!m_bInHotState)
		OnChangeHotState(!m_bInHotState);
}

template<class PARENTWND>
void COXCoolCtrl<PARENTWND>::OnMouseLeave(const CPoint& ptCursor) 
{
	UNREFERENCED_PARAMETER(ptCursor);
	if(m_bInHotState)
		OnChangeHotState(!m_bInHotState);
}


template<class PARENTWND>
BOOL COXCoolCtrl<PARENTWND>::OnChangeTooltipText(const CString& sTooltipText)
{
	UNREFERENCED_PARAMETER(sTooltipText);
	return TRUE;
}


template<class PARENTWND>
void COXCoolCtrl<PARENTWND>::DrawNcBorders()
{
	if(!IsInCoolState())
		return;

    // set up all the pens we are likely to need
    static CPen penHilite(PS_SOLID,1,::GetSysColor(COLOR_3DHIGHLIGHT));
    static CPen penShadow(PS_SOLID,1,::GetSysColor(COLOR_3DSHADOW));
    static CPen penFace3D(PS_SOLID,1,::GetSysColor(COLOR_3DFACE));
    static CPen penDkShad3D(PS_SOLID,1,::GetSysColor(COLOR_3DDKSHADOW));

	CRect rectClient;
	GetClientRect(rectClient);
	CRect rectWindow;
	GetWindowRect(rectWindow);
	ClientToScreen(rectClient);

	if(rectClient==rectWindow)
		return;

	CRect rectAttached(0,0,0,0);
	HWND hAttachedSpinButton=FindAttachedSpinButton(GetSafeHwnd());
	if(hAttachedSpinButton!=NULL)
	{
		ASSERT(IsSpinButtonControl(hAttachedSpinButton));
		::GetWindowRect(hAttachedSpinButton,rectAttached);
	}

	rectAttached-=rectWindow.TopLeft();
	rectClient-=rectWindow.TopLeft();
	rectWindow-=rectWindow.TopLeft();
	rectAttached.IntersectRect(rectAttached,rectWindow);

    const BOOL bIsEnabled=IsWindowEnabled();
	const BOOL bIsHot=IsInHotState() & bIsEnabled;

    // paint background
    CWindowDC dc(this);
	
    // set up rectangles for scrollbars
	BOOL bHasHScrollBar=((GetStyle()&WS_HSCROLL)==WS_HSCROLL);
	BOOL bHasVScrollBar=((GetStyle()&WS_VSCROLL)==WS_VSCROLL);

	
	CRect rectHorzScrollBar(0,0,0,0);
	if(bHasHScrollBar)
	{
		rectHorzScrollBar=rectClient;
		rectHorzScrollBar.top=rectHorzScrollBar.bottom;
		rectHorzScrollBar.bottom+=::GetSystemMetrics(SM_CYHSCROLL);

		DrawScrollBar(&dc,rectHorzScrollBar,TRUE);
	}

	CRect rectVertScrollBar(0,0,0,0);
	if(bHasVScrollBar)
	{
		rectVertScrollBar=rectClient;
		rectVertScrollBar.left=rectVertScrollBar.right;
		rectVertScrollBar.right+=::GetSystemMetrics(SM_CXVSCROLL);

		DrawScrollBar(&dc,rectVertScrollBar,FALSE);
	}

    // but first exclude the 'real' client area from the DC
    dc.SelectClipRgn(NULL);   // from drawing background
	dc.ExcludeClipRect(&rectAttached);
    dc.ExcludeClipRect(&rectClient);
    dc.ExcludeClipRect(&rectHorzScrollBar);
    dc.ExcludeClipRect(&rectVertScrollBar);

	// working rect
	CRect rect;

    // draw border
    if(!bIsHot)
    {
        // now paint background 
		rect=rectWindow;
		if(bIsEnabled)
	        dc.SelectObject(penHilite);
		else
	        dc.SelectObject(penShadow);
        dc.Rectangle(rect);
		rect.DeflateRect(1,1);
        dc.Rectangle(rect);
    }
    else
    {
        // HOT case - difficult to describe, but reasonably obvious
		rect=rectWindow;

        // paint the border
		dc.Draw3dRect(rect,::GetSysColor(COLOR_3DSHADOW),
			::GetSysColor(COLOR_3DHIGHLIGHT));
		rect.DeflateRect(1,1);
		dc.Draw3dRect(rect,::GetSysColor(COLOR_3DDKSHADOW),
			::GetSysColor(COLOR_3DFACE));
    }

    // unselect the clip region
    dc.SelectClipRgn(NULL);

    // now draw highlight border, if disabled
    if(!bIsEnabled)
    {
        ::SelectObject(dc.m_hDC,::GetStockObject(NULL_BRUSH));
        dc.SelectObject(penHilite);
        dc.Rectangle(rectWindow);
    }

	if(hAttachedSpinButton!=NULL)
	{
		::RedrawWindow(hAttachedSpinButton,NULL,NULL,
			RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW|RDW_FRAME);
	}
}


#define SCROLLTHUMB_MIN_LENGTH			8

template<class PARENTWND>
void COXCoolCtrl<PARENTWND>::DrawScrollBar(CDC* pDC,CRect rectBar,BOOL bHorz)
{
	if(!IsInCoolState() || IsInHotState())
		return;

	// draw scroll bar arrow buttons
	//
	CRect rectScrollButton1=rectBar;
	CRect rectScrollButton2=rectBar;
	CRect rectThumb=rectBar;
	int nButtonWidth=::GetSystemMetrics(SM_CXHSCROLL);
	int nButtonHeight=::GetSystemMetrics(SM_CYVSCROLL);
	int nScrollBarLength=0;
	if(bHorz)
	{
		if(nButtonWidth*2>rectBar.Width())
			nButtonWidth=rectBar.Width()/2;
		rectScrollButton1.right=rectScrollButton1.left+nButtonWidth;
		rectScrollButton2.left=rectScrollButton2.right-nButtonWidth;
		nScrollBarLength=rectBar.Width()-nButtonWidth*2;
		rectThumb.left+=nButtonWidth;
		rectThumb.right-=nButtonWidth;
	}
	else
	{
		if(nButtonHeight*2>rectBar.Height())
			nButtonHeight=rectBar.Height()/2;
		rectScrollButton1.bottom=rectScrollButton1.top+nButtonHeight;
		rectScrollButton2.top=rectScrollButton2.bottom-nButtonHeight;
		nScrollBarLength=rectBar.Height()-nButtonHeight*2;
		rectThumb.top+=nButtonHeight;
		rectThumb.bottom-=nButtonHeight;
	}
	
	pDC->Draw3dRect(rectScrollButton1,::GetSysColor(COLOR_3DHIGHLIGHT),
		::GetSysColor(COLOR_3DSHADOW));
	rectScrollButton1.DeflateRect(1,1);
	pDC->Draw3dRect(rectScrollButton1,::GetSysColor(COLOR_3DFACE),
		::GetSysColor(COLOR_3DFACE));

	pDC->Draw3dRect(rectScrollButton2,::GetSysColor(COLOR_3DHIGHLIGHT),
		::GetSysColor(COLOR_3DSHADOW));
	rectScrollButton2.DeflateRect(1,1);
	pDC->Draw3dRect(rectScrollButton2,::GetSysColor(COLOR_3DFACE),
		::GetSysColor(COLOR_3DFACE));
	//
	/////////////////////////////

	// draw thumb
	//
	SCROLLINFO scrollInfo={ sizeof(SCROLLINFO) };
	scrollInfo.fMask=SIF_ALL;
	if(!CWnd::GetScrollInfo((bHorz ? SB_HORZ : SB_VERT),&scrollInfo,SIF_ALL))
	{
		SendMessage(SBM_GETSCROLLINFO,NULL,(LPARAM)&scrollInfo);
	}
	int nThumbWidth=0;
	if((int)scrollInfo.nPage>scrollInfo.nMax || scrollInfo.nPage==0 ||
		nScrollBarLength<SCROLLTHUMB_MIN_LENGTH)
	{
		nThumbWidth=0;
	}
	else
	{
		nThumbWidth=(int)((float)(nScrollBarLength*scrollInfo.nPage)/
			(float)(scrollInfo.nMax+1)+0.5f);
		if(nThumbWidth<SCROLLTHUMB_MIN_LENGTH)
			nThumbWidth=SCROLLTHUMB_MIN_LENGTH;
	}

	if(nThumbWidth>0)
	{
		int nThumbPos=(int)((float)((nScrollBarLength-nThumbWidth)*scrollInfo.nPos)/
			(float)(scrollInfo.nMax-scrollInfo.nPage+1)+0.5);

		if(bHorz)
		{
			rectThumb.left+=nThumbPos;
			rectThumb.right=rectThumb.left+nThumbWidth;

			if(rectThumb.right>nScrollBarLength+nButtonWidth+rectBar.left)
			{
				rectThumb.OffsetRect(nScrollBarLength+
					nButtonWidth+rectBar.left-rectThumb.right,0);
			}
		}
		else
		{
			rectThumb.top+=nThumbPos;
			rectThumb.bottom=rectThumb.top+nThumbWidth;

			if(rectThumb.bottom>nScrollBarLength+nButtonHeight+rectBar.top)
			{
				rectThumb.OffsetRect(0,nScrollBarLength+
					nButtonHeight+rectBar.top-rectThumb.bottom);
			}
		}

		pDC->Draw3dRect(rectThumb,::GetSysColor(COLOR_3DHIGHLIGHT),
			::GetSysColor(COLOR_3DSHADOW));
		rectThumb.DeflateRect(1,1);
		pDC->Draw3dRect(rectThumb,::GetSysColor(COLOR_3DFACE),
			::GetSysColor(COLOR_3DFACE));
	}

	//
	/////////////////////////////

}


/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// COXCoolButton window

class OX_CLASS_DECL COXCoolButton : public COXCoolCtrl<CButton>
{
// Construction
public:

	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXCoolButton() {};

// Attributes
public:

protected:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXCoolButton)
	//}}AFX_VIRTUAL

protected:
	// overriden virtual functions
	// called when hot state is about to be changed
	virtual void OnChangeHotState(BOOL bInHotState);
	/////////////////////////////////////////

// Implementation
public:
	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXCoolButton() {};

	// Generated message map functions
protected:
	//{{AFX_MSG(COXCoolButton)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// COXCoolScrollBar window

class OX_CLASS_DECL COXCoolScrollBar : public COXCoolCtrl<CScrollBar>
{
// Construction
public:

	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXCoolScrollBar() {};

// Attributes
public:

protected:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXCoolScrollBar)
	//}}AFX_VIRTUAL

protected:
	// overriden virtual functions
	// called when hot state is about to be changed
	virtual void OnChangeHotState(BOOL bInHotState);
	/////////////////////////////////////////

// Implementation
public:
	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXCoolScrollBar() {};

	// Generated message map functions
protected:
	//{{AFX_MSG(COXCoolScrollBar)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// COXCoolSpinButtonCtrl window

class OX_CLASS_DECL COXCoolSpinButtonCtrl : public COXCoolCtrl<CSpinButtonCtrl>
{
// Construction
public:

	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXCoolSpinButtonCtrl() {};

// Attributes
public:

protected:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXCoolSpinButtonCtrl)
	//}}AFX_VIRTUAL

protected:
	// overriden virtual functions
	// called when hot state is about to be changed
	virtual void OnChangeHotState(BOOL bInHotState);
	/////////////////////////////////////////

// Implementation
public:
	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXCoolSpinButtonCtrl() {};

	BOOL IsAttached();

	// Generated message map functions
protected:
	//{{AFX_MSG(COXCoolSpinButtonCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// COXCoolSliderCtrl window

class OX_CLASS_DECL COXCoolSliderCtrl : public COXCoolCtrl<CSliderCtrl>
{
// Construction
public:

	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXCoolSliderCtrl() {};

// Attributes
public:

protected:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXCoolSliderCtrl)
	//}}AFX_VIRTUAL

protected:
	// overriden virtual functions
	// called when hot state is about to be changed
	virtual void OnChangeHotState(BOOL bInHotState);
	/////////////////////////////////////////

// Implementation
public:
	// --- In  :	
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXCoolSliderCtrl() {};

	// Generated message map functions
protected:
	//{{AFX_MSG(COXCoolSliderCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////


typedef COXCoolCtrl<CEdit> COXCoolEdit;
typedef COXCoolCtrl<CRichEditCtrl> COXCoolRichEditCtrl;
typedef COXCoolCtrl<CTreeCtrl> COXCoolTreeCtrl;
typedef COXCoolCtrl<CListCtrl> COXCoolListCtrl;
typedef COXCoolCtrl<CListBox> COXCoolListBox;
typedef COXCoolCtrl<CProgressCtrl> COXCoolProgressCtrl;
typedef COXCoolCtrl<CHotKeyCtrl> COXCoolHotKeyCtrl;
#if _MFC_VER > 0x0421
#include "afxdtctl.h"
typedef COXCoolCtrl<CDateTimeCtrl> COXCoolDateTimeCtrl;
typedef COXCoolCtrl<CMonthCalCtrl> COXCoolMonthCalCtrl;
typedef COXCoolCtrl<CIPAddressCtrl> COXCoolIPAddressCtrl;
#endif	//	_MFC_VER > 0x0421



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_OXCOOLCTRL_H__)

