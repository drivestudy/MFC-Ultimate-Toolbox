// ==========================================================================
// 					Class Specification : COXCheckBase, COXCheckSum8, 
//						COXCheckSum16, COXCheckSum32, COXCRC16, COXCRC32
// ==========================================================================

// Header file : OCCRCChk.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXCRCChk.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

COXCheckBase::COXCheckBase(ESize eSize /* = szNone */) :
	m_nCheck32(0xFFFFFFFF),
	m_eSize(eSize)
{
}

COXCheckBase::~COXCheckBase()
{
}

DWORD COXCheckBase::CalculateBlock( const LPVOID pData, DWORD nCount)
{
	return CalculateBlock(pData, nCount, m_nCheck32);
}

DWORD COXCheckBase::CalculateFile(CFile* pFile, DWORD nCount /*= 0xFFFFFFFF*/, DWORD nBufferLength /*= 2048*/)
{
	LPBYTE	pBuffer;
	DWORD	nLengthRead;
	DWORD	nOldCheck32 = m_nCheck32;

	if (nCount < nBufferLength)
		nBufferLength = nCount;

	pBuffer = new BYTE[nBufferLength];

	do
	{
		TRY
			{
			nLengthRead = pFile->Read(pBuffer, nBufferLength);
			}
		CATCH(CFileException, e)
			{
			m_nCheck32 = nOldCheck32;
			delete[] pBuffer;
			THROW_LAST();
			}
		END_CATCH
		m_nCheck32 = CalculateBlock(pBuffer, nLengthRead, m_nCheck32);
		nCount -= nLengthRead;
		if (nCount < nBufferLength)
			nBufferLength = nCount;
	}
	while ((nBufferLength!=0) && (nLengthRead == nBufferLength));	// So while not EOF
	
	delete[] pBuffer;

	return m_nCheck32;
}

DWORD COXCheckBase::CalculateFile(CString sFileName)
{
	CFile			file;
	CFileException	exception;
	DWORD			dwKey;

	if (file.Open(sFileName, CFile::modeRead | CFile::shareDenyWrite, &exception) == FALSE)
		AfxThrowFileException(exception.m_cause, exception.m_lOsError, sFileName);
	TRY
	{
		dwKey = CalculateFile(&file);
	}
	CATCH(CFileException, e)
	{
		file.Close();
		THROW_LAST();
	}
	END_CATCH
	file.Close();
	return dwKey;
}

COXCheckSum8::COXCheckSum8() :
	COXCheckBase(sz8bit)
{
}

DWORD COXCheckSum8::CalculateBlock( const LPVOID pData, DWORD nCount, DWORD nSeed)
{
	m_nCheck8 = (BYTE) nSeed;
	for( DWORD i=0 ; i<nCount ; i++ )
		m_nCheck8 = (BYTE) (m_nCheck8 + ((LPBYTE)pData)[i]);
	return (DWORD) m_nCheck8;
}

COXCheckSum16::COXCheckSum16() :
	COXCheckBase(sz16bit)
{
}

DWORD COXCheckSum16::CalculateBlock( const LPVOID pData, DWORD nCount, DWORD nSeed)
{
	m_nCheck16 = (WORD) nSeed;
	for( DWORD i=0 ; i<nCount ; i++ )
		m_nCheck16 = (WORD) (m_nCheck16 + ((LPBYTE)pData)[i]);
	return (DWORD) m_nCheck16;
}

COXCheckSum32::COXCheckSum32() :
	COXCheckBase(sz32bit)
{
}

DWORD COXCheckSum32::CalculateBlock( const LPVOID pData, DWORD nCount, DWORD nSeed)
{
	m_nCheck32 = nSeed;
	for( DWORD i=0 ; i<nCount ; i++ )
		m_nCheck32 += ((LPBYTE)pData)[i];
	return m_nCheck32;
}


/////////////////////////////////////////////////////////////////////////////
// Definition of static members
//

COXCRC16::CTableList COXCRC16::m_tableList;
COXCRC32::CTableList COXCRC32::m_tableList;


COXCRC16::CTableList::CTableList()
{
}

COXCRCTable<WORD>* COXCRC16::CTableList::CreateTable(WORD nPolynomial)
{
	COXCRCTable<WORD>* pTable = new COXCRCTable<WORD>(nPolynomial);
	int		i, j, k;
	WORD	nCRC;

	// Calculate the CRC value for index position n.
	for( i=0 ; i<256 ; i++ )
	{
		k = i << 8;
		nCRC = 0;
		for( j=0 ; j<8 ; j++ )
		{
			if ((nCRC^k) & 0x8000)
				nCRC = (WORD) ((nCRC << 1) ^ nPolynomial);
			else
				nCRC <<= 1;
			k <<= 1;
		}
		pTable->values[i] = nCRC;
	}
	return pTable;
}

COXCRC16::COXCRC16(WORD nPolynomial /* = CRC16_POLYNOMIAL*/) :
	COXCheckBase(sz16bit),
	m_pTable(NULL)
{
	m_pTable = m_tableList.GetTable(nPolynomial);
}

DWORD COXCRC16::CalculateBlock( const LPVOID pData, DWORD nCount, DWORD nSeed)
{
	LPBYTE	p = (LPBYTE) pData;

	m_nCheck16 = (WORD) nSeed;

	// Calculate the 16 bit CRC using the table lookup method.
	while( nCount-- )
		m_nCheck16 = (WORD) ((m_nCheck16 << 8) ^ m_pTable->values[(BYTE)((m_nCheck16 >> 8) ^ *p++)]);

	return (DWORD) m_nCheck16;
}


/////////////////////////////////////////////////////////////////////////////
// Definition of static members
//

COXCRC32::CTableList::CTableList()
{
}

COXCRCTable<DWORD>* COXCRC32::CTableList::CreateTable(DWORD nPolynomial)
{
	COXCRCTable<DWORD>* pTable = new COXCRCTable<DWORD>(nPolynomial);
	int		i, j;
	DWORD	nCRC;

	// Calculate the CRC value for index position n.
	for(i=0 ; i<256 ; i++)
	{
		nCRC = i;
		for (j=8 ; j>0 ; j--)
		{
			if( nCRC & 1 )
				nCRC = ( nCRC >> 1 ) ^ nPolynomial;
			else
				nCRC >>= 1;
		}
		pTable->values[i] = nCRC;
	}
	return pTable;
}


COXCRC32::COXCRC32(DWORD nPolynomial /*= CRC32_POLYNOMIAL*/) :
	COXCheckBase(sz32bit),
	m_pTable(NULL)
{
	m_pTable = m_tableList.GetTable(nPolynomial);
}

DWORD COXCRC32::CalculateBlock( const LPVOID pData, DWORD nCount, DWORD nSeed)
{
	DWORD	dwTemp1;
	DWORD	dwTemp2;
	LPBYTE	p = (LPBYTE) pData;

	m_nCheck32 = nSeed;

	// Calculate the 32 bit CRC using the table lookup method.
	while( nCount-- )
	{
		dwTemp1 = (m_nCheck32 >> 8) & 0x00FFFFFFL;
		dwTemp2 = m_pTable->values[(m_nCheck32 ^ *p++) & 0xff];
		m_nCheck32 = dwTemp1 ^ dwTemp2;
	}

	return m_nCheck32;
}

