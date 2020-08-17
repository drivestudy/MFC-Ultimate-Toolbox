// ==========================================================================
// 					Class Specification : COXRollup
// ==========================================================================

// Header file : oxrollup.h

// Version: 9.3

//
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
//
// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	YES	Derived from CDialog

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	YES 	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	This class can be used to have an interface like CorelDraw.
//	It is a Rollup dialog.  Dialog with a special caption and that can be rolled up (only
//	caption is visible) or rolled out (caption + dialog + controls are visible)

// Remark:
//		***
// Prerequisites (necessary conditions):
//		***

// This class needs the bmp resource with the ID TITLEBAR_BMP
// It also needs the menuID's IDM_RU_ABOUT, IDM_RU_CLOSE, IDM_RU_ROLLUP , IDM_RU_ROLLDOWN,
// IDM_RU_ARRANGE and IDM_RU_ARRANGEALL

//	This file uses resources.
//	The reserved ID ranges are : 23000 -> 23019 and 53000 -> 53019

/////////////////////////////////////////////////////////////////////////////

#ifndef __ROLLUP_H__
#define __ROLLUP_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"

typedef struct tagSCROLL_HELP
	{
	int decrement;
	int width;
	int height;
	int capHeight;
	int steps;
	int curstep;
	int direction;
	}
SCROLL_STRUCT;

//important defines

#define ID_PRIVATEMSG (IDNO + 0x0039)

#define DELAY_TIME		5
#define ROLL_STATES		5
#define TBAR_HEIGHT		14

// for notification messages; message map macro; use it!!
#ifdef _WIN32
	#define ON_ROLLUP_NOTIFICATION() \
		{ WM_LMSUROLLUP, 0, 0, 0, AfxSig_bWww, \
			(AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(CWnd*, UINT, UINT))OnRollupMessage },
#else
	#define ON_ROLLUP_NOTIFICATION() \
		{ WM_LMSUROLLUP, 0, AfxSig_bWww, \
			(AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(CWnd*, UINT, UINT))OnRollupMessage },
#endif

//  Message Map:
//
//	afx_msg BOOL OnRollupMessage(CWnd* pWndRollup,UINT message,UINT rollupID);
//

// Parent/MR ---> Dialog
#define ID_ROLLUP		(ID_PRIVATEMSG + 2)	// View->Dlg
#define ID_ROLLDOWN		(ID_PRIVATEMSG + 3)	// View->Dlg

// Message ID (intended, no must)
// IDCANCEL		delivers close message to parent
// is defined an is!!!! used
// you can use furthermore: IDOK,IDABORT,IDRETRY,IDIGNORE,
// IDYES,IDNO
#define ID_APPLY		(ID_PRIVATEMSG + 4)	// Dlg->View
#define ID_CHANGED		(ID_PRIVATEMSG + 5)	// Dlg->View

#include "OXTleBar.h"


class OX_CLASS_DECL COXRollup : public CDialog
{
	friend class COXTitleBar;		// no must, but convenient
	DECLARE_DYNAMIC(COXRollup);

// Data members -------------------------------------------------------------
public:

protected:
	COXTitleBar*	m_pTitleBar;		// Titlebar window
	BOOL			m_bRolledUp;		// rolled up?
	BOOL			m_bResizingFrame;	// do we have a resizing frame?
	
	// variables to control dragging
	UINT			m_myTimerID;		// timer id used for rolling up and down
	
	int				m_xWidth;
	int				m_yHeight;			// stored heights and widths of rollup (for resizeable)
	CMenu*			m_pSysMenu;			// pointer to system menu
	
	WORD			m_nRollup;			// ID of our dialog, external
	
	UINT			m_nTemplateID;		// Ressource ID of dialog
	
	HWND			m_hWndMR;			// window getting messages (currently)
	HWND			m_hWndDestroyRC;	// window that gets call to destroy rollup	
	CString			m_sCaption;			// caption of Rollup

	int				m_nScrollSteps;		// the number of steps to be used for rolling up/down

	BOOL			m_bRolling;

private:
	static CPtrList m_RollupList;		// list for our created rollups
	static CPtrList m_ArrangedRollups;
	BOOL			m_bIsArranged;

