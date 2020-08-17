// OXCustomizeIntelliMousePage.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXCustomizeIntelliMousePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL COXCustomizeIntelliMouseOrganizer::IsQualified(HWND hWnd, LPCTSTR lpszWndClass) 
{
	ASSERT(IsAttachedAllInThread());
	ASSERT(m_pCustomizeIntelliMousePage!=NULL);

	if(!COXIntelliMouseOrganizer::IsQualified(hWnd,lpszWndClass))
		return FALSE;

	return m_pCustomizeIntelliMousePage->
		IsQualifiedForIntelliMouseSupport(hWnd,lpszWndClass);
}


/////////////////////////////////////////////////////////////////////////////
// COXCustomizeIntelliMousePage dialog

IMPLEMENT_DYNCREATE(COXCustomizeIntelliMousePage, COXCustomizePage)

COXCustomizeIntelliMousePage::COXCustomizeIntelliMousePage()
{
	//{{AFX_DATA_INIT(COXCustomizeIntelliMousePage)
	m_bAltKey = FALSE;
	m_bCtrlKey = FALSE;
	m_bOnlyViews = FALSE;
	m_bShiftKey = FALSE;
	m_bSupportIntelliMouse = TRUE;
	m_nStartEvent = 0;
	//}}AFX_DATA_INIT

	m_nDialogID=IDD;
	m_sProfileName.Empty();

	static CString sStr1;
	static CString sStr2;
	static CString sStr3;
	static CString sStr4;
	static CString sStr5;
	static CString sStr6;
	static CString sStr7;
	static CString sStr8;
	static CString sStr9;

	VERIFY(sStr1.LoadString(IDS_OX_CSTMZEINTELLILBDOWN	));//"Left button down"
	VERIFY(sStr2.LoadString(IDS_OX_CSTMZEINTELLIMBDOWN	));//"Middle button down"
	VERIFY(sStr3.LoadString(IDS_OX_CSTMZEINTELLIRBDOWN	));//"Right button down"
	VERIFY(sStr4.LoadString(IDS_OX_CSTMZEINTELLILBUP	));//"Left button up"
	VERIFY(sStr5.LoadString(IDS_OX_CSTMZEINTELLIMBUP	));//"Middle button up"
	VERIFY(sStr6.LoadString(IDS_OX_CSTMZEINTELLIRBUP	));//"Right button up"
	VERIFY(sStr7.LoadString(IDS_OX_CSTMZEINTELLILBDBL	));//"Left button double click"
	VERIFY(sStr8.LoadString(IDS_OX_CSTMZEINTELLIMBDBL	));//"Middle button double click"
	VERIFY(sStr9.LoadString(IDS_OX_CSTMZEINTELLIRBDBL	));//"Right button double click"
	
	static LPCTSTR str1[] = {(LPCTSTR) sStr1, _T("WM_LBUTTONDOWN")};
	static LPCTSTR str2[] = {(LPCTSTR) sStr2, _T("WM_MBUTTONDOWN")};
	static LPCTSTR str3[] = {(LPCTSTR) sStr3, _T("WM_RBUTTONDOWN")};
	static LPCTSTR str4[] = {(LPCTSTR) sStr4, _T("WM_LBUTTONUP")};
	static LPCTSTR str5[] = {(LPCTSTR) sStr5, _T("WM_MBUTTONUP")};
	static LPCTSTR str6[] = {(LPCTSTR) sStr6, _T("WM_RBUTTONUP")};
	static LPCTSTR str7[] = {(LPCTSTR) sStr7, _T("WM_LBUTTONDBLCLK")};
	static LPCTSTR str8[] = {(LPCTSTR) sStr8, _T("WM_MBUTTONDBLCLK")};
	static LPCTSTR str9[] = {(LPCTSTR) sStr9, _T("WM_RBUTTONDBLCLK")};

	m_mapStartEventDescriptions.SetAt(WM_LBUTTONDOWN,(LPCTSTR*) str1);
	m_mapStartEventDescriptions.SetAt(WM_MBUTTONDOWN,(LPCTSTR*) str2);
	m_mapStartEventDescriptions.SetAt(WM_RBUTTONDOWN,(LPCTSTR*) str3);
	m_mapStartEventDescriptions.SetAt(WM_LBUTTONUP,(LPCTSTR*) str4);
	m_mapStartEventDescriptions.SetAt(WM_MBUTTONUP,(LPCTSTR*) str5);
	m_mapStartEventDescriptions.SetAt(WM_RBUTTONUP,(LPCTSTR*) str6);
	m_mapStartEventDescriptions.SetAt(WM_LBUTTONDBLCLK,(LPCTSTR*) str7);
	m_mapStartEventDescriptions.SetAt(WM_MBUTTONDBLCLK,(LPCTSTR*) str8);
	m_mapStartEventDescriptions.SetAt(WM_RBUTTONDBLCLK,(LPCTSTR*) str9);

	m_arrStartEvents.Add(WM_LBUTTONDOWN);
	m_arrStartEvents.Add(WM_MBUTTONDOWN);
	m_arrStartEvents.Add(WM_RBUTTONDOWN);
	m_arrStartEvents.Add(WM_LBUTTONUP);
	m_arrStartEvents.Add(WM_MBUTTONUP);
	m_arrStartEvents.Add(WM_RBUTTONUP);
	m_arrStartEvents.Add(WM_LBUTTONDBLCLK);
	m_arrStartEvents.Add(WM_MBUTTONDBLCLK);
	m_arrStartEvents.Add(WM_RBUTTONDBLCLK);

	m_intelliMouseOrganizer.m_pCustomizeIntelliMousePage=this;
}


