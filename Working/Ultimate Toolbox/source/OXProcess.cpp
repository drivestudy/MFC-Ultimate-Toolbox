// OXProcess.cpp: implementation of the COXProcess class.
//
//////////////////////////////////////////////////////////////////////
// Version: 9.3


#include "stdafx.h"
#include "OXProcess.h"
#include "OXMainRes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef BOOL (FAR WINAPI *ISDEBUGGERPRESENT) (VOID);

BOOL IsRunningNT()
{
	OSVERSIONINFO	osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(!::GetVersionEx(&osvi))
		return FALSE;
	return (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT);
}


BOOL COXProcess::m_bIsRunningNT=IsRunningNT();

BOOL COXProcess::m_bProcessFunctionsInitialized=FALSE;
HINSTANCE COXProcess::m_hProcessHelperLib=NULL;
// PSAPI functions
PFNENUMPROCESSES COXProcess::pfnEnumProcesses=NULL;
PFNENUMPROCESSMODULES COXProcess::pfnEnumProcessModules=NULL;
PFNGETMODULEFILENAMEEXA COXProcess::pfnGetModuleFileNameExA=NULL;
// Toolhelp32 functions
PFNCREATETOOLHELP32SNAPSHOT COXProcess::pfnCreateToolhelp32Snapshot=NULL;
PFNPROCESS32FIRST COXProcess::pfnProcess32First=NULL;
PFNPROCESS32NEXT COXProcess::pfnProcess32Next=NULL;
PFNMODULE32FIRST COXProcess::pfnModule32First=NULL;
PFNMODULE32NEXT COXProcess::pfnModule32Next=NULL;



//////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////

