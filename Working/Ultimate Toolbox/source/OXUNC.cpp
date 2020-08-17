// ==========================================================================
//				Class Implementation : COXUNC
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
			  
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXUNC.h"
#include "OXUNCStandardActor.h"

#include "UTBStrOp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members
// All string constants will use normal slashes (no back slashes)
// and lowercase characters
const TCHAR COXUNC::m_cNull = _T('\0');
const TCHAR COXUNC::m_cSlash = _T('/');
const TCHAR COXUNC::m_cBackslash = _T('\\');
const TCHAR COXUNC::m_cColon = _T(':');
const TCHAR COXUNC::m_cDot = _T('.');

static const TCHAR szSlashes[] = _T("/\\");
static const TCHAR szInvalidChars[] = _T("<>|\"");
static const TCHAR szSpecialChars[] = _T(":/\\");

const LPCTSTR COXUNC::m_pszSlashes = szSlashes;
const LPCTSTR COXUNC::m_pszInvalidChars = szInvalidChars;
const LPCTSTR COXUNC::m_pszSpecialChars = szSpecialChars;


// Data members -------------------------------------------------------------
// protected:
	// CString m_sUNC;
	// --- The full UNC specification
	//	   This must be the first data member of this object, so that a dumb cast
	//	    of a UNC object to a LPCTSTR will work.
	//	    (The first data member of a CString is the LPCTSTR)
	//	   This is important when working with a variable number of paramaters
	//		for a function call (e.g. printf, TRACE etc)

	// BOOL m_bMainBuilt;
	// --- Whether the main part has been built
	//	   When this is FALSE the value of m_sUNC should not be used

	// CString m_sServer;
	// --- The server or drive specification
	// CString m_sShare;
	// --- The share specification
	// CString m_sDirectory;
	// --- The directory specification
	// CString m_sFile;
	// --- The file specification

	// BOOL m_bPartsBuilt;
	// --- Whether the parts have been built
	//	   When this is FALSE the values of m_sServer, m_sShare, m_sDirectory and 
	//		m_sFile should not be used

	// BOOL m_bURLPart;
	// --- Whether this is part of a URL

	// HRESULT m_nLastError;
	// --- The last error that occurred

	// COXUNCStandardActor* m_pActor;
	// --- The actor used by this UNC


// private:
	
// Member functions ---------------------------------------------------------
// public:

COXUNC::COXUNC(LPCTSTR pszUNC /* = NULL */, BOOL bURLPart /* = FALSE */)
	{
	m_sUNC = pszUNC;
	m_bURLPart = bURLPart;
	m_nLastError = ERROR_SUCCESS;

	// ... Use the one and only standard actor
	m_pActor = &COXUNCStandardActor::m_theOneAndOnly;
	ASSERT_VALID(m_pActor);

	SetMainBuilt();
	DestroyParts();
	}

COXUNC::COXUNC(LPCTSTR pszServer, LPCTSTR pszShare, LPCTSTR pszDirectory, 
	LPCTSTR pszFile, BOOL bURLPart /* = FALSE */)
	{
	m_sServer = pszServer;
	m_sShare = pszShare;
	m_sDirectory = pszDirectory;
	m_sFile = pszFile;
	m_bURLPart = bURLPart;
	m_nLastError = ERROR_SUCCESS;

	// ... Use the one and only standard actor
	m_pActor = &COXUNCStandardActor::m_theOneAndOnly;
	ASSERT_VALID(m_pActor);

	SetPartsBuilt();
	DestroyMain();
	}

COXUNC::COXUNC(const COXUNC& UNC)
	{
	m_sUNC =		UNC.m_sUNC;
	m_bMainBuilt =	UNC.m_bMainBuilt;

	m_sServer =		UNC.m_sServer;
	m_sShare =		UNC.m_sShare;
	m_sDirectory =	UNC.m_sDirectory;
	m_sFile =		UNC.m_sFile;
	m_bPartsBuilt =	UNC.m_bPartsBuilt;
	
	m_bURLPart =	UNC.m_bURLPart;
	m_nLastError =	UNC.m_nLastError;

	// ... Use the one and only standard actor
	m_pActor = &COXUNCStandardActor::m_theOneAndOnly;
	ASSERT_VALID(m_pActor);
	}

COXUNC& COXUNC::operator=(const COXUNC& UNC)
	{
	if(this==&UNC)
		return *this;
		
	m_sUNC =		UNC.m_sUNC;
	m_bMainBuilt =	UNC.m_bMainBuilt;

	m_sServer =		UNC.m_sServer;
	m_sShare =		UNC.m_sShare;
	m_sDirectory =	UNC.m_sDirectory;
	m_sFile =		UNC.m_sFile;
	m_bPartsBuilt =	UNC.m_bPartsBuilt;

	m_bURLPart =	UNC.m_bURLPart;
	m_nLastError =	UNC.m_nLastError;

	// ... Use our current actor
	//	   m_pActor

	return *this;
	}

