// ==========================================================================
// 					Class Implementation : COXMonthCalCtrl
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

// OXMonthCalCtrl.cpp : implementation file
//

#include "stdafx.h"
#include <windowsx.h>
#include "OXMonthCalCtrl.h"
#include "UTB64Bit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXMonthCalCtrl


#if _MFC_VER<=0x0421
IMPLEMENT_DYNAMIC(COXMonthCalCtrl, CWnd)

#endif	//	_MFC_VER>0x0421

BOOL COXMonthCalCtrl::bComCtlInitialized=FALSE;

COXMonthCalCtrl::COXMonthCalCtrl()
{
	m_dwMonthCalStyle=0;
}

COXMonthCalCtrl::~COXMonthCalCtrl()
{
	DWORD dwMonthYear;
	LPHOLIDAYS pHolidays;
	POSITION pos=m_arrHolidays.GetStartPosition();
	while(pos!=NULL)
	{
		m_arrHolidays.GetNextAssoc(pos,dwMonthYear,pHolidays);
		ASSERT(pHolidays!=NULL);
		delete pHolidays;
	}

	if(::IsWindow(GetSafeHwnd()))
		DestroyWindow();
}


#if _MFC_VER<=0x0421
BEGIN_MESSAGE_MAP(COXMonthCalCtrl, CWnd)
#else
BEGIN_MESSAGE_MAP(COXMonthCalCtrl, CMonthCalCtrl)
#endif	//	_MFC_VER>0x0421
	//{{AFX_MSG_MAP(COXMonthCalCtrl)
	ON_WM_DESTROY()
	ON_MESSAGE(MCM_GETCURSEL,OnGetCurSel)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT_EX(MCN_GETDAYSTATE,OnGetDayState)
	ON_NOTIFY_EX(TTN_NEEDTEXTA, 0, OnToolTip)
	ON_NOTIFY_EX(TTN_NEEDTEXTW, 0, OnToolTip)
END_MESSAGE_MAP()


BOOL COXMonthCalCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
							 DWORD dwMonthCalStyle/*=OXMCS_BOLDHOLIDAYS|OXMCS_INFOTIP*/)
{
	m_dwMonthCalStyle=dwMonthCalStyle;

	BOOL bRetVal=FALSE;

#if _MFC_VER<=0x0421
	// initialize common controls
	if(!bComCtlInitialized)
	{
		INITCOMMONCONTROLSEX icex;
		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC = ICC_DATE_CLASSES;
		::InitCommonControlsEx(&icex);
		bComCtlInitialized = true;
	}

	CWnd* pWnd = this;
	bRetVal=pWnd->Create(MONTHCAL_CLASS, NULL, dwStyle, rect, pParentWnd, nID);
#else
	bRetVal=CMonthCalCtrl::Create(dwStyle,rect,pParentWnd,nID);
#endif	//_MFC_VER<=0x0421

	if(bRetVal && !InitMonthCalCtrl())
	{
		TRACE(_T("COXMonthCalCtrl::Create: failed to initialize the calendar control"));
		bRetVal=FALSE;
	}

	return bRetVal;
}

BOOL COXMonthCalCtrl::Create(DWORD dwStyle, const POINT& pt, CWnd* pParentWnd, UINT nID, 
							 DWORD dwMonthCalStyle/*=OXMCS_BOLDHOLIDAYS|OXMCS_INFOTIP*/, 
							 int nXDimension/*=1*/, int nYDimension/*=1*/)
{
	ASSERT(nXDimension>=1 && nYDimension>=1);

	if(nXDimension*nYDimension>ID_MAXMONTHCOUNT)
	{
		TRACE(_T("COXMonthCalCtrl::Create: The maximum total number of displayed months is %d\n"),ID_MAXMONTHCOUNT);
		return FALSE;
	}

	BOOL bWasVisible = (dwStyle & WS_VISIBLE);
	dwStyle &= ~WS_VISIBLE;

	CRect rect(pt.x, pt.y, 0, 0);

	BOOL bRetVal = FALSE;
	if(Create(dwStyle, rect, pParentWnd, nID, dwMonthCalStyle))
	{
		if(GetReqRect(rect,nXDimension,nYDimension))
		{
			rect+=pt;
			MoveWindow(rect,FALSE);

			if(bWasVisible)
				ShowWindow(SW_SHOWNA);
		
			// initialize calendar control
			if(!InitMonthCalCtrl())
				TRACE(_T("COXMonthCalCtrl::Create: failed to initialize the calendar control"));
			else
				bRetVal = TRUE;
		}
		else
			DestroyWindow();
	}

	return bRetVal;
}

void COXMonthCalCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	_AFX_THREAD_STATE* pThreadState=AfxGetThreadState();
	// hook not already in progress
	if(pThreadState->m_pWndInit==NULL)
	{
		if(!InitMonthCalCtrl())
			TRACE(_T("COXMonthCalCtrl::PreSubclassWindow: failed to initialize calendar control"));
	}

#if _MFC_VER<=0x0421
	CWnd::PreSubclassWindow();
#else
	CMonthCalCtrl::PreSubclassWindow();
#endif	//	_MFC_VER<=0x0421
}

void COXMonthCalCtrl::OnDestroy()
{
#if _MFC_VER >= 0x0700
	AFX_MODULE_THREAD_STATE* pThreadState = AfxGetModuleState()->m_thread.GetDataNA();
#else
	_AFX_THREAD_STATE* pThreadState=AfxGetThreadState();
#endif
	CToolTipCtrl* pToolTip=pThreadState->m_pToolTip;

	if(pToolTip && ::IsWindow(pToolTip->GetSafeHwnd()))
	{
		pToolTip->DestroyWindow();
		delete pToolTip;
		pThreadState->m_pToolTip=NULL;
		pToolTip=NULL;
	}

#if _MFC_VER<=0x0421
	CWnd::OnDestroy();
#else
	CMonthCalCtrl::OnDestroy();
#endif	//	_MFC_VER<=0x0421
}

