//==========================================================================
// 							Class Implementation : 
//					COXEdit & COXNumericEdit & COXCurrencyEdit
//==========================================================================

// Implementation file : OXEdit.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

////////////////
// COXEdit implements a edit control which could be set to use
// user-defined color and font to draw text. Also you can show tooltip
//

#include "StdAfx.h"
#include "OXEdit.h"

#include <math.h> 
#include <stdlib.h>

#pragma warning (push, 3)
#include <iomanip>
#include <sstream>
#pragma warning (pop)

#include "UTBStrOp.h"
#include "UTB64Bit.h"


using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif


#if _MFC_VER<=0x0421

static void	RemoveCharFromString(CString& sText, TCHAR chToRemove)
{
	int nPos=sText.Find(chToRemove);
	while(nPos!=-1)
	{
		sText=sText.Left(nPos)+sText.Mid(nPos+1);
		nPos=sText.Find(chToRemove);
	}
}


static void	ReplaceCharInString(CString& sText, TCHAR chToBeReplaced, TCHAR chToReplaceTo)
{
	CString sCopy=sText;
	CString sToReplaceTo(chToReplaceTo);
	int nPos=sCopy.Find(chToBeReplaced);
	sText.Empty();
	while(nPos!=-1)
	{
		sText=sText+sCopy.Left(nPos)+sToReplaceTo;
		sCopy=sCopy.Mid(nPos+1);
		nPos=sCopy.Find(chToBeReplaced);
	}
	sText+=sCopy;
}

#endif


//////////////////////////////////////////////////	
//////////////////////////////////////////////////	
	
IMPLEMENT_DYNCREATE(COXEdit, CEdit)

BEGIN_MESSAGE_MAP(COXEdit, CEdit)
	//{{AFX_MSG_MAP(COXEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT,OnCut)
	ON_MESSAGE(WM_COPY,OnCopy)
	ON_MESSAGE(WM_PASTE,OnPaste)
	ON_MESSAGE(WM_CLEAR,OnClear)
	ON_MESSAGE(WM_SETTEXT,OnSetText)
END_MESSAGE_MAP()

///////////////////
// Constructor
//
// --- In  :
// --- Out : 
// --- Returns :
// --- Effect : Constructs the object
COXEdit::COXEdit()
{
	m_clrText=::GetSysColor(COLOR_WINDOWTEXT); 
	m_clrBack=::GetSysColor(COLOR_WINDOW); 
	m_clrReadOnlyBack=::GetSysColor(COLOR_BTNFACE);
	m_clrReadOnlyFont=::GetSysColor(COLOR_GRAYTEXT);

	m_bInsertMode=TRUE;

	TCHAR chMask[2];
	chMask[0]=OXEDITABLETEXT_SYMBOL;
	chMask[1]=_T('\0');
	VERIFY(SetMask(chMask));

	m_bInitialized=FALSE;

	m_nSetTextSemaphor=0;
	m_bNotifyParent=TRUE;

	m_bHighlightOnSetFocus=FALSE;

	m_bNoSaveFont=FALSE;
}

void COXEdit::SetTextColor(COLORREF clrText, BOOL bRedraw/*=TRUE*/)
{ 
	if(m_clrText!=clrText)
	{
		m_clrText=clrText; 
		if(bRedraw)
			RedrawWindow();
	}
}

void COXEdit::SetBkColor(COLORREF clrBack, BOOL bRedraw/*=TRUE*/)
{ 
	if(m_clrBack!=clrBack)
	{
		m_clrBack=clrBack; 
		if(bRedraw)
			RedrawWindow();
	}
}

BOOL COXEdit::SetToolTipText(LPCTSTR sText)
{ 
	if(::IsWindow(m_ttc.GetSafeHwnd()))
	{
		m_sToolTipText=sText; 
		m_ttc.UpdateTipText(m_sToolTipText, this, ID_OXEDIT_TOOLTIP);

		return TRUE;
	}

	return FALSE;
}

//////////////////
// Handle reflected WM_CTLCOLOR to set custom control color and font.
HBRUSH COXEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	UNREFERENCED_PARAMETER(nCtlColor);

	COLORREF clrText=GetTextColor();
	COLORREF clrBk=GetBkColor();
	if(!IsWindowEnabled())
	{
		clrText=::GetSysColor(COLOR_GRAYTEXT);
		clrBk=::GetSysColor(COLOR_BTNFACE);
	}
	else if(GetStyle()&ES_READONLY)
	{
		clrText=m_clrReadOnlyFont;
		clrBk=m_clrReadOnlyBack;
	}
	pDC->SetTextColor(clrText);
	pDC->SetBkColor(clrBk);

	if((HFONT)m_font!=NULL)
	{
		pDC->SelectObject(&m_font);
	}

	static CBrush brush;
	if((HBRUSH)brush!=NULL)
		brush.DeleteObject();
	brush.CreateSolidBrush(clrBk);

	return (HBRUSH)brush;
}

// handle it to pass a mouse message to a tool tip control for processing 
BOOL COXEdit::PreTranslateMessage(MSG* pMsg)
{
	if(::IsWindow(m_ttc.GetSafeHwnd()))
	{
		m_ttc.Activate(TRUE); 
		m_ttc.RelayEvent(pMsg);
	}
	return CEdit::PreTranslateMessage(pMsg);
}

// handle it to initialize tooltip control and save the original size 
// of edit control window
void COXEdit::PreSubclassWindow()
{
	_AFX_THREAD_STATE* pThreadState=AfxGetThreadState();
	// hook not already in progress
	if(pThreadState->m_pWndInit==NULL && !m_bInitialized)
	{
		if(!InitControl())
			TRACE(_T("COXEdit::PreSubclassWindow: failed to initialize edit control\n"));
	}

	CEdit::PreSubclassWindow();
}

// handle it to update original size of window and if m_bFitToText is TRUE then
// resize it to fit to its text
void COXEdit::OnSize(UINT nType, int cx, int cy)
{
	CEdit::OnSize(nType, cx, cy);
	
	// update the original size of the edit control window
	CRect rect;
	GetWindowRect(rect);
	CWnd* pParent=GetParent();
	if(pParent!=NULL)
	{
		pParent->ScreenToClient(rect);
	}

	// Change information about window's rect in tooltip control
	if(::IsWindow(m_ttc.GetSafeHwnd()))
	{
		CToolInfo toolInfo;
		if(m_ttc.GetToolInfo(toolInfo,this,ID_OXEDIT_TOOLTIP))
		{
			toolInfo.rect.left=0;
			toolInfo.rect.top=0;
			toolInfo.rect.right=rect.Width();
			toolInfo.rect.bottom=rect.Height();
			UTBStr::tcscpy(toolInfo.szText, _tcsclen(m_sToolTipText) + 1, m_sToolTipText);
			m_ttc.SetToolInfo(&toolInfo);
		}
	}
}

BOOL COXEdit::InitControl()
{
	ASSERT(::IsWindow(GetSafeHwnd()));

	GetLocaleSettings();

	// Create the tooltip
	CRect rect; 
	GetClientRect(rect);
	m_ttc.Create(this);
	m_ttc.AddTool(this, m_sToolTipText, rect, ID_OXEDIT_TOOLTIP);

	// save font
	VERIFY(SaveFont());

	CString sText;
	GetWindowText(sText);
	EmptyData();
	SetInputData(sText);

	m_bInitialized=TRUE;

	return TRUE;
}

BOOL COXEdit::Create(DWORD dwStyle, const RECT& rect, 
					 CWnd* pParentWnd, UINT nID)
{
	BOOL bResult=CEdit::Create(dwStyle, rect, pParentWnd, nID);
	if(bResult)
	{
		// Initialize edit control
		if(!InitControl())
		{
			TRACE(_T("COXEdit::Create: failed to initialize edit control"));
			return FALSE;
		}
	}

	return bResult;
}

void COXEdit::OnDestroy()
{
	if(::IsWindow(m_ttc.GetSafeHwnd()))
	{
		m_ttc.DelTool(this, ID_OXEDIT_TOOLTIP);
		m_ttc.DestroyWindow();
	}

	CEdit::OnDestroy();
}


BOOL COXEdit::SaveFont()
{
	LOGFONT lf;
	CFont* pFont=GetFont();
	if(pFont!=NULL)
	{
		if(!pFont->GetLogFont(&lf))
			return FALSE;
	}
	
	if((HFONT)m_font!=NULL)
		m_font.DeleteObject();
	if(pFont!=NULL)
	{
		if(!m_font.CreateFontIndirect(&lf))
			return FALSE;
	}

	return TRUE;
}


LRESULT COXEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
#if defined (_WINDLL)
#if defined (_AFXDLL)
	AFX_MANAGE_STATE(AfxGetAppModuleState());
#else
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#endif

	LRESULT lResult=CEdit::WindowProc(message, wParam, lParam);

	switch(message)
	{
	case WM_SETFONT:
		{
			if(!m_bNoSaveFont)
			{
				VERIFY(SaveFont());
			}
			break;
		}

	case WM_SETTINGCHANGE:
		{
			CString sInputData=GetInputData();
			GetLocaleSettings();
			EmptyData(TRUE);
			VERIFY(SetInputData(sInputData));
			break;
		}
	}

	return lResult;
}


void COXEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// Keep the OnKeyDown processing to a minimum.  This is because the edit 
	// control does lots of processing before OnChar() is sent and we want 
	// to let it continue. 

	if((GetStyle()&ES_READONLY)==ES_READONLY)
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}

	BOOL bIsShiftKeyDown=::GetAsyncKeyState(VK_SHIFT)<0;
	BOOL bIsCtrlKeyDown=::GetAsyncKeyState(VK_CONTROL)<0;
	if(nChar==VK_DELETE)
	{
		if (bIsShiftKeyDown)//cut operation
		{
			CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
			return;
		}
		int nSelectionStart=0;
		int nSelectionEnd=0;
		GetSel(nSelectionStart, nSelectionEnd);
		// Delete has two functions, it can delete the selection and
		// it can delete characters to the right.
		if(nSelectionStart==nSelectionEnd)
		{
			nSelectionEnd++; // Do the equivalent of a selection.
			if(DeleteRange(nSelectionStart, nSelectionEnd))
			{
				Update(nSelectionStart);
			}
			else	// Must be on a literal, so continue moving to right
					// and re-attempting the delete until we either delete
					// a character or run out of characters.
			{
				while(nSelectionEnd<=
					m_arrLeftLiterals.GetSize()+m_arrInputData.GetSize())
				{
					nSelectionStart++;
					nSelectionEnd++; // Do the equivalent of a selection.
					if(DeleteRange(nSelectionStart, nSelectionEnd))
					{
						Update(nSelectionStart);
						break;
					}
				}
			}
		}
		else if(DeleteRange(nSelectionStart, nSelectionEnd))
		{
			Update(nSelectionStart);
		}
		else	// Must be on a literal, so continue moving to right
				// and reattempting the delete until we either delete
				// a character or run out of characters.
		{
			while(nSelectionEnd<=
					m_arrLeftLiterals.GetSize()+m_arrInputData.GetSize())
			{
				nSelectionStart++;
				nSelectionEnd++; // Do the equivalent of a selection.
				if(DeleteRange(nSelectionStart, nSelectionEnd))
				{
					Update(nSelectionStart);
					break;
				}
			}
		}
	}
	else if(nChar==VK_HOME)
	{
		// If the shift key is not down, then HOME is a navigation and we need to 
		// move the insertion point to the first available position. 
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if(!bIsShiftKeyDown)
		{
			int nSelectionStart=0;
			int nSelectionEnd=0;
			GetSel(nSelectionStart, nSelectionEnd);
			if(!IsInputPosition(nSelectionStart))
				UpdateInsertionPointForward(nSelectionStart);
		}
	}
	else if(nChar==VK_END)
	{
		// If the shift key is not down, then HOME is a navigation and we need to 
		// move the insertion point to the first available position. 
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if(!bIsShiftKeyDown)
		{
			int nSelectionStart=0;
			int nSelectionEnd=0;
			GetSel(nSelectionStart, nSelectionEnd);
			if(!IsInputPosition(nSelectionStart))
				UpdateInsertionPointBackward(nSelectionStart);
		}
	}
	else if(nChar==VK_LEFT)
	{
		// If the shift key is not down, then LEFT is a navigation and we need to 
		// move the insertion point to the previous available position. 
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if(!bIsShiftKeyDown)
		{
			int nSelectionStart=0;
			int nSelectionEnd=0;
			GetSel(nSelectionStart, nSelectionEnd);
			UpdateInsertionPointBackward(nSelectionStart);
		}
	}
	else if(nChar==VK_UP)
	{
		// If the shift key is not down, then UP is a navigation and we need to 
		// move the insertion point to the previous available position. 
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if(!bIsShiftKeyDown)
		{
			int nSelectionStart=0;
			int nSelectionEnd=0;
			GetSel(nSelectionStart, nSelectionEnd);
			UpdateInsertionPointBackward(nSelectionStart);
		}
	}
	else if(nChar==VK_RIGHT)
	{
		// If the shift key is not down, then RIGHT is a navigation and we need to 
		// move the insertion point to the next available position. 
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if(!bIsShiftKeyDown)
		{
			int nSelectionStart=0;
			int nSelectionEnd=0;
			GetSel(nSelectionStart, nSelectionEnd);
			UpdateInsertionPointForward(nSelectionStart);
		}
	}
	else if(nChar==VK_DOWN)
	{
		// If the shift key is not down, then DOWN is a navigation and we need to 
		// move the insertion point to the next available position. 
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		if(!bIsShiftKeyDown)
		{
			int nSelectionStart=0;
			int nSelectionEnd=0;
			GetSel(nSelectionStart, nSelectionEnd);
			UpdateInsertionPointForward(nSelectionStart);
		}
	}
	else if(nChar==VK_INSERT)
	{

		if (!bIsShiftKeyDown && !bIsCtrlKeyDown)
		{
			// The standard CEdit control does not support over-typing. 
			// This flag is used to manage over-typing internally. 
			SetInsertMode(!GetInsertMode());
		}
		else
			CEdit::OnKeyDown(nChar, nRepCnt, nFlags);//copy or paste
	}
	else
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}


void COXEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if((GetStyle()&ES_READONLY)==ES_READONLY)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	int nSelectionStart=0;
	int nSelectionEnd=0;
	GetSel(nSelectionStart, nSelectionEnd);

	BOOL bAcceptReturnKey=(GetStyle()&ES_MULTILINE) && (GetStyle()&ES_WANTRETURN);
	if(nChar==10)
	{
		nChar=VK_RETURN;
		bAcceptReturnKey=(GetStyle()&ES_MULTILINE);
	}

	// If character value is above 32, then it is ANSI or Extended. 
	// Below 32 are control and navigation characters. 
	if(nChar>=32 || (nChar==VK_RETURN && bAcceptReturnKey))
	{
		if(nSelectionStart==nSelectionEnd)
		{
			if(GetStyle()&ES_NUMBER && nChar!=VK_RETURN && 
				(nChar<_T('0') || nChar>_T('9')))
			{
				CEdit::OnChar(nChar, nRepCnt, nFlags);
				return;
			}

			if(IsInputPosition(nSelectionStart))
			{
				int nActualInsertionPoint=nSelectionStart;
				if(m_bInsertMode)
				{
					if(nChar==VK_RETURN)
					{
						// InsertAt will return -1 if the character cannot be inserted here. 
						nActualInsertionPoint=InsertAt(nSelectionStart,_T('\r'));
						if(nActualInsertionPoint>=0)
						{
							nSelectionStart=nActualInsertionPoint+1;
							nActualInsertionPoint=InsertAt(nSelectionStart,_T('\n'));
						}
					}
					else
					{
						nActualInsertionPoint=InsertAt(nSelectionStart,(TCHAR)nChar);
					}
				}
				else
				{
					if(nChar==VK_RETURN)
					{
						nActualInsertionPoint=SetAt(nSelectionStart,_T('\r'));
						// SetAt will return -1 if the character cannot be set here. 
						if(nActualInsertionPoint>=0)
						{
							nSelectionStart=nActualInsertionPoint+1;
							nActualInsertionPoint=SetAt(nSelectionStart,_T('\n'));
						}
					}
					else
					{
						nActualInsertionPoint=SetAt(nSelectionStart,(TCHAR)nChar);
					}
				}
				
				// InsertAt will return -1 if the character cannot be inserted here. 
				if(nActualInsertionPoint>=0)
				{
					nSelectionStart=nActualInsertionPoint+1;
				}
				else
				{
					ValidationError();
				}
				
				Update(nSelectionStart);
			}
			else
			{
				// Beep if trying to type over a literal. 
				ValidationError();
				UpdateInsertionPointForward(nSelectionStart);
			}
		}
		else
		{
			// First delete the remaining selection. 
			// The function will return a valid count if 
			// some input characters were deleted. We use 
			// this value to determine if it makes sense to insert. 
			if(DeleteRange(nSelectionStart,nSelectionEnd))
			{
				// InsertAt will place the character at the next available position, 
				// then return that positition
				int nActualInsertionPoint=nSelectionStart;
				if(nChar==VK_RETURN)
				{
					// InsertAt will return -1 if the character cannot be inserted here. 
					nActualInsertionPoint=InsertAt(nSelectionStart,_T('\r'));
					if(nActualInsertionPoint>=0)
					{
						nSelectionStart=nActualInsertionPoint+1;
						nActualInsertionPoint=InsertAt(nSelectionStart,_T('\n'));
					}
				}
				else
				{
					nActualInsertionPoint=InsertAt(nSelectionStart,(TCHAR)nChar);
				}
				
				// InsertAt will return -1 if the character cannot be inserted here. 
				if(nActualInsertionPoint>=0)
				{
					nSelectionStart=nActualInsertionPoint+1;
				}
				else
				{
					ValidationError();
				}
				
				Update(nSelectionStart);
			}
			else  // Must be on a literal, so beep and move to a valid location. 
			{
				ValidationError();
				UpdateInsertionPointForward(nSelectionStart);
			}
		}
	}
	else
	{
		if(nChar==VK_BACK)
		{
			// Backspace performs two functions, if there is a selection,
			// then the backspace is the same as deleting the selection.
			// If there is no selection, then the backspace deletes the
			// first non-literal character to the left.
			if(nSelectionStart==nSelectionEnd)
			{
				while( nSelectionStart > 0 && nSelectionStart == nSelectionEnd )
				{
					nSelectionStart--; // Do the equivalent of a backspace.

					if(DeleteRange(nSelectionStart,nSelectionEnd))
					{
						Update(nSelectionStart);
						break;
					}

					nSelectionEnd--;
				}
			}
			else if(DeleteRange(nSelectionStart,nSelectionEnd))
			{
				Update(nSelectionStart);
			}
			else	// Must be on a literal, so continue moving to left
					// and re-attempting the delete until we either delete
					// a character or run out of characters.
			{
				if(nSelectionStart>=1)
				{
					while(nSelectionStart>0)
					{
						nSelectionStart--; // Do the equivalent of a backspace.

						if(DeleteRange(nSelectionStart,nSelectionEnd))
						{
							Update(nSelectionStart);
							break;
						}
						
						nSelectionEnd--;
					}
				}
			}
		}
		else
		{
			// let edit control to do its job 
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}
}

void COXEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	if(!GetHighlightOnSetFocus())
	{
		// The default behavior is to highlight the entire string. 
		// If this is the case, then move the insertion to the first input position. 
		int nSelectionStart=0;
		int nSelectionEnd=0;
		GetSel(nSelectionStart, nSelectionEnd);
		if((nSelectionStart==0) && (nSelectionEnd==GetWindowTextLength()))
		{
			// Only update the insertion point if the entire string is selected. 
			// This will allow the mouse to be used to set the cursor without 
			// our interfering. 
			UpdateInsertionPointForward(0);
		}
	}

	m_bNoSaveFont=TRUE;
	SetFont(&m_font);
	m_bNoSaveFont=FALSE;
}


LONG COXEdit::OnCut(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	int nSelectionStart=0;
	int nSelectionEnd=0;
	GetSel(nSelectionStart,nSelectionEnd);

	// Before updating, let the control do its normal thing. 
	// This will save us the effort of filling the clipboard. 
	CEdit::Default();

	// First do our version of the cut. 
	int nDeleteCount=DeleteRange(nSelectionStart,nSelectionEnd);

	// Now we update with our standard mask. 
	Update(nSelectionStart);
	if(nDeleteCount==0)
	{
		// I don't think we want to beep if no input characters were cut. 
		//ValidationError();
	}

	return 0;
}


LONG COXEdit::OnCopy(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	// Just let copy do its thing and copy the selected text. 
	CEdit::Default();

	return 0;
}


