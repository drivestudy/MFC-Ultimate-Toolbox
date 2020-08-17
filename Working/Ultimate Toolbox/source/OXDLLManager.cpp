// ==========================================================================
// 							Class Implementation : 
//								COXDLLManager
// ==========================================================================

// Source file : OXDLLManager.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXDLLManager.h"
#include "UTB64Bit.h"

UINT AFXAPI HashKey<MODULEINFO> (MODULEINFO moduleInfo)
{
	UINT_PTR nHandleHash=((DWORD_PTR)moduleInfo.hModule) >> 4;
	UINT nFileNameHash=0;
	LPCTSTR key=(LPCTSTR)moduleInfo.sModuleFileName;
	while(*key)
		nFileNameHash=(nFileNameHash<<5)+nFileNameHash+*key++;
	return (DWORD)((nHandleHash>>2)+(nFileNameHash<<2));
};

/////////////////////////////////////////////////////////////////////////////
// COXDLLManager


BOOL COXDLLManager::m_bIsWindowsNT=COXDLLManager::IsWindowsNTRunning();


COXDLLManager::COXDLLManager()
{
	Reset();
}

COXDLLManager::COXDLLManager(LPCTSTR lpLibFileName)
{
	Reset();
	LoadLibrary(lpLibFileName);
}

COXDLLManager::COXDLLManager(LPCTSTR lpLibFileName, const HANDLE hFile, 
							 DWORD dwFlags)
{
	Reset();
	LoadLibraryEx(lpLibFileName,hFile,dwFlags);
}

COXDLLManager::~COXDLLManager()
{
	FreeLibrary();
}


HINSTANCE COXDLLManager::LoadLibrary(LPCTSTR lpLibFileName) 
{
	ASSERT(lpLibFileName!=NULL);

	FreeLibrary();

	m_bWasAlreadyLoaded=TRUE;
	m_hModule=::GetModuleHandle(lpLibFileName);
	if(m_hModule==NULL)
	{
		m_hModule=::LoadLibrary(lpLibFileName);
		m_bWasAlreadyLoaded=FALSE;
	}

	return m_hModule;
}

HINSTANCE COXDLLManager::LoadLibraryEx(LPCTSTR lpLibFileName, 
									   HANDLE hFile, DWORD dwFlags)
{
	ASSERT(lpLibFileName!=NULL);
	ASSERT(hFile!=NULL);

	FreeLibrary();

	m_bWasAlreadyLoaded=FALSE;
	m_hModule=::LoadLibraryEx(lpLibFileName,hFile,dwFlags);

	return m_hModule;
}

BOOL COXDLLManager::FreeLibrary() 
{
	BOOL bResult=TRUE;

	if(IsLoaded() && !m_bWasAlreadyLoaded)
		bResult=::FreeLibrary(m_hModule);
	
	Reset();
	
	return bResult;
}

BOOL COXDLLManager::Attach(HMODULE hModule, BOOL bUnloadOnDestroy) 
{
	ASSERT(hModule!=NULL);

	if(!FreeLibrary())
		return FALSE;

	m_hModule=hModule;
	m_bWasAlreadyLoaded=!bUnloadOnDestroy;

	TCHAR szModuleFileName[_MAX_PATH];
	m_bIsOwningModule=(::GetModuleFileName(hModule,szModuleFileName,
		sizeof(szModuleFileName)/sizeof(szModuleFileName[0]))>0) ? TRUE : FALSE;

	return TRUE;
}

BOOL COXDLLManager::GetVersionInfo(DLLVERSIONINFO* pdvi) const
{
	ASSERT(IsLoaded());
	ASSERT(pdvi!=NULL);
	ASSERT(!IsBadWritePtr(pdvi,sizeof(DLLVERSIONINFO)));

	if(!IsLoaded())
		return FALSE;

	/*
	You must get this function explicitly because earlier versions of the DLL 
	don't implement this function. That makes the lack of implementation of the 
	function a version marker in itself.
	*/
	DLLGETVERSIONPROC pDllGetVersion=
		(DLLGETVERSIONPROC)GetProcAddress("DllGetVersion");
   
	if(pDllGetVersion)
	{
		::ZeroMemory(pdvi,sizeof(DLLVERSIONINFO));
		pdvi->cbSize=sizeof(DLLVERSIONINFO);
		HRESULT hResult=(*pDllGetVersion)(pdvi);
      
		if(SUCCEEDED(hResult))
			return TRUE;
	}
  
	return FALSE;
}

