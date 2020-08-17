// ==========================================================================
//			Class Implementation: COXCustomizeInstanceManagerPage
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
#include "OXCustomizeInstanceManagerPage.h"
#include "UTB64Bit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT IDT_OXCUSTOMIZEINSTANCEMANAGER_CHECK_INTANCES=278;
const UINT ID_OXCUSTOMIZEINSTANCEMANAGER_CHECK_INTANCES_DELAY=1000;

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeInstanceManagerPage dialog

IMPLEMENT_DYNCREATE(COXCustomizeInstanceManagerPage, COXCustomizePage)

COXCustomizeInstanceManagerPage::COXCustomizeInstanceManagerPage() :
	m_instanceManager(GetCurrentInstanceName())
{
	//{{AFX_DATA_INIT(COXCustomizeInstanceManagerPage)
	m_nInstanceLimitType = 0;
	m_nMaxAllowedInstanceNumber = 2;
	//}}AFX_DATA_INIT

	m_nDialogID=IDD;
	m_sProfileName.Empty();

	m_nTimerCheckInstancesID=0;
}


COXCustomizeInstanceManagerPage::~COXCustomizeInstanceManagerPage()
{
}


BOOL COXCustomizeInstanceManagerPage::
InitializeInstanceManager(int nMaxAllowedInstances/*=OX_MAX_NUM_INSTANCES_IN_LIST*/,
						  LPCTSTR lpszProfileName/*=_T("CustomizeInstanceManager")*/)
{
	ASSERT(nMaxAllowedInstances>0 && 
		nMaxAllowedInstances<=OX_MAX_NUM_INSTANCES_IN_LIST);
	if(nMaxAllowedInstances<1 || 
		nMaxAllowedInstances>OX_MAX_NUM_INSTANCES_IN_LIST)
	{
		TRACE(_T("COXCustomizeInstanceManagerPage::InitializeInstanceManager: the maximum allowed number of instances=%d is out of range: %d...%d\n"),nMaxAllowedInstances,1,OX_MAX_NUM_INSTANCES_IN_LIST);
		return FALSE;
	}

	m_nMaxAllowedInstanceNumber=nMaxAllowedInstances;

	CWinApp* pApp=AfxGetApp();
	ASSERT(pApp!=NULL);

	if(lpszProfileName!=NULL)
	{
		m_sProfileName=lpszProfileName;
	
		m_nMaxAllowedInstanceNumber=pApp->GetProfileInt(m_sProfileName,
			_T("MaxAllowedInstanceNumber"),m_nMaxAllowedInstanceNumber);
	}

	ASSERT(m_nMaxAllowedInstanceNumber>0 && 
		m_nMaxAllowedInstanceNumber<=OX_MAX_NUM_INSTANCES_IN_LIST);
	m_instanceManager.SetMaxAllowedInstances(m_nMaxAllowedInstanceNumber);
	
	m_nInstanceLimitType=
		(m_nMaxAllowedInstanceNumber==OX_MAX_NUM_INSTANCES_IN_LIST ? 0 : 
		(m_nMaxAllowedInstanceNumber==1 ? 1 : 2));

	return TRUE;
}

BOOL COXCustomizeInstanceManagerPage::OnCloseManager(BOOL bIsOk) 
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
				_T("MaxAllowedInstanceNumber"),m_nMaxAllowedInstanceNumber);
		}
	}
	else
	{
		if(!m_sProfileName.IsEmpty())
		{
			m_nMaxAllowedInstanceNumber=pApp->GetProfileInt(m_sProfileName,
				_T("MaxAllowedInstanceNumber"),m_nMaxAllowedInstanceNumber);
			m_instanceManager.SetMaxAllowedInstances(m_nMaxAllowedInstanceNumber);
		}
	}

	return TRUE;
}


