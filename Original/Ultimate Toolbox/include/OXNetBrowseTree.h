// ==========================================================================
// 					Class Specification : COXNetBrowseTree
// ==========================================================================

// Header file : OXNetBrowseTree.h

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
//	YES	Derived from CTreeCtrl

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	YES	Needs a resource (IDB_NET_IMAGELIST bitmap)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This tree control shows the network resources.
//	It is derived from CTreeCtrl so it can be used to subclass a tree control
//	Some initial setting can be supplied through the functions
//	 ShowCommentName(), ShowDisks(), ShowPrinters(), SetMaxNumLevels()
//	The normal notifications from this tree control to its parent window can be used
//	 (e.g. TVN_SELCHANGED)

//	GetCurrentNetResource() returns the network resource of the currently selected item
//	 It contains info about : type, display type, local name, remote name, comment and provider

// Remark:
//	It uses the bitmap resource IDB_NET_IMAGELIST to show icons for each type of resource
//	Some string resources are used as well
//	The reserved ID ranges are : 23100 -> 23119 and 53100 -> 53119

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXNETBROWSETREE_H__
#define __OXNETBROWSETREE_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include <afxtempl.h>
#include "WinNetWk.h"	// To browse network resources (mpr.lib)
// ... Make sure linker uses the the network library
#pragma comment(lib, "mpr.lib")




// To work around the problem when putting a HTREEITEM in an MFC collection
// we declare the dummy structure here (see also MSDN PSS ID Number: Q140229)
struct _TREEITEM {};

class OX_CLASS_DECL COXNetBrowseTree : public CTreeCtrl
{
DECLARE_DYNAMIC(COXNetBrowseTree);

// Data members -------------------------------------------------------------
public:
protected:
	BOOL m_bInitialized;
	BOOL m_bShowCommentName;
	BOOL m_bShowDisks;
	BOOL m_bShowPrinters;
	int m_nResourceScope;
	int m_nMaxNumLevels;
	int m_nInitialExpandLevel;
	BOOL m_bReport;

	const NETRESOURCE* m_pCurrentNetResource;
	CImageList m_netImages;

	CMap<HTREEITEM, HTREEITEM, NETRESOURCE*, NETRESOURCE*> m_resourceMap;

private:
	
// Member functions ---------------------------------------------------------
public:
	COXNetBrowseTree();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object

	BOOL IsInitialized() const;
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the initialization was successful
	// --- Effect : 

	virtual BOOL Initialize();
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the initialization was successful
	// --- Effect : Initializes the object
	//				This function is called automatically and should almost never be called directly

	BOOL ShowCommentName(BOOL bShow);
	// --- In  : bShow : Whether the comment name of each resource should be shown as well
	// --- Out : 
	// --- Returns : The previous setting
	// --- Effect : 

	BOOL ShowDisks(BOOL bShow);
	// --- In  : bShow : Whether disk resources should be shown
	// --- Out : 
	// --- Returns : The previous setting
	// --- Effect : 

	BOOL ShowPrinters(BOOL bShow);
	// --- In  : bShow : Whether printer resources should be shown
	// --- Out : 
	// --- Returns : The previous setting
	// --- Effect : 

	int SetResourceScope(int nScope);
	// --- In  : nScope : The scope of resource that will be displayed
	// --- Out : 
	// --- Returns : The previous setting
	// --- Effect : Possible values are RESOURCE_CONNECTED (connected resources), 
	//				RESOURCE_GLOBALNET (global resources) or
	//				RESOURCE_REMEMBERED (persistent connections)
	//				RESOURCE_RECENT (recent connections, Win 4 only)
	//				RESOURCE_CONTEXT (current context resources, Win 4 only)
	//				By default RESOURCE_GLOBALNET is used

	int SetMaxNumLevels(int nMaxNumLevels);
	// --- In  : nMaxNumLevels : The maximum number of levels of the tree that may be shown
	//                       Deeper children will not be accessible
	//						 (1 = only the root, 2 = only the root and its direct children, etc.)
	// --- Out : 
	// --- Returns : The previous setting
	// --- Effect : By default all levels are shown

	int SetInitialExpandLevel(int nLevel);
	// --- In  : nLevels : Number of levels to expand
	// --- Out : 
	// --- Returns : The previous setting
	// --- Effect : Set the number of levels the tree should be expanded
	//              when it is first show
	//				When you expand 1 level you will end up with 2 levels shown
	//				(the root level and the expanded level)

	BOOL ReportErrors(BOOL bReport);
	// --- In  : bReport : Whether network errors should be reported to the user
	// --- Out : 
	// --- Returns : The previous setting
	// --- Effect : 

	virtual BOOL BuildTreeContents();
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the build was successful
	// --- Effect : Fills the tree control with its actual contents
	//				All setting that are important for this (e.g. ShowDisks() etc)
	//				are used now
	//				This function is called automatically but can be called
	//				explicitly to rebuild the tree

	const NETRESOURCE* GetCurrentNetResource() const;
	// --- In  :
	// --- Out : 
	// --- Returns : The net resource info belonging to the currently foccused item
	// --- Effect : When this function is called after the window has been destroyed
	//              (but before this C++ object has been deleted) the last selected
	//              item is used.

	const NETRESOURCE* GetAssocNetResource(HTREEITEM hTreeItem) const;
	// --- In  : hTreeItem : Node of which the associated net resource is requested
	// --- Out : 
	// --- Returns : The net resource associated with the specified node
	// --- Effect : Should only be called for valid tree items

	void ExpandBranch(HTREEITEM hParentItem, int nLevels);
	// --- In  : hParentItem : Item to expand
	//			 nLevels : Number of levels to expand
	// --- Out : 
	// --- Returns : 
	// --- Effect : Expand a specified item of the tree a specified number of levels

	int GetItemLevel(HTREEITEM hTreeItem);
	// --- In  : hTreeItem
	// --- Out : 
	// --- Returns : The level in the tree on which the item exists
	//               (root = 0, its direct children = 1, etc)
	// --- Effect : 

	virtual BOOL Uninitialize();
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the uninitialization was successful
	// --- Effect : Uninitializes the object
	//				This function is called automatically and should almost never be called directly

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

	virtual ~COXNetBrowseTree();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXNetBrowseTree)
	protected:
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	virtual NETRESOURCE* GetAssocNetResourceEx(HTREEITEM hTreeItem) const;
	BOOL ExpandNode(HTREEITEM hTreeItem);
	BOOL CreateChildren(HTREEITEM hParentItem, NETRESOURCE* pParentNetResources);
	HTREEITEM InsertResourceItem(const NETRESOURCE* pNetResources, HTREEITEM hParentItem);
	void Cleanup();
	virtual void ReportNetError(DWORD nResult, LPCTSTR pszResource);
	CString GetResultMessage(HRESULT resultCode, ...);
	BOOL RetrieveResultMessage(CString sModuleName, HRESULT resultCode, 
		va_list* pArgs, CString& sResultMessage);

	virtual BOOL PreInsertResourceItem(const NETRESOURCE* pNetResources, TV_INSERTSTRUCT * pIinsertStruct);
	virtual void PostInsertResourceItem(const NETRESOURCE* pNetResources, TV_INSERTSTRUCT * pInsertStruct, HTREEITEM hNewItem);

	//{{AFX_MSG(COXNetBrowseTree)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnPostInit(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
                   
};

#endif // __OXNETBROWSETREE_H__
// ==========================================================================
