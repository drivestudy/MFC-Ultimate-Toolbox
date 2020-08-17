// ==========================================================================
// 					Class Specification : COXSysInfo
// ==========================================================================
//
// Version: 9.3
//
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
//
// //////////////////////////////////////////////////////////////////////////
//	Properties:
//
//	NO		Abstract class (does not have any objects)
//	YES		Derived from CObject
//	NO		Is a CWnd
//	NO		Two stage creation (constructor & Create())
//	NO		Has a message map
//	NO		Needs a resource template
//
//	NO		Persistant objects
//	NO		Uses exceptions

// //////////////////////////////////////////////////////////////////////////

/*

DESCRIPTION


The COXSysInfo class is an easy way to have an application query system information.

COXSysInfo provides a number of function that allow you to retrieve different 
useful system information in following areas:


	COMPUTER/NETWORK

	1) Computer name
	2) User name
	3) Domain name (NT only)
	4) Network Interface Card (NIC) address



	COMMUNICATION

	1) Primary IP addres
	2) List of IP addresses for multihomed computer



	WINDOWS

	1) Version, build number, platform
	2) Distinguishes NT from 95/98 and NT Server from NT Workstation, detects OSR2
	3) Retrieves Windows, System and Temp directories


	
	DISKDRIVES

	1) Type of any drive and type of file system installed on each
	2) Volume name, serial number, total space, free space for any drive



	DISPLAY

	1) Current screen resolution and color depth
	2) Max available screen resolution and color depth
	3) List of all display modes (unique combination of screen resolution 
	and color depth)
	4) Retrieves flag that specifies whether small or large fonts are 
	currently used in the sytem



	MEMORY

	1) Total and free physical memeory
	2) Total and free page file size
	3) Total and free virtual memeory



	RESOURCES

	1) Detects the percentage of free system, GDI and user resources 
	(makes sense only for Win 95/98)



	CPU

	1) Retrieves number of processors in system
	2) Type and speed of processor
	3) Retrieves flag that specifies presence of CoProcessor, MMX-technology
	4) Processor time stamp



	MODEM

	1) Retrieves number of modems in system
	2) For every modem installed in the system retrieves port on which it installed,
	desciption, manufacrurer and model info



	PRINTER

	1) Retrieves default printer name
	2) Retrieves names of all printers installed in the system
	3) Every printer settings: name, server, port, driver name, 
	environment (paper size and orientation, printer's DPI)
	4) Retrieves flags that specify whether printer is local or network one,
	and whether it is shared or not.



Refer to the COXSysInfo class function reference for details. And also 
it's good idea to take look at the "SystemStateInfo" sample where we show 
how to use COXSysInfo class to its full extent. The sample can be found
in .\Samples\Advanced\SystemStateInfo subdirectory of your Ultimate Toolbox
directory.

  



Dependencies:

	#include "OXProcess.h"


Source code files:

	"OXProcess.cpp"


Libraries:
	In order to use COXSysInfo class in your project you have to link the project
	to Cpuinfr.lib (Release version) and Cpuinfd.lib (Debug version) these libraries 
	can be found in .\Lib\libs subdirectory of your Ultimate Toolbox directory

*/

#ifndef __OXSYSINFO_H__
#define __OXSYSINFO_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

#include <shellapi.h>
#include <winspool.h>

// includes file for CPU speed functions
extern "C"
{
#include "Cpuid.h"
#include "Speed.h"
}

#include "UTB64Bit.h"

// Additional processor type defines:

#define PROCESSOR_UNKNOWN				-1
#define PROCESSOR_MIPSR4000				4000
#define PROCESSOR_ALPHA21064			21064
#define PROCESSOR_ALPHA21066			21066
#define PROCESSOR_ALPHA21164			21164
#define PROCESSOR_PPC601				601
#define PROCESSOR_PPC603				603
#define PROCESSOR_PPC604				604
#define PROCESSOR_PPC603PLUS			606
#define PROCESSOR_PPC604PLUS			609
#define PROCESSOR_PPC620				620
#define PROCESSOR_INTEL_PENTIUMPRO		626
#define PROCESSOR_INTEL_PENTIUMCELERON	646
#define PROCESSOR_INTEL_PENTIUM2		686
#define PROCESSOR_INTEL_PENTIUM3		786


