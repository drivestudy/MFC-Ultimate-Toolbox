// ==========================================================================
// 					Class Implementation : COXJPEGCompressor
// ==========================================================================

// Source file : OXJPGCom.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXJPGCom.h"

#include "OXJPGFle.h"
#include "OXGphFle.h"

#include "OXJPGExp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

#define QUALITY_DEFAULT	75
#define SIZE_ERROR_BUF	255

/////////////////////////////////////////////////////////////////////////////
// Definition of static members


// Data members -------------------------------------------------------------
// protected:


//	CFile*		m_pGraphFile;
//	---			

	// private:

// Member functions ---------------------------------------------------------
// public:

COXJPEGCompressor::COXJPEGCompressor()
	:m_nQuality(QUALITY_DEFAULT),
	m_bGrayScale(FALSE),
	m_bOptimize(FALSE),
	m_bProgressive(FALSE),
	m_eDisCosTransf(DC_Undefined),
	m_nSmooth(0),
	m_nMaxMem(0),
	m_bBaseLine(FALSE)
	{
	}

#ifdef _DEBUG
void COXJPEGCompressor::Dump(CDumpContext& dc) const
	{
	COXJPEGCodec::Dump(dc);

	dc << TEXT("\nm_nQuality : ") << 		m_nQuality;
	dc << TEXT("\nm_bGrayScale : ") << 	(WORD)m_bGrayScale;
	dc << TEXT("\nm_bOptimize : ") <<	(WORD)m_bOptimize;
	dc << TEXT("\nm_bBaseLine ") << 	(WORD)m_bBaseLine;
	dc << TEXT("\nm_bProgressive ") << 	(WORD)m_bProgressive;
	dc << TEXT("\nm_eDisCosTransf ") << 	(int)m_eDisCosTransf;
	dc << TEXT("\nm_nSmooth ") << 	m_nSmooth;
	dc << TEXT("\nm_nMaxMem ") << 	m_nMaxMem;
	}

void COXJPEGCompressor::AssertValid() const
	{
	COXJPEGCodec::AssertValid();
	}
#endif

COXJPEGCompressor::~COXJPEGCompressor()
	{
	}

void COXJPEGCompressor::SetQuality(UINT nQuality)
	{
	ASSERT(0 <= nQuality && nQuality <= 100);

	m_nQuality = nQuality;
	}

void COXJPEGCompressor::SetOptimize(BOOL bOptimize)
		{
#ifndef ENTROPY_OPT_SUPPORTED
		TRACE(_T("Entropy optimization was not compiled\n"));
#endif
		m_bOptimize = bOptimize;
		}

void COXJPEGCompressor::SetProgressive(BOOL bProgressive)
		{
#ifndef C_PROGRESSIVE_SUPPORTED
		TRACE(_T("progressive output was not compiled\n"));
#endif
		m_bProgressive = bProgressive;
		}

void COXJPEGCompressor::SetDisCosTranf(EDiscreteCosTransf eDisCosTransf)
	{
	ASSERT(DC_FIRST <= eDisCosTransf && eDisCosTransf <= DC_LAST);

	m_eDisCosTransf = eDisCosTransf;
	}

void COXJPEGCompressor::SetSmooth(UINT nSmooth)
	{
	ASSERT(1 <= nSmooth && nSmooth <= 100);

	m_nSmooth = nSmooth;
	}

