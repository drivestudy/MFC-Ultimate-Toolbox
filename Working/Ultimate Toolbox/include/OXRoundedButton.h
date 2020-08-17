// ==========================================================================
// 					Class Specification : COXRoundedButton
// ==========================================================================

// Header file : OXRoundedButton.h

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

COXRoundedButton is COXBitmapButton derived class that makes use of the 

	DrawRoundedButton()

function found in COXGraphics class. This function allows us to draw 
3D rounded rectangles. We use it in order to draw the button's background.

As long as COXRoundedButton is derived from COXBitmapButton class, all 
functionality found in COXBitmapButton class applied to COXRoundedButton
as well (custom text font and color, images, including 256> colors, 
tooltips and much more. Refer to the documentation on COXBitmapButton
class for details). 

Actually, COXRoundedButton class handles only the process of drawing of 
the background. You can specify the base color that should be used 
in order to fill the button rectangle and other settings that influence 
the appearance of the button using the following functions:


	void SetButtonColor();
	void SetLightColor();
	void SetSphereExternalRadius();
	void SetSphereInternalRadius();
	void SetLightIntensityCoef();
	void SetThetta();
	void SetPhi();
	void SetPhong();
	void SetMirrorCoef();
	void SetDiffuseCoef();
	void SetAmbientCoef();


No doubt, most of this function wouldn't tell you anything about the way
they change the appearance of the button's background. If you would like to 
understand the meaning of all settings you might refer to the following 
book:


  "Computer Graphics, principle and practice" by	James D. Foley,
													Andries van Dam
													Steven K. Feiner
													John F. Hughes

  Addison-Wesley publishing company. ISBN 0-201-84840-6

  chapter 16 "Illumination and Shading".


Most people wouldn't care about it so we came up with a set of default values
that would produce pretty good results for most colors. It means, that in order 
to use the class you only have to specify the button color using the following 
function:

	void SetButtonColor()

Other than that you should treat COXRoundedButton as COXBitmapButton object.

Although we have to note that if you are going to use too dark or too light 
colors you might would like to use different settings. In order to simplify the
process of finding of set of appropriate values we provide "Customize Rounded Button"
page in our RoundedButton sample that can be found in the 
.\Samples\gui\RoundedButton subdirectory of your Ultimate Toolbox directory.
On this page you can easily apply different combination of all settings.
This sample also demonstrate COXRoundedButton samples (refer to 
"Rounded Button samples" page).



Dependency:
	#include "OXRoundedButton.h"


Source code files:
	"OXRoundedButton.cpp"
	"OXBitmapButton.cpp"	-	base class
	"OXGraphics.cpp"		-	drawing routine


Resource includes:
	"OXBitmapButton.rc"		-	base class resource file

*/

#if !defined(_OXROUNDEDBUTTON_H__)
#define _OXROUNDEDBUTTON_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include "OXBitmapButton.h"

// OXRoundedButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COXRoundedButton window

class OX_CLASS_DECL COXRoundedButton : public COXBitmapButton
{
// Construction
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXRoundedButton();

// Attributes
public:

protected:
	// colors
	COLORREF m_clrButton;
	COLORREF m_clrLight;
	COLORREF m_clrBk;

	// figure apperance
	int m_nSphereExtRadius;
	int m_nSphereIntRadius;

	// light source intensity
	float m_fLightIntensityCoef;

	// light angles
	float m_fThetta;
	float m_fPhi;

	// surface parameters
	int m_nPhong;
	float m_fMirror;
	float m_fDiffuse;
	float m_fAmbient;


// Operations
public:
	// --- In  :	clrButton		-	button background color
	//				bRedraw			-	if TRUE then button will be redrawn right away
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets button background color
	void SetButtonColor(COLORREF clrButton, BOOL bRedraw=TRUE);

	// --- In  :	
	// --- Out : 
	// --- Returns: Button background color
	// --- Effect : Retrieves button background color
	inline COLORREF GetButtonColor() const { return m_clrButton; }