COXCustomizeIntelliMousePage::~COXCustomizeIntelliMousePage()
{
}


BOOL COXCustomizeIntelliMousePage::
InitializeIntelliMouse(BOOL bSupportIntelliMouse/*=TRUE*/, 
					   UINT nStartEventID/*=WM_MBUTTONDOWN*/, 
					   DWORD dwKeyState/*=0*/, 
					   BOOL bOnlyViews/*=TRUE*/,
					   LPCTSTR lpszProfileName/*=_T("CustomizeIntelliMouse")*/)
{
	int nIndex=0;
	for(nIndex=0; nIndex<m_arrStartEvents.GetSize(); nIndex++)
	{
		if(nStartEventID==m_arrStartEvents[nIndex])
			break;
	}
	if(nIndex==m_arrStartEvents.GetSize())
	{
		TRACE(_T("COXCustomizeIntelliMousePage::InitializeIntelliMouse: invalid start event ID has been specified\n"));
		return FALSE;
	}
	ASSERT(nIndex>=0 && nIndex<m_mapStartEventDescriptions.GetCount());

	m_bSupportIntelliMouse=bSupportIntelliMouse;
	m_bOnlyViews=bOnlyViews;
	m_nStartEvent=nIndex;

	m_bAltKey=((dwKeyState&KEYPRESSED_ALT)!=0);
	m_bCtrlKey=((dwKeyState&KEYPRESSED_CONTROL)!=0);
	m_bShiftKey=((dwKeyState&KEYPRESSED_SHIFT)!=0);

	CWinApp* pApp=AfxGetApp();
	ASSERT(pApp!=NULL);

	if(lpszProfileName!=NULL)
	{
		m_sProfileName=lpszProfileName;
	
		m_bSupportIntelliMouse=pApp->GetProfileInt(m_sProfileName,
			_T("SupportIntelliMouse"),m_bSupportIntelliMouse);
		m_bOnlyViews=pApp->GetProfileInt(m_sProfileName,
			_T("OnlyViews"),m_bOnlyViews);
		m_nStartEvent=pApp->GetProfileInt(m_sProfileName,
			_T("StartEventIndex"),m_nStartEvent);
		m_bAltKey=pApp->GetProfileInt(m_sProfileName,
			_T("AltKeyPressed"),m_bAltKey);
		m_bCtrlKey=pApp->GetProfileInt(m_sProfileName,
			_T("CtrlKeyPressed"),m_bCtrlKey);
		m_bShiftKey=pApp->GetProfileInt(m_sProfileName,
			_T("ShiftKeyPressed"),m_bShiftKey);
	}

	SetupIntelliMouseSupport();

	return TRUE;
}

BOOL COXCustomizeIntelliMousePage::OnCloseManager(BOOL bIsOk) 
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
				_T("SupportIntelliMouse"),m_bSupportIntelliMouse);
			pApp->WriteProfileInt(m_sProfileName,
				_T("OnlyViews"),m_bOnlyViews);
			pApp->WriteProfileInt(m_sProfileName,
				_T("StartEventIndex"),m_nStartEvent);
			pApp->WriteProfileInt(m_sProfileName,
				_T("AltKeyPressed"),m_bAltKey);
			pApp->WriteProfileInt(m_sProfileName,
				_T("CtrlKeyPressed"),m_bCtrlKey);
			pApp->WriteProfileInt(m_sProfileName,
				_T("ShiftKeyPressed"),m_bShiftKey);
		}
	}
	else
	{
		if(!m_sProfileName.IsEmpty())
		{
			m_bSupportIntelliMouse=pApp->GetProfileInt(m_sProfileName,
				_T("SupportIntelliMouse"),m_bSupportIntelliMouse);
			m_bOnlyViews=pApp->GetProfileInt(m_sProfileName,
				_T("OnlyViews"),m_bOnlyViews);
			m_nStartEvent=pApp->GetProfileInt(m_sProfileName,
				_T("StartEventIndex"),m_nStartEvent);
			m_bAltKey=pApp->GetProfileInt(m_sProfileName,
				_T("AltKeyPressed"),m_bAltKey);
			m_bCtrlKey=pApp->GetProfileInt(m_sProfileName,
				_T("CtrlKeyPressed"),m_bCtrlKey);
			m_bShiftKey=pApp->GetProfileInt(m_sProfileName,
				_T("ShiftKeyPressed"),m_bShiftKey);

			SetupIntelliMouseSupport();
		}
	}

	return TRUE;
}


void COXCustomizeIntelliMousePage::DoDataExchange(CDataExchange* pDX)
{
	COXCustomizePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXCustomizeIntelliMousePage)
	DDX_Control(pDX, IDC_OX_SEPARATOR_START_EVENT, m_sepStartEvent);
	DDX_Control(pDX, IDC_OX_SEPARATOR_KEYPRESSED, m_sepKeyPressed);
	DDX_Control(pDX, IDC_OX_COMBO_START_EVENT, m_comboStartEvent);
	DDX_Check(pDX, IDC_OX_CHECK_ALT, m_bAltKey);
	DDX_Check(pDX, IDC_OX_CHECK_CTRL, m_bCtrlKey);
	DDX_Check(pDX, IDC_OX_CHECK_ONLY_VIEWS, m_bOnlyViews);
	DDX_Check(pDX, IDC_OX_CHECK_SHIFT, m_bShiftKey);
	DDX_Check(pDX, IDC_OX_CHECK_SUPPORT_INTELLIMOUSE, m_bSupportIntelliMouse);
	DDX_CBIndex(pDX, IDC_OX_COMBO_START_EVENT, m_nStartEvent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COXCustomizeIntelliMousePage, COXCustomizePage)
	//{{AFX_MSG_MAP(COXCustomizeIntelliMousePage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeIntelliMousePage message handlers

BOOL COXCustomizeIntelliMousePage::Load(const COXCustomizeManager* pCustomizeManager)
{
	// Call default implementation. It will load this demo dialog as the first 
	// page and will create About and CodeSample pages if specified.
	if(!COXCustomizePage::Load(pCustomizeManager))
		return FALSE;

	return TRUE;
}


void COXCustomizeIntelliMousePage::Unload()
{
	// add here code for cleaning up all objects created by demo
	//
	//
	//////////////////////////////////////////////////////////////////////////

	COXCustomizePage::Unload();
}

void COXCustomizeIntelliMousePage::OnInitDialog()
{
	// must call default implementation
	COXCustomizePage::OnInitDialog();

	// add here initialization code for your demo dialog. Treat it as a
	// normal COXCustomizePage::OnInitDialog function

	// populate multicolumn combo box
	m_comboStartEvent.SetColumnCount(2);
	m_comboStartEvent.SetColumnWidth(0,150);
	m_comboStartEvent.SetColumnWidth(1,150);
	for(int nIndex=0; nIndex<m_arrStartEvents.GetSize(); nIndex++)
	{
		LPCTSTR* ppszString=NULL;
		VERIFY(m_mapStartEventDescriptions.
			Lookup(m_arrStartEvents[nIndex],ppszString));
		ASSERT(ppszString!=NULL);
		VERIFY(m_comboStartEvent.AddString((LPCTSTR*)ppszString,2)!=-1);
	}
	m_comboStartEvent.SetFitToSize(TRUE);

	// layout
	//
	m_LayoutManager.TieChild(&m_sepKeyPressed,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_sepStartEvent,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	m_LayoutManager.TieChild(&m_comboStartEvent,
		OX_LMS_LEFT|OX_LMS_RIGHT,OX_LMT_SAME);
	//
	///////////////////////////////////////

	m_bInitialized=TRUE;

	UpdateData(FALSE);
}


BOOL COXCustomizeIntelliMousePage::ApplyChanges()
{
	if(!UpdateData(TRUE))
		return FALSE;

	SetupIntelliMouseSupport();

	return TRUE;
}


void COXCustomizeIntelliMousePage::SetupIntelliMouseSupport()
{
	ASSERT(m_nStartEvent>=0 && m_nStartEvent<m_arrStartEvents.GetSize());

	if(m_intelliMouseOrganizer.IsAttachedAllInThread())
		VERIFY(m_intelliMouseOrganizer.DetachAllInThread());

	if(m_bSupportIntelliMouse)
	{
		DWORD dwKeyState=0;
		if(m_bShiftKey)
			dwKeyState|=KEYPRESSED_SHIFT;
		if(m_bCtrlKey)
			dwKeyState|=KEYPRESSED_CONTROL;
		if(m_bAltKey)
			dwKeyState|=KEYPRESSED_ALT;
		VERIFY(m_intelliMouseOrganizer.AttachAllInThread(::GetCurrentThreadId(),
			m_arrStartEvents[m_nStartEvent],dwKeyState));
	}
}


BOOL COXCustomizeIntelliMousePage::
IsQualifiedForIntelliMouseSupport(HWND hWnd, LPCTSTR lpszWndClass)
{
	UNREFERENCED_PARAMETER(lpszWndClass);

	if(!m_bOnlyViews)
		return TRUE;

	CWnd* pWnd=CWnd::FromHandlePermanent(hWnd);
	if(pWnd==NULL)
		return FALSE;

	if(!pWnd->IsKindOf(RUNTIME_CLASS(CView)))
		return FALSE;

	return TRUE;
}
