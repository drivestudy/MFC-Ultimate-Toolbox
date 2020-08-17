// ==========================================================================
//				Class Implementation : COXURL
// ==========================================================================

// Source file : OXURL.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
			  
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXURL.h"

#include "UTBStrOp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

#pragma warning(disable : 4927)

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// All string constants will use normal slashes (no back slashes)
// and lowercase characters
static const TCHAR szURLPrePrefix[] = _T("url:");
static const TCHAR szProtocolSuffix[] = _T("://");
static const TCHAR szFileProtocol[] = _T("file://");
static const TCHAR szPortPrefix[] = _T(":");

const LPCTSTR COXURL::m_pszURLPrePrefix = szURLPrePrefix;
// --- The (optional) prefix of a URL
const LPCTSTR COXURL::m_pszProtocolSuffix = szProtocolSuffix;
// --- The suffix of a protocol
const LPCTSTR COXURL::m_pszFileProtocol = szFileProtocol;
// --- The file protocol
const LPCTSTR COXURL::m_pszPortPrefix = szPortPrefix;
// --- The prefix of a port

const UINT COXURL::m_nUnknownPort = 0;
// Data members -------------------------------------------------------------
// protected:
	// CString m_sURL;
	// --- The full URL specification
	//	   This must be the first data member of this object, so that a dumb cast
	//	    of a URL object to a LPCTSTR will work.
	//	    (The first data member of a CString is the LPCTSTR)
	//	   This is important when working with a variable number of paramaters
	//		for a function call (e.g. printf, TRACE etc)

	// BOOL m_bMainBuilt;
	// --- Whether the main part has been built
	//	   When this is FALSE the value of m_sURL should not be used

	// CString m_sProtocol;
	// --- The protocol specification
	// UINT m_nPort;
	// --- The port specification
	// COXUNC m_UNC;
	// --- The UNC specification

	// BOOL m_bPartsBuilt;
	// --- Whether the parts have been built
	//	   When this is FALSE the values of m_sProtocol, m_nPort and m_UNC should not be used

// private:
	
// Member functions ---------------------------------------------------------
// public:

COXURL::COXURL(LPCTSTR pszURL /* = NULL */)
	{
	m_sURL = pszURL;
	m_UNC.URLPart() = TRUE;

	SetMainBuilt();
	DestroyParts();
	}

COXURL::COXURL(LPCTSTR pszProtocol, int nPort, LPCTSTR pszUNC)
	{
	m_sProtocol = pszProtocol;
	m_nPort = nPort;
	m_UNC = pszUNC;
	m_UNC.URLPart() = TRUE;

	SetPartsBuilt();
	DestroyMain();
	}

COXURL::COXURL(LPCTSTR pszProtocol, int nPort, COXUNC UNC)
	{
	m_sProtocol = pszProtocol;
	m_nPort = nPort;
	m_UNC = UNC;
	m_UNC.URLPart() = TRUE;

	SetPartsBuilt();
	DestroyMain();
	}

COXURL::COXURL(const COXURL& URL)
	{
	m_sURL =		URL.m_sURL;
	m_bMainBuilt =	URL.m_bMainBuilt;

	m_sProtocol =	URL.m_sProtocol;
	m_nPort =		URL.m_nPort;
	m_UNC =			URL.m_UNC;
	m_bPartsBuilt = URL.m_bPartsBuilt;
	}

COXURL& COXURL::operator=(const COXURL& URL)
	{
	if(this==&URL)
		return *this;
		
	m_sURL =		URL.m_sURL;
	m_bMainBuilt =	URL.m_bMainBuilt;

	m_sProtocol =	URL.m_sProtocol;
	m_nPort =		URL.m_nPort;
	m_UNC =			URL.m_UNC;
	m_bPartsBuilt = URL.m_bPartsBuilt;

	return *this;
	}

COXURL::operator const CString() const
	{
	COXURL copyThis(*this);
	return copyThis;
	}
	
const CString COXURL::Protocol() const
	{
	COXURL copyThis(*this);
	return copyThis.Protocol();
	}

const UINT COXURL::Port() const
	{
	COXURL copyThis(*this);
	return copyThis.Port();
	}

const COXUNC COXURL::UNC() const
	{
	COXURL copyThis(*this);
	copyThis.UNC().URLPart() = TRUE;

	return copyThis.UNC();
	}