LONG COXEdit::OnPaste(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	int nSelectionStart=0;
	int nSelectionEnd=0;
	GetSel(nSelectionStart, nSelectionEnd);

	CEdit::Default();

	// This is a real dump paste routine that expects SetInputData do 
	// do the filtering.  There is probably no easy solution to this 
	// task because anything can be pasted.  We could try and match 
	// the literals, but maybe we will get to that later. 
	CString sNewString;
	GetWindowText(sNewString);

	if(nSelectionStart!=nSelectionEnd)
	{
		CString sLeftLiterals=GetLeftLiterals();
		CString sRightLiterals=GetRightLiterals();

		if(sNewString.Find(sLeftLiterals)==0)
			sNewString=sNewString.Mid(sLeftLiterals.GetLength());
		int nPosition=sNewString.Find(sRightLiterals);
		if(nPosition!=-1 && 
			nPosition+sRightLiterals.GetLength()==sNewString.GetLength())
			sNewString=sNewString.Left(nPosition);
	}

	EmptyData(TRUE);
	SetInputData(sNewString,0);
	// Setting the insertion point after a paste is tricky because the 
	// expected location is after the last valid pasted character. 
	// Try and determine this location by setting the insertion point 
	// to the first empty location after the specified starting point. 
	int nNewInsertionPoint=GetNextInputLocation(nSelectionStart);
	SetSel(nNewInsertionPoint,nNewInsertionPoint);

	return 0;
}


LONG COXEdit::OnClear(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	TRACE(_T("COXEdit::OnClear\n"));

	int nSelectionStart=0;
	int nSelectionEnd=0;
	GetSel(nSelectionStart,nSelectionEnd);

	// Before updating, let the control do its normal thing. 
	CEdit::Default();

	// First do our version of the cut. 
	int nDeleteCount=DeleteRange(nSelectionStart,nSelectionEnd);
	UNREFERENCED_PARAMETER(nDeleteCount);

	// Now we update with our standard mask. 
	Update(nSelectionStart);

	return 0;
}


void COXEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	CEdit::OnLButtonDown(nFlags,point);

	int nSelectionStart=0;
	int nSelectionEnd=0;
	GetSel(nSelectionStart,nSelectionEnd);
	if(nSelectionStart==nSelectionEnd && !IsInputPosition(nSelectionStart))
		UpdateInsertionPointForward(0);
}


LRESULT COXEdit::OnSetText(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if(m_nSetTextSemaphor>0)
	{
		LRESULT result=CEdit::Default();
		NotifyParent(EN_UPDATE);
		if(m_bNotifyParent)
			NotifyParent(EN_CHANGE);
		return result;
	}
	else
	{
		ASSERT(m_nSetTextSemaphor==0);
		CString sText=(LPCTSTR)(INT_PTR)lParam;
	//	EmptyData(TRUE);
		m_arrInputData.RemoveAll();
		m_bNotifyParent=FALSE;
		SetInputData(sText,0);
		m_bNotifyParent=TRUE;
		return TRUE;
	}
}

BOOL COXEdit::SetMask(LPCTSTR lpszMask)
{
	CString sMask=lpszMask;
	if(sMask.IsEmpty() || sMask.Find(OXEDITABLETEXT_SYMBOL)==-1)
		return FALSE;

	DeleteContents();

	BOOL bLeftLiteral=TRUE;
	for(int nIndex=0; nIndex<sMask.GetLength(); nIndex++)
	{
		BOOL bSaveLiteral=TRUE;
		TCHAR chSymbol=sMask[nIndex];
		TCHAR chSpecialSymbol=OXEDITABLETEXT_SYMBOL;
		if(chSymbol==chSpecialSymbol && (nIndex==sMask.GetLength()-1 || 
			sMask[nIndex+1]!=chSpecialSymbol))
		{
			bSaveLiteral=FALSE;
			bLeftLiteral=FALSE;
		}
		else if(chSymbol==chSpecialSymbol && nIndex<sMask.GetLength()-1 && 
			sMask[nIndex+1]==chSpecialSymbol)
		{
			nIndex++;
		}

		if(bSaveLiteral)
		{
			if(bLeftLiteral)
				m_arrLeftLiterals.Add(chSymbol);
			else
				m_arrRightLiterals.Add(chSymbol);
		}
	}

	if(bLeftLiteral)
	{
		VERIFY(SetMask(GetMask()));
		return FALSE;
	}

	m_sMask=sMask;
//	UpdateMask();

	//ValidateInput();
	Validate();

	Update();

	return TRUE;
}


void COXEdit::DeleteContents()
{
	m_arrLeftLiterals.RemoveAll();
	m_arrRightLiterals.RemoveAll();
	m_arrInputData.RemoveAll();
}


CString COXEdit::GetInputData()const
{
	CString sInputData;
	for(int nIndex=0; nIndex<m_arrInputData.GetSize(); nIndex++)
	{
		sInputData+=m_arrInputData[nIndex];
	}
	return sInputData;
}


BOOL COXEdit::SetInputData(LPCTSTR pszInputData, int nBeginPos/*=0*/)
{
	if(nBeginPos<0 || nBeginPos>m_arrInputData.GetSize())
		return FALSE;

	CString sInputData=pszInputData;
	for(int nIndex=0; nIndex<sInputData.GetLength(); nIndex++)
	{
		if(GetInsertMode())
			m_arrInputData.InsertAt(nBeginPos+nIndex,sInputData[nIndex]);
		else
			m_arrInputData.SetAtGrow(nBeginPos+nIndex,sInputData[nIndex]);
	}
	
//	ValidateInput();
	Validate();

	Update(-1);

	return TRUE;
}


void COXEdit::EmptyData(BOOL bOnlyInput/*=FALSE*/)
{
	if(bOnlyInput)
		m_arrInputData.RemoveAll();
	else
		DeleteContents();
	
	Update();
}

BOOL COXEdit::GetInsertMode() const
{
	// The standard CEdit control does not support over-typing. 
	// This flag is used to manage over-typing internally. 
	return m_bInsertMode;
}

void COXEdit::SetInsertMode(BOOL bInsertMode)
{
	// The standard CEdit control does not support over-typing. 
	// This flag is used to manage over-typing internally. 
	m_bInsertMode=bInsertMode;
}


CString COXEdit::GetFormattedText() const
{
	CString sFormattedText;
	int nIndex=0;
	for(nIndex=0; nIndex<m_arrLeftLiterals.GetSize(); nIndex++)
		sFormattedText+=m_arrLeftLiterals[nIndex];
	for(nIndex=0; nIndex<m_arrInputData.GetSize(); nIndex++)
		sFormattedText+=m_arrInputData[nIndex];
	for(nIndex=0; nIndex<m_arrRightLiterals.GetSize(); nIndex++)
		sFormattedText+=m_arrRightLiterals[nIndex];	
	return sFormattedText;
}


BOOL COXEdit::IsInputData(int nPosition) const
{
	// We frequently need to know if a position refers to 
	// input data or to a literal. 
	return((nPosition>=m_arrLeftLiterals.GetSize()&& 
		nPosition<m_arrLeftLiterals.GetSize()+
		m_arrInputData.GetSize())? TRUE : FALSE);
}


BOOL COXEdit::IsInputPosition(int nPosition) const
{
	// We frequently need to know if a position refers to 
	// input data or to a literal. 
	return((nPosition>=m_arrLeftLiterals.GetSize() && 
		nPosition<=m_arrLeftLiterals.GetSize()+
		m_arrInputData.GetSize())? TRUE : FALSE);
}


int COXEdit::DeleteRange(int& nSelectionStart, int& nSelectionEnd)
{
	int nDeleteCount=0;
	TCHAR chLastDeleted=_T('\0');
	TCHAR chFirstDeleted=_T('\0');
	int nIndex=0;
	for(nIndex=nSelectionEnd-1; nIndex>=nSelectionStart; nIndex--)
	{
		if(IsInputData(nIndex))
		{
			if(chFirstDeleted==_T('\0'))
			{
				chFirstDeleted=m_arrInputData[RPtoLP(nIndex)];
			}
			chLastDeleted=m_arrInputData[RPtoLP(nIndex)];
			m_arrInputData.RemoveAt(RPtoLP(nIndex));
			nDeleteCount++;
		}
	}
	if(chLastDeleted==_T('\r') && RPtoLP(nIndex+1)!=-1 && 
		m_arrInputData[RPtoLP(nIndex+1)]==_T('\n'))
	{
		m_arrInputData.RemoveAt(RPtoLP(nIndex+1));
		nDeleteCount++;
	}
	if(chFirstDeleted==_T('\n') && RPtoLP(nSelectionStart-1)!=-1 && 
		m_arrInputData[RPtoLP(nSelectionStart-1)]==_T('\r'))
	{
		m_arrInputData.RemoveAt(RPtoLP(nSelectionStart-1));
		nDeleteCount++;
		nSelectionStart--;
	}

	if(nDeleteCount)
	{
		Update(-1);
	}

	// return the deleted count so that an error can be generated 
	// if none were deleted. 
	return nDeleteCount;
}

int COXEdit::InsertAt(int nSelectionStart, TCHAR chNewChar)
{
	int nInsertionPoint=nSelectionStart;
	if(!IsInputPosition(nInsertionPoint))
	{
		nInsertionPoint=GetNextInputLocation(nInsertionPoint+1);
		ASSERT(IsInputPosition(nInsertionPoint));
	}

	m_arrInputData.InsertAt(RPtoLP(nInsertionPoint),chNewChar);

	//nInsertionPoint+=ValidateInput();
	nInsertionPoint+=Validate();

	Update(-1);

	return nInsertionPoint;
}


int COXEdit::SetAt(int nSelectionStart, TCHAR chNewChar)
{
	int nInsertionPoint=nSelectionStart;
	if(!IsInputData(nInsertionPoint))
	{
		nInsertionPoint=GetNextInputLocation(nInsertionPoint+1);
		if(!IsInputData(nInsertionPoint))
			return -1;
	}

	m_arrInputData.SetAt(RPtoLP(nInsertionPoint),chNewChar);

	nInsertionPoint+=Validate();

	Update(-1);

	return nInsertionPoint;
}


