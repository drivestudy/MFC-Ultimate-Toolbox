// ==========================================================================
//						   Class Specification : 
//							COXIntelliMouse
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

A lot of popular applications such as Internet Explorer or Microsoft Excel 
have support for advanced scrolling functionality. But you probably even 
haven't noticed it because it's available only if you have Microsoft 
IntelliMouse (generally, any three-button mouse which is quite rare case now).
IntelliMouse has special weel turning which you can scroll the contents of
the active window. But if you click on this weel it will act as common mouse 
middle button. The most interesting thing is what happen after you clicked 
on the mouse weel. E.g. Internet Explorer will do the following: if opened
page has vertical and/or horizontal scrollbar(s) then it will display 
non-rectangular splash window in shape of target and if you move mouse it
will scroll the contents of the page in the corresponding direction (if it's 
possible to scroll). Note that simultaneous scrolling in vertical and 
horizontal directions is supported. Moreover, the far you move mouse from 
the point where Intelli Mouse Support was started the faster the contents
will be scrolled (ballistic scrolling). 

We believe that the functionality like that would be extremely useful in
many situations: scrolling of lengthy document or scrolling tree control, 
list control or listbox with big number of items. Generally speacking such
functionality would be useful for any window that has WS_VSCROLL and/or
WS_HSCROLL style set. 

We decided to come up with the class that will provide Intelli Mouse Support
for any window. Although there are different implementation of Intelli 
Mouse Support in different commercial application we decided to stick with 
the implementation found in Internet Explorer and which was described above.

As long as this class has to provide Intelli Mouse Support for any window
we decided to use our COXHookWnd class in order to do that (we use the 
same approach we used in COXItemTip class and most recently in 
COXSoundEffectManager class). In order to display non-rectangular splash
window we use our COXSplashWnd class. 

Also we decided to provide Intelli Mouse Support even though you don't have 
Microsoft IntelliMouse or three-button mouse installed (most people don't).
The idea was to provide the way programmers can set up their own event ID that
will launch Intelli Mouse Support (e.g. WM_LBUTTONDOWN with "Shift" key 
pressed).


We implemented all above described functionality in our COXIntelliMouse class.
COXIntelliMouse is derived from COXHookWnd and uses internally COXSplashWnd
class. Also in order to visually display the direction of the scrolling
when Intelli Mouse Support is on COXIntelliMouse class uses set of cursors
that is defined in "OXIntelliMouse.rc" file.

!!!!!!!!!!!!
Note:	you have to include in "Resource Includes"-"Compile Time Directives" 
		of your application's resource file reference to "OXIntelliMouse.rc"
!!!!!!!!!!!!


Following technique should be applied in order to provide Intelli Mouse 
Support in your application using COXIntelliMouse class:

1)	Instantiate one object of COXIntelliMouse class for every window class
	for which you are going to provide Intelli Mouse Support.


	Example:

	class CMyView : public CView
	{
		. . . . . . . . . . . . . .
	protected:
		// intelli mouse support
		COXIntelliMouse m_IntelliMouse;

		. . . . . . . . . . . . . .
	}

