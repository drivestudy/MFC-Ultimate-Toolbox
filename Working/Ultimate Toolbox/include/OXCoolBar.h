#if _MFC_VER>=0x0420

// Version: 9.3

////////////////////////////////////////////////////////
// CoolBar control is based on standard REBAR control which
// is included in comctl32.dll starting from version 4.70
// (supplied with IE 3.0 and now freely redistributable).
// That means that you can use it only with MFC version >=4.2
////////////////////////////////////////////////////////

#ifndef _COOLBAR_H
#define _COOLBAR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"


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

#ifdef WIN32
#ifndef __AFXTEMPL_H__
	#include <afxtempl.h>		// MFC extensions for Templates
#endif
#endif

// this value is associated with CoolBar by default
#ifndef IDW_COOLBAR
#define IDW_COOLBAR		0xEF00
#endif

static const TCHAR szMask[]=_T("Mask");
static const TCHAR szStyle[]=_T("Style");
static const TCHAR szcxMin[]=_T("cxMin");
static const TCHAR szcyMin[]=_T("cyMin");
static const TCHAR szWidth[]=_T("Width");
static const TCHAR szText[]=_T("Text");
static const TCHAR szImage[]=_T("ImageIndex");
static const TCHAR szID[]=_T("ID");
static const TCHAR szIndex[]=_T("Index");
static const TCHAR szBandCount[]=_T("BandCount");
static const TCHAR szBandSection[]=_T("%s_Band_%d");

