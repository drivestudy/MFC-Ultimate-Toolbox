// ==========================================================================
// 					Class Implementation : COXGraphics
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "afxtempl.h"
#include <math.h>

#include "OXGraphics.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COXGraphics::COXGraphics()
{
}

COXGraphics::~COXGraphics()
{
}


DWORD* COXGraphics::PrepareSphere(CDC *pDC, 
								  int nLeft, int nTop,		// coordinates of the button
								  int nExternalRadius,
								  int nInternalRadius,
								  COLORREF clrLightSource,
								  float Lx, float Ly, float Lz,
								  float fLightIntensity,	//power of light source
								  int nPhong,
								  float fMirror, float fDiffuse, float fAmbient,
								  COLORREF clrButton,		//color for filing the bitmap
								  COLORREF clrBack) 
{
	ASSERT(pDC!=NULL);

	float Nx, Ny, Nz, cosThetta;
	int myR, myG, myB;

	float R=(float) GetRValue(clrButton);
	float G=(float) GetGValue(clrButton);
	float B=(float) GetBValue(clrButton);

	float LightR=(float) GetRValue(clrLightSource);
	float LightG=(float) GetGValue(clrLightSource);
	float LightB=(float) GetBValue(clrLightSource);

	float amb_lth, mirr;
	float rz, cosAlphaN;

	BOOL bUseExistingBackground=(clrBack==CLR_NONE);
	clrBack=0xff000000 | (((GetRValue(clrBack)) << 16) | 
		((GetGValue(clrBack)) << 8) | GetBValue(clrBack));

	// calculate color of flat part in the middle
	DWORD FlatColor;
	
	cosThetta=-Lz;
	amb_lth=fAmbient;

	//add mirror component
	mirr=0.f; // remove from circle ???
	if (cosThetta>0) 
	{
		amb_lth += fLightIntensity*fDiffuse*cosThetta;
		rz=Lz+2*cosThetta;
		if (rz>0) 
		{
			cosAlphaN=rz;
			for (int pow=1; pow<2*nPhong-1; pow++) 
				cosAlphaN *= rz;
			mirr=(fMirror*fLightIntensity*cosAlphaN);
		}
	} 
	
	myR=(int) (R*amb_lth+mirr*LightR);
	if (myR<0) myR=0;
	else if (myR>255) myR=255;
	myG=(int) (G*amb_lth+mirr*LightG);
	if (myG<0) myG=0;
	else if (myG>255) myG=255;
	myB=(int) (B*amb_lth+mirr*LightB);
	if (myB<0) myB=0;
	else if (myB>255) myB=255;
	
	FlatColor=((((BYTE)myR) << 16) | (((BYTE)myG) << 8) | (BYTE)myB);
	
	
	//process the "sphere"
	int width=2*nExternalRadius+1;
	int height=width;
	
	DWORD *SphereBitmap=new DWORD[width*height];

	int X, Y;
	float coef, RXY;
	
	for (Y=-nExternalRadius; Y<=nExternalRadius; Y++) 
	{
		DWORD *SphereRow=SphereBitmap+(nExternalRadius+Y)*width;

		for (X=-nExternalRadius; X<=nExternalRadius; X++) 
		{
			RXY=(float) sqrt((float) (X*X+Y*Y));

			if (RXY>nInternalRadius && RXY<=nExternalRadius) 
			{
				coef=(1.f-nInternalRadius/RXY)/(nExternalRadius-nInternalRadius);
				Nx=coef*X;
				Ny=coef*Y;
				Nz=(float) sqrt(1.f-Nx*Nx-Ny*Ny);
				
				cosThetta=-(Lx*Nx+Ly*Ny+Lz*Nz);
				amb_lth=fAmbient;

				//add mirror component
				mirr=0.f; // remove from circle ???
				if (cosThetta>0) {
					amb_lth += fLightIntensity*fDiffuse*cosThetta;
					rz=Lz+2*Nz*cosThetta;
					if (rz>0) {
						cosAlphaN=rz;
						for (int pow=1; pow<2*nPhong-1; pow++) 
							cosAlphaN *= rz;
						mirr=(fMirror*fLightIntensity*cosAlphaN);
					}
				} 
				
				myR=(int) (R*amb_lth+mirr*LightR);
				if (myR<0) myR=0;
				else if (myR>255) myR=255;
				myG=(int) (G*amb_lth+mirr*LightG);
				if (myG<0) myG=0;
				else if (myG>255) myG=255;
				myB=(int) (B*amb_lth+mirr*LightB);
				if (myB<0) myB=0;
				else if (myB>255) myB=255;
				
				SphereRow[nExternalRadius+X]=
					((((BYTE)myR) << 16) | (((BYTE)myG) << 8) | (BYTE)myB);
			}
			else 
			{ // flat internal part
				//Nx=0.f;
				//Ny=0.f;
				//Nz=1.f;
				if (RXY<=nExternalRadius)
				{
					SphereRow[nExternalRadius+X]=FlatColor;
				}
				else 
				{
					if(!bUseExistingBackground)
						SphereRow[nExternalRadius+X]=clrBack;
					else
						SphereRow[nExternalRadius+X]=
							pDC->GetPixel(nExternalRadius+X+nLeft,
							nExternalRadius+Y+nTop);
				}
			}

		} // next X
	} // next Y

	return SphereBitmap;
}


