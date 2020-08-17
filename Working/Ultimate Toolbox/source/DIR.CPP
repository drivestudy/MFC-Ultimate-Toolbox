// ==========================================================================
// 							Class Implementation : COXDirSpec
// ==========================================================================

// Source file : dir.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "dir.h"		// class specification
#include "path.h"		// For MakeUnique() and FindLowerDirectory()

#include "returns.h"	// internal return codes

#ifndef NOT_REGISTERED
#include "copytree.h"	// For copying of entire dir trees
#include "cpystdlg.h"	// For displaying info while copying dir trees
#endif // !NOT_REGISTERED

#include <direct.h>		// For directory functions (getdcwd(), ...)
#include <stdlib.h>		// For constant definitions  (_MAX_DIR, ...)
#include <dos.h>		// For _find_t, _dos_findfirst, ...

#include "UTB64Bit.h"

#define _A_ALL   _A_ARCH | _A_HIDDEN | _A_NORMAL | _A_RDONLY | _A_SYSTEM

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif                                  

#include "filelmt.h"	// for invalid chars

IMPLEMENT_SERIAL(COXDirSpec, CObject, 0)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members


// Data members -------------------------------------------------------------
// protected:

// private:
	// CString m_sDrive;
	// --- The drive specification consists of a drive letter (capital letter)
	//	   followed be a colon

	//CString m_sSubdirectory;
	// --- A subdirecory consists of the directory specification, without the drive
	//     and file specification
	//     In case of the root directory, it ends in a back slash, 
	//	   otherwise it never ends in a back slash
	
// Member functions ---------------------------------------------------------
// public:

COXDirSpec::COXDirSpec()
	:
	m_sDrive(_T("")),
	m_sSubdirectory(_T(""))
	{
	}
	
COXDirSpec::COXDirSpec(LPCTSTR pszDirectory)
	{
	// ... Must be valid pointer
	ASSERT(pszDirectory != NULL);
	if (!SetDirectory(pszDirectory))
		{
		TRACE(_T("COXDirSpec::COXDirSpec : An invalid directory (%s) was specified, clearing object\n"),
			pszDirectory);
		Empty();
		}
	}
	
COXDirSpec::COXDirSpec(const COXDirSpec& dirSrc)
	:
	m_sDrive(dirSrc.m_sDrive),
	m_sSubdirectory(dirSrc.m_sSubdirectory)
	{
	}
	
BOOL COXDirSpec::AppendDirectory(const COXDirSpec& dirSecond)
	{
	// The drive spec of dirSecond must be empty or the same as
	// that of *this
	if (!dirSecond.GetDrive().IsEmpty() && (GetDrive() != dirSecond.GetDrive()))
		{
		TRACE(_T("COXDirSpec::AppendDirectory : Illegal drive spec (%s != %s)\n"),
			(LPCTSTR)GetDrive(), (LPCTSTR)dirSecond.GetDrive());
		return FALSE;
		}
		
	if (dirSecond.GetSubdirectory().IsEmpty())
		return TRUE;
		
	if (dirSecond.GetSubdirectory().Left(1) == _T("\\"))
		{
		TRACE(_T("COXDirSpec::AppendDirectory : Second dir cannot start with back slash (%s)\n"),
			dirSecond.GetSubdirectory());
		return FALSE;
		}
		
	CString sSubdir = GetSubdirectory();
	if (!sSubdir.IsEmpty() && (sSubdir.Right(1) != _T("\\")))
		sSubdir += _T("\\");
	return SetSubdirectory(sSubdir += dirSecond.GetSubdirectory());
	}
	
COXDirSpec& COXDirSpec::operator=(const COXDirSpec& dirSrc)
{
	if(this==&dirSrc)
		return *this;
		
	m_sDrive = 			dirSrc.m_sDrive;
	m_sSubdirectory = 	dirSrc.m_sSubdirectory;
	return *this;
}
	
CString COXDirSpec::GetDrive() const
{
	return m_sDrive;
}
	
BOOL COXDirSpec::SetDrive(LPCTSTR pszDrive)
	{
	// ... Must be valid pointer
	ASSERT(pszDrive != NULL);
	if (_tcsclen(pszDrive) == 0)
		{
		m_sDrive = pszDrive;
		return TRUE;
		}
	ASSERT(_tcsclen(pszDrive) != 0);
	if ( ((_T('a') <= pszDrive[0]) && (pszDrive[0] <= _T('z'))) ||
	     ((_T('A') <= pszDrive[0]) && (pszDrive[0] <= _T('Z'))) 
	   )
	   	{
		m_sDrive = CString(TCHAR(toupper(pszDrive[0]))) + _T(':');
		return TRUE;
		}
	else
		{
		TRACE(_T("COXDirSpec::SetDrive : Invalid drive specification (%c)\n"), pszDrive[0]);
		return FALSE;
		}
	}
	
