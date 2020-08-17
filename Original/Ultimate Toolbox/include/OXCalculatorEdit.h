
// Version: 9.3

#if !defined(_OXCALCULATORREDIT_H__)
#define _OXCALCULATORREDIT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"


#include "OXEdit.h"
#include "OXDropEdit.h"
#include "OXCalculatorCtrl.h"

// OXCalculatorEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COXCalculatorEdit window

class OX_CLASS_DECL COXCalculatorEdit : public COXDropEdit<COXNumericEdit>
{
	DECLARE_DYNCREATE(COXCalculatorEdit)
// Construction
public:
	COXCalculatorEdit();

// Attributes
public:
	
protected:
	// calculator popup window
	COXCalculatorPopup m_calculatorPopup;

// Operations
public:

	// --- In  :
	// --- Out : 
	// --- Returns:	pointer to COXCalculatorPopup object associated with button
	// --- Effect : 
	inline COXCalculatorPopup* GetCalculatorPopup() { return &m_calculatorPopup; }


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXCalculatorEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXCalculatorEdit() {};

	// initialize control
	virtual BOOL InitializeDropEdit();

	// called every time dropdown button is pressed
	virtual void OnDropButton();


	// Generated message map functions
protected:
	//{{AFX_MSG(COXCalculatorEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif / !defined(_OXCALCULATORREDIT_H__)
