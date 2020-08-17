// ==========================================================================
//			Class Implementation: COXCustomizeToolbarsPage
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
#include "OXCustomizeToolbarsPage.h"
#include "OXCustomizeManager.h"
#include "OXSkins.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeToolbarsPage dialog

IMPLEMENT_DYNCREATE(COXCustomizeToolbarsPage, COXCustomizePage)


COXCustomizeToolbarsPage::COXCustomizeToolbarsPage()
{
	//{{AFX_DATA_INIT(COXCustomizeToolbarsPage)
	m_bCoolLook=TRUE;
	m_bShowTooltips=TRUE;
	m_nBorderSize=0;
	m_bShowGripper=TRUE;
	//}}AFX_DATA_INIT

	m_nDialogID=IDD;
	m_sProfileName.Empty();
}


COXCustomizeToolbarsPage::~COXCustomizeToolbarsPage()
{
	for(int nIndex=0; nIndex<m_arrCustomMadeToolbars.GetSize(); nIndex++)
	{
		COXCoolToolBar* pToolbar=m_arrCustomMadeToolbars[nIndex];
		ASSERT(pToolbar!=NULL);
		delete pToolbar;
	}
	m_arrCustomMadeToolbars.RemoveAll();

	m_arrToolbars.RemoveAll();

	m_mapAssociatedToolbars.RemoveAll();
}


BOOL COXCustomizeToolbarsPage::OnCloseManager(BOOL bIsOk) 
{	
	if(!UpdateData(TRUE))
		return FALSE;

	if(bIsOk)
	{
		if(!m_sProfileName.IsEmpty())
		{
#ifndef _MAC
			CWinApp* pApp=AfxGetApp();
			ASSERT(pApp!=NULL);

			// save toolbar options
			pApp->WriteProfileInt(m_sProfileName,_T("CoolLook"),m_bCoolLook);
			pApp->WriteProfileInt(m_sProfileName,_T("ShowTooltips"),m_bShowTooltips);
			pApp->WriteProfileInt(m_sProfileName,_T("ShowGripper"),m_bShowGripper);
			pApp->WriteProfileInt(m_sProfileName,_T("BorderSize"),m_nBorderSize);

			// delete old information about created custom made toolbars
			int nCustomToolbarIndex=0;
			while(TRUE)
			{
				CString sProfileKey=BuildToolbarKey(nCustomToolbarIndex)+_T("_ID");
				UINT nToolbarID=(UINT)pApp->GetProfileInt(m_sProfileName,
					sProfileKey,NULL);
				if(nToolbarID==NULL)
					break;
				pApp->WriteProfileString(m_sProfileName,sProfileKey,NULL);
				sProfileKey=BuildToolbarKey(nCustomToolbarIndex)+_T("_Title");
				pApp->WriteProfileString(m_sProfileName,sProfileKey,NULL);

				nCustomToolbarIndex++;
			}

			// save information about created custom made toolbars
			nCustomToolbarIndex=0;
			for(int nIndex=0; nIndex<m_arrToolbars.GetSize(); nIndex++)
			{
				OXCUSTOMIZE_TOOLBARINFO& tbInfo=m_arrToolbars[nIndex];
				ASSERT(tbInfo.m_pToolBar!=NULL);
				if(tbInfo.m_bCustomMade)
				{
					CString sProfileKey=BuildToolbarKey(nCustomToolbarIndex)+_T("_ID");
					pApp->WriteProfileInt(m_sProfileName,sProfileKey,
						tbInfo.m_pToolBar->GetDlgCtrlID());

					sProfileKey=BuildToolbarKey(nCustomToolbarIndex)+_T("_Title");
					CString sTitle;
					tbInfo.m_pToolBar->GetWindowText(sTitle);
					ASSERT(!sTitle.IsEmpty());
					pApp->WriteProfileString(m_sProfileName,sProfileKey,sTitle);

					nCustomToolbarIndex++;
				}
			}
#endif
		}
	}

	return TRUE;
}


