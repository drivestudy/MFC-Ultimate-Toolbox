// ==========================================================================
// 							Class Implementation : COXConvertedFile
// ==========================================================================

// Source file : convfile.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "convfile.h"	// class specification
#include <limits.h>		// for INT_MAX definition
#include "UTBStrop.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXConvertedFile, CFile)

#ifdef _DEBUG
// Helper macro for TRACE output
#define FILE_FROM_TEXT(nFrom) (nFrom == CFile::begin ?  TEXT("begin") :	\
	(nFrom == CFile::end ? TEXT("end") : TEXT("current")) )
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members


// Data members -------------------------------------------------------------
// protected:
// BOOL m_bEnabled;
// --- Whether conversion has been enabled or not
//     This only enables or disables the calls to the conversion finctions

// UINT m_nBufferLength;
// --- The length of the buffer (defined at construction time)

// LPBYTE m_pOriginalBuffer;
// --- The original (not converted) buffer itself (or NULL when not allocated)

// COXWatchBuffer	m_convertedBuffer;
// --- The converted buffer 

// int m_nBufferPos;
// --- Current position in the converted buffer

// DWORD m_nFilePos;
// --- The file position that corresponds with the beginning of buffer

// BOOL m_bRead;
// --- Whether the present contents of the buffer has been read from file

// BOOL m_bOpenedForRead;
// --- Whether the file has been opened for Read

// BOOL m_bOpenedForWrite;
// --- Whether the file has been opened for Write

// CFile* m_pDelegateToFile
// --- The next transformation in the transformation chain

// private:

// Member functions ---------------------------------------------------------
// public:

COXConvertedFile::COXConvertedFile(UINT wBufferLength, BOOL bConvertEnabled /* = TRUE */)
:
m_bEnabled(bConvertEnabled),
m_nBufferLength(wBufferLength),
m_pOriginalBuffer(NULL),
m_convertedBuffer(),
m_nBufferPos(0),
m_nFilePos(0),
m_bRead(FALSE),
m_bOpenedForRead(FALSE),
m_bOpenedForWrite(FALSE),
m_pDelegateToFile(NULL)
{
	// ... Must have strict positive buffer length
	ASSERT(0 < m_nBufferLength);
	m_pOriginalBuffer = new BYTE[m_nBufferLength];
	m_convertedBuffer.Create(m_nBufferLength);
	ASSERT_VALID(this);
}

UINT COXConvertedFile::GetBufferLength() const
{
	return m_nBufferLength;
}

BOOL COXConvertedFile::IsConvertEnabled() const
{
	ASSERT_VALID(this);
	return m_bEnabled;
}

BOOL COXConvertedFile::EnableConvert(BOOL bEnable /* = TRUE */)
{
	ASSERT_VALID(this);


	// Before changing the state, first flush everything
	if (m_bEnabled != bEnable)
	{
		// Check whether we have an empty or completely filled buffer,
		// because changing the conversion state is otherwise not possible
		if ((m_nBufferPos != 0) && ((UINT)m_nBufferPos != m_nBufferLength))
		{
			TRACE(TEXT("COXConvertedFile::EnableConvert : Changing the conversion state with half filled buffer is not allowed\n"));
			return FALSE;
		}
		ASSERT(GetPosition() % GetBufferLength() == 0);
		Flush();

		m_bEnabled = bEnable;
	}
	return TRUE;
}

WORD COXConvertedFile::ForceEnableConvert(BOOL bEnable /* = TRUE */)
{
	// Even when  m_bEnabled == bEnable  bytes are skipped until the next
	//  buffer boundary, this is for compatibility reasons

	WORD wBytesSkipped = 0;

	// Check whether we have an empty or completely filled buffer,
	if (IsOpen() && (m_nBufferPos != 0) && ((UINT)m_nBufferPos != m_nBufferLength))
	{
		// Not completely filled buffer, skip bytes
		wBytesSkipped = WORD(m_nBufferLength - m_nBufferPos);
		LPBYTE pSkippedBytes = new BYTE[wBytesSkipped];

		TRY
		{
			// ... File is open, so must be reading or writing
			ASSERT(m_bOpenedForRead || m_bOpenedForWrite);
			if (m_bOpenedForWrite)
			{
				/// .... Write '\0'-bytes
				memset(pSkippedBytes, 0, wBytesSkipped);
				Write(pSkippedBytes, wBytesSkipped);
			}
			else
			{
				// ... Must be able to read to the end of the buffer
				VERIFY(wBytesSkipped == Read(pSkippedBytes, wBytesSkipped));
#ifdef _DEBUG				
				// Check whether the skipped bytes are all '\0'
				WORD nByteIndex = 0;
				while ((nByteIndex < wBytesSkipped) && (pSkippedBytes[nByteIndex] == '\0'))
					nByteIndex++;
				if (nByteIndex < wBytesSkipped)
					TRACE(TEXT("COXConvertedFile::ForceEnableConvert : Skipping non-zero bytes\n"));
#endif				
			}
		}
		CATCH_ALL(px) 
		{
			delete[] pSkippedBytes;
			THROW_LAST();
		}
		END_CATCH_ALL
			delete[] pSkippedBytes;
	}

	// ... Should be able to change conversion state now
	VERIFY(EnableConvert(bEnable));

	return wBytesSkipped;
}

