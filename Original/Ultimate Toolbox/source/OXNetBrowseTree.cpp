// ==========================================================================
//				Class Implementation : COXNetBrowseTree
// ==========================================================================

// Source file : OXNetBrowseTree.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXNetBrowseTree.h"
#include "OXMainRes.h"

#include "UTBStrOp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXNetBrowseTree, CTreeCtrl)

#define new DEBUG_NEW

// Images in the small image list for this network tree
#define NET_IMAGE_UNKNOWN				0
#define NET_IMAGE_UNKNWON_EXPANDED		1
#define NET_IMAGE_DOMAIN				2
#define NET_IMAGE_DOMAIN_EXPANDED		3
#define NET_IMAGE_SERVER				4
#define NET_IMAGE_SERVER_EXPANDED		5
#define NET_IMAGE_DISK_SHARE			6
#define NET_IMAGE_DISK_SHARE_EXPANDED	7
#define NET_IMAGE_PRINTER_SHARE			8
#define NET_IMAGE_PRINTER_SHARE_EXPANDED 9
#define NET_IMAGE_OTHER_SHARE			10
#define NET_IMAGE_OTHER_SHARE_EXPANDED	11
#define NET_IMAGE_FILE					12
#define NET_IMAGE_FILE_EXPANDED			13
#define NET_IMAGE_GROUP					14
#define NET_IMAGE_GROUP_EXPANDED		15
#define NET_IMAGE_NETWORK				16
#define NET_IMAGE_NETWORK_EXPANDED		17
#define NET_IMAGE_ROOT					18
#define NET_IMAGE_ROOT_EXPANDED			19
#define NET_IMAGE_SHARADMIN				20
#define NET_IMAGE_SHAREADMIN_EXPANDED	21
#define NET_IMAGE_DIRECTORY				22
#define NET_IMAGE_DIRECTORY_EXPANDED	23
#define NET_IMAGE_TREE					24
#define NET_IMAGE_TREE_EXPANDED			25

/////////////////////////////////////////////////////////////////////////////
// Definition of static members
static TCHAR szUnknownError[] = _T("*** Unknown Error ***");

static OSVERSIONINFO OSVersion;
struct _OX_WINDOWS_VERSION
{
	_OX_WINDOWS_VERSION()
	{
		OSVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		VERIFY(::GetVersionEx(&OSVersion));
	}
};
static const _OX_WINDOWS_VERSION windowsVersion;


// Data members -------------------------------------------------------------
// protected:
// BOOL m_bInitialized;
// --- Whether the control has been properly initialized
//     This is done by posting the message WM_POST_INIT 

// BOOL m_bShowCommentName;
// --- Whether the comment should be shown for each net resource

// BOOL m_bShowDisks;
// --- Whether disk resources should be shown

// BOOL m_bShowPrinters;
// ---- Whether printer resources should be shown

// int m_nResourceScope;
// ---- The scope of the resources that are show in the tree control

// int m_nMaxNumLevels;
// --- The last lvel in the tree that will be shown
//     Deeper levels are hidden for the user

// int m_nInitialExpandLevel;
// --- The number levels that should be initially expanded wqhen the control
//     becomes visisble for the first time

// BOOL m_bReport;
// --- Whether network errors should be shown in an message box to the user

// NETRESOURCE* m_pCurrentNetResource;
// --- The net resource associated with the last selected item

// CImageList m_netImages;
// --- The image list used by this control

// CMap<HTREEITEM, HTREEITEM, NETRESOURCE*, NETRESOURCE*> m_resourceMap;
// --- The mapping between the handle of the tree item and its associated net resource info

// private:

// Member functions ---------------------------------------------------------
// public:

BEGIN_MESSAGE_MAP(COXNetBrowseTree, CTreeCtrl)
	//{{AFX_MSG_MAP(COXNetBrowseTree)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT_EX(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT_EX(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(TVN_GETDISPINFO, OnGetDispInfo)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_POST_INIT, OnPostInit)
END_MESSAGE_MAP()

COXNetBrowseTree::COXNetBrowseTree()
:
m_bInitialized(FALSE),
m_bShowCommentName(TRUE),
m_bShowDisks(TRUE),
m_bShowPrinters(FALSE),
m_nResourceScope(RESOURCE_GLOBALNET),
m_nMaxNumLevels(100),
m_nInitialExpandLevel(0),
m_bReport(TRUE),
m_pCurrentNetResource(NULL),
m_netImages()
{
	ASSERT_VALID(this);
}

BOOL COXNetBrowseTree::IsInitialized() const
{
	return m_bInitialized;
}

