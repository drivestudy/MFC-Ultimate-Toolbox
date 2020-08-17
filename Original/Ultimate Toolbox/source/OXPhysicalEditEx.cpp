// ==========================================================================
// 			Class Implementation : COXTimeEdit, COXAngleEdit,
//									COXLengthEdit, COXTempEdit
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
#include "OXPhysicalEditEx.h"
#include "OXMainRes.h"	// resource ID's

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COXLengthEdit static variables

LPCTSTR	COXLengthEdit::m_rgpszLengthUnitNames[] =
{
	_T("km"),			// OX_LENGTH_KILOMETER	
	_T("m"),			// OX_LENGTH_METER		
	_T("dm"),			// OX_LENGTH_DECIMETER	
	_T("cm"),			// OX_LENGTH_CENTIMETER
	_T("mm"),			// OX_LENGTH_MILLIMETER
	_T("µm"),			// OX_LENGTH_MICROMETER
	_T("nm"),			// OX_LENGTH_NANOMETER	
	_T("Å"),			// OX_LENGTH_ANGSTROM	
	_T("\""),			// OX_LENGTH_INCH		
	_T("pt"),			// OX_LENGTH_POINT		
};

COXConversionParams COXLengthEdit::m_rgnLengthConversionParams[] =
{
		{0,		1e3,				0},		// OX_LENGTH_KILOMETER	(1 km = 1e3 m)
		{0,		1,					0},		// OX_LENGTH_METER		(1 m  = 1 m)
		{0,		1e-1,				0},		// OX_LENGTH_DECIMETER	(1 dm = 1e-1 m)
		{0,		1e-2,				0},		// OX_LENGTH_CENTIMETER	(1 cm = 1e-2 m)
		{0,		1e-3,				0},		// OX_LENGTH_MILLIMETER	(1 mm = 1e-3 m)
		{0,		1e-6,				0},		// OX_LENGTH_MICROMETER	(1 µm = 1e-6 m)
		{0,		1e-9,				0},		// OX_LENGTH_NANOMETER	(1 nm = 1e-9 m)
		{0,		1e-10,				0},		// OX_LENGTH_ANGSTROM	(1 Å  = 1e-10 m)
		{0,		2.54e-2,			0},		// OX_LENGTH_INCH		(1 "  = 2.54 * 1e-2 m)
		{0,		3.527777777778e-4,	0},		// OX_LENGTH_POINT		(1 pt = 2.54 / 72 * 1e-2 m)
};

COXConversionData COXLengthEdit::m_lengthConversionData =
{
	OX_LENGTH_LAST + 1,						// m_nNumberOfUnits
	OX_LENGTH_METER,						// m_nDefaultUnitIndex
	m_rgpszLengthUnitNames,					// m_ppszUnitNames
	m_rgnLengthConversionParams,			// m_pConversionParams
	MAKEINTRESOURCE(IDS_OX_BAD_LENGTH_UNIT)	// m_pszErrorMsg;
};

/////////////////////////////////////////////////////////////////////////////
// COXLengthEdit

IMPLEMENT_DYNCREATE(COXLengthEdit, COXPhysicalEdit)

BEGIN_MESSAGE_MAP(COXLengthEdit, COXPhysicalEdit)
	//{{AFX_MSG_MAP(COXLengthEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

COXLengthEdit::COXLengthEdit(int nDefaultUnitIndex/*=OX_LENGTH_METER*/)	:
	COXPhysicalEdit(&m_lengthConversionData,nDefaultUnitIndex)
{
}

void COXLengthEdit::SetLength(double dValue, int nUnit/*=-1*/, 
							  LPCTSTR pszFormat/*=NULL*/)
{
	COXPhysicalEdit::SetValue(dValue,nUnit,pszFormat);
}

double COXLengthEdit::GetLength(int nUnit/*=-1*/, BOOL bNotify/*=TRUE*/)
{
	return COXPhysicalEdit::GetValue(nUnit,bNotify);
}

double COXLengthEdit::ConversionHelper(double dValue, int nUnitFrom, int nUnitTo)
{
	return COXPhysicalEdit::ConversionHelper(
		dValue,nUnitFrom,nUnitTo,m_lengthConversionData.m_pConversionParams);
}

BOOL COXLengthEdit::SetInputData(LPCTSTR pszInputData, int /*nBeginPos=0*/)
{
	if (pszInputData == NULL || ::_tcslen(pszInputData) == 0)
		return TRUE;

	// Interpret the input data
	double dLength;
	TCHAR szUnitName[255];
	CString strInputData = pszInputData;
	strInputData.Remove(',');
#if _MSC_VER >= 1400
	::_stscanf_s(strInputData, _T("%lf %s"), &dLength, szUnitName, 255);
#else
	::_stscanf(strInputData, _T("%lf %s"), &dLength, szUnitName);
#endif
	CString strUnitName(szUnitName);

	// Resolve the unit name
	int iUnit = 0;
	for (int i = 0; i < m_lengthConversionData.m_nNumberOfUnits; i++)
		if (m_rgpszLengthUnitNames[i] == strUnitName)
		{
			iUnit = i;
			break;
		}

	SetLength(dLength, iUnit);
	return TRUE;
}

