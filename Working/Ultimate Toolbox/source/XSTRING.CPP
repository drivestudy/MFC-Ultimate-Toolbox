// ==========================================================================
// 						Class Implementation : COXString
// ==========================================================================

// Source file : stringx.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xstring.h"
#include <ctype.h>

#include "UTBStrOp.h"
#include "UTB64Bit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

#define DECIMAL_CHARACTER_STRING_LENGTH   2  // Including 0-termination
/////////////////////////////////////////////////////////////////////////////
// Definition of static members
TCHAR pszDecimalDestination[2];
LPTSTR pszDecimalDefault = _T(",");
// ... The sequential-evaluation operator (comma operator) is used here
TCHAR COXString::cDecimalCharacter(
								   (::GetProfileString(_T("Intl"), _T("sDecimal"), pszDecimalDefault, pszDecimalDestination, DECIMAL_CHARACTER_STRING_LENGTH),
								   *pszDecimalDestination));


// Data members -------------------------------------------------------------
// protected:
// private:

// Member functions ---------------------------------------------------------
// public:

COXString::COXString()
{
}

COXString::COXString(LPCTSTR psz)
{
	*((CString*)this) = psz;
}

COXString::COXString(const CString& sSource)
{
	*((CString*)this) = sSource;
}

COXString::COXString(LONG nSource)
{
	TCHAR pszBuffer[20];
	UTBStr::ltot(nSource, pszBuffer, 20, 10);
	*this = COXString(&pszBuffer[0]);
}

COXString::COXString(const COXString& sString)
: CString(sString)
{

}

COXString& COXString::operator=(const COXString& sString)
{
	CString::operator=(sString);

	return *this;
}

void COXString::LTrim()
{    
	int nStringIndex = 0;
	int nLength = GetLength();

	// find first non-space character
	LPCTSTR lpsz = *this;
	while (_istspace(*lpsz))
#ifdef WIN32
		lpsz = _tcsinc(lpsz);
#else
		lpsz++;
#endif

	// fix up data and length
#if _MFC_VER < 0x0700
	nStringIndex = lpsz - m_pchData;
#else
	nStringIndex = PtrToInt(lpsz - GetBuffer());
#endif
	if (nStringIndex == nLength)
		*this = COXString(_T(""));
	else	
		*this = Mid(nStringIndex);	
}

void COXString::RTrim()
{    
	int nStringIndex = GetLength() - 1;
	while((0 <= nStringIndex) && _istspace(GetAt(nStringIndex)) )
		nStringIndex--;
	*this =  Left(nStringIndex + 1);

}

void COXString::XTrim()
{                                                            
	// Optimisation :
	// The output string sOut is initialized by the value of the string
	// 	The real significant characters in sOut are from position 0 till nPosOut - 1
	// 	Characters are thus added by overwriting a character in sOut and incrementing nPosOut by 1.
	// 	Now memory is allocated only once (at initialisation of sOut)
	// When you would start with an empty string and would concatenate characters
	// 	then extra memory must be allocated frequently (overhead).

	COXString sOut(*this);
	int nPosIn = 0;
	int nPosOut = 0;
	BOOL bSpace = FALSE;

	while (nPosIn < GetLength())
	{
		if (_istspace(GetAt(nPosIn)))
			if (bSpace)
			{
				// ... Second white space character encountered
				//     Skip this character (do not copy)
				nPosIn++;
			}
			else 
			{
				// ... First white space character encountered
				//     Copy space (all white space characters are replaced by a space)
				sOut.SetAt(nPosOut++, _T(' '));
				nPosIn++;
				bSpace = TRUE;
			}              
		else
		{    
			// ... Copying a non white space character
			sOut.SetAt(nPosOut++, GetAt(nPosIn++));
			bSpace = FALSE;
		}
	}
	*this = sOut.Left(nPosOut);
}

int COXString::GetInt() const 
{
	return _ttoi(*this);
}

long COXString::GetLongInt() const 
{
	return _ttol(*this);
}

BOOL COXString::IsInt()
{
	for (int nIndex = 0; nIndex < GetLength(); nIndex++)
		if (!_istdigit(GetAt(nIndex)))
			return FALSE;
	return TRUE;		
}

BOOL COXString::IsNumber()
{
	LPCTSTR pc = *this;
	BOOL bDigit = FALSE;

	// Skip white space characters
	while (_istspace(*pc))
		pc++;
	// Skip sign
	if ((*pc == _T('-')) || (*pc == _T('+')))
		pc++;
	// Skip digits
	while (_istdigit(*pc))
	{
		pc++;
		bDigit = TRUE;
	}
	// Skip decimal sign
	if (*pc == cDecimalCharacter)
		pc++;
	if (!bDigit && !_istdigit(*pc))
		return FALSE;
	// Skip digits
	while (_istdigit(*pc))
		pc++;
	// Skip exponent sign and rest
	if ((*pc == _T('E')) || (*pc == _T('e')) || (*pc == _T('D')) || (*pc == _T('d')))
	{
		pc++;
		// Skip sign
		if ((*pc == _T('-')) || (*pc == _T('+')))
			pc++;
		// Skip digits
		if (!_istdigit(*pc))
			// Exponent sign is not followed by digits
			return FALSE;
		while (_istdigit(*pc))
			pc++;
	}
	// Skip white space characters
	while (_istspace(*pc))
		pc++;

	// Pointer should not be behind the end of the string
	ASSERT(pc <= (((LPCTSTR)*this) + GetLength()));

	// Must be at the end of the string by now, otherwise the number is not valid
	return (*pc == _T('\0'));
}

void COXString::Format(LPCTSTR pszFormat, LPCTSTR* rgpsz, int nString)
{
	// NOTE: will not work for strings > 255 characters

	int nTotalLen = PtrToInt(_tcslen(pszFormat));
	int i = 0;
	for (i = 0; i < nString; i++)
	{
		if (rgpsz[i] != NULL)
			nTotalLen += PtrToInt(_tcslen(rgpsz[i]));
	}

	LPCTSTR pchSrc = pszFormat;
	LPTSTR pchDestBegin  =  GetBuffer(nTotalLen+1);
	LPTSTR pchDest = pchDestBegin;
	while (*pchSrc != '\0')
	{
		if (pchSrc[0] == _T('%') && (pchSrc[1] >= _T('1') && pchSrc[1] <= _T('9')))
		{
			i = pchSrc[1] - _T('1');
			pchSrc += 2;
			if (i >= nString)
			{
				TRACE1("COXString::Format : Illegal string index requested %d\n", i);
				*pchDest++ = _T('?');
			}   	
			else if (rgpsz[i] != NULL)
			{
				UTBStr::tcscpy(pchDest, nTotalLen, rgpsz[i]);
				pchDest += _tcslen(pchDest);
			}
		}
		else
		{
			*pchDest++ = *pchSrc++;
		}
	}


	ReleaseBuffer((int)((LPCTSTR)pchDest - (LPCTSTR)pchDestBegin));
	// Release will assert if we went too far
}

void COXString::BarToNull()
{
	int nLength = GetLength();
	LPTSTR pszData = GetBuffer(nLength);
	while ((pszData = _tcschr(pszData, _T('|'))) != NULL)
		*pszData++ = _T('\0');
	ReleaseBuffer(nLength);
	ASSERT(GetLength() == nLength);
}

COXString::~COXString()
{
}

// protected:

// private:

// ==========================================================================