COXURL& COXURL::operator=(LPCTSTR pszURL)
	{
	operator=(COXURL(pszURL));
	return *this;
	}

COXURL::operator CString&()
	{
	BuildMain();
	// ... We pass a reference so the user may change the string
	DestroyParts();

	return m_sURL;
	}
	
CString& COXURL::Protocol()
	{
	BuildParts();
	// ... We pass a reference so the user may change the string
	DestroyMain();

	return m_sProtocol;
	}

UINT& COXURL::Port()
	{
	BuildParts();
	// ... We pass a reference so the user may change the string
	DestroyMain();

	return m_nPort;
	}

COXUNC& COXURL::UNC()
	{
	BuildParts();
	// ... We pass a reference so the user may change the string
	DestroyMain();

	return m_UNC;
	}

COXURL COXURL::StandardForm(BOOL bMakeLower /* = TRUE */) const
	{
	CString sStandardURL = *this;
	COXUNC::ConvertBackslashToSlash(sStandardURL);
	if (bMakeLower)
		sStandardURL.MakeLower();
	return COXURL(sStandardURL);
	}

void COXURL::Empty()
	{
	m_sURL.Empty();
	m_bMainBuilt = FALSE;

	m_sProtocol.Empty();
	m_nPort = m_nUnknownPort;
	m_UNC.Empty();
	m_bPartsBuilt = FALSE;
	}

COXURL::~COXURL()
	{
	}

// protected:
void COXURL::BuildMain()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Builds the main URL from the parts
	{
	if (IsMainBuilt())
		{
		// ... Re-adjust main data (may have changed)
		AdjustMain();
		return;
		}

	if (!ArePartsBuilt())
		{
		// Nothing has been built yet, clean everything and set to built
		Empty();
		SetPartsBuilt();
		return;
		}

	// ... Adjust parts before we use them
	ASSERT(ArePartsBuilt());
	AdjustParts();

	// Build main data
	if (m_nPort == m_nUnknownPort)
		m_sURL = m_sProtocol + (LPCTSTR)m_UNC;
	else
		{
		CString sPort;
		sPort.Format(_T("%lu"), m_nPort);
		m_sURL = m_sProtocol + m_UNC.Server() + m_pszPortPrefix + sPort + 
			m_UNC.Share() + m_UNC.Directory() + m_UNC.File();
		}

	// ... Adjust main after we have built it
	AdjustMain();

	// ... Mark that the main data has been built now
	SetMainBuilt();
	return;
	}

