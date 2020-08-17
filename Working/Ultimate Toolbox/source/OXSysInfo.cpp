// ==========================================================================
// 					Class Implementation : COXSysInfo
// ==========================================================================
//
// Version: 9.3
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900. 
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXSysInfo.h"
#include "OXMainRes.h"

#include <winsock.h>
#include <afxdisp.h>        // MFC OLE automation classes
#include <math.h>

#include <lm.h>
#include <lmerr.h>
#include <lmaccess.h>

#include "UTBStrOp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// Constants used to set unitialized values 
#define UNINIT_BYTE     0x17 
#define UNINIT_DWORD    0x17171717 


IMPLEMENT_DYNAMIC(COXSysInfo, CObject)

// Data members -------------------------------------------------------------
//		None

// private:
	
// Member functions ---------------------------------------------------------
// public:

COXSysInfo::COXSysInfo()
{
	// constructor
}

COXSysInfo::~COXSysInfo()
{
	// destructor
}

BOOL COXSysInfo::GetComputerName(CString *psComputerName) const
{
	//	--- In:			
	//	--- Out:		CString *psComputerName: Name of local computer
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the name of the local computer

	ASSERT(psComputerName != NULL);

	BOOL	bReturn;
	DWORD	dwBufSize;
	TCHAR	buffer[MAX_COMPUTERNAME_LENGTH + 1];

	dwBufSize =  sizeof(buffer)/sizeof(buffer[0]);

	bReturn = ::GetComputerName(buffer, &dwBufSize);

	if (bReturn)
		*psComputerName = buffer;
	else
	{
		TRACE(_T("COXSysInfo::GetComputerName - ::GetComputerName() failed\n"));
	}

	return (bReturn);
}

BOOL COXSysInfo::GetUserName(CString *psUserName) const
{
	//	--- In:			
	//	--- Out:		CString *psUserName: Name of current user
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the name of the current user

	ASSERT(psUserName != NULL);

	BOOL	bReturn;
	DWORD	dwBufSize;
	TCHAR	buffer[UNLEN+1];

	dwBufSize = sizeof(buffer)/sizeof(buffer[0]);

	bReturn = ::GetUserName(buffer, &dwBufSize);

	if (bReturn)
		*psUserName = buffer;
	else
	{
		TRACE(_T("COXSysInfo::GetUserName - ::GetUserName() failed\n"));
	}

	return (bReturn);
}


BOOL COXSysInfo::GetUserAndDomainName(CString *psUserName, CString *psDomainName) const
{
	//	--- In:			
	//	--- Out:		CString *psUserName: Name of current user
	//					CString *psDomainName: Name of current domain logged on to
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the name of the current user and the domain. Works only
	//					on Win NT.

	ASSERT(psUserName != NULL);
	ASSERT(psDomainName != NULL);

	BOOL	bSuccess = FALSE;
	#define BUF_SIZE	512
	DWORD	dwBufSize = BUF_SIZE;
	TCHAR	buffer[BUF_SIZE];

	BOOL bIsNT;
	if (!IsNT(&bIsNT))
		return (FALSE);
	if (bIsNT)
		// We're running on NT.
	{
		HANDLE hToken(0);
		if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, &hToken))
		{
			if (GetLastError() == ERROR_NO_TOKEN)
				// We'll try to open the process token since no thread token exists
			{
				if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
				{
					TRACE(_T("COXSysInfo::GetUserAndDomainName - ::OpenProcessToken() failed\n"));
					return FALSE;
				}
			}
			else
				// Failed to get the thread token
			{
				TRACE(_T("COXSysInfo::GetUserAndDomainName - ::OpenThreadToken() failed\n"));
				return FALSE;
			}
		}

		ASSERT(hToken != NULL);
		bSuccess = GetTokenInformation(hToken, TokenUser, buffer, dwBufSize, &dwBufSize);

		if (!bSuccess)
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
				// Allocate a bigger bufer and try again
			{
				TRACE(_T("COXSysInfo::GetUserAndDomainName - Insuffient buffer() trying again ...\n"));
				CloseHandle(hToken);
				return FALSE;
			}
			else
				// We have an error while getting the token info
			{
				TRACE(_T("COXSysInfo::GetUserAndDomainName - ::GetTokenInformation() failed\n"));
				CloseHandle(hToken);
				return FALSE;
			}
		}

		CloseHandle(hToken);

		SID_NAME_USE	Snu;
		DWORD			dwUserName = BUF_SIZE;
		DWORD			dwDomainName = BUF_SIZE;
		TCHAR			userName[BUF_SIZE];
		TCHAR			domainName[BUF_SIZE];

		bSuccess = LookupAccountSid(NULL, ((PTOKEN_USER)buffer)->User.Sid, userName, &dwUserName, domainName, &dwDomainName, &Snu);
		if (bSuccess)
		{
			*psUserName = userName;
			*psDomainName = domainName;
		}
	}
	else
		// Not Win NT
	{
		TRACE(_T("COXSysInfo::GetUserAndDomainName : NON WIN NT NOT SUPPORTED\n"));
		VERIFY(psUserName->LoadString(IDS_OX_SYSINFOUNKNOWNUSER)); //"<UNKNOWN>"
		VERIFY(psDomainName->LoadString(IDS_OX_SYSINFOUNKNOWNDOMAIN)); //"<UNKNOWN>"
	}

	return (bSuccess);
}

BOOL COXSysInfo::GetPrimaryIPAddress(CString *psIPAddress, 
									 LPCSTR pszHostName/*=NULL*/) const
{
	//	--- In:			LPCSTR pszHostName:		Host name for which primary 
	//											IP address will be retrieved
	//	--- Out:		CString *psIPAddress:	Current IP address (0.0.0.0 
	//											if undefined)
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the current network IP address

	ASSERT(psIPAddress != NULL);

	CStringArray IPArray;
	if(!GetListIPAddresses(&IPArray,TRUE,pszHostName))
		return FALSE;

	*psIPAddress=IPArray[0];
	return (TRUE);
}

BOOL COXSysInfo::GetListIPAddresses(CStringArray* psIPAddressList, 
									BOOL bPrimary /* = FALSE */, 
									LPCSTR pszHostName/*=NULL*/) const
{
	//	--- In:			BOOL bPrimary :		TRUE if only primary IP must be 
	//										retrieved
	//					LPCSTR pszHostName:	Host name for which IP address 
	//										will be retrieved
	//	--- Out:		CStringArray* psIPAddressList: List of IP addresses
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the list of network IP address of a 
	//					multi-homed computer

	ASSERT(psIPAddressList!=NULL);

	CHAR		buffer[255];
	WORD		wVersionRequested;
	WSADATA		wsaData;
	int			err;
  
	HINSTANCE hLib = LoadLibrary(_T("WS2_32.DLL"));
	if (hLib != NULL)
	{
		// Loaded WS2_32.DLL...
		typedef int (WINAPI* WSASTARTUP) (WORD wVersionRequested, LPWSADATA lpWSAData);
		WSASTARTUP dWSAStartup = (WSASTARTUP) GetProcAddress(hLib, "WSAStartup");
		
		typedef int (WINAPI* GETHOSTNAME) (char FAR* name, int namelen);
		GETHOSTNAME dgethostname = (GETHOSTNAME) GetProcAddress(hLib, "gethostname");

		typedef struct hostent FAR* (WINAPI* GETHOSTBYNAME) (const char FAR* name);
		GETHOSTBYNAME dgethostbyname = (GETHOSTBYNAME) GetProcAddress(hLib, "gethostbyname");

		typedef int (WINAPI* WSACLEANUP) ();
		WSACLEANUP dWSACleanup = (WSACLEANUP) GetProcAddress(hLib, "WSACleanup");

		typedef char FAR* (WINAPI* INET_NTOA) (struct in_addr in);
		INET_NTOA dinet_ntoa = (INET_NTOA) GetProcAddress(hLib, "inet_ntoa");

		// Verify all dll entries
		if (dWSAStartup == NULL || dgethostname == NULL ||
			dgethostbyname == NULL || dWSACleanup == NULL || dinet_ntoa == NULL)
		{
			TRACE(_T("COXSysInfo::GetListIPAddresses - GetProcAddress() failed\n"));
			// Free the DLL
			FreeLibrary(hLib);
			return (FALSE);
		}

		wVersionRequested = MAKEWORD(1, 1);
		err = dWSAStartup(wVersionRequested, &wsaData);
		if (err != 0)
		{
			TRACE(_T("COXSysInfo::GetListIPAddresses - WSAStartup() failed\n"));
			// Free the DLL
			FreeLibrary(hLib);
			return (FALSE);
		}

		BOOL bSuccess=TRUE;
		if(pszHostName==NULL)
		{
			bSuccess=(dgethostname(buffer, sizeof(buffer))==0);
			pszHostName=(LPCSTR)buffer;
		}
		if (bSuccess)
		{
			hostent* pHostInfo = dgethostbyname(pszHostName);
			if (pHostInfo != NULL)
			{
				ASSERT(pHostInfo->h_addrtype == PF_INET);
				ASSERT(pHostInfo->h_length == 4);
	
				// Iterate all the address of this host
				// and put them in stringarray
				int i = 0;
				in_addr address;
				while (pHostInfo->h_addr_list[i] != NULL)
				{
					PBYTE pAddress = (PBYTE)pHostInfo->h_addr_list[i];

					// ... Put the seperate bytes in a single long
					address.S_un.S_addr = (u_long)pAddress[0];
					address.S_un.S_addr += ((u_long)pAddress[1]) << 8;
					address.S_un.S_addr += ((u_long)pAddress[2]) << 16;
					address.S_un.S_addr += ((u_long)pAddress[3]) << 24;

					// ... Convert long to string (4 numbers seperated by dots)
					psIPAddressList->Add(CString(dinet_ntoa(address)));

					if (bPrimary)
						break;
				
					i++;
				}

				// Free the Winsock resources
				dWSACleanup();
				// Free the DLL
				FreeLibrary(hLib);
				return TRUE;
			}
		}

		TRACE(_T("COXSysInfo::GetListIPAddresses - gethostname() or gethostbyname() failed\n"));
		// Free the Winsock resources
		dWSACleanup();
		// Free the DLL
		FreeLibrary(hLib);
		return FALSE;
	}

	TRACE(_T("COXSysInfo::GetListIPAddresses - LoadLibrary of WS2_32.DLL failed\n"));
	return FALSE;
}

