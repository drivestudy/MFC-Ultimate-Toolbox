// Version: 9.3
#if !defined(_OXBUTTONAPPEARANCEDLG_H__)
#define _OXBUTTONAPPEARANCEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#define __AFXTEMPL_H__
#endif


#include "OXDllExt.h"

#include "OXSeparator.h"
#include "OXImageListBox.h"


/////////////////////////////////////////////////////////////////////////////
// COXButtonAppearanceDlg dialog

class OX_CLASS_DECL COXButtonAppearanceDlg : public CDialog
{
// Construction
public:
	COXButtonAppearanceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COXButtonAppearanceDlg)
	enum { IDD = IDD_OX_BUTTONAPPEARANCE_DIALOG };
	COXSeparator	m_sepText;
	COXSeparator	m_sepImages;
	COXSeparator	m_sepSizes;
	COXSeparator	m_sepDisplaySettings;
	COXImageListBox	m_listCustomImages;
	CString			m_sText;
	int				m_nCustomImageIndex;
	int				m_nButtonType;
	BOOL			m_bDropDown;
	CComboBox		m_comboShortkeys;
	int				m_nShortkeyIndex;
	BOOL			m_bDisplayHorzDock;
	BOOL			m_bDisplayVertDock;
	BOOL			m_bDisplayFloat;
	int				m_nHeight;
	int				m_nWidth;
	CSpinButtonCtrl	m_spinHeight;
	CSpinButtonCtrl	m_spinWidth;
	//}}AFX_DATA

	CArray<CString,LPCTSTR> m_arrShortkeyEntries;

protected:
	CImageList* m_pImageList;

	// flag that specifies that image settings cannot be changed 
	BOOL m_bOnlyText;
	// flag that specifies that style settings cannot be changed 
	BOOL m_bStyleReadOnly;
	// flag that specifies that shortkey text cannot be changed 
	BOOL m_bShortkeyReadOnly;
	// flag that specifies that the edited button is a custom one
	BOOL m_bCustomButton;

public:
	inline void SetImageList(CImageList* pImageList)
	{
		if(m_pImageList!=pImageList)
		{
			m_pImageList=pImageList;
			if(::IsWindow(m_listCustomImages.GetSafeHwnd()))
				m_listCustomImages.SetImageList(m_pImageList);
		}
	}

	inline CImageList* GetImageList() const { return m_pImageList; }

	inline void SetModifyTextOnly(BOOL bOnlyText) { m_bOnlyText=bOnlyText; }
	inline BOOL GetModifyTextOnly() const { return m_bOnlyText; }

	inline void SetStyleReadOnly(BOOL bStyleReadOnly) { m_bStyleReadOnly=bStyleReadOnly; }
	inline BOOL GetStyleReadOnly() const { return m_bStyleReadOnly; }

	inline void SetShortkeyReadOnly(BOOL bShortkeyReadOnly) { m_bShortkeyReadOnly=bShortkeyReadOnly; }
	inline BOOL GetShortkeyReadOnly() const { return m_bShortkeyReadOnly; }
	inline CString GetShortkeyText() const 
	{ 
		return (m_nShortkeyIndex<=0 ? _T("") : m_arrShortkeyEntries[m_nShortkeyIndex]);
	}

	inline void SetCustomButton(BOOL bCustomButton) { m_bCustomButton=bCustomButton; }
	inline BOOL IsCustomButton() const { return m_bCustomButton; }


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXButtonAppearanceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ShowControls();

	// Generated message map functions
	//{{AFX_MSG(COXButtonAppearanceDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadioImageOnly();
	afx_msg void OnRadioImageAndText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_OXBUTTONAPPEARANCEDLG_H__)
