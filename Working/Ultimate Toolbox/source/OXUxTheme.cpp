// ==========================================================================
//				Class Implementation : COXUxTheme
// ==========================================================================

// Source file: OXUxTheme.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

#include "stdafx.h"
#include "OXUxTheme.h"
#include <comdef.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COXUxTheme::COXUxTheme()
{
	m_hThemeDLL = ::LoadLibrary(_T("UxTheme.dll"));
}

COXUxTheme::~COXUxTheme()
{
	if (m_hThemeDLL != NULL)
		::FreeLibrary(m_hThemeDLL);
}

BOOL COXUxTheme::IsUxThemeLoaded()
{
	if (m_hThemeDLL != NULL)
		return TRUE;
	else
		return FALSE;
}

HTHEME COXUxTheme::GetWindowTheme(HWND hWnd)
{
	if (!IsUxThemeLoaded())
		return NULL;

	ASSERT(m_hThemeDLL != NULL);

	HTHEME (PASCAL* pfnGetWindowTheme)(HWND hWnd) = NULL;
	(FARPROC&) pfnGetWindowTheme = ::GetProcAddress(m_hThemeDLL, "GetWindowTheme");
	HTHEME hTheme = (*pfnGetWindowTheme)(hWnd);
	return hTheme;
}

BOOL COXUxTheme::DrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCRECT pRect, LPCRECT pClipRect)
{
	ASSERT(m_hThemeDLL != NULL);

	HRESULT (PASCAL* pfnDrawThemeBackground)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCRECT pRect, LPCRECT pClipRect) = NULL;
	(FARPROC&) pfnDrawThemeBackground = ::GetProcAddress(m_hThemeDLL, "DrawThemeBackground");
	HRESULT hr = (*pfnDrawThemeBackground)(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);

	if (hr == S_OK)
		return TRUE;
	else
		return FALSE;
}

BOOL COXUxTheme::DrawThemeText(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCTSTR lpszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect)

{
	ASSERT(m_hThemeDLL != NULL);

	HRESULT (PASCAL* pfnDrawThemeText)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR lpszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect) = NULL;
	(FARPROC&) pfnDrawThemeText = ::GetProcAddress(m_hThemeDLL, "DrawThemeText");
	HRESULT hr = (*pfnDrawThemeText)(hTheme, hdc, iPartId, iStateId, _bstr_t(lpszText), iCharCount, dwTextFlags, dwTextFlags2, pRect);

	if (hr == S_OK)
		return TRUE;
	else
		return FALSE;
}

COLORREF COXUxTheme::GetThemeSysColor(HTHEME hTheme, int iColorID)
{
	ASSERT(m_hThemeDLL != NULL);

	COLORREF (PASCAL* pfnGetThemeSysColor)(HTHEME hTheme, int iColorID) = NULL;
	(FARPROC&) pfnGetThemeSysColor = ::GetProcAddress(m_hThemeDLL, "GetThemeSysColor");
	return (*pfnGetThemeSysColor)(hTheme, iColorID);
}

HTHEME COXUxTheme::OpenThemeData(HWND hWnd, LPCTSTR lpszClassList)
{
	if (m_hThemeDLL == NULL)
		return NULL;

	HTHEME (PASCAL* pfnOpenThemeData)(HWND hWnd, LPCWSTR pszClassList) = NULL;
	(FARPROC&) pfnOpenThemeData = ::GetProcAddress(m_hThemeDLL, "OpenThemeData");
	return (*pfnOpenThemeData)(hWnd, _bstr_t(lpszClassList));
}