LRESULT COXMonthCalCtrl::OnGetCurSel(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	LRESULT result=Default();

	if(result!=0)
	{
		SYSTEMTIME* pSysTime=(SYSTEMTIME*)lParam;
		if(pSysTime->wHour>24)
			pSysTime->wHour=0;
		if(pSysTime->wMinute>60)
			pSysTime->wMinute=0;
		if(pSysTime->wSecond>60)
			pSysTime->wSecond=0;
	}

	return result;
}

BOOL COXMonthCalCtrl::InitMonthCalCtrl()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	// tooltips must be always enabled
	EnableToolTips(m_dwMonthCalStyle&OXMCS_INFOTIP);

	return TRUE;
}



#if _MFC_VER<=0x0421

BOOL COXMonthCalCtrl::SizeMinReq(BOOL bRepaint /* = TRUE */)
{
	CRect rect;
	BOOL bRetVal = FALSE;
	if (GetMinReqRect(rect))
	{
		DWORD dwFlags = SWP_NOZORDER | SWP_NOREPOSITION | SWP_NOMOVE | SWP_NOACTIVATE;
		if (!bRepaint)
			dwFlags |= SWP_NOREDRAW;
		SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), dwFlags);
		bRetVal = TRUE;
	}

	return bRetVal;
}

void COXMonthCalCtrl::SetToday(const COleDateTime& refTime)
{
	ASSERT_VALID(this);

	// make sure the time isn't invalid
	ASSERT(refTime.GetStatus() != COleDateTime::invalid);
	ASSERT(::IsWindow(m_hWnd));

	SYSTEMTIME sysTime;
	LPSYSTEMTIME pSysTime = NULL;
	WPARAM wParam = GDT_NONE;

	// if the passed time is null or out of range,
	// we'll set the control to NULL

	if (GetAsSystemTime(refTime,sysTime))
	{
		pSysTime = &sysTime;
		wParam = GDT_VALID;
	}

	if (::IsWindow(m_hWnd))
		::SendMessage(m_hWnd, MCM_SETTODAY, wParam, (LPARAM) pSysTime);
}

void COXMonthCalCtrl::SetToday(const CTime* pDateTime)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT_VALID(this);

	// if the passed time is NULL, we'll set the
	// control to NULL

	WPARAM wParam = GDT_NONE;
	LPSYSTEMTIME pSysTime = NULL;
	SYSTEMTIME sysTime;

	if (pDateTime != NULL && GetAsSystemTime(*pDateTime,sysTime))
	{
		wParam = GDT_VALID;
		pSysTime = &sysTime;
	}

	if (::IsWindow(m_hWnd))
 		::SendMessage(m_hWnd, MCM_SETTODAY, wParam, (LPARAM) pSysTime);
}

BOOL COXMonthCalCtrl::SetCurSel(const COleDateTime& refTime)
{
	ASSERT(::IsWindow(m_hWnd));

	SYSTEMTIME sysTime;
	BOOL bRetVal = FALSE;

	// if the passed time is null or out of range,
	// we'll set the control to NULL

	if (GetAsSystemTime(refTime,sysTime) &&
		refTime.GetStatus() == COleDateTime::valid)
	{
		bRetVal = (BOOL)
			::SendMessage(m_hWnd, MCM_SETCURSEL, 0, (LPARAM) &sysTime);
	}

	return bRetVal;
}

BOOL COXMonthCalCtrl::SetCurSel(const CTime& refTime)
{
	ASSERT(::IsWindow(m_hWnd));

	SYSTEMTIME sysTime;
	BOOL bRetVal = FALSE;

	if (GetAsSystemTime(refTime,sysTime))
	{
		bRetVal = (BOOL)
			::SendMessage(m_hWnd, MCM_SETCURSEL, 0, (LPARAM) &sysTime);
	}

	return bRetVal;
}

BOOL COXMonthCalCtrl::GetCurSel(COleDateTime& refTime) const
{
	ASSERT(::IsWindow(m_hWnd));

	// can't use this method on multiple selection controls
	ASSERT(!(GetStyle() & MCS_MULTISELECT));

	SYSTEMTIME sysTime;
	BOOL bResult = (BOOL)
		::SendMessage(m_hWnd, MCM_GETCURSEL, 0, (LPARAM) &sysTime);

	if (bResult)
		refTime = COleDateTime(sysTime);

	return bResult;
}

BOOL COXMonthCalCtrl::GetToday(COleDateTime& refTime) const
{
	ASSERT(::IsWindow(m_hWnd));

	// can't use this method on multiple selection controls
	ASSERT(!(GetStyle() & MCS_MULTISELECT));

	SYSTEMTIME sysTime;
	BOOL bResult = (BOOL)
		::SendMessage(m_hWnd, MCM_GETTODAY, 0, (LPARAM) &sysTime);

	if (bResult)
		refTime = COleDateTime(sysTime);

	return bResult;
}

BOOL COXMonthCalCtrl::GetCurSel(CTime& refTime) const
{
	ASSERT(::IsWindow(m_hWnd));

	// can't use this method on multiple selection controls
	ASSERT(!(GetStyle() & MCS_MULTISELECT));

	SYSTEMTIME sysTime;
	BOOL bResult = (BOOL)
		::SendMessage(m_hWnd, MCM_GETCURSEL, 0, (LPARAM) &sysTime);

	if (bResult)
		refTime = CTime(sysTime);

	return bResult;
}

BOOL COXMonthCalCtrl::GetToday(CTime& refTime) const
{
	ASSERT(::IsWindow(m_hWnd));

 	// can't use this method on multiple selection controls
	ASSERT(!(GetStyle() & MCS_MULTISELECT));

	SYSTEMTIME sysTime;
	BOOL bResult = (BOOL)
		::SendMessage(m_hWnd, MCM_GETTODAY, 0, (LPARAM) &sysTime);

	if (bResult)
		refTime = CTime(sysTime);

	return bResult;
}

