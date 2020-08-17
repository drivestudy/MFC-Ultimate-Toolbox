// ==========================================================================
// 					Class Specification : COXUNCStandardActor
// ==========================================================================

// Header file : OXUNCStandardActor.h

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
//	This class works together with the COXUNC class.  The COXUNC class encapsulates
//	 the data, whereas this COXUNCStandardActor class encapsulates the actual actions
//	 (moving, deleting etc.)

// Remark:
//	All result codes are full HRESULTs.  Win32 error are converted through
//	 the HRESULT_FROM_WIN32 macro
//	Return values are only valid when the result code is SUCCEEDED

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXUNCSTANDARDACTOR_H__
#define __OXUNCSTANDARDACTOR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include "OXUNC.h"


class OX_CLASS_DECL COXUNCStandardActor : public CObject
{
DECLARE_DYNAMIC(COXUNCStandardActor);

// Data members -------------------------------------------------------------
public:
	static COXUNCStandardActor m_theOneAndOnly;
	// --- The one and only instantiation of this class

protected:
private:
	
// Member functions ---------------------------------------------------------
public:
	COXUNCStandardActor();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object

	virtual HRESULT MakeAbsolute(COXUNC& UNC);
	// --- In  : UNC : The (possible relative) UNC
	// --- Out : UNC : The absolute representation of the UNC is successful,
	//				   otherwise the original value
	// --- Returns : The result code
	// --- Effect : Converts a possible relativeUNC to and absolute one

	virtual HRESULT Exists(COXUNC UNC, BOOL& bExists);
	// --- In  : UNC : The UNC
	// --- Out : bExists : Whether the UNC (directory / file) exists
	// --- Returns : The result code
	// --- Effect : Checks the existance of an UNC.
	//			    Note that when queried whether a directory exists
	//				and a file with the specified name is found, FALSE is returned
	//				So both the name and the type (directory/file) must match

	virtual HRESULT Create(COXUNC UNC);
	// --- In  : UNC : The UNC (deirectory / file) to create
	// --- Out : 
	// --- Returns : The result code
	// --- Effect : Creates a directory or file (or both)

	virtual HRESULT CreateTemporaryFile(COXUNC tempDir, LPCTSTR pszPrefix, COXUNC& file);
	// --- In  : tempDir : The directory in which a temporary file should be created
	//			 pszPrefix : The (3-character) prefix to be used in the file name
	// --- Out : file : The file that has been created or empty otherwise
	// --- Returns : The result code
	// --- Effect : Creates an empty temporary file in the specified directory
	//				This file is not deleted automatically

	virtual HRESULT Copy(COXUNC sourceUNC, COXUNC destinationUNC, BOOL bReplaceExisting);
	// --- In  : sourceUNC : The source UNC (file)
	//			 destinationUNC : The destination UNC (file)
	//			 bReplaceExisting : Whether an existing file should be replaced
	// --- Out : 
	// --- Returns : The result code
	// --- Effect : Copies the file to a different location

	virtual HRESULT MoveRename(COXUNC sourceUNC, COXUNC destinationUNC, DWORD dwFlags);
	// --- In  : sourceUNC : 
	//			 destinationUNC : 
	//			 dwFlags : Any combination of
	//				MOVEFILE_COPY_ALLOWED : Whether a copy to a different disk is allowed
	//				MOVEFILE_REPLACE_EXISTING : Whether an already existing destination should be overwritten
	//				MOVEFILE_DELAY_UNTIL_REBOOT	: (NT only) Command is only executed when the operating system is restarted
	//				MOVEFILE_WRITE_THROUGH : (NT only) The function does not return until the file has actually been moved on the disk. 
	// --- Out : 
	// --- Returns : The result code
	// --- Effect : Moves or replaces the specified file or directory

	virtual HRESULT Delete(COXUNC UNC, BOOL bRemoveReadOnly);
	// --- In  : UNC : 
	//			 bRemoveReadOnly : Whether read/only files should also be deleted
	// --- Out : 
	// --- Returns : The result code
	// --- Effect : Deletes the specified file or directory

