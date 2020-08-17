// ==========================================================================
// 					Class Specification : 
//			COXCoolToolBar & COXCoolToolBarCmdUI
// ==========================================================================

// Header file : OXCoolToolBar.h

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
//	YES	Derived from CToolBar / CCmdUI

//	YES / NO	Is a Cwnd. 
//	YES / NO	Two stage creation 
//	YES / NO	Has a message map

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//
// COXCoolToolBarCmdUI   - CCmdUI derived class.
//
// COXCoolToolBar - New toolbar that utilizes new functionality provided by new
//					comctl32.dll (supplied with IE 3.0 and now redistibutable.
//					You have to use MFC >=4.2). 
//					New features:
//						1)	flat mode;
//						2)	cool mode (can be used in CoolBars);
//						3)	in flat mode draws gripper and separators (Office'97 look);
//						4)	list mode (looks like Links toolbar in IE 3.0);
//						5)	set different image lists for default, disabled and hot mode
//							(to get the effect like in IE 3.0);
//						6)	set text color for default and hot mode;
//						7)	set of functions to set text to buttons in CoolToolBar 
//							(you can set MinMaxWidth of text in row and 
//							the number of text rows in buttons, 
//							you can automatically set text to buttons
//							using string or menu resources);
//						8)	change CoolToolBar style at run-time;
//						9)	custom toolbar buttons are fully supported (e.g combo box
//							or slider toolbar buttons)
//						10)	extensive customization support implemented through our 
//							Customization Manager Framework 
//						11) Drag and Drop for moving, copying and deleting buttons				
//						12) persistence - you can save and load state of CoolToolBar
//							buttons

//
//
//					To utilize this class you have to do (in case with standard project
//					created using MFC App Wizard):
//					(*)	1)	add #include "OXCoolToolBar.h" to your "MainFrm.h" 
//							header file;
//
//					(*)	2)	change CToolBar on COXCoolToolBar;
//
//						after creation of CoolToolBar:
//						3)	use IniSizes(CSize& sizeImage) if you use 
//							non-standard size of images for buttons
//
//						4)	CoolToolBar created with flat style set by default, 
//							if you don't want this feature call SetFlat(FALSE);
//
//						5)	if you are going to use CoolToolBar within CoolBar then 
//							call SetCool(TRUE);
//
//						6)	if you want you CoolToolBar to be customizable:
//							a)  use Customize Manager Framework;
//							b)	use SetBitmapIds(UINT* pIds, int nButtons) function 
//								to set an array of button Ids to use in Customize 
//								dialog andin save and load state functions (if you 
//								don't call this functions your CoolToolBar 
//								uncustomizable by default).	It is up to you to 
//								create and delete (if needed) array of buttons ID;
//
//						7)	if you want to set text to buttons then you have to set 
//							max width of buttons and max number of text rows 
//							in a button (if you use comctl32.dll v 4.0 and higher
//							you might use TBSTYLE_AUTOSIZE style. In this case the 
//							buttons will be automatically sized in order to fit the 
//							size of text and image);
//
//						8)	if needed call functions to set images to show in 
//							default, disabled and hot mode:
//
//								HIMAGELIST SetDefaultImageList(HIMAGELIST hImageList);
//								HIMAGELIST SetDisabledImageList(HIMAGELIST hImageList);
//								HIMAGELIST SetHotImageList(HIMAGELIST hImageList);
//
//							If you reside Image Lists in resource file 
//							it is recommended to use to create Image Lists
//							COXCoolToolBar-inner function: 
//							HIMAGELIST AddImageList(UINT nImageListID, int cx=16, 
//								int cGrow=0, COLORREF crMask=RGB(255,0,255));
//							In that case you shouldn't care about deleting used 
//							Image Lists, COXCoolToolBar destructor 
//							will do all job for you
//
//						9)	if needed call functions to set Text Color in 
//							default and/or hot mode:
//
//								void SetDefaultTextColor(COLORREF crTextColor);
//								void SetHotTextColor(COLORREF crTextColor);
//
//						10)	if needed set indent to CoolToolBar
//
//					(*)	11)	call UpdateSizes() to correctly set sizes of buttons
//
//			(*) - marked steps that have to be taken to use CoolToolBar;
//			steps from 4 to 10 can be taken in any order
//							
//
// Prerequisites (necessary conditions):
//					To use this class, you must include the
//					header files:
//						 <afxcmn.h>
//						 <afxext.h>
//						 <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////

#ifndef _COOLTOOLBAR_H
#define _COOLTOOLBAR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"
#include "UTB64Bit.h"


// OXCoolToolBar.h : header file
//

// there are some changes in new common control dll version 4.71
// which is supplied with IE 4.0. We use constant defined in the 
// new dll - _WIN32_IE that define the version of using Internet Explorer 
// (it is set to 0x04000). Actually we can use this constant to define 
// version of using common control dll
//
//	_WIN32_IE		comctl32.dll version	
//	  0x4000				4.71, 4.72		IE 4.0-4.01
//	  0x3000				4.70			IE 3.0
//	  0x2000				4.00
//
#ifndef _WIN32_IE
#if _MFC_VER>=0x0420
#define _WIN32_IE 0x0300
#else
#define _WIN32_IE 0x0200
#endif
#endif

#ifndef __AFXCMN_H__
#include <afxcmn.h>
#define __AFXCMN_H__
#endif

#ifndef __AFXEXT_H__
#include <afxext.h>
#define __AFXEXT_H__
#endif

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#define __AFXTEMPL_H__
#endif

#ifndef __AFXPRIV_H__
#include <afxpriv.h>
#define __AFXPRIV_H__
#endif

#ifndef __OXMFCIMPL_H__
#if _MFC_VER < 0x0700
	#include <..\src\afximpl.h>
#else
	#include <..\src\mfc\afximpl.h>
#endif
#define __OXMFCIMPL_H__
#endif


// for drag and drop support
#include "OXDragDropSupport.h"

#include "OXBaseSubclassedComboBox.h"


#if _MFC_VER<0x0421
#define TBSTYLE_LIST				0x1000
#define TB_SETBUTTONWIDTH			(WM_USER + 59)
#define TB_SETMAXTEXTROWS		    (WM_USER + 60)
#define TB_GETTEXTROWS				(WM_USER + 61)
#endif

// we need these type definitions to get information about dll version
// we use it in our GetComCtlVersion function
#ifndef _OXDLLVERSIONINFO_DECL
#define _OXDLLVERSIONINFO_DECL
#if _MFC_VER >0x0421
#include <shlwapi.h>
#else
typedef struct _DllVersionInfo
{
	DWORD cbSize;
	DWORD dwMajorVersion;
	DWORD dwMinorVersion;
	DWORD dwBuildNumber;
	DWORD dwPlatformID;
}DLLVERSIONINFO;
#define DLLVER_PLATFORM_WINDOWS		0	// Windows platform (95/98)
#define DLLVER_PLATFORM_NT			1	// Windows NT platform
typedef int (FAR WINAPI *DLLGETVERSIONPROC) (DLLVERSIONINFO *);
#endif
#endif
///////////////////////////

// this structure is defined in new commctrl.h header file
// as long as we want our class to work with all version of 
// comctl32.dll we have to provide definiton of this structure
//
// Custom Draw Structure
#if _WIN32_IE<0x0400
typedef struct _NMTBCUSTOMDRAW 
{
    NMCUSTOMDRAW nmcd;
    HBRUSH hbrMonoDither;
    HBRUSH hbrLines;                // For drawing lines on buttons
    HPEN hpenLines;                 // For drawing lines on buttons

    COLORREF clrText;               // Color of text
    COLORREF clrMark;               // Color of text bk when marked. (only if TBSTATE_MARKED)
    COLORREF clrTextHighlight;      // Color of text when highlighted
    COLORREF clrBtnFace;            // Background of the button
    COLORREF clrBtnHighlight;       // 3D highlight
    COLORREF clrHighlightHotTrack;  // In conjunction with fHighlightHotTrack
                                    // will cause button to highlight like a menu
    RECT rcText;                    // Rect for text

    int nStringBkMode;
    int nHLStringBkMode;
} NMTBCUSTOMDRAW, * LPNMTBCUSTOMDRAW;
#endif

#if _WIN32_IE<0x0400
#define TBSTYLE_EX_DRAWDDARROWS 0x00000001
#define TBSTYLE_TRANSPARENT     0x8000
#define TBSTYLE_AUTOSIZE        0x0010 // automatically calculate the cx of the button
#define TBSTYLE_REGISTERDROP    0x4000
#define TBSTYLE_NOPREFIX        0x0020 // if this button should not have accel prefix
#endif

#if _WIN32_IE<0x0400
#define TBN_GETOBJECT           (TBN_FIRST - 12)

// Structure for TBN_HOTITEMCHANGE notification
//
typedef struct tagNMTBHOTITEM
{
    NMHDR   hdr;
    int     idOld;
    int     idNew;
    DWORD   dwFlags;           // HICF_*
} NMTBHOTITEM, * LPNMTBHOTITEM;

// Hot item change flags
#define HICF_OTHER          0x00000000
#define HICF_MOUSE          0x00000001          // Triggered by mouse
#define HICF_ARROWKEYS      0x00000002          // Triggered by arrow keys
#define HICF_ACCELERATOR    0x00000004          // Triggered by accelerator
#define HICF_DUPACCEL       0x00000008          // This accelerator is not unique
#define HICF_ENTERING       0x00000010          // idOld is invalid
#define HICF_LEAVING        0x00000020          // idNew is invalid
#define HICF_RESELECT       0x00000040          // hot item reselected


#define TBN_HOTITEMCHANGE       (TBN_FIRST - 13)
#define TBN_DRAGOUT             (TBN_FIRST - 14) // this is sent when the user clicks down on a button then drags off the button
#define TBN_DELETINGBUTTON      (TBN_FIRST - 15) // uses TBNOTIFY
#define TBN_GETDISPINFOA        (TBN_FIRST - 16) // This is sent when the  toolbar needs  some display information
#define TBN_GETDISPINFOW        (TBN_FIRST - 17) // This is sent when the  toolbar needs  some display information
#define TBN_GETINFOTIPA         (TBN_FIRST - 18)
#define TBN_GETINFOTIPW         (TBN_FIRST - 19)


typedef struct tagNMTBGETINFOTIPA
{
    NMHDR hdr;
    LPSTR pszText;
    int cchTextMax;
    int iItem;
    LPARAM lParam;
} NMTBGETINFOTIPA, *LPNMTBGETINFOTIPA;

typedef struct tagNMTBGETINFOTIPW
{
    NMHDR hdr;
    LPWSTR pszText;
    int cchTextMax;
    int iItem;
    LPARAM lParam;
} NMTBGETINFOTIPW, *LPNMTBGETINFOTIPW;

#ifdef UNICODE
#define TBN_GETINFOTIP          TBN_GETINFOTIPW
#define NMTBGETINFOTIP          NMTBGETINFOTIPW
#define LPNMTBGETINFOTIP        LPNMTBGETINFOTIPW
#else
#define TBN_GETINFOTIP          TBN_GETINFOTIPA
#define NMTBGETINFOTIP          NMTBGETINFOTIPA
#define LPNMTBGETINFOTIP        LPNMTBGETINFOTIPA
#endif

#define TBNF_IMAGE              0x00000001
#define TBNF_TEXT               0x00000002
#define TBNF_DI_SETITEM         0x10000000

typedef struct {
    NMHDR  hdr;
    DWORD dwMask;     // [in] Specifies the values requested .[out] Client ask the data to be set for future use
    int idCommand;    // [in] id of button we're requesting info for
    DWORD lParam;     // [in] lParam of button
    int iImage;       // [out] image index
    LPSTR pszText;    // [out] new text for item
    int cchText;      // [in] size of buffer pointed to by pszText
} NMTBDISPINFOA, *LPNMTBDISPINFOA;

typedef struct {
    NMHDR hdr;
    DWORD dwMask;      //[in] Specifies the values requested .[out] Client ask the data to be set for future use
    int idCommand;    // [in] id of button we're requesting info for
    DWORD lParam;     // [in] lParam of button
    int iImage;       // [out] image index
    LPWSTR pszText;   // [out] new text for item
    int cchText;      // [in] size of buffer pointed to by pszText
} NMTBDISPINFOW, *LPNMTBDISPINFOW;


#ifdef UNICODE
#define TBN_GETDISPINFO       TBN_GETDISPINFOW
#define NMTBDISPINFO	      NMTBDISPINFOW
#define LPNMTBDISPINFO        LPNMTBDISPINFOW
#else
#define TBN_GETDISPINFO       TBN_GETDISPINFOA
#define NMTBDISPINFO          NMTBDISPINFOA
#define LPNMTBDISPINFO        LPNMTBDISPINFOA
#endif

// Return codes for TBN_DROPDOWN
#define TBDDRET_DEFAULT         0
#define TBDDRET_NODEFAULT       1
#define TBDDRET_TREATPRESSED    2       // Treat as a standard press button

#endif

#if _WIN32_IE<0x0400
#define TBSTATE_MARKED          0x80
#endif

#if _WIN32_IE<0x0400
#define TB_MARKBUTTON           (WM_USER + 6)
#define TB_ISBUTTONHIGHLIGHTED  (WM_USER + 14)
#define TB_GETOBJECT            (WM_USER + 62)  // wParam == IID, lParam void **ppv
#define TB_GETHOTITEM           (WM_USER + 71)
#define TB_SETHOTITEM           (WM_USER + 72)  // wParam == iHotItem
#define TB_SETANCHORHIGHLIGHT   (WM_USER + 73)  // wParam == TRUE/FALSE
#define TB_GETANCHORHIGHLIGHT   (WM_USER + 74)
#define TB_MAPACCELERATORA      (WM_USER + 78)  // wParam == ch, lParam int * pidBtn

typedef struct {
    int   iButton;
    DWORD dwFlags;
} TBINSERTMARK, * LPTBINSERTMARK;
#define TBIMHT_AFTER      0x00000001 // TRUE = insert After iButton, otherwise before
#define TBIMHT_BACKGROUND 0x00000002 // TRUE iff missed buttons completely

#define TB_GETINSERTMARK        (WM_USER + 79)  // lParam == LPTBINSERTMARK
#define TB_SETINSERTMARK        (WM_USER + 80)  // lParam == LPTBINSERTMARK
#define TB_INSERTMARKHITTEST    (WM_USER + 81)  // wParam == LPPOINT lParam == LPTBINSERTMARK
#define TB_MOVEBUTTON           (WM_USER + 82)
#define TB_GETMAXSIZE           (WM_USER + 83)  // lParam == LPSIZE
#define TB_SETEXTENDEDSTYLE     (WM_USER + 84)  // For TBSTYLE_EX_*
#define TB_GETEXTENDEDSTYLE     (WM_USER + 85)  // For TBSTYLE_EX_*
#define TB_GETPADDING           (WM_USER + 86)
#define TB_SETPADDING           (WM_USER + 87)
#define TB_SETINSERTMARKCOLOR   (WM_USER + 88)
#define TB_GETINSERTMARKCOLOR   (WM_USER + 89)

#define TB_SETCOLORSCHEME       CCM_SETCOLORSCHEME  // lParam is color scheme
#define TB_GETCOLORSCHEME       CCM_GETCOLORSCHEME	// fills in COLORSCHEME pointed to by lParam

