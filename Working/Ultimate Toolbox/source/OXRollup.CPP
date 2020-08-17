// ==========================================================================
// 					Class Implementation : COXRollup
// ==========================================================================

// Source file : oxrollup.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "OXTleBar.h"
#include "OXRollup.h"

#include <stdlib.h>		
#include "UTB64Bit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

// internal struct for helping scrolling

#define IDC_CAPTION			1		// ID of caption

IMPLEMENT_DYNAMIC(COXRollup, CDialog);

#define new DEBUG_NEW		// track memory allocation with line numbers

/////////////////////////////////////////////////////////////////////////////
// Definition of static members
CPtrList COXRollup::m_RollupList;
CPtrList COXRollup::m_ArrangedRollups;

// Data members -------------------------------------------------------------
// protected:
	// COXTitleBar* m_pTitleBar;
	// --- Pointer to the Titlebar window

	// BOOL	m_bRolledUp;
	// --- rolled up?

	// BOOL	m_bResizingFrame;
	// --- do we have a resizing frame?
	
	// UINT	m_myTimerID;
	// --- timer id used for rolling up and down
	
	// int m_xWidth;
	// int m_yHeight;
	// --- stored heights and widths of rollup (for resizeable)

	// CMenu* m_pSysMenu;
	// --- pointer to system menu
	
	// WORD m_Rollup;
	// --- ID of our dialog, external
	
	// UINT	m_nTemplateID;
	// --- Ressource ID of dialog
	
	// HWND	m_hWndMR;
	// --- window getting messages (currently)

	// HWND	m_hWndDestroyRC;
	// --- window that gets call to destroy rollup	

	// CString m_sCaption;
	// --- caption of Rollup

// private:
	
// Member functions ---------------------------------------------------------
// public:

COXRollup::COXRollup(UINT nTemplate,CWnd* pParent /*=NULL*/)
	: CDialog(nTemplate, pParent),
	m_myTimerID(12345),	// timer id
	m_nTemplateID(nTemplate),
	m_bResizingFrame(FALSE),
	m_bRolledUp(FALSE),                   
	m_xWidth(-1),
	m_yHeight(-1),
	m_nRollup(0),
	m_nScrollSteps(ROLL_STATES),
	m_pSysMenu(NULL),
	m_pTitleBar(NULL),
	m_hWndMR(NULL),
	m_hWndDestroyRC(NULL),
	m_bIsArranged(FALSE),
	m_bRolling(FALSE)
	{
	// add entry to our rollup list
	m_RollupList.AddTail(this);
	}

COXRollup::~COXRollup()
	{
	if (m_pSysMenu)
		{
		m_pSysMenu->DestroyMenu();
		delete m_pSysMenu;
		}

    if (m_pTitleBar)
		{
		m_pTitleBar->DestroyWindow();
		delete m_pTitleBar;
		}
	// we have to remove ourselves from the rollup-list
	RemoveFromRUList();

	DestroyWindow();
	}

void COXRollup::RemoveFromRUList()
	{
	ASSERT_VALID(this);
	POSITION pos = m_RollupList.Find(this, NULL);

	// this is not to be expected an error (maybe you don´t want this rollup to arrange, so
	// you called this function before)
	if (pos != NULL)
		m_RollupList.RemoveAt(pos);
	}

void COXRollup::DoDataExchange(CDataExchange* pDX)
	{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COXRollup)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP  
	}

BOOL COXRollup::IsRolling()
	{
	return m_bRolling;
	}

BEGIN_MESSAGE_MAP(COXRollup, CDialog)
	//{{AFX_MSG_MAP(COXRollup)
	ON_COMMAND(IDM_OX_RU_ROLLUP, OnClickedRoll)
	ON_COMMAND(IDM_OX_RU_CLOSE, OnCloseRollup)
	ON_MESSAGE(WM_LMSUROLLUP,OnRollMessage)
	ON_COMMAND(IDM_OX_RU_ABOUT, OnRollUpAbout)
	ON_WM_SIZE()
	ON_WM_ACTIVATE()   
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_COMMAND(IDM_OX_RU_ARRANGE, Arrange)
	ON_COMMAND(IDM_OX_RU_ARRANGEALL, ArrangeAll)
	ON_COMMAND(IDM_OX_RU_ROLLDOWN, OnClickedRoll)
	ON_WM_DESTROY()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXRollup message handlers