void COXCustomizeInstanceManagerPage::DoDataExchange(CDataExchange* pDX)
{
	COXCustomizePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXCustomizeInstanceManagerPage)
	DDX_Control(pDX, IDC_OX_SPIN_LIMIT_NUMBER, m_spinMaxAllowedInstances);
	DDX_Control(pDX, IDC_OX_LIST_INSTANCES, m_listInstances);
	DDX_Control(pDX, IDC_OX_STATIC_INSTANCE_COUNT, m_staticInstanceCount);
	DDX_Control(pDX, IDC_OX_SEPARATOR_OPTIONS, m_sepOptions);
	DDX_Control(pDX, IDC_OX_SEPARATOR_OPERATIONS, m_sepOperations);
	DDX_Control(pDX, IDC_OX_SEPARATOR_INSTANCES, m_sepInstances);
	DDX_Radio(pDX, IDC_OX_RADIO_MAXNUMBER_INSTANCE_NOLIMIT, m_nInstanceLimitType);
	DDX_Text(pDX, IDC_OX_EDIT_LIMIT_NUMBER, m_nMaxAllowedInstanceNumber);
	DDV_MinMaxUInt(pDX, m_nMaxAllowedInstanceNumber, 1, OX_MAX_NUM_INSTANCES_IN_LIST);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COXCustomizeInstanceManagerPage, COXCustomizePage)
	//{{AFX_MSG_MAP(COXCustomizeInstanceManagerPage)
	ON_BN_CLICKED(IDC_OX_RADIO_MAXNUMBER_INSTANCE_LIMITTO, OnRadioMaxnumberInstanceLimitto)
	ON_BN_CLICKED(IDC_OX_RADIO_MAXNUMBER_INSTANCE_NOLIMIT, OnRadioMaxnumberInstanceNolimit)
	ON_BN_CLICKED(IDC_OX_RADIO_MAXNUMBER_INSTANCE_ONLYONE, OnRadioMaxnumberInstanceOnlyone)
	ON_EN_CHANGE(IDC_OX_EDIT_LIMIT_NUMBER, OnChangeEditLimitNumber)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_OX_LIST_INSTANCES, OnItemchangedListInstances)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_OX_BUTTON_BRING_TO_FRONT, OnButtonBringToFront)
	ON_BN_CLICKED(IDC_OX_BUTTON_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_OX_BUTTON_MAXIMIZE, OnButtonMaximize)
	ON_BN_CLICKED(IDC_OX_BUTTON_MINIMIZE, OnButtonMinimize)
	ON_BN_CLICKED(IDC_OX_BUTTON_RESTORE, OnButtonRestore)
	ON_BN_CLICKED(IDC_OX_BUTTON_SWITCH_TO, OnButtonSwitchTo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CString COXCustomizeInstanceManagerPage::GetCurrentInstanceName()
{
	CString sFileName(_T(""));
	::GetModuleFileName(NULL,sFileName.GetBuffer(MAX_PATH),MAX_PATH);
	sFileName.ReleaseBuffer();
#if _MFC_VER>0x0421
	sFileName.Remove(_T('\\'));
#else
	int nPos=sFileName.Find(_T('\\'));
	while(nPos!=-1)
	{
		sFileName=sFileName.Left(nPos)+sFileName.Mid(nPos+1);
		nPos=sFileName.Find(_T('\\'));
	}
#endif	//	_MFC_VER>0x0421
	return sFileName;
}

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeInstanceManagerPage message handlers

BOOL COXCustomizeInstanceManagerPage::Load(const COXCustomizeManager* pCustomizeManager)
{
	// Call default implementation. It will load this demo dialog as the first 
	// page and will create About and CodeSample pages if specified.
	if(!COXCustomizePage::Load(pCustomizeManager))
		return FALSE;

	return TRUE;
}


void COXCustomizeInstanceManagerPage::Unload()
{
	// add here code for cleaning up all objects created by demo
	//
	//
	//////////////////////////////////////////////////////////////////////////

	COXCustomizePage::Unload();
}

void COXCustomizeInstanceManagerPage::OnInitDialog()
{
	// must call default implementation
	COXCustomizePage::OnInitDialog();

	// add here initialization code for your demo dialog. Treat it as a
	// normal COXCustomizePage::OnInitDialog function

	m_nTimerCheckInstancesID=SetTimer(IDT_OXCUSTOMIZEINSTANCEMANAGER_CHECK_INTANCES,
		ID_OXCUSTOMIZEINSTANCEMANAGER_CHECK_INTANCES_DELAY,NULL);
	ASSERT(m_nTimerCheckInstancesID!=0);

	m_spinMaxAllowedInstances.SetRange(1,OX_MAX_NUM_INSTANCES_IN_LIST);

	CString sItem;
	VERIFY(sItem.LoadString(IDS_OX_CSTMZEINSTANCEINSTANCE));
	m_listInstances.InsertColumn(0,sItem,LVCFMT_LEFT,100);//"Instance"
	VERIFY(sItem.LoadString(IDS_OX_CSTMZEINSTANCEPID));
	m_listInstances.InsertColumn(1,sItem,LVCFMT_CENTER,40);//"PID"
	VERIFY(sItem.LoadString(IDS_OX_CSTMZEINSTANCESTATUS));
	m_listInstances.InsertColumn(2,sItem,LVCFMT_CENTER,50);//"Status"
	PopulateInstancesList();

	m_staticInstanceCount.SetTextColor(RGB(0,0,128));
	CFont font;
	VERIFY(font.CreatePointFont(140,_T("Times New Roman")));
	m_staticInstanceCount.SetTextFont(&font);

	// layout
	//
	m_LayoutManager.TieChild(&m_sepOptions,OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_sepInstances,OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_listInstances,OX_LMS_ANY,OX_LMT_SAME);

	m_LayoutManager.TieChild(&m_sepOperations,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_BUTTON_CLOSE,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_BUTTON_SWITCH_TO,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_BUTTON_BRING_TO_FRONT,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_BUTTON_MAXIMIZE,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_BUTTON_MINIMIZE,OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(IDC_OX_BUTTON_RESTORE,OX_LMS_RIGHT,OX_LMT_SAME);
	//
	///////////////////////////////////////

	m_bInitialized=TRUE;

	UpdateData(FALSE);
	ShowVars();
}


BOOL COXCustomizeInstanceManagerPage::ApplyChanges()
{
	if(!UpdateData(TRUE))
		return FALSE;

	switch(m_nInstanceLimitType)
	{
	case 0:
		m_nMaxAllowedInstanceNumber=OX_MAX_NUM_INSTANCES_IN_LIST;
		break;
	case 1:
		m_nMaxAllowedInstanceNumber=1;
		break;
	}

	m_instanceManager.SetMaxAllowedInstances(m_nMaxAllowedInstanceNumber);

	return TRUE;
}


void COXCustomizeInstanceManagerPage::OnRadioMaxnumberInstanceLimitto() 
{
	// TODO: Add your control notification handler code here
	ShowVars();
}

void COXCustomizeInstanceManagerPage::OnRadioMaxnumberInstanceNolimit() 
{
	// TODO: Add your control notification handler code here
	ShowVars();
}

void COXCustomizeInstanceManagerPage::OnRadioMaxnumberInstanceOnlyone() 
{
	// TODO: Add your control notification handler code here
	ShowVars();
}


void COXCustomizeInstanceManagerPage::PopulateInstancesList()
{
	int nInstanceRunningCount=m_instanceManager.GetNumberOfInstances();
	CString sInstanceRunningCount;
	sInstanceRunningCount.Format(_T("%d"),nInstanceRunningCount);
	m_staticInstanceCount.SetWindowText(sInstanceRunningCount);

	int nSelectedItem=-1;
#if _MFC_VER > 0x0421
	POSITION pos=m_listInstances.GetFirstSelectedItemPosition();
#else
	ASSERT(::IsWindow(m_listInstances.m_hWnd));
	POSITION pos = (POSITION) (1+m_listInstances.GetNextItem(-1, LVIS_SELECTED)); 
#endif
	if(pos!=NULL)
	{
#if _MFC_VER > 0x0421
		int nSelectedItem=m_listInstances.GetNextSelectedItem(pos);
#else
		ASSERT(::IsWindow(m_listInstances.m_hWnd));
		int nSelectedItem = (int)pos-1;
		POSITION npos = (POSITION) (1+m_listInstances.GetNextItem(nSelectedItem, LVIS_SELECTED));
#endif
		ASSERT(pos==NULL);
		ASSERT(nSelectedItem!=-1);
	}

	m_listInstances.DeleteAllItems();
	// populate list of all running instances
	CDWordArray arrInstances;
	VERIFY(m_instanceManager.GetInstanceCollection(arrInstances));
	ASSERT(arrInstances.GetSize()==nInstanceRunningCount);
	DWORD dwCurrentPID=m_instanceManager.GetCurrentInstanceID();
	for(int nIndex=0; nIndex<nInstanceRunningCount; nIndex++)
	{
		CString sItem;
		sItem.Format(IDS_OX_CSTMZEINSTANCEINSTANCEF,nIndex+1);//"Instance %d"
		VERIFY(m_listInstances.InsertItem(nIndex,sItem)==nIndex);
		CString sPID;
		sPID.Format(_T("%d"),arrInstances[nIndex]);
		m_listInstances.SetItemText(nIndex,1,sPID);
		VERIFY(sItem.LoadString(IDS_OX_CSTMZEINSTANCECURRENT));//"Current"
		if(arrInstances[nIndex]==dwCurrentPID)
			m_listInstances.SetItemText(nIndex,2,sItem);
		m_listInstances.SetItemData(nIndex,(LPARAM)arrInstances[nIndex]);
	}

	if(nSelectedItem>=0 && nSelectedItem<m_listInstances.GetItemCount())
	{
		m_listInstances.SetItemState(nSelectedItem,LVIS_SELECTED|LVIS_FOCUSED,
			LVIS_SELECTED|LVIS_FOCUSED);
	}
}


void COXCustomizeInstanceManagerPage::ShowVars()
{
	if(!m_bInitialized)
		return;

	if(!UpdateData(TRUE))
		return;

	GetDlgItem(IDC_OX_EDIT_LIMIT_NUMBER)->EnableWindow(m_nInstanceLimitType==2);
	GetDlgItem(IDC_OX_SPIN_LIMIT_NUMBER)->EnableWindow(m_nInstanceLimitType==2);

#if _MFC_VER > 0x0421
	POSITION pos=m_listInstances.GetFirstSelectedItemPosition();
#else
	ASSERT(::IsWindow(m_listInstances.m_hWnd));
	POSITION pos = (POSITION) (1+m_listInstances.GetNextItem(-1, LVIS_SELECTED)); 
#endif
	if(pos!=NULL)
	{
#if _MFC_VER > 0x0421
		int nSelectedItem=m_listInstances.GetNextSelectedItem(pos);
#else
		ASSERT(::IsWindow(m_listInstances.m_hWnd));
		int nSelectedItem = (int)pos-1;
		POSITION npos = (POSITION) (1+m_listInstances.GetNextItem(nSelectedItem, LVIS_SELECTED));
#endif
		ASSERT(pos==NULL);
		ASSERT(nSelectedItem!=-1);

		DWORD_PTR dwPID=m_listInstances.GetItemData(nSelectedItem);
		BOOL bCurrentInstance=(dwPID==m_instanceManager.GetCurrentInstanceID());

		GetDlgItem(IDC_OX_BUTTON_CLOSE)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_OX_BUTTON_SWITCH_TO)->EnableWindow(!bCurrentInstance);
		GetDlgItem(IDC_OX_BUTTON_BRING_TO_FRONT)->EnableWindow(!bCurrentInstance);

		DWORD dwStyle=
			::GetWindowLongPtr(m_instanceManager.GetMainWindow(dwPID),GWL_STYLE);
		BOOL bSysMenu=(dwStyle&WS_SYSMENU)!=0;
		BOOL bMaximizeBtn=bSysMenu & ((dwStyle&WS_MAXIMIZEBOX)!=0);
		BOOL bCanMaximize=bMaximizeBtn & ((dwStyle&WS_MAXIMIZE)==0);
		BOOL bMinimizeBtn=bSysMenu & ((dwStyle&WS_MINIMIZEBOX)!=0);
		BOOL bCanMinimize=bMinimizeBtn & ((dwStyle&WS_MINIMIZE)==0);

		GetDlgItem(IDC_OX_BUTTON_MAXIMIZE)->EnableWindow(bCanMaximize);
		GetDlgItem(IDC_OX_BUTTON_MINIMIZE)->EnableWindow(bCanMinimize);
		GetDlgItem(IDC_OX_BUTTON_RESTORE)->
			EnableWindow(!((bCanMaximize && bCanMinimize) ||
			(!bMaximizeBtn && bCanMinimize) || !bSysMenu));
	}
	else
	{
		GetDlgItem(IDC_OX_BUTTON_CLOSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_OX_BUTTON_SWITCH_TO)->EnableWindow(FALSE);
		GetDlgItem(IDC_OX_BUTTON_BRING_TO_FRONT)->EnableWindow(FALSE);
		GetDlgItem(IDC_OX_BUTTON_MAXIMIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_OX_BUTTON_MINIMIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_OX_BUTTON_RESTORE)->EnableWindow(FALSE);
	}
}



void COXCustomizeInstanceManagerPage::OnChangeEditLimitNumber() 
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
}

