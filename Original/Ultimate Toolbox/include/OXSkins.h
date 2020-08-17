// COXDockbarSkin.h
//
// Version: 9.3


#include "OXDllExt.h"
#include "OXShortcutbar.h"

//Added by Nish to detect if themes are enabled for the running application.
//Generic function that also works on pre-XP OSes and pre-VC7.1 compilers.
BOOL IsThemed();

// Forward declarations
class COXCoolToolBar;
class COXSizeControlBar;

struct OXCONTROLBARCONSTANTS
{
	int iButtonGap;
};

// Added by Christian 23/06/2006. 
// This will require some maintenance if we ever add any skins, but
// it's a lot cleaner than refering to skins by passing strings around
// The string code will remain for backwards compatibility.
// The 'None' type is for use in the base class.
enum OXAppSkins { OXSkinClassic, OXSkinOfficeXP, OXSkinOffice2003, OXSkinNone };


#if !defined(COXDockbarSkin_h)
#define COXDockbarSkin_h

class COXSplitterRect;
class COXSizeDockBar;

class OX_CLASS_DECL COXDockbarSkin
{
public:
	virtual ~COXDockbarSkin() { };
    virtual void DrawGripper(CDC* pDC, COXSizeControlBar* pSizeControlBar) = 0;
	virtual void DrawCloseButton(CDC* pDC, COXSizeControlBar* pSizeControlBar) = 0;
	virtual void DrawResizeButton(CDC* pDC, COXSizeControlBar* pSizeControlBar) = 0;
	virtual OXCONTROLBARCONSTANTS& GetControlBarConstants() = 0;
    virtual void DrawNonClientArea(CDC* pDC, LPCRECT lpRect, COXSizeControlBar* pSizeControlBar) = 0;
	virtual void DrawSplitter(CDC* pDC, COXSplitterRect* pSplitterRect, COXSizeDockBar* pSizeDockBar) = 0;
	virtual void OnNcPaintSizeDockBar(COXSizeDockBar* pSizeDockBar) = 0;
	virtual void DrawBackground(CDC* pDC, COXSizeDockBar* pSizeDockBar) = 0;
	virtual void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp, COXSizeControlBar* pSizeControlBar) = 0;
	virtual void RecalcLayout(COXSizeControlBar* pSizeControlBar) = 0;
	virtual COLORREF GetClientBorderColor() = 0;

protected:


private:


};

#endif /* COXDockbarSkin_h */
// COXDockbarSkinClassic.h
//

#if !defined(COXDockbarSkinClassic_h)
#define COXDockbarSkinClassic_h

#include "OXSizeCtrlBar.h"
#include "OXSizeDockBar.h"

class OX_CLASS_DECL COXDockbarSkinClassic : public COXDockbarSkin
{
public:
	COXDockbarSkinClassic();
	virtual ~COXDockbarSkinClassic();

    virtual void DrawGripper(CDC* pDC, COXSizeControlBar* pSizeControlBar);
    virtual void DrawCloseButton(CDC* pDC, COXSizeControlBar* pSizeControlBar);
    virtual void DrawResizeButton(CDC* pDC, COXSizeControlBar* pSizeControlBar);
	virtual OXCONTROLBARCONSTANTS& GetControlBarConstants() { return m_ControlBarConstants; };
	virtual void DrawNonClientArea(CDC* pDC, LPCRECT lpRect, COXSizeControlBar* pSizeControlBar);
	virtual void DrawSplitter(CDC* pDC, COXSplitterRect* pSplitterRect, COXSizeDockBar* pSizeDockBar);
	virtual void OnNcPaintSizeDockBar(COXSizeDockBar* pSizeDockBar) { pSizeDockBar->Default(); };
	virtual void DrawBackground(CDC* pDC, COXSizeDockBar* pSizeDockBar);
	virtual void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp, COXSizeControlBar* pSizeControlBar);
	virtual void RecalcLayout(COXSizeControlBar* pSizeControlBar);
	virtual COLORREF GetClientBorderColor();

protected:


private:
	CFont m_fontCaption;
	CFont m_fontCaptionSolid;
	OXCONTROLBARCONSTANTS m_ControlBarConstants;

};

#endif /* COXDockbarSkinClassic_h */

// COXDockbarSkinXP.h
//

#if !defined(COXDockbarSkinXP_h)
#define COXDockbarSkinXP_h


class COXSkinXP;

class OX_CLASS_DECL COXDockbarSkinXP : public COXDockbarSkin
{
public:
	COXDockbarSkinXP(COXSkinXP* pSkinXP);
	virtual ~COXDockbarSkinXP();

    virtual void DrawGripper(CDC* pDC, COXSizeControlBar* pSizeControlBar);
    virtual void DrawCloseButton(CDC* pDC, COXSizeControlBar* pSizeControlBar);
    virtual void DrawResizeButton(CDC* pDC, COXSizeControlBar* pSizeControlBar);
	virtual OXCONTROLBARCONSTANTS& GetControlBarConstants() { return m_ControlBarConstants; };
	virtual void DrawNonClientArea(CDC* pDC, LPCRECT lpRect, COXSizeControlBar* pSizeControlBar);
	virtual void DrawSplitter(CDC* pDC, COXSplitterRect* pSplitterRect, COXSizeDockBar* pSizeDockBar);
	virtual void OnNcPaintSizeDockBar(COXSizeDockBar* pSizeDockBar);
	virtual void DrawBackground(CDC* pDC, COXSizeDockBar* pSizeDockBar);
	virtual void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp, COXSizeControlBar* pSizeControlBar);
	virtual void RecalcLayout(COXSizeControlBar* pSizeControlBar);
	virtual COLORREF GetClientBorderColor();

protected:
	COXSkinXP* m_pSkinXP;
	CFont m_fontCaption;
	OXCONTROLBARCONSTANTS m_ControlBarConstants;
};

#endif /* COXDockbarSkinXP_h */

// COXDockbarSkin2003.h
//

#if !defined(COXDockbarSkin2003_h)
#define COXDockbarSkin2003_h

#define ID_O11_GRIPPER 13

class COXSkin2003;

class OX_CLASS_DECL COXDockbarSkin2003 : public COXDockbarSkinXP
{
public:
	COXDockbarSkin2003(COXSkin2003* pSkin2003);
	virtual ~COXDockbarSkin2003();

	virtual void DrawBackground(CDC* pDC, COXSizeDockBar* pSizeDockBar);
	virtual void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp, COXSizeControlBar* pSizeControlBar);
    virtual void DrawGripper(CDC* pDC, COXSizeControlBar* pSizeControlBar);
	virtual void DrawNonClientArea(CDC* pDC, LPCRECT lpRect, COXSizeControlBar* pSizeControlBar);
	virtual void DrawSplitter(CDC* pDC, COXSplitterRect* pSplitterRect, COXSizeDockBar* pSizeDockBar);
	virtual void RecalcLayout(COXSizeControlBar* pSizeControlBar);
    virtual void DrawCloseButton(CDC* pDC, COXSizeControlBar* pSizeControlBar);
    virtual void DrawResizeButton(CDC* pDC, COXSizeControlBar* pSizeControlBar);
	virtual void OnNcPaintSizeDockBar(COXSizeDockBar* pSizeDockBar);

protected:


private:

};

#endif /* COXDockbarSkin2003_h */

// COXTabSkin.h
//

#if !defined(COXTabSkin_h)
#define COXTabSkin_h

class COX3DTabViewContainer;
class COXSkinnedSpinCtrl;
class COXTabWorkspace;
class COXTabClientWnd;