#define TB_SETUNICODEFORMAT     CCM_SETUNICODEFORMAT
#define TB_GETUNICODEFORMAT     CCM_GETUNICODEFORMAT

#define TB_MAPACCELERATORW      (WM_USER + 90)  // wParam == ch, lParam int * pidBtn
#ifdef UNICODE
#define TB_MAPACCELERATOR       TB_MAPACCELERATORW      
#else
#define TB_MAPACCELERATOR       TB_MAPACCELERATORA
#endif
#endif


#if _WIN32_IE<0x0400
#define TBIF_IMAGE              0x00000001
#define TBIF_TEXT               0x00000002
#define TBIF_STATE              0x00000004
#define TBIF_STYLE              0x00000008
#define TBIF_LPARAM             0x00000010
#define TBIF_COMMAND            0x00000020
#define TBIF_SIZE               0x00000040

typedef struct {
    UINT cbSize;
    DWORD dwMask;
    int idCommand;
    int iImage;
    BYTE fsState;
    BYTE fsStyle;
    WORD cx;
    DWORD lParam;
    LPSTR pszText;
    int cchText;
} TBBUTTONINFOA, *LPTBBUTTONINFOA;

typedef struct {
    UINT cbSize;
    DWORD dwMask;
    int idCommand;
    int iImage;
    BYTE fsState;
    BYTE fsStyle;
    WORD cx;
    DWORD lParam;
    LPWSTR pszText;
    int cchText;
} TBBUTTONINFOW, *LPTBBUTTONINFOW;

#ifdef UNICODE
#define TBBUTTONINFO TBBUTTONINFOW
#define LPTBBUTTONINFO LPTBBUTTONINFOW
#else
#define TBBUTTONINFO TBBUTTONINFOA
#define LPTBBUTTONINFO LPTBBUTTONINFOA
#endif


// BUTTONINFO APIs do NOT support the string pool.
#define TB_GETBUTTONINFOW        (WM_USER + 63)
#define TB_SETBUTTONINFOW        (WM_USER + 64)
#define TB_GETBUTTONINFOA        (WM_USER + 65)
#define TB_SETBUTTONINFOA        (WM_USER + 66)
#ifdef UNICODE
#define TB_GETBUTTONINFO        TB_GETBUTTONINFOW
#define TB_SETBUTTONINFO        TB_SETBUTTONINFOW
#else
#define TB_GETBUTTONINFO        TB_GETBUTTONINFOA
#define TB_SETBUTTONINFO        TB_SETBUTTONINFOA
#endif


#define TB_INSERTBUTTONW        (WM_USER + 67)
#define TB_ADDBUTTONSW          (WM_USER + 68)

#define TB_HITTEST              (WM_USER + 69)

#define TB_SETDRAWTEXTFLAGS     (WM_USER + 70)  // wParam == mask lParam == bit values

#endif         // _WIN32_IE < 0x0400


// this structure will be sent to the parent window to provide Custom Draw
// functionality:
//
// hdr				-	standard NMHDR
// pCustomDraw		-	pointer to NMCUSTOMDRAW (version comctl <= 4.70) or 
//						NMTBCUSTOMDRAW (version comctl >= 4.71). See SDK documentation
//						for description of these structures and also refer to the 
//						documentation for NM_CUSTOMDRAW notification. As an example 
//						of using these structures in your OXCTBN_CUSTOMDRAW handler
//						you can use COXCoolToolBar::OnCustomDraw function
// result			-	The value your application can return depends on the current 
//						drawing stage. dwDrawStage member of the associated NMCUSTOMDRAW 
//						structure holds a value that specifies the drawing stage. You must 
//						return one of the following values. 
//
//						When dwDrawStage equals CDDS_PREPAINT:  
//
//						CDRF_DODEFAULT			-	The control will draw itself. It will 
//													not send any additional OXCTBN_CUSTOMDRAW 
//													messages for this paint cycle. 
//						CDRF_NOTIFYITEMDRAW		-	The control will notify the parent of 
//													any item-related drawing operations. 
//													It will send OXCTBN_CUSTOMDRAW 
//													notification messages before and after 
//													drawing items. 
//						CDRF_NOTIFYITEMERASE	-	The control will notify the parent when 
//													an item will be erased. It will send 
//													OXCTBN_CUSTOMDRAW notification messages 
//													before and after erasing items. 
//						CDRF_NOTIFYPOSTERASE	-	The control will notify the parent after
//													erasing an item. 
//						CDRF_NOTIFYPOSTPAINT	-	The control will notify the parent after 
//													painting an item. 
//						CDRF_NOTIFYSUBITEMDRAW	-	Version 4.71. The control will notify 
//													the parent when a list view subitem is 
//													being drawn.  
//
//						When dwDrawStage equals CDDS_ITEMPREPAINT:  
//
//						CDRF_NEWFONT			-	Your application specified a new font 
//													for the item; the control will use the 
//													new font. 
//						CDRF_SKIPDEFAULT		-	Your application drew the item manually.
//													The control will not draw the item. 
//						TBCDRF_NOEDGES			-	Version 4.71. Don't draw button edges. 
//						TBCDRF_HILITEHOTTRACK	-	Version 4.71. Use the clrHighlightHotTrack 
//													member of the NMTBCUSTOMDRAW structure 
//													to drawing the background of hot-tracked 
//													items. 
//						TBCDRF_NOOFFSET			-	Version 4.71. Don't offset button when 
//													pressed. 
//						TBCDRF_NOMARK			-	Version 4.71. Don't draw default 
//													highlight of items that have the 
//													TBSTATE_MARKED state. 
//						TBCDRF_NOETCHEDEFFECT	-	Version 4.71. Don't draw etched effect 
//													for disabled items.  
//
// bDoDefault		-	if TRUE then default implementation will be used. Note that in that
//						case result member of this structure won't be used
//
typedef struct tagOXCTB_CUSTOMDRAW
{
	NMHDR hdr;
	LPNMCUSTOMDRAW pCustomDraw;
	LRESULT result;
	BOOL bDoDefault;
} OXCTB_CUSTOMDRAW, * LPOXCTB_CUSTOMDRAW;

/////////////////////////////////////////

// IE 4.0 uses comctl32.dll version 4.71
// i.e. major=4, minor=71 
// then we will get _IE40_COMCTL_VERSION as MAKELONG(minor,major) 
#define _IE40_COMCTL_VERSION 0x00040047



//
//////////////////////////////////////////////////////////


typedef struct _tagNMCTBCUSTOMIZE
{
	NMHDR nmhdr;
	UINT nCustomizeEventID;
	LPARAM lParam;
} NMCTBCUSTOMIZE;
//
//////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////
//	CUSTOM TOOLBAR BUTTONS
//////////////////////////////////////////////////////////////////////////
/*

Overview

Toolbar control can be easily populated with standard buttons and in most 
cases standard buttons is all you need in your application. But what if you 
would like to put a combo box or slider control on a toolbar? In this case
the solution, although it exists, is clumsy one and hardly can be considered 
as flexible enough to provide customization functionality (as a reminder, 
in order to put an arbitrary control on a toolbar you have to add separator 
button as placeholder, create the control, initialize it with necessary data
and then call SetButtonInfo function where you specify the index of the 
placeholder button, ID of the control and with of the control). 

The major challenge arises when you try to implement support for putting 
any window on a toolbar as a custom toolbar button is to support the 
customization functionality provided by our Customize Manager Framework. 
Customize Manager allows you to use drag and drop functionality in order to
move, copy and delete buttons. While moving and deleting are not that 
difficult to implement copying of a custom toolbar button is not a trivial 
task at all because it involves the creation of new window and population 
it with some data. Different kind of windows will have different approaches 
for creating their copies. 

From the other side some operations on custom buttons (like moving or deleting 
them or changing their width) can be implemented in the same way independently
of the type of the associated control window. 

So the problem was to create an easy way for creating custom toolbar buttons that
will have an associated window of any kind and will support customization
functionality to full extent. It is not a trivial task to solve and here is 
what we came up with.

Every custom button must be derived from COXCustomTBButtonTemplate class (in other 
words it must implement the interface defined by COXCustomTBButtonTemplate class).
COXCustomTBButtonTemplate can be considered as abstract class that defines the way
to communicate with the window associated with a custom toolbar button in a standard
way that doesn't depend on the type of this window. Here is the list of the functions
that defines an interface to access the custom button associated window.

	virtual CWnd* CBGetWindow();
	virtual BOOL CBCreate();
	virtual COXCustomTBButtonTemplate* CBCreateCopy() const;
	virtual COXCustomTBButtonTemplate* CBCreateNewInstance() const;
	virtual void CBCopyContents() const; 
	virtual BOOL CBDelete();
	virtual BOOL CBSaveState();
	virtual BOOL CBLoadState();


Refere to the COXCustomTBButtonTemplate class reference for details on these 
functions. As you can see these methods are designed for aceessing associated
window, creating a copy of button, removing a button, saving and restoring the 
state of the button. 

Using multiple inheritance and deriving from COXCustomTBButtonTemplate class and
from the class that implements the associated window you can create a custom button
class. This approach will guarantee that a custom button is easily accessible in 
the standard way defined by COXCustomTBButtonTemplate class. 

COXCustomTBButtonTemplate methods can be implemented in more or less standard way
for most simple control windows. We have created new template based class that 
implements those methods: COXCustomTBButtonWnd, which is declared as following:

	template<class PARENTWND> 
	class COXCustomTBButtonWnd :	public PARENTWND, 
												public COXCustomTBButtonTemplate

so e.g. if you need to put an edit box on a toolbar you need to declare the 
following custom toolbar button object:

	COXCustomTBButtonWnd<CEdit> m_editCB;

Refer to the COXCustomTBButtonWnd class reference for the list of all available 
functions. Here we just briefly outline the details of COXCustomTBButtonWnd 
implementation:

	CBCreate()			uses standard CWnd::CreateEx() in order to create the
						associated window
	CBCreateCopy()		calls internally CBCreateNewInstance() in order to 
						instantiate new object, then calls CBCreate() specifying
						the style and settings (including) window text of the 
						original window and	then calls CBCopyContents()
	CBCopyContents()	just copies the font assigned to the original window
	CBDelete()			hide the original or destroys the copy of the associated
						window
	CBSaveState()		saves window text
	CBLoadState()		restore window text

As you can see the implemented functionality would satisfy only simple window
like edit control or static control. In most cases you still have to derive your 
own class and implement some of the functions. 

The most common scenario would be to provide implementation of CBCreateNewInstance()
function (a must requirement as long as you have to create an instance of a particular 
class) and implementation of CBCopyContents() function where you would put 
the associated window's specific code for copying its contents (e.g. in the case
with slider control you will copy the current thumb position).

Template-based COXCustomTBComboBox class is a good example on how COXCustomTBButtonWnd
derived classes should be implemented. This class is declared as:

	template<class PARENTCOMBOBOX>
	class COXCustomTBComboBox : public COXCustomTBButtonWnd<PARENTCOMBOBOX>

E.g. if you need to put a history combo box on a toolbar you need to declare the 
following custom button:

	COXCustomTBComboBox<COXHistoryCombo> m_histComboCB;


We also have created another helper class for adding combo boxes on a toolbar 
(we figured it would be the most popular control to put on a toolbar). 
COXCustomTBSubclassedComboBox is declared as following:

	template<class PARENTCOMBOBOX, class PARENTLISTBOX, class PARENTEDIT>
	class COXCustomTBSubclassedComboBox : 
	public COXBaseSubclassedComboBox<COXCustomTBComboBox<PARENTCOMBOBOX>,PARENTLISTBOX,PARENTEDIT>

Pretty messy but the idea is to allow you to specify any class as a parent class
for combo box components. E.g. if you want to put a combo box with masked edit 
control as its edit control on a toolbar you have to declare the following custom
toolbar button:

	COXCustomTBSubclassedComboBox<CComboBox,CListBox,COXMaskedEdit> m_maskedEditComboCB;




Now when we know how to create custom toolbar button let us move to the topic that
describes how to put them on a toolbar. 

COXCoolToolBar implements full support for the custom buttons. This class has a set
of functions to work with custom buttons. And Customize manager framework supports
customization functionality for custom buttons.

Refer to COXCoolToolBar class reference to the full list of functions related to 
the use of custom toolbar buttons. Here we give you an overview on how to add 
any custom button to a toolbar.

1)	Declare custom toolbar buttom object. You have to guarantee that this object
	will exist at least as long as the containing toolbar:

	COXCustomTBComboBox<CComboBox> m_ComboBoxCB;


2)	Every custom button has global scope so it can be put in any toolbar. Before
	inserting a custom button it must be registered using global 
	COXCoolToolBar::AddCustomButton() function, which a global ID that uniquely
	identified the registered custom button in the application

  
3)	Insert custom button specifying position and custom button index using the 
	following function:

	BOOL InsertCustomButton();





Dependency:
	#include "OXCoolToolBar.h"


Source code files:
	"OXCoolToolBar.cpp"		
  
*/

#ifndef IDT_OXCTBB_CHECKFORDRAGDROPEVENT
#define IDT_OXCTBB_CHECKFORDRAGDROPEVENT			412
#endif	//	IDT_OXCTBB_CHECKFORDRAGDROPEVENT
#ifndef ID_OXCTBB_CHECKFORDRAGDROPEVENT_DELAY
#define ID_OXCTBB_CHECKFORDRAGDROPEVENT_DELAY		200
#endif	//	ID_OXCTBB_CHECKFORDRAGDROPEVENT_DELAY




// class for defining custom toolbar button window
//
class OX_CLASS_DECL COXCustomTBButtonTemplate
{
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXCustomTBButtonTemplate() : 
		m_bInCustomizationMode(FALSE), 
		m_bDynamicallyCreated(FALSE),
		m_nCheckForDragDropEventTimerID(0) 
		{};

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXCustomTBButtonTemplate() {};


protected:
	// flag that specifies that the button is in customization mode
	BOOL m_bInCustomizationMode;
	// flag that specifies that the button was dynamically created (if it is
	// TRUE we effectively can say that this object is a copy of original)
	BOOL m_bDynamicallyCreated;

	// internal timer for starting the process of reallocating the button
	// in the toolbar
	UINT_PTR m_nCheckForDragDropEventTimerID;

public:
	// --- In  :
	// --- Out : 
	// --- Returns: A pointer to the CWnd object associated with the button
	// --- Effect : Retrieves a pointer to window object associated with the 
	//				custom button
	virtual CWnd* CBGetWindow() { return NULL; }


	// --- In  :
	// --- Out : 
	// --- Returns: TRUE if custom button window was successfully created;
	//				FALSE otherwise
	// --- Effect : Creates the window associated with the custom button 
	virtual BOOL CBCreate(int nID, CWnd* pParent, DWORD dwStyle=WS_CHILD|WS_VISIBLE, 
		DWORD dwStyleEx=0, LPCTSTR lpszClassName=AfxRegisterWndClass(CS_DBLCLKS),
		LPCTSTR lpszWindowName=_T("")) 
	{ 
		UNREFERENCED_PARAMETER(nID);
		UNREFERENCED_PARAMETER(pParent);
		UNREFERENCED_PARAMETER(dwStyle);
		UNREFERENCED_PARAMETER(dwStyleEx);
		UNREFERENCED_PARAMETER(lpszClassName);
		UNREFERENCED_PARAMETER(lpszWindowName);
		return FALSE;
	}


