// ==========================================================================
// 							   Class Specification : 
//						COXCaptionInfo & COXCaptionPainter
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

#ifndef _CAPTIONPAINTER_H
#define _CAPTIONPAINTER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#define __AFXTEMPL_H__
#endif

#ifndef __AFXPRIV_H__
#include <afxpriv.h>
#define __AFXPRIV_H__
#endif

#include "OXHookWnd.h"

//////////////////////////////////////////////////////////////////////////////
// IMPORTANT !!!
//////////////////////////////////////////////////////////////////////////////
// if you wouldn't need to save state of caption into registry then
// you probably wouldn't want to include source code for 
// COXRegistryValFile class into your project (that is a must if you 
// call COXCaptionPainter::SaveState() and/or COXCaptionPainter::LoadState()).
// If this is a case then you have to define include in you stdafx.h file
// next code:
//	
//	#define OXCP_NO_SAVESTATE
//
//////////////////////////////////////////////////////////////////////////////

// types of gradient alignment: left, right and center 
#define ID_GRADIENT_LEFT		0x00000000
#define ID_GRADIENT_CENTER		0x00000001
#define ID_GRADIENT_RIGHT		0x00000002

// types of algorithm used to draw gradient caption
#define ID_GRADIENT_LINEAR		0x00000000
#define ID_GRADIENT_SQUARE		0x00000001

// generic constant used to define that none of custom color was set
#define ID_OXCP_COLOR_NONE		0xffffffff

// inner constatnts used in SetHackStyle and SetHackStyleEx functions
#define ID_OXHACK_ADDSTYLE		0x00000000
#define ID_OXHACK_REMOVESTYLE	0x00000001

//////////////////////////////////////////////////////////////////////////
// Class COXCaptionInfo defines caption attributes. This class is heavily
// used by COXCaptionPainter class. The main feature of this class is
// that it is serializable. That means that you can save caption info
// in a file or registry.
//
class OX_CLASS_DECL COXCaptionInfo : public CObject 
{

    DECLARE_SERIAL(COXCaptionInfo)

// member variables
protected:
	// Background color, by default it is set to ID_OXCP_COLOR_NONE that means
	// that COXCaptionPainter will use standard color to fill caption
	COLORREF m_clrBackground;

	// If TRUE then COXCaptionPainter will draw caption gradiently
	BOOL m_bGradient;
	
	// Gradient filling means that caption will be drawn using set of colors 
	// from m_clrBackground to black. Gradient alignment is defined by location
	// of black color in the caption. At the moment next are defined:
	//		ID_GRADIENT_LEFT
	//		ID_GRADIENT_CENTER
	//		ID_GRADIENT_RIGHT
	UINT m_nGradientAlignment;
	
	// Gradient Filling could be accomplished using different algorithm. We support
	// two algorithms: linear and squared, that are defined by next constants:
	//		ID_GRADIENT_LINEAR
	//		ID_GRADIENT_SQUARE
	// 
	// Linear algorithm fill caption from black to m_clrBackground using next formula:
	//
	//		color = m_clrBackground * r
	//
	// where r is the ratio x/w, which ranges from 0 (x=0, left) to 1 (x=w, right). 
	// This results in a mostly black title bar, since we humans don't distinguish 
	// dark colors as well as light ones. 
	// Squared algorithm resolves this problem. It used next formula:
	//
	//		color = m_clrBackground * [1-(1-r)^2]
	//
	// which still equals black when r=0 and CaptionColor when r=1, but spends more 
	// time near m_clrBackground. For example, when r=0.5, the multiplier is 
	// [1-(1-.5)^2] = 0.75, closer to 1 than 0.5. Above formula is equivalent to
	//
	//		color = CaptionColor - (CaptionColor*(w-x)*(w-x))/(w*w)
	//
	// The computation looks horrendous, but it's only done once each
	// time the caption changes size; thereafter BitBlt'ed to the screen.
	UINT m_nGradientAlgorithm;
	
	// The number of color shades used for gradient filling. By default we use 64 shades
	UINT m_nNumberShade;
	
	// The font used to draw window's text. By default we use standard font used to 
	// draw in a caption.
	CFont m_font;
	
	// The font used to draw window's text (Small caption). By default we use 
	// standard font used to draw in a caption.
	CFont m_fontSm;
	
	// Color used to draw window's text. By default it is set to ID_OXCP_COLOR_NONE 
	// that means that COXCaptionPainter will use standard color to draw window's text
	COLORREF m_clrText;
	
