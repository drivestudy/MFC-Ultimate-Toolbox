/***************************************************
MFC DAO FrameWork Wrapper Class
Dundas Software Ltd. 1997 - 2003
****************************************************/
// Version: 9.3


#include "stdafx.h"

#include "OXDao.h"

#include "UTBStrOp.h"

// Comment out this define to eliminate message boxes in the
// DAO exception handlers - usefull for debugging...
// #define UG_DAO_REPORTERROR


/***************************************************
COXDao
	Purpose
		Initializes all member variables
	Params
		none
	Return 
		none
****************************************************/
COXDao::COXDao(){

	m_database			= NULL; 
	m_recordSet			= NULL;
	m_tableDef			= NULL;
	m_queryDef			= NULL;
	m_fields			= NULL;

	m_password			= "";

	m_lastErrorCode		= -1;
	

}

/***************************************************
~COXDao
	Purpose
		Closes all data objects
	Params
		none
	Return 
		none
****************************************************/
COXDao::~COXDao(){

	Close();
}

/***************************************************
SetErrorCode
	Purpose
		Used internally to set the last error code
		This code can be retrieved by GetLastError
		In debug mode this function posts TRACE
		statements of all reported errors
	Params
		code - error code number 0 - n
	Return 
		none
****************************************************/
void COXDao::SetErrorCode(int code){
	
	m_lastErrorCode = code;
	
	TRACE(COXDao_errorStrings[m_lastErrorCode]);
	TRACE(_T("\n"));
}

/***************************************************
GetLastError
	Purpose
		If a function fails call this to retrieve
		extended error information. Also see the
		funciton GetLastErrorText to retrieve an 
		associated text error message.
	Params
		none
	Return 
		none
****************************************************/
int COXDao::GetLastError(){

	return m_lastErrorCode;	
}

/***************************************************
GetLastErrorText
	Purpose
		Returns a constant string containing a message
		on the last error.
	Params
		none
	Return 
		none
****************************************************/
LPCTSTR COXDao::GetLastErrorText(){

	if(m_lastErrorCode < 0)
	{
		CString sError;
		VERIFY(sError.LoadString(IDS_OX_DAONOERROR));//"No Error"
		return sError;
	}

	return  COXDao_errorStrings[m_lastErrorCode];
}