////////////////////////////////////////////////////////
// CoolBar controls act as containers for child windows. 
// An application assigns child windows, which are often 
// other controls, to a CoolBar control band. CoolBar 
// controls contain one or more bands, and each band can 
// have any combination of a gripper bar, a bitmap, 
// a text label, and a child window. However, bands cannot 
// contain more than one child window. 
//
// A CoolBar control displays the child window over a specified 
// background bitmap. As you dynamically reposition a CoolBar 
// control band, the CoolBar control manages the size and 
// position of the child window assigned to that band. 
//
// 
// All CoolBar control bands can be resized, except those 
// that use the RBBS_FIXEDSIZE style. To resize or change 
// the order of bands within the control, click and drag 
// a band's gripper bar. The CoolBar control automatically 
// resizes and repositions child windows assigned to its 
// bands. Additionally, you can toggle the size of a band 
// by clicking on the band text, if there is any. 
//
//
//
//		Utilization of this class (in case with standard project created 
//			using MFC App Wizard):
//		Usually you would use a CoolBar control within your MDI or SDI 
//		framework. If this is the case then:
//		1)	Add #include "OXCoolBar.h" to your "MainFrm.h" header file
//		2)	Add variable of COXCoolBar class to your CMainFrame class definition:
//				COXCoolBar m_wndCoolBar;			
//		3)	Initialization of COXCoolBar control goes in two steps: contruction
//			of object and calling COXCoolBar::Create function to really 
//			create the object. The best place to do that is in your 
//			CMainFrame::OnCreate function:
//
//				if (!m_wndCoolBar.Create(this,
//					WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|
//					RBS_TOOLTIPS|RBS_BANDBORDERS|RBS_VARHEIGHT)) 
//				{
//					TRACE(_T("Failed to create cool bar\n"));
//					return -1;      // fail to create
//				}
//	
//			 (Refer to the documentation about styles used to create Coolbar)
//		4)	After creation of CoolBar control you have to populate it with bands.
//			Usually you use some child windows (e.g. ToolBar controls, ComboBox control) 
//			within bands. Therefore all these child windows must be created before
//			inserting into bands. E.g. if you want to use ToolBar control within CoolBar
//			you have to create it first of all:
//
//				if (!m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP,
//					ID_VIEW_TOOLBAR) ||
//					!m_wndToolBar.LoadToolBar(IDR_TOOLBAR))
//				{
//					TRACE(_T("Failed to create toolbar\n"));
//					return -1;      // fail to create
//				} 
//				
//			!!!!!!!
//			We recommend you always set some unique ID to whatever child window you 
//			going to use in CoolBar control. Because you can use the same value to set 
//			ID of corresponding band within the CoolBar
//			!!!!!!!
//			If you use CoolToolBar as child window don't forget to call 
//			COXCoolToolBar::SetCool function to let CoolToolBar control to draw itself 
//			correctly when used within CoolBar:
//				m_wndToolBarFile.SetCool();
//			!!!!!!!
//		5)	If an application is using an image list with a CoolBar control, it must 
//			create object of COXCoolBarInfo class and set it to the CoolBar before adding 
//			bands to the control. Before setting COXCoolBarInfo object to the CoolBar, 
//			call COXCoolBarInfo::SetImageList(HIMAGELIST il) to set an image list:
//
//				// create object of COXCoolBarInfo class to attach image list to the Coolbar
//				COXCoolBarInfo rbi;
//				// if you call COXCoolBar::AddImageList function to set an image list
//				// then CoolBar will take care about creating, saving and
//				// destroying of that image list object
//				rbi.SetImageList(m_wndCoolBar.AddImageList(IDR_COOLBAR_IMAGELIST));
//				if(!m_wndCoolBar.SetBarInfo(&rbi))
//				{
//					TRACE(_T("Failed to set CoolBar info\n"));
//					return -1;      // fail to insert
//				} 
//		6)	After you created all child windows you going to use in the CoolBar and set
//			COXCoolBarInfo object to the control you are ready to create bands and insert
//			them into the CoolBar. Below you will find the scheme how to do that:
//			
//			First of all create an instance of of object of COXCoolBarBandInfo class:
//				COXCoolBarBandInfo rbbi;
//
//			Set all the band's properties and attributes using COXCoolBarBandInfo::SetXXX
//			functions (refer to the documentation to get full list of all functions):
//
//				// we want the background bitmap does not move 
//				// when the band is resized
//				rbbi.SetStyle(RBBS_FIXEDBMP);
//				// set the CoolToolBar as child window
//				rbbi.SetChildWindow(&m_wndToolBar);
//				// set the CoolToolBar window min size
//				// the first argument is width and the second is height
//				rbbi.SetChildWindowMinSize(m_wndToolBar.CalcDynamicLayout(-1,0).cx,
//					m_wndToolBar.CalcDynamicLayout(-1,LM_HORZ).cy-4);
//				// set background bitmap
//				// if you call COXCoolBar::AddBitmap function to add bitmap
//				// then CoolBar will take care about creating, saving and
//				// destroying of bitmap object
//				rbbi.SetBackgroundBitmap(m_wndCoolBar.AddBitmap(IDB_COOLBAR_WALLPAPER));
//				// set the text to the band
//				rbbi.SetText(_T("Toolbar"));
//				// set the zero based index number in image list associated with CoolBar
//				rbbi.SetImage(0);
//				// you have to set to the band an unique, non zero ID
//				rbbi.SetID(ID_VIEW_TOOLBAR);
//
//		7)	After you set all band's properties you wanted to set you can insert the band
//			into the CoolBar control using COXCoolBar::InsertBand function:
//
//				if(!m_wndCoolBar.InsertBand((UINT)-1,&rbbi))
//				{
//					TRACE(_T("Failed to insert band\n"));
//					return -1;      // fail to insert
//				} 
//
//		8) repeat step 6 and 7 as much as the number of band you want to insert
//
////////////////////////////////////////////////////////



////////////////////////////////////////////////////////
// If an application is using an image list with a CoolBar control, 
// it must add REBARINFO structure before adding bands to the control. 
// Use COXCoolBar::SetBarInfo function to do that
////////////////////////////////////////////////////////
//
// Programmer-friendly REBARINFO initializes itself
//
// Here is the definition of REBARINFO structure taken from commctrl.h(4.70)
//
// typedef struct tagREBARINFO
// {
//    UINT        cbSize;
//    UINT        fMask;
// #ifndef NOIMAGEAPIS
//    HIMAGELIST  himl;
// #else
//    HANDLE      himl;
// #endif
// }   REBARINFO, FAR *LPREBARINFO;
//
//	cbSize	- Size of this structure, in bytes.  
//	fMask	- Flag values that describe characteristics of the CoolBar control. 
//				Currently, CoolBar controls support only one value: RBIM_IMAGELIST 
//				The himl member is valid or must be filled. 
//	himl	- Handle to an image list. The CoolBar control will use the specified 
//				image list to obtain images. 
//
////////////////////////////////////////////////////////


