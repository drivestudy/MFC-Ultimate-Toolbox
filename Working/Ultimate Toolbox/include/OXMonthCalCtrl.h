// ==========================================================================
// 					Class Specification : COXMonthCalCtrl
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CWnd

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Desciption :         

/*

///////////////////
General Info


A COXMonthCalCtrl object encapsulates the functionality of a month calendar control. 
The month calendar control provides the user with a simple calendar interface, from which 
the user can select a date. The user can change the display by: 

		Scrolling backward and forward, from month to month.

		Clicking the Today text to display the current day (if the MCS_NOTODAY style 
		is not used).

		Picking a month or a year from a pop-up menu. 

You can customize the month calendar control by applying a variety of styles to the object 
when you create it. The following are the styles used with month calendar controls. 

		MCS_DAYSTATE		-	The month calendar will send MCN_GETDAYSTATE notifications 
								to request information about which days should be displayed 
								in bold. We provide also OXMCN_GETDAYSTATE notification. 
								While MCN_GETDAYSTATE can still be used to set bold state 
								of particular days we found it a little bit confusing how 
								to handle that notification. We introduce our 
								OXMCN_GETDAYSTATE notification which we believe will 
								simplify things. see Processing the MCN_GETDAYSTATE and 
								OXMCN_GETDAYSTATE Notification Messages for details. 

		MCS_MULTISELECT		-	The month calendar will allow the user to select a range of 
								dates within the control. By default, the maximum range is 
								one week. You can change the maximum range that can be 
								selected by using the SetMaxSelCount() function.  

		MCS_NOTODAY			-	The month calendar control will not display the "today" 
								date at the bottom of the control.  

		MCS_NOTODAYCIRCLE	-	The month calendar control will not circle the "today" date.  
	
		MCS_WEEKNUMBERS		-	The month calendar control will display week numbers (1-52) 
								to the left of each row of days. 

COXMonthCalCtrl specific styles. They can be set in Create function during control's 
creation or using SetMonthCalStyle() at run time. In order to display the control with 
changed specific style you should invalidate it. To retrieve COXMonthCalCtrl specific 
styles use GetMonthCalStyle().

		OXMCS_BOLDHOLIDAYS	-	Using SetHolidays() function you can define special days 
								that we call "holidays". In order to display them in bold 
								MCS_DAYSTATE and OXMCS_BOLDHOLIDAYS style must be set. 
								Refer to description of SetHolidays() and GetHolidays() 
								functions for more details
 
		OXMCS_INFOTIP		-	We provide built-in support for tooltips (infotips) for 
								any element found in calendar control (title, background, 
								calendar day, etc). It's programmer's responsibility to
								provide textual information for tooltips. If OXMCS_INFOTIP 
								style set then COXMonthCalCtrl will send OXMCN_GETINFOTIP 
								notification to its parent window. Refer to description of 
								OXMCN_GETINFOTIP notification for details how to implement 
								tooltips in your application

The month calendar control can display more than one month, and it can indicate special 
days (such as holidays) by bolding the date.




///////////////////
Using COXMonthCalCtrl


The month calendar control (COXMonthCalCtrl) implements a calendar-like user interface. 
This provides the user with a very intuitive and recognizable method of entering or 
selecting a date. The control also provides the application with the means to obtain 
and set the date information in the control using existing data types. By default, 
the month calendar control displays the current day and month. However, the user is 
able to scroll to the previous and next months and select a specific month and/or year.

!!!
Note   To use the COXMonthCalCtrl class in your project, you must include OXMonthCalCtrl.h
!!!



///////////////////
Creating the Month Calendar Control


How the month calendar control is created depends on whether you are using the control 
in a dialog box or creating it in a nondialog window.


To use COXMonthCalCtrl directly in a dialog box:

Visual C++ v6.0:
In the dialog editor, add a Month Calendar Control to your dialog template resource. 
Specify its control ID. Specify any styles required, using the Properties dialog box of 
the month calendar control. Use ClassWizard to add a member variable of type 
COXMonthCalCtrl with the Control property. You can use this member to call COXMonthCalCtrl 
member functions. Use ClassWizard to map handler functions in the dialog class for any 
month calendar control notification messages you need to handle. In OnInitDialog, set any 
additional styles for the COXMonthCalCtrl object. 

Visual C++ v5.0:
In the dialog editor, add a Static Control to your dialog template resource (it will be
a placeholder for the month calendar control). Specify its control ID. In OnInitDialog, 
you can use next code to replace Static Control on our calendar control:
  
	CWnd *pFrame=GetDlgItem(IDC_MONTHCAL);
	CRect rcWnd;
	pFrame->GetWindowRect(rcWnd);
	pFrame->DestroyWindow(); //pFrame was just a placeholder anyway.
	ScreenToClient(rcWnd);
	m_ctlMonthCal.Create(WS_CHILD|WS_VISIBLE,rcWnd.TopLeft(),this,IDC_MONTHCAL);


To use COXMonthCalCtrl in a nondialog window 

Define the control in the view or window class. Call the control's Create member function, 
possibly in OnInitialUpdate, possibly as early as the parent window's OnCreate handler 
function (if you're subclassing the control). Set the styles for the control. 

  

///////////////////
Processing Notification Messages in Month Calendar Controls


As users interact with the month calendar control (selecting dates and/or viewing a 
different month), the control (COXMonthCalCtrl) sends notification messages to its parent 
window, usually a view or dialog object. Handle these messages if you want to do something 
in response. For example, when the user selects a new month to view, you could provide a 
set of dates that should be emphasized.

!!!
Visual C++ v6.0:
Use WizardBar or ClassWizard to add notification handlers to the parent class for those 
messages you want to implement.
!!!

The following list describes the various notifications sent by the month calendar control. 

	MCN_GETDAYSTATE			-	Requests information about which days should be displayed 
								in bold. 
	OXMCN_GETDAYSTATE		-	Requests information about which days should be displayed 
								in bold. 
	MCN_SELCHANGE			-	Notifies the parent that the selected date or range of the 
								date has changed.
	MCN_SELECT				-	Notifies the parent that an explicit date selection has 
								been made. 
	OXMCN_GETINFOTIP		-	Notifies the parent that the tooltip for calendar control
								element is requested. 



///////////////////
Processing the MCN_GETDAYSTATE Notification Message

MCN_GETDAYSTATE
    lpNMDayState = (LPNMDAYSTATE) lParam;

Month calendar controls send the MCN_GETDAYSTATE notification message to request 
information about how the days within the visible months should be displayed.
 
		lpNMDayState	-	Address of an NMDAYSTATE structure. The structure contains 
							information about the time frame for which the control needs 
							information, and it receives the address of an array that 
							provides this data. 

		NMDAYSTATE

		typedef struct tagNMDAYSTATE {
			NMHDR           nmhdr;
			SYSTEMTIME      stStart;
			int             cDayState;
			LPMONTHDAYSTATE prgDayState; 
		} NMDAYSTATE, FAR * LPNMDAYSTATE;

		Carries information required to process the MCN_GETDAYSTATE notification message. 
		All members of this structure are for input, except prgDayState, which the 
		receiving application must set when processing MCN_GETDAYSTATE. 
		
			nmhdr		-	NMHDR structure that contains information about this 
							notification message. 
			stStart		-	SYSTEMTIME structure that contains the starting date. 
			cDayState	-	INT value specifying the total number of elements that must be 
							in the array at prgDayState. 
			prgDayState -	Address of an array of MONTHDAYSTATE values. This must be set 
							to point to an array that contains as many elements as 
							specified in cDayState. The first element in the array 
							corresponds to the date in stStart. 

  
The following application-defined function, OnGetDayState, processes MCN_GETDAYSTATE by 
filling an array of MONTHDAYSTATE values to highlight the 15th day of each month. 

		void CMyDialog::OnGetDayState(NMHDR *pHdr, LRESULT *pRes)
		{
			#define BOLDDAY(ds,iDay) if(iDay>0 && iDay<32)\
						                (ds)|=(0x00000001<<(iDay-1))

			NMDAYSTATE* pDayState=(NMDAYSTATE*)pHdr;
			// draw first five days of every month in bold
			for(int i=0;i<pDayState->cDayState;i++)
	            BOLDDAY(pDayState->prgDayState[i],15);
		}


Preparing the MONTHDAYSTATE Array

The MCN_GETDAYSTATE notification message require an 
array of MONTHDAYSTATE values to determine how dates will be displayed. Each month that 
the control displays must have a corresponding element within the array. 

To support these messages, your application must properly prepare the array. 
The following is a simple macro that sets a bit in a MONTHDAYSTATE value for a given 
day within that month. 

#define BOLDDAY(ds,iDay) if(iDay>0 && iDay<32)\
                            (ds)|=(0x00000001<<(iDay-1))
Using this macro, an application could simply loop through an array of important dates, 
setting bits within the corresponding array elements. This approach is not the most 
efficient, of course, but works well for many purposes. As long as your application sets 
MONTHDAYSTATE bits appropriately, it does not matter how those bits were set. 



///////////////////
Processing the OXMCN_GETDAYSTATE Notification Message

Although MCN_GETDAYSTATE notification handling provide the most effective way (in terms of 
memory) of setting some days to be drawn in bold, we found it a little bit confusing and 
not friendly. We provide our counterpart of this notification message: OXMCN_GETDAYSTATE.

	OXMCN_GETDAYSTATE
		lpOXNMDayState = (LPOXNMDAYSTATE) lParam;

Sent by a month calendar control to request information about how individual days 
should be displayed. This notification message is sent only by month calendar 
controls that use the MCS_DAYSTATE style, and it is sent in the form of a WM_NOTIFY 
message. 

		lpOXNMDayState		-	Address of an OXNMDAYSTATE structure. The structure 
								contains information about the month and year for which 
								the control needs information, and it contains the 
								array which must be populated with all days that should 
								be displayed in bold (refer to description of 
								OXNMDAYSTATE structure for details). Note, that the 
								array can be already populated with some days previously
								set using SetHolidays() function

The following application-defined function, OnOXGetDayState, processes OXMCN_GETDAYSTATE by 
filling an array of MONTHDAYS values to highlight the first 5 days of each month. 

		void CMyDialog::OnOXGetDayState(NMHDR *pHdr, LRESULT *pRes)
		{
			LPOXNMDAYSTATE pDayState=(LPOXNMDAYSTATE)pHdr;
			// draw first five days of every month in bold
			for(int i=1;i<5;i++)
				pDayState->parrDayState.AddTail(i);
		}



///////////////////
Processing the MCN_SELCHANGE Notification Message

	MCN_SELCHANGE
		lpNMSelChange = (LPNMSELCHANGE) lParam;

Sent by a month calendar control when the currently selected date or range of dates 
changes. This notification message is sent in the form of a WM_NOTIFY message. 

For example, the control sends MCN_SELCHANGE when the user explicitly changes his or her 
selection within the current month or when the selection is implicitly changed in response 
to next/previous month navigation. 

		lpNMSelChange		-	Address of an NMSELCHANGE structure that contains 
								information about the currently selected date range. 

		NMSELCHANGE

		typedef struct tagNMSELCHANGE{
			NMHDR           nmhdr;
			SYSTEMTIME      stSelStart;
			SYSTEMTIME      stSelEnd;
		} NMSELCHANGE, FAR * LPNMSELCHANGE;
		
		Carries information required to process the MCN_SELCHANGE notification message. 

			nmhdr		-	NMHDR structure that contains information about this 
							notification message. 
			stSelStart	-	SYSTEMTIME structure that contains the date for the first 
							day in the user's selection range. 
			stSelEnd	-	SYSTEMTIME structure that contains the date for the last 
							day in the user's selection range. 

  
This notification message is similar to MCN_SELECT, but it is sent in response to any 
selection change. MCN_SELECT is sent only for an explicit date selection. 

  

  
///////////////////
Processing the MCN_SELECT Notification Message

	MCN_SELECT
		lpNMSelChange = (LPNMSELCHANGE) lParam;

	Sent by a month calendar control when the user makes an explicit date selection 
	within a month calendar control. This notification is sent in the form of a 
	WM_NOTIFY message. 

		lpNMSelChange		-	Address of an NMSELCHANGE structure that contains 
								information about the currently selected date range. 

	This notification message is similar to MCN_SELCHANGE, but it is sent only in 
	response to a user's explicit date selections. MCN_SELCHANGE applies to any 
	selection change. 

  

  
///////////////////
Processing the OXMCN_GETINFOTIP Notification Message

	OXMCN_GETINFOTIP
		lpOXNMInfoTip = (LPOXNMMCINFOTIP) lParam;

Sent by a month calendar control to request information about tooltip text to be 
displayed while mouse is over of specified point. This notification message is sent 
only by month calendar controls that use the OXMCS_INFOTIP style, and it is sent in 
the form of a WM_NOTIFY message. 

		lpOXNMInfoTip		-	Address of an OXNMMCINFOTIP structure. The structure 
								contains variables that are to be filled with tooltip 
								text and it contains structure which describes calendar
								control's element for which tooltip should be provided 
								(refer to description of OXNMMCINFOTIP structure for 
								details). 


Example of handling of OXMCN_GETINFOTIP notification 

		void CMyDialog::OnGetInfoTip(NMHDR *pHdr, LRESULT *pRes)
		{
			LPOXNMMCINFOTIP pInfoTip=(LPOXNMMCINFOTIP)pHdr;

			CString sText=_T("");

			if((pInfoTip->pMCHitTest->uHit==MCHT_CALENDARDATENEXT || 
				pInfoTip->pMCHitTest->uHit==MCHT_CALENDARDATEPREV || 
				pInfoTip->pMCHitTest->uHit==MCHT_CALENDARDATE) && 
				pInfoTip->pMCHitTest->st.GetMonth()==6 && 
				pInfoTip->pMCHitTest->st.GetDay()==1 )
			{
				sText=_T("Happy Birthday!");
			}

			_tcsncpy(pInfoTip->szText,sText.GetBuffer(sText.GetLength()),
				countof(pInfoTip->szText));
			sText.ReleaseBuffer();

			*pRes=0;
		}



*/

