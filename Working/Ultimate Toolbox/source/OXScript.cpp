// ==========================================================================
// 					Class Implementation : COXScript
// ==========================================================================

// Source file : oxscript.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "oxscript.h"

#include "UTBStrOp.h"
#include "UTB64Bit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

COXScript::COXScript()
	{

	}

COXScript::~COXScript()
	{

	}


int COXScript::DoMain(int argc, char** argv)
{
    /* What method were we invoked through? */
	LPTSTR pRequestMethod = UTBStr::tgetenv(_T("REQUEST_METHOD"));
    if (pRequestMethod == NULL)
    {
		OnRunCommandLine(argc, argv);
		return 0;
    }

    if (_tcsicmp(pRequestMethod, _T("POST")) == 0)
    {
        /* Read in the data from the client */
		LPTSTR p = UTBStr::tgetenv(_T("CONTENT_LENGTH"));
        int cbContent = (p != NULL) ? _ttoi(p) : 0;
        if (cbContent > sizeof(InputBuffer) -1)
        {
            // BOGUS: Truncate it!
            cbContent = sizeof(InputBuffer)-1;
        }

   	    /* Turn buffering off for stdin */
	    setvbuf(stdin,NULL,_IONBF,0);

        int i = 0;
        while (i < cbContent)
	    {
			if (feof(stdin))
                break;
            int x = _fgettc(stdin);
            if (x == EOF)
                break;
        
			InputBuffer[i++] = (TCHAR)x;
        }

        InputBuffer[i] = _T('\0');
        cbContent = i;

		p = UTBStr::tgetenv(_T("CONTENT_TYPE"));
        if (p == NULL)
            return 0;

        OnPost(p, InputBuffer);
    }
    else if (_tcsicmp(pRequestMethod, _T("GET")) == 0)
	{
        // QUERY_STRING should be the same as argv[1]
		LPTSTR p = UTBStr::tgetenv(_T("QUERY_STRING"));
        OnGet(p);
  	}
	
    return 0;
}

BOOL COXScript::Extract(CString& strRet, LPCTSTR pszData, int iArg, LPCTSTR pszLabel)
	{
    CString str;

    if (!ExtractSubString(str, pszData, iArg, _T('&')))
        return FALSE;

    int cchLabel = PtrToInt(_tcslen(pszLabel));
    if (str.GetLength() < cchLabel+1 ||	str[cchLabel] != _T('=') ||
        _tcsncmp(str, pszLabel, cchLabel) != 0)
        return FALSE;
    
	strRet = str.Mid(cchLabel+1);
	return TRUE;
	}

BOOL COXScript::ExtractSubString(CString& rString, LPCTSTR lpszFullString, int iSubString, TCHAR chSep)
	{
	if (lpszFullString == NULL)
		return FALSE;

	while (iSubString--)
		{
		lpszFullString = _tcschr(lpszFullString, chSep);
		if (lpszFullString == NULL)
			{
			rString.Empty();        // return empty string as well
			return FALSE;
			}
		lpszFullString++;       // point past the separator
		}

	LPCTSTR lpchEnd = _tcschr(lpszFullString, chSep);
	int nLen = (lpchEnd == NULL) ?
		lstrlen(lpszFullString) : (int)(lpchEnd - lpszFullString);
	ASSERT(nLen >= 0);
	memcpy(rString.GetBufferSetLength(nLen), lpszFullString, nLen*sizeof(TCHAR));
	return TRUE;
	}