class OX_CLASS_DECL COXCoolBarInfo : private REBARINFO 
{
public:
	COXCoolBarInfo() 
	{
		Reset();
	}
// Attributes
public:

protected:
	
// Operations
public:
	inline void Reset() 
	{ 
		ZeroMemory(this,sizeof(REBARINFO));
		// set the size of structure
		cbSize = sizeof(REBARINFO); 
	}


	// If you used COXCoolBar::AddImageList function to create image list
	// object then never destroy it yourself.
	// It's up to CoolBar to do that
	// returns TRUE if image was set and fill il with handle to it, or FALSE otherwise
	inline BOOL GetImageList(HIMAGELIST& il)
	{
		if(fMask&RBIM_IMAGELIST)
		{
			il=himl;
			return TRUE;
		}
		return FALSE;
	}

	// it is your responsibility to create and destroy Image List object 
	// to simplify this task use COXCoolBar::AddImageList function
	// which creates Image List from resource and returns handle to it
	// when corresponding CoolBar destroyed it will destroy all image lists
	// created using COXCoolBar::AddImageList function
	// returns TRUE if successful, or FALSE otherwise
	inline BOOL SetImageList(HIMAGELIST il)
	{
		if(il!=NULL)
		{
			fMask|=RBIM_IMAGELIST;
			himl=il;
			return TRUE;
		}
		return FALSE;
	}

	// RBIM_IMAGELIST - the only flag used at the moment

	// fill nMask with current mask, always return TRUE
	inline BOOL GetMask(UINT& nMask)
	{
		nMask=fMask;
		return TRUE;
	}
	// returns always TRUE
	inline BOOL SetMask(UINT nMask)
	{
		fMask=nMask;
		return TRUE;
	}
};

