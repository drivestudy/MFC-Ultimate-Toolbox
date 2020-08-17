// ==========================================================================
// 					Class Specification : COXSplashWndDIB
// ==========================================================================
// Header file : COXSplashWndDib.h
// ==========================================================================
// Version: 9.3
//
// Properties:
//		NO	Abstract class (does not have any objects)
//		NO	Derived from CObject
//
//		NO	Is a CWnd.                     
//		NO	Two stage creation (constructor & Create())
//		NO	Has a message map
//		NO	Needs a resource (template)
//
//		NO	Persistent objects (saveable on disk)      
//		NO	Uses exceptions
//
// Description:
//
//		This is a helper class for the splash window
//		It contains the DIB and its related functions
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __OXSPLASHWNDDIB_H__
#define __OXSPLASHWNDDIB_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#ifndef CLR_NONE
#define CLR_NONE 0xFFFFFFFFL
#endif // CLR_NONE

#ifndef CLR_DEFAULT
#define CLR_DEFAULT 0xFF000000L
#endif // CLR_DEFAULT


/////////////////////////////////////////////////////////////////////////////
// helper class COXSplashWndDIB
class OX_CLASS_DECL COXSplashWndDIB
{
// Data members -------------------------------------------------------------
public:
protected:
	// DDB members:
	CBitmap* m_pBitmap;
	CPalette* m_pPalette;

	// DIB members:
	BITMAPINFOHEADER m_bmih;
	CByteArray m_bits;
	DWORD m_nWidthBits;

	// GetPixel() cache
	int m_xMax;
	int m_yMax; 
	int m_nBitCount;
	int m_nRightShift;
	BOOL m_bTopdown;
	BYTE* m_pBits0;

private:

// Member functions ---------------------------------------------------------
public:
	COXSplashWndDIB();
	// --- In      : 
	// --- Out     :
	// --- Returns : 
	// --- Effect  : Constructor of the object

	virtual ~COXSplashWndDIB();
	// --- In      : 
	// --- Out     :
	// --- Returns : 
	// --- Effect  : Destructor of the object

	CBitmap* GetBitmap();
	// --- In      : 
	// --- Out     :
	// --- Returns : The DDB bitmap of thei object
	// --- Effect  : 

	BOOL GetBitmapInfo(BITMAP& bm);
	// --- In      : 
	// --- Out     : bm
	// --- Returns : Whether it succeeded or not
	// --- Effect  : Gets info from this bitmap object

	BOOL LoadResource(LPCTSTR lpszResourceName, BOOL bPrepareGetPixel);
	// --- In      : lpszResourceName, specifies the bitmap resource
	//				 bPrepareGetPixel, specifies whether a future GetPixel() will
	//					be called; if TRUE, some data members will be initialized
	// --- Out     :
	// --- Returns : TRUE if successful; FALSE otherwise.
	// --- Effect  : load and create a DDB from DIB/DDB in resource

	BOOL LoadFile(LPCTSTR lpszFileName, BOOL bPrepareGetPixel);
	// --- In      : lpszFileName, specifies the bitmap file
	//				 bPrepareGetPixel, specifies whether a future GetPixel() will
	//					be called; if TRUE, some data members will be initialized
	// --- Out     :
	// --- Returns : TRUE if successful; FALSE otherwise.
	// --- Effect  : load and create a DDB from DIB/DDB file

#ifdef OX_SPLASHWND_JPEG
	BOOL LoadJPEGFile(LPCTSTR lpszFileName, BOOL bPrepareGetPixel);
	// --- In      : lpszFileName, specifies the bitmap file
	//				 bPrepareGetPixel, specifies whether a future GetPixel() will
	//					be called; if TRUE, some data members will be initialized
	// --- Out     :
	// --- Returns : TRUE if successful; FALSE otherwise.
	// --- Effect  : load and create a DDB from DIB/DDB file
#endif
	BOOL Draw(CDC* pDC);
	// --- In      : pDC, pointer to the target CDC
	// --- Out     :
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : draw the bitmap in a device context

	COLORREF GetPixel(POINT pt);
	// --- In      : pt, the point to retrieve color
	// --- Out     :
	// --- Returns : color value; CLR_INVALID if out of bound
	// --- Effect  : obtain color of a pixel in the bitmap
	//				 NOTE: have to specify TRUE in LoadResource(..., bPrepareGetPixel)

	COLORREF GetPixel(int x, int y);
	// --- In      : (x, y), the point to retrieve color
	// --- Out     :
	// --- Returns : color value; CLR_INVALID if out of bound
	// --- Effect  : obtain color of a pixel in the bitmap
	//				 NOTE: have to specify TRUE in LoadResource(..., bPrepareGetPixel)

	static HPALETTE CreateDIBPalette (LPBITMAPINFO lpbmi, int& nNumColors);
	// --- In      : lpbmi: Pointer to the DIB
	// --- Out     : nNumColors : Number of colors in palette
	// --- Returns : A palette created from the DIB bitmap
	// --- Effect  : 

protected:
	void PrepareGetPixel(BITMAPINFOHEADER& bmih, int iNumColors);
	BOOL CopyFile(CFile* pDestFile, CFile* pSourceFile); 

private:
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __OXSPLASHWNDDIB_H__

// end of OXSplashWndDIB.h