BOOL COXScript::FindAndReplace(LPCTSTR pszcFind, LPCTSTR pszcReplace, LPCTSTR pszcSource, LPTSTR& pszDestination)
	{
	INT_PTR FindLength = _tcslen(pszcFind);
	INT_PTR ReplaceLength = _tcslen(pszcReplace);
	INT_PTR SourceLength = _tcslen(pszcSource);

	// Count the occurrence of the SubString to be found
	LPTSTR pszResult = pszDestination;
	LPCTSTR pszBeginToken = pszcSource;
	LPCTSTR pszEndToken = pszcSource;

	int Count = 0;
	pszEndToken = _tcsstr(pszEndToken, pszcFind);
	while(pszEndToken != NULL)
		{
		Count++;
		pszEndToken += FindLength;
		pszEndToken = _tcsstr(pszEndToken, pszcFind);
		}
	
	INT_PTR NeededLength = SourceLength - (Count * FindLength) + (Count * ReplaceLength);
	if (pszResult == NULL)
		{
		pszResult = new TCHAR[NeededLength + 1];
		pszDestination = pszResult;
		}
	else
		{
		if ((int)_tcslen(pszResult) < NeededLength)
			return FALSE;
		}
	
	// find first matching substring
	pszBeginToken = pszcSource;
	pszEndToken = pszcSource;
	pszEndToken = _tcsstr(pszEndToken, pszcFind);
	while(pszEndToken != NULL)
		{
		Count = (int)(pszEndToken - pszBeginToken);
		UTBStr::tcsncpy(pszResult, Count, pszBeginToken, Count);
		
		// Move the result pointer on its terminating zero
		pszResult += Count;
		UTBStr::tcscpy(pszResult, _tcslen(pszcReplace), pszcReplace);
		pszResult += ReplaceLength;

		// Move the token pointers one token further
		pszEndToken += FindLength;
		pszBeginToken = pszEndToken;
		pszEndToken = _tcsstr(pszEndToken, pszcFind);
		}

	UTBStr::tcscpy(pszResult, _tcslen(pszBeginToken), pszBeginToken);
	
	return TRUE;
	}

// The following function does not do UUDecoding.  It has been named improperly.  However
// To save legacy code that uses this function we will not rename it but simply provide this
// information to users who may wish to change it.
//
// What this function does is simply parsing out the query sent by the browser to the CGI
// script
//
// ex: Jeff%20Scott%20Webmaster
   //  would be converted to 
   //  Jeff Scott Webmaster  
   //  through this function
BOOL COXScript::UUDecode(LPCTSTR pszcSource, LPTSTR& pszDestination)
	{
	INT_PTR SourceLength = _tcslen(pszcSource);

	// Count the occurrence of the SubString to be found
	LPTSTR pszResult = pszDestination;
	LPCTSTR pszBeginToken = pszcSource;

	if (pszResult == NULL)
		{
		pszResult = new TCHAR[SourceLength + 1];
		pszDestination = pszResult;
		}
	else
		{
		if ((int)_tcslen(pszResult) < SourceLength)
			return FALSE;
		}
	
	LPTSTR pszEncodedChar = new TCHAR[3];
	int iHex = 0;
	while(*pszBeginToken != _T('\0'))
		{
		if (*pszBeginToken == _T('%'))
			{
			pszBeginToken++;
			UTBStr::tcsncpy(pszEncodedChar, 2, pszBeginToken, 2);

#if _MSC_VER >= 1400
			_stscanf_s(pszEncodedChar, _T("%x"), &iHex);
#else
			_stscanf(pszEncodedChar, _T("%x"), &iHex);
#endif
			*pszResult++ = (TCHAR)iHex;
			pszBeginToken += 2;
			}
		else if (*pszBeginToken == _T('+'))
			{
			*pszResult++ = _T(' ');
			pszBeginToken++;
			}
		else
			*pszResult++ = *pszBeginToken++;
		}

	*pszResult = _T('\0');

	delete[] pszEncodedChar;
	return TRUE;
	}


LPCTSTR COXScript::GetPathInfo()
{
	return UTBStr::tgetenv(_T("PATH_INFO"));
}

LPCTSTR COXScript::GetScriptName()
    {
    return UTBStr::tgetenv(_T("SCRIPT_NAME"));
    }

LPCTSTR COXScript::GetRemoteAddress()
    {
    return UTBStr::tgetenv(_T("REMOTE_ADDR"));
    }

LPCTSTR COXScript::GetServerName()
    {
    return UTBStr::tgetenv(_T("SERVER_NAME"));
    }

LPCTSTR COXScript::GetServerProtocol()
    {
    return UTBStr::tgetenv(_T("SERVER_PROTOCOL"));
    }

LPCTSTR COXScript::GetServerSoftware()
    {
    return UTBStr::tgetenv(_T("SERVER_SOFTWARE"));
    }

int COXScript::GetServerPort()
	{
    LPCTSTR p = UTBStr::tgetenv(_T("SERVER_PORT"));
    if (p == NULL)
        return -1;      // unknown
    else
        return _ttoi(p);
	}

LPCTSTR COXScript::GetClientInfo(LPCTSTR pszPart)
	{
    TCHAR szT[256];
    
	UTBStr::tcscpy(szT, 256, _T("HTTP_"));
	UTBStr::tcscat(szT, 256, pszPart);

    return UTBStr::tgetenv(szT);
	}