class OX_CLASS_DECL COXTabSkin
{
public:
	virtual ~COXTabSkin() { };

	// Handle WM_PAINT for CTabCtrl derived classes
	virtual void OnPaintTabCtrl(CTabCtrl* pTabCtrl) = 0;

	// Handle WM_PAINT for COXSkinnedSpinCtrl
	virtual void OnPaintSpinCtrl(COXSkinnedSpinCtrl* pSpinCtrl) = 0;

	// Draw the outer border of an MDI tab workspace
	virtual void DrawMDITabBorder(CDC* pDC, LPCRECT lpRect, COXTabWorkspace* pTabWorkspace) = 0;

	// Handle WM_NCPAINT for COXTabClientWnd
	virtual void OnNcPaintTabClient(COXTabClientWnd* pTabClient) = 0;

	// Adjust the rectangle of the child item for the given tab control
	virtual void AdjustChildItemRect(LPRECT pRectChild, int iInflateValue, CTabCtrl* pTabCtrl) = 0;

protected:


private:


};

#endif /* COXTabSkin_h */
// COXTabSkinClassic.h
//

#if !defined(COXTabSkinClassic_h)
#define COXTabSkinClassic_h

#include "OX3DTabView.h"
#include "OXTabClientWnd.h"

class OX_CLASS_DECL COXTabSkinClassic : public COXTabSkin
{
public:
	virtual ~COXTabSkinClassic() { };
	virtual void OnPaintTabCtrl(CTabCtrl* pTabCtrl);
	virtual void OnPaintSpinCtrl(COXSkinnedSpinCtrl* pSpinCtrl) { pSpinCtrl->Default(); };
	virtual void DrawMDITabBorder(CDC* pDC, LPCRECT lpRect, COXTabWorkspace* pTabWorkspace);
	virtual void OnNcPaintTabClient(COXTabClientWnd* pTabClient) { pTabClient->Default(); };
	virtual void AdjustChildItemRect(LPRECT /*pRectChild*/, int /*iInflateValue*/, CTabCtrl* /*pTabCtrl*/) { };

protected:


private:


};

#endif /* COXTabSkinClassic_h */
// COXTabSkinXP.h
//

#if !defined(COXTabSkinXP_h)
#define COXTabSkinXP_h


class COXSkinXP;

class OX_CLASS_DECL COXTabSkinXP : public COXTabSkin
{
public:
	COXTabSkinXP(COXSkinXP* pSkinXP);
	virtual ~COXTabSkinXP() { };
	virtual void OnPaintTabCtrl(CTabCtrl* pTabCtrl);
	virtual void OnPaintSpinCtrl(COXSkinnedSpinCtrl* pSpinCtrl);
	virtual void DrawMDITabBorder(CDC* pDC, LPCRECT lpRect, COXTabWorkspace* pTabWorkspace);
	virtual void OnNcPaintTabClient(COXTabClientWnd* pTabClient);
	virtual void AdjustChildItemRect(LPRECT pRectChild, int iInflateValue, CTabCtrl* pTabCtrl);

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct, CTabCtrl* pTabContainer);
	virtual void DrawBackground(CDC* pDC, LPCRECT lpRect, CTabCtrl* pTabCtrl);
	virtual void DrawItemBorder(LPDRAWITEMSTRUCT lpDrawItemStruct, CTabCtrl* pTabContainer);

protected:
	void DrawDownArrow(CDC* pDc, CRect btnRect, COXSkinnedSpinCtrl* pSpinCtrl);
	void DrawUpArrow(CDC* pDc, CRect btnRect, COXSkinnedSpinCtrl* pSpinCtrl);
	void DrawArrowBorder( CDC* pDc, CRect btnRect, COXSkinnedSpinCtrl* pSpinCtrl);
	BOOL AdjustItemRect(int nIndex, RECT *tabRect, CTabCtrl* pTabCtrl);
	BOOL IsNextButtonSelected(int nItemID, CTabCtrl* pTabCtrl);
	COXSkinnedSpinCtrl* GetSpinCtrl(CTabCtrl* pTabContainer);
	COXSkinXP* m_pSkinXP;
	CPen m_sepPen;
	CFont m_tabFont;
	CBrush m_btnFill;
	CPen m_btnLine;
};

#endif // COXTabSkinXP_h

// COXTabSkin2003.h
//

#if !defined(COXTabSkin2003_h)
#define COXTabSkin2003_h


class OX_CLASS_DECL COXTabSkin2003 : public COXTabSkinXP
{
public:
	COXTabSkin2003(COXSkin2003* pSkin2003);
	virtual ~COXTabSkin2003();

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct, CTabCtrl* pTabContainer);
	virtual void DrawBackground(CDC* pDC, LPCRECT lpRect, CTabCtrl* pTabCtrl);
	virtual void OnPaintTabCtrl(CTabCtrl* pTabCtrl);
	virtual void OnPaintSpinCtrl(COXSkinnedSpinCtrl* pSpinCtrl);
	virtual void DrawItemBorder(LPDRAWITEMSTRUCT lpDrawItemStruct, CTabCtrl* pTabContainer);

protected:

};

#endif // COXTabSkin2003_h

// COXToolbarSkin.h
//

#if !defined(COXToolbarSkin_h)
#define COXToolbarSkin_h

class COXSkinnedComboBox;
class COXMenuBar;

class OX_CLASS_DECL COXToolbarSkin
{
public:
	COXToolbarSkin() { };
	virtual ~COXToolbarSkin() { };
    virtual void DrawGripper(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar) = 0;
    virtual void DrawNonClientArea(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar) = 0;
    virtual void DrawSeparator(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar) = 0;
	
	// Draws the button when in customize mode
	virtual void DrawCustomizedButton(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar) = 0;
	
	virtual void OnCustomDraw(NMHDR*& pNotify, LRESULT*& pResult, COXCoolToolBar* pCoolToolbar) = 0;
	virtual void OnPaintComboBox(COXSkinnedComboBox* pComboBox) = 0;
    virtual void DrawSystemButton(CDC* /*pDC*/) { };
	virtual void DrawComboBox(COXSkinnedComboBox* pComboBox) = 0;
	virtual void DrawWndFrame(CWnd* pWnd) = 0;
	virtual void DrawMenuButtons(CDC* pDC, COXMenuBar* pMenuBar) = 0;
	virtual BOOL CallOnEraseBkgnd() = 0;

protected:


private:


};

#endif /* COXToolbarSkin_h */


// COXStatusbarSkin.h
//

#if !defined(COXStatusbarSkin_h)
#define COXStatusbarSkin_h

struct AFX_STATUSPANE
{
	UINT    nID;        // IDC of indicator: 0 => normal text area
	int     cxText;     // width of string area in pixels
						//   on both sides there is a 3 pixel gap and
						//   a one pixel border, making a pane 6 pixels wider
	UINT    nStyle;     // style flags (SBPS_*)
	UINT    nFlags;     // state flags (SBPF_*)
	CString strText;    // text in the pane
};

class COXStatusBar;

class OX_CLASS_DECL COXStatusbarSkin
{
public:
	virtual ~COXStatusbarSkin() { };
	virtual void OnPaintStatusbar(COXStatusBar* pStatusBar) = 0;
	virtual void DrawStatusBarPanes(CDC* /*pDC*/, COXStatusBar* pStatusBar) = 0;
	virtual void DrawFlatPanes(CDC* pDC, COXStatusBar* pStatusBar) = 0;
	virtual void DrawGripper(CDC* pDC, COXStatusBar* pStatusBar) = 0;

protected:


private:


};

