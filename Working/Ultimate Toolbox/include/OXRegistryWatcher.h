// ==========================================================================
// 					Class Specification : COXRegistryWatcher
// ==========================================================================

// Header file : OXRegistryWatcher.h

// Version: 9.3
      
// //////////////////////////////////////////////////////////////////////////

// Properties :
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CObject

//	NO	Is a Cwnd.
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Description :
//	This class can be used to organize Registry change notifications.
//	It notifies the caller about changes to the attributes or contents of a specified Registry key.
//	All notifications are handled by virtual function, which can be overridden in derived classes.
//	COXRegistryWatcher class can also convert notifications to a window message. This message
//	can be sent to a specified window and translated there (e.g. by PreTranslateMessage()).
//	Note that COXRegistryWatcher doesn't notify the caller if the specified key is deleted.

// Example :
//		COXRegistryWatcher	RegWatch;
//		if ( ::RegOpenKeyEx(hKey, sKeyName, 0, KEY_NOTIFY, &hKeyAdd) == ERROR_SUCCESS && hKeyAdd )
//		{
//		if ( RegWatch.EnableWindowNotification(RegWatch.AddWatch(hKeyAdd, TRUE), this) )
//			TRACE1("Watched Key : %s\n", sKeyName);
//		}

// Remark :
//	Registry watch is carried out by the separate worker thread.
//	Therefore COXRegistryWatcher's Registry watching doesn't "hung" your programs.

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#ifndef	__OXREGISTRYWATCHER_H__
#define	__OXREGISTRYWATCHER_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"

#include "OXRegistryWatchNotifier.h"



// //////////////////////////////////////////////////////////////////////////
// Definitions of error codes.
#define OX_FACILITY_REGISTRY_WATCHER		0x221
#define OX_STATUS_SEVERITY_SUCCESS			0x0
#define OX_STATUS_SEVERITY_ERROR			0x3
#define OX_CODE_REGISTRY_WATCHER_ERROR_SUCCESS		1
#define OX_CODE_REGISTRY_WATCHER_NO_HKEY			2
#define OX_CODE_REGISTRY_WATCHER_THREAD_FAILURE		3
#define OX_CODE_REGISTRY_WATCHER_EVENT_FAILURE		4
#define OX_CODE_REGISTRY_WATCHER_EMPTY_WATCHER		5
#define OX_CODE_REGISTRY_WATCHER_INCORRECT_HKEY		6
#define OX_CODE_REGISTRY_WATCHER_ALREADY_STARTED	7
#define OX_CODE_REGISTRY_WATCHER_ALREADY_STOPPED	8
#define OX_CODE_REGISTRY_WATCHER_VERSION_FAILURE	9
#define OX_CODE_REGISTRY_WATCHER_NO_NOTIFIER		10
#define OX_CODE_REGISTRY_WATCHER_INCORRECT_ID		11
#define OX_CODE_REGISTRY_WATCHER_SYNCHRO_FAILURE	12

// The operation completed successfully.
#define OX_REGISTRY_WATCHER_ERROR_SUCCESS	MAKE_HRESULT(OX_STATUS_SEVERITY_SUCCESS,\
											OX_FACILITY_REGISTRY_WATCHER, OX_CODE_REGISTRY_WATCHER_ERROR_SUCCESS)

// Registry key is not specifying.
#define OX_REGISTRY_WATCHER_NO_HKEY			MAKE_HRESULT(OX_STATUS_SEVERITY_ERROR,\
											OX_FACILITY_REGISTRY_WATCHER, OX_CODE_REGISTRY_WATCHER_NO_HKEY)

// Failure to start or process the worker thread.
#define OX_REGISTRY_WATCHER_THREAD_FAILURE	MAKE_HRESULT(OX_STATUS_SEVERITY_ERROR,\
											OX_FACILITY_REGISTRY_WATCHER, OX_CODE_REGISTRY_WATCHER_THREAD_FAILURE)

// Failure of notification event initialization.
#define OX_REGISTRY_WATCHER_EVENT_FAILURE	MAKE_HRESULT(OX_STATUS_SEVERITY_ERROR,\
											OX_FACILITY_REGISTRY_WATCHER, OX_CODE_REGISTRY_WATCHER_EVENT_FAILURE)

// No watched keys (empty "watch queue").
#define OX_REGISTRY_WATCHER_EMPTY_WATCHER	MAKE_HRESULT(OX_STATUS_SEVERITY_ERROR,\
											OX_FACILITY_REGISTRY_WATCHER, OX_CODE_REGISTRY_WATCHER_EMPTY_WATCHER)

// Registry key is not watched by this COXRegistryWatcher object.
#define OX_REGISTRY_WATCHER_INCORRECT_HKEY	MAKE_HRESULT(OX_STATUS_SEVERITY_ERROR,\
											OX_FACILITY_REGISTRY_WATCHER, OX_CODE_REGISTRY_WATCHER_INCORRECT_HKEY)

