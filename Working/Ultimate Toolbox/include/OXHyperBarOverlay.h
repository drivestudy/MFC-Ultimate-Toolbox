#pragma once
#include "oxdllext.h"

class COXHyperBarRender;

// COXHyperBarOverlay

// This class represents the window that we draw over the top of the toolbar, using a screenshot as a background, 
// and thus allowing us to render buttons that appear to be larger than the toolbar which should contain them.
// The truth is, we're rendering the toolbar on a floating window, and drawing the buttons within the confines of that window.

class OX_CLASS_DECL COXHyperBarOverlay : public CWnd
{
	DECLARE_DYNAMIC(COXHyperBarOverlay)

public:
	COXHyperBarOverlay(CToolBar * pParent, COXHyperBarRender * pRender, SIZE szBtn);
	virtual ~COXHyperBarOverlay();

	void SetBitmap(CBitmap * pBmp);
	virtual BOOL ShowWindow(int nCmdShow);

protected:
	//{{AFX_MSG(COXHyperBar)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	COXHyperBarOverlay();
	CToolBar * m_pParent;
	CBitmap * m_pOverlay;
	COXHyperBarRender * m_pRender;
	SIZE m_szBtn;
};