COXLengthEdit::~COXLengthEdit()
{
}

/////////////////////////////////////////////////////////////////////////////
// COXTempEdit static variables

LPCTSTR	COXTempEdit::m_rgpszTempUnitNames[] =
{
	_T("K"),			// OX_TEMP_KELVIN	
	_T("°C"),			// OX_TEMP_CELSIUS		
	_T("°F"),			// OX_TEMP_FAHRENHEIT	
};

COXConversionParams COXTempEdit::m_rgnTempConversionParams[] =
{
	{0,		1,			  -273.15},		// OX_TEMP_KELVIN		(K - 273.15 = °C)
	{0,		1,					0},		// OX_TEMP_CELSIUS		(°C = °C)
	{-32,	0.5555555555556,	0},		// OX_TEMP_FAHRENHEIT	((°F - 32) * 5/9 = °C)
};

COXConversionData COXTempEdit::m_tempConversionData =
{
	OX_TEMP_LAST + 1,					// m_nNumberOfUnits
	OX_TEMP_CELSIUS,					// m_nDefaultUnitIndex
	m_rgpszTempUnitNames,				// m_ppszUnitNames
	m_rgnTempConversionParams,			// m_pConversionParams
	MAKEINTRESOURCE(IDS_OX_BAD_TEMP_UNIT)	// m_pszErrorMsg;
};

/////////////////////////////////////////////////////////////////////////////
// COXTempEdit

IMPLEMENT_DYNCREATE(COXTempEdit, COXPhysicalEdit)

BEGIN_MESSAGE_MAP(COXTempEdit, COXPhysicalEdit)
	//{{AFX_MSG_MAP(COXTempEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

COXTempEdit::COXTempEdit(int nDefaultUnitIndex/*=OX_TEMP_CELSIUS*/)	:
	COXPhysicalEdit(&m_tempConversionData, nDefaultUnitIndex)
{
}

void COXTempEdit::SetTemp(double dValue, int nUnit/*=-1*/, 
						  LPCTSTR pszFormat/*=NULL*/)
{
	COXPhysicalEdit::SetValue(dValue,nUnit,pszFormat);
}

double COXTempEdit::GetTemp(int nUnit/*=-1*/, BOOL bNotify/*=TRUE*/)
{
	return COXPhysicalEdit::GetValue(nUnit,bNotify);
}

double COXTempEdit::ConversionHelper(double dValue, int nUnitFrom, int nUnitTo)
{
	return COXPhysicalEdit::ConversionHelper(
		dValue,nUnitFrom,nUnitTo,m_tempConversionData.m_pConversionParams);
}

BOOL COXTempEdit::SetInputData(LPCTSTR pszInputData, int /*nBeginPos=0*/)
{
	if (pszInputData == NULL || ::_tcslen(pszInputData) == 0)
		return TRUE;

	// Interpret the input data
	double dTemperature;
	TCHAR szUnitName[255];
	CString strInputData = pszInputData;
	strInputData.Remove(',');
#if _MSC_VER >= 1400
	::_stscanf_s(strInputData, _T("%lf %s"), &dTemperature, szUnitName, 255);
#else
	::_stscanf(strInputData, _T("%lf %s"), &dTemperature, szUnitName);
#endif
	CString strUnitName(szUnitName);

	// Resolve the unit name
	int iUnit = 0;
	for (int i = 0; i < m_tempConversionData.m_nNumberOfUnits; i++)
		if (m_rgpszTempUnitNames[i] == strUnitName)
		{
			iUnit = i;
			break;
		}

	SetTemp(dTemperature, iUnit);
	return TRUE;
}

COXTempEdit::~COXTempEdit()
{
}


/////////////////////////////////////////////////////////////////////////////
// COXTimeEdit static variables

LPCTSTR	COXTimeEdit::m_rgpszTimeUnitNames[] =
{
	_T("s"), 
	_T("min"), 
	_T("h"), 
	_T("d"),
	_T("ms"), 
	_T("µs"), 
	_T("ns"), 
	_T("sec"), 
};

