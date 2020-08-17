// ==========================================================================
// 							Class Specification : COXBMPFile
// ==========================================================================

// Header file : OXBMPFle.h

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
//	YES	Derived from COXGraphicsFile

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO  Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	YES	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This class encapsulates the reading and writing of BMP files
// Remark:
//	

// Prerequisites (necessary conditions):
//	

/////////////////////////////////////////////////////////////////////////////
#ifndef __BMPFILE_H__
#define __BMPFILE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include "OXGphFle.h"	


class OX_CLASS_DECL COXBMPFile : public COXGraphicFile
{
friend class COXJPEGCompressor;
friend class COXJPEGDecompressor;

// Data Members
public:

protected:
	// Shared members
	jvirt_sarray_ptr	m_WholeImage;	
	JDIMENSION			m_RowWidth;		

	// Specific Compress members
	j_compress_ptr		m_cinfo;
	JSAMPARRAY			m_Colormap;		
	JDIMENSION			m_SourceRow;	
	int					m_BitsPerPixel;	
	JDIMENSION			(COXBMPFile::*m_pfnGetPixelRows)(j_compress_ptr cinfo);

	// Specific Decompress members
	BOOL				m_bIsOS2;		/* saves the OS2 format request flag */
	JDIMENSION			m_DataWidth;	/* JSAMPLEs per row */
	int					m_nPadBytes;	/* number of padding bytes needed per row */
	JDIMENSION			m_CurOutputRow;	/* next row# to write to virtual array */
	void				(COXBMPFile::*m_pfnPutPixelRows)(j_decompress_ptr cinfo, JDIMENSION rows_supplied);
	
private:

// Member Functions
public:
	COXBMPFile(CString sFullPath);  
	// --- In  : sFullPath : the full path subdirs + name + extender of the graphic file
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object
	//				File will not be opened yet. Must be a BMP file 

	COXBMPFile(CFile* pGraphicFile);  
	// --- In  : pGraphicFile : a pointer to a CFile.  must already be open
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object
	//				Could be used to pass a CMemFile as parameter. Must be a BMP file 

	void SetOS2Format(BOOL bIsOS2)
		{ m_bIsOS2 = bIsOS2;}

	BOOL GetOS2Format()
		{ return m_bIsOS2;}

#ifdef _DEBUG
	virtual void Dump(CDumpContext&) const;
	virtual void AssertValid() const;
#endif //_DEBUG

	virtual ~COXBMPFile();  
	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object

protected:
	virtual BOOL StartInput(j_compress_ptr cinfo);
	virtual BOOL FinishInput(j_compress_ptr cinfo);
	virtual JDIMENSION GetPixelRows(j_compress_ptr cinfo);

	virtual BOOL StartOutput(j_decompress_ptr cinfo);
	virtual BOOL FinishOutput(j_decompress_ptr cinfo);
	virtual BOOL PutPixelRows(j_decompress_ptr cinfo, JDIMENSION rowsSupplied);

	virtual BOOL InitRead(j_compress_ptr cinfo);
	virtual BOOL InitWrite(j_decompress_ptr cinfo, BOOL bExtra = FALSE);

	int ReadByte();
	void ReadColorMap(int cmaplen, int mapentrysize);
	JDIMENSION Get8bitRow(j_compress_ptr cinfo);
	JDIMENSION Get24bitRow(j_compress_ptr cinfo);
	JDIMENSION PreloadImage(j_compress_ptr cinfo);

	void PutGrayRows(j_decompress_ptr cinfo, JDIMENSION rows_supplied);
	void Put24PixelRows(j_decompress_ptr cinfo, JDIMENSION rows_supplied);
	void WriteBMPHeader(j_decompress_ptr cinfo);
	void WriteOS2Header(j_decompress_ptr cinfo);
	void WriteColorMap (j_decompress_ptr cinfo, int map_colors, int map_entry_size);

private:

};

#endif //__BMPFILE_H__
