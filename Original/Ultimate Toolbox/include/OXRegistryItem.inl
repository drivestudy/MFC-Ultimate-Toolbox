// ==========================================================================
// 					Inline functions : COXRegistryItem 
// ==========================================================================

// Header file : OXRegistryItem.inl

// Version: 9.3

// Source : R.Mortelmans
// Creation Date : 	   31st January 1997
// Last Modification : 10th Fenruary 1997
                          
// //////////////////////////////////////////////////////////////////////////

// public:
inline BOOL COXRegistryItem::IsEmpty()
	{
	ASSERT_VALID(this);
	return GetFullRegistryItem().IsEmpty();
	}

inline HRESULT COXRegistryItem::GetLastError() const
	{
	ASSERT_VALID(this);
	return m_nLastError;
	}

inline void COXRegistryItem::SetLastError(HRESULT nLastError)
	{
	ASSERT_VALID(this);
	m_nLastError = nLastError;
	ASSERT_VALID(this);
	}

inline BOOL COXRegistryItem::IsOpen() const
	{
	ASSERT_VALID(this);
	return (m_hKey != NULL);
	}

inline HKEY COXRegistryItem::GetRootKey() const
	{
	ASSERT_VALID(this);
	return m_hRootKey;
	}

inline HKEY COXRegistryItem::GetKey() const
	{
	ASSERT_VALID(this);
	return m_hKey;
	}

// protected:
inline void COXRegistryItem::SetMainBuilt()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Mark that the main (full) registry item has been built
	{
	m_bMainBuilt = TRUE;
	}

inline void COXRegistryItem::SetPartsBuilt()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Mark that the registry parts have been built
	{
	m_bPartsBuilt = TRUE;
	}

inline void COXRegistryItem::DestroyMain()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Mark that the main (full) regsitry item is not valid anymore
	{
	// ... We just flag that the main data has not been built yet
	m_bMainBuilt = FALSE;
	}

inline void COXRegistryItem::DestroyParts()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Mark that the registry parts are not valid anymore
	{
	// ... We just flag that the parts data has not been built yet
	m_bPartsBuilt = FALSE;
	}

inline BOOL COXRegistryItem::IsMainBuilt() const
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the main (full) regsitry item has been built
	// --- Effect : 
	{
	return m_bMainBuilt;
	}

inline BOOL COXRegistryItem::ArePartsBuilt() const
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the registry parts have been built
	// --- Effect : 
	{
	return m_bPartsBuilt;
	}

inline HRESULT COXRegistryItem::HResultFromWin32(int nWin32Error) const
	// --- In  :
	// --- Out : 
	// --- Returns : The HResult code for a Win32 error code
	// --- Effect : This inline functions does the same as the HRESULT_FROM_WIN32(x)
	//				macro apart from the side effects (double expression evaluation)
	{
	return (nWin32Error ? ((HRESULT) (((nWin32Error) & 0x0000FFFF) | (FACILITY_WIN32 << 16) | 0x80000000)) : 0 );
	}

// //////////////////////////////////////////////////////////////////////////
