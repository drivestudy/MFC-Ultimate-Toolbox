// ==========================================================================
// 					Inline functions : COXResourceLibrary
// ==========================================================================
//
// Inline file : OXResourceLibrary.inl
// Source : Written by: Haiping Zhao

// Version: 9.3


inline CString COXResourceLibrary::GetFileName() const
	{
	return m_sFileName;
	}

inline DWORD COXResourceLibrary::GetResourceSize(HRSRC hRes) const
	{
	return ::SizeofResource(m_hLib, hRes);
	}

inline HGLOBAL COXResourceLibrary::LoadResource(HRSRC hRes) const
	{
	return ::LoadResource(m_hLib, hRes);
	}

inline BOOL COXResourceLibrary::Commit(BOOL bDeleteExistingResources /* = FALSE */)
	{
	return (BeginUpdate(bDeleteExistingResources) && EndUpdate(TRUE));
	}

inline HINSTANCE COXResourceLibrary::GetLibraryHandle() const
	{
	return m_hLib;
	}

inline HANDLE COXResourceLibrary::GetUpdateHandle() const
	{
	return m_hUpdateRes;
	}

// end of OXResourceLibrary.inl