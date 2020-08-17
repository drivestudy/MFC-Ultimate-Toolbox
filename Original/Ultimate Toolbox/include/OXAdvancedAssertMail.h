// ==========================================================================
//						Class Specification : 
//						COXAdvancedAssertMail
// ==========================================================================

// Header file : OXAdvancedAssertMail.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         

#ifndef __AdvancedAssertMail_h__
#define __AdvancedAssertMail_h__

// If this is not a debug or a beta version, then don't use any of this code. 
#if defined(_DEBUG) || defined(BETA)   // entire file

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include <mapi.h>  // MapiMessage

/////////////////////////////////////////////////////////////////////////////
// COXAdvancedAssertMail

class OX_CLASS_DECL COXAdvancedAssertMail
#ifdef _WIN32
 : public CNoTrackObject
#endif
	{
	public:
		COXAdvancedAssertMail() ;
		virtual ~COXAdvancedAssertMail() ;
	
	public:
		BOOL    IsLoaded()        const ;
		BOOL    IsValidSendMail() const ;
		BOOL    LoadMapiLibrary() ;
		void    FreeMapiLibrary() ;
		
		ULONG   Logon   (ULONG ulUIParam, LPSTR lpszName, LPSTR lpszPassword, FLAGS flFlags, ULONG ulReserved) ;
		ULONG   Logoff  (ULONG ulUIParam, FLAGS flFlags, ULONG ulReserved) ;
		ULONG   SendMail(ULONG ulUIParam, lpMapiMessage lpMessage, FLAGS flFlags, ULONG ulReserved) ;

	protected:
		HINSTANCE        m_hInstMail     ;      // handle to MAPI32.DLL
		LHANDLE          m_lhSession     ;
		ULONG   (PASCAL* m_lpfnMAPILogon   )(ULONG   ulUIParam, LPSTR lpszName, LPSTR lpszPassword, FLAGS flFlags, ULONG ulReserved, LPLHANDLE lplhSession) ;
		ULONG   (PASCAL* m_lpfnMAPILogoff  )(LHANDLE lhSession, ULONG ulUIParam, FLAGS flFlags, ULONG ulReserved) ;
		HRESULT (PASCAL* m_lpfnMAPISendMail)(LHANDLE lhSession, ULONG ulUIParam, lpMapiMessage lpMessage, FLAGS flFlags, ULONG ulReserved) ;
	} ;

#ifdef _WIN32
EXTERN_PROCESS_LOCAL(COXAdvancedAssertMail, appMailState)
#else
extern COXAdvancedAssertMail _appMailState ;
#endif

int SendMail      (LPCSTR pszTo, LPCSTR pszSubject, LPCSTR pszBody, LPCSTR pszAttachment=NULL) ;
int SendMailToBeta(              LPCSTR pszSubject, LPCSTR pszBody, LPCSTR pszAttachment=NULL) ;

#endif // _DEBUG || BETA

#endif // __AdvancedAssertMail_h__
/////////////////////////////////////////////////////////////////////////////
