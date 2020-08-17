// ==========================================================================
// 							Class Specification : COXScrollWnd
// ==========================================================================

// Header file : OXScrollWnd.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////
//
/*
	DESCRIPTION

The COXScrollWnd is a virtual base class for simple scrolling and scaling 
functionality. COXScrollWnd was created with COXZoomView as a template. 
It is almost 100% plug-in compatible with it, but of course the difference is that 
COXScrollWnd can be used anywhere and should be considered as a base class for 
controls that have to support scrolling functionality internally while COXZoomView
is only useful in Document/View structure. But if you used COXZoomView before it would
be easier to use the COXScrollWnd class as long as its function's set is almost the 
same as the COXZoomView's one. Though COXScrollWnd has new extended functionality that 
is described below.

COXScrollWnd allows you to scroll the contents and provides functionality of scaling 
its contents from 10% to 1000%, completely transparent for your derived class. 


Here are the main features of the COXScrollWnd class:

	Supports MM_ANISOTROPIC mapping mode.
	Scroll sizes can be defined in percent of the client area
	ScrollToPosition() allows the anchor to be the upper left corner, or 
		additionally the lower left corner or the center of the window.
	Accepts a CRect for the contents size, so the origin can be located in the 
		lower left corner or the center or any arbitrary point of the control. 
		You don't need to handle negative y-coords
	Zoom level can vary between 10% and 1000%
	When zooming, either keeps the upper left window corner, the lower left window 
		corner or keeps the center.
	Allows zooming up to a rectangle(in client coords).
	After zooming, the rectangle can be justified to either the upper left or 
		lower left corner or to the window center.
	Allows zooming to the window size (so the scrollbars just disappear).
	Output can be aligned vertically and horizontally.
	Supports smooth scrolling which means that the process of scrolling of the 
		contents from one position to another is divided in a number of smaller 
		scrolling processes 
	Supports 'Always fit to window' option that if set will scale the contents
		of the control to its size whenever it is resized
	Context menu can be displayed that allows a user easily set most used 
		zoom levels
	Track zooming option is supported which means that when a user left clicks and 
		drag the mouse cursor the tracker will be displayed so a user can choose 
		the rectangle to which the contents must be zoomed
	Scrolling using mouse wheel is supported
	Optional support for ruler bars (using COXRulerBarOrganizer class)




	Usage

To take advantage of automatic scrolling and zooming, derive your class from 
COXScrollWnd instead of from CWnd. When the control is created and whenever the 
size of the contents is changed, you need to specify the scroll sizes using

	void SetScrollSizes();
or	void SetDeviceScrollSizesRelative();

functions. 

Also you might want to specify the scrolling and content alignments using:

	void SetContentsAlign();
	void SetZoomAlign();


In the case the contents size is smaller then the client area of the control window
you have to fill the area that is not covered by the contents. In order to do that 
you might handle WM_ERASEBKND message. From the message handler you should call:

	virtual void FillOutsideRect() const;

function that will draw the background in most effective way (without flickering).


At the initialization you might want to specify the COXScrollWnd environment 
settings.

Smooth scrolling settings can be set using:

	void SetSmoothScrolling();
	void SetSmoothEnvironment();

functions.

Ruler bar settings, if supported, can be set using:

	BOOL AttachRuler();
	void DetachRuler();
	COXRulerOrganizer* GetRulerOrganizer();
 	void SetShowVertRulerBar(BOOL bShowVertRulerBar);
	BOOL GetShowVertRulerBar() const;
	void SetShowHorzRulerBar(BOOL bShowHorzRulerBar);
	BOOL GetShowHorzRulerBar() const;

Note that you can retrieve a pointer to the internal ruler bar control calling
GetRulerOrganizer(). Using this pointer you can further customize the appearance 
of the ruler bar.

Support for ruler bar is optional for the COXScrollWnd class. In order to do it
you have to add the following define into your "stdafx.h" file or project 
preprocessor definitions list:

	#define OXSCRLWND_USE_RULER


Availability of the context menu which allows a user to change zooming and ruler bar
settings can be set using:

	void SetDisplayContextMenu();
	BOOL GetDisplayContextMenu() const;


And track zooming functionality (if a user left clicks and drags the mouse cursor 
the tracker will be displayed so a user can choose the rectangle to which the 
contents must be zoomed)

	void SetUseTrackZoom(BOOL bUseTrackZoom);
	BOOL GetUseTrackZoom() const;



Zoom level can be set/retrieved using the following pair of functions:

	int GetZoomLevel() const;
	int SetZoomLevel();

The contents of the control can be automatically zoomed to the size of the 
client area of the control's window if you call:

	void ZoomToWindow();

and, generally, the contents can be zoomed to any rectangle (in client coordinates) 
using:

	void ZoomToRectangle();

Also you can set the flag that specifies that the control contents should be always
zoomed to fit the size of the control window.

	void SetAlwaysFitWindow(BOOL bAlwaysFitWindow);

In this case whenever the size of control window is changed the zoom level will 
be adjusted so the content will fit the control's client area.

On the other hand the size of the control window can be changed in order to fit the 
current size of the contents taking into account zooming level. To do that call:

	BOOL SizeToContent();


Refer to the class reference for full list of all public functions.

  
As we mentioned before this class is the base one for controls that have to support 
scrolling so we suggest that you should refer to COXImageViewer class (which is 
included in the Ultimate Toolbox library) as an example that demonstrates the 
way you might create your own derivation. 
Also you might want to take look at the sample that demonstrates COXImageViewer class.
it is called ImageViewer and can be found in the .\Samples\graphics\ImageViewer
subdirectory of your Ultimate Toolbox directory.



Dependency:
	#include "OXScrollWnd.h"


Source code files:
	"OXScrollWnd.cpp"
#ifdef OXSCRLWND_USE_RULER
	"OXRulerOrganizer.cpp"	-	COXRulerOrganizer implementation
	"OXHookWnd.cpp"			-	implementation of the COXHookWnd class 
								from which the COXRulerOrganizer class is derived
#endif
  


*/


