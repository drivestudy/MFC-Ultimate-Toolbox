// ====================================================================================
//								Class Specification : 
//					COXSHBEdit & COXSHBListCtrl & COXShortcutBar
// ====================================================================================

// Header file : OXShortcutBar.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
//////////////////////////////////////////////////////////////////////////////
                         
#if !defined(_OX_SHORTCUTBAR_)
#define _OX_SHORTCUTBAR_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#define __AFXTEMPL_H__
#endif

#ifndef __AFXOLE_H__
#include <afxole.h>         // MFC OLE classes
#define __AFXOLE_H__
#endif

#ifndef _WIN32_IE
#if _MFC_VER>=0x0420
#define _WIN32_IE 0x0300
#else
#define _WIN32_IE 0x0200
#endif
#endif

#if (_WIN32_IE < 0x0400)
#define LVS_EX_FLATSB           0x00000100
#define LVS_EX_REGIONAL         0x00000200
#define LVS_EX_INFOTIP          0x00000400 // listview does InfoTips for you
#define LVS_EX_UNDERLINEHOT     0x00000800
#define LVS_EX_UNDERLINECOLD    0x00001000
#define LVS_EX_MULTIWORKAREAS   0x00002000
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

// IE 4.0 uses comctl32.dll version 4.71
// i.e. major=4, minor=71 
// then we will get _IE40_COMCTL_VERSION as MAKELONG(minor,major) 
#define _IE40_COMCTL_VERSION 0x00040047


// for drag and drop support
#include "OXDragDropSupport.h"


// determine number of elements in an array (not bytes)
#define countof(array) (sizeof(array)/sizeof(array[0]))

// Default values
//
// text format used to draw text in group headers
#define DFLT_HDRTEXTFMT				DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS
// header height
#define DFLT_HDRHEIGHT				18
#define DFLT_HDRHEIGHTXP			18
#define DFLT_HDRHEIGHT2003			32
#define DFLT_TOPHDRHEIGHT2003		25

// margin between 2 groups
#define DFLT_GROUPMARGIN			0
// left margin for child window
#define DFLT_LEFTMARGIN				5
// right margin for child window
#define DFLT_RIGHTMARGIN			5
// top margin for child window
#define DFLT_TOPMARGIN				5
// bottom margin for child window
#define DFLT_BOTTOMMARGIN			5

// mininmum size of control
#define DFLT_MINHEIGHT				20
#define DFLT_MINWIDTH				20

// scroll button size
#define DFLT_SCROLLBUTTONWIDTH		16
#define DFLT_SCROLLBUTTONHEIGHT		16

// delay in milliseconds of scrollind (normal and autoscroll)
#define DFLT_SCROLLINGDELAY			200
#define DFLT_AUTOSCROLLINGDELAY		400

// delay in milliseconds of waiting for drag header event
#define DFLT_WAITFORDRAGHEADER		100

// delay in milliseconds of waiting for group auto expand event
#define DFLT_WAITFORAUTOEXPAND		800

// delay in milliseconds of checking for which group header the mouse cursor is over
#define DFLT_CHECKMOUSEISOVERGROUP	500

// default offset (number of pixels) used to animate group expanding
#define DFLT_ANIMATIONOFFSET		20
// default delay (in milliseconds) between redrawing while animating group expansion
#define DFLT_ANIMATIONDELAY			100


// constants used to display list control's items
//
// margin between items in list control (must be an odd number)
#define ID_ITEMMARGIN				6
// margins between the first and the last list control items rectangles
// and list control client window
#define ID_EDGELEFTMARGIN			0
#define ID_EDGETOPMARGIN			2
#define ID_EDGERIGHTMARGIN			0
#define ID_EDGEBOTTOMMARGIN			2
// margins for autoscrolling during drag and drop operation
#define ID_DRAGDROPLEFTMARGIN		0
#define ID_DRAGDROPTOPMARGIN		8
#define ID_DRAGDROPRIGHTMARGIN		0
#define ID_DRAGDROPBOTTOMMARGIN		8

// margin between item image and item text for both large and small icons views
#define ID_IMAGETEXTVERTMARGIN		0
#define ID_IMAGETEXTHORZMARGIN		0

// adjustment that should be applied to display text in edit control
#define ID_TEXTLEFTMARGIN			2
#define ID_TEXTRIGHTMARGIN			2
#define ID_TEXTTOPMARGIN			2
#define ID_TEXTBOTTOMMARGIN			2
// minimum width of edit control to display text
#define ID_EDITMINWIDTH				10

// height of place holder arrow (must be uneven)
#define ID_HEIGHTPLACEHOLDER		15

// ID of timer used to scroll list control contents
#define ID_SCROLLINGTIMER			1
// ID of timer used to wait for drag header event
#define ID_WAITFORDRAGHEADERTIMER	2
// ID of timer used to wait for group auto expand event
#define ID_WAITFORAUTOEXPANDTIMER	3
// ID of timer used to check which group header the mouse cursor is over
#define ID_CHECKMOUSEISOVERGROUP	4

// undefined color
#define ID_COLOR_NONE				0xffffffff


// SHBE_DISPLAY structure can be used as parameter in COXSHBEdit::StartEdit function
// that defines options to display and edit text within COXSHBEdit edit control.
// Below you will find an explanation of meanings of all items:
//
//		sText			-	text to be displayed and edited
//		rectDisplay		-	edit control window's rect	
//		rectMax			-	max rect to which edit control window's can be enlarged
//		dwStyle			-	style that should be applied to edit control 
//		bEnlargeAsTyping-	enlarge edit control window to show all typed text
//		ptSelRange		-	selection range
//		pFont			-	font to set to edit control
//		rectMin			-	min rect to display in edit control
//		clrText			-	color used to draw text
//		clrBackground	-	color used to fill background
//		nMask			-	mask flags that indicate which of the other structure members 
//								contain valid data. Can be a combination of these flags:
//								SHBEIF_TEXT			-	sText is valid
//								SHBEIF_RECTDISPLAY	-	rectDisplay is valid
//								SHBEIF_RECTMAX		-	rectMax is valid
//								SHBEIF_STYLE		-	dwStyle is valid
//								SHBEIF_ENLARGE		-	bEnlargeAsTyping is valid
//								SHBEIF_SELRANGE		-	ptSelRange is valid
//								SHBEIF_FONT			-	pFont is valid
//								SHBEIF_RECTMIN		-	rectMin is valid
//								SHBEIF_CLRTEXT		-	rectMin is valid
//								SHBEIF_CLRBACK		-	rectMin is valid
//
typedef struct _tagSHBE_DISPLAY
{
	LPCTSTR lpText;
	CRect rectDisplay;
	CRect rectMax;
	DWORD dwStyle;
	BOOL bEnlargeAsTyping;
	CPoint ptSelRange;
	CFont* pFont;
	CRect rectMin;
	COLORREF clrText;
	COLORREF clrBack;
	UINT nMask;

	_tagSHBE_DISPLAY()
	{
		ZeroMemory(this,sizeof(_tagSHBE_DISPLAY));
	}

	// note that we don't allocate memory for text and font 
	// we save in this structure only temporary points
	void Copy(_tagSHBE_DISPLAY* pSHBED)
	{
		lpText=pSHBED->lpText;
		rectDisplay=pSHBED->rectDisplay;
		rectMax=pSHBED->rectMax;
		dwStyle=pSHBED->dwStyle;
		bEnlargeAsTyping=pSHBED->bEnlargeAsTyping;
		ptSelRange=pSHBED->ptSelRange;
		pFont=pSHBED->pFont;
		rectMin=pSHBED->rectMin;
		nMask=pSHBED->nMask;
		clrText=pSHBED->clrText;
		clrBack=pSHBED->clrBack;
	}

} SHBE_DISPLAY, * LPSHBE_DISPLAY;

// Mask flags of SHBE_DISPLAY structure
#define SHBEIF_TEXT					0x00000001
#define SHBEIF_RECTDISPLAY			0x00000002
#define SHBEIF_RECTMAX				0x00000004
#define SHBEIF_STYLE				0x00000008
#define SHBEIF_ENLARGE				0x00000010
#define SHBEIF_SELRANGE				0x00000020
#define SHBEIF_FONT					0x00000040
#define SHBEIF_RECTMIN				0x00000080
#define SHBEIF_CLRTEXT				0x00000100
#define SHBEIF_CLRBACK				0x00000200

// Shortcut Bar Edit control specific notifications
//
// Notifies parent window that editing of text was finished. As lParam we send
// NMSHBEDIT structure.
//
//SHBEN_FINISHEDIT			

// Structure that is used with notifications that shortcut bar edit control sends to 
// its parent window
typedef struct _tagNMSHBEDIT
{   
	// standard header structure
	NMHDR hdr;
	// set to FALSE if editing was cancelled
    BOOL bOK;

	_tagNMSHBEDIT()
	{
		ZeroMemory(this,sizeof(_tagNMSHBEDIT));
	}

} NMSHBEDIT, * LPNMSHBEDIT;


// SHB_DESCRIPTOR structure can be associated with any group within ShortcutBar.
// Below you will find an explanation of meanings of all items:
//
//		clrBackground		-	background color of list control
//		clrText				-	color used to draw text in list control
//		clrHeaderBackground	-	background color of header control
//		clrHeaderText		-	color used to draw text in header control
//		nHeaderTextFormat	-	text format used to display group name in header control. 
//								Refer to DrawText function to get full description of all
//								available formats.
//		lfText				-	font to draw items in list control
//		lfHeader			-	font to draw items in header control
//		nHeaderHeight		-	height of header control
//		nMask				-	mask flags that indicate which of the other structure members 
//								contain valid data. Can be a combination of these flags:
//								SHBIF_CLRBACK		-	clrBackground is valid
//								SHBIF_CLRTEXT		-	clrText is valid
//								SHBIF_HDRCLRBACK	-	clrHeaderBackground is valid
//								SHBIF_HDRCLRTEXT	-	clrHeaderText is valid
//								SHBIF_HDRTEXTFMT	-	nHeaderTextFormat is valid
//								SHBIF_FONT			-	lfText is valid
//								SHBIF_HDRFONT		-	lfHeader is valid
//								SHBIF_HDRHEIGHT		-	nHeaderHeight is valid
//
typedef struct _tagSHB_DESCRIPTOR
{
public:
	COLORREF clrBackground;
	COLORREF clrText;
	COLORREF clrHeaderBackground;
	COLORREF clrHeaderText;
	UINT nHeaderTextFormat;
	LOGFONT lfText;
	LOGFONT lfHeader;
	int nHeaderHeight;
	UINT nMask;

	_tagSHB_DESCRIPTOR()
	{
		ZeroMemory(this,sizeof(_tagSHB_DESCRIPTOR));
	}

} SHB_DESCRIPTOR, * LPSHB_DESCRIPTOR;

// Mask flags of SHB_DESCRIPTOR structure
#define SHBIF_CLRBACK				0x00000001
#define SHBIF_CLRTEXT				0x00000002
#define SHBIF_HDRCLRBACK			0x00000004
#define SHBIF_HDRCLRTEXT			0x00000008
#define SHBIF_HDRTEXTFMT			0x00000010
#define SHBIF_FONT					0x00000020
#define SHBIF_HDRFONT				0x00000040
#define SHBIF_HDRHEIGHT				0x00000080


// Types of view of items within list controls
//
// Icons are large, text is under image, item is centered.
#define SHB_LARGEICON				0x00000001
// Icons are small, text is at the right side of image, item is left aligned.
#define SHB_SMALLICON				0x00000002

// SHB_GROUPINFO structure is designed to provide full info about any group within 
// ShortcutBar. Below you will find an explanation of meanings of all items:
//
//		pszText				-	group name
//		nTextMax			-	max length of pszText
//		nImage				-	order of image from associated image list used to display
//								on header control when group is not expanded
//		nImageExpanded		-	order of image from associated image list used to display
//								on header control when group is expanded
//		lParam				-	user-defined data associated with group
//		nOrder				-	Order in which the group appears within the Shorcut Bar, from 
//								top to bottom. That is, the value for the top group is 0. The 
//								value for the next group to the bottom is 1, and so on. While 
//								using as parameter of InsertGroup function can be also one of:
//								SHBI_FIRST		-	Inserts the group at the beginning of the 
//													list.
//								SHBI_LAST		-	Inserts the group at the end of the list.
//								SHBI_SORT		-	Inserts the group into the list in 
//													alphabetical order.
//		nView				-	type of viewing items in list control, can be on of:
//								SHB_LARGEICON
//								SHB_SMALLICON
//		pDescriptor			-	pointer to SHB_DESCRIPTOR structure that provides additional 
//								customization information on the group's appearance
//		hwndChild			-	handle of window to display when the group is expanded. By 
//								default we create list control to display as child window
//								and provide the functionality to fill it with items and 
//								manipulate them. If hwndChild is not NULL then we won't create 
//								corresponding list control and all the user interface 
//								interpretation is up to the programmer.
//		nMask				-	mask flags that indicate which of the other structure members 
//								contain valid data. Can be a combination of these flags:
//								SHBIF_TEXT			-	pszText and nTextMax is valid
//								SHBIF_IMAGE			-	nImage is valid
//								SHBIF_IMAGEEXPANDED	-	nImageExpanded is valid
//								SHBIF_PARAM			-	lParam is valid
//								SHBIF_ORDER			-	nOrder is valid
//								SHBIF_VIEW			-	nView is valid
//								SHBIF_DESCRIPTOR	-	pDescriptor is valid
//								SHBIF_CHILDWND		-	hwndChild is valid
//								
//
typedef struct _tagSHB_GROUPINFO
{
public:
	LPTSTR pszText;
	int nTextMax;
	int nImage;
	int nImageExpanded;
	LPARAM lParam;
	int nOrder;
	int	nView;
	LPSHB_DESCRIPTOR pDescriptor;
	HWND hwndChild;
	UINT nMask;

private:
	// Flag for dynamically created pszText
	BOOL bDynAlloc;

public:
	_tagSHB_GROUPINFO()
	{
		ZeroMemory(this,sizeof(_tagSHB_GROUPINFO));
	}

	~_tagSHB_GROUPINFO()
	{
		if(bDynAlloc && pszText!=NULL)
		{
			delete[] pszText;
			pszText = NULL;
		}
	}

	// call this function if pszText was dynamically created using new operator
	void DynCreated(BOOL bDynCreated = TRUE) 
	{
		bDynAlloc=bDynCreated;
	}

	BOOL IsDynCreated() 
	{
		return bDynAlloc;
	}

} SHB_GROUPINFO, * LPSHB_GROUPINFO;

// Mask flags of SHB_GROUPINFO structure
#define SHBIF_TEXT					0x00010000
#define SHBIF_IMAGE					0x00020000
#define SHBIF_IMAGEEXPANDED			0x00040000
#define SHBIF_PARAM					0x00080000
#define SHBIF_ORDER					0x00100000
#define SHBIF_VIEW					0x00200000
#define SHBIF_DESCRIPTOR			0x00400000
#define SHBIF_CHILDWND				0x00800000

// Flags that can be used as SHB_GROUPINFO::nOrder in InsertGroup function
//
// Group will be inserted at top
#define SHBI_FIRST					0xffffffff
// Group will be inserted at bottom
#define SHBI_LAST					0xfffffffe
// Group will be inserted in alphabetical order
#define SHBI_SORT					0xfffffffd



// Define handle to group
//
// We refer to any inserted group by its handle
typedef HANDLE HSHBGROUP;


// Shortcut Bar specific styles
// 
// In Create function you can specify any of following styles. Note that any of that style 
// can be set/unset using SetBarStyle function.
//
// shortcut bar won't send SHBN_BEGINDRAGITEM notification
#define SHBS_DISABLEDRAGITEM		0x00000001
// shortcut bar won't accept any OLE drag'n'drop object
#define SHBS_DISABLEDROPITEM		0x00000002
// shortcut bar won't support OLE drag'n'drop
#define SHBS_DISABLEDRAGDROPITEM	SHBS_DISABLEDRAGITEM|SHBS_DISABLEDROPITEM
// group headers can be edited
#define SHBS_EDITHEADERS			0x00000004
// item text can be edited
#define SHBS_EDITITEMS				0x00000008
// SHBN_GETHEADERINFOTIP and SHBN_GETITEMINFOTIP notifications will be fired
#define SHBS_INFOTIP				0x00000010
// scroll buttons won't be displayed in child windows
#define SHBS_NOSCROLL				0x00000020
// text in expanded group header will be drawn using bold font
#define SHBS_BOLDEXPANDEDGROUP		0x00000040
// text of hot item in expanded group child window will be drawn using underline font
#define SHBS_UNDERLINEHOTITEM		0x00000080
// active item in expanded group child window will be shawn even if the shortcut bar 
// doesn't have mouse input
#define SHBS_SHOWACTIVEALWAYS		0x00000100
// shortcut bar won't send SHBN_BEGINDRAGHEADER notification
#define SHBS_DISABLEDRAGDROPHEADER	0x00000200
// draws header image while dragging it 
#define SHBS_DRAWHEADERDRAGIMAGE	0x00000400
// draws item image while dragging it 
#define SHBS_DRAWITEMDRAGIMAGE		0x00000800
// scrolling will be run automatically when mouse is over scrolling buttons
#define SHBS_AUTOSCROLL				0x00001000
// animation effect will be applied while expanding a group
#define SHBS_ANIMATEEXPAND			0x00002000
// auto expansion of the group if mouse is over group header for some time
#define SHBS_AUTOEXPAND				0x00004000
// shortcut bar will display flat group buttons
#define SHBS_FLATGROUPBUTTON		0x00008000



// Structure that is used with notifications that shortcut bar sends to its parent
//
typedef struct _tagNMSHORTCUTBAR
{
    // standard header
	NMHDR hdr;
	// handle to the group
    HSHBGROUP hGroup;
	// handle to the item
	int nItem;
	// additional info, depends on notification
	LPARAM lParam;

	_tagNMSHORTCUTBAR()
	{
		::ZeroMemory(this,sizeof(_tagNMSHORTCUTBAR));
		nItem=-1;
	}

} NMSHORTCUTBAR, * LPNMSHORTCUTBAR;

