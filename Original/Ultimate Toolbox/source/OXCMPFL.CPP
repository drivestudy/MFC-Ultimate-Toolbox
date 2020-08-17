// ==========================================================================
// 							Class Implementation : COXCompressedFile
// ==========================================================================

// Source file : oxcmpfl.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "oxcmpfl.h"	// class specification
#include <limits.h>		// for SHRT_MAX

#pragma warning (disable : 4702)

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXCompressedFile, CFile)         

#define new DEBUG_NEW                              
#define COMPRESS_BIT	0x8000                                
#define TRUE_LENGTH		0x4000
#define ACTUAL_LENGTH 	0x3FFF
#define min0(a,b) __max(0,__min( (a) , (b) ) )
/////////////////////////////////////////////////////////////////////////////
// Definition of static members
const WORD COXCompressedFile::m_wBufferLength = 1024;

// Data members -------------------------------------------------------------
// protected:
// BOOL m_bEOF
// --- if false, value of m_lFileLength is not valid
// BOOL m_bOpenedForRead;
// --- Whether the file has been opened for Read
// BOOL m_bOpenedForWrite;
// --- Whether the file has been opened for Write
// int m_nFirstBufferLength
// --- length of first buffer, is null when no reading has taken place
// long m_lPosition
// --- vitual filepointer
// long m_lFileLength
// --- virtual file length, calculated in GetFinalPos()
// long m_lPrevLengthPos
// ---  disk position of the coded length of a buffer
// UINT m_nExpandedLength
// --- Length after reading from disk (decoded if needed)
// UINT m_nInternalPos
// --- current position in m_pInternalBuf
// unsigned char  *m_pInternalBuf
// --- processing buffer between inputbuffer and information on disk
// WORD m_wReadHeaderLength
// --- real length of header on disk
// WORD m_wLastBufferLength
// --- length of last buffer read
// static const WORD m_wBufferLength
// --- length of m_pInternalBuf and helpbuffers
//  CWordArray m_BufLenArray
// --- Array to hold all buferlengths already read
// CFile* m_pDelegateToFile
// --- The next transformation in the transformation chain


// private:

// Member functions ---------------------------------------------------------
// public:

COXCompressedFile::COXCompressedFile(UINT nHeaderLength)
:
m_bEOF(FALSE),
m_bOpenedForRead(FALSE),
m_bOpenedForWrite(FALSE),
m_lPosition(0),
m_lFileLength(0),
m_nInternalPos(0),
m_lPrevLengthPos(0),
m_nExpandedLength(0),
m_wLastBufferLength(0),
m_nFirstBufferLength(0),
m_wReadHeaderLength(0),
m_nHeaderLength(nHeaderLength),
m_pDelegateToFile(NULL)
{	
	m_pInternalBuf = new unsigned char[m_wBufferLength];
}

COXCompressedFile::COXCompressedFile(LPCTSTR pszFilename, UINT nOpenFlags, UINT nHeaderLength)
:
m_bEOF(FALSE),
m_bOpenedForRead(FALSE),
m_bOpenedForWrite(FALSE),
m_lPosition(0),
m_lFileLength(0),
m_nInternalPos(0),
m_lPrevLengthPos(0),
m_nExpandedLength(0),
m_wLastBufferLength(0),
m_nFirstBufferLength(0),
m_wReadHeaderLength(0),
m_nHeaderLength(nHeaderLength),
m_pDelegateToFile(NULL)
{	
	m_pInternalBuf = new unsigned char[m_wBufferLength];
	CFileException* pxFile = NULL;
	if (!Open(pszFilename,nOpenFlags,pxFile))
		AfxThrowFileException(pxFile->m_cause);
}                                      


CFile* COXCompressedFile::Duplicate() const
{
	AfxThrowNotSupportedException();
	return NULL;
}

BOOL COXCompressedFile::Open(LPCTSTR pszFileName, UINT nOpenFlags,
							 CFileException* pException /* = NULL */)
{
	ASSERT(m_nHeaderLength < m_wBufferLength);
	ASSERT(m_pDelegateToFile == NULL);

	// reading and writing together not allowed
	if (nOpenFlags & CFile::modeReadWrite)
	{
		if (pException != NULL)
			pException->m_cause = CFileException::accessDenied;
		return FALSE;
	}
	// ... Store open flag                             
	m_bEOF				  = FALSE;
	m_bOpenedForWrite     = (nOpenFlags & CFile::modeWrite);
	m_bOpenedForRead      = TRUE;	// Read always permitted CFile::modeRead == 0x0000,
	m_nExpandedLength     = 0;
	m_nInternalPos        = 0;
	m_lPosition			  = 0;

	// ... Call base class implementation
	return CFile::Open(pszFileName, nOpenFlags, pException);
}