void COXCustomizeInstanceManagerPage::OnItemchangedListInstances(NMHDR* pNMHDR, 
																 LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	UNREFERENCED_PARAMETER(pNMListView);
	// TODO: Add your control notification handler code here

	ShowVars();
	
	*pResult = 0;
}

void COXCustomizeInstanceManagerPage::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==m_nTimerCheckInstancesID)
	{
		int nNewInstanceCount=m_instanceManager.GetNumberOfInstances();
		if(nNewInstanceCount!=m_listInstances.GetItemCount())
		{
			PopulateInstancesList();
		}
		return;
	}
	
	COXCustomizePage::OnTimer(nIDEvent);
}

void COXCustomizeInstanceManagerPage::OnDestroy() 
{
	COXCustomizePage::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_nTimerCheckInstancesID!=0)
	{
		KillTimer(m_nTimerCheckInstancesID);
		m_nTimerCheckInstancesID=0;
	}
}

void COXCustomizeInstanceManagerPage::OnButtonBringToFront() 
{
	// TODO: Add your control notification handler code here
#if _MFC_VER > 0x0421
	POSITION pos=m_listInstances.GetFirstSelectedItemPosition();
#else
	ASSERT(::IsWindow(m_listInstances.m_hWnd));
	POSITION pos = (POSITION) (1+m_listInstances.GetNextItem(-1, LVIS_SELECTED)); 
#endif
	ASSERT(pos!=NULL);
#if _MFC_VER > 0x0421
		int nSelectedItem=m_listInstances.GetNextSelectedItem(pos);
#else
		ASSERT(::IsWindow(m_listInstances.m_hWnd));
		int nSelectedItem = (int)pos-1;
		POSITION npos = (POSITION) (1+m_listInstances.GetNextItem(nSelectedItem, LVIS_SELECTED));
#endif
	ASSERT(pos==NULL);
	ASSERT(nSelectedItem!=-1);

	DWORD_PTR dwPID=m_listInstances.GetItemData(nSelectedItem);
	BOOL bCurrentInstance=(dwPID==m_instanceManager.GetCurrentInstanceID());
	ASSERT(!bCurrentInstance);
	HWND hWnd= (HWND)(DWORD_PTR)m_instanceManager.GetMainWindow(dwPID);

	if(::GetWindowLongPtr(hWnd,GWL_STYLE)&WS_MINIMIZE)
	{
		OnButtonRestore();
		AfxGetMainWnd()->SetForegroundWindow();
	}
	else
	{
		::SetWindowPos(hWnd,AfxGetMainWnd()->GetSafeHwnd(),0,0,0,0,
			SWP_SHOWWINDOW|SWP_ASYNCWINDOWPOS|SWP_NOACTIVATE|
			SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
	}

	ShowVars();
}

void COXCustomizeInstanceManagerPage::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
#if _MFC_VER > 0x0421
	POSITION pos=m_listInstances.GetFirstSelectedItemPosition();
#else
	ASSERT(::IsWindow(m_listInstances.m_hWnd));
	POSITION pos = (POSITION) (1+m_listInstances.GetNextItem(-1, LVIS_SELECTED)); 
#endif
	ASSERT(pos!=NULL);
#if _MFC_VER > 0x0421
		int nSelectedItem=m_listInstances.GetNextSelectedItem(pos);
#else
		ASSERT(::IsWindow(m_listInstances.m_hWnd));
		int nSelectedItem = (int)pos-1;
		POSITION npos = (POSITION) (1+m_listInstances.GetNextItem(nSelectedItem, LVIS_SELECTED));
#endif
	ASSERT(pos==NULL);
	ASSERT(nSelectedItem!=-1);

	DWORD_PTR dwPID=m_listInstances.GetItemData(nSelectedItem);

	VERIFY(m_instanceManager.CloseInstance(dwPID));

	ShowVars();
}

