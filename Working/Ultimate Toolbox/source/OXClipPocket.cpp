// OXClipPocket.cpp : implementation file
//
// Version: 9.3


#include "stdafx.h"
#include "OXClipPocket.h"

#include "OXMulticlipboardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXClipPocket

IMPLEMENT_DYNAMIC(COXClipPocket,CButton)

COXClipPocket::COXClipPocket(COXMulticlipboardDlg* pParent) :
	m_bMouseOver(FALSE),m_bLButtonDown(FALSE),
	m_pParent(pParent)
{
	 ASSERT(pParent);
}

COXClipPocket::~COXClipPocket()
{
}


BEGIN_MESSAGE_MAP(COXClipPocket, CButton)
	//{{AFX_MSG_MAP(COXClipPocket)
	ON_WM_NCHITTEST()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_NCMOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXClipPocket message handlers


void COXClipPocket::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rect;
	GetClientRect(&rect);

	//draw pushed/unpushed state
	if (m_data.nFormat && m_bMouseOver)
	{
		if (m_bLButtonDown)
			dc.DrawFrameControl(&rect, DFC_BUTTON , 
				DFCS_BUTTONPUSH | DFCS_PUSHED);
		else
			dc.DrawFrameControl(&rect, DFC_BUTTON,DFCS_BUTTONPUSH);
	}
	//inactive state - do not draw frame rect, fill rect out
	else
		dc.FillRect(&rect,CBrush::FromHandle(::GetSysColorBrush(COLOR_BTNFACE)));

	//draw icon
	ASSERT(m_pParent);
	UINT nEdge=m_pParent->GetEdgeOffset();
	rect.DeflateRect(nEdge,nEdge);

	CPoint pt(nEdge,nEdge);
	if (m_bLButtonDown && m_data.nFormat)
	{
		pt.x+=1;
		pt.y+=1;
	}

	if (m_data.nFormat >= OX_CLIP_CUSTOM_FORMAT)
		m_pParent->m_imgList.Draw(&dc, OX_CLIP_CUSTOM_FORMAT, pt,ILD_NORMAL);
	else
		m_pParent->m_imgList.Draw(&dc, m_data.nFormat, pt,ILD_NORMAL);

	
}
#if _MSC_VER == 1310	// VS 2003 - this shouldn't be necessary
	UINT COXClipPocket::OnNcHitTest(CPoint point)
#else
	LRESULT COXClipPocket::OnNcHitTest(CPoint point)
#endif
{
	LRESULT nRet=CButton::OnNcHitTest(point);

	//check position of the cursor and "popup" frame rect

	if (m_data.nFormat==OX_CLIP_CF_NONE)
		return nRet;

	if (m_hWnd!=::GetCapture())
		SetCapture();
	
	CRect rect;
	GetWindowRect(&rect);


	if (rect.PtInRect(point))
	{
		if (!m_bMouseOver)
		{
			m_bMouseOver=TRUE;
			RedrawWindow();
		}
	}
	return nRet;
}

void COXClipPocket::OnMouseMove(UINT nFlags, CPoint point) 
{
	//if cursor gone outside of the rect remove frame
	if (m_hWnd!=::GetCapture())
		SetCapture();
	
	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);

	if (!rect.PtInRect(point))
	{
		m_bMouseOver=FALSE;
		ReleaseCapture();
	
		if (m_data.nFormat)
		{
			RedrawWindow();
		}
	}
	
	CButton::OnMouseMove(nFlags, point);
}



UINT COXClipPocket::GetDataType() const
{
	return m_data.nFormat;
}

BOOL COXClipPocket::Create(const POINT& ptTopLeft, UINT nID) 
{

	//the function actually creates window
	//with the rect dimension equals 
	//sum(2*COXMulticlipboardDlg::m_nEdgeOffset+width/height of the icon)
	//the ptTopLeft should point to the leftpoint of the icon
	//this size actually will be inflated by m_nEdgeOffset 
	ASSERT(m_pParent);
	ASSERT(::IsWindow(m_pParent->m_hWnd));
	
	CRect rect;
	m_pParent->GetClientRect(&rect);
	
	if (!rect.PtInRect(ptTopLeft))
	{
		TRACE0("COXClipPocket::Create() invalid point");
		return NULL;
	}
	
	CRect rectThis;
	rectThis.left=ptTopLeft.x;
	rectThis.top=ptTopLeft.y;
	rectThis.right=rectThis.left+m_pParent->GetIconWidth();
	rectThis.bottom=rectThis.top+m_pParent->GetIconHeight();
	rectThis.InflateRect(m_pParent->GetEdgeOffset(),m_pParent->GetEdgeOffset());

	if (!rect.PtInRect(rectThis.BottomRight()))
	{
		TRACE0("COXClipPocket::Create() invalid point");
		return NULL;
	}

	
	return CButton::Create(NULL,WS_CHILD | WS_VISIBLE, rectThis,
		m_pParent, nID);
}


void COXClipPocket::OnLButtonDown(UINT nFlags, CPoint point) 
{

	m_bLButtonDown=TRUE;
	if (m_data.nFormat)
	{
		CButton::OnLButtonDown(nFlags, point);
		RedrawWindow();
	}
}

void COXClipPocket::OnLButtonUp(UINT nFlags, CPoint point) 
{

	m_bLButtonDown=FALSE;
	if (m_data.nFormat)
	{
		CButton::OnLButtonUp(nFlags, point);
		RedrawWindow();
	}
}



void COXClipPocket::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(point);
}

void COXClipPocket::SetData(UINT nType, STGMEDIUM* pData)
{
	m_data.SetData(nType,pData);
	RedrawWindow();
}

tOXData* COXClipPocket::GetData() 
{
	return &m_data;
}


void COXClipPocket::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	static int h=0;
	TRACE1("%d OnNCMouseMove\r\n",h++);	
	CButton::OnNcMouseMove(nHitTest, point);
}

void COXClipPocket::OnCaptureChanged(CWnd *pWnd) 
{
	m_bMouseOver=FALSE;
	RedrawWindow();
	CButton::OnCaptureChanged(pWnd);
}