COXUNC& COXUNC::operator=(LPCTSTR pszUNC)
	{
	operator=(COXUNC(pszUNC, m_bURLPart));
	return *this;
	}

const CString COXUNC::Full() const
	{
	COXUNC copyThis(*this);
	return (LPCTSTR)copyThis;
	}

const CString COXUNC::Server() const
	{
	COXUNC copyThis(*this);
	return copyThis.Server();
	}

const CString COXUNC::Share() const
	{
	COXUNC copyThis(*this);
	return copyThis.Share();
	}

const CString COXUNC::Directory() const
	{
	COXUNC copyThis(*this);
	return copyThis.Directory();
	}

const CString COXUNC::File() const
	{
	COXUNC copyThis(*this);
	return copyThis.File();
	}

const CString COXUNC::Base() const
	{
	CString sFile = File();
	int nDotPos = sFile.ReverseFind(m_cDot);
	if (0 <= nDotPos)
		return sFile.Left(nDotPos);
	else
		return sFile;
	}

const CString COXUNC::Extension(BOOL bIncludeDot /* = TRUE */) const
	{
	CString sFile = File();
	int nDotPos = sFile.ReverseFind(m_cDot);
	if (0 <= nDotPos)
		{
		if (bIncludeDot)
			// ... Include the leading dot
			return sFile.Mid(nDotPos);
		else
			// ... Exclude the leading dot
			return sFile.Mid(nDotPos + 1);
		}
	else
		return _T("");
	}

const BOOL COXUNC::URLPart() const
	{
	return m_bURLPart;
	}

const HRESULT COXUNC::LastError() const
	{
	return m_nLastError;
	}

CString& COXUNC::Full()
	{
	BuildMain();
	// ... We pass a reference so the user may change the string
	DestroyParts();

	return m_sUNC;
	}

COXUNC::operator LPCTSTR()
	{
	BuildMain();
	return m_sUNC;
	}

CString& COXUNC::Server()
	{
	BuildParts();
	// ... We pass a reference so the user may change the string
	DestroyMain();

	return m_sServer;
	}

CString& COXUNC::Share()
	{
	BuildParts();
	// ... We pass a reference so the user may change the string
	DestroyMain();

	return m_sShare;
	}

CString& COXUNC::Directory()
	{
	BuildParts();
	// ... We pass a reference so the user may change the string
	DestroyMain();

	return m_sDirectory;
	}

CString& COXUNC::File()
	{
	BuildParts();
	// ... We pass a reference so the user may change the string
	DestroyMain();

	return m_sFile;
	}

BOOL& COXUNC::URLPart()
	{
	return m_bURLPart;
	}

HRESULT& COXUNC::LastError()
	{
	return m_nLastError;
	}

TCHAR COXUNC::PreferedSlash() const
	{
	if (m_bURLPart)
		return COXUNC::m_cSlash;
	else
		return COXUNC::m_cBackslash;
	}

CString COXUNC::StandardForm(BOOL bMakeLower /* = TRUE */) const
	{
	CString sStandardUNC = Full();
	if (PreferedSlash() == m_cBackslash)
		ConvertSlashToBackslash(sStandardUNC);
	else
		ConvertBackslashToSlash(sStandardUNC);
	if (bMakeLower)
		sStandardUNC.MakeLower();
	return sStandardUNC;
	}

CString COXUNC::FileForm() const
	{
	CString sDir = Directory();
	CString sFile = File();
	if (sFile.IsEmpty() && (1 < sDir.GetLength()))
		{
		// ... Last character must be a slash
		ASSERT(_tcschr(m_pszSlashes, sDir[sDir.GetLength() - 1]) != NULL);
		// ... Remove it
		sDir = sDir.Left(sDir.GetLength() - 1);
		}
	CString sFileForm = Server() + Share() + sDir + sFile;
	return sFileForm;
	}

BOOL COXUNC::IsEmpty() const
	{
	return Full().IsEmpty();
	}

void COXUNC::Empty()
	{
	m_sUNC.Empty();
	m_bMainBuilt = FALSE;

	m_sServer.Empty();
	m_sShare.Empty();
	m_sDirectory.Empty();
	m_sFile.Empty();
	m_bPartsBuilt = FALSE;

	// ... Keep the present actor
	}

BOOL COXUNC::MakeAbsolute()
	{
	ASSERT_VALID(m_pActor);
	m_nLastError = m_pActor->MakeAbsolute(*this);
	return (SUCCEEDED(m_nLastError));
	}

BOOL COXUNC::Exists()
	{
	ASSERT_VALID(m_pActor);
	BOOL bExists = FALSE;
	m_nLastError = m_pActor->Exists(*this, bExists);
	return bExists;
	}

BOOL COXUNC::Create()
	{
	ASSERT_VALID(m_pActor);
	m_nLastError = m_pActor->Create(*this);
	return (SUCCEEDED(m_nLastError));
	}