/***************************************************
Open
	Purpose
		Opens the specified table in the specified
		database. This function opens the full table
		as a dynaset. To open using other modes please
		use the last version of the open function.
	Params
		database - name of the database to open (ex. c:\data.mdb)
		table - name of the table in the database to open
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::Open(LPCTSTR database,LPCTSTR table){
	
	return Open(database,table,dbOpenDynaset,NULL,0);
}

/***************************************************
Open
	Purpose
		Excutes the specified SQL query in the specified
		database. The result set is opened as a
		dynaset. To open using other modes please
		use the last version of the open function.
	Params
		database - name of the database to open (ex. c:\data.mdb)
		sql - query to run (ex. SELECT * from MyTable)
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::OpenSQL(LPCTSTR database,LPCTSTR sql){
	
	return Open(database,NULL,dbOpenDynaset,sql,0);
}

/***************************************************
Open
	Purpose
		Opens the specified table or SQL statement on
		the specified database. If the 'sql' parameter 
		is NULL then the 'table' param is used, otherwise
		the 'sql' param is used.

	Params
		database - name of the database to open (ex. c:\data.mdb)
		table - name of the table in the database to open
		openType - type of table to open, possible types are
			dbOpenDynaset,dbOpenTable ,dbOpenSnapshot
			(For more information see CDaoRecordset::Open)
		sql - query to run (ex. SELECT * from MyTable)
		options - specifies other opening options as listed below
			dbAppendOnly, dbForwardOnly, dbSeeChanges, dbDenyWrite,
			dbDenyRead, dbReadOnly, dbInconsistent, dbConsistent   
			(For more information see CDaoRecordset::Open)
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::Open(LPCTSTR database,LPCTSTR table,int openType,LPCTSTR sql,
				  int options/*=0*/,CString optStr/*=_T("")*/)
{
	//make sure that all database objects are closed
	Close();

	//check for a password
	if(m_password.GetLength()>0)
	{
		CString sPassword;
		sPassword.Format(_T(";PWD=%s"),m_password);
		optStr+=sPassword;
	}

	//create the CDaoDatabase object
	m_database = new CDaoDatabase(NULL);

	//try and open the database object, on failure set the error number
	try{
		m_database->Open(database,FALSE,FALSE,optStr);
	}
		catch(CMemoryException* e){
			#ifdef UG_DAO_REPORTERROR
				e->ReportError();
			#endif
		e->Delete();
		AfxAbort();
	}
	catch(CDaoException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		SetErrorCode(0);		
		return FALSE;
	}

	if(table == NULL && sql == NULL){
		SetErrorCode(1);		
		return FALSE;
	}

	//if the sql statement is NULL then create and open a CDaoTableDef
	//on failure set the error number
	if(sql == NULL){

		m_tableDef = new CDaoTableDef(m_database);

		try{
			m_tableDef->Open(table);
		}
		catch(CMemoryException* e){
			#ifdef UG_DAO_REPORTERROR
				e->ReportError();
			#endif
			e->Delete();
			AfxAbort();
		}
		catch(CDaoException* e){
			#ifdef UG_DAO_REPORTERROR
				e->ReportError();
			#endif
			e->Delete();
			SetErrorCode(2);
			return FALSE;
		}
		
		//create and open the CDaoRecordset object, on failure set the error number
		m_recordSet = new CDaoRecordset(m_database);

		try{
			m_recordSet->Open(m_tableDef,openType,options);
		}
		catch(CMemoryException* e){
			#ifdef UG_DAO_REPORTERROR
				e->ReportError();
			#endif
			e->Delete();
			AfxAbort();
		}
		catch(CDaoException* e){
			#ifdef UG_DAO_REPORTERROR
				e->ReportError();
			#endif
			e->Delete();
			SetErrorCode(3);
			return FALSE;
		}

	}
	//create and open the CDaoQueryDef object, on failure set the error number
	else{

		m_queryDef = new CDaoQueryDef(m_database);

		try{
			m_queryDef->Create(NULL,sql);
		}
		catch(CMemoryException* e){
			#ifdef UG_DAO_REPORTERROR
				e->ReportError();
			#endif
			e->Delete();
			AfxAbort();
		}
		catch(CDaoException* e){
			#ifdef UG_DAO_REPORTERROR
				e->ReportError();
			#endif
			e->Delete();
			SetErrorCode(4);
			return FALSE;
		}

		//create and open the CDaoRecordset object, on failure set the error number
		m_recordSet = new CDaoRecordset(m_database);
		
		try{
			if(openType == dbOpenTable)
				openType = dbOpenDynaset;

			m_recordSet->Open(m_queryDef,openType,options);
		}

		catch(CMemoryException* e){
			#ifdef UG_DAO_REPORTERROR
				e->ReportError();
			#endif
			e->Delete();
			AfxAbort();
		}
		catch(CDaoException* e){
			#ifdef UG_DAO_REPORTERROR
				e->ReportError();
			#endif
			e->Delete();
			SetErrorCode(5);
			return FALSE;
		}

	}
	//retrieve all of the field information
	GatherFieldInfo();

	return TRUE;
}

/***************************************************
GatherFieldInfo
	Purpose
		This internal function gathers the meta data
		for the fields once a recordset is opened.
		The information gathered can be viewed using 
		the	'GetFieldInfo' function
	Params
		none
	Return 
		none
****************************************************/
void COXDao::GatherFieldInfo(){

	ASSERT(m_recordSet);

	int loop;
	int fields = m_recordSet->GetFieldCount();

	//delete any previous field data
	if(m_fields != NULL)
		delete[] m_fields;
	
	//create an array of CDaoFieldInfo objects 
	m_fields = new CDaoFieldInfo[fields];
	
	//store each fields information
	for(loop=0;loop<fields;loop++)
		m_recordSet->GetFieldInfo(loop,m_fields[loop],AFX_DAO_PRIMARY_INFO );
}

