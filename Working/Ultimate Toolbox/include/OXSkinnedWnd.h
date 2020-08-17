
// Version: 9.3

#if !defined(AFX_OXSKINNEDWND_H__6BAD3A54_4046_453C_A79B_4A9BB99FF063__INCLUDED_)
#define AFX_OXSKINNEDWND_H__6BAD3A54_4046_453C_A79B_4A9BB99FF063__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"

// OXSkinnedWnd.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// COXSkinnedWnd window

class COXToolbarSkin;

template<class BASE_CLASS>
class COXSkinnedWnd : public BASE_CLASS
{
	friend class COXToolbarSkinClassic;
	friend class COXToolbarSkinXP;

// Construction
public:
	COXSkinnedWnd()
	{
		m_pToolbarSkin = NULL;
	};

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

// Implementation
public:
	virtual ~COXSkinnedWnd() { };

private:
	COXToolbarSkin* m_pToolbarSkin;
	COXToolbarSkin* GetToolbarSkin();
	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
	static HHOOK m_hMouseHook;
	static HWND m_hwndPrevMouseMoveWnd;
	static CList<HWND, HWND> m_Handles;
};

/////////////////////////////////////////////////////////////////////////////
// COXSkinnedWnd

template<class BASE_CLASS>
HHOOK COXSkinnedWnd<BASE_CLASS>::m_hMouseHook = NULL;

template<class BASE_CLASS>
HWND COXSkinnedWnd<BASE_CLASS>::m_hwndPrevMouseMoveWnd = NULL;

template<class BASE_CLASS>
CList<HWND, HWND> COXSkinnedWnd<BASE_CLASS>::m_Handles;

template<class BASE_CLASS>
COXToolbarSkin* COXSkinnedWnd<BASE_CLASS>::GetToolbarSkin()
{
	// Check if the app is derived from COXSkinnedApp
	COXSkinnedApp* pSkinnedApp = DYNAMIC_DOWNCAST(COXSkinnedApp, AfxGetApp());
	if (pSkinnedApp != NULL && pSkinnedApp->GetCurrentSkin() != NULL)
		return pSkinnedApp->GetCurrentSkin()->GetToolbarSkin();
	else
	{
		// Create a classic skin for this class if not created already
		if (m_pToolbarSkin == NULL)
			m_pToolbarSkin = new COXToolbarSkinClassic();

		return m_pToolbarSkin;
	}
};

// Update the edit box when the mouse leaves
template<class BASE_CLASS>
LRESULT CALLBACK COXSkinnedWnd<BASE_CLASS>::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return ::CallNextHookEx(m_hMouseHook, nCode, wParam, lParam);

	MOUSEHOOKSTRUCT* pMH = (MOUSEHOOKSTRUCT*) lParam;

	if (nCode == HC_ACTION && (wParam == WM_NCMOUSEMOVE || wParam == WM_MOUSEMOVE))
	{
		// If the previous message was for COXSizableMiniDockFrameWnd and the current is not
		// we need to update the caption buttons
		POSITION posPrev = m_Handles.Find(m_hwndPrevMouseMoveWnd);
		COXSkinnedWnd<BASE_CLASS>* pPrev = NULL;
		if (posPrev != NULL)
			pPrev = (COXSkinnedWnd<BASE_CLASS>*) CWnd::FromHandlePermanent(m_Handles.GetAt(posPrev));

		POSITION posCurrent = m_Handles.Find(pMH->hwnd);
		COXSkinnedWnd<BASE_CLASS>* pCurrent = NULL;
		if (posCurrent != NULL)
			pCurrent = (COXSkinnedWnd<BASE_CLASS>*) CWnd::FromHandlePermanent(m_Handles.GetAt(posCurrent));


		if (pPrev != NULL && pCurrent != pPrev)
		{
			// The mouse just left the window
			pPrev->GetToolbarSkin()->DrawWndFrame(pPrev);
		}
		else if (pCurrent != NULL)
		{
			// The mouse just entered the window
			pCurrent->GetToolbarSkin()->DrawWndFrame(pCurrent);
		}

		m_hwndPrevMouseMoveWnd = pMH->hwnd;
	}

	return ::CallNextHookEx(m_hMouseHook, nCode, wParam, lParam);
};


template<class BASE_CLASS>
LRESULT COXSkinnedWnd<BASE_CLASS>::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	// TODO: Add your specialized code here and/or call the base class
	ASSERT_VALID(this);

	
	switch(message) 
	{
	case WM_CREATE:
		{
			// Hook the mouse
			if (m_hMouseHook == NULL)
			{
				m_hMouseHook = ::SetWindowsHookEx(WH_MOUSE, MouseProc, 0, AfxGetApp()->m_nThreadID);
			}

			if (m_Handles.Find(m_hWnd) == NULL)
				m_Handles.AddTail(m_hWnd);
			
			break;
		}

	case WM_DESTROY:
		{
			// Unhook the mouse
			if (m_hMouseHook)
			{
				::UnhookWindowsHookEx(m_hMouseHook);
				m_hMouseHook = NULL;
			}

			POSITION pos = m_Handles.Find(m_hWnd);
			if (pos != NULL)
				m_Handles.RemoveAt(pos);

			break;
		}

	case WM_PAINT:
		{
			LRESULT lResult = BASE_CLASS::WindowProc(message, wParam, lParam);
			GetToolbarSkin()->DrawWndFrame(this);
			
			return lResult;
		}

	case WM_SETFOCUS:
	case WM_KILLFOCUS:
		{
			GetToolbarSkin()->DrawWndFrame(this);
			break;
		}

	default:
		{
			break;
		}
	}

	return BASE_CLASS::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OXSKINNEDWND_H__6BAD3A54_4046_453C_A79B_4A9BB99FF063__INCLUDED_)