void COXCustomizeInstanceManagerPage::OnButtonMaximize() 
{
	// TODO: Add your control notification handler code here
#if _MFC_VER > 0x0421
	POSITION pos=m_listInstances.GetFirstSelectedItemPosition();
#else
	ASSERT(::IsWindow(m_listInstances.m_hWnd));
	POSITION pos = (POSITION) (1+m_listInstances.GetNextItem(-1, LVIS_SELECTED)); 
#endif
	ASSERT(pos!=NULL);
#if _MFC_VER > 0x0421
		int nSelectedItem=m_listInstances.GetNextSelectedItem(pos);
#else
		ASSERT(::IsWindow(m_listInstances.m_hWnd));
		int nSelectedItem = (int)pos-1;
		POSITION npos = (POSITION) (1+m_listInstances.GetNextItem(nSelectedItem, LVIS_SELECTED));
#endif
	ASSERT(pos==NULL);
	ASSERT(nSelectedItem!=-1);

	DWORD_PTR dwPID=m_listInstances.GetItemData(nSelectedItem);
	HWND hWnd=m_instanceManager.GetMainWindow(dwPID);

	if(::GetWindowLongPtr(hWnd,GWL_STYLE)&WS_MINIMIZE)
	{
		::SendMessage(hWnd,WM_SYSCOMMAND,SC_MAXIMIZE,(LPARAM)hWnd);
		AfxGetMainWnd()->SetForegroundWindow();
	}
	else
	{
		::SendMessage(hWnd,WM_SYSCOMMAND,SC_MAXIMIZE,(LPARAM)hWnd);
	}

	ShowVars();
}