// CONSTANT DEFINITIONS ////////////////////////////////////////
#define CLONE_MASK		0x8000	// Mask to be 'OR'ed with proc-
								//   essor family type

enum CPUSpeed 
{
	CPUSPEED_16=16,
	CPUSPEED_20=20,
	CPUSPEED_25=25,
	CPUSPEED_33=33,
	CPUSPEED_40=40,
	CPUSPEED_50=50,
	CPUSPEED_60=60,
	CPUSPEED_66=66,
	CPUSPEED_75=75,
	CPUSPEED_80=80,
	CPUSPEED_90=90,
	CPUSPEED_100=100,
	CPUSPEED_120=120,
	CPUSPEED_133=133,
	CPUSPEED_150=150,
	CPUSPEED_166=166,
	CPUSPEED_180=180,
	CPUSPEED_200=200,
	CPUSPEED_233=233,
	CPUSPEED_266=266,
	CPUSPEED_300=300,
	CPUSPEED_333=333,
	CPUSPEED_350=350,
	CPUSPEED_400=400,
	CPUSPEED_450=450,
	CPUSPEED_500=500,
	CPUSPEED_533=533,
	CPUSPEED_550=550,
	CPUSPEED_600=600,
	CPUSPEED_650=650,
	CPUSPEED_667=667,
	CPUSPEED_700=700,
	CPUSPEED_733=733,
	CPUSPEED_750=750,
	CPUSPEED_800=800,
};


typedef struct _tagNICADDRESS
{
	BYTE data[6];

} NICADDRESS, FAR* LPNICADDRESS;


// determine number of elements in an array (not bytes)
#define countof(array) (sizeof(array)/sizeof(array[0]))


typedef struct _tagMODEMINFO
{
	// number of COM port on which modem is installed
	int nPort;
	// description of modem driver
	CString sDriverDesc;
	// manafacturer info
	CString sManufacturer;
	// model info
	CString sModel;

    // assignment operator
    _tagMODEMINFO& operator=( const _tagMODEMINFO& mi )
	{
		if(this==&mi)
			return *this;
		
		nPort=mi.nPort;
		sDriverDesc=mi.sDriverDesc;
		sManufacturer=mi.sManufacturer;
		sModel=mi.sModel;
		return *this;
	}

} MODEMINFO, FAR* LPMODEMINFO;

typedef CArray<MODEMINFO,MODEMINFO&> CArrayModemInfo;


typedef struct _tagDISPLAYMODE
{
	// color depth (number of bits used to save info about pixel color) 
	DWORD dwBitsPerPixel;
	// color depth (number of colors)
	DWORDLONG dwNumColors;
	// horizontal screen resolution
	DWORD dwHorzResolution;
	// vertical screen resolution
	DWORD dwVertResolution;

    // Copy constructor
    _tagDISPLAYMODE& operator=( const _tagDISPLAYMODE& dm )
	{
		if(this==&dm)
			return *this;
		
		dwBitsPerPixel=dm.dwBitsPerPixel;
		dwNumColors=dm.dwNumColors;
		dwHorzResolution=dm.dwHorzResolution;
		dwVertResolution=dm.dwVertResolution;
		return *this;
	}

} DISPLAYMODE, FAR* LPDISPLAYMODE;

typedef CArray<DISPLAYMODE,DISPLAYMODE&> CArrayDisplayMode;


int _cdecl DevModeCompare(const void *elem1, const void *elem2);


enum RESOURCETYPE
{
	// system resources
	RT_SYSTEM=0,
	// GDI resources
	RT_GDI=1,
	// USER resources
	RT_USER=2
};

typedef struct _tagSYSHEAPINFO
{
	DWORD dwSize;
	int nUserFreePercent;
    int nGDIFreePercent;
	int nUserSegment;
    int nGDISegment;

	_tagSYSHEAPINFO()
	{
		::ZeroMemory(this,sizeof(*this));
		dwSize=sizeof(*this);
	}

} SYSHEAPINFO;


