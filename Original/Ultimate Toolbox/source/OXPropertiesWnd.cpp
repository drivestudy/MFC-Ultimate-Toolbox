// ==========================================================================
// 							Class Implementation : COXPropertiesWnd
// ==========================================================================

// Source file : OXPropertiesWnd.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.   

#include "stdafx.h"
#include "OXPropertiesWnd.h"
#include "OXListEdit.h"
#include "OXMaskedEdit.h"
#include "OXSkins.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4355)

/////////////////////////////////////////////////////////////////////////////
// COXPropertiesWnd

COXPropertiesWnd::COXPropertiesWnd() :
	m_wndTree(this)
{
	m_bSortCategories = TRUE;
	m_bCategorized = TRUE;
	m_hCurrentProp = NULL;
	m_pEditWnd = NULL;
	m_hEditProp = NULL;
	m_pFrameSkin = NULL;
}

COXPropertiesWnd::~COXPropertiesWnd()
{
	m_mapProp2Cat.RemoveAll();
	m_mapProp2Desc.RemoveAll();
	m_mapProp2Editor.RemoveAll();
	m_listCategories.RemoveAll();
}


BEGIN_MESSAGE_MAP(COXPropertiesWnd, CFrameWnd)
	//{{AFX_MSG_MAP(COXPropertiesWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()	
	ON_COMMAND(ID_CATEGORIZED, OnCategorized)
	ON_COMMAND(ID_ALPHABETIC, OnAlphabetic)
	ON_WM_PAINT()
	ON_UPDATE_COMMAND_UI(ID_CATEGORIZED, OnUpdateCategorized)
	ON_UPDATE_COMMAND_UI(ID_ALPHABETIC, OnUpdateAlphabetic)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, _TREE_ID, OnItemChanged)
	ON_MESSAGE(WM_USER_QUERY_PROPERTIESWND, OnQueryPropertiesWnd)
	ON_MESSAGE(WM_USER_STARTEDITING, OnStartEditing)
	ON_MESSAGE(WM_USER_ADJUSTLASTCOLUMN, OnUserAdjustLastColumn)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COXPropertiesWnd message handlers

int COXPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	// Create the combo
	if (!m_wndCombo.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, CRect(0, 0, 0, 300), this, _COMBO_ID))
	{
		TRACE0("Failed to create the combo box.\n");
		return -1;      // fail to create
	}

	// Create the toolbar
	if (!m_wndToolBar.Create(this,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,
		IDR_PROPERTIES_TOOLBAR) || !m_wndToolBar.LoadToolBar(IDR_PROPERTIES_TOOLBAR))
	{
		TRACE0("Failed to create toolbar.\n");
		return -1;      // fail to create
	}
	m_wndToolBar.SetOwner(this);

	// Create the category list
	if (!m_wndTree.Create(WS_CHILD | WS_VISIBLE | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS,
		CRect(0, 0, 0, 0), this, _TREE_ID,
		TVOXS_VGRID | TVOXS_HGRID | TVOXS_COLUMNHDR | TVOXS_NOSORTHEADER | TVOXS_NOFOCUSRECT |
		TVOXS_EXTENDCOLUMNS | TVOXS_ROWSEL | TVOXS_FLGRID))
	{
		TRACE0("Failed to create the category list.\n");
		return -1;      // fail to create
	}
	
	// Create the description pane
	// Create the combo
	if (!m_wndDescription.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, _DESCRIPTION_ID))
	{
		TRACE0("Failed to create the description pane.\n");
		return -1;      // fail to create
	}

	m_wndTree.SetHorizontalGridColor(::GetSysColor(COLOR_3DFACE));
	m_wndTree.SetVerticalGridColor(::GetSysColor(COLOR_3DFACE));

	// Set up the two columns
	LV_COLUMN lvc;
	memset(&lvc, 0, sizeof(lvc));
	lvc.fmt = LVCFMT_LEFT;
	lvc.cx = 100;
	lvc.pszText = _T("Property");
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	m_wndTree.SetColumn(0,&lvc);
	m_wndTree.InsertColumn(1, _T("Value"), LVCFMT_LEFT, 100, 1);

	m_wndTree.m_wndEdit.SetBorder(FALSE);

	return 0;
}