void COXRollup::OnNcPaint()
	{
	Default();

	if (!m_bRolledUp && m_bResizingFrame)
		{
		CWindowDC dc(this);
		COLORREF FrameColor = ::GetSysColor(COLOR_WINDOWFRAME);
		CPen FramePen(PS_SOLID, 1, FrameColor);
		CPen* pOldPen = dc.SelectObject(&FramePen);

		CRect WndRect;
		GetWindowRect(WndRect);
		dc.MoveTo (0, 0);
		dc.LineTo (WndRect.Width() - 1, 0);
		dc.LineTo (WndRect.Width() - 1, WndRect.Height() - 1);
		dc.LineTo (0, WndRect.Height() - 1);
		dc.LineTo (0, 0);

		dc.SelectObject(pOldPen);
		}
	}

void COXRollup::OnClickedRoll()
	{
	// don´t care about it if I´m no window
	if (GetSafeHwnd() == NULL)
		return;
	
	if (m_bRolledUp)	// currently rolled up, do downrolling
		{
//====================================================================================
//  commented out (behaviour dependent on own needs)
//		UnArrange();	// get out of arranging (false heights otherwise)
//====================================================================================
		CRect fullRect;
		GetWindowRect(&fullRect);
		int	capHeight = fullRect.Height();
		
		if (m_bResizingFrame)
			{
			// memorize new width if border resizable
			m_xWidth = fullRect.Width();
			} 
		
		int decrement = m_yHeight - capHeight;
		decrement /= m_nScrollSteps;
		
		m_SCROLL_HELP.decrement	= decrement;
		m_SCROLL_HELP.width		= m_xWidth;
		m_SCROLL_HELP.height		= m_yHeight;
		m_SCROLL_HELP.capHeight	= capHeight;
		m_SCROLL_HELP.steps		= m_nScrollSteps;
		m_SCROLL_HELP.curstep		= 1;
		m_SCROLL_HELP.direction	= SCROLL_DOWN;
		if (SetTimer(m_myTimerID, DELAY_TIME,NULL) == 0)
			{
			CString sMessage;
			VERIFY(sMessage.LoadString(IDS_OX_ROLLUPCOULDNTALLOCTIMER));//"Could not alloc timer!"
			AfxMessageBox(sMessage);
			}
		m_bRolling = TRUE;
		}
	else	// roll it up again
		{
		// now roll it up
		if (m_bResizingFrame || m_xWidth == -1)
			{
			CRect fullRect;
			GetWindowRect(&fullRect);
			m_xWidth = fullRect.Width();
			m_yHeight = fullRect.Height();
			}
		
		CRect roll2;
		m_pTitleBar->GetClientRect(&roll2);
		
		int nFrameSize = 2 * ::GetSystemMetrics(SM_CYBORDER);
		int decrement = m_yHeight - roll2.Height() - nFrameSize +
			(m_bResizingFrame ? 2 * ::GetSystemMetrics(SM_CYDLGFRAME) : 0);
		
		decrement /= m_nScrollSteps;
		
		m_SCROLL_HELP.decrement	= decrement;
		m_SCROLL_HELP.width		= m_xWidth;
		m_SCROLL_HELP.height		= m_yHeight;
		m_SCROLL_HELP.capHeight	= roll2.Height() + nFrameSize;
		m_SCROLL_HELP.steps		= m_nScrollSteps;
		m_SCROLL_HELP.curstep		= 1;
		m_SCROLL_HELP.direction	= SCROLL_UP;
		if (SetTimer(m_myTimerID, DELAY_TIME,NULL) == 0)
			{
			CString sMessage;
			VERIFY(sMessage.LoadString(IDS_OX_ROLLUPCOULDNTALLOCTIMER));//"Could not alloc timer!"
			AfxMessageBox(sMessage);
			}
		m_bRolling = TRUE;
		}

	m_bRolledUp = !m_bRolledUp;

	// reflect the changes
	m_pTitleBar->SetRollupState(m_bRolledUp ? SCROLL_DOWN : SCROLL_UP);
	Invalidate();
	}