BOOL COXCompressedFile::DelegateOpen(CFile* pDelegateToFile, UINT nOpenFlags)
{
	ASSERT_VALID(this);
	ASSERT(pDelegateToFile != NULL);

	// reading and writing together not allowed
	if (nOpenFlags & CFile::modeReadWrite)
		return FALSE;

	// ... Store open flag
	m_bOpenedForWrite     = (nOpenFlags & CFile::modeWrite);
	m_bOpenedForRead      = TRUE;	// Read always permitted CFile::modeRead == 0x0000,
	m_bEOF				  = FALSE;

	m_lPosition = 0;
	m_lFileLength = 0;
	m_nInternalPos = 0;
	m_lPrevLengthPos = 0;
	m_nExpandedLength = 0;
	m_wLastBufferLength = 0;
	m_nFirstBufferLength = 0;
	m_wReadHeaderLength = 0;

	// setup the chain
	m_pDelegateToFile = pDelegateToFile;

	ASSERT_VALID(this);
	return TRUE;
}

BOOL COXCompressedFile::UnDelegateOpen()
{
	m_pDelegateToFile = NULL;

	m_bOpenedForWrite     = FALSE;
	m_bOpenedForRead      = FALSE;
	m_bEOF				  = FALSE;

	m_lPosition = 0;
	m_lFileLength = 0;
	m_nInternalPos = 0;
	m_lPrevLengthPos = 0;
	m_nExpandedLength = 0;
	m_wLastBufferLength = 0;
	m_nFirstBufferLength = 0;
	m_wReadHeaderLength = 0;

	return TRUE;
}

UINT COXCompressedFile::Read(void FAR* lpBuf, UINT nCount) 
{
	if (m_bOpenedForWrite || !m_bOpenedForRead)
		AfxThrowFileException(CFileException::accessDenied);

	UINT	nBytesRead        = 0;
	int  	nAllocatebleBytes = 0;       
	WORD	wCodedReadLength  = 0;
	char*	lpBuffer = (char*)lpBuf;  

	// processing header info
	nBytesRead = ReadHeader(lpBuffer, nCount);

	int pos(0);
	if (m_pDelegateToFile != NULL)
		pos = (int) m_pDelegateToFile->GetPosition();
	else
		pos = (int) CFile::GetPosition();

	// processing compressed info
	while(nBytesRead < nCount)
	{
		// nAllocatebleBytes is number of bytes processed, before a new buffer is read it's first 
		// bytes are transferred till final position reached
		nAllocatebleBytes = min0((signed)nCount - (signed)nBytesRead,
			(signed)m_nExpandedLength - (signed)m_nInternalPos);

		memcpy(&lpBuffer[nBytesRead], &m_pInternalBuf[m_nInternalPos], nAllocatebleBytes);

		nBytesRead     += nAllocatebleBytes;
		m_nInternalPos += nAllocatebleBytes;
		m_lPosition    += nAllocatebleBytes;

		// when not finished reading
		if (nCount > nBytesRead) 
		{
			// if read failes, EOF is reached
			if (m_pDelegateToFile != NULL)
			{
				if (m_pDelegateToFile->Read(&wCodedReadLength, sizeof(WORD)) != sizeof(WORD))
					break;
			}
			else
			{
				if (CFile::Read(&wCodedReadLength, sizeof(WORD)) != sizeof(WORD))
					break;
			}

			if (wCodedReadLength & TRUE_LENGTH)
				// this coded length was the length of the last buffer so
				// skip true length to read the actual compressed data
			{
				WORD wTrueLength;
				// if read failes, EOF is reached
				if (m_pDelegateToFile != NULL)
				{
					if (m_pDelegateToFile->Read(&wTrueLength, sizeof(WORD)) != sizeof(WORD))
						break;
				}
				else
				{
					if (CFile::Read(&wTrueLength, sizeof(WORD)) != sizeof(WORD))
						break;
				}
			}

			if (m_pDelegateToFile != NULL)
				m_lPrevLengthPos    = (long) __max(m_lPrevLengthPos,m_pDelegateToFile->Seek(0, CFile::current) - (int)sizeof(WORD));
			else
				m_lPrevLengthPos    = (long) __max(m_lPrevLengthPos,CFile::Seek(0, CFile::current) - (int)sizeof(WORD));

			m_wLastBufferLength = (WORD)InternalRead(wCodedReadLength);
			m_nInternalPos = 0;                                   
		}			
	}
	return nBytesRead;
}   

