// ==========================================================================
//				Class Implementation :COXCalendarPopup
// ==========================================================================

// Source file :COXCalendarPopup.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
			  
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXCalendarPopup.h"
#include "OXCalendar.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* define message to inform that date selected */
#define WM_ENDPICK	WM_USER+10
// Data members -------------------------------------------------------------
//	protected:
//		int m_nFirstDayOfWeek;
//--- number of first day of week
// this value stored in internal format
// 0 - Monday 1 - Tuesday, ect., 6 - Sunday

//		CButton m_ButtonMonth;
//--- button displayed current month and year
//		CButton m_ButtonNone;
//- button to chose none date
//		CButton m_ButtonToday;
//- button to chose today's date
//		CString m_sNone;
//--- name of <None> button
//		CString m_sToday;
//--- name of <Today> button
//		CRect m_rectGrid;
//--- specified size and position of
//    days grid relative to client area
//		CSize m_SizeWnd;
//--- size of calendar window
//		CSize m_szCell;
//--- size of one cell displayed day number
//		CFont m_font;
// window font
//		CRect m_rectRightNavBar;
//--- size and position of right navigation arrow
// relative to client area
//		CRect m_rectLeftNavBar;
//--- size and position of left navigation arrow
// relative to client area
//		COleDateTime m_DatePicked;
// store selected date 
//		COleDateTime m_DateToday;
// store today's date
//	private:
//		COleDateTime m_InitialSelectedDate;
// previous selected date (showing inversed)
//		COleDateTime m_DateEnd;
//		COleDateTime m_DateStart;
// values specified valid range of date
//		COleDateTime m_CurrentMonth;
// current month showing
//		int m_nTick;
// if 0 - no timer set,
//    1 - timer set to repeate jump to prev month
//    2 - timer set to repeate jump to next month

// 
COXCalendarPopup::COXCalendarPopup()
	// --- In  :
	// --- Out :
	// --- Returns: 
	// --- Effect : constructor
{
	VERIFY(m_sToday.LoadString(IDS_OX_CALENDARTODAY)); //"Today"
	VERIFY(m_sNone.LoadString(IDS_OX_CALENDARNONE)); //"None"
	m_nTick = 0; // no timer set
	m_nDragMode=dragNothing;//no dragging

	m_nFirstDayOfWeek=COXCalendar::GetLocaleFirstDayOfWeek();
	if(COXCalendar::ms_straWeekDays.GetSize()==0 || 
		COXCalendar::ms_straMonthNames.GetSize()==0)
	{
		COXCalendar::GetCalendarLocaleData();
	}
}

COXCalendarPopup::~COXCalendarPopup()
{
	if(::IsWindow(m_hWnd))
	{
		DestroyWindow();
	}
}


BEGIN_MESSAGE_MAP(COXCalendarPopup, CWnd)
	//{{AFX_MSG_MAP(COXCalendarPopup)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SETTINGCHANGE,OnSettingChanged)
END_MESSAGE_MAP()



BOOL COXCalendarPopup::Create(COXCalendar * pBaseCalendar)
	// --- In  :
	// --- Out :
	// --- Returns: TRUE if window created successfully,
	//				FALSE otherwise
	// --- Effect : 
	{
	static CString sClassName;
	if(sClassName.IsEmpty())
		{
		sClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,0,
			(HBRUSH)GetStockObject(WHITE_BRUSH));
		}
 
	if(!CWnd::CreateEx(WS_EX_OVERLAPPEDWINDOW |WS_EX_STATICEDGE | 
		WS_EX_TOOLWINDOW |WS_EX_DLGMODALFRAME,
		sClassName,_T(""),WS_POPUP|WS_BORDER,0,0,0,0,
		NULL,NULL,NULL))
		{
		TRACE(_T("COXCalendarPopup::Create fail!"));
		return FALSE;
		}
	if(pBaseCalendar)
		{
		ASSERT(::IsWindow(pBaseCalendar->m_hWnd));
		// Set default font from COXCalendar edit control
		LOGFONT lf;
		pBaseCalendar->m_font.GetLogFont(&lf);
		m_font.CreateFontIndirect(&lf);
		}
