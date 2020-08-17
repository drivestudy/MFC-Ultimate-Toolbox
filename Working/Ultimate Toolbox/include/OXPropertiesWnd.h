// ============================================================================
// 					 Class Specification : COXPropertiesWnd
// ============================================================================
//
// Version: 9.3
//
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
//                        
///////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OXPROPERTIESWND_H__733A17FD_6455_4497_98A8_99A2E9457DFC__INCLUDED_)
#define AFX_OXPROPERTIESWND_H__733A17FD_6455_4497_98A8_99A2E9457DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OXPropertiesWnd.h : header file
//

#define WM_USER_STARTEDITING (WM_USER + 94)

#include "OXDllExt.h"

#include "OXPropertyDescriptionPane.h"
#include "OXCoolToolBar.h"	
#include "OXBrowseColorEdit.h"
#include "OXPropertyTree.h"


/////////////////////////////////////////////////////////////////////////////
// COXPropertiesWnd window

#define _COMBO_ID				1000
#define _TREE_ID				1001
#define _DESCRIPTION_ID			1002
#define _EDITOR_ID				1003

#define HCATEGORY				HTREEITEM
#define HPROPERTY				HTREEITEM

/*
///////////////////////////////////////////////////////////////////////////////
Description:

The COXPropertiesWnd class implements a properties window similar to the one
found in Visual Studio .NET. The SkinsDemo sample found in /samples/advanced
demonstrates some of the capabilities of COXPropertiesWnd. The method
CPropertiesBar::InitProperties() is of particular interest since it shows how
easy it is to add different property types and editors to the properties
window.

The properties window is very flexible and is designed to support different
property value editors derived from CEdit, COXEdit or COXMaskedEdit. All you
have to do is pass a pointer to an editor when calling
COXPropertiesWnd::InsertProperty(...). The following Ultimate ToolBox classes
are ready to be used as property editors:

  COXEdit			- text field

  COXListEdit		- text field with a drop list.

  COXNumericEdit	- numeric field

  COXMaskedEdit		- masked field - can be used for phone numbers, serial
					  numbers, etc.

  COXCurrencyEdit	- currency field.

  COXIPEdit			- IP address field.

  COXCalendarEdit	- date field. The format of the date can be specified in
					  the constructor.

  COXTimeEdit		- time interval field.

  COXAngleEdit		- angle field. (Degrees, radians and grades are supported).

  COXLengthEdit		- physical length field. (empyreal and metric units are
					  supported)

  COXTempEdit		- temperature field. (Celsius, Fahrenheit and Kelvin)

  COXBrowseColorEdit- color picker field.

  COXCalculatorEdit - numeric field with a drop down calculator.

  COXBrowseDirEdit	- directory field with a drop down browser.

  COXBrowseFileEdit - file name field with a drop down browser.

Also you can derive from any of these classes in order to implement further
validation or need more customization. Also you can implement your own drop
editor if you derive your editor class from COXDropEdit<COXEdit> and override
COXDropEdit::OnDropButton() virtual function.

In order to make the property values flexible they are set and retrieved as
strings by using the SetPropertyValue() and the GetPropertyValue() methods.

In addition to the properties area the properties window also contains a combo
box, a toolbar and a description area. The methods GetComboBoxEx() and
GetToolBar() can be used to obtain a pointer to the controls. Also all there
elements can be show or hidden using the ShowCombo(), ShowToolBar() and
ShowDescription() methods.

///////////////////////////////////////////////////////////////////////////////
*/

class COXFrameSkin;

class OX_CLASS_DECL COXPropertiesWnd : public CFrameWnd
{
	friend class COXListEdit;
	friend class COXPropertyTree;
	friend class COXPropertyDescriptionPane;

// Construction
public:
	COXPropertiesWnd();

// Attributes
public:

// Operations
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXPropertiesWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	// Overrides

	// --- In  :		hProperty		-	A handle to the property that changed.
	//					lpszOldValue	-	A string containing the previous value
	//					lpszNewValue	-	A string containing the new value
	// --- Out :	
	// --- Returns:	TRUE if the new value is to be kept, otherwise FALSE
	// --- Effect:	This function is called when the user changes a property value
	//				using the UI. Override this function in order to detect a change
	//				in a property value or to validate the new property value.
	virtual BOOL OnPropertyValueChanged(HPROPERTY hProperty, LPCTSTR lpszOldValue, LPCTSTR lpszNewValue);