2)	In OnCreate function of such window class call COXIntelliMouse::Attach 
	function in order to register it with COXIntelliMouse object and to 
	set up the start event that will be interpreted as trigger to launch
	Intelli Mouse Support. Here is the description of that function:

	BOOL Attach(CWnd* pWnd, UINT nStartEventID=WM_MBUTTONDOWN, DWORD dwKeyState=0);
	// --- In  :	pWnd			-	pointer to created CListCtrl-derived
	//									control	to be hooked
	//				nStartEventID	-	identificator of the event that will
	//									serve as start point of launching of 
	//									intelli mouse scrolling support
	//				dwKeyState		-	zero or any combination of folowing flags:
	//
	//							KEYPRESSED_CONTROL	-	"Ctrl" key must be pressed
	//													when nStartEventID event is
	//													fired
	//							KEYPRESSED_SHIFT	-	"Shift" key must be pressed
	//													when nStartEventID event is
	//													fired
	//							KEYPRESSED_ALT		-	"Alt" key must be pressed
	//													when nStartEventID event is
	//													fired
	// --- Out : 
	// --- Returns:	TRUE if pWnd was successfully hooked, otherwise FALSE
	// --- Effect : Hooks window

	
	Note, that you can conditionally specify state settings of the "Ctrl",
	"Shift" and "Alt" keys. It was done specifically to simplify the 
	process of setting of start event ID. Although we use WM_MBUTTONDOWN 
	event as default one, it's not an option for those who don't have
	Microsoft IntelliMouse or three-button mouse. From the other hand other
	mouse-related events such as WM_LBUTTONDOWN or WM_RBUTTONDOWN usually 
	already have some functionality associated with them. So we believe it
	quite reasonable to guess that programmers would like to use some of 
	the above listed keys in combination with one of the standard mouse 
	event in order to launch Intelli Mouse Support.


	Example:

	int CMyView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		. . . . . . . . . . . . . .
		// mouse left button down altogether with "Ctrl" key pressed down
		// will launch Intelli Mouse Support
		m_IntelliMouse.Attach(this,WM_LBUTTONDOWN,KEYPRESSED_CONTROL);
		. . . . . . . . . . . . . .
	}


3)	That's it.




Although you need to call only one function in order to provide Intelli 
Mouse Support for any window class we provide also a number of functions
that won't be used that often but still can be very useful if you need to
customize Intelli Mouse Support at full extent.


Next function can be used in order to remove Intelli Mouse Support:

	void Detach();
	// --- Effect : Unhooks attached window if any was hooked



Following functions can be used to specify the appearance of the splash
window and cursors that show the scrolling direction:

	HCURSOR GetScrollCursor(SCROLLDIRECTION scrollDirection) const
	void SetScrollCursor(SCROLLDIRECTION scrollDirection, HCURSOR hCursor) 
	BOOL LoadSplashWndBitmap(LPCTSTR lpszResourceName, 
		COLORREF crBorder = CLR_DEFAULT, LPPOINT pStartPoint = NULL, BYTE nTolerance = 0) 
	BOOL LoadSplashWndBitmap(UINT nIDResource, COLORREF crBorder = CLR_DEFAULT, 
		LPPOINT pStartPoint = NULL, BYTE nTolerance = 0)



And, finally, there are functions that let you to request and change the 
settings that has to do with the way Intelli Mouse Support is launched. 
Below you will find the list of such functions:

	UINT GetStartEventID() const
	UINT GetKeyPressedState() const
	void SetStartEventID(UINT nStartEventID, DWORD dwKeyState=0)



Refer to the description of all above mentioned functions for details.


*/

#ifndef _INTELLIMOUSE_H
#define _INTELLIMOUSE_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#define __AFXTEMPL_H__
#endif

#include "OXMainRes.h"
#include "OXHookWnd.h"
#include "OXSplashWnd.h"



// timer for scrolling settings
//
#define ID_INTELLIMOUSESCROLLTIMER		200
#define ID_INTELLIMOUSESCROLLDELAY		100

#define ID_SCROLLCOEF					25


// scroll directions
//
typedef enum _tagSCROLLDIRECTION 
{
	SCRL_LEFT		=	0x00000001,
	SCRL_TOP		=	0x00000002,
	SCRL_RIGHT		=	0x00000004,
	SCRL_BOTTOM		=	0x00000008,
	SCRL_TOPLEFT	=	SCRL_TOP|SCRL_LEFT,
	SCRL_TOPRIGHT	=	SCRL_TOP|SCRL_RIGHT,
	SCRL_BOTTOMRIGHT=	SCRL_BOTTOM|SCRL_RIGHT,
	SCRL_BOTTOMLEFT	=	SCRL_BOTTOM|SCRL_LEFT,
	SCRL_LEFTRIGHT	=	SCRL_LEFT|SCRL_RIGHT,
	SCRL_TOPBOTTOM	=	SCRL_TOP|SCRL_BOTTOM,
	SCRL_ALL		=	SCRL_LEFT|SCRL_RIGHT|SCRL_TOP|SCRL_BOTTOM,
	SCRL_NONE		=	0x00000000
} SCROLLDIRECTION;