#if !defined(_OXSCROLLWND_H__)
#define _OXSCROLLWND_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"


#ifdef OXSCRLWND_USE_RULER
#include "OXRulerOrganizer.h"
#endif	//	OXSCRLWND_USE_RULER


// IDs of popup menu items and corresponding menu strings
//
//
//////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// COXScrollWnd window

class OX_CLASS_DECL COXScrollWnd : public CWnd
{
	DECLARE_DYNCREATE(COXScrollWnd)

// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Contructs the object
	COXScrollWnd();

// Attributes
public:
	// enumerator for possible scrolling alignments
    enum ZoomAlignment	{	ZA_DEFAULT, ZA_TOPLEFT, ZA_BOTTOMLEFT, ZA_CENTER	};

	// enumerator for possible contents position alignments
    enum ContentsAlignment	{	CA_TOPLEFT, CA_TOPCENTER, CA_TOPRIGHT,
								CA_CENTERLEFT, CA_CENTER, CA_CENTERRIGHT,
								CA_BOTTOMLEFT, CA_BOTTOMCENTER, CA_BOTTOMRIGHT	};

	// enumerator that defines minimum and maximum percentage to which the contents
	// might be scaled. The contents can be shrinked to 10% of its original size
	// or enlarged to up to 1000% of its original size
    enum { MINZOOM = 10, MAXZOOM = 1000 };

protected:
	// current DC map mode
	int	m_nMapMode;
	
	// total size of the contents in logical coordinates
	CSize m_totalLog;
	// rectangle for the contents in logical coordinates
	CRect m_rectLog;
	// total size of the contents in device coordinates
	CSize m_totalDev;
	// size of one page in device coordinates
	CSize m_pageDev;
	// size of one line in device coordinates
	CSize m_lineDev;
	// total size of the contents in pixels
	CSize m_sizeDev;

	// relative size of the page (relative to the size of the contents in percents)
	int	m_nPagePercent;
	// relative size of the line (relative to the size of the page in percents)
	int	m_nLinePercent;
	// if TRUE then scrolling settings are calculated based on the relative sizes
	// of the page and the line
	BOOL m_bUseRelativeScrollSizes;

	// current zooming level
	int	m_nZoomLevel;

	// current zooming alignment
	ZoomAlignment m_align;
	// current contents alignment
	ContentsAlignment m_alignContents;

	// flag that specifies if smooth scrolling is supported
	BOOL m_bSmoothScrolling;
	// smooth scrolling properties
	int m_nLineSlices;
	int m_nPageSlices;
	DWORD m_dwWaitingTime;
	BOOL m_bIsInSmoothScrolling;
	/////////////////////////////////

