// ==========================================================================
// 				Class Specification : COXTimeEdit, COXAngleEdit,
//									  COXLengthEdit, COXTempEdit
// ==========================================================================

// Header file : OXPhysicalEditEx.h

// Version: 9.3

// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.

// //////////////////////////////////////////////////////////////////////////

// Properties:	( all four classes have the same properties)
//	NO	Abstract class (does not have any objects)
//	YES	Derived from COXPhysicalEdit

//	YES	Is a Cwnd.                     
//	YES	Two stage creation (constructor & Create())
//	YES	Has a message map
//	YES	Needs a resource (template)

//	NO	Persistent objects (saveable on disk)      
//	YES	Uses exceptions (CUserException)

// //////////////////////////////////////////////////////////////////////////

// Desciption :         
//	These classes encapsulate edit controls for editing of physical values
//	such as time (COXTimeEdit), angle (COXAngleEdit), length (COXLengthEdit), and
//	temperature (COXTempEdit).
//	Contents of these controls consist of a numerical value and an optional
//	character string describing a physical unit
//	(e.g.: "12 cm", where "12" is the numerical value and "cm" the character
//	string for the unit)
//	member functions allow:
//		setting a value with a specified unit
//		retrieving a value in a specified unit
//		converting values between different units (static public functions)

// Remark:

// Prerequisites (necessary conditions):
// This control needs string resource with the following IDs
//		IDS_BAD_TEMPERATURE_UNIT
//		IDS_BAD_ANGLE_UNIT
//		IDS_BAD_LENGTH_UNIT
//		IDS_BAD_TIME_UNIT

/////////////////////////////////////////////////////////////////////////////

#ifndef __OXPHYSICALEDITEX_H__
#define __OXPHYSICALEDITEX_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#include "OXPhysicalEdit.h"


/////////////////////////////////////////////////////////////////////////////
// COXLengthEdit window

#define OX_LENGTH_KILOMETER		(0)			// km	(= 1e3 m)
#define OX_LENGTH_METER			(1)			// m
#define OX_LENGTH_DECIMETER		(2)			// dm	(= 1e-1 m)
#define OX_LENGTH_CENTIMETER	(3)			// cm	(= 1e-2 m)
#define OX_LENGTH_MILLIMETER	(4)			// mm	(= 1e-3 m)
#define OX_LENGTH_MICROMETER	(5)			// µm	(= 1e-6 m)
#define OX_LENGTH_NANOMETER		(6)			// nm	(= 1e-9 m)
#define OX_LENGTH_ANGSTROM		(7)			// Å	(= 1e-10 m)
#define OX_LENGTH_INCH			(8)			// "	(= 2.54 * 1e-2 m)
#define OX_LENGTH_POINT			(9)			// pt	(= 2.54 / 72 * 1e-2 m)

#define OX_LENGTH_FIRST			OX_LENGTH_KILOMETER
#define OX_LENGTH_LAST			OX_LENGTH_POINT

class OX_CLASS_DECL COXLengthEdit : public COXPhysicalEdit
{
DECLARE_DYNCREATE(COXLengthEdit)

// Data members -------------------------------------------------------------
public:
protected:
	static COXConversionData	m_lengthConversionData;
	static LPCTSTR				m_rgpszLengthUnitNames[];
	static COXConversionParams	m_rgnLengthConversionParams[];

private:

// Member functions ---------------------------------------------------------
public:
	COXLengthEdit(int nDefaultUnitIndex = OX_LENGTH_METER);
	// --- In : nDefaultUnitIndex : The index of the unit that should be used as default
	// --- Out : 
	// --- Returns :
	// --- Effect : Standard contructor

	void SetLength(double dValue, int nUnit = -1, LPCTSTR pszFormat = NULL);
	// --- In  : dValue : The length value to be set
	//			 nUnit : The unit of dValue (one of the predefined units)
	//			 pszFormat : A "printf" like format string (if NULL "%g" will be used)
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets a new length of the edit control

	double GetLength(int nUnit = -1, BOOL bNotify = TRUE);
	// --- In  : nUnits : the unit in which the value should be retreived
	//			 bNotify : Messagebox is shown in case of error
	// --- Out : the value in the edit control
	// --- Returns :
	// --- Effect : Retreives the length from the edit control
	//				This function throws a CUserException when the control contains
	//				an unknown unit

	static double ConversionHelper(double dValue, int nUnitFrom, int nUnitTo);
	// --- In  : dValue : The value to be converted
	//			 nUnitFrom : The unit of dValue on input (one of the predefined units)
	//			 nUnitTo : The unit of dValue on output (one of the predefined units)
	// --- Out : 
	// --- Returns : The value of the edit control
	// --- Effect : Static function that converts a value bewteen different units

	virtual BOOL SetInputData(LPCTSTR pszInputData, int nBeginPos=0);
	// --- In     : pszInputData -	Each character is entered into the control as 
	//								if the user typed it in. 
	//            : nBeginPos    -	Begin position for inserting or overwriting the 
	//								input data depending on current state of 
	//								insert mode. 
	// --- Out    :
	// --- Returns: TRUE if some data was inserted. 
	// --- Effect : Use to programmatically insert pszInputData into the edit control. 
	//            : The mask is applied to the input data.