	// --- In  :	pParent	- parent window
	// --- Out : 
	// --- Returns: A pointer to a new custom button object that copies the
	//				contents of this custom button if succeessful; NULL otherwise
	// --- Effect : Creates a copy of this custom button. Depending on what kind 
	//				of functionality the derived custom button object will provide 
	//				the implementation of this function may vary from simple 
	//				(e.g. just duplicate window styles and window text) to pretty 
	//				complex. It is up to implementor to define the level of 
	//				similarity that must exist between the original and the copy.
	//				The general recommendation is to create the copy so it matches 
	//				the original as precise as possible.
	//
	//				The process of creation of the custom button copy can be divided 
	//				in two stages: instantiating of new COXCustomTBButtonTemplate
	//				derived object and copying into it the contents of the original. 
	//				In your implementation this two stages could be implemented 
	//				through the following virtual functions: CBCreateNewInstance()
	//				and CBCopyContents(). The advantage of using this function adds 
	//				the granularity level to your implementation that allows 
	//				programmers to override your classes without code duplication
	//				(e.g. every COXCustomTBButtonTemplate class must implement
	//				CBCreateNewInstance() class in order to instantiate object of 
	//				needed type)
	virtual COXCustomTBButtonTemplate* CBCreateCopy(CWnd* pParent) const
	{
		UNREFERENCED_PARAMETER(pParent);
		return NULL;
	}

	// --- In  :
	// --- Out : 
	// --- Returns: A pointer to a new custom button object if succeessful; 
	//				NULL otherwise
	// --- Effect : Creates new instance of the object. This is a mandatory part
	//				of creation copy of the custom button (another part would be
	//				the copying of the button contents). Altough it is not a must
	//				requirement but whenever you need to instantiate 
	//				COXCustomTBButtonTemplate derived object in your implementation 
	//				you should use this function and therefore should implement it.
	virtual COXCustomTBButtonTemplate* CBCreateNewInstance() const { return NULL; }
	
	// --- In  :	pCBTemplate	-	a pointer to custom button object the contents
	//								of this button will be copied to
	// --- Out : 
	// --- Returns: 
	// --- Effect : Copies the contents of this custom button object into the 
	//				specified object. This is a crucial part of a process 
	//				of creation copy of the custom button. Altough it is not a must
	//				requirement but whenever you need to do operation like this 
	//				(e.g. in your CBCreateCopy() function implementation) you 
	//				should use this function and therefore should implement it.
	virtual void CBCopyContents(COXCustomTBButtonTemplate* pCBTemplate) const 
	{
		UNREFERENCED_PARAMETER(pCBTemplate);
	}
	

	// --- In  :	
	// --- Out : 
	// --- Returns: TRUE if this custom button was successfully removed from toolbar;
	//				FALSE otherwise
	// --- Effect : Removes this custom button from toolbar. The implemtation of 
	//				this function must take into account whether the custom button 
	//				is the original or the copy
	virtual BOOL CBDelete() { return FALSE; }


	// --- In  :	
	// --- Out : 
	// --- Returns: TRUE if this custom button is in customization mode;
	//				FALSE otherwise
	// --- Effect : Retrieves the flag that specifies if custom button is in 
	//				customization or normal mode. In customization mode the 
	//				button might handle input messages (mouse and keyboard) 
	//				in different way.
	virtual BOOL CBIsInCustomizationMode() const { return m_bInCustomizationMode; }

	// --- In  :	bInCustomizationMode	-	TRUE if button will be set in
	//											customization mode;
	//											FALSE if button will be set in 
	//											normal mode
	// --- Out : 
	// --- Returns: 
	// --- Effect : Sets the flag that specifies if custom button is in 
	//				customization or normal mode. In customization mode the 
	//				button might handle input messages (mouse and keyboard) 
	//				in different way.
	virtual void CBSetCustomizationMode(BOOL bInCustomizationMode)
	{
		m_bInCustomizationMode=bInCustomizationMode;
	}


	// --- In  :	
	// --- Out : 
	// --- Returns: TRUE if this custom button was dynamically created 
	//				(i.e. a copy of the original custom button);
	//				FALSE otherwise (i.e. the original)
	// --- Effect : Retrieves the flag that specifies if custom button was created
	//				dynamically or not. In other words it allows us to distinguish 
	//				between the original custom button and its copies (copy is 
	//				created dynamically). Depending on this flag the process
	//				of removing the button from toolbar (CBDelete() function)
	//				must be handled differently. By deleting a copy we need to 
	//				destroy the associated window, by deleting the original
	//				we just need to hide the window
	virtual BOOL CBIsDynamicallyCreated() const { return m_bDynamicallyCreated; }

	// --- In  :	bDynamicallyCreated	-	TRUE if this button was dynamically
	//										created (a copy of the original);
	//										FALSE if this button is the original
	// --- Out : 
	// --- Returns: 
	// --- Effect : Sets the flag that specifies if custom button was created
	//				dynamically or not. Don't set this flag unless you are 100% 
	//				sure about the origin of the button
	virtual void CBSetDynamicallyCreated(BOOL bDynamicallyCreated)
	{
		m_bDynamicallyCreated=bDynamicallyCreated;
	}


	// --- In  :	ar	-	a reference to an archive object used for saving
	//						this custom button properties and state
	// --- Out : 
	// --- Returns: TRUE if custom button state was successfully stored;
	//				FALSE otherwise					
	// --- Effect : Saves the custom button properties and state into archive 
	//				object. Any kind of relevant data might be saved
	virtual BOOL CBSaveState(CArchive& ar) 
	{ 
		UNREFERENCED_PARAMETER(ar);
		return FALSE; 
	}

	// --- In  :	ar	-	a reference to an archive object that contains
	//						the  saved custom button properties and state
	// --- Out : 
	// --- Returns: TRUE if custom button state was successfully loaded;
	//				FALSE otherwise					
	// --- Effect : Loads the custom button properties and state from archive 
	//				object. The retrieved properties must be applied to the 
	//				button right away
	virtual BOOL CBLoadState(CArchive& ar) 
	{ 
		UNREFERENCED_PARAMETER(ar);
		return FALSE; 
	}
};



// class for defining custom toolbar button window
//
template<class PARENTWND>
class COXCustomTBButtonWnd : 
public PARENTWND, public COXCustomTBButtonTemplate
{
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXCustomTBButtonWnd();

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXCustomTBButtonWnd();

public:
	// --- In  :
	// --- Out : 
	// --- Returns: A pointer to the CWnd object associated with the button
	// --- Effect : Retrieves a pointer to window object associated with the 
	//				custom button
//	virtual CWnd* CBGetWindow() { return dynamic_cast<CWnd*>(this); }
	virtual CWnd* CBGetWindow() { return (CWnd*)this; }


	// --- In  :	nID				-	custom button window ID
	//				pParent			-	a pointer to parent window
	//				dwStyle			-	window style for custom button window
	//				dwStyleEx		-	extemnded window style for custom button 
	//									window
	//				lpszClassName	-	window class name for custom button 
	//									window 
	//				lpszWindowName	-	window text for custom button window 
	// --- Out : 
	// --- Returns: TRUE if custom button window was successfully created;
	//				FALSE otherwise
	// --- Effect : Creates the window associated with the custom button 
	virtual BOOL CBCreate(int nID, CWnd* pParent, DWORD dwStyle=WS_CHILD|WS_VISIBLE, 
		DWORD dwStyleEx=0, LPCTSTR lpszClassName=AfxRegisterWndClass(CS_DBLCLKS),
		LPCTSTR lpszWindowName=_T(""));

	
	// --- In  :	pParent	- parent window
	// --- Out : 
	// --- Returns: A pointer to a new custom button object that copies the
	//				contents of this custom button if succeessful; NULL otherwise
	// --- Effect : Creates a copy of this custom button. This implementation uses 
	//				CBCreateNewInstance() for instantiating new object that will
	//				contain the copy of this button. Then for the created 
	//				object it calls CBCreate() function with this button settings
	//				and then it calls it creates the CBCopyContents() for copying 
	//				the contents of this button
	virtual COXCustomTBButtonTemplate* CBCreateCopy(CWnd* pParent) const;

	// --- In  :
	// --- Out : 
	// --- Returns: A pointer to a new custom button object if succeessful; 
	//				NULL otherwise
	// --- Effect : Creates new instance of the object. This function is called
	//				in CBCreateCopy() implementation in order to intantiate a copy 
	//				of the original button
	virtual COXCustomTBButtonTemplate* CBCreateNewInstance() const;

	// --- In  :	pCBTemplate	-	a pointer to custom button object the contents
	//								of this button will be copied to
	// --- Out : 
	// --- Returns: 
	// --- Effect : Copies the contents of this custom button object into the 
	//				specified object. This function is called in CBCreateCopy() 
	//				implementation in order to copy button contents
	virtual void CBCopyContents(COXCustomTBButtonTemplate* pCBTemplate) const;
	

	// --- In  :	
	// --- Out : 
	// --- Returns: TRUE if this custom button was successfully removed from toolbar;
	//				FALSE otherwise
	// --- Effect : Removes this custom button from toolbar. The implemtation of 
	//				this function must take into account whether the custom button 
	//				is the original or the copy
	virtual BOOL CBDelete();


	// --- In  :	ar	-	a reference to an archive object used for saving
	//						this custom button properties and state
	// --- Out : 
	// --- Returns: TRUE if custom button state was successfully stored;
	//				FALSE otherwise					
	// --- Effect : Saves the custom button properties and state into archive 
	//				object. Any kind of relevant data might be saved
	virtual BOOL CBSaveState(CArchive& ar); 

	// --- In  :	ar	-	a reference to an archive object that contains
	//						the  saved custom button properties and state
	// --- Out : 
	// --- Returns: TRUE if custom button state was successfully loaded;
	//				FALSE otherwise					
	// --- Effect : Loads the custom button properties and state from archive 
	//				object. The retrieved properties must be applied to the 
	//				button right away
	virtual BOOL CBLoadState(CArchive& ar); 

protected:
	// --- In  :	message	-	message ID
	//				wParam	-	WPARAM
	//				lParam	-	LPARAM
	// --- Out : 
	// --- Returns:	result of message handling. Different for different messages.
	// --- Effect :	Handle all messages that go to the window
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);	

	// for deleting the object in the case it was dynamically created
	virtual void PostNcDestroy();
};


template<class PARENTWND>
COXCustomTBButtonWnd<PARENTWND>::COXCustomTBButtonWnd()
{
	// has to be derived from CWnd or its derivates
	if(!IsKindOf(RUNTIME_CLASS(CWnd)))
	{
		TRACE(_T("COXCustomTBButtonWnd has to be derived from CWnd or its derivates\n"));
		AfxThrowNotSupportedException();
	}
}

template<class PARENTWND>
COXCustomTBButtonWnd<PARENTWND>::~COXCustomTBButtonWnd()
{
}


template<class PARENTWND>
BOOL COXCustomTBButtonWnd<PARENTWND>::CBCreate(int nID, CWnd* pParent, 
											   DWORD dwStyle/*=WS_CHILD|WS_VISIBLE*/, 
											   DWORD dwStyleEx/*=0*/, 
											   LPCTSTR lpszClassName/*=AfxRegisterWndClass(CS_DBLCLKS)*/,
											   LPCTSTR lpszWindowName/*=_T("")*/)
{
	ASSERT(!::IsWindow(GetSafeHwnd()));
	return CWnd::CreateEx(dwStyleEx,lpszClassName,lpszWindowName,
		dwStyle,CRect(0,0,0,0),pParent,nID);
}


template<class PARENTWND> COXCustomTBButtonTemplate* 
COXCustomTBButtonWnd<PARENTWND>::CBCreateCopy(CWnd* pParent) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	
	CString sClassName;
	VERIFY(::GetClassName(GetSafeHwnd(),sClassName.GetBuffer(MAX_PATH),MAX_PATH)!=0);
	sClassName.ReleaseBuffer();
	CString sWindowText;
	GetWindowText(sWindowText);
	COXCustomTBButtonWnd<PARENTWND>* pNewWnd=
		(COXCustomTBButtonWnd<PARENTWND>*)CBCreateNewInstance();
	if(pNewWnd==NULL)
	{
		TRACE(_T("CBCreateCopy: failed to instantiate new custom button object\n"));
		return NULL;
	}
	if(!pNewWnd->CBCreate(GetDlgCtrlID(),pParent,GetStyle(),GetExStyle(),
		sClassName,sWindowText))
	{
		delete pNewWnd;
		TRACE(_T("CBCreateCopy: failed to create new custom button\n"));
		return NULL;
	}
	pNewWnd->m_bDynamicallyCreated=TRUE;

	CBCopyContents((COXCustomTBButtonTemplate*)pNewWnd);
	return (COXCustomTBButtonTemplate*)pNewWnd;
}
	

template<class PARENTWND> 
COXCustomTBButtonTemplate* COXCustomTBButtonWnd<PARENTWND>::
CBCreateNewInstance() const
{
//	return dynamic_cast<COXCustomTBButtonTemplate*>(new COXCustomTBButtonWnd<PARENTWND>);
	return (COXCustomTBButtonTemplate*)(new COXCustomTBButtonWnd<PARENTWND>);
}


template<class PARENTWND> 
void COXCustomTBButtonWnd<PARENTWND>::
CBCopyContents(COXCustomTBButtonTemplate* pCBTemplate) const
{
	ASSERT(pCBTemplate!=NULL);
	CFont* pFont=GetFont();
	if(pFont!=NULL)
	{
		pCBTemplate->CBGetWindow()->SetFont(pFont);
	}
}


template<class PARENTWND>
BOOL COXCustomTBButtonWnd<PARENTWND>::CBDelete()
{
	ASSERT(::IsWindow(GetSafeHwnd()));
	if(CBIsDynamicallyCreated())
	{
		if(!DestroyWindow())
		{
			TRACE(_T("COXCustomTBButtonWnd::CBDelete: failed to destroy custom toolbar button window\n"));
			return FALSE;
		}
	}
	else
	{
		if(!ShowWindow(SW_HIDE))
		{
			TRACE(_T("COXCustomTBButtonWnd::CBDelete: failed to hide custom toolbar button window\n"));
			return FALSE;
		}
	}

	return TRUE;
}


template<class PARENTWND>
BOOL COXCustomTBButtonWnd<PARENTWND>::CBSaveState(CArchive& ar)
{
	CString sWindowText(_T(""));
	if(::IsWindow(GetSafeHwnd()))
	{
		GetWindowText(sWindowText);
	}
	ar<<sWindowText;

	return TRUE;
}


template<class PARENTWND>
BOOL COXCustomTBButtonWnd<PARENTWND>::CBLoadState(CArchive& ar)
{
	CString sWindowText;
	ar>>sWindowText;
	if(::IsWindow(GetSafeHwnd()))
	{
		SetWindowText(sWindowText);
	}

	return TRUE;
}


template<class PARENTWND>
void COXCustomTBButtonWnd<PARENTWND>::PostNcDestroy()
{
	if(CBIsDynamicallyCreated())
	{
		delete this;
	}
}