	// special format to draw window's text. Could be a combination of:
	//		DT_BOTTOM		-	Justifies the text to the bottom of the caption rectangle. 
	//							This value must be combined with DT_SINGLELINE. 
	//		DT_CENTER		-	Centers text horizontally in the caption rectangle. 
	//		DT_EDITCONTROL	-	Duplicates the text-displaying characteristics of 
	//							a multiline edit control. Specifically, the average 
	//							character width is calculated in the same manner as for 
	//							an edit control, and the function does not display  
	//							a partially visible last line. 
	//		DT_END_ELLIPSIS	
	//	 or DT_PATH_ELLIPSIS-	Replaces part of the given string with ellipses, 
	//							if necessary, so that the result fits in the caption 
	//							rectangle. The given string is not modified unless 
	//							the DT_MODIFYSTRING flag is specified. You can specify 
	//							DT_END_ELLIPSIS to replace characters at the end of the 
	//							string, or DT_PATH_ELLIPSIS to replace characters in the 
	//							middle of the string. If the string contains backslash (\)
	//							characters, DT_PATH_ELLIPSIS preserves as much as 
	//							possible of the text after the last backslash.
	//		DT_EXPANDTABS	-	Expands tab characters. The default number of characters 
	//							per tab is eight. 
	//		DT_EXTERNALLEADING	-	Includes the font external leading in line height. 
	//							Normally, external leading is not included in the height 
	//							of a line of text. 
	//		DT_LEFT			-	Aligns text to the left. 
	//		DT_MODIFYSTRING	-	Modifies the given string to match the displayed text. 
	//							This flag has no effect unless the DT_END_ELLIPSIS or 
	//							DT_PATH_ELLIPSIS flag is specified. 
	//		DT_NOPREFIX		-	Turns off processing of prefix characters. Normally, 
	//							the mnemonic-prefix character & is interpreted 
	//							as a directive to underscore the character that follows, 
	//							and the mnemonic-prefix characters && as a directive to 
	//							print a single &. By specifying DT_NOPREFIX, 
	//							this processing is turned off. 
	//		DT_RIGHT		-	Aligns text to the right. 
	//		DT_RTLREADING	-	Layout in right to left reading order for bi-directional 
	//							text when the font selected into the hdc is a Hebrew or 
	//							Arabic font. The default reading order for all text is 
	//							left to right. 
	//		DT_SINGLELINE	-	Displays text on a single line only. Carriage returns 
	//							and linefeeds do not break the line. 
	//		DT_TOP			-	Top-justifies text (single line only). 
	//		DT_VCENTER		-	Centers text vertically (single line only). 
	//		DT_WORDBREAK	-	Breaks words. Lines are automatically broken between 
	//							words if a word would extend past the edge of the 
	//							caption rectangle. A carriage return-linefeed sequence 
	//							also breaks the line. 
	//		DT_WORD_ELLIPSIS-	Truncates text that does not fit in the caption rectangle 
	//							and adds ellipses.  
	//
	// By default we use DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS
	UINT m_nTextFormat;


public:  
    virtual void Serialize(CArchive& ar);

private:
	// helper function for font serialization
	void SerializeFont(CArchive& ar, CFont* pFont);

public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
    COXCaptionInfo();

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destroys the object
    virtual ~COXCaptionInfo() {}


	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets all caption info properties to its default value
	void Reset();

	
	// --- In  :	clrBackground	-	background color. If specified as 
	//									ID_OXCP_COLOR_NONE then standard color 
	//									will be used
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets caption's background color
	inline void SetBackgroundColor(COLORREF clrBackground=ID_OXCP_COLOR_NONE) { 
		m_clrBackground=clrBackground; 
	}

	// --- In  :	bActive	-	TRUE if background color is requsted for caption in 
	//							the active state, or FALSE if in the inactive.
	// --- Out : 
	// --- Returns: Caption's background color. If it was set as ID_OXCP_COLOR_NONE 
	//				then standard color will be returned (that's why we need bActive).
	// --- Effect : 
	COLORREF GetBackgroundColor(BOOL bActive) const;


	// --- In  :	clrText	-	color to draw window's text. If specified as 
	//							ID_OXCP_COLOR_NONE then standard color will be used
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets color to draw window's text.
	inline void SetTextColor(COLORREF clrText=ID_OXCP_COLOR_NONE) { m_clrText=clrText; }

	// --- In  :	bActive	-	TRUE if text color is requsted for caption in 
	//							the active state, or FALSE if in the inactive.
	// --- Out : 
	// --- Returns: Color to draw window's text. If it was set as ID_OXCP_COLOR_NONE 
	//				then standard color will be returned (that's why we need bActive).
	// --- Effect : 
	COLORREF GetTextColor(BOOL bActive) const;


	// --- In  :	bGradient	-	if TRUE caption will be filled gradiently,
	//								otherwise plain.
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets type of caption filling.
	inline void SetGradient(BOOL bGradient) { m_bGradient=bGradient; }

	// --- In  :	bGradient	-	if TRUE caption will be filled gradiently,
	//								otherwise plain.
	// --- Out : 
	// --- Returns:	TRUE if caption is filled gradiently, or FALSE otherwise.
	// --- Effect :
	inline BOOL GetGradient() const { return m_bGradient; }


	// --- In  :	nGradientAlignment	-	type of gradient alignment. 
	//										Next are allowed:
	//
	//										ID_GRADIENT_LEFT
	//										ID_GRADIENT_CENTER
	//										ID_GRADIENT_RIGHT
	// --- Out : 
	// --- Returns:	
	// --- Effect :	Sets gradient alignment.
	inline void SetGradientAlignment(UINT nGradientAlignment) { 
		ASSERT(nGradientAlignment==ID_GRADIENT_LEFT || 
			nGradientAlignment==ID_GRADIENT_CENTER || 
			nGradientAlignment==ID_GRADIENT_RIGHT);
		m_nGradientAlignment=nGradientAlignment; 
	}

