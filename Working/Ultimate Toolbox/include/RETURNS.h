// ==========================================================================
// 							Return Codes specification
// ==========================================================================

// Header file : returns.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                          
// //////////////////////////////////////////////////////////////////////////
// Desciption :         
//		Contains some return codes used internally in dirspec 

// Remark:
//		***

// Prerequisites (necessary conditions):
//		***

/////////////////////////////////////////////////////////////////////////////
#ifndef __RETURNS_H__
#define __RETURNS_H__

#ifndef WIN32
#define DRIVE_CDROM 5
#define DRIVE_RAMDISK 6
#endif

// Statuses
#define SUCCESS 0
#define F_OPENFILE 1
#define F_MEMORY 2
#define F_NOINDEXES 3
#define F_SEEK 4
#define F_READ 5
#define F_NOTFOUND 6                                     
#define F_CREATEPROCESS 7
#define F_GETCURRENTDIR 8
#define F_SETCURRENTDIR 9
#define F_CREATEDIR 10
#define F_COPYFILE 11
#define F_NOCD 12
#define F_NOVOL 13
#define F_CANCEL 14
#define F_INSTALLPROG 15
#define F_INVALIDDIRLEN 16
#define F_NODRIVE 17
#define F_RESERVED 18
#define F_MIPSONLY 19
#define F_INTELONLY 20
#define F_ALPHAONLY 21
#define F_32ONLY 22
#define F_DELETEFILE 23
#define F_DELETEDIR 24
#define F_NOWAIT 25
#define F_WRITE 26
#define F_WIN95ONLY 27

// platforms                 
#define DOSEXE 1
#define WINEXE 16
#define NTIEXE 32
#define NTMEXE 33
#define NTAEXE 34
#define NTINTELEXE 35
#define WIN95EXE 40

// directory defines
#define MAXDIRLEN 256
#define NUMDRIVES 26
#define FIXED 0
#define REMOVEABLE 1
#define REMOTE 2
#define CDROM 3

#endif // __RETURNS_H__
// ==========================================================================