// Now performing size calculation
	CalcWindowSize();
	if(!m_ButtonToday.Create(m_sToday,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,100))
		{
		// failed to create today button
		ASSERT(FALSE);
		DestroyWindow();
		return FALSE;
		}
	if(!m_ButtonNone.Create(m_sNone,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,101))
		{
		// failed to create none button
		ASSERT(FALSE);
		DestroyWindow();
		return FALSE;
		}
	if(!m_ButtonMonth.Create(m_sNone,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,102))
	{
		// failed to create month button
		ASSERT(FALSE);
		DestroyWindow();
		return FALSE;
	}
	SetButtons();
	if(pBaseCalendar)
	{
		// Set default font from COXCalendar edit control
		m_ButtonToday.SetFont(&m_font);
		m_ButtonNone.SetFont(&m_font);
		m_ButtonMonth.SetFont(&m_font);
	}

	return TRUE;
}


void COXCalendarPopup::CalcCellSize()
{
	// --- In  :
	// --- Out :
	// --- Returns: 
	// --- Effect : calculate size of elemental cell that show day
	ASSERT(::IsWindow(m_hWnd));
	CClientDC dc(this);
	CFont *oldfont = dc.SelectObject(&m_font);
	//assumed that all digits have approximately equal size
	CString strSampleDigits("0123456789");
	m_szCell = dc.GetTextExtent(strSampleDigits);
	m_szCell.cx =m_szCell.cx / 5; // One cell is kept 2 digits
	m_szCell.cy += m_szCell.cy / 5;
	m_szCell.cx += m_szCell.cx/4;
	dc.SelectObject(oldfont);
}

CSize COXCalendarPopup::CalcWindowSize()
{
	ASSERT(::IsWindow(m_hWnd));
	CalcCellSize();
	ASSERT(COXCalendar::ms_straMonthNames.GetSize() == 12);

	// calculate width of month navigatoin bar

	// now add size of left and right navigation arrows

	//now calculate a size of grid that show days
	int nGridX = m_szCell.cx*7,nGridY=m_szCell.cy*6;
	int nMonthBarExtentX,nMonthBarExtentY;
	int nArrowX=::GetSystemMetrics(SM_CXHSCROLL);

	nMonthBarExtentX = CalcBarWidth();
	nMonthBarExtentY = m_szCell.cy;
	m_SizeWnd.cx = __max(nMonthBarExtentX,nGridX+nArrowX*2);
	m_SizeWnd.cy = nMonthBarExtentY + nGridY 
		+ m_szCell.cy*3;// One cell for weekdays, other for bottom buttons

	// add borders
	m_SizeWnd.cx += ::GetSystemMetrics(SM_CXEDGE)*2;
	m_SizeWnd.cy += ::GetSystemMetrics(SM_CYEDGE)*2;
	
	SetWindowPos(NULL,0,0,m_SizeWnd.cx,m_SizeWnd.cy,SWP_NOACTIVATE | SWP_NOZORDER);
	
	CRect rClient;
	GetClientRect(&rClient);
	m_rectGrid.top = nMonthBarExtentY+m_szCell.cy + 2;
	m_rectGrid.left = (rClient.Width() - nGridX)/2;
	m_rectGrid.BottomRight()=m_rectGrid.TopLeft()+CPoint(nGridX,nGridY);

	GetClientRect(&m_rectLeftNavBar);
	m_rectLeftNavBar.bottom = nMonthBarExtentY;
	m_rectLeftNavBar.right = nArrowX;
	GetClientRect(&m_rectRightNavBar);
	m_rectRightNavBar.left = m_rectRightNavBar.right - nArrowX;
	m_rectRightNavBar.bottom = nMonthBarExtentY;
	return m_SizeWnd;
}