BOOL COXConvertedFile::IsOpen() const
{
	ASSERT_VALID(this);
	if (m_pDelegateToFile != NULL)
		return (m_pDelegateToFile->m_hFile != CFile::hFileNull);
	else
		return (m_hFile != CFile::hFileNull);
}

CFile* COXConvertedFile::Duplicate() const
{
	ASSERT_VALID(this);
	if (m_pDelegateToFile != NULL)
		return m_pDelegateToFile->Duplicate();
	else
		return CFile::Duplicate();
}

BOOL COXConvertedFile::Open(LPCTSTR pszFileName, UINT nOpenFlags,
							CFileException* pException /* = NULL */)
{
	ASSERT_VALID(this);
	ASSERT(m_pDelegateToFile == NULL);

	// ... Store open flag
	m_bOpenedForWrite = (nOpenFlags & CFile::modeWrite) || (nOpenFlags & CFile::modeReadWrite);
	m_bOpenedForRead = TRUE;	// Read always permitted CFile::modeRead == 0x0000,

	// ... Beginning of buffer and file
	m_nBufferPos = 0;
	m_nFilePos = 0;

	// ... Nothing actually read
	m_bRead = FALSE;

	// Call base member implementation
	BOOL bResult = CFile::Open(pszFileName, nOpenFlags, pException);
	ASSERT_VALID(this);
	return bResult;
}

BOOL COXConvertedFile::DelegateOpen(CFile* pDelegateToFile, UINT nOpenFlags)
{
	ASSERT_VALID(this);
	ASSERT(pDelegateToFile != NULL);

	// ... Store open flag
	m_bOpenedForWrite = (nOpenFlags & CFile::modeWrite) || (nOpenFlags & CFile::modeReadWrite);
	m_bOpenedForRead = TRUE;	// Read always permitted CFile::modeRead == 0x0000,

	// ... Beginning of buffer and file
	m_nBufferPos = 0;
	m_nFilePos = 0;

	// ... Nothing actually read
	m_bRead = FALSE;

	// Call base member implementation
	m_pDelegateToFile = pDelegateToFile;

	ASSERT_VALID(this);
	return TRUE;
}

BOOL COXConvertedFile::UnDelegateOpen()
{
	m_bEnabled = FALSE;

	m_pDelegateToFile = NULL;

	m_convertedBuffer.Empty();
	m_nBufferPos = 0;
	m_nFilePos = 0;

	m_bRead = FALSE;
	m_bOpenedForRead = FALSE;
	m_bOpenedForWrite = FALSE;

	return TRUE;
}

UINT COXConvertedFile::Read(void FAR* lpBuf, UINT nCount)
{
	ASSERT_VALID(this);
	ASSERT(lpBuf != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nCount));

	// Immediately return for empty requests
	if (nCount <= 0)
		return 0;  

	if (!m_bOpenedForRead)
	{
		TRACE(TEXT("COXConvertedFile::Read : Trying to read while not permitted, throwing exception\n"));
		AfxThrowFileException(CFileException::accessDenied);
	}

	UINT nCopy = 0;
	UINT nRead = 0;
	BOOL bEOF = FALSE;

	// If buffer does not yet contain information from file, read now
	if (!m_bRead)
		bEOF = ReadBuffer();

	while ((nRead < nCount) && !bEOF)
	{
		// First give caller the bytes that are in the buffer
		nCopy = __min(nCount - nRead, (UINT)(m_nBufferLength - m_nBufferPos));
		if (nCopy != 0)
			memcpy(lpBuf, m_convertedBuffer.Get(m_nBufferPos), nCopy);
		lpBuf = LPBYTE(lpBuf) + nCopy;
		m_nBufferPos += nCopy;
		nRead += nCopy;

		// If still bytes needed : read a new buffer
		if (nRead < nCount)
			bEOF = ReadBuffer();
	}
	// ... Should not have read more bytes than asked for
	ASSERT(nRead <= nCount);
	return nRead;
}

