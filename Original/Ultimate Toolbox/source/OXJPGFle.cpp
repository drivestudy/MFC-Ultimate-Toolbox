// ==========================================================================
// 					Class Implementation : COXJPEGFile
// ==========================================================================

// Source file : OXJPGFle.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXJPGFle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members


// Data members -------------------------------------------------------------
// protected:


//	CFile*		m_pJPEGFile;
//	---			

//	CString		m_sFullPath;
//	---			

//	BOOL		m_bTotalReset;
//	---

	// private:

// Member functions ---------------------------------------------------------
// public:

/* Expanded data source object for MFC CFile */
struct MFC_source_mgr
	{
	struct jpeg_source_mgr pub;	/* public fields */

	CFile* pInfile;				/* source stream */
	JOCTET* buffer;				/* start of buffer */
	jpeg_boolean start_of_file;		/* have we gotten any data yet? */
	};

typedef MFC_source_mgr* MFC_src_ptr;

#define INPUT_BUF_SIZE  4096	/* choose an efficiently fread'able size */

/*
 * Initialize source --- called by jpeg_read_header
 * before any data is actually read.
 */

void COXJPEGFile::init_source(j_decompress_ptr cinfo)
	{
	MFC_src_ptr src = (MFC_src_ptr) cinfo->src;

	/* We reset the empty-input-file flag for each image,
	* but we don't clear the input buffer.
	* This is correct behavior for reading a series of images from one source.
	*/
	src->start_of_file = TRUE;
	}

/*
 * Fill the input buffer --- called whenever buffer is emptied.
 *
 * In typical applications, this should read fresh data into the buffer
 * (ignoring the current state of next_input_byte & bytes_in_buffer),
 * reset the pointer & count to the start of the buffer, and return TRUE
 * indicating that the buffer has been reloaded.  It is not necessary to
 * fill the buffer entirely, only to obtain at least one more byte.
 *
 * There is no such thing as an EOF return.  If the end of the file has been
 * reached, the routine has a choice of ERREXIT() or inserting fake data into
 * the buffer.  In most cases, generating a warning message and inserting a
 * fake EOI marker is the best course of action --- this will allow the
 * decompressor to output however much of the image is there.  However,
 * the resulting error message is misleading if the real problem is an empty
 * input file, so we handle that case specially.
 *
 * In applications that need to be able to suspend compression due to input
 * not being available yet, a FALSE return indicates that no more data can be
 * obtained right now, but more may be forthcoming later.  In this situation,
 * the decompressor will return to its caller (with an indication of the
 * number of scanlines it has read, if any).  The application should resume
 * decompression after it has loaded more data into the input buffer.  Note
 * that there are substantial restrictions on the use of suspension --- see
 * the documentation.
 *
 * When suspending, the decompressor will back up to a convenient restart point
 * (typically the start of the current MCU). next_input_byte & bytes_in_buffer
 * indicate where the restart point will be if the current call returns FALSE.
 * Data beyond this point must be rescanned after resumption, so move it to
 * the front of the buffer rather than discarding it.
 */

jpeg_boolean COXJPEGFile::fill_input_buffer(j_decompress_ptr cinfo)
	{
	MFC_src_ptr src = (MFC_src_ptr) cinfo->src;
	size_t nbytes=0;

	TRY
		{
		nbytes = src->pInfile->Read(src->buffer, INPUT_BUF_SIZE);
		}
	CATCH(CFileException, e)
		{
		TRACE(_T("COXJPEGFile::fill_input_buffer : Catching CFileException\n"));
		}
	END_CATCH

	if (nbytes <= 0)
		{
		if (src->start_of_file)	/* Treat empty input file as fatal error */
			ERREXIT(cinfo, JERR_INPUT_EMPTY);
		WARNMS(cinfo, JWRN_JPEG_EOF);
	
		/* Insert a fake EOI marker */
		src->buffer[0] = (JOCTET) 0xFF;
		src->buffer[1] = (JOCTET) JPEG_EOI;
		nbytes = 2;
		}

	src->pub.next_input_byte = src->buffer;
	src->pub.bytes_in_buffer = nbytes;
	src->start_of_file = FALSE;

	return TRUE;
	}


/*
 * Skip data --- used to skip over a potentially large amount of
 * uninteresting data (such as an APPn marker).
 *
 * Writers of suspendable-input applications must note that skip_input_data
 * is not granted the right to give a suspension return.  If the skip extends
 * beyond the data currently in the buffer, the buffer can be marked empty so
 * that the next read will cause a fill_input_buffer call that can suspend.
 * Arranging for additional bytes to be discarded before reloading the input
 * buffer is the application writer's problem.
 */

