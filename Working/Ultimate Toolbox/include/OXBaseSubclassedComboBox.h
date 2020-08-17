// ==========================================================================
//								Class Specification : 
//							COXBaseSubclassedComboBox
// ==========================================================================

// Header file : OXBaseSubclassedComboBox.h

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

Most common controls are relatively easy to customize through subclassing. E.g if
you need to use your own list box control with extended functionality you just 
have to subclass the existing list box. Most common controls are self sufficient.
That means that all control's functionality resides within the control and it 
doesn't depend on anything else.

But it is not the case with combo box control which consists of 3 controls:
combo box, dropdown list box and edit (edit control is not used when LBS_DROPDOWNLIST
style is specified). Imagine that you want to combine the functionality of different 
derived classes with extended functionality into new control. E.g you might want to 
create history combo box with masked edit control used to type in the items or 
multicolumn combo box that allows a user to type in only currency info.

In this case the following template class will come in handy: 

	template<class PARENTCOMBOBOX, class PARENTLISTBOX, class PARENTEDIT>
	class COXBaseSubclassedComboBox : public PARENTCOMBOBOX { ... }

As you can see you must provide name of the class for combo box, list box and 
edit control. So if you might declare new class as:

	class CHistoryMaskedComboBox : 
	public COXBaseSubclassedComboBox<COXHistoryCombo, CListBox, COXMaskedEdit> { ... }

in order to create a history combo box with masked edit control used to type in 
the items.

Usually the declaring of new class in this way is all you have to do in order to 
get new functionality. But sometimes you have to provide additional initialization 
steps. In this case you might be interested in overriding the following virtual
functions:

	virtual CEdit* GetEditCtrl();
	virtual CListBox* GetListBoxCtrl();
	virtual void SubclassEditCtrl(CWnd* pWnd);
	virtual void SubclassListBoxCtrl(CWnd* pWnd);

Refer to the documentation on this functions for details.


Dependency:

	#include "OXBaseSubclassedComboBox.h"


*/ 
#ifndef _OX_SUBCLASSEDCOMBOBOX_H__
#define _OX_SUBCLASSEDCOMBOBOX_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"

#ifdef SubclassWindow
#undef SubclassWindow
#endif


template<class PARENTCOMBOBOX, class PARENTLISTBOX, class PARENTEDIT>
class COXBaseSubclassedComboBox : public PARENTCOMBOBOX
{
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect:	Constructs the object
	COXBaseSubclassedComboBox();

// Attributes
public:

protected:
	// dropdown list box control
	PARENTLISTBOX m_listBox;
	// edit control
	PARENTEDIT m_edit;

	// flag that spesifies if corresponding controls has already been subclassed
	BOOL m_bSubclassedListBoxCtrl;
	BOOL m_bSubclassedEditCtrl;

// Operations
public:
	// --- In  :
	// --- Out : 
	// --- Returns: Pointer to the internal edit control
	// --- Effect:	Retrieves a pointer to the internal edit control. Advanced 
	//				overridable can be overridden in order to supply non default
	//				edit control.
	virtual CEdit* GetEditCtrl() { return (CEdit*)&m_edit; }


	// --- In  :
	// --- Out : 
	// --- Returns: Pointer to the internal list box control
	// --- Effect:	Retrieves a pointer to the internal list box control. Advanced 
	//				overridable can be overridden in order to supply non default
	//				list box control.
	virtual CListBox* GetListBoxCtrl() { return (CListBox*)&m_listBox; }

// Implementation
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect:	Destructs the object
	virtual ~COXBaseSubclassedComboBox();


	// called when the control is being subclassed
	virtual void PreSubclassWindow();

protected:
	// --- In  :	pWnd	-	pointer to a CWnd object that represents the 
	//							internal edit control. Advanced overridable, can be 
	//							overridden in order to provide additional 
	//							initialization when the object has just been 
	//							subclassed.
	// --- Out : 
	// --- Returns:
	// --- Effect:	Subclasses the internal edit control using object returned by
	//				GetEditCtrl() function
	virtual void SubclassEditCtrl(CWnd* pWnd);

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if Edit control has already been subclassed
	// --- Effect:	Retrieves the flag that specifies that edit control has already 
	//				been subclassed
	virtual BOOL IsSubclassedEditCtrl() const { return m_bSubclassedEditCtrl; }



	// --- In  :	pWnd	-	pointer to a CWnd object that represents the 
	//							internal list box control. Advanced overridable, 
	//							can be overridden in order to provide additional 
	//							initialization when the object has just been 
	//							subclassed.
	// --- Out : 
	// --- Returns:
	// --- Effect:	Subclasses the internal list box control using object returned by
	//				GetListBoxCtrl() function
	virtual void SubclassListBoxCtrl(CWnd* pWnd);

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if list box control has already been subclassed
	// --- Effect:	Retrieves the flag that specifies that list box control has 
	//				already been subclassed
	virtual BOOL IsSubclassedListBoxCtrl() const { return m_bSubclassedListBoxCtrl; }


	// --- In  :
	// --- Out : 
	// --- Returns:	TRUE if combo box was successfully initialized
	// --- Effect:	Initializes the internal state of the combo box. Called right
	//				after the control was created or subclassed
	virtual BOOL InitializeComboBox();


