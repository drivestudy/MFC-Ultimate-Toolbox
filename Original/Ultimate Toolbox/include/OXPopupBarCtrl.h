// ==========================================================================
// 					Class Specification : COXPopupBarCtrl
// ==========================================================================

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
//	YES	Derived from CWnd

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         

/*
We define Popup Bar control as any control that can be represented as top most 
popup window with items on it that can be chosen using mouse or keyboard. 
For example standard Popup Menu Bar or Color Picker from Word 97 are Popup Bar 
controls. Our Popup Bar can be graphically represented as:

  --------------------------------
  |  --------------------------  |
  |  |        Default         |  | <--		"Default" button. Always is located on the top. 
  |  --------------------------  |			Size of this button calculated automatically.
  | ---  ---  ---  ---  ---  --- |
  | | |  | |  | |  | |  | |  | | | <--  | 
  | ---  ---  ---  ---  ---  --- |		|
  | ---  ---  ---  ---  ---  --- |      |
  | | |  | |  | |  | |  | |  | | | <--  |
  | ---  ---  ---  ---  ---  --- |      |
  | ---  ---  ---  ---  ---  --- |      |
  | | |  | |  | |  | |  | |  | | | <--  |  <--	"Common" buttons. You can set the number
  | ---  ---  ---  ---  ---  --- |      |		of common buttons and its size. You can set 
  | ---  ---  ---  ---  ---  --- |      |		the number of rows used to display them. 
  | | |  | |  | |  | |  | |  | | | <--  |
  | ---  ---  ---  ---  ---  --- |      |
  | ---  ---  ---  ---  ---  --- |      |
  | | |  | |  | |  | |  | |  | | | <--  |
  | ---  ---  ---  ---  ---  --- |
  |  --------------------------  |
  |  |         Custom         |  | <--		"Custom" button. Always is located in the bottom
  |  --------------------------  |			Size of this button calculated automatically.
  --------------------------------

Default and Custom buttons are optional and at least one common button has to be assigned
to Popup Bar control.

After displaying, Popup Bar control capture all mouse messages and hide itself if any mouse 
button was clicked or user press ESC, ENTER or SPACE button. If at the moment of clicking 
mouse wasn't over any button within Popup Bar or user pressed ESC then nothing was chosen.

COXPopupBarCtrl is a generic class designed specifically as a base class to provide 
basic functionality of any Popup Bar control. 

Next are the main points about this class:

1)	With any button (except Custom button) we associate DWORD value to uniquely identify 
	them. In derived class you specify the logic of this value. It can be just a plain value
	or a pointer to any object, but it's up to implementor to provide logic that stands by 
	that value.

2)	COXPopupBarCtrl is created in two steps: constructor and Create function. In constructor 
	you can specify number of common buttons, number of rows to display common buttons, 
	common button size, the value that should be associated with Default button and the text 
	that is going to be displayed on Default and Custom buttons. Constructor is defined as:

		COXPopupBarCtrl(UINT nButtons=20, UINT nRows=4, 
			DWORD dwDefault=ID_POPUPBAR_DATA_NONE,
			CSize sizeButton=CSize(18,18),
			CString sDefaultButtonText=_T("Default"),
			CString sCustomButtonText=_T("Custom..."));

	In your derived class you can define your own predefined values to be used in the
	constructor.

	Create function physically create window. In create function you have to specify 
	parent window of the control and optionally you can set specific COXPopupBarCtrl
	styles (will be listed below) and brush object to draw control's window background.
	This function is defined as:

		BOOL Create(CWnd* pParentWnd, UINT nPopupBarStyle=0, HBRUSH hbrBackground=NULL);

		nPopupBarStyle		-	popup bar style. Can be any combination of:
	
								POPUPBAR_NOTOOLTIP	-	tooltip won't be displayed
								POPUPBAR_NODEFAULT	-	default button won't be displayed
								POPUPBAR_NOCUSTOM	-	custom button won't be displayed
								POPUPBAR_NOFLAT		-	reserved for future use
	
	After creation you can change whatever control's attributes you want (including those
	specified in constructor and Create function) using next functions:

		void SetNumButtons(UINT nButtons);
		// --- Effect : set number of common buttons in the control
		void SetNumRows(UINT nRows); 
		// --- Effect : set number of rows of common buttons in the control
		void SetPopupBarStyle(UINT nPopupBarStyle); 
		// --- Effect : set control specific styles
		void SetDefaultData(DWORD dwDefault);
		// --- Effect : set the data of default button
		void SetButtonSize(CSize sizeButton);
		// --- Effect : set common button size
		void SetDefaultButtonText(CString sText);
		// --- Effect : set default button text
		void SetCustomButtonText(CString sText);
		// --- Effect : set default button text
		BOOL SetTextFont(CFont* pFont);
		// --- Effect : set font to draw text in buttons


	And you can retrieve them using next functions:

		UINT GetNumButtons();
		// --- Returns:	number of common buttons in the control
		UINT GetNumRows();
		// --- Returns:	number of rows of common buttons in the control
		UINT GetNumColumns();
		// --- Returns:	number of columns of common buttons in the control
		UINT GetPopupBarStyle();
		// --- Returns:	control specific styles
		DWORD GetDefaultData();
		// --- Returns:	data associated with default button
		CSize GetButtonSize();
		// --- Returns:	common button size
		CString GetDefaultButtonText();
		// --- Returns:	default button text
		CString GetCustomButtonText();
		// --- Returns:	custom button text
		CFont* GetTextFont();
		// --- Returns:	pointer to the font used to draw text in buttons (if doesnt set 
		//				returns NULL)


3)	As I mentioned above any button (except Custom button) should be associated with DWORD
	value in order to be uniquely identified. Next function is provided to populate control 
	with common button values (Default button value is set using another function 
	SetDefaultData):

		BOOL SetButtonDataTable(DWORD* arrData, UINT nElements);
		// --- In  :	arrData		-	array of DWORD elements
		//				nElements	-	the number of elements in the array
		BOOL SetButtonDataTable(ButtonDataTable& arrData);
		// --- In  :	arrData		-	CArray of DWORD elements. ButtonDataTable is defined as:
		//									typedef CArray<DWORD, DWORD> ButtonDataTable;

	Values associated with Common buttons can be retrieved using next function:

		ButtonDataTable* GetButtonDataTable();


	As long as we associate unique values with any custom button we can use them to provide
	tooltips for all common buttons. We provide next function to set tooltips:

		BOOL SetToolTipTextTable(ButtonToolTipMap& arrToolTipText);
		// --- In  :	arrToolTipText -	ButtonToolTipMap map array. ButtonToolTipMap is 
		//									defined as:
		//
		//					typedef CMap<DWORD, DWORD, CString, CString&> ButtonToolTipMap;
		//
		BOOL SetToolTipTextTable(ButtonToolTipTable* arrToolTipText, UINT nElements);
		// --- In  :	arrToolTipText	-	array of ButtonToolTipTable structure elements.
		//									This structure is defined as:
		//
		//									typedef struct {
		//										DWORD dwData; 
		//										LPCTSTR pszToolTipText;
		//									} ButtonToolTipTable;
		//
		//									where:
		//
		//									dwData			-	value associated with button
		//									pszToolTipText	-	tooltip text
		//
		//				nElements		-	the number of elements in the array


	Tooltips can be retrieved using:

		ButtonToolTipMap* GetToolTipTextTable();


4)	The ultimate goal of using Popup Bar control is to provide the way user can pick on
	of displayed buttons (items). We call picked item as selected and you can get and 
	programmatically set selected item using next function:

		void SetSelectedData(DWORD dwSelected);
		// --- Effect : set selected button using data associated with it
		DWORD GetSelectedData();
		// --- Returns:	data associated with selected button


5)	Popup Bar control reminds modal dialog by functionality so we decided to provide 
	function similar to CDialog::DoModal() function to display the control:

		virtual BOOL Pick(UINT nAlignment=ID_POPUPBAR_ALIGNBOTTOM, CRect* pParentRect=NULL, 
			CSize sizeOffset=CSize(0,0));
		// --- In  :	nAlignment	-	popup bar control alignment relatively to pParentRect
		//								rectangle if it's not NULL or to parent window rectangle
		//								otherwise. Can be one of:
		//
		//							ID_POPUPBAR_ALIGNLEFT	-	control's window RightTop will 
		//														be displayed at the LeftBottom 
		//														point of relative rectangle
		//							ID_POPUPBAR_ALIGNRIGHT	-	control's window LeftTop will 
		//														be displayed at the RightTop 
		//														point of relative rectangle
		//							ID_POPUPBAR_ALIGNTOP	-	control's window LeftBottom will 
		//														be displayed at the LeftTop 
		//														point of relative rectangle
		//							ID_POPUPBAR_ALIGNBOTTOM	-	control's window LeftTop will be 
		//														displayed at the LeftBottom 
		//														point of relative rectangle
		//
		//				pParentRect	-	if it's not NULL then it will be used as relative 
		//								rectangle to position popup bar
		//				sizeOffset	-	size of popup bar window position offset
		// --- Returns:	TRUE if user selects any button, or FALSE otherwise
		// --- Effect : Displays popup bar control. To get data of selected button call 
		//				GetSelectedData function


	And here is an example of displaying of any Popup Bar control:

		COXPopupBarCtrl Picker;
		if(Picker.Create(this))
		{
			if(Picker.Pick(ID_POPUPBAR_ALIGNBOTTOM))
			{
				DWORD nData=Picker.GetSelectedData();
			}
		}


6)	In order to provide graphical representation of buttons in your derived class
	you'd probably need to overwrite next virtual functions:

		virtual void DrawButton(CDC* pDC, UINT nIndex);
		virtual void DrawDefaultButton(CDC* pDC);
		virtual void DrawCustomButton(CDC* pDC);
		virtual void DrawFrameRect(CDC* pDC, CRect& rect, UINT nIndex);

		// pDC		-	device context to draw in
		// nIndex	-	index of button	to be drawn


	Wnen Custom button is pressed next virtual function is called:

		virtual BOOL OnCustomButton();

	Overwrite it too in order to provide your own handling of custom button in derived 
	class.

7)	Next helper functions are defined:

		BOOL IsToolTip();
		// --- Returns:	TRUE if tooltips will be displayed or FALSE otherwise
		BOOL IsDefaultButton();
		// --- Returns:	TRUE if default button will be displayed or FALSE otherwise
		BOOL IsCustomButton();
		// --- Returns:	TRUE if custom button will be displayed or FALSE otherwise


8)	As long as COXPopupBarCtrl is a generic class and you are hardly going to use it 
	in you applications directly it's good idea to take look at the PopupBar sample
	that resides in .\Samples\gui\PopupBar subdirectory of your Ultimate Toolbox 
	directory that provides an example of a number of COXPopupBarCtrl derived controls.

	And, of course, I should mention that our Color Picker Control (COXColorPickerCtrl 
	class) is derived from COXPopupBarCtrl.

*/

