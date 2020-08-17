// ==========================================================================
//			Class Implementation: COXCustomizeWorkspaceStatePage
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXCustomizeWorkspaceStatePage.h"
#include "OXCustomizeManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT IDC_CHECK_PAGE_BASE=2000;

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeWorkspaceStatePage dialog

IMPLEMENT_DYNCREATE(COXCustomizeWorkspaceStatePage, COXCustomizePage)

COXCustomizeWorkspaceStatePage::COXCustomizeWorkspaceStatePage()
{
	//{{AFX_DATA_INIT(COXCustomizeWorkspaceStatePage)
	m_bIncludeBars = FALSE;
	m_bIncludeMainWnd = FALSE;
	m_bIncludeChildFrames = FALSE;
	m_bIncludeSplitterPanes = FALSE;
	//}}AFX_DATA_INIT

	m_nDialogID=IDD;
	m_sProfileName.Empty();
}


COXCustomizeWorkspaceStatePage::~COXCustomizeWorkspaceStatePage()
{
}


BOOL COXCustomizeWorkspaceStatePage::OnCloseManager(BOOL bIsOk) 
{	
	if(bIsOk)
	{
		if(!m_sProfileName.IsEmpty())
		{
#ifndef _MAC
			CWinApp* pApp=AfxGetApp();
			ASSERT(pApp!=NULL);

			if(!UpdateData(TRUE))
				return FALSE;

			pApp->WriteProfileInt(m_sProfileName,
				_T("IncludeBars"),m_bIncludeBars);
			pApp->WriteProfileInt(m_sProfileName,
				_T("IncludeMainWnd"),m_bIncludeMainWnd);
			pApp->WriteProfileInt(m_sProfileName,
				_T("IncludeChildFrames"),m_bIncludeChildFrames);
			pApp->WriteProfileInt(m_sProfileName,
				_T("IncludeSplitterPanes"),m_bIncludeSplitterPanes);

			// save info about customize pages viewability 
			POSITION pos=m_mapPageNames.GetStartPosition();
			int nIndex=0;
			while(pos!=NULL)
			{
				CString sPageKey(_T(""));
				BOOL bViewable=FALSE;
				m_mapPageNames.GetNextAssoc(pos,sPageKey,bViewable);
				ASSERT(!sPageKey.IsEmpty());

				CString sProfileKey;
				sProfileKey.Format(_T("%d_CustomizePageKey"),nIndex);
				pApp->WriteProfileString(m_sProfileName,sProfileKey,sPageKey);
				
				pApp->WriteProfileInt(m_sProfileName,sPageKey,bViewable);

				nIndex++;
			}
#endif
		}
	}
	else
	{
		GetPagesInfo();
	}

	return TRUE;
}


BOOL COXCustomizeWorkspaceStatePage::
LoadWorkspaceState(BOOL bIncludeMainWnd/*=TRUE*/,
				   BOOL bIncludeBars/*=TRUE*/,
				   BOOL bIncludeChildFrames/*=TRUE*/,
				   BOOL bIncludeSplitterPanes/*=TRUE*/,
				   LPCTSTR lpszProfileName/*=_T("CustomizeWorkspaceState")*/)
{
	m_bIncludeBars=bIncludeBars;
	m_bIncludeMainWnd=bIncludeMainWnd;
	m_bIncludeChildFrames=bIncludeChildFrames;
	m_bIncludeSplitterPanes=bIncludeSplitterPanes;

	m_sProfileName=lpszProfileName;
	if(!m_sProfileName.IsEmpty())
	{
#ifndef _MAC
		CWinApp* pApp=AfxGetApp();
		ASSERT(pApp!=NULL);

		m_bIncludeBars=(BOOL)pApp->GetProfileInt(lpszProfileName,
			_T("IncludeBars"),m_bIncludeBars);
		m_bIncludeMainWnd=(BOOL)pApp->GetProfileInt(lpszProfileName,
			_T("IncludeMainWnd"),m_bIncludeMainWnd);
		m_bIncludeChildFrames=(BOOL)pApp->GetProfileInt(lpszProfileName,
			_T("IncludeChildFrames"),m_bIncludeChildFrames);
		m_bIncludeSplitterPanes=(BOOL)pApp->GetProfileInt(lpszProfileName,
			_T("IncludeSplitterPanes"),m_bIncludeSplitterPanes);
#endif
	}

	m_workspaceState.IncludeMainWindow(m_bIncludeMainWnd);
	m_workspaceState.IncludeChildFrame(m_bIncludeChildFrames);
	m_workspaceState.IncludeSplitterPane(m_bIncludeSplitterPanes);
	m_workspaceState.IncludeBarProperty(m_pCustomizeManager->IsDockingFramework() ? 
		FALSE : m_bIncludeBars);
	BOOL bResult=m_workspaceState.LoadFromRegistry();

	if(bResult)
	{
		// read saved info about customize pages displayed 
		GetPagesInfo();
	}
	else
	{
		TRACE(_T("COXCustomizeWorkspaceStatePage::LoadWorkspaceState: failed to load workspace from registry, continuing\n"));
	}

	return TRUE;
}