BOOL COXDLLManager::GetFileInfo(DLLFILEINFO* pdfi) const
{
	ASSERT(IsLoaded());
	ASSERT(pdfi!=NULL);

	if(!IsLoaded())
		return FALSE;

	pdfi->translation.nCharset = 1252;		// default = ANSI code page
	::ZeroMemory(&pdfi->fixedInfo,sizeof(VS_FIXEDFILEINFO));

	// get module file name
	TCHAR pszFileName[_MAX_PATH];
	DWORD dwLength=GetModuleFileName(pszFileName,
		sizeof(pszFileName)/sizeof(pszFileName[0]));
	if(dwLength<=0)
		return FALSE;

	// read file version info
	DWORD dwHandle; // will always be set to zero
	dwLength=GetFileVersionInfoSize(pszFileName, &dwHandle);
	if(dwLength<=0)
		return FALSE;

	// allocate version info
	LPBYTE pFileVersionInfo=new BYTE[dwLength]; 
	if(!::GetFileVersionInfo(pszFileName,0,dwLength,pFileVersionInfo))
	{
		delete[] pFileVersionInfo;
		return FALSE;
	}

	LPVOID lpvi;
	UINT nLength;
	if(!VerQueryValue(pFileVersionInfo,_T("\\"),&lpvi,&nLength))
	{
		delete[] pFileVersionInfo;
		return FALSE;
	}

	// copy fixed info
	pdfi->fixedInfo=*(VS_FIXEDFILEINFO*)lpvi;

	// Get translation info
	if(VerQueryValue(pFileVersionInfo,_T("\\VarFileInfo\\Translation"), 
		&lpvi,&nLength) && nLength>=4) 
	{
		pdfi->translation=*(TRANSLATION*)lpvi;
	}

	BOOL bResult=FALSE;
	if(pdfi->fixedInfo.dwSignature==VS_FFI_SIGNATURE)
	{
		pdfi->sCompanyName=
			GetFileInfoValue(pFileVersionInfo,pdfi->translation.nLangID,
			pdfi->translation.nCharset,_T("CompanyName"));
		pdfi->sFileDescription=
			GetFileInfoValue(pFileVersionInfo,pdfi->translation.nLangID,
			pdfi->translation.nCharset,_T("FileDescription"));
		pdfi->sFileVersion=
			GetFileInfoValue(pFileVersionInfo,pdfi->translation.nLangID,
			pdfi->translation.nCharset,_T("FileVersion"));
		pdfi->sInternalName=
			GetFileInfoValue(pFileVersionInfo,pdfi->translation.nLangID,
			pdfi->translation.nCharset,_T("InternalName"));
		pdfi->sLegalCopyright=
			GetFileInfoValue(pFileVersionInfo,pdfi->translation.nLangID,
			pdfi->translation.nCharset,_T("LegalCopyright"));
		pdfi->sOriginalFilename=
			GetFileInfoValue(pFileVersionInfo,pdfi->translation.nLangID,
			pdfi->translation.nCharset,_T("OriginalFilename"));
		pdfi->sProductName=
			GetFileInfoValue(pFileVersionInfo,pdfi->translation.nLangID,
			pdfi->translation.nCharset,_T("ProductName"));
		pdfi->sProductVersion=
			GetFileInfoValue(pFileVersionInfo,pdfi->translation.nLangID,
			pdfi->translation.nCharset,_T("ProductVersion"));

		bResult=TRUE;
	}

	delete[] pFileVersionInfo;

	return bResult;
}

