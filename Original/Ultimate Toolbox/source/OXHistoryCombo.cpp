// ==========================================================================
//				Class Implementation : COXHistoryCombo
// ==========================================================================

// Source file : OXHistoryCombo.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXHistoryCombo.h"
#include "commctrl.h"	// For toolbar text handler
#include <afxpriv.h>	// for WM_IDLEUPDATECMDUI
#include "UTB64Bit.h"

#include "UTBStrOp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// determine number of elements in an array (not bytes)
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif // _countof

static const TCHAR szSoftware[] =		_T("Software");
static const TCHAR szHistoryCombo[] =	_T("HistoryCombo");

IMPLEMENT_DYNAMIC(COXHistoryCombo, CComboBox)

/////////////////////////////////////////////////////////////////////////////
// Definition of static members
COXHistoryCombo::EToolbarPosition COXHistoryCombo::TBP_FIRST = COXHistoryCombo::TBPNone;
COXHistoryCombo::EToolbarPosition COXHistoryCombo::TBP_LAST = COXHistoryCombo::TBPHorizontalRightBottom;

// Data members -------------------------------------------------------------
// protected:
	// EToolbarPosition m_eToolbarPosition;
	// --- The current position of the toolbar seen from this control

	// BOOL m_bUseGap;
	// --- Whether a gap should be shown between the combo box and the toolbar

	// int m_nMaxHistoryCount;
	// --- Maximum number of entries allowed in the list of the combo box

	// CToolBar* m_pToolbar;
	// --- The toolbar window
	//     This window iss a sibling of this control, but this control is its owner
	//     This way it receives important messages from the toolbar

	// BOOL m_rgbShowToolButton[OX_HISTORY_COMBO_MAX_TOOLBUTTONS];
	// --- An array with one entry for each button
	//     Whether the button is marked as visible or not

	// LPCTSTR m_lpszToolbarResource;
	// --- Resource name of the toolbar
	
	// BOOL m_bAutoPersistent;
	// --- Whether auto persistent mode is on (restore on creation, save on destruction)

	// CString m_sAutoPersistentCompany;
	// --- Company name for auto persistent

	// CString m_sAutoPersistentApplication;
	// --- Application name for auto persistent

	// CString m_sAutoPersistentValueName;
	// --- Value name for auto persistent

	// struct CFileOpenParams m_fileOpenParams;
	// --- Variable containing the information needed to popup a file open dialog


// private:
	
// Member functions ---------------------------------------------------------
// public:


BEGIN_MESSAGE_MAP(COXHistoryCombo, CComboBox)
	//{{AFX_MSG_MAP(COXHistoryCombo)
	ON_WM_CREATE()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_DESTROY()		
	ON_COMMAND(ID_OX_HISTORY_COMBO_NEW, OnNew)
	ON_UPDATE_COMMAND_UI(ID_OX_HISTORY_COMBO_NEW, OnUpdateNew)
	ON_COMMAND(ID_OX_HISTORY_COMBO_DELETE, OnDelete)
	ON_UPDATE_COMMAND_UI(ID_OX_HISTORY_COMBO_DELETE, OnUpdateDelete)
	ON_COMMAND(ID_OX_HISTORY_COMBO_BROWSE, OnBrowse)
	ON_UPDATE_COMMAND_UI(ID_OX_HISTORY_COMBO_BROWSE, OnUpdateBrowse)
	ON_COMMAND(ID_OX_HISTORY_COMBO_RESERVED_1, OnReserved1)
	ON_UPDATE_COMMAND_UI(ID_OX_HISTORY_COMBO_RESERVED_1, OnUpdateReserved1)
	ON_COMMAND(ID_OX_HISTORY_COMBO_RESERVED_2, OnReserved2)
	ON_UPDATE_COMMAND_UI(ID_OX_HISTORY_COMBO_RESERVED_2, OnUpdateReserved2)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipText)
	ON_COMMAND_EX_RANGE(0, 0xFFFF, OnToolbarCommand)
	ON_UPDATE_COMMAND_UI_RANGE(0, 0xFFFF, OnToolbarUpdateUI)
END_MESSAGE_MAP()

COXHistoryCombo::COXHistoryCombo(EToolbarPosition eToolbarPosition /* = TBPHorizontalRightCenter */,
	BOOL bUseGap /* = FALSE */, LPCTSTR lpszToolbarResource /* = MAKEINTRESOURCE(IDR_HISTORY_COMBO_TOOLS) */)
	:
	m_eToolbarPosition(eToolbarPosition),
	m_bUseGap(bUseGap),
	m_nMaxHistoryCount(6),
	m_pToolbar(NULL),
	m_lpszToolbarResource(NULL),
	m_bAutoPersistent(FALSE)
{
	// ... eToolbarPosition must be valid
	ASSERT(TBP_FIRST <= eToolbarPosition);
	ASSERT(eToolbarPosition <= TBP_LAST);

	// Initialize the visibility of the buttons
	// ... Set the non-reserved buttons to visible by default
	for (int nButtonIndex = 0; nButtonIndex < OX_HISTORY_COMBO_MAX_TOOLBUTTONS; nButtonIndex++)
	{
		// ... Hide the reserved buttons (	ID_HISTORY_COMBO_RESERVED_1 and 
		//									ID_HISTORY_COMBO_RESERVED_2	)
		m_rgbShowToolButton[nButtonIndex] = (nButtonIndex<3);
	}
	ASSERT(5 <= OX_HISTORY_COMBO_MAX_TOOLBUTTONS);
	m_rgbShowToolButton[3] = FALSE;
	m_rgbShowToolButton[4] = FALSE;

	// ... Construct the toolbar (should exist at all times)
	m_pToolbar = new CToolBar();

	// Set the toolbar resource
	SetToolbarResource(lpszToolbarResource);

	ASSERT_VALID(this);
}