///////////////////////////////////////////////////////////


class OX_CLASS_DECL COXSysInfo : public CObject
{
DECLARE_DYNAMIC(COXSysInfo);

//	Data members --------------------------------------------
public:

protected:
	MEMORYSTATUS	m_memStatus;
	SYSTEM_INFO		m_sysInfo;


//	Member functions --------------------------------------------
public:
	//	--- In:
	//	--- Out:
	//	--- Returns:	
	//	---	Effect: Constructor
	COXSysInfo();

	//	--- In:
	//	--- Out:
	//	--- Returns:	
	//	---	Effect: Destructor
	virtual ~COXSysInfo();	

	// ******** Computer/Network ********
	//

	//	--- In:			
	//	--- Out:		psComputerName	-	Name of local computer
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the name of the local computer
	BOOL	GetComputerName(CString *psComputerName) const;

	//	--- In:			
	//	--- Out:		psUserName		-	Name of current user
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the name of the current user. Works on Win95
	//					and Win NT
	BOOL	GetUserName(CString *psUserName) const;
	
	//	--- In:			
	//	--- Out:		psUserName		-	Name of current user
	//					psDomainName	-	Name of current domain logged on to
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the name of the current user and the domain. 
	//					Works only on Win NT.
	BOOL	GetUserAndDomainName(CString *psUserName, CString *psDomainName) const;

	//	--- In:			
	//	--- Out:		pNICAddress		-	valid pointer to Network
	//										Interface Card (NIC) address structure
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the NIC address
	BOOL	GetNICAddress(LPNICADDRESS pNICAddress) const;



	// *********	COMMUNICATION	***********
	//

	//	--- In:			pszHostName		-	Host name for which primary 
	//										IP address will be retrieved
	//	--- Out:		psIPAddress		-	Current IP address (0.0.0.0 if undefined)
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the current network IP address
	BOOL	GetPrimaryIPAddress(CString* psIPAddress, LPCSTR pszHostName=NULL) const;

	//	--- In:			bPrimary		-	TRUE if only primary IP must be retrieved
	//					pszHostName		-	Host name for which IP address 
	//										will be retrieved
	//	--- Out:		psIPAddressList	-	List of IP addresses
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the list of network IP address of a 
	//					multi-homed computer
	BOOL	GetListIPAddresses(CStringArray* psIPAddressList, 
		BOOL bPrimary = FALSE, LPCSTR pszHostName=NULL) const;



	// ******** Windows ********
	//

	//	--- In:			
	//	--- Out:		pdwPlatform		-	Current Windows platform
	//					pdwMajor		-	Major OS version
	//					pdwMinor		-	Minor OS version
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the current Windows OS version
	BOOL	GetWindowsVersion(DWORD *pdwPlatform, DWORD *pdwMajor, 
		DWORD *pdwMinor) const;

	//	--- In:			
	//	--- Out:		pdwBuildNumber	-	Windows platform build number
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the current Windows OS version build number
	BOOL	GetWindowsBuildNumber(DWORD *pdwBuildNumber) const;

	//	--- In:			
	//	--- Out:		sPlatformInfo	-	additional information about 
	//										the operating system
	//	---	Effect:		Retrieves additional information for current 
	//					Windows OS version
	BOOL	GetWindowsPlatformInfo(CString& sPlatformInfo) const;

	//	--- In:			
	//	--- Out:		pbResult		-	TRUE if Windows NT running
	//	--- Returns:	TRUE if success
	//	---	Effect:		Determines if the current Windows OS 
	//					version is NT 
	BOOL	IsNT(BOOL *pbResult) const;

	//	--- In:			
	//	--- Out:		pbResult		-	TRUE if Windows NT Server
	//										installed
	//	--- Returns:	TRUE if success
	//	---	Effect:		Determines if the current Windows OS 
	//					version is NT Server or not
	BOOL	IsNTServer(BOOL *pbResult) const;

