// ===================================================================================
// 					Class Specification : COXSizeControlBar
// ===================================================================================

// Header file : OXSizeCtrlBar.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
// Some portions Copyright (C)1994-5	Micro Focus Inc, 2465 East Bayshore Rd, Palo Alto, CA 94303.
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CControlBar

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	NO  Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//
// This class provides the basic behaviour for control bars that can be resized, 
// both when docked and floating. The resizing is free-form, ie there are no 
// discrete steps, and the bar has difference sizes, depending on whether it is 
// docked vertically, horizontally or floating.
// To implement this feature in your application you must:
// 1.	Derive you main frame window (usually CMainFrame) from either 
//		COXFrameWndSizeDock (instead of CFrameWnd, for SDI apps), or 
//		COXMDIFrameWndSizeDock (instead of CMDIFrameWnd, for MDI apps).
// 2.	Derive any specific control bars you need from COXSizeControlBar, 
//		and/or make use of COXSizeDialogBar and COXSizeToolBar.

// You may combine both sizeable (COXSizeControlBar-derived)  and non-sizeable 
// (CControlBar-derived) controls bars in the same application. To best get to 
// grips with writing to these classes, read the implementation summary and 
// look at the example. In general the steps you’ll go through are pretty
// much the same as with MFC’s CControlBar.

// 1.	Construct the MFC COXSize...Bar object, either using new, or on the stack. 
//		Note that the constructor takes a parameter. If you wish to create an 
//		object on the stack, then you may find it easier to use the 
//		SetSizeDockStyle() function, immediately after constructing the
//		object, than specifying a parameter to the constructor.
// 2.	Use Create() to create the bar. 
// 3.	Use EnableDocking().
// 4.	Float or Dock the bar. Generally, CControlBars don’t work with much 
//		sense until you done this step.

// One feature you may find useful in deriving from COXSizeControlBar, is that 
// WM_COMMAND notifications to be handled locally inside the derived class, 
// as well as via the standard routing mechanism. E.g. if you have a button 
// control on a COXSizeControlBar derived class, you can implement the
// ON_COMMAND handler either in your derived class, OR, in the view, frame, etc. 
// as with normal CControlBars.

// Remark:
//

// Prerequisites (necessary conditions):
//		***

/////////////////////////////////////////////////////////////////////////////

#ifndef __SIZECTRLBAR_H__
#define __SIZECTRLBAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"

#ifdef IsMaximized
#undef IsMaximized
#endif

// closing the floating window closes the control bar
#define SZBARF_DESTROY_ON_CLOSE     0x00000001     
// keeps window in an array, so it can be deleted
#define SZBARF_AUTOTIDY    			0x00000002		
										

// standard mouse handling for the menu
#define SZBARF_STDMOUSECLICKS		0x00000004		

// auto-size dialog bars
#define SZBARF_DLGAUTOSIZE			0x00000008		

// allow bar to float in an MDI window
#define SZBARF_ALLOW_MDI_FLOAT		0x00000010		

// will not display gripper
#define SZBARF_NOGRIPPER			0x00000020		

// will not display close button (close button will be displayed unless 
// SZBARF_NOGRIPPER style is specified)
#define SZBARF_NOCLOSEBTN			0x00000040		

// will not display resize button (resize button will be displayed unless 
// SZBARF_NOGRIPPER style is specified)
#define SZBARF_NORESIZEBTN			0x00000080		

// will not display bar caption on the gripper (caption will be displayed unless 
// SZBARF_NOGRIPPER style is specified)
#define SZBARF_NOCAPTION			0x00000100		

// displays the gripper as window caption bar (filled with solid color defined
// for standard windows caption bars)
#define SZBARF_SOLIDGRIPPER			0x00000200		

// Provide Visual Studio .NET style of docking - detachable tab windows, etc.
#define SZBARF_VS7STYLEDOCKING		0x00000400



#define CBRS_MOVED_BY_USER  		0x0001L    // set if dragged by user 


#define ID_CONTAINER_GAP			5
#define ID_BUTTON_SIDE				11
#define ID_TEXT_OFFSET				6
#define ID_TEXTGRIPPER_MARGIN		3
#define ID_GRIPPER_MINSIZE			6

class COXDockbarSkin;
class COXSizeDockBar;

class OX_CLASS_DECL COXSizeControlBar : public CControlBar
{
	friend class COXDockbarSkinClassic;
	friend class COXDockbarSkinXP;
	friend class COXDockbarSkin2003;
	friend class COXSizableMiniDockFrameWnd;
	friend class COXDockTabCtrl;
	friend class COXSkin2003;

//DECLARE_DYNCREATE(COXSizeControlBar)
DECLARE_DYNAMIC(COXSizeControlBar)

// Data members -------------------------------------------------------------
public:

