// ==========================================================================
// 					Class Specification : COXWorkspaceState
// ==========================================================================

// Header file : OXWorkspaceState.h

// Version: 9.3

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
//	This class can contain all the settings of a "workspace"
//	A workspace includes : 
//		- The position of the main application window and its state,
//		- The state of the controlbars (toolbars, statusbar etc)
//		- The position and type of all the open frame windows on documents
//	For each open open frame window the doc template is computed and the name of the
//	 document is queried

//	All these properties can be stored and loaded by using a file, an archive
//	 or the registry.

//	Apart from constructing the object the following functions are sufficient in most cases
//	 StoreToFile() and LoadFromFile() or 
//	 StoreToRegistry() and LoadFromRegistry()

// Remark:
//	This class has only been tested in an MDI environment (not SDI or Dialog based)
//	 because in this situation it is most useful.

//	When the mainframe was minimized when the state was stored, the window
//	 will be shown in a restored state when the state is loaded again.
//	This is doen because starting an application in minimized state is often
//	 confusing for the user.

//	Coordinates with are saved can later be used to restore the original state
//	These numbers are not scaled, so when the screen resolution changes between 
//	 the save and the load, the restored situation will not produce the
//	 desired result.
//	The original screen size is saved together with the rest of the workspace
//	 so an extension can use this to implement scaling.

// Prerequisites (necessary conditions):
//	When the state of a frame window (view of document) is about to be stored,
//   first the correct template to restore it it will be searched.
//	The correct template is the one with the same runtime class for
//	 document, frame and view.  So this combination should be unique for all
//	 document templates.

//	A frame window may contain a splitter window with multiple views.
//	The view in the first pane will be used to establish the document template 
//	 to recreate the document later on.

/////////////////////////////////////////////////////////////////////////////

#if (_MFC_VER < 0x0420)
// This file uses classes that were introduced in MFC Version 4.2
// These classes are now officially documented by Microsoft, but did not exist in previous versions
// Therefore this file will be completely excluded for older versions of MFC
#pragma message("Warning : OXWorkspaceState.h not included because MFC Version < 4.2")
#else
// The entire file

#ifndef __OXWORKSPACESTATE_H__
#define __OXWORKSPACESTATE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include <afxadv.h>
#include <afxtempl.h>
#include "OXChildFrameState.h"

struct OXDOCKTABPOSITION
{
	UINT nControlBarID;
	UINT nDockBarID;
	int iTabIndex;
	BOOL bSelected; // TRUE is this tab is the selected one, FALSE otherwise
};

class COXChildFrameState;

class OX_CLASS_DECL COXWorkspaceState : public CObject
{
DECLARE_SERIAL(COXWorkspaceState);

// Data members -------------------------------------------------------------
public:
	// ... Schema version of this class used in serialization
	static const int m_nSerializeSchemaVersion;
	// ... Magic number identifying this class
	static const int m_nMagicNumber;

protected:
	CSize	m_screenSize;

	BOOL	m_bSaveMainWndProps;
	WINDOWPLACEMENT	m_mainWndPlacement;

	BOOL	m_bSaveBarProps;
	CDockState* m_pDockState;

	// This map is used to save the positions of all
	// tabbed control bars. It maps the ID of the
	// docking window to its index in the tab control.
	// If the window is not docked, then the mapped
	// value will be -1.
	typedef CMap<UINT, UINT, OXDOCKTABPOSITION, OXDOCKTABPOSITION> BarID2TabIdxMap;
	BarID2TabIdxMap m_mapBarID2TabIdx;	

	BOOL m_bSaveChildFrames;
	CObArray* m_pChildFrameStates;

	BOOL m_bSaveSplitterPanes;
	CObArray* m_pSplitterPanes;

	int m_nSerializeSchemaVersionLoad;

private:
	
// Member functions ---------------------------------------------------------
public:
	COXWorkspaceState();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object

	BOOL IsMainWindowIncluded() const;
	BOOL IsBarPropertyIncluded() const;
	BOOL IsChildFrameIncluded() const;
	BOOL IsSplitterPaneIncluded() const;
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the specified part is included in this workspace
	// --- Effect : 

	void IncludeMainWindow(BOOL bInclude = TRUE);
	void IncludeBarProperty(BOOL bInclude = TRUE);
	void IncludeChildFrame(BOOL bInclude = TRUE);
	void IncludeSplitterPane(BOOL bInclude = TRUE);
	// --- In  : bInclude : Whether the specified item should be included
	//						in this workspace (all default to TRUE)
	// --- Out : 
	// --- Returns : 
	// --- Effect : 

	virtual BOOL ComputeProperties();
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Computes the current workspace and stores it into this object

	virtual BOOL ApplyProperties() const;
	// --- In  : 
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Applies the properties of this object to the current workspace 

	virtual void Serialize(CArchive& ar);
	// --- In  : ar : Archive used in serialization
	// --- Out : 
	// --- Returns :
	// --- Effect : Serializes the object
	//				This action may throw an exception on failure