BOOL COXNetBrowseTree::Initialize()
{
	// Create the small icon image list	for context list
	// ... The bitmap resource IDB_NET_IMAGELIST should contain the images
	// ... Must find the resource
	//     (Make sure OXNetBrowseTree.rc is included in your resource file)
	ASSERT(AfxFindResourceHandle(MAKEINTRESOURCE(IDB_OX_NET_IMAGELIST), RT_BITMAP) != NULL);
	VERIFY(m_netImages.Create(IDB_OX_NET_IMAGELIST,16,0,RGB(255,0,255)));
	// m_netImages.SetBkColor(GetSysColor(COLOR_WINDOW));
	m_netImages.SetBkColor(CLR_NONE);
	// ... Associate the image lists with the context list
	SetImageList(&m_netImages, TVSIL_NORMAL);

	// Build the tree contents for the first time
	BuildTreeContents();

	m_bInitialized = TRUE;
	return m_bInitialized;
}

BOOL COXNetBrowseTree::ShowCommentName(BOOL bShow)
{
	BOOL bPrevShow = m_bShowCommentName;
	m_bShowCommentName = bShow;
	return bPrevShow;
}

BOOL COXNetBrowseTree::ShowDisks(BOOL bShow)
{
	BOOL bPrevShow = m_bShowDisks;
	m_bShowDisks = bShow;
	return bPrevShow;
}

BOOL COXNetBrowseTree::ShowPrinters(BOOL bShow)
{
	BOOL bPrevShow = m_bShowPrinters;
	m_bShowPrinters = bShow;
	return bPrevShow;
}

int COXNetBrowseTree::SetResourceScope(int nScope)
{
	ASSERT(0 <= nScope);
	int nPrevScope = m_nResourceScope;
	m_nResourceScope = nScope;
	return nPrevScope;
}

int COXNetBrowseTree::SetMaxNumLevels(int nMaxNumLevels)
{
	ASSERT(0 <= nMaxNumLevels);
	int nPrevMaxNumLevels = m_nMaxNumLevels;
	m_nMaxNumLevels = nMaxNumLevels;
	return nPrevMaxNumLevels;
}

int COXNetBrowseTree::SetInitialExpandLevel(int nLevel)
{
	int nPrevLevel = m_nInitialExpandLevel;
	m_nInitialExpandLevel = nLevel;
	return nPrevLevel;
}

BOOL COXNetBrowseTree::ReportErrors(BOOL bReport)
{
	BOOL bPrevReport = bReport;
	m_bReport = bReport;
	return bPrevReport;
}

BOOL COXNetBrowseTree::BuildTreeContents()
{

	SetRedraw(FALSE);

	// If the tree till contains nodes, remove them all
	DeleteAllItems();

	// Cleanup data members
	Cleanup();

	// Create the root node
	if (0 < m_nMaxNumLevels)
		ExpandNode(NULL);

	// Expand all top level items
	HTREEITEM hTopItem = NULL;
	hTopItem = GetRootItem();
	while (hTopItem != NULL)
	{
		ExpandBranch(hTopItem, m_nInitialExpandLevel);
		hTopItem = GetNextSiblingItem(hTopItem);
	}

	// If at least one top level item is expandable, connect lines to 
	// the root of the control otherwise do not
	BOOL bLinesAtRoot = FALSE;
	hTopItem = GetRootItem();
	while(!bLinesAtRoot && (hTopItem  != NULL))
	{
		bLinesAtRoot = ItemHasChildren(hTopItem);
		hTopItem = GetNextSiblingItem(hTopItem);
	}
	if (bLinesAtRoot)
		ModifyStyle(0, TVS_LINESATROOT /* add */);
	else
		ModifyStyle(TVS_LINESATROOT /* remove */, 0);

	SetRedraw(TRUE);

	return TRUE;
}

const NETRESOURCE* COXNetBrowseTree::GetCurrentNetResource() const
{
	return m_pCurrentNetResource;
}

const NETRESOURCE* COXNetBrowseTree::GetAssocNetResource(HTREEITEM hTreeItem) const
{
	return GetAssocNetResourceEx(hTreeItem);
}

void COXNetBrowseTree::ExpandBranch(HTREEITEM hParentItem, int nLevels)
{
	if (nLevels <= 0)
		return;

	ASSERT(hParentItem != NULL);

	// First expand the parent itself
	if (!Expand(hParentItem, TVE_EXPAND))
		// ... Expand failed (the network resource probably produced an error)
		return;

	// Then expand all its children
	HTREEITEM hChildItem = GetChildItem(hParentItem);
	while (hChildItem != NULL)
	{
		ExpandBranch(hChildItem, nLevels - 1);
		hChildItem = GetNextSiblingItem(hChildItem);
	}
}