	CSize           m_FloatSize;			// size when floating
   	CSize			m_HorzDockSize;			// size when docked horizontal
	CSize			m_VertDockSize;			// size when docked vertical
	//  - size reflects the current docked size of the window (I do nothing clever for stretched)
	//  - height is generally shared across the rows
   	CSize			m_SavedDockSize;			// size before maximizing

    CPoint			m_FloatingPosition;		// floating position
	DWORD			m_dwAllowDockingState;  // saved enable style for allow docking on/off
    int             m_Style;                // style flags
    CSize           m_PrevSize;
    BOOL            m_bPrevFloating;		// Indicates if control bar is floating
	BOOL			m_bDragging;			// Indicates if the control is currently being dragged
	BOOL			m_bOkToDrag;			// Indicates if it is okay to start dragging
	int				m_iLastTabPosition;		// Position in the tab control when last tabbed

protected:
	CRect			m_rectGripper;			// gripper rect
	CRect			m_rectCloseBtn;			// close button rect
	CRect			m_rectResizeBtn;		// Resize button rect
	CPoint			m_ptOffset;
	BOOL			m_bDelayRecalcLayout;
	BOOL			m_bClientBorder;		// determines whether a client border should be drawn

	typedef enum _tagSIZEBARBTN
	{
		CLOSEBTN=-3,
		RESIZEBTN=-2,
		NONE=-1
	} SIZEBARBTN;

	SIZEBARBTN m_pressedBtn;

	BOOL m_bMaximized;

	BOOL m_bActive;

private :

	static CObArray* m_parrAllocBars;		// array of dynamically allocated bars
	CPoint m_ptLButtonDown;	
	COXDockbarSkin* m_pDockbarSkin;
	COXDockbarSkin* GetDockbarSkin();
	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
	static HHOOK m_hMouseHook;
	static HWND m_hwndPrevMouseMoveWnd;

// Member functions ---------------------------------------------------------
public:
	BOOL IsShown();
	COXDockTabCtrl* GetDockTabCtrl();
	void ShowBar(BOOL bShow);

    COXSizeControlBar(int nStyle = SZBARF_STDMOUSECLICKS);

   	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Clears array of dynamically allocated bars
	static void TidyUp(CFrameWnd* pTopLevelFrame);


	// --- In  : nStyle : the look and feel of the controlbar
	//
	//				SZBARF_DESTROY_ON_CLOSE	When set, clicking the close button on 
	//										the floating frame causes the control bar 
	//										to be closed and destroyed. Normally, 
	//										the control bar would just be hidden.
	//
	//				SZBARF_AUTOTIDY			MFC CControlBar objects cannot be deleted 
	//										from PostNCDestroy(). Normally CControlBar 
	//										objects are allocated within another object 
	//										(e.g. CMainFrame) so the destructors are 
	//										called when that object is deleted. 
	//										If you allocate control bars dynamically 
	//										(as is often useful with sizeable control 
	//										bars), then you will get memory leaks. 
	//										Setting this flag causes all allocated 
	//										COXSizeControlBars to be tracked, and 
	//										deleted automatically when the frame window
	//										is destroyed. 
	//
	//				SZBARF_STDMOUSECLICKS	The control bar will respond to standard 
	//										mouse events, such as double click on the 
	//										title bar to dock, pop up a context menu
	//										to hide or enable/disable docking.  
	//
	//				SZBARF_DLGAUTOSIZE		Use in conjunction with dialog bars. The 
	//										gadgets on the dialog bar will resize to 
	//										fit the new window. This is a simple linear
	//										mapping based on the original sizes & 
	//										positions in the dialog resource, but it 
	//										does work surprisingly well. 
	//
	//				SZBARF_ALLOW_MDI_FLOAT	When set, the context menu will allow the 
	//										user to float the bar as an MDI child 
	//										window.
	//
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructor of object
	//				It will initialize the internal state.
	virtual BOOL Create( LPCTSTR lpszClassName, LPCTSTR lpszWindowName,
		DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, 
		CCreateContext* pContext = NULL);