#ifndef PALETTE_MAX_ENTRY_COUNT
#define PALETTE_MAX_ENTRY_COUNT			256
#endif
#ifndef PALVERSION
#define PALVERSION			0x300
#endif

BOOL COXGraphics::BuildButtonPalette(CDC *pDC, DWORD *bitmap, 
									 int width, int height, 
									 CPalette* pPalette)
{
	ASSERT(pDC!=NULL);
	ASSERT(pPalette!=NULL);

	if((HPALETTE)pPalette!=NULL)
		pPalette->DeleteObject();

	CPalette* pCurrentPalette=pDC->GetCurrentPalette();
	int nExistingColorCount=pCurrentPalette->GetEntryCount();
	int nUniqueColorCount=0;
	CMap<DWORD,DWORD,int,int> mapColors;
	int nIndex=0;
	for(nIndex=0; nIndex<width*height; nIndex++)      
	{
		COLORREF clr=bitmap[nIndex]&0x00ffffff;
		int nMapIndex;
		if(!mapColors.Lookup(clr,nMapIndex))
		{
			mapColors.SetAt(clr,nUniqueColorCount);
			nUniqueColorCount++;
		}
	}   

	ASSERT(nUniqueColorCount>0);
	int nColorsToAdd=__min(nUniqueColorCount,
		PALETTE_MAX_ENTRY_COUNT-nExistingColorCount);

    // Create the palette
    struct {
        LOGPALETTE    LogPalette;
        PALETTEENTRY  PalEntry[PALETTE_MAX_ENTRY_COUNT];
    } pal;

    LOGPALETTE* pLogPalette=(LOGPALETTE*)&pal;
    pLogPalette->palVersion=PALVERSION;
	pLogPalette->palNumEntries=(WORD)(nColorsToAdd+nExistingColorCount); 

	int nPaletteIndex=pCurrentPalette->GetPaletteEntries(0,nExistingColorCount,
		pLogPalette->palPalEntry);
	ASSERT(nPaletteIndex==nExistingColorCount);

	if(nColorsToAdd>0)
	{
		int nStep=__max(1,nUniqueColorCount/nColorsToAdd);

		POSITION pos=mapColors.GetStartPosition();
		while(pos!=NULL)      
		{
			COLORREF clr;
			mapColors.GetNextAssoc(pos,clr,nIndex);
			if(nIndex%nStep==0)
			{
				pLogPalette->palPalEntry[nPaletteIndex].peRed=GetBValue(clr);   	  
				pLogPalette->palPalEntry[nPaletteIndex].peGreen=GetGValue(clr); 	    
				pLogPalette->palPalEntry[nPaletteIndex].peBlue=GetRValue(clr);  	   
				pLogPalette->palPalEntry[nPaletteIndex].peFlags=0;  
				nPaletteIndex++;
			}
		}   	  
	}
	ASSERT(nPaletteIndex==nColorsToAdd+nExistingColorCount);

	mapColors.RemoveAll();


	BOOL bResult=pPalette->CreatePalette(pLogPalette);

	return bResult;
}


