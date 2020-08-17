// ==========================================================================
// 					Class Specification : COXUUDecoder
// ==========================================================================

// Header file : oxuudec.h

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
//	YES	Derived from CObject

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO 	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This class can be used to decode a UUencoded file
//	This class can be used by supplying the names of the file directly

// Remark:

// Prerequisites (necessary conditions):
//		***

/////////////////////////////////////////////////////////////////////////////

#ifndef __UUDECODE_H__
#define __UUDECODE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


/* bufsize can be 81 because uuencoded files should not be wider */
/* this includes the first begin line where the filename is specified */
#define BUFSIZE		81

class OX_CLASS_DECL COXUUDecoder : public CObject
{
DECLARE_DYNAMIC(COXUUDecoder)
// Data members -------------------------------------------------------------
public:

protected:
	char			m_bufa[BUFSIZE];
	char			m_bufb[BUFSIZE];
	char			m_bufc[BUFSIZE];
	char			m_bufd[BUFSIZE];
					
	char*			m_buf;
	char*			m_bufm1;
	char*			m_bufm2;
	char*			m_bufm3;
					
	int				m_goodbuf;
	int				m_goodbufm1;
	int				m_goodbufm2;
	int				m_goodbufm3;
					
	BOOL			m_bSkipIfExists;

private:
	
// Member functions ---------------------------------------------------------
public:
	COXUUDecoder();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object
	//				It will initialize the internal state

	BOOL IsUUEncodedFile(const char* EncodedFilNam);
	// --- In  : EncodedFilNam : path of file to test
	// --- Out : 
	// --- Returns : whether this file is a UUEncoded file or not
	// --- Effect : test to see vaild UUEncoded file

	HRESULT UUDecodeFile(const char* EncodedFilNam, const char* pszDir  = NULL ,
						 CStringArray* pTargetFiles = NULL);
	// --- In  : EncodedFilNam : Full path of the encoded file
	//			 pTargetFiles : Pointer to array of target file paths
	// --- Out : 
	// --- Returns :
	// --- Effect : Decodes the specified file and writes the targetfiles.  The default names
	//				of these target files are stored in the encoded file but the user can
	//				pass a Stringarray with new file paths for the targetfiles.  The paths in
	//				the stringarray must have the same order than the order of separate uuencodings
	//				within the Encoded file.  An encoding that does not find a new pathname
	//				within he stringarray will retain his default name as specified in the endoded 
	//				file.
	//				When a part of a uuencoded file, representing a full uudecoded file, is succesfully
	//				decoded its full path is stored in the StringArray if one was specified.  Like this
	//				you can query the stringarray to determine which files were decoded and what paths they
	//				have.

	void SetSkipIfExists(BOOL bSkip)
		{ m_bSkipIfExists = bSkip;}
	// --- In  : bSkip : TRUE to skip writing target files that already exist on disk
	// --- Out : 
	// --- Returns :
	// --- Effect : default is FALSE

	BOOL GetSkipIfExists()
		{ return m_bSkipIfExists;}
	// --- In  : 
	// --- Out : 
	// --- Returns : whether to skip writing target files that already exist on disk or not
	// --- Effect : default is FALSE

#ifdef _DEBUG
	virtual void Dump(CDumpContext&) const;
	virtual void AssertValid() const;
#endif //_DEBUG

	virtual ~COXUUDecoder();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object

protected:
	HRESULT UUDecodeFile(CStdioFile* pEncodedFil, const char* pszDir,
		CStringArray* pTargetFiles, CStringArray* pTempTargetFiles);

	void FixEOLN(char buf[], int n);
	BOOL GetLongLine(char* s, int n, CStdioFile* pFile);
	int IsGoodBuf(char buf[], int n);
	int UUD1Line(CFile* pFile, char buf[]);
	void rotatebufs();
	void clearmbufs();
	int ExtractUU(CStdioFile* pfrom, CFile* pTo);

private:
                   
};

#endif	// __UUDECODE_H__
// ==========================================================================