/////////////////////////////////////////////////////////////////////////////

#if !defined(_OXMONTHCALCTRL_H_)
#define _OXMONTHCALCTRL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"

#ifndef _WIN32_IE
#if _MFC_VER>=0x0420
#define _WIN32_IE 0x0300
#else
#define _WIN32_IE 0x0200
#endif
#endif

#ifndef __AFXWIN_H__
#include <afxwin.h>
#endif

#ifndef __AFXDISP_H__
#include <afxdisp.h>
#endif

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

#if _MFC_VER>0x0421
#include <afxdtctl.h>
#endif	//	_MFC_VER>0x0421

#include <math.h>

#ifndef MCS_NOTODAYCIRCLE
#undef MCS_NOTODAY
#if (_WIN32_IE >= 0x0400)
#define MCS_NOTODAYCIRCLE   0x0008
#define MCS_NOTODAY         0x0010
#else
#define MCS_NOTODAY         0x0008
#endif	//	(_WIN32_IE >= 0x0400)
#endif	//	MCS_NOTODAYCIRCLE


///////////////////////////////////////////////////////////////////////////////////////
// OXMonthCalCtrl.h : header file
//

///////////////////////////////////////////////////////////////////////////////////////
// all number of days in the longest month
enum _tagMONTHDAYS
{
	FIRST=1, SECOND=2, THIRD=3, FOURTH=4, FIFTH=5, SIXTH=6, SEVENTH=7,
	EIGHTH=8, NINTH=9, TENTH=10, ELEVENTH=11, TWELFTH=12, THIRTEENTH=13,
	FOURTEENTH=14, FIFTEENTH=15, SIXTEENTH=16, SEVENTEENTH=17, EIGHTTEENTH=18,
	NINTEENTH=19, TWENTIETH=20, TWENTY_FIRST=21, TWENTY_SECOND=22, TWENTY_THIRD=23,
	TWENTY_FOURTH=24, TWENTY_FIFTH=25, TWENTY_SIXTH=26, TWENTY_SEVENTH=27,
	TWENTY_EIGHTH=28, TWENTY_NINTH=29, THIRTIETH=30, THIRTY_FIRST=31
};
typedef enum _tagMONTHDAYS MONTHDAYS, FAR * LPMONTHDAYS;
// array of month days 
typedef CList<MONTHDAYS,MONTHDAYS> ARRMONTHDAYS, FAR * LPARRMONTHDAYS;