COXConversionParams COXTimeEdit::m_rgnTimeConversionParams[] =
{
	{0,		1,				0},		// OX_TIME_SECOND		(1 s = 1 s)
	{0,		60,				0},		// OX_TIME_MINUTE		(1 min = 60 s)
	{0,		3600,			0},		// OX_TIME_HOUR			(1 d = 60 * 60 s)
	{0,		86400,			0},		// OX_TIME_DAY			(1 d = 24 * 60 * 60 s)
	{0,		1e-3,			0},		// OX_TIME_MILLISECOND	(1 ms = 1e-3 s)
	{0,		1e-6,			0},		// OX_TIME_MICROSECOND	(1 µs = 1e-6 s)
	{0,		1e-9,			0},		// OX_TIME_NANOSECOND	(1 ns = 1e-9 s)
	{0,		1,				0},		// OX_TIME_SECOND_2		(1 sec = 1 s)
};

COXConversionData COXTimeEdit::m_timeConversionData =
{
	OX_TIME_LAST + 1,					// m_nNumberOfUnits
	OX_TIME_SECOND,						// m_nDefaultUnitIndex
	m_rgpszTimeUnitNames,				// m_ppszUnitNames
	m_rgnTimeConversionParams,			// m_pConversionParams
	MAKEINTRESOURCE(IDS_OX_BAD_TIME_UNIT)	// m_pszErrorMsg;
};

/////////////////////////////////////////////////////////////////////////////
// COXTempEdit

IMPLEMENT_DYNCREATE(COXTimeEdit, COXPhysicalEdit)

BEGIN_MESSAGE_MAP(COXTimeEdit, COXPhysicalEdit)
	//{{AFX_MSG_MAP(COXTimeEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

COXTimeEdit::COXTimeEdit(int nDefaultUnitIndex/*=OX_TIME_SECOND*/) :
	COXPhysicalEdit(&m_timeConversionData, nDefaultUnitIndex)
{
}

void COXTimeEdit::SetDuration(double dValue, int nUnit/*=-1*/, LPCTSTR pszFormat/*=NULL*/)
{
	COXPhysicalEdit::SetValue(dValue,nUnit,pszFormat);
}

double COXTimeEdit::GetDuration(int nUnit/*=-1*/, BOOL bNotify/*=TRUE*/)
{
	return COXPhysicalEdit::GetValue(nUnit,bNotify);
}

double COXTimeEdit::ConversionHelper(double dValue, int nUnitFrom, int nUnitTo)
{
	return COXPhysicalEdit::ConversionHelper(
		dValue,nUnitFrom,nUnitTo,m_timeConversionData.m_pConversionParams);
}

BOOL COXTimeEdit::SetInputData(LPCTSTR pszInputData, int /*nBeginPos=0*/)
{
	if (pszInputData == NULL || ::_tcslen(pszInputData) == 0)
		return TRUE;

	// Interpret the input data
	double dTime;
	TCHAR szUnitName[255];
	CString strInputData = pszInputData;
	strInputData.Remove(',');
#if _MSC_VER >= 1400
	::_stscanf_s(strInputData, _T("%lf %s"), &dTime, szUnitName, 255);
#else
	::_stscanf(strInputData, _T("%lf %s"), &dTime, szUnitName);
#endif
	CString strUnitName(szUnitName);

	// Resolve the unit name
	int iUnit = 0;
	for (int i = 0; i < m_timeConversionData.m_nNumberOfUnits; i++)
		if (m_rgpszTimeUnitNames[i] == strUnitName)
		{
			iUnit = i;
			break;
		}

	SetDuration(dTime, iUnit);
	return TRUE;
}

COXTimeEdit::~COXTimeEdit()
{
}


/////////////////////////////////////////////////////////////////////////////
// COXAngleEdit static variables

LPCTSTR	COXAngleEdit::m_rgpszAngleUnitNames[] =
{
	_T("°"), 
	_T("rd"), 
	_T("grad"), 
	_T("deg"),
	_T("rad"), 
};

COXConversionParams COXAngleEdit::m_rgnAngleConversionParams[] =
{
	{0,		1,				0},		// OX_ANGLE_DEGREE		(1 * = 1 °
	{0,		57.29577951308, 0},		// OX_ANGLE_RADIAN		(1 rd = 180/pi °)
	{0,		0.9,			0},		// OX_ANGLE_GRAD		(1 grad = 9/10 °)
	{0,		1,				0},		// OX_ANGLE_DEGREE_2	(1 deg = 1 °
	{0,		57.29577951308,	0},		// OX_ANGLE_RADIAN_2	(1 rad = 180/pi °)
};

COXConversionData COXAngleEdit::m_angleConversionData =
{
	OX_ANGLE_LAST + 1,					// m_nNumberOfUnits
	OX_ANGLE_DEGREE,					// m_nDefaultUnitIndex
	m_rgpszAngleUnitNames,				// m_ppszUnitNames
	m_rgnAngleConversionParams,			// m_pConversionParams
	MAKEINTRESOURCE(IDS_OX_BAD_ANGLE_UNIT) // m_pszErrorMsg;
};

