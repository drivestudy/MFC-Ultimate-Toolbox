// ==========================================================================
// 							Class Specification : COXDIB
// ==========================================================================

// Header file : oxdib.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

                         
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CObject

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO  Needs a resource (template)

//	YES	Persistent objects (saveable on disk)      
//	YES	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This class represents a persistent Device Independent Bitmap (DIB)
//	The bitmap can be read from file and saved back to file
//	By using the Paint function you can display (a part of) it
//	on a DC

// Remark:
//	Copy and assignment operators are implemented as well.
//  Notice that in this case a GlobalAlloc is called to 
//	 actually vopy the bitmaps contents

// Prerequisites (necessary conditions):
//	

/////////////////////////////////////////////////////////////////////////////
#ifndef __DIB_H__
#define __DIB_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

// Handle to a DIB 
DECLARE_HANDLE(HDIB);
  
typedef     LPBITMAPINFOHEADER PDIB;

#if defined(WIN32) || defined(_WIN32)
    #define _huge
#endif

// DIB constants 
#define PALVERSION   0x300

// DIB Macros
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))

#ifdef  WIN32
    #define HandleFromDib(lpbi) GlobalHandle(lpbi)
#else
    #define HandleFromDib(lpbi) (HANDLE)GlobalHandle(SELECTOROF(lpbi))
#endif

#define DibFromHandle(h)        (PDIB)GlobalLock(h)

#define DibFree(pdib)           GlobalFreePtr(pdib)

// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits"
// parameter is the bit count for the scanline (biWidth * biBitCount),
// and this macro returns the number of DWORD-aligned bytes needed
// to hold those bits.
#ifndef WIDTHBYTES
#define WIDTHBYTES(bits)        ((unsigned)((bits+31)&(~31))/8)  /* ULONG aligned ! */
#endif

#define DibWidth(lpbi)          (UINT)(((LPBITMAPINFOHEADER)(lpbi))->biWidth)
#define DibHeight(lpbi)         (UINT)(((LPBITMAPINFOHEADER)(lpbi))->biHeight)
#define DibBitCount(lpbi)       (UINT)(((LPBITMAPINFOHEADER)(lpbi))->biBitCount)
#define DibCompression(lpbi)    (DWORD)(((LPBITMAPINFOHEADER)(lpbi))->biCompression)

#define DibWidthBytesN(lpbi, n) (UINT)WIDTHBYTES((UINT)(lpbi)->biWidth * (UINT)(n))
#define DibWidthBytes(lpbi)     DibWidthBytesN(lpbi, (lpbi)->biBitCount)

#define DibSizeImage(lpbi)      ((lpbi)->biSizeImage == 0 \
                                    ? ((DWORD)(UINT)DibWidthBytes(lpbi) * (DWORD)(UINT)(lpbi)->biHeight) \
                                    : (lpbi)->biSizeImage)

#define DibSize(lpbi)           ((lpbi)->biSize + (lpbi)->biSizeImage + (int)(lpbi)->biClrUsed * sizeof(RGBQUAD))
#define DibPaletteSize(lpbi)    (DibNumColors(lpbi) * sizeof(RGBQUAD))

#define DibFlipY(lpbi, y)       ((int)(lpbi)->biHeight-1-(y))

//HACK for NT BI_BITFIELDS DIBs
#ifdef WIN32
    #define DibPtr(lpbi)            ((lpbi)->biCompression == BI_BITFIELDS \
                                       ? (LPVOID)(DibColors(lpbi) + 3) \
                                       : (LPVOID)(DibColors(lpbi) + (UINT)(lpbi)->biClrUsed))
#else
    #define DibPtr(lpbi)            (LPVOID)(DibColors(lpbi) + (UINT)(lpbi)->biClrUsed)
#endif

#define DibColors(lpbi)         ((RGBQUAD FAR *)((LPBYTE)(lpbi) + (int)(lpbi)->biSize))