	// --- In  :
	// --- Out : 
	// --- Returns:	Gradient alignment.
	// --- Effect :	
	inline UINT GetGradientAlignment() const { return m_nGradientAlignment; }


	// --- In  :	nGradientAlgorithm	-	type of gradient algorithm. 
	//										Next are allowed:
	//
	//										ID_GRADIENT_LINEAR
	//										ID_GRADIENT_SQUARE
	// --- Out : 
	// --- Returns:	
	// --- Effect :	Sets gradient algorithm.
	//
	// Gradient Filling could be accomplished using different algorithm. We support
	// two algorithms: linear and squared, that are defined by next constants:
	//		ID_GRADIENT_LINEAR
	//		ID_GRADIENT_SQUARE
	// 
	// Linear algorithm fill caption from black to m_clrBackground using next formula:
	//
	//		color = m_clrBackground * r
	//
	// where r is the ratio x/w, which ranges from 0 (x=0, left) to 1 (x=w, right). 
	// This results in a mostly black title bar, since we humans don't distinguish 
	// dark colors as well as light ones. 
	// Squared algorithm resolves this problem. It used next formula:
	//
	//		color = m_clrBackground * [1-(1-r)^2]
	//
	// which still equals black when r=0 and CaptionColor when r=1, but spends more 
	// time near m_clrBackground. For example, when r=0.5, the multiplier is 
	// [1-(1-.5)^2] = 0.75, closer to 1 than 0.5. Above formula is equivalent to
	//
	//		color = CaptionColor - (CaptionColor*(w-x)*(w-x))/(w*w)
	//
	// The computation looks horrendous, but it's only done once each
	// time the caption changes size; thereafter BitBlt'ed to the screen.
	//
	inline void SetGradientAlgorithm(UINT nGradientAlgorithm) { 
		ASSERT(nGradientAlgorithm==ID_GRADIENT_LINEAR || 
			nGradientAlgorithm==ID_GRADIENT_SQUARE);
		m_nGradientAlgorithm=nGradientAlgorithm; 
	}

	// --- In  :
	// --- Out : 
	// --- Returns:	Gradient algorithm.
	// --- Effect :	
	inline UINT GetGradientAlgorithm() const { return m_nGradientAlgorithm; }

	
	// --- In  :	plf	-	point to LOGFONT structure.
	// --- Out : 
	// --- Returns:	TRUE if succeeds, or FALSE otherwise
	// --- Effect :	Sets font used to draw text in caption. By default we use 
	//				current window font. You can set whatever font you want to 
	//				display the caption text, but remember that it is up to you 
	//				to adjust the height of the caption. If plf is NULL then 
	//				standard font will be used to draw text in caption.
	BOOL SetCaptionLogFont(LOGFONT* plf=NULL);

	// --- In  :	plf	-	point to LOGFONT structure.
	// --- Out : 
	// --- Returns:	TRUE if succeeds, or FALSE otherwise
	// --- Effect :	Sets font used to draw text in caption (Small caption). 
	//				By default we use current window font. You can set 
	//				whatever font you want to display the caption text, 
	//				but remember that it is up to you to adjust the height 
	//				of the caption. If plf is NULL then standard font will 
	//				be used to draw text in caption.
	BOOL SetSmCaptionLogFont(LOGFONT* plf=NULL);


	// --- In  :	plf	-	point to LOGFONT structure.
	// --- Out : 
	// --- Returns:	TRUE if succeeds, or FALSE otherwise
	// --- Effect :	Fills plf with font used to draw text in caption. 
	BOOL GetCaptionLogFont(LOGFONT* plf) const;

	// --- In  :	plf	-	point to LOGFONT structure.
	// --- Out : 
	// --- Returns:	TRUE if succeeds, or FALSE otherwise
	// --- Effect :	Fills plf with font used to draw text in caption 
	//				(Small Caption). 
	BOOL GetSmCaptionLogFont(LOGFONT* plf) const;