void COXConvertedFile::Write(const void FAR* lpBuf, UINT nCount)
{
	ASSERT_VALID(this);
	ASSERT(lpBuf != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nCount, FALSE));

	// Immediately return for empty requests
	if (nCount == 0)
		return;

	if (!m_bOpenedForWrite)
	{
		TRACE(TEXT("COXConvertedFile::Write : Trying to write while not permitted, throwing exception\n"));
		AfxThrowFileException(CFileException::accessDenied);
	}

	UINT nCopy = 0;
	UINT nWrite = 0;

	while (nWrite < nCount)
	{
		// First write bytes in the remaining part of the buffer
		nCopy = __min(nCount - nWrite, UINT(m_nBufferLength - m_nBufferPos));
		m_convertedBuffer.Set(m_nBufferPos, (LPBYTE)lpBuf, nCopy);
		lpBuf = LPBYTE(lpBuf) + nCopy;
		m_nBufferPos += nCopy;
		nWrite += nCopy;

		// If still bytes to write : write the old buffer
		if (nWrite < nCount)
		{
			// ... Must be at end of buffer
			ASSERT((UINT)m_nBufferPos == m_nBufferLength);
			// ... Flush old buffer
			FlushBuffer();
			// ... Set new bufferpos after the present one
			CheckBufferPosition();
		}
	}
	// ... Should have written the ewact amount of bytes that was asked for
	ASSERT(nWrite == nCount);
	ASSERT_VALID(this);
}

LPSTR COXConvertedFile::ReadString(LPSTR psz, UINT nMax)
{
	// Remark : Carriage Return CR = '\r', Line Feed LF = '\n', 
	//			End-Of-File character = '\x1A'

	// Must have non-trival buffer
	ASSERT(psz != NULL);
	ASSERT(1 <= nMax);
	// ... Given buffer must be valid (for data AND zero-terminator)
	ASSERT(AfxIsValidAddress(psz, nMax));

	BOOL bEOL = FALSE;
	BOOL bEOF = FALSE;
	// ... Position to receive the next charater read
	char* pszOut = psz;
	// ... Last position in buffer (can only be used for zero-terminator)
	char* pszLast = &psz[nMax - 1];

	// Loop while buffer not full, not End-Of-Line and not End-Of-File	
	while ((pszOut != pszLast) &&!bEOL && !bEOF)
	{
		// EOF has been reached when nothing more can be read, or EOF-character was read
		bEOF = (Read(pszOut, 1) == 0);
		if (!bEOF && (*pszOut == '\x1A'))
		{
			bEOF = TRUE,
				// ... Unread EOF character, so that subsequent Reads will read it again
				Seek(-1, CFile::current);
		}
		if (!bEOF)
		{
			// When LF encountered, we reached EOL
			if (*pszOut == '\n')
			{
				bEOL = TRUE;
				// ... Check whether previous read character was CR, if so remove it
				if ((psz <= pszOut - 1) && (*(pszOut - 1) == '\r'))
					*(pszOut - 1) = '\n';
				else
					pszOut++;
			}
			else
				pszOut++;
		}

	}

	// If last character read was CR, perhaps a LF will follow
	if (!bEOF && (psz <= pszOut - 1) && (*(pszOut - 1) == '\r'))
	{
		char cNext;
		UINT uCount;
		if (m_pDelegateToFile != NULL)
			uCount = m_pDelegateToFile->Read(&cNext, 1);
		else
			uCount = Read(&cNext, 1);

		if (uCount == 1)
		{
			if (cNext == '\n')
				// ... Replace CR by LF
				*(pszOut - 1) = '\n';
			else
			{
				// ... Unread character
				Seek(-1, CFile::current);
			}
		}
	}

	// ... Add zero-terminator
	*pszOut = '\0';
	return (psz < pszOut ? psz : NULL);
}