/***************************************************
Close
	Purpose
		Closes all currently open data objects and
		prepares the class for reuse
	Params
		none
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::Close(){

	//close and delete all field objects
	if(m_fields != NULL){
		delete[] m_fields;
		m_fields = NULL;
	}
	//close and delete all recordset objects
	if(m_recordSet != NULL){
		m_recordSet->Close();
		delete m_recordSet;
		m_recordSet = NULL;
	}
	//close and delete all tabledef objects
	if(m_tableDef != NULL){
		m_tableDef->Close();
		delete m_tableDef;
		m_tableDef = NULL;
	}
	//close and delete all querydef objects
	if(m_queryDef != NULL){
		m_queryDef->Close();
		delete m_queryDef;
		m_queryDef = NULL;
	}
	//close and delete all database objects
	if(m_database != NULL){
		if(m_database->IsOpen())
			m_database->Close();
		delete m_database;
		m_database = NULL;
	}
	
	return TRUE;
}

/***************************************************
GetField
	Purpose
		Retrieves the specified field as a short value.
		If the specified field contains a numeric type
		larger than a short the value may be truncated
	Params
		fieldNum - the number of the field to get (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		value - a reference to the variable that will hold the
			retrieved data
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::GetField(int fieldNum,short& value){

	COleVariant cv;

	//get the field as a variant
	if(GetField(fieldNum,cv) == FALSE)
		return FALSE;
	
	//check to see if the data type returned can be converted to a short
	switch(cv.vt){
		case VT_UI1://unsigned char
			value = cv.bVal;
			return TRUE;
		case VT_BOOL://boolean
			value = cv.iVal;
			return TRUE;
		case VT_I2://short or 16bit integer
			value = cv.iVal;
			return TRUE;
		case VT_I4://long or 32bit integer
			value = (short)cv.lVal;
			TRACE(_T("COXDao::GetField - Possible Truncation, short=long"));
			return TRUE;
		case VT_R4://float
			value = (short)cv.fltVal;
			TRACE(_T("COXDao::GetField - Possible Truncation, short=float"));
			return TRUE;
		case VT_R8://double
			value = (short)cv.dblVal;
			TRACE(_T("COXDao::GetField - Possible Truncation, short=double"));
			return TRUE;
	}

	//conversion failed, so set an error code
	SetErrorCode(7);
	
	return FALSE;
}

/***************************************************
SetField
	Purpose
		Sets the specified field as a short value. conversion
		is automatic for compatible field data types.
	Params
		fieldNum - the number of the field to set (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		value - data to set into the specified field
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::SetField(int fieldNum,short value){

	//turn the short into a variant
	COleVariant cv(value,VT_I2);

	//set the variant
	return SetField(fieldNum,cv);
}

/***************************************************
GetField
	Purpose
		Retrieves the specified field as a BOOL value.
	Params
		fieldNum - the number of the field to get (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		value - a reference to the variable that will hold the
			retrieved data
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::GetField(int fieldNum, BOOL& value){

	COleVariant cv;

	//get the field as a variant
	if(GetField(fieldNum,cv) == FALSE)
		return FALSE;
	
	//check to see if the data type returned can be converted to a short
	switch(cv.vt){
		case VT_UI1://unsigned char
			value = cv.bVal!=0 ? TRUE : FALSE;
			return TRUE;
		case VT_BOOL://boolean
			value = cv.iVal!=0 ? TRUE : FALSE;
			return TRUE;
		case VT_I2://short or 16bit integer
			value = cv.iVal!=0 ? TRUE : FALSE;
			return TRUE;
		case VT_I4://long or 32bit integer
			value = cv.lVal!=0 ? TRUE : FALSE;
			return TRUE;
		case VT_R4://float
			value = cv.fltVal!=0 ? TRUE : FALSE;
			return TRUE;
		case VT_R8://double
			value = cv.dblVal!=0 ? TRUE : FALSE;
			return TRUE;
	}

	//conversion failed, so set an error code
	SetErrorCode(7);
	
	return FALSE;
}

/***************************************************
SetField
	Purpose
		Sets the specified field as a BOOL value. conversion
		is automatic for compatible field data types.
	Params
		fieldNum - the number of the field to set (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		value - data to set into the specified field
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::SetField(int fieldNum, BOOL value){

	//turn the short into a variant
	COleVariant cv((BYTE)value);

	//set the variant
	return SetField(fieldNum,cv);
}

/***************************************************
GetField
	Purpose
		Retrieves the specified field as a long value.
		If the specified field contains a numeric type
		larger than a short the value may be truncated
	Params
		fieldNum - the number of the field to get (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		value - a reference to the variable that will hold the
			retrieved data
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::GetField(int fieldNum,long& value){

	COleVariant cv;

	//get the field as a variant
	if(GetField(fieldNum,cv) == FALSE)
		return FALSE;

	//check to see if the data type returned can be converted to a long
	switch(cv.vt){
		case VT_UI1://unsigned char
			value = cv.bVal;
			return TRUE;
		case VT_BOOL://boolean
			value = cv.iVal;
			return TRUE;
		case VT_I2://short or 16bit integer
			value = cv.iVal;
			return TRUE;
		case VT_I4://long or 32bit integer
			value = cv.lVal;
			return TRUE;
		case VT_R4://float
			value = (long)cv.fltVal;
			TRACE(_T("COXDao::GetField - Possible Truncation, long=float"));
			return TRUE;
		case VT_R8://double
			value = (long)cv.dblVal;
			TRACE(_T("COXDao::GetField - Possible Truncation, long=double"));
			return TRUE;
	}

	//conversion failed, so set an error code
	SetErrorCode(8);

	return FALSE;
}

/***************************************************
SetField
	Purpose
		Sets the specified field as a short long. conversion
		is automatic for compatible field data types.
	Params
		fieldNum - the number of the field to set (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		value - data to set into the specified field
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::SetField(int fieldNum,long value){

	//turn the long into a variant
	COleVariant cv(value,VT_I4);

	//set the variant
	return SetField(fieldNum,cv);
}

/***************************************************
GetField
	Purpose
		Retrieves the specified field as a double value.
		Fields containing smaller numeric data types 
		are converted.
	Params
		fieldNum - the number of the field to get (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		value - a reference to the variable that will hold the
			retrieved data
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::GetField(int fieldNum,double& value){

	COleVariant cv;

	//get the field as a variant
	if(GetField(fieldNum,cv) == FALSE){
		return FALSE;
	}

	//check to see if the data returned can be converted into a double
	switch(cv.vt){
		case VT_UI1://unsigned char
			value = cv.bVal;
			return TRUE;
		case VT_BOOL://boolean
			value = cv.iVal;
			return TRUE;
		case VT_I2://short or 16bit integer
			value = cv.iVal;
			return TRUE;
		case VT_I4://long or 32bit integer
			value = cv.lVal;
			return TRUE;
		case VT_R4://float
			value = cv.fltVal;
			return TRUE;
		case VT_R8://double
			value = cv.dblVal;
			return TRUE;
	}

	//conversion failed, so set an error code
	SetErrorCode(9);

	return FALSE;
}

/***************************************************
SetField
	Purpose
		Sets the specified field as a short double. conversion
		is automatic for compatible field data types.
	Params
		fieldNum - the number of the field to set (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		value - data to set into the specified field
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::SetField(int fieldNum,double value){

	//change the double into a variant
	COleVariant cv(value);

	//set the variant into the field
	return SetField(fieldNum,cv);
}

/***************************************************
GetField
	Purpose
		Retrieves the specified field as a double value.
		Fields containing smaller numeric data types 
		are converted.
	Params
		fieldNum - the number of the field to get (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		value - a reference to the variable that will hold the
			retrieved data
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::GetField(int fieldNum,float& value){

	COleVariant cv;

	//get the field as a variant
	if(GetField(fieldNum,cv) == FALSE){
		return FALSE;
	}

	//check to see if the data returned can be converted into a float
	switch(cv.vt){
		case VT_UI1://unsigned char
			value = cv.bVal;
			return TRUE;
		case VT_BOOL://boolean
			value = cv.iVal;
			return TRUE;
		case VT_I2://short or 16bit integer
			value = cv.iVal;
			return TRUE;
		case VT_I4://long or 32bit integer
			value = (float)cv.lVal;
			TRACE(_T("COXDao::GetField - Possible Truncation, float=long"));
			return TRUE;
		case VT_R4://float
			value = cv.fltVal;
			return TRUE;
		case VT_R8://double
			value = (float)cv.dblVal;
			TRACE(_T("COXDao::GetField - Possible Truncation, float=double"));
			return TRUE;
	}

	//conversion failed, so set an error code
	SetErrorCode(9);

	return FALSE;
}

/***************************************************
SetField
	Purpose
		Sets the specified field as a float. conversion
		is automatic for compatible field data types.
	Params
		fieldNum - the number of the field to set (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		value - data to set into the specified field
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::SetField(int fieldNum, float value){

	//change the double into a variant
	COleVariant cv(value);

	//set the variant into the field
	return SetField(fieldNum,cv);
}

/***************************************************
GetField
	Purpose
		Retrieves the specified field as a 
		COleDataTime object. If the field specified
		is not a date type then the function will fail.
	Params
		fieldNum - the number of the field to get (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		var - a reference to the COleDataTime object that will
			hold the retrieved data
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::GetField(int fieldNum,COleDateTime& var){

	COleVariant cv;

	//get the field as a variant
	if(GetField(fieldNum,cv) == FALSE){
		return FALSE;
	}
	
	//change the variant into a COleDataTime object
	var =  cv;
	
	//check to see if the conversion was successful
	if(var.GetStatus() == var.invalid)
		return FALSE;

	return TRUE;
}

/***************************************************
SetField
	Purpose
		Sets the specified field using a
		COleDataTime object. If the field specified
		is not a date type then the function will fail.
	Params
		fieldNum - the number of the field to set (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		var - a COleDataTime object that will be set into
			the specified field
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::SetField(int fieldNum,COleDateTime value){

	//change the datatime object into a variant
	COleVariant cv(value);

	//set the variant
	return SetField(fieldNum,cv);
}

/***************************************************
GetField
	Purpose
		Retrieves the specified field as a 
		COleCurrency object. If the field specified
		is not a currency then the function will fail.
	Params
		fieldNum - the number of the field to get (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		var - a reference to the COleCurrency object that will
			hold the retrieved data
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::GetField(int fieldNum,COleCurrency& var){
	
	COleVariant cv;

	//get the field as a variant
	if(GetField(fieldNum,cv) == FALSE){
		return FALSE;
	}
	
	//change the variant into a COleCurrency Object
	var =  cv;

	//check to see if the conversion worked
	if(var.GetStatus() == var.invalid )
		return FALSE;

	return TRUE;
}

/***************************************************
SetField
	Purpose
		Sets the specified field using a
		COleCurrency object. If the field specified
		is not a currency type then the function will fail.
	Params
		fieldNum - the number of the field to set (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		var - a COlecurrency object that will be set into
			the specified field
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::SetField(int fieldNum,COleCurrency value){

	//convert the currency into a variant
	COleVariant cv(value);

	//set the variant
	return SetField(fieldNum,cv);
}

/***************************************************
GetField
	Purpose
		Retrieves the specified field as a 
		COleVariant object. This is the native format 
		for all DAO data. All types can be retrieved 
		with this type.
	Params
		fieldNum - the number of the field to get (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		var - a reference to the COleVariant object that will
			hold the retrieved data
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::GetField(int fieldNum,COleVariant& var){

	ASSERT(m_recordSet);

	//check to see that the given fieldNum is witin range
	if(fieldNum < 0 || fieldNum >= m_recordSet->GetFieldCount()){
		SetErrorCode(10);
		return FALSE;
	}

	//check to see if the current record is past the end
	if(m_recordSet->IsEOF()){
		SetErrorCode(11);
		return FALSE;
	}

	//try and get the data from the field, on error set the error code
	try{
		var = m_recordSet->GetFieldValue(fieldNum);
	}
	catch(CMemoryException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		AfxAbort();
	}
	catch(CDaoException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		SetErrorCode(12);
		return FALSE;
	}
	return TRUE;
}

/***************************************************
SetField
	Purpose
		Sets the specified field as a COleVariant object. 
		This is the native format for all DAO data. All 
		types can be set with this type.
	Params
		fieldNum - the number of the field to set (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		var - the COleVariant object that will be used to set
			the specified field
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::SetField(int fieldNum,COleVariant var){

	ASSERT(m_recordSet);

	//check to see if the specified field is within range
	if(fieldNum < 0 || fieldNum >= m_recordSet->GetFieldCount()){
		SetErrorCode(14);
		return FALSE;
	}

	//check to see if the specified field can be updated
	if((m_fields[fieldNum].m_lAttributes&dbUpdatableField) ==0){
		SetErrorCode(13);
		return FALSE;
	}

	//try and set the field value, on error set the error code
	try{
		if(var.vt & VT_EMPTY)
			m_recordSet->SetFieldValueNull(fieldNum);
		else
			m_recordSet->SetFieldValue(fieldNum,var);
	}
	catch(CMemoryException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		AfxAbort();
	}
	catch(CDaoException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		SetErrorCode(15);
		return FALSE;
	}

	return TRUE;
}

/***************************************************
GetField
	Purpose
		Retrieves the specified field as a string.
		Most data types are automatically converted
		to strings if needed.
	Params
		fieldNum - the number of the field to get (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		str - a reference to string that will hold the retrieved 
			data
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::GetField(int fieldNum,CString &str){

	COleVariant cv;

	//get the specified field as a variant
	if(GetField(fieldNum,cv) == FALSE)
		return FALSE;

	//retrieve the data from the variant
	switch(cv.vt){
		case VT_EMPTY:
			str = "";
			return TRUE;
		case VT_BSTR:
			str = ((TCHAR *)cv.bstrVal);
			return TRUE;
		case VT_UI1:
			str.Format(_T("%d"),cv.bVal);
			return TRUE;
		case VT_I2:
			str.Format(_T("%d"),(int)cv.iVal);
			return TRUE;
		case VT_I4:
			str.Format(_T("%ld"),cv.lVal);
			return TRUE;
		case VT_R4:
			str.Format(_T("%f"),cv.fltVal);
			return TRUE;
		case VT_R8:
			TCHAR buf[64];
			UTBStr::stprintf(buf, 64, _T("%lf"),(double)cv.dblVal);
			str = buf;
			return TRUE;
		case VT_BOOL:
			if(cv.iVal)
				str = "1";
			else
				str = "0";
			return TRUE;
		case VT_CY:{
			COleCurrency cur(cv.cyVal);
			str = cur.Format(LOCALE_NOUSEROVERRIDE,LANG_USER_DEFAULT);
			return TRUE;
		}
		case VT_DATE:{
			COleDateTime dt(cv.date);
			str = dt.Format();
			return TRUE;
		}
 		case VT_NULL:
			str = "";
			return TRUE;
		default:
			str =  "*unknown*";
	}
	
	//if the data could not be converted, set the error code
	SetErrorCode(16);

	return FALSE;
}

/***************************************************
SetField
	Purpose
		Sets the specified field as a string.
		Most data types are automatically converted
		to if needed.
	Params
		fieldNum - the number of the field to set (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		str - the string that will used to set the specified
			field with
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::SetField(int fieldNum,LPCTSTR str){

	//convert the string into a variant
	#ifdef __UNICODE__
	COleVariant cv(str,VT_BSTR);
	#else
	COleVariant cv(str,VT_BSTRT);
	#endif

	//set the variant
	if(SetField(fieldNum,cv) == FALSE)
		return FALSE;

	return TRUE;

}

/***************************************************
GetField
	Purpose
		Retrieves the specified field as a string.
		Most data types are automatically converted
		to strings if needed.
	Params
		fieldName - the name of the field to get
		str - a reference to string that will hold the retrieved 
			data
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::GetField(LPCTSTR fieldName,CString& str){
	
	//retrieve the field number from the name
	int fieldNum = GetFieldNumber(fieldName);
	if(fieldNum <0)
		return FALSE;

	//get the field
	return GetField(fieldNum,str);
}

/***************************************************
SetField
	Purpose
		Sets the specified field as a string.
		Most data types are automatically converted
		to if needed.
	Params
		fieldName - the name of the field to set.
		str - the string that will used to set the specified
			field with
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::SetField(LPCTSTR fieldName,LPCTSTR str){

	//retrieve the field number from the name
	int fieldNum = GetFieldNumber(fieldName);
	if(fieldNum <0)
		return FALSE;

	//set the field
	return SetField(fieldNum,str);
}

/***************************************************
GetNumberRows
	Purpose
		Returns the approximate number of rows in 
		the currently opened recordset. This function
		more accuratly indicates the number of records
		accessed.
	Params
		none
	Return 
		The number of records found
****************************************************/
long COXDao::GetNumberRows(){
	
	ASSERT(m_recordSet);

	//check to see if the recordset is empty
	if(m_recordSet->IsEOF() && m_recordSet->IsBOF()){	
		return 0;
	}

	//get the current count
	long num = m_recordSet->GetRecordCount();
	if(num <1)
		num = 1;

	return num;

}