#define DibNumColors(lpbi)      ((lpbi)->biClrUsed == 0 && (lpbi)->biBitCount <= 8 \
                                    ? (int)(1 << (int)(lpbi)->biBitCount)          \
                                    : (int)(lpbi)->biClrUsed)

#define DibXYN(lpbi,pb,x,y,n)   (LPVOID)(                                     \
                                (BYTE _huge *)(pb) +                          \
                                (UINT)((UINT)(x) * (UINT)(n) / 8u) +          \
                                ((DWORD)DibWidthBytesN(lpbi,n) * (DWORD)(UINT)(y)))

#define DibXY(lpbi,x,y)         DibXYN(lpbi,DibPtr(lpbi),x,y,(lpbi)->biBitCount)

#define FixBitmapInfo(lpbi)     if ((lpbi)->biSizeImage == 0)                 \
                                    (lpbi)->biSizeImage = DibSizeImage(lpbi); \
                                if ((lpbi)->biClrUsed == 0)                   \
                                    (lpbi)->biClrUsed = DibNumColors(lpbi);   /*\
                                if ((lpbi)->biCompression == BI_BITFIELDS && (lpbi)->biClrUsed == 0) \
                                    (lpbi)->biClrUsed = 3;                    */

#define DibInfo(pDIB)     ((BITMAPINFO FAR *)(pDIB))

/***************************************************************************/

#ifndef BI_BITFIELDS
  #define BI_BITFIELDS 3
#endif 


// put the following line in your "stdafx.h" if you would like to use the 
// foolowing routines that provide the functionality to read JPEG images 
// and save DIB as JPEG:
//
//	BOOL ReadJPEG(LPCTSTR pszPath);
//	BOOL WriteJPEG(LPCTSTR pszPath);
//	static BOOL ConvertDIBtoJPEG(LPCTSTR pszDIBPath, LPCTSTR pszJPEGPath);
//	static BOOL ConvertJPEGtoDIB(LPCTSTR pszJPEGPath, LPCTSTR pszDIBPath);
// 
#ifdef OXDIB_SUPPORTJPEG
#include "oxbmpfle.h"
#include "oxjpgfle.h"
#include "oxjpgcom.h"
#include "oxjpgdom.h"
#include "oxjpgexp.h"
#endif 


class OX_CLASS_DECL COXDIB : public CObject
{
	DECLARE_SERIAL(COXDIB)

// Data members -------------------------------------------------------------
public:
	
//protected:
    HDIB		m_hDIB;
    CPalette*	m_pPalette;

#ifndef NO_DITHER
	// static coefficient for dithering conversion
	static PALETTEENTRY const aHalfTonePalette[256];

	static BYTE const aDividedBy51Rounded[256];
	static BYTE const aDividedBy51[256];
	static BYTE const aModulo51[256];
	static BYTE const aTimes6[6];
	static BYTE const aTimes36[6];
	static BYTE const aHalftone16x16[256];
	static BYTE const aHalftone8x8[64];
	static BYTE const aHalftone4x4_1[16];
	static BYTE const aHalftone4x4_2[16];
	static BYTE const aWinGHalftoneTranslation[216];

	static const COLORREF  NT_REF;
	//////////////////////////////////////////////////
#endif  // NO_DITHER

private:
	
// Member functions ---------------------------------------------------------
public:
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object
	//				It will initialize the internal state as empty
	//				This is a handy routine for clipboard support
	COXDIB();

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object out of a handle
	//				It will initialize the internal state as empty
	COXDIB(HDIB hDIB);

	// --- In :	DIBSrc : DIB object which will be copied
	// --- Out : 
	// --- Returns :
	// --- Effect : Copy contruction.
	//				Notice that global memory will be copied
    COXDIB(const COXDIB& DIBSrc);
    
	// --- In :	DIBSrc : DIB object which will be assign to 'this' DIB object
	// --- Out:
	// --- Returns: 
	// --- Effect : Assignment operator
	COXDIB& operator=(const COXDIB& DIBSrc);

	// --- In :	hDIBSrc : handle to DIB  which will be assign to 'this' DIB object
	// --- Out:
	// --- Returns: 
	// --- Effect : Assignment operator
	COXDIB& operator=(HDIB hDIBSrc);
	