// Starting thread is already started.
#define OX_REGISTRY_WATCHER_ALREADY_STARTED	MAKE_HRESULT(OX_STATUS_SEVERITY_ERROR,\
											OX_FACILITY_REGISTRY_WATCHER, OX_CODE_REGISTRY_WATCHER_ALREADY_STARTED)

// Stoping thread is already stoped.
#define OX_REGISTRY_WATCHER_ALREADY_STOPPED	MAKE_HRESULT(OX_STATUS_SEVERITY_ERROR,\
											OX_FACILITY_REGISTRY_WATCHER, OX_CODE_REGISTRY_WATCHER_ALREADY_STOPPED)

// Failure to retrieve the Windows version.
#define OX_REGISTRY_WATCHER_VERSION_FAILURE	MAKE_HRESULT(OX_STATUS_SEVERITY_ERROR,\
											OX_FACILITY_REGISTRY_WATCHER, OX_CODE_REGISTRY_WATCHER_VERSION_FAILURE)

// Can't to find specified notifier.
#define OX_REGISTRY_WATCHER_NO_NOTIFIER		MAKE_HRESULT(OX_STATUS_SEVERITY_ERROR,\
											OX_FACILITY_REGISTRY_WATCHER, OX_CODE_REGISTRY_WATCHER_NO_NOTIFIER)

// Incorrect notifier ID.
#define OX_REGISTRY_WATCHER_INCORRECT_ID	MAKE_HRESULT(OX_STATUS_SEVERITY_ERROR,\
											OX_FACILITY_REGISTRY_WATCHER, OX_CODE_REGISTRY_WATCHER_INCORRECT_ID)

// Failure of synchronization event setting.
#define OX_REGISTRY_WATCHER_SYNCHRO_FAILURE	MAKE_HRESULT(OX_STATUS_SEVERITY_ERROR,\
											OX_FACILITY_REGISTRY_WATCHER, OX_CODE_REGISTRY_WATCHER_SYNCHRO_FAILURE)

// //////////////////////////////////////////////////////////////////////////

class OX_CLASS_DECL COXRegistryWatcher : public CObject
{
// Data members -------------------------------------------------------------
public:
	// Flags to specify which type of changes should be reported (for AddWatch()).
	const static DWORD OXRegistryWatchChangeName;		// Subkey was added or deleted.
	const static DWORD OXRegistryWatchChangeAttributes;	// Changes to the attributes of the key.
	const static DWORD OXRegistryWatchChangeLastSet;	// Changes to a value of the key.
	const static DWORD OXRegistryWatchChangeSecurity;	// Changes to the security descriptor of the key.

protected:
	CEvent						m_EventWatchLoop;
	CEvent						m_EventWatchRestart;
	CEvent						m_EventWatchBuildBegin;
	CEvent						m_EventWatchBuildEnd;
	HRESULT						m_hResultError;
	DWORD						m_dwWatchesNumber;
	CWinThread*					m_pNotificationThread;
	COXRegistryWatchNotifier*	m_pRegistryWatchNotifier;

private:

// Member functions ---------------------------------------------------------
public:
	COXRegistryWatcher();
	// --- In :
	// --- Out :
	// --- Returns :
	// --- Effect :		Constructs the COXRegistryWatcher object.

	virtual ~COXRegistryWatcher();
	// --- In :
	// --- Out :
	// --- Returns :
	// --- Effect :		Removes all watched keys and destroys the COXRegistryWatcher object.

	BOOL IsWatchingSupported();
	// --- In :
	// --- Out :
	// --- Returns :	TRUE - if Registry key watching is supported, FALSE - otherwise.
	// --- Effect :		This function returns whether Registry key watching is supported.
	//					This is only supported on Windows NT version 4.0 and higher.

	DWORD AddWatch(HKEY hRegKey, BOOL bWatchSubtree = FALSE,
				   DWORD dwWatchFilter = OXRegistryWatchChangeName | OXRegistryWatchChangeLastSet);
	// --- In :			hRegKey :		Registry key to be watched.
	//					bWatchSubtree :	Flag that indicates whether to report changes in the specified key
	//									and all of its subkeys or only in the specified key. If this parameter
	//									is TRUE, the class reports changes in the key and its subkeys.
	//									If the parameter is FALSE, the class reports changes only in the key.
	//					dwWatchFilter :	Specifies a combination of flags that control which changes should be
	//									reported. Flags are described above in the "Data members - public:"
	//									section of COXRegistryWatcher Class Specification.
	// --- Out :
	// --- Returns :	If the function succeeds - ID of added notifier; if the function fails - 0.
	//					To get extended error information, call GetLastError() method of this class.
	// --- Effect :		Adds Registry key watch. When hRegKey changes, class calls virtual function
	//					OnNotify(), which can be overridden in your derived class.
	//					You can add unlimited number of keys to watch.

