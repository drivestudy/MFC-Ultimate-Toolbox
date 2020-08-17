// ==========================================================================
//				Class Implementation : COXInteger
// ==========================================================================

// Source file : OXInteger.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
			  
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXInteger.h"

#include "UTBStrOp.h"
#include "UTB64Bit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static variables
static const TCHAR szDigits[] =	_T("0123456789abcdefghijklmnopqrstuvwxyz");
static const TCHAR szMinusDigits[] = _T("-0123456789abcdefghijklmnopqrstuvwxyz");

// Definition of static members
// ... Constants are defined in <Limits.h>
const LONGLONG COXInteger::m_nMinNumber = _I64_MIN;
const LONGLONG COXInteger::m_nMaxNumber = _I64_MAX;
const int COXInteger::m_nMinRadix = 2;
const int COXInteger::m_nMaxRadix = 46;
// ... Max length occurs in binary representation of m_nMinNumber
//     1 (sign) + 64 + 1 (terminating zero)
const int COXInteger::m_nMaxStringNumberLength = 66;
TCHAR COXInteger::m_cDefaultSeparator = _T(',');
LPCTSTR COXInteger::m_pszDigits = szDigits;
LPCTSTR COXInteger::m_pszMinusDigits = szMinusDigits;

struct _COXIntegerStaticInitializer
	{
	_COXIntegerStaticInitializer()
		{
		CString sThousandSeparator;
		int nThousandSeparatorMaxLength = 0;
		nThousandSeparatorMaxLength = ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, NULL, 0);
		VERIFY(::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, 
			sThousandSeparator.GetBuffer(nThousandSeparatorMaxLength), nThousandSeparatorMaxLength) != 0);
		sThousandSeparator.ReleaseBuffer();
		COXInteger::m_cDefaultSeparator = sThousandSeparator.GetAt(0);
		}
	} _integerStaticInitializer;


// Data members -------------------------------------------------------------
// protected:
	// LONGLONG m_nDecimal;
	// --- The actual (decimal) value of the number

	// static LPCTSTR m_pszDigits;
	// --- String containing all the valid digits
	// static LPCTSTR m_pszMinusDigits;
	// --- String containing all the valid digits and a minus sign

// private:
	
// Member functions ---------------------------------------------------------
// public:

CString COXInteger::GetStringNumber(int nRadix /* = 10 */, BOOL bSeparated /* = FALSE */,
	TCHAR cSeparator /* = _T('\0')*/, int nGroupLength /* = 3 */) const
	{
	if ((nRadix < m_nMinRadix) || (m_nMaxRadix < nRadix))
		{
		TRACE1("COXInteger::GetStringNumber : Invalid radix = %i\n", nRadix);
		return _T("");;
		}

	TCHAR szResult[m_nMaxStringNumberLength];
	LPTSTR pszDigit = szResult;
	*pszDigit = _T('\0');
	if (nRadix == 10)
		{
		// ... Use quick implementation for decimal numbers
			UTBStr::stprintf(szResult, m_nMaxStringNumberLength, _T("%I64i"), m_nDecimal);
		}
	else
		{
		// Use a buffer which we will fill backwards
		LONGLONG nNumber = m_nDecimal;
		pszDigit = &szResult[m_nMaxStringNumberLength - 1];
		*pszDigit = _T('\0');

		// Check for minus sign
		BOOL bNegative = (nNumber < 0);
		if (bNegative)
			nNumber = -nNumber;

		if (nNumber < 0)
			// Number is still negative, only possible when overfolw occurred
			// during nNumber = -nNumber, so nNumber must be set to 0
			*(--pszDigit) = _T('0');
		else
			{
			// Iterate and add % nRadix at the front of the string
			do
				{
				*(--pszDigit) = m_pszDigits[nNumber % nRadix];
				nNumber /= nRadix;
				} while (nNumber != 0);

			// Adjust the sign if necessary
			if (bNegative)
				*(--pszDigit) = _T('-');
			}
		}

	if (!bSeparated)
		return pszDigit;
	else
		return ThousandSeparated(pszDigit, cSeparator, nGroupLength);
	}

void COXInteger::SetStringNumber(LPCTSTR pszNumber, int nRadix /* = 10 */)
	{
	ASSERT(pszNumber != NULL);
	Empty();

	if ((nRadix < m_nMinRadix) || (m_nMaxRadix < nRadix))
		{
		TRACE1("COXInteger::SetStringNumber : Invalid radix = %i\n", nRadix);
		return;
		}

	// Skip invalid digits
	while ((*pszNumber != _T('\0')) && (_tcschr(m_pszMinusDigits, _totlower(*pszNumber)) == NULL))
		pszNumber++;

	// Check for minus sign
	BOOL bNegative = (*pszNumber == _T('-'));
	if (bNegative)
		pszNumber++;

	// Convert the number
	LPCTSTR pszDigit = NULL;
	while(*pszNumber != _T('\0'))
		{
		pszDigit = _tcschr(m_pszDigits, _totlower(*(pszNumber++)));
		if ((pszDigit != NULL) && (pszDigit - m_pszDigits < nRadix))
			{
#ifdef _DEBUG
			if ((m_nMaxNumber - (pszDigit - m_pszDigits)) / nRadix < m_nDecimal)
				TRACE0("COXInteger::SetStringNumber : Number overflow\n");
#endif // _DEBUG
			m_nDecimal = m_nDecimal * nRadix + (pszDigit - m_pszDigits);
			}
		// ... Do not trace every illegal character (may become too much)
		// else
		//	TRACE1("COXInteger::SetStringNumber : Skipping illegal character '%c'\n", *pszDigit);
		}

	// Adjust the sign if necessary
	if (bNegative)
		m_nDecimal = -m_nDecimal;
	}


CString COXInteger::ThousandSeparated(LPCTSTR pszOriginalNumberText, TCHAR cSeparator /* = _T('\0') */,
	int nGroupLength /* = 3 */)
	{
	// ... Use correct thousand separator
	if (cSeparator == _T('\0'))
		cSeparator = m_cDefaultSeparator;

	// Insert thousand seperator
	int nOriginalLength = PtrToInt(_tcslen(pszOriginalNumberText));
	LPCTSTR pszPart = pszOriginalNumberText;

	int nNewLength = nOriginalLength + (nOriginalLength - 1) / nGroupLength;
	CString sResultString;
	ASSERT(0 + (0 - 1) / nGroupLength == 0);
	// ... Use buffer of correct length, instead of concatening each character individually (speed)
	LPTSTR pszResultString = sResultString.GetBuffer(nNewLength);

	// First check for a possible leading minus sign
	if (*pszPart == _T('-'))
		{
		*(pszResultString++) = *(pszPart++);
		nOriginalLength--;
		}

	// Then convert the rest
	while (0 < nOriginalLength)
		{
		*(pszResultString++) = *(pszPart++);
		nOriginalLength--;
		if ((nOriginalLength != 0) && (nOriginalLength % nGroupLength == 0))
			*(pszResultString++) = cSeparator;
		}

	// ... Add terminating zero character
	*pszResultString = _T('\0');
	sResultString.ReleaseBuffer();

	return sResultString;
	}


// protected:
// private:

// ==========================================================================
