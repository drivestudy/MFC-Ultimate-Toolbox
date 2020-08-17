// ==========================================================================
// 							Class Implementation : 
//							      COXTrace
// ==========================================================================

// Source code file : OXTrace.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
#include "stdafx.h"
#include "OXTrace.h"

#include "UTBStrOp.h"
#include "UTB64Bit.h"

#if _MFC_VER<=0x0421

#define TCHAR_ARG   TCHAR
#define WCHAR_ARG   WCHAR
#define CHAR_ARG    char

#ifdef _X86_
	#define DOUBLE_ARG  _AFX_DOUBLE
#else
	#define DOUBLE_ARG  double
#endif

#define FORCE_ANSI      0x10000
#define FORCE_UNICODE   0x20000
#define FORCE_INT64     0x40000
// printf-like formatting using variable arguments parameter
void FormatV(CString& sText, LPCTSTR lpszFormat, va_list argList);

#endif

//
// Static member variables
//
int		COXTrace::m_nIndent = 0;			// current indentation level
int		COXTrace::m_nIndentStep = 4;		// current indentation step
BOOL	COXTrace::m_bTracing = TRUE;		// current tracing state
int		COXTrace::m_nWrapWidth = 120;		// wrap width for messages
BOOL	COXTrace::m_bPrefixOn = FALSE;		// whether or not mMsg should be prepended to messages
CFile	COXTrace::m_File;					// file to copy trace messages to
BOOL	COXTrace::m_bFlushOnWrite = FALSE;	// should each line be flushed when written

COXTrace::COXTrace(LPCTSTR lpstrMsg)
	: m_strHeader(lpstrMsg)
{
	if (m_bTracing)
	{
		//
		// Let the user know that a new tracing block has started
		//
		WriteMsg(lpstrMsg);
	}

	//
	// Increment the indentation level
	//
	m_nIndent += m_nIndentStep;
}

COXTrace::~COXTrace()
{
	//
	// Decrement the indentation level first
	//
	m_nIndent -= m_nIndentStep;

	if (m_bTracing)
	{
		//
		// Let the user know that this tracing block has ended
		//
		WriteMsg(_T("==== ")+m_strHeader + _T(" exit"));
	}
}

void COXTrace::SetDumpFile(LPCTSTR lpstrFilename, 
						   UINT nOpenFlags/*=CFile::modeWrite|
										  CFile::shareDenyWrite|
										  CFile::modeCreate*/)
{
	if (m_File.m_hFile != CFile::hFileNull)
		m_File.Close();

	m_File.Open(lpstrFilename, nOpenFlags);
}

/**
 * Write
 *	Write out a text message
 *
 * --- In      : lpstrMsg : Pointer to the text to be written out.
 * --- Out     : 
 * --- Returns : 
 * --- Effect  : The debugging console, and possiblely the dump file, will have this
 *				 message sent to them for viewing.
 */
void COXTrace::Write(LPCTSTR lpstrMsg)
{
	if (m_bTracing)  
	{
		if (m_bPrefixOn)
			WriteMsg(m_strHeader + _T(" : ") + lpstrMsg);
		else
			WriteMsg(lpstrMsg);
	}
}

/**
 * Write
 *	Write out a text message, followed by the passed in value.
 *
 * --- In      : lpstrMsg : Pointer to the text to be written out.
 *				 nValue : Integer value to append to the end of lpstrMsg
 * --- Out     : 
 * --- Returns : 
 * --- Effect  : The debugging console, and possiblely the dump file, will have this
 *				 message sent to them for viewing.
 */
void COXTrace::Write(LPCTSTR lpstrMsg, int nValue)
{
  	WriteFormatted(_T("%s %d"), (LPCTSTR)lpstrMsg, nValue);
}

/**
 * Write
 *	Write out a text message, followed by the passed in value.
 *
 * --- In      : lpstrMsg : Pointer to the text to be written out.
 *				 unValue : Unsigned integer value to append to the end of lpstrMsg
 * --- Out     : 
 * --- Returns : 
 * --- Effect  : The debugging console, and possiblely the dump file, will have this
 *				 message sent to them for viewing.
 */
void COXTrace::Write(LPCTSTR lpstrMsg, unsigned int unValue)
{
  	WriteFormatted(_T("%s %u"), (LPCTSTR)lpstrMsg, unValue);
}

