// ==========================================================================
// 				Class Implementation : COXPhysicalEdit
// ==========================================================================

// Source file : OXPhysicalEdit.cpp

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.
                         
// //////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"				// standard MFC include
#include "OXPhysicalEdit.h"
#include "UTB64Bit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COXPhysicalEdit, COXNumericEdit)

/////////////////////////////////////////////////////////////////////////////
// Definition of static members
// Data members -------------------------------------------------------------
// protected:
	// int m_nUnitIndex;
	// --- The last used unit (during SetValue or construction) of this edit control

	// COXConversionData* m_pConversionData;
	// --- Pointer to the conversion data or NULL

// private:
	
// Member functions ---------------------------------------------------------
// public:

BEGIN_MESSAGE_MAP(COXPhysicalEdit, COXNumericEdit)
	//{{AFX_MSG_MAP(COXPhysicalEdit)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_OXPHYSICALEDIT_INDEXBASE,0xffff,OnChangeUnit)
END_MESSAGE_MAP()

COXPhysicalEdit::COXPhysicalEdit()
	:
	m_nUnitIndex(-1),
	m_pConversionData(NULL),
	m_bShowUnit(FALSE)
{
}

COXPhysicalEdit::COXPhysicalEdit(COXConversionData* pConversionData, 
								 int nDefaultUnitIndex /* = -1 */,
								 BOOL bShowUnit/*=TRUE*/)
	:
	m_nUnitIndex(nDefaultUnitIndex),
	m_pConversionData(pConversionData),
	m_bShowUnit(FALSE)
{
	if (nDefaultUnitIndex == -1)
	{
		// No default unit was specified, than valid conversion data must be supplied,
		// so we can extract a default unit from that data
		ASSERT(pConversionData != NULL);
		ASSERT(AfxIsValidAddress(pConversionData, sizeof(COXConversionData)));
		m_nUnitIndex = pConversionData->m_nDefaultUnitIndex;
	}

	if(pConversionData!=NULL)
	{
		VERIFY(m_menu.CreatePopupMenu());
		LPCTSTR* arrUnitNames=GetUnitNames();
		for(int nIndex=0; nIndex<GetNumberOfUnits(); nIndex++)
		{
			m_menu.AppendMenu(MF_STRING,nIndex+ID_OXPHYSICALEDIT_INDEXBASE,
				arrUnitNames[nIndex]);
		}
		AssociateMenu(&m_menu);
	}

	ShowUnit(bShowUnit);


	ASSERT_VALID(this);
}

void COXPhysicalEdit::SetValue(double dValue, int nUnit /* = -1 */, 
							   LPCTSTR pszFormat /* = NULL */)
{
	ASSERT_VALID(this);
//	ASSERT((pszFormat == NULL) || AfxIsValidString(pszFormat));
	UNREFERENCED_PARAMETER(pszFormat);

	// ... Check for default unit
	if (nUnit==-1)
		nUnit=GetDefaultUnit();
	if(m_nUnitIndex!=nUnit)
		m_nUnitIndex=nUnit;

	COXNumericEdit::SetValue(dValue);

	ASSERT_VALID(this);
}

double COXPhysicalEdit::GetValue(int nUnit /* = -1 */, BOOL bNotify /* = TRUE */)
{
	UNREFERENCED_PARAMETER(bNotify);

	ASSERT_VALID(this);

	double dValue=COXNumericEdit::GetValue();
	if(nUnit!=m_nUnitIndex)
	{
		if(nUnit==-1)
		{
			nUnit=GetDefaultUnit();
		}
		dValue=Convert(dValue,m_nUnitIndex,nUnit);
	}

	ASSERT_VALID(this);
	return dValue;
}

void COXPhysicalEdit::UpdateMask()
{
	COXNumericEdit::UpdateMask();
	if(GetShowUnit())
	{
		m_bShowUnit=FALSE;
		ShowUnit(TRUE);
	}
}