void COXDirSpec::ForceSetDrive(LPCTSTR pszDrive)
	{
	// ... Must be valid pointer
	ASSERT(pszDrive != NULL);
	if ((0 < _tcsclen(pszDrive)) &&
	     ( ((_T('a') <= pszDrive[0]) && (pszDrive[0] <= _T('z'))) ||
		   ((_T('A') <= pszDrive[0]) && (pszDrive[0] <= _T('Z'))) 
		 )
	   ) 
		 VERIFY(SetDrive(CString(TCHAR(toupper(pszDrive[0]))) + _T(":")));
	else
		 VERIFY(SetDrive(_T("")));
	}
	
CString COXDirSpec::GetSubdirectory() const
	{
	return m_sSubdirectory;
	}
	
BOOL COXDirSpec::SetSubdirectory(LPCTSTR pszSubdirectory)
	{
	// ... Must be valid pointer
	ASSERT(pszSubdirectory != NULL);
	CString sSubdirectory(pszSubdirectory);
	// ... Short circuit evaluation
	if ( (sSubdirectory.Right(1) != _T("\\")) || (sSubdirectory.GetLength() == 1))
		{
#ifndef OX_FILEMNG_NOCHANGECASE
		sSubdirectory.MakeUpper();
#endif
		if ((int)_tcscspn(sSubdirectory, INVALID_DIR_CHARS) != sSubdirectory.GetLength())
			{
			TRACE(_T("COXFileSpec::SetSubdirectory : Subdirectorye (%s) contains illegal characters\n"), sSubdirectory);
			return FALSE;
			}
		m_sSubdirectory = sSubdirectory;
		return TRUE;
		}
	else
		{
		TRACE(_T("COXDirSpec::SetSubdirectory : Only root directory may end in \"\\\", not %s\n"), sSubdirectory);
		return FALSE;
		}
	}

void COXDirSpec::ForceSetSubdirectory(LPCTSTR pszSubdirectory)
	{
	// ... Must be valid pointer
	ASSERT(pszSubdirectory != NULL);
	CString sSubdirectory(pszSubdirectory);
	int nIndexWrongChar;
	while ((nIndexWrongChar = (int)_tcscspn(sSubdirectory, INVALID_DIR_CHARS)) != sSubdirectory.GetLength())
		{
		sSubdirectory = sSubdirectory.Left(nIndexWrongChar) + sSubdirectory.Mid(nIndexWrongChar + 1);
		}
	if ( (sSubdirectory.Right(1) == _T("\\")) && (sSubdirectory.GetLength() != 1))
		// Remove trailing back slash
		sSubdirectory = sSubdirectory.Mid(0,sSubdirectory.GetLength() - 1);
	VERIFY(SetSubdirectory(sSubdirectory));
	}
	
COXDirSpec COXDirSpec::GetLastSubdirectory() const
	{
	COXDirSpec resultDir;
	CString sSubdir;
	int nIndex;
	
	resultDir.SetDrive(GetDrive());
	sSubdir = GetSubdirectory();
	nIndex = sSubdir.ReverseFind(_T('\\'));
	if (nIndex == -1)
		// No back slash found
		resultDir.SetSubdirectory(GetSubdirectory());
	else
		{
		ASSERT(0 <= nIndex);
		resultDir.SetSubdirectory(sSubdir.Mid(nIndex + 1));
		}
	return resultDir;
	}
	
void COXDirSpec::RemoveLastSubdirectory()
	{
	int nIndex;

	nIndex = m_sSubdirectory.ReverseFind(_T('\\'));
	if (nIndex != -1)
		// Back slash found
		{
		ASSERT(0 <= nIndex);
		if (nIndex != 0)
			// Not the root directory
			m_sSubdirectory = m_sSubdirectory.Left(nIndex);
		else
			// The root directory
			m_sSubdirectory = _T("\\");
		}
	}
	
CString COXDirSpec::GetDirectory() const
	{
	return m_sDrive + m_sSubdirectory;
	}
	
BOOL COXDirSpec::SetDirectory(LPCTSTR pszDirectory)
	{
	// ... Must be valid pointer
	ASSERT(pszDirectory != NULL);
	CString sDirectory(pszDirectory);
	if (! sDirectory.IsEmpty() && sDirectory.Mid(1,1) == _T(":"))
		{
		if (!SetDrive(sDirectory.Left(2)))
			return FALSE;
		if (3 <= sDirectory.GetLength())
			return SetSubdirectory(sDirectory.Mid(2));
		else
			return SetSubdirectory(_T(""));
		}
	else
		{
		return SetDrive(_T("")) && SetSubdirectory(sDirectory);
		}
	}
	
void COXDirSpec::ForceSetDirectory(LPCTSTR pszDirectory)
	{
	// ... Must be valid pointer
	ASSERT(pszDirectory != NULL);
	if ((0 < _tcsclen(pszDirectory)) && (pszDirectory[1] == _T(':')))
		{
		ForceSetDrive(&pszDirectory[0]);
		if (3 <= _tcsclen(pszDirectory))
			ForceSetSubdirectory(&pszDirectory[2]);
		else
			ForceSetSubdirectory(_T(""));
		}
	else
		{
		ForceSetDrive(_T(""));
		ForceSetSubdirectory(pszDirectory);
		}
	}
	