COXHistoryCombo::~COXHistoryCombo()
{
	ASSERT_VALID(this);

	if(::IsWindow(GetSafeHwnd()))
	{
		DestroyWindow();
	}

	// ... Toolbar should have been constructed
	ASSERT(m_pToolbar!=NULL);
	if(::IsWindow(m_pToolbar->GetSafeHwnd()))
	{
		m_pToolbar->DestroyWindow();
	}

	// We destruct the toolbar  now
	delete m_pToolbar;
	m_pToolbar = NULL;
}


void COXHistoryCombo::
SetToolbarResource(UINT nIDToolbarResource/*=IDR_HISTORY_COMBO_TOOLS*/)
{
	SetToolbarResource(MAKEINTRESOURCE(nIDToolbarResource));
}

void COXHistoryCombo::
SetToolbarResource(LPCTSTR lpszToolbarResource/*=MAKEINTRESOURCE(IDR_HISTORY_COMBO_TOOLS)*/)
{
#ifdef _DEBUG
	if (AfxFindResourceHandle(lpszToolbarResource, RT_TOOLBAR) == NULL)
	{
		TRACE0("COXHistoryCombo::SetToolbarResource : Specified toolbar resource not found\n");
		// ... Must find the resource
		//     (If you are using the default IDR_HISTORY_COMBO_TOOLS, 
		//      make sure OXHistoryCombo.rc is included in your resource file)
		ASSERT(FALSE);
	}
#endif // _DEBUG
	m_lpszToolbarResource = lpszToolbarResource;

	ASSERT_VALID(this);
}

void COXHistoryCombo::InitCombo(CWnd* pParentWnd /* = NULL */)
{
	ASSERT_VALID(this);

#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	// Create a toolbar as sibling of this control
	// ... Invisible by default, we will show it when the correct dimensions are knwon
	//     in PositionToolbar
	// ... Get the parent window if none was specified
	if (pParentWnd == NULL)
	{
		pParentWnd = GetParent();
	}
	ASSERT(pParentWnd != NULL);

	// ... Toolbar should have been constructed, but not created yet
	ASSERT(m_pToolbar!=NULL);
	ASSERT(m_pToolbar->m_hWnd==NULL);
	VERIFY(m_pToolbar->Create(pParentWnd, WS_CHILD | CBRS_TOOLTIPS));

	m_pToolbar->SetWindowPos( &wndTop, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE );
	SetWindowPos( &wndTop, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE );

	// Set the recipient for notifications messages to this window
	// (This will not become the actual owner : see also COXHistoryCombo::OnDestroy)
	m_pToolbar->SetOwner(this);
	
	RefreshToolbar();

#ifdef _DEBUG
	// Check the styles of the control
	if ((GetStyle() & CBS_AUTOHSCROLL) != CBS_AUTOHSCROLL)
		TRACE0("COXHistoryCombo::InitCombo : Control does not have the CBS_AUTOHSCROLL style, text may be truncated\n");
	if ((GetStyle() & CBS_SORT) == CBS_SORT)
		TRACE0("COXHistoryCombo::InitCombo : Control has the CBS_SORT style, new items will not be added at the top\n");
#endif // _DEBUG

	// Start auto persistence if necessary
	if (m_bAutoPersistent)
	{
		RestoreContents(m_sAutoPersistentValueName, m_sAutoPersistentCompany, 
			m_sAutoPersistentApplication);
	}

	ASSERT_VALID(this);
}