COXProcess::~COXProcess() 
{
	if(m_bProcessFunctionsInitialized && m_hProcessHelperLib!=NULL)
	{
		::FreeLibrary(m_hProcessHelperLib);
		m_bProcessFunctionsInitialized=FALSE;
		m_hProcessHelperLib=NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// Static Functions
//////////////////////////////////////////////////////////////////////

DWORD COXProcess::GetCurrentProcessID()
{
	return ::GetCurrentProcessId();
}


BOOL COXProcess::GetCurrentProcessEnvironment(CStringArray& arrEnvironmentString)
{

	arrEnvironmentString.RemoveAll();

	// Get a pointer to the environment block. 
	LPVOID lpvEnv=::GetEnvironmentStrings(); 
	if(lpvEnv==NULL)
	{
		TRACE(_T("COXProcess::GetCurrentProcessEnvironment: GetEnvironmentStrings() returned NULL pointer to environment block\n"));
		return FALSE;
	}
 
	// Variable strings are separated by NULL byte, and the block is 
	// terminated by a NULL byte. 
	CString sEnvironmentString;
	LPTSTR lpszVariable=(LPTSTR)lpvEnv;
	for(;;) 
	{ 
		sEnvironmentString=lpszVariable;
		if(sEnvironmentString.IsEmpty())
			break;
		else
		{
			arrEnvironmentString.Add(sEnvironmentString);
			lpszVariable+=(sEnvironmentString.GetLength()+1)*sizeof(TCHAR);
		}
	}
	
	::FreeEnvironmentStrings((LPTSTR)lpvEnv);

	return TRUE;
}
	
BOOL COXProcess::IsDebuggerPresent()
{
	// get handle of the KERNEL.LIB
	BOOL bAlreadyLoaded=TRUE;
	HINSTANCE hKernel=::GetModuleHandle(_T("kernel32.dll"));
	if(hKernel==NULL)
	{
		// load the DLL
		hKernel=::LoadLibrary(_T("kernel32.dll"));
		bAlreadyLoaded=FALSE;
	}

	BOOL bResult=FALSE;

	if(hKernel)
	{
		/*
		You must get this function explicitly because earlier versions
		don't implement this function.
		*/
		ISDEBUGGERPRESENT pIsDebuggerPresent=
			(ISDEBUGGERPRESENT)GetProcAddress(hKernel, "IsDebuggerPresent");
   
		if(pIsDebuggerPresent)
			bResult=(*pIsDebuggerPresent)();
   
		if(!bAlreadyLoaded)
			::FreeLibrary(hKernel);
	}

	return bResult;
}

BOOL COXProcess::IsValidProcessID(DWORD dwProcessID)
{
	if(GetCurrentProcessID()==dwProcessID || dwProcessID==0 || dwProcessID==2)
		return TRUE;

	// Using the process ID, open up a handle to the process
	HANDLE hProcess=
		::OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,dwProcessID);
	if(hProcess!=NULL)
	{
		::CloseHandle(hProcess);
		return TRUE;
	}

	return FALSE;
}

BOOL COXProcess::GetProcessImageFileName(DWORD dwProcessID, CString& sFileName)
{
	COXProcess process(dwProcessID);
	if(!process.IsValid())
		return FALSE;

	return process.GetImageFileName(sFileName);
}

//////////////////////////////////////////////////////////////////////
// Protected Functions
//////////////////////////////////////////////////////////////////////

BOOL COXProcess::InitializeProcessFunctions() const
{
	if(m_bProcessFunctionsInitialized)
		return TRUE;

	if(m_bIsRunningNT)
	{
		m_hProcessHelperLib=::LoadLibrary(_T("PSAPI.DLL"));
		if(m_hProcessHelperLib==NULL)
			return FALSE;

		pfnEnumProcesses=
			(PFNENUMPROCESSES)::GetProcAddress(m_hProcessHelperLib,"EnumProcesses");
		pfnEnumProcessModules=
			(PFNENUMPROCESSMODULES)::GetProcAddress(m_hProcessHelperLib,"EnumProcessModules");
		pfnGetModuleFileNameExA=
			(PFNGETMODULEFILENAMEEXA)::GetProcAddress(m_hProcessHelperLib,"GetModuleFileNameExA");

		if(!pfnEnumProcesses || !pfnEnumProcessModules || !pfnGetModuleFileNameExA)
		{
			::FreeLibrary(m_hProcessHelperLib);
			m_hProcessHelperLib=NULL;
			return FALSE;
		}
	}
	else
	{
		BOOL bWasLoaded=TRUE;
		HINSTANCE m_hProcessHelperLib=::GetModuleHandle(_T("KERNEL32.DLL"));
		if(m_hProcessHelperLib==NULL)
		{
			HINSTANCE m_hProcessHelperLib=::LoadLibrary(_T("KERNEL32.DLL"));
			if(m_hProcessHelperLib==NULL)
				return FALSE;
			bWasLoaded=FALSE;
		}

		pfnCreateToolhelp32Snapshot=
			(PFNCREATETOOLHELP32SNAPSHOT)
			::GetProcAddress(m_hProcessHelperLib,"CreateToolhelp32Snapshot");
		pfnProcess32First=
			(PFNPROCESS32FIRST)::GetProcAddress(m_hProcessHelperLib,"Process32First");
		pfnProcess32Next=
			(PFNPROCESS32NEXT)::GetProcAddress(m_hProcessHelperLib,"Process32Next");
		pfnModule32First=
			(PFNMODULE32FIRST)::GetProcAddress(m_hProcessHelperLib,"Module32First");
		pfnModule32Next=
			(PFNMODULE32NEXT)::GetProcAddress(m_hProcessHelperLib,"Module32Next");
        
		if(!pfnCreateToolhelp32Snapshot || !pfnProcess32First || 
			!pfnProcess32Next || !pfnModule32First || !pfnModule32Next)
		{
			if(!bWasLoaded)
				::FreeLibrary(m_hProcessHelperLib);
			m_hProcessHelperLib=NULL;
			return (FALSE);
		}

		if(bWasLoaded)
			m_hProcessHelperLib=NULL;
	}

	m_bProcessFunctionsInitialized=TRUE;

	return (TRUE);
}


//////////////////////////////////////////////////////////////////////
// Public Functions
//////////////////////////////////////////////////////////////////////

BOOL COXProcess::GetImageFileName(CString& sFileName) const
{
	if(GetCurrentProcessID()==m_dwProcessID)
	{
		LPTSTR pszBuffer=sFileName.GetBuffer(MAX_PATH);
		ASSERT(pszBuffer!=NULL);
		BOOL bResult=(::GetModuleFileName(NULL,pszBuffer,MAX_PATH)>0);
		sFileName.ReleaseBuffer();

		return bResult;
	}

	VERIFY(InitializeProcessFunctions());

	if(m_bIsRunningNT)
	{
		// Using the process ID, open up a handle to the process
		HANDLE hProcess=::OpenProcess(
			PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,m_dwProcessID);
		if(hProcess==NULL)
		{
			if(m_dwProcessID==0)
			{
				VERIFY(sFileName.LoadString(IDS_OX_PROCESSIDLE));//"System Idle Process"
				return TRUE;
			}
			if(m_dwProcessID==2 || m_dwProcessID==8)
			{
				VERIFY(sFileName.LoadString(IDS_OX_PROCESSSYSTEM));//"System"
				return TRUE;
			}
			return FALSE;
		}
            
		HMODULE moduleArray[1024];
		DWORD cbNeeded;
		// EnumProcessModules returns an array of HMODULEs for the process
		if(!pfnEnumProcessModules(hProcess,moduleArray,sizeof(moduleArray),&cbNeeded))
		{
			::CloseHandle(hProcess);
			if(m_dwProcessID==0)
			{
				VERIFY(sFileName.LoadString(IDS_OX_PROCESSIDLE));//"System Idle Process"
				return TRUE;
			}
			if(m_dwProcessID==2 || m_dwProcessID==8)
			{
				VERIFY(sFileName.LoadString(IDS_OX_PROCESSSYSTEM));//"System"
				return TRUE;
			}
			return FALSE;
		}

		// First module is the EXE
		char szModuleName[MAX_PATH];
		// GetModuleFileNameEx is like GetModuleFileName, but works
		// in other process address spaces
		pfnGetModuleFileNameExA(
			hProcess,moduleArray[0],szModuleName,sizeof(szModuleName));
		sFileName=szModuleName;

		::CloseHandle(hProcess);    // We're done with this process handle
	}
	else
	{
	    // Create a ToolHelp32 snapshot containing the process list
	    //  
	    HANDLE hSnapshotProcess;
	    hSnapshotProcess=pfnCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	    if(hSnapshotProcess==NULL)
		{
			return FALSE;
		}

		// Iterate through each of the processes in the snapshot
		PROCESSENTRY32 procEntry={ sizeof(PROCESSENTRY32) };
		BOOL bProcessContinue;
		BOOL bSuccess=FALSE;
		for(bProcessContinue=pfnProcess32First(hSnapshotProcess,&procEntry);
			bProcessContinue;
			bProcessContinue=pfnProcess32Next(hSnapshotProcess,&procEntry))
		{
			if(m_dwProcessID==procEntry.th32ProcessID)
			{
				sFileName=procEntry.szExeFile;
				bSuccess=TRUE;
				break;
			}
		}
    
		::CloseHandle(hSnapshotProcess);    // Done with process list snapshot

		if(!bSuccess)
		{
			return FALSE;
		}
	}

	return TRUE;
}


BOOL COXProcess::GetPriorityClass(DWORD& dwPriorityClass) const
{
	// Using the process ID, open up a handle to the process
	HANDLE hProcess=::OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,
		FALSE,m_dwProcessID);
	if(hProcess==NULL)
		return FALSE;

	dwPriorityClass=::GetPriorityClass(hProcess);
	if(dwPriorityClass==0)
	{
		return FALSE;
	}

	return TRUE;
}


