// ==========================================================================
// 					Inline functions : COXStaticText
// ==========================================================================
// inline file :		OXStaticText.inl

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                          
// //////////////////////////////////////////////////////////////////////////

inline int COXStaticText::GetHorzAlignment() const
	{
	return m_nHorzAlignment;
	}

inline int COXStaticText::GetVertAlignment() const
	{
	return m_nVertAlignment;
	}

inline COLORREF COXStaticText::GetBkColor() const
	{
	return m_dwBkColor;
	}

inline COLORREF COXStaticText::GetTextColor() const
	{
	return m_dwTextColor;
	}

inline BOOL COXStaticText::GetEmboss() const
	{
	return m_bEmbossText;
	}

inline LPCTSTR COXStaticText::GetText() const
	{
	return (LPCTSTR)m_sText;
	}

inline int COXStaticText::GetGraphicsMode() const
	{
	return m_nGraphicsMode;
	}

inline int COXStaticText::GetEllipseMode() const
	{
	return  m_nEllipseMode;
	}

inline DWORD COXStaticText::Get3Doffset() const
	{
	return m_dwOffset;
	}

inline BOOL COXStaticText::IsItalic() const
	{
	return m_LogFont.lfItalic;
	}

inline BOOL COXStaticText::IsUnderlined() const
	{
	return m_LogFont.lfUnderline;
	}

inline BOOL COXStaticText::IsStrikedOut() const
	{
	return m_LogFont.lfStrikeOut;
	}

inline int COXStaticText::GetStringAngle() const
	{
	return m_LogFont.lfEscapement;
	}

inline int COXStaticText::GetCharAngle() const
	{
	return m_LogFont.lfOrientation;
	}

inline int COXStaticText::GetCharSet() const
	{
	return m_LogFont.lfCharSet;
	}

inline int COXStaticText::GetFontHeight() const
	{
	return m_LogFont.lfHeight;
	}

inline int COXStaticText::GetFontWidth() const
	{
	return m_LogFont.lfWidth;
	}

inline CString COXStaticText::GetFontName() const
	{
	return m_LogFont.lfFaceName;
	}

inline int COXStaticText::GetScrollDirection() const
	{
	return m_nScrollDirection;
	}

inline DWORD COXStaticText::GetScrollSpeed() const
	{
	return m_dwScrollSpeed;
	}

inline DWORD COXStaticText::GetMinTimeOut() const
	{
	return m_dwMinTimeOut;
	}

inline CSize COXStaticText::GetGapSize() const
	{
	return m_szGapSize;
	}

inline CRect COXStaticText::GetViewMargins() const
	{
	return m_rectViewMargins;
	}

inline CSize COXStaticText::GetTextSize() const
	{
	return m_szTextSize;
	}

// end of OXStaticText.inl