CString COXDirSpec::GetFileSystemType()
	{
	if (m_sDrive.IsEmpty())
		{
		TRACE(_T("COXDirSpec::GetFileSystemType : No drive specified, returning empty string\n"));
		return _T("");
		}
#ifdef WIN32
	const int nMaxSystemNameLength = 50;
	TCHAR szSystemName[nMaxSystemNameLength + 1];
	BOOL bSuccess;
	// ... The Error Mode is temporarily set to SEM_FAILCRITICALERRORS to 
	//	   allow failures to immediately return to the calling program.  
	//     This eliminates unwanted dialog boxes that prompt for disks 
	//     to be placed in the drive.
    UINT nOldErrorMode = ::SetErrorMode(SEM_FAILCRITICALERRORS); 
  	bSuccess = GetVolumeInformation(m_sDrive + _T("\\"), NULL, 0, NULL,
  		 NULL, NULL, szSystemName, nMaxSystemNameLength);
	// ... Restore the error code
	::SetErrorMode(nOldErrorMode);
	if (bSuccess)
		return szSystemName;
	else
		{
		TRACE(_T("COXDirSpec::GetFileSystemType : Could not determine file system, returning empty string\n"));
		return _T("");
		}
#else
	// WIN16 cannot distinguish between file systems, so always return "FAT"
	return _T("FAT");
#endif
	}

BOOL COXDirSpec::MakeTemp()
	{
#ifdef WIN32
	CString sTempPath;
	BOOL bSuccess = ::GetTempPath(_MAX_PATH, sTempPath.GetBuffer(_MAX_PATH));
	sTempPath.ReleaseBuffer();
	if (bSuccess)
		{
		// we'll have to use FORCE... because the path returned
		// by WIN32 api ends with a '\'
		ForceSetDirectory(sTempPath);
		return TRUE;
		}
	else
		return FALSE;
#else
	char path_buffer[_MAX_PATH + 1]; 
	char chDriveLetter = 'C';
	char* pszSlash = NULL;

	// Get temp path
	::GetTempFileName(0, 			// Use the default drive
					  _T("TMP"), 		// TMP-prefix
					  1, 			// Do not try to open and close the file
					  path_buffer);	// Result
	// ... Remove the file name (keep only the dir, WITHOUT trailing black slash!)
	pszSlash = strrchr(path_buffer, '\\');
	if (pszSlash != NULL)
		*pszSlash = '\0';
		
	if (*path_buffer != '\0')
		{
		return SetDirectory(path_buffer);
		}
	else
		{
		TRACE(_T("COXDirSpec::MakeTemp : Could not make temp path, using root of temp drive\n"));
		VERIFY(SetDirectory(_T("\\")));
		return SetDrive(CString(::GetTempDrive(chDriveLetter)));
		}
#endif
	}

BOOL COXDirSpec::MakeUnique()
	{
	COXPathSpec path;
	// First convert to file name and at the end convert back to dir
	path.SetPath(GetDirectory());
	if (path.MakeUnique() && SetDirectory(path.GetPath()))	// Short circuit evaluation
		return TRUE;
	else
		{
		TRACE(_T("COXDirSpec::MakeUnique : Could not make unique dir\n"));
		return FALSE;
		}
	}
	