	// --- In  :
	// --- Out : 
	// --- Returns : succeeded or not
	// --- Effect : Clear the System Palette so that we can ensure an identity palette 
	//				mapping for fast performance.
	static BOOL ClearSystemPalette();
 	
	// --- In  :
	// --- Out : pPalette : the current system palette
	// --- Returns : succeeded or not
	// --- Effect : Creates the System Palette
	static BOOL GetSystemPalette(CPalette* pPalette);

	// --- In      : lpbmi: Pointer to the DIB
	// --- Out     : nNumColors : Number of colors in palette
	// --- Returns : A palette created from the DIB bitmap
	// --- Effect  : 
	static HPALETTE CreateDIBPalette(LPBITMAPINFO lpbmi, int& nNumColors);

	// --- In  :	lpszResourceName	-	resource string
	//				nResourceID			-	resource ID
	//				hInstance			-	handle of instance from which to 
	//										load the resource
	// --- Out : 
	// --- Returns: TRUE if image was successfully loaded
	// --- Effect : Load DIB from resource
	inline BOOL LoadResource(UINT nResourceID, HINSTANCE hInstance=NULL) {
		return LoadResource(MAKEINTRESOURCE(nResourceID),hInstance);
	}
	BOOL LoadResource(LPCTSTR lpszResourceName, HINSTANCE hInstance=NULL);


	// --- In  :
	// --- Out : 
	// --- Returns : Handle to new global memory block.
	// --- Effect : Makes a copy of this DIB to a new global memory block.
	//				This is a handy routine for clipboard support
	HDIB WINAPI MakeCopy();
	
	// --- In  :  pDC : the device context from where the bitmap will be copied to
	//			  bmSize : A CSize
	// --- Out : bmSize : The size of the newly selected bitmap
	// --- Returns : Pointer to the old bitmap off the dc. This is a temporary
	//				 pointer and should not be stored for later use.
	// --- Effect : Replaces the dc's existing bitmap with a new one from the DIB
	//				and return the new size
	CBitmap* MakeBitmap(CDC* pDC, CSize& bmSize);
		

	// --- In  :
	// --- Out : 
	// --- Returns : Whether the objects contains a DIB or is still empty
	// --- Effect : 
	BOOL IsEmpty() const;
	
	// --- In  :
	// --- Out : 
	// --- Returns : 
	// --- Effect : Clear the contents of the DIB
	void Empty();
	

	// --- In  : pDC : DC to do output to
	//			 rDCRect : rectangle on DC to do output to
	//			 rDIBRect : rectangle of DIB to output into rDCRect
	// --- Out : 
	// --- Returns :TRUE if DIB was drawn, FALSE otherwise
	// --- Effect : Painting routine for a DIB.  Calls StretchDIBits() or
	//				SetDIBitsToDevice() to paint the DIB.  The DIB is
	//				output to the specified DC, at the coordinates given
	//				in rDCRect.  The area of the DIB to be output is
	//				given by rDIBRect.
	BOOL Paint(CDC* pDC, const CRect& rDCRect, const CRect& rDIBRect) ;
 

	// --- In  :  lpDIBHeader : pointer to the dib header whose bits we want to locate
	// --- Out : 
	// --- Returns : pointer to the DIB bits
	// --- Effect : This function calculates the address of the DIB's bits and returns a
	//				pointer to the DIB bits.
	//				To be of any use, the global memory must be locked
	LPSTR FindDIBBits (LPSTR lpDIBHeader = NULL);
	

	// --- In  :
	// --- Out : 
	// --- Returns : The size of the IDB
	// --- Effect : This function gets the width of the DIB from the BITMAPINFOHEADER
	//				width field if it is a Windows 3.0-style DIB or from the BITMAPCOREHEADER
	//				width field if it is an other-style DIB.
	CSize GetSize(LPSTR lpDIBHeader = NULL) const;
 
	
	// --- In  :
	// --- Out : 
	// --- Returns : The size of the color palette of the DIB
	// --- Effect : This function gets the size required to store the DIB's palette by
	//				multiplying the number of colors by the size of an RGBQUAD (for a
	//				Windows 3.0-style DIB) or by the size of an RGBTRIPLE (for an other-
	//				style DIB).
	WORD GetPaletteSize(LPSTR lpDIBHeader = NULL) const;

