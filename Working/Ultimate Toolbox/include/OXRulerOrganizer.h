// ==========================================================================
// 					Class Specification : COXRulerOrganizer
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

DESCRIPTION

We introduce a couple of classes that allows you to display horizontal and/or 
vertical ruler bars attached to any(!) window. By definition the ruler bar is 
rectangular area with scale. Ruler bar has base unit (by default we use 1 inch 
as base unit) every base unit is designated by corresponding number on the scale.
If distance between two units is big enough the ticks will be drawn on the scale.

Ruler bars are drawn in the non-client area of the attached window, much the same 
like scroll bars. 

Let's go to the details about the classes that implement the support for ruler bars.

First of all, we've got the class that implements all the functionality that has 
to do with the appearance of the ruler bar. This class is called COXRulerBar.

COXRulerBar is derived from CWnd and can be used independently as a stand alone 
ruler bar if you need to use it in order to provide some measurement functionality
in your application. In order to use this class independently you have to create
the bar using Create() function where you should specify orientation of the 
ruler bar (horizontal and vertical) and, optionally, the base unit that should be 
used (by default we use 1 inch as base unit). Alternately, you could subclass any 
existing window, but make sure to call Init() function right after subclassing in 
order to specify orientation and base unit size for the subclassed bar.

After that you can resize the bar and the ruler bar scale will be adjusted 
automatically. 

Ruler bar provides the following additional functionality:

1)	Hiliting
	
	Using Hilite() function you can hilite any rectangular area on the bar. 
	For example you can use this function in order to display a trace line 
	for the mouse movements.

2)	User defined base unit.
	
	Using SetCalibrate() function you can specify any base unit. By default 
	we use 1 inch as base unit which corresponds to the calibrate value of 100.
	In order to use centimeters use calibrate value of 254 (to use millimeters
	use 2540 and so on).

3)	Scrolling

	The contents of the ruler bar can be scrolled in the positive direction using
	Scroll() function. In this function you can specify in pixels new scroll 
	position for the bar (from the beginning the current scroll position is set to 0).

4)	Zooming

	The contents of the ruler bar can be zoomed in or zoomed out using Zoom() function.
	In this function you have to specify the new zoom level in percents relatively to
	the original size (original size corresponds to 100%)



COXRulerBar itself is useful control but in most cases you would be intersted to use
it in combination with another window. The anology with scroll bar comes here in handy.
You can use CScrollBar control independently but in most cases you want to associate
it with some window that supports scrolling functionality and use it without caring to
much about creating the scroll bars, positioning them and constantly updating them. 
The same story with ruler bars. Wouldn't it be nice to have automatic support for
horizontal and/or vertical ruler bars for any window. The COXRulerOrganizer class
provides that kind of functionality.

In order to provide ruler bars for any window you have to do the following:

1)	declare an object of COXRulerOrganizer class so the lifetime of such object would 
	be at least the same as the lifetime of the window for which ruler bars support
	must be provided.

	class CMyView : public CView
	{
	. . . . . . . . . . . . . . . 
	protected:
		COXRulerOrganizer m_ruler;
	. . . . . . . . . . . . . . . 
	}

2)	Attach the window to COXRulerOrganizer object using Attach() function. In this 
	function you can specify what ruler bars (horizontal and/or vertical) should be 
	shown from the very beginning.

	int CMyView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
	{
		if (CMyView::OnCreate(lpCreateStruct) == -1)
			return -1;
		. . . . . . . . . . . . . . . 
		// atach view to the COXRulerOrganizer object and display 
		// bot horizontal and vertical ruler bars
		VERIFY(m_ruler.Attach(this,TRUE,TRUE));
		. . . . . . . . . . . . . . . 
		return 0;
	}
	

That's it. After that all your mouse movements will be traced and displayed on the
ruler bars and if the attached window support scrolling the ruler bars will be
scrolled correspondingly.

After a window is attached to the COXRulerOrganizer object you can use a lot of
COXRulerOrganizer functions that would allow you to utilize this class functionality
to the full extent.

1)	Show/hide ruler bars. In order to do that call the following functions:

	SetShowHorzRulerBar();
	SetShowVertRulerBar();

2)	Change height/width of the bars. Call the following functions:

	SetHorzRulerBarHeight();
	SetVertRulerBarWidth();