void COXHistoryCombo::PositionToolbar(EToolbarPosition eToolbarPosition/*=TBPHorizontalRightCenter*/,
									  BOOL bUseGap/*=TRUE*/)
{
	ASSERT(TBP_FIRST <= eToolbarPosition);
	ASSERT(eToolbarPosition <= TBP_LAST);
	ASSERT_VALID(this);

	// Store new setting
	m_bUseGap = bUseGap;
	m_eToolbarPosition  = eToolbarPosition;

	int nCount=GetVisibleButtonCount();
	m_pToolbar->ShowWindow(
		(nCount == 0 || eToolbarPosition == TBPNone) ? SW_HIDE : SW_SHOW);
	if (nCount == 0 || eToolbarPosition == TBPNone)
	{
		// Invisible toolbar : nothing more to do
		ASSERT_VALID(this);
		return;
	}

	CRect rect;
	CRect toolRect(0,0,0,0);
	CSize toolSize;
	CSize offset;
	GetWindowRect(rect);
	GetParent()->ScreenToClient(rect);

	// ... Change bar style to reflect horizontal orientation
	DWORD nOldStyle = m_pToolbar->GetBarStyle();
	m_pToolbar->SetBarStyle(nOldStyle | CBRS_ORIENT_HORZ);
	// ... Get the bar size
	toolSize = m_pToolbar->CalcFixedLayout(FALSE /* bStretch */, TRUE /* bHorz */);
	toolRect.right = toolRect.left + toolSize.cx;
	toolRect.bottom = toolRect.top + toolSize.cy;

	// Then adjust the position of the toolbar
	// ... Get the border size of the toolbar 
	int cxToolbarLeftBorder = m_pToolbar->m_cxLeftBorder;
	int cxToolbarRightBorder = m_pToolbar->m_cxRightBorder;
	int cyToolbarTopBorder = m_pToolbar->m_cyTopBorder;
	int cyToolbarBottomBorder = m_pToolbar->m_cyBottomBorder;
	int nBorderFactor = m_bUseGap ? 0 : 1;

	// ... Handle all possible cases
	switch (eToolbarPosition)
	{
	case TBPHorizontalTopLeft:
		// ... Top (Horizontal)
		offset.cy = rect.top - toolRect.Height() + cyToolbarTopBorder * nBorderFactor;
		// ... Left (Horizontal)
		offset.cx = rect.left - cxToolbarLeftBorder;
		break;
	case TBPHorizontalTopCenter:
		// ... Top (Horizontal)
		offset.cy = rect.top - toolRect.Height() + cyToolbarTopBorder * nBorderFactor;
		// ... Center (Horizontal)
		offset.cx = rect.left + (rect.Width() - toolRect.Width()) / 2;
		break;
	case TBPHorizontalTopRight:
		// ... Top (Horizontal)
		offset.cy = rect.top - toolRect.Height() + cyToolbarTopBorder * nBorderFactor;
		// ... Right (Horizontal)
		offset.cx = rect.right - toolRect.Width() + cxToolbarRightBorder;
		break;
	case TBPHorizontalBottomLeft:
		// ... Bottom (Horizontal)
		offset.cy = rect.bottom - cyToolbarBottomBorder * nBorderFactor;
		// ... Left (Horizontal)
		offset.cx = rect.left - cxToolbarLeftBorder;
		break;
	case TBPHorizontalBottomCenter:
		// ... Bottom (Horizontal)
		offset.cy = rect.bottom - cyToolbarBottomBorder * nBorderFactor;
		// ... Center (Horizontal)
		offset.cx = rect.left + (rect.Width() - toolRect.Width()) / 2;
		break;
	case TBPHorizontalBottomRight:
		// ... Bottom (Horizontal)
		offset.cy = rect.bottom - cyToolbarBottomBorder * nBorderFactor;
		// ... Right (Horizontal)
		offset.cx = rect.right - toolRect.Width() + cxToolbarRightBorder;
		break;

	case TBPHorizontalLeftTop:
		// ... Top (Horizontal)
		offset.cy = rect.top - cyToolbarTopBorder;
		// ... Left (Horizontal)
		offset.cx = rect.left - toolRect.Width() + cxToolbarLeftBorder * nBorderFactor;
		break;
	case TBPHorizontalLeftCenter:
		// ... Center (Horizontal)
		offset.cy = rect.top + (rect.Height() - toolRect.Height()) / 2;
		// ... Left (Horizontal)
		offset.cx = rect.left - toolRect.Width() + cxToolbarLeftBorder * nBorderFactor;
		break;
	case TBPHorizontalLeftBottom:
		// ... Bottom (Horizontal)
		offset.cy = rect.top + (rect.Height() - toolRect.Height()) + cyToolbarBottomBorder;
		// ... Left (Horizontal)
		offset.cx = rect.left - toolRect.Width() + cxToolbarLeftBorder * nBorderFactor;
		break;
	case TBPHorizontalRightTop:
		// ... Top (Horizontal)
		offset.cy = rect.top  - cyToolbarTopBorder;
		// ... Right (Horizontal)
		offset.cx = rect.right - cxToolbarRightBorder * nBorderFactor;
		break;
	case TBPHorizontalRightCenter:
		// ... Center (Horizontal)
		offset.cy = rect.top + (rect.Height() - toolRect.Height()) / 2;
		// ... Right (Horizontal)
		offset.cx = rect.right - cxToolbarRightBorder * nBorderFactor;
		break;
	case TBPHorizontalRightBottom:
		// ... Bottom (Horizontal)
		offset.cy = rect.top + (rect.Height() - toolRect.Height())  + cyToolbarBottomBorder;
		// ... Right (Horizontal)
		offset.cx = rect.right - cxToolbarRightBorder * nBorderFactor;
		break;
	default:
		TRACE1("COXHistoryCombo::PositionToolbar : Unexpected case in switch (%i)\n", eToolbarPosition);
		ASSERT(FALSE);
		break;
	}
	toolRect += offset;

	// Reposition the toolbar
	m_pToolbar->MoveWindow(toolRect);
	// ... If already visible : redraw
	m_pToolbar->Invalidate();

	ASSERT_VALID(this);
}

COXHistoryCombo::EToolbarPosition COXHistoryCombo::GetToolbarPosition() const
{
	ASSERT_VALID(this);
	return m_eToolbarPosition;
}

void COXHistoryCombo::ShowButton(int nButtonIndex, BOOL bShow /* = FALSE */)
{
	ASSERT_VALID(this);
	ASSERT(nButtonIndex < OX_HISTORY_COMBO_MAX_TOOLBUTTONS);

	m_rgbShowToolButton[nButtonIndex] = bShow;

	ASSERT_VALID(this);
}

BOOL COXHistoryCombo::IsButtonShown(int nButtonIndex) const
{
	ASSERT_VALID(this);
	ASSERT(nButtonIndex < OX_HISTORY_COMBO_MAX_TOOLBUTTONS);

	return m_rgbShowToolButton[nButtonIndex];
}

void COXHistoryCombo::SetMaxHistoryCount(int nMaxHistoryCount)
{
	ASSERT_VALID(this);

	m_nMaxHistoryCount = nMaxHistoryCount;

	// If the list contains more items than allowed, truncate the list
	// ... Cast to DWORD so that -1 becomes a very large number
	if ((DWORD)m_nMaxHistoryCount < (DWORD)GetCount())
	{
		for (int nItemIndex = GetCount() - 1; m_nMaxHistoryCount <= nItemIndex; nItemIndex--)
		{
			DeleteString(nItemIndex);
		}
	}

	ASSERT_VALID(this);
}

int COXHistoryCombo::GetMaxHistoryCount() const
{
	ASSERT_VALID(this);
	return m_nMaxHistoryCount;
}