BOOL COXCustomizeToolbarsPage::
InitializeToolbars(BOOL bCoolLook/*=TRUE*/, 
				   BOOL bShowGripper/*=TRUE*/,
				   BOOL bShowTooltips/*=TRUE*/, 
				   int nBorderSize/*=0*/, 
				   LPCTSTR lpszProfileName/*=_T("CustomizeToolbars")*/)
{
	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL);
	CFrameWnd* pFrameWnd=DYNAMIC_DOWNCAST(CFrameWnd,pWnd);
	ASSERT(pFrameWnd!=NULL);

	m_bCoolLook=bCoolLook;
	m_bShowTooltips=bShowTooltips;
	m_nBorderSize=nBorderSize;
	m_bShowGripper=bShowGripper;

	// go through all existing toolbars and save information about them
	CControlBar* pBar=NULL;
	POSITION pos=pFrameWnd->m_listControlBars.GetHeadPosition();
	while(pos!=NULL)
	{
		pBar=(CControlBar*)pFrameWnd->m_listControlBars.GetNext(pos);
		ASSERT(pBar!=NULL);

		if(pBar->IsKindOf(RUNTIME_CLASS(CToolBar)))
		{
			CToolBar* pToolbar=DYNAMIC_DOWNCAST(CToolBar,pBar);
			ASSERT(pToolbar!=NULL);
			OXCUSTOMIZE_TOOLBARINFO tbInfo(pToolbar,FALSE);
			m_arrToolbars.Add(tbInfo);

#ifdef _DEBUG
			int nToolbarIndex=-1;
			ASSERT(!m_mapAssociatedToolbars.Lookup(pToolbar,nToolbarIndex));
#endif	//	_DEBUG
			m_mapAssociatedToolbars.SetAt(pToolbar,PtrToInt(m_arrToolbars.GetSize())-1);
		}
	}


	if(lpszProfileName!=NULL)
		m_sProfileName=lpszProfileName;

#ifndef _MAC
	if(!m_sProfileName.IsEmpty())
	{
		CWinApp* pApp=AfxGetApp();
		ASSERT(pApp!=NULL);

		// retrieve toolbar options
		m_bCoolLook=(BOOL)pApp->GetProfileInt(m_sProfileName,
			_T("CoolLook"),m_bCoolLook);
		m_bShowTooltips=(BOOL)pApp->GetProfileInt(m_sProfileName,
			_T("ShowTooltips"),m_bShowTooltips);
		m_bShowGripper=(BOOL)pApp->GetProfileInt(m_sProfileName,
			_T("ShowGripper"),m_bShowGripper);
		m_nBorderSize=(int)pApp->GetProfileInt(m_sProfileName,
			_T("BorderSize"),m_nBorderSize);

		// retrieve information about saved custom made toolbars
		// and create them
		int nIndex=0;
		while(TRUE)
		{
			CString sProfileKey=BuildToolbarKey(nIndex)+_T("_ID");
			UINT nToolbarID=(UINT)pApp->GetProfileInt(m_sProfileName,sProfileKey,NULL);
			if(nToolbarID==NULL)
				break;
			sProfileKey=BuildToolbarKey(nIndex)+_T("_Title");
			CString sTitle=pApp->GetProfileString(m_sProfileName,sProfileKey,_T(""));
			ASSERT(!sTitle.IsEmpty());

			COXCoolToolBar* pToolbar=new COXCoolToolBar;
			ASSERT(pToolbar!=NULL);
			m_arrCustomMadeToolbars.Add(pToolbar);
			VERIFY(pToolbar->Create(pFrameWnd,WS_CHILD|WS_VISIBLE|CBRS_TOP|
				CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC,nToolbarID));
			pToolbar->IniSizes(CSize(ID_OXCUSTOMIZE_TBIMAGE_WIDTH,
				ID_OXCUSTOMIZE_TBIMAGE_HEIGHT));
			pToolbar->UpdateSizes();
			pToolbar->SetWindowText(sTitle);
			pToolbar->EnableDocking(CBRS_ALIGN_ANY);
			pFrameWnd->DockControlBar(pToolbar);

			OXCUSTOMIZE_TOOLBARINFO tbInfo(pToolbar,TRUE);
			m_arrToolbars.Add(tbInfo);

#ifdef _DEBUG
			int nToolbarIndex=-1;
			ASSERT(!m_mapAssociatedToolbars.Lookup(pToolbar,nToolbarIndex));
#endif	//	_DEBUG
			m_mapAssociatedToolbars.SetAt(pToolbar,PtrToInt(m_arrToolbars.GetSize())-1);

			nIndex++;
		}
	}
