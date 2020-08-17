// ==========================================================================
// 					Inline functions : COXSplashWnd
// ==========================================================================
//
//
// ==========================================================================
// Version: 9.3


inline BOOL COXSplashWnd::GetAutoDelete() const
	{ 
	return m_bAutoDelete;
	}

inline void COXSplashWnd::SetAutoDelete(BOOL bAutoDelete /* = TRUE */)
	{ 
	m_bAutoDelete = bAutoDelete;
	}

inline void COXSplashWnd::SetUserCancel(BOOL bUserCancel /* = TRUE */)
	{ 
	m_bUserCancel = bUserCancel;
	}

inline BOOL COXSplashWnd::GetUserCancel()
	{ 
	return m_bUserCancel;
	}

inline BOOL COXSplashWnd::LoadBitmap(UINT nIDResource, 
									 COLORREF crBorder /* = CLR_DEFAULT */,
									 LPPOINT pStartPoint /* = NULL */, 
									 BYTE nTolerance /* = 0 */)
	{ 
	return LoadBitmap(MAKEINTRESOURCE(nIDResource), crBorder,
		pStartPoint, nTolerance);
	}

inline COXSplashWndDIB* COXSplashWnd::GetDIB()
	{
	return &m_dib;
	}

inline BOOL COXSplashWnd::GetRegion (CRgn& rgn)
	{ 
	return ((HRGN)m_rgn != NULL) && (rgn.CopyRgn(&m_rgn) != ERROR);
	}

// static members
inline COXSplashWnd* COXSplashWnd::GetGlobalWnd()
	{ 
	return c_pGlobalWnd;
	}

inline void COXSplashWnd::EnableSplashScreen(BOOL bEnable /*= TRUE*/)
	{ 
	c_bShowSplashWnd = bEnable;
	}

inline BOOL COXSplashWnd::LoadGlobalBitmap(UINT nIDResource, 
										   COLORREF crBorder /* = CLR_DEFAULT */, 	LPPOINT pStartPoint /* = NULL */, 
										   BYTE nTolerance /* = 0 */)
	{ 
	return LoadGlobalBitmap(MAKEINTRESOURCE(nIDResource), crBorder,
		pStartPoint, nTolerance);
	}

inline BOOL COXSplashWnd::ShowGlobal(int nTimeOut /* = OXSPLASH_DEFTIMEOUT */, 
									 CWnd* pParentWnd /* = NULL */, 
									 CPoint* pptDisplayAt /* = NULL */)
	{ 
	return (c_pGlobalWnd != NULL) && 
		c_pGlobalWnd->Show(nTimeOut, pParentWnd, pptDisplayAt);
	}

inline BOOL COXSplashWnd::HideGlobal(int nTimeOut /* = 0 */)
	{ 
	return (c_pGlobalWnd != NULL) && c_pGlobalWnd->Hide(nTimeOut);
	}

inline BOOL COXSplashWnd::GetGlobalRegion (CRgn& rgn)
	{ 
	return (c_pGlobalWnd != NULL) && c_pGlobalWnd->GetRegion(rgn);
	}

inline BOOL COXSplashWnd::SetGlobalRegion (CRgn& rgn)
	{ 
	return (c_pGlobalWnd != NULL) && c_pGlobalWnd->SetRegion(rgn);
	}

// end of OXSplashWnd.inl