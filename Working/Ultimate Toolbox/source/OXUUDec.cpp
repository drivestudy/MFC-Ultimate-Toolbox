// ==========================================================================
// 					Class Specification : COXUUDecoder
// ==========================================================================

// Header file : oxuudec.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      

// ///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "oxuudec.h"	// file header
#include "afxpriv.h"	// for CMirrorFile

#include <io.h>
#include <direct.h>
#include <ctype.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXUUDecoder, CObject)

#define new DEBUG_NEW

#include "OXUUDRet.h"

/////////////////////////////////////////////////////////////////////////////
// Definition of static members

// Data members -------------------------------------------------------------
// protected:

// private:

// Member functions ---------------------------------------------------------
// public:

COXUUDecoder::COXUUDecoder()
: m_buf(m_bufa),
m_bufm1(m_bufb),
m_bufm2(m_bufc),
m_bufm3(m_bufd),
m_goodbuf(-1),
m_goodbufm1(-1),
m_goodbufm2(-1),
m_goodbufm3(-1),
m_bSkipIfExists(FALSE)
{
}

BOOL COXUUDecoder::IsUUEncodedFile(const char* EncodedFilNam)
{
	CStdioFile EncodedFile;

	if (!EncodedFile.Open((LPCTSTR)EncodedFilNam, CFile::modeRead | CFile::typeBinary | CFile::shareDenyWrite))
		return FALSE; 

	m_buf[0] = 0;		/* tells extractuu to search for begin line */
	int res = ExtractUU(&EncodedFile, NULL);

	m_buf[0] = 0;		// Reset Buffer
	EncodedFile.Close();

	return (res == 0);
}

HRESULT COXUUDecoder::UUDecodeFile(const char* EncodedFilNam, const char* pszDir /* = NULL */,
								   CStringArray* pTargetFiles /* = NULL */)
{
	HRESULT hr;
	CStdioFile	EncodedFile;
	CStringArray TempTargetFiles;
	CStringArray* pTrgtFiles = pTargetFiles;
	if (pTargetFiles == NULL)
		pTrgtFiles = new CStringArray;

	if (!EncodedFile.Open((LPCTSTR)EncodedFilNam, CFile::modeRead | CFile::typeBinary | CFile::shareDenyWrite))
		return (HRESULT)MSG_UUDEC_OPEN_ENCODED_ERROR; 

	hr = UUDecodeFile(&EncodedFile, pszDir, pTrgtFiles, &TempTargetFiles);

	EncodedFile.Close();

	// Examine the TempTargetFiles array to see whether there are some temporary files created
	// that have to be renamed
	for (int i = 0; i < TempTargetFiles.GetSize(); i++)
	{
		if (!TempTargetFiles.GetAt(i).IsEmpty() && pTrgtFiles != NULL)
		{
			CFile::Remove(pTrgtFiles->GetAt(i));
			CFile::Rename(TempTargetFiles.GetAt(i), pTrgtFiles->GetAt(i));
		}
	}
	delete pTrgtFiles;

	return hr;
}