BOOL COXProcess::GetVersion(DWORD& dwVersion) const
{
	dwVersion=::GetProcessVersion(m_dwProcessID);
	return TRUE;
}


// Windows NT functions
//

BOOL COXProcess::IsPriorityBoostDisabled(BOOL& bDisabled) const
{
#if (_WIN32_WINNT >= 0x0400)
	// Using the process ID, open up a handle to the process
	HANDLE hProcess=::OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,
		FALSE,m_dwProcessID);
	if(hProcess==NULL)
		return FALSE;

	return ::GetProcessPriorityBoost(hProcess,&bDisabled);
#else
	UNREFERENCED_PARAMETER(bDisabled);
	return FALSE;
#endif
}


BOOL COXProcess::GetMinMaxWorkingSetSize(DWORD& dwMinWorkingSetSize, 
										 DWORD& dwMaxWorkingSetSize) const
{
	// Using the process ID, open up a handle to the process
	HANDLE hProcess=::OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,
		FALSE,m_dwProcessID);
	if(hProcess==NULL)
		return FALSE;

	return ::GetProcessWorkingSetSize(hProcess,
		&dwMinWorkingSetSize,&dwMaxWorkingSetSize);
}


BOOL COXProcess::GetCreationTime(FILETIME& fileTime) const
{
	// Using the process ID, open up a handle to the process
	HANDLE hProcess=::OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,
		FALSE,m_dwProcessID);
	if(hProcess==NULL)
		return FALSE;
	
	FILETIME exitTime, kernelTime, userTime;
	return ::GetProcessTimes(hProcess,&fileTime,&exitTime,&kernelTime,&userTime);
}

BOOL COXProcess::GetCreationTime(CTime& time) const
{
	FILETIME creationTime;
	if(!GetCreationTime(creationTime))
		return FALSE;

	CTime timeCopy(creationTime);
	time=timeCopy;

	return TRUE;
}

BOOL COXProcess::GetCreationTime(COleDateTime& oleTime) const
{
	FILETIME creationTime;
	if(!GetCreationTime(creationTime))
		return FALSE;

	COleDateTime timeCopy(creationTime);
	oleTime=timeCopy;

	return TRUE;
}


