// ==========================================================================
//					Class Inline Implementation : COXRegistryWatchNotifier
// ==========================================================================

// $Archive:: /MFCEXT/Samples/RegEvent/OXRegistryWatchNotifier.inl        $
// Source : R.Partyka
// Creation Date : 4th July 1997
// $Author:: Roel                                                         $
// $Date:: 7/08/97 12:14                                                  $
// $Revision:: 2                                                          $
			  
// //////////////////////////////////////////////////////////////////////////

// Version: 9.3


inline CTime COXRegistryWatchNotifier::GetNotificationTime() const
	{
	return m_tNotificationTime;
	}

inline HKEY COXRegistryWatchNotifier::GetRegKey() const
	{
	return m_hWatchKey;
	}

inline BOOL COXRegistryWatchNotifier::GetWatchSubtree() const
	{
	return m_bWatchSubtree;
	}

inline DWORD COXRegistryWatchNotifier::GetWatchFilter() const
	{
	return m_dwNotifyFilter;
	}

inline DWORD COXRegistryWatchNotifier::GetWatchID() const
	{
	return m_dwID;
	}

inline CEvent* COXRegistryWatchNotifier::GetEvent() const
	{
	return m_phWatchEvent;
	}

inline CWnd* COXRegistryWatchNotifier::GetWndDst() const
	{
	return CWnd::FromHandle(m_hWndDst);
	}

inline BOOL COXRegistryWatchNotifier::GetPost() const
	{
	return m_bPost;
	}

// protected:
inline void COXRegistryWatchNotifier::SetNotificationTime()
	// --- In :
	// --- Out :
	// --- Returns :
	// --- Effect :		Sets the current time when the notification was received.
	{
	m_tNotificationTime = CTime::GetCurrentTime();
	}

inline void COXRegistryWatchNotifier::SetPost(BOOL bPost)
	// --- In :			bPost :	If TRUE, notification message is posted to a window,
	//							if FALSE, message sent.
	// --- Out :
	// --- Returns :
	// --- Effect :		Sets whether notification message posted or sent to a window.
	{
	m_bPost = bPost;
	}

inline void COXRegistryWatchNotifier::SetWndDst(CWnd* pWndDst)
	// --- In :			pWndDst :	Pointer to CWnd object that represents receiving window.
	// --- Out :
	// --- Returns :
	// --- Effect :		Sets the window that receives the notification message.
	{
	m_hWndDst = pWndDst->GetSafeHwnd();
	}

// //////////////////////////////////////////////////////////////////////////
