
// Version: 9.3

/***************************************************
MFC DAO FrameWork Wrapper Class
Dundas Software Ltd. 1997 - 2003
****************************************************/

#ifndef COXDaoClassDefined
#define COXDaoClassDefined

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"
#include "OXMainRes.h"

#ifndef __AFXDAO_H
	#include <afxdao.h>
#endif

#pragma warning(disable : 4995)

#define GFN(nameStr) GetFieldNumber(nameStr)


class OX_CLASS_DECL COXDao
{

protected:
	
	CDaoDatabase* 	m_database;
	CDaoTableDef*	m_tableDef;
	CDaoQueryDef*	m_queryDef;
	CDaoRecordset*	m_recordSet;
	CDaoFieldInfo*  m_fields;

	CString			m_password;

	int				m_lastErrorCode;
	CString			m_lastErrorString;

	void GatherFieldInfo();
	void SetErrorCode(int code);

public:

	COXDao();
	virtual ~COXDao();

	BOOL	Open(LPCTSTR database,LPCTSTR table);
	BOOL	OpenSQL(LPCTSTR database,LPCTSTR sql);
	BOOL	Open(LPCTSTR database,LPCTSTR table,int openType,
				 LPCTSTR sql,int options=0,CString optStr=_T(""));
	BOOL	Close();

	inline	BOOL	IsOpen() const { 
		return (m_database!=NULL ? m_database->IsOpen() : FALSE); 
	}
	inline	BOOL	IsOpenRS() const { 
		return (m_recordSet!=NULL ? m_recordSet->IsOpen() : FALSE); 
	}

	BOOL	GetField(int fieldNum,CString& data);
	BOOL	SetField(int fieldNum,LPCTSTR data);
	
	BOOL	GetField(int fieldNum,short& value);
	BOOL	SetField(int fieldNum,short value);

	BOOL	GetField(int fieldNum,long& value);
	BOOL	SetField(int fieldNum,long value);

	BOOL	GetField(int fieldNum,double& value);
	BOOL	SetField(int fieldNum,double value);

	BOOL	GetField(int fieldNum,float& value);
	BOOL	SetField(int fieldNum,float value);

	BOOL	GetField(int fieldNum,COleDateTime& var);
	BOOL	SetField(int fieldNum,COleDateTime value);

	BOOL	GetField(int fieldNum,COleCurrency& var);
	BOOL	SetField(int fieldNum,COleCurrency value);

	BOOL	GetField(int fieldNum,COleVariant& var);
	BOOL	SetField(int fieldNum,COleVariant var);

	BOOL	GetField(LPCTSTR fieldName,CString& str);
	BOOL	SetField(LPCTSTR fieldName,LPCTSTR str);
	
	BOOL	GetField(int fieldNum,BOOL& value);
	BOOL	SetField(int fieldNum,BOOL value);

	int		GetFieldNumber(LPCTSTR);
	LPCTSTR GetFieldName(int fieldNum);

	long	GetNumberRows();
	int		GetNumberFields();


	BOOL	FindFirst(int fieldNum,CString &string);
	BOOL	FindNext(int fieldNum,CString &string);
	
	BOOL	MoveNext();
	BOOL	MovePrev();
	BOOL	MoveFirst();
	BOOL	MoveLast();

	BOOL	Edit();
	BOOL	Add();
	BOOL	CancelUpdate();
	BOOL	Update();
	BOOL	Delete();

	BOOL	IsEOF();
	BOOL	IsBOF();

	BOOL	SetPassword(LPCTSTR password);

	int		GetLastError();
	LPCTSTR GetLastErrorText();

	CDaoRecordset*	GetRecordSet();
	CDaoDatabase*	GetDatabase();
	CDaoFieldInfo*	GetFieldInfo(int fieldNum);
	CDaoTableDef*	GetTableDef();
	CDaoQueryDef*	GetQueryDef();

};


// error code strings
static LPCTSTR COXDao_errorStrings[] = {	
	_T("COXDao::Open, Exception Thrown during CDAODatabase::Open"),			//0
	_T("COXDao::Open, table and sql parameters both NULL"),					//1
	_T("COXDao::Open, Exception thrown during CDAOTableDef::Open"),			//2
	_T("COXDao::Open, Exception thrown during CDAORecordset::Open"),			//3
	_T("COXDao::Open, Exception thrown during CDAOQueryDef::Open"),			//4
	_T("COXDao::Open, Exception thrown during CDAORecordset::Open"),			//5
	_T("COXDao::Open, Exception thrown during CDaoRecordset::MoveFirst"),		//6
	_T("COXDao::GetField, returned data not compatible with type short"),		//7
	_T("COXDao::GetField, returned data not compatible with type long"),		//8
	_T("COXDao::GetField, returned data not compatible with type double"),	//9
	_T("COXDao::GetField, specified field number is out of range"),			//10
	_T("COXDao::GetField, recordset is not on a valid record"),				//11
	_T("COXDao::GetField, Exception thrown during CDAORecordset::GetFieldValue"), //12
	_T("COXDao::SetField, specified field cannot be updated"),				//13
	_T("COXDao::SetField, specified field number is out of range"),			//14
	_T("COXDao::SetField, Exception thrown during CDAORecordset::SetFieldValue"), //15
	_T("COXDao::GetField, returned data not compatible with type CString"),	//16
	_T("NA"),																//17
	_T("COXDao::GetNumRows, Exception thrown during CDAORecordset::MoveNext"),//18
	_T("COXDao::GetFieldNumber, field name not found in table"),				//19
	_T("COXDao::GetFieldName, specified field number is out of range"),		//20
	_T("COXDao::GetFieldInfo, specified field number is out of range"),		//21
	_T("COXDao::FindFirst, Exception thrown during CDAORecordset::FindFirst"),//22
	_T("COXDao::FindNext, Exception thrown during CDAORecordset::FindNext"),	//23
	_T("COXDao::MoveFirst, Exception thrown during CDaoRecordset::MoveFirst"),//24
	_T("COXDao::MoveNext, Exception thrown during CDaoRecordset::MoveNext"),	//25
	_T("COXDao::MovePrev, Exception thrown during CDaoRecordset::MovePrev"),	//26
	_T("COXDao::MoveLast, Exception thrown during CDaoRecordset::MoveLast"),	//27
	_T("COXDao::Update, Exception thrown during CDaoRecordset::Update"),		//28
	_T("COXDao::CancelUpdate, Exception thrown during CDaoRecordset::CancelUpdate"), //29
	_T("COXDao::Edit, Exception thrown during CDaoRecordset::Edit"),			//30
	_T("COXDao::Add, Exception thrown during CDaoRecordset::AddNew"),			//31
	_T("COXDao::Delete, Exception thrown during CDaoRecordset::Delete"),		//31
	_T("")
	};

#endif