/***************************************************
GetNumberFields
	Purpose
		Returns the number of fields in the current
		recordset
	Params
		none
	Return 
		The number of fields
****************************************************/
int COXDao::GetNumberFields(){
	
	ASSERT(m_recordSet);
	
	//get the number of fields
	return m_recordSet->GetFieldCount();
}


/***************************************************
GetFieldNumber
	Purpose
		Returns the corresponding field number for a
		given field name. A -1 is returned on failure.
		Since this function may be used often a shorter
		macro can be used: GFN(name)
	Params
		name - the name of the field to get the ordinal of
	Return 
		returns the field ordinal for the given field name.
		-1 is returned if the field can not be found.
****************************************************/
int COXDao::GetFieldNumber(LPCTSTR name){

	ASSERT(m_recordSet);

	//get the number of fields
	int numFields = m_recordSet->GetFieldCount();

	//find the field ordinal by comparing each field
	//string with the given field string
	for(int loop = 0; loop < numFields;loop++){
		//compare the strings
		if(lstrcmp(m_fields[loop].m_strName,name) == 0)
			//return the field ordinal
			return loop;
	}

	//if a match was not found then set the error code
	SetErrorCode(19);

	return -1;
}

/***************************************************
GetFieldName
	Purpose
		Returns the name of a given field ordinal
	Params
		fieldNum - ordinal of the field (0 indexed)
	Return 
		constant string containing the field name
		NULL on error , call GetLastError for extended 
		information	none
****************************************************/
LPCTSTR COXDao::GetFieldName(int fieldNum){
	
	ASSERT(m_recordSet);
	
	//make sure that the given field number is within range
	if(fieldNum < 0 || fieldNum >= m_recordSet->GetFieldCount()){
		//if it is out of range then set the error code
		SetErrorCode(20);

		return NULL;
	}

	//return the field name for the given number
	return (LPCTSTR)m_fields[fieldNum].m_strName;
}

/***************************************************
GetFieldInfo
	Purpose
		Returns a pointer to a CDaoFieldObject for the 
		given field.This object contains all of the fields
		meta data
	Params
		fieldNum - the number of the field (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
	Return 
		none
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
CDaoFieldInfo* COXDao::GetFieldInfo(int fieldNum){

	ASSERT(m_recordSet);
	
	//make sure that the given field number is within range
	if(fieldNum < 0 || fieldNum >= m_recordSet->GetFieldCount()){
		//if it is out of range then set the error code
		SetErrorCode(21);

		return NULL;
	}

	//return the CDaoFieldInfo for the given field
	return &m_fields[fieldNum];
}

/***************************************************
FindFirst
	Purpose
		Finds the first record that matches the given
		string. Many field types are automatically
		converted to string if needed. If a match
		is found then the recordset is moved to the
		record found
	Params
		fieldNum - the number of the field (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		string - string to find.
	Return 
		none
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::FindFirst(int fieldNum,CString &string){

	BOOL result=FALSE;
	CString srch;

	//create the search string
///	srch.Format(_T("%s = %s"),m_fields[fieldNum].m_strName,string);			
	if (m_fields[fieldNum].m_nType == dbText)
		srch.Format(_T("[%s] = '%s'"),m_fields[fieldNum].m_strName,string); 
	else
		srch.Format(_T("[%s] = %s"),m_fields[fieldNum].m_strName,string); 

	//try the search
	try{
		result = m_recordSet->Find(AFX_DAO_FIRST, srch);
	}
	catch(CMemoryException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		AfxAbort();
	}
	catch(CDaoException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		SetErrorCode(22);
		return FALSE;
	}

	return result;
}

/***************************************************
FindNext
	Purpose
		Finds the next record that matches the given
		string. Many field types are automatically
		converted to string if needed. If a match
		is found then the recordset is moved to the
		record found
	Params
		fieldNum - the number of the field (0 indexed)
			(use GetFieldNumber or GFN to translate field names)
		string - string to find.
	Return 
		none
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::FindNext(int fieldNum,CString &string){

	BOOL result=FALSE;
	CString srch;

	//create the search string
	//srch.Format(_T("%s = %s"),m_fields[fieldNum].m_strName,string);
	if (m_fields[fieldNum].m_nType == dbText)
		srch.Format(_T("[%s] = '%s'"),m_fields[fieldNum].m_strName,string); 
	else
		srch.Format(_T("[%s] = %s"),m_fields[fieldNum].m_strName,string); 

	//try the search
	try{
		result = m_recordSet->Find(AFX_DAO_NEXT, srch);
	}
	catch(CMemoryException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		AfxAbort();
	}
	catch(CDaoException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		SetErrorCode(23);
		return FALSE;
	}

	return result;
}

/***************************************************
GetRecordSet
	Purpose
		Returns the pointer to the classes internal
		recordset. This member is exposed so that
		other CDaoRecordset functions may be used.
	Params
		none
	Return 
		pointer to the CDaoRecordset object
		NULL if the recordset is not opened
****************************************************/
CDaoRecordset* COXDao::GetRecordSet(){
	
	return m_recordSet;
}