	// --- In  :	nTextFormat	-	format used to draw text in caption
	// --- Out : 
	// --- Returns:	
	// --- Effect :	Sets format to display window's text. 
	//				Could be a combination of:
	//		DT_BOTTOM		-	Justifies the text to the bottom of the caption rectangle. 
	//							This value must be combined with DT_SINGLELINE. 
	//		DT_CENTER		-	Centers text horizontally in the caption rectangle. 
	//		DT_EDITCONTROL	-	Duplicates the text-displaying characteristics of 
	//							a multiline edit control. Specifically, the average 
	//							character width is calculated in the same manner as for 
	//							an edit control, and the function does not display  
	//							a partially visible last line. 
	//		DT_END_ELLIPSIS	
	//	 or DT_PATH_ELLIPSIS-	Replaces part of the given string with ellipses, 
	//							if necessary, so that the result fits in the caption 
	//							rectangle. The given string is not modified unless 
	//							the DT_MODIFYSTRING flag is specified. You can specify 
	//							DT_END_ELLIPSIS to replace characters at the end of the 
	//							string, or DT_PATH_ELLIPSIS to replace characters in the 
	//							middle of the string. If the string contains backslash (\)
	//							characters, DT_PATH_ELLIPSIS preserves as much as 
	//							possible of the text after the last backslash.
	//		DT_EXPANDTABS	-	Expands tab characters. The default number of characters 
	//							per tab is eight. 
	//		DT_EXTERNALLEADING	-	Includes the font external leading in line height. 
	//							Normally, external leading is not included in the height 
	//							of a line of text. 
	//		DT_LEFT			-	Aligns text to the left. 
	//		DT_MODIFYSTRING	-	Modifies the given string to match the displayed text. 
	//							This flag has no effect unless the DT_END_ELLIPSIS or 
	//							DT_PATH_ELLIPSIS flag is specified. 
	//		DT_NOPREFIX		-	Turns off processing of prefix characters. Normally, 
	//							the mnemonic-prefix character & is interpreted 
	//							as a directive to underscore the character that follows, 
	//							and the mnemonic-prefix characters && as a directive to 
	//							print a single &. By specifying DT_NOPREFIX, 
	//							this processing is turned off. 
	//		DT_RIGHT		-	Aligns text to the right. 
	//		DT_RTLREADING	-	Layout in right to left reading order for bi-directional 
	//							text when the font selected into the hdc is a Hebrew or 
	//							Arabic font. The default reading order for all text is 
	//							left to right. 
	//		DT_SINGLELINE	-	Displays text on a single line only. Carriage returns 
	//							and linefeeds do not break the line. 
	//		DT_TOP			-	Top-justifies text (single line only). 
	//		DT_VCENTER		-	Centers text vertically (single line only). 
	//		DT_WORDBREAK	-	Breaks words. Lines are automatically broken between 
	//							words if a word would extend past the edge of the 
	//							caption rectangle. A carriage return-linefeed sequence 
	//							also breaks the line. 
	//		DT_WORD_ELLIPSIS-	Truncates text that does not fit in the caption rectangle 
	//							and adds ellipses.  
	inline void SetTextFormat(UINT nTextFormat) { m_nTextFormat=nTextFormat; }

	// --- In  :	nTextFormat	-	format used to draw text in caption
	// --- Out : 
	// --- Returns:	
	// --- Effect :	Sets format to display window's text. 
	inline UINT GetTextFormat() const { return m_nTextFormat; }


	// --- In  :	nNumberShade	-	the number of color shades
	// --- Out : 
	// --- Returns:	
	// --- Effect :	Sets the number of color shades used in gradient filling. 
	inline void SetNumberShade(UINT nNumberShade) { 
		ASSERT(nNumberShade>0);
		if(nNumberShade>0)
			m_nNumberShade=nNumberShade; 
	}
	
	// --- In  :	
	// --- Out : 
	// --- Returns:	The number of color shades used in gradient filling. 
	// --- Effect :	
	inline UINT GetNumberShade() const { return m_nNumberShade; }

    // assignment operator
    COXCaptionInfo& operator=(const COXCaptionInfo& ci);
};

////////////////////////



/////////////////////////////////////////////////////////////////////////////
// Generic caption painter. Derived from COXHookWnd - generic class that is 
// used to subclass any CWnd and provides capability to handle any message 
// routed to the hooked window before it. Handles WM_NCPAINT, WM_NCACTIVATE, 
// and whole bunch of other messages to handle drawing custom captions. 
// To use it:
//
//	1)	call Attach from your window's OnCreate function. 
//
//	2)	to set caption properties create COXCaptionInfo object or get
//		associated one using GetCaptionInfo() function. COXCaptionPainter
//		uses different COXCaptionInfo object for active and inactive state.
//
//	3)	change any COXCaptionInfo properties you want to.
//
//	4)	set updated COXCaptionInfo object to COXCaptionPainter object using
//		SetCaptionInfo() function.
//
//	5)	invalidate window's caption using SendMessage(WM_NCPAINT) in order to 
//		redraw it
//
//
// Additional functionality:
//
//	1)	you can load and/or save caption info in the registry using functions
//		SaveState and LoadState
//
//	2)	you can get any COXCaptionPainter object associated with any window
//		calling GetCaptionPainter() function and copy all properties from
//		any COXCaptionPainter object to the COXCaptionPainter associated with 
//		any window using SetCaptionPainter(). These function are especially 
//		useful when you want to set the same COXCaptionPainter properties  
//		for a set of windows (e.g. if all properties of parent window should be
//		copied to all siblings)
//
class OX_CLASS_DECL COXCaptionPainter : public COXHookWnd 
{
protected:
	DECLARE_DYNAMIC(COXCaptionPainter);

	// Size of caption rectangle. We always keep this information as flag
	// that indicate that we have to rebuild bitmaps for caption.
	CSize m_sizeCaption;

	// Bitmap for active captions 
	CBitmap m_bmActive;	
	// Bitmap for inactive captions 
	CBitmap m_bmInactive;	
	// Active/inactive state
	BOOL m_bActive;		
	// The problem is that while minimizing, maximizing, tiling, cascading or
	// restoring the size of window Windows by default applies special animation
	// effects using standard colors. And if you set your own background colors
	// it could be rather annoying to see animation with different color.
	// By default we turn off animation to eliminate this problem. Of course you
	// can turn off this functionality using SetAnimation() function. Next property
	// is set to TRUE if animation will be displayed, or FALSE otherwise
	// Window minimize/maximize animation
	BOOL m_bAnimation;
	