BOOL COXDLLManager::GetSnapshot(DLLSNAPSHOT* pSnapshot) const
{
	ASSERT(pSnapshot!=NULL);
	ASSERT(IsLoaded() || !m_bIsOwningModule);

	pSnapshot->nRefCount=0;
	pSnapshot->arrDependantProcesses.RemoveAll();

	if(m_bIsWindowsNT)
	{
	    COXDLLManager dllPSAPI(_T("PSAPI.DLL"));
		if(!dllPSAPI.IsLoaded())
			return FALSE;
		static PFNENUMPROCESSES pfnEnumProcesses=
			(PFNENUMPROCESSES)dllPSAPI.GetProcAddress("EnumProcesses");
		static PFNENUMPROCESSMODULES pfnEnumProcessModules=
			(PFNENUMPROCESSMODULES)dllPSAPI.GetProcAddress("EnumProcessModules");
		static PFNGETMODULEFILENAMEEXA pfnGetModuleFileNameExA=
			(PFNGETMODULEFILENAMEEXA)dllPSAPI.GetProcAddress("GetModuleFileNameExA");

		if(!pfnEnumProcesses || !pfnEnumProcessModules || !pfnGetModuleFileNameExA)
			return FALSE;
    
		// an array for 1024 processes should be enough :-)
		DWORD pidArray[1024];
		DWORD cbNeeded;
		// EnumProcesses returns an array of process IDs
		if(!pfnEnumProcesses(pidArray,sizeof(pidArray),&cbNeeded))
			return FALSE;

		// Iterate through each process in the array
		for(int nProcess=0; nProcess<(int)(cbNeeded/sizeof(DWORD)); nProcess++)
		{
			// Using the process ID, open up a handle to the process
			HANDLE hProcess=OpenProcess(
				PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,
				pidArray[nProcess]);
			if(hProcess==NULL)
				continue;
            
			HMODULE moduleArray[1024];
			DWORD cbNeeded;
			// EnumProcessModules returns an array of HMODULEs for the process
			if(!pfnEnumProcessModules(hProcess,moduleArray,
				sizeof(moduleArray),&cbNeeded))
			{
				CloseHandle(hProcess);
				continue;
			}

			CString sProcessFileName(_T(""));
	        // Iterate through each of the process's modules
		    for(int nModule=0; nModule<(int)(cbNeeded/sizeof(moduleArray[0])); 
				nModule++)
			{
				char szModuleName[MAX_PATH];
	            // GetModuleFileNameEx is like GetModuleFileName, but works
		        // in other process address spaces
				pfnGetModuleFileNameExA(hProcess,moduleArray[nModule],
					szModuleName,sizeof(szModuleName));

				if(0==nModule)   // First module is the EXE.
				{
	                sProcessFileName=szModuleName;    
				}
		        else    // Not the first module.  It's a DLL
				{
					if(moduleArray[nModule]==m_hModule)
					{
						pSnapshot->nRefCount++;
						pSnapshot->arrDependantProcesses.Add(sProcessFileName);
						break;
					}
				}
			}
    
			CloseHandle( hProcess );    // We're done with this process handle
		}
 
		return TRUE;
	}
	else
	{
	    static COXDLLManager dllKERNEL32(_T("KERNEL32.DLL"));
		if(!dllKERNEL32.IsLoaded())
			return FALSE;

	    static PFNCREATETOOLHELP32SNAPSHOT pfnCreateToolhelp32Snapshot=
			(PFNCREATETOOLHELP32SNAPSHOT)
			dllKERNEL32.GetProcAddress("CreateToolhelp32Snapshot");
        static PFNPROCESS32FIRST pfnProcess32First=
			(PFNPROCESS32FIRST)
			dllKERNEL32.GetProcAddress("Process32First");
        static PFNPROCESS32NEXT pfnProcess32Next=
			(PFNPROCESS32NEXT)
			dllKERNEL32.GetProcAddress("Process32Next");
        static PFNMODULE32FIRST pfnModule32First=
			(PFNMODULE32FIRST)
			dllKERNEL32.GetProcAddress("Module32First");
        static PFNMODULE32NEXT pfnModule32Next=
			(PFNMODULE32NEXT)
			dllKERNEL32.GetProcAddress("Module32Next");
        
		if(!pfnCreateToolhelp32Snapshot || !pfnProcess32First || 
			!pfnProcess32Next || !pfnModule32First || !pfnModule32Next)
			return FALSE;

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
	        // Enumerate the module list for this process.  Start by taking
		    // another ToolHelp32 snapshot, this time of the process's module list
			//
			// Using the process ID, open up a handle to the process
			HANDLE hSnapshotModule;
			hSnapshotModule=pfnCreateToolhelp32Snapshot(TH32CS_SNAPMODULE,
				procEntry.th32ProcessID);
			if(!hSnapshotModule)
				continue;

			// Iterate through each module in the snapshot                                                  
			MODULEENTRY32 modEntry={ sizeof(MODULEENTRY32) };
			BOOL bModuleContinue;

			for(bModuleContinue=pfnModule32First(hSnapshotModule,&modEntry);
				bModuleContinue;
				bModuleContinue=pfnModule32Next(hSnapshotModule,&modEntry))
			{
				if(modEntry.hModule==m_hModule)
				{
					pSnapshot->nRefCount++;
					pSnapshot->
						arrDependantProcesses.Add((LPCTSTR)procEntry.szExeFile);
					break;
				}
	        }
        
			CloseHandle( hSnapshotModule ); // Done with module list snapshot
		}
    
		CloseHandle( hSnapshotProcess );    // Done with process list snapshot
    
		return TRUE;
	}

}


