// OXPropertyTree.cpp : implementation file
//

#include "stdafx.h"
#include "OXPropertyTree.h"
#include "OXPropertiesWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXPropertyTree

IMPLEMENT_DYNAMIC(COXPropertyTree, COXTreeCtrl)

COXPropertyTree::COXPropertyTree(COXPropertiesWnd* pPropertiesWnd)
{
	m_pPropertiesWnd = pPropertiesWnd;
}

COXPropertyTree::~COXPropertyTree()
{
}


BEGIN_MESSAGE_MAP(COXPropertyTree, COXTreeCtrl)
	//{{AFX_MSG_MAP(COXPropertyTree)
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_EN_KILLFOCUS(_EDITOR_ID, OnKillEditFocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXPropertyTree message handlers

void COXPropertyTree::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	m_pPropertiesWnd->FinishEditing();
	
	COXTreeCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void COXPropertyTree::OnLButtonDown(UINT nFlags, CPoint point) 
{
	COXTreeCtrl::OnLButtonDown(nFlags, point);

	if (HitTest(point, &nFlags) == -1)
		m_pPropertiesWnd->FinishEditing();
}

BOOL COXPropertyTree::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN) {
			if(m_pPropertiesWnd->m_pEditWnd != NULL && ::GetFocus() == m_pPropertiesWnd->m_pEditWnd->m_hWnd) {
				m_pPropertiesWnd->FinishEditing();
			}
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE) {
			if(m_pPropertiesWnd->m_pEditWnd != NULL && ::GetFocus() == m_pPropertiesWnd->m_pEditWnd->m_hWnd) {
				m_pPropertiesWnd->FinishEditing(FALSE);	
			}
			return TRUE;
		}
		else if (pMsg->wParam == VK_TAB)
		{
			// if we're not editing, start editing
			if(m_pPropertiesWnd->m_pEditWnd != NULL && ::GetFocus() == m_pPropertiesWnd->m_pEditWnd->m_hWnd) {
				m_pPropertiesWnd->FinishEditing(TRUE);
				if(GetKeyState(VK_SHIFT) & 0xf0000000) {
					SendMessage(WM_KEYDOWN,VK_UP);
				}
				else {
					SendMessage(WM_KEYDOWN,VK_DOWN);
				}
			}
			HPROPERTY hSelectedProp = m_pPropertiesWnd->GetSelectedProperty();
			m_pPropertiesWnd->StartEditing(hSelectedProp);
			if(m_pPropertiesWnd->m_pEditWnd != NULL) {
				m_pPropertiesWnd->m_pEditWnd->SetFocus();
				m_pPropertiesWnd->m_pEditWnd->ShowCaret();
			}
			else {
				// read only field - move on...
				if(GetKeyState(VK_SHIFT) & 0xf0000000) {
					SendMessage(WM_KEYDOWN,VK_UP);
				}
				else {
					SendMessage(WM_KEYDOWN,VK_DOWN);
				}
			}
			return TRUE;
		}
	}
	
	return COXTreeCtrl::PreTranslateMessage(pMsg);
}

void COXPropertyTree::OnKillEditFocus() 
{
	if (m_pPropertiesWnd)
		m_pPropertiesWnd->FinishEditing();		
}