template<class PARENTWND>
LRESULT COXCustomTBButtonWnd<PARENTWND>::WindowProc(UINT message, 
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

	ASSERT(::IsWindow(GetSafeHwnd()));

	BOOL bPreventUserInput=FALSE;
	BOOL bInCustomizationMode=CBIsInCustomizationMode();

	switch(message)
	{
	case WM_MOUSEMOVE:
		{
			if(m_nCheckForDragDropEventTimerID!=0 && ::GetKeyState(VK_LBUTTON)>=0)
			{
				KillTimer(m_nCheckForDragDropEventTimerID);
				m_nCheckForDragDropEventTimerID=0;
			}
			break;
		}

	case WM_NCLBUTTONDOWN:
	case WM_LBUTTONDOWN:
		{
			BOOL bCheckForDragDrop=bInCustomizationMode;
			if(!bCheckForDragDrop)
			{
				COXCoolToolBar* pCoolToolbar=
					DYNAMIC_DOWNCAST(COXCoolToolBar,GetParent());
				if(pCoolToolbar!=NULL)
				{
					if(pCoolToolbar->IsCustomizable(TRUE) && 
						((pCoolToolbar->GetStyle()&TBSTYLE_ALTDRAG && 
						::GetKeyState(VK_MENU)<0) ||
						(!(pCoolToolbar->GetStyle()&TBSTYLE_ALTDRAG) && 
						::GetKeyState(VK_SHIFT)<0)))
					{
						bCheckForDragDrop=TRUE;
					}
				}
			}
			if(bCheckForDragDrop)
			{
				bPreventUserInput=TRUE;
				ASSERT(m_nCheckForDragDropEventTimerID==0);
				m_nCheckForDragDropEventTimerID=
					SetTimer(IDT_OXCTBB_CHECKFORDRAGDROPEVENT,
					ID_OXCTBB_CHECKFORDRAGDROPEVENT_DELAY,NULL);
				ASSERT(m_nCheckForDragDropEventTimerID!=0);
			}

			break;
		}

	case WM_TIMER:
		{
			if((int)wParam==m_nCheckForDragDropEventTimerID)
			{
				KillTimer(m_nCheckForDragDropEventTimerID);
				m_nCheckForDragDropEventTimerID=0;

				if(::GetKeyState(VK_LBUTTON)<0)
				{
					COXCoolToolBar* pCoolToolbar=
						DYNAMIC_DOWNCAST(COXCoolToolBar,GetParent());
					if((pCoolToolbar!=NULL))
					{
						// do drag and drop by sending TBN_BEGINDRAG notification
						NMTOOLBAR nmtb;
						nmtb.hdr.hwndFrom=pCoolToolbar->GetSafeHwnd();
						nmtb.hdr.idFrom=GetDlgCtrlID();
						nmtb.hdr.code=TBN_BEGINDRAG;
						nmtb.iItem=PtrToInt(nmtb.hdr.idFrom);
						CWnd* pParentWnd=pCoolToolbar->GetParentFrame();
						ASSERT(pParentWnd!=NULL);
						pParentWnd->
							SendMessage(WM_NOTIFY,nmtb.hdr.idFrom,(LPARAM)&nmtb);
					}
				}
				//
				////////////////////////////////////
				return 0;
			}
			break;
		}
	}

	if(bInCustomizationMode || bPreventUserInput)
	{
		if(message>=WM_MOUSEFIRST && message<=WM_MOUSELAST)
		{
			CPoint point(LOWORD(lParam),HIWORD(lParam));
			ClientToScreen(&point);
			CWnd* pParentWnd=GetParent();
			ASSERT(pParentWnd!=NULL);
			pParentWnd->ScreenToClient(&point);
			return pParentWnd->SendMessage(message,wParam,MAKELPARAM(point.x,point.y));
		}

		if(message>=WM_KEYFIRST && message<=WM_KEYLAST)
		{
			return 0;
		}
	}

	// I don't handle it: pass along
	return PARENTWND::WindowProc(message,wParam,lParam);
}

//
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Predefined Custom Toolbar Buttons
//

//////////////////////////////////////////////////////////////////////////
//			ComboBox				
//
template<class PARENTCOMBOBOX>
class COXCustomTBComboBox : public COXCustomTBButtonWnd<PARENTCOMBOBOX>
{
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXCustomTBComboBox();


public:
	// --- In  :
	// --- Out : 
	// --- Returns: A pointer to a new combo box button object if succeessful; 
	//				NULL otherwise
	// --- Effect : Creates new instance of the object
	virtual COXCustomTBButtonTemplate* CBCreateNewInstance() const;


	// --- In  :	pCBTemplate	-	a pointer to combo box button object the contents
	//								of this button will be copied to
	// --- Out : 
	// --- Returns: 
	// --- Effect : Copies the contents of this combo box button object into the 
	//				specified object. For non-ownerdrawn combo boxes the items 
	//				will be copied
	virtual void CBCopyContents(COXCustomTBButtonTemplate* pCBTemplate) const;

	// --- In  :	nID				-	custom button window ID
	//				pParent			-	a pointer to parent window
	//				dwStyle			-	window style for custom button window
	//				dwStyleEx		-	extemnded window style for custom button 
	//									window
	//				lpszClassName	-	window class name for custom button 
	//									window 
	//				lpszWindowName	-	window text for custom button window 
	// --- Out : 
	// --- Returns: TRUE if custom button window was successfully created;
	//				FALSE otherwise
	// --- Effect : Creates the window associated with the custom button 
	virtual BOOL CBCreate(int nID, CWnd* pParent, DWORD dwStyle=WS_CHILD|WS_VISIBLE, 
		DWORD dwStyleEx=0, LPCTSTR lpszClassName=AfxRegisterWndClass(CS_DBLCLKS),
		LPCTSTR lpszWindowName=_T(""));
};


template<class PARENTCOMBOBOX>
COXCustomTBComboBox<PARENTCOMBOBOX>::COXCustomTBComboBox()
{
#ifdef _DEBUG
	// has to be derived from CWnd or its derivates
	if(!IsKindOf(RUNTIME_CLASS(CComboBox)))
	{
		TRACE(_T("COXCustomTBComboBox has to be derived from CComboBox or its derivates\n"));
		AfxThrowNotSupportedException();
	}
#endif
}

template<class PARENTWND>
BOOL COXCustomTBComboBox<PARENTWND>::CBCreate(int nID, CWnd* pParent, 
											   DWORD dwStyle/*=WS_CHILD|WS_VISIBLE*/, 
											   DWORD dwStyleEx/*=0*/, 
											   LPCTSTR lpszClassName/*=AfxRegisterWndClass(CS_DBLCLKS)*/,
											   LPCTSTR lpszWindowName/*=_T("")*/)
{
	ASSERT(!::IsWindow(GetSafeHwnd()));

	int iWidth = 0, iHeight = 0;
	COXCoolToolBar* pCoolToolBar = DYNAMIC_DOWNCAST(COXCoolToolBar, pParent);
	if (pCoolToolBar != NULL)
	{
		OXCUSTOMBUTTONDESCRIPTOR descriptorOrig;
		VERIFY(pCoolToolBar->m_mapAllCustomButtons.Lookup(nID,descriptorOrig));
		iWidth = descriptorOrig.m_nWidth;
		iHeight = descriptorOrig.m_nHeight;
	}
	
	return CWnd::CreateEx(dwStyleEx,lpszClassName,lpszWindowName,dwStyle,
		CRect(0,0,iWidth,iHeight),pParent,nID);
}

template<class PARENTCOMBOBOX> 
COXCustomTBButtonTemplate* COXCustomTBComboBox<PARENTCOMBOBOX>::
CBCreateNewInstance() const
{
	return (COXCustomTBButtonTemplate*)(new COXCustomTBComboBox<PARENTCOMBOBOX>);
}


template<class PARENTCOMBOBOX> 
void COXCustomTBComboBox<PARENTCOMBOBOX>::
CBCopyContents(COXCustomTBButtonTemplate* pCBTemplate) const
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	COXCustomTBButtonWnd<PARENTCOMBOBOX>::CBCopyContents(pCBTemplate);

	PARENTCOMBOBOX* pNewComboBox=(PARENTCOMBOBOX*)pCBTemplate->CBGetWindow();
	if((pNewComboBox->GetStyle()&(CBS_OWNERDRAWFIXED|CBS_OWNERDRAWVARIABLE))==0)
	{
		// remove existing items
		pNewComboBox->ResetContent();

		// copy string items and their associated data
		for(int nIndex=0; nIndex<GetCount(); nIndex++)
		{
			CString sText;
			GetLBText(nIndex,sText);
			int iIdx = pNewComboBox->AddString(sText);
			DWORD dwData = PtrToInt(GetItemData(nIndex));
			pNewComboBox->SetItemData(iIdx, dwData);
		}

		// set selection
		int nSelectedItem=GetCurSel();
		if(nSelectedItem!=-1)
		{
			pNewComboBox->SetCurSel(nSelectedItem);
		}

		CString sWindowText;
		GetWindowText(sWindowText);
		pNewComboBox->SetWindowText(sWindowText);
	}
}

//////////////////////////////////////////////////////////////////////////

template<class PARENTCOMBOBOX, class PARENTLISTBOX, class PARENTEDIT>
class COXCustomTBSubclassedComboBox : 
public COXBaseSubclassedComboBox<COXCustomTBComboBox<PARENTCOMBOBOX>,PARENTLISTBOX,PARENTEDIT>
{
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXCustomTBSubclassedComboBox();


public:
	// --- In  :
	// --- Out : 
	// --- Returns: A pointer to a new combo box button object if succeessful; 
	//				NULL otherwise
	// --- Effect : Creates new instance of the object
	virtual COXCustomTBButtonTemplate* CBCreateNewInstance() const;
};


template<class PARENTCOMBOBOX, class PARENTLISTBOX, class PARENTEDIT>
COXCustomTBSubclassedComboBox<PARENTCOMBOBOX,PARENTLISTBOX,PARENTEDIT>::
COXCustomTBSubclassedComboBox()
{
#ifdef _DEBUG
	PARENTLISTBOX listBox;
	if(!listBox.IsKindOf(RUNTIME_CLASS(CListBox)))
	{
		TRACE(_T("COXCustomTBSubclassedComboBox::COXCustomTBSubclassedComboBox: Parent class for the dropdown list box must be CListBox derived\n"));
		AfxThrowNotSupportedException();
	}

	PARENTEDIT edit;
	if(!edit.IsKindOf(RUNTIME_CLASS(CEdit)))
	{
		TRACE(_T("COXCustomTBSubclassedComboBox::COXCustomTBSubclassedComboBox: Parent class for the combo box edit control must be CEdit derived\n"));
		AfxThrowNotSupportedException();
	}
#endif
}

template<class PARENTCOMBOBOX, class PARENTLISTBOX, class PARENTEDIT> 
COXCustomTBButtonTemplate*
COXCustomTBSubclassedComboBox<PARENTCOMBOBOX,PARENTLISTBOX,PARENTEDIT>::
CBCreateNewInstance() const
{
//	return dynamic_cast<COXCustomTBButtonTemplate*>(
//		new COXCustomTBSubclassedComboBox<PARENTCOMBOBOX,PARENTLISTBOX,PARENTEDIT>);
	return (COXCustomTBButtonTemplate*)(
		new COXCustomTBSubclassedComboBox<PARENTCOMBOBOX,PARENTLISTBOX,PARENTEDIT>);
}

//
//////////////////////////////////////////////////////////////////////////



// flags for custom button descriptor
#define OXCBD_SHOWVERT					0x00000001
#define OXCBD_SHOWHORZ					0x00000002
#define OXCBD_SHOWFLOAT					0x00000004
#define OXCBD_SHOWANY					OXCBD_SHOWVERT|OXCBD_SHOWHORZ|OXCBD_SHOWFLOAT
#define OXCBD_SHOWONLYDOCK				OXCBD_SHOWVERT|OXCBD_SHOWHORZ
#define OXCBD_SHOWONLYFLOAT				OXCBD_SHOWFLOAT

// structure for custom buttons (any window can be assigned to toolbar button)
//
typedef struct _tagOXCUSTOMBUTTONDESCRIPTOR
{
	COXCustomTBButtonTemplate* m_pCBTemplate;
	int m_nID;
	int m_nWidth;
	int m_nHeight;
	int m_nFlags;
	int m_nImageIndex;

	// default constructor
	_tagOXCUSTOMBUTTONDESCRIPTOR(COXCustomTBButtonTemplate* pCBTemplate=NULL, 
		int nID=0, int nWidth=-1, int nHeight=-1, int nFlags=OXCBD_SHOWANY, 
		int nImageIndex=-1) 
		:
		m_pCBTemplate(pCBTemplate), m_nID(nID), m_nWidth(nWidth), m_nHeight(nHeight), 
		m_nFlags(nFlags), m_nImageIndex(nImageIndex)
		{
		};

	// copy constructor
	_tagOXCUSTOMBUTTONDESCRIPTOR(const _tagOXCUSTOMBUTTONDESCRIPTOR& descriptor) 
		:
		m_pCBTemplate(descriptor.m_pCBTemplate), 
		m_nID(descriptor.m_nID), 
		m_nWidth(descriptor.m_nWidth), 
		m_nHeight(descriptor.m_nHeight), 
		m_nFlags(descriptor.m_nFlags), 
		m_nImageIndex(descriptor.m_nImageIndex) 
		{
		};

	// assignment operator
	_tagOXCUSTOMBUTTONDESCRIPTOR& operator=(const _tagOXCUSTOMBUTTONDESCRIPTOR& descriptor) 
	{
		if(this==&descriptor)
			return *this;

		m_nID=descriptor.m_nID;
		m_pCBTemplate=descriptor.m_pCBTemplate;
		m_nWidth=descriptor.m_nWidth; 
		m_nHeight=descriptor.m_nHeight; 
		m_nFlags=descriptor.m_nFlags; 
		m_nImageIndex=descriptor.m_nImageIndex; 

		return *this;
	}

	BOOL CopyProperties(const _tagOXCUSTOMBUTTONDESCRIPTOR& descriptor)
	{
		if(this==&descriptor)
			return TRUE;

		if(m_nID!=descriptor.m_nID)
			return FALSE;

		m_nWidth=descriptor.m_nWidth; 
		m_nHeight=descriptor.m_nHeight; 
		m_nFlags=descriptor.m_nFlags; 
		m_nImageIndex=descriptor.m_nImageIndex; 

		return TRUE;
	}

	void SaveState(CArchive& ar)
	{
		ar<<m_nWidth; 
		ar<<m_nHeight; 
		ar<<m_nFlags; 
		ar<<m_nImageIndex; 
	}

	void LoadState(CArchive& ar)
	{
		ar>>m_nWidth; 
		ar>>m_nHeight; 
		ar>>m_nFlags; 
		ar>>m_nImageIndex; 
	}

} OXCUSTOMBUTTONDESCRIPTOR;
//
//////////////////////////////////////////////////////////////////////////


// helper class 
class OX_CLASS_DECL COXCoolToolBarCmdUI : public CCmdUI    
{
// Data members -------------------------------------------------------------
public:

protected:

private :

// Member functions ---------------------------------------------------------
public: // re-implementations only

	// --- In  : See CCmdUI::Enable
	// --- Out : See CCmdUI::Enable
	// --- Returns : See CCmdUI::Enable
	// --- Effect : See CCmdUI::Enable
	virtual void Enable(BOOL bOn);