HRESULT COXUUDecoder::UUDecodeFile(CStdioFile* pEncodFil, const char* pszDir,
								   CStringArray* pTargetFiles, CStringArray* pTempTargetFiles)
{
	ASSERT_VALID(pEncodFil);

	HRESULT hr=NULL;

#ifdef WIN32
	CMirrorFile TargetFile;
#else
	CFile TargetFile;
#endif

	CString sTargetName, sTempTargetName;
	char targetfile[256];   /* 255 chars max length of a pathname */

	int res,success,errors; /* gp result, success and error counters */
	int amode;              /* "begin xxx" where xxx is amode, for chmod() */
	int nTargetIndex(-1);	// index of the encoding part in a multiple encoded file

	errors = 0;
	success = 0;
	res = 0;
	amode = -1;
	targetfile[0] = 0;

	while (res < 3)
	{  /* 0 success, 1 error, 2 begin before end, 3 eof seen */
		if (res < 2)
		{     /* skip this if buf[] already contains begin line */
			m_buf[0] = 0;    /* tells extractuu to search for begin line */
			res = ExtractUU(pEncodFil, NULL);
			nTargetIndex++;
		}

		/*
		* here if we have res==2 then we must be recovering from a
		* begin before end error on the previous pass, recycling buf[],
		* the extractuu() above couldn't have generated a res==2 because
		* it's looking for a begin so it must have come from the one below
		*/
		if (res == 0 || res == 2)
		{
#if _MSC_VER >= 1400
			if (sscanf_s(m_buf, "begin %d %255s", &amode, targetfile) != 2)
#else
			if (sscanf(m_buf, "begin %d %255s", &amode, targetfile) != 2)
#endif
			{
				res = 1;
				goto badfilename;
			}

			// Renaming target when Stringarray has new target name
			sTargetName.Empty();
			if (pTargetFiles != NULL)
				if (pTargetFiles->GetSize() > nTargetIndex)
					sTargetName = pTargetFiles->GetAt(nTargetIndex);

			if (sTargetName.IsEmpty())
				if (pszDir != NULL)
					sTargetName = CString(pszDir) + CString("\\") + CString(targetfile);
				else
					sTargetName = targetfile;
			else
				TRACE1("renaming target to \"%s\"\n", sTargetName);

			/* check for file existance */
			sTempTargetName = sTargetName;
#if _MSC_VER >= 1400
			if (_access((LPCSTR)(LPCTSTR)sTargetName, 0) == 0)
#else
			if (access((LPCSTR)(LPCTSTR)sTargetName, 0) == 0)
#endif
			{        /* it exists */
				if (m_bSkipIfExists)
				{  /* quietly skip without counting as error */
					TRACE1("Skipped, file exists : %s\n", sTargetName);
					res = 0;
					if (pTargetFiles != NULL)
						pTargetFiles->SetAtGrow(nTargetIndex, sTargetName);

					goto badfilename1;
				}

				/* check if file has write access */  
				CFileStatus status;
				pEncodFil->GetStatus(status);

				if (sTargetName != status.m_szFullName && 					
#if _MSC_VER >= 1400
					_access((LPCSTR)(LPCTSTR)sTargetName, 2) != 0)
#else
					access((LPCSTR)(LPCTSTR)sTargetName, 2) != 0)
#endif
				{    /* write not allowed */
					TRACE1("\"%s\" ", sTargetName);
					TRACE(_T("Skipped, file exists write protected.\n"));
					res = 1;
					hr = (HRESULT)MSG_UUDEC_WRITING_OF_A_FILE_FAILED;
					goto badfilename;
				}

				// get the directory for a temporary file
				sTempTargetName.Empty();
#ifdef WIN32
				TCHAR szPath[_MAX_PATH];
				GetTempPath(_MAX_PATH, szPath);

				//let's create a temporary file name
				GetTempFileName(szPath, _T("OXTEMP"), 0,
					sTempTargetName.GetBuffer(_MAX_PATH+1));
#else
				//let's create a temporary file name
				::GetTempFileName(0, _T("OXTEMP"), 0,
					sTempTargetName.GetBuffer(_MAX_PATH+1));
#endif
				sTempTargetName.ReleaseBuffer();
				pTempTargetFiles->SetAtGrow(nTargetIndex, sTempTargetName);

				/* we determined that the file is ok to overwrite */
				TRACE1("updating \"%s\"\n",targetfile);
			}

			/* Create the new decoded file */
			if (pszDir != NULL)
#if _MSC_VER >= 1400
				_mkdir(pszDir);
#else
				mkdir(pszDir);
#endif
			if (!TargetFile.Open(sTempTargetName, CFile:: modeCreate | CFile::modeWrite | CFile::typeBinary))
			{
				TRACE2("%s\"%s\"\n", "fatal: can't open target file ", sTempTargetName);
				res = 3;
				hr = (HRESULT)MSG_UUDEC_WRITING_OF_A_FILE_FAILED;
				goto badfilename;
			}

			/* m_buf[] contains begin line, this tells extractuu to decode */
			/* rather than search for the next begin line */
			res = ExtractUU(pEncodFil, &TargetFile);

			TargetFile.Close();

			if (res > 0)
			{
				TRACE1("trying to delete \"%s\"\n", sTargetName);
#if _MSC_VER >= 1400
				if (_unlink((LPCSTR)(LPCTSTR)sTargetName) != 0)
#else
				if (unlink((LPCSTR)(LPCTSTR)sTargetName) != 0)
#endif
					TRACE1("error deleting \"%s\"\n",sTargetName);
			}
			else
			{
				if (pTargetFiles != NULL)
					pTargetFiles->SetAtGrow(nTargetIndex, sTargetName);
			}
		}

badfilename:
		if (res == 0)
			success++;

badfilename1:
		if (res == 1 || res == 2)
			errors++;
	}                                    /* <-- end of while (res<3) { ... */

	if (errors != 0)
		return (hr);

	return (HRESULT)MSG_UUDEC_SUCCESS;
}

