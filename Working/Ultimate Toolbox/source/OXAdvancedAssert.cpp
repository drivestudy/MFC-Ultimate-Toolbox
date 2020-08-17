// ==========================================================================
// 						Class Implementation : COXAdvancedAssert
// ==========================================================================

// Source file OXAdvancedAssert.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.  

#include "stdafx.h"

#ifndef __AdvancedAssert_h__
#error In order to use Advanced Asserts "OXAdvancedAssert.h" must be included in stdafx.h
#endif

#include "OXAdvancedAssertMail.h"  // SendMail()

#include "UTBStrOp.h"
#include "UTB64Bit.h"


#ifdef AFX_DBG1_SEG
#pragma code_seg(AFX_DBG1_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

#ifndef szNULL
#define szNULL _T("")
#endif // szNULL

#ifndef szCRLF
#define szCRLF _T("\r\n")
#endif // szCRLF

#ifdef _UNICODE
#define _tcstombsz     wcstombsz
#else
#define _tcstombsz     strncpy
#endif

#include "UTBStrOp.h"

/////////////////////////////////////////////////////////////////////////////
// Advanced Assert Send state

static BOOL    g_bEnableAssertSend  = TRUE ;
static char    g_szEmailAddress[128] = BETA_EMAIL_ADDRESS ;  // Must not be a Unicode string. 

static CString g_csAssertNote1= OX_ADVANCEDASSERT_NOTE1;
static CString g_csAssertNote2= OX_ADVANCEDASSERT_NOTE2;



BOOL AppGetAssertSendState()
{
	return g_bEnableAssertSend ;
}

BOOL AppSetAssertSendState(BOOL bEnable/*=TRUE*/)
{
	BOOL bOldState = g_bEnableAssertSend ;
	g_bEnableAssertSend = bEnable ;
	return bOldState ;
}

LPCSTR AppGetAssertEmailAddress()
{
	return g_szEmailAddress ;
}

CString AppSetAssertEmailAddress(CString csEmailAddress)
{
	CString csOldEmailAddress ( g_szEmailAddress ) ;
	UTBStr::tcsncpy((TCHAR*)g_szEmailAddress, 128, csEmailAddress, __min(csEmailAddress.GetLength()+1, sizeof(g_szEmailAddress)) ) ;
	return csOldEmailAddress ;
}

CString AppGetAssertNote1()
{
	return g_csAssertNote1 ;
}

CString AppSetAssertNote1(CString csNote)
{
	CString csOldNote ( g_csAssertNote1 ) ;
	g_csAssertNote1 = csNote ;
	return csOldNote ;
}

CString AppGetAssertNote2()
{
	return g_csAssertNote2 ;
}

CString AppSetAssertNote2(CString csNote)
{
	CString csOldNote ( g_csAssertNote2 ) ;
	g_csAssertNote2 = csNote ;
	return csOldNote ;
}

// If this is not a debug or a beta version, then don't use any of this code. 
#if defined(_DEBUG) || defined(BETA)   // entire file

/////////////////////////////////////////////////////////////////////////////
// Advanced Assert dialog

#define IDD_ADVANCED_ASSERTS 100
#define IDC_MESSAGE          101
#define IDC_SEND             102
#define IDC_NOTE1            103
#define IDC_NOTE2            104



#ifndef _WIN32
typedef char wchar_t ;
#endif

#pragma pack(2)
typedef struct _tagSDialogFontTemplate
{
	WORD        wFontSize      ;
	wchar_t     szFontName[14] ;
} SDialogFontTemplate;

typedef struct _tagSDialogTemplate
{
	DLGTEMPLATE header  ;
	wchar_t     szMenu  ;  // Align on WORD boundry. 
	wchar_t     szClass ;
	wchar_t     szTitle ;

	SDialogFontTemplate font ;
	WORD                wUnused ; // Help it to align.  Interesting that it does not align automatically. 
} SDialogTemplate;

typedef struct _tagSDialogItemTemplate
{
	DLGITEMTEMPLATE item      ;
	wchar_t         szOrdinal ;  // Align on WORD boundry. 
	wchar_t         szClass   ;
	wchar_t         szTitle   ;
	// Creation data falls on a DWORD boundry. 
} SDialogItemTemplate;
#pragma pack()