//////////////////
// Programmer-friendly REBARBANDINFO initializes itself
//
// Here is the definition of REBARBANDINFO structure taken from commctrl.h(4.70)
//
// typedef struct tagREBARBANDINFOW
// {
//    UINT        cbSize;
//    UINT        fMask;
//    UINT        fStyle;
//    COLORREF    clrFore;
//    COLORREF    clrBack;
//    LPWSTR      lpText;
//    UINT        cch;
//    int         iImage;
//    HWND        hwndChild;
//    UINT        cxMinChild;
//    UINT        cyMinChild;
//    UINT        cx;
//    HBITMAP     hbmBack;
//    UINT        wID;
// }   REBARBANDINFOW, FAR *LPREBARBANDINFOW;
//
//	Contains information that defines a band in a CoolBar control. 
//		cbSize		- Size of this structure, in bytes.
//		fMask		- Flags that indicate which members of this structure are valid or 
//				must be filled. This value can be a combination of the following: 
//				RBBIM_BACKGROUND The hbmBack member is valid or must be filled. 
//				RBBIM_CHILD The hwndChild member is valid or must be filled. 
//				RBBIM_CHILDSIZE The cxMinChild and cyMinChild members are 
//					valid or must be filled. 
//				RBBIM_COLORS The clrFore and clrBack members are valid or 
//					must be filled. 
//				RBBIM_ID The wID member is valid or must be filled. 
//				RBBIM_IMAGE The iImage member is valid or must be filled. 
//				RBBIM_SIZE The cx member is valid or must be filled. 
//				RBBIM_STYLE The fStyle member is valid or must be filled. 
//				RBBIM_TEXT The lpText member is valid or must be filled. 
//		fStyle		- Flags that specify the band style. This value can be 
//				a combination of the following: 
//				RBBS_BREAK The band is on a new line. 
//				RBBS_CHILDEDGE The band has an edge at the top and 
//					bottom of the child window. 
//				RBBS_FIXEDBMP The background bitmap does not move 
//					when the band is resized. 
//				RBBS_FIXEDSIZE The band can't be sized. With this style, 
//					the sizing grip is not displayed on the band. 
//				RBBS_HIDDEN The band will not be visible. 
//				RBBS_NOVERT The band won't be displayed when the CoolBar control 
//					uses the CCS_VERT style. 
//		clrFore		- Band foreground and background colors. If hbmBack specifies a 
//		clrBack		background bitmap, these members are ignored. By default, 
//				the band will use the background color of the CoolBar control 
//				set with the COXCoolBarBandInfo::SetBkColor() or 
//				COXCoolBar::SetBandBkColor() function. If a background color is 
//				specified here, then this background color will be used instead. 
//		lpText		- Address of a buffer that contains the display text for the band. 
//				If band information is being requested from the control and 
//				RBBIM_TEXT is specified in fMask, this member must be initialized 
//				to the address of the buffer that will receive the text. 
//		cch			- Size of the buffer at lpText, in bytes. If information is not 
//				being requested from the control, this member is ignored. 
//		iImage		- Zero-based index of any image that should be displayed in the band. 
//				The image list is set using COXCoolBarInfo class (described above)
//		hwndChild	- Handle to the child window contained in the band, if any. 
//		cxMinChild	- Minimum width of the child window, in pixels. 
//				The band can't be sized smaller than this value. 
//		cyMinChild	- Minimum height of the child window, in pixels. 
//				The band can't be sized smaller than this value. 
//		cx			- Length of the band, in pixels. 
//		hbmBack		- Handle to a bitmap that is used as the background for this band.
//		wID			- UINT value that the control uses to identify this band 
//				for custom draw notification messages. This value may be used 
//				for additional purposes in the future. 
//
//	The cxMinChild, cyMinChild, and cx members provide information on dimensions 
//	relative to the orientation of the control. That is, for a horizontal CoolBar 
//	control, cxMinChild and cx are horizontal measurements and cyMinChild is 
//	a vertical measurement. However, if the control uses the CCS_VERT style, 
//	cxMinChild and cx are vertical measurements and cyMinChild is a horizontal 
//	measurement. 
//
//////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//	You have to set non zero, unique wID to every band (demanded by design)
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//////////////////
class OX_CLASS_DECL COXCoolBarBandInfo : private REBARBANDINFO 
{
// Construction
public:
	COXCoolBarBandInfo() 
	{
		lpText=NULL;
		Reset();
	}
	
// Attributes
public:

protected:

private:
	BOOL bDynText;

// Operations
public:
	inline void Reset() 
	{ 
		if(lpText!=NULL)
		{
			free(lpText);
			lpText=NULL;
		}
		bDynText=FALSE;
		ZeroMemory(this, sizeof(REBARBANDINFO));
		cbSize=sizeof(REBARBANDINFO); 
	}

	///////////
	// styles
	///////////
	// RBBS_BREAK			- break to new line
	// RBBS_FIXEDSIZE		- band can't be sized
	// RBBS_CHILDEDGE		- edge around top & bottom of child window
	// RBBS_HIDDEN			- don't show
	// RBBS_NOVERT			- don't show when vertical
	// RBBS_FIXEDBMP		- bitmap doesn't move during band resize
	///////////
	// starting from version 4.71
	// RBBS_GRIPPERALWAYS	- the band will always have sizing grip, 
	//							even if it is the only band in the CoolBar. 
	// RBBS_NOGRIPPER		- the band will never have sizing grip, 
	//							even if there is more than one band in the CoolBar. 
	// RBBS_VARIABLEHEIGHT	- the band can be resized by the CoolBar control. 
	//							cyIntegral and cyMaxChild affect how 
	//							the CoolBar will resize the band. 
	////////////////////////
	// returns TRUE if any style was previously set and 
	// fill nStyle with it, or FALSE otherwise
	BOOL GetStyle(UINT& nStyle);
	// returns TRUE if successful, or FALSE otherwise
	BOOL SetStyle(UINT nStyle);

	// returns TRUE if some foreground color was previously set and 
	// fill crColor with it, or FALSE otherwise
	BOOL GetColor(COLORREF& crColor);
	// returns TRUE if successful, or FALSE otherwise
	BOOL SetColor(COLORREF crColor);