BOOL COXCustomizeWorkspaceStatePage::SaveWorkspaceState()
{
	if(!COXCustomizePage::SaveWorkspaceState())
		return FALSE;

	m_workspaceState.IncludeMainWindow(m_bIncludeMainWnd);
	m_workspaceState.IncludeChildFrame(m_bIncludeChildFrames);
	m_workspaceState.IncludeSplitterPane(m_bIncludeSplitterPanes);
	m_workspaceState.IncludeBarProperty(m_pCustomizeManager->IsDockingFramework() ? 
		FALSE : m_bIncludeBars);
	if(!m_workspaceState.StoreToRegistry())
	{
		TRACE(_T("COXCustomizeWorkspaceStatePage::SaveWorkspaceState: failed to save workspace from registry, continuing\n"));
		return FALSE;
	}

	return TRUE;
}


void COXCustomizeWorkspaceStatePage::DoDataExchange(CDataExchange* pDX)
{
	COXCustomizePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXCustomizeWorkspaceStatePage)
	DDX_Control(pDX, IDC_OX_SEPARATOR_WORKSPACE, m_separator);
	DDX_Control(pDX, IDC_OX_TREE_CUSTOMIZE_PAGES, m_treePages);
	DDX_Check(pDX, IDC_OX_CHECK_BAR, m_bIncludeBars);
	DDX_Check(pDX, IDC_OX_CHECK_MAINWND, m_bIncludeMainWnd);
	DDX_Check(pDX, IDC_OX_CHECK_CHILD, m_bIncludeChildFrames);
	DDX_Check(pDX, IDC_OX_CHECK_SPLITTER, m_bIncludeSplitterPanes);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COXCustomizeWorkspaceStatePage, COXCustomizePage)
	//{{AFX_MSG_MAP(COXCustomizeWorkspaceStatePage)
	ON_NOTIFY(OTN_OPTIONCHANGED, IDC_OX_TREE_CUSTOMIZE_PAGES, OnOptionChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeWorkspaceStatePage message handlers

BOOL COXCustomizeWorkspaceStatePage::Load(const COXCustomizeManager* pCustomizeManager)
{
	// Call default implementation. It will load this demo dialog as the first 
	// page and will create About and CodeSample pages if specified.
	if(!COXCustomizePage::Load(pCustomizeManager))
		return FALSE;

	return TRUE;
}


void COXCustomizeWorkspaceStatePage::Unload()
{
	// add here code for cleaning up all objects created by demo
	//
	//
	//////////////////////////////////////////////////////////////////////////

	COXCustomizePage::Unload();
}

void COXCustomizeWorkspaceStatePage::OnInitDialog()
{
	// must call default implementation
	COXCustomizePage::OnInitDialog();

	// add here initialization code for your demo dialog. Treat it as a
	// normal CDialog::OnInitDialog function

	GetVars();

	// layout
	//
	m_LayoutManager.TieChild(&m_treePages,OX_LMS_ANY,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_separator,OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	//
	///////////////////////////////////////

	m_bInitialized=TRUE;
}


void COXCustomizeWorkspaceStatePage::GetVars()
{
	if(!m_pCustomizeManager->IsMDIFramework())
		GetDlgItem(IDC_OX_CHECK_CHILD)->ShowWindow(SW_HIDE);

	// build tree control
	//
	m_treePages.DeleteAllItems();

	// root item
	int nImageIndex=m_treePages.AddImage(IDB_OX_CUSTOMIZEWORKSPACESTATE_PAGES,RGB(192,192,192));
	ASSERT(nImageIndex!=-1);
	CString sItem;
	VERIFY(sItem.LoadString(IDS_OX_CSTMZEWORKSPACEGROUP));//"Customize pages (check the page to be displayed)"
	HTREEITEM hRootItem=m_treePages.AddControlGroup(sItem,NULL,TRUE,
		nImageIndex,nImageIndex);
	ASSERT(hRootItem!=NULL);
	m_treePages.SetItemState(hRootItem,TVIS_BOLD,TVIS_BOLD);

	CMapPages* pMapPages=m_pCustomizeManager->GetMapPages();
	ASSERT(pMapPages!=NULL);
	POSITION pos=pMapPages->GetStartPosition();
	int nIndex=0;
	while(pos!=NULL)
	{
		DWORD dwID=NULL;
		COXCustomizePage* pCustomizePage=NULL;
		pMapPages->GetNextAssoc(pos,pCustomizePage,dwID);
		ASSERT(pCustomizePage!=NULL && dwID!=NULL);


		HSHBGROUP hGroupTest=NULL;
		int nIndexTest=-1;
		BOOL bIsViewable=m_pCustomizeManager->FindPage(pCustomizePage,
			hGroupTest,nIndexTest);
		BOOL bIsReadOnly=(pCustomizePage==this);
		HTREEITEM hItem=m_treePages.AddCheckBox(IDC_CHECK_PAGE_BASE+nIndex,
			pCustomizePage->GetTitle(),hRootItem,
			(bIsViewable ? OTITEM_CHECKED : OTITEM_UNCHECKED),
			TVI_SORT,bIsReadOnly);
		ASSERT(hItem!=NULL);
		VERIFY(m_treePages.SetItemData(hItem,(LPARAM)pCustomizePage));

		nIndex++;
	}
}


BOOL COXCustomizeWorkspaceStatePage::IsViewablePage(COXCustomizePage* pCustomizePage)
{
	ASSERT(pCustomizePage!=NULL);
	if(pCustomizePage==NULL)
		return FALSE;

	if(pCustomizePage==this)
		return TRUE;
	
	CString sPageKey=BuildPageKey(pCustomizePage);
	ASSERT(!sPageKey.IsEmpty());
	BOOL bViewable=FALSE;
	if(!m_mapPageNames.Lookup(sPageKey,bViewable))
	{
		TRACE(_T("COXCustomizeWorkspaceStatePage::IsViewablePage: failed to find specified page\n"));
		return TRUE;
	}

	return bViewable;
}


void COXCustomizeWorkspaceStatePage::OnOptionChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMOPTIONTREE* pNMOptionTree=(NMOPTIONTREE*)pNMHDR;
	ASSERT(pNMOptionTree!=NULL);
	// TODO: Add your control notification handler code here

	*pResult = 0;

	HTREEITEM hItem=pNMOptionTree->hItem;
	ASSERT(hItem!=NULL);
	COXCustomizePage* pCustomizePage=
		(COXCustomizePage*)m_treePages.GetItemData(hItem);
	ASSERT(pCustomizePage!=NULL);

	BOOL bViewable=FALSE;
	CString sPageKey=BuildPageKey(pCustomizePage);
	ASSERT(!sPageKey.IsEmpty());

	bViewable=(pNMOptionTree->nNewCheck==OTITEM_CHECKED);
	m_mapPageNames.SetAt(sPageKey,bViewable);
	
}


void COXCustomizeWorkspaceStatePage::GetPagesInfo()
{
	m_mapPageNames.RemoveAll();
	
	if(!m_sProfileName.IsEmpty())
	{
#ifndef _MAC
		CWinApp* pApp=AfxGetApp();
		ASSERT(pApp!=NULL);

		for(int nIndex=0; ; nIndex++)
		{
			CString sProfileKey;
			sProfileKey.Format(_T("%d_CustomizePageKey"),nIndex);
			CString sPageKey=pApp->GetProfileString(m_sProfileName,sProfileKey);
			if(sPageKey.IsEmpty())
				break;
			BOOL bViewable=(BOOL)pApp->GetProfileInt(m_sProfileName,sPageKey,TRUE);
			m_mapPageNames.SetAt(sPageKey,bViewable);
		}
#endif
	}
}