3)	Retrieve the position of the ruler bars:

	GetHorzRulerBarRect();
	GetVertRulerBarRect();

4)	Direct access to the underlying COXRulerBar objects:

	GetHorzRulerBar();
	GetVertRulerBar();

5)	Set of wrapper functions that call corresponding COXRulerBar functions:

	// hilite area on the ruler bar
	HiliteRuler();

	// change base unit
	CalibrateRuler();

	// scroll the contents of the ruler bar
	ScrollRuler();

	// zoom in or zoom out the contents of the ruler
	ZoomRuler();



You can use these function at any time in your code as long as some window is 
attached to the corresponding COXRulerOrganizer object. For example: 

	// hide horizontal ruler 
	m_ruler.SetShowHorzRulerBar(FALSE);

	//use centimeters as base units for the horizontal and vertical ruler bars
	m_ruler.CalibrateRuler(254,TRUE);
	m_ruler.CalibrateRuler(254,FALSE);



The sample that demonstrate how to use COXRulerOrganizer class is called RulerBar 
and can be found in the .\Samples\gui\RulerBar subdirectory of your 
Ultimate Toolbox directory.



Dependency:
	#include "OXRulerOrganizer.h"


Source code files:
	"OXRulerOrganizer.cpp"
	"OXHookWnd.cpp"			-	implementation of the COXHookWnd class 
								from which the COXRulerOrganizer class is derived

*/



#ifndef _OXRULERORGANIZER_H__
#define _OXRULERORGANIZER_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include "OXHookWnd.h"


const int OXRB_HORZHEIGHT=18;
const int OXRB_VERTWIDTH=18;

const int OXRBO_BORDER=2;

// timer for checking scroll position
const UINT IDT_OXRBO_CHECK_SCROLLPOS=243;
// delay between two checks
const UINT OXRBO_CHECK_SCROLLPOS_DELAY=100;


/////////////////////////////////////////////////////////////////////////////
// COXRulerBar window

class OX_CLASS_DECL COXRulerBar : public CWnd
{
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Contructs the object
	COXRulerBar();


	// --- In  :	pParentWnd	-	pointer to the parent window, must not be NULL
	//				bHorizontal	-	if TRUE then the bar will be oriented 
	//								horizontally, if FALSE - vertically
	//				rect		-	bar's rectangle
	//				dwStyle		-	bar window style, must include WS_CHILD
	//				nCalibrate	-	base unit size, by default we set 
	//								it to 100 (1 inch), use 254 value to set the 
	//								base unit to represent 1 centimeter, 2540 -
	//								1 millimeter and so on.
	//				nID			-	bar window ID
	// --- Out : 
	// --- Returns:	TRUE if the ruler bar has been created successfully
	// --- Effect:	Creates the ruler bar
	virtual BOOL Create(CWnd* pParentWnd, BOOL bHorizontal, CRect rect, 
		DWORD dwStyle=WS_CHILD|WS_VISIBLE, UINT nCalibrate=100,
		UINT nID=IDC_STATIC);

	// --- In  :	nCalibrate	-	base unit size, by default we set 
	//								it to 100 (1 inch), use 254 value to set the 
	//								base unit to represent 1 centimeter, 2540 -
	//								1 millimeter and so on.
	//				bHorizontal	-	if TRUE then the bar will be oriented 
	//								horizontally, if FALSE - vertically
	// --- Out : 
	// --- Returns:	TRUE if the ruler bar has been initialized successfully
	// --- Effect:	initializes the object by setting its orientation and 
	//				calibrate value
	virtual BOOL Init(BOOL bHorizontal=TRUE, UINT nCalibrate=100);

// Attributes
public:

protected:
	// flag that specifies bar orientation
	BOOL m_bHorizontal;
	// origin position
	UINT m_nPos;
	// zoom level (100% is normal)
	UINT m_nZoom;
	// base unit size
	UINT m_nCalibrate; // 100=inches, 254=cm

	// the view's Dpi, by default we use 100 dpi
	double m_dDisplayDpi;
	// flag that specifies if non-default dpi (other than 100 dpi) should be used
	BOOL m_bUseNonDefaultDpi;

	// font to display horizontal ruler bar
	CFont m_font;
	// font to display vertical ruler bar
	CFont m_fontVert;

