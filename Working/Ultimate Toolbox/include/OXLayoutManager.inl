// ==========================================================================
// 					Class Specification : COXLayoutManager
// ==========================================================================
// Header file :		OXLayoutManager.h
//
// Version: 9.3
//
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
//
/////////////////////////////////////////////////////////////////////////////

inline int COXLayoutManager::AddChild(CWnd* pChildWnd,
									  BOOL bSetDefaultConstraints /* = TRUE */)
{
	ASSERT(pChildWnd && pChildWnd->GetDlgCtrlID());
	return AddChild(pChildWnd->GetDlgCtrlID(), bSetDefaultConstraints);
}

inline BOOL COXLayoutManager::RemoveChild(CWnd* pChildWnd)
{
	ASSERT(pChildWnd && pChildWnd->GetDlgCtrlID());
	return RemoveChild(pChildWnd->GetDlgCtrlID());
}

inline BOOL COXLayoutManager::SetConstraint(CWnd* pChildWnd, int nSide, int nType,
											int nOffset /* = 0 */, CWnd* pBaseWnd /* = NULL */)
{
	ASSERT(pChildWnd && pChildWnd->GetDlgCtrlID());
	ASSERT(pBaseWnd == NULL || pBaseWnd->GetDlgCtrlID() != 0);
	return SetConstraint(pChildWnd->GetDlgCtrlID(), nSide, nType,
		nOffset, pBaseWnd == NULL ? 0 : pBaseWnd->GetDlgCtrlID());
}

inline BOOL COXLayoutManager::SetMinMax(CWnd* pChildWnd, CSize sizeMin, 
										CSize sizeMax /* = CSize(0,0) */)
{
	ASSERT(pChildWnd && pChildWnd->GetDlgCtrlID());
	return SetMinMax(pChildWnd->GetDlgCtrlID(), sizeMin, sizeMax);
}

inline BOOL COXLayoutManager::RemoveConstraint(CWnd* pChildWnd, int nSide)
{
	ASSERT(pChildWnd && pChildWnd->GetDlgCtrlID());
	return RemoveConstraint(pChildWnd->GetDlgCtrlID(), nSide);
}

inline int COXLayoutManager::GetFractionBase() const
{
	return m_nBase;
}

inline void COXLayoutManager::SetFractionBase(int nBase)
{
	if (nBase) m_nBase = nBase;
}

inline void COXLayoutManager::ResetContainerMinMax()
{
	m_cxMin = m_cyMin = -1;
	m_cxMax = m_cyMax = 32767;
}

inline BOOL COXLayoutManager::TieChild(CWnd* pChildWnd, int nSide, int nType, 
									   CWnd* pBaseWnd/*=NULL*/)
{
	ASSERT(pChildWnd && pChildWnd->GetDlgCtrlID());
	ASSERT(pBaseWnd==NULL || pBaseWnd->GetDlgCtrlID()!=0);
	return TieChild(pChildWnd->GetDlgCtrlID(),nSide, nType,
		pBaseWnd==NULL ? 0 : pBaseWnd->GetDlgCtrlID());
}

inline BOOL COXLayoutManager::IsAttached() const
{
	return (m_pContainerWnd!=NULL);
}

// end of OXLayoutManager.inl