	// if TRUE then the contents will be scalled to fit the control window
	// anytime its size being changed
	BOOL m_bAlwaysFitWindow;

	// if TRUE then on right click context menu with zooming options will be displayed
	BOOL m_bDisplayContextMenu;

	// if TRUE then on left click track rectangle will be displayed that 
	// might be used in order to specify rectangle to which contents must be scaled
	BOOL m_bUseTrackZoom;

	// internal flag that is signaled if resizing message was sent from 
	// UpdateBars() routine. Doesn't allow recursive calls.
	BOOL m_bInsideUpdate;


#ifdef OXSCRLWND_USE_RULER
	// ruller object
	COXRulerOrganizer m_ruler;
#endif	//	OXSCRLWND_USE_RULER


// Operations
public:


	// --- In:		
	// --- Out: 
	// --- Returns:	origin point starting from which the contents of the window
	//				be drawn
	// --- Effect:	calculates the origin point starting from which the contents 
	//				of the window be drawn
	CPoint GetOrigin(CDC* pDC=NULL) const;


	// --- In:		pDC		-	device context in which the filling is to be done
	//				pBrush	-	brush with which the area is to be filled
	// --- Out:
	// --- Returns:
	// --- Effect:	fills the area of the view that appears outside of the scrolling
	//				area. Use FillOutsideRect in your COXScrollWnd derived class'
	//				OnEraseBkgnd handler function to prevent excessive background 
	//				repainting.
	virtual void FillOutsideRect(CDC* pDC, CBrush* pBrush) const;


	// --- In:
	// --- Out:
	// --- Returns:	The horizontal and vertical positions (in device units) of the
	//				scroll boxes as a CPoint object
	// --- Effect:	Call GetDeviceScrollPosition when you need to retrieve the current 
	//				horizontal and vertical positions of the scroll boxes in the 
	//				scroll bars. This coordinate pair corresponds to the location 
	//				in the contents to which the upper-left corner of the control's 
	//				window has been scrolled. This is useful for offsetting 
	//				mouse-device positions to scroll-control device positions.
	CPoint GetDeviceScrollPosition() const;


	// --- In:
	// --- Out:		nMapMode	-	the current mapping mode for this control. 
	//								For a list of possible values, see SetScrollSizes().
	//				sizeTotal	-	the current total size of the contents in device 
	//								units.
	//			    sizePage	-	the current horizontal and vertical amounts to 
	//								scroll in each direction in response to a mouse 
	//								click in a scroll-bar shaft.
	//			    sizeLine	-	the current horizontal and vertical amounts to 
	//								scroll in each direction in response to a mouse 
	//								click in a scroll arrow.
	// --- Returns:
	// --- Effect:	Gets the current mapping mode, the total size, and the line and 
	//				page sizes of the contents. Sizes are in device units. 
	//				This member function is rarely called.
	void GetDeviceScrollSizes(int& nMapMode, SIZE& sizeTotal, 
		SIZE& sizePage, SIZE& sizeLine) const;


	// --- In:		Align		-	the zooming alignment 
	// --- Out:
	// --- Returns:	The horizontal and vertical positions (in logical units) of the
	//				scroll boxes as a CPoint object.
	// --- Effect:	Call GetScrollPosition when you need the current horizontal and 
	//				vertical positions of the scroll boxes in the scroll bars. 
	//				This coordinate pair corresponds to the location in the contents 
	//				to which the upper-left corner of the control's window 
	//				has been scrolled.
	CPoint GetScrollPosition(ZoomAlignment Align=ZA_DEFAULT) const; 
		
	// --- In:		pt		-	the point to scroll to, in logical units; 
	//							the cx member must be a positive value 
	//							(greater than or equal to 0, up to the total size
	//							of the contents); the same is true for the 
	//							cy member when the mapping mode is MM_TEXT;
	//							the cy member is negative in mapping modes other 
	//							than MM_TEXT.
	//				align	-	the new zooming alignment for the control. 
	//							Might be one of the following:
	//								ZA_TOPLEFT
	//								ZA_BOTTOMLEFT
	//								ZA_CENTER.
	// --- Out:
	// --- Returns:
	// --- Effect:	Scrolls the contents to the specified POINT position. 
	//				If you specify align as ZA_DEFAULT it uses the value set up with
	//				SetZoomAlignment() function.
	void ScrollToPosition(POINT pt, ZoomAlignment Align=ZA_DEFAULT);    