void COXJPEGFile::skip_input_data(j_decompress_ptr cinfo, long num_bytes)
	{
	MFC_src_ptr src = (MFC_src_ptr) cinfo->src;

	/* Just a dumb implementation for now.  Could use fseek() except
	* it doesn't work on pipes.  Not clear that being smart is worth
	* any trouble anyway --- large skips are infrequent.
	*/
	if (num_bytes > 0)
		{
		while (num_bytes > (long) src->pub.bytes_in_buffer)
			{
			num_bytes -= (long) src->pub.bytes_in_buffer;
			(void) fill_input_buffer(cinfo);
			/* note we assume that fill_input_buffer will never return FALSE,
			* so suspension need not be handled.
			*/
			}

		src->pub.next_input_byte += (size_t) num_bytes;
		src->pub.bytes_in_buffer -= (size_t) num_bytes;
		}
	}


/*
 * An additional method that can be provided by data source modules is the
 * resync_to_restart method for error recovery in the presence of RST markers.
 * For the moment, this source module just uses the default resync method
 * provided by the JPEG library.  That method assumes that no backtracking
 * is possible.
 */


/*
 * Terminate source --- called by jpeg_finish_decompress
 * after all data has been read.  Often a no-op.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */

void COXJPEGFile::term_source (j_decompress_ptr cinfo)
	{
	UNREFERENCED_PARAMETER(cinfo);
	/* no work necessary here */
	}

/*
 * Prepare for input from a MFC CFile.
 * The caller must have already opened the CFile, and is responsible
 * for closing it after finishing decompression.
 */

void COXJPEGFile::InitJPGSource(j_decompress_ptr cinfo, CFile* pInfile)
	{
	MFC_src_ptr src;

	/* The source object and input buffer are made permanent so that a series
	* of JPEG images can be read from the same file by calling jpeg_stdio_src
	* only before the first one.  (If we discarded the buffer at the end of
	* one image, we'd likely lose the start of the next one.)
	* This makes it unsafe to use this manager and a different source
	* manager serially with the same JPEG object.  Caveat programmer.
	*/
	if (cinfo->src == NULL)
		{	/* first time for this JPEG object? */
		cinfo->src = (struct jpeg_source_mgr *)
			(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
			SIZEOF(MFC_source_mgr));
	
		src = (MFC_src_ptr) cinfo->src;
		src->buffer = (JOCTET *)
			(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
			INPUT_BUF_SIZE * SIZEOF(JOCTET));
		}

	src = (MFC_src_ptr) cinfo->src;
	src->pub.init_source = init_source;
	src->pub.fill_input_buffer = fill_input_buffer;
	src->pub.skip_input_data = skip_input_data;
	src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
	src->pub.term_source = term_source;
	src->pInfile = pInfile;
	src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
	src->pub.next_input_byte = NULL; /* until buffer loaded */
	}

/* Expanded data destination object for stdio output */
struct MFC_destination_mgr
	{
	  struct jpeg_destination_mgr pub;	/* public fields */

	  CFile* pOutfile;					/* target CFile */
	  JOCTET* buffer;					/* start of buffer */
	};

typedef MFC_destination_mgr* MFC_dest_ptr;

#define OUTPUT_BUF_SIZE  4096	/* choose an efficiently fwrite'able size */

/*
 * Initialize destination --- called by jpeg_start_compress
 * before any data is actually written.
 */

void COXJPEGFile::init_destination (j_compress_ptr cinfo)
	{
	MFC_dest_ptr dest = (MFC_dest_ptr) cinfo->dest;

	/* Allocate the output buffer --- it will be released when done with image */
	dest->buffer = (JOCTET *)
		(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
			OUTPUT_BUF_SIZE * SIZEOF(JOCTET));

	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
	}


/*
 * Empty the output buffer --- called whenever buffer fills up.
 *
 * In typical applications, this should write the entire output buffer
 * (ignoring the current state of next_output_byte & free_in_buffer),
 * reset the pointer & count to the start of the buffer, and return TRUE
 * indicating that the buffer has been dumped.
 *
 * In applications that need to be able to suspend compression due to output
 * overrun, a FALSE return indicates that the buffer cannot be emptied now.
 * In this situation, the compressor will return to its caller (possibly with
 * an indication that it has not accepted all the supplied scanlines).  The
 * application should resume compression after it has made more room in the
 * output buffer.  Note that there are substantial restrictions on the use of
 * suspension --- see the documentation.
 *
 * When suspending, the compressor will back up to a convenient restart point
 * (typically the start of the current MCU). next_output_byte & free_in_buffer
 * indicate where the restart point will be if the current call returns FALSE.
 * Data beyond this point will be regenerated after resumption, so do not
 * write it out when emptying the buffer externally.
 */