	// --- In  :
	// --- Out : 
	// --- Returns : The palette of the DIB
	// --- Effect : It is possible that the DIB does not have a palette,
	//				a NULL pointer will be returned in this case
	CPalette* GetPalette() const;	
	

	// --- In  :
	// --- Out : 
	// --- Returns : The number of colors in the color table
	// --- Effect : This function calculates the number of colors in the DIB's color table
	//				by finding the bits per pixel for the DIB (whether Win3.0 or other-style
	//				DIB). If bits per pixel is 1: colors=2, if 4: colors=16, if 8: colors=256,
	//				if 24, no colors in color table.
	WORD GetNumColors(LPSTR lpDIBHeader = NULL) const;


	// --- In  :
	// --- Out : 
	// --- Returns : The number of bits used to represent one pixel
	// --- Effect : 
	WORD GetNumBitsPerPixel(LPSTR lpDIBHeader = NULL) const;
 

	// --- In  : dwForeground : 
	//			 dwBackground
	// --- Out : 
	// --- Returns : 
	// --- Effect :
    void SetMonoColors(DWORD dwForeground, DWORD dwBackground, 
		LPSTR lpDIBHeader = NULL);  


    // --- In  : dwForeground : 
	//			 dwBackground
	// --- Out : 
	// --- Returns : whether the palette was monochrome or not
	// --- Effect :
    BOOL GetMonoColors(DWORD& dwForeground, DWORD& dwBackground, LPSTR lpDIBHeader = NULL); 
    

	// --- In  :  pDC : The device context with which characteristics the bitmap 
	//					will be made
	// --- Out : 
	// --- Returns : Pointer to the new bitmapc.
	// --- Effect : Creates out of the DIB a bitmap
	//				The bitmap is not selected into the dc
	CBitmap* MakeBitmapFromDIB(CDC* pDC);
	
	// --- In :	hBitmap : HANDLE to global memory containing a BMP spec
	//                     (either BITMAPINFOHEADER or BITMAPCOREHEADER)
	//						compatible with default screen display device.
	//			pPal : Palette to render the DIB with.  If it's NULL,
	//                     use the default palette.		
	// --- Out:
	// --- Returns: a DIB made of the bmp
	// --- Effect : Given a handle to global memory with a DIB spec in it,
	//             and a palette, returns a device dependent bitmap.  The
	//             The DDB will be rendered with the specified palette.
	HDIB BitmapToDIB(HBITMAP hBitmap, CPalette* pPal = NULL);
	
#ifdef _WIN32
	// --- In :	hIcon : HANDLE to icon
	//			sizeIcon : icon size
	//			pPal : Palette to render the DIB with.  If it's NULL,
	//                     use the default palette.		
	// --- Out:
	// --- Returns: a DIB made of the bmp
	// --- Effect : 
	HDIB IconToDIB(HICON hIcon, CSize sizeIcon, CPalette* pPal=NULL);
#endif
	// --- In :	wBitsPerPix : number of bits per pixel
	//			dwWidth : width of the new empty dib
	//			dwHeight : height of the new empty dib
	// --- Out:
	// --- Returns: succeeded or not
	// --- Effect : Creates a empty dib in this object with given size and bits per pixel
	//				Any previous dib contained in this object, is discarded.
	BOOL CreateEmptyDIB(WORD wBitsPerPix, DWORD dwWidth, DWORD dwHeight);
	

	// --- In :	pPal : the palette to adjust the DIB to
	//			wUsage : DIB_PAL_COLORS the DIB color table is set to 0-256
	//					 DIB_RGB_COLORS the DIB color table is set to the RGB values
	// --- Out:
	// --- Returns: succeeded or not
	// --- Effect : Modifies the color table of the DIB for use with the wUsage
	//				parameter specifed.
	BOOL SetPaletteUsage(CPalette* pPal, UINT wUsage);