void COXConvertedFile::WriteString(LPCSTR psz)
{
	// Remark : Carriage Return CR = '\r' and Line Feed LF = '\n'

	// Must have valid buffer
	ASSERT(psz != NULL);

	LPCSTR pszOut = psz;
	LPCSTR pszLF;

	while (*pszOut != '\0')
	{
		// ... Search for first LF character
		pszLF = strchr(pszOut, '\n');
		if (pszLF != NULL)
		{
			// ... Write data, followed by CR-LF pair
			Write(pszOut, (UINT)(pszLF - pszOut));
			Write(TEXT("\r\n"), 2);
			// ... Position have LF character
			pszOut = pszLF + 1;
		}
		else 
		{
			// ... Write the remaining data
			Write(pszOut, (UINT)strlen(pszOut));
			// ... Position on zero-terminator
			pszOut += strlen(pszOut);
		}
	}
}

#ifdef WIN32
// UNICODE conversion
LPSTR COXConvertedFile::ReadString(LPWSTR psz, UINT nMax)
{
	// Convert UNICODE string to ANSI
	LPSTR pszAnsi = new char[nMax + 1];
	size_t c;
	UTBStr::wcstombs(&c, pszAnsi, nMax, psz, nMax);

	LPSTR pszResult = ReadString(pszAnsi, nMax);

	delete [] pszAnsi;

	return pszResult;
}

void COXConvertedFile::WriteString(LPCWSTR psz)
{
	const size_t nMaxAnsiLength = wcslen(psz) * 2 + 1;
	size_t nRequiredSize;
	UTBStr::wcstombs(&nRequiredSize, NULL, nMaxAnsiLength, psz, nMaxAnsiLength);
	LPSTR pszAnsi = new char[nRequiredSize + 1];
	UTBStr::wcstombs(&nRequiredSize, pszAnsi, nRequiredSize, psz, nRequiredSize + 1);

	WriteString(pszAnsi);

	delete [] pszAnsi;
}

#endif // WIN32

LONG COXConvertedFile::Seek(LONG lOff, UINT nFrom)
{
	// Remark : 
	//	This function calls CFile::GetLength() 
	//	 which calls  CFile::SeekToEnd()
	//	 which calls the virtual Seek()
	//	 and thus arrives in this COXConvertedFile::Seek() again
	// 	That's why this function uses a private version of CFile::GetLength()
	ASSERT_VALID(this);
	DWORD lAbsoluteFilePos;
	DWORD nNewFilePos;
	switch (nFrom)
	{
	case CFile::begin:
		lAbsoluteFilePos = lOff;
		break;
	case CFile::current:
		lAbsoluteFilePos = (DWORD) GetPosition() + lOff;
		break;
	case CFile::end:
		lAbsoluteFilePos = GetLengthPrivate() + lOff;
		break;
	default:
		TRACE(TEXT("COXConvertedFile::Seek : Unexpected case in switch nFrom = %u\n"), nFrom);
		ASSERT(FALSE);
		lAbsoluteFilePos = 0;
		break;
	}
	// ... Align on buffer boundaries
	nNewFilePos = (lAbsoluteFilePos / m_nBufferLength) * m_nBufferLength;
	if (nNewFilePos != m_nFilePos)
		// New position outside current buffer
	{
		// ... Flush the old buffer
		Flush();
		// ... Init new buffer to correct position and mark as unread
		m_nFilePos = nNewFilePos;
		m_bRead = FALSE;
	}

	// ... Compute new position in buffer		
	ASSERT(lAbsoluteFilePos - m_nFilePos < INT_MAX);;
	m_nBufferPos = int(lAbsoluteFilePos - m_nFilePos);
	ASSERT((0 <= m_nBufferPos) && ((UINT)m_nBufferPos <= m_nBufferLength));


#ifdef _DEBUG
	// TRACE(TEXT("COXConvertedFile::Seek : Converting from (%s, %li) to (%s, %li)\n"), 
	//			FILE_FROM_TEXT(nFrom), lOff, FILE_FROM_TEXT(CFile::begin), m_nFilePos);
#endif

	ASSERT_VALID(this);
	if (m_pDelegateToFile != NULL)
		return (LONG) m_pDelegateToFile->Seek(m_nFilePos, CFile::begin) + m_nBufferPos;
	else
		return (LONG) CFile::Seek(m_nFilePos, CFile::begin) + m_nBufferPos;
}