BOOL COXUNC::CreateTemporaryFile(LPCTSTR pszPrefix /* = NULL */)
	{
	ASSERT_VALID(m_pActor);

	COXUNC tempDir;
	if (!tempDir.GetTemporaryDirectory())
		{
		ASSERT(FAILED(m_nLastError));
		return FALSE;
		}

	m_nLastError = m_pActor->CreateTemporaryFile(tempDir, pszPrefix, *this);
	return (SUCCEEDED(m_nLastError));
	}

BOOL COXUNC::Copy(COXUNC destinationUNC, BOOL bReplaceExisting /* = FALSE */)
	{
	ASSERT_VALID(m_pActor);
	m_nLastError = m_pActor->Copy(*this, destinationUNC, bReplaceExisting);
	return (SUCCEEDED(m_nLastError));
	}

BOOL COXUNC::Move(COXUNC destinationUNC, BOOL bReplaceExisting /* = FALSE */, 
	BOOL bCopyAllowed /* = TRUE */)
	{
	ASSERT_VALID(m_pActor);
	DWORD dwFlags = (bReplaceExisting ? MOVEFILE_REPLACE_EXISTING : 0) | (bCopyAllowed ? MOVEFILE_COPY_ALLOWED : 0);
	m_nLastError = m_pActor->MoveRename(*this, destinationUNC, dwFlags);
	return (SUCCEEDED(m_nLastError));
	}

BOOL COXUNC::Rename(COXUNC destinationUNC, BOOL bReplaceExisting /* = FALSE */)
	{
	ASSERT_VALID(m_pActor);
	DWORD dwFlags = (bReplaceExisting ? MOVEFILE_REPLACE_EXISTING : 0);
	m_nLastError = m_pActor->MoveRename(*this, destinationUNC, dwFlags);
	return (SUCCEEDED(m_nLastError));
	}

BOOL COXUNC::Delete(BOOL bRemoveReadOnly /* = FALSE */)
	{
	ASSERT_VALID(m_pActor);
	m_nLastError = m_pActor->Delete(*this, bRemoveReadOnly);
	return (SUCCEEDED(m_nLastError));
	}

BOOL COXUNC::SetCurrentDirectory()
	{
	ASSERT_VALID(m_pActor);
	m_nLastError = m_pActor->SetCurrentDirectory(*this);
	return (SUCCEEDED(m_nLastError));
	}

BOOL COXUNC::GetCurrentDirectory()
	{
	ASSERT_VALID(m_pActor);
	m_nLastError = m_pActor->GetCurrentDirectory(*this);
	return (SUCCEEDED(m_nLastError));
	}

BOOL COXUNC::GetWindowsDirectory()
	{
	ASSERT_VALID(m_pActor);
	m_nLastError = m_pActor->GetWindowsDirectory(*this);
	return (SUCCEEDED(m_nLastError));
	}

BOOL COXUNC::GetSystemDirectory()
	{
	ASSERT_VALID(m_pActor);
	m_nLastError = m_pActor->GetSystemDirectory(*this);
	return (SUCCEEDED(m_nLastError));
	}

BOOL COXUNC::GetApplicationDirectory()
	{
	ASSERT_VALID(m_pActor);
	m_nLastError = m_pActor->GetApplicationDirectory(*this);
	return (SUCCEEDED(m_nLastError));
	}

BOOL COXUNC::GetTemporaryDirectory()
	{
	ASSERT_VALID(m_pActor);
	m_nLastError = m_pActor->GetTemporaryDirectory(*this);
	return (SUCCEEDED(m_nLastError));
	}

LONGLONG COXUNC::GetSize()
	{
	ASSERT_VALID(m_pActor);
	DWORDLONG nSize;
	m_nLastError = m_pActor->GetSize(*this, nSize);
	if (SUCCEEDED(m_nLastError))
		return nSize;
	else
		return (LONGLONG)-1;
	}

LONGLONG  COXUNC::GetCompressedSize()
	{
	ASSERT_VALID(m_pActor);
	DWORDLONG nSize;
	m_nLastError = m_pActor->GetCompressedSize(*this, nSize);
	if (SUCCEEDED(m_nLastError))
		return nSize;
	else
		return (LONGLONG)-1;
	}

BOOL COXUNC::SetSize(LONGLONG nNewSize)
	{
	ASSERT_VALID(m_pActor);
	if (nNewSize < 0)
		nNewSize = 0;
	m_nLastError = m_pActor->SetSize(*this, (DWORDLONG)nNewSize);
	return (SUCCEEDED(m_nLastError));
	}

COXUNC COXUNC::GetShortName()
	{
	COXUNC shortUNC;

	ASSERT_VALID(m_pActor);
	m_nLastError = m_pActor->GetShortName(*this, shortUNC);
	return shortUNC;
	}

COXUNC COXUNC::GetLongName()
	{
	COXUNC longUNC;

	ASSERT_VALID(m_pActor);
	m_nLastError = m_pActor->GetLongName(*this, longUNC);
	return longUNC;
	}

