// ==========================================================================
// 					Class Specification : COXService
// ==========================================================================
// Header file :		OXService.h
//
// Version: 9.3
//
// Properties:
//		NO		 Abstract class (does not have any objects)
//		NO		 Derived from CObject
//
//		NO		 Is a CWnd.                     
//		NO		 Two stage creation (constructor & Create())
//		NO		 Has a message map
//		NO		 Needs a resource (template)
//
//		NO		 Persistent objects (saveable on disk)      
//		NO		 Uses exceptions
//
// Description:
//		The class COXService is a class that provides access to NT services.
//
//		Microsoft® Windows NT® supports an application type known as a service.
//		A service conforms to the interface rules of the Service Control Manager
//		(SCM). It can be started automatically at system boot, by a user through
//		the Services control panel applet, or by an application that uses the
//		service Win32-functions. Services can execute even when no user is 
//		logged on to the system.
//
//		The class allows you to start and stop a service, create a new one, 
//		change its properties etc.

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXSERVICE_H__
#define __OXSERVICE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include <winsvc.h>


/////////////////////////////////////////////////////////////////////////////

class OX_CLASS_DECL COXService
{
// Data members -------------------------------------------------------------
public:
protected:
	static const TCHAR m_cNull;
	static const TCHAR m_cBar;
	static const LPCTSTR m_pszBar;
	static SC_HANDLE c_hSCM;
	static SC_LOCK c_Sclock;
	static DWORD c_dwSCMAccess;
	SC_HANDLE m_hSC;
	DWORD m_dwErrCode;
	CString m_sKeyName;

private:

// Member functions ---------------------------------------------------------
public:
	COXService(LPCTSTR pszKeyName = NULL);
	// --- In      : pszKeyName, the service key name to start with
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Constructor of object

	COXService(const COXService& service);
	// --- In      : service, the service to copy
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Copy constructor for the object.
	//				 The destination service will not inherit the handle from
	//				 the source. But because the key name is copied, it can 
	//				 be opened again.

	virtual ~COXService();
	// --- In      :
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Destructor of object. If it's opened, it will be closed.

	COXService& operator=(const COXService& service);
	// --- In      : service, the service to copy
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Assignment operator for the object.
	//				 The destination service will not inherit the handle from
	//				 the source. But because the key name is copied, it can 
	//				 be opened again.

	void Empty();
	// --- In      :
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Clears the entire contents of the objects,

	BOOL IsEmpty() const;
	// --- In      :
	// --- Out     : 
	// --- Returns : TRUE if empty; FALSE otherwise.
	// --- Effect  : checks wether it is empty
	//				 A service object is fully defined through its key name.
	//				 "Empty" means not only closed, but also no key name is
	//				 specified.

	CString GetKeyName() const;
	// --- In      :
	// --- Out     : 
	// --- Returns : service key name
	// --- Effect  : retrieve the key name

	void SetKeyName(LPCTSTR pszKeyName);
	// --- In      : pszKeyName, new key name to be associated with this object
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Change service key name. This may close a possible 
	//				 open service handle.
	//				 NOTE: you can never change key name of the currently
	//				 opened service (you may change display name), because
	//				 a service's key name is defined by the programmer who
	//				 creates the service.

	BOOL Open(DWORD dwDesiredAccess = SERVICE_ALL_ACCESS, 
			  LPCTSTR pszKeyName = NULL);
	// --- In      : dwDesiredAccess, specifies under which security options
	//					you want to open the service
	//				 pszKeyName, the service's key name; can be NULL only when
	//					you previously specified a key name
	// --- Out     : 
	// --- Returns :
	// --- Effect  : open a service

	BOOL Start(DWORD dwNumServiceArgs = 0, 
			   LPCTSTR* rgpszServiceArgVectors = NULL);
	// --- In      : dwNumServiceArgs, argument's number
	//				 rgpszServiceArgVectors, argument list
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise, check GetLastError()
	//					to get the error type
	// --- Effect  : start a service; if it's not opened yet, it will be opened
	//				 for you with dafault parameters (see Open()).

	BOOL Pause();
	// --- In      :
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise, check GetLastError()
	//					to get the error type
	// --- Effect  : pause a started service

	BOOL Continue();
	// --- In      :
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise, check GetLastError()
	//					to get the error type
	// --- Effect  : continue a paused service

