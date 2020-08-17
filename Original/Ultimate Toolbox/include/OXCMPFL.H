// ==========================================================================
// 							Class Specification : COXCompressedFile
// ==========================================================================

// Header file : oxcmpfl.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CFile

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO  Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	Yes	Uses exceptions (same as CFile)

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This class enhances the CFile bass class by enabling compression

// Remark:
// COXCompressedFile::GetLength() isn't const anymore

// Prerequisites (necessary conditions):
//		
// - You cannot read and write at the same time
// - Seek can only be used with read
// - Lock and UnLock are not permitted
// - GetPosition is always permitted
// - Write only sequential
// - Read direct access
// - m_nHeaderLength allways < m_wBufferLength
// - Trying to seek after end of file will not throw an exception but will 
// - place the filepointer after the last valid position
// - Trying to seek before begin of file throws an exception
// - Flushing is not permitted.  The class handles all flushing automatically.
/////////////////////////////////////////////////////////////////////////////

#ifndef __COMPFILE_H__
#define __COMPFILE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#define MAX_OUT_LENGTH  16

#include "oxcompr.h"

class OX_CLASS_DECL COXCompressedFile : public CFile
{
DECLARE_DYNAMIC(COXCompressedFile)

// Data members -------------------------------------------------------------
public:
	
protected:
	BOOL				m_bEOF;
	BOOL				m_bReadingHeader;
	BOOL				m_bOpenedForRead;
	BOOL				m_bOpenedForWrite;
    COXCompressor		m_Convert;
	int					m_nFirstBufferLength;
	long				m_lPosition;
    long				m_lFileLength;
    long            	m_lPrevLengthPos;
	UINT 				m_nInternalPos;
	const UINT			m_nHeaderLength;
    UINT 				m_nExpandedLength;
    unsigned char*  	m_pInternalBuf;
    WORD				m_wReadHeaderLength;
    WORD				m_wLastBufferLength;
    static const WORD 	m_wBufferLength;

	CWordArray			m_BufLenArray;

	CFile*				m_pDelegateToFile;

    private:
	
// Member functions ---------------------------------------------------------
public:
	COXCompressedFile(UINT nHeaderLength = 0);
	// --- In  : nHeaderLength, the length of the header
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object
	//				It will initialize the internal state
	
	COXCompressedFile(LPCTSTR pszFilename, UINT nOpenFlags, UINT nHeaderLength = 0);
	// --- In  : pszFilename : name of file to be opened
	//           nOpenFlags  : conditions for opening file
	//			 nHeaderLength : the length of the header
	// --- Out :
	// --- Returns
	// --- Effect    : constructor of object
	
	virtual BOOL Open(LPCTSTR pszFileName, UINT nOpenFlags,	CFileException* pError = NULL);
	// --- In  : pszFileName A string that is the path to the desired file. The path may be 
	//				relative or absolute
	//			 nOpenFlags : Defines the file's sharing and access mode.
	//			 pError : A pointer to an existing file-exception object that indicates 
	//				the completion status of the open operation.
	// --- Out : 
	// --- Returns : Whether the Open succeeded (pError is meaningfull only is OPen failed)
	// --- Effect : Opens the specified file

	BOOL DelegateOpen(CFile* pDelegateToFile, UINT nOpenFlags);
	// --- In  : pDelegateToFile : The new file pointer to delegated to.
	//			 nOpenFlags : The flags that will determine how this compressed file should
	//						  be treated.
	// --- Out : 
	// --- Returns : succesfull or not
	// --- Effect : This function enables this conversion to be inserted into a chain of transformations.
	//				This chaining requires a setup sequence of transformations.  Via this function 
	//				you tell this conversion on what file pointer he has to do his conversion.
	//				This file pointer can allready be converted himself, so that's the way to 
	//				form a sequence of transformations.  More specifically this file pointer replaces
	//				the CFile baseclass of this encrypted file.  All calls to the baseclass will now
	//				be routed(delegated) to this file pointer, which is the next in the transformation chain.