BOOL COXDirSpec::MakeLargestExisting()
	{
	// Create a temp path spec to make the dir spec absolute
	COXPathSpec absolutePath;
	// .... Dir spec should already be avlid and file spec is valid too,
	//      so path spec should be valid as well !
	VERIFY(absolutePath.SetPath(*this, COXFileSpec(_T("dummy.tmp"))));
	if (!absolutePath.MakeAbsolute())
		{
		TRACE(_T("COXDirSpec::MakeLargestExisting : Could not make '%s' absolute, failing\n"),
			(LPCTSTR)absolutePath.GetPath());
		// ... Use root directory (best we can do)
		VERIFY(SetDrive(absolutePath.GetDrive()));
		VERIFY(SetSubdirectory(_T("\\")));
		return FALSE;
		}

	// ... Use direcory part of absoluite path
	operator=(absolutePath);

	// First check whether the dir spec exists entirely
	if (Exists())
		return TRUE;

	// Now iterate all the subdirectories of this absolute dir spec
	// starting from the root, until all subdirs are tried or a non-existing 
	// subdirectory has been encountered
	// E.g. if dir spec = C:\ONE\TWO\THREE then
	//		then try C:\  
	//		then try C:\ONE
	//		then try C:\ONE\TWO
	// If one of them does not exist, the search is stopped
	LPCTSTR pszSubdirBegin;
	LPCTSTR pszPrevSubdirEnd;
	LPCTSTR pszSubdirEnd;
	BOOL bExist = TRUE;
	COXDirSpec testDir;

	pszSubdirBegin = m_sSubdirectory;
	pszPrevSubdirEnd = pszSubdirBegin;
	pszSubdirEnd = _tcschr(pszSubdirBegin, _T('\\'));
	while (bExist && (pszSubdirEnd != NULL))
		{
		// ... End the subdir spec BEFORE the terminating back slash
		//     except when the root (first char is back slash)
		if (pszSubdirBegin == pszSubdirEnd)
			pszSubdirEnd++;
		VERIFY(testDir.SetDrive(GetDrive()));
		VERIFY(testDir.SetSubdirectory(CString(pszSubdirBegin, PtrToInt(pszSubdirEnd - pszSubdirBegin))));
		bExist = testDir.Exists();
		if (bExist)
			{
			// Get next subdir
			pszPrevSubdirEnd = pszSubdirEnd;
			pszSubdirEnd = _tcschr(pszSubdirEnd + 1, _T('\\'));
			}
		}

	if (bExist)
		{
		// All the checked subdirectories exist, 
		// and we know that the complete dir spec does not exist
		ASSERT(!Exists());
		ASSERT(pszSubdirEnd == NULL);
		ASSERT(pszPrevSubdirEnd != NULL);
		VERIFY(SetSubdirectory(CString(pszSubdirBegin, PtrToInt(pszPrevSubdirEnd - pszSubdirBegin))));
		}
	else
		{
		// A non-existing subdir has been encountered, 
		// use the last existing sub dir
		ASSERT(pszPrevSubdirEnd != NULL);
		VERIFY(SetSubdirectory(CString(pszSubdirBegin, PtrToInt(pszPrevSubdirEnd - pszSubdirBegin))));
		}

	// The dir spec must exist now
	ASSERT(Exists());
	return TRUE;
	}

BOOL COXDirSpec::Exists() const
	{
	// Assume that an empty directory or the root directory always exist
	// (although CFile::GetStatus("C:\\") return FALSE)
	if (GetSubdirectory().IsEmpty() || (GetSubdirectory() == _T("\\")))
		return TRUE;

	CFileStatus fileStatus;
	return ( (CFile::GetStatus(GetDirectory(), fileStatus)) && 
		     (fileStatus.m_attribute & CFile::directory) );
	}
	
BOOL COXDirSpec::IsEmpty() const
	{
	return GetDirectory().IsEmpty();
	}
	
void COXDirSpec::Empty()
	{
	m_sDrive.Empty();
	m_sSubdirectory.Empty();
	}
	
BOOL COXDirSpec::IsEmptyDir() const
	{
	COXPathSpec filePath;

	BOOL bFileFound(FALSE);	
	BOOL bValid(FALSE);	
	VERIFY(filePath.SetPath(*this, COXFileSpec(_T("*.*"))));

#ifdef WIN32
	WIN32_FIND_DATA fileData;

    HANDLE hFindFile = FindFirstFile(filePath.GetPath(), &fileData);
	if (hFindFile != INVALID_HANDLE_VALUE)
		bFileFound = TRUE;	
    // As long a something is found, but is not a good one, keep searching
    // ... Ignore non-subdirectories and subdirectories starting with a full steop
	while (!bValid && bFileFound)
		{
		if (fileData.cFileName[0] != _T('.'))
			bValid = TRUE;

		if (!bValid)	
			bFileFound = FindNextFile(hFindFile, &fileData);
		}

	if (hFindFile != INVALID_HANDLE_VALUE)
		FindClose(hFindFile);

#else 	
	_find_t fileInfo;

    bFileFound = !_dos_findfirst(filePath.GetPath(), _A_SUBDIR | _A_ALL, &fileInfo);

   	while (!bValid && bFileFound)
		{
		if ((strcmp(fileInfo.name, _T(".")) != 0) && (strcmp(fileInfo.name, _T("..")) != 0))
			bValid = TRUE;
		
		if (!bValid)	
			bFileFound = (_dos_findnext(&fileInfo) == 0);
		}		

#endif

    return !bValid;
	}

	
BOOL COXDirSpec::DoGetCurrentDir()
	{
	TCHAR pszSubdirectory[_MAX_DIR];
#ifndef WIN32 
	if(_getcwd(pszSubdirectory, _MAX_DIR) != NULL)
#else                                   
	if (GetCurrentDirectory(_MAX_DIR, pszSubdirectory))
#endif
		{
		ASSERT(CString(pszSubdirectory).GetLength() >= 2);
		SetDrive(CString(pszSubdirectory).Left(2));
		SetSubdirectory(CString(pszSubdirectory).Mid(2));
		return TRUE;
		}
	else
		{
		TRACE(_T("COXDirSpec::DoGetCurrentDir : Error occurred while accessing current directory\n"));
		return FALSE;
		}


	}

