
// Version: 9.3

#ifndef __OXTASKBARICON_INL__
#define __OXTASKBARICON_INL__

inline UINT COXTaskbarIcon::GetID() const
	{
	return m_nTaskbarIconID;
	}

inline int COXTaskbarIcon::GetTooltipText(LPTSTR lpszStringBuf, int nMaxCount) const
	{
	ASSERT(::IsWindow(m_hWnd));
	return ::GetWindowText(m_hWnd, lpszStringBuf, nMaxCount);
	}

inline HICON COXTaskbarIcon::GetIcon() const
	{
	ASSERT(::IsWindow(m_hWnd));
	return (HICON)::SendMessage(m_hWnd, WM_GETICON, FALSE, 0);
	}

inline BOOL COXTaskbarIcon::IsShowing() const
	{
	return m_bShowing;
	}

#endif	// __OXTASKBARICON_INL__