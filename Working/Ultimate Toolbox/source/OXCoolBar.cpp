// COXCoolBar implements coolbars for MFC.
//
// Version: 9.3


#include "StdAfx.h"
#include "OXCoolBar.h"

#include "UTBStrOp.h"
#include "UTB64Bit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if _MFC_VER>=0x0420

//////////////////////////////////////
// Programmer-friendly REBARBANDINFO 
//////////////////////////////////////

////////////////////////////////////////////////////
// all GetXXX functions return FALSE if requested 
// property is not set in bands mask
////////////////////////////////////////////////////

////////////////////////////////////////////////////
BOOL COXCoolBarBandInfo::GetStyle(UINT& nStyle)
{
	if(fMask&RBBIM_STYLE)
	{
		nStyle=fStyle;
		return TRUE;
	}
	return FALSE;
}

BOOL COXCoolBarBandInfo::SetStyle(UINT nStyle)
{
	fMask|=RBBIM_STYLE;
	fStyle=nStyle;
	return TRUE;
}

BOOL COXCoolBarBandInfo::GetColor(COLORREF& crColor)
{
	if(fMask&RBBIM_COLORS)
	{
		crColor=clrFore;
		return TRUE;
	}
	return FALSE;
}

BOOL COXCoolBarBandInfo::SetColor(COLORREF crColor)
{
	fMask|=RBBIM_COLORS;
	clrFore=crColor;
	return TRUE;
}

BOOL COXCoolBarBandInfo::GetBkColor(COLORREF& crColor)
{
	if(fMask&RBBIM_COLORS)
	{
		crColor=clrBack;
		return TRUE;
	}
	return FALSE;
}

BOOL COXCoolBarBandInfo::SetBkColor(COLORREF crColor)
{
	fMask|=RBBIM_COLORS;
	clrBack=crColor;
	return TRUE;
}

BOOL COXCoolBarBandInfo::GetText(CString& sText)
{
	if(fMask&RBBIM_TEXT)
	{
		sText=CString(lpText);
		return TRUE;
	}
	return FALSE;
}

BOOL COXCoolBarBandInfo::SetText(LPTSTR sText)
{
	// allocate memory on heap to save text
	fMask|=RBBIM_TEXT;
	ASSERT(!(bDynText && lpText==NULL));
	int len = 0;
	if(!bDynText)
	{
		len = PtrToInt((_tcslen(sText)+1)*sizeof(TCHAR));
		lpText=(LPTSTR)malloc(len);
		bDynText=TRUE;
	}
	else
	{
		len = PtrToInt((_tcslen(sText)+1)*sizeof(TCHAR));
		lpText=(LPTSTR)realloc(lpText, len);
	}
	if(lpText!=NULL)
	{
		UTBStr::tcscpy(lpText, len, sText);
		return TRUE;
	}
	return FALSE;
}

BOOL COXCoolBarBandInfo::GetImage(int& nImage)
{
	if(fMask&RBBIM_IMAGE)
	{
		nImage=iImage;
		return TRUE;
	}
	return FALSE;
}

BOOL COXCoolBarBandInfo::SetImage(int nImage)
{
	fMask|=RBBIM_IMAGE;
	iImage=nImage;
	return TRUE;
}

BOOL COXCoolBarBandInfo::GetChildWindow(HWND& hWnd)
{
	if(fMask&RBBIM_CHILD)
	{
		hWnd=hwndChild;
		return TRUE;
	}
	return FALSE;
}

BOOL COXCoolBarBandInfo::SetChildWindow(CWnd* pChildWnd)
{
	ASSERT_VALID(pChildWnd);   // must have a child window

	fMask|=RBBIM_CHILD;
	hwndChild=pChildWnd->GetSafeHwnd();
	return TRUE;
}

BOOL COXCoolBarBandInfo::GetChildWindowMinSize(CSize& sizeMin)
{
	if(fMask&RBBIM_CHILDSIZE)
	{
		sizeMin.cx=cxMinChild;
		sizeMin.cy=cyMinChild;
		return TRUE;
	}
	return FALSE;
}

BOOL COXCoolBarBandInfo::SetChildWindowMinSize(UINT cxMin, UINT cyMin)
{
	fMask|=RBBIM_CHILDSIZE;
	cxMinChild=cxMin;
	cyMinChild=cyMin;
	return TRUE;
}

BOOL COXCoolBarBandInfo::GetWidth(UINT& nWidth)
{
	if(fMask&RBBIM_SIZE)
	{
		nWidth=cx;
		return TRUE;
	}
	return FALSE;
}

BOOL COXCoolBarBandInfo::SetWidth(UINT nWidth)
{
	fMask|=RBBIM_SIZE;
	cx=nWidth;
	return TRUE;
}

// If you used COXCoolBar::AddBitmap function to create background
// bitmap object then never destroy hBitmap yourself.
// It's up to CoolBar to do that
BOOL COXCoolBarBandInfo::GetBackgroundBitmap(HBITMAP& hBitmap)
{
	if(fMask&RBBIM_BACKGROUND)
	{
		hBitmap=hbmBack;
		return TRUE;
	}
	return FALSE;
}