	// We keep two COXCaptionInfo objects for two caption state: active and inactive
	COXCaptionInfo m_ciActive;
	COXCaptionInfo m_ciInactive;

	// Windows is really inconsistent in its way of drawing caption. To eliminate 
	// some of undesirable Windows drawings we use some hacks: we change style
	// (or extended style) of hooked window for a period of standard Windows handling
	// of some messages and then set it again. m_bHackStyleSet property is set to 
	// TRUE if any hack was applied to hooked window and we save original window style
	// in m_dwSavedStyle. The same consideration applied to m_bHackStyleExSet and
	// m_dwSavedStyleEx properties (extended style)
	BOOL m_bHackStyleSet;
	LONG m_dwSavedStyle;
	BOOL m_bHackStyleExSet;
	LONG m_dwSavedStyleEx;
	BOOL m_bUpdate;
	// defines a number of bits used to represent colors in current video mode
	UINT m_nColorBits;

	// last used window text
	CString m_sWindowText;

	// flags that defines the actions that should be taken when 
	// the hooked window is in idle state
	enum IdleFlags	{ oxidleRedrawCaption=1 };
	int m_nIdleFlags;


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

	virtual void OnNcPaint(HRGN hUpdateRgn);
	virtual BOOL OnNcActivate(BOOL bActive);
	virtual void OnSetText(LPCTSTR lpText);
	virtual LRESULT OnNCLButtonDown(WPARAM wp, LPARAM lp);

	// Windows is really inconsistent in its way of drawing caption. To eliminate 
	// some of undesirable Windows drawings we use some hacks: we change style
	// (or extended style) of hooked window for a period of standard Windows handling
	// of some messages and then set it again. Next functions are especially designed 
	// to provide such functionality. 
	//
	// --- In  :	dwStyle	-	any combination of window's styles 
	//				nAction	-	if equals to ID_OXHACK_ADDSTYLE then
	//							dwStyle will be set to hooked window,
	//							if equals to ID_OXHACK_REMOVESTYLE then
	//							dwStyle will be removed from hooked window,
	// --- Out : 
	// --- Returns:	
	// --- Effect :	Change style of hooked window to force Windows to handle
	//				some messages the way we like. 
	void SetHackStyle(LONG dwStyle, UINT nAction=ID_OXHACK_REMOVESTYLE);

	// --- In  :	
	// --- Out : 
	// --- Returns:	
	// --- Effect :	Set original style to hooked window. 
	void RemoveHackStyle();

	
	// --- In  :	dwStyleEx	-	any combination of window's extended styles 
	//				nAction		-	if equals to ID_OXHACK_ADDSTYLE then
	//								dwStyleEx will be set to hooked window,
	//								if equals to ID_OXHACK_REMOVESTYLE then
	//								dwStyleEx will be removed from hooked window,
	// --- Out : 
	// --- Returns:	
	// --- Effect :	Change extended style of hooked window to force Windows to handle
	//				some messages the way we like. 
	void SetHackStyleEx(LONG dwStyleEx, UINT nAction=ID_OXHACK_REMOVESTYLE);

	// --- In  :	
	// --- Out : 
	// --- Returns:	
	// --- Effect :	Set original extended style to hooked window. 
	void RemoveHackStyleEx();

	// --- In  :	msg	-	message ID
	//				wp	-	WPARAM
	//				lp	-	LPARAM
	// --- Out : 
	// --- Returns:	result of message handling. Different for different messages.
	// --- Effect :	The problem is that while minimizing, maximizing, tiling, 
	//				cascading or restoring the size of window Windows by default 
	//				applies special animation effects using standard colors. 
	//				And if you set your own background colors it could be rather 
	//				annoying to see animation with different color. By default we 
	//				turn off animation to eliminate this problem. Of course you
	//				can turn off this functionality using SetAnimation() function. 
	//				If animation wasn't turned of then this function juct handle
	//				the message, otherwise before handling the message animation 
	//				flag will be turned off and after handling set to its 
	//				previous condition.
	LRESULT HackAnimation(UINT msg, WPARAM wp, LPARAM lp);


	// --- In  :	
	// --- Out : 
	// --- Returns:	
	// --- Effect :	Draws caption. 
	virtual void DrawCaption();


	// Helper function
	// --- In  :	rect	-	rectangle to copy coordinates of caption rectangle
	// --- Out : 
	// --- Returns:	
	// --- Effect :	Calculates caption rectangle. 
	void GetCaptionRect(CRect& rect);


	// Helper function
	// --- In  :	
	// --- Out : 
	// --- Returns:	number of bits used to represent colors in current video mode.
	// --- Effect :	
	UINT GetNumColorBits();