#endif /* COXStatusbarSkin_h */
// COXStatusbarSkinClassic.h
//

#if !defined(COXStatusbarSkinClassic_h)
#define COXStatusbarSkinClassic_h

class OX_CLASS_DECL COXStatusbarSkinClassic : public COXStatusbarSkin
{
public:
	virtual ~COXStatusbarSkinClassic() { };
	virtual void OnPaintStatusbar(COXStatusBar* pStatusBar);
	virtual void DrawStatusBarPanes(CDC* /*pDC*/, COXStatusBar* /*pStatusBar*/) { };
	virtual void DrawFlatPanes(CDC* /*pDC*/, COXStatusBar* /*pStatusBar*/) { };
	virtual void DrawGripper(CDC* /*pDC*/, COXStatusBar* /*pStatusBar*/) { };

protected:


private:


};

#endif /* COXStatusbarSkinClassic_h */
// COXStatusbarSkinXP.h
//

#if !defined(COXStatusbarSkinXP_h)
#define COXStatusbarSkinXP_h


class OX_CLASS_DECL COXStatusbarSkinXP : public COXStatusbarSkin
{
public:
	COXStatusbarSkinXP(COXSkinXP* pSkinXP);
	virtual ~COXStatusbarSkinXP();
	virtual void OnPaintStatusbar(COXStatusBar* pStatusBar);
	virtual void DrawStatusBarPanes(CDC* pDC, COXStatusBar* pStatusBar);
	virtual void DrawFlatPanes(CDC* pDC, COXStatusBar* pStatusBar);
	virtual void DrawGripper(CDC* pDC, COXStatusBar* pStatusBar);

protected:
	COXSkinXP* m_pSkinXP;
	void DrawPaneText( CDC *pDc, CRect rPane, COXStatusBar* pStatusBar, int nIndex, BOOL enabled = TRUE );
};
#endif /* COXStatusbarSkinXP_h */


// COXStatusbarSkinXP.h
//

#if !defined(COXStatusbarSkin2003_h)
#define COXStatusbarSkin2003_h


class OX_CLASS_DECL COXStatusbarSkin2003 : public COXStatusbarSkinXP
{
public:
	COXStatusbarSkin2003(COXSkin2003* pSkin2003);
	virtual ~COXStatusbarSkin2003();

	virtual void DrawStatusBarPanes(CDC* pDC, COXStatusBar* pStatusBar);
	virtual void DrawFlatPanes(CDC* pDC, COXStatusBar* pStatusBar);
	virtual void DrawGripper(CDC* pDC, COXStatusBar* pStatusBar);

protected:
	void DrawGradientBackground(CDC* pDC, LPCRECT lpRect);
};

#endif /* COXStatusbarSkin2003_h */


///////////////////////////////

// COXFrameSkin.h
//

#if !defined(COXFrameSkin_h)
#define COXFrameSkin_h

class OX_CLASS_DECL COXFrameSkin
{
public:
	virtual ~COXFrameSkin() { };

	// Draws the border of the view
	virtual void DrawViewBorder(CView* pView) = 0;

	// Handles WM_NCCALSIZE for a skinned view
	virtual void OnNcCalcSizeView(LPARAM lParam, CView* pView) = 0;

	// Returns the backgorund color of the frame window
	virtual COLORREF GetBackgroundColor() = 0;

	// Returns the border color
	virtual COLORREF GetBorderColor() = 0;

protected:


private:


};

#endif /* COXFrameSkin_h */

// COXFrameSkinClassic.h
//

#if !defined(COXFrameSkinClassic_h)
#define COXFrameSkinClassic_h

class OX_CLASS_DECL COXFrameSkinClassic : public COXFrameSkin
{
public:
	virtual ~COXFrameSkinClassic() { };
	virtual void DrawViewBorder(CView* /*pView*/) { };
	virtual void OnNcCalcSizeView(LPARAM /*lParam*/, CView* /*pView*/) { };
	virtual COLORREF GetBackgroundColor() { return ::GetSysColor(COLOR_3DFACE); };
	virtual COLORREF GetBorderColor() { return ::GetSysColor(COLOR_3DSHADOW); };

protected:


private:


};

#endif /* COXFrameSkinClassic_h */
// COXFrameSkinXP.h
//

#if !defined(COXFrameSkinXP_h)
#define COXFrameSkinXP_h


class OX_CLASS_DECL COXFrameSkinXP : public COXFrameSkin
{
public:
	COXFrameSkinXP(COXSkinXP* pSkinXP);
	virtual ~COXFrameSkinXP() { };
	virtual void DrawViewBorder(CView* pView);
	virtual void OnNcCalcSizeView(LPARAM lParam, CView* pView);
	virtual COLORREF GetBackgroundColor() { return ::GetSysColor(COLOR_3DFACE); };
	virtual COLORREF GetBorderColor() { return ::GetSysColor(COLOR_3DSHADOW); };

protected:
	COXSkinXP* m_pSkinXP;
};
#endif /* COXFrameSkinXP_h */


// COXFrameSkin2003.h
//

#if !defined(COXFrameSkin2003_h)
#define COXFrameSkin2003_h


class OX_CLASS_DECL COXFrameSkin2003 : public COXFrameSkinXP
{
public:
	COXFrameSkin2003(COXSkin2003* pSkin2003);
	virtual ~COXFrameSkin2003() { };
	virtual COLORREF GetBackgroundColor();
	virtual COLORREF GetBorderColor();
};
#endif /* COXFrameSkin2003_h */


// COXMenuSkin.h
//

#if !defined(COXMenuSkin_h)
#define COXMenuSkin_h

#include "OXBitmapMenu.h"

class OX_CLASS_DECL COXMenuSkin
{
public:
	virtual ~COXMenuSkin() { };
	// Draws the background of menu item
	virtual void DrawBackground(CDC* pDC, UINT nState, COXImageInfo* pImageInfo, LPRECT lpItemRect, LPRECT lpButtonRect, COXBitmapMenu* pBitmapMenu) = 0;
	// Draws the button which frames the menu item image
	virtual void DrawButton(CDC* pDC, UINT nState,  COXImageInfo* pImageInfo, LPRECT lpButtonRect, COXBitmapMenu* pBitmapMenu) = 0;
	// Draws the menu item image
	virtual void DrawImage(CDC* pDC, UINT nState, COXImageInfo* pImageInfo, LPRECT lpImageRect, COXBitmapMenu* pBitmapMenu) = 0;
	// Draws the menu item text
	virtual void DrawText(CDC* pDC, UINT nState, LPCTSTR lpszText, LPRECT lpText1Rect, LPRECT lpText2Rect, COXBitmapMenu* pBitmapMenu) = 0;
	// Draws a menu separator
	virtual void DrawSeparator(CDC* pDC, LPRECT lpItemRect, COXBitmapMenu* pBitmapMenu) = 0;
	// Draws customized menu item
	virtual void DrawCustomized(CDC* pDC, LPRECT lpItemRect, COXBitmapMenu* pBitmapMenu) = 0;
	// Draws insert mark
	virtual void DrawInsertMark(CDC* pDC, LPRECT lpItemRect, BOOL bBefore, COXBitmapMenu* pBitmapMenu) = 0;
	// Draws submenu item
	virtual void DrawSubmenuItem(CDC* pDC, UINT nState, LPCTSTR lpszText, COXImageInfo* pImageInfo,
		LPRECT lpItemRect, LPRECT lpButtonRect, LPRECT lpText1Rect, LPRECT lpText2Rect, COXBitmapMenu* pBitmapMenu) = 0;
	// Draws expansion item
	virtual void DrawExpansionItem(CDC* pDC, LPRECT lpItemRect, UINT nState, COXBitmapMenu* pBitmapMenu) = 0;
	// Returns a reference to a structure containing the item dimention constants
	virtual OXDIMENSIONCONSTANTS& GetDimentionConstants() = 0;
	// Handles all messages for the subclassed menu popup window
	virtual long MenuPopupWndProc(WNDPROC origWndProc, HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam) = 0;
	// Handles WM_NCPAINT for COXBitmapMenuPopupWnd depending on the skin
	virtual void OnNcPaintCustomizePopupWnd(COXBitmapMenuPopupWnd* pPopupWnd) = 0;
	// Handles WM_PAINT for COXShadowedItemWnd depending on the skin
	virtual void OnPaintShadowedItemWnd(COXShadowedItemWnd* pShadowedItemWnd) = 0;
	// Adjust the height of an expansion item
	virtual void AdjustExpansionItemHeight(UINT& nItemHeight, COXBitmapMenu* pBitmapMenu) = 0;
	// Determines the dimentions of the rectangles representing the different menu components
	virtual void DistributeSpace(UINT nState, COXImageInfo* pImageInfo, LPRECT lpItemRect, CRect& rectButton, CRect& rectImage, CRect& rectText1, CRect& rectText2, COXBitmapMenu* pBitmapMenu) = 0;

protected:


private:


};