#define ID_ICONCOUNT					11

typedef CMap<SCROLLDIRECTION,SCROLLDIRECTION,CRect,CRect&> CMapScrollRect;
typedef CMap<SCROLLDIRECTION,SCROLLDIRECTION,HANDLE,HANDLE> CMapScrollCursor;


#define KEYPRESSED_CONTROL			0x00000001
#define KEYPRESSED_SHIFT			0x00000002
#define KEYPRESSED_ALT				0x00000004

///////////////////////////////////////////////////////////////////////
//

class OX_CLASS_DECL COXIntelliMouse : public COXHookWnd 
{
	DECLARE_DYNAMIC(COXIntelliMouse);

protected:
	// splash window to be used to display bitmap
	COXSplashWnd m_splashWnd;
	
	// identificator of the event that will	serve as start point of 
	// intelli mouse scrolling support
	UINT m_nStartEventID;

	// state of the "Ctrl", "Shift", "Alt" keys when m_nStartEventID event is fired.
	// Can be zero or any combination of folowing flags:
	//
	//		KEYPRESSED_CONTROL	-	"Ctrl" key must be pressed when nStartEventID 
	//								event is fired
	//		KEYPRESSED_SHIFT	-	"Shift" key must be pressed	when nStartEventID 
	//								event is fired
	//		KEYPRESSED_ALT		-	"Alt" key must be pressed when nStartEventID 
	//								event is fired
	DWORD m_dwKeyState;
	
	// flag is set to TRUE if we currently are providing intelli mouse 
	// support for the hooked window
	BOOL m_bIsHandling;
	// combination of flags that specifies in which direction the contents
	// of the hooked window can be scrolled, can be any combination of:
	// SCRL_LEFT,SCRL_TOP,SCRL_RIGHT,SCRL_BOTTOM,SCRL_NONE
	DWORD m_dwScrollFlag;
	// current scrolling info for horizontal and vertical direction. Used 
	// in combination with m_dwScrollFlag flags
	SCROLLINFO m_scrollHorzInfo;
	SCROLLINFO m_scrollVertInfo;

	// timer ID used to scroll the contents of the hooked window
	UINT_PTR m_nScrollTimerID;

	// current scroll direction
	SCROLLDIRECTION m_scrollDirection;

	// rectangles to test mouse cursor location on to define the scroll 
	// direction
	CMapScrollRect m_arrScrollRect;

	// cursor handles to show all scrolling directions
	CMapScrollCursor m_arrScrollCursor;

	// cx element defines the number of lines to scroll horizontally 
	// (positive number means scrolling right and negative - left),
	// cy element defines the number of lines to scroll vertically 
	// (positive number means scrolling down and negative - up)
	CSize m_szScrollSize;

	// mouse cursor coordinates when intelli mouse support was started
	// in result of firing of m_nStartEventID event
	CPoint m_ptStart;

	// helper window
	CWnd m_helperWnd;
	CWnd* m_pOldForegroundWnd;

protected:
	// The heart of the class. Handle all needed messages. While deriving
	// your own class make sure to call this function. I wouldn't recommend you 
	// to change anything in it
	//
	// --- In  :	msg	-	message ID
	//				wp	-	WPARAM
	//				lp	-	LPARAM
	// --- Out : 
	// --- Returns:	result of message handling. Different for different messages.
	// --- Effect :	Sets the number of color shades used in gradient filling. 
	virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);	