BOOL COXSysInfo::GetWindowsVersion(DWORD *pdwPlatform, DWORD *pdwMajor, 
								   DWORD *pdwMinor) const
{
	//	--- In:			
	//	--- Out:		DWORD *pdwPlatform: Current Windows platform
	//					DWORD *pdwMajor:	Major OS version
	//					DWORD *pdwMinor:	Minor OS version
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the current Windows OS version

	ASSERT(pdwPlatform!=NULL && pdwMajor!=NULL && pdwMinor!=NULL);

	OSVERSIONINFO	osvi;

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	BOOL bReturn = GetVersionEx(&osvi);

	if (!bReturn)
	{
		TRACE(_T("COXSysInfo::GetWindowsVersion - ::GetVersionEx() failed\n"));
		return (FALSE);
	}

	// Possible platform IDs:
	// VER_PLATFORM_WIN32s	Win32s on Windows 3.1. 
	// VER_PLATFORM_WIN32_WINDOWS (dwMinorVersion is 0) Windows 95.
	// VER_PLATFORM_WIN32_WINDOWS (dwMinorVersion is 1) Windows 98.
	// VER_PLATFORM_WIN32_NT	Windows NT/XP/Vista.

	*pdwPlatform = osvi.dwPlatformId;
	*pdwMajor = osvi.dwMajorVersion;
	*pdwMinor = osvi.dwMinorVersion;

	return (TRUE);
}

BOOL COXSysInfo::GetWindowsBuildNumber(DWORD *pdwBuildNumber) const
{
	//	--- In:			
	//	--- Out:		DWORD *pdwBuildNumber: Windows platform build number
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the current Windows OS version build number

	ASSERT(pdwBuildNumber!=NULL);

	OSVERSIONINFO	osvi;

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	BOOL bReturn = GetVersionEx(&osvi);

	if (!bReturn)
	{
		TRACE(_T("COXSysInfo::GetWindowsBuildNumber - ::GetVersionEx() failed\n"));
		return (FALSE);
	}

	// Possible platform IDs:
	// VER_PLATFORM_WIN32s	Win32s on Windows 3.1. 
	// VER_PLATFORM_WIN32_WINDOWS (dwMinorVersion is 0) Windows 95.
	// VER_PLATFORM_WIN32_WINDOWS (dwMinorVersion is 1) Windows 98.
	// VER_PLATFORM_WIN32_NT	Windows NT.

	if(osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)
		*pdwBuildNumber = osvi.dwBuildNumber;
	else
		*pdwBuildNumber = LOWORD(osvi.dwBuildNumber);

	return (TRUE);
}

BOOL COXSysInfo::GetWindowsPlatformInfo(CString& sPlatformInfo) const
{
	//	--- In:			
	//	--- Out:		CString& sPlatformInfo: additional information about 
	//											the operating system
	//	---	Effect:		Retrieves additional information for current 
	//					Windows OS version

	OSVERSIONINFO	osvi;

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	BOOL bReturn = GetVersionEx(&osvi);

	if (!bReturn)
	{
		TRACE(_T("COXSysInfo::GetWindowsBuildNumber - ::GetVersionEx() failed\n"));
		return (FALSE);
	}

	// Possible platform IDs:
	// VER_PLATFORM_WIN32s	Win32s on Windows 3.1. 
	// VER_PLATFORM_WIN32_WINDOWS (dwMinorVersion is 0) Windows 95.
	// VER_PLATFORM_WIN32_WINDOWS (dwMinorVersion is 1) Windows 98.
	// VER_PLATFORM_WIN32_NT	Windows NT.

	sPlatformInfo=osvi.szCSDVersion;

	return (TRUE);
}

BOOL COXSysInfo::IsNT(BOOL *pbResult) const
{
	//	--- In:			
	//	--- Out:		DWORD *pbResult: TRUE if Windows NT running
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Determines if the current Windows OS 
	//					version is NT 

	ASSERT(pbResult!=NULL);

	OSVERSIONINFO	osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if (!GetVersionEx(&osvi))
	{
		TRACE(_T("COXSysInf::IsNT: GetVersionEx() has failed\n"));
		return (FALSE);
	}
	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
		// We're running on NT.
		*pbResult=TRUE;
	else
		*pbResult=FALSE;

	return (TRUE);
}

BOOL COXSysInfo::IsNTServer(BOOL *pbResult) const
{
	//	--- In:			
	//	--- Out:		DWORD *pbResult: TRUE if Windows NT Server
	//									 installed
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Determines if the current Windows OS 
	//					version is NT Server or not

	ASSERT(pbResult!=NULL);

	BOOL bIsNT;
	if (!IsNT(&bIsNT))
		return (FALSE);
	if (bIsNT)
	{
		HKEY hKey;
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			_T("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"), 
			0, KEY_EXECUTE, &hKey) != ERROR_SUCCESS)
			return (FALSE);

		DWORD dwType, dwSize;
		CString sProductType;
		LPTSTR lpProductType=sProductType.GetBuffer(MAX_PATH+1);
		if(RegQueryValueEx(hKey,_T("ProductType"),NULL,&dwType,
			(LPBYTE)lpProductType,&dwSize)!=ERROR_SUCCESS)
		{
			sProductType.ReleaseBuffer();
			return (FALSE);
		}
		sProductType.ReleaseBuffer();
		ASSERT(dwType==REG_SZ);
		RegCloseKey(hKey);

		if (sProductType==_T("WinNT"))
			*pbResult=FALSE;
		else if (sProductType==_T("LanmanNT"))
			*pbResult=TRUE;
		else if (sProductType==_T("ServerNT"))
			*pbResult=TRUE;
		else 
			return (FALSE);
	}
	else
		*pbResult=FALSE;
	
	return (TRUE);
}

