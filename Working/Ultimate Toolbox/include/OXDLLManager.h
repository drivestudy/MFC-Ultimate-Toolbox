// ==========================================================================
// 							Class Specification : 
//								COXDLLManager
// ==========================================================================

// Header file : OXDLLManager.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////

#ifndef _DLLMANAGER_H
#define _DLLMANAGER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


#ifndef _INC_TOOLHELP32
#include "tlhelp32.h"
#define _INC_TOOLHELP32
#endif

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif


// tell linker to link with version.lib for VerQueryValue, etc.
#pragma comment(linker, "/defaultlib:version.lib")

#ifndef DWORD_PTR // for VC++ 6
	typedef DWORD DWORD_PTR;
#endif

// we need these type definitions to get information about dll version
// we use it in our GetDLLVersionInfo function
#ifndef _OXDLLVERSIONINFO_DECL
#define _OXDLLVERSIONINFO_DECL
#if _MFC_VER >0x0421
#include <shlwapi.h>
#else
typedef struct _DllVersionInfo
{
	DWORD cbSize;
	DWORD dwMajorVersion;
	DWORD dwMinorVersion;
	DWORD dwBuildNumber;
	DWORD dwPlatformID;
}DLLVERSIONINFO;
#define DLLVER_PLATFORM_WINDOWS		0	// Windows platform (95/98)
#define DLLVER_PLATFORM_NT			1	// Windows NT platform
typedef int (FAR WINAPI *DLLGETVERSIONPROC) (DLLVERSIONINFO *);
#endif
#endif
///////////////////////////

typedef struct _tagTRANSLATION 
{
	WORD nLangID;			// language ID
	WORD nCharset;			// character set (code page)

} TRANSLATION;

typedef struct _tagDLLFILEINFO
{
	VS_FIXEDFILEINFO fixedInfo;
	TRANSLATION translation;
	CString sCompanyName;
	CString sFileDescription;
	CString sFileVersion;
	CString sInternalName;
	CString sLegalCopyright;
	CString sOriginalFilename;
	CString sProductName;
	CString sProductVersion;

} DLLFILEINFO;
////////////////////////////

typedef struct _tagDLLSNAPSHOT
{
	int nRefCount;
	CStringArray arrDependantProcesses;

} DLLSNAPSHOT;



typedef struct _tagMODULEINFO
{
	HMODULE hModule;
	CString sModuleFileName;

	_tagMODULEINFO(HMODULE Module=NULL, CString ModuleFileName=_T(""))
	{
		hModule=Module;
		sModuleFileName=ModuleFileName;
	}

    // assignment operator
	_tagMODULEINFO& operator=(const _tagMODULEINFO& mi)
	{
		if(this==&mi)
			return *this;
		
		hModule=mi.hModule;
		sModuleFileName=mi.sModuleFileName;
		return *this;
	}

    // compare operator
	BOOL operator==(const _tagMODULEINFO mi) const 
	{
		return ((hModule==mi.hModule)&
			(sModuleFileName.CompareNoCase(mi.sModuleFileName)==0));
	}

/*	operator DWORD() const
	{
		UINT nHandleHash=((DWORD)hModule) >> 4;
		UINT nFileNameHash=0;
		LPCTSTR key=(LPCTSTR)sModuleFileName;
		while(*key)
			nFileNameHash=(nFileNameHash<<5)+nFileNameHash+*key++;
		return (DWORD)(nHandleHash>>2+nFileNameHash<<2);
	}
*/
} MODULEINFO;

#ifdef _BUILD_UTB_INTO_EXTDLL
	template UINT AFXAPI HashKey<MODULEINFO> (MODULEINFO moduleInfo);
#else
	template<> UINT AFXAPI HashKey<MODULEINFO> (MODULEINFO moduleInfo);
#endif

typedef CMap<MODULEINFO,MODULEINFO,int,int> CModuleMap;

typedef struct _tagPROCESSINFO
{
	HMODULE hProcessModule;
	CModuleMap moduleMap;

	virtual ~_tagPROCESSINFO()
	{
		moduleMap.RemoveAll();
	}

} PROCESSINFO;