int COXEdit::GetNextInputLocation(int nSelectionStart)
{
	int nNextInputLocation=nSelectionStart;
	if(!IsInputPosition(nNextInputLocation))
	{
		nNextInputLocation= PtrToInt(m_arrLeftLiterals.GetSize()+
			m_arrInputData.GetSize());
		for(int nIndex=nSelectionStart; 
			nIndex<m_arrInputData.GetSize()+m_arrLeftLiterals.GetSize(); 
			nIndex++)
		{
			if(IsInputPosition(nIndex))
			{
				nNextInputLocation=nIndex;
				break;
			}
		}
	}

	ASSERT(IsInputPosition(nNextInputLocation));
	return nNextInputLocation;
}


int COXEdit::GetPreviousInputLocation(int nSelectionStart)
{
	int nPreviousInputLocation=nSelectionStart;
	if(!IsInputPosition(nPreviousInputLocation))
	{
		nPreviousInputLocation=PtrToInt(m_arrLeftLiterals.GetSize());
		for(int nIndex=nSelectionStart; 
			nIndex>=m_arrLeftLiterals.GetSize(); 
			nIndex--)
		{
			if(IsInputPosition(nIndex))
			{
				nPreviousInputLocation=nIndex;
				break;
			}
		}
	}

	ASSERT(IsInputPosition(nPreviousInputLocation));
	return nPreviousInputLocation;
}


void COXEdit::Update(int nSelectionStart/*=0*/)
{
	// Update the edit control if it exists. 
	if(::IsWindow(GetSafeHwnd()))
	{
		m_nSetTextSemaphor++;
		SetWindowText(GetFormattedText());
		m_nSetTextSemaphor--;
		SetModify(TRUE);
		// We usually need to update the insertion point. 
		if(nSelectionStart>=0)
			UpdateInsertionPointForward(nSelectionStart);
	}
}


void COXEdit::UpdateInsertionPointForward(int nSelectionStart)
{
	int nNewInsertionPoint=GetNextInputLocation(nSelectionStart);
	SetSel(nNewInsertionPoint,nNewInsertionPoint);
}


void COXEdit::UpdateInsertionPointBackward(int nSelectionStart)
{
	int nNewInsertionPoint=GetPreviousInputLocation(nSelectionStart);
	SetSel(nNewInsertionPoint,nNewInsertionPoint);
}


int COXEdit::RPtoLP(int nRealPos)const 
{
	// All COXEdit functions that take cusor position as argument interpret it
	// as real position within edit control(taking into account all symbols including 
	// literals). But sometimes we want to know which non-literal symbol is at  
	// particular real position. In that case this function is really useful

	if(nRealPos<m_arrLeftLiterals.GetSize() || 
		nRealPos>m_arrLeftLiterals.GetSize()+m_arrInputData.GetSize())
	{
		return -1;
	}
	else 
	{
		return PtrToInt(nRealPos-m_arrLeftLiterals.GetSize());
	}
}

		
int COXEdit::LPtoRP(int nLogicalPos)const 
{
	if(nLogicalPos<0 || nLogicalPos>m_arrInputData.GetSize())
	{
		return -1;
	}
	else
	{
		return PtrToInt(nLogicalPos+m_arrLeftLiterals.GetSize());
	}
}


void COXEdit::ValidationError()
{
	::MessageBeep(MB_ICONEXCLAMATION);
}


CString COXEdit::GetLeftLiterals() const
{
	CString sLeftLiterals;
	for(int nIndex=0; nIndex<m_arrLeftLiterals.GetSize(); nIndex++)
	{
		sLeftLiterals+=m_arrLeftLiterals[nIndex];
	}
	return sLeftLiterals;
}


CString COXEdit::GetRightLiterals() const
{
	CString sRightLiterals;
	for(int nIndex=0; nIndex<m_arrRightLiterals.GetSize(); nIndex++)
	{
		sRightLiterals+=m_arrRightLiterals[nIndex];
	}
	return sRightLiterals;
}


BOOL COXEdit::NotifyParent(UINT nNotificationID)
{
	CWnd* pParentWnd=GetParent();
	if(pParentWnd==NULL)
		return FALSE;

	pParentWnd->SendMessage(WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(),nNotificationID),
		(LPARAM)GetSafeHwnd());
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////



////////////////
// COXNumericEdit implements a edit control which numeric values.
//
//////////////////////////////////////////////////	
//////////////////////////////////////////////////	
	
IMPLEMENT_DYNCREATE(COXNumericEdit, COXEdit)

BEGIN_MESSAGE_MAP(COXNumericEdit, COXEdit)
	//{{AFX_MSG_MAP(COXNumericEdit)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////
// Constructor
COXNumericEdit::COXNumericEdit()
{
	m_bNegativeValue=FALSE;

	m_clrTextNegative=m_clrText;

	m_bForceToUsePeriod=TRUE;

	// default values
	//
	m_nDecimalDigitCount=-1;
	m_chGroupSeparator=_T(',');
	m_nGroupLength=3;	
	m_chDecimalSeparator=_T('.');	
	m_bShowLeadingZero=TRUE;	
	m_nFractionalDigitCount=2;
	m_sFormatNegative=_T("-1.1");
	m_sFormatPositive=_T("1.1");

	m_nDecimalDigitCountDefault=-1;
	m_chGroupSeparatorDefault=_T(',');
	m_nGroupLengthDefault=3;	
	m_chDecimalSeparatorDefault=_T('.');	
	m_bShowLeadingZeroDefault=TRUE;	
	m_nFractionalDigitCountDefault=2;
	m_sFormatNegativeDefault=_T("-1.1");
	m_sFormatPositiveDefault=_T("1.1");

	//
	//////////////////////
}


BOOL COXNumericEdit::InitControl()
{
	if(!COXEdit::InitControl())
		return FALSE;
	
	return TRUE;
}


void COXNumericEdit::DeleteContents()
{
	COXEdit::DeleteContents();
	m_dValue=0.0;
}


int COXNumericEdit::InsertAt(int nSelectionStart, TCHAR chNewChar)
{
	return TranslateSymbol(nSelectionStart,chNewChar,FALSE);
}


int COXNumericEdit::SetAt(int nSelectionStart, TCHAR chNewChar)
{
	return TranslateSymbol(nSelectionStart,chNewChar,TRUE);
}


int COXNumericEdit::DeleteRange(int& nSelectionStart, int& nSelectionEnd)
{
	ASSERT(nSelectionStart >= 0);
	ASSERT(nSelectionStart <= nSelectionEnd);

	nSelectionStart = GetNextInputLocation( nSelectionStart );

	// remember the number of group delimeter symbols before deleting range
	int nGroupDelimiterSymbolCount = GetGroupSeparatorSymbolsInserted();
	int nGroupDelimiterSymbolCountBeforeRangeStartPos = 
		GetGroupSeparatorSymbolsInserted(0,RPtoLP(nSelectionStart));

	int nDeletedCount = 0;
	int nDecimalDelimiterPos = FindSymbol(GetDecimalSeparator());
	if(nDecimalDelimiterPos == -1 || 
		nDecimalDelimiterPos < nSelectionStart ||
		nDecimalDelimiterPos > nSelectionEnd)
	{
		nDeletedCount = COXEdit::DeleteRange(nSelectionStart,nSelectionEnd);
		if(nDeletedCount > 0)
		{
			//ValidateInput();
			Validate();
		}
	}
	else
	{
		if(IsNegativeValue() && nSelectionStart <= m_arrLeftLiterals.GetSize() && nSelectionEnd >= GetWindowTextLength() - m_arrRightLiterals.GetSize())
		{	// Clear the negative value flag when all contents of the edit conrol is deleted
			m_bNegativeValue = FALSE;
		}
		if(nDecimalDelimiterPos < nSelectionEnd)
		{
			int nPosition = nDecimalDelimiterPos+1;
			nDeletedCount = COXEdit::DeleteRange(nPosition,nSelectionEnd);
		}
		if(nDecimalDelimiterPos > nSelectionStart)
		{
			nDeletedCount += COXEdit::DeleteRange(
				nSelectionStart,nDecimalDelimiterPos);
		}
		if(nDeletedCount>0)
		{
			//ValidateInput();
			Validate();
		}
	}

	// take into account the fact that due to deleting of the range
	// some of group delimeter symbols might get deleted 
	nDeletedCount += nGroupDelimiterSymbolCount - GetGroupSeparatorSymbolsInserted();
	// adjust selection range based on the fact that number of
	// group delimeter symbols before the original start of the range
	// might have changed
	int nShiftRange = nGroupDelimiterSymbolCountBeforeRangeStartPos - 
		GetGroupSeparatorSymbolsInserted(0,RPtoLP(nSelectionStart));

	nSelectionStart -= nShiftRange;
	nSelectionStart = (nSelectionStart < 0 ? 0 : nSelectionStart);
	nSelectionEnd -= nShiftRange;
	nSelectionEnd = (nSelectionEnd < 0 ? 0 : nSelectionEnd);

	return nDeletedCount;
}


CString COXNumericEdit::GetInputData()const
{
	CString sInputData=COXEdit::GetInputData();
	if(GetGroupLength()>0)
	{
#if _MFC_VER>0x0421
		sInputData.Remove(GetGroupSeparator());
#else
		RemoveCharFromString(sInputData,GetGroupSeparator());
#endif
	}
	return sInputData;
}


BOOL COXNumericEdit::IsInputData(int nPosition) const
{
	if(!COXEdit::IsInputData(nPosition))
	{
		return FALSE;
	}

	if(m_arrInputData[RPtoLP(nPosition)]==GetDecimalSeparator() ||
		m_arrInputData[RPtoLP(nPosition)]==GetGroupSeparator())
	{
		return FALSE;
	}

	return TRUE;
}