BOOL COXSysInfo::IsOSR2(BOOL *pbResult) const
{
	//	--- In:			
	//	--- Out:		DWORD *pbResult: TRUE if Windows 95 OSR 2 
	//									 installed
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Determines if the current Windows OS 
	//					version is Windows 95 OSR 2 or not

	ASSERT(pbResult!=NULL);

	OSVERSIONINFO osvi = { sizeof(OSVERSIONINFO) };
	BOOL bReturn = GetVersionEx(&osvi);
	if (!bReturn)
	{
		TRACE(_T("COXSysInfo::IsOSR2 - ::GetVersionEx() failed\n"));
		return (FALSE);
	}
	
	WORD wVersion = LOWORD(osvi.dwBuildNumber);
	*pbResult=FALSE;
	if ((osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && (wVersion > 1000))
		// OSR2 (or greater) of Windows 95...
		*pbResult=TRUE;

	return (TRUE);
}

BOOL COXSysInfo::GetWindowsDir(CString *psWinDir) const
{
	//	--- In:			
	//	--- Out:		CString *psWinDir: Windows directory
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the Windows directory

	ASSERT(psWinDir != NULL);

	TCHAR	buffer[_MAX_PATH+1];
	BOOL	bReturn;

	bReturn = GetWindowsDirectory(buffer, sizeof(buffer)/sizeof(buffer[0]));

	if (bReturn)
		*psWinDir = buffer;
	else
	{
		TRACE(_T("COXSysInfo::GetWindowsDir - ::GetWindowsDirectory() failed\n"));
	}

	return (bReturn);
}

BOOL COXSysInfo::GetSystemDir(CString *psSysDir) const
{
	//	--- In:			
	//	--- Out:		CString *psSysDir: System directory
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the System directory

	ASSERT(psSysDir != NULL);

	TCHAR	buffer[MAX_PATH+1];
	BOOL	bReturn;

	bReturn = ::GetSystemDirectory(buffer, sizeof(buffer)/sizeof(buffer[0]));

	if (bReturn)
		*psSysDir = buffer;
	else
		TRACE(_T("COXSysInfo::GetSystemDir - ::GetSystemDirectory() failed\n"));

	return (bReturn);
}

BOOL COXSysInfo::GetTempDir(CString *psTempDir) const
{
	//	--- In:			
	//	--- Out:		CString *psTempDir: Temp directory
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the temp directory

	ASSERT(psTempDir != NULL);

	TCHAR	buffer[_MAX_PATH+1];
	BOOL	bReturn;

	bReturn = ::GetTempPath(sizeof(buffer)/sizeof(buffer[0]), buffer);

	if (bReturn)
		*psTempDir = buffer;
	else
		TRACE(_T("COXSysInfo::GetTempDir - ::GetTempPath() failed\n"));

	return (bReturn);
}


HICON COXSysInfo::GetFileIcon(LPCTSTR pszFileName, BOOL bSmall, 
							  BOOL bSelected, BOOL bFileMustExist) const
{
	SHFILEINFO shfi;

	// FILE_ATTRIBUTE_NORMAL and SHGFI_USEFILEATTRIBUTES allow to retrieve data 
	// without checking if file exists
	if(!::SHGetFileInfo(pszFileName,
		(bFileMustExist ? 0 : FILE_ATTRIBUTE_NORMAL),&shfi,sizeof(SHFILEINFO),
		SHGFI_SHELLICONSIZE|SHGFI_ICON|
		(bFileMustExist ? 0 : SHGFI_USEFILEATTRIBUTES)|
		(bSmall ? SHGFI_SMALLICON : SHGFI_LARGEICON)|
		(bSelected ? SHGFI_SELECTED : 0)))
	{
		return NULL;
	}

	return shfi.hIcon;
}


HICON COXSysInfo::GetFileExtIcon(LPCTSTR pszFileExt, BOOL bSmall, BOOL bSelected) const
{
	CString sFileName;
	sFileName.Format(_T(".%s"),pszFileExt);
	return GetFileIcon(sFileName,bSmall,bSelected,FALSE);
}


BOOL COXSysInfo::GetDriveTypeInfo(int nDrive, CString *psFileSysType, 
								  int *pnDiskType) const
{
	//	--- In:			int nDrive: drive 0-25 (A-Z)
	//	--- Out:		CString *psFileSysType:	File system type
	//					int *pnDiskType: Disk type
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves disk drive information

	ASSERT(psFileSysType != NULL);
	ASSERT(pnDiskType != NULL);

	// Looking for drives 0-25 (A-Z)
	if ((nDrive < 0) || (nDrive >= 26))
	{
		TRACE(_T("COXSysInfo::GetDriveTypeInfo - invalid drive number\n"));
		return (FALSE);
	}

	TCHAR	szRoot[8];
	TCHAR	szVolName[_MAX_PATH+1];
	DWORD	dwVolSer, dwMaxLen, dwFileSysFlags;
	TCHAR	szFileSysName[_MAX_PATH+1];

	UTBStr::stprintf(szRoot, 8, _T("%c:\\"), 65 + nDrive);
	UINT uiDriveType = ::GetDriveType(szRoot);

	if ((uiDriveType == DRIVE_UNKNOWN) || (uiDriveType == DRIVE_NO_ROOT_DIR))
	{
		TRACE(_T("COXSysInfo::GetDriveTypeInfo - ::GetDriveType() failed\n"));
		return (FALSE);
	}

	*pnDiskType = (int) uiDriveType;

	if (GetVolumeInformation(szRoot, szVolName, 
		sizeof(szVolName)/sizeof(szVolName[0]), &dwVolSer, &dwMaxLen, 
		&dwFileSysFlags, szFileSysName, 
		sizeof(szFileSysName)/sizeof(szFileSysName[0])) == FALSE)
	{
		TRACE(_T("COXSysInfo::GetDriveTypeInfo - ::GetVolumeInformation() failed\n"));
		return (FALSE);
	}

	*psFileSysType = szFileSysName;

	return (TRUE);
}

BOOL COXSysInfo::GetDriveVolumeInfo(int nDrive, CString *psVolumeName, 
									DWORD *pdwVolSer, DWORDLONG *pdwTotalSpace, 
									DWORDLONG *pdwFreeSpace) const
{
	//	--- In:			int nDrive: drive 0-25 (A-Z)
	//	--- Out:		CString *psVolumeName: Disk volume name
	//					DWORD *pdwVolSer: Disk serial number
	//					DWORDLONG *pdwTotalSpace: Total space bytes
	//					DWORDLONG *pdwFreeSpace: Total free space bytes
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves disk drive volume information

	ASSERT(psVolumeName != NULL);
	ASSERT(pdwVolSer != NULL);
	ASSERT(pdwTotalSpace != NULL);
	ASSERT(pdwFreeSpace != NULL);

	TCHAR		szRoot[8];
	TCHAR		szVolName[_MAX_PATH+1];
	DWORD		dwVolSer, dwMaxLen, dwFileSysFlags;
	TCHAR		szFileSysName[_MAX_PATH+1];
	BOOL		bRet = TRUE;

	// Looking for drives 0-25 (A-Z)
	if ((nDrive < 0) || (nDrive >= 26))
	{
		TRACE(_T("COXSysInfo::GetDriveVolumeInfo - invalid drive number\n"));
		return (FALSE);
	}

	UTBStr::stprintf(szRoot, 8, _T("%c:\\"), 65 + nDrive);

	dwMaxLen = MAX_PATH;

	if (GetVolumeInformation(szRoot, szVolName, 
		sizeof(szVolName)/sizeof(szVolName[0]), &dwVolSer, &dwMaxLen, 
		&dwFileSysFlags, szFileSysName, 
		sizeof(szFileSysName)/sizeof(szFileSysName[0])) == FALSE)
	{
		TRACE(_T("COXSysInfo::GetDriveVolumeInfo - ::GetVolumeInformation() failed\n"));
		return (FALSE);
	}

	*psVolumeName = szVolName;
	*pdwVolSer = dwVolSer;

	DWORD	dwSectorsPerCluster, dwBytesPerSector;
	DWORD	dwNumberOfFreeClusters, dwTotalNumberOfClusters;

	BOOL bIsOSR2;
	if(IsOSR2(&bIsOSR2) && bIsOSR2)
	{
		// OSR2 (or greater) of Windows 95...

		HINSTANCE hLib = LoadLibrary(_T("KERNEL32.DLL"));

		if (hLib != NULL)
		{
			// Loaded KERNEL32.DLL...
			typedef BOOL (WINAPI *GETDISKFREESPACEEX) (LPCTSTR lpDirectoryName, 
				PULARGE_INTEGER lpFreeBytesAvailableToCaller, 
				PULARGE_INTEGER lpTotalNumberOfBytes, 
				PULARGE_INTEGER lpTotalNumberOfFreeBytes);

			GETDISKFREESPACEEX	dGetDiskFreeSpaceEx = 
				(GETDISKFREESPACEEX) GetProcAddress(hLib, "GetDiskFreeSpaceExA");

			ULARGE_INTEGER		ulFreeBytesAvailable;
			ULARGE_INTEGER		ulTotalBytes;
			ULARGE_INTEGER		ulTotalFree;

			if (dGetDiskFreeSpaceEx != NULL)
			{
				if (dGetDiskFreeSpaceEx(szRoot, &ulFreeBytesAvailable, &ulTotalBytes, 
					&ulTotalFree) > 0)
				{
					// Get total disk space...
					*pdwTotalSpace = (DWORDLONG) ulTotalBytes.QuadPart;

					// Get free disk space...
					*pdwFreeSpace = (DWORDLONG) ulTotalFree.QuadPart;
				}
			}
			else
			{
				if (GetDiskFreeSpace(szRoot, &dwSectorsPerCluster, &dwBytesPerSector, 
					&dwNumberOfFreeClusters, &dwTotalNumberOfClusters) == 0)
					return (FALSE);

				// Get total disk space...
				*pdwTotalSpace = (DWORDLONG)dwSectorsPerCluster * 
					(DWORDLONG)dwBytesPerSector * (DWORDLONG)dwTotalNumberOfClusters;
					
				// Get free disk space...
				*pdwFreeSpace = (DWORDLONG)dwSectorsPerCluster * 
					(DWORDLONG)dwBytesPerSector * (DWORDLONG)dwNumberOfFreeClusters;
			}

			// Free the DLL
			FreeLibrary(hLib);
		}
	}
	else
	{
		if (GetDiskFreeSpace(szRoot, &dwSectorsPerCluster, &dwBytesPerSector, 
			&dwNumberOfFreeClusters, &dwTotalNumberOfClusters) == 0)
		{
			return (FALSE);
		}

		// Get total disk space...
		*pdwTotalSpace = (DWORDLONG)dwSectorsPerCluster * 
			(DWORDLONG)dwBytesPerSector * (DWORDLONG)dwTotalNumberOfClusters;
			
		// Get free disk space...
		*pdwFreeSpace = (DWORDLONG)dwSectorsPerCluster * 
			(DWORDLONG)dwBytesPerSector * (DWORDLONG)dwNumberOfFreeClusters;
	}

	return (bRet);
}

BOOL COXSysInfo::GetDisplayResolution(int *pnxRes, int *pnyRes) const
{
	//	--- In:			
	//	--- Out:		int *xRes: horizontal screen resolution (pixels)
	//					int *yRes: vertical screen resolution (pixels)
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the display resolution size

	ASSERT(pnxRes != NULL);
	ASSERT(pnyRes != NULL);

	int iXRes = DeviceCaps(HORZRES);
	if (iXRes == -1)
	{
		TRACE(_T("COXSysInfo::GetDisplayResolution - invalid X resolution\n"));
		return (FALSE);
	}

	int iYRes = DeviceCaps(VERTRES);
	if (iYRes == -1)
	{
		TRACE(_T("COXSysInfo::GetDisplayResolution - invalid Y resolution\n"));
		return (FALSE);
	}

	*pnxRes = iXRes;
	*pnyRes = iYRes;

	return (TRUE);
}

BOOL COXSysInfo::GetDisplayNumColors(DWORDLONG *pdwNumColors, int *pnNumBits) const
{
	//	--- In:			
	//	--- Out:		DWORDLONG *pdwNumColors: Number of colors used by current display
	//					int *pnNumBits: Number of bits for color depth
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the display color usage

	ASSERT(pdwNumColors != NULL);
	ASSERT(pnNumBits != NULL);

	int		iBitsPixel = DeviceCaps(PLANES) * DeviceCaps(BITSPIXEL);
	int		i;
	*pdwNumColors = 1;

	for(i=0;i<iBitsPixel;i++)
		*pdwNumColors = *pdwNumColors * 2;

	*pnNumBits = iBitsPixel;

	return (TRUE);
}

BOOL COXSysInfo::GetAllDisplayModes(CArrayDisplayMode* parrDisplayMode) const
{
	ASSERT(parrDisplayMode!=NULL);

	parrDisplayMode->RemoveAll();

	// # of DEVMODEs
	int nModeNum=-1;	
	// Information about supported display resolutions, 200 should be enough
	DEVMODE dvmd[1024];		

	// Get all of the possible display modes
	::ZeroMemory((void*)&dvmd, sizeof(dvmd));
	do 
	{
		nModeNum++;
		dvmd[nModeNum].dmSize = sizeof(DEVMODE);
		dvmd[nModeNum].dmDriverExtra = 0;
	} while(::EnumDisplaySettings(NULL, nModeNum, &dvmd[nModeNum]));

	ASSERT(nModeNum>0);

	// Sort all of the display modes:
	// 1: Resolution (width), 2: color depth, 3: frequency.
	::qsort(&dvmd[0],nModeNum,sizeof(DEVMODE),DevModeCompare);

	for(int nIndex=0; nIndex<nModeNum; nIndex++) 
	{
		if(nIndex>0) 
		{
			// If this is not the first DEVMODE entry, possibly add a seperator bar
			if((dvmd[nIndex].dmPelsWidth==dvmd[nIndex-1].dmPelsWidth) &&
				(dvmd[nIndex].dmPelsHeight==dvmd[nIndex-1].dmPelsHeight) &&
				(dvmd[nIndex].dmBitsPerPel==dvmd[nIndex-1].dmBitsPerPel)) 
			{
				// This entry is identical to the previous entry except for refrash rate
				// Let's not add this entry to the menu
				continue;
			}
		}
		
		// We started a new resolution, add it to our array
		DISPLAYMODE displayMode;

		displayMode.dwBitsPerPixel=dvmd[nIndex].dmBitsPerPel;
		displayMode.dwHorzResolution=dvmd[nIndex].dmPelsWidth;
		displayMode.dwVertResolution=dvmd[nIndex].dmPelsHeight;

		displayMode.dwNumColors=1;
		for(DWORD nIndex=0; nIndex<displayMode.dwBitsPerPixel; nIndex++)
			displayMode.dwNumColors*=2;

		parrDisplayMode->Add(displayMode);
	}

	return TRUE;
}

BOOL COXSysInfo::GetDisplayMaxResolution(int *pnxRes, int *pnyRes) const
{
	//	--- In:			
	//	--- Out:		int *xRes: max horizontal screen resolution (pixels)
	//					int *yRes: max vertical screen resolution (pixels)
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the display resolution size

	ASSERT(pnxRes != NULL);
	ASSERT(pnyRes != NULL);

	CArrayDisplayMode arrDisplayMode;
	if(!GetAllDisplayModes(&arrDisplayMode) || arrDisplayMode.GetSize()==0)
		return (FALSE);

	DWORD dwMaxXRes=0;
	DWORD dwMaxYRes=0;
	for(int nIndex=0; nIndex<arrDisplayMode.GetSize(); nIndex++)
	{
		DISPLAYMODE displayMode=arrDisplayMode.GetAt(nIndex);
		if(dwMaxXRes*dwMaxYRes<
			displayMode.dwHorzResolution*displayMode.dwVertResolution)
		{
			dwMaxXRes=displayMode.dwHorzResolution;
			dwMaxYRes=displayMode.dwVertResolution;
		}
	}

	ASSERT(dwMaxXRes*dwMaxYRes>0);

	*pnxRes=dwMaxXRes;
	*pnyRes=dwMaxYRes;

	return (TRUE);
}

BOOL COXSysInfo::GetDisplayMaxNumColors(DWORDLONG *pdwNumColors, int *pnNumBits) const
{
	//	--- In:			
	//	--- Out:		DWORDLONG *pdwNumColors: Max number of colors used 
	//											 by current display
	//					int *pnNumBits: Max number of bits for color depth
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the display color usage

	ASSERT(pdwNumColors != NULL);
	ASSERT(pnNumBits != NULL);

	CArrayDisplayMode arrDisplayMode;
	if(!GetAllDisplayModes(&arrDisplayMode) || arrDisplayMode.GetSize()==0)
		return (FALSE);

	DWORD dwMaxNumBits=0;
	int nIndex=0;
	for(nIndex=0; nIndex<arrDisplayMode.GetSize(); nIndex++)
	{
		DISPLAYMODE displayMode=arrDisplayMode.GetAt(nIndex);
		if(dwMaxNumBits<displayMode.dwBitsPerPixel)
			dwMaxNumBits=displayMode.dwBitsPerPixel;
	}

	ASSERT(dwMaxNumBits>0);

	*pnNumBits=dwMaxNumBits;

	*pdwNumColors = 1;
	for(nIndex=0;nIndex<(int)dwMaxNumBits;nIndex++)
		*pdwNumColors = *pdwNumColors * 2;

	return (TRUE);
}

BOOL COXSysInfo::IsSmallFont(BOOL* pbIsSmall) const
{
	//	--- In:			
	//	--- Out:		BOOL *pbIsSmall:	if TRUE then small fonts are used,
	//										otherwise the large ones
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the flag that specifies whether small or
	//					large fonts are used in the system

	ASSERT(pbIsSmall!=NULL);

	CWnd* pWnd=CWnd::GetDesktopWindow();
	if(pWnd!=NULL)
	{
		CDC* pDC=pWnd->GetWindowDC();
		if(pDC!=NULL)
		{
			TEXTMETRIC tm;
			if(pDC->GetTextMetrics(&tm))
			{
				if(tm.tmHeight>16)
					*pbIsSmall=FALSE;
				else
					*pbIsSmall=TRUE;
				pWnd->ReleaseDC(pDC);
				return TRUE;
			}
			pWnd->ReleaseDC(pDC);
		}
	}
	return FALSE;
}

BOOL COXSysInfo::GetTotalPhysicalMemory(DWORD_PTR *pdwPhysMem)
{
	//	--- In:			
	//	--- Out:		DWORD *pdwPhysMem: Total physical RAM
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the amount of physical memory
	
	ASSERT(pdwPhysMem != NULL);
	
	GetMemStatus() ;
	
	*pdwPhysMem = m_memStatus.dwTotalPhys;
	
	return (TRUE);
}

BOOL COXSysInfo::GetFreePhysicalMemory(DWORD_PTR *pdwFreeMem)
{
	//	--- In:			
	//	--- Out:		DWORD *pdwFreeMem: Total free physical RAM
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the amount of free physical memory
	
	ASSERT(pdwFreeMem != NULL);
	
	GetMemStatus();
	
	*pdwFreeMem = m_memStatus.dwAvailPhys;
	
	return (TRUE);
}

BOOL COXSysInfo::GetTotalPageFile(DWORD_PTR *pdwTotalPageFile)
{
	//	--- In:			
	//	--- Out:		DWORD *pdwTotalPageFile: Total page file size
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the size of the page file
	
	ASSERT(pdwTotalPageFile != NULL);
	
	GetMemStatus();
	
	*pdwTotalPageFile = m_memStatus.dwTotalPageFile;
	
	return (TRUE);
}

BOOL COXSysInfo::GetFreePageFile(DWORD_PTR *pdwFreePageFile)
{
	//	--- In:			
	//	--- Out:		DWORD *pdwFreePageFile: Total free page file size
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the free space of the page file
	
	ASSERT(pdwFreePageFile != NULL);
	
	GetMemStatus();
	
	*pdwFreePageFile = m_memStatus.dwAvailPageFile;
	
	return (TRUE);
}

BOOL COXSysInfo::GetTotalVirtual(DWORD_PTR *pdwTotalVirtual)
{
	//	--- In:			
	//	--- Out:		DWORD *pdwTotalVirtual: Total virtual memory
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the size of virtual memory
	
	ASSERT(pdwTotalVirtual != NULL);
	
	GetMemStatus();
	
	*pdwTotalVirtual = m_memStatus.dwTotalVirtual;
	
	return (TRUE);
}

BOOL COXSysInfo::GetFreeVirtual(DWORD_PTR *pdwFreeVirtual)
{
	//	--- In:			
	//	--- Out:		DWORD *pdwFreeVirtual: Total free virtual memory
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the size of the free virtual memory
	
	ASSERT(pdwFreeVirtual != NULL);
	
	GetMemStatus();
	
	*pdwFreeVirtual = m_memStatus.dwAvailVirtual;
	
	return (TRUE);
}

BOOL COXSysInfo::GetNumProcessors(int *pnNumProcessors)
{
	//	--- In:			
	//	--- Out:		int *pnNumProcessors: Total number of processors in machine
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the number of CPUs
	
	ASSERT(pnNumProcessors != NULL);
	
	GetSysInfo();
	
	*pnNumProcessors = (int) m_sysInfo.dwNumberOfProcessors;
	
	return (TRUE);
}

BOOL COXSysInfo::GetProcessorType(DWORD *pdwProcessorType)
{
	//	--- In:			
	//	--- Out:		DWORD *pdwProcessorType: Processor type
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the processor type (primary processor)
	
	ASSERT(pdwProcessorType != NULL);
	
	GetSysInfo();
	
	BOOL bIsNT;
	if (!IsNT(&bIsNT))
		return (FALSE);

	*pdwProcessorType = (DWORD)PROCESSOR_UNKNOWN;
	
	// VER_PLATFORM_WIN32s	Win32s on Windows 3.1. 
	// VER_PLATFORM_WIN32_WINDOWS	Win32 on Windows 95.
	// VER_PLATFORM_WIN32_NT	Win32 on Windows NT.
	
	if (bIsNT)
	{
		// Running NT - so step through processor
		// architecture and type to determine
		// the actual CPU
		
		// Now determine processor architecture
		switch(m_sysInfo.wProcessorArchitecture)
		{
			case PROCESSOR_ARCHITECTURE_INTEL:
				switch(m_sysInfo.wProcessorLevel)
				{
				case 3:
					*pdwProcessorType = PROCESSOR_INTEL_386;
					break;
						
				case 4:
					*pdwProcessorType = PROCESSOR_INTEL_486;
					break;
						
				case 5:
					*pdwProcessorType = PROCESSOR_INTEL_PENTIUM;
					break;
				}
				break;
				
			case PROCESSOR_ARCHITECTURE_MIPS:
				*pdwProcessorType = PROCESSOR_MIPSR4000;
				break;
						
			case PROCESSOR_ARCHITECTURE_ALPHA:
				*pdwProcessorType = m_sysInfo.wProcessorLevel;
				break;
						
			case PROCESSOR_ARCHITECTURE_PPC:
				switch(m_sysInfo.wProcessorLevel)
				{
				case 1:
					*pdwProcessorType = PROCESSOR_PPC601;
					break;
								
				case 3:
					*pdwProcessorType = PROCESSOR_PPC603;
					break;
								
				case 4:
					*pdwProcessorType = PROCESSOR_PPC604;
					break;
								
				case 6:
					*pdwProcessorType = PROCESSOR_PPC603PLUS;
					break;
								
				case 9:
					*pdwProcessorType = PROCESSOR_PPC604PLUS;
					break;
								
				case 20:
					*pdwProcessorType = PROCESSOR_PPC620;
					break;
				}
				break;
						
			case PROCESSOR_ARCHITECTURE_UNKNOWN:
				break;
		}
	}
	else
	{
		// Running Win95
		
		// PROCESSOR_INTEL_386
		// PROCESSOR_INTEL_486
		// PROCESSOR_INTEL_PENTIUM
		
		*pdwProcessorType = m_sysInfo.dwProcessorType;
	}

	if(*pdwProcessorType==PROCESSOR_INTEL_PENTIUM || 
		*pdwProcessorType == (DWORD)PROCESSOR_UNKNOWN)
	{
		WORD nCPUType=wincpuid();
		ASSERT(nCPUType>=5);
		switch(nCPUType)
		{
		// Pentium
		case 5:
			{
			break;
			}
		// Pentium Pro, Pentium II, Pentium Celeron, Pentium III
		case 6:
			{
				// retrieve CPUID
				WORD nCPUidEx=wincpuidext();
				// analyze the model number
				int nModel=(nCPUidEx&0x00f0)>>4;
				switch(nModel)
				{
				case 1:
					*pdwProcessorType=(DWORD)PROCESSOR_INTEL_PENTIUMPRO;
					break;
				case 2:
				case 3:
				case 5:
					*pdwProcessorType=(DWORD)PROCESSOR_INTEL_PENTIUM2;
					break;
				case 6:
					*pdwProcessorType=(DWORD)PROCESSOR_INTEL_PENTIUMCELERON;
					break;
				case 7:
				case 8:
					*pdwProcessorType=(DWORD)PROCESSOR_INTEL_PENTIUM3;
					break;
				default:
					*pdwProcessorType=(DWORD)PROCESSOR_UNKNOWN;
					break;
				}

				break;
			}
		default:
			{
			*pdwProcessorType=(DWORD)PROCESSOR_UNKNOWN;
			break;
			}
		}
	}
	
	return (TRUE);
}

BOOL COXSysInfo::IsCoProcessorPresent(BOOL* pbResult)
{ 
	ASSERT(pbResult!=NULL);

	BOOL bIsNT;
	if(!IsNT(&bIsNT))
		return (FALSE);

	DWORD dwProcessorType;
	GetProcessorType(&dwProcessorType);

	// we return TRUE if we're not running on x86
	// other CPUs have built in floating-point, with no registry entry

	if((dwProcessorType != PROCESSOR_INTEL_386) &&
		(dwProcessorType != PROCESSOR_INTEL_486) &&
		(dwProcessorType != PROCESSOR_INTEL_PENTIUM))
	{
		*pbResult=TRUE;
	}
	else
	{
		if(!bIsNT)
		{
			DWORD dwFeatures=wincpufeatures();
		
			if(dwFeatures & 0x00000001) //then CPU contains a floating-point unit (FPU)
				*pbResult=TRUE;
			else
				*pbResult=FALSE;
		}
		else
		{
			HKEY hKey;
			if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				_T("HARDWARE\\DESCRIPTION\\System\\FloatingPointProcessor"), 
				0, KEY_EXECUTE, &hKey) != ERROR_SUCCESS)
			{
				// GetLastError() will indicate ERROR_RESOURCE_DATA_NOT_FOUND
				// if we can't find the key.  This indicates no coprocessor present
				*pbResult=FALSE;
			}
			RegCloseKey(hKey);
		}
	}

	return TRUE;
} 