void COXCustomizeInstanceManagerPage::OnButtonMinimize() 
{
	// TODO: Add your control notification handler code here
#if _MFC_VER > 0x0421
	POSITION pos=m_listInstances.GetFirstSelectedItemPosition();
#else
	ASSERT(::IsWindow(m_listInstances.m_hWnd));
	POSITION pos = (POSITION) (1+m_listInstances.GetNextItem(-1, LVIS_SELECTED)); 
#endif
	ASSERT(pos!=NULL);
#if _MFC_VER > 0x0421
		int nSelectedItem=m_listInstances.GetNextSelectedItem(pos);
#else
		ASSERT(::IsWindow(m_listInstances.m_hWnd));
		int nSelectedItem = (int)pos-1;
		POSITION npos = (POSITION) (1+m_listInstances.GetNextItem(nSelectedItem, LVIS_SELECTED));
#endif
	ASSERT(pos==NULL);
	ASSERT(nSelectedItem!=-1);

	DWORD_PTR dwPID=m_listInstances.GetItemData(nSelectedItem);
	HWND hWnd=m_instanceManager.GetMainWindow(dwPID);

	::SendMessage(hWnd,WM_SYSCOMMAND,SC_MINIMIZE,(LPARAM)hWnd);

	ShowVars();
}