#endif /* COXMenuSkin_h */
// COXMenuSkinClassic.h
//

#if !defined(COXMenuSkinClassic_h)
#define COXMenuSkinClassic_h



class COXMenubarSkinClassic;

class OX_CLASS_DECL COXMenuSkinClassic : public COXMenuSkin
{
public:
	COXMenuSkinClassic();
	virtual ~COXMenuSkinClassic() { };

	virtual void DrawBackground(CDC* pDC, UINT nState, COXImageInfo* pImageInfo, LPRECT lpItemRect, LPRECT lpButtonRect, COXBitmapMenu* pBitmapMenu);
	virtual void DrawButton(CDC* pDC, UINT nState,  COXImageInfo* pImageInfo, LPRECT lpButtonRect, COXBitmapMenu* pBitmapMenu);
	virtual void DrawImage(CDC* pDC, UINT nState, COXImageInfo* pImageInfo, LPRECT lpImageRect, COXBitmapMenu* pBitmapMenu);
	virtual void DrawText(CDC* pDC, UINT nState, LPCTSTR lpszText, LPRECT lpText1Rect, LPRECT lpText2Rect, COXBitmapMenu* pBitmapMenu);
	virtual void DrawSeparator(CDC* pDC, LPRECT lpItemRect, COXBitmapMenu* pBitmapMenu);
	virtual void DrawCustomized(CDC* pDC, LPRECT lpItemRect, COXBitmapMenu* pBitmapMenu);
	virtual void DrawInsertMark(CDC* pDC, LPRECT lpItemRect, BOOL bBefore, COXBitmapMenu* pBitmapMenu);
	virtual void DrawSubmenuItem(CDC* pDC, UINT nState, LPCTSTR lpszText, COXImageInfo* pImageInfo,
		LPRECT lpItemRect, LPRECT lpButtonRect, LPRECT lpText1Rect, LPRECT lpText2Rect, COXBitmapMenu* pBitmapMenu);
	virtual void DrawExpansionItem(CDC* pDC, LPRECT lpItemRect, UINT nState, COXBitmapMenu* pBitmapMenu);
	virtual OXDIMENSIONCONSTANTS& GetDimentionConstants() { return m_DimensionConstants; };
	virtual LRESULT MenuPopupWndProc(WNDPROC origWndProc, HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnNcPaintCustomizePopupWnd(COXBitmapMenuPopupWnd* pPopupWnd) { pPopupWnd->Default(); };
	virtual void OnPaintShadowedItemWnd(COXShadowedItemWnd* pShadowedItemWnd) { pShadowedItemWnd->Default(); };
	virtual void AdjustExpansionItemHeight(UINT& /*nItemHeight*/, COXBitmapMenu* /*pBitmapMenu*/) { }; 
	virtual void DistributeSpace(UINT nState, COXImageInfo* pImageInfo, LPRECT lpItemRect,
		CRect& rectButton, CRect& rectImage, CRect& rectText1, CRect& rectText2, COXBitmapMenu* pBitmapMenu);

protected:


private:
	OXDIMENSIONCONSTANTS m_DimensionConstants;

};

#endif /* COXMenuSkinClassic_h */
// COXMenuSkinXP.h
//

#if !defined(COXMenuSkinXP_h)
#define COXMenuSkinXP_h


class COXMenubarSkinXP;
class COXSkinXP;

class OX_CLASS_DECL COXMenuSkinXP : public COXMenuSkin
{
public:
	COXMenuSkinXP(COXSkinXP* pSkinXP);
	virtual ~COXMenuSkinXP();
	virtual void DrawBackground(CDC* pDC, UINT nState, COXImageInfo* pImageInfo, LPRECT lpItemRect, LPRECT lpButtonRect, COXBitmapMenu* pBitmapMenu);
	virtual void DrawButton(CDC* pDC, UINT nState,  COXImageInfo* pImageInfo, LPRECT lpButtonRect, COXBitmapMenu* pBitmapMenu);
	virtual void DrawImage(CDC* pDC, UINT nState, COXImageInfo* pImageInfo, LPRECT lpImageRect, COXBitmapMenu* pBitmapMenu);
	virtual void DrawText(CDC* pDC, UINT nState, LPCTSTR lpszText, LPRECT lpText1Rect, LPRECT lpText2Rect, COXBitmapMenu* pBitmapMenu);
	virtual void DrawSeparator(CDC* pDC, LPRECT lpItemRect, COXBitmapMenu* pBitmapMenu);
	virtual void DrawCustomized(CDC* pDC, LPRECT lpItemRect, COXBitmapMenu* pBitmapMenu);
	virtual void DrawInsertMark(CDC* pDC, LPRECT lpItemRect, BOOL bBefore, COXBitmapMenu* pBitmapMenu);
	virtual void DrawSubmenuItem(CDC* pDC, UINT nState, LPCTSTR lpszText, COXImageInfo* pImageInfo,
		LPRECT lpItemRect, LPRECT lpButtonRect, LPRECT lpText1Rect, LPRECT lpText2Rect, COXBitmapMenu* pBitmapMenu);
	virtual void DrawExpansionItem(CDC* pDC, LPRECT lpItemRect, UINT nState, COXBitmapMenu* pBitmapMenu);
	virtual OXDIMENSIONCONSTANTS& GetDimentionConstants() { return m_DimensionConstants; };
	virtual LRESULT MenuPopupWndProc(WNDPROC origWndProc, HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnNcPaintCustomizePopupWnd(COXBitmapMenuPopupWnd* pPopupWnd);
	virtual void OnPaintShadowedItemWnd(COXShadowedItemWnd* pShadowedItemWnd);
	virtual void AdjustExpansionItemHeight(UINT& nItemHeight, COXBitmapMenu* pBitmapMenu); 
	virtual void DistributeSpace(UINT nState, COXImageInfo* pImageInfo, LPRECT lpItemRect,
		CRect& rectButton, CRect& rectImage, CRect& rectText1, CRect& rectText2, COXBitmapMenu* pBitmapMenu);

	virtual void DrawItemStrip(CDC* pDC, LPRECT lpStripRect); // Draws the gray strip underneath the icon.
	virtual void DrawJoiningLine(CDC* pDC, LPRECT lpRect); // Draws the line which joins the menu frame to the menu item
	virtual void FillIntersectionRect(HDC hDC, LPRECT lpRect); // Fills the intersection rectangle between the menu and the menu item

	COXSkinXP* m_pSkinXP;
	OXDIMENSIONCONSTANTS m_DimensionConstants;
	COLORREF DarkenColor(long lScale, COLORREF lColor);
	void DrawMenuShadow(HDC hDC, LPRECT lpRect, LPRECT lpItemRect, OXSHADOWARRAY* pShadowArray);
	void DrawMenuItemShadow(HDC hDC, LPRECT lpRect, UINT nMenuItemShadowType);
	CMap<HWND, HWND, CPoint, CPoint> m_mapHWNDtoPos;
};

#endif /* COXMenuSkin2003_h */

#if !defined(COXMenuSkin2003_h)
#define COXMenuSkin2003_h


class COXSkin2003;

class OX_CLASS_DECL COXMenuSkin2003 : public COXMenuSkinXP
{
public:
	COXMenuSkin2003(COXSkin2003* pSkin2003);
	virtual ~COXMenuSkin2003();

