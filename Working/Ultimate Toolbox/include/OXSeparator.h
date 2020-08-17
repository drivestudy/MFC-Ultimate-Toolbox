/////////////////////////////////////////////////////////////////////////////
// Class Specification : COXSeparator
/////////////////////////////////////////////////////////////////////////////
// OXSeparator.h : header file

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

// //////////////////////////////////////////////////////////////////////////

#ifndef _OXSEPARATOR_H__
#define _OXSEPARATOR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


/*

Separator is an example of the simple but very useful control. Separators 
placed in right position in the dialog or form view can greatly improve 
the way your applications look. 

Amazingly, there is no such control among common controls. So we decided 
to fill the gap. We introduce new class COXSeparator that was designed 
primarily to provide easy-to-use separator control.

COXSeparator is derived from CStatic class so you can use standard static 
control to create an object in Dialog Editor. Then you have to associate
CStatic object with the control using Class Wizard and replace the name 
of class CStatic with COXSeparator. 

If you specify any text while creating static control in Dialog Editor 
then it will be displayed in the separator control either. And it will
be placed on the base of static control styles that you specify (SS_LEFT,
SS_RIGHT,SS_CENTER). In order to display just plain separator you shouldn't
associate any text with static conrol in Dialog Editor or you should call
SetWindowText(_T("")) function while initializing controls. By the way, you
can use SetWindowText function to set any text to separator control at 
run-time.

Static control provides the functionality to display only horizontally
aligned text. We go further and provide the functionality to display
separator and associated text vertically. In order to set/remove vertical 
alignment you have to call the following function:


	// --- In  :	bVertOriented	-	if TRUE then separator and control's text 
	//									will be drawn vertically, otherwise -
	//									horizontally (used by default).
	//				bRedraw			-	if TRUE then control will be repainted 
	//									right away
	// --- Out : 
	// --- Returns:	TRUE if success, otherwise FALSE
	// --- Effect : Sets the control orientation (vertical or horizontal)
	void SetVertOriented(BOOL bVertOriented=TRUE, BOOL bRedraw=FALSE)

By default we use horizontal orientation. 

Unfortunately only true type fonts can be used to display text vertically and
usually default Windows font used in controls is not true type. Luckily you just
can use standard SetFont function in order to set any font to separator control.

We also provide customization functionality so you can specify the text color 
and Top/Left and Bottom/Right colors used to draw separator. Use following 
function in order to do that:


	// --- In  :	clrTopLeft		-	color to be used to draw the top/left
	//									side of the separator
	//				clrBottomRight	-	color to be used to draw the 
	//									bottom/right side of the separator
	//				bRedraw			-	if TRUE then control will be repainted 
	//									right away
	// --- Out : 
	// --- Returns:	TRUE if success, otherwise FALSE
	// --- Effect : Sets the separator color
	BOOL SetSeparatorColors(COLORREF clrTopLeft=::GetSysColor(COLOR_BTNSHADOW), 
							COLORREF clrBottomRight=::GetSysColor(COLOR_BTNHILIGHT), 
							BOOL bRedraw=FALSE) 


	// --- In  :	clrText	-	color to be used to draw the control's text
	//				bRedraw	-	if TRUE then control will be repainted 
	//							right away
	// --- Out : 
	// --- Returns:	TRUE if success, otherwise FALSE
	// --- Effect : Sets the text color
	BOOL SetTextColor(COLORREF clrText=::GetSysColor(COLOR_WINDOWTEXT), BOOL bRedraw=FALSE) 



Take at the "Separator" sample that demonstrate different separators.
It can be found in .\Samples\GUI\Separator subdirectory of your
Ultimate Toolbox directory.

*/

/////////////////////////////////////////////////////////////////////////////
// COXSeparator window

