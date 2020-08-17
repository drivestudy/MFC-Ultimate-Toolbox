// OXBrowseColorEdit.cpp : implementation file
//
// Version: 9.3

#include "stdafx.h"
#include <math.h>
#include "OXBrowseColorEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXBrowseColorEdit

IMPLEMENT_DYNCREATE(COXBrowseColorEdit, COXEdit)

void COXBrowseColorEdit::OnSetColor(COLORREF clr) 
{ 
	if(::IsWindow(GetSafeHwnd()))
	{
		if (m_bSetColor)
		{
			if(clr==ID_CLRPICK_COLOR_NONE)
			{
				SetBkColor(::GetSysColor(COLOR_WINDOW));
				SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
			}
			else
			{
				SetBkColor(clr);
				SetTextColor(RGB(abs(64-GetRValue(clr)),abs(128-GetGValue(clr)),
					abs(255-GetBValue(clr))));
			}
		}
		EmptyData(TRUE);
		SetInputData(GetColorText(clr));
	}
}


void AFXAPI DDX_EditColorPicker(CDataExchange *pDX, int nIDC, COLORREF& clr)
{
    HWND hWnd=pDX->PrepareCtrl(nIDC);
    ASSERT(hWnd!=NULL);                
    
    COXBrowseColorEdit* pEdit=(COXBrowseColorEdit*)CWnd::FromHandle(hWnd);
    if(pDX->m_bSaveAndValidate)
    {
        clr=pEdit->GetColor();
    }
    else 
    {
        pEdit->SetColor(clr);
    }
}


void COXBrowseColorEdit::ShowColorInBackground(BOOL bShow)
{
	m_bSetColor = bShow;
}