	// --- In :
	// --- Out :
	// --- Returns: the contents size in logical units
	// --- Effect :	retrieves the contents size in logical units as CSize object
	inline CSize GetTotalSize() const { return m_totalLog; }


	// --- In:
	// --- Out:
	// --- Returns:	the state of the zoom alignment
	// --- Effect:	retrieves the state of the zoom alignment
    inline ZoomAlignment GetZoomAlign() const { return m_align; }

	// --- In :		align	-	the state of the zoom alignment. You can specify
	//							one of the following enumerated values: 
	//								ZA_TOPLEFT
	//								ZA_BOTTOMLEFT
	//								ZA_CENTER.
	// --- Out :
	// --- Returns:	
	// --- Effect : This sets the zooming alignment for the control.
	void SetZoomAlign(ZoomAlignment align);


	// --- In:
	// --- Out:
	// --- Returns:	the state of the contents alignment
	// --- Effect:	retrieves the state of the contents alignment. Refer to 
	//				SetContentsAlign() for possible values
    inline ContentsAlignment GetContentsAlign() const { return m_alignContents; }

	// --- In :		align	-	the state of the contents alignment
	// --- Out :
	// --- Returns:	
	// --- Effect : This sets the contents alignment for the control.You can specify
	//				one of the following enumerated values: 
	//					CA_TOPLEFT
	//					CA_TOPCENTER
	//					CA_TOPRIGHT
	//					CA_CENTERLEFT
	//					CA_CENTER
	//					CA_CENTERRIGHT
	//					CA_BOTTOMLEFT
	//					CA_BOTTOMCENTER
	//					CA_BOTTOMRIGHT
	void SetContentsAlign(ContentsAlignment align);

	
	// --- In :
	// --- Out :
	// --- Returns:	the current zoom level in percent relatively to the original 
	//				contents size.
	// --- Effect :
	inline int GetZoomLevel() const { return m_nZoomLevel; }

	// --- In :		nNewLevel		-	the new zoom level in percent relatively 
	//									to the original size.
	//				Align			-	the new zooming alignment, might be any of:
	//										ZA_TOPLEFT
	//										ZA_BOTTOMLEFT
	//										ZA_CENTER
	// --- Out :
	// --- Returns:	the previous zoom level
	// --- Effect : Sets the new zoom level and updates the window.
	int SetZoomLevel(int nNewLevel, ZoomAlignment Align=ZA_DEFAULT);


	// --- In  :	nMapMode	-	the mapping mode to set for the control
	//				sizeTotal	-	the total size of the contents. 
	//								The cx member contains the horizontal extent. 
	//								The cy member contains the vertical	extent. 
	//								Sizes are in logical units. Both cx and cy 
	//								must be	greater than or equal to 0.
	//				sizePage	-	the horizontal and vertical amounts to scroll 
	//								in each	direction in response to a mouse click 
	//								in a scroll-bar shaft.
	//				sizeLine	-	the horizontal and vertical amounts to scroll 
	//								in each	direction in response to a mouse click 
	//								in a scroll arrow.
	// --- Out :
	// --- Returns:
	// --- Effect : Use this to set TEXT, ENGLISH or METRIC mapping modes. You have
	//				to specify the mode and the size of the contents. If you are not
	//				using MM_TEXT make the rectangles top member bigger than the bottom
	//				member, because these modes assume that the y coords are getting
	//				bigger downside up. If you make top 0 and bottom negative you'll get
	//				normal COXScrollWnd behaviour, having y always negative or 0. 
	//				If you make bottom 0 and top positive, you y coords are always 
	//				positive.
	void SetScrollSizes(int nMapMode, SIZE sizeTotal,
		const SIZE& sizePage = CSize(0,0), const SIZE& sizeLine = CSize(0,0));


	// --- In  :	nMapMode		-	the mapping mode to set for the control
	//				rectContents	-	the total size of the contents.
	//				nPagePercent	-	the amount to scroll in response to a 
	//									mouse click in a scroll-bar shaft relative 
	//									to the size of the contents
	//				nLinePercent	-	the amount to scroll in response to a 
	//									mouse click in a scroll arrow relative
	//									to the size of the page
	// --- Out :
	// --- Returns:
	// --- Effect : Use this to set TEXT, ENGLISH or METRIC mapping modes. You have
	//				to specify the mode and the size of the contents. If you are not
	//				using MM_TEXT make the rectangles top member bigger than the bottom
	//				member, because these modes assume that the y coords are getting
	//				bigger downside up. If you make top 0 and bottom negative you'll get
	//				normal CScrollView behaviour, having y always negative ore 0. If you
	//				make bottom 0 and top positive, you y coords are always positive.
	void SetDeviceScrollSizesRelative(int nMapMode, const CRect& rectContents, 
		int nPagePercent = 25, int nLinePercent = 5 );


