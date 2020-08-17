// ===================================================================================
// 					Class Specification : COXSizeDialogBar
// ===================================================================================

// Header file : OXSizeDialogBar.h

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
//	YES	Derived from COXSizeControlBar

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	YES	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	NO	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//
//	This class implements a sizeable dialog bar, that should be entirely compatible with
//	CDialogBar. If you specify the SZBARF_DLGAUTOSIZE on construction, the controls on the
//	dialog bar will be moved and sized to fit within the current size of the dialog bar.
//	This provides a very simple, yet effective form of geometry management. If you don not
//	specify this style, then there is no geometry management facilities, so the controls
//	within the dialog bar remain fixed, and will simply be clipped if the bar is resized
//	too small. You may or course, override OnSizedOrDocked to move the controls around.
//
//	This control should be 100% compatible with CDialogBar. Please see the MFC documentation
//	on CDialogBar for details.

// Remark:
//

// Prerequisites (necessary conditions):
//		***

/////////////////////////////////////////////////////////////////////////////

#ifndef __SIZEDLGBAR_H__
#define __SIZEDLGBAR_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include "OXSizeCtrlBar.h"

typedef void* GADGETRESIZEHANDLE;
GADGETRESIZEHANDLE CreateGadgetResizeHandle(CWnd* pWnd);
void DestroyGadgetResizeHandle(GADGETRESIZEHANDLE Handle);
void ResizeGadgetsOnWindow(GADGETRESIZEHANDLE Handle, int cx, int cy);

class OX_CLASS_DECL COXSizeDialogBar : public COXSizeControlBar
{
DECLARE_DYNCREATE(COXSizeDialogBar)

// Data members -------------------------------------------------------------
public:

protected:
	CSize 					m_sizeDefault;
	GADGETRESIZEHANDLE		m_GadgetResizeHandle;	
	_AFX_OCC_DIALOG_INFO* m_pOccDialogInfo;
  	LPCTSTR					m_lpszTemplateName;

private :

// Member functions ---------------------------------------------------------
public:

	COXSizeDialogBar(int nStyle = SZBARF_STDMOUSECLICKS);
	// --- In  : nStyle : See COXSizeControlBar::COXSizeDialogBar for details
	// --- Out : 
	// --- Returns :
	// --- Effect : Constructor of object
	//				It will initialize the internal state

	BOOL Create(CWnd* pParentWnd, LPCTSTR lpszTemplateName, UINT nStyle, UINT nID);
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : See CDialogBar::Create for details

	BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
		{ return Create(pParentWnd, MAKEINTRESOURCE(nIDTemplate), nStyle, nID); };
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : See CDialogBar::Create for details

    virtual void OnSizedOrDocked(int cx, int cy, BOOL bFloating, int flags);
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : See COXSizeControlBar::OnSizedOrDocked for details

	virtual ~COXSizeDialogBar();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Destructor of object

	virtual BOOL OnInitDialog() 
	{
			ASSERT((GetStyle() & WS_THICKFRAME)==NULL);
		return TRUE; 
	}

protected:
	// support for OCX's inside dialog bars
#ifndef _AFX_NO_OCC_SUPPORT
	// functions necessary for OLE control containment
	virtual BOOL SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* pOccDialogInfo);
	afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
#endif

    //{{AFX_MSG(COXSizeDialogBar)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	DECLARE_MESSAGE_MAP()

private:

};

#endif // __SIZEDLGBAR_H__