int COXNetBrowseTree::GetItemLevel(HTREEITEM hTreeItem)
{
	int nLevel = -1;
	HTREEITEM hParentItem = hTreeItem;
	while (hParentItem != NULL)
	{
		hParentItem = GetParentItem(hParentItem);
		nLevel++;
	}
	return nLevel;
}

BOOL COXNetBrowseTree::Uninitialize()
{
	ASSERT(IsInitialized());

	// Destroy the the small icon image list
	VERIFY(m_netImages.DeleteImageList());

	m_bInitialized = FALSE;
	return TRUE;
}

#ifdef _DEBUG
void COXNetBrowseTree::AssertValid() const
{
	CTreeCtrl::AssertValid();
}

void COXNetBrowseTree::Dump(CDumpContext& dc) const
{
	CTreeCtrl::Dump(dc);
	dc << "\nm_bInitialized : " << m_bInitialized;
	dc << "\nm_bShowCommentName : " << m_bShowCommentName;
	dc << "\nm_bShowDisks : " << m_bShowDisks;
	dc << "\nm_bShowPrinters : " << m_bShowPrinters;
	dc << "\nm_nResourceScope : " << m_nResourceScope;
	dc << "\nm_nMaxNumLevels : " << m_nMaxNumLevels;
	dc << "\nm_nInitialExpandLevel : " << m_nInitialExpandLevel;
	dc << "\nm_bReport : " << m_bReport;
	dc << "\nm_pCurrentNetResource : " << m_pCurrentNetResource;
	dc << "\nm_netImages : " << &m_netImages;
	dc << "\n";
}
#endif //_DEBUG

COXNetBrowseTree::~COXNetBrowseTree()
{
	Cleanup();
}

// protected:
NETRESOURCE* COXNetBrowseTree::GetAssocNetResourceEx(HTREEITEM hTreeItem) const
// --- In  : hTreeItem : Node of which the associated net resource is requested
// --- Out : 
// --- Returns : The net resource associated with the specified node
//				 Notice that this return value is not const
// --- Effect : Should only be called for valid tree items
{
	NETRESOURCE* pNetResource = NULL;
	if (!m_resourceMap.Lookup(hTreeItem, pNetResource))
		pNetResource = NULL;
#ifdef _DEBUG
	// ... No associated net resource found, may be a dangerous situation
	if (pNetResource == NULL)
	{
		TRACE1("COXNetBrowseTree::GetAssocNetResourceEx : No associated net resource found for tree item 0x%X, returning NULL pointer\n",
			hTreeItem);
		// ASSERT(FALSE);
	}
#endif
	return pNetResource;
}

BOOL COXNetBrowseTree::ExpandNode(HTREEITEM hTreeItem) 
// --- In  : hTreeItem : Node to expand (NULL is top level)
// --- Out : 
// --- Returns :
// --- Effect : Expands the specified node
{
	// If hTreeItem == NULL we have to have an empty tree
	ASSERT((hTreeItem != NULL) || (GetRootItem() == NULL));

	// Get the net resource of the parent item
	NETRESOURCE* pNetResources = NULL;
	if (hTreeItem != NULL)
	{
		pNetResources = GetAssocNetResourceEx(hTreeItem);
		ASSERT(pNetResources != NULL);
	}
	else
		pNetResources = NULL;

	if ((pNetResources != NULL) && ((pNetResources->dwUsage & RESOURCEUSAGE_CONTAINER) != RESOURCEUSAGE_CONTAINER))
		// Net resource exists, but is not a container and thus cannot be enumerated
		return FALSE;

	// Expand the node (may take a while)
	CWaitCursor wc;
	return CreateChildren(hTreeItem, pNetResources);
}

