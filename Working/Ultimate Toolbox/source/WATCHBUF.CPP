// ==========================================================================
// 							Class Implementation : COXWatchBuffer
// ==========================================================================

// Source file :watchbuf.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "watchbuf.h"	// class specification
#include "UTB64Bit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXWatchBuffer, CObject)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members


// Data members -------------------------------------------------------------
// protected:
// LPBYTE m_pBuffer;
// --- The actual buffer that holds the data

// LPBYTE m_pFlags;
// --- Array of flags with the same size os the buffer
//     Each element signifies whether the corresponding element
//	   in the buffer has been changed by operator[]

// int m_nLength;
// --- The length of the buffer (and the flag array)

// private:

// Member functions ---------------------------------------------------------
// public:

COXWatchBuffer::COXWatchBuffer()
:
m_pBuffer(NULL),
m_pFlags(NULL),
m_nLength(0)
{
	ASSERT_VALID(this);
}

void COXWatchBuffer::Create(int nLength)
{
	ASSERT_VALID(this);
	ASSERT(0 < nLength);
	m_pBuffer = new BYTE[nLength];
	m_pFlags = new BYTE[nLength];
	m_nLength = nLength;

	// ... Clear buffer and all flags by default
	Empty();	

	ASSERT_VALID(this);
}

int COXWatchBuffer::GetLength() const
{ 
	ASSERT_VALID(this);
	return m_nLength; 
}

LPBYTE COXWatchBuffer::Get(int nIndex /* = 0 */) const
{ 
	ASSERT((0 <= nIndex) && (nIndex < m_nLength));
	return &m_pBuffer[nIndex]; 
}

void COXWatchBuffer::Set(int nIndex, LPBYTE pNewElements, int nLength /* = 1 */)
{ 
	// Return for empty request
	if (nLength == 0)
		return;

	ASSERT((0 <= nIndex) && (nIndex + nLength <= m_nLength));
	ASSERT(AfxIsValidAddress(pNewElements, nLength * sizeof(BYTE)));
	memcpy(&m_pBuffer[nIndex], pNewElements, nLength); 
	memset(&m_pFlags[nIndex], TRUE, nLength); 
	ASSERT_VALID(this);
}

BOOL COXWatchBuffer::IsModified() const
{
	ASSERT_VALID(this);
	return (memchr(m_pFlags, TRUE, m_nLength) != NULL);
}

BOOL COXWatchBuffer::IsModified(int nIndex) const
{
	ASSERT((-1 <= nIndex) && (nIndex < m_nLength));
	ASSERT_VALID(this);
	return m_pFlags[nIndex];
}

void COXWatchBuffer::SetModified(BOOL bModified)
{
	ASSERT_VALID(this);
	memset(m_pFlags, bModified, m_nLength);
}

void COXWatchBuffer::SetModified(BOOL bModified, int nIndex)
{
	ASSERT_VALID(this);
	m_pFlags[nIndex] = BYTE(bModified);
}

int COXWatchBuffer::GetModified() const
{
	ASSERT_VALID(this);
	ASSERT(sizeof(BYTE) == 1);
	LPBYTE pModified = (LPBYTE)memchr(m_pFlags, TRUE, m_nLength);
	if (pModified != NULL)
		return PtrToInt(pModified - m_pFlags);
	else
		return -1;
}

int COXWatchBuffer::GetUnmodified() const
{
	ASSERT_VALID(this);
	ASSERT(sizeof(BYTE) == 1);
	LPBYTE pModified = (LPBYTE)memchr(m_pFlags, FALSE, m_nLength);
	if (pModified != NULL)
		return PtrToInt(pModified - m_pFlags);
	else
		return -1;
}

void COXWatchBuffer::Empty(int nStartIndex /* = 0 */, int nLength /* = -1 */)
{
	ASSERT((0 <= nStartIndex) && (nStartIndex + nLength <= m_nLength));
	ASSERT_VALID(this);
	if(nLength == -1)
		nLength = m_nLength - nStartIndex;
	memset(&m_pBuffer[nStartIndex], 0, nLength);
	memset(&m_pFlags[nStartIndex], 0, nLength);
}

void COXWatchBuffer::Destroy()
{
	ASSERT_VALID(this);
	delete[] m_pBuffer;
	delete[] m_pFlags;
	m_pBuffer = NULL;
	m_pFlags = NULL;
	m_nLength = 0;
	ASSERT_VALID(this);
}

#ifdef _DEBUG
void COXWatchBuffer::Dump(CDumpContext& dc) const
{
	ASSERT_VALID(this);

	CObject::Dump(dc);

	dc << TEXT("containing ") << m_nLength << TEXT(" elements");
	if (dc.GetDepth() > 0)
	{
		dc << TEXT("\n");
		for (int i = 0; i < m_nLength; i++)
		{
			dc << TEXT("\n\tm_pBuffer[") << i << TEXT("] = ") << m_pBuffer[i];
			dc << TEXT("\n\tm_pFlags[") << i << TEXT("] = ") << m_pFlags[i];
		}
	}
}

void COXWatchBuffer::AssertValid() const
{
	CObject::AssertValid();
	if (m_nLength == 0)
	{
		ASSERT(m_pBuffer == NULL);
		ASSERT(m_pFlags == NULL);
	}
	else
	{
		ASSERT(AfxIsValidAddress(m_pBuffer, m_nLength * sizeof(BYTE)));
		ASSERT(AfxIsValidAddress(m_pFlags, m_nLength * sizeof(BYTE)));
	}
}
#endif

COXWatchBuffer::~COXWatchBuffer()
{
	ASSERT_VALID(this);
	if (m_nLength != 0)
		Destroy();
	ASSERT_VALID(this);
}

// protected:

// private:

// ==========================================================================