void COXCompressedFile::Write(const void FAR* lpBuf, UINT nCount)
{                  
	if (!m_bOpenedForWrite)
		AfxThrowFileException(CFileException::accessDenied);

	UINT 		nBytesWritten     = 0;
	UINT 		nAllocatebleBytes = 0;       
	LPCTSTR		lpBuffer = (LPCTSTR)lpBuf;

	// processing header information
	nBytesWritten = WriteHeader(lpBuffer, nCount);

	// processing information
	while(nBytesWritten < nCount)
	{                                     
		nAllocatebleBytes = min0(nCount - nBytesWritten, m_wBufferLength - m_nInternalPos);
		ASSERT(nAllocatebleBytes >= 0 && nAllocatebleBytes <= m_wBufferLength);
		// transfer lpBuf to internal buffer
		memcpy(&m_pInternalBuf[m_nInternalPos], &lpBuffer[nBytesWritten], nAllocatebleBytes);
		nBytesWritten     += nAllocatebleBytes;
		m_nInternalPos    += nAllocatebleBytes;
		ASSERT(m_nInternalPos <= m_wBufferLength);
		m_lPosition		  += nAllocatebleBytes;
		// write when position in internal buffer equals length (end) of internal buffer
		if (m_nInternalPos == m_wBufferLength)
		{
			InternalWrite();
			m_nInternalPos = 0;
		}
	}
}

#if _MFC_VER >= 0x0700
ULONGLONG COXCompressedFile::GetPosition() const
#else
DWORD COXCompressedFile::GetPosition() const
#endif
{
	return m_lPosition;
}

void COXCompressedFile::Flush()
{
	TRACE(_T("In COXCompressedFile::Flush : Trying to flush a compressed file. This is not allowed\n"));

	AfxThrowNotSupportedException();
}

void COXCompressedFile::Close()
{
	if (!m_bOpenedForRead)
	{
		TRACE(_T("In COXCompressedFile::Close : Trying to close a file that hasn't been opened yet.\n"));
		return;
	}

	AutoFlush(TRUE);
	if (m_pDelegateToFile == NULL)
		CFile::Close();

	// ... Re-init the read/write flags
	m_bOpenedForRead = FALSE;
	m_bOpenedForWrite = FALSE;
}

void COXCompressedFile::Abort()
{
	// ... Call base class implementation
	if (m_pDelegateToFile != NULL)
		m_pDelegateToFile->Abort();
	else
		CFile::Abort();

	// ... Re-init the read/write flags
	m_bOpenedForRead = FALSE;
	m_bOpenedForWrite = FALSE;
}

void COXCompressedFile::LockRange(DWORD dwPos, DWORD dwCount)
{
	dwPos;
	dwCount;

	AfxThrowNotSupportedException();
}

void COXCompressedFile::UnlockRange(DWORD dwPos, DWORD dwCount)
{
	dwPos;
	dwCount;

	AfxThrowNotSupportedException();
}

void COXCompressedFile::SetLength(DWORD dwNewLen)
{
	dwNewLen;

	AfxThrowNotSupportedException();
}

ULONGLONG COXCompressedFile::GetLength()
{
	long lLastFilePointerPos;

	if (m_pDelegateToFile != NULL)
		lLastFilePointerPos  = (long) m_pDelegateToFile->Seek(0,CFile::current);
	else
		lLastFilePointerPos  = (long) CFile::Seek(0,CFile::current);

	GetFinalPos();
	if (m_pDelegateToFile != NULL)
		m_pDelegateToFile->Seek(lLastFilePointerPos,CFile::begin);
	else
		CFile::Seek(lLastFilePointerPos,CFile::begin);

	return m_lFileLength;
}