BOOL  COXCalendarPopup::Pick(COleDateTime& pickDate,
							 const COleDateTime& startDate,
							 const COleDateTime& endDate,
							 CRect rectCalendar)
	// --- In  : valid date range to show,window rect of major COXCalendar control
	// --- Out : date selected from calendar
	// --- Returns: TRUE if user select date from calendar window,
				//FALSE if user cancel selection
	// --- Effect : display calendar window, process date selections
	{
	if(startDate.GetStatus() == 0 && endDate.GetStatus() == 0
		&& startDate >= endDate)
		{
		TRACE(_T("COXCalendarPopup::Pick: passed StartDate > EndDate!"));
		ASSERT(FALSE);
		return FALSE;
		}
	m_DateToday = COleDateTime::GetCurrentTime();
	m_DateToday.m_dt = (int)m_DateToday.m_dt;
	int x = __max(0,rectCalendar.right - m_SizeWnd.cx);
	int y;
	BOOL bPicked = FALSE;
	if(rectCalendar.bottom + m_SizeWnd.cy >= ::GetSystemMetrics(SM_CYSCREEN))
		y = rectCalendar.top -  m_SizeWnd.cy; //drop Up
	else
		y = rectCalendar.bottom;
	m_DateStart = startDate;
	m_DateEnd   = endDate;
	m_InitialSelectedDate = pickDate;
	// clear time value for date comparation
	m_DateStart.m_dt = (int)m_DateStart.m_dt;
	m_DateEnd.m_dt = (int)m_DateEnd.m_dt;
	m_InitialSelectedDate.m_dt = (int)m_InitialSelectedDate.m_dt;

	m_ButtonToday.EnableWindow(m_DateToday >= m_DateStart &&
		m_DateToday <= m_DateEnd);

	if(m_InitialSelectedDate.GetStatus() == 0)//valid date
		m_CurrentMonth = m_InitialSelectedDate;
	else
		{
		m_CurrentMonth = COleDateTime::GetCurrentTime();
		m_CurrentMonth.m_dt = (int)m_CurrentMonth.m_dt;
		if(m_DateStart.GetStatus() == 0 && m_CurrentMonth < m_DateStart)
			{
			m_CurrentMonth = m_DateStart;
			}
		if(m_DateEnd.GetStatus() == 0 && m_CurrentMonth > m_DateEnd)
			{
			m_CurrentMonth = m_DateEnd;
			}
			
		}
	// show calendar window
	SetWindowPos(NULL,x,y,m_SizeWnd.cx,m_SizeWnd.cy,SWP_SHOWWINDOW|SWP_NOZORDER);
	ShowWindow(SW_SHOW);
	SetCapture();
	// init message loop
	for(;;)
		{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		if (::GetCapture() != m_hWnd)
			break;
		switch (msg.message)
			{
			case WM_ENDPICK:
				// date selection finished, hide window now
				bPicked = (BOOL)msg.wParam;// if TRUE, date has been selected,
				// otherwise, user canceled date selection
				goto ExitLoop;
			case WM_KEYDOWN:
				if (msg.wParam != VK_ESCAPE)
					break;
			case WM_RBUTTONDOWN:
				goto ExitLoop;
				break;
			default:
				DispatchMessage(&msg);
				break;
			}

		}
ExitLoop:
	if (::GetCapture() == m_hWnd)
		ReleaseCapture();
	ShowWindow(SW_HIDE);
	if(bPicked) //date has been selected
		pickDate = m_DatePicked;
	return bPicked;
}