class CProcessModulesMap : public CMap<DWORD_PTR,DWORD_PTR,PROCESSINFO*,PROCESSINFO*> 
{
public:
	virtual ~CProcessModulesMap()
	{
		POSITION pos=GetStartPosition();
		while(pos!=NULL)
		{
			DWORD dwProcessID;
			PROCESSINFO* pProcessInfo;
			GetNextAssoc(pos,dwProcessID,pProcessInfo);
			ASSERT(pProcessInfo!=NULL);
			delete pProcessInfo;
		}

		RemoveAll();
	}
};




//==================== typedefs for ToolHelp32 functions =====================

typedef HANDLE (WINAPI* PFNCREATETOOLHELP32SNAPSHOT)
		(DWORD dwFlags, DWORD th32ProcessID);
    
typedef BOOL (WINAPI* PFNPROCESS32FIRST)
		(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
    
typedef BOOL (WINAPI* PFNPROCESS32NEXT)
		(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
    
typedef BOOL (WINAPI* PFNMODULE32FIRST)
		(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
    
typedef BOOL (WINAPI* PFNMODULE32NEXT)
		(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
    

//====================== typedefs for PSAPI.DLL functions =====================

typedef BOOL (WINAPI* PFNENUMPROCESSES)
		(DWORD* lpidProcess, DWORD cb, DWORD* cbNeeded);

typedef BOOL (WINAPI* PFNENUMPROCESSMODULES)
		(HANDLE hProcess, HMODULE *lphModule, DWORD cb, LPDWORD lpcbNeeded);

typedef DWORD (WINAPI* PFNGETMODULEFILENAMEEXA)
		(HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize);


/////////////////////////////////////////////////////////////////////////////
// COXDLLManager window

class OX_CLASS_DECL COXDLLManager
{
// Construction
public:
	COXDLLManager();
	COXDLLManager(LPCTSTR lpLibFileName);
	COXDLLManager(LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);

// Attributes
public:

protected:
	static BOOL m_bIsWindowsNT;

	HMODULE m_hModule;
	BOOL m_bWasAlreadyLoaded;
	BOOL m_bIsOwningModule;


// Operations
public:
	// static functions
	static BOOL IsWindowsNTRunning();
	static BOOL GetAllLoadedDLLs(CProcessModulesMap& mapProcessModules);
	static BOOL GetProcessLoadedDLLs(HINSTANCE hInstance, 
		CModuleMap& mapModules);

	inline static BOOL GetVersionInfo(HMODULE hModule, DLLVERSIONINFO* pdvi) 
	{
		COXDLLManager dll;
		if(!dll.Attach(hModule,FALSE))
			return FALSE;
		return dll.GetVersionInfo(pdvi);
	}
	
	inline static BOOL GetVersionInfo(LPCTSTR lpLibFileName, DLLVERSIONINFO* pdvi) 
	{
		COXDLLManager dll(lpLibFileName);
		if(!dll.IsLoaded())
			return FALSE;
		return dll.GetVersionInfo(pdvi);
	}

	inline static BOOL GetFileInfo(HMODULE hModule, DLLFILEINFO* pdfi) 
	{
		COXDLLManager dll;
		if(!dll.Attach(hModule,FALSE))
			return FALSE;
		return dll.GetFileInfo(pdfi);
	}
	inline static BOOL GetFileInfo(LPCTSTR lpLibFileName, DLLFILEINFO* pdfi) 
	{
		COXDLLManager dll(lpLibFileName);
		if(!dll.IsLoaded())
			return FALSE;
		return dll.GetFileInfo(pdfi);
	}
	
	inline static BOOL GetSnapshot(HMODULE hModule, DLLSNAPSHOT* pSnapshot) 
	{
		COXDLLManager dll;
		if(!dll.Attach(hModule,FALSE))
			return FALSE;
		return dll.GetSnapshot(pSnapshot);
	}
	
	inline static BOOL GetSnapshot(LPCTSTR lpLibFileName, DLLSNAPSHOT* pSnapshot) 
	{
		COXDLLManager dll(lpLibFileName);
		if(!dll.IsLoaded())
			return FALSE;
		return dll.GetSnapshot(pSnapshot);
	}

	inline static BOOL GetReferenceCount(HMODULE hModule, int& nRefCount) 
	{
		COXDLLManager dll;
		if(!dll.Attach(hModule,FALSE))
			return FALSE;
		return dll.GetReferenceCount(nRefCount);
	}

	inline static BOOL GetReferenceCount(LPCTSTR lpLibFileName, int& nRefCount) 
	{
		COXDLLManager dll(lpLibFileName);
		if(!dll.IsLoaded())
			return FALSE;
		return dll.GetReferenceCount(nRefCount);
	}

	inline static BOOL GetDependantProcesses(HMODULE hModule, 
		CStringArray& arrDependantProcesses) 
	{
		COXDLLManager dll;
		if(!dll.Attach(hModule,FALSE))
			return FALSE;
		return dll.GetDependantProcesses(arrDependantProcesses);
	}
	
	inline static BOOL GetDependantProcesses(LPCTSTR lpLibFileName, 
		CStringArray& arrDependantProcesses) 
	{
		COXDLLManager dll(lpLibFileName);
		if(!dll.IsLoaded())
			return FALSE;
		return dll.GetDependantProcesses(arrDependantProcesses);
	}
	///////////////////////////////////////

	inline BOOL IsLoaded() const { 
		return ((m_hModule!=NULL)&(m_bIsOwningModule)) ? TRUE : FALSE; }

	// counterparts of existing functions
	//
	HINSTANCE LoadLibrary(LPCTSTR lpLibFileName);
	HINSTANCE LoadLibraryEx(LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);

	BOOL FreeLibrary();

	inline void FreeLibraryAndExitThread(DWORD dwExitCode) 
	{
		ASSERT(IsLoaded());
		::FreeLibraryAndExitThread(m_hModule,dwExitCode);
	}
  
	inline HRSRC FindResource(LPCTSTR lpName, LPCTSTR lpType) const 
	{
		ASSERT(IsLoaded());
		return ::FindResource(m_hModule,lpName,lpType);
	}

	inline HRSRC FindResourceEx(LPCTSTR lpName, LPCTSTR lpType, WORD wLanguage) const 
	{
		ASSERT(IsLoaded());
		return ::FindResourceEx(m_hModule,lpName,lpType,wLanguage);
	}

	inline HGLOBAL LoadResource(HRSRC hResInfo) const 
	{
		ASSERT(IsLoaded());
		return ::LoadResource(m_hModule,hResInfo);
	}

	inline BOOL DisableThreadLibraryCalls() const 
	{
		ASSERT(IsLoaded());
		return ::DisableThreadLibraryCalls(m_hModule);
	}

	inline HMODULE GetModuleHandle() const { return m_hModule; }

	inline DWORD GetModuleFileName(LPTSTR lpFilename, DWORD nSize) const 
	{ 
		ASSERT(lpFilename!=NULL);
		return ::GetModuleFileName(m_hModule,lpFilename,nSize);
	}

	inline FARPROC GetProcAddress(LPCSTR lpProcName) const 
	{
		ASSERT(lpProcName!=NULL);
		ASSERT(IsLoaded());
		return ::GetProcAddress(m_hModule,lpProcName);
	}
	//////////////////////////////////////////
	

	BOOL Attach(HMODULE hModule, BOOL bUnloadOnDestroy);

	inline HMODULE Detach() 
	{
		HMODULE hModule=m_hModule;
		Reset();
		return hModule;
	}


	BOOL GetVersionInfo(DLLVERSIONINFO* pdvi) const;
	BOOL GetFileInfo(DLLFILEINFO* pdfi) const;

		
	BOOL GetSnapshot(DLLSNAPSHOT* pSnapshot) const;

	inline BOOL GetReferenceCount(int& nRefCount) const 
	{
		DLLSNAPSHOT snapShot;
		if(GetSnapshot(&snapShot))
		{
			nRefCount=snapShot.nRefCount;
			return TRUE;
		}
		else
			return FALSE;
	}

	inline BOOL GetDependantProcesses(CStringArray& arrDependantProcesses) const 
	{
		DLLSNAPSHOT snapShot;
		if(GetSnapshot(&snapShot))
		{
			arrDependantProcesses.RemoveAll();
			arrDependantProcesses.Append(snapShot.arrDependantProcesses);
			return TRUE;
		}
		else
			return FALSE;
	}

protected:
	inline void Reset() 
	{
		m_hModule=NULL;
		m_bWasAlreadyLoaded=FALSE;
		m_bIsOwningModule=TRUE;
	}

	CString	GetFileInfoValue(const LPBYTE pFileVersionInfo, WORD nLangID, 
		WORD nCharset, LPCTSTR lpKeyName) const;


// Implementation
public:
	virtual ~COXDLLManager();
};

/////////////////////////////////////////////////////////////////////////////
#endif	//	_DLLMANAGER_H
