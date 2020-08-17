/////////////////////////////////////////////////////////////////////////////
// Class Specification : COXCalendar
/////////////////////////////////////////////////////////////////////////////
// OXCalendar.h : header file

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
//	YES	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :
// This class implement date edit window with ability
// to visually pick a date from a calendar

// To use it in resouce (for example, in CFormView or CDialog-
// derived classes you just place CStatic or CEdit control with apropriate
// ID (rather than -1) on your form and replace ClassWizard-provided member
// variable type to COXCalendar (don't forget include OXCalendar.h header file)
// Use Create member function to create it dynamically

//When COXCalendar receives input focus, it looks like drop-down combo box
//showing current date in edit field.When user click down arrow, a calendar
//window appeared. It shows the days of the week on top.Then 6 lines to show 
//the days of the month. The days of current month are shown in black. 
//The days of the previous and next month are shown in a lighter color
// Now user can select a date by mouse clicking and jump to 
//the previous or next month.Also he can select the date of today using
// button with same name. Other button can be used to select no date. 
//In the latter case the combo box will also show "None".

//If user type a date in text field and click down arrow, calendar window
//initially will show a month contained this date. If no text typed, it will
//show current month.
#ifndef _OXCALENDAR_H
#define _OXCALENDAR_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include "OXCalendarPopup.h"

class OX_CLASS_DECL COXCalendar : public CWnd
{
// Data members -------------------------------------------------------------
	//to handle construction and destroying common class data
public:
	// store default locale weekday names
	static CStringArray ms_straWeekDays;
	// store default locale month names
	static CStringArray ms_straMonthNames;

public:
	enum {IDEDIT=100};
	CEdit m_wndEdit;
	CFont m_font;

protected:
	void ShrinkDateInBounds();
	COleDateTime m_CurrentDate;
	COleDateTime m_EndDate;
	COleDateTime m_StartDate;
	int m_nFirstDayOfWeek;
	COXCalendarPopup m_wndDatePicker;
	HCURSOR m_hCursor;
	BOOL m_bDrawButton;
	int m_nButtonWidth;
	CString m_sFormat;

private:
	static BOOL CALLBACK EnumDayOfWeek(LPTSTR lpCalendarInfoString);
	static BOOL CALLBACK EnumMonth(LPTSTR lpCalendarInfoString);

public:
	static void GetCalendarLocaleData();
	static int GetLocaleFirstDayOfWeek();

// Member functions ---------------------------------------------------------
public:
	COXCalendar();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object

	BOOL SetCurrentDate(const COleDateTime &currDate);
	// --- In  :date value to set as current
	// --- Out : 
	// --- Returns :Whether the setting was successful
	//--- Effect :sets current date value

	COleDateTime GetCurrentDate();
	// --- In  :
	// --- Out : 
	// --- Returns :value of current date displayed in control
	// --- Effect :

	void GetDateRange(COleDateTime& startDate, COleDateTime& endDate);
	// --- In  :
	// --- Out :values of current range of valid dates
	// --- Returns :
	// --- Effect :

	BOOL SetDateRange(COleDateTime startDate, COleDateTime endDate);
	// --- In  :start and end date of valid date range
	// --- Out :
	// --- Returns :
	// --- Effect :

	int GetFirstDayOfWeek();
	// --- In  :
	// --- Out :
	// --- Returns :number of first day of week
		//using the following values:
				//			0 Sunday, 1 Monday, 
				//			2 Tuesday, 3 Wednesday,
				//			4 Thursday, 5 Friday, 6 Saturday. 
	// --- Effect :

	BOOL SetFirstDayOfWeek(int nDayOfWeek);
	// --- In  :number of first day of week
		//using the following values:
				//			0 Sunday, 1 Monday, 
				//			2 Tuesday, 3 Wednesday,
				//			4 Thursday, 5 Friday, 6 Saturday
	// --- Out :
	// --- Returns : success (TRUE) or failure (FALSE)
	//				if incorrect value specified
	// --- Effect :Set number of first day of week



	// 
	BOOL Create(DWORD dwStyle,const CRect&r ,CWnd * pParentWnd,UINT nID);
	// --- In  :window style, rectangle, parent and ID
	// --- Out : 
	// --- Returns :Whether the creation was successful
	// --- Effect :Creates a Windows child window and 
	//			attaches it to the CWnd object. 
 

	/////////////////////////////
	// --- In  : A formatting string
	// --- Out : 
	// --- Returns :
	// --- Effect :sets current formatting string
	void SetFormat(LPCTSTR sFormat) { m_sFormat=sFormat; }

	// --- In  :
	// --- Out : 
	// --- Returns :gets current formatting string
	//--- Effect :
	CString GetFormat() { return m_sFormat; }
	////////////////////

public:
	virtual ~COXCalendar();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXCalendar)
	protected:
	virtual void PreSubclassWindow();
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL


protected:
	void ReInitLayout(BOOL bShowBtn);

	///
	virtual void SetOutputText(CString& sOutputString);
	///

	BOOL Init();
	// Generated message map functions
	//{{AFX_MSG(COXCalendar)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg LRESULT OnSettingChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnChange();
	afx_msg void OnEnUpdate();
	afx_msg void OnSetFocusEdit();
	afx_msg void OnKillFocusEdit();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif