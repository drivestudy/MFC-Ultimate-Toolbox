// ==========================================================================
// 					Inline functions : COXShortcut
// ==========================================================================

// Inline file : OXShortcut.inl

// Version: 9.3

// Source : Written by: Haiping Zhao


inline CString COXShortcut::GetCurFile() const
	{
	return m_sCurFileName;
	}

inline HRESULT COXShortcut::GetLastError() const
	{
	return m_hres;
	}

inline BOOL COXShortcut::Validate(void* pv)
	// --- In      :
	// --- Out     : 
	// --- Returns :
	// --- Effect  : when calling a COM method, we use this function to verify every pointer
	//				 that cannot be NULL (at run-time) to minimize disaster
	{
	if (pv == NULL)
		{
		// ... we define this as unexpected
		m_hres = E_UNEXPECTED;
		return FALSE;
		}
	return TRUE;
	}

// end of OXShortcut.inl