void COXPropertiesWnd::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);

	CRect rectClient;
	GetClientRect(rectClient);

	m_wndCombo.MoveWindow(rectClient);
	
	// Get the height of the combo box
	CRect rectCombo;
	m_wndCombo.GetClientRect(rectCombo);

	if (!(m_wndCombo.GetStyle() & WS_VISIBLE))
		rectCombo.bottom = rectCombo.top;

	// Position the toolbar below the combo box
	CRect rectToolBar = rectClient;
	rectToolBar.top += rectCombo.Height();
	rectToolBar.bottom = rectToolBar.top + 25;
	m_wndToolBar.MoveWindow(rectToolBar);

	if (!(m_wndToolBar.GetStyle() & WS_VISIBLE))
		rectToolBar.bottom = rectToolBar.top;

	// Position the description pane at the bottom
	CRect rectDescription = rectClient;
	rectDescription.top = rectDescription.bottom - 50;
	m_wndDescription.MoveWindow(rectDescription);

	if (!(m_wndDescription.GetStyle() & WS_VISIBLE))
		rectDescription.bottom = rectDescription.top;

	// Position the tree in the space that remains
	CRect rectTree = rectClient;
	rectTree.top += rectCombo.Height() + rectToolBar.Height();
	rectTree.bottom -= rectDescription.Height();
	rectTree.DeflateRect(1, 1);
	m_wndTree.MoveWindow(rectTree);

	// If an item is being edited reposition the editor
	if (m_pEditWnd != NULL)
	{
		HPROPERTY hSelectedProp = GetSelectedProperty();
		CRect rectSubItem;
		int iItemIndex = m_wndTree.GetItemIndex(hSelectedProp);
		m_wndTree.GetSubItemRect(iItemIndex, 1, LVIR_LABEL, rectSubItem);
		rectSubItem.DeflateRect(0, 0, 1, 1);

		m_pEditWnd->MoveWindow(rectSubItem);
	}
}

void COXPropertiesWnd::OnCategorized() 
{
	if (m_bCategorized)
		return;

	m_bCategorized = TRUE;
	m_wndTree.ModifyStyle(0, TVS_LINESATROOT);


	// Insert the categories
	POSITION catpos = m_listCategories.GetHeadPosition();
	for(int i=0; i<m_listCategories.GetCount(); i++)
	{
		InsertCategory(m_listCategories.GetNext(catpos));
	}


	// Re-insert all properties under their categories
	POSITION pos = m_mapProp2Cat.GetStartPosition();
	while (pos != NULL)
	{
		HPROPERTY hProp;
		CString strCategory;
		m_mapProp2Cat.GetNextAssoc(pos, hProp, strCategory);		

		// Insert the category if it hasn't been inserted yet
		HCATEGORY hCategory = m_wndTree.GetRootItem();
		if (strCategory.IsEmpty() == FALSE)
		{
			hCategory= FindCategory(strCategory);
			if (hCategory == NULL)
				hCategory = InsertCategory(strCategory);
		}

		if (hCategory != m_wndTree.GetRootItem())
		{
			m_wndTree.MoveItem(hProp, hCategory);
			m_wndTree.SortChildren(hCategory);
		}
	}

	// Sort
	if(m_bSortCategories)
		m_wndTree.SortChildren(m_wndTree.GetRootItem());
}

void COXPropertiesWnd::OnAlphabetic() 
{
	if (!m_bCategorized)
		return;

	m_bCategorized = FALSE;
	m_wndTree.ModifyStyle(TVS_LINESATROOT, 0);

	// Remove all categories from the tree and insert all items directly under the root item

	// If a root item is not in m_mapProp2Cat when it needs to be removed
	HTREEITEM hRoot = m_wndTree.GetRootItem();
	if (m_wndTree.ItemHasChildren(hRoot))
	{
		HCATEGORY hNextCat, hCat = m_wndTree.GetChildItem(hRoot);
		while (hCat != NULL)
		{
			CString strDummy;
			hNextCat = m_wndTree.GetNextItem(hCat, TVGN_NEXT);
			if (!m_mapProp2Cat.Lookup(hCat, strDummy))
			{
				// This is a category, so move all its children to the root and delete it
				HPROPERTY hNextProp, hProp = m_wndTree.GetChildItem(hCat);
				while (hProp != NULL)
				{
					hNextProp = m_wndTree.GetNextItem(hProp, TVGN_NEXT);
					m_wndTree.MoveItem(hProp, hRoot);
					hProp = hNextProp;
				}

				m_wndTree.DeleteItem(hCat);
			}

			hCat = hNextCat;
		}
	}

	// Now sort all items under the root
	m_wndTree.SortChildren(hRoot);
}