	// internal variables to draw hilited areas
	BOOL m_bOldIsHilited;
	CRect m_rectInvert;
	CRect m_rectOldInvert;
	////////////////////////////////////////////////

	// static helper for drawing ticks
	static double PIXEL_PAD;

// Operations
public:
	// --- In  :	pDC			-	pointer to the Device Context to draw in
	//				rect		-	bounding rectangle for the bar
	//				rectClip	-	part of the bounding rectangle that 
	//								should be drawn
	// --- Out : 
	// --- Returns:	
	// --- Effect:	draws ruler bar in the specified device Context
	virtual void DrawRuler(CDC* pDC, CRect rect, CRect rectClip);

	// --- In  :	pDC			-	pointer to the Device Context to draw in
	//				rect		-	bounding rectangle for the bar
	// --- Out : 
	// --- Returns:	
	// --- Effect:	draws scale with ticks and numbers on it
	virtual void DrawScale(CDC* pDC, CRect rect);


	// --- In  :	pDC			-	pointer to the Device Context to draw in
	//				rect		-	bounding rectangle for the bar
	//				nStartPos	-	start point of the area on the bar that 
	//								must be hilited 
	//				nWidth		-	width (height for a vertical bar) of the 
	//								hilite area
	// --- Out : 
	// --- Returns:	
	// --- Effect:	hilite the specified area on the bar
	void Hilite(CDC* pDC, CRect rect, int nStartPos, int nWidth=1);

	// --- In  :	bRedraw		-	if TRUE the bar will be redrawn right away
	// --- Out : 
	// --- Returns:	
	// --- Effect:	remove the previously set hilite area from the bar
	void RemoveHilite(BOOL bRedraw=TRUE);


	// --- In  :	nPos		-	new start position for the ruler bar scale
	//								in pixels
	//				bRedraw		-	if TRUE the bar will be redrawn right away
	// --- Out : 
	// --- Returns:	
	// --- Effect:	sets the new start position for the ruler bar scale	in pixels
	void Scroll(UINT nPos, BOOL bRedraw=TRUE);

	// --- In  :	
	// --- Out : 
	// --- Returns:	the current start position for the ruler bar scale
	// --- Effect:	retrieves the current start position for the 
	//				ruler bar scale (in pixels)
	inline UINT GetCurrentPos() const { return m_nPos; }

	
	// --- In :		nZoom		-	the new zoom level in percent relatively 
	//								to the original size.
	//				bRedraw		-	if TRUE the bar will be redrawn right away
	// --- Out :
	// --- Returns:	
	// --- Effect : sets the new zoom level
	void Zoom(UINT nZoom, BOOL bRedraw=TRUE);

	// --- In :		
	// --- Out :
	// --- Returns:	current zoom level
	// --- Effect : retrieves the current zoom level (100% - original size)
	inline UINT GetZoomLevel() const { return m_nZoom; }


	// --- In :		dDpi		-	the new value of the display resolution.
	//								By default we use 100 dpi
	//				bRedraw		-	if TRUE the bar will be redrawn right away
	// --- Out :
	// --- Returns:	
	// --- Effect : sets the new value of the display resolution (100 dpi is 
	//				used by default)
	void SetDpi(double dDpi, BOOL bRedraw=TRUE);

	// --- In :		
	// --- Out :
	// --- Returns:	the current value of the display resolution
	// --- Effect : retrieves the current value of the display resolution
	inline double GetDpi() const { return m_dDisplayDpi; }


	// --- In :		bUseDefaultDpi	-	if TRUE then the default display resolution
	//									will be used while calculating the appearance
	//									of the bar even though non-default value has 
	//									been set before.
	//				bRedraw			-	if TRUE the bar will be redrawn right away
	// --- Out :
	// --- Returns:	
	// --- Effect : sets the flag that will force to calculate the appearance of the 
	//				bar based on the default display resolution (100 dpi)
	void UseDefaultDpi(BOOL bUseDefaultDpi, BOOL bRedraw=TRUE);

	// --- In :		
	// --- Out :
	// --- Returns:	the flag that specifies if the control be forced to calculate 
	//				the appearance of the bar based on the default display 
	//				resolution (100 dpi)
	// --- Effect : retrieves the flag that will force to calculate the appearance 
	//				of the bar based on the default display resolution (100 dpi)
	inline BOOL IsUsingDefaultDpi() const { return !m_bUseNonDefaultDpi; }


