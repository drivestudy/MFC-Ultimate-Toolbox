
// Version: 9.3

#if !defined(_OXCUSTOMIZEINTELLIMOUSEPAGE_H__)
#define _OXCUSTOMIZEINTELLIMOUSEPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXDllExt.h"

#include "OXCustomizePage.h"
#include "OXSeparator.h"
#include "OXMultiComboBox.h"
#include "OXIntelliMouse.h"


class COXCustomizeIntelliMousePage;

class OX_CLASS_DECL COXCustomizeIntelliMouseOrganizer : public COXIntelliMouseOrganizer
{
public:
	COXCustomizeIntelliMouseOrganizer() : m_pCustomizeIntelliMousePage(NULL) {};

public:
	COXCustomizeIntelliMousePage* m_pCustomizeIntelliMousePage;

protected:
	virtual BOOL IsQualified(HWND hWnd, LPCTSTR lpszWndClass);
};


/////////////////////////////////////////////////////////////////////////////
// COXCustomizeIntelliMousePage dialog

class OX_CLASS_DECL COXCustomizeIntelliMousePage : public COXCustomizePage
{
	DECLARE_DYNCREATE(COXCustomizeIntelliMousePage)
// Construction
public:
	COXCustomizeIntelliMousePage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(COXCustomizeIntelliMousePage)
	enum { IDD = IDD_OX_PAGE_INTELLIMOUSE };
	COXSeparator	m_sepStartEvent;
	COXSeparator	m_sepKeyPressed;
	COXMultiComboBox	m_comboStartEvent;
	BOOL	m_bAltKey;
	BOOL	m_bCtrlKey;
	BOOL	m_bOnlyViews;
	BOOL	m_bShiftKey;
	BOOL	m_bSupportIntelliMouse;
	int		m_nStartEvent;
	//}}AFX_DATA


protected:
	COXCustomizeIntelliMouseOrganizer m_intelliMouseOrganizer;

	CArray<UINT,UINT> m_arrStartEvents;
	CMap<UINT,UINT,LPCTSTR*,LPCTSTR*> m_mapStartEventDescriptions;

public:
	virtual BOOL Load(const COXCustomizeManager* pCustomizeManager);
	virtual void Unload();
	virtual BOOL IsSupportingApplyChanges() { return TRUE; }
	virtual BOOL ApplyChanges();

	virtual CString GetTitle() const {
		CString sRet;
		VERIFY(sRet.LoadString(IDS_OX_CSTMZEINTELLIMOUSETITLE));
		return sRet;
		//_T("IntelliMouse Support"); 
	}
	virtual LPCTSTR GetImageResource() const { 
		return MAKEINTRESOURCE(IDB_OX_CUSTOMIZE_INSTANCE_MANAGER); 
	}
	virtual CString GetTooltip() const {
		CString sRet;
		VERIFY(sRet.LoadString(IDS_OX_CSTMZEINTELLIMOUSETOOLTIP));
		return sRet;
	//_T("Tooltip for IntelliMouse Support"); 
	}

	virtual BOOL OnCloseManager(BOOL bIsOk);

	virtual BOOL InitializeIntelliMouse(BOOL bSupportIntelliMouse=TRUE, 
		UINT nStartEventID=WM_MBUTTONDOWN, DWORD dwKeyState=0, BOOL bOnlyViews=TRUE,
		LPCTSTR lpszProfileName=_T("CustomizeIntelliMouse"));

	inline COXIntelliMouseOrganizer* GetIntelliMouseOrganizer() { 
		return &m_intelliMouseOrganizer; 
	}

	virtual BOOL IsQualifiedForIntelliMouseSupport(HWND hWnd, LPCTSTR lpszWndClass);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXCustomizeIntelliMousePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COXCustomizeIntelliMousePage();   // destructor

protected:
	virtual void OnInitDialog();

	void SetupIntelliMouseSupport();

	// Generated message map functions
	//{{AFX_MSG(COXCustomizeIntelliMousePage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_OXCUSTOMIZEINTELLIMOUSEPAGE_H__)