BOOL COXSysInfo::GetProcessorSpeed(DWORD* pdwProcessorSpeed, int nIndex/*=0*/) const
{ 
	ASSERT(pdwProcessorSpeed!=NULL);

	BOOL bIsNT;
	if (!IsNT(&bIsNT))
		return (FALSE);
	if (!bIsNT && nIndex>0)
	{
		TRACE(_T("COXSysInfo::GetProcessorSpeed: Windows 95 doesn't support multiprocessor systems\n"));
		return (FALSE);
	}

	DWORD dwSpeed=0;
	if(nIndex>0 || bIsNT)
	{
		ASSERT(bIsNT);
		// Running NT
	
		CString sRegistryPath;
		sRegistryPath.Format(_T("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d"),nIndex);

		HKEY hKey;
		if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,sRegistryPath,0,KEY_READ,&hKey)!=ERROR_SUCCESS)
			return (FALSE);

		DWORD dwType, dwValue, dwSize;
		if(RegQueryValueEx(hKey,_T("~MHz"),NULL,&dwType,(LPBYTE)&dwValue,&dwSize)!=
			ERROR_SUCCESS)
			return (FALSE);

		ASSERT(dwType==REG_DWORD && dwSize==sizeof(DWORD));

		dwSpeed=dwValue;

		RegCloseKey(hKey);
	}
	else
	{
		struct FREQ_INFO CPUSpeed=cpuspeed(0);
		if(CPUSpeed.in_cycles==0 && CPUSpeed.ex_ticks==0) 
		{
			TRACE(_T("This processor cannot be accurately timed with this function.\n The processor is either an Intel Clone or is below 80386 level."));
			return (FALSE);
		}

		dwSpeed=CPUSpeed.norm_freq;
	}

	enum CPUSpeed arrCPUSpeed[35];
	arrCPUSpeed[0]=CPUSPEED_16;
	arrCPUSpeed[1]=CPUSPEED_20;
	arrCPUSpeed[2]=CPUSPEED_25;
	arrCPUSpeed[3]=CPUSPEED_33;
	arrCPUSpeed[4]=CPUSPEED_40;
	arrCPUSpeed[5]=CPUSPEED_50;
	arrCPUSpeed[6]=CPUSPEED_60;
	arrCPUSpeed[7]=CPUSPEED_66;
	arrCPUSpeed[8]=CPUSPEED_75;
	arrCPUSpeed[9]=CPUSPEED_80;
	arrCPUSpeed[10]=CPUSPEED_90;
	arrCPUSpeed[11]=CPUSPEED_100;
	arrCPUSpeed[12]=CPUSPEED_120;
	arrCPUSpeed[13]=CPUSPEED_133;
	arrCPUSpeed[14]=CPUSPEED_150;
	arrCPUSpeed[15]=CPUSPEED_166;
	arrCPUSpeed[16]=CPUSPEED_180;
	arrCPUSpeed[17]=CPUSPEED_200;
	arrCPUSpeed[18]=CPUSPEED_233;
	arrCPUSpeed[19]=CPUSPEED_266;
	arrCPUSpeed[20]=CPUSPEED_300;
	arrCPUSpeed[21]=CPUSPEED_333;
	arrCPUSpeed[22]=CPUSPEED_350;
	arrCPUSpeed[23]=CPUSPEED_400;
	arrCPUSpeed[24]=CPUSPEED_450;
	arrCPUSpeed[25]=CPUSPEED_500;
	arrCPUSpeed[26]=CPUSPEED_533;
	arrCPUSpeed[27]=CPUSPEED_550;
	arrCPUSpeed[28]=CPUSPEED_600;
	arrCPUSpeed[29]=CPUSPEED_650;
	arrCPUSpeed[30]=CPUSPEED_667;
	arrCPUSpeed[31]=CPUSPEED_700;
	arrCPUSpeed[32]=CPUSPEED_733;
	arrCPUSpeed[33]=CPUSPEED_750;
	arrCPUSpeed[34]=CPUSPEED_800;

	*pdwProcessorSpeed=0;
	UINT nMargin=0xffff;
	UINT nNewMargin;
	for(int nIterator=0; nIterator<countof(arrCPUSpeed); nIterator++)
	{
		nNewMargin=abs((int) (dwSpeed-arrCPUSpeed[nIterator]));
		if(nNewMargin<nMargin)
		{
			nMargin=nNewMargin;
			*pdwProcessorSpeed=arrCPUSpeed[nIterator];
		}
	}

	if(*pdwProcessorSpeed==0)
		return (FALSE);
	else
		return (TRUE);
} 

