// OXCoolControlOrganizer.cpp: implementation of the COXCoolControlOrganizer class.
//
//////////////////////////////////////////////////////////////////////
// Version: 9.3


#include "stdafx.h"
#include "OXCoolControlOrganizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// static variables
CMap<DWORD,DWORD,COXCoolControlOrganizer*,COXCoolControlOrganizer*> 
COXCoolControlOrganizer::m_arrThreadOrganizers;

HHOOK COXCoolControlOrganizer::m_pfnOriginalCBTHookProc=NULL;
HHOOK COXCoolControlOrganizer::m_pfnOriginalGetMessageHookProc=NULL;
/////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COXCoolControlOrganizer::COXCoolControlOrganizer() :
	m_dwThreadID(NULL),
	m_pfnOldCBTHookProc(NULL),
	m_pfnOldGetMessageHookProc(NULL)
{
}


COXCoolControlOrganizer::~COXCoolControlOrganizer()
{
	if(IsAttachedAllInThread())
	{
		VERIFY(DetachAllInThread());
	}
	else
	{
		VERIFY(Detach(NULL));
	}

	ASSERT(m_pfnOriginalCBTHookProc==NULL);
	ASSERT(m_pfnOldCBTHookProc==NULL);
	ASSERT(m_pfnOriginalGetMessageHookProc==NULL);
}


BOOL COXCoolControlOrganizer::Attach(HWND hWnd)
{
	ASSERT(hWnd!=NULL);
	ASSERT(::IsWindow(hWnd));

	if(IsAttached(hWnd))
	{
		TRACE(_T("COXCoolControlOrganizer::Attach: specified window has already been attached to a cool control object\n"));
		return FALSE;
	}

	if(IsRejected(hWnd))
	{
		TRACE(_T("COXCoolControlOrganizer::Attach: specified window has been rejected before\n"));
		return FALSE;
	}


	CWnd* pWnd=CWnd::FromHandlePermanent(hWnd);
	if(pWnd!=NULL)
	{
#ifdef _DEBUG
		if(!IsAttachedAllInThread())
		{
			TRACE(_T("COXCoolControlOrganizer::Attach: cannot attach already subclassed window\n"));
		}
#endif	//	_DEBUG
		m_mapRejectedControls.SetAt(hWnd,1);
		return FALSE;
	}

	CString sWndClass;
	::GetClassName(hWnd,sWndClass.GetBuffer(512),512);

	if(!IsQualified(hWnd,sWndClass))
	{
#ifdef _DEBUG
		if(!IsAttachedAllInThread())
		{
			TRACE(_T("COXCoolControlOrganizer::Attach: the window didn't qualify to be attached\n"));
		}
#endif	//	_DEBUG
		m_mapRejectedControls.SetAt(hWnd,2);
		return FALSE;
	}

	void* pCoolCtrl=CreateCoolCtrl(hWnd,sWndClass);
	if(pCoolCtrl==NULL)
	{
		TRACE(_T("COXCoolControlOrganizer::Attach: failed to create cool control for the specified window\n"));
		m_mapRejectedControls.SetAt(hWnd,3);
		return FALSE;
	}

	m_mapAttachedControls.SetAt(hWnd,pCoolCtrl);

	::RedrawWindow(hWnd,NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_FRAME);

	return TRUE;
}


BOOL COXCoolControlOrganizer::IsAttached(const HWND hWnd) const
{
	void* pCoolCtrl=NULL;
	BOOL bResult=m_mapAttachedControls.Lookup(hWnd,pCoolCtrl);
	ASSERT(!bResult || pCoolCtrl!=NULL);
	return bResult;
}


BOOL COXCoolControlOrganizer::IsRejected(const HWND hWnd) const
{
	DWORD dwReason=0;
	BOOL bResult=m_mapRejectedControls.Lookup(hWnd,dwReason);
	ASSERT(!bResult || dwReason!=0);
	return bResult;
}


BOOL COXCoolControlOrganizer::Detach(HWND hWnd)
{
	if(hWnd!=NULL)
	{
		if(!IsAttached(hWnd))
		{
			TRACE(_T("COXCoolControlOrganizer::Detach: the specified window is not currently attached to a cool control object\n"));
			return FALSE;
		}

		void* pCoolCtrl=NULL;
		VERIFY(m_mapAttachedControls.Lookup(hWnd,pCoolCtrl));
		ASSERT(pCoolCtrl!=NULL);

		if(::IsWindow(((COXCoolCtrl<CWnd>*)pCoolCtrl)->GetSafeHwnd()))
		{
			((COXCoolCtrl<CWnd>*)pCoolCtrl)->UnsubclassWindow();
			::RedrawWindow(hWnd,NULL,NULL,
				RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_FRAME);
		}
		else
		{
			((COXCoolCtrl<CWnd>*)pCoolCtrl)->m_hWnd=NULL;
		}

		delete (COXCoolCtrl<CWnd>*)pCoolCtrl;
		m_mapAttachedControls.RemoveKey(hWnd);

		return TRUE;
	}
	else
	{
		for(POSITION pos=m_mapAttachedControls.GetStartPosition(); pos!=NULL;
			pos=m_mapAttachedControls.GetStartPosition())
		{
			HWND hWndAttached=NULL;
			void* pCoolCtrl=NULL;
			m_mapAttachedControls.GetNextAssoc(pos,hWndAttached,pCoolCtrl);
			ASSERT(hWndAttached!=NULL && pCoolCtrl!=NULL);
			VERIFY(Detach(hWndAttached));
		}
		return TRUE;
	}
}