BOOL COXNumericEdit::GetLocaleSettings()
{
	int nLength;
	BOOL bSetNewValue;

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SDECIMAL,NULL,0);
	if(nLength)
	{
		CString sExisting(m_chDecimalSeparator);
		CString sDefault(m_chDecimalSeparatorDefault);
		bSetNewValue=(sExisting.CompareNoCase(sDefault)==0);
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SDECIMAL,
			&m_chDecimalSeparatorDefault,nLength);
		if(bSetNewValue)
		{
			m_chDecimalSeparator=m_chDecimalSeparatorDefault;
		}
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_STHOUSAND,NULL,0);
	if(nLength)
	{
		CString sExisting(m_chGroupSeparator);
		CString sDefault(m_chGroupSeparatorDefault);
		bSetNewValue=(sExisting.CompareNoCase(sDefault)==0);
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_STHOUSAND,
			&m_chGroupSeparatorDefault,nLength);
		if(bSetNewValue)
		{
			m_chGroupSeparator=m_chGroupSeparatorDefault;
		}
	}

	TCHAR* buffer;
	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_ILZERO,NULL,0);
	if(nLength)
	{
		bSetNewValue=(m_bShowLeadingZero==m_bShowLeadingZeroDefault);
		buffer=new TCHAR[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_ILZERO,(LPTSTR)buffer,nLength);
		m_bShowLeadingZeroDefault=(BOOL)_ttoi(buffer);
		delete[] buffer;
		if(bSetNewValue)
		{
			m_bShowLeadingZero=m_bShowLeadingZeroDefault;
		}
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SGROUPING,NULL,0);
	if(nLength)
	{
		bSetNewValue=(m_nGroupLength==m_nGroupLengthDefault);
		buffer=new TCHAR[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SGROUPING,(LPTSTR)buffer,nLength);
		CString sGrouping=buffer;
		int nPosition=sGrouping.Find(_T(";0"));
		if(nPosition!=-1)
		{
			ASSERT(nPosition>0);
			sGrouping=sGrouping.Left(nPosition);
			m_nGroupLengthDefault=_ttoi(sGrouping);
		}
		delete[] buffer;
		if(bSetNewValue)
		{
			m_nGroupLength=m_nGroupLengthDefault;
		}
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_IDIGITS,NULL,0);
	if(nLength)
	{
		bSetNewValue=(m_nFractionalDigitCount==m_nFractionalDigitCountDefault);
		buffer=new TCHAR[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_IDIGITS,(LPTSTR)buffer,nLength);
		m_nFractionalDigitCountDefault=_ttoi(buffer);
		delete[] buffer;
		if(bSetNewValue)
		{
			m_nFractionalDigitCount=m_nFractionalDigitCountDefault;
		}
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_INEGNUMBER,NULL,0);
	if(nLength)
	{
		bSetNewValue=(m_sFormatNegative.CompareNoCase(m_sFormatNegativeDefault)==0);
		buffer=new TCHAR[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_INEGNUMBER,(LPTSTR)buffer,nLength);
		int nNegativeNumberFormatType=_ttoi(buffer);
		switch(nNegativeNumberFormatType)
		{
		case 0:
			m_sFormatNegativeDefault=_T("(1.1)");
			break;
		case 1:
			m_sFormatNegativeDefault=_T("-1.1");
			break;
		case 2:
			m_sFormatNegativeDefault=_T("- 1.1");
			break;
		case 3:
			m_sFormatNegativeDefault=_T("1.1-");
			break;
		case 4:
			m_sFormatNegativeDefault=_T("1.1 -");
			break;
		}
		delete[] buffer;
		if(bSetNewValue)
		{
			m_sFormatNegative=m_sFormatNegativeDefault;
		}
	}

	return TRUE;
}


int COXNumericEdit::FindSymbol(TCHAR chSymbol) const
{
	int nPosition=-1;
	for(int nIndex=0; nIndex<m_arrInputData.GetSize(); nIndex++)
	{
		if(m_arrInputData[nIndex]==chSymbol)
		{
			nPosition=LPtoRP(nIndex);
			break;
		}
	}

	return nPosition;
}


int COXNumericEdit::ValidateInput()
{
	int nSymbolsBefore=PtrToInt(m_arrInputData.GetSize());

	BOOL bSetToNegative=FALSE;
	BOOL bDecimalSeparatorFound=FALSE;
	for(int nIndex=PtrToInt(m_arrInputData.GetSize())-1; nIndex>=0; nIndex--)
	{
		if(!(m_arrInputData[nIndex]>=_T('0') && m_arrInputData[nIndex]<=_T('9')))
		{
			if(nIndex==0 && m_arrInputData[nIndex]==_T('-'))
				bSetToNegative=TRUE;

			if(m_arrInputData[nIndex]==m_chDecimalSeparator)
			{
				if(!bDecimalSeparatorFound)
					bDecimalSeparatorFound=TRUE;
				else
					m_arrInputData.RemoveAt(nIndex);
			}
			else
				m_arrInputData.RemoveAt(nIndex);
		}
	}

	int nDecimalDelimiterPos=FindSymbol(GetDecimalSeparator());
	int nDecimalDigitsInserted=GetDecimalDigitsInserted();
	int nFractionalDigitsInserted=GetFractionalDigitsInserted();

	for(;m_arrInputData.GetSize()>0;)
	{
		if(m_arrInputData[0]==_T('0') && nDecimalDigitsInserted>1)
		{
			m_arrInputData.RemoveAt(0);
			nDecimalDigitsInserted--;
			if(nDecimalDelimiterPos!=-1)
				nDecimalDelimiterPos--;
		}
		else
			break;
	}

	BOOL bUpdate=FALSE;

	int nFractionalOvertypedCount=
		nFractionalDigitsInserted-GetFractionalDigitCount();
	if(nFractionalOvertypedCount>0)
	{
		for(int nIndex=0; nIndex<nFractionalOvertypedCount; nIndex++)
			m_arrInputData.RemoveAt(m_arrInputData.GetSize()-1);
		bUpdate=TRUE;
	}

	if(m_arrInputData.GetSize()>0 && GetDecimalDigitCount()!=-1)
	{
		if(m_arrInputData[0]!=_T('0'))
		{
			int nDecimalOvertypedCount=nDecimalDigitsInserted-GetDecimalDigitCount();
			if(nDecimalOvertypedCount>0)
			{
				for(int nIndex=0; nIndex<nDecimalOvertypedCount; nIndex++)
				{
					m_arrInputData.RemoveAt(0);
					nDecimalDigitsInserted--;
					if(nDecimalDelimiterPos!=-1)
						nDecimalDelimiterPos--;
				}
				bUpdate=TRUE;
			}
		}
		else
		{
			ASSERT(nDecimalDigitsInserted==1);
		}
	}

	if(GetFractionalDigitCount()!=0)
	{
		if(nDecimalDelimiterPos==-1)
			m_arrInputData.InsertAt(m_arrInputData.GetSize(),GetDecimalSeparator());

		if(GetFractionalDigitCount()>nFractionalDigitsInserted)
		{
			for(int nIndex=nFractionalDigitsInserted; 
				nIndex<GetFractionalDigitCount(); 
				nIndex++)
			{
				m_arrInputData.InsertAt(m_arrInputData.GetSize(),_T('0'));
			}
		}

		if(nDecimalDigitsInserted==0 && GetShowLeadingZero())
			m_arrInputData.InsertAt(0,_T('0'));

		if(nDecimalDigitsInserted==1 && !GetShowLeadingZero() 
			&& m_arrInputData[0]==_T('0'))
		{
			m_arrInputData.RemoveAt(0);
			nDecimalDigitsInserted--;
			if(nDecimalDelimiterPos!=-1)
				nDecimalDelimiterPos--;
		}
	}
	else
	{
		if(nDecimalDelimiterPos!=-1)
		{
			m_arrInputData.RemoveAt(RPtoLP(nDecimalDelimiterPos));
			nDecimalDelimiterPos=-1;
		}

		if(nDecimalDigitsInserted==0 && GetShowLeadingZero())
			m_arrInputData.InsertAt(0,_T('0'));
	}

	if(GetGroupLength()>0)
	{
		int nDecimalDelimiterPos=RPtoLP(FindSymbol(m_chDecimalSeparator));
		nDecimalDelimiterPos=(nDecimalDelimiterPos==-1) ?
			PtrToInt(m_arrInputData.GetSize()) : nDecimalDelimiterPos;

		int nDigitsInRow=0;
		for(int nIndex=nDecimalDelimiterPos-1; nIndex>=0; nIndex--)
		{
			if(nDigitsInRow==GetGroupLength())
			{
				if(m_arrInputData[nIndex]!=GetGroupSeparator())
				{
					m_arrInputData.InsertAt(nIndex+1,GetGroupSeparator());
					bUpdate=TRUE;
				}
				nDigitsInRow=1;
			}
			else
			{
				if(m_arrInputData[nIndex]!=GetGroupSeparator())
					nDigitsInRow++;
			}
		}
	}


	if(bSetToNegative)
		ChangeSign(bSetToNegative);
	else
		ChangeSign(IsNegativeValue());

	if(bUpdate)
		Update(-1);

	CString sInputData=GetInputData();
	if(GetGroupLength()>0)
	{
#if _MFC_VER>0x0421
		sInputData.Remove(GetGroupSeparator());
		sInputData.Replace(GetDecimalSeparator(),_T('.'));
#else
		RemoveCharFromString(sInputData,GetGroupSeparator());
		ReplaceCharInString(sInputData,GetDecimalSeparator(),_T('.'));
#endif
	}
	TCHAR* pchStop;
	m_dValue=_tcstod(sInputData,&pchStop);
	if(IsNegativeValue())
		m_dValue*=-1;

	int nSymbolsAfter=PtrToInt(m_arrInputData.GetSize());

	return nSymbolsAfter-nSymbolsBefore;
}