void COXRollup::OnCloseRollup()
	{
	// called either from titlebar or from OnCloseRollup
	// override if you want to do something special in here (delete this, anything like that)
	ShowWindow(SW_HIDE);	// first we hide
	UnArrange();			// second get out of arrange orderings
	
	ReleaseRecipient(CWnd::FromHandle(m_hWndMR));	// release MR to send to parent
	Send2MR(IDCANCEL);
	}

void COXRollup::OnProcessSysMenu()
	{
	// dim the roll up and down commands via the boolean flag
	CMenu *pSysmenu = GetSysMenu();
	pSysmenu->EnableMenuItem(IDM_OX_RU_ROLLUP,
		MF_BYCOMMAND|(m_bRolledUp ? MF_DISABLED|MF_GRAYED : MF_ENABLED));
	pSysmenu->EnableMenuItem(IDM_OX_RU_ROLLDOWN,
		MF_BYCOMMAND|(m_bRolledUp ? MF_ENABLED : MF_DISABLED|MF_GRAYED));
	pSysmenu->EnableMenuItem(IDM_OX_RU_ARRANGE,
		MF_BYCOMMAND|(IsArranged() ? MF_DISABLED|MF_GRAYED : MF_ENABLED));
	
	// set the position of the system menu
	CRect rect;
	m_pTitleBar->GetWindowRect(&rect);
	pSysmenu->TrackPopupMenu(TPM_LEFTBUTTON|TPM_LEFTALIGN, rect.left, rect.bottom, this, NULL);
	}

BOOL COXRollup::CreateRollUp(CWnd* pParent, WORD nID, int nIDCaption)
{
	CString sCaption;
	VERIFY(sCaption.LoadString(nIDCaption));
	return CreateRollUp(pParent, nID, sCaption);
}

BOOL COXRollup::CreateRollUp(CWnd* pParent, WORD nID, LPCTSTR lpszCaption)
{
	ASSERT(pParent!=NULL && ::IsWindow(pParent->GetSafeHwnd()));
	ASSERT(lpszCaption!=NULL);

	BOOL bAlreadyCreated=::IsWindow(GetSafeHwnd());
	if(bAlreadyCreated)
	{
		DestroyWindow();
	}

	ASSERT(nID!=0);
	// set our roll up ID used later when dispatching messages
	m_nRollup=nID;

	m_sCaption=lpszCaption;

	// set up the window which is responsible for handling the close msg
	m_hWndDestroyRC = pParent->GetSafeHwnd();

	// register ourselves ( the parent ) as owner of the message dispatch
	SetRecipient(pParent);

	// now create us!
	m_bRolledUp = FALSE;	// full size
	if(!CDialog::Create(m_nTemplateID, pParent))
		return FALSE;
	
	ASSERT(::IsWindow(m_hWnd));
	DetermineFrameStyle();
	
	LPCTSTR lptstrBitmap=GetTitleBarBitmap();
	ASSERT(lptstrBitmap!=NULL);
	VERIFY(m_pTitleBar->LoadBitmap(lptstrBitmap));
	
	return TRUE;
}

// find our responsible window for acknowledging our messages
//
HWND COXRollup::GetCurMsgRecipient()
	{
	// if nobody is there except our parent..
	return m_hWndMR != NULL ? m_hWndMR : m_hWndDestroyRC;
	}

// SendMessage like
//
LRESULT COXRollup::Send2MR(WORD msg)
	{
	// we can´t send a message to a parent if we are no window!
	ASSERT(GetSafeHwnd() != NULL);
	
	HWND hWnd = GetCurMsgRecipient();

	// now route the message to the one who feels responsible	
	return ::SendMessage(hWnd, WM_LMSUROLLUP, (WPARAM)GetSafeHwnd(), MAKELPARAM(msg, m_nRollup));
	}

