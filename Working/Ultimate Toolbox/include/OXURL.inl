// ==========================================================================
// 					Inline functions : COXURL
// ==========================================================================

// Header file : OXURL.inl

// Version: 9.3

// //////////////////////////////////////////////////////////////////////////

inline void COXURL::SetMainBuilt()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Mark that the main (full) URL has been built
	{
	m_bMainBuilt = TRUE;
	}

inline void COXURL::SetPartsBuilt()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Mark that the URL parts have been built
	{
	m_bPartsBuilt = TRUE;
	}

inline void COXURL::DestroyMain()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Mark that the main (full) URL is not valid anymore
	{
	// ... We just flag that the main data has not been built yet
	m_bMainBuilt = FALSE;
	}

inline void COXURL::DestroyParts()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Mark that the URL parts are not valid anymore
	{
	// ... We just flag that the parts data has not been built yet
	m_bPartsBuilt = FALSE;
	}

inline BOOL COXURL::IsMainBuilt() const
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the main (full) URL has been built
	// --- Effect : 
	{
	return m_bMainBuilt;
	}

inline BOOL COXURL::ArePartsBuilt() const
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the URL parts have been built
	// --- Effect : 
	{
	return m_bPartsBuilt;
	}

// ==========================================================================