#if !defined(_OXPOPUPBARCTRL_H_)
#define _OXPOPUPBARCTRL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#define __AFXTEMPL_H__
#endif


// OXPopupBarCtrl.h : header file
//



// timer for checking mouse position
#define IDT_OXPB_CHECKMOUSE			243
#define ID_OXPB_CHECKMOUSE_DELAY	200


// popup bar styles
//
// tooltip won't be displayed
#define POPUPBAR_NOTOOLTIP			0x00000001
// default button won't be displayed
#define POPUPBAR_NODEFAULT			0x00000002
// custom button won't be displayed
#define POPUPBAR_NOCUSTOM			0x00000004
// reserved for future 
#define POPUPBAR_NOFLAT				0x00000008
// popup bar cannot be "torn-off"
#define POPUPBAR_NOTEAROFF			0x00000010


// special data value specifies that none was chosen
#define ID_POPUPBAR_DATA_NONE		0xffffffff

// special index specifies that none was chosen
#define ID_POPUPBAR_INDEX_NONE		0xffffffff

// max number of buttons in the control
#define ID_POPUPBAR_MAX_BUTTONS		256


// control's alignment
//
// control's window RightTop will be displayed at the LeftTop point of parent window 
// or rect specified in COXPopupBarCtrl::Pick function
#define ID_POPUPBAR_ALIGNLEFT				0x00000000		
// control's window LeftTop will be displayed at the RightTop point of parent window 
// or rect specified in COXPopupBarCtrl::Pick function
#define ID_POPUPBAR_ALIGNRIGHT				0x00000001		
// control's window LeftBottom will be displayed at the LeftTop point of parent window 
// or rect specified in COXPopupBarCtrl::Pick function
#define ID_POPUPBAR_ALIGNTOP				0x00000002
// control's window LeftTop will be displayed at the LeftBottom point of parent window 
// or rect specified in COXPopupBarCtrl::Pick function
#define ID_POPUPBAR_ALIGNBOTTOM				0x00000003
// control's window RightBottom will be displayed at the LeftBottom point of parent window 
// or rect specified in COXPopupBarCtrl::Pick function
#define ID_POPUPBAR_ALIGNLEFTBOTTOM			0x00000004		
// control's window LeftBottom will be displayed at the RightBottom point of parent window 
// or rect specified in COXPopupBarCtrl::Pick function
#define ID_POPUPBAR_ALIGNRIGHTBOTTOM		0x00000005		
// control's window RightBottom will be displayed at the RightTop point of parent window 
// or rect specified in COXPopupBarCtrl::Pick function
#define ID_POPUPBAR_ALIGNTOPRIGHT			0x00000006
// control's window RightTop will be displayed at the RightBottom point of parent window 
// or rect specified in COXPopupBarCtrl::Pick function
#define ID_POPUPBAR_ALIGNBOTTOMRIGHT		0x00000007


