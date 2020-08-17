// ==========================================================================
// 						Class Implementation : COXCopyStatusDialog
// ==========================================================================

// Source file :cpystdlg.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
                          
// //////////////////////////////////////////////////////////////////////////

#include "stdafx.h"		// standard MFC include
#include "cpystdlg.h"	// class specification
#include "UTB64Bit.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXCopyStatusDialog, COXModelessDialog)

#define new DEBUG_NEW

#ifndef WIN32
	typedef const void FAR* LPDLGTEMPLATE;
#endif 

#define IDC_COPYFROM	200
#define IDC_COPYTO		300
#define IDC_FROM		400
#define IDC_TO			500

/////////////////////////////////////////////////////////////////////////////
// Definition of static members
const COXCopyStatusDialog::ECopyStatus COXCopyStatusDialog::CS_FIRST(COXCopyStatusDialog::CSCopying);
const COXCopyStatusDialog::ECopyStatus COXCopyStatusDialog::CS_LAST(COXCopyStatusDialog::CSRemoving);

// Data members -------------------------------------------------------------
// protected:
	// BOOL m_bCancelled;
	// --- Whether the user want to cancel the copy process that is in progress

	// CWnd* m_pParentWnd;
	// --- Pointer to the parent window

	// CStatic* m_pFromLabel;
	// --- Static control showing "FROM"
	// CStatic* m_pToLabel;
	// --- Static control showing "TO"

	// CStatic* m_pFromMsg;
	// --- Static control showing the source file name
	// CStatic* m_pToMsg;
	// --- Static control showing the destination file name

// private:
	
// Member functions ---------------------------------------------------------
// public:

COXCopyStatusDialog::COXCopyStatusDialog(COXCopyStatusDialog** ppSelf /* = NULL */, CWnd* pParentWnd /* = NULL */)
    : 
COXModelessDialog((COXModelessDialog**)ppSelf, ((ppSelf == NULL) ? FALSE : TRUE)),
	m_pParentWnd(pParentWnd),
	m_bCancelled(FALSE),
	m_pFromLabel(NULL),
	m_pToLabel(NULL),  
	m_pFromMsg(NULL),  
	m_pToMsg(NULL)    
	{
	}


BOOL COXCopyStatusDialog::Create()
	{
	WORD  *p, *pdlgtemplate;
	DWORD lStyle;

	// allocate some memory to play with 
	pdlgtemplate = p = (PWORD) LocalAlloc (LPTR, 1000);
 
	// start to fill in the dlgtemplate information.  addressing by WORDs
	lStyle = DS_MODALFRAME | WS_CAPTION | WS_VISIBLE;

#ifdef WIN32
	int   nchar;
	
	*p++ = LOWORD (lStyle);
	*p++ = HIWORD (lStyle);
	*p++ = 0;          // LOWORD (lExtendedStyle)
	*p++ = 0;          // HIWORD (lExtendedStyle)
	*p++ = 1;          // NumberOfItems
	*p++ = 10;         // x
	*p++ = 10;         // y
	*p++ = 260;        // cx
	*p++ = 60;   	   // cy
	*p++ = 0;          // Menu
	*p++ = 0;          // Class

	// copy the title of the dialog 
	nchar = nCopyAnsiToWideChar (p, _T("Copying..."));
	p += nchar;

	// add in the wPointSize and szFontName here iff the DS_SETFONT bit on
	// make sure the first item starts on a DWORD boundary
	ULONG ul;
	ul = (ULONG)(ULONG_PTR)p;
	ul +=3;
	ul >>=2;
	ul <<=2;
	p = (LPWORD)(ULONG_PTR) ul;

	// now start with the first item
	lStyle = BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_TABSTOP;
	*p++ = LOWORD (lStyle);
	*p++ = HIWORD (lStyle);
	*p++ = 0;          // LOWORD (lExtendedStyle)
	*p++ = 0;          // HIWORD (lExtendedStyle)
	*p++ = 210;        // x
	*p++ = 45;         // y
	*p++ = 40;         // cx
	*p++ = 12;         // cy
	*p++ = IDCANCEL;   // ID


	// fill in class i.d., this time by name
	nchar = nCopyAnsiToWideChar (p, _T("BUTTON"));
	p += nchar;


	// copy the text of the first item
	nchar = nCopyAnsiToWideChar (p, _T("Cancel"));
	p += nchar;

	*p++ = 0;  // advance pointer over nExtraStuff WORD

#else
    BYTE* pByte;

	*p++ = LOWORD (lStyle);
	*p++ = HIWORD (lStyle);
	
	pByte = (BYTE*)p;
	*pByte++ = 1;  	   // NumberOfItems
	p = (WORD*)pByte; 
	
	*p++ = 10;         // x
	*p++ = 10;         // y
	*p++ = 260;        // cx
	*p++ = 60;   	   // cy

	pByte = (BYTE*)p;
	*pByte++ = 0;  	   // Menu
	*pByte++ = 0;  	   // Class
	p = (WORD*)pByte; 
	
	// copy the title of the dialog
	pByte = (BYTE*)p; 
	strcpy((char*)pByte, "Copying...");
	pByte += strlen((char*)pByte) + 1; // +1 for null termimator
	p = (WORD*)pByte;

	// add in the wPointSize and szFontName here iff the DS_SETFONT bit on
	// make sure the first item starts on a WORD boundary
	DWORD w = (DWORD)p;
	w += 1;
	w >>=1;
	w <<=1;
	p = (WORD*) w;

	// now start with the first item
	*p++ = 210;        // x
	*p++ = 45;         // y
	*p++ = 40;         // cx
	*p++ = 12;         // cy
	*p++ = IDCANCEL;   // ID
	lStyle = BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | WS_TABSTOP;
	*p++ = LOWORD (lStyle);
	*p++ = HIWORD (lStyle);


	// fill in class i.d., this time by class type
	pByte = (BYTE*)p;
	*pByte++ = 0x80;  // a button has 0x80 as class type
	p = (WORD*)pByte; 

	// copy the text of the first item
	pByte = (BYTE*)p; 
	strcpy((char*)pByte, "Cancel");
	pByte += strlen((char*)pByte) + 1; // +1 for null termimator
	p = (WORD*)pByte;

	*p++ = 0;  // advance pointer over nExtraStuff WORD

#endif

	BOOL bSuccess = CreateIndirect((LPDLGTEMPLATE)pdlgtemplate, m_pParentWnd);

 	LocalFree (LocalHandle ((void NEAR *)pdlgtemplate));

	if (bSuccess)
		bSuccess = CreateControls();

	return bSuccess;
	}