// Shortcut Bar specific notifications. We set lParam to NMSHORTCUTBAR structure. 
//

// Group oriented notifications
//
// Notifies that header is about to be draged. lParam of NMSHORTCUTBAR points to the point
// of mouse cursor. If you handle this notification return non-zero value to not allow to
// run default drag header implementation.
//
//SHBN_BEGINDRAGHEADER		
//
// Notifies that header text is about to be edited. lParam of NMSHORTCUTBAR is LPCTSTR to the
// header text. If you handle this notification return non-zero value to not allow to
// run default editing text header implementation.
//
//SHBN_BEGINHEADEREDIT		
//
// Notifies that group is about to be deleted. If you handle this notification return 
// non-zero value to not allow to run default delete group implementation.
//
//SHBN_DELETEGROUP			
//
// Notifies that header text editing was finished. lParam of NMSHORTCUTBAR is LPCTSTR to the
// updated text. If you handle this notification return non-zero value to not allow to
// run default updating text header implementation.
//
//SHBN_ENDHEADEREDIT			
//
// Notifies that tooltip for group header is about to be displayed. lParam of 
// NMSHORTCUTBAR points to SHBINFOTIP structure which is described below. If you handle 
// this notification fill the SHBINFOTIP structure correspondingly to display the text 
// you need
//
//SHBN_GETHEADERINFOTIP		
//
// Notifies that group was just expanded.
//
//SHBN_GROUPEXPANDED			
//
// Notifies that group is about to be expanded. If you handle this notification return 
// non-zero value to not allow to run default group expanding implementation.
//
//SHBN_GROUPEXPANDING			
//
// Notifies that header is about to be drawn. lParam of NMSHORTCUTBAR points to the 
// corresponding DRAWITEMSTRUCT used to draw header. Refer to the source code of DrawHeader
// function to detailes on filling this structure. If you handle this notification return 
// non-zero value to not allow to run default header drawing implementation.
//
//SHBN_DRAWHEADER				
//
// Notifies that header was draged and droped onto another group. lParam is the handle
// to drop target group.
//
//SHBN_DROPGROUP				
//
// Notifies that new group is about to be inserted. lParam of NMSHORTCUTBAR points to the 
// corresponding SHB_GROUPINFO used to create group. If you handle this notification return 
// non-zero value to not allow to run default group inserting implementation.
//
//SHBN_INSERTGROUP			



// Child window oriented notifications
//
// Notifies that item is about to be draged. lParam of NMSHORTCUTBAR points to the point
// of mouse cursor. If you handle this notification return non-zero value to not allow to
// run default drag item implementation.
//
//SHBN_BEGINDRAGITEM			
//
// Notifies that item text is about to be edited. lParam of NMSHORTCUTBAR is LPCTSTR to the
// item text. If you handle this notification return non-zero value to not allow to
// run default editing item text implementation.
//
//SHBN_BEGINITEMEDIT			
//
// Notifies that all items is about to be deleted. 
//
//SHBN_DELETEALLITEMS			
//
// Notifies that item is about to be deleted. 
//
//SHBN_DELETEITEM			
//
// Notifies that item text editing was finished. lParam of NMSHORTCUTBAR is LPCTSTR to the
// updated text. If you handle this notification return non-zero value to not allow to
// run default updating item text implementation.
//
//SHBN_ENDITEMEDIT			
//
// Notifies that tooltip for child window is about to be displayed. lParam of 
// NMSHORTCUTBAR points to SHBINFOTIP structure which is described below. If you handle 
// this notification fill the SHBINFOTIP structure correspondingly to display the text 
// you need
//
//SHBN_GETITEMINFOTIP			
//
// Notifies that new item is about to be inserted. lParam of NMSHORTCUTBAR points to 
// corresponding LV_ITEM structure that specifies all parameters of new item.
//
// SHBN_INSERTITEM				
//
// Notifies that item is about to be selected. If you handle this notification return 
// non-zero value to not allow to run default item selecting implementation.
//
//SHBN_SELECTITEM				
//
// Notifies that item is about to be activated. If you handle this notification return 
// non-zero value to not allow to run default item activatting implementation.
//
// SHBN_ACTIVATEITEM			
//
// Notifies that item is about to be set as hot. If you handle this notification return 
// non-zero value to not allow to run default item hot setting implementation.
//
// SHBN_HOTITEM				
//
// Notifies that item is about to be drawn. lParam of NMSHORTCUTBAR points to the 
// corresponding DRAWITEMSTRUCT used to draw item. If you handle this notification return 
// non-zero value to not allow to run default item drawing implementation.
//
// SHBN_DRAWITEM				
//
// Notifies that child window or shortcut bar is about to register its COleDropTarget 
// object. lParam of NMSHORTCUTBAR points to the NULL and if COleDropTarget is 
// requested for child window then hGroup of NMSHORTCUTBAR is handle of corresponding 
// group or if COleDropTarget is requested for shortcut bar then hGroup of 
// NMSHORTCUTBAR is NULL. If you handle this notification and would like to provide 
// your own COleDropTarget object then you must return non-zero value and set lParam 
// of NMSHORTCUTBAR structure to point to that COleDropTarget object.
//
// SHBN_GETDROPTARGET			
//
// Notifies that child window is about to start drag'n'drop operation. lParam of
// NMSHORTCUTBAR points to the NULL. If you handle this notification and would like 
// to provide your own COleDropSource object to start drag'n'drop operation then you 
// must return non-zero value and set to lParam of NMSHORTCUTBAR structure to point 
// to that COleDropSource object
//
// SHBN_GETDROPSOURCE			
//
// Notifies that drag and drop item operation in child window has started. lParam of
// NMSHORTCUTBAR points to the COleDataSource object used to accomplish the operation. 
// If you handle this notification and would like to provide your own drag'n'drop 
// implementation then you must return non-zero value and set up the drag image, call 
// DoDragDrop and clean up after the drag drop operation finishes (you have to delete 
// COleDataSource object).
//
// SHBN_HANDLEDRAGITEM			
//
// Before dragging child window item we save some info about it in order to create 
// the full copy of the this item in drop target. Programmer might would like to 
// specify additional information to be saved in drag object in order to handle the
// drop event and inserting of new item. To accomplish this task child window will send
// SHBN_GETADDITIONALDROPINFO notification before saving dragged object info. lParam of 
// NMSHORTCUTBAR will be set to SHBADDITIONALDROPINFO structure so programmer can set 
// pointer to additional info and the size of the info in bytes. After inserting of new 
// item in result of drop event the COXShortcutBar child window will send 
// SHBN_SETADDITIONALDROPINFO notification where lParam of NMSHORTCUTBAR will be 
// set to SHBADDITIONALDROPINFO structure so programmer can use the additional info 
// the way he would like to.
//
// SHBN_GETADDITIONALDROPINFO	
// SHBN_SETADDITIONALDROPINFO	
//
// Sent to parent window to notify that a drag and drop operation has been finished. 
// lParam of NMSHORTCUTBAR will be set to the drop effect in which resulted the drag 
// and drop operation.
//
//SHBN_ENDDRAGDROPITEM		


// General notifications
//
// Notifies that item is about to display context menu. lParam of NMSHORTCUTBAR points to the 
// corresponding SHBCONTEXTMENU used to display popup menu (SHBCONTEXTMENU structure will be 
// covered below). If you handle this notification return non-zero value to not allow to run 
// default context menu displaying implementation.
//
//SHBN_CONTEXTMENU			


// User can define it's own notifications starting from SHBN_USER
//
//SHBN_USER					


// Structure that is used with SHBM_POPULATECONTEXTMENU message and sent to 
// shortcut bar child window before displaying context menu in order to give a chance to 
// child window to modify this menu.
typedef struct _tagSHBCONTEXTMENU
{    
	// pointer to the context menu that is about to be displayed
	CMenu* pMenu;
	// pointer to corresponding shortcut bar control (to be used in SHBN_CONTEXTMENU 
	// notification)
	class COXShortcutBar* pShortcutBar;
	// group handle
	HSHBGROUP hGroup;
	// mouse cursor coordinates to display the menu
	CPoint point;

	_tagSHBCONTEXTMENU()
	{
		ZeroMemory(this,sizeof(_tagSHBCONTEXTMENU));
	}

} SHBCONTEXTMENU, * LPSHBCONTEXTMENU;

///////////////////////////////////////////
// Structure that is used with SHBN_GETITEMINFOTIP & SHBN_GETHEADERINFOTIP notifications
// and sent to shortcut bar parent window before displaying tooltip for group header or
// child window item
typedef struct _tagSHBINFOTIP
{
    LPTSTR    lpszText;   // see below
    TCHAR     szText[80]; // buffer for infotip text
    HINSTANCE hinst;      // see below

	_tagSHBINFOTIP()
	{
		ZeroMemory(this,sizeof(_tagSHBINFOTIP));
	}

} SHBINFOTIP, *LPSHBINFOTIP;

//	lpszText		-	Address of a string to receive the text for a infotip.
//	szText			-	Buffer that receives the infotip text. An application can copy 
//						the text to this buffer as an alternative to specifying a 
//						string address.
//	hinst			-	Handle of the instance that contains a string resource to be 
//						used as the infotip text. If lpszText is the address of the 
//						infotip text, this member is NULL.
//
// When you handle the SHBN_GETITEMINFOTIP or SHBN_GETHEADERINFOTIP notifications, 
// specify the string to be displayed in one of the following ways:
//
//			-	Copy the text to the buffer specified by the szText member.
//			-	Copy the address of the buffer that contains the text to the lpszText 
//				member.
//			-	Copy the identifier of a string resource to the lpszText member, 
//				and copy the handle of the instance that contains the resource to the 
//				hinst member. 
///////////////////////////////////////////


// Shortcut Bar specific messages
//
// Next messages will be sent to any child window. It's up to window to handle them 
//
// Sent by shortcut bar to child window of expanded group before displaying context menu
// so child window can modify that menu. lParam points to SHBCONTEXTMENU structure
//
// SHBM_POPULATECONTEXTMENU	
//
// Sent by shortcut bar to child window to set the handle of group associated with
// child window. lParam is HSHBGROUP handle to the group. At this moment child window 
// can cast its parent window to shortcut bar
//
// SHBM_SETSHBGROUP			
//
// Sent by shortcut bar to child window to notify it that corresponding group
// properties have been changed. lParam is pointer to flags of all changed attributes
// (like nMask element of SHB_GROUPINFO structure).
//
//SHBM_GROUPINFOCHANGED		
//
// Sent by shortcut bar to all child windows to notify them that shortcut bar properties
// have been changed. lParam is pointer to flags of all changed attributes. Can be any 
// combination of:
//
//		SHBIF_SCRLBTNSIZE		-	size of scroll buttons has been changed
//		SHBIF_SCRLDELAY			-	time of scrolling delay has been changed
//		SHBIF_AUTOSCRLDELAY		-	time of autoscrolling delay has been changed
//		SHBIF_GROUPMARGIN		-	size of margin between groups has been changed
//		SHBIF_CHILDMARGINS		-	margins between child window rect and shortcut
//									bar specially designated rect for child window
//									have been changed
//		SHBIF_SHBSTYLE			-	style of parent shortcut control has been changed
//		SHBIF_AUTOEXPANDDELAY	-	time of autoexpanding delay has been changed
//
//
// SHBM_SHBINFOCHANGED			
//
// Sent by shortcut bar to child window to notify it that corresponding group is about 
// to be deleted.
//
//SHBM_DELETESHBGROUP			

// Message used to check mouse capture in list control
//
//SHBLCM_CHECKCAPTURE			
//
// Message used to handle starting of list control item dragging. lParam points to 
// COleDataSource object and wParam is index of dragging item
//
// SHBLCM_HANDLEDRAG			
//
//

// user can define it's own messages starting from SHBM_USER
//
//SHBM_USER					


// constants used to notify child windows about the nature of changes happened to 
// parent shortcut bar and sent as lParam with SHBM_SHBINFOCHANGED message
#define SHBIF_SCRLBTNSIZE			0x00000001
#define SHBIF_SCRLDELAY				0x00000002
#define SHBIF_AUTOSCRLDELAY			0x00000004
#define SHBIF_GROUPMARGIN			0x00000008
#define SHBIF_CHILDMARGINS			0x00000010
#define SHBIF_SHBSTYLE				0x00000020
#define SHBIF_AUTOEXPANDDELAY		0x00000040

// Callback function for sorting groups. If you want to sort groups in the way other than
// provided by default (alphabetical order) you san use SortGroupsCB() function where the 
// first argument should be a pointer to the function that have to be defined as below where:
//
//		lParam1		-	handle to the first group
//		lParam2		-	handle to the second group (to be compared to the first group)
//		lParamSort	-	function specific data, the second argument of SortGroupsCB() function 
//
// return values have to be:
//
//		-1			-	first group "<" second group
//		1			-	first group ">" second group
//		0			-	first group "==" second group
//
typedef int (CALLBACK *PFNSHBCOMPARE)(LPARAM lParam1, LPARAM lParam2, 
									  LPARAM lParamSort);


// Edit control ID to be used to edit header and item text
#define SHB_IDCEDIT					0x0000ffff

// Additional definition use as constants in different functions
//
// Flags that can be set in COXShortcutBar::HitTest function for groups
//
// Point is in header rectangle
#define SHB_ONHEADER				0x00000001
// Point is in child window rectangle
#define SHB_ONCHILD					0x00000002
// Point is out of shortcut bar rectangle
#define SHB_NOWHERE					0x00000003

// Flags that can be set in COXSHBListCtrl::HitTest function for list control items 
// in addition to standard flags used in CListCtrl::HitTest function
//
// Point is in item rectangle
#define SHBLC_ONITEM				0xffffffff
// Point is in image and text combined rectangle
#define SHBLC_ONIMAGEANDTEXT		0xfffffffe
// Point is in top scroll button rectangle
#define SHBLC_ONTOPSCROLLBUTTON		0xfffffffd
// Point is in top scroll button rectangle
#define SHBLC_ONBOTTOMSCROLLBUTTON	0xfffffffc

// Flags that can be set in COXSHBListCtrl::GetItemRect function for list control items 
// in addition to ones used in standard CListCtrl::GetItemRect function
//
// Entire item rect was requested
#define SHBLC_ENTIREITEM			0xffffffff

// If user click right mouse on shortcut bar we display context menu. Below you will find 
// predefined menu item IDs that can be used in context menu. 
//
// Group oriented 
//
// Child window type of view will be set to SHB_LARGEICON
//
// SHB_IDMLARGEICON			
//
// Child window type of view will be set to SHB_SMALLICON
//
// SHB_IDMSMALLICON			
//
// New group will be added at the bottom and editing of header will be run
//
// SHB_IDMADDNEWGROUP			
//
// Group will be removed.
//
// SHB_IDMREMOVEGROUP			
//
// Editing of header will be run.
//
// SHB_IDMRENAMEGROUP			
//

// Child Window oriented 
// item will be removed
//
// SHBLC_IDMREMOVEITEM			
//
// Editing of item will be run
//
// SHBLC_IDMRENAMEITEM			
//

// User can define it's own items of context menu starting from SHB_IDMUSER
//
// SHB_IDMUSER					



// Copy flags used with COXShortcutBar::CopyGroupInfo and 
// COXShortcutBar::CopyDescriptor functions that copy source structure 
// (SHB_GROUPINFO or SHB_DESCRIPTOR) to destination structure
//		SHB_GICOPY		-	remove existing contents of destination and copy full 
//							contents of the source into destination
//		SHB_GIGET		-	copy from source only those elements that exist both
//							in source and destination
//		SHB_GISET		-	copy from source to destination all elements found in 
//							source
#define SHB_CPYINFO_COPY			0x00000001
#define SHB_CPYINFO_GET				0x00000002
#define SHB_CPYINFO_SET				0x00000004

// Structure that is used in messages sending from COXSHBDropTarget object
// Following structures defined to provide description of any dragging item. 

enum SHBIMAGETYPE {
	SHBIT_NORMAL=0,
	SHBIT_SELECTED=1,
	SHBIT_HOT=2,
	SHBIT_DISABLED=3,
	SHBIT_USERDEFINED=4,
};

enum SHBIMAGESTATE {
	SHBIS_LARGE=0,
	SHBIS_SMALL=1,
	SHBIS_STATE=2,
	SHBIS_USERDEFINED=3,
};

typedef struct _tagSHBIMAGEDROPINFO
{
	SHBIMAGETYPE imageType;
	SHBIMAGESTATE imageState;

	_tagSHBIMAGEDROPINFO()
	{
		ZeroMemory(this,sizeof(_tagSHBIMAGEDROPINFO));
	}

} SHBIMAGEDROPINFO, * LPSHBIMAGEDROPINFO;


// Programmer can specify unlimited amount of additional information that will be 
// saved with dragged item. COXShortcutBar child window will send next notification
// SHBN_GETADDITIONALDROPINFO where lParam of NMSHORTCUTBAR will be set to 
// SHBADDITIONALDROPINFO structure so programmer can set pointer to additional info
// and the size of the info in bytes. While inserting of new item in result of drop
// event the COXShortcutBar child window will send next notification
// SHBN_SETADDITIONALDROPINFO where lParam of NMSHORTCUTBAR will be set to 
// SHBADDITIONALDROPINFO structure so programmer can use the additional info the way
// he would like to.
//
typedef struct _tagSHBADDITIONALDROPINFO
{
	// length of additional info in bytes
	DWORD nBufferLength;
	// pointer to the additional info
	void* pBuffer;

	_tagSHBADDITIONALDROPINFO()
	{
		ZeroMemory(this,sizeof(_tagSHBADDITIONALDROPINFO));
	}

} SHBADDITIONALDROPINFO, * LPSHBADDITIONALDROPINFO;



/////////////////////////////////////////////////////////////////////////////
// COXSHBEdit window

