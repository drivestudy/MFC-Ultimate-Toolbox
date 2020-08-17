// ==========================================================================
// 					Class Implementation : COXEventLog
// ==========================================================================

// Source file : evntlog.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                         
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "evntlog.h"	// class specification
#include "OXIteratorRegistryItem.h"
#include "UTB64Bit.h"


#if defined( _DEBUG )
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC( COXEventLog, CObject );

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// Data members -------------------------------------------------------------
// protected:

// private:
	
// Member functions ---------------------------------------------------------
// public:

COXEventLog::COXEventLog()
	{
	Initialize();
	}

COXEventLog::COXEventLog(LPCTSTR pszSourceName)
	{
	Initialize();
	RegisterSource(pszSourceName);
	}

COXEventLog::~COXEventLog()
	{
	if (m_EventSourceHandle != INVALID_HANDLE_VALUE)
		{
		DeregisterSource();
		}

	if (m_LogHandle != INVALID_HANDLE_VALUE)
		{
		Close();
		}

	Initialize();
	}

void COXEventLog::Initialize()
	{
	ASSERT_VALID( this );

	m_sComputerName.Empty();
	m_sLogName.Empty();

	m_LogHandle                 = INVALID_HANDLE_VALUE;
	m_EventSourceHandle         = INVALID_HANDLE_VALUE;
	m_ErrorCode                 = 0;
	m_NumberOfBytesRead         = 0;
	m_NumberOfBytesInNextRecord = 0;
	}

BOOL COXEventLog::Backup(LPCTSTR pszNameOfBackupFile)
	{
	ASSERT_VALID(this);
	ASSERT(m_LogHandle != INVALID_HANDLE_VALUE);
	ASSERT(pszNameOfBackupFile != NULL);

	if (m_LogHandle == INVALID_HANDLE_VALUE)
		{
		m_ErrorCode = ERROR_INVALID_HANDLE;
		return(FALSE);
		}

	if (pszNameOfBackupFile == NULL)
		{
		m_ErrorCode = ERROR_INVALID_PARAMETER;
		return(FALSE);
		}
	
	BOOL return_value = ::BackupEventLog(m_LogHandle, pszNameOfBackupFile);

	if (return_value != TRUE)
		{
		m_ErrorCode = ::GetLastError();
		}

	return(return_value);
	}

BOOL COXEventLog::Clear(LPCTSTR pszNameOfBackupFile)
	{
	ASSERT_VALID( this );
	ASSERT(m_LogHandle != INVALID_HANDLE_VALUE);

	if (m_LogHandle == INVALID_HANDLE_VALUE)
		{
		m_ErrorCode = ERROR_INVALID_HANDLE;
		return(FALSE);
		}

	/*
	** pszNameOfBackupFile can be NULL
	*/
	BOOL return_value = ::ClearEventLog(m_LogHandle, pszNameOfBackupFile);

	if (return_value != TRUE)
		{
		m_ErrorCode = ::GetLastError();
		}

	return(return_value);
	}

BOOL COXEventLog::Close()
	{
	ASSERT_VALID(this);

	if (m_LogHandle == INVALID_HANDLE_VALUE)
		{
		TRACE0("COXEventLog::In Close : handle Invalid");
		return(FALSE);
		}

	BOOL return_value = ::CloseEventLog(m_LogHandle);

	if (return_value != TRUE)
		{
		m_ErrorCode = ::GetLastError();
		}

	m_LogHandle = INVALID_HANDLE_VALUE;

	return(return_value);
	}