BOOL COXNetBrowseTree::CreateChildren(HTREEITEM hParentItem, NETRESOURCE* pParentNetResources)
// --- In  : hParentItem : Node of which the children nodes have to be created
//			 pParentNetResources : Net resource of this parent node
// --- Out : 
// --- Returns :
// --- Effect : Computes the netresources of the children and creates the child nodes
{
	HANDLE hEnum = NULL;
	DWORD dwScope = pParentNetResources == NULL ? m_nResourceScope : pParentNetResources->dwScope;
	DWORD nResult = WNetOpenEnum(
		dwScope,				// scope of enumeration 
		RESOURCETYPE_ANY,		// resource types to list 
		0,						// resource usage to list 
		pParentNetResources,	// pointer to resource structure 
		&hEnum);				// pointer to enumeration handle buffer 
	if (nResult != NO_ERROR)
	{
		TRACE2("COXNetBrowseTree::CreateChildren : WNetOpenEnum failed with error code %i == 0x%X\n",
			nResult, nResult);
		ReportNetError(nResult, pParentNetResources == NULL ? NULL : pParentNetResources->lpRemoteName);
		return FALSE;
	}

	DWORD nCurrentCount(0);
	DWORD nCurrentSkipCount(0);
	/* =============================================================================== */	

	// The problem with the WNetEnumResoiurce fuction is that allthough you use 
	// 0xFFFFFFFF as requested resource count (this means everything) the function
	// does NOT return ERROR_MORE_DATA if the buffer is too small.  It only returns
	// this value if the buffer supplied is too small even for one value, in this
	// case the space needed to hold the first resource found in the enumeration
	// Normally the size of this resource should be sizeof(NETRESOURCE) which is 
	// 32 bytes but experience learned that this fluctuates between 32 bytes and 
	// more than 1000 bytes.  This is probably due to the fact that WNetEnumResource
	// also needs allocated memory for the strings inside the NETRESOURCE struct.
	// This leads to the conclusion that we cannot calculate with certainty the size
	// of the buffer we need for a certain number of resources.  The most robust
	// solution to this problem is to request a absolute number of resources, make
	// an serious and realistic estimation of the maximum amount of memory needed
	// to hold ALL requested resources and then test to see whether you have retrieved
	// all requested resources.  If this is TRUE then again enumerate the resources
	// to determine whether there aren't any left and so on until the returned
	// number of resources is smaller than the requested number.  This last remark
	// explains why we need to be sure that the requested number of resources 
	// allways fit in the amount of memory we allocated for the buffer.  We could 
	// alocated a very big buffer but we prefer the loop.

	// Also note that we don't use NETRESOURCE* pRes = new NETRESOURCE[Count]
	// because the array allocated will be an array of structs of size sizeof(NETRESOURCE)
	// and that's just not correct to  hold one netresource. That's why we use
	// GlobalAlloc.

	// USERS WHO WANT TO TUNE THE PERFORMANCE OF THIS FUNCTION CAN PLAY WITH THE 
	// NUMBER OF NETRESOURCES VIA THE nCOUNT VARIABLE AND WITH THE SIZE OF THE 
	// ALLOCATED BUFFER VIA THE nBUFFERSIZE VARIABLE BUT KEEP THE REMARKS ABOVE
	// IN MIND.
	/* =============================================================================== */
	// Start with a reasonable buffer size
	DWORD nCount = 5;
	DWORD nBufferSize = 5000;
	LPNETRESOURCE rgpNetResources = (LPNETRESOURCE)GlobalAlloc(GPTR, nBufferSize);

	while (TRUE)
	{
		DWORD nTempCount = nCount;
		DWORD nTempBufferSize = nBufferSize;
		memset(rgpNetResources, 0, nBufferSize);
		DWORD nResult2 = WNetEnumResource(
			hEnum,					// handle to enumeration 
			&nTempCount,				// pointer to entries to list 
			(LPVOID)rgpNetResources, // pointer to buffer for results 
			&nTempBufferSize);			// pointer to buffer size variable 
		TRACE2("COXNetBrowseTree::WNetEnumResource : Number of Netresources (%i), in buffersize (0x%X)\n", nTempCount, nTempBufferSize);

		if ((nResult2 != NO_ERROR) && (nResult2 != ERROR_NO_MORE_ITEMS) &&
			(nResult2 != ERROR_MORE_DATA))
		{
			TRACE2("COXNetBrowseTree::CreateChildren : WNetEnumResource failed with error code %i == 0x%X\n",
				nResult2, nResult2);
			ReportNetError(nResult2, pParentNetResources == NULL ? NULL : pParentNetResources->lpRemoteName);
			// ... Cleanup the handle and memeory allocated
			VERIFY(WNetCloseEnum(hEnum) == NO_ERROR);
			GlobalFree((HGLOBAL)rgpNetResources);
			return FALSE;
		}

		if (nResult2 == ERROR_NO_MORE_ITEMS)
			nTempCount = 0;

		// Loop the requested number of NetResources and make tree item nodes
		{
			HTREEITEM hNewItem;
			NETRESOURCE* pSourceNetResource = NULL;
			NETRESOURCE* pCopyNetResource = NULL;
			DWORD nIndex;
			DWORD nSkipCount = 0;
			for (nIndex = 0; nIndex < nTempCount; nIndex++)
			{
				pSourceNetResource  = &rgpNetResources[nIndex];

				// Check special case for disks and printers
				if ((pSourceNetResource->dwType == RESOURCETYPE_DISK) && !m_bShowDisks)
				{
					// Skip this item
					nSkipCount++;
					continue;
				}
				if ((pSourceNetResource->dwType == RESOURCETYPE_PRINT) && !m_bShowPrinters)
				{
					// Skip this item
					nSkipCount++;
					continue;
				}

				// ... Create a new item
				hNewItem = InsertResourceItem(pSourceNetResource, hParentItem);
				if (hNewItem  == NULL)
				{
					TRACE0("COXNetBrowseTree::InsertResourceItem returned NULL\n");
					// Skip this item
					nSkipCount++;
					continue;
				}

				// Add a copy to the map
				// ... Should not yet be in map
#ifdef _DEBUG
				NETRESOURCE* pCheckNetResource = NULL;
				ASSERT(!m_resourceMap.Lookup(hNewItem, pCheckNetResource));
#endif // _DEBUG
				pCopyNetResource = new NETRESOURCE;
				// ... Copy the struct itself
				memcpy(pCopyNetResource, pSourceNetResource, sizeof(NETRESOURCE));
				// ... Make a copy of all the string members
				if (pSourceNetResource->lpLocalName != NULL)
				{
					size_t len = _tcslen(pSourceNetResource->lpLocalName) + 1;
					pCopyNetResource->lpLocalName = new TCHAR[len];
					UTBStr::tcscpy(pCopyNetResource->lpLocalName, len, pSourceNetResource->lpLocalName);
				}
				else
				{
					pCopyNetResource->lpLocalName = new TCHAR[1];
					*pCopyNetResource->lpLocalName = _T('\0');
				}

				if (pSourceNetResource->lpRemoteName != NULL)
				{
					size_t len = _tcslen(pSourceNetResource->lpRemoteName) + 1;
					pCopyNetResource->lpRemoteName = new TCHAR[len];
					UTBStr::tcscpy(pCopyNetResource->lpRemoteName, len, pSourceNetResource->lpRemoteName);
				}
				else
				{
					pCopyNetResource->lpRemoteName = new TCHAR[1];
					*pCopyNetResource->lpRemoteName = _T('\0');
				}

				if (pSourceNetResource->lpComment != NULL)
				{
					size_t len = _tcslen(pSourceNetResource->lpComment) + 1;
					pCopyNetResource->lpComment = new TCHAR[len];
					UTBStr::tcscpy(pCopyNetResource->lpComment, len, pSourceNetResource->lpComment);
				}
				else
				{
					pCopyNetResource->lpComment = new TCHAR[1];
					*pCopyNetResource->lpComment = _T('\0');
				}

				if (pSourceNetResource->lpProvider != NULL)
				{
					size_t len = _tcslen(pSourceNetResource->lpProvider) + 1;
					pCopyNetResource->lpProvider = new TCHAR[len];
					UTBStr::tcscpy(pCopyNetResource->lpProvider, len, pSourceNetResource->lpProvider);
				}
				else
				{
					pCopyNetResource->lpProvider = new TCHAR[1];
					*pCopyNetResource->lpProvider = _T('\0');
				}

				// ... Add to map
				m_resourceMap.SetAt(hNewItem, pCopyNetResource);
			}

			// we need to keep track of the real number of nodes because we 
			// need it to set the correct treeitem number for the parent node
			nCurrentCount += nTempCount;
			nCurrentSkipCount += nSkipCount;

			if (hParentItem != NULL)
			{
				// Mark the parent node as expanded at least once
				VERIFY(SetItemState(hParentItem, TVIS_EXPANDEDONCE, TVIS_EXPANDEDONCE));

				// Set the number of child items to the correct value
				TV_ITEM item;
				item.hItem = hParentItem;
				item.mask = TVIF_CHILDREN;
				ASSERT(nSkipCount <= nTempCount);
				item.cChildren = nCurrentCount - nCurrentSkipCount;
				VERIFY(SetItem(&item));
			}
		}

		if (nResult2 == ERROR_MORE_DATA  || nTempCount == nCount)
			// Possibly there is more data to retrieve
		{
			nTempCount = nCount;
			continue;
		}
		else
			// There was no error and there isn't anymore data to retrieve
			break;
	}

	// Cleanup the handle and allocated memory
	VERIFY(WNetCloseEnum(hEnum) == NO_ERROR);
	GlobalFree((HGLOBAL)rgpNetResources);
	return TRUE;
}

HTREEITEM COXNetBrowseTree::InsertResourceItem(const NETRESOURCE* pNetResources, HTREEITEM hParentItem)
// --- In  : pNetResources : The net resource
//			 hParentItem : The perant node
// --- Out : 
// --- Returns : The handle of the new node
// --- Effect : Creates a new node as child of the specified parent
//				The net resource of the child is provided
{
	HTREEITEM hNewItem = NULL;
	CString sLabel;

	// Start with remote net name
	sLabel = pNetResources->lpRemoteName;
	// ... Remove leading back slashes
	while ((0 < sLabel.GetLength()) && (sLabel[0] == _T('\\')))
		sLabel = sLabel.Mid(1);
	// ... Remove everything in front of (and including) the last back slash
	int nBackSlashPos = sLabel.ReverseFind(_T('\\'));
	if (0 <= nBackSlashPos)
		sLabel = sLabel.Mid(nBackSlashPos + 1);

	// Add the comment name if necessary
	if (m_bShowCommentName)
	{
		CString sComment = pNetResources->lpComment;
		if (!sComment.IsEmpty())
			sLabel += _T(" (") + sComment + _T(")");
	}

	// Check whether the node can be further expanded
	// ... It must be a container to be expandable
	BOOL bExpandable = ((pNetResources->dwUsage & RESOURCEUSAGE_CONTAINER) == RESOURCEUSAGE_CONTAINER);
	// ... The maximum level should not be exceeded
	// ... We substract 3 because :
	//	   The level of the parent item is one less than the item that will be added here
	//	   The level number of the last item is one less than the totel number of levels
	//	   bExpandable means thet this item has further children
	bExpandable = bExpandable && (GetItemLevel(hParentItem) <= m_nMaxNumLevels - 3);

	// Add the new item to the tree control
	TV_INSERTSTRUCT insertStruct;
	memset(&insertStruct, 0, sizeof(insertStruct));

	insertStruct.hParent = hParentItem;
	insertStruct.hInsertAfter = TVI_SORT;
	insertStruct.item.mask = TVIF_CHILDREN | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	insertStruct.item.iImage = I_IMAGECALLBACK;
	insertStruct.item.iSelectedImage = I_IMAGECALLBACK;
	insertStruct.item.pszText = sLabel.GetBuffer(0);
	// ... If this item is expandable set the children count to non-zero 
	//     so that + button is shown in front of item
	//     This will be set to the correct value upon expanding
	insertStruct.item.cChildren = bExpandable ? 1 : 0;
	// ... Not yet expanded
	ASSERT((insertStruct.item.state & TVIS_EXPANDEDONCE) == 0);
	BOOL bPreInsert = PreInsertResourceItem(pNetResources, &insertStruct);
	if (bPreInsert)
		hNewItem = InsertItem(&insertStruct);
	sLabel.ReleaseBuffer();
	if (bPreInsert)
		PostInsertResourceItem(pNetResources, &insertStruct, hNewItem);

	return hNewItem;
}