void COXHistoryCombo::RefreshToolbar()
{
	ASSERT_VALID(this);

	// First reload the toolbar
	ASSERT(m_pToolbar != NULL);
	ASSERT(m_pToolbar->m_hWnd != NULL);
	VERIFY(m_pToolbar->LoadToolBar(m_lpszToolbarResource));

	// Remove buttons that are marked as invisible
	RemoveButtons();

	// Reposition the toolbar (extra buttons may have become visible)
	PositionToolbar(GetToolbarPosition());

	ASSERT_VALID(this);
}

BOOL COXHistoryCombo::AddNewItem(LPCTSTR pszItemText /* = NULL */)
{
	ASSERT_VALID(this);
	CString sNewItem(pszItemText);

	if (sNewItem.IsEmpty())
		// ... No text was specified, get the text from the edit control
		GetWindowText(sNewItem);

	if (sNewItem.IsEmpty())
		// ... Do not add empty string to the list
		return FALSE;

	// Check whether the item is already in the list or not
	BOOL bAlreadyInList = FALSE;
	int nOldPos = FindStringExact(-1, sNewItem);
	if (nOldPos != CB_ERR)
	{
		// ... Already in the list, remove the old item
		DeleteString(nOldPos);
		bAlreadyInList = TRUE;
	}

	// ... Insert the new string at the top of the list and select it
	InsertString(0, sNewItem);
	SetCurSel(0);

	// If the list contains more items than allowed, truncate the list
	// ... Cast to DWORD so that -1 becomes a very large number
	if ((DWORD)m_nMaxHistoryCount < (DWORD)GetCount())
	{
		for (int nItemIndex = GetCount() - 1; m_nMaxHistoryCount <= nItemIndex; nItemIndex--)
		{
			DeleteString(nItemIndex);
		}
	}
	// ... Select the entire string
	SetEditSel(0, -1);

	ASSERT_VALID(this);
	return !bAlreadyInList;
}

BOOL COXHistoryCombo::DeleteItem(int nItemIndex /* = -1 */)
{
	ASSERT_VALID(this);

	if (nItemIndex == -1)
		// ... No index was specified, get the current selection
		nItemIndex = GetCurSel();

	BOOL bSuccess = (DeleteString(nItemIndex) != CB_ERR);

	if (bSuccess)
	{
		// Select the next item if it exists
		if (nItemIndex < GetCount())
		{
			// ... Items have shifted up, select the same index
			SetCurSel(nItemIndex);

			CWnd* pParentWnd=GetParent();
			if(pParentWnd!=NULL)
			{
				pParentWnd->SendMessage(WM_COMMAND,
					MAKEWPARAM(GetDlgCtrlID(),CBN_SELCHANGE),(LPARAM)GetSafeHwnd());
			}
		}
		else
			// ... Last item was deleted, select the current last one (may not exist if the list is empty)
			SetCurSel(nItemIndex - 1);
	}
	return bSuccess;
}

BOOL COXHistoryCombo::BrowseItem()
{
	ASSERT_VALID(this);

	CFileDialog fileDlg(
		m_fileOpenParams.m_bOpenFileDialog, 
		m_fileOpenParams.m_sDefExt.IsEmpty() ? NULL : (LPCTSTR)m_fileOpenParams.m_sDefExt,
		m_fileOpenParams.m_sFileName.IsEmpty() ? NULL : (LPCTSTR)m_fileOpenParams.m_sFileName,
		m_fileOpenParams.m_dwFlags,
		m_fileOpenParams.m_sFilter.IsEmpty() ? NULL : (LPCTSTR)m_fileOpenParams.m_sFilter,
		m_fileOpenParams.m_pParentWnd);

	//added 11/12/1999
	fileDlg.m_ofn.lpstrInitialDir=
		m_fileOpenParams.m_sStartDir.IsEmpty() ? NULL : (LPCTSTR) m_fileOpenParams.m_sStartDir;

	BOOL bBrowseOK = FALSE;
	if (fileDlg.DoModal() == IDOK)
	{
		SetWindowText(fileDlg.GetPathName());

		CWnd* pParentWnd=GetParent();
		if(pParentWnd!=NULL)
		{
			pParentWnd->SendMessage(WM_COMMAND,
				MAKEWPARAM(GetDlgCtrlID(),CBN_EDITCHANGE),(LPARAM)GetSafeHwnd());
		}
		
		bBrowseOK = TRUE;
	}

	ASSERT_VALID(this);
	return bBrowseOK;
}


BOOL COXHistoryCombo::CanAddNewItem() const
{
	ASSERT_VALID(this);
	// ... Can add if text in edit control is not empty
	return (GetWindowTextLength() != 0);
}

BOOL COXHistoryCombo::CanDeleteItem() const
{
	ASSERT_VALID(this);
	// ... Can only delete if one is selected
	return GetCurSel() != CB_ERR;
}

BOOL COXHistoryCombo::CanBrowseItem() const
{
	ASSERT_VALID(this);
	// ... Can always browse by default
	return TRUE;
}

void COXHistoryCombo::SetFileDialogParams(BOOL bOpenFileDialog /* = TRUE */, 
										  LPCTSTR pszDefExt /* = NULL */,	
										  LPCTSTR pszFileName /* = NULL */, 
										  DWORD dwFlags /* = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT */, 
										  LPCTSTR pszFilter /* = NULL */, 
										  CWnd* pParentWnd /* = NULL */,
					  LPCTSTR lpszInitDir/*=NULL*/)
{
	ASSERT_VALID(this);

	m_fileOpenParams.m_bOpenFileDialog = bOpenFileDialog;
	m_fileOpenParams.m_sDefExt = pszDefExt;
	m_fileOpenParams.m_sFileName = pszFileName;
	m_fileOpenParams.m_dwFlags = dwFlags;
	m_fileOpenParams.m_sFilter = pszFilter;
	m_fileOpenParams.m_pParentWnd = pParentWnd;
	m_fileOpenParams.m_sStartDir=lpszInitDir;//added 11/12/1999

	ASSERT_VALID(this);
}