	// --- In  : See CCmdUI::SetCheck
	// --- Out : See CCmdUI::SetCheck
	// --- Returns : See CCmdUI::SetCheck
	// --- Effect : See CCmdUI::SetCheck
	virtual void SetCheck(int nCheck);

protected:

private :

};


#if _MFC_VER>=0x0420
static const TCHAR szTextType[]=_T("TextType");
static const TCHAR szCool[]=_T("Cool");
static const TCHAR szGripper[]=_T("Gripper");
static const TCHAR szSeparator[]=_T("Separator");
static const TCHAR szFlat[]=_T("Flat");
static const TCHAR szList[]=_T("List");
static const TCHAR szIndent[]=_T("Indent");
static const TCHAR szDefaultTextColor[]=_T("DefaultTextColor");
static const TCHAR szHotTextColor[]=_T("HotTextColor");
static const TCHAR szSelectedTextColor[]=_T("SelectedTextColor");
static const TCHAR szCheckedTextColor[]=_T("CheckedTextColor");
static const TCHAR szDefaultBkColor[]=_T("DefaultBkColor");
static const TCHAR szHotBkColor[]=_T("HotBkColor");
static const TCHAR szSelectedBkColor[]=_T("SelectedBkColor");
static const TCHAR szCheckedBkColor[]=_T("CheckedBkColor");
static const TCHAR szDefaultBorderColor[]=_T("DefaultBorderColor");
static const TCHAR szHotBorderColor[]=_T("HotBorderColor");
static const TCHAR szSelectedBorderColor[]=_T("SelectedBorderColor");
static const TCHAR szCheckedBorderColor[]=_T("CheckedBorderColor");
static const TCHAR szTextRows[]=_T("TextRows");
static const TCHAR szMinButtonsWidth[]=_T("MinButtonsWidth");
static const TCHAR szMaxButtonsWidth[]=_T("MaxButtonsWidth");
static const TCHAR szDropDownArrow[]=_T("DropDownArrow");
static const TCHAR szDropDownButtonList[]=_T("DropDownButtonList");
#endif
static const TCHAR szCustomizable[]=_T("Customizable");

/////////////////////////////////////////////////////////////////////////////
// COXCoolToolBar window

#include "MFCToolBar.h"

#define CToolBar CMFCToolBar

// Define the WM_QUERYSNAPING user message used to query a frame window whether or not
// the snaping and tear-off behavior is enabled
#ifndef WM_QUERYSNAPPING
	#define WM_QUERYSNAPPING (WM_USER + 92)
#endif

class COXToolbarSkin;
class COXSizeDockBar;

class OX_CLASS_DECL COXCoolToolBar : public CToolBar
{
	friend class COXToolbarSkinClassic;
	friend class COXToolbarSkinXP;
	friend class COXToolbarSkin2003;
	friend class COXBitmapMenu;
	friend class COXSizableMiniDockFrameWnd;
	friend class COXDockTabCtrl;
	friend class COXMenuSkinXP;
	friend class COXMenuSkin2003;

	DECLARE_DYNAMIC(COXCoolToolBar)
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXCoolToolBar();

// Attributes
public:
	// functions to automatically set text to buttons
	//	TTID_PLAIN - lookup string resource for var equals button's ID
	//			if found, set text in button to string text 
	//			(not includded tool tip text)
	//	TTID_TOOLTIP - lookup string resource for var equals button's ID
	//			if found, set text in button to tooltip text 
	//	TTID_MENU - lookup current active menu for item that equals to button's ID
	//			if found, set text in button to text of corresponding Item in menu
	//	TTID_NOTSET - default value (there is no any text associated with toolbar)
	//	TTID_NONE - remove all text associated with buttons
	enum TextTypeFromID	{ TTID_PLAIN, TTID_TOOLTIP, TTID_MENU, TTID_NOTSET, TTID_NONE };

protected:

	// set to TRUE if WindowsNT running
	// some draw routines behave differently under Windows95 and WindowsNT
	BOOL m_bWindowsNTRunning;

	// defines version of comctl32.dll currently installed on the computer
	// GetComCtlVersion function returns major and minor version of this dll
	// and we combine these values using MAKELONG macro
	static DWORD m_dwComCtlVersion;

	// defines size of dropdown arrow which is drawn if utton has TBSTYLE_DROPDOWN set
	// and CoolToolBar has TBSTYLE_EX_DRAWDDARROWS extended style set
	int m_nDropDownArrowWidth;

	// type of text that is set to CoolToolBar
	TextTypeFromID m_ttID;

#if _MFC_VER>=0x0420
	// if we use CoolToolBar in CoolBar then this flag must be set 
	// in order to correctly render bar on screen
	BOOL m_bCool;
	// if set to TRUE and CoolToolBar is Flat and not Cool then
	// it draws the separator
	BOOL m_bSeparator; 
	// if set to TRUE and any button in the toolbar have TBSTYLE_DROPDOWN style
	// it would be drawn altogether with dropdown arrow, otherwise it wouldn't
	BOOL m_bDropDownArrow;

	// Save the width of indention because there is no such message as TB_GETINDENT
	int m_nIndent;

	// Save the min and max width of buttons because 
	// there is no such message as TB_GETBUTTONWIDTH
	CSize m_sizeMinMaxWidth;

	// text color in different states
	COLORREF m_crDefaultTextColor;
	COLORREF m_crHotTextColor;
	COLORREF m_crSelectedTextColor;
	COLORREF m_crCheckedTextColor;

	// background color in different states
	COLORREF m_crDefaultBkColor;
	COLORREF m_crHotBkColor;
	COLORREF m_crSelectedBkColor;
	COLORREF m_crCheckedBkColor;

	// border color in different states
	COLORREF m_crDefaultBorderColor;
	COLORREF m_crHotBorderColor;
	COLORREF m_crSelectedBorderColor;
	COLORREF m_crCheckedBorderColor;

	CPoint m_ptLButtonDown;

#endif

	// flag that specifies the previous state (docking/floating) 
	// of the control
	BOOL m_bPrevFloating;

	// variable that specifies the previous docking side of the control
	DWORD m_dwPrevDockSide;

	// icon to be displayed at the left or top side
	HICON m_hIcon;
	// rectangle for icon
	CRect m_iconRect;

	// index of the button being currently customized
	int m_nCustomizedButtonIndex;
	// flag that specifies that control supports advanced customization
	BOOL m_bAdvancedCutomizable;
	// flag that specifies that the toolbar is in the advanced customization mode
	BOOL m_bInAdvancedCustomizationMode;
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
	// index of the button being currently dragged
	int m_nDraggedButtonIndex;
	// If true ise the VS7 style snaping
	BOOL m_bSnapWhileDragging;
	// Deternines if the toolbar is currently being dragger
	BOOL m_bDragging;

	// array of IDs to show in the Bar
	UINT* m_pBitmapIds;				// ID pallette in the bitmap 
	// the number of all buttons
	int	m_nBitmapButtons;			// number of buttons in the toolbar palette

	// map array to keep all created handles to bitmap used by CoolToolBar
	typedef CMap<LPTSTR, LPTSTR, HBITMAP, HBITMAP> allBitmaps;
	allBitmaps m_allBitmaps;

	// map array to keep all created handles to image lists used by CoolToolBar
	typedef CMap<LPTSTR, LPTSTR, HANDLE, HANDLE> allImageLists;
	allImageLists m_allImageLists;

	// map of the text associated with buttons 
	CMap<UINT,UINT,CString,CString&> m_mapButtonText;

	// global array of all custom butons indexes
	static CArray<int,int> m_arrAllCustomButtonIDs;
	// global flag that specifies that custom button settings has been saved
	static BOOL m_bCustomButtonsStateSaved;
	// global flag that specifies that custom button settings has been loaded
	static BOOL m_bCustomButtonsStateLoaded;

	// map of the custom buttons (accessed by index)
	CMap<int,int,OXCUSTOMBUTTONDESCRIPTOR,OXCUSTOMBUTTONDESCRIPTOR&> 
		m_mapCustomButtons;

	// flag that might be set for preventing excessive redrawing of the 
	// toolbar's contents
	BOOL m_bNoInternalRedraw;
	// flag that might be set for preventing excessive redrawing of the 
	// toolbar's background
	BOOL m_bNoBkgndRedraw;

	// flags that defines the actions that should be taken when 
	// the toolbar is in idle state
	enum IdleFlags	{ oxidleRedrawToolbar=1, oxidleUpdateCustomButtons=2 };
	int m_nIdleFlags;

	BOOL m_bFloatingEnabled; // used to store whether the toolbar can be floated
							 // TRUE by default

public:
	// global map of all custom buttons (accessed by ctrlID)
	static CMap<int,int,OXCUSTOMBUTTONDESCRIPTOR,OXCUSTOMBUTTONDESCRIPTOR&> 
		m_mapAllCustomButtons;

// Operations

public:
	////////////////////////////////////////////////////////////////////////////
	//

	// --- In  :	descriptor	-	custom button descriptor. OXCUSTOMBUTTONDESCRIPTOR
	//								structure represents custom button properties.
	//								It includes the following members:
	//							
	//								COXCustomTBButtonTemplate* m_pCBTemplate;
	//								int m_nID;
	//								int m_nWidth;
	//								int m_nHeight;
	//								int m_nFlags;
	//								int m_nImageIndex;
	//								
	//								Refer to the description of this function 
	//								arguments below that are equivalents of  
	//								OXCUSTOMBUTTONDESCRIPTOR members
	//
	//				pCBTemplate	-	pointer to custom button object
	//				nID			-	ID of the window associated with the custom 
	//								button (this window must be always created 
	//								using WS_CHILD style)
	//				nWidth		-	the button width (i.e. the width of the 
	//								associated window)
	//				nHeight		-	the button height (i.e. the height of the 
	//								associated window). If -1 is specified then
	//								the button height will be set to the current 
	//								height of a standard toolbar button
	//				nFlags		-	any combination of the following flags:
	//
	//								OXCBD_SHOWVERT	-	show custom button window 
	//													when the toolbar is 
	//													vertically docked
	//								OXCBD_SHOWHORZ	-	show custom button window 
	//													when the toolbar is 
	//													horizontally docked
	//								OXCBD_SHOWFLOAT	-	show custom button window 
	//													when the toolbar is floating
	//								
	//								or one of the following predefined flag 
	//								combinations:
	//
	//								OXCBD_SHOWANY		-	show custom button window 
	//														always
	//								OXCBD_SHOWONLYDOCK	-	show custom button window 
	//														only when toolbar is 
	//														docked
	//								OXCBD_SHOWONLYFLOAT	-	show custom button window 
	//														only when toolbar is 
	//														floating
	//
	//								The default value is set to OXCBD_SHOWANY but
	//								sometimes it is inconvenient to display a custom
	//								button when toolbar is docked to a some side or
	//								is floating. You might consider setting this 
	//								parameter to a different value. E.g. for combo box 
	//								custom toolbar button it makes sense to show it
	//								when toolbar is horizontally docked or is floating
	//								and don't show it when toolbar is vertically 
	//								docked. 
	//								When custom button is not shown the standard 
	//								button is being displayed instead and 
	//								nImageIndex value will be used as button's image
	//
	//				nImageIndex	-	index of the button image that will be used 
	//								in standard button that will replace custom 
	//								buttonwhen custom button is not supposed 
	//								to be shown
	//				
	// --- Out : 
	// --- Returns:	The global ID of the registered custom button if succeed;
	//				otherwise FALSE. This ID can be used in order to insert
	//				this button in any toolbar (the first inserted instance 
	//				will be marked as original and the oters as copies)
	// --- Effect : Registeres custom toolbar button in global array of all
	//				custom buttons. The returned ID number of the registered 
	//				custom button can be used to insert such button in a toolbar
	//				or to access properties information for specific custom 
	//				button. This function must be called before adding any
	//				custom buttons in any toolbar
	static int AddCustomButton(OXCUSTOMBUTTONDESCRIPTOR& descriptor); 
	static int AddCustomButton(COXCustomTBButtonTemplate* pCBTemplate, int nID, 
		int nWidth, int nHeight=-1, int nFlags=OXCBD_SHOWANY, int nImageIndex=-1);


	// --- In  :	nCtrlID	-	custom button ID (ID of the window associated
	//							with the custom button)
	//				pWnd	-	a pointer to CWnd object associated	with a custom 
	//							button
	// --- Out :	
	// --- Returns:	Global custom button index assigned to the custom button if any 
	//				found; otherwise -1
	// --- Effect : Retrieves a position in the toolbar at which custom button with 
	//				specified ID is located
	static int GetCustomButtonIndex(int nCtrlID);
	static int GetCustomButtonIndex(CWnd* pWnd);


	// --- In  :	nCustomButtonIndex	-	index of the registered custom button
	//				descriptor			-	custom button descriptor. Refer to
	//										AddCustomButton() for details on
	//										this parameter
	// --- Out : 
	// --- Returns:	TRUE if custom button settings have been updated successfully;
	//				otherwise FALSE
	// --- Effect : Changes custom button settings and apply them to existing 
	static BOOL SetCustomButton(
		int nCustomButtonIndex, OXCUSTOMBUTTONDESCRIPTOR& descriptor); 


	// --- In  :	nIndex				-	button position in the toolbar
	//				nCustomButtonIndex	-	index of the registered custom button
	//				dwStyle				-	window style for custom button window
	//				dwStyleEx			-	extended window style for custom button 
	//										window
	//				lpszClassName		-	window class name for custom button 
	//										window 
	//				lpszWindowName		-	window text for custom button window 	
	// --- Out : 
	// --- Returns:	TRUE if custom button have been inserted successfully;
	//				otherwise FALSE
	// --- Effect : Inserts custom button at specified position and creates the 
	//				window associated with it using function's arguments
	BOOL InsertCustomButton(int nIndex, int nCustomButtonIndex, 
		DWORD dwStyle=WS_CHILD|WS_VISIBLE, DWORD dwStyleEx=0, 
		LPCTSTR lpszClassName=AfxRegisterWndClass(CS_DBLCLKS),
		LPCTSTR lpszWindowName=_T(""));


	// --- In  :	nIndex		-	button index in the toolbar
	// --- Out :	descriptor	-	custom button descriptor. Will be populated with
	//								custom button properties if any found at the 
	//								specified position. Refer to AddCustomButton()
	//								function for details on OXCUSTOMBUTTONDESCRIPTOR
	//								structure
	// --- Returns:	TRUE if custom button was found at the specified position;
	//				otherwise FALSE
	// --- Effect : Retrieves custom button properties at the specified position 
	//				in the toolbar
	inline BOOL GetCustomButton(int nIndex, OXCUSTOMBUTTONDESCRIPTOR& descriptor) const
	{
		return (m_mapCustomButtons.Lookup(nIndex,descriptor));
	}


	// --- In  :	nIndex		-	button index in the toolbar
	// --- Out :	
	// --- Returns:	A pointer to CWnd object associated with custom button if found
	//				at the specified position; otherwise NULL
	// --- Effect : Retrieves a ponter to CWnd object associated with custom button
	//				at the specified position in the toolbar
	inline CWnd* GetCustomButtonWnd(int nIndex) const
	{
		OXCUSTOMBUTTONDESCRIPTOR descriptor;
		if(!m_mapCustomButtons.Lookup(nIndex,descriptor))
		{
			return NULL;
		}
		else
		{
			return descriptor.m_pCBTemplate->CBGetWindow();
		}
	}


