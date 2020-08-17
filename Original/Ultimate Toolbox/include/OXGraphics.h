// ==========================================================================
// 					Class Specification : COXGraphics
// ==========================================================================

// Header file : OXGraphics.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                          
// //////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////

/*

DESCRIPTION

COXGraphics class was designed in order to provide advanced graphics routines 
for programmers. We should note from the very beginning that we are not going 
to explain the underlying theory that has been used while creating this class.
For those who are interested in depth information about graphics algorithms
we would recommend toread the following book:

  "Computer Graphics, principle and practice" by	James D. Foley,
													Andries van Dam
													Steven K. Feiner
													John F. Hughes

  Addison-Wesley publishing company. ISBN 0-201-84840-6


This class will be constantly updated with new cool graphics routines. At this 
point it provides the following functionality:


1)	Drawing rounded rectangles. Rectangle is filled with given color. 
	The color of every point is calculated depending on the location of 
	light source (vertical and horizontal angles) and surface
	characteristics (light intensity, spot size, etc.). This way we 
	can achieve 3D effect. 

	In order to draw 3D rounded rectangle you should call the following function:

	DrawRoundedButton()
	
	Refer to the documentation on this function which arguments reveal all 
	settings that influence the ultimate appearance of the drawn rectangle.

	The functionality provided by this function is used in the following
	Ultimate Toolbox class: COXRoundedButton. Refer to this class as to 
	the real-life example of using of this function. Also you might want 
	to take look at the RoundedButton sample that can be found in the
	.\Samples\gui\RoundedButton subdirectory of your Ultimate Toolbox 
	directory.




Dependency:
	#include "OXGraphics.h"


Source code files:
	"OXGraphics.cpp"


*/

#if !defined(_OXGRAPHICS_H__)
#define _OXGRAPHICS_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


class OX_CLASS_DECL COXGraphics  
{
public:
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Constructs the object
	COXGraphics();

	
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : Destructs the object
	virtual ~COXGraphics();


protected:
	
	// Functions to draw rounded button
	//
	
	// Calculates colors on sphere with light source
	DWORD* PrepareSphere(CDC *pDC,		// dc where button will be drawn
		int nLeft, int nTop,			// coordinates of the top/left corner of the button
		int nExternalRadius,			// roundness of the button
		int nInternalRadius,			// part of sphere that will be painted using flat sphere color
		COLORREF clrLightSource,		// color of light source
		float Lx, float Ly, float Lz,	// normalized vector of the normal to the sphere
		float fLightIntensity,			// power of the light source
		int nPhong,						// influence on spot size
		float fMirror,					// influence on spot intensity
		float fDiffuse,					// diffuse lighting
		float fAmbient,					// ambient lighting
		COLORREF clrButton,				// color for filing the bitmap
		COLORREF clrBack				// color to fill the space that is not covered by the button
										// if CLR_NONE is specified then the background will be covered 
										// using background of the specified devicce context
		);

	// Helper function. Draws specified bitmap in the specified position. 
	// If pPalette is not NULL then it will be populated with palette
	// that was used to draw the bitmap (only for 256-color video mode)
	void DrawBitmap(CDC *pDC, int nLeft, int nTop, int width, int height, 
		DWORD *bitmap, CPalette* pPalette);

	// Helper function. Builds palette for specified bitmap 
	BOOL BuildButtonPalette(CDC *pDC, DWORD *bitmap, int width, 
		int height, CPalette* pPalette);

	// Create bitmap for rounded button on the base of the specified sphere bitmap 
	// that was built using PrepareSphere() function
	DWORD* PrepareButton(int nButtonWidth, int nButtonHeight, 
		DWORD *SphereBitmap, int SphereRadius);

	//
	///////////////////////////////////////////////////////////////

public:
	// --- In  :	pDC				-	device context to draw button in
	//				nLeft			-	x coordinate of top/left corner of the button
	//				nTop			-	y coordinate of top/left corner of the button
	//				nButtonWidth	-	button width
	//				nButtonHeight	-	button height
	//				nExternalRadius	-	radius of flat center and rounded border
	//				nInternalRadius	-	radius of flat part
	//				pPalette		-	pointer to palette object to be populated with 
	//									entries used to draw the button when 256-color 
	//									video mode is set on
	//				clrButton		-	solid color of button without lighting
	//				clrLightSource	-	color of light source
	//				fThetta			-	vertical angle of the light source
	//				fPhi			-	horizontal angle of the light source
	//				fLightIntensity	-	influence on diffuse lighting and spot intensity
	//				nPhong			-	influence on spot size
	//				fMirror			-	influence on spot intensity
	//				fDiffuse		-	diffuse lighting
	//				fAmbient		-	ambient lighting
	//				clrBack			-	color to fill the space that is not covered 
	//									by the button. If CLR_NONE color is specified 
	//									then the background will be covered using 
	//									background of the specified device context
	//				
	// --- Out : 
	// --- Returns:
	// --- Effect : Draws rounded button. The button background color will be 
	//				adjusted in order to accommodate light source, which properties
	//				can be set using parameters of the function. If current video
	//				mode is set to display only 256 colors the palette that has 
	//				been used in order to draw the button will be saved in pPalette 
	//				parameter
	void DrawRoundedButton(CDC* pDC, int nLeft, int nTop, int nButtonWidth,
		int nButtonHeight, int nExternalRadius=8, int nInternalRadius=3,
		CPalette* pPalette=NULL,
		COLORREF clrButton=::GetSysColor(COLOR_BTNFACE),
		COLORREF clrLightSource=::GetSysColor(COLOR_3DHIGHLIGHT),
		float fThetta=60, float fPhi=-45,
		float fLightIntensity=1.3f,	int nPhong=15, float fMirror=0.6f,
		float fDiffuse=0.5f, float fAmbient=0.9f,
		COLORREF clrBack=CLR_NONE);
};

#endif // !defined(_OXGRAPHICS_H__)