void COXCalendarPopup::OnLButtonDown(UINT /* nFlags */, CPoint point) 
	{
	CRect rWnd;
	GetWindowRect(&rWnd);
	CPoint p = point;
	ClientToScreen(&p);
	if(!rWnd.PtInRect(p))
		{
		PostMessage(WM_ENDPICK);// now cancel showing 
		return;
		}
	int nHitTest = GridHitTest(point);
	if(nHitTest != -1)
		{
		m_nDragMode = dragGrid;
		GetFirstShownDate(m_DatePicked);
		m_DatePicked += nHitTest;

		if ( m_InitialSelectedDate.GetStatus() != COleDateTimeSpan::valid )
			m_InitialSelectedDate = m_DatePicked;
		else if ( m_InitialSelectedDate != m_DatePicked )
			{
			InvalidateSelChange(m_InitialSelectedDate,m_DatePicked);
			m_InitialSelectedDate = m_DatePicked;
			}
		return;
		}
	if(ButtonTodayHit(point))	
		{
		// user press <Today> button and Today is between
		// Start and End Date values
		m_nDragMode = dragTodayButton;
		m_DatePicked = m_DateToday;
		m_ButtonToday.SetState(TRUE);
		return;
		}
	if(ButtonNoneHit(point))
		{
		m_nDragMode = dragNoneButton;
		m_DatePicked.m_dt = 0;
		m_DatePicked.SetStatus(COleDateTime::null);
		m_ButtonNone.SetState(TRUE);
		return;
		}
	// otherwise process check for navigation arrows
	DoNavigationCheck(point);
	}