public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXIntelliMouse();

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destroys the object
	virtual ~COXIntelliMouse();


	// --- In  :	pWnd			-	pointer to created window to be hooked
	//				nStartEventID	-	identificator of the event that will
	//									serve as start point of launching of 
	//									intelli mouse scrolling support
	//				dwKeyState		-	zero or any combination of following flags:
	//
	//							KEYPRESSED_CONTROL	-	"Ctrl" key must be pressed
	//													when nStartEventID event is
	//													fired
	//							KEYPRESSED_SHIFT	-	"Shift" key must be pressed
	//													when nStartEventID event is
	//													fired
	//							KEYPRESSED_ALT		-	"Alt" key must be pressed
	//													when nStartEventID event is
	//													fired
	// --- Out : 
	// --- Returns:	TRUE if pWnd was successfully hooked, otherwise FALSE
	// --- Effect : Hooks window
	BOOL Attach(CWnd* pWnd, UINT nStartEventID=WM_MBUTTONDOWN, DWORD dwKeyState=0);

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Unhooks attached window if any was hooked
	void Detach();


	// --- In  :	scrollDirection	-	scroll direction. One of the following
	//									values can be specified
	//
	//									SCRL_LEFT
	//									SCRL_TOP
	//									SCRL_RIGHT
	//									SCRL_BOTTOM
	//									SCRL_TOPLEFT
	//									SCRL_TOPRIGHT
	//									SCRL_BOTTOMRIGHT
	//									SCRL_BOTTOMLEFT
	//									SCRL_LEFTRIGHT
	//									SCRL_TOPBOTTOM
	//									SCRL_ALL
	//
	// --- Out : 
	// --- Returns:	handle to the cursor that is displayed while scrolling
	//				in specified direction if succeeds, or NULL otherwise
	// --- Effect :
	inline HCURSOR GetScrollCursor(SCROLLDIRECTION scrollDirection) const
	{
		HANDLE hCursor=NULL;
		m_arrScrollCursor.Lookup(scrollDirection,hCursor);
		return (HCURSOR)hCursor;
	}
	
	// --- In  :	scrollDirection	-	scroll direction. Refer to description 
	//									of GetScrollCursor function for 
	//									possible values
	//				hCursor			-	valid handle to the cursor that will be 
	//									displayed while scrolling in specified 
	//									direction if succeeds
	// --- Out : 
	// --- Returns:	
	// --- Effect :	Sets cursor to be displayed while scrolling in specified 
	//				direction
	inline void SetScrollCursor(SCROLLDIRECTION scrollDirection, HCURSOR hCursor) 
	{
		ASSERT(hCursor!=NULL);
		m_arrScrollCursor.SetAt(scrollDirection,hCursor);
	}
	

	// --- In     : lpszResourceName
	//				nIDResource			-	the bitmap resource
	//				crBorder			-	indicates which color should be 
	//										used as border color. Every outer 
	//										pixel in this color will be made 
	//										invisible. This will create the 
	//										necessary shape of the splash 
	//										window. You can either specify the 
	//										color or use one of the two special 
	//										values:
	//	
	//										CLR_NONE:	no border color needs 
	//													to be used.	The bitmap 
	//													will just be shown as a 
	//													rectangle bitmap.
	//										CLR_DEFAULT:the color of the pixel 
	//													specified by pStartPoint
	//													should be used.
	//
	//				pStartPoint			-	indicates which pixel within the 
	//										bitmap should be used to start 
	//										looking for the actual border. When 
	//										no starting point is specified, 
	//										(0,0) is used by default.
	//				nTolerance			-	indicates how much the color has to 
	//										be different from crBorder before 
	//										it is actaully accepted	as a 
	//										different color. A tolerance of 0 
	//										indicates that all colors except an 
	//										exact match with crBorder are 
	//										treated as different.
	// --- Out    : 
	// --- Returns: TRUE if successful; FALSE otherwise.
	// --- Effect : Loads the bitmap from resource, and specify parameters for
	//				 determining the shape (visible-invisible boundary).
	inline BOOL LoadSplashWndBitmap(LPCTSTR lpszResourceName, 
		COLORREF crBorder=CLR_DEFAULT, LPPOINT pStartPoint=NULL, BYTE nTolerance=0) 
	{
		return m_splashWnd.LoadBitmap(lpszResourceName,crBorder,
			pStartPoint,nTolerance);
	}
	inline BOOL LoadSplashWndBitmap(UINT nIDResource, COLORREF crBorder=CLR_DEFAULT, 
		LPPOINT pStartPoint=NULL, BYTE nTolerance=0)
	{
		return m_splashWnd.LoadBitmap(nIDResource,crBorder,
			pStartPoint,nTolerance);
	}


	// --- In  :	
	// --- Out : 
	// --- Returns:	identificator of the event that will serve as start point 
	//				of launching of intelli mouse scrolling support
	// --- Effect : Retrieves the start intelli mouse scrolling support event ID
	inline UINT GetStartEventID() const { return m_nStartEventID; }

	// --- In  :	
	// --- Out : 
	// --- Returns:	state of the "Ctrl", "Shift", "Alt" keys when m_nStartEventID 
	//				event is fired. For list of all state flags refer to 
	//				description of Attach function
	// --- Effect : 
	inline UINT GetKeyPressedState() const { return m_dwKeyState; }
	
	// --- In  :	nStartEventID	-	identificator of the event that will
	//									serve as start point of launching of 
	//									intelli mouse scrolling support
	//				dwKeyState		-	state of the "Ctrl", "Shift", "Alt" keys 
	//									when m_nStartEventID event is fired. For 
	//									list of all state flags refer to 
	//									description of Attach function
	// --- Out : 
	// --- Returns:	
	// --- Effect : Sets up event ID to start intelli mouse scrolling support
	inline void SetStartEventID(UINT nStartEventID, DWORD dwKeyState=0)
	{
		m_nStartEventID=nStartEventID;
		m_dwKeyState=dwKeyState;
	}