BOOL COXDirSpec::DoSetCurrentDir() const
	{
	if (!m_sDrive.IsEmpty())
		{
		ASSERT(m_sDrive[0] == toupper(m_sDrive[0]));
		if (_chdrive((int)m_sDrive[0] - _T('A') + 1) != 0)
			{
			TRACE(_T("COXDirSpec::DoSetCurrentDir : Drive change to %s failed\n"), m_sDrive);
			return FALSE;
			}
		}
	if (!m_sSubdirectory.IsEmpty())
		{
		if (_tchdir((LPCTSTR)m_sSubdirectory) != 0)
			{
			TRACE(_T("COXDirSpec::DoSetCurrentDir : Directory change to %s failed\n"), m_sSubdirectory);
			return FALSE;
			}
		}
	
	return TRUE;
	}

UINT COXDirSpec::DoGetDriveType()
	{
	if (m_sDrive.IsEmpty())
		{
		TRACE(_T("COXDirSpec::DoGetDriveType : No drive specified, returning empty string\n"));
		return 0;
		}

	ASSERT(m_sDrive[0] == toupper(m_sDrive[0]));

#ifndef WIN32 
	int iDrive;
	UINT uiType;
	BOOL fCDROM=FALSE;
	BOOL fRAM=FALSE;

	iDrive = m_sDrive[0] - _T('A');
	uiType = GetDriveType(iDrive);        
    
    // Check for CDROM on FIXED and REMOTE drives only
    if (DRIVE_FIXED==uiType || DRIVE_REMOTE==uiType)
		{
		_asm
	    	{
		    mov     ax,1500h        // Check if MSCDEX exists
		    xor     bx,bx
		    int     2fh

		    or      bx,bx           // BX unchanged if MSCDEX is not around
		    jz      CheckRAMDrive   // No?  Go check for RAM drive.

		    mov     ax,150Bh        // Check if drive is using CD driver
		    mov     cx,iDrive
		    int     2fh

		    mov     fCDROM,ax       // AX if the CD-ROM flag
		    or      ax,ax
		    jnz     Exit            // Leave if we found a CD-ROM drive.

		    CheckRAMDrive:
		    }
		}

    // Check for RAM drives on FIXED disks only.
    if (DRIVE_FIXED==uiType)
		{
		/*
		 * Check for RAM drive is done by reading the boot sector and
		 * looking at the number of FATs.  Ramdisks only have 1 while
		 * all others have 2.
		 */
		_asm
		    {
		    push    ds

		    mov     bx,ss
		    mov     ds,bx

		    sub     sp,0200h            //Reserve 512 bytes to read a sector
		    mov     bx,sp               //and point BX there.

		    mov     ax,iDrive           //Read the boot sector of the drive
		    mov     cx,1
		    xor     dx,dx

		    int     25h
		    add     sp,2                //Int 25h requires our stack cleanup.
		    jc      DriveNotRAM

		    mov     bx,sp
		    cmp     ss:[bx+15h],0f8h    //Reverify fixed disk
		    jne     DriveNotRAM
		    cmp     ss:[bx+10h],1       //Check if there's only one FATs
		    jne     DriveNotRAM
		    mov     fRAM,1

		    DriveNotRAM:
		    add     sp,0200h
		    pop     ds

		    Exit:
		    //Leave fRAM untouched  it's FALSE by default.
		    }
		}

    /*
     * If either CD-ROM or RAM drive flags are set, return privately
     * defined flags for them (outside of Win32).  Otherwise return
     * the type given from GetDriveType.
     */

    if (fCDROM)
		return DRIVE_CDROM;

    if (fRAM)
		return DRIVE_RAMDISK;

    //Drive B on a one drive system returns < 2 from GetDriveType.
	return uiType;      
#else                                            
	return GetDriveType(m_sDrive + _T("\\"));
#endif
	}

CString COXDirSpec::DoGetVolumeInformation()
	{
	if (m_sDrive.IsEmpty())
		{
		TRACE(_T("COXDirSpec::DoGetVolumeInformation : No drive specified, returning empty string\n"));
		return _T("");
		}

	ASSERT(m_sDrive[0] == toupper(m_sDrive[0]));

#ifndef WIN32
	struct _find_t findStruct;
	if (_dos_findfirst(m_sDrive + _T("\\"), _A_VOLID, &findStruct) == 0)
		{
		// a volume label in 16bit can contain 11 chars.  But it's written as
		// 8.3 string on disk. So when asking this volume label a period appears in
		// the label. We will remove it here.		char* pszPeriod = NULL;
    	char* pszPeriod = strchr(findStruct.name, '.');
	    if (pszPeriod != NULL)
	    	strcpy(pszPeriod, pszPeriod + 1);

		return CString(findStruct.name);
		}
		
	return _T("");
#else                              
	TCHAR VolumeName[_MAX_DIR + 1]; 
	if (!GetVolumeInformation(m_sDrive + _T("\\"), VolumeName,_MAX_DIR, NULL,NULL,NULL,NULL,0))
		{
		TRACE(_T("COXDirSpec::DoGetVolumeInformation : GetVolumeInformation failed\n"));
		return _T("");
		}

	return CString(VolumeName);
#endif
	}