/***************************************************
GetDatabase
	Purpose
		Returns the pointer to the classes internal
		recordset. This member is exposed so that
		other CDaoDatabase functions may be used.
	Params
		none
	Return 
		pointer to the CDaoDatabase object
		NULL if the database is not opened
****************************************************/
CDaoDatabase* COXDao::GetDatabase(){
	
	return m_database;
}

/***************************************************
GetTableDef
	Purpose
		Returns the pointer to the classes internal
		recordset. This member is exposed so that
		other CDaoTableDef functions may be used.
	Params
		none
	Return 
		pointer to the CDaoTableDef object
		NULL if the database is not opened
****************************************************/
CDaoTableDef* COXDao::GetTableDef(){
	
	return m_tableDef;
}

/***************************************************
GetDaoQueryDef
	Purpose
		Returns the pointer to the classes internal
		recordset. This member is exposed so that
		other CDaoQueryDef functions may be used.
	Params
		none
	Return 
		pointer to the CDaoQueryDef object
		NULL if the database is not opened
****************************************************/
CDaoQueryDef* COXDao::GetQueryDef(){
	
	return m_queryDef;
}

/***************************************************
MoveFirst
	Purpose
		Moves the recordset to the first record. Use 
		IsEOF afterward to determine if a record exists.
	Params
		none
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::MoveFirst(){

	ASSERT(m_recordSet);

	//try and move to the first record, on failure the error code is set
	try{
		m_recordSet->MoveFirst();
	}
	catch(CMemoryException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		AfxAbort();
	}
	catch(CDaoException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		SetErrorCode(24);
		return FALSE;
	}
	return TRUE;
}

/***************************************************
MoveNext
	Purpose
		Moves the recordset to the next available record.
		Use IsEOF afterward to determine if a record exists.
		This function will fail if the last record is already
		reached.
	Params
		none
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::MoveNext(){

	ASSERT(m_recordSet);

	//try and move to the next record, on failure the error code is set
	try{
		m_recordSet->MoveNext();
	}
	catch(CMemoryException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		AfxAbort();
	}
	catch(CDaoException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		SetErrorCode(25);
		return FALSE;
	}
	return TRUE;
}

/***************************************************
MovePrev
	Purpose
		Moves the recordset to the previous record.
		This function will fail if the currenct record
		is already at the start of the recordset; Use
		IsBOF to determine this.
	Params
		none
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::MovePrev(){

	ASSERT(m_recordSet);

	//try and move to the previous record, on failure the error code is set
	try{
		m_recordSet->MovePrev();
	}
	catch(CMemoryException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		AfxAbort();
	}
	catch(CDaoException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		SetErrorCode(26);
		return FALSE;
	}
	return TRUE;
}

/***************************************************
MovePrev
	Purpose
		Moves the recordset to the last record in the
		set.
	Params
		none
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
****************************************************/
BOOL COXDao::MoveLast(){

	ASSERT(m_recordSet);

	//try and move to the last record, on failure the error code is set
	try{
		m_recordSet->MoveLast();
	}
	catch(CMemoryException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		AfxAbort();
	}
	catch(CDaoException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		SetErrorCode(27);
		return FALSE;
	}
	return TRUE;
}