BOOL COXHistoryCombo::OnIdle(LONG lCount /* = 0 */)
{
	// Update the state of the GUI now
	// ... We only have to update the first time the idle loop is executed
	if(lCount==0)
	{
		SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE /* bDisableIfNoHandler */, 0);
	}

	// ... We do not need more idle processing
	return FALSE;
}

BOOL COXHistoryCombo::SetAutoPersistent(LPCTSTR pszValueName, 
										LPCTSTR pszCompany /* = NULL */, 
										LPCTSTR pszApplication /* = NULL */)
{
	ASSERT_VALID(this);

	BOOL bSuccess = TRUE;
	if (m_hWnd != NULL)
	{
		// Control is already created, restore persistent values now
		bSuccess = RestoreContents(pszValueName, pszCompany, pszApplication);
	}
	if (bSuccess)
	{
		// Only set the new values if the previous save succeeded
		m_bAutoPersistent = TRUE;
		m_sAutoPersistentCompany = pszCompany;
		m_sAutoPersistentApplication = pszApplication;
		m_sAutoPersistentValueName = pszValueName;
	}
	else
	{
		TRACE0("COXHistoryCombo::SetAutoPersistent : Failed to save the current contents, failing\n");
	}

	ASSERT_VALID(this);

	return bSuccess;
}

void COXHistoryCombo::RemoveAutoPersistent()
{
	ASSERT_VALID(this);

	m_bAutoPersistent = FALSE;

	ASSERT_VALID(this);
}

BOOL COXHistoryCombo::SaveContents(LPCTSTR pszValueName, 
								   LPCTSTR pszCompany /* = NULL */, 
								   LPCTSTR pszApplication /* = NULL*/ , 
								   HKEY hKeyRoot /* = HKEY_CURRENT_USER */, 
								   BOOL bAddNewItem /* = TRUE */)
{
	ASSERT_VALID(this);

	CString sValueName(pszValueName);
	CString sCompany(pszCompany);
	CString sApplication(pszApplication);
	CString sContents;

	if (sValueName.IsEmpty())
	{
		TRACE0("COXHistoryCombo::SaveContents : No value name provided, failing\n");
		return FALSE;
	}

	if (sCompany.IsEmpty())
		sCompany = AfxGetApp()->m_pszRegistryKey;
	if (sCompany.IsEmpty())
	{
		TRACE0("COXHistoryCombo::SaveContents : No valid company name is provided, failing\n");
		return FALSE;
	}

	if (sApplication.IsEmpty())
		sApplication = AfxGetApp()->m_pszProfileName;

	// Add the current contents of the dit control to the list (if not yet one)
	if (bAddNewItem)
		AddNewItem();

	// Get the contents from the list
	sContents = GetContents();

	BOOL bSuccess = SaveContentsToRegistry(hKeyRoot, sCompany, sApplication, sValueName, sContents);

	ASSERT_VALID(this);
	return bSuccess;
}

BOOL COXHistoryCombo::RestoreContents(LPCTSTR pszValueName, 
									  LPCTSTR pszCompany /* = NULL */, 
									  LPCTSTR pszApplication /* = NULL*/ , 
									  HKEY hKeyRoot /* = HKEY_CURRENT_USER */, 
									  BOOL bRestoreFirstItem /* = TRUE */)
{
	ASSERT_VALID(this);

	CString sValueName(pszValueName);
	CString sCompany(pszCompany);
	CString sApplication(pszApplication);
	CString sContents;

	if (sValueName.IsEmpty())
	{
		TRACE0("COXHistoryCombo::SaveContents : No value name provided, failing\n");
		return FALSE;
	}

	if (sCompany.IsEmpty())
		sCompany = AfxGetApp()->m_pszRegistryKey;
	if (sCompany.IsEmpty())
	{
		TRACE0("COXHistoryCombo::SaveContents : No valid company name is provided, failing\n");
		return FALSE;
	}

	if (sApplication.IsEmpty())
		sApplication = AfxGetApp()->m_pszProfileName;

	BOOL bSuccess = LoadContentsFromRegistry(hKeyRoot, sCompany, sApplication, sValueName, sContents);
	if (bSuccess)
	{
		// ... Set the contents in the list box
		SetContents(sContents);

		// Select the first item from the list
		if (bRestoreFirstItem)
		{
			SetCurSel(0);

			if (::IsWindow(GetSafeHwnd())
				&& GetFocus()==this && ::IsWindowEnabled(m_hWnd))
			SetEditSel(0, -1);
		}
	}

	ASSERT_VALID(this);
	return bSuccess;
}

#ifdef _DEBUG
void COXHistoryCombo::AssertValid() const
{
	CComboBox::AssertValid();

	// The toolbar object should exist at all times
	ASSERT(m_pToolbar != NULL);
}

void COXHistoryCombo::Dump(CDumpContext& dc) const
{
	CComboBox::Dump(dc);
	dc << "\nTBP_FIRST : " << (int)TBP_FIRST;
	dc << "\nTBP_LAST : " << (int)TBP_LAST;
	dc << "\nm_eToolbarPosition : " << (int)m_eToolbarPosition;
	dc << "\nm_bUseGap : " << m_bUseGap;
	dc << "\nm_nMaxHistoryCount : " << m_nMaxHistoryCount;
	dc << "\nm_pToolbar : " << m_pToolbar;

	dc << "\nm_rgbShowToolButton : ";
	for (int nIndex = 0; nIndex < OX_HISTORY_COMBO_MAX_TOOLBUTTONS; nIndex++)
		dc << "\n[" << nIndex << "] : " << m_rgbShowToolButton[nIndex];

	dc << "\nm_lpszToolbarResource : " << m_lpszToolbarResource;
	dc << "\nm_bAutoPersistent : " << m_bAutoPersistent;
	dc << "\nm_sAutoPersistentCompany : " << m_sAutoPersistentCompany;
	dc << "\nm_sAutoPersistentApplication : " << m_sAutoPersistentApplication;
	dc << "\nm_sAutoPersistentValueName : " << m_sAutoPersistentValueName;
	dc << "\nm_fileOpenParams : " << &m_fileOpenParams;
}
#endif //_DEBUG