	// returns TRUE if some background color was previously set and 
	// fill crColor with it, or FALSE otherwise
	BOOL GetBkColor(COLORREF& crColor);
	// returns TRUE if successful, or FALSE otherwise
	BOOL SetBkColor(COLORREF crColor);

	// returns TRUE if some text was previously set and 
	// fill sText with it, or FALSE otherwise
	BOOL GetText(CString& sText);
	// returns TRUE if successful, or FALSE otherwise
	BOOL SetText(LPTSTR sText);

	// returns TRUE if some zero-based index in image list associated with CoolBar
	// was previously set and fill nImage with that index, or FALSE otherwise
	BOOL GetImage(int& nImage);
	// returns TRUE if successful, or FALSE otherwise
	BOOL SetImage(int nImage);

	// returns TRUE if any child window was associated with the band and 
	// fill hWnd with the handle to it, or FALSE otherwise
	BOOL GetChildWindow(HWND& hWnd);
	// returns TRUE if successful, or FALSE otherwise
	BOOL SetChildWindow(CWnd* pChildWnd);

	// returns TRUE if minimum size of child window was previously set and 
	// fill sizeMin with it, or FALSE otherwise
	BOOL GetChildWindowMinSize(CSize& sizeMin);
	// returns TRUE if successful, or FALSE otherwise
	BOOL SetChildWindowMinSize(UINT cxMin, UINT cyMin);

	// returns TRUE if initial width of child window was previously set and 
	// fill nWidth with it, or FALSE otherwise
	BOOL GetWidth(UINT& nWidth);
	// returns TRUE if successful, or FALSE otherwise
	BOOL SetWidth(UINT nWidth);

	// returns TRUE if any bitmap was previously set and 
	// fill hBitmap with the handle to it, or FALSE otherwise
	BOOL GetBackgroundBitmap(HBITMAP& hBitmap);
	// returns TRUE if successful, or FALSE otherwise
	BOOL SetBackgroundBitmap(HBITMAP hBitmap);

	
	// returns TRUE if any ID was previously set and 
	// fill nID with it, or FALSE otherwise
	BOOL GetID(UINT& nID);
	// returns TRUE if successful, or FALSE otherwise
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//	You have to set non zero, unique wID to every band (demanded by design)
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	BOOL SetID(UINT nID);

	// just to be consistent
	// always return TRUE and fill nMask with current mask
	BOOL GetMask(UINT& nMask);
	// always return TRUE
	BOOL SetMask(UINT nMask);


	/////////////
	// save the state of the band in the registry
	// next are saved if set:
	//		style
	//		min size of child window
	//		initial width
	//		text associated with the band
	//		ID
	//		zero index of image list in COXCoolBarInfo
	//		zero index of band in the parent CoolBar (parameter nIndex)
	/////////////
	// returns TRUE if successful, or FALSE otherwise
	BOOL SaveState(LPCTSTR lpszProfileName, int nIndex);
	// applies previously saved state
	// returns TRUE if successful, or FALSE otherwise
	BOOL LoadState(LPCTSTR lpszProfileName);

// Implementation
public:
	~COXCoolBarBandInfo() 
	{
		// we probaby allocate some memory to save text then free it 
		if(lpText!=NULL)
		{
			free(lpText);
		}
	}
};

//////////////////
// COXCoolBar encapsulates IE 3.0 common coolbar for MFC.
//
class OX_CLASS_DECL COXCoolBar : public CControlBar 
{
protected:
	DECLARE_DYNAMIC(COXCoolBar)

public:
	COXCoolBar();
	virtual ~COXCoolBar();