/***************************************************
IsEOF
	Purpose
		Use this function to determine if the record
		set is on the last record
	Params
		none
	Return 
		TRUE if the recordset is on the last record
		otherwise FALSE
****************************************************/
BOOL COXDao::IsEOF(){
	
	ASSERT(m_recordSet);

	//check to see if the recordset is at the end
	return m_recordSet->IsEOF();
}

/***************************************************
IsBOF
	Purpose
		Use this function to determine if the record
		set is on the first record
	Params
		none
	Return 
		TRUE if the recordset is on the first record
		otherwise FALSE
****************************************************/
BOOL COXDao::IsBOF(){
	
	ASSERT(m_recordSet);

	//check to see if the recordset is as the start
	return m_recordSet->IsBOF();
}

/***************************************************
Update
	Purpose
		Updates the current record. Use the Add or Edit
		functions to initiate the updating process first.
	Params
		none
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
		none
****************************************************/
BOOL COXDao::Update(){

	ASSERT(m_recordSet);

	//try and finish 'Add' or 'Edit', on failure set the error code
	try{
		m_recordSet->Update();
	}
	catch(CMemoryException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		AfxAbort();
	}
	catch(CDaoException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		SetErrorCode(28);
		return FALSE;
	}
	return TRUE;
}

/***************************************************
CancelUpdate
	Purpose
		Cancels any pending Add or Edit.
	Params
		none
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
		none
****************************************************/
BOOL COXDao::CancelUpdate(){

	ASSERT(m_recordSet);

	//try and cancel the 'Add' or 'Edit', on failure set the error code
	try{
		m_recordSet->CancelUpdate();
	}
	catch(CMemoryException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		AfxAbort();
	}
	catch(CDaoException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		SetErrorCode(29);
		return FALSE;
	}
	return TRUE;
}