	virtual ~COXLengthEdit();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Standard destructor

	//{{AFX_VIRTUAL(COXLengthEdit)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(COXLengthEdit)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

};

/////////////////////////////////////////////////////////////////////////////
// COXTempEdit window

#define OX_TEMP_KELVIN			(0)			// K	(K - 273.15 = °C)
#define OX_TEMP_CELSIUS			(1)			// °C   (°C = °C)
#define OX_TEMP_FAHRENHEIT		(2)			// °F	((°F - 32) * 5/9 = °C)

#define OX_TEMP_FIRST			OX_TEMP_KELVIN
#define OX_TEMP_LAST			OX_TEMP_FAHRENHEIT

class OX_CLASS_DECL COXTempEdit : public COXPhysicalEdit
{
DECLARE_DYNCREATE(COXTempEdit)

// Data members -------------------------------------------------------------
public:
protected:
	static COXConversionData	m_tempConversionData;
	static LPCTSTR				m_rgpszTempUnitNames[];
	static COXConversionParams	m_rgnTempConversionParams[];

private:

// Member functions ---------------------------------------------------------
public:
	COXTempEdit(int nDefaultUnitIndex = OX_TEMP_CELSIUS);
	// --- In : nDefaultUnitIndex : The index of the unit that should be used as default
	// --- Out : 
	// --- Returns :
	// --- Effect : Standard contructor

	void SetTemp(double dValue, int nUnit = -1, LPCTSTR pszFormat = NULL);
	// --- In  : dValue : The temp value to be set
	//			 nUnit : The unit of dValue (one of the predefined units)
	//			 pszFormat : A "printf" like format string (if NULL "%g" will be used)
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets a new temperature of the edit control

	double GetTemp(int nUnit = -1, BOOL bNotify = TRUE);
	// --- In  : nUnits : the unit in which the value should be retreived
	//			 bNotify : Messagebox is shown in case of error
	// --- Out : the value in the edit control
	// --- Returns :
	// --- Effect : Retreives the tempearture from the edit control
	//				This function throws a CUserException when the control contains
	//				an unknown unit

	static double ConversionHelper(double dValue, int nUnitFrom, int nUnitTo);
	// --- In  : dValue : The value to be converted
	//			 nUnitFrom : The unit of dValue on input (one of the predefined units)
	//			 nUnitTo : The unit of dValue on output (one of the predefined units)
	// --- Out : 
	// --- Returns : The value of the edit control
	// --- Effect : Static function that converts a value bewteen different units

	virtual BOOL SetInputData(LPCTSTR pszInputData, int nBeginPos=0);
	// --- In     : pszInputData -	Each character is entered into the control as 
	//								if the user typed it in. 
	//            : nBeginPos    -	Begin position for inserting or overwriting the 
	//								input data depending on current state of 
	//								insert mode. 

	virtual ~COXTempEdit();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Standard destructor

	//{{AFX_VIRTUAL(COXTempEdit)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(COXTempEdit)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
};

/////////////////////////////////////////////////////////////////////////////
// COXTempEdit window

#define OX_TIME_SECOND			(0)			// s	(= 1 s)
#define OX_TIME_MINUTE			(1)			// min  (= 60 s)
#define OX_TIME_HOUR			(2)			// h	(= 60 * 60 s)
#define OX_TIME_DAY				(3)			// d	(= 24 * 60 * 60 s)
#define OX_TIME_MILLISECOND		(4)			// ms	(= 1e-3 s)
#define OX_TIME_MICROSECOND		(5)			// µs	(= 1e-6 s)
#define OX_TIME_NANOSECOND		(6)			// ns	(= 1e-9 s)
#define OX_TIME_SECOND_2		(7)			// sec	(= 1 s)

#define OX_TIME_FIRST			OX_TIME_SECOND
#define OX_TIME_LAST			OX_TIME_SECOND_2


class OX_CLASS_DECL COXTimeEdit : public COXPhysicalEdit
{
DECLARE_DYNCREATE(COXTimeEdit)

// Data members -------------------------------------------------------------
public:

protected:
	static COXConversionData	m_timeConversionData;
	static LPCTSTR				m_rgpszTimeUnitNames[];
	static COXConversionParams	m_rgnTimeConversionParams[];

private:

// Member functions ---------------------------------------------------------
public:
	COXTimeEdit(int nDefaultUnitIndex = OX_TIME_SECOND);
	// --- In : nDefaultUnitIndex : The index of the unit that should be used as default
	// --- Out : 
	// --- Returns :
	// --- Effect : Standard contructor

	void SetDuration(double dValue, int nUnit = -1, LPCTSTR pszFormat = NULL);
	// --- In  : dValue : The duration value to be set
	//			 nUnit : The unit of dValue (one of the predefined units)
	//			 pszFormat : A "printf" like format string (if NULL "%g" will be used)
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets a new duration of the edit control