BOOL COXUNC::GetLogicalDrives(CStringArray& drives)
{
	ASSERT_VALID(m_pActor);
	CString sDrives;
	m_nLastError = m_pActor->GetLogicalDrives(sDrives);

	drives.RemoveAll();
	if (SUCCEEDED(m_nLastError))
	{
		// Parse string (delimiter is '|') and put each drive in the array
		LPTSTR pszDrives = sDrives.GetBuffer(0);

		TCHAR * p;
		LPTSTR pszDrive = UTBStr::tcstok(pszDrives, _T("|"), &p);
		while (pszDrive != NULL)
		{
			drives.Add(pszDrive);
			pszDrive = UTBStr::tcstok(pszDrives, _T("|"), &p);
		}
		sDrives.ReleaseBuffer();
	}

	return (SUCCEEDED(m_nLastError));
}

LONGLONG COXUNC::GetTotalDiskSpace()
	{
	ASSERT_VALID(m_pActor);
	DWORDLONG nTotalSpace;
	m_nLastError = m_pActor->GetTotalDiskSpace(*this, nTotalSpace);
	if (SUCCEEDED(m_nLastError))
		return nTotalSpace;
	else
		return (LONGLONG)-1;
	}

LONGLONG COXUNC::GetFreeDiskSpace()
	{
	ASSERT_VALID(m_pActor);
	DWORDLONG nFreeSpace;
	m_nLastError = m_pActor->GetFreeDiskSpace(*this, nFreeSpace);
	if (SUCCEEDED(m_nLastError))
		return nFreeSpace;
	else
		return (LONGLONG)-1;
	}

UINT COXUNC::GetDriveType()
	{
	ASSERT_VALID(m_pActor);
	UINT nDriveType;
	m_nLastError = m_pActor->GetDriveType(*this, nDriveType);
	if (SUCCEEDED(m_nLastError))
		return nDriveType;
	else
		return (UINT)0;
	}

CString COXUNC::GetDiskName()
	{
	ASSERT_VALID(m_pActor);

	CString sName;
	DWORD nSerialNumber = 0;
	DWORD nMaximumComponentLength = 0;
	DWORD nFileSystemFlags = 0;
	CString sFileSystemName;

	m_nLastError = m_pActor->GetDiskInformation(*this, sName, nSerialNumber, 
		nMaximumComponentLength, nFileSystemFlags, sFileSystemName);

	if (SUCCEEDED(m_nLastError))
		return sName;
	else
		return _T("");
	}

BOOL COXUNC::SetDiskName(LPCTSTR pszDiskName)
	{
	ASSERT_VALID(m_pActor);
	m_nLastError = m_pActor->SetDiskName(*this, pszDiskName);
	return (SUCCEEDED(m_nLastError));
	}

DWORD COXUNC::GetDiskSerialNumber()
	{
	ASSERT_VALID(m_pActor);

	CString sName;
	DWORD nSerialNumber = 0;
	DWORD nMaximumComponentLength = 0;
	DWORD nFileSystemFlags = 0;
	CString sFileSystemName;

	m_nLastError = m_pActor->GetDiskInformation(*this, sName, nSerialNumber, 
		nMaximumComponentLength, nFileSystemFlags, sFileSystemName);

	if (SUCCEEDED(m_nLastError))
		return nSerialNumber;
	else
		return 0;
	}

DWORD COXUNC::GetDiskMaximumComponentLength()
	{
	ASSERT_VALID(m_pActor);

	CString sName;
	DWORD nSerialNumber = 0;
	DWORD nMaximumComponentLength = 0;
	DWORD nFileSystemFlags = 0;
	CString sFileSystemName;

	m_nLastError = m_pActor->GetDiskInformation(*this, sName, nSerialNumber, 
		nMaximumComponentLength, nFileSystemFlags, sFileSystemName);

	if (SUCCEEDED(m_nLastError))
		return nMaximumComponentLength;
	else
		return 0;
	}

DWORD COXUNC::GetDiskFileSystemFlags()
	{
	ASSERT_VALID(m_pActor);

	CString sName;
	DWORD nSerialNumber = 0;
	DWORD nMaximumComponentLength = 0;
	DWORD nFileSystemFlags = 0;
	CString sFileSystemName;

	m_nLastError = m_pActor->GetDiskInformation(*this, sName, nSerialNumber, 
		nMaximumComponentLength, nFileSystemFlags, sFileSystemName);

	if (SUCCEEDED(m_nLastError))
		return nFileSystemFlags;
	else
		return 0;
	}

CString COXUNC::GetDiskFileSystemName()
	{
	ASSERT_VALID(m_pActor);

	CString sName;
	DWORD nSerialNumber = 0;
	DWORD nMaximumComponentLength = 0;
	DWORD nFileSystemFlags = 0;
	CString sFileSystemName;

	m_nLastError = m_pActor->GetDiskInformation(*this, sName, nSerialNumber, 
		nMaximumComponentLength, nFileSystemFlags, sFileSystemName);

	if (SUCCEEDED(m_nLastError))
		return sFileSystemName;
	else
		return _T("");
	}

