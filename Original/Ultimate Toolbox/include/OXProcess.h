// ==========================================================================
//						   Class Specification : 
//					COXProcess & COXProcessIterator
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////


/*

DESCRIPTION

Process can be defined as an executing application that consists of a private 
virtual address space, code, data, and other operating system resources. 
A process also contains one or more threads that run in the context of the process. 

In most cases programmers don't care about process objects as long as every
application runs in its own space and not aware about existence of other applications.
But for some applications it is really important to get a global picture of all
processes running in the system. Applications that have to support comunicatation 
between different processes, applications that spawn child processes and have to 
control them, or sysatem diagnostic tools could benefit from knowing about
running processes in the system. 

In order to simplify the process of working with process objects in Windows 95/98/NT
we designed two classes: COXProcess and COXProcessIterator.

COXProcess class wraps subset of Windows API functions that allows you to retrieve
information about process. Process is uniquely identified throughout the system 
by its ID. This ID is returned to you when you create new process. 
By default COXProcess object is set to represent the current running process but
you can set explicitly the ID of the process you want to wrap by specifying it
as argument in COXProcess constructor.

Process can exit or be terminated at any time. COXProcess provides validation 
functionality so you can check if COXProcess object represent active process or not.

	IsValid();

As long as process is active (COXProcess object is valid) you can retrieve the
following information about it: 
	
	image file name (the name of executable of the application)
	priority class
	version of the system the process supposed to run on
	priority boost setting
	minimum and maximum workspace size
	creation time
	time spent in kernel and user modes

Refer to COXProcess function reference for details. 

Although COXProcess class in most parts is just a wrapper around corresponding 
Windows API functions it also uses PSAPI (for Windows NT) and 
Toolhelp32 (for Win 95/98) functions. The primary reason for that is the fact
that NT and 95/98 platforms doesn't have the same set of API that can be used 
in order to access all information about processes in the system. COXProcess
works both on NT and 95/98 so we had to explicitly link to corresponding
functions depending on the platform your application is running on.

As we mentioned above you can retrieve the ID of new process when you 
call CreateProcess function. But what if you want to keep an eye on all processes
in the system or need to retrieve info about the process which ID you don't know. 
In order to help in situations like this we designed COXProcessIterator class.


COXProcessIterator is derived from COXProcess class so at any time it acts as 
iterator and process object simulteneously. In order to iterate through
all processes in the system you have to take the following steps:

	1)	Instantiate COXProcessIterator object
		
			COXProcessIterator procIterator;

	2)	Initialize iteration process by calling MakeSnapshot function. Unless
		you call this function COXProcessIterator iteration routines will fail.

			procIterator.MakeSnapshot();

	3)	If return value of MakeSnapshot function is TRUE (that means that 
		snapshot of currently active processes was taken successfully) you can
		retrieve the number of processes in snapshot using function
			
			GetProcessCount();

		and call the following iteration functions:

			Next()
			Prev()
			operator++()
			operator--()
			operator+()
			operator-()
			operator+=()
			operator-=()


	Example:

		// populate array with all active processes image names
		//
		CStringArray arrProcessNames;
		COXProcessIterator procIterator;
		if(procIterator.MakeSnapshot())
		{
			CString sFileName;
			for(int nIndex=0; nIndex<procIterator.GetProcessCount(); nIndex++)
			{
				if(procIterator.GetImageFileName(sFileName))	
					arrProcessNames.Add(sFileName);
				++procIterator;
			}
		}
		





Refer to "SystemStateInfo" sample that can be found in 
.\Samples\Advanced\SystemStateInfo subdirectory of your Ultimate Toolbox Directory





Dependencies:

	#include "OXProcess.h"


Source code files:

	"OXProcess.cpp"

*/


#ifndef _OXPROCESS_H_
#define _OXPROCESS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"

#include <afxdisp.h>
#include <afxtempl.h>

#ifndef _INC_TOOLHELP32
#include "tlhelp32.h"
#define _INC_TOOLHELP32
#endif

#include "UTB64Bit.h"


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