BOOL COXSysInfo::GetTimeStamp(DWORD *pdwHigh, DWORD *pdwLow) const
{
	ASSERT(pdwHigh!=NULL);
	ASSERT(pdwLow!=NULL);

	TIME_STAMP stamp=winrdtsc();
	
	if(stamp.High!=0 || stamp.Low!=0) 
	{
		*pdwHigh=stamp.High;
		*pdwLow=stamp.Low;
		return TRUE;
	}
	else
	{
		TRACE(_T("The primary processor doesn't support time stamp functionality"));
		return FALSE;
	}
}

BOOL COXSysInfo::IsMMXProcessor(BOOL* pbMMX)
{ 
	ASSERT(pbMMX!=NULL);

	DWORD dwProcessorType;
	GetProcessorType(&dwProcessorType);

	// relevant only to x86 processors

	if((dwProcessorType != PROCESSOR_INTEL_386) &&
		(dwProcessorType != PROCESSOR_INTEL_486) &&
		(dwProcessorType != PROCESSOR_INTEL_PENTIUM) &&
		(dwProcessorType != PROCESSOR_INTEL_PENTIUM2) &&
		(dwProcessorType != PROCESSOR_INTEL_PENTIUMPRO) &&
		(dwProcessorType != PROCESSOR_INTEL_PENTIUMCELERON) &&
		(dwProcessorType != PROCESSOR_INTEL_PENTIUM3))
	{
		*pbMMX=FALSE;
	}
	else
	{
		DWORD dwFeatures=wincpufeatures();
		
		if(dwFeatures & 0x00800000) //then MMX(TM) technology
			*pbMMX=TRUE;
		else
			*pbMMX=FALSE;
	}

	return TRUE;
} 