/**
 * Write
 *	Write out a text message, followed by the passed in value.
 *
 * --- In      : lpstrMsg : Pointer to the text to be written out.
 *				 lValue : Long integer value to append to the end of lpstrMsg
 * --- Out     : 
 * --- Returns : 
 * --- Effect  : The debugging console, and possiblely the dump file, will have this
 *				 message sent to them for viewing.
 */
void COXTrace::Write(LPCTSTR lpstrMsg, long lValue)
{
  	WriteFormatted(_T("%s %ld"), (LPCTSTR)lpstrMsg, lValue);
}

/**
 * Write
 *	Write out a text message, followed by the passed in value.
 *
 * --- In      : lpstrMsg : Pointer to the text to be written out.
 *				 ulValue : Unsigned long integer value to append to the end of lpstrMsg
 * --- Out     : 
 * --- Returns : 
 * --- Effect  : The debugging console, and possiblely the dump file, will have this
 *				 message sent to them for viewing.
 */
void COXTrace::Write(LPCTSTR lpstrMsg, unsigned long ulValue)
{
	WriteFormatted(_T("%s %lu"), (LPCTSTR)lpstrMsg, ulValue);
}

/**
 * Write
 *	Write out a text message, followed by the passed in value.
 *
 * --- In      : lpstrMsg : Pointer to the text to be written out.
 *				 fValue : Floating point value to append to the end of lpstrMsg
 * --- Out     : 
 * --- Returns : 
 * --- Effect  : The debugging console, and possiblely the dump file, will have this
 *				 message sent to them for viewing.
 */
void COXTrace::Write(LPCTSTR lpstrMsg, double fValue)
{
	WriteFormatted(_T("%s %f"), (LPCTSTR)lpstrMsg, fValue);
}

/**
 * Write
 *	Write out a text message, followed by the passed in value.
 *
 * --- In      : lpstrMsg : Pointer to the text to be written out.
 *				 lpstrValue : Text string to append to the end of lpstrMsg
 * --- Out     : 
 * --- Returns : 
 * --- Effect  : The debugging console, and possibly the dump file, will have this
 *				 message sent to them for viewing.
 */
void COXTrace::Write(LPCTSTR lpstrMsg, LPCTSTR lpstrValue)
{
	if (m_bPrefixOn)
		WriteMsg(m_strHeader + _T(" : ") + lpstrMsg + lpstrValue);
	else
	{
		CString	strMsg(lpstrMsg);

		WriteMsg(strMsg + lpstrValue);
	}
}

/**
 * Write
 *	Write out a text message, followed by the passed in value. In this case, the 
 * object's Dump() method will be called, if _DEBUG is defined. The data will be
 * dumped only to the console.
 *
 * --- In      : lpstrMsg : Pointer to the text to be written out.
 *				 rObject : Reference to a CObject derived object.
 * --- Out     : 
 * --- Returns : 
 * --- Effect  : The debugging console, and possiblely the dump file, will have this
 *				 message sent to them for viewing.
 */
void COXTrace::Write(LPCTSTR lpstrMsg, const CObject& rObject)
{
	CString strMsg(lpstrMsg);
	CString strSep = _T("==========");

	//
	// Remove the trailing _T(" == ") added by the macros
	//
	if (strMsg.Right(4).Compare(_T(" == ")) == 0)
		strMsg = strMsg.Left(strMsg.GetLength() - 4);

	Write(strSep + _T(" ") + strMsg + _T(" dump ") + strSep);

	if (m_File.m_hFile != CFile::hFileNull)
	{
		//
		// Write the data out to the output file
		//
#ifdef _DEBUG
		CMemFile memFile;
		CDumpContext context(&memFile);
		context.SetDepth(1);
		rObject.Dump(context);

		memFile.SeekToBegin();
		TCHAR szSymbolToAdd[2];
		szSymbolToAdd[1]=_T('\0');
		TCHAR chSymbol;
		DWORD dwSymbolCount= (DWORD) memFile.GetLength()/sizeof(TCHAR);
		CString sLineToAdd;
		BOOL bFirstLine=TRUE;
		DWORD dwCurSymbol=0;
		COXTrace* pTrace=NULL;
		while(dwCurSymbol<dwSymbolCount)
		{
			VERIFY(memFile.Read(&chSymbol,sizeof(TCHAR))==sizeof(TCHAR));
			if(chSymbol==_T('\n'))
			{
				if(bFirstLine)
				{
					pTrace=new COXTrace(sLineToAdd);
					bFirstLine=FALSE;
				}
				else
				{
					ASSERT(pTrace!=NULL);
					pTrace->Write(sLineToAdd);
				}
				sLineToAdd.Empty();
			}
			else
			{
				szSymbolToAdd[0]=chSymbol;
				sLineToAdd+=szSymbolToAdd;
			}
			dwCurSymbol++;
		}
		if(pTrace!=NULL)
		{
			if(!sLineToAdd.IsEmpty())
				pTrace->Write(sLineToAdd);
			delete pTrace;
		}
#else
		UNUSED(rObject);
		Write(_T("Dumping of CObject available only in debug version (_DEBUG must be defined)!"));
#endif
	}

#ifdef _DEBUG
	rObject.Dump(afxDump);
#endif

	Write(strSep + _T(" end of ") + strMsg + _T(" dump ") + strSep);
}