// Align on 4-byte boundry. 
#pragma pack(4)
typedef struct _tagSAdvancedAssertDialogTemplate
{
	SDialogTemplate     header                ;
	SDialogItemTemplate itemSend              ;
	DWORD               dwCreationDataSend    ;  // Align on DWORD boundry. 
	SDialogItemTemplate itemNote1             ;
	DWORD               dwCreationDataNote1   ;
	SDialogItemTemplate itemNote2             ;
	DWORD               dwCreationDataNote2   ;
	SDialogItemTemplate itemMessage           ;
	DWORD               dwCreationDataMessage ;
	SDialogItemTemplate itemAbort             ;
	DWORD               dwCreationDataAbort   ;
	SDialogItemTemplate itemRetry             ;
	DWORD               dwCreationDataRetry   ;
	SDialogItemTemplate itemIgnore            ;
	DWORD               dwCreationDataIgnore  ;
} SAdvancedAssertDialogTemplate;
#pragma pack()

const DWORD DS_ADVANCED_DIALOG = WS_POPUP | WS_CAPTION | WS_SYSMENU | DS_MODALFRAME | DS_SETFONT ;
const DWORD WS_ADVANCED_BUTTON = WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON ;
const DWORD WS_ADVANCED_TEXT   = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_CENTER | ES_MULTILINE | ES_READONLY | WS_VSCROLL ;
const DWORD WS_ADVANCED_STATIC = WS_CHILD | WS_VISIBLE | WS_GROUP   | SS_LEFT ;

const WORD CL_BUTTON    = 0x0080 ;
const WORD CL_EDIT      = 0x0081 ;
const WORD CL_STATIC    = 0x0082 ;
const WORD CL_LISTBOX   = 0x0083 ;
const WORD CL_SCROLLBAR = 0x0084 ;
const WORD CL_COMBOBOX  = 0x0085 ;

static SAdvancedAssertDialogTemplate dlgAdvancedAssertTemplate =
{
#ifdef _WIN32
	{ { DS_ADVANCED_DIALOG, 0,   7,   0,   0, 292, 126,             },      0,         0, 0, { 8, L"MS Sans Serif" } }, // header
#else
	{ { DS_ADVANCED_DIALOG, 0,   7,   0,   0, 292, 126,             },      0,         0, 0, { 8,  "MS Sans Serif" } }, // header. Not UNICODE because not WIN32
#endif
	{ { WS_ADVANCED_BUTTON, 0,      220, 105,  50,  14, IDC_SEND    }, 0xFFFF, CL_BUTTON, 0 }, 0,  // itemSend  
	{ { WS_ADVANCED_STATIC, 0,       24,   7, 243,  21, IDC_NOTE1   }, 0xFFFF, CL_STATIC, 0 }, 0,  // itemNote1  
	{ { WS_ADVANCED_STATIC, 0,       24,  28, 243,  21, IDC_NOTE2   }, 0xFFFF, CL_STATIC, 0 }, 0,  // itemNote2  
	{ { WS_ADVANCED_TEXT  , 0,       22,  52, 247,  48, IDC_MESSAGE }, 0xFFFF, CL_EDIT  , 0 }, 0,  // itemMessage
	{ { WS_ADVANCED_BUTTON, 0,       22, 105,  50,  14, IDABORT     }, 0xFFFF, CL_BUTTON, 0 }, 0,  // itemAbort 
	{ { WS_ADVANCED_BUTTON, 0,       88, 105,  50,  14, IDRETRY     }, 0xFFFF, CL_BUTTON, 0 }, 0,  // itemRetry 
	{ { WS_ADVANCED_BUTTON, 0,      154, 105,  50,  14, IDIGNORE    }, 0xFFFF, CL_BUTTON, 0 }, 0,  // itemIgnore
} ;



typedef struct _tagSAdvancedAssertData
{
	LPCTSTR pszMessage  ;
	BOOL    bEnableSend ;
} SAdvancedAssertData;