// structure used to provide tooltips for buttons where
//		dwData			-	value associated with button
//		nID				-	tooltip text resource ID
//
typedef struct {
	DWORD dwData; 
	int nID;
} ButtonToolTipIDTable;


// structure used to provide tooltips for buttons where
//		dwData			-	value associated with button
//		pszToolTipText	-	tooltip text
//
typedef struct {
	DWORD dwData; 
	LPCTSTR pszToolTipText;
} ButtonToolTipTable;


// map array for tooltips
typedef CMap<DWORD, DWORD, CString, CString&> ButtonToolTipMap;
// array for data associated with buttons
typedef CArray<DWORD, DWORD> ButtonDataTable;
// array for buttons rectangles
typedef CArray<CRect, CRect&> ButtonRectTable;

/////////////////////////////////////////////////////////////////////////////
// COXPopupBarCtrl window

class OX_CLASS_DECL COXPopupBarCtrl : public CWnd
{
// Construction
public:
	// --- In  :	nButtons			-	number of buttons in the popup bar (doesn't 
	//										include	default and custom buttons)
	//				nRows				-	number of button rows (cannot be more than the 
	//										number of buttons)
	//				dwDefault			-	value to be associated with default button
	//				sizeButton			-	size of buttons (not applicable for default and 
	//										custom buttons their size are calculated 
	//										automatically and depend on the size of 
	//										Popup Bar window)
	//				sDefaultButtonText	-	text to be drawn on default button
	//				sCustomButtonText	-	text to be drawn on custom button
	//				nIDdefault			-	text resource ID to be drawn on 
	//										default button
	//				nIDcustom			-	text resource ID to be drawn on 
	//										custom button
	//				
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXPopupBarCtrl(UINT nButtons, UINT nRows,  DWORD dwDefault, 
		CSize sizeButton, CString sDefaultButtonText, CString sCustomButtonText);
	COXPopupBarCtrl(UINT nButtons=20, UINT nRows=4, 
		DWORD dwDefault=ID_POPUPBAR_DATA_NONE, CSize sizeButton=CSize(18,18),
		int nIDdefault=IDS_OX_BROWSECLRDEFAULT,	int nIDcustom=IDS_OX_BROWSECLRCUSTOM);

// Data
protected:
	// array of data of buttons to be displayed
	ButtonDataTable m_arrData;
	// the number of common buttons shown in the control
	UINT m_nButtons;
	// the number of lines to use to display common buttons
	UINT m_nRows;