void COXCalendarPopup::OnPaint() 
	{
	CPaintDC dc(this); // device context for painting

	CPen RedPen(PS_SOLID,1,RGB(192,0,0)); // pen for outline today's date
	CFont *oldfont=NULL;
	static COLORREF clrLight = ::GetSysColor(COLOR_3DSHADOW),
			clrNormal = 0,
			clrHighLightBg = ::GetSysColor(COLOR_HIGHLIGHT),
			clrHighLightText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	CRect rClip,r;int i;
	dc.GetClipBox(&rClip);
	//draw navigation arrows
	if(r.IntersectRect(rClip,m_rectLeftNavBar))
		dc.DrawFrameControl(&m_rectLeftNavBar,DFC_SCROLL,DFCS_SCROLLLEFT);
	if(r.IntersectRect(rClip,m_rectRightNavBar))
	dc.DrawFrameControl(&m_rectRightNavBar,DFC_SCROLL,DFCS_SCROLLRIGHT);

	CString sMonth;
	// set current showing month name and year
	sMonth = COXCalendar::ms_straMonthNames[m_CurrentMonth.GetMonth() -1] + 
		m_CurrentMonth.Format(_T(" %Y"));
	m_ButtonMonth.SetWindowText(sMonth);
	oldfont = dc.SelectObject(&m_font);
	dc.SelectStockObject(BLACK_PEN);
	COleDateTime DateShow;
	GetFirstShownDate(DateShow);
	dc.SetTextColor(clrNormal);
	CRect rCell;
	// draw days of week
	r.SetRect(0,m_rectGrid.top - m_szCell.cy,
		m_rectRightNavBar.right,m_rectGrid.top);
	if(r.IntersectRect(rClip,r))
	for(i=0;i < 7;i++)
		{
		///
		CString sWeekDay = (CString)COXCalendar::ms_straWeekDays[(i + m_nFirstDayOfWeek)%7][0];
		ASSERT(!sWeekDay.IsEmpty());
		sWeekDay=sWeekDay.Left(1);
		///
		rCell.SetRect(m_rectGrid.left + m_szCell.cx*i,
						m_rectGrid.top - m_szCell.cy,
						m_rectGrid.left + m_szCell.cx*(i+1),
						m_rectGrid.top);
		dc.DrawText(sWeekDay,&rCell,DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
		}
	// draw separator lines
	dc.MoveTo(m_rectGrid.left,m_rectGrid.top -1);
	dc.LineTo(m_rectGrid.right,m_rectGrid.top-1);
	dc.MoveTo(m_rectGrid.right,m_rectGrid.bottom+1);
	dc.LineTo(m_rectGrid.left,m_rectGrid.bottom+1);
	// draw calendar grid
	for(i=0;i < 6;i++)
		for(int j=0;j < 7;j++)
			{
			if(DateShow.GetStatus() != COleDateTime::valid ||
				DateShow.GetDay() < 0)
				{
				DateShow.m_dt += 1;
				continue;// ignore invalid dates (like > 12/31/9999)
				}

			rCell.TopLeft()=m_rectGrid.TopLeft();
			rCell.left += m_szCell.cx*j;
			rCell.top += m_szCell.cy*i;
			rCell.BottomRight()=rCell.TopLeft()+m_szCell;
			r = rCell;
			if(!r.IntersectRect(r,rClip))
				{
				DateShow.m_dt += 1;
				continue;// this date not needed to be drawn
				}
			if(DateShow.GetMonth() == m_CurrentMonth.GetMonth())
				dc.SetTextColor(clrNormal);
			else
				dc.SetTextColor(clrLight);
			CString sDay;
			sDay.Format(_T("%d"),DateShow.GetDay());
			// check date to be drawn in selected colors
			if(m_InitialSelectedDate.GetStatus() == 0 && 
				DateShow == m_InitialSelectedDate)
				{
				int oldBkMode = dc.SetBkMode(TRANSPARENT);
				int oldBkColor = dc.SetBkColor(clrHighLightBg);
				int oldTextColor = dc.SetTextColor(clrHighLightText);
				r=rCell;
				r.OffsetRect(1,0);
				dc.FillSolidRect(&r,clrHighLightBg);
				dc.DrawText(sDay,&rCell,DT_RIGHT | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
				dc.SetBkMode(oldBkMode);
				dc.SetBkColor(oldBkColor);
				dc.SetTextColor(oldTextColor);
				}
			else	
				dc.DrawText(sDay,&rCell,DT_RIGHT | DT_VCENTER | DT_SINGLELINE|DT_NOCLIP);
			DateShow.m_dt += 1.0;
			}
	GetFirstShownDate(DateShow);
	COleDateTime DateEnd = DateShow;
	DateEnd.m_dt += 41;
	if(m_DateToday >= DateShow && m_DateToday <= DateEnd)
		{
		// Outline today's date in red
		int offset = (int)(m_DateToday.m_dt - DateShow.m_dt);
		GetDayRect(offset,rCell);
		r=rCell;
		CPen *oldpen = dc.SelectObject(&RedPen);
		int oldBkMode = dc.SetBkMode(TRANSPARENT);
		CBrush *oldbrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);
		r.OffsetRect(1,0);
		dc.Rectangle(&r);
		dc.SetBkMode(oldBkMode);
		dc.SelectObject(oldbrush);
		dc.SelectObject(oldpen);
		}
	dc.SelectObject(oldfont);
	}

void COXCalendarPopup::GetFirstShownDate(COleDateTime& d)
	// --- In  :
	// --- Out :value of first shown date in calendar
	// --- Returns: 
	// --- Effect : 
	{
	d.SetDate(m_CurrentMonth.GetYear(),m_CurrentMonth.GetMonth(),1);
	int dow = d.GetDayOfWeek();
	if(dow == 1)
		dow = 6;
	else dow -=2;
	int pos = (dow + 7 - m_nFirstDayOfWeek)%7;
	if(!pos)
		pos = 7;
	d.m_dt -= pos; // roll date back
	}

void COXCalendarPopup::GetLastShownDate(COleDateTime& d)
	// --- In  :
	// --- Out :value of last shown date in calendar
	// --- Returns: 
	// --- Effect : 
	{
	GetFirstShownDate(d);
	d.m_dt += 41;
	}

int COXCalendarPopup::GridHitTest(CPoint pt)
	// --- In  :coordinate of cursor
	// --- Out :
	// --- Returns::hit test for cursor over showing dates
	// -1 if no dates hit or date not in valid range
	// otherwise offset of first shown date
	// example: HitDate.m_dt = FirstShownDate.m_dt + HitTest
	// --- Effect : 
	{
	if(!m_rectGrid.PtInRect(pt))
		return -1;// hit nothing
	pt -= m_rectGrid.TopLeft();
	int ht =  (pt.x / m_szCell.cx + 7*(pt.y / m_szCell.cy));
	COleDateTime TestDate;
	GetFirstShownDate(TestDate);
	TestDate.m_dt += ht;
	if(TestDate < m_DateStart || TestDate > m_DateEnd)
		return -1; // selected date not in range
	return ht;
	}

int COXCalendarPopup::HitTestNavigation(CPoint pt)
	// --- In  : coordinate of cursor
	// --- Out :
	// --- Returns:hit test for cursor over one of navigation
	// arrows as 0 - nothing hit, 1 - left (prev. month) arrow hit
	//			2 - right (next month) arrow hit
	// --- Effect : 

	{
	if(m_rectLeftNavBar.PtInRect(pt))
		return 1;//left arrow hit

	if(m_rectRightNavBar.PtInRect(pt))
		return 2;//right arrow hit
	return 0; // noting hit
	}

void COXCalendarPopup::OnTimer(UINT nIDEvent) 
	{
	// TODO: Add your message handler code here and/or call default
	ASSERT(nIDEvent == 0);// only one timer set
	/*
	timer is used when user click mouse button over
	navigation arrow and hold it.Each tick repeate
	jump to next or previous month if possible
	*/
	COleDateTime NewMonth = m_CurrentMonth;
	ASSERT(m_nTick);
	if(m_nTick == 1) //left arrow pressed
		DecMonth(NewMonth);
	else
		{
		// only 0,1 and 2 is valid values for hit test
		ASSERT(m_nTick == 2);
		IncMonth(NewMonth);
		}
	if(CanShowMonth(NewMonth))
		{
		m_CurrentMonth = NewMonth;
		CRect r;
		GetClientRect(&r);
		r.top = m_rectGrid.top;
		r.bottom = m_rectGrid.bottom;
		InvalidateRect(&r);
		}	
	CWnd::OnTimer(nIDEvent);
	}

BOOL COXCalendarPopup::CanShowMonth(COleDateTime& month)
	{
	COleDateTime tmpDate=m_CurrentMonth;
	BOOL bCanShow = TRUE;
	m_CurrentMonth = month;
	COleDateTime DateCheck;
	GetFirstShownDate(DateCheck);
	if(DateCheck.GetStatus() != COleDateTime::valid)
		{
		m_CurrentMonth = tmpDate;
		return FALSE;
		}

	if(DateCheck > m_DateEnd)
		{
		bCanShow=FALSE;
		}
	else
	{
	GetLastShownDate(DateCheck);
	if(DateCheck < m_DateStart)
		{
		bCanShow=FALSE;
		}
	}
	m_CurrentMonth = tmpDate;
	return bCanShow;
	}

void COXCalendarPopup::DoNavigationCheck(CPoint pt)
	// --- In  :coordinate of cursor
	// --- Out :
	// --- Returns: 
	// --- Effect : check coordinate for navigation
	// jump to next or previous month if possible
	{
	int nHitTest = HitTestNavigation(pt);
	if(!nHitTest)
		return;//arrows not clicked
	COleDateTime NewMonth = m_CurrentMonth;
	if(nHitTest == 1) //left arrow pressed
		DecMonth(NewMonth);
	else
		{
		// only 0,1 and 2 is valid values for hit test
		ASSERT(nHitTest == 2);
		IncMonth(NewMonth);
		}
	// check new month can be shown
	if(CanShowMonth(NewMonth))
		{
		m_CurrentMonth = NewMonth;
		CRect r;
		GetClientRect(&r);
		r.top = m_rectGrid.top;
		r.bottom = m_rectGrid.bottom;
		InvalidateRect(&r);
		m_nTick = nHitTest;
		// set timer to repeateble 
		SetTimer(0,200,NULL);
		}
	else
		InvalidateRect(NULL);

	}

void COXCalendarPopup::IncMonth(COleDateTime& month)
	// --- In  :
	// --- Out :
	// --- Returns: 
	// --- Effect : increase given date until jump
	// to next month

	{
	int m = month.GetMonth();
	while(m == month.GetMonth())
		month.m_dt += 1;
	}

void COXCalendarPopup::DecMonth(COleDateTime& month)
	// --- In  :
	// --- Out :
	// --- Returns: 
	// --- Effect : decrease given date until jump
	// to previous month

	{
	int m = month.GetMonth();
	while(m == month.GetMonth())
		month.m_dt -= 1;
	}

void COXCalendarPopup::OnMouseMove(UINT nFlags, CPoint point) 
	{
	// TODO: Add your message handler code here and/or call default
	if(m_nTick)
		if(m_nTick != HitTestNavigation(point))
			{
			KillTimer(0);
			m_nTick = 0;
			}
	switch(m_nDragMode)
		{
		case dragNoneButton:
			m_ButtonNone.SetState(ButtonNoneHit(point));
			break;
		case dragTodayButton:
			m_ButtonToday.SetState(ButtonTodayHit(point));
			break;
		case dragGrid:
			{
			int nHitTest = GridHitTest(point);
			if(nHitTest != -1)
				{
				GetFirstShownDate(m_DatePicked);
				m_DatePicked.m_dt += nHitTest;
				InvalidateSelChange(m_InitialSelectedDate,m_DatePicked);
				m_InitialSelectedDate = m_DatePicked;
				}
			}
			break;
		case dragNothing:
			break;
		default:
			TRACE(_T("Undefined dragging mode!\n"));
			ASSERT(FALSE);
			break;
		}
	CWnd::OnMouseMove(nFlags, point);
	}

void COXCalendarPopup::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nTick)
	{
		KillTimer(0);
		m_nTick=0;
	}

	switch(m_nDragMode)
		{
		case dragGrid:
			{
			int nHitTest = GridHitTest(point);
			if(nHitTest != -1)
				{
				GetFirstShownDate(m_DatePicked);
				m_DatePicked += nHitTest;
				PostMessage(WM_ENDPICK,1);// now end showing and return today
				}
			}
				break;
		case dragNoneButton:
			m_ButtonNone.SetState(FALSE);
			if(ButtonNoneHit(point))
				{
				m_DatePicked.m_dt = 0;
				m_DatePicked.SetStatus(COleDateTime::null);
				PostMessage(WM_ENDPICK,1);// now end showing and return today
				}
			break;
		case dragTodayButton:
			m_ButtonToday.SetState(FALSE);
			if(ButtonTodayHit(point))
			{
				// user press <Today> button and Today is between
				// Start and End Date values
				m_DatePicked = m_DateToday;
				PostMessage(WM_ENDPICK,1);// now end showing and return today
			}
			break;
		}
	m_nDragMode = dragNothing;
	CWnd::OnLButtonUp(nFlags, point);
}