void COXCustomizeInstanceManagerPage::OnButtonRestore() 
{
	// TODO: Add your control notification handler code here
#if _MFC_VER > 0x0421
	POSITION pos=m_listInstances.GetFirstSelectedItemPosition();
#else
	ASSERT(::IsWindow(m_listInstances.m_hWnd));
	POSITION pos = (POSITION) (1+m_listInstances.GetNextItem(-1, LVIS_SELECTED)); 
#endif
	ASSERT(pos!=NULL);
#if _MFC_VER > 0x0421
		int nSelectedItem=m_listInstances.GetNextSelectedItem(pos);
#else
		ASSERT(::IsWindow(m_listInstances.m_hWnd));
		int nSelectedItem = (int)pos-1;
		POSITION npos = (POSITION) (1+m_listInstances.GetNextItem(nSelectedItem, LVIS_SELECTED));
#endif
	ASSERT(pos==NULL);
	ASSERT(nSelectedItem!=-1);

	DWORD_PTR dwPID=m_listInstances.GetItemData(nSelectedItem);
	HWND hWnd=m_instanceManager.GetMainWindow(dwPID);

	if(::GetWindowLongPtr(hWnd,GWL_STYLE)&WS_MINIMIZE)
	{
		::SendMessage(hWnd,WM_SYSCOMMAND,SC_RESTORE,(LPARAM)hWnd);
		AfxGetMainWnd()->SetForegroundWindow();
	}
	else
	{
		::SendMessage(hWnd,WM_SYSCOMMAND,SC_RESTORE,(LPARAM)hWnd);
	}

	ShowVars();
}