BOOL COXSysInfo::IsOverdriveProcessor(BOOL* pbOverdrive)
{ 
	ASSERT(pbOverdrive!=NULL);

	DWORD dwProcessorType;
	GetProcessorType(&dwProcessorType);

	// relevant only to x86 processors

	if((dwProcessorType != PROCESSOR_INTEL_386) &&
		(dwProcessorType != PROCESSOR_INTEL_486) &&
		(dwProcessorType != PROCESSOR_INTEL_PENTIUM) &&
		(dwProcessorType != PROCESSOR_INTEL_PENTIUM2) &&
		(dwProcessorType != PROCESSOR_INTEL_PENTIUMPRO) &&
		(dwProcessorType != PROCESSOR_INTEL_PENTIUMCELERON) &&
		(dwProcessorType != PROCESSOR_INTEL_PENTIUM3))
	{
		*pbOverdrive=FALSE;
	}
	else
	{
		// retrieve CPUID
		WORD nCPUidEx=wincpuidext();

		// get processor type
		if(((nCPUidEx&0x3000)>>12)==1)	// if it equals 1 then it is Overdrive processor
			*pbOverdrive=TRUE;
		else
			*pbOverdrive=FALSE;
	}

	return TRUE;
} 



BOOL COXSysInfo::GetModemInfo(CArrayModemInfo* parrModemInfo) const
{
	ASSERT(parrModemInfo!=NULL);

	parrModemInfo->RemoveAll();

	BOOL bIsNT;
	if (!IsNT(&bIsNT))
		return (FALSE);
	
	CString sRegistryPath;
	HKEY hKey=NULL;
	HKEY hSubKey=NULL;
	BOOL bSuccess=FALSE;

	if(bIsNT)
	{
		// Running NT
	
		sRegistryPath.Format(_T("System\\CurrentControlSet\\Control\\Class"));

		if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,sRegistryPath,0,KEY_READ,&hKey)!=
			ERROR_SUCCESS)
		{
			TRACE(_T("COXSysInfo::GetNumModems: failed to open registry key where modem information should be located"));
			return (FALSE);
		}

		// enumerate all subkeys

		DWORD dwIndex=0;
		CString sSubKeyPath;
		LPTSTR lpName=sSubKeyPath.GetBuffer(MAX_PATH+1);
		while(::RegEnumKey(hKey,dwIndex,lpName,MAX_PATH+1)==ERROR_SUCCESS)
		{
			if(::RegOpenKeyEx(hKey,sSubKeyPath,0,KEY_READ,&hSubKey)==ERROR_SUCCESS)
			{
				DWORD dwType, dwSize;
				LPTSTR lpszValue=NULL;
				if(::RegQueryValueEx(hSubKey,_T("Class"),NULL,&dwType,(LPBYTE)lpszValue,
					&dwSize)==ERROR_SUCCESS)
				{
					ASSERT(dwType==REG_SZ);

					CString sClassName;
					lpszValue=sClassName.GetBuffer(dwSize/sizeof(TCHAR));
					VERIFY(::RegQueryValueEx(hSubKey,_T("Class"),NULL,&dwType,
						(LPBYTE)lpszValue,&dwSize)==ERROR_SUCCESS);
					sClassName.ReleaseBuffer();
					if(sClassName==_T("Modem"))
						bSuccess=TRUE;
				}

				if(bSuccess)
					break;
				else
				{
					RegCloseKey(hSubKey);
					hSubKey=NULL;
				}
			}

			dwIndex++;
		}
		sSubKeyPath.ReleaseBuffer();

		RegCloseKey(hKey);
	}
	else
	{
		// Running Windows 95/98
	
		sRegistryPath.Format(_T("System\\CurrentControlSet\\Services\\Class\\Modem"));

		if(::RegOpenKeyEx(HKEY_LOCAL_MACHINE,sRegistryPath,0,KEY_READ,&hSubKey)!=
			ERROR_SUCCESS)
		{
			TRACE(_T("COXSysInfo::GetNumModems: failed to open registry key where modem information should be located"));
			return (FALSE);
		}

		// enumerate all subkeys

		bSuccess=TRUE;
	}

	if(bSuccess)
	{
		ASSERT(hSubKey!=NULL);

		DWORD dwModemIndex=0;
		CString sModemSubKeyPath;
		LPTSTR lpModemName=sModemSubKeyPath.GetBuffer(_MAX_PATH+1);

		while(::RegEnumKey(hSubKey,dwModemIndex,lpModemName,
			(_MAX_PATH+1)*sizeof(TCHAR))==ERROR_SUCCESS)
		{
			HKEY hModemSubKey=NULL;
			if(::RegOpenKeyEx(hSubKey,sModemSubKeyPath,0,
				KEY_QUERY_VALUE,&hModemSubKey)==ERROR_SUCCESS)
			{
				// enumerate all values

				DWORD dwIndex=0;
				DWORD dwType, dwSize;
				CString sValueName;
				LPTSTR lpValueName=sValueName.GetBuffer(_MAX_PATH+1);
				DWORD dwValueNameLength=(_MAX_PATH+1)*sizeof(TCHAR);
				LPBYTE lpData=NULL;

				MODEMINFO modemInfo;
				BOOL bModemFound=FALSE;
		
				while(::RegEnumValue(hModemSubKey,dwIndex,lpValueName,&dwValueNameLength,
					NULL,&dwType,lpData,&dwSize)==ERROR_SUCCESS)
				{
					sValueName.ReleaseBuffer();

					if(sValueName==_T("DeviceType"))
					{
						ASSERT(dwType==REG_BINARY && dwSize==1);

						lpValueName=sValueName.GetBuffer(_MAX_PATH+1);
						dwValueNameLength=(_MAX_PATH+1)*sizeof(TCHAR);

						BYTE deviceType;
						VERIFY(::RegEnumValue(hModemSubKey,dwIndex,lpValueName,
							&dwValueNameLength,NULL,&dwType,&deviceType,&dwSize)==
							ERROR_SUCCESS);

						if(deviceType==2)
							bModemFound=TRUE;
					}
					else if(sValueName==_T("DriverDesc"))
					{
						ASSERT(dwType==REG_SZ);

						lpValueName=sValueName.GetBuffer(_MAX_PATH+1);
						dwValueNameLength=(_MAX_PATH+1)*sizeof(TCHAR);

						LPTSTR lpszDriverDesc=modemInfo.sDriverDesc.
							GetBuffer(dwSize/sizeof(TCHAR));
						VERIFY(::RegEnumValue(hModemSubKey,dwIndex,lpValueName,
							&dwValueNameLength,NULL,&dwType,(LPBYTE)lpszDriverDesc,
							&dwSize)==ERROR_SUCCESS);
						modemInfo.sDriverDesc.ReleaseBuffer();
					}
					else if(sValueName==_T("Manufacturer"))
					{
						ASSERT(dwType==REG_SZ);

						lpValueName=sValueName.GetBuffer(_MAX_PATH+1);
						dwValueNameLength=(_MAX_PATH+1)*sizeof(TCHAR);

						LPTSTR lpszManufacturer=modemInfo.sManufacturer.
							GetBuffer(dwSize/sizeof(TCHAR));
						VERIFY(::RegEnumValue(hModemSubKey,dwIndex,lpValueName,
							&dwValueNameLength,NULL,&dwType,(LPBYTE)lpszManufacturer,
							&dwSize)==ERROR_SUCCESS);
						modemInfo.sManufacturer.ReleaseBuffer();
					}
					else if(sValueName==_T("Model"))
					{
						ASSERT(dwType==REG_SZ);

						lpValueName=sValueName.GetBuffer(_MAX_PATH+1);
						dwValueNameLength=(_MAX_PATH+1)*sizeof(TCHAR);

						LPTSTR lpszModel=modemInfo.sModel.
							GetBuffer(dwSize/sizeof(TCHAR));
						VERIFY(::RegEnumValue(hModemSubKey,dwIndex,lpValueName,
							&dwValueNameLength,NULL,&dwType,(LPBYTE)lpszModel,
							&dwSize)==ERROR_SUCCESS);
						modemInfo.sModel.ReleaseBuffer();
					}
					else if(sValueName==_T("PortSubClass"))
					{
						ASSERT(dwType==REG_BINARY && dwSize==1);

						lpValueName=sValueName.GetBuffer(_MAX_PATH+1);
						dwValueNameLength=(_MAX_PATH+1)*sizeof(TCHAR);

						BYTE port;
						VERIFY(::RegEnumValue(hModemSubKey,dwIndex,lpValueName,
							&dwValueNameLength,NULL,&dwType,&port,&dwSize)==
							ERROR_SUCCESS);

						modemInfo.nPort=port;
					}

					lpValueName=sValueName.GetBuffer(_MAX_PATH+1);
					dwValueNameLength=(_MAX_PATH+1)*sizeof(TCHAR);

					dwIndex++;
				}
				sValueName.ReleaseBuffer();
				RegCloseKey(hModemSubKey);

				if(bModemFound)
					parrModemInfo->Add(modemInfo);
			}
			dwModemIndex++;
		}

		RegCloseKey(hSubKey);
	}

	return TRUE;
}