	inline void DelayDrawCaption() { m_nIdleFlags|=oxidleRedrawCaption; }


public:
	// Registered messages to set/get COXCaptionPainter object. Used in 
	// SetCaptionPainter() and GetCaptionPainter() functions. You can 
	// use them independently sending a message to hooked window:
	// WPARAM is not used, LPARAM is set to pointer to COXCaptionPainter
	// object. E.g. if you want to set the same COXCaptionPainter properties
	// for all child windows you can call next function:
	//
	//		SendMessageToDescendants(COXCaptionPainter::m_nSetCaptionPainter,0,
	//								 (LPARAM)&m_Caption);
	//
	static UINT m_nSetCaptionPainter;
	static UINT m_nGetCaptionPainter;

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXCaptionPainter();

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destroys the object
	virtual ~COXCaptionPainter();


	// --- In  :	pWnd	-	pointer to created window to be hooked
	// --- Out : 
	// --- Returns:	TRUE if pWnd was successfully hooked, otherwise FALSE
	// --- Effect : Hooked window
	BOOL Attach(CWnd* pWnd);

	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Unhooked attached window if any was hooked
	void Detach();


	// --- In  :	bActive	-	TRUE if you want to get COXCaptionInfo object
	//							for active state or FALSE - for inactive state.
	// --- Out : 
	// --- Returns:	pointer to corresponding COXCaptionInfo object
	// --- Effect : 
	inline COXCaptionInfo* GetCaptionInfo(BOOL bActive) { 
		return bActive ? &m_ciActive : &m_ciInactive; 
	}

	// --- In  :	pCI		-	pointer to COXCaptionInfo object
	//				bActive	-	TRUE if you want to set COXCaptionInfo object
	//							for active state or FALSE - for inactive state.
	// --- Out : 
	// --- Returns:	
	// --- Effect : Sets caption properties 
	void SetCaptionInfo(COXCaptionInfo* pCI, BOOL bActive);

	
	// --- In  :	pWnd	-	pointer to hooked window
	//				pCP		-	pointer to COXCaptionPainter object where 
	//							information will be saved
	// --- Out : 
	// --- Returns:	TRUE if succeeds, or FALSE otherwise.
	// --- Effect : Retrieves COXCaptionPainter object associated with pWnd
	static BOOL GetCaptionPainter(CWnd* pWnd, COXCaptionPainter* pCP);

	// --- In  :	pWnd	-	pointer to hooked window.
	//				pCP		-	pointer to COXCaptionPainter object
	// --- Out : 
	// --- Returns:	TRUE if succeeds, or FALSE otherwise.
	// --- Effect : Copy all properties from pCP to COXCaptionPainter object 
	//				associated with pWnd
	static BOOL SetCaptionPainter(CWnd* pWnd, COXCaptionPainter* pCP);


	// --- In  :	pDC			-	pointer to device context to draw in
	//				pCI			-	pointer to COXCaptionInfo object with all
	//								caption's properties.
	//				pCaptionRect-	pointer to caption's rect to draw in
	// --- Out : 
	// --- Returns:	
	// --- Effect : Draws caption background using caption's properties. 
	//				Declared as virtual so you can provide your own drawing routine.
	virtual void DrawCaptionBackground(CDC* pDC, const COXCaptionInfo* pCI, 
									   const CRect* pCaptionRect);

	// --- In  :	pDC			-	pointer to device context to draw in
	//				pCI			-	pointer to COXCaptionInfo object with all
	//								caption's properties.
	//				pCaptionSize-	pointer to caption's size
	// --- Out : 
	// --- Returns:	width of all drawn buttons with all gaps.
	// --- Effect : Draws window's standard buttons (minimize, maximize, close,
	//				restore, help). Declared as virtual so you can provide your 
	//				own drawing routine.
	virtual int DrawCaptionButtons(CDC* pDC, const COXCaptionInfo* pCI, 
								   const CSize* pCaptionSize);

	// --- In  :	pDC			-	pointer to device context to draw in
	//				pCI			-	pointer to COXCaptionInfo object with all
	//								caption's properties.
	//				pCaptionSize-	pointer to caption's size
	// --- Out : 
	// --- Returns:	width of drawn icon with all gaps.
	// --- Effect : Draws window's icon. Declared as virtual so you can provide 
	//				your own drawing routine.
	virtual int DrawCaptionIcon(CDC* pDC, const COXCaptionInfo* pCI, 
								const CSize* pCaptionSize);
	
	// --- In  :	pDC			-	pointer to device context to draw in
	//				pCI			-	pointer to COXCaptionInfo object with all
	//								caption's properties.
	//				pCaptionSize-	pointer to caption's size
	//				pIndentsSize-	pointer to CSize object that represents
	//								indents from caption left/right sides of 
	//								caption to draw window's text in where
	//								cx - indent from the left side of caption
	//								cy - indent from the right side of caption
	// --- Out : 
	// --- Returns:	
	// --- Effect : Draws window's text. Declared as virtual so you can provide 
	//				your own drawing routine.
	virtual void DrawCaptionText(CDC* pDC, const COXCaptionInfo* pCI, 
								 const CSize* pCaptionSize, 
								 const CSize* pIndentsSize);

	
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Resets size of caption that will eventually cause 
	//				rebuilding of bitmaps for active/inactive caption
	inline void Reset() { m_sizeCaption.cx=0; m_sizeCaption.cy=0; }