LONG COXCompressedFile::Seek(LONG lOff, UINT nFrom)
{
	if (m_bOpenedForWrite)
		AfxThrowFileException(CFileException::invalidFile);

	long    lBytesSought          = 0;

	if(m_bOpenedForWrite)
		return 0;


	if (nFrom == CFile::begin)
	{
		m_lPosition          = 0;
		m_nInternalPos       = 0;
		m_nExpandedLength    = 0;
		m_nFirstBufferLength = 0;

		// Force a reload of the header if the sought element falls in it.
		m_wReadHeaderLength = 0;
	}	

	//trying to position before BOF
	if(m_lPosition + lOff < 0 && nFrom != CFile::end)
		AfxThrowFileException(CFileException::badSeek);

	// trying to position after EOF
	if (lOff > 0 && nFrom == CFile::end)
		AfxThrowFileException(CFileException::endOfFile);

	switch(nFrom)               
	{						  
	case CFile::begin:
		if (m_pDelegateToFile != NULL)
			m_pDelegateToFile->Seek(0,CFile::begin);
		else
			CFile::Seek(0,CFile::begin);
		break;
	case CFile::current:
		if ((LONG)m_nInternalPos + lOff <= 0)
		{
			Seek(m_lPosition + lOff, CFile::begin);
			return m_lPosition;
		}
		break;
	case CFile::end:
		if (!m_bEOF)
			GetFinalPos();

		if (m_lFileLength + lOff >= 0)
		{ 	
			Seek(m_lFileLength + lOff, CFile::begin);
			return m_lPosition;
		}
		else
			//trying to position before BOF
			AfxThrowFileException(CFileException::badSeek);
	default:
		break;
	}                                          

	// seek happens in header itself, no seeking in internal buffer	
	if (m_lPosition + lOff <= (long)m_nHeaderLength)
	{
		if (m_wReadHeaderLength == 0)
		{
			if (m_pDelegateToFile != NULL)
			{
				m_pDelegateToFile->Seek(0,CFile::begin);
				m_wReadHeaderLength = (WORD)m_pDelegateToFile->Read(m_pInternalBuf,m_nHeaderLength);
			}
			else
			{
				CFile::Seek(0,CFile::begin);
				m_wReadHeaderLength = (WORD)CFile::Read(m_pInternalBuf,m_nHeaderLength);
			}
		}

		// seek possible, in header only
		if (m_lPosition + lOff <= (LONG)m_wReadHeaderLength)
		{
			m_lPosition     += lOff;
			m_nInternalPos  += lOff;
			return m_lPosition;
		}
		else
			// seeking after EOF
		{
			if (m_pDelegateToFile != NULL)
			{
				m_lPosition      = (long) m_pDelegateToFile->GetPosition();
				m_nInternalPos   = (UINT) m_pDelegateToFile->GetPosition();
			}
			else
			{
				m_lPosition      = (long) CFile::GetPosition();
				m_nInternalPos   = (UINT) CFile::GetPosition();
			}

			AfxThrowFileException(CFileException::endOfFile);
		}
	}
	else
	{
		// if positioned in header and seeking outside header move m_lPosition after header
		if (m_lPosition < (long)m_nHeaderLength)
		{
			// if m_wReadHeaderLength is not known yet, get the real length of the header
			if (m_wReadHeaderLength == 0)
			{                        
				if (m_pDelegateToFile != NULL)
				{
					m_pDelegateToFile->Seek(0,CFile::begin);
					m_wReadHeaderLength  = (WORD)m_pDelegateToFile->Read(m_pInternalBuf,m_nHeaderLength);
				}
				else
				{
					CFile::Seek(0,CFile::begin);
					m_wReadHeaderLength  = (WORD)CFile::Read(m_pInternalBuf,m_nHeaderLength);
				}
			}
			// if header is smaller than fixed m_nHeaderLength
			if (m_wReadHeaderLength < m_nHeaderLength)
			{
				if (m_pDelegateToFile != NULL)
					m_lPrevLengthPos     = (long) m_pDelegateToFile->Seek(m_wReadHeaderLength, CFile::begin);
				else
					m_lPrevLengthPos     = (long) CFile::Seek(m_wReadHeaderLength, CFile::begin);
				m_lPosition          = m_wReadHeaderLength;
				m_nInternalPos       = m_wReadHeaderLength;
				m_nFirstBufferLength = m_wReadHeaderLength;
				return m_lPosition;
			}
			// position on end of header
			else
			{
				if (m_pDelegateToFile != NULL)
					m_lPrevLengthPos     = (long) m_pDelegateToFile->Seek(m_nHeaderLength, CFile::begin);
				else
					m_lPrevLengthPos     = (long) CFile::Seek(m_nHeaderLength, CFile::begin);
				lBytesSought         = m_nHeaderLength;
				m_lPosition          = m_nHeaderLength;
				m_nFirstBufferLength = m_nHeaderLength;

				// The part of the offset already skipped is the diference between the total
				// headerlength and the internal pos (the last position of the virtual filepointer) 
				lOff -= m_nHeaderLength - m_nInternalPos;
				m_nInternalPos       = 0;
			}
		}
	}

	if (m_nInternalPos != 0)
		// Seek started somewhere within a buffer
	{
		if (lOff >= 0)
		{
			if ((long)(m_wBufferLength - m_nInternalPos) > lOff)
				// offset stays within current buffer
			{
				m_lPosition += lOff;
				m_nInternalPos += lOff;

				lOff = 0;
				return m_lPosition;
			}
			else
			{
				m_lPosition += m_wBufferLength - m_nInternalPos;

				lOff -= m_wBufferLength - m_nInternalPos;

				m_nInternalPos = 0;
			}
		}
		else 
		{
			if ((long)m_nInternalPos > labs(lOff))
			{
				m_lPosition += lOff;
				m_nInternalPos += lOff;

				lOff = 0;
				return m_lPosition;
			}
		}

	}

	MoveFromBufferBoundaries(m_lPosition / m_wBufferLength, lOff);

	return m_lPosition;
}