BOOL COXSysInfo::GetNumModems(int *pnNumModems) const
{
	ASSERT(pnNumModems!=NULL);
		
	CArrayModemInfo arrModemInfo;

	if(!GetModemInfo(&arrModemInfo))
		return FALSE;
	else
	{
		*pnNumModems= PtrToInt(arrModemInfo.GetSize());
		return TRUE;
	}
}

BOOL COXSysInfo::GetNICAddress(LPNICADDRESS pNICAddress) const
{
	//	--- In:			
	//	--- Out:		LPNICADDRESS pNICAddress:	valid pointer to Network
	//												Interface Card (NIC) address
	//												structure
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the NIC address
	//	---	Comment:	This function bases its functionality on the fact that
	//					the MAC address is part of the GUID generated on given
	//					machine.  It also recognises the fact that on Windows 2000
	//					and later CoCreateGuid will return GUID, which contains
	//					encrypted MAC address and therefore cannot be traced back.
	//					On these platforms UuidCreateSequential has to be used
	//					to obtain the version of GUID that we expect..

	ASSERT(pNICAddress!=NULL);

	int nIndex;
	GUID guid;
	// create signature for the UuidCreateSequential function
	typedef int (WINAPI* UuidCreateSequential) (UUID *Uuid);
	UuidCreateSequential dUuidCreateSequential = NULL;
	// attempt to open the rpcrt4.DLL
	HINSTANCE hLib = NULL;
	hLib = LoadLibrary(_T("rpcrt4.DLL"));

	if (hLib != NULL)
	{	// Loaded rpcrt4.DLL, if possible get pointer to the UuidCreateSequential method
		dUuidCreateSequential = (UuidCreateSequential) GetProcAddress(hLib, "UuidCreateSequential");

		if ( dUuidCreateSequential == NULL )
		{	// if the UuidCreateSequential method in the rpcrt4.DLL is not found then use
			// the traditional way to retrieve the GUID
			if( ::CoCreateGuid( &guid ) != S_OK )
				return FALSE;
		}
		else if( dUuidCreateSequential( &guid ) != RPC_S_OK )
			return FALSE;
	}
	else if(::CoCreateGuid(&guid)!=S_OK)
		return FALSE;

	for(nIndex=0; nIndex<countof(pNICAddress->data); nIndex++)
		pNICAddress->data[nIndex]=guid.Data4[nIndex+2];

	for(nIndex=0; nIndex<countof(pNICAddress->data); nIndex++)
		if(pNICAddress->data[nIndex]!=0)
			return TRUE;

	return FALSE;
}


BOOL COXSysInfo::GetFreeSystemResources(int* pnFreeResources, 
										enum RESOURCETYPE resourceType) const
{
	ASSERT(pnFreeResources!=NULL);

	BOOL bIsNT;
	if (!IsNT(&bIsNT))
		return (FALSE);
	if (bIsNT)
		// We're running on NT.
		return (FALSE);

	HINSTANCE hLib = LoadLibrary(_T("RSRC32.dll"));
	if (hLib != NULL)
	{
		typedef LONG (WINAPI* GETFREESYSTEMRESOURCES) (int);
		GETFREESYSTEMRESOURCES pfnGetFreeSystemResources = 
			(GETFREESYSTEMRESOURCES) GetProcAddress(hLib, "_MyGetFreeSystemResources32@4");
		ASSERT(pfnGetFreeSystemResources!=NULL);

		*pnFreeResources=pfnGetFreeSystemResources((int)resourceType);

		FreeLibrary(hLib);
		
		return (TRUE);
	}
	else
		return (FALSE);

}

// printers info
//

BOOL COXSysInfo::GetDefaultPrinterName(CString& sPrinterName) const
{ 
	//	--- In:			
	//	--- Out:		sPrinterName:	reference to string to which the
	//									name of the default printer will be 
	//									copied. If it is empty then there is
	//									default printers in the system
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the name of the default printer

	// Local variables 
	BOOL bIsNT;
	if (!IsNT(&bIsNT))
		return (FALSE);
	
	sPrinterName.Empty();

	if (!bIsNT)
	{  
		LPTSTR lpszPrinterName=GetRegistryString(HKEY_CURRENT_CONFIG, 
			_T("SYSTEM\\CurrentControlSet\\Control\\Print\\Printers"), 
			_T("Default"));
		if(lpszPrinterName!=NULL)
		{
			sPrinterName=lpszPrinterName;
			::GlobalFree(lpszPrinterName);
		}
	} 
    else  
	{
		TCHAR szTemp[MAX_PATH]; 
        LPTSTR lpszTemp;          // Get Default printer name. 
        GetProfileString(_T("windows"),_T("device"),_T(""), 
			szTemp, sizeof(szTemp));  
        if(lstrlen(szTemp)!=0)     
		{ 
			// Terminate at first comma, just want printer name. 
		    lpszTemp=_tcschr(szTemp,',');
			if(lpszTemp!=NULL) 
				*lpszTemp = '\x0';         
			sPrinterName=(LPTSTR)szTemp;     
		}
	} 
    return (TRUE); 
}