#endif

	// go through toolbars and apply options and load saved states
	for(int nIndex=0; nIndex<m_arrToolbars.GetSize(); nIndex++)
	{
		OXCUSTOMIZE_TOOLBARINFO& tbInfo=m_arrToolbars[nIndex];
		CToolBar* pToolbar=tbInfo.m_pToolBar;
		ASSERT(pToolbar!=NULL);

		// apply saved toobars options
		if(!pToolbar->IsKindOf(RUNTIME_CLASS(COXMenuBar)))
		{
			if(m_bShowTooltips)
				pToolbar->SetBarStyle(pToolbar->GetBarStyle()|CBRS_TOOLTIPS);
			else
				pToolbar->SetBarStyle(pToolbar->GetBarStyle()&~CBRS_TOOLTIPS);
			pToolbar->m_cxLeftBorder=m_nBorderSize;
			pToolbar->m_cyTopBorder=m_nBorderSize;
			pToolbar->m_cxRightBorder=m_nBorderSize;
			pToolbar->m_cyBottomBorder=m_nBorderSize;

			if(pToolbar->IsKindOf(RUNTIME_CLASS(COXCoolToolBar)))
			{
				COXCoolToolBar* pCoolToolbar=DYNAMIC_DOWNCAST(COXCoolToolBar,pToolbar);
				ASSERT(pCoolToolbar!=NULL);
				pCoolToolbar->SetFlat(m_bCoolLook);
				pCoolToolbar->SetGripper(m_bShowGripper);
				pCoolToolbar->RedrawToolBar();
			}
			else
			{
				pToolbar->SetWindowPos(NULL,0,0,0,0,
					SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_DRAWFRAME);
				pToolbar->RedrawWindow(NULL,NULL,
					RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_FRAME|RDW_ALLCHILDREN);

				CFrameWnd* pFrameWnd=pToolbar->GetDockingFrame();
				if(pFrameWnd!=NULL)
					pFrameWnd->RecalcLayout();
			}
		}
	}

	return TRUE;
}


void COXCustomizeToolbarsPage::DoDataExchange(CDataExchange* pDX)
{
	COXCustomizePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXCustomizeToolbarsPage)
	DDX_Control(pDX, IDC_OX_TREE_TOOLBARS, m_treeToolbars);
	DDX_Control(pDX, IDC_OX_SPIN_BORDER_SIZE, m_spinBorderSize);
	DDX_Control(pDX, IDC_OX_SEPARATOR_TOOLBARS, m_sepToolbars);
	DDX_Control(pDX, IDC_OX_SEPARATOR_OPTIONS, m_sepOptions);
	DDX_Control(pDX, IDC_OX_SEPARATOR_OPERATIONS, m_sepOperations);
	DDX_Check(pDX, IDC_OX_CHECK_COOL, m_bCoolLook);
	DDX_Check(pDX, IDC_OX_CHECK_TOOLTIPS, m_bShowTooltips);
	DDX_Text(pDX, IDC_OX_EDIT_BORDER_SIZE, m_nBorderSize);
	DDV_MinMaxDWord(pDX, m_nBorderSize, 0, 100);
	DDX_Check(pDX, IDC_OX_CHECK_GRIPPER, m_bShowGripper);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COXCustomizeToolbarsPage, COXCustomizePage)
	//{{AFX_MSG_MAP(COXCustomizeToolbarsPage)
	ON_BN_CLICKED(IDC_OX_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_OX_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_OX_BUTTON_RENAME, OnButtonRename)
	ON_BN_CLICKED(IDC_OX_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_OX_BUTTON_RESET_ALL, OnButtonResetAll)
	ON_BN_CLICKED(IDC_OX_CHECK_COOL, OnCheckCool)
	ON_BN_CLICKED(IDC_OX_CHECK_TOOLTIPS, OnCheckTooltips)
	ON_EN_CHANGE(IDC_OX_EDIT_BORDER_SIZE, OnChangeEditBorderSize)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_OX_TREE_TOOLBARS, OnEndlabeleditTreeToolbars)
	ON_NOTIFY(OTN_OPTIONCHANGED, IDC_OX_TREE_TOOLBARS, OnOptionChanged)
	ON_BN_CLICKED(IDC_OX_CHECK_GRIPPER, OnCheckGripper)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_OX_TREE_TOOLBARS, OnBeginlabeleditTreeToolbars)
	ON_NOTIFY(TVN_SELCHANGED, IDC_OX_TREE_TOOLBARS, OnSelchangedTreeToolbars)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeToolbarsPage message handlers
BOOL COXCustomizeToolbarsPage::Load(const COXCustomizeManager* pCustomizeManager)
{
	// Call default implementation. It will load this demo dialog as the first 
	// page and will create About and CodeSample pages if specified.
	if(!COXCustomizePage::Load(pCustomizeManager))
		return FALSE;

	return TRUE;
}