	virtual HRESULT GetCurrentDirectory(COXUNC& UNC);
	// --- In  : 
	// --- Out : UNC : The current directory if succesful (empty otherwise)
	// --- Returns : The result code
	// --- Effect : Retrieves the current directory of this process

	virtual HRESULT SetCurrentDirectory(COXUNC UNC);
	// --- In  : UNC : The directory to make current
	// --- Out : 
	// --- Returns : The result code
	// --- Effect : Sets the current directory of this process

	virtual HRESULT GetWindowsDirectory(COXUNC& UNC);
	// --- In  : 
	// --- Out : UNC : The windows directory if succesful (empty otherwise)
	// --- Returns : The result code
	// --- Effect : Retrieves the windows directory

	virtual HRESULT GetSystemDirectory(COXUNC& UNC);
	// --- In  : 
	// --- Out : UNC : The windows system directory if succesful (empty otherwise)
	// --- Returns : The result code
	// --- Effect : Retrieves the windows system directory

	virtual HRESULT GetApplicationDirectory(COXUNC& UNC);
	// --- In  : 
	// --- Out : UNC : The application directory if succesful (empty otherwise)
	// --- Returns : The result code
	// --- Effect : Retrieves the directory in which this executable is located

	virtual HRESULT GetTemporaryDirectory(COXUNC& UNC);
	// --- In  : 
	// --- Out : UNC : The temporary directory if succesful (empty otherwise)
	// --- Returns : The result code
	// --- Effect : Retrieves the temporary directory

	virtual HRESULT GetSize(COXUNC UNC, DWORDLONG& nSize);
	// --- In  : UNC : The UNC (file) to query 
	// --- Out : nSize : The size in bytes of the specified file
	// --- Returns : The result code
	// --- Effect : Retrieves the size of a file

	virtual HRESULT GetCompressedSize(COXUNC UNC, DWORDLONG& nSize);
	// --- In  : UNC : The UNC (file) to query 
	// --- Out : nSize : The compressed size in bytes of the specified file
	// --- Returns : The result code
	// --- Effect : Retrieves the compressed size of a file

	virtual HRESULT SetSize(COXUNC UNC, DWORDLONG nSize);
	// --- In  : UNC : The UNC (file) to change
	//			 nSize : The new size  in bytes of the specified file
	// --- Out : 
	// --- Returns : The result code
	// --- Effect : Sets the new size of a file
	//				When the file is enlarged, the added bytes have an undefined value

	virtual HRESULT GetShortName(COXUNC UNC, COXUNC& shortUNC);
	// --- In  : UNC : The UNC 
	// --- Out : shortUNC : The short (8.3) name of the UNC if successful or empty otherwise
	// --- Returns : The result code
	// --- Effect : Converst a UNC to an 8.3-format name

	virtual HRESULT GetLongName(COXUNC UNC, COXUNC& longUNC);
	// --- In  : UNC : The UNC 
	// --- Out : longUNC : The Long File Name of the specified UNC
	// --- Returns : The result code
	// --- Effect : Converst an 8.3-format name toa LFN

	virtual HRESULT GetLogicalDrives(CString& sDrives);	// delimiter '|'
	// --- In  : 
	// --- Out : sDrives : A string containg all the logical drives seperated by a 
	//				vertical line ('|') if successful, empty otherwise
	// --- Returns : The result code
	// --- Effect : Returns a list of logical drives

	virtual HRESULT GetTotalDiskSpace(COXUNC UNC, DWORDLONG& nTotalSpace);
	// --- In  : UNC : The UNC (root)
	// --- Out : nTotalSpace :  The total space on the disk or 0 otherwise
	// --- Returns : The result code
	// --- Effect : Retrieves to total size of a disk

	virtual HRESULT GetFreeDiskSpace(COXUNC UNC, DWORDLONG& nFreeSpace);
	// --- In  : UNC : The UNC (root)
	// --- Out : nFreeSpace :  The free space on the disk or 0 otherwise
	// --- Returns : The result code
	// --- Effect : Retrieves to free size of a disk

