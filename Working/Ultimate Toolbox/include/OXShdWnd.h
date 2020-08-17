// Version: 9.3
#if !defined(AFX_OXSHDWND_H__996C0646_ADAF_4130_AB5D_BBEAF6621CCC__INCLUDED_)
#define AFX_OXSHDWND_H__996C0646_ADAF_4130_AB5D_BBEAF6621CCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OXShdWnd.h : header file
//

#include <afxtempl.h>
#include "OXDllExt.h"

/////////////////////////////////////////////////////////////////////////////
// COXShdWnd window

typedef CArray<COLORREF, COLORREF> OXSHADOWARRAY;
typedef CMap<DWORD, DWORD, COLORREF, COLORREF> OXPIXELMAP;

class OX_CLASS_DECL COXShdWnd : public CWnd
{
// Construction
public:
	COXShdWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXShdWnd)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetCastingWindow(CWnd* pWnd);
	virtual ~COXShdWnd();

	// Generated message map functions
protected:
	CWnd* m_pCastingWindow; // pointer to the window casting the shadow
	//{{AFX_MSG(COXShdWnd)
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void AddPixelToMap(int x, int y, COLORREF clr);
	void DrawShadow(HDC hDC, LPRECT lpRect, OXSHADOWARRAY* pShadowArray);
	COLORREF DarkenColor(long lScale, COLORREF lColor);
	COLORREF GetShadowPixel(HDC hDC, int iXPos, int iYPos, bool bUseSavedShadow);
	void SetShadowPixel(HDC hDC, int iXPos, int iYPos, COLORREF clr, bool bSaveShadow, bool bUseSavedShadow, OXSHADOWARRAY* pShadowArray, int& iIndex);
	OXPIXELMAP m_mapPixels;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OXSHDWND_H__996C0646_ADAF_4130_AB5D_BBEAF6621CCC__INCLUDED_)