BOOL COXProcess::GetExitTime(FILETIME& fileTime) const
{
	// Using the process ID, open up a handle to the process
	HANDLE hProcess=::OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,
		FALSE,m_dwProcessID);
	if(hProcess==NULL)
		return FALSE;
	
	FILETIME creationTime, kernelTime, userTime;
	return ::GetProcessTimes(hProcess,&creationTime,&fileTime,&kernelTime,&userTime);
}

BOOL COXProcess::GetExitTime(CTime& time) const
{
	FILETIME creationTime;
	if(!GetExitTime(creationTime))
		return FALSE;

	CTime timeCopy(creationTime);
	time=timeCopy;

	return TRUE;
}

BOOL COXProcess::GetExitTime(COleDateTime& oleTime) const
{
	FILETIME creationTime;
	if(!GetExitTime(creationTime))
		return FALSE;

	COleDateTime timeCopy(creationTime);
	oleTime=timeCopy;

	return TRUE;
}


BOOL COXProcess::GetKernelTime(DWORDLONG& timeSpent) const
{
	// Using the process ID, open up a handle to the process
	HANDLE hProcess=::OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,
		FALSE,m_dwProcessID);
	if(hProcess==NULL)
		return FALSE;
	
	FILETIME creationTime, exitTime, kernelTime, userTime;
	if(::GetProcessTimes(hProcess,&creationTime,&exitTime,&kernelTime,&userTime))
	{
		timeSpent=(((DWORDLONG)kernelTime.dwHighDateTime)<<32)+
			((DWORDLONG)kernelTime.dwLowDateTime);
		return TRUE;
	}

	return FALSE;
}


BOOL COXProcess::GetUserTime(DWORDLONG& timeSpent) const
{
	// Using the process ID, open up a handle to the process
	HANDLE hProcess=::OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,
		FALSE,m_dwProcessID);
	if(hProcess==NULL)
		return FALSE;
	
	FILETIME creationTime, exitTime, kernelTime, userTime;
	if(::GetProcessTimes(hProcess,&creationTime,&exitTime,&kernelTime,&userTime))
	{
		timeSpent=(((DWORDLONG)userTime.dwHighDateTime)<<32)+
			((DWORDLONG)userTime.dwLowDateTime);
		return TRUE;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////


BOOL COXProcessIterator::MakeSnapshot()
{
	Cleanup();

	if(!InitializeProcessFunctions())
		return FALSE;

	if(m_bIsRunningNT)
	{
		// an array for 1024 processes should be enough :-)
		DWORD pidArray[1024];
		DWORD cbNeeded;
		// EnumProcesses returns an array of process IDs
		if(!pfnEnumProcesses(pidArray,sizeof(pidArray),&cbNeeded))
			return FALSE;

		// Iterate through each process in the array
		for(int nProcess=0; nProcess<(int)(cbNeeded/sizeof(DWORD)); nProcess++)
			m_arrProcesses.Add(pidArray[nProcess]);
	}
	else
	{
	    // Create a ToolHelp32 snapshot containing the process list
	    //  
	    HANDLE hSnapshotProcess;
	    hSnapshotProcess=pfnCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	    if(hSnapshotProcess==NULL)
			return FALSE;

		// Iterate through each of the processes in the snapshot
		PROCESSENTRY32 procEntry={ sizeof(PROCESSENTRY32) };
		BOOL bProcessContinue;
		for(bProcessContinue=pfnProcess32First(hSnapshotProcess,&procEntry);
			bProcessContinue;
			bProcessContinue=pfnProcess32Next(hSnapshotProcess,&procEntry))
		{
			m_arrProcesses.Add(procEntry.th32ProcessID);
		}
    
		::CloseHandle(hSnapshotProcess);    // Done with process list snapshot
	}

	m_bCanIterate=TRUE;

	ASSERT(GetProcessCount()>0);
	m_nCurrentIndex=0;
	m_dwProcessID=m_arrProcesses[0];

	return TRUE;
}


BOOL COXProcessIterator::Next(UINT nOffset)
{
	if(!CanIterate())
		return FALSE;

	if(nOffset==0)
		return TRUE;

	if(m_nCurrentIndex+(int)nOffset>=GetProcessCount())
		return FALSE;

	m_nCurrentIndex+=nOffset;
	m_dwProcessID=m_arrProcesses[m_nCurrentIndex];

	return TRUE;
}


BOOL COXProcessIterator::Prev(UINT nOffset)
{
	if(!CanIterate())
		return FALSE;

	if(nOffset==0)
		return TRUE;

	if(m_nCurrentIndex-nOffset<0)
		return FALSE;

	m_nCurrentIndex-=nOffset;
	m_dwProcessID=m_arrProcesses[m_nCurrentIndex];

	return TRUE;
}