/**
 * WriteFormatted
 *	Write out a trace message using printf() style formatting for the message.
 *
 * --- In      : lpstrFormat : Pointer to a printf() style formatting string
 *				 ... : additional arguments, as needed by the formatting string
 * --- Out     : 
 * --- Returns : 
 * --- Effect  : The debugging console, and possiblely the dump file, will have this
 *				 message sent to them for viewing.
 */
void COXTrace::WriteFormatted(LPCTSTR lpstrFormat, ...)
{
	CString	strMessage;
	va_list	args;

	va_start(args, lpstrFormat);

	//
	// Format the message
	//
#if _MFC_VER<=0x0421
	FormatV(strMessage, lpstrFormat, args);
#else
	strMessage.FormatV(lpstrFormat, args);
#endif

	//
	// Write out the message
	//
	if (m_bPrefixOn)
		WriteMsg(m_strHeader + _T(" : ") + strMessage);
	else
		WriteMsg(strMessage);

	va_end(args);
}

/**
 * WriteMsg
 *	This function performs the actual work of outputing the message to the debugging
 * console, as well as to the dump file, if one has been specified.
 *
 * --- In      : lpstrMessage : Pointer to the text to be written out.
 * --- Out     : 
 * --- Returns : 
 * --- Effect  : The debugging console, and possiblely the dump file, will have this
 *				 message sent to them for viewing.
 */
void COXTrace::WriteMsg(LPCTSTR lpstrMessage)
{
	if (m_bTracing)
	{
		LPTSTR	lpstrLine = new TCHAR[m_nWrapWidth + 3];
		TCHAR	tChar;
		int		nIndex = 0, nPos = 0;
		int		nLength = PtrToInt(_tcslen(lpstrMessage));

		if (lpstrLine == NULL)
		{
			TRACE(_T("COXTrace::WriteMsg : Unable to allocate %u characters for line buffer\n"), m_nWrapWidth + 3);
			return ;
		}

		while (nPos < nLength)
		{
			if (m_nIndent > 0 && m_nIndent < (m_nWrapWidth - 10))
			{
				UTBStr::stprintf(lpstrLine, m_nWrapWidth + 3, _T("%*s\r\n"), m_nIndent, _T(" "));
				nIndex = m_nIndent;
			}
			else
			{
				nIndex = 0;
			}

			while (nIndex < m_nWrapWidth && nPos < nLength)
			{
				tChar = lpstrMessage[nPos++];

				if (tChar == _T('\n'))
				{
					break;
				}
				else if (tChar == _T('\r'))
				{
					if (lpstrMessage[nPos] == _T('\n'))
						++nPos;
					break;
				}
				else
				{
					lpstrLine[nIndex++] = tChar;
				}
			}

			lpstrLine[nIndex] = _T('\r');
			lpstrLine[nIndex+1] = _T('\n');
			lpstrLine[nIndex+2] = _T('\0');
			TRACE(lpstrLine);
 
			if (m_File.m_hFile != CFile::hFileNull)
			{
				m_File.Write(lpstrLine, (nIndex + 2) * sizeof(TCHAR));
			}
		}
		delete lpstrLine;
			
		if (m_File.m_hFile != CFile::hFileNull && m_bFlushOnWrite)
		{
			//
			// only fluch at end of each message, instead of at end
			// of each file.
			//
			m_File.Flush();
		}
	}
}


////////////////////////////////////////////////////////////


#if _MFC_VER<=0x0421