jpeg_boolean COXJPEGFile::empty_output_buffer (j_compress_ptr cinfo)
	{
	MFC_dest_ptr dest = (MFC_dest_ptr) cinfo->dest;

	TRY
		{
		dest->pOutfile->Write(dest->buffer, OUTPUT_BUF_SIZE);
		}
	CATCH(CFileException, e)
		{
		TRACE(_T("COXJPEGFile::empty_output_buffer : Catching CFileException\n"));
		ERREXIT(cinfo, JERR_FILE_WRITE);
		return FALSE;
		}
	END_CATCH

	dest->pub.next_output_byte = dest->buffer;
	dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;

	return TRUE;
	}


/*
 * Terminate destination --- called by jpeg_finish_compress
 * after all data has been written.
 *
 * NB: *not* called by jpeg_abort or jpeg_destroy; surrounding
 * application must deal with any cleanup that should happen even
 * for error exit.
 */

void COXJPEGFile::term_destination(j_compress_ptr cinfo)
	{
	MFC_dest_ptr dest = (MFC_dest_ptr) cinfo->dest;
	size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;

	/* Write any data remaining in the buffer */
	if (datacount > 0)
		{
		TRY
			{
			dest->pOutfile->Write(dest->buffer, (UINT)datacount);
			}
		CATCH(CFileException, e)
			{
			TRACE(_T("COXJPEGFile::term_destination : Catching CFileException\n"));
			ERREXIT(cinfo, JERR_FILE_WRITE);
			return;
			}
		END_CATCH
		}
	}


/*
 * Prepare for output to a MFC CFile
 * The caller must have already opened the CFile, and is responsible
 * for closing it after finishing compression.
 */

void COXJPEGFile::InitJPGDestination(j_compress_ptr cinfo, CFile* pOut)
	{
	MFC_dest_ptr dest;

	/* The destination object is made permanent so that multiple JPEG images
	* can be written to the same file without re-executing jpeg_stdio_dest.
	* This makes it dangerous to use this manager and a different destination
	* manager serially with the same JPEG object, because their private object
	* sizes may be different.  Caveat programmer.
	*/
	if (cinfo->dest == NULL)
		{	/* first time for this JPEG object? */
		cinfo->dest = (struct jpeg_destination_mgr *)
			(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
			SIZEOF(MFC_destination_mgr));
		}

	dest = (MFC_dest_ptr) cinfo->dest;
	dest->pub.init_destination = init_destination;
	dest->pub.empty_output_buffer = empty_output_buffer;
	dest->pub.term_destination = term_destination;
	dest->pOutfile = pOut;
	}

COXJPEGFile::COXJPEGFile()
	: m_pJPEGFile(NULL),
	m_bTotalReset(TRUE)
	{
	}

COXJPEGFile::COXJPEGFile(CString sFullPath)
	: m_sFullPath(sFullPath),
	m_pJPEGFile(NULL),
	m_bTotalReset(TRUE)
	{
	}

COXJPEGFile::COXJPEGFile(CFile* pJPEGFile)
	: m_pJPEGFile(pJPEGFile),
	m_bTotalReset(TRUE)
	{
	ASSERT(m_pJPEGFile != NULL);
	}

#ifdef _DEBUG
void COXJPEGFile::Dump(CDumpContext& dc) const
	{
	CObject::Dump(dc);

	dc << TEXT("\nm_sFullPath ") << 	m_sFullPath;
	dc << TEXT("\nm_pJPEGFile ") << 	(void*)m_pJPEGFile;
	}

void COXJPEGFile::AssertValid() const
	{
	CObject::AssertValid();
	}
#endif


COXJPEGFile::~COXJPEGFile()
	{
	CloseFile();
	}

CString COXJPEGFile::GetFullFilePath()
	{
	return m_sFullPath;
	}

void COXJPEGFile::SetTotalReset(BOOL bTotal)
	{
	m_bTotalReset = bTotal;
	}

// Protected

void COXJPEGFile::OpenFile(UINT nOpenFlags)
	{
	if (m_pJPEGFile != NULL)
		return;	// Already open
	
	if (m_sFullPath.IsEmpty())
		THROW(new CFileException(CFileException::badPath));	// No path specified
	
	m_pJPEGFile = new CStdioFile(m_sFullPath, 
		(nOpenFlags | CFile::typeBinary) & ~CFile::typeText);
	}