void COXGraphics::DrawBitmap(CDC *pDC, int nLeft, int nTop, 
							 int width, int height, 
							 DWORD *bitmap, CPalette* pPalette)
{
	
	BOOL bUsePalette=(pDC->GetDeviceCaps(SIZEPALETTE)<=256 && 
		pDC->GetDeviceCaps(SIZEPALETTE)>0);

	CPalette* pOldPalette=NULL;
	if(bUsePalette)
	{
		CPalette palette;
		VERIFY(BuildButtonPalette(pDC,bitmap,width,height,
			(pPalette!=NULL ? pPalette : &palette)));
		pOldPalette=pDC->SelectPalette((pPalette!=NULL ? pPalette : &palette),FALSE);
		pDC->RealizePalette();
	}
	
	BITMAPINFO bmi;
	memset(&bmi,0,sizeof(BITMAPINFO));

	bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth=width;
	bmi.bmiHeader.biHeight=height;
	bmi.bmiHeader.biPlanes=1;
	bmi.bmiHeader.biBitCount=32; 
	bmi.bmiHeader.biCompression=BI_RGB;

	BOOL bSuccess =
		::SetDIBitsToDevice(pDC->m_hDC,
			nLeft,	// x-coordinate of upper-left corner of dest. rect. 
			nTop,	// y-coordinate of upper-left corner of dest. rect. 
			width,	// source rectangle width 
			height,	// source rectangle height 
			0,	// x-coordinate of lower-left corner of source rect. 
			0,	// y-coordinate of lower-left corner of source rect. 
			0,	// first scan line in array 
			height,	// number of scan lines 
			bitmap,	// DIB bits 
			&bmi,	// bitmap info structure  
			DIB_RGB_COLORS		// RGB or palette indices 
		);
	ASSERT(bSuccess);

	if(pOldPalette!=NULL)
	{
		pDC->SelectPalette(pOldPalette,FALSE);
	}
}


