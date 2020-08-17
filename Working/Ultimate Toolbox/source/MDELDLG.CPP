// ==========================================================================
// 						Class Implementation : COXModelessDialog
// ==========================================================================

// Source file :mdeldlg.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                         
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "mdeldlg.h"	// class specification


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXModelessDialog, CDialog)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members


// Data members -------------------------------------------------------------
// protected:
	// BOOL m_bAutoDelete;
	// --- Whether the c++ object should be deleted when the window is destroyed

// private:
	
// Member functions ---------------------------------------------------------
// public:

COXModelessDialog::COXModelessDialog(COXModelessDialog** ppSelf /* = NULL */, BOOL bAutoDelete /* = TRUE */)
	: 
	m_bAutoDelete(bAutoDelete)
	{
	// ... Pointer to pointer to self
    m_ppSelf = ppSelf;                     
	}

void COXModelessDialog::PostNcDestroy() 
	{
	if (m_bAutoDelete)
    	delete this;
	}

void COXModelessDialog::OnOK()
	{
   	if (!UpdateData(TRUE))
   		// Do data exchange
		{
    	TRACE(_T("COXModelessDialog::OnOK : UpdateData failed during dialog termination\n"));
    
       	// ... The UpdateData routine will set focus to correct item
		return;
   		}
   	
   	DestroyWindow();
	}

void COXModelessDialog::OnCancel()
	{
	// ... (without updating data)
    DestroyWindow();       
	}

#ifdef _DEBUG
void COXModelessDialog::Dump(CDumpContext& dc) const
	{
	CDialog::Dump(dc);
	dc << _T("\nm_ppSelf : ") << (const void*)m_ppSelf;
	}
	
void COXModelessDialog::AssertValid() const
	{
	CObject::AssertValid();
    ASSERT(m_ppSelf == NULL || *m_ppSelf == this);
	}
#endif

COXModelessDialog::~COXModelessDialog()
	{
    ASSERT(m_ppSelf == NULL || *m_ppSelf == this);
    if (m_ppSelf != NULL && m_bAutoDelete)
		*m_ppSelf = NULL;
	}
	
// protected:

// private:

// ==========================================================================