void COXJPEGFile::AbortFile()
	{
	if (m_pJPEGFile == NULL)
		return;	// there is no file pointer
	ASSERT_VALID(m_pJPEGFile);
	
	if (m_sFullPath.IsEmpty())
		{
		TRACE0("In COXJPEGFile::AbortFile() : No path specified. Graphics file pointer no constructed by class");
		if (m_bTotalReset)
			m_pJPEGFile = NULL;
		}
	else
		{
		m_pJPEGFile->Close();
		delete m_pJPEGFile;
		m_pJPEGFile = NULL;
		}

	}

void COXJPEGFile::CloseFile()
	{
	if (m_pJPEGFile == NULL)
		return;	// Already closed
	ASSERT_VALID(m_pJPEGFile);
	
	if (m_sFullPath.IsEmpty())
		{
		TRACE0("In COXJPEGFile::CloseFile() : No path specified. Graphics file pointer no constructed by class");
		if (m_bTotalReset)
			m_pJPEGFile = NULL;
		}
	else
		{
		m_pJPEGFile->Close();
		delete m_pJPEGFile;
		m_pJPEGFile = NULL;
		}

	}

// protected

BOOL COXJPEGFile::StartInput(j_decompress_ptr cinfo)
	{
	(void) jpeg_start_decompress(cinfo);

	return TRUE;
	}

BOOL COXJPEGFile::FinishInput(j_decompress_ptr cinfo)
	{
	(void) jpeg_finish_decompress(cinfo);

	return TRUE;
	}

JDIMENSION COXJPEGFile::GetPixelRows(j_decompress_ptr cinfo,
									 JSAMPARRAY scanlines, JDIMENSION max_lines)
	{
	return jpeg_read_scanlines(cinfo, scanlines, max_lines);
	}


BOOL COXJPEGFile::StartOutput(j_compress_ptr cinfo)
	{
	jpeg_start_compress(cinfo, TRUE);

	return TRUE;
	}

BOOL COXJPEGFile::FinishOutput(j_compress_ptr cinfo)
	{
	jpeg_finish_compress(cinfo);

	return TRUE;
	}

BOOL COXJPEGFile::PutPixelRows(j_compress_ptr cinfo, JSAMPARRAY Buf, JDIMENSION rowsSupplied)
	{
    (void) jpeg_write_scanlines(cinfo, Buf, rowsSupplied);

	return TRUE;
	}


BOOL COXJPEGFile::InitRead(j_decompress_ptr cinfo)
	{
	ASSERT(m_pJPEGFile != NULL);

	InitJPGSource(cinfo, m_pJPEGFile);
	(void) jpeg_read_header(cinfo, TRUE);

	return TRUE;
	}

BOOL COXJPEGFile::InitWrite(j_compress_ptr cinfo, BOOL bExtra /* = FALSE */)
	{
	UNREFERENCED_PARAMETER(bExtra);
	ASSERT(m_pJPEGFile != NULL);

	InitJPGDestination(cinfo, m_pJPEGFile);

	return TRUE;
	}

jvirt_barray_ptr* COXJPEGFile::ReadCoefficients(j_decompress_ptr srcinfo)
	{
	return jpeg_read_coefficients(srcinfo);
	}

void COXJPEGFile::CopyCriticalParams(j_decompress_ptr srcinfo, j_compress_ptr dstinfo)
	{
	jpeg_copy_critical_parameters(srcinfo, dstinfo);
	}

void COXJPEGFile::WriteCoefficients(j_compress_ptr dstinfo, jvirt_barray_ptr* coef_arrays)
	{
	jpeg_write_coefficients(dstinfo, coef_arrays);
	}

size_t COXJPEGFile::ReadData(void* pBuffer, size_t length)
	{
	ASSERT(m_pJPEGFile != NULL);
	
	size_t nNumRead(0); 
	TRY
		{
		nNumRead = m_pJPEGFile->Read(pBuffer, (UINT)length);
		}
	CATCH(CFileException, e)
		{
		TRACE(_T("COXJPEGFile::ReadData : Catching CFileException\n"));
		return 0;
		}
	END_CATCH
	
	return nNumRead;
	}

size_t COXJPEGFile::WriteData(const void* pBuffer, size_t length)
	{
	ASSERT(m_pJPEGFile != NULL);
	
	TRY
		{
		m_pJPEGFile->Write(pBuffer, (UINT)length);
		}
	CATCH(CFileException, e)
		{
		TRACE(_T("COXJPEGFile::WriteData : Catching CFileException\n"));
		return 0;
		}
	END_CATCH
	
	return 1;
	}


///////////////////////////////////////////////////////////////////////////