void FormatV(CString& sText, LPCTSTR lpszFormat, va_list argList)
{
	ASSERT(AfxIsValidString(lpszFormat));

	va_list argListSave = argList;

	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	for (LPCTSTR lpsz = lpszFormat; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
	{
		// handle '%' character, but watch out for '%%'
		if (*lpsz != '%' || *(lpsz = _tcsinc(lpsz)) == '%')
		{
			nMaxLen += _tclen(lpsz);
			continue;
		}

		int nItemLen = 0;

		// handle '%' character with format
		int nWidth = 0;
		for (; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
		{
			// check for valid flags
			if (*lpsz == '#')
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(argList, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' ||
				*lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = _ttoi(lpsz);
			for (; *lpsz != '\0' && _istdigit(*lpsz); lpsz = _tcsinc(lpsz))
				;
		}
		ASSERT(nWidth >= 0);

		int nPrecision = 0;
		if (*lpsz == '.')
		{
			// skip past '.' separator (width.precision)
			lpsz = _tcsinc(lpsz);

			// get precision and skip it
			if (*lpsz == '*')
			{
				nPrecision = va_arg(argList, int);
				lpsz = _tcsinc(lpsz);
			}
			else
			{
				nPrecision = _ttoi(lpsz);
				for (; *lpsz != '\0' && _istdigit(*lpsz); lpsz = _tcsinc(lpsz))
					;
			}
			ASSERT(nPrecision >= 0);
		}

		// should be on type modifier or specifier
		int nModifier = 0;
		if (_tcsncmp(lpsz, _T("I64"), 3) == 0)
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
#if !defined(_X86_) && !defined(_ALPHA_)
			// __int64 is only available on X86 and ALPHA platforms
			ASSERT(FALSE);
#endif
		}
		else
		{
			switch (*lpsz)
			{
			// modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				lpsz = _tcsinc(lpsz);
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				lpsz = _tcsinc(lpsz);
				break;

			// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				lpsz = _tcsinc(lpsz);
				break;
			}
		}

		// now should be on specifier
		switch (*lpsz | nModifier)
		{
		// single characters
		case 'c':
		case 'C':
			nItemLen = 2;
			va_arg(argList, TCHAR_ARG);
			break;
		case 'c'|FORCE_ANSI:
		case 'C'|FORCE_ANSI:
			nItemLen = 2;
			va_arg(argList, CHAR_ARG);
			break;
		case 'c'|FORCE_UNICODE:
		case 'C'|FORCE_UNICODE:
			nItemLen = 2;
			va_arg(argList, WCHAR_ARG);
			break;

		// strings
		case 's':
			{
				LPCTSTR pstrNextArg = va_arg(argList, LPCTSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6;  // "(null)"
				else
				{
				   nItemLen = lstrlen(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			}
			break;

		case 'S':
			{
#ifndef _UNICODE
				LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6;  // "(null)"
				else
				{
				   nItemLen = wcslen(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
#else
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = lstrlenA(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
#endif
			}
			break;

		case 's'|FORCE_ANSI:
		case 'S'|FORCE_ANSI:
			{
				LPCSTR pstrNextArg = va_arg(argList, LPCSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = lstrlenA(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			}
			break;

		case 's'|FORCE_UNICODE:
		case 'S'|FORCE_UNICODE:
			{
				LPWSTR pstrNextArg = va_arg(argList, LPWSTR);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = wcslen(pstrNextArg);
				   nItemLen = max(1, nItemLen);
				}
			}
			break;
		}

		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			if (nPrecision != 0)
				nItemLen = min(nItemLen, nPrecision);
			nItemLen = max(nItemLen, nWidth);
		}
		else
		{
			switch (*lpsz)
			{
			// integers
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				if (nModifier & FORCE_INT64)
					va_arg(argList, __int64);
				else
					va_arg(argList, int);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			case 'e':
			case 'g':
			case 'G':
				va_arg(argList, DOUBLE_ARG);
				nItemLen = 128;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			case 'f':
				va_arg(argList, DOUBLE_ARG);
				nItemLen = 128; // width isn't truncated
				// 312 == strlen("-1+(309 zeroes).")
				// 309 zeroes == max precision of a double
				nItemLen = max(nItemLen, 312+nPrecision);
				break;

			case 'p':
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = max(nItemLen, nWidth+nPrecision);
				break;

			// no output
			case 'n':
				va_arg(argList, int*);
				break;

			default:
				ASSERT(FALSE);  // unknown formatting option
			}
		}

		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}

	LPTSTR lpszBufer=sText.GetBuffer(nMaxLen);
	_vstprintf(lpszBufer, lpszFormat, argListSave);
	sText.ReleaseBuffer();

	va_end(argListSave);
}

#endif