DWORD COXDirSpec::DoGetDiskFreeSpace()
	{
	DWORD dwBytes;

	if (m_sDrive.IsEmpty())
		{
		TRACE(_T("COXDirSpec::DoGetDiskFreeSpace : No drive specified, returning zero space\n"));
		return 0;
		}

	ASSERT(m_sDrive[0] == toupper(m_sDrive[0]));

#ifndef WIN32
	struct _diskfree_t  DriveInfo;
	int iDrive;

	iDrive = m_sDrive[0] - 'A' + 1;

    if (_dos_getdiskfree(iDrive, &DriveInfo) == 0)
	    {
		dwBytes = (long)DriveInfo.bytes_per_sector * (long)DriveInfo.sectors_per_cluster;
		dwBytes *= (long)DriveInfo.avail_clusters;
		return dwBytes;
	    }
	else
		{
		TRACE(_T("COXDirSpec::DoGetDiskFreeSpace : _dos_getdiskfree failed\n"));
	    return -1;
		}
#else
	BOOL bOk;
	DWORD dwSectPerClust, dwBytesPerSect, dwFreeClust, dwTotClust;

	bOk = GetDiskFreeSpace(m_sDrive + _T("\\"), &dwSectPerClust, &dwBytesPerSect, &dwFreeClust, &dwTotClust); 
	if (bOk)
		{
		dwBytes = dwFreeClust * dwSectPerClust * dwBytesPerSect;
		return dwBytes;
		}
	else
		{
		TRACE(_T("COXDirSpec::DoGetDiskFreeSpace : GetDiskFreeSpace failed\n"));
	    return 0;
		}
#endif
	}

BOOL COXDirSpec::DoMakeNew() const
	{
	// Do not try to create a directory with an empty name and
	// do not try to create the root directory (it already exists)
	if (!m_sSubdirectory.IsEmpty() && (m_sSubdirectory != _T("\\")))
		{
		CString sDir = m_sDrive + m_sSubdirectory;
		// First try to create all the directories in front of the last back slash
		// This is done without error checking, because they may already exist
		int nSlashPosition;
		int nDeltaSlashPosition;             
		
		nDeltaSlashPosition = sDir.Find(_T('\\'));
		nSlashPosition = nDeltaSlashPosition;
		while ( nDeltaSlashPosition != -1)
			{
#ifndef WIN32     
			_mkdir(sDir.Left(nSlashPosition));
#else                                
			SECURITY_ATTRIBUTES sSecAttr;
	
			sSecAttr.nLength = sizeof(sSecAttr);
			sSecAttr.lpSecurityDescriptor = NULL;
			sSecAttr.bInheritHandle = FALSE;

			CreateDirectory(sDir.Left(nSlashPosition), &sSecAttr);
#endif
			
			// ... Only root dir can end in \\ and this case is excluded here by spanning is
			ASSERT(sDir.Right(1) != _T("\\"));	
			nDeltaSlashPosition = sDir.Mid(nSlashPosition + 1).Find(_T('\\'));
			nSlashPosition += nDeltaSlashPosition + 1;
			}
			
		// Now try to create the entire directory specification,
		// with error checking
#ifndef WIN32     
		if (_mkdir(sDir) != 0)
			{
			TRACE(_T("COXDirSpec::DoMakeNew : Directory creation %s failed\n"), sDir);
			return FALSE;
			}
#else                                
		SECURITY_ATTRIBUTES sSecAttr2;

		sSecAttr2.nLength = sizeof(sSecAttr2);
		sSecAttr2.lpSecurityDescriptor = NULL;
		sSecAttr2.bInheritHandle = FALSE;

		if (!CreateDirectory(sDir, &sSecAttr2))
			{
			TRACE(_T("COXDirSpec::DoMakeNew : Directory creation %s failed\n"), sDir);
			return FALSE;
			}
#endif
		}

	return TRUE;
	}

#ifndef NOT_REGISTERED
BOOL COXDirSpec::DoCopyDirectory(COXDirSpec DestDirectory, BOOL bOnlyContents /* = TRUE */,
	BOOL bCleanUp  /* = FALSE */, COXCopyStatusDialog* pCpyStatDlg /* = NULL */)
	{
	COXCopyTree CopyTree;

	return CopyTree.DoCopyTree(*this, DestDirectory, bOnlyContents, bCleanUp, pCpyStatDlg);
	}
#endif // !NOT_REGISTERED
	 	