	// --- In  :		hProperty		-	A handle to the property that changed.
	//					lpszOldValue	-	A string containing the previous value
	//					lpszNewValue	-	A string containing the new value
	// --- Out :	
	// --- Returns:	TRUE if it is okay to delete the property, false otherwise.
	// --- Effect:	This function is called when a property is about to be deleted.
	//				Override to perform any clean up and return FALSE if the property
	//				should not be deleted.
	virtual BOOL OnDeleteProperty(HPROPERTY hProperty);

// Implementation
public:
	// --- In  :
	// --- Out :	
	// --- Returns: A handle to the currently selected property or NULL if no
	//				is selected.
	// --- Effect:	Call this function to obtain the currently selected property.
	HPROPERTY GetSelectedProperty();

	// Selects the next property if any - Last Mod : Nish - June 14th, 2005
	BOOL SelectNextProperty();

	// --- In  :	bShow	-	If this parameter is true the combo will be visible,
	//							otherwize it will not. 
	// --- Out :	
	// --- Returns:
	// --- Effect:	Shows/hides the combo box.
	void ShowCombo(BOOL bShow = TRUE);

	// --- In  :	bShow	-	If this parameter is true the toolbar will be visible,
	//							otherwize it will not. 
	// --- Out :	
	// --- Returns:
	// --- Effect:	Shows/hides the toolbar.
	void ShowToolBar(BOOL bShow = TRUE);

	// --- In  :	bShow	-	If this parameter is true the description pane will
	//							be visible,	otherwize it will not. 
	// --- Out :	
	// --- Returns:
	// --- Effect:	Shows/hides the description pane.
	void ShowDescription(BOOL bShow = TRUE);

	// --- In  :	
	// --- Out :	
	// --- Returns:	A reference to the properties window combo box
	// --- Effect:	Obtains a reference to the properties window combo box.
	CComboBoxEx& GetComboBoxEx();

	// --- In  :	
	// --- Out :	
	// --- Returns:	A reference to the properties window toolbar
	// --- Effect:	Obtains a reference to the properties window toolbar.
	COXCoolToolBar& GetToolBar();

	// --- In  :	bCategorized	- if TRUE the properties window will display
	//								  the properties under their categopries.
	// --- Out :	
	// --- Returns:
	// --- Effect:	Causes the properties window to display the properties categories
	//				Clicking on the Categorized button has the same effect.
	void SetCategorized(BOOL bCategorized = TRUE);

	// --- In  :	lpszName		-	The name of the property.
	//				lpszCategory	-	The name of the category to which this property
	//									belongs. If this parameter is NULL the
	//									properties without a category will be searched.
	// --- Returns:	A handle to the found property or NULL if not property was found.
	// --- Effect:	Searches for a property under a category.
	HPROPERTY FindProperty(LPCTSTR lpszName, LPCTSTR lpszCategory = NULL);
	

	// --- In  :	hProperty		-	A handle to the property. Must not be NULL and
	//									the property must exist. Use FindProperty(...) to
	//									obtain the property handle.
	// --- Returns:	A string containing the property's value
	// --- Effect:	Retrienes the value of the given property.
	CString GetPropertyValue(HPROPERTY hProperty);

	// --- In  :	hProperty		-	A handle to the property. Must not be NULL and
	//									the property must exist. Use FindProperty(...) to
	//									obtain the property handle.
	//				lpszValue		-	A string containing the value to be set. Pass an empty
	//									string or NULL to clear the value.
	// --- Returns:	TRUE if successfull, FALSE otherwise
	// --- Effect:	Sets/clears the value of a property.
	BOOL SetPropertyValue(HPROPERTY hProperty, LPCTSTR lpszValue);

	// --- In  :	hProperty		-	A handle to the property. Must not be NULL and
	//									the property must exist. Use FindProperty(...) to
	//									obtain the property handle.
	// --- Returns:	TRUE if the operation was successfull, FALSE otherwise
	// --- Effect:	Deletes the given property.	If this property was under a category which is
	//				now empty the category will be removed as well.
	BOOL DeleteProperty(HPROPERTY hProperty);
		
