// ==========================================================================
// 					Class Specification : COXUNC
// ==========================================================================

// Header file : OXUNC.h

// Version: 9.3

// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CObject

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This class encapsulates a full path specification
//	A COXUNC object can have three different forms
//		- Strict UNC : \\Server\Share\Directory\File.ext
//		- Local path : C:\Directory\File.ext
//		- URL part : www.dundas.com/directory/file.ext

//	A UNC must always start with a double slash
//	A URL must be explicitly marked as URL part (URLPart())
//	Other COXUNC objects or local paths

//	To put a directory in a COXUNC object let it end in a (back)slash
//	E.g. C:\Temp\ is a directory
//		 C:\Test is a file

// Remark:
//	This class makes no differences between a slash "/" and a backslash "\"

//	If a directory is specified it always ends in a (back)slash
//	If the directory is absolute it also starts with a (back)slash

//	All public functions which do not start with a verb access only
//	 the internal structure of the object and do not perform an action
//	 on the actual file system (like rename, delete etc.)

//	Two conversion operators that were present in a previous version of this class
//   have been replaced by an explicit function call
//	operator const CString() const;		-> const CString Full() const;
//	operator CString&();				-> CString& Full();

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXUNC_H__
#define __OXUNC_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


class COXUNCStandardActor;

class OX_CLASS_DECL COXUNC 
{
friend class COXURL;

// Data members -------------------------------------------------------------
protected:

public:
	CString m_sUNC;
	static const TCHAR m_cNull;
	// --- NULL character
	static const TCHAR m_cSlash;
	// --- Slash character
	static const TCHAR m_cBackslash;
	// --- Backslash character
	static const TCHAR m_cColon;
	// --- Colon character
	static const TCHAR m_cDot;
	// --- Dot character
	static const LPCTSTR m_pszSlashes;
	// --- String containing slash and backslash
	static const LPCTSTR m_pszInvalidChars;
	// --- String caontaing illegal file characters
	static const LPCTSTR m_pszSpecialChars;
	// --- String containing valid characters that have a special meaning

protected:
	BOOL	m_bMainBuilt;

	CString	m_sServer;
	CString	m_sShare;
	CString m_sDirectory;
	CString	m_sFile;
	BOOL	m_bPartsBuilt;

	BOOL	m_bURLPart;
	HRESULT	m_nLastError;

	COXUNCStandardActor* m_pActor;

private:
	
// Member functions ---------------------------------------------------------
public:
	COXUNC(LPCTSTR pszUNC = NULL, BOOL bURLPart = FALSE);
	// --- In  : pszURL : Full UNC specification
	//			 bURLPart : Whether this UNC is part of a URL
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructor of the object

	COXUNC(LPCTSTR pszServer, LPCTSTR pszShare, LPCTSTR pszDirectory, LPCTSTR pszFile, BOOL bURLPart = FALSE);
	// --- In  : pszServer : 
	//			 pszShare : 
	//			 pszDirectory : 
	//			 pszFile : 
	//			 bURLPart : Whether this UNC is part of a URL
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructor of the object


	COXUNC(const COXUNC& UNC);
	// --- In  : UNC : Source UNC
	// --- Out : 
	// --- Returns :
	// --- Effect : Copy constructor

	COXUNC& operator=(const COXUNC& UNC);
	COXUNC& operator=(LPCTSTR pszUNC);
	// --- In  : UNC : Source UNC
	// --- Out : 
	// --- Returns : This object
	// --- Effect : Assignment operator

	// operator const CString() const;
	const CString Full() const;
	const CString Server() const;
	const CString Share() const;
	const CString Directory() const;
	const CString File() const;
	const CString Base() const;
	const CString Extension(BOOL bIncludeDot= TRUE) const;
	const BOOL URLPart() const;
	const HRESULT LastError() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : The complete UNC or the requested part
	// --- Effect : Retrieves the requested value (cannot be changed)