BOOL COXCoolControlOrganizer::
AttachAllInThread(DWORD dwThreadID/*=::GetCurrentThreadId()*/)
{
	if(IsAttachedAllInThread())
	{
		TRACE(_T("COXCoolControlOrganizer::AttachAllInThread: this object already attached to a thread\n"));
		return FALSE;
	}

	COXCoolControlOrganizer* pOrganizer=NULL;
	if(m_arrThreadOrganizers.Lookup(dwThreadID,pOrganizer))
	{
		ASSERT(pOrganizer!=NULL);
		TRACE(_T("COXCoolControlOrganizer::AttachAllInThread: specified thread already attached to a COXCoolControlOrganizer object\n"));
		return FALSE;
	}
	m_arrThreadOrganizers.SetAt(dwThreadID,this);

	m_dwThreadID=dwThreadID;
	// go through all windows and attach them
	::EnumWindows(&EnumThreadWindows,(LPARAM)this);

	// setup hooks for Computer Based Training
	if(m_pfnOriginalCBTHookProc==NULL)
	{
		m_pfnOriginalCBTHookProc=
			::SetWindowsHookEx(WH_CBT,CoolControlOrganizerCBTHookProc,NULL,dwThreadID);
		m_pfnOldCBTHookProc=m_pfnOriginalCBTHookProc;
	}
	else
	{
		m_pfnOldCBTHookProc=
			::SetWindowsHookEx(WH_CBT,CoolControlOrganizerCBTHookProc,NULL,dwThreadID);
	}

	// setup hooks for GetMessage
	if(m_pfnOriginalGetMessageHookProc==NULL)
	{
		m_pfnOriginalGetMessageHookProc=::SetWindowsHookEx(WH_GETMESSAGE,
			CoolControlOrganizerGetMessageHookProc,NULL,dwThreadID);
		m_pfnOldGetMessageHookProc=m_pfnOriginalGetMessageHookProc;
	}
	else
	{
		m_pfnOldGetMessageHookProc=::SetWindowsHookEx(WH_GETMESSAGE,
			CoolControlOrganizerGetMessageHookProc,NULL,dwThreadID);
	}

	return TRUE;
}


void COXCoolControlOrganizer::AttachAllWindows(HWND hWndStartFrom)
{
	ASSERT(hWndStartFrom!=NULL);

	HWND hWnd=hWndStartFrom;
	while(hWnd!=NULL)
	{
		DWORD dwThreadID=::GetWindowThreadProcessId(hWnd,NULL);
		if(dwThreadID!=GetAttachedThread())
			break;

		if(!IsAttached(hWnd) && !IsRejected(hWnd))
		{
			Attach(hWnd);
		}

		// loop through children
		HWND hWndChild=::GetWindow(hWnd,GW_CHILD);

		if(!IsAttached(hWndChild) && !IsRejected(hWndChild) && hWndChild!=NULL)
		{
			AttachAllWindows(hWndChild);
		}

		// loop through windows
		hWnd=::GetWindow(hWnd,GW_HWNDNEXT);
	}
}


LRESULT CALLBACK COXCoolControlOrganizer::
CoolControlOrganizerCBTHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	if(nCode>=0 && ::IsWindow((HWND)wParam))
	{
		DWORD dwThreadID=::GetWindowThreadProcessId((HWND)wParam,NULL);
		COXCoolControlOrganizer* pOrganizer=NULL;
		if(COXCoolControlOrganizer::m_arrThreadOrganizers.
			Lookup(dwThreadID,pOrganizer))
		{
			ASSERT(pOrganizer!=NULL);
			ASSERT(pOrganizer->IsAttachedAllInThread());

			if(nCode==HCBT_DESTROYWND)
			{
				// check if the window that is about to be destroyed
				// had been added to cool control organizer list
				if(pOrganizer->IsAttached((HWND)wParam))
					pOrganizer->Detach((HWND)wParam);
			}
			return ::CallNextHookEx(pOrganizer->GetSavedCBTHookProc(),
				nCode,wParam,lParam);
		}
	}

	return ::CallNextHookEx(COXCoolControlOrganizer::GetOriginalCBTHookProc(),
		nCode,wParam,lParam);
}