	// --- In  :	clrLight		-	light source color
	//				bRedraw			-	if TRUE then button will be redrawn right away
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets light source color
	void SetLightColor(COLORREF clrLight, BOOL bRedraw=TRUE);

	// --- In  :	
	// --- Out : 
	// --- Returns: Light source color
	// --- Effect : Retrieves light sourced color
	inline COLORREF GetLightColor() const { return m_clrLight; }


	// --- In  :	clrBk			-	button's background color
	//				bRedraw			-	if TRUE then button will be redrawn right away
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets the button's background color
	void SetBkColor(COLORREF clrBk, BOOL bRedraw=TRUE);

	// --- In  :	
	// --- Out : 
	// --- Returns: button's background color
	// --- Effect : Retrieves the button's background color
	inline COLORREF GetBkColor() const { return m_clrBk; }


	// --- In  :	nSphereExtRadius	-	radius of flat center and rounded border
	//										of the button
	//				bRedraw				-	if TRUE then button will be redrawn 
	//										right away
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets radius of flat center and rounded border of the button
	void SetSphereExternalRadius(int nSphereExtRadius, BOOL bRedraw=TRUE);

	// --- In  :	
	// --- Out : 
	// --- Returns: Radius of flat center and rounded border of the button
	// --- Effect : Retrieves radius of flat center and rounded border of the button
	inline int GetSphereExternalRadius() const { return m_nSphereExtRadius; }


	// --- In  :	nSphereIntRadius	-	radius of flat part	of the button
	//				bRedraw				-	if TRUE then button will be redrawn 
	//										right away
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets radius of flat part of the button
	void SetSphereInternalRadius(int nSphereIntRadius, BOOL bRedraw=TRUE);

	// --- In  :	
	// --- Out : 
	// --- Returns: Radius of flat part of the button
	// --- Effect : Retrieves radius of flat part of the button
	inline int GetSphereInternalRadius() const { return m_nSphereIntRadius; }


	// --- In  :	fLightIntensityCoef	-	influence on diffuse lighting and 
	//										spot intensity of the light source
	//				bRedraw				-	if TRUE then button will be redrawn 
	//										right away
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets the coefficient of influence on diffuse lighting and 
	//				spot intensity of the light source
	void SetLightIntensityCoef(float fLightIntensityCoef, BOOL bRedraw=TRUE);

	// --- In  :	
	// --- Out : 
	// --- Returns: Coefficient of influence on diffuse lighting and 
	//				spot intensity of the light source
	// --- Effect : Retrieves coefficient of influence on diffuse lighting and 
	//				spot intensity of the light source
	inline float GetLightIntensityCoef() const { return m_fLightIntensityCoef; }


	// --- In  :	fThetta	-	vertical angle of the light source
	//				bRedraw	-	if TRUE then button will be redrawn right away
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets the vertical angle of the light source
	void SetThetta(float fThetta, BOOL bRedraw=TRUE);

	// --- In  :	
	// --- Out : 
	// --- Returns: Vertical angle of the light source
	// --- Effect : Retrieves the vertical angle of the light source
	inline float GetThetta() const { return m_fThetta; }


	// --- In  :	fPhi	-	horizontal angle of the light source
	//				bRedraw	-	if TRUE then button will be redrawn right away
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets the horizontal angle of the light source
	void SetPhi(float fPhi, BOOL bRedraw=TRUE);

	// --- In  :	
	// --- Out : 
	// --- Returns: Horizontal angle of the light source
	// --- Effect : Retrieves the horizontal angle of the light source
	inline float GetPhi() const { return m_fPhi; }


	// --- In  :	nPhong	-	influence on spot size of the light source
	//				bRedraw	-	if TRUE then button will be redrawn right away
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets the coffecient of the influence on spot size 
	//				of the light source
	void SetPhong(int nPhong, BOOL bRedraw=TRUE);