BOOL COXDirSpec::DoRemove(BOOL bRecursively /* = FALSE */,
						BOOL bAlsoRemoveReadOnly /* = FALSE */) const
	{
#ifdef _DEBUG
	if (GetSubdirectory() == _T("\\"))
		{
		TRACE(_T("COXDirSpec::DoRemove : Trying to remove root directory (%s) and all it's subdirectories\n"),
			GetSubdirectory());
		// Dangerous situation : Give user change to abort or go on (retry)
		ASSERT(FALSE);
		}
#endif	
	if (!m_sSubdirectory.IsEmpty())
		{
		if (bRecursively)
			// First remove all underlying directories
			{
			COXDirSpec lowerDir;
			lowerDir = FindLowerDirectory();
			while (!lowerDir.IsEmpty())
				{
				if (!lowerDir.DoRemove(bRecursively, bAlsoRemoveReadOnly))
					// Could not remove lower directory, abort
					return FALSE;
				lowerDir = FindLowerDirectory();
				}
			}
			
		// Remove all the files in the directory
		if (!RemoveAllFiles(bAlsoRemoveReadOnly))
			{
			TRACE(_T("COXDirSpec::DoRemove : Removal of files failed, cannot remove directory %s\n"), GetDirectory());
			return FALSE;
			}
		
		// Remove the directory itself
#ifdef WIN32
		if (!RemoveDirectory(GetDirectory()))
#else
		if (_rmdir(GetDirectory()) != 0)
#endif
			{
			TRACE(_T("COXDirSpec::DoRemove : Cannot remove directory : %s\n"), GetDirectory());
			return FALSE;
			}
		}
	return TRUE;
	}

void COXDirSpec::Serialize(CArchive& ar)
	{
	CObject::Serialize(ar);
	
	TRY
		{
		if (ar.IsStoring())
			{
			ar << m_sDrive;
			ar << m_sSubdirectory;
			}	
		else
			{
			ar >> m_sDrive;
			ar >> m_sSubdirectory;
			}
		}
	CATCH(CException, e)
		{
		if (ar.IsStoring())
			TRACE0("An Exception was thrown while storing a COXDirSpec");
		else
			TRACE0("An Exception was thrown while loading a COXDirSpec");

		THROW_LAST();
		}
	END_CATCH
	}

BOOL COXDirSpec::operator==(const COXDirSpec& dirSpec) const
	{
	return ( (m_sDrive == dirSpec.m_sDrive) &&
			 (m_sSubdirectory == dirSpec.m_sSubdirectory) );
	}
	
BOOL COXDirSpec::operator!=(const COXDirSpec& dirSpec) const
	{
	return ( (m_sDrive != dirSpec.m_sDrive) ||
			 (m_sSubdirectory != dirSpec.m_sSubdirectory) );
	}
	
BOOL COXDirSpec::operator<=(const COXDirSpec& dirSpec) const
	{
	return (m_sDrive + m_sSubdirectory <= dirSpec.m_sDrive + dirSpec.m_sSubdirectory);
	}
	
BOOL COXDirSpec::operator<(const COXDirSpec& dirSpec) const
	{
	return (m_sDrive + m_sSubdirectory < dirSpec.m_sDrive + dirSpec.m_sSubdirectory);
	}  
	
BOOL COXDirSpec::operator>=(const COXDirSpec& dirSpec) const
	{
	return (m_sDrive + m_sSubdirectory >= dirSpec.m_sDrive + dirSpec.m_sSubdirectory);
	}
	
BOOL COXDirSpec::operator>(const COXDirSpec& dirSpec) const
	{
	return (m_sDrive + m_sSubdirectory > dirSpec.m_sDrive + dirSpec.m_sSubdirectory);
	}

#ifdef _DEBUG
void COXDirSpec::Dump(CDumpContext& dc) const
	{
	CObject::Dump(dc);
	dc << _T("\nm_sDrive : ") << m_sDrive;
	dc << _T("\nm_sSubdirectory : ") << m_sSubdirectory;
	}

void COXDirSpec::AssertValid() const
	{
	CObject::AssertValid();
	}
#endif

COXDirSpec::~COXDirSpec()
	{
	}

#ifdef WIN32
BOOL COXDirSpec::IsChildDir(LPWIN32_FIND_DATA lpFindFileData)	const
	// --- In  : 
	// --- Out : 
	// --- Returns : if lpFindFileData contains a subdir...
	// --- Effect : 

