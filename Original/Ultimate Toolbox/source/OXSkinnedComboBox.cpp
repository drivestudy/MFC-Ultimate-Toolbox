// OXSkinnedComboBox.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXSkinnedComboBox.h"
#include "OXSkins.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <Shlwapi.h>

// This function is used to check the version of comctl32.dll
#define PACKVERSION(major,minor) MAKELONG(minor,major)
DWORD GetDllVersion(LPCTSTR lpszDllName)
{
    HINSTANCE hinstDll;
    DWORD dwVersion = 0;

    hinstDll = LoadLibrary(lpszDllName);
	
    if(hinstDll)
    {
        DLLGETVERSIONPROC pDllGetVersion;
        pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, 
                          "DllGetVersion");

        /*Because some DLLs might not implement this function, you
        must test for it explicitly. Depending on the particular 
        DLL, the lack of a DllGetVersion function can be a useful
        indicator of the version.*/

        if(pDllGetVersion)
        {
            DLLVERSIONINFO dvi;
            HRESULT hr;

            ZeroMemory(&dvi, sizeof(dvi));
            dvi.cbSize = sizeof(dvi);

            hr = (*pDllGetVersion)(&dvi);

            if(SUCCEEDED(hr))
            {
               dwVersion = PACKVERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
            }
        }

        FreeLibrary(hinstDll);
    }
    return dwVersion;
}

/////////////////////////////////////////////////////////////////////////////
// COXSkinnedComboBox

IMPLEMENT_DYNCREATE(COXSkinnedComboBox, CComboBox)

HHOOK COXSkinnedComboBox::m_hMouseHook = NULL;
HWND COXSkinnedComboBox::m_hwndPrevMouseMoveWnd = NULL;

COXSkinnedComboBox::COXSkinnedComboBox() :
	m_pToolbarSkin(NULL)
{
	// Determine if we are ussing common controls 6.0 or not
	if (GetDllVersion(_T("comctl32.dll")) >= PACKVERSION(6,00))
		m_bCommonControls6 = true;
	else
		m_bCommonControls6 = false;
}

COXSkinnedComboBox::~COXSkinnedComboBox()
{
	if (m_pToolbarSkin != NULL)
		delete m_pToolbarSkin;
}


BEGIN_MESSAGE_MAP(COXSkinnedComboBox, CComboBox)
	//{{AFX_MSG_MAP(COXSkinnedComboBox)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_CONTROL_REFLECT_EX(CBN_KILLFOCUS, OnKillFocus)
	ON_CONTROL_REFLECT_EX(CBN_SETFOCUS, OnSetFocus)
	ON_CONTROL_REFLECT_EX(CBN_SELCHANGE, OnSelChange)
	ON_CONTROL_REFLECT_EX(CBN_CLOSEUP, OnCloseUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXSkinnedComboBox message handlers

void COXSkinnedComboBox::OnPaint() 
{
	GetToolbarSkin()->OnPaintComboBox(this);
}

COXToolbarSkin* COXSkinnedComboBox::GetToolbarSkin()
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
}

// Update the combo box when the mouse leaves
LRESULT CALLBACK COXSkinnedComboBox::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return ::CallNextHookEx(m_hMouseHook, nCode, wParam, lParam);

	if (nCode == HC_ACTION && wParam == WM_MOUSEMOVE)
	{
		MOUSEHOOKSTRUCT* pMH = (MOUSEHOOKSTRUCT*) lParam;

		// If the previous message was for COXSizableMiniDockFrameWnd and the current is not
		// we need to update the caption buttons
		COXSkinnedComboBox* pPrev = DYNAMIC_DOWNCAST(COXSkinnedComboBox,
			CWnd::FromHandlePermanent(m_hwndPrevMouseMoveWnd));
		if (pPrev == NULL)
			pPrev = DYNAMIC_DOWNCAST(COXSkinnedComboBox,
				CWnd::FromHandlePermanent(::GetParent(m_hwndPrevMouseMoveWnd)));

		COXSkinnedComboBox* pCurrent = DYNAMIC_DOWNCAST(COXSkinnedComboBox,
			CWnd::FromHandlePermanent(pMH->hwnd));
		if (pCurrent == NULL)
			pCurrent = DYNAMIC_DOWNCAST(COXSkinnedComboBox,
				CWnd::FromHandlePermanent(::GetParent(pMH->hwnd)));

		if (pPrev != NULL && pCurrent != pPrev)
		{
			// The mouse just left the combo box
			pPrev->GetToolbarSkin()->DrawComboBox(pPrev);
		}
		else if (pCurrent != NULL && pCurrent != pPrev)
		{

			// The mouse just entered the combo box
			pCurrent->GetToolbarSkin()->DrawComboBox(pCurrent);
		}

		m_hwndPrevMouseMoveWnd = pMH->hwnd;
	}

	return ::CallNextHookEx(m_hMouseHook, nCode, wParam, lParam);
}

int COXSkinnedComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Hook the mouse
	if (m_hMouseHook == NULL)
		m_hMouseHook = ::SetWindowsHookEx(WH_MOUSE, MouseProc, 0, AfxGetApp()->m_nThreadID);
	
	return 0;
}

void COXSkinnedComboBox::OnDestroy() 
{
	// Unhook the mouse
	if (m_hMouseHook)
	{
		::UnhookWindowsHookEx(m_hMouseHook);
		m_hMouseHook = NULL;
	}

	CComboBox::OnDestroy();
}

BOOL COXSkinnedComboBox::OnKillFocus() 
{
	GetToolbarSkin()->DrawComboBox(this);
	return FALSE;
}

BOOL COXSkinnedComboBox::OnSetFocus() 
{
	GetToolbarSkin()->DrawComboBox(this);
	return FALSE;	
}

BOOL COXSkinnedComboBox::OnSelChange() 
{
	GetToolbarSkin()->DrawComboBox(this);
	return FALSE;	
}

BOOL COXSkinnedComboBox::OnCloseUp() 
{
	GetToolbarSkin()->DrawComboBox(this);
	return FALSE;
}