int COXMonthCalCtrl::GetFirstDayOfWeek(BOOL* pbLocal /* = NULL */) const
{
	ASSERT(::IsWindow(m_hWnd));
	DWORD dwResult;
	dwResult = (DWORD) ::SendMessage(m_hWnd, MCM_GETFIRSTDAYOFWEEK, 0, 0);

	// set *pbLocal to reflect if the first day of week
	// matches current locale setting

	if (pbLocal)
		*pbLocal = HIWORD(dwResult);
	return LOWORD(dwResult);
}

BOOL COXMonthCalCtrl::SetFirstDayOfWeek(int iDay, int* lpnOld /* = NULL */)
{
	ASSERT(::IsWindow(m_hWnd));
	DWORD dwResult;
	dwResult = (DWORD) ::SendMessage(m_hWnd, MCM_SETFIRSTDAYOFWEEK, 0, (WPARAM) iDay);

	if (lpnOld != NULL)
		*lpnOld = LOWORD(dwResult);

	return (BOOL) HIWORD(dwResult);
}

BOOL COXMonthCalCtrl::SetDayState(int nMonths, LPMONTHDAYSTATE pStates)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(AfxIsValidAddress(pStates, nMonths * sizeof(MONTHDAYSTATE), FALSE));
	return (BOOL) ::SendMessage(m_hWnd, MCM_SETDAYSTATE, (WPARAM) nMonths, (LPARAM) pStates);
}

BOOL COXMonthCalCtrl::SetRange(const COleDateTime* pMinRange,
	const COleDateTime* pMaxRange)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pMinRange == NULL || pMinRange->GetStatus() != COleDateTime::invalid);
	ASSERT(pMaxRange == NULL || pMaxRange->GetStatus() != COleDateTime::invalid);

	SYSTEMTIME sysTimes[2];
	WPARAM wFlags = 0;

	if (pMinRange != NULL && pMinRange->GetStatus() != COleDateTime::null)
	{
		if (GetAsSystemTime(*pMinRange,sysTimes[0]))
			wFlags |= GDTR_MIN;
	}

	if (pMaxRange != NULL && pMaxRange->GetStatus() != COleDateTime::null)
	{
		if (GetAsSystemTime(*pMaxRange,sysTimes[1]))
			wFlags |= GDTR_MAX;
	}

	return (BOOL)
		::SendMessage(m_hWnd, MCM_SETRANGE, wFlags, (LPARAM) &sysTimes);
}

BOOL COXMonthCalCtrl::SetRange(const LPSYSTEMTIME pMinRange,
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

	return (BOOL)
		::SendMessage(m_hWnd, MCM_SETRANGE, wFlags, (LPARAM) &sysTimes);
}

DWORD COXMonthCalCtrl::GetRange(COleDateTime* pMinRange,
								COleDateTime* pMaxRange) const
{
	ASSERT(::IsWindow(m_hWnd));

	SYSTEMTIME sysTimes[2];
	memset(sysTimes, 0, sizeof(sysTimes));

	DWORD dwRanges = (DWORD)
		::SendMessage(m_hWnd, MCM_GETRANGE, 0, (LPARAM) &sysTimes);

	if (dwRanges & GDTR_MIN && pMinRange)
		*pMinRange = COleDateTime(sysTimes[0]);

	if (dwRanges & GDTR_MAX && pMaxRange)
		*pMaxRange = COleDateTime(sysTimes[1]);

	return dwRanges;
}

DWORD COXMonthCalCtrl::GetRange(LPSYSTEMTIME pMinRange,
								LPSYSTEMTIME pMaxRange) const
{
	ASSERT(::IsWindow(m_hWnd));

	SYSTEMTIME sysTimes[2];
	memset(sysTimes, 0, sizeof(sysTimes));

	DWORD dwRanges = (DWORD)
		::SendMessage(m_hWnd, MCM_GETRANGE, 0, (LPARAM) &sysTimes);

	if (dwRanges & GDTR_MIN && pMinRange)
		memcpy(pMinRange, &sysTimes[0], sizeof(SYSTEMTIME));

	if (dwRanges & GDTR_MAX && pMaxRange)
		memcpy(pMaxRange, &sysTimes[1], sizeof(SYSTEMTIME));

	return dwRanges;
}

BOOL COXMonthCalCtrl::SetRange(const CTime* pMinRange, const CTime* pMaxRange)
{
	ASSERT(::IsWindow(m_hWnd));

	SYSTEMTIME sysTimes[2];
	WPARAM wFlags = 0;
	if (pMinRange != NULL && GetAsSystemTime(*pMinRange,sysTimes[0]))
		wFlags |= GDTR_MIN;

	if (pMaxRange != NULL && GetAsSystemTime(*pMaxRange,sysTimes[1]))
		wFlags |= GDTR_MAX;

	return (BOOL)
		::SendMessage(m_hWnd, MCM_SETRANGE, wFlags, (LPARAM) &sysTimes);
}

DWORD COXMonthCalCtrl::GetRange(CTime* pMinRange, CTime* pMaxRange) const
{
	ASSERT(::IsWindow(m_hWnd));

	SYSTEMTIME sysTimes[2];
	memset(sysTimes, 0, sizeof(sysTimes));

	DWORD dwRanges = (DWORD)
		::SendMessage(m_hWnd, MCM_GETRANGE, 0, (LPARAM) &sysTimes);

	if (dwRanges & GDTR_MIN && pMinRange)
		*pMinRange = CTime(sysTimes[0]);

	if (dwRanges & GDTR_MAX && pMaxRange)
		*pMaxRange = CTime(sysTimes[1]);

	return dwRanges;
}