	// --- In :	dwWidth : the new width for the DIB
	//			dwHeight : the new height for the DIB
	// --- Out:
	// --- Returns: succeeded or not
	// --- Effect : Replaces this DIB with its Resized counterpart
	//				Any previous dib contained in this object, is discarded.
	//				ONLY supported for 24 bit and 8 bit DIB's
	BOOL ResizeDIB(DWORD dwWidth, DWORD dwHeight);


	// --- In  :
	// --- Out : pPalette : the halftone palette
	// --- Returns : succeeded or not
	// --- Effect : Creates the halftone Palette
	static BOOL GetHalfTonePalette(CPalette* pPalette);

#ifndef NO_DITHER
	// --- In :	
	// --- Out:
	// --- Returns: succeeded or not
	// --- Effect : Replaces this DIB with its halftone counterpart
	//				Any previous dib contained in this object, is discarded.
	//				You can use this to change 24-bit-pixel DIBS into 8-bit-pixel
	//				DIBS and display them on a 256 color display.
	//				ONLY supported for 24 bit DIB's
	BOOL HalfToneDitherDIB();
#endif // NO_DITHER
	
	// --- In :	lpBmInfoHdr == Far pointer to a BITMAPINFOHEADER structure
	//                            to be filled in.
	//             dwWidth     == Width of DIB (not in Win 3.0 & 3.1, high
	//                            word MUST be 0).
	//             dwHeight    == Height of DIB (not in Win 3.0 & 3.1, high
	//                            word MUST be 0).
	//             nBPP        == Bits per Pixel for the DIB.		
	// --- Out:
	// --- Returns: 
	// --- Effect : Does a "standard" initialization of a BITMAPINFOHEADER,
	//             given the Width, Height, and Bits per Pixel for the
	//             DIB.
	//
	//             By standard, I mean that all the relevant fields are set
	//             to the specified values.  biSizeImage is computed, the
	//             biCompression field is set to "no compression," and all
	//             other fields are 0.
	//
	//             Note that DIBs only allow BitsPixel values of 1, 4, 8, or
	//             24.  This routine makes sure that one of these values is
	//             used (whichever is most appropriate for the specified
	//             nBPP).
	void InitBitmapInfoHeader (PDIB lpBmInfoHdr, DWORD dwWidth,
		DWORD dwHeight, WORD nBPP);

	// --- In :	ar : A CArchive object to serialize to or from
	// --- Out : 
	// --- Returns :
	// --- Effect : Inits load or save of the COXDIB
	virtual void Serialize(CArchive& ar);
	
	// --- In :		pszPath:	path specification of DIB file
	//				pDIBFile:	the open DIB file
	//				ar:			the archive used to read DIB
	// --- Out : 
	// --- Returns:	TRUE if successful, else FALSE or CFileException
	// --- Effect:	read DIB from the given file, archive or open CFile object 
	BOOL Read(LPCTSTR pszPath);
	BOOL Read(CFile* pDIBFile);
	BOOL Read(CArchive& ar);


	// --- In :		pszPath:	path specification of DIB file
	//				pDIBFile:	the open DIB file
	//				bCloseFile:	close the pDIBFile after it was written 
	//				ar:			the archive used to write DIB
	// --- Out : 
	// --- Returns:	TRUE if successful, else FALSE or CFileException
	// --- Effect:	write DIB in the given file, archive or open CFile object 
	BOOL Write(LPCTSTR pszPath);
	BOOL Write(CFile* pDIBFile, BOOL bCloseFile=TRUE);
	BOOL Write(CArchive& ar);


#ifdef OXDIB_SUPPORTJPEG
	// --- In :		pszPath:	path specification of JPEG file
	//				pFile:		pointer to CFile object that contains JPEG file
	// --- Out : 
	// --- Returns:	TRUE if successful, else FALSE or CFileException
	// --- Effect:	read JPEG from the given file and convert it to DIB
	BOOL ReadJPEG(LPCTSTR pszPath);
	BOOL ReadJPEG(CFile* pFile);