	//	--- In:			
	//	--- Out:		pbResult		-	TRUE if Windows 95 OSR 2 
	//										installed
	//	--- Returns:	TRUE if success
	//	---	Effect:		Determines if the current Windows OS 
	//					version is Windows 95 OSR 2 or not
	BOOL	IsOSR2(BOOL *pbResult) const;

	//	--- In:			
	//	--- Out:		psWinDir		-	Windows directory
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the Windows directory
	BOOL	GetWindowsDir(CString *psWinDir) const;

	//	--- In:			
	//	--- Out:		psSysDir		-	System directory
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the System directory
	BOOL	GetSystemDir(CString *psSysDir) const;

	//	--- In:			
	//	--- Out:		psTempDir		-	Temp directory
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the temp directory
	BOOL	GetTempDir(CString *psTempDir) const;



	// ******** files ********
	//

	//	--- In:			pszFileName		-	name of the file
	//					bSmall			-	set to TRUE if small icon 
	//										is requested
	//					bSelected		-	set to TRUE if icon in selected state 
	//										is requested
	//					bFileMustExist	-	set to FALSE to retrieve an icon without 
	//										checking if file exist
	//	--- Out:		
	//	--- Returns:	A handle to requested icon if successful; otherwise NULL 
	//	---	Effect:		Retrieves the icon associated with specified file. 
	//					It is caller responsibility to eventually destroy the
	//					returned icon object using ::DestroyIcon() function
	HICON GetFileIcon(
		LPCTSTR pszFileName, BOOL bSmall, BOOL bSelected, BOOL bFileMustExist) const;

	//	--- In:			pszFileExt		-	file extension
	//					bSmall			-	set to TRUE if small icon 
	//										is requested
	//					bSelected		-	set to TRUE if icon in selected state 
	//										is requested
	//	--- Out:		
	//	--- Returns:	A handle to requested icon if successful; otherwise NULL 
	//	---	Effect:		Retrieves the icon associated with specified file extension. 
	//					It is caller responsibility to eventually destroy the
	//					returned icon object using ::DestroyIcon() function
	HICON GetFileExtIcon(LPCTSTR pszFileName, BOOL bSmall, BOOL bSelected) const;



	// ******** disk drives ********
	//

	//	--- In:			nDrive			-	drive, acceptable values: 0-25 (A-Z)
	//	--- Out:		psFileSysType	-	File system type
	//					pnDiskType		-	Disk type
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves disk drive information
	BOOL	GetDriveTypeInfo(int nDrive, CString *psFileSysType, 
		int *pnDiskType) const;

	//	--- In:			nDrive			-	drive, acceptable values: 0-25 (A-Z)
	//	--- Out:		psVolumeName	-	Disk volume name
	//					pdwVolSer		-	Disk serial number
	//					pdwTotalSpace	-	Total space bytes
	//					pdwFreeSpace	-	Total free space bytes
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves disk drive volume information
	BOOL	GetDriveVolumeInfo(int nDrive, CString *psVolumeName, DWORD *pdwVolSer, 
		DWORDLONG *pdwTotalSpace, DWORDLONG *pdwFreeSpace) const;



	// ********	display	*********
	//

	//	--- In:			
	//	--- Out:		xRes			-	horizontal screen resolution (in pixels)
	//					yRes			-	vertical screen resolution (in pixels)
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the display resolution size
	BOOL	GetDisplayResolution(int *pnxRes, int *pnyRes) const;

	//	--- In:			
	//	--- Out:		pdwNumColors	-	Number of colors used by current display
	//					pnNumBits		-	Number of bits for color depth
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the display color usage
	BOOL	GetDisplayNumColors(DWORDLONG *pdwNumColors, int *pnNumBits) const;

	//	--- In:			
	//	--- Out:		parrDisplayMode	-	pointer to array of all possible 
	//										combinations of screen resolution and 
	//										color depth
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves all possible display modes
	BOOL	GetAllDisplayModes(CArrayDisplayMode* parrDisplayMode) const;

	//	--- In:			
	//	--- Out:		xRes			-	max horizontal screen resolution (in pixels)
	//					yRes			-	max vertical screen resolution (in pixels)
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the max display resolution size
	BOOL	GetDisplayMaxResolution(int *pnxRes, int *pnyRes) const;