void COXCustomizeToolbarsPage::Unload()
{
	// add here code for cleaning up all objects created by demo
	//
	//
	//////////////////////////////////////////////////////////////////////////

	COXCustomizePage::Unload();
}

void COXCustomizeToolbarsPage::OnInitDialog()
{
	// must call default implementation
	COXCustomizePage::OnInitDialog();

	// add here initialization code for your demo dialog. Treat it as a
	// normal CDialog::OnInitDialog function

	// separator controls
	COLORREF clr=::GetSysColor(COLOR_WINDOW);
	m_sepToolbars.SetTextColor(clr);
	m_sepOptions.SetTextColor(clr);
	m_sepOperations.SetTextColor(clr);

	// spin control
	m_spinBorderSize.SetRange(0,1000);

	GetVars();

	// layout
	//
	m_LayoutManager.TieChild(&m_treeToolbars,OX_LMS_ANY,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_sepToolbars,OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_sepOperations,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_sepOptions,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_BUTTON_NEW,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_BUTTON_DELETE,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_BUTTON_RENAME,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_BUTTON_RESET,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_BUTTON_RESET_ALL,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_CHECK_TOOLTIPS,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_CHECK_COOL,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_CHECK_GRIPPER,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_STATIC_BORDER_SIZE,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_EDIT_BORDER_SIZE,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_SPIN_BORDER_SIZE,OX_LMS_RIGHT,OX_LMT_SAME);
	//
	///////////////////////////////////////

	// If the OfficeXP skin is on disable the CoolLook checkbox
	COXSkinnedApp* pSkinnedApp = DYNAMIC_DOWNCAST(COXSkinnedApp, AfxGetApp());
	if (pSkinnedApp != NULL)
	{
		COXSkin* pCurrentSkin = pSkinnedApp->GetCurrentSkin();
		if (pCurrentSkin != NULL)
			if (pCurrentSkin->GetName() == _T("Office XP"))
			{
				CButton* pBtn = (CButton*) GetDlgItem(IDC_OX_CHECK_COOL);
				pBtn->SetCheck(TRUE);
				pBtn->EnableWindow(FALSE);
			}
	}

	m_bInitialized=TRUE;

	UpdateState();
}


void COXCustomizeToolbarsPage::GetVars()
{
	// build tree control
	//
	m_treeToolbars.DeleteAllItems();

	for(int nIndex=0; nIndex<m_arrToolbars.GetSize(); nIndex++)
	{
		OXCUSTOMIZE_TOOLBARINFO& tbInfo=m_arrToolbars[nIndex];
		ASSERT(tbInfo.m_pToolBar!=NULL);

		CString sTitle;
		tbInfo.m_pToolBar->GetWindowText(sTitle);
		BOOL bIsVisible=((tbInfo.m_pToolBar->GetStyle()&WS_VISIBLE)==WS_VISIBLE);

		HTREEITEM hItem=m_treeToolbars.AddCheckBox(tbInfo.m_pToolBar->GetDlgCtrlID(),
			sTitle,TVI_ROOT,(bIsVisible ? OTITEM_CHECKED : OTITEM_UNCHECKED));
		ASSERT(hItem!=NULL);
		m_treeToolbars.SetItemData(hItem,(LPARAM)tbInfo.m_pToolBar);
	}
}


void COXCustomizeToolbarsPage::UpdateState()
{
	if(!m_bInitialized)
		return;
	
	if(!UpdateData(TRUE))
		return;

#ifndef OX_CUSTOMIZE_COMMANDS
	GetDlgItem(IDC_OX_BUTTON_RESET)->EnableWindow(FALSE);
	GetDlgItem(IDC_OX_BUTTON_RESET_ALL)->EnableWindow(FALSE);
#endif	//	 OX_CUSTOMIZE_COMMANDS

	HTREEITEM hItem=m_treeToolbars.GetSelectedItem();
	if(hItem==NULL)
	{
		GetDlgItem(IDC_OX_BUTTON_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_OX_BUTTON_RENAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_OX_BUTTON_RESET)->EnableWindow(FALSE);
	}
	else
	{
		CToolBar* pToolbar=(CToolBar*)m_treeToolbars.GetItemData(hItem);
		ASSERT(pToolbar!=NULL);

		int nToolbarIndex=-1;
		VERIFY(m_mapAssociatedToolbars.Lookup(pToolbar,nToolbarIndex));
		ASSERT(nToolbarIndex!=-1);
		OXCUSTOMIZE_TOOLBARINFO& rtbInfo=m_arrToolbars[nToolbarIndex];
		ASSERT(rtbInfo.m_pToolBar==pToolbar);
		if(rtbInfo.m_bCustomMade)
		{
			GetDlgItem(IDC_OX_BUTTON_DELETE)->EnableWindow(TRUE);
			GetDlgItem(IDC_OX_BUTTON_RENAME)->EnableWindow(TRUE);
			GetDlgItem(IDC_OX_BUTTON_RESET)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_OX_BUTTON_DELETE)->EnableWindow(FALSE);
			GetDlgItem(IDC_OX_BUTTON_RENAME)->EnableWindow(FALSE);
#ifdef OX_CUSTOMIZE_COMMANDS
			GetDlgItem(IDC_OX_BUTTON_RESET)->EnableWindow(TRUE);
#endif	//	 OX_CUSTOMIZE_COMMANDS
		}
	}

	GetDlgItem(IDC_OX_CHECK_GRIPPER)->EnableWindow(m_bCoolLook);
}