	///////////
	// styles
	///////////
	// RBS_BANDBORDERS		- control displays narrow lines to separate 
	//						adjacent bands. 
	// RBS_FIXEDORDER		- control always displays bands in the same order. 
	//						You can move bands to different rows, 
	//						but the band order is static. 
	// RBS_TOOLTIPS			- not yet supported. 
	// RBS_VARHEIGHT		- control displays bands at the minimum required height, 
	//						when possible. Without this style, the CoolBar control 
	//						displays all bands at the same height, 
	//						using the height of the tallest visible band 
	//						to determine the height of other bands. 
	///////////
	// starting from version 4.71
	// RBS_AUTOSIZE			- control will automatically change the layout 
	//						of the bands when the size or position of the 
	//						control changes. An RBN_AUTOSIZE notification 
	//						will be sent when this occurs. 
	// RBS_DBLCLKTOGGLE		- band will toggle its maximized or minimized state 
	//						when the user double-clicks on the band. 
	//						Without this style, the maximized or minimized 
	//						state is toggled when the user single-clicks 
	//						on the band. 
	// RBS_REGISTERDROP		- control generates RBN_GETOBJECT notification messages 
	//						when an object is dragged over a band in the control. 
	//						To receive the RBN_GETOBJECT notifications, 
	//						initialize OLE with a call to OleInitialize or 
	//						CoInitialize. 
	// RBS_VERTICALGRIPPER	- the size grip will be displayed vertically 
	//						instead of horizontally in a vertical CoolBar control. 
	//						This style is ignored for CoolBar controls that 
	//						do not have the CCS_VERT style. 
	BOOL Create(CWnd* pParentWnd, DWORD dwStyle,
		DWORD dwAfxBarStyle = CBRS_ALIGN_TOP,
		UINT nID = IDW_COOLBAR);

	// !!!!!!!!!!!!!!!!!!!!!!!!!!
	// When a CoolBar control is destroyed, it destroys any child windows 
	// assigned to the bands within it. To prevent the control from 
	// destroying child windows assigned to its bands, remove the bands 
	// using DeleteBand(UINT nWhich) function, and then reset the parent 
	// to another window with the CWnd::SetParent() function before 
	// destroying the CoolBar control. 
	// !!!!!!!!!!!!!!!!!!!!!!!!!!

	////////////////////////
	// Message wrappers
	////////////////////////
	// Retrieves information about the CoolBar control and the image list it uses 
	// returns TRUE if successful, or FALSE otherwise
	inline BOOL GetBarInfo(COXCoolBarInfo* pI)
	{ 
		ASSERT(::IsWindow(m_hWnd));
		if(!pI->SetMask(RBIM_IMAGELIST))
		{
			return FALSE;
		}
		return (BOOL)SendMessage(RB_GETBARINFO, 0, (LPARAM)pI); 
	}
	// Sets the characteristics of a CoolBar control
	// returns TRUE if successful, or FALSE otherwise
	inline BOOL SetBarInfo(COXCoolBarInfo* pI)
	{ 
		ASSERT(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(RB_SETBARINFO, (WPARAM)0, (LPARAM)pI); 
	}

	// Retrieves information about a specified band in a CoolBar control. 
	// uBand - Zero-based index of the band for which the information 
	//		will be retrieved. 	
	// returns TRUE if successful, or FALSE otherwise
	BOOL GetBandInfo(UINT nBand, COXCoolBarBandInfo* pBI);
	// Sets characteristics of an existing band in a CoolBar control
	// returns TRUE if successful, or FALSE otherwise
	BOOL SetBandInfo(UINT nBand, COXCoolBarBandInfo* pBI);

	// returns TRUE if successful, or FALSE otherwise
	BOOL InsertBand(UINT nWhere, COXCoolBarBandInfo* pBI);
	// Deletes a band from a CoolBar control. 
	// nWhich - Zero-based index of the band to be deleted
	// returns TRUE if successful, or FALSE otherwise
	BOOL DeleteBand(UINT nWhich);

	// Retrieves the count of bands currently in the CoolBar control
	// Returns a UINT value that represents the number of bands 
	// assigned to the control. 	
	inline UINT GetBandCount()
	{ 
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)SendMessage(RB_GETBANDCOUNT); 
	}