HCATEGORY COXPropertiesWnd::InsertCategory(LPCTSTR lpszCategoryName)
{
	HCATEGORY hCategory = m_wndTree.InsertItem(lpszCategoryName);
	m_wndTree.SetItemBackColor(hCategory, ::GetSysColor(COLOR_3DFACE));
	m_wndTree.SetSubItem(hCategory, 1, OX_SUBITEM_BGCOLOR, NULL, 0, NULL, 0, ::GetSysColor(COLOR_3DFACE));
	CStringArray saDummy;
	m_wndTree.SetEditMode(hCategory, OXET_NOEDIT, saDummy, 0);	
	m_wndTree.SetEditMode(hCategory, OXET_NOEDIT, saDummy, 1);	
	
	m_wndTree.Expand(hCategory, TVE_EXPAND);
	
	return hCategory;
}

HCATEGORY COXPropertiesWnd::FindCategory(LPCTSTR lpszCategory)
{
	HTREEITEM hRoot = m_wndTree.GetRootItem();
	if (m_wndTree.ItemHasChildren(hRoot))
	{
		HTREEITEM hNextItem, hChildItem = m_wndTree.GetChildItem(hRoot);
		while (hChildItem != NULL)
		{
			hNextItem = m_wndTree.GetNextItem(hChildItem, TVGN_NEXT);
			if (m_wndTree.GetItemText(hChildItem) == lpszCategory)
				return hChildItem;

			hChildItem = hNextItem;
		}
	}
	return NULL;
}

HPROPERTY COXPropertiesWnd::InsertProperty(LPCTSTR lpszName,
										   LPCTSTR lpszInitialValue,
										   LPCTSTR lpszDescription,
										   LPCTSTR lpszCategory,
										   CEdit* pEditorClass)
{
	if (pEditorClass != NULL)
	{
		ASSERT(pEditorClass->IsKindOf(RUNTIME_CLASS(CWnd)));
	}


	HCATEGORY hCategory = NULL;
	if (lpszCategory != NULL)
	{
		hCategory= FindCategory(lpszCategory);
		if (hCategory == NULL)
		{
			hCategory = InsertCategory(lpszCategory);
			CString tmpStr = lpszCategory;
			m_listCategories.AddTail(tmpStr);
		}
	}

	HPROPERTY hProp = m_wndTree.InsertItem(lpszName, hCategory);
	if (hProp == NULL)
		return NULL;

	// Insert the sub item
	m_wndTree.SetSubItem(hProp, 1, 0, NULL);

	// Set up the editing mode to none
	CStringArray saDummy;
	m_wndTree.SetEditMode(hProp, OXET_NOEDIT, saDummy, 0);
	m_wndTree.SetEditMode(hProp, OXET_NOEDIT, saDummy, 1);

	// Sort
	if (hCategory != NULL)
		m_wndTree.SortChildren(hCategory);
	else
	{
		if(m_bSortCategories)
			m_wndTree.SortChildren(m_wndTree.GetRootItem());
		else
			m_wndTree.MoveItem(hProp,m_wndTree.GetRootItem(),TVI_FIRST);
	}

	// Insert an entry in the property to category map
	CString strCategory = _T("");
	if (hCategory != NULL)
		strCategory = m_wndTree.GetItemText(hCategory);
	m_mapProp2Cat.SetAt(hProp, strCategory);

	// Insert an entry in the property to description map
	m_mapProp2Desc.SetAt(hProp, lpszDescription);

	// Insert an entry to the editor map
	if (pEditorClass != NULL)
		m_mapProp2Editor.SetAt(hProp, pEditorClass);

	// Set the initial value (if any)
	if (lpszInitialValue != NULL)
		SetPropertyValue(hProp, lpszInitialValue);

	return hProp;
}

BOOL COXPropertiesWnd::DeleteProperty(HPROPERTY hProperty)
{
	ASSERT(hProperty != NULL);

	FinishEditing();

	if (!OnDeleteProperty(hProperty))
		return FALSE;

	HTREEITEM hParent = m_wndTree.GetParentItem(hProperty);

	// Remove the property handle from the maps
	m_mapProp2Cat.RemoveKey(hProperty);
	m_mapProp2Desc.RemoveKey(hProperty);
	m_mapProp2Editor.RemoveKey(hProperty);

	BOOL bRes = m_wndTree.DeleteItem((HTREEITEM) hProperty);
	if (hParent != m_wndTree.GetRootItem())
	{
		// The parent is a category
		COXTreeItem* pParent = m_wndTree.GetXItem(hParent);
		if (pParent->GetChildrenCount() == 0)
		{
			CString strCategory = m_wndTree.GetItemText(hParent);
			if(POSITION pos = m_listCategories.Find(strCategory))
			{
				m_listCategories.RemoveAt(pos);
			}
			m_wndTree.DeleteItem(hParent); // delete the empty category
		}
	}

	return bRes;
}