	// --- In :		nCalibrate	-	new base unit size. Use 100 for	inches, 
	//								254 for centimeters	
	//				bRedraw		-	if TRUE the bar will be redrawn right away
	// --- Out :
	// --- Returns:	
	// --- Effect : sets the base unit size. Base unit size defines the appearance
	//				of the ruler bar scale
	void SetCalibrate(UINT nCalibrate, BOOL bRedraw=TRUE);

	// --- In :		
	// --- Out :
	// --- Returns:	
	// --- Effect : retrieves the base unit size. Base unit size defines the appearance
	//				of the ruler bar scale. The value of 100 represents 1 inch,
	//				254 represents 1 centimeter
	inline double GetCalibrate() const { return m_nCalibrate; }


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXRulerBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	// --- In  :
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Destructs the object
	virtual ~COXRulerBar();

protected:
	// helper function for drawing tick on the ruler bar scale
	virtual void DrawTick(CDC* pDC, CRect& rect, double dOffset, BOOL bMidTick);
	// sets/removes hilite from the bar scale
	virtual void ReverseHilite(CDC* pDC, CRect rect);

	// Generated message map functions
protected:
	//{{AFX_MSG(COXRulerBar)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//////////////////////////////////////////////////////


class OX_CLASS_DECL COXRulerOrganizer : public COXHookWnd
{
protected:
	// flag that specifies whether the horizontal ruler bar will be shown or not
	BOOL m_bShowHorzRulerBar;
	// flag that specifies whether the vertical ruler bar will be shown or not
	BOOL m_bShowVertRulerBar;

	// rectangle for horizontal ruler bar
	CRect m_rectHorzRulerBar;
	// rectangle for vertical ruler bar
	CRect m_rectVertRulerBar;
	// rectangle for blank space
	CRect m_rectBlank;

	// height of the horizontal ruler bar
	int m_nHorzRulerBarHeight;
	// width of the vertical ruler bar
	int m_nVertRulerBarWidth;

	// horizontal ruler bar
	COXRulerBar m_horzRulerBar;
	// vert ruler bar
	COXRulerBar m_vertRulerBar;

	// timer to check the current scroll position in the attached window
	// and to update ruler bars correspondingly
	INT_PTR m_nTimerCheckScrollPos;

public:

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXRulerOrganizer();

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXRulerOrganizer();


	// --- In  :	pWnd		-	pointer to the CWnd object of created window 
	//								that would use rulers
	//				bHorzRuler	-	horizontal ruler band will be displayed
	//				bVertRuler	-	vertical ruler band will be displayed
	// --- Out : 
	// --- Returns: TRUE if specified window was successfully attached
	// --- Effect : Subclasses the specified window in order to display ruler bands.
	BOOL Attach(CWnd* pWnd, BOOL bHorzRuler=TRUE, BOOL bVertRuler=TRUE);

	// --- In  : 
	// --- Out : 
	// --- Returns:	
	// --- Effect : Unsubclasses the attached window
	void Detach();

	// --- In  : 
	// --- Out : 
	// --- Returns:	TRUE if the object is attached to some window, or FALSE otherwise
	// --- Effect : retrieves the flag that specifies whether the object is attached 
	//				to any window or not
	inline BOOL IsAttached() const { return IsHooked(); }


	// --- In  :	bRecalcSize	-	if TRUE then position of the ruler bars will be
	//								recalculated
	// --- Out : 
	// --- Returns:	
	// --- Effect : redraws, and recalculate the position if specified, 
	//				the ruler bars
	inline void RedrawAttached(BOOL bRecalcSize=FALSE) 
	{
		if(bRecalcSize)
		{
			::SetWindowPos(m_hWndHooked,NULL,0,0,0,0,
				SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER);
		}
		::RedrawWindow(m_hWndHooked,NULL,NULL,
			RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW);
	}