class OX_CLASS_DECL COXProcess  
{
public:
	
	// constuctors
	//

	// --- In:		dwProcessID		-	process ID
	//				process			-	reference to COXProcess object to copy
	// --- Out:	
	// --- Returns:	
	// --- Effect:	Constructs the object. Default constructor will create the
	//				object that represent the current process
	COXProcess() { m_dwProcessID=GetCurrentProcessID(); }
	COXProcess(DWORD dwProcessID) { m_dwProcessID=dwProcessID; }
	COXProcess(const COXProcess& process) { m_dwProcessID=process.m_dwProcessID; }

	// --- In:		process			-	reference to COXProcess object to copy
	// --- Out:	
	// --- Returns:	
	// --- Effect:	Copy the contents of specified COXProcess object
	inline COXProcess& operator=(const COXProcess& process) 
	{ 
		if(this==&process)
			return *this;
		
		m_dwProcessID=process.m_dwProcessID; 
		return *this;
	}

	//
	/////////////////////////////


	// --- In:		
	// --- Out:	
	// --- Returns:	
	// --- Effect:	Destructs the object
	virtual ~COXProcess();


	// static functions
	//

	// --- In:		
	// --- Out:	
	// --- Returns:	Current process ID
	// --- Effect:	Retrieves the current process ID (unique throughout 
	//				the system).
	static DWORD GetCurrentProcessID();


	// --- In:		
	// --- Out:		arrEnvironmentString	-	reference to string array to which
	//											all environment strings will be
	//											copied 
	// --- Returns:	TRUE if success
	// --- Effect:	Retrieves the environment info for current process.
	static BOOL GetCurrentProcessEnvironment(CStringArray& arrEnvironmentString);


	// --- In:		
	// --- Out:		
	// --- Returns:	TRUE if current process is running under debugger
	// --- Effect:	Retrieves the flag that specifies whether the current 
	//				process is running under the context of a debugger.
	static BOOL IsDebuggerPresent();


	// --- In:		dwProcessID		-	process ID
	// --- Out:
	// --- Returns:	TRUE if specified ID is valid process ID
	// --- Effect:	Retrieves the flag that specifies whether given process ID 
	//				is valid or not
	static BOOL IsValidProcessID(DWORD dwProcessID);


	// --- In:		dwProcessID		-	process ID	
	// --- Out:		sFileName		-	reference to the string that will be 
	//									populated with filename of the process image
	// --- Returns:	TRUE if success, or FALSE otherwise
	// --- Effect:	Retrieves the filename of the process executable
	static BOOL GetProcessImageFileName(DWORD dwProcessID, CString& sFileName);

	//
	/////////////////////////////


	//	====================================================================


	// --- In:			
	// --- Out:		sFileName	-	reference to the string that will be 
	//								populated with filename of the process image
	// --- Returns:	TRUE if success, or FALSE otherwise
	// --- Effect:	Retrieves the filename of the process executable
	BOOL GetImageFileName(CString& sFileName) const;

	// --- In:		
	// --- Out:
	// --- Returns:	TRUE if COXProcess object represent existing process
	// --- Effect:	Retrieves the flag that specifies whether the object
	//				is valid or not
	inline BOOL IsValid() const { return IsValidProcessID(m_dwProcessID); }

	// --- In:		
	// --- Out:
	// --- Returns:	global system identificator of the process
	// --- Effect:	Retrieves the process ID
	inline DWORD GetProcessID() const { return m_dwProcessID; }