DWORD COXUNC::GetAttributes()
	{
	ASSERT_VALID(m_pActor);
	DWORD nAttributes = 0;
	m_nLastError = m_pActor->GetAttributes(*this, nAttributes);
	if (SUCCEEDED(m_nLastError))
		return nAttributes;
	else
		return MAXDWORD;
	}

BOOL COXUNC::ModifyAttributes(DWORD nRemoveAttributes, DWORD nAddAttributes)
	{
	ASSERT_VALID(m_pActor);

	// Get the current (old) attributes
	DWORD nOldAttributes = GetAttributes();
	if (nOldAttributes == MAXDWORD)
		{
		ASSERT(FAILED(m_nLastError));
		return FALSE;
		}

	// Compute the new attributes
	DWORD nNewAttributes = (nOldAttributes & ~nRemoveAttributes) | nAddAttributes;
	if (nNewAttributes == nOldAttributes)
		{
		ASSERT(SUCCEEDED(m_nLastError));
		return TRUE;
		}

	// Set the attributes
	m_nLastError = m_pActor->SetAttributes(*this, nNewAttributes);

	// Change the compression state if necessary
	BOOL bOldCompressed = (nOldAttributes & FILE_ATTRIBUTE_COMPRESSED);
	BOOL bNewCompressed = (nNewAttributes & FILE_ATTRIBUTE_COMPRESSED);
	if (SUCCEEDED(m_nLastError) && (bOldCompressed != bNewCompressed))
		{
		USHORT nNewCompressionState = 0;
		if (bNewCompressed)
			nNewCompressionState = (USHORT)COMPRESSION_FORMAT_DEFAULT;
		else
			nNewCompressionState = (USHORT)COMPRESSION_FORMAT_NONE;
		m_nLastError = m_pActor->SetCompression(*this, nNewCompressionState);
		}

	return (SUCCEEDED(m_nLastError));
	}

CTime COXUNC::GetCreationTime()
	{
	ASSERT_VALID(m_pActor);
	CTime creationTime;
	CTime lastAccessTime;
	CTime lastWriteTime;

	m_nLastError = m_pActor->GetTime(*this, creationTime, lastAccessTime, lastWriteTime);
	if (SUCCEEDED(m_nLastError))
		return creationTime;
	else
		return CTime(0);
	}

CTime COXUNC::GetLastAccessTime()
	{
	ASSERT_VALID(m_pActor);
	CTime creationTime;
	CTime lastAccessTime;
	CTime lastWriteTime;

	m_nLastError = m_pActor->GetTime(*this, creationTime, lastAccessTime, lastWriteTime);
	if (SUCCEEDED(m_nLastError))
		return lastAccessTime;
	else
		return CTime(0);
	}

CTime COXUNC::GetLastWriteTime()
	{
	ASSERT_VALID(m_pActor);
	CTime creationTime;
	CTime lastAccessTime;
	CTime lastWriteTime;

	m_nLastError = m_pActor->GetTime(*this, creationTime, lastAccessTime, lastWriteTime);
	if (SUCCEEDED(m_nLastError))
		return lastWriteTime;
	else
		return CTime(0);
	}

BOOL COXUNC::SetCreationTime(CTime newCreationTime)
	{
	ASSERT_VALID(m_pActor);
	m_nLastError = m_pActor->SetTime(*this, newCreationTime, CTime(0), CTime(0));
	return (SUCCEEDED(m_nLastError));
	}

BOOL COXUNC::SetLastAccessTime(CTime newLastAccessTime)
	{
	ASSERT_VALID(m_pActor);
	m_nLastError = m_pActor->SetTime(*this, CTime(0), newLastAccessTime, CTime(0));
	return (SUCCEEDED(m_nLastError));
	}

BOOL COXUNC::SetLastWriteTime(CTime newLastWriteTime)
	{
	ASSERT_VALID(m_pActor);
	m_nLastError = m_pActor->SetTime(*this, CTime(0), CTime(0), newLastWriteTime);
	return (SUCCEEDED(m_nLastError));
	}

COXUNC COXUNC::GetRoot() const
	{
	COXUNC rootUNC(*this);

	// ... MakeAbsolute may fail
	rootUNC.MakeAbsolute();
	// ... Remove directory and file name
	rootUNC.Directory() = PreferedSlash();
	rootUNC.File().Empty();

	return rootUNC;
	}

COXUNC COXUNC::GetTitle() const
	{
	COXUNC titleUNC(*this);

	// ... MakeAbsolute may fail
	titleUNC.MakeAbsolute();
	// ... Remove extension
	titleUNC.File() = titleUNC.Base();

	return titleUNC;
	}