	BOOL Stop();
	// --- In      :
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise, check GetLastError()
	//					to get the error type
	// --- Effect  : stop a started service

	BOOL Control(DWORD dwControl, 
				 LPSERVICE_STATUS pszServiceStatus = NULL);
	// --- In      : dwControl, the control code, can be: SERVICE_CONTROL_STOP,
	//					SERVICE_CONTROL_PAUSE, SERVICE_CONTROL_CONTINUE or 
	//					SERVICE_CONTROL_INTERROGATE
	//				 pszServiceStatus, a SERVICE_STATUS structure to receive
	//					service's status (after actions taken)
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise, check GetLastError()
	//					to get the error type
	// --- Effect  : a more general control over a service

	BOOL Create(LPCTSTR pszBinaryPathName,
				LPCTSTR pszDisplayName, 
				LPCTSTR pszKeyName = NULL, 
				DWORD dwDesiredAccess = SERVICE_ALL_ACCESS, 
				DWORD dwServiceType = SERVICE_WIN32_OWN_PROCESS, 
				DWORD dwStartType = SERVICE_DEMAND_START, 
				DWORD dwErrorControl = SERVICE_ERROR_NORMAL, 
				LPCTSTR pszLoadOrderGroup = NULL, 
				LPDWORD pdwTagId = NULL, 
				LPCTSTR pszDependencies = NULL, 
				LPCTSTR pszServiceStartName = NULL, 
				LPCTSTR pszPassword = NULL);
	// --- In      : (see Win32's CreateService() for the meaning of these 
	//				 parameters)
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise, check GetLastError()
	//					to get the error type
	// --- Effect  : create a new service

	BOOL Delete();
	// --- In      :
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise, check GetLastError()
	//					to get the error type
	// --- Effect  : delete a service from the service database

	BOOL Close();
	// --- In      :
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise, check GetLastError()
	//					to get the error type
	// --- Effect  : close an opened service

	SC_HANDLE GetHandle();
	// --- In      :
	// --- Out     : 
	// --- Returns : the service handle
	// --- Effect  : obtain the handle of the service; if service is not opened,
	//				 it will be opened for you, and return the handle

	// for following functions
	// --- see Win32's ChangeServiceConfig() for detailed parameter's descriptions
	// --- Returns : TRUE if successful; FALSE otherwise, check GetLastError()
	//					to get the error type
	// NOTE: Dependencies are expressed as names separated by a bar (|).
	BOOL ChangeDisplayName(LPCTSTR pszDisplayName);
	BOOL ChangeServiceType(DWORD dwServiceType);
	BOOL ChangeStartType(DWORD dwStartType);
	BOOL ChangeErrorControl(DWORD dwErrorControl);
	BOOL ChangeBinaryPathName(LPCTSTR pszBinaryPathName);
	BOOL ChangeLoadOrderGroup(LPCTSTR pszLoadOrderGroup);
	BOOL ChangeTagId(DWORD dwTagId);
	BOOL ChangeDependencies(LPCTSTR pszDependencies);
	BOOL ChangeStartName(LPCTSTR pszServiceStartName, 
						 LPCTSTR pszPassword = NULL);
	BOOL ChangeConfig(LPCTSTR pszDisplayName = NULL, 
					  DWORD dwServiceType = SERVICE_NO_CHANGE, 
					  DWORD dwStartType = SERVICE_NO_CHANGE, 
					  DWORD dwErrorControl = SERVICE_NO_CHANGE, 
					  LPCTSTR pszBinaryPathName = NULL, 
					  LPCTSTR pszLoadOrderGroup = NULL, 
					  LPDWORD pdwTagId = NULL, 
					  LPCTSTR pszDependencies = NULL, 
					  LPCTSTR pszServiceStartName = NULL, 
					  LPCTSTR pszPassword = NULL);

	// for following functions
	// --- see Win32's QueryServiceConfig() for detailed parameter's descriptions
	// NOTE: Dependencies are expressed as names separated by a bar (|).
	CString QueryDisplayName();
	DWORD QueryServiceType();
	DWORD QueryStartType();
	DWORD QueryErrorControl();
	CString QueryBinaryPathName();
	CString QueryLoadOrderGroup();
	DWORD QueryTagId();
	CString QueryDependencies();
	CString QueryStartName();
	BOOL QueryConfig(LPQUERY_SERVICE_CONFIG lpServiceConfig, 
					 DWORD cbBufSize, 
					 LPDWORD pcbBytesNeeded);

