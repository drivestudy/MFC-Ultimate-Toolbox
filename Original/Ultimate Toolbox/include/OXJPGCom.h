// ==========================================================================
// 							Class Specification : COXJPEGCompressor
// ==========================================================================

// Header file : OXJPGCom.h

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
//	YES	Derived from COXJPEGCodec

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO  Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	YES	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This class encapsulates the parameters of compression of JPEG images and a compress
//  method

// Remark:
//	

// Prerequisites (necessary conditions):
//	

/////////////////////////////////////////////////////////////////////////////
#ifndef __JPEGCOMP_H__
#define __JPEGCOMP_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include "oxjpgcod.h"


class COXGraphicFile;
class COXJPEGFile;
	
class OX_CLASS_DECL COXJPEGCompressor : public COXJPEGCodec
{
// Data Members
public:

protected:
	// Common switches
	UINT				m_nQuality;
	BOOL				m_bGrayScale;
	BOOL				m_bOptimize;
	BOOL				m_bProgressive;

	// Advanced switches
	EDiscreteCosTransf	m_eDisCosTransf;
	UINT				m_nSmooth;
	UINT				m_nMaxMem;

	// Wizard switches
	BOOL				m_bBaseLine;

private:

// Member Functions
public:
	COXJPEGCompressor();  
	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object

	// Common switches
	void SetQuality(UINT nQuality);

	UINT GetQuality()
		{ return m_nQuality;}

	void SetGrayScale(BOOL bGrayScale)
		{ m_bGrayScale = bGrayScale;}

	BOOL GetGrayScale()
		{ return m_bGrayScale;}

	void SetOptimize(BOOL bOptimize);

	BOOL GetOptimize()
		{ return m_bOptimize;}

	void SetProgressive(BOOL bProgressive);
	
	BOOL GetProgressive()
		{ return m_bProgressive;}

	// Advanced switches
	void SetDisCosTranf(EDiscreteCosTransf eDisCosTransf);

	EDiscreteCosTransf GetDisCosTranf()
		{ return m_eDisCosTransf;}

	void SetSmooth(UINT nSmooth);

	UINT GetSmooth()
		{ return m_nSmooth;}

	void SetMaxMemory(UINT nMaxMem)
		{ m_nMaxMem = nMaxMem;}

	UINT GetMaxMemory()
		{ return m_nMaxMem;}

	// Wizard switches
	void SetBaseLine(BOOL bBaseLine)
		{ m_bBaseLine = bBaseLine;}

	BOOL GetBaseLine()
		{ return m_bBaseLine;}

	virtual short DoCompress(COXGraphicFile* pGraphicsFile, COXJPEGFile* pJPEGFile);
	// --- In  : pGraphicsFile : pointer to a BMP, GIF, TARGA or RLE type of graphics file
	//			 pJPEGFile : pointer to a JPEG File
	// --- Out : 
	// --- Returns : succeeded or not
	// --- Effect : compresses the graphics file to the specified JPEG file.

#ifdef _DEBUG
	virtual void Dump(CDumpContext&) const;
	virtual void AssertValid() const;
#endif //_DEBUG

	virtual ~COXJPEGCompressor();  
	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object

protected:
	virtual void ProcessSwitches(j_compress_ptr cinfo, BOOL bForReal);

private:

};

#endif //__JPEGCOMP_H__
