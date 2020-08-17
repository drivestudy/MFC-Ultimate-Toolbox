// ==========================================================================
//				Class Implementation : COXWatchedDir
// ==========================================================================

// Source file : OXWatchedDir.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
			  
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXWatchedDir.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// COXWatchedDir

/////////////////////////////////////////////////////////////////////////////
// Definition of static members
#if defined(_UNICODE) && (_WIN32_WINNT >= 0x400)
DWORD COXWatchedDir::dwBytesReturned = 0;
// --- Used only in the call of the ReadDirectoryChangesW() function
#endif // defined(_UNICODE) && (_WIN32_WINNT >= 0x400)

// Data members -------------------------------------------------------------
// protected:
	// CString	m_sPath;				
	// --- Directory to be watched

	// BOOL	m_bWatchSubTree;		
	// --- Flag for monitoring directory or directory tree 

	// DWORD	m_dwWatchFilter;		
	// --- Filter conditions to watch for 

	// HWND	m_hwndWindowToNotify;	
	// --- Handle of the window to be notified

	// BOOL	m_bPost;				
	// --- Flag for posting or sending

	// HANDLE	m_hEvent;				
	// --- Handle of the event to be signaled

// #if defined(_UNICODE) && (_WIN32_WINNT >= 0x400)
	// BOOL	m_bExtended;			
	// --- Is extended info availanle

	// HANDLE	m_hDirectory;			
	// --- Handle to the directory to be watched 

	// LPVOID	m_lpBuffer;				
	// --- Pointer to the buffer to receive the read results 

	// DWORD	m_nBufferLength;		
	// --- Length of lpBuffer 

	// OVERLAPPED* m_pOverlapped;		
	// --- Pointer to overlapped structure
// #endif // defined(_UNICODE) && (_WIN32_WINNT >= 0x400)

// private:

// Member functions ---------------------------------------------------------
// public:
// protected:

COXWatchedDir::COXWatchedDir(CString sPath, BOOL bWatchSubTree, DWORD dwWatchFilter, BOOL bExtended)
	// --- In  : sPath: the path to be watched
	//			 bWatchSubTree: indicates whether to watch subtree or not
	//			 dwWatchFilter: specifys which type of changes should be reported
	//							For possible values see public data members Notification types
	//			 bExtended: if TRUE - extended info is available
	// --- Out : 
	// --- Returns : 
	// --- Effect : constructs the object
	:
	m_sPath(sPath),
	m_bWatchSubTree(bWatchSubTree),
	m_dwWatchFilter(dwWatchFilter),
	m_hwndWindowToNotify(NULL),
	m_bPost(TRUE), 
	m_hEvent(NULL)
#if defined(_UNICODE) && (_WIN32_WINNT >= 0x400)
// Extended info is supported
	,
	m_bExtended(bExtended),
	m_hDirectory(NULL),
	m_lpBuffer(NULL),
	m_nBufferLength(0),
	m_pOverlapped(NULL)
	{
	}
#else
	{
	UNUSED_ALWAYS(bExtended);
	}
#endif // defined(_UNICODE) && (_WIN32_WINNT >= 0x400)

COXWatchedDir::~COXWatchedDir()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Object destructor
	{
#if defined(_UNICODE) && (_WIN32_WINNT >= 0x400)
	if(m_bExtended)
		{
		if(!HasOverlappedIoCompleted(m_pOverlapped))
			{
			::CancelIo(m_hDirectory);
			}
		
		delete m_pOverlapped;
		::CloseHandle(m_hEvent);
		::CloseHandle(m_hDirectory);
		delete [] m_lpBuffer;
		}
	else
#endif // defined(_UNICODE) && (_WIN32_WINNT >= 0x400)

		::FindCloseChangeNotification(m_hEvent);
	}