// It is your responsibility to create and destroy background bitmap object. 
// To simplify this task use COXCoolBar::AddBitmap function
// which creates bitmap from resource and returns handle to it.
// When corresponding CoolBar destroyed it will destroy all bitmaps
// created using COXCoolBar::AddBitmap function
// returns TRUE if successful, or FALSE otherwise
BOOL COXCoolBarBandInfo::SetBackgroundBitmap(HBITMAP hBitmap)
{
	if(hBitmap!=NULL)
	{
		fMask|=RBBIM_BACKGROUND;
		hbmBack=hBitmap;
		return TRUE;
	}
	return FALSE;
}

BOOL COXCoolBarBandInfo::GetID(UINT& nID)
{
	if(fMask&RBBIM_ID)
	{
		nID=wID;
		return TRUE;
	}
	return FALSE;
}

BOOL COXCoolBarBandInfo::SetID(UINT nID)
{
	// band's ID cannot be zero
	if(nID!=0)
	{
		fMask|=RBBIM_ID;
		wID=nID;
		return TRUE;
	}
	return FALSE;
}

BOOL COXCoolBarBandInfo::GetMask(UINT& nMask)
{
	nMask=fMask;
	return TRUE;
}

BOOL COXCoolBarBandInfo::SetMask(UINT nMask)
{
	fMask=nMask;
	return TRUE;
}

/////////////
// save the state of the band in the registry
// next are saved if set:
//		style
//		min size of child window
//		initial width
//		text associated with the band
//		ID
//		zero index of image list in COXCoolBarInfo
//		zero index of band in the parent CoolBar (parameter nIndex)
/////////////
BOOL COXCoolBarBandInfo::SaveState(LPCTSTR lpszProfileName, int nIndex)
{
	ASSERT(nIndex>=0);

	CWinApp* pApp=AfxGetApp();

	if(( fMask & RBBIM_ID ) == 0 )
	{
		TRACE0("COXCoolBarBandInfo::SaveState: cannont save state of bar with unset ID\n");
		return FALSE;
	}

	UINT nMask=0;
	if(fMask&RBBIM_STYLE)
	{
		nMask|=RBBIM_STYLE;
		pApp->WriteProfileInt(lpszProfileName,szStyle,fStyle);
	}
	if(fMask&RBBIM_CHILDSIZE)
	{
		nMask|=RBBIM_CHILDSIZE;
		pApp->WriteProfileInt(lpszProfileName,szcxMin,cxMinChild);
		pApp->WriteProfileInt(lpszProfileName,szcyMin,cyMinChild);
	}
	if(fMask&RBBIM_SIZE)
	{
		nMask|=RBBIM_SIZE;
		pApp->WriteProfileInt(lpszProfileName,szWidth,cx);
	}
	if(fMask&RBBIM_TEXT)
	{
		nMask|=RBBIM_TEXT;
		pApp->WriteProfileString(lpszProfileName,szText,lpText);
	}
	if(fMask&RBBIM_IMAGE)
	{
		nMask|=RBBIM_IMAGE;
		pApp->WriteProfileInt(lpszProfileName,szImage,iImage);
	}
	if(fMask&RBBIM_ID)
	{
		nMask|=RBBIM_ID;
		pApp->WriteProfileInt(lpszProfileName,szID,wID);
	}
	if(nMask)
	{
		// if there is anything to save then save mask of 
		// saved band's properties
		pApp->WriteProfileInt(lpszProfileName,szMask,nMask);
		// save the band's order number
		pApp->WriteProfileInt(lpszProfileName,szIndex,nIndex);
	}
	
	// returns TRUE if saved anything, otherwise FALSE
	return nMask!=0;
}

BOOL COXCoolBarBandInfo::LoadState(LPCTSTR lpszProfileName)
{
	CWinApp* pApp=AfxGetApp();

	// get the mask of saved band's properties
	UINT nMask=pApp->GetProfileInt(lpszProfileName,szMask,0);
	if(nMask&RBBIM_STYLE)
	{
		if(!SetStyle(pApp->GetProfileInt(lpszProfileName,szStyle,0)))
		{
			return FALSE;
		}
	}
	if(nMask&RBBIM_CHILDSIZE)
	{
		if(!SetChildWindowMinSize(pApp->GetProfileInt(lpszProfileName,szcxMin,0),
			pApp->GetProfileInt(lpszProfileName,szcyMin,0)))
		{
			return FALSE;
		}
	}
	if(nMask&RBBIM_SIZE)
	{
		if(!SetWidth(pApp->GetProfileInt(lpszProfileName,szWidth,0)))
		{
			return FALSE;
		}
	}
	if(nMask&RBBIM_TEXT)
	{
		CString sText=pApp->GetProfileString(lpszProfileName,szText,_T(""));
		try
		{
			if(!SetText(sText.GetBuffer(sText.GetLength())))
			{
				sText.ReleaseBuffer();
				return FALSE;
			}
			sText.ReleaseBuffer();
		}
		catch(CMemoryException* e)
		{
			CString sMessage;
			VERIFY(sMessage.LoadString(IDS_OX_COOLBAROUTMEMORY));
			AfxMessageBox(sMessage,MB_ICONSTOP);//"Out of memory!"
			e->Delete();
			return FALSE;
		}
	}
	if(nMask&RBBIM_IMAGE)
	{
		if(!SetImage(pApp->GetProfileInt(lpszProfileName,szImage,-1)))
		{
			return FALSE;
		}
	}
	if(nMask&RBBIM_ID)
	{
		if(!SetID(pApp->GetProfileInt(lpszProfileName,szID,0)))
		{
			return FALSE;
		}
	}

	return TRUE;
}