	// --- In  :	nCtrlID	-	custom button ID (ID of the window associated
	//							with the custom button)
	//				pWnd	-	a pointer to CWnd object associated	with a custom 
	//							button
	// --- Out :	
	// --- Returns:	A position in the toolbar at which custom button with specified 
	//				ID or associated window is located if any found; otherwise -1
	// --- Effect : Retrieves a position in the toolbar at which custom button with 
	//				specified ID associated window is located
	int GetCustomButtonPosition(int nCtrlID) const;
	inline int GetCustomButtonPosition(CWnd* pWnd) const
	{
		ASSERT(pWnd!=NULL);
		return GetCustomButtonPosition(pWnd->GetDlgCtrlID());
	}


	// --- In  :	nIndex	-	button index in the toolbar
	// --- Out :	
	// --- Returns:	TRUE if button at the specified position is custom button; 
	//				otherwise FALSE
	// --- Effect : Retrieves a flag that specifies if the toolbar button at the 
	//				specified position is custom button or not
	inline BOOL IsCustomButton(int nIndex) const
	{
		OXCUSTOMBUTTONDESCRIPTOR descriptor;
		return (m_mapCustomButtons.Lookup(nIndex,descriptor));
	}

	// --- In  :	nCtrlID	-	window control ID
	//				pWnd	-	a pointer to CWnd object	
	// --- Out :	
	// --- Returns:	TRUE if specified window is a window associated with a 
	//				custom button; otherwise FALSE
	// --- Effect : Retrieves a flag that specifies if specified window 
	//				is a window associated with a custom button or not
	BOOL IsCustomButtonWnd(int nCtrlID) const;
	inline BOOL IsCustomButtonWnd(CWnd* pWnd) const
	{
		ASSERT(pWnd!=NULL);
		return IsCustomButtonWnd(pWnd->GetDlgCtrlID());
	}


	// --- In  :	nFirstButtonIndex	-	the toolbar button index from which to
	//										start the update of custom buttons
	//				nLastButtonIndex	-	the toolbar button index up to which
	//										custom buttons will be updated. If -1 
	//										is specified the n all custom buttons 
	//										started from nFirstButtonIndex will be
	//										updated
	// --- Out :	
	// --- Returns:	
	// --- Effect : Updates the state of the custom buttons and redraws them
	void UpdateCustomButtons(int nFirstButtonIndex=0, int nLastButtonIndex=-1);

	// --- In  :	nIndex	-	the toolbar custom button index to update
	// --- Out :	
	// --- Returns:	
	// --- Effect : Updates the state of the specified custom button and redraws it
	inline void UpdateCustomButton(int nIndex)
	{
		UpdateCustomButtons(nIndex,nIndex);
	}

	// --- In  :	
	// --- Out :	
	// --- Returns:	
	// --- Effect : Forces to update the state of all custom buttons defined in the 
	//				toolbar next time when application is in idle state
	inline void DelayUpdateCustomButtons() { m_nIdleFlags|=oxidleUpdateCustomButtons; }


	/////////////////////////////////////////////////////////////////////////////
	// Set of helper functions for inserting custom buttons of specific types
	

	// --- In  :	nIndex				-	button position in the toolbar
	//				nCustomButtonIndex	-	index of the registered combo box button
	//				dwStyle				-	combo box window styles for combo box 
	//										button window
	//				dwStyleEx			-	extended combo box window styles for 
	//										combo box button window
	// --- Out : 
	// --- Returns:	TRUE if combo box button have been inserted successfully;
	//				otherwise FALSE
	// --- Effect : Inserts combo box button at specified position and creates the 
	//				combo box window associated with it using function's arguments
	BOOL InsertComboBox(int nIndex, int nCustomButtonIndex, 
		DWORD dwStyle=WS_CHILD|WS_VISIBLE|CBS_DROPDOWN|WS_VSCROLL, 
		DWORD dwStyleEx=0);


	// --- In  :	nIndex				-	button position in the toolbar
	//				nCustomButtonIndex	-	index of the registered edit button
	//				lpszWindowName		-	window text for edit button window 	
	//				dwStyle				-	edit window styles for edit 
	//										button window
	//				dwStyleEx			-	extended edit window styles for 
	//										edit button window
	// --- Out : 
	// --- Returns:	TRUE if edit button have been inserted successfully;
	//				otherwise FALSE
	// --- Effect : Inserts edit button at specified position and creates the 
	//				edit window associated with it using function's arguments
	BOOL InsertEditBox(int nIndex, int nCustomButtonIndex, 
		LPCTSTR lpszWindowName=_T(""),
		DWORD dwStyle=WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, 
		DWORD dwStyleEx=WS_EX_CLIENTEDGE);


	// --- In  :	nIndex				-	button position in the toolbar
	//				nCustomButtonIndex	-	index of the registered rich edit button
	//				dwStyle				-	rich edit window styles for rich edit 
	//										button window
	//				dwStyleEx			-	extended rich edit window styles for 
	//										rich edit button window
	// --- Out : 
	// --- Returns:	TRUE if edit button have been inserted successfully;
	//				otherwise FALSE
	// --- Effect : Inserts edit button at specified position and creates the 
	//				edit window associated with it using function's arguments
	BOOL InsertRichEditBox(int nIndex, int nCustomButtonIndex, 
		DWORD dwStyle=WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, 
		DWORD dwStyleEx=WS_EX_CLIENTEDGE);


	// --- In  :	nIndex				-	button position in the toolbar
	//				nCustomButtonIndex	-	index of the registered static button
	//				lpszWindowName		-	window text for static button window 	
	//				dwStyle				-	static window styles for static 
	//										button window
	//				dwStyleEx			-	extended static window styles for 
	//										static button window
	// --- Out : 
	// --- Returns:	TRUE if static button have been inserted successfully;
	//				otherwise FALSE
	// --- Effect : Inserts static button at specified position and creates the 
	//				static window associated with it using function's arguments
	BOOL InsertStaticCtrl(int nIndex, int nCustomButtonIndex, 
		LPCTSTR lpszWindowName, 
		DWORD dwStyle=WS_CHILD|WS_VISIBLE|SS_LEFT|SS_SIMPLE, 
		DWORD dwStyleEx=0);


	// --- In  :	nIndex				-	button position in the toolbar
	//				nCustomButtonIndex	-	index of the registered hot key button
	//				dwStyle				-	hot key window styles for hot key 
	//										button window
	//				dwStyleEx			-	extended hot key window styles for 
	//										hot key button window
	// --- Out : 
	// --- Returns:	TRUE if hot key button have been inserted successfully;
	//				otherwise FALSE
	// --- Effect : Inserts hot key button at specified position and creates the 
	//				hot key window associated with it using function's arguments
	BOOL InsertHotKeyCtrl(int nIndex, int nCustomButtonIndex, 
		DWORD dwStyle=WS_CHILD|WS_VISIBLE, DWORD dwStyleEx=0);


	// --- In  :	nIndex				-	button position in the toolbar
	//				nCustomButtonIndex	-	index of the registered button control 
	//										toolbar button
	//				lpszWindowName		-	window text for button control 
	//										toolbar button window 	
	//				dwStyle				-	button control window styles for button 
	//										control toolbar button window
	//				dwStyleEx			-	extended button control window styles for 
	//										button control toolbar button window
	// --- Out : 
	// --- Returns:	TRUE if button control toolbar button have been inserted 
	//				successfully; otherwise FALSE
	// --- Effect : Inserts button control toolbar button at specified position and 
	//				creates the button control window associated with it using 
	//				function's arguments
	BOOL InsertButtonCtrl(int nIndex, int nCustomButtonIndex, LPCTSTR lpszWindowName, 
		DWORD dwStyle=WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, DWORD dwStyleEx=0);


	// --- In  :	nIndex				-	button position in the toolbar
	//				nCustomButtonIndex	-	index of the registered check box button
	//				lpszWindowName		-	window text for check box button window 	
	//				dwStyle				-	check box window styles for check box 
	//										button window
	//				dwStyleEx			-	extended check box window styles for 
	//										check box button window
	// --- Out : 
	// --- Returns:	TRUE if check box button have been inserted successfully;
	//				otherwise FALSE
	// --- Effect : Inserts check box button at specified position and creates the 
	//				check box window associated with it using function's arguments
	BOOL InsertCheckBox(int nIndex, int nCustomButtonIndex, LPCTSTR lpszWindowName, 
		DWORD dwStyle=WS_CHILD|WS_VISIBLE|BS_CHECKBOX, DWORD dwStyleEx=0);


	// --- In  :	nIndex				-	button position in the toolbar
	//				nCustomButtonIndex	-	index of the registered progress button
	//				dwStyle				-	progress window styles for progress 
	//										button window
	//				dwStyleEx			-	extended progress window styles for 
	//										progress button window
	// --- Out : 
	// --- Returns:	TRUE if progress button have been inserted successfully;
	//				otherwise FALSE
	// --- Effect : Inserts progress button at specified position and creates the 
	//				progress window associated with it using function's arguments
	BOOL InsertProgressBar(int nIndex, int nCustomButtonIndex, 
		DWORD dwStyle=WS_CHILD|WS_VISIBLE, DWORD dwStyleEx=0);


	// --- In  :	nIndex				-	button position in the toolbar
	//				nCustomButtonIndex	-	index of the registered slider button
	//				dwStyle				-	slider window styles for slider 
	//										button window
	//				dwStyleEx			-	extended slider window styles for 
	//										slider button window
	// --- Out : 
	// --- Returns:	TRUE if slider button have been inserted successfully;
	//				otherwise FALSE
	// --- Effect : Inserts slider button at specified position and creates the 
	//				slider window associated with it using function's arguments
	BOOL InsertSlider(int nIndex, int nCustomButtonIndex, 
		DWORD dwStyle=WS_CHILD|WS_VISIBLE|TBS_HORZ, DWORD dwStyleEx=0);

#if _MFC_VER>0x0421
	// --- In  :	nIndex				-	button position in the toolbar
	//				nCustomButtonIndex	-	index of the registered date time picker 
	//										button
	//				date				-	COleDateTime object that specifies the
	//										date that will be originally set to
	//										date time picker button window
	//				dwStyle				-	date time picker window styles for 
	//										date time picker button window
	//				dwStyleEx			-	extended date time picker window styles 
	//										for date time picker button window
	// --- Out : 
	// --- Returns:	TRUE if date time picker button have been inserted successfully;
	//				otherwise FALSE
	// --- Effect : Inserts date time picker button at specified position and creates 
	//				the date time picker window associated with it using function's 
	//				arguments
	BOOL InsertDateTimePicker(int nIndex, int nCustomButtonIndex, 
		COleDateTime date, DWORD dwStyle=WS_CHILD|WS_VISIBLE, DWORD dwStyleEx=0);


	// --- In  :	nIndex				-	button position in the toolbar
	//				nCustomButtonIndex	-	index of the registered IPAddress button
	//				dwStyle				-	IPAddress window styles for IPAddress 
	//										button window
	//				dwStyleEx			-	extended IPAddress window styles for 
	//										IPAddress button window
	// --- Out : 
	// --- Returns:	TRUE if IPAddress button have been inserted successfully;
	//				otherwise FALSE
	// --- Effect : Inserts IPAddress button at specified position and creates the 
	//				IPAddress window associated with it using function's arguments
	BOOL InsertIPAddressCtrl(int nIndex, int nCustomButtonIndex, 
		DWORD dwStyle=WS_CHILD|WS_VISIBLE, DWORD dwStyleEx=0);


	// --- In  :	nIndex				-	button position in the toolbar
	//				nCustomButtonIndex	-	index of the registered extended 
	//										combo box button
	//				dwStyle				-	extended combo box window styles for 
	//										extended combo box button window
	//				dwStyleEx			-	extended extended combo box window styles 
	//										for extended combo box button window
	// --- Out : 
	// --- Returns:	TRUE if extended combo box button have been inserted successfully;
	//				otherwise FALSE
	// --- Effect : Inserts extended combo box button at specified position and 
	//				creates the extended combo box window associated with it using 
	//				function's arguments
	BOOL InsertComboBoxEx(int nIndex, int nCustomButtonIndex, 
		DWORD dwStyle=WS_CHILD|WS_VISIBLE|CBS_DROPDOWN, DWORD dwStyleEx=0);
#endif	//	_MFC_VER>0x0421

	//
	////////////////////////////////////////////////////////////////////////////


	// functions you can use to create bitmaps and image lists from resource file
	// to use them in CoolToolBar. In this case you shouldn't care about destroying 
	// created GDI object, CoolToolBar will destroy it automatically
	inline HBITMAP AddBitmap(UINT nBitmapID)
	{
		return AddBitmap(MAKEINTRESOURCE(nBitmapID));
	}
	HBITMAP AddBitmap(LPTSTR lpszResourceName);

	inline HIMAGELIST AddImageList(UINT nImageListID, int cx=16, 
								   int cGrow=0, COLORREF crMask=RGB(192, 192, 192), 
								   UINT uType=IMAGE_BITMAP, UINT uFlags=LR_DEFAULTCOLOR)
	{
		return AddImageList(MAKEINTRESOURCE(nImageListID),cx,cGrow,crMask,uType,uFlags);
	}
	HIMAGELIST AddImageList(LPTSTR lpszResourceName, int cx=16, 
							int cGrow=0, COLORREF crMask=RGB(192, 192, 192), 
							UINT uType=IMAGE_BITMAP, 
							UINT uFlags=LR_DEFAULTCOLOR);

	inline HIMAGELIST AddImageList(UINT nImageListID, int cx, 
								   COLORREF crMask=RGB(192, 192, 192), 
								   UINT uFlags=ILC_COLOR|ILC_MASK,
								   int cInitial=0, int cGrow=0)
	{
		return AddImageList(MAKEINTRESOURCE(nImageListID),cx,crMask,uFlags,cInitial,cGrow);
	}
	HIMAGELIST AddImageList(LPTSTR lpszResourceName, int cx, 
							COLORREF crMask=RGB(192, 192, 192), 
							UINT uFlags=ILC_COLOR|ILC_MASK,
							int cInitial=0, int cGrow=0);

	// standard create function
	// as soon as we set flat mode by default we do that in that function
	BOOL Create(CWnd* pParentWnd, DWORD dwStyle=WS_CHILD|WS_VISIBLE|CBRS_TOP,
		UINT nID=AFX_IDW_TOOLBAR);

	// --- In  : pIds : Points to an array of button bitmap Ids.
	//			 nButtons : Number of buttons in the above array.
	// --- Out : 
	// --- Returns :
	// --- Effect : It specifies an array of command IDs that the images 
	//				in the bitmap represent. Common control customization
	//				dialog uses it to determine which buttons could possible be 
	//				on the toolbar. The idea is that you call SetBitmapIds() 
	//				to set the ID’s that correspond  to the images in the bitmap, 
	//				and SetButtons() to determine the command ID’s
	//				that are actually on the toolbar. 
	//
	//				The common control customization allows you to drag buttons,
	//				by holding the shift button and clicking and dragging the button, 
	//				either to a new position on the toolbar, or off it 
	//				(to remove it from the toolbar). Double-clicking displays 
	//				the configuration dialog.
	///////////////////////////////////
	void SetBitmapIds(UINT* pIds, int nButtons);

