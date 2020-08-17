// ==========================================================================
// 					Class Implementation : COXBMPFile
// ==========================================================================

// Source file : OXBMPFle.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                         
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXBMPFle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

#ifdef BMP_SUPPORTED

/* Macros to deal with unsigned chars as efficiently as compiler allows */

#ifdef HAVE_UNSIGNED_CHAR
typedef unsigned char U_CHAR;
#define UCH(x)	((int) (x))
#else /* !HAVE_UNSIGNED_CHAR */
#ifdef CHAR_IS_UNSIGNED
typedef char U_CHAR;
#define UCH(x)	((int) (x))
#else
typedef char U_CHAR;
#define UCH(x)	((int) (x) & 0xFF)
#endif
#endif /* HAVE_UNSIGNED_CHAR */


/////////////////////////////////////////////////////////////////////////////
// Definition of static members


// Data members -------------------------------------------------------------
// protected:

//	j_compress_ptr		m_cinfo;
//	---	back link saves passing separate parm		

//	JSAMPARRAY			m_Colormap;
//	---	BMP colormap (converted to my format)		

//	jvirt_sarray_ptr	m_WholeImage;
//	---	Needed to reverse row order		

//	DIMENSION			m_SourceRow;
//	---	Current source row number		

//	JDIMENSION			m_RowWidth;
//	---	Physical width of scanlines in file		

//	int					m_BitsPerPixel
//	---	remembers 8- or 24-bit format		

//	BOOL				m_bIsOS2;	
//	---	saves the OS2 format request flag

//	JDIMENSION			m_DataWidth;
//	---	JSAMPLEs per row

//	int					m_nPadBytes;
//	---	number of padding bytes needed per row

//	JDIMENSION			m_CurOutputRow;	
//	---	next row# to write to virtual array

	// private:

// Member functions ---------------------------------------------------------
// public:

COXBMPFile::COXBMPFile(CString sFullPath)
	: COXGraphicFile(sFullPath),
	m_cinfo(NULL),
	m_Colormap(NULL),
	m_WholeImage(NULL),
	m_SourceRow(0),
	m_RowWidth(0),
	m_BitsPerPixel(0),
	m_bIsOS2(FALSE)
	{
	}

COXBMPFile::COXBMPFile(CFile* pGraphicFile)
	: COXGraphicFile(pGraphicFile),
	m_cinfo(NULL),
	m_Colormap(NULL),
	m_WholeImage(NULL),
	m_SourceRow(0),
	m_RowWidth(0),
	m_BitsPerPixel(0),
	m_bIsOS2(FALSE)
	{
	}

#ifdef _DEBUG
void COXBMPFile::Dump(CDumpContext& dc) const
	{
	COXGraphicFile::Dump(dc);

	dc << TEXT("\nm_SourceRow : ") << 		m_SourceRow;
	dc << TEXT("\nm_RowWidth : ") << 	m_RowWidth;
	dc << TEXT("\nm_BitsPerPixel : ") <<	m_BitsPerPixel;
	dc << TEXT("\nm_bIsOS2 ") << 	(WORD)m_bIsOS2;

	dc << TEXT("\nm_cinfo ") << 	(void*)m_cinfo;
	dc << TEXT("\nm_Colormap ") << 	(void*)m_Colormap;
	dc << TEXT("\nm_WholeImage ") << 	(void*)m_WholeImage;
	}

void COXBMPFile::AssertValid() const
	{
	COXGraphicFile::AssertValid();
	}
#endif

COXBMPFile::~COXBMPFile()
	{
	}

// protected

int COXBMPFile::ReadByte()
	/* Read next byte from BMP file */
	{
	BYTE b;
	
	if ((ReadData(&b, 1)) == 0)
		ERREXIT(m_cinfo, JERR_INPUT_EOF);
	
	return b;
	}