	//	--- In:			
	//	--- Out:		pdwNumColors	-	Max number of colors used 
	//										by current display
	//					pnNumBits		-	Max number of bits for color depth
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the display max available color usage
	BOOL	GetDisplayMaxNumColors(DWORDLONG *pdwNumColors, int *pnNumBits) const;

	//	--- In:			
	//	--- Out:		pbIsSmall		-	if TRUE then small fonts are used,
	//										otherwise the large ones
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the flag that specifies whether small or
	//					large fonts are used in the system
	BOOL	IsSmallFont(BOOL* pbIsSmall) const;



	// ******** memory ********
	//

	//	--- In:			
	//	--- Out:		pdwPhysMem		-	Total physical RAM
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the amount of physical memory
	BOOL	GetTotalPhysicalMemory(DWORD_PTR *pdwPhysMem);

	//	--- In:			
	//	--- Out:		pdwFreeMem		-	Total free physical RAM
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the amount of free physical memory
	BOOL	GetFreePhysicalMemory(DWORD_PTR *pdwFreeMem);

	//	--- In:			
	//	--- Out:		pdwTotalPageFile-	Total page file size
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the size of the page file
	BOOL	GetTotalPageFile(DWORD_PTR *pdwTotalPageFile);

	//	--- In:			
	//	--- Out:		pdwFreePageFile	-	Total free page file size
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the free space of the page file
	BOOL	GetFreePageFile(DWORD_PTR *pdwFreePageFile);

	//	--- In:			
	//	--- Out:		pdwTotalVirtual	-	Total virtual memory
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the size of virtual memory
	BOOL	GetTotalVirtual(DWORD_PTR *pdwTotalVirtual);

	//	--- In:			
	//	--- Out:		pdwFreeVirtual	-	Total free virtual memory
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the size of the free virtual memory
	BOOL	GetFreeVirtual(DWORD_PTR *pdwFreeVirtual);



	// ******** resources ********
	//

	//	--- In:			resourceType	-	type of requested resource:
	//
	//							RT_SYSTEM		system
	//							RT_GDI			GDI
	//							RT_USER			user
	//
	//	--- Out:		pnFreeResources	-	percentage of free system resources
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the percentage of free system resources
	BOOL	GetFreeSystemResources(int *pnFreeResources, 
		enum RESOURCETYPE resourceType) const;



	// ******** CPU ********
	//

	//	--- In:			
	//	--- Out:		pnNumProcessors		-	Total number of processors on computer
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the number of CPUs
	BOOL	GetNumProcessors(int *pnNumProcessors);

	//	--- In:			
	//	--- Out:		pdwProcessorType	-	Processor type, one of the following:
	//
	//									PROCESSOR_UNKNOWN
	//									PROCESSOR_INTEL_386
	//									PROCESSOR_INTEL_486
	//									PROCESSOR_INTEL_PENTIUM
	//									PROCESSOR_INTEL_PENTIUM2
	//									PROCESSOR_MIPS_R4000
	//									PROCESSOR_ALPHA_21064
	//									PROCESSOR_MIPSR4000
	//									PROCESSOR_ALPHA21064
	//									PROCESSOR_ALPHA21066
	//									PROCESSOR_ALPHA21164
	//									PROCESSOR_PPC601
	//									PROCESSOR_PPC603
	//									PROCESSOR_PPC604
	//									PROCESSOR_PPC603PLUS
	//									PROCESSOR_PPC604PLUS
	//									PROCESSOR_PPC620
	//
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the processor type (primary processor)
	BOOL	GetProcessorType(DWORD *pdwProcessorType);

	//	--- In:			
	//	--- Out:		pbResult			-	TRUE if CoProcessor is present, 
	//											or FALSE otherwise
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the presense of coprocessor
	BOOL	IsCoProcessorPresent(BOOL* pbResult);

	//	--- In:			nIndex				-	the zero-based index of processor
	//	--- Out:		pdwProcessorSpeed	-	Processor speed (in MHz)
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the processor speed
	BOOL	GetProcessorSpeed(DWORD* pdwProcessorSpeed, int nIndex=0) const;