// all days of week
enum _tagWEEKDAYS
{
	MONDAY=0, TUESDAY=1, WENDSDAY=2, THURSDAY=3, FRIDAY=4, SATURDAY=5, SUNDAY=6
};
typedef enum _tagWEEKDAYS WEEKDAYS, FAR * LPWEEKDAYS;
// array of week days 
typedef CList<WEEKDAYS,WEEKDAYS> ARRWEEKDAYS, FAR * LPARRWEEKDAYS;


// structure for holidays for particular month and year
//
typedef struct _tagHOLIDAYS
{
	int nMonth;
	int nYear;
	ARRWEEKDAYS arrWeekDays;
	// to specify holidays you can use the number of the monthday (FIRST, SECOND, etc.)
	ARRMONTHDAYS arrMonthDays;

	// destructor
	_tagHOLIDAYS::~_tagHOLIDAYS()
	{
		Reset();
	}

	void _tagHOLIDAYS::Reset()
	{
		arrWeekDays.RemoveAll();
		arrMonthDays.RemoveAll();
	}

    // assignment operator
    _tagHOLIDAYS& operator=( const _tagHOLIDAYS& holidays )
	{
		if(this==&holidays)
			return *this;
		
		Reset();

		nMonth=holidays.nMonth;
		nYear=holidays.nYear;

		POSITION posList=holidays.arrWeekDays.GetHeadPosition();
		while(posList!=NULL)
		{
			WEEKDAYS weekDay=holidays.arrWeekDays.GetNext(posList);
			arrWeekDays.AddTail(weekDay);
		}

		posList=holidays.arrMonthDays.GetHeadPosition();
		while(posList!=NULL)
		{
			MONTHDAYS monthDay=holidays.arrMonthDays.GetNext(posList);
			arrMonthDays.AddTail(monthDay);
		}

		return *this;
	}

} HOLIDAYS, FAR * LPHOLIDAYS;
//
// Carries information about "holidays" (days to be displayed in bold).
//
//		nMonth			-	a month for which holidays are applied, if it equals to -1 
//							then the specified holidays are aplied to any month in the 
//							specified nYear year
//		nYear			-	a year for which holidays are applied, if it equals to -1 then 
//							the specified holidays are aplied to any year in the specified 
//							nMonth month
//		arrWeekDays		-	holidays can be specified using the name of the weekday 
//							(MONDAY, TUESDAY, etc.)
//		arrMonthDays	-	holidays can be specified using the number of the day in the
//							month (FIRST, SECOND, etc.)
//

/////////////////////////////////////////////////////////////////////////////////
// array of holidays for different months
typedef CMap<DWORD, DWORD, LPHOLIDAYS, LPHOLIDAYS> ARRHOLIDAYS, FAR * LPARRHOLIDAYS;


/////////////////////
//	STYLES
/////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
// COXMonthCalCtrl specific styles. They can be set in Create function
// during control's creation or using SetMonthCalStyle() at run time.
// In order to display the control with changed specific style you should invalidate it.
// To retrieve COXMonthCalCtrl specific styles use GetMonthCalStyle().
//
//
// Using SetHolidays() function you can define special days that we call "holidays". In
// order to display them in bold next OXMCS_BOLDHOLIDAYS style must be set. Refer
// to description of SetHolidays() and GetHolidays() functions for more details
#define	OXMCS_BOLDHOLIDAYS	0x00000001
// 
// We provide built-in support for tooltips (infotips) for any element found in calendar 
// controls (title, background, calendar day, etc). It's programmer's responsibility to
// provide textual information for tooltips. If OXMCS_INFOTIP style set then 
// COXMonthCalCtrl will send OXMCN_GETINFOTIP notification to its parent window. Refer
// to description of OXMCN_GETINFOTIP notification for details how to implement 
// tooltips in your application
#define OXMCS_INFOTIP		0x00000002



/////////////////////
//	NOTIFICATIONS
/////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
// Our counterpart of MCN_GETDAYSTATE notification. While MCN_GETDAYSTATE can still be 
// used to set bold state of particular days we found it a little bit confusing how to 
// handle that notification. We introduce our OXMCN_GETDAYSTATE notification which we
// believe will simplify things:
//
// OXMCN_GETDAYSTATE
//		lpOXNMDayState = (LPOXNMDAYSTATE) lParam;
//
// Sent by a month calendar control to request information about how individual days 
// should be displayed. This notification message is sent only by month calendar 
// controls that use the MCS_DAYSTATE style, and it is sent in the form of a WM_NOTIFY 
// message. 
//
//		lpOXNMDayState		-	Address of an OXNMDAYSTATE structure. The structure 
//								contains information about the month and year for which 
//								the control needs information, and it contains the 
//								array which must be populated with all days that should 
//								be displayed in bold (refer to description of 
//								OXNMDAYSTATE structure for details). Note, that the 
//								array can be already populated with some days previously
//								set using SetHolidays() function
//
//
///////////////////////////////////////////////////////////////////////////////////////
// OXMCN_GETINFOTIP
//		lpOXNMInfoTip = (LPOXNMMCINFOTIP) lParam;
//
// Sent by a month calendar control to request information about tooltip text to be 
// displayed while mouse is over of specified point. This notification message is sent 
// only by month calendar controls that use the OXMCS_INFOTIP style, and it is sent in 
// the form of a WM_NOTIFY message. 
//
//		lpOXNMInfoTip		-	Address of an OXNMMCINFOTIP structure. The structure 
//								contains variables that are to be filled with tooltip 
//								text and it contains structure which describes calendar
//								control's element for which tooltip should be provided 
//								(refer to description of OXNMMCINFOTIP structure for 
//								details). 
//



/////////////////////
//	STRUCTURES FOR 
//	NOTIFICATIONS
/////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
//
typedef struct _tagOXNMDAYSTATE 
{
    NMHDR nmhdr;
	int nMonth;
	int nYear;
    LPARRMONTHDAYS parrDayState; 

	// constructor
	_tagOXNMDAYSTATE::_tagOXNMDAYSTATE()
	{
		parrDayState=NULL;
	}

	// destructor
	_tagOXNMDAYSTATE::~_tagOXNMDAYSTATE()
	{
		delete parrDayState;
	}

    // assignment operator
    _tagOXNMDAYSTATE& operator=( const _tagOXNMDAYSTATE& nmds )
	{
		if(this==&nmds)
			return *this;
		
		delete parrDayState;
		parrDayState=new ARRMONTHDAYS;

		nMonth=nmds.nMonth;
		nYear=nmds.nYear;

		POSITION posList=nmds.parrDayState->GetHeadPosition();
		while(posList!=NULL)
		{
			MONTHDAYS monthDay=nmds.parrDayState->GetNext(posList);
			parrDayState->AddTail(monthDay);
		}

		return *this;
	}

} OXNMDAYSTATE, FAR * LPOXNMDAYSTATE;
//
// Carries information required to process the OXMCN_GETDAYSTATE notification message. 
// All members of this structure are for input, except parrDayState, which the receiving 
// application must populate when processing OXMCN_GETDAYSTATE. Note, that the array can 
// be already populated with some days previously set using SetHolidays() function
//
//		nmhdr			-	NMHDR structure that contains information about this 
//							notification message. 
//		nMonth			-	the month for which array of days should be populated
//		nYear			-	the year for which array of days should be populated
//		parrDayState	-	address of an array of ARRMONTHDAYS type. This must be 
//							populated with days in the specified month and year that 
//							should be displayed in bold (days are in the range 1..31 
//							depending on month). You can also remove some days that
//							might be already included in the array (as result of using
//							SetHolidays() function)
//
//
//
// Example of handling of OXMCN_GETDAYSTATE notification 
//
//		void CMyDialog::OnOXGetDayState(NMHDR *pHdr, LRESULT *pRes)
//		{
//			LPOXNMDAYSTATE pDayState=(LPOXNMDAYSTATE)pHdr;
//			// draw first five days of every month in bold
//			for(int i=1;i<5;i++)
//				pDayState->parrDayState.AddTail(i);
//		}
//
//
///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
// Structure that is used with OXMCN_GETINFOTIP notification and sent to calendar 
// control parent window before displaying tooltip for any calendar control element
//
typedef struct _tagOXNMMCINFOTIP
{
    NMHDR nmhdr;						// standard header
	PMCHITTESTINFO		pMCHitTest;		// see below
    LPTSTR				lpszText;		// see below
    TCHAR				szText[80];		// buffer for infotip text
    HINSTANCE			hinst;			// see below

	_tagOXNMMCINFOTIP()
	{
		ZeroMemory(this,sizeof(_tagOXNMMCINFOTIP));
	}

} OXNMMCINFOTIP, *LPOXNMMCINFOTIP;
//
//	nmhdr			-	NMHDR structure that contains information about this 
//						notification message. 
//	pMCHitTest		-	pointer to MCHITTESTINFO which describes the calendar control
//						element which requested the tooltip
//	lpszText		-	Address of a string to receive the text for a infotip.
//	szText			-	Buffer that receives the infotip text. An application can copy 
//						the text to this buffer as an alternative to specifying a 
//						string address.
//	hinst			-	Handle of the instance that contains a string resource to be 
//						used as the infotip text. If lpszText is the address of the 
//						infotip text, this member is NULL.
//
// When you handle the OXMCN_GETINFOTIP notification, specify the string to be displayed 
// in one of the following ways:
//
//			-	Copy the text to the buffer specified by the szText member.
//			-	Copy the address of the buffer that contains the text to the lpszText 
//				member.
//			-	Copy the identifier of a string resource to the lpszText member, 
//				and copy the handle of the instance that contains the resource to the 
//				hinst member. 
//
// To get an information about the calendar control element for which tooltip is requested.
// Refer to HitTest function on detailed description of MCHITTESTINFO structure.
//
//
//
// Example of handling of OXMCN_GETINFOTIP notification 
//
//		void CMyDialog::OnGetInfoTip(NMHDR *pHdr, LRESULT *pRes)
//		{
//			LPOXNMMCINFOTIP pInfoTip=(LPOXNMMCINFOTIP)pHdr;
//
//			CString sText=_T("");
//
//			if((pInfoTip->pMCHitTest->uHit==MCHT_CALENDARDATENEXT || 
//				pInfoTip->pMCHitTest->uHit==MCHT_CALENDARDATEPREV || 
//				pInfoTip->pMCHitTest->uHit==MCHT_CALENDARDATE) && 
//				pInfoTip->pMCHitTest->st.GetMonth()==6 && 
//				pInfoTip->pMCHitTest->st.GetDay()==1 )
//			{
//				sText=_T("Happy Birthday!");
//			}
//
//			_tcsncpy(pInfoTip->szText,sText.GetBuffer(sText.GetLength()),
//				countof(pInfoTip->szText));
//			sText.ReleaseBuffer();
//
//			*pRes=0;
//		}
//
//
///////////////////////////////////////////////////////////////////////////////////////