	double GetDuration(int nUnit = -1, BOOL bNotify = TRUE);
	// --- In  : nUnits : the unit in which the value should be retreived
	//			 bNotify : Messagebox is shown in case of error
	// --- Out : the value in the edit control
	// --- Returns :
	// --- Effect : Retreives the duration from the edit control
	//				This function throws a CUserException when the control contains
	//				an unknown unit

	static double ConversionHelper(double dValue, int nUnitFrom, int nUnitTo);
	// --- In  : dValue : The value to be converted
	//			 nUnitFrom : The unit of dValue on input (one of the predefined units)
	//			 nUnitTo : The unit of dValue on output (one of the predefined units)
	// --- Out : 
	// --- Returns : The value of the edit control
	// --- Effect : Static function that converts a value bewteen different units

	virtual BOOL SetInputData(LPCTSTR pszInputData, int nBeginPos=0);
	// --- In     : pszInputData -	Each character is entered into the control as 
	//								if the user typed it in. 
	//            : nBeginPos    -	Begin position for inserting or overwriting the 
	//								input data depending on current state of 
	//								insert mode. 

	virtual ~COXTimeEdit();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Standard destructor

	//{{AFX_VIRTUAL(COXTimeEdit)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(COXTimeEdit)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// COXAngleEdit window

#define OX_ANGLE_DEGREE			(0)			// °	(= 1 °)
#define OX_ANGLE_RADIAN			(1)			// rd   (= 180/pi °)
#define OX_ANGLE_GRAD			(2)			// grad	(= 9/10 °)
#define OX_ANGLE_DEGREE_2		(3)			// deg	(= 1 °)
#define OX_ANGLE_RADIAN_2		(4)			// rad  (= 180/pi °)

#define OX_ANGLE_FIRST			OX_ANGLE_DEGREE
#define OX_ANGLE_LAST			OX_ANGLE_RADIAN_2


class OX_CLASS_DECL COXAngleEdit : public COXPhysicalEdit
{
DECLARE_DYNCREATE(COXAngleEdit)

// Data members -------------------------------------------------------------
public:

protected:
	static COXConversionData	m_angleConversionData;
	static LPCTSTR				m_rgpszAngleUnitNames[];
	static COXConversionParams	m_rgnAngleConversionParams[];

private:

// Member functions ---------------------------------------------------------
public:
	COXAngleEdit(int nDefaultUnitIndex = OX_ANGLE_DEGREE);
	// --- In : nDefaultUnitIndex : The index of the unit that should be used as default
	// --- Out : 
	// --- Returns :
	// --- Effect : Standard contructor

	void SetAngle(double dValue, int nUnit = -1, LPCTSTR pszFormat = NULL);
	// --- In  : dValue : The angle value to be set
	//			 nUnit : The unit of dValue (one of the predefined units)
	//			 pszFormat : A "printf" like format string (if NULL "%g" will be used)
	// --- Out : 
	// --- Returns :
	// --- Effect : Sets a new angle of the edit control

	double GetAngle(int nUnit = -1, BOOL bNotify = TRUE);
	// --- In  : nUnits : the unit in which the value should be retreived
	//			 bNotify : Messagebox is shown in case of error
	// --- Out : the value in the edit control
	// --- Returns :
	// --- Effect : Retreives the angle from the edit control
	//				This function throws a CUserException when the control contains
	//				an unknown unit

	static double ConversionHelper(double dValue, int nUnitFrom, int nUnitTo);
	// --- In  : dValue : The value to be converted
	//			 nUnitFrom : The unit of dValue on input (one of the predefined units)
	//			 nUnitTo : The unit of dValue on output (one of the predefined units)
	// --- Out : 
	// --- Returns : The value of the edit control
	// --- Effect : Static function that converts a value bewteen different units
 
	virtual BOOL SetInputData(LPCTSTR pszInputData, int nBeginPos=0);
	// --- In     : pszInputData -	Each character is entered into the control as 
	//								if the user typed it in. 
	//            : nBeginPos    -	Begin position for inserting or overwriting the 
	//								input data depending on current state of 
	//								insert mode. 
	// --- Out    :
	// --- Returns: TRUE if some data was inserted. 
	// --- Effect : Use to programmatically insert pszInputData into the edit control. 
	//            : The mask is applied to the input data.

	virtual ~COXAngleEdit();
	// --- In  :
	// --- Out : 
	// --- Returns :
	// --- Effect : Standard destructor

	//{{AFX_VIRTUAL(COXAngleEdit)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(COXAngleEdit)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// global public functions

OX_API_DECL void AFXAPI DDX_OXLengthEdit(CDataExchange* pDX, int nIDC, 
										 double& dValue, int nUnitIndex);
OX_API_DECL void AFXAPI DDX_OXTimeEdit(CDataExchange* pDX, int nIDC, 
									   double& dValue, int nUnitIndex);
OX_API_DECL void AFXAPI DDX_OXTempEdit(CDataExchange* pDX, int nIDC, 
									   double& dValue, int nUnitIndex);
OX_API_DECL void AFXAPI DDX_OXAngleEdit(CDataExchange* pDX, int nIDC, 
										double& dValue, int nUnitIndex);

/////////////////////////////////////////////////////////////////////////////

#endif // __OXPHYSICALEDITEX_H__
