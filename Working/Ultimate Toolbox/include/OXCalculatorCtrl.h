// Version: 9.3

#if !defined(_OXCALCULATORCTRL_H__)
#define _OXCALCULATORCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"

#include "OXBitmapButton.h"
#include "OXStatic.h"

#include "OXMainRes.h"

#define ID_NUMBERSYSTEM_DECIMAL			0
#define ID_NUMBERSYSTEM_HEXADECIMAL		1
#define ID_NUMBERSYSTEM_OCTAL			2
#define ID_NUMBERSYSTEM_BINARY			3

#ifndef	PI
#define PI								3.14159265359
#endif

#define MAX_DIGIT_COUNT					13
#define MAX_FRACTIONAL_COUNT			12


// OXCalculatorCtrl.h : header file
//


enum Operator { OpNone, OpAdd, OpSubtract, OpMultiply, OpDivide, OpMod, 
				OpRaiseToPower, OpSin, OpCos, OpTan, OpCotan, OpFactorial,
				OpLogarithm, OpNaturalLogarithm, OpReciprocal, OpSqrt };
enum CalcError { ErrNone, ErrDivideByZero, ErrOverflow };

/////////////////////////////////////////////////////////////////////////////
// COXCalculatorCtrl control

class OX_CLASS_DECL COXCalculatorCtrl : public CDialog
{
// Construction
public:
	COXCalculatorCtrl(CWnd* pParent=NULL, double dValue=0);   // standard constructor

	virtual BOOL Create(CWnd* pParentWnd);

protected:
// Dialog Data
	//{{AFX_DATA(COXCalculatorCtrl)
	enum { IDD = IDD_OX_DIALOG_CALCULATOR };
	CComboBox	m_comboNumberSystem;
	COXBitmapButton	m_btnNaturalAlgorithm;
	COXBitmapButton	m_btnLogarithm;
	COXBitmapButton	m_btnF;
	COXBitmapButton	m_btnE;
	COXBitmapButton	m_btnD;
	COXBitmapButton	m_btnC;
	COXBitmapButton	m_btnB;
	COXBitmapButton	m_btnA;
	COXStatic	m_ctlDisplay;
	COXBitmapButton	m_btnTan;
	COXBitmapButton	m_btnSin;
	COXBitmapButton	m_btnPower;
	COXBitmapButton	m_btnPi;
	COXBitmapButton	m_btnFactorial;
	COXBitmapButton	m_btnCotan;
	COXBitmapButton	m_btnCos;
	COXBitmapButton	m_btnSubtract;
	COXBitmapButton	m_btnSqrt;
	COXBitmapButton	m_btnReciprocal;
	COXBitmapButton	m_btnMultiply;
	COXBitmapButton	m_btnMod;
	COXBitmapButton	m_btnEqual;
	COXBitmapButton	m_btnDivide;
	COXBitmapButton	m_btnDelimiter;
	COXBitmapButton	m_btnClearNumber;
	COXBitmapButton	m_btnClear;
	COXBitmapButton	m_btnChangeSign;
	COXBitmapButton	m_btnBack;
	COXBitmapButton	m_btnAdd;
	COXBitmapButton	m_btn9;
	COXBitmapButton	m_btn8;
	COXBitmapButton	m_btn7;
	COXBitmapButton	m_btn6;
	COXBitmapButton	m_btn5;
	COXBitmapButton	m_btn4;
	COXBitmapButton	m_btn3;
	COXBitmapButton	m_btn2;
	COXBitmapButton	m_btn1;
	COXBitmapButton	m_btn0;
	BOOL	m_bScientific;
	int		m_nNumberSystem;
	CString	m_sDisplayString;
	//}}AFX_DATA

	double m_dValue;
	double m_dOperand;
	BOOL m_bOperandAvailable;
	BOOL m_bAddingFractional;
	Operator m_operator;
	Operator m_lastCompletedOperator;
	CalcError m_errorState;

	TCHAR m_chDecimalSeparator;
	int m_nFractionalCount;

	HACCEL m_hAccel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXCalculatorCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	inline TCHAR GetDecimalSeparator() const { return m_chDecimalSeparator; }
	void SetDecimalSeparator(TCHAR chDecimalSeparator); 

	inline double GetResult() const { return m_dValue; }
	inline CString GetResultAsString() const { return GetDisplayString(m_dValue); }

	inline int GetNumberSystem() const { return m_nNumberSystem; }
	void SetNumberSystem(const int nNumberSystem);

	void SetOperand(double dOperand, BOOL bRecalcLayout=FALSE);
	void SetResult(double dValue);

// Implementation
protected:
	void SetupButton(COXBitmapButton* pButton, COLORREF clrText);

	void ShowControls();
	
	CString GetDisplayString(double dValue, int nFractionalCount=-1) const;

	int GetDecimalDigitCount(double dValue) const;
	int GetFractionalDigitCount(double dValue) const;

	void AddDigit(double dDigit);
	void RemoveLastDigit();

	void PerformUnaryOperation(Operator oper);
	void PerformBinaryOperation(Operator oper);
	void PerformLastOperation();

