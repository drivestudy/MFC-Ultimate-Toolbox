// ==========================================================================
// 					Class Specification : COXCheckBase, COXCheckSum8, 
//						COXCheckSum16, COXCheckSum32, COXCRC16, COXCRC32
// ==========================================================================

// Header file : OXCRCChk.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	YES	Abstract class (does not have any objects) --> COXCheckBase
//	NO	Abstract class --> The other
//	YES	Derived from CObject

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO 	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption: COXCheckBase
//         
//	Class COXCheckBase is the abstract base class used for deriving
//	the classes which carry out integrity checking on data
//	buffers.
//	The pure virtual function 'CalculateBlock' needs to be implemented
//	by the derived classes.

// Desciption: COXCheckSum8, 16 and 32
//         
//	Class COXChecksum8, 16 and 32 implements the simplest method of
//	integrity check. It involves a simple summation of each byte of data,
//	wrapping when the capacity of the checksum variable is exceeded.

// Desciption: COXCRC16 and 32
//         
//	The additive checksum calculation provides a simple test of integrity for
//	transmitted data blocks. It is possible for a block of data to be corrupted
//	so that the checksum is not altered. For example, two complementary errors
//	in a block would be undetected. Cyclic redundancy check or CRC calculation
//	provides a more secure form of error checking.
//	A CRC is similar to a normal checksum in that operations are performed on
//	successive bytes of data. The CRC value is derived using division and shifting.


// Remark:
//	All the from COXCheckBase derived implementation classes have an identical 
//	interface with this base class.
//	There are two main functions: CalculateBlock() and CalculateFile()
//	CalculateBlock() is used for the checksum (or CRC) calculation of a memory
//	block.
//	CalculateFile() has two version. The first one receives a CFile object as 
//	parameter and the second receives a string with the file name of the file.


// Prerequisites (necessary conditions):
//		***

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXCRCCHK_H__
#define __OXCRCCHK_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


#define OX_CRC16_POLYNOMIAL		0x1021			// default 16 bit polynomial
#define OX_CRC32_POLYNOMIAL     0xEDB88320L		// default 32 bit polynomial

////////////////////////////////////////////////////////////////////////////
// The base class for all the derived integrity check implementation classes
//

class OX_CLASS_DECL COXCheckBase : public CObject
{
// Data members -------------------------------------------------------------
public:
	union
	{
		BYTE	m_nCheck8;		// the 8 bit check result of the calculation
		WORD	m_nCheck16;		// the 16 bit check result
		DWORD	m_nCheck32;		// the 32 bit check result
	};

    enum ESize
	{
		szNone,
		sz8bit,
		sz16bit,
		sz32bit
	};
	
	const ESize	m_eSize;		// the size of the check result (8, 16 or 32 bit)

protected:

private:

// Member functions ---------------------------------------------------------
public:
    COXCheckBase(ESize eSize = szNone);
	// --- In  : eSize: the size of the check key that will be used
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object
	//				It will initialize the internal state

    virtual ~COXCheckBase();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object

    virtual DWORD CalculateBlock( const LPVOID pData,
								  DWORD nCount);
	// --- In  : pData: The buffer to do the checksum calculation on.
	//			 nCount: The number of bytes in the buffer.
	// --- Out : 
	// --- Returns : the check result
	// --- Effect : Calls the overloaded seed version of this function.
	//				The last check result is passed as seed parameter.
	//				In this way you can combine different data blocks
	//				in one Check Result.

    virtual DWORD CalculateBlock( const LPVOID pData,
								  DWORD nCount,
								  DWORD nSeed) = 0;
	// --- In  : pData : The buffer to do the checksum calculation on.
	//			 nCount : The number of bytes in the buffer.
	//			 seed : A seed value is used as a begin value for the Check
	//					calculation
	//					The overloaded version uses the previous check result
	//					as begin value.
	// --- Out : none
	// --- Returns : the check result
	// --- Effect : used to calculate a Check Result for a supplied buffer. This
	//				function is pure virtual so it has to be redefined in derived classes.

    virtual DWORD CalculateFile(CFile* pFile, DWORD nCount = 0xFFFFFFFF, DWORD nBufferLength = 2048);
    // --- In  : pFile : The CFile pointer of the file which check is to be calculated
	//			 nCount : the number of bytes
	//			 nBufferLength: The size of the read buffer:
	//				 a large read buffer increases the performance
	// --- Out : none
	// --- Returns : the check result
	// --- Effect : The CFile must already be opened. The CalculateBlock function is called.
	//				The previous check result is used as seed for the new calculation.
	//				Use the Reset() function to use calculate with the initial seed value.
	// --- EXCEPTIONS: Can throw CFileExceptions

    virtual DWORD CalculateFile(CString sFileName);
    // --- In  : sFileName : The Name of the file which crc is to be calculated
	// --- Out : 
	// --- Returns : the check result
	// --- Effect : This function calls the calculateBlock function with the data of the file
	// --- EXCEPTIONS: Can throw CFileExceptions