BOOL COXCopyStatusDialog::IsCancelled()
	{
	return m_bCancelled;
	}

void COXCopyStatusDialog::OnCancel()
	{
	// Don't call our base class because then the dialog would be destroyed by
	// the DestroyWindow function.  The user has to delete his own dialog.
	m_bCancelled = TRUE;
	}

void COXCopyStatusDialog::SetStatusText(ECopyStatus eCopyStatus, LPCTSTR pszFromText /* = "" */,
	LPCTSTR pszToText /* = "" */)
	{
	switch(eCopyStatus)
		{
		case COXCopyStatusDialog::CSCopying:
			if (m_pFromLabel != NULL)
				m_pFromLabel->SetWindowText(_T("Copying:"));
			if (m_pToLabel != NULL)
				m_pToLabel->SetWindowText(_T("To:"));

			if (m_pFromMsg != NULL)
				m_pFromMsg->SetWindowText(pszFromText);
			if (m_pToMsg != NULL)
				m_pToMsg->SetWindowText(pszToText);
			break;
		case COXCopyStatusDialog::CSRemoving:
			if (m_pFromLabel != NULL)
				m_pFromLabel->SetWindowText(_T("Removing:"));
			if (m_pToLabel != NULL)
				m_pToLabel->SetWindowText(_T(""));

			if (m_pFromMsg != NULL)
				m_pFromMsg->SetWindowText(pszFromText);
			if (m_pToMsg != NULL)
				m_pToMsg->SetWindowText(pszToText);
			break;
		default:
			TRACE(_T("COXCopyStatusDialog::SetStatusText : Unkown Copy Status %i"), eCopyStatus);
			ASSERT(FALSE);			
			break;
		}
	}

int COXCopyStatusDialog::nCopyAnsiToWideChar (LPWORD lpWCStr, LPTSTR lpAnsiIn)
	{
	int nChar = 0; 

	do
		{
	    *lpWCStr++ = (WORD) *lpAnsiIn;
	    nChar++;
	  	} while (*lpAnsiIn++);

	return nChar;
	}

#ifdef _DEBUG
void COXCopyStatusDialog::Dump(CDumpContext& dc) const
	{
	COXModelessDialog::Dump(dc);

	dc << _T("\nm_pFromLabel: ") << (const void*)m_pFromLabel;
	dc << _T("\nm_pToLabel: ") << (const void*)m_pToLabel;
	dc << _T("\nm_pFromMsg: ") << (const void*)m_pFromMsg;
	dc << _T("\nm_pToMsg: ") << (const void*)m_pToMsg;
	}
	