//
// "message posting" implemented; intended use is simple notification
// like "we are rolling up now"
//
BOOL COXRollup::Post2MR(WORD msg)
	{
	// we can´t send a message to a parent if we are no window!
	ASSERT(GetSafeHwnd() != NULL);
	
	HWND hWnd = GetCurMsgRecipient();

	// now route the message to the one who feels responsible	
	return ::PostMessage(hWnd, WM_LMSUROLLUP, (WPARAM)GetSafeHwnd(), MAKELPARAM(msg, m_nRollup));
	}

HWND COXRollup::SetRecipient(CWnd* pWnd)
	{
	ASSERT(pWnd != NULL && ::IsWindow(pWnd->GetSafeHwnd()));
	
	// store old value for return
	HWND hOld = m_hWndMR;
	// set recipient
	m_hWndMR = pWnd->GetSafeHwnd();

	TRACE1("[RU]New recipient´s HWND : %ld\n", m_hWndMR);
	return hOld;
	} 

void COXRollup::ReleaseRecipient(const CWnd* pRCRequesting, BOOL bCallNotifyFunction)
	{
	// do this only when he is the current owner of the message dispatch system
	if (IsOwner(pRCRequesting)) 
		{
		TRACE1("[RU]Released the owner of the mds. Handle : %ld\n", m_hWndMR);
		
		// if he chose to do some cleanup, do it
		if (bCallNotifyFunction)
			OnRecipientRelease();
		
		m_hWndMR = NULL;
		}
	// else do nothing (somebody else has registered as current user
	}

// Intent: Parent can inform us to roll up or down
//
LRESULT COXRollup::OnRollMessage(WPARAM /* wParam */, LPARAM lParam)
	{
	if (HIWORD(lParam) == m_nRollup)
		{
		switch (LOWORD(lParam))
			{
			case ID_ROLLDOWN:
				( m_bRolledUp ? OnClickedRoll() : NULL);
				break;
			case ID_ROLLUP:
				( m_bRolledUp ? NULL : OnClickedRoll());
				break;
			}
		}

	return 0L;
	}

// Intent: Display About information 
//
void COXRollup::OnRollUpAbout()
	{
	// just a very simple about box
	AfxMessageBox(IDS_OX_RU_ABOUT, MB_OK | MB_ICONINFORMATION);
	}

// Intent: don´t allow wrong resources the class doesn´t allow
//
void  COXRollup::DetermineFrameStyle()
	{
	long style = ::GetWindowLongPtr(m_hWnd, GWL_STYLE);
#ifdef _DEBUG
	if (style & DS_MODALFRAME)
		{
		TRACE0("Create a rollup with DS_MODALFRAME? Uh,no!\n");
		ASSERT(FALSE);
		} 
	
	if (style & WS_CHILD)
		{
		TRACE0("Create a rollup with WS_CHILD property? Uh,no!\n");
		ASSERT(FALSE);
		}
#endif 
	
	if (style & WS_BORDER)
		m_bResizingFrame = FALSE;
	if (style & WS_THICKFRAME)
		m_bResizingFrame = TRUE;
	}

void COXRollup::OnSize(UINT nType, int cx, int cy)
	{
	CDialog::OnSize(nType, cx, cy);
	// if no resizing style and I don´t exist
	if ((!m_bResizingFrame) || (GetSafeHwnd() == NULL))
		return;
	
	CRect crCaption, crClient;
	GetClientRect(&crClient);
	
	// if dialog (Titlebar) doesn´t exist, skip sizing
	// this will happen at the first call in this routine
	if((m_pTitleBar == NULL) || (m_pTitleBar->GetSafeHwnd() == NULL))
		return;

	// if its allowed, now I will resize
	m_pTitleBar->GetClientRect(&crCaption);
    m_pTitleBar->MoveWindow(0, 0, crClient.Width(), crCaption.Height());
	}

void COXRollup::OnActivate(UINT nState, CWnd* pOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState,pOther,bMinimized);

	// now draw the caption bars
	if((WA_ACTIVE == nState) || (WA_CLICKACTIVE == nState))
	{
		DrawCaption(COLOR_ACTIVECAPTION);
	}

	if(WA_INACTIVE==nState)
	{
		DrawCaption(COLOR_INACTIVECAPTION);
	}
}


void COXRollup::DrawCaption(int nDisplayElement)
	{
	// activation automatically draws menu bar
	m_pTitleBar->SetActivation(nDisplayElement);
	}

void COXRollup::OnTimer(UINT nIDEvent)
	{
	// this is not my timer event...
	if ((m_myTimerID)!= nIDEvent)
		return;

	if (SCROLL_DOWN == m_SCROLL_HELP.direction)
		{
		if (m_SCROLL_HELP.curstep != m_SCROLL_HELP.steps)
			{
			SetWindowPos(NULL, 0, 0, m_SCROLL_HELP.width,
				m_SCROLL_HELP.capHeight + m_SCROLL_HELP.decrement * m_SCROLL_HELP.curstep,
				SWP_NOMOVE | SWP_NOZORDER);
			m_SCROLL_HELP.curstep++;
			}
		else
			{
			SetWindowPos(NULL, 0, 0, m_SCROLL_HELP.width, m_SCROLL_HELP.height,
				SWP_NOMOVE | SWP_NOZORDER);	

			if (m_bResizingFrame)
				{
				long style = ::GetWindowLongPtr(m_hWnd,GWL_STYLE);
				style |= WS_THICKFRAME;
				style &= ~WS_BORDER;
				::SetWindowLongPtr(m_hWnd, GWL_STYLE, style); 

				// Now make Windows redraw the window.
				SetWindowPos(NULL, 0, 0, 0, 0,
							 SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_FRAMECHANGED);
				}

			// kill the timer
			KillTimer(m_myTimerID);
			m_bRolling = FALSE;
			}
		} // end scroll down
	else if (SCROLL_UP == m_SCROLL_HELP.direction)
		{	
		if (m_SCROLL_HELP.curstep != m_SCROLL_HELP.steps)
			{
			SetWindowPos(NULL, 0, 0, m_SCROLL_HELP.width,
				m_SCROLL_HELP.height - m_SCROLL_HELP.decrement * m_SCROLL_HELP.curstep,
				SWP_NOMOVE | SWP_NOZORDER);
			m_SCROLL_HELP.curstep++;			
			}
		else
			{
			// last final positioning
			SetWindowPos(NULL, 0, 0, m_SCROLL_HELP.width, m_SCROLL_HELP.capHeight,
				SWP_NOMOVE | SWP_NOZORDER);			
			
			// set back to thinframe
			if (m_bResizingFrame)
				{
				long style = ::GetWindowLongPtr(m_hWnd,GWL_STYLE);
				style &= ~WS_THICKFRAME;
				style |= WS_BORDER;
				::SetWindowLongPtr(m_hWnd,GWL_STYLE,style);

				// Now make Windows redraw the window.
				SetWindowPos(NULL, 0, 0, 0, 0,
					 SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_FRAMECHANGED);
				}

			KillTimer(m_myTimerID);
			m_bRolling = FALSE;
			}
		} // end scroll up
	}

// Intent: Works like COleServerDoc::GetEmbeddedItem
//		   One time construction per menu needed	
CMenu* COXRollup::GetSysMenu()
	{
	if (NULL == m_pSysMenu)
		{
		m_pSysMenu = OnGetSysMenu();
		}

	ASSERT_VALID(m_pSysMenu);	// this check is for your override of OnGet...
	return m_pSysMenu;
	}