void COXCustomizeToolbarsPage::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hItem=m_treeToolbars.GetSelectedItem();
	ASSERT(hItem!=NULL);

	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL);
	CFrameWnd* pFrameWnd=DYNAMIC_DOWNCAST(CFrameWnd,pWnd);
	ASSERT(pFrameWnd!=NULL);

	CToolBar* pToolbar=(CToolBar*)m_treeToolbars.GetItemData(hItem);
	ASSERT(pToolbar!=NULL);
	BOOL bWasFloating=pToolbar->IsFloating();

	int nToolbarIndex=-1;
	VERIFY(m_mapAssociatedToolbars.Lookup(pToolbar,nToolbarIndex));
	ASSERT(nToolbarIndex!=-1);
	OXCUSTOMIZE_TOOLBARINFO& rtbInfo=m_arrToolbars[nToolbarIndex];
	ASSERT(rtbInfo.m_pToolBar==pToolbar);
	ASSERT(rtbInfo.m_bCustomMade);

	VERIFY(pToolbar->DestroyWindow());
	POSITION pos=pFrameWnd->m_listControlBars.Find(pToolbar);
	if(pos!=NULL)
		pFrameWnd->m_listControlBars.RemoveAt(pos);
	
	m_mapAssociatedToolbars.RemoveKey(pToolbar);
	pos=m_mapAssociatedToolbars.GetStartPosition();
	while(pos!=NULL)
	{
		CToolBar* pToolbarTest=NULL;
		int nIndexTest=-1;
		m_mapAssociatedToolbars.GetNextAssoc(pos,pToolbarTest,nIndexTest);
		ASSERT(pToolbarTest!=NULL);
		ASSERT(nIndexTest!=-1 && nIndexTest!=nToolbarIndex);
		if(nIndexTest>nToolbarIndex)
			m_mapAssociatedToolbars.SetAt(pToolbarTest,nIndexTest-1);
	}

	m_arrToolbars.RemoveAt(nToolbarIndex);

	m_treeToolbars.DeleteItem(hItem);

	if(!bWasFloating)
		pFrameWnd->RecalcLayout();
}

void COXCustomizeToolbarsPage::OnButtonNew() 
{
	// TODO: Add your control notification handler code here

	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL);
	CFrameWnd* pFrameWnd=DYNAMIC_DOWNCAST(CFrameWnd,pWnd);
	ASSERT(pFrameWnd!=NULL);

	COXCoolToolBar* pToolbar=NULL;
	// go through already created custom-made toolbars and find 
	// if any was already deleted
	int nIndex=0;
	for(nIndex=0; nIndex<m_arrCustomMadeToolbars.GetSize(); nIndex++)
	{
		COXCoolToolBar* pExistingToolbar=m_arrCustomMadeToolbars[nIndex];
		ASSERT(pExistingToolbar!=NULL);
		int nToolbarIndex=-1;
		if(!m_mapAssociatedToolbars.Lookup(pExistingToolbar,nToolbarIndex))
		{
			ASSERT(nToolbarIndex==-1);
			pToolbar=pExistingToolbar;
			break;
		}
	}
	if(pToolbar==NULL)
	{
		pToolbar=new COXCoolToolBar;
		ASSERT(pToolbar!=NULL);
		m_arrCustomMadeToolbars.Add(pToolbar);
	}
	ASSERT(pToolbar!=NULL);

	UINT nToolbarID=NULL;
	for(nIndex=AFX_IDW_CONTROLBAR_LAST; nIndex>AFX_IDW_CONTROLBAR_FIRST+20; nIndex--)
	{
		if(pFrameWnd->GetControlBar(nIndex)==NULL)
		{
			nToolbarID=nIndex;
			break;
		}
	}
	if(nToolbarID==NULL)
	{
		TRACE(_T("COXCustomizeToolbarsPage::OnButtonNew: failed to create new toolbar, there is no available IDs left\n"));
		return;
	}

	VERIFY(pToolbar->Create(pFrameWnd,WS_CHILD|WS_VISIBLE|CBRS_TOP|
		CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC,nToolbarID));

	// convert toolbar to be compatible with "Customize Commands" page
