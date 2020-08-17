// ==========================================================================
// 					Class Specification : COXSplashWnd
// ==========================================================================
//
// Header file : OXSplashWnd.h
//
// Version: 9.3
//
// Properties:
//		NO		 Abstract class (does not have any objects)
//		YES		 Derived from CWnd
//
//		YES		 Is a CWnd.                     
//		YES		 Two stage creation (constructor & Create())
//		YES		 Has a message map
//		YES		 Needs a resource (template)
//
//		NO		 Persistent objects (saveable on disk)      
//		NO		 Uses exceptions
//
// Description:
//
//		COXSplashWnd is a class that allows a programmer to show an 
//		initial window when the applications starts. A bitmap (even 256-
//		color) can be shown within this window. After a short period of
//		time (e.g. 0.8 seconds) the window will disappear. This can be 
//		used to show an initial logo, copyright information etc.
//		A special feature of this class allows the window region to be
//		fitted according to the bounding region of the bitmap. So when the
//		bitmap is not rectangular, neither will be the window. This 
//		bounding path is calculated based upon color differences. 
//		"Holes" within the bitmap are not supported.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __OXSPLASHWND_H__
#define __OXSPLASHWND_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#ifndef CLR_NONE
#define CLR_NONE 0xFFFFFFFFL
#endif // CLR_NONE

#ifndef CLR_DEFAULT
#define CLR_DEFAULT 0xFF000000L
#endif // CLR_DEFAULT

#define SPLASH_NOTIMER -1		// flag for Show() function, splash windows stays
#define OXSPLASHWND_TIMERID_HIDE 1

#include "OXSplashWndDIB.h"


/////////////////////////////////////////////////////////////////////////////
class OX_CLASS_DECL COXSplashWnd : public CWnd
{
// Data members -------------------------------------------------------------
public:
	static int m_nDefaultTimeout;
	// --- Default time out value for splash window (in milliseconds)

protected:
	static COXSplashWnd* c_pGlobalWnd;
	static BOOL c_bShowSplashWnd;

	int m_nTimeout;
	BOOL m_bAutoDelete;

	COXSplashWndDIB m_dib;
	CRgn m_rgn;
	COLORREF m_crBorder;
	BYTE m_crBorderR;
	BYTE m_crBorderG;
	BYTE m_crBorderB;
	BYTE m_nTolerance;

	CWnd m_invisParent;

	BOOL m_bUserCancel;

private:

// Member functions ---------------------------------------------------------
public:
	COXSplashWnd();
	// --- In      :
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Constructor of object

	virtual ~COXSplashWnd();
	// --- In      :
	// --- Out     : 
	// --- Returns :
	// --- Effect  : Destructor of object

	BOOL GetAutoDelete() const;
	// --- In      :
	// --- Out     : 
	// --- Returns : AutoDelete setting
	// --- Effect  : retrieve AutoDelete setting. TRUE if the C++ object
	//				 (COXSplashWnd) should be deleted automatically when
	//				 the Windows's window is destroyed. FALSE otherwise.
	//				 Default value is (1) FALSE when an explicit object was
	//				 created (a regular object); (2) TRUE when static function
	//				 was called to implicitly create an object (a global
	//				 splash window)

	void SetAutoDelete(BOOL bAutoDelete = TRUE);
	// --- In      : bAutoDelete, the new AutoDelete setting
	// --- Out     : 
	// --- Returns :
	// --- Effect  : specifies whether the C++ object (COXSplashWnd) should
	//				 be deleted automatically when the Windows's window is 
	//				 destroyed.

