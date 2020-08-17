
// Version: 9.3

#if !defined(_CUSTOMIZECAPTIONPAGE_H__)
#define _CUSTOMIZECAPTIONPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"

#include "OXCustomizePage.h"
#include "OXSeparator.h"
#include "OXColorPickerButton.h"
#include "OXFontPickerButton.h"
#include "OXCaptionPainter.h"
#include "OXMainRes.h"

/////////////////////////////////////////////////////////////////////////////
// COXCustomizeCaptionPage dialog

class OX_CLASS_DECL COXCustomizeCaptionPage : public COXCustomizePage
{
	DECLARE_DYNCREATE(COXCustomizeCaptionPage)
// Construction
public:
	COXCustomizeCaptionPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(COXCustomizeCaptionPage)
	enum { IDD = IDD_OX_PAGE_CAPTION_PAINTER };
	COXFontPickerButton	m_btnSmallTextFont;
	COXStatic	m_preview;
	CEdit	m_ctlEditNumberShade;
	CComboBox	m_ctlComboTextEllipsis;
	CComboBox	m_ctlComboTextAlignment;
	CComboBox	m_ctlComboGradientAlgorithm;
	CComboBox	m_ctlComboGradientAlignment;
	CComboBox	m_ctlComboCaptionState;
	CSpinButtonCtrl	m_spinNumberShade;
	COXSeparator	m_separator;
	COXFontPickerButton	m_btnTextFont;
	COXColorPickerButton	m_btnBackColor;
	BOOL	m_bGradient;
	int		m_nCaptionState;
	int		m_nGradientAlgorithm;
	int		m_nGradientAlignment;
	int		m_nTextAlignment;
	int		m_nTextEllipsis;
	UINT	m_nNumberShade;
	BOOL	m_bPaintCaption;
	//}}AFX_DATA
	COLORREF m_clrBackground;
	COLORREF m_clrText;
	COLORREF m_clrTextSmall;
	CFont m_fontSmall;
	CFont m_font;

protected:
	BOOL m_bOnlyMainWindow;
	COXCaptionInfo m_ActiveInfo;
	COXCaptionInfo m_PassiveInfo;

	COXCaptionPainterOrganizer m_captionPainterOrganizer;

public:
	virtual BOOL Load(const COXCustomizeManager* pCustomizeManager);
	virtual void Unload();
	virtual BOOL IsSupportingApplyChanges() { return FALSE; }

	virtual CString GetTitle() const {
		CString sRet;
		VERIFY(sRet.LoadString(IDS_OX_CSTMZECAPTIONTITLE));
		return sRet;
		/*_T("Caption Painter"); */}
	virtual LPCTSTR GetImageResource() const { 
		return MAKEINTRESOURCE(IDB_OX_CUSTOMIZE_CAPTION_PAINTER); 
	}
	virtual CString GetTooltip() const {
		 CString sRet;
		VERIFY(sRet.LoadString(IDS_OX_CSTMZECAPTIONTOOLTIP));
		return sRet;/*_T("Customize Caption Painter"); */
	}

	virtual BOOL OnCloseManager(BOOL bIsOk);

	virtual BOOL InitializeCaptionPainter(BOOL bPaintCaption=TRUE,
		LPCTSTR lpszProfileName=_T("CustomizeCaptionPainter"), 
		BOOL bOnlyMainWindow=FALSE);

	inline COXCaptionPainterOrganizer* GetPainterOrganizer() {
		return &m_captionPainterOrganizer;
	}

protected:
	void GetVars(BOOL bLoad=TRUE);
	void SetVars();
	void ShowControls();

	void SetupCaptionPainting();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXCustomizeCaptionPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXCustomizeCaptionPage();   // destructor

protected:
	virtual void OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(COXCustomizeCaptionPage)
	afx_msg void OnButtonBackcolor();
	afx_msg void OnButtonTextfont();
	afx_msg void OnCheckGradient();
	afx_msg void OnSelchangeComboCaptionState();
	afx_msg void OnSelchangeComboGradientAlgorithm();
	afx_msg void OnSelchangeComboGradientAlignment();
	afx_msg void OnSelchangeComboTextAlignment();
	afx_msg void OnSelchangeComboTextEllipsis();
	afx_msg void OnChangeEditNumshades();
	afx_msg void OnButtonSmalltextfont();
	afx_msg void OnCheckPaintCaption();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(_CUSTOMIZECAPTIONPAGE_H__)