short COXJPEGCompressor::DoCompress(COXGraphicFile* pGraphicsFile, COXJPEGFile* pJPEGFile)
	{
	TCHAR ErrorBuffer[SIZE_ERROR_BUF];
	TRY
		{
		pGraphicsFile->OpenFile(CFile::modeRead | CFile::shareDenyWrite);

		pJPEGFile->OpenFile(CFile::modeCreate | CFile::modeWrite);
		}
	CATCH(CFileException, e)
		{
		pGraphicsFile->AbortFile();
		pJPEGFile->AbortFile();
		
		e->GetErrorMessage(ErrorBuffer, SIZE_ERROR_BUF);
		THROW(new COXJPEGException(e->m_cause, (LPCTSTR)ErrorBuffer));
		}
	END_CATCH


	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JDIMENSION num_scanlines;

	TRY
		{
		// Mapping needed to call virtual functions of right codec
		m_RunningCodecsMap.SetAt(&cinfo, this);

		/* Initialize the JPEG compression object with default error handling. */
		SetJPEGErrorHandling(&jerr);
		cinfo.err = &jerr;

		jpeg_create_compress(&cinfo);

		/* Initialize JPEG parameters.
		* Much of this may be overridden later.
		* In particular, we don't yet know the input file's color space,
		* but we need to provide some value for jpeg_set_defaults() to work.
		*/

		cinfo.in_color_space = JCS_RGB; /* arbitrary guess */
		jpeg_set_defaults(&cinfo);

		// Configure the JPG Compression Manager
		ProcessSwitches(&cinfo, FALSE);

		pGraphicsFile->InitRead(&cinfo);
		/* Read the input file header to obtain file size & colorspace. */
		pGraphicsFile->StartInput(&cinfo);

		/* Now that we know input colorspace, fix colorspace-dependent defaults */
		jpeg_default_colorspace(&cinfo);

		// Adjust default compression parameters by Re-Configuring the JPG Compression Manager
		ProcessSwitches(&cinfo, TRUE);

		/* Specify data destination for compression */
		pJPEGFile->InitWrite(&cinfo, TRUE);

		/* Start compressor */
		pJPEGFile->StartOutput(&cinfo);

		/* Process data */
		while (cinfo.next_scanline < cinfo.image_height)
			{
			num_scanlines = pGraphicsFile->GetPixelRows(&cinfo);
			pJPEGFile->PutPixelRows(&cinfo, pGraphicsFile->m_buffer, num_scanlines);
			}

		/* Finish compression and release memory */
		pGraphicsFile->FinishInput(&cinfo);
		pJPEGFile->FinishOutput(&cinfo);
		jpeg_destroy_compress(&cinfo);
		}
	CATCH(COXJPEGException, e)
		{
		m_RunningCodecsMap.RemoveKey(&cinfo);
	
		THROW_LAST();
		}
	END_CATCH

	m_RunningCodecsMap.RemoveKey(&cinfo);

	TRY
		{
		/* Close files, if we opened them */
		pGraphicsFile->CloseFile();

		pJPEGFile->CloseFile();
		}
	CATCH(CFileException, e)
		{
		pGraphicsFile->AbortFile();
		pJPEGFile->AbortFile();
		
		e->GetErrorMessage(ErrorBuffer, SIZE_ERROR_BUF);
		THROW(new COXJPEGException(e->m_cause, (LPCTSTR)ErrorBuffer));
		}
	END_CATCH

	CString sWarnings = GetWarningMessages();
	if (!sWarnings.IsEmpty())
		return 2;

	return 0;
	}


// Protected
void COXJPEGCompressor::ProcessSwitches(j_compress_ptr cinfo, BOOL bForReal)
	{
	cinfo->err->trace_level = 0;

	// Select DCT algoritm
	switch(m_eDisCosTransf)
		{
		case DC_Int:
			cinfo->dct_method = JDCT_ISLOW;
			break;
		case DC_FastInt:	
			cinfo->dct_method = JDCT_IFAST;
			break;
		case DC_Float:
			cinfo->dct_method = JDCT_FLOAT;
			break;
		default:
			// do nothing
			break;
		}

	if (m_bGrayScale)
		// Force a monochrome jpeg to be generated
		jpeg_set_colorspace(cinfo, JCS_GRAYSCALE);

	if (m_nMaxMem != 0)
		// Maximum memory in Kb
		cinfo->mem->max_memory_to_use = m_nMaxMem * 1000L;

	if (m_bOptimize)
		/* Enable entropy parm optimization. */
		{
#ifdef ENTROPY_OPT_SUPPORTED
		cinfo->optimize_coding = TRUE;
#else
		TRACE(_T("Entropy optimization was not compiled\n"));
#endif
		}

	if (m_nQuality != QUALITY_DEFAULT)
		jpeg_quality_scaling(m_nQuality);


    if (m_nSmooth != 0)
		cinfo->smoothing_factor = m_nSmooth;

	if (bForReal)
		{
		if (m_nQuality != QUALITY_DEFAULT)
			jpeg_set_quality(cinfo, m_nQuality, m_bBaseLine);
		
		if (m_bProgressive)
			{
#ifdef C_PROGRESSIVE_SUPPORTED
			jpeg_simple_progression(cinfo);
#else
			TRACE(_T("progressive output was not compiled\n"));
#endif
			}

		}
	}


///////////////////////////////////////////////////////////////////////////