void COXCopyStatusDialog::AssertValid() const
	{
	COXModelessDialog::AssertValid();
	}
#endif

COXCopyStatusDialog::~COXCopyStatusDialog()
{
	delete m_pFromLabel;
	delete m_pToLabel;
	delete m_pFromMsg;
	delete m_pToMsg;
}

BOOL COXCopyStatusDialog::CreateControls()
	// --- In  : 
	// --- Out : 
	// --- Returns : succeeded or not
	// --- Effect : Creates our four default Static controls
	{
	CRect 	ClientRect;
	CPoint	TopLeft;

	GetClientRect(ClientRect);
	int nSeparatorWidth = ClientRect.Width() / 30;
	int nSeparatorHeight = ClientRect.Height() / 8;

	CDC* pDC = GetWindowDC();
	CSize TextSize = pDC->GetTextExtent(_T("MMMMMMMM"), 8);
	ReleaseDC(pDC);

	TopLeft.x = nSeparatorWidth;
	TopLeft.y = nSeparatorHeight;
	if (!CreateStatic(IDC_COPYFROM, _T("Copying:"), CRect(TopLeft, TextSize)))
		return FALSE;

	TopLeft.y = TextSize.cy + 2 * nSeparatorHeight;
	if (!CreateStatic(IDC_COPYTO, _T("To:"), CRect(TopLeft,	TextSize)))
		return FALSE;

	TopLeft.x = nSeparatorWidth + TextSize.cx;
	TopLeft.y = nSeparatorHeight;
	TextSize.cx = ClientRect.Width() - (2 * nSeparatorWidth) - TextSize.cx;
	if (!CreateStatic(IDC_FROM, _T(""), CRect(TopLeft, TextSize)))
		return FALSE;

	TopLeft.y = TextSize.cy + 2 * nSeparatorHeight;
	if (!CreateStatic(IDC_TO, _T(""), CRect(TopLeft, TextSize)))
		return FALSE;

	return TRUE;
	}

BOOL COXCopyStatusDialog::CreateStatic(UINT nIDC, LPCTSTR pszText /* = "" */, const CRect& Position /* CRect(0,0,0,0) */)
	// --- In  : nIDC : one of the four prdefined IDC's
	//			 pszText : the text you want to set to the control with nIDC
	//			 Position : the position you want to set to the control with nIDC
	// --- Out : 
	// --- Returns : Succeeded or not
	// --- Effect : You can use four predifined IDC's, to create 4 Static controls
	//				with. They'll have pszText as windowtext and position as control
	//				position (in client coordinates)
	{
	BOOL bSuccess(TRUE);

	switch(nIDC)
		{
		case IDC_COPYFROM:
			if (m_pFromLabel == NULL)
				{
				m_pFromLabel = new CStatic;				
				bSuccess = m_pFromLabel->Create(pszText, SS_LEFT | WS_VISIBLE | WS_CHILD,
					Position, this, nIDC);
				}
			break;
		case IDC_COPYTO:
			if (m_pToLabel == NULL)
				{
				m_pToLabel = new CStatic;				
				bSuccess = m_pToLabel->Create(pszText, SS_LEFT | WS_VISIBLE | WS_CHILD,
					Position, this, nIDC);
				}
			break;
		case IDC_FROM:
			if (m_pFromMsg == NULL)
				{
				m_pFromMsg = new CStatic;				
				bSuccess = m_pFromMsg->Create(pszText, SS_LEFT | WS_VISIBLE | WS_CHILD,
					Position, this, nIDC);
				}
			break;
		case IDC_TO:	
			if (m_pToMsg == NULL)
				{
				m_pToMsg = new CStatic;				
				bSuccess = m_pToMsg->Create(pszText, SS_LEFT | WS_VISIBLE | WS_CHILD,
					Position, this, nIDC);
				}
			break;
		default:
			break;
		}

	return bSuccess;
	}

void COXCopyStatusDialog::DoDataExchange(CDataExchange* pDX)
	{
       COXModelessDialog::DoDataExchange(pDX);

       //{{AFX_DATA_MAP(COXCopyStatusDialog)
       // (No data)
       //}}AFX_DATA_MAP
	}

BEGIN_MESSAGE_MAP(COXCopyStatusDialog, COXModelessDialog)
       //{{AFX_MSG_MAP(COXCopyStatusDialog)
       //}}AFX_MSG_MAP
END_MESSAGE_MAP()

	
// protected:

// private:

// ==========================================================================