BOOL COXSysInfo::GetAllPrintersName(CStringArray& arrPrinterName) const
{
	//	--- In:			
	//	--- Out:		sPrinterName:	reference to string array to which the
	//									name of found printers will be copied. 
	//									If it is empty then there is no 
	//									printers found
	//									
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the name of all printers in the system.

	BOOL bIsNT;
	if (!IsNT(&bIsNT))
		return (FALSE);
	
	arrPrinterName.RemoveAll();

	BOOL bReturnCode;
	DWORD dwSize, dwPrinters, dwNeeded;
	DWORD dwType=PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS;
	DWORD dwLevel=(bIsNT) ? 4 : 5;
	bReturnCode=::EnumPrinters(dwType,NULL,dwLevel,NULL,0,&dwSize,&dwPrinters); 

	// If Return Code is TRUE, there is nothing to enumerate. 
    if (bReturnCode)
		return (TRUE);

    // If LastError is any thing other than allocate size error, flag and exit. 
    DWORD dwErrorCode=::GetLastError(); 
    if (dwErrorCode!=ERROR_INSUFFICIENT_BUFFER)     
        return (FALSE);

	// Loop until we have size right. 
	BOOL bRightSize=FALSE;
	LPBYTE lpInfo=NULL;
    while(!bRightSize)     
	{ 
		if(NULL!=(lpInfo=(LPBYTE)GlobalAlloc(GPTR,dwSize)))
		{ 
#ifdef _DEBUG
			::memset(lpInfo,UNINIT_BYTE,dwSize); 
#endif 
            // Enumerate 
            bRightSize=EnumPrinters(dwType,NULL,dwLevel,lpInfo,dwSize,&dwNeeded,&dwPrinters); 
            if(!bRightSize)             
			{ 
				dwErrorCode=GetLastError();  
                // If anything other than allocate size error, flag and exit. 
                if (dwErrorCode != ERROR_INSUFFICIENT_BUFFER)                 
				{ 
					::GlobalFree(lpInfo); 
			        return (FALSE);
				}                 
				else 
                {
					::GlobalFree(lpInfo); 
                    lpInfo=NULL;                     
					dwSize=dwNeeded; 
                }   
			} 
            else  // EnumPrinters returned success        
			{ 
				if(bIsNT)
				{
					// Save pointer to PRINTER_INFO structure 
					LPPRINTER_INFO_4 lpPrinterInfo=(LPPRINTER_INFO_4)lpInfo;  
					for(DWORD dwIndex=0; dwIndex<dwPrinters; dwIndex++) 
						arrPrinterName.
							Add((LPTSTR)lpPrinterInfo[dwIndex].pPrinterName);
				}
				else
				{
					// Save pointer to PRINTER_INFO structure 
					LPPRINTER_INFO_5 lpPrinterInfo=(LPPRINTER_INFO_5)lpInfo;  
					for(DWORD dwIndex=0; dwIndex<dwPrinters; dwIndex++) 
						arrPrinterName.
							Add((LPTSTR)lpPrinterInfo[dwIndex].pPrinterName);
				}
				::GlobalFree(lpInfo); 
            }   
		}    
		else 
	        return (FALSE);
    }    

	return(TRUE); 
}


BOOL COXSysInfo::GetPrinterInfo(const CString& sPrinterName, 
								LPPRINTER_INFO_2& lpPrinterInfo) const
{
	//	--- In:			sPrinterName:	reference to string which is the
	//									name of installed printer.
	//	--- Out:		lpPrinterInfo:	reference to the pointer to 
	//									PRINTER_INFO_2 structure 
	//									that will be populated with data as
	//									result of execution of this function.
	//									You shouldn't allocate any memory.
	//									This function will allocate all 
	//									necessary memory. The caller will 
	//									be responsible for freeing that memory.
	//									Use GlobalFree() function in order to 
	//									do that
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the printer info for specified printer
	//					installed inthe system

	ASSERT(!sPrinterName.IsEmpty());

	HANDLE hPrinter=NULL;
	if(!::OpenPrinter((LPTSTR)(LPCTSTR)sPrinterName,&hPrinter,NULL) || 
		hPrinter==NULL)
	{
		TRACE(_T("COXSysInfo::GetPrinterInfo: OpenPrinter() has failed\n"));
        return (FALSE);
	}

	DWORD dwSize, dwNeeded;
	if(::GetPrinter(hPrinter,2,NULL,0,&dwSize)) 
	{
		TRACE(_T("COXSysInfo::GetPrinterInfo: GetPrinter() has failed\n"));
        return (FALSE);
	}

    // If LastError is any thing other than allocate size error, flag and exit. 
    DWORD dwErrorCode=::GetLastError(); 
    if(dwErrorCode!=ERROR_INSUFFICIENT_BUFFER)     
        return (FALSE);

	// Loop until we have size right. 
	BOOL bRightSize=FALSE;
	LPBYTE lpInfo=NULL;
    while(!bRightSize)     
	{ 
		if(NULL!=(lpInfo=(LPBYTE)GlobalAlloc(GPTR,dwSize)))
		{ 
#ifdef _DEBUG
			::memset(lpInfo,UNINIT_BYTE,dwSize); 
#endif 
            // Get Printer Info 
            bRightSize=::GetPrinter(hPrinter,2,lpInfo,dwSize,&dwNeeded); 
            if(!bRightSize)             
			{ 
				dwErrorCode=GetLastError();  
                // If anything other than allocate size error, flag and exit. 
				if(dwErrorCode!=ERROR_INSUFFICIENT_BUFFER)                 
				{ 
					::GlobalFree(lpInfo); 
			        return (FALSE);
				}                 
				else 
                {
					::GlobalFree(lpInfo); 
                    lpInfo=NULL;                     
					dwSize=dwNeeded; 
                }   
			} 
            else  // GetPrinter returned success        
			{ 
				// Save pointer to PRINTER_INFO structure 
				lpPrinterInfo=(LPPRINTER_INFO_2)lpInfo;  
            }   
		}    
		else 
	        return (FALSE);
    }    

	return (TRUE);
}

BOOL COXSysInfo::IsLocalPrinter(const CString& sPrinterName, 
								BOOL* pbLocalPrinter) const
{
	//	--- In:			sPrinterName:	reference to string which is the
	//									name of installed printer.
	//	--- Out:		pbLocalPrinter:	TRUE if specified printer is local,
	//									or FALSE otherwise
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the flag that specify whether the printer 
	//					is local or not

	ASSERT(pbLocalPrinter!=NULL);

	LPPRINTER_INFO_2 lpPrinterInfo=NULL;
	if(GetPrinterInfo(sPrinterName,lpPrinterInfo) && lpPrinterInfo!=NULL)
	{
		*pbLocalPrinter=(lpPrinterInfo->pServerName!=NULL && 
			lstrlen(lpPrinterInfo->pServerName)>0) ? FALSE : TRUE;
		::GlobalFree(lpPrinterInfo);
		return (TRUE);
	}

	return (FALSE);
}

BOOL COXSysInfo::IsSharedPrinter(const CString& sPrinterName, 
								 BOOL* pbSharedPrinter) const
{
	//	--- In:			sPrinterName:	reference to string which is the
	//									name of installed printer.
	//	--- Out:		pbSharedPrinter:TRUE if specified printer is shared,
	//									or FALSE otherwise
	//	--- Returns:	BOOL - TRUE if success
	//	---	Effect:		Retrieves the flag that specify whether the printer 
	//					is shared or not

	ASSERT(pbSharedPrinter!=NULL);

	LPPRINTER_INFO_2 lpPrinterInfo=NULL;
	if(GetPrinterInfo(sPrinterName,lpPrinterInfo) && lpPrinterInfo!=NULL)
	{
		*pbSharedPrinter=((lpPrinterInfo->
			Attributes&PRINTER_ATTRIBUTE_SHARED)!=0 &&
			lpPrinterInfo->pShareName!=NULL && 
			lstrlen(lpPrinterInfo->pShareName)>0) ? TRUE : FALSE;
		::GlobalFree(lpPrinterInfo);
		return (TRUE);
	}

	return (FALSE);
}


// protected

int COXSysInfo::DeviceCaps(int iIndex) const
{
	int		iValue;
	
	HDC hDC = CreateIC(_T("DISPLAY"), NULL, NULL, NULL);
	ASSERT(hDC != NULL);
	iValue = GetDeviceCaps(hDC, iIndex);
	DeleteDC(hDC);
	
	return (iValue);
}

void COXSysInfo::GetMemStatus()
{
	m_memStatus.dwLength = sizeof(m_memStatus);
	GlobalMemoryStatus(&m_memStatus);
}

void COXSysInfo::GetSysInfo()
{
	::GetSystemInfo(&m_sysInfo);
}

LPTSTR COXSysInfo::GetRegistryString(HKEY hKeyClass, LPTSTR lpszSubKey, 
									 LPTSTR lpszValueName) const
{     
	// Local variables 
    HKEY hKey=NULL;				// Registry key 
    LPTSTR lpszKeyValue;		// Buffer for key name 
    DWORD dwKeySize=0;			// Size of key value 
    DWORD dwKeyDataType=0;		// Type of data stored in key 

	if(ERROR_SUCCESS!=RegOpenKey(hKeyClass, lpszSubKey, &hKey))
		return NULL;

    // Got key, get value.  First, get the size of the key. 
    if(ERROR_SUCCESS!=RegQueryValueEx(hKey,lpszValueName,
		NULL,NULL,NULL,&dwKeySize)) 
		return NULL;
    if(dwKeySize <= 1)
		return NULL;

    lpszKeyValue=(LPTSTR)GlobalAlloc(GPTR,(++dwKeySize)); 
    if(lpszKeyValue==NULL)   
		return NULL;

    RegQueryValueEx(hKey,lpszValueName,NULL,&dwKeyDataType,
		(LPBYTE)lpszKeyValue,&dwKeySize);

	// it's caller responsibility to free the memory allocated for lpszKeyValue
    return lpszKeyValue; 
}


// static

// Sort all of the display modes:
// 1: Resolution (width), 2: color depth, 3: frequency.
int _cdecl DevModeCompare(const void *elem1, const void *elem2) 
{
	ASSERT(elem1!=NULL);
	ASSERT(elem2!=NULL);

	PDEVMODE pdvmd1=(PDEVMODE)elem1;
	PDEVMODE pdvmd2=(PDEVMODE)elem2;

	int nResult=pdvmd1->dmPelsWidth-pdvmd2->dmPelsWidth;
	if(nResult!=0) 
		return (nResult);

	nResult=pdvmd1->dmBitsPerPel-pdvmd2->dmBitsPerPel;
	if(nResult!=0) 
		return (nResult);

	nResult=pdvmd1->dmDisplayFrequency-pdvmd2->dmDisplayFrequency;
	return(nResult);
}

  