	// if parameter is TRUE then providing that you call function SetBitmapIds() 
	// before, you can customize the CoolToolBar
	void SetCustomizable(BOOL bCustomizable=TRUE, BOOL bAdvanced=TRUE);
	virtual BOOL IsCustomizable(BOOL bAdvanced=TRUE) const 
	{ 
		if(bAdvanced)
			return m_bAdvancedCutomizable;
		else
			return (GetStyle()&CCS_ADJUSTABLE ? TRUE : FALSE); 
	}

#if _MFC_VER>=0x0420
	// set whether CoolToolBar in flat mode or not
	inline void SetFlat(BOOL bFlat=TRUE) { ModifyStyle(bFlat ? 0 : TBSTYLE_FLAT, 
		bFlat ? TBSTYLE_FLAT : 0); }
	// set whether CoolToolBar in list mode or not
	inline void SetList(BOOL bList=TRUE) { ModifyStyle(bList ? 0 : TBSTYLE_LIST, 
		bList ? TBSTYLE_LIST : 0); }
	// set flag m_bCool which defines the way of painting CoolToolBar 
	// in CoolBars
	inline void SetCool(BOOL bCool=TRUE) { m_bCool=bCool; }
	// whether there is a gripper or not
	inline void SetGripper(BOOL bGripper = TRUE)
	{
		if (bGripper)
			SetBarStyle(GetBarStyle() | CBRS_GRIPPER); 
		else
			SetBarStyle(GetBarStyle() & ~CBRS_GRIPPER);
	}
	// whether draw separator in flat mode or not
	inline void SetSeparator(BOOL bSeparator=TRUE) { m_bSeparator=bSeparator; }
	// whether draw dropdown arrow on dropdown buttons or not
	void SetDropDownArrow(BOOL bDropDownArrow=TRUE);

	inline BOOL IsFlat() const { return GetStyle()&TBSTYLE_FLAT ? TRUE : FALSE; }
	inline BOOL IsList() const { return GetStyle()&TBSTYLE_LIST ? TRUE : FALSE; }
	inline BOOL IsCool() const { return m_bCool; }
	inline BOOL IsGripper() { return GetBarStyle() & CBRS_GRIPPER ? TRUE : FALSE; }
	inline BOOL IsSeparator() const { return m_bSeparator; }
	BOOL IsDropDownArrow() const;

	// set / get text color
	// by default we use CLR_DEFAULT color which means that toolbar default color
	// for corresponding state will be used
	inline void SetDefaultTextColor(COLORREF crTextColor) 
	{ 
		m_crDefaultTextColor=crTextColor; 
	}
	inline COLORREF GetDefaultTextColor() const { return m_crDefaultTextColor; }

	inline void SetHotTextColor(COLORREF crTextColor) 
	{ 
		m_crHotTextColor=crTextColor; 
	}
	inline COLORREF GetHotTextColor() const { return m_crHotTextColor; }

	inline void SetSelectedTextColor(COLORREF crTextColor) 
	{ 
		m_crSelectedTextColor=crTextColor; 
	}
	inline COLORREF GetSelectedTextColor() const { return m_crSelectedTextColor; }

	inline void SetCheckedTextColor(COLORREF crTextColor) 
	{ 
		m_crCheckedTextColor=crTextColor; 
	}
	inline COLORREF GetCheckedTextColor() const { return m_crCheckedTextColor; }


	// set / get background color
	// by default we use CLR_DEFAULT color which means that toolbar default color
	// for corresponding state will be used
	inline void SetDefaultBkColor(COLORREF crBkColor) 
	{ 
		m_crDefaultBkColor=crBkColor; 
	}
	inline COLORREF GetDefaultBkColor() const { return m_crDefaultBkColor; }

	inline void SetHotBkColor(COLORREF crBkColor) 
	{ 
		m_crHotBkColor=crBkColor; 
	}
	inline COLORREF GetHotBkColor() const { return m_crHotBkColor; }

	inline void SetSelectedBkColor(COLORREF crBkColor) 
	{ 
		m_crSelectedBkColor=crBkColor; 
	}
	inline COLORREF GetSelectedBkColor() const { return m_crSelectedBkColor; }

	inline void SetCheckedBkColor(COLORREF crBkColor) 
	{ 
		m_crCheckedBkColor=crBkColor; 
	}
	inline COLORREF GetCheckedBkColor() const { return m_crCheckedBkColor; }


	// set / get border color
	// by default we use CLR_DEFAULT color which means that toolbar default color
	// for corresponding state will be used
	inline void SetDefaultBorderColor(COLORREF crBorderColor) 
	{ 
		m_crDefaultBorderColor=crBorderColor; 
	}
	inline COLORREF GetDefaultBorderColor() const { return m_crDefaultBorderColor; }

	inline void SetHotBorderColor(COLORREF crBorderColor) 
	{ 
		m_crHotBorderColor=crBorderColor; 
	}
	inline COLORREF GetHotBorderColor() const { return m_crHotBorderColor; }

	inline void SetSelectedBorderColor(COLORREF crBorderColor) 
	{ 
		m_crSelectedBorderColor=crBorderColor; 
	}
	inline COLORREF GetSelectedBorderColor() const { return m_crSelectedBorderColor; }

	inline void SetCheckedBorderColor(COLORREF crBorderColor) 
	{ 
		m_crCheckedBorderColor=crBorderColor; 
	}
	inline COLORREF GetCheckedBorderColor() const { return m_crCheckedBorderColor; }


	// --- In  : 
	//			 
	// --- Out : 
	// --- Returns : If nonzero, anchor highlighting is enabled. 
	//				 If zero, anchor highlighting is disabled.
	// --- Effect : This member function implements the behavior 
	//				of the Win32 messageTB_GETANCHORHIGHLIGHT, 
	//				as described in the Platform SDK.
	inline BOOL GetAnchorHighlight() const 
	{ 
		ASSERT(::IsWindow(m_hWnd)); 
		return (BOOL)::SendMessage(m_hWnd,TB_GETANCHORHIGHLIGHT,0,0);
	}

	// --- In  : fAnchor - Specifies if anchor highlighting 
	//						is enabled or disabled. 
	//						If this value is nonzero, 
	//						anchor highlighting will be enabled. 
	//						If this value is zero, anchor highlighting 
	//						will be disabled
	//			 
	// --- Out : 
	// --- Returns : Nonzero if successful; otherwise zero.
	// --- Effect : This member function implements the behavior 
	//				of the Win32 messageTB_SETANCHORHIGHLIGHT, 
	//				as described in the Platform SDK.
	inline BOOL SetAnchorHighlight(BOOL fAnchor=TRUE) 
	{ 
		ASSERT(::IsWindow(m_hWnd)); 
		return (BOOL)::SendMessage(m_hWnd,TB_SETANCHORHIGHLIGHT,fAnchor,0); 
	}

	// --- In  :  
	// --- Out : ptbim - A pointer to theTBINSERTMARK structure 
	//					 that contains the insertion mark.
	// --- Returns : 
	// --- Effect : This member function implements the behavior 
	//				of the Win32 messageTB_SETINSERTMARK, 
	//				as described in the Platform SDK.
	inline void GetInsertMark(TBINSERTMARK* ptbim) const 
	{ 
		ASSERT(::IsWindow(m_hWnd)); 
		ASSERT(ptbim!=NULL); 
		::SendMessage(m_hWnd,TB_GETINSERTMARK,0,(LPARAM)ptbim); 
	}

	// --- In  : ptbim - A pointer to theTBINSERTMARK structure 
	//					 that contains the insertion mark. 
	// --- Out : 
	// --- Returns : 
	// --- Effect : This member function implements the behavior 
	//				of the Win32 messageTB_SETINSERTMARK, 
	//				as described in the Platform SDK.
	inline void SetInsertMark(TBINSERTMARK* ptbim) 
	{
		ASSERT(::IsWindow(m_hWnd)); 
		ASSERT(ptbim!=NULL); 
		::SendMessage(m_hWnd,TB_SETINSERTMARK,0,(LPARAM)ptbim); 
	}
	inline void SetInsertMark(int nButtonIndex, DWORD dwFlags=0) 
	{
		ASSERT(::IsWindow(m_hWnd)); 
		TBINSERTMARK tbim; 
		tbim.iButton=nButtonIndex;
		tbim.dwFlags=dwFlags;
		::SendMessage(m_hWnd,TB_SETINSERTMARK,0,(LPARAM)&tbim); 
	}

	// --- In  : nID - The button identifier.
	//			 fHighlight - Specifies the highlight state to be set.
	//						By default, TRUE. If set to FALSE, 
	//						the button is set to its default state. 
	// --- Out : 
	// --- Returns : Nonzero if successful; otherwise zero. 
	// --- Effect : This member function implements the behavior 
	//				of the Win32 message TB_MARKBUTTON, 
	//				as described in the Platform SDK.
	inline BOOL MarkButton(int nID, BOOL fHighlight=TRUE) 
	{
		ASSERT(::IsWindow(m_hWnd)); 
		return (BOOL)::SendMessage(m_hWnd,TB_MARKBUTTON,nID,
			(LPARAM)MAKELONG(fHighlight,0)); 
	}

	// --- In  : nID - The command ID for the toolbar button.
	// --- Out : 
	// --- Returns : Nonzero if successful; otherwise zero. 
	// --- Effect : Call this member function to check 
	//				the highlight state of a toolbar button.
	inline BOOL IsButtonHighlighted(int nID) const 
	{ 
		ASSERT(::IsWindow(m_hWnd)); 
		return (BOOL)::SendMessage(m_hWnd,TB_ISBUTTONHIGHLIGHTED,nID,0); 
	}

#endif

	// --- In  : nOldPos - The zero-based index of the button 
	//						to be moved.
	//			 nNewPos - The zero-based index of the 
	//						button's destination.
	// --- Out : 
	// --- Returns : Nonzero if successful; otherwise zero. 
	// --- Effect : This member function implements the behavior 
	//				of the Win32 messageTB_MOVEBUTTON, 
	//				as described in the Platform SDK.
	BOOL MoveButton(UINT nOldPos, UINT nNewPos);

	// if we want to use non-standard image (not 16x15) we have to initialize
	// sizes of images and buttons to let CoolToolBar to handle them correctly
	inline void IniSizes(CSize csBitmapSize) 
	{ 
		SetSizes(CSize(csBitmapSize.cx+7,csBitmapSize.cy+6),csBitmapSize); 
		GetToolBarCtrl().AutoSize();
	}
#if _MFC_VER>=0x0420
	// we have to call this function to correctly set the size of buttons.
	// usually, this is the last function in the set of functions that we use
	// to initialize CoolToolBar
	inline void UpdateSizes() 
	{ 
		SetSizes(GetButtonsSize(),m_sizeImage); 
		GetToolBarCtrl().AutoSize();
	}
#endif



#if _MFC_VER>=0x0420
	///////////////////
	// Message wrappers
	///////////////////


	// use to change style of CoolToolBar dynamically
	inline void SetStyle(DWORD dwStyle)
	{ 
		ASSERT(::IsWindow(m_hWnd));
		SendMessage(TB_SETSTYLE, 0, (LPARAM)dwStyle); 
	}
	// Get style of bar 
	inline DWORD GetStyle() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd,TB_GETSTYLE,0,(LPARAM)0); 
	}

	// use to change extended style of CoolToolBar dynamically
	inline void SetStyleEx(DWORD dwStyleEx)
	{ 
		ASSERT(::IsWindow(m_hWnd));
		SendMessage(TB_SETEXTENDEDSTYLE, 0, (LPARAM)dwStyleEx); 
	}
	// Get extended style of CoolToolBar 
	inline DWORD GetStyleEx() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (DWORD)::SendMessage(m_hWnd,TB_GETEXTENDEDSTYLE,0,(LPARAM)0); 
	}

	// wParam is the Cmd not index
	// does not work correctly
	// for separator it returns always coordinates of first one

	// you can set image list for buttons for default, disabled and hot conditions
	// using hot image list you can make your CoolToolBar looks like one found
	// in the Internet Explorer. The fastest way to do that just create hot image list
	// in resource editor and set it to CoolToolBar using SetHotImageList(), 
	// you don't have to create default and disabled image lists providing 
	// that you call LoadBitmap() or LoadToolbar() functions before.

	// if the handle to the image list is NULL, then
	// no images will be displayed in the buttons
	inline HIMAGELIST SetDefaultImageList(HIMAGELIST hImageList)
	{ 
		ASSERT(::IsWindow(m_hWnd));
		return (HIMAGELIST)SendMessage(TB_SETIMAGELIST, 0, (LPARAM)hImageList); 
	}
	inline HIMAGELIST GetDefaultImageList() const
	{ 
		ASSERT(::IsWindow(m_hWnd));
		return (HIMAGELIST)::SendMessage(m_hWnd,TB_GETIMAGELIST,0,(LPARAM)0); 
	}
	inline HIMAGELIST SetDisabledImageList(HIMAGELIST hImageList)
	{ 
		ASSERT(::IsWindow(m_hWnd));
		return (HIMAGELIST)SendMessage(TB_SETDISABLEDIMAGELIST, 0, 
			(LPARAM)hImageList); 
	}
	inline HIMAGELIST GetDisabledImageList() const
	{ 
		ASSERT(::IsWindow(m_hWnd));
		return (HIMAGELIST)::SendMessage(m_hWnd,TB_GETDISABLEDIMAGELIST,0,(LPARAM)0); 
	}
	inline HIMAGELIST SetHotImageList(HIMAGELIST hImageList)
	{ 
		ASSERT(::IsWindow(m_hWnd));
		return (HIMAGELIST)SendMessage(TB_SETHOTIMAGELIST,0,(LPARAM)hImageList); 
	}
	inline HIMAGELIST GetHotImageList() const
	{ 
		ASSERT(::IsWindow(m_hWnd));
		return (HIMAGELIST)::SendMessage(m_hWnd,TB_GETHOTIMAGELIST,0,(LPARAM)0); 
	}

	// you can load any bitmap from any loaded application as default image.
	// There is new set of system defined images that you can use in your application
	// IDB_HIST_SMALL_COLOR and IDB_HIST_LARGE_COLOR.
	// To load these images use IDB_XXX as nBitmapID and HINST_COMMCTRL as hInstance
	// !!! The default image list must have already been set using SetDefaultImageList()
	// !!! before use this function
	// returns an INT value that represents the count of images in the image list, 
	// not including the one just added. Returns zero if the toolbar has no 
	// image list or if the existing image list is empty. 
	inline int LoadImage(int nBitmapID, HINSTANCE hInstance)
	{ 
		ASSERT(::IsWindow(m_hWnd));
		return (int)SendMessage(TB_LOADIMAGES,(WPARAM)nBitmapID,(LPARAM)hInstance); 
	}

	// set minimum and maximum width of button
	// very useful when you want to set descriptive text to button
	// !!! Note that like in case with standard SetSizes() function
	// !!! nMaxWidth must be equal or more 
	// !!! than size of bitmap associated with button + 7 points
	// returns nonzero if successful, or zero otherwise
	BOOL SetButtonsMinMaxWidth(UINT nMinWidth,UINT nMaxWidth)
	{ 
		ASSERT(::IsWindow(m_hWnd));
		ASSERT(nMaxWidth-m_sizeImage.cx>=7);
		if(SendMessage(TB_SETBUTTONWIDTH, 0, MAKELPARAM(nMinWidth, nMaxWidth)))
		{
			m_sizeMinMaxWidth.cx=nMinWidth;
			m_sizeMinMaxWidth.cy=nMaxWidth;
			return TRUE;
		}
		return FALSE; 
	}

	// returns CSize object with cx set to min buttons width
	// and cy set to max buttons width
	inline CSize GetButtonsMinMaxWidth() const
	{ 
		ASSERT(::IsWindow(m_hWnd));
		return m_sizeMinMaxWidth; 
	}

	// now you don't have to calculate the size of buttons in CoolToolBar yourself
	// GetButtonSize will do all job. You can use result in SetSizes function
	inline CSize GetButtonsSize() const
	{ 
		ASSERT(::IsWindow(m_hWnd));
		DWORD result=(DWORD)::SendMessage(m_hWnd,TB_GETBUTTONSIZE,0,(LPARAM)0); 
		return CSize(LOWORD(result),HIWORD(result)); 
	}

	// now you can set max number of text rows in the CoolToolBar
	// returns nonzero if successful, or zero otherwise
	inline BOOL SetMaxTextRows(int nRows)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_SETMAXTEXTROWS, nRows, (LPARAM)0); 
	}
	inline int GetTextRows() const
	{
		ASSERT(::IsWindow(m_hWnd));
		return (int)::SendMessage(m_hWnd,TB_GETTEXTROWS,0,(LPARAM)0); 
	}

	// you can set indent for CoolToolBar
	// returns nonzero if successful, or zero otherwise
	inline BOOL SetIndent(int nIndent)
	{
		ASSERT(::IsWindow(m_hWnd));
		if(SendMessage(TB_SETINDENT, nIndent, (LPARAM)0)) 
		{
			m_nIndent=nIndent;
			return TRUE;
		}
		return FALSE;
	}
	inline int GetIndent() const { return m_nIndent; }