	// --- In:		
	// --- Out:		dwPriorityClass	-	reference to DWORD variable that 
	//									will be populated with process 
	//									priority class, can be one of the
	//									following:
	//
	//			HIGH_PRIORITY_CLASS		Indicates a process that performs 
	//									time-critical tasks that must be executed 
	//									immediately for it to run correctly. 
	//									The threads of a high-priority class process 
	//									preempt the threads of normal or idle priority 
	//									class processes. An example is the Task List, 
	//									which must respond quickly when called by the 
	//									user, regardless of the load on the operating 
	//									system. Use extreme care when using the 
	//									high-priority class, because a high-priority 
	//									class CPU-bound application can use nearly 
	//									all available cycles. 
	//
	//			IDLE_PRIORITY_CLASS		Indicates a process whose threads run only 
	//									when the system is idle and are preempted 
	//									by the threads of any process running in a 
	//									higher priority class. An example is a screen 
	//									saver. The idle priority class is inherited 
	//									by child processes. 
	//
	//			NORMAL_PRIORITY_CLASS	Indicates a normal process with no special 
	//									scheduling needs. 
	//
	//			REALTIME_PRIORITY_CLASS Indicates a process that has the highest 
	//									possible priority. The threads of a real-time 
	//									priority class process preempt the threads 
	//									of all other processes, including operating 
	//									system processes performing important tasks. 
	//									For example, a real-time process that executes 
	//									for more than a very brief interval can cause 
	//									disk caches not to flush or cause the mouse to 
	//									be unresponsive. 
	//
	// --- Returns:	TRUE if success, or FALSE otherwise
	// --- Effect:	Retrieves the process priority class
	BOOL GetPriorityClass(DWORD& dwPriorityClass) const;

	// --- In:		
	// --- Out:		dwVersion	-	reference to DWORD variable that 
	//								will be populated with the version of the system 
	//								on which the process expects to run. The high 
	//								word of the return value contains the major 
	//								version number. The low word of the return value 
	//								contains the minor version number. 
	// --- Returns:	TRUE if success, or FALSE otherwise
	// --- Effect:	Retrieves the version the system on which the process 
	//				expects to run
	BOOL GetVersion(DWORD& dwVersion) const;


	//	Only Windows NT
	//

	// --- In:		
	// --- Out:		bDisabled	-	reference to BOOL variable that will be
	//								populated with the flag that specifies 
	//								whether priority boost for the process is 
	//								disabled or not
	// --- Returns:	TRUE if success, or FALSE otherwise
	// --- Effect:	Retrieves the flag that specifies whether priority boost 
	//				for the process is disabled or not
	BOOL IsPriorityBoostDisabled(BOOL& bDisabled) const;

	// --- In:		
	// --- Out:		dwMinWorkingSetSize	-	reference to DWORD variable that 
	//										will be populated with process 
	//										minimum working set size. The virtual 
	//										memory manager attempts to keep at least 
	//										this much memory resident in the process 
	//										whenever the process is active.
	//				dwMaxWorkingSetSize	-	reference to DWORD variable that 
	//										will be populated with process 
	//										maximum working set size. The virtual 
	//										memory manager attempts to keep no more 
	//										than this much memory resident in the 
	//										process whenever the process is active 
	//										when memory is in short supply. 
	// --- Returns:	TRUE if success, or FALSE otherwise
	// --- Effect:	Retrieves the process minimum and maximum working set sizes
	BOOL GetMinMaxWorkingSetSize(DWORD& dwMinWorkingSetSize, 
		DWORD& dwMaxWorkingSetSize) const;

	// --- In:		
	// --- Out:		fileTime
	//				time
	//				oleTime		-	time variable of different types that 
	//								will be populated with process creation time
	// --- Returns:	TRUE if success, or FALSE otherwise
	// --- Effect:	Retrieves the process creation time
	BOOL GetCreationTime(FILETIME& fileTime) const;
	BOOL GetCreationTime(CTime& time) const;
	BOOL GetCreationTime(COleDateTime& oleTime) const;

	// --- In:		
	// --- Out:		fileTime
	//				time
	//				oleTime		-	time variable of different types that 
	//								will be populated with process exit time
	// --- Returns:	TRUE if success, or FALSE otherwise
	// --- Effect:	Retrieves the process exit time
	BOOL GetExitTime(FILETIME& fileTime) const;
	BOOL GetExitTime(CTime& time) const;
	BOOL GetExitTime(COleDateTime& oleTime) const;