/***************************************************
Edit
	Purpose
		Puts the current record into edit mode. Once
		in edit mode the fields can be updated using 
		the SetField functions. Once finished call
		Upate to save the changes or CancelUpdate to
		cancel.
	Params
		none
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
		none
****************************************************/
BOOL COXDao::Edit(){

	ASSERT(m_recordSet);

	//try and start editing the current record, on failure set the error code
	try{
		m_recordSet->Edit();
	}
	catch(CMemoryException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		AfxAbort();
	}
	catch(CDaoException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		SetErrorCode(30);
		return FALSE;
	}
	return TRUE;
}

/***************************************************
Add
	Purpose
		Starts the process of adding a new record to
		the current record set. Once this function is
		called successfully the fields can be updated 
		using the SetField functions. Once finished call
		Upate to finish the changes or CancelUpdate to
		cancel.
	Params
		none
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
		none
****************************************************/
BOOL COXDao::Add(){

	ASSERT(m_recordSet);

	//check to see if appending is allowed
	if(m_recordSet->CanAppend()==0)
		return FALSE;

	//try and start appending a record, on failure set the error code
	try{
		m_recordSet->AddNew();
	}
	catch(CMemoryException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		AfxAbort();
	}
	catch(CDaoException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		SetErrorCode(31);
		return FALSE;
	}
	return TRUE;
}

/***************************************************
Delete
	Purpose
		Deletes the current record. Then moves to the
		next record if possible.
	Params
		none
	Return 
		TRUE - success
		FALSE - error, call GetLastError for extended information
		none
****************************************************/
BOOL COXDao::Delete(){

	ASSERT(m_recordSet);
	
	//try and delete the current record, on failure set the error code
	try{
		m_recordSet->Delete();
	}
	catch(CMemoryException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		AfxAbort();
	}
	catch(CDaoException* e){
		#ifdef UG_DAO_REPORTERROR
			e->ReportError();
		#endif
		e->Delete();
		SetErrorCode(32);
		return FALSE;
	}

	try{
		m_recordSet->MoveNext();
	}
	catch(CDaoException* e){
		e->Delete();
		try{
			m_recordSet->MovePrev();
		}
		catch(CDaoException* e){
			e->Delete();
			m_recordSet->MoveFirst();
		}
	}
	return TRUE;
}

/***************************************************
SetPassword
	Purpose
		Sets the password string that will be used
		during the open process. Passwords should 
		only be used if they are needed by the 
		database being opened. Use "" if clear
		the password setting.
	Params
		pass - the password to set
	Return 
		TRUE - success
		FALSE - failure
****************************************************/
BOOL COXDao::SetPassword(LPCTSTR pass){
	
	//store the given password
	m_password = pass;

	return TRUE;
}