	// handles some messages that are important for internal implementation
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////

template<class PARENTCOMBOBOX, class PARENTLISTBOX, class PARENTEDIT>
COXBaseSubclassedComboBox<PARENTCOMBOBOX,PARENTLISTBOX,PARENTEDIT>::
COXBaseSubclassedComboBox() : 	m_bSubclassedListBoxCtrl(FALSE),
								m_bSubclassedEditCtrl(FALSE)

{
#ifdef _DEBUG
	PARENTCOMBOBOX comboBox;
	if(!comboBox.IsKindOf(RUNTIME_CLASS(CComboBox)))
	{
		TRACE(_T("COXBaseSubclassedComboBox<PARENTCOMBOBOX,PARENTLISTBOX,PARENTEDIT>::COXBaseSubclassedComboBox: Parent class must be CComboBox derived\n"));
		AfxThrowNotSupportedException();
	}

	PARENTLISTBOX listBox;
	if(!listBox.IsKindOf(RUNTIME_CLASS(CListBox)))
	{
		TRACE(_T("COXBaseSubclassedComboBox<PARENTCOMBOBOX,PARENTLISTBOX,PARENTEDIT>::COXBaseSubclassedComboBox: Parent class for the dropdown list box must be CListBox derived\n"));
		AfxThrowNotSupportedException();
	}

	PARENTEDIT edit;
	if(!edit.IsKindOf(RUNTIME_CLASS(CEdit)))
	{
		TRACE(_T("COXBaseSubclassedComboBox<PARENTCOMBOBOX,PARENTLISTBOX,PARENTEDIT>::COXBaseSubclassedComboBox: Parent class for the combo box edit control must be CEdit derived\n"));
		AfxThrowNotSupportedException();
	}
#endif
}


template<class PARENTCOMBOBOX, class PARENTLISTBOX, class PARENTEDIT>
COXBaseSubclassedComboBox<PARENTCOMBOBOX,PARENTLISTBOX,PARENTEDIT>::
~COXBaseSubclassedComboBox() 
{
}


template<class PARENTCOMBOBOX, class PARENTLISTBOX, class PARENTEDIT>
LRESULT COXBaseSubclassedComboBox<PARENTCOMBOBOX,PARENTLISTBOX,PARENTEDIT>::
WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	switch(message)
	{
	case WM_CTLCOLOREDIT:
		{
			LRESULT lResult=PARENTCOMBOBOX::WindowProc(message,wParam,lParam);
			if(!IsSubclassedEditCtrl())
			{
				m_bSubclassedEditCtrl=TRUE;
				SubclassEditCtrl(CWnd::FromHandle((HWND)lParam));
			}
			return lResult;
		}
	case WM_CTLCOLORLISTBOX:
		{
			LRESULT lResult=PARENTCOMBOBOX::WindowProc(message,wParam,lParam);
			if(!IsSubclassedListBoxCtrl())
			{
				m_bSubclassedListBoxCtrl=TRUE;
				SubclassListBoxCtrl(CWnd::FromHandle((HWND)lParam));
			}
			return lResult;
		}
	case WM_CREATE:
		{
			LRESULT lResult=PARENTCOMBOBOX::WindowProc(message,wParam,lParam);
			if(!InitializeComboBox())
			{
				lResult=FALSE;
			}
			return lResult;
		}
	}

	return PARENTCOMBOBOX::WindowProc(message,wParam,lParam);
}


template<class PARENTCOMBOBOX, class PARENTLISTBOX, class PARENTEDIT>
void COXBaseSubclassedComboBox<PARENTCOMBOBOX,PARENTLISTBOX,PARENTEDIT>::
SubclassEditCtrl(CWnd* pWnd) 
{
	ASSERT(pWnd!=NULL);
	HWND hWnd=pWnd->GetSafeHwnd();
	VERIFY(GetEditCtrl()->SubclassWindow(hWnd)); 
}


template<class PARENTCOMBOBOX, class PARENTLISTBOX, class PARENTEDIT>
void COXBaseSubclassedComboBox<PARENTCOMBOBOX,PARENTLISTBOX,PARENTEDIT>::
SubclassListBoxCtrl(CWnd* pWnd) 
{
	ASSERT(pWnd!=NULL);
	HWND hWnd=pWnd->GetSafeHwnd();
	VERIFY(GetListBoxCtrl()->SubclassWindow(hWnd)); 
}


template<class PARENTCOMBOBOX, class PARENTLISTBOX, class PARENTEDIT>
BOOL COXBaseSubclassedComboBox<PARENTCOMBOBOX,PARENTLISTBOX,PARENTEDIT>::
InitializeComboBox() 
{
	return TRUE;
}


template<class PARENTCOMBOBOX, class PARENTLISTBOX, class PARENTEDIT>
void COXBaseSubclassedComboBox<PARENTCOMBOBOX,PARENTLISTBOX,PARENTEDIT>::
PreSubclassWindow() 
{
	_AFX_THREAD_STATE* pThreadState=AfxGetThreadState();
	// hook not already in progress
	if(pThreadState->m_pWndInit==NULL)
	{
		if(!InitializeComboBox())
		{
			TRACE(_T("COXBaseSubclassedComboBox::PreSubclassWindow: failed to initialize the control\n"));
		}
	}

	PARENTCOMBOBOX::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////

#endif	//	#ifndef _OX_SUBCLASSEDCOMBOBOX_H__