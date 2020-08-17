// ==========================================================================
// 					Class Specification : 
//						COXUxTheme
// ==========================================================================

// Header file : COXUxTheme.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

#if !defined(AFX_OXUXTHEME_H__C471434C_4DF8_41F9_87AD_E84047F31D0A__INCLUDED_)
#define AFX_OXUXTHEME_H__C471434C_4DF8_41F9_87AD_E84047F31D0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef HTHEME
typedef HANDLE HTHEME;
#endif

#ifndef HIS_NORMAL
#define HIS_NORMAL 1
#endif

#ifndef HIS_HOT
#define HIS_HOT 2
#endif

#ifndef HIS_PRESSED
#define HIS_PRESSED 3
#endif

#ifndef HP_HEADERITEM
#define HP_HEADERITEM 1
#endif

#include "OXDllExt.h"

class OX_CLASS_DECL COXUxTheme  
{
public:
	HTHEME OpenThemeData(HWND hWnd, LPCTSTR lpszClassList);
	COLORREF GetThemeSysColor(HTHEME hTheme, int iColorID);
	HTHEME GetWindowTheme(HWND hWnd);
	BOOL DrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCRECT pRect, LPCRECT pClipRect);
	BOOL DrawThemeText(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCTSTR lpszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect);	
	BOOL IsUxThemeLoaded();
	
	COXUxTheme();
	virtual ~COXUxTheme();

protected:
	HMODULE m_hThemeDLL;
};

#endif // !defined(AFX_OXUXTHEME_H__C471434C_4DF8_41F9_87AD_E84047F31D0A__INCLUDED_)
