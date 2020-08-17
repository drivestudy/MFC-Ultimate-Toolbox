// ==========================================================================
// 					Class Specification : COXDocTemplateSpy
// ==========================================================================

// Header file : OXDocTemplateSpy.h

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
//	YES	Derived from CDocTemplate

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This is a helper class to check to the runtime class members of a template object
//  You should never create an object of this class, but you can easily cast a 
//   template object to an object of this class.

// Remark:
//	This class does not add extra members to the base class

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#if (_MFC_VER < 0x0420)
// This file uses classes that were introduced in MFC Version 4.2
// These classes are now officially documented by Microsoft, but did not exist in previous versions
// Therefore this file will be completely excluded for older versions of MFC
#pragma message("Warning : OXDocTemplateSpy.h not included because MFC Version < 4.2")
#else
// The entire file

#ifndef __OXDOCTEMPLATESPY_H__
#define __OXDOCTEMPLATESPY_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"



class COXWorkspaceState;

class OX_CLASS_DECL COXDocTemplateSpy : public CDocTemplate
{
friend class COXWorkspaceState;

// Data members -------------------------------------------------------------
public:
protected:
private:
	
// Member functions ---------------------------------------------------------
public:
	BOOL CheckMatch(CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass = NULL, 
		CRuntimeClass* pViewClass = NULL) const;
	// --- In  : pDocClass : Runtime class of document type to check, 
	//			 pFrameClass : Runtime class of frame window type to check, 
	//			 pViewClass : Runtime class of view type to check, 
	// --- Out : 
	// --- Returns : Whether the specified runtime classes are the same as those
	//               of this template object 
	// --- Effect : 

	BOOL CheckMatch(LPCTSTR pszDocClass, LPCTSTR pszFrameClass = NULL, 
		LPCTSTR pszViewClass = NULL) const;
	// --- In  : pszDocClass : Name of the runtime class of document type to check, 
	//			 pszFrameClass : Name of the runtime class of frame window type to check, 
	//			 pszViewClass : Name of the runtime class of view type to check, 
	// --- Out : 
	// --- Returns : Whether the specified names of runtime classes are the same 
	//				 as those of this template object 
	// --- Effect : 

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


protected:
	COXDocTemplateSpy();
	virtual ~COXDocTemplateSpy();

private:
                   
};

#endif // __OXDOCTEMPLATESPY_H__
#endif // _MFC_VER
// ==========================================================================