	virtual BOOL OnNotify(COXRegistryWatchNotifier* pRegWatchNotifier);
	// --- In :			pRegWatchNotifier : Copy of the received parameters in the form of a
	//										pointer to COXRegistryWatchNotifier object.
	// --- Out :
	// --- Returns :	Indicates whether this notification should be ignored (TRUE) or whether the
	//					object may continue handling the event (FALSE). The last option (FALSE) is
	//					necessary if the event is to be translated into a window message.
	// --- Effect :		This function is called when a Registry change notification is received by
	//					specified Registry watch object. It can be overriden in derived classes.

	BOOL GetWatchIDsFromKey(HKEY hRegKey, CDWordArray& IDs);
	// --- In :			hRegKey :	Watched Registry key.
	//					IDs :		Array to store indexes of notifiers linked with specified key.
	// --- Out :		IDs :		Array of indexes of notifiers linked with specified key.
	// --- Returns :	FALSE, if IDs array is empty; TRUE otherwise.
	// --- Effect :		Retrieves indexes of notifiers (COXRegistryWatchNotifier objects) that
	//					linked to specified Registry key.

	DWORD COXRegistryWatcher::GetNotifCount();
	// --- In :
	// --- Out :
	// --- Returns :	Number of notifiers. If no notifiers added, returns 0.
	// --- Effect :		Returns number of successfully added by AddWatch() function notifiers
	//					(COXRegistryWatchNotifier objects), in other words - returns number of
	//					watched Registry keys.

	BOOL RemoveWatch(DWORD dwID);
	// --- In :			dwID :	Notifier ID (watched Registry key, that was added by AddWatch()).
	// --- Out :
	// --- Returns :	TRUE - if the function succeeds, FALSE - if the function fails.
	//					To get extended error information, call GetLastError() method of this class.
	// --- Effect :		Removes the specified notifier (watch of linked Registry key).

	BOOL RemoveAllWatches();
	// --- In :
	// --- Out :
	// --- Returns :	TRUE - if the function succeeds, FALSE - if the function fails.
	//					To get extended error information, call GetLastError() method of this class.
	// --- Effect :		Removes all notifiers (watches on all current watched keys).

	BOOL EnableWindowNotification(DWORD dwID, CWnd* pWnd, BOOL bPost = TRUE);
	// --- In :			dwID :	ID of notifier (watched Registry key, that was added by AddWatch()).
	//					pWnd :	Points to the CWnd class object, that represents window,
	//							which will receive the notification message when Registry key changes.
	//					bPost :	If TRUE, message posts (by default), if FALSE, message sends.
	// --- Out :
	// --- Returns :	TRUE - if the function succeeds, FALSE - if the function fails.
	//					To get extended error information, call GetLastError() method of this class.
	// --- Effect :		Connects a future notification of a Registry watch to a message
	//					to a particular window. When a notification is received for that
	//					particular key, a specific message is posted/sent to the specified window.
	//					This message is WM_OX_REGISTRY_NOTIFY, wParam is index of received
	//					notification object (object can be retrieved by GetWatchNotifier()),
	//					and lParam is a handle of watched Registry key (HKEY).

	BOOL DisableWindowNotification(DWORD dwID);
	// --- In :			dwID :	ID of notifier (watched Registry key, that was added by AddWatch()).
	// --- Out :
	// --- Returns :	TRUE - if the function succeeds, FALSE - if the function fails.
	//					To get extended error information, call GetLastError() method of this class.
	// --- Effect :		Removes connection between specified notification and window.

	BOOL DisableAllWindowNotifications();
	// --- In :
	// --- Out :
	// --- Returns :	TRUE - if the function succeeds, FALSE - if the function fails.
	//					To get extended error information, call GetLastError() method of this class.
	// --- Effect :		Removes all connections between notifications of Registry keys and windows.
	
	COXRegistryWatchNotifier* GetWatchNotifier(DWORD dwID, DWORD* pdwQueueIndex = NULL);
	// --- In :			dwID :			ID of notification object (COXRegistryWatchNotifier).
	//					pdwQueueIndex :	Address of buffer for index of this object in "watch queue".
	//									This parameter can be NULL (default).
	// --- Out :		pdwQueueIndex :	Index of this object in "watch queue" (if pdwQueueIndex != NULL).
	// --- Returns :	Pointer to COXRegistryWatchNotifier object.
	//					When no object is linked to specified ID, NULL is returned.
	// --- Effect :		Returns a specific Registry watch notifier object.
	//					This function can be used for notifier ID validity check.

	HRESULT GetLastError() const;
	// --- In :
	// --- Out :
	// --- Returns :	Code of the last error. Codes are described above in the
	//					"Definitions of error codes" section of Class Specification.
	// --- Effect :		Returns the error code of the last error that has occurred.

protected:
	void AfterNotify(COXRegistryWatchNotifier* pRegWatchNotifier);
	static UINT RegistryWatchThreadFunction(LPVOID pParam);
	BOOL StartWatchThread();
	BOOL StopWatchThread();
	BOOL IsWatchStarted();
	DWORD FindNewID();

private:

};

#endif	// __OXREGISTRYWATCHER_H__
// ==========================================================================
