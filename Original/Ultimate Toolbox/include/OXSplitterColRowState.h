// ==========================================================================
// 					Class Specification : COXSplitterColRowState
// ==========================================================================

// Header file : OXSplitterColRowState.h

// Version: 9.3

// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CObject

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This class represents the state of one row or column of the splitter window

// Remark:

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#if (_MFC_VER < 0x0420)
// This file uses classes that were introduced in MFC Version 4.2
// These classes are now officially documented by Microsoft, but did not exist in previous versions
// Therefore this file will be completely excluded for older versions of MFC
#pragma message("Warning : OXSplitterColRowState.h not included because MFC Version < 4.2")
#else
// The entire file

#ifndef __OXSPLITTERCOLROWSTATE_H__
#define __OXSPLITTERCOLROWSTATE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


class OX_CLASS_DECL COXSplitterColRowState : public CObject
{
DECLARE_SERIAL(COXSplitterColRowState);

// Data members -------------------------------------------------------------
public:
	// ... Schema version of this class used in serialization
	static const int m_nSerializeSchemaVersion;

protected:
	BOOL m_bRow;
	int m_nColRowIndex;
	int m_nIdealSize;
	int m_nMinSize;

	int m_nSerializeSchemaVersionLoad;

private:
	
// Member functions ---------------------------------------------------------
public:
	COXSplitterColRowState();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object

	BOOL ComputeProperties(CSplitterWnd* pSplitterWnd, int nColRowIndex, BOOL bRow);
	// --- In  : pSplitterWnd : The splitter window to use
	//			 nColRowIndex : The index of the row (or column)
	//			 bRow : Whether this is a row or column
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Computes the state of the splitter row or column
	//				and stores it into this object

	BOOL ApplyProperties(CSplitterWnd* pSplitterWnd) const;
	// --- In  : pSplitterWnd : The splitter window to use
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Applies the properties of this object to the specified splitter window

	virtual void Serialize(CArchive& ar);
	// --- In  : ar : Archive used in serialization
	// --- Out : 
	// --- Returns :
	// --- Effect : Serializes the object

#ifdef _DEBUG
	virtual void AssertValid() const;
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : AssertValid performs a validity check on this object 
	//				by checking its internal state. 
	//				In the Debug version of the library, AssertValid may assert and 
	//				thus terminate the program.
	
	virtual void Dump(CDumpContext& dc) const;
	// --- In  : dc : The diagnostic dump context for dumping, usually afxDump.
	// --- Out : 
	// --- Returns :
	// --- Effect : Dumps the contents of the object to a CDumpContext object. 
	//				It provides diagnostic services for yourself and 
	//				 other users of your class. 
	//				Note  The Dump function does not print a newline character
	//				 at the end of its output. 
#endif

	virtual ~COXSplitterColRowState();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object

protected:
	void Initialize();

	void StoreProperties(CArchive& ar);
	void LoadProperties(CArchive& ar);

private:
                   
};

OX_API_DECL void AFXAPI SerializeElements(CArchive& ar, 
										  COXSplitterColRowState* pSplitterColRowState, 
										  int nCount);

#endif // __OXSPLITTERCOLROWSTATE_H__
#endif // _MFC_VER
// ==========================================================================