	void SetUserCancel(BOOL bUserCancel = TRUE);
	// --- In      : bUserCancel	-	if TRUE then user can cancel displaying
	//									of splash window by clicking any mouse 
	//									button or pressing any button on a keyboard
	// --- Out     : 
	// --- Returns :
	// --- Effect  : specifies whether splash window will be hidden in result of 
	//				 clicking any mouse button or pressing any button on a keyboard
	//				 by an user. Set to TRUE by default.
	BOOL GetUserCancel();
	// --- In      : 
	// --- Out     : 
	// --- Returns : TRUE if splash window will be hidden in result of clicking any 
	//               mouse button or pressing any button on a keyboard by an user,
	//				 or FALSE otherwise.
	// --- Effect  : 

	virtual BOOL LoadBitmap(LPCTSTR lpszResourceName, 
		COLORREF crBorder = CLR_DEFAULT, LPPOINT pStartPoint = NULL, 
		BYTE nTolerance = 0);
	virtual BOOL LoadBitmap(UINT nIDResource, COLORREF crBorder = CLR_DEFAULT, 
		LPPOINT pStartPoint = NULL, BYTE nTolerance = 0);
	virtual BOOL LoadBitmapFile(LPCTSTR lpszFileName, 
		COLORREF crBorder = CLR_DEFAULT, LPPOINT pStartPoint = NULL, 
		BYTE nTolerance = 0);
	// --- In      : lpszResourceName, nIDResource, the bitmap resource
	//				 crBorder, indicates which color should be used as 
	//					border color. Every outer pixel in this color 
	//					will be made invisible. This will create the 
	//					necessary shape of the splash window. You can 
	//					either specify the color or use one of the two 
	//					special values:
	//						CLR_NONE:	no border color needs to be used.
	//									The bitmap will just be shown as a 
	//									rectangle bitmap.
	//						CLR_DEFAULT:the color of the pixel specified 
	//									by pStartPoint should be used.
	//				 pStartPoint, indicates which pixel within the bitmap
	//					should be used to start looking for the actual border.
	//					When no starting point is specified, (0,0) is used 
	//					by default.
	//				 nTolerance, indicates how much the color has to be 
	//					different from crBorder before it is actaully accepted
	//					as a different color. A tolerance of 0 indicates that
	//					all colors except an exact match with crBorder are 
	//					treated as different.
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise.
	// --- Effect  : Loads the bitmap from resource, and specify parameters for
	//				 determining the shape (visible-invisible boundary).

	virtual BOOL Show(int nTimeOut = m_nDefaultTimeout, CWnd* pParentWnd = NULL, 
		CPoint* pptDisplayAt = NULL);
	// --- In      : nTimeOut, indicates how long the window should exist
	//					(in milliseconds).  If this nTimeout is SPLASH_NOTIMER, no
	//					timer is set and the user must take care of the hiding.  Can
	//					be used in conjunction with the Hide() function.
	//				 pParentWnd, the parent window	if NULL is specified the splash
	//					window is a toplevel window with no parent window
	//				 pptDisplayAt, pointer to the point on the screen at which splash 
	//					window should be displayed, if it's NULL then splash window
	//					will be centered
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE if no valid bitmap was loaded.
	// --- Effect  : Display the splash window.

	virtual BOOL Hide(int nTimeOut = 0);
	// --- In      : nTimeOut, indicates how long the window should exist
	//					(in milliseconds)and be visible after calling this
	//					function.
	// --- Out     : 
	// --- Returns : TRUE if successful;
	// --- Effect  : Hides and destructs the the splash window after a certain time.
	
	BOOL GetRegion(CRgn& rgn);
	// --- In      :
	// --- Out     : rgn, a copy of the previously calculated region
	// --- Returns : whether a valid region has been returned or not.
	// --- Effect  : When the class has already calculated the border region 
	//				 (in LoadBitmap()) a copy of the region will be returned
	//				 by this function.

	BOOL SetRegion(CRgn& rgn);
	// --- In      : rgn, the region describing the shape of the bitmap to
	//					display
	// --- Out     : 
	// --- Returns : TRUE if successful; FALSE otherwise
	// --- Effect  : Pass an existing region to the COXSplashWnd object.
	//				 The region will be copied, so destruction of rgn is
	//				 the caller's resposibility.

