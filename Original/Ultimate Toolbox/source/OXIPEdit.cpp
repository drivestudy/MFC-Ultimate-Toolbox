// OXIPEdit.cpp : implementation file
//

#include "stdafx.h"
#include "OXIPEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXIPEdit

IMPLEMENT_DYNCREATE(COXIPEdit, COXMaskedEdit)

COXIPEdit::COXIPEdit()
{
	SetMask(_T("###.###.###.###"));
}

COXIPEdit::~COXIPEdit()
{
}


BEGIN_MESSAGE_MAP(COXIPEdit, COXMaskedEdit)
	//{{AFX_MSG_MAP(COXIPEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COXIPEdit message handlers

BOOL COXIPEdit::IsValidIP(LPCTSTR lpszTestIP)
{
	ASSERT(lpszTestIP != NULL);

	if (IsEmptyIP(lpszTestIP))
		return TRUE;

	BYTE btDummy1, btDummy2, btDummy3, btDummy4;
	return GetIPFromString(lpszTestIP, btDummy1, btDummy2, btDummy3, btDummy4);
}

BOOL COXIPEdit::IsEmptyIP(LPCTSTR lpszTestIP)
{
	CString strTestIP = lpszTestIP;
	if (strTestIP.IsEmpty())
		return TRUE;

	if (strTestIP == _T("   .   .   .   "))
		return TRUE;
	else
		return FALSE;
}

CString COXIPEdit::GetText()
{
	CString strText = COXMaskedEdit::GetText();
	
	if (IsEmptyIP(strText))
		return _T("");
	else if (IsValidIP(strText))
	{
		strText.Remove(' ');
		return strText;
	}
	else
		return GetErrorString();
}

CString COXIPEdit::GetErrorString()
{
	return _T("<bad address>");
}

void COXIPEdit::SetText(LPCTSTR lpszIP)
{
	if (IsEmptyIP(lpszIP))
		COXMaskedEdit::SetText(_T(""));
	else if (IsValidIP(lpszIP))
	{
		// Format the ip for the masked edit control
		BYTE nField0, nField1, nField2, nField3;
		GetIPFromString(lpszIP, nField0, nField1, nField2, nField3);
		CString strIP;
		strIP.Format(_T("%3u.%3u.%3u.%3u"), nField0, nField1, nField2, nField3);

		COXMaskedEdit::SetText(strIP);
	}
	else
		COXMaskedEdit::SetText(_T(""));
}

CString COXIPEdit::GetStringFromIP(BYTE nField0, BYTE nField1, BYTE nField2, BYTE nField3)
{
	CString strIPAddress;
	strIPAddress.Format(_T("%u.%u.%u.%u"), nField0, nField1, nField2, nField3);
	return strIPAddress;
}

BOOL COXIPEdit::GetIPFromString(LPCTSTR lpszIPString, BYTE& nField0, BYTE& nField1, BYTE& nField2, BYTE& nField3)
{
	ASSERT(lpszIPString != NULL);
	CString strTestIP = lpszIPString;

	// Remove all spaces
	strTestIP.Remove(' ');

	// Make sure the IP address contains 3 dots and the other characters are numbers only
	int iDotCount = 0;
	for (int i = 0; i < strTestIP.GetLength(); i++)
	{
		if (strTestIP[i] == '.')
			iDotCount++;
		else if (strTestIP[i] < '0' || strTestIP[i] > '9')
			return FALSE; // non numeric character found
	}

	if (iDotCount != 3)
		return FALSE; // does not contain 3 dots

	// Replace the 3 dots with spaces and read the numbers
	strTestIP.Replace('.', ' ');
	UINT nF0, nF1, nF2, nF3;

#if _MSC_VER >= 1400
	::_stscanf_s(strTestIP, _T("%u %u %u %u"), &nF0, &nF1, &nF2, &nF3);
#else
	::_stscanf(strTestIP, _T("%u %u %u %u"), &nF0, &nF1, &nF2, &nF3);
#endif
	// Make sure none of the numbers are greater than 255
	if (nF0 > 255 || nF1 > 255 || nF2 > 255 || nF3 > 255)
		return FALSE; // one or more of the fields in greater than 255

	nField0 = (BYTE) nF0;
	nField1 = (BYTE) nF1;
	nField2 = (BYTE) nF2;
	nField3 = (BYTE) nF3;

	return TRUE;
}