void COXNetBrowseTree::Cleanup()
// --- In  :
// --- Out : 
// --- Returns :
// --- Effect : Clean up the internal structures
{
	// Clean up the association map between tree item handles and net resource infos
	POSITION pos;
	HTREEITEM hTreeItem;
	NETRESOURCE* pNetResource;
	pos = m_resourceMap.GetStartPosition();
	while (pos != NULL)
	{
		m_resourceMap.GetNextAssoc(pos, hTreeItem, pNetResource);
		// ... First delete the strings
		ASSERT(pNetResource->lpLocalName == NULL || AfxIsValidString(pNetResource->lpLocalName));
		ASSERT(pNetResource->lpRemoteName == NULL || AfxIsValidString(pNetResource->lpRemoteName));
		ASSERT(pNetResource->lpComment == NULL || AfxIsValidString(pNetResource->lpComment));
		ASSERT(pNetResource->lpProvider == NULL || AfxIsValidString(pNetResource->lpProvider));
		delete[] pNetResource->lpLocalName;
		delete[] pNetResource->lpRemoteName;
		delete[] pNetResource->lpComment;
		delete[] pNetResource->lpProvider;
		ASSERT(AfxIsValidAddress(pNetResource, sizeof(NETRESOURCE)));
		delete pNetResource;
	}
	m_resourceMap.RemoveAll();
}

void COXNetBrowseTree::ReportNetError(DWORD nResult, LPCTSTR pszResource)
{
	// ... pszResource may be NULL
	CString sNetResource(pszResource);
	CString sErrorMsg;
	sErrorMsg = GetResultMessage(nResult);
	CString sPrompt;
	AfxFormatString2(sPrompt, IDS_OX_NET_BROWSE_ERROR, sNetResource, sErrorMsg);
	if (m_bReport)
		AfxMessageBox(sPrompt, MB_ICONEXCLAMATION | MB_OK, IDS_OX_NET_BROWSE_ERROR);
	else
	{
		TRACE1("COXNetBrowseTree::ReportNetError : Not warning the user of the network error :\n\t%s",
			sPrompt);
		return;
	}
}