/////////////////////////////////////////////////////////////////////////////
// COXTempEdit

IMPLEMENT_DYNCREATE(COXAngleEdit, COXPhysicalEdit)

BEGIN_MESSAGE_MAP(COXAngleEdit, COXPhysicalEdit)
	//{{AFX_MSG_MAP(COXAngleEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

COXAngleEdit::COXAngleEdit(int nDefaultUnitIndex/*=OX_ANGLE_DEGREE*/) :
	COXPhysicalEdit(&m_angleConversionData, nDefaultUnitIndex)
{
}

BOOL COXAngleEdit::SetInputData(LPCTSTR pszInputData, int /*nBeginPos=0*/)
{
	if (pszInputData == NULL || ::_tcslen(pszInputData) == 0)
		return TRUE;

	// Interpret the input data
	double dAngle;
	TCHAR szUnitName[255];
	CString strInputData = pszInputData;
	strInputData.Remove(',');
#if _MSC_VER >= 1400
	::_stscanf_s(strInputData, _T("%lf %s"), &dAngle, szUnitName, 255);
#else
	::_stscanf(strInputData, _T("%lf %s"), &dAngle, szUnitName);
#endif
	CString strUnitName(szUnitName);

	// Resolve the unit name
	int iUnit = 0;
	for (int i = 0; i < m_angleConversionData.m_nNumberOfUnits; i++)
		if (m_rgpszAngleUnitNames[i] == strUnitName)
		{
			iUnit = i;
			break;
		}

	SetAngle(dAngle, iUnit);
	return TRUE;
}

void COXAngleEdit::SetAngle(double dValue, int nUnit/*=-1*/, 
							LPCTSTR pszFormat/*=NULL*/)
{
	COXPhysicalEdit::SetValue(dValue,nUnit,pszFormat);
}

double COXAngleEdit::GetAngle(int nUnit/*=-1*/, BOOL bNotify/*=TRUE*/)
{
	return COXPhysicalEdit::GetValue(nUnit,bNotify);
}

double COXAngleEdit::ConversionHelper(double dValue, int nUnitFrom, int nUnitTo)
{
	return COXPhysicalEdit::ConversionHelper(
		dValue,nUnitFrom,nUnitTo,m_angleConversionData.m_pConversionParams);
}

COXAngleEdit::~COXAngleEdit()
{
}

// global:
void AFXAPI DDX_OXLengthEdit(CDataExchange* pDX, int nIDC, double& dValue, int nUnits)
{	
#ifdef _DEBUG
	COXLengthEdit* pEdit = (COXLengthEdit*)pDX->m_pDlgWnd->GetDlgItem(nIDC);
	ASSERT(pEdit->IsKindOf(RUNTIME_CLASS(COXLengthEdit)));
	ASSERT_VALID(pEdit);
#endif

	// ... Call base class imlementation
	DDX_OXPhysicalEdit(pDX,nIDC,dValue,nUnits);
}

void AFXAPI DDX_OXTimeEdit(CDataExchange* pDX, int nIDC, double& dValue, int nUnits)
{	
#ifdef _DEBUG
	COXTimeEdit* pEdit = (COXTimeEdit*)pDX->m_pDlgWnd->GetDlgItem(nIDC);
	ASSERT(pEdit->IsKindOf(RUNTIME_CLASS(COXTimeEdit)));
	ASSERT_VALID(pEdit);
#endif

	// ... Call base class imlementation
	DDX_OXPhysicalEdit(pDX,nIDC,dValue,nUnits);
}

void AFXAPI DDX_OXTempEdit(CDataExchange* pDX, int nIDC, double& dValue, int nUnits)
{	
#ifdef _DEBUG
	COXTempEdit* pEdit = (COXTempEdit*)pDX->m_pDlgWnd->GetDlgItem(nIDC);
	ASSERT(pEdit->IsKindOf(RUNTIME_CLASS(COXTempEdit)));
	ASSERT_VALID(pEdit);
#endif

	// ... Call base class imlementation
	DDX_OXPhysicalEdit(pDX,nIDC,dValue,nUnits);
}

void AFXAPI DDX_OXAngleEdit(CDataExchange* pDX, int nIDC, double& dValue, int nUnits)
{	
#ifdef _DEBUG
	COXAngleEdit* pEdit = (COXAngleEdit*)pDX->m_pDlgWnd->GetDlgItem(nIDC);
	ASSERT(pEdit->IsKindOf(RUNTIME_CLASS(COXAngleEdit)));
	ASSERT_VALID(pEdit);
#endif

	// ... Call base class imlementation
	DDX_OXPhysicalEdit(pDX,nIDC,dValue,nUnits);
}

// ==========================================================================