void COXBMPFile::ReadColorMap(int cmaplen, int mapentrysize)
	/* Read the colormap from a BMP file */
	{
	int i;
	
	switch (mapentrysize)
		{
		case 3:
			/* BGR format (occurs in OS/2 files) */
			for (i = 0; i < cmaplen; i++)
				{
				m_Colormap[2][i] = (JSAMPLE) ReadByte();
				m_Colormap[1][i] = (JSAMPLE) ReadByte();
				m_Colormap[0][i] = (JSAMPLE) ReadByte();
				}
			break;
		case 4:
			/* BGR0 format (occurs in MS Windows files) */
			for (i = 0; i < cmaplen; i++)
				{
				m_Colormap[2][i] = (JSAMPLE) ReadByte();
				m_Colormap[1][i] = (JSAMPLE) ReadByte();
				m_Colormap[0][i] = (JSAMPLE) ReadByte();
				
				(void) ReadByte();
				}
			break;
		default:
			ERREXIT(m_cinfo, JERR_BMP_BADCMAP);
			break;
		}
	}

/*
 * Read one row of pixels.
 * The image has been read into the whole_image array, but is otherwise
 * unprocessed.  We must read it out in top-to-bottom row order, and if
 * it is an 8-bit image, we must expand colormapped pixels to 24bit format.
 */

JDIMENSION COXBMPFile::Get8bitRow(j_compress_ptr cinfo)
	/* This version is for reading 8-bit colormap indexes */
	{
	register JSAMPARRAY colormap = m_Colormap;
	JSAMPARRAY image_ptr;
	register int t;
	register JSAMPROW inptr, outptr;
	register JDIMENSION col;

	/* Fetch next row from virtual array */
	m_SourceRow--;
	image_ptr = (*cinfo->mem->access_virt_sarray)((j_common_ptr) cinfo, m_WholeImage,
		m_SourceRow, (JDIMENSION) 1, FALSE);

	/* Expand the colormap indexes to real data */
	inptr = image_ptr[0];
	outptr = m_buffer[0];
	for (col = cinfo->image_width; col > 0; col--)
		{
		t = GETJSAMPLE(*inptr++);
		*outptr++ = colormap[0][t];	/* can omit GETJSAMPLE() safely */
		*outptr++ = colormap[1][t];
		*outptr++ = colormap[2][t];
		}

	return 1;
	}


JDIMENSION COXBMPFile::Get24bitRow(j_compress_ptr cinfo)
	/* This version is for reading 24-bit pixels */
	{
	JSAMPARRAY image_ptr;
	register JSAMPROW inptr, outptr;
	register JDIMENSION col;

	/* Fetch next row from virtual array */
	m_SourceRow--;
	image_ptr = (*cinfo->mem->access_virt_sarray)((j_common_ptr) cinfo, m_WholeImage,
		m_SourceRow, (JDIMENSION) 1, FALSE);

	/* Transfer data.  Note source values are in BGR order
	* (even though Microsoft's own documents say the opposite).
	*/
	inptr = image_ptr[0];
	outptr = m_buffer[0];
	for (col = cinfo->image_width; col > 0; col--)
		{
		outptr[2] = *inptr++;	/* can omit GETJSAMPLE() safely */
		outptr[1] = *inptr++;
		outptr[0] = *inptr++;
		outptr += 3;
		}

	return 1;
	}



JDIMENSION COXBMPFile::PreloadImage(j_compress_ptr cinfo)
	{
	register BYTE b;
	register JSAMPROW out_ptr;
	JSAMPARRAY image_ptr;
	JDIMENSION row, col;
//	cd_progress_ptr progress = (cd_progress_ptr) cinfo->progress;

	/* Read the data into a virtual array in input-file row order. */
	TRY
		{
		for (row = 0; row < cinfo->image_height; row++)
			{
	//		if (progress != NULL)
	//			{
	//			progress->pub.pass_counter = (long) row;
	//			progress->pub.pass_limit = (long) cinfo->image_height;
	//			(*progress->pub.progress_monitor) ((j_common_ptr) cinfo);
	//			}

			image_ptr = (*cinfo->mem->access_virt_sarray)((j_common_ptr) cinfo, m_WholeImage,
				row, (JDIMENSION) 1, TRUE);
			
			out_ptr = image_ptr[0];
			if (sizeof(*out_ptr) == 1)
				{
				if (m_pGraphFile->Read(out_ptr, m_RowWidth) != m_RowWidth)
					ERREXIT(cinfo, JERR_INPUT_EOF);
				}
			else
				{
				for (col = m_RowWidth; col > 0; col--)
					{
					/* inline copy of ReadData() for speed */
					if (m_pGraphFile->Read(&b, 1) != 1)
						ERREXIT(cinfo, JERR_INPUT_EOF);
					
					*out_ptr++ = (JSAMPLE) b;
					}
				}
			}
		}
	CATCH(CFileException, e)
		{
		TRACE(_T("COXBMPFile::PreloadImage : Catching CFileException\n"));
		ERREXIT(m_cinfo, JERR_INPUT_EOF);
		return 0;
		}
	END_CATCH

//	if (progress != NULL)
//		progress->completed_extra_passes++;

	/* Set up to read from the virtual array in top-to-bottom order */
	switch (m_BitsPerPixel)
		{
		case 8:
			m_pfnGetPixelRows = &COXBMPFile::Get8bitRow;
			break;
		case 24:
			m_pfnGetPixelRows = &COXBMPFile::Get24bitRow;
			break;
		default:
			ERREXIT(cinfo, JERR_BMP_BADDEPTH);
		}
	m_SourceRow = cinfo->image_height;

	/* And read the first row */
	return (this->*m_pfnGetPixelRows)(cinfo);
	}