#ifdef _DEBUG
void COXUUDecoder::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}

void COXUUDecoder::AssertValid() const
{
	CObject::AssertValid();
}
#endif

COXUUDecoder::~COXUUDecoder()
{
}

// protected:
void COXUUDecoder::FixEOLN(char buf[], int n)
// --- In  : buf[] : buffer to be examined
//			 n : length of buffer
// --- Out : 
// --- Returns :
// --- Effect : This subroutine strips all <cr> and <lf> from the end of buf[]
//				This is done because a lot of Internet-News newbies don't know how to
//				uuencode their files properly.
//				Standard unix uudecode programs usually puke if there is a ^M in the line.
{
	size_t len,nn;
	int done;

	if (n < 1)
		return;

	nn = (size_t)n - 1;
	len = strlen(buf);
	if (nn < len)
		len = n;

	done=0;
	while (len>0 && !done)
	{
		switch (buf[len-1])
		{
		case '\r':
			len--;

			break;
		case '\n':
			len--;

			break;
		case ' ':
			if (len > 61)
				len--;
			else
				done = 1;

			break;
		default:
			done = 1;
		}
	}

	buf[len] = '\0';
	return;
}


BOOL COXUUDecoder::GetLongLine(char* s, int n, CStdioFile* pFile)
// --- In  : s : buffer to store read line
//			 n : max length of line to read
//			 pFile : the encoded file
// --- Out : 
// --- Returns :
// --- Effect : This subroutine is just like the standard fgets() except smarter.
//				When lines in the stream are longer than the buffer size, the line gets
//				truncated and the stream is purged until the actual <lf> is seen.
//				This works just like the Pascal readln(f,stringvar)
{
	char buf[32];
	char* result;
	size_t nn;

	TRY
	{
		result = (LPSTR)pFile->ReadString((LPTSTR)s, n);

		if (n < 2 || result == NULL)
			return FALSE;

		nn = (size_t)n - 1;
		if (strlen(s) == nn && s[n-2] != '\n')
		{
			while (pFile->ReadString((LPTSTR)buf, sizeof(buf)))
				if (strlen(buf) + 1 < sizeof(buf) || buf[sizeof(buf) - 2] == '\n')
					break;
		}

		FixEOLN(s, n);
	}
	CATCH(CFileException, e)
	{
		return FALSE;
	}
	END_CATCH

		return TRUE;
}

int COXUUDecoder::IsGoodBuf(char buf[], int n)
// --- In  : buf[] : buffer to examine
//			 n : length of buffer
// --- Out : 
// --- Returns :
// --- Effect : This subroutine checks buf to see if it looks like
//				a standard uuencode line and returns a number for best guess.
//				
//				returned values are one of the following...
//				  0) unknown, possibly text or short last line
//				  1) "begin 644 filename" type of line
//				  2) "M..." etc., main body line, short last line is not detected as 2
//				  3) "`" or " ", this is usually the 2nd from last line in a uuencode
//				  4) "end", usually last line of a uuencode
//				
//				n is the size of buf[]

{
	size_t len,nn;

	len = strlen(buf);
	nn = (size_t)n - 1;
	if (nn < len)
		len = nn;

	if (len == 61 && buf[0] == 'M')
		return(2);

	if (len > 10 && strncmp(buf, "begin ", 6) == 0 && isdigit(buf[6]) &&
		isdigit(buf[7]) && isdigit(buf[8]))
		return(1);

	if (len >= 1 && (buf[0] == ' ' || buf[0] == '`'))
		return(3);

	if (len >= 3 && strncmp(buf, "end", 3) == 0)
		return(4);

	return(0);
}