// static members

BOOL COXDLLManager::IsWindowsNTRunning()
{
	BOOL bResult=FALSE;

	OSVERSIONINFO verInfo;
	verInfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	if (::GetVersionEx(&verInfo))
	{
		if (verInfo.dwPlatformId==VER_PLATFORM_WIN32_NT && 
			verInfo.dwMajorVersion>=4)
		{
			bResult=TRUE;
		}
	}

	return bResult;
}

BOOL COXDLLManager::GetAllLoadedDLLs(CProcessModulesMap& mapProcessModules)
{
	mapProcessModules.RemoveAll();

	if(m_bIsWindowsNT)
	{
	    COXDLLManager dllPSAPI(_T("PSAPI.DLL"));
		if(!dllPSAPI.IsLoaded())
			return FALSE;
		static PFNENUMPROCESSES pfnEnumProcesses=
			(PFNENUMPROCESSES)dllPSAPI.GetProcAddress("EnumProcesses");
		static PFNENUMPROCESSMODULES pfnEnumProcessModules=
			(PFNENUMPROCESSMODULES)dllPSAPI.GetProcAddress("EnumProcessModules");
		static PFNGETMODULEFILENAMEEXA pfnGetModuleFileNameExA=
			(PFNGETMODULEFILENAMEEXA)dllPSAPI.GetProcAddress("GetModuleFileNameExA");

		if(!pfnEnumProcesses || !pfnEnumProcessModules || !pfnGetModuleFileNameExA)
			return FALSE;
    
		// an array for 1024 processes should be enough :-)
		DWORD pidArray[1024];
		DWORD cbNeeded;
		// EnumProcesses returns an array of process IDs
		if(!pfnEnumProcesses(pidArray,sizeof(pidArray),&cbNeeded))
			return FALSE;

		// Iterate through each process in the array
		for(int nProcess=0; nProcess<(int)(cbNeeded/sizeof(DWORD)); nProcess++)
		{
			// Using the process ID, open up a handle to the process
			HANDLE hProcess=OpenProcess(
				PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,
				pidArray[nProcess]);
			if(hProcess==NULL)
				continue;
            
			HMODULE moduleArray[1024];
			DWORD cbNeeded;
			// EnumProcessModules returns an array of HMODULEs for the process
			if(!pfnEnumProcessModules(hProcess,moduleArray,
				sizeof(moduleArray),&cbNeeded))
			{
				CloseHandle(hProcess);
				continue;
			}

			PROCESSINFO* pProcessInfo=new PROCESSINFO;
			HMODULE hProcessModule=NULL;
	        // Iterate through each of the process's modules
		    for(int nModule=0; nModule<(int)(cbNeeded/sizeof(moduleArray[0])); 
				nModule++)
			{
				char szModuleName[MAX_PATH];
	            // GetModuleFileNameEx is like GetModuleFileName, but works
		        // in other process address spaces
				pfnGetModuleFileNameExA(hProcess,moduleArray[nModule],
					szModuleName,sizeof(szModuleName));

				if(0==nModule)   // First module is the EXE.
				{
					hProcessModule=moduleArray[nModule];
				}
		        else    // Not the first module.  It's a DLL
				{
					CString sModuleName((LPCSTR)szModuleName);
					pProcessInfo->moduleMap.SetAt(MODULEINFO(moduleArray[nModule],sModuleName),1);
				}
			}
			ASSERT(hProcessModule!=NULL);
			pProcessInfo->hProcessModule=hProcessModule;
			mapProcessModules.SetAt((DWORD_PTR)hProcess,pProcessInfo);
    
			CloseHandle( hProcess );    // We're done with this process handle
		}
 
		return TRUE;
	}
	else
	{

		return TRUE;
	}

}


// protected members

CString	COXDLLManager::GetFileInfoValue(const LPBYTE pFileVersionInfo, 
										WORD nLangID, WORD nCharset,
										LPCTSTR lpKeyName) const
{
	CString sValue(_T(""));

	CString sSubBlock;
	sSubBlock.Format(_T("\\StringFileInfo\\%04x%04x\\%s"),
		nLangID,nCharset,lpKeyName);

	LPVOID pValue;
	UINT nLengthValue;
	if(::VerQueryValue(pFileVersionInfo,(LPTSTR)(LPCTSTR)sSubBlock,
		&pValue,&nLengthValue)) 
	{
		sValue=(LPCTSTR)pValue;
	}

	return sValue;
}