#endif

	inline int HitTest(LPPOINT ppt) const 
	{
		ASSERT(::IsWindow(m_hWnd)); 
#if _MFC_VER>=0x0420
		return (int)::SendMessage(m_hWnd, TB_HITTEST, 0, (LPARAM)ppt); 
#else
		return -1;
#endif
	}

	// use this function to associate buttons with text that is 
	// located in corresponding string or menu resources
	inline void SetButtonTextFromID(TextTypeFromID ttID=TTID_TOOLTIP) 
	{ 
		SetButtonTextFromID(0, 0xffff, ttID); 
	}
	
	void SetButtonTextFromID(UINT nFirstButtonID, UINT nLastButtonID, 
		TextTypeFromID ttID=TTID_PLAIN);
	inline TextTypeFromID GetTextType() const { return m_ttID; }

	// save to and load from registry state of buttons in CoolToolBar
	// we use default registry key assigned to your application by MFC
	// make sure you called SetButtonsIds() and 
	// CWinApp::SetRegistryKey() functions before.
	// Makes no sense to load/store buttons state info for uncustomizable CoolToolBar
	//
	////////////////////////
	// if bProperties is TRUE you can save/load next CoolToolBar properties:
	//		
	//		modes: cool, flat, list, customizable
	//		draw properties: gripper, separator
	//		size of the indent
	//		hot and default text color
	//		text type
	//		max text rows
	//		min/max buttons width
	////////////////////////
	virtual BOOL SaveBarState(LPCTSTR lpszSubKey, LPCTSTR lpszValueName, 
		BOOL bProperties=TRUE);
	virtual BOOL LoadBarState(LPCTSTR lpszSubKey, LPCTSTR lpszValueName, 
		BOOL bProperties=TRUE);

	// dock CoolToolBar to the left side of another docked ToolBar
	// if use NULL as parameter then this function work just as common
	// CFrameWnd::DockControlBar()
	void DockControlBarLeftOf(CToolBar *leftOf=NULL);

	inline DWORD GetComCtlVersion() const { return m_dwComCtlVersion; }

#if _MFC_VER<=0x0421
	void SetHeight(int cyHeight);
#endif

	// --- In  :	nIconID		-	resource ID of the icon
	//				lpszIcon	-	resource string of the icon
	//				hIcon		-	handle to the icon. If NULL then 
	//								current toolbar icon will be removed
	//				szIcon		-	size of the icon
	//				hInstance	-	handle of the module that owns the
	//								specified icon resource
	//				bRedraw		-	flag that specifies whether to redraw 
	//								the toolbar after setting new icon or not
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets/removes icon to be displayed in the toolbar
	inline void SetIcon(UINT nIconID, 
		CSize szIcon=CSize(::GetSystemMetrics(SM_CXSMICON),
		::GetSystemMetrics(SM_CYSMICON)), HINSTANCE hInstance=NULL, BOOL bRedraw=TRUE) 
	{
		SetIcon(MAKEINTRESOURCE(nIconID),szIcon,hInstance,bRedraw);
	}
	
	inline void SetIcon(LPCTSTR lpszIcon, 
		CSize szIcon=CSize(::GetSystemMetrics(SM_CXSMICON), 
		::GetSystemMetrics(SM_CYSMICON)), HINSTANCE hInstance=NULL, 
		BOOL bRedraw=TRUE) 
	{
		if(hInstance==NULL)
			hInstance=AfxFindResourceHandle(lpszIcon,RT_ICON);
		if(hInstance!=NULL)
		{
			HICON hIcon=(HICON)::LoadImage(hInstance,lpszIcon,
				IMAGE_ICON,szIcon.cx,szIcon.cy,LR_DEFAULTCOLOR);
			if(hIcon)
				SetIcon(hIcon,bRedraw);
		}
	}
	inline void SetIcon(HICON hIcon, BOOL bRedraw=TRUE) 
	{
		if(m_hIcon!=hIcon)
		{
			m_iconRect.SetRectEmpty();
			m_hIcon=hIcon;
			if(bRedraw)
			{
				RedrawToolBar(TRUE,TRUE);
			}
		}
	}

	// --- In  :
	// --- Out : 
	// --- Returns:	handle to the toolbar icon or NULL if there is none set
	// --- Effect : Retrieves icon that is displayed in the toolbar
	inline HICON GetIcon() const { return m_hIcon; }


	// --- In  :	bRecalcLayout	-	if TRUE then docking frame will
	//									recalculate its layout
	//				bOnlyFrame		-	if TRUE then only non-client area of 
	//									the control will be redrawn
	// --- Out : 
	// --- Returns:	
	// --- Effect : Redraws the toolbar
	void RedrawToolBar(BOOL bRecalcLayout=TRUE, BOOL bOnlyFrame=FALSE);
	inline void DelayRedrawToolBar() { m_nIdleFlags|=oxidleRedrawToolbar; }


	// --- In  :	nIndex	-	index of the button to redraw
	// --- Out : 
	// --- Returns:	
	// --- Effect : Redraws the specified button in a toolbar
	void RedrawButton(int nIndex);


	// --- In  :	nButtonIndex	-	toolbar button index
	// --- Out : 
	// --- Returns:	tooltip text for the specified toolbar button
	// --- Effect : Retrieves the tooltip text for the specified toolbar button
	CString GetButtonTooltip(int nButtonIndex) const;

	// --- In  :	
	// --- Out :	
	// --- Returns:
	// --- Effect : Enables/disables floating of the menu bar
	void EnableFloating(BOOL bEnable = TRUE);

	// --- In  :	
	// --- Out :	
	// --- Returns: TRUE if floating is enabled, FALSE otherwize
	// --- Effect : Tell wheather or not floating is enabled
	BOOL IsFloatingEnabled();

	inline BOOL IsButtonDragged(int nIndex) const 
	{ 
		return (nIndex==m_nDraggedButtonIndex);
	}

	inline void SetDraggedButton(int nIndex) { m_nDraggedButtonIndex=nIndex; }
	inline int GetDraggedButton() const { return m_nDraggedButtonIndex; }


	inline BOOL IsButtonCustomized(int nIndex) const 
	{ 
		return (nIndex==m_nCustomizedButtonIndex);
	}

	int SetCustomizedButton(int nIndex);
	inline int GetCustomizedButton() const { return m_nCustomizedButtonIndex; }

	inline BOOL IsInAdvancedCustomizationMode() const 
	{ 
		return m_bInAdvancedCustomizationMode; 
	}

	void SetAdvancedCustomizationMode(BOOL bInAdvancedCustomizationMode,
		HWND hWndCustomizeOrganizer=NULL); 


	virtual BOOL DisplayCustomizeButtonContextMenu(int nButtonIndex, CPoint point);

	CString GetRecentButtonText(UINT nCommandID);

	// override to provide your own COleDropSource to handle drag'n'drop operation
	virtual COleDropSource* GetDropSource() 
	{ 
		m_oleDropSource.SetOwner(this);
		return &m_oleDropSource; 
	}
	// override to provide your own COleDropTarget to handle drag'n'drop operation
	virtual COleDropTarget* GetDropTarget() { return &m_oleDropTarget; }


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

protected:

	// Creates custom button. If custom button with specified index has been 
	// already created in one of the toolbars defined in application then this 
	// function will create a copy that will duplicate the original button
	BOOL CreateCustomButton(int nCustomButtonIndex, 
		OXCUSTOMBUTTONDESCRIPTOR& descriptor, 
		DWORD dwStyle=WS_CHILD|WS_VISIBLE, DWORD dwStyleEx=0, 
		LPCTSTR lpszClassName=AfxRegisterWndClass(CS_DBLCLKS),
		LPCTSTR lpszWindowName=_T(""));

	// displays custom button at the specified position. The current custom button
	// settings will be applied
	BOOL ShowCustomButton(int nIndex);

	// Returns a pointer to the toolbar skin
	COXToolbarSkin* GetToolbarSkin();


#if _MFC_VER>=0x0420
	void _GetButton(int nIndex, TBBUTTON* pButton) const;
	void _SetButton(int nIndex, TBBUTTON* pButton);
	virtual CSize CalcLayout(DWORD nMode, int nLength = -1);
	virtual CSize CalcSize(TBBUTTON* pData, int nCount);
	virtual int WrapToolBar(TBBUTTON* pData, int nCount, int nWidth);
	virtual void SizeToolBar(TBBUTTON* pData, int nCount, 
		int nLength, BOOL bVert=FALSE);

	// virtual functions for drawing gripper and separators
	// if you don't like the way we draw them then you can 
	// use your own algorithm
	virtual void DrawGripper(CDC& dc, CRect& rect);
	virtual void DrawSeparator(CDC& dc, CRect& rc);
	// virtual functions for drawing icon
	virtual void DrawIcon(CDC& dc, CRect& rect);
	// virtual functions for drawing icon
	virtual void DrawCustomizedButton(CDC& dc, CRect& rect);
	// virtual function for designating some space
	virtual void BookSpace(CRect& rectBookedSpace, DWORD dwMode) 
	{
		UNREFERENCED_PARAMETER(rectBookedSpace);
		UNREFERENCED_PARAMETER(dwMode);
	}
	// Draw in Booked space
	virtual void DrawInBookedSpace(CDC& dc, CRect& rectToDrawIn) 
	{ 
		UNREFERENCED_PARAMETER(dc);
		UNREFERENCED_PARAMETER(rectToDrawIn);
	};
#endif

	// function called everytime when toolbar changes its state from
	// docking to floating and vise versa
	virtual void OnFloatingDocking(BOOL bFloating) 
	{
		UNREFERENCED_PARAMETER(bFloating);
		if(m_mapCustomButtons.GetCount()>0)
		{
			RedrawToolBar(TRUE,TRUE);
		}
	}

	// function called everytime when toolbar changes its docking side
	virtual void OnChangeDockSide(DWORD dwDockSide) 
	{
		UNREFERENCED_PARAMETER(dwDockSide);
		if(m_mapCustomButtons.GetCount()>0)
		{
			DelayRedrawToolBar();
		}
	}

	LRESULT SendCustomizeNotification(UINT nCustomizeCmdID, LPARAM lParam=NULL);
	void SaveMouseOffset(CPoint point);

// Implementation
public:
	virtual ~COXCoolToolBar();

	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
#if _MFC_VER>=0x0420
	// we have to overwrite this function from CToolBar class
	// to draw non-client area of CoolToolBar when it is used in CoolToolBar
	virtual void EraseNonClient();

	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);
	virtual void CalcInsideRect(CRect& rect, BOOL bHorz);
#endif

	// Generated message map functions
protected:
	// overwrite standard handlers to overcome some problems with MFC
	// standard painting routine
	//{{AFX_MSG(COXCoolToolBar)
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	afx_msg LRESULT OnInsertButton(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteButton(WPARAM wParam, LPARAM lParam);
#if _MFC_VER>=0x0420
	afx_msg LRESULT OnMoveButton(WPARAM wParam, LPARAM lParam);
#endif

	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam);

	// reflect the messages to let the CoolToolBar provide 
	// customization functionality
	afx_msg BOOL OnTBNBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnTBNEndDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnTBNQueryInsert(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnTBNQueryDelete(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnTBNToolBarChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnTBNGetButtonInfo(NMHDR* pNMHDR, LRESULT* pResult);

	// drag and drop support
	virtual LONG OnDragEnter(WPARAM wParam, LPARAM lParam);
	virtual LONG OnDragOver(WPARAM wParam, LPARAM lParam);
	virtual LONG OnDragLeave(WPARAM wParam, LPARAM lParam);
	virtual LONG OnDrop(WPARAM wParam, LPARAM lParam);

	// advanced customization commands handlers
	afx_msg void OnCustTBDelete();
	afx_msg void OnCustTBAppearance();
	afx_msg void OnCustTBImageOnly();
	afx_msg void OnCustTBImageText();
	afx_msg void OnCustTBSeparatorBefore();
	afx_msg void OnCustTBSeparatorAfter();

	// handler that is called when button is to be removed due to to 
	// drag'n'drop operation
	virtual void OnRemoveDraggedButton(int nButtonIndex);


#if _MFC_VER>=0x0420
	// reflect custom draw notification
	afx_msg void OnCustomDraw(NMHDR* pNotifyStruct, LRESULT* result );
#endif

	BOOL OnDropDownButton(NMHDR* pNMHDR, LRESULT* result);

	DECLARE_MESSAGE_MAP()

private:
	int m_iDropDownIndex;
	int m_iLastDropDownIndex;
	CPoint m_ptOffset;
	COXToolbarSkin* m_pToolbarSkin;
	// helper function to find button in m_pBitmapIds array
	int FindBitmapIndex(UINT nID) const;
	// helper function to define the version of comctl32.dll
	// if succeed it sets major number of version to pdwMajor and
	// minor number to pdwMinor
	HRESULT GetComCtlVersion(LPDWORD pdwMajor, LPDWORD pdwMinor) const;

public:
	void EnableDocking(DWORD dwDockStyle, BOOL bSnapWhileDragging = TRUE);
	// helper function to define whether WindowsNT >=4.0 is running
	BOOL IsWindowsNTRunning();
	// helper function to define the size dropdown arrow 
	int GetDropDownArrowWidth() const;
#if _MFC_VER >= 0x0700
	virtual void DrawBorders(CDC* pDC, CRect& rect);
#endif
};

/////////////////////////////////////////////////////////////////////////////
#endif //_COOLTOOLBAR_H