protected:
	void Reset();
	BOOL LaunchIntelliMouseSupport();
	void CancelIntelliMouseSupport();
	virtual BOOL GetHookedScrollInfo();
	void CalculateScrollRects(const CSize& szSplashWindowBitmap, 
		const CPoint& ptStart, SCROLLDIRECTION scrollDirection);
	void ShowScrollCursor(SCROLLDIRECTION scrollDirection) const;
	SCROLLDIRECTION FindScrollDirectionFromPoint(const CPoint& ptTest) const;
};


//////////////////////////////////////////////////////////////////////


class OX_CLASS_DECL COXIntelliMouseOrganizer
{
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXIntelliMouseOrganizer();

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destroys the object
	virtual ~COXIntelliMouseOrganizer();


	// --- In  :	dwThreadID		-	thread ID which all windows will be attached
	//									to intelli mouse objects. By default we
	//									use the current process thread
	//				nStartEventID	-	identificator of the event that will
	//									serve as start point of launching of 
	//									intelli mouse scrolling support
	//				dwKeyState		-	zero or any combination of folowing flags:
	//
	//							KEYPRESSED_CONTROL	-	"Ctrl" key must be pressed
	//													when nStartEventID event is
	//													fired
	//							KEYPRESSED_SHIFT	-	"Shift" key must be pressed
	//													when nStartEventID event is
	//													fired
	//							KEYPRESSED_ALT		-	"Alt" key must be pressed
	//													when nStartEventID event is
	//													fired
	// --- Out : 
	// --- Returns:	TRUE if all windows created by the specified thread were 
	//				successfully hooked, otherwise FALSE
	// --- Effect : Creates COXIntelliMouse objects for all windows created 
	//				by the specified thread. The windows 
	//				that will be created by the thread in the future will be 
	//				attached as soon as they will be created. Whenever a window
	//				is attached it uses the intelli mouse settings assigned to 
	//				already attached windows	
	BOOL AttachAllInThread(DWORD dwThreadID=::GetCurrentThreadId(),
		UINT nStartEventID=WM_MBUTTONDOWN, DWORD dwKeyState=0);


	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if all windows were successfully unhooked, 
	//				otherwise FALSE
	// --- Effect : Detaches all previously attached windows that has been 
	//				created by the thread specified in AttachAllInThread()
	//				function that was called before
	BOOL DetachAllInThread();