	// --- In  :	sizeDevice		-	contents size in pixels
	//				rectContents	-	the total size of the contents.
	//				nPagePercent	-	the amount to scroll in response to a 
	//									mouse click in a scroll-bar shaft relative 
	//									to the size of the contents
	//				nLinePercent	-	the amount to scroll in response to a 
	//									mouse click in a scroll arrow relative
	//									to the size of the page
	// --- Out :
	// --- Returns:
	// --- Effect : Use this for MM_ANISOTROPIC mapping mode. You specify the 
	//				contents rectangle as usual and the corresponding size of the 
	//				contents in pixels. cx and cy of the size have to be positive, 
	//				COXScrollWnd will invert cy if necessary.
	void SetDeviceScrollSizesRelative(SIZE sizeDevice, const CRect& rectContents, 
		int nPagePercent=25, int nLinePercent=5);


	// --- In  :	nMapMode		-	the mapping mode to set for the control
	//				sizeTotal		-	The total size of the contents. The cx member 
	//									contains the horizontal extent. The cy member 
	//									contains the vertical extent. Sizes are in 
	//									logical units. Both cx and cy must be 
	//									greater than or equal to 0.
	//				nPagePercent	-	the amount to scroll in response to a 
	//									mouse click in a scroll-bar shaft relative 
	//									to the size of the contents
	//				nLinePercent	-	the amount to scroll in response to a 
	//									mouse click in a scroll arrow relative
	//									to the size of the page
	// --- Out :
	// --- Returns:
	// --- Effect : this is the most compatible version to CScrollView. This call will
	//				synthesize a rect based on the passed SIZE and the mapping mode.
	void SetDeviceScrollSizesRelative(int nMapMode, SIZE sizeTotal,	
		int nPagePercent, int nLinePercent);


	// --- In:
	// --- Out:
	// --- Returns:
	// --- Effect:	scale the contents to fit the control's window and updates 
	//				the zoom level accordingly.
	void ZoomToWindow();

	// --- In:		rectZoom	-	the rectangle that scaled contents must take
	//				Align		-	the new zooming alignment, might be any of:
	//									ZA_TOPLEFT
	//									ZA_BOTTOMLEFT
	//									ZA_CENTER
	// --- Out:
	// --- Returns:
	// --- Effect:	scales the contents, so it fits the passed rectangle 
	//				as much as possible, at least in one dimension.
	void ZoomToRectangle(CRect rectZoom, ZoomAlignment Align=ZA_DEFAULT);


	// --- In:
	// --- Out:
	// --- Returns: TRUE if control was successfully resized
	// --- Effect:	resize the control window to match the size of the 
	//				scaled contents using current zoom level
	BOOL SizeToContent();


	// --- In:		bSmoothScrolling	-	if TRUE then smooth scrolling will be
	//										supported. By default this flag is set 
	//										to FALSE
	// --- Out:
	// --- Returns:
	// --- Effect:	sets/removes the flag that specifies whether smooth scrolling is 
	//				supported or not. Smooth scrolling means that the process of 
	//				scrolling of the contents from one position to another is divided
	//				in a number of smaller scrolling processes. Please note that
	//				this mode will slow down the scrolling significantly which might
	//				be intolerable for slow computers or controls with large contents
	inline void SetSmoothScrolling(BOOL bSmoothScrolling=TRUE) { 
		m_bSmoothScrolling=bSmoothScrolling; 
	}

	// --- In:		
	// --- Out:
	// --- Returns:	TRUE if smooth scrolling is supported
	// --- Effect:	retrieves the flag that specifies whether smooth scrolling is 
	//				supported or not. Smooth scrolling means that the process of 
	//				scrolling of the contents from one position to another is divided
	//				in a number of smaller scrolling processes. Please note that
	//				if smooth scrolling is supported then it will slow down the 
	//				scrolling significantly which might	be intolerable for slow 
	//				computers or controls with large contents
	inline BOOL IsSmoothScrolling() const { return m_bSmoothScrolling; }