void COXURL::BuildParts()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Builds the different parts from the full URL
	{
	if (ArePartsBuilt())
		{
		// ... Re-adjust parts data (may have changed)
		AdjustParts();
		return;
		}

	if (!IsMainBuilt())
		{
		// Nothing has been built yet, clean everything and set to built
		Empty();
		SetMainBuilt();
		}

	// ... Adjust main before we use it
	ASSERT(IsMainBuilt());
	AdjustMain();

	// Build the parts

	// Search for protocol and port and two parts of the UNC
	// E.g. http://www.test.com:80/page.htm
	//      must give : protocol : "http://"
	//					port : 80
	//					UNC1 : "www.test.com
	//					UNC2 : "/page.htm"

	LPCTSTR pszProtocol= NULL;
	LPCTSTR pszPort = NULL;
	LPCTSTR pszUNC1 = NULL;
	LPCTSTR pszUNC2 = NULL;

	UINT nProtocolLength = 0;
	UINT nPortLength = 0;
	UINT nUNC1Length = 0;
	UINT nUNC2Length = 0;

	// Remove possible URL preprefix ("URL:")
	if (m_sURL.Left((int)_tcslen(m_pszURLPrePrefix)) == m_pszURLPrePrefix)
		m_sURL = m_sURL.Mid((int)_tcslen(m_pszURLPrePrefix));

	// Initialize UNC (if there is no protocol or port)
	ASSERT(IsMainBuilt());
	pszUNC1 = m_sURL;
	nUNC1Length = m_sURL.GetLength();

	// Search for protocol
	// ... First make a copy with only normal slashes (no backslashes) and lowercase characters
	//     This copy will only be used to search
	CString sURLCopy = m_sURL;
	sURLCopy.MakeLower();
	COXUNC::ConvertBackslashToSlash(sURLCopy);

	// ... Now search for protocol suffix
	int nProtocolPos = sURLCopy.Find(m_pszProtocolSuffix);
	if (0 <= nProtocolPos)
		{
		// ... Store pointer to protocol
		pszProtocol = m_sURL;
		nProtocolLength = (UINT)(nProtocolPos + _tcslen(m_pszProtocolSuffix));
		// ... Move UNC1
		pszUNC1 += nProtocolLength;
		nUNC1Length -= nProtocolLength;
		// ... Check string boundaries
		ASSERT(((LPCTSTR)m_sURL <= pszUNC1) && (pszUNC1 + nUNC1Length <= (LPCTSTR)m_sURL + m_sURL.GetLength()));
		ASSERT(((LPCTSTR)m_sURL <= pszProtocol) && (pszProtocol + nProtocolLength <= (LPCTSTR)m_sURL + m_sURL.GetLength()));
		}

	// Search for port number

	// Search for the first slash after a non-slash character
	// Then search back for a colon
	// E.g. //Comp12:80/page.htm
	//		will result in 80
	LPCTSTR pszPortCopyStart = (LPCTSTR)sURLCopy + (pszUNC1 - (LPCTSTR)m_sURL);
	LPCTSTR pszPortCopySearch = pszPortCopyStart;
	LPCTSTR pszPortCopyEnd = NULL;
	// ... Skip leading slashes
	while (*pszPortCopySearch == COXUNC::m_cSlash)
		pszPortCopySearch++;
	ASSERT(*pszPortCopySearch != COXUNC::m_cSlash);

	// ... Skip non-slash characters
	while ( (*pszPortCopySearch != COXUNC::m_cNull) && (*pszPortCopySearch != COXUNC::m_cSlash))
		pszPortCopySearch++;
	pszPortCopyEnd = pszPortCopySearch;
	ASSERT( (*pszPortCopySearch == COXUNC::m_cNull) || (*pszPortCopySearch == COXUNC::m_cSlash));
	ASSERT( (*pszPortCopyEnd == COXUNC::m_cNull) || (*pszPortCopyEnd == COXUNC::m_cSlash));

	// ... Go back until we find a colon (or the leading slash again)
	pszPortCopySearch--;
	while ( (pszPortCopyStart < pszPortCopySearch) && (*pszPortCopySearch != COXUNC::m_cColon))
		pszPortCopySearch--;
	if (*pszPortCopySearch == COXUNC::m_cColon)
		{
		// ... Colon position from the beginning of the search
		int nColonPos = (int)(pszPortCopySearch - pszPortCopyStart);
		// ... Set port right after colon
		ASSERT((*pszPortCopyEnd == COXUNC::m_cNull) || (*pszPortCopyEnd == COXUNC::m_cSlash));
		pszPort = pszUNC1 + nColonPos + 1;
		nPortLength = (UINT)( pszPortCopyEnd - pszPortCopySearch - 1);
		// ... Ignore ports that contain non-numeric characters
		//     (According to the URL specification a port may only consist of digits : 0-9)
		for (UINT nPortIndex = 0; nPortIndex < nPortLength; nPortIndex++)
			{
			if (!_istdigit(pszPort[nPortIndex]))
				nPortLength  = 0;
			}
		// ... Check string boundaries
		ASSERT(((LPCTSTR)m_sURL <= pszPort) && (pszPort + nPortLength <= (LPCTSTR)m_sURL + m_sURL.GetLength()));

		// ... nPortLength may be 0 if colon is immediately followed by slash or backslash
		if (nPortLength != 0)
			{
			// Adjust UNC1 and UNC2
			pszUNC2 = pszPort + nPortLength;
			nUNC2Length = nUNC1Length - (pszUNC2 - pszUNC1);
			nUNC1Length = (UINT)(pszPort - pszUNC1 - 1);
			// ... Check string boundaries
			ASSERT(((LPCTSTR)m_sURL <= pszUNC1) && (pszUNC1 + nUNC1Length <= (LPCTSTR)m_sURL + m_sURL.GetLength()));
			ASSERT(((LPCTSTR)m_sURL <= pszUNC2) && (pszUNC2 + nUNC2Length<= (LPCTSTR)m_sURL + m_sURL.GetLength()));
			}
		}

	// The sum of all parts must result in the total UNC
	ASSERT((UINT)m_sURL.GetLength() == nProtocolLength + (nPortLength == 0 ? 0 : nPortLength + 1) + nUNC1Length + nUNC2Length);
	// ... Assign protocol
	if (pszProtocol != NULL)
		UTBStr::tcsncpy(m_sProtocol.GetBufferSetLength(nProtocolLength), nProtocolLength+1, pszProtocol, nProtocolLength);
	else
		m_sProtocol.Empty();

	// ... Assign port
	if (pszPort != NULL)
		m_nPort = _ttol(pszPort);
	else
		m_nPort = m_nUnknownPort;

	// ... Assign UNC
	CString sUNC;
	sUNC.GetBufferSetLength(nUNC1Length + nUNC2Length);
	if (pszUNC1 != NULL)
		UTBStr::tcsncpy(sUNC.GetBuffer(0), nUNC1Length+1, pszUNC1, nUNC1Length);
	if (pszUNC2 != NULL)
		UTBStr::tcsncpy(sUNC.GetBuffer(0) + nUNC1Length, nUNC2Length+1, pszUNC2, nUNC2Length);
	m_UNC = COXUNC(sUNC, TRUE);

	// ... Adjust parts after we have built them
	AdjustParts();

	// ... Mark that the parts data has been built now
	SetPartsBuilt();
	return;
	}