DWORD* COXGraphics::PrepareButton(int nButtonWidth, int nButtonHeight, 
								  DWORD *SphereBitmap, int SphereRadius)
{
	//		---------------------
	//		|1	|	7		|2	|
	//		|	|			|	|			There are btmap regions for button and sphere.
	//		---------------------			The sizes of regions 1-4 are the same.
	//		|5	|	9		|6	|           The region 9 for sphere is one (center) pixel.
	//		|	|			|	|			The regions 5-8 for sphere are central lines (horizontal and vertical)
	//		---------------------			
	//		|3	|	8		|4	|			
	//		|	|			|	|
	//		---------------------

	DWORD *ButtonBitmap=new DWORD[nButtonWidth*nButtonHeight];

	int dH=nButtonHeight-2*SphereRadius;
	int dW=nButtonWidth-2*SphereRadius;

	int i, j, ss, bs;
	ss=2*SphereRadius+1;
	bs=nButtonWidth;
	
	//region 1
	for (i=0; i<SphereRadius; i++)
		memcpy(ButtonBitmap+i*bs, SphereBitmap+i*ss, SphereRadius*sizeof(DWORD));
	
	//region 2
	for (i=0; i<SphereRadius; i++)
		memcpy(ButtonBitmap+(i+1)*bs-SphereRadius, 
			SphereBitmap+(i+1)*ss-SphereRadius, SphereRadius*sizeof(DWORD));
	
	//region 3
	for (i=0; i<SphereRadius; i++)
		memcpy(ButtonBitmap+(nButtonHeight-i-1)*bs, 
			SphereBitmap+(ss-i-1)*ss, SphereRadius*sizeof(DWORD));
	
	//region 4
	for (i=0; i<SphereRadius; i++)
		memcpy(ButtonBitmap+(nButtonHeight-i)*bs-SphereRadius,
			SphereBitmap+(ss-i)*ss-SphereRadius, SphereRadius*sizeof(DWORD));
	
	//region 5
	for (i=SphereRadius; i<SphereRadius+dH; i++)
		memcpy(ButtonBitmap+i*bs, SphereBitmap+(SphereRadius+1)*ss, 
			SphereRadius*sizeof(DWORD));

	//region 6
	for (i=SphereRadius; i<SphereRadius+dH; i++)
		memcpy(ButtonBitmap+(i+1)*bs-SphereRadius, 
			SphereBitmap+(SphereRadius+2)*ss-SphereRadius, 
			SphereRadius*sizeof(DWORD));

	//region 7
	for (i=0; i<SphereRadius; i++) 
	{
		int bpos=i*bs;
		int spos=i*ss;
		DWORD color=SphereBitmap[spos+SphereRadius+1];
		for (j=SphereRadius; j<SphereRadius+dW; j++)
			ButtonBitmap[bpos+j]=color;
	}
	
	//region 8
	for (i=0; i<SphereRadius; i++) 
	{
		int bpos=(nButtonHeight-i-1)*bs;
		int spos=(ss-i-1)*ss;
		DWORD color=SphereBitmap[spos+SphereRadius+1];
		for (j=SphereRadius; j<SphereRadius+dW; j++)
			ButtonBitmap[bpos+j]=color;
	}

	//region 9
	for (i=SphereRadius; i<SphereRadius+dH; i++) 
	{
		int bpos=i*bs;
		DWORD color=SphereBitmap[(SphereRadius+1)*(ss+1)];
		for (j=SphereRadius; j<SphereRadius+dW; j++)
			ButtonBitmap[bpos+j]=color;
	}
	
	return ButtonBitmap;
}


#ifndef PI
#define PI 3.14159f
#endif

void COXGraphics::DrawRoundedButton(CDC *pDC,
									int nLeft, int nTop,	// coordinates of the button
									int nButtonWidth,		// button width
									int nButtonHeight,		// button height
									int nExternalRadius,	// radius of flat center and rounded border
									int nInternalRadius,	// radius of flat part
									CPalette* pPalette,
									COLORREF clrButton,		// solid color of sphere without lighting
									COLORREF clrLightSource,// color of light
									float fThetta,			// vertical angle
									float fPhi,				// horizontal angle
									float fLightIntensity,	// influence on diffuse lighting and spot intensity
									int nPhong,				// influence on spot size
									float fMirror,			// influence on spot intensity
									float fDiffuse,			// diffuse lighting
									float fAmbient,			// ambient lighting
									COLORREF clrBack)		// background color
{
	DWORD *SphereBitmap, *ButtonBitmap;

	// light direction
	float Lx, Ly, Lz;
	
	float ct=(float)cos(fThetta*PI/180.f);
	float st=(float)sin(fThetta*PI/180.f);
	
	Lz=-ct;
	Lx=st*(float) cos(fPhi*PI/180.f);
	Ly=st*(float) sin(fPhi*PI/180.f);

	SphereBitmap=PrepareSphere(pDC,nLeft,nTop,nExternalRadius,nInternalRadius,
		clrLightSource,Lx,Ly,Lz,fLightIntensity, 
		nPhong,fMirror,fDiffuse,fAmbient,clrButton,clrBack);
	
	ButtonBitmap=PrepareButton(nButtonWidth,nButtonHeight,SphereBitmap,nExternalRadius);  
		
	DrawBitmap(pDC,nLeft,nTop,nButtonWidth,nButtonHeight,ButtonBitmap,pPalette);
	
	delete SphereBitmap;
	delete ButtonBitmap;
}