void COXBMPFile::PutGrayRows (j_decompress_ptr cinfo, JDIMENSION rows_supplied)
	/* This version is for grayscale OR quantized color output */
	{
	UNREFERENCED_PARAMETER(rows_supplied);

	JSAMPARRAY image_ptr;
	register JSAMPROW inptr, outptr;
	register JDIMENSION col;
	int pad;
	
	/* Access next row in virtual array */
	image_ptr = (*cinfo->mem->access_virt_sarray)((j_common_ptr) cinfo, m_WholeImage,
		m_CurOutputRow, (JDIMENSION) 1, TRUE);
	m_CurOutputRow++;
	
	/* Transfer data. */
	inptr = m_buffer[0];
	outptr = image_ptr[0];
	for (col = cinfo->output_width; col > 0; col--) {
		*outptr++ = *inptr++;	/* can omit GETJSAMPLE() safely */
		}
	
	/* Zero out the pad bytes. */
	pad = m_nPadBytes;
	while (--pad >= 0)
		*outptr++ = 0;
	}

void COXBMPFile::Put24PixelRows(j_decompress_ptr cinfo, JDIMENSION rows_supplied)
	/* This version is for writing 24-bit pixels */
	{
	UNREFERENCED_PARAMETER(rows_supplied);

	JSAMPARRAY image_ptr;
	register JSAMPROW inptr, outptr;
	register JDIMENSION col;
	int pad;

	/* Access next row in virtual array */
	image_ptr = (*cinfo->mem->access_virt_sarray)((j_common_ptr) cinfo, m_WholeImage,
		m_CurOutputRow, (JDIMENSION) 1, TRUE);
	m_CurOutputRow++;

	/* Transfer data.  Note destination values must be in BGR order
	* (even though Microsoft's own documents say the opposite).
	*/
	inptr = m_buffer[0];
	outptr = image_ptr[0];
	for (col = cinfo->output_width; col > 0; col--)
		{
		outptr[2] = *inptr++;	/* can omit GETJSAMPLE() safely */
		outptr[1] = *inptr++;
		outptr[0] = *inptr++;
		outptr += 3;
		}

	/* Zero out the pad bytes. */
	pad = m_nPadBytes;
	while (--pad >= 0)
		*outptr++ = 0;
	}

/*
 * Finish up at the end of the file.
 *
 * Here is where we really output the BMP file.
 *
 * First, routines to write the Windows and OS/2 variants of the file header.
 */
