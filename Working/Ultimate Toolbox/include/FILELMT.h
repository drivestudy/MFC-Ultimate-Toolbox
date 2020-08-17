// ==========================================================================
// 						File System Limit Specification 
// ==========================================================================

// Header file : filelmt.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//		This file defines several file system limits


/////////////////////////////////////////////////////////////////////////////
#ifndef __FILELMT_H__
#define __FILELMT_H__             

// ... The maximum number of characters that a full file name
//     (base name and full stop and extender) may use,
//     EXCLUDING  the zero-terminator
//     The is also the maximum length for a directory name
//		FAT : 12, VFAT : 254, HPFS : 254, NTFS : 255 
#ifdef WIN32
#define MAX_FILE_NAME_LENGTH	255
#else
// ... WIN16 is always FAT
#define MAX_FILE_NAME_LENGTH	12
#endif

// ... The maximum number of characters that a full path specification
//     may use, EXCLUDING the zero-terminator
//		FAT : 66, VFAT : 259, HPFS : 254, NTFS : 255 
#ifdef WIN32
#define MAX_PATH_NAME_LENGTH	259
#else
// ... WIN16 is always FAT
#define MAX_PATH_NAME_LENGTH	66
#endif

// .... Characters that are always invalid for directory or file names
//		* characters in the (decimal) range 0 through 31
//		* less-than (<), geater-than (>), double quotation mark (")
//        pipe (|)
//		* Drive separator : colon(:)
//		* Directory separators : slash (/) and back slash (\)
//      * Wild characters : question mark (?) and asterisk (*)
//      Different file systems add extra limitations
//		FAT : left-square-bracket ([), right-square-bracket (]),
//		      semi-colon (;), equals-to (=), comma (,) and plus (+)
//			  Full stop (.) may only be used for extender speration
//		VFAT :
//		HPFS :
//		NTFS : 
//	The infix _W_ means that wild characters are allowed and thus not invalid chars
//  The NULL-character cannot be checked using string functions and is therefore
//  removed from the invalid chars
#ifdef WIN32
#define INVALID_W_PATH_CHARS	_T("\x01\x02\x03\x04\x05\x06\x07\x08\x09")		\
								_T("\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13")	\
								_T("\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D")	\
								_T("\x1E\x1F")									\
								_T("<>\"|")
#else
// ... WIN16 has much less valid chars
#define INVALID_W_PATH_CHARS	"\x01\x02\x03\x04\x05\x06\x07\x08\x09"		\
								"\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13"	\
								"\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D"	\
								"\x1E\x1F"									\
								"<>\"|"										\
								" +,;=[]`"									\
								"\x7F"										\
								"\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x80" \
								"\x8A\x8B\x8C\x8D\x8E\x8F"					   \
								"\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x90" \
								"\x9A\x9B\x9C\x9D\x9E\x9F"					   \
								"\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xA0" \
								"\xAA\xAB\xAC\xAD\xAE\xAF"					   \
								"\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xB0" \
								"\xBA\xBB\xBC\xBD\xBE\xBF"					   \
								"\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xC0" \
								"\xCA\xCB\xCC\xCD\xCE\xCF"					   \
								"\xD0\xD1\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9\xD0" \
								"\xDA\xDB\xDC\xDD\xDE\xDF"					   \
								"\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xE0" \
								"\xEA\xEB\xEC\xED\xEE\xEF"					   \
								"\xF0\xF1\xF2\xF3\xF4\xF5\xF6\xF7\xF8\xF9\xF0" \
								"\xFA\xFB\xFC\xFD\xFE\xFF"					  

#endif

#define INVALID_W_DIR_CHARS		INVALID_W_PATH_CHARS _T(":")
#define INVALID_W_FILE_CHARS	INVALID_W_DIR_CHARS	 _T("/\\")

#define INVALID_PATH_CHARS		INVALID_W_PATH_CHARS _T("?*")
#define INVALID_DIR_CHARS		INVALID_W_DIR_CHARS	 _T("?*")
#define INVALID_FILE_CHARS		INVALID_W_FILE_CHARS _T("?*")

#endif // __FILELMT_H__             
