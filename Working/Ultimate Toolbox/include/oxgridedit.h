// ==========================================================================
// 					Class Specification : COXGridEdit
// ==========================================================================

// Header file : OXGridEdit.h

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
//	YES	Derived from CEdit

//	NO	Is a Cwnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :       
//	This class is used to subclass the edit control of a list control (during editing)  

// Remark:
//	Most functions are only interesting for a subclassed edit control of the list control

//	Setting the window text of the edit control right after it has been subclassed
//   seems to destroy (!) the edit control, therefore SetDeferedWindowText will
//	 postpone this until the first OnSize event is generated.
//	Setting the window text earlier (e.g. OnSetFocus, OnShowWindow etc) will
//	 also destroy the window
//	This behaviuor was detected on Win NT 3.51

// The editing can be ended by several different keys
//		Enter : keep the changes
//		Esc : Discard the changes
//		Tab or Insert : Keep the changes and start editing the next subitem in the list control
//		Shift Tab : Keep the changes and start editing the previous subitem in the list control
//	These last two ways are additions of these class, the list control can
//	differentiate between them by calling GetEndTabKey()

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXGRIDEDIT_H__
#define __OXGRIDEDIT_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"


class OX_CLASS_DECL COXGridEdit : public CEdit
{
DECLARE_DYNAMIC(COXGridEdit);

// Data members -------------------------------------------------------------
public:
protected:
	CString m_sDeferedWindowText;
	BOOL m_bDeferedWindowText;
	CPoint m_ptWindowPos;
	BOOL m_bWindowPos;
	int m_nWindowHeight;
	BOOL m_bWindowHeight;
	int m_nCXOffset;
	BOOL m_bAdjustWindowWidth;
	UINT m_nEndKeyChar;
	BOOL m_bEndKeyShift;
	BOOL m_bEndKeyCtrl;
	int	m_nRightParentBorder;
	BOOL m_bFitToClient;

private:
	
// Member functions ---------------------------------------------------------
public:
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructs the object
	COXGridEdit();

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Initializes the object
	//				This function should be called before subclassing an edit control
	void Initialize();

	// --- In  : lpszString : The new window text
	// --- Out : 
	// --- Returns :
	// --- Effect : Set the new window text of this control
	//				This text will only be set during the next OnSize event
	void SetDeferedWindowText(LPCTSTR lpszString);

	// --- In  : pt : The new top left corner of the control
	// --- Out : 
	// --- Returns :
	// --- Effect : This new position will be used to overide every other position
	//				during a OnWindowPosChanging
	void SetWindowPos(const CPoint& pt);

	// --- In  : nHeight : The new height of the control
	// --- Out : 
	// --- Returns :
	// --- Effect : This height will be used to overide every other height
	//				during a OnWindowPosChanging
	void SetWindowHeight(int nHeight);

	// --- In  : nWidth : An offset that will be added to width of the window text
	//					  to calculate the width of the edit control
	// --- Out : 
	// --- Returns :
	// --- Effect : By setting this additional width, the origianl width will be discarded
	//				The width will be calculated from the width of the window text
	//				and the nWidth will be added to this number
	void AdjustWindowWidth(int nWidth);

	// --- In  :
	// --- Out : nChar : Virtual key code of the key that ended the editing
	//			 bShift : Whether Shift was pressed as well
	//			 bCtrl : Whether Ctrl was pressed as well
	// --- Returns : Whether the editing was ended by pressing a key
	// --- Effect : 
	BOOL GetEndKey(UINT& nChar, BOOL& bShift, BOOL& bCtrl);


	// --- In  :	bFitToClient	-	TRUE if the size of the control must be 
	//									adjusted to fill all client area of the 
	//									parent window to the right. Otherwise
	//									the size of the control will be adjusted 
	//									to fit the text size.
	// --- Out : 
	// --- Returns:
	// --- Effect : sets the flag that specifies the way the size of the control 
	//				is adjusted
	inline void SetFitToClient(BOOL bFitToClient) { m_bFitToClient=bFitToClient; }

	// --- In  :	
	// --- Out : 
	// --- Returns:	TRUE if the size of the control must be adjusted to fill all 
	//				client area of the parent window to the right. Otherwise
	//				the size of the control will be adjusted to fit the text size.
	// --- Effect : retrieves the flag that specifies the way the size of the control 
	//				is adjusted
	inline BOOL GetFitToClient() const { return m_bFitToClient; }


#ifdef _DEBUG
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : AssertValid performs a validity check on this object 
	//				by checking its internal state. 
	//				In the Debug version of the library, AssertValid may assert and 
	//				thus terminate the program.
	virtual void AssertValid() const;
	
	// --- In  : dc : The diagnostic dump context for dumping, usually afxDump.
	// --- Out : 
	// --- Returns :
	// --- Effect : Dumps the contents of the object to a CDumpContext object. 
	//				It provides diagnostic services for yourself and 
	//				 other users of your class. 
	//				Note  The Dump function does not print a newline character
	//				 at the end of its output. 
	virtual void Dump(CDumpContext& dc) const;
#endif

	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of the object
	virtual ~COXGridEdit();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COXGridEdit)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(COXGridEdit)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
                   
};

#endif // __OXGRIDEDIT_H__
// ==========================================================================