//////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(COXCoolBar, CControlBar)

BEGIN_MESSAGE_MAP(COXCoolBar, CControlBar)
	//{{AFX_MSG_MAP(COXCoolBar)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(RBN_HEIGHTCHANGE, OnHeightChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

COXCoolBar::COXCoolBar()
{
}

COXCoolBar::~COXCoolBar()
{
	LPTSTR lpszResourceName;

	// delete all bitmaps that we associated with CoolBar
	HBITMAP hBitmap;
	POSITION pos=m_allBitmaps.GetStartPosition();
	while(pos!=NULL)
	{
		m_allBitmaps.GetNextAssoc(pos,lpszResourceName,hBitmap);
		::DeleteObject(hBitmap);
	}
	m_allBitmaps.RemoveAll();

	// delete all image lists that we associated with CoolBar
	HANDLE hImageList;
	pos=m_allImageLists.GetStartPosition();
	while(pos!=NULL)
	{
		m_allImageLists.GetNextAssoc(pos,lpszResourceName,hImageList);
		ImageList_Destroy((HIMAGELIST)hImageList);
	}
	m_allImageLists.RemoveAll();

	// empty all map arrays
	m_allBandText.RemoveAll();
	m_allBandMask.RemoveAll();
	m_allBandID.RemoveAll();
}

/////////////////////////
// Create coolbar
// !!!!!!!!!!!!!!!!!!!!!!
// !!!! at the moment we support only top alignment
// !!!!!!!!!!!!!!!!!!!!!!
/////////////////////////
BOOL COXCoolBar::Create(CWnd* pParentWnd, DWORD dwStyle,
	DWORD dwAfxBarStyle, UINT nID)
{
	ASSERT_VALID(pParentWnd);   // must have a parent

	// we support only top alignment
	dwAfxBarStyle=CBRS_ALIGN_TOP;

	// vertical coolbar not supported
	dwStyle&=~CCS_VERT;

	// dynamic coolbar not supported
	dwStyle&=~CBRS_SIZE_DYNAMIC;

	// save the style (this code copied from MFC--probably unecessary)
	m_dwStyle=dwAfxBarStyle;
	if (nID==AFX_IDW_TOOLBAR)
		m_dwStyle|=CBRS_HIDE_INPLACE;

	// MFC requires these:
	dwStyle|=CCS_NODIVIDER|CCS_NOPARENTALIGN;

	// Initialize cool common controls
	static BOOL bInitCoolBar=FALSE;
	if (!bInitCoolBar) 
	{
		INITCOMMONCONTROLSEX sex;
		sex.dwSize=sizeof(INITCOMMONCONTROLSEX);
		sex.dwICC=ICC_COOL_CLASSES;
		InitCommonControlsEx(&sex);
		bInitCoolBar = TRUE;
	}

	// Create the cool bar using style and parent.
	CRect rc;
	rc.SetRectEmpty();
	return CWnd::CreateEx(WS_EX_TOOLWINDOW, REBARCLASSNAME, NULL,
		dwStyle, rc, pParentWnd, nID);
}

//////////////////
// Standard UI handler updates any controls in the coolbar.
//
void COXCoolBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}

/////////////////
// These two functions are called by MFC to calculate the layout of
// the main frame. Since COXCoolBar is not designed to be dynamic, the
// size is always fixed, and the same as the window size. 
//
CSize COXCoolBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	UNREFERENCED_PARAMETER(nLength);
	return CalcFixedLayout(dwMode & LM_STRETCH, dwMode & LM_HORZ);
}

CSize COXCoolBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CRect rc;
	GetWindowRect(&rc);
	CSize sz(bHorz && bStretch ? 0x7FFF : rc.Width(),
		!bHorz && bStretch ? 0x7FFF : rc.Height());
	return sz;
}

//////////////////
// Low-level height-changed handler just passes to virtual 
// function with nicer arguments.
//
void COXCoolBar::OnHeightChange(NMHDR* pNMHDR, LRESULT* pRes)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	CRect rc;
	GetWindowRect(&rc);
	OnHeightChange(rc);
	*pRes = 0; // why not?
}

//////////////////
// Height changed:
// Notify the parent frame by posting a WM_SIZE message. This will cause the
// frame to do RecalcLayout. The message must be posted, not sent, because
// the coolbar could send RBN_HEIGHTCHANGE while the user is sizing, which
// would be in the middle of a CFrame::RecalcLayout, and RecalcLayout doesn't
// let you re-enter it. Posting gurantees that CFrameWnd can finish any recalc
// it may be in the middle of before handling my posted WM_SIZE. Very confusing.
//
void COXCoolBar::OnHeightChange(const CRect& rcNew)
{
	UNREFERENCED_PARAMETER(rcNew);
	CWnd* pParent = GetParent();
	CRect rc;
	pParent->GetWindowRect(&rc);
	pParent->PostMessage(WM_SIZE, 0, MAKELONG(rc.Width(),rc.Height()));
}

