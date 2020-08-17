// ==========================================================================
//							Specification : 
//							AdvancedAssert
// ==========================================================================

// Header file : OXAdvancedAssert.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         

#ifndef __AdvancedAssert_h__
#define __AdvancedAssert_h__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#ifndef OX_ADVANCEDASSERT_NOTE1
#define OX_ADVANCEDASSERT_NOTE1 _T("An unexpected event has just occurred.  Please report this event by sending an email.  This will help eliminate bugs.")
#endif

#ifndef OX_ADVANCEDASSERT_NOTE2
#define OX_ADVANCEDASSERT_NOTE2 _T("Any information you can give about the state when this event occurred would be greately appreciated.")
#endif

/////////////////////////////////////////////////////////////////////////////
// Diagnostic support

#if defined(_DEBUG) || defined(BETA)
#ifdef ASSERT
#undef ASSERT

/////////////////////////////////////////////////////////////////////////////
// Desciption : This is an internal function that is used by the ASSERT macro. 
//              using some advanced macro processing, the condition that fails 
//              is converted to a string that is later used in the dialog. 
// -- In      : lpszCondition - Condition to be displayed
//              lpszFileName  - filename    of failed condition. 
//              nLine         - line number of failed condition. 
// -- Out     : nothing.
// -- Returns : Returns TRUE when user selects Retry.
// -- Effect  : Sends trace message and presents user with Abort, Retry, Ignore, Send dialog. 
// Note: file names are still ANSI strings (filenames rarely need UNICODE)
OX_API_DECL BOOL AFXAPI AdvancedAssertFailedLine(LPCTSTR lpszCondition, 
												 LPCSTR lpszFileName, int nLine) ;

#ifndef _WIN32
void AfxDebugBreak() ;
#endif

#define THIS_FILE __FILE__
#define ASSERT(f) \
	do \
		{ \
		if ( !(f) && AdvancedAssertFailedLine(_T(#f), THIS_FILE, __LINE__) ) \
			AfxDebugBreak() ; \
		} \
	while (0) \

#endif // ASSERT

#ifdef VERIFY
#undef VERIFY
#define VERIFY(f)          ASSERT(f)
#endif // VERIFY

#endif // _DEBUG || BETA

/////////////////////////////////////////////////////////////////////////////
// Desciption : Public access function that allows programmatic override of 
//              global send enable flag.  When this flag is FALSE, the Send 
//              button is disabled. 
// -- In      : New send enable state. 
// -- Out     : 
// -- Returns : Previous send enable state. 
// -- Effect  : Replace the send enable state. 
OX_API_DECL BOOL AppSetAssertSendState(BOOL bEnable=TRUE) ;

/////////////////////////////////////////////////////////////////////////////
// Desciption : Public access function that allows programmer to get global send 
//				enable flag.  When this flag is FALSE, the Send button is disabled. 
// -- In      :  
// -- Out     : 
// -- Returns : send enable state. 
// -- Effect  : 
OX_API_DECL BOOL AppGetAssertSendState() ;

/////////////////////////////////////////////////////////////////////////////
// Desciption : Public access function that allows programmatic override of 
//              global beta email address. 
// -- In      : csEmailAddress - New email address supplied when user selects Send. 
// -- Out     : 
// -- Returns : Previous beta email address. 
// -- Effect  : Replace the beta email address. 
OX_API_DECL CString AppSetAssertEmailAddress(CString csEmailAddress) ;

/////////////////////////////////////////////////////////////////////////////
// Desciption : This function is used to safely gain access to beta email address. 
// -- In      : 
// -- Out     : 
// -- Returns : Current beta email address. 
// -- Effect  : 
OX_API_DECL LPCSTR AppGetAssertEmailAddress() ;

/////////////////////////////////////////////////////////////////////////////
// Desciption : Public access function that allowas programmatic override of 
//              global note 1 text. 
// -- In      : csNote - New text used for note 1. 
// -- Out     : 
// -- Returns : Previous note 1 text. 
// -- Effect  : Replace the global text used for note 1 in the assert dialog. 
OX_API_DECL CString AppSetAssertNote1(CString csNote) ;

/////////////////////////////////////////////////////////////////////////////
// Desciption : Public access function that allows programmer to get text that 
//				displayed at the top of Assert dialog (Note1). 
// -- In      :  
// -- Out     : 
// -- Returns : Note1 text. 
// -- Effect  : 
OX_API_DECL CString AppGetAssertNote1() ;

/////////////////////////////////////////////////////////////////////////////
// Desciption : Public access function that allowas programmatic override of 
//              global note 2 text. 
// -- In      : csNote - New text used for note 2. 
// -- Out     : 
// -- Returns : Previous note 2 text. 
// -- Effect  : Replace the global text used for note 2. in the assert dialog.
OX_API_DECL CString AppSetAssertNote2(CString csNote) ;

/////////////////////////////////////////////////////////////////////////////
// Desciption : Public access function that allows programmer to get text that 
//				displayed at the top of Assert dialog under Note1 (Note2). 
// -- In      :  
// -- Out     : 
// -- Returns : Note2 text. 
// -- Effect  : 
OX_API_DECL CString AppGetAssertNote2() ;

// You will want to correct the beta address. 
#define BETA_EMAIL_ADDRESS "Support[SMTP:beta@yourcompany.com]"

#endif // __AdvancedAssert_h__
/////////////////////////////////////////////////////////////////////////////