	// First handle left/right scroll messages. If scrolling by page,
	// scroll m_nPageSlices times rather than 1. If scrolling by line,
	// scroll m_nLineSlices times.
	// --- In:		nLineSlices		-	the number of steps to which the process of 
	//									scrolling by line will be divided	
	//				nPageSlices		-	the number of steps to which the process of 
	//									scrolling by page will be divided
	//				dwWaitingTime	-	delay in milliseconds between finishing
	//									the previous and starting the next 
	//									scrolling step
	// --- Out:
	// --- Returns:	
	// --- Effect:	setup smooth scrolling environment properties
	inline void SetSmoothEnvironment(int nLineSlices=4, int nPageSlices=12, 
		DWORD dwWaitingTime=10) 
	{ 
		m_nLineSlices=nLineSlices;
		m_nPageSlices=nPageSlices;
		m_dwWaitingTime=dwWaitingTime; 
	}
			

	// --- In:		bAlwaysFitWindow	-	if TRUE then anytime control window
	//										was resized the zoom level will be 
	//										adjusted automatically to fit the
	//										contents into the new size of window
	// --- Out:
	// --- Returns:
	// --- Effect:	sets/removes the flag that specifies whether the contents will be
	//				scalled automatically every time control is resized in order 
	//				to fit into the new window size
	inline void SetAlwaysFitWindow(BOOL bAlwaysFitWindow) 
	{
		m_bAlwaysFitWindow=bAlwaysFitWindow;
		if(m_bAlwaysFitWindow && ::IsWindow(GetSafeHwnd()))
			ZoomToWindow();
	}

	// --- In:		
	// --- Out:
	// --- Returns:	TRUE if every time the control window is resized the zoom level 
	//				will be adjusted automatically to fit the contents into the 
	//				new size of window
	// --- Effect:	retrieves the flag that specifies if every time the control 
	//				window is resized the zoom level will be adjusted automatically 
	//				to fit the contents into the new size of window
	inline BOOL IsAlwaysFitWindow() const { return m_bAlwaysFitWindow; }


	// --- In:		bDisplayContextMenu	-	if TRUE then popup context menu will
	//										be displayed every time a user right clicks 
	//										over the control. Using menu a user can 
	//										easily set most used predefined zooming
	//										levels
	// --- Out:
	// --- Returns:
	// --- Effect:	sets/removes the flag that specifies whether the predefined
	//				popup context menu will be displayed or not.
	inline void SetDisplayContextMenu(BOOL bDisplayContextMenu) {
		m_bDisplayContextMenu=bDisplayContextMenu; 
	}

	// --- In:		
	// --- Out:
	// --- Returns:	TRUE if popup context menu will be displayed every time 
	//				a user right clicks over the control. Using menu a user can 
	//				easily set most used predefined zooming	levels.
	// --- Effect:	retrieves the flag that specifies whether the predefined
	//				popup context menu will be displayed or not.
	inline BOOL GetDisplayContextMenu() const { return m_bDisplayContextMenu; }
	

	// --- In:		bUseTrackZoom	-	if TRUE then if a user left clicks and drag 
	//									the mouse cursor the tracker will be displayed 
	//									so a user can choose the rectangle to which 
	//									the contents must be zoomed
	// --- Out:
	// --- Returns:
	// --- Effect:	sets/removes the flag that specifies whether if a user 
	//				left clicks and drag the mouse cursor the tracker will be 
	//				displayed so a user can choose the rectangle to which the 
	//				contents must be zoomed
	inline void SetUseTrackZoom(BOOL bUseTrackZoom) {
		m_bUseTrackZoom=bUseTrackZoom; 
	}

	// --- In:		
	// --- Out:
	// --- Returns:	TRUE if when a user left clicks and drag the mouse cursor 
	//				the tracker will be displayed so a user can choose the 
	//				rectangle to which the contents must be zoomed
	// --- Effect:	retrieves the flag that specifies whether if a user 
	//				left clicks and drag the mouse cursor the tracker will be 
	//				displayed so a user can choose the rectangle to which the 
	//				contents must be zoomed
	inline BOOL GetUseTrackZoom() const { return m_bUseTrackZoom; }


