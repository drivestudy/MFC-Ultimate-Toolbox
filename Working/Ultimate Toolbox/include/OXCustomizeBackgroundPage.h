// Version: 9.3

#if !defined(_OXCUSTOMIZEBACKGROUNDPAGE_H__)
#define _OXCUSTOMIZEBACKGROUNDPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"

#include "OXCustomizePage.h"
#include "OXSeparator.h"
#include "OXColorPickerButton.h"
#include "OXBackgroundPainter.h"
#include "OXBMPFilePickerCombo.h"
#include "OXMainRes.h"


/////////////////////////////////////////////////////////////////////////////
// COXCustomizeBackgroundPage dialog

class OX_CLASS_DECL COXCustomizeBackgroundPage : public COXCustomizePage
{
	DECLARE_DYNCREATE(COXCustomizeBackgroundPage)
// Construction
public:
	COXCustomizeBackgroundPage();

// Dialog Data
	//{{AFX_DATA(COXCustomizeBackgroundPage)
	enum { IDD = IDD_OX_PAGE_BACKGROUND_PAINTER };
	COXSeparator	m_ctlSeparator2;
	COXSeparator	m_ctlSeparator1;
	COXBMPFilePickerCombo	m_cmbFilename;
	COXColorPickerButton	m_btnBackColor;
	int		m_nAlign;
	CString	m_sFilename;
	BOOL	m_bOnlyClientArea;
	BOOL	m_bPaintBackground;
	int		m_nImageSourceType;
	//}}AFX_DATA
	COLORREF m_clrBack;


protected:
	COXBackgroundPainterOrganizer m_backgroundPainterOrganizer;

	BOOL m_bUseDefaultImage;
	COXDIB m_dibDefault;

	// preview window
	CWnd m_pictureWnd;


public:
	virtual BOOL Load(const COXCustomizeManager* pCustomizeManager);
	virtual void Unload();
	virtual BOOL IsSupportingApplyChanges() { return TRUE; }
	virtual BOOL ApplyChanges();

	virtual CString GetTitle() const { 
		CString sRet;
		VERIFY(sRet.LoadString(IDS_OX_CSTMZEBCKGRNDTITLE));
		return sRet;/*_T("Background Painter"); */}
	virtual LPCTSTR GetImageResource() const { 
		return MAKEINTRESOURCE(IDB_OX_CUSTOMIZE_BACKGROUND_PAINTER); 
	}
	virtual CString GetTooltip() const {
				CString sRet;
		VERIFY(sRet.LoadString(IDS_OX_CSTMZEBCKGRNDTOOLTIP));
		return sRet;
		/*_T("Customize Background Painter"); */}

	virtual BOOL OnCloseManager(BOOL bIsOk);

	BOOL InitializeBackgroundPainter(const BOOL bPaintBackground,
		const BOOL bOnlyClientArea,	LPCTSTR lpszDIBResourceName, 
		const PaintType paintType=Tile, const COLORREF clrBk=CLR_DEFAULT, 
		LPCTSTR lpszProfileName=_T("CustomizeBackgroundPainter"));

	BOOL InitializeBackgroundPainter(const BOOL bPaintBackground,
		const BOOL bOnlyClientArea,	const UINT nDIBResourceID, 
		const PaintType paintType=Tile, const COLORREF clrBk=CLR_DEFAULT, 
		LPCTSTR lpszProfileName=_T("CustomizeBackgroundPainter"));

	BOOL InitializeBackgroundPainter(const BOOL bPaintBackground,
		const BOOL bOnlyClientArea, const CString& sDIBFileName, 
		const PaintType paintType=Tile, const COLORREF clrBk=CLR_DEFAULT, 
		LPCTSTR lpszProfileName=_T("CustomizeBackgroundPainter"));

	virtual BOOL InitializeBackgroundPainter(const BOOL bPaintBackground,
		const BOOL bOnlyClientArea,	const COXDIB* pDIB, 
		const PaintType paintType=Tile, const COLORREF clrBk=CLR_DEFAULT, 
		LPCTSTR lpszProfileName=_T("CustomizeBackgroundPainter"));

	inline COXBackgroundPainterOrganizer* GetBackgroundOrganizer() {
		return &m_backgroundPainterOrganizer;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXCustomizeBackgroundPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXCustomizeBackgroundPage();   // destructor

protected:
	virtual void OnInitDialog();

	void SetupBackgroundPainting();

	// Generated message map functions
	//{{AFX_MSG(COXCustomizeBackgroundPage)
	afx_msg void OnAlign();
	afx_msg void OnBackColor();
	afx_msg void OnEditchangeFilename();
	afx_msg void OnSelchangeFilename();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnRadioBackgroundImageSourceDefault();
	afx_msg void OnRadioBackgroundImageSourceFilename();
	afx_msg void OnEnable();
	//}}AFX_MSG
	afx_msg LRESULT OnKickIdle(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	COXHelperWnd* m_pHelpWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_OXCUSTOMIZEBACKGROUNDPAGE_H__)