// Diagnostics ------------------------------------------------------------------


#ifdef _DEBUG
void COXCompressedFile::Dump(CDumpContext& dc) const
{
	if (m_pDelegateToFile != NULL)
		m_pDelegateToFile->Dump(dc);
	else
		CFile::Dump(dc);
	dc << "\nm_bOpenedForRead : " << 		(WORD)m_bOpenedForRead;
	dc << "\nm_bOpenedForWrite : " <<	(WORD)m_bOpenedForWrite;
}

void COXCompressedFile::AssertValid() const
{
	if (m_pDelegateToFile != NULL)
		m_pDelegateToFile->AssertValid();
	else
		CFile::AssertValid();
}
#endif

COXCompressedFile::~COXCompressedFile()
{
	if (m_pDelegateToFile != NULL)
		Close();

	delete [] m_pInternalBuf;
}

// protected:

UINT COXCompressedFile::ReadHeader(char* lpBuffer, UINT nCount)
// In	: nCount  , the number af characters to be read in
// Out  : lpBuffer, a character buffer containing the header
// Returns : number of characters read
// Effect  : reading a number of header bytes
{
	UINT nBytesRead = 0;
	// when positioned in header	                                   
	if (m_lPosition < (long)m_nHeaderLength)
	{	
		if (m_pDelegateToFile != NULL)
		{
			m_pDelegateToFile->Seek(0, CFile::begin);
			m_wLastBufferLength = m_wReadHeaderLength = (WORD)m_pDelegateToFile->Read(m_pInternalBuf,m_nHeaderLength);
		}
		else
		{
			CFile::Seek(0, CFile::begin);
			m_wLastBufferLength = m_wReadHeaderLength = (WORD)CFile::Read(m_pInternalBuf,m_nHeaderLength);
		}
		// reading rest of header
		if (m_lPosition + nCount >= m_wReadHeaderLength)
		{
			memcpy(&lpBuffer[nBytesRead], &m_pInternalBuf[m_nInternalPos], m_wReadHeaderLength - m_lPosition);
			nBytesRead      = m_wReadHeaderLength - m_lPosition;
			m_lPosition    += m_wReadHeaderLength - m_lPosition;
			m_nInternalPos  = 0;
		}
		// read header in one time
		else
		{
			memcpy(&lpBuffer[nBytesRead], &m_pInternalBuf[m_nInternalPos], nCount);
			nBytesRead      = nCount;
			m_lPosition    += nCount;	      
			m_nInternalPos += nCount;    
		}
	}
	return nBytesRead;
}		

int COXCompressedFile::InternalRead(WORD wCodedReadLength)
// In	: wCodedReadLength, the coded length of the buffer on disk	
// Out  :
// Returns : the decoded length of the buffer on disk
// Effect  : reads the information from disk (can be compressed) and puts it into m_pInternalBuf 
{
	unsigned char   pReadBuffer[m_wBufferLength];
	WORD			wReadLength;

	wReadLength = (WORD)(wCodedReadLength & ACTUAL_LENGTH);	
	if (wReadLength > m_wBufferLength)
	{
		TRACE0("In COXCompressedFile::InternalRead : Length to read is bigger than buffer length\n");
		AfxThrowFileException(CFileException::endOfFile);
	}

	// if the highest bit of wCodedReadLength is one bit the file is compressed
	if (wCodedReadLength & COMPRESS_BIT)
	{
		if (m_pDelegateToFile != NULL)
			m_pDelegateToFile->Read(pReadBuffer,wReadLength);
		else
			CFile::Read(pReadBuffer,wReadLength);
		m_nExpandedLength = m_Convert.Expand(pReadBuffer,wReadLength,m_pInternalBuf,m_wBufferLength);
	}
	else
	{
		if (m_pDelegateToFile != NULL)
			m_nExpandedLength = m_pDelegateToFile->Read(m_pInternalBuf,wReadLength);
		else
			m_nExpandedLength = CFile::Read(m_pInternalBuf,wReadLength);
	}
	ASSERT(m_nExpandedLength <= m_wBufferLength);
	m_wLastBufferLength = (WORD)m_nExpandedLength;	     
	return m_nExpandedLength;
}