void COXCoolBar::OnPaint()
{
	Default();	// bypass CControlBar
}

BOOL COXCoolBar::OnEraseBkgnd(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	return (BOOL)Default();  // bypass CControlBar
}

/////////////////////
// there are two ways of getting/setting info to a band in CoolBar:
//		1)	you can instantiate object of COXCoolBarBandInfo class;
//			if band exists you can fill the object with band's info 
//				using COXCoolBar::GetBandInfo;
//			you can use COXCoolBarBandInfo::SetXXX functions to set
//				band's info
//			if you create new band use COXCoolBar::InsertBand to insert
//				the band into CoolBar;
//			if you change existing band use COXCoolBar::SetBandInfo to 
//				set the band's info;
/////////////////////
//		2)	you can use COXCoolBar::SetBandXXX and COXCoolBar::GetBandXXX
//			functions to set particular band's property
/////////////////////
//
// as seen from description you use first method when you want to insert
// new band into CoolBar (the only way to do that) or when you want to set
// or get many band's properties at one time;
// the second way is preferable when you want to set/get only a few band's
// properties at one time
/////////////////////
//
// in all GetBandXXX/SetBandXXX functions you use zero-based index number 
// of band in CoolBar excluding GetBandNumber function that you use 
// to define the number of band based on band's ID;
//
// as soon as every band in CoolBar has to have non zero, unique ID (by design)
// you can always get this number;
//
// therefore you have to save the IDs you set to bands in some place or
// if you use child windows within bands (almost always you do) you can 
// create these windows with the same ID as you set to corresponding band
// (it is safe because you can associate only one child window with a band)
// and call GetDlgCtrlID function of child window to get the ID of 
// corresponding band
//
/////////////////////

/////////////////////////////////////////////////////////////////////
// all functions return TRUE if successful, or FALSE otherwise;
// if any GetBandXXX function returns FALSE then 
// the second parameter is undefined
/////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////
BOOL COXCoolBar::GetBandInfo(UINT nBand,  COXCoolBarBandInfo* pBI)
{ 
	ASSERT(::IsWindow(m_hWnd));

	// if mask is not set then we assume 
	// that you want to get info on all set 
	// band's properties
	UINT nMask;
	if(!pBI->GetMask(nMask))
	{
		return FALSE;
	}
	if(nMask==0)
	{
		if(!GetBandMask(nBand,nMask))
		{
			return FALSE;
		}
		if(!pBI->SetMask(nMask))
		{
			return FALSE;
		}
	}

	if(SendMessage(RB_GETBANDINFO, (WPARAM)nBand, (LPARAM)pBI)) 
	{
		// if we set text to a band
		// then we have to load it from m_allBandText
		if(nMask&RBBIM_TEXT)
		{
			CString sText;
			if(!GetBandText(nBand,sText))
			{
				return FALSE;
			}
			// set text to pBI->lpText
			try
			{
				if(!pBI->SetText(sText.GetBuffer(sText.GetLength())))
				{
					sText.ReleaseBuffer();
					return FALSE;
				}
				sText.ReleaseBuffer();
			}
			catch(CMemoryException* e)
			{
				CString sMessage;
				VERIFY(sMessage.LoadString(IDS_OX_COOLBAROUTMEMORY));
				AfxMessageBox(sMessage,MB_ICONSTOP);//"Out of memory!"
				e->Delete();
				return FALSE;
			}
		}
		return TRUE;
	}
	return FALSE;
}