int COXMonthCalCtrl::GetMonthRange(COleDateTime& refMinRange,
								   COleDateTime& refMaxRange, 
								   DWORD dwFlags) const
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(dwFlags == GMR_DAYSTATE || dwFlags == GMR_VISIBLE);

	SYSTEMTIME sysTimes[2];
	memset(sysTimes, 0, sizeof(sysTimes));
	int nCount = (int) ::SendMessage(m_hWnd, MCM_GETMONTHRANGE,
		(WPARAM) dwFlags, (LPARAM) &sysTimes);

	refMinRange = COleDateTime(sysTimes[0]);
	refMaxRange = COleDateTime(sysTimes[1]);

	return nCount;
}

int COXMonthCalCtrl::GetMonthRange(LPSYSTEMTIME pMinRange,
								   LPSYSTEMTIME pMaxRange, 
								   DWORD dwFlags) const
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT_POINTER(pMinRange, SYSTEMTIME);
	ASSERT_POINTER(pMaxRange, SYSTEMTIME);

	SYSTEMTIME sysTimes[2];
	
	int nCount = (int) ::SendMessage(m_hWnd, MCM_GETMONTHRANGE,
		(WPARAM) dwFlags, (LPARAM) &sysTimes);

	memcpy(pMinRange, &sysTimes[0], sizeof(SYSTEMTIME));
	memcpy(pMaxRange, &sysTimes[1], sizeof(SYSTEMTIME));

	return nCount;
}

int COXMonthCalCtrl::GetMonthRange(CTime& refMinRange, 
								   CTime& refMaxRange,
								   DWORD dwFlags) const
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(dwFlags == GMR_DAYSTATE || dwFlags == GMR_VISIBLE);

	SYSTEMTIME sysTimes[2];
	memset(sysTimes, 0, sizeof(sysTimes));
	int nCount = (int) ::SendMessage(m_hWnd, MCM_GETMONTHRANGE,
		(WPARAM) dwFlags,	(LPARAM) &sysTimes);

	refMinRange = CTime(sysTimes[0]);
	refMaxRange = CTime(sysTimes[1]);

	return nCount;
}

BOOL COXMonthCalCtrl::GetSelRange(LPSYSTEMTIME pMinRange,
								  LPSYSTEMTIME pMaxRange) const
{
	ASSERT(m_hWnd != NULL);
	ASSERT((GetStyle() & MCS_MULTISELECT));

	ASSERT_POINTER(pMinRange, SYSTEMTIME);
	ASSERT_POINTER(pMaxRange, SYSTEMTIME);

	SYSTEMTIME sysTimes[2];
	BOOL bReturn = (BOOL) ::SendMessage(m_hWnd, MCM_GETSELRANGE,
										0, (LPARAM) &sysTimes);

	if (bReturn)
	{
		memcpy(pMinRange, &sysTimes[0], sizeof(SYSTEMTIME));
		memcpy(pMaxRange, &sysTimes[1], sizeof(SYSTEMTIME));
	}

	return bReturn;
}

BOOL COXMonthCalCtrl::SetSelRange(const LPSYSTEMTIME pMinRange,
								  const LPSYSTEMTIME pMaxRange)
{
	ASSERT(m_hWnd != NULL);
	ASSERT((GetStyle() & MCS_MULTISELECT));

	ASSERT_POINTER(pMinRange, SYSTEMTIME);
	ASSERT_POINTER(pMaxRange, SYSTEMTIME);

	SYSTEMTIME sysTimes[2];
	memcpy(&sysTimes[0], pMinRange, sizeof(SYSTEMTIME));
	memcpy(&sysTimes[1], pMaxRange, sizeof(SYSTEMTIME));

	return (BOOL) ::SendMessage(m_hWnd, MCM_SETSELRANGE,
		0, (LPARAM) &sysTimes);
}

BOOL COXMonthCalCtrl::SetSelRange(const COleDateTime& refMinRange,
								  const COleDateTime& refMaxRange)
{
	// control must have multiple select
	ASSERT((GetStyle() & MCS_MULTISELECT));
	ASSERT(::IsWindow(m_hWnd));

	SYSTEMTIME sysTimes[2];
	BOOL bResult = FALSE;

	if (refMinRange.GetStatus() == COleDateTime::valid &&
		refMinRange.GetStatus() == COleDateTime::valid)
	{
		if (GetAsSystemTime(refMinRange,sysTimes[0]) &&
			GetAsSystemTime(refMaxRange,sysTimes[1]))
		{
			bResult = (BOOL)
				::SendMessage(m_hWnd, MCM_SETSELRANGE, 0, (LPARAM)sysTimes);
		}
	}

	return bResult;
}

BOOL COXMonthCalCtrl::GetSelRange(COleDateTime& refMinRange,
								  COleDateTime& refMaxRange) const
{
	// control must have multiple select
	ASSERT((GetStyle() & MCS_MULTISELECT));
	ASSERT(::IsWindow(m_hWnd));

	SYSTEMTIME sysTimes[2];
	memset(sysTimes, 0, sizeof(sysTimes));
	BOOL bResult = (BOOL)
		::SendMessage(m_hWnd, MCM_GETSELRANGE, 0, (LPARAM) &sysTimes);

	if (bResult)
	{
		refMinRange = COleDateTime(sysTimes[0]);
		refMaxRange = COleDateTime(sysTimes[1]);
	}
	return bResult;
}

BOOL COXMonthCalCtrl::SetSelRange(const CTime& refMinRange,
								  const CTime& refMaxRange)
{
	// control must have multiple select
	ASSERT((GetStyle() & MCS_MULTISELECT));
	ASSERT(::IsWindow(m_hWnd));

	SYSTEMTIME sysTimes[2];
	BOOL bResult = FALSE;

	if (GetAsSystemTime(refMinRange,sysTimes[0]) &&
		GetAsSystemTime(refMaxRange,sysTimes[1]))
	{
		bResult = (BOOL)
			::SendMessage(m_hWnd, MCM_SETSELRANGE, 0, (LPARAM)sysTimes);
	}

	return bResult;
}