CString COXNetBrowseTree::GetResultMessage(HRESULT resultCode, ...)
{
	CString sResultMessage;
	BOOL bSuccess = FALSE;

	va_list args;
	va_start(args, resultCode);
	bSuccess = RetrieveResultMessage(_T(""), resultCode, &args, sResultMessage);
	va_end(args);
	if (bSuccess)
		return sResultMessage;
	else
		return _T("");
}


BOOL COXNetBrowseTree::RetrieveResultMessage(CString sModuleName, HRESULT resultCode, 
											 va_list* pArgs, CString& sResultMessage)
											 // --- In  : pszModuleName : Name of the module containing the message resource
											 //			 resultCode : Result code to use
											 //           pARgs : Optional parameters used to build the message
											 // --- Out : sResultMessage : The resulting message
											 // --- Returns : Whether the message could be retrieved successfully
											 // --- Effect : Initializes a result item object with the specified
{
	LPTSTR pszMsgBuf = NULL;
	BOOL bUnknown = FALSE;
	DWORD dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM;
	HMODULE hModule = NULL;

	// ... Get the module handle if a module name is specified
	if (!sModuleName.IsEmpty())
	{
		hModule = ::GetModuleHandle(sModuleName);
		dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE;
		if (hModule == NULL)
			TRACE(_T("COXResultItem::RetrieveResultMessage : Module '%s' not found\n"), (LPCTSTR)sModuleName);
	}

	// ... Get the actual message 
	if (::FormatMessage(dwFlags, hModule, resultCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&pszMsgBuf, 0, pArgs) == 0)
	{
		TRACE2("COXResultItem::RetrieveResultMessage : No message was found for result code %i == 0x%8.8X\n",
			resultCode, resultCode);
		//pszMsgBuf = szUnknownError;
		VERIFY(sResultMessage.LoadString(IDS_OX_NETBROWSEUNKERROR));
		bUnknown = TRUE;
	}
	else
		sResultMessage = pszMsgBuf;

	// ... Clean up
	if (!bUnknown)
		LocalFree(pszMsgBuf);

	return !bUnknown;
}

BOOL COXNetBrowseTree::PreInsertResourceItem(const NETRESOURCE* /* pNetResources */, TV_INSERTSTRUCT* /* pIinsertStruct */)
// --- In  : pNetResources : The network resource info
//			 pIinsertStruct : The insert struct that is filled out
// --- Out : pIinsertStruct : The changed insert struct
// --- Returns : Whether this item may be inserted in the tree
// --- Effect : Intercept the addition of an item in the tree
{
	// Nothing to do by default
	// ... Let the insert be completed
	return TRUE;
}

void COXNetBrowseTree::PostInsertResourceItem(const NETRESOURCE* /* pNetResources */, TV_INSERTSTRUCT* /* pInsertStruct */, HTREEITEM /* hNewItem */)
// --- In  : pNetResources : The network resource info
//			 pIinsertStruct : The insert struct that is filled out
//			 hNewItem : The handle of the newly added item
// --- Out : 
// --- Returns : 
// --- Effect : Function that i scalled after an item has been added to the tree
{
	// Nothing to do by default
}

// private:

// ==========================================================================

LRESULT COXNetBrowseTree::OnPostInit(WPARAM wParam, LPARAM lParam)
{
	// ... Parameters are not used (reserved for future extensions)
	ASSERT(wParam == 0);
	ASSERT(lParam == 0);
	UNUSED(wParam);
	UNUSED(lParam);

	// ... Window must be valid
	ASSERT(m_hWnd != NULL);
	ASSERT(::IsWindow(m_hWnd));

	// Perform first time initialization if necessary
	if (!IsInitialized())
		VERIFY(Initialize());

	return TRUE;
}

void COXNetBrowseTree::PreSubclassWindow() 
{
	// ... Window must already be valid (although it has not yet been fully subclassed)
	ASSERT(m_hWnd != NULL);
	ASSERT(::IsWindow(m_hWnd));

	PostMessage(WM_POST_INIT);

	CTreeCtrl::PreSubclassWindow();
}


int COXNetBrowseTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ... Window must already be valid (although it has not yet been fully subclassed)
	ASSERT(m_hWnd != NULL);
	ASSERT(::IsWindow(m_hWnd));

	PostMessage(WM_POST_INIT);

	return 0;
}