/*
  COXSHBEdit control is derived from standard CEdit control. Primarily it was designed to
  be used for in-place editing (likewise the edit controls used to edit items in tree 
  and list controls). Our COXSHBEdit control is child window which will enlarge to fit the 
  the while typing it. You can specify max rectangle to which COXSHBEdit can be enlarged and 
  also the minimum rectangle to which COXSHBEdit can be shrinked. 

  The key element of using this class is SHBE_DISPLAY structure.


// SHBE_DISPLAY structure can be used as parameter in COXSHBEdit::StartEdit function
// that defines options to display and edit text within COXSHBEdit edit control.
// Below you will find an explanation of meanings of all items:
//
//		sText			-	text to be displayed and edited
//		rectDisplay		-	edit control window's rect	
//		rectMax			-	max rect to which edit control window's can be enlarged
//		dwStyle			-	style that should be applied to edit control 
//		bEnlargeAsTyping-	enlarge edit control window to show all typed text
//		ptSelRange		-	selection range
//		pFont			-	font to set to edit control
//		rectMin			-	min rect to display in edit control
//		clrText			-	color used to draw text
//		clrBackground	-	color used to fill background
//		nMask			-	mask flags that indicate which of the other structure members 
//								contain valid data. Can be a combination of these flags:
//								SHBEIF_TEXT			-	sText is valid
//								SHBEIF_RECTDISPLAY	-	rectDisplay is valid
//								SHBEIF_RECTMAX		-	rectMax is valid
//								SHBEIF_STYLE		-	dwStyle is valid
//								SHBEIF_ENLARGE		-	bEnlargeAsTyping is valid
//								SHBEIF_SELRANGE		-	ptSelRange is valid
//								SHBEIF_FONT			-	pFont is valid
//								SHBEIF_RECTMIN		-	rectMin is valid
//								SHBEIF_CLRTEXT		-	rectMin is valid
//								SHBEIF_CLRBACK		-	rectMin is valid
//
typedef struct _tagSHBE_DISPLAY
{
	LPCTSTR lpText;
	CRect rectDisplay;
	CRect rectMax;
	DWORD dwStyle;
	BOOL bEnlargeAsTyping;
	CPoint ptSelRange;
	CFont* pFont;
	CRect rectMin;
	COLORREF clrText;
	COLORREF clrBack;
	UINT nMask;

	. . . . . . . . . . . . . . 

} SHBE_DISPLAY, * LPSHBE_DISPLAY;


  As you see from description you have to fill structure elements you're interested in and 
  just call StartEdit function. When control lost focus or user click any mouse button out 
  of control's window or pressed ENTER or ESC button the control hides itself and sends 
  SHBEN_FINISHEDIT notification to parent window. lParam we set to pointer to NMSHBEDIT 
  structure which is defined as:


typedef struct _tagNMSHBEDIT
{   
	// standard header structure
	NMHDR hdr;
	// set to FALSE if editing was cancelled (user pressed ESC)
    BOOL bOK;

  . . . . . . . . . . . . . . . . . . . . . 

} NMSHBEDIT, * LPNMSHBEDIT;


  Parent window should handle  this notification and update its item correspondingly.
  
  Now about the steps that should be taken to deploy COXSHBEdit control in your own class
  if it needs in-place editing:

	1)	Instantiate COXSHBEdit statically or dinamically (make sure it lives all the 
		time you need). E.g. you can use it as protected member of your class
		
		  COXSHBEdit m_edit;

	2)	Create COXSHBEdit. While creating you can set whatever styles you need. Set the 
		control's ID you can recognize while handling notification from the control
		and make sure the control is invisible at this moment.

		  CRect rect(0,0,0,0);
		  m_edit.Create(WS_BORDER,rect,this,SHB_IDCEDIT);

	3)	At any time you need to start edit any text you call StartEdit function. This
		function take as argument pointer to SHBE_DISPLAY, so you can specify anything
		you want (refer to SHBE_DISPLAY description above). If you use NULL as argument
		then edit control will be displayed in the latest state.

		SHBE_DISPLAY shbed;
		shbed.nMask=SHBEIF_TEXT|SHBEIF_RECTDISPLAY|SHBEIF_STYLE|SHBEIF_SELRANGE;
		shbed.lpText=sText;
		shbed.rectDisplay=rectHeader;
		shbed.dwStyle=WS_BORDER|ES_AUTOHSCROLL|ES_LEFT;
		shbed.ptSelRange=CPoint(0,-1);

		if(!m_edit.StartEdit(&shbed))
		{
			TRACE(_T("COXShortcutBar::EditGroupHeader: COXSHBEdit::StartEdit failed"));
			return NULL;
		}

	4)	Editing of text can be finished in two ways: by user interface event (lost focus, 
		any mouse button was clicked, ENTER or ESC key was pressed) or programmatically
		using FinishEdit(BOOL bOK). Set bOK to TRUE if you want to get a notification about
		successfully finished editing, or set it to FALSE to get a notification about
		cancelled editing.


  That's definitely it. The only thing that I probably have to mention is that we use this 
  control to providing editing functionality in our COXShortcutBar and COXSHBListCtrl classes,
  so you can refer to the source code of them on example of using this control. 

*/

class OX_CLASS_DECL COXSHBEdit : public CEdit
{
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Contructor of the object
	COXSHBEdit();

// Attributes
public:

protected:
	// pointer to SHBE_DISPLAY structure used to display text
	LPSHBE_DISPLAY m_pSHBED;
	// font to display text
	CFont m_font;
	// brush to fill background
	CBrush m_brush;
	// flag that specifies if editing process is active 
	BOOL m_bIsEditing;

// Operations
public:
	// --- In  :	pSHBED	-	pointer to SHBE_DISPLAY structure that specifies 
	//							edit controls display settings
	// --- Out : 
	// --- Returns:	TRUE if editing was successfully started
	// --- Effect : show and activate edit control
	virtual BOOL StartEdit(LPSHBE_DISPLAY pSHBED);

	// --- In  :	bOK		-	TRUE if editing was successfully finished or 
	//							FALSE if cancelled
	// --- Out : 
	// --- Returns:	
	// --- Effect : hide edit control and sends SHBEN_FINISHEDIT notification to the 
	//				parent window
	virtual void FinishEdit(BOOL bOK);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXSHBEdit)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructor of the object
	virtual ~COXSHBEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(COXSHBEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnUpdate();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// send notification
	LRESULT SendSHBENotification(LPNMSHBEDIT pNMSHBE);
	// verify SHBE_DISPLAY structure elements
	BOOL VerifyDisplayInfo(LPSHBE_DISPLAY pSHBED);

};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// COXSHBListCtrl window

/*
  COXSHBListCtrl is designed specifically to be default child window of shortcut bar,
  but you still can use it independently too. COXSHBListCtrl is based on standard 
  CListCtrl but overwrites almost all functions to make it work its own way. 

  COXSHBListCtrl can be only in one of two views: LVS_ICON or LVS_SMALLICON that are 
  internally translated into SHB_LARGEICON and SHB_SMALLICON types of view 
  correspondingly. 

	//		SHB_LARGEICON		Icons are large, text is under image, item is centered.
	//		SHB_SMALLICON		Icons are small, text is at the right side of image, 
	//							item is left aligned.

  Scroll elements are drawn independently too. Instead of drawing standard scroll bar 
  we draw only scroll buttons and only for those direction where it make sense to scroll.

  All in all you should use COXSHBListCtrl the way you use CListCtrl taking into account 
  limitations discussed above. We provided counterparts of all CListCtrl functions that
  can be applied to CListCtrl with LVS_ICON or LVS_SMALLICON style set. 

  Although internal representation of data in our COXSHBListCtrl is the same as in 
  CListCtrl, visual representation is in some way completely different. E.g. in 
  SHB_LARGEICON view items can be vary by height and width (in SHB_SMALLICON only
  by width) and you cannot actually set the optional size of items - everything is 
  calculated based on view type, text and image size.

  User input interface is almost completely different for COXSHBListCtrl. The control 
  never set focus, so keyboard input is ignored. Using mouse you can set any item as 
  hot while just moving a mouse over item bounding rect. At the moment there can be no 
  more than 1 hot item. If user presses mouse left button down over any item it will be 
  then marked as selected. If user unpresses left button over the selected item then 
  it is marked as active. As hot item, there can be no more than 1 selected and no more 
  than 1 active item.
  
  Below you will find the list of functions that provide a little bit different 
  functionality from the standard ones (found in CListCtrl). Refer to the documentation
  for full description of all functions.

	int GetCountPerPage() const;
	int HitTest(CPoint pt, UINT* pFlags, BOOL bOnlyItems = FALSE) const;
	int HitTest(LV_HITTESTINFO* pHitTestInfo, BOOL bOnlyItems = FALSE) const;
	BOOL GetViewRect(LPRECT lpRectView, BOOL bOnlyVisible = FALSE) const;
	BOOL GetItemRect(int nItem, LPRECT lpRect, UINT nCode) const;
	int FindItem(LV_FINDINFO* pFindInfo, int nStart = -1) const;
	CImageList* CreateDragImage(int nItem, LPPOINT lpPoint = NULL);


  As long as COXSHBListCtrl provides a lot of additional functionality there is a 
  necessity to notify the parent window on what going on in the control. Plus,
  as long as the control primarily used as COXShortcutBar child window it should  
  notify ShortcutBar parent about some events to be fully compliant with COXShortcutBar.

  Below you will find the list of all COXSHBListCtrl & COXShortcutBar specific 
  notifications:

		SHBN_BEGINDRAGITEM
		SHBN_BEGINITEMEDIT
		SHBN_DELETEALLITEMS
		SHBN_DELETEITEM
		SHBN_ENDITEMEDIT
		SHBN_GETITEMINFOTIP
		SHBN_INSERTITEM
		SHBN_SELECTITEM
		SHBN_ACTIVATEITEM
		SHBN_HOTITEM
		SHBN_DRAWITEM
		SHBN_GETDROPTARGET
		SHBN_GETDROPSOURCE
		SHBN_HANDLEDRAGITEM
		SHBN_HANDLEDRAGITEM
		SHBN_DRAGENTER				
		SHBN_DRAGLEAVE
		SHBN_DRAGOVER			
		SHBN_DRAGSCROLL			
		SHBN_DROP
		SHBN_DROPEX


  Refer to the documentation on full description of all notifications. But the most 
  interested of them will be discussed below.

  COXSHBListCtrl has built in info tip functionality for its items. All you have to 
  do is to handle SHBN_GETITEMINFOTIP notification which notifies that tooltip for 
  the control's item is about to be displayed. As in all SHBN_* notifications lParam 
  points to NMSHORTCUTBAR structure which in its turn has lparam element which points 
  to SHBINFOTIP structure which is described below. If you handle this notification fill 
  the SHBINFOTIP structure correspondingly to display the text you need


  COXSHBListCtrl has built in drag and drop functionality that let you to reposition, 
  copy and move items within the control or between different controls that use
  COXShortcutBar::m_nChildWndItemFormat format in drag and drop operations (you will 
  find the description of this format in COXShortcutbar documentation). We use 
  COXSHBDropTarget & COXSHBDropSource classes to support OLE Drag and Drop.

  Next virtual functions are defined:

	// overwrite to provide your own COleDropSource to handle drag'n'drop operation
	virtual COleDropSource* GetDropSource();
	// overwrite to provide your own COleDropTarget to handle drag'n'drop operation
	virtual COleDropTarget* GetDropTarget();

	virtual LONG OnHandleDrag(WPARAM wParam, LPARAM lParam);
	virtual LONG OnDragEnter(WPARAM wParam, LPARAM lParam);
	virtual LONG OnDragOver(WPARAM wParam, LPARAM lParam);
	virtual LONG OnDragLeave(WPARAM wParam, LPARAM lParam);
	virtual LONG OnDrop(WPARAM wParam, LPARAM lParam);

  You can override them in order to control drag and drop operation at any stage. 
  Alternatively you can handle next notifications that are sent to shortcut bar 
  parent window:

		SHBN_HANDLEDRAGITEM
		SHBN_DRAGENTER				
		SHBN_DRAGLEAVE
		SHBN_DRAGOVER			
		SHBN_DROP


*/

class OX_CLASS_DECL COXSHBListCtrl : public CListCtrl
{

	friend class COXShortcutBarSkin2003;

DECLARE_DYNCREATE(COXSHBListCtrl)

// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Contructor of object
	COXSHBListCtrl();


	// --- In  :	dwStyle		-	Specifies the list control's style. Apply any 
	//								combination of list control styles to the control. 
	//								See the CListCtrl:Create function for details.
	//				rect		-	Specifies the list control's size and position. It can 
	//								be either a CRect object or a RECT structure.
	//				pParentWnd	-	Specifies the list control's parent window, usually a 
	//								CDialog. It must not be NULL.
	//				nID			-	Specifies the list control's ID.
	// --- Out : 
	// --- Returns: TRUE if successful or FALSE otherwise
	// --- Effect : Create the control
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);


// Attributes
public:

protected:
	// As long as this class is primarily was designed to be used with COXShortcutBar
	// we keep pointer to the object of this class in case it instantiated the control
	class COXShortcutBar* m_pShortcutBar;
	// Also we keep handle of corresponding group within shortcut bar
	HSHBGROUP m_hGroup;

	// selected item - the one that was activated by left mouse down message
	int m_nSelectedItem;
	// hot item - the one that was set while mouse was moving over
	int m_nHotItem;
	// latest hot item - the one that was previously set as a hot one
	int m_nLastHotItem;
	// active item - the selected item that was activated by left mouse up message
	int m_nActiveItem;
	// drop target item that will be hilited in the control
	int m_nDropHilightItem;
	// item that is being dragged
	int m_nDragItem;
	// item that is beign editing
	int m_nEditItem;

	// edit control used to edit items in the control
	COXSHBEdit m_edit;

	// map array to associate items and corresponding rectangles
	// to display them on the screen
	typedef CMap<int, int, CRect, CRect> mapItemToBoundRect;
	mapItemToBoundRect m_mapItemToBoundRect;
	mapItemToBoundRect m_mapItemToImageRect;
	mapItemToBoundRect m_mapItemToTextRect;

	// rectangles to display scroll buttons
	CRect m_rectTopScrollButton;
	CRect m_rectBottomScrollButton;

	// view origin
	CPoint m_ptOrigin;

	// default COleDropSource for drag'n'drop operation
	COXSHBDropSource m_oleDropSource;

	// default COleDropTarget for drag'n'drop operation
	COXSHBDropTarget m_oleDropTarget;

	CImageList m_ilLarge;
	CImageList m_ilSmall;

	// defines version of comctl32.dll currently installed on the computer
	// GetComCtlVersion function returns major and minor version of this dll
	// and we combine these values using MAKELONG macro
	static DWORD m_dwComCtlVersion;

private:
	// mouse is over the control
	BOOL m_bMouseIsOver;
	// scroll up button is currently "pressed"
	BOOL m_bScrollingUp;
	// scroll down button is currently "pressed"
	BOOL m_bScrollingDown;
	// autoscrolling is currently "active"
	BOOL m_bAutoScrolling;

	// notifies drawing routines that we are creating drag image of chosen item
	BOOL m_bCreatingDragImage;
	// drag item image
	CImageList* m_pDragImage;
	// point in drag image where the mouse cursor point to
	CPoint m_ptDragImage;
	// point on the control where left button has been pressed
	CPoint m_ptClickedLButton;
	// probable drag item (we use some time interval to make sure that user 
	// really want to the drag item)
	int m_nSuspectDragItem;
	// Flag that specified whether we launched drag'n'drop operation or not
	BOOL m_bDragDropOwner;
	// Flag that specified if any drag and drop operation is undergoing
	BOOL m_bDragDropOperation;

	// timer for scrolling event
	UINT_PTR m_nScrollTimerID;
	// timer for checking mouse position
	UINT_PTR m_nCheckMouseTimerID;

	// item index to insert drop object before
	int m_nInsertItemBefore;