LRESULT CALLBACK COXCoolControlOrganizer::
CoolControlOrganizerGetMessageHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	HWND hWnd=((MSG*)lParam)->hwnd;
	if(nCode>=0 && ::IsWindow(hWnd))
	{
		DWORD dwThreadID=::GetWindowThreadProcessId(hWnd,NULL);
		COXCoolControlOrganizer* pOrganizer=NULL;
		if(COXCoolControlOrganizer::
			m_arrThreadOrganizers.Lookup(dwThreadID,pOrganizer))
		{
			ASSERT(pOrganizer!=NULL);
			ASSERT(pOrganizer->IsAttachedAllInThread());

			// check if new window is created and attach it.
			if(pOrganizer->IsAttached(hWnd) && ((MSG*)lParam)->message==WM_DESTROY)
			{
				pOrganizer->Detach(hWnd);
			}
			else if(!pOrganizer->IsAttached(hWnd))
			{
				pOrganizer->AttachAllWindows(hWnd);
			}

			return ::CallNextHookEx(pOrganizer->GetSavedGetMessageHookProc(),
				nCode,wParam,lParam);
		}
	}

	return ::CallNextHookEx(COXCoolControlOrganizer::GetOriginalGetMessageHookProc(),
		nCode,wParam,lParam);
}


BOOL CALLBACK COXCoolControlOrganizer::EnumThreadWindows(HWND hWnd, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	ASSERT(lParam!=NULL);
	ASSERT(::IsWindow(hWnd));
	COXCoolControlOrganizer* pOrganizer=(COXCoolControlOrganizer*)lParam;
	ASSERT(pOrganizer->IsAttachedAllInThread());

	DWORD dwThreadID=::GetWindowThreadProcessId(hWnd,NULL);
	if(dwThreadID==pOrganizer->GetAttachedThread())
	{
		pOrganizer->AttachAllWindows(hWnd);
		return FALSE;
	}
	return TRUE;
}


BOOL COXCoolControlOrganizer::DetachAllInThread()
{
	if(!IsAttachedAllInThread())
		return FALSE;

	ASSERT(m_dwThreadID!=NULL);
	ASSERT(m_pfnOldCBTHookProc!=NULL);
	ASSERT(m_pfnOriginalCBTHookProc!=NULL);
	ASSERT(m_pfnOldGetMessageHookProc!=NULL);
	ASSERT(m_pfnOriginalGetMessageHookProc!=NULL);

	// unhook CBT
	if(m_pfnOldCBTHookProc!=NULL)
	{
		VERIFY(::UnhookWindowsHookEx(m_pfnOldCBTHookProc));
		m_pfnOldCBTHookProc=NULL;
		m_pfnOriginalCBTHookProc=NULL;
	}

	// unhook GetMessage
	if(m_pfnOldGetMessageHookProc!=NULL)
	{
		VERIFY(::UnhookWindowsHookEx(m_pfnOldGetMessageHookProc));
		m_pfnOldGetMessageHookProc=NULL;
		m_pfnOriginalGetMessageHookProc=NULL;
	}

	// clear map of rejected windows
	m_mapRejectedControls.RemoveAll();

	m_arrThreadOrganizers.RemoveKey(m_dwThreadID);

	m_dwThreadID=NULL;

	return Detach(NULL);
}


BOOL COXCoolControlOrganizer::IsQualified(HWND hWnd, LPCTSTR lpszWndClass)
{
	ASSERT(::IsWindow(hWnd));

	CString sWndClass=lpszWndClass;

	if(sWndClass.CompareNoCase(EDIT_CLASSNAME)==0 ||
		sWndClass.CompareNoCase(BUTTON_CLASSNAME)==0 ||
		sWndClass.CompareNoCase(SPIN_CLASSNAME)==0 ||
		sWndClass.CompareNoCase(HOTKEY_CLASSNAME)==0 ||
		sWndClass.CompareNoCase(COMBOBOX_CLASSNAME)==0 ||
		sWndClass.CompareNoCase(SCROLLBAR_CLASSNAME)==0 ||
		sWndClass.CompareNoCase(PROGRESS_CLASSNAME)==0 ||
		sWndClass.CompareNoCase(SLIDER_CLASSNAME)==0 ||
#if _MFC_VER > 0x0421
		sWndClass.CompareNoCase(DATETIME_CLASSNAME)==0 ||
		sWndClass.CompareNoCase(MONTHCAL_CLASSNAME)==0 ||
		sWndClass.CompareNoCase(IPADDRESS_CLASSNAME)==0 ||
#endif	//	_MFC_VER > 0x0421
		sWndClass.CompareNoCase(LISTVIEW_CLASSNAME)==0 ||
		sWndClass.CompareNoCase(TREEVIEW_CLASSNAME)==0 ||
		sWndClass.CompareNoCase(LISTBOX_CLASSNAME)==0 ||
		sWndClass.CompareNoCase(RICHEDIT_CLASSNAME)==0)
	{
		// special case with combo box - it subclasses its child edit and list box 
		// controls internally
		if(sWndClass.CompareNoCase(EDIT_CLASSNAME)==0/* || 
			sWndClass.CompareNoCase(LISTBOX_CLASSNAME)==0*/)
		{
			HWND hWndParent=::GetParent(hWnd);
			if(hWndParent!=NULL)
			{
				if(COXCoolCtrl<CWnd>::IsComboBox(hWndParent))
				{
					m_mapRejectedControls.SetAt(hWnd,4);
					return FALSE;
				}
			}
		}

		return TRUE;
	}

	return FALSE;
}