CString COXUNC::GetAbbreviation(int nMaxLength, BOOL bAtLeastFile /* = TRUE */) const
	// See also the MFC function AbbreviateName() in filelist.cpp
	{
	CString sUNC = Full();
	CString sServer;
	CString sShare;
	CString sDirectory;
	CString sFile;
	
	// If nMaxLength is more than enough to hold the full UNC name, we're done.
	// This is probably a pretty common case, so we'll put it first.
	if (sUNC.GetLength() <= nMaxLength)
		return Full();
	
	// If nMaxLength isn't enough to hold at least the filename, we're done
	sFile = File();
	if (nMaxLength < sFile.GetLength())
		{
		if (bAtLeastFile)
			return sFile;
		else
			return _T("");
		}
	
	// If nMaxLength isn't enough to hold at least <server><share>\...\<file>, the
	// result is the filename.
	sServer = Server();
	sShare = Share();
	sDirectory = Directory();
	CString sAbbr = PreferedSlash() + CString(_T("...")) + PreferedSlash();
	int nServerShareFileLength = sServer.GetLength() + sShare.GetLength() + sFile.GetLength();
	if (nMaxLength < nServerShareFileLength + sAbbr.GetLength())
		return sFile;
	
	// Now loop through the remaining directory components until something
	// of the form <server><share>\...\<one_or_more_dirs>\<file> fits.
	int nRemainingSpace = nMaxLength - nServerShareFileLength - sAbbr.GetLength();
	ASSERT(0 <= nRemainingSpace);
	LPCTSTR pszStartDirectory = (LPCTSTR)sDirectory;
	LPCTSTR pszSearchDirectory = pszStartDirectory + sDirectory.GetLength();
	LPCTSTR pszUsableDirectoryPart = pszSearchDirectory;
	// ... Add one to the remainings space for the directory
	//     bacause we are counting the number of characters in the directory
	//	   but we will remove its leading slash (so one extra character is allowed)
	nRemainingSpace++;
	while ((pszStartDirectory < pszSearchDirectory) && (0 < nRemainingSpace))
		{
		pszSearchDirectory--;
		nRemainingSpace--;
		if (_tcschr(m_pszSlashes, *pszSearchDirectory) != NULL) 
			// ... Do not include the leading slash
			pszUsableDirectoryPart = pszSearchDirectory + 1;
		}

	return sServer + sShare + sAbbr + pszUsableDirectoryPart + sFile;
	}


#ifdef _DEBUG
#endif //_DEBUG

COXUNC::~COXUNC()
	{
	// ... Remove association with the actor (do not delete the standard actor)
	m_pActor = NULL;
	}

// protected:
void COXUNC::BuildMain()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Builds the main UNC from the parts
	{
	if (IsMainBuilt())
		{
		// ... Re-adjust main data (may have changed)
		AdjustMain();
		return;
		}

	if (!ArePartsBuilt())
		{
		// Nothing has been built yet, clean everything and set to built
		Empty();
		SetPartsBuilt();
		return;
		}

	// ... Adjust parts before we use them
	ASSERT(ArePartsBuilt());
	AdjustParts();

	// Build main data
	m_sUNC = m_sServer + m_sShare + m_sDirectory + m_sFile;

	// ... Adjust main after we have built it
	AdjustMain();

	// ... Mark that the main data has been built now
	SetMainBuilt();
	return;
	}