	void Reset() {m_nCheck32 = 0xFFFFFFFF;}
    // --- In  : none
	// --- Out : none
	// --- Returns : none
	// --- Effect : Sets the check result back to the initial value

protected:
private:
};


////////////////////////////////////////////////////////////////////////////
// The Checksum classes: COXCheckSum8, COXCheckSum16 and COXCheckSum32
//

class OX_CLASS_DECL COXCheckSum8 : public COXCheckBase
{
// Data members -------------------------------------------------------------
public:
protected:
private:

// Member functions ---------------------------------------------------------
public:
    COXCheckSum8();
	// --- In: none
	// --- Out: none
	// --- Returns: none
	// --- Effect: constructs the object and initialize the internal state

    virtual DWORD CalculateBlock( const LPVOID pData, DWORD nCount, DWORD nSeed);
	// --- In  : pData : The buffer to do the checksum calculation on.
	//			 nCount : The number of bytes in the buffer.
	//			 seed : A seed value is used as a begin value for the CRC
	//					calculation
	// --- Out : none
	// --- Returns : the 8 bit checksum result of the data block
	// --- Effect : This Checksum implemation is the simplest integrity check method.
	//				It involves a simple summation of each byte of data.

protected:
private:
};

///////////////////////////////////////////////////////////////////////////
// an 16 bit version of the previous class
class OX_CLASS_DECL COXCheckSum16 : public COXCheckBase
{
public:
    COXCheckSum16();
    virtual DWORD CalculateBlock( const LPVOID pData, DWORD nCount, DWORD nSeed);
};

///////////////////////////////////////////////////////////////////////////
// an 32 bit version of the previous class
class OX_CLASS_DECL COXCheckSum32 : public COXCheckBase
{
public:
    COXCheckSum32();
    virtual DWORD CalculateBlock( const LPVOID pData, DWORD nCount, DWORD nSeed);
};


////////////////////////////////////////////////////////////////////////////
// The CRC classes: COXCRC16 and COXCRC32
//

// Helper classes for the CRC classes
// These helper classes are implemented to keep track of the CRC tables.
// A CRC table is calculated just one time for each used polynomial

	template<class VALUE>
	struct COXCRCTable
	{
		VALUE			values[256];
		VALUE			m_nPolynomial;
		COXCRCTable*	m_pNextTable;

		COXCRCTable(VALUE nPolynomial);
	};

	template<class VALUE>
	class COXCRCTableList
	{
	public:
		COXCRCTable<VALUE>*	m_pTableList;
	public:
		COXCRCTableList();
		virtual ~COXCRCTableList();
		virtual COXCRCTable<VALUE>* CreateTable(VALUE nPolynomial) = 0;
		COXCRCTable<VALUE>* GetTable(VALUE nPolynomial);
	};


class OX_CLASS_DECL COXCRC16 : public COXCheckBase
{
// Data members -------------------------------------------------------------
public:
    class OX_CLASS_DECL CTableList : public COXCRCTableList<WORD>
	{
	public:
		CTableList();
		virtual COXCRCTable<WORD>* CreateTable(WORD nPolynomial);
	};
	
	static CTableList	m_tableList;
	COXCRCTable<WORD>*	m_pTable;
protected:

private:

// Member functions ---------------------------------------------------------
public:
    COXCRC16(WORD nPolynomial = OX_CRC16_POLYNOMIAL);
	// --- In: none
	// --- Out: none
	// --- Returns: none
	// --- Effect: constructs the object and initialize the internal state

    virtual DWORD CalculateBlock( const LPVOID pData, DWORD nCount, DWORD nSeed);
	// --- In  : pData : The buffer to do the checksum calculation on.
	//			 nCount : The number of bytes in the buffer.
	//			 seed : A seed value is used as a begin value for the CRC
	//					calculation
	// --- Out : none
	// --- Returns : the 16 bit CRC result of the data block
	// --- Effect : This CRC implementation uses Cyclic redun
protected:

private:
};


// an 32 bit version of the previous class
class OX_CLASS_DECL COXCRC32 : public COXCheckBase
{
// Data members -------------------------------------------------------------
public:
protected:
    class OX_CLASS_DECL CTableList : public COXCRCTableList<DWORD>
	{
	public:
		CTableList();
		virtual COXCRCTable<DWORD>* CreateTable(DWORD nPolynomial);
	};

	static CTableList	m_tableList;
	COXCRCTable<DWORD>*	m_pTable;

private:

// Member functions ---------------------------------------------------------
public:
    COXCRC32(DWORD nPolynomial = OX_CRC32_POLYNOMIAL);
    virtual DWORD CalculateBlock( const LPVOID pData, DWORD nCount, DWORD nSeed);
protected:
private:
};


#include "OXCRCChk.inl"


#endif //__OXCRCCHK_H__
// ==========================================================================