#if _MFC_VER >= 0x0700
ULONGLONG COXConvertedFile::GetPosition() const
#else
DWORD COXConvertedFile::GetPosition() const
#endif
{
	ASSERT_VALID(this);
	return m_nFilePos + m_nBufferPos;
}

void COXConvertedFile::Flush()
{
	// Flush unwritten bytes to file
	ASSERT_VALID(this);

	// If writing, write the buffer now
	if (m_bOpenedForWrite)
	{
		FlushBuffer();
		// ... To be compatible with WIN32 CFile::Flush() will only be called 
		//     when yje file is opened for writing
		if (m_pDelegateToFile != NULL)
			m_pDelegateToFile->Flush();
		else
			CFile::Flush();
	}

	// If reading mark it as unread
	m_bRead = FALSE;
}

void COXConvertedFile::Close()
{
	ASSERT_VALID(this);
	Flush();
	if (m_pDelegateToFile == NULL)
		CFile::Close();

	m_bOpenedForRead = FALSE;
	m_bOpenedForWrite = FALSE;
	m_nBufferPos = 0;
	m_nFilePos = 0;
	m_bRead = FALSE;
}

void COXConvertedFile::Abort()
{
	if (m_pDelegateToFile != NULL)
		m_pDelegateToFile->Abort();
	else
		CFile::Abort();

	m_bOpenedForRead = FALSE;
	m_bOpenedForWrite = FALSE;
	m_nBufferPos = 0;
	m_nFilePos = 0;
	m_bRead = FALSE;
}

void COXConvertedFile::LockRange(DWORD dwPos, DWORD dwCount)
{
	ASSERT_VALID(this);
	// ... Round down starting position 
	DWORD dwNewPos = (dwPos / m_nBufferLength) * m_nBufferLength;
	// ... Round up count
	DWORD dwNewCount = ((dwCount + m_nBufferLength - 1) / m_nBufferLength) * m_nBufferLength;

	TRACE(TEXT("COXConvertedFile::LockRange : Converting from %lu %lu to %lu %lu\n"), 
		dwPos, dwCount, dwNewPos, dwNewCount);

	if (m_pDelegateToFile != NULL)
		m_pDelegateToFile->LockRange(dwNewPos, dwNewCount);
	else
		CFile::LockRange(dwNewPos, dwNewCount);
}

void COXConvertedFile::UnlockRange(DWORD dwPos, DWORD dwCount)
{
	ASSERT_VALID(this);
	// ... Round down starting position 
	DWORD dwNewPos = (dwPos / m_nBufferLength) * m_nBufferLength;
	// ... Round up count
	DWORD dwNewCount = ((dwCount + m_nBufferLength - 1) / m_nBufferLength) * m_nBufferLength;

	TRACE(TEXT("COXConvertedFile::UnlockRange : Converting from %lu %lu to %lu %lu\n"), 
		dwPos, dwCount, dwNewPos, dwNewCount);
	if (m_pDelegateToFile != NULL)
		m_pDelegateToFile->UnlockRange(dwNewPos, dwNewCount);
	else
		CFile::UnlockRange(dwNewPos, dwNewCount);
}

void COXConvertedFile::SetLength(DWORD dwNewLen)
{
	ASSERT_VALID(this);
	// ... Round up length
	DWORD dwNewNewLen = ((dwNewLen + m_nBufferLength - 1) / m_nBufferLength) * m_nBufferLength;
	TRACE(TEXT("COXConvertedFile::SetLength : Converting from %lu to %lu\n"), 
		dwNewLen, dwNewNewLen);
	if (m_pDelegateToFile != NULL)
		m_pDelegateToFile->SetLength(dwNewNewLen);
	else
		CFile::SetLength(dwNewNewLen);
}

#if _MFC_VER >= 0x0700
ULONGLONG COXConvertedFile::GetLength() const
#else
DWORD COXConvertedFile::GetLength() const
#endif
{
	ASSERT_VALID(this);
	// TRACE(TEXT("COXConvertedFile::GetLength : returning %lu, which might rounded\n"), CFile::GetLength());

	// CFile::GetLength will internally call COXConvertedFile::Seek() to determine the length
	//  this will return the adjusted langth
	if (m_pDelegateToFile != NULL)
		return m_pDelegateToFile->GetLength();
	else
		return CFile::GetLength();
}