void COXBMPFile::WriteBMPHeader(j_decompress_ptr cinfo)
	/* Write a Windows-style BMP file header, including colormap if needed */
	{
	char bmpfileheader[14];
	char bmpinfoheader[40];
#define PUT_2B(array,offset,value)  \
	(array[offset] = (char) ((value) & 0xFF), \
	array[offset+1] = (char) (((value) >> 8) & 0xFF))
#define PUT_4B(array,offset,value)  \
	(array[offset] = (char) ((value) & 0xFF), \
	array[offset+1] = (char) (((value) >> 8) & 0xFF), \
	array[offset+2] = (char) (((value) >> 16) & 0xFF), \
	array[offset+3] = (char) (((value) >> 24) & 0xFF))
	INT32 headersize, bfSize;
	int bits_per_pixel, cmap_entries;
	
	/* Compute colormap size and total file size */
	if (cinfo->out_color_space == JCS_RGB)
		{
		if (cinfo->quantize_colors)
			{
			/* Colormapped RGB */
			bits_per_pixel = 8;
			cmap_entries = 256;
			}
		else
			{
			/* Unquantized, full color RGB */
			bits_per_pixel = 24;
			cmap_entries = 0;
			}
		}
	else
		{
		/* Grayscale output.  We need to fake a 256-entry colormap. */
		bits_per_pixel = 8;
		cmap_entries = 256;
		}
	
	/* File size */
	headersize = 14 + 40 + cmap_entries * 4; /* Header and colormap */
	bfSize = headersize + (INT32) m_RowWidth * (INT32) cinfo->output_height;
		
	/* Set UNREFERENCED_PARAMETER fields of header to 0 */
	MEMZERO(bmpfileheader, SIZEOF(bmpfileheader));
	MEMZERO(bmpinfoheader, SIZEOF(bmpinfoheader));
		
	/* Fill the file header */
	bmpfileheader[0] = 0x42;	/* first 2 bytes are ASCII 'B', 'M' */
	bmpfileheader[1] = 0x4D;
	PUT_4B(bmpfileheader, 2, bfSize); /* bfSize */
	/* we leave bfReserved1 & bfReserved2 = 0 */
	PUT_4B(bmpfileheader, 10, headersize); /* bfOffBits */
		
	/* Fill the info header (Microsoft calls this a BITMAPINFOHEADER) */
	PUT_2B(bmpinfoheader, 0, 40);	/* biSize */
	PUT_4B(bmpinfoheader, 4, cinfo->output_width); /* biWidth */
	PUT_4B(bmpinfoheader, 8, cinfo->output_height); /* biHeight */
	PUT_2B(bmpinfoheader, 12, 1);	/* biPlanes - must be 1 */
	PUT_2B(bmpinfoheader, 14, bits_per_pixel); /* biBitCount */
	/* we leave biCompression = 0, for none */
	/* we leave biSizeImage = 0; this is correct for uncompressed data */
	if (cinfo->density_unit == 2)
		{
		/* if have density in dots/cm, then */
		PUT_4B(bmpinfoheader, 24, (INT32) (cinfo->X_density*100)); /* XPels/M */
		PUT_4B(bmpinfoheader, 28, (INT32) (cinfo->Y_density*100)); /* XPels/M */
		}
	
	PUT_2B(bmpinfoheader, 32, cmap_entries); /* biClrUsed */
	/* we leave biClrImportant = 0 */
		
	if (WriteData(bmpfileheader, 14) == 0)
		ERREXIT(cinfo, JERR_FILE_WRITE);
	if (WriteData(bmpinfoheader, 40) == 0)
		ERREXIT(cinfo, JERR_FILE_WRITE);
	
	if (cmap_entries > 0)
		WriteColorMap(cinfo, cmap_entries, 4);
	}