BOOL COXNetBrowseTree::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	BOOL bEaten = FALSE;
	*pResult = 0;

	// If expanding and not yet expanded once : expand now
	if (	(	(pNMTreeView->action == TVE_EXPAND)  || 
		((pNMTreeView->action == TVE_TOGGLE) && 
		((pNMTreeView->itemNew.state & TVIS_EXPANDED) != 0)) ) &&
		((pNMTreeView->itemNew.state & TVIS_EXPANDEDONCE) == 0) )
	{
		ExpandNode(pNMTreeView->itemNew.hItem);
	}
	return bEaten;
}

BOOL COXNetBrowseTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNREFERENCED_PARAMETER(pNMHDR);

	BOOL bEaten = FALSE;
	*pResult = 0;

	// Get the net resource belonging to the currently selected item
	HTREEITEM hTreeItem = GetSelectedItem();
	if (hTreeItem != NULL)
	{
		m_pCurrentNetResource = GetAssocNetResource(hTreeItem);
		ASSERT(m_pCurrentNetResource != NULL);
	}
	else
		m_pCurrentNetResource = NULL;
	ASSERT((m_pCurrentNetResource == NULL) || 
		(AfxIsValidAddress(m_pCurrentNetResource, sizeof(NETRESOURCE))));

	return bEaten;
}

void COXNetBrowseTree::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	*pResult = 0;

	if (((pTVDispInfo->item.mask & TVIF_IMAGE) == TVIF_IMAGE) ||
		((pTVDispInfo->item.mask & TVIF_SELECTEDIMAGE) == TVIF_SELECTEDIMAGE))
	{
		// ... Start with unknown type
		int nImage = NET_IMAGE_UNKNOWN;
		// Get the associated net resource
		const NETRESOURCE* pNetResource = NULL;
		pNetResource = GetAssocNetResource(pTVDispInfo->item.hItem);
		if (pNetResource == NULL)
			return;

		switch(pNetResource->dwDisplayType)
		{
		case RESOURCEDISPLAYTYPE_GENERIC:
			nImage = NET_IMAGE_UNKNOWN;
			break;
		case RESOURCEDISPLAYTYPE_DOMAIN:
			nImage = NET_IMAGE_DOMAIN;
			break;
		case RESOURCEDISPLAYTYPE_SERVER:
			nImage = NET_IMAGE_SERVER;
			break;
		case RESOURCEDISPLAYTYPE_SHARE:
			{
				switch(pNetResource->dwType)
				{
				case RESOURCETYPE_DISK:
					nImage = NET_IMAGE_DISK_SHARE;
					break;
				case RESOURCETYPE_PRINT:
					nImage = NET_IMAGE_PRINTER_SHARE;
					break;
				default:
					nImage = NET_IMAGE_OTHER_SHARE;
					break;
				}
			}
			break;
		case RESOURCEDISPLAYTYPE_FILE:
			nImage = NET_IMAGE_FILE;
			break;
		case RESOURCEDISPLAYTYPE_GROUP:
			nImage = NET_IMAGE_GROUP;
			break;
		case RESOURCEDISPLAYTYPE_NETWORK:
			nImage = NET_IMAGE_NETWORK;
			break;
		case RESOURCEDISPLAYTYPE_ROOT:
			nImage = NET_IMAGE_ROOT;
			break;
		case RESOURCEDISPLAYTYPE_SHAREADMIN:
			nImage = NET_IMAGE_SHARADMIN;
			break;
		case RESOURCEDISPLAYTYPE_DIRECTORY:
			nImage = NET_IMAGE_DIRECTORY;
			break;
		case RESOURCEDISPLAYTYPE_TREE:
			nImage = NET_IMAGE_TREE;
			break;
		}

		// Under Windows NT 3.51 The root node of RESOURCE_GLOBALNET tree
		// has display type RESOURCEDISPLAYTYPE_GENERIC instead of RESOURCEDISPLAYTYPE_NETWORK
		// We will adjust for this here
		if ((OSVersion.dwMajorVersion < 4) &&
			(pNetResource->dwScope == RESOURCE_GLOBALNET) &&
			(GetParentItem(pTVDispInfo->item.hItem) == NULL))
		{
			nImage = NET_IMAGE_NETWORK;
		}

		// If the item is expanded, use the next image
		if ((GetItemState(pTVDispInfo->item.hItem, TVIS_EXPANDED) & TVIS_EXPANDED) == TVIS_EXPANDED)
			nImage++;

		// Return the result
		pTVDispInfo->item.iImage = nImage;
		pTVDispInfo->item.iSelectedImage = nImage;
	}
}


void COXNetBrowseTree::PostNcDestroy() 
{
	// Unitialize the control
	Uninitialize();

	CTreeCtrl::PostNcDestroy();
}