	// style of popup bar control
	// that defines control's appearance;
	UINT m_nPopupBarStyle;

	// array of tooltips
	// you can provide your own array of tooltips
	ButtonToolTipMap m_arrToolTipText;
	// control to display tooltips
	CToolTipCtrl m_ctlToolTip;

	// data of initially selected button
	DWORD m_dwSelected;
	// data associated with default button
	DWORD m_dwDefault;

	// size of common buttons
	CSize m_sizeButton;

	// text displayed on default button
	CString m_sDefaultButtonText;
	// text displayed on custom button
	CString m_sCustomButtonText;
	// Tooltip text displayed when mouse over tearoff bar
    CString m_sTearoffText;
    // Caption when the control is floating
    CString m_sCaption;

	// font used to draw text on buttons
	CFont m_font;

	// parent window
	CWnd* m_pParentWnd;

	// palette for 256-color displays
	CPalette m_CustomPalette;

	// last window message to be routed
	MSG m_lastMsg;

	// timer used to check the mouse position
	UINT_PTR m_nCheckMouseTimer;
	// flag that specified if current index should be updated during 
	// checking mouse position
	BOOL m_bCheckMousePos;
	// coordinate of the last mouse position
	CPoint m_ptLastMousePos;

	// tracking bar, used when converting to floating state
	//
	CDC* m_pTrackingDC;
	// coordinate of the moving track bar
	CPoint m_ptLastTrack;
	// coordinate of the tracking rectangle
	CRect m_rectTrackBar;
	// last saved coordinate of the tracking rectangle
	CRect m_rectLastTrackBar;
	// last used thickness of the tracking rectangle
	CSize m_sizeLastTrackBar;
	// flag that specifies that last time when tracking bar was drawn 
	// the dithered brush has been used
	BOOL m_bDitherLast;
	//
	/////////////////////////////////////////////////

//////////////////////////////////////////////
// data for inner use