int COXNumericEdit::ChangeSign(BOOL bNegative)
{
	m_bNegativeValue=bNegative;

	CString sMask=GetMask();
	int nPosInputData=sMask.Find(OXEDITABLETEXT_SYMBOL);
	CString sLeftLiterals=(nPosInputData>0 ? sMask.Left(nPosInputData) : _T(""));
	CString sRightLiterals=(nPosInputData==-1 ? 
		_T("") : sMask.Mid(nPosInputData+1));

	int nLeftLiteralsBefore=PtrToInt(m_arrLeftLiterals.GetSize());

	m_arrLeftLiterals.RemoveAll();
	m_arrRightLiterals.RemoveAll();

	int nIndex=0;
	for(nIndex=0; nIndex<sLeftLiterals.GetLength(); nIndex++)
	{
		m_arrLeftLiterals.InsertAt(m_arrLeftLiterals.GetSize(),
			sLeftLiterals[nIndex]);
	}

	if(m_bNegativeValue)
	{
		int nPosInNegative=m_sFormatNegative.Find(OXNUMERICVALUE_PLACEHOLDER);
		CString sLeftNegativeLiterals=(nPosInNegative>0 ? 
			m_sFormatNegative.Left(nPosInNegative) : _T(""));
		CString sRightNegativeLiterals=(nPosInNegative==-1 ? 
			_T("") : m_sFormatNegative.Mid(nPosInNegative+
			OXNUMERICVALUE_PLACEHOLDER_LENGTH));

		for(nIndex=0; nIndex<sLeftNegativeLiterals.GetLength(); nIndex++)
		{
			m_arrLeftLiterals.InsertAt(m_arrLeftLiterals.GetSize(),
				sLeftNegativeLiterals[nIndex]);
		}
		for(nIndex=0; nIndex<sRightNegativeLiterals.GetLength(); nIndex++)
		{
			m_arrRightLiterals.InsertAt(m_arrRightLiterals.GetSize(),
				sRightNegativeLiterals[nIndex]);
		}
	}
	else
	{
		int nPosInPositive=m_sFormatPositive.Find(OXNUMERICVALUE_PLACEHOLDER);
		CString sLeftPositiveLiterals=(nPosInPositive>0 ? 
			m_sFormatPositive.Left(nPosInPositive) : _T(""));
		CString sRightPositiveLiterals=(nPosInPositive==-1 ? 
			_T("") : m_sFormatPositive.Mid(nPosInPositive+
			OXNUMERICVALUE_PLACEHOLDER_LENGTH));

		for(nIndex=0; nIndex<sLeftPositiveLiterals.GetLength(); nIndex++)
		{
			m_arrLeftLiterals.InsertAt(m_arrLeftLiterals.GetSize(),
				sLeftPositiveLiterals[nIndex]);
		}
		for(nIndex=0; nIndex<sRightPositiveLiterals.GetLength(); nIndex++)
		{
			m_arrRightLiterals.InsertAt(m_arrRightLiterals.GetSize(),
				sRightPositiveLiterals[nIndex]);
		}
	}

	for(nIndex=0; nIndex<sRightLiterals.GetLength(); nIndex++)
	{
		m_arrRightLiterals.InsertAt(m_arrRightLiterals.GetSize(),
			sRightLiterals[nIndex]);
	}

	UpdateMask();

	Update(-1);

	if((m_bNegativeValue && m_dValue>=0) || (!m_bNegativeValue && m_dValue<0))
		m_dValue*=-1;

	return PtrToInt(m_arrLeftLiterals.GetSize())-nLeftLiteralsBefore;
}


BOOL COXNumericEdit::SetDecimalDigitCount(int nDecimalDigitCount)
{
	if(nDecimalDigitCount<-1)
	{
		TRACE(_T("COXNumericEdit::SetDecimalDigitCount: invalid decimal digit count has been specified\n"));
		return FALSE;
	}

	if(m_nDecimalDigitCount==nDecimalDigitCount)
		return TRUE;

	CString sInputData=GetInputData();
	m_nDecimalDigitCount=nDecimalDigitCount;
	EmptyData(TRUE);
	VERIFY(SetInputData(sInputData));

	return TRUE;
}


BOOL COXNumericEdit::SetDecimalSeparator(TCHAR chDecimalSeparator)
{
	if((chDecimalSeparator>=_T('0') && chDecimalSeparator<=_T('9')) ||
		chDecimalSeparator==_T('-') || chDecimalSeparator==GetGroupSeparator() ||
		chDecimalSeparator==NULL)
	{
		TRACE(_T("COXNumericEdit::SetDecimalSeparator: invalid decimal separator has been specified\n"));
		return FALSE;
	}

	int nDecimalDelimiterPos=FindSymbol(GetDecimalSeparator());
	m_chDecimalSeparator=chDecimalSeparator;
	if(nDecimalDelimiterPos!=-1)
	{
		m_arrInputData.SetAt(RPtoLP(nDecimalDelimiterPos),chDecimalSeparator);
		Update(-1);
	}

	return TRUE;
}


BOOL COXNumericEdit::SetFractionalDigitCount(int nFractionalDigitCount)
{
	if(nFractionalDigitCount<0)
	{
		TRACE(_T("COXNumericEdit::SetFractionalDigitCount: invalid fractional digit count has been specified\n"));
		return FALSE;
	}

	if(m_nFractionalDigitCount==nFractionalDigitCount)
		return TRUE;

	CString sInputData=GetInputData();
	m_nFractionalDigitCount=nFractionalDigitCount;
	EmptyData(TRUE);
	VERIFY(SetInputData(sInputData));

	return TRUE;
}

BOOL COXNumericEdit::SetGroupLength(int nGroupLength)
{
	if(nGroupLength<0)
	{
		TRACE(_T("COXNumericEdit::SetGroupLength: invalid group length has been specified\n"));
		return FALSE;
	}

	if(m_nGroupLength==nGroupLength)
	{
		return TRUE;
	}

	CString sInputData=GetInputData();
	m_nGroupLength=nGroupLength;
	EmptyData(TRUE);
	VERIFY(SetInputData(sInputData));

	return TRUE;
}


BOOL COXNumericEdit::SetGroupSeparator(TCHAR chGroupSeparator)
{
	if((chGroupSeparator>=_T('0') && chGroupSeparator<=_T('9')) ||
		chGroupSeparator==_T('-') || chGroupSeparator==GetDecimalSeparator() ||
		chGroupSeparator==NULL)
	{
		TRACE(_T("COXNumericEdit::SetGroupSeparator: invalid group separator has been specified\n"));
		return FALSE;
	}

	if(m_chGroupSeparator==chGroupSeparator)
		return TRUE;

	CString sInputData=GetInputData();
	m_chGroupSeparator=chGroupSeparator;
	EmptyData(TRUE);
	VERIFY(SetInputData(sInputData));

	return TRUE;
}


BOOL COXNumericEdit::SetNegativeFormat(LPCTSTR lpszFormatNegative)
{
	ASSERT(lpszFormatNegative!=NULL);

	CString sNegativeFormat=lpszFormatNegative;
	if(sNegativeFormat.Find(OXNUMERICVALUE_PLACEHOLDER)==-1)
	{
		TRACE(_T("COXNumericEdit::SetNegativeFormat: invalid format for displaying negative value has been specified\n"));
		return FALSE;
	}

	m_sFormatNegative=sNegativeFormat;
	ChangeSign(IsNegativeValue());

	return TRUE;
}


BOOL COXNumericEdit::SetPositiveFormat(LPCTSTR lpszFormatPositive)
{
	ASSERT(lpszFormatPositive!=NULL);

	CString sPositiveFormat=lpszFormatPositive;
	if(sPositiveFormat.Find(OXNUMERICVALUE_PLACEHOLDER)==-1)
	{
		TRACE(_T("COXNumericEdit::SetPositiveFormat: invalid format for displaying positive value has been specified\n"));
		return FALSE;
	}

	m_sFormatPositive=sPositiveFormat;
	ChangeSign(IsNegativeValue());

	return TRUE;
}


BOOL COXNumericEdit::SetValue(double dValue)
{
	m_dValue=dValue;
	m_bNegativeValue=(m_dValue<0 ? TRUE : FALSE);
	BOOL bSetNegative=m_bNegativeValue;

	int decimal, sign;

#if _MSC_VER >= 1400
			int count = GetFractionalDigitCount();
			char buffer[MAX_PATH];	
			_fcvt_s(buffer, MAX_PATH, dValue, count, &decimal,&sign);
#else
			char *buffer= UTBStr::fcvt(m_dValue,GetFractionalDigitCount(),&decimal,&sign);
#endif
	int nSymbolCount=PtrToInt(strlen(buffer));
	TCHAR* chInputData=new TCHAR[nSymbolCount+1];
#ifdef _UNICODE
	UTBStr::mbstowcs(chInputData,nSymbolCount+1,buffer,nSymbolCount);
#else
	UTBStr::strncpy(chInputData,nSymbolCount+1, buffer,nSymbolCount);
#endif
	chInputData[nSymbolCount]=_T('\0');

	CString sInputData=chInputData;
	CString sDecimalSeparator(GetDecimalSeparator());
	if(!sInputData.IsEmpty())
	{
		for(int nIndex=decimal; nIndex<1; nIndex++)
		{
			sInputData=_T("0")+sInputData;
			decimal++;
			nSymbolCount++;
		}
		sInputData=sInputData.Left(decimal)+sDecimalSeparator+sInputData.Mid(decimal);
		if(sign!=0)
		{
			sInputData=_T("-")+sInputData;
		}
	}
	EmptyData(TRUE);
	VERIFY(COXEdit::SetInputData(sInputData));

	delete[] chInputData;

	ChangeSign(bSetNegative);

	return TRUE;
}


void COXNumericEdit::SetShowLeadingZero(BOOL bShowLeadingZero)
{
	if(m_bShowLeadingZero==bShowLeadingZero)
	{
		return;
	}

	CString sInputData=GetInputData();
	m_bShowLeadingZero=bShowLeadingZero;
	EmptyData(TRUE);
	VERIFY(SetInputData(sInputData));

	return;
}


void COXNumericEdit::SetNegativeTextColor(COLORREF clrTextNegative, 
										  BOOL bRedraw/*=TRUE*/)
{
	if(m_clrTextNegative!=clrTextNegative)
	{
		m_clrTextNegative=clrTextNegative;
		if(bRedraw && IsNegativeValue())
			RedrawWindow();
	}
}