UINT COXCompressedFile::WriteHeader(LPCTSTR lpBuffer, UINT &nCount)
// In	: lpBuffer, the header bytes to be written
//		  nCount, the number of bytes to be written
// Out  :
// Returns : number of bytes written
// Effect  : puts header bytes from character buffer on disk
{               
	UINT nBytesWritten = 0;        
	// when positioned in header	                                   
	if (m_lPosition < (long)m_nHeaderLength)
	{   
		memcpy(&m_pInternalBuf[m_nInternalPos], &lpBuffer[nBytesWritten],
			__min(nCount, m_nHeaderLength - m_nInternalPos));
		// when writing after header filling up rest of header
		if (m_lPosition + nCount >= m_nHeaderLength)
		{
			if (m_pDelegateToFile != NULL)
				m_pDelegateToFile->Write(m_pInternalBuf,m_nHeaderLength);
			else
				CFile::Write(m_pInternalBuf,m_nHeaderLength);

			nBytesWritten   = m_nHeaderLength - m_lPosition;
			m_lPosition    += m_nHeaderLength - m_lPosition;
			m_nInternalPos  = 0;
		}
		else
		{
			nBytesWritten   = nCount;
			m_lPosition    += nCount;	      
			m_nInternalPos += nCount;
		}
	}	
	return nBytesWritten;	
}

void COXCompressedFile::InternalWrite(BOOL bLast /* = FALSE */)
// In 	: 
// Out
// Returns
// Effect puts information from m_pInternalBuf on disk, compresses if compressed length > non 
//        compressed length
{
	int 			nCompressedLength;
	WORD			wWriteLength;
	// ... Buffer may become larger when we try to compress it
	//     In this case we will use the original data, but we still need to
	//     allocate enough memory for the larger compressed buffer (therefore + 1)
	unsigned char   pCompressedBuffer[m_wBufferLength + 1];

	nCompressedLength = m_Convert.Compress(m_pInternalBuf,m_nInternalPos,pCompressedBuffer,m_wBufferLength);

	// when compressed length exceeds original length, a non compressed buffer is written
	if (nCompressedLength == -1 || (UINT)nCompressedLength >= m_nInternalPos)
	{
		if (m_pDelegateToFile != NULL)
		{
			m_pDelegateToFile->Write(&m_nInternalPos,sizeof(WORD));
			m_pDelegateToFile->Write(m_pInternalBuf,m_nInternalPos);
		}
		else
		{
			CFile::Write(&m_nInternalPos,sizeof(WORD));
			CFile::Write(m_pInternalBuf,m_nInternalPos);
		}
	}
	else
	{
		wWriteLength = (WORD)(COMPRESS_BIT | nCompressedLength);

		// When writing the last buffer, set an extra identification bit
		if (bLast)
			wWriteLength |= TRUE_LENGTH;

		if (m_pDelegateToFile != NULL)
		{
			m_pDelegateToFile->Write(&wWriteLength,sizeof(WORD));
			// When writing the last buffer an extra identification bit	has been set to notify
			// a extra length ( = the true length = expanded length ) of this buffer is written
			if (bLast)
				m_pDelegateToFile->Write(&m_nInternalPos,sizeof(WORD));

			m_pDelegateToFile->Write(pCompressedBuffer,nCompressedLength);
		}
		else
		{
			CFile::Write(&wWriteLength,sizeof(WORD));

			// When writing the last buffer an extra identification bit	has been set to notify
			// a extra length ( = the true length = expanded length ) of this buffer is written
			if (bLast)
				CFile::Write(&m_nInternalPos,sizeof(WORD));

			CFile::Write(pCompressedBuffer,nCompressedLength);
		}
	}						
}			        