BOOL FAR PASCAL AdvancedAssertDlgProc(HWND hDlg, UINT nMessage, 
									  WPARAM wParam, LPARAM lParam)
{
	switch ( nMessage )
	{
	case WM_INITDIALOG:
		{
			// Set the text here so that the dialog structure can be kept simple. 
			// Obviously, No attempt has been make to internationalize this code. 

			::SetWindowText ( hDlg, _T("Assertion Failed!") ) ;
			::SetDlgItemText( hDlg, IDABORT  , _T("&Abort"    ) ) ;
			::SetDlgItemText( hDlg, IDRETRY  , _T("&Retry"    ) ) ;
			::SetDlgItemText( hDlg, IDIGNORE , _T("&Ignore"   ) ) ;
			::SetDlgItemText( hDlg, IDC_SEND , _T("&Send Mail") ) ;
			::SetDlgItemText( hDlg, IDC_NOTE1, g_csAssertNote1  ) ;
			::SetDlgItemText( hDlg, IDC_NOTE2, g_csAssertNote2  ) ;

			SAdvancedAssertData* pData = (SAdvancedAssertData*) lParam ;
			ASSERT( pData);  // Careful, we are already inside an assert. 

			LPCTSTR pszMessage = pData->pszMessage ;
			::SetDlgItemText ( hDlg, IDC_MESSAGE, pszMessage ) ;
#ifndef _DEBUG
			HWND hwndRetry = ::GetDlgItem ( hDlg, IDRETRY ) ;
			::EnableWindow ( hwndRetry, FALSE ) ;
#endif
			HWND hwndSendEmail = ::GetDlgItem ( hDlg, IDC_SEND ) ;
			::EnableWindow ( hwndSendEmail, pData->bEnableSend ) ;

			// Setting the selection does not work. 
			// Try setting focus to the send email button first. 
			::SendDlgItemMessage ( hDlg, IDC_MESSAGE, EM_SETSEL, 0, 0 ) ;
			return TRUE ;
		}
	case WM_COMMAND:
		{
			switch ( wParam )
			{
			case IDCANCEL:  // Escape
				wParam = IDIGNORE ;
			case IDABORT:
			case IDRETRY:
			case IDIGNORE:
			case IDC_SEND:
				::EndDialog ( hDlg, wParam ) ;
				return TRUE ;
			}
			break ;
		}
	}
	return FALSE ;
}


/////////////////////////////////////////////////////////////////////////////
// Advanced Assert function

#define MAX_MSG 4096
#ifdef _WIN32
LONG lAdvancedAssertBusy       = -1 ;
LONG lAdvancedAssertReallyBusy = -1 ;
BOOL (AFXAPI* afxAssertFailedLine)(LPCSTR, int) ;

#ifndef _DEBUG
AFX_DATA BOOL afxTraceEnabled=FALSE ;
#else
#ifdef _AFXDLL
extern __declspec(dllimport) AFX_DATA BOOL afxTraceEnabled ;
#else
extern AFX_DATA BOOL afxTraceEnabled ;
#endif // _AFXDLL
#endif // _DEBUG

#else
int  NEAR nAdvancedIgnoreAssertCount =  0 ;
LONG NEAR lAdvancedAssertBusy        = -1 ;
HWND PASCAL _AfxGetSafeOwner(CWnd* pParent) ;
void AfxDebugBreak()
{
	_asm { int 3 } ;
}

#endif


