// OXCustomizeCoolControlsPage.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXCustomizeCoolControlsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int IDC_CHECK_CONTROLTYPE_BASE=1;


BOOL COXCustomizeCoolControlOrganizer::IsQualified(HWND hWnd, LPCTSTR lpszWndClass) 
{
	ASSERT(m_pCustomizeCoolControlsPage!=NULL);
	if(!COXCoolControlOrganizer::IsQualified(hWnd,lpszWndClass))
		return FALSE;
	return m_pCustomizeCoolControlsPage->
		IsQualifiedForCoolControl(hWnd,lpszWndClass);
}


IMPLEMENT_DYNCREATE(COXCustomizeCoolControlsPage, COXCustomizePage)

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeCoolControlsPage dialog

COXCustomizeCoolControlsPage::COXCustomizeCoolControlsPage()
{
	//{{AFX_DATA_INIT(COXCustomizeCoolControlsPage)
	m_bConvertToCool = FALSE;
	//}}AFX_DATA_INIT
	m_nDialogID=IDD;
	m_sProfileName.Empty();

	m_mapControls.SetAt(0,_T("Edit box"));
	m_mapControls.SetAt(1,_T("Button"));
	m_mapControls.SetAt(2,_T("Combo box"));
	m_mapControls.SetAt(3,_T("Tree control"));
	m_mapControls.SetAt(4,_T("List control"));
	m_mapControls.SetAt(5,_T("List box"));
	m_mapControls.SetAt(6,_T("Spin button"));
	m_mapControls.SetAt(7,_T("Scroll bar"));
	m_mapControls.SetAt(8,_T("Progress bar"));
	m_mapControls.SetAt(9,_T("Slider control"));
	m_mapControls.SetAt(10,_T("Hot key control"));
	m_mapControls.SetAt(11,_T("Rich edit control"));
#if _MFC_VER > 0x0421
	m_mapControls.SetAt(12,_T("Date-time picker"));
	m_mapControls.SetAt(13,_T("Month calendar"));
	m_mapControls.SetAt(14,_T("IP address edit box"));
#endif	//	_MFC_VER > 0x0421

	m_mapControlWndClass.SetAt(EDIT_CLASSNAME,0);
	m_mapControlWndClass.SetAt(BUTTON_CLASSNAME,1);
	m_mapControlWndClass.SetAt(SPIN_CLASSNAME,6);
	m_mapControlWndClass.SetAt(HOTKEY_CLASSNAME,10);
	m_mapControlWndClass.SetAt(COMBOBOX_CLASSNAME,2);
	m_mapControlWndClass.SetAt(SCROLLBAR_CLASSNAME,7);
	m_mapControlWndClass.SetAt(PROGRESS_CLASSNAME,8);
	m_mapControlWndClass.SetAt(SLIDER_CLASSNAME,9);
	m_mapControlWndClass.SetAt(LISTVIEW_CLASSNAME,4);
	m_mapControlWndClass.SetAt(TREEVIEW_CLASSNAME,3);
	m_mapControlWndClass.SetAt(LISTBOX_CLASSNAME,5);
	m_mapControlWndClass.SetAt(RICHEDIT_CLASSNAME,11);
#if _MFC_VER > 0x0421
	m_mapControlWndClass.SetAt(DATETIME_CLASSNAME,12);
	m_mapControlWndClass.SetAt(MONTHCAL_CLASSNAME,13);
	m_mapControlWndClass.SetAt(IPADDRESS_CLASSNAME,14);
#endif	//	_MFC_VER > 0x0421

	for(int nIndex=0; nIndex<m_mapControls.GetCount(); nIndex++)
	{
		m_mapControlState.SetAt(nIndex,OTITEM_CHECKED);
	}

	m_coolControlOrganizer.m_pCustomizeCoolControlsPage=this;
}


COXCustomizeCoolControlsPage::~COXCustomizeCoolControlsPage()
{
}