	// Methods
	virtual void DrawBackground(CDC* pDC, UINT nState, COXImageInfo* pImageInfo, LPRECT lpItemRect, LPRECT lpButtonRect, COXBitmapMenu* pBitmapMenu);
	virtual void DrawItemStrip(CDC* pDC, LPRECT lpStripRect); // Draws the gray strip underneath the icon.
	virtual void DrawSeparator(CDC* pDC, LPRECT lpItemRect, COXBitmapMenu* pBitmapMenu);
	virtual void DrawImage(CDC* pDC, UINT nState, COXImageInfo* pImageInfo, LPRECT lpImageRect, COXBitmapMenu* pBitmapMenu);
	virtual void DrawJoiningLine(CDC* pDC, LPRECT lpRect);
	virtual void FillIntersectionRect(HDC /*hDC*/, LPRECT /*lpRect*/) { };
	virtual void OnPaintShadowedItemWnd(COXShadowedItemWnd* pShadowedItemWnd);

protected:


private:

};

#endif /* COXMenuSkin2003_h */

// COXShortcutBarSkin.h
//

#if !defined(COXShortcutBarSkin_h)
#define COXShortcutBarSkin_h

class COXShortcutBar;
class COXSHBListCtrl;

class OX_CLASS_DECL COXShortcutBarSkin
{
public:
	virtual ~COXShortcutBarSkin() { };
	virtual COLORREF GetBackgroundColor(CWnd* pWnd) = 0;
	virtual COLORREF GetGroupTextColor(COXShortcutBar* pShortcutBar) = 0;
	virtual void DrawItemBorder(CDC* pDC, LPCRECT lpRect, BOOL bHot, BOOL bSelected, COXSHBListCtrl* pSHBListCtrl) = 0;
	virtual void FillBackground(CDC* pDC, LPCRECT lpRect, CWnd* pWnd, BOOL bHasBackgroundColor = FALSE, COLORREF clrBackgroundColor = 0) = 0;

    virtual void DrawHeader(LPDRAWITEMSTRUCT lpDrawItemStruct, COXShortcutBar * bar);
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	virtual OXAppSkins GetType() {return OXSkinNone; };

	virtual void DrawTopHeader(COXShortcutBar * /*bar*/, CDC * /*pDC*/) {};

	bool AnimateSelection () { return m_bAnimate; }

protected:

	bool m_bAnimate;

	int DrawHeaderImage(HSHBGROUP hGroup, COXShortcutBar * bar, CDC *pDC, POINT topLeft);
	void DrawHeaderText(HSHBGROUP hGroup, COXShortcutBar * bar, CDC * pDC, RECT rectHeader, UINT nFormat, bool alwaysBold);
	void DrawDropTarget(HSHBGROUP hGroup, COXShortcutBar * bar, CDC *pDC, LPDRAWITEMSTRUCT lpDrawItemStruct);

private:


};

#endif /* COXShortcutBarSkin_h */
// COXShortcutBarSkinClassic.h
//

#if !defined(COXShortcutBarSkinClassic_h)
#define COXShortcutBarSkinClassic_h


class OX_CLASS_DECL COXShortcutBarSkinClassic : public COXShortcutBarSkin
{
public:
	COXShortcutBarSkinClassic::COXShortcutBarSkinClassic() { m_bAnimate = true; };
	virtual ~COXShortcutBarSkinClassic() { };
	virtual COLORREF GetBackgroundColor(CWnd* pWnd);
	virtual COLORREF GetGroupTextColor(COXShortcutBar* pShortcutBar);
	virtual void DrawItemBorder(CDC* pDC, LPCRECT lpRect, BOOL bHot, BOOL bSelected, COXSHBListCtrl* pSHBListCtrl);
	virtual void FillBackground(CDC* pDC, LPCRECT lpRect, CWnd* pWnd, BOOL bHasBackgroundColor = FALSE, COLORREF clrBackgroundColor = 0);

	OXAppSkins GetType() {return OXSkinClassic; };

	virtual void DrawTopHeader(COXShortcutBar * /*bar*/, CDC * /*pDC*/) {};

protected:


private:
	COLORREF UpdateColor(COLORREF clr, int nOffset);

};

#endif /* COXShortcutBarSkinClassic_h */
// COXShortcutBarSkinXP.h
//

#if !defined(COXShortcutBarSkinXP_h)
#define COXShortcutBarSkinXP_h


class COXSkinXP;

class OX_CLASS_DECL COXShortcutBarSkinXP : public COXShortcutBarSkin
{
public:
	COXShortcutBarSkinXP(COXSkinXP* pSkinXP);
	virtual ~COXShortcutBarSkinXP() { };
	virtual COLORREF GetBackgroundColor(CWnd* pWnd);
	virtual COLORREF GetGroupTextColor(COXShortcutBar* pShortcutBar);
	virtual void DrawItemBorder(CDC* pDC, LPCRECT lpRect, BOOL bHot, BOOL bSelected, COXSHBListCtrl* pSHBListCtrl);
	virtual void FillBackground(CDC* pDC, LPCRECT lpRect, CWnd* pWnd, BOOL bHasBackgroundColor = FALSE, COLORREF clrBackgroundColor = 0);
 	virtual void DrawTopHeader(COXShortcutBar * bar, CDC * pDC);

	OXAppSkins GetType() {return OXSkinOfficeXP; };

protected:
	COXSkinXP* m_pSkinXP;

};

#endif /* COXShortcutBarSkinXP_h */



#if !defined(COXShortcutBarSkin2003_h)
#define COXShortcutBarSkin2003_h


class OX_CLASS_DECL COXShortcutBarSkin2003 : public COXShortcutBarSkinXP
{
public:
	COXShortcutBarSkin2003(COXSkin2003* pSkin2003);
	virtual ~COXShortcutBarSkin2003();

	OXAppSkins GetType() {return OXSkinOffice2003; };