class OX_CLASS_DECL COXSeparator : public CStatic
{
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXSeparator();

// Attributes
public:

protected:
	// text color
	COLORREF m_clrText;
	// color of top/left line of the separator
	COLORREF m_clrTopLeft;
	// color of bottom/right line of the separator
	COLORREF m_clrBottomRight;
	// flag that specifies whether the separator should be drawn 
	// vertically (the control is horizontally oriented by default)
	BOOL m_bVertOriented;

private:
	// window's text
	CString m_sText;


// Operations
public:
	// --- In  :	clrText	-	color to be used to draw the control's text
	//				bRedraw	-	if TRUE then control will be repainted 
	//							right away
	// --- Out : 
	// --- Returns:	TRUE if success, otherwise FALSE
	// --- Effect : Sets the text color
	inline BOOL SetTextColor(COLORREF clrText=::GetSysColor(COLOR_WINDOWTEXT), 
		BOOL bRedraw=FALSE) 
	{ 
		if(bRedraw && m_clrText==clrText)
			bRedraw=FALSE;
		m_clrText=clrText;
		if(bRedraw)
			RedrawWindow();
		return TRUE;
	}

	// --- In  :	clrText	-	reference to COLORREF variable that will be set 
	//							to the color currently used to draw the 
	//							control's text
	// --- Out : 
	// --- Returns:	TRUE if success, otherwise FALSE
	// --- Effect : Retrieves the text color
	inline BOOL GetTextColor(COLORREF& clrText) const 
	{ 
		clrText=m_clrText; 
		return TRUE;
	}

	// --- In  :	clrTopLeft		-	color to be used to draw the top/left
	//									side of the separator
	//				clrBottomRight	-	color to be used to draw the 
	//									bottom/right side of the separator
	//				bRedraw			-	if TRUE then control will be repainted 
	//									right away
	// --- Out : 
	// --- Returns:	TRUE if success, otherwise FALSE
	// --- Effect : Sets the separator color
	inline BOOL SetSeparatorColors(COLORREF clrTopLeft=::GetSysColor(COLOR_BTNSHADOW), 
		COLORREF clrBottomRight=::GetSysColor(COLOR_BTNHILIGHT), BOOL bRedraw=FALSE) 
	{ 
		if(bRedraw && m_clrTopLeft==clrTopLeft && 
			m_clrBottomRight==clrBottomRight)
			bRedraw=FALSE;
		m_clrTopLeft=clrTopLeft; 
		m_clrBottomRight=clrBottomRight;
		if(bRedraw)
			RedrawWindow();
		return TRUE;
	}

	// --- In  :	clrTopLeft		-	reference to COLORREF variable that 
	//									will be set to the color currently 
	//									used to draw the top/left side of the 
	//									separator
	//				clrBottomRight	-	reference to COLORREF variable that 
	//									will be set to the color currently 
	//									used to draw the bottom/right side 
	//									of the separator
	// --- Out : 
	// --- Returns:	TRUE if success, otherwise FALSE
	// --- Effect : Retrieves the separator color
	inline BOOL GetSeparatorColors(COLORREF& clrTopLeft, COLORREF& clrBottomRight) const 
	{ 
		clrTopLeft=m_clrTopLeft; 
		clrBottomRight=m_clrBottomRight;
		return TRUE;
	}

	// --- In  :	bVertOriented	-	if TRUE then separator and control's text 
	//									will be drawn vertically, otherwise -
	//									horizontally (used by default).
	//				bRedraw			-	if TRUE then control will be repainted 
	//									right away
	// --- Out : 
	// --- Returns:	TRUE if success, otherwise FALSE
	// --- Effect : Sets the control orientation (vertical or horizontal)
	inline void SetVertOriented(BOOL bVertOriented=TRUE, BOOL bRedraw=FALSE)
	{
		if(bRedraw && m_bVertOriented==bVertOriented)
			bRedraw=FALSE;
		m_bVertOriented=bVertOriented;
		if(bRedraw)
			RedrawWindow();
	}

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if the text and separator will be drawn vertically, 
	//				FALSE if the text and separator will be drawn horizontally
	// --- Effect : Retrieves the flag that specifies whether the control 
	//				is vertically or horizontally oriented
	inline BOOL IsVertOriented() const
	{
		return m_bVertOriented;
	}


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXSeparator)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXSeparator();
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Object's destructor

	// Generated message map functions
protected:
	//{{AFX_MSG(COXSeparator)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg LONG OnSetText(UINT wParam, LONG lParam);
	afx_msg LONG OnGetText(UINT wParam, LONG lParam);
	afx_msg LONG OnGetTextLength(UINT wParam, LONG lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(_OXSEPARATOR_H__)