	BOOL UnDelegateOpen();
	// --- In  : 
	// --- Out : 
	// --- Returns : succesfull or not
	// --- Effect : Calling this function ensures that this transformation is not anymore
	//				chained to other transfromations.  It's again a stand-alone transformation.
	//				The state where this object will be in after calling this function, is the
	//				same when the object was just constructed.

	virtual CFile* Duplicate() const;
	// --- In  :
	// --- Out : 
	// --- Returns : NULL
	// --- Effect : This is a not supported function in COXCompressedFile

#if _MFC_VER >= 0x0700
	virtual ULONGLONG GetPosition() const;
#else
	virtual DWORD GetPosition() const;
#endif
	// --- In  :
	// --- Out : 
	// --- Returns : The current position in the file
	// --- Effect : 

	virtual LONG Seek(LONG lOff, UINT nFrom);
	// --- In  : lOff : Number of bytes to move the file pointer
	//			 nForm : Pointer movement mode.  Must be one of the following :
	//				CFile::begin : from the beginning of the file.
	//				CFile::current : from the current position in the file.
	//				CFile::end : from the end of the file.
	// --- Out : 
	// --- Returns :If the requested position is legal Seek returns the new byte offset 
	//				from the beginning of the file. Otherwise the return value is undefined 
	//				and a CFileException object is thrown.

	// --- Effect : Contructor of object

	virtual ULONGLONG GetLength();
	// --- In  :
	// --- Out : 
	// --- Returns : The length of the file
	// --- Effect : 

	virtual void SetLength(DWORD dwNewLen);
	// --- In  :  
	// --- Out : 
	// --- Returns : 
	// --- Effect : This is a not supported function in COXCompressedFile

	virtual UINT Read(void FAR* lpBuf, UINT nCount);
	// --- In  : lpBuf : Pointer to the user-supplied buffer that is 
	//				to receive the data read from the file.
	//			 nCount : The maximum number of bytes to be read from the file
	// --- Out : 
	// --- Returns : The number of bytes transferred to the buffer. 
	//				 Note that the return value may be less than nCount if the end of file was reached. 
	// --- Effect : Reads bytes from the file

	virtual void Write(const void FAR* lpBuf, UINT nCount);
	// --- In  : lpBuf : A pointer to the user-supplied buffer that contains 
	//				the data to be written to the file.
	//			 nCount : The number of bytes to be transferred from the buffer.
	// --- Out : 
	// --- Returns : 
	// --- Effect : Writes the specified buffer to the file

	virtual void LockRange(DWORD dwPos, DWORD dwCount);
	virtual void UnlockRange(DWORD dwPos, DWORD dwCount);
	// These functions are not supported function in COXCompressedFile

	virtual void Abort();
	// --- In  :
	// --- Out : 
	// --- Returns : 
	// --- Effect : Closes the file associated with this object and makes the file 
	//				unavailable for reading or writing. If you have not closed the file 
	//				before destroying the object, the destructor closes it for you.
	//				This function will not throw an exception on failures because 
	//				failures are ignored by Abort.

	virtual void Flush();
	// --- In  :
	// --- Out : 
	// --- Returns : 
	// --- Effect : This is a not supported function in COXCompressedFile

	virtual void Close();
	// --- In  :
	// --- Out : 
	// --- Returns : 
	// --- Effect : Closes the file associated with this object and makes the file 
	//				unavailable for reading or writing. 

#ifdef _DEBUG
	virtual void Dump(CDumpContext&) const;
	virtual void AssertValid() const;
#endif //_DEBUG

	virtual ~COXCompressedFile();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object
	//				If you have not closed the file before destroying the object, 
	//				the destructor closes it for you. 

protected:      
	void GetFinalPos  ();
	void AutoFlush(BOOL bLast = FALSE);
    void InternalWrite(BOOL bLast = FALSE);
	int  InternalRead (WORD wCodedReadLength);
	UINT WriteHeader  (LPCTSTR lpBuffer, UINT &nCount);
	UINT ReadHeader   (char* lpBuffer, UINT nCount);

    DWORD MoveFromBufferBoundaries(DWORD dwStartBufferIndex, long lOffset);

private:
                   
// Message handlers ---------------------------------------------------------

};

#endif
// ==========================================================================