void COXBMPFile::WriteOS2Header(j_decompress_ptr cinfo)
/* Write an OS2-style BMP file header, including colormap if needed */
	{
	char bmpfileheader[14];
	char bmpcoreheader[12];
	INT32 headersize, bfSize;
	int bits_per_pixel, cmap_entries;
	
	/* Compute colormap size and total file size */
	if (cinfo->out_color_space == JCS_RGB)
		{
		if (cinfo->quantize_colors)
			{
			/* Colormapped RGB */
			bits_per_pixel = 8;
			cmap_entries = 256;
			}
		else 
			{
			/* Unquantized, full color RGB */
			bits_per_pixel = 24;
			cmap_entries = 0;
			}
		}
	else
		{
		/* Grayscale output.  We need to fake a 256-entry colormap. */
		bits_per_pixel = 8;
		cmap_entries = 256;
		}

	/* File size */
	headersize = 14 + 12 + cmap_entries * 3; /* Header and colormap */
	bfSize = headersize + (INT32) m_RowWidth * (INT32) cinfo->output_height;
	
	/* Set UNREFERENCED_PARAMETER fields of header to 0 */
	MEMZERO(bmpfileheader, SIZEOF(bmpfileheader));
	MEMZERO(bmpcoreheader, SIZEOF(bmpcoreheader));
	
	/* Fill the file header */
	bmpfileheader[0] = 0x42;	/* first 2 bytes are ASCII 'B', 'M' */
	bmpfileheader[1] = 0x4D;
	PUT_4B(bmpfileheader, 2, bfSize); /* bfSize */
	/* we leave bfReserved1 & bfReserved2 = 0 */
	PUT_4B(bmpfileheader, 10, headersize); /* bfOffBits */
	
	/* Fill the info header (Microsoft calls this a BITMAPCOREHEADER) */
	PUT_2B(bmpcoreheader, 0, 12);	/* bcSize */
	PUT_2B(bmpcoreheader, 4, cinfo->output_width); /* bcWidth */
	PUT_2B(bmpcoreheader, 6, cinfo->output_height); /* bcHeight */
	PUT_2B(bmpcoreheader, 8, 1);	/* bcPlanes - must be 1 */
	PUT_2B(bmpcoreheader, 10, bits_per_pixel); /* bcBitCount */
	
	if (WriteData(bmpfileheader, 14) == 0)
		ERREXIT(cinfo, JERR_FILE_WRITE);
	if (WriteData(bmpcoreheader, 12) == 0)
		ERREXIT(cinfo, JERR_FILE_WRITE);
	
	if (cmap_entries > 0)
		WriteColorMap(cinfo, cmap_entries, 3);
	}

/*
 * Write the colormap.
 * Windows uses BGR0 map entries; OS/2 uses BGR entries.
 */

void COXBMPFile::WriteColorMap (j_decompress_ptr cinfo, int map_colors, int map_entry_size)
	{
	JSAMPARRAY colormap = cinfo->colormap;
	int num_colors = cinfo->actual_number_of_colors;
	int i;
	BYTE b;
	
	if (colormap != NULL)
		{
		if (cinfo->out_color_components == 3)
			{
			/* Normal case with RGB colormap */
			for (i = 0; i < num_colors; i++)
				{
				b = (BYTE)GETJSAMPLE(colormap[2][i]);
				WriteData(&b, 1);
				b = (BYTE)GETJSAMPLE(colormap[1][i]);
				WriteData(&b, 1);
				b = (BYTE)GETJSAMPLE(colormap[0][i]);
				WriteData(&b, 1);
				if (map_entry_size == 4)
					{
					b = 0;
					WriteData(&b, 1);
					}
				}
			}
		else 
			{
			/* Grayscale colormap (only happens with grayscale quantization) */
			for (i = 0; i < num_colors; i++)
				{
				b = (BYTE)GETJSAMPLE(colormap[0][i]);
				WriteData(&b, 1);
				WriteData(&b, 1);
				WriteData(&b, 1);
				if (map_entry_size == 4)
					{
					b = 0;
					WriteData(&b, 1);
					}
				}
			}
		}
	else
		{
		/* If no colormap, must be grayscale data.  Generate a linear "map". */
		for (i = 0; i < 256; i++)
			{
			b = (BYTE)i;
			WriteData(&b, 1);
			WriteData(&b, 1);
			WriteData(&b, 1);
			if (map_entry_size == 4)
				{
				b = 0;
				WriteData(&b, 1);
				}
			}
		}

	/* Pad colormap with zeros to ensure specified number of colormap entries */ 
	if (i > map_colors)
		ERREXIT1(cinfo, JERR_TOO_MANY_COLORS, i);
	for (; i < map_colors; i++)
		{
		b = 0;
		WriteData(&b, 1);
		WriteData(&b, 1);
		WriteData(&b, 1);
		if (map_entry_size == 4)
			WriteData(&b, 1);
		}
	}