	//				The Title is the full path specification without the extension
	//				and it may be abbreviated (some directories replaced by ...)
	//				The Root is the root directory spoecification of this UNC

	operator LPCTSTR();
	CString& Full();
	CString& Server();
	CString& Share();
	CString& Directory();
	CString& File();
	BOOL& URLPart();
	HRESULT& LastError();
	// --- In  : 
	// --- Out : 
	// --- Returns : A reference to the complete UNC or the requested part
	// --- Effect : Retrieves the requested value 
	//				You can change the value by assigning a new value
	//				Do not store this reference for later use !

	// *** Requesting a UNC in a different form

	TCHAR PreferedSlash() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : The prefered slash character (slash or backslash)
	// --- Effect : The result depends on whether this a part of a URL ("/") or not ("\")

	CString StandardForm(BOOL bMakeLower = TRUE) const;
	// --- In  : 
	// --- Out : 
	// --- Returns : A standard form of this UNC
	//				 The standard form uses all backslashes if it not marked a part
	//				 of a URL, otherwise it uses all slashes
	//				 All characters are converted to lower case letters
	//				 The standard form is usefull for comparisons
	// --- Effect : 

	CString FileForm() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : The UNC, but if it specifies a directory, the terminating
	//				 slash is removed.  One exception is the root directory,
	//				 this always ends in a slash
	// --- Effect : 

	BOOL IsEmpty() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : Clears the contents

	void Empty();
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : Clears the contents

	// *** Helper functions

	COXUNC GetRoot() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : The root of this UNC
	// --- Effect : 

	COXUNC GetTitle() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : The title of this UNC (everything without the extension)
	// --- Effect : 

	CString GetAbbreviation(int nMaxLength, BOOL bAtLeastFile = TRUE) const;
	// --- In  : nMaxLength : The maximum length of return string
	//		     bAtLeastFile : Whether at least the file name should be included
	//				even when the maximum length is exceeded
	// --- Out : 
	// --- Returns : The abbreviated UNC (some parts will be replaced by "..."
	// --- Effect : 

	// *** General actions
	
	BOOL MakeAbsolute();
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether it succceeded or not (sets LastError())
	// --- Effect : Make a relative UNC into an absolute using the current drive and directory

	BOOL Exists();
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether it exists or not 
	//				 FALSE is also returned in case of an error (sets LastError())
	// --- Effect : Checks whether the specified directory or file exists
	//			    Note that when queried whether a directory exists
	//				and a file with the specified name is found, FALSE is returned
	//				So both the name and the type (directory/file) must match

	BOOL Create();
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : Creates the specified directory or file 

	BOOL CreateTemporaryFile(LPCTSTR pszPrefix = NULL);
	// --- In  : pszPrefix : The prefix that should be used in the file name
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : Creates an empty file in the temporary directory

	BOOL Copy(COXUNC destinationUNC, BOOL bReplaceExisting = FALSE);
	// --- In  : destinationUNC : 
	//			 bReplaceExisting : Whether an already existing destination should be overwritten
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : Copies a file to a different locatiuon

	BOOL Move(COXUNC destinationUNC, BOOL bReplaceExisting = FALSE, BOOL bCopyAllowed = TRUE);
	// --- In  : destinationUNC : 
	//			 bReplaceExisting : Whether an already existing destination should be overwritten
	//			 bCopyAllowed : Whether a copy is allowed (needed when source and destination
	//				are not on the same disk)
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : Moves a file to a different locatiuon

	BOOL Rename(COXUNC destinationUNC, BOOL bReplaceExisting = FALSE);
	// --- In  : destinationUNC : 
	//			 bReplaceExisting : Whether an already existing destination should be overwritten
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : Renames a file or directory

	BOOL Delete(BOOL bRemoveReadOnly = FALSE);
	// --- In  : bRemoveReadOnly : Whether a read/only file should be deleted as well
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : Deletes a file or (empty) dierectory