// Intent: Works like COleServerDoc::OnGetEmbeddedItem
//		   Constructs a menu once to be used further
CMenu* COXRollup::OnGetSysMenu()
	{
	// this is our base implementation; you can override it if you want to
	CMenu *pSysMenu = new CMenu();
	VERIFY(pSysMenu->CreatePopupMenu());

	// add all menu items; for strings see string table in OXRollup.rc
	CString sMenuText;
	sMenuText.LoadString(IDS_OX_RUM_ROLLUP);
	pSysMenu->AppendMenu(MF_STRING | MF_ENABLED, IDM_OX_RU_ROLLUP, sMenuText);
	sMenuText.LoadString(IDS_OX_RUM_ROLLDOWN);
	pSysMenu->AppendMenu(MF_STRING | MF_ENABLED, IDM_OX_RU_ROLLDOWN, sMenuText);
	pSysMenu->AppendMenu(MF_SEPARATOR);
	sMenuText.LoadString(IDS_OX_RUM_ARRANGE);
	pSysMenu->AppendMenu(MF_STRING | MF_ENABLED, IDM_OX_RU_ARRANGE, sMenuText);
	sMenuText.LoadString(IDS_OX_RUM_ARRANGEALL);
	pSysMenu->AppendMenu(MF_STRING | MF_ENABLED, IDM_OX_RU_ARRANGEALL, sMenuText);
	pSysMenu->AppendMenu(MF_SEPARATOR);
	sMenuText.LoadString(IDS_OX_RUM_ABOUT);
	pSysMenu->AppendMenu(MF_STRING | MF_ENABLED, IDM_OX_RU_ABOUT, sMenuText);
	sMenuText.LoadString(IDS_OX_RUM_CLOSE);
	pSysMenu->AppendMenu(MF_STRING | MF_ENABLED, IDM_OX_RU_CLOSE, sMenuText);
	
	ASSERT_VALID(pSysMenu);
	return pSysMenu;
	}

int COXRollup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
	{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// this happens if: called CreateRollUp, closed and 
	// reopened via CreateRollUp
	if (m_pTitleBar != NULL)
		{
		ASSERT(m_pTitleBar->GetSafeHwnd() == NULL);
		}
	else
		{
		if ((m_pTitleBar = OnGetTitleBar()) == NULL)
			return -1;
		}
	// you have to derive from COXTitleBar!!!
	ASSERT_VALID(m_pTitleBar);
	ASSERT(m_pTitleBar->IsKindOf(RUNTIME_CLASS(COXTitleBar)));
	
	CRect rcTBar;
	GetTitleBarRect(rcTBar);
	if (! m_pTitleBar->Create(m_sCaption, rcTBar, this, IDC_CAPTION))
		return -1;
	
	return 0;
	}

void COXRollup::OnDestroy() 
	{
	CDialog::OnDestroy();
	}

COXTitleBar* COXRollup::OnGetTitleBar()
	{
	ASSERT_VALID(this);
	
	// here your override should create an own window
	COXTitleBar* pTitleBar = new COXTitleBar();
	return pTitleBar;
	}

void COXRollup::GetTitleBarRect(CRect& rcTBarRect)
	{
	GetClientRect(rcTBarRect);
	rcTBarRect.bottom = TBAR_HEIGHT;
	}


/////////////////////////////////////////////////////////////////////////////
// COXRollup::Arrange - arrange this rollup

