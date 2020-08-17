// ==========================================================================
//	Class Specification :     OXFontPickerButton
// ==========================================================================

// Header file : OXFontPickerButton.h

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

COXFontPickerButton is COXBitmapButton derived class that is designed to allow
a user to choose font and text color. This control uses standard CFontDialog
to choose font and text color and uses COXStatic control in order to display 
the chosen font and text color. And of course it allows you to use all power
of COXBitmapButton class such as displaying an image, specifying text font and 
color, tooltip and many more features available in this class. Also we provide
DDX routines that allows you to associate COLORREF and CFont variables with the 
control and set/retrieve data in standard manner (the details will be provided 
below).


  Usage

I.	COXFontPickerButton in dialog or form view
	
	1)	In resource editor put a button control in a dialog template. Make sure 
		that you specify ownerdrawn style for this button (COXBitmapButton 
		requirement)

	2)	Put a static control next to the button. This static control will be later 
		explicitly associated with the button and subclassed with COXStatic object. 
		This control will be used in order to display chosen font and text color

	3)	In OnInitDialog() for CDialog-derived implementation and in OnInitialUpdate()
		for CFormView-derived implementation you must call the following function
		that will associate the static control with font picker button:

			BOOL SetBuddy(UINT nBuddyWndID);

	4)	You might want to associate COLORREF and/or CFont variable with the font 
		picker button using the following DDX routines:

			void DDX_FontPickerFont(CDataExchange *pDX, int nIDC, CFont* pFont);
			void DDX_FontPickerColor(CDataExchange *pDX, int nIDC, COLORREF& clr);

		In order to do that you have to add corresponding variables to your CDialog
		or CFormView derived class:

			CFont m_font;
			COLORREF m_clrText;

		and update your DoDataExchange() routine by adding the following lines of code:

			DDX_FontPickerFont(pDX, IDC_BUTTON_FONT, &m_font);
			DDX_FontPickerColor(pDX, IDC_BUTTON_FONT, m_clrText);

		where IDC_BUTTON_FONT is presumed to be an ID of the font picker button.
		Using these DDX routines you can set/retrive font and text color by simply 
		calling UpdateData() function.


II.	COXFontPickerButton as a child control of any arbitrary window (explicit creation)
	
	1)	Create the COXFontPickerButton control explicitly using CButton::Create()
		function. Make sure that you specify BS_OWNERDRAW style for this button 
		(COXBitmapButton requirement) 

	2)	Create the COXStatic control explicitly using COXStatic::Create() function. 
		before calling this function you need to calculate the rectangle for this 
		control so it will be positioned nicely relatively to the COXFontPickerButton
		control

	3)	After both controls were successfully created you have to associate the 
		COXStatic control with the COXFontPickerButton control using:

			BOOL SetBuddy(COXStatic* pBuddy);

	4)	After accomplishing three previous steps you've got fully functionable font
		picker button control. User can click on the button and standard CFontDialog 
		will appear. The chosen font and text color can be retrieved using following 
		functions:

			CFont* GetBuddyFont() const;
			BOOL GetBuddyLogFont(LOGFONT* pLF) const;
			COLORREF GetBuddyTextColor() const;





The sample that demonstrates COXFontPickerButton class is called FontPickers
( it also shows the COXFontComboBox class that is related in terms of functionality)
and can be found in the .\Samples\gui\FontPickers subdirectory of your 
Ultimate Toolbox directory. 



Dependency:
	#include "OXFontPickerButton.h"


Source code files:
	"OXFontPickerButton.cpp"
	"OXBitmapButton.cpp"			COXBitmapButton implementation		
	"OXStatic.cpp"					COXStatic implementation		

Resource file:
	#include "OXBitmapButton.rc"	COXBitmapButton resource file



*/


#if !defined(_OXFONTPICKERBUTTON_H_)
#define _OXFONTPICKERBUTTON_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


#include "OXBitmapButton.h"
#include "OXStatic.h"

// The following functions were introduced in order to be specifically used in 
// DoDataExchange function of any Dialog or FormView based application for 
// Font Picker buttons. 
//

// --- In  :	pDX		-	A pointer to a CDataExchange object. The framework 
//							supplies this object to establish the context of the 
//							data exchange, including its direction.
//				nIDC	-	The resource ID of the subclassed font picker button 
//							associated with the control property
//				pFont	-	pointer to a CFont object that should be set to the control
//							or populated with control's chosen font
// --- Out : 
// --- Returns:
// --- Effect:	Manages the transfer of font data between a subclassed font picker
//				button in a dialog box, form view, or control view object and a 
//				data member of the dialog box, form view, or control view object
OX_API_DECL void AFXAPI DDX_FontPickerFont(CDataExchange *pDX, int nIDC, CFont* pFont);

