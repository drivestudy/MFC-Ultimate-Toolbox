// ==========================================================================
// 					Class Implementation : COXBrowseDirEditList
// ==========================================================================

// Header file : OXBrowseDirEditList.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OXBrowseDirEditList.h"


LRESULT COXBrowseDirGridEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	switch(message)
	{
	case WM_KILLFOCUS:
		if(m_bIsInDropDown)
		{
			return 0;
		}
		break;
	default:
		break;
	}

	return COXBaseBrowseDirEdit<COXGridEdit>::WindowProc(message,wParam,lParam);
}


LRESULT COXBrowseDirGridEdit16::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	switch(message)
	{
	case WM_KILLFOCUS:
		if(m_bIsInDropDown)
		{
			return 0;
		}
		break;
	default:
		break;
	}

	return COXBaseBrowseDirEdit16<COXGridEdit>::WindowProc(message,wParam,lParam);
}


LRESULT COXBrowseFileGridEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	switch(message)
	{
	case WM_KILLFOCUS:
		if(m_bIsInDropDown)
		{
			return 0;
		}
		break;
	default:
		break;
	}

	return COXBaseBrowseFileEdit<COXGridEdit>::WindowProc(message,wParam,lParam);
}