void COXRollup::Arrange()
	{
	ASSERT_VALID(this);
	if (IsArranged()) return;
	
	CWnd* pMainWnd = AfxGetMainWnd();
	ASSERT((pMainWnd != NULL) && ::IsWindow(pMainWnd->m_hWnd));		// don´t care about OLE
	if (!pMainWnd->IsWindowEnabled() && pMainWnd->IsIconic())
		return;
	
	InternalRollUp();
	
	COXRollup* pRollup = NULL;
	if (!m_ArrangedRollups.IsEmpty())
		pRollup = (COXRollup*)m_ArrangedRollups.GetTail();	// get last rolled up element
	
	// we have got a rollup (or not...)
	CPoint topLeft(0,0);
	if (pRollup == NULL)
		{
		// we found no college that was rolled up, so we arrange to the left of the screen
		CRect rcLeftOver;
		CWnd* pWndLeftOverPane = pMainWnd->GetDlgItem(AFX_IDW_PANE_FIRST);
		ASSERT(pWndLeftOverPane != NULL);
		pWndLeftOverPane->GetWindowRect(&rcLeftOver);
		topLeft = CPoint(rcLeftOver.left, rcLeftOver.top);
		}
	else
		{
		ASSERT(pRollup != this);		// can´t happen, otherwise we would have been arranged
		ASSERT(pRollup->IsRolledUp());	// must be up if arranged
		CRect rcLastRollup;
		pRollup->GetWindowRect(&rcLastRollup);
		topLeft = CPoint(rcLastRollup.left, rcLastRollup.bottom - 1);
		}
	
	SetWindowPos(NULL, topLeft.x, topLeft.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	m_ArrangedRollups.AddTail(this);
	m_bIsArranged = TRUE;			
	}

/////////////////////////////////////////////////////////////////////////////
// COXRollup::ArrangeAll - all open rollups are arranged

void COXRollup::ArrangeAll()
	{
	ASSERT_VALID(this);
	
	CWnd* pMainWnd = AfxGetMainWnd();
	ASSERT((pMainWnd != NULL) && ::IsWindow(pMainWnd->m_hWnd));		// don´t care about OLE
	if (!pMainWnd->IsWindowEnabled() && pMainWnd->IsIconic())
		return;
	
	// set out new top point; because we have to move to first position
	CWnd* pWndLeftOverPane = pMainWnd->GetDlgItem(AFX_IDW_PANE_FIRST);
	ASSERT(pWndLeftOverPane != NULL);
	
	CRect rcWndNewPos;
	pWndLeftOverPane->GetWindowRect(&rcWndNewPos);
	int nLeft = rcWndNewPos.left;
	int nNewTop = rcWndNewPos.top;
	
	POSITION pos = m_ArrangedRollups.GetHeadPosition();
	COXRollup* pRoll = NULL;
	while (pos != NULL)
		{
		pRoll = (COXRollup*)m_ArrangedRollups.GetNext(pos);
		pRoll->GetWindowRect(&rcWndNewPos);
		pRoll->SetWindowPos(NULL, nLeft, nNewTop, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
		nNewTop += rcWndNewPos.Height() - 1;
		}
	// now add the ones that haven´t been rolled up before
	POSITION posNotArranged = m_RollupList.GetHeadPosition();
	while (posNotArranged != NULL)
		{
		pRoll = (COXRollup*)m_ArrangedRollups.GetNext(posNotArranged);
		if (m_ArrangedRollups.Find(pRoll, NULL) == NULL)
			{
			if ((::IsWindow(pRoll->m_hWnd)) && pRoll->IsWindowVisible())
				{
				pRoll->InternalRollUp();
				pRoll->SetArrangeFlagTrue();
				pRoll->GetWindowRect(&rcWndNewPos);
				pRoll->SetWindowPos(NULL, nLeft, nNewTop, 0, 0,SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
				nNewTop += rcWndNewPos.Height() - 1;
				m_ArrangedRollups.AddTail(pRoll);	// add it to list
				}
			}
		}
	}

/////////////////////////////////////////////////////////////////////////////
// COXRollup::ReArrangeArranged - call this function if your application moves

void COXRollup::ReArrangeArranged()
	{
	POSITION pos = m_ArrangedRollups.GetHeadPosition();
	if (pos == NULL)
		return;	// this function has to be as fast as possible
	
	CWnd* pMainWnd = AfxGetMainWnd();
	ASSERT((pMainWnd != NULL) && ::IsWindow(pMainWnd->m_hWnd));		// don´t care about OLE
	if (!pMainWnd->IsWindowEnabled() && pMainWnd->IsIconic())
		return;
	
	// set out new top point; because we have to move to first position
	CWnd* pWndLeftOverPane = pMainWnd->GetDlgItem(AFX_IDW_PANE_FIRST);
	ASSERT(pWndLeftOverPane != NULL);
	
	CRect rcWndNewPos;
	pWndLeftOverPane->GetWindowRect(&rcWndNewPos);
	int nLeft = rcWndNewPos.left;
	int nNewTop = rcWndNewPos.top;
	
	COXRollup* pRoll = NULL;
	while (pos != NULL)
		{
		pRoll = (COXRollup*)m_ArrangedRollups.GetNext(pos);
		pRoll->GetWindowRect(&rcWndNewPos);
		pRoll->SetWindowPos(NULL, nLeft, nNewTop, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
		nNewTop += rcWndNewPos.Height() - 1;
		}
	}

/////////////////////////////////////////////////////////////////////////////
// COXRollup::InternalRollUp - roll up quickly

void COXRollup::InternalRollUp()
	{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(m_pTitleBar != NULL);
	
	if (m_bRolledUp)
		return;	// we are up, get out
	
	CRect rcWnd;
	if (m_bResizingFrame || m_xWidth == -1)
		{
		GetWindowRect(&rcWnd);
		m_xWidth = rcWnd.Width();
		m_yHeight = rcWnd.Height();
		// set back to thickframe
		if (m_bResizingFrame)
			{
			long style = ::GetWindowLongPtr(m_hWnd,GWL_STYLE);
			style &= ~WS_THICKFRAME;
			style |= WS_BORDER;
			::SetWindowLongPtr(m_hWnd,GWL_STYLE,style);
			}
		}
	
	m_pTitleBar->GetClientRect(&rcWnd);
	int nFrameSize = 2 * ::GetSystemMetrics(SM_CYBORDER);
	
	SetWindowPos(NULL, 0, 0, m_xWidth, rcWnd.Height() + nFrameSize,	SWP_NOMOVE | SWP_NOZORDER);			
	m_bRolledUp = TRUE;
	m_pTitleBar->SetRollupState(SCROLL_DOWN);
	Invalidate();
	}

void COXRollup::PostNcDestroy()
	{
	// why this unarrange here and not in the destructor?
	// You would get a GP-fault when referencing the main wnd if you have arranged rollups
	// at application termination time, so this is the last valid time to do it safely
	UnArrange();
	CDialog::PostNcDestroy();
	}

void COXRollup::UnArrange()
	{
	ASSERT_VALID(this);
	m_bIsArranged = FALSE;

	// we have to find ourselves in the list
	POSITION pos = m_ArrangedRollups.Find(this, NULL);
	if (pos != NULL )
		{
		// first: all other rollups (arranged ones) move up one step
		COXRollup* pNext = (COXRollup*)m_ArrangedRollups.GetAt(pos);
		POSITION posCur = pos;
		CRect rcWndNewPos;
		int nLeft, nNewTop;
		
		if (m_ArrangedRollups.GetHeadPosition() == posCur)
			{
			// set out new top point; because we have to move to first position
			CWnd* pWndLeftOverPane = AfxGetMainWnd()->GetDlgItem(AFX_IDW_PANE_FIRST);
			ASSERT(pWndLeftOverPane != NULL);
			pWndLeftOverPane->GetWindowRect(&rcWndNewPos);
			nLeft = rcWndNewPos.left;
			nNewTop = rcWndNewPos.top;
			}
		else											   
			{
			POSITION posPrev = posCur;
			m_ArrangedRollups.GetPrev(posPrev);	// we skip this element
			COXRollup* pPrev = (COXRollup*)m_ArrangedRollups.GetPrev(posPrev);
			pPrev->GetWindowRect(&rcWndNewPos);
			nLeft = rcWndNewPos.left;
			nNewTop = rcWndNewPos.bottom - 1;
			}
		
		m_ArrangedRollups.GetNext(posCur);	// we skip this element
		while (posCur != NULL)	// only if we are not last element
			{
			pNext = (COXRollup*)m_ArrangedRollups.GetNext(posCur);
			pNext->GetWindowRect(&rcWndNewPos);
			pNext->SetWindowPos(NULL, nLeft, nNewTop, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
			nNewTop += rcWndNewPos.Height() - 1;
			}
		
		m_ArrangedRollups.RemoveAt(pos);		// now we delete the element from the list
		}
	}