	virtual BOOL StoreToFile(LPCTSTR pszPath, BOOL bSaveOpenDocs = TRUE, 
		BOOL bComputeProperties = TRUE, BOOL bCloseApplication = TRUE);
	// --- In  : pszPath : File path to store to
	//			 bSaveOpenDocs : Whether open document that have been changed should be
	//				saved first (unnamed docs will then receive a name)
	//			 bComputeProperties :  Whether the current workspace should be recomputed
	//				(Same as ComputeProperties())
	//			 bCloseApplication : Whether this function has been called when 
	//				application is being closed
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Stores the properties from this object to the specified file

	virtual BOOL LoadFromFile(LPCTSTR pszPath, BOOL bApplyProperties = TRUE);
	// --- In  : pszPath : File path to load from
	//			 bApplyProperties :  Whether read properties should be applied to the current workspace 
	//				(Same as ApplyProperties())
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Reads properties from file to this object

	virtual BOOL StoreToRegistry(LPCTSTR pszValueName = NULL, 
		LPCTSTR pszCompany = NULL, LPCTSTR pszApplication = NULL, 
		HKEY hKeyRoot = HKEY_CURRENT_USER, BOOL bSaveOpenDocs = TRUE, 
		BOOL bComputeProperties = TRUE, BOOL bCloseApplication = TRUE);
	// --- In  : pszValueName   The name of the value to set. 
	//				(A default name will be used when this is NULL)
	//			 pszCompany : The name of the subkey with which a value is associated. 
	//				If the parameter is NULL, AfxGetApp()->m_pszRegistryKey is used 
	//				(use AfxGetApp()->SetRegistryKey() to set this to your company name)
	//			 pszApplication : Name of this application
	//				If the paramater is NULL, AfxGetApp()->m_pszProfileName is used
	//			 hKeyRoot : An open key in the registry or any of the following predefined handle values: 
	//				HKEY_CLASSES_ROOT, HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE, HKEY_USERS
	//			 bSaveOpenDocs : Whether open document that have been changed should be
	//				saved first (unnamed docs will then receive a name)
	//			 bComputeProperties :  Whether the current workspace should be recomputed
	//				(Same as ComputeProperties())
	//			 bCloseApplication : Whether this function has been called when 
	//				application is being closed
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Stores the properties from this object to the registry

	virtual BOOL LoadFromRegistry(LPCTSTR pszValueName = NULL, 
		LPCTSTR pszCompany = NULL, LPCTSTR pszApplication = NULL, 
		HKEY hKeyRoot = HKEY_CURRENT_USER, BOOL bApplyProperties = TRUE);
	// --- In  : pszValueName   The name of the value to read
	//				(A default name will be used when this is NULL)
	//			 pszCompany : The name of the subkey with which a value is associated. 
	//				If the parameter is NULL, AfxGetApp()->m_pszRegistryKey is used 
	//				(use AfxGetApp()->SetRegistryKey() to set this to your company name)
	//			 pszApplication : Name of this application
	//				If the paramater is NULL, AfxGetApp()->m_pszProfileName is used
	//			 hKeyRoot : An open key in the registry or any of the following predefined handle values: 
	//				HKEY_CLASSES_ROOT, HKEY_CURRENT_USER, HKEY_LOCAL_MACHINE, HKEY_USERS
	//			 bApplyProperties :  Whether read properties should be applied to the current workspace 
	//				(Same as ApplyProperties())
	// --- Out : 
	// --- Returns : Whether it succeeded or not
	// --- Effect : Reads properties from registry to this object



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

	virtual ~COXWorkspaceState();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object

protected:
	void Initialize();

	virtual void ComputeDockingWindowsState(CPtrArray& aryBarInfo);
	virtual void ApplyDockingWindowsState(CPtrArray& aryBarInfo) const;

	virtual BOOL ComputeChildFrameState();
	virtual BOOL ApplyChildFrameState() const;

	virtual BOOL ComputeSplitterPanes(CFrameWnd* pFrameWnd);
	virtual BOOL ApplySplitterPanes(CFrameWnd* pFrameWnd) const;

	virtual void StoreProperties(CArchive& ar);
	virtual void LoadProperties(CArchive& ar);
	virtual void StoreLoadFile(LPCTSTR pszPath, BOOL bStoring);

	virtual BOOL SetBinaryWorkspaceState(CByteArray* pBinaryState);
	virtual BOOL GetBinaryWorkspaceState(CByteArray* pBinaryState);
	virtual BOOL StoreLoadRegistry(BOOL bStoring, LPCTSTR pszValueName, LPCTSTR pszCompany, 
		LPCTSTR pszApplication, HKEY hKeyRoot, CByteArray* pBinaryState);

	static void EmptyChildFrames(CObArray* pChildFrameStates);
	static void EmptySplitterPanes(CObArray* pSplitterPanes);
	static POSITION GetFirstMDIChildWndPosition();
	static CMDIChildWnd* GetNextMDIChildWnd(POSITION& pos);
	static CSplitterWnd* GetSplitterWindow(CFrameWnd* pFrameWnd);

private:
                   
};

#endif // __OXWORKSPACESTATE_H__
#endif // _MFC_VER
// ==========================================================================