        virtual void DrawHeader(LPDRAWITEMSTRUCT lpDrawItemStruct, COXShortcutBar * bar);
	virtual void FillBackground(CDC* pDC, LPCRECT lpRect, CWnd* pWnd, BOOL bHasBackgroundColor = FALSE, COLORREF clrBackgroundColor = 0);
	virtual void DrawTopHeader(COXShortcutBar * bar, CDC * pDC);
};

#endif /* COXShortcutBarSkin2003_h */

// COXMiniFrameSkin.h
//

#if !defined(COXMiniFrameSkin_h)
#define COXMiniFrameSkin_h


class COXSizableMiniDockFrameWnd;

class OX_CLASS_DECL COXMiniFrameSkin
{
public:
	virtual ~COXMiniFrameSkin() { };
    virtual void OnNcPaint(COXSizableMiniDockFrameWnd* pMiniDockFrameWnd) = 0;
    virtual BOOL OnNcActivate(BOOL bActive, COXSizableMiniDockFrameWnd* pMiniDockFrameWnd) = 0;
	virtual void DrawCaption(COXSizableMiniDockFrameWnd* pMiniDockFrameWnd) = 0;

	// Draw the caption buttons of the mini frame window.
	// Return value:
	//			TRUE - the function performed the drawing
	//			FALSE - the funtion did not draw anything
	virtual BOOL DrawCaptionButtons(COXSizableMiniDockFrameWnd* pMiniDockFrameWnd) = 0;

protected:


private:


};

#endif /* COXMiniFrameSkin_h */

// COXMiniFrameSkinClassic.h
//

#if !defined(COXMiniFrameSkinClassic_h)
#define COXMiniFrameSkinClassic_h


class OX_CLASS_DECL COXMiniFrameSkinClassic : public COXMiniFrameSkin
{
public:
	virtual ~COXMiniFrameSkinClassic() { };
    virtual void OnNcPaint(COXSizableMiniDockFrameWnd* pMiniDockFrameWnd);
    virtual BOOL OnNcActivate(BOOL bActive, COXSizableMiniDockFrameWnd* pMiniDockFrameWnd);
	virtual void DrawCaption(COXSizableMiniDockFrameWnd* /*pMiniDockFrameWnd*/) { };
	virtual BOOL DrawCaptionButtons(COXSizableMiniDockFrameWnd* /*pMiniDockFrameWnd*/) { return FALSE; };

protected:


private:


};

#endif /* COXMiniFrameSkinClassic_h */
// COXMiniFrameSkinXP.h
//

#if !defined(COXMiniFrameSkinXP_h)
#define COXMiniFrameSkinXP_h


class COXSkinXP;

class OX_CLASS_DECL COXMiniFrameSkinXP : public COXMiniFrameSkin
{
public:
	COXMiniFrameSkinXP(COXSkinXP* pSkinXP);
	virtual ~COXMiniFrameSkinXP() { };
    virtual void OnNcPaint(COXSizableMiniDockFrameWnd* pMiniDockFrameWnd);
    virtual BOOL OnNcActivate(BOOL bActive, COXSizableMiniDockFrameWnd* pMiniDockFrameWnd);
	virtual void DrawCaption(COXSizableMiniDockFrameWnd* pMiniDockFrameWnd);
	virtual BOOL DrawCaptionButtons(COXSizableMiniDockFrameWnd* pMiniDockFrameWnd);

protected:
	COXSkinXP* m_pSkinXP;
	CFont m_fontCaption;
	CRect GetCaptionRect(COXSizableMiniDockFrameWnd* pMiniDockFrameWnd);
};

#endif /* COXMiniFrameSkinXP_h */

// COXSkin.h
//

#if !defined(COXSkin_h)
#define COXSkin_h


class OX_CLASS_DECL COXSkin
{
public:
	COXSkin();
	virtual ~COXSkin();
    /*
     * Returns the name of the look and feel.
     */
    CString GetName() const;
    /*
     * Returns a pointer to COXToolbarSkin derived class.
     */
    COXToolbarSkin* GetToolbarSkin() const;
    /*
     * Returns a pointer to COXMenuSkin derived class.
     */
    COXMenuSkin* GetMenuSkin() const;
    /*
     * Returns a pointer to COXDockbarSkin derived class.
     */
    COXDockbarSkin* GetDockbarSkin() const;
    /*
     * Returns a pointer to COXTabSkin derived class.
     */
    COXTabSkin* GetTabSkin() const;
    /*
     * Returns a pointer to COXShortcutBarSkin derived class.
     */
    COXShortcutBarSkin* GetShortcutBarSkin() const;
    COXMiniFrameSkin* GetMiniFrameSkin() const;
    COXToolbarSkin* GetMenubarSkin() const;
    COXStatusbarSkin* GetStatusbarSkin() const;
    COXFrameSkin* GetFrameSkin() const;
	void SetToolbarSkin(COXToolbarSkin* pToolbarSkin);
	void SetTabSkin(COXTabSkin* pTabSkin);
	void SetStatusbarSkin(COXStatusbarSkin* pStatusbarSkin);
	void SetShortcutBarSkin(COXShortcutBarSkin* pShortcutBarSkin);
	void SetMiniFrameSkin(COXMiniFrameSkin* pMiniFrameSkin);
	void SetMenuSkin(COXMenuSkin* pMenuSkin);
	void SetMenubarSkin(COXToolbarSkin* pMenubarSkin);
	void SetFrameSkin(COXFrameSkin* pFrameSkin);
	void SetDockbarSkin(COXDockbarSkin* pDockbarSkin);
    CString COXSkin::GetProperty(LPCTSTR lpszName) const;
    void COXSkin::SetProperty(LPCTSTR lpszName, LPCTSTR lpszValue);

	virtual OXAppSkins GetType() { return OXSkinNone; };

protected:
    CString	m_strName;
    COXToolbarSkin*	m_pToolbarSkin;
    COXMenuSkin*	m_pMenuSkin;
    COXToolbarSkin*	m_pMenubarSkin;
    COXDockbarSkin*	m_pDockbarSkin;
    COXTabSkin*	m_pTabSkin;
    COXShortcutBarSkin*	m_pShortcutBarSkin;
	COXMiniFrameSkin* m_pMiniFrameSkin;
	COXStatusbarSkin* m_pStatusbarSkin;
	COXFrameSkin* m_pFrameSkin;


private:
    CMapStringToString	m_mapProperties;

};

#endif /* COXSkin_h */
// COXSkinClassic.h
//

#if !defined(COXSkinClassic_h)
#define COXSkinClassic_h


class COXToolbarSkinClassic;
class COXMenuSkinClassic;
class COXDockbarSkinClassic;
class COXTabSkinClassic;
class COXShortcutBarSkinClassic;

class OX_CLASS_DECL COXSkinClassic : public COXSkin
{
public:

    COXSkinClassic();
    virtual ~COXSkinClassic();

	OXAppSkins GetType() { return OXSkinClassic; };


protected:


private:


};

#endif /* COXSkinClassic_h */
#if !defined(AFX_OXSKINNEDAPP_H__CF96F5BF_EF34_4BC1_BAD2_5878582C8AC3__INCLUDED_)
#define AFX_OXSKINNEDAPP_H__CF96F5BF_EF34_4BC1_BAD2_5878582C8AC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OXSkinnedApp.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// COXSkinnedApp class
// ==========================================================================
//								Class Specification : 
//									COXSkinnedApp
// ==========================================================================

// Header file : OXSkins.h

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////
/*

Overview

COXSkinnedApp provides the ability to change the look and feel of your application.
In order to use this functionality you must derive your application class from
COXSkinnedApp instead of CWinApp.

The COXSkinnedApp class provides member functions to change the current look and
feel (also referred as skins) at runtime. In addition it allows custom skins to be
added.

*/

class OX_CLASS_DECL COXSkinnedApp : public CWinApp
{
	DECLARE_DYNCREATE(COXSkinnedApp)
protected:
	COXSkinnedApp();           // protected constructor used by dynamic creation
	OXAppSkins m_currentSkin;

// Operations
public:
	// --- In  :
	// --- Out : 
	// --- Returns: Pointer to the currenctly selected skin
	// --- Effect : Returns a pointer to the current skin
    COXSkin* GetCurrentSkin();