void COXUNC::BuildParts()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Builds the different parts from the full UNC
	{
	if (ArePartsBuilt())
		{
		// ... Re-adjust parts data (may have changed)
		AdjustParts();
		return;
		}

	if (!IsMainBuilt())
		{
		// Nothing has been built yet, clean everything and set to built
		Empty();
		SetMainBuilt();
		return;
		}

	// ... Adjust main before we use it
	ASSERT(IsMainBuilt());
	AdjustMain();

	// Build the parts

	// There are three possibilities for server
	// 1. An actual UNC name (e.g. \\Comp1\Share\Dir\File.ext : \\Comp1, \Share, \Dir\, File.ext)
	//	  Always starts with two (back)slashes
	// 2. A TCP/IP server name (e.g. server.company.com/Dir/File.ext : server.company.com, , /Dir/, File.ext)
	//	  m_bURLPart == TRUE
	// 3. A local file specification (e.g. C:\Dir\File.ext : C:, ,\Dir\, File.ext)

	// ... First make a copy with only normal slashes (no backslashes) and lowercase characters
	// Possibility 2 and 3 can be handled the same way

	// All parts are empty by default
	m_sServer.Empty();
	m_sShare.Empty();
	m_sDirectory.Empty();
	m_sFile.Empty();

	if ((2 <= m_sUNC.GetLength()) &&
		(_tcschr(m_pszSlashes, *(LPCTSTR)m_sUNC) != NULL) &&
		(_tcschr(m_pszSlashes, *((LPCTSTR)m_sUNC + 1)) != NULL) )
		{
		// Possibility 1 : Actual UNC
		// ... Search for first slash after Server name
		LPCTSTR pszShare = _tcspbrk((LPCTSTR)m_sUNC + 2, m_pszSlashes); 
		if (pszShare != NULL)
			{
			// ... Share found
			INT_PTR nServerLength = pszShare - (LPCTSTR)m_sUNC;
			UTBStr::tcsncpy(m_sServer.GetBufferSetLength((int)nServerLength), (int)nServerLength+1, m_sUNC, (int)nServerLength);
			// ... Search for first slash after Share name
			LPCTSTR pszDir = _tcspbrk(pszShare + 1, m_pszSlashes); 
			if (pszDir != NULL)
				{
				// ... Directory found
				INT_PTR nShareLength = pszDir - pszShare;
				UTBStr::tcsncpy(m_sShare.GetBufferSetLength((int)nShareLength), (int)nShareLength+1, pszShare, (int)nShareLength);
				// ... Search for last slash after Share name
				LPCTSTR pszLastSlash = _tcsrchr(pszDir + 1, m_cSlash);
				LPCTSTR pszLastBackslash = _tcsrchr(pszDir + 1, m_cBackslash);
				LPCTSTR pszFile = pszLastSlash < pszLastBackslash ? pszLastBackslash : pszLastSlash;
				if (pszFile != NULL)
					{
					// ... File found (skip leading slash)
					pszFile++;
					INT_PTR nDirLength = pszFile - pszDir;
					UTBStr::tcsncpy(m_sDirectory.GetBufferSetLength((int)nDirLength), (int)nDirLength+1, pszDir, (int)nDirLength);
					m_sFile = pszFile;
					}
				else
					{
					// ... No file found after directory, interpret directory as file
					//     and use root as directory
					//		E.g. \Test -> Dir : \, File : Test
					// ... First character must be a slash
					ASSERT(_tcschr(m_pszSlashes, *pszDir) != NULL);
					UTBStr::tcsncpy(m_sDirectory.GetBufferSetLength(1), 2, pszDir, 1);
					m_sFile = pszDir + 1;
					}
				}
			else
				{
				// ... Directory not found
				m_sShare = pszShare;
				}
			}
		else
			{
			// ... Share not found
			m_sServer = m_sUNC;
			}
		}
	else if (URLPart() && (_tcschr(m_sUNC, m_cColon) == NULL))
		{
		// Possibility 2 : A TCP/IP URL part
		//				   A URL should contain an absolute specification so by checking
		//				   for a colon we can see whether it is a TCP/IP name or local name
		// ... Search for first slash after Server name
		LPCTSTR pszDir = _tcspbrk(m_sUNC, m_pszSlashes); 
		if (pszDir != NULL)
			{
			// ... Directory found
			INT_PTR nServerLength = pszDir - (LPCTSTR)m_sUNC;
			UTBStr::tcsncpy(m_sServer.GetBufferSetLength((int)nServerLength), (int)nServerLength+1, m_sUNC, (int)nServerLength);
			// ... Search for last slash after Directory name
			LPCTSTR pszLastSlash = _tcsrchr(pszDir + 1, m_cSlash);
			LPCTSTR pszLastBackslash = _tcsrchr(pszDir + 1, m_cBackslash);
			LPCTSTR pszFile = pszLastSlash < pszLastBackslash ? pszLastBackslash : pszLastSlash;
			if (pszFile != NULL)
				{
				// ... File found (skip leading slash)
				pszFile++;
				INT_PTR nDirLength = pszFile - pszDir;
				UTBStr::tcsncpy(m_sDirectory.GetBufferSetLength((int)nDirLength), (int)nDirLength+1, pszDir, (int)nDirLength);
				m_sFile = pszFile;
				}
			else
				{
				// ... No file found after directory, interpret directory as file
				//     and use root as directory
				//		E.g. \Test -> Dir : \, File : Test
				// ... First character must be a slash
				ASSERT(_tcschr(m_pszSlashes, *pszDir) != NULL);
				UTBStr::tcsncpy(m_sDirectory.GetBufferSetLength(1), 2, pszDir, 1);
				m_sFile = pszDir + 1;
				}
			}
		else
			{
			// ... Directory not found
			m_sServer = m_sUNC;
			}
		}
	else
	{
		// Possibility  3 : A local file
		CString sBaseName;
		CString sExtension;

		// ... _tsplitpath will return the current directory if m_sUNC is empty
		//     We do not want that.
		if (!m_sUNC.IsEmpty())
		{
			UTBStr::tsplitpath(m_sUNC, m_sServer.GetBuffer(_MAX_DRIVE), _MAX_DRIVE,
				m_sDirectory.GetBuffer(_MAX_DIR), _MAX_DIR,
				sBaseName.GetBuffer(_MAX_FNAME), _MAX_FNAME,
				sExtension.GetBuffer(_MAX_EXT), _MAX_EXT);
			m_sServer.ReleaseBuffer();
			m_sDirectory.ReleaseBuffer();
			sBaseName.ReleaseBuffer();
			sExtension.ReleaseBuffer();
			m_sFile = sBaseName + sExtension;
		}
	}

	// ... Adjust parts after we have built them
	AdjustParts();

	// ... Mark that the parts data has been built now
	SetPartsBuilt();
	return;
	}