// Note: file names are still ANSI strings (filenames rarely need UNICODE)
BOOL AFXAPI AdvancedAssertFailedLine(LPCTSTR lpszCondition, LPCSTR lpszFileName, int nLine)
{
#ifdef _WIN32
#ifdef _DEBUG
	BOOL bUseCrtDebug=FALSE ;
	if ( bUseCrtDebug )
	{
		// This little bit of code was taken directly from the MFC sources. 
		// It is placed here just as a reference. 
		// we remove WM_QUIT because if it is in the queue then the message box won't display
		MSG msg ;
		BOOL bQuit = PeekMessage ( &msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE ) ;
		char szCondition[MAX_MSG]="" ;
		int nConditionLength = (int) _tcslen ( lpszCondition ) ;
		UTBStr::tcsncpy((TCHAR*) szCondition,MAX_MSG, lpszCondition, __min(nConditionLength+1, sizeof(szCondition)) ) ;

		BOOL bResult = _CrtDbgReport ( _CRT_ASSERT, lpszFileName, nLine, NULL, szCondition ) ;
		if ( bQuit )
			PostQuitMessage ( (int)msg.wParam ) ;
		return bResult ;
	}
#endif	
	// handle the (hopefully rare) case of AfxGetAllocState ASSERT
	if ( ::InterlockedIncrement(&lAdvancedAssertReallyBusy) > 0 )
	{
		TCHAR szMessage[MAX_MSG ]=szNULL ;
		TCHAR szExeName[_MAX_PATH]=szNULL ;
		if ( !GetModuleFileName(NULL, szExeName, _MAX_PATH) )
			UTBStr::tcscpy ( szExeName, _MAX_PATH, _T("<program name unknown>") ) ;

		// assume the debugger or auxiliary port
		// Keep the trace down to one line. 
		wsprintf ( szMessage, _T("ASSERT( %s );  Assertion Failed: %s: File %hs, Line %d\n"), lpszCondition, szExeName, lpszFileName, nLine ) ;
		::OutputDebugString ( szMessage ) ;
		::InterlockedDecrement ( &lAdvancedAssertReallyBusy ) ;

		// assert w/in assert (examine call stack to determine first one)
		AfxDebugBreak() ;
		return FALSE ;
	}

	// check for special hook function (for testing diagnostics)
	::InterlockedDecrement ( &lAdvancedAssertReallyBusy ) ;
	if ( afxAssertFailedLine != NULL )
		return (*afxAssertFailedLine)(lpszFileName, nLine) ;

	TCHAR szMessage[MAX_MSG ]=szNULL ;  // Separate message because of potential interlock problem. 
	TCHAR szExeName[_MAX_PATH]=szNULL ;
	if ( !GetModuleFileName(NULL, szExeName, _MAX_PATH) )
		UTBStr::tcscpy ( szExeName, _MAX_PATH, _T("<program name unknown>") ) ;

	if ( afxTraceEnabled )
	{
		// assume the debugger or auxiliary port
		// output into MacsBug looks better if it's done in one string,
		// since MacsBug always breaks the line after each output
		// Keep the trace down to one line. 
		wsprintf ( szMessage, _T("ASSERT( %s );  Assertion Failed: %s: File %hs, Line %d\n"), lpszCondition, szExeName, lpszFileName, nLine ) ;
		::OutputDebugString ( szMessage ) ;
	}

	if ( ::InterlockedIncrement ( &lAdvancedAssertBusy) > 0 )
	{
		::InterlockedDecrement ( &lAdvancedAssertBusy ) ;

		// assert within assert (examine call stack to determine first one)
		AfxDebugBreak() ;
		return FALSE ;
	}

	// active popup window for the current thread
	HWND hwndParent = ::GetActiveWindow() ;
	if ( hwndParent != NULL )
		hwndParent = ::GetLastActivePopup ( hwndParent ) ;

	// Separate the assertion from the file and line info when displaying in a message box. 
	wsprintf ( szMessage, _T("Condition: ASSERT( %s ); ") szCRLF _T("Program:%s ") szCRLF _T("File: %hs ") szCRLF _T("Line: %d"), lpszCondition, szExeName, lpszFileName, nLine ) ;

	// we remove WM_QUIT because if it is in the queue then the message box
	// won't display
	MSG msg ;
	BOOL bQuit = ::PeekMessage ( &msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE ) ;
	// display the assert

	SAdvancedAssertData dataAssert ;
	dataAssert.pszMessage  = szMessage           ;
	dataAssert.bEnableSend = g_bEnableAssertSend ;


	// The dialog template must be aligned on a 4-byte boundry. 
	LPVOID lpvSend    = &dlgAdvancedAssertTemplate.itemSend    ;
	LPVOID lpvNote1   = &dlgAdvancedAssertTemplate.itemNote1   ;
	LPVOID lpvNote2   = &dlgAdvancedAssertTemplate.itemNote2   ;
	LPVOID lpvMessage = &dlgAdvancedAssertTemplate.itemMessage ;
	LPVOID lpvAbort   = &dlgAdvancedAssertTemplate.itemAbort   ;
	LPVOID lpvRetry   = &dlgAdvancedAssertTemplate.itemRetry   ;
	LPVOID lpvIgnore  = &dlgAdvancedAssertTemplate.itemIgnore  ;
	// Can't assert here, so just trace the equivalent. 
	if ( ((DWORD_PTR)lpvSend    & 0x0003) != 0 ) TRACE( _T("ASSERT( lpvSend    & 0x0003) == 0);\n") );
	if ( ((DWORD_PTR)lpvNote1   & 0x0003) != 0 ) TRACE( _T("ASSERT( lpvNote1   & 0x0003) == 0);\n") );
	if ( ((DWORD_PTR)lpvNote2   & 0x0003) != 0 ) TRACE( _T("ASSERT( lpvNote2   & 0x0003) == 0);\n") );
	if ( ((DWORD_PTR)lpvMessage & 0x0003) != 0 ) TRACE( _T("ASSERT( lpvMessage & 0x0003) == 0);\n") );
	if ( ((DWORD_PTR)lpvAbort   & 0x0003) != 0 ) TRACE( _T("ASSERT( lpvAbort   & 0x0003) == 0);\n") );
	if ( ((DWORD_PTR)lpvRetry   & 0x0003) != 0 ) TRACE( _T("ASSERT( lpvRetry   & 0x0003) == 0);\n") );
	if ( ((DWORD_PTR)lpvIgnore  & 0x0003) != 0 ) TRACE( _T("ASSERT( lpvIgnore  & 0x0003) == 0);\n") );

	HINSTANCE hInstance = AfxGetResourceHandle() ;
	int nCode = (int)::DialogBoxIndirectParam ( hInstance, (LPCDLGTEMPLATE)&dlgAdvancedAssertTemplate, hwndParent, AdvancedAssertDlgProc, (LPARAM)&dataAssert ) ;
	// If the custom version fails, then use the original version. 
	if ( nCode == -1 )
		nCode = ::MessageBox ( hwndParent, szMessage, _T("Assertion Failed!"), MB_TASKMODAL | MB_ICONHAND | MB_ABORTRETRYIGNORE | MB_SETFOREGROUND ) ;
	if ( bQuit )
		::PostQuitMessage ((int) msg.wParam ) ;

	// cleanup
	::InterlockedDecrement ( &lAdvancedAssertBusy ) ;

#else
	if ( nAdvancedIgnoreAssertCount > 0 )
	{
		nAdvancedIgnoreAssertCount-- ;
		return FALSE ;
	}

	// In case _AfxGetAppDataFails.
	if ( ++lAdvancedAssertBusy > 0 )
	{
		TCHAR szMessage[MAX_MSG ]=szNULL ;
		TCHAR szExeName[_MAX_PATH]=szNULL ;
		if ( !GetModuleFileName(NULL, szExeName, _MAX_PATH) )
			_tcscpy ( szExeName, _T("<program name unknown>") ) ;

		// assume the debugger or auxiliary port
		wsprintf ( szMessage, _T("ASSERT( %s );  Assertion Failed: %s: File %s, Line %d\n"), lpszCondition, szExeName, lpszFileName, nLine ) ;
		::OutputDebugString ( szMessage ) ;
		lAdvancedAssertBusy-- ;

		// break into the debugger (or Dr Watson log)
		AfxDebugBreak() ;
		return FALSE ;
	}

	TCHAR szMessage[MAX_MSG ]=szNULL ;  // Separate message because of potential interlock problem. 
	TCHAR szExeName[_MAX_PATH]=szNULL ;
	if ( !GetModuleFileName(NULL, szExeName, _MAX_PATH) )
		_tcscpy ( szExeName, _T("<program name unknown>") ) ;

	if ( afxTraceEnabled )
	{
		// assume the debugger or auxiliary port
		wsprintf ( szMessage, _T("ASSERT( %s );  Assertion Failed: %s: File %hs, Line %d\n"), lpszCondition, szExeName, lpszFileName, nLine ) ;
		::OutputDebugString ( szMessage ) ;
	}

	wsprintf ( szMessage, _T("Condition: ASSERT( %s ); ") szCRLF _T("Program:%s ") szCRLF _T("File: %s ") szCRLF _T("Line: %d"), lpszCondition, szExeName, lpszFileName, nLine ) ;

	// we remove WM_QUIT because if it is in the queue then the message box
	// won't display
	MSG msg ;
	BOOL bQuit = ::PeekMessage ( &msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE ) ;
	// display the assert

	SAdvancedAssertData dataAssert ;
	dataAssert.pszMessage  = szMessage           ;
	dataAssert.bEnableSend = g_bEnableAssertSend ;


	// The dialog template must be aligned on a 4-byte boundry. 
	LPVOID lpvSend    = &dlgAdvancedAssertTemplate.itemSend    ;
	LPVOID lpvNote1   = &dlgAdvancedAssertTemplate.itemNote1   ;
	LPVOID lpvNote2   = &dlgAdvancedAssertTemplate.itemNote2   ;
	LPVOID lpvMessage = &dlgAdvancedAssertTemplate.itemMessage ;
	LPVOID lpvAbort   = &dlgAdvancedAssertTemplate.itemAbort   ;
	LPVOID lpvRetry   = &dlgAdvancedAssertTemplate.itemRetry   ;
	LPVOID lpvIgnore  = &dlgAdvancedAssertTemplate.itemIgnore  ;
	// Can't assert here, so just trace the equivalent. 
	if ( ((DWORD)lpvSend    & 0x0003) != 0 ) TRACE( _T("ASSERT( lpvSend    & 0x0003) == 0);\n") );
	if ( ((DWORD)lpvNote1   & 0x0003) != 0 ) TRACE( _T("ASSERT( lpvNote1   & 0x0003) == 0);\n") );
	if ( ((DWORD)lpvNote2   & 0x0003) != 0 ) TRACE( _T("ASSERT( lpvNote2   & 0x0003) == 0);\n") );
	if ( ((DWORD)lpvMessage & 0x0003) != 0 ) TRACE( _T("ASSERT( lpvMessage & 0x0003) == 0);\n") );
	if ( ((DWORD)lpvAbort   & 0x0003) != 0 ) TRACE( _T("ASSERT( lpvAbort   & 0x0003) == 0);\n") );
	if ( ((DWORD)lpvRetry   & 0x0003) != 0 ) TRACE( _T("ASSERT( lpvRetry   & 0x0003) == 0);\n") );
	if ( ((DWORD)lpvIgnore  & 0x0003) != 0 ) TRACE( _T("ASSERT( lpvIgnore  & 0x0003) == 0);\n") );

	HINSTANCE hInstance  =  AfxGetResourceHandle() ;
	HWND      hwndParent = _AfxGetSafeOwner ( NULL ) ;
	int nCode = ::DialogBoxIndirectParam ( hInstance, (LPCDLGTEMPLATE)&dlgAdvancedAssertTemplate, hwndParent, AdvancedAssertDlgProc, (LPARAM)&dataAssert ) ;
	// If the custom version fails, then use the original version. 
	if ( nCode == -1 )
		nCode = ::MessageBox ( hwndParent, szMessage, _T("Assertion Failed!"), MB_SYSTEMMODAL | MB_ICONHAND | MB_ABORTRETRYIGNORE ) ;
	if ( bQuit )
		::PostQuitMessage ( msg.wParam ) ;

	lAdvancedAssertBusy-- ;

#endif
	if ( nCode == IDIGNORE )
		return FALSE ;   // ignore
	else if ( nCode == IDRETRY )
		return TRUE ;    // will cause AfxDebugBreak
	else if ( nCode == IDC_SEND )
	{
		CString csSubject ;
		csSubject  = AfxGetApp()->m_pszAppName ;
		csSubject += " Assertion Failed!" ;

		// Simple MAPI does not like unicode strings, so convert to regular strings. 
		char szNote   [MAX_MSG]="" ;
		char szSubject[MAX_MSG]="" ;
		int nSubjectLength = (int)_tcslen ( csSubject ) ;
		int nMessageLength = (int)_tcslen ( szMessage ) ;
		UTBStr::tcsncpy ((TCHAR*) szSubject, MAX_MSG, csSubject, __min(nSubjectLength+1, sizeof(szSubject)) ) ;
		UTBStr::tcsncpy ((TCHAR*) szNote   , MAX_MSG, szMessage, __min(nMessageLength+1, sizeof(szNote   )) ) ;

		LPCSTR pszAttachment = NULL ;
		SendMailToBeta ( szSubject, szNote, pszAttachment ) ;
		return FALSE ;
	}
	AfxAbort() ;     // should not return (but otherwise AfxDebugBreak)
	return TRUE ;
}
#endif // _DEBUG || BETA