	// Messages sent to popup bar window
	// 
	// hide popup bar control and return TRUE from Pick function
	static UINT m_nOkMessage;
	// hide popup bar control and return FALSE from Pick function
	static UINT m_nCancelMessage;
	// hide popup bar control and call virtual OnCustomButton function.
	// Result of OnCustomButton function is used as a result of Pick function
	static UINT m_nCustomButtonPicked;
    // convert popup bar control to floating
    static UINT m_nTearoffMessage;

	// different margins we use to draw buttons on the control
	//
	// margin for window's edge
	CRect m_rectEdgeMargin;
	// margin for Common Buttons
	CRect m_rectButtonMargin;
	// margin for Default Button
	CRect m_rectDefaultButtonMargin;
	// margin for Custom Button
	CRect m_rectCustomButtonMargin;
    // margin for Tear off Bar
    CRect m_rectTearoffBarMargin;


	// selected button (non zero based index)
	// if m_nSelectedIndex=0 then default is selected
	// if m_nSelectedIndex=m_nButtons+1 then Custom is selected
	UINT m_nSelectedIndex;
	// current index (mouse is over or user moved to using keyboard)
	UINT m_nCurrentIndex;

	// window rect to place control (screen coordinates)
	CRect m_rect;
	// size of button to display Default button (client coordinates)
	CRect m_rectDefaultButton;
	// size of button to display Custom button (client coordinates)
	CRect m_rectCustomButton;
    // size of tear off bar (client coordinates)
    CRect m_rectTearoffBar;

	// to make thing really easy we introduce array of rects for all 
	// Common Buttons on the control (client coordinates)
	ButtonRectTable m_arrButtonRect;


	// if true - mouse button (left or right) is pressed
	BOOL m_bMouseButtonPressed;
    // if true - mouse button was clicked in tear off bar
    BOOL m_bClickedInTearoffBar;
    // if true - menu has been torn off and is floating
    BOOL m_bFloating;

