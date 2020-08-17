// ==========================================================================
// 							Class Specification : COXStatusBar
// ==========================================================================

// Header file : status.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                          
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CControlBar and created using CStatusBarCtrl

//	YES	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	YES	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption : extends the CStatusBar class with bitmaps and progress indicator
//				allowed in its panes, Text coloring, font selecting and special
//				cursors when over a pane. Based on MFC 4.X CStatusbar 

// Remark:

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////
#ifndef __XSTATUSBAR_H__
#define __XSTATUSBAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif


#define SBPS_PERCENT 0x2000000	 // New Statusbar pane style for progress bars


enum EOrientation
	{ 
	EO_Undefined 	=   0,
	EO_Stretch		=	1,
	EO_LeftFit		=	2,
	EO_CenterFit	= 	3,
	EO_RightFit		=	4,
	EO_LeftClip		=	5,
	EO_CenterClip	= 	6,
	EO_RightClip	=	7
	};
const EOrientation EO_FIRST = EO_Stretch;	
const EOrientation EO_LAST =  EO_RightClip;

class COXStatusbarSkin;

class OX_CLASS_DECL COXStatusBar : public CStatusBar
{
	DECLARE_DYNCREATE(COXStatusBar)

	friend class COXStatusbarSkinClassic;
	friend class COXStatusbarSkinXP;
// Data members -------------------------------------------------------------
public:
	//{{AFX_DATA(COXStatusBar)
	//}}AFX_DATA
			
protected:
	class OX_CLASS_DECL CBmpInfo : public CObject
		{ 
		public:
			UINT 			m_nIDResource;		  // The resource ID of this bitmap
			CBitmap*		m_pBitmap;			  // The bitmap
			CSize			m_BmpSize;			  // The true size of the bitmap
			EOrientation	m_eBmpOrien;		  // Stretch :	bitmap is drawn stretched in the pane
												  // left		bitmap is drawn left in the pane
												  // middle		bitmap is drawn middle in the pane
												  // right		bitmap is drawn right in the pane
			COLORREF		m_clrMask;

  		};
	
	class OX_CLASS_DECL CEvolInfo : public CObject
	    {
	    public:
			CProgressCtrl	m_ProgressPane;

			unsigned short  m_nPercentDone;
			CRect			m_ProgressRect;
			BOOL			m_bPercentText;
			BOOL			m_bRectInitialized;
	    };

	CDWordArray		m_CursorArray;
    CObArray 		m_PaneBmp;
    CDWordArray     m_ColorArray;		
    CPtrArray      	m_EvolArray;
	CObArray 		m_PaneFont;
	CStringArray	m_TipArray;

   	CDC				m_srcDC;    

// Member functions ---------------------------------------------------------
public:
    // --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object
	//				It will initialize the internal state
    COXStatusBar();

	
	// --- In  :	
	// --- Out : 
	// --- Returns : TRUE if succes, otherwise FALSE
	// --- Effect : 		Sets the number of panes
	//
	BOOL SetIndicators(const UINT FAR* lpIDArray, int nIDCount);

	
	// --- In  :	nIndex : index of the pane in which the cursor is shown
	//				nCursorID : The resource ID of the cursor. If 0, resets to
	//							default stausbar cursor
	// --- Out : 
	// --- Returns : Succeeded or not
	// --- Effect : Setting a cursor to a specific pane causes the mouse cursor
	//				to change into that cursor when moving over the pane.
	BOOL SetPaneCursor(int nIndex, UINT nCursorID = 0);


	// --- In  :	nIndex : index of the pane for which the font is set
	//				pFont : pointer to a CFont. If NULL, resets to default
	//						statusbar font
	// --- Out : 
	// --- Returns : Succeeded or not 
	// --- Effect :  Sets a specific font for a textpane.
	BOOL SetPaneFont(int nIndex, CFont* pFont = NULL);