	OXAppSkins GetCurrentSkinType()
	{
		return m_currentSkin;
	}

	BOOL SetCurrentSkin(OXAppSkins skin);

	// --- In  : A pointer to the new skin
	// --- Out : 
	// --- Returns:
	// --- Effect : Adds a new skin. A skin must be added before it can be selected.
    void AddSkin(COXSkin* pSkin);

	// --- In  : The name of the skin that is to be removed.
	// --- Out : 
	// --- Returns: TRUE is the skins was removed successfully, otherwise FALSE.
	// --- Effect : Removes a skins. Once a skin is removed it cannot be selected.
    BOOL RemoveSkin(LPCTSTR lpszName);

	// --- In  : The name of the skin that is to be set as current.
	// --- Out : 
	// --- Returns: TRUE is the skins was selected successfully, otherwise FALSE.
	// --- Effect : Select the skin. A the application will have the look and feel
	//				of the currenctly selected skin.
    BOOL SetCurrentSkin(LPCTSTR lpszName);

	// --- In  : The name of the skin that is to be retrieved.
	// --- Out : 
	// --- Returns: A pointer to the skin class or NULL if the skin cannot be found.
	// --- Effect : Returns a pointer to the given skin by name.
	COXSkin* GetSkin(LPCTSTR lpszName);

	// --- In  : The name of the skin that is to be set as current.
	// --- Out : 
	// --- Returns: TRUE is the skins was selected successfully, otherwise FALSE.
	// --- Effect : Select the skin. A the application will have the look and feel
	//				of the currenctly selected skin.	
	BOOL SetCurrentSkin(COXSkin* pSkin);

// Implementation
protected:
	virtual ~COXSkinnedApp();

private:
	void UpdateFloatingWindows(CWnd* pMainWnd);
	void UpdateChildrenRec(CWnd* pWnd);
    CMapStringToPtr	m_mapSkins;
	COXSkin* m_pCurrentSkin;
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_OXSKINNEDAPP_H__CF96F5BF_EF34_4BC1_BAD2_5878582C8AC3__INCLUDED_)

// COXSkinXP.h
//

#if !defined(COXSkinXP_h)
#define COXSkinXP_h


class COXToolbarSkinXP;
class COXMenuSkinXP;
class COXDockbarSkinXP;
class COXTabSkinXP;
class COXShortcutBarSkinXP;

class OX_CLASS_DECL COXSkinXP : public virtual COXSkin
{
public:
    COXSkinXP();
	COXSkinXP(BOOL) { }; // A constructor that does nothing
    virtual ~COXSkinXP();

	virtual COLORREF GetHotItemColor();
	virtual COLORREF GetCheckedItemColor();
	virtual COLORREF GetHotSelectedItemColor();
	virtual COLORREF GetActiveBorderColor();
	virtual COLORREF GetMenuFaceColor();
	virtual COLORREF GetMenuBorderColor();
	virtual COLORREF GetBackgroundColor();
	virtual COLORREF GetDockbarBackgroundColor();
	virtual COLORREF GetInactiveTabTextColor();
	virtual COLORREF GetSeparatorColor();
	virtual COLORREF GetBorderColor();
	virtual COLORREF GetGripperColor();
	virtual COLORREF GetMiniFrameCaptionColor();
	virtual COLORREF GetMiniFrameCaptionTextColor();
	virtual COLORREF GetMiniFrameBorderColor();
	virtual COLORREF GetMiniFrameLightBorderColor();
	virtual COLORREF GetDisabledTextColor();
	virtual COLORREF GetDisabledMenuTextColor();

	COLORREF GetHiddenItemBackgroundColor();
	COLORREF GetTextColor();
	COLORREF GetSelectedTextColor();
	COLORREF GetTabButtonsBackgroundColor();
	COLORREF GetStatusPaneColor();
	void DrawTransparentBitmap(CBitmap* pBitmap, CDC* pDestDC, int x, int y, int nWidth, int nHeight, COLORREF clrTransColor);
	virtual void DrawFrameButton(CDC* pDC, CRect& rcButton, UINT uType, BOOL bDisabled, BOOL bInvertFace, COLORREF clrBackground, CWnd* pParentWnd);
	COLORREF InvertColor(COLORREF clr);

	// Increments the given color by the given red, green and blue values
	COLORREF ModifyColor(COLORREF crBase, int iRed, int iGreen, int iBlue);

	// Multiplies the given golor by the given red, green and blue percentage values
	COLORREF ModifyColor(COLORREF crBase, float fRed, float fGreen, float fBlue);

	// Blends the three colors given the provided ratio
	COLORREF Blend3Colors(COLORREF clrFirst, COLORREF clrSecond, COLORREF clrThird, float fFirts, float fSecond, float fThird);

	OXAppSkins GetType() { return OXSkinOfficeXP; };

};

#endif /* COXSkinXP_h */

// COXSkin2003.h
//

#if !defined(COXSkin2003_h)
#define COXSkin2003_h

#include "OXUxTheme.h"


class COXToolbarSkinXP;
class COXMenuSkinXP;
class COXDockbarSkinXP;
class COXTabSkinXP;
class COXShortcutBarSkinXP;

class OX_CLASS_DECL COXSkin2003 : public COXSkinXP
{
public:
    COXSkin2003();
    virtual ~COXSkin2003();

	virtual void DrawFrameButton(CDC* pDC, CRect& rcButton, UINT uType, BOOL bDisabled, BOOL bInvertFace, COLORREF clrBackground, CWnd* pParentWnd);
	void DrawGripperStripes(CDC* pDC, CRect& rect, bool bHorizontal);

	BOOL GradientFill(CDC* pDC, LPCRECT lpRect, COLORREF clrLeft, COLORREF clrRight, ULONG dwMode);
	BOOL GradientFillManual(CDC* pDC, LPCRECT lpRect, COLORREF clrLeft, COLORREF clrRight, ULONG dwMode);
	BOOL COXSkin2003::FourPointGradientFill(CDC* pDC, LPCRECT lpRect,
										  COLORREF clrTopLeft, COLORREF clrTopRight,
										  COLORREF clrBottomLeft, COLORREF clrBottomRight);
	BOOL COXSkin2003::FourPointGradientFillManual(CDC* pDC, LPCRECT lpRect,
										  COLORREF clrTopLeft, COLORREF clrTopRight,
										  COLORREF clrBottomLeft, COLORREF clrBottomRight);

	// Colors
	COLORREF GetToolbarLightColor();
	COLORREF GetToolbarDarkColor();
	COLORREF GetMenuStripLeftColor();
	COLORREF GetMenuStripRightColor();
	COLORREF GetGripperStripesShadowColor();
	COLORREF GetControlBarLightColor();
	COLORREF GetControlBarDarkColor();
	COLORREF GetDockbarLeftColor();
	COLORREF GetDockbarRightColor();
	COLORREF GetTabBackgroundLightColor();
	COLORREF GetTabBackgroundDarkColor();
	COLORREF GetTabItemLightColor();
	COLORREF GetTabItemDarkColor();
	COLORREF GetMenubarItemLightColor();
	COLORREF GetMenubarItemDarkColor();
	COLORREF GetShortcutBarLightColor();
	COLORREF GetShortcutBarDarkColor();
	COLORREF GetSelShortcutBarLightColor();
	COLORREF GetSelShortcutBarDarkColor();
	COLORREF GetHotShortcutBarLightColor();
	COLORREF GetHotShortcutBarDarkColor();
	COLORREF GetStatusbarTopColor();
	COLORREF GetStatusbarMiddleColor();
	COLORREF GetStatusbarBottomColor();
	COLORREF GetSplitterLineColor();
	