	// m_nState = 0 - control is active
	// m_nState = 1 - user selected some color
	// m_nState = 2 - user left control without selecting any color
	// m_nState = 3 - custom button was pressed
	UINT m_nState;

// Attributes
public:

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if tooltips will be displayed or FALSE otherwise
	// --- Effect : 
	inline BOOL IsToolTip() const { 
		return !(m_nPopupBarStyle & POPUPBAR_NOTOOLTIP); 
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if default button will be displayed or FALSE otherwise
	// --- Effect : 
	inline BOOL IsDefaultButton() const { 
		return !(m_nPopupBarStyle & POPUPBAR_NODEFAULT); 
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if custom button will be displayed or FALSE otherwise
	// --- Effect : 
	inline BOOL IsCustomButton() const { 
		return !(m_nPopupBarStyle & POPUPBAR_NOCUSTOM); 
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	reserved for future use
	// --- Effect : 
	inline BOOL IsFlat() const { return !(m_nPopupBarStyle & POPUPBAR_NOFLAT); }

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if popup bar can be torn-off or FALSE otherwise
	// --- Effect : 
	inline BOOL IsTearOff() const { 
		return !(m_nPopupBarStyle&POPUPBAR_NOTEAROFF) && !m_bFloating; }

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if popup bar is floating
	// --- Effect : 
	inline BOOL IsFloating() const { return m_bFloating; }

// Operations
public:

	// --- In  :	pParentWnd			-	pointer to the parent window. 
	//				nPopupBarStyle		-	popup bar style. Can be any combination of:
	//								POPUPBAR_NOTOOLTIP	-	tooltip won't be 
	//														displayed
	//								POPUPBAR_NODEFAULT	-	default button won't 
	//														be displayed
	//								POPUPBAR_NOCUSTOM	-	custom button won't 
	//														be displayed
	//								POPUPBAR_NOFLAT		-	reserved for future use
    //								POPUPBAR_NOTEAROFF  -   "Tear-off" is not available
	// --- Out : 
	// --- Returns:	reserved TRUE if succeed or FALSE otherwise
	// --- Effect : Creates popup bar control. After creation you can change whatever 
	//				control's attributes you want. To display control call Pick function
	BOOL Create(CWnd* pParentWnd, UINT nPopupBarStyle=POPUPBAR_NOTEAROFF, 
		HBRUSH hbrBackground=NULL);


	// --- In  :	nAlignment	-	popup bar control alignment relatively to pParentRect
	//								rectangle if it's not NULL or to parent window rectangle
	//								otherwise. Can be one of:
	//
	//				ID_POPUPBAR_ALIGNLEFT		-	control's window RightTop will 
	//												be displayed at the LeftBottom 
	//												point of relative rectangle
	//				ID_POPUPBAR_ALIGNRIGHT		-	control's window LeftTop will 
	//												be displayed at the RightTop 
	//												point of relative rectangle
	//				ID_POPUPBAR_ALIGNTOP		-	control's window LeftBottom will 
	//												be displayed at the LeftTop 
	//												point of relative rectangle
	//				ID_POPUPBAR_ALIGNBOTTOM		-	control's window LeftTop will be 
	//												displayed at the LeftBottom 
	//												point of relative rectangle
	//				ID_POPUPBAR_ALIGNLEFTBOTTOM	-	control's window 
	//												RightBottom will be 
	//												displayed at the LeftBottom 
	//												point of relative rectangle
	//				ID_POPUPBAR_ALIGNRIGHTBOTTOM-	control's window 
	//												LeftBottom will 
	//												be displayed at the RightBottom 
	//												point of relative rectangle
	//				ID_POPUPBAR_ALIGNTOPRIGHT	-	control's window RightBottom 
	//												will be displayed at the RightTop 
	//												point of relative rectangle
	//				ID_POPUPBAR_ALIGNBOTTOMRIGHT-	control's window RightTop 
	//												will be displayed at the 
	//												RightBottom point of 
	//												relative rectangle
	//
	//				pParentRect	-	if it's not NULL then it will be used as relative 
	//								rectangle to position popup bar
	//				sizeOffset	-	size of popup bar window position offset
	// --- Out : 
	// --- Returns:	TRUE if user selects any button, or FALSE otherwise
	// --- Effect : Displays popup bar control. To get data of selected button call 
	//				GetSelectedData function
	virtual BOOL Pick(UINT nAlignment=ID_POPUPBAR_ALIGNBOTTOM, 
		CRect* pParentRect=NULL, CSize sizeOffset=CSize(0,0));


	// --- In  :	arrData		-	array of DWORD elements
	//				nElements	-	the number of elements in the array
	// --- Out : 
	// --- Returns:	BOOL if succeed or FALSE otherwise
	// --- Effect : sets array of common buttons data
	BOOL SetButtonDataTable(DWORD* arrData, UINT nElements);

	// --- In  :	arrData		-	CArray of DWORD elements. 
	//								ButtonDataTable is defined as:
	//
	//					typedef CArray<DWORD, DWORD> ButtonDataTable;
	//
	// --- Out : 
	// --- Returns:	BOOL if succeed or FALSE otherwise
	// --- Effect : set array of common buttons data
	BOOL SetButtonDataTable(ButtonDataTable& arrData);

	// --- In  :	
	// --- Out : 
	// --- Returns:	pointer to array of common buttons data
	// --- Effect : get array of common buttons data
	inline ButtonDataTable* GetButtonDataTable() { return &m_arrData; }


	// --- In  :	nButtons	-	number of buttons. It cannot be more than
	//								ID_POPUPBAR_MAX_BUTTONS
	// --- Out : 
	// --- Returns:	
	// --- Effect : set number of common buttons in the control
	inline void SetNumButtons(UINT nButtons) 
	{ 
		if(IsFloating())
			SendNotification(m_nCancelMessage);

		ASSERT(nButtons<=ID_POPUPBAR_MAX_BUTTONS && nButtons>0); 
		m_nButtons=nButtons; 
	}

	
	// --- In  :	
	// --- Out : 
	// --- Returns:	number of common buttons in the control
	// --- Effect : 
	inline UINT GetNumButtons() const { return m_nButtons; }

	// --- In  :	nRows	-	number of rows. It cannot be more than 
	//							ID_POPUPBAR_MAX_BUTTONS
	// --- Out : 
	// --- Returns:	
	// --- Effect : set number of rows of common buttons in the control
	inline void SetNumRows(UINT nRows) 
	{ 
		if(IsFloating())
			SendNotification(m_nCancelMessage);

		ASSERT(nRows<=ID_POPUPBAR_MAX_BUTTONS && nRows>0); 
		m_nRows=nRows; 
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	number of rows of common buttons in the control
	// --- Effect : 
	inline UINT GetNumRows() const { return m_nRows; }
	
	// --- In  :	
	// --- Out : 
	// --- Returns:	number of columns of common buttons in the control
	// --- Effect : 
	UINT GetNumColumns() const;

	
	// --- In  :	arrToolTipText -	ButtonToolTipMap map array. 
	//									ButtonToolTipMap is defined as:
	//
	//				typedef CMap<DWORD, DWORD, CString, CString&> ButtonToolTipMap;
	//
	// --- Out : 
	// --- Returns:	BOOL if succeed or FALSE otherwise
	// --- Effect : set array of tooltips associated with common buttons
	BOOL SetToolTipTextTable(ButtonToolTipMap& arrToolTipText);

	// --- In  :	arrToolTipText	-	array of ButtonToolTipTable structure elements.
	//									This structure is defined as:
	//
	//									typedef struct {
	//										DWORD dwData; 
	//										LPCTSTR pszToolTipText;
	//									} ButtonToolTipTable;
	//
	//									where:
	//
	//									dwData			-	value associated with button
	//									pszToolTipText	-	tooltip text
	//
	//				nElements		-	the number of elements in the array
	// --- Out : 
	// --- Returns:	BOOL if succeed or FALSE otherwise
	// --- Effect : set array of tooltips associated with common buttons
	BOOL SetToolTipTextTable(ButtonToolTipTable* arrToolTipText, UINT nElements);

	// --- In  :	arrToolTipText	-	array of ButtonToolTipIDTable structure 
	//									elements. This structure is defined as:
	//
	//									typedef struct {
	//										DWORD dwData; 
	//										int nID;
	//									} ButtonToolTipTable;
	//
	//									where:
	//
	//									dwData		-	value associated with button
	//									nID			-	tooltip text resource ID
	//
	//				nElements		-	the number of elements in the array
	// --- Out : 
	// --- Returns:	BOOL if succeed or FALSE otherwise
	// --- Effect : set array of tooltips associated with common buttons
	BOOL SetToolTipTextIDTable(ButtonToolTipIDTable* arrToolTipText, UINT nElements);

	// --- In  :	
	// --- Out : 
	// --- Returns:	pointer to ButtonToolTipMap of tooltips
	// --- Effect : get array of tooltips associated with common buttons
	inline ButtonToolTipMap* GetToolTipTextTable() { return &m_arrToolTipText; }


	// --- In  :	nPopupBarStyle		-	popup bar style. 
	//										Can be any combination of:
	//
	//								POPUPBAR_NOTOOLTIP	-	tooltip won't be 
	//														displayed
	//								POPUPBAR_NODEFAULT	-	default button won't 
	//														be displayed
	//								POPUPBAR_NOCUSTOM	-	custom button won't 
	//														be displayed
	//								POPUPBAR_NOFLAT		-	reserved for future use
    //								POPUPBAR_NOTEAROFF  -   "Tear-off" is not available
	// --- Out : 
	// --- Returns:	
	// --- Effect : set control specific styles
	inline void SetPopupBarStyle(UINT nPopupBarStyle) { 
		m_nPopupBarStyle=nPopupBarStyle; 
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	control specific styles
	// --- Effect : 
	inline UINT GetPopupBarStyle() const { return m_nPopupBarStyle; }


	
	// --- In  :	dwSelected	-	value associated with button
	// --- Out : 
	// --- Returns:	
	// --- Effect : set selected button using data associated with it
	inline void SetSelectedData(DWORD dwSelected) { m_dwSelected=dwSelected; }

	// --- In  :
	// --- Out : 
	// --- Returns:	data associated with selected button
	// --- Effect : 
	inline DWORD GetSelectedData() const { return m_dwSelected; }

	// --- In  :	dwDefault	-	value associated with button
	// --- Out : 
	// --- Returns:	
	// --- Effect : set the data of default button
	inline void SetDefaultData(DWORD dwDefault) { m_dwDefault=dwDefault; }

	// --- In  :
	// --- Out : 
	// --- Returns:	data associated with default button
	// --- Effect : 
	inline DWORD GetDefaultData() const { return m_dwDefault; }


	// --- In  :	sizeButton	-	common button size
	// --- Out : 
	// --- Returns:	
	// --- Effect : set common button size
	inline void SetButtonSize(CSize sizeButton) { m_sizeButton=sizeButton; }

	// --- In  :	
	// --- Out : 
	// --- Returns:	common button size
	// --- Effect : 
	inline CSize GetButtonSize() const { return m_sizeButton; }
	

	// --- In  :	sText	-	text to be displayed in default button
	// --- Out : 
	// --- Returns:	
	// --- Effect : set default button text
	inline void SetDefaultButtonText(const CString& sText) { 
		m_sDefaultButtonText=sText; 
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	default button text
	// --- Effect : 
	inline CString GetDefaultButtonText() const { return m_sDefaultButtonText; }

	// --- In  :	sText	-	text to be displayed in custom button
	// --- Out : 
	// --- Returns:	
	// --- Effect : set default button text
	inline void SetCustomButtonText(const CString& sText) { 
		m_sCustomButtonText=sText; 
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	custom button text
	// --- Effect : 
	inline CString GetCustomButtonText() const { return m_sCustomButtonText; }
	

	// --- In  :	sText	-	text to be displayed in caption when popup bar 
	//							is floating
	// --- Out : 
	// --- Returns:	
	// --- Effect : sets caption text
    inline void SetCaptionText(CString sText) { m_sCaption=sText; }

	// --- In  :	
	// --- Out : 
	// --- Returns:	text displayed in caption when a popup bar is floating
	// --- Effect : 
	inline CString GetCaptionText() const { return m_sCaption; }


	// --- In  :	sText	-	tooltip text displayed when mouse over tearoff bar
	// --- Out : 
	// --- Returns:	
	// --- Effect : sets caption text
    inline void SetTearoffText(CString sText) { m_sTearoffText=sText; }

	// --- In  :	
	// --- Out : 
	// --- Returns:	tooltip text displayed when mouse over tearoff bar
	// --- Effect : 
	inline CString GetTearoffText() const { return m_sTearoffText; }


	// --- In  :	pFont	-	pointer to CFont object
	// --- Out : 
	// --- Returns:	
	// --- Effect : set font to draw text in buttons
	BOOL SetTextFont(CFont* pFont);

	// --- In  :	
	// --- Out : 
	// --- Returns:	pointer to the font used to draw text in buttons (if doesnt set 
	//				returns NULL)
	// --- Effect : 
	inline CFont* GetTextFont() { return (HFONT)m_font==NULL ? NULL : &m_font; }

	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXPopupBarCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
public:
	virtual ~COXPopupBarCtrl();

// implementation
protected:
	// calculate size of window and location of default and 
	// custom buttons within the control
	BOOL CalcWindowSize();
	// user for sure want to control displaying of the control 
	BOOL AdjustWindowPosition(UINT nAlignment=ID_POPUPBAR_ALIGNBOTTOM, 
		CRect* pParentRect=NULL, CSize sizeMargin=CSize(0,0));
	// find index of button in the control
	// 0 - default button
	// 1 ... m_nButtons - common button(s)
	// m_nButtons+1 - custom button
	UINT GetIndexFromData(DWORD dwData);
	UINT GetIndexFromPoint(CPoint point);
	// returns index of the first button on the control
	UINT GetFirstButtonIndex();
	// returns index of the last button on the control
	UINT GetLastButtonIndex();

	void RedrawElement(UINT nIndex);


	// --- In  :	
	// --- Out : 
	// --- Returns:	BOOL if succeed or FALSE otherwise
	// --- Effect : physically set tooltips to every button on the control including 
	//				default and custom buttons
	BOOL PopulateToolTip();


    // converts the popup bar ctrl to a tear off bar control
    BOOL ConvertToTearOff();

    // Sends a message to either this window (not floating), 
	// or the parent (if floating)
    void SendNotification(UINT nMessage);

	// functions that implement tracking functionality
	void MoveTrackBar(CPoint pt);
	BOOL TrackBar();
	void EndTrackBar();
	void CancelTrackBar();
	void DrawTrackingRect(BOOL bRemoveRect);
	//////////////////////////////////////////////

	// overwrite next functions in your derived class to provide your 
	// own drawing routines
	//
	// pDC		-	device context to draw in
	// nIndex	-	index of button	to be drawn
	//
	virtual void DrawBackground(CDC* pDC);
	virtual void DrawButton(CDC* pDC, UINT nIndex);
    virtual void DrawTearoffBar(CDC* pDC);
	virtual void DrawDefaultButton(CDC* pDC);
	virtual void DrawCustomButton(CDC* pDC);
	virtual void DrawFrameRect(CDC* pDC, CRect& rect, UINT nIndex);
	//////////////////////////////////////////////////////////////////

	virtual void OnMouseButtonDown(CPoint point);
	virtual void OnMouseButtonUp(CPoint point);

	// overwrite next function to provide your own handling of custom button in derived class
	virtual BOOL OnCustomButton();

	// called every time current index being changed
	virtual void OnChangeCurrentIndex(UINT newCurrentIndex);
	// called every time selected index being changed
	virtual void OnChangeSelectedIndex(UINT newSelectedIndex);

	// Generated message map functions
protected:
	//{{AFX_MSG(COXPopupBarCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnQueryNewPalette();
    afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnFloatStatus(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpcs);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(_OXPOPUPBARCTRL_H_)