void COXCompressedFile::GetFinalPos()
{
	if (m_bEOF)
		return;

	char cTempHeaderBuffer[m_wBufferLength];

	// calculate begin position
	if (m_pDelegateToFile != NULL)
		m_pDelegateToFile->Seek(0,CFile::begin);
	else
		CFile::Seek(0,CFile::begin);

	m_lPosition          = 0;
	m_nInternalPos       = 0;
	m_nExpandedLength    = 0;
	m_nFirstBufferLength = 0;
	m_wReadHeaderLength  = 0;

	if (m_pDelegateToFile != NULL)
		m_wReadHeaderLength = (WORD)m_pDelegateToFile->Read(cTempHeaderBuffer, m_nHeaderLength);
	else
		m_wReadHeaderLength = (WORD)CFile::Read(cTempHeaderBuffer, m_nHeaderLength);
	m_lPosition = m_wReadHeaderLength;

	if (m_wReadHeaderLength == m_nHeaderLength)
	{
		// Move until EOF is reached, the starting index is 0 because we're just behind the header
		// ready to read the first block
		MoveFromBufferBoundaries(0, -1);
	}

	m_lFileLength = m_lPosition;
	m_bEOF = TRUE;
}	

void COXCompressedFile::AutoFlush(BOOL bLast /* = FALSE */)
{
	if (m_bOpenedForRead && !m_bOpenedForWrite)
	{
		TRACE(_T("In COXCompressedFile::Flush : Autoflushing a file that has been opened for read.\n"));
		return;
	}

	if (m_bOpenedForWrite)					
	{
		if (m_lPosition > (long)m_nHeaderLength)
		{
			if (m_nInternalPos > 0)
				InternalWrite(bLast);
		}
		else
		{
			if (m_pDelegateToFile != NULL)
				m_pDelegateToFile->Write(m_pInternalBuf,m_lPosition);
			else
				CFile::Write(m_pInternalBuf,m_lPosition);
		}
	}
}