	// --- In  :	bShowHorzRulerBar	-	if TRUE then horizontal ruler bar will be
	//										displayed, otherwise it will be hidden
	// --- Out : 
	// --- Returns:	
	// --- Effect : shows/hides horizontal ruler bar
	inline void SetShowHorzRulerBar(BOOL bShowHorzRulerBar) 
	{
		m_bShowHorzRulerBar=bShowHorzRulerBar;
		RedrawAttached(TRUE);
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if horizontal ruler bar is displayed and FALSE if hidden
	// --- Effect : retrieves the flag that specifies whether the horizontal 
	//				ruler bar is shown or hidden
	inline BOOL GetShowHorzRulerBar() const { return m_bShowHorzRulerBar; }

	
	// --- In  :	bShowVertRulerBar	-	if TRUE then vertical ruler bar will be
	//										displayed, otherwise it will be hidden
	// --- Out : 
	// --- Returns:	
	// --- Effect : shows/hides vertical ruler bar
	inline void SetShowVertRulerBar(BOOL bShowVertRulerBar) 
	{
		m_bShowVertRulerBar=bShowVertRulerBar;
		RedrawAttached(TRUE);
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if vertical ruler bar is displayed and FALSE if hidden
	// --- Effect : retrieves the flag that specifies whether the vertical 
	//				ruler bar is shown or hidden
	inline BOOL GetShowVertRulerBar() const { return m_bShowVertRulerBar; }


	// --- In  :	nHorzRulerBarHeight	-	new height for the horizontal ruler bar
	// --- Out : 
	// --- Returns:	
	// --- Effect : sets the new height for horizontal ruler bar
	void SetHorzRulerBarHeight(int nHorzRulerBarHeight);

	// --- In  :	
	// --- Out : 
	// --- Returns:	height of the horizontal ruler bar 
	// --- Effect : retrieves the height of the horizontal ruler bar
	inline int GetHorzRulerBarHeight() const { return m_nHorzRulerBarHeight; }

	// --- In  :	
	// --- Out : 
	// --- Returns:	rectangle for the horizontal ruler bar 
	// --- Effect : retrieves the rectangle for the horizontal ruler bar in screen 
	//				coordinates. 
	inline CRect GetHorzRulerBarRect() const 
	{ 
		CRect rectWindow;
		::GetWindowRect(m_hWndHooked,rectWindow);
		CRect rect=m_rectHorzRulerBar;
		rect+=rectWindow.TopLeft();
		return rect; 
	}


	// --- In  :	nVertRulerBarWidth	-	new width for the vertical ruler bar 
	// --- Out : 
	// --- Returns:	
	// --- Effect : sets the new width for vertical ruler bar
	void SetVertRulerBarWidth(int nVertRulerBarWidth);

	// --- In  :	
	// --- Out : 
	// --- Returns:	width of the vertical ruler bar 
	// --- Effect : retrieves the width of the vertical ruler bar
	inline int GetVertRulerBarWidth() const { return m_nVertRulerBarWidth; }

	// --- In  :	
	// --- Out : 
	// --- Returns:	rectangle for the vertical ruler bar 
	// --- Effect : retrieves the rectangle for the vertical ruler bar in screen 
	//				coordinates. 
	inline CRect GetVertRulerBarRect() const 
	{ 
		CRect rectWindow;
		::GetWindowRect(m_hWndHooked,rectWindow);
		CRect rect=m_rectVertRulerBar;
		rect+=rectWindow.TopLeft();
		return rect; 
	}


	// --- In  :	
	// --- Out : 
	// --- Returns:	pointer to the horizontal ruler bar object
	// --- Effect : retrieves the pointer to the horizontal ruler bar object
	inline COXRulerBar* GetHorzRulerBar() { return &m_horzRulerBar; }

	// --- In  :	
	// --- Out : 
	// --- Returns:	pointer to the vertical ruler bar object
	// --- Effect : retrieves the pointer to the vertical ruler bar object
	inline COXRulerBar* GetVertRulerBar() { return &m_vertRulerBar; }

	
	// --- In  :	nPos		-	new start position for the ruler bar scale
	//								in pixels
	//				bHorizontal	-	if TRUE then the operation will be applied to 
	//								the horizontal ruler bar, otherwise to the
	//								vertical ruler bar
	//				bRedraw		-	if TRUE the bar will be redrawn right away
	// --- Out : 
	// --- Returns:	
	// --- Effect:	sets the new start position for the specified ruler bar 
	//				scale in pixels
	inline void ScrollRuler(UINT nPos, BOOL bHorizontal, BOOL bRedraw=TRUE) 
	{
		if(bHorizontal)
			m_horzRulerBar.Scroll(nPos,FALSE);
		else
			m_vertRulerBar.Scroll(nPos,FALSE);
		if(bRedraw)
			RedrawAttached();
	}

	// --- In :		bHorizontal	-	if TRUE then the operation will be applied to 
	//								the horizontal ruler bar, otherwise to the
	//								vertical ruler bar
	// --- Out : 
	// --- Returns:	the current start position for the ruler bar scale
	// --- Effect:	retrieves the current start position for the 
	//				specified ruler bar scale (in pixels)
	inline UINT GetRulerCurrentPos(BOOL bHorizontal) const 
	{
		if(bHorizontal)
			return m_horzRulerBar.GetCurrentPos();
		else
			return m_vertRulerBar.GetCurrentPos();
	}

	
	// --- In  :	nStartPos	-	start point of the area on the bar that 
	//								must be hilited 
	//				bHorizontal	-	if TRUE then the operation will be applied to 
	//								the horizontal ruler bar, otherwise to the
	//								vertical ruler bar
	//				nWidth		-	width (height for a vertical bar) of the 
	//								hilite area
	// --- Out : 
	// --- Returns:	
	// --- Effect:	Hilites the specified area on the bar
	inline void HiliteRuler(int nStartPos, BOOL bHorizontal, int nWidth=1) 
	{
		CWindowDC dc(GetHookedWnd());
		if(bHorizontal)
		{
			m_horzRulerBar.Hilite(&dc,m_rectHorzRulerBar,nStartPos,nWidth);
		}
		else
		{
			m_vertRulerBar.Hilite(&dc,m_rectVertRulerBar,nStartPos,nWidth);
		}
	}

	
	// --- In :		nZoom		-	the new zoom level in percent relatively 
	//								to the original size.
	//				bHorizontal	-	if TRUE then the operation will be applied to 
	//								the horizontal ruler bar, otherwise to the
	//								vertical ruler bar
	//				bRedraw		-	if TRUE the bar will be redrawn right away
	// --- Out :
	// --- Returns:	
	// --- Effect : sets the new zoom level for corresponding ruler bar
	inline void ZoomRuler(UINT nZoom, BOOL bHorizontal, BOOL bRedraw=TRUE) 
	{
		if(bHorizontal)
		{
			m_horzRulerBar.Zoom(nZoom,FALSE);
			m_horzRulerBar.Scroll(
				GetHookedWnd()->GetScrollPos(SB_HORZ)*100/nZoom,FALSE);
		}
		else
		{
			m_vertRulerBar.Zoom(nZoom,FALSE);
			m_vertRulerBar.Scroll(
				GetHookedWnd()->GetScrollPos(SB_VERT)*100/nZoom,FALSE);
		}
		if(bRedraw)
		{
			RedrawAttached();
		}
	}

	// --- In :		bHorizontal	-	if TRUE then the operation will be applied to 
	//								the horizontal ruler bar, otherwise to the
	//								vertical ruler bar
	// --- Out :
	// --- Returns:	current zoom level for the specified ruler bar
	// --- Effect : retrieves the current zoom level (100% - original size) of 
	//				the specified ruler bar
	inline UINT GetRulerZoomLevel(BOOL bHorizontal) const 
	{
		if(bHorizontal)
			return m_horzRulerBar.GetZoomLevel();
		else
			return m_vertRulerBar.GetZoomLevel();
	}


	// --- In :		nCalibrate	-	new base unit size for the specified ruler. 
	//								Use 100 for	inches, 254 for centimeters	
	//				bHorizontal	-	if TRUE then the operation will be applied to 
	//								the horizontal ruler bar, otherwise to the
	//								vertical ruler bar
	//				bRedraw		-	if TRUE the bar will be redrawn right away
	// --- Out :
	// --- Returns:	
	// --- Effect : sets the base unit size for the specified ruler. Base unit size 
	//				defines the appearance of the ruler bar scale
	inline void CalibrateRuler(UINT nCalibrate, BOOL bHorizontal, BOOL bRedraw=TRUE) 
	{
		if(bHorizontal)
		{
			m_horzRulerBar.SetCalibrate(nCalibrate,FALSE);
		}
		else
		{
			m_vertRulerBar.SetCalibrate(nCalibrate,FALSE);
		}
		if(bRedraw)
			RedrawAttached();
	}

	// --- In :		bHorizontal	-	if TRUE then the operation will be applied to 
	//								the horizontal ruler bar, otherwise to the
	//								vertical ruler bar
	// --- Out :
	// --- Returns:	base unit size for the specified ruler bar
	// --- Effect : retrieves the base unit size for the specified ruler bar. 
	//				Base unit size defines the appearance of the ruler bar scale. 
	//				The value of 100 represents 1 inch,	254 represents 1 centimeter
	inline double GetRulerCalibrate(BOOL bHorizontal) const 
	{ 		
		if(bHorizontal)
			return m_horzRulerBar.GetCalibrate();
		else
			return m_vertRulerBar.GetCalibrate();
	}


	// --- In :		dDpi		-	the new value of the display resolution.
	//								By default we use 100 dpi
	//				bHorizontal	-	if TRUE then the operation will be applied to 
	//								the horizontal ruler bar, otherwise to the
	//								vertical ruler bar
	//				bRedraw		-	if TRUE the bar will be redrawn right away
	// --- Out :
	// --- Returns:	
	// --- Effect : sets the new value of the display resolution (100 dpi is 
	//				used by default) for the specified ruler bar
	inline void SetRulerDpi(double dDpi, BOOL bHorizontal, BOOL bRedraw=TRUE) 
	{
		if(bHorizontal)
			m_horzRulerBar.SetDpi(dDpi,FALSE);
		else
			m_vertRulerBar.SetDpi(dDpi,FALSE);
		if(bRedraw)
			RedrawAttached();
	}

	// --- In :		bHorizontal	-	if TRUE then the operation will be applied to 
	//								the horizontal ruler bar, otherwise to the
	//								vertical ruler bar
	// --- Out :
	// --- Returns:	the current value of the display resolution of the specified
	//				ruler bar
	// --- Effect : retrieves the current value of the display resolution of the 
	//				specified ruler bar
	inline double GetRulerDpi(BOOL bHorizontal) const 
	{
		if(bHorizontal)
			return m_horzRulerBar.GetDpi();
		else
			return m_vertRulerBar.GetDpi();
	}

	
	// --- In :		bUseDefaultDpi	-	if TRUE then the default display resolution
	//									will be used while calculating the appearance
	//									of the bar even though non-default value has 
	//									been set before.
	//				bHorizontal		-	if TRUE then the operation will be applied to 
	//									the horizontal ruler bar, otherwise to the
	//									vertical ruler bar
	//				bRedraw			-	if TRUE the bar will be redrawn right away
	// --- Out :
	// --- Returns:	
	// --- Effect : sets the flag that will force to calculate the appearance of the 
	//				specified bar based on the default display resolution (100 dpi)
	inline void RulerUseDefaultDpi(BOOL bUseDefaultDpi, BOOL bHorizontal, 
		BOOL bRedraw=TRUE) 
	{
		if(bHorizontal)
			m_horzRulerBar.UseDefaultDpi(bUseDefaultDpi,FALSE);
		else
			m_vertRulerBar.UseDefaultDpi(bUseDefaultDpi,FALSE);
		if(bRedraw)
			RedrawAttached();
	}

	// --- In :		bHorizontal	-	if TRUE then the operation will be applied to 
	//								the horizontal ruler bar, otherwise to the
	//								vertical ruler bar
	// --- Out :
	// --- Returns:	the flag that specifies if the given ruler bar be forced to 
	//				calculate its appearance based on the default display 
	//				resolution (100 dpi)
	// --- Effect : retrieves the flag that will force to calculate the appearance 
	//				of the specified bar based on the default display 
	//				resolution (100 dpi)
	inline BOOL IsRulerUsingDefaultDpi(BOOL bHorizontal) const 
	{
		if(bHorizontal)
			return m_horzRulerBar.IsUsingDefaultDpi();
		else
			return m_vertRulerBar.IsUsingDefaultDpi();
	}


protected:
	// function check the current scroll position of the window and update the 
	// ruler bars correspondingly
	void CheckScrollPos();

	// calculate the layout of the ruler bars with the attached window
	virtual void CalcLayout(BOOL bRepositionBars=FALSE);

	// function to handle messages before the attached window has a chance to 
	// handle them
	virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif //_OXRULERORGANIZER_H__
/////////////////////////////////////////////////////////////////////////////