// --------------------------------------------------------------------------------------


#ifdef _DEBUG
void COXConvertedFile::Dump(CDumpContext& dc) const
{
	if (m_pDelegateToFile != NULL)
		m_pDelegateToFile->Dump(dc);
	else
		CFile::Dump(dc);

	dc << TEXT("\nm_bEnabled : ") << 		(WORD)m_bEnabled;
	dc << TEXT("\nm_nBufferLength : ") << 	(WORD)m_nBufferLength;
	dc << TEXT("\nm_pOriginalBuffer : ") <<	(char *)m_pOriginalBuffer;
	dc << TEXT("\nm_convertedBuffer ") << 	m_convertedBuffer;
	dc << TEXT("\nm_nBufferPos ; ") << 		(WORD)m_nBufferPos;
	dc << TEXT("\nm_nFilePos : ")	<< 		m_nFilePos;
	dc << TEXT("\nm_bRead : ") << 			(WORD)m_bRead;
	dc << TEXT("\nm_bOpenedForRead : ") << 	(WORD)m_bOpenedForRead;
	dc << TEXT("\nm_bOpenedForWrite : ") <<	(WORD)m_bOpenedForWrite;
}

void COXConvertedFile::AssertValid() const
{
	if (m_pDelegateToFile != NULL)
		m_pDelegateToFile->AssertValid();
	else
		CFile::AssertValid();
	ASSERT(0 <= m_nBufferPos);
	ASSERT((UINT)m_nBufferPos <= m_nBufferLength);
	ASSERT(AfxIsValidAddress(m_pOriginalBuffer, m_nBufferLength));
	ASSERT(AfxIsValidAddress(m_convertedBuffer.Get(), m_nBufferLength));
}
#endif

COXConvertedFile::~COXConvertedFile()
{
	delete[] m_pOriginalBuffer;
	m_convertedBuffer.Destroy();
}

// protected:
DWORD COXConvertedFile::GetLengthPrivate() const
{
	DWORD dwLen, dwCur;

	// Seek is a non const operation
	if (m_pDelegateToFile != NULL)
	{
		dwCur = (DWORD) m_pDelegateToFile->Seek(0L, CFile::current);
		// ... SeekToEnd() is replaced by Seek(0, CFile::end)
		dwLen = (DWORD) m_pDelegateToFile->Seek(0L, CFile::end);
		VERIFY(dwCur == (DWORD)(m_pDelegateToFile->Seek(dwCur, CFile::begin)));
	}
	else
	{
		dwCur = (DWORD) ((CFile*)this)->CFile::Seek(0L, CFile::current);
		// ... SeekToEnd() is replaced by Seek(0, CFile::end)
		dwLen = (DWORD) ((CFile*)this)->CFile::Seek(0, CFile::end);
		VERIFY(dwCur == (DWORD)(((CFile*)this)->CFile::Seek(dwCur, CFile::begin)));
	}

	return dwLen;
}

BOOL COXConvertedFile::ReadBuffer()
// --- In  : 
// --- Out : 
// --- Returns : TRUE if no data was read because EOF has been reached
//				 If EOF has been reached but some data could still be returned
//				  FALSE will be returned
// --- Effect : Reads a new contents for the internal buffer
{
	UINT nRead;

	// ... Flush buffer when it contains modified info
	FlushBuffer();

	// ... If already at the end of this buffer, move file position to the next
	CheckBufferPosition();

	// ... Set actual file position to the correct position
	//	   (must be on buffer boundary)
	ASSERT(m_nFilePos % m_nBufferLength == 0);
	if (m_pDelegateToFile != NULL)
		m_pDelegateToFile->Seek(m_nFilePos, CFile::begin);
	else
		CFile::Seek(m_nFilePos, CFile::begin);

	// ... Read buffer from file
	memset(m_pOriginalBuffer, 0, m_nBufferLength);
	if (m_pDelegateToFile != NULL)
		nRead = m_pDelegateToFile->Read(m_pOriginalBuffer, m_nBufferLength);
	else
		nRead = CFile::Read(m_pOriginalBuffer, m_nBufferLength);
	if (IsConvertEnabled())
		VERIFY(ConvertRead(m_pOriginalBuffer, m_convertedBuffer.Get()));
	else
		memcpy(m_convertedBuffer.Get(), m_pOriginalBuffer, m_nBufferLength);

	// ... If not EOF, mark as actually read from file
	m_bRead = (nRead != 0);

	return !m_bRead;
}