BOOL COXHistoryCombo::IsFrameWnd() const
{
	// Mimic a frame window to get the idle Update CmdUI messages from the toolbar
	return TRUE;
}

void COXHistoryCombo::RemoveButtons()
{
	ASSERT_VALID(this);
	ASSERT(AfxIsValidAddress(m_rgbShowToolButton, 
		sizeof(BOOL)*OX_HISTORY_COMBO_MAX_TOOLBUTTONS));

	// ... Toolbar should have been constructed
	ASSERT(m_pToolbar != NULL);

	// Compose a new button list
	int nCount=m_pToolbar->GetCount();
	ASSERT(OX_HISTORY_COMBO_MAX_TOOLBUTTONS>=nCount);

	int nNewCount = 0;
	UINT* rgID = new UINT[nCount];
	UINT* rgStyle = new UINT[nCount];
	int* rgImage = new int[nCount];

	// Iterate all the existing buttons and keep those we need
	for (int nIndex=0; nIndex<nCount; nIndex++)
	{
		if(m_rgbShowToolButton[nIndex])
		{
			ASSERT(nNewCount<nCount);
			m_pToolbar->GetButtonInfo(nIndex, rgID[nNewCount], 
				rgStyle[nNewCount], rgImage[nNewCount]);
			nNewCount++;
		}
	}

	// Set the new buttons
	// ... We will only show the toolbar if at least one button is visible
	m_pToolbar->ShowWindow((nNewCount == 0) ? SW_HIDE : SW_SHOW);
	if (nNewCount != 0)
	{
		VERIFY(m_pToolbar->SetButtons(NULL, nNewCount));
		for (int nNewIndex = 0; nNewIndex < nNewCount; nNewIndex++)
		{
			m_pToolbar->SetButtonInfo(nNewIndex, rgID[nNewIndex], 
				rgStyle[nNewIndex], rgImage[nNewIndex]);
		}
	}

	//Clean up arrays
	delete[] rgID;
	delete[] rgStyle;
	delete[] rgImage;

	ASSERT_VALID(this);
}

CString COXHistoryCombo::GetContents() const
{
	ASSERT_VALID(this);

	CString sContents;
	CString sLBItem;
	const int nItemCount = GetCount();
	for (int nItemIndex = 0; nItemIndex < nItemCount; nItemIndex++)
	{
		GetLBText(nItemIndex, sLBItem);
		if (!sContents.IsEmpty())
			sContents += ITEM_SEPERATOR;
		sContents += sLBItem;
	}

	ASSERT_VALID(this);
	return sContents;
}

void COXHistoryCombo::SetContents(LPCTSTR pszContents)
{
	ASSERT_VALID(this);

	CString sContents(pszContents);

	// First remove everuthing from the combo
	ResetContent();

	// Then add all new the items
	LPTSTR pszItem = sContents.GetBuffer(0);
	DWORD nIndex = 0;
	TCHAR * p;
	pszItem =  UTBStr::tcstok(pszItem, ITEM_SEPERATOR, &p);

	// ... Cast to DWORD so that -1 becomes a very large number
	while ((nIndex < (DWORD)m_nMaxHistoryCount) && (pszItem != NULL))
	{
		AddString(pszItem);
		pszItem = UTBStr::tcstok(NULL, ITEM_SEPERATOR, &p);
	}

	sContents.ReleaseBuffer();
	ASSERT_VALID(this);
}

BOOL COXHistoryCombo::SaveContentsToRegistry(HKEY hKeyRoot, LPCTSTR pszCompany, 
											 LPCTSTR pszApplication,
											 LPCTSTR pszValueName, LPCTSTR pszValue)
{
	ASSERT(hKeyRoot != NULL);
	ASSERT((pszCompany != NULL) && (*pszCompany != _T('\0')));
	ASSERT((pszApplication != NULL) && (*pszApplication != _T('\0')));
	ASSERT((pszValueName != NULL) && (*pszValueName != _T('\0')));
	// ... Value may be empty string
	ASSERT((pszValue != NULL));

	// Open key for hKeyRoot\<szSoftware>\<pszCompany>\<pszApplication>\<szHistoryCombo>
	HKEY hSoftwareKey = NULL;
	HKEY hCompanyKey = NULL;
	HKEY hApplicationKey = NULL;
	HKEY hHistoryKey = NULL;
	if (::RegOpenKeyEx(hKeyRoot, szSoftware, 0, KEY_WRITE | KEY_READ,
		&hSoftwareKey) == ERROR_SUCCESS)
	{
		DWORD dw;
		if (::RegCreateKeyEx(hSoftwareKey, pszCompany, 0, REG_NONE,
			REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL,
			&hCompanyKey, &dw) == ERROR_SUCCESS)
		{
			if (::RegCreateKeyEx(hCompanyKey, pszApplication, 0, REG_NONE,
				REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL,
				&hApplicationKey, &dw) == ERROR_SUCCESS)
			{
				::RegCreateKeyEx(hApplicationKey, szHistoryCombo, 0, REG_NONE,
					REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ, NULL,
					&hHistoryKey, &dw);
			}
		}
	}
	if (hSoftwareKey != NULL)
		::RegCloseKey(hSoftwareKey);
	if (hCompanyKey != NULL)
		::RegCloseKey(hCompanyKey);
	if (hApplicationKey != NULL)
		::RegCloseKey(hApplicationKey);

	if (hHistoryKey == NULL)
	{
		TRACE0("COXHistoryCombo::SaveContentsToRegistry : Failed to open history key\n");
		return FALSE;
	}

	// Set value
	LONG nResult;
	if (*pszValue == _T('\0'))
		// ... Necessary to cast away const 
		nResult = ::RegDeleteValue(hHistoryKey, (LPTSTR)pszValueName);
	else
		nResult = RegSetValueEx(hHistoryKey, pszValueName, NULL, REG_SZ,
			(LPBYTE)pszValue, PtrToLong((_tcslen(pszValue) + 1) * sizeof(TCHAR)));
	::RegCloseKey(hHistoryKey);

#ifdef _DEBUG
	if (nResult != ERROR_SUCCESS)
		TRACE0("COXHistoryCombo::SaveContentsToRegistry : Failed to set value\n");
#endif // _DEBUG

	return (nResult == ERROR_SUCCESS);
}