BOOL COXMonthCalCtrl::GetSelRange(CTime& refMinRange, CTime& refMaxRange) const
{
	// control must have multiple select
	ASSERT((GetStyle() & MCS_MULTISELECT));
	ASSERT(::IsWindow(m_hWnd));

	SYSTEMTIME sysTimes[2];
	memset(sysTimes, 0, sizeof(sysTimes));
	BOOL bResult = (BOOL)
		::SendMessage(m_hWnd, MCM_GETSELRANGE, 0, (LPARAM) &sysTimes);

	if (bResult)
	{
		refMinRange = CTime(sysTimes[0]);
		refMaxRange = CTime(sysTimes[1]);
	}
	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// DDX_ routines

void AFXAPI DDX_MonthCalCtrl(CDataExchange* pDX, int nIDC,
							 COleDateTime& value)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	COXMonthCalCtrl* pWnd = (COXMonthCalCtrl*) CWnd::FromHandle(hWndCtrl);

	if (pDX->m_bSaveAndValidate)
		pWnd->GetCurSel(value);
	else
		pWnd->SetCurSel(value);
}

void AFXAPI DDX_MonthCalCtrl(CDataExchange* pDX, int nIDC, CTime& value)
{
	HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
	COXMonthCalCtrl* pWnd = (COXMonthCalCtrl*) CWnd::FromHandle(hWndCtrl);

	if (pDX->m_bSaveAndValidate)
		pWnd->GetCurSel(value);
	else
		pWnd->SetCurSel(value);
}

void AFXAPI DDV_MinMaxMonth(CDataExchange* pDX,	CTime& refValue,
							const CTime* pMinRange, const CTime* pMaxRange)
{
	ASSERT(pMinRange == NULL || pMaxRange == NULL || *pMinRange <= *pMaxRange);
	COXMonthCalCtrl* pWnd =
		(COXMonthCalCtrl*) CWnd::FromHandle(pDX->m_hWndLastControl);

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

void AFXAPI DDV_MinMaxMonth(CDataExchange* pDX,	COleDateTime& refValue,
							const COleDateTime* pMinRange, 
							const COleDateTime* pMaxRange)
{
	ASSERT(pMinRange == NULL || pMaxRange == NULL || *pMinRange <= *pMaxRange);
	COXMonthCalCtrl* pWnd =
		(COXMonthCalCtrl*) CWnd::FromHandle(pDX->m_hWndLastControl);

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

#endif	//	_MFC_VER>0x0421


BOOL COXMonthCalCtrl::SetHolidays(LPHOLIDAYS pHolidays)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pHolidays!=NULL);
	ASSERT(pHolidays->nMonth==-1 || (pHolidays->nMonth>=1 && pHolidays->nMonth<=12));
	ASSERT(pHolidays->nYear==-1 || (pHolidays->nYear>=100 && pHolidays->nYear<=9999));

	LPHOLIDAYS pNewHolidays=NULL; 

	DWORD dwMonthYear=pHolidays->nMonth+pHolidays->nYear*100;


	if (m_arrHolidays.Lookup (dwMonthYear,pNewHolidays)) 
      { 
      if (pNewHolidays) 
         delete pNewHolidays; 
      } 

	pNewHolidays=new HOLIDAYS; 
	*pNewHolidays=*pHolidays; 

	m_arrHolidays.SetAt(dwMonthYear,pNewHolidays);

	return TRUE;
}

BOOL COXMonthCalCtrl::GetHolidays(LPHOLIDAYS pDestHolidays)
{
	ASSERT(pDestHolidays->nMonth>=1 && pDestHolidays->nMonth<=12);
	ASSERT(pDestHolidays->nYear>=100 && pDestHolidays->nYear<=9999);
	ASSERT(pDestHolidays!=NULL);

	pDestHolidays->arrWeekDays.RemoveAll();
	pDestHolidays->arrMonthDays.RemoveAll();

	DWORD dwMonthYear;
	LPHOLIDAYS pHolidays;
	POSITION pos=m_arrHolidays.GetStartPosition();
	while(pos!=NULL)
	{
		m_arrHolidays.GetNextAssoc(pos,dwMonthYear,pHolidays);

		ASSERT(pHolidays!=NULL);
		ASSERT(pHolidays->nMonth==-1 || (pHolidays->nMonth>=1 && pHolidays->nMonth<=12));
		ASSERT(pHolidays->nYear==-1 || (pHolidays->nYear>=100 && pHolidays->nYear<=9999));

		POSITION posList;
		if((pHolidays->nMonth==-1 && pHolidays->nYear==-1) || 
			(pHolidays->nMonth==-1 && pHolidays->nYear==pDestHolidays->nYear) || 
			(pHolidays->nMonth==pDestHolidays->nMonth && pHolidays->nYear==-1) ||
			(pHolidays->nMonth==pDestHolidays->nMonth && 
			pHolidays->nYear==pDestHolidays->nYear)) 
		{
			posList=pHolidays->arrWeekDays.GetHeadPosition();
			while(posList!=NULL)
			{
				WEEKDAYS weekDay=pHolidays->arrWeekDays.GetNext(posList);
				if(!pDestHolidays->arrWeekDays.Find(weekDay))
					pDestHolidays->arrWeekDays.AddTail(weekDay);
			}

			posList=pHolidays->arrMonthDays.GetHeadPosition();
			while(posList!=NULL)
			{
				MONTHDAYS monthDay=pHolidays->arrMonthDays.GetNext(posList);
				if(!pDestHolidays->arrMonthDays.Find(monthDay))
					pDestHolidays->arrMonthDays.AddTail(monthDay);
			}
		}
	}

	return TRUE;
}