	// retrieves the number of rows of bands in a CoolBar control 
	// returns a UINT value that represents the number of band rows in the control
	inline UINT GetRowCount()
	{
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)SendMessage(RB_GETROWCOUNT); 
	}
	// retrieves the height of a specified row in a CoolBar control 
	// returns a UINT value that represents the row height, in pixels
	inline UINT GetRowHeight(UINT nWhich)
	{ 
		ASSERT(::IsWindow(m_hWnd));
		return (UINT)SendMessage(RB_GETROWHEIGHT, (WPARAM)nWhich); 
	}

	// Sets a CoolBar control's parent window. 
	// Returns the handle to the previous parent window, or NULL 
	// if there is no previous parent. 
	// The CoolBar control sends notification messages to the window 
	// you specify with this message. This message does not actually change 
	// the parent of the CoolBar control
	inline HWND SetParent(HWND hwndParent)
	{
		ASSERT(::IsWindow(m_hWnd));
		return (HWND)SendMessage(RB_SETPARENT, (WPARAM)hwndParent); 
	}
	////////////// end wrappers


	
	/////////////////////
	// there are two ways of getting/setting info to a band in CoolBar:
	//		1)	you can instantiate object of COXCoolBarBandInfo class;
	//			if band exists you can fill the object with band's info 
	//				using COXCoolBar::GetBandInfo;
	//			you can use COXCoolBarBandInfo::SetXXX functions to set
	//				band's info
	//			if you create new band use COXCoolBar::InsertBand to insert
	//				the band into CoolBar;
	//			if you change existing band use COXCoolBar::SetBandInfo to 
	//				set the band's info;
	/////////////////////
	//		2)	you can use COXCoolBar::SetBandXXX and COXCoolBar::GetBandXXX
	//			functions to set particular band's property
	/////////////////////
	//
	// as seen from description you use first method when you want to insert
	// new band into CoolBar (the only way to do that) or when you want to set
	// or get many band's properties at one time;
	// the second way is preferable when you want to set/get only a few band's
	// properties at one time
	/////////////////////
	//
	// in all GetBandXXX/SetBandXXX functions you use zero-based index number 
	// of band in CoolBar excluding GetBandNumber function that you use 
	// to define the number of band based on band's ID;
	//
	// as soon as every band in CoolBar has to have non zero, unique ID (by design)
	// you can always get this number;
	//
	// therefore you have to save the IDs you set to bands in some place or
	// if you use child windows within bands (almost always you do) you can 
	// create these windows with the same ID as you set to corresponding band
	// (it is safe because you can associate only one child window with a band)
	// and call GetDlgCtrlID function of child window to get the ID of 
	// corresponding band
	//
	/////////////////////

	/////////////////////////////////////////////////////////////////////
	// all functions return TRUE if successful, or FALSE otherwise;
	// if any GetBandXXX function returns FALSE then 
	// the second parameter is undefined
	/////////////////////////////////////////////////////////////////////
	BOOL GetBandStyle(UINT nBand, UINT& nStyle);
	BOOL SetBandStyle(UINT nBand, UINT nStyle);

	BOOL GetBandColor(UINT nBand, COLORREF& crColor);
	BOOL SetBandColor(UINT nBand, COLORREF crColor);

	BOOL GetBandBkColor(UINT nBand, COLORREF& crColor);
	BOOL SetBandBkColor(UINT nBand, COLORREF crColor);

	BOOL GetBandText(UINT nBand, CString& sText);
	BOOL SetBandText(UINT nBand, LPTSTR sText);

	BOOL GetBandMask(UINT nBand, UINT& nMask);
	// be very careful using this function because 
	// it doesn't change the band's properties explicitly
	// but implicitly when you call other SetBandXXX and GetBandXXX functions
	BOOL SetBandMask(UINT nBand, UINT nMask);

	BOOL GetBandImage(UINT nBand, int& nImage);
	BOOL SetBandImage(UINT nBand, int nImage);

	BOOL GetBandChildWindow(UINT nBand, HWND& hChildWnd);
	BOOL SetBandChildWindow(UINT nBand, CWnd* pChildWnd);

	BOOL GetBandChildWindowMinSize(UINT nBand, CSize& sizeChild);
	BOOL SetBandChildWindowMinSize(UINT nBand, UINT cxMin, UINT cyMin);

	BOOL GetBandWidth(UINT nBand, UINT& nWidth);
	BOOL SetBandWidth(UINT nBand, UINT nWidth);

	BOOL GetBandBackgroundBitmap(UINT nBand, HBITMAP& hBitmap);
	BOOL SetBandBackgroundBitmap(UINT nBand, HBITMAP hBitmap);

	BOOL GetBandID(UINT nBand, UINT& nID);
	BOOL SetBandID(UINT nBand, UINT nID);
	/////////////////////////////////////////////////////////////////////

	// use this function to get band number based on its ID
	BOOL GetBandNumber(UINT nID, UINT& nBand);

	// functions you can use to create bitmaps and image lists from resource file
	// to use them in CoolBar. In this case you shouldn't care about destroying 
	// created GDI object, CoolBar will destroy it automatically
	inline HBITMAP AddBitmap(UINT nBitmapID)
	{
		return AddBitmap(MAKEINTRESOURCE(nBitmapID));
	}
	HBITMAP AddBitmap(LPTSTR lpszResourceName);

	inline HIMAGELIST AddImageList(UINT nImageListID, int cx=16, 
							int cGrow=0, COLORREF crMask=RGB(255,0,255), 
							UINT uType=IMAGE_BITMAP, 
							UINT uFlags=LR_DEFAULTCOLOR)
	{
		return AddImageList(MAKEINTRESOURCE(nImageListID),cx,cGrow,crMask,uType,uFlags);
	}
	HIMAGELIST AddImageList(LPTSTR lpszResourceName, int cx=16, 
							int cGrow=0, COLORREF crMask=RGB(255,0,255), 
							UINT uType=IMAGE_BITMAP, 
							UINT uFlags=LR_DEFAULTCOLOR);


	// save to and load from registry (or *.ini file) state of bands in CoolBar
	// it has mostly to do with the order of bands
	// all other job is done by 
	// COXCoolBarBandInfo::SetState/COXCoolBarBandInfo::LoadState
	BOOL SaveState(LPCTSTR lpszProfileName);
	BOOL LoadState(LPCTSTR lpszProfileName);