BOOL COXHistoryCombo::LoadContentsFromRegistry(HKEY hKeyRoot, 
											   LPCTSTR pszCompany, 
											   LPCTSTR pszApplication,
											   LPCTSTR pszValueName, 
											   CString& sValue)
{
	ASSERT(hKeyRoot != NULL);
	ASSERT((pszCompany != NULL) && (*pszCompany != _T('\0')));
	ASSERT((pszApplication != NULL) && (*pszApplication != _T('\0')));
	ASSERT((pszValueName != NULL) && (*pszValueName != _T('\0')));

	// Open key for hKeyRoot\<szSoftware>\<pszCompany>\<pszApplication>\<szHistoryCombo>
	HKEY hSoftwareKey = NULL;
	HKEY hCompanyKey = NULL;
	HKEY hApplicationKey = NULL;
	HKEY hHistoryKey = NULL;
	if (::RegOpenKeyEx(hKeyRoot, szSoftware, 0, KEY_READ,
		&hSoftwareKey) == ERROR_SUCCESS)
	{
		if (::RegOpenKeyEx(hSoftwareKey, pszCompany, 0, KEY_READ, 
			&hCompanyKey) == ERROR_SUCCESS)
		{
			if (::RegOpenKeyEx(hCompanyKey, pszApplication, 0, KEY_READ,
				&hApplicationKey) == ERROR_SUCCESS)
			{
				::RegOpenKeyEx(hApplicationKey,szHistoryCombo,0,KEY_READ,&hHistoryKey);
			}
		}
	}
	if (hSoftwareKey != NULL)
		::RegCloseKey(hSoftwareKey);
	if (hCompanyKey != NULL)
		::RegCloseKey(hCompanyKey);
	if (hApplicationKey != NULL)
		::RegCloseKey(hApplicationKey);

	if (hHistoryKey == NULL)
	{
		// Failed to open history key, return default (empty string)
		sValue.Empty();
		return TRUE;
	}

	// Get value
	DWORD dwType, dwCount;
	LONG nResult = ::RegQueryValueEx(hHistoryKey, (LPTSTR)pszValueName, NULL, &dwType,
		NULL, &dwCount);
	if (nResult == ERROR_SUCCESS)
	{
		ASSERT(dwType == REG_SZ);
		nResult = ::RegQueryValueEx(hHistoryKey, (LPTSTR)pszValueName, NULL, &dwType,
			(LPBYTE)sValue.GetBuffer(dwCount / sizeof(TCHAR)), &dwCount);
		sValue.ReleaseBuffer();
		ASSERT(dwType == REG_SZ);
	}
	::RegCloseKey(hHistoryKey);

	if (nResult != ERROR_SUCCESS)
		// ... Could not get value, use default (empty string)
		sValue.Empty();

	return TRUE;
}

void COXHistoryCombo::PreSubclassWindow() 
{
	ASSERT_VALID(this);

	// ... Attached window must be valid
	ASSERT(m_hWnd != NULL);
	ASSERT(::IsWindow(m_hWnd));
	
	_AFX_THREAD_STATE* pThreadState=AfxGetThreadState();
	// hook not already in progress
	if(pThreadState->m_pWndInit==NULL)
	{
		if(GetParent() != NULL)
		{
			// Initialize this control
			InitCombo();
		}
	}

	// If GetParent() == NULL, this control is being created through Create()
	// and it is still to ealy to initialize, this will be done in OnCreate
	
	CComboBox::PreSubclassWindow();

	ASSERT_VALID(this);
}

int COXHistoryCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	ASSERT_VALID(this);

	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// ... Window must be valid
	ASSERT(m_hWnd != NULL);
	ASSERT(::IsWindow(m_hWnd));

	// Initialize this control
	// ... Pass on the window that will become the parent (GetParent() returns NULL at this point)
	InitCombo(CWnd::FromHandle(lpCreateStruct->hwndParent));

	ASSERT_VALID(this);
	return 0;
}

LRESULT COXHistoryCombo::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	ASSERT_VALID(this);

	// ... Toolbar should have been constructed
	ASSERT(m_pToolbar != NULL);

	// Pass on to the toolbar
	m_pToolbar->SendMessage(WM_IDLEUPDATECMDUI, wParam, lParam);

	return 0L;
}