	// --- In  :	pWnd			-	pointer to created window to be hooked
	//				nStartEventID	-	identificator of the event that will
	//									serve as start point of launching of 
	//									intelli mouse scrolling support
	//				dwKeyState		-	zero or any combination of folowing flags:
	//
	//							KEYPRESSED_CONTROL	-	"Ctrl" key must be pressed
	//													when nStartEventID event is
	//													fired
	//							KEYPRESSED_SHIFT	-	"Shift" key must be pressed
	//													when nStartEventID event is
	//													fired
	//							KEYPRESSED_ALT		-	"Alt" key must be pressed
	//													when nStartEventID event is
	//													fired
	// --- Out : 
	// --- Returns:	pointer to created COXIntelliMouse object or 
	//				NULL if function failed
	// --- Effect : Hooks window
	COXIntelliMouse* Attach(CWnd* pWnd, UINT nStartEventID=WM_MBUTTONDOWN,
		DWORD dwKeyState=0);

	// --- In  :	pWnd	-	pointer to attached window or NULL if you 
	//							want to detach all attached windows
	// --- Out : 
	// --- Returns:	TRUE if operation finished successfully, or FALSE otherwise
	// --- Effect : Unhooks attached window if any was hooked
	BOOL Detach(CWnd* pWnd=NULL);


	// --- In  :	pWnd	-	pointer to the attached window
	// --- Out : 
	// --- Returns:	Pointer to COXIntelliMouse object that corresponds 
	//				to the specified attached window if succeed or NULL
	//				otherwise
	// --- Effect : Retrieves the pointer to COXIntelliMouse object that
	//				corresponds to the specified attached window.
	COXIntelliMouse* GetIntelliMouseObject(const CWnd* pWnd) const;


	// --- In  :	pWnd	-	pointer to the window
	// --- Out : 
	// --- Returns:	TRUE if specified window was hooked before using 
	//				Attach function or FALSE otherwise
	// --- Effect : Retrieves the flag that specifies whether the given 
	//				window was attached before or not.
	BOOL IsAttached(const CWnd* pWnd) const;


	BOOL IsRejected(HWND hWnd) const;


	// --- In  :	pWnd	-	pointer to attached window
	// --- Out : 
	// --- Returns:	identificator of the event that will serve as start point 
	//				of launching of intelli mouse scrolling support
	// --- Effect : Retrieves the start intelli mouse scrolling support event ID
	UINT GetStartEventID(CWnd* pWnd) const;

	// --- In  :	pWnd	-	pointer to attached window
	// --- Out : 
	// --- Returns:	state of the "Ctrl", "Shift", "Alt" keys when m_nStartEventID 
	//				event is fired. For list of all state flags refer to 
	//				description of Attach function
	// --- Effect : 
	UINT GetKeyPressedState(CWnd* pWnd) const;
	
	// --- In  :	pWnd			-	pointer to attached window or NULL if you 
	//									want to set the specified start event to 
	//									all attached windows
	//				nStartEventID	-	identificator of the event that will
	//									serve as start point of launching of 
	//									intelli mouse scrolling support
	//				dwKeyState		-	state of the "Ctrl", "Shift", "Alt" keys 
	//									when m_nStartEventID event is fired. For 
	//									list of all state flags refer to 
	//									description of Attach function
	// --- Out : 
	// --- Returns:	TRUE if start event was successfully set
	// --- Effect : Sets up event ID to start intelli mouse scrolling support
	BOOL SetStartEventID(CWnd* pWnd, UINT nStartEventID, DWORD dwKeyState=0);


	inline BOOL IsAttachedAllInThread() const { return (m_dwThreadID!=NULL); }
	inline DWORD GetAttachedThread() const { return m_dwThreadID; }
	inline HHOOK GetSavedGetMessageHookProc() const { 
		return m_pfnOldGetMessageHookProc; 
	}
	static HHOOK GetOriginalGetMessageHookProc() {
		return COXIntelliMouseOrganizer::m_pfnOriginalGetMessageHookProc; 
	}
	inline HHOOK GetSavedCBTHookProc() const { return m_pfnOldCBTHookProc; }
	static HHOOK GetOriginalCBTHookProc() {
		return COXIntelliMouseOrganizer::m_pfnOriginalCBTHookProc; 
	}