#ifdef OX_CUSTOMIZE_COMMANDS
	COXCustomizeCommandsPage* pCommandsPage=m_pCustomizeManager->GetCommandsPage();
	if(pCommandsPage!=NULL)
	{
		pCommandsPage->ConvertToolbar(pToolbar);
	}
#endif	//	OX_CUSTOMIZE_COMMANDS

	CString sTitle;
	sTitle.Format(IDS_OX_CSTMZETOOLBARTITLECUSTOM, m_arrCustomMadeToolbars.GetSize());//"Custom Toolbar %d"
	pToolbar->SetWindowText(sTitle);
	pToolbar->EnableDocking(CBRS_ALIGN_ANY);
	pFrameWnd->FloatControlBar(pToolbar,CPoint(100,100));

	// apply toolbar options
	if(m_bShowTooltips)
		pToolbar->SetBarStyle(pToolbar->GetBarStyle()|CBRS_TOOLTIPS);
	else
		pToolbar->SetBarStyle(pToolbar->GetBarStyle()&~CBRS_TOOLTIPS);
	pToolbar->m_cxLeftBorder=m_nBorderSize;
	pToolbar->m_cyTopBorder=m_nBorderSize;
	pToolbar->m_cxRightBorder=m_nBorderSize;
	pToolbar->m_cyBottomBorder=m_nBorderSize;

	pToolbar->SetFlat(m_bCoolLook);
	pToolbar->SetGripper(m_bShowGripper);
	pToolbar->RedrawToolBar();

	OXCUSTOMIZE_TOOLBARINFO tbInfo(pToolbar,TRUE);
	m_arrToolbars.Add(tbInfo);

#ifdef _DEBUG
	int nToolbarIndex=-1;
	VERIFY(!m_mapAssociatedToolbars.Lookup(pToolbar,nToolbarIndex));
#endif	//	_DEBUG
	m_mapAssociatedToolbars.SetAt(pToolbar,PtrToInt(m_arrToolbars.GetSize())-1);

	// add new item to the tree
	HTREEITEM hItem=m_treeToolbars.AddCheckBox(pToolbar->GetDlgCtrlID(),
		sTitle,TVI_ROOT,OTITEM_CHECKED);
	ASSERT(hItem!=NULL);
	m_treeToolbars.SetItemData(hItem,(LPARAM)tbInfo.m_pToolBar);

	m_treeToolbars.SelectItem(hItem);
	m_treeToolbars.EditLabel(hItem);
}

void COXCustomizeToolbarsPage::OnButtonRename() 
{
	// TODO: Add your control notification handler code here
	
	HTREEITEM hItem=m_treeToolbars.GetSelectedItem();
	ASSERT(hItem!=NULL);

	m_treeToolbars.EditLabel(hItem);
}

void COXCustomizeToolbarsPage::OnButtonReset() 
{
#ifdef OX_CUSTOMIZE_COMMANDS
	// TODO: Add your control notification handler code here
	HTREEITEM hItem=m_treeToolbars.GetSelectedItem();
	ASSERT(hItem!=NULL);

	CToolBar* pToolbar=(CToolBar*)m_treeToolbars.GetItemData(hItem);
	ASSERT(pToolbar!=NULL);

	// restore the state from the registry
	ASSERT(m_pCustomizeManager!=NULL);
	ASSERT(m_pCustomizeManager->GetCommandsPage()!=NULL);
	m_pCustomizeManager->GetCommandsPage()->LoadToolbarState(pToolbar);
#endif	//	OX_CUSTOMIZE_COMMANDS
}