	SCROLL_STRUCT	m_SCROLL_HELP;

// Member functions ---------------------------------------------------------
public:
	COXRollup(UINT nTemplate, CWnd* pParent = NULL);
	// --- In  : nTemplate : the resource template
	//			 pParent : the window parent of the rollup dialog
	// --- Out : 
	// --- Returns :
	// --- Effect : Contructor of object
	//				It will initialize the internal state

	void RemoveFromRUList();
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : Called by destructor to remove it from active rollup list.
	//				Call if you don´t want your Rollup available to Arrange All

	void InternalRollUp();
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : Used primarily by Arrange/ArrangeAll. Use it if you want to quickly scroll up
	//				the rollup (for example after a call to CreateRollUp(...) to show it
	//				initially rolled up)

	BOOL IsRolling();
	// --- In  : 
	// --- Out : 
	// --- Returns : TRUE if the rollup is rolling
	// --- Effect : 

	static void ReArrangeArranged();
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : Call it from CMainFrame::OnSize(...) and CMainFrame::OnMove(...)to guarantee that
	//				all former arranged Rollups will align to upper left corner of CMainFrame after
	//				sizing or moving.

	BOOL IsArranged();
	// --- In  : 
	// --- Out : 
	// --- Returns : arranged or not
	// --- Effect : Call to obtain information about arrange-state

	void UnArrange();
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : Call if you want to remove the rollup from the arranged list.
	//				In contrast with RemoveFromRUList() this function will not affect
	//				the rollup to be arranged again after a new call to ArrangeAll().

	BOOL CreateRollUp(CWnd* pParent, WORD nID, int nIDCaption=IDS_OX_ROLLUPCAPTION); //"No Caption!"
	BOOL CreateRollUp(CWnd* pParent, WORD nID, LPCTSTR lpszCaption); 
	// --- In  : pParent : Pointer to the parent (destroyer)
	//			 nID : Simple number to identify later which Rollup sent the message.
	//			 lpcstrCaption : Caption of dialog
	// --- Out : 
	// --- Returns : suceeded or not
	// --- Effect : Call this function to create the rollup dialog

	virtual COXTitleBar* OnGetTitleBar();
	// --- In  : 
	// --- Out : 
	// --- Returns : the titlebar
	// --- Effect : If overriding this function allocate storage for your derived object,
	//				cast it to COXTitleBar. This is the function to hook your own titlebar
	//				in the Rollup system. See the overrideables in the COXTitleBar class.

	virtual void GetTitleBarRect(CRect &rcTBarRect);
	// --- In  : 
	// --- Out : rcTBarRect : the rectangle of the titlebar (+1 pt border)
	// --- Returns : 
	// --- Effect : Is called after OnGetTitlebar. Pass in the size you want for your Titlebar.

	virtual LPCTSTR GetTitleBarBitmap();
	// --- In  : 
	// --- Out : 
	// --- Returns : a resource string
	// --- Effect : Return the Resource string of this Rollup

	// managing the message dispatch
    HWND SetRecipient(CWnd* pWnd);
	// --- In  : pWnd : Pointer to the new message recipient
	// --- Out : 
	// --- Returns : the window handle of the old message recipient
	// --- Effect : Call this function from an OnSetFocus handler to notify the Rollup of its
	//				new address to send its messages. Store return value for ongoing „talk" between Rollup
	//				and message recipient

	LRESULT	Send2MR(WORD msg);
	// --- In  : msg : message to be send
	// --- Out : 
	// --- Returns : message return
	// --- Effect : Sends the specified message to the current message recipient.

	BOOL Post2MR(WORD msg);
	// --- In  : msg : message to be send
	// --- Out : 
	// --- Returns : If the function succeeds, the return value is TRUE.
	//				 If the function fails, the return value is FALSE. 
	// --- Effect : posts the specified message to the current message recipient.

	HWND GetCurMsgRecipient();
	// --- In  : 
	// --- Out : 
	// --- Returns : the window handle of the current message recipient
	// --- Effect : 
		  
	void ReleaseRecipient(const CWnd* pRCRequesting, BOOL bCallNotifyFunction = FALSE);
	// --- In  : pRCRequesting : pointer to the window requesting to be released as recipient
	//			 bCallNotifyFunction : whether or not to be notified of release
	// --- Out : 
	// --- Returns : 
	// --- Effect : Call this function to dismiss yourself as message recipient.