BOOL COXPropertiesWnd::SetPropertyValue(HPROPERTY hProperty, LPCTSTR lpszValue)
{
	ASSERT(hProperty != NULL);
	return m_wndTree.SetItemText(hProperty, lpszValue, 1);
}

CString COXPropertiesWnd::GetPropertyValue(HPROPERTY hProperty)
{
	ASSERT(hProperty != NULL);
	return m_wndTree.GetItemText(hProperty, 1);
}

HPROPERTY COXPropertiesWnd::FindProperty(LPCTSTR lpszName, LPCTSTR lpszCategory)
{
	// First try to find the category
	HCATEGORY hCategory = NULL;
	if (lpszCategory != NULL)
		hCategory = FindCategory(lpszCategory);

	HTREEITEM hParent = hCategory;
	if (hParent == NULL)
		hParent = m_wndTree.GetRootItem();
	
	// At this point we know that if the property exist it is a child of hParent
	if (m_wndTree.ItemHasChildren(hParent))
	{
		HTREEITEM hNextItem, hChildItem = m_wndTree.GetChildItem(hParent);
		while (hChildItem != NULL)
		{
			hNextItem = m_wndTree.GetNextItem(hChildItem, TVGN_NEXT);
			if (m_wndTree.GetItemText(hChildItem) == lpszName)
				return hChildItem;

			hChildItem = hNextItem;
		}
	}
	return NULL;
}

void COXPropertiesWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectClient;
	GetClientRect(rectClient);
	dc.FillSolidRect(rectClient, GetFrameSkin()->GetBackgroundColor());
	CBrush brBackground;
	brBackground.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
	dc.FrameRect(rectClient, &brBackground);

	// Draw a gray border around the tree
	CRect rectTree;
	m_wndTree.GetWindowRect(rectTree);
	ScreenToClient(rectTree);
	rectTree.InflateRect(1, 1);
	CBrush brBorder;
	brBorder.CreateSolidBrush(GetFrameSkin()->GetBorderColor());
	dc.FrameRect(rectTree, &brBorder);
}

void COXPropertiesWnd::PostNcDestroy() 
{
	// DO NOTHING
}

void COXPropertiesWnd::OnUpdateCategorized(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bCategorized);	
}

void COXPropertiesWnd::OnUpdateAlphabetic(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!m_bCategorized);	
}

void COXPropertiesWnd::SetCategorized(BOOL bCategorized)
{
	m_bCategorized = bCategorized;

	if (bCategorized)
		OnCategorized();
	else
		OnAlphabetic();
}

CComboBoxEx& COXPropertiesWnd::GetComboBoxEx()
{
	return m_wndCombo;
}

COXCoolToolBar& COXPropertiesWnd::GetToolBar()
{
	return m_wndToolBar;
}

void COXPropertiesWnd::ShowCombo(BOOL bShow)
{
	if (bShow)
		m_wndCombo.ModifyStyle(0, WS_VISIBLE);
	else
		m_wndCombo.ModifyStyle(WS_VISIBLE, 0);
}

void COXPropertiesWnd::ShowToolBar(BOOL bShow)
{
	if (bShow)
		m_wndToolBar.ModifyStyle(0, WS_VISIBLE);
	else
		m_wndToolBar.ModifyStyle(WS_VISIBLE, 0);
}

void COXPropertiesWnd::ShowDescription(BOOL bShow)
{
	if (bShow)
		m_wndDescription.ModifyStyle(0, WS_VISIBLE);
	else
		m_wndDescription.ModifyStyle(WS_VISIBLE, 0);
}

HPROPERTY COXPropertiesWnd::GetSelectedProperty()
{
	HTREEITEM hSelected = m_wndTree.GetNextItem(TVI_ROOT, TVGN_FIRSTSELECTED);
	
	if (hSelected == NULL)
		return NULL; // no property is selected
	
	CString strDummy;
	if (m_mapProp2Cat.Lookup(hSelected, strDummy))
		return hSelected; // this is indeed a property;
	else
		return NULL; // it is a category
}