void COXConvertedFile::FlushBuffer()
// --- In  :
// --- Out : 
// --- Returns : 
// --- Effect : Writes the contents of the internal buffer to file
//				when it was modified and not yet written
//				It does not change any internal buffer positions
{
	if (!m_convertedBuffer.IsModified())
		return;

	// ... Must have write access
	ASSERT(m_bOpenedForWrite);
	// If the contents of the buffer was originally read from file
	// or all the buffer was overwritten : OK
	if (!m_bRead && m_convertedBuffer.GetUnmodified() != -1)
	{
		// There exist some bytes in the buffer that do not originate from file
		// and are never overwritten
		// Now get the original contents for the unmodified bytes

		// ... Must be able to read as well
		ASSERT(m_bOpenedForRead);
		int nRead;
		int nUnmodPos;
		LPBYTE pTempOrgBuffer = new BYTE[m_nBufferLength];
		LPBYTE pTempConvBuffer = new BYTE[m_nBufferLength];
		memset(pTempOrgBuffer, 0, m_nBufferLength);

		// ... Go back to the original beginning of the buffer
		if (m_pDelegateToFile != NULL)
		{
			m_pDelegateToFile->Seek(m_nFilePos, CFile::begin);
			nRead = m_pDelegateToFile->Read(pTempOrgBuffer, m_nBufferLength);
		}
		else
		{
			CFile::Seek(m_nFilePos, CFile::begin);
			nRead = CFile::Read(pTempOrgBuffer, m_nBufferLength);
		}
		if (IsConvertEnabled())
			VERIFY(ConvertRead(pTempOrgBuffer, pTempConvBuffer));
		else
			memcpy(pTempConvBuffer, pTempOrgBuffer, m_nBufferLength);

		// ... While there exist still bytes from file that are not in the buffer
		//     copy them
		while (	((nUnmodPos = m_convertedBuffer.GetUnmodified()) != -1) &&
			(nUnmodPos <= nRead) )
		{
			// ... This will change the state from unmodified to modified
			ASSERT(!m_convertedBuffer.IsModified(nUnmodPos));
			m_convertedBuffer.Set(nUnmodPos, &pTempConvBuffer[nUnmodPos]);
			ASSERT(m_convertedBuffer.IsModified(nUnmodPos));
		}
		// ... Now no unmodified bytes exist anymore or
		//     their corresponding bytes in the file do not exist
		ASSERT((m_convertedBuffer.GetUnmodified() == -1) ||
			(nRead < m_convertedBuffer.GetUnmodified()));
		m_convertedBuffer.SetModified(TRUE);

		delete[] pTempOrgBuffer;
		delete[] pTempConvBuffer;
	}

	// ... Set actual file position to the correct position
	//	   (must be on buffer boundary)
	ASSERT(m_nFilePos % m_nBufferLength == 0);
	if (m_pDelegateToFile != NULL)
		m_pDelegateToFile->Seek(m_nFilePos, CFile::begin);
	else
		CFile::Seek(m_nFilePos, CFile::begin);

	// Actually write to file
	if (IsConvertEnabled())
		VERIFY(ConvertWrite(m_convertedBuffer.Get(), m_pOriginalBuffer));
	else
		memcpy(m_pOriginalBuffer, m_convertedBuffer.Get(), m_nBufferLength);

	if (m_pDelegateToFile != NULL)
		m_pDelegateToFile->Write(m_pOriginalBuffer, m_nBufferLength);
	else
		CFile::Write(m_pOriginalBuffer, m_nBufferLength);

	// ... Mark as unmodified and read from file (because we have just written it)
	m_convertedBuffer.SetModified(FALSE);
	m_bRead = TRUE;
}

void COXConvertedFile::CheckBufferPosition()
// --- In  :
// --- Out : 
// --- Returns : 
// --- Effect : When the buffer position is after the end of the buffer
//			 	put it on 0 and move the file position
{
	if ((UINT)m_nBufferPos == m_nBufferLength)
	{
		m_nFilePos += m_nBufferLength;
		m_nBufferPos = 0;
		m_convertedBuffer.Empty();
		m_bRead = FALSE;
	}
}
// private:

// Message handlers ---------------------------------------------------------

// ==========================================================================