protected:
	// map array to keep all created handles to bitmap used by CoolBar
	typedef CMap<LPTSTR, LPTSTR, HBITMAP, HBITMAP> allBitmaps;
	allBitmaps m_allBitmaps;

	// map array to keep all created handles to image lists used by CoolBar
	typedef CMap<LPTSTR, LPTSTR, HANDLE, HANDLE> allImageLists;
	allImageLists m_allImageLists;

	// map array to keep all strings that are set to bands
	typedef CMap<UINT, UINT, CString, CString&> allBandText;
	allBandText m_allBandText;

	// there is no easy way to define which properties of band are set or not set
	// to resolve this problem we decided to keep all and's masks in map array
	typedef CMap<UINT, UINT, UINT, UINT> allBandMask;
	allBandMask m_allBandMask;

	// band's ID has to be non zero and unique, to keep trace of all IDs
	// we use this map array
	typedef CMap<UINT, UINT, UINT, UINT> allBandID;
	allBandID m_allBandID;


	/////////////////////////////////////////////////////////////////
	// all functions return TRUE if successful, or FALSE otherwise;
	// called from SetBandInfo and InsertBand functions
	/////////////////////////////////////////////////////////////////
	// saves new mask in m_allBandMask
	BOOL CheckInBandMask(UINT nBand, UINT nNewMask);
	// saves new text in m_allBandText
	BOOL CheckInBandText(UINT nBand, CString& sText);
	// checks and saves new ID in m_allBandID
	BOOL CheckInBandID(UINT nID, BOOL bCreate=FALSE);
	// if we change the ID then we have to change corresponding
	// entries in m_allBandText and m_allBandMask
	BOOL ChangeID(UINT nOldID, UINT nNewID);

	// find in registry saved info for band with nBandID
	int FindSavedBandInfo(UINT nBandID, LPCTSTR lpszProfileName, UINT nBandCount);

	// new virtual functions you must/can override
	virtual void OnHeightChange(const CRect& rcNew);

	// CControlBar Overrides
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

	// message handlers
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnHeightChange(NMHDR* pNMHDR, LRESULT* pRes);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#endif //_COOLBAR_H

#endif //_MFC_VER>=0x0420


