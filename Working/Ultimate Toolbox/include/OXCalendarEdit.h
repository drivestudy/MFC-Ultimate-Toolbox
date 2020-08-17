// ==========================================================================
// 			Class Specification : COXCalendarEdit, COXMonthCalPopup
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////
#if !defined(AFX_OXCALENDAREDIT_H__B08CF643_2C81_4A4B_89FF_9223FABF0040__INCLUDED_)
#define AFX_OXCALENDAREDIT_H__B08CF643_2C81_4A4B_89FF_9223FABF0040__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OXCalendarEdit.h : header file
//

#include "OXDllExt.h"
#include "OXDropEdit.h"
#include "OXMaskedEdit.h"


class OX_CLASS_DECL COXMonthCalPopup : public CMonthCalCtrl
{
// Construction
public:
	COXMonthCalPopup();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXMonthCalPopup)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Pick(CRect rect, CRect rectParent, COleDateTime dtInitialDate, COleDateTime& dtPickedDate);
	virtual ~COXMonthCalPopup();

	// Generated message map functions
protected:
	//{{AFX_MSG(COXMonthCalPopup)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	// This function modifies the given rectangle so that it is within the bounds
	// of the closest monitor.
	void AdjustDisplayRectangle(CRect& rect, CRect rectParent);
};

/////////////////////////////////////////////////////////////////////////////
// COXCalendarEdit window

/*
Description:

  This class implementes a date drop edit control with a calendar popup
*/

class OX_CLASS_DECL COXCalendarEdit : public COXDropEdit<COXMaskedEdit>
{
// Construction
public:
	COXCalendarEdit(LPCTSTR lpszDateFormat);
	COXCalendarEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXCalendarEdit)
		afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
	// Determines if the given date is empty
	BOOL IsEmptyDate(LPCTSTR lpszTestDate);

	// Determines if the given gate is valid
	virtual BOOL IsValidDate(LPCTSTR lpszTestDate);

	// Return the date in a CString object
	virtual CString GetText();

	// Sets the date from a string having a formatted date
	virtual void SetText(LPCTSTR lpszDate);

	// Returns a string with the formatted date from a COleDateTime object
	CString GetStringFromDate(COleDateTime dtDate);

	// Returns a COleDateTime object from a formatted string
	COleDateTime GetDateFromString(LPCTSTR lpszDateString);

	// Sets the format of the date
	void SetDateFormat(LPCTSTR lpszDateFormat);

	virtual ~COXCalendarEdit();

	// Generated message map functions
protected:
	// Returns the error string to be displayed when a bad date is entered.
	virtual CString GetErrorString();

	virtual BOOL InitializeDropEdit();
	virtual void OnDropButton();
	DECLARE_DYNCREATE(COXCalendarEdit)
	//{{AFX_MSG(COXCalendarEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	COXMonthCalPopup m_Calendar;
	CString m_strDateFormat;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OXCALENDAREDIT_H__B08CF643_2C81_4A4B_89FF_9223FABF0040__INCLUDED_)