	// Generated message map functions
	//{{AFX_MSG(COXCalculatorCtrl)
	afx_msg void OnButton0();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonBack();
	afx_msg void OnButtonChangesign();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonClearnumber();
	afx_msg void OnButtonDelimiter();
	afx_msg void OnButtonDivide();
	afx_msg void OnButtonEqual();
	afx_msg void OnButtonMod();
	afx_msg void OnButtonMultiply();
	afx_msg void OnButtonReciprocal();
	afx_msg void OnButtonSqrt();
	afx_msg void OnButtonSubtract();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonA();
	afx_msg void OnButtonB();
	afx_msg void OnButtonC();
	afx_msg void OnButtonCos();
	afx_msg void OnButtonCotan();
	afx_msg void OnButtonD();
	afx_msg void OnButtonE();
	afx_msg void OnButtonF();
	afx_msg void OnButtonFactorial();
	afx_msg void OnButtonLogarithm();
	afx_msg void OnButtonNaturalLogarithm();
	afx_msg void OnButtonPi();
	afx_msg void OnButtonPower();
	afx_msg void OnButtonSin();
	afx_msg void OnButtonTan();
	afx_msg void OnCheckScientific();
	afx_msg void OnSelchangeComboNumberSystem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class COXCalculatorPopup;
};

/////////////////////////////////////////////////////////////

// popup window alignment
//
// popup window window RightTop will be displayed at the LeftTop point of parent window 
// or rect specified in COXPopupBarCtrl::Pick function
#define ID_OXCALCULATOR_ALIGNLEFT				0x00000000		
// popup window window LeftTop will be displayed at the RightTop point of parent window 
// or rect specified in COXPopupBarCtrl::Pick function
#define ID_OXCALCULATOR_ALIGNRIGHT				0x00000001		
// popup window window LeftBottom will be displayed at the LeftTop point of parent window 
// or rect specified in COXPopupBarCtrl::Pick function
#define ID_OXCALCULATOR_ALIGNTOP				0x00000002
// popup window window LeftTop will be displayed at the LeftBottom point of parent window 
// or rect specified in COXPopupBarCtrl::Pick function
#define ID_OXCALCULATOR_ALIGNBOTTOM				0x00000003
// popup window window RightBottom will be displayed at the LeftBottom point of parent window 
// or rect specified in COXPopupBarCtrl::Pick function
#define ID_OXCALCULATOR_ALIGNLEFTBOTTOM			0x00000004		
// popup window window LeftBottom will be displayed at the RightBottom point of parent window 
// or rect specified in COXPopupBarCtrl::Pick function
#define ID_OXCALCULATOR_ALIGNRIGHTBOTTOM		0x00000005		
// popup window window RightBottom will be displayed at the RightTop point of parent window 
// or rect specified in COXPopupBarCtrl::Pick function
#define ID_OXCALCULATOR_ALIGNTOPRIGHT			0x00000006
// popup window window RightTop will be displayed at the RightBottom point of parent window 
// or rect specified in COXPopupBarCtrl::Pick function
#define ID_OXCALCULATOR_ALIGNBOTTOMRIGHT		0x00000007


#define OXCALCULATORPOPUP_XMARGIN				5
#define OXCALCULATORPOPUP_YMARGIN				5
#define OXCALCULATORPOPUP_BUTTON_HEIGHT			24



class OX_CLASS_DECL COXCalculatorPopup : public CWnd
{
// Construction
public:
	COXCalculatorPopup();   
	virtual ~COXCalculatorPopup();

	BOOL Create(CWnd* pParentWnd=NULL, HBRUSH hbrBackground=NULL);

	BOOL Pick(double dStartValue=0, int nAlignment=ID_OXCALCULATOR_ALIGNBOTTOM, 
		CRect rectParent=CRect(0,0,0,0), CSize szOffset=CSize(0,0));

	inline double GetResult() const 
	{ 
		ASSERT(::IsWindow(m_calculator.GetSafeHwnd()));
		return m_calculator.GetResult();
	}

	inline CString GetResultAsString() const 
	{ 
		ASSERT(::IsWindow(m_calculator.GetSafeHwnd()));
		return m_calculator.GetResultAsString();
	}

	inline COXCalculatorCtrl* GetCalculatorCtrl() { return &m_calculator; }

	// helper for detecting whether child descendent of parent
	// (works with owned popups as well)
	static BOOL IsDescendant(CWnd* pWndParent, CWnd* pWndChild);

protected:
	BOOL CalcWindowSize();
	BOOL AdjustWindowPos(int nAlignment=ID_OXCALCULATOR_ALIGNBOTTOM, 
		CRect rectParent=CRect(0,0,0,0), CSize szOffset=CSize(0,0));

	// Generated message map functions
	//{{AFX_MSG(COXCalculatorPopup)
	afx_msg void OnOk();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	COXCalculatorCtrl m_calculator;
	COXBitmapButton m_btnOk;
	COXBitmapButton m_btnCancel;

	CWnd* m_pParentWnd;

	// m_nState = 0 - control is active
	// m_nState = 1 - user pressed 'OK'
	// m_nState = 2 - user left the window without choosing any number
	int m_nState;

	CRect m_rect;
	CRect m_rectCalculator;
	CRect m_rectBtnOk;
	CRect m_rectBtnCancel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_OXCALCULATORCTRL_H__)
