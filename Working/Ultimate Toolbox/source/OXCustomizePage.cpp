// ==========================================================================
//			Class Implementation: COXCustomizePage
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
#include "OXCustomizePage.h"
#include "OXCustomizeManager.h"

#ifndef __OXMFCIMPL_H__
#if _MFC_VER < 0x0700
	#include <..\src\afximpl.h>
#else
	#include <..\src\mfc\afximpl.h>
#endif
#define __OXMFCIMPL_H__
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const int IDC_DEFAULTPAGE=1000;


IMPLEMENT_DYNCREATE(COXCustomizePage, CWnd)

/////////////////////////////////////////////////////////////////////////////
// COXCustomizePage

COXCustomizePage::COXCustomizePage() :
	m_nDialogID(0xffffffff),
	m_pCustomizeManager(NULL),
	m_sProfileName(_T("")),
	m_bInitialized(FALSE)
{
#ifndef _AFX_NO_OCC_SUPPORT
	m_pOccDialogInfo = NULL;
#endif
}

COXCustomizePage::~COXCustomizePage()
{
}


BEGIN_MESSAGE_MAP(COXCustomizePage, CWnd)
	//{{AFX_MSG_MAP(COXCustomizePage)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COXCustomizePage message handlers

// virtual override of CWnd::Create
BOOL COXCustomizePage::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd, CRect rect, 
							  UINT nID, DWORD dwStyle/*=WS_VISIBLE|WS_CHILD*/)
{
	ASSERT(pParentWnd!=NULL);
	ASSERT(lpszTemplateName!=NULL);

	// initialize common controls
	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTLS_REG));

	BOOL bShowWindow=((dwStyle&WS_VISIBLE)==WS_VISIBLE);
	dwStyle&=~WS_VISIBLE;

	// call PreCreateWindow to get prefered extended style
	CREATESTRUCT cs={ 0 };
	cs.style=dwStyle;
	if(!PreCreateWindow(cs))
		return FALSE;

	// create a modeless dialog
	if(!CreateDlg(lpszTemplateName, pParentWnd))
		return FALSE;

	// we use the style from the template - but make sure that
	//  the WS_BORDER bit is correct
	// the WS_BORDER bit will be whatever is in dwRequestedStyle
	ModifyStyle(WS_BORDER|WS_CAPTION,(cs.style&~(WS_BORDER|WS_CAPTION))|WS_CHILD);
	ModifyStyleEx(WS_EX_CLIENTEDGE,(cs.dwExStyle&~WS_EX_CLIENTEDGE));

	SetDlgCtrlID(nID);

	// initialize controls etc
	if(!ExecuteDlgInit(lpszTemplateName))
		return FALSE;

	// give the page an opportunity to initialize its contents
	OnInitDialog();

	// force the size requested
	SetWindowPos(NULL, rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		SWP_NOZORDER|SWP_NOACTIVATE);

	// make visible if requested
	if(bShowWindow)
		ShowWindow(SW_NORMAL);

	return TRUE;
}


BOOL COXCustomizePage::Load(const COXCustomizeManager* pCustomizeManager)
{
	ASSERT(m_nDialogID!=0xffffffff);
	ASSERT(pCustomizeManager!=NULL);
	ASSERT(m_pCustomizeManager==NULL || m_pCustomizeManager==pCustomizeManager);

	if(::IsWindow(GetSafeHwnd()))
	{
		ASSERT(m_pCustomizeManager==pCustomizeManager);
		ShowWindow(SW_SHOW);
		return TRUE;
	}
	else
	{
		m_pCustomizeManager=(COXCustomizeManager*)pCustomizeManager;

		if(!Create(MAKEINTRESOURCE(m_nDialogID),m_pCustomizeManager->GetSite(),
			CRect(0,0,0,0),IDC_DEFAULTPAGE))
		{
			m_pCustomizeManager=NULL;
			return FALSE;
		}

		return TRUE;
	}
}

void COXCustomizePage::Unload()
{
	if(::IsWindow(GetSafeHwnd()))
	{
		ShowWindow(SW_HIDE);
	}
}


void COXCustomizePage::OnDestroy() 
{
	// TODO: Add your message handler code here
	if(m_LayoutManager.IsAttached())
		m_LayoutManager.Detach();

	m_bInitialized=FALSE;

	CWnd::OnDestroy();
}


BOOL COXCustomizePage::PreTranslateMessage(MSG* pMsg)
{
	ASSERT(pMsg!=NULL);
	ASSERT_VALID(this);

	// allow tooltip messages to be filtered
	if(CWnd::PreTranslateMessage(pMsg))
		return TRUE;

	// don't translate dialog messages when in Shift+F1 help mode
	CFrameWnd* pFrameWnd=GetTopLevelFrame();
	if(pFrameWnd!=NULL && pFrameWnd->m_bHelpMode)
		return FALSE;

	// since 'IsDialogMessage' will eat frame window accelerators,
	// we call all frame windows' PreTranslateMessage first
	pFrameWnd=GetParentFrame();   // start with first parent frame
	while(pFrameWnd!=NULL)
	{
		// allow owner & frames to translate before IsDialogMessage does
		if(pFrameWnd->PreTranslateMessage(pMsg))
			return TRUE;

		// try parent frames until there are no parent frames
		pFrameWnd=pFrameWnd->GetParentFrame();
	}

	// don't call IsDialogMessage if demo is empty
	if(::GetWindow(GetSafeHwnd(),GW_CHILD)==NULL)
		return FALSE;

	// filter both messages to demo children
	return PreTranslateInput(pMsg);
}


BOOL COXCustomizePage::SaveWorkspaceState()
{
	return TRUE; 
}

#ifndef _AFX_NO_OCC_SUPPORT
BOOL COXCustomizePage::SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* pOccDialogInfo)
{
	m_pOccDialogInfo = pOccDialogInfo;
	return TRUE;
}
#endif //!_AFX_NO_OCC_SUPPORT

BOOL COXCustomizePage::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
#ifdef OX_CUSTOMPAGE_KEYSOKCANCEL
	if (nID==IDCANCEL || nID==IDOK)
		return m_pCustomizeManager->OnCmdMsg(nID, nCode, pExtra,pHandlerInfo);
	else
#endif
		return CWnd::OnCmdMsg(nID, nCode, pExtra,pHandlerInfo);
}