void COXCustomizeInstanceManagerPage::OnButtonSwitchTo() 
{
	// TODO: Add your control notification handler code here
#if _MFC_VER > 0x0421
	POSITION pos=m_listInstances.GetFirstSelectedItemPosition();
#else
	ASSERT(::IsWindow(m_listInstances.m_hWnd));
	POSITION pos = (POSITION) (1+m_listInstances.GetNextItem(-1, LVIS_SELECTED)); 
#endif
	ASSERT(pos!=NULL);
#if _MFC_VER > 0x0421
		int nSelectedItem=m_listInstances.GetNextSelectedItem(pos);
#else
		ASSERT(::IsWindow(m_listInstances.m_hWnd));
		int nSelectedItem = (int)pos-1;
		POSITION npos = (POSITION) (1+m_listInstances.GetNextItem(nSelectedItem, LVIS_SELECTED));
#endif
	ASSERT(pos==NULL);
	ASSERT(nSelectedItem!=-1);

	DWORD_PTR dwPID=m_listInstances.GetItemData(nSelectedItem);
	BOOL bCurrentInstance=(dwPID==m_instanceManager.GetCurrentInstanceID());
	ASSERT(!bCurrentInstance);
	HWND hWnd=m_instanceManager.GetMainWindow(dwPID);

	::SetForegroundWindow(hWnd);
	if(::GetWindowLongPtr(hWnd,GWL_STYLE)&WS_MINIMIZE)
	{
		::SendMessage(hWnd,WM_SYSCOMMAND,SC_RESTORE,(LPARAM)hWnd);
	}

	ShowVars();
}