	// --- In  :	lpszName		-	The name of the property.
	//				lpszInitialValue-	The initial value that this property will have.
	//				lpszDescription	-	The description of the property which is shown
	//									in the box below.
	//				lpszCategory	-	The name of the category to which this property belongs
	//									If this parameter is NULL the property will not belong to
	//									any category.
	//				pEditorClass	-	A pointer to a CEdit or derived editor class. If NULL
	//									the property will be read only.
	// --- Out :	
	// --- Returns:	A handle to the inserted property or NULL if the operation failed.				
	// --- Effect:	Inserts a new property into the property window.
	HPROPERTY InsertProperty(
		LPCTSTR lpszName,
		LPCTSTR lpszInitialValue = NULL,
		LPCTSTR lpszDescription = NULL,
		LPCTSTR lpszCategory = NULL,
		CEdit* pEditorClass = NULL);

	// --- In  :
	// --- Out :	
	// --- Returns:	TRUE if all the properties were deleted successfully, FALSE otherwise
	// --- Effect:	Deletes all the properties in the properties window.
	BOOL DeleteAll();

	// --- Effect : Sets a flag to indicate that categories are to be sorted
	void SetSortCategories(BOOL bSortCategories = TRUE);	
	
	// Destructor
	virtual ~COXPropertiesWnd();

protected:

	//List of categories
	CList<CString,CString&> m_listCategories;

	// A pointer to the current active editor
	CEdit* m_pEditWnd; 

	// Handle to the property that is currently being edited	
	HPROPERTY m_hEditProp;

	// HPROPERTY to Category name map that holds the parent-child
	// relationship between properties and categories
	CMap<HPROPERTY, HPROPERTY, CString, CString> m_mapProp2Cat;

	// HPROPERTY to Description map that holds the one-to-one
	// relationship between properties and their description
	CMap<HPROPERTY, HPROPERTY, CString, CString> m_mapProp2Desc; 
	
	// HPROPERTY to CEdit* map that binds each property to the
	// pointer of its editor
	CMap<HPROPERTY, HPROPERTY, CEdit*, CEdit*> m_mapProp2Editor;
	
	// A flag which is TRUE when the properties window in in categorized
	// mode and FALSE otherwise.
	BOOL m_bCategorized;

	// A flag set to TRUE to sort categories in categorized view. FALSE otherwise.
	// Default is TRUE.	
	BOOL m_bSortCategories;

	// The toolbar which appears on top of the properties window.
	COXCoolToolBar m_wndToolBar;

	// The description pane window.
	COXPropertyDescriptionPane m_wndDescription;
	
	// The combo box which appears on top of the properties window.
	CComboBoxEx m_wndCombo;
	
	// The properties tree window which displays all the properties.
	COXPropertyTree m_wndTree;

	// Finds a category by name
	HCATEGORY FindCategory(LPCTSTR lpszCategory);
	
	// Inserts a new category
	HCATEGORY InsertCategory(LPCTSTR lpszCategoryName);

	// Start editing a property
	void StartEditing(HPROPERTY hSelectedProp);
	
	// Finish editing a property
	void FinishEditing(BOOL bSave = TRUE);
	
	// Handle to the currently selected property
	HPROPERTY m_hCurrentProp;

	// helper for detecting whether child descendant of parent
	// (works with owned popups as well)
	static BOOL IsDescendant(CWnd* pWndParent, CWnd* pWndChild);

	// Message handlers
	//{{AFX_MSG(COXPropertiesWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCategorized();
	afx_msg void OnAlphabetic();
	afx_msg void OnPaint();
	afx_msg void OnUpdateCategorized(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAlphabetic(CCmdUI* pCmdUI);	
	//}}AFX_MSG
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	LRESULT OnQueryPropertiesWnd(WPARAM wParam, LPARAM lParam);
	LRESULT OnStartEditing(WPARAM wParam, LPARAM lParam);
	LRESULT OnUserAdjustLastColumn(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	COXFrameSkin* GetFrameSkin();
	COXFrameSkin* m_pFrameSkin;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OXPROPERTIESWND_H__733A17FD_6455_4497_98A8_99A2E9457DFC__INCLUDED_)