	// Menu selection colors
	virtual COLORREF GetHotItemColor();
	virtual COLORREF GetCheckedItemColor();
	virtual COLORREF GetHotSelectedItemColor();
	virtual COLORREF GetActiveBorderColor();
	virtual COLORREF GetMenuFaceColor();
	virtual COLORREF GetMenuBorderColor();

	// Other colors
	virtual COLORREF GetBackgroundColor();
	virtual COLORREF GetDockbarBackgroundColor();
	virtual COLORREF GetInactiveTabTextColor();
	virtual COLORREF GetSeparatorColor();
	virtual COLORREF GetBorderColor();
	virtual COLORREF GetGripperColor();
	virtual COLORREF GetMiniFrameCaptionColor();
	virtual COLORREF GetMiniFrameCaptionTextColor();
	virtual COLORREF GetMiniFrameBorderColor();
	virtual COLORREF GetMiniFrameLightBorderColor();
	virtual COLORREF GetDisabledTextColor();
	virtual COLORREF GetDisabledMenuTextColor();

	// Toolbar selection colors
	COLORREF GetHotToolbarItemLightColor();
	COLORREF GetHotToolbarItemDarkColor();
	COLORREF GetCheckedToolbarItemLightColor();
	COLORREF GetCheckedToolbarItemDarkColor();
	COLORREF GetHotSelectedToolbarItemLightColor();
	COLORREF GetHotSelectedToolbarItemDarkColor();

	OXAppSkins GetType() { return OXSkinOfficeXP; };


protected:
	// Data members
	HMODULE m_hMsimg32DLL;
	FARPROC m_fpGradientFill;
	COXUxTheme m_UxTheme;

private:

};

#endif /* COXSkin2003_h */

// COXToolbarSkinClassic.h
//

#if !defined(COXToolbarSkinClassic_h)
#define COXToolbarSkinClassic_h

#include "OXSkinnedComboBox.h"
#include "OXSkinnedWnd.h"

class OX_CLASS_DECL COXToolbarSkinClassic : public COXToolbarSkin
{
public:
    virtual void DrawGripper(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar);
    virtual void DrawNonClientArea(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar);
    virtual void DrawSeparator(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar);
	virtual void DrawCustomizedButton(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar);
	virtual void OnCustomDraw(NMHDR*& pNotify, LRESULT*& pResult, COXCoolToolBar* pCoolToolbar);
	virtual void OnPaintComboBox(COXSkinnedComboBox* pComboBox) { pComboBox->Default(); };
	virtual void DrawComboBox(COXSkinnedComboBox* /*pComboBox*/) { };
	virtual void DrawWndFrame(CWnd* /*pWnd*/) { };
	virtual void DrawMenuButtons(CDC* pDC, COXMenuBar* pMenuBar);
	virtual BOOL CallOnEraseBkgnd() { return TRUE; };

protected:


private:
	void DrawBackground(CDC* pDC, COXCoolToolBar* pCoolToolbar);

};

#endif /* COXToolbarSkinClassic_h */

// COXToolbarSkinXP.h
//

#if !defined(COXToolbarSkinXP_h)
#define COXToolbarSkinXP_h


class COXSkinXP;

class OX_CLASS_DECL COXToolbarSkinXP : public COXToolbarSkin
{
public:
	COXToolbarSkinXP(COXSkinXP* pSkinXP);
	virtual ~COXToolbarSkinXP() { };
    virtual void DrawGripper(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar);
    virtual void DrawNonClientArea(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar);
	virtual void OnCustomDraw(NMHDR*& pNotify, LRESULT*& pResult, COXCoolToolBar* pCoolToolbar);
    virtual void DrawSeparator(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar);
	virtual void DrawCustomizedButton(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar);
	virtual void OnPaintComboBox(COXSkinnedComboBox* pComboBox);
	virtual void DrawComboBox(COXSkinnedComboBox* pComboBox);
	virtual void DrawWndFrame(CWnd* pWnd);
	virtual void DrawMenuButtons(CDC* pDC, COXMenuBar* pMenuBar);

	virtual void DrawBackground(CDC* pDC, COXCoolToolBar* pCoolToolbar);
	virtual void DrawItem(CDC* pDC, TBBUTTON* pTBB, LPCRECT lpRectItem, COXCoolToolBar* pCoolToolbar, BOOL bDrawBackground = TRUE);
	virtual void DrawDroppedItemBackground(CDC* pDC, LPCRECT lpRect);
	virtual BOOL CallOnEraseBkgnd() { return FALSE; };

protected:
	// Methods
	void DrawItemIcon(CDC* pDC, TBBUTTON* pTBB, DWORD dwStyleEx, LPCRECT lpRect, bool bRaised, bool bHot, bool bGrayed, COXCoolToolBar* pCoolToolbar);
	void DrawGripperStripes(CDC* pDC, CRect& rect, bool bHorizontal);
	void DrawItemBackground(CDC* pDC, TBBUTTON* pTBB, DWORD dwStyleEx, LPCRECT lpRect, COLORREF crBorder, COLORREF crInside);
	void DrawItemText(CDC* pDC, TBBUTTON* pTBB, DWORD dwStyleEx, LPCRECT lpRectItem, COLORREF clrText, COXCoolToolBar* pCoolToolbar);
	void DrawInsertMark(CDC* pDC, COXCoolToolBar* pCoolToolbar);
	CRect GetIconRect(DWORD dwCommandID, COXCoolToolBar* pCoolToolbar);
	CRect GetDropArrowRect(CDC* pDC, LPCRECT lpRectItem);

	// Data members
	COXSkinXP* m_pSkinXP;
};

#endif /* COXToolbarSkinXP_h */

// COXToolbarSkin2003.h
//

#if !defined(COXToolbarSkin2003_h)
#define COXToolbarSkin2003_h


class COXSkinXP;

class OX_CLASS_DECL COXToolbarSkin2003 : public COXToolbarSkinXP
{
public:
	COXToolbarSkin2003(COXSkin2003* pSkin2003);
	virtual ~COXToolbarSkin2003();

protected:
	// Methods
    virtual void DrawGripper(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar);
	virtual void DrawBackground(CDC* pDC, COXCoolToolBar* pCoolToolbar);
    virtual void DrawNonClientArea(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar);
    virtual void DrawSeparator(CDC* pDC, LPCRECT lpRect, COXCoolToolBar* pCoolToolbar);
	virtual void DrawMenuButtons(CDC* pDC, COXMenuBar* pMenuBar);
	virtual void DrawDroppedItemBackground(CDC* pDC, LPCRECT lpRect);
	virtual void DrawItem(CDC* pDC, TBBUTTON* pTBB, LPCRECT lpRectItem, COXCoolToolBar* pCoolToolbar, BOOL bDrawBackground = TRUE);
	virtual void DrawComboBox(COXSkinnedComboBox* pComboBox);
	virtual void DrawWndFrame(CWnd* pWnd);

	void DrawGradientItemBackground(CDC* pDC, TBBUTTON* pTBB, DWORD dwStyleEx, LPCRECT lpRect, COLORREF crBorder, COLORREF crInsideLight, COLORREF crInsideDark);

private:


};

#endif /* COXToolbarSkin2003_h */