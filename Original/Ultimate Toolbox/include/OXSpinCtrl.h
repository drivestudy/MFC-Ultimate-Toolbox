// ==========================================================================
// 					Class Specification : COXSpinCtrl
// ==========================================================================

// Header file : OXSpinCtrl.h

// Version: 9.3

// Properties :
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CSpinButtonCtrl

//	YES	Is a CWnd.
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	YES	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Description :
//	The class COXSpinCtrl is a class that extends the MFC class CSpinButtonCtrl.
//	It extends the functionality of the common control (spin) that is being wrapped by MFC.
//	Apart from the two buttons (up and down) it shows a small rectangle (spin thumb) in
//	between these two buttons. When the user clicks and holds this rectangle, the mouse cursor
//	will change shape, and he can drag the mouse to increase or decrease the value. Up and down
//	buttons have the same effect as the standard control. Extended functionality can be
//	switched off (even at run-time), so COXSpinCtrl can be used as the standard spin common
//	control. Functionality of CSpinButtonCtrl is completely supported by COXSpinCtrl.

// Example :
//	COXSpinCtrl m_SpinEx;
//	...
//	m_SpinEx.Create(WS_VISIBLE | UDS_HORZ | UDS_SETBUDDYINT, rectSpin, this, IDC_SPINEX);
//	m_SpinEx.SetBuddy(&m_Buddy);
//	m_SpinEx.SetRange(0, 100);
//	m_SpinEx.SetPos(50);

// Remarks :
//	1.	Double click on the thumb swaps the spin value between the outer limits.
//	2.	When user clicks the thumb and drags the mouse, change of mouse coordinates
//		is translated to spin value change. COXSpinCtrl supports two methods of such
//		translation:
//			a)	one screen pixel corresponds to one spin unit;
//			b)	screen borders are considered as spin value limits, so one spin
//				unit can be correlated with various number of pixels.
//		These methods are:
//			a)	represented by special constants (see below);
//			b)	carried out by virtual function ComputeValueFromDeltaPixel().
//		So user can define his/her own constants and override
//		ComputeValueFromDeltaPixel() to realize his/her own translation methods.

// Prerequisites (necessary conditions):
// Cursor files SpinHor.cur (ID = IDC_OXSPINHORCUR) and SpinVer.cur (ID = IDC_OXSPINVERCUR).
// Do not forget to include OXSpinCtrl.rc in your project

// The following resource ID ranges are reserved for use by this class
// 23930 -> 23949 and 53930 -> 53949

/////////////////////////////////////////////////////////////////////////////

#ifndef	__OXSPINCTRL_H__
#define	__OXSPINCTRL_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


// //////////////////////////////////////////////////////////////////////////
// Definition of constants for computation method representation.
#define OX_SPIN_DELTA_PIXEL_IS_DELTA_VALUE	0	// one screen pixel = one spin unit
#define OX_SPIN_SCREEN_AREA					1	// screen borders <=> spin value limits

// //////////////////////////////////////////////////////////////////////////

class OX_CLASS_DECL COXSpinCtrl : public CSpinButtonCtrl
{
// Data members -------------------------------------------------------------
public:

protected:
	BOOL	m_bEnable;
	BOOL	m_bHoriz;
	BYTE	m_ucWasPressed;
	int		m_nComputationMethod;
	int		m_nSpinWidth;
	int		m_nSpinHeight;
	int		m_nThumbTop;
	int		m_nThumbBottom;
	int		m_nOriginalPixel;
	int		m_nMaxPixel;
	int		m_nOriginalValue;
	int		m_nMinValue;
	int		m_nMaxValue;
	DWORD	m_dwClickTicks;
	HCURSOR	m_hThumbHorCursor;
	HCURSOR	m_hThumbVerCursor;
	HCURSOR	m_hThumbDefCursor;

private:

// Member functions ---------------------------------------------------------
public:
	COXSpinCtrl(BOOL bEnable = TRUE);
	// --- In :			bEnable :	Flag that indicates whether the spin thumb is initially
	//								enabled (TRUE) or disabled (FALSE).
	// --- Out :
	// --- Returns :
	// --- Effect :		Constructs the COXSpinCtrl object and set the spin thumb enabled or disabled.
	//					By default the thumb is enabled, but it can be disabled later.

	virtual ~COXSpinCtrl();
	// --- In :
	// --- Out :
	// --- Returns :
	// --- Effect :		Destroys the COXSpinCtrl object.

	inline BOOL IsThumbEnabled() const { return m_bEnable; }
	// --- In :
	// --- Out :
	// --- Returns :	TRUE - if the spin thumb is enabled, FALSE - otherwise.
	// --- Effect :		This inline function returns whether extended functionality of
	//					spin control - spin thumb - is currently enabled.

	void EnableThumb(BOOL bEnable = TRUE);
	// --- In :			bEnable :	Flag to enable (TRUE) or disable (FALSE) the spin thumb.
	// --- Out :
	// --- Returns :
	// --- Effect :		Enables (by default) or disables the spin thumb.

	inline int GetComputationMethod() const { return m_nComputationMethod; }
	// --- In :
	// --- Out :
	// --- Returns :	Current computation method constant.
	// --- Effect :		Retrieves the current spin value change computation method
	//					(initially OX_SPIN_DELTA_PIXEL_IS_DELTA_VALUE).

	inline void SetComputationMethod(int nComputationMethod) { m_nComputationMethod = nComputationMethod; }
	// --- In :			Valid computation method constant.
	// --- Out :
	// --- Returns :
	// --- Effect :		Sets the current spin value change computation method.

	virtual int ComputeValueFromDeltaPixel(int nDeltaPixel) const;
	// --- In :			Pixel change (the difference between initial and
	//					terminal points of mouse moving to be translated).
	// --- Out :
	// --- Returns :	Spin value change, that corresponds to specified pixel change.
	// --- Effect :		Translates specified pixel change to the spin value change,
	//					according to current computation method. Initially COXSpinCtrl
	//					supports two such methods, but this function can be overridden
	//					to support user-defined value change computation methods.

protected:
	void GetSizeAndOrientation();
	void ThumbUpDownDefine();
	void ThumbDraw(CPaintDC* pdc, DWORD dwStateThumb);
	void ArrowsDraw(CPaintDC* pdc, DWORD dwStateUp, DWORD dwStateDown);
	void ArrowTriangle(CPaintDC* pdc, int nWid, int nHei, int nShift, int nUL);
	BOOL IsDoubleClick();

	//{{AFX_MSG(COXSpinCtrl)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	//{{AFX_VIRTUAL(COXSpinCtrl)
	//}}AFX_VIRTUAL

	DECLARE_MESSAGE_MAP()

private:

};

#endif	// __OXSPINCTRL_H__