	virtual HRESULT GetDriveType(COXUNC UNC, UINT& nDriveType);
	// --- In  : UNC : The UNC (root)
	// --- Out : nDriveType : The type of drive if successful
	// --- Returns : The result code
	// --- Effect : Retrieves to type of drive.  Possible values in case of success are
	//				DRIVE_REMOVABLE, DRIVE_FIXED, DRIVE_REMOTE, DRIVE_CDROM, DRIVE_RAMDISK

	virtual HRESULT GetDiskInformation(COXUNC UNC, CString& sName, DWORD& nSerialNumber, 
		DWORD& nMaximumComponentLength, DWORD& nFileSystemFlags, CString& sFileSystemName);
	// --- In  : UNC : The UNC (root)
	// --- Out : sName : The (volume) name of the disk
	//			 nSerialNumber : The serial number of the disk
	//			 nMaximumComponentLength : The maximum length, in characters, of a filename 
	//				component supported by the specified file system
	//			 nFileSystemFlags : Any combination of the following
	//				FS_CASE_IS_PRESERVED : File system preserves the case of filenames
	//				FS_CASE_SENSITIVE : File system supports case-sensitive filenames
	//				FS_UNICODE_STORED_ON_DISK : The file system supports Unicode in filenames
	//				FS_PERSISTENT_ACLS : File system preserves and enforces ACLs. 
	//				FS_FILE_COMPRESSION	 : File system supports file-based compression.
	//				FS_VOL_IS_COMPRESSED : The specified volume is a compressed volume
 	//			 sFileSystemName : The name of the file system (e.g. "NTFS", "FAT" etc)
	// --- Returns : The result code
	// --- Effect : Retrieves information about a drive

	virtual HRESULT SetDiskName(COXUNC UNC, LPCTSTR pszDiskName);
	// --- In  : UNC : The UNC (root)
	//			 pszDiskName : The new name of the disk
	// --- Out : 
	// --- Returns : The result code
	// --- Effect : Gives the specified valome a new name

	virtual HRESULT GetAttributes(COXUNC UNC, DWORD& nAttributes);
	// --- In  : UNC : The UNC
	// --- Out : nAttributes : 
	// --- Returns : The result code
	// --- Effect : Retrieves the attributes of the specified directory or file
	//				(FILE_ATTRIBUTE_ARCHIVE, FILE_ATTRIBUTE_HIDDEN, FILE_ATTRIBUTE_NORMAL,
	//				FILE_ATTRIBUTE_OFFLINE, FILE_ATTRIBUTE_READONLY, FILE_ATTRIBUTE_SYSTEM, 
	//				FILE_ATTRIBUTE_TEMPORARY, FILE_ATTRIBUTE_COMPRESSED, FILE_ATTRIBUTE_DIRECTORY)

	virtual HRESULT SetAttributes(COXUNC UNC, DWORD nAttributes);
	// --- In  : UNC : The UNC
	//			 nAttributes : The new attributes to set
	// --- Out : 
	// --- Returns : The result code
	// --- Effect : Sets the attributes of the specified directory or file
	//				(FILE_ATTRIBUTE_ARCHIVE, FILE_ATTRIBUTE_HIDDEN, FILE_ATTRIBUTE_NORMAL,
	//				FILE_ATTRIBUTE_OFFLINE, FILE_ATTRIBUTE_READONLY, FILE_ATTRIBUTE_SYSTEM, 
	//				FILE_ATTRIBUTE_TEMPORARY)

	virtual HRESULT GetCompression(COXUNC UNC, USHORT& nCompressionState);
	// --- In  : UNC : The UNC
	// --- Out : nCompressionState : The compression state of the UNC if successful or
	//				0 otherwise
	// --- Returns : The result code
	// --- Effect : Retrieves the compression state of the specified directory or file
	//				Possible values are 
	//					COMPRESSION_FORMAT_NONE
	//					COMPRESSION_FORMAT_LZNT1

	virtual HRESULT SetCompression(COXUNC UNC, USHORT nCompressionState);
	// --- In  : UNC : The UNC
	//			 nCompressionState : The compression state of the UNC
	// --- Out : 
	// --- Returns : The result code
	// --- Effect : Sets the compression state of the specified directory or file
	//				Possible values are 
	//					COMPRESSION_FORMAT_NONE
	//					COMPRESSION_FORMAT_LZNT1

	virtual HRESULT GetTime(COXUNC UNC, CTime& creationTime, CTime& lastAccessTime, CTime& lastWriteTime);
	// --- In  : UNC : The UNC
	// --- Out : creationTime : The creation time
	//			 lastAccessTime : The last access time
	//			 lastWriteTime :  The last write time
	// --- Returns : The result code
	// --- Effect : Gets the file times

	virtual HRESULT SetTime(COXUNC UNC, CTime creationTime, CTime lastAccessTime, CTime lastWriteTime);
	// --- In  : UNC : The UNC
	//			 creationTime : The new creation time or 0 if not to set this time
	//			 lastAccessTime : The new last access time or 0 if not to set this time
	//			 lastWriteTime :  The new last write time or 0 if not to set this time
	// --- Out : 
	// --- Returns : The result code
	// --- Effect : Sets the new file times

	virtual HRESULT FindFirstFile(COXUNC UNC, HANDLE &hFindFile, WIN32_FIND_DATA &findFileData,
		BOOL bOnlyDirectories = FALSE);
	// --- In  : UNC : The UNC
	//			 bOnlyDirectories : Whether this function should only return directories
	//                              and not normal files (Note : this is only a hint parameter
	//                              and may be ignored by the file system)
	// --- Out : hFindFile : The handle to be use in the next iterations, 
	//						 or NULL (not INVALID_HANDLE_VALUE) when failed
	//			 findFileData : Data about the file found
	// --- Returns : The result code
	// --- Effect : Searches a directory for a file whose name matches the specified 
	//              filename. Normally this function examines subdirectory names as well 
	//				as filenames. 
	//              Even when only directories are requested the file system 
	//				may also return files as well as subdirectories


	virtual HRESULT FindNextFile(HANDLE hFindFile, WIN32_FIND_DATA &findFileData);
	// --- In  : hFindFile : A valid find handle
	// --- Out : findFileData : Data about the file found
	// --- Returns : The result code
	// --- Effect : Searches a directory for the next file whose name matches the specified 
	//              filename. This function examines subdirectory names as well 
	//				as filenames. 

	virtual HRESULT FindClose(HANDLE hFindFile);
	// --- In  : hFindFile : A valid find handle
	// --- Out : 
	// --- Returns : The result code
	// --- Effect : Closes the find handle to end the search
	//				 The handle cannot be used in subsequent calls to either 
	//				 the FindNextFile or FindClose function. 

#ifdef _DEBUG
	virtual void AssertValid() const;
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : AssertValid performs a validity check on this object 
	//				by checking its internal state. 
	//				In the Debug version of the library, AssertValid may assert and 
	//				thus terminate the program.
	
	virtual void Dump(CDumpContext& dc) const;
	// --- In  : dc : The diagnostic dump context for dumping, usually afxDump.
	// --- Out : 
	// --- Returns :
	// --- Effect : Dumps the contents of the object to a CDumpContext object. 
	//				It provides diagnostic services for yourself and 
	//				 other users of your class. 
	//				Note  The Dump function does not print a newline character
	//				 at the end of its output. 
#endif

	virtual ~COXUNCStandardActor();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object

protected:

#ifdef _DEBUG
	static CString GetResultMessage(HRESULT hResult);
#endif // _DEBUG
	BOOL GetDiskSpace(LPCTSTR pszRoot, DWORDLONG& nTotalSpace, DWORDLONG& nFreeSpace);
	BOOL TimeToFileTime(const CTime& time, LPFILETIME pFileTime);
	
private:
                   
};

#endif // __OXUNCSTANDARDACTOR_H__
// ==========================================================================