BOOL COXCoolBar::SetBandInfo(UINT nBand, COXCoolBarBandInfo* pBI)
{ 
	ASSERT(::IsWindow(m_hWnd));

	UINT nOldID;
	if(!GetBandID(nBand,nOldID))
	{
		return FALSE;
	}
	UINT nNewID;
	if(!pBI->GetID(nNewID))
	{
		return FALSE;
	}
	
	// check if the ID is changing
	if(nOldID!=nNewID)
	{
		if(!ChangeID(nOldID,nNewID))
		{
			return FALSE;
		}
	}

	if(SendMessage(RB_SETBANDINFO, (WPARAM)nBand, (LPARAM)pBI))
	{
		UINT nMask;
		if(!pBI->GetMask(nMask))
		{
			return FALSE;
		}
		// save current mask
		if(!CheckInBandMask(nBand,nMask))
		{
			return FALSE;
		}
		// save current text if set
		if(nMask&RBBIM_TEXT)
		{
			CString sText;
			if(!pBI->GetText(sText))
			{
				return FALSE;
			}
			if(!CheckInBandText(nBand,sText))
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL COXCoolBar::InsertBand(UINT nWhere, COXCoolBarBandInfo* pBI)
{ 
	ASSERT(::IsWindow(m_hWnd));

	if(nWhere==-1)
	{
		nWhere=GetBandCount();
	}

	UINT nID;
	if(!pBI->GetID(nID))
	{
		return FALSE;
	}
	// as soon as we inserting new band
	// we have to check the band's ID
	// and if it is Ok save it in m_allBandID
	if(!CheckInBandID(nID,TRUE))
	{
		return FALSE;
	}

	if(SendMessage(RB_INSERTBAND, (WPARAM)nWhere, (LPARAM)pBI))
	{
		UINT nMask;
		if(!pBI->GetMask(nMask))
		{
			return FALSE;
		}
		// save current mask
		if(!CheckInBandMask(nWhere,nMask))
		{
			return FALSE;
		}
		// save current text if set
		if(nMask&RBBIM_TEXT)
		{
			CString sText;
			if(!pBI->GetText(sText))
			{
				return FALSE;
			}
			if(!CheckInBandText(nWhere,sText))
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL COXCoolBar::DeleteBand(UINT nWhich)
{ 
	ASSERT(::IsWindow(m_hWnd));

	UINT nID;
	if(!GetBandID(nWhich,nID))
	{
		return FALSE;
	}

	if(SendMessage(RB_DELETEBAND, (WPARAM)nWhich))
	{
		// delete corresponding entries in the map arrays
		m_allBandText.RemoveKey(nID);
		m_allBandMask.RemoveKey(nID);
		m_allBandID.RemoveKey(nID);

		return TRUE;
	}

	return FALSE;
}



/////////////////////////////////////////////////////////////////////
// all GetBandXXX function get band's mask and check 
// if requested property was previously set
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// all functions return TRUE if successful, or FALSE otherwise;
// if any GetBandXXX function returns FALSE then 
// the second parameter is undefined
/////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
BOOL COXCoolBar::GetBandStyle(UINT nBand, UINT& nStyle)
{
	ASSERT(::IsWindow(m_hWnd));

	UINT nMask;
	if(GetBandMask(nBand,nMask) && nMask&RBBIM_STYLE)
	{
		COXCoolBarBandInfo bi;
		if(bi.SetMask(RBBIM_STYLE) && GetBandInfo(nBand,&bi))
		{
			if(bi.GetStyle(nStyle))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL COXCoolBar::SetBandStyle(UINT nBand, UINT nStyle)
{
	ASSERT(::IsWindow(m_hWnd));

	COXCoolBarBandInfo bi;
	if(GetBandInfo(nBand,&bi))
	{
		if(bi.SetStyle(nStyle) && SetBandInfo(nBand,&bi))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL COXCoolBar::GetBandColor(UINT nBand, COLORREF& crColor)
{
	ASSERT(::IsWindow(m_hWnd));

	UINT nMask;
	if(GetBandMask(nBand,nMask) && nMask&RBBIM_COLORS)
	{
		COXCoolBarBandInfo bi;
		if(bi.SetMask(RBBIM_COLORS) && GetBandInfo(nBand,&bi))
		{
			if(bi.GetColor(crColor))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL COXCoolBar::SetBandColor(UINT nBand, COLORREF crColor)
{
	ASSERT(::IsWindow(m_hWnd));

	COXCoolBarBandInfo bi;
	if(GetBandInfo(nBand,&bi))
	{
		if(bi.SetColor(crColor) && SetBandInfo(nBand,&bi))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL COXCoolBar::GetBandBkColor(UINT nBand, COLORREF& crColor)
{
	ASSERT(::IsWindow(m_hWnd));

	UINT nMask;
	if(GetBandMask(nBand,nMask) && nMask&RBBIM_COLORS)
	{
		COXCoolBarBandInfo bi;
		if(bi.SetMask(RBBIM_COLORS) && GetBandInfo(nBand,&bi))
		{
			if(bi.GetBkColor(crColor))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL COXCoolBar::SetBandBkColor(UINT nBand, COLORREF crColor)
{
	ASSERT(::IsWindow(m_hWnd));

	COXCoolBarBandInfo bi;
	if(GetBandInfo(nBand,&bi))
	{
		if(bi.SetBkColor(crColor) && SetBandInfo(nBand,&bi))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL COXCoolBar::GetBandText(UINT nBand, CString& sText)
{
	ASSERT(::IsWindow(m_hWnd));

	UINT nMask;
	if(GetBandMask(nBand,nMask) && nMask&RBBIM_TEXT)
	{
		// use info saved in m_allBandText to get band's text
		UINT nID;
		if(GetBandID(nBand,nID))
		{
			if(m_allBandText.Lookup(nID,sText))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL COXCoolBar::SetBandText(UINT nBand, LPTSTR sText)
{
	ASSERT(::IsWindow(m_hWnd));

	COXCoolBarBandInfo bi;
	if(GetBandInfo(nBand,&bi))
	{
		if(bi.SetText(sText) && SetBandInfo(nBand,&bi))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL COXCoolBar::GetBandMask(UINT nBand, UINT& nMask)
{
	ASSERT(::IsWindow(m_hWnd));

	// use info saved in m_allBandMask to get band's mask
	UINT nID;
	if(GetBandID(nBand,nID))
	{
		if(m_allBandMask.Lookup(nID,nMask))
		{
			return TRUE;
		}
	}
	return FALSE;
}

// be very careful using this function because 
// it doesn't change the band's properties explicitly
// but implicitly when you call other SetBandXXX and GetBandXXX functions
BOOL COXCoolBar::SetBandMask(UINT nBand, UINT nMask)
{
	ASSERT(::IsWindow(m_hWnd));

	UINT nID;
	if(GetBandID(nBand,nID))
	{
		m_allBandMask.SetAt(nID,nMask);
		return TRUE;
	}
	return FALSE;
}

BOOL COXCoolBar::GetBandImage(UINT nBand, int& nImage)
{
	ASSERT(::IsWindow(m_hWnd));

	UINT nMask;
	if(GetBandMask(nBand,nMask) && nMask&RBBIM_IMAGE)
	{
		COXCoolBarBandInfo bi;
		if(bi.SetMask(RBBIM_IMAGE) && GetBandInfo(nBand,&bi))
		{
			if(bi.GetImage(nImage))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL COXCoolBar::SetBandImage(UINT nBand, int nImage)
{
	ASSERT(::IsWindow(m_hWnd));

	COXCoolBarBandInfo bi;
	if(GetBandInfo(nBand,&bi))
	{
		if(bi.SetImage(nImage) && SetBandInfo(nBand,&bi))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL COXCoolBar::GetBandChildWindow(UINT nBand, HWND& hChildWnd)
{
	ASSERT(::IsWindow(m_hWnd));

	UINT nMask;
	if(GetBandMask(nBand,nMask) && nMask&RBBIM_CHILD)
	{
		COXCoolBarBandInfo bi;
		if(bi.SetMask(RBBIM_CHILD) && GetBandInfo(nBand,&bi))
		{
			if(bi.GetChildWindow(hChildWnd))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL COXCoolBar::SetBandChildWindow(UINT nBand, CWnd* pChildWnd)
{
	ASSERT(::IsWindow(m_hWnd));

	COXCoolBarBandInfo bi;
	if(GetBandInfo(nBand,&bi))
	{
		if(bi.SetChildWindow(pChildWnd) && SetBandInfo(nBand,&bi))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL COXCoolBar::GetBandChildWindowMinSize(UINT nBand, CSize& sizeChild)
{
	ASSERT(::IsWindow(m_hWnd));

	UINT nMask;
	if(GetBandMask(nBand,nMask) && nMask&RBBIM_CHILDSIZE)
	{
		COXCoolBarBandInfo bi;
		if(bi.SetMask(RBBIM_CHILDSIZE) && GetBandInfo(nBand,&bi))
		{
			if(bi.GetChildWindowMinSize(sizeChild))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL COXCoolBar::SetBandChildWindowMinSize(UINT nBand, UINT cxMin, UINT cyMin)
{
	ASSERT(::IsWindow(m_hWnd));

	COXCoolBarBandInfo bi;
	if(GetBandInfo(nBand,&bi))
	{
		if(bi.SetChildWindowMinSize(cxMin,cyMin) && SetBandInfo(nBand,&bi))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL COXCoolBar::GetBandWidth(UINT nBand, UINT& nWidth)
{
	ASSERT(::IsWindow(m_hWnd));

	UINT nMask;
	if(GetBandMask(nBand,nMask) && nMask&RBBIM_SIZE)
	{
		COXCoolBarBandInfo bi;
		if(bi.SetMask(RBBIM_SIZE) && GetBandInfo(nBand,&bi))
		{
			if(bi.GetWidth(nWidth))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL COXCoolBar::SetBandWidth(UINT nBand, UINT nWidth)
{
	ASSERT(::IsWindow(m_hWnd));

	COXCoolBarBandInfo bi;
	if(GetBandInfo(nBand,&bi))
	{
		if(bi.SetWidth(nWidth) && SetBandInfo(nBand,&bi))
		{
			return TRUE;
		}
	}
	return FALSE;
}

// If you used COXCoolBar::AddBitmap function to create background
// bitmap object then never destroy hBitmap yourself.
// It's up to CoolBar to do that
BOOL COXCoolBar::GetBandBackgroundBitmap(UINT nBand, HBITMAP& hBitmap)
{
	ASSERT(::IsWindow(m_hWnd));

	UINT nMask;
	if(GetBandMask(nBand,nMask) && nMask&RBBIM_BACKGROUND)
	{
		COXCoolBarBandInfo bi;
		if(bi.SetMask(RBBIM_BACKGROUND) && GetBandInfo(nBand,&bi))
		{
			if(bi.GetBackgroundBitmap(hBitmap))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

// It is your responsibility to create and destroy background bitmap object. 
// To simplify this task use COXCoolBar::AddBitmap function
// which creates bitmap from resource and returns handle to it.
// When corresponding CoolBar destroyed it will destroy all bitmaps
// created using COXCoolBar::AddBitmap function
BOOL COXCoolBar::SetBandBackgroundBitmap(UINT nBand, HBITMAP hBitmap)
{
	ASSERT(::IsWindow(m_hWnd));

	COXCoolBarBandInfo bi;
	if(GetBandInfo(nBand,&bi))
	{
		if(bi.SetBackgroundBitmap(hBitmap) && SetBandInfo(nBand,&bi))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL COXCoolBar::GetBandID(UINT nBand, UINT& nID)
{
	ASSERT(::IsWindow(m_hWnd));

	COXCoolBarBandInfo bi;
	if(bi.SetMask(RBBIM_ID) && GetBandInfo(nBand,&bi))
	{
		if(bi.GetID(nID))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL COXCoolBar::SetBandID(UINT nBand, UINT nID)
{
	ASSERT(::IsWindow(m_hWnd));

	COXCoolBarBandInfo bi;
	if(GetBandInfo(nBand,&bi))
	{
		if(bi.SetID(nID) && SetBandInfo(nBand,&bi))
		{
			return TRUE;
		}
	}
	return FALSE;
}

//////////////////////////////////////////////////

// save the text associated with the band
BOOL COXCoolBar::CheckInBandText(UINT nBand, CString& sText)
{
	UINT nID;
	if(GetBandID(nBand,nID))
	{
		m_allBandText.SetAt(nID,sText);
		return TRUE;
	}
	return FALSE;
}

// save the mask associated with the band
BOOL COXCoolBar::CheckInBandMask(UINT nBand, UINT nMask)
{
	UINT nID;
	if(GetBandID(nBand,nID))
	{
		m_allBandMask.SetAt(nID,nMask);
		return TRUE;
	}
	return FALSE;
}

// check and save the ID associated with the band
BOOL COXCoolBar::CheckInBandID(UINT nID, BOOL bCreate)
{
	// ID cannot be zero
	if(nID==0)
	{
		TRACE0("COXCoolBar::CheckInBandID: ID of band cannot be zero");
		return FALSE;
	}

	if(bCreate)
	{
		// if we call it from InsertBand function then
		// make sure that the ID is unique
		if(m_allBandID.Lookup(nID,nID))
		{
			TRACE1("COXCoolBar::CheckInBandID: band with ID=%d already exists",nID);
			return FALSE;
		}
		m_allBandID.SetAt(nID,nID);
	}
	else
	{
		// if we call it from SetBandInfo function then
		// make sure that the ID already exists
		if(!m_allBandID.Lookup(nID,nID))
		{
			TRACE1("COXCoolBar::CheckInBandID: band with ID=%d does not exist",nID);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL COXCoolBar::ChangeID(UINT nOldID, UINT nNewID)
{
	if(nOldID==nNewID)
	{
		return TRUE;
	}

	if(!CheckInBandID(nOldID) || !CheckInBandID(nNewID,TRUE))
	{
		return FALSE;
	}
	
	// take care about the text and the mask associated with the band
	CString sText;
	if(m_allBandText.Lookup(nOldID,sText))
	{
		m_allBandText.RemoveKey(nOldID);
		m_allBandText.SetAt(nNewID,sText);
	}

	UINT nMask;
	if(m_allBandMask.Lookup(nOldID,nMask))
	{
		m_allBandMask.RemoveKey(nOldID);
		m_allBandMask.SetAt(nNewID,nMask);
	}

	m_allBandID.RemoveKey(nOldID);

	return TRUE;
}


// All functions SetBandXXX/GetBandXXX operate with the number of band in CoolBar.
// This function lets you get that number if you know the and's ID
BOOL COXCoolBar::GetBandNumber(UINT nID, UINT& nBand)
{
	int nBandCount=GetBandCount();
	UINT nTempID;
	for(int nIndex=0; nIndex<nBandCount; nIndex++)
	{
		if(GetBandID(nIndex,nTempID))
		{
			if(nTempID==nID)
			{
				nBand=nIndex;
				return TRUE;
			}
		}
	}
	return FALSE;
}

//////////////////////////////////////////////////

// helper function to keep array of all used bitmaps
HBITMAP COXCoolBar::AddBitmap(LPTSTR lpszResourceName)
{
	HBITMAP hBitmap;
	if(!m_allBitmaps.Lookup(lpszResourceName,hBitmap))
	{
		if((hBitmap=::LoadBitmap(AfxGetResourceHandle(),
			lpszResourceName))==NULL)
		{
			TRACE0("COXCoolBar::AddBitmap: unable to load bitmap\n");
			return NULL;
		}
		m_allBitmaps.SetAt(lpszResourceName,hBitmap);
	}

	return hBitmap;
}

// helper function to keep array of all used image lists
HIMAGELIST COXCoolBar::AddImageList(LPTSTR lpszResourceName, int cx, int cGrow, 
									COLORREF crMask, UINT uType, UINT uFlags)
{
	HANDLE hImageList;
	if(!m_allImageLists.Lookup(lpszResourceName,hImageList))
	{
		if((hImageList=(HANDLE)::ImageList_LoadImage(AfxGetResourceHandle(),
			lpszResourceName,cx,cGrow,crMask,uType,uFlags))==NULL)
		{
			TRACE0("COXCoolBar::AddImageList: unable to load image list\n");
			return NULL;
		}
		m_allImageLists.SetAt(lpszResourceName,hImageList);
	}

	return (HIMAGELIST)hImageList;
}

BOOL COXCoolBar::SaveState(LPCTSTR lpszProfileName)
{
	CWinApp* pApp=AfxGetApp();

	UINT nBandCount=GetBandCount();
	if(nBandCount==0)
	{
		TRACE0("COXCoolBar::SaveState: there is nothing to save");
		return FALSE;
	}

	// save the number of bands in CoolBar
	pApp->WriteProfileInt(lpszProfileName,szBandCount,nBandCount);

	TCHAR szSection[256];
	BOOL bResult=TRUE;
	COXCoolBarBandInfo bi;
	for(int nIndex=0; nIndex<(int)nBandCount; nIndex++)
	{
		UINT nMask;
		if(GetBandMask(nIndex,nMask) && bi.SetMask(nMask) && GetBandInfo(nIndex,&bi))
		{
			// if initial width wasn't set we will do it now
			// to save width of the band
			if((nMask&RBBIM_SIZE)==0)
			{
				CheckInBandMask(nIndex,nMask|RBBIM_SIZE);
				UINT nWidth;
				GetBandWidth(nIndex,nWidth);
				if(!bi.SetWidth(nWidth) || !bi.SetMask(nMask|RBBIM_SIZE))
				{
					bResult=FALSE;
				}
			}
			wsprintf(szSection,szBandSection,lpszProfileName,nIndex);

			// call COXCoolBarBandInfo::SaveState to do rest of the job
			if(!bi.SaveState(szSection,nIndex))
			{
				TRACE1("COXCoolBar::SaveState: unable to save state of band nIndex=%d",nIndex);
				bResult=FALSE;
			}
		}
		else
		{
			TRACE1("COXCoolBar::SaveState: unable to get state of band nIndex=%d",nIndex);
			bResult=FALSE;
		}
	}
	return bResult;
}

BOOL COXCoolBar::LoadState(LPCTSTR lpszProfileName)
{
	CWinApp* pApp=AfxGetApp();
	
	UINT nBandCount=pApp->GetProfileInt(lpszProfileName,szBandCount,0);
	if(nBandCount==0)
	{
		return FALSE;
	}

	TCHAR szSection[256];
	BOOL bResult=TRUE;
	int nIndex, nIndexSaved;

	COXCoolBarBandInfo* pBI;
	// map array to save info about currently inserted bands
	CMapWordToPtr mapBandInfo;

	for(nIndex=0; nIndex<(int)nBandCount; nIndex++)
	{
		pBI=new COXCoolBarBandInfo;
		UINT nMask;
		if(GetBandMask(nIndex,nMask) && pBI->SetMask(nMask) && 
			GetBandInfo(nIndex,pBI))
		{
			UINT nID;
			if(pBI->GetID(nID))
			{
				// find an index of this band in registry
				nIndexSaved=FindSavedBandInfo(nID,lpszProfileName,nBandCount);
				if(nIndexSaved!=-1)
				{
					// if saved info found
					wsprintf(szSection,szBandSection,lpszProfileName,nIndexSaved);
					// load band's properties
					if(!pBI->LoadState(szSection))
					{
						TRACE1("COXCoolBar::LoadState: unable to load state of band ID=%d",nID);
						bResult=FALSE;
					}
					else
					{
						// save band to map array
						mapBandInfo.SetAt((unsigned short)nIndexSaved,(void*)pBI);
					}
				}
				else
				{
					TRACE1("COXCoolBar::LoadState: unable to find saved state of band ID=%d",nID);
					bResult=FALSE;
				}
			}
			else
			{
				bResult=FALSE;
			}
		}
		else
		{
			TRACE1("COXCoolBar::LoadState: unable to get state of band nIndex=%d",nIndex);
			bResult=FALSE;
		}
		if(!bResult)
		{
			delete pBI;
			break;
		}
	}

	// Actually, to put all bands in the same order we have to repopulate
	// CoolBar in back order taking into account order information 
	// saved in registry
	
	// first of all, delete all bands
	// note that all child windows associated with the CoolBar are not destroyed
	for(nIndex=0; nIndex<(int)nBandCount; nIndex++)
	{
		if(!DeleteBand(0))
		{
			bResult=FALSE;
		}
	}

	// insert bands in back order
	for(nIndex=nBandCount-1; nIndex>=0; nIndex--)
	{
		if(mapBandInfo.Lookup((unsigned short)nIndex,(void*&)pBI))
		{
			if(bResult)
			{
				if(!InsertBand(0,pBI))
				{
					TRACE0("Failed to insert band\n");
					bResult=FALSE;
				} 
			}
			delete pBI;
		}
		else
		{
			bResult=FALSE;
		}
	}
	mapBandInfo.RemoveAll();

	GetParentFrame()->RecalcLayout();

	return bResult;
}

// find in registry info about the band with nBandID ID
// return the zero based index or -1 if not found
int COXCoolBar::FindSavedBandInfo(UINT nBandID, LPCTSTR lpszProfileName, 
								  UINT nBandCount)
{
	if(nBandID==0)
	{
		return -1;
	}

	CWinApp* pApp=AfxGetApp();

	UINT nID;
	int nResult=-1;
	TCHAR szSection[256];
	for(int nIndex=0; nIndex<(int)nBandCount; nIndex++)
	{
		wsprintf(szSection,szBandSection,lpszProfileName,nIndex);
		nID=pApp->GetProfileInt(szSection,szID,0);
		if(nBandID==nID)
		{
			nResult=pApp->GetProfileInt(szSection,szIndex,-1);
			break;
		}
	}

	return nResult;
}


#endif //_MFC_VER>=0x0420