BOOL COXHistoryCombo::OnToolTipText(UINT /* nControlID */, NMHDR* pNMHDR, 
									LRESULT* pResult)
{
	ASSERT_VALID(this);
	// This code is almost the same as in CFrameWnd::OnToolTipText
	// see WinFrm.cpp
	ASSERT(pNMHDR->code == TTN_NEEDTEXT);

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*) pNMHDR;
	TCHAR szFullText[256];
	CString strTipText;
	UINT_PTR nID = pNMHDR->idFrom;
	if (pNMHDR->code == TTN_NEEDTEXT && (pTTT->uFlags & TTF_IDISHWND))
	{
		// ... idFrom is actually the HWND of the tool
		nID = ((UINT)(WORD)::GetDlgCtrlID((HWND)nID));
	}

	// ... nID will be zero on a separator
	if (nID != 0) 
	{
		// don't handle the message if no string resource found
		if(AfxLoadString(PtrToUint(nID),szFullText)==0)
		{
			return FALSE;
		}
		// ... this is the command id, not the button index
		AfxExtractSubString(strTipText, szFullText, 1, '\n');
	}

	UTBStr::tcscpy(pTTT->szText, 80, strTipText);

	// Bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
		SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE);

	// Message was handled
	// ... Message does not use result
	*pResult = 0;
	return TRUE;    
}


void COXHistoryCombo::OnDestroy() 
{
	ASSERT_VALID(this);
	CComboBox::OnDestroy();
	
	// ... Toolbar should have been constructed
	ASSERT(m_pToolbar != NULL);

	// Note : 
	// A child window is confined to its parent window's client area.
	// A window is automatically destroyed when its owner is destroyed
	// Windows does not provide a method to change a window's owner.
	// See also MSDN : Window Owners and Parents (PSS ID Number: Q84190)

	// The toolbar is a window created with the WS_CHILD style, so does not have 
	// an explicit owner; it is implicitly owned by its parent

	// Calling CToolbar::SetOwner will only send the TB_SETPARENT message and set the
	// CWnd data member m_hWndOwner

	// So the actual owner of the toolbar is still the parent window,
	// and thus the toolbar is only destroyed when the parent window is destroyed

	// We will explicitely destroy the toolbar here
	m_pToolbar->DestroyWindow();

	// When the toolbar is destroyed but not destructed MFC does not reset the 
	// protected data member CToolbar::m_hbmImageWell
	// Because of this all subsequent LoadToolBar calls will fail
	// To work around we actually destruct the toolbar and construct a new one.
	delete m_pToolbar;
	m_pToolbar = new CToolBar();

	// Start auto persistence if necessary
	if (m_bAutoPersistent)
	{
		SaveContents(m_sAutoPersistentValueName,m_sAutoPersistentCompany, 
			m_sAutoPersistentApplication);
	}

	ASSERT_VALID(this);
}

BOOL COXHistoryCombo::OnToolbarCommand(UINT nCommand)
{
	ASSERT_VALID(this);

	// All WM_COMMANDs which we do not handle ourselves arrive here

	// Pass this command to our parent
	WPARAM wParam = (WPARAM)nCommand;
	LPARAM lParam = (LPARAM)m_pToolbar->m_hWnd;
	return (BOOL)GetParent()->SendMessage(WM_COMMAND, wParam, lParam);
}

void COXHistoryCombo::OnToolbarUpdateUI(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);

	// All ON_UPDATE_COMMAND_UIs which we do not handle ourselves arrive here
	// Pass this command to our parent
	pCmdUI->DoUpdate(GetParent(), TRUE/* bDisableIfNoHndler */);
}


void COXHistoryCombo::OnNew() 
{
	// Add the string to the list box and clear the string
	AddNewItem();
}

void COXHistoryCombo::OnUpdateNew(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsWindowEnabled()&CanAddNewItem());
}

void COXHistoryCombo::OnDelete() 
{
	// Delete the item
	DeleteItem();
}

void COXHistoryCombo::OnUpdateDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsWindowEnabled()&CanDeleteItem());
}

void COXHistoryCombo::OnBrowse() 
{
	// Open the file oppen dialog
	BrowseItem();
}

void COXHistoryCombo::OnUpdateBrowse(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsWindowEnabled()&CanBrowseItem());
}

void COXHistoryCombo::OnReserved1() 
{
	// This function should not be used (yet)
	ASSERT(FALSE);
}

void COXHistoryCombo::OnUpdateReserved1(CCmdUI* pCmdUI) 
{
	// Disable reserved commands by default
	pCmdUI->Enable(FALSE);
}

void COXHistoryCombo::OnReserved2() 
{
	// This function should not be used (yet)
	ASSERT(FALSE);
}

void COXHistoryCombo::OnUpdateReserved2(CCmdUI* pCmdUI) 
{
	// Disable reserved commands by default
	pCmdUI->Enable(FALSE);
}

void COXHistoryCombo::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	if(::IsWindow(GetToolBar()->GetSafeHwnd()))
	{
		PositionToolbar(m_eToolbarPosition);
	}
	CComboBox::OnWindowPosChanged(lpwndpos);
}

void COXHistoryCombo::OnEnable(BOOL bEnable)
{
	CComboBox::OnEnable(bEnable);
	SetEditSel(0,0);
	ASSERT(m_pToolbar!=NULL);
	m_pToolbar->EnableWindow(bEnable);
}

int COXHistoryCombo::GetVisibleButtonCount()
{
	ASSERT(m_pToolbar!=NULL);
	int nCount=m_pToolbar->GetCount();
	ASSERT(OX_HISTORY_COMBO_MAX_TOOLBUTTONS>=nCount);

	int nNewCount=0;
	// Iterate all the existing buttons
	for (int nIndex=0; nIndex<OX_HISTORY_COMBO_MAX_TOOLBUTTONS; nIndex++)
	{
		if(m_rgbShowToolButton[nIndex])
		{
			nNewCount++;
		}
	}

	return nNewCount;
}