BOOL COXMonthCalCtrl::SetCtrlDimension(int nXDimension, int nYDimension,
									   const CSize& szMargins/*=CSize(0,0)*/)
{
	ASSERT(::IsWindow(m_hWnd));

	if(nXDimension*nYDimension>ID_MAXMONTHCOUNT)
	{
		TRACE(_T("COXMonthCalCtrl::SetCtrlDimension: The maximum total number of displayed months is %d\n"),ID_MAXMONTHCOUNT);
		return FALSE;
	}

	CRect rect;
	GetWindowRect(rect);
	CPoint ptTopLeft=rect.TopLeft();

	CWnd* pWnd=GetParent();
	if(pWnd!=NULL)
	{
		ASSERT(::IsWindow(pWnd->GetSafeHwnd()));
		pWnd->ScreenToClient(&ptTopLeft);
	}

	BOOL bResult=FALSE;
	if(GetReqRect(rect,nXDimension,nYDimension))
	{
		rect.right+=szMargins.cx*2;
		rect.bottom+=szMargins.cy*2;
		CalcWindowRect(rect);
		if(SetWindowPos(NULL,ptTopLeft.x,ptTopLeft.y,rect.Width(),rect.Height(),
			SWP_NOZORDER|SWP_NOACTIVATE))
			bResult=TRUE;
	}

	return bResult;
}

BOOL COXMonthCalCtrl::GetMonthFitCount(CSize& szMonthFit) const
{
	ASSERT(::IsWindow(m_hWnd));

	CRect rectMin;
	if(!GetMinReqRect(rectMin))
		return FALSE;

	CRect rect;
	GetClientRect(rect);

	int nWidth=rect.Width();
	int nWidthMin=rectMin.Width();
	int nHorzFit=nWidth/nWidthMin;
	if(nHorzFit!=0)
	{
		nWidth-=ID_MCHORZGAP*(nHorzFit-1);
		nHorzFit=nWidth/nWidthMin;
	}
	nHorzFit=(nHorzFit>1 ? nHorzFit : 1);


	int nHeight=rect.Height();
	int nHeightMin=rectMin.Height();
	int nVertFit=nHeight/nHeightMin;
	if(nVertFit!=0)
	{
		CFont* pFont=GetFont();
		ASSERT(pFont!=NULL);
		LOGFONT lf;
		VERIFY(pFont->GetLogFont(&lf));
		nHeight+=abs(lf.lfHeight)*(nVertFit);

		nVertFit=nHeight/nHeightMin;
	}
	nVertFit=(nVertFit>1 ? nVertFit : 1);

	if(nHorzFit>ID_MAXMONTHCOUNT)
	{
		nHorzFit=ID_MAXMONTHCOUNT;
		nVertFit=1;
	}
	else if(nHorzFit*nVertFit>ID_MAXMONTHCOUNT)
		nVertFit=ID_MAXMONTHCOUNT/nHorzFit;

	
	szMonthFit.cx=nHorzFit;
	szMonthFit.cy=nVertFit;

	return TRUE;
}

BOOL COXMonthCalCtrl::GetReqRect(RECT* pRect, int nXDimension, int nYDimension) const
{
	COXMonthCalCtrl ctlDummy;
	if(ctlDummy.Create((GetStyle()&~(MCS_NOTODAY|WS_POPUP)),
		CRect(0,0,0,0),(CWnd*)this,100) && ctlDummy.GetMinReqRect(pRect))
	{
		pRect->right*=nXDimension;
		pRect->right+=ID_MCHORZGAP*(nXDimension-1);
		pRect->bottom*=nYDimension;

		CFont* pFont=GetFont();
		ASSERT(pFont!=NULL);
		LOGFONT lf;
		VERIFY(pFont->GetLogFont(&lf));
		pRect->bottom-=abs(lf.lfHeight)*(nYDimension-1);

		CWnd* pWnd=(CWnd*)this;
		pWnd->CalcWindowRect(pRect);
		pRect->right-=pRect->left;
		pRect->bottom-=pRect->top;
		pRect->left=0;
		pRect->top=0;

		return TRUE;
	}
	else
		return FALSE;
}

BOOL COXMonthCalCtrl::OnGetDayState(NMHDR *pHdr, LRESULT *pRes)
{
	NMDAYSTATE *pds=(NMDAYSTATE*)pHdr;

	COleDateTime dtMin,dtMax;
	GetMonthRange(dtMin,dtMax,GMR_DAYSTATE);
	int nMonth=dtMin.GetMonth();
	int nYear=dtMin.GetYear();
	for(int nIndex=0; nIndex<pds->cDayState; nIndex++)
	{
		HOLIDAYS holidays;
		holidays.nMonth=nMonth;
		holidays.nYear=nYear;
		VERIFY(GetHolidays(&holidays));

		OXNMDAYSTATE nmds;
		nmds.nmhdr.code=OXMCN_GETDAYSTATE;
		nmds.nMonth=holidays.nMonth;
		nmds.nYear=holidays.nYear;
		nmds.parrDayState=new ARRMONTHDAYS;

		if((GetMonthCalStyle()&OXMCS_BOLDHOLIDAYS)!=0)
		{
			POSITION posList=holidays.arrWeekDays.GetHeadPosition();
			while(posList!=NULL)
			{
				WEEKDAYS weekDay=holidays.arrWeekDays.GetNext(posList);
				COleDateTime dtDummy;
				for(int nDay=1; nDay<=31; nDay++)
				{
					dtDummy.SetDate(nmds.nYear,nmds.nMonth,nDay);
					if(dtDummy.GetDayOfWeek()==GetAsCOleDateTimeDayOfWeek(weekDay) &&
						!nmds.parrDayState->Find((MONTHDAYS)nDay))
						nmds.parrDayState->AddTail((MONTHDAYS)nDay);
				}
			}

			posList=holidays.arrMonthDays.GetHeadPosition();
			while(posList!=NULL)
			{
				MONTHDAYS monthDay=holidays.arrMonthDays.GetNext(posList);
				if(!nmds.parrDayState->Find(monthDay))
					nmds.parrDayState->AddTail(monthDay);
			}
		}

		// notify parent
		CWnd* pParentWnd=GetParent();
		if(pParentWnd)
		{
			// fill notification structure
			nmds.nmhdr.hwndFrom=GetSafeHwnd();
			nmds.nmhdr.idFrom=GetDlgCtrlID();

			pParentWnd->SendMessage(WM_NOTIFY,(WPARAM)GetDlgCtrlID(),(LPARAM)&nmds);
		}

		pds->prgDayState[nIndex]=0;
		POSITION posList=nmds.parrDayState->GetHeadPosition();
		while(posList!=NULL)
		{
			MONTHDAYS monthDay=nmds.parrDayState->GetNext(posList);
			BOLDDAY(pds->prgDayState[nIndex],monthDay);
		}

		nMonth++;
		if(nMonth>ID_MAXMONTHCOUNT)
		{
			nMonth=1;
			nYear++;
		}

	}

	*pRes=0;

	return FALSE;
}

int COXMonthCalCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(GetSafeHwnd()));

	// the control should be designated to provide tooltips
	if((GetMonthCalStyle()&OXMCS_INFOTIP)==0)
		return -1;

	pTI->hwnd=GetSafeHwnd();

	GetClientRect(&pTI->rect);

	MCHITTESTINFO mcHitTest;
	::ZeroMemory((void*)&mcHitTest,sizeof(mcHitTest));
	mcHitTest.cbSize=sizeof(mcHitTest);
	mcHitTest.pt=point;
///	HitTest(&mcHitTest);
	::SendMessage(GetSafeHwnd(),MCM_HITTEST,0,(LPARAM)&mcHitTest);

	pTI->uId=mcHitTest.uHit;

	pTI->uFlags&=~(TTF_IDISHWND);

	// set text to LPSTR_TEXTCALLBACK in order to get TTN_NEEDTEXT message when 
	// it's time to display tool tip
	pTI->lpszText=LPSTR_TEXTCALLBACK;

	return PtrToInt(pTI->uId);
}

BOOL COXMonthCalCtrl::OnToolTip(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{   
	UNREFERENCED_PARAMETER(id);

	ASSERT(pNMHDR->code==TTN_NEEDTEXTA || pNMHDR->code==TTN_NEEDTEXTW);

	DWORD dwMessagePos=::GetMessagePos();
	CPoint point(GET_X_LPARAM(dwMessagePos),GET_Y_LPARAM(dwMessagePos));
	ScreenToClient(&point);
	// now hit test against COXMonthCalCtrl elements
	MCHITTESTINFO mcHitTest;
	::ZeroMemory((void*)&mcHitTest,sizeof(mcHitTest));
	mcHitTest.cbSize=sizeof(mcHitTest);
	mcHitTest.pt=point;
	HitTest(&mcHitTest);

	ASSERT(pNMHDR->idFrom==mcHitTest.uHit);

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA=(TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW=(TOOLTIPTEXTW*)pNMHDR;
    
	// idFrom must be the handle to the group
	if (pNMHDR->code==TTN_NEEDTEXTA)
		ASSERT((pTTTA->uFlags&TTF_IDISHWND)==0);
	else
		ASSERT((pTTTW->uFlags&TTF_IDISHWND)==0);

	OXNMMCINFOTIP infoTip;
	// fill structure for notification
	infoTip.nmhdr.code=OXMCN_GETINFOTIP;
	infoTip.pMCHitTest=&mcHitTest;

	// notify parent
	CWnd* pParentWnd=GetParent();
	if(pParentWnd)
	{
		// fill notification structure
		infoTip.nmhdr.hwndFrom=GetSafeHwnd();
		infoTip.nmhdr.idFrom=GetDlgCtrlID();

		pParentWnd->SendMessage(WM_NOTIFY,(WPARAM)GetDlgCtrlID(),(LPARAM)&infoTip);
	}

#ifndef _UNICODE
	if(pNMHDR->code==TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText,infoTip.szText,countof(pTTTA->szText));
	else
		_mbstowcsz(pTTTW->szText,infoTip.szText,countof(pTTTW->szText));
#else
	if (pNMHDR->code==TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText,infoTip.szText,countof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText,infoTip.szText,countof(pTTTW->szText));
#endif

	if(pNMHDR->code==TTN_NEEDTEXTA)
	{
		if(infoTip.lpszText==NULL)
			pTTTA->lpszText=pTTTA->szText;
		else
			pTTTA->lpszText=(LPSTR)infoTip.lpszText;
		pTTTA->hinst=infoTip.hinst;
	}
	else
	{
		if(infoTip.lpszText==NULL)
			pTTTW->lpszText=pTTTW->szText;
		else
			pTTTW->lpszText=(LPWSTR)infoTip.lpszText;
		pTTTW->hinst=infoTip.hinst;
	}

	*pResult=0;

	return TRUE;    // message was handled
}

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

#if _MFC_VER<=0x0421

BOOL COXMonthCalCtrl::GetAsSystemTime(const COleDateTime& rDateTime, SYSTEMTIME& sysTime)
{
	BOOL bRetVal = FALSE;
	if(rDateTime.GetStatus() == COleDateTime::valid)
	{
		struct tm tmTemp;
		if (TimeFromOleDate(rDateTime.m_dt, tmTemp))
		{
			sysTime.wYear = (WORD) tmTemp.tm_year;
			sysTime.wMonth = (WORD) tmTemp.tm_mon;
			sysTime.wDayOfWeek = (WORD) tmTemp.tm_wday;
			sysTime.wDay = (WORD) tmTemp.tm_mday;
			sysTime.wHour = (WORD) tmTemp.tm_hour;
			sysTime.wMinute = (WORD) tmTemp.tm_min;
			sysTime.wSecond = (WORD) tmTemp.tm_sec;
			sysTime.wMilliseconds = 0;

			bRetVal = TRUE;
		}
	}

	return bRetVal;
}

BOOL COXMonthCalCtrl::GetAsSystemTime(const CTime& rTime, SYSTEMTIME& timeDest)
{
	struct tm* ptm = rTime.GetLocalTm(NULL);
	if (ptm == NULL)
		return FALSE;

	timeDest.wYear = (WORD) (1900 + ptm->tm_year);
	timeDest.wMonth = (WORD) (1 + ptm->tm_mon);
	timeDest.wDay = (WORD) ptm->tm_mday;
	timeDest.wHour = (WORD) ptm->tm_hour;
	timeDest.wMinute = (WORD) ptm->tm_min;
	timeDest.wSecond = (WORD) ptm->tm_sec;
	timeDest.wMilliseconds = 0;

	return TRUE;
}


BOOL COXMonthCalCtrl::TimeFromOleDate(DATE dtSrc, struct tm& tmDest)
{
	// The legal range does not actually span year 0 to 9999.
	if (dtSrc > MAX_DATE || dtSrc < MIN_DATE) // about year 100 to about 9999
		return FALSE;

	long nDays;             // Number of days since Dec. 30, 1899
	long nDaysAbsolute;     // Number of days since 1/1/0
	long nSecsInDay;        // Time in seconds since midnight
	long nMinutesInDay;     // Minutes in day

	long n400Years;         // Number of 400 year increments since 1/1/0
	long n400Century;       // Century within 400 year block (0,1,2 or 3)
	long n4Years;           // Number of 4 year increments since 1/1/0
	long n4Day;             // Day within 4 year block
							//  (0 is 1/1/yr1, 1460 is 12/31/yr4)
	long n4Yr;              // Year within 4 year block (0,1,2 or 3)
	BOOL bLeap4 = TRUE;     // TRUE if 4 year block includes leap year

	double dblDate = dtSrc; // tempory serial date

	// If a valid date, then this conversion should not overflow
	nDays = (long)dblDate;

	// Round to the second
	dblDate += ((dtSrc > 0.0) ? HALF_SECOND : -HALF_SECOND);

	nDaysAbsolute = (long)dblDate + 693959L; // Add days from 1/1/0 to 12/30/1899

	dblDate = fabs(dblDate);
	nSecsInDay = (long)((dblDate - floor(dblDate)) * 86400.);

	// Calculate the day of week (sun=1, mon=2...)
	//   -1 because 1/1/0 is Sat.  +1 because we want 1-based
	tmDest.tm_wday = (int)((nDaysAbsolute - 1) % 7L) + 1;

	// Leap years every 4 yrs except centuries not multiples of 400.
	n400Years = (long)(nDaysAbsolute / 146097L);

	// Set nDaysAbsolute to day within 400-year block
	nDaysAbsolute %= 146097L;

	// -1 because first century has extra day
	n400Century = (long)((nDaysAbsolute - 1) / 36524L);

	// Non-leap century
	if (n400Century != 0)
	{
		// Set nDaysAbsolute to day within century
		nDaysAbsolute = (nDaysAbsolute - 1) % 36524L;

		// +1 because 1st 4 year increment has 1460 days
		n4Years = (long)((nDaysAbsolute + 1) / 1461L);

		if (n4Years != 0)
			n4Day = (long)((nDaysAbsolute + 1) % 1461L);
		else
		{
			bLeap4 = FALSE;
			n4Day = (long)nDaysAbsolute;
		}
	}
	else
	{
		// Leap century - not special case!
		n4Years = (long)(nDaysAbsolute / 1461L);
		n4Day = (long)(nDaysAbsolute % 1461L);
	}

	if (bLeap4)
	{
		// -1 because first year has 366 days
		n4Yr = (n4Day - 1) / 365;

		if (n4Yr != 0)
			n4Day = (n4Day - 1) % 365;
	}
	else
	{
		n4Yr = n4Day / 365;
		n4Day %= 365;
	}

	// n4Day is now 0-based day of year. Save 1-based day of year, year number
	tmDest.tm_yday = (int)n4Day + 1;
	tmDest.tm_year = n400Years * 400 + n400Century * 100 + n4Years * 4 + n4Yr;

	// Handle leap year: before, on, and after Feb. 29.
	if (n4Yr == 0 && bLeap4)
	{
		// Leap Year
		if (n4Day == 59)
		{
			/* Feb. 29 */
			tmDest.tm_mon = 2;
			tmDest.tm_mday = 29;
			goto DoTime;
		}

		// Pretend it's not a leap year for month/day comp.
		if (n4Day >= 60)
			--n4Day;
	}

	// Make n4DaY a 1-based day of non-leap year and compute
	//  month/day for everything but Feb. 29.
	++n4Day;

	// Month number always >= n/32, so save some loop time */
	for (tmDest.tm_mon = (n4Day >> 5) + 1;
		n4Day > arrMonthStartDay[tmDest.tm_mon]; tmDest.tm_mon++);

	tmDest.tm_mday = (int)(n4Day - arrMonthStartDay[tmDest.tm_mon-1]);

DoTime:
	if (nSecsInDay == 0)
		tmDest.tm_hour = tmDest.tm_min = tmDest.tm_sec = 0;
	else
	{
		tmDest.tm_sec = (int)nSecsInDay % 60L;
		nMinutesInDay = nSecsInDay / 60L;
		tmDest.tm_min = (int)nMinutesInDay % 60;
		tmDest.tm_hour = (int)nMinutesInDay / 60;
	}

	return TRUE;
}

#endif	//	_MFC_VER<=0x0421


int COXMonthCalCtrl::GetAsCOleDateTimeDayOfWeek(const WEEKDAYS weekDay)
{
	switch(weekDay)
	{
	case MONDAY:
		return 2;
	case TUESDAY:
		return 3;
	case WENDSDAY:
		return 4;
	case THURSDAY:
		return 5;
	case FRIDAY:
		return 6;
	case SATURDAY:
		return 7;
	case SUNDAY:
		return 1;
	default:
		return -1;
	}
}