	// --- In:		
	// --- Out:		timeSpent	-	time variable of different types that 
	//								will be populated with the amount of time 
	//								that the process has executed in kernel mode. 
	//								The time that each of the threads of the 
	//								process has executed in kernel mode is 
	//								determined, and then all of those times are 
	//								summed together to obtain this value
	//								(That is the number of 100-nanosecond units 
	//								in one second). 
	// --- Returns:	TRUE if success, or FALSE otherwise
	// --- Effect:	Retrieves the time the process spent in kernel mode
	BOOL GetKernelTime(DWORDLONG& timeSpent) const;

	// --- In:		
	// --- Out:		timeSpent	-	time variable of different types that 
	//								will be populated with amount of time that 
	//								the process has executed in user mode. 
	//								The time that each of the threads of the process 
	//								has executed in user mode is determined, and 
	//								then all of those times are summed together 
	//								to obtain this value (That is the number of 
	//								100-nanosecond units in one second).  
	// --- Returns:	TRUE if success, or FALSE otherwise
	// --- Effect:	Retrieves the time the process spent in user mode
	BOOL GetUserTime(DWORDLONG& timeSpent) const;

	//
	////////////////////////////////////////////////////////////////

protected:
	// initialize operation system specific functions
	BOOL InitializeProcessFunctions() const;

	// Attributes
protected:
	// process ID (global identifier of process throughout the system)
	DWORD m_dwProcessID;

	// static variable that is set to true if application is running on NT
	static BOOL m_bIsRunningNT;

	// static flag that is set to true when operation system specific 
	// functions has been initialized
	static BOOL m_bProcessFunctionsInitialized;
	// handle to the library file that is loaded in to the application space
	// and provides helper functions to operate on processes
	static HINSTANCE m_hProcessHelperLib;
	// PSAPI functions
	static PFNENUMPROCESSES pfnEnumProcesses;
	static PFNENUMPROCESSMODULES pfnEnumProcessModules;
	static PFNGETMODULEFILENAMEEXA pfnGetModuleFileNameExA;
	// Toolhelp32 functions
	static PFNCREATETOOLHELP32SNAPSHOT pfnCreateToolhelp32Snapshot;
	static PFNPROCESS32FIRST pfnProcess32First;
	static PFNPROCESS32NEXT pfnProcess32Next;
	static PFNMODULE32FIRST pfnModule32First;
	static PFNMODULE32NEXT pfnModule32Next;
};


///////////////////////////////////////////////////////////////////////////


class OX_CLASS_DECL COXProcessIterator : public COXProcess
{
public:
	// --- In:		
	// --- Out:	
	// --- Returns:	
	// --- Effect:	Constructs the object
	COXProcessIterator() { Cleanup(); };

	// --- In:		
	// --- Out:	
	// --- Returns:	
	// --- Effect:	Destructs the object
	virtual ~COXProcessIterator() { Cleanup(); }

	// --- In:		processIterator	-	reference to COXProcessIterator
	//									object to copy
	// --- Out:	
	// --- Returns:	
	// --- Effect:	Copy the contents of specified COXProcessIterator object
	inline COXProcessIterator& operator=(const COXProcessIterator& processIterator) 
	{ 
		if(this!=&processIterator)
		{
			m_dwProcessID=processIterator.m_dwProcessID; 
			m_bCanIterate=processIterator.m_bCanIterate; 
			m_nCurrentIndex=processIterator.m_nCurrentIndex; 
			m_arrProcesses.Copy(processIterator.m_arrProcesses);
		}
		return *this;
	}


	// --- In:			
	// --- Out:		
	// --- Returns:	TRUE if success, or FALSE otherwise
	// --- Effect:	Make snapshot of all running processes in the system.
	//				You have to call this function before starting the process 
	//				of iteration. Note, that there is possibility that after 
	//				you've made a snapshot of all processes a process can be 
	//				closed. So you have to validate the process that you retrive 
	//				on iteration step.
	BOOL MakeSnapshot();

	// --- In:			
	// --- Out:		
	// --- Returns:	TRUE if iteration of processes can be accomplished, 
	//				or FALSE otherwise
	// --- Effect:	Retrieves the flag that specifies whether the object
	//				is properly initialized in order to iterate processes  
	inline BOOL CanIterate() const { return m_bCanIterate; }

	// --- In:			
	// --- Out:		
	// --- Returns:	number of running processes at the moment the last
	//				snapshot was taken, or -1 if object wasn't properly
	//				initialized
	// --- Effect:	Retrieves the number of running processes at the moment 
	//				the last snapshot was taken
	inline int GetProcessCount() const 
	{
		if(!CanIterate())
			return -1;
		else
			return PtrToInt(m_arrProcesses.GetSize()); 
	}

	// --- In:		nOffset	-	number of processes to iterate forward
	// --- Out:		
	// --- Returns:	TRUE if iteration step was successful or FALSE otherwise
	// --- Effect:	Iterate to next process in the list of processes running  
	//				at the moment when the last snapshot was taken
	BOOL Next(UINT nOffset=1);

	// --- In:		nOffset	-	number of processes to iterate backward
	// --- Out:		
	// --- Returns:	TRUE if iteration step was successful or FALSE otherwise
	// --- Effect:	Iterate to previous process in the list of processes running  
	//				at the moment when the last snapshot was taken
	BOOL Prev(UINT nOffset=1);

	// --- In:		
	// --- Out:	
	// --- Returns:	
	// --- Effect:	Prefix increment operator (iterate to next process in snapshot)
	inline COXProcessIterator& operator++()
	{
		Next();
		return *this; 
	}

	// --- In:		
	// --- Out:	
	// --- Returns:	
	// --- Effect:	Prefix decrement operator (iterate to previous process in snapshot)
	inline COXProcessIterator& operator--()
	{
		Prev();
		return *this; 
	}

	// --- In:		nOffset	-	number of processes to iterate
	// --- Out:	
	// --- Returns:	
	// --- Effect:	Addition operator (iterate in list of process in snapshot)
	inline COXProcessIterator& operator+(int nOffset)
	{
		if(nOffset>0)
			Next(nOffset);
		else if(nOffset>0)
			Prev(-nOffset);
		return *this; 
	}

	// --- In:		nOffset	-	number of processes to iterate
	// --- Out:	
	// --- Returns:	
	// --- Effect:	Subtraction operator (iterate in list of process in snapshot)
	inline COXProcessIterator& operator-(int nOffset)
	{
		if(nOffset>0)
			Prev(nOffset);
		else if(nOffset>0)
			Next(-nOffset);
		return *this; 
	}

	// --- In:		nOffset	-	number of processes to iterate
	// --- Out:	
	// --- Returns:	
	// --- Effect:	Addition assignment operator (iterate in list of 
	//				process in snapshot)
	inline COXProcessIterator& operator+=(int nOffset)
	{
		if(nOffset>0)
			Next(nOffset);
		else if(nOffset>0)
			Prev(-nOffset);
		return *this; 
	}

	// --- In:		nOffset	-	number of processes to iterate
	// --- Out:	
	// --- Returns:	
	// --- Effect:	Subtraction assignment operator (iterate in list of 
	//				process in snapshot)
	inline COXProcessIterator& operator-=(int nOffset)
	{
		if(nOffset>0)
			Prev(nOffset);
		else if(nOffset>0)
			Next(-nOffset);
		return *this; 
	}


protected:

	// cleans up the object previous snapshot and prepares the object to 
	// take another one
	inline void Cleanup() 
	{ 
		m_bCanIterate=FALSE;
		m_nCurrentIndex=-1;
		m_arrProcesses.RemoveAll();
		m_dwProcessID=0xffffffff;
	}

	// Attributes
protected:
	// flag that specifies if object is properly initialized to iterate through
	// processes
	BOOL m_bCanIterate;
	// current iteration index in the list of all processes
	int m_nCurrentIndex;
	// array that used to save info about all process at the time when 
	// last process snapshot was taken
	CArray<DWORD,DWORD> m_arrProcesses;
};

#endif // !defined(_OXPROCESS_H_)