// Operations
public:

	//
	// --- In  :	
	// --- Out : 
	// --- Returns: Background color of the list control
	// --- Effect : 
	COLORREF GetBkColor() const;

	// --- In  : clr -- color to set the foreground of the control to 
	// --- Out : 
	// --- Returns: TRUE if successful otherwise FALSE
	// --- Effect : Sets the foreground color ot the control
	BOOL SetBkColor(COLORREF clr);

	
	// --- In  :	
	// --- Out : 
	// --- Returns: Text color of the list control
	// --- Effect : 
	COLORREF GetTextColor() const;

	// --- In  : clr -- color to set the text to	
	// --- Out : 
	// --- Returns: TRUE if successful otherwise FALSE
	// --- Effect : Changes the text color
	BOOL SetTextColor(COLORREF clr);


	// --- In  :	
	// --- Out : 
	// --- Returns: the background color of the text
	// --- Effect : 
	COLORREF GetTextBkColor() const;

	// --- In  : clr -- color to set the text background color to 	
	// --- Out : 
	// --- Returns: TRUE if successful otherwise FALSE
	// --- Effect : changes the background color of the text
	BOOL SetTextBkColor(COLORREF clr);


	// --- In  : 
	// --- Out : 
	// --- Returns: The number of items that can fit vertically in the visible area 
	//              of a list view control. May vary depending on current view origin
	// --- Effect : 
	int GetCountPerPage() const;


	// --- In  : 
	// --- Out : 
	// --- Returns: If successful, a pointer to the COXSHBEdit object that is used to 
	//				edit the item text; otherwise NULL.
	// --- Effect : Retrieves the handle of the edit control used to edit a list view 
	//				item's text
	inline COXSHBEdit* GetEditControl() { return &m_edit; }


	// --- In  : pHitTestInfo   Address of a LV_HITTESTINFO structure that contains 
	//							the position to hit test and that receives information 
	//							about the results of the hit test.
	//			 pt             Point to be tested.
	//           pFlags         Pointer to an integer that receives information about 
	//							the results of the test. Can be one of:
	//				
	//		LVHT_ABOVE				The position is above the client area of the control.
	//		LVHT_BELOW				The position is below the client area of the control.
	//		LVHT_NOWHERE			The position is inside the list view control's 
	//								client window but is not over a list item.
	//		LVHT_ONITEMICON			The position is over a list view item's icon.
	//		LVHT_ONITEMLABEL		The position is over a list view item's text.
	//		LVHT_TOLEFT				The position is to the left of the list view 
	//								control's client area.
	//		LVHT_TORIGHT			The position is to the right of the list view 
	//								control's client area.
	//		SHBLC_ONITEM			The position is in item rectangle
	//		SHBLC_ONIMAGEANDTEXT	The position is in image and text combined rectangle
	//		SHBLC_ONTOPSCROLLBUTTON	The position is in top scroll button rectangle
	//		SHBLC_ONBOTTOMSCROLLBUTTON	Point is in top scroll button rectangle
	//
	//			 bOnlyItems		Only list control items positions will be taken into
	//							account (scroll buttons will be ignored)
	// --- Out : 
	// --- Returns: The index of the item at the position specified by pHitTestInfo, 
	//				if any, or -1 otherwise
	// --- Effect : Determines which list view item, if any, is at a specified position.
	int HitTest(CPoint pt, UINT* pFlags, BOOL bOnlyItems = FALSE) const;
	inline int HitTest(LV_HITTESTINFO* pHitTestInfo, BOOL bOnlyItems = FALSE) const 
	{
		pHitTestInfo->iItem=HitTest(pHitTestInfo->pt,&pHitTestInfo->flags,bOnlyItems);
		return pHitTestInfo->iItem;
	}
	

	// --- In  :	lpRectView	-	address of a RECT structure.
	//				bOnlyVisible-	return only currently visible part of view rect
	// --- Out : 
	// --- Returns: TRUE if successful; otherwise FALSE.
	// --- Effect : Retrieves the bounding rectangle of all or only visible items in the 
	//				list view control.
	BOOL GetViewRect(LPRECT lpRectView, BOOL bOnlyVisible = FALSE) const;

	// --- In  :	nItem	-	The index of the item whose position is to be retrieved.
	//				lpRect	-	Address of a RECT structure that receives the bounding 
	//							rectangle. 
	//				nCode	-	Portion of the list view item for which to retrieve the 
	//							bounding rectangle. It can be one of these values:
	//
	//				LVIR_BOUNDS			Returns the bounding rectangle of the entire 
	//									item, including the icon and label.
	//				LVIR_ICON			Returns the bounding rectangle of the icon or 
	//									small icon.
	//				LVIR_LABEL			Returns the bounding rectangle of the item text.
	//				SHBLC_ENTIREITEM	Returns entire item rectangle
	//
	// --- Out : 
	// --- Returns: If successful, TRUE ; otherwise FALSE.
	// --- Effect : Retrieves the bounding rectangle for all or part of an item in the 
	//				current view.
	BOOL GetItemRect(int nItem, LPRECT lpRect, UINT nCode) const;

	// --- In  :	nItem	-	The index of the item whose position is to be retrieved.
	//				lpPoint	-	Address of a POINT structure that receives the position 
	//							of the item's upper-left corner, in view coordinates.
	// --- Out : 
	// --- Returns: If successful, TRUE ; otherwise FALSE.
	// --- Effect : Retrieves the position of a list view item.
	BOOL GetItemPosition(int nItem, LPPOINT lpPoint) const;


	// --- In  :  nItem -- Item to be redrawn 
	// --- Out : 
	// --- Returns: If successful, TRUE ; otherwise FALSE.
	// --- Effect : Forces the ListControl to repaint the item specified by nItem
	BOOL Update(int nItem);

	// --- In  : nFirst - index of the first element to be redrawn
	//			 nLast  - index of the last element to be redrawn
	// --- Out : 
	// --- Returns: If successful, TRUE ; otherwise FALSE.
	// --- Effect : Redraws a range of items
	BOOL RedrawItems(int nFirst, int nLast);


	// --- In  : size - A CSize object specifying the amount of horizontal and vertical 
	//           scrolling, in pixels. 
	// --- Out : 
	// --- Returns: If successful, TRUE ; otherwise FALSE.
	// --- Effect : Scrolls the content of a list view control
	BOOL Scroll(CSize size);
	inline BOOL Scroll(int nXScroll, int nYScroll) 
	{ 
		return Scroll(CSize(nXScroll,nYScroll)); 
	}


	// --- In  : lpPoint - Address of a POINT structure that receives the view origin
	// --- Out : 
	// --- Returns: If successful, TRUE ; otherwise FALSE.
	// --- Effect : Retrieves the current view origin for a list view control
	inline BOOL GetOrigin(LPPOINT lpPoint) const 
	{
		lpPoint->x=m_ptOrigin.x;
		lpPoint->y=m_ptOrigin.y;
		return TRUE;
	}


	// --- In  : pFindInfo   A pointer to a LV_FINDINFO structure containing information 
	//                       about the item to be searched for. 
	//			 nStart      Index of the item to begin the search with, or 1 to start 
	//                       from the beginning. The item at nStart is excluded from the 
	//                       search if nStart is not equal to -1.
	// --- Out : 
	// --- Returns: 0 based index of the element
	// --- Effect : Searches the list control for an item matching the information given 
	//				in pFindInfo
	int FindItem(LV_FINDINFO* pFindInfo, int nStart = -1) const;
	

	// --- In  : 
	// --- Out : 
	// --- Returns: 0 based index of the top most visible element
	// --- Effect : 
	int GetTopIndex() const;

	
	// --- In  : nItem -- the item to be made visible
	//           bPartialOK -- TRUE partial display OK
	//                         FALSE display element entirely
	// --- Out :
	// --- Returns: If successful, TRUE ; otherwise FALSE.
	// --- Effect:
	BOOL EnsureVisible(int nItem, BOOL bPartialOK);


	// --- In  : nItem -- element to be edited
	// --- Out : 
	// --- Returns: a pointer to the edit control for the element
	// --- Effect : provides an edit interface to the list control
	COXSHBEdit* EditLabel(int nItem);


	// --- In  : nItem		Index of the item whose drag image list is to be created.
	//           lpPoint	Address of a POINT structure that receives the initial 
	//						location of the upper-left corner of the image, in view 
	//						coordinates. Included only in order to be compatible with
	//						CListCtrl::CreateDragImage function. Can be NULL
	// --- Out : 
	// --- Returns: A pointer to the drag image list if successful; otherwise NULL
	// --- Effect : Call this function to create a drag image list for the item specified
	//              by nItem. The CImageList object is permanent, and you MUST delete it 
	//              when finished working with it.
	CImageList* CreateDragImage(int nItem, LPPOINT lpPoint = NULL);


	////////////////////////////////////////////////////////////////////////
	// non-standard
	//

	// --- In  :	rectScrollable	-	rectangle to retrieve information on scrollable
	//									rectangle.
	//				nTopItem		-	if set to -1 then scrollable rect will be return
	//									for current top item, otherwise for given one
	// --- Out : 
	// --- Returns: If successful, TRUE ; otherwise FALSE.
	// --- Effect : Retrieves the scrollable rectangle. May vary depending on current
	//				view origin.
	BOOL GetScrollableRect(CRect& rectScrollable, int nTopItem =-1) const;

	
	// --- In  : 
	// --- Out : 
	// --- Returns: additional styles used to organize and display the control. Can be 
	//				any combination of the next:
	//
	//		SHBS_DISABLEDRAGITEM			items won't as drag source
	//		SHBS_DISABLEDROPITEM			won't accept any OLE drag'n'drop object
	//		SHBS_DISABLEDRAGDROPITEM		won't support OLE drag'n'drop (combination
	//										of SHBS_DISABLEDRAGITEM and 
	//										SHBS_DISABLEDROPITEM styles
	//		SHBS_EDITITEMS					items text can is editable
	//		SHBS_NOSCROLL					scroll buttons won't be displayed 
	//		SHBS_UNDERLINEHOTITEM			text of hot item will be drawn using 
	//										underline font
	//		SHBS_SHOWACTIVEALWAYS			active item will be shawn even if the 
	//										shortcut bar doesn't have mouse input
	//		SHBS_AUTOSCROLL					scrolling will be run automatically when 
	//										mouse is over scrolling buttons
	//		SHBS_DRAWITEMDRAGIMAGE			draws item image while dragging it 
	// --- Effect : 
	DWORD GetBarStyle() const;

	
	// --- In  :	pNMSHB		-	pointer to NMSHORTCUTBAR structure specifically
	//								designed to send notification to shortcut bar parent
	//								window.
	// --- Out : 
	// --- Returns: result of notification handling
	// --- Effect : make sense to use only if parent of the list control is shortcut bar 
	LRESULT SendSHBNotification(LPNMSHORTCUTBAR pNMSHB) const;


	// --- In  : 
	// --- Out : 
	// --- Returns: one of two possible view state:
	//		SHB_LARGEICON		Icons are large, text is under image, item is centered.
	//		SHB_SMALLICON		Icons are small, text is at the right side of image, 
	//							item is left aligned.
	// --- Effect : 
	int GetView() const;

	
	// --- In  : 
	// --- Out : 
	// --- Returns: size of scroll button
	// --- Effect : 
	CSize GetScrollButtonSize() const;

	
	// --- In  : 
	// --- Out : 
	// --- Returns: delay in milliseconds between two scrolling events
	// --- Effect : 
	UINT GetScrollingDelay() const;


	// --- In  : 
	// --- Out : 
	// --- Returns: delay in milliseconds between two autoscrolling events
	// --- Effect : 
	UINT GetAutoScrollingDelay() const;

	
	// --- In  : 
	// --- Out : 
	// --- Returns: the 0 based index of the element that is currently selected
	// --- Effect : allows the program to determine which element is selected
	inline int GetSelectedItem() { return m_nSelectedItem; }

	// --- In  : nItem -- item to select
	// --- Out : 
	// --- Returns: the 0 based index of the previously selected item if any, 
	//				otherwise -1
	// --- Effect : 
	virtual int SelectItem(int nItem);


	// --- In  : 
	// --- Out : 
	// --- Returns: hot item index
	// --- Effect : allow the program to determine which element is hot
	inline int GetHotItem() { return m_nHotItem; }

	// --- In  : nItem -- item to set as hot
	// --- Out : 
	// --- Returns: the 0 based index of the previously hot item if any, 
	//				otherwise -1
	// --- Effect : 
	int SetHotItem(int nItem);

	// --- In  : 
	// --- Out : 
	// --- Returns: hot item index
	// --- Effect : allow the program to determine which element is hot
	inline int GetLastHotItem() { return m_nLastHotItem; }


	// --- In  : 
	// --- Out : 
	// --- Returns: the 0 based index of the active item if successful otherwise -1
	// --- Effect : allows the program to determine which element is currently active
	inline int GetActiveItem() { return m_nActiveItem; }

	// --- In  : nItem -- item to activate
	// --- Out : 
	// --- Returns: the 0 based index of the previously active item if any, 
	//				otherwise -1
	// --- Effect : 
	int ActivateItem(int nItem);


	// --- In  : 
	// --- Out : 
	// --- Returns: the 0 based index of the item highlighted as drop target
	// --- Effect : allows the program to determine which element is about to receive
	//              data through a drag and drop operation
	inline int GetDropHilightItem() { return m_nDropHilightItem; }

	// --- In  : nItem -- item to set as drop target.
	// --- Out : 
	// --- Returns: the 0 based index of the previous drop targer (drop hilight) item 
	//				if any otherwise -1
	// --- Effect : sets the target item for a drag and drop operation
	int SetDropTargetItem(int nItem);


	// --- In  : 
	// --- Out : 
	// --- Returns: the 0 based index of the element being dragged or -1 if none
	// --- Effect : allows the program to determine what element if any is being dragged
	inline int GetDragItem() { return m_nDragItem; }

	// --- In  : nItem -- element to select
	// --- Out : 
	// --- Returns: the 0 based index of the selected item if successful otherwise -1
	// --- Effect : sets a specified item as the current drag object to be placed elsewhere
	//              when the mouse is released or the drag and drop is completed
	int SetDragItem(int nItem);


	// --- In  : 
	// --- Out : 
	// --- Returns: the 0 based index of the item currently edited if any, otherwise -1
	// --- Effect : allows the program to determine is any item is beign editing 
	//				at the moment
	inline int GetEditItem() { return m_nEditItem; }


	// --- In  :	nItem	-	index of item for which image index will be returned
	// --- Out : 
	// --- Returns: the 0 based index of the image in the image lists associated with 
	//				the control or -1 if there is no any image
	// --- Effect : allows the program to determine the image index associated with the 
	//				specified item
	int GetItemImage(const int nItem);


	// --- In  :	
	// --- Out : 
	// --- Returns: the number of current version of comctl32.dll
	// --- Effect : allows the program to determine the version of currently running 
	//				comctl32.dll
	inline DWORD GetComCtlVersion() { return m_dwComCtlVersion; }


	// --- In  :	
	// --- Out : 
	// --- Returns: TRUE if drag and drop operation was initiated from the control
	// --- Effect : allows the program to determine if the draged item is from 
	//				this control
	inline BOOL IsDragDropOwner() { return m_bDragDropOwner; }


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXSHBListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL


// Implementation
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructor of object
	virtual ~COXSHBListCtrl();


	// --- In  :	point	-	Specifies the x- and y-coordinate of the cursor. 
	//							These coordinates are always relative to the upper-left 
	//							corner of the window
	//				pTI		-	A pointer to a TOOLINFO structure
	// --- Out : 
	// --- Returns:	If -1, the tooltip control was not found, otherwise was found
	// --- Effect : this function is called to detemine whether a point is in the 
	//				bounding rectangle of the specified tool. If the point is in the 
	//				rectangle, it retrieves information about the tool. If the area 
	//				with which the tooltip is associated is not a button, OnToolHitTest 
	//				sets the structure flags to TTF_NOTBUTTON and TTF_CENTERTIP. 
	//				See TOOLINFO, in the Win32 SDK Programmer's Reference, for more 
	//				information about the structure.
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;


protected:
	// overwrite to provide your own routine for drawing item
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	// overwrite to provide your own routine for drawing scroll buttons
	virtual void DrawScrollButtons(CDC* pDC);
	// overwrite to provide your own routine for drawing place holder arrow
	virtual void DrawPlaceHolder(CDC* pDC);
	// overwrite to provide your own routine for creating edit control
	virtual BOOL CreateEditControl();
	// overwrite to provide your own routine for calculating item positions
	virtual void CalculateBoundRects();
	// overwrite to provide your own routine for calculating scroll buttons positions
	virtual void CalculateScrollRects();
	// overwrite to provide your own COleDropSource to handle drag'n'drop operation
	virtual COleDropSource* GetDropSource();
	// overwrite to provide your own COleDropTarget to handle drag'n'drop operation
	virtual COleDropTarget* GetDropTarget();

	// Generated message map functions