int COXCalendarPopup::CalcBarWidth()
	// --- In  :
	// --- Out :
	// --- Returns: width, in pixels, of navigation bar
	//that shows current month name, year and two arrows to jump
	//to next or previous month based on current font
	// --- Effect : 
{
	CClientDC dc(this);
	CFont * oldfont = dc.SelectObject(&m_font);
	int nMonthBarExtentX=0,curLenght,nLongerMonth=0;
	for(int i=0;i < 12;i++)
	{
		curLenght = dc.GetTextExtent(COXCalendar::ms_straMonthNames[i]).cx;
		if(curLenght > nMonthBarExtentX)
		{
			nMonthBarExtentX = curLenght;
			nLongerMonth=i;
		}
	}
	CString strTest;
	// init string with average value
	strTest.Format(_T(" %s 1997 "),(LPCTSTR)COXCalendar::ms_straMonthNames[nLongerMonth]);
	nMonthBarExtentX = dc.GetTextExtent(strTest).cx;

	int nArrowX=::GetSystemMetrics(SM_CXHSCROLL);
	nMonthBarExtentX += nArrowX*2;
	
	dc.SelectObject(oldfont);
	return nMonthBarExtentX;
}

void COXCalendarPopup::SetButtons()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect  :set up sizes and positions of buttons
	{
	CRect rectButton;
	rectButton.TopLeft() = CPoint(m_rectGrid.left,
		m_rectGrid.bottom + m_szCell.cy/2);

	rectButton.BottomRight() = rectButton.TopLeft();
	rectButton.right  += m_szCell.cx*3;
	rectButton.bottom += m_szCell.cy;
	m_ButtonToday.MoveWindow(&rectButton,FALSE);
	rectButton.TopLeft() = CPoint(m_rectGrid.right - m_szCell.cx*3,
		m_rectGrid.bottom + m_szCell.cy/2);
	rectButton.BottomRight() = rectButton.TopLeft();
	rectButton.right  += m_szCell.cx*3;
	rectButton.bottom += m_szCell.cy;
	m_ButtonNone.MoveWindow(&rectButton,FALSE);

	rectButton.top =  m_rectLeftNavBar.top;
	rectButton.bottom = m_rectLeftNavBar.bottom;
	rectButton.right=m_rectRightNavBar.left;
	rectButton.left = m_rectLeftNavBar.right;
	m_ButtonMonth.MoveWindow(&rectButton,FALSE);
	}