BOOL COXBMPFile::StartInput(j_compress_ptr cinfo)
	{
	ASSERT(cinfo != NULL);

	U_CHAR bmpfileheader[14];
	U_CHAR bmpinfoheader[64];
	#define GET_2B(array,offset)  ((unsigned int) UCH(array[offset]) + \
				   (((unsigned int) UCH(array[offset+1])) << 8))
	#define GET_4B(array,offset)  ((INT32) UCH(array[offset]) + \
				   (((INT32) UCH(array[offset+1])) << 8) + \
				   (((INT32) UCH(array[offset+2])) << 16) + \
				   (((INT32) UCH(array[offset+3])) << 24))
	INT32 bfOffBits;
	INT32 headerSize;
	INT32 biWidth = 0;		/* initialize to avoid compiler warning */
	INT32 biHeight = 0;
	unsigned int biPlanes;
	INT32 biCompression;
	INT32 biXPelsPerMeter,biYPelsPerMeter;
	INT32 biClrUsed = 0;
	int mapentrysize = 0;		/* 0 indicates no colormap */
	INT32 bPad;
	JDIMENSION row_width;

	/* Read and verify the bitmap file header */
	if (! ReadOK(bmpfileheader, 14))
		ERREXIT(cinfo, JERR_INPUT_EOF);
	if (GET_2B(bmpfileheader,0) != 0x4D42) /* 'BM' */
		ERREXIT(cinfo, JERR_BMP_NOT);
	bfOffBits = (INT32) GET_4B(bmpfileheader,10);
	/* We ignore the remaining fileheader fields */

	/* The infoheader might be 12 bytes (OS/2 1.x), 40 bytes (Windows),
	* or 64 bytes (OS/2 2.x).  Check the first 4 bytes to find out which.
	*/
	if (! ReadOK(bmpinfoheader, 4))
		ERREXIT(cinfo, JERR_INPUT_EOF);
	headerSize = (INT32) GET_4B(bmpinfoheader,0);
	if (headerSize < 12 || headerSize > 64)
		ERREXIT(cinfo, JERR_BMP_BADHEADER);
	if (! ReadOK(bmpinfoheader+4, headerSize-4))
		ERREXIT(cinfo, JERR_INPUT_EOF);

	switch ((int) headerSize)
		{
		case 12:
			/* Decode OS/2 1.x header (Microsoft calls this a BITMAPCOREHEADER) */
			biWidth = (INT32) GET_2B(bmpinfoheader,4);
			biHeight = (INT32) GET_2B(bmpinfoheader,6);
			biPlanes = GET_2B(bmpinfoheader,8);
			m_BitsPerPixel = (int) GET_2B(bmpinfoheader,10);

			switch (m_BitsPerPixel)
				{
				case 8:			/* colormapped image */
					mapentrysize = 3;		/* OS/2 uses RGBTRIPLE colormap */
					TRACEMS2(cinfo, 1, JTRC_BMP_OS2_MAPPED, (int) biWidth, (int) biHeight);
					break;
				case 24:			/* RGB image */
					TRACEMS2(cinfo, 1, JTRC_BMP_OS2, (int) biWidth, (int) biHeight);
					break;
				default:
					ERREXIT(cinfo, JERR_BMP_BADDEPTH);
					break;
				}

			if (biPlanes != 1)
				ERREXIT(cinfo, JERR_BMP_BADPLANES);
			break;
		case 40:
		case 64:
			/* Decode Windows 3.x header (Microsoft calls this a BITMAPINFOHEADER) */
			/* or OS/2 2.x header, which has additional fields that we ignore */
			biWidth = GET_4B(bmpinfoheader,4);
			biHeight = GET_4B(bmpinfoheader,8);
			biPlanes = GET_2B(bmpinfoheader,12);
			m_BitsPerPixel = (int) GET_2B(bmpinfoheader,14);
			biCompression = GET_4B(bmpinfoheader,16);
			biXPelsPerMeter = GET_4B(bmpinfoheader,24);
			biYPelsPerMeter = GET_4B(bmpinfoheader,28);
			biClrUsed = GET_4B(bmpinfoheader,32);
			/* biSizeImage, biClrImportant fields are ignored */

			switch (m_BitsPerPixel)
				{
				case 8:			/* colormapped image */
					mapentrysize = 4;		/* Windows uses RGBQUAD colormap */
					TRACEMS2(cinfo, 1, JTRC_BMP_MAPPED, (int) biWidth, (int) biHeight);
					break;
				case 24:			/* RGB image */
					TRACEMS2(cinfo, 1, JTRC_BMP, (int) biWidth, (int) biHeight);
					break;
				default:
					ERREXIT(cinfo, JERR_BMP_BADDEPTH);
					break;
				}
			
			if (biPlanes != 1)
				ERREXIT(cinfo, JERR_BMP_BADPLANES);
			if (biCompression != 0)
				ERREXIT(cinfo, JERR_BMP_COMPRESSED);

			if (biXPelsPerMeter > 0 && biYPelsPerMeter > 0)
				{
				/* Set JFIF density parameters from the BMP data */
				cinfo->X_density = (UINT16) (biXPelsPerMeter/100); /* 100 cm per meter */
				cinfo->Y_density = (UINT16) (biYPelsPerMeter/100);
				cinfo->density_unit = 2;	/* dots/cm */
				}
			break;
		default:
			ERREXIT(cinfo, JERR_BMP_BADHEADER);
		break;
		}

	/* Compute distance to bitmap data --- will adjust for colormap below */
	bPad = bfOffBits - (headerSize + 14);

	/* Read the colormap, if any */
	if (mapentrysize > 0)
		{
		if (biClrUsed <= 0)
			biClrUsed = 256;		/* assume it's 256 */
		else if (biClrUsed > 256)
			ERREXIT(cinfo, JERR_BMP_BADCMAP);
	
		/* Allocate space to store the colormap */
		m_Colormap = (*cinfo->mem->alloc_sarray)((j_common_ptr) cinfo, JPOOL_IMAGE,
			(JDIMENSION) biClrUsed, (JDIMENSION) 3);

		/* and read it from the file */
		ReadColorMap((int) biClrUsed, mapentrysize);
		
		/* account for size of colormap */
		bPad -= biClrUsed * mapentrysize;
		}

	/* Skip any remaining pad bytes */
	if (bPad < 0)			/* incorrect bfOffBits value? */
		ERREXIT(cinfo, JERR_BMP_BADHEADER);
	while (--bPad >= 0)
		{
		(void) ReadByte();
		}

	/* Compute row width in file, including padding to 4-byte boundary */
	if (m_BitsPerPixel == 24)
		row_width = (JDIMENSION) (biWidth * 3);
	else
		row_width = (JDIMENSION) biWidth;
	
	while ((row_width & 3) != 0)
		row_width++;
	m_RowWidth = row_width;

	/* Allocate space for inversion array, prepare for preload pass */
	m_WholeImage = (*cinfo->mem->request_virt_sarray)((j_common_ptr) cinfo, JPOOL_IMAGE, FALSE,
		row_width, (JDIMENSION) biHeight, (JDIMENSION) 1);
	
	m_pfnGetPixelRows = &COXBMPFile::PreloadImage;