	// --- In:		fFlags		-	Indicates whether various virtual keys are down. 
	//								This parameter can be any combination of the 
	//								following values: 
	//
	//						MK_CONTROL	-	Set if the CTRL key is down.
	//						MK_LBUTTON	-	Set if the left mouse button is down.
	//						MK_MBUTTON	-	Set if the middle mouse button is down.
	//						MK_RBUTTON	-	Set if the right mouse button is down.
	//						MK_SHIFT	-	Set if the SHIFT key is down. 
	//
	//				zDelta		-	Indicates distance rotated. The zDelta value is 
	//								expressed in multiples or divisions of WHEEL_DELTA, 
	//								which is 120. A value less than zero indicates 
	//								rotating back (toward the user) while a value 
	//								greater than zero indicates rotating forward 
	//								(away from the user). The user can reverse this 
	//								response by changing the Wheel setting in the 
	//								mouse software. See the Remarks for more 
	//								information about this parameter.
	//
	//				pt			-	Specifies the x- and y-coordinate of the cursor. 
	//								These coordinates are always relative to the 
	//								upper-left corner of the window.
	//
	// --- Out:
	// --- Returns:	TRUE if mouse wheel scrolling is enabled; otherwise FALSE.
	// --- Effect:	This member function is called when a user rotates the mouse wheel 
	//				and encounters the wheel's next notch. The zDelta parameter is a 
	//				multiple of WHEEL_DELTA, which is set at 120. This value is the 
	//				threshold for an action to be taken, and one such action 
	//				(for example, scrolling forward one notch) should occur for each 
	//				delta. The delta was set to 120 to allow for future 
	//				finer-resolution wheels, such as a freely-rotating wheel with no 
	//				notches. Such a device might send more messages per rotation, 
	//				but with a smaller value in each message. To support this 
	//				possibility, either aggregate the incoming delta values until 
	//				WHEEL_DELTA is reached (so you get the same response for a given 
	//				delta-rotation), or scroll partial lines in response to the more 
	//				frequent messages. You could also choose your scroll granularity 
	//				and accumulate deltas until WHEEL_DELTA is reached. 
	BOOL DoMouseWheel(UINT fFlags, short zDelta, CPoint point);

#ifdef OXSCRLWND_USE_RULER
	// --- In  :	bHorzRuler	-	horizontal ruler band will be displayed
	//				bVertRuler	-	vertical ruler band will be displayed
	// --- Out : 
	// --- Returns:	TRUE if COXRulerOrganizer object was successfully attached
	// --- Effect:	attaches to COXRulerOrganizer object which is responsible
	//				for displaying ruler bars
	inline BOOL AttachRuler(BOOL bHorzRuler=TRUE, BOOL bVertRuler=TRUE) { 
		if(m_ruler.IsAttached())
		{
			SetShowHorzRulerBar(bHorzRuler);
			SetShowVertRulerBar(bVertRuler);
			return TRUE;
		}
		return m_ruler.Attach(this,bHorzRuler,bVertRuler);
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	
	// --- Effect:	detaches the attached COXRulerOrganizer object which is responsible
	//				for displaying ruler bars
	inline void DetachRuler() { m_ruler.Detach(); }


	// --- In  :
	// --- Out : 
	// --- Returns:	pointer to the internal COXRulerOrganizer object that is used to 
	//				draw ruler bars
	// --- Effect:	retrieves the pointer to the internal COXRulerOrganizer object 
	//				that is used to draw ruler bars
	inline COXRulerOrganizer* GetRulerOrganizer() { return &m_ruler; }


	// --- In  :	bShowVertRulerBar	-	if TRUE then vertical ruler bar will
	//										be displayed
	// --- Out : 
	// --- Returns:	
	// --- Effect:	sets the flag that specifies whether to display the 
	//				vertical ruler bar or not
 	inline void SetShowVertRulerBar(BOOL bShowVertRulerBar) {
		m_ruler.SetShowVertRulerBar(bShowVertRulerBar);
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if vertical ruler bar is displayed or FALSE otherwise
	// --- Effect:	retrieves the flag that specifies whether to display the 
	//				vertical ruler bar or not
	inline BOOL GetShowVertRulerBar() const { return m_ruler.GetShowVertRulerBar(); }


	// --- In  :	bShowHorzRulerBar	-	if TRUE then horizontal ruler bar will
	//										be displayed
	// --- Out : 
	// --- Returns:	
	// --- Effect:	sets the flag that specifies whether to display the 
	//				horizontal ruler bar or not
	inline void SetShowHorzRulerBar(BOOL bShowHorzRulerBar) {
		m_ruler.SetShowHorzRulerBar(bShowHorzRulerBar);
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if horizontal ruler bar is displayed or FALSE otherwise
	// --- Effect:	retrieves the flag that specifies whether to display the 
	//				horizontal ruler bar or not
	inline BOOL GetShowHorzRulerBar() const { return m_ruler.GetShowHorzRulerBar(); }

#endif	//	OXSCRLWND_USE_RULER


	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXScrollWnd)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

#ifdef _DEBUG
	virtual void Dump(CDumpContext&) const;
	virtual void AssertValid() const;
#endif //_DEBUG

// Implementation
public:
	// --- In  :
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Destructs the object
	virtual ~COXScrollWnd();


protected:
	// virtual function, used to initialize the COXScrollWnd object after 
	// it was created or was used to subclass created window
	virtual BOOL Initialize();

	// scroll the contents to the specified position in device units
	void ScrollToDevicePosition(POINT ptDev); 
	
	// set of conversion functions that take CRect or CPoint in logical 
	// coordinates and convert them to/from scaled equivalents on the screen. 
	// These function might be very useful while drawing the contents
	void NormalToScaled(CRect* pRect);
	void NormalToScaled(CPoint* pPoint);
	void ScaledToNormal(CRect* pRect);
	void ScaledToNormal(CPoint* pPoint);
	///////////////////////////////////////

	// set of conversion functions that take CRect or CPoint in logical 
	// coordinates and convert them to/from scaled equivalents on the printer. 
	// These function might be very useful while printing the contents
	void NormalToPrinted(CDC* pDC, CRect* pRect);
	void NormalToPrinted(CDC* pDC, CPoint* pPoint);
	void PrintedToNormal(CDC* pDC, CRect* pRect);
	void PrintedToNormal(CDC* pDC, CPoint* pPoint);
	///////////////////////////////////////


	// helper functions for zooming
	void SetMapMode(CDC* pDC) const;
	inline void ScaleViewport(CDC* pDC) const { 
		pDC->ScaleViewportExt(m_nZoomLevel,100,m_nZoomLevel,100); 
	}
	///////////////////////////////////////

	// function must be called every time scroll info has been updated
	void UpdateBars(BOOL bSendRecalc=TRUE);

	// retrieves real size of the client area of the control 
	// taking into account the scrollbars
	BOOL GetTrueClientSize(CSize& size, CSize& sizeSb);
	// size with no bars
	void GetScrollBarSizes(CSize& sizeSb);
	// call this function to retrieve full scroll info
	void GetScrollBarState(CSize sizeClient, CSize& needSb,
		CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient);

	// this function is called by the framework to size windows prior to creation. 
	// It is called in order to compute the required size of the window rectangle 
	// based on the desired client-rectangle size. The resulting window rectangle 
	// (contained in lpClientRect) can then be passed to the Create member function 
	// to create a window whose client area is the desired size.
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);


	// you must call this function in your COXScrollWnd derived class
	// before drawing or printing out the contents. This function adjusts the 
	// origin of the specified device context based on the current zooming level 
	// and contents alignment
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

	// called internally to do scrolling. Might be override in order to 
	// implement scrolling functionality in different way
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	// called when smooth scrolling flag is set
	virtual BOOL OnSmoothScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE); 

	// called before displaying the context menu. Might be overriden in order to update 
	// the context menu
	virtual BOOL OnPopulateContextMenu(CMenu* pMenu, CPoint& point) {
		UNREFERENCED_PARAMETER(pMenu);
		UNREFERENCED_PARAMETER(point);
		return TRUE;
	}

	virtual BOOL HandleKeyInput(UINT nChar, UINT nRepCnt, UINT nFlags);

	// helper for mouse wheel support
	static UINT GetMouseScrollLines();

	// Generated message map functions
protected:
	//{{AFX_MSG(COXScrollWnd)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnChangeZoom(UINT nID);
	afx_msg void OnAlwaysFitWindow();
	afx_msg void OnUseTrackZoom();
	afx_msg void OnSmoothScrolling();
#ifdef OXSCRLWND_USE_RULER
	afx_msg void OnShowHorzRulerBar();
	afx_msg void OnShowVertRulerBar();
	afx_msg void OnRulerBarUseInches();
	afx_msg void OnRulerBarUseCentimeters();
#endif	//	OXSCRLWND_USE_RULER
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(_OXSCROLLWND_H__)