void COXCustomizeToolbarsPage::OnButtonResetAll() 
{
#ifdef OX_CUSTOMIZE_COMMANDS
	// TODO: Add your control notification handler code here
	// go through toolbars and apply options and load saved states
	for(int nIndex=0; nIndex<m_arrToolbars.GetSize(); nIndex++)
	{
		OXCUSTOMIZE_TOOLBARINFO& tbInfo=m_arrToolbars[nIndex];
		CToolBar* pToolbar=tbInfo.m_pToolBar;
		ASSERT(pToolbar!=NULL);

		// restore the state from the registry
		ASSERT(m_pCustomizeManager!=NULL);
		ASSERT(m_pCustomizeManager->GetCommandsPage()!=NULL);
		m_pCustomizeManager->GetCommandsPage()->LoadToolbarState(pToolbar);
	}
#endif	//	OX_CUSTOMIZE_COMMANDS
}

void COXCustomizeToolbarsPage::OnCheckCool() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInitialized)
		return;
	
	if(!UpdateData(TRUE))
		return;

	// go through toolbars and apply options
	for(int nIndex=0; nIndex<m_arrToolbars.GetSize(); nIndex++)
	{
		OXCUSTOMIZE_TOOLBARINFO& tbInfo=m_arrToolbars[nIndex];
		CToolBar* pToolbar=tbInfo.m_pToolBar;
		ASSERT(pToolbar!=NULL);
		if(pToolbar->IsKindOf(RUNTIME_CLASS(COXCoolToolBar)) && 
			!pToolbar->IsKindOf(RUNTIME_CLASS(COXMenuBar)))
		{
			COXCoolToolBar* pCoolToolbar=DYNAMIC_DOWNCAST(COXCoolToolBar,pToolbar);
			ASSERT(pCoolToolbar!=NULL);
			pCoolToolbar->SetFlat(m_bCoolLook);
			pCoolToolbar->RedrawToolBar(TRUE,TRUE);
		}
	}

	UpdateState();
}

void COXCustomizeToolbarsPage::OnCheckTooltips() 
{
	// TODO: Add your control notification handler code here
	
	if(!m_bInitialized)
		return;
	
	if(!UpdateData(TRUE))
		return;

	// go through toolbars and apply options
	for(int nIndex=0; nIndex<m_arrToolbars.GetSize(); nIndex++)
	{
		OXCUSTOMIZE_TOOLBARINFO& tbInfo=m_arrToolbars[nIndex];
		CToolBar* pToolbar=tbInfo.m_pToolBar;
		ASSERT(pToolbar!=NULL);
		if(!pToolbar->IsKindOf(RUNTIME_CLASS(COXMenuBar)))
		{
			if(m_bShowTooltips)
				pToolbar->SetBarStyle(pToolbar->GetBarStyle()|CBRS_TOOLTIPS);
			else
				pToolbar->SetBarStyle(pToolbar->GetBarStyle()&~CBRS_TOOLTIPS);
		}
	}

}

void COXCustomizeToolbarsPage::OnCheckGripper() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInitialized)
		return;
	
	if(!UpdateData(TRUE))
		return;

	ASSERT(m_bCoolLook);
	// go through toolbars and apply options
	for(int nIndex=0; nIndex<m_arrToolbars.GetSize(); nIndex++)
	{
		OXCUSTOMIZE_TOOLBARINFO& tbInfo=m_arrToolbars[nIndex];
		CToolBar* pToolbar=tbInfo.m_pToolBar;
		ASSERT(pToolbar!=NULL);
		if(pToolbar->IsKindOf(RUNTIME_CLASS(COXCoolToolBar)) && 
			!pToolbar->IsKindOf(RUNTIME_CLASS(COXMenuBar)))
		{
			COXCoolToolBar* pCoolToolbar=DYNAMIC_DOWNCAST(COXCoolToolBar,pToolbar);
			ASSERT(pCoolToolbar!=NULL);
			pCoolToolbar->SetGripper(m_bShowGripper);
			pCoolToolbar->RedrawToolBar(TRUE,TRUE);
		}
	}
}

void COXCustomizeToolbarsPage::OnChangeEditBorderSize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the COXCustomizePage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	if(!m_bInitialized)
		return;
	
	if(!UpdateData(TRUE))
		return;

	// go through toolbars and apply options
	for(int nIndex=0; nIndex<m_arrToolbars.GetSize(); nIndex++)
	{
		OXCUSTOMIZE_TOOLBARINFO& tbInfo=m_arrToolbars[nIndex];
		CToolBar* pToolbar=tbInfo.m_pToolBar;
		ASSERT(pToolbar!=NULL);
		pToolbar->m_cxLeftBorder=m_nBorderSize;
		pToolbar->m_cyTopBorder=m_nBorderSize;
		pToolbar->m_cxRightBorder=m_nBorderSize;
		pToolbar->m_cyBottomBorder=m_nBorderSize;

		pToolbar->SetWindowPos(NULL,0,0,0,0,
			SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_DRAWFRAME);
		pToolbar->RedrawWindow(NULL,NULL,
			RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_FRAME|RDW_ALLCHILDREN);

		CFrameWnd* pFrameWnd=pToolbar->GetDockingFrame();
		if(pFrameWnd!=NULL)
			pFrameWnd->RecalcLayout();
	}
}