	// for following functions
	// --- see Win32's QueryServiceObjectSecurity() for detailed parameter's 
	//	   descriptions
	// --- Returns : TRUE if successful; FALSE otherwise, check GetLastError()
	//					to get the error type
	BOOL QuerySecurity(SECURITY_INFORMATION dwSecurityInformation, 
					   PSECURITY_DESCRIPTOR lpSecurityDescriptor, 
					   DWORD cbBufSize, 
					   LPDWORD pcbBytesNeeded);
	BOOL ChangeSecurity(SECURITY_INFORMATION dwSecurityInformation, 
						PSECURITY_DESCRIPTOR lpSecurityDescriptor);

	BOOL QueryStatus(LPSERVICE_STATUS lpServiceStatus, 
		BOOL bInterrogateNow = FALSE);
	// --- In      : lpServiceStatus, pointer to a service status structure
	//					to receive the info
	//				 bInterrogateNow, specify whether re-query the state of
	//					the service to update the service manager
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise, check GetLastError()
	//					to get the error type
	// --- Effect  : obtain service's status

	DWORD QueryCurrentState(BOOL bInterrogateNow = FALSE);
	// --- In      : bInterrogateNow, specify whether re-query the state of
	//					the service to update the service manager
	// --- Out     : 
	// --- Returns : current state of the service
	// --- Effect  : retrieve service's state

	DWORD GetLastError() const;
	// --- In      :
	// --- Out     : 
	// --- Returns : last error code
	// --- Effect  : get last error's code

	// SCManager Functions
	static SC_HANDLE GetSCManagerHandle();
	// --- In      :
	// --- Out     : 
	// --- Returns : service manager's handle
	// --- Effect  : get service manager's handle, if it's not opened yet,
	//				 it will be opened for you, but it is your responsibility
	//				 to close it

	static BOOL OpenSCManager(LPCTSTR pszMachineName = NULL, 
							  LPCTSTR pszDatabaseName = NULL, 
							  DWORD dwDesiredAccess = SC_MANAGER_ALL_ACCESS);
	// --- In      : (see Win32's OpenSCManager())
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : open the service manager
	//				 Note: even though you don't need to call this function 
	//					explicitly (calling other functions that need an opened
	//					service manager will open the manager using default
	//					parameters in this function), you need to call 
	//					CloseSCManager() anyway. Therefore, it's a good habit to
	//					call CloseSCManager() all the time.

	static BOOL CloseSCManager();
	// --- In      :
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : close the service manager. If it's already closed, it 
	//				 does nothing (return TRUE).

	static BOOL LockDatabase();
	// --- In      :
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : lock service's database

	static BOOL UnlockDatabase();
	// --- In      :
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : unlock service's database

	static BOOL QueryDatabaseLockStatus(DWORD& bIsLocked, 
										CString& sLockOwner, 
										DWORD& dwLockDuration);
	// --- In      : 
	// --- Out     : bIsLocked, whether the database is loacked
	//				 sLockOwner, if locked, the owner of the lock
	//				 dwLockDuration, if locked, the duration of the lock
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : retreieve service database's locking status

	static CString QueryDisplayName(LPCTSTR pszKeyName);
	// --- In      : pszKeyName, query service's key name 
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : get display name for a given key name

	static CString QueryKeyName(LPCTSTR pszDisplayName);
	// --- In      : pszDisplayName, query service's display name 
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : get key name for a given display name

protected:
	BOOL SetErrCode(BOOL bFunctionSucceeded);
	BOOL PrepareSCHandle(DWORD dwMinimumAccess = SERVICE_ALL_ACCESS);
	static BOOL PrepareSCManager(DWORD dwMinimumAccess = SC_MANAGER_ALL_ACCESS);
	
	CString GetQSCData(int QSC_Code, DWORD* pdwResult = NULL);
	void NullToBarSeparator(LPTSTR pStringData);
	void BarToNullSeparator(LPTSTR pStringData);
private:
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __OXSERVICE_H__

// end of OXService.h