BOOL COXPropertiesWnd::OnPropertyValueChanged(HPROPERTY /*hProperty*/, LPCTSTR /*lpszOldValue*/, LPCTSTR /*lpszNewValue*/)
{
	return TRUE; // accept the new value
}

void COXPropertiesWnd::OnItemChanged(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/) 
{
	CString strName, strDescription;
	HPROPERTY hSelectedProp = GetSelectedProperty();
	if (hSelectedProp == NULL)
	{
		strName = _T("");
		strDescription = _T("");
	}
	else
	{
		strName = m_wndTree.GetItemText(hSelectedProp);
		m_mapProp2Desc.Lookup(hSelectedProp, strDescription);

		SendMessage(WM_USER_STARTEDITING, 0, (LPARAM) hSelectedProp);
	}

	m_wndDescription.SetDescription(strName, strDescription);
}

LRESULT COXPropertiesWnd::OnQueryPropertiesWnd(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return TRUE; // yes, this is a COXPropertiesWnd
}

LRESULT COXPropertiesWnd::OnStartEditing(WPARAM /*wParam*/, LPARAM lParam)
{
	HPROPERTY hSelectedProp = (HPROPERTY) lParam;

	// Finish editing first
	if (m_pEditWnd != NULL)
		FinishEditing();
	else
		StartEditing(hSelectedProp);

	return 0;
}

void COXPropertiesWnd::StartEditing(HPROPERTY hSelectedProp)
{
	// Create the editing control and place it over the sub-item
	CEdit* pEditor;
	BOOL bFound = m_mapProp2Editor.Lookup(hSelectedProp, pEditor);
	if (bFound)
	{
		m_pEditWnd = pEditor;

		CRect rectSubItem;
		int iItemIndex = m_wndTree.GetItemIndex(hSelectedProp);
		m_wndTree.GetSubItemRect(iItemIndex, 1, LVIR_LABEL, rectSubItem);
		rectSubItem.DeflateRect(0, 0, 1, 1);

		// Create the editor window if not created already
		if (!::IsWindow(m_pEditWnd->m_hWnd))
			m_pEditWnd->Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, rectSubItem, &m_wndTree, _EDITOR_ID);
		else
		{
			m_pEditWnd->MoveWindow(rectSubItem);
			m_pEditWnd->ShowWindow(SW_SHOW);
		}
		m_pEditWnd->SetOwner(&m_wndTree);

		m_pEditWnd->SetFont(m_wndTree.GetFont());

		// Set the text of the editing window
		CString strText = GetPropertyValue(hSelectedProp);
		
		COXEdit* pOXEdit = DYNAMIC_DOWNCAST(COXEdit, m_pEditWnd);
		COXMaskedEdit* pOXMaskedEdit = DYNAMIC_DOWNCAST(COXMaskedEdit, m_pEditWnd);
		if (pOXEdit != NULL)
		{
			pOXEdit->EmptyData();
			pOXEdit->SetInputData(strText);
		}
		else if (pOXMaskedEdit != NULL)
			pOXMaskedEdit->SetText(strText);
		else
			m_pEditWnd->SetWindowText(strText);

		m_hEditProp = hSelectedProp;
	}
}

void COXPropertiesWnd::FinishEditing(BOOL bSave)
{
	if (m_hEditProp == NULL) {
		return;
	}
	if (bSave)
	{
		// Call OnPropertyValueChanged(...) and save the value
		CString strText;
		COXMaskedEdit* pMaskedEdit = DYNAMIC_DOWNCAST(COXMaskedEdit, m_pEditWnd);
		if (pMaskedEdit != NULL)
			strText = pMaskedEdit->GetText();
		else
			m_pEditWnd->GetWindowText(strText);
		
		BOOL bSaveValue = OnPropertyValueChanged(m_hEditProp,
			GetPropertyValue(m_hEditProp), strText);
		
		if (bSaveValue)
			SetPropertyValue(m_hEditProp, strText);
	}

	// Hide the editor
	m_pEditWnd->ShowWindow(SW_HIDE);
	m_pEditWnd = NULL;
	m_hEditProp = NULL;

	m_wndTree.SelectItem(NULL);
}

BOOL COXPropertiesWnd::PreTranslateMessage(MSG* pMsg) 
{
	return CFrameWnd::PreTranslateMessage(pMsg);
}