int COXNumericEdit::TranslateSymbol(int nSelectionStart, 
									TCHAR chNewChar, BOOL bReplaceExisting)
{
	if((chNewChar>=_T('0') && chNewChar<=_T('9')) || 
		chNewChar==m_chDecimalSeparator || chNewChar==_T('-') ||
		(m_bForceToUsePeriod && chNewChar==_T('.')))
	{
		if(chNewChar==_T('-'))
		{
			nSelectionStart+=ChangeSign(!IsNegativeValue())-1;
			return nSelectionStart;
		}

		if(chNewChar==m_chDecimalSeparator || 
			(m_bForceToUsePeriod && chNewChar==_T('.')))
		{
			if(GetFractionalDigitCount()==0)
				return -1;
			int nPos=FindSymbol(m_chDecimalSeparator);
			if(nPos==-1)
			{
				nSelectionStart=PtrToInt(m_arrLeftLiterals.GetSize()+m_arrInputData.GetSize());
				return COXEdit::InsertAt(nSelectionStart,chNewChar);
			}
			else
				return GetNextInputLocation(nPos);
		}

		if(m_arrInputData.GetSize()>0 && chNewChar==_T('0') && 
			RPtoLP(nSelectionStart)==0 && IsInputData(nSelectionStart))
		{
			if(bReplaceExisting)
			{
				m_arrInputData.RemoveAt(0);
				//ValidateInput();
				Validate();
				if(m_arrInputData[0]==_T('0') || 
					m_arrInputData[0]==GetDecimalSeparator())
					return nSelectionStart;
				else
					return -1;
			}
			else
				return -1;
		}

		if(m_arrInputData.GetSize()>0 && RPtoLP(nSelectionStart)==0)
		{
			if(bReplaceExisting && m_arrInputData[0]==GetDecimalSeparator())
				return COXEdit::InsertAt(nSelectionStart,chNewChar);
			else if(!bReplaceExisting && m_arrInputData[0]==_T('0'))
				return COXEdit::SetAt(nSelectionStart,chNewChar);
		}

		int nInsertedAtPos=-1;
		if(bReplaceExisting)
		{
			nInsertedAtPos=COXEdit::SetAt(nSelectionStart,chNewChar);
		}
		else
		{
			int nDecimalDelimiterPos=FindSymbol(GetDecimalSeparator());
			if(nDecimalDelimiterPos==-1 || nDecimalDelimiterPos>=nSelectionStart)
			{
				if(nDecimalDelimiterPos==-1 && GetDecimalDigitsInserted()==1 &&
					m_arrInputData[0]==_T('0'))
					nInsertedAtPos=COXEdit::SetAt(nSelectionStart-1,chNewChar);
				else if(GetDecimalDigitsInserted()<GetDecimalDigitCount() ||
					GetDecimalDigitCount()==-1)
					nInsertedAtPos=COXEdit::InsertAt(nSelectionStart,chNewChar);
				else
					nInsertedAtPos=COXEdit::SetAt(nSelectionStart,chNewChar);
			}
			else
			{
				if(GetFractionalDigitsInserted()<GetFractionalDigitCount())
					nInsertedAtPos=COXEdit::InsertAt(nSelectionStart,chNewChar);
				else
					nInsertedAtPos=COXEdit::SetAt(nSelectionStart,chNewChar);
			}
		}

		return nInsertedAtPos;
	}

	return -1;
}


int COXNumericEdit::GetDecimalDigitsInserted() const
{
	int nCount=0;
	int nDecimalDelimiterPos=FindSymbol(GetDecimalSeparator());
	nDecimalDelimiterPos=(nDecimalDelimiterPos==-1 ? 
		PtrToInt(m_arrInputData.GetSize()) : RPtoLP(nDecimalDelimiterPos));
	for(int nIndex=0; nIndex<nDecimalDelimiterPos; nIndex++)
	{
		if(m_arrInputData[nIndex]>=_T('0') && m_arrInputData[nIndex]<=_T('9'))
		{
			nCount++;
		}
	}

	return nCount;
}


int COXNumericEdit::GetFractionalDigitsInserted() const
{
	int nCount=0;
	int nDecimalDelimiterPos=FindSymbol(GetDecimalSeparator());
	nDecimalDelimiterPos=(nDecimalDelimiterPos==-1 ? 
		PtrToInt(m_arrInputData.GetSize()) : RPtoLP(nDecimalDelimiterPos));
	for(int nIndex=nDecimalDelimiterPos+1; nIndex<m_arrInputData.GetSize(); nIndex++)
	{
		if(m_arrInputData[nIndex]>=_T('0') && m_arrInputData[nIndex]<=_T('9'))
		{
			nCount++;
		}
	}

	return nCount;
}


int COXNumericEdit::GetGroupSeparatorSymbolsInserted(int nStartPos/* = 0*/,
													 int nEndPos/* = -1*/) const
{
	if(nEndPos == -1)
	{
		nEndPos = PtrToInt(m_arrInputData.GetSize()) - 1;
	}
	ASSERT(nEndPos >= nStartPos);
	ASSERT(nStartPos >= 0 && nStartPos <= m_arrInputData.GetSize());
	ASSERT(nEndPos >= 0 && nEndPos <= m_arrInputData.GetSize());

	int nCount = 0;
	int nDecimalDelimiterPos = FindSymbol(GetDecimalSeparator());
	nDecimalDelimiterPos = (nDecimalDelimiterPos == -1 ? 
		PtrToInt(m_arrInputData.GetSize()) : RPtoLP(nDecimalDelimiterPos));
	for(int nIndex = nStartPos; nIndex < min(nDecimalDelimiterPos,nEndPos); nIndex++)
	{
		if(m_arrInputData[nIndex] == GetGroupSeparator())
		{
			nCount++;
		}
	}

	return nCount;
}


//////////////////
// Handle reflected WM_CTLCOLOR to set custom control color and font.
// 
HBRUSH COXNumericEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	HBRUSH hBrush=COXEdit::CtlColor(pDC,nCtlColor);
	if(IsNegativeValue())
		pDC->SetTextColor(GetNegativeTextColor());
	return hBrush;
}



// global:
void AFXAPI DDX_OXNumericEdit(CDataExchange* pDX, int nIDC, double& dValue)
{
	// be sure that the control is a COXNumericEdit
	COXNumericEdit* pEdit = (COXNumericEdit*)pDX->m_pDlgWnd->GetDlgItem(nIDC);
	ASSERT(pEdit->IsKindOf(RUNTIME_CLASS(COXNumericEdit)));
	ASSERT_VALID(pEdit);
	if (pDX->m_bSaveAndValidate)
	{
		// retreive data
		dValue = pEdit->GetValue();
	}
	else
	{
		// set data
		pEdit->SetValue(dValue);
	}
}

void AFXAPI DDX_OXNumericEdit(CDataExchange* pDX, int nIDC, float& fValue)
{
	// be sure that the control is a COXNumericEdit
	COXNumericEdit* pEdit = (COXNumericEdit*)pDX->m_pDlgWnd->GetDlgItem(nIDC);
	ASSERT(pEdit->IsKindOf(RUNTIME_CLASS(COXNumericEdit)));
	ASSERT_VALID(pEdit);
	if (pDX->m_bSaveAndValidate)
	{
		// retreive data
		fValue = (float)pEdit->GetValue();
	}
	else
	{
		// set data
		pEdit->SetValue((double)fValue);
	}
}

void AFXAPI DDX_OXNumericEdit(CDataExchange* pDX, int nIDC, int& nValue)
{
	// be sure that the control is a COXNumericEdit
	COXNumericEdit* pEdit = (COXNumericEdit*)pDX->m_pDlgWnd->GetDlgItem(nIDC);
	ASSERT(pEdit->IsKindOf(RUNTIME_CLASS(COXNumericEdit)));
	ASSERT_VALID(pEdit);
	if (pDX->m_bSaveAndValidate)
	{
		// retreive data
		nValue = (int)pEdit->GetValue();
	}
	else
	{
		// set data
		pEdit->SetValue((double)nValue);
	}
}

void AFXAPI DDX_OXNumericEdit(CDataExchange* pDX, int nIDC, long& nValue)
{
	// be sure that the control is a COXNumericEdit
	COXNumericEdit* pEdit = (COXNumericEdit*)pDX->m_pDlgWnd->GetDlgItem(nIDC);
	ASSERT(pEdit->IsKindOf(RUNTIME_CLASS(COXNumericEdit)));
	ASSERT_VALID(pEdit);
	if (pDX->m_bSaveAndValidate)
	{
		// retreive data
		nValue = (long)pEdit->GetValue();
	}
	else
	{
		// set data
		pEdit->SetValue((double)nValue);
	}
}


/////////////////////////////////////////////////////////////////////////////



////////////////
// COXCurrencyEdit implements a edit control which numeric values.
//
//////////////////////////////////////////////////	
//////////////////////////////////////////////////	
	
IMPLEMENT_DYNCREATE(COXCurrencyEdit, COXNumericEdit)

BEGIN_MESSAGE_MAP(COXCurrencyEdit, COXNumericEdit)
	//{{AFX_MSG_MAP(COXCurrencyEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////
// Constructor
COXCurrencyEdit::COXCurrencyEdit()
{
	m_nDecimalDigitCount=-1;
	m_chGroupSeparator=_T(',');
	m_nGroupLength=3;	
	m_chDecimalSeparator=_T('.');	
	m_bShowLeadingZero=TRUE;	
	m_nFractionalDigitCount=2;
	m_sFormatNegative=_T("($1.1)");
	m_sFormatPositive=_T("$1.1");
	m_sCurrency=_T("$");

	m_nDecimalDigitCountDefault=-1;
	m_chGroupSeparatorDefault=_T(',');
	m_nGroupLengthDefault=3;	
	m_chDecimalSeparatorDefault=_T('.');	
	m_bShowLeadingZeroDefault=TRUE;	
	m_nFractionalDigitCountDefault=2;
	m_sFormatNegativeDefault=_T("($1.1)");
	m_sFormatPositiveDefault=_T("$1.1");
	m_sCurrencyDefault=_T("$");
}