BOOL COXWatchedDir::FindFirstHandle(HRESULT& rhrResult)
	// --- In  :
	// --- Out : rhrResult: HERESULT of the last operation inside the function
	// --- Returns : TRUE if object is successfuly initialized
	// --- Effect : Initializes the object and creates the handle used for wait functions
	{
	rhrResult = ERROR_SUCCESS;

#if defined(_UNICODE) && (_WIN32_WINNT >= 0x400)
// Extended info is supported
	
	if(m_bExtended)
		{  // Extended info will be supported
		m_nBufferLength = sizeof(FILE_NOTIFY_INFORMATION) + _MAX_PATH * sizeof(TCHAR) * 2;
		m_lpBuffer = new char[m_nBufferLength];

		// Create directory handle
		m_hDirectory = ::CreateFile(m_sPath,												// pointer to the file name
								   FILE_LIST_DIRECTORY,									// access (read-write) mode
								   FILE_SHARE_READ|FILE_SHARE_DELETE|FILE_SHARE_WRITE,	// share mode
								   NULL,												// security descriptor
								   OPEN_EXISTING,										// how to create
								   FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,	// file attributes
								   NULL);												// file with attributes to copy
		if(m_hDirectory != INVALID_HANDLE_VALUE)
			{ //... Directory handle is created.
			m_hEvent = ::CreateEvent(NULL,		// pointer to security attributes 
									TRUE,		// flag for manual-reset event 
									FALSE,		// flag for initial state 
									NULL);		// pointer to event-object name 
												// In this case unnamed event
			if(m_hEvent != NULL)
				{ // ... Event created.
				m_pOverlapped = new OVERLAPPED;
				m_pOverlapped->hEvent = m_hEvent;
				BOOL bRetVal = ::ReadDirectoryChangesW(
									   m_hDirectory,	// handle to the directory to be watched 
									   m_lpBuffer,		// pointer to the buffer to receive the read results 
									   m_nBufferLength, // length of lpBuffer 
									   m_bWatchSubTree, // flag for monitoring directory or directory tree 
									   m_dwWatchFilter, // filter conditions to watch for 
									   &dwBytesReturned,// number of bytes returned. For asynchronous calls, this parameter is undefined
									   m_pOverlapped,	// pointer to structure needed for overlapped I/O 
									   NULL); //FileIOCompletionRoutine); // pointer to completion routine 
				if(bRetVal != FALSE)
					{ // It's OK
					return TRUE;
					}
				else
					{ // Something wrong. Do cleanup and try FindFirstChangeNotification
					rhrResult = HRESULT_FROM_WIN32(::GetLastError());
					}
				delete m_pOverlapped;
				m_pOverlapped = NULL;

				::CloseHandle(m_hEvent);
				m_hEvent = NULL;
				}
			else
				{ //if(m_hEvent != NULL)
				rhrResult = HRESULT_FROM_WIN32(::GetLastError());
				}
			::CloseHandle(m_hDirectory);
			m_hDirectory = NULL;
			}
		else
			{ // if(m_hDirectory !=0 )
			rhrResult = HRESULT_FROM_WIN32(::GetLastError());
			}
		delete [] m_lpBuffer;
		m_lpBuffer = NULL;
		m_nBufferLength = 0;
		m_bExtended = FALSE;
		}
#endif // defined(_UNICODE) && (_WIN32_WINNT >= 0x400)

	
	// No extended information is suported. Try FindFirstChangeNotification()
	m_hEvent = ::FindFirstChangeNotification(m_sPath,			// pointer to name of directory to watch 
										  m_bWatchSubTree,	// flag for monitoring directory or directory tree 
										  m_dwWatchFilter);	// filter conditions to watch for 
	if(m_hEvent != INVALID_HANDLE_VALUE)
		{
		// OK
		return TRUE;
		}
	else
		{
		rhrResult = HRESULT_FROM_WIN32(::GetLastError());
		m_hEvent = NULL;
		}

	return FALSE;
	}

BOOL COXWatchedDir::FindNextHandle(HANDLE& hHandle)
	// --- In  :
	// --- Out : rhrResult: HERESULT of the last operation inside the function
	// --- Returns : TRUE if success
	// --- Effect : Reinitializes the handle used for wait functions
	{
#if defined(_UNICODE) && (_WIN32_WINNT >= 0x400)
// Extended info is supported
	if(m_bExtended)
		{
		::ResetEvent(hHandle);
		return ::ReadDirectoryChangesW(m_hDirectory,	// handle to the directory to be watched 
									  m_lpBuffer,		// pointer to the buffer to receive the read results 
									  m_nBufferLength,	// length of lpBuffer 
									  m_bWatchSubTree,	// flag for monitoring directory or directory tree 
									  m_dwWatchFilter,	// filter conditions to watch for 
									  &dwBytesReturned,	// number of bytes returned. For asynchronous calls, this parameter is undefined
									  m_pOverlapped,	// pointer to structure needed for overlapped I/O 
									  NULL);			// pointer to completion routine 
		}
	else
#endif // defined(_UNICODE) && (_WIN32_WINNT >= 0x400)

		return ::FindNextChangeNotification(hHandle);
	}


// private:

// //////////////////////////////////////////////////////////////////////////