LRESULT COXPropertiesWnd::OnUserAdjustLastColumn(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// If an item is being edited reposition the editor
	if (m_pEditWnd != NULL)
	{
		HPROPERTY hSelectedProp = GetSelectedProperty();
		CRect rectSubItem;
		int iItemIndex = m_wndTree.GetItemIndex(hSelectedProp);
		m_wndTree.GetSubItemRect(iItemIndex, 1, LVIR_LABEL, rectSubItem);
		rectSubItem.DeflateRect(0, 0, 1, 1);

		m_pEditWnd->MoveWindow(rectSubItem);
	}

	return 0;
}

BOOL COXPropertiesWnd::IsDescendant(CWnd* pWndParent, CWnd* pWndChild)
	// helper for detecting whether child descendent of parent
	//  (works with owned popups as well)
{
	ASSERT(pWndParent!=NULL);
	ASSERT(::IsWindow(pWndParent->GetSafeHwnd()));
	ASSERT(pWndChild!=NULL);
	ASSERT(::IsWindow(pWndChild->GetSafeHwnd()));

	HWND hWndParent=pWndParent->GetSafeHwnd();
	HWND hWndChild=pWndChild->GetSafeHwnd();
	do
	{
		if (hWndParent == hWndChild)
			return TRUE;

		// check for permanent-owned window first
		CWnd* pWnd=CWnd::FromHandlePermanent(hWndChild);
		if(pWnd!=NULL)
			hWndChild=pWnd->GetOwner()->GetSafeHwnd();
		else
			hWndChild=(::GetWindowLongPtr(hWndChild,GWL_STYLE)&WS_CHILD) ?
				::GetParent(hWndChild) : ::GetWindow(hWndChild,GW_OWNER);

	} while (hWndChild != NULL);

	return FALSE;
}

BOOL COXPropertiesWnd::OnDeleteProperty(HPROPERTY /*hProperty*/)
{
	return TRUE;
}

BOOL COXPropertiesWnd::DeleteAll()
{
	// Loop through all the properties using the m_mapProp2Cat map

	CArray<HPROPERTY, HPROPERTY> arrToBeDeleted;

	POSITION pos = m_mapProp2Cat.GetStartPosition();
	while (pos != NULL)
	{
		HPROPERTY hProperty;
		CString strCategory;
		m_mapProp2Cat.GetNextAssoc(pos, hProperty, strCategory);
		arrToBeDeleted.Add(hProperty);
	}

	BOOL bAllDeleted = TRUE;
	for (int i = 0; i < arrToBeDeleted.GetSize(); i++)
		if (!DeleteProperty(arrToBeDeleted[i]))
			bAllDeleted = FALSE;

	return bAllDeleted;
}

COXFrameSkin* COXPropertiesWnd::GetFrameSkin()
{
	// Check if the app is derived from COXSkinnedApp
	COXSkinnedApp* pSkinnedApp = DYNAMIC_DOWNCAST(COXSkinnedApp, AfxGetApp());
	if (pSkinnedApp != NULL && pSkinnedApp->GetCurrentSkin() != NULL)
		return pSkinnedApp->GetCurrentSkin()->GetFrameSkin();
	else
	{
		// Create a classic skin for this class if not created already
		if (m_pFrameSkin == NULL)
			m_pFrameSkin = new COXFrameSkinClassic();

		return m_pFrameSkin;
	}
}

void COXPropertiesWnd::SetSortCategories(BOOL bSortCategories /*= TRUE*/ )
{
	m_bSortCategories = bSortCategories;
}


/*
	Used to jump to the next property (if it's the end of the current 
	category, the first property in the next category gets selected).
	Nish - June 14, 2005
*/

BOOL COXPropertiesWnd::SelectNextProperty()
{
	HTREEITEM hSelected = m_wndTree.GetNextItem(TVI_ROOT, TVGN_FIRSTSELECTED);

	if (hSelected == NULL)
		return FALSE; // no property is selected	

	HTREEITEM hNext = m_wndTree.GetNextItem(hSelected, TVGN_CHILD);

	if (hNext == NULL)
		hNext = m_wndTree.GetNextItem(hSelected, TVGN_NEXT);	

	if (hNext == NULL)
		hNext = m_wndTree.GetNextItem(m_wndTree.GetParentItem(hSelected), TVGN_NEXT);

	if (hNext == NULL)
		return FALSE; // no more properties		

	FinishEditing(FALSE);
	m_wndTree.SelectItem(hNext);			
	m_wndTree.SetItemState(hNext,LVIS_SELECTED, LVIS_SELECTED);
	StartEditing(hNext);		

	if(m_wndTree.ItemHasChildren(hNext))
		return SelectNextProperty();

	return TRUE;	
}