void COXCustomizeToolbarsPage::OnEndlabeleditTreeToolbars(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo=(TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult=FALSE;

	CString sNewTitle=pTVDispInfo->item.pszText;
	if(sNewTitle.IsEmpty())
		return;

	ASSERT(pTVDispInfo!=NULL);
	ASSERT(pTVDispInfo->item.hItem!=NULL);
	ASSERT(pTVDispInfo->item.lParam!=NULL);

	CToolBar* pToolbar=(CToolBar*)pTVDispInfo->item.lParam;
	ASSERT(!pToolbar->IsKindOf(RUNTIME_CLASS(COXMenuBar)));

	*pResult=TRUE;
	pToolbar->SetWindowText(sNewTitle);

	CFrameWnd* pFrameWnd=pToolbar->GetParentFrame();
	if(pFrameWnd!=NULL && pFrameWnd->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)))
		pFrameWnd->SetWindowText(sNewTitle);
}

void COXCustomizeToolbarsPage::OnOptionChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMOPTIONTREE* pNMOptionTree=(NMOPTIONTREE*)pNMHDR;
	ASSERT(pNMOptionTree!=NULL);
	// TODO: Add your control notification handler code here

	*pResult=0;

	CWnd* pWnd=AfxGetMainWnd();
	ASSERT(pWnd!=NULL);
	CFrameWnd* pFrameWnd=DYNAMIC_DOWNCAST(CFrameWnd,pWnd);
	ASSERT(pFrameWnd!=NULL);

	HTREEITEM hItem=pNMOptionTree->hItem;
	ASSERT(hItem!=NULL);
	CToolBar* pToolbar=(CToolBar*)m_treeToolbars.GetItemData(hItem);
	BOOL bIsVisible=(pNMOptionTree->nNewCheck==OTITEM_CHECKED);

#ifdef _DEBUG
	ASSERT(pToolbar!=NULL);
	int nToolbarIndex=-1;
	VERIFY(m_mapAssociatedToolbars.Lookup(pToolbar,nToolbarIndex));
	ASSERT(nToolbarIndex!=-1);
	OXCUSTOMIZE_TOOLBARINFO& rtbInfo=m_arrToolbars[nToolbarIndex];
	ASSERT(rtbInfo.m_pToolBar==pToolbar);
	BOOL bWasVisible=((pToolbar->GetStyle()&WS_VISIBLE)==WS_VISIBLE);
	ASSERT(bIsVisible==!bWasVisible);
#endif	//	_DEBUG

	pFrameWnd->ShowControlBar(pToolbar,bIsVisible,FALSE);
}

void COXCustomizeToolbarsPage::OnBeginlabeleditTreeToolbars(NMHDR* pNMHDR, 
															LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult=FALSE;

	ASSERT(pTVDispInfo!=NULL);
	ASSERT(pTVDispInfo->item.hItem!=NULL);
	ASSERT(pTVDispInfo->item.lParam!=NULL);

	CToolBar* pToolbar=(CToolBar*)pTVDispInfo->item.lParam;
	ASSERT(!pToolbar->IsKindOf(RUNTIME_CLASS(COXMenuBar)));

	int nToolbarIndex=-1;
	VERIFY(m_mapAssociatedToolbars.Lookup(pToolbar,nToolbarIndex));
	ASSERT(nToolbarIndex!=-1);
	OXCUSTOMIZE_TOOLBARINFO& rtbInfo=m_arrToolbars[nToolbarIndex];
	ASSERT(rtbInfo.m_pToolBar==pToolbar);
	if(!rtbInfo.m_bCustomMade)
		*pResult=TRUE;
}

void COXCustomizeToolbarsPage::OnSelchangedTreeToolbars(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	UNREFERENCED_PARAMETER(pNMTreeView);
	// TODO: Add your control notification handler code here

	UpdateState();
	
	*pResult = 0;
}

