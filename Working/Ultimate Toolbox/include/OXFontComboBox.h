// ==========================================================================
//	Class Specification :     COXFontComboBox
// ==========================================================================

// Header file : OXFontComboBox.h

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

COXFontComboBox is CComboBox derived class that uses COXFontListBox as dropdown
list box control. It is important to understand the functionality COXFontListBox
provides so it is recommended to read the documentation on this class before 
continuing reading this overview. 

COXFontListBox is a list box control which is automatically populated with
all fonts installed in the specified device context (by default we use screen
device context). So whenever a user clicks on the dropdown button the list box
with items that represent available fonts is displayed. The name of the selected 
font will appear in edit part of the combo box. 


	Usage

In order to use COXFontComboBox object in your application you have to create it
using standard CComboBox::Create function or subclass the existing control
(e.g. using DDX/DDV technology). When creating control explicitely or defining 
it in dialog template you have to make sure that the following requirements 
are met:

  CBS_OWNERDRAWVARIABLE must be specified
  CBS_HASSTRINGS style must be specified
  CBS_SORT style must NOT be specified

After control was successfully created or subclassed then before the dropdown list box
is displayed for first time it will be automatically populated with all fonts 
defined in screen device context (default device context). 

While a control is being populated with enumerated fonts the following protected 
virtual function will be called for every font before it is added to the list:

	virtual BOOL FilterFont(OXLBFONTINFO* pLBFI);

Default implementation of this function returns always TRUE. in your own 
implementation you might override it in order to filter the fonts displayed in 
the list box. 

More advanced customization is available through getting a pointer to internal
COXFontListBox object and calling its functions. Use this function in order to 
retrieve a pointer to the object:

	COXFontListBox* GetFontListBox();



The sample that demonstrates COXFontComboBox class is called ExtendedListBox and 
can be found in the .\Samples\gui\ExtendedListBox subdirectory of your 
Ultimate Toolbox directory. 



Dependency:
	#include "OXFontComboBox.h"


Source code files:
	"OXFontComboBox.cpp"
	"OXListBoxEx.cpp"		-	COXFontListBox implementation		

Resource file:
	#include "OXListBoxEx.rc"


*/

#if !defined(_OXFONTCOMBOBOX_H__)
#define _OXFONTCOMBOBOX_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"


#include "OXListBoxEx.h"
#include "OXBaseSubclassedComboBox.h"



class OX_CLASS_DECL COXFontListBoxHelper : public COXFontListBox
{
	DECLARE_DYNAMIC(COXFontListBoxHelper);
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect:	Constructs the object
	COXFontListBoxHelper();

// Attributes
public:

protected:
	// pointer to associated combo box
	class COXFontComboBox* m_pComboBox;

// Operations
public:
	// --- In  :	pComboBox	-	pointer to associated font combo box control
	// --- Out : 
	// --- Returns:
	// --- Effect:	Sets combo box that owns the control
	inline void AssociateComboBox(COXFontComboBox* pComboBox)
	{
		ASSERT(pComboBox!=NULL);
		m_pComboBox=pComboBox;
	}

	// --- In  :	
	// --- Out : 
	// --- Returns: Pointer to associated font combo box control
	// --- Effect:	Retrieves pointer to the combo box that owns the control
	inline COXFontComboBox* GetAssociateComboBox() const { return m_pComboBox; }

protected:
	// --- In  :	pLBFI	-	pointer to OXLBFONTINFO structure that defines 
	//							the font that is about to be included in the list box.
	//							This structure is declared as follows:
	//
	//				typedef struct _tagOXLBFONTINFO
	//				{
	//					LOGFONT m_lf;
	//					DWORD m_dwType;
	//					BOOL m_bIsSymbol;
	//				} OXLBFONTINFO;
	//
	//				where
	//
	//				m_lf		-	log font info
	//				m_dwType	-	font type. Might be combination of these:
	//								TRUETYPE_FONTTYPE		TrueType font
	//								DEVICE_FONTTYPE			device-supplied font
	//								RASTER_FONTTYPE			raster font
	//				m_bIsSymbol	-	flag that set to TRUE if the font displays 
	//								only symbols
	// --- Out : 
	// --- Returns:	TRUE if specified font is qualified to be added to the list box.
	//				Default implementation of this function always returns TRUE.
	// --- Effect:	Checks if font can be included in the list box. Advanced 
	//				overridable, can be overridden in a derived class in order to 
	//				filter the fonts that are included in the list box
	virtual BOOL FilterFont(OXLBFONTINFO* pLBFI);

	// --- In  :
	// --- Out : 
	// --- Returns:	TRUE if control was successfully initialized
	// --- Effect:	Initializes the internal members of the control. Advanced 
	//				overridable, can be overridden in a derived class in order to 
	//				initialize an additional data
	virtual BOOL InitializeListBox();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXFontListBoxHelper)
	//}}AFX_VIRTUAL