void COXPhysicalEdit::ShowUnit(BOOL bShowUnit)
{
	if(bShowUnit!=m_bShowUnit)
	{
		ASSERT(m_nUnitIndex!=-1);
		CString sUnitName(_T(" "));
		sUnitName+=GetUnitNames()[m_nUnitIndex];

		CString sMask=GetMask();
		int nPosInputData=sMask.Find(OXEDITABLETEXT_SYMBOL);
		CString sRightLiterals=(nPosInputData==-1 ? _T("") : sMask.Mid(nPosInputData+1));

		int nOffset=PtrToInt(m_arrRightLiterals.GetSize()-sRightLiterals.GetLength());
		ASSERT(nOffset>=0);
		for(int nIndex=0; nIndex<sUnitName.GetLength(); nIndex++)
		{
			if(bShowUnit)
			{
				m_arrRightLiterals.InsertAt(nIndex+nOffset,sUnitName[nIndex]);
			}
			else
			{
				ASSERT(m_arrRightLiterals.GetSize()>0);
				m_arrRightLiterals.RemoveAt(nOffset-sUnitName.GetLength());
			}
		}

		m_bShowUnit=bShowUnit;

		Update(-1);
	}
}

double COXPhysicalEdit::Convert(double dValue, int nUnitFrom, int nUnitTo)
{
	ASSERT_VALID(this);

	// MAke a call to the static version
	return ConversionHelper(dValue, nUnitFrom, nUnitTo, GetConversionParams());
}

double COXPhysicalEdit::ConversionHelper(double dValue, int nUnitFrom, int nUnitTo, COXConversionParams* pConversionParams)
{
	ASSERT(pConversionParams != NULL);
	ASSERT(AfxIsValidAddress(pConversionParams, sizeof(COXConversionParams)));

	// ... Convert value to default unit
	double dDefValue = (dValue + pConversionParams[nUnitFrom].m_dPreTerm) * 
		pConversionParams[nUnitFrom].m_dFactor + pConversionParams[nUnitFrom].m_dPostTerm;
	// ... and then to requested unit
	ASSERT(pConversionParams[nUnitTo].m_dFactor != 0);
	double dRequestValue = (dDefValue - pConversionParams[nUnitTo].m_dPostTerm) /
		pConversionParams[nUnitTo].m_dFactor - pConversionParams[nUnitTo].m_dPreTerm;
	return dRequestValue;
}

int	COXPhysicalEdit::GetNumberOfUnits()
{
	ASSERT_VALID(this);

	// If this function is called, m_pConversionData should point to valid data
	// If you do not want to use m_pConversionData, you should override this function
	// in a derived class
	ASSERT(m_pConversionData != NULL);

	return m_pConversionData->m_nNumberOfUnits;
}

int	COXPhysicalEdit::GetDefaultUnit()
{
	ASSERT_VALID(this);

	// If this function is called, m_pConversionData should point to valid data
	// If you do not want to use m_pConversionData, you should override this function
	// in a derived class
	ASSERT(m_pConversionData != NULL);

	return m_pConversionData->m_nDefaultUnitIndex;
}

LPCTSTR* COXPhysicalEdit::GetUnitNames()
{
	ASSERT_VALID(this);

	// If this function is called, m_pConversionData should point to valid data
	// If you do not want to use m_pConversionData, you should override this function
	// in a derived class
	ASSERT(m_pConversionData != NULL);

	return m_pConversionData->m_ppszUnitNames;
}

COXConversionParams* COXPhysicalEdit::GetConversionParams()
{
	ASSERT_VALID(this);

	// If this function is called, m_pConversionData should point to valid data
	// If you do not want to use m_pConversionData, you should override this function
	// in a derived class
	ASSERT(m_pConversionData != NULL);

	return m_pConversionData->m_pConversionParams;
}

