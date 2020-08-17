// OXListEdit.cpp : implementation file
//

#include "stdafx.h"
#include "OXListEdit.h"
#include "OXPropertiesWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_LIST_BOX	300
#define WM_USER_PICKED	(WM_USER + 100)

/////////////////////////////////////////////////////////////////////////////
// COXListPopup

COXListPopup::COXListPopup()
{
}

COXListPopup::~COXListPopup()
{
}


BEGIN_MESSAGE_MAP(COXListPopup, CListBox)
	//{{AFX_MSG_MAP(COXListPopup)
//	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COXListEdit

IMPLEMENT_DYNCREATE(COXListEdit, COXEdit)

COXListEdit::COXListEdit()
{
}

COXListEdit::~COXListEdit()
{
}


BEGIN_MESSAGE_MAP(COXListEdit, COXEdit)
	//{{AFX_MSG_MAP(COXListEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int COXListPopup::Pick(CRect rect, CRect rectParent)
{
	AdjustDisplayRectangle(rect, rectParent);

	MoveWindow(rect);
	ShowWindow(SW_SHOWNA);
	SetCapture();

	// init message loop
	bool bBreak = false;
	int iReturnItemIdx = -1;
	while (!bBreak)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		if (msg.message == WM_LBUTTONUP)
		{
			// Get the item under the mouse cursor
			int xPos = GET_X_LPARAM(msg.lParam); 
			int yPos = GET_Y_LPARAM(msg.lParam);

			BOOL bOutside;
			UINT nIndex = ItemFromPoint(CPoint(xPos, yPos), bOutside);
			if (!bOutside)
				iReturnItemIdx = (int) nIndex;
			bBreak = true;
		}
		else if (msg.message == WM_KEYDOWN)
		{
			// Handle ESCAPE, UP, DOWN and ENTER
			if (msg.wParam == VK_ESCAPE)
				bBreak = true;
			else if (msg.wParam == VK_UP)
			{
				int iSel = GetCurSel();
				if (iSel == -1 || iSel == 0)
					SetCurSel(0);
				else
					SetCurSel(iSel - 1);
			}
			else if (msg.wParam == VK_DOWN)
			{
				// Move the selection 1 item down
				int iSel = GetCurSel();
				if (iSel == -1)
					SetCurSel(0);
				else if (iSel == GetCount() - 1)
				{
					// Do nothing
				}
				else
					SetCurSel(iSel + 1);
			}
			else if (msg.wParam == VK_RETURN)
			{
				iReturnItemIdx = GetCurSel();
				bBreak = true;
			}
		}
		else if (msg.message == WM_LBUTTONDOWN)
		{
			// Do nothing				
		}
		else if (msg.message == WM_MOUSEMOVE)
		{
			// Select the item under the mouse cursor
			int xPos = GET_X_LPARAM(msg.lParam); 
			int yPos = GET_Y_LPARAM(msg.lParam);

			BOOL bOutside;
			UINT nIndex = ItemFromPoint(CPoint(xPos, yPos), bOutside);
			if (!bOutside)
				SetCurSel((int) nIndex);
		}
		else
		{
			DispatchMessage(&msg);
		}
	}

	ReleaseCapture();
	ShowWindow(SW_HIDE);

	return iReturnItemIdx;
}

void COXListPopup::AdjustDisplayRectangle(CRect &rect, CRect rectParent)
{
	// Get the rectangle of the monitor closest to the menu rectangle
	HMONITOR hMonitor = ::MonitorFromRect(rect, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi;
	mi.cbSize = sizeof(MONITORINFO);
	::GetMonitorInfo(hMonitor, &mi);

	const int iMixScreenX = mi.rcMonitor.left;
	const int iMaxScreenX = mi.rcMonitor.right;
	const int iMaxScreenY = mi.rcMonitor.bottom;

	CSize sizeParent(rectParent.Width(), rectParent.Height());
	CPoint ptTopLeft(rect.left, rect.top);

	if (ptTopLeft.x < iMixScreenX)
		ptTopLeft.x = iMixScreenX;
	if (ptTopLeft.x + rect.Width() > iMaxScreenX)
		ptTopLeft.x = iMaxScreenX - rect.Width();

	// Make sure the popup is not clipped at the bottom
	if (rect.bottom > iMaxScreenY)
	{
		// The popup should be above the item
		ptTopLeft.y = rectParent.top - rect.Height();
	}

	CSize sizeTemp = rect.Size();
	rect.SetRect(ptTopLeft.x, ptTopLeft.y,
		ptTopLeft.x + sizeTemp.cx, ptTopLeft.y + sizeTemp.cy);
}

/////////////////////////////////////////////////////////////////////////////
// COXListEdit message handlers

BOOL COXListEdit::InitializeDropEdit()
{
	if(!m_DropList.CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, _T("ListBox"),_T(""),
		WS_POPUP | WS_BORDER, CRect(0, 0, 0, 0), this, 0, NULL))
	{
		TRACE(_T("Unable to create drop list.\n"));
		AfxThrowResourceException();
	}
	m_DropList.SetOwner(this);

	m_font.CreatePointFont(80, _T("MS Sans Serif"));
	m_DropList.SetFont(&m_font);

	return TRUE;
}

void COXListEdit::OnDropButton()
{
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(rect);
	rect.right += GetButtonWidth();
	CRect rectParent = rect;
	rect.top = rect.bottom;

	// Determine the height of the list box
	int iItemsMax = m_DropList.GetCount();
	if (iItemsMax > 20)
		iItemsMax = 20;

	rect.bottom += iItemsMax * (m_DropList.GetItemHeight(0) + 1);
	if (iItemsMax == 1)
		rect.bottom += 1;

	// Display the drop list
	::SetFocus(NULL);
	int iSel = m_DropList.Pick(rect, rectParent);
	if (iSel != -1)
	{
		CString strText;
		m_DropList.GetText(iSel, strText);
		SetWindowText(strText);
		m_pPropertiesWnd->FinishEditing();
	}
}

void COXListEdit::SetListItems(CStringArray* parItems, COXPropertiesWnd* pPropertiesWnd)
{
	ASSERT(parItems != NULL);
	ASSERT(pPropertiesWnd != NULL);

	if (!::IsWindow(m_hWnd))
		Create(WS_CHILD, CRect(0, 0, 0, 0), &pPropertiesWnd->m_wndTree, _EDITOR_ID);

	m_DropList.ResetContent();

	for (int i = 0; i < parItems->GetSize(); i++)
		m_DropList.AddString(parItems->GetAt(i));

	m_pPropertiesWnd = pPropertiesWnd;
}