	// *** Navigation

	BOOL GetCurrentDirectory();
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : Makes this UNC the current directury

	BOOL SetCurrentDirectory();
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : Sets the current directury to this UNC

	BOOL GetWindowsDirectory();
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : Makes this UNC the windows directury

	BOOL GetSystemDirectory();
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : Makes this UNC the windows system directury

	BOOL GetApplicationDirectory();
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : Makes this UNC the application directury
	//				(Directory in which the EXE is located)

	BOOL GetTemporaryDirectory();
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : Makes this UNC the temporary directury

	// *** Classification

	LONGLONG GetSize();
	// --- In  : 
	// --- Out : 
	// --- Returns : The size of the UNC file if successful,
	//				 -1 otherwise (sets LastError())
	// --- Effect : 

	LONGLONG GetCompressedSize();
	// --- In  : 
	// --- Out : 
	// --- Returns : The compressed size of the UNC file if successful,
	//				 -1 otherwise (sets LastError())
	// --- Effect : 

	BOOL SetSize(LONGLONG nNewSize);
	// --- In  : nNewSize : The new size of the file
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : Resizes the specified file
	//				When the file is enlarged, the added bytes have an undefined value

	COXUNC GetShortName();
	// --- In  : 
	// --- Out : 
	// --- Returns : The short UNC representation (8.3 format) of this object
	//				 empty otherwise (sets LastError())
	// --- Effect : 

	COXUNC GetLongName();
	// --- In  : 
	// --- Out : 
	// --- Returns : The Long File Neme UNC representation of this object
	//				 empty otherwise (sets LastError())
	// --- Effect : 

	BOOL GetLogicalDrives(CStringArray& drives);
	// --- In  : drives
	// --- Out : drives : A collection of all the logical drives "A:", etc.
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : Retrieves a list of all the logical drives

	UINT GetDriveType();
	// --- In  : 
	// --- Out : 
	// --- Returns : The type of drive if successful or 0 otherwise (sets LastError())
	// --- Effect : Retrieves to type of drive.  Possible values in case of success are
	//				DRIVE_REMOVABLE, DRIVE_FIXED, DRIVE_REMOTE, DRIVE_CDROM, DRIVE_RAMDISK

	LONGLONG GetTotalDiskSpace();
	// --- In  : 
	// --- Out : 
	// --- Returns : The total number of bytes on the disk if successful 
	//				 or -1 otherwise (sets LastError())
	// --- Effect : 

	LONGLONG GetFreeDiskSpace();
	// --- In  : 
	// --- Out : 
	// --- Returns : The number of bytes of the free space on the disk if successful 
	//				 or -1 otherwise (sets LastError())
	// --- Effect : 

	CString GetDiskName();
	// --- In  : 
	// --- Out : 
	// --- Returns : The name of the disk if successful or empty otherwise (sets LastError())
	// --- Effect : 

	BOOL SetDiskName(LPCTSTR pszDiskName);
	// --- In  : pszDiskName : The new name of the disk
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : 

	DWORD GetDiskSerialNumber();
	// --- In  : 
	// --- Out : 
	// --- Returns : The serial number of the disk if successful or 0 otherwise (sets LastError())
	// --- Effect : 

	DWORD GetDiskMaximumComponentLength();
	// --- In  : 
	// --- Out : 
	// --- Returns : The maximum length, in characters, of a filename component supported 
	//				 by the specified file system if successful 
	//				 or 0 otherwise (sets LastError())
	// --- Effect : 

	DWORD GetDiskFileSystemFlags();
	// --- In  : 
	// --- Out : 
	// --- Returns : The file system flags of the specified file system if successful 
	//				 or 0 otherwise (sets LastError())
	// --- Effect : Possible values are any combination of the following
	//				FS_CASE_IS_PRESERVED : File system preserves the case of filenames
	//				FS_CASE_SENSITIVE : File system supports case-sensitive filenames
	//				FS_UNICODE_STORED_ON_DISK : The file system supports Unicode in filenames
	//				FS_PERSISTENT_ACLS : File system preserves and enforces ACLs. 
	//				FS_FILE_COMPRESSION	 : File system supports file-based compression.
	//				FS_VOL_IS_COMPRESSED : The specified volume is a compressed volume