    virtual void OnRecipientRelease()
		{};
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : virtual called during ReleaseRecipient if BOOLEAN bCallNotifyFunction is TRUE

    BOOL IsOwner(const CWnd* pRecipWnd) const;
	// --- In  : pRecipWnd : a window pointer
	// --- Out : 
	// --- Returns : whether or not pRecipWnd is the current message recipient				
	// --- Effect : Check if you are the owner of the message dispatch

    // helpers for frame/rolling
    BOOL IsRolledUp();
	// --- In  : 
	// --- Out : 
	// --- Returns : rolled up or not
	// --- Effect : Call this function to get the rollup-state
	//				(TRUE == rolled up, FALSE == rolled down)

	BOOL CanResize();       
	// --- In  : 
	// --- Out : 
	// --- Returns : whether or not the rollup is resizble (WS_THICKFRAME or not)
	// --- Effect : 

	// special UpdateData function; see implementation
	BOOL RUpdateData(BOOL bUpdateFlag);
	// --- In  : 
	// --- Out : 
	// --- Returns : see CWnd::UpdateData(...)
	// --- Effect : 

	WORD GetRollupID();
	// --- In  : 
	// --- Out : 
	// --- Returns : unique Rollup ID 
	// --- Effect : retrieve unique Rollup ID when you have more than one Rollup in use
	//				Passed at creation time : CreateRollup(...)

	int GetScrollSteps()
		{ return m_nScrollSteps; }
	// --- In  : 
	// --- Out : 
	// --- Returns : the number of steps used when scrolling down/up
	// --- Effect : 

	void SetScrollSteps(int nScrollSteps)
		{ m_nScrollSteps = nScrollSteps; }
	// --- In  : nSCrollSteps : the number of steps to be used when scrolling down/up
	// --- Out : 
	// --- Returns : 
	// --- Effect : 

	void OnCancel()
		{};	// just do nothing
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : 

    void OnOK()
		{};		// again: just do nothing
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : 

	// additional menu functions
	CMenu* GetSysMenu();
	// --- In  : 
	// --- Out : 
	// --- Returns : the menu attached to this rollup
	// --- Effect : 

    virtual ~COXRollup();
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : destructor of the object

protected:
	// advanced overrideables
	virtual void OnProcessSysMenu();
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : Override this function to do your own menu processing

	virtual CMenu* OnGetSysMenu();
	// --- In  : 
	// --- Out : 
	// --- Returns : 
	// --- Effect : Override this function to provide an alternative system
	//				menu for your Rollup

	virtual void PostNcDestroy();

	// helper functions
	void SetArrangeFlagTrue();
    void DetermineFrameStyle();
	void DrawCaption(int nDisplayElement);
  
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CRollup)
	afx_msg void OnClickedRoll();
	virtual void OnCloseRollup();
	afx_msg LRESULT OnRollMessage(WPARAM,LPARAM);
	afx_msg void OnRollUpAbout();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivate(UINT,CWnd*,BOOL);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void Arrange();
	afx_msg void ArrangeAll();
	afx_msg void OnDestroy();
	afx_msg void OnNcPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// inlined functions
inline BOOL COXRollup::IsRolledUp()
	{ 	return m_bRolledUp; }

inline BOOL COXRollup::IsOwner(const CWnd* pRecipWnd) const
	{
	ASSERT(pRecipWnd != NULL && IsWindow(pRecipWnd->GetSafeHwnd()));
	return (m_hWndMR == pRecipWnd->GetSafeHwnd());
	}

inline BOOL COXRollup::CanResize() 
	{ 	return m_bResizingFrame; }

inline BOOL COXRollup::RUpdateData(BOOL bUpdateFlag)
	{	
	if (GetSafeHwnd() == NULL)
		return FALSE;
	
	return UpdateData(bUpdateFlag);
	}  

inline WORD COXRollup::GetRollupID()
	{	return m_nRollup; }

inline LPCTSTR COXRollup::GetTitleBarBitmap()
	{	
	CString sTitle;
	VERIFY(sTitle.LoadString(IDS_OX_ROLLUPBITMAP));
	return sTitle;
	}

inline BOOL COXRollup::IsArranged()
	{ return m_bIsArranged; }

inline void COXRollup::SetArrangeFlagTrue()
	{ m_bIsArranged = TRUE; }

#endif

// ==========================================================================