DWORD COXCompressedFile::MoveFromBufferBoundaries(DWORD dwStartBufferIndex, long lOffset)
// In	: dwStartBufferIndex, zero based index of block to begin move from	
//		  lOffset : How far to move starting from dwStartBufferIndex
//					if lOffset is equal to -1 we loop the file until EOF
// Out  :
// Returns : the moved length
// Effect  : moves the virtual filepointer with an offset of lOffset starting from a buffer
//			 boundary.
{
	// Must be on a buffer boundary and behind the header
	ASSERT(m_nInternalPos == 0);
	ASSERT(m_lPosition >= m_wReadHeaderLength);

	WORD  	wCompressedLength = 0;
	long  	lSkipLength = 0;
	WORD  	wCompLengthCorrection = 0;
	WORD  	wTrueLengthCorrection = 0;
	DWORD 	dwDistance = 0;
	BOOL 	bEOF = FALSE;
	UINT 	nNumBuffers;
	UINT 	nOffsetInBuffer;

	// Initialize the while loop
	if (lOffset == -1)
	{
		nNumBuffers = 1;
		nOffsetInBuffer = 0;
	}
	else
	{
		nNumBuffers = lOffset / m_wBufferLength;
		nOffsetInBuffer = lOffset % m_wBufferLength;
	}

	while(nNumBuffers != 0 && !bEOF)
	{
		wCompLengthCorrection = sizeof(WORD);
		ASSERT((DWORD)m_BufLenArray.GetSize() >= dwStartBufferIndex);	// dwStartBufferIndex is zero based
		m_wLastBufferLength = m_wBufferLength;
		if ((DWORD)m_BufLenArray.GetSize() == dwStartBufferIndex)
			// The length of this buffer is not yet available in the list
		{
			// Execute the previous seeklength, because this is not a length we found in the array
			// and we need to position the true filepointer exactly before the length to be read
			if (m_pDelegateToFile != NULL)
				m_lPrevLengthPos = (long) m_pDelegateToFile->Seek(lSkipLength, CFile::current);
			else
				m_lPrevLengthPos = (long) CFile::Seek(lSkipLength, CFile::current);

			// Read the length
			UINT nRead(0);
			if (m_pDelegateToFile != NULL)
				nRead = m_pDelegateToFile->Read(&wCompressedLength, sizeof(WORD));
			else
				nRead = CFile::Read(&wCompressedLength, sizeof(WORD));

			if (nRead != sizeof(WORD))
				// Could not read a whole word, probably end of file
			{
				bEOF = TRUE;
			}
			else
			{
				// Store the length in the array
				m_BufLenArray.SetAtGrow(dwStartBufferIndex, wCompressedLength);
				m_wLastBufferLength = m_wBufferLength;

				// Is it the last buffer length we're reading here
				if (wCompressedLength & TRUE_LENGTH)
				{
					if (m_pDelegateToFile != NULL)
						m_pDelegateToFile->Read(&m_wLastBufferLength, sizeof(WORD));
					else
						CFile::Read(&m_wLastBufferLength, sizeof(WORD));
				}

				// By reading the length the true filepointer already placed itself behind the length
				wCompLengthCorrection = 0; 
				wTrueLengthCorrection = 0;
				lSkipLength = 0;
			}
		}

		if (!bEOF)
		{
			// Move the true filepointer just at the begining of the next buffer length
			wCompressedLength = m_BufLenArray.GetAt(dwStartBufferIndex);

			if ((wCompressedLength & TRUE_LENGTH) && wCompLengthCorrection != 0)
				// the TRUE_LENGTH bit is detected and we haven't added a new length to the 
				// array so the true filepointer hasn't moved yet after the lengths.
				// Add those lengths therefore to the skiplength.
			{
				wTrueLengthCorrection = sizeof(WORD);
			}

			lSkipLength += (wCompressedLength & ACTUAL_LENGTH) + wCompLengthCorrection + wTrueLengthCorrection;

			// Adjust the internals to this new position
			m_nInternalPos = 0;
			dwDistance += m_wLastBufferLength;
			m_lPosition += m_wLastBufferLength;

			// Adjust the counters
			dwStartBufferIndex++;
			if (lOffset != -1)
				// the loop is not meant to be until EOF
				nNumBuffers--;
		}
	}

	if (!bEOF)
	{
		if (lSkipLength != 0)
		{
			// There is still some length to be skipped
			if (m_pDelegateToFile != NULL)
				m_lPrevLengthPos = (long) m_pDelegateToFile->Seek(lSkipLength, CFile::current);
			else
				m_lPrevLengthPos = (long) CFile::Seek(lSkipLength, CFile::current);
		}

		ASSERT((DWORD)m_BufLenArray.GetSize() >= dwStartBufferIndex);	// dwStartBufferIndex is zero based
		wCompLengthCorrection = sizeof(WORD);
		if ((DWORD)m_BufLenArray.GetSize() == dwStartBufferIndex)
			// The length of this buffer is not yet available in the list
		{
			// Read the length
			UINT nRead(0);
			if (m_pDelegateToFile != NULL)
				nRead = m_pDelegateToFile->Read(&wCompressedLength, sizeof(WORD));
			else
				nRead = CFile::Read(&wCompressedLength, sizeof(WORD));

			if (nRead != sizeof(WORD))
				// Could not read a whole word, probably end of file
			{
				bEOF = TRUE;
			}
			else
			{
				// Store the length in the array
				m_BufLenArray.SetAtGrow(dwStartBufferIndex, wCompressedLength);
				m_wLastBufferLength = m_wBufferLength;

				// Is it the last buffer length we're reading here
				if (wCompressedLength & TRUE_LENGTH)
				{
					if (m_pDelegateToFile != NULL)
						m_pDelegateToFile->Read(&m_wLastBufferLength, sizeof(WORD));
					else
						CFile::Read(&m_wLastBufferLength, sizeof(WORD));
				}

				// By reading the length the true filepointer already placed itself behind the length
				wCompLengthCorrection = 0; 
			}
		}

		if (!bEOF)
		{
			// Get the length and move after it
			wCompressedLength = m_BufLenArray.GetAt(dwStartBufferIndex);
			// Is it the last buffer length we're reading here
			if ((wCompressedLength & TRUE_LENGTH) && wCompLengthCorrection != 0)
				// the TRUE_LENGTH bit is detected and we haven't added a new length to the 
				// array so the true filepointer hasn't moved yet after the lengths.
				// Add those lengths therefore to the skiplength.
			{
				wCompLengthCorrection += sizeof(WORD);
			}

			if (m_pDelegateToFile != NULL)
				m_pDelegateToFile->Seek(wCompLengthCorrection, CFile::current);
			else
				CFile::Seek(wCompLengthCorrection, CFile::current);

			// Read the buffer where the element to be sougth resides
			m_wLastBufferLength = (WORD)InternalRead(wCompressedLength);	

			// Adjust the internals to this new position
			int nTrueOffset = (nOffsetInBuffer < m_wLastBufferLength ? nOffsetInBuffer : m_wLastBufferLength);
			m_nInternalPos = nTrueOffset;
			dwDistance += nTrueOffset;
			m_lPosition += nTrueOffset;
		}
	}

	return dwDistance;
}

// private:

// Message handlers ---------------------------------------------------------

// ==========================================================================
