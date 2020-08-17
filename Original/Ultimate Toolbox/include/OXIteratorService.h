// ==========================================================================
// 					Class Specification : COXIteratorService
// ==========================================================================
// Header file :		OXIteratorService.h
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
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
//		This class is derived from COXService. It can be used to iterate 
//		through a list of services. Because the iterator object is itself
//		a service, it is easy to apply functions to each service in a list.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __OXITERATORSERVICE_H__
#define __OXITERATORSERVICE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include "OXService.h"


/////////////////////////////////////////////////////////////////////////////

class OX_CLASS_DECL COXIteratorService : public COXService
{
// Data members -------------------------------------------------------------
public:
protected:
	CStringArray m_SrvKeyNames;
	CStringArray m_SrvDspNames;
	int m_nPos;
private:

// Member functions ---------------------------------------------------------
public:
	COXIteratorService(LPCTSTR pszKeyName = NULL);
	// --- In      : pszKeyName, the service key name to start with
	// --- Out     :
	// --- Returns :
	// --- Effect  : Constructor

	COXIteratorService(const COXService& service);
	// --- In      : service, the service to copy
	// --- Out     :
	// --- Returns :
	// --- Effect  : Constructor

	COXIteratorService(const COXIteratorService& iteratorService);
	// --- In      : iteratorService, the iterator service to copy
	// --- Out     :
	// --- Returns :
	// --- Effect  : Constructor

	COXIteratorService& operator=(const COXService& service);
	// --- In      : service, the service to copy
	// --- Out     :
	// --- Returns :
	// --- Effect  : copy a serice

	COXIteratorService& operator=(const COXIteratorService& iteratorService);
	// --- In      : iteratorService, the iterator service to copy
	// --- Out     :
	// --- Returns :
	// --- Effect  : copy an iterator service

	BOOL StartIteration(DWORD dwServiceType = SERVICE_WIN32 | SERVICE_DRIVER,
		DWORD dwServiceState = SERVICE_STATE_ALL);
	// --- In      : dwServiceType, service type to set filter on
	//				 dwServiceState, service state to set filter on
	// --- Out     :
	// --- Returns : TRUE if successful (even though it may be empty()); 
	//				 FALSE otherwise (use GetLastError() to check).
	// --- Effect  : starts the iteration. You can specify some filters 
	//				 (which type and state you are interested in). 
	//				 This object will get the new value. When this object is
	//				 empty, the end of the iteration has been reached.
	
	BOOL StartDependentIteration(DWORD dwServiceState = SERVICE_STATE_ALL);
	// --- In      : dwServiceState, service state to set filter on
	// --- Out     :
	// --- Returns : TRUE if successful (even though it may be empty()); 
	//				 FALSE otherwise (use GetLastError() to check).
	// --- Effect  : starts the iteration. It will iterate the services that
	//				 depend on this service. Note that this service must 
	//				 already been given a key name. You can specify which 
	//				 state you are interested in. This object will get the 
	//				 new value. When this object is empty, the end of the 
	//				 iteration has been reached.
	//				 The returned services entries are ordered in the reverse
	//				 order of the start order, with group order taken into 
	//				 account. If you need to stop the dependent services, 
	//				 you can use this order to stop the dependent services
	//				 in the proper order.
	
	BOOL Next();
	// --- In      :
	// --- Out     :
	// --- Returns : TRUE if successful; FALSE if already at the end (this
	//				 will be empty)
	// --- Effect  : move to next service in the iteration

	BOOL Prev();
	// --- In      :
	// --- Out     :
	// --- Returns : TRUE if successful; FALSE if already at the beginning
	//				 (this will be empty)
	// --- Effect  : move to previous service in the iteration

	COXIteratorService& operator++(); 
	// --- In      :
	// --- Out     :
	// --- Returns : resulted current service
	// --- Effect  : move to next service in the iteration
	//				 if over the end, no error occurs (this will be empty).

	COXIteratorService operator+(int nOffset); 
	// --- In      : nOffset, the offset to move forward
	// --- Out     :
	// --- Returns : resulted current service
	// --- Effect  : move forward nOffset positions in the iteration
	//				 if over the end, this will be empty, and 
	//				 no error occurs. if over the beginning (when nOffset is
	//				 negative), this will be empty, and no error occurs.

	COXIteratorService& operator--();
	// --- In      :
	// --- Out     :
	// --- Returns : resulted current service
	// --- Effect  : move to previous service in the iteration
	//				 if over the beginning, no error occurs (this will be empty).

	COXIteratorService operator-(int nOffset); 
	// --- In      : nOffset, the offset to move backward
	// --- Out     :
	// --- Returns : resulted current service
	// --- Effect  : move backward nOffset positions in the iteration
	//				 if over the beginning, this will be empty, and no error
	//				 occurs. if over the end (when nOffset is negative), this
	//				 will be empty, and no error occurs.

	void GetKeyNames(CStringArray& sBuffer);
	// --- In      : sBuffer, the string array to store returned key names
	// --- Out     :
	// --- Returns :
	// --- Effect  : get all key names from the current iteration

	void GetDisplayNames(CStringArray& sBuffer);
	// --- In      : sBuffer, the string array to store returned display names
	// --- Out     :
	// --- Returns :
	// --- Effect  : get all display names from the current iteration

	int GetServiceCount();
	// --- In      :
	// --- Out     :
	// --- Returns : the number of services in the iteration
	// --- Effect  : get number of services in the iteration

	void End();
	// --- In      :
	// --- Out     :
	// --- Returns :
	// --- Effect  : terminate current iteration

protected:
	void GetNamesFromESSArray(LPENUM_SERVICE_STATUS pESSArray,
		DWORD dwServicesReturned);

private:
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __OXITERATORSERVICE_H__

// end of OXIteratorService.h