// Implementation
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect:	Destructs the object
	virtual ~COXFontListBoxHelper();

	// Generated message map functions
protected:
	//{{AFX_MSG(COXFontListBoxHelper)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// COXFontComboBox window

// The following functions were introduced in order to be specifically used in 
// DoDataExchange function of any Dialog or FormView based application for 
// Font Combo Box. 
OX_API_DECL void AFXAPI DDX_FontComboFont(CDataExchange *pDX, int nIDC, CFont* pFont);

class OX_CLASS_DECL COXFontComboBox : 
public COXBaseSubclassedComboBox<CComboBox, COXFontListBoxHelper, CEdit>
{
	DECLARE_DYNAMIC(COXFontComboBox);
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect:	Constructs the object
	COXFontComboBox();

// Attributes
public:

protected:
	BOOL m_bAllowToUpdateMRU;

// Operations
public:

	// --- In  :	
	// --- Out : 
	// --- Returns: Pointer to associated font combo box control
	// --- Effect:	Retrieves pointer to the combo box that owns the control
	inline COXFontListBox* GetFontListBox() 
	{ 
		return DYNAMIC_DOWNCAST(COXFontListBox,GetListBoxCtrl()); 
	}

	// --- In  :
	// --- Out : 
	// --- Returns: Pointer to the currently selected font
	// --- Effect:	Retrieves a pointer to currently selected font
	CFont* GetSelectedFont();


	// --- In  :	pFont			-	pointer to CFont object that defines the item 
	//									that should be selected in the combo box
	//				pLF				-	pointer to LOGFONT structure that defines 
	//									the item that should be selected in the 
	//									combo box
	//				lpszFontName	-	name of the font that should be selected 
	//									in the combo box
	// --- Out : 
	// --- Returns:	The index of the selected item if the search was successful. 
	//				If the search was unsuccessful, the return value is LB_ERR and 
	//				the current selection is not changed
	// --- Effect:	Searches for a dropdown list-box item that matches the specified 
	//				font, and if a matching item is found, it selects the item
	int SelectFont(CFont* pFont);
	int SelectFont(LOGFONT* pLF);
	int SelectFont(LPCTSTR lpszFontName);


	// --- In  :	bSaveRestore	-	if TRUE then the order of items in MRU 
	//									section of the dropdown list box will be 
	//									saved in the registry before the control is 
	//									destroyed. 
	//									Refer to COXListBoxEx::SetSaveRestoreMRUState()
	//									function documentation for more details
	// --- Out : 
	// --- Returns: 
	// --- Effect:	Sets the flag that specifies whether MRU state of the dropdown
	//				list box control will be saved in the registry or not
	void SetSaveRestoreMRUState(BOOL bSaveRestore);

	// --- In  :	
	// --- Out : 
	// --- Returns: TRUE if the order of items in MRU section of the dropdown list box
	//				will be saved in the registry before the control is destroyed. 
	//				Refer to COXListBoxEx::GetSaveRestoreMRUState()	function 
	//				documentation for more details
	// --- Effect:	Retrieves the flag that specifies whether MRU state will be saved
	//				in the registry or not
	BOOL GetSaveRestoreMRUState();


	// --- In  :	pLBFI	-	pointer to OXLBFONTINFO structure that defines 
	//							the font that is about to be included in the 
	//							dropdown list box.
	//							This structure is declared as follows:
	//
	//				typedef struct _tagOXLBFONTINFO
	//				{
	//					LOGFONT m_lf;
	//					DWORD m_dwType;
	//					BOOL m_bIsSymbol;
	//				} OXLBFONTINFO;
	//
	//				where
	//
	//				m_lf		-	log font info
	//				m_dwType	-	font type. Might be combination of these:
	//								TRUETYPE_FONTTYPE		TrueType font
	//								DEVICE_FONTTYPE			device-supplied font
	//								RASTER_FONTTYPE			raster font
	//				m_bIsSymbol	-	flag that set to TRUE if the font displays 
	//								only symbols
	// --- Out : 
	// --- Returns:	TRUE if specified font is qualified to be added to the list box.
	//				Default implementation of this function always returns TRUE.
	// --- Effect:	Checks if font can be included in the list box. Advanced 
	//				overridable, can be overridden in a derived class in order to 
	//				filter the fonts that are included in the list box
	virtual BOOL FilterFont(OXLBFONTINFO* pLBFI) 
	{ 
		UNREFERENCED_PARAMETER(pLBFI);
		return TRUE; 
	}


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXFontComboBox)
	public:
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect:	Destructs the object
	virtual ~COXFontComboBox();

protected:
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
	// --- Returns:	TRUE if combo box was successfully initialized
	// --- Effect:	Initializes the internal state of the combo box. Called right
	//				after the control was created or subclassed
	virtual BOOL InitializeComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(COXFontComboBox)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSelEndOk();
	afx_msg BOOL OnSelChange();
	afx_msg BOOL OnCloseUp();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_OXFONTCOMBOBOX_H__)