BOOL COXCustomizeCoolControlsPage::
InitializeCoolControls(BOOL bConvertToCool/*=TRUE*/,
					   LPCTSTR lpszProfileName/*=_T("CustomizeCoolControls")*/)
{
	m_bConvertToCool=bConvertToCool;

	CWinApp* pApp=AfxGetApp();
	ASSERT(pApp!=NULL);

	if(lpszProfileName!=NULL)
	{
		m_sProfileName=lpszProfileName;
		m_bConvertToCool=(BOOL)pApp->GetProfileInt(m_sProfileName,
			_T("ConvertCommonToCool"),m_bConvertToCool);
	}

	if(m_bConvertToCool)
	{
		CWnd* pWnd=AfxGetMainWnd();
		ASSERT(pWnd!=NULL && ::IsWindow(pWnd->GetSafeHwnd()));
		if(!m_coolControlOrganizer.AttachAllInThread())
			return FALSE;

		if(!m_sProfileName.IsEmpty())
		{
			for(int nIndex=0; nIndex<m_mapControls.GetCount(); nIndex++)
			{
				CString sItem;
				sItem.Format(_T("ControlTypeState_%d"),nIndex);
				int nState=-1;
				VERIFY(m_mapControlState.Lookup(nIndex,nState));
				ASSERT(nState!=-1);
				m_mapControlState.SetAt(nIndex,
					pApp->GetProfileInt(m_sProfileName,sItem,nState));
			}
		}
	}

	return TRUE;
}


BOOL COXCustomizeCoolControlsPage::OnCloseManager(BOOL bIsOk) 
{	
	CWinApp* pApp=AfxGetApp();
	ASSERT(pApp!=NULL);

	if(bIsOk)
	{
		if(!ApplyChanges())
			return FALSE;
		if(!m_sProfileName.IsEmpty())
		{
			pApp->WriteProfileInt(m_sProfileName,
				_T("ConvertCommonToCool"),m_bConvertToCool);

			for(int nIndex=0; nIndex<m_mapControls.GetCount(); nIndex++)
			{
				CString sItem;
				sItem.Format(_T("ControlTypeState_%d"),nIndex);
				pApp->WriteProfileInt(m_sProfileName,sItem,
					m_treeControlTypes.GetCheck(IDC_CHECK_CONTROLTYPE_BASE+nIndex));
			}

			m_treeControlTypes.SaveState(m_sProfileName,_T("TreeControlTypes"));
		}
	}
	else
	{
		if(!m_sProfileName.IsEmpty())
		{
			m_bConvertToCool=(BOOL)pApp->GetProfileInt(m_sProfileName,
				_T("ConvertCommonToCool"),m_bConvertToCool);
			UpdateData(FALSE);
			m_treeControlTypes.LoadState(m_sProfileName,_T("TreeControlTypes"));
			VERIFY(ApplyChanges());
		}
	}

	return TRUE;
}