	// --- In  :	
	// --- Out : 
	// --- Returns: Coffecient of the influence on spot size 
	//				of the light source
	// --- Effect : Retrieves the coffecient of the influence on spot size 
	//				of the light source
	inline int GetPhong() const { return m_nPhong; }


	// --- In  :	fMirror	-	influence on spot intensity of the light source
	//				bRedraw	-	if TRUE then button will be redrawn right away
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets the coffecient of the influence on spot intensity 
	//				of the light source
	void SetMirrorCoef(float fMirror, BOOL bRedraw=TRUE);

	// --- In  :	
	// --- Out : 
	// --- Returns: Coffecient of the influence on spot intensity
	//				of the light source
	// --- Effect : Retrieves the coffecient of the influence on spot intensity 
	//				of the light source
	inline float GetMirrorCoef() const { return m_fMirror; }


	// --- In  :	fDiffuse	-	diffuse lighting
	//				bRedraw		-	if TRUE then button will be redrawn right away
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets the coffecient of the diffuse lighting
	void SetDiffuseCoef(float fDiffuse, BOOL bRedraw=TRUE);

	// --- In  :	
	// --- Out : 
	// --- Returns: Coffecient of the diffuse lighting
	// --- Effect : Retrieves the coffecient of the diffuse lighting
	inline float GetDiffuseCoef() const { return m_fDiffuse; }


	// --- In  :	fAmbient	-	ambient lighting
	//				bRedraw		-	if TRUE then button will be redrawn right away
	// --- Out : 
	// --- Returns:
	// --- Effect : Sets the coffecient of the ambient lighting
	void SetAmbientCoef(float fAmbient, BOOL bRedraw=TRUE);

	// --- In  :	
	// --- Out : 
	// --- Returns: Coffecient of the ambient lighting
	// --- Effect : Retrieves the coffecient of the ambient lighting
	inline float GetAmbientCoef() const { return m_fAmbient; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXRoundedButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXRoundedButton();


	// --- In  :
	// --- Out : 
	// --- Returns : The size of button in pixels when it fits its entire contents
	// --- Effect : 
	virtual CSize GetFitButtonSize();

protected:
	// virtual function called every time the control needs to calculate 
	// the postion of its elements
	virtual void DistributeSpace(UINT nState, CRect itemRect, 
		CRect& buttonRect, CRect& imageRect, CRect& textRect);
	// virtual function called every time the control needs to draw
	// the button background
	virtual void DrawButton(CDC* pDC, UINT nState, CRect buttonRect);
	// virtual function called every time the control needs to draw
	// focus rectangle for the button
	virtual void DrawFocusRectangle(CDC* pDC, UINT nState, CRect buttonRect, 
		CRect bitmapRect);

	// new virtual function used to draw the button background in particular
	virtual void DrawButtonBackground(CDC* pDC, CRect buttonRect, 
		UINT nState, CPalette* pPalette=NULL);

	// calculates the parameters that define the shape of the button
	CSize CalcSphereRadius();

	// Generated message map functions
protected:
	virtual BOOL IsInClientArea(CPoint pt);
	//{{AFX_MSG(COXRoundedButton)
	afx_msg void OnLButtonDblClk( UINT nFlags, CPoint point );
	afx_msg void OnRButtonDblClk( UINT nFlags, CPoint point );
	afx_msg void OnMButtonDblClk( UINT nFlags, CPoint point );
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnRButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnMButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnRButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnMButtonUp( UINT nFlags, CPoint point );
	afx_msg LRESULT OnChangeSettings(WPARAM wParam, LPARAM lParam);
#if _MSC_VER >= 1400
	afx_msg LRESULT OnNcHitTest(CPoint point);
#else
	afx_msg UINT OnNcHitTest(CPoint point);
#endif
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_OXROUNDEDBUTTON_H__)