	// --- In  :	nIndex : index of the pane for which the text is set
	//				lpszNewText : text to be set
	//				clrTextColor : the text color of the pane
	//				bUpdate : Update pane ?
	// --- Out : 
	// --- Returns : Succeeded or not 
	// --- Effect :  Sets a specific text for a textpane.  The clrTextColor color
	//				 determines the color of the text that will be shown.
	BOOL SetPaneText(int nIndex, LPCTSTR lpszNewText, 
		COLORREF clrTextColor = ::GetSysColor(COLOR_BTNTEXT), BOOL bUpdate = TRUE);

	
	// --- In  :	nIdex :			index of the pane in which the bitmap must
	//								be painted						
	//				nIDResource :	resource ID of the bitmap
	//				eBMPOrient :
	//					EO_Streched : Width and height of BMP are stretched to fit the dimensions of the pane.
	//					EO_LeftFit : BMP is placed left in pane. Height of BMP is stretched to fit width of pane. Width remains.
	//					EO_CenterFit : BMP is placed center in pane. Height of BMP is stretched to fit width of pane. Width remains.
	//					EO_RightFit : BMP is placed right in pane. Height of BMP is stretched to fit width of pane. Width remains.
	//					EO_LeftClip : BMP is placed left in pane. Height remains. Width remains.
	//					EO_CenterClip : BMP is placed center in pane. Height remains. Width remains.
	//					EO_RightClip : BMP is placed center in pane. Height remains. Width remains.
	//				clrBkGnd :		Color of transparent color of bitmap
	//				bUpdate :		update pane after set ?
	// --- Out : 
	// --- Returns : TRUE if succes, otherwise FALSE
	// --- Effect : Displays a bitmap in a pane.  It's allways transparent.
	BOOL SetPaneBitmap(int nIndex, UINT nIDResource, 
		EOrientation eBMPOrient = EO_CenterFit,
		COLORREF clrMask = RGB(255,255,255),  
		BOOL bUpdate = TRUE);
	

	// --- In  :	nIndex : index of the pane in which the progress is shown
	//				bUpdate : Update pane ?
	//				dwTotal : The maximum of the process in progress
	//				dwDone : The number of pieces done of the process in progress
	// --- Out : 
	// --- Returns : 
	// --- Effect : Sets the part already processed of a proces in progress 
	void SetBarProgress(int nIndex, BOOL bUpdate=TRUE, DWORD dwTotal=100, 
		DWORD dwDone=0);


	// --- In  : nIndex : index of the pane in which the progress is to be reset
	//			 bDestroy : whether to destroy the ProgressCtrl or not
	// --- Out : 
	// --- Returns :
	// --- Effect : Resets (= Back to zero progress) the progressbar in a pane with index nIndex
	void ResetBar(int nIndex, BOOL bDestroy = FALSE);


	// --- In  : nIndex : index of the pane in which the progress is to be set
	//  		 bUpdate : Update pane ?
	//			 bPercentText : show percent text feedback, in front of ProgressCtrl?
	// --- Out : 
	// --- Returns : TRUE if succes, otherwise FALSE
	// --- Effect : Prepares a pane to begin a progress and to show a progressbar
	BOOL SetUpBar(int nIndex, BOOL bUpdate = TRUE, BOOL bPercentText = TRUE);

	
	// --- In  : bActivate : TRUE if tooltips is to be activated, otherwise FALSE
	// --- Out : 
	// --- Returns : TRUE if succes, otherwise FALSE
	// --- Effect : Enables/disables tooltips on statusbar panes
	BOOL ActivatePaneTips(BOOL bActivate);

	// --- In  : nIndex : index of the pane which tooltip text is to be set
	//  		 lpszNewPaneTip : the new text of the tooltip. may not be NULL
	// --- Out : 
	// --- Returns : TRUE if succes, otherwise FALSE
	// --- Effect : Sets a new pane tip text
	BOOL SetPaneTip(int nIndex, LPCTSTR lpszNewPaneTip);
	
	// --- In  : nIndex : index of the pane
	// --- Out : 
	// --- Returns : success or not
	// --- Effect : Deletes a pane out of the tooltip list
	BOOL DeletePaneTip(int nIndex);


	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect : Neccesary copies to alter functionality
	//				Please refer to the MFC help for documentation
	void SetPaneStyle(int nIndex, UINT nStyle);
	void SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth);

	
	// --- In  : 
	// --- Out : 
	// --- Returns :
	// --- Effect :Destructor of object 
	virtual ~COXStatusBar();

	void UpdateAllPanes(BOOL bUpdateRects, BOOL bUpdateText);

	int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	
// Implementation
protected:
    void PASCAL DrawStatusBmp(CDC *pDC,int nPane, UINT nStyle);
	void DrawTranspBitmap(CDC* pDC, CBmpInfo* pInfo, CRect& rcDrawBMP, CSize& szSrcBMP);

    AFX_STATUSPANE* _GetPanePtr(int nIndex) const;

	BOOL CalcInsideBorder(int nIndex, CRect& PaneRect, BOOL bEvolutionBar=FALSE);
	BOOL CreateEvolutionPane(int nIndex, CEvolInfo* pEvolInfo);

	// Generated message map functions
	//{{AFX_MSG(COXStatusBar)
	afx_msg void OnLButtonDblClk( UINT nFlags, CPoint point );
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnSetMinHeight(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	COXStatusbarSkin* m_pStatusbarSkin;
	COXStatusbarSkin* GetStatusbarSkin();
};

#endif
/////////////////////////////////////////////////////////////////////////////
