// ==========================================================================
// 					Class Implementation : COXDateTimeCtrl
// ==========================================================================

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

// Version: 9.3
                          
// //////////////////////////////////////////////////////////////////////////

// OXDateTimeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "OXDateTimeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXDateTimeCtrl

//#if _MFC_VER<=0x0421
IMPLEMENT_DYNCREATE(COXDateTimeCtrl, CWnd)
//#endif	//	_MFC_VER>0x0421

BOOL COXDateTimeCtrl::bComCtlInitialized=FALSE;

COXDateTimeCtrl::COXDateTimeCtrl()
{
}

COXDateTimeCtrl::~COXDateTimeCtrl()
{
	if(::IsWindow(GetSafeHwnd()))
		DestroyWindow();
}

void COXDateTimeCtrl::InitializeDTPCtrl()
{
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_DATE_CLASSES;
	::InitCommonControlsEx(&icex);
}


#if _MFC_VER<=0x0421
BEGIN_MESSAGE_MAP(COXDateTimeCtrl, CWnd)
#else
BEGIN_MESSAGE_MAP(COXDateTimeCtrl, CDateTimeCtrl)
#endif	//	_MFC_VER>0x0421
	//{{AFX_MSG_MAP(COXDateTimeCtrl)
	ON_NOTIFY_REFLECT_EX(DTN_DROPDOWN, OnDropDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL COXDateTimeCtrl::Create(DWORD dwStyle, const RECT& rect,
							 CWnd* pParentWnd, UINT nID)
{
	BOOL bRetVal=FALSE;

#if _MFC_VER<=0x0421
	// initialize common controls
	if(!bComCtlInitialized)
	{
		InitializeDTPCtrl();
		bComCtlInitialized = true;
	}

	CWnd* pWnd = this;
	bRetVal=pWnd->Create(DATETIMEPICK_CLASS, NULL, dwStyle, rect, pParentWnd, nID);
#else
	bRetVal=CDateTimeCtrl::Create(dwStyle,rect,pParentWnd,nID);
#endif	//_MFC_VER<=0x0421

	if(bRetVal && !InitDateTimeCtrl())
	{
		TRACE(_T("COXDateTimeCtrl::Create: failed to initialize the date time control"));
		bRetVal=FALSE;
	}

	return bRetVal;
}

void COXDateTimeCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	_AFX_THREAD_STATE* pThreadState=AfxGetThreadState();
	// hook not already in progress
	if(pThreadState->m_pWndInit==NULL)
	{
		DWORD dwStyle=GetStyle();
		// check if subclassed window has WS_CHILD style
		VERIFY((dwStyle&WS_CHILD)!=0);

		BOOL bRecreate=FALSE;
		TCHAR buffer[254];
		if(::GetClassName(GetSafeHwnd(),buffer,254)>0)
		{
			CString sClassName=buffer;
			bRecreate=(sClassName!=_T("SysDateTimePick32"));
		}

		if(bRecreate)
		{
			int nCtrlID=GetDlgCtrlID();
			CWnd* pWndParent=GetParent();
			ASSERT(pWndParent!=NULL);
			CRect rcWnd;
			GetWindowRect(rcWnd);
			HWND hSubclassedWnd=Detach();
			::DestroyWindow(hSubclassedWnd);
			pWndParent->ScreenToClient(rcWnd);
			VERIFY(Create(WS_TABSTOP|WS_CHILD|
				((dwStyle&WS_VISIBLE)!=0 ? WS_VISIBLE : 0),
				rcWnd,pWndParent,nCtrlID));
		}

		if(!InitDateTimeCtrl())
			TRACE(_T("COXDateTimeCtrl::PreSubclassWindow: failed to initialize date time control"));
	}

#if _MFC_VER<=0x0421
	CWnd::PreSubclassWindow();
#else
	CDateTimeCtrl::PreSubclassWindow();
#endif	//	_MFC_VER<=0x0421
}

BOOL COXDateTimeCtrl::InitDateTimeCtrl()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	return TRUE;
}

DWORD COXDateTimeCtrl::GetRange(LPSYSTEMTIME pMinRange,
								LPSYSTEMTIME pMaxRange) const
{
	ASSERT(::IsWindow(m_hWnd));

	SYSTEMTIME sysTimes[2];
	memset(sysTimes, 0, sizeof(sysTimes));

	DWORD dwResult = (DWORD)
		::SendMessage(m_hWnd, DTM_GETRANGE, 0, (LPARAM) &sysTimes);

	if (dwResult & GDTR_MIN && pMinRange)
		memcpy(pMinRange, &sysTimes[0], sizeof(SYSTEMTIME));

	if (dwResult & GDTR_MAX && pMaxRange)
		memcpy(pMaxRange, &sysTimes[1], sizeof(SYSTEMTIME));

	return dwResult;
}

BOOL COXDateTimeCtrl::SetRange(const LPSYSTEMTIME pMinRange,
							   const LPSYSTEMTIME pMaxRange)
{
	ASSERT(::IsWindow(m_hWnd));

	SYSTEMTIME sysTimes[2];
	WPARAM wFlags = 0;

	if (pMinRange != NULL)
	{
		memcpy(&sysTimes[0], pMinRange, sizeof(SYSTEMTIME));
		wFlags |= GDTR_MIN;
	}

	if (pMaxRange != NULL)
	{
		memcpy(&sysTimes[1], pMaxRange, sizeof(SYSTEMTIME));
		wFlags |= GDTR_MAX;
	}

	return (BOOL)::SendMessage(m_hWnd, DTM_SETRANGE, wFlags, (LPARAM) &sysTimes);
}



#if _MFC_VER<=0x0421

BOOL COXDateTimeCtrl::SetTime(LPSYSTEMTIME pTimeNew /* = NULL */)
{
	ASSERT(::IsWindow(m_hWnd));
	WPARAM wParam = (pTimeNew == NULL) ? GDT_NONE : GDT_VALID;
	return (BOOL) ::SendMessage(m_hWnd, DTM_SETSYSTEMTIME,
		wParam, (LPARAM) pTimeNew);
}

BOOL COXDateTimeCtrl::SetTime(const COleDateTime& timeNew)
{
	BOOL bRetVal = FALSE;

	// make sure the time isn't invalid
	ASSERT(timeNew.GetStatus() != COleDateTime::invalid);
	ASSERT(::IsWindow(m_hWnd));

	SYSTEMTIME sysTime;
	WPARAM wParam = GDT_NONE;
	if (timeNew.GetStatus() == COleDateTime::valid &&
		COXMonthCalCtrl::GetAsSystemTime(timeNew,sysTime))
	{
		wParam = GDT_VALID;
	}

	bRetVal = (BOOL) ::SendMessage(m_hWnd,
			DTM_SETSYSTEMTIME, wParam, (LPARAM) &sysTime);

	return bRetVal;
}

BOOL COXDateTimeCtrl::SetTime(const CTime* pTimeNew)
{
	BOOL bRetVal = FALSE;

	// make sure the time isn't invalid
	ASSERT(::IsWindow(m_hWnd));

	SYSTEMTIME sysTime;
	WPARAM wParam = GDT_NONE;
	if (pTimeNew != NULL && COXMonthCalCtrl::GetAsSystemTime(*pTimeNew,sysTime))
	{
		wParam = GDT_VALID;
	}

	bRetVal = (BOOL) ::SendMessage(m_hWnd,
			DTM_SETSYSTEMTIME, wParam, (LPARAM) &sysTime);

	return bRetVal;
}

BOOL COXDateTimeCtrl::GetTime(COleDateTime& timeDest) const
{
	SYSTEMTIME sysTime;
	BOOL bRetVal = TRUE;

	LRESULT result = ::SendMessage(m_hWnd, DTM_GETSYSTEMTIME, 0, (LPARAM) &sysTime);
	if (result == GDT_VALID)
	{
		timeDest = COleDateTime(sysTime);
		bRetVal = TRUE;
		ASSERT(timeDest.GetStatus() == COleDateTime::valid);
	}
	else if (result == GDT_NONE)
	{
		timeDest.SetStatus(COleDateTime::null);
		bRetVal = TRUE;
	}
	else
		timeDest.SetStatus(COleDateTime::invalid);
	return bRetVal;
}