{
	return (
		((lpFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) &&
		(lpFindFileData->cFileName[0] != _T('.')));
}	
#endif

// protected:
COXDirSpec COXDirSpec::FindLowerDirectory() const
	// --- In  : 
	// --- Out : 
	// --- Returns : A directory that is a subdirectory of this dir spec
	// --- Effect : 
	{
	COXDirSpec lowerDir;
	COXPathSpec searchPath;
	VERIFY(searchPath.SetPath(*this, COXFileSpec(_T("*.*"))));

	BOOL bDirFound(FALSE);
	BOOL bFileFound(FALSE);

#ifdef WIN32
	WIN32_FIND_DATA fileData;

    HANDLE hFindFile = FindFirstFile(searchPath.GetPath(), &fileData);
	if (hFindFile != INVALID_HANDLE_VALUE)
		bFileFound = TRUE;	

    // As long a something is found, but is not a good one, keep searching
    // ... Ignore non-subdirectories and subdirectories starting with a full steop
	while (!bDirFound && bFileFound)
		{
		bDirFound = IsChildDir(&fileData);
		
		if (!bDirFound)	
			bFileFound = FindNextFile(hFindFile, &fileData);

		}

	if (hFindFile != INVALID_HANDLE_VALUE)
		FindClose(hFindFile);

	if (bDirFound)
		{
		lowerDir = *this;
		VERIFY(lowerDir.AppendDirectory(COXDirSpec(fileData.cFileName)));
	    return(lowerDir);
	    }
	else
		// Returning an empty directory
	    return(lowerDir);
#else
	_find_t fileInfo;
	
    bDirFound = !_dos_findfirst(searchPath.GetPath(), _A_SUBDIR | _A_ALL, &fileInfo);
    // As long a something is found, but is not a good one, keep searching
    // ... Ignore non-subdirectories and subdirectories starting with a full steop
	while( (bDirFound) && 
		   ( !((fileInfo.attrib & _A_SUBDIR) == _A_SUBDIR) || (*fileInfo.name == '.') ) )
	    bDirFound = !_dos_findnext(&fileInfo);

	if (bDirFound)
		{
		lowerDir = *this;
		VERIFY(lowerDir.AppendDirectory(COXDirSpec(fileInfo.name)));
	    return(lowerDir);
	    }
	else
		// Returning an empty directory
	    return(lowerDir);
#endif
	}

BOOL COXDirSpec::RemoveAllFiles(BOOL bAlsoRemoveReadOnly /* = FALSE */) const
	// --- In  : bAlsoRemoveReadOnly : Whether to remove all the files,
	//				even when they are read only (TRUE) or not (FALSE)
	// --- Out : 
	// --- Returns : Whether all the removals succeeded
	// --- Effect : Removes all the files of this directory
	{
	COXPathSpec filePath;
	VERIFY(filePath.SetPath(*this, COXFileSpec(_T("*.*"))));

	BOOL bFileFound(TRUE);
#ifdef WIN32
	WIN32_FIND_DATA fileData;

    HANDLE hFindFile = FindFirstFile(filePath.GetPath(), &fileData);
    // As long a something is found, but is not a good one, keep searching
    // ... Ignore non-subdirectories and subdirectories starting with a full steop
	if(hFindFile != INVALID_HANDLE_VALUE)
		{
	    while(bFileFound)
			{
			if (IsChildDir(&fileData) || fileData.cFileName[0] == _T('.'))
				{
				bFileFound = FindNextFile(hFindFile, &fileData);
				continue;
				}
			VERIFY(filePath.SetFileName(fileData.cFileName));
			// If the file is Read/only and it is allowed to remove it, try so
			if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
				{
				if (bAlsoRemoveReadOnly)
					{
					if (!SetFileAttributes(filePath.GetPath(), fileData.dwFileAttributes & ~FILE_ATTRIBUTE_READONLY))
						{
						TRACE(_T("COXDirSpec::RemoveAllFiles : Could not change the Read/Only attribute of file %s\n"), 
									filePath.GetPath());
						::FindClose(hFindFile);
						return FALSE;
						}
					}
				else
					{
					TRACE(_T("COXDirSpec::RemoveAllFiles : Could not remove the file %s, because it is Read/Only\n"), 
								filePath.GetPath());
					::FindClose(hFindFile);
					return FALSE;
					}
				}
			if (!DeleteFile(filePath.GetPath()))
				{
				TRACE(_T("COXDirSpec::RemoveAllFiles : Could not remove the file %s\n"), filePath.GetPath());
				::FindClose(hFindFile);
				return FALSE;
	            }
			bFileFound = FindNextFile(hFindFile, &fileData);
	    	}  //while
		
		::FindClose(hFindFile);
		} // if(INVALID_HANDLE_VALUE)
#else
	_find_t fileInfo;

	VERIFY(filePath.SetPath(*this, COXFileSpec(_T("*.*"))));
    bFileFound = !_dos_findfirst(filePath.GetPath(), _A_ALL, &fileInfo);
    while(bFileFound)
		{
		VERIFY(filePath.SetFileName(fileInfo.name));
		// If the file is Read/only and it is allowed to remove it, try so
		if ((fileInfo.attrib & _A_RDONLY) == _A_RDONLY)
			{
			if (bAlsoRemoveReadOnly)
				{
				if (_dos_setfileattr(filePath.GetPath(), fileInfo.attrib & ~_A_RDONLY) != 0)
					{
					TRACE(_T("COXDirSpec::RemoveAllFiles : Could not change the Read/Only attribute of file %s\n"), 
								filePath.GetPath());
					return FALSE;
					}
				}
			else
				{
				TRACE(_T("COXDirSpec::RemoveAllFiles : Could not remove the file %s, because it is Read/Only\n"), 
							filePath.GetPath());
				return FALSE;
				}
			}
		if (remove(filePath.GetPath()) != 0)
			{
			TRACE(_T("COXDirSpec::RemoveAllFiles : Could not remove the file %s\n"), filePath.GetPath());
			return FALSE;
            }

		bFileFound = !_dos_findnext(&fileInfo);
    	}
#endif

    return TRUE;
	} 
	
    
// private:

// Message handlers ---------------------------------------------------------

// ==========================================================================
