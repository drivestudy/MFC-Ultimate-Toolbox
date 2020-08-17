#pragma once

#include "oxdllext.h"

class COXHyperBarOverlay;
class COXHyperBarRender;
// COXHyperBar

#pragma warning (push, 3)
#include <vector>
#pragma warning (pop)

// This class is the core of the Hyperbar functionality.  It is derived from CToolBar, which means it can easily be added to any existing project, and all
// toolbar behaviour works.  
class OX_CLASS_DECL COXHyperBar : public CToolBar
{
	friend class COXHyperBarOverlay;
	friend class COXHyperBarRender;

	DECLARE_DYNAMIC(COXHyperBar)

public:
	COXHyperBar();
	virtual ~COXHyperBar();
	static int OffsetY;
	void SetBackColor(COLORREF col);
private:
	COXHyperBarOverlay * m_wndInvisible;  // The overlay window we use to draw the toolbar
	COXHyperBarRender *  m_pRender;       // This pointer is also passed to the overlay, so both can use this class to perform all rendering


	void RepositionInvisibleWindow();
	bool m_bHasRedrawn;                // This flag is needed so that when we have two toolbars and move between them, 
									   // the first has been redrawn unselected, before we grab the screenshot to draw the second
	static std::vector<COXHyperBar*> m_AllBars; // We keep a static list of all bars, so we can check m_bHasRedrawn across them all
	static bool HasVisibleOverlay();          // This is the method which iterates over m_AllBars, looking for m_bHasRedrawn to be true.

	// This is private as you should only override the one that takes a RECT, this one just calls that one
	void RenderBackground(CDC * pDC, int left, int top, int width, int height);

protected:
	//{{AFX_MSG(COXHyperBar)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	// This method is provided/extracted so that it can be overridden, if the user wants to create a different sort of toolbar background
	// It's in this class, as this is the class the user can override, that is, if it was in the renderer class, the user would still have
	// to override this class, to create an instance of the new render class, which would be messy and superfluous.
	virtual void RenderBackground(CDC * pDC, RECT * pRect);
	COLORREF m_backColor;
};