void COXCustomizeCoolControlsPage::DoDataExchange(CDataExchange* pDX)
{
	COXCustomizePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXCustomizeCoolControlsPage)
	DDX_Control(pDX, IDC_OX_TREE_CONTROL_TYPES, m_treeControlTypes);
	DDX_Check(pDX, IDC_OX_CHECK_CONVERT, m_bConvertToCool);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COXCustomizeCoolControlsPage, COXCustomizePage)
	//{{AFX_MSG_MAP(COXCustomizeCoolControlsPage)
	ON_BN_CLICKED(IDC_OX_BUTTON_CONVERT_ALL, OnButtonConvertAll)
	ON_BN_CLICKED(IDC_OX_CHECK_CONVERT, OnCheckConvert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeCoolControlsPage message handlers


BOOL COXCustomizeCoolControlsPage::Load(const COXCustomizeManager* pCustomizeManager)
{
	// Call default implementation. It will load this demo dialog as the first 
	// page and will create About and CodeSample pages if specified.
	if(!COXCustomizePage::Load(pCustomizeManager))
		return FALSE;

	return TRUE;
}


void COXCustomizeCoolControlsPage::Unload()
{
	// add here code for cleaning up all objects created by demo
	//
	//
	//////////////////////////////////////////////////////////////////////////

	COXCustomizePage::Unload();
}

void COXCustomizeCoolControlsPage::OnInitDialog()
{
	// must call default implementation
	COXCustomizePage::OnInitDialog();

	// add here initialization code for your demo dialog. Treat it as a
	// normal CDialog::OnInitDialog function

	// populate tree control
	m_treeControlTypes.DeleteAllItems();

	// root item
	int nImageIndex=m_treeControlTypes.AddImage(IDB_OX_CUSTOMIZECOOLCONTROLS_CONTROLS,
		RGB(192,192,192));
	ASSERT(nImageIndex!=-1);
	CString sItem;
	VERIFY(sItem.LoadString(IDS_OX_CSTMZECOOLCONTROLCONVERT));//"Convert the following common controls"
	HTREEITEM hRootItem=m_treeControlTypes.
		AddControlGroup(sItem,
		NULL,TRUE,nImageIndex,nImageIndex);
	ASSERT(hRootItem!=NULL);
	m_treeControlTypes.SetItemState(hRootItem,TVIS_BOLD,TVIS_BOLD);

	for(int nIndex=0; nIndex<m_mapControls.GetCount(); nIndex++)
	{
		CString sControlType;
		VERIFY(m_mapControls.Lookup(nIndex,sControlType));

		int nState=-1;
		VERIFY(m_mapControlState.Lookup(nIndex,nState));
		ASSERT(nState!=-1);

		HTREEITEM hItem=m_treeControlTypes.
			AddCheckBox(IDC_CHECK_CONTROLTYPE_BASE+nIndex,
			sControlType,hRootItem,nState,TVI_SORT);
		ASSERT(hItem!=NULL);
	}

	if(!m_sProfileName.IsEmpty())
	{
		m_treeControlTypes.LoadState(m_sProfileName,_T("TreeControlTypes"));
	}

	// layout
	//
	m_LayoutManager.TieChild(IDC_OX_BUTTON_CONVERT_ALL,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_treeControlTypes,OX_LMS_ANY,OX_LMT_SAME);
	//
	///////////////////////////////////////

	m_bInitialized=TRUE;

	UpdateData(FALSE);
	OnCheckConvert();
}

void COXCustomizeCoolControlsPage::OnButtonConvertAll() 
{
	// TODO: Add your control notification handler code here
	ASSERT(m_bConvertToCool);
	
	for(int nIndex=0; nIndex<m_mapControls.GetCount(); nIndex++)
	{
		m_treeControlTypes.SetCheck(IDC_CHECK_CONTROLTYPE_BASE+nIndex,TRUE);
	}
}


BOOL COXCustomizeCoolControlsPage::ApplyChanges()
{
	if(!UpdateData(TRUE))
		return FALSE;

	for(int nIndex=0; nIndex<m_mapControls.GetCount(); nIndex++)
	{
		m_mapControlState.SetAt(nIndex,
			m_treeControlTypes.GetCheck(IDC_CHECK_CONTROLTYPE_BASE+nIndex));
	}

	if(m_coolControlOrganizer.IsAttachedAllInThread())
	{
		if(!m_coolControlOrganizer.DetachAllInThread())
			return FALSE;
	}

	if(m_bConvertToCool)
	{
		if(!m_coolControlOrganizer.AttachAllInThread())
			return FALSE;
	}

	return TRUE;
}


void COXCustomizeCoolControlsPage::OnCheckConvert() 
{
	// TODO: Add your control notification handler code here
	
	if(!UpdateData(TRUE))
		return;

	m_treeControlTypes.EnableWindow(m_bConvertToCool);
	GetDlgItem(IDC_OX_BUTTON_CONVERT_ALL)->EnableWindow(m_bConvertToCool);
}


BOOL COXCustomizeCoolControlsPage::IsQualifiedForCoolControl(HWND hWnd, 
															 LPCTSTR lpszWndClass)
{
	UNREFERENCED_PARAMETER(hWnd);

	int nType=-1;
	VERIFY(m_mapControlWndClass.Lookup(_T("ComboBox"),nType));

	int nControlType=-1;
	if(m_mapControlWndClass.Lookup(lpszWndClass,nControlType))
	{
		ASSERT(nControlType!=-1);
		int nState=-1;
		VERIFY(m_mapControlState.Lookup(nControlType,nState));
		ASSERT(nState!=-1);
		if(nState==OTITEM_UNCHECKED)
			return FALSE;
	}

	return TRUE;
}