//	if (cinfo->progress != NULL)
//		{
//		cd_progress_ptr progress = (cd_progress_ptr) cinfo->progress;
//		progress->total_extra_passes++; /* count file input as separate pass */
//		}

	/* Allocate one-row buffer for returned data */
	m_buffer = (*cinfo->mem->alloc_sarray)((j_common_ptr) cinfo, JPOOL_IMAGE,
		(JDIMENSION) (biWidth * 3), (JDIMENSION) 1);
	m_buffer_height = 1;

	cinfo->in_color_space = JCS_RGB;
	cinfo->input_components = 3;
	cinfo->data_precision = 8;
	cinfo->image_width = (JDIMENSION) biWidth;
	cinfo->image_height = (JDIMENSION) biHeight;

	return TRUE;
	}

BOOL COXBMPFile::FinishInput(j_compress_ptr cinfo)
	{
	/* no work here */
	UNREFERENCED_PARAMETER(cinfo);
	return TRUE;
	}

JDIMENSION COXBMPFile::GetPixelRows(j_compress_ptr cinfo)
	{
	return (this->*m_pfnGetPixelRows)(cinfo);
	}


BOOL COXBMPFile::StartOutput(j_decompress_ptr dinfo)
	{
	/* no work here */
	UNREFERENCED_PARAMETER(dinfo);
	return TRUE;
	}