   	// --- In  : pParent : Points to the parent frame window of the control. Should be derived
	//					   from either COXMDIFrameWndSizeDock, or COXFrameWndSizeDock.
	//			 lpszTitle : Specifies the control’s caption - used in the floating frame window,
	//						 when the bar is undocked.
	//			 nID : the window ID of the controlbar
	//			 dwStyle : The styles of the controlbar (See CControlBar for styles)
	//			 rect : the initial size and placement of the controlbar (in client coordinates)
	// --- Out : 
	// --- Returns : succeeded or not
	// --- Effect : Creates the control bar.  You should use the WS_VISIBLE style when creating
	//				your toolbar, otherwise the initially size may be incorrect.
	BOOL Create(CWnd* pParent, LPCTSTR lpszTitle, UINT nID = 1,
                DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,
                const RECT& rect = CFrameWnd::rectDefault);

	
   	// --- In  : dwStyle : the new style(s) for the controlbar (See constructor for styles)
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets the internal style flag for the control bar. Use this as an alternative
	//				to specifing a parameter on the constructor (except where specifying
	//				SZBARF_AUTOTIDY), and call it immediately after construction, but before Create().
	void SetSizeDockStyle(DWORD dwStyle);


   	// --- In  : dwDockStyle : See CControlBar::EnableDocking
	// --- Out : 
	// --- Returns :
	// --- Effect : This function is equivalent to CControlBar::EnableDocking(), but applies to sizeable
	//				control bars. Note that this function is NOT virtual, so you must take care that you
	//				do not inadvertently call CControlBar::EnableDocking() on a COXSizeControlBar object.
    void EnableDocking(DWORD dwDockStyle);
		

   	// --- In  :
	// --- Out : 
	// --- Returns : TRUE if the control bar is floating (inside a floating frame window).
	//				 Unlike CControlBar::IsFloating(), it returns TRUE if the bar is in
	//				 transition between floating and non-floating. This function proves more
	//				 useful inside the OnSizedOrDocked() overridable.
	// --- Effect : Indicates if control bar is floating
	BOOL IsProbablyFloating();


	// command handlers for standard menus

   	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Called when controlbar is to be hidden
	void OnHide();

   	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : called when docking state is switched
	void OnToggleAllowDocking();

   	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : called when controlbar is floated in an MDI frame
	void OnFloatAsMDI();


	inline BOOL IsGripper() const { return (m_Style & SZBARF_NOGRIPPER)==0; }
	inline BOOL IsCloseBtn() const { return (m_Style & SZBARF_NOCLOSEBTN)==0; }
	inline BOOL IsResizeBtn() const { return (m_Style & SZBARF_NORESIZEBTN)==0; }
	inline BOOL IsCaption() const { return (m_Style & SZBARF_NOCAPTION)==0; }
	inline BOOL IsSolidGripper() const { return (m_Style & SZBARF_SOLIDGRIPPER)!=0; }
	inline void SetClientBorder(BOOL bDrawBorder) { m_bClientBorder = bDrawBorder; }


	inline BOOL IsMaximized() const { return m_bMaximized; }
	inline void SetMaximized(BOOL bMaximized) 
	{ 
		if(m_bMaximized!=bMaximized)
		{
			m_bMaximized=bMaximized; 
			RedrawResizeBtn();
		}
	}


	void SetActive(BOOL bActive, BOOL bResetAllCtrlBars=FALSE);
	inline BOOL IsActive() const { return m_bActive; }

	
	virtual BOOL CanResize() const;

	virtual void EraseNonClient();

// Overrides
	// ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COXSizeControlBar)
    //}}AFX_VIRTUAL

    virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object
    virtual ~COXSizeControlBar();

protected:
    // Overridable functions
	
   	// --- In  : cx : New width of the control bar.
	//			 cy : New height of the control bar.
	//			 bFloating : TRUE if the bar is now floating.
	//			 flags : Reserved.
	// --- Out : 
	// --- Returns :
	// --- Effect : This overridable is called whenever the control bar is sized or docked/undocked.
	//				It is probably the easiest place to respond to resizing the control.
	virtual void OnSizedOrDocked(int cx, int cy, BOOL bFloating, int flags);

	virtual void DrawGripper(CDC* pDC); 
	virtual void DrawCloseBtn(CDC* pDC); 
	virtual void DrawResizeBtn(CDC* pDC);
	
	void RecalcLayout();
	void SaveMouseOffset(CPoint point);
	void RedrawCloseBtn();
	void RedrawResizeBtn();

    //{{AFX_MSG(COXSizeControlBar)
    afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*);
    afx_msg void OnNcPaint();
    afx_msg void OnPaint();
#if _MSC_VER >= 1400
	afx_msg LRESULT OnNcHitTest(CPoint point);
#else
	afx_msg UINT OnNcHitTest(CPoint point);
#endif	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

    virtual LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnAddContextMenuItems(UINT wParam, LPARAM lParam);
	afx_msg LONG OnAfterFloatMessage(UINT, LONG);
    
	DECLARE_MESSAGE_MAP()

private:		
	void RedrawButtons();
};

#endif // __SIZECTRLBAR_H__