void* COXCoolControlOrganizer::CreateCoolCtrl(HWND hWnd, LPCTSTR lpszWndClass)
{
	CString sWndClass=lpszWndClass;
	COXCoolCtrl<CWnd>* pCoolCtrl=NULL;
	
	if(sWndClass.CompareNoCase(EDIT_CLASSNAME)==0)
		pCoolCtrl=(COXCoolCtrl<CWnd>*)new COXCoolEdit;
	else if(sWndClass.CompareNoCase(BUTTON_CLASSNAME)==0)
		pCoolCtrl=(COXCoolCtrl<CWnd>*)new COXCoolButton;
	else if(sWndClass.CompareNoCase(SPIN_CLASSNAME)==0)
		pCoolCtrl=(COXCoolCtrl<CWnd>*)new COXCoolSpinButtonCtrl;
	else if(sWndClass.CompareNoCase(HOTKEY_CLASSNAME)==0)
		pCoolCtrl=(COXCoolCtrl<CWnd>*)new COXCoolHotKeyCtrl;
	else if(sWndClass.CompareNoCase(COMBOBOX_CLASSNAME)==0)
		pCoolCtrl=(COXCoolCtrl<CWnd>*)new COXCoolComboBox;
	else if(sWndClass.CompareNoCase(SCROLLBAR_CLASSNAME)==0)
		pCoolCtrl=(COXCoolCtrl<CWnd>*)new COXCoolScrollBar;
	else if(sWndClass.CompareNoCase(PROGRESS_CLASSNAME)==0)
		pCoolCtrl=(COXCoolCtrl<CWnd>*)new COXCoolProgressCtrl;
	else if(sWndClass.CompareNoCase(SLIDER_CLASSNAME)==0)
		pCoolCtrl=(COXCoolCtrl<CWnd>*)new COXCoolSliderCtrl;
#if _MFC_VER > 0x0421
	else if(sWndClass.CompareNoCase(DATETIME_CLASSNAME)==0)
		pCoolCtrl=(COXCoolCtrl<CWnd>*)new COXCoolDateTimeCtrl;
	else if(sWndClass.CompareNoCase(MONTHCAL_CLASSNAME)==0)
		pCoolCtrl=(COXCoolCtrl<CWnd>*)new COXCoolMonthCalCtrl;
	else if(sWndClass.CompareNoCase(IPADDRESS_CLASSNAME)==0)
		pCoolCtrl=(COXCoolCtrl<CWnd>*)new COXCoolIPAddressCtrl;
#endif	//	_MFC_VER > 0x0421
	else if(sWndClass.CompareNoCase(LISTVIEW_CLASSNAME)==0)
		pCoolCtrl=(COXCoolCtrl<CWnd>*)new COXCoolListCtrl;
	else if(sWndClass.CompareNoCase(TREEVIEW_CLASSNAME)==0)
		pCoolCtrl=(COXCoolCtrl<CWnd>*)new COXCoolTreeCtrl;
	else if(sWndClass.CompareNoCase(LISTBOX_CLASSNAME)==0)
		pCoolCtrl=(COXCoolCtrl<CWnd>*)new COXCoolListBox;
	else if(sWndClass.CompareNoCase(RICHEDIT_CLASSNAME)==0)
		pCoolCtrl=(COXCoolCtrl<CWnd>*)new COXCoolRichEditCtrl;
	else
		pCoolCtrl=(COXCoolCtrl<CWnd>*)new COXCoolCtrl<CWnd>;

	ASSERT(pCoolCtrl!=NULL);
	if(!pCoolCtrl->SubclassWindow(hWnd))
	{
		TRACE(_T("COXCoolControlOrganizer::CreateCoolCtrl: failed to subclass the window\n"));
		delete pCoolCtrl;
		return NULL;
	}

	return (void*)pCoolCtrl;
}