	inline COXIntelliMouse* GetFirstIntelliMouseObject(POSITION& pos) const 
	{
		pos=m_arrAttachedWnd.GetStartPosition();
		HWND hWnd=NULL;
		COXIntelliMouse* pIntelliMouseObject=NULL;
		while(pos!=NULL)
		{
			m_arrAttachedWnd.GetNextAssoc(pos,hWnd,pIntelliMouseObject);
			if(::IsWindow(hWnd))
			{
				ASSERT(pIntelliMouseObject!=NULL);
				break;
			}
			else
			{
				pIntelliMouseObject=NULL;
			}
		}
		return pIntelliMouseObject;
	}
	
	inline COXIntelliMouse* GetNextIntelliMouseObject(POSITION& pos) const 
	{
		if(pos==NULL)
			return NULL;
		HWND hWnd=NULL;
		COXIntelliMouse* pIntelliMouseObject=NULL;
		while(pos!=NULL)
		{
			m_arrAttachedWnd.GetNextAssoc(pos,hWnd,pIntelliMouseObject);
			if(::IsWindow(hWnd))
			{
				ASSERT(pIntelliMouseObject!=NULL);
				break;
			}
			else
			{
				pIntelliMouseObject=NULL;
			}
		}
		return pIntelliMouseObject;
	}

	void AttachAllWindows(HWND hWndStartFrom);


public:
	// map of all organizers that has been created to handle intelli mouse
	// support for all windows created by particular thread
	static CMap<DWORD,DWORD,COXIntelliMouseOrganizer*,
		COXIntelliMouseOrganizer*> m_arrThreadOrganizers;

	// original identificator of the event that will serve as start point of 
	// intelli mouse scrolling support for the windows attached through
	// AttachAllInThread() function call
	UINT m_nThreadOrigStartEventID;

	// original state of the "Ctrl", "Shift", "Alt" keys when m_nStartEventID 
	// event is fired for the windows attached through AttachAllInThread() 
	// function call
	DWORD m_dwThreadOrigKeyState;


protected:
	// map of all attached windows and associated COXIntelliMouse objects
	CMap<HWND,HWND,COXIntelliMouse*,COXIntelliMouse*> m_arrAttachedWnd;
	// array of all used COXIntelliMouse objects
	CArray<COXIntelliMouse*,COXIntelliMouse*> m_arrUsedObjects;

	// map of windows that wasn't qualified to be substituted with cool controls
	CMap<HWND,HWND,DWORD,DWORD> m_mapRejectedControls;

	// ID of the attached thread 
	DWORD m_dwThreadID;
	// handle of the old CBT hook procedure
	HHOOK m_pfnOldCBTHookProc;
	// handle of the original CBT hook procedure
	static HHOOK m_pfnOriginalCBTHookProc;

	// handle of the old GetMessage hook procedure
	HHOOK m_pfnOldGetMessageHookProc;
	// handle of the original GetMessage hook procedure
	static HHOOK m_pfnOriginalGetMessageHookProc;


	// hook procedure for CBT
	static LRESULT CALLBACK IntelliMouseCBTHookProc(int nCode, 
		WPARAM wParam, LPARAM lParam);

	// hook procedure for CBT
	static LRESULT CALLBACK IntelliMouseGetMessageHookProc(int nCode, 
		WPARAM wParam, LPARAM lParam);

	// callback for enumerating topmost windows
	static BOOL CALLBACK EnumThreadWindows(HWND hWnd, LPARAM lParam);

	virtual BOOL IsQualified(HWND hWnd, LPCTSTR lpszWndClass) 
	{ 
		UNREFERENCED_PARAMETER(hWnd);
		UNREFERENCED_PARAMETER(lpszWndClass);
		return TRUE; 
	}
};



#endif // _INTELLIMOUSE_H