void COXPhysicalEdit::ShowErrorMessage(LPCTSTR pszUnit)
{
	SetFocus();
	CString sMessage;
	CString sFullMessage;

	// If this function is called, m_pConversionData should point to valid data
	// If you do not want to use m_pConversionData, you should override this function
	// in a derived class
	ASSERT(m_pConversionData != NULL);

	if ((m_pConversionData != NULL) && (m_pConversionData->m_pszErrorMsg != NULL))
	{
		if (HIWORD(m_pConversionData->m_pszErrorMsg) == NULL)
		{
			// Implicit resource ID specified, load it
			UINT nID = LOWORD((DWORD_PTR)m_pConversionData->m_pszErrorMsg);
			sMessage.LoadString(LOWORD((DWORD)nID));
		}
		else
			// Explicit string pointer
			sMessage = m_pConversionData->m_pszErrorMsg;
	}
	// ... Fill in the value if the string contains "%1"
	AfxFormatStrings(sFullMessage, (LPCTSTR)sMessage, &pszUnit, 1);

	// Show a message box to the user
	AfxMessageBox(sFullMessage, MB_ICONEXCLAMATION | MB_OK);
}


void COXPhysicalEdit::OnChangeUnit(UINT nID)
{
	if((int)nID<ID_OXPHYSICALEDIT_INDEXBASE || 
		(int)nID>=ID_OXPHYSICALEDIT_INDEXBASE+GetNumberOfUnits())
	{
		// don't handle it
		return;
	}

	nID-=ID_OXPHYSICALEDIT_INDEXBASE;
	SetValue(GetValue(nID),nID);
	UpdateInsertionPointForward(0);
}


#ifdef _DEBUG
void COXPhysicalEdit::AssertValid() const
{
	CEdit::AssertValid();
	CString sDummy;

	// ... A valid unit index has to be specified !
	ASSERT(0 <= m_nUnitIndex);

	// If conversion data is supplied, check that it is valid
	if(m_pConversionData != NULL)
	{
		ASSERT(AfxIsValidAddress(m_pConversionData, sizeof(COXConversionData)));
		// ... Should have at least one unit
		ASSERT(0 < m_pConversionData->m_nNumberOfUnits);
		// ... Default unit index has to be valid
		ASSERT(0 <= m_pConversionData->m_nDefaultUnitIndex);
		if(m_pConversionData->m_pszErrorMsg != NULL)
		{
			if (HIWORD(m_pConversionData->m_pszErrorMsg)!=NULL)
			{
				// Explicit string pointer
				ASSERT(AfxIsValidString(m_pConversionData->m_pszErrorMsg));
			}
		}

		// Array should be valid
		ASSERT(AfxIsValidAddress(
			m_pConversionData->m_ppszUnitNames, 
			sizeof(LPCTSTR*) * m_pConversionData->m_nNumberOfUnits));
		ASSERT(AfxIsValidAddress(
			m_pConversionData->m_pConversionParams, 
			sizeof(COXConversionParams*) * m_pConversionData->m_nNumberOfUnits));
		for(int nUnitIndex = 0; 
			nUnitIndex < m_pConversionData->m_nNumberOfUnits; nUnitIndex++)
		{
			ASSERT(AfxIsValidString(m_pConversionData->m_ppszUnitNames[nUnitIndex]));
			ASSERT(AfxIsValidAddress(
				&m_pConversionData->m_pConversionParams[nUnitIndex], 
				sizeof(COXConversionParams)));
			// ... Multiplication factor should never be 0
			ASSERT(m_pConversionData->m_pConversionParams[nUnitIndex].m_dFactor != 0);
		}
	}
}

void COXPhysicalEdit::Dump(CDumpContext& dc) const
{
	COXNumericEdit::Dump(dc);
}
#endif //_DEBUG

COXPhysicalEdit::~COXPhysicalEdit()
{
	ASSERT_VALID(this);
}

// protected:
// private:

// global:
void AFXAPI DDX_OXPhysicalEdit(CDataExchange* pDX, int nIDC, double& dValue, int nUnits)
{
	// be sure that the control is a COXTimeEdit
	COXPhysicalEdit* pEdit = (COXPhysicalEdit*)pDX->m_pDlgWnd->GetDlgItem(nIDC);
	ASSERT(pEdit->IsKindOf(RUNTIME_CLASS(COXPhysicalEdit)));
	ASSERT_VALID(pEdit);
	if (pDX->m_bSaveAndValidate)
	{
		// retreive data
		dValue = pEdit->GetValue(nUnits);
	}
	else
	{
		// set data
		pEdit->SetValue(dValue, nUnits);
	}
}

// ==========================================================================