	// --- In  :	bAnimation	-	if equals FALSE then we remove animation
	//								effects that Windows used while minimizing, 
	//								maximizing, tiling, cascading or restoring 
	//								the size of a window; if equals TRUE then
	//								we won't try to remove it. By default, it is
	//								set to FALSE.
	// --- Out : 
	// --- Returns:
	// --- Effect : 
	inline void SetAnimation(BOOL bAnimation) { m_bAnimation=bAnimation; }

	// --- In  :	bAnimation	-	if equals TRUE then we remove animation
	//								effects that Windows used while minimizing, 
	//								maximizing, tiling, cascading or restoring 
	//								the size of a window; if equals FALSE then
	//								we won't try to remove it.
	// --- Out : 
	// --- Returns:
	// --- Effect : 
	inline BOOL GetAnimation() { return m_bAnimation; }

#ifndef OXCP_NO_SAVESTATE
	// Save to and load from registry (or *.ini file) state of caption
	// These functions require using COXRegistryValFile class. If you 
	// are not going to use them then you can include in your stdafx.h 
	// file next code:
	//	
	//		#define OXCP_NO_SAVESTATE
	//
	// In this case you don't need to include into your project references 
	// to source code for COXRegistryValFile class.
	//
	// --- In  :	lpszProfileName	-	name .ini file or hive in registry where
	//									all information about COXCaptionPainter
	//									will be saved. 
	// --- Out : 
	// --- Returns:	TRUE if succeeds, or FALSE otherwise.
	// --- Effect : Saves COXCaptionPainter state into registry or .ini file
	BOOL SaveState(LPCTSTR lpszProfileName);

	// --- In  :	lpszProfileName	-	name of .ini file or hive in registry where
	//									all information about COXCaptionPainter
	//									was saved. 
	//				bApply			-	if equals TRUE then COXCaptionPainter will be
	//									to redraw caption.
	//									
	// --- Out : 
	// --- Returns:	TRUE if succeeds, or FALSE otherwise.
	// --- Effect : Loads COXCaptionPainter state from registry or *.ini file
	BOOL LoadState(LPCTSTR lpszProfileName, BOOL bApply=TRUE);
#endif // OXCP_NO_SAVESTATE

private:
	static DWORD GetDllVersion(LPCTSTR lpszDllName);
	BOOL IsAppSkinned();
};

///////////////////////////////////////////////////////////////

class OX_CLASS_DECL COXCaptionPainterOrganizer
{
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXCaptionPainterOrganizer() : 
		m_pfnOldCBTHookProc(NULL), 
		m_dwThreadID(NULL),
		m_pfnOldGetMessageHookProc(NULL) {};


	// --- In  :	dwThreadID	-	thread ID which all windows will be attached
	//								to caption painter objects. By default we
	//								use the current process thread
	// --- Out : 
	// --- Returns:	TRUE if all windows created by the specified thread were 
	//				successfully hooked, otherwise FALSE
	// --- Effect : Creates COXCaptionPainter objects for all windows created 
	//				by the specified thread. The windows 
	//				that will be created by the thread in the future will be 
	//				attached as soon as they will be created. Whenever a window
	//				is attached it uses the caption settings of the 
	//				mainframe window
	BOOL AttachAllInThread(DWORD dwThreadID=::GetCurrentThreadId());


	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if all windows were successfully unhooked, 
	//				otherwise FALSE
	// --- Effect : Detaches all previously attached windows that has been 
	//				created by the thread specified in AttachAllInThread()
	//				function that was called before
	BOOL DetachAllInThread(BOOL bRedraw=TRUE);


	// --- In  :	pWnd	-	pointer to created window
	// --- Out : 
	// --- Returns:	pointer to created COXCaptionPainter object if pWnd was 
	//				successfully hooked, otherwise NULL
	// --- Effect : Creates COXCaptionPainter object for specified window 
	//				and attaches the specified window to it. Refer to the 
	//				documentation on COXCaptionPainter::Attach() for 
	//				additional info.
	//
	COXCaptionPainter* Attach(CWnd* pWnd);


	// --- In  :	pWnd	-	pointer to the attached window or NULL. 
	//							If NULL is specified then all attached
	//							windows will be unhooked.
	//				bRedraw	-	if TRUE then the soon to be detached window(s) 
	//							will be redrawn
	// --- Out : 
	// --- Returns:	TRUE if function succeeded, or FALSE otherwise
	// --- Effect : Unhooks specified or all attached window(s).
	BOOL Detach(const CWnd* pWnd=NULL, BOOL bRedraw=TRUE);


	// --- In  :	pWnd	-	pointer to the attached window or NULL. 
	//							If NULL is specified then all attached
	//							windows will be set to use specified 
	//							caption info object.
	//				pCI		-	pointer to COXCaptionInfo object
	//				bActive	-	TRUE if you want to set COXCaptionInfo object
	//							for active state or FALSE - for inactive state.
	//				bRedraw	-	if TRUE then the attached window 
	//							will be redrawn
	// --- Out : 
	// --- Returns:	
	// --- Effect : Sets caption properties 
	BOOL SetCaptionInfo(const CWnd* pWnd, COXCaptionInfo* pCI, 
						BOOL bActive, BOOL bRedraw=TRUE) const;