BOOL COXCurrencyEdit::GetLocaleSettings()
{
	int nLength;
	BOOL bSetNewValue;


	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SMONDECIMALSEP,NULL,0);
	if(nLength)
	{
		CString sExisting(m_chDecimalSeparator);
		CString sDefault(m_chDecimalSeparatorDefault);
		bSetNewValue=(sExisting.CompareNoCase(sDefault)==0);
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SMONDECIMALSEP,
			&m_chDecimalSeparatorDefault,nLength);
		if(bSetNewValue)
		{
			m_chDecimalSeparator=m_chDecimalSeparatorDefault;
		}
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SMONTHOUSANDSEP,NULL,0);
	if(nLength)
	{
		CString sExisting(m_chGroupSeparator);
		CString sDefault(m_chGroupSeparatorDefault);
		bSetNewValue=(sExisting.CompareNoCase(sDefault)==0);
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SMONTHOUSANDSEP,
			&m_chGroupSeparatorDefault,nLength);
		if(bSetNewValue)
		{
			m_chGroupSeparator=m_chGroupSeparatorDefault;
		}
	}

	TCHAR* buffer;
	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_ILZERO,NULL,0);
	if(nLength)
	{
		bSetNewValue=(m_bShowLeadingZero==m_bShowLeadingZeroDefault);
		buffer=new TCHAR[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_ILZERO,(LPTSTR)buffer,nLength);
		m_bShowLeadingZeroDefault=(BOOL)_ttoi(buffer);
		delete[] buffer;
		if(bSetNewValue)
		{
			m_bShowLeadingZero=m_bShowLeadingZeroDefault;
		}
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SMONGROUPING,NULL,0);
	if(nLength)
	{
		bSetNewValue=(m_nGroupLength==m_nGroupLengthDefault);
		buffer=new TCHAR[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SMONGROUPING,(LPTSTR)buffer,nLength);
		CString sGrouping=buffer;
		int nPosition=sGrouping.Find(_T(";0"));
		if(nPosition!=-1)
		{
			ASSERT(nPosition>0);
			sGrouping=sGrouping.Left(nPosition);
			m_nGroupLengthDefault=_ttoi(sGrouping);
		}
		delete[] buffer;
		if(bSetNewValue)
		{
			m_nGroupLength=m_nGroupLengthDefault;
		}
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_ICURRDIGITS,NULL,0);
	if(nLength)
	{
		bSetNewValue=(m_nFractionalDigitCount==m_nFractionalDigitCountDefault);
		buffer=new TCHAR[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_ICURRDIGITS,(LPTSTR)buffer,nLength);
		m_nFractionalDigitCountDefault=_ttoi(buffer);
		delete[] buffer;
		if(bSetNewValue)
		{
			m_nFractionalDigitCount=m_nFractionalDigitCountDefault;
		}
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SCURRENCY,NULL,0);
	if(nLength)
	{
		bSetNewValue=(m_sCurrency.CompareNoCase(m_sCurrencyDefault)==0);
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SCURRENCY,
			m_sCurrencyDefault.GetBuffer(nLength-1),nLength);
		m_sCurrencyDefault.ReleaseBuffer();
		if(bSetNewValue)
		{
			m_sCurrency=m_sCurrencyDefault;
		}
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_ICURRENCY,NULL,0);
	if(nLength)
	{
		bSetNewValue=(m_sFormatPositive.CompareNoCase(m_sFormatPositiveDefault)==0);
		buffer=new TCHAR[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_ICURRENCY,(LPTSTR)buffer,nLength);
		int nPositiveNumberFormatType=_ttoi(buffer);
		switch(nPositiveNumberFormatType)
		{
		case 0:
			m_sFormatPositiveDefault=_T("$1.1");
			break;
		case 1:
			m_sFormatPositiveDefault=_T("1.1$");
			break;
		case 2:
			m_sFormatPositiveDefault=_T("$ 1.1");
			break;
		case 3:
			m_sFormatPositiveDefault=_T("1.1 $");
			break;
		}
		delete[] buffer;
		if(bSetNewValue)
		{
			m_sFormatPositive=m_sFormatPositiveDefault;
		}
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_INEGCURR,NULL,0);
	if(nLength)
	{
		bSetNewValue=(m_sFormatNegative.CompareNoCase(m_sFormatNegativeDefault)==0);
		buffer=new TCHAR[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_INEGCURR,(LPTSTR)buffer,nLength);
		int nNegativeNumberFormatType=_ttoi(buffer);
		switch(nNegativeNumberFormatType)
		{
		case 0:
			m_sFormatNegativeDefault=_T("($1.1)");
			break;
		case 1:
			m_sFormatNegativeDefault=_T("-$1.1");
			break;
		case 2:
			m_sFormatNegativeDefault=_T("$-1.1");
			break;
		case 3:
			m_sFormatNegativeDefault=_T("$1.1-");
			break;
		case 4:
			m_sFormatNegativeDefault=_T("(1.1$)");
			break;
		case 5:
			m_sFormatNegativeDefault=_T("-1.1$");
			break;
		case 6:
			m_sFormatNegativeDefault=_T("1.1-$");
			break;
		case 7:
			m_sFormatNegativeDefault=_T("1.1$-");
			break;
		case 8:
			m_sFormatNegativeDefault=_T("-1.1 $");
			break;
		case 9:
			m_sFormatNegativeDefault=_T("-$ 1.1");
			break;
		case 10:
			m_sFormatNegativeDefault=_T("1.1 $-");
			break;
		case 11:
			m_sFormatNegativeDefault=_T("$ 1.1-");
			break;
		case 12:
			m_sFormatNegativeDefault=_T("$ -1.1");
			break;
		case 13:
			m_sFormatNegativeDefault=_T("1.1- $");
			break;
		case 14:
			m_sFormatNegativeDefault=_T("($ 1.1)");
			break;
		case 15:
			m_sFormatNegativeDefault=_T("(1.1 $)");
			break;
		}
		delete[] buffer;
		if(bSetNewValue)
		{
			m_sFormatNegative=m_sFormatNegativeDefault;
		}
	}

	return TRUE;
}


BOOL COXCurrencyEdit::SetCurrencyName(LPCTSTR lpszCurrency)
{
	if(lpszCurrency==NULL)
		return FALSE;

	CString sCurrency=lpszCurrency;
	if(sCurrency.GetLength()==0)
		return FALSE;

	CString sInputData=GetInputData();
	m_sCurrency=sCurrency;
	EmptyData(TRUE);
	VERIFY(SetInputData(sInputData));

	return TRUE;
}


BOOL COXCurrencyEdit::SetNegativeFormat(LPCTSTR lpszFormatNegative)
{
	ASSERT(lpszFormatNegative!=NULL);

	CString sNegativeFormat=lpszFormatNegative;
	if(sNegativeFormat.Find(OXNUMERICVALUE_PLACEHOLDER)==-1 || 
		sNegativeFormat.Find(_T("$"))==-1)
	{
		TRACE(_T("COXCurrencyEdit::SetNegativeFormat: invalid format for displaying negative value has been specified\n"));
		return FALSE;
	}

	return COXNumericEdit::SetNegativeFormat(lpszFormatNegative);
}


BOOL COXCurrencyEdit::SetPositiveFormat(LPCTSTR lpszFormatPositive)
{
	ASSERT(lpszFormatPositive!=NULL);

	CString sPositiveFormat=lpszFormatPositive;
	if(sPositiveFormat.Find(OXNUMERICVALUE_PLACEHOLDER)==-1 || 
		sPositiveFormat.Find(_T("$"))==-1)
	{
		TRACE(_T("COXCurrencyEdit::SetPositiveFormat: invalid format for displaying positive value has been specified\n"));
		return FALSE;
	}

	return COXNumericEdit::SetPositiveFormat(lpszFormatPositive);
}


void COXCurrencyEdit::UpdateMask()
{
	COXNumericEdit::UpdateMask();

	CString sCurrency=GetCurrencyName();
	ASSERT(sCurrency.GetLength()>0);

	CString sFormat=(IsNegativeValue() ? GetNegativeFormat() : GetPositiveFormat());
	int nPosCurrency=sFormat.Find(OXCURRENCY_SYMBOL);
	ASSERT(nPosCurrency!=-1);
	int nPosValue=sFormat.Find(OXNUMERICVALUE_PLACEHOLDER);
	ASSERT(nPosValue!=-1);

	CString sMask=GetMask();
	int nPosInputData=sMask.Find(OXEDITABLETEXT_SYMBOL);

	if(nPosCurrency<nPosValue)
	{
		CString sLeftLiterals=(nPosInputData==-1 ? _T("") : sMask.Left(nPosInputData));
		ASSERT(m_arrLeftLiterals[sLeftLiterals.GetLength()+nPosCurrency]==
			OXCURRENCY_SYMBOL);
		m_arrLeftLiterals.RemoveAt(sLeftLiterals.GetLength()+nPosCurrency);
		for(int nIndex=0; nIndex<sCurrency.GetLength(); nIndex++)
		{
			m_arrLeftLiterals.InsertAt(sLeftLiterals.GetLength()+
				nPosCurrency+nIndex,sCurrency[nIndex]);
		}
	}
	else
	{
		nPosCurrency-=nPosValue+OXNUMERICVALUE_PLACEHOLDER_LENGTH;
		CString sRightLiterals=(nPosInputData==-1 ? _T("") : sMask.Mid(nPosInputData+1));
		ASSERT(m_arrRightLiterals[nPosCurrency]==OXCURRENCY_SYMBOL);
		m_arrRightLiterals.RemoveAt(nPosCurrency);
		for(int nIndex=0; nIndex<sCurrency.GetLength(); nIndex++)
		{
			m_arrRightLiterals.InsertAt(nPosCurrency+nIndex,sCurrency[nIndex]);
		}
	}
}


// global:
void AFXAPI DDX_OXCurrencyEdit(CDataExchange* pDX, int nIDC, double& dValue)
{
	// be sure that the control is a COXNumericEdit
	COXCurrencyEdit* pEdit = (COXCurrencyEdit*)pDX->m_pDlgWnd->GetDlgItem(nIDC);
	ASSERT(pEdit->IsKindOf(RUNTIME_CLASS(COXCurrencyEdit)));
	ASSERT_VALID(pEdit);
	if (pDX->m_bSaveAndValidate)
	{
		// retreive data
		dValue = pEdit->GetValue();
	}
	else
	{
		// set data
		pEdit->SetValue(dValue);
	}
}

void AFXAPI DDX_OXCurrencyEdit(CDataExchange* pDX, int nIDC, float& fValue)
{
	// be sure that the control is a COXNumericEdit
	COXCurrencyEdit* pEdit = (COXCurrencyEdit*)pDX->m_pDlgWnd->GetDlgItem(nIDC);
	ASSERT(pEdit->IsKindOf(RUNTIME_CLASS(COXCurrencyEdit)));
	ASSERT_VALID(pEdit);
	if (pDX->m_bSaveAndValidate)
	{
		// retreive data
		fValue = (float)pEdit->GetValue();
	}
	else
	{
		// set data
		pEdit->SetValue((double)fValue);
	}
}

void AFXAPI DDX_OXCurrencyEdit(CDataExchange* pDX, int nIDC, int& nValue)
{
	// be sure that the control is a COXNumericEdit
	COXCurrencyEdit* pEdit = (COXCurrencyEdit*)pDX->m_pDlgWnd->GetDlgItem(nIDC);
	ASSERT(pEdit->IsKindOf(RUNTIME_CLASS(COXCurrencyEdit)));
	ASSERT_VALID(pEdit);
	if (pDX->m_bSaveAndValidate)
	{
		// retreive data
		nValue = (int)pEdit->GetValue();
	}
	else
	{
		// set data
		pEdit->SetValue((double)nValue);
	}
}
///////////////////////////////////////////////////////////////////////////////

int COXEdit::Validate()
{
	static CString sFullText;
	int nRet=ValidateInput();

	CString sNewText=GetInputData();
	if ((::IsWindow(m_hWnd)) && (UINT) sNewText.GetLength()>GetLimitText())
	{
		int n=0;
		for (n=0; n<sFullText.GetLength(); n++)
			m_arrInputData.SetAtGrow(n,sFullText.GetAt(n));
		m_arrInputData.SetAtGrow(n,NULL);
	}
	sFullText=COXEdit::GetInputData();
	return nRet;

}	