DWORD COXDateTimeCtrl::GetTime(CTime& timeDest) const
{
	SYSTEMTIME sysTime;
	DWORD dwResult = (DWORD)
		::SendMessage(m_hWnd, DTM_GETSYSTEMTIME, 0, (LPARAM) &sysTime);

	if (dwResult == GDT_VALID)
		timeDest = CTime(sysTime);

	return dwResult;
}

#endif	//	_MFC_VER>0x0421


DWORD COXDateTimeCtrl::GetRange(CTime* pMinTime, CTime* pMaxTime) const
{
#if _MFC_VER<=0x0421
	ASSERT(::IsWindow(m_hWnd));
	SYSTEMTIME sysTimes[2];
	memset(sysTimes, 0, sizeof(sysTimes));

	DWORD dwResult = ::SendMessage(m_hWnd, DTM_GETRANGE, 0, (LPARAM) &sysTimes);

	if (pMinTime != NULL)
	{
		if (dwResult & GDTR_MIN)
			*pMinTime = CTime(sysTimes[0]);
	}

	if (pMaxTime != NULL)
	{
		if (dwResult & GDTR_MAX)
			*pMaxTime = CTime(sysTimes[1]);
	}

	return dwResult;
#else
	return CDateTimeCtrl::GetRange(pMinTime,pMaxTime);
#endif	//	_MFC_VER>0x0421
}

DWORD COXDateTimeCtrl::GetRange(COleDateTime* pMinTime,
								COleDateTime* pMaxTime) const
{
#if _MFC_VER<=0x0421
	ASSERT(::IsWindow(m_hWnd));

	SYSTEMTIME sysTimes[2];
	memset(sysTimes, 0, sizeof(sysTimes));

	DWORD dwResult = ::SendMessage(m_hWnd, DTM_GETRANGE, 0, (LPARAM) &sysTimes);
	if (pMinTime != NULL)
	{
		if (dwResult & GDTR_MIN)
			*pMinTime = COleDateTime(sysTimes[0]);
		else
			pMinTime->SetStatus(COleDateTime::null);
	}

	if (pMaxTime != NULL)
	{
		if (dwResult & GDTR_MAX)
			*pMaxTime = COleDateTime(sysTimes[1]);
		else
			pMaxTime->SetStatus(COleDateTime::null);
	}

	return dwResult;
#else
	return CDateTimeCtrl::GetRange(pMinTime,pMaxTime);
#endif	//	_MFC_VER>0x0421
}

BOOL COXDateTimeCtrl::SetRange(const CTime* pMinTime, const CTime* pMaxTime)
{
#if _MFC_VER<=0x0421
	ASSERT(::IsWindow(m_hWnd));

	SYSTEMTIME sysTimes[2];

	WPARAM wFlags = 0;
	if (pMinTime != NULL && COXMonthCalCtrl::GetAsSystemTime(*pMinTime,sysTimes[0]))
		wFlags |= GDTR_MIN;

	if (pMaxTime != NULL && COXMonthCalCtrl::GetAsSystemTime(*pMaxTime,sysTimes[1]))
		wFlags |= GDTR_MAX;

	return (BOOL)::SendMessage(m_hWnd, DTM_SETRANGE, wFlags, (LPARAM) &sysTimes);
#else
	return CDateTimeCtrl::SetRange(pMinTime,pMaxTime);
#endif	//	_MFC_VER>0x0421
}

BOOL COXDateTimeCtrl::SetRange(const COleDateTime* pMinTime, 
							   const COleDateTime* pMaxTime)
{
#if _MFC_VER<=0x0421
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pMinTime == NULL || pMinTime->GetStatus() != COleDateTime::invalid);
	ASSERT(pMaxTime == NULL || pMaxTime->GetStatus() != COleDateTime::invalid);

	SYSTEMTIME sysTime[2];

	WPARAM wFlags = 0;
	if (pMinTime != NULL && pMinTime->GetStatus() != COleDateTime::null)
	{
		if (COXMonthCalCtrl::GetAsSystemTime(*pMinTime,sysTime[0]))
			wFlags |= GDTR_MIN;
	}

	if (pMaxTime != NULL && pMaxTime->GetStatus() != COleDateTime::null)
	{
		if (COXMonthCalCtrl::GetAsSystemTime(*pMaxTime,sysTime[1]))
			wFlags |= GDTR_MAX;
	}

	return (BOOL) ::SendMessage(m_hWnd, DTM_SETRANGE, wFlags, (LPARAM) &sysTime);
#else
	return CDateTimeCtrl::SetRange(pMinTime,pMaxTime);
#endif	//	_MFC_VER>0x0421
}

BOOL COXDateTimeCtrl::OnDropDown(NMHDR *pHdr, LRESULT *pRes)
{
	UNREFERENCED_PARAMETER(pHdr);

	HWND hwndMonthCal=(HWND)::SendMessage(m_hWnd,DTM_GETMONTHCAL,0,0);
	if(hwndMonthCal!=NULL)
	{
		if(::IsWindow(m_ctlMonthCal.GetSafeHwnd()))
			m_ctlMonthCal.DestroyWindow();

		m_ctlMonthCal.SubclassWindow(hwndMonthCal);
	}

	*pRes=0;

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// DDX_ routines

#if _MFC_VER<=0x0421

void AFXAPI DDX_DateTimeCtrl(CDataExchange* pDX, int nIDC, COleDateTime& value)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	COXDateTimeCtrl* pWnd = (COXDateTimeCtrl*) CWnd::FromHandle(hWndCtrl);

	if (pDX->m_bSaveAndValidate)
		pWnd->GetTime(value);
	else
		pWnd->SetTime(value);
}

void AFXAPI DDX_DateTimeCtrl(CDataExchange* pDX, int nIDC, CTime& value)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	COXDateTimeCtrl* pWnd = (COXDateTimeCtrl*) CWnd::FromHandle(hWndCtrl);

	if (pDX->m_bSaveAndValidate)
		pWnd->GetTime(value);
	else
		pWnd->SetTime(&value);
}

void AFXAPI DDV_MinMaxDateTime(CDataExchange* pDX, CTime& refValue,
	const CTime* pMinRange, const CTime* pMaxRange)
{
	ASSERT(pMinRange == NULL || pMaxRange == NULL || *pMinRange <= *pMaxRange);

	COXDateTimeCtrl* pWnd =
		(COXDateTimeCtrl*) CWnd::FromHandle(pDX->m_hWndLastControl);

	if (!pDX->m_bSaveAndValidate)
	{
		if ( (pMinRange != NULL && *pMinRange > refValue) ||
			  (pMaxRange != NULL && *pMaxRange < refValue))
		{
#ifdef _DEBUG
			int nIDC = GetWindowLongPtr(pDX->m_hWndLastControl, GWL_ID);
			TRACE1("Warning: initial dialog data is out of range in control ID %d.\n", nIDC);
#endif
			return;     // don't stop now
		}
	}

	pWnd->SetRange(pMinRange, pMaxRange);
}

void AFXAPI DDV_MinMaxDateTime(CDataExchange* pDX,  COleDateTime& refValue,
	const COleDateTime* pMinRange, const COleDateTime* pMaxRange)
{
	ASSERT(pMinRange == NULL || pMaxRange == NULL || *pMinRange <= *pMaxRange);
	COXDateTimeCtrl* pWnd =
		(COXDateTimeCtrl*) CWnd::FromHandle(pDX->m_hWndLastControl);

	if (!pDX->m_bSaveAndValidate)
	{
		if ( (pMinRange != NULL && *pMinRange > refValue) ||
			  (pMaxRange != NULL && *pMaxRange < refValue))
		{
			int nIDC = GetWindowLongPtr(pDX->m_hWndLastControl, GWL_ID);
			TRACE1("Warning: initial dialog data is out of range in control ID %d.\n", nIDC);
			return;     // don't stop now
		}
	}

	pWnd->SetRange(pMinRange, pMaxRange);
}

#endif
