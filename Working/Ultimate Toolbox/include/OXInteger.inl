// ==========================================================================
// 					Inline Implementation : COXInteger
// ==========================================================================

// Header file : OXInteger.inl

// Version: 9.3

// Source : R.Mortelmans
// Creation Date : 	   12th February 1997
// Last Modification : 12th February 1997
                          
// //////////////////////////////////////////////////////////////////////////

inline COXInteger::COXInteger(LONGLONG nNumber /* = 0 */)
	:
	m_nDecimal(nNumber)
	{
	}

inline COXInteger::COXInteger(LPCTSTR pszNumber, int nRadix /* = 10 */)
	:
	m_nDecimal(0)
	{
	SetStringNumber(pszNumber, nRadix);
	}

inline LONGLONG COXInteger::GetNumber() const
	{
	return m_nDecimal;
	}

inline void COXInteger::SetNumber(LONGLONG nNumber)
	{
	m_nDecimal = nNumber;
	}

inline void COXInteger::Empty()
	{
	m_nDecimal = 0;
	}

inline BOOL COXInteger::IsEmpty() const
	{
	return (m_nDecimal == 0);
	}

inline COXInteger::operator LONGLONG() const
	{
	return GetNumber();
	}

inline COXInteger::~COXInteger()
	{
	}

// ==========================================================================
