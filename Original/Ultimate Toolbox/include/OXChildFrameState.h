// ==========================================================================
// 					Class Specification : COXChildFrameState
// ==========================================================================

// Header file : OXChildFrameState.h

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
//	NO	Needs a resource (template)

//	YES	Persistent objects (saveable on disk)      
//	YES	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This object represents the state of one MDI child frame window
//	It may include a collection of the prositiosn of all the panes of
//	 the splitter window (when applicable)

// Remark:

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#if (_MFC_VER < 0x0420)
// This file uses classes that were introduced in MFC Version 4.2
// These classes are now officially documented by Microsoft, but did not exist in previous versions
// Therefore this file will be completely excluded for older versions of MFC
#pragma message("Warning : OXChildFrameState.h not included because MFC Version < 4.2")
#else
// The entire file

#ifndef __OXCHILDFRAMESTATE_H__
#define __OXCHILDFRAMESTATE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


class OX_CLASS_DECL COXChildFrameState : public CObject
{
DECLARE_SERIAL(COXChildFrameState);

// Data members -------------------------------------------------------------
public:
	// ... Schema version of this class used in serialization
	static const int m_nSerializeSchemaVersion;

protected:
	WINDOWPLACEMENT	m_framePlacement;
	CString m_sDocPath;

	CString m_sDocClassName;
	CString m_sFrameClassName;
	CString m_sViewClassName;

	BOOL m_bSaveSplitterPanes;
	CObArray* m_pSplitterPanes;

	int m_nSerializeSchemaVersionLoad;

private:
	
// Member functions ---------------------------------------------------------
public:
	COXChildFrameState();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object

	BOOL IsSplitterPaneIncluded() const;
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the panes of the splitter windows are included in this workspace
	// --- Effect : 

	void IncludeSplitterPane(BOOL bInclude = TRUE);
	// --- In  : bInclude : Whether the panes of the splitter windows should be included
	//						in this workspace (all default to TRUE)
	// --- Out : 
	// --- Returns : 
	// --- Effect : 

	virtual BOOL ComputeProperties(CFrameWnd* pFrameWnd);
	// --- In  : pFrameWnd : Frame window to use
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Computes the state of the specified MDI child frame window
	//				and stores it into this object

	virtual BOOL ApplyProperties() const;
	// --- In  : pFrameWnd : Frame window to use
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Applies the properties of this object to the specified MDI child frame window

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

	virtual ~COXChildFrameState();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object

protected:
	void Initialize();

	virtual BOOL ComputeSplitterPanes(CFrameWnd* pFrameWnd);
	virtual BOOL ApplySplitterPanes(CFrameWnd* pFrameWnd) const;

	virtual void StoreProperties(CArchive& ar);
	virtual void LoadProperties(CArchive& ar);

	static void EmptySplitterPanes(CObArray* pSplitterPanes);
	static CView* GetFirstView(CDocument* pDoc);
	static BOOL OpenDocument(CDocTemplate* pDocTemplate, LPCTSTR pszDocPath, 
		CDocument*& pDoc, CFrameWnd*& pFrameWnd, CView*& pView);
	static CDocument* SearchDocument(LPCTSTR pszDocPath);
	static CView* GetFirstView(CFrameWnd* pFrameWnd);
	static CSplitterWnd* GetSplitterWindow(CFrameWnd* pFrameWnd);
	static CDocTemplate* GetDocTemplate(LPCTSTR pszDocName, LPCTSTR pszFrameWndName, 
		LPCTSTR pszViewName);
	static BOOL GetDocView(CFrameWnd* pFrameWnd, CDocument*& pDoc, CView*& pView);

private:
                   
};

OX_API_DECL void AFXAPI SerializeElements(CArchive& ar, 
										  COXChildFrameState** pChildFrameState, 
										  int nCount);

#endif // __OXCHILDFRAMESTATE_H__
#endif // _MFC_VER
// ==========================================================================