protected:
	//{{AFX_MSG(COXSHBListCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	virtual LRESULT OnHandleDrag(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCheckCapture(WPARAM wParam, LPARAM lParam);
	
	afx_msg BOOL OnItemToolTip(UINT id, NMHDR* pTTTStruct, LRESULT* pResult);

	virtual LRESULT OnDragEnter(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnDragOver(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnDragLeave(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnDrop(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnSetSHBGroup(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSHBInfoChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGroupInfoChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnChangeItemText(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg LRESULT OnPopulateContextMenu(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnCreateDragImage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteAllItems(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditLabel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnsureVisible(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFindItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetBkColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetCountPerPage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetEditControl(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetHotItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetItemPosition(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetItemRect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetOrigin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetTextBkColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetTextColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetTopIndex(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetViewRect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHitTest(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInsertItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRedrawItems(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScroll(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetBkColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetHotItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetItemText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetTextBkColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetTextColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSortItems(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdate(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

protected:
	// initialize list control: create edit and tooltip controls
	BOOL InitListControl();

	// adjust text rectangle of the item based on the image rectangle
	CRect AdjustTextRect(CString sText,CRect& rectImage, int nMaxWidth);

	// fill background of the control with the background color
	void FillBackground(CDC* pDC);

	// capture mouse messages
	BOOL CheckCapture();
	// check if we lost capture and if needed capture it again
	void PostCheckCapture();

#ifndef _INC_WINDOWSX
	// return TRUE if mouse left button is pressed down
	inline BOOL IsLButtonDown() { return (GetKeyState(VK_LBUTTON)<0); }
#endif
	// handle all mouse messages
	void AnticipateMouseMessages();

	// start the process of scrolling items within the control
	void StartScrolling(BOOL bScrollingUp);
	// stop the process of scrolling items within the control
	void StopScrolling();

	// get coordinates of rectangle that represent gap before specified item
	CRect GetGapBetweenItems(int nItemBefore);
	// get coordinates of place holder rectangle that is located before specified item
	CRect GetPlaceHolderRect(int nItemBefore);

private:
	// helper function to define the version of comctl32.dll
	// if succeed it sets major number of version to pdwMajor and
	// minor number to pdwMinor
	HRESULT GetComCtlVersion(LPDWORD pdwMajor, LPDWORD pdwMinor);
	BOOL HasBkColor() const;
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// COXShortcutBar window

/*

  Shortcut bar is new control that emulates the one found in Microsoft Outlook and suggest 
  some additional functionality that put it on the same level as CoolBars. Let me just remind
  that CoolBar (aka ReBar control) operates as a container of child windows (e.g. Toolbars) 
  that reside in bands and provides functionality of repositioning these bands vertically and 
  horizontally.

  Shortcut bar also acts as a container of child windows. It uses group paradigm to represent
  them. Every group consist of header and child window. Group can be in expanded and 
  unexpanded state. Only one group can be expanded at once. Correspondingly only child window
  of expanded group will be shown. Only headers of unexpanded groups will be shown. If at 
  least one group created in shortcut bar then there is always expanded group in the control.

  Also we provide with our shortcut bar standard child window that will be used as default 
  if nothing else specified - special list control that entirely emulate one found in
  Microsoft Outlook.

  Schematically we can represent shortcut bar in next way:

	|-----------------------|
	|		 Outlook		|	<--		Group 
	|-----------------------|
	|		  Mail			|	<--		Expanded Group
	|-----------------------|
	|						|
	|		|-------|		|		
	|		|	+	|		|		
	|		|  + +	|		|			
	|		|-------|		|			
	|		  Inbox			|			
	|						|		|----------------------------------------------------
	|		|-------|		|		|	
	|		|	+	|		|		|	Our special list control as child window.
	|		|  + +	|		|	<--
	|		|-------|		|		|	Large Icons view is set
	|		Sent Items		|		|	
	|						|		|----------------------------------------------------
	|		|-------|		|		
	|		|	+	|		|		
	|		|  + +	|		|		
	|		|-------|		|		
	|		  Outbox		|		
	|						|		
	|		|-------|	 _	|		
	|		|	+	|	|_|	|		
	|-----------------------|
	|		 Others			|	<--		Group 
	|-----------------------|



  Every group has to have associated child window. The only requirement is that child 
  window must be created with WS_CHILD style specified. When the user clicks over 
  unexpanded group previously expanded group will shrink and clicked group will expand.

  Each group has special SHB_GROUPINFO structure associated with it which provides 
  all group information. This structure defined as:

		typedef struct _tagSHB_GROUPINFO
		{
			LPTSTR pszText;
			int nTextMax;
			int nImage;
			int nImageExpanded;
			LPARAM lParam;
			int nOrder;
			int	nView;
			LPSHB_DESCRIPTOR pDescriptor;
			HWND hwndChild;
			UINT nMask;

			.................................

		} SHB_GROUPINFO, * LPSHB_GROUPINFO;

  SHB_GROUPINFO structure is designed to provide full info about any group within 
  ShortcutBar. Below you will find an explanation of meanings of all items:

		pszText				-	group name
		nTextMax			-	max length of pszText
		nImage				-	index of image from associated image list used to display
								on header control when group is not expanded
		nImageExpanded		-	index of image from associated image list used to display
								on header control when group is expanded
		lParam				-	user-defined data associated with group
		nOrder				-	Order in which the group appears within the Shorcut Bar, from 
								top to bottom. That is, the value for the top group is 0. The 
								value for the next group to the bottom is 1, and so on. While 
								using as parameter of InsertGroup function can be also one of:
								SHBI_FIRST		-	Inserts the group at the beginning of the 
													list.
								SHBI_LAST		-	Inserts the group at the end of the list.
								SHBI_SORT		-	Inserts the group into the list in 
													alphabetical order.
		nView				-	type of viewing items in list control, can be on of:
								SHB_LARGEICON
								SHB_SMALLICON
		pDescriptor			-	pointer to SHB_DESCRIPTOR structure that provides additional 
								customization information on the group's appearance
		hwndChild			-	handle of window to display when the group is expanded. By 
								default we create list control to display as child window
								and provide the functionality to fill it with items and 
								manipulate them. If hwndChild is not NULL then we won't create 
								corresponding list control and all the user interface 
								interpretation is up to the programmer.
		nMask				-	mask flags that indicate which of the other structure members 
								contain valid data. Can be a combination of these flags:
								SHBIF_TEXT			-	pszText and nTextMax is valid
								SHBIF_IMAGE			-	nImage is valid
								SHBIF_IMAGEEXPANDED	-	nImageExpanded is valid
								SHBIF_PARAM			-	lParam is valid
								SHBIF_ORDER			-	nOrder is valid
								SHBIF_VIEW			-	nView is valid
								SHBIF_DESCRIPTOR	-	pDescriptor is valid
								SHBIF_CHILDWND		-	hwndChild is valid
								


  Using this structure you can specify all information you need to set about a group. Most
  elements of the structure are pretty straightforward. The only element that need additional
  explanation is pDescriptor. pDescriptor is pointer to SHB_DESCRIPTOR structure that can be 
  associated with any group. This structure was specifically designed to cudtomize group 
  visual appearence and declared as:

		typedef struct _tagSHB_DESCRIPTOR
		{
			COLORREF clrBackground;
			COLORREF clrText;
			COLORREF clrHeaderBackground;
			COLORREF clrHeaderText;
			UINT nHeaderTextFormat;
			LOGFONT lfText;
			LOGFONT lfHeader;
			int nHeaderHeight;
			UINT nMask;

			...........................

		} SHB_DESCRIPTOR, * LPSHB_DESCRIPTOR;

 SHB_DESCRIPTOR structure can be associated with any group within ShortcutBar.
 Below you will find an explanation of meanings of all items:

		clrBackground		-	background color of list control
		clrText				-	color used to draw text in list control
		clrHeaderBackground	-	background color of header control
		clrHeaderText		-	color used to draw text in header control
		nHeaderTextFormat	-	text format used to display group name in header control. 
								Refer to DrawText function to get full description of all
								available formats.
		lfText				-	font to draw items in list control
		lfHeader			-	font to draw items in header control
		nHeaderHeight		-	height of header control
		nMask				-	mask flags that indicate which of the other structure members 
								contain valid data. Can be a combination of these flags:
								SHBIF_CLRBACK		-	clrBackground is valid
								SHBIF_CLRTEXT		-	clrText is valid
								SHBIF_HDRCLRBACK	-	clrHeaderBackground is valid
								SHBIF_HDRCLRTEXT	-	clrHeaderText is valid
								SHBIF_HDRTEXTFMT	-	nHeaderTextFormat is valid
								SHBIF_FONT			-	lfText is valid
								SHBIF_HDRFONT		-	lfHeader is valid
								SHBIF_HDRHEIGHT		-	nHeaderHeight is valid

  You can see from the declaration that you can specify major stuff about group apperance.
  But we shouldn't forget that group always associated with child window. And it's child 
  window responsibility to show itself the way it described in SHB_DESCRIPTOR. 

  Also different shortcut bar style can be applied to the control. Here is the list of all 
  available styles:


	//		SHBS_DISABLEDRAGITEM		items won't as drag source
	//		SHBS_DISABLEDROPITEM		won't accept any OLE drag'n'drop object
	//		SHBS_DISABLEDRAGDROPITEM	won't support OLE drag'n'drop (combination
	//									of SHBS_DISABLEDRAGITEM and 
	//									SHBS_DISABLEDROPITEM styles
	//		SHBS_EDITHEADERS			group headers can be edited
	//		SHBS_EDITITEMS				child window's item text can be edited
	//		SHBS_INFOTIP				SHBN_GETHEADERINFOTIP and SHBN_GETITEMINFOTIP 
	//									notifications will be fired 
	//		SHBS_NOSCROLL				scroll buttons won't be displayed in child 
	//									windows
	//		SHBS_BOLDEXPANDEDGROUP		text in expanded group header will be drawn 
	//									using bold font
	//		SHBS_UNDERLINEHOTITEM		text of hot item in expanded group child 
	//									window will be drawn using underline font
	//		SHBS_SHOWACTIVEALWAYS		active item in expanded group child window 
	//									will be shawn even if the shortcut bar 
	//									doesn't have mouse input
	//		SHBS_DISABLEDRAGDROPHEADER	shortcut bar won't send SHBN_BEGINDRAGHEADER 
	//									notification
	//		SHBS_DRAWHEADERDRAGIMAGE	draws header image while dragging it 
	//		SHBS_DRAWITEMDRAGIMAGE		draws child window item image while 
	//									dragging it 
	//		SHBS_AUTOSCROLL				scrolling will be run automatically when 
	//									mouse is over scrolling buttons
	//		SHBS_ANIMATEEXPAND			animation effect will be applied while 
	//									expanding a group
	//		SHBS_AUTOEXPAND				auto expanding of the group if mouse is over 
	//									the group header for some time
	//
	//								Note that implementation of some styles behaviour
	//								in application defined child windows is up to a 
	//								programmer. Some style can be ignored by child 
	//								window. Our default child window COXSHBListCtrl
	//								implements all of them.


  Probably it's already too much information that you wouldn't probably need while using our
  COXShortcutBar. As long as we provide lots functionality by default let's just move to 
  the steps that should be taken to implement shortcut bar in its default state and after
  that we will move to advanced topic.

  1)	As any window COXShortcutBar control should be created in order to be displayed. We 
		provide our own Create function which is defined as:

	BOOL Create(CWnd* pParentWnd, const RECT& rect, 
		DWORD dwBarStyle = SHBS_EDITHEADERS|SHBS_EDITITEMS|
		SHBS_DISABLEDRAGDROPITEM|SHBS_DISABLEDRAGDROPHEADER, 
		UINT nID = 0xffff, DWORD dwStyle = WS_CHILD|WS_VISIBLE, 
		DWORD dwExStyle = 0);
	// --- In  :	pParentWnd	-	pointer to parent window, cannot be NULL
	//				rect		-	window's rectangle
	//				dwBarStyle	-	shortcut bar specific styles.
	//				nId			-	control ID	
	//				dwStyle		-	window style. WS_CHILD must be specified.
	//				dwExStyle	-	window extended style

		As you can see the only difference from creating standard window is that you
		can specify shortcut bar styles.


		An alternative way of using COXShortcutBar is subclassing of existing controls.
		E.g. if you want to use the control in dialog then it's good idea to use CStatic 
		control as placeholder. And after that associate this static control to our 
		shortcut bar using Class Wizard. In the case of subclassing we us by default next
		COXShortcutBar styles: SHBS_EDITHEADERS|SHBS_EDITITEMS|SHBS_DISABLEDRAGDROPITEM|
		SHBS_DISABLEDRAGDROPHEADE. 

  2)	after creating or subclassing we've got empty shortcut bar that should be populated 
		with groups. To insert new group into shortcut bar use next function:

	HSHBGROUP InsertGroup(const LPSHB_GROUPINFO pGroupInfo);
	// --- In  : pGroupInfo -- a pointer to an SHB_GROUPINFO structure containing the 
	//						   information for the new group
	// --- Out : 
	// --- Returns: a handle to the new group if successful otherwise NULL
	// --- Effect : 
		
		You have to fill only those elements of SHB_GROUPINFO structure that you are
		interested in. E.g.

  	sText=_T("Outlook");
	shbGroup.nMask=SHBIF_TEXT;
	shbGroup.nTextMax=sText.GetLength();
	shbGroup.pszText=sText.GetBuffer(shbGroup.nTextMax);
	sText.ReleaseBuffer();
	m_ctlShortcutBar.InsertGroup(&shbGroup);


		If you haven't specified application defined window as group child window then
		standard COXSHBListCtrl will be created and associated with the new created group
		as its child window. To populate COXSHBListCtrl with items you can use next function 
		that are just wrappers around corresponding functions defined in COXSHBListCtrl class:


	CImageList* SetLCImageList(HSHBGROUP hGroup, CImageList* pImageList, int nImageList);
	CImageList* GetLCImageList(HSHBGROUP hGroup, int nImageList) const;
	BOOL GetLCItem(HSHBGROUP hGroup, LV_ITEM* pItem) const;
	BOOL SetLCItem(HSHBGROUP hGroup, const LV_ITEM* pItem);
	int InsertLCItem(HSHBGROUP hGroup, const LV_ITEM* pItem);
	BOOL DeleteLCItem(HSHBGROUP hGroup, int nItem);
	COXSHBEdit* EditLCItem(HSHBGROUP hGroup, int nItem);

		
		E.g.:

	TCHAR shbszItems[][20]={_T("Inbox"),_T("Calendar"),_T("Contracts"), 
		_T("Tasks"),_T("Journal"),_T("Notes"),_T("Deleted Items"),_T("Inbox"),
		_T("Sent Items"),_T("Outbox"),_T("Deleted Items")};

	m_ctlShortcutBar.SetLCImageList(hGroup,&m_ilLarge,LVSIL_NORMAL);
	m_ctlShortcutBar.SetLCImageList(hGroup,&m_ilSmall,LVSIL_SMALL);

	for(nIndex=0; nIndex<(sizeof(shbszItems)/sizeof(shbszItems[0])) ; nIndex++)
	{
		CString sText=shbszItems[nIndex];
		LV_ITEM lvi;
		lvi.iItem=nIndex;
		lvi.iSubItem=0;
		lvi.mask=LVIF_TEXT|LVIF_IMAGE;
		lvi.iImage=nIndex;
		lvi.cchTextMax=sText.GetLength();
		lvi.pszText=sText.GetBuffer(lvi.cchTextMax);
		sText.ReleaseBuffer();
		m_ctlShortcutBar.InsertLCItem(hGroup,&lvi);
	}

		
		Alternatively, you could have get a pointer to list control associated with group
		using next function:

	COXSHBListCtrl* GetGroupListCtrl(HSHBGROUP hGroup) const;

		and call counterparts of above described functions directly.


  3)	After creating shorcut bar and populating its groups child windows we can manipulate
		the control in the way we'd like to. 

		We can add/delete new groups; change group info; manipulate child windows and much 
		more. Refer to shortcut bar functions referense for details. If you use default 
		child window - COXSHBListCtrl then you can use all power of this class too.

		All in all manipulating of shortcut bar control reminds the way we manipulate
		list or tree controls. And these controls have set of notification messages that 
		are used to notify parent window about any event happened in the control.

		Our COXShortcutBar also have its own set of notifications. Below you will find 
		full reference of all of them:

		SHBN_BEGINDRAGHEADER
		SHBN_DELETEGROUP
		SHBN_ENDHEADEREDIT
		SHBN_GETHEADERINFOTIP
		SHBN_GROUPEXPANDED
		SHBN_GROUPEXPANDING
		SHBN_DRAWHEADER
		SHBN_DROPGROUP
		SHBN_INSERTGROUP

		SHBN_BEGINDRAGITEM
		SHBN_BEGINITEMEDIT
		SHBN_DELETEALLITEMS
		SHBN_DELETEITEM
		SHBN_ENDITEMEDIT
		SHBN_GETITEMINFOTIP
		SHBN_INSERTITEM
		SHBN_SELECTITEM
		SHBN_ACTIVATEITEM
		SHBN_HOTITEM
		SHBN_DRAWITEM

		SHBN_CONTEXTMENU
		SHBN_DRAGENTER
		SHBN_DRAGLEAVE
		SHBN_DRAGOVER
		SHBN_DRAGSCROLL
		SHBN_DROP
		SHBN_DROPEX


		Refer to the documentation on explanations on all notifications. By handling 
		different notifications you can handle the shortcut bar at full extent.

  4)	COXShortcutBar support infotips (or tooltips) for its group headers and child 
  		window items. While infotips for items are child window responsibility the 
		shortcut bar completely support itemtips fot group headers. In order to set 
		tooltip to any header you have to handle  SHBN_GETHEADERINFOTIP notification 
		which notifies that tooltip for the group header is about to be displayed. As in 
		all SHBN_* notifications lParam points to NMSHORTCUTBAR structure which in its 
		turn has lparam element which points to SHBINFOTIP structure which is described 
		below. If you handle this notification fill the SHBINFOTIP structure 
		correspondingly to display the text you need.

		Also COXShortcutBar support any drag and drop operation in next way: if cursor
		being dragged over collapsed group header after some delay this group will be 
		expanded. if you would like to specify your own or additional functionality you 
		can override next virtual functions
 
			// overwrite to provide your own COleDropTarget to handle drag'n'drop operation
			virtual COleDropTarget* GetDropTarget();

			virtual LONG OnDragOver(WPARAM wParam, LPARAM lParam);
			virtual LONG OnDragLeave(WPARAM wParam, LPARAM lParam);

		Alternatively you can handle next notifications that are sent to the parent 
		window:

			SHBN_DRAGLEAVE
			SHBN_DRAGOVER			


  5)	What about using non default child window in shortcut bar? Well, as a matter of 
		fact, you can associate whatever child window (with WS_CHILD style) you want
		with any group and it'll work. But it's good idea to follow next rules while
		developing your own child window to make it fully compatible with COXShortcutBar:

		1.	After child window was associated with a group shortcut bar will send to it
			SHBM_SETSHBGROUP message. lParam will be set to the handle of associated
			group. Child window should handle this message and save the handle of its group
			within shortcut bar. After that child window can cast pointer to its parent 
			window to COXShortcutBar*.

		2.	Visual representation of the child window should depend on shortcut bar styles
			and its group settings. 

		3.	Shortcut bar will send to its child windows next messages to notify them about
			some events that happened in shortcut bar in order to let child windows to react 
			on them correspondingly:

			SHBM_POPULATECONTEXTMENU		Sent by shortcut bar to child window of expanded 
											group before displaying context menu so child 
											window can modify that menu. lParam points to 
											SHBCONTEXTMENU structure which can be updated
											in result of handling of this message.
			SHBM_GROUPINFOCHANGED			Sent by shortcut bar to child window to notify it 
											that corresponding group properties have been 
											changed. lParam is pointer to flags of all changed 
											attributes (like nMask element of SHB_GROUPINFO 
											structure).
			SHBM_SHBINFOCHANGED				Sent by shortcut bar to all child windows to 
											notify them that shortcut bar properties have 
											been changed. lParam is combination of flags of 
											all changed attributes. Can be any combination of:

											SHBIF_SCRLBTNSIZE	-	size of scroll buttons 
																	has been changed
											SHBIF_SCRLDELAY		-	time of scrolling delay 
																	has been changed
											SHBIF_AUTOSCRLDELAY	-	time of autoscrolling 
																	delay has been changed
											SHBIF_GROUPMARGIN	-	size of margin between 
																	groups has been changed
											SHBIF_CHILDMARGINS	-	margins between child 
																	window rect and shortcut
																	bar specially designated 
																	rect for child window 
																	have been changed
											SHBIF_SHBSTYLE		-	style of parent shortcut 
																	control has been changed
			SHBM_DELETESHBGROUP				Sent by shortcut bar to child window to notify it 
											that corresponding group is about to be deleted.

		
		4.	If your child window is item oriented you should send as much related 
			COXShortcutBar notifications on its behalf as appropriate. 


		All in all if you are going to create your own child window it's good idea to take 
		look at the implementation of COXSHBListCtrl because it's fully compatible with
		COXShortcutBar control.
			


*/

class COXShortcutBarSkin;

class OX_CLASS_DECL COXShortcutBar : public CWnd
{
	friend class COXSHBListCtrl;
	friend class COXShortcutBarSkinClassic;
	friend class COXShortcutBarSkinXP;
	friend class COXShortcutBarSkin2003;
	friend class COXShortcutBarSkin;

	DECLARE_DYNCREATE(COXShortcutBar)
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructor of object
	COXShortcutBar();

	
	// --- In  :	pParentWnd	-	pointer to parent window, cannot be NULL
	//				rect		-	window's rectangle
	//				dwBarStyle	-	shortcut bar specific styles. Can be any 
	//								combimation of next:
	//
	//		SHBS_DISABLEDRAGITEM		items won't as drag source
	//		SHBS_DISABLEDROPITEM		won't accept any OLE drag'n'drop object
	//		SHBS_DISABLEDRAGDROPITEM	won't support OLE drag'n'drop (combination
	//									of SHBS_DISABLEDRAGITEM and 
	//									SHBS_DISABLEDROPITEM styles
	//		SHBS_EDITHEADERS			group headers can be edited
	//		SHBS_EDITITEMS				child window's item text can be edited
	//		SHBS_INFOTIP				SHBN_GETHEADERINFOTIP and SHBN_GETITEMINFOTIP 
	//									notifications will be fired (not implemented
	//									in this version)
	//		SHBS_NOSCROLL				scroll buttons won't be displayed in child 
	//									windows
	//		SHBS_BOLDEXPANDEDGROUP		text in expanded group header will be drawn 
	//									using bold font
	//		SHBS_UNDERLINEHOTITEM		text of hot item in expanded group child 
	//									window will be drawn using underline font
	//		SHBS_SHOWACTIVEALWAYS		active item in expanded group child window 
	//									will be shawn even if the shortcut bar 
	//									doesn't have mouse input
	//		SHBS_DISABLEDRAGDROPHEADER	shortcut bar won't send SHBN_BEGINDRAGHEADER 
	//									notification
	//		SHBS_DRAWHEADERDRAGIMAGE	draws header image while dragging it 
	//		SHBS_DRAWITEMDRAGIMAGE		draws child window item image while 
	//									dragging it 
	//		SHBS_AUTOSCROLL				scrolling will be run automatically when 
	//									mouse is over scrolling buttons
	//		SHBS_ANIMATEEXPAND			animation effect will be applied while 
	//									expanding a group
	//		SHBS_AUTOEXPAND				auto expanding of the group if mouse is over 
	//									the group header for some time
	//		SHBS_FLATGROUPBUTTON		group buttons (headers) will be drawn 
	//									using flat style
	//
	//								Note that implementation of some styles behaviour
	//								in application defined child windows is up to a 
	//								programmer. Some style can be ignored by child 
	//								window. Our default child window COXSHBListCtrl
	//								implements all of them.
	//
	//				nId			-	control ID	
	//				dwStyle		-	window style. WS_CHILD must be specified.
	//				dwExStyle	-	window extended style
	// --- Out : 
	// --- Returns:	TRUE if shortcut bar was successfully created and initialized,
	//				or FALSE otherwise
	// --- Effect : Create shortcut bar control
	BOOL Create(CWnd* pParentWnd, const RECT& rect, 
		DWORD dwBarStyle = SHBS_EDITHEADERS|SHBS_EDITITEMS|
		SHBS_DISABLEDRAGDROPITEM|SHBS_DISABLEDRAGDROPHEADER, 
		UINT nID = 0xffff, DWORD dwStyle = WS_CHILD|WS_VISIBLE, 
		DWORD dwExStyle = 0);

// Attributes
public:
	static CLIPFORMAT m_nChildWndItemFormat;

protected:
	// image list used to draw group header images
	CImageList* m_pImageList;
	// drop hilight (drop target) group
	HSHBGROUP m_hDropHilightGroup;
	// expanded group
	HSHBGROUP m_hExpandedGroup;
	// currently being edited group
	HSHBGROUP m_hEditGroup;
	// hot group (user right click over it)
	HSHBGROUP m_hHotGroup;
	// currently being dragged group
	HSHBGROUP m_hDragGroup;
	// pressed group (user left click over it)
	HSHBGROUP m_hPressedGroup;
	// sort order (0 - no order, 1 - ascending, -1 - descending)
	int m_nSortOrder;
	// shortcut bar specific styles
	DWORD m_dwBarStyle;
	// edit control to edit group header text
	COXSHBEdit m_edit;

	// map array to associate handlers of created groups and pointers to corresponding 
	// SHB_GROUPINFO structures
	typedef CMap<HSHBGROUP, HSHBGROUP, LPSHB_GROUPINFO, LPSHB_GROUPINFO> mapHandleToInfo;
	mapHandleToInfo m_mapHandleToInfo;

	// map array to save pointers to allocated SHB_DESCRIPTOR structures
#if _MSC_VER >= 1400
	typedef CMap<LPSHB_DESCRIPTOR, LPSHB_DESCRIPTOR, INT_PTR, INT_PTR> mapDescriptors;
#else
	typedef CMap<LPSHB_DESCRIPTOR, LPSHB_DESCRIPTOR, int, int> mapDescriptors;
#endif
	mapDescriptors m_mapDescriptors;

	// map array to associate handlers of created groups and pointers to associated 
	// COXSHBListCtrl controls
	typedef CMap<HSHBGROUP, HSHBGROUP, COXSHBListCtrl*, COXSHBListCtrl*> mapHandleToListCtrl;
	mapHandleToListCtrl m_mapHandleToListCtrl;

	// map array to associate handlers of created groups and corresponding rectangles
	// to display them on the screen
	typedef CMap<HSHBGROUP, HSHBGROUP, CRect, CRect> mapHandleToBoundRect;
	mapHandleToBoundRect m_mapHandleToBoundRect;

	// rect that is occupied by child window of expanded group
	CRect m_rectChildWnd;

	// customization properties
	//
	// margin between groups
	int m_nGroupMargin;
	// margin for child window rect
	CRect m_rectChildWndMargins;

	// min size of shortcut bar
	CSize m_sizeMin;

	// size of scroll buttons in child window
	CSize m_sizeScrollButton;

	// delay in milliseconds of scrolling
	UINT m_nScrollingDelay;
	// delay in milliseconds of autoscrolling
	UINT m_nAutoScrollingDelay;

	// delay in milliseconds of group autoexpand event (when mouse is over the header
	// of unexpanded group for m_nAutoExpandDelay milliseconds)
	UINT m_nAutoExpandDelay;

	// default COleDropTarget for drag'n'drop operation
	COXSHBDropTarget m_oleDropTarget;

	// flag used in drawing routines and specifies that currently we are creating
	// drag image of group header
	BOOL m_bCreatingDragImage;

private:
	// internal counter of created group handles
	INT_PTR m_nLastHandle;
	// default font used to draw text in group header
	CFont m_fontDefault;

	// image list used to draw dragging group header
	CImageList* m_pDragImage;
	// point in drag image where the mouse cursor point to
	CPoint m_ptDragImage;

	// timer for start drag of group header
	UINT_PTR m_nWaitForDragHeaderID;
	// probable drag group (we use some time interval to make sure that user 
	// really want to the drag group header)
	HSHBGROUP m_hSuspectDragGroup;

	// timer for group auto expand
	UINT_PTR m_nWaitForAutoExpandID;
	// probable auto expand group (we use some time interval and after it expires 
	// we expand the group if mouse cursor is still over it)
	HSHBGROUP m_hSuspectAutoExpandGroup;

	// timer for checking which group header the mouse cursor is over
	UINT_PTR m_nCheckMouseIsOverGroupID;
	// "hover over" group (mouse is over it)
	HSHBGROUP m_hLastMouseIsOverGroup;

	COXShortcutBarSkin* m_pShortcutBarSkin;
	COXShortcutBarSkin* GetShortcutBarSkin();

// Operations
public:

	// --- In  : dwBarStyle -- the shortcut bar style to verify 
	// --- Out : 
	// --- Returns: TRUE if the specified shortcut bar style is valid,
	//              or FALSE otherwise
	// --- Effect : 
	BOOL VerifyBarStyle(DWORD dwBarStyle) const;
	
	// --- In  : dwStyle -- the window style to verify
	// --- Out : 
	// --- Returns: TRUE if the specified window style is valid to be used as 
	//				shortcut bar window style, or FALSE otherwise
	// --- Effect : 
	BOOL VerifyWindowStyle(DWORD dwStyle) const;
	
	// --- In  : nImage -- the image you are looking for
	// --- Out : 
	// --- Returns: TRUE if the specified image index is in valid range,
	//              or FALSE otherwise
	// --- Effect : 
	BOOL VerifyImage(int nImage) const;
	
	// --- In  : nView -- type of shortcut bar view. At the moment defined only:
	//
	//			SHB_LARGEICON		Icons are large, text is under image, item is 
	//								centered.
	//			SHB_SMALLICON		Icons are small, text is at the right side of 
	//								image, item is left aligned.
	// --- Out : 
	// --- Returns: TRUE if the specified view type is valid,
	//              or FALSE otherwise
	// --- Effect : 
	BOOL VerifyView(int nView) const;
	
	// --- In  :	nOrder	-	zero-based index of order of group
	// --- Out : 
	// --- Returns: TRUE if the specified order is in valid range,
	//              or FALSE otherwise
	// --- Effect : 
	BOOL VerifyOrder(int nOrder) const;
	
	// --- In  : pDescriptor -- pointer to group descriptor
	// --- Out : 
	// --- Returns: TRUE if the specified descriptor is valid,
	//              or FALSE otherwise
	// --- Effect : 
	BOOL VerifyDescriptor(LPSHB_DESCRIPTOR pDescriptor) const;
	
	// --- In  : hwndChild -- handle to a child window
	// --- Out : 
	// --- Returns: TRUE if hwndChild is valid to be used as group child window, 
	//				FALSE otherwise
	// --- Effect : 
	BOOL VerifyChildWnd(HWND hwndChild) const;
	
	// --- In  : nMask -- mask of flags used in SHB_GROUPINFO structure
	// --- Out : 
	// --- Returns: TRUE if mask consist of valid flags, FALSE otherwise
	// --- Effect : verifies that the mask is correct
	BOOL VerifyMask(UINT nMask) const;
	
	// --- In  : pGroupInfo	-	pointer to group info structure
	// --- Out : 
	// --- Returns: TRUE if group info is valid, FALSE otherwise
	// --- Effect : Consecutively calls Verify* function on all elements
	//				of SHB_GROUPINFO structure
	BOOL VerifyGroupInfo(LPSHB_GROUPINFO pGroupInfo) const;


	// the only leagal way of creating descriptor to be used with groups
	LPSHB_DESCRIPTOR AddDescriptor();

	
	// copy one SHB_GROUPINFO to another
	BOOL CopyGroupInfo(LPSHB_GROUPINFO pDest, const LPSHB_GROUPINFO pSource, 
		int nCopyFlag=SHB_CPYINFO_SET);
	// copy one SHB_DESCRIPTOR to another
	BOOL CopyDescriptor(LPSHB_DESCRIPTOR pDest, const LPSHB_DESCRIPTOR pSource, 
		int nCopyFlag=SHB_CPYINFO_SET) const;
	// copy one LOGFONT to another
	BOOL CopyLogFont(LPLOGFONT pDest, const LPLOGFONT pSource) const;


	// 
	// --- In  : pNMSHB	-	pointer to NMSHORTCUTBAR that is set as lParam in 
	//						notifications sent to shortcut bar parent window.
	//						This structure is defined as:
	//		
	//		typedef struct _tagNMSHORTCUTBAR
	//		{
	//			// standard header
	//			NMHDR hdr;
	//			// handle to the group
	//			HSHBGROUP hGroup;
	//			// handle to the item
	//			int nItem;
	//			// additional info, depends on notification
	//			LPARAM lParam;
	//
	//		............................
	//
	//		} NMSHORTCUTBAR, * LPNMSHORTCUTBAR;
	//
	//
	// --- Out : 
	// --- Returns: result of notification handling.
	// --- Effect : helper function to send shortcut bar notifications to its 
	//				parent window
	LRESULT SendSHBNotification(LPNMSHORTCUTBAR pNMSHB);

	// --- In  : dwBarStyle -- the shortcut bar specific style to set. Can be any 
	//							combimation of next:
	//
	//		SHBS_DISABLEDRAGITEM		items won't as drag source
	//		SHBS_DISABLEDROPITEM		won't accept any OLE drag'n'drop object
	//		SHBS_DISABLEDRAGDROPITEM	won't support OLE drag'n'drop (combination
	//									of SHBS_DISABLEDRAGITEM and 
	//									SHBS_DISABLEDROPITEM styles
	//		SHBS_EDITHEADERS			group headers can be edited
	//		SHBS_EDITITEMS				child window's item text can be edited
	//		SHBS_INFOTIP				SHBN_GETHEADERINFOTIP and SHBN_GETITEMINFOTIP 
	//									notifications will be fired (not implemented
	//									in this version)
	//		SHBS_NOSCROLL				scroll buttons won't be displayed in child 
	//									windows
	//		SHBS_BOLDEXPANDEDGROUP		text in expanded group header will be drawn 
	//									using bold font
	//		SHBS_UNDERLINEHOTITEM		text of hot item in expanded group child 
	//									window will be drawn using underline font
	//		SHBS_SHOWACTIVEALWAYS		active item in expanded group child window 
	//									will be shawn even if the shortcut bar 
	//									doesn't have mouse input
	//		SHBS_DISABLEDRAGDROPHEADER	shortcut bar won't send SHBN_BEGINDRAGHEADER 
	//									notification
	//		SHBS_DRAWHEADERDRAGIMAGE	draws header image while dragging it 
	//		SHBS_DRAWITEMDRAGIMAGE		draws child window item image while 
	//									dragging it 
	//		SHBS_AUTOSCROLL				scrolling will be run automatically when 
	//									mouse is over scrolling buttons
	//		SHBS_ANIMATEEXPAND			animation effect will be applied while 
	//									expanding a group
	//		SHBS_AUTOEXPAND				auto expanding of the group if mouse is over 
	//									the group header for some time
	//		SHBS_AUTOEXPAND				auto expanding of the group if mouse is over 
	//									the group header for some time
	//		SHBS_FLATGROUPBUTTON		group buttons (headers) will be drawn 
	//									using flat style
	//							Note that implementation of some styles behaviour
	//							in application defined child windows is up to a 
	//							programmer. Some style can be ignored by child 
	//							window. Our default child window COXSHBListCtrl
	//							implements all of them.
	// --- Out : 
	// --- Returns: TRUE if successful or FALSE otherwise
	// --- Effect : sets the style of this shortcut bar to the style specified in 
	//				dwBarStyle
	BOOL SetBarStyle(DWORD dwBarStyle);

	// --- In  : 
	// --- Out : 
	// --- Returns: shortcut bar specific style
	// --- Effect : allows the program to determine the current shortcut bar specific 
	//				style
	inline DWORD GetBarStyle() const { return m_dwBarStyle; }


	// --- In  : pImageList -- pointer to the image list for the shortcut bar
	// --- Out : 
	// --- Returns: a pointer to the previous image list.
	// --- Effect : sets the imagelist for the shortcut bar
	CImageList* SetImageList(CImageList* pImageList);

	// --- In  : 
	// --- Out : 
	// --- Returns: a pointer to the image list for shortcut bar
	// --- Effect : 
	inline CImageList* GetImageList() const { return m_pImageList; }


	// --- In  : 
	// --- Out : 
	// --- Returns: the edit control used to edit group header text
	// --- Effect : 
	inline COXSHBEdit* GetEditControl() { return &m_edit; }


	// --- In  : 
	// --- Out : 
	// --- Returns: the number of groups on the shortcut bar
	// --- Effect : 
	inline INT_PTR GetGroupCount() const { return m_mapHandleToInfo.GetCount(); }


	// --- In  : 
	// --- Out : 
	// --- Returns: 
	// --- Effect : redraws the child window of expanded group
	inline void RedrawChildWnd() { ShowChildWnd(m_hExpandedGroup); }

	// --- In  : 
	// --- Out : 
	// --- Returns: 
	// --- Effect : redraws the entire shortcut bar
	inline void RedrawBar() 
	{ 
		RedrawChildWnd(); 
		RedrawWindow();
	}


	// --- In  : 
	// --- Out : 
	// --- Returns: last sort order applied to shortcut bar groups:
	//				0	-	no order
	//				1	-	ascending
	//				-1	-	descending
	// --- Effect : 
	inline int GetSortOrder() const { return m_nSortOrder; }


	///////////////////////////////////////////////////////
	// functions to work with groups
	//

	// --- In  : hGroup -- handle to a group
	// --- Out : 
	// --- Returns: TRUE if the group's child window is default COXSHBListCtrl, 
	//				FALSE otherwise
	// --- Effect : 
	BOOL ChildWndIsLC(HSHBGROUP hGroup) const;

	// --- In  : hGroup - handle to a group
	// --- Out : 
	// --- Returns: a pointer to the COXSHBListCtrl list control on success 
	//				or NULL if hGroup has non default child window (application defined)
	// --- Effect : 
	COXSHBListCtrl* GetGroupListCtrl(HSHBGROUP hGroup) const;


	// --- In  : hGroup     -- handle to the group for which information is needed
	//           pGroupInfo -- a pointer to a SHB_GROUPINFO structure to hold the 
	//						   information for the group. It should be created by caller
	//           bSubstitue -- if this is set to TRUE then all information will be 
	//						   retrieved into pGroupInfo, if this is FALSE then only 
	//						   the information requested through pGroupInfo->nMask 
	//						   will be returned
	// --- Out : 
	// --- Returns: TRUE if successful or FALSE otherwise
	// --- Effect : The programmer may request all group information(bSubstitute = TRUE, 
	//				pGroupInfo contains is ignored) or the programmer may request 
	//				specific information(bSubstitute = TRUE, pGroupInfo->nMask contains 
	//				the appropriate mask to obtain the requested information).
	BOOL GetGroupInfo(
		HSHBGROUP hGroup, LPSHB_GROUPINFO pGroupInfo, BOOL bSubstitute=FALSE);
	
	// --- In  : hGroup     -- handle to the group for which information will be set
	//           pGroupInfo -- a pointer to a SHB_GROUPINFO structure that hold the 
	//						   information to be set for the group
	//           bSubstitue -- if this is set to TRUE then all information will be 
	//						   copied from pGroupInfo, if this is FALSE then only the 
	//						   information designated through pGroupInfo->nMask will 
	//						   be set
	// --- Out : 
	// --- Returns: TRUE if successful or FALSE otherwise
	// --- Effect : The programmer may want to change entirely the group information 
	//				(bSubstitute = TRUE) or the programmer may want to change specific 
	//				information (bSubstitute = TRUE, pGroupInfo->nMask contains the 
	//			    appropriate mask to set the designated information)
	BOOL SetGroupInfo(
		HSHBGROUP hGroup, const LPSHB_GROUPINFO pGroupInfo, BOOL bSubstitute=FALSE);


	// Following SetGroup* functions eventually call SetGroupInfo
	//

	// --- In  : hGroup     -- handle to the group for which information is needed
	// --- Out : 
	// --- Returns:  the index (from the image list) of the image for this group header
	// --- Effect : 
	int GetGroupImage(HSHBGROUP hGroup) const;

	// --- In  : hGroup     -- handle to the group for which information will be set
	//           nImage     -- the index (from the ImageList) of the image to set for 
	//						   the group header
	// --- Out : 
	// --- Returns: TRUE if successful otherwise FALSE
	// --- Effect : 
	BOOL SetGroupImage(HSHBGROUP hGroup, int nImage);


	// --- In  : hGroup     -- handle to the group for which information is needed
	// --- Out : 
	// --- Returns:  the index (from the ImageList) of the image used for the expanded 
	//				 view of the group header
	// --- Effect : 
	int GetGroupImageExpanded(HSHBGROUP hGroup) const;

	// --- In  : hGroup     -- handle to the group for which information will be set
	//           nImageExpanded -- index (from the Image List) of the image to use for 
	//			 expanded view of group header
	// --- Out : 
	// --- Returns:  TRUE if successful, otherwise FALSE
	// --- Effect : sets the image to use for the expanded view of the specified group
	BOOL SetGroupImageExpanded(HSHBGROUP hGroup, const int nImageExpanded);


	// --- In  : hGroup     -- handle to the group for which information is needed
	// --- Out : 
	// --- Returns:  the text that is displayed in the header for the group
	// --- Effect : 
	CString GetGroupText(HSHBGROUP hGroup) const;
	
	// --- In  : hGroup     -- handle to the group for which information will be set
	//           sText      -- the text to display in the header of the group
	//           pszText    -- the text to display in the header of the group
	//           nTextMax   -- the length of the string in pszText
	// --- Out : 
	// --- Returns:  TRUE if the text was set successfully, FALSE otherwise
	// --- Effect : sets the display text for any group specified by hGroup
	BOOL SetGroupText(HSHBGROUP hGroup, CString sText);
	BOOL SetGroupText(HSHBGROUP hGroup, LPCTSTR pszText, int nTextMax);


	// --- In  : hGroup     -- handle to the group for which information is needed
	// --- Out : 
	// --- Returns: an application defined data associated with group
	// --- Effect : this function returns a 32 bit value that can then be translated any
	//              way the programmer would like.
	LPARAM GetGroupData(HSHBGROUP hGroup) const;
	
	// --- In  : hGroup     -- handle to the group for which information will be set
	//           lParam     -- 32 bit value representing the application defined group 
	//						   data
	// --- Out : 
	// --- Returns: TRUE on success, FALSE otherwise
	// --- Effect : 
	BOOL SetGroupData(HSHBGROUP hGroup, LPARAM lParam);


	// --- In  : hGroup     -- handle to the group for which information is needed
	// --- Out : 
	// --- Returns:  the 0 based index of group within the shortcut bar (from top 
	//				 to bottom)
	// --- Effect : 
	int GetGroupOrder(HSHBGROUP hGroup) const;
	
	// --- In  : hGroup     -- handle to the group for which information will be set
	// --- Out : 
	// --- Returns:  TRUE on success, FALSE otherwise
	// --- Effect : sets the index of hGroup within the shortcut bar
	BOOL SetGroupOrder(HSHBGROUP hGroup, int nOrder);


	// --- In  : hGroup     -- handle to the group for which information is needed
	// --- Out : 
	// --- Returns:  SHB_LARGEICON  the group is displayed with large icons
	//               SHB_SMALLICON  the group is displayed with small icons
	// --- Effect : 
	int GetGroupView(HSHBGROUP hGroup) const;
	
	// --- In  : hGroup     -- handle to the group for which information will be set
	//           nView      -- SHB_LARGEICON  display the group with large icons
	//						-- SHB_SMALLICON  display the group with small icons
	// --- Out : 
	// --- Returns:  TRUE on success, FALSE otherwise
	// --- Effect : 
	BOOL SetGroupView(HSHBGROUP hGroup, int nView);


	// --- In  : hGroup     -- handle to the group for which information is needed
	// --- Out : 
	// --- Returns:  a pointer to the groups SHB_DESCRIPTOR structure of the group
	// --- Effect : 
	LPSHB_DESCRIPTOR GetGroupDescriptor(HSHBGROUP hGroup) const;

	// --- In  : hGroup      -- handle to the group for which information will be set
	//							If hGroup is NULL then this function will be applied 
	//							to every group in shortcut bar.
	//           pDescriptor -- pointer to the SHB_DESCRIPTOR structure holding the data 
	//							for the group. 
	// --- Out : 
	// --- Returns:  TRUE if successful, FALSE otherwise
	// --- Effect :  sets the data for specified group. This data relates to the 
	//				 appearance of the group itself
	BOOL SetGroupDescriptor(HSHBGROUP hGroup, const LPSHB_DESCRIPTOR pDescriptor);


	// --- In  : hGroup     -- handle to the group for which information is needed
	// --- Out : 
	// --- Returns: a handle to the child window for this group. If default 
	//				COXSHBListCtrl control is the child window for specified group
	//				then this function returns NULL
	// --- Effect : 
	HWND GetGroupChildWnd(HSHBGROUP hGroup) const;
	
	// --- In  : hGroup     -- handle to the group for which information will be set
	//           hwndChild  -- a handle to the child window to be attached to this group
	// --- Out : 
	// --- Returns:  TRUE if successful, FALSE otherwise
	// --- Effect : associates a child window with a group. This child window will be 
	//				displayed when the group is ezpanded. Specified child window must 
	//				exist at the moment.
	BOOL SetGroupChildWnd(HSHBGROUP hGroup, HWND hwndChild);


	// --- In  : hGroup     -- handle to the group for which information is needed
	// --- Out : 
	// --- Returns:  a handle to the child window for this group. If default 
	//				 COXSHBListCtrl control is the child window for specified group
	//				 then this function returns handle to this control 
	// --- Effect : 
	HWND GetGroupChildWndHandle(HSHBGROUP hGroup) const;


	// Following SetGroup* functions eventually call SetGroupDescriptor
	//

	// --- In  : hGroup     -- handle to the group for which information is needed
	// --- Out : 
	// --- Returns:  the background color for the group
	// --- Effect : 
	COLORREF GetGroupBkColor(HSHBGROUP hGroup);

	// --- In  : hGroup     -- handle to the group for which information will be set.
	//						   If hGroup is NULL then this function will be applied 
	//						   to every group in shortcut bar.
	//           clr        -- new background color for the group
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : sets the background color of any group to color specified in clr
	BOOL SetGroupBkColor(HSHBGROUP hGroup, COLORREF clr);


	// --- In  : hGroup     -- handle to the group for which information is needed
	// --- Out : 
	// --- Returns:  the color for the text of the group
	// --- Effect : 
	COLORREF GetGroupTextColor(HSHBGROUP hGroup);

	// --- In  : hGroup     -- handle to the group for which information will be set
	//						   If hGroup is NULL then this function will be applied 
	//						   to every group in shortcut bar.
	//           clr        -- new color for the group text
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : sets the color of the text for any group to color specified in clr
	BOOL SetGroupTextColor(HSHBGROUP hGroup, COLORREF clr);


	// --- In  : hGroup     -- handle to the group for which information is needed
	// --- Out : 
	// --- Returns: the background color of the header for the specified group
	// --- Effect : 
	COLORREF GetGroupHeaderBkColor(HSHBGROUP hGroup) const;

	// --- In  : hGroup     -- handle to the group for which information will be set
	//						   If hGroup is NULL then this function will be applied 
	//						   to every group in shortcut bar.
	//           clr        -- new background color for the group header
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : sets the background color of any group header to color 
	//				specified in clr
	BOOL SetGroupHeaderBkColor(HSHBGROUP hGroup, COLORREF clr);


	// --- In  : hGroup     -- handle to the group for which information is needed
	// --- Out : 
	// --- Returns: the color used to display the Header text of specified group
	// --- Effect : 
	COLORREF GetGroupHeaderTextColor(HSHBGROUP hGroup) const;

	// --- In  : hGroup     -- handle to the group for which information will be set
	//						   If hGroup is NULL then this function will be applied 
	//						   to every group in shortcut bar.
	//           clr        -- new text color for the group header
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : sets the header text color of specified group to color specified in clr
	BOOL SetGroupHeaderTextColor(HSHBGROUP hGroup, COLORREF clr);


	// --- In  : hGroup     -- handle to the group for which information is needed
	// --- Out : 
	// --- Returns: an unsigned integer representing the text format
	// --- Effect : 
	UINT GetGroupHeaderTextFormat(HSHBGROUP hGroup) const;
	
	// --- In  : hGroup     -- handle to the group for which information will be set
	//						   If hGroup is NULL then this function will be applied 
	//						   to every group in shortcut bar.
	//           nFormat    -- the text format for the header
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : sets the display format of the header text for any group as 
	//				specified by hGroup
	BOOL SetGroupHeaderTextFormat(HSHBGROUP hGroup, UINT nFormat);


	// --- In  : hGroup     -- handle to the group for which information is needed
	// --- Out : 
	// --- Returns: a pointer to the LOGFONT structure used to display text in the 
	//				group if successful, otherwise NULL
	// --- Effect : 
	LPLOGFONT GetGroupTextFont(HSHBGROUP hGroup) const;

	// --- In  : hGroup     -- handle to the group for which information will be set
	//						   If hGroup is NULL then this function will be applied 
	//						   to every group in shortcut bar.
	//           pLF        -- pointer to the LOGFONT structure to use
    //			 pFont      -- pointer to the font to use
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : sets the font to use for the group
	BOOL SetGroupTextFont(HSHBGROUP hGroup, const LPLOGFONT pLF);
	BOOL SetGroupTextFont(HSHBGROUP hGroup, CFont* pFont);


	// --- In  : hGroup     -- handle to the group for which information is needed
	// --- Out : 
	// --- Returns: a pointer to the LOGFONT structure used to display text in the 
	//				group header if successful, otherwise NULL
	// --- Effect : 
	LPLOGFONT GetGroupHeaderTextFont(HSHBGROUP hGroup);

	// --- In  : hGroup     -- handle to the group for which information will be set
	//						   If hGroup is NULL then this function will be applied 
	//						   to every group in shortcut bar.
	//           pLF        -- pointer to the LOGFONT structure to use
    //			 pFont      -- pointer to the font to use
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : sets the font to use for the group header
	BOOL SetGroupHeaderTextFont(HSHBGROUP hGroup, const LPLOGFONT pLF);
	BOOL SetGroupHeaderTextFont(HSHBGROUP hGroup, CFont* pFont);


	// --- In  : hGroup     -- handle to the group for which information is needed
	// --- Out : 
	// --- Returns: the height of the Header
	// --- Effect : 
	int GetGroupHeaderHeight(HSHBGROUP hGroup) const;
	
	// --- In  : hGroup     -- handle to the group for which information will be set
	//						   If hGroup is NULL then this function will be applied 
	//						   to every group in shortcut bar.
	//           nHeight    -- new height for the group header
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : 
	BOOL SetGroupHeaderHeight(HSHBGROUP hGroup, int nHeight);

	
	// --- In  : pGroupInfo -- a pointer to an SHB_GROUPINFO structure containing the 
	//						   information for the new group
	// --- Out : 
	// --- Returns: a handle to the new group if successful otherwise NULL
	// --- Effect : 
	HSHBGROUP InsertGroup(const LPSHB_GROUPINFO pGroupInfo);
	
	// --- In  : refer to the description of SHB_GROUPINFO structure elements
	// --- Out : 
	// --- Returns: a handle to the new group if successful otherwise NULL
	// --- Effect : 
	HSHBGROUP InsertGroup(LPCTSTR pszText, int nOrder, int nImage, 
		int nImageExpanded, LPARAM lParam, int nView, 
		LPSHB_DESCRIPTOR pDescriptor, HWND hwndChild);

	// --- In  : refer to the description of SHB_GROUPINFO structure elements
	// --- Out : 
	// --- Returns: a handle to the new group if successful otherwise NULL
	// --- Effect : 
	inline HSHBGROUP InsertGroup(LPCTSTR pszText) 
	{
		return InsertGroup(pszText,-1,-1,-1,NULL,-1,NULL,NULL);
	}

	// --- In  : refer to the description of SHB_GROUPINFO structure elements
	// --- Out : 
	// --- Returns: a handle to the new group if successful otherwise NULL
	// --- Effect : 
	inline HSHBGROUP InsertGroup(LPCTSTR pszText, int nOrder, LPARAM lParam) 
	{
		return InsertGroup(pszText,nOrder,-1,-1,lParam,-1,NULL,NULL);
	}

	// --- In  : refer to the description of SHB_GROUPINFO structure elements
	// --- Out : 
	// --- Returns: a handle to the new group if successful otherwise NULL
	// --- Effect : 
	inline HSHBGROUP InsertGroup(LPCTSTR pszText, int nOrder) 
	{
		return InsertGroup(pszText,nOrder,-1,-1,NULL,-1,NULL,NULL);
	}

	// --- In  : refer to the description of SHB_GROUPINFO structure elements
	// --- Out : 
	// --- Returns: a handle to the new group if successful otherwise NULL
	// --- Effect : 
	inline HSHBGROUP InsertGroup(
		LPCTSTR pszText, int nOrder, int nImage, int nImageExpanded) 
	{
		return InsertGroup(pszText,nOrder,nImage,nImageExpanded,NULL,-1,NULL,NULL);
	}


	// --- In  : hGroup     -- handle to the group 
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : deletes a group from the shortcut bar
	BOOL DeleteGroup(HSHBGROUP hGroup);

	// --- In  : 
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : call this function to remove all groups from the shortcut bar
	BOOL DeleteAllGroups();


	// --- In  : hGroup     -- handle to the group to be expanded
	// --- Out : 
	// --- Returns: the handle to the group
	// --- Effect : will cause a group to expand 
	HSHBGROUP ExpandGroup(HSHBGROUP hGroup);

	// --- In  : 
	// --- Out : 
	// --- Returns: a handle to the expanded group
	// --- Effect : 
	inline HSHBGROUP GetExpandedGroup() const { return m_hExpandedGroup; }


	// --- In  : hGroup     -- handle to the group that is to be set as the drop target
	// --- Out : 
	// --- Returns: the handle to the previous drop target (drop hilight) group
	// --- Effect : call this function to set a group as the target of a drag and drop 
	HSHBGROUP SetDropTargetGroup(HSHBGROUP hGroup);
	
	// --- In  : 
	// --- Out : 
	// --- Returns: a handle to the group which is set as drop target
	// --- Effect : 
	inline HSHBGROUP GetDropHilightGroup() const { return m_hDropHilightGroup; }


	// --- In  : 
	// --- Out : 
	// --- Returns: a handle to the group which header is currently being edited
	// --- Effect : 
	inline HSHBGROUP GetEditGroup() const { return m_hEditGroup; }


	// --- In  : 
	// --- Out : 
	// --- Returns: a handle to the current hot group (user right clicked over it)
	// --- Effect : 
	inline HSHBGROUP GetHotGroup() const { return m_hHotGroup; }

	
	// --- In  : 
	// --- Out : 
	// --- Returns: a handle to the current pressed group (user left clicked over it)
	// --- Effect : 
	inline HSHBGROUP GetPressedGroup() const { return m_hPressedGroup; }

	
	// --- In  : hGroup     -- handle to the group that is being dragged
	// --- Out : 
	// --- Returns: a handle to the group that was being dragged before 
	// --- Effect : sets a group to be dropped at the end of a drag and drop of groups
	//              allows the user to reorganize their groups visually
	HSHBGROUP SetDragGroup(HSHBGROUP hGroup);
	

	// --- In  : 
	// --- Out : 
	// --- Returns: a handle to the group being dragged
	// --- Effect : 
	inline HSHBGROUP GetDragGroup() const { return m_hDragGroup; }


	// --- In  : hGroup     -- handle to the group for which information is needed
	//           lpRect     -- pointer to the RECT structure to receive the information
	// --- Out : 
	// --- Returns: returns TRUE if successful, FALSE otherwise
	// --- Effect : fills the structure pointed to by lpRect with the dimensions of the 
	//              Header rctangle.  
	BOOL GetGroupHeaderRect(HSHBGROUP hGroup, LPRECT lpRect) const;


	// --- In  : hGroup     -- handle to the group
	// --- Out : 
	// --- Returns: a pointer to the edit control used to edit header text if successful,
	//				NULL otherwise
	// --- Effect : 
	COXSHBEdit* EditGroupHeader(HSHBGROUP hGroup);


	// --- In  : pt     -- a CPoint to test
	//           pFlags -- Pointer to an integer that receives information about the 
	//					   results of the test. Can be one of the next:
	//
	//				SHB_ONHEADER		Point is over the header rectangle
	//				SHB_ONCHILD			Point is over the child window rectangle
	//				SHB_NOWHERE			Point is out of shortcut bar rectangle
	//
	// --- Out : 
	// --- Returns: The handle of the group at the position specified by pt, if any, 
	//				or NULL otherwise.
	// --- Effect : determines if there is a group at the point specified
	HSHBGROUP HitTest(CPoint pt, UINT* pFlags) const;


	// --- In  : hGroup     -- handle to a group
	// --- Out : 
	// --- Returns: a pointer to the image if successful, NULL otherwise
	// --- Effect : creates an image to be displayed while the group is being dragged. 
	//				It's caller responsibility to delete returned image list object 
	//				afterwards
	virtual CImageList* CreateGroupDragImage(HSHBGROUP hGroup);


	// --- In  : nSortOrder -- an integer representing the sorting to be done
	//
	//						     -1 sort descending
	//						      0 no sorting
	//							  1 sort ascending 
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : sort groups in alphabetical order
	BOOL SortGroups(int nSortOrder);
	
	// --- In  : lpfnCompare --  a pointer to a function to compare the groups
	//                         this function must be defined in the following manner:
	//
	//						int (CALLBACK *PFNSHBCOMPARE)(LPARAM lParam1, 
	//								LPARAM lParam2, LPARAM lParamSort);

	//					    it should return -1 for descending
	//			                  0 for equallity
	//							  1 for ascending
	//
	//						 lParam1	 --	handle to the first group
	//					     lParam2	 --	handle to the second group (to be compared 
	//										to the first group)
	//						 lParamSort	 --	function specific data, the second argument 
	//										of SortGroupsCB() function 
	//
	//			lParamSoft	-	application defined data that will be sent to callback
	//							function
	//			nSortOrder	-	sort order:
	//
	//					     -1 sort descending
	//					      0 no sorting
	//						  1 sort ascending 
	//
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : 
	BOOL SortGroupsCB(PFNSHBCOMPARE lpfnCompare, LPARAM lParamSort, int nSortOrder);


	// --- In  : pGroupInfo -- a pointer to the group info to be found by
	// --- Out : 
	// --- Returns: a handle to the found group if successful, NULL otherwise
	// --- Effect : searches the shortcut bar group which group info is superset of 
	//				specified pGroupInfo (all of elements from pGroupInfo are the same 
	//				as in given group info)
	HSHBGROUP FindGroup(const LPSHB_GROUPINFO pGroupInfo) const;
	
	// --- In  : nOrder  -- an integer indicating the 0 based index of the 
	//						group (from top to bottom)
	// --- Out : 
	// --- Returns: a handle to the group if found, NULL otherwise
	// --- Effect : searches the shortcut bar group by its order.
	HSHBGROUP FindGroupByOrder(int nOrder) const;

	// --- In  : lParam  -- data associated with the group
	// --- Out : 
	// --- Returns: a handle to the group if found, NULL otherwise
	// --- Effect : searches the shortcut bar group by its data.
	HSHBGROUP FindGroupByParam(LPARAM lParam) const;

	// --- In  : lpszTitle  -- group's title
	// --- Out : 
	// --- Returns: a handle to the group if found, NULL otherwise
	// --- Effect : searches the shortcut bar group by its tile.
	HSHBGROUP FindGroupByTitle(LPCTSTR lpszTitle) const;


	// --- In  : hGroup     -- handle to a group
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : updates the header display for group hGroup
	BOOL UpdateHeader(HSHBGROUP hGroup);



	// functions to work with group list controls (defined only if programmer didn't set 
	// hwndChild member of SHB_GROUPINFO structure)
	//

	// --- In  : hGroup     -- handle to the group for the list control
	//           pImageList -- a pointer to the image list for the list control
	//           nImageList -- Type of image list. It can be one of these values:
	//
	//				LVSIL_NORMAL   Image list with large icons.
	//				LVSIL_SMALL    Image list with small icons.
	//
	// --- Out : 
	// --- Returns: a pointer to the previous image list.
	// --- Effect: Assigns an image list to the list view control for group hGroup
	CImageList* SetLCImageList(
		HSHBGROUP hGroup, CImageList* pImageList, int nImageList);

	// --- In  : hGroup     -- handle to the group
	//           nImageList -- Type of image list. It can be one of these values:
	//
	//				LVSIL_NORMAL   Image list with large icons.
	//				LVSIL_SMALL    Image list with small icons.
	//
	// --- Effect : sets an image list for the list control of the group
	CImageList* GetLCImageList(HSHBGROUP hGroup, int nImageList) const;


	// --- In  : hGroup     -- handle to the group
	//           pItem      -- a pointer to an LV_ITEM structure to retrieve the 
	//						   information about list control item
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : functions the same as CListCtrl::GetItem(LV_ITEM* pItem) except that 
	//				the group must be specified for the shortcut bar
	BOOL GetLCItem(HSHBGROUP hGroup, LV_ITEM* pItem) const;


	// --- In  : hGroup     -- handle to the group
	// --- Out : 
	// --- Returns: number of items in the list control associated with the 
	//				specified group
	// --- Effect : functions the same as CListCtrl::GetItem(LV_ITEM* pItem) except that 
	//				the group must be specified for the shortcut bar
	int GetLCItemCount(HSHBGROUP hGroup) const;


	// --- In  :	hGroup		-	Handle to the group
	//				pItem		-	A pointer to an LV_ITEM structure to set the 
	//								information about list control item
	//				nItem		-	Index of the item whose attributes are to be set
	//				nSubItem	-	Index of the subitem whose attributes are to be 
	//								set. Always must be zero
	//				nMask		-	Specifies which attributes are to be set
	//				lpszItem	-	Address of a null-terminated string specifying 
	//								the item's label
	//				nImage		-	Index of the item's image within the image list
	//				nState		-	Specifies values for states to be changed
	//				nStateMask	-	Specifies which states are to be changed
	//				lParam		-	A 32-bit application-specific value to be 
	//								associated with the item.
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : functions the same as CListCtrl::SetItem(LV_ITEM* pItem) except 
	//				that the group must be specified for the shortcut bar
	BOOL SetLCItem(HSHBGROUP hGroup, const LV_ITEM* pItem);
	BOOL SetLCItem(HSHBGROUP hGroup, int nItem, int nSubItem, UINT nMask, 
		LPCTSTR lpszItem, int nImage, UINT nState, UINT nStateMask, LPARAM lParam);


	// --- In  :	hGroup		-	Handle to the group
	//				pItem		-	A pointer to an LV_ITEM structure with information
	//								about the new list control item
	//				nItem		-	Index of the item to be inserted
	//				lpszItem    -	Address of a string containing the item's label 
	//				nImage		-	Index of the item's image
	//				nMask		-	Specifies which attributes are valid
	//				nState		-	Specifies values for states to be changed
	//				nStateMask	-	Specifies which states are valid
	//				lParam		-	A 32-bit application-specific value associated 
	//								with the item.
	// --- Out : 
	// --- Returns: 0 based index of the item if successful, -1 otherwise
	// --- Effect : functions are the same as corresponding CListCtrl::InsertItem() 
	//				functions except that the group must be specified for the 
	//				shortcut bar
	int InsertLCItem(HSHBGROUP hGroup, const LV_ITEM* pItem);
	int InsertLCItem(HSHBGROUP hGroup, UINT nMask, int nItem, LPCTSTR lpszItem, 
		UINT nState, UINT nStateMask, int nImage, LPARAM lParam);
	inline int InsertLCItem(HSHBGROUP hGroup, int nItem, LPCTSTR lpszItem) 
	{
		return InsertLCItem(hGroup,LVIF_TEXT,nItem,lpszItem,0,0,0,-1);
	}
	inline int InsertLCItem(HSHBGROUP hGroup, int nItem, LPCTSTR lpszItem, int nImage)
	{ 
		return InsertLCItem(hGroup,LVIF_TEXT|LVIF_IMAGE,nItem,lpszItem,0,0,nImage,-1);
	}
	inline int InsertLCItem(
		HSHBGROUP hGroup, int nItem, LPCTSTR lpszItem, int nImage, LPARAM lParam) 
	{ 
		return InsertLCItem(
			hGroup,LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM,nItem,lpszItem,0,0,nImage,lParam);
	}


	// --- In  : hGroup     -- handle to the group
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : sorts items inlist control associated with specified group
	BOOL SortLCItems(HSHBGROUP hGroup);


	// --- In  : hGroup     -- handle to the group
	//           nItem      -- 0 based index of the item to delete
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : functions the same as CListCtrl::DeleteItem(int nItem) except that 
	//				the group must be specified for the shortcut bar
	BOOL DeleteLCItem(HSHBGROUP hGroup, int nItem);


	// --- In  : hGroup     -- handle to the group
	//           nItem      -- 0 based index of the item to edit
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : functions the same as CListCtrl::EditLabel(int nItem) except that 
	//				the group must be specified for the shortcut bar
	COXSHBEdit* EditLCItem(HSHBGROUP hGroup, int nItem);


	// --- In  : size  -- the amount of scrolling to be performed in pixels. 
	//					  Currently only vertical scrolling is supported.
	// --- Out : 
	// --- Returns: TRUE if successful, FALSE otherwise
	// --- Effect : will scroll the list control a certain amount specified in size
	BOOL ScrollExpandedLC(CSize size);


	// --- In  : 
	// --- Out : 
	// --- Returns: size of the expanded group child window 
	// --- Effect : 
	inline CRect GetChildWndRect() const { return m_rectChildWnd; }


	// customization methods
	//

	// --- In  : 
	// --- Out : 
	// --- Returns: the margin between groups in the shortcut bar
	// --- Effect : 
	inline int GetGroupMargin() { return m_nGroupMargin; }
	
	// --- In  : nGroupMargin  -- an integer containing the size of the margin to be 
	//			set between groups in the shortcut bar
	// --- Out : 
	// --- Returns: the previous margin settings
	// --- Effect : 
	int SetGroupMargin(int nGroupMargin);


	// --- In  : 
	// --- Out : 
	// --- Returns: a CRect containing the dimensions of the child window margins
	// --- Effect : 
	inline CRect GetChildWndMargins() { return m_rectChildWndMargins; }
	
	// --- In  : rectChildWndMargins -- a CRect containing the new dimensions for the 
	//									margins of the child window
	// --- Out : 
	// --- Returns: previous settings for the child window margins
	// --- Effect : sets the margins for the child window
	CRect SetChildWndMargins(CRect rectChildWndMargins);


	// --- In  : 
	// --- Out : 
	// --- Returns: a CSize object containing the minimum size of the shortcut bar
	// --- Effect : 
	inline CSize GetBarMinSize() { return m_sizeMin; }
	
	// --- In  : sizeMin -- a CSize object containing the minimum size of the shortcut 
	//						bar
	// --- Out : 
	// --- Returns: previous settings for the minimum size
	// --- Effect : 
	CSize SetBarMinSize(CSize sizeMin);


	// --- In  : 
	// --- Out : 
	// --- Returns: the size of the scroll button (top and bottom buttons are 
	//				of the same size)
	// --- Effect : 
	inline CSize GetScrollButtonSize() { return m_sizeScrollButton; }

	// --- In  : sizeScrollButton -- a CSize object containing the new size for the 
	//								 scroll button (top and bottom buttons are 
	//				 				 of the same size)
	// --- Out : 
	// --- Returns: previous settings for the size of the scroll button
	// --- Effect : 
	CSize SetScrollButtonSize(CSize sizeScrollButton);


	// --- In  : 
	// --- Out : 
	// --- Returns: the current delay settings for scrolling
	// --- Effect : the scroll delay causes a pause after each new item is displayed 
	//				within a group. To eliminate this delay call SetScrollingDelay(0);
	inline UINT GetScrollingDelay() { return m_nScrollingDelay; }

	// --- In  :   nScrollingDelay  -- the amount of delay between scrolling the items
	//                                 after a new item has been displayed
	// --- Out : 
	// --- Returns: previous scrolling delay
	// --- Effect : the scrolling delay causes a pause after each new item is displayed 
	//				within a group.  To eliminate this delay call SetScrollingDelay(0);
	UINT SetScrollingDelay(UINT nScrollingDelay);


	// --- In  : 
	// --- Out : 
	// --- Returns: the current delay settings for autoscrolling
	// --- Effect : this is the same as GetScrollingDelay except that the autoscrolling 
	//				feature only requires the user to move the mouse over the scroll 
	//				button and not click. This is a unique shortcut bar feature.
	inline UINT GetAutoScrollingDelay() { return m_nAutoScrollingDelay; }

	// --- In  :   nScrollingDelay  -- the amount of delay between scrolling the items 
	//								   after a new item has been displayed
	// --- Out : 
	// --- Returns: previous delay settings for autoscrolling
	// --- Effect : sets the pause on the autoscrolling feature.  This feature simply 
	//				requires a mouse presence over the scroll button to scroll, it does 
	//				not require the user to click the scroll button.
	UINT SetAutoScrollingDelay(UINT nAutoScrollingDelay);


	// --- In  : 
	// --- Out : 
	// --- Returns: the current delay settings for autoexpand event
	// --- Effect : if mouse cursor is over the header of an unexpanded group and 
	//				if the corresponding SHBS_AUTOEXPAND style is set then after 
	//				some time this group will be automatically expanded. This function
	//				returns the time in milliseconds the shortcut bar will wait.
	inline UINT GetAutoExpandDelay() { return m_nAutoExpandDelay; }

	// --- In  :   nAutoExpandDelay  -- the amount of time in milliseconds the
	//									shortcut bar will wait until automatically 
	//									expand an unexpanded group if mouse cursor is
	//									over its header for designated time.
	// --- Out : 
	// --- Returns: previous autoexpand delay
	// --- Effect : if mouse cursor is over the header of an unexpanded group and 
	//				if the corresponding SHBS_AUTOEXPAND style is set then after 
	//				some time this group will be automatically expanded. This function
	//				returns the time in milliseconds the shortcut bar will wait.
	UINT SetAutoExpandDelay(UINT nAutoExpandDelay);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXShortcutBar)
	public:
	virtual BOOL OnCmdMsg(
		UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL


// Implementation
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructor of object
	virtual ~COXShortcutBar();


	// --- In  :	point	-	Specifies the x- and y-coordinate of the cursor. 
	//							These coordinates are always relative to the upper-left 
	//							corner of the window
	//				pTI		-	A pointer to a TOOLINFO structure
	// --- Out : 
	// --- Returns:	If -1, the tooltip control was not found, otherwise was found
	// --- Effect : this function is called to detemine whether a point is in the 
	//				bounding rectangle of the specified tool. If the point is in the 
	//				rectangle, it retrieves information about the tool. If the area 
	//				with which the tooltip is associated is not a button, OnToolHitTest 
	//				sets the structure flags to TTF_NOTBUTTON and TTF_CENTERTIP. 
	//				See TOOLINFO, in the Win32 SDK Programmer's Reference, for more 
	//				information about the structure.
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;


protected:
	// overwrite this function to provide your own header fdrawing routine
	virtual void DrawHeader(LPDRAWITEMSTRUCT lpDrawItemStruct);
	// fill background of shortcut bar
	virtual void FillBackground(CDC* pDC);

	// create edit control
	virtual BOOL CreateEditControl();
	// create list control for specified group
	virtual BOOL CreateListControl(HSHBGROUP hGroup);
	// creates new instance of COXSHBListCtrl object
	virtual COXSHBListCtrl* NewSHBListCtrl();

	// calculate positions for all group
	virtual void CalculateBoundRects();

	// request on unique header handle
	virtual HSHBGROUP GetUniqueGroupHandle();
	// compare two groups
	virtual BOOL CompareGroups(const LPSHB_GROUPINFO pGroup, 
		const LPSHB_GROUPINFO pGroupCompareTo) const;
	// overwrite to provide your own COleDropTarget to handle drag'n'drop operation
	virtual COleDropTarget* GetDropTarget();

	// Generated message map functions
protected:
	//{{AFX_MSG(COXShortcutBar)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
#if _MSC_VER >= 1400
	afx_msg LRESULT OnNcHitTest(CPoint point);
#else
	afx_msg UINT OnNcHitTest(CPoint point);
#endif
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg BOOL OnHeaderToolTip(UINT id, NMHDR* pTTTStruct, LRESULT* pResult);

	afx_msg void OnChangeGroupHeaderText(NMHDR* pNotifyStruct, LRESULT* result);

	virtual LONG OnDragOver(WPARAM wParam, LPARAM lParam);
	virtual LONG OnDragLeave(WPARAM wParam, LPARAM lParam);

	// handlers for default context menu elements
	afx_msg void OnLargeIcon();
	afx_msg void OnSmallIcon();
	afx_msg void OnAddNewGroup();
	afx_msg void OnRemoveGroup();
	afx_msg void OnRenameGroup();
	afx_msg void OnRemoveLCItem();
	afx_msg void OnRenameLCItem();

	afx_msg void OnUpdateLargeIcon(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSmallIcon(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

protected:
	// initialize shortcut bar: create edit and tooltip controls
	BOOL InitShortcutBar();

	// update order of groups in result of deleting or inserting a group
	void UpdateHeaderOrder(int nFirst, int nLast, int nMargin);
	// update size of group bound rects 
	void UpdateBoundRects(int nStartFrom, int nMargin);

	// hide child window
	void HideChildWnd(HSHBGROUP hGroup);
	// show (and redraw if requested) child wndow
	void ShowChildWnd(HSHBGROUP hGroup, BOOL bRedraw = TRUE);

	// reset handle to the group that is positioned under mouse cursor
	void SetMouseIsOverGroup(HSHBGROUP hGroup);

#ifndef _INC_WINDOWSX
	// return TRUE if mouse left button is pressed down
	inline BOOL IsLButtonDown() { return (GetKeyState(VK_LBUTTON)<0);}
#endif

	// function to compare groups by header's text
	static int CALLBACK CompareHeaders(
		LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

private:
	BOOL HasGroupBkColor(HSHBGROUP hGroup);
};

/////////////////////////////////////////////////////////////////////////////

// helper functions
//
// function to manipulate colors
COLORREF UpdateColor(COLORREF clr, int nOffset);
COLORREF InvertColor(COLORREF clr);
BOOL IsColorCloseTo(COLORREF clr,COLORREF clrCompareTo,UINT nMaxMargin);
void* CopyImageFromIL(CImageList* pil, int nImage, DWORD& nImageSize);
int CopyImageToIL(CImageList* pil, void* pBuffer, DWORD nBufferLength);

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(_OX_SHORTCUTBAR_)
