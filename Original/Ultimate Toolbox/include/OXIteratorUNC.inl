// ==========================================================================
// 					Inline functions : COXIteratorUNC
// ==========================================================================

// Header file : OXIteratorUNC.inl

// Version: 9.3

// Source : R.Mortelmans
// Creation Date : 	   16th January 1996
// Last Modification : 16th January 1996
                          
// //////////////////////////////////////////////////////////////////////////
inline COXIteratorUNC& COXIteratorUNC::operator=(const COXUNC& UNC)
	{
	COXUNC::operator=(UNC);

	// Do not copy search specific data, because find handles cannot be copied

	return *this;
	}

inline COXIteratorUNC& COXIteratorUNC::operator=(const COXIteratorUNC& iterUNC)
	{
	COXUNC::operator=(iterUNC);

	// Do not copy search specific data, because find handles cannot be copied

	return *this;
	}

inline COXIteratorUNC& COXIteratorUNC::operator=(LPCTSTR pszUNC)
	{
	COXUNC::operator=(pszUNC);

	// Do not copy search specific data, because find handles cannot be copied

	return *this;
	}

inline const WIN32_FIND_DATA* COXIteratorUNC::GetCurrentSearchResult() const
	{
	if (m_fileSearch.m_hFindFile != NULL)
		return &m_fileSearch.m_findFileData;
	else
		return NULL;
	}


// ==========================================================================