int COXUUDecoder::UUD1Line(CFile* pFile, char buf[])
// --- In  : pFile : file to write to
//			 buf : buffer contaning encode chars
// --- Out : 
// --- Returns :
// --- Effect : uudecode 1 line, assumes buf[] is a valid uuencoded line
//				if test=1 then range check every character decoded, a little slower!
//				return 0 if no error
//				return 1 if general error
//				return 2 if illegal character (only when test=1)
{
	int len,b,i,j;
	size_t l;
	unsigned char c0, c1, x, *from, *to;
	static unsigned char binbuf[45];
	static const unsigned char c[3][4] = {   /* used by slow decoder below */
		{2,    4,    6   , 0 },
		{4,    2,    0   , 0 },
		{0x03, 0x0f, 0x3f, 0 }
	};

	len = (int)buf[0] - 0x20; /* first char on line is # 8 bit bytes to decode */
	l = strlen((const char*)buf);
	b = 0;
	/* this is the FAST! decoder for the main body lines */
	TRY
	{
		if (len == 45 && l >= 61)
		{
			from = (unsigned char*)&buf[1];
			to = binbuf;
			while ( b < len)
			{
				if (*from < ' ' || *from > '`')
					return(2);
				*(to++) = (unsigned char)(((*from - 0x20) << 2) | (((*(from + 1) - 0x20) >> 4) & 0x03));
				from++;
				if (*from < ' ' || *from > '`')
					return(2);
				*(to++) = (unsigned char)(((*from - 0x20) << 4) | (((*(from + 1) - 0x20) >> 2) & 0x0f));
				from++;
				if (*from < ' ' || *from > '`')
					return(2);
				*(to++) = (unsigned char)(((*from - 0x20) << 6) | (( *(from + 1) - 0x20      ) & 0x3f));
				from++;
				if (*from < ' ' || *from > '`')
					return(2);
				from++;
				b = b + 3;
			}

			pFile->Write(binbuf, len);

			return(0);
		}

		if (len < 1 || len > 45)
		{
			return(2);
		}
		while (b < len)
		{    /* this is the slow decoder for the last short line */
			i = b % 3;
			j = b * 4 / 3 + 1;
			if ((size_t)j < l)
				c0 = buf[j];
			else
				c0 = ' ';

			if ((size_t)j + 1 < l)
				c1 = buf[j + 1];
			else
				c1 = ' ';

			if (c0 < ' ' || c0 > '`' || c1 < ' ' || c1 > '`')
				return(2);

			x = (unsigned char)(((c0 - 0x20) << c[0][i]) + (((c1 - 0x20) >> c[1][i]) & c[2][i]));

			pFile->Write(&x, 1);

			b++;
		}
	}
	CATCH(CFileException, e)
	{
		return(1);
	}
	END_CATCH

		return(0);
}

void COXUUDecoder::rotatebufs()
{
	char*	temp;

	temp = m_bufm3;
	m_bufm3 = m_bufm2;
	m_bufm2 = m_bufm1;
	m_bufm1 = m_buf;
	m_buf = temp;

	m_buf[0] = 0;

	m_goodbufm3 = m_goodbufm2;
	m_goodbufm2 = m_goodbufm1;
	m_goodbufm1 = m_goodbuf;
	m_goodbuf = -1;
}	

void COXUUDecoder::clearmbufs()
{
	m_bufm3[0] = 0;
	m_bufm2[0] = 0;
	m_bufm1[0] = 0;
	m_goodbufm3 = -1;
	m_goodbufm2 = -1;
	m_goodbufm1 = -1;
}