	COXSplashWndDIB* GetDIB();
	// --- In     : 
	// --- Out    : 
	// --- Returns:	pointer to COXSplashWndDIB object used internally to display
	//				loaded bitmap resource.
	// --- Effect : Retrieves internal COXSplashWndDIB object. Refer to the 
	//				documentation on COXSplashWndDIB class for full reference 
	//				of all its public functions

	
	// Global Functions:
	//		 The class contains one static pointer to an object of the class.
	//		 This can be useful, because normally an application uses only one
	//		 splash window (at startup). All functions that have the word 
	//		 "Global" in their name act on this object. The meaning is the same
	//		 as the counterpart without "Global" in its function name.
	//	NOTE:when LoadGlobalBitmap() is called, an COXSplashWnd object
	//		 was created and the AutoDelete attribute will be set to TRUE,
	//		 meaning the C++ object will be deleted automatically when the 
	//		 window is destroyed.
	//		 Calling ShowGlobal() without a previous successful call to 
	//		 LoadGlobalBitmap() will not succeed. A failure of LoadGlobalBitmap()
	//		 could be due to a not-found bitmap, or an empty region was calculated
	//		 from the loading process. A TRACE message from the LoadGlobalBitmap()
	//		 would describe the reason why it failed.

	static void EnableSplashScreen(BOOL bEnable /*= TRUE*/);
	// --- In      : bEnable, specify whether global splash window will
	//				 be enabled
	// --- Out     : 
	// --- Returns :
	// --- Effect  : enable/disable global splash window.
	//				 NOTE: This function can be called before
	//					LoadGlobalBitmap(), and the global window will not 
	//					be created only after LoadGlobalBitmap() succeeds.
	//					This function has no effects on a regular object as
	//					opposed to the global object created through 
	//					LoadGlobalBitmap().
	//				 A common use of this function would be in
	//					CMyApp::InitInstance():
	//					...
	//					CCommandLineInfo cmdInfo;
	//					ParseCommandLine(cmdInfo);
	//					COXSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);

	static COXSplashWnd* GetGlobalWnd();
	// --- In      :
	// --- Out     : 
	// --- Returns : pointer to the global COXSplashWnd object
	// --- Effect  : obtain the pointer to the global COXSplashWnd object 

	static BOOL LoadGlobalBitmap(LPCTSTR lpszResourceName, 
		COLORREF crBorder=CLR_DEFAULT, LPPOINT pStartPoint=NULL, 
		BYTE nTolerance=0);
	static BOOL LoadGlobalBitmap(UINT nIDResource, COLORREF crBorder=CLR_DEFAULT, 
		LPPOINT pStartPoint=NULL, BYTE nTolerance=0);
	static BOOL LoadGlobalBitmapFile(LPCTSTR lpszFileName, 
		COLORREF crBorder=CLR_DEFAULT, LPPOINT pStartPoint=NULL, 
		BYTE nTolerance=0);
	static BOOL ShowGlobal(int nTimeOut = m_nDefaultTimeout, CWnd* pParentWnd = NULL, 
		CPoint* pptDisplayAt = NULL);
	static BOOL HideGlobal(int nTimeOut = 0);
	static BOOL GetGlobalRegion (CRgn& rgn);
	static BOOL SetGlobalRegion (CRgn& rgn);

	//{{AFX_VIRTUAL(COXSplashWnd)
	protected:
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	virtual BOOL BuildRegion(POINT ptStart);
	virtual BOOL IsBorder(POINT pt, BOOL bConsiderInvalidAsBorder = TRUE);
	
	//{{AFX_MSG(COXSplashWnd)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
};

// inline functions
#include "OXSplashWnd.inl"

#endif	// __OXSPLASHWND_H__

// end of OXSplashWnd.h
/////////////////////////////////////////////////////////////////////////////