	// --- In  :	pWnd	-	pointer to the attached window or NULL. 
	//							If NULL is specified then all attached
	//							windows will be reset.
	//				bRedraw	-	if TRUE then the attached window 
	//							will be redrawn
	// --- Out : 
	// --- Returns:
	// --- Effect : Resets size of caption that will eventually cause 
	//				rebuilding of bitmaps for active/inactive caption
	BOOL Reset(CWnd* pWnd, BOOL bRedraw=TRUE) const;


	// --- In  :	pWnd	-	pointer to the attached window
	// --- Out : 
	// --- Returns:	Pointer to COXCaptionPainter object that corresponds 
	//				to the specified attached window if succeed or NULL
	//				otherwise
	// --- Effect : Retrieves the pointer to COXCaptionPainter object that
	//				corresponds to the specified attached window.
	COXCaptionPainter* GetPainter(const CWnd* pWnd) const;


	// --- In  :	pWnd	-	pointer to the window
	// --- Out : 
	// --- Returns:	TRUE if specified window was hooked before using 
	//				Attach function or FALSE otherwise
	// --- Effect : Retrieves the flag that specifies whether the given 
	//				window was attached before or not.
	BOOL IsAttached(const CWnd* pWnd) const;


#ifndef OXCP_NO_SAVESTATE
	// Save to and load from registry (or *.ini file) state of caption
	// These functions require using COXRegistryValFile class. If you 
	// are not going to use them then you can include in your stdafx.h 
	// file next code:
	//	
	//		#define OXCP_NO_SAVESTATE
	//
	// --- In  :	pWnd			-	pointer to the attached window or NULL. 
	//									If NULL is specified then all attached
	//									windows will be reset.
	//				lpszProfileName	-	name of .ini file or hive in registry where
	//									all information about COXCaptionPainter
	//									was saved. 
	//				bApply			-	if equals TRUE then COXCaptionPainter will be
	//									to redraw caption.
	//									
	// --- Out : 
	// --- Returns:	TRUE if succeeds, or FALSE otherwise.
	// --- Effect : Loads COXCaptionPainter state from registry or *.ini file
	BOOL LoadState(const CWnd* pWnd, LPCTSTR lpszProfileName, BOOL bApply=TRUE);
#endif // OXCP_NO_SAVESTATE

	inline BOOL IsAttachedAllInThread() const { return (m_dwThreadID!=NULL); }
	inline DWORD GetAttachedThread() const { return m_dwThreadID; }
	inline HHOOK GetSavedGetMessageHookProc() const { 
		return m_pfnOldGetMessageHookProc; 
	}
	inline HHOOK GetSavedCBTHookProc() const { return m_pfnOldCBTHookProc; }
	static HHOOK GetOriginalCBTHookProc() {
		return COXCaptionPainterOrganizer::m_pfnOriginalCBTHookProc; 
	}
	static HHOOK GetOriginalGetMessageHookProc() {
		return COXCaptionPainterOrganizer::m_pfnOriginalGetMessageHookProc; 
	}

	inline COXCaptionPainter* GetFirstPainter(POSITION& pos) const {
		pos=m_arrAttachedWnd.GetStartPosition();
		HWND hWnd=NULL;
		COXCaptionPainter* pPainter=NULL;
		while(pos!=NULL)
		{
			m_arrAttachedWnd.GetNextAssoc(pos,hWnd,pPainter);
			if(::IsWindow(hWnd))
			{
				ASSERT(pPainter!=NULL);
				break;
			}
			else
			{
				pPainter=NULL;
			}
		}
		return pPainter;
	}
	inline COXCaptionPainter* GetNextPainter(POSITION& pos) const {
		if(pos==NULL)
			return NULL;
		HWND hWnd=NULL;
		COXCaptionPainter* pPainter=NULL;
		while(pos!=NULL)
		{
			m_arrAttachedWnd.GetNextAssoc(pos,hWnd,pPainter);
			if(::IsWindow(hWnd))
			{
				ASSERT(pPainter!=NULL);
				break;
			}
			else
			{
				pPainter=NULL;
			}
		}
		return pPainter;
	}

	void AttachAllWindows(HWND hWndStartFrom);

public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXCaptionPainterOrganizer(); 

public:
	// map of all organizers that has been created to handle caption painting
	// for all windows created by particular thread
	static CMap<DWORD,DWORD,COXCaptionPainterOrganizer*,
		COXCaptionPainterOrganizer*> m_arrThreadOrganizers;

protected:
	// map of all attached windows and associated COXCaptionPainter objects
	CMap<HWND,HWND,COXCaptionPainter*,COXCaptionPainter*> m_arrAttachedWnd;
	// array of all used COXCaptionPainter objects
	CArray<COXCaptionPainter*,COXCaptionPainter*> m_arrUsedPainters;

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
	static LRESULT CALLBACK CaptionPainterCBTHookProc(int nCode, 
		WPARAM wParam, LPARAM lParam);

	// hook procedure for CBT
	static LRESULT CALLBACK CaptionPainterGetMessageHookProc(int nCode, 
		WPARAM wParam, LPARAM lParam);

	// callback for enumerating topmost windows
	static BOOL CALLBACK EnumThreadWindows(HWND hWnd, LPARAM lParam);
};

#endif // _CAPTIONPAINTER_H

