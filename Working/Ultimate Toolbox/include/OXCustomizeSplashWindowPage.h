// ==========================================================================
//			Class Specification: COXCustomizeSplashWindowPage
// ==========================================================================

// Header file : OXCustomizeSplashWindowPage.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////

/*
	Overview

COXCustomizeSplashWindowPage is COXCustomizePage-derived class. It implements one of the 
predefined (standard) customize pages that is responsible for customizing of the 
appearance of splash windows. 

This standard page will be automatically inserted in the customize manager if you
add the following define to your stdafx.h file:

	#define OX_CUSTOMIZE_SPLASHWINDOW

In order to initialize the page you must call the following function:

InitializeSplashWindow(UINT nBitmapResourceID, BOOL bShowSplashWnd=TRUE,
					   int nDisplayFor=-1, BOOL bUserCanCancel=FALSE,
					   COLORREF clrTransparent=CLR_DEFAULT, 
					   LPPOINT pColorPoint=NULL, BYTE nTolerance=0/, 
					   LPCTSTR lpszProfileName=_T("CustomizeSplashWindow"))

This class implements the set of virtual functions defined in the COXCustomizePage 
class that must be implemented by any page.

The best way of learning about Customize manager capabilities is to take look at the 
VisualStudioLikeDemo sample that can be found in the 
.\Samples\Advanced\VisualStudioLikeDemo subdirectory of your Ultimate Toolbox 
directory. In the sample menu choose "View"-"Customize" in order to display the 
Customize manager window. 



Dependency:
	#include "OXCustomizeSplashWindowPage.h"


Source code files:
	"OXCustomizeSplashWindowPage.cpp"		
	"OXCustomizePage.cpp"		
	"OXSeparator.cpp"		
	"OXBMPFilePickerCombo.cpp"
	"OXColorPickerButton.cpp"
	"OXSplashWnd.cpp"
	

Resource files:
	"OXHistoryCombo.rc"		
	"OXPreviewDialog.rc"		


*/
#if !defined(_OXCUSTOMIZESPLASHWINDOWPAGE_H__)
#define _OXCUSTOMIZESPLASHWINDOWPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"

#include "OXCustomizePage.h"
#include "OXSeparator.h"
#include "OXBMPFilePickerCombo.h"
#include "OXColorPickerButton.h"
#include "OXSplashWnd.h"
#include "OXMainRes.h"


/////////////////////////////////////////////////////////////////////////////
// COXCustomizeSplashWindowPage dialog

class OX_CLASS_DECL COXCustomizeSplashWindowPage : public COXCustomizePage
{
	DECLARE_DYNCREATE(COXCustomizeSplashWindowPage)
// Construction
public:
	COXCustomizeSplashWindowPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(COXCustomizeSplashWindowPage)
	enum { IDD = IDD_OX_PAGE_SPLASHWINDOW };
	CSpinButtonCtrl	m_spinTolerance;
	COXBMPFilePickerCombo	m_comboSplashImageFileName;
	CSpinButtonCtrl	m_spinShowFor;
	COXSeparator	m_sepTransparentColor;
	COXSeparator	m_sepPreview;
	COXSeparator	m_sepOpenImage;
	COXSeparator	m_sepDisplaySettings;
	COXColorPickerButton	m_btnTransparentColor;
	BOOL	m_bInterruptable;
	BOOL	m_bDisplaySplashWindow;
	UINT	m_nShowDelay;
	UINT	m_nXCoord;
	UINT	m_nYCoord;
	int		m_nTransparentColorType;
	int		m_nImageSourceType;
	int		m_nDelayType;
	CString	m_sSplashImageFileName;
	UINT	m_nTolerance;
	//}}AFX_DATA
	COLORREF m_clrTransparent;


protected:
	UINT m_nDefaultBitmapID;
	BOOL m_bUseDefaultImage;
	BOOL m_bShowInfinitely;

	COXSplashWnd m_splashWindow;
	COXSplashWnd m_splashWindowTest;


public:
	virtual BOOL Load(const COXCustomizeManager* pCustomizeManager);
	virtual void Unload();
	virtual BOOL IsSupportingApplyChanges() { return FALSE; }
	virtual BOOL ApplyChanges();

	virtual CString GetTitle() const {
		CString sRet;
		VERIFY(sRet.LoadString(IDS_OX_CSTMZESPLASHTITLE));
		return sRet;
		//_T("Splash Window"); 
	}
	virtual LPCTSTR GetImageResource() const { 
		return MAKEINTRESOURCE(IDB_OX_CUSTOMIZE_SPLASHWINDOW); 
	}
	virtual CString GetTooltip() const {
		CString sRet;
		VERIFY(sRet.LoadString(IDS_OX_CSTMZESPLASHTOOLTIP));
		return sRet;
		//_T("Tooltip for Splash Window"); 
	}

	virtual BOOL OnCloseManager(BOOL bIsOk);

	virtual BOOL InitializeSplashWindow(UINT nBitmapResourceID, 
		BOOL bShowSplashWnd=TRUE, int nDisplayFor=-1, 
		BOOL bUserCanCancel=FALSE, COLORREF clrTransparent=CLR_DEFAULT, 
		LPPOINT pColorPoint=NULL, BYTE nTolerance=0, 
		LPCTSTR lpszProfileName=_T("CustomizeSplashWindow"));

	inline COXSplashWnd* GetSplashWindow() { return &m_splashWindow; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXCustomizeSplashWindowPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXCustomizeSplashWindowPage();   // destructor

protected:
	virtual void OnInitDialog();

	void ShowVars();
	void ShowSplashWindow(COXSplashWnd* pSplashWindow, BOOL bForceUserCancel=FALSE);

	// Generated message map functions
	//{{AFX_MSG(COXCustomizeSplashWindowPage)
	afx_msg void OnButtonPreview();
	afx_msg void OnRadioColor();
	afx_msg void OnRadioColorNone();
	afx_msg void OnRadioColoratpoint();
	afx_msg void OnRadioImageSourceDefault();
	afx_msg void OnRadioImageSourceFile();
	afx_msg void OnRadioShowFor();
	afx_msg void OnRadioShowInfinitely();
	//}}AFX_MSG
	afx_msg LRESULT OnKickIdle(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_OXCUSTOMIZESPLASHWINDOWPAGE_H__)