	//	--- In:			
	//	--- Out:		pdwHigh		-	Upper 32-bits of Time Stamp (Register value)
	//					pdwLow		-	Lower 32-bits of Time Stamp (Register value)
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the processor time stamp
	BOOL	GetTimeStamp(DWORD *pdwHigh, DWORD *pdwLow) const;

	//	--- In:			
	//	--- Out:		pbMMX		-	TRUE if primary processor is MMX(TM) technology 
	//									processor, or FALSE otherwise
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the presense of MMX processor
	BOOL	IsMMXProcessor(BOOL* pbMMX);

	//	--- In:			
	//	--- Out:		pbOverdrive	-	TRUE if primary processor is Overdrive 
	//									processor, or FALSE otherwise
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the flag that specifies if primary processor
	//					is Overdrive or not
	BOOL	IsOverdriveProcessor(BOOL* pbOverdrive);



	// ******** Modem ********
	//

	//	--- In:			
	//	--- Out:		pnNumModems		-	Total number of modems installed
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the number of modems installed
	BOOL	GetNumModems(int *pnNumModems) const;

	//	--- In:			
	//	--- Out:		parrModemInfo	-	pointer to valid array of MODEMINFO 
	//										structures that provide information about 
	//										all installed modems
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the description of all installed modems
	BOOL	GetModemInfo(CArrayModemInfo* parrModemInfo) const;



	// *******	Printer	******
	//

	//	--- In:			
	//	--- Out:		sPrinterName	-	reference to string to which the
	//										name of the default printer will be 
	//										copied. If it is empty then there is
	//										no default printers in the system
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the name of the default printer
	BOOL GetDefaultPrinterName(CString& sPrinterName) const; 

	//	--- In:			
	//	--- Out:		arrPrinterName	-	reference to string array to which the
	//										name of found printers will be copied. 
	//										If it is empty then there is no 
	//										printers found
	//									
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the name of all printers in the system.
	BOOL GetAllPrintersName(CStringArray& arrPrinterName) const;

	//	--- In:			sPrinterName	-	reference to string which is the
	//										name of installed printer.
	//	--- Out:		lpPrinterInfo	-	reference to the pointer to 
	//										PRINTER_INFO_2 structure 
	//										that will be populated with data as
	//										result of execution of this function.
	//										You shouldn't allocate any memory.
	//										This function will allocate all 
	//										necessary memory. The caller will 
	//										be responsible for freeing that memory.
	//										Use GlobalFree() function in order to 
	//										do that
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the printer info for specified printer
	//					installed in the system
	BOOL GetPrinterInfo(const CString& sPrinterName, 
		LPPRINTER_INFO_2& lpPrinterInfo) const;

	//	--- In:			sPrinterName	-	reference to string which is the
	//										name of installed printer.
	//	--- Out:		pbLocalPrinter	-	TRUE if specified printer is local,
	//										or FALSE otherwise
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the flag that specify whether the printer 
	//					is local or not
	BOOL IsLocalPrinter(const CString& sPrinterName, BOOL* pbLocalPrinter) const;

	//	--- In:			sPrinterName	-	reference to string which is the
	//										name of installed printer.
	//	--- Out:		pbSharedPrinter	-	TRUE if specified printer is shared,
	//										or FALSE otherwise
	//	--- Returns:	TRUE if success
	//	---	Effect:		Retrieves the flag that specify whether the printer 
	//					is shared or not
	BOOL IsSharedPrinter(const CString& sPrinterName, BOOL* pbSharedPrinter) const;


protected:
	// Retrieves memory status
	void GetMemStatus();
	// Retrieves info about display capabilities
	int DeviceCaps(int iIndex) const;
	// Retrives general sys info
	void GetSysInfo();
	// Helper function to retrieve value form registry
	LPTSTR GetRegistryString(HKEY hKeyClass, LPTSTR lpszSubKey, 
		LPTSTR lpszValueName) const;
};

#endif	// __OXSYSINFO_H__
