// ==========================================================================
// 					Inline functions : COXUNC
// ==========================================================================

// Header file : OXUNC.inl

// Version: 9.3

// Source : R.Mortelmans
                          
// //////////////////////////////////////////////////////////////////////////

inline void COXUNC::SetMainBuilt()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Mark that the main (full) UNC has been built
	{
	m_bMainBuilt = TRUE;
	}

inline void COXUNC::SetPartsBuilt()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Mark that the UNC parts have been built
	{
	m_bPartsBuilt = TRUE;
	}

inline void COXUNC::DestroyMain()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Mark that the main (full) UNC is not valid anymore
	{
	// ... We just flag that the main data has not been built yet
	m_bMainBuilt = FALSE;
	}

inline void COXUNC::DestroyParts()
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Mark that the UNC parts are not valid anymore
	{
	// ... We just flag that the parts data has not been built yet
	m_bPartsBuilt = FALSE;
	}

inline BOOL COXUNC::IsMainBuilt() const
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the main (full) UNC has been built
	// --- Effect : 
	{
	return m_bMainBuilt;
	}

inline BOOL COXUNC::ArePartsBuilt() const
	// --- In  :
	// --- Out : 
	// --- Returns : Whether the UNC parts have been built
	// --- Effect : 
	{
	return m_bPartsBuilt;
	}

// ==========================================================================