void COXUNC::AdjustMain()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Adjust the main (full URL)
	{
	Trim(m_sUNC);
	}

void COXUNC::AdjustParts()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Adjust the URL parts 
	{
	Trim(m_sServer);
	Trim(m_sShare);
	Trim(m_sDirectory);
	Trim(m_sFile);

	// Adjust server
	// ... Is a share exists, make sure the server starts with a double (back)slash
	if (!m_sShare.IsEmpty())
		if (_tcschr(m_pszSlashes, *(LPCTSTR)m_sServer) == NULL)
			// ... Add two backslashes
			m_sServer = CString(m_cBackslash) + m_cBackslash + m_sServer;
		else if (_tcschr(m_pszSlashes, *((LPCTSTR)m_sServer + 1)) == NULL)
			// ... Duplicate the first character
			m_sServer = *(LPCTSTR)m_sServer + m_sServer;

	// Adjust share
	// ... Add leading backslash if necessary
	//     (if the share is not empty then it must end in a backslash)
	if (!m_sShare.IsEmpty() && 
		(*(LPCTSTR)m_sShare != m_cBackslash) &&
		(*(LPCTSTR)m_sShare != m_cSlash) )
		m_sShare = m_cBackslash + m_sShare;

	// Adjust directory
	// ... Add leading backslash if necessary
	//     (If the server is not empty and does not end in a colon
	//      or the share is not empty then the directory must start with a backslash)
	if ( ((!m_sServer.IsEmpty() && (m_sServer.Right(1) != m_cColon)) ||
	     !m_sShare.IsEmpty()) && 
		!m_sDirectory.IsEmpty() && 
		(*(LPCTSTR)m_sDirectory != m_cBackslash) &&
		(*(LPCTSTR)m_sDirectory != m_cSlash) )
		m_sDirectory = PreferedSlash() + m_sDirectory;
	// ... Add trailing backslash if necessary
	//     (if the directory is not empty then it must end in a backslash)
	if (!m_sDirectory.IsEmpty() && 
		(m_sDirectory.Right(1) != m_cBackslash) &&
		(m_sDirectory.Right(1) != m_cSlash) )
		m_sDirectory += PreferedSlash();

	// Adjust file
	// ... If the file ends in a backslash, it is a directory, not a file
	if (!m_sFile.IsEmpty() && 
		(m_sFile.Right(1) == m_cBackslash) ||
		(m_sFile.Right(1) == m_cSlash) )
		{
		m_sDirectory += m_sFile;
		m_sFile.Empty();

		// Recheck the directory
		// ... Add leading backslash if necessary
		//     (If the server is not empty and does not end in a colon
		//      or the share is not empty then the directory must start with a backslash)
		ASSERT(!m_sDirectory.IsEmpty());
		if ( ((!m_sServer.IsEmpty() && (m_sServer.Right(1) != m_cColon)) ||
			 !m_sShare.IsEmpty()) && 
			(*(LPCTSTR)m_sDirectory != m_cBackslash) &&
			(*(LPCTSTR)m_sDirectory != m_cSlash) )
			m_sDirectory = PreferedSlash() + m_sDirectory;
		}
	}

void COXUNC::Trim(CString& sText)
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Removes invalid characters and leading and trailing spaces
	{
	// ... Remove invalid chars	<>|"
	int nInvalidCharPos = sText.FindOneOf(m_pszInvalidChars);
	while (0 <= nInvalidCharPos)
		{
		sText = sText.Left(nInvalidCharPos) + sText.Mid(nInvalidCharPos + 1);
		nInvalidCharPos = sText.FindOneOf(m_pszInvalidChars);
		}

	// ... Remove leading and trailng spaces
	sText.TrimLeft();
	sText.TrimRight();
	}

void COXUNC::ConvertBackslashToSlash(CString& sText)
	// --- In  : sText
	// --- Out : sText
	// --- Returns :
	// --- Effect : Converts all ocurrences of a backslash to a slash
	{
	LPTSTR pszText = sText.GetBuffer(0);
	while (*pszText != m_cNull)
		{
		if (*pszText == m_cBackslash)
			*pszText = m_cSlash;
		pszText++;
		}
	sText.ReleaseBuffer();
	}

void COXUNC::ConvertSlashToBackslash(CString& sText)
	// --- In  : sText
	// --- Out : sText
	// --- Returns :
	// --- Effect : Converts all ocurrences of a slash to a backslash
	{
	LPTSTR pszText = sText.GetBuffer(0);
	while (*pszText != m_cNull)
		{
		if (*pszText == m_cSlash)
			*pszText = m_cBackslash;
		pszText++;
		}
	sText.ReleaseBuffer();
	}

// private:

// ==========================================================================