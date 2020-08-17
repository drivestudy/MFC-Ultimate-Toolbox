// ==========================================================================
// 							Class Implementation : COXDIB
// ==========================================================================

// Source file : oxdib.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "oxdib.h"		// class specification
#include "limits.h"		// For long -> int truncation

#ifndef NO_DITHER
	#include "oxhlftne.h"	// conversion tables and halftone palettefor 16 bit
#endif // NO_DITHER

#include <windowsx.h>
#include <windowsx.h>

#include "UTB64Bit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[]=__FILE__;
#endif

#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')

#define SIZE_ERROR_BUF	255


IMPLEMENT_SERIAL(COXDIB, CObject, 1)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// Data members -------------------------------------------------------------
// protected:
    // HDIB m_hDIB;
    // --- Handle(Global memory) to DIB (or NULL) when not connected to a DIB

    // CPalette* m_pPalette;
	// --- Pointer to the pallet belonging to the DIB (or NULL if none)

// private:
	
// Member functions ---------------------------------------------------------
// public:

COXDIB::COXDIB()
	:
	m_hDIB(NULL),
	m_pPalette(NULL)
{
	ASSERT_VALID(this);
}
	
COXDIB::COXDIB(HDIB hDIB)
	:
	m_hDIB(NULL),
	m_pPalette(NULL)
{
	// Copy the DIB memory and initialize the palette of the copy
	m_hDIB=CopyHandle (hDIB);
	if (m_hDIB != NULL)
	{
		LPSTR lpDIB=(LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	
		InitPalette(lpDIB); 
										
		::GlobalUnlock(m_hDIB);
	}
	
	ASSERT_VALID(this);
}

BOOL COXDIB::ClearSystemPalette()
{
	//*** A dummy palette setup
	struct
	{
		WORD Version;
		WORD NumberOfEntries;
		PALETTEENTRY aEntries[256];
	} Palette =
			{
			0x300,
			256
			};

	HPALETTE ScreenPalette=0;
	HDC ScreenDC;
	int Counter;
	UINT nMapped=0;
	BOOL bOK=FALSE;
	int  nOK=0;

	// Reset everything in the system palette to black
	for(Counter=0; Counter < 256; Counter++)
	{
		Palette.aEntries[Counter].peRed=0;
		Palette.aEntries[Counter].peGreen=0;
		Palette.aEntries[Counter].peBlue=0;
		Palette.aEntries[Counter].peFlags=PC_NOCOLLAPSE;
	}

	// Create, select, realize, deselect, and delete the palette
	ScreenDC=::GetDC(NULL);
	ScreenPalette=::CreatePalette((LOGPALETTE*)&Palette);

	if (ScreenPalette)
	{
		ScreenPalette=::SelectPalette(ScreenDC, ScreenPalette, FALSE);
		nMapped=::RealizePalette(ScreenDC);
		ScreenPalette=::SelectPalette(ScreenDC, ScreenPalette, FALSE);
		bOK=::DeleteObject(ScreenPalette);
	}

	nOK=::ReleaseDC(NULL, ScreenDC);

	return bOK && (nOK==1);
}

BOOL COXDIB::GetSystemPalette(CPalette* pPalette)
	{
	ASSERT(pPalette != NULL);

    HDC hDC;                // handle to a DC
    HANDLE hLogPal;         // handle to a logical palette
    LPLOGPALETTE lpLogPal;  // pointer to a logical palette

    // Find out how many palette entries we want.
    hDC=GetDC(NULL);
    if (hDC==NULL)
        return FALSE;

    // For non-palette devices, there's no palette available
    if ((::GetDeviceCaps(hDC, RASTERCAPS) & RC_PALETTE) != RC_PALETTE)
		{
		TRACE(_T("COXDIB::GetSystemPalette : No palette device\n"));
        return FALSE;
		}

    // Find out the number of palette entries on this device.
    int nColors=GetDeviceCaps(hDC, SIZEPALETTE);

    // Sometimes we'll use the # of system reserved
    // colors for our palette size.
    if (nColors==0)
        nColors=GetDeviceCaps(hDC, NUMCOLORS);

    ASSERT(nColors != 0);

    // Allocate room for the palette and lock it.

  	hLogPal=::GlobalAlloc(GHND, sizeof(LOGPALETTE)
								+ sizeof(PALETTEENTRY)
								* nColors);

	// if not enough memory, clean up and return NULL 
	if (hLogPal==NULL)
		{
		TRACE(_T("COXDIB::GetSystemPalette : Not enough memory for logical  palette\n"));
	    ReleaseDC(NULL, hDC);
		return FALSE;
		}

    // get a pointer to the logical palette
	lpLogPal=(LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);

	// set version and number of palette entries
	lpLogPal->palVersion=PALVERSION;
	lpLogPal->palNumEntries=(WORD)nColors;

    // Copy the current system palette into our logical palette

    ::GetSystemPaletteEntries(hDC, 0, nColors,
            (LPPALETTEENTRY)(lpLogPal->palPalEntry));

    // Go ahead and create the palette.  Once it's created,
    // we no longer need the LOGPALETTE, so free it.    
	// -- create the palette and get handle to it
	BOOL bResult=pPalette->CreatePalette(lpLogPal);
	::GlobalUnlock((HGLOBAL) hLogPal);
	::GlobalFree((HGLOBAL) hLogPal);

    ReleaseDC(NULL, hDC);

    return bResult;
	}

	
HPALETTE COXDIB::CreateDIBPalette(LPBITMAPINFO lpbmi,int& nNumColors)
{ 
	LPBITMAPINFOHEADER lpbi=NULL;
	LPLOGPALETTE lpPal=NULL;
	HANDLE hLogPal=NULL;   
	HPALETTE hPal=NULL;   
	int i=0;   

	// ... Initialize return variable
	nNumColors=0;
   
	lpbi=(LPBITMAPINFOHEADER)lpbmi;   
	if(lpbi->biBitCount<=8)
		nNumColors=(1<<lpbi->biBitCount);   
	else   
		nNumColors=0;  // No palette needed for 24 BPP DIB   
   
	if(lpbi->biClrUsed>0)   
		nNumColors=lpbi->biClrUsed;  // Use biClrUsed
   
	if (nNumColors!=0)
	{
		hLogPal=::GlobalAlloc(GHND,
			sizeof(LOGPALETTE)+sizeof (PALETTEENTRY)*nNumColors);   
		lpPal=(LPLOGPALETTE)::GlobalLock(hLogPal);      
		lpPal->palVersion=(WORD)0x300;      
		lpPal->palNumEntries=(WORD)nNumColors;      
   
		for(i=0; i<nNumColors; i++)      
		{
			lpPal->palPalEntry[i].peRed=lpbmi->bmiColors[i].rgbRed;   	  
			lpPal->palPalEntry[i].peGreen=lpbmi->bmiColors[i].rgbGreen; 	    
			lpPal->palPalEntry[i].peBlue=lpbmi->bmiColors[i].rgbBlue;  	   
			lpPal->palPalEntry[i].peFlags=0;   	  
		}   	  
		hPal=::CreatePalette(lpPal);      
		::GlobalUnlock(hLogPal);      
		::GlobalFree(hLogPal);      
	}
	return hPal;   
} 

BOOL COXDIB::LoadResource(LPCTSTR lpszResourceName, 
						  HINSTANCE hInstance/*=NULL*/)
{
	BOOL bResult=FALSE;

    HRSRC hRsrc=NULL;
    HGLOBAL hGlobal=NULL;
    HBITMAP hBitmapFinal=NULL;
	HPALETTE hPal=NULL;
    LPBITMAPINFOHEADER lpbi=NULL;
    HDC hdc=NULL;
    int iNumColors=0;

	CPalette palette; 

	if(hInstance==NULL)
		hInstance=AfxFindResourceHandle(lpszResourceName,RT_BITMAP);

	if(hInstance!=NULL)
		hRsrc=::FindResource(hInstance,lpszResourceName,RT_BITMAP);
	if(hRsrc!=NULL)
		hGlobal=::LoadResource(hInstance,hRsrc);

	if(hGlobal!=NULL)
	{
		lpbi=(LPBITMAPINFOHEADER)::LockResource(hGlobal);
		if(lpbi!=NULL)
		{
			hdc=::GetDC(NULL);
			if(hdc!=NULL)
			{
				hPal=CreateDIBPalette((LPBITMAPINFO)lpbi,iNumColors);
				if((hPal==NULL)&&(::GetDeviceCaps(hdc,SIZEPALETTE)==256))
				{
					// .... Trying to show bitmap without palette on 
					//		256-color display so let's use halftone palette 
					//		(better than nothing)
					if(GetHalfTonePalette(&palette))
						hPal=(HPALETTE)palette.Detach();

				}
				if(hPal != NULL)
				{
					SelectPalette(hdc, hPal, FALSE);
					RealizePalette(hdc);
				}
		
				hBitmapFinal=::CreateDIBitmap(hdc,(LPBITMAPINFOHEADER)lpbi,
					(LONG)CBM_INIT,
					(LPSTR)lpbi+lpbi->biSize+iNumColors*sizeof(RGBQUAD),
					(LPBITMAPINFO)lpbi,DIB_RGB_COLORS);

				palette.Attach(hPal);
				bResult=(BitmapToDIB(hBitmapFinal,&palette)!=NULL);

				::DeleteObject((HGDIOBJ)palette.Detach());
				::DeleteObject((HGDIOBJ)hPal);
				::DeleteObject((HGDIOBJ)hBitmapFinal);
				::DeleteObject((HGDIOBJ)hPal);
				::ReleaseDC(NULL,hdc);
			}
			::UnlockResource(hGlobal);
		}
		::FreeResource(hGlobal);
	}

    return bResult;
} 

HDIB WINAPI COXDIB::MakeCopy()
{
	return CopyHandle(m_hDIB);
}

CBitmap* COXDIB::MakeBitmap(CDC* pDC, CSize& bmSize)
{
	BITMAP bm;
    DWORD  dwFore, dwBack;
    
    LPSTR lpDIB;               	// pointer to packed-DIB
	PDIB lpbmih;				// pointer to a Win 3.0-style DIB
	LPBITMAPINFO lpbmi;  		// pointer to the bitmap info

	lpDIB=(LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);

	// point to the header (whether Win 3.0 and old) 
	lpbmi=(LPBITMAPINFO)lpDIB;
	lpbmih=(PDIB)lpDIB;
	
    // checks to see whether DIB buffer is properly loaded
    // ... Check for valid DIB handle
	if (m_hDIB==NULL)
	{
		TRACE(_T("COXDIB::MakeBitmap : DIB not loaded, nothing to copy to bitmap\n"));
        bmSize.cx=bmSize.cy=0;
        ::GlobalUnlock((HGLOBAL) m_hDIB);
        return NULL;
   	}

    // checks to see wether DIB header is valid
    // ... Check for valid DIB header compression member (must be uncompressed=BI_RGB)
	if (lpbmi->bmiHeader.biCompression != BI_RGB || lpbmih->biCompression != BI_RGB)
	{
		TRACE(_T("COXDIB::MakeBitmap : DIB compressed, unable to handle bitmap\n"));
        ::GlobalUnlock((HGLOBAL) m_hDIB);
        return NULL;
   	}

	
    // this code conditions the bitmap for mono or color
    int nPlanes=pDC->GetDeviceCaps(PLANES);
    int nBitsPixel=pDC->GetDeviceCaps(BITSPIXEL);
    
    CBitmap* pConfigBitmap=new CBitmap;
    
    char bits[100];
    if (GetNumBitsPerPixel(lpDIB)==1)
	{
        pConfigBitmap->CreateBitmap(1, 1, 1, 1, bits);
	}
    else
	{
        pConfigBitmap->CreateBitmap(1, 1, nPlanes, nBitsPixel, bits);
	}

    CBitmap* pOriginalBitmap =
        (CBitmap*) pDC->SelectObject(pConfigBitmap);

	// CreateDIBitmap "switches bits" for mono bitmaps, depending on colors,
	//  so we'll fool it
    if(GetMonoColors(dwFore,dwBack,lpDIB))
	{
        SetMonoColors(0L,0xFFFFFFL,lpDIB);
	}
    
    // Get the DIB's palette, then select it into DC
	HPALETTE hPal=NULL;           // Our DIB's palette
	HPALETTE hOldPal=NULL;        // Previous palette
	if (m_pPalette != NULL)
	{
		hPal=(HPALETTE) m_pPalette->m_hObject;

		// ... Select as background since we have
		//     already realized in forground if needed
		hOldPal=::SelectPalette(pDC->m_hDC, hPal, TRUE);
		pDC->RealizePalette();
	}
    
#ifdef WIN32
    HBITMAP hBitmap=::CreateDIBitmap(pDC->GetSafeHdc(), lpbmih,
            CBM_INIT, (CONST BYTE*) FindDIBBits(lpDIB), lpbmi, DIB_RGB_COLORS);
#else
    HBITMAP hBitmap=::CreateDIBitmap(pDC->GetSafeHdc(), lpbmih,
            CBM_INIT, FindDIBBits(lpDIB), lpbmi, DIB_RGB_COLORS);
#endif
	// ... Reselect old palette
	if (hOldPal != NULL)
	{
		::SelectPalette(pDC->m_hDC, hOldPal, TRUE);
	}

    if (hBitmap==NULL)
	{
        TRACE(_T("COXDIB::MakeBitmap: null bitmap\n"));  
        delete pDC->SelectObject(pOriginalBitmap); // delete config bitmap
        ::GlobalUnlock((HGLOBAL) m_hDIB);
        return NULL; // untested error logic
	}
    
    if(GetNumBitsPerPixel(lpDIB)==1)
	{
		SetMonoColors(dwFore,dwBack,lpDIB);
	}
    
    // Can't use CBitmap::FromHandle here because we need to
    //  delete the object later
    CBitmap* pBitmap=new CBitmap;
    pBitmap->Attach(hBitmap);
    pBitmap->GetObject(sizeof(bm), &bm);
    bmSize.cx=bm.bmWidth;
    bmSize.cy=bm.bmHeight;
    delete pDC->SelectObject(pBitmap); // delete configuration bitmap
    
    ::GlobalUnlock((HGLOBAL) m_hDIB);
    
    return pOriginalBitmap;
}

HDIB COXDIB::Detach()
	{
	HDIB hDIB=m_hDIB;
	m_hDIB=NULL;
	return hDIB;
	}

BOOL COXDIB::IsEmpty() const
	{
	return (m_hDIB==NULL);
	}
	
void COXDIB::Empty()
{
	ASSERT_VALID(this);
	if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB=NULL;
		delete m_pPalette;	// May already be NULL
		m_pPalette=NULL;
	}

	ASSERT_VALID(this);
}
	
void COXDIB::SetMonoColors(DWORD dwForeground, DWORD dwBackground, 
						   LPSTR lpDIBHeader /*==NULL */)
{
	BOOL bWasLockedBefore=TRUE;
    // ... Check for valid Info pointer
	if(lpDIBHeader==NULL)
	{
		lpDIBHeader=(LPSTR)::GlobalLock((HGLOBAL)m_hDIB);
		if(lpDIBHeader==NULL)
		{
			TRACE(_T("COXDIB::SetMonoColors : fails with null pointer\n"));
			return;
		}
		bWasLockedBefore=FALSE;
	}
    
	ASSERT(GetNumBitsPerPixel(lpDIBHeader)==1);
    if(GetNumBitsPerPixel(lpDIBHeader)!=1)
   	{
		if(!bWasLockedBefore)
		    ::GlobalUnlock((HGLOBAL)lpDIBHeader);
        return;
   	}
    
	LPBITMAPINFOHEADER lpbmih;  // pointer to a Win 3.0-style DIB

	// point to the header (whether Win 3.0 and old) 
	lpbmih=(LPBITMAPINFOHEADER)lpDIBHeader;

#ifdef WIN32
    ULONG* pPalette=(ULONG*)((LPSTR) lpbmih + sizeof(BITMAPINFOHEADER));
#else
	unsigned long far* pPalette=
	   (unsigned long far*)((LPSTR) lpbmih + sizeof(BITMAPINFOHEADER));
#endif

	dwForeground=RGB(GetBValue(dwForeground),GetGValue(dwForeground),
		GetRValue(dwForeground));
	dwBackground=RGB(GetBValue(dwBackground),GetGValue(dwBackground),
		GetRValue(dwBackground));
    *pPalette=dwForeground;
    *(++pPalette)=dwBackground;

	if(!bWasLockedBefore)
		::GlobalUnlock((HGLOBAL)lpDIBHeader);

    return;
}

///////////////////////////////////////////////////////////////////
BOOL COXDIB::GetMonoColors(DWORD& dwForeground, DWORD& dwBackground, LPSTR lpDIBHeader /*==NULL */)
{
	BOOL bWasLockedBefore=TRUE;
	// ... Check for valid Info pointer
	if (lpDIBHeader==NULL)
	{
		lpDIBHeader=(LPSTR)::GlobalLock((HGLOBAL)m_hDIB);
		if(lpDIBHeader==NULL)
		{
			TRACE(_T("COXDIB::GetMonoColors : fails with null pointer\n"));
			return FALSE;
		}
		bWasLockedBefore=FALSE;
	}
	
    if (GetNumBitsPerPixel(lpDIBHeader) != 1)
	{
		if(!bWasLockedBefore)
		    ::GlobalUnlock((HGLOBAL)lpDIBHeader);
        return FALSE;
	}                                  
    
	LPBITMAPINFOHEADER lpbmih;  // pointer to a Win 3.0-style DIB

	// point to the header (whether Win 3.0 and old) 
	lpbmih=(LPBITMAPINFOHEADER)lpDIBHeader;
    
    unsigned long far* pPalette=(unsigned long far*)
                       ((LPSTR) lpbmih + sizeof(BITMAPINFOHEADER));
    dwForeground=*pPalette;
    dwBackground=*(++pPalette);

	if(!bWasLockedBefore)
		::GlobalUnlock((HGLOBAL)lpDIBHeader);

    return TRUE;
}
	

BOOL COXDIB::Paint(CDC* pDC, const CRect& rDCRect, const CRect& rDIBRect) 
{
	ASSERT_VALID(this);

	LPSTR    lpDIBHdr;            // Pointer to BITMAPINFOHEADER
	LPSTR    lpDIBBits;           // Pointer to DIB bits
	BOOL     bSuccess=FALSE;      // Success/fail flag
	HPALETTE hPal=NULL;           // Our DIB's palette
	HPALETTE hOldPal=NULL;        // Previous palette

	// ... Check for valid DIB handle
	if (m_hDIB==NULL)
	{
		TRACE(_T("COXDIB::PaintDIB : DIB not loaded, nothing to paint\n"));
		return FALSE;
	}

	// Lock down the DIB, and get a pointer to the beginning of the bit buffer
	lpDIBHdr =(LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	lpDIBBits=FindDIBBits(lpDIBHdr);

	// Get the DIB's palette, then select it into DC
	if (m_pPalette != NULL)
	{
		hPal=(HPALETTE) m_pPalette->m_hObject;

		// ... Select as background since we have
		//     already realized in forground if needed
		hOldPal=::SelectPalette(pDC->m_hDC, hPal, FALSE);
		::RealizePalette(pDC->m_hDC);
	}

	// ... Make sure to use the stretching mode best for color pictures
	::SetStretchBltMode(pDC->m_hDC, COLORONCOLOR);

	// Determine whether to call Stretcm_hDIBits() or SetDIBitsToDevice()
	if ((rDCRect.Width()==rDIBRect.Width()) &&
	   (rDCRect.Height()==rDIBRect.Height()) &&
	   (pDC->GetMapMode()==MM_TEXT))
		bSuccess=::SetDIBitsToDevice(pDC->m_hDC,       	// pDC->m_hDC
								   rDCRect.left,            // DestX
								   rDCRect.top,             // DestY
								   rDCRect.Width(),        	// nDestWidth
								   rDCRect.Height(),       	// nDestHeight
								   rDIBRect.left,           // SrcX
								   (int)GetSize(lpDIBHdr).cy -
									  rDIBRect.top -
									  rDIBRect.Height(),   	// SrcY
								   0,                       // nStartScan
								   (WORD)GetSize(lpDIBHdr).cy,  	// nNumScans
								   lpDIBBits,               // lpBits
								   (LPBITMAPINFO)lpDIBHdr,  // lpBitsInfo
								   DIB_RGB_COLORS);         // wUsage
   else
	  bSuccess=::StretchDIBits(pDC->m_hDC,                // pDC->m_hDC
							   rDCRect.left,                // DestX
							   rDCRect.top,                 // DestY
							   rDCRect.Width(),            	// nDestWidth
							   rDCRect.Height(),           	// nDestHeight
							   rDIBRect.left,               // SrcX
							   rDIBRect.top,                // SrcY
							   rDIBRect.Width(),           	// wSrcWidth
							   rDIBRect.Height(),          	// wSrcHeight
							   lpDIBBits,                   // lpBits
							   (LPBITMAPINFO)lpDIBHdr,      // lpBitsInfo
							   DIB_RGB_COLORS,              // wUsage
							   SRCCOPY);                    // dwROP

   ::GlobalUnlock((HGLOBAL) m_hDIB);

	// ... Reselect old palette
	if (hOldPal != NULL)
	{
		::SelectPalette(pDC->m_hDC, hOldPal, TRUE);
	}

   	return bSuccess;
}
	
LPSTR COXDIB::FindDIBBits(LPSTR lpDIBHeader/*=NULL*/)
{
	ASSERT_VALID(this);

	BOOL bWasLockedBefore=TRUE;
	// if handle to DIB is invalid, return NULL
	if (lpDIBHeader==NULL)
	{
		lpDIBHeader=(LPSTR)::GlobalLock((HGLOBAL)m_hDIB);
		if(lpDIBHeader==NULL)
		{
			TRACE(_T("COXDIB::FindDIBBits: DIB not loaded, cannot find bits\n"));
	  		return NULL;
		}
		bWasLockedBefore=FALSE;
  	}

	LPSTR lpResult;
	
	lpResult=(lpDIBHeader + *(LPDWORD)lpDIBHeader + GetPaletteSize(lpDIBHeader));
	
	if(!bWasLockedBefore)
		::GlobalUnlock((HGLOBAL)lpDIBHeader);

	return lpResult;
}
	
CSize COXDIB::GetSize(LPSTR lpbi/*=NULL*/) const
{
	ASSERT_VALID(this);

	BOOL bOwnDIB=FALSE;

	// if handle to DIB is invalid, return (0,0) 
	if (lpbi==NULL)
	{            
		if (m_hDIB==NULL)
		{
			TRACE(_T("COXDIB::GetSize: DIB not loaded, returning (0,0)\n"));
		  	return CSize(0,0);
	  	}
		 else
	 	{
		 	lpbi=(LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
		 	bOwnDIB=TRUE;
	 	}
	}


	LPBITMAPINFOHEADER lpbmi;  // pointer to a Win 3.0-style DIB
	LPBITMAPCOREHEADER lpbmc;  // pointer to an other-style DIB
	CSize sResult;

	// point to the header (whether Win 3.0 and old) 
	lpbmi=(LPBITMAPINFOHEADER)lpbi;
	lpbmc=(LPBITMAPCOREHEADER)lpbi;

	// return the DIB width if it is a Win 3.0 DIB 
	if (IS_WIN30_DIB(lpbi))
	{
		// Truncating long to int
		ASSERT(lpbmi->biWidth < INT_MAX);
		ASSERT(lpbmi->biHeight < INT_MAX);
		sResult=CSize((int)lpbmi->biWidth, (int)lpbmi->biHeight);
	}
	else  // it is an other-style DIB, so return its width 
		sResult=CSize(lpbmc->bcWidth, lpbmc->bcHeight);
		
	if (bOwnDIB)
	{            
	 	::GlobalUnlock((HGLOBAL) m_hDIB);
	}

	return sResult;
}
	
WORD COXDIB::GetPaletteSize(LPSTR lpDIBHeader/*=NULL*/) const
{
	ASSERT_VALID(this);

	BOOL bWasLockedBefore=TRUE;
	// if handle to DIB is invalid, return 0 
	if (lpDIBHeader==NULL)
	{
		lpDIBHeader=(LPSTR)::GlobalLock((HGLOBAL)m_hDIB);
		if(lpDIBHeader==NULL)
		{
			TRACE(_T("COXDIB::GetPaletteSize: DIB not loaded, returning 0\n"));
	  		return 0;
		}
		bWasLockedBefore=FALSE;
	}

	// calculate the size required by the palette 
   	BOOL bWin30;

   	bWin30=(IS_WIN30_DIB (lpDIBHeader));

   	WORD nResult=0;
   	if (bWin30)
		nResult=(WORD)(GetNumColors(lpDIBHeader) * sizeof(RGBQUAD));
   	else
	  	nResult=(WORD)(GetNumColors(lpDIBHeader) * sizeof(RGBTRIPLE));

	if(!bWasLockedBefore)
		::GlobalUnlock((HGLOBAL)lpDIBHeader);

	return nResult;
}
	
CPalette* COXDIB::GetPalette() const
{
	ASSERT_VALID(this);

	return m_pPalette;
}

WORD COXDIB::GetNumBitsPerPixel(LPSTR lpbi) const
{
	ASSERT_VALID(this);    
	
	BOOL bOwnDIB=FALSE;

	// if handle to DIB is invalid, return 0 
	if (lpbi==NULL)
	{            
		if (m_hDIB==NULL)
		{
			TRACE(_T("COXDIB::GetNumBitsPerPixel: DIB not loaded, returning 0\n"));
		  	return 0;
	  	}
		 else
	 	{
		 	lpbi=(LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
		 	bOwnDIB=TRUE;
	 	}
  	}

	WORD wBitCount;  // DIB bit count

	//  Calculate the number of bits per pixel for the DIB.

	if (IS_WIN30_DIB(lpbi))
		wBitCount=((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	else
		wBitCount=((LPBITMAPCOREHEADER)lpbi)->bcBitCount;

	if (bOwnDIB)
	{            
	 	::GlobalUnlock((HGLOBAL) m_hDIB);
  	}

	return wBitCount;
}
	
WORD COXDIB::GetNumColors(LPSTR lpbi) const
{
	ASSERT_VALID(this);

	BOOL bOwnDIB=FALSE;

	// if handle to DIB is invalid, return 0 
	if (lpbi==NULL)
	{            
		if (m_hDIB==NULL)
		{
			TRACE(_T("COXDIB::GetNumColors: DIB not loaded, returning 0\n"));
		  	return 0;
	  	}
		 else
	 	{
		 	lpbi=(LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
		 	bOwnDIB=TRUE;
	 	}
  	}

	WORD wBitCount;  // DIB bit count

	//  If this is a Windows-style DIB, the number of colors in the
	//  color table can be less than the number of bits per pixel
	//  allows for (i.e. lpbi->biClrUsed can be set to some value).
	//  If this is the case, return the appropriate value.

	if (IS_WIN30_DIB(lpbi))
	{
		DWORD dwClrUsed;

		dwClrUsed=((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
		if (dwClrUsed != 0)
		{
			if (bOwnDIB)
			{            
	 			::GlobalUnlock((HGLOBAL) m_hDIB);
  			}
			return (WORD)dwClrUsed;
		}
	}

	//  Calculate the number of colors in the color table based on
	//  the number of bits per pixel for the DIB.

	if (IS_WIN30_DIB(lpbi))
		wBitCount=((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	else
		wBitCount=((LPBITMAPCOREHEADER)lpbi)->bcBitCount;

	if (bOwnDIB)
	{            
	 	::GlobalUnlock((HGLOBAL) m_hDIB);
  	}
	
	// return number of colors based on bits per pixel
	switch (wBitCount)
	{
		case 1:
			return 2;
		case 4:
			return 16;
		case 8:
			return 256;
		default:
			return 0;
	}
}
	
COXDIB::COXDIB(const COXDIB& DIBSrc)
	:
	m_hDIB(NULL),
	m_pPalette(NULL)
{
	ASSERT_VALID(this);
	ASSERT_VALID(&DIBSrc);
	
	TRACE(_T("COXDIB::COXDIB: Warning, copying bitmap\n"));
	
	// Copy DIB memory
	m_hDIB=CopyHandle(DIBSrc.m_hDIB);
	if(m_hDIB!=NULL)
	{
		LPSTR pDIB=(LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
		// Initialize the palette of the copy
		InitPalette(pDIB);
		
		::GlobalUnlock(m_hDIB);
	}
}

COXDIB& COXDIB::operator=(const COXDIB& DIBSrc)
{
	if(this==&DIBSrc)
		return *this;
		
	ASSERT_VALID(this);
	ASSERT_VALID(&DIBSrc);
	
	// Call COXDIB::operator=(HDIB hDIBSrc) to make a deep copy
	*this=DIBSrc.m_hDIB;

	// ... Make sure deep copy is made and succeeded
	// ... The handles must be different, except when they are both NULL
	ASSERT((m_hDIB != DIBSrc.m_hDIB) || ((m_hDIB==NULL) && (DIBSrc.m_hDIB==NULL)));
	ASSERT_VALID(this);
	
	return *this;
}

COXDIB& COXDIB::operator=(HDIB hDIBSrc)
{
	ASSERT_VALID(this);

	if (hDIBSrc != NULL)
	{
		LPSTR pDIBSrc=(LPSTR)::GlobalLock((HGLOBAL)hDIBSrc);
		if (!IS_WIN30_DIB(pDIBSrc))
		{
			TRACE(_T("COXDIB::operator=: DIB is an other-style DIB (copy not supported)\n"));
			ASSERT(FALSE);
		}
		else
		{
			// If this already contains a DIB, delete first
			Empty();

			// Copy DIB memory
			m_hDIB=CopyHandle(hDIBSrc);
			if (m_hDIB != NULL)
			{
				LPSTR pDIB=(LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
				// Initialize the palette of the copy
				InitPalette(pDIB);
				
				::GlobalUnlock(m_hDIB);
			}
		}

		::GlobalUnlock(hDIBSrc);
	}

	ASSERT_VALID(this);
	return *this;
}

CBitmap* COXDIB::MakeBitmapFromDIB(CDC* pDC)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	if (pDC==NULL)
		return NULL;
				 
	BOOL bOwnDIB=FALSE;

	// if handle to DIB is invalid, return 0
	LPSTR lpbi=NULL;               	// pointer to packed-DIB 
	if (lpbi==NULL)
	{            
		if (m_hDIB==NULL)
		{
			TRACE(_T("COXDIB::MakeBitmapFromDIB: DIB not loaded, returning 0\n"));
		  	return 0;
	  	}
		 else
	 	{
		 	lpbi=(LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
			bOwnDIB=TRUE;
	 	}
  	}

	LPSTR		lpDIBHdr, lpDIBBits;

	HBITMAP  	hBitmap;
	CPalette* 	pOldPal=NULL;

	// Lock down the DIB, and get a pointer to the beginning of the bit buffer
	lpDIBHdr =lpbi;
	lpDIBBits=FindDIBBits(lpDIBHdr);

	if (m_pPalette != NULL)
	{
		pOldPal=pDC->SelectPalette (m_pPalette, FALSE);
		pDC->RealizePalette();
	}
	
	hBitmap=::CreateDIBitmap (pDC->GetSafeHdc(),
	 	                       (LPBITMAPINFOHEADER)lpDIBHdr,
	     	                   CBM_INIT,
	        	               lpDIBBits,
	            	           (LPBITMAPINFO) lpbi,
	                	       DIB_RGB_COLORS);
	                	       
	if (hBitmap==NULL)
	{
        TRACE(_T("COXDIB::MakeBitmapFromDIB: null bitmap\n"));  
		if (bOwnDIB)
		{            
	 		::GlobalUnlock((HGLOBAL) m_hDIB);
  		}
	    return NULL; // untested error logic
   	}
	
	if (pOldPal != NULL)
		pDC->SelectPalette (pOldPal, FALSE);
	
	CBitmap* pBitmap=new CBitmap;
    pBitmap->Attach(hBitmap);

	if (bOwnDIB)
	{            
	 	::GlobalUnlock((HGLOBAL) m_hDIB);
  	}
	
	return pBitmap;
}


BOOL COXDIB::SetPaletteUsage(CPalette* pPal, UINT wUsage)
	{
    PALETTEENTRY       ape[256];
    RGBQUAD FAR*       pRgb;
    WORD FAR*          pw;
    int                nColors;
    int                n;
	
    if (pPal==NULL)
        pPal=CPalette::FromHandle((HPALETTE)GetStockObject(DEFAULT_PALETTE));
	
    if (m_hDIB==NULL)
        return FALSE;
	
    PDIB pdib=(PDIB)::GlobalLock((HGLOBAL)m_hDIB);

	nColors=DibNumColors(pdib);
    
    if (nColors==3 && DibCompression(pdib)==BI_BITFIELDS)
        nColors=0;
	
    if (nColors>0)
		{
        pRgb=DibColors(pdib);
		
        switch (wUsage)
			{
            //
            // Set the DIB color table to palette indexes
            //
            case DIB_PAL_COLORS:
                for (pw=(WORD FAR*)pRgb, n=0; n < nColors; n++, pw++)
                    *pw=(WORD)n;
                break;
				
				//
				// Set the DIB color table to RGBQUADS
				//
            default:
            case DIB_RGB_COLORS:
                nColors=__min(nColors, 256);
				
                pPal->GetPaletteEntries(0, nColors, ape);
				
                for (n=0; n < nColors; n++)
					{
                    pRgb[n].rgbRed     =ape[n].peRed;
                    pRgb[n].rgbGreen   =ape[n].peGreen;
                    pRgb[n].rgbBlue    =ape[n].peBlue;
                    pRgb[n].rgbReserved=0;
					}
                break;
			}
		}

	::GlobalUnlock((HGLOBAL)m_hDIB);
    return TRUE;
	}

BOOL COXDIB::CreateEmptyDIB(WORD wBitsPerPix, DWORD dwWidth, DWORD dwHeight)
{
    PDIB		lpbi;
    DWORD       dwSizeImage;
    int         i;
    DWORD FAR*	pdw;

    dwSizeImage=dwHeight * WIDTHBYTES(dwWidth*wBitsPerPix);

    lpbi=(PDIB)GlobalAllocPtr(GHND, sizeof(BITMAPINFOHEADER) + dwSizeImage + 1024);

    if (lpbi==NULL)
        return NULL;

	InitBitmapInfoHeader(lpbi, dwWidth, dwHeight, wBitsPerPix);

    pdw=(DWORD FAR*)((LPBYTE)lpbi + (int)lpbi->biSize);

    for (i=0; i < (int)lpbi->biClrUsed / 16; i++)
	{
        *pdw++=0x00000000;    // 0000  black
        *pdw++=0x00800000;    // 0001  dark red
        *pdw++=0x00008000;    // 0010  dark green
        *pdw++=0x00808000;    // 0011  mustard
        *pdw++=0x00000080;    // 0100  dark blue
        *pdw++=0x00800080;    // 0101  purple
        *pdw++=0x00008080;    // 0110  dark turquoise
        *pdw++=0x00C0C0C0;    // 1000  gray
        *pdw++=0x00808080;    // 0111  dark gray
        *pdw++=0x00FF0000;    // 1001  red
        *pdw++=0x0000FF00;    // 1010  green
        *pdw++=0x00FFFF00;    // 1011  yellow
        *pdw++=0x000000FF;    // 1100  blue
        *pdw++=0x00FF00FF;    // 1101  pink (magenta)
        *pdw++=0x0000FFFF;    // 1110  cyan
        *pdw++=0x00FFFFFF;    // 1111  white
	}

	// Discard existing DIB and assign newly created
	Empty();
	m_hDIB=(HDIB)GlobalPtrHandle(lpbi);
	GlobalUnlockPtr(lpbi);

	return TRUE;
}

BOOL COXDIB::ResizeDIB(DWORD dwWidth, DWORD dwHeight)
{
	if (m_hDIB==NULL)
		return FALSE;

	PDIB pdibSource=(PDIB)::GlobalLock((HGLOBAL)m_hDIB);
	WORD wBitPixel= GetNumBitsPerPixel((LPSTR)pdibSource);
	int nBytePerPixel=wBitPixel / 8;
	if (nBytePerPixel <= 0)
	{
		TRACE(_T("In COXDIB::ResizeDIB: Resizing only supported for 24 and 8 bit DIB's\n"));
		::GlobalUnlock((HGLOBAL)m_hDIB);
		return FALSE;
	}

	// Create a new empty destination memory block to hold the new resized DIB
	// needs the same number of bits per pixel as the source DIB
	// we're going to use some pointers to the DIB Bits, so lock down the memory block
    DWORD dwSizeImage=dwHeight * (DWORD)((dwWidth * wBitPixel / 8 + 3)& ~3);
    PDIB pDIBDest=(PDIB)GlobalAllocPtr(GHND, 
		sizeof(BITMAPINFOHEADER) + dwSizeImage + 1024);
    if (pDIBDest==NULL)
	{
		TRACE(_T("COXDIB::ResizeDIB: Could not create an empty but allocated DIB\n"));
		::GlobalUnlock((HGLOBAL)m_hDIB);
		return FALSE;
	}

	// The destination DIB must have the same bitmap info header + palette as the source DIB
	// so copy a (bitmap info + palette) memory block.
	DWORD_PTR dwInfoAndPal=(BYTE*)DibPtr(pdibSource) - (BYTE*)pdibSource;
	memcpy(pDIBDest, pdibSource, (size_t)dwInfoAndPal);

	// because its a resize adjust the old values (stored in the new memory 
	// block by the memcpy) to the new size values
	pDIBDest->biWidth			= dwWidth;
	pDIBDest->biHeight			= dwHeight;
	pDIBDest->biSizeImage		= dwSizeImage;

	// Assign and precalculate some values, to use in the formula
	// P=2 * (D - S) with D is destination and S is Source
	// When do we put a pixel extra or leave one out : (first version of formula)
	//		accumulator += P
	//		accumulator>S  is the same as accumulator / S>1

	// We spotted some rounding errors --> second formula
	//  (accumulator + sign(accumulator)S) / 2 * S>1=divresult
	//		then accumulator -= 2 * S * divresult;

	DWORD dwSourceWidth=DibWidth(pdibSource);
	DWORD dwSourceHeight=DibHeight(pdibSource);
	DWORD dwOffsetWidth=2 * (dwWidth - dwSourceWidth);
	DWORD dwOffsetHeight=2 * (dwHeight - dwSourceHeight);

	WORD* WidthCopyArray=new WORD[dwSourceWidth];
	WORD* pWCpy=WidthCopyArray;

	int nTempTotal=0;
	
	int nAccumulator=0;
	int nDivResult=0;

	// Compute the transformation array for one scanline
	int Pixel;                                                     
	int nSourceWidth=(int)dwSourceWidth;
	int nDoubleSWidth=2 * nSourceWidth;
	for(Pixel=0; Pixel < nSourceWidth; Pixel++)
	{
		// Per new pixel the acc must be incremented
		nAccumulator += (int)dwOffsetWidth;

		// Do we need to write a pixel
		nDivResult=(nAccumulator + nSourceWidth * ((nAccumulator>0) ? 1 : -1)) / nDoubleSWidth;

		ASSERT(nDivResult >=-1);
		if (nDivResult != 0)
			// we needed to write or delete a pixel, so adjust the acc to account for that
			nAccumulator -= nDoubleSWidth * nDivResult;

		// write the number of pixels to write in the correct place in the array and 
		// move the array pointer one place further
		*(pWCpy++)=(WORD)(nDivResult + 1);

#ifdef _DEBUG
		nTempTotal += nDivResult + 1;
#endif
	}
#ifdef _DEBUG
		ASSERT(dwWidth==(DWORD)nTempTotal);
#endif

	WORD* HeightCopyArray=new WORD[dwSourceHeight];
	WORD* pHCpy=HeightCopyArray;

	// Compute the transformation array for the different scanlines (= height DIB)
	nTempTotal=0;
	nAccumulator=0;
	int Scanline;
	int nSourceHeight=(int)dwSourceHeight;
	int nDoubleSHeight=2 * nSourceHeight;
	for(Scanline=0; Scanline < nSourceHeight; Scanline++)
	{
		nAccumulator += (int)dwOffsetHeight;
		nDivResult=(nAccumulator + nSourceHeight * ((nAccumulator>0) ? 1 : -1)) / nDoubleSHeight;

		ASSERT(nDivResult >=-1);
		if (nDivResult != 0)
			nAccumulator -= nDoubleSHeight * nDivResult;

		*(pHCpy++)=(WORD)(nDivResult + 1);

#ifdef _DEBUG
		nTempTotal += nDivResult + 1;
#endif
	}
	ASSERT(dwHeight==(DWORD)nTempTotal);
	

	// Reset the transformation array pointers to their begin
	pHCpy=HeightCopyArray;
	pWCpy=WidthCopyArray;
	
	// Get pointers to begin of each bitmap
	BYTE _huge* pSourceScanline=(BYTE _huge*)DibXY(pdibSource, 0, 0);
	BYTE _huge* pWithinSourceScanline=pSourceScanline;
	BYTE _huge* pDestScanline=(BYTE _huge*)DibXY(pDIBDest, 0, 0);
	BYTE _huge* pWithinDestScanline=pDestScanline;

	// Step through, resizing each pixel in each scan line
	// to its required size
	int OldScanline=0;
	ULONG ul;
	while(OldScanline < nSourceHeight)
	{
		// If equal to 0, this scanline disappears, so skip and proceed with next one
		if (*pHCpy != 0)
		{
			DWORD nDup=0;
			int OldPixel=0;
			pWCpy=WidthCopyArray;
			// iterate the pixels on one scanline
			while(OldPixel < nSourceWidth)
			{
				nDup=*(pWCpy++);
				// if the pixel we're dealing with has to be copied several times
				while(nDup>0)
				{
					for(int j=0; j < nBytePerPixel; j++)
						*(pWithinDestScanline++)=*(pWithinSourceScanline + j);
					nDup--;
				}

				
				pWithinSourceScanline += nBytePerPixel;
				OldPixel++;
			}

			ASSERT((pWithinDestScanline - pDestScanline)==(LONG)dwWidth * nBytePerPixel);

			ul=(ULONG)(ULONG_PTR)pWithinDestScanline;
			ul +=3;
			ul >>=2;
			ul <<=2;
			pWithinDestScanline=(LPBYTE)ULongToPtr(ul);
			DWORD dwAlignedScanLineLength=PtrToLong(pWithinDestScanline - pDestScanline);
			
			nDup=1;
			while(nDup < *pHCpy)
			{
				memcpy(pWithinDestScanline, pDestScanline, (size_t)dwAlignedScanLineLength);
				pWithinDestScanline += dwAlignedScanLineLength;
				nDup++;
			}

			// align the source and destination pointers on DWORD boundaries
			ul=(ULONG)(ULONG_PTR)pWithinDestScanline;
			ul +=3;
			ul >>=2;
			ul <<=2;
			pWithinDestScanline=(LPBYTE)ULongToPtr(ul);
			pDestScanline=pWithinDestScanline;
		}
		else
			pWithinSourceScanline += nSourceWidth * nBytePerPixel;

		// align the source and destination pointers on DWORD boundaries
		ul=(ULONG)(ULONG_PTR)pWithinSourceScanline;
		ul +=3;
		ul >>=2;
		ul <<=2;
		pWithinSourceScanline=(LPBYTE)LongToPtr(ul);
		
		OldScanline++;
		pHCpy++;
	}
	
	delete[] WidthCopyArray;
	delete[] HeightCopyArray;

	::GlobalUnlock((HGLOBAL)m_hDIB);
	GlobalUnlockPtr(pDIBDest);

	// Release the old source DIB handle
	//  and assign the new DIB memory handle to this COXDIB object
	::GlobalFree((HGLOBAL) m_hDIB);
	m_hDIB=(HDIB)GlobalPtrHandle(pDIBDest);

	return TRUE;
}

BOOL COXDIB::GetHalfTonePalette(CPalette* pPalette)
{
	ASSERT(pPalette != NULL);
	
	BOOL bResult(FALSE); 

	CDC* pDC=CWnd::GetDesktopWindow()->GetDC();
	if (pDC==NULL)
	{
		TRACE(_T("COXDIB::GetHalfTonePalette: Could not allocate a DC for the desktopwindow\n"));
		return FALSE;
	}

    // Find out the number of palette entries on this device.
    int nColors=pDC->GetDeviceCaps(SIZEPALETTE);

    // For non-palette devices, we'll use the # of system
    // colors for our palette size.
    if (nColors==0)
        nColors=pDC->GetDeviceCaps(NUMCOLORS);

    if (nColors < 256)
	{
		CWnd::GetDesktopWindow()->ReleaseDC(pDC);
		return FALSE;
	}

#ifdef WIN32
	bResult=pPalette->CreateHalftonePalette(pDC);
#else
    HANDLE hLogPal;         // handle to a logical palette
    LPLOGPALETTE lpLogPal;  // pointer to a logical palette

    // Allocate room for the palette and lock it.
  	hLogPal=::GlobalAlloc(GHND, sizeof(LOGPALETTE)+sizeof(PALETTEENTRY)*256);
	// if not enough memory, clean up and return NULL 
	if (hLogPal==NULL)
	{
		TRACE(_T("COXDIB::GetHalfTonePalette: Not enough memory for logical  palette\n"));
		CWnd::GetDesktopWindow()->ReleaseDC(pDC);
		return FALSE;
	}

    // get a pointer to the logical palette
	lpLogPal=(LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);

	// set version and number of palette entries
	lpLogPal->palVersion=PALVERSION;
	lpLogPal->palNumEntries=(WORD)256;

    // Copy the Halftone palette into our logical palette
	memcpy(lpLogPal->palPalEntry, aHalfTonePalette, sizeof(aHalfTonePalette));

    // Go ahead and create the palette.  Once it's created,
    // we no longer need the LOGPALETTE, so free it.    
	// -- create the palette and get handle to it
	bResult=pPalette->CreatePalette(lpLogPal);
	::GlobalUnlock((HGLOBAL) hLogPal);
	::GlobalFree((HGLOBAL) hLogPal);
	
#endif

	CWnd::GetDesktopWindow()->ReleaseDC(pDC);
    return bResult;
}

#ifndef NO_DITHER

BOOL COXDIB::HalfToneDitherDIB()
{
	if (m_hDIB==NULL)
		return FALSE;

	PDIB pdibSource=(PDIB)::GlobalLock((HGLOBAL)m_hDIB);

	// Only work on 24-bit sources
	if(GetNumBitsPerPixel((LPSTR)pdibSource) < 24)
	{
		TRACE(_T("COXDIB::SetHalfTone: Setting halftone only works for 24 bit images\n"));
		::GlobalUnlock((HGLOBAL)m_hDIB);
		return FALSE;
	}

	CPalette* pHalfTonePal=new CPalette;
	if (!GetHalfTonePalette(pHalfTonePal))
	{
		TRACE(_T("COXDIB::SetHalfTone: Could not create a halftone palette\n"));
		::GlobalUnlock((HGLOBAL)m_hDIB);

		delete pHalfTonePal; 
		return FALSE;
	}

	BOOL bWinGHalfTone=FALSE;
	// test whether the halftone palette is a WING halftone palette (Win95) or
	// a Win NT halftone palette
	PALETTEENTRY PaletteEntry;
	pHalfTonePal->GetPaletteEntries(1, 1, &PaletteEntry);
	if (NT_REF != RGB(PaletteEntry.peRed, PaletteEntry.peGreen, PaletteEntry.peBlue))
		bWinGHalfTone=TRUE;

	//Create an 8-bit DIB to halftone to
	COXDIB* pHalfToneDIB=new COXDIB;
	if (!pHalfToneDIB->CreateEmptyDIB(8, DibWidth(pdibSource), DibHeight(pdibSource)))
	{
		TRACE(_T("In COXDIB::SetHalfTone(): Could not create an empty but allocated DIB\n"));
		::GlobalUnlock((HGLOBAL)m_hDIB);

		delete pHalfToneDIB; 
		delete pHalfTonePal; 
		return FALSE;
	}

	int Scanline;

	// Fill in the DIB color table with the halftone palette
	pHalfToneDIB->SetPaletteUsage(pHalfTonePal, DIB_RGB_COLORS);
	PDIB pDIBDest=(PDIB)::GlobalLock((HGLOBAL)(pHalfToneDIB->m_hDIB));

	// Step through, converting each pixel in each scan line
	// to the nearest halftone match
	for(Scanline=0; Scanline < (int)DibHeight(pdibSource); Scanline++)
	{
		BYTE _huge* pSourceScanline=(BYTE _huge*)DibXY(pdibSource, 0, Scanline);
		BYTE _huge* pDestScanline=(BYTE _huge*)DibXY(pDIBDest, 0, Scanline);

		int Pixel;

		for(Pixel=0; Pixel < (int)DibWidth(pdibSource); Pixel++)
		{
			// This is the meat of the halftoning algorithm:
			// Convert an RGB into an index into the halftone palette.

			// First, extract the raw RGB information
			int Red=pSourceScanline[Pixel * 3 + 2];
			int Green=pSourceScanline[Pixel * 3 + 1];
			int Blue=pSourceScanline[Pixel * 3];

			// Now, look up each value in the halftone matrix
			// using an 8x8 ordered dither.
			BYTE RedTemp=(BYTE)(aDividedBy51[Red] +
				(aModulo51[Red]>aHalftone8x8[(Pixel%8)* 8 + Scanline%8]));
			BYTE GreenTemp=(BYTE)(aDividedBy51[(BYTE)Green] +
				(aModulo51[Green]>aHalftone8x8[(Pixel%8)*8 + Scanline%8]));
			BYTE BlueTemp=(BYTE)(aDividedBy51[(BYTE)Blue] +
				(aModulo51[Blue]>aHalftone8x8[(Pixel%8)*8 + Scanline%8]));

			// Recombine the halftoned RGB values into a palette index
			BYTE PaletteIndex=(BYTE)(RedTemp + aTimes6[GreenTemp] + aTimes36[BlueTemp]);

			if (!bWinGHalfTone)
				pDestScanline[Pixel]=PaletteIndex;
			else
				// And translate through the WinG Halftone Palette
				// translation vector to give the correct value.
				pDestScanline[Pixel]=aWinGHalftoneTranslation[PaletteIndex];
		}
	}

	::GlobalUnlock((HGLOBAL)m_hDIB);
	::GlobalUnlock((HGLOBAL)(pHalfToneDIB->m_hDIB));

	Empty();
	m_hDIB=pHalfToneDIB->m_hDIB;
	m_pPalette=pHalfTonePal;

	// Clean up temporary DIB.  First reset his HDIB handle, because we already assigned it to
	// this COXDib object, otherwise the destructor would free and destroy it.
	pHalfToneDIB->m_hDIB=NULL;
	delete pHalfToneDIB;

	return TRUE;
}

#endif // NO_DITHER

void COXDIB::InitBitmapInfoHeader (PDIB lpBmInfoHdr,
								   DWORD dwWidth,
								   DWORD dwHeight,
								   WORD nBPP)
{
#ifdef WIN32
	memset (lpBmInfoHdr, 0, sizeof (BITMAPINFOHEADER));
#else
	_fmemset (lpBmInfoHdr, 0, sizeof (BITMAPINFOHEADER));
#endif

	lpBmInfoHdr->biSize			= sizeof (BITMAPINFOHEADER);
	lpBmInfoHdr->biWidth		= dwWidth;
	lpBmInfoHdr->biHeight		= dwHeight;
	lpBmInfoHdr->biPlanes		= 1;
    lpBmInfoHdr->biCompression =BI_RGB;

	if (nBPP <= 1)
		nBPP=1;
	else if (nBPP <= 4)
	{
		nBPP=4;
		lpBmInfoHdr->biClrUsed=16;
	}
	else if (nBPP <= 8)
	{
		nBPP=8;
        lpBmInfoHdr->biClrUsed=256;
	}
	else
		nBPP=24;

	lpBmInfoHdr->biBitCount =nBPP;
	lpBmInfoHdr->biSizeImage=WIDTHBYTES (dwWidth * nBPP) * dwHeight;
}

HDIB COXDIB::BitmapToDIB(HBITMAP hBitmap, CPalette* pPal /*=NULL */)
{
	BITMAP				Bitmap;
	BITMAPINFOHEADER	bmInfoHdr;
	LPBITMAPINFOHEADER	lpbmInfoHdr;
	LPSTR				lpBits;
	HDC					hMemDC;
	CDC*				pDC=NULL;
	CPalette*			pOldPal=NULL;

	// Do some setup -- make sure the Bitmap passed in is valid,
	//  get info on the bitmap (like its height, width, etc.),
	//  then setup a BITMAPINFOHEADER.

	if (!hBitmap)
		return NULL;

	if (!::GetObject (hBitmap, sizeof (Bitmap), (LPSTR) &Bitmap))
		return NULL;

	InitBitmapInfoHeader(&bmInfoHdr, Bitmap.bmWidth, Bitmap.bmHeight,
		WORD(Bitmap.bmPlanes*Bitmap.bmBitsPixel));

	// If DIB was already allocated, dealocate first
	Empty();               

	// Now allocate memory for the DIB.  Then, set the BITMAPINFOHEADER
	//  into this memory, and find out where the bitmap bits go.

	m_hDIB=(HDIB)::GlobalAlloc (GHND, sizeof (BITMAPINFOHEADER) +
		GetPaletteSize((LPSTR) &bmInfoHdr) + bmInfoHdr.biSizeImage);

	if (!m_hDIB)
		return NULL;

	lpbmInfoHdr=(LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) m_hDIB);

	*lpbmInfoHdr=bmInfoHdr;
	lpBits      =FindDIBBits ((LPSTR) lpbmInfoHdr);


	// Now, we need a DC to hold our bitmap.  If the app passed us
	//  a palette, it should be selected into the DC.

	hMemDC=GetDC (NULL);
	pDC=CDC::FromHandle(hMemDC);

	if (pPal != NULL)
	{
		pOldPal=pDC->SelectPalette (pPal, FALSE);
		pDC->RealizePalette();
	}

	// We're finally ready to get the DIB.  Call the driver and let
	//  it party on our bitmap.  It will fill in the color table,
	//  and bitmap bits of our global memory block.

	if (!::GetDIBits (hMemDC,hBitmap,0,Bitmap.bmHeight,lpBits,
		(LPBITMAPINFO) lpbmInfoHdr,DIB_RGB_COLORS))
	{
		::GlobalUnlock (m_hDIB);
		::GlobalFree (m_hDIB);

		m_hDIB=NULL;
	}
	else
		::GlobalUnlock (m_hDIB);

	// Finally, clean up and return.

	if (pOldPal != NULL)
		pDC->SelectPalette (pOldPal, FALSE);

	// Copy Palette
	if (pPal != NULL)
	{
		SetPaletteUsage(pPal, DIB_RGB_COLORS);

		LPSTR lpDIB=(LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
		InitPalette(lpDIB); 
		::GlobalUnlock(m_hDIB);
	}

	::ReleaseDC (NULL, hMemDC);

	return m_hDIB;
}

#ifdef _WIN32

HDIB COXDIB::IconToDIB(HICON hIcon, CSize sizeIcon, CPalette* pPal /*=NULL */)
{
	// If DIB was already allocated, dealocate first
	Empty();               

	CDC SrcDC;
	VERIFY(SrcDC.CreateDC(_T("DISPLAY"),NULL,NULL,NULL));
	CDC memDC;
	CBitmap bitmap;
	VERIFY(memDC.CreateCompatibleDC(&SrcDC));
	VERIFY(bitmap.CreateCompatibleBitmap(&SrcDC,sizeIcon.cx,sizeIcon.cy));
    CBitmap* pOldBitmap=memDC.SelectObject(&bitmap);
    VERIFY(::DrawIconEx(
		memDC.GetSafeHdc(),0,0,hIcon,sizeIcon.cx,sizeIcon.cy,0,NULL,DI_NORMAL));
	memDC.SelectObject(pOldBitmap);
	SrcDC.DeleteDC();
	memDC.DeleteDC();
	return BitmapToDIB((HBITMAP)bitmap,pPal);
}

#endif

BOOL COXDIB::Write(CArchive& ar)
{
	ASSERT_VALID(this);

	BITMAPFILEHEADER bmfHdr; // Header for Bitmap file
	LPBITMAPINFOHEADER lpBI;   // Pointer to DIB info structure
	DWORD dwDIBSize;

	if (m_hDIB==NULL)
	{
		TRACE(_T("COXDIB::Write: DIB not loaded, cannot save\n"));
		return FALSE;
	}

	 // Get a pointer to the DIB memory, the first of which contains
	 // a BITMAPINFO structure
	lpBI=(LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) m_hDIB);
	if (lpBI==NULL)
	{
		TRACE(_T("COXDIB::Write  : Out of memory\n"));
		return FALSE;
	}

	if (!IS_WIN30_DIB(lpBI))
	{
		::GlobalUnlock((HGLOBAL) m_hDIB);
		TRACE(_T("COXDIB::Write  : DIB is an other-style DIB (save not supported)\n"));
		return FALSE;       
	}

	 // Fill in the fields of the file header

	// Fill in file type (first 2 bytes must be "BM" for a bitmap) 
	bmfHdr.bfType=DIB_HEADER_MARKER;  // "BM"

	// Calculating the size of the DIB is a bit tricky (if we want to
	// do it right).  The easiest way to do this is to call GlobalSize()
	// on our global handle, but since the size of our global memory may have
	// been padded a few bytes, we may end up writing out a few too
	// many bytes to the file (which may cause problems with some apps).
	//
	// So, instead let's calculate the size manually (if we can)
	//
	// First, find size of header plus size of color table.  Since the
	// first DWORD in both BITMAPINFOHEADER and BITMAPCOREHEADER conains
	// the size of the structure, let's use this.

	dwDIBSize=*(LPDWORD)lpBI + GetPaletteSize((LPSTR)lpBI);  // Partial Calculation

	// Now calculate the size of the image

	if ((lpBI->biCompression==BI_RLE8) || (lpBI->biCompression==BI_RLE4))
	{
		// It's an RLE bitmap, we can't calculate size, so trust the
		// biSizeImage field

		dwDIBSize += lpBI->biSizeImage;
	}
	else
	{
		DWORD dwBmBitsSize;  // Size of Bitmap Bits only

		// It's not RLE, so size is Width (DWORD aligned) * Height

		dwBmBitsSize=WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) * lpBI->biHeight;

		dwDIBSize += dwBmBitsSize;

		// Now, since we have calculated the correct size, why don't we
		// fill in the biSizeImage field (this will fix any .BMP files which
		// have this field incorrect).

		lpBI->biSizeImage=dwBmBitsSize;
	}


	// Calculate the file size by adding the DIB size to sizeof(BITMAPFILEHEADER)

	bmfHdr.bfSize=dwDIBSize + sizeof(BITMAPFILEHEADER);
	bmfHdr.bfReserved1=0;
	bmfHdr.bfReserved2=0;

	// Now, calculate the offset the actual bitmap bits will be in
	// the file -- It's the Bitmap file header plus the DIB header,
	// plus the size of the color table.
	bmfHdr.bfOffBits=(DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize
											  + GetPaletteSize((LPSTR)lpBI);

	TRY
	{
		// Write the file header
		ar.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
		//
		// Write the DIB header and the bits
		//
		WriteHuge(ar, lpBI, dwDIBSize);
	}
	CATCH (CFileException, e)
	{
		::GlobalUnlock((HGLOBAL) m_hDIB);
		THROW_LAST();
	}
	END_CATCH

	::GlobalUnlock((HGLOBAL) m_hDIB);
	return TRUE;
}

BOOL COXDIB::Write(LPCTSTR pszPath)
{
	CFile file;
	CFileException fe;

	if(!file.Open(pszPath,
		CFile::modeCreate|CFile::modeReadWrite|CFile::shareExclusive,&fe))
	{
		TRACE1("COXDIB::Write : Could not open DIB file : %s\n", pszPath);
		return FALSE;
	}
	
	if (!Write(&file))
	{
		file.Abort(); // will not throw an exception
		TRACE1("COXDIB::Write : Error writing file : %s\n", pszPath);
		return FALSE;
	}

	return TRUE;
}

BOOL COXDIB::Write(CFile* pDIBFile, BOOL bCloseFile/*=TRUE*/)
{
	BOOL bSuccess=FALSE;
	TRY
	{
		CArchive ar(pDIBFile, CArchive::store);
		bSuccess=Write(ar);
		ar.Close();
		if(bCloseFile)
			pDIBFile->Close();
	}
	CATCH (CException, eSave)
	{
		TRACE(_T("COXDIB::Write : Catching file exception while writing file\n"));
		return FALSE;
	}
	END_CATCH

	return bSuccess;
}

// save as JPEG file
#ifdef OXDIB_SUPPORTJPEG
BOOL COXDIB::WriteJPEG(LPCTSTR pszPath)
{
	DWORD LastError=0;

	if(IsEmpty())
		return FALSE;

	BOOL bSuccess=FALSE;

	// mem file for conversion purpose
	CMemFile tmpBMPFile;
	CArchive ar(&tmpBMPFile,CArchive::store);
	bSuccess=Write(ar);
	ar.Close();
	if(!bSuccess)
		return FALSE;

	// go to the beginning of the file
	tmpBMPFile.SeekToBegin();

	// create bitmap file
	COXBMPFile bmpFile(&tmpBMPFile);
	// create empty JPEG file
	COXJPEGFile jpgFile(pszPath);	
	LastError=GetLastError();

	if(!ConvertDIBtoJPEG(&bmpFile,&jpgFile))
		return FALSE;

	SetLastError(LastError);

	return TRUE;
}

BOOL COXDIB::WriteJPEG(CFile* pFile)
{
	ASSERT(pFile!=NULL);

	DWORD LastError=0;

	if(IsEmpty())
		return FALSE;

	BOOL bSuccess=FALSE;

	// mem file for conversion purpose
	CMemFile tmpBMPFile;
	CArchive ar(&tmpBMPFile,CArchive::store);
	bSuccess=Write(ar);
	ar.Close();
	if(!bSuccess)
		return FALSE;

	// go to the beginning of the file
	tmpBMPFile.SeekToBegin();

	// create bitmap file
	COXBMPFile bmpFile(&tmpBMPFile);
	// create empty JPEG file
	COXJPEGFile jpgFile(pFile);	
	LastError=GetLastError();

	if(!ConvertDIBtoJPEG(&bmpFile,&jpgFile))
		return FALSE;

	SetLastError(LastError);

	return TRUE;
}
#endif


PDIB COXDIB::ReadBitmapInfo(CArchive& ar)
{
	BITMAPFILEHEADER bmfHeader;
	PDIB pdibInfo=NULL;

	// Go read the DIB file header and check if it's valid.
	if ((ar.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) !=
		sizeof(bmfHeader)) || (bmfHeader.bfType != DIB_HEADER_MARKER))
	{
		TRACE(_T("COXDIB::ReadBitmapInfo : Invalid bitmap header\n"));
		return NULL;
	}

	// Go read the size of the struct that follows the bitmapfileheader
	DWORD dwSize;
	if (ar.Read(&dwSize, sizeof(dwSize)) != sizeof(dwSize))
	{
		TRACE(_T("COXDIB::ReadBitmapInfo : Invalid bitmap size\n"));
		return NULL;
	}
	
    // what type of bitmap info is this?
    BITMAPCOREHEADER   bmCoreHdr;
    BITMAPINFOHEADER   bminfo;
	switch (dwSize)
	{
	default:
    case sizeof(BITMAPINFOHEADER):
		{
   			// Go read the DIB bitmap info header
			BYTE* pByte=(BYTE*)&bminfo;
			// allready read the size so skip it
			pByte=pByte + sizeof(DWORD);
			if (ar.Read(pByte, sizeof(bminfo) - sizeof(DWORD)) != (sizeof(bminfo) - sizeof(DWORD)))
			{
				TRACE(_T("COXDIB::ReadBitmapInfo : Invalid bitmap info\n"));
				return NULL;
			}

			bminfo.biSize=dwSize;
		}
	    break;

	case sizeof(BITMAPCOREHEADER):
		{
			// Go read the DIB bitmap info header
			BYTE* pByte=(BYTE*)&bmCoreHdr;
			// allready read the size so skip it
			pByte=pByte + sizeof(DWORD);
			if (ar.Read(pByte, sizeof(bmCoreHdr) - sizeof(DWORD)) != (sizeof(bmCoreHdr)-sizeof(DWORD)))
			{
				TRACE(_T("COXDIB::ReadBitmapInfo : Invalid bitmap core header\n"));
				return NULL;
			}

            bminfo.biSize              =sizeof(BITMAPINFOHEADER);
            bminfo.biWidth             =(DWORD)bmCoreHdr.bcWidth;
            bminfo.biHeight            =(DWORD)bmCoreHdr.bcHeight;
            bminfo.biPlanes            = (WORD)bmCoreHdr.bcPlanes;
            bminfo.biBitCount          = (WORD)bmCoreHdr.bcBitCount;
            bminfo.biCompression       =BI_RGB;
            bminfo.biSizeImage         =0;
            bminfo.biXPelsPerMeter     =0;
            bminfo.biYPelsPerMeter     =0;
            bminfo.biClrUsed           =0;
            bminfo.biClrImportant      =0;
		}

		break;
	}
   
    int nNumColors=DibNumColors(&bminfo);

    FixBitmapInfo(&bminfo);

    pdibInfo=(PDIB)GlobalAllocPtr(GMEM_MOVEABLE,(LONG)bminfo.biSize + 
		nNumColors * sizeof(RGBQUAD));

    if (pdibInfo==NULL)
        return NULL;

	// assign contents of bitmap info to dibinfo
	*pdibInfo=bminfo;

	RGBQUAD FAR*	pRgb;
	pRgb=DibColors(pdibInfo);

    if (nNumColors != 0)
	{
        if (dwSize==sizeof(BITMAPCOREHEADER))
		{
            /*
			* convert a old color table (3 byte entries) to a new
			* color table (4 byte entries)
			*/
			if (ar.Read((LPVOID)pRgb, nNumColors * sizeof(RGBTRIPLE)) != 
				(nNumColors * sizeof(RGBTRIPLE)))
			{
				TRACE(_T("COXDIB::ReadBitmapInfo : Invalid RGB color table\n"));
		        GlobalFreePtr(pdibInfo);
				return NULL;
			}
			
            for (int i=nNumColors - 1; i >= 0; i--)
			{
                RGBQUAD rgb;
				
                rgb.rgbRed     =((RGBTRIPLE FAR*)pRgb)[i].rgbtRed;
                rgb.rgbBlue    =((RGBTRIPLE FAR*)pRgb)[i].rgbtBlue;
                rgb.rgbGreen   =((RGBTRIPLE FAR*)pRgb)[i].rgbtGreen;
                rgb.rgbReserved=(BYTE)0;
				
                pRgb[i]=rgb;
			}
		}
        else
		{
			if (ar.Read((LPVOID)pRgb, nNumColors * sizeof(RGBQUAD)) != (nNumColors * sizeof(RGBQUAD)))
			{
				TRACE(_T("COXDIB::ReadBitmapInfo : Invalid RGB color table\n"));
		        GlobalFreePtr(pdibInfo);
				return NULL;
			}
		}
	}

    if (bmfHeader.bfOffBits != 0L)
	{
		TRACE(_T("COXDIB::ReadBitmapInfo : BMP with offset bits\n"));
	}

	return pdibInfo;
}
	

BOOL COXDIB::Read(CArchive& ar)
{
	// If DIB was already allocated, dealocate first
	Empty();

    PDIB pdib=ReadBitmapInfo(ar);
    if (pdib==NULL)
	{
		TRACE(_T("COXDIB::Read : Could not read bitmap info\n"));
		return FALSE;
	}

    // How much memory do we need to hold the DIB
    DWORD dwBits=pdib->biSizeImage;
    DWORD dwLen =pdib->biSize + DibPaletteSize(pdib) + dwBits;

    // Can we get more memory?
    LPVOID pGlobal=GlobalReAllocPtr(pdib, dwLen, 0);

    if (pGlobal==NULL)
	{
        GlobalFreePtr(pdib);
        pdib=NULL;
	}
    else
	{
        pdib=(PDIB)pGlobal;
		m_hDIB=(HDIB)GlobalPtrHandle(pdib);
	}

	if (pdib != NULL)
	{
		// Go read the bits.
		if (ReadHuge(ar, (LPBYTE)pdib + (UINT)pdib->biSize + DibPaletteSize(pdib), dwBits) != dwBits)
		{
	        GlobalFreePtr(pdib);
			m_hDIB=NULL;
			TRACE(_T("COXDIB::Read : Invalid bitmap size\n"));
			return FALSE;
		}
	
		// Initialize the palette
		if (!InitPalette((LPSTR)pdib))
			return FALSE;

        GlobalUnlockPtr(pdib);
	}
	else
	{
		TRACE(_T("COXDIB::Read : Invalid bitmap memory block\n"));
		Empty();
		return FALSE;
	}
				
	ASSERT_VALID(this);
	return TRUE;
}


BOOL COXDIB::Read(LPCTSTR pszPath)
{
	CFile file;
	CFileException fe;
	if(!file.Open(pszPath,CFile::modeRead|CFile::shareDenyWrite,&fe))
	{
		TRACE1("COXDIB::Read : Could not open DIB file : %s\n", pszPath);
		return FALSE;
	}

	if(!Read(&file))
	{
		file.Abort(); // will not throw an exception
		TRACE1("COXDIB::Read : Error reading file : %s\n", pszPath);
		return FALSE;
	}

	return TRUE;
}


BOOL COXDIB::Read(CFile* pDIBFile)
{
	// replace calls to Serialize with Read function
	BOOL bSuccess=FALSE;
	TRY
	{
		CArchive ar(pDIBFile, CArchive::load);
		bSuccess=Read(ar);
		ar.Close();
	}
	CATCH (CFileException, eLoad)
	{
		TRACE(_T("COXDIB::Read : Catching file exception while reading file\n"));
		return FALSE;
	}
	END_CATCH

	return bSuccess;
}


// load JPEG file
#ifdef OXDIB_SUPPORTJPEG

BOOL COXDIB::ReadJPEG(LPCTSTR pszPath)
{
	// mem file for conversion purpose
	CMemFile tmpBMPFile;
	// create bitmap file
	COXBMPFile bmpFile(&tmpBMPFile);
	// create JPEG file object
	COXJPEGFile jpgFile(pszPath);	

	if(!ConvertJPEGtoDIB(&jpgFile,&bmpFile))
		return FALSE;

	tmpBMPFile.SeekToBegin();
	return Read(&tmpBMPFile);
}

BOOL COXDIB::ReadJPEG(CFile* pFile)
{
	ASSERT(pFile!=NULL);

	// mem file for conversion purpose
	CMemFile tmpBMPFile;
	// create bitmap file
	COXBMPFile bmpFile(&tmpBMPFile);
	// create JPEG file object
	COXJPEGFile jpgFile(pFile);	

	if(!ConvertJPEGtoDIB(&jpgFile,&bmpFile))
		return FALSE;

	tmpBMPFile.SeekToBegin();
	return Read(&tmpBMPFile);
}

#endif	//	#ifdef OXDIB_SUPPORTJPEG


void COXDIB::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
		Write(ar);
	else
		Read(ar);
}
	

// static functions for JPEG/DIB conversions
#ifdef OXDIB_SUPPORTJPEG
BOOL COXDIB::ConvertDIBtoJPEG(LPCTSTR pszDIBPath, LPCTSTR pszJPEGPath)
{
	COXDIB dib;
	if(!dib.Read(pszDIBPath))
		return FALSE;
	if(dib.IsEmpty())
		return FALSE;
	return dib.WriteJPEG(pszJPEGPath);
}

BOOL COXDIB::ConvertDIBtoJPEG(COXBMPFile* pDIBFile, COXJPEGFile* pJPEGFile)
{
	ASSERT(pDIBFile!=NULL);
	ASSERT(pJPEGFile!=NULL);

	// object responsible for conversion
	COXJPEGCompressor JCompr;

	short nReturn(1);
	TCHAR ErrorBuffer[SIZE_ERROR_BUF];
	TRY
	{
		// the only line of code we need to convert bitmap to JPEG
		nReturn=JCompr.DoCompress(pDIBFile,pJPEGFile);
		if(nReturn==2)
		{
			TRACE(JCompr.GetWarningMessages());
		}
	}
	CATCH(COXJPEGException, e)
	{
		// if something went wrong then notify about it
		e->GetErrorMessage(ErrorBuffer, SIZE_ERROR_BUF);
		CString sErrorMessage=_T("COXDIB::ConvertDIBtoJPEG: Exception ");
		sErrorMessage+=ErrorBuffer;
		TRACE(sErrorMessage);
	}
	END_CATCH

	return (nReturn==0);
}

BOOL COXDIB::ConvertJPEGtoDIB(LPCTSTR pszJPEGPath, LPCTSTR pszDIBPath)
{
	COXDIB dib;
	if(!dib.ReadJPEG(pszJPEGPath))
		return FALSE;
	if(dib.IsEmpty())
		return FALSE;
	return dib.Write(pszDIBPath);
}

BOOL COXDIB::ConvertJPEGtoDIB(COXJPEGFile* pJPEGFile, COXBMPFile* pDIBFile)
{
	ASSERT(pJPEGFile!=NULL);
	ASSERT(pDIBFile!=NULL);

	// object responsible for conversion
	COXJPEGDecompressor JDecompr;

	short nReturn(1);
	TCHAR ErrorBuffer[SIZE_ERROR_BUF];
	TRY
	{
		// the only line of code that we need to decompress JPEG to bitmap file
		nReturn=JDecompr.DoDecompress(pJPEGFile,pDIBFile);
		if(nReturn==2)
		{
			TRACE(JDecompr.GetWarningMessages());
		}
	}
	CATCH(COXJPEGException, e)
	{
		// if something went wrong then notify about it
		e->GetErrorMessage(ErrorBuffer, SIZE_ERROR_BUF);
		CString sErrorMessage=_T("COXDIB::ConvertJPEGtoDIB: Exception ");
		sErrorMessage+=ErrorBuffer;
		TRACE(sErrorMessage);
	}
	END_CATCH

	if(nReturn!=0)
		return FALSE;

	return TRUE;
}
#endif


BOOL COXDIB::Rotate(int nAngleDegree, BOOL bFlipHorz/*=FALSE*/, 
					BOOL bFlipVert/*=FALSE*/)
{
	if((nAngleDegree%90)!=0)
	{
		TRACE(_T("COXDIB::Rotate: support only 90 degrees rotation step\n"));
		return FALSE;
	}

	if(nAngleDegree<0)
	{
		nAngleDegree+=(nAngleDegree/360)*360;
		nAngleDegree=360-nAngleDegree;
	}
	else
		nAngleDegree-=(nAngleDegree/360)*360;

	if(IsEmpty())
		return FALSE;

	if(nAngleDegree==0 && !bFlipVert && !bFlipHorz)
		return TRUE;

	COXDIB dibCopy=*this;

	Empty();

	LPSTR lpDIBSrc=(LPSTR)::GlobalLock((HGLOBAL)dibCopy.m_hDIB);

	int nWidthSrc=dibCopy.GetSize().cx;
	int nHeightSrc=dibCopy.GetSize().cy;
	WORD nBitsPerPixel=dibCopy.GetNumBitsPerPixel(lpDIBSrc);

	// calculate resulting size of image
	int nWidthDest=((nAngleDegree%180)==0 ? nWidthSrc : nHeightSrc);
	int nHeightDest=((nAngleDegree%180)==0 ? nHeightSrc : nWidthSrc);

	// recreate DIB
	if(!CreateEmptyDIB(nBitsPerPixel,nWidthDest,nHeightDest))
	{
		::GlobalUnlock((HGLOBAL)dibCopy.m_hDIB);
		*this=dibCopy;
		TRACE(_T("COXDIB::Rotate: failed to create empty DIB\n"));
		return FALSE;
	}

	LPSTR lpDIBDest=(LPSTR)::GlobalLock((HGLOBAL)m_hDIB);

	// copy image and rotate pixels
	//
	LPSTR lpDIBBitsSrc=dibCopy.FindDIBBits(lpDIBSrc);
	LPSTR lpDIBBitsDest=FindDIBBits(lpDIBDest);

	int nWidthBytesSrc=WIDTHBYTES(nWidthSrc*(DWORD)nBitsPerPixel);
	int nWidthBytesDest=WIDTHBYTES(nWidthDest*(DWORD)nBitsPerPixel);
	// mask for monochrome DIB 
	BYTE BitMask=0x80;
	// masks for 16-color DIB
	BYTE LoMask=0x0F;
	BYTE HiMask=0xF0;

	// copy color table for non true-color images
	if(nBitsPerPixel<=8)
	{
		// Copy source DIB's color table to destination DIB
		RGBQUAD* lpSrcColor=(RGBQUAD*)(lpDIBSrc+(sizeof(BITMAPINFOHEADER)));
		RGBQUAD* lpDestColor=(RGBQUAD*)(lpDIBDest+(sizeof(BITMAPINFOHEADER)));
		WORD wNumColors=dibCopy.GetNumColors(lpDIBSrc);
		for(int nIndex=0; nIndex<(int)wNumColors; nIndex++)
			*lpDestColor++=*lpSrcColor++;
		// Initialize the palette of the copy
		InitPalette(lpDIBSrc);
	}

	int nBytesPerPixel=(nBitsPerPixel>=8 ? (nBitsPerPixel>>3) : 1);
	int nPixelsPerByte=(nBitsPerPixel>=8 ? 1 : 8/nBitsPerPixel);
	for(int nRow=0; nRow<nHeightSrc; nRow++)
	{
		for(int nCol=0; nCol<nWidthSrc; nCol++)
		{
			for(int nIndex=0; nIndex<nBytesPerPixel; nIndex++)
			{
				int nPositionSrc=nRow*nWidthBytesSrc+
					(nCol/nPixelsPerByte)*nBytesPerPixel+nIndex;
				int nPositionDest=nPositionSrc;
				switch(nAngleDegree)
				{
				case 0:
					nPositionDest=
						(bFlipHorz ? nHeightDest-1-nRow : nRow)*nWidthBytesDest+
						((bFlipVert ? nWidthDest-1-nCol : nCol)/nPixelsPerByte)*
						nBytesPerPixel+nIndex;
					break;
				case 90:
					nPositionDest=
						(bFlipHorz ? nCol : nHeightDest-1-nCol)*nWidthBytesDest+
						((bFlipVert ? nWidthDest-1-nRow : nRow)/nPixelsPerByte)*
						nBytesPerPixel+nIndex;
					break;
				case 180:
					nPositionDest=
						(bFlipHorz ? nRow : nHeightDest-1-nRow)*nWidthBytesDest+
						((bFlipVert ? nCol : nWidthDest-1-nCol)/nPixelsPerByte)*
						nBytesPerPixel+nIndex;
					break;
				case 270:
					nPositionDest=
						(bFlipHorz ? nHeightDest-1-nCol : nCol)*nWidthBytesDest+
						((bFlipVert ? nRow : nWidthDest-1-nRow)/nPixelsPerByte)*
						nBytesPerPixel+nIndex;
					break;
				default:
					ASSERT(FALSE);
				}

				// flag that specifies that bits in the byte should be flipped
				// for monochrome or 16-color bitmaps
				BOOL bFlip=bFlipVert;

				switch(nBitsPerPixel)
				{
				// monochrome bitmap
				case 1:
					{
						// Get the source byte
						BYTE srcByte=lpDIBBitsSrc[nPositionSrc];
						// Get the destination byte
						BYTE destByte=lpDIBBitsDest[nPositionDest];

						// Mask off the bit we want from the src
						srcByte&=(BYTE)(BitMask>>(nCol%nPixelsPerByte));

						switch(nAngleDegree)
						{
						case 180:
							bFlip=!bFlip;
						case 0:
							{
								if(bFlip)
								{
									// Shift src bit to dest bit's position
									srcByte<<=(nCol%nPixelsPerByte);
									srcByte>>=((nWidthDest-nCol-1)%nPixelsPerByte);

									// Zero out only the bit we're going to replace
									destByte&=~(BitMask>>((nWidthDest-nCol-1)%
										nPixelsPerByte));
								}
								else
								{
									// Zero out only the bit we're going to replace
									destByte&=~(BitMask>>(nCol%nPixelsPerByte));
								}
								break;
							}
						case 270:
							bFlip=!bFlip;
						case 90:
							{
								if(bFlip)
								{
									// Shift src bit to dest bit's position
									srcByte<<=(nCol%nPixelsPerByte);
									srcByte>>=((nWidthDest-nRow-1)%
										nPixelsPerByte);

									// Zero out only the bit we're going to replace
									destByte&=~(BitMask>>((nWidthDest-nRow-1)%
										nPixelsPerByte));
								}
								else
								{
									// Shift src bit to dest bit's position
									srcByte<<=(nCol%nPixelsPerByte);
									srcByte>>=(nRow%nPixelsPerByte);

									// Zero out only the bit we're going to replace
									destByte&=~(BitMask>>(nRow%nPixelsPerByte));
								}
								break;
							}
						default:
							ASSERT(FALSE);
						}

						// Set the destination bit
						lpDIBBitsDest[nPositionDest]=(BYTE)(srcByte|destByte);

						break;
					}

				// 16-color bitmap
				case 4:
					{
						// Get the source byte
						BYTE srcByte=lpDIBBitsSrc[nPositionSrc];
						// Get the destination byte
						BYTE destByte=lpDIBBitsDest[nPositionDest];

						if(nCol%2==0)
							srcByte&=HiMask;
						else
							srcByte&=LoMask;

						switch(nAngleDegree)
						{
						case 180:
							bFlip=!bFlip;
						case 0:
							{
								if(bFlip)
								{
									if(nCol%2==0 && (nWidthDest-nCol-1)%2!=0)
										srcByte>>=nBitsPerPixel;
									else if(nCol%2!=0 && (nWidthDest-nCol-1)%2==0)
										srcByte<<=nBitsPerPixel;

									if((nWidthDest-nCol-1)%2==0)
										destByte&=LoMask;
									else
										destByte&=HiMask;
								}
								else
								{
									if(nCol%2==0)
										destByte&=LoMask;
									else
										destByte&=HiMask;
								}
								break;
							}
						case 270:
							bFlip=!bFlip;
						case 90:
							{
								if(bFlip)
								{
									if(nCol%2==0 && (nWidthDest-nRow-1)%2!=0)
										srcByte>>=nBitsPerPixel;
									else if(nCol%2!=0 && (nWidthDest-nRow-1)%2==0)
										srcByte<<=nBitsPerPixel;

									if((nWidthDest-nRow-1)%2==0)
										destByte&=LoMask;
									else
										destByte&=HiMask;
								}
								else
								{
									if(nCol%2==0 && nRow%2!=0)
										srcByte>>=nBitsPerPixel;
									else if(nCol%2!=0 && nRow%2==0)
										srcByte<<=nBitsPerPixel;

									if(nRow%2==0)
										destByte&=LoMask;
									else
										destByte&=HiMask;
								}
								break;
							}
						default:
							ASSERT(FALSE);
						}

						// Set the destination bit
						lpDIBBitsDest[nPositionDest]=(BYTE)(srcByte|destByte);

						break;
					}

				// 256-color and true-color bitmaps
				case 8:
				case 24:
				default:
					{
						lpDIBBitsDest[nPositionDest]=lpDIBBitsSrc[nPositionSrc];
						break;
					}
				}
			}
		}
	}
	//
	//////////////////////////////////////////

	::GlobalUnlock((HGLOBAL)m_hDIB);
	ASSERT(m_hDIB!=NULL);
	::GlobalUnlock((HGLOBAL)dibCopy.m_hDIB);
	ASSERT(dibCopy.m_hDIB!=NULL);

	return TRUE;
}


COXDIB::~COXDIB()
{
	Empty();
}
	
// Diagnostics --------------------------------------------------------------

#ifdef _DEBUG
void COXDIB::AssertValid() const
	{
	CObject::AssertValid();
	if (m_hDIB != NULL)
		{
		// Handle to global memory must be valid
#ifdef WIN32
		BYTE *lp=(BYTE *) ::GlobalLock((HGLOBAL) m_hDIB);
#else
		BYTE huge* lp=(BYTE huge *) ::GlobalLock((HGLOBAL) m_hDIB);
#endif
		ASSERT(lp != NULL);
		::GlobalUnlock((HGLOBAL) m_hDIB);
		}
	if (m_pPalette != NULL)
		{
		// Palette may only exist together with a DIB
		ASSERT(m_hDIB != NULL);
		ASSERT_VALID(m_pPalette);
		}
	}

void COXDIB::Dump(CDumpContext& dc) const
	{
	CObject::Dump(dc);

	dc << TEXT("\nm_hDIB : ") << m_hDIB;
	dc << TEXT("\nm_pPalette : ") << (const void*)m_pPalette;

	}
#endif //_DEBUG

// protected:
BOOL COXDIB::InitPalette(LPSTR lpbi)
	// --- In  :
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : This function creates a palette from a DIB by allocating memory for the
	//				logical palette, reading and storing the colors from the DIB's color table
	//				into the logical palette, creating a palette from this logical palette,
	//				and then returning the palette's handle. This allows the DIB to be
	//				displayed using the best possible colors (important for DIBs with 256 or
	//				more colors).
{
	LPLOGPALETTE lpPal;      // pointer to a logical palette
	HANDLE hLogPal;          // handle to a logical palette
	int i;                   // loop index
	WORD wNumColors;         // number of colors in color table
	LPBITMAPINFO lpbmi;      // pointer to BITMAPINFO structure (Win3.0)
	LPBITMAPCOREINFO lpbmc;  // pointer to BITMAPCOREINFO structure (old)
	BOOL bWinStyleDIB;       // flag which signifies whether this is a Win3.0 DIB
	BOOL bResult=TRUE;     // Initialized with TRUE because when NumColors returns ZERO
							 // ie. when reading a TRUE COLOR DIB, NumColors returns ZERO
							 // and then InitPalette  succeedes because there is no need
							 // for a palette.

	// if handle to DIB is invalid, return FALSE
	if (lpbi==NULL)
	{
		TRACE(_T("COXDIB::InitPalette : DIB not loaded, cannot create palette\n"));
	  	return FALSE;
  	}

   	// get pointer to BITMAPINFO (Win 3.0)
   	lpbmi=(LPBITMAPINFO)lpbi;

   	// get pointer to BITMAPCOREINFO (old 1.x)
   	lpbmc=(LPBITMAPCOREINFO)lpbi;
		
   	// get the number of colors in the DIB
   	wNumColors=GetNumColors(lpbi);

   	if (wNumColors != 0)
   {
		// allocate memory block for logical palette 
		hLogPal=::GlobalAlloc(GHND, sizeof(LOGPALETTE)
									+ sizeof(PALETTEENTRY)
									* wNumColors);

		// if not enough memory, clean up and return NULL 
		if (hLogPal==0)
		{
			TRACE(_T("COXDIB::InitPalette : Not enough memory for logical  palette\n"));
			return FALSE;
		}

		lpPal=(LPLOGPALETTE) ::GlobalLock((HGLOBAL) hLogPal);

		// set version and number of palette entries
		lpPal->palVersion=PALVERSION;
		lpPal->palNumEntries=(WORD)wNumColors;

		// is this a Win 3.0 DIB?
		bWinStyleDIB=IS_WIN30_DIB(lpbi);
		for (i=0; i < (int)wNumColors; i++)
		{
			if (bWinStyleDIB)
			{
				lpPal->palPalEntry[i].peRed=lpbmi->bmiColors[i].rgbRed;
				lpPal->palPalEntry[i].peGreen=lpbmi->bmiColors[i].rgbGreen;
				lpPal->palPalEntry[i].peBlue=lpbmi->bmiColors[i].rgbBlue;
				lpPal->palPalEntry[i].peFlags=0;
			}
			else
			{
				lpPal->palPalEntry[i].peRed=lpbmc->bmciColors[i].rgbtRed;
				lpPal->palPalEntry[i].peGreen=lpbmc->bmciColors[i].rgbtGreen;
				lpPal->palPalEntry[i].peBlue=lpbmc->bmciColors[i].rgbtBlue;
				lpPal->palPalEntry[i].peFlags=0;
			}
		}

		// Cannot initialize the palette twice for the same DIB
		ASSERT(m_pPalette==NULL);
		
		m_pPalette=new CPalette;
		if (m_pPalette==NULL)
		{
			// we must be really low on memory
			TRACE(_T("COXDIB::InitPalette : Out of memory\n"));
			Empty();
			return FALSE;
		}

		// create the palette and get handle to it
		bResult=m_pPalette->CreatePalette(lpPal);
		::GlobalUnlock((HGLOBAL) hLogPal);
		::GlobalFree((HGLOBAL) hLogPal);
	}

	return bResult;
}
	

DWORD COXDIB::ReadHuge(CArchive& ar, void FAR* lpBuffer, DWORD dwCount)
	// --- In  :ar : archive to read from
	//			lpBuffer : pointer to buffer to read
	//			dwCount : Number of bytes to read
	// --- Out : 
	// --- Returns :Number of bytes read
	// --- Effect : Helper function to read large bitmaps (see filex.cpp for original)
{
	DWORD dwToRead=dwCount;
	while (dwToRead>0)
	{
		UINT nRead=CalcSize(dwToRead, lpBuffer);
		UINT nActuallyRead;
		if ((nActuallyRead=ar.Read(lpBuffer, nRead)) < nRead)
			return ((dwCount - dwToRead) + nActuallyRead);
		ASSERT(nActuallyRead==nRead);
		dwToRead -= nRead;
#ifdef WIN32
		lpBuffer=((BYTE*)lpBuffer) + nRead;
#else
		lpBuffer=((BYTE _huge*)lpBuffer) + nRead;
#endif
	}
	return dwCount;
}

void COXDIB::WriteHuge(CArchive& ar, const void FAR* lpBuffer, DWORD dwCount)
	// --- In  :ar : archive to read from
	//			lpBuffer : pointer to buffer to write
	//			dwCount : Number of bytes to write
	// --- Out : 
	// --- Returns :Number of bytes read
	// --- Effect : Helper function to write ead large bitmaps (see filex.cpp for original)
{
	DWORD dwToWrite=dwCount;
	while (dwToWrite>0)
	{
		UINT nWrite=CalcSize(dwToWrite, lpBuffer);
		ar.Write(lpBuffer, nWrite);
		dwToWrite -= nWrite;
#ifdef WIN32
		lpBuffer=((const BYTE*)lpBuffer) + nWrite;
#else
		lpBuffer=((const BYTE _huge*)lpBuffer) + nWrite;
#endif
	}
}

UINT COXDIB::CalcSize(DWORD cbTotal, const void FAR* lpStart)
	// --- In  :
	// --- Out : 
	// --- Returns :Number of bytes to read or write
	// --- Effect : Helper function
{
	// return size to read/write (16K max unless limited by segment bounds)
#ifdef WIN32
	lpStart;
	DWORD cb=cbTotal;
#else
	DWORD cb=0x10000L - _AFX_FP_OFF(lpStart);
	if (cb>cbTotal)
		cb=cbTotal;
#endif
	return (cb>16384) ? 16384 : (UINT)cb;
}

HDIB WINAPI COXDIB::CopyHandle(HDIB hDIB, DWORD dwExtraBytes /*=0 */)
	// --- In  : h :Handle to global memory to duplicate.
	// --- Out : 
	// --- Returns :Returns a handle to the new memory block (NULL on error)
	// --- Effect : Makes a copy of the given global memory block.  
{
#ifdef WIN32
	BYTE       *lpCopy;
	BYTE       *lp;
	HDIB       hCopy;
	DWORD_PTR  dwLen;
#else
	BYTE huge *lpCopy;
	BYTE huge *lp;
	HDIB       hCopy;
	DWORD      dwLen;
#endif
	if (hDIB==NULL)
		return NULL;

	dwLen=::GlobalSize((HGLOBAL) hDIB) + dwExtraBytes;

	if ((hCopy=(HDIB) ::GlobalAlloc(GHND, dwLen)) != NULL)
	{
#ifdef WIN32
		lpCopy=(BYTE *) ::GlobalLock((HGLOBAL) hCopy);
		lp    =(BYTE *) ::GlobalLock((HGLOBAL) hDIB);
#else
		lpCopy=(BYTE huge *) ::GlobalLock((HGLOBAL) hCopy);
		lp    =(BYTE huge *) ::GlobalLock((HGLOBAL) hDIB);
#endif
		while (dwLen--)
			*lpCopy++=*lp++;

		::GlobalUnlock((HGLOBAL) hCopy);
		::GlobalUnlock((HGLOBAL) hDIB);
	}
	else
	{
		TRACE1("COXDIB::CopyHandle : Could not GlobalAlloc( ,%i)\n", dwLen);
	}
	return hCopy;
}
	

DWORD COXDIB::CalcPadding(DWORD dwBitsPerPixel, DWORD dwPixels)
{
    DWORD dwBits=dwBitsPerPixel*dwPixels;
	DWORD dwPadBits=0;

	int nBitsInDWORD=sizeof(DWORD)<<3;
    
    if((dwBits%nBitsInDWORD)==0)
        // already DWORD aligned, no padding needed
        return 0;
    else
        dwPadBits=nBitsInDWORD-(dwBits%nBitsInDWORD);
    
    return (dwPadBits/8+(((dwPadBits%8)>0) ? 1 : 0));
}


// private:

// ==========================================================================