	// --- In :		pszPath:	path specification of JPEG file
	//				pFile:		pointer to CFile object that will contain JPEG file
	// --- Out : 
	// --- Returns:	TRUE if successful, else FALSE or CFileException
	// --- Effect:	write DIB in the given file in JPEG format
	BOOL WriteJPEG(LPCTSTR pszPath);
	BOOL WriteJPEG(CFile* pFile);

	// --- In :		pszDIBPath:		path specification of DIB file
	//				pszJPEGPath:	path specification of JPEG file
	//				pDIBFile:		pointer to the CFile object that contains DIB file
	//				pszJPEGPath:	pointer to the CFile object that contains JPEG file
	// --- Out : 
	// --- Returns:	TRUE if successful, else FALSE or CFileException
	// --- Effect:	convert specified DIB file to JPEG format and save it 
	//				under the specified name
	static BOOL ConvertDIBtoJPEG(LPCTSTR pszDIBPath, LPCTSTR pszJPEGPath);
	static BOOL ConvertDIBtoJPEG(COXBMPFile* pDIBFile, COXJPEGFile* pJPEGFile);

	// --- In :		pszJPEGPath:	path specification of DIB file
	//				pszDIBPath:		path specification of JPEG file
	//				pszJPEGPath:	pointer to the CFile object that contains JPEG file
	//				pDIBFile:		pointer to the CFile object that contains DIB file
	// --- Out : 
	// --- Returns:	TRUE if successful, else FALSE or CFileException
	// --- Effect:	convert specified DIB file to JPEG format and save it 
	//				under the specified name
	static BOOL ConvertJPEGtoDIB(LPCTSTR pszJPEGPath, LPCTSTR pszDIBPath);
	static BOOL ConvertJPEGtoDIB(COXJPEGFile* pJPEGFile, COXBMPFile* pDIBFile);
#endif

	// --- In :		nAngleDegree:	the angle of closckwise rotation of image 
	//								in degrees. The value must be divided by 90
	//								without remainder. The negative value might 
	//								be specified. In this case the image will be 
	//								rotated in counterclockwise direction
	//				bFlipVert:		if TRUE then after rotation the image will be
	//								flipped vertically
	//				bFlipHorz:		if TRUE then after rotation the image will be
	//								flipped horizontally.
	// --- Out : 
	// --- Returns:	TRUE if successful
	// --- Effect:	Rotates the image. Optionally, image can be flipped vertically
	//				and/or horizontally. in order to flip image without rotating
	//				it specify nAngleDegree parameter as 0.
	BOOL Rotate(int nAngleDegree, BOOL bFlipVert=FALSE, BOOL bFlipHorz=FALSE);

	// --- In :		
	// --- Out : 
	// --- Returns:	TRUE if successful
	// --- Effect:	Flips image vertically
	inline BOOL FlipVert() { return Rotate(0,TRUE,FALSE); }

	// --- In :		
	// --- Out : 
	// --- Returns:	TRUE if successful
	// --- Effect:	Flips image horizontally
	inline BOOL FlipHorz() { return Rotate(0,FALSE,TRUE); }


	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object
	virtual ~COXDIB();

// Diagnostics --------------------------------------------------------------
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:      
	BOOL InitPalette(LPSTR lpbi);
	DWORD ReadHuge(CArchive& ar, void FAR* lpBuffer, DWORD dwCount);
	void WriteHuge(CArchive& ar, const void FAR* lpBuffer, DWORD dwCount);
	UINT CalcSize(DWORD cbTotal, const void FAR* lpStart);
	HDIB WINAPI CopyHandle (HDIB hDIB, DWORD dwExtraBytes = 0);
	PDIB ReadBitmapInfo(CArchive& ar);
	DWORD CalcPadding(DWORD dwBitsPerPixel, DWORD dwPixels);

private:
	HDIB Detach();
                   
};

#endif
// ==========================================================================
