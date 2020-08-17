// ==========================================================================
// 						Class Implementation : COXBitBuffer
// ==========================================================================

// Source file :bitbuff.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "oxbitbuf.h"	// class specification

#include "UTBStrOp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXBitBuffer, CObject)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members


// Data members -------------------------------------------------------------
// protected:
// LPBYTE m_pBuffer;
// --- The buffer ow which bits are written or read

// private:

// Member functions ---------------------------------------------------------
// public:

COXBitBuffer::COXBitBuffer(LPBYTE pBuffer)
{
	m_pBuffer = pBuffer;
	m_ucMask = 0x80;
	m_cByte = 0; 		// index gelezen byte
	m_nRack = 0;
}

COXBitBuffer::COXBitBuffer(LPBYTE pBuffer, int nInLength)
{
	m_pBuffer = pBuffer;
	m_nLastByte = nInLength;
	m_ucMask = 0x80;
	m_cByte = 0; 		// index gelezen byte
	m_nRack = 0;
}

void COXBitBuffer::OutputBit(int bit)
{   
	if ( bit )
		m_nRack |= m_ucMask;

	m_ucMask >>= 1;
	if ( m_ucMask == 0 ) 
	{   
		m_pBuffer[m_cByte] = (BYTE)m_nRack;
		m_cByte++;
		m_nRack = 0;
		m_ucMask = 0x80;
	}
}

void COXBitBuffer::OutputBits(int code, int count)
{
	int test = 0;       
	unsigned long mask;
	mask = 1L << ( count - 1 );
	while ( mask != 0) 
	{
		if ( mask & code )
			m_nRack |= m_ucMask;
		m_ucMask >>= 1;
		if ( m_ucMask == 0 ) 
		{
			m_pBuffer[m_cByte] = (BYTE)m_nRack;
			m_cByte++;
			m_nRack = 0;
			m_ucMask = 0x80;
		}
		mask >>= 1;
	}
	if (m_ucMask == 0x01)
		test++;
} 

void COXBitBuffer::OutputBitsEOS()
{
	if ( m_ucMask == 0x80)
		return;
	while ( m_ucMask != 0)
	{
		m_ucMask >>= 1;
		if ( m_ucMask == 0 ) 
		{
			m_pBuffer[m_cByte] = (BYTE)m_nRack;
			m_cByte++;
		}
	}
}

int	COXBitBuffer::InputBit()
{
	int value;
	if (m_ucMask == 0x80) 
	{  
		m_nRack = m_pBuffer[m_cByte];
		m_cByte++;
	}
	value = m_nRack & m_ucMask;
	m_ucMask >>= 1;
	if ( m_ucMask == 0 )
		m_ucMask = 0x80;
	return( value ? 1 : 0 );
}

unsigned long COXBitBuffer::InputBits(int bit_count)
{
	unsigned long mask;
	unsigned long return_value;
	mask = 1L << ( bit_count - 1 );
	return_value = 0;
	while ( mask != 0) 
	{
		if ( m_ucMask == 0x80 ) 
			if (m_cByte < m_nLastByte )
			{  
				m_nRack = m_pBuffer[m_cByte];
				m_cByte++;
			}                                            
			else
				return 0;    
		if ( m_nRack & m_ucMask )
			return_value |= mask;
		mask >>= 1;
		m_ucMask >>= 1;
		if ( m_ucMask == 0 )
			if ( m_nLastByte == m_cByte )
				return 0;
			else
				m_ucMask = 0x80;
	}
	return( return_value );
}	

#ifdef _DEBUG
void COXBitBuffer::Dump(CDumpContext& dc) const
{
	int binair[8];
	char sTxt[10];
	int hulp;
	CString xStr;

	CObject::Dump(dc);

	for (int index = 0; index < m_cByte; index++)
	{
		hulp = m_pBuffer[index];
		UTBStr::itoa(index, sTxt, 10, 10);
		xStr = CString(sTxt);
		dc << xStr << CString(TEXT(" "));
		UTBStr::itoa(hulp, sTxt, 10, 10);
		xStr = CString(sTxt);
		dc << xStr << CString(TEXT(" "));
		for (int exponent = 0; exponent < 8; exponent++)
		{
			binair[7-exponent] = hulp % 2;
			hulp /=2;    		 		
		}           
		for (int tel=0;tel<8;tel++)
		{
			UTBStr::itoa(binair[tel],sTxt, 10, 10);
			xStr = CString(sTxt);
			dc << xStr;
		}
		dc << CString(TEXT(" "));
	}	
}

void COXBitBuffer::AssertValid() const
{
	CObject::AssertValid();
}
#endif

COXBitBuffer::~COXBitBuffer()
{
}

// protected:

// private:

// ==========================================================================