BOOL COXCalendarPopup::ButtonTodayHit(CPoint point)
	{
	CRect rButton;
	m_ButtonToday.GetWindowRect(&rButton);
	ScreenToClient(&rButton);
	if(rButton.PtInRect(point) && m_ButtonToday.IsWindowEnabled())
		return TRUE;

	return FALSE;
	}

BOOL COXCalendarPopup::ButtonNoneHit(CPoint point)
	{
	CRect rButton;
	m_ButtonNone.GetWindowRect(&rButton);
	ScreenToClient(&rButton);
	if(rButton.PtInRect(point))
		return TRUE;

	return FALSE;
	}

void COXCalendarPopup::GetDayRect(int offset,CRect& rect)
	{
	ASSERT(offset >= 0 && offset <= 41);
	int i = offset % 7,j = offset / 7;
	rect.SetRect(m_rectGrid.left + m_szCell.cx*i,
						m_rectGrid.top + m_szCell.cy*j,
						m_rectGrid.left + m_szCell.cx*(i+1),
						m_rectGrid.top + m_szCell.cy*(j+1));
	}

void COXCalendarPopup::InvalidateSelChange(COleDateTime& SelOld,COleDateTime& SelNew)
	// --- In  :old and new highlighted date value 
	// --- Out :
	// --- Returns:
	// --- Effect :Calculate and Invalidate areas for update window while
	//dragging mouse
	{
	COleDateTime FirstDate,LastDate;
	GetFirstShownDate(FirstDate);
	GetLastShownDate(LastDate);
	int nHitNew = (int)(SelNew.m_dt - FirstDate.m_dt);
	int nHitOld = (int)(SelOld.m_dt - FirstDate.m_dt);
	if(SelOld == SelNew)
		return;
	CRect rOld,rNew;CString sDay;
	// old selections check
	if(SelOld.GetStatus() == COleDateTime::valid &&
		SelOld >= FirstDate && SelOld <= LastDate)
		{
		GetDayRect(nHitOld,rOld);
		rOld.OffsetRect(1,0);
		rOld.NormalizeRect();
		}
	else
		rOld.SetRectEmpty();

	GetDayRect(nHitNew,rNew);
	rNew.OffsetRect(1,0);
	rNew.NormalizeRect();
	InvalidateRect(&rNew);
	if(!rOld.IsRectEmpty())
		InvalidateRect(&rOld);
	}

void COXCalendarPopup::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_font.DeleteObject();
	CWnd::PostNcDestroy();
}


LRESULT COXCalendarPopup::OnSettingChanged(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	COXCalendar::GetCalendarLocaleData();

	return 0;
}