BOOL COXBMPFile::FinishOutput(j_decompress_ptr cinfo)
	{
	JSAMPARRAY image_ptr;
	register JSAMPROW data_ptr;
	JDIMENSION row;
	register JDIMENSION col;
//	cd_progress_ptr progress = (cd_progress_ptr) cinfo->progress;

	/* Write the header and colormap */
	if (m_bIsOS2)
		WriteOS2Header(cinfo);
	else
		WriteBMPHeader(cinfo);

	TRY
		{
		/* Write the file body from our virtual array */
		for (row = cinfo->output_height; row > 0; row--)
			{
	//		if (progress != NULL)
	//			{
	//			progress->pub.pass_counter = (long) (cinfo->output_height - row);
	//			progress->pub.pass_limit = (long) cinfo->output_height;
	//			(*progress->pub.progress_monitor) ((j_common_ptr) cinfo);
	//			}
		
			image_ptr = (*cinfo->mem->access_virt_sarray)
				((j_common_ptr) cinfo, m_WholeImage, row - 1, (JDIMENSION) 1, FALSE);
			data_ptr = image_ptr[0];

			if (sizeof(*data_ptr) == 1)
				{
				m_pGraphFile->Write(data_ptr, m_RowWidth);
				}
			else
				{
				BYTE b;
				for (col = m_RowWidth; col > 0; col--)
					{
					b = (BYTE)GETJSAMPLE(*data_ptr);
					m_pGraphFile->Write(&b, 1);
					data_ptr++;
					}
				}
			}
		}
	CATCH(CFileException, e)
		{
		TRACE(_T("COXBMPFile::FinishOutput : Catching CFileException\n"));
		return FALSE;
		}
	END_CATCH

//	if (progress != NULL)
//		progress->completed_extra_passes++;

	return TRUE;
	}

BOOL COXBMPFile::PutPixelRows(j_decompress_ptr cinfo, JDIMENSION rowsSupplied)
	{
	(this->*m_pfnPutPixelRows)(cinfo, rowsSupplied);
	
	return TRUE;
	}


BOOL COXBMPFile::InitRead(j_compress_ptr cinfo)
	{
	m_cinfo = cinfo;

	return TRUE;
	}

BOOL COXBMPFile::InitWrite(j_decompress_ptr cinfo, BOOL bExtra /* = FALSE */)
	{
	UNREFERENCED_PARAMETER(bExtra);

	JDIMENSION row_width;

	if (cinfo->out_color_space == JCS_GRAYSCALE)
		{
		m_pfnPutPixelRows = &COXBMPFile::PutGrayRows;
		}
	else if (cinfo->out_color_space == JCS_RGB)
		{
		if (cinfo->quantize_colors)
			m_pfnPutPixelRows = &COXBMPFile::PutGrayRows;
		else
			m_pfnPutPixelRows = &COXBMPFile::Put24PixelRows;
		}
	else
		{
		ERREXIT(cinfo, JERR_BMP_COLORSPACE);
		}

	/* Calculate output image dimensions so we can allocate space */
	jpeg_calc_output_dimensions(cinfo);

	/* Determine width of rows in the BMP file (padded to 4-byte boundary). */
	row_width = cinfo->output_width * cinfo->output_components;
	m_DataWidth = row_width;
	while ((row_width & 3) != 0)
		row_width++;
	m_RowWidth = row_width;
	m_nPadBytes = (int) (row_width - m_DataWidth);

	/* Allocate space for inversion array, prepare for write pass */
	m_WholeImage = (*cinfo->mem->request_virt_sarray)((j_common_ptr) cinfo, JPOOL_IMAGE, FALSE,
		row_width, cinfo->output_height, (JDIMENSION) 1);
	m_CurOutputRow = 0;
//	if (cinfo->progress != NULL)
//		{
//		cd_progress_ptr progress = (cd_progress_ptr) cinfo->progress;
//		progress->total_extra_passes++; /* count file input as separate pass */
//		}

	/* Create decompressor output buffer. */
	m_buffer = (*cinfo->mem->alloc_sarray)((j_common_ptr) cinfo, JPOOL_IMAGE, row_width, (JDIMENSION) 1);
	m_buffer_height = 1;

	return TRUE;
	}

#endif /* BMP_SUPPORTED */
///////////////////////////////////////////////////////////////////////////