BOOL COXEventLog::CreateApplicationLog(LPCTSTR pszApplicationName,
									   LPCTSTR pszFileContainingMessageTableResource, 
									   DWORD dwSupportedTypes)
{
	ASSERT_VALID(this);
	ASSERT(pszApplicationName != NULL);
	ASSERT(pszFileContainingMessageTableResource != NULL);

	if (pszApplicationName == NULL || 
		lstrlen(pszApplicationName) <= 0 ||
		pszFileContainingMessageTableResource == NULL ||
		lstrlen(pszFileContainingMessageTableResource) <= 0)
	{
		m_ErrorCode = ERROR_INVALID_PARAMETER;
		return(FALSE);
	}

	COXRegistryItem regItem;
	CString log_key_name(_T("\\LocalMachine\\SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\"));
	log_key_name += pszApplicationName;
	log_key_name += _T("\\");
	regItem.SetFullRegistryItem(log_key_name);
	if (regItem.Open() == FALSE)
	{
		m_ErrorCode = regItem.GetLastError();
		return FALSE;
	}

	if (regItem.SetStringValue(pszFileContainingMessageTableResource, 
							   _T("EventMessageFile"),
							   TRUE) == FALSE)
	{
		m_ErrorCode = regItem.GetLastError();
		return FALSE;
	}

	if (regItem.SetNumberValue(dwSupportedTypes, 
							   _T("TypesSupported")) == FALSE)
	{
		m_ErrorCode = regItem.GetLastError();
		return FALSE;
	}

	return TRUE;
}

BOOL COXEventLog::DeleteApplicationLog(LPCTSTR pszApplicationName)
{
	ASSERT_VALID(this);
	ASSERT(pszApplicationName != NULL);

	if (pszApplicationName == NULL || 
		lstrlen(pszApplicationName) <= 0)
	{
		m_ErrorCode = ERROR_INVALID_PARAMETER;
		return(FALSE);
	}

	COXRegistryItem regItem;
	CString log_key_name(_T("\\LocalMachine\\SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\"));
	log_key_name += pszApplicationName;
	log_key_name += _T("\\");
	regItem.SetFullRegistryItem(log_key_name);
	if (regItem.Delete() == FALSE)
	{
		m_ErrorCode = regItem.GetLastError();
		return FALSE;
	}

	/*
	** Microsoft has a bug in this area. Even though we deleted the application from the
	** HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\EventLog\Application\
	** registry area, they don't provide a way to delete the application from the 
	** HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\EventLog\Application\Sources
	** value. The application name is one of the strings in this REG_MULTI_SZ value. We
	** still need to delete it from there. The names listed in this value appear in the 
	** "Source" combobox of the Event Viewer application View->Filter Events... menu selection.
	*/

	log_key_name = _T("\\LocalMachine\\SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\");
	regItem.SetFullRegistryItem(log_key_name);
	CStringArray sources;
	if (regItem.Open(FALSE) == FALSE ||
		regItem.GetMultiStringValue(sources, _T("Sources")) == FALSE)
	{
		m_ErrorCode = regItem.GetLastError();
		return FALSE;
	}
	
	int index = 0;
	int number_of_sources = PtrToInt(sources.GetSize());
	BOOL application_was_found = FALSE;
	while(index < number_of_sources)
	{
		if (sources[index] == pszApplicationName)
		{
			application_was_found = TRUE;
			sources.RemoveAt(index);
			index = number_of_sources;
		}

		index++;
	}
	if (application_was_found != FALSE)
	{
		regItem.SetMultiStringValue(sources, _T("Sources"));
	}

	return TRUE;
}

BOOL COXEventLog::DeregisterSource()
	{
	ASSERT_VALID(this);

	BOOL return_value = TRUE;

	if (m_EventSourceHandle != INVALID_HANDLE_VALUE)
		{
		return_value = ::DeregisterEventSource(m_EventSourceHandle);

		if (return_value != TRUE)
			{
			m_ErrorCode = ::GetLastError();
			}

		m_EventSourceHandle = INVALID_HANDLE_VALUE;
		}

	return(return_value);
	}

#if defined(_DEBUG)

void COXEventLog::Dump(CDumpContext& dump_context) const
	{
	CObject::Dump(dump_context);

	dump_context << _T("m_LogHandle = ")                 << m_LogHandle             << _T("\n");
	dump_context << _T("m_EventSourceHandle = ")         << m_EventSourceHandle         << _T("\n");
	dump_context << _T("m_ErrorCode = ")                 << m_ErrorCode                 << _T("\n");
	dump_context << _T("m_NumberOfBytesRead = ")         << m_NumberOfBytesRead         << _T("\n");
	dump_context << _T("m_NumberOfBytesInNextRecord = ") << m_NumberOfBytesInNextRecord << _T("\n");
	dump_context << _T("m_sComputerName = \"")              << m_sComputerName          << _T("\"\n");
	dump_context << _T("m_sLogName = \"")                   << m_sLogName               << _T("\"\n");
	}

void COXEventLog::AssertValid() const
	{
	CObject::AssertValid();
	}

#endif // _DEBUG

DWORD COXEventLog::GetErrorCode() const
	{
	ASSERT_VALID(this);
	return(m_ErrorCode);
	}

BOOL COXEventLog::GetNumberOfRecords(DWORD& dwNumberOfRecords)
	{
	ASSERT_VALID(this);
	ASSERT(m_LogHandle != INVALID_HANDLE_VALUE);

	if (m_LogHandle == INVALID_HANDLE_VALUE)
		{
		m_ErrorCode = ERROR_INVALID_HANDLE;
		return(FALSE);
		}

	BOOL return_value = ::GetNumberOfEventLogRecords(m_LogHandle, &dwNumberOfRecords);

	if (return_value != TRUE)
		{
		m_ErrorCode = ::GetLastError();
		}

	return(return_value);
	}

BOOL COXEventLog::GetNumberOfBytesInNextRecord(DWORD& dwNumberOfBytesInNextRecord)
	{
	dwNumberOfBytesInNextRecord = m_NumberOfBytesInNextRecord;
	return TRUE;
	}


BOOL COXEventLog::NotifyChange(HANDLE hEventHandle, HANDLE hLogHandle /* = NULL */)
	{
	ASSERT_VALID(this);
	ASSERT(hEventHandle != INVALID_HANDLE_VALUE);

	if (hEventHandle == INVALID_HANDLE_VALUE)
		{
		m_ErrorCode = ERROR_INVALID_PARAMETER;
		return( FALSE );
		}

	if (hLogHandle == INVALID_HANDLE_VALUE || hLogHandle == NULL)
		{
		hLogHandle = m_LogHandle;
		}

	ASSERT(hLogHandle != INVALID_HANDLE_VALUE);
	if (hLogHandle == INVALID_HANDLE_VALUE)
		{
		m_ErrorCode = ERROR_INVALID_HANDLE;
		return(FALSE);
		}

	BOOL return_value = ::NotifyChangeEventLog(hLogHandle, hEventHandle);

	if (return_value != TRUE)
		{
		m_ErrorCode = ::GetLastError();
		}

	return(return_value);
	}

BOOL COXEventLog::Open(LPCTSTR pszLogName, LPCTSTR pszNameOfComputer /* = NULL */)
	{
	ASSERT_VALID(this);
	ASSERT(pszLogName != NULL);

	/*
	** name_of_computer can be NULL
	*/
 
	if (pszLogName == NULL)
		{
		m_ErrorCode = ERROR_INVALID_PARAMETER;
		return(FALSE);
		}

	if (m_LogHandle != INVALID_HANDLE_VALUE && m_LogHandle != NULL)
		{
		// We do not return FALSE here, should this fail, because the old handle could be invalid
		// but that is not our concern anymore
		Close();
		}

	BOOL return_value = TRUE;

	m_LogHandle = ::OpenEventLog(pszNameOfComputer, pszLogName);

	if (m_LogHandle == NULL)
		{
		m_LogHandle  = INVALID_HANDLE_VALUE;
		m_ErrorCode  = ::GetLastError();
		return_value = FALSE;
		}
	else
		{
		if (pszNameOfComputer == NULL)
			{
			TCHAR computer_name[MAX_PATH] = _T("");
			DWORD size = sizeof(computer_name);

			if (::GetComputerName(computer_name, &size) == TRUE)
				{
				m_sComputerName = computer_name;
				}
			else
				{
				m_sComputerName.Empty();
				}
			}
		else
			{
			m_sComputerName = pszNameOfComputer;
			}
		}

	return(return_value);
	}

BOOL COXEventLog::OpenBackup(LPCTSTR pszNameOfBackupFile, LPCTSTR pszNameOfComputer /* = NULL */)
	{
	ASSERT_VALID(this);
	ASSERT(pszNameOfBackupFile != NULL);
	if (m_LogHandle != INVALID_HANDLE_VALUE && m_LogHandle != NULL)
		{
		// We do not return FALSE here, should this fail, because the old handle could be invalid
		// but that is not our concern anymore
		Close();
		}

	/*
	** pszNameOfComputer can be NULL
	*/
	if (pszNameOfBackupFile == NULL)
		{
		m_ErrorCode = ERROR_INVALID_PARAMETER;
		return(FALSE);
		}

	BOOL return_value = TRUE;

	m_LogHandle = ::OpenBackupEventLog(pszNameOfComputer, pszNameOfBackupFile);

	if (m_LogHandle == NULL)
		{
		m_LogHandle = INVALID_HANDLE_VALUE;
		m_ErrorCode = ::GetLastError();
		return_value = FALSE;
		}
	else
		{
		if (pszNameOfComputer == NULL)
			{
			TCHAR computer_name[ MAX_PATH ] = _T("");
			DWORD size = sizeof(computer_name);

			if (::GetComputerName(computer_name, &size) == TRUE)
				{
				m_sComputerName = computer_name;
				}
			else
				{
				m_sComputerName.Empty();
				}
			}
		else
			{
			m_sComputerName = pszNameOfComputer;
			}
		}

	return(return_value);
	}

BOOL COXEventLog::Read(DWORD dwRecordNumber, LPVOID pBuffer, DWORD& dwNumberOfBytesToRead,
	DWORD dwHowToRead /* = EVENTLOG_FORWARDS_READ | EVENTLOG_SEQUENTIAL_READ */)
	{
	ASSERT_VALID(this);
	ASSERT(pBuffer != NULL);
	ASSERT(m_LogHandle != INVALID_HANDLE_VALUE);

	if (m_LogHandle == INVALID_HANDLE_VALUE)
		{
		m_ErrorCode = ERROR_INVALID_HANDLE;
		return(FALSE);
		}

	if (pBuffer == NULL)
		{
		m_ErrorCode = ERROR_INVALID_PARAMETER;
		return( FALSE );
		}

	BOOL return_value = ::ReadEventLog(m_LogHandle,
	                               dwHowToRead,
	                               dwRecordNumber,
	                               pBuffer,
	                               dwNumberOfBytesToRead,
	                              &dwNumberOfBytesToRead,
	                              &m_NumberOfBytesInNextRecord);

	if (return_value != TRUE)
		{
		m_ErrorCode = ::GetLastError();
		}

	return(return_value);
	}

BOOL COXEventLog::RegisterSource(LPCTSTR pszSourceName, LPCTSTR pszNameOfComputer /* = NULL */)
	{
	ASSERT_VALID(this);
	ASSERT(pszSourceName != NULL);

	/*
	** pszNameOfComputer can be NULL
	*/
	if (pszSourceName == NULL)
		{
		m_ErrorCode = ERROR_INVALID_PARAMETER;
		return(FALSE);
		}

	BOOL return_value = TRUE;

	if (m_EventSourceHandle != INVALID_HANDLE_VALUE)
		{
		DeregisterSource();
		}

	m_EventSourceHandle = ::RegisterEventSource(pszNameOfComputer, pszSourceName);

	if (m_EventSourceHandle == NULL)
		{
		TRACE0("RegisterEventSource returned NULL\n");
		m_EventSourceHandle = INVALID_HANDLE_VALUE;
		m_ErrorCode          = ::GetLastError();
		return_value        = FALSE;
		}

	return(return_value);
	}

BOOL COXEventLog::Report(EventType eEventType,
                        DWORD     dwEventIdentifier,
                        WORD      wCategory /* = 0 */,
                        WORD      wNumberOfStrings /* = 0 */,
                        LPCTSTR*  pszStringArray /* = NULL */,
                        DWORD     dwNumberOfRawDataBytes /* = 0 */,
                        LPVOID    pRawDataBuffer /* = NULL */,
                        PSID      pUserSecurityIdentifier /* = NULL */)
	{
	ASSERT_VALID(this);

	BYTE security_identifier_buffer[4096];

	DWORD size_of_security_identifier_buffer = sizeof(security_identifier_buffer);

	if (pUserSecurityIdentifier == NULL)
		{
		TCHAR user_name[256];
		DWORD size_of_user_name  = sizeof(user_name);

		TCHAR domain_name[256];
		DWORD size_of_domain_name = sizeof(domain_name);

		SID_NAME_USE type_of_security_identifier;

		::ZeroMemory(user_name, size_of_user_name);
		::ZeroMemory(domain_name, size_of_domain_name);
		::ZeroMemory(security_identifier_buffer, size_of_security_identifier_buffer);

		::GetUserName(user_name, &size_of_user_name);

		if (::LookupAccountName(NULL,
		                    user_name,
		                   &security_identifier_buffer,
		                   &size_of_security_identifier_buffer,
		                    domain_name,
		                   &size_of_domain_name,
		                   &type_of_security_identifier) == TRUE)
			{
			pUserSecurityIdentifier = security_identifier_buffer;
			}
		}

	BOOL return_value = FALSE;

	if (m_EventSourceHandle != INVALID_HANDLE_VALUE)
		{
		return_value = ::ReportEvent(m_EventSourceHandle,
		                        (WORD)eEventType,
		                        wCategory,
		                        dwEventIdentifier,
		                        pUserSecurityIdentifier,
		                        wNumberOfStrings,
		                        dwNumberOfRawDataBytes,
		                        pszStringArray,
		                        pRawDataBuffer);

		TRACE0("COXEventLog::Report(), Calling ReportEvent() went OK\n");

		if (return_value != TRUE)
			{
			m_ErrorCode = ::GetLastError();
			}
		}
	else
		{
		m_ErrorCode = ERROR_INVALID_HANDLE;
		}

	return(return_value);
	}

BOOL COXEventLog::Report(LPCTSTR pszLogName, DWORD dwMessageStringResourceID,
						 WORD wNumberOfStrings /* = 0 */, LPCTSTR* pszStringArray /* = NULL */)
	{
	ASSERT_VALID(this);
	ASSERT(pszLogName != NULL);

	if (pszLogName == NULL)
		{
		m_ErrorCode = ERROR_INVALID_PARAMETER;
		return(FALSE);
		}

	if (pszStringArray == (LPCTSTR*)NULL)
		{
		m_ErrorCode = ERROR_INVALID_HANDLE;
		return(FALSE);
		}

	if (RegisterSource(pszLogName) == TRUE)
		{
		if (Report(eventError, dwMessageStringResourceID, 0, wNumberOfStrings, pszStringArray) != TRUE)
			{
			DeregisterSource();
			return(FALSE);
			}

		DeregisterSource();
		}
	else
		{
		return(FALSE);
		}

	return(TRUE);
	}

void COXEventLog::ReportError(LPCTSTR pszStringToReport)
	{
	LPCTSTR string_array[1];

	if (pszStringToReport == (LPCTSTR)NULL)
		{
		string_array[0] = _T("COXEventLog::ReportError(NULL)");
		}
	else
		{
		string_array[0] = pszStringToReport;
		}

	TRACE0("COXEventLog::ReportError()\n");
	Report(eventError, 0, 0, 1, string_array);
	}

void COXEventLog::ReportInformation(LPCTSTR pszStringToReport)
	{
	LPCTSTR string_array[1];

	if (pszStringToReport == (LPCTSTR)NULL)
		{
		string_array[0] = _T("COXEventLog::ReportInformation(NULL)");
		}
	else
		{
		string_array[0] = pszStringToReport;
		}

	Report(eventInformation, 0, 0, 1, string_array);
	}