void COXURL::AdjustMain()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Adjust the main (full URL)
	{
	Trim(m_sURL);
	}

void COXURL::AdjustParts()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Adjust the URL parts 
	{
	Trim(m_sProtocol);

	// Adjust Protocol
	// ... Is protocol exists, make sure it ends with "://"
	if (!m_sProtocol.IsEmpty())
		{
		// First check trailng slashes
		LPCTSTR pszFirstSlash = NULL;
		LPCTSTR pszSecondSlash = NULL;
		pszFirstSlash = _tcspbrk(m_sProtocol, COXUNC::m_pszSlashes);
		if (pszFirstSlash != NULL)
			pszSecondSlash = _tcspbrk(pszFirstSlash + 1, COXUNC::m_pszSlashes);
		if (pszFirstSlash == NULL)
			{
			// ... No slash present add two
			m_sProtocol += COXUNC::m_cSlash;
			m_sProtocol += COXUNC::m_cSlash;
			}
		else if (pszSecondSlash != pszFirstSlash + 1)
			{
			// Second slash does not immediately follow the first
			TCHAR cSlash = *pszFirstSlash;
			// ... One slash present, trucate after this slash
			m_sProtocol = m_sProtocol.Left((int)(pszFirstSlash - (LPCTSTR)m_sProtocol + 1));
			// ... Add a second slash
			m_sProtocol += cSlash;
			}

		// Check for colon in fraont of the two slashes
		LPCTSTR pszColon = _tcschr(m_sProtocol, COXUNC::m_cColon);
		pszFirstSlash = _tcspbrk(m_sProtocol, COXUNC::m_pszSlashes);
		ASSERT(pszFirstSlash != NULL);
		ASSERT(2 <= m_sProtocol.GetLength());
		if (pszColon != pszFirstSlash - 1)
			{
			// ... Colon not in front of trailing slashes
			// Remove all colons
			int nColonPos = m_sProtocol.Find(COXUNC::m_cColon);
			while (0 <= nColonPos)
				{
				m_sProtocol = m_sProtocol.Left(nColonPos) + m_sProtocol.Mid(nColonPos + 1);
				nColonPos = m_sProtocol.Find(COXUNC::m_cColon);
				}

			// Add colon in front of trailing slashes
			pszFirstSlash = _tcspbrk(m_sProtocol, COXUNC::m_pszSlashes);
			m_sProtocol = m_sProtocol.Left((int)(pszFirstSlash - (LPCTSTR)m_sProtocol)) + 
				COXUNC::m_cColon + m_sProtocol.Right(2);
			}
		}

	// Adjust Port
	// ... Nothing to adjust

	// Adjust UNC
	m_UNC.AdjustParts();
	}

void COXURL::Trim(CString& sText)
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Removes invalid characters and leading and trailing spaces
	{
	// ... Remove invalid chars	<>|"
	int nInvalidCharPos = sText.FindOneOf(COXUNC::m_pszInvalidChars);
	while (0 <= nInvalidCharPos)
		{
		sText = sText.Left(nInvalidCharPos) + sText.Mid(nInvalidCharPos + 1);
		nInvalidCharPos = sText.FindOneOf(COXUNC::m_pszInvalidChars);
		}

	// ... Remove leading and trailng spaces
	sText.TrimLeft();
	sText.TrimRight();
	}

// private:

// ==========================================================================
