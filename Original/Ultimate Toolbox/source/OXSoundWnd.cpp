// ==========================================================================
//				Class Implementation : COXSoundWnd
// ==========================================================================

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
			  
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXSoundWnd.h"
#include "OXSound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXSoundWnd, CWnd)

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Definition of static members
COXSoundWnd	COXSoundWnd::m_theSoundWnd;

// Data members -------------------------------------------------------------
// protected:
	// static COXSoundWnd m_theSoundWnd;
	// The one and only object of this class

// private:
	
// Member functions ---------------------------------------------------------
// public:

BEGIN_MESSAGE_MAP(COXSoundWnd, CWnd)
	//{{AFX_MSG_MAP(CWnd)
	ON_MESSAGE(WM_OX_INTERNAL_SOUNDCALLBACK, OnInternalSoundCallback)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

COXSoundWnd::COXSoundWnd()
	{
	ASSERT_VALID(this);
	}

HWND COXSoundWnd::CreateTheSoundWindow()
	{
	if ((m_theSoundWnd.m_hWnd == NULL) && !m_theSoundWnd.CreateEx(0, AfxRegisterWndClass(0),
		_T("COXSoundWnd Notification Sink"), WS_OVERLAPPED, 0, 0, 0, 0, NULL, NULL))
		{
		TRACE0("COXSoundWnd::CreateTheSoundWindow: Unable to create COXSoundWnd notify window\n");
		TRACE0("COXSoundWnd::CreateTheSoundWindow: Throwing resource exception\n");
		AfxThrowResourceException();
		}

	ASSERT(m_theSoundWnd.m_hWnd != NULL);
	ASSERT(CWnd::FromHandlePermanent(m_theSoundWnd.m_hWnd) == &m_theSoundWnd);

	ASSERT_VALID(&m_theSoundWnd);
	return m_theSoundWnd.m_hWnd;
	}

HWND COXSoundWnd::GetTheSoundWindow()
	{
#ifdef _DEBUG
	if (m_theSoundWnd.m_hWnd == NULL)
		TRACE0("COXSoundWnd::GetTheSoundWindow: COXSoundWnd notify window has not been created yet, returning NULL\n");
#endif // _DEBUG

	return m_theSoundWnd.m_hWnd;
	}

LRESULT COXSoundWnd::OnInternalSoundCallback(WPARAM wParam, LPARAM /* lParam */)
	{
	ASSERT_VALID(this);

	COXSound* pSound = (COXSound*)wParam;
	DWORD nDummy = 0;
	// ... Check wheter the pointer still points to a valid object (may have been deleted in the meanwhile)
	if (COXSound::m_allSoundObjects.Lookup(pSound, nDummy))
		{
		ASSERT_VALID(pSound);
		// Have the COXSound class handle the callback
		pSound->HandleCallback();
		}
	else
		TRACE(_T("COXSoundWnd::OnInternalSoundCallback: Ignoring callback of non-existing COXSound object : address 0x%X\n"), (DWORD_PTR)pSound);

	return 0;
	}

#ifdef _DEBUG
void COXSoundWnd::AssertValid() const
	{
	// ... There should be only one object of this class
	ASSERT(&m_theSoundWnd == this);
	CWnd::AssertValid();
	}

void COXSoundWnd::Dump(CDumpContext& dc) const
	{
	CWnd::Dump(dc);
	}
#endif //_DEBUG

COXSoundWnd::~COXSoundWnd()
	{
	if (GetSafeHwnd() != NULL)
		{
		// ... Sestroy the window when this object is deleted
		DestroyWindow();
		}
	}

// protected:
// private:

// ==========================================================================