// --- In  :	pDX		-	A pointer to a CDataExchange object. The framework 
//							supplies this object to establish the context of the 
//							data exchange, including its direction.
//				nIDC	-	The resource ID of the subclassed font picker button 
//							associated with the control property
//				pFont	-	reference to a COLORREF variable that should be used as
//							source of the text color that must be set to the subclassed
//							font picker button or will save the chosen text color
// --- Out : 
// --- Returns:
// --- Effect:	Manages the transfer of text color data between a subclassed font 
//				picker button in a dialog box, form view, or control view object and a 
//				data member of the dialog box, form view, or control view object
OX_API_DECL void AFXAPI DDX_FontPickerColor(CDataExchange *pDX, int nIDC, COLORREF& clr);

//
////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// COXFontPickerButton window

class OX_CLASS_DECL COXFontPickerButton : public COXBitmapButton
{
	DECLARE_DYNAMIC(COXFontPickerButton);
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect:	Constructs the object
	COXFontPickerButton();

// Attributes
public:

protected:
	// associated static control that diplays the name of the chosen font using
	// chosen text color
	COXStatic* m_pBuddy;

// Operations
public:
	// --- In  :	clrText	-	text color for associated static control
	//				bRedraw	-	if TRUE then the associated static control will be 
	//							redrawn immediately
	// --- Out : 
	// --- Returns:	TRUE if succeed; otherwise FALSE
	// --- Effect:	Sets text color for associated static control
	BOOL SetBuddyTextColor(COLORREF clrText, BOOL bRedraw=TRUE);

	// --- In  :	
	// --- Out : 
	// --- Returns:	Text color for associated static control
	// --- Effect:	Retrieves the text color for associated static control
	COLORREF GetBuddyTextColor() const;


	// --- In  :	pFont	-	font for associated static control
	//				bRedraw	-	if TRUE then the associated static control will be 
	//							redrawn immediately
	// --- Out : 
	// --- Returns:	TRUE if succeed; otherwise FALSE
	// --- Effect:	Sets the font for associated static control
	BOOL SetBuddyFont(CFont* pFont, BOOL bRedraw=TRUE);

	// --- In  :	
	// --- Out : 
	// --- Returns:	Pointer to the font for associated static control if succeed;
	//				otherwise NULL
	// --- Effect:	Retrieves the font for associated static control
	CFont* GetBuddyFont() const;

	// --- In  :	pLF		-	font LOGFONT structure for associated static control
	//				bRedraw	-	if TRUE then the associated static control will be 
	//							redrawn immediately
	// --- Out : 
	// --- Returns:	TRUE if succeed; otherwise FALSE
	// --- Effect:	Sets the font for associated static control using LOGFONT structure
	BOOL SetBuddyLogFont(LOGFONT* pLF, BOOL bRedraw=TRUE);

	// --- In  :	
	// --- Out :	pLF		-	font LOGFONT structure that will be filled with the 
	//							properties of the font
	// --- Returns:	TRUE if succeed; otherwise FALSE
	// --- Effect:	Retrieves the font for associated static control through 
	//				LOGFONT structure
	BOOL GetBuddyLogFont(LOGFONT* pLF) const;


	// --- In  :	pBuddy		-	a pointer to COXStatic object that will be 
	//								associated with the font picker button and 
	//								used to display the	chosen font name using 
	//								chosen text color
	//				nBuddyWndID	-	ID of the static control that will be subclassed 
	//								internally with COXStatic object and then 
	//								associated with the font picker button
	// --- Out : 
	// --- Returns:	TRUE if succeed; otherwise FALSE
	// --- Effect:	Associates a static control to be used to display chosen 
	//				font name using chosen text color
	BOOL SetBuddy(COXStatic* pBuddy);
	BOOL SetBuddy(UINT nBuddyWndID);

	// --- In  :	
	// --- Out : 
	// --- Returns:	a pointer to associated COXStatic control that is used to 
	//				display the	chosen font name using chosen text color
	// --- Effect:	Retrieves a pointer to associated static control
	inline COXStatic* GetBuddy() { return m_pBuddy; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXFontPickerButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect:	Destructs the object
	virtual ~COXFontPickerButton();


	// --- In  :	nPointSize	-	font height in points
	//				pDC			-	device context which settings are used for 
	//								units conversion, if NULL is specified then
	//								screen DC is being used
	// --- Out : 
	// --- Returns: Font height in logical units
	// --- Effect:	Converts font height unit from points to logical units
	static int ConvertPointToLogUnit(int nPointSize, CDC* pDC=NULL);

	// --- In  :	nLogUnitSize	-	font height in logical units
	//				pDC				-	device context which settings are used for 
	//									units conversion, if NULL is specified then
	//									screen DC is being used
	// --- Out : 
	// --- Returns: Font height in points
	// --- Effect:	Converts font height unit from logical units to points
	static int ConvertLogUnitToPoint(int nLogUnitSize, CDC* pDC=NULL);

	// Generated message map functions
protected:
	//{{AFX_MSG(COXFontPickerButton)
	//}}AFX_MSG
	virtual BOOL OnClicked();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(_OXFONTPICKERBUTTON_H_)