// One-based array of days in year at month start
static int arrMonthStartDay[13] = {
	0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

#ifndef MIN_DATE
#define MIN_DATE                (-657434L)  // about year 100
#endif	// MIN_DATE
#ifndef MAX_DATE
#define MAX_DATE                2958465L    // about year 9999
#endif	// MIN_DATE
#ifndef HALF_SECOND
#define HALF_SECOND				(1.0/172800.0)
#endif	// HALF_SECOND

#define ID_MCHORZGAP			6

#define ID_MAXMONTHCOUNT		12

// macro that can be used to set bold state for iDay 
#define BOLDDAY(ds,iDay) if(iDay>0 && iDay<32)\
                           (ds)|=(0x00000001<<(iDay-1))

// determine number of elements in an array (not bytes)
#define countof(array) (sizeof(array)/sizeof(array[0]))

///////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// COXMonthCalCtrl window

#if _MFC_VER<=0x0421
class OX_CLASS_DECL COXMonthCalCtrl : public CWnd
#else
class OX_CLASS_DECL COXMonthCalCtrl : public CMonthCalCtrl
#endif	//	_MFC_VER>0x0421
{
#if _MFC_VER<=0x0421
	DECLARE_DYNAMIC(COXMonthCalCtrl)
#endif	//	_MFC_VER>0x0421

protected:
	static BOOL bComCtlInitialized;

	ARRHOLIDAYS m_arrHolidays;
	DWORD m_dwMonthCalStyle;

public:

	COXMonthCalCtrl();
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : constructor. You must call Create after you construct the object.

// Creation
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, 
		DWORD dwMonthCalStyle=OXMCS_BOLDHOLIDAYS|OXMCS_INFOTIP);
	BOOL Create(DWORD dwStyle, const POINT& pt, CWnd* pParentWnd, UINT nID, 
		DWORD dwMonthCalStyle=OXMCS_BOLDHOLIDAYS|OXMCS_INFOTIP, int nXDimension=1, 
		int nYDimension=1);
	// --- In  :	dwStyle			-	Specifies the combination of Windows styles applied 
	//									to the month calendar control. The following are 
	//									the styles used with month calendar controls: 
	//
	//					MCS_DAYSTATE		-	The month calendar will send 
	//											MCN_GETDAYSTATE and OXMCN_GETDAYSTATE 
	//											notifications to request information about 
	//											which days should be displayed in bold. 
	//					MCS_MULTISELECT		-	The month calendar will allow the user to 
	//											select a range of dates within the control.
	//											By default, the maximum range is one week. 
	//											You can change the maximum range that can 
	//											be changed using SetMaxSelCount function. 
	//					MCS_NOTODAY			-	The month calendar control will not 
	//											display the "today" date at the bottom of 
	//											the control.  
	//					MCS_NOTODAYCIRCLE	-	The month calendar control will not circle 
	//											the "today" date.  
	//					MCS_WEEKNUMBERS		-	The month calendar control will display 
	//											week numbers (1-52) to the left of each 
	//											row of days.  
	//
	//				rect			-	A reference to a RECT structure. Contains the position 
	//									and size of the month calendar control.
	//				pt				-	A reference to a POINT structure that identifies the 
	//									location of the month calendar control. 
	//				pParentWnd		-	A pointer to a CWnd object that is the parent window 
	//									of the month calendar control. It must not be NULL.
	//				nID				-	Specifies the month calendar control’s control ID.
	//				dwMonthCalStyle	-	COXMonthCalendar control specific styles. The 
	//									following are the specific styles used with month 
	//									calendar controls: 
	//
	//					OXMCS_BOLDHOLIDAYS	-	The month calendar will draw holidays 
	//											that can be specified using SetHolidays
	//											function in bold. 
	//
	//				nXDimension		-	the number of months that should be displayed 
	//									horizontally in the control, mustn't be less than 1
	//				nYDimension		-	the number of months that should be displayed 
	//									vertically in the control, mustn't be less than 1
	//
	// --- Out : 
	// --- Returns:	Nonzero if initialization was successful; otherwise 0.
	// --- Effect : Create a month calendar control in two steps: 
	//				
	//				Call CMonthCalCtrl to construct a CMonthCalCtrl object.
	//
	//				Call this member function, which creates a month calendar control 
	//				and attaches it to the COXMonthCalCtrl object. When you call Create, 
	//				the common controls are initialized. The version of Create you call 
	//				determines how it is sized: 
	//
	//				To have MFC automatically size the control to one month, call the 
	//				override that uses the pt parameter. 
	//
	//				To size the control yourself, call the override of this function that 
	//				uses the rect parameter. 

#if _MFC_VER<=0x0421

////////////////////////////////////////////////////////////////////////////////////////
//Attributes
	
	inline BOOL GetMinReqRect(RECT* pRect) const { 
		ASSERT(m_hWnd!=NULL); 
		return (BOOL)::SendMessage(m_hWnd,MCM_GETMINREQRECT,0,(LPARAM)pRect); 
	}

	// --- In  :	pRect		-	A pointer to a RECT structure that will receive 
	//								bounding rectangle information. This parameter 
	//								must be a valid address and cannot be NULL. 
	// --- Out : 
	// --- Returns:	If successful, this member function returns nonzero and lpRect 
	//				receives the applicable bounding information. If unsuccessful, 
	//				the member function returns 0.
	// --- Effect : Retrieves the minimum size required to display a full month in a 
	//				month calendar control. The top and left members of pRect will 
	//				always be zero. The right and bottom members represent the minimum 
	//				cx and cy required for the control. The minimum required window size 
	//				for a month calendar control depends on the currently selected font. 

	int SetMonthDelta(int iDelta) { 
		ASSERT(m_hWnd!=NULL); 
		return (int)::SendMessage(m_hWnd,MCM_SETMONTHDELTA,(WPARAM)iDelta,0); 
	}
	// --- In  :	iDelta		-	The number of months to be set as the control's 
	//								scroll rate. If this value is zero, the month delta 
	//								is reset to the default, which is the number of 
	//								months displayed in the control. 
	// --- Out : 
	// --- Returns:	The previous scroll rate. If the scroll rate has not been previously 
	//				set, the return value is 0. 
	// --- Effect : Sets the scroll rate for a month calendar control. The scroll rate 
	//				is the number of months that the control moves its display when the 
	//				user clicks a scroll button. 

	inline int GetMonthDelta() const { 
		ASSERT(m_hWnd != NULL); 
		return (int)::SendMessage(m_hWnd,MCM_GETMONTHDELTA,0,0); 
	}
	// --- In  :
	// --- Out : 
	// --- Returns:	The scroll rate for the month calendar control. The scroll rate is 
	//				the number of months that the control moves its display when the user 
	//				clicks a scroll button once. 
	// --- Effect : Retrieves the scroll rate for a month calendar control.

	BOOL SetFirstDayOfWeek(int iDay, int* lpnOld = NULL);
	// --- In  :	iDay		-	An integer value representing which day is to be set 
	//								as the first day of the week. This value must be one 
	//								of the day numbers. See GetFirstDayOfWeek for a 
	//								description of the day numbers.
	// --- Out :	lpnOld		-	A pointer to an integer indicating the first day of 
	//								the week previously set.
	// --- Returns:	Nonzero if the previous first day of the week is set to a value 
	//				other than that of LOCALE_IFIRSTDAYOFWEEK, which is the day indicated 
	//				in the control panel setting. Otherwise, this function returns 0.
	// --- Effect : Sets the first day of the week for a month calendar control.

	int GetFirstDayOfWeek(BOOL* pbLocal = NULL) const;
	// --- In  :	
	// --- Out :	pbLocal		-	A pointer to a BOOL value. If the value is non-zero, 
	//								the control's setting does not match the setting in 
	//								the control panel. 
	// --- Returns:	An integer value that represents the first day of the week. 
	//				The days of the week are represented as integers, as follows.
	//				
	//				Value	Day of the Week 
	//
	//				0		Monday  
	//				1		Tuesday  
	//				2		Wednesday  
	//				3		Thursday  
	//				4		Friday  
	//				5		Saturday  
	//				6		Sunday  
	//
	// --- Effect : Retrieves the first day of the week for a month calendar control. 

	inline COLORREF GetColor(int nRegion) const { 
		ASSERT(m_hWnd!=NULL); 
		return (COLORREF)::SendMessage(m_hWnd,MCM_GETCOLOR,(WPARAM)nRegion,0); 
	}
	// --- In  :	nRegion		-	The region of the month calendar control from which 
	//								the color is retrieved. For a list of values, see 
	//								the nRegion parameter of SetColor.
	// --- Out : 
	// --- Returns:	A COLORREF value specifying the color associated with the portion 
	//				of the month calendar control, if successful. Otherwise, this member 
	//				function returns -1.
	// --- Effect : Call this member function to retrieve the color of an area of the 
	//				month calendar control specified by nRegion.

	inline COLORREF SetColor(int nRegion, COLORREF ref) { 
		ASSERT(m_hWnd!=NULL); 
		return (COLORREF)::SendMessage(m_hWnd,MCM_SETCOLOR,(WPARAM)nRegion,(LPARAM)ref); 
	}
	// --- In  :	nRegion		-	An integer value specifying which month calendar 
	//								color to set. This value can be one of the following.
	//
	//					MCSC_BACKGROUND		-	The background color displayed between 
	//											months.  
	//					MCSC_MONTHBK		-	The background color displayed within 
	//											the month.  
	//					MCSC_TEXT			-	The color used to display text within a 
	//											month.  
	//					MCSC_TITLEBK		-	The background color displayed in the 
	//											calendar's title.  
	//					MCSC_TITLETEXT		-	The color used to display text within the 
	//											calendar's title.  
	//					MCSC_TRAILINGTEXT	-	The color used to display header and 
	//											trailing-day text. Header and trailing 
	//											days are the days from the previous and 
	//											following months that appear on the 
	//											current calendar.  
	//
	//				ref			-	A COLORREF value for the new color setting for the 
	//								specified portion of the month calendar control. 
	// --- Out : 
	// --- Returns:	A COLORREF value that represents the previous color setting for 
	//				the specified portion of the month calendar control, if successful. 
	//				Otherwise this function returns -1. 
	// --- Effect : Sets the color for a given portion of a month calendar control

	inline DWORD HitTest(PMCHITTESTINFO pMCHitTest) { 
		ASSERT(::IsWindow(m_hWnd)); 
		return (DWORD)::SendMessage(m_hWnd,MCM_HITTEST,0,(LPARAM)pMCHitTest); 
	}
	// --- In  :	pMCHitTest	-	A pointer to a MCHITTESTINFO structure containing hit 
	//								testing points for the month calendar control.
	//				
	//				typedef struct {
    //					UINT       cbSize;
    //					POINT      pt;
    //					UINT       uHit;   // An output member
    //					SYSTEMTIME st;     // An output member
	//				} MCHITTESTINFO, *PMCHITTESTINFO;
	//
	//				Carries information specific to hit-testing points for a month 
	//				calendar control. 
	//
	//						cbSize		-	Size of this structure, in bytes. 
	//						pt			-	POINT structure that contains information 
	//										about the point to be hit-tested. 
	//						uHit		-	Output member that receives a bit flag 
	//										representing the result of the hit-test 
	//										operation. This value will be one of the 
	//										following: 
	//
	//							MCHT_CALENDARBK			-	The given point was in the 
	//														calendar's background.  
	//							MCHT_CALENDARDATE		-	The given point was on a 
	//														particular date within the 
	//														calendar. The SYSTEMTIME 
	//														structure at pMCHitTest->st 
	//														is set to the date at the 
	//														given point.  
	//							MCHT_CALENDARDATENEXT	-	The given point was over a 
	//														date from the next month 
	//														(partially displayed at the 
	//														end of the currently displayed 
	//														month). If the user clicks 
	//														here, the month calendar will 
	//														scroll its display to the next 
	//														month or set of months.  
	//							MCHT_CALENDARDATEPREV	-	The given point was over a 
	//														date from the previous month 
	//														(partially displayed at the 
	//														end of the currently displayed 
	//														month). If the user clicks 
	//														here, the month calendar will 
	//														scroll its display to the 
	//														previous month or set of months.
	//							MCHT_CALENDARDAY		-	The given point was over a day 
	//														abbreviation ("Fri", for 
	//														example). The SYSTEMTIME 
	//														structure at lpMCHitTest->st is
	//														set to the corresponding date 
	//														in the top row.  
	//							MCHT_CALENDARWEEKNUM	-	The given point was over a week
	//														number (MCS_WEEKNUMBERS style 
	//														only). The SYSTEMTIME structure
	//														at lpMCHitTest->st is set to 
	//														the corresponding date in the 
	//														leftmost column.  
	//							MCHT_NOWHERE			-	The given point was not on the 
	//														month calendar control, or it 
	//														was in an inactive portion of 
	//														the control.  
	//							MCHT_TITLEBK			-	The given point was over the 
	//														background of a month's title.  
	//							MCHT_TITLEBTNNEXT		-	The given point was over the 
	//														button at the top right corner 
	//														of the control. If the user 
	//														clicks here, the month calendar 
	//														will scroll its display to the 
	//														next month or set of months.  
	//							MCHT_TITLEBTNPREV		-	The given point was over the 
	//														button at the top left corner 
	//														of the control. If the user 
	//														clicks here, the month calendar
	//														will scroll its display to the 
	//														previous month or set of months.  
	//							MCHT_TITLEMONTH			-	The given point was in a 
	//														month's title bar, over a 
	//														month name.  
	//							MCHT_TITLEYEAR			-	The given point was in a 
	//														month's title bar, over the 
	//														year value.  
	//							MCHT_TODAYLINK			-	The given point was over 
	//														Today link button.  
	//
	//						st		-	SYSTEMTIME structure that receives date and time 
	//									information specific to the location that was 
	//									hit-tested. 
	// --- Out : 
	// --- Returns:	A DWORD value. Equal to the uHit member of the MCHITTESTINFO structure.
	// --- Effect : Call this member function to determine which month calendar control, 
	//				if any, is at a specified position. HitTest uses the MCHITTESTINFO 
	//				structure, which contains information about the hit test.



////////////////////////////////////////////////////////////////////////////////////////
// Operations
	
	BOOL SizeMinReq(BOOL bRepaint = TRUE);
	// --- In  :	bRepaint		-	Specifies whether the control is to be repainted. 
	//									By default, TRUE. If FALSE, no repainting occurs.
	// --- Out : 
	// --- Returns:	Nonzero if the month calendar control is sized to its minimum; 
	//				otherwise 0
	// --- Effect : Call this member function to display the month calendar control to 
	//				the minimum size that displays one month. Calling SizeMinReq 
	//				successfully displays the entire month calendar control for one 
	//				month's calendar.

	void SetToday(const COleDateTime& refDateTime);
	void SetToday(const CTime* pDateTime);
	inline void SetToday(const LPSYSTEMTIME pDateTime) { 
		ASSERT(m_hWnd!=NULL);
		::SendMessage(m_hWnd,MCM_SETTODAY,0,(LPARAM)pDateTime); 
	}
	// --- In  :	refDateTime		-	A reference to a COleDateTime object that 
	//									contains the current date.
	//				pDateTime		-	In the second version, a pointer to a CTime 
	//									object containing the current date information. 
	//									In the third version, a pointer to a SYSTEMTIME 
	//									structure that contains the current date 
	//									information. 
	// --- Out : 
	// --- Returns:
	// --- Effect :	Sets the "today" selection for a month calendar control. If the 
	//				"today" selection is set to any date other than the default, the 
	//				following conditions apply: 
	//
	//				The control will not automatically update the "today" selection when 
	//				the time passes midnight for the current day. 
	//
	//				The control will not automatically update its display based on locale 
	//				changes.  

	BOOL GetToday(CTime& refTime) const;
	BOOL GetToday(COleDateTime& refDateTime) const;
	inline BOOL GetToday(LPSYSTEMTIME pDateTime) const { 
		ASSERT(m_hWnd!=NULL); 
		return (BOOL)::SendMessage(m_hWnd,MCM_GETTODAY,0,(LPARAM) pDateTime); 
	}
	// --- In  :	
	// --- Out :	refDateTime		-	A reference to a COleDateTime or CTime object 
	//									indicating the current day.
	//				pDateTime		-	A pointer to a SYSTEMTIME structure that will 
	//									receive the date information. This parameter must 
	//									be a valid address and cannot be NULL.
	// --- Returns:	Nonzero if successful; otherwise 0.
	// --- Effect : Retrieves the date information for the date specified as "today" 
	//				for a month calendar control. 

	inline BOOL SetCurSel(const LPSYSTEMTIME pDateTime) { 
		ASSERT(m_hWnd!=NULL); 
		return (BOOL)::SendMessage(m_hWnd,MCM_SETCURSEL,0,(LPARAM)pDateTime); 
	}
	BOOL SetCurSel(const CTime& refDateTime);
	BOOL SetCurSel(const COleDateTime& refDateTime);
	// --- In  :	refDateTime		-	A reference to a COleDateTime or CTime object 
	//									indicating the currently-selected month calendar 
	//									control. 
	//				pDateTime		-	Pointer to a SYSTEMTIME structure that contains 
	//									the date to be set as the current selection. 
	// --- Out :	
	// --- Returns:	Nonzero if successful; otherwise 0. 
	// --- Effect : Sets the currently selected date for a month calendar control. If the 
	//				specified date is not in view, the control updates the display to 
	//				bring it into view. 

	inline BOOL GetCurSel(LPSYSTEMTIME pDateTime) const { 
		ASSERT(m_hWnd!=NULL); 
		return (BOOL)::SendMessage(m_hWnd,MCM_GETCURSEL,0,(LPARAM)pDateTime); 
	}
	BOOL GetCurSel(CTime& refDateTime) const;
	BOOL GetCurSel(COleDateTime& refDateTime) const;
	// --- In  :
	// --- Out :	refDateTime		-	A reference to a COleDateTime object or a CTime 
	//									object. Receives the current time.
	//				pDateTime		-	A pointer to a SYSTEMTIME structure that will 
	//									receive the currently-selected date information. 
	//									This parameter must be a valid address and cannot 
	//									be NULL.
	// --- Returns:	Nonzero if successful; otherwize 0.
	// --- Effect : Retrieves the currently selected date.

	BOOL SetDayState(int nMonths, LPMONTHDAYSTATE pStates);
	// --- In  :	nMonths			-	Value indicating how many elements are in the 
	//									array that pStates points to.
	//				pStates			-	A pointer to a MONTHDAYSTATE array of values that 
	//									define how the month calendar control will draw 
	//									each day in its display. The MONTHDAYSTATE data 
	//									type is a bit field, where each bit (1 through 31) 
	//									represents the state of a day in a month. If a bit 
	//									is on, the corresponding day will be displayed in 
	//									bold; otherwise it will be displayed with no 
	//									emphasis. 
	//									
	//									MONTHDAYSTATE is a new data type that is defined as
	//				
	//									typedef DWORD MONTHDAYSTATE, FAR * LPMONTHDAYSTATE;
	//				
	//									The following is a simple macro that sets a bit in 
	//									a MONTHDAYSTATE value for a given day within that 
	//									month. 
	//						
	//									#define BOLDDAY(ds,iDay) if(iDay>0 && iDay<32)\
	//				                            (ds)|=(0x00000001<<(iDay-1))
	//
	//									Using this macro, an application could simply 
	//									loop through an array of important dates, setting 
	//									bits within the corresponding array elements. 
	//									This approach is not the most efficient, of course,
	//									but works well for many purposes. As long as your 
	//									application sets MONTHDAYSTATE bits appropriately, 
	//									it does not matter how those bits were set. 
	// --- Out : 
	// --- Returns:	Nonzero if successful; otherwise 0.
	// --- Effect : Sets the day states for all months that are currently visible within 
	//				a month calendar control. The array at pStates must contain as many 
	//				elements as the value returned by the GetMonthRange function. 
	//				Keep in mind that the array at pStates must contain MONTHDAYSTATE 
	//				values that correspond with all months currently in the control's 
	//				display, in chronological order. This includes the two months only 
	//				partially displayed before the first month and after the last month.

	inline BOOL SetMaxSelCount(int nMax) { 
		ASSERT(m_hWnd!=NULL); 
		return (BOOL)::SendMessage(m_hWnd,MCM_SETMAXSELCOUNT,nMax,0); 
	}
	// --- In  :	nMax		-	The value that will be set to represent the maximum 
	//								number of selectable days. 
	// --- Out :	
	// --- Returns:	Nonzero if successful; otherwise 0. 
	// --- Effect : Sets the maximum number of days that can be selected in a month 
	//				calendar control.

	inline int GetMaxSelCount() const { 
		ASSERT(m_hWnd!=NULL); 
		return (int)::SendMessage(m_hWnd,MCM_GETMAXSELCOUNT,0,0); 
	}
	// --- In  :
	// --- Out : 
	// --- Returns:	An integer value that represents the total number of days that can 
	//				be selected for the control.
	// --- Effect : Retrieves the maximum date range that can be selected in a month 
	//				calendar control. Use this member function for controls with the 
	//				MCS_MULTISELECT style set.

	BOOL SetRange(const COleDateTime* pMinRange, const COleDateTime* pMaxRange);
	BOOL SetRange(const CTime* pMinRange, const CTime* pMaxRange);
	BOOL SetRange(const LPSYSTEMTIME pMinRange, const LPSYSTEMTIME pMaxRange);
	// --- In  :	pMinRange		-	A pointer to a COleDateTime object, a CTime object,
	//									or SYSTEMTIME structure containing the date at 
	//									the lowest end of the range. 
	//				pMaxRange		-	A pointer to a COleDateTime object, a CTime object,
	//									or SYSTEMTIME structure containing the date at the 
	//									highest end of the range.
	// --- Out : 
	// --- Returns:	Nonzero if successful; otherwise 0. 
	// --- Effect : Sets the minimum and maximum allowable dates for a month calendar 
	//				control. 

	DWORD GetRange(COleDateTime* pMinRange, COleDateTime* pMaxRange) const;
	DWORD GetRange(CTime* pMinRange, CTime* pMaxRange) const;
	DWORD GetRange(LPSYSTEMTIME pMinRange, LPSYSTEMTIME pMaxRange) const;
	// --- In  :
	// --- Out :	pMinRange		-	A pointer to a COleDateTime object, a CTime object,
	//									or SYSTEMTIME structure containing the date at the 
	//									lowest end of the range. 
	//				pMaxRange		-	A pointer to a COleDateTime object, a CTime object,
	//									or SYSTEMTIME structure containing the date at the 
	//									highest end of the range.
	//
	// --- Returns:	A DWORD that can be zero (no limits are set) or a combination of the 
	//				following values that specify limit information. 
	//
	//				GDTR_MAX		-	A maximum limit is set for the control; 
	//									pMaxRange is valid and contains the applicable 
	//									date information.  
	//				GDTR_MIN		-	A minimum limit is set for the control; pMinRange 
	//									is valid and contains the applicable date 
	//									information.  
	// --- Effect : Retrieves the minimum and maximum allowable dates set for a month 
	//				calendar control.

	int GetMonthRange(COleDateTime& refMinRange, COleDateTime& refMaxRange,
		DWORD dwFlags) const;
	int GetMonthRange(CTime& refMinRange, CTime& refMaxRange,
		DWORD dwFlags) const;
	int GetMonthRange(LPSYSTEMTIME pMinRange, LPSYSTEMTIME pMaxRange,
		DWORD dwFlags) const;
	// --- In  :	dwFlags			-	Value specifying the scope of the range limits to 
	//									be retrieved. This value must be one of the 
	//									following.
	//
	//									GMR_DAYSTATE	-	Include preceding and trailing 
	//														months of visible range that 
	//														are only partially displayed.  
	//									GMR_VISIBLE		-	Include only those months that 
	//														are entirely displayed.  
	//
	// --- Out :	refMinRange		-	A reference to a COleDateTime or CTime object 
	//									containing the minimum date allowed.
	//				refMaxRange		-	A reference to a COleDateTime or CTime object 
	//									containing the maximum date allowed.
	//				pMinRange		-	A pointer to aSYSTEMTIME structure containing the 
	//									date at the lowest end of the range.
	//				pMaxRange		-	A pointer to a SYSTEMTIME structure containing the 
	//									date at the highest end of the range.
	//				
	// --- Returns:	An integer that represents the range, in months, spanned by the two 
	//				limits indicated by refMinRange and refMaxRange in the first and 
	//				second versions, or pMinRange and pMaxRange in the third version.
	// --- Effect : Retrieves date information that represents the high and low limits of 
	//				a month calendar control's display. 

	BOOL SetSelRange(const COleDateTime& pMinRange,const COleDateTime& pMaxRange);
	BOOL SetSelRange(const CTime& pMinRange, const CTime& pMaxRange);
	BOOL SetSelRange(const LPSYSTEMTIME pMinRange, const LPSYSTEMTIME pMaxRange);
	// --- In  :	pMinRange		-	A pointer to a COleDateTime object, a CTime object,
	//									or SYSTEMTIME structure containing the date at the 
	//									lowest end of the range. 
	//				pMaxRange		-	A pointer to a COleDateTime object, a CTime object,
	//									or SYSTEMTIME structure containing the date at the 
	//									highest end of the range.
	// --- Out :	
	// --- Returns:	Nonzero if successful; otherwise 0. 
	// --- Effect : Sets the selection for a month calendar control to a given date range. 

	BOOL GetSelRange(COleDateTime& refMinRange, COleDateTime& refMaxRange) const;
	BOOL GetSelRange(CTime& refMinRange, CTime& refMaxRange) const;
	BOOL GetSelRange(LPSYSTEMTIME pMinRange, LPSYSTEMTIME pMaxRange) const;
	// --- In  :
	// --- Out :	refMinRange		-	A reference to a COleDateTime or CTime object 
	//									containing the minimum date allowed.
	//				refMaxRange		-	A reference to a COleDateTime or CTime object 
	//									containing the maximum date allowed.
	//				pMinRange		-	A pointer to aSYSTEMTIME structure containing the 
	//									date at the lowest end of the range.
	//				pMaxRange		-	A pointer to a SYSTEMTIME structure containing 
	//									the date at the highest end of the range.
	// --- Returns:	Nonzero if successful; otherwise 0.
	// --- Effect : Retrieves date information that represents the upper and lower limits 
	//				of the date range currently selected by the user. GetSelRange will 
	//				fail if applied to a month calendar control that does not use the 
	//				MCS_MULTISELECT style.

#endif	//	_MFC_VER>0x0421

	inline DWORD GetMonthCalStyle() const { 
		ASSERT(::IsWindow(GetSafeHwnd()));
		return m_dwMonthCalStyle; 
	}
	// --- In  :	
	// --- Out : 
	// --- Returns:	DWORD value of COXMonthCalCtrl specific style
	// --- Effect : Retrieves COXMonthCalCtrl specific style. Refer to description of
	//				SetMonthCalStyle function for more details	

	inline void SetMonthCalStyle(const DWORD dwMonthCalStyle) { 
		ASSERT(::IsWindow(GetSafeHwnd()));
		EnableToolTips(dwMonthCalStyle&OXMCS_INFOTIP);
		m_dwMonthCalStyle=dwMonthCalStyle; 
	}
	// --- In  :	dwMonthCalStyle	-	COXMonthCalendar control specific styles. The 
	//									following are the specific styles used with month 
	//									calendar controls: 
	//
	//					OXMCS_BOLDHOLIDAYS	-	The month calendar will draw holidays 
	//											that can be specified using SetHolidays
	//											function in bold. 
	//					OXMCS_INFOTIP		-	We provide built-in support for tooltips 
	//											(infotips) for any element found in 
	//											calendar control (title, background, 
	//											calendar day, etc). It's programmer's 
	//											responsibility to provide textual 
	//											information for tooltips. If 
	//											OXMCS_INFOTIP style set then 
	//											COXMonthCalCtrl will send 
	//											OXMCN_GETINFOTIP notification to its 
	//											parent window. Refer to description of 
	//											OXMCN_GETINFOTIP notification for 
	//											details how to implement tooltips in 
	//											your application
	//
	// --- Out : 
	// --- Returns:	DWORD value of COXMonthCalCtrl specific style
	// --- Effect : Retrieves COXMonthCalCtrl specific style

	inline COLORREF SetBkColor(COLORREF ref) { 
		ASSERT(m_hWnd!=NULL); 
		return SetColor(MCSC_BACKGROUND, ref); 
	}
	// --- In  :	ref			-	A COLORREF value for the background color displayed 
	//								between months. 
	// --- Out : 
	// --- Returns:	A COLORREF value that represents the previous color setting for 
	//				the background color displayed between months, if successful. 
	//				Otherwise this function returns -1. 
	// --- Effect : Sets the color for background color displayed between months.

	inline COLORREF SetMonthBkColor(COLORREF ref) { 
		ASSERT(m_hWnd!=NULL); 
		return SetColor(MCSC_MONTHBK, ref); 
	}
	// --- In  :	ref			-	A COLORREF value for background color displayed 
	//								within the month. 
	// --- Out : 
	// --- Returns:	A COLORREF value that represents the previous color setting for 
	//				the background color displayed within the month, if successful. 
	//				Otherwise this function returns -1. 
	// --- Effect : Sets the color for a background color displayed within the month

	inline COLORREF SetTextColor(COLORREF ref) { 
		ASSERT(m_hWnd!=NULL); 
		return SetColor(MCSC_TEXT, ref); 
	}
	// --- In  :	ref			-	A COLORREF value for the color used to display text 
	//								within a month. 
	// --- Out : 
	// --- Returns:	A COLORREF value that represents the color used to display text 
	//				within a month, if successful. Otherwise this function returns -1. 
	// --- Effect : Sets the color for a color used to display text within a month

	inline COLORREF SetTitleBkColor(COLORREF ref) { 
		ASSERT(m_hWnd!=NULL); 
		return SetColor(MCSC_TITLEBK, ref); 
	}
	// --- In  :	ref			-	A COLORREF value for the background color displayed 
	//								in the calendar's title. 
	// --- Out : 
	// --- Returns:	A COLORREF value that represents the background color displayed 
	//				in the calendar's title, if successful. Otherwise this function 
	//				returns -1. 
	// --- Effect : Sets the color for a background color displayed in the calendar's 
	//				title

	inline COLORREF SetTitleTextColor(COLORREF ref) { 
		ASSERT(m_hWnd!=NULL); 
		return SetColor(MCSC_TITLETEXT, ref); 
	}
	// --- In  :	ref			-	A COLORREF value for the color used to display text 
	//								within the calendar's title. 
	// --- Out : 
	// --- Returns:	A COLORREF value that represents the previous color setting for 
	//				the color used to display text within the calendar's title, 
	//				if successful. Otherwise this function returns -1. 
	// --- Effect : Sets the color for a color used to display text within the 
	//				calendar's title

	inline COLORREF SetTrailingTextColor(COLORREF ref) { 
		ASSERT(m_hWnd!=NULL); 
		return SetColor(MCSC_TRAILINGTEXT, ref); 
	}
	// --- In  :	ref			-	A COLORREF value for the color used to display 
	//								header and trailing-day text. Header and trailing 
	//								days are the days from the previous and following 
	//								months that appear on the current calendar. 
	// --- Out : 
	// --- Returns:	A COLORREF value that represents the previous color setting for 
	//				the color used to display header and trailing-day text, if successful. 
	//				Otherwise this function returns -1. 
	// --- Effect : Sets the color for a color used to display header and 
	//				trailing-day text

	inline COLORREF GetBkColor() const { 
		ASSERT(m_hWnd!=NULL); 
		return GetColor(MCSC_BACKGROUND); 
	}
	// --- In  :	
	// --- Out : 
	// --- Returns:	A COLORREF value specifying the background color displayed between 
	//				months, if successful. Otherwise, this member function returns -1.
	// --- Effect : Call this member function to retrieve the background color displayed 
	//				between months.

	inline COLORREF GetMonthBkColor() const { 
		ASSERT(m_hWnd!=NULL); 
		return GetColor(MCSC_MONTHBK); 
	}
	// --- In  :	
	// --- Out : 
	// --- Returns:	A COLORREF value specifying the background color displayed within 
	//				the month, if successful. Otherwise, this member function returns -1.
	// --- Effect : Call this member function to retrieve the background color displayed 
	//				within the month.

	inline COLORREF GetTextColor() const { 
		ASSERT(m_hWnd!=NULL); 
		return GetColor(MCSC_TEXT); 
	}
	// --- In  :	
	// --- Out : 
	// --- Returns:	A COLORREF value specifying the color used to display text within a 
	//				month, if successful. Otherwise, this member function returns -1.
	// --- Effect : Call this member function to retrieve the color used to display text 
	//				within a month.

	inline COLORREF GetTitleBkColor() const { 
		ASSERT(m_hWnd!=NULL); 
		return GetColor(MCSC_TITLEBK); 
	}
	// --- In  :	
	// --- Out : 
	// --- Returns:	A COLORREF value specifying the background color displayed in the 
	//				calendar's title, if successful. Otherwise, this member function 
	//				returns -1.
	// --- Effect : Call this member function to retrieve the background color displayed 
	//				in the calendar's title.

	inline COLORREF GetTitleTextColor() const { 
		ASSERT(m_hWnd!=NULL); 
		return GetColor(MCSC_TITLETEXT); 
	}
	// --- In  :	
	// --- Out : 
	// --- Returns:	A COLORREF value specifying the color used to display text within the 
	//				calendar's title, if successful. Otherwise, this member function 
	//				returns -1.
	// --- Effect : Call this member function to retrieve the color used to display text 
	//				within the calendar's title.

	inline COLORREF GetTrailingTextColor() const { 
		ASSERT(m_hWnd!=NULL); 
		return GetColor(MCSC_TRAILINGTEXT); 
	}
	// --- In  :	
	// --- Out : 
	// --- Returns:	A COLORREF value specifying the color used to display header and 
	//				trailing-day text, if successful. Header and trailing days are the 
	//				days from the previous and following months that appear on the 
	//				current calendar. Otherwise, this member function returns -1.
	// --- Effect : Call this member function to retrieve the color used to display 
	//				header and trailing-day text.

	BOOL SetHolidays(LPHOLIDAYS pHolidays);
	// --- In  :	pHolidays		-	pointer to valid HOLIDAYS structure
	// --- Out : 
	// --- Returns:	Nonzero if successful; otherwise 0.
	// --- Effect : Sets the holidays for specified in the HOLIDAYS structure month 
	//				and year. 

	BOOL GetHolidays(LPHOLIDAYS pDestHolidays);
	// --- In  :	pHolidays		-	pointer to valid HOLIDAYS structure to be filled
	// --- Out : 
	// --- Returns:	Nonzero if successful; otherwise 0.
	// --- Effect : Gets the holidays for specified in the HOLIDAYS structure month 
	//				and year. 

	BOOL SetCtrlDimension(int nXDimension, int nYDimension, 
		const CSize& szMargins=CSize(0,0));
	// --- In  :	nXDimension		-	the number of months that should be displayed 
	//									horizontally in the control, mustn't be less than 1
	//				nYDimension		-	the number of months that should be displayed 
	//									vertically in the control, mustn't be less than 1
	//				szMargins		-	cx element of it represents margin that must be 
	//									added to the right and left sides of the calendar
	//									control, cx element of it represents margin that 
	//									must be added to the top and bottom sides of the 
	//									calendar control
	// --- Out : 
	// --- Returns:	Nonzero if successful; otherwise 0.
	// --- Effect : 

	BOOL GetReqRect(RECT* pRect, int nXDimension, int nYDimension) const;
	// --- In  :	pRect			-	A pointer to a RECT structure that will receive 
	//									bounding rectangle information. This parameter 
	//									must be a valid address and cannot be NULL. 
	//				nXDimension		-	the number of months that should be displayed 
	//									horizontally in the control, mustn't be less than 1
	//				nYDimension		-	the number of months that should be displayed 
	//									vertically in the control, mustn't be less than 1
	// --- Out : 
	// --- Returns:	If successful, this member function returns nonzero and lpRect 
	//				receives the applicable bounding information. If unsuccessful, 
	//				the member function returns 0.
	// --- Effect : Retrieves the minimum size required to display specified number of 
	//				month vertically and horizontally in a month calendar control. 
	//				The top and left members of pRect will always be zero. The right and 
	//				bottom members represent the minimum cx and cy required for the 
	//				control. The required window size for a month calendar control depends 
	//				on the currently selected font. 

	BOOL GetMonthFitCount(CSize& szMonthFit) const;
	// --- In  :	
	// --- Out :	szMonthFit		-	cx element of it will be fitlled with the number
	//									of month displayed horizontally, cy element of it 
	//									will be fitlled with the number	of month displayed 
	//									vertically
	// --- Returns:	TRUE if succeed, or FALSE otherwise
	// --- Effect : 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXSHBListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

public:
	virtual ~COXMonthCalCtrl();
	// --- In  :
	// --- Out : 
	// --- Returns:
	// --- Effect : destructor.


	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	// --- In  :	point	-	Specifies the x- and y-coordinate of the cursor. 
	//							These coordinates are always relative to the upper-left 
	//							corner of the window
	//				pTI		-	A pointer to a TOOLINFO structure
	// --- Out : 
	// --- Returns:	If -1, the tooltip control was not found, otherwise was found
	// --- Effect : this function is called to detemine whether a point is in the 
	//				bounding rectangle of the specified tool. If the point is in the 
	//				rectangle, it retrieves information about the tool. If the area 
	//				with which the tooltip is associated is not a button, OnToolHitTest 
	//				sets the structure flags to TTF_NOTBUTTON and TTF_CENTERTIP. 
	//				See TOOLINFO, in the Win32 SDK Programmer's Reference, for more 
	//				information about the structure.

protected:

	// Generated message map functions
	//{{AFX_MSG(COXMonthCalCtrl)
	afx_msg void OnDestroy( );
	afx_msg LRESULT OnGetCurSel(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	afx_msg BOOL OnToolTip(UINT id, NMHDR* pTTTStruct, LRESULT* pResult);
	afx_msg BOOL OnGetDayState(NMHDR *pHdr, LRESULT *pRes);
	DECLARE_MESSAGE_MAP()

protected:
	// initialize calendar control
	virtual BOOL InitMonthCalCtrl();

public:
#if _MFC_VER<=0x0421
	static BOOL GetAsSystemTime(const COleDateTime& rDateTime, SYSTEMTIME& sysTime);
	static BOOL GetAsSystemTime(const CTime& rTime, SYSTEMTIME& timeDest);
	static BOOL TimeFromOleDate(DATE dtSrc, struct tm& tmDest);
#endif	//	_MFC_VER<=0x0421
	// convert the WEEKDAYS to the internal represantation used by COleDateTime
	static int GetAsCOleDateTimeDayOfWeek(const WEEKDAYS weekDay);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(_OXMONTHCALCTRL_H_)