	CString GetDiskFileSystemName();
	// --- In  : 
	// --- Out : 
	// --- Returns : The name of the file system (e.g. "NTFS", "FAT" etc) of this disk if successful or 
	//				 empty otherwise (sets LastError())
	// --- Effect : 

	DWORD GetAttributes();
	// --- In  : 
	// --- Out : 
	// --- Returns : The attributes of this file or directory if successful 
	//				 or MAXDWORD otherwise (sets LastError())
	// --- Effect : Possible values are any combination of the following
	//				FILE_ATTRIBUTE_ARCHIVE, FILE_ATTRIBUTE_HIDDEN, FILE_ATTRIBUTE_NORMAL,
	//				FILE_ATTRIBUTE_OFFLINE, FILE_ATTRIBUTE_READONLY, FILE_ATTRIBUTE_SYSTEM, 
	//				FILE_ATTRIBUTE_TEMPORARY, FILE_ATTRIBUTE_COMPRESSED, FILE_ATTRIBUTE_DIRECTORY

	BOOL ModifyAttributes(DWORD nRemoveAttributes, DWORD nAddAttributes);
	// --- In  : nRemoveAttributes : The attributes to remove
	//			 nAddAttributes : The attributes to add
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : Possible values are any combination of the following
	//				FILE_ATTRIBUTE_ARCHIVE, FILE_ATTRIBUTE_HIDDEN, FILE_ATTRIBUTE_NORMAL,
	//				FILE_ATTRIBUTE_OFFLINE, FILE_ATTRIBUTE_READONLY, FILE_ATTRIBUTE_SYSTEM, 
	//				FILE_ATTRIBUTE_TEMPORARY, FILE_ATTRIBUTE_COMPRESSED

	CTime GetCreationTime();
	// --- In  : 
	// --- Out : 
	// --- Returns : The creation time of the files or directory if successful 
	//				 or 0 otherwise (sets LastError())
	// --- Effect : 

	CTime GetLastAccessTime();
	// --- In  : 
	// --- Out : 
	// --- Returns : The last access time of the files or directory if successful 
	//				 or 0 otherwise (sets LastError())
	// --- Effect : 

	CTime GetLastWriteTime();
	// --- In  : 
	// --- Out : 
	// --- Returns : The last write time of the files or directory if successful 
	//				 or 0 otherwise (sets LastError())
	// --- Effect : 

	BOOL SetCreationTime(CTime newCreationTime);
	// --- In  : newCreationTime : The new time to set
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : 

	BOOL SetLastAccessTime(CTime newLastAccessTime);
	// --- In  : newLastAccessTime : The new time to set
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : 

	BOOL SetLastWriteTime(CTime newLastWriteTime); 
	// --- In  : newLastWriteTime : The new time to set
	// --- Out : 
	// --- Returns : Whether it succeeded or not (sets LastError())
	// --- Effect : 

#ifdef _DEBUG
#endif

	virtual ~COXUNC();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object

protected:
	void BuildMain();
	void BuildParts();
	void AdjustMain();
	void AdjustParts();
	static void Trim(CString& sText);
	void SetMainBuilt();
	void SetPartsBuilt();
	void DestroyMain();
	void DestroyParts();
	BOOL IsMainBuilt() const;
	BOOL ArePartsBuilt() const;

	static void ConvertBackslashToSlash(CString& sText);
	static void ConvertSlashToBackslash(CString& sText);

private:
};

// Include inline functions
#include "OXUNC.inl"

#endif // __OXUNC_H__
// ==========================================================================