int COXUUDecoder::ExtractUU(CStdioFile* pfrom,   /* pointer to input file*/
							CFile* pTo)		/* pointer to output file*/
							// --- In  : pfrom : file to read from
							//			 pTo : file to write to
							// --- Out : 
							// --- Returns :
							// --- Effect : This is the main extract subroutine.
							//				buf[] is used in the event that a new "begin" is encountered
							//				before the ending of the previous uuencoding.  This is an error
							//				condition and to keep from messing up the next uudecode which is
							//				probably ok, this begin line is stuffed into the buf[] for
							//				the next pass through extractuu().
							//				returned values...
							//				 0=success
							//				 1=error
							//				 2=begin line encountered before end line decoded
							//				 3=eof seen
{
	int amode,res;
	char fn[BUFSIZE];

	if (strlen(m_buf) == 0)
	{
retry:
		m_goodbuf = 0;
		while (GetLongLine(m_buf, BUFSIZE, pfrom))
			if ((m_goodbuf = IsGoodBuf(m_buf, BUFSIZE)) == 1)
				break;

		if (m_goodbuf != 1)
			return(3);      /* indicate eof seen */

#if _MSC_VER >= 1400
		if (sscanf_s(m_buf, "begin %d %80s", &amode, fn) != 2)
#else
		if (sscanf(m_buf, "begin %d %80s", &amode, fn) != 2)
#endif
			goto retry;

		return(0);
	}

	/* here when begin line was found and is in buf[], now do decoding */
	clearmbufs();                /* clear back history buffers */
	m_goodbuf = 1;                   /* assume begin line is in buf[] */
	if (IsGoodBuf(m_buf, BUFSIZE) != m_goodbuf)
	{
		return(1);
	}

	rotatebufs();
	while (GetLongLine(m_buf, BUFSIZE, pfrom))
	{
		m_goodbuf = IsGoodBuf(m_buf, BUFSIZE);
		if (m_goodbufm1 == 2 && (m_goodbuf == 2 || m_goodbufm2 == 2 || m_goodbufm2 == 1))
			if ((res = UUD1Line(pTo, m_bufm1)) != 0)
				return(1);

		if (m_goodbuf == 1)
			return(2);

		/*  end detected...logic for what to do with 3rd from last line bufm2
		*  (for relax=0 or 1)
		*
		*  ignore this end | decode line bufm2 | just exit, already decoded
		*     (m2) (m3)    |    (m2) (m3)      |    (m2) (m3)   <-- goodbuf
		*  ----------------+-------------------+---------------------------
		*      0    0      |     0    1        |     1    x
		*      0    3      |     0    2        |     2    1
		*      0    4      |     2    0        |     2    2
		*      2    3      |                   |
		*      2    4      |                   |
		*      3    x      |                   |
		*      4    x      |                   |
		*/
		if (m_goodbuf == 4 && m_goodbufm1 == 3)
		{     /* end sequence seen */
			if (m_goodbufm2 == 1)
				return(0);
			if ((m_goodbufm2 == 0 && (m_goodbufm3 == 0 || m_goodbufm3 >= 3)) ||
				(m_goodbufm2 >= 3) || (m_goodbufm2 == 2 && m_goodbufm3 >= 3))
				goto nextline;

			if ((m_goodbufm2 == 0 && (m_goodbufm3 == 1 || m_goodbufm3 == 2)) ||
				(m_goodbufm2 == 2 && m_goodbufm3 == 0))
			{
				if ((res = UUD1Line(pTo, m_bufm2)) != 0)
					return(1);
			}

			return(0);
		}

		/*  end detected...logic for what to do with 2nd from last line bufm1
		*  (for relax=1)
		*
		*  ignore this end | decode line bufm2 | just exit, already decoded
		*     (m1) (m2)    |    (m1) (m2)      |    (m1) (m2)   <-- goodbuf
		*  ----------------+-------------------+---------------------------
		*      0    0      |     0    1        |     1    x
		*      0    3      |     0    2        |     2    1
		*      0    4      |     2    0        |     2    2
		*      2    3      |                   |
		*      2    4      |                   |
		*      3    x      |                   |
		*      4    x      |                   |
		*/
		if (m_goodbuf == 4)
		{     /* end sequence seen */
			if (m_goodbufm1 == 1)
				return(0);

			if ((m_goodbufm1 == 0 && (m_goodbufm2 == 0 || m_goodbufm2 >= 3)) ||
				(m_goodbufm1 >= 3) || (m_goodbufm1 == 2 && m_goodbufm2 >= 3))
				goto nextline;

			if ((m_goodbufm1 == 0 && (m_goodbufm2 == 1 || m_goodbufm2 == 2)) ||
				(m_goodbufm1 == 2 && m_goodbufm2 == 0))
			{
				if ((res = UUD1Line(pTo, m_bufm1)) != 0)
					return(1);
			}
			return(0);
		}

nextline:
		rotatebufs();
	}

	if (m_goodbufm1 == 2 && (m_goodbufm2 == 1 || m_goodbufm2 == 2))
		res = UUD1Line(pTo, m_bufm1);    

	return(3);                 /* indicate end of input file */
}

// private:
